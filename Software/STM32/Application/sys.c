#include "sys.h"






u8 All_Init(void)
{
	u8 code1 = 1, code2 = 1;
	/****************ϵͳ����******************/
	//�ж����ȼ��������
	NVIC_PriorityGroupConfig(NVIC_GROUP);		
	//�δ�ʱ��
	SysTick_Init(); 	
	//��ʱ
	Delay_ms(100);
	led_init();
	adc1_init();


	
	
/****************����ѡ��******************/

	BlueTooth_Init();
	//usart1_init(500000);
	while( code1 || code2)
	{
		code1 = MPU_Init();
		code2 = mpu_dmp_init();
		
		code1 ? (LED1 = 0) : (LED1 = 1);
		code2 ? (LED2 = 0) : (LED2 = 1);
	}
	//hmc5883l_init();
	LED1 = 1;
	LED2 = 1;
	TIMx_Config();

	
	
	
	
	
/****************ϵͳĩβ��ʼ��******************/
	core_run_init(CYCLE);

	//��ʼ����ɣ�����0
	return (0);
}






