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

extern ROBOSHARK robosharkstate;


OS_TCB CommandAnalysisTCB;
__align(8) CPU_STK COMMAND_ANALYSIS_TASK_STK[COMMAND_ANALYSIS_STK_SIZE];


void command_execute(Command command, unsigned char* message, uint8_t message_len)
{
	OS_ERR err;
	
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
		
		case SYNCHRONIZE_CLOCK:
			robosharkstate.communicate_basetime_tick = OSTimeGet(&err); // 同步时间
			RFLink_sendStruct(BOXFISH_5, SYNCHRONIZE_CLOCK , databuf , 0);
			#ifdef PRINT_COMMAND_EN
				BuffPrintf("SYNCHRONIZE_CLOCK\n");
			#endif
			break;
		
		case SET_SWIM_RUN:
			swim_control_start();
			// robosharkstate.swim_state = SWIM_RUN;
			#ifdef PRINT_COMMAND_EN
				BuffPrintf("SET_SWIM_RUN\n");
			#endif
			break;
			
		case SET_SWIM_STOP:
			swim_control_stop();
			// robosharkstate.swim_state = SWIM_STOP;
			#ifdef PRINT_COMMAND_EN
				BuffPrintf("SET_SWIM_STOP\n");
			#endif
			break;
		
		case SET_SWIM_FORCESTOP:
			swim_control_forcestop();
			// robosharkstate.swim_state = SWIM_FORCESTOP;
		
			#ifdef PRINT_COMMAND_EN
				BuffPrintf("SET_SWIM_FORCESTOP\n");
			#endif
			break;
		
		case SET_SWIM_SPEEDUP:
			var = robosharkstate.swim_param.motion_freq + 0.2;
			set_tail_freq(var);
			robosharkstate.swim_param.motion_freq = var;
		
			#ifdef PRINT_COMMAND_EN
				BuffPrintf("SET_SWIM_SPEEDUP\n");
			#endif
			break;
		
		case SET_SWIM_SPEEDDOWN:
			var = robosharkstate.swim_param.motion_freq - 0.2;
			set_tail_freq(var);
			robosharkstate.swim_param.motion_freq = var;
		
		  #ifdef PRINT_COMMAND_EN
				BuffPrintf("SET_SWIM_SPEEDDOWN\n");
			#endif
			break;
		
		case SET_SWIM_LEFT:
			set_tail_offset(-0.3);
			robosharkstate.swim_param.motion_offset = -30;
		
			#ifdef PRINT_COMMAND_EN
				BuffPrintf("SET_SWIM_LEFT\n");
			#endif
		  break;
		
		case SET_SWIM_STRAIGHT:
			set_tail_offset(0);
			robosharkstate.swim_param.motion_offset = 0;
		
			#ifdef PRINT_COMMAND_EN
				BuffPrintf("SET_SWIM_RIGHT\n");
			#endif
		  break;
		
		case SET_SWIM_RIGHT:
			set_tail_offset(0.3);
			robosharkstate.swim_param.motion_offset = 30;
		
			#ifdef PRINT_COMMAND_EN
				BuffPrintf("SET_SWIM_RIGHT\n");
			#endif
		  break;
		
		case SET_SWIM_UP:
			set_pect_fin_angle(-0.8, 0);
			set_pect_fin_angle(0.8, 1);
			robosharkstate.swim_param.pecfin_angle[0] = -0.6;
			robosharkstate.swim_param.pecfin_angle[1] = 0.6;
			#ifdef PRINT_COMMAND_EN
				BuffPrintf("SET_SWIM_UP\n");
			#endif
			break;
		
		case SET_SWIM_DOWN:
			set_pect_fin_angle(0.8, 0);
			set_pect_fin_angle(-0.8, 1);
			robosharkstate.swim_param.pecfin_angle[0] = 0.6;
			robosharkstate.swim_param.pecfin_angle[1] = -0.6;
			#ifdef PRINT_COMMAND_EN
				BuffPrintf("SET_SWIM_DOWN\n");
			#endif
			break;
				
		case SET_SINE_MOTION_AMP:
			data = atof(message);
			set_tail_amp(data);
			robosharkstate.swim_param.motion_amp = data;
		
			#ifdef PRINT_COMMAND_EN
				BuffPrintf("SET_SINE_MOTION_AMP\n");
			#endif
			break;
		
		case SET_SINE_MOTION_FREQ:
			data = atof(message);
			set_tail_freq(data);
			robosharkstate.swim_param.motion_freq = data;
		
			#ifdef PRINT_COMMAND_EN
				BuffPrintf("SET_SINE_MOTION_FREQ\n");
			#endif
			break;
		
		case SET_SINE_MOTION_OFFSET:
			data = atof(message);
			set_tail_offset(data);
			robosharkstate.swim_param.motion_offset = data;
		
			#ifdef PRINT_COMMAND_EN
				BuffPrintf("SET_SINE_MOTION_OFFSET\n");
			#endif
			break;
		
		case SET_LEFTPECFIN_UP:
			robosharkstate.swim_param.pecfin_angle[0] += 0.27;
			set_pect_fin_angle(robosharkstate.swim_param.pecfin_angle[0], 0);
			
			#ifdef PRINT_COMMAND_EN
				BuffPrintf("SET_LEFTPECFIN_UP\n");
			#endif
			break;
		
		case SET_LEFTPECFIN_ZERO:
			robosharkstate.swim_param.pecfin_angle[0] = 0.0f;
			set_pect_fin_angle(robosharkstate.swim_param.pecfin_angle[0], 0);
		
			#ifdef PRINT_COMMAND_EN
				BuffPrintf("SET_LEFTPECFIN_ZERO\n");
			#endif
			break;
		
		case SET_LEFTPECFIN_DOWN:
			robosharkstate.swim_param.pecfin_angle[0] -= 0.27;
			set_pect_fin_angle(robosharkstate.swim_param.pecfin_angle[0], 0);
		
			#ifdef PRINT_COMMAND_EN
				BuffPrintf("SET_LEFTPECFIN_DOWN\n");
			#endif
			break;
		
		case SET_RIGHTPECFIN_UP:
			robosharkstate.swim_param.pecfin_angle[1] -= 0.27;
			set_pect_fin_angle(robosharkstate.swim_param.pecfin_angle[1], 1);
		
			#ifdef PRINT_COMMAND_EN
				BuffPrintf("SET_RIGHTPECFIN_UP\n");
			#endif
			break;
		
		case SET_RIGHTPECFIN_ZERO:
			robosharkstate.swim_param.pecfin_angle[1] = 0.0f;
			set_pect_fin_angle(robosharkstate.swim_param.pecfin_angle[1], 1);
		
			#ifdef PRINT_COMMAND_EN
				BuffPrintf("SET_RIGHTPECFIN_ZERO\n");
			#endif
			break;
		
		case SET_RIGHTPECFIN_DOWN:
			robosharkstate.swim_param.pecfin_angle[1] += 0.27;
			set_pect_fin_angle(robosharkstate.swim_param.pecfin_angle[1], 1);
		
			#ifdef PRINT_COMMAND_EN
				BuffPrintf("SET_RIGHTPECFIN_DOWN\n");
			#endif
			break;		
		
		case SET_DATASHOW_OVER:
			data_show_app_stop();
		
			#ifdef PRINT_COMMAND_EN
				BuffPrintf("SET_READDATA_OVER\n");
			#endif
			break;
		
		case SET_DEPTHCTL_START:
			depth_control_app_resume();
			#ifdef PRINT_COMMAND_EN
				BuffPrintf("SET_DEPTHCTL_START\n");
			#endif
			break;
		
		case SET_DEPTHCTL_STOP:
			depth_control_app_stop();
			#ifdef PRINT_COMMAND_EN
				BuffPrintf("SET_DEPTHCTL_STOP\n");
			#endif
			break;
		
		case SET_DEPTHCTL_PARAM:
			#ifdef PRINT_COMMAND_EN
				BuffPrintf("SET_DEPTHCTL_PARAM\n");
			#endif
			break;
		case SET_AUTOCTL_RUN:
			robosharkstate.autoctl_state = AutoCTL_RUN;
			break;
		
		case SET_AUTOCTL_STOP:
			robosharkstate.autoctl_state = AutoCTL_STOP;
			break;
        
        case SET_AN_EVENT:
            curEvt = (EventID)message[0];
            break;
			
		case READ_ROBOT_STATUS:
			uint8_t rmstatus = 0x00;
			rmstatus = (uint8_t)(((robosharkstate.swim_state & 0x03)<<6) + ((robosharkstate.autoctl_state & 0x01)<<5));
			memcpy(databuf, &rmstatus, 1);
			RFLink_sendStruct(FRIEND_ID1, command, databuf, 1);
		  
			#ifdef PRINT_COMMAND_EN
				BuffPrintf("READ_ROBOT_STATUS\n");
			#endif
			break;
		
		case READ_SINE_MOTION_PARAM:
			memcpy(databuf,   &(robosharkstate.swim_param.motion_amp),    4);
			memcpy(databuf+4, &(robosharkstate.swim_param.motion_freq),   4);
			memcpy(databuf+8, &(robosharkstate.swim_param.motion_offset), 4);
			RFLink_sendStruct(FRIEND_ID1,command, databuf, 12);
		  
			#ifdef PRINT_COMMAND_EN
				BuffPrintf("READ_SINE_MOTION_PARAM\n");
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
				
		case READ_DEPTH:
			data = atof(message);
			set_data_type(command,data);
			data_show_app_resume();
		
			#ifdef PRINT_COMMAND_EN
				BuffPrintf("READ_DEPTH\n");
			#endif
			break;
		
		case READ_INFRARED_SWITCH:
			data = atof(message);
			set_data_type(command,data);
			data_show_app_resume();
			break;
		
		case READ_INFRARED_DISTANCE:
			data = atof(message);
			set_data_type(command,data);
			data_show_app_resume();
			break;
		
		case READ_FILE_LIST:
		  mf_scan_files("0:");
			//RFLink_message("%s",  "read file");
			#ifdef PRINT_COMMAND_EN
				BuffPrintf("READ_FILE_LIST\n");
			#endif
			break;
		
		
		case GOTO_STORAGE_DATA:
			if(message[0] == 0x01) // 储存1型数据
			{
				if(message_len > 1) // 如果message里面带了文件名字
				{
					*(message+message_len+1) = '\0';
					set_savefile_name(1, message+1);
				}
				else // 如果message里面没有带文件名字
					set_savefile_name(1, "data.bin");
			}
			RFLink_sendStruct(FRIEND_ID1,command, NULL, 0);
			data_storage_app_resume();
			
			#ifdef PRINT_COMMAND_EN
				BuffPrintf("GOTO_STORAGE_DATA\n");
			#endif
			break;
		
		case GOTO_STOP_STORAGE:
			data_storage_app_stop();
			RFLink_sendStruct(FRIEND_ID1,command, NULL, 0);
		
			#ifdef PRINT_COMMAND_EN
				BuffPrintf("GOTO_STOP_STORAGE\n");
			#endif
			break;
			
		case GOTO_SEND_DATA:
			if(message[0] == 0x01) // 如果message里面没有带文件名字，那么就是储存基础数据
			{
				databuf[0] = 0x01;
				RFLink_sendStruct(FRIEND_ID1, command, databuf, 1);
				delay_ms(1000);
				if(message_len > 1) // 如果message里面带了文件名字
				{
					*(message+message_len+1) = '\0';
					set_sendfile_name(message+1);
					go_to_sendback(1);
				}
				else // 如果message里面没有带文件名字
				{
					set_sendfile_name("data.bin");
					go_to_sendback(1);
				}
				
			}
			
			#ifdef PRINT_COMMAND_EN
				BuffPrintf("SEND_DATA\n");
			#endif
			break;
		
		case LAST_COMMAND_FLAG:
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

		if(RFLink_byteAnalysisCall(USART2_ID, rx_data))
		{
			//BuffPrintf("Package analysised\n");
			result =  RFLink_commandAnalysis(USART2_ID);
			command_execute(result.command, result.message, result.message_len);
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



