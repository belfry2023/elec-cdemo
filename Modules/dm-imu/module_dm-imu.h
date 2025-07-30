#ifndef __MODULE_DM-IMU_H
#define __MODULE_DM-IMU_H

#include "bsp_can.h"
#include "ins_task.h"
#include "stdint.h"
#include "daemon.h"

#define DM_IMU_MX_CNT 1
#define ACCEL_CAN_MAX (58.8f)
#define ACCEL_CAN_MIN	(-58.8f)
#define GYRO_CAN_MAX	(34.88f)
#define GYRO_CAN_MIN	(-34.88f)
#define PITCH_CAN_MAX	(90.0f)
#define PITCH_CAN_MIN	(-90.0f)
#define ROLL_CAN_MAX	(180.0f)
#define ROLL_CAN_MIN	(-180.0f)
#define YAW_CAN_MAX		(180.0f)
#define YAW_CAN_MIN 	(-180.0f)
#define TEMP_MIN			(0.0f)
#define TEMP_MAX			(60.0f)
#define Quaternion_MIN	(-1.0f)
#define Quaternion_MAX	(1.0f)

typedef struct 
{
    uint16_t can_id;
    attitude_t measure;
    CANInstance *dm_imu_can_instance;
    uint32_t lost_cnt;
    uint8_t en;
    /* data */
}module_dm_imu_t;

typedef struct 
{
    CAN_Init_Config_s dm_imu_can_config;
    uint16_t can_id;
    /* data */
}Module_DM_Imu_Init_Config_t;

module_dm_imu_t* DMImuRegister(Module_DM_Imu_Init_Config_t *config);
void IMU_RequestData(module_dm_imu_t *dm_imu,uint8_t reg);
void IMU_Task();

#endif