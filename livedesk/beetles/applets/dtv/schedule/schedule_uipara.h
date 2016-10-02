/*
**************************************************************************************************************
*											         ePDK
*						            the Easy Portable/Player Develop Kits
*									           desktop system 
*
*						        	 (c) Copyright 2007-2010, ANDY, China
*											 All Rights Reserved
*
* File    	: schedules_uipara.h
* By      	: lantian.lin
* Func	: ui parameter
* Version	: v1.0
* ============================================================================================================
**************************************************************************************************************
*/

#ifndef __SCHEDULE_UIPARA_H_
#define  __SCHEDULE_UIPARA_H_
#include "schedule_i.h"


typedef struct schedule_uipara_s
{
	RECT   title;                       //标题
	RECT   listbar;                     //listbar坐标
	RECT   epg_detailed_content;
	RECT   epg_detailed_content_text;
	
    __s32  epg_data_x_left_offset;      //data相对于listbar右移的坐标
    __s32  epg_data_x_right_offset;     //data防止超出listbar宽度左移的坐标
    
    __s32  epg_content_x_left_offset;   //content相对于listbar右移的坐标
    __s32  epg_content_x_right_offset;  //content防止超出listbar宽度左移的坐标

    __s32  epg_content_x_right_roll_offset;    
	__s32  listbar_number;	            // listbar 总数
	
	__u8   listbar_height;
	__s32  listbar_width;
}schedule_uipara_t;


schedule_uipara_t* schedule_get_uipara(void);

#endif

