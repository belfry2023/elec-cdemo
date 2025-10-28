#include "smith_predictor.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

// 史密斯预估器实现

SmithPredictor* smith_predictor_create(double kp, double ki, double kd, 
                                      double process_gain, double time_constant, 
                                      double process_delay, double dt) {
    SmithPredictor* sp = (SmithPredictor*)malloc(sizeof(SmithPredictor));
    if (!sp) return NULL;
    
    sp->kp = kp;
    sp->ki = ki;
    sp->kd = kd;
    sp->process_gain = process_gain;
    sp->time_constant = time_constant;
    sp->process_delay = process_delay;
    sp->dt = dt;
    
    // 计算延迟采样数
    sp->delay_samples = (uint32_t)(process_delay / dt);
    sp->buffer_size = sp->delay_samples + 2; // 额外空间确保安全
    
    // 分配延迟缓冲区
    sp->delay_buffer = (double*)malloc(sp->buffer_size * sizeof(double));
    if (!sp->delay_buffer) {
        free(sp);
        return NULL;
    }
    
    // 初始化缓冲区
    memset(sp->delay_buffer, 0, sp->buffer_size * sizeof(double));
    sp->write_index = 0;
    sp->read_index = (sp->delay_samples) % sp->buffer_size;
    
    sp->setpoint = 0.0;
    sp->output = 0.0;
    sp->last_error = 0.0;
    sp->integral = 0.0;
    sp->initialized = 1;
    
    return sp;
}

void smith_predictor_destroy(SmithPredictor* sp) {
    if (sp) {
        if (sp->delay_buffer) {
            free(sp->delay_buffer);
        }
        free(sp);
    }
}

void smith_predictor_reset(SmithPredictor* sp) {
    if (!sp || !sp->delay_buffer) return;
    
    memset(sp->delay_buffer, 0, sp->buffer_size * sizeof(double));
    sp->write_index = 0;
    sp->read_index = (sp->delay_samples) % sp->buffer_size;
    sp->setpoint = 0.0;
    sp->output = 0.0;
    sp->last_error = 0.0;
    sp->integral = 0.0;
}

double smith_predictor_update(SmithPredictor* sp, double setpoint, double measurement) {
   // 检查史密斯预估器结构体指针是否有效以及是否已完成初始化
// 如果指针为空或未初始化，直接返回0.0，避免后续操作引发错误
if (!sp || !sp->initialized) return 0.0;

// 更新设定值为新的目标值
// setpoint: 期望的系统输出目标值
sp->setpoint = setpoint;

// 更新延迟缓冲区：将当前控制输出存入环形缓冲区
// 实现公式：buffer[write_index] = output
// 使用环形缓冲区模拟纯时间延迟环节
sp->delay_buffer[sp->write_index] = sp->output;

// 更新写指针位置，采用模运算实现环形缓冲区循环
// write_index = (write_index + 1) % buffer_size
sp->write_index = (sp->write_index + 1) % sp->buffer_size;

// 从延迟缓冲区读取延迟后的控制输出
// delayed_output = buffer[read_index]
// 该值代表经过纯延迟τ时间后的历史控制输出
double delayed_output = sp->delay_buffer[sp->read_index];

// 更新读指针位置，保持与写指针的固定距离
// read_index = (read_index + 1) % buffer_size
sp->read_index = (sp->read_index + 1) % sp->buffer_size;

// 计算无延迟过程模型输出（一阶惯性环节）
// 离散化公式：y(k) = α × y(k-1) + (1-α) × u(k) × K
// 其中：α = e^(-Δt/τ)，τ为过程时间常数，K为过程增益
static double model_state = 0.0;
double alpha = exp(-sp->dt / sp->time_constant);
model_state = alpha * model_state + (1 - alpha) * sp->output * sp->process_gain;

// 计算带延迟的过程模型输出
// 使用延迟后的控制输出delayed_output作为输入
// 公式：y_delayed(k) = α × y_delayed(k-1) + (1-α) × u(k-τ) × K
static double delayed_model_state = 0.0;
delayed_model_state = alpha * delayed_model_state + (1 - alpha) * delayed_output * sp->process_gain;

// 史密斯预估器核心计算：修正后的误差
// 标准误差：e = setpoint - measurement
// 史密斯修正：e_corrected = setpoint - [measurement + G(0) - G(τ)]
// 其中：G(0)为无延迟模型输出，G(τ)为带延迟模型输出
// 这样就从实际反馈中消除了纯延迟的影响
double error = setpoint - (measurement + model_state - delayed_model_state);

// PID控制器积分项计算
// 离散积分公式：I(k) = I(k-1) + e(k) × Δt
sp->integral += error * sp->dt;

// PID控制器微分项计算
// 离散微分公式：D(k) = [e(k) - e(k-1)] / Δt
double derivative = (error - sp->last_error) / sp->dt;

// PID控制器输出计算
// 标准PID公式：u(k) = Kp × e(k) + Ki × I(k) + Kd × D(k)
sp->output = sp->kp * error + sp->ki * sp->integral + sp->kd * derivative;

// 输出限幅和积分抗饱和处理
// 防止控制输出超出执行器物理限制
const double output_limit = 10000.0;
if (sp->output > output_limit) {
    sp->output = output_limit;           // 上限限幅
    sp->integral -= error * sp->dt;      // 积分抗饱和：当输出饱和时停止积分累积
} else if (sp->output < -output_limit) {
    sp->output = -output_limit;          // 下限限幅
    sp->integral -= error * sp->dt;      // 积分抗饱和
}

// 保存当前误差用于下一次微分计算
// last_error = error
sp->last_error = error;

// 返回最终的控制输出值
return sp->output;
}

void smith_predictor_set_parameters(SmithPredictor* sp, double kp, double ki, double kd) {
    if (!sp) return;
    sp->kp = kp;
    sp->ki = ki;
    sp->kd = kd;
}

void smith_predictor_set_process_model(SmithPredictor* sp, double gain, double time_constant, double delay) {
    if (!sp) return;
    sp->process_gain = gain;
    sp->time_constant = time_constant;
    sp->process_delay = delay;
    
    // 重新计算延迟缓冲区
    uint32_t new_delay_samples = (uint32_t)(delay / sp->dt);
    if (new_delay_samples != sp->delay_samples) {
        sp->delay_samples = new_delay_samples;
        sp->buffer_size = sp->delay_samples + 2;
        
        if (sp->delay_buffer) {
            free(sp->delay_buffer);
        }
        sp->delay_buffer = (double*)malloc(sp->buffer_size * sizeof(double));
        memset(sp->delay_buffer, 0, sp->buffer_size * sizeof(double));
        sp->write_index = 0;
        sp->read_index = (sp->delay_samples) % sp->buffer_size;
    }
}

// 普通PID控制器实现

PIDController* pid_controller_create(double kp, double ki, double kd, double dt) {
    PIDController* pid = (PIDController*)malloc(sizeof(PIDController));
    if (!pid) return NULL;
    
    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;
    pid->dt = dt;
    pid->setpoint = 0.0;
    pid->integral = 0.0;
    pid->last_error = 0.0;
    
    return pid;
}

void pid_controller_destroy(PIDController* pid) {
    if (pid) {
        free(pid);
    }
}

double pid_controller_update(PIDController* pid, double setpoint, double measurement) {
    if (!pid) return 0.0;
    
    pid->setpoint = setpoint;
    double error = setpoint - measurement;
    
    pid->integral += error * pid->dt;
    double derivative = (error - pid->last_error) / pid->dt;
    
    double output = pid->kp * error + pid->ki * pid->integral + pid->kd * derivative;
    
    // 限制输出和抗饱和
    const double output_limit = 100.0;
    if (output > output_limit) {
        output = output_limit;
        pid->integral -= error * pid->dt;
    } else if (output < -output_limit) {
        output = -output_limit;
        pid->integral -= error * pid->dt;
    }
    
    pid->last_error = error;
    return output;
}

void pid_controller_reset(PIDController* pid) {
    if (!pid) return;
    pid->setpoint = 0.0;
    pid->integral = 0.0;
    pid->last_error = 0.0;
}