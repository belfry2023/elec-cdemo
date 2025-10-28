#ifndef TASK_DEF_H
#define TASK_DEF_H

#include "stdint.h"

#define OK 1
#define NO 0

#pragma pack(1)

typedef enum
{
    ROBOT_STOP = 0,
    ROBOT_READY,
} Robot_Status_e;

// 应用状态
typedef enum
{
    APP_OFFLINE = 0,
    APP_ONLINE,
    APP_ERROR,
} App_Status_e;

// 底盘模式设置
/**
 * @brief 后续考虑修改为云台跟随底盘,而不是让底盘去追云台,云台的惯量比底盘小.
 *
 */
typedef enum
{
    CHASSIS_ZERO_FORCE = 0,    // 电流零输入
    CHASSIS_ROTATE,            // 小陀螺模式
    CHASSIS_NO_FOLLOW,         // 不跟随，允许全向平移  //普通模式
    CHASSIS_FOLLOW_GIMBAL_YAW, // 跟随模式，底盘叠加角度环控制
} chassis_mode_e;

// 云台模式设置
typedef enum
{
    GIMBAL_ZERO_FORCE = 0, // 电流零输入
    GIMBAL_FREE_MODE,      // 云台自由运动模式,即与底盘分离(底盘此时应为NO_FOLLOW)反馈值为电机total_angle;似乎可以改为全部用IMU数据?
    GIMBAL_GYRO_MODE,      // 云台陀螺仪反馈模式,反馈值为陀螺仪pitch,total_yaw_angle,底盘可以为小陀螺和跟随模式
    GIMBAL_DEBUG_MODE,
    GIMBAL_NAV_MODE
} gimbal_mode_e;

// 发射模式设置
typedef enum
{
    SHOOT_OFF = 0,
    SHOOT_ON,
} shoot_mode_e;
typedef enum
{
    FRICTION_OFF = 0, // 摩擦轮关闭
    FRICTION_ON,      // 摩擦轮开启
} friction_mode_e;

typedef enum
{
    LID_OPEN = 0, // 弹舱盖打开
    LID_CLOSE,    // 弹舱盖关闭
} lid_mode_e;

typedef enum
{
    LOAD_STOP = 0,  // 停止发射
    LOAD_REVERSE,   // 反转
    LOAD_1_BULLET,  // 单发
    LOAD_3_BULLET,  // 三发
    LOAD_BURSTFIRE, // 连发
} loader_mode_e;

typedef struct
{ // 功率控制
    float chassis_power_mx;
} Chassis_Power_Data_s;

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
    float chassis_power_mx;
    uint16_t buffer_energy;
    /* data */
}Chassis_Upload_CAP_s;


#pragma pack()



#endif // TASK_DEF_H