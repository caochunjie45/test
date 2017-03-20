/*
**********************************************************************************************************************
*                                                    ePDK
*                                    the Easy Portable/Player Develop Kits
*                                              LiveTouch Sub-System
*
*                                   (c) Copyright 2007-2009, Steven.ZGJ.China
*                                             All Rights Reserved
*
* Moudle  : ebook
* File    : ebook_uipara.c
*
* By      : Bayden.chen
* Version : v1.0
* Date    : 2009-12-25 14:53:05
**********************************************************************************************************************
*/

#ifndef __SETTING_PARA_H__
#define __SETTING_PARA_H__
#include "setting_para.h"

static tips_uipara_t setting_tips_uipara_800_480=
{
	{0,0},	//tips_bmp_pos
	{0,0,191,20},	//title pos 
	{15,28},	//tip_tsize_pos
	{15,50},	//tip_fsize_pos
	{61,72}	//tip_ok_bmp_pos

};

static tips_uipara_t setting_tips_uipara_400_240=
{
	{0,0},	//tips_bmp_pos
	{0,0,191,20},	//title pos 
	{15,28},	//tip_tsize_pos
	{15,50},	//tip_fsize_pos
	{61,72}	//tip_ok_bmp_pos

};

static set_general_para setting_general_uipara_800_480=
{    
		{0,203,400,37},   //toolbar_rect
		{11,30},		  //bmp_big_setting_pos
		{10,219,30,16}, // bmp_prompt_rect;
		{45,217},		//  string_prompt_pos;
		{11,155,126,25},		//RECT  string_detail_prompt_rect;
		{11,175,126,25},		//RECT  string_detail_prompt_rect_ex;
		{
			{           //item0;
				{150,15},		// bmp_focus_pos;
				{150,15},		// bmp_unfocus_pos;
				{270,15},		//bmp_left_pos;
				{360,15},		//bmp_right_pos;
				{155,16},		//string_title_pos;
				{290,15,70,25} //string_content_rect;
			},
			{           //item1;
				{150,45},		// bmp_focus_pos;
				{150,45},		// bmp_unfocus_pos;
				{270,45},		//bmp_left_pos;
				{360,46},		//bmp_right_pos;
				{155,45},		//string_title_pos;
				{290,45,70,25} //string_content_rect;
			},
			{           //item2;
				{150,75},		// bmp_focus_pos;
				{150,75},		// bmp_unfocus_pos;
				{270,75},		//bmp_left_pos;
				{360,75},		//bmp_right_pos;
				{155,76},		//string_title_pos;
				{290,75,70,25} //string_content_rect;
			},
			{           //item3;
				{150,105},		// bmp_focus_pos;
				{150,105},		// bmp_unfocus_pos;
				{270,105},		//bmp_left_pos;
				{360,105},		//bmp_right_pos;
				{155,106},		//string_title_pos;
				{290,105,70,25} //string_content_rect;
			},
			
			{           //item4;
				{150,135},		// bmp_focus_pos;
				{150,135},		// bmp_unfocus_pos;
				{270,135},		//bmp_left_pos;
				{360,135},		//bmp_right_pos;
				{155,136},		//string_title_pos;
				{290,135,70,25} //string_content_rect;
			},
			{           //item5;
				{150,165},		// bmp_focus_pos;
				{150,165},		// bmp_unfocus_pos;
				{270,165},		//bmp_left_pos;
				{360,165},		//bmp_right_pos;
				{155,166},		//string_title_pos;
				{290,165,70,25} //string_content_rect;
			},
			{           //item6;
				{150,195},		// bmp_focus_pos;
				{150,195},		// bmp_unfocus_pos;
				{270,195},		//bmp_left_pos;
				{360,195},		//bmp_right_pos;
				{155,196},		//string_title_pos;
				{290,195,70,25} //string_content_rect;
			}
		}
};

static set_power_para setting_power_uipara_800_480=
{    
		{0,219,400,21},   //toolbar_rect
		{11,30},		  //bmp_big_setting_pos
		{10,219,30,16}, // bmp_prompt_rect;
		{45,217},		//  string_prompt_pos;
		{11,155,200,25},		//RECT  string_detail_prompt_rect;
		{11,180,200,25},		//RECT  string_detail_prompt_rect_ex;
		{
			{           //item0;
				{150,30},		// bmp_focus_pos;
				{150,30},		// bmp_unfocus_pos;
				{270,37},		//bmp_left_pos;
				{360,37},		//bmp_right_pos;
				{155,31},		//string_title_pos;
				{290,30,70,24} //string_content_rect;
			},
			{           //item1;
				{150,54},		// bmp_focus_pos;
				{150,54},		// bmp_unfocus_pos;
				{270,61},		//bmp_left_pos;
				{360,61},		//bmp_right_pos;
				{155,55},		//string_title_pos;
				{290,54,70,24} //string_content_rect;
			},
			{           //item2;
				{150,78},		// bmp_focus_pos;
				{150,78},		// bmp_unfocus_pos;
				{270,85},		//bmp_left_pos;
				{360,85},		//bmp_right_pos;
				{155,79},		//string_title_pos;
				{290,78,70,24} //string_content_rect;
			}
		}
};

static set_general_para setting_general_uipara_400_240=
{    
		{0,203,400,37},   //toolbar_rect
		{10,140},		  //bmp_big_setting_pos
		{10,219,30,16}, // bmp_prompt_rect;
		{10,217},		//  string_prompt_pos;
		{5,220,400,20},		//RECT  string_detail_prompt_rect;
		{5,240,400,20},		//RECT  string_detail_prompt_rect_ex;
		{	//item0;  cai++
			{
				{140,6},
				{140,6},
				{330,13},
				{450,13},
				{145,6},
				{276,6,84,24}
			},
			{           //item0; //item1
				{140,30},		// bmp_focus_pos;
				{140,30},		// bmp_unfocus_pos;
				{350,37},		//bmp_left_pos;
				{450,37},		//bmp_right_pos;
				{145,30},		//string_title_pos;
				{276,30,84,24} //string_content_rect;
			},
			{           //item1; //item2
				{140,54},		// bmp_focus_pos;
				{140,54},		// bmp_unfocus_pos;
				{350,61},		//bmp_left_pos;
				{450,61},		//bmp_right_pos;
				{145,54},		//string_title_pos;
				{276,54,84,24} //string_content_rect;
			},
			{           //item2;  //item3
				{140,78},		// bmp_focus_pos;
				{140,78},		// bmp_unfocus_pos;
				{350,85},		//bmp_left_pos;
				{450,85},		//bmp_right_pos;
				{145,78},		//string_title_pos;
				{276,78,84,24} //string_content_rect;
			},
			{           //item3;  //item4
				{140,102},		// bmp_focus_pos;
				{140,102},		// bmp_unfocus_pos;
				{350,109},		//bmp_left_pos;
				{450,109},		//bmp_right_pos;
				{145,102},		//string_title_pos;
				{276,102,84,24} //string_content_rect;
			},
			
			{           //item4;  //item5
				{140,126},		// bmp_focus_pos;
				{140,126},		// bmp_unfocus_pos;
				{350,133},		//bmp_left_pos;
				{450,133},		//bmp_right_pos;
				{145,126},		//string_title_pos;
				{276,126,84,24} //string_content_rect;
			},
			{           //item5;  //item6
				{140,150},		// bmp_focus_pos;
				{140,150},		// bmp_unfocus_pos;
				{350,157},		//bmp_left_pos;
				{450,157},		//bmp_right_pos;
				{145,150},		//string_title_pos;
				{276,150,84,24} //string_content_rect;
			},
#if BEETLES_TVOUT_ENABLE
			{           //item6;  //item7
				{140,174},		// bmp_focus_pos;
				{140,174},		// bmp_unfocus_pos;
				{350,181},		//bmp_left_pos;
				{450,181},		//bmp_right_pos;
				{145,174},		//string_title_pos;
				{276,174,84,24} //string_content_rect;
			},
#endif
#if 0//BEETLES_KEYTONE_ENABLE
			{
				{150,198},
				{150,198},
				{270,205},
				{360,205},
				{157,201},
				{276,198,84,24}
			}
#endif
		}
};

static set_power_para setting_power_uipara_400_240=
{    
		{0,203,400,37},   //toolbar_rect
		{11,30},		  //bmp_big_setting_pos
		{10,219,30,16}, // bmp_prompt_rect;
		{45,217},		//  string_prompt_pos;
		{11,115,200,20},		//RECT  string_detail_prompt_rect;
		{11,135,200,20},		//RECT  string_detail_prompt_rect_ex;
		{
			{           //item0;
				{150,30},		// bmp_focus_pos;
				{150,30},		// bmp_unfocus_pos;
				{270,37},		//bmp_left_pos;
				{360,37},		//bmp_right_pos;
				{157,33},		//string_title_pos;
				{276,30,84,24} //string_content_rect;
			},
			{           //item1;
				{150,54},		// bmp_focus_pos;
				{150,54},		// bmp_unfocus_pos;
				{270,61},		//bmp_left_pos;
				{360,61},		//bmp_right_pos;
				{157,57},		//string_title_pos;
				{276,54,84,24} //string_content_rect;
			},
			{           //item2;
				{150,78},		// bmp_focus_pos;
				{150,78},		// bmp_unfocus_pos;
				{270,85},		//bmp_left_pos;
				{360,85},		//bmp_right_pos;
				{157,81},		//string_title_pos;
				{276,78,84,24} //string_content_rect;
			}
		}
};

 set_general_para* setting_get_general_para(SIZE screen_size)
{
	
	if((screen_size.width == 800 )&&( screen_size.height == 480))
		return &setting_general_uipara_800_480;
	else if((screen_size.width == 400 )&&( screen_size.height == 240))
		return &setting_general_uipara_400_240;
	else if((screen_size.width == 480 )&&( screen_size.height == 272))
		return &setting_general_uipara_400_240;
	else
	{
		__msg("ERR: NULL ui para..\n");
		return  &setting_general_uipara_800_480;	
	}		
}
set_power_para* setting_get_power_para(SIZE screen_size )
{
	__s32 			screen_width;
	__s32 			screen_height;
	
	if((screen_size.width == 800 )&&( screen_size.height == 480))
		return &setting_power_uipara_800_480;
	else if((screen_size.width == 400 )&&( screen_size.height == 240))
		return &setting_power_uipara_400_240;
	else if((screen_size.width == 480 )&&( screen_size.height == 272))
		return &setting_power_uipara_400_240;
	else
	{
		__msg("ERR: NULL ui para..\n");
		return  &setting_power_uipara_800_480;	
	}		
}

tips_uipara_t* setting_get_tips_para(SIZE screen_size)
{
	__s32			screen_width;
	__s32			screen_height;
		
	if((screen_size.width == 800 )&&( screen_size.height == 480))
		return &setting_tips_uipara_800_480;
	else if((screen_size.width == 400 )&&( screen_size.height == 240))
		return &setting_tips_uipara_400_240;
	else if((screen_size.width == 480 )&&( screen_size.height == 272))
		return &setting_tips_uipara_400_240;
	else
	{
		__msg("ERR: NULL ui para..\n");
		return	&setting_tips_uipara_400_240;	
	}		
}

void  main_cmd2parent(H_WIN hwin, __s32 id, __s32 data1, __s32 data2)
{
	__gui_msg_t msg;

	msg.h_deswin = GUI_WinGetParent(hwin);
	msg.h_srcwin = NULL;
	msg.id = GUI_MSG_COMMAND;
	msg.dwAddData1 = MAKELONG(GUI_WinGetItemId(hwin), id);
	msg.dwAddData2 = data1;
	msg.dwReserved = data2;

	GUI_SendNotifyMessage(&msg);
}

#endif;


