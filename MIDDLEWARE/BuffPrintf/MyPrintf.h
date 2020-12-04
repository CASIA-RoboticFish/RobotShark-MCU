/*
 * =====================================================================================
 *
 *       Filename:  MyPrintf.h
 *
 *    Description:  The header of MyPrintf.c
 *
 *        Version:  1.0
 *        Created:  2015/5/15 0:19:32
 *       Revision:  none
 *       Compiler:  Keil ARM_MDK 4.60
 *
 *         Author:  Yang Xiang (Shawn), yangxiang92@163.com
 *        Company:  Insitute of Automation, Chinese Academy of Sciences
 *
 * Modification History:
 * 01a 2015/5/15 0:19:32
 *
 * =====================================================================================
 */
#ifndef _MYPRINTF_H_
#define _MYPRINTF_H_

#define MYPRINTF_STRING_BUFFER_SIZE 20

void MyPrintf_PutChar ( char c );
void MyPrintf_PutString ( char * str );
void MyPrintf ( char * fmt, ... );

void MyPrintf_PutChar_Usart1 ( char c );
void MyPrintf_PutString_Usart1 ( char * str );
void MyPrintf_Usart1 ( char * fmt, ... );

#endif
