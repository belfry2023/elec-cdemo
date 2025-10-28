#ifndef BUFFER_H
#define BUFFER_H

#include "stdint.h"
#include "string.h"

// 可配置的队列大小，默认32（可根据需要调整）
#ifndef BUFFER_SIZE
#define BUFFER_SIZE 32
#endif

typedef struct 
{
    float queue[BUFFER_SIZE];
    uint8_t id, od;
    uint8_t size;           // 队列实际大小
    uint8_t data_count;     // 已存储数据计数
} buf_t;

// 原有函数
buf_t *BUFRegister();
float BUFUpdata(buf_t *_buf, float n, uint8_t time);

// 新增功能函数
float BUFGetHistory(buf_t *_buf, uint8_t steps_back);    // 获取历史数据
float BUFMovingAverage(buf_t *_buf, uint8_t window_size); // 移动平均滤波
float BUFWeightedAverage(buf_t *_buf, const float *weights, uint8_t window_size); // 加权平均
uint8_t BUFIsReady(buf_t *_buf);                         // 检查缓冲区状态

#endif