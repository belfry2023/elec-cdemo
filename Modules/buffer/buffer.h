#ifndef BUFFER_H
#define BUFFER_H

#include "stdint.h"
#include "string.h"

// 可配置的队列大小
#ifndef BUFFER_SIZE
#define BUFFER_SIZE 32
#endif

typedef struct 
{
    float queue[BUFFER_SIZE];
    uint8_t id, od;
    uint8_t size;
    uint8_t data_count;
} buf_t;

// 原有函数
buf_t *BUFRegister();
float BUFUpdata(buf_t *_buf, float n, uint8_t time);

// 新增功能函数
float BUFGetHistory(buf_t *_buf, uint8_t steps_back);
float BUFMovingAverage(buf_t *_buf, uint8_t window_size);
float BUFWeightedAverage(buf_t *_buf, const float *weights, uint8_t window_size);
uint8_t BUFIsReady(buf_t *_buf);
void BUFFree(buf_t *_buf);

#endif