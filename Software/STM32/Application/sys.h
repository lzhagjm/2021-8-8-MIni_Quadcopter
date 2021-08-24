#ifndef _SYS_H_
#define _SYS_H_

#include "stm32f10x.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/***************�ж����ȼ�******************/
																				
#define NVIC_GROUP NVIC_PriorityGroup_3 //�жϷ���ѡ��  NVIC_PriorityGroup_3  | 0-7  |   0-1  |

#define NVIC_TIM5_P 0 //TIM5�ж�����   �������
#define NVIC_TIM5_S 0

//#define NVIC_systick_P 0  //�δ�ʱ��
//#define NVIC_systick_S 0

#define NVIC_UART6_P  6    //k210����
#define NVIC_UART6_S  0

#define NVIC_TIM7_P 0 //TIM7�ж�����   ���Ķ�ʱ��
#define NVIC_TIM7_S 0

#define NVIC_UART3_P 1 //����3�ж�����   //��λ��
#define NVIC_UART3_S 0

#define NVIC_EXTI2_P 6 //EXTI2�ж�     //������
#define NVIC_EXTI2_S 0

#define NVIC_TIM4_P 6 //TIM4�ж�����   ������2
#define NVIC_TIM4_S 0

#define NVIC_TIM2_P 6 //TIM2�ж�����   ������1
#define NVIC_TIM2_S 1


//u8 CYCLE = 5;

//������������
#define CYCLE   10  //MS   max = 65.536ms   min = 0.001ms

/*           ϵͳ����               */
#include "Scheduler.h" 
#include "Task.h"
#include "core_run.h"

/*           BSP������               */
#include "bsp_led.h"
#include "bsp_s_i2c.h"
#include "bsp_h_i2c.h"
#include "bsp_systick.h"
#include "bsp_usart.h"
#include "bsp_mpu6050.h"
#include "bsp_adc.h"
#include "bsp_bluetooth.h"
#include "bsp_uc_debug.h"	
#include "bsp_hollow_cup.h"


/*              �㷨��               */
#include "math_lib.h"
#include "FIR_lib.h"
#include "pid_lib.h"



/*           mpu_6050 Lib��������               */
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"



/*****************����********************/




/* ֱ�Ӳ����Ĵ����ķ�������IO */
#define	digitalHi(p,i)			 {p->BSRRL=i;}		//����Ϊ�ߵ�ƽ
#define digitalLo(p,i)			 {p->BSRRH=i;}		//����͵�ƽ
#define digitalToggle(p,i)	 {p->ODR ^=i;}		//�����ת״̬


//λ������,ʵ��51���Ƶ�GPIO���ƹ���
//����ʵ��˼��,�ο�<<CM3Ȩ��ָ��>>������(87ҳ~92ҳ).M4ͬM3����,ֻ�ǼĴ�����ַ����.
//IO�ڲ����궨��
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
//IO�ڵ�ַӳ��
#define GPIOA_ODR_Addr    (GPIOA_BASE+20) //0x40020014
#define GPIOB_ODR_Addr    (GPIOB_BASE+20) //0x40020414 
#define GPIOC_ODR_Addr    (GPIOC_BASE+20) //0x40020814 
#define GPIOD_ODR_Addr    (GPIOD_BASE+20) //0x40020C14 
#define GPIOE_ODR_Addr    (GPIOE_BASE+20) //0x40021014 
#define GPIOF_ODR_Addr    (GPIOF_BASE+20) //0x40021414    
#define GPIOG_ODR_Addr    (GPIOG_BASE+20) //0x40021814   
#define GPIOH_ODR_Addr    (GPIOH_BASE+20) //0x40021C14    
#define GPIOI_ODR_Addr    (GPIOI_BASE+20) //0x40022014     

#define GPIOA_IDR_Addr    (GPIOA_BASE+16) //0x40020010 
#define GPIOB_IDR_Addr    (GPIOB_BASE+16) //0x40020410 
#define GPIOC_IDR_Addr    (GPIOC_BASE+16) //0x40020810 
#define GPIOD_IDR_Addr    (GPIOD_BASE+16) //0x40020C10 
#define GPIOE_IDR_Addr    (GPIOE_BASE+16) //0x40021010 
#define GPIOF_IDR_Addr    (GPIOF_BASE+16) //0x40021410 
#define GPIOG_IDR_Addr    (GPIOG_BASE+16) //0x40021810 
#define GPIOH_IDR_Addr    (GPIOH_BASE+16) //0x40021C10 
#define GPIOI_IDR_Addr    (GPIOI_BASE+16) //0x40022010 
 
//IO�ڲ���,ֻ�Ե�һ��IO��!
//ȷ��n��ֵС��16!
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //��� 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //���� 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //��� 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //���� 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //��� 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //���� 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //��� 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //���� 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //��� 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //����

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //��� 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //����

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //��� 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //����

#define PHout(n)   BIT_ADDR(GPIOH_ODR_Addr,n)  //��� 
#define PHin(n)    BIT_ADDR(GPIOH_IDR_Addr,n)  //����

#define PIout(n)   BIT_ADDR(GPIOI_ODR_Addr,n)  //��� 
#define PIin(n)    BIT_ADDR(GPIOI_IDR_Addr,n)  //����




#define TICK_PER_SECOND	1000
#define TICK_US	(1000000/TICK_PER_SECOND)


u8 All_Init(void);

#endif







