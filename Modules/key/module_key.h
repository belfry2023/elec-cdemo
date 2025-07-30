#ifndef __MODULE_KEY_H
#define __MODULE_KEY_H

#include "gpio.h"
#include "bsp_dwt.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal.h"
typedef struct {
    GPIO_TypeDef* GPIO;
    uint16_t GPIO_PIN;
    GPIO_PinState Press_Level;
    uint8_t Key_Long_Press[2];
    uint8_t Key_Short_Press[2];
    uint8_t Key_Repeat_Press; // 重复按下标志
    uint8_t isPressed;
    float dt;
    float dt_last; // 上一次按键扫描的时间间隔
    GPIO_PinState State;
} KeyInstance;

typedef struct key_init_config
{
    GPIO_TypeDef* GPIO;
    uint16_t GPIO_PIN;
    GPIO_PinState Press_Level;
    /* data */
}key_init_config_t;


void Key_Init(KeyInstance *key, key_init_config_t *config);
void Key_Scan(KeyInstance *key);
#endif