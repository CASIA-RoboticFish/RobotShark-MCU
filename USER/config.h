#ifndef __CONFIG_H
#define __CONFIG_H

// 上位机通讯参数设置
#define HOST_ID 0x11 // 上位机
#define SLAVE_ID 0x22 // 机载电脑

// 分布式通讯参数设置
#define BOXFISH_NUM 5 
#define COMMUNICATE_LOOPTIME 200 // 100ms，所有鱼的通讯轮一次

typedef enum BOXFISHID
{
	BOXFISH_ALL=0x00,
	BOXFISH_1,
	BOXFISH_2,
	BOXFISH_3,
	BOXFISH_4,
	BOXFISH_5,
}BOXFISHID;

#define BOXFISH_1_TIME_START 0
#define BOXFISH_1_TIME_STOP  20

#define BOXFISH_2_TIME_START 40
#define BOXFISH_2_TIME_STOP  60

#define BOXFISH_3_TIME_START 80
#define BOXFISH_3_TIME_STOP  100

#define BOXFISH_4_TIME_START 120
#define BOXFISH_4_TIME_STOP  140

#define BOXFISH_5_TIME_START 160
#define BOXFISH_5_TIME_STOP  180

// 以下为用户需要修改的地方，要把Local ID改为对应的
#define LOCAL_ID BOXFISH_1
#define LOCAL_TIME_START BOXFISH_1_TIME_START // 本机ID
#define LOCAL_TIME_STOP  BOXFISH_1_TIME_STOP  // 本机ID

//#define LOCAL_ID BOXFISH_2
//#define LOCAL_TIME_START BOXFISH_2_TIME_START // 本机ID
//#define LOCAL_TIME_STOP  BOXFISH_2_TIME_STOP  // 本机ID

// data_storage_app中和data_sendback_app中都需要用到的常数
// 储存数据的长度
#define DATA_SAVE_LENGTH 16 

#endif