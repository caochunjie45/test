/*
**************************************************************************************************************
*											         ePDK
*						            the Easy Portable/Player Develop Kits
*									           desktop system 
*
*						        	 (c) Copyright 2007-2010, ANDY, China
*											 All Rights Reserved
*
* File    	: settings_para.h
* By      	: Andy.zhang
* Func		: desk main thread
* Version	: v1.0
* ============================================================================================================
* 2009-7-20 8:51:52  andy.zhang  create this file, implements the fundemental interface;
**************************************************************************************************************
*/

#ifndef __SETTING_PARA_H_
#define  __SETTING_PARA_H_
#include "app_setting_i.h"

typedef struct item_para
{
	OFFSET  item_bmp_focus_pos;
	OFFSET	 item_bmp_unfocus_pos;
	OFFSET  item_bmp_left_pos;
	OFFSET  item_bmp_right_pos;
	OFFSET  item_string_title_pos;
	RECT item_string_content_rect;
}set_item_para;
typedef struct general_uipara
{
	RECT toolbar_rect;
	OFFSET bmp_big_setting_pos;
	RECT bmp_prompt_rect;
	OFFSET  string_prompt_pos;
	RECT  string_detail_prompt_rect;
	RECT  string_detail_prompt_rect_ex;
	//set_item_para item[7];
  	set_item_para item[9];
}set_general_para;

typedef struct power_uipara
{
	RECT toolbar_rect;
	OFFSET bmp_big_setting_pos;
	RECT bmp_prompt_rect;
	OFFSET  string_prompt_pos;
	RECT  string_detail_prompt_rect;
	RECT  string_detail_prompt_rect_ex;
	set_item_para item[3];	

}set_power_para;

typedef struct tips_uipara
{
	OFFSET   tips_bmp_pos;	//��ʾ�򱳾�ͼ����λ��
	RECT      tips_tiltle_pos; 	//��ʾ��title����λ��
	OFFSET   tip_tsize_pos;		//�ܴ�����������λ��
	OFFSET   tip_fsize_pos;		//ʣ�������������λ��
	OFFSET   tip_ok_bmp_pos;	//OK �ַ���ʾ��ͼ����
  
}tips_uipara_t;


set_general_para* setting_get_general_para(SIZE screen_size);
set_power_para* setting_get_power_para(SIZE screen_size);
tips_uipara_t* setting_get_tips_para(SIZE screen_size);
void  main_cmd2parent(H_WIN hwin, __s32 id, __s32 data1, __s32 data2);


#endif

