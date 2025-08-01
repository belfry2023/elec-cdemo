#include "dmmotor.h"
#include "memory.h"
#include "general_def.h"
#include "user_lib.h"
#include "cmsis_os.h"
#include "string.h"
#include "daemon.h"
#include "stdlib.h"


static uint8_t idx;
static DMMotorInstance *dm_motor_instance[DM_MOTOR_CNT];
static osThreadId dm_task_handle[DM_MOTOR_CNT];
/* 两个用于将uint值和float值进行映射的函数,在设定发送值和解析反馈值时使用 */


static void DMMotorSetMode(DMMotor_Mode_e cmd, DMMotorInstance *motor)
{
    memset(motor->motor_can_instace->tx_buff, 0xff, 7);  // 发送电机指令的时候前面7bytes都是0xff
    motor->motor_can_instace->tx_buff[7] = (uint8_t)cmd; // 最后一位是命令id
    CANTransmit(motor->motor_can_instace, 1);
}

static void DMMotorDecode(CANInstance *motor_can)
{
    uint16_t tmp; // 用于暂存解析值,稍后转换成float数据,避免多次创建临时变量
    uint8_t *rxbuff = motor_can->rx_buff;
    DMMotorInstance *motor = (DMMotorInstance *)motor_can->id;
    DM_Motor_Measure_s *measure = &(motor->measure); // 将can实例中保存的id转换成电机实例的指针

    DaemonReload(motor->motor_daemon);

    measure->last_position = measure->position;
    tmp = (uint16_t)((rxbuff[1] << 8) | rxbuff[2]);
    measure->position = uint_to_float(tmp, DM_P_MIN, DM_P_MAX, 16);

    tmp = (uint16_t)((rxbuff[3] << 4) | rxbuff[4] >> 4);
    measure->velocity = uint_to_float(tmp, DM_V_MIN, DM_V_MAX, 12);

    tmp = (uint16_t)(((rxbuff[4] & 0x0f) << 8) | rxbuff[5]);
    measure->torque = uint_to_float(tmp, DM_T_MIN, DM_T_MAX, 12);

    measure->T_Mos = (float)rxbuff[6];
    measure->T_Rotor = (float)rxbuff[7];
}

static void DMMotorLostCallback(void *motor_ptr)
{
}
void DMMotorCaliEncoder(DMMotorInstance *motor)
{
    DMMotorSetMode(DM_CMD_ZERO_POSITION, motor);
    DWT_Delay(0.1);
}
DMMotorInstance *DMMotorInit(DMMotor_Init_Config_s *config)
{
    DMMotorInstance *motor = (DMMotorInstance *)malloc(sizeof(DMMotorInstance));
    memset(motor, 0, sizeof(DMMotorInstance));
    
    motor->work_mode = config->work_mode;
    motor->Custom_Controller.motor_settings = config->controller_setting_init_config;
    PIDInit(&motor->Custom_Controller.angle_PID, &config->angle_PID);
    motor->Custom_Controller.other_angle_feedback_ptr = config->other_angle_feedback_ptr;
    switch (motor->work_mode)
    {
    case 0b00000000:
        /* code */
        motor->Universal_Controller.MIT_Control_Setting.Kp = 3;
        motor->Universal_Controller.MIT_Control_Setting.Kd = 1;
        motor->Custom_Controller.motor_settings.outer_loop_type = OPEN_LOOP;
        motor->Custom_Controller.motor_settings.close_loop_type = OPEN_LOOP;
        break;
    case 0b00000001:
        motor->Universal_Controller.MIT_Control_Setting.Kp = 3;
        motor->Universal_Controller.MIT_Control_Setting.Kd = 1;
        motor->Custom_Controller.motor_settings.outer_loop_type = OPEN_LOOP;
        motor->Custom_Controller.motor_settings.close_loop_type = OPEN_LOOP;
        break;
    case 0b00000011:
        motor->Universal_Controller.MIT_Control_Setting.Kp = 0;
        motor->Universal_Controller.MIT_Control_Setting.Kd = 1;
        motor->Custom_Controller.motor_settings.outer_loop_type = ANGLE_LOOP;
        motor->Custom_Controller.motor_settings.close_loop_type = ANGLE_LOOP;
        break;
    default:
        motor->Universal_Controller.MIT_Control_Setting.Kp = 0;
        motor->Universal_Controller.MIT_Control_Setting.Kd = 1;
        motor->Custom_Controller.motor_settings.outer_loop_type = ANGLE_LOOP;
        motor->Custom_Controller.motor_settings.close_loop_type = ANGLE_LOOP;
        break;
    }
    config->can_init_config.can_module_callback = DMMotorDecode;
    config->can_init_config.id = motor;
    motor->motor_can_instace = CANRegister(&config->can_init_config);

    Daemon_Init_Config_s conf = {
        .callback = DMMotorLostCallback,
        .owner_id = motor,
        .reload_count = 10,
    };
    motor->motor_daemon = DaemonRegister(&conf);

    DMMotorEnable(motor);
    DMMotorSetMode(DM_CMD_MOTOR_MODE, motor);
    DWT_Delay(0.1);
    DMMotorCaliEncoder(motor);
    DWT_Delay(0.1);
    dm_motor_instance[idx++] = motor;
    return motor;
}

void DMMotorSetRef(DMMotorInstance *motor, float ref)
{
    motor->pid_ref = ref;
}

void DMMotorEnable(DMMotorInstance *motor)
{
    motor->stop_flag = MOTOR_ENALBED;
}

void DMMotorStop(DMMotorInstance *motor)//不使用使能模式是因为需要收到反馈
{
    motor->stop_flag = MOTOR_STOP;
}

void DMMotorOuterLoop(DMMotorInstance *motor, Closeloop_Type_e type)
{
    motor->Custom_Controller.motor_settings.outer_loop_type = type;
}


//@Todo: 目前只实现了力控，更多位控PID等请自行添加
void DMMotorTask(void const *argument)
{
    float  pid_measure, pid_ref, set;
    DMMotorInstance *motor = (DMMotorInstance *)argument;
   //DM_Motor_Measure_s *measure = &motor->measure;
    Motor_Control_Setting_s *setting = &motor->Custom_Controller.motor_settings;
    DM_Motor_Measure_s *measure = &motor->measure;
    //CANInstance *motor_can = motor->motor_can_instace;
    //uint16_t tmp;
    DMMotor_Send_s motor_send_mailbox;
    while (1)
    {
        
        pid_ref = motor->pid_ref;
        if (setting->motor_reverse_flag == MOTOR_DIRECTION_REVERSE)
            pid_ref *= -1;
        
        if ((setting->close_loop_type & ANGLE_LOOP) && setting->outer_loop_type == ANGLE_LOOP)
        {
            if (setting->angle_feedback_source == OTHER_FEED)
                pid_measure = *motor->Custom_Controller.other_angle_feedback_ptr;
            else
                pid_measure = measure->position; // MOTOR_FEED,对total angle闭环,防止在边界处出现突跃
            // 更新pid_ref进入下一个环
            pid_ref = PIDCalculate(&motor->Custom_Controller.angle_PID, pid_measure, pid_ref);
        }

        if (setting->feedback_reverse_flag == FEEDBACK_DIRECTION_REVERSE)
            pid_ref *= -1;
        set = (int16_t)pid_ref;

        switch (motor->work_mode)
        {
        case 0b00000000:
            /* code */
            motor->Universal_Controller.MIT_Control_Setting.Kp = 3;
            motor->Universal_Controller.MIT_Control_Setting.Kd = 1;
            motor->Custom_Controller.motor_settings.outer_loop_type = OPEN_LOOP;
            motor->Custom_Controller.motor_settings.close_loop_type = OPEN_LOOP;
            LIMIT_MIN_MAX(set, DM_P_MIN, DM_P_MAX);
            motor_send_mailbox.position_des = float_to_uint(set, DM_P_MIN, DM_P_MAX, 16);
            motor_send_mailbox.velocity_des = float_to_uint(0, DM_V_MIN, DM_V_MAX, 12);
            motor_send_mailbox.torque_des = float_to_uint(0, DM_T_MIN, DM_T_MAX, 12);
            break;
        case 0b00000001:
            motor->Universal_Controller.MIT_Control_Setting.Kp = 3;
            motor->Universal_Controller.MIT_Control_Setting.Kd = 1;
            motor->Custom_Controller.motor_settings.outer_loop_type = OPEN_LOOP;
            motor->Custom_Controller.motor_settings.close_loop_type = OPEN_LOOP;
            LIMIT_MIN_MAX(set, DM_P_MIN, DM_P_MAX);
            motor_send_mailbox.position_des = float_to_uint(set, DM_P_MIN, DM_P_MAX, 16);
            motor_send_mailbox.velocity_des = float_to_uint(0, DM_V_MIN, DM_V_MAX, 12);
            motor_send_mailbox.torque_des = float_to_uint(0, DM_T_MIN, DM_T_MAX, 12);
            break;
        case 0b00000011:
            motor->Universal_Controller.MIT_Control_Setting.Kp = 0;
            motor->Universal_Controller.MIT_Control_Setting.Kd = 1;
            motor->Custom_Controller.motor_settings.outer_loop_type = ANGLE_LOOP;
            motor->Custom_Controller.motor_settings.close_loop_type = ANGLE_LOOP;
            LIMIT_MIN_MAX(set, DM_V_MIN, DM_V_MAX);
            motor_send_mailbox.position_des = float_to_uint(set, DM_P_MIN, DM_P_MAX, 16);
            motor_send_mailbox.velocity_des = float_to_uint(0, DM_V_MIN, DM_V_MAX, 12);
            motor_send_mailbox.torque_des = float_to_uint(0, DM_T_MIN, DM_T_MAX, 12);
            break;
        default:
            
            LIMIT_MIN_MAX(set, DM_V_MIN, DM_V_MAX);
            motor_send_mailbox.position_des = float_to_uint(0, DM_P_MIN, DM_P_MAX, 16);
            motor_send_mailbox.velocity_des = float_to_uint(0, DM_V_MIN, DM_V_MAX, 12);
            motor_send_mailbox.torque_des = float_to_uint(set, DM_T_MIN, DM_T_MAX, 12);
            break;
        }

        if(motor->stop_flag == MOTOR_STOP)
        {
            motor_send_mailbox.Kp = 0;
            motor_send_mailbox.Kd = 0;
        }

        motor->motor_can_instace->tx_buff[0] = (uint8_t)(motor_send_mailbox.position_des >> 8);
        motor->motor_can_instace->tx_buff[1] = (uint8_t)(motor_send_mailbox.position_des);
        motor->motor_can_instace->tx_buff[2] = (uint8_t)(motor_send_mailbox.velocity_des >> 4);
        motor->motor_can_instace->tx_buff[3] = (uint8_t)(((motor_send_mailbox.velocity_des & 0xF) << 4) | (motor_send_mailbox.Kp >> 8));
        motor->motor_can_instace->tx_buff[4] = (uint8_t)(motor_send_mailbox.Kp);
        motor->motor_can_instace->tx_buff[5] = (uint8_t)(motor_send_mailbox.Kd >> 4);
        motor->motor_can_instace->tx_buff[6] = (uint8_t)(((motor_send_mailbox.Kd & 0xF) << 4) | (motor_send_mailbox.torque_des >> 8));
        motor->motor_can_instace->tx_buff[7] = (uint8_t)(motor_send_mailbox.torque_des);

        CANTransmit(motor->motor_can_instace, 1);

        osDelay(2);
    }
}
void DMMotorControlInit()
{
    char dm_task_name[5] = "dm";
    // 遍历所有电机实例,创建任务
    if (!idx)
        return;
    for (size_t i = 0; i < idx; i++)
    {
        char dm_id_buff[2] = {0};
        __itoa(i, dm_id_buff, 10);
        strcat(dm_task_name, dm_id_buff);
        osThreadDef(dm_task_name, DMMotorTask, osPriorityNormal, 0, 128);
        dm_task_handle[i] = osThreadCreate(osThread(dm_task_name), dm_motor_instance[i]);
    }
}