#ifndef _PID_LIB_H_
#define _PID_LIB_H_
#include "sys.h"

typedef struct{
	float Kp;           //��ʼP
	float Ki;						//��ʼP
	float Kd;							//��ʼP
	float err;						//��ʼ���
	float last_err;      //��ʼ��һ�����
	float last_last_err;  //��ʼ����һ�����
	float observed_val;    //ʵ��ֵ
	float target_val;    //Ŀ��ֵ
	float output_val;    //���ֵ
	float integral;      //�ۼ����
	float Integral_limit_up; //�������޷�
	float Integral_limit_down; //�������޷�
	float output_limit_up; 		//������޷�
	float output_limit_down; 		//������޷�
}PID_TypeDef;


void PID_set_P(PID_TypeDef* PID , float Kp);
void PID_set_I(PID_TypeDef* PID , float Ki);
void PID_set_D(PID_TypeDef* PID , float Kd);

void Set_target_val(PID_TypeDef* PID,float target_val);

/*            ��׼λ��ʽPD         */
float Compute_Position_PD(PID_TypeDef* PID);
/*            ��׼λ��ʽPID         */
float Compute_Position_PID(PID_TypeDef* PID);
/*            ��׼����ʽPID         */
float Compute_Incremental_PID(PID_TypeDef* PID);

#endif
