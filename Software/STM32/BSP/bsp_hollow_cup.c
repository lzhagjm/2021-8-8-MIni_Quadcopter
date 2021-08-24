#include "bsp_hollow_cup.h"

void HOLLOW_CUP_GPIO_Config(void)
{
	/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
	GPIO_InitTypeDef GPIO_InitStructure;
	/*������ص�GPIO����ʱ��*/
	RCC_AHB1PeriphClockCmd (CupGroup1_TIM_CH1_CLK, ENABLE);
	RCC_AHB1PeriphClockCmd (CupGroup1_TIM_CH2_CLK, ENABLE);
	RCC_AHB1PeriphClockCmd (CupGroup1_TIM_CH3_CLK, ENABLE);
	RCC_AHB1PeriphClockCmd (CupGroup1_TIM_CH4_CLK, ENABLE);
	RCC_AHB1PeriphClockCmd (CupGroup2_TIM_CH1_CLK, ENABLE);
	RCC_AHB1PeriphClockCmd (CupGroup2_TIM_CH2_CLK, ENABLE);
	RCC_AHB1PeriphClockCmd (CupGroup2_TIM_CH3_CLK, ENABLE);
	RCC_AHB1PeriphClockCmd (CupGroup2_TIM_CH4_CLK, ENABLE);	
  /* ��ʱ��ͨ�����Ÿ��� */
	GPIO_PinAFConfig(CupGroup1_TIM_CH1_PORT,CupGroup1_TIM_CH1_PINSOURCE,CupGroup1_TIM_AF); 
  GPIO_PinAFConfig(CupGroup1_TIM_CH2_PORT,CupGroup1_TIM_CH2_PINSOURCE,CupGroup1_TIM_AF);
	GPIO_PinAFConfig(CupGroup1_TIM_CH3_PORT,CupGroup1_TIM_CH3_PINSOURCE,CupGroup1_TIM_AF);
	GPIO_PinAFConfig(CupGroup1_TIM_CH4_PORT,CupGroup1_TIM_CH4_PINSOURCE,CupGroup1_TIM_AF);
	GPIO_PinAFConfig(CupGroup2_TIM_CH1_PORT,CupGroup2_TIM_CH1_PINSOURCE,CupGroup2_TIM_AF);
	GPIO_PinAFConfig(CupGroup2_TIM_CH2_PORT,CupGroup2_TIM_CH2_PINSOURCE,CupGroup2_TIM_AF);
	GPIO_PinAFConfig(CupGroup2_TIM_CH3_PORT,CupGroup2_TIM_CH3_PINSOURCE,CupGroup2_TIM_AF);
	GPIO_PinAFConfig(CupGroup2_TIM_CH4_PORT,CupGroup2_TIM_CH4_PINSOURCE,CupGroup2_TIM_AF);
	/* ��ʱ��ͨ���������� */															   		
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;    
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 
	
	GPIO_InitStructure.GPIO_Pin = CupGroup1_TIM_CH1_PIN;
	GPIO_Init(CupGroup1_TIM_CH1_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = CupGroup1_TIM_CH2_PIN;
	GPIO_Init(CupGroup1_TIM_CH2_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = CupGroup1_TIM_CH3_PIN;
	GPIO_Init(CupGroup1_TIM_CH3_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = CupGroup1_TIM_CH4_PIN;
	GPIO_Init(CupGroup1_TIM_CH4_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = CupGroup2_TIM_CH1_PIN;
	GPIO_Init(CupGroup2_TIM_CH1_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = CupGroup2_TIM_CH2_PIN;
	GPIO_Init(CupGroup2_TIM_CH2_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = CupGroup2_TIM_CH3_PIN;
	GPIO_Init(CupGroup2_TIM_CH3_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = CupGroup2_TIM_CH4_PIN;
	GPIO_Init(CupGroup2_TIM_CH4_PORT, &GPIO_InitStructure);
}

void HOLLOW_CUP_TIM_Mode_Config(u32 arr, int psc)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef  				TIM_OCInitStructure;
	/***********************��1��ʱ����ʼ��***************************/
	
	// ����TIMx_CLK,x[2,3,4,5,12,13,14] 
  RCC_APB1PeriphClockCmd(CupGroup1_TIM_CLK, ENABLE); 
  /* �ۼ� TIM_Period�������һ�����»����ж�*/		
  //����ʱ����0������8399����Ϊ8400�Σ�Ϊһ����ʱ����
  TIM_TimeBaseStructure.TIM_Period = arr-1;       	
	// ͨ�ÿ��ƶ�ʱ��ʱ��ԴTIMxCLK = HCLK/2=84MHz 
	// �趨��ʱ��Ƶ��Ϊ=TIMxCLK/(TIM_Prescaler+1)=100KHz
  TIM_TimeBaseStructure.TIM_Prescaler = psc-1;	
  // ����ʱ�ӷ�Ƶ
  TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
  // ������ʽ
  TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
	// ��ʼ����ʱ��TIMx, x[2,3,4,5,12,13,14] 
	TIM_TimeBaseInit(CupGroup1_TIM, &TIM_TimeBaseStructure);
	/*PWMģʽ����*/
	/* PWM1 Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    //����ΪPWMģʽ1
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	
  TIM_OCInitStructure.TIM_Pulse = 0;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  	  //����ʱ������ֵС��CCR1_ValʱΪ�ߵ�ƽ
  TIM_OC1Init(CupGroup1_TIM, &TIM_OCInitStructure);	 //ʹ��ͨ��1 
	TIM_OC2Init(CupGroup1_TIM, &TIM_OCInitStructure);	 //ʹ��ͨ��1
	TIM_OC3Init(CupGroup1_TIM, &TIM_OCInitStructure);	 //ʹ��ͨ��1
	TIM_OC4Init(CupGroup1_TIM, &TIM_OCInitStructure);	 //ʹ��ͨ��1
	/*ʹ��ͨ��1����*/
	TIM_OC1PreloadConfig(CupGroup1_TIM, TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(CupGroup1_TIM, TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(CupGroup1_TIM, TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(CupGroup1_TIM, TIM_OCPreload_Enable);	
	// ʹ�ܶ�ʱ��
	TIM_Cmd(CupGroup1_TIM, ENABLE);
	
	
	/***********************��2��ʱ����ʼ��***************************/
	
	// ����TIMx_CLK,x[2,3,4,5,12,13,14] 
  RCC_APB1PeriphClockCmd(CupGroup2_TIM_CLK, ENABLE); 
  /* �ۼ� TIM_Period�������һ�����»����ж�*/		
  //����ʱ����0������8399����Ϊ8400�Σ�Ϊһ����ʱ����
  TIM_TimeBaseStructure.TIM_Period = arr-1;       	
	// ͨ�ÿ��ƶ�ʱ��ʱ��ԴTIMxCLK = HCLK/2=84MHz 
	// �趨��ʱ��Ƶ��Ϊ=TIMxCLK/(TIM_Prescaler+1)=100KHz
  TIM_TimeBaseStructure.TIM_Prescaler = psc-1;	
  // ����ʱ�ӷ�Ƶ
  TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
  // ������ʽ
  TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;	
	// ��ʼ����ʱ��TIMx, x[2,3,4,5,12,13,14] 
	TIM_TimeBaseInit(CupGroup2_TIM, &TIM_TimeBaseStructure);	
	/*PWMģʽ����*/
	/* PWM1 Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    //����ΪPWMģʽ1
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	
  TIM_OCInitStructure.TIM_Pulse = 0;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  	  //����ʱ������ֵС��CCR1_ValʱΪ�ߵ�ƽ
  TIM_OC1Init(CupGroup2_TIM, &TIM_OCInitStructure);	 //ʹ��ͨ��1  
	TIM_OC2Init(CupGroup2_TIM, &TIM_OCInitStructure);	 //ʹ��ͨ��1
	TIM_OC3Init(CupGroup2_TIM, &TIM_OCInitStructure);	 //ʹ��ͨ��1
	TIM_OC4Init(CupGroup2_TIM, &TIM_OCInitStructure);	 //ʹ��ͨ��1
	/*ʹ��ͨ��1����*/
	TIM_OC1PreloadConfig(CupGroup2_TIM, TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(CupGroup2_TIM, TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(CupGroup2_TIM, TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(CupGroup2_TIM, TIM_OCPreload_Enable);	
	// ʹ�ܶ�ʱ��
	TIM_Cmd(CupGroup2_TIM, ENABLE);
}

void HOLLOW_CUP_Init(u32 arr, int psc)
{
	HOLLOW_CUP_GPIO_Config();
	HOLLOW_CUP_TIM_Mode_Config(arr, psc);
}

void SetSpeedHollowCup1(int speed)
{
	if(speed>0)
	{
		TIM_SetCompare1(CupGroup1_TIM,speed);
		TIM_SetCompare2(CupGroup1_TIM,0);
	}
	else
	{
		TIM_SetCompare1(CupGroup1_TIM,0);
		TIM_SetCompare2(CupGroup1_TIM,-speed);
	}
}
void SetSpeedHollowCup2(int speed)
{
	if(speed>0)
	{
		TIM_SetCompare3(CupGroup1_TIM,speed);
		TIM_SetCompare4(CupGroup1_TIM,0);
	}
	else
	{
		TIM_SetCompare3(CupGroup1_TIM,0);
		TIM_SetCompare4(CupGroup1_TIM,-speed);
	}
}
void SetSpeedHollowCup3(int speed)
{
	if(speed>0)
	{
		TIM_SetCompare1(CupGroup2_TIM,speed);
		TIM_SetCompare2(CupGroup2_TIM,0);
	}
	else
	{
		TIM_SetCompare1(CupGroup2_TIM,0);
		TIM_SetCompare2(CupGroup2_TIM,-speed);
	}
}
void SetSpeedHollowCup4(int speed)
{
	if(speed>0)
	{
		TIM_SetCompare3(CupGroup2_TIM,speed);
		TIM_SetCompare4(CupGroup2_TIM,0);
	}
	else
	{
		TIM_SetCompare3(CupGroup2_TIM,0);
		TIM_SetCompare4(CupGroup2_TIM,-speed);
	}
}
