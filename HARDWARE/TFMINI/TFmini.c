/******************************ʵ�ֺ���**********************************
*��    ��
*��    �ܣ�    ��ȡTFmini������
*��    �룺    
*��    �أ�    ����
*************************************************************************/
#include "TFmini.h"
#include "myiic4.h"

u8 tf_command_sys_reset[4] = {0x5A, 0x04, 0x02, 0x60};
u8 tf_command_get_dist[5] = {0x5A, 0x05, 0x00, 0x01, 0x60};
u8 tf_command_uart2iic[5] = {0x5A, 0x05, 0x0A, 0x00, 0x69};
u8 tf_command_iic2uart[5] = {0x5A, 0x05, 0x0A, 0x01, 0x6A};
u8 tf_command_save_cfg[4] = {0x5A, 0x04, 0x11, 0x6F};

void TFmini_init()
{
    IIC4_Init();
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


	IIC4_Start();
	IIC4_Send_Byte(dev<<1);	   //����д����
	IIC4_Wait_Ack();	  
    for(count=0; count<send_length;count++)
	{
        IIC4_Send_Byte(send_data[count]); 
		IIC4_Wait_Ack(); 
	}
    IIC4_Stop();
    delay_ms(100);
	IIC4_Start();
	IIC4_Send_Byte((dev<<1)+1);  //�������ģʽ	
	IIC4_Wait_Ack();
	for(count=0;count<read_length;count++)
	{
		 
		 if(count!=read_length-1)
			read_data[count]=IIC4_Read_Byte(1);  //��ACK�Ķ�����
		 else
			read_data[count]=IIC4_Read_Byte(0);	 //���һ���ֽ�NACK
	}
	IIC4_Stop();//����һ��ֹͣ����
    
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
	IIC4_Start();
	IIC4_Send_Byte(dev<<1);	   //����д����
	IIC4_Wait_Ack();	  
	for(count=0;count<length;count++)
	{
		IIC4_Send_Byte(data[count]); 
		IIC4_Wait_Ack(); 
	}
	IIC4_Stop();//����һ��ֹͣ����

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
        for(i = 0; i < (TFMINI_DATA_LEN - 1); i++)
        {
            chk_cal += data[i];
        }

        if(chk_cal == data[TFMINI_DATA_LEN - 1])
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


uint8_t TFmini_get_distance()
{
//	int8_t TFmini_foreward[TFMINI_DATA_Len];
////	uint8_t Send_Data[] = {0x5a, 0x04, 0x02, 0x60};
////	uint16_t foreward_dist = 0;
////    
//// uint8_t TFmini_forward[TFMINI_DATA_Len];
////    uint16_t TFmini_results[2];

////    uint8_t if_data_creticable = 0;
////    uint8_t send_length = (uint8_t) (sizeof(c_get_dist) /sizeof(c_get_dist[0]));
////                                                                                 	while(1)
////	{
////       
//	TFmini_read_bytes(TFMINI, TFMINI_DATA_Len, send_length, &c_get_dist[0], &TFmini_forward[0]);
//	if_data_creticable = TFmini_data_analyse(&TFmini_forward[0], &TFmini_results[0]);
	
	return  0;
}
