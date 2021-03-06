
#include "distributed_communicate_app.h"
#include "stdio.h"

static OS_MEM s_sDistComPartition;
static char s_caDistComPartitionStorage[DISTCOM_MAX_QUEUE_NUM][DISTCOM_QUEUE_UNIT_SIZE];
static OS_SEM s_sDistComSem;
static OS_Q   s_sDistComQ;

#ifdef DISTCOM_CFG_REPORT_DATA_MISS
static unsigned char s_ucDataMissFlag = 0;
#endif

static OS_TCB DistComTaskTCB;
__align(8) CPU_STK  DISTCOM_TASK_STK[DISTCOM_STK_SIZE];


// 
extern ROBOSHARK robosharkstate;

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
static void DistCom_PutString ( char * p_str )
{
    MyPrintf_PutString_Usart1(p_str);
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
void distributed_communicate_app_task ( void * p_arg )
{
    char * p_str = NULL;
    OS_MSG_SIZE ms_size = 0;
    CPU_TS ts = 0;
    OS_ERR err;
    OS_TICK now_os_tick;
    int commun_time;
	
    while(1)
    {
	// �ȴ���Ϣ�����д�������ӡ���ݵ�ָ��
	p_str = (char *)OSQPend(&s_sDistComQ,
				0, // û���յ���Ϣ�Ļ��������޵ĵȴ���ȥ
				OS_OPT_PEND_BLOCKING,
				&ms_size,
				&ts,
				&err);
		
	now_os_tick = OSTimeGet(&err);
	//float ttt = (float)(now_os_tick - robosharkstate.communicate_basetime_tick)*1000/(float)(OS_CFG_TICK_RATE_HZ);
	commun_time = (int)((float)(now_os_tick - robosharkstate.communicate_basetime_tick)*1000/(float)(OS_CFG_TICK_RATE_HZ))%COMMUNICATE_LOOPTIME;
	// ����BOXFISH_1����ʼ���͵�ʱ����0ms��������10ms�����commun_time�����ʱ���ڣ���ô�Ϳ��Է���
	// �������,����commun_time��25����ô�Ͳ��ܷ��ͣ�Ҫ�ȴ���һ�λ���
	//int id = LOCAL_TIME_START;
	if(commun_time >= LOCAL_TIME_START && commun_time < LOCAL_TIME_STOP)
	{
		
		if(err == OS_ERR_NONE)
		{
		    DistCom_PutString(p_str);
							// �ͷŴ洢��
		    OSMemPut(&s_sDistComPartition,
			     (void *)p_str,
			     &err);
		    p_str = NULL;
							// �ͷ��ź������ź���+1,˵����ʹ�õĴ洢������+1
		    OSSemPost(&s_sDistComSem,
			      OS_OPT_POST_1,
			      &err);
		}
		else
		{

		}
		
		int inter = OSTimeGet(&err)-now_os_tick;
		now_os_tick = OSTimeGet(&err);
		int a = 1;
	}
	else
	{
	    // ����commun_time��25����ô�Ͳ��ܷ��ͣ�Ҫ�ȴ���һ�λ���
	    // �ȴ���ʱ���ǣ�100-25+0=75����
	    OSTimeDlyHMSM(0,0,0,(COMMUNICATE_LOOPTIME-commun_time+LOCAL_TIME_START),OS_OPT_TIME_HMSM_STRICT,&err);
	    OSQPost(&s_sDistComQ,
		    p_str,
		    DISTCOM_QUEUE_UNIT_SIZE,
		    OS_OPT_POST_FIFO,
		    &err);
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
void distributed_communicate_app_init ( void )
{
    static unsigned char s_ucInitedFlag = 0;
    OS_ERR err;
    CPU_SR_ALLOC();

    if(s_ucInitedFlag == 1)
    {
        return;
    }

    OS_CRITICAL_ENTER();
    OSSemCreate(&s_sDistComSem,
                "DistCom_Sem",
                DISTCOM_MAX_QUEUE_NUM, //���ﲻΪ1��˵������һ���������ź������ź�������Ŀ�ʹ洢��ĸ���Ҳ��һ����
                &err);

    OSQCreate(&s_sDistComQ, //��Ϣ����
              "DistCom_Q",
              DISTCOM_MAX_QUEUE_NUM, //��Ϣ���еĳ��Ⱥʹ洢��ĸ�����һ����
              &err);

    OSMemCreate(&s_sDistComPartition,
                "DistCom_Partition",
                &s_caDistComPartitionStorage[0][0],
                DISTCOM_MAX_QUEUE_NUM, // �洢��ĸ���
                DISTCOM_QUEUE_UNIT_SIZE, // �洢��Ĵ�С
                &err);

    OSTaskCreate(&DistComTaskTCB,
                 "Distributed communicate task",
                 distributed_communicate_app_task,
                 0,
                 DISTCOM_TASK_PRIO,
                 &DISTCOM_TASK_STK[0],
                 DISTCOM_STK_SIZE/10,
                 DISTCOM_STK_SIZE,
                 0,
                 0,
                 0,
                 OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 &err);
    OS_CRITICAL_EXIT();

    s_ucInitedFlag = 1;
}       /* -----  end of function BuffPrintf_Init  ----- */

// ��ͣ����
void distributed_communicate_app_stop(void)
{
	CPU_SR_ALLOC();

	OS_ERR err;

	OS_CRITICAL_ENTER();

	OSTaskSuspend(&DistComTaskTCB, &err);

	OS_CRITICAL_EXIT();
}

// �ָ�����
void distributed_communicate_app_resume(void)
{
	CPU_SR_ALLOC();

	OS_ERR err;

	OS_CRITICAL_ENTER();

	OSTaskResume(&DistComTaskTCB, &err);

	OS_CRITICAL_EXIT();
}


//

unsigned char DistComPrintf ( const RFMessage* tx_message )
{
	uint32_t check_sum = 0;

	char * str_buf = NULL;
	char * str_buf_p = NULL;

	OS_ERR err;
	CPU_TS ts;
	
	// ����һ���ź����������Ժ��ź������1��˵������ź�����ֵ�Ǻ�ʣ��Ĵ洢��������ȵ�
	OSSemPend(&s_sDistComSem,
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
	str_buf = (char *)OSMemGet(&s_sDistComPartition,
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
	OSQPost(&s_sDistComQ,
	    str_buf,
	    DISTCOM_QUEUE_UNIT_SIZE,
	    OS_OPT_POST_FIFO,
	    &err);

	return 0;
}


