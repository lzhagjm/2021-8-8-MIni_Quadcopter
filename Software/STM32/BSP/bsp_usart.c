#include "bsp_usart.h"

void NoUse(u8 data){}
//���ڽ��ս�������
#define U1GetOneByte	NoUse
#define U2GetOneByte	NoUse
#define U3GetOneByte	data_analysis
#define U6GetOneByte	k210_data_analysis
	
#if USE_USART1
//====uart1
void usart1_init(u32 br_num)
{
    USART_InitTypeDef USART_InitStructure;
    USART_ClockInitTypeDef USART_ClockInitStruct;
    NVIC_InitTypeDef NVIC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    USART_StructInit(&USART_InitStructure);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); //����USART1ʱ��
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    //�����ж����ȼ�
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = NVIC_UART1_P;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = NVIC_UART1_S;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);

    //����PA9��ΪUSART1��Tx
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    //����PA10��ΪUSART1��Rx
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    USART_DeInit(USART1);
    //����USART1
    //�жϱ�������
    USART_InitStructure.USART_BaudRate = br_num;                                    //�����ʿ���ͨ������վ����
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;                     //8λ����
    USART_InitStructure.USART_StopBits = USART_StopBits_1;                          //��֡��β����1��ֹͣλ
    USART_InitStructure.USART_Parity = USART_Parity_No;                             //������żУ��
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //Ӳ��������ʧ��
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;                 //���͡�����ʹ��
    //����USART1ʱ��
   USART_ClockInitStruct.USART_Clock = USART_Clock_Disable;     //ʱ�ӵ͵�ƽ�
   USART_ClockInitStruct.USART_CPOL = USART_CPOL_Low;           //SLCK������ʱ������ļ���->�͵�ƽ
   USART_ClockInitStruct.USART_CPHA = USART_CPHA_2Edge;         //ʱ�ӵڶ������ؽ������ݲ���
   USART_ClockInitStruct.USART_LastBit = USART_LastBit_Disable; //���һλ���ݵ�ʱ�����岻��SCLK���

    USART_Init(USART1, &USART_InitStructure);
    USART_ClockInit(USART1, &USART_ClockInitStruct);
		USART_Cmd(USART1, ENABLE);
    //ʹ��USART1�����ж�
		USART_ClearFlag(USART1, USART_FLAG_TC);
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    //ʹ��USART1
}


//���ͺ���
u8 Tx1Buffer[256];
u8 Tx1Counter = 0;
u8 count1 = 0;
void DrvUart1SendBuf(unsigned char *DataToSend, u8 data_num)
{
    u8 i;
    for (i = 0; i < data_num; i++)
    {
        Tx1Buffer[count1++] = *(DataToSend + i);
    }

    if (!(USART1->CR1 & USART_CR1_TXEIE))
    {
        USART_ITConfig(USART1, USART_IT_TXE, ENABLE); //�򿪷����ж�
    }
}

//���ܴ���
u8 U1RxDataTmp[100];
u8 U1RxInCnt = 0;
u8 U1RxoutCnt = 0;
void drvU1GetByte(u8 data)
{
	U1RxDataTmp[U1RxInCnt++] = data;
	if(U1RxInCnt >= 100)
		U1RxInCnt = 0;
}
void drvU1DataCheck(void)
{
	while(U1RxInCnt!=U1RxoutCnt)
	{
		U1GetOneByte(U1RxDataTmp[U1RxoutCnt++]);
		if(U1RxoutCnt >= 100)
			U1RxoutCnt = 0;
	}
}

void USART1_IRQHandler(void)
{
    u8 com_data;

    if (USART1->SR & USART_SR_ORE) //ORE�ж�
    {
        com_data = USART1->DR;
    }
    //�����ж�
    if (USART_GetITStatus(USART1, USART_IT_RXNE))
    {
        USART_ClearITPendingBit(USART1, USART_IT_RXNE); //����жϱ�־
        com_data = USART1->DR;
				USART_SendData(USART3, com_data);
				while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET){};
        drvU1GetByte(com_data);
    }
    //���ͣ�������λ���ж�
    if (USART_GetITStatus(USART1, USART_IT_TXE))
    {
        USART1->DR = Tx1Buffer[Tx1Counter++]; //дDR����жϱ�־
        if (Tx1Counter == count1)
        {
            USART1->CR1 &= ~USART_CR1_TXEIE; //�ر�TXE�������жϣ��ж�
        }
    }
}
#endif


#if USE_USART6
//===usart6
void usart6_init(u32 br_num)
{
    USART_InitTypeDef USART_InitStructure;
    USART_ClockInitTypeDef USART_ClockInitStruct;
    NVIC_InitTypeDef NVIC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    USART_StructInit(&USART_InitStructure);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE); //����USART1ʱ��
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

    //�����ж����ȼ�
    NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = NVIC_UART6_P;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = NVIC_UART6_S;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_USART6);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_USART6);

    //����PA9��ΪUSART6��Tx
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    //����PA10��ΪUSART6��Rx
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    USART_DeInit(USART6);
    //����USART1
    //�жϱ�������
    USART_InitStructure.USART_BaudRate = br_num;                                    //�����ʿ���ͨ������վ����
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;                     //8λ����
    USART_InitStructure.USART_StopBits = USART_StopBits_1;                          //��֡��β����1��ֹͣλ
    USART_InitStructure.USART_Parity = USART_Parity_No;                             //������żУ��
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //Ӳ��������ʧ��
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;                 //���͡�����ʹ��
    //����USART1ʱ��
   USART_ClockInitStruct.USART_Clock = USART_Clock_Disable;     //ʱ�ӵ͵�ƽ�
   USART_ClockInitStruct.USART_CPOL = USART_CPOL_Low;           //SLCK������ʱ������ļ���->�͵�ƽ
   USART_ClockInitStruct.USART_CPHA = USART_CPHA_2Edge;         //ʱ�ӵڶ������ؽ������ݲ���
   USART_ClockInitStruct.USART_LastBit = USART_LastBit_Disable; //���һλ���ݵ�ʱ�����岻��SCLK���

    USART_Init(USART6, &USART_InitStructure);
    USART_ClockInit(USART6, &USART_ClockInitStruct);
		USART_Cmd(USART6, ENABLE);
    //ʹ��USART1�����ж�
		USART_ClearFlag(USART6, USART_FLAG_TC);
    USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);
    //ʹ��USART1
}


//���ͺ���
u8 Tx6Buffer[256];
u8 Tx6Counter = 0;
u8 count6 = 0;
void DrvUart6SendBuf(unsigned char *DataToSend, u8 data_num)
{
    u8 i;
    for (i = 0; i < data_num; i++)
    {
        Tx6Buffer[count6++] = *(DataToSend + i);
    }

    if (!(USART6->CR1 & USART_CR1_TXEIE))
    {
        USART_ITConfig(USART6, USART_IT_TXE, ENABLE); //�򿪷����ж�
    }
}

//���ܴ���
u8 U6RxDataTmp[100];
u8 U6RxInCnt = 0;
u8 U6RxoutCnt = 0;
void drvU6GetByte(u8 data)
{
	U6RxDataTmp[U6RxInCnt++] = data;
	if(U6RxInCnt >= 100)
		U6RxInCnt = 0;
}
void drvU6DataCheck(void)
{
	while(U6RxInCnt!=U6RxoutCnt)
	{
		U6GetOneByte(U6RxDataTmp[U6RxoutCnt++]);
		if(U6RxoutCnt >= 100)
			U6RxoutCnt = 0;
	}
}

void USART6_IRQHandler(void)
{
    u8 com_data;

    if (USART6->SR & USART_SR_ORE) //ORE�ж�
    {
        com_data = USART6->DR;
    }
    //�����ж�
    if (USART_GetITStatus(USART6, USART_IT_RXNE))
    {
        USART_ClearITPendingBit(USART6, USART_IT_RXNE); //����жϱ�־
        com_data = USART6->DR;
        drvU6GetByte(com_data);
    }
    //���ͣ�������λ���ж�
    if (USART_GetITStatus(USART6, USART_IT_TXE))
    {
        USART6->DR = Tx6Buffer[Tx6Counter++]; //дDR����жϱ�־
        if (Tx6Counter == count6)
        {
            USART6->CR1 &= ~USART_CR1_TXEIE; //�ر�TXE�������жϣ��ж�
        }
    }
}

#endif



#if USE_USART2
//====uart2
void usart2_init(u32 br_num)
{
    USART_InitTypeDef USART_InitStructure;
    USART_ClockInitTypeDef USART_ClockInitStruct;
    NVIC_InitTypeDef NVIC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    USART_StructInit(&USART_InitStructure);

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); //����USART2ʱ��
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

    //�����ж����ȼ�
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = NVIC_UART2_P;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = NVIC_UART2_S;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_USART2);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_USART2);

    //����PD5��ΪUSART2��Tx
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    //����PD6��ΪUSART2��Rx
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    //����USART2
    //�жϱ�������
    USART_InitStructure.USART_BaudRate = br_num;                                    //�����ʿ���ͨ������վ����
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;                     //8λ����
    USART_InitStructure.USART_StopBits = USART_StopBits_1;                          //��֡��β����1��ֹͣλ
    USART_InitStructure.USART_Parity = USART_Parity_No;                             //������żУ��
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //Ӳ��������ʧ��
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;                 //���͡�����ʹ��
    //����USART2ʱ��
    USART_ClockInitStruct.USART_Clock = USART_Clock_Disable;     //ʱ�ӵ͵�ƽ�
    USART_ClockInitStruct.USART_CPOL = USART_CPOL_Low;           //SLCK������ʱ������ļ���->�͵�ƽ
    USART_ClockInitStruct.USART_CPHA = USART_CPHA_2Edge;         //ʱ�ӵڶ������ؽ������ݲ���
    USART_ClockInitStruct.USART_LastBit = USART_LastBit_Disable; //���һλ���ݵ�ʱ�����岻��SCLK���

    USART_Init(USART2, &USART_InitStructure);
    USART_ClockInit(USART2, &USART_ClockInitStruct);

    //ʹ��USART2�����ж�
		USART_ClearFlag(USART2, USART_FLAG_TC);
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
    //ʹ��USART2
    USART_Cmd(USART2, ENABLE);
}

u8 TxBuffer[256];
u8 TxCounter = 0;
u8 count = 0;
void DrvUart2SendBuf(unsigned char *DataToSend, u8 data_num)
{
    u8 i;
    for (i = 0; i < data_num; i++)
    {
        TxBuffer[count++] = *(DataToSend + i);
    }

    if (!(USART2->CR1 & USART_CR1_TXEIE))
    {
        USART_ITConfig(USART2, USART_IT_TXE, ENABLE); //�򿪷����ж�
    }
}
u8 U2RxDataTmp[100];
u8 U2RxInCnt = 0;
u8 U2RxoutCnt = 0;
void drvU2GetByte(u8 data)
{
	U2RxDataTmp[U2RxInCnt++] = data;
	if(U2RxInCnt >= 100)
		U2RxInCnt = 0;
}
void drvU2DataCheck(void)
{
	while(U2RxInCnt!=U2RxoutCnt)
	{
		U2GetOneByte(U2RxDataTmp[U2RxoutCnt++]);
		if(U2RxoutCnt >= 100)
			U2RxoutCnt = 0;
	}
}
void USART2_IRQHandler(void)
{
    u8 com_data;

    if (USART2->SR & USART_SR_ORE) //ORE�ж�
    {
        com_data = USART2->DR;
    }

    //�����ж�
    if (USART_GetITStatus(USART2, USART_IT_RXNE))
    {
        USART_ClearITPendingBit(USART2, USART_IT_RXNE); //����жϱ�־
        com_data = USART2->DR;
				drvU2GetByte(com_data);
    }
    //���ͣ�������λ���ж�
    if (USART_GetITStatus(USART2, USART_IT_TXE))
    {
        USART2->DR = TxBuffer[TxCounter++]; //дDR����жϱ�־
        if (TxCounter == count)
        {
            USART2->CR1 &= ~USART_CR1_TXEIE; //�ر�TXE�������жϣ��ж�
        }
    }
}
#endif

#if USE_USART3
//====uart3
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void usart3_init(u32 br_num)
{
    USART_InitTypeDef USART_InitStructure;
    USART_ClockInitTypeDef USART_ClockInitStruct;
    NVIC_InitTypeDef NVIC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    USART_StructInit(&USART_InitStructure);

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE); //����USART2ʱ��
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

    //�����ж����ȼ�
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = NVIC_UART3_P;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = NVIC_UART3_S;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);

    //����PD5��ΪUSART2��Tx
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    //����PD6��ΪUSART2��Rx
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    //����USART2
    //�жϱ�������
    USART_InitStructure.USART_BaudRate = br_num;                                    //�����ʿ���ͨ������վ����
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;                     //8λ����
    USART_InitStructure.USART_StopBits = USART_StopBits_1;                          //��֡��β����1��ֹͣλ
    USART_InitStructure.USART_Parity = USART_Parity_No;                             //������żУ��
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //Ӳ��������ʧ��
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;                 //���͡�����ʹ��
    //����USART2ʱ��
    USART_ClockInitStruct.USART_Clock = USART_Clock_Disable;     //ʱ�ӵ͵�ƽ�
    USART_ClockInitStruct.USART_CPOL = USART_CPOL_Low;           //SLCK������ʱ������ļ���->�͵�ƽ
    USART_ClockInitStruct.USART_CPHA = USART_CPHA_2Edge;         //ʱ�ӵڶ������ؽ������ݲ���
    USART_ClockInitStruct.USART_LastBit = USART_LastBit_Disable; //���һλ���ݵ�ʱ�����岻��SCLK���

    USART_Init(USART3, &USART_InitStructure);
    USART_ClockInit(USART3, &USART_ClockInitStruct);

    //ʹ��USART2�����ж�
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
    //ʹ��USART2
    USART_Cmd(USART3, ENABLE);
}

u8 Tx3Buffer[256];
u8 Tx3Counter = 0;
u8 count3 = 0;
void DrvUart3SendBuf(unsigned char *DataToSend, u8 data_num)
{
    u8 i;
    for (i = 0; i < data_num; i++)
    {
        Tx3Buffer[count3++] = *(DataToSend + i);
    }
    if (!(USART3->CR1 & USART_CR1_TXEIE))
    {
        USART_ITConfig(USART3, USART_IT_TXE, ENABLE); //�򿪷����ж�
    }
}
u8 U3RxDataTmp[100];
u8 U3RxInCnt = 0;
u8 U3RxoutCnt = 0;
void drvU3GetByte(u8 data)
{
	U3RxDataTmp[U3RxInCnt++] = data;
	if(U3RxInCnt >= 100)
		U3RxInCnt = 0;
}
void drvU3DataCheck(void)
{
	while(U3RxInCnt!=U3RxoutCnt)
	{
		U3GetOneByte(U3RxDataTmp[U3RxoutCnt++]);
		if(U3RxoutCnt >= 100)
			U3RxoutCnt = 0;
	}
}
void USART3_IRQHandler(void)
{
    u8 com_data;

    if (USART3->SR & USART_SR_ORE) //ORE�ж�
        com_data = USART3->DR;

    //�����ж�
    if (USART_GetITStatus(USART3, USART_IT_RXNE))
    {
        USART_ClearITPendingBit(USART3, USART_IT_RXNE); //����жϱ�־
        com_data = USART3->DR;
		drvU3GetByte(com_data);
    }
    //���ͣ�������λ���ж�
    if (USART_GetITStatus(USART3, USART_IT_TXE))
    {
        USART3->DR = Tx3Buffer[Tx3Counter++]; //дDR����жϱ�־
        if (Tx3Counter == count3)
        {
            USART3->CR1 &= ~USART_CR1_TXEIE; //�ر�TXE�������жϣ��ж�
        }
    }
}
#endif

//�ض���c�⺯��printf�����ڣ��ض�����ʹ��printf����
int fputc(int ch,FILE *f)
{
	//����һ���ֽ����ݵ�����
	USART_SendData(USART3,(uint8_t) ch);
	//�ȴ��������
	while(USART_GetFlagStatus(USART3,USART_FLAG_TXE) == RESET);
	return (ch);
}

//�ض���c�⺯��scanf�����ڣ��ض�����ʹ��scanf,getchar�Ⱥ���
int fgetc(FILE *f)
{
	//�ȴ�������������
	while(USART_GetFlagStatus(USART3,USART_FLAG_RXNE) == RESET);
	return (int)USART_ReceiveData(USART3);
}


void DrvUartDataCheck(void)
{
	//drvU1DataCheck();
	//drvU2DataCheck();
	drvU3DataCheck();
}
