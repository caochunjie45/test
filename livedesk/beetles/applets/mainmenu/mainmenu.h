/*
**************************************************************************************************************
*											         ePDK
*						            the Easy Portable/Player Develop Kits
*									           desktop system 
*
*						        	 (c) Copyright 2007-2010, ANDY, China
*											 All Rights Reserved
*
* File    	: main_menu.h
* By      	: James.lu
* Func		: desk main thread
* Version	: v1.0
* ============================================================================================================
* 2009-7-20 8:51:52  andy.zhang  create this file, implements the fundemental interface;
**************************************************************************************************************
*/
#ifndef __MAINMENU_H_
#define __MAINMENU_H_


typedef	enum tag_MAINMENU_LYRTYPE
{
	MAINMENU_LYRTYPE_NORMAL = 0x00,
	MAINMENU_LYRTYPE_SPRITE,
	
}__mainmenu_lyrtype_e;            

typedef struct tag_res_id_t
{
	__s32 lang_id;
	__s32 focus_bmp_id;
	__s32 unfocus_bmp_id;
	
}mainmenu_res_id_t;


void alpha_fadeout( void *buf, __u32 len);
H_LYR get_mainmenu_lyrwin(H_WIN hWnd);
H_WIN app_mainmenu_create(root_para_t *root_para);
__s32 app_mainmenu_destroy(H_WIN h_win);


#endif

