/*
**************************************************************************************************************
*											         ePDK
*						            the Easy Portable/Player Develop Kits
*									           desktop system 
*
*						        	 (c) Copyright 2007-2011, CHIPHD, China
*											 All Rights Reserved
*
* File    	: fm_uipara.h
* By      	: CQQ
* Func		: 
* Version	: v1.0
* ============================================================================================================
* 2011/07/25 10:31  create this file, implements the fundamental interface;
**************************************************************************************************************
*/
#ifndef __FM_UIPARA_H__E5B6392A_B497_4020_ADC5_7C344A883270__
#define __FM_UIPARA_H__E5B6392A_B497_4020_ADC5_7C344A883270__

#include "beetles_app.h"

typedef struct
{
	TOUCH_UI_RECT rt;        
	TOUCH_UI_RECT scale_bg;  // ��߱���λ��
	TOUCH_UI_DPOS scale;     // ��� X ��Χ
	TOUCH_UI_RTSZ btn_thumb; // ��ť��С
	TOUCH_UI_INT  cursor_y;  // �α��Yֵ
	TOUCH_UI_INT  txt_y;     // �ı���ʼyֵ

	__s32         clr_txt_n; //��ͨ�ı���ɫ

	
}FM_UI_MAINWND;

//ui����
typedef struct
{
	RECT rt;
	FM_UI_MAINWND main;
#ifdef SUPPORT_FM_VOLUME
	RECT  volume_bar_lyr;
	RECT bmp_slide_bar;
	RECT bmp_slide_block_l;
	RECT bmp_slide_block_r;
	RECT bmp_slide_block;
	RECT bmp_volume_l;
	RECT bmp_volume_r;
	RECT volume_value_pos;
#endif	
}FM_UI_PARA_T;

//////////////////////////////////////////////////////////////////////////
extern FM_UI_PARA_T * get_fm_uipara(void);

#endif //__FM_UIPARA_H__E5B6392A_B497_4020_ADC5_7C344A883270__
//End of this file

