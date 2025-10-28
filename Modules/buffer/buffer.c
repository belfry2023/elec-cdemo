#include "buffer.h"
#include "stdlib.h"
#include "math.h"
#include "string.h"
#include "buffer_advanced.h"
// IIR滤波器结构体
// typedef struct {
//     float *b;           // 分子系数
//     float *a;           // 分母系数  
//     float *x_history;   // 输入历史
//     float *y_history;   // 输出历史
//     uint8_t b_order;    // b系数阶数
//     uint8_t a_order;    // a系数阶数
// } iir_filter_t;

// // 控制系统模拟器结构体
// typedef struct {
//     buf_t *buffer_yaw;
//     buf_t *buffer_pitch;
//     uint32_t time_step;
// } control_simulator_t;

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

// ==================== IIR滤波器实现 ====================

// 创建IIR滤波器
iir_filter_t* IIRFilterCreate(const float *b_coeffs, uint8_t b_order, const float *a_coeffs, uint8_t a_order)
{
    iir_filter_t *filter = (iir_filter_t*)malloc(sizeof(iir_filter_t));
    if (filter == NULL) return NULL;
    
    filter->b_order = b_order;
    filter->a_order = a_order;
    
    // 分配系数内存
    filter->b = (float*)malloc(b_order * sizeof(float));
    filter->a = (float*)malloc(a_order * sizeof(float));
    
    // 分配历史数据内存
    filter->x_history = (float*)malloc(b_order * sizeof(float));
    filter->y_history = (float*)malloc((a_order - 1) * sizeof(float));
    
    if (filter->b == NULL || filter->a == NULL || 
        filter->x_history == NULL || filter->y_history == NULL) {
        IIRFilterFree(filter);
        return NULL;
    }
    
    // 初始化系数
    memcpy(filter->b, b_coeffs, b_order * sizeof(float));
    memcpy(filter->a, a_coeffs, a_order * sizeof(float));
    
    // 初始化历史数据为0
    memset(filter->x_history, 0, b_order * sizeof(float));
    memset(filter->y_history, 0, (a_order - 1) * sizeof(float));
    
    return filter;
}

// IIR滤波器更新
float IIRFilterUpdate(iir_filter_t *filter, float input)
{
    if (filter == NULL) return 0.0f;
    
    // 移动输入历史
    for (int i = filter->b_order - 1; i > 0; i--) {
        filter->x_history[i] = filter->x_history[i - 1];
    }
    filter->x_history[0] = input;
    
    // 计算输出
    float output = 0.0f;
    
    // 前向路径 (b系数)
    for (uint8_t i = 0; i < filter->b_order; i++) {
        output += filter->b[i] * filter->x_history[i];
    }
    
    // 反馈路径 (a系数，跳过a[0])
    for (uint8_t i = 1; i < filter->a_order; i++) {
        output -= filter->a[i] * filter->y_history[filter->a_order - 1 - i];
    }
    
    // 移动输出历史
    for (int i = filter->a_order - 2; i > 0; i--) {
        filter->y_history[i] = filter->y_history[i - 1];
    }
    if (filter->a_order > 1) {
        filter->y_history[0] = output;
    }
    
    return output;
}

// 重置IIR滤波器
void IIRFilterReset(iir_filter_t *filter)
{
    if (filter == NULL) return;
    
    memset(filter->x_history, 0, filter->b_order * sizeof(float));
    memset(filter->y_history, 0, (filter->a_order - 1) * sizeof(float));
}

// 释放IIR滤波器
void IIRFilterFree(iir_filter_t *filter)
{
    if (filter != NULL) {
        if (filter->b != NULL) free(filter->b);
        if (filter->a != NULL) free(filter->a);
        if (filter->x_history != NULL) free(filter->x_history);
        if (filter->y_history != NULL) free(filter->y_history);
        free(filter);
    }
}

// ==================== 高级滤波器设计 ====================

// 指数平滑滤波器设计
void AdvancedFilter_ExponentialSmoothing(float alpha, float *b, float *a)
{
    b[0] = alpha;
    a[0] = 1.0f;
    a[1] = alpha - 1.0f;
}

// 移动平均滤波器设计 (FIR)
void AdvancedFilter_MovingAverage(uint8_t window_size, float *b, float *a)
{
    float weight = 1.0f / window_size;
    for (uint8_t i = 0; i < window_size; i++) {
        b[i] = weight;
    }
    a[0] = 1.0f;
}

// 简化的巴特沃斯低通滤波器设计 (二阶)
void AdvancedFilter_ButterworthLowpass(float cutoff_freq, float sample_freq, float *b, float *a)
{
    float nyquist = sample_freq / 2.0f;
    float normalized_cutoff = cutoff_freq / nyquist;
    float theta_c = 3.14159265358979323846f * normalized_cutoff;
    float k = tanf(theta_c / 2.0f);
    float k2 = k * k;
    float sqrt2 = 1.41421356237f; // sqrt(2)
    
    float a0 = 1.0f + sqrt2 * k + k2;
    b[0] = k2 / a0;
    b[1] = 2.0f * b[0];
    b[2] = b[0];
    a[0] = 1.0f;
    a[1] = 2.0f * (k2 - 1.0f) / a0;
    a[2] = (1.0f - sqrt2 * k + k2) / a0;
}

// ==================== 控制系统模拟器 ====================

// 创建控制系统模拟器
control_simulator_t* ControlSimulatorCreate(void)
{
    control_simulator_t *sim = (control_simulator_t*)malloc(sizeof(control_simulator_t));
    if (sim == NULL) return NULL;
    
    sim->buffer_yaw = BUFRegister();
    sim->buffer_pitch = BUFRegister();
    sim->time_step = 0;
    
    if (sim->buffer_yaw == NULL || sim->buffer_pitch == NULL) {
        ControlSimulatorFree(sim);
        return NULL;
    }
    
    return sim;
}

// 模拟正弦波信号
float ControlSimulator_SineWave(control_simulator_t *sim, float amplitude, float frequency, float phase)
{
    if (sim == NULL) return 0.0f;
    return amplitude * sinf(2.0f * 3.14159265358979323846f * frequency * sim->time_step + phase);
}

// 模拟噪声信号
float ControlSimulator_Noise(float amplitude)
{
    return amplitude * ((float)rand() / RAND_MAX - 0.5f);
}

// 执行一步控制循环
control_result_t ControlSimulator_Step(control_simulator_t *sim, float target_yaw, float target_pitch, uint8_t delay, float k)
{
    control_result_t result = {0};
    
    if (sim == NULL) return result;
    
    sim->time_step++;
    
    // 延迟补偿处理
    float delayed_yaw = BUFUpdata(sim->buffer_yaw, target_yaw, delay);
    float delayed_pitch = BUFUpdata(sim->buffer_pitch, target_pitch, delay);
    
    // 移动平均滤波
    float filtered_yaw = BUFMovingAverage(sim->buffer_yaw, 4);
    
    // 延迟补偿
    float compensated_yaw = delayed_yaw + k * (filtered_yaw - delayed_yaw);
    
    result.time_step = sim->time_step;
    result.target_yaw = target_yaw;
    result.target_pitch = target_pitch;
    result.delayed_yaw = delayed_yaw;
    result.delayed_pitch = delayed_pitch;
    result.filtered_yaw = filtered_yaw;
    result.compensated_yaw = compensated_yaw;
    
    return result;
}

// 释放控制系统模拟器
void ControlSimulatorFree(control_simulator_t *sim)
{
    if (sim != NULL) {
        if (sim->buffer_yaw != NULL) BUFFree(sim->buffer_yaw);
        if (sim->buffer_pitch != NULL) BUFFree(sim->buffer_pitch);
        free(sim);
    }
}