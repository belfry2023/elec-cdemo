#ifndef SMITH_PREDICTOR_H
#define SMITH_PREDICTOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

// 史密斯预估器结构体
typedef struct {
    double kp;          // 比例增益
    double ki;          // 积分增益
    double kd;          // 微分增益
    
    double setpoint;    // 设定值
    double output;      // 控制器输出
    double last_error;  // 上一次误差
    double integral;    // 积分项
    
    // 过程模型参数 (一阶惯性加纯延迟)
    double process_gain;    // 过程增益
    double time_constant;   // 时间常数
    double process_delay;   // 过程延迟
    
    // 延迟缓冲区
    double *delay_buffer;   // 延迟缓冲区
    uint32_t buffer_size;   // 缓冲区大小
    uint32_t delay_samples; // 延迟采样数
    uint32_t write_index;   // 写入索引
    uint32_t read_index;    // 读取索引
    
    double dt;          // 采样时间
    uint32_t initialized; // 初始化标志
} SmithPredictor;

// 函数声明
SmithPredictor* smith_predictor_create(double kp, double ki, double kd, 
                                      double process_gain, double time_constant, 
                                      double process_delay, double dt);
void smith_predictor_destroy(SmithPredictor* sp);
void smith_predictor_reset(SmithPredictor* sp);
double smith_predictor_update(SmithPredictor* sp, double setpoint, double measurement);
void smith_predictor_set_parameters(SmithPredictor* sp, double kp, double ki, double kd);
void smith_predictor_set_process_model(SmithPredictor* sp, double gain, double time_constant, double delay);

// 普通PID控制器用于对比
typedef struct {
    double kp, ki, kd;
    double setpoint;
    double integral;
    double last_error;
    double dt;
} PIDController;

PIDController* pid_controller_create(double kp, double ki, double kd, double dt);
void pid_controller_destroy(PIDController* pid);
double pid_controller_update(PIDController* pid, double setpoint, double measurement);
void pid_controller_reset(PIDController* pid);

#ifdef __cplusplus
}
#endif

#endif // SMITH_PREDICTOR_H