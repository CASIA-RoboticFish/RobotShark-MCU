/******************************实现函数**********************************
*函    数
*功    能：    读取TFmini的数据
*输    入：    
*返    回：    距离
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

/**************************实现函数********************************************
*函数原型: u8 TFmini_read_bytes(u8 dev, u8 reg, u8 length, u8 *data )
*功　　能: 读取指定设备 指定寄存器的 length个值
输入	dev  目标设备地址
	read_length  要读的字节数
	send_length  要发的字节数
	*read_data  读出的数据将要存放的指针
	*send_data  I2C发送要数据的数组指针
返回    读出来的字节数量
*******************************************************************************/ 
u8 TFmini_read_bytes(u8 dev, u8 read_length, uint8_t send_length, uint8_t *send_data, uint8_t *read_data)
{
	u8 count = 0;


	IIC4_Start();
	IIC4_Send_Byte(dev<<1);	   //发送写命令
	IIC4_Wait_Ack();	  
    for(count=0; count<send_length;count++)
	{
        IIC4_Send_Byte(send_data[count]); 
		IIC4_Wait_Ack(); 
	}
    IIC4_Stop();
    delay_ms(100);
	IIC4_Start();
	IIC4_Send_Byte((dev<<1)+1);  //进入接收模式	
	IIC4_Wait_Ack();
	for(count=0;count<read_length;count++)
	{
		 
		 if(count!=read_length-1)
			read_data[count]=IIC4_Read_Byte(1);  //带ACK的读数据
		 else
			read_data[count]=IIC4_Read_Byte(0);	 //最后一个字节NACK
	}
	IIC4_Stop();//产生一个停止条件
    
	return count;
}

/**************************实现函数********************************************
*函数原型: u8 TFmini_write_bytes(u8 dev, u8 reg, u8 length, u8* data)
*功　　能: 将多个字节写入指定设备 指定寄存器
输入	dev  目标设备地址
	reg	  寄存器地址
	length 要写的字节数
	*data  将要写的数据的首地址
返回    1
*******************************************************************************/ 
u8 TFmini_write_bytes(u8 dev, u8 length, u8 *data)
{
  
	u8 count = 0;
	IIC4_Start();
	IIC4_Send_Byte(dev<<1);	   //发送写命令
	IIC4_Wait_Ack();	  
	for(count=0;count<length;count++)
	{
		IIC4_Send_Byte(data[count]); 
		IIC4_Wait_Ack(); 
	}
	IIC4_Stop();//产生一个停止条件

	return 1; //status == 0;
	
}

/**************************实现函数********************************************
*函数原型: u8 TFmini_data_analyse(uint8_t *data, uint16_t *results)
*功　　能: 解析回传数据 主要是长度为9的数据帧的 [2, 3, 4, 5]位 
输入		*data   	读取到的数据的指针
		*results	解析的有效数据结果 （长度为2： 0位存放测距距离， 1位存放信号强弱(0-65535， 越大越好)）
返回    返回是否成功
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
        return 1;   //距离值可信 信号足够强
    }
    else
    {   
        return 0;   //距离值不可信，信号不够强   或者  返回数据错误
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
