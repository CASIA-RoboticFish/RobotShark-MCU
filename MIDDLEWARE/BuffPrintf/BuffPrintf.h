/*
 * =====================================================================================
 *
 *       Filename:  BuffPrintf.h
 *
 *    Description:  The header of BuffPrintf.c
 *
 *        Version:  1.0
 *        Created:  2015/5/17 11:56:57
 *       Revision:  none
 *       Compiler:  Keil ARM-MDK V4.60
 *
 *         Author:  Yang Xiang (Shawn), yangxiang92@163.com
 *        Company:  Insitute of Automation, Chinese Academy of Sciences
 *
 * Modification History:
 * 01a 2015/5/17 11:56:57
 *
 * =====================================================================================
 */
#ifndef _BUFFPRINTF_H_
#define _BUFFPRINTF_H_

#include "includes.h"
#include "rflink.h"

#define BUFFPRINTF_CFG_REPORT_DATA_MISS

#define BUFFPRINTF_MAX_QUEUE_NUM    50
#define BUFFPRINTF_QUEUE_UNIT_SIZE  128

#define BUFFPRINTF_TASK_PRIO        30
#define BUFFPRINTF_STK_SIZE         512

void BuffPrintf_Init ( void );
unsigned char BuffPrintf ( char * fmt, ... );
unsigned char RFLinkPrintf ( const RFMessage* tx_message );
void BuffPrintf_Stop ( void );
void BuffPrintf_Resume ( void );
#endif
