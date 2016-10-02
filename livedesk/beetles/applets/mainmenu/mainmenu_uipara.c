/*
**************************************************************************************************************
*											         ePDK
*						            the Easy Portable/Player Develop Kits
*									           desktop system 
*
*						        	 (c) Copyright 2007-2010, ANDY, China
*											 All Rights Reserved
*
* File    	: mainmenu_uipara.c
* By      	: James.lu
* Func		: desk main thread
* Version	: v1.0
* ============================================================================================================
* 2009-7-20 8:51:52  andy.zhang  create this file, implements the fundemental interface;
**************************************************************************************************************
*/

#include "mainmenu_private.h"


static __mainmenu_uipara_t mainmenu_uipara;


static __mainmenu_frm_para_t frm_para[ITEM_NUM_ALL] = 
{
	{ ID_MAINMENU_TV,   RESID_INDEX_TV,         20, 10, 110, 110,80,80, 1},//tv
	{ ID_MAINMENU_MOVIE,RESID_INDEX_MOVIE,      130,10, 110, 110,80,80, 1},//movie
	{ ID_MAINMENU_MUSIC,RESID_INDEX_MUSIC,      240,10, 110, 110,80,80, 1},//music
	{ ID_MAINMENU_PHOTO,RESID_INDEX_PHOTO,      350,10, 110, 110,80,80, 1},//photo
	{ ID_MAINMENU_FM,   RESID_INDEX_FM,         20, 120,110, 110,80,80, 1},//fm
	{ ID_MAINMENU_EBOOK,RESID_INDEX_EBOOK,      130,120,110, 110,80,80, 1},//ebook
    { ID_MAINMENU_EXPLORER,RESID_INDEX_EXPLORER,240,120,110, 110,80,80, 1},//explorer
	{ ID_MAINMENU_SETTING,RESID_INDEX_SETTING,  350,120,110, 110,80,80, 1},//setting
};



__mainmenu_uipara_t* mainmenu_init_uipara(void)
{
	__s32       screen_width;
	__s32 		screen_height;

	dsk_display_get_size(&screen_width, &screen_height);

	if((screen_width == 800 )&&( screen_height == 480))
	{
		mainmenu_uipara.mainmenu_para.all_item   = ITEM_NUM_ALL;
		mainmenu_uipara.mainmenu_para.cur_layout = 0;


		mainmenu_uipara.mainmenu_para.frm_para  = frm_para;

		mainmenu_uipara.mainmenu_lyr.fb_w  = 688;
		mainmenu_uipara.mainmenu_lyr.fb_h  = 320;

		mainmenu_uipara.mainmenu_lyr.src_x = 0;
		mainmenu_uipara.mainmenu_lyr.src_y = 0;
		mainmenu_uipara.mainmenu_lyr.src_w = mainmenu_uipara.mainmenu_lyr.fb_w;
		mainmenu_uipara.mainmenu_lyr.src_h = mainmenu_uipara.mainmenu_lyr.fb_h;

		mainmenu_uipara.mainmenu_lyr.scn_x = 56;
		mainmenu_uipara.mainmenu_lyr.scn_y = 102;	
		mainmenu_uipara.mainmenu_lyr.scn_w = mainmenu_uipara.mainmenu_lyr.fb_w;		
		mainmenu_uipara.mainmenu_lyr.scn_h = mainmenu_uipara.mainmenu_lyr.fb_h;

		mainmenu_uipara.mainmenu_lyr.pipe  = 1;
	}

	if((screen_width == 1024 )&&( screen_height == 600))
	{
		mainmenu_uipara.mainmenu_para.all_item   = ITEM_NUM_ALL;
		mainmenu_uipara.mainmenu_para.cur_layout = 0;


		mainmenu_uipara.mainmenu_para.frm_para  = frm_para;

		mainmenu_uipara.mainmenu_lyr.fb_w  = 1024;
		mainmenu_uipara.mainmenu_lyr.fb_h  = 550;

		mainmenu_uipara.mainmenu_lyr.src_x = 0;
		mainmenu_uipara.mainmenu_lyr.src_y = 0;
		mainmenu_uipara.mainmenu_lyr.src_w = mainmenu_uipara.mainmenu_lyr.fb_w;
		mainmenu_uipara.mainmenu_lyr.src_h = mainmenu_uipara.mainmenu_lyr.fb_h;

		mainmenu_uipara.mainmenu_lyr.scn_x = 0;
		mainmenu_uipara.mainmenu_lyr.scn_y = 50;	
		mainmenu_uipara.mainmenu_lyr.scn_w = mainmenu_uipara.mainmenu_lyr.fb_w;		
		mainmenu_uipara.mainmenu_lyr.scn_h = mainmenu_uipara.mainmenu_lyr.fb_h;

		mainmenu_uipara.mainmenu_lyr.pipe  = 1;
	}

	if((screen_width == 480 )&&( screen_height == 272))
	{
		mainmenu_uipara.mainmenu_para.all_item   = ITEM_NUM_ALL;
		mainmenu_uipara.mainmenu_para.cur_layout = 0;


		mainmenu_uipara.mainmenu_para.frm_para  = frm_para;

		mainmenu_uipara.mainmenu_lyr.fb_w  = 480;
		mainmenu_uipara.mainmenu_lyr.fb_h  = 242;

		mainmenu_uipara.mainmenu_lyr.src_x = 0;
		mainmenu_uipara.mainmenu_lyr.src_y = 0;
		mainmenu_uipara.mainmenu_lyr.src_w = mainmenu_uipara.mainmenu_lyr.fb_w;
		mainmenu_uipara.mainmenu_lyr.src_h = mainmenu_uipara.mainmenu_lyr.fb_h;

		mainmenu_uipara.mainmenu_lyr.scn_x = 0;
		mainmenu_uipara.mainmenu_lyr.scn_y = 30;	
		mainmenu_uipara.mainmenu_lyr.scn_w = mainmenu_uipara.mainmenu_lyr.fb_w;		
		mainmenu_uipara.mainmenu_lyr.scn_h = mainmenu_uipara.mainmenu_lyr.fb_h;

		mainmenu_uipara.mainmenu_lyr.pipe  = 1;
	}

	return &mainmenu_uipara;
}

void mainmenu_deinit_uipara(void)
{
	return;
}

