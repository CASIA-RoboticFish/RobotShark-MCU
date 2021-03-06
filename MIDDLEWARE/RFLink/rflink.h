/*****************************************************************************
Copyright: CASIA 仿生机器鱼实验室
File name: 
Description: 
Author: VincentFEI
Version: 1.0.0
Date: 2019.07.03
History: 
*****************************************************************************/
#ifndef __RFLINK_H
#define __RFLINK_H
#include "stm32f4xx.h"
#include "includes.h"
#include "string.h"
#include "config.h"

#define MY_ID     LOCAL_ID
#define FRIEND_ID1 HOST_ID // 上位机
#define FRIEND_ID2 SLAVE_ID // 飞轮

#define USART2_ID 0
#define USART6_ID 1
#define USART1_ID 2

typedef struct RFMessage
{
	unsigned char sender_id;
	unsigned char receiver_id;
	unsigned char length;
	unsigned char data[120];
}RFMessage;

typedef enum Recstate
{
	WAITING_FF=0,
	SENDER_ID,
	RECEIVER_ID,
	RECEIVE_LEN,
	RECEIVE_PACKAGE,
	RECEIVE_CHECK
}Recstate;

typedef enum Command
{
	SHAKING_HANDS=1,
	SYNCHRONIZE_CLOCK,
	SET_SWIM_RUN,
	SET_SWIM_STOP,
	SET_SWIM_FORCESTOP,
	SET_SWIM_SPEEDUP,
	SET_SWIM_SPEEDDOWN,
	SET_SWIM_LEFT,
	SET_SWIM_RIGHT,
	SET_SWIM_STRAIGHT,
	SET_SWIM_UP,
	SET_SWIM_DOWN,
	SET_CPG_AMP,
	SET_CPG_FREQ,
	SET_CPG_OFFSET,
	SET_SINE_MOTION_AMP,
	SET_SINE_MOTION_FREQ,
	SET_SINE_MOTION_OFFSET,
	SET_PECFIN_UP,
	SET_PECFIN_ZERO,
	SET_PECFIN_DOWN,
	SET_LEFTPECFIN_UP,
	SET_LEFTPECFIN_ZERO,
	SET_LEFTPECFIN_DOWN,
	SET_RIGHTPECFIN_UP,
	SET_RIGHTPECFIN_ZERO,
	SET_RIGHTPECFIN_DOWN,
	SET_VALVE1_ON,
	SET_VALVE1_OFF,
	SET_VALVE2_ON,
	SET_VALVE2_OFF,
	SET_PUMP_ON,
	SET_PUMP_OFF,
	SET_PUMP_IN_ON,
	SET_PUMP_IN_OFF,
	SET_PUMP_OUT_ON,
	SET_PUMP_OUT_OFF,
	SET_GIMBAL_RUN,
	SET_GIMBAL_STOP,
	SET_GIMBAL_ZERO,
	SET_FLYWHEEL_RUN,
	SET_FLYWHEEL_STOP,
	SET_FLYWHEEL_DATA,
	SET_FLYWHEEL_CMD,
	SET_TARGET_POS,
	SET_DATASHOW_OVER,
	SET_DEPTHCTL_START,
	SET_DEPTHCTL_STOP,
	SET_DEPTHCTL_PARAM,
	SET_AUTOCTL_RUN,
	SET_AUTOCTL_STOP,
	READ_ROBOT_STATUS,
	READ_CPG_PARAM,
	READ_SINE_MOTION_PARAM,
	READ_IMU1_ATTITUDE,
	READ_IMU1_ACCEL,
	READ_IMU1_GYRO,
	READ_IMU2_ATTITUDE,
	READ_IMU2_ACCEL,
	READ_IMU2_GYRO,
	READ_VARISTOR1_VAL,
	READ_VARISTOR2_VAL,
	READ_GIMBAL1_ANGLE,
	READ_GIMBAL2_ANGLE,
	READ_FLYWHEEL_ANGLE,
	READ_FLYWHEEL_VEL,
	READ_DEPTH,
	READ_FILE_LIST,
	GOTO_ATTACH,
	GOTO_DETACH,
	GOTO_STORAGE_DATA,
	GOTO_STOP_STORAGE,
	GOTO_SEND_DATA,
	PRINT_SYS_MSG,
	LAST_COMMAND_FLAG
}Command;

typedef struct AnalysisResult
{
	Command command;
	unsigned char message[32];
	uint8_t message_len;
}AnalysisResult;

uint8_t RFLink_byteAnalysisCall(uint8_t usart_id, uint8_t rx_data);
uint8_t RFLink_receiveStates(uint8_t usart_id, uint8_t rx_data);
AnalysisResult RFLink_commandAnalysis(uint8_t usart_id);

void RFLink_sendStruct(uint8_t receiver_id, const Command command_type , unsigned char* p , uint8_t len);
void RFLink_message(char * msg, ...);

#endif