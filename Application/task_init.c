#include "task_init.h"
#include "app_chassis.h"
#include "app_gimbal.h"
#include "app_cmd.h"
#include "realtime_task.h"
#include "bsp_dwt.h"
#include "app_ui.h"
void task_init(void)
{
    __disable_irq();
    
    DWT_Init(168);

    oled_ui_init();
    OLED_ClearArea(10, 4, 96, 30);
    OLED_ShowString(10, 4, "OLED OK!", OLED_6X8);
    OLED_ShowString(10, 14, "CMD Init...", OLED_6X8);
	OLED_DrawRectangle(0, 40, 32, 15, OLED_FILLED);
    OLED_DrawRectangle(0, 40, 96, 15, OLED_UNFILLED);
	OLED_Update();

    cmd_init();
    OLED_ClearArea(10, 4, 96, 30);
    OLED_ShowString(10, 4, "CMD OK!", OLED_6X8);
    OLED_ShowString(10, 14, "Gimbal Init...", OLED_6X8);
	OLED_DrawRectangle(0, 40, 64, 15, OLED_FILLED);
    OLED_DrawRectangle(0, 40, 96, 15, OLED_UNFILLED);
	OLED_Update();

    gimbal_init();
    OLED_ClearArea(10, 4, 96, 30);
    OLED_ShowString(10, 4, "Gimbal OK!", OLED_6X8);
    OLED_ShowString(10, 14, "Chassis Init...", OLED_6X8);
	OLED_DrawRectangle(0, 40, 96, 15, OLED_FILLED);
    OLED_DrawRectangle(0, 40, 96, 15, OLED_UNFILLED);
	OLED_Update();

    // 初始化底盘
    // chassis_init();
    // OLED_ClearArea(10, 4, 96, 30);
    // OLED_ShowString(10, 4, "Gimbal OK!", OLED_6X8);
    // OLED_ShowString(10, 14, "Chassis Init...", OLED_6X8);
	// OLED_DrawRectangle(0, 40, 72, 15, OLED_FILLED);
    // OLED_DrawRectangle(0, 40, 96, 15, OLED_UNFILLED);
	// OLED_Update();
    // 其他任务初始化可以在这里添加
    
    OSTaskInit(); // 如果需要使用FreeRTOS任务调度器，可以取消注释
    OLED_ClearArea(10, 4, 96, 30);
    OLED_ShowString(10, 4, "Thread Started!", OLED_6X8);
    OLED_ShowString(10, 14, "Task Run!", OLED_6X8);
	OLED_DrawRectangle(0, 40, 96, 15, OLED_FILLED);
	OLED_DrawRectangle(0, 40, 96, 15, OLED_UNFILLED);
	OLED_Update();
    OLED_Clear();
    __enable_irq();
    
}


void task_run(void)
{
    oled_ui_task();
    cmd_task(); // 执行命令处理任务
    gimbal_task(); // 执行云台任务
    // chassis_task(); // 执行底盘任务
}