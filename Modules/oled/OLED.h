#ifndef __OLED_H
#define __OLED_H

#include "main.h"
#include <stdint.h>
#include "OLED_Data.h"
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdarg.h>

/************************************* 以下为参数宏定义*******************************************/

/**
 * FontSize参数取值
 * 此参数值不仅用于判断，而且用于计算横向字符偏移，默认值为字体像素宽度
 */
#define OLED_8X16               8
#define OLED_6X8                6

/*IsFilled参数数值*/
#define OLED_UNFILLED           0
#define OLED_FILLED             1

/*******************移植需要修改的宏定义******************/
/* 如果用到中文，编译器附加选项需要加 --no-multibyte-chars (用AC6编译器的不用加) */

/**
 * 选择OLED驱动方式，默认使用硬件I2C。
 * 如果要用软件I2C就将硬件I2C那行的宏定义注释掉，将软件I2C那行的注释取消。
 * 不能同时两个都同时取消注释！
 */

//#define OLED_13                                      /* 1.3寸OLED */
#define OLED_96                                        /* 0.96寸OLED */

#define OLED_USE_HW_I2C                                /* 硬件I2C */
//#define OLED_USE_SW_I2C                              /* 软件I2C */

#ifdef OLED_USE_HW_I2C
#define OLED_I2C                hi2c2                  /* 定义OLED屏使用hi2cx接口 */
extern  I2C_HandleTypeDef       hi2c2;                 /* HAL库使用，指定硬件IIC接口 */
extern uint8_t OLED_FLAG;
#endif

/* STM32F103C8T6芯片的硬件I2C1: PB6 -- SCL; PB7 -- SDA
 * 需要在stm32cubemx代码初始化时将SCL和SDA引脚的"user lable"分别设置为对应的I2Cx_SCL和I2Cx_SDA。
 */

#define OLED_SCL                I2C_SCL_Pin            /* I2Cx_SCL引脚 */
#define OLED_SDA                I2C_SDA_Pin            /* I2Cx_SDA引脚 */
#define OLED_SCL_GPIO_Port      I2C_SCL_GPIO_Port      /* I2Cx_SCL端口 */
#define OLED_SDA_GPIO_Port      I2C_SDA_GPIO_Port      /* I2Cx_SDA端口 */

/*******************移植需要修改的宏定义******************/

/**
 * OLED从机地址
 * 0x3C是OLED的7位地址，左移1位最后位做读写位变成0x78
 */
#define OLED_ADDRESS 0x3C << 1

/* I2C超时时间 */
#define OLED_I2C_TIMEOUT 10

/**
 * 软件I2C用的延时时间
 * 下面数值为170MHz主频要延时的值，如果你的主频不一样可以修改一下
 * 100MHz以内的主频改成0就行
 */
#define Delay_time 0

/***************************************以下为函数声明*******************************************/

typedef struct {
    float x;
    float y;
    float z;
} Point3D;

// 立方体定义
typedef struct {
    Point3D vertices[8]; // 8个顶点
    int edges[12][2];    // 12条边
    float pitch; // X轴旋转
    float yaw;   // Y轴旋转
    float roll;  // Z轴旋转
} Cube;

typedef enum {
    MOTOR_RUNNING,
    MOTOR_STOPPING,
} MotorState;

// 电机结构体
typedef struct {
    int x;              // 电机图标左上角X坐标
    int y;              // 电机图标左上角Y坐标
    int width;          // 电机图标宽度
    int height;         // 电机图标高度
    float angle;        // 当前旋转角度(弧度)
    float speed;        // 旋转速度(弧度/帧)
    float torque;
    MotorState state;   // 电机状态
    int power;          // 功率百分比(0-100)
} MotorIcon;

typedef struct {
    uint16_t data[256];
    int front;
    int rear;
    int count;
} CircularQueue;

// 波形结构体
typedef struct {
     int16_t* dataSource;      // 注册的数据源地址
    CircularQueue dataQueue;  // 数据队列
    float timePerDivision;    // 时间单位 (ms/div)
    float valuePerDivision;   // 值量程 (单位/div)
    float refreshRate;        // 刷新频率 (Hz)
    uint32_t lastSampleTime;  // 上次采样时间
    uint32_t lastRefreshTime; // 上次刷新时间
    float sampleInterval;     // 采样间隔(ms)
    uint8_t zeroLine;         // 零线位置 (固定为32)
} Waveform;

/*初始化函数*/
void OLED_Init(void);

/*更新函数*/
void OLED_Update(void);
void OLED_UpdateArea(int16_t L_X, int16_t L_Y, uint8_t Width, uint8_t Height);

/*显存控制函数*/
void OLED_Clear(void);
void OLED_ClearArea(int16_t L_X, int16_t L_Y, uint8_t Width, uint8_t Height);
void OLED_Reverse(void);
void OLED_ReverseArea(int16_t L_X, int16_t L_Y, uint8_t Width, uint8_t Height);

/*显示函数*/
void OLED_ShowChar(int16_t L_X, int16_t L_Y, char Char, uint8_t FontSize);
void OLED_ShowString(int16_t L_X, int16_t L_Y, char *String, uint8_t FontSize);
void OLED_ShowNum(int16_t L_X, int16_t L_Y, uint32_t Number, uint8_t Length, uint8_t FontSize);
void OLED_ShowSignedNum(int16_t L_X, int16_t L_Y, int32_t Number, uint8_t Length, uint8_t FontSize);
void OLED_ShowHexNum(int16_t L_X, int16_t L_Y, uint32_t Number, uint8_t Length, uint8_t FontSize);
void OLED_ShowBinNum(int16_t L_X, int16_t L_Y, uint32_t Number, uint8_t Length, uint8_t FontSize);
void OLED_ShowFloatNum(int16_t L_X, int16_t L_Y, double Number, uint8_t IntLength, uint8_t FraLength, uint8_t FontSize);
void OLED_ShowImage(int16_t L_X, int16_t L_Y, uint8_t Width, uint8_t Height, const uint8_t *Image);
void OLED_Printf(int16_t L_X, int16_t L_Y, uint8_t FontSize, char *format, ...);

/*绘图函数*/
void OLED_DrawPoint(int16_t L_X, int16_t L_Y);
uint8_t OLED_GetPoint(int16_t L_X, int16_t L_Y);
void OLED_DrawLine(int16_t X0, int16_t Y0, int16_t X1, int16_t Y1);
void OLED_DrawRectangle(int16_t L_X, int16_t L_Y, uint8_t Width, uint8_t Height, uint8_t IsFilled);
void OLED_DrawTriangle(int16_t X0, int16_t Y0, int16_t X1, int16_t Y1, int16_t X2, int16_t Y2, uint8_t IsFilled);
void OLED_DrawCircle(int16_t L_X, int16_t L_Y, uint8_t Radius, uint8_t IsFilled);
void OLED_DrawEllipse(int16_t L_X, int16_t L_Y, uint8_t A, uint8_t B, uint8_t IsFilled);
void OLED_DrawArc(int16_t L_X, int16_t L_Y, uint8_t Radius, int16_t StartAngle, int16_t EndAngle, uint8_t IsFilled);
void initCube(Cube *cube);
void drawCube(Cube *cube);
void drawAxes(Cube *cube);
void initMotor(MotorIcon *motor,uint8_t id);
void drawMotorIcon(MotorIcon *motor);
void Waveform_Register(Waveform *wave, uint16_t* dataSource);
void Waveform_SetParameters(Waveform *wave, float timePerDiv, float valuePerDivision, float refreshRate);
void Waveform_Update(Waveform *wave);
void Waveform_Draw(Waveform *wave);
#endif


/*****************江协科技|版权所有****************/
/*****************jiangxiekeji.com*****************/
