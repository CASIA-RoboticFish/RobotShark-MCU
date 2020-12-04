/*****************************************************************************
Copyright: CASIA ����������ʵ����
File name: boxfishstate.h
Description: ���ļ�����������boxfish�����˿������Ĳ�������������Ҫ�Ŀ���������������������ļ����涨��
	     ���磬��ȿ�������
	     typedef struct DepthControlParam // ������������Լ�����
	     {
		float depthctl_kp;
		float depthctl_ki;
		float depthctl_kd;
	     }DepthControlParam;
	     ֮��������Ҫ����ȿ�������app�ж�������ṹ�壬���磺DepthControlParam depth_ctl_param;
	     �����command_analysis_app.h�������ⲿ������extern DepthControlParam depth_ctl_param;
	     �������Ϳ�����command_analysis_app�аѴ��ڽ��յ��Ĳ���д��depth_ctl_param��Ȼ����
Author: VincentFEI
Version: 2.0.0
Date: 2020.09.03
History: 
*****************************************************************************/

#ifndef _CONTROL_PARAM_H_
#define _CONTROL_PARAM_H_


typedef struct DepthControlParam // ������������Լ�����
{
	float depthctl_kp;
	float depthctl_ki;
	float depthctl_kd;
}DepthControlParam;

#endif