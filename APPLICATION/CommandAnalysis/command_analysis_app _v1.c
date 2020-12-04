/*****************************************************************************
Copyright: CASIA 仿生机器鱼实验室
File name: 
Description: 
Author: VincentFEI
Version: 1.0.0
Date: 2019.07.03
History: 
*****************************************************************************/
#include "command_analysis_app.h"
#include "swim_control_app.h"

extern ROBOREMORA roboremora;


OS_TCB CommandAnalysisTCB;
__align(8) CPU_STK COMMAND_ANALYSIS_TASK_STK[COMMAND_ANALYSIS_STK_SIZE];


void command_execute(Command command, unsigned char* message)
{
	float var = 0.0f;
	float data = 0.0f;
	uint8_t rw_cmd = 0;
	uint8_t databuf[12];
	switch(command)
	{
		case SHAKING_HANDS:
			RFLink_sendStruct(FRIEND_ID1, command, databuf, 0);
			#ifdef PRINT_COMMAND_EN
				BuffPrintf("SHAKING_HANDS\n");
			#endif
			break;
		
		case SET_SWIM_RUN:
			swim_control_start();
			roboremora.swim_state = SWIM_RUN;
		
			#ifdef PRINT_COMMAND_EN
				BuffPrintf("SET_SWIM_RUN\n");
			#endif
			break;
			
		case SET_SWIM_STOP:
			swim_control_stop();
			roboremora.swim_state = SWIM_STOP;
		
			#ifdef PRINT_COMMAND_EN
				BuffPrintf("SET_SWIM_STOP\n");
			#endif
			break;
		
		case SET_SWIM_FORCESTOP:
			swim_control_forcestop();
			roboremora.swim_state = SWIM_FORCESTOP;
		
			#ifdef PRINT_COMMAND_EN
				BuffPrintf("SET_SWIM_FORCESTOP\n");
			#endif
			break;
		
		case SET_SWIM_SPEEDUP:
			var = roboremora.swim_param.cpg_amp + 0.05;
			set_tail_freq(var);
			roboremora.swim_param.cpg_freq = var;
		
			#ifdef PRINT_COMMAND_EN
				BuffPrintf("SET_SWIM_SPEEDUP\n");
			#endif
			break;
		
		case SET_SWIM_SPEEDDOWN:
			var = roboremora.swim_param.cpg_amp - 0.05;
			set_tail_freq(var);
			roboremora.swim_param.cpg_freq = var;
		
		  #ifdef PRINT_COMMAND_EN
				BuffPrintf("SET_SWIM_SPEEDDOWN\n");
			#endif
			break;
		
		case SET_SWIM_LEFT:
			set_tail_offset(-0.3);
			roboremora.swim_param.cpg_offset = -0.3;
		
			#ifdef PRINT_COMMAND_EN
				BuffPrintf("SET_SWIM_LEFT\n");
			#endif
		  break;
		
		case SET_SWIM_STRAIGHT:
			set_tail_offset(0);
			roboremora.swim_param.cpg_offset = 0;
		
			#ifdef PRINT_COMMAND_EN
				BuffPrintf("SET_SWIM_RIGHT\n");
			#endif
		  break;
		
		case SET_SWIM_RIGHT:
			set_tail_offset(0.3);
			roboremora.swim_param.cpg_offset = 0.3;
		
			#ifdef PRINT_COMMAND_EN
				BuffPrintf("SET_SWIM_RIGHT\n");
			#endif
		  break;
		
		case SET_SWIM_UP:
			set_pect_fin_angle(-0.6, 0);
			set_pect_fin_angle(0.6, 1);
			roboremora.swim_param.pecfin_angle[0] = -0.6;
			roboremora.swim_param.pecfin_angle[1] = 0.6;
			#ifdef PRINT_COMMAND_EN
				BuffPrintf("SET_SWIM_UP\n");
			#endif
			break;
		
		case SET_SWIM_DOWN:
			set_pect_fin_angle(0.6, 0);
			set_pect_fin_angle(-0.6, 1);
			roboremora.swim_param.pecfin_angle[0] = 0.6;
			roboremora.swim_param.pecfin_angle[1] = -0.6;
			#ifdef PRINT_COMMAND_EN
				BuffPrintf("SET_SWIM_DOWN\n");
			#endif
			break;
				
		case SET_CPG_AMP:
			data = atof(message);
			set_tail_amp(data);
			roboremora.swim_param.cpg_amp = data;
		
			#ifdef PRINT_COMMAND_EN
				BuffPrintf("SET_CPG_AMP\n");
			#endif
			break;
		
		case SET_CPG_FREQ:
			data = atof(message);
			set_tail_freq(data);
			roboremora.swim_param.cpg_freq = data;
		
			#ifdef PRINT_COMMAND_EN
				BuffPrintf("SET_CPG_FREQ\n");
			#endif
			break;
		
		case SET_CPG_OFFSET:
			data = atof(message);
			set_tail_offset(data);
			roboremora.swim_param.cpg_offset = data;
		
			#ifdef PRINT_COMMAND_EN
				BuffPrintf("SET_CPG_OFFSET\n");
			#endif
			break;
		
		case SET_LEFTPECFIN_UP:
			roboremora.swim_param.pecfin_angle[0] += 0.27;
			set_pect_fin_angle(roboremora.swim_param.pecfin_angle[0], 0);
			
			#ifdef PRINT_COMMAND_EN
				BuffPrintf("SET_LEFTPECFIN_UP\n");
			#endif
			break;
		
		case SET_LEFTPECFIN_ZERO:
			roboremora.swim_param.pecfin_angle[0] = 0.0f;
			set_pect_fin_angle(roboremora.swim_param.pecfin_angle[0], 0);
		
			#ifdef PRINT_COMMAND_EN
				BuffPrintf("SET_LEFTPECFIN_ZERO\n");
			#endif
			break;
		
		case SET_LEFTPECFIN_DOWN:
			roboremora.swim_param.pecfin_angle[0] -= 0.27;
			set_pect_fin_angle(roboremora.swim_param.pecfin_angle[0], 0);
		
			#ifdef PRINT_COMMAND_EN
				BuffPrintf("SET_LEFTPECFIN_DOWN\n");
			#endif
			break;
		
		case SET_RIGHTPECFIN_UP:
			roboremora.swim_param.pecfin_angle[1] -= 0.27;
			set_pect_fin_angle(roboremora.swim_param.pecfin_angle[1], 1);
		
			#ifdef PRINT_COMMAND_EN
				BuffPrintf("SET_RIGHTPECFIN_UP\n");
			#endif
			break;
		
		case SET_RIGHTPECFIN_ZERO:
			roboremora.swim_param.pecfin_angle[1] = 0.0f;
			set_pect_fin_angle(roboremora.swim_param.pecfin_angle[1], 1);
		
			#ifdef PRINT_COMMAND_EN
				BuffPrintf("SET_RIGHTPECFIN_ZERO\n");
			#endif
			break;
		
		case SET_RIGHTPECFIN_DOWN:
			roboremora.swim_param.pecfin_angle[1] += 0.27;
			set_pect_fin_angle(roboremora.swim_param.pecfin_angle[1], 1);
		
			#ifdef PRINT_COMMAND_EN
				BuffPrintf("SET_RIGHTPECFIN_DOWN\n");
			#endif
			break;
		
		case SET_VALVE1_ON:
			valve_on(VALVE1);
			roboremora.valve1_state = VALVE_ON;
		
			#ifdef PRINT_COMMAND_EN
				BuffPrintf("SET_VALVE1_ON\n");
			#endif
			break;
		
		case SET_VALVE1_OFF:
			valve_off(VALVE1);
			roboremora.valve1_state = VALVE_OFF;
		
			#ifdef PRINT_COMMAND_EN
				BuffPrintf("SET_VALVE1_OFF\n");
			#endif
			break;
		
		case SET_VALVE2_ON:
			valve_on(VALVE2);
			roboremora.valve2_state = VALVE_ON;
		
			#ifdef PRINT_COMMAND_EN
				BuffPrintf("SET_VALVE2_ON\n");
			#endif
			break;
		
		case SET_VALVE2_OFF:
			valve_off(VALVE2);
			roboremora.valve2_state = VALVE_OFF;
		
			#ifdef PRINT_COMMAND_EN
				BuffPrintf("SET_VALVE2_OFF\n");
			#endif
			break;
		
		case SET_PUMP_ON:
			pump_on();
			roboremora.pump_state = PUMP_ON;
		
			#ifdef PRINT_COMMAND_EN
				BuffPrintf("SET_PUMP_ON\n");
			#endif
			break;
		
		case SET_PUMP_OFF:
			pump_off();
			roboremora.pump_state = PUMP_OFF;
		
			#ifdef PRINT_COMMAND_EN
				BuffPrintf("SET_PUMP_OFF\n");
			#endif
			break;
		
		case SET_GIMBAL_RUN:
			roboremora.gimbal_state = GIMBAL_RUN;
			pantilt_control_resume();
			#ifdef PRINT_COMMAND_EN
				BuffPrintf("SET_GIMBAL_RUN\n");
			#endif
			break;
		
		case SET_GIMBAL_STOP:
			roboremora.gimbal_state = GIMBAL_STOP;
			pantilt_control_stop();
			#ifdef PRINT_COMMAND_EN
				BuffPrintf("SET_GIMBAL_STOP\n");
			#endif
			break;
		
		case SET_GIMBAL_ZERO:
			roboremora.gimbal_state = GIMBAL_ZERO;
			#ifdef PRINT_COMMAND_EN
				BuffPrintf("SET_GIMBAL_ZERO\n");
			#endif
			break;
		
		case SET_FLYWHEEL_RUN:
			roboremora.flywheel_state = FLYWHEEL_RUN;
		  RFLink_sendStruct(FRIEND_ID2, command, NULL, 0);
			#ifdef PRINT_COMMAND_EN
				BuffPrintf("SET_FLYWHEEL_RUN\n");
			#endif
			break;
		
		case SET_FLYWHEEL_STOP:
			roboremora.flywheel_state = FLYWHEEL_STOP;
		  RFLink_sendStruct(FRIEND_ID2, command, NULL, 0);
			#ifdef PRINT_COMMAND_EN
				BuffPrintf("SET_FLYWHEEL_STOP\n");
			#endif
			break;
		
		case SET_FLYWHEEL_CMD:
			rw_cmd = message[0];
			RFLink_sendStruct(FRIEND_ID2, command, message, 1);
			#ifdef PRINT_COMMAND_EN
				BuffPrintf("SET_FLYWHEEL_CMD\n");
			#endif
			break;
		
		case SET_DATASHOW_OVER:
			data_show_app_stop();
		
			#ifdef PRINT_COMMAND_EN
				BuffPrintf("SET_READDATA_OVER\n");
			#endif
			break;
		
		case READ_ROBOT_STATUS:
			uint8_t rmstatus = 0x00;
			rmstatus = (uint8_t)(((roboremora.swim_state & 0x03)<<6) + \
								 ((roboremora.valve1_state & 0x01)<<5) + \
								 ((roboremora.valve2_state & 0x01)<<4) + \
								 ((roboremora.pump_state & 0x01)<<3) + \
			           ((roboremora.gimbal_state & 0x03)<<1) + \
			           ((roboremora.flywheel_state & 0x01)));
			memcpy(databuf, &rmstatus, 1);
			RFLink_sendStruct(FRIEND_ID1, command, databuf, 1);
		  
			#ifdef PRINT_COMMAND_EN
				BuffPrintf("READ_ROBOT_STATUS\n");
			#endif
			break;
		
		case READ_CPG_PARAM:
			memcpy(databuf,   &(roboremora.swim_param.cpg_amp),    4);
			memcpy(databuf+4, &(roboremora.swim_param.cpg_freq),   4);
			memcpy(databuf+8, &(roboremora.swim_param.cpg_offset), 4);
			RFLink_sendStruct(FRIEND_ID1,command, databuf, 12);
		  
			#ifdef PRINT_COMMAND_EN
				BuffPrintf("READ_CPG_PARAM\n");
			#endif
			break;
		
		case READ_IMU1_ATTITUDE:
			data = atof(message);
			set_data_type(command,data);
			data_show_app_resume();
		
			#ifdef PRINT_COMMAND_EN
				BuffPrintf("READ_IMU1_ATTITUDE\n");
			#endif
			break;
		
		case READ_IMU1_ACCEL:
			data = atof(message);
			set_data_type(command,data);
			data_show_app_resume();
		
			#ifdef PRINT_COMMAND_EN
				BuffPrintf("READ_IMU1_ACCEL\n");
			#endif
			break;
		
		case READ_IMU1_GYRO:
			data = atof(message);
			set_data_type(command,data);
			data_show_app_resume();
		
			#ifdef PRINT_COMMAND_EN
				BuffPrintf("READ_IMU1_GYRO\n");
			#endif
			break;
		
		case READ_IMU2_ATTITUDE:
			data = atof(message);
			set_data_type(command,data);
			data_show_app_resume();
		
			#ifdef PRINT_COMMAND_EN
				BuffPrintf("READ_IMU2_ATTITUDE\n");
			#endif
			break;
		
		case READ_IMU2_ACCEL:
			data = atof(message);
			set_data_type(command,data);
			data_show_app_resume();
		
			#ifdef PRINT_COMMAND_EN
				BuffPrintf("READ_IMU2_ACCEL\n");
			#endif
			break;
		
		case READ_IMU2_GYRO:
			data = atof(message);
			set_data_type(command,data);
			data_show_app_resume();
		
			#ifdef PRINT_COMMAND_EN
				BuffPrintf("READ_IMU2_GYRO\n");
			#endif
			break;
		
		case READ_VARISTOR1_VAL:
			data = atof(message);
			set_data_type(command,data);
			data_show_app_resume();
		
			#ifdef PRINT_COMMAND_EN
				BuffPrintf("READ_VARISTOR1_VAL\n");
			#endif
			break;
		
		case READ_VARISTOR2_VAL:
			data = atof(message);
			set_data_type(command,data);
			data_show_app_resume();
		
			#ifdef PRINT_COMMAND_EN
				BuffPrintf("READ_VARISTOR2_VAL\n");
			#endif
			break;
		
		case READ_GIMBAL1_ANGLE:
			data = atof(message);
			set_data_type(command,data);
			data_show_app_resume();
		
			#ifdef PRINT_COMMAND_EN
				BuffPrintf("READ_GIMBAL1_ANGLE\n");
			#endif
			break;
		
		case READ_GIMBAL2_ANGLE:
			data = atof(message);
			set_data_type(command,data);
			data_show_app_resume();
		
			#ifdef PRINT_COMMAND_EN
				BuffPrintf("READ_GIMBAL2_ANGLE\n");
			#endif
			break;
		
		case READ_FLYWHEEL_ANGLE:
			data = atof(message);
			set_data_type(command,data);
			data_show_app_resume();
		
			#ifdef PRINT_COMMAND_EN
				BuffPrintf("READ_FLYWHEEL_ANGLE\n");
			#endif
			break;
		
		case READ_FLYWHEEL_VEL:
			data = atof(message);
			set_data_type(command,data);
			data_show_app_resume();
		
			#ifdef PRINT_COMMAND_EN
				BuffPrintf("READ_FLYWHEEL_VEL\n");
			#endif
			break;
		
		case READ_FILE_LIST:
			BuffPrintf("READ_FILE_LIST\n");
		  mf_scan_files("0:");
			#ifdef PRINT_COMMAND_EN
				BuffPrintf("READ_FILE_LIST\n");
			#endif
			break;
		
		case GOTO_ATTACH:
			go_to_attach();
			#ifdef PRINT_COMMAND_EN
				BuffPrintf("GOTO_ATTACH\n");
			#endif
			break;
		
		case GOTO_DETACH:
			go_to_detach();
			#ifdef PRINT_COMMAND_EN
				BuffPrintf("GOTO_DETACH\n");
			#endif
			break;
		
		case GOTO_STORAGE_DATA:
			if(message[0] == 0x01) // 如果message里面没有带文件名字，那么就是储存基础数据
				set_savefile_name(1, "data.bin");
			else if(message[0] == 0x02) // 如果message里面带了文件名字，那么就是储存训练数据
				//set_savefile_name(2, message+1);
				set_savefile_name(2, "train.bin");
			RFLink_sendStruct(FRIEND_ID1,command, NULL, 0);
			data_storage_app_resume();
			#ifdef PRINT_COMMAND_EN
				BuffPrintf("GOTO_STORAGE_DATA\n");
			#endif
			break;
		
		case GOTO_SEND_DATA:
			
			if(message[0] == 0x01) // 如果message里面没有带文件名字，那么就是储存基础数据
			{
				databuf[0] = 0x01;
				RFLink_sendStruct(FRIEND_ID1, command, databuf, 1);
				delay_ms(1000);
				set_sendfile_name(1, "data.bin");
				go_to_sendback(1);
			}
			else if(message[0] == 0x02) // 如果message里面带了文件名字，那么就是储存训练数据
			{	
				databuf[0] = 0x02;
				RFLink_sendStruct(FRIEND_ID1, command, databuf, 1);
				delay_ms(1000);
				//set_sendfile_name(2, message+1);
				set_sendfile_name(2, "train.bin");
				go_to_sendback(2);
			}
			#ifdef PRINT_COMMAND_EN
				BuffPrintf("SEND_DATA\n");
			#endif
			break;
		
		case LAST_COMMAND_FLAG:
			tail_servo_set_angle(1,TAIL_SERVO1);
			#ifdef PRINT_COMMAND_EN
				BuffPrintf("LAST_COMMAND_FLAG\n");
			#endif
			break;
		
		default:
			break;
	}
}


static void command_analysis_app_task(void * p_arg)
{
	OS_ERR err;
	CPU_TS ts;
	OS_MSG_SIZE size;
	uint8_t rx_data;
	uint8_t ret;
	AnalysisResult result;
	while(1)
	{

		rx_data = (uint8_t)OSTaskQPend(0,
																  OS_OPT_PEND_BLOCKING,
																	&size,
																	&ts,
																	&err);
		
		if(err != OS_ERR_NONE)
    {
			continue;
    }
		
		if(RFLink_byteAnalysisCall(FRIEND_ID1, rx_data))
		{
			//BuffPrintf("Package analysised\n");
			result =  RFLink_commandAnalysis(FRIEND_ID1);
			command_execute(result.command, result.message);
		}
		
	}
}


void command_analysis_app_init ( void )
{
	CPU_SR_ALLOC();

	OS_ERR err;

	OS_CRITICAL_ENTER();

	OSTaskCreate(&CommandAnalysisTCB,
							 "Command Analysis App",
							 command_analysis_app_task,
							 0,
							 COMMAND_ANALYSIS_TASK_PRIO,
							 &COMMAND_ANALYSIS_TASK_STK[0],
							 COMMAND_ANALYSIS_STK_SIZE/10,
							 COMMAND_ANALYSIS_STK_SIZE,
							 COMMAND_ANALYSIS_QUEUE_SIZE,
							 0,
							 0,
							 OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
							 &err);


	OS_CRITICAL_EXIT();
} 



