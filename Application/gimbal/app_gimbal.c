#include "app_gimbal.h"
#include "module_servo.h"
#include "message_center.h"
#include "task_def.h"
#include "ins_task.h"
#include "OLED.h"
#include "dji_motor.h"
#include "module_dm-imu.h"
static Publisher_t *gimbal_pub;                   // 云台应用消息发布者(云台反馈给cmd)
static Subscriber_t *gimbal_sub;                  // cmd控制消息订阅者
static Gimbal_Upload_Data_s gimbal_feedback_data; // 回传给cmd的云台状态信息
static Gimbal_Ctrl_Cmd_s gimbal_cmd_recv;         // 来自cmd的控制信息
static Publisher_t *gimbal_ui_pub;
static Gimbal_Upload_UI_s gimbal_feeddata_ui;
static DJIMotorInstance *pitch_motor, *yaw_motor;
// static module_dm_imu_t *gimba_IMU_data;
static attitude_t *gimbal_IMU_data;
static float speed_forward, yaw_forward;
void gimbal_init()
{
    gimbal_IMU_data = INS_Init();
    // DWT_Delay(1);
    // Module_DM_Imu_Init_Config_t conf = {
    //     .dm_imu_can_config = {
    //         .can_handle = &hcan2,
    //         .tx_id = 0x6ff,
    //         .rx_id = 0x11
    //     },
    //     .can_id = 0x01
    // };
    // gimba_IMU_data = DMImuRegister(&conf);
    Motor_Init_Config_s yaw_config = {
        .can_init_config = {
            .can_handle = &hcan1,
            .tx_id = 1,
        },
        .controller_param_init_config = {
            .angle_PID = {
                .Kp = 60,// 50, // 8
                .Ki = 0,
                .Kd = 0.6,// 4,
                .DeadBand = 0, 
                .Derivative_LPF_RC = 0.003,
                .Output_LPF_RC = 0.02,
                .Improve = PID_Trapezoid_Intergral | PID_Integral_Limit | PID_Derivative_On_Measurement,
                .IntegralLimit = 100,
                .MaxOut = 300,
                .MaxOut_ = -300
            },
            .speed_PID = {
                .Kp = 40,// 100, // 50
                .Ki = 0,// 200, // 200
                .Kd = 0,// 70,
                .Output_LPF_RC = 0.001,
                .Improve = PID_Trapezoid_Intergral | PID_Integral_Limit | PID_Derivative_On_Measurement | PID_OutputFilter,
                .IntegralLimit = 3000,
                .MaxOut = 30000,
                .MaxOut_ = -20000
            },
            .other_angle_feedback_ptr = &gimbal_IMU_data->YawTotalAngle,
            .other_speed_feedback_ptr = &gimbal_IMU_data->Gyro[2], //&rotate_compensator,
            .speed_feedforward_ptr = &yaw_forward
        },
        .controller_setting_init_config = {
            .angle_feedback_source = OTHER_FEED,
            .speed_feedback_source = OTHER_FEED,
            .outer_loop_type = ANGLE_LOOP,
            .close_loop_type = ANGLE_LOOP | SPEED_LOOP,
            .motor_reverse_flag = MOTOR_DIRECTION_NORMAL,
            .feedforward_flag = SPEED_FEEDFORWARD,
        },
        .motor_type = GM6020};
    // PITCH
    Motor_Init_Config_s pitch_config = {
        .can_init_config = {
            .can_handle = &hcan1,
            .tx_id = 3,
        },   
        .controller_param_init_config = {
            .angle_PID = {
                .Kp = 30,// 40, // 10
                .Ki = 0,
                .Kd = 0.6,// 0.6,
                .Derivative_LPF_RC = 0.2,
                .Output_LPF_RC = 0.13,
                .Improve = PID_Trapezoid_Intergral | PID_Integral_Limit | PID_Derivative_On_Measurement,
                .IntegralLimit = 100,
                .MaxOut = 100,
                .MaxOut_ = -100
            },
            .speed_PID = {
                .Kp = 30,// 80,  // 50
                .Ki = 0,// 350, // 350
                .Kd = 0,   // 0
                .Output_LPF_RC = 0.001,
                .Improve = PID_Trapezoid_Intergral | PID_Integral_Limit | PID_Derivative_On_Measurement | PID_OutputFilter,
                .IntegralLimit = 2500,
                .MaxOut = 25000,
                .MaxOut_ = -25000
            },
            .other_angle_feedback_ptr = &gimbal_IMU_data->Pitch,
            // 还需要增加角速度额外反馈指针,注意方向,ins_task.md中有c板的bodyframe坐标系说明
            .other_speed_feedback_ptr = (&gimbal_IMU_data->Gyro[0]),
            .speed_feedforward_ptr = &speed_forward,
        },
        .controller_setting_init_config = {
            .angle_feedback_source = OTHER_FEED,
            .speed_feedback_source = OTHER_FEED,
            .outer_loop_type = ANGLE_LOOP,
            .close_loop_type = SPEED_LOOP | ANGLE_LOOP,
            .motor_reverse_flag = MOTOR_DIRECTION_NORMAL,
            .feedforward_flag = SPEED_FEEDFORWARD,
        },
        .motor_type = GM6020,
    };
    // 电机对total_angle闭环,上电时为零,会保持静止,收到遥控器数据再动
    yaw_motor = DJIMotorInit(&yaw_config);
    pitch_motor = DJIMotorInit(&pitch_config);
    gimbal_pub = PubRegister("gimbal_feed", sizeof(Gimbal_Upload_Data_s));
    gimbal_ui_pub = PubRegister("gimbal_feed_ui",sizeof(Gimbal_Upload_UI_s));
    gimbal_sub = SubRegister("gimbal_cmd", sizeof(Gimbal_Ctrl_Cmd_s));
    
}

static void Hysteresis_comparator(float *Output, float meassure, float max, float min, float high, float low)
{
    if(meassure > max)
    {
        *Output = high;
    }else if (meassure < min)
    {
        *Output = low;
    }
}

static void change_spd_ford()
{
    
    // if(pitch_motor->motor_controller.angle_PID.Err > 0)
    //     speed_forward = pitch_motor->motor_controller.angle_PID.Err > 1 ? 100 : 0;
    // else
    //     speed_forward = pitch_motor->motor_controller.angle_PID.Err < -1 ? -100 : 0;
    if(pitch_motor->motor_controller.angle_PID.Err > 0)
        Hysteresis_comparator(&speed_forward, pitch_motor->motor_controller.angle_PID.Err, 1, 0.3, 0, 0);
    else
        Hysteresis_comparator(&speed_forward, pitch_motor->motor_controller.angle_PID.Err, -0.3, -1, 0, -0);

}

static void change_yaw_spd_ford()
{
    // if(yaw_motor->motor_controller.angle_PID.Err > 0)
    //     yaw_forward = yaw_motor->motor_controller.angle_PID.Err > 1 ? 100 : 0;
    // else
    //     yaw_forward = yaw_motor->motor_controller.angle_PID.Err < -1 ? -100 : 0;
    if(yaw_motor->motor_controller.angle_PID.Err > 0)
        Hysteresis_comparator(&yaw_forward, yaw_motor->motor_controller.angle_PID.Err, 1, 0.1, 0, 0);
    else
        Hysteresis_comparator(&yaw_forward, yaw_motor->motor_controller.angle_PID.Err, -0.1, -1, 0, -0);
}

static uint8_t flag;
void gimbal_task()
{
    flag++;
    flag%=2;
    SubGetMessage(gimbal_sub, (&gimbal_cmd_recv));
    change_spd_ford();
    change_yaw_spd_ford();
    // DJIMotorStop(yaw_motor);
    // DJIMotorStop(pitch_motor);
    
    DJIMotorStop(yaw_motor);
    DJIMotorStop(pitch_motor);
    DJIMotorChangeFeed(yaw_motor, ANGLE_LOOP, OTHER_FEED);
    DJIMotorChangeFeed(yaw_motor, SPEED_LOOP, OTHER_FEED);
    DJIMotorChangeFeed(pitch_motor, ANGLE_LOOP, OTHER_FEED);
    DJIMotorChangeFeed(pitch_motor, SPEED_LOOP, OTHER_FEED);
    if(gimbal_cmd_recv.gimbal_task)
    {
        DJIMotorEnable(yaw_motor);
        DJIMotorEnable(pitch_motor);
    }
    DJIMotorEnable(yaw_motor);
    DJIMotorEnable(pitch_motor);
    DJIMotorSetRef(yaw_motor, gimbal_cmd_recv.yaw); // yaw和pitch会在robot_cmd中处理好多圈和单圈
    DJIMotorSetRef(pitch_motor, gimbal_cmd_recv.pitch);
    gimbal_feedback_data.pitch = gimbal_IMU_data->Pitch;
    gimbal_feedback_data.yaw = gimbal_IMU_data->YawTotalAngle;
    gimbal_feeddata_ui.pitch = gimbal_IMU_data->Pitch;
    gimbal_feeddata_ui.yaw = gimbal_IMU_data->YawTotalAngle;
    gimbal_feeddata_ui.roll = gimbal_IMU_data->Roll;
    gimbal_feeddata_ui.yaw_motor.err = yaw_motor->motor_controller.angle_PID.Err;
    gimbal_feeddata_ui.yaw_motor.spd = yaw_motor->measure.speed_aps;
    gimbal_feeddata_ui.yaw_motor.dir = yaw_motor->measure.angle_single_round;
    gimbal_feeddata_ui.yaw_motor.tor = yaw_motor->measure.real_current;
    gimbal_feeddata_ui.pitch_motor.err = pitch_motor->motor_controller.angle_PID.Err;
    gimbal_feeddata_ui.pitch_motor.spd = pitch_motor->measure.speed_aps;
    gimbal_feeddata_ui.pitch_motor.dir = pitch_motor->measure.angle_single_round;
    gimbal_feeddata_ui.pitch_motor.tor = pitch_motor->measure.real_current;
    PubPushMessage(gimbal_pub, (void *)&gimbal_feedback_data);
    PubPushMessage(gimbal_ui_pub,&gimbal_feeddata_ui);
}