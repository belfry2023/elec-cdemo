#include "module_dm-imu.h"
#include "memory.h"
#include "general_def.h"
#include "user_lib.h"
#include "cmsis_os.h"
#include "string.h"
#include "daemon.h"
#include "stdlib.h"
#include "bsp_dwt.h"
static uint8_t idx;
static module_dm_imu_t *dm_imu_instance[DM_IMU_MX_CNT];
static osThreadId dm_task_handle[DM_IMU_MX_CNT];


static void IMU_UpdateAccel(module_dm_imu_t *dm_imu)
{
	uint16_t accel[3];
	
	accel[0]=dm_imu->dm_imu_can_instance->rx_buff[3]<<8|dm_imu->dm_imu_can_instance->rx_buff[2];
	accel[1]=dm_imu->dm_imu_can_instance->rx_buff[5]<<8|dm_imu->dm_imu_can_instance->rx_buff[4];
	accel[2]=dm_imu->dm_imu_can_instance->rx_buff[7]<<8|dm_imu->dm_imu_can_instance->rx_buff[6];
	
	dm_imu->measure.Accel[0]=uint_to_float(accel[0],ACCEL_CAN_MIN,ACCEL_CAN_MAX,16);
	dm_imu->measure.Accel[1]=uint_to_float(accel[1],ACCEL_CAN_MIN,ACCEL_CAN_MAX,16);
	dm_imu->measure.Accel[2]=uint_to_float(accel[2],ACCEL_CAN_MIN,ACCEL_CAN_MAX,16);
	
}

static void IMU_UpdateGyro(module_dm_imu_t *dm_imu)
{
	uint16_t gyro[3];
	
	gyro[0]=dm_imu->dm_imu_can_instance->rx_buff[3]<<8|dm_imu->dm_imu_can_instance->rx_buff[2];
	gyro[1]=dm_imu->dm_imu_can_instance->rx_buff[5]<<8|dm_imu->dm_imu_can_instance->rx_buff[4];
	gyro[2]=dm_imu->dm_imu_can_instance->rx_buff[7]<<8|dm_imu->dm_imu_can_instance->rx_buff[6];
	
	dm_imu->measure.Gyro[0]=uint_to_float(gyro[0],GYRO_CAN_MIN,GYRO_CAN_MAX,16);
	dm_imu->measure.Gyro[1]=uint_to_float(gyro[1],GYRO_CAN_MIN,GYRO_CAN_MAX,16);
	dm_imu->measure.Gyro[2]=uint_to_float(gyro[2],GYRO_CAN_MIN,GYRO_CAN_MAX,16);
}


static void IMU_UpdateEuler(module_dm_imu_t *dm_imu)
{
	static uint16_t i;
	i++;
	int euler[3];
	
	euler[0]=dm_imu->dm_imu_can_instance->rx_buff[3]<<8|dm_imu->dm_imu_can_instance->rx_buff[2];
	euler[1]=dm_imu->dm_imu_can_instance->rx_buff[5]<<8|dm_imu->dm_imu_can_instance->rx_buff[4];
	euler[2]=dm_imu->dm_imu_can_instance->rx_buff[7]<<8|dm_imu->dm_imu_can_instance->rx_buff[6];
	
	dm_imu->measure.Pitch=uint_to_float(euler[0],PITCH_CAN_MIN,PITCH_CAN_MAX,16);
	dm_imu->measure.Yaw=uint_to_float(euler[1],YAW_CAN_MIN,YAW_CAN_MAX,16);
	dm_imu->measure.Roll=uint_to_float(euler[2],ROLL_CAN_MIN,ROLL_CAN_MAX,16);
	if(i < 500)
	{
		dm_imu->measure.YawOffset = dm_imu->measure.Yaw;
		dm_imu->measure.Yaw = 0;
		dm_imu->en = 0;
	}else
	{
		dm_imu->en = 1;
	}
	if(i > 1000)
	{
		i = 500;
	}
	if (dm_imu->measure.Yaw - dm_imu->measure.YawAngleLast > 180.0f)
    {
        dm_imu->measure.YawRoundCount--;
    }
    else if (dm_imu->measure.Yaw - dm_imu->measure.YawAngleLast < -180.0f)
    {
        dm_imu->measure.YawRoundCount++;
    }
    dm_imu->measure.YawTotalAngle = 360.0f * dm_imu->measure.YawRoundCount + dm_imu->measure.Yaw - dm_imu->measure.YawOffset;
	dm_imu->measure.YawAngleLast = dm_imu->measure.Yaw;
}


static void IMU_UpdateQuaternion(module_dm_imu_t *dm_imu)
{
	int w = dm_imu->dm_imu_can_instance->rx_buff[1]<<6| ((dm_imu->dm_imu_can_instance->rx_buff[2]&0xF8)>>2);
	int x = (dm_imu->dm_imu_can_instance->rx_buff[2]&0x03)<<12|(dm_imu->dm_imu_can_instance->rx_buff[3]<<4)|((dm_imu->dm_imu_can_instance->rx_buff[4]&0xF0)>>4);
	int y = (dm_imu->dm_imu_can_instance->rx_buff[4]&0x0F)<<10|(dm_imu->dm_imu_can_instance->rx_buff[5]<<2)|(dm_imu->dm_imu_can_instance->rx_buff[6]&0xC0)>>6;
	int z = (dm_imu->dm_imu_can_instance->rx_buff[6]&0x3F)<<8|dm_imu->dm_imu_can_instance->rx_buff[7];
	
	// dm_imu.q[0] = uint_to_float(w,Quaternion_MIN,Quaternion_MAX,14);
	// imu.q[1] = uint_to_float(x,Quaternion_MIN,Quaternion_MAX,14);
	// imu.q[2] = uint_to_float(y,Quaternion_MIN,Quaternion_MAX,14);
	// imu.q[3] = uint_to_float(z,Quaternion_MIN,Quaternion_MAX,14);
}

static void IMU_UpdateData(module_dm_imu_t *dm_imu)
{

	switch(dm_imu->dm_imu_can_instance->rx_buff[0])
	{
		case 1:
			IMU_UpdateAccel(dm_imu);
			break;
		case 2:
			IMU_UpdateGyro(dm_imu);
			break;
		case 3:
			IMU_UpdateEuler(dm_imu);
			break;
		case 4:
			IMU_UpdateQuaternion(dm_imu);
			break;
	}
}

static void DMImuDecode(CANInstance *dm_imu_can)
{
    
    uint8_t *rxbuff = dm_imu_can->rx_buff;
    module_dm_imu_t *dm_imu = (module_dm_imu_t *)dm_imu_can->id;
    attitude_t *measure = &(dm_imu->measure); // 将can实例中保存的id转换成电机实例的指针
    IMU_UpdateData(dm_imu);
}

static void DMImuLostCallback()
{

}

module_dm_imu_t* DMImuRegister(Module_DM_Imu_Init_Config_t *config)
{
    module_dm_imu_t *dm_imu = (module_dm_imu_t*)malloc(sizeof(module_dm_imu_t));
    memset(dm_imu,0,sizeof(module_dm_imu_t));


    dm_imu->can_id = config->can_id;
    config->dm_imu_can_config.can_module_callback = DMImuDecode;
    config->dm_imu_can_config.id = dm_imu;
    dm_imu->dm_imu_can_instance = CANRegister(&config->dm_imu_can_config);
    CANSetDLC(dm_imu->dm_imu_can_instance,4);

    dm_imu_instance[idx++] = dm_imu;
    return dm_imu;
}

void IMU_RequestData(module_dm_imu_t *dm_imu,uint8_t reg)
{
	
	uint8_t cmd[4]={(uint8_t)dm_imu->can_id,(uint8_t)(dm_imu->can_id>>8),reg,0xCC};
	memcpy(dm_imu->dm_imu_can_instance->tx_buff,cmd,sizeof(cmd));
	CANTransmit(dm_imu->dm_imu_can_instance,1);
}

void IMU_Task()
{
	static uint8_t read;
	read++;
	read %= 4;
	for(uint8_t i = 0; i < DM_IMU_MX_CNT; i++)
	{
		IMU_RequestData(dm_imu_instance[i],read);
	}
}