#include "filter.h"

void first_order_filter_init(first_order_filter_type_t *first_order_filter_type, fp32 frame_period,fp32 num)
{
    first_order_filter_type->frame_period = frame_period;
    first_order_filter_type->num = num;
    first_order_filter_type->input = 0.0f;
    first_order_filter_type->out = 0.0f;
}

void first_order_filter_cali(first_order_filter_type_t *first_order_filter_type, fp32 input)
{
    first_order_filter_type->input = input;
    first_order_filter_type->out =
		first_order_filter_type->num / (first_order_filter_type->num + first_order_filter_type->frame_period) * first_order_filter_type->out + 
		first_order_filter_type->frame_period / (first_order_filter_type->num + first_order_filter_type->frame_period) * first_order_filter_type->input;
}

float kalman_filter( float ResrcData,float ProcessNiose_Q,float MeasureNoise_R)
{
	float R = MeasureNoise_R;
  float Q = ProcessNiose_Q;

  static float x_last;
  float x_mid = x_last;
  float x_now;

  static float p_last;
  float p_mid ;
  float p_now;

  float kg;

  x_mid=x_last;                       //x_last=x(k-1|k-1),x_mid=x(k|k-1)
  p_mid=p_last+Q;                     //p_mid=p(k|k-1),p_last=p(k-1|k-1),Q=噪声

  /*
   *  卡尔曼滤波的五个重要公式
   */
  kg=p_mid/(p_mid+R);                 //kg为kalman filter，R 为噪声
  x_now=x_mid+kg*(ResrcData-x_mid);   //估计出的最优值
  p_now=(1-kg)*p_mid;                 //最优值对应的covariance
  p_last = p_now;                     //更新covariance 值
  x_last = x_now;                     //更新系统状态值

  return x_now;
}

void kalman_filter_create(kalman_filter_type_t *p,float T_Q,float T_R)
{
    p->X_last = (float)0;
    p->P_last = 0;
    p->Q = T_Q;
    p->R = T_R;
    p->A = 1;
		p->B = 0;
    p->H = 1;
    p->X_mid = p->X_last;
}

float kalman_filter_cali(kalman_filter_type_t* p,float dat)
{
    p->X_mid =p->A*p->X_last;                     //百度对应公式(1)    x(k|k-1) = A*X(k-1|k-1)+B*U(k)+W(K)
    p->P_mid = p->A*p->P_last+p->Q;               //百度对应公式(2)    p(k|k-1) = A*p(k-1|k-1)*A'+Q
    p->kg = p->P_mid/(p->P_mid+p->R);             //百度对应公式(4)    kg(k) = p(k|k-1)*H'/(H*p(k|k-1)*H'+R)
    p->X_now = p->X_mid+p->kg*(dat-p->X_mid);     //百度对应公式(3)    x(k|k) = X(k|k-1)+kg(k)*(Z(k)-H*X(k|k-1))
    p->P_now = (1-p->kg)*p->P_mid;                //百度对应公式(5)    p(k|k) = (I-kg(k)*H)*P(k|k-1)
    p->P_last = p->P_now;                         //状态更新
    p->X_last = p->X_now;
    return p->X_now;							  //输出预测结果x(k|k)
}
