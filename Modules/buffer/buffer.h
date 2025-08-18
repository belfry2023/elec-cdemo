#ifndef BUFFER_H
#define BUFFER_H

#include "stdint.h"
#include "string.h"

typedef struct 
{
    float quene[256];
    uint8_t id, od;
}buf_t;

buf_t *BUFRegister();
float BUFUpdata(buf_t *_buf, float data, uint8_t n);

#endif