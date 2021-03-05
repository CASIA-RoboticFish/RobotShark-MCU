/******************************ʵ�ֺ���**********************************
*��    ��
*��    �ܣ�    ��ȡTFmini������
*��    �룺    
*��    �أ�    ����
*************************************************************************/
#include "TFmini.h"
#include "myiic2.h"


void TFmini_init()
{
    IIC2_Init();
}

/**************************ʵ�ֺ���********************************************
*����ԭ��: u8 TFmini_read_bytes(u8 dev, u8 reg, u8 length, u8 *data )
*��������: ��ȡָ���豸 ָ���Ĵ����� length��ֵ
����	dev  Ŀ���豸��ַ
	read_length  Ҫ�����ֽ���
    send_length  Ҫ�����ֽ���
	*read_data  ���������ݽ�Ҫ��ŵ�ָ��
	*send_data  I2C����Ҫ���ݵ�����ָ��
����    ���������ֽ�����
*******************************************************************************/ 
u8 TFmini_read_bytes(u8 dev, u8 read_length, uint8_t send_length, uint8_t *send_data, uint8_t *read_data)
{
	u8 count = 0;


	IIC2_Start();
	IIC2_Send_Byte(dev<<1);	   //����д����
	IIC2_Wait_Ack();	  
    for(count=0; count<send_length;count++)
	{
        IIC2_Send_Byte(send_data[count]); 
		IIC2_Wait_Ack(); 
	}
    IIC2_Stop();
    delay_ms(100);
	IIC2_Start();
	IIC2_Send_Byte((dev<<1)+1);  //�������ģʽ	
	IIC2_Wait_Ack();
	for(count=0;count<read_length;count++)
	{
		 
		 if(count!=read_length-1)
			read_data[count]=IIC2_Read_Byte(1);  //��ACK�Ķ�����
		 else
			read_data[count]=IIC2_Read_Byte(0);	 //���һ���ֽ�NACK
	}
	IIC2_Stop();//����һ��ֹͣ����
    
	return count;
}

/**************************ʵ�ֺ���********************************************
*����ԭ��: u8 TFmini_write_bytes(u8 dev, u8 reg, u8 length, u8* data)
*��������: ������ֽ�д��ָ���豸 ָ���Ĵ���
����	dev  Ŀ���豸��ַ
	reg	  �Ĵ�����ַ
	length Ҫд���ֽ���
	*data  ��Ҫд�����ݵ��׵�ַ
����    1
*******************************************************************************/ 
u8 TFmini_write_bytes(u8 dev, u8 length, u8 *data)
{
  
	u8 count = 0;
	IIC2_Start();
	IIC2_Send_Byte(dev<<1);	   //����д����
	IIC2_Wait_Ack();	  
	for(count=0;count<length;count++)
	{
		IIC2_Send_Byte(data[count]); 
		IIC2_Wait_Ack(); 
	}
	IIC2_Stop();//����һ��ֹͣ����

	return 1; //status == 0;
	
}

/**************************ʵ�ֺ���********************************************
*����ԭ��: u8 TFmini_data_analyse(uint8_t *data, uint16_t *results)
*��������: �����ش����� ��Ҫ�ǳ���Ϊ9������֡�� [2, 3, 4, 5]λ 
����		*data   	��ȡ�������ݵ�ָ��
		*results	��������Ч���ݽ�� ������Ϊ2�� 0λ��Ų����룬 1λ����ź�ǿ��(0-65535�� Խ��Խ��)��
����    �����Ƿ�ɹ�
*******************************************************************************/ 
uint8_t TFmini_data_analyse(uint8_t *data, uint16_t *results)
{
    uint32_t i = 0;
    uint8_t chk_cal = 0;

    if((TFMINI_DATA_HEAD == data[0])&&(TFMINI_DATA_HEAD == data[1]))
    {
        for(i = 0; i < (TFMINI_DATA_Len - 1); i++)
        {
            chk_cal += data[i];
        }

        if(chk_cal == data[TFMINI_DATA_Len - 1])
        {
            results[0] = data[2] | (data[3] << 8);
            results[1] = data[4] | (data[5] << 8);
        }
    }
    if (results[1] > 100)
    {
        return 1;   //����ֵ���� �ź��㹻ǿ
    }
    else
    {   
        return 0;   //����ֵ�����ţ��źŲ���ǿ   ����  �������ݴ���
    }
}
