#include "bsp_mpu6050.h"


int Sensors_I2C_WriteRegister(unsigned char slave_addr, unsigned char reg_addr,unsigned char length, unsigned char *data)
{
#if I2C_CHOOSE
	return H_I2C_Write(slave_addr,reg_addr,length,data);
#else
	return S_I2C_Write(slave_addr,reg_addr,data,length);
#endif
}

int Sensors_I2C_ReadRegister(unsigned char slave_addr, unsigned char reg_addr,unsigned char length, unsigned char *data)
{
#if I2C_CHOOSE
	return H_I2C_Read(slave_addr,reg_addr,length,data);
#else
	return S_I2C_Read(slave_addr,reg_addr,data,length);
#endif
}

void mget_ms(unsigned long *time)
{

}

//��ʼ��MPU6050
//����ֵ:0,�ɹ�
//    ����,�������
u8 MPU_Init(void)
{ 
	u8 res;
#if I2C_CHOOSE
	I2cMaster_Init();//��ʼ��IIC����
	H_I2C_Write_Byte(MPU_ADDR,MPU_PWR_MGMT1_REG,0X80);	//��λMPU6050
  Delay_ms(100);
	H_I2C_Write_Byte(MPU_ADDR,MPU_PWR_MGMT1_REG,0X00);	//����MPU6050 
	MPU_Set_Gyro_Fsr(3);					//�����Ǵ�����,��2000dps
	MPU_Set_Accel_Fsr(0);					//���ٶȴ�����,��2g
	MPU_Set_Rate(200);						//���ò�����50Hz
	H_I2C_Write_Byte(MPU_ADDR,MPU_INT_EN_REG,0X00);	//�ر������ж�
	H_I2C_Write_Byte(MPU_ADDR,MPU_USER_CTRL_REG,0X00);	//I2C��ģʽ�ر�
	H_I2C_Write_Byte(MPU_ADDR,MPU_FIFO_EN_REG,0X00);	//�ر�FIFO
	H_I2C_Write_Byte(MPU_ADDR,MPU_INTBP_CFG_REG,0X80);	//INT���ŵ͵�ƽ��Ч
	res=H_I2C_Read_Byte(MPU_ADDR,MPU_DEVICE_ID_REG);
	if(res==MPU_ADDR)//����ID��ȷ
	{
		H_I2C_Write_Byte(MPU_ADDR,MPU_PWR_MGMT1_REG,0X01);	//����CLKSEL,PLL X��Ϊ�ο�
		H_I2C_Write_Byte(MPU_ADDR,MPU_PWR_MGMT2_REG,0X00);	//���ٶ��������Ƕ�����
		MPU_Set_Rate(200);						//���ò�����Ϊ50Hz
 	}else return 1;
	return 0;
#else
	S_I2C_Init();//��ʼ��IIC����
	S_I2C_Write_Byte(MPU_ADDR,MPU_PWR_MGMT1_REG,0X80);	//��λMPU6050
  Delay_ms(100);
	S_I2C_Write_Byte(MPU_ADDR,MPU_PWR_MGMT1_REG,0X00);	//����MPU6050 
	MPU_Set_Gyro_Fsr(3);					//�����Ǵ�����,��2000dps
	MPU_Set_Accel_Fsr(0);					//���ٶȴ�����,��2g
	MPU_Set_Rate(200);						//���ò�����50Hz
	S_I2C_Write_Byte(MPU_ADDR,MPU_INT_EN_REG,0X00);	//�ر������ж�
	S_I2C_Write_Byte(MPU_ADDR,MPU_USER_CTRL_REG,0X00);	//I2C��ģʽ�ر�
	S_I2C_Write_Byte(MPU_ADDR,MPU_FIFO_EN_REG,0X00);	//�ر�FIFO
	S_I2C_Write_Byte(MPU_ADDR,MPU_INTBP_CFG_REG,0X80);	//INT���ŵ͵�ƽ��Ч
	res=S_I2C_Read_Byte(MPU_ADDR,MPU_DEVICE_ID_REG);
	if(res==MPU_ADDR)//����ID��ȷ
	{
		S_I2C_Write_Byte(MPU_ADDR,MPU_PWR_MGMT1_REG,0X01);	//����CLKSEL,PLL X��Ϊ�ο�
		S_I2C_Write_Byte(MPU_ADDR,MPU_PWR_MGMT2_REG,0X00);	//���ٶ��������Ƕ�����
		MPU_Set_Rate(200);						//���ò�����Ϊ50Hz
 	}else return 1;
	return 0;
#endif
	
}
//����MPU6050�����Ǵ����������̷�Χ
//fsr:0,��250dps;1,��500dps;2,��1000dps;3,��2000dps
//����ֵ:0,���óɹ�
//    ����,����ʧ�� 
u8 MPU_Set_Gyro_Fsr(u8 fsr)
{
#if I2C_CHOOSE
	return H_I2C_Write_Byte(MPU_ADDR,MPU_GYRO_CFG_REG,fsr<<3);//���������������̷�Χ  
#else
	return S_I2C_Write_Byte(MPU_ADDR,MPU_GYRO_CFG_REG,fsr<<3);//���������������̷�Χ  
#endif
	
}
//����MPU6050���ٶȴ����������̷�Χ
//fsr:0,��2g;1,��4g;2,��8g;3,��16g
//����ֵ:0,���óɹ�
//    ����,����ʧ�� 
u8 MPU_Set_Accel_Fsr(u8 fsr)
{
#if I2C_CHOOSE
	return H_I2C_Write_Byte(MPU_ADDR,MPU_ACCEL_CFG_REG,fsr<<3);//���ü��ٶȴ����������̷�Χ  
#else
	return S_I2C_Write_Byte(MPU_ADDR,MPU_ACCEL_CFG_REG,fsr<<3);//���ü��ٶȴ����������̷�Χ  
#endif
	
}
//����MPU6050�����ֵ�ͨ�˲���
//lpf:���ֵ�ͨ�˲�Ƶ��(Hz)
//����ֵ:0,���óɹ�
//    ����,����ʧ�� 
u8 MPU_Set_LPF(u16 lpf)
{
	u8 data=0;
	if(lpf>=188)data=1;
	else if(lpf>=98)data=2;
	else if(lpf>=42)data=3;
	else if(lpf>=20)data=4;
	else if(lpf>=10)data=5;
	else data=6; 
#if I2C_CHOOSE
	return H_I2C_Write_Byte(MPU_ADDR,MPU_CFG_REG,data);//�������ֵ�ͨ�˲���  
#else
	return S_I2C_Write_Byte(MPU_ADDR,MPU_CFG_REG,data);//�������ֵ�ͨ�˲���  
#endif
	
}
//����MPU6050�Ĳ�����(�ٶ�Fs=1KHz)
//rate:4~1000(Hz)
//����ֵ:0,���óɹ�
//    ����,����ʧ�� 
u8 MPU_Set_Rate(u16 rate)
{
	u8 data;
	if(rate>1000)rate=1000;
	if(rate<4)rate=4;
	data=1000/rate-1;
#if I2C_CHOOSE
	data=H_I2C_Write_Byte(MPU_ADDR,MPU_SAMPLE_RATE_REG,data);	//�������ֵ�ͨ�˲���
#else
	data=S_I2C_Write_Byte(MPU_ADDR,MPU_SAMPLE_RATE_REG,data);	//�������ֵ�ͨ�˲���
#endif
 	return MPU_Set_LPF(rate/2);	//�Զ�����LPFΪ�����ʵ�һ��
}

//�õ��¶�ֵ
//����ֵ:�¶�ֵ(������100��)
short MPU_Get_Temperature(void)
{
    u8 buf[2]; 
    short raw;
	float temp;
	Sensors_I2C_ReadRegister(MPU_ADDR,MPU_TEMP_OUTH_REG,2,buf); 
    raw=((u16)buf[0]<<8)|buf[1];  
    temp=36.53+((double)raw)/340;  
    return temp*100;;
}
//�õ�������ֵ(ԭʼֵ)
//gx,gy,gz:������x,y,z���ԭʼ����(������)
//����ֵ:0,�ɹ�
//    ����,�������
u8 MPU_Get_Gyroscope(short *gx,short *gy,short *gz)
{
    u8 buf[6],res;  
	res=Sensors_I2C_ReadRegister(MPU_ADDR,MPU_GYRO_XOUTH_REG,6,buf);
	if(res==0)
	{
		*gx=((u16)buf[0]<<8)|buf[1];  
		*gy=((u16)buf[2]<<8)|buf[3];  
		*gz=((u16)buf[4]<<8)|buf[5];
	} 	
    return res;;
}
//�õ����ٶ�ֵ(ԭʼֵ)
//gx,gy,gz:������x,y,z���ԭʼ����(������)
//����ֵ:0,�ɹ�
//    ����,�������
u8 MPU_Get_Accelerometer(short *ax,short *ay,short *az)
{
    u8 buf[6],res;  
	res=Sensors_I2C_ReadRegister(MPU_ADDR,MPU_ACCEL_XOUTH_REG,6,buf);
	if(res==0)
	{
		*ax=((u16)buf[0]<<8)|buf[1];  
		*ay=((u16)buf[2]<<8)|buf[3];  
		*az=((u16)buf[4]<<8)|buf[5];
	} 	
    return res;;
}



//q30��ʽ,longתfloatʱ�ĳ���.
#define q30  1073741824.0f

//�����Ƿ�������
static signed char gyro_orientation[9] = { 1, 0, 0,
                                           0, 1, 0,
                                           0, 0, 1};
//MPU6050�Բ���
//����ֵ:0,����
//    ����,ʧ��
u8 run_self_test(void)
{
	int result;
	//char test_packet[4] = {0};
	long gyro[3], accel[3]; 
	result = mpu_run_self_test(gyro, accel);
	if (result == 0x3) 
	{
		/* Test passed. We can trust the gyro data here, so let's push it down
		* to the DMP.
		*/
		float sens;
		unsigned short accel_sens;
		mpu_get_gyro_sens(&sens);
		gyro[0] = (long)(gyro[0] * sens);
		gyro[1] = (long)(gyro[1] * sens);
		gyro[2] = (long)(gyro[2] * sens);
		dmp_set_gyro_bias(gyro);
		mpu_get_accel_sens(&accel_sens);
		accel[0] *= accel_sens;
		accel[1] *= accel_sens;
		accel[2] *= accel_sens;
		dmp_set_accel_bias(accel);
		return 0;
	}else return 1;
}
//�����Ƿ������
unsigned short inv_orientation_matrix_to_scalar(
    const signed char *mtx)
{
    unsigned short scalar; 
    /*
       XYZ  010_001_000 Identity Matrix
       XZY  001_010_000
       YXZ  010_000_001
       YZX  000_010_001
       ZXY  001_000_010
       ZYX  000_001_010
     */

    scalar = inv_row_2_scale(mtx);
    scalar |= inv_row_2_scale(mtx + 3) << 3;
    scalar |= inv_row_2_scale(mtx + 6) << 6;


    return scalar;
}
//����ת��
unsigned short inv_row_2_scale(const signed char *row)
{
    unsigned short b;

    if (row[0] > 0)
        b = 0;
    else if (row[0] < 0)
        b = 4;
    else if (row[1] > 0)
        b = 1;
    else if (row[1] < 0)
        b = 5;
    else if (row[2] > 0)
        b = 2;
    else if (row[2] < 0)
        b = 6;
    else
        b = 7;      // error
    return b;
}
//mpu6050,dmp��ʼ��
//����ֵ:0,����
//    ����,ʧ��
u8 mpu_dmp_init(void)
{
	u8 res=0;
	S_I2C_Init(); 	//��ʼ��IIC����
	if(mpu_init(NULL)==0)	//��ʼ��MPU6050
	{	 
		res=mpu_set_sensors(INV_XYZ_GYRO|INV_XYZ_ACCEL);//��������Ҫ�Ĵ�����
		if(res)return 1; 
		res=mpu_configure_fifo(INV_XYZ_GYRO|INV_XYZ_ACCEL);//����FIFO
		if(res)return 2; 
		res=mpu_set_sample_rate(DEFAULT_MPU_HZ);	//���ò�����
		if(res)return 3; 
		res=dmp_load_motion_driver_firmware();		//����dmp�̼�
		
		if(res)return 4; 
		res=dmp_set_orientation(inv_orientation_matrix_to_scalar(gyro_orientation));//���������Ƿ���
		if(res)return 5; 
		res=dmp_enable_feature(DMP_FEATURE_6X_LP_QUAT|DMP_FEATURE_TAP|	//����dmp����
		    DMP_FEATURE_ANDROID_ORIENT|DMP_FEATURE_SEND_RAW_ACCEL|DMP_FEATURE_SEND_CAL_GYRO|
		    DMP_FEATURE_GYRO_CAL);
		if(res)return 6; 
		res=dmp_set_fifo_rate(DEFAULT_MPU_HZ);	//����DMP�������(��󲻳���200Hz)
		if(res)return 7;   
		res=run_self_test();		//�Լ�
		if(res)return 8;    
		res=mpu_set_dmp_state(1);	//ʹ��DMP
		if(res)return 9;     
	}else return 10;
	return 0;
}
//�õ�dmp����������(ע��,��������Ҫ�Ƚ϶��ջ,�ֲ������е��)
//pitch:������ ����:0.1��   ��Χ:-90.0�� <---> +90.0��
//roll:�����  ����:0.1��   ��Χ:-180.0��<---> +180.0��
//yaw:�����   ����:0.1��   ��Χ:-180.0��<---> +180.0��
//����ֵ:0,����
//    ����,ʧ��
u8 mpu_dmp_get_data(float *pitch,float *roll,float *yaw)
{
	float q0=1.0f,q1=0.0f,q2=0.0f,q3=0.0f;
	unsigned long sensor_timestamp;
	short gyro[3], accel[3], sensors;
	unsigned char more;
	long quat[4]; 
	if(dmp_read_fifo(gyro, accel, quat, &sensor_timestamp, &sensors,&more))return 1;	 
	/* Gyro and accel data are written to the FIFO by the DMP in chip frame and hardware units.
	 * This behavior is convenient because it keeps the gyro and accel outputs of dmp_read_fifo and mpu_read_fifo consistent.
	**/
	/*if (sensors & INV_XYZ_GYRO )
	send_packet(PACKET_TYPE_GYRO, gyro);
	if (sensors & INV_XYZ_ACCEL)
	send_packet(PACKET_TYPE_ACCEL, accel); */
	/* Unlike gyro and accel, quaternions are written to the FIFO in the body frame, q30.
	 * The orientation is set by the scalar passed to dmp_set_orientation during initialization. 
	**/
	if(sensors&INV_WXYZ_QUAT) 
	{
		q0 = quat[0] / q30;	//q30��ʽת��Ϊ������
		q1 = quat[1] / q30;
		q2 = quat[2] / q30;
		q3 = quat[3] / q30; 
		//����õ�������/�����/�����
		*pitch = asin(-2 * q1 * q3 + 2 * q0* q2)* 57.3;	// pitch
		*roll  = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1)* 57.3;	// roll
		*yaw   = atan2(2*(q1*q2 + q0*q3),q0*q0+q1*q1-q2*q2-q3*q3) * 57.3;	//yaw
	}else return 2;
	return 0;
}



