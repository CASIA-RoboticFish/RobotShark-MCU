/*****************************************************************************
Copyright: CASIA ����������ʵ���ң�����ԭ��
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
//���ʹ��ucos,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_UCOS
#include "includes.h"                   //ucos ʹ��
#endif

#if EN_USART6_RX   //���ʹ���˽���
//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���
u8 USART6_RX_BUF[USART6_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��   ������ɱ�־
//bit14��   ���յ�0x0d
//bit13~0�� ���յ�����Ч�ֽ���Ŀ
u16 USART6_RX_STA=0;       //����״̬���

int uart6_fputc(int ch)
{
    while((USART6->SR&0X40)==0);//ѭ������,ֱ���������
    USART6->DR = (u8) ch;
    return ch;
}
/********************************************************************************
*
* Function name ��uart2_init
* Description   ����ʼ��IO ����2
*                 
* Parameter     ��
* 		@bound    ��������
* Return        ��null
********************************************************************************/
void uart6_init(u32 bound)
{
   //GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE); //ʹ��GPIOAʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6,ENABLE);//ʹ��USART6ʱ��

		
	  //����2��Ӧ���Ÿ���ӳ��
    GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_USART6); //GPIOA2����ΪUSART6
    GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_USART6); //GPIOA3����ΪUSART6

    //USART1�˿�����
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; //GPIOA2��GPIOA3
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   //�ٶ�50MHz
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
    GPIO_Init(GPIOC,&GPIO_InitStructure); //��ʼ��PA2��PA3

   //USART1 ��ʼ������
    USART_InitStructure.USART_BaudRate = bound;//����������
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
    USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //�շ�ģʽ
    USART_Init(USART6, &USART_InitStructure); //��ʼ������2

    //�����ж�
    USART_ITConfig(USART6, USART_IT_TXE, DISABLE); //�رշ����ж�
    USART_ITConfig(USART6, USART_IT_RXNE, ENABLE); //���������ж�
		USART_ClearFlag(USART6, USART_FLAG_TC);
		USART_Cmd(USART6, ENABLE);  //ʹ�ܴ���2
		

#if EN_USART6_RX
    USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);//��������ж�

    //USART3 NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;//����1�ж�ͨ��
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//��ռ���ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;       //�����ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;         //IRQͨ��ʹ��
    NVIC_Init(&NVIC_InitStructure); //����ָ���Ĳ�����ʼ��VIC�Ĵ���

#endif

}


/********************************************************************************
*
* Function name ��USART6_IRQHandler
* Description   ������2�жϷ�����
*                 
* Parameter     ��
* 		@null
* Return        ��null
********************************************************************************/
void USART6_IRQHandler(void)                	//����2�жϷ������
{
	u8 Res;
	OS_ERR err;
    uint32_t i = 0;
    uint8_t chk_cal = 0;

#if SYSTEM_SUPPORT_OS 		//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(USART6, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		Res =USART_ReceiveData(USART6);//(USART3->DR);	//��ȡ���յ�������
        
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

		
//		if((USART6_RX_STA&0x8000)==0)//����δ���
//		{
//			if(USART6_RX_STA&0x4000)//���յ���0x0d
//			{
//				if(Res!=0x0a)USART6_RX_STA=0;//���մ���,���¿�ʼ
//				else USART6_RX_STA|=0x8000;	//��������� 
//			}
//			else //��û�յ�0X0D
//			{	
//				if(Res==0x0d)USART6_RX_STA|=0x4000;
//				else
//				{
//					USART6_RX_BUF[USART6_RX_STA&0X3FFF]=Res ;
//					USART6_RX_STA++;
//					if(USART6_RX_STA>(USART6_REC_LEN-1))USART6_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
//				}		 
//			}
//			// д���ճ���***
//			//������ڽ���Ϣ���У�����������Ϣ
//			OSTaskQPost(&TestTaskTCB,    // CommandAnalysisSlaveTCB
//                        (void *)Res,
//                        sizeof(Res),
//                        OS_OPT_POST_FIFO,
//                        &err);
//		}   		 
  } 
#if SYSTEM_SUPPORT_OS 	//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
	OSIntExit();  											 
#endif
} 

#endif





