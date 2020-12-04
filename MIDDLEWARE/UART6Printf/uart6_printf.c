/*****************************************************************************
Copyright: CASIA 仿生机器鱼实验室
File name: 
Description: 
Author: VincentFEI
Version: 1.0.0
Date: 2019.07.03
History: 
*****************************************************************************/
#include "uart6_printf.h"
void RFLinkUart6Printf ( const RFMessage* tx_message )
{
		uint32_t check_sum = 0;
		
		// 按照RFLink协议发送
		check_sum = 0;
		uart6_fputc(0xff);
		check_sum += 0xff;
		
		uart6_fputc(tx_message->sender_id);
		check_sum += tx_message->sender_id;
	
		uart6_fputc(tx_message->receiver_id);
		check_sum += tx_message->receiver_id;
	
		uart6_fputc((unsigned char)tx_message->length);
		check_sum += (unsigned char)tx_message->length;
	
		unsigned short int tx_i  = 0;
    for(tx_i = 0;  tx_i < tx_message->length+1; tx_i++)   //package
    {
			uart6_fputc(tx_message->data[tx_i]);
			check_sum += tx_message->data[tx_i];
    }
		
		check_sum = check_sum % 255;
		uart6_fputc((uint8_t)check_sum);
	
}