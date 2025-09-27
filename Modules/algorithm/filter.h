#ifndef __FILTER_H
#define __FILTER_H

typedef float fp32;

typedef struct
{
    fp32 input;        //输入数据
    fp32 out;          //滤波输出的数据
    fp32 num;       //滤波参数
    fp32 frame_period; //滤波的时间间隔 单位 s
}first_order_filter_type_t;

typedef struct {
    float X_last; //上一时刻的最优结果  X(k-|k-1)
    float X_mid;  //当前时刻的预测结果  X(k|k-1)
    float X_now;  //当前时刻的最优结果  X(k|k)
    float P_mid;  //当前时刻预测结果的协方差  P(k|k-1)
    float P_now;  //当前时刻最优结果的协方差  P(k|k)
    float P_last; //上一时刻最优结果的协方差  P(k-1|k-1)
    float kg;     //kalman增益
    float A;      //系统参数
		float B;
    float Q;
    float R;
    float H;
}kalman_filter_type_t;

extern void first_order_filter_init(first_order_filter_type_t *first_order_filter_type, fp32 frame_period,fp32 num);
extern void first_order_filter_cali(first_order_filter_type_t *first_order_filter_type, fp32 input);

float kalman_filter( float ResrcData,float ProcessNiose_Q,float MeasureNoise_R);
void kalman_filter_create(kalman_filter_type_t *p,float T_Q,float T_R);
float kalman_filter_cali(kalman_filter_type_t* p,float dat);

#endif
