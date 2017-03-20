/*
**************************************************************************************************************
*											         ePDK
*						            the Easy Portable/Player Develop Kits
*									           desktop system 
*
*						        	 (c) Copyright 2007-2010, ANDY, China
*											 All Rights Reserved
*
* File    	: program_previews.c
* By      	: Hot.lee
* Func		: desk main thread
* Version	: v1.0
* ============================================================================================================
* 2013-3-30 8:51:52  Hot.lee  create this file, implements the fundemental interface;
**************************************************************************************************************
*/

#ifndef __APP_PROGRAM_PREVIEW_H__
#define  __APP_PROGRAM_PREVIEW_H__
#include "program_preview_i.h"
#include "program_preview_list.h"


typedef enum
{
	PROGRAM_PREVIEW_GENERAL = 0,
	PROGRAM_PREVIEW_POWER_MANAGE,
	PROGRAM_PREVIEW_TIPS,	
	PROGRAM_PREVIEW_ALARM,
}program_preview_root_type_t;

typedef struct tag_program_preview_para
{
	GUI_FONT *program_preview_font;

	H_LYR layer;
	program_preview_root_type_t root_type;
	__s32 focus_id;
	__s32 old_focus;
	
}program_preview_para_t;


typedef struct tag_program_preview_ctrl
{
	H_WIN 		h_frm_list;//��ϸ��Ϣ

	H_WIN 		layer;

    __s32 		focus_id;//��ǰѡ�����Ŀ
    __s32 		old_focus;

	GUI_FONT 	*program_preview_font;

}program_preview_ctrl_t;


H_WIN program_create(H_WIN	h_parent,H_LYR layer);

#endif
