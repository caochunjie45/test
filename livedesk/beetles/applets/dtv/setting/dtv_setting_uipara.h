/*
**************************************************************************************************************
*											         ePDK
*						            the Easy Portable/Player Develop Kits
*									           desktop system 
*
*						        	 (c) Copyright 2007-2010, ANDY, China
*											 All Rights Reserved
*
* File    	: dtv_settings_uipara.h
* By      	: lantian.lin
* Func	: ui parameter
* Version	: v1.0
* ============================================================================================================
**************************************************************************************************************
*/

#ifndef __dtv_SETTING_UIPARA_H_
#define  __dtv_SETTING_UIPARA_H_
#include "dtv_setting_i.h"

typedef struct setting_uipara_s
{
	RECT   title;                       //标题
	RECT   listbar;                     //listbar坐标	
	__s32  listbar_number;	            // listbar 总数
	__u8   listbar_height;
	__u8   listbar_width;
}setting_uipara_t;

setting_uipara_t* setting_get_uipara(void);

#endif

