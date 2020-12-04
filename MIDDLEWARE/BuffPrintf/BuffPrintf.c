/*
 * =====================================================================================
 *
 *       Filename:  BuffPrintf.c
 *
 *    Description:  The Printf function use the Buff to minimun the time the process
 *                  that need to print something cost, because generally speaking,
 *                  the printf function in the embeded system needs to wait for the
 *                  data transfering.
 *
 *        Version:  1.0
 *        Created:  2015/5/17 11:54:16
 *       Revision:  none
 *       Compiler:  Keil ARM-MDK 4.60
 *
 *         Author:  Yang Xiang (Shawn), yangxiang92@163.com
 *        Company:  Insitute of Automation, Chinese Academy of Sciences
 *
 * Modification History:
 * 01a 2015/5/17 11:54:16
 *
 * =====================================================================================
 */
#include "BuffPrintf.h"
#include "stdio.h"

static OS_MEM s_sBuffPrintfPartition;
static char s_caBuffPrintfPartitionStorage[BUFFPRINTF_MAX_QUEUE_NUM][BUFFPRINTF_QUEUE_UNIT_SIZE];
static OS_SEM s_sBuffPrintfSem;
static OS_Q   s_sBuffPrintfQ;

#ifdef BUFFPRINTF_CFG_REPORT_DATA_MISS
static unsigned char s_ucDataMissFlag = 0;
#endif

static OS_TCB s_sBuffPrintfTaskTCB;
__align(8) CPU_STK  BUFFPRINTF_TASK_STK[BUFFPRINTF_STK_SIZE];

/*-----------------------------------------------------------------------------
 *  >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
 *  SELF-DEFINE REGION
 *-----------------------------------------------------------------------------*/
#include "MyPrintf.h"
/*
 * ===  FUNCTION  ======================================================================
 *
 *         Name: BuffPrintf_PutString
 *  Description: The Function the BuffPrintf actually send a string
 *               (in the hardware level)
 *
 * Modification History:
 * 01a 2015/5/17 14:29:52
 *
 * =====================================================================================
 */
static void BuffPrintf_PutString ( char * p_str )
{
    MyPrintf_PutString(p_str);
}       /* -----  end of function BuffPrintf_PutString  ----- */
/*-----------------------------------------------------------------------------
 *  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 *-----------------------------------------------------------------------------*/

/*
 * ===  FUNCTION  ======================================================================
 *
 *         Name: BuffPrintf_Task
 *  Description: The task that the BuffPrintf actually send data
 *
 * Modification History:
 * 01a 2015/5/17 13:23:58
 *
 * =====================================================================================
 */
void BuffPrintf_Task ( void * p_arg )
{
    char * p_str = NULL;
    OS_MSG_SIZE ms_size = 0;
    CPU_TS ts = 0;
    OS_ERR err;

    while(1)
    {
				// �ȴ���Ϣ�����д�������ӡ���ݵ�ָ��
        p_str = (char *)OSQPend(&s_sBuffPrintfQ,
                                0, // û���յ���Ϣ�Ļ��������޵ĵȴ���ȥ
                                OS_OPT_PEND_BLOCKING,
                                &ms_size,
                                &ts,
                                &err);

        if(err == OS_ERR_NONE)
        {
            BuffPrintf_PutString(p_str);
						// �ͷŴ洢��
            OSMemPut(&s_sBuffPrintfPartition,
                     (void *)p_str,
                     &err);
            p_str = NULL;
						// �ͷ��ź������ź���+1,˵����ʹ�õĴ洢������+1
            OSSemPost(&s_sBuffPrintfSem,
                      OS_OPT_POST_1,
                      &err);
        }
        else
        {

        }
    }
}       /* -----  end of function BuffPrintf_Task  ----- */
/*
 * ===  FUNCTION  ======================================================================
 *
 *         Name: BuffPrintf_Init
 *  Description: Init the BuffPrintf. This includes the initialization of
 *               the Memory Partion, the Task, the Sem and the Message Queue
 *               the Buff Printf needs.
 *               You had better call this function in a rather stable enviroment, which
 *               means you might have to run it in the CRITICAL RIGION.
 *               And also, you have to call this function after the uCOS-III has been
 *               initialize.
 *
 * Modification History:
 * 01a 2015/5/17 12:01:38
 *
 * =====================================================================================
 */
void BuffPrintf_Init ( void )
{
    static unsigned char s_ucInitedFlag = 0;
    OS_ERR err;
    CPU_SR_ALLOC();

    if(s_ucInitedFlag == 1)
    {
        return;
    }

    OS_CRITICAL_ENTER();
    OSSemCreate(&s_sBuffPrintfSem,
                "BuffPrintf_Sem",
                BUFFPRINTF_MAX_QUEUE_NUM, //���ﲻΪ1��˵������һ���������ź������ź�������Ŀ�ʹ洢��ĸ���Ҳ��һ����
                &err);

    OSQCreate(&s_sBuffPrintfQ, //��Ϣ����
              "BuffPrintf_Q",
              BUFFPRINTF_MAX_QUEUE_NUM, //��Ϣ���еĳ��Ⱥʹ洢��ĸ�����һ����
              &err);

    OSMemCreate(&s_sBuffPrintfPartition,
                "BuffPrintf_Partition",
                &s_caBuffPrintfPartitionStorage[0][0],
                BUFFPRINTF_MAX_QUEUE_NUM, // �洢��ĸ���
                BUFFPRINTF_QUEUE_UNIT_SIZE, // �洢��Ĵ�С
                &err);

    OSTaskCreate(&s_sBuffPrintfTaskTCB,
                 "BuffPrintf task",
                 BuffPrintf_Task,
                 0,
                 BUFFPRINTF_TASK_PRIO,
                 &BUFFPRINTF_TASK_STK[0],
                 BUFFPRINTF_STK_SIZE/10,
                 BUFFPRINTF_STK_SIZE,
                 0,
                 0,
                 0,
                 OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 &err);
    OS_CRITICAL_EXIT();

    s_ucInitedFlag = 1;
}       /* -----  end of function BuffPrintf_Init  ----- */

// ��ͣ����
void BuffPrintf_Stop(void)
{
	CPU_SR_ALLOC();

	OS_ERR err;

	OS_CRITICAL_ENTER();

	OSTaskSuspend(&s_sBuffPrintfTaskTCB,
								&err);

	OS_CRITICAL_EXIT();
}

// �ָ�����
void BuffPrintf_Resume(void)
{
	CPU_SR_ALLOC();

	OS_ERR err;

	OS_CRITICAL_ENTER();

	OSTaskResume(&s_sBuffPrintfTaskTCB,
						 &err);

	OS_CRITICAL_EXIT();
}


/*
 * ===  FUNCTION  ======================================================================
 *
 *         Name: BuffPrintf
 *  Description: Use the Queue and Sem in the Ucos to implement the printf function.
 *               I write this function to reduce the time the function call printf,
 *               I put the string it need to print to a queue, and send it when the
 *               CPU is free, which can help to reduce the time the process need.
 *               RETURN VALUE:
 *                  0: error.
 *                  1: no error.
 *
 * Modification History:
 * 01a 2015/5/17 14:38:58
 *
 * =====================================================================================
 */
unsigned char BuffPrintf ( char * fmt, ... )
{
    va_list ap;
    char *p, *sval;
    int ival;
    double fval;
    char buf[30];

    ///////////////////////////////////////////////////////////////////////////////

    char * str_buf = NULL;
    char * str_buf_p = NULL;
    unsigned char i = 0;
    OS_ERR err;
    CPU_TS ts;
		// ����һ���ź����������Ժ��ź������1��˵������ź�����ֵ�Ǻ�ʣ��Ĵ洢��������ȵ�
    OSSemPend(&s_sBuffPrintfSem,
              0,
              OS_OPT_PEND_NON_BLOCKING,
              &ts,
              &err);
    if(err != OS_ERR_NONE)
    {
#ifdef BUFFPRINTF_CFG_REPORT_DATA_MISS
        s_ucDataMissFlag = 1;
#endif
        return 1;
    }
		// ����һ��洢��
    str_buf = (char *)OSMemGet(&s_sBuffPrintfPartition,
                               &err);
    str_buf_p = str_buf;
    if(err != OS_ERR_NONE)
    {
        return 1;
    }

    ///////////////////////////////////////////////////////////////////////////////

#ifdef BUFFPRINTF_CFG_REPORT_DATA_MISS
    if(s_ucDataMissFlag == 1)
    {
        *str_buf_p = '*';
        str_buf_p ++;
        s_ucDataMissFlag = 0;
    }
#endif

    va_start(ap, fmt);
    for (p = fmt; *p; p++)
    {
        if(*p != '%')
        {
            *str_buf_p = *p;
            str_buf_p ++;
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
            }
            break;
        case 'f':
            fval = va_arg(ap, double);
            sprintf(buf, "%f", fval);
            for(i = 0; buf[i] != '\0'; i++)
            {
                *str_buf_p = buf[i];
                str_buf_p ++;
            }
            break;
        case 's':
            sval = va_arg(ap, char *);
            while(*sval != '\0')
            {
                *str_buf_p = *sval;
                str_buf_p ++;
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

		// '\0'�����Ϊ��ֹ����������⣬���Ի���0xEF
    *str_buf_p = 0xEF;
		
		// ��Ҫ��ӡ�����ݣ�����洢���ڣ�Ȼ����һ����Ϣ���Ѵ洢��ĵ�ַ���������ӡ������
    OSQPost(&s_sBuffPrintfQ,
            str_buf,
            BUFFPRINTF_QUEUE_UNIT_SIZE,
            OS_OPT_POST_FIFO,
            &err);

    return 0;
}       /* -----  end of function BuffPrintf  ----- */


unsigned char RFLinkPrintf ( const RFMessage* tx_message )
{
		uint32_t check_sum = 0;
	
		char * str_buf = NULL;
		char * str_buf_p = NULL;
	
		OS_ERR err;
    CPU_TS ts;
		// ����һ���ź����������Ժ��ź������1��˵������ź�����ֵ�Ǻ�ʣ��Ĵ洢��������ȵ�
    OSSemPend(&s_sBuffPrintfSem,
              0,
              OS_OPT_PEND_NON_BLOCKING,
              &ts,
              &err);
		if(err != OS_ERR_NONE)
    {
#ifdef BUFFPRINTF_CFG_REPORT_DATA_MISS
        s_ucDataMissFlag = 1;
#endif
        return 1;
    }	
		
		
		// ����һ��洢��
    str_buf = (char *)OSMemGet(&s_sBuffPrintfPartition,
                               &err);
    str_buf_p = str_buf;
    if(err != OS_ERR_NONE)
    {
        return 1;
    }
		
		
		// �ֽ�1
		*str_buf_p = 0xff;
		check_sum += 0xff;
		str_buf_p++;
		
		// �ֽ�2
		*str_buf_p = tx_message->sender_id;
		check_sum += *str_buf_p;
		str_buf_p++;
		
		// �ֽ�3
		*str_buf_p = tx_message->receiver_id;
		check_sum += *str_buf_p;
		str_buf_p++;
				
		// �ֽ�4
		*str_buf_p = (unsigned char)tx_message->length;  
		check_sum += *str_buf_p;
		str_buf_p++;
		
		// �ֽ�5~n
		unsigned short int tx_i  = 0;
    for(tx_i = 0;  tx_i < tx_message->length+1; tx_i++)   //package
    {
				*(str_buf_p+tx_i) = tx_message->data[tx_i];
				check_sum += *(str_buf_p+tx_i);
    }
		str_buf_p += tx_i;
		
		// �ֽ�n+1
		check_sum = check_sum % 255;
    *str_buf_p = check_sum;
		str_buf_p++;
		
		// ������
		// '\0'�����Ϊ��ֹ����������⣬���Ի���0xEF
    *str_buf_p = 0xEF;
		
		
		// ��Ҫ��ӡ�����ݣ�����洢���ڣ�Ȼ����һ����Ϣ���Ѵ洢��ĵ�ַ���������ӡ������
    OSQPost(&s_sBuffPrintfQ,
            str_buf,
            BUFFPRINTF_QUEUE_UNIT_SIZE,
            OS_OPT_POST_FIFO,
            &err);

    return 0;
}


