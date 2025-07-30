#ifndef DMMOTOR_H
#define DMMOTOR_H
#include <stdint.h>
#include "bsp_can.h"
#include "controller.h"
#include "motor_def.h"
#include "daemon.h"

#define DM_MOTOR_CNT 4

#define DM_P_MIN  (-12.5f)
#define DM_P_MAX  12.5f
#define DM_V_MIN  (-45.0f)
#define DM_V_MAX  45.0f
#define DM_T_MIN  (-18.0f)
#define DM_T_MAX   18.0f

typedef struct 
{
    uint8_t id;
    uint8_t state;
    float velocity;
    float last_position;
    float position;
    float torque;
    float T_Mos;
    float T_Rotor;
    int32_t total_round;
}DM_Motor_Measure_s;

typedef struct
{
    uint16_t position_des;
    uint16_t velocity_des;
    uint16_t torque_des;
    uint16_t Kp;
    uint16_t Kd;
}DMMotor_Send_s;

typedef struct 
{
    Motor_Control_Setting_s motor_settings;
    PIDInstance angle_PID;
    float *other_angle_feedback_ptr;
    /* data */
}DMMotor_Custom_Control_t;



typedef struct 
{
    DMMotor_Send_s MIT_Control_Setting;
    /* data */
}DMMotor_Universal_Control_t;

typedef enum
{
    OPEN = 0b00000000,                // 0000 0000
    MIT = 0b00000001,              // 0000 0001
    CLOSE = 0b00000010              // 0000 0010
} DMMotor_Work_Mode_e;

typedef struct 
{
    DM_Motor_Measure_s measure;
    DMMotor_Custom_Control_t Custom_Controller;
    DMMotor_Universal_Control_t Universal_Controller;
    DMMotor_Work_Mode_e work_mode;
    float pid_ref;
    Motor_Working_Type_e stop_flag;
    CANInstance *motor_can_instace;
    DaemonInstance* motor_daemon;
    uint32_t lost_cnt;
}DMMotorInstance;


typedef struct 
{
    CAN_Init_Config_s can_init_config;
    float *other_angle_feedback_ptr;
    PID_Init_Config_s angle_PID;
    DMMotor_Work_Mode_e work_mode;
    Motor_Control_Setting_s controller_setting_init_config;
    /* data */
}DMMotor_Init_Config_s;


typedef enum
{
    DM_CMD_MOTOR_MODE = 0xfc,   // 使能,会响应指令
    DM_CMD_RESET_MODE = 0xfd,   // 停止
    DM_CMD_ZERO_POSITION = 0xfe, // 将当前的位置设置为编码器零位
    DM_CMD_CLEAR_ERROR = 0xfb // 清除电机过热错误
}DMMotor_Mode_e;

DMMotorInstance *DMMotorInit(DMMotor_Init_Config_s *config);

void DMMotorSetRef(DMMotorInstance *motor, float ref);

void DMMotorOuterLoop(DMMotorInstance *motor,Closeloop_Type_e closeloop_type);

void DMMotorEnable(DMMotorInstance *motor);

void DMMotorStop(DMMotorInstance *motor);
void DMMotorCaliEncoder(DMMotorInstance *motor);
void DMMotorControlInit();
#endif // !DMMOTOR