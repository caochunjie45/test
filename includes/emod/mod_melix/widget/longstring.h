/*
**************************************************************************************************************************
*													        eMOD
*						            		the Easy Portable/Player Develop Kits
*									   				melix widget module 
*
*                             		 Copyright(C), 2011-2012, Softwinner Microelectronic Co., Ltd.
*									               All Rights Reserved
*
* File name		:	longstring.h
* Author			: 	Kiden
* Version 		: 	V1.00
* Date    		: 	2012-04-10
* Description	:
* Others 		:
* History 		:
* <Author>           <time>          <version>      <description>
* Kiden		    2012-04-10         1.0.0          Create File
*
**************************************************************************************************************************
*/

#ifndef __LONGSTRING_H__
#define __LONGSTRING_H__

typedef void *HSTRING;

typedef struct GUI_LSTRING_CREATE_PARA
{
	RECT					region;
	GUI_FONT				*pFont;
	GuiCharset				encode_id;
	char					*string;
	GuiColor				bkcolor;
	void					*bmp;
	GuiPos					bmp_pos;
	GuiColor				fontcolor;
	__u32					align;
}gui_lstring_create_para_t;

HSTRING gui_longstring_create(gui_lstring_create_para_t *para);

__s32 gui_longstring_delete(HSTRING hstring);

__s32 gui_longstring_start(HSTRING hstring);

__s32 gui_longstring_reset(HSTRING hstring, gui_lstring_create_para_t *para);

__s32 gui_longstring_stop(HSTRING hstring);

__s32 gui_longstring_move(HSTRING hstring);

#endif
