#ifndef __520MOTOR_H
#define __520MOTOR_H
#include "stdbool.h"
#include "usart.h"
typedef enum _motor_type  //此类型用做判断死区	This type is used to determine the dead zone
{
	TYPE_NONE = 0x00,       // 保留	reserve
	MOTOR_520 ,       //520电机 包含L型	520 motor including L type
	MOTOR_310 ,       //310电机	310 motor
	MOTOR_TT_Encoder ,  //tt电机,带编码器	tt motor with encoder
	MOTOR_TT , 				// tt电机,不带编码器	tt motor, without encoder

	Motor_TYPE_MAX                // 最后一个电机类型，仅作为判断	The last motor type is for judgment only
} motor_type_t;

void MotorUSARTInit(UART_HandleTypeDef *huart);

// 发送函数
void send_motor_type(motor_type_t data);
void send_motor_deadzone(uint16_t data);
void send_pulse_line(uint16_t data);
void send_pulse_phase(uint16_t data);
void send_wheel_diameter(float data);
void send_motor_PID(float P, float I, float D);
void send_upload_data(bool ALLEncoder_Switch, bool TenEncoder_Switch, bool Speed_Switch);
void Contrl_Speed(int16_t M1_speed, int16_t M2_speed, int16_t M3_speed, int16_t M4_speed);
void Contrl_Pwm(int16_t M1_pwm, int16_t M2_pwm, int16_t M3_pwm, int16_t M4_pwm);
void Test_send();
// 接收数据访问接口
extern float g_Speed[4];
extern int Encoder_Offset[4];
extern int Encoder_Now[4];
extern uint8_t g_recv_flag;

#endif