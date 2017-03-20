/*
**************************************************************************************************************
*											         ePDK
*						            the Easy Portable/Player Develop Kits
*									           desktop system 
*
*						        	 (c) Copyright 2007-2010, ANDY, China
*											 All Rights Reserved
*
* File    	: app_settings.c
* By      	: Andy.zhang
* Func		: desk main thread
* Version	: v1.0
* ============================================================================================================
* 2009-7-20 8:51:52  andy.zhang  create this file, implements the fundemental interface;
**************************************************************************************************************
*/

#ifndef __APP_SETTINGS_H__
#define  __APP_SETTINGS_H__
#include "app_setting_i.h"


typedef enum
{
	SETTING_GENERAL = 0,
	SETTING_POWER_MANAGE,
	SETTING_TIPS,	
}setting_root_type_t;
typedef struct tag_setting_para
{
	GUI_FONT *setting_font;

	H_LYR layer;
	setting_root_type_t root_type;
	__s32 focus_id;
	__s32 old_focus;
	
}setting_para_t;

typedef struct tag_setting_ctrl
{
	H_WIN h_frm_general;		//ͨ�����ô��ھ��
	H_WIN h_frm_power;		// ��Դ�����ھ��
	H_WIN h_frm_tips;		//��Ʒ��Ϣ��ʾ����
	H_WIN h_dialoag_win ;		//�ָ��������öԻ�����
	H_WIN lyr_setting;

	setting_root_type_t root_type;
	GUI_FONT *setting_font;
	
}setting_ctrl_t;



H_WIN app_setting_create(root_para_t *para);

#endif
