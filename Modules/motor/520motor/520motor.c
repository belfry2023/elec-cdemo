#include "520motor.h"
#include "string.h"
#include "bsp_usart.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdint.h"
#include "stdbool.h"
#define MOTOR_PROTOCOL_MAX_FRAME_SIZE 128u
#define MOTOR_PROTOCOL_TIMEOUT_MS 100

// 全局数据
float g_Speed[4] = {0};
int Encoder_Offset[4] = {0};
int Encoder_Now[4] = {0};
uint8_t g_recv_flag = 0;

// 串口实例
static USARTInstance *motor_usart = NULL;

//////////********************发送部分********************///////////
// 通用发送函数
static void MotorSendData(uint8_t *data, uint16_t len)
{
    if (motor_usart != NULL) {
        USARTSend(motor_usart, data, len, USART_TRANSFER_BLOCKING);
    }
}

//发送电机类型
void send_motor_type(motor_type_t data)
{
    uint8_t send_buff[50];
    sprintf((char*)send_buff,"$mtype:%d#",data);
    MotorSendData(send_buff, strlen((char*)send_buff));
}

//发送电机死区
void send_motor_deadzone(uint16_t data)
{
    uint8_t send_buff[50];
    sprintf((char*)send_buff,"$deadzone:%d#",data);
    MotorSendData(send_buff, strlen((char*)send_buff));
}

//发送电机磁环脉冲
void send_pulse_line(uint16_t data)
{
    uint8_t send_buff[50];
    sprintf((char*)send_buff,"$mline:%d#",data);
    MotorSendData(send_buff, strlen((char*)send_buff));
}

//发送电机减速比
void send_pulse_phase(uint16_t data)
{
    uint8_t send_buff[50];
    sprintf((char*)send_buff,"$mphase:%d#",data);
    MotorSendData(send_buff, strlen((char*)send_buff));
}

//发送轮子直径
void send_wheel_diameter(float data)
{
    uint8_t send_buff[50];
    sprintf((char*)send_buff,"$wdiameter:%.3f#",data);
    MotorSendData(send_buff, strlen((char*)send_buff));
}

//发送PID参数
void send_motor_PID(float P,float I,float D)
{
    uint8_t send_buff[50];
    sprintf((char*)send_buff,"$mpid:%.3f,%.3f,%.3f#",P,I,D);
    MotorSendData(send_buff, strlen((char*)send_buff));
}

//需要接收数据的开关
void send_upload_data(bool ALLEncoder_Switch,bool TenEncoder_Switch,bool Speed_Switch)
{
    uint8_t send_buff[50];
    sprintf((char*)send_buff,"$upload:%d,%d,%d#",ALLEncoder_Switch,TenEncoder_Switch,Speed_Switch);
    MotorSendData(send_buff, strlen((char*)send_buff));
}

//控制速度
void Contrl_Speed(int16_t M1_speed,int16_t M2_speed,int16_t M3_speed,int16_t M4_speed)
{
    uint8_t send_buff[50];
    sprintf((char*)send_buff,"$spd:%d,%d,%d,%d#",M1_speed,M2_speed,M3_speed,M4_speed);
    MotorSendData(send_buff, strlen((char*)send_buff));
}

//控制pwm
void Contrl_Pwm(int16_t M1_pwm,int16_t M2_pwm,int16_t M3_pwm,int16_t M4_pwm)
{
    uint8_t send_buff[50];
    sprintf((char*)send_buff,"$pwm:%d,%d,%d,%d#",M1_pwm,M2_pwm,M3_pwm,M4_pwm);
    MotorSendData(send_buff, strlen((char*)send_buff));
}

//测试
void Test_send()
{
    uint8_t send_buff[8] = {0,1,2,3,4,5,6,7};
    
    MotorSendData(send_buff,8);
}

//////////********************接收部分********************///////////
/**
 * @brief 分割字符串
 * @param mystrArray 输出字符串数组
 * @param str 输入字符串
 * @param delimiter 分隔符
 */
static void splitString(char* mystrArray[], char *str, const char *delimiter) 
{
    char *token = strtok(str, delimiter);
    mystrArray[0] = token;
    int i = 1;
    
    while (token != NULL) 
    {
        token = strtok(NULL, delimiter);
        mystrArray[i] = token;
        i++;
    }
}

/**
 * @brief 处理接收到的数据帧
 * @param frame 完整数据帧
 * @param len 数据帧长度
 */
static void ProcessMotorFrame(uint8_t *frame, uint16_t len)
{
    // 确保字符串以NULL结尾
    uint8_t temp_buf[MOTOR_PROTOCOL_MAX_FRAME_SIZE];
    uint16_t copy_len = len < MOTOR_PROTOCOL_MAX_FRAME_SIZE - 1 ? len : MOTOR_PROTOCOL_MAX_FRAME_SIZE - 1;
    memcpy(temp_buf, frame, copy_len);
    temp_buf[copy_len] = '\0';
    
    // 总编码器数据
    if (strncmp("MAll", (char*)temp_buf, 4) == 0)
    {
        char* strArray[5] = {NULL}; // 4个电机 + 结束标记
        
        // 跳过"MAll:"部分
        splitString(strArray, (char*)temp_buf + 5, ", ");
        
        for (int i = 0; i < 4 && strArray[i] != NULL; i++)
        {
            Encoder_Now[i] = atoi(strArray[i]);
        }
    }
    // 10ms编码器数据
    else if (strncmp("MTEP", (char*)temp_buf, 4) == 0)
    {
        char* strArray[5] = {NULL};
        
        // 跳过"MTEP:"部分
        splitString(strArray, (char*)temp_buf + 5, ", ");
        
        for (int i = 0; i < 4 && strArray[i] != NULL; i++)
        {
            Encoder_Offset[i] = atoi(strArray[i]);
        }
    }
    // 速度数据
    else if (strncmp("MSPD", (char*)temp_buf, 4) == 0)
    {
        char* strArray[5] = {NULL};
        
        // 跳过"MSPD:"部分
        splitString(strArray, (char*)temp_buf + 5, ", ");
        
        for (int i = 0; i < 4 && strArray[i] != NULL; i++)
        {
            g_Speed[i] = atof(strArray[i]);
        }
    }
}

/**
 * @brief 串口接收回调函数
 */
static void MotorRxCallback()
{
    uint8_t *buf = motor_usart->recv_buff;
    uint16_t buff_size = motor_usart->recv_buff_size;
    uint16_t processed = 0;

    while (processed < buff_size) {
        // 查找帧头
        uint8_t *start = memchr(buf + processed, '$', buff_size - processed);
        if (!start) break;
        
        // 查找帧尾
        uint8_t *end = memchr(start, '#', buff_size - (start - buf));
        if (!end) break;
        
        // 计算帧长度
        uint16_t frame_len = end - start + 1;
        
        // 处理有效帧
        if (frame_len > 2 && frame_len < MOTOR_PROTOCOL_MAX_FRAME_SIZE) {
            g_recv_flag = 1;
            ProcessMotorFrame(start + 1, frame_len - 2); // 去掉$和#
        }
        
        processed = (end - buf) + 1;
    }
}

/**
 * @brief 初始化电机串口通信
 * @param huart 串口句柄
 */
void MotorUSARTInit(UART_HandleTypeDef *huart)
{
    USART_Init_Config_s conf = {
        .module_callback = MotorRxCallback,
        .usart_handle = huart,
        .recv_buff_size = MOTOR_PROTOCOL_MAX_FRAME_SIZE,
    };
    motor_usart = USARTRegister(&conf);
    
    // 配置电机参数
    send_motor_type(MOTOR_520);           // 设置电机类型为M3508
    send_motor_deadzone(1600);          // 设置死区为50
    send_pulse_line(13);            // 设置磁环脉冲数为4096
    send_pulse_phase(20);
    send_wheel_diameter(48.0f);       // 设置轮子直径为76.5mm
    send_motor_PID(10.0f, 0.5f, 0.2f); // 设置PID参数
    
    // 开启数据上传
    send_upload_data(false,true,false); // 开启所有数据上传
}