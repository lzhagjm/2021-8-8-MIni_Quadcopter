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

	while( code1 || code2)
	{
		code1 = MPU_Init();
		code2 = mpu_dmp_init();
	}

	HOLLOW_CUP_Init(10000,1);  //8.4k

	
	
	
	
	
/****************ϵͳĩβ��ʼ��******************/
	core_run_init(CYCLE);

	//��ʼ����ɣ�����0
	return (0);
}






