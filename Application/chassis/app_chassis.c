#include "app_chassis.h"
#include "bsp_dwt.h"
#include "module_ir.h"
#include "message_center.h"
#include "task_def.h"
#include "OLED.h"

#include "dji_motor.h"
#include "520motor.h"
#include "CAN_supercap_communication.h"
#include "chassis_power_control_with_supercap.h"
#include "power_measure.h"
static Subscriber_t *chassis_sub;
static Publisher_t *chassis_pub; // 发布者实例指针
static Chassis_Ctrl_Cmd_s chassis_cmd_recv; // 车的控制指令
static Chassis_Upload_CMD_s chassis_feedback_cmd; // 车的状态
static Chassis_Upload_CAP_s chassis_feedback_cap;
static Publisher_t *chassis_ui_pub;
static Chassis_Upload_UI_s chassis_feedback_ui;
static DJIMotorInstance *motor_lf, *motor_rf, *motor_lb, *motor_rb; // left right forward back
static SuperCap_s *supercap;
static module_ir_t *ir = NULL;

void chassis_init()
{	
	// 初始化底盘
    INA226_Init(INA226_ADDR1);
    supercap = SuperCapInit(&hcan1);
    supercap->TX_Temp.Enable = DISABLE;
    supercap->TX_Temp.Powerlimit = 45;
	Motor_Init_Config_s chassis_motor_config = {
        .can_init_config.can_handle = &hcan2,
        .controller_param_init_config = {
            .speed_PID = {
                .Kp = 10, // 4.5
                .Ki = 0,  // 0
                .Kd = 0,  // 0
                .IntegralLimit = 3000,
                .Improve = PID_Trapezoid_Intergral | PID_Integral_Limit | PID_Derivative_On_Measurement,
                .MaxOut = 12000,
            },
            .current_PID = {
                .Kp = 0.5, // 0.4
                .Ki = 0,   // 0
                .Kd = 0,
                .IntegralLimit = 3000,
                .Improve = PID_Trapezoid_Intergral | PID_Integral_Limit | PID_Derivative_On_Measurement,
                .MaxOut = 15000,
            },
        },
        .controller_setting_init_config = {
            .mode = power_control,
            .angle_feedback_source = MOTOR_FEED,
            .speed_feedback_source = MOTOR_FEED,
            .outer_loop_type = SPEED_LOOP,
            .close_loop_type = SPEED_LOOP | CURRENT_LOOP,
        },
        .motor_type = M3508,
    };

	chassis_motor_config.can_init_config.tx_id = 1;
    chassis_motor_config.controller_setting_init_config.motor_reverse_flag = MOTOR_DIRECTION_REVERSE;
    motor_lf = DJIMotorInit(&chassis_motor_config);

    chassis_motor_config.can_init_config.tx_id = 2;
    chassis_motor_config.controller_setting_init_config.motor_reverse_flag = MOTOR_DIRECTION_REVERSE;
    motor_rf = DJIMotorInit(&chassis_motor_config);

    chassis_motor_config.can_init_config.tx_id = 4;
    chassis_motor_config.controller_setting_init_config.motor_reverse_flag = MOTOR_DIRECTION_REVERSE;
    motor_lb = DJIMotorInit(&chassis_motor_config);

    chassis_motor_config.can_init_config.tx_id = 3;
    chassis_motor_config.controller_setting_init_config.motor_reverse_flag = MOTOR_DIRECTION_REVERSE;
    motor_rb = DJIMotorInit(&chassis_motor_config);

    // MotorUSARTInit(&huart6);

	// ir = ir_init();
	// if (ir == NULL) {
	// 	// Handle memory allocation failure
	// 	return;
	// }

	chassis_sub = SubRegister("chassis_control", sizeof(Chassis_Ctrl_Cmd_s)); // 订阅车的控制指令
    chassis_pub = PubRegister("chassis_state", sizeof(Chassis_Upload_CMD_s)); // 发布车的状态
    chassis_ui_pub = PubRegister("chassis_feed_ui",sizeof(Chassis_Upload_UI_s));
}


static void parameter_acceptance()
{
    set_chassis_power(100);
    supercap->TX_Temp.Enable = ENABLE;
    supercap->TX_Temp.Powerlimit = 120;

}

static void core_task()
{
    SuperCapSend(supercap);
    // INA226_Read_Registers(INA226_ADDR1);
    CAP_Ctrl_Chassis_s *chassis_cap_recv = chassis_power_control_with_supercap(&chassis_feedback_cap);
    DJIMotorSetCur(motor_lf, chassis_cap_recv->motor[0].speed_pid_out);
    DJIMotorSetCur(motor_rf, chassis_cap_recv->motor[1].speed_pid_out);
    DJIMotorSetCur(motor_lb, chassis_cap_recv->motor[2].speed_pid_out);
    DJIMotorSetCur(motor_rb, chassis_cap_recv->motor[3].speed_pid_out);
}

static void parameter_passing()
{
    chassis_feedback_cap.motor[0].spd = motor_lf->measure.speed_aps;
    chassis_feedback_cap.motor[1].spd = motor_rf->measure.speed_aps;
    chassis_feedback_cap.motor[2].spd = motor_lb->measure.speed_aps;
    chassis_feedback_cap.motor[3].spd = motor_rb->measure.speed_aps;
    chassis_feedback_cap.motor[0].speed_pid_out = motor_lf->motor_controller.speed_PID.Output;
    chassis_feedback_cap.motor[1].speed_pid_out = motor_rf->motor_controller.speed_PID.Output;
    chassis_feedback_cap.motor[2].speed_pid_out = motor_lb->motor_controller.speed_PID.Output;
    chassis_feedback_cap.motor[3].speed_pid_out = motor_rb->motor_controller.speed_PID.Output;
    chassis_feedback_cap.SuperCapReady = supercap->CAN_SuperCapRXData.SuperCapReady;
    chassis_feedback_cap.ChassisPower = supercap->CAN_SuperCapRXData.ChassisPower;
}

void chassis_task()
{
	SubGetMessage(chassis_sub, &chassis_cmd_recv); // 获取车的控制指令
    parameter_acceptance();
    core_task();
    parameter_passing();
    PubPushMessage(chassis_pub, &chassis_feedback_cmd); // 发布车的状态
    PubPushMessage(chassis_ui_pub,&chassis_feedback_ui);
}