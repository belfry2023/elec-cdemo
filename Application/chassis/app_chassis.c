#include "app_chassis.h"
#include "bsp_dwt.h"
#include "module_ir.h"
#include "message_center.h"
#include "task_def.h"
#include "OLED.h"
#include "dji_motor.h"
#include "520motor.h"
static Subscriber_t *chassis_sub;
static Publisher_t *chassis_pub; // 发布者实例指针
static chassis_control_t chassis_control; // 车的控制指令
static chassis_state_t chassis_state; // 车的状态
static Publisher_t *chassis_ui_pub;
static Chassis_Upload_UI_s chassis_feedback_ui;
// static DJIMotorInstance *motor_lf, *motor_rf, *motor_lb, *motor_rb; // left right forward back
static module_ir_t *ir = NULL;

void chassis_init()
{	
	// 初始化底盘
	// Motor_Init_Config_s chassis_motor_config = {
    //     .can_init_config.can_handle = &hcan1,
    //     .controller_param_init_config = {
    //         .speed_PID = {
    //             .Kp = 10, // 4.5
    //             .Ki = 0,  // 0
    //             .Kd = 0,  // 0
    //             .IntegralLimit = 3000,
    //             .Improve = PID_Trapezoid_Intergral | PID_Integral_Limit | PID_Derivative_On_Measurement,
    //             .MaxOut = 12000,
    //         },
    //         .current_PID = {
    //             .Kp = 0.5, // 0.4
    //             .Ki = 0,   // 0
    //             .Kd = 0,
    //             .IntegralLimit = 3000,
    //             .Improve = PID_Trapezoid_Intergral | PID_Integral_Limit | PID_Derivative_On_Measurement,
    //             .MaxOut = 15000,
    //         },
    //     },
    //     .controller_setting_init_config = {
    //         .angle_feedback_source = MOTOR_FEED,
    //         .speed_feedback_source = MOTOR_FEED,
    //         .outer_loop_type = SPEED_LOOP,
    //         .close_loop_type = SPEED_LOOP | CURRENT_LOOP,
    //     },
    //     .motor_type = M2006,
    // };

	// chassis_motor_config.can_init_config.tx_id = 1;
    // chassis_motor_config.controller_setting_init_config.motor_reverse_flag = MOTOR_DIRECTION_REVERSE;
    // motor_lf = DJIMotorInit(&chassis_motor_config);

    // chassis_motor_config.can_init_config.tx_id = 2;
    // chassis_motor_config.controller_setting_init_config.motor_reverse_flag = MOTOR_DIRECTION_REVERSE;
    // motor_rf = DJIMotorInit(&chassis_motor_config);

    // chassis_motor_config.can_init_config.tx_id = 4;
    // chassis_motor_config.controller_setting_init_config.motor_reverse_flag = MOTOR_DIRECTION_REVERSE;
    // motor_lb = DJIMotorInit(&chassis_motor_config);

    // chassis_motor_config.can_init_config.tx_id = 3;
    // chassis_motor_config.controller_setting_init_config.motor_reverse_flag = MOTOR_DIRECTION_REVERSE;
    // motor_rb = DJIMotorInit(&chassis_motor_config);

    MotorUSARTInit(&huart6);

	ir = ir_init();
	if (ir == NULL) {
		// Handle memory allocation failure
		return;
	}

	chassis_sub = SubRegister("chassis_control", sizeof(chassis_control_t)); // 订阅车的控制指令
    chassis_pub = PubRegister("chassis_state", sizeof(chassis_state_t)); // 发布车的状态
    chassis_ui_pub = PubRegister("chassis_feed_ui",sizeof(Chassis_Upload_UI_s));
	OLED_ClearArea(10, 4, 96, 30);
	OLED_ShowString(10, 4, "Chassis OK!", OLED_6X8);
	OLED_ShowString(10, 14, "Start Thread...", OLED_6X8);
	OLED_DrawRectangle(0, 40, 96, 15, OLED_FILLED);
	OLED_DrawRectangle(0, 40, 96, 15, OLED_UNFILLED);
	OLED_Update();
}


void chassis_task()
{
	SubGetMessage(chassis_sub, &chassis_control); // 获取车的控制指令
	deal_IRdata();
    Contrl_Speed(400,400,400,400);
    // Test_send();
    PubPushMessage(chassis_pub, &chassis_state); // 发布车的状态
    PubPushMessage(chassis_ui_pub,&chassis_feedback_ui);
}