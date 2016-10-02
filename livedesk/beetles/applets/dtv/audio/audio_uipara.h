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

#ifndef __AUDIO_UIPARA_H_
#define  __AUDIO_UIPARA_H_

#include "audio_preview_i.h"


typedef struct audio_uipara_s
{
	RECT   title;                       //标题
	RECT   listbar;                     //listbar坐标	
	__s32  listbar_number;	            // listbar 总数
	__u8   listbar_height;
	__u8   listbar_width;
}audio_uipara_t;


audio_uipara_t* audio_get_uipara(void);

#endif

