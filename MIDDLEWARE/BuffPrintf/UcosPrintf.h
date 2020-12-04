/*
 * =====================================================================================
 *
 *       Filename:  UcosPrintf.h
 *
 *    Description:  The header of UcosPrintf.c
 *
 *        Version:  1.0
 *        Created:  2015/5/15 17:52:24
 *       Revision:  none
 *       Compiler:  Keil ARM_MDK 4.60
 *
 *         Author:  Yang Xiang (Shawn), yangxiang92@163.com
 *        Company:  Insitute of Automation, Chinese Academy of Sciences
 *
 * Modification History:
 * 01a 2015/5/15 17:52:24
 *
 * =====================================================================================
 */
#ifndef _UCOSPRINTF_H_
#define _UCOSPRINTF_H_

#include "includes.h"   // The header of UCOS-III

#define UCOSPRINTF_MUTEX_WAIT_TIME_MS 100

void UcosPrintfInit ( void );
void UcosPrintf ( char * fmt, ... );

#endif
