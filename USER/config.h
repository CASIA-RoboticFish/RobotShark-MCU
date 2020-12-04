#ifndef __CONFIG_H
#define __CONFIG_H

// ��λ��ͨѶ��������
#define HOST_ID 0x11 // ��λ��
#define SLAVE_ID 0x22 // ���ص���

// �ֲ�ʽͨѶ��������
#define BOXFISH_NUM 5 
#define COMMUNICATE_LOOPTIME 200 // 100ms���������ͨѶ��һ��

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

// ����Ϊ�û���Ҫ�޸ĵĵط���Ҫ��Local ID��Ϊ��Ӧ��
#define LOCAL_ID BOXFISH_1
#define LOCAL_TIME_START BOXFISH_1_TIME_START // ����ID
#define LOCAL_TIME_STOP  BOXFISH_1_TIME_STOP  // ����ID

//#define LOCAL_ID BOXFISH_2
//#define LOCAL_TIME_START BOXFISH_2_TIME_START // ����ID
//#define LOCAL_TIME_STOP  BOXFISH_2_TIME_STOP  // ����ID

// data_storage_app�к�data_sendback_app�ж���Ҫ�õ��ĳ���
// �������ݵĳ���
#define DATA_SAVE_LENGTH 16 

#endif