/*****************************************************************************
Copyright: CASIA 仿生机器鱼实验室
File name: 
Description: 
Author: VincentFEI
Version: 1.0.0
Date: 2019.07.03
History: 
*****************************************************************************/

#include "rflink.h"

static uint32_t s_u32_analysis_package_count = 0;
static uint32_t s_u32_receive_message_count = 0;
static uint8_t s_u8_shaking_hands_state = 0;

static RFMessage tx_message;
static RFMessage rx_message_set[3];
static Recstate receive_state_set[3]={WAITING_FF, WAITING_FF, WAITING_FF};
static uint32_t receive_check_sum_set[3];
static uint8_t receive_message_length_set[3];
static uint16_t byte_count_set[3];


uint8_t RFLink_byteAnalysisCall(uint8_t usart_id, uint8_t rx_data)
{
	if(RFLink_receiveStates(usart_id, rx_data))
	{
		return 1;
	}
		
	return 0;
}

uint8_t RFLink_receiveStates(uint8_t usart_id, uint8_t rx_data)
{
	Recstate receive_state = receive_state_set[usart_id];
	uint32_t receive_check_sum = receive_check_sum_set[usart_id];
	uint8_t receive_message_length = receive_message_length_set[usart_id];
	uint16_t byte_count = byte_count_set[usart_id];
	
	
	switch(receive_state)
	{
		case WAITING_FF:
			if (rx_data == 0xff)
			{
				receive_state = SENDER_ID;
				receive_check_sum =0;
				receive_message_length = 0;
				byte_count = 0;
				receive_check_sum += rx_data;
			}
			
			break;
			
			
		case SENDER_ID:
			rx_message_set[usart_id].sender_id = rx_data;
			if (rx_data == FRIEND_ID1 || rx_data == FRIEND_ID2 || (rx_data > 0x00 && rx_data < BOXFISH_NUM+1))  //id check
			{
					receive_check_sum += rx_data;
					receive_state = RECEIVER_ID;
			}
			else
			{
					receive_state = WAITING_FF;
			}	
			break;
			
		case RECEIVER_ID:
			rx_message_set[usart_id].receiver_id = rx_data ;
			if (rx_data == MY_ID || rx_data == BOXFISH_ALL)  //id check
			{
					receive_check_sum += rx_data;
					receive_state = RECEIVE_LEN;
			}
			else
			{
					receive_state = WAITING_FF;
			}		
			break;
		
		
		case RECEIVE_LEN:
			receive_check_sum += rx_data;
			receive_message_length = rx_data;
			rx_message_set[usart_id].length = receive_message_length;
			receive_state = RECEIVE_PACKAGE;
			break;
		
		case RECEIVE_PACKAGE:
			receive_check_sum += rx_data;
			rx_message_set[usart_id].data[byte_count++] = rx_data;
			if(byte_count > receive_message_length)
			{
					receive_state = RECEIVE_CHECK;
					receive_check_sum=receive_check_sum % 255;
			}
			break;
		
		case RECEIVE_CHECK:
			if(rx_data == (uint8_t)receive_check_sum)
			{
					receive_check_sum=0;
					receive_state = WAITING_FF;
					s_u32_receive_message_count ++ ;
				
					receive_state_set[usart_id] = receive_state;
					return 1 ;
			}
			else
			{
					receive_state = WAITING_FF;
			}
			break;

		default:
			receive_state = WAITING_FF;
		
	}
	
	receive_state_set[usart_id] = receive_state;
	receive_check_sum_set[usart_id] = receive_check_sum;
	receive_message_length_set[usart_id] = receive_message_length;
	byte_count_set[usart_id] = byte_count;
	
	return 0;
}



AnalysisResult RFLink_commandAnalysis(uint8_t usart_id)
{
	AnalysisResult result;

	result.command = (Command)rx_message_set[usart_id].data[0];
	result.message_len = rx_message_set[usart_id].length;

	if(s_u8_shaking_hands_state==0 && result.command != SHAKING_HANDS && result.command != SYNCHRONIZE_CLOCK) //if not  shaking hands
	{
		// 上位机没有和下位机握手的话，所有的指令都为0
		result.command = 0;
		return result;
	}
	
	if(result.command == SHAKING_HANDS || result.command == SYNCHRONIZE_CLOCK)
	{
		s_u8_shaking_hands_state = 1;
	}
	
	if(rx_message_set[usart_id].length != 0)
	{
		memcpy(result.message, rx_message_set[usart_id].data+1,rx_message_set[usart_id].length);
		*(result.message+rx_message_set[usart_id].length) = '\0';
	}	
	else
	{
		result.message[0] = 0xFF;
	}
	return result;
}


void RFLink_sendStruct(uint8_t receiver_id, const Command command_type , unsigned char* p , uint8_t len)
{
    tx_message.sender_id = MY_ID;
    tx_message.receiver_id = receiver_id;
    tx_message.length=len;
    tx_message.data[0] = (unsigned char)command_type;
    if(len > 0)
    {
        memcpy(&tx_message.data[1], p, len);
    }
		
    if(receiver_id==FRIEND_ID1)
	RFLinkPrintf(&tx_message);
    else if(receiver_id==FRIEND_ID2)
	RFLinkUart6Printf(&tx_message);
    else if(receiver_id > 0x00 && receiver_id < BOXFISH_NUM+1)
	DistComPrintf(&tx_message);
}


void RFLink_message(char * msg, ...)
{
	va_list ap;
	char *p, *sval;
	int ival;
	double fval;
	char buf[30];
	uint8_t strcnt = 0;
	///////////////////////////////////////////////////////////////////////////////

	char str_buf[64];
	char * str_buf_p = str_buf;
	unsigned char i = 0;
	
	va_start(ap, msg);
	for (p = msg; *p; p++)
	{
			if(*p != '%')
			{
					*str_buf_p = *p;
					str_buf_p ++;
					strcnt ++;
					continue;
			}

			switch(*++p)
			{
			case 'd':
					ival = va_arg(ap, int);
					sprintf(buf, "%d", ival);
					for(i = 0; buf[i] != '\0'; i++)
					{
							*str_buf_p = buf[i];
							str_buf_p ++;
							strcnt ++;
					}
					break;
			case 'f':
					fval = va_arg(ap, double);
					sprintf(buf, "%f", fval);
					for(i = 0; buf[i] != '\0'; i++)
					{
							*str_buf_p = buf[i];
							str_buf_p ++;
							strcnt ++;
					}
					break;
			case 's':
					sval = va_arg(ap, char *);
					while(*sval != '\0')
					{
							*str_buf_p = *sval;
							str_buf_p ++;
							strcnt ++;
							sval ++;
					}
					break;
			default:
					*str_buf_p = *(p-1);
					*str_buf_p = *p;
					break;
			}
	}
	va_end(ap);
	
	RFLink_sendStruct(FRIEND_ID1, PRINT_SYS_MSG, str_buf, strcnt);
	
}

