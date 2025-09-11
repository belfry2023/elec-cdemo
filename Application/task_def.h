#ifndef TASK_DEF_H
#define TASK_DEF_H

#include "stdint.h"

#define OK 1
#define NO 0

#pragma pack(1)

typedef struct 
{
    float spd;
    float dir;
    float tor;
    float speed_pid_out;
    float err;
    float AKp;
    float SKp;
    /* data */
}motor_data;

typedef enum{
    HOLD = 0,
    FIRE = 1
}shoot_mode;

// cmd控制结构体
typedef struct
{
    float w;
    float v;
} Chassis_Ctrl_Cmd_s;



typedef struct
{
    float pitch;
    float yaw;
    uint8_t gimbal_task;
    motor_data yaw_motor;
    motor_data pitch_motor;
} Gimbal_Ctrl_Cmd_s;

typedef struct
{
    motor_data fricl;
    motor_data fricr;
    motor_data loader;
    shoot_mode mode;
} Shoot_Ctrl_Cmd_s;

// ui控制结构体
typedef struct 
{
    uint8_t chassis_task;
    uint8_t gimbal_task;
    motor_data yaw_motor;
    motor_data pitch_motor;
    /* data */
}CMD_Ctrl_UI_s;

typedef struct 
{
    motor_data motor[4];
    /* data */ 
} CAP_Ctrl_Chassis_s;

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
    motor_data fricl;
    motor_data fricr;
    motor_data loader;
    /* data */
}Shoot_Upload_UI_s;

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

typedef struct
{
    
}Shoot_Upload_Data_s;

typedef struct
{
    
}Chassis_Upload_CMD_s;

typedef struct
{
    motor_data motor[4];
    uint8_t SuperCapReady;
    uint16_t ChassisPower;
    /* data */
}Chassis_Upload_CAP_s;


#pragma pack()



#endif // TASK_DEF_H