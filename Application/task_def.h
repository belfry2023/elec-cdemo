#ifndef TASK_DEF_H
#define TASK_DEF_H

#include "stdint.h"

#pragma pack(1)

typedef struct 
{
    float spd;
    float dir;
    float tor;
    float err;
    /* data */
}motor_data;


// cmd控制结构体
typedef struct chassis_control_s
{
    float w;
    float v;
} chassis_control_t;

typedef struct
{
    float pitch;
    float yaw;
    uint8_t gimbal_task;
} Gimbal_Ctrl_Cmd_s;

// ui控制结构体
typedef struct 
{
    uint8_t chassis_task;
    uint8_t gimbal_task;
    /* data */
}CMD_Ctrl_UI_s;

/* *************************** */

// ui数据接收结构体
typedef struct 
{

    /* data */
}Chassis_Upload_UI_s;

typedef struct 
{
    float yaw;
    float pitch;
    float roll;
    motor_data yaw_motor;
    motor_data pitch_motor;
    /* data */
}Gimbal_Upload_UI_s;

typedef struct 
{
    float k230_yaw;
    float k230_pitch;
    /* data */
}CMD_Upload_UI_s;


// cmd接收结构体
typedef struct
{
    float yaw;
    float pitch;
    float roll;
}Gimbal_Upload_Data_s;

typedef struct chassis_state_s
{
    
}chassis_state_t;

#pragma pack()



#endif // TASK_DEF_H