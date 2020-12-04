/*****************************************************************************
Copyright: CASIA 仿生机器鱼实验室
File name: boxfishstate.h
Description: 本文件定义了描述boxfish机器人控制器的参数，机器人需要的控制器参数都可以在这个文件下面定义
	     例如，深度控制器：
	     typedef struct DepthControlParam // 这个可以由你自己定义
	     {
		float depthctl_kp;
		float depthctl_ki;
		float depthctl_kd;
	     }DepthControlParam;
	     之后，我们需要在深度控制器的app中定义这个结构体，例如：DepthControlParam depth_ctl_param;
	     最后，在command_analysis_app.h中申明外部变量：extern DepthControlParam depth_ctl_param;
	     这样，就可以在command_analysis_app中把串口接收到的参数写入depth_ctl_param，然后在
Author: VincentFEI
Version: 2.0.0
Date: 2020.09.03
History: 
*****************************************************************************/

#ifndef _CONTROL_PARAM_H_
#define _CONTROL_PARAM_H_


typedef struct DepthControlParam // 这个可以由你自己定义
{
	float depthctl_kp;
	float depthctl_ki;
	float depthctl_kd;
}DepthControlParam;

#endif