/*****************************************************************************
Copyright: CASIA 仿生机器鱼实验室，正点原子
File name: 
Description: 
Author: VincentFEI
Version: 1.0.0
Date: 2019.07.03
History: 
*****************************************************************************/
#include "sys.h"
#include "usart6.h"
#include "TFmini.h"
#include "command_analysis_slave_app.h"

uint16_t cordist = 0;
//extern OS_TCB TestTaskTCB;   //CommandAnalysisSlaveTCB;

//////////////////////////////////////////////////////////////////////////////////
//如果使用ucos,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_UCOS
#include "includes.h"                   //ucos 使用
#endif

#if EN_USART6_RX   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误
u8 USART6_RX_BUF[USART6_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，   接收完成标志
//bit14，   接收到0x0d
//bit13~0， 接收到的有效字节数目
u16 USART6_RX_STA=0;       //接收状态标记

int uart6_fputc(int ch)
{
    while((USART6->SR&0X40)==0);//循环发送,直到发送完毕
    USART6->DR = (u8) ch;
    return ch;
}
/********************************************************************************
*
* Function name ：uart2_init
* Description   ：初始化IO 串口2
*                 
* Parameter     ：
* 		@bound    ：波特率
* Return        ：null
********************************************************************************/
void uart6_init(u32 bound)
{
   //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE); //使能GPIOA时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6,ENABLE);//使能USART6时钟

		
	  //串口2对应引脚复用映射
    GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_USART6); //GPIOA2复用为USART6
    GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_USART6); //GPIOA3复用为USART6

    //USART1端口配置
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; //GPIOA2与GPIOA3
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   //速度50MHz
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
    GPIO_Init(GPIOC,&GPIO_InitStructure); //初始化PA2，PA3

   //USART1 初始化设置
    USART_InitStructure.USART_BaudRate = bound;//波特率设置
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //收发模式
    USART_Init(USART6, &USART_InitStructure); //初始化串口2

    //串口中断
    USART_ITConfig(USART6, USART_IT_TXE, DISABLE); //关闭发送中断
    USART_ITConfig(USART6, USART_IT_RXNE, ENABLE); //开启接收中断
		USART_ClearFlag(USART6, USART_FLAG_TC);
		USART_Cmd(USART6, ENABLE);  //使能串口2
		

#if EN_USART6_RX
    USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);//开启相关中断

    //USART3 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;//串口1中断通道
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;       //子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;         //IRQ通道使能
    NVIC_Init(&NVIC_InitStructure); //根据指定的参数初始化VIC寄存器

#endif

}


/********************************************************************************
*
* Function name ：USART6_IRQHandler
* Description   ：串口2中断服务函数
*                 
* Parameter     ：
* 		@null
* Return        ：null
********************************************************************************/
void USART6_IRQHandler(void)                	//串口2中断服务程序
{
	u8 Res;
	OS_ERR err;
    uint32_t i = 0;
    uint8_t chk_cal = 0;

#if SYSTEM_SUPPORT_OS 		//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(USART6, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		Res =USART_ReceiveData(USART6);//(USART3->DR);	//读取接收到的数据
        
        USART6_RX_BUF[USART6_RX_STA&0X3FFF] = Res;
        USART6_RX_STA++;
        
        if (TFMINI_DATA_Len == USART6_RX_STA)
        {
            if ((USART6_RX_BUF[0] == TFMINI_DATA_HEAD)&&(USART6_RX_BUF[1]==TFMINI_DATA_HEAD))
            {
                for(i = 0; i < (TFMINI_DATA_Len - 1); i++)
                {
                    chk_cal += USART6_RX_BUF[i];
                }
                if (chk_cal == USART6_RX_BUF[TFMINI_DATA_Len - 1])
                {
                    cordist = USART6_RX_BUF[2] | (USART6_RX_BUF[3] << 8);
                }
            }
            USART6_RX_STA = 0;
        }

		
//		if((USART6_RX_STA&0x8000)==0)//接收未完成
//		{
//			if(USART6_RX_STA&0x4000)//接收到了0x0d
//			{
//				if(Res!=0x0a)USART6_RX_STA=0;//接收错误,重新开始
//				else USART6_RX_STA|=0x8000;	//接收完成了 
//			}
//			else //还没收到0X0D
//			{	
//				if(Res==0x0d)USART6_RX_STA|=0x4000;
//				else
//				{
//					USART6_RX_BUF[USART6_RX_STA&0X3FFF]=Res ;
//					USART6_RX_STA++;
//					if(USART6_RX_STA>(USART6_REC_LEN-1))USART6_RX_STA=0;//接收数据错误,重新开始接收	  
//				}		 
//			}
//			// 写接收程序***
//			//任务的内建消息队列，向任务发送消息
//			OSTaskQPost(&TestTaskTCB,    // CommandAnalysisSlaveTCB
//                        (void *)Res,
//                        sizeof(Res),
//                        OS_OPT_POST_FIFO,
//                        &err);
//		}   		 
  } 
#if SYSTEM_SUPPORT_OS 	//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntExit();  											 
#endif
} 

#endif





