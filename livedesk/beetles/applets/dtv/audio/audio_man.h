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

#ifndef __AUDIO_MAN_H__
#define  __AUDIO_MAN_H__

#include "dtv_audio.h"
#include "audio_preview_i.h"

typedef struct tag_audio_preview_para
{
	GUI_FONT *audio_preview_font;

	H_LYR layer;
	__s32 focus_id;
	__s32 old_focus;
	
}audio_preview_para_t;


typedef struct tag_audio_preview_ctrl
{
	H_WIN 		h_frm_list;//详细信息
	H_WIN 		layer;
    __s32 		focus_id;//当前选择的条目
    __s32 		old_focus;
	GUI_FONT 	*audio_preview_font;

}audio_preview_ctrl_t;


__s32 dtv_preview_submenu_exist(void);

H_WIN audio_create(H_WIN h_parent,H_LYR layer);

#endif
