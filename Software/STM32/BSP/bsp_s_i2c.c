#include "bsp_s_i2c.h"
/*******************ģ��Ӳ������******************/
//��ʱ
static void S_I2C_Delay(void)
{
	Delay_us(2);
}

//ֹͣ�ź�
static void S_I2C_Stop(void)
{
	//��SCL�ߵ�ƽʱ��SDA���������ر�ʾI2C����ֹͣ�ź�
	SDA_0();
	SCL_1();
	S_I2C_Delay();
	SDA_1();
}
//����һ��Ack�ź�
static void S_I2C_Ack(void)
{
	SDA_0();
	S_I2C_Delay();
	SCL_1();
	S_I2C_Delay();
	SCL_0();
	S_I2C_Delay();
	SDA_1();
}
//����һ��NAck�ź�
static void S_I2C_NAck(void)
{
	SDA_1();
	S_I2C_Delay();
	SCL_1();
	S_I2C_Delay();
	SCL_0();
	S_I2C_Delay();
}


//�����ź�
static void S_I2C_Start(void)
{
	//��SCL�ߵ�ƽʱ,SDA����һ���½��ر�ʾI2C���������ź�
	SDA_1();
	SCL_1();
	S_I2C_Delay();
	SDA_0();
	S_I2C_Delay();
	SCL_0();
	S_I2C_Delay();
}
//��ȡ8λ����
static uint8_t S_I2C_ReadByte(uint8_t ack)
{
	uint8_t i,value;
	//������1��bitΪ���ݵ�bit7
	value = 0;
	for(i = 0;i < 8;i++)
	{
		value <<= 1;
		SCL_1();
		S_I2C_Delay();
		if(SDA_In())
		{
			value++;
		}
		SCL_0();
		S_I2C_Delay();
	}
	if(ack==0)
		S_I2C_NAck();
	else
		S_I2C_Ack();
	return value;
}

//��������
static void S_I2C_SendByte(uint8_t data)
{
	uint8_t i;
	for(i = 0 ; i < 8 ; i++)
	{
		if(data & 0x80)
		{
			SDA_1();
		}else
		{
			SDA_0();
		}
		S_I2C_Delay();
		SCL_1();
		S_I2C_Delay();
		SCL_0();
		if(i == 7)
		{
			SDA_1();//�ͷ�����
		}
		data <<= 1;
		S_I2C_Delay();
	}
}

//��ȡACKӦ���ź�
static uint8_t S_I2C_WaitAck(void)
{
	uint8_t re;
	SDA_1();
	S_I2C_Delay();
	SCL_1();
	S_I2C_Delay();
	if(SDA_In())
	{
		re = 1;
	}else
	{
		re = 0;
	}
	SCL_0();
	S_I2C_Delay();
	return re;
}

/*******************bsp�ӿں���******************/

//����GPIO
void S_I2C_Init(void)
{
	//����GPIO�ṹ��
	GPIO_InitTypeDef GPIO_InitSteruct;
	//��ʱ��
	RCC_AHB1PeriphClockCmd(S_I2C_SCL_RCC_PORT|S_I2C_SDA_RCC_PORT,ENABLE);
	//���ýṹ��
	GPIO_InitSteruct.GPIO_Mode = GPIO_Mode_OUT;  //��©���
	GPIO_InitSteruct.GPIO_Pin = S_I2C_SDA_Pin;
	GPIO_InitSteruct.GPIO_Speed = GPIO_Fast_Speed;
	GPIO_InitSteruct.GPIO_OType = GPIO_OType_OD;
	GPIO_InitSteruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	//���ýṹ��
	GPIO_Init(S_I2C_SDA_PORT,&GPIO_InitSteruct);
	GPIO_InitSteruct.GPIO_Pin = S_I2C_SCL_Pin;
	GPIO_Init(S_I2C_SCL_PORT,&GPIO_InitSteruct);
	//��һ��ֹͣ�źţ���λI2C�����ϵ������豸������ģʽ
	S_I2C_Stop();
}

u8 S_I2C_Write_Byte(u8 addr,u8 cmd,u8 data) 				 
{ 
  S_I2C_Start(); 
	S_I2C_SendByte((addr<<1)|0);//����������ַ+д����	
	if(S_I2C_WaitAck())	//�ȴ�Ӧ��
	{
		S_I2C_Stop();		 
		return 1;		
	}
    S_I2C_SendByte(cmd);	//д�Ĵ�����ַ
    S_I2C_WaitAck();		//�ȴ�Ӧ�� 
	S_I2C_SendByte(data);//��������
	if(S_I2C_WaitAck())	//�ȴ�ACK
	{
		S_I2C_Stop();	 
		return 1;		 
	}		 
    S_I2C_Stop();	 
	return 0;
}
//IIC��һ���ֽ� 
//reg:�Ĵ�����ַ 
//����ֵ:����������
u8 S_I2C_Read_Byte(u8 addr,u8 cmd)
{
	u8 res;
  S_I2C_Start(); 
	S_I2C_SendByte((addr<<1)|0);//����������ַ+д����	
	S_I2C_WaitAck();		//�ȴ�Ӧ�� 
  S_I2C_SendByte(cmd);	//д�Ĵ�����ַ
  S_I2C_WaitAck();		//�ȴ�Ӧ��
  S_I2C_Start();
	S_I2C_SendByte((addr<<1)|1);//����������ַ+������	
  S_I2C_WaitAck();		//�ȴ�Ӧ�� 
	res=S_I2C_ReadByte(0);//��ȡ����,����nACK 
  S_I2C_Stop();			//����һ��ֹͣ���� 
	return res;		
}


//���Ͷ���ֽ�����
S_IIC_DEBUG S_I2C_Write(uint8_t Addr,uint8_t cmd,uint8_t *data,uint8_t size)
{
	uint16_t i,m;
	for(i = 0 ; i < size ; i++)
	{
		if(i == 0)
		{
			S_I2C_Stop();
			for(m = 0 ; m < 1000 ; m++)
			{
				S_I2C_Start();
				S_I2C_SendByte((Addr<<1) | S_I2C_WR);
				if(S_I2C_WaitAck() == 0)
				{
					break;
				}
			}
			if(m == 1000)
			{
				S_I2C_Stop();
				return S_IIC_ERROR;
			}
			if(cmd)
			{
				S_I2C_SendByte((uint8_t)cmd);
				if(S_I2C_WaitAck() != 0)
				{
					S_I2C_Stop();
					return S_IIC_ERROR;
				}
			}
		}
		S_I2C_SendByte(data[i]);
		if(S_I2C_WaitAck() != 0)
		{
			S_I2C_Stop();
			return S_IIC_ERROR;
		}		
	}
	S_I2C_Stop();
	return S_IIC_OK;
}

//��ȡ�������
S_IIC_DEBUG S_I2C_Read(uint8_t Addr,uint8_t cmd,uint8_t *data,uint8_t size)
{
	uint16_t i;
	S_I2C_Start();//��ʼ�ź�
	S_I2C_SendByte((Addr<<1)|S_I2C_WR);//���Ϳ���ָ��
	//�ȴ�ACK
	if(S_I2C_WaitAck() != 0)
	{
		S_I2C_Stop();
		return S_IIC_ERROR;
	}
	if(cmd)
	{
			//�����ֽڵ�ַ
			S_I2C_SendByte((uint8_t)cmd);
			//�ȴ�ACK
			if(S_I2C_WaitAck() != 0)
			{
				S_I2C_Stop();
				return S_IIC_ERROR;
			}
	}
	S_I2C_Start();
	S_I2C_SendByte((Addr<<1)|S_I2C_RD);
	if(S_I2C_WaitAck() != 0)
	{
		S_I2C_Stop();
		return S_IIC_ERROR;
	}
	//��ȡ����
	for(i=0;i < (size - 1);i++){
		*data = S_I2C_ReadByte(1);
		data++;
	}
	*data=S_I2C_ReadByte(0);
	S_I2C_Stop();	
	return S_IIC_OK;
}

