#ifndef _DISTRIBUTED_COMMUNICATE_APP_H_
#define _DISTRIBUTED_COMMUNICATE_APP_H_

#include "includes.h"
#include "os_cfg_app.h"

#include "rflink.h"
#include "config.h"
#include "robotstate.h"

#define DISTCOM_CFG_REPORT_DATA_MISS

#define DISTCOM_MAX_QUEUE_NUM    25
#define DISTCOM_QUEUE_UNIT_SIZE  128

#define DISTCOM_TASK_PRIO        4
#define DISTCOM_STK_SIZE         512

void distributed_communicate_app_init ( void );
unsigned char DistComPrintf ( const RFMessage* tx_message );
void distributed_communicate_app_stop ( void );
void distributed_communicate_app_resume ( void );
#endif