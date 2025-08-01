#ifndef __MODULE_LASER_H
#define __MODULE_LASER_H

#include "tim.h"
#include "stdint.h"
#include "main.h"

typedef struct servo_typedef
{
    TIM_HandleTypeDef *laser_tim_driver; // 驱动定时器
    uint32_t Channel;                    // PWM通道
    uint8_t en;
    
} laser_t;

typedef struct servo_config
{
    TIM_HandleTypeDef *laser_tim_driver; // 驱动定时器
    uint32_t Channel;                    // PWM通道
    
} laser_c;

laser_t *laser_init(laser_c *config);


void laser_enable(); // 设置PWM输出
void laser_disable();
#endif  