#include "FIR_lib.h"

float KalmanFilter(FIR_TypeDef* FIR,float ADC_Value)
{
	float x_k1_k1,x_k_k1;
	float Z_k;

	float Kg = 0;
	float P_k_k1;

	float kalman_adc;

	Z_k = ADC_Value;

	if (my_abs(FIR->x_last-ADC_Value)>=0.5f)
	{ 
			x_k1_k1= ADC_Value*0.382f + FIR->x_last*0.618f;
	}else
	{
			x_k1_k1 = FIR->x_last;
	}
	x_k_k1 = x_k1_k1;
	P_k_k1 = FIR->p_last + FIR->Q;

	Kg = P_k_k1/(P_k_k1 + FIR->R);

	kalman_adc = x_k_k1 + Kg * (Z_k - FIR->x_last);
	FIR->p_last = (1 - Kg)*P_k_k1;
	P_k_k1 = FIR->p_last;

	FIR->x_last = kalman_adc;

	return kalman_adc;
}




///*-------------------------------------------------------------------------------------------------------------*/
///*        
//        Q:����������Q���󣬶�̬��Ӧ��죬�����ȶ��Ա仵
//        R:����������R���󣬶�̬��Ӧ�����������ȶ��Ա��        
//*/

//float KalmanFilter(FIR_TypeDef* FIR,float ResrcData)
//{
//        float x_mid = FIR->x_last;

//        float p_mid;
//        float p_now;
//        float kg;        

//        x_mid = FIR->x_last; //x_last=x(k-1|k-1),x_mid=x(k|k-1)
//        p_mid = FIR->p_last + FIR->Q; //p_mid=p(k|k-1),p_last=p(k-1|k-1),Q=����
//        kg = p_mid / ( p_mid + FIR->R); //kgΪkalman filter��RΪ����
//        FIR->x_now = x_mid +kg * (ResrcData - x_mid);//���Ƴ�������ֵ
//                
//        p_now=(1-kg)*p_mid;//����ֵ��Ӧ��covariance        

//        FIR->p_last = p_now; //����covarianceֵ
//        FIR->x_last = FIR->x_now; //����ϵͳ״ֵ̬

//        return FIR->x_now;                
//}

