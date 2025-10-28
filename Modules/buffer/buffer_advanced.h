#ifndef BUFFER_ADVANCED_H
#define BUFFER_ADVANCED_H

#include "buffer.h"
#include <stdint.h>

// IIR滤波器结构体
typedef struct {
    float *b;           // 分子系数
    float *a;           // 分母系数  
    float *x_history;   // 输入历史
    float *y_history;   // 输出历史
    uint8_t b_order;    // b系数阶数
    uint8_t a_order;    // a系数阶数
} iir_filter_t;

// 控制结果结构体
typedef struct {
    uint32_t time_step;
    float target_yaw;
    float target_pitch;
    float delayed_yaw;
    float delayed_pitch;
    float filtered_yaw;
    float compensated_yaw;
} control_result_t;

// 控制系统模拟器结构体
typedef struct {
    buf_t *buffer_yaw;
    buf_t *buffer_pitch;
    uint32_t time_step;
} control_simulator_t;

// IIR滤波器函数
iir_filter_t* IIRFilterCreate(const float *b_coeffs, uint8_t b_order, const float *a_coeffs, uint8_t a_order);
float IIRFilterUpdate(iir_filter_t *filter, float input);
void IIRFilterReset(iir_filter_t *filter);
void IIRFilterFree(iir_filter_t *filter);

// 高级滤波器设计
void AdvancedFilter_ExponentialSmoothing(float alpha, float *b, float *a);
void AdvancedFilter_MovingAverage(uint8_t window_size, float *b, float *a);
void AdvancedFilter_ButterworthLowpass(float cutoff_freq, float sample_freq, float *b, float *a);

// 控制系统模拟器
control_simulator_t* ControlSimulatorCreate(void);
float ControlSimulator_SineWave(control_simulator_t *sim, float amplitude, float frequency, float phase);
float ControlSimulator_Noise(float amplitude);
control_result_t ControlSimulator_Step(control_simulator_t *sim, float target_yaw, float target_pitch, uint8_t delay, float k);
void ControlSimulatorFree(control_simulator_t *sim);

#endif