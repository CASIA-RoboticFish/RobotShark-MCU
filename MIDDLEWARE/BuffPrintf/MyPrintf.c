/*
 * =====================================================================================
 *
 *       Filename:  MyPrintf.c
 *
 *    Description:  The printf function of my own definition
 *
 *        Version:  1.0
 *        Created:  2015/5/15 0:18:07
 *       Revision:  none
 *       Compiler:  Keil ARM-MDK 4.60
 *
 *         Author:  Yang Xiang (Shawn), yangxiang92@163.com
 *        Company:  Insitute of Automation, Chinese Academy of Sciences
 *
 * Modification History:
 * 01a 2015/5/15 0:18:07
 *
 * =====================================================================================
 */
#include "MyPrintf.h"

#include <stdio.h>
#include <stdarg.h>

/*-----------------------------------------------------------------------------
 *  ###########################################################################
 *  BEGIN of Self-define Region
 *  ###########################################################################
 *-----------------------------------------------------------------------------*/
#include "usart3.h"
#include "usart1.h"
/*-----------------------------------------------------------------------------
 *  TODO:
 *  Change this MyPrintf_PutChar function into you function, of which should be
 *  able to send one single char to your target device.
 *-----------------------------------------------------------------------------*/
/*
 * ===  FUNCTION  ======================================================================
 *
 *         Name: void MyPrintf_PutChar
 *  Description: Send one single char to your device
 *
 * Modification History:
 * 01a 2015/5/15 0:22:00
 *
 * =====================================================================================
 */
void MyPrintf_PutChar ( char c )
{
    FILE * fp = NULL;
    fputc(c, fp);
}       /* -----  end of function void MyPrintf_PutChar  ----- */

void MyPrintf_PutChar_Usart1 ( char c )
{
    FILE * fp = NULL;
    fputc_usart1(c, fp);
}       /* -----  end of function void MyPrintf_PutChar  ----- */
/*-----------------------------------------------------------------------------
 *  ############################################################################
 *  END of Self-define Region
 *  ############################################################################
 *-----------------------------------------------------------------------------*/
/*
 * ===  FUNCTION  ======================================================================
 *
 *         Name: void MyPrintf_PutString
 *  Description: Send a string to device
 *
 * Modification History:
 * 01a 2015/5/15 0:25:50
 *
 * =====================================================================================
 */
void MyPrintf_PutString ( char * str )
{
	// '\0'这个作为终止符会出现问题，所以换成0xEF
    while(*str != 0xEF)
    {
        MyPrintf_PutChar(*str);
        str ++;
    }
}       /* -----  end of function void MyPrintf_PutString  ----- */

void MyPrintf_PutString_Usart1 ( char * str )
{
	// '\0'这个作为终止符会出现问题，所以换成0xEF
    while(*str != 0xEF)
    {
        MyPrintf_PutChar_Usart1(*str);
        str ++;
    }
}       /* -----  end of function void MyPrintf_PutString  ----- */
/*
 * ===  FUNCTION  ======================================================================
 *
 *         Name: void MyPrintf
 *  Description: My printf function(only have the minimun amount functions)
 *
 * Modification History:
 * 01a 2015/5/15 0:27:05
 *
 * =====================================================================================
 */
void MyPrintf ( char * fmt, ... )
{
    va_list ap;
    char *p, *sval;
    int ival;
    double dval;
    char buf[MYPRINTF_STRING_BUFFER_SIZE];

    va_start(ap, fmt);
    for (p = fmt; *p; p++)
    {
        if(*p != '%')
        {
            MyPrintf_PutChar(*p);
            continue;
        }

        switch(*++p)
        {
        case 'd':
            ival = va_arg(ap, int);
            sprintf(buf, "%d", ival);
            MyPrintf_PutString(buf);
            break;
        case 'f':
            dval = va_arg(ap, double);
            sprintf(buf, "%f", dval);
            MyPrintf_PutString(buf);
            break;
        case 's':
            sval = va_arg(ap, char *);
            MyPrintf_PutString(sval);
            break;
        default:
            MyPrintf_PutChar(*(p-1));
            MyPrintf_PutChar(*p);
            break;
        }
    }
    va_end(ap);
}       /* -----  end of function void MyPrintf  ----- */

void MyPrintf_Usart1 ( char * fmt, ... )
{
    va_list ap;
    char *p, *sval;
    int ival;
    double dval;
    char buf[MYPRINTF_STRING_BUFFER_SIZE];

    va_start(ap, fmt);
    for (p = fmt; *p; p++)
    {
        if(*p != '%')
        {
            MyPrintf_PutChar_Usart1(*p);
            continue;
        }

        switch(*++p)
        {
        case 'd':
            ival = va_arg(ap, int);
            sprintf(buf, "%d", ival);
            MyPrintf_PutString_Usart1(buf);
            break;
        case 'f':
            dval = va_arg(ap, double);
            sprintf(buf, "%f", dval);
            MyPrintf_PutString_Usart1(buf);
            break;
        case 's':
            sval = va_arg(ap, char *);
            MyPrintf_PutString_Usart1(sval);
            break;
        default:
            MyPrintf_PutChar_Usart1(*(p-1));
            MyPrintf_PutChar_Usart1(*p);
            break;
        }
    }
    va_end(ap);
}       /* -----  end of function void MyPrintf  ----- */