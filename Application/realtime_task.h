#pragma once

#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "task_init.h"
#include "OLED.h"
#include "dmmotor.h"
#include "module_dm-imu.h"
osThreadId insTaskHandle;
osThreadId testTaskHandle;
osThreadId motorTaskHandle;
osThreadId daemonTaskHandle;
void StartINSTASK(void const *argument);
void StartTESTTASK(void const *argument);
void StartMOTORTASK(void const *argument);
void StartDAEMONTASK(void const *argument);
void OSTaskInit()
{
    osThreadDef(instask, StartINSTASK, osPriorityAboveNormal, 0, 256);
    insTaskHandle = osThreadCreate(osThread(instask), NULL); // 由于是阻塞读取传感器,为姿态解算设置较高优先级,确保以1khz的频率执行
    // 后续修改为读取传感器数据准备好的中断处理,

    osThreadDef(testtask, StartTESTTASK, osPriorityNormal, 0, 2048);
    testTaskHandle = osThreadCreate(osThread(testtask), NULL);
    
    osThreadDef(motortask, StartMOTORTASK, osPriorityNormal, 0, 256);
    motorTaskHandle = osThreadCreate(osThread(motortask), NULL);

    osThreadDef(daemontask, StartDAEMONTASK, osPriorityNormal, 0, 128);
    daemonTaskHandle = osThreadCreate(osThread(daemontask), NULL);

    DMMotorControlInit();
    
}



__attribute__((noreturn)) void StartINSTASK(void const *argument)
{
    INS_Init(); // 确保BMI088被正确初始化.
    static uint32_t ins_cnt;
    for (;;)
    {
        ins_cnt = 1;
        INS_Task();
        // IMU_Task();
        ins_cnt = 0;
        osDelay(1);
        ins_cnt = 2;
    }
}

__attribute__((noreturn)) void StartTESTTASK(void const *argument)
{
    static uint32_t test_cnt;
    for (;;)
    {
        test_cnt = 1;
        task_run();
        test_cnt = 0;
        osDelay(1);
        test_cnt = 2;
    }
}

__attribute__((noreturn)) void StartMOTORTASK(void const *argument)
{
    static uint32_t motor_cnt;
    for (;;)
    {
        motor_cnt = 1;
        MotorControlTask();
        motor_cnt = 0;
        osDelay(1);
        motor_cnt = 2;
    }
}

__attribute__((noreturn)) void StartDAEMONTASK(void const *argument)
{
    static uint32_t daemon_cnt;
    for (;;)
    {
        daemon_cnt = 1;
        DaemonTask();
        daemon_cnt = 0;
        osDelay(10);
        daemon_cnt = 2;
    }
}