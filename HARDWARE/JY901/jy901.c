#include "jy901.h"

void JY901_init()
{
	IIC2_Init();
}
/**************************ʵ�ֺ���********************************************
*����ԭ��: u8 JY901_read_bytes(u8 dev, u8 reg, u8 length, u8 *data)
*��������: ��ȡָ���豸 ָ���Ĵ����� length��ֵ
����	dev  Ŀ���豸��ַ
	reg  �Ĵ�����ַ
	length Ҫ�����ֽ���
	*data  ���������ݽ�Ҫ��ŵ�ָ��
����    ���������ֽ�����
*******************************************************************************/ 
u8 JY901_read_bytes(u8 dev, u8 reg, u8 length, u8 *data)
{
	u8 count = 0;

	IIC2_Start();
	IIC2_Send_Byte(dev<<1);	   //����д����
	IIC2_Wait_Ack();
	IIC2_Send_Byte(reg);   //���͵�ַ
	IIC2_Wait_Ack();	  
	IIC2_Start();
	IIC2_Send_Byte((dev<<1)+1);  //�������ģʽ	
	IIC2_Wait_Ack();

	for(count=0;count<length;count++)
	{
		 
		 if(count!=length-1)
			data[count]=IIC2_Read_Byte(1);  //��ACK�Ķ�����
		 else
			data[count]=IIC2_Read_Byte(0);	 //���һ���ֽ�NACK
	}
	IIC2_Stop();//����һ��ֹͣ����
	return count;
}

/**************************ʵ�ֺ���********************************************
*����ԭ��: u8 JY901_write_bytes(u8 dev, u8 reg, u8 length, u8* data)
*��������: ������ֽ�д��ָ���豸 ָ���Ĵ���
����	dev  Ŀ���豸��ַ
	reg	  �Ĵ�����ַ
	length Ҫд���ֽ���
	*data  ��Ҫд�����ݵ��׵�ַ
����    �����Ƿ�ɹ�
*******************************************************************************/ 
u8 JY901_write_bytes(u8 dev, u8 reg, u8 length, u8* data)
{
  
	u8 count = 0;
	IIC2_Start();
	IIC2_Send_Byte(dev<<1);	   //����д����
	IIC2_Wait_Ack();
	IIC2_Send_Byte(reg);   //���͵�ַ
	IIC2_Wait_Ack();	  
	for(count=0;count<length;count++)
	{
		IIC2_Send_Byte(data[count]); 
		IIC2_Wait_Ack(); 
	}
	IIC2_Stop();//����һ��ֹͣ����

	return 1; //status == 0;
	
}