#include "module_key.h"
#include "gpio.h"
#include "bsp_dwt.h"
#include "string.h"
#include "general_def.h"
void Key_Init(KeyInstance *key, key_init_config_t *config)
{
    memset(key, 0, sizeof(KeyInstance)); // 初始化结构体
    memcpy(key,config,sizeof(key_init_config_t));
}

void Key_Scan(KeyInstance *key)
{   
    key->Key_Long_Press[LAST] = key->Key_Long_Press[NOW];
    key->Key_Short_Press[LAST] = key->Key_Short_Press[NOW];
    key->Key_Long_Press[NOW] = 0;
    key->Key_Short_Press[NOW] = 0;
    key->Key_Repeat_Press = 0;
    key->State = HAL_GPIO_ReadPin(key->GPIO,key->GPIO_PIN);
    if (HAL_GPIO_ReadPin(key->GPIO, key->GPIO_PIN) == key->Press_Level) {
        key->dt++;
        if(key->dt >= 2 && key->dt_last >= 2)
        {
            if(key->dt_last <= 10)
            {
                key->Key_Repeat_Press = 1;
            }else
            
            key->dt_last = 0;
        }
        if(key->dt >= 2)
        {
            key->isPressed = 1;
        }
    }else
    { 
        key->dt_last++;
        if(key->dt >= 2 && key->dt_last >= 2)
        {
            if (key->dt >= 10 && key->dt_last > 10 && key->Key_Repeat_Press == 0) 
            { // 长按超过1秒
                key->Key_Long_Press[NOW] = 1;
                key->dt = 0;
            } else 
            if(key->dt < 10 && key->dt_last > 10 && key->Key_Repeat_Press == 0)
            { // 短按
                key->Key_Short_Press[NOW] = 1;
                key->dt = 0;
            }
        }
        key->isPressed = 0;
        if(key->dt_last > 1000) // 如果超过1秒没有按下，重置时间间隔
        {
            key->dt_last = 100;
        }
    }
}