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

#ifndef __DTV_PAINT_H__
#define  __DTV_PAINT_H__

#include "dtv_setting_i.h"

extern dtv_ctr_t  dtv_ctr;


void show_dtv_record_staue_win(void);
void show_dtv_destory_win(void);
H_WIN layer_win_create( RECT *rect);

#endif
