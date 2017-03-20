/*
**************************************************************************************************************
*											         ePDK
*						            the Easy Portable/Player Develop Kits
*									           desktop system 
*
*						        	 (c) Copyright 2007-2010, ANDY, China
*											 All Rights Reserved
*
* File    	: dtv_settings.c
* By      	: hot.lee
* Func		: desk main thread
* Version	: v1.0
* ============================================================================================================
* 2009-7-20 8:51:52  hot.lee  create this file, implements the fundemental interface;
**************************************************************************************************************
*/

#ifndef __APP_dtv_SETTINGS_H__
#define  __APP_dtv_SETTINGS_H__
#include "dtv_setting_i.h"
#include "dtv_setting_list.h"
#include "dtv_setting_disp.h"
#include "dtv_setting_lcd.h"
#include "dtv_setting_lcn.h"
#include "dtv_setting_antenna.h"


typedef enum
{
	dtv_SETTING_GENERAL = 0,
	dtv_SETTING_POWER_MANAGE,
	dtv_SETTING_TIPS,	
	dtv_SETTING_ALARM,
}dtv_setting_root_type_t;

typedef struct tag_dtv_setting_para
{
	GUI_FONT *dtv_setting_font;

	H_LYR layer;
	dtv_setting_root_type_t root_type;
	__s32 focus_id;
	__s32 old_focus;
	
}dtv_setting_para_t;


typedef struct tag_dtv_setting_ctrl
{
	H_WIN 		h_frm_list;//��ϸ��Ϣ

	H_WIN 		layer;

    __s32 		focus_id;//��ǰѡ�����Ŀ
    __s32 		old_focus;

	GUI_FONT 	*dtv_setting_font;

}dtv_setting_ctrl_t;



H_WIN setting_create(H_WIN	h_parent,H_LYR layer);

#endif
