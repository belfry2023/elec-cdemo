#include "module_laser.h"
#include "tim.h"
#include "stdint.h"
#include "string.h"
#include "stdlib.h"
static laser_t *laser[1] = {NULL};
static uint8_t idx;
laser_t *laser_init(laser_c *config)
{
    laser_t *_instance = (laser_t*)malloc(sizeof (laser_t));
    memset(_instance, 0, sizeof(laser_t));

    _instance->laser_tim_driver = config->laser_tim_driver;
    _instance->Channel = config->Channel;
    HAL_TIM_PWM_Start(_instance->laser_tim_driver,_instance->Channel);
    
    laser[idx++] = _instance;
    
    return _instance;
}

void laser_enable()
{
    for(uint8_t i = 0; i < idx; i++)
    {
         laser_t *l = laser[i];
         __HAL_TIM_SET_COMPARE(l->laser_tim_driver, l->Channel, 20000);
    }
}

void laser_disable()
{
    for(uint8_t i; i < idx; i++)
    {
         laser_t *l = laser[i];
         __HAL_TIM_SET_COMPARE(l->laser_tim_driver, l->Channel, 0);
    }
}