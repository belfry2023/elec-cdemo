#include "app_shoot.h"
#include "message_center.h"
#include "task_def.h"
#include "ins_task.h"
#include "OLED.h"
#include "dji_motor.h"

static Publisher_t *shoot_pub;                   // 云台应用消息发布者(云台反馈给cmd)
static Subscriber_t *shoot_sub;                  // cmd控制消息订阅者
static Shoot_Upload_Data_s shoot_feedback_data; // 回传给cmd的云台状态信息
static Shoot_Ctrl_Cmd_s shoot_cmd_recv;         // 来自cmd的控制信息
static Publisher_t *shoot_ui_pub;
static Shoot_Upload_UI_s shoot_feeddata_ui;
static DJIMotorInstance *friction_l, *friction_r, *loader;

void shoot_init()
{
    Motor_Init_Config_s friction_config = {
        .can_init_config = {
            .can_handle = &hcan2,
        },
        .controller_param_init_config = {
            .adrc_config = Default_3508_speed,
            .speed_PID = {
                .Kp = 20, // 20
                .Ki = 0, // 1
                .Kd = 0,
                .Improve = PID_Integral_Limit,
                .IntegralLimit = 10000,
                .MaxOut = 15000,
                .MaxOut_ = -15000,
                .DeadBand = 0
            },
            .current_PID = { 
                .Kp = 1.1, // 0.7
                .Ki = 0, // 0.1
                .Kd = 0,
                .Improve = PID_Integral_Limit,
                .IntegralLimit = 10000,
                .MaxOut = 15000,
                .MaxOut_ = -15000
            },
        },
        .controller_setting_init_config = {
            .angle_feedback_source = MOTOR_FEED,
            .speed_feedback_source = MOTOR_FEED,
            .outer_loop_type = SPEED_LOOP,
            .close_loop_type = SPEED_LOOP | CURRENT_LOOP,
            .motor_reverse_flag = MOTOR_DIRECTION_NORMAL,
        },
        .motor_type = M3508};
    friction_config.can_init_config.tx_id = 1,
    friction_l = DJIMotorInit(&friction_config);

    friction_config.can_init_config.tx_id = 2; // 右摩擦轮,改txid和方向就行
    friction_config.controller_setting_init_config.motor_reverse_flag = MOTOR_DIRECTION_REVERSE;
    friction_r = DJIMotorInit(&friction_config);

    // 拨盘电机
    Motor_Init_Config_s loader_config = {
        .can_init_config = {
            .can_handle = &hcan1,
            .tx_id = 7,
        },
        .controller_param_init_config = {
            .angle_PID = {
                // 如果启用位置环来控制发弹,需要较大的I值保证输出力矩的线性度否则出现接近拨出的力矩大幅下降
                .Kp = 1000, // 10
                .Ki = 3,
                .Kd = 6,
                .MaxOut = 10000,
                .MaxOut_ = -10000,
                .Improve = PID_Trapezoid_Intergral | PID_Integral_Limit | PID_Derivative_On_Measurement
            },
            .speed_PID = {
                .Kp = 6, // 10
                .Ki = 0, // 1
                .Kd = 0,
                .Improve = PID_Integral_Limit | PID_ErrorHandle,
                .IntegralLimit = 5000,
                .MaxOut = 10000,
                .MaxOut_ = -10000
            },
            .current_PID = {
                .Kp = 1.1, // 0.7
                .Ki = 0, // 0.1
                .Kd = 0,
                .Improve = PID_Integral_Limit,
                .IntegralLimit = 5000,
                .MaxOut = 7000,
                .MaxOut_ = -7000
            },
        },
        .controller_setting_init_config = {
            .angle_feedback_source = MOTOR_FEED, .speed_feedback_source = MOTOR_FEED,
            .outer_loop_type = SPEED_LOOP, // 初始化成SPEED_LOOP,让拨盘停在原地,防止拨盘上电时乱转
            .close_loop_type = SPEED_LOOP | CURRENT_LOOP | ANGLE_LOOP,
            .motor_reverse_flag = MOTOR_DIRECTION_NORMAL, // 注意方向设置为拨盘的拨出的击发方向
        },
        .motor_type = M2006 // 英雄使用m3508
    };
    loader = DJIMotorInit(&loader_config);

    shoot_pub = PubRegister("shoot_feed", sizeof(Shoot_Upload_Data_s));
    shoot_sub = SubRegister("shoot_cmd", sizeof(Shoot_Ctrl_Cmd_s));
}

static void core_task()
{
    if(shoot_cmd_recv.mode == HOLD)
    {
        DJIMotorSetRef(friction_l, 0);
        DJIMotorSetRef(friction_r, 0);
        // loader保持静止
        DJIMotorSetRef(loader, 0);
    }
    else if(shoot_cmd_recv.mode == FIRE)
    {
        // 摩擦轮开到一定速度
        DJIMotorSetRef(friction_l, 5000);
        DJIMotorSetRef(friction_r, 5000);
        DJIMotorSetRef(loader, 0);
        if(friction_l->measure.speed_aps > 4000 && friction_r->measure.speed_aps > 4000) // 摩擦轮转起来后再拨弹
            DJIMotorSetRef(loader, 1000);
    }
}

void shoot_task()
{
    SubGetMessage(shoot_sub, (&shoot_cmd_recv));

    core_task();

    PubPushMessage(shoot_pub, (void *)&shoot_feedback_data);
    PubPushMessage(shoot_ui_pub,&shoot_feeddata_ui);
}


