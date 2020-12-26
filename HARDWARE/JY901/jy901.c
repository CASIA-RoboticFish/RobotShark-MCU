#include "jy901.h"

void JY901_init()
{
	IIC2_Init();
}
/**************************实现函数********************************************
*函数原型: u8 JY901_read_bytes(u8 dev, u8 reg, u8 length, u8 *data)
*功　　能: 读取指定设备 指定寄存器的 length个值
输入	dev  目标设备地址
	reg  寄存器地址
	length 要读的字节数
	*data  读出的数据将要存放的指针
返回    读出来的字节数量
*******************************************************************************/ 
u8 JY901_read_bytes(u8 dev, u8 reg, u8 length, u8 *data)
{
	u8 count = 0;

	IIC2_Start();
	IIC2_Send_Byte(dev<<1);	   //发送写命令
	IIC2_Wait_Ack();
	IIC2_Send_Byte(reg);   //发送地址
	IIC2_Wait_Ack();	  
	IIC2_Start();
	IIC2_Send_Byte((dev<<1)+1);  //进入接收模式	
	IIC2_Wait_Ack();

	for(count=0;count<length;count++)
	{
		 
		 if(count!=length-1)
			data[count]=IIC2_Read_Byte(1);  //带ACK的读数据
		 else
			data[count]=IIC2_Read_Byte(0);	 //最后一个字节NACK
	}
	IIC2_Stop();//产生一个停止条件
	return count;
}

/**************************实现函数********************************************
*函数原型: u8 JY901_write_bytes(u8 dev, u8 reg, u8 length, u8* data)
*功　　能: 将多个字节写入指定设备 指定寄存器
输入	dev  目标设备地址
	reg	  寄存器地址
	length 要写的字节数
	*data  将要写的数据的首地址
返回    返回是否成功
*******************************************************************************/ 
u8 JY901_write_bytes(u8 dev, u8 reg, u8 length, u8* data)
{
  
	u8 count = 0;
	IIC2_Start();
	IIC2_Send_Byte(dev<<1);	   //发送写命令
	IIC2_Wait_Ack();
	IIC2_Send_Byte(reg);   //发送地址
	IIC2_Wait_Ack();	  
	for(count=0;count<length;count++)
	{
		IIC2_Send_Byte(data[count]); 
		IIC2_Wait_Ack(); 
	}
	IIC2_Stop();//产生一个停止条件

	return 1; //status == 0;
	
}