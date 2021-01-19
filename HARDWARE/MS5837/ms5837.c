/*****************************************************************************
Copyright: CASIA ����������ʵ����
File name: 
Description: 
Author: VincentFEI
Version: 1.0.0
Date: 2019.07.03
History: 
*****************************************************************************/
#include "ms5837.h"
#include "myiic3.h"
#include "BuffPrintf.h"
/*
C1 ѹ�������� SENS|T1
C2  ѹ������  OFF|T1
C3	�¶�ѹ��������ϵ�� TCS
C4	�¶�ϵ����ѹ������ TCO
C5	�ο��¶� T|REF
C6 	�¶�ϵ�����¶� TEMPSENS
*/
uint16_t  Cal_C[7];	     // ���ڴ��PROM�е�6������1-6
int32_t dT,MS5837_TEMP;  // ȫ�ֱ���
float global_depth;

// ����д��
unsigned long MS583703BA_getConversion(uint8_t command)
{
 
		unsigned long conversion = 0;
		u8 temp[3];

		IIC3_Start();
		IIC3_Send_Byte(0xEC); 		//д��ַ
		IIC3_Wait_Ack();
		IIC3_Send_Byte(command); //дת������
		IIC3_Wait_Ack();
		IIC3_Stop();

		delay_ms(10);
		IIC3_Start();
		IIC3_Send_Byte(0xEC); 		//д��ַ
		IIC3_Wait_Ack();
		IIC3_Send_Byte(0);				// start read sequence
		IIC3_Wait_Ack();
		IIC3_Stop();
	 
		IIC3_Start();
		IIC3_Send_Byte(0xEC+0x01);  //�������ģʽ
		IIC3_Wait_Ack();
		temp[0] = IIC3_Read_Byte(1);  //��ACK�Ķ�����  bit 23-16
		temp[1] = IIC3_Read_Byte(1);  //��ACK�Ķ�����  bit 8-15
		temp[2] = IIC3_Read_Byte(0);  //��NACK�Ķ����� bit 0-7
		IIC3_Stop();
		
		conversion = (unsigned long)temp[0] * 65536 + (unsigned long)temp[1] * 256 + (unsigned long)temp[2];
		return conversion;
}


// ����д��
void MS583703BA_getTemperature(void)
{
	uint32_t D2_Temp;
	D2_Temp = MS583703BA_getConversion(0x48);
	dT=D2_Temp - (((int32_t)Cal_C[5])*256);
	MS5837_TEMP=2000+(int64_t)dT*((int64_t)Cal_C[6])/8388608;
}

// ����д��
double MS583703BA_getPressure(void)
{
	double Pressure;
	
	uint32_t D1_Pres;

	int64_t OFF_; // OFF ʵ���¶Ȳ���
	int64_t SENS; // SENS ʵ���¶�������
	int64_t OFF2,SENS2;
	int32_t T2;	//�¶�У��ֵ
	
	D1_Pres= MS583703BA_getConversion(0x48);
	
	OFF_=(int64_t)Cal_C[2]*65536+((int64_t)Cal_C[4]*(int64_t)dT)/128;
	SENS=(int64_t)Cal_C[1]*32768+((int64_t)Cal_C[3]*(int64_t)dT)/256;
	
	MS5837_TEMP = 2000;
	if(MS5837_TEMP<2000)  // low temp
	{
		T2 = 3*((int64_t)dT*(int64_t)dT) / 8589934592; 
		//T2 = 3*((int64_t)dT*(int64_t)dT) / 0x80000000
		OFF2 = 3 * (MS5837_TEMP - 2000) * (MS5837_TEMP - 2000)/2; 
		SENS2 = 5 * (MS5837_TEMP - 2000) * (MS5837_TEMP - 2000)/8; 	
	}
	else
	{
		T2=2*((int64_t)dT*(int64_t)dT)/137438953472;
		OFF2 = (MS5837_TEMP - 2000) * (MS5837_TEMP - 2000)/16; 
		SENS2 = 0;
	 }
	OFF_ = OFF_ - OFF2;
	SENS = SENS - SENS2;	
	Pressure=(double)((D1_Pres*SENS/2097152-OFF_)/8192)/10;
	MS5837_TEMP=(MS5837_TEMP-T2)/100;
	return Pressure;
}


// **************************************************************
// �Լ��ĺ�������ʼ��Ӳ��
void ms5837_init(void)
{
	IIC3_Init();
	u8 inth,intl;
	delay_ms(20);
  int i;
  for (i=1;i<=6;i++) 
	{
 
		IIC3_Start();
    IIC3_Send_Byte(0xEC);
		IIC3_Wait_Ack();
		IIC3_Send_Byte(0xA0 + (i*2));
		IIC3_Wait_Ack();
    IIC3_Stop();
		delay_us(5);
		IIC3_Start();
		IIC3_Send_Byte(0xEC+0x01);  //�������ģʽ
		delay_us(1);
		IIC3_Wait_Ack();
		inth = IIC3_Read_Byte(1);  		//��ACK�Ķ�����
		delay_us(1);
		intl = IIC3_Read_Byte(0); 			//���һ���ֽ�NACK		
		IIC3_Stop();
    Cal_C[i] = (((uint16_t)inth << 8) | intl);
	}
}

// �����豸
void ms5837_reset(void)
{
	delay_us(100);
	IIC3_Start();
	IIC3_Send_Byte(0xEC);//CSB�ӵأ�������ַ��0XEE������ 0X77
	IIC3_Wait_Ack();
	IIC3_Send_Byte(0x1E);//���͸�λ����
	IIC3_Wait_Ack();
	IIC3_Stop();
}

// ��ȡѹ������
float get_ms5837_data(void)
{
	double pressure;
	double ms5837_value;
	MS583703BA_getTemperature();//��ȡ�¶�
	pressure = MS583703BA_getPressure();   //��ȡѹ��
	ms5837_value = pressure; // ��λ�ǣ�mbar��1 mbar=100 Pa
	float depth = 0.0f;
	// ����ѹ=101.325 Kpa=101325 Pa
	// P = rho*g*h
	// h = P/(rho*g)
	depth = 100*(ms5837_value*100 - 101300)/(1029*9.80665); // ��λ������
	global_depth = depth;
	return depth;
}











