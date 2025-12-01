#include "buffer.h"
#include "stdlib.h"

buf_t *BUFRegister()
{
    buf_t *_buf = (buf_t *)malloc(sizeof(buf_t));
    if (_buf == NULL) return NULL;
    
    memset(_buf, 0, sizeof(buf_t));
    _buf->id = 0;
    _buf->od = 1;
    _buf->size = BUFFER_SIZE;
    _buf->data_count = 0;
    return _buf;
}

float BUFUpdata(buf_t *_buf, float n, uint8_t time)
{
    if (!BUFIsReady(_buf)) return 0.0f;
    
    if (time >= _buf->size) {
        time = _buf->size - 1;
    }
    
    _buf->od %= time + 1;
    _buf->queue[_buf->id++] = n;
    _buf->id %= time + 1;
    
    if (_buf->data_count < _buf->size) {
        _buf->data_count++;
    }
    
    return _buf->queue[_buf->od++];
}

float BUFGetHistory(buf_t *_buf, uint8_t steps_back)
{
    if (!BUFIsReady(_buf) || steps_back >= _buf->size) {
        return 0.0f;
    }
    
    if (steps_back >= _buf->data_count) {
        return 0.0f;
    }
    
    int16_t history_idx = _buf->id - steps_back - 1;
    if (history_idx < 0) {
        history_idx += _buf->size;
    }
    
    return _buf->queue[history_idx % _buf->size];
}

float BUFMovingAverage(buf_t *_buf, uint8_t window_size)
{
    if (!BUFIsReady(_buf) || window_size == 0 || window_size > _buf->size) {
        return 0.0f;
    }
    
    float sum = 0.0f;
    uint8_t valid_samples = 0;
    
    for (uint8_t i = 0; i < window_size; i++) {
        float data = BUFGetHistory(_buf, i);
        if (i < _buf->data_count) {
            sum += data;
            valid_samples++;
        }
    }
    
    return (valid_samples > 0) ? (sum / valid_samples) : 0.0f;
}

float BUFWeightedAverage(buf_t *_buf, const float *weights, uint8_t window_size)
{
    if (!BUFIsReady(_buf) || weights == NULL || 
        window_size == 0 || window_size > _buf->size) {
        return 0.0f;
    }
    
    float result = 0.0f;
    float weight_sum = 0.0f;
    uint8_t valid_samples = 0;
    
    for (uint8_t i = 0; i < window_size; i++) {
        float data = BUFGetHistory(_buf, i);
        if (i < _buf->data_count) {
            result += data * weights[i];
            weight_sum += weights[i];
            valid_samples++;
        }
    }
    
    return (valid_samples > 0 && weight_sum > 0) ? (result / weight_sum) : 0.0f;
}

uint8_t BUFIsReady(buf_t *_buf)
{
    return (_buf != NULL && _buf->size > 0);
}

void BUFFree(buf_t *_buf)
{
    if (_buf != NULL) {
        free(_buf);
    }
}