/*
**************************************************************************************************************
*											         ePDK
*						            the Easy Portable/Player Develop Kits
*									           desktop system 
*
*						        	 (c) Copyright 2007-2011, CHIPHD, China
*											 All Rights Reserved
*
* File    	: fm_uipara.c
* By      	: CQQ
* Func		: 
* Version	: v1.0
* ============================================================================================================
* 2011/07/25 10:31  create this file, implements the fundamental interface;
**************************************************************************************************************
*/

#include "fm_uipara.h"
//#undef FOR_800X480_UI

//////////////////////////////////////////////////////////////////////////

static FM_UI_PARA_T fm_ui_480x272 = 
{
	{//TOUCH_UI_RECT
		0, 0, 480, 272
	},
	{//FM_UI_MAINWND
		{// rt
			0, 0, 480, 272,
		},
		{// scale_bg 标尺背景位置
			//0, 60, 480, 45
			//29, 172, 420, 12
			30, 142, 420, 12
		},
		{//scale, 刻度x值范围
			//29, 449
			29,449
		},
		{//btn_thumb; // 按钮大小
			//28, 17
			15,11
		},
		0, //cursor_y, 游标y值(相对于刻度图片)
		24, //文本y值(相对于刻度图片)
		APP_COLOR_WHITE,
	},
#ifdef SUPPORT_FM_VOLUME
		{0,0,0,0},
		{0,0,0,0},
		{0,0,0,0},
		{0,0,0,0},
		{0,0,0,0},
		{0,0,0,0},
		{0,0,0,0},
		{0,0,0,0},
#endif		
};

#ifdef FOR_800X480_UI
static FM_UI_PARA_T fm_ui_800x480 = 
{
	{//TOUCH_UI_RECT
		0+180, 0+120, 480, 272
	},
	{//FM_UI_MAINWND
		{// rt
			0+180, 0+120, 480, 272,
		},
		{// scale_bg 标尺背景位置
			//0, 60, 480, 45
			//29, 172, 420, 12
			30+200, 142, 420, 12
		},
		{//scale, 刻度x值范围
			//29, 449
			29,449
		},
		{//btn_thumb; // 按钮大小
			//28, 17
			15,11
		},
		0, //cursor_y, 游标y值(相对于刻度图片)
		24+120, //文本y值(相对于刻度图片)
		APP_COLOR_WHITE,
	},
#ifdef SUPPORT_FM_VOLUME
		{0,0,0,0},
		{0,0,0,0},
		{0,0,0,0},
		{0,0,0,0},
		{0,0,0,0},
		{0,0,0,0},
		{0,0,0,0},
		{0,0,0,0},
#endif		
};
#endif
//
FM_UI_PARA_T * get_fm_uipara(void)
{
#ifdef SUPPORT_FM_VOLUME
#ifdef FOR_800X480_UI
	fm_ui_800x480.volume_bar_lyr.x  = 30;
	fm_ui_800x480.volume_bar_lyr.y  = 220+2;
	fm_ui_800x480.volume_bar_lyr.width  = 400;
	fm_ui_800x480.volume_bar_lyr.height  = 50;
	fm_ui_800x480.bmp_volume_r.x	= 350;
	fm_ui_800x480.bmp_volume_r.y = 1+2;
	fm_ui_800x480.bmp_volume_r.width = 15;
	fm_ui_800x480.bmp_volume_r.height = 13;
	
	fm_ui_800x480.volume_value_pos.x = 370;
	fm_ui_800x480.volume_value_pos.y = 0;
	fm_ui_800x480.volume_value_pos.width = 30;
	fm_ui_800x480.volume_value_pos.height = 24;

	fm_ui_800x480.bmp_slide_block_l.x = 82;
	fm_ui_800x480.bmp_slide_block_l.y = 8+2;
	fm_ui_800x480.bmp_slide_block_l.width = 5;
	fm_ui_800x480.bmp_slide_block_l.height = 4;

	fm_ui_800x480.bmp_slide_bar.x = 82;
	fm_ui_800x480.bmp_slide_bar.y = 8+2;
	fm_ui_800x480.bmp_slide_bar.width = 255;
	fm_ui_800x480.bmp_slide_bar.height = 4;

	fm_ui_800x480.bmp_slide_block_r.x = 322;
	fm_ui_800x480.bmp_slide_block_r.y = 8+2;
	fm_ui_800x480.bmp_slide_block_r.width = 5;
	fm_ui_800x480.bmp_slide_block_r.height = 4;

	fm_ui_800x480.bmp_slide_block.x = 0;
	fm_ui_800x480.bmp_slide_block.y = 0+2;
	fm_ui_800x480.bmp_slide_block.width = 5;
	fm_ui_800x480.bmp_slide_block.height = 4;
	
	fm_ui_800x480.bmp_volume_l.x	= 54;
	fm_ui_800x480.bmp_volume_l.y = 1+2;
	fm_ui_800x480.bmp_volume_l.width = 15;
	fm_ui_800x480.bmp_volume_l.height = 13;
#else
	fm_ui_480x272.volume_bar_lyr.x  = 30;
	fm_ui_480x272.volume_bar_lyr.y  = 220;
	fm_ui_480x272.volume_bar_lyr.width  = 400;
	fm_ui_480x272.volume_bar_lyr.height  = 50;
	fm_ui_480x272.bmp_volume_r.x	= 350;
	fm_ui_480x272.bmp_volume_r.y = 1;
	fm_ui_480x272.bmp_volume_r.width = 15;
	fm_ui_480x272.bmp_volume_r.height = 13;
	
	fm_ui_480x272.volume_value_pos.x = 370;
	fm_ui_480x272.volume_value_pos.y = 0;
	fm_ui_480x272.volume_value_pos.width = 20;
	fm_ui_480x272.volume_value_pos.height = 24;

	fm_ui_480x272.bmp_slide_block_l.x = 82;
	fm_ui_480x272.bmp_slide_block_l.y = 8;
	fm_ui_480x272.bmp_slide_block_l.width = 5;
	fm_ui_480x272.bmp_slide_block_l.height = 4;

	fm_ui_480x272.bmp_slide_bar.x = 82;
	fm_ui_480x272.bmp_slide_bar.y = 8;
	fm_ui_480x272.bmp_slide_bar.width = 255;
	fm_ui_480x272.bmp_slide_bar.height = 4;

	fm_ui_480x272.bmp_slide_block_r.x = 322;
	fm_ui_480x272.bmp_slide_block_r.y = 8;
	fm_ui_480x272.bmp_slide_block_r.width = 5;
	fm_ui_480x272.bmp_slide_block_r.height = 4;

	fm_ui_480x272.bmp_slide_block.x = 0;
	fm_ui_480x272.bmp_slide_block.y = 0;
	fm_ui_480x272.bmp_slide_block.width = 5;
	fm_ui_480x272.bmp_slide_block.height = 4;
	
	fm_ui_480x272.bmp_volume_l.x	= 54;
	fm_ui_480x272.bmp_volume_l.y = 1;
	fm_ui_480x272.bmp_volume_l.width = 15;
	fm_ui_480x272.bmp_volume_l.height = 13;
#endif	
#endif	
#ifdef FOR_800X480_UI
	return &fm_ui_800x480;
#else
	return &fm_ui_480x272;
#endif
}

//////////////////////////////////////////////////////////////////////////
//end of file


