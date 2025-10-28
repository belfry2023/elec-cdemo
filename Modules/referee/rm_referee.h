#ifndef RM_REFEREE_H
#define RM_REFEREE_H

#include "usart.h"
#include "referee_protocol.h"
#include "task_def.h"
#include "bsp_usart.h"
#include "FreeRTOS.h"

extern uint8_t UI_Seq;

#pragma pack(1)
typedef struct
{
	uint8_t Robot_Color;
	uint16_t Robot_ID;
	uint16_t Cilent_ID;
	uint16_t Receiver_Robot_ID;
} referee_id_t;

typedef struct
{
	referee_id_t referee_id;

	xFrameHeader FrameHeader;
	uint16_t CmdID;
	ext_game_state_t GameState;
	ext_game_result_t GameResult;
	ext_game_robot_HP_t GameRobotHP;
	ext_event_data_t EventData;
	ext_referee_warning_t RefereeWarning;
	ext_dart_info_t DartInfo;
	ext_game_robot_state_t GameRobotState;
	ext_power_heat_data_t PowerHeatData;
	ext_game_robot_pos_t GameRobotPos;
	ext_buff_musk_t BuffMusk;
	ext_robot_hurt_t RobotHurt;
	ext_shoot_data_t ShootData;
	ext_projectile_allowance_t ProjectileAllowance;
	ext_rfid_status_t RfidStatus;
	ext_dart_client_cmd_t DartClientCmd;
	ext_ground_robot_position_t GroundRobotPosition;
	ext_radar_mark_data_t RadarMarkData;
	ext_sentry_info_t SentryInfo;
	ext_radar_info_t RadarInfo;

	Communicate_ReceiveData_t ReceiveData;

	uint8_t init_flag;

} referee_info_t;

typedef struct
{
	uint32_t chassis_flag : 1;
	uint32_t gimbal_flag : 1;
	uint32_t shoot_flag : 1;
	uint32_t lid_flag : 1;
	uint32_t friction_flag : 1;
	uint32_t Power_flag : 1;
	uint32_t bullet_speed_flag : 1;
} Referee_Interactive_Flag_t;

typedef struct
{
	Referee_Interactive_Flag_t Referee_Interactive_Flag;
	
	chassis_mode_e chassis_mode;
	gimbal_mode_e gimbal_mode;
	shoot_mode_e shoot_mode;
	friction_mode_e friction_mode;
	lid_mode_e lid_mode;
	Chassis_Power_Data_s Chassis_Power_Data;
	int bullet_speed;

	chassis_mode_e chassis_last_mode;
	gimbal_mode_e gimbal_last_mode;
	shoot_mode_e shoot_last_mode;
	friction_mode_e friction_last_mode;
	lid_mode_e lid_last_mode;
	Chassis_Power_Data_s Chassis_last_Power_Data;
	int last_bullet_speed;

} Referee_Interactive_info_t;

#pragma pack()

referee_info_t *RefereeInit(UART_HandleTypeDef *referee_usart_handle);
void RefereeSend(uint8_t *send, uint16_t tx_len);

#endif