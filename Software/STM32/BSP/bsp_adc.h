#ifndef _BSP_ADC_H_
#define _BSP_ADC_H_	
#include "sys.h" 

#define RHEOSTAT_NOFCHANEL      2
 							   
void adc1_init(void); 				//ADCͨ����ʼ��
//u16  Get_Adc(u8 ch); 				//���ĳ��ͨ��ֵ 
//u16 Get_Adc_Average(u8 ch,u8 times);//�õ�ĳ��ͨ����������������ƽ��ֵ  
float Get_temperature(void);

#endif 
