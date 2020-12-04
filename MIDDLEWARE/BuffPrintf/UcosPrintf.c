/*
 * =====================================================================================
 *
 *       Filename:  UcosPrintf.c
 *
 *    Description:  The printf used in the UCOS system
 *
 *        Version:  1.0
 *        Created:  2015/5/15 17:51:43
 *       Revision:  none
 *       Compiler:  Keil ARM_MDK 4.60
 *
 *         Author:  Yang Xiang (Shawn), yangxiang92@163.com
 *        Company:  Insitute of Automation, Chinese Academy of Sciences
 *
 * Modification History:
 * 01a 2015/5/15 17:51:43
 *
 * =====================================================================================
 */
#include "UcosPrintf.h"
#include "MyPrintf.h"
#include <stdio.h>

static OS_MUTEX     gl_sUcosPrintf_Mutex;

/* 
 * ===  FUNCTION  ======================================================================
 *
 *         Name: UcosPrintfInit                                              
 *  Description: Because we need to use the MUTEX of UCOS-III, 
 *               we need to init the MUTEX UcosPrintf needs.        
 *
 * Modification History:                                                
 * 01a 2015/5/15 17:57:33   
 * 
 * =====================================================================================
 */
void UcosPrintfInit ( void )
{
    OS_ERR err;
    static unsigned char s_cMutexCreatedFlag = 0;

    /* To ensure we only init the MUTEX for one time. */
    if(0 == s_cMutexCreatedFlag)
    {
        OSMutexCreate(&gl_sUcosPrintf_Mutex,
                      "UcosPrintf_Mutex",
                      &err);
        s_cMutexCreatedFlag = 1;
    }
    else
    {
        // Do nothing.
    }
}		/* -----  end of function UcosPrintfInit  ----- */
/* 
 * ===  FUNCTION  ======================================================================
 *
 *         Name: UcosPrintf                                              
 *  Description: Use the MUTEX of UCOS-III to prevent the situation of 
 *               more than two process using the resource of the printf hardware.        
 *               NOTE:
 *                  This function is exactly the same as the MyPrintf, 
 *                  except for the using of MUTEX.
 *                  You ask me why I don't Call the MyPrintf function?
 *                  Because of the damn changable varialble list usage!=_=!
 *
 * Modification History:                                                
 * 01a 2015/5/15 18:02:32   
 * 
 * =====================================================================================
 */
void UcosPrintf ( char * fmt, ... )
{
    va_list ap;
    char *p, *sval;
    int ival;
    double dval;
    char buf[20];

    ///////////////////////////////////////////////////////////////////////////////////

    OS_ERR err;
    CPU_TS ts;
    OSMutexPend(&gl_sUcosPrintf_Mutex,
                UCOSPRINTF_MUTEX_WAIT_TIME_MS,  // If the process didn't get the MUTEX 
                                                // after 100 ms, we will abort waiting.
                OS_OPT_PEND_BLOCKING,
                &ts,
                &err);
    if(err != OS_ERR_NONE)
    {
        return;
    }
    ///////////////////////////////////////////////////////////////////////////////////

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

    ///////////////////////////////////////////////////////////////////////////////////
    OSMutexPost(&gl_sUcosPrintf_Mutex,
                OS_OPT_POST_NONE,
                &err);
}		/* -----  end of function UcosPrintf  ----- */
