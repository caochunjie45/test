/*
**************************************************************************************************************
*											         ePDK
*						            the Easy Portable/Player Develop Kits
*									           desktop system 
*
*						        	 (c) Copyright 2007-2010, ANDY, China
*											 All Rights Reserved
*
* File    	: setting_general.c
* By      	: Andy.zhang
* Func		: desk main thread
* Version	: v1.0
* ============================================================================================================
* 2009-7-20 8:51:52  andy.zhang  create this file, implements the fundemental interface;
**************************************************************************************************************
*/

#include "Setting_general.h"
#include "setting_para.h"




typedef enum
{
	BACKLIGHTOFF_NEVER,
	BACKLIGHTOFF_10SECOND,
	BACKLIGHTOFF_30SECOND,
	BACKLIGHTOFF_60SECOND,
	BACKLIGHTOFF_90SECOND,
	BACKLIGHTOFF_MAX,	
}backlightoff_t;

typedef struct tag_setting_item_id
{
	__s32 id_string_detail_prompt;
	__s32 id_string_detail_prompt_ex;
	__s32 id_string_title;
	__s32 id_string_content;
	__s32  id_bmp_left;
	__s32  id_bmp_right;
}setting_item_id_t;

typedef struct tag_setting_item_res
{
	char string_title[128];
	OFFSET string_title_pos;
	
	char string_content[128];
    RECT  string_content_rect;
	
	H_THEME  h_bmp_focus;
	OFFSET   bmp_focus_pos;
	
	H_THEME  h_bmp_unfocus;
	OFFSET   bmp_unfocus_pos;
	
	H_THEME  h_bmp_left;
	OFFSET   bmp_left_pos;
	
	H_THEME  h_bmp_right;
	OFFSET   bmp_right_pos;
	
	__s32 content_num;
	__s32 content_nr;	
	
}setting_item_res_t;

typedef struct tag_setting_focus_item_id
{
	__s32  id_string_detail_prompt;	
	__s32  id_bmp_focus_item;
}setting_focus_item_id_t;

typedef struct tag_setting_general_attr
{
	char string_prompt[128];
	char string_detail_prompt[128];
	char string_detail_prompt_ex[128];
	H_THEME 	h_bmp_big_setting,h_bmp_prompt,h_bmp_bottom;
	setting_item_res_t res_language;
	setting_item_res_t res_topic;
	setting_item_res_t res_product_info;
	setting_item_res_t res_factory;
#if BEETLES_TVOUT_ENABLE
	setting_item_res_t res_output;
#endif
	//setting_item_res_t res_keytone;
	setting_item_res_t res_voice;

	setting_item_res_t	res_backlight_level;
	setting_item_res_t	res_back_light_off;

	__s32	focus_txt_color;
	__s32	unfocus_txt_color;

	H_WIN	layer;
	GUI_FONT *font;

	__s32 new_focus;
	__s32 old_focus;

	__s32 item_nr;
	setting_general_para_t *general_para;
	set_general_para* general_uipara;

	//cai++ for setting modify or not  modify1
	__bool	init_flag;	//cai++ for setting modify flag
	__s32	new_content_num[9];   //cai++ for setiing modify or not
	__s32	original_content_num[9];   //cai++ for setting modify or not
	__bool	setting_content_modify_flag;  //cai++ for setting modify or not
}setting_general_attr_t;

extern void  main_cmd2parent(H_WIN hwin, __s32 id, __s32 data1, __s32 data2);
static void __app_setting_change_to_default_bg(void)//sun++
{    
    reg_init_para_t* para;
    para = dsk_reg_get_para_by_app(REG_APP_INIT);
    if(para)
    {
        para->bgd_flag_in_main = gscene_bgd_get_flag();
        if(EPDK_TRUE == para->bgd_flag_in_main)//当前是默认背景，则不做任何事情
        {                                       
        }
        else//当前不是默认背景，修改为默认背景
        {
            gscene_bgd_set_flag(EPDK_TRUE);
            gscene_bgd_refresh();
        }                                    
    }
    else
    {
        __msg("dsk_reg_get_para_by_app fail...\n");
    }
}

static reg_system_para_t* setting_reg_para;

static __s32 content_lang_id[] = //ITEM1 ( 设置语言)
{
	STRING_SET_COMMON_LANGUAGE_ENGLISH_CON,
	STRING_SET_COMMON_LANGUAGE_CHINESE_CON,
	STRING_SET_COMMON_LANGUAGE_PORTUGUESE_CON,
	STRING_SET_COMMON_LANGUAGE_SPANISH_CON
};

static __s32 content_style_id[] = //ITEM2 (设置主题风格)
{
	STRING_SET_COMMON_TOPIC_STYLE1_CON,
	STRING_SET_COMMON_TOPIC_STYLE2_CON,
	//STRING_SET_COMMON_TOPIC_STYLE3_CON
};

#if BEETLES_TVOUT_ENABLE
static __s32 content_output_id[] = //输出类型
{
	STRING_SET_OUTPUT_NTSC_CON,
	STRING_SET_OUTPUT_PAL_CON
};
#endif


static __s32 content_backlight_level_id[] = //ITEM1 (背光亮度)
{
	STRING_SET_POWER_BGT_LEVEL1_CON,
	STRING_SET_POWER_BGT_LEVEL2_CON,
	STRING_SET_POWER_BGT_LEVEL3_CON,
	STRING_SET_POWER_BGT_LEVEL4_CON,
	STRING_SET_POWER_BGT_LEVEL5_CON,
	STRING_SET_POWER_BGT_LEVEL6_CON,
	STRING_SET_POWER_BGT_LEVEL7_CON,
	STRING_SET_POWER_BGT_LEVEL8_CON,
	STRING_SET_POWER_BGT_LEVEL9_CON,
	STRING_SET_POWER_BGT_LEVEL10_CON,
	STRING_SET_POWER_BGT_LEVEL11_CON,
	STRING_SET_POWER_BGT_LEVEL12_CON,
	STRING_SET_POWER_BGT_LEVEL13_CON,
	STRING_SET_POWER_BGT_LEVEL14_CON,
	STRING_SET_POWER_BGT_LEVEL15_CON
	
};

static __s32 content_backlight_off_id[] = //ITEM2 (关闭背光)  //cai++
{
	STRING_SET_POWER_BGT_NEVER_CON,
	STRING_SET_POWER_BGT_LAST10S_CON,
	STRING_SET_POWER_BGT_LAST30S_CON,
	STRING_SET_POWER_BGT_LAST60S_CON,
	STRING_SET_POWER_BGT_LAST90S_CON
};

static setting_focus_item_id_t setting_focus_item_res_id[] =
{
	{ STRING_SET_COMMON_LANGUAGE_EX,ID_SETTING_SET_ITEM_F_BMP},	// 0
	{ STRING_SET_COMMON_TOPIC_STYLE_EX,ID_SETTING_SET_ITEM_F_BMP},	// 1
	{ STRING_SET_POWER_BGT_EX,ID_SETTING_SET_ITEM_F_BMP},	// 0
	{ STRING_SET_POWER_BGT_OFF_EX,ID_SETTING_SET_ITEM_F_BMP},	// 1
#if BEETLES_TVOUT_ENABLE
	{ STRING_SET_OUTPUT_EX,ID_SETTING_SET_ITEM_F_BMP},	// 4
#endif
	{ STRING_SET_COMMON_PRODUCTINFO_EX,ID_SETTING_SET_ITEM_F_BMP},		// 2
	{ STRING_SET_COMMON_FACTORY_DEFAULT_EX,ID_SETTING_SET_ITEM_F_BMP}	// 3
};

static setting_item_id_t setting_general_res_id[] =
{
	{STRING_SET_COMMON_LANGUAGE_EX,STRING_SET_COMMON_LANGUAGE_DE, STRING_SET_COMMON_LANGUAGE_TITLE,STRING_SET_COMMON_LANGUAGE_CHINESE_CON,ID_SETTING_SET_BUTT_LF_BMP,ID_SETTING_SET_BUTT_RF_BMP},	// 0
	{STRING_SET_COMMON_TOPIC_STYLE_EX,STRING_SET_COMMON_TOPIC_STYLE_DE, STRING_SET_COMMON_TOPIC_STYLE_TITLE,STRING_SET_COMMON_TOPIC_STYLE1_CON,ID_SETTING_SET_BUTT_LF_BMP,ID_SETTING_SET_BUTT_RF_BMP},	// 1
	{STRING_SET_POWER_BGT_EX,STRING_SET_POWER_BGT_DE, STRING_SET_POWER_BGT_TITLE,STRING_SET_POWER_BGT_LEVEL1_CON,ID_SETTING_SET_BUTT_LF_BMP,ID_SETTING_SET_BUTT_RF_BMP},	// cai++ 背光亮度
	{STRING_SET_POWER_BGT_OFF_EX, STRING_SET_POWER_BGT_OFF_DE,STRING_SET_POWER_BGT_OFF_TITLE,STRING_SET_POWER_BGT_NEVER_CON,ID_SETTING_SET_BUTT_LF_BMP,ID_SETTING_SET_BUTT_RF_BMP},	// 1	 //背光开关
#if BEETLES_TVOUT_ENABLE
	{STRING_SET_OUTPUT_EX,STRING_SET_OUTPUT_DE, STRING_SET_OUTPUT_TITLE,STRING_SET_OUTPUT_PAL_CON,ID_SETTING_SET_BUTT_LF_BMP,ID_SETTING_SET_BUTT_RF_BMP},	// 输出
#endif
	{STRING_SET_COMMON_PRODUCTINFO_EX, STRING_SET_COMMON_PRODUCTINFO_DE,STRING_SET_COMMON_PRODUCTINFO_TITLE,NULL,NULL,NULL},		// 2  //cai modify //{STRING_SET_COMMON_PRODUCTINFO_EX, STRING_SET_COMMON_PRODUCTINFO_DE,STRING_SET_COMMON_PRODUCTINFO_TITLE,NULL,NULL,ID_SETTING_SET_BUTT_RF_BMP},
	{STRING_SET_COMMON_FACTORY_DEFAULT_EX,STRING_SET_COMMON_FACTORY_DEFAULT_DE, STRING_SET_COMMON_FACTORY_DEFAULT_TITLE,NULL,NULL,NULL}	// 5  //cai modify  //{STRING_SET_COMMON_FACTORY_DEFAULT_EX,STRING_SET_COMMON_FACTORY_DEFAULT_DE, STRING_SET_COMMON_FACTORY_DEFAULT_TITLE,NULL,NULL,ID_SETTING_SET_BUTT_RF_BMP},
};

static __s32 __setting_switch_output(reg_system_para_t* set_ctrl)  //cai++
{
    //切换输出
		reg_system_para_t* para;
		para = (reg_system_para_t*)dsk_reg_get_para_by_app(REG_APP_SYSTEM);
		__msg("....11......11..............\n");
		if (para)
		{
			__msg("....00....................para->pal_ntsc = %d\n", para->pal_ntsc);
			if (0 == para->pal_ntsc)
			{
				g_display_switch_output(LION_DISP_TV_PAL);						
			}
			else
			{
				g_display_switch_output(LION_DISP_TV_NTSC);						
			}
	}
    
    return EPDK_OK;
}

static void setting_restore_default(__gui_msg_t *msg)
{
    dsk_reg_set_app_restore_all();

	if(setting_reg_para)
	{
		dsk_langres_set_type(setting_reg_para->language);			//语言恢复出厂设置
		//gscene_bgd_set_default_bg_index(setting_reg_para->style);	//恢复默认背景图
		gscene_bgd_refresh();
#if 0//BEETLES_KEYTONE_ENABLE
		dsk_keytone_set_state((__set_keytone_t)(setting_reg_para->keytone));//desktop模块里面1 表示on,2 表示off
		//dsk_keytone_on();
#endif		
		dsk_volume_set(setting_reg_para->volume);	//音量恢复出厂设置
		dsk_display_set_lcd_bright((__lion_bright_t)(setting_reg_para->backlight));	//背光亮度
		g_set_close_scn_time(setting_reg_para->backlightoff);	//自动关屏时间

#if BEETLES_RTC_EXIST
		{
		__date_t factory_date;
		__time_t factory_time;
		factory_date = setting_reg_para->date;
		factory_time = setting_reg_para->time;
		esTIME_SetDate(&factory_date);
		esTIME_SetTime(&factory_time);
		}
#endif		
	}
	setting_general_paint(msg);
}
/*
	申请资源
*/
static void _setting_general_res_init(setting_general_attr_t *general_attr)
{
	__u32 	i; 
	__bool  restore_flag;
	setting_item_res_t  *p_item_res;

	general_attr->h_bmp_big_setting = theme_open(ID_SETTING_SET_BSYSSET_BMP);
	general_attr->h_bmp_prompt = theme_open(ID_SETTING_SET_SETICON00_BMP);	
	general_attr->h_bmp_bottom= theme_open(ID_SETTING_SET_BOTTOM_BMP);		

	general_attr->item_nr = sizeof(setting_general_res_id) /sizeof(setting_general_res_id[0]); 
	
	for (i=0; i<general_attr->item_nr; i++)
	{
		switch (i)
		{
			case 0:
				p_item_res = &general_attr->res_language;
				break;
			case 1:
				p_item_res = &general_attr->res_topic;
				break;
			case 2:
				//p_item_res = &general_attr->res_product_info;
				p_item_res = &general_attr->res_backlight_level;
				break;
			case 3:
				//p_item_res = &general_attr->res_factory;
				p_item_res	= &general_attr->res_back_light_off;
				break;
#if BEETLES_TVOUT_ENABLE
			case 4:
				p_item_res = &general_attr->res_output;
				break;

			case 5:
				p_item_res = &general_attr->res_product_info;
				break;
			case 6:
				p_item_res = &general_attr->res_factory;
				break;
#else
			case 4:
				p_item_res = &general_attr->res_voice;
				break;				
			case 5:
				p_item_res = &general_attr->res_product_info;
				break;
			case 6:
				p_item_res = &general_attr->res_factory;
				break;
#endif				
			default:
				break;
		}

		p_item_res->string_content_rect = general_attr->general_uipara->item[i].item_string_content_rect;
		p_item_res->string_title_pos = general_attr->general_uipara->item[i].item_string_title_pos;

		p_item_res->bmp_left_pos= general_attr->general_uipara->item[i].item_bmp_left_pos;//初始化各图片的放置位置
		p_item_res->bmp_right_pos = general_attr->general_uipara->item[i].item_bmp_right_pos;
		p_item_res->bmp_focus_pos = general_attr->general_uipara->item[i].item_bmp_focus_pos;

		p_item_res->h_bmp_left= theme_open(setting_general_res_id[i].id_bmp_left);
		p_item_res->h_bmp_right= theme_open(setting_general_res_id[i].id_bmp_right);
	}
	
	
	
}

static void _setting_general_res_uninit(setting_general_attr_t *general_attr)
{
    __u32 	i; 
	setting_item_res_t  *p_item_res;
         
	theme_close(general_attr->h_bmp_big_setting);		
	theme_close(general_attr->h_bmp_prompt);			
	theme_close(general_attr->h_bmp_bottom);
	
	for (i=0; i<general_attr->item_nr; i++)
	{
		switch (i)
		{
			case 0:
				p_item_res = &general_attr->res_language;
				break;
			case 1:
				p_item_res = &general_attr->res_topic;
				break;
			case 2:
				//p_item_res = &general_attr->res_product_info;
				p_item_res = &general_attr->res_backlight_level;
				break;
			case 3:
				//p_item_res = &general_attr->res_factory;
				p_item_res = &general_attr->res_back_light_off;
				break;
#if BEETLES_TVOUT_ENABLE
			case 4:
				p_item_res = &general_attr->res_output;
				break;
			case 5:
				p_item_res = &general_attr->res_product_info;
				break;
			case 6:
				p_item_res = &general_attr->res_factory;
				break;
#else
			case 4:
				p_item_res = &general_attr->res_voice;
				break;				
			case 5:
				p_item_res = &general_attr->res_product_info;
				break;
			case 6:
				p_item_res = &general_attr->res_factory;
				break;
#endif
			default:
				break;
		}
		theme_close(p_item_res->h_bmp_focus);	
		theme_close(p_item_res->h_bmp_left);	
		theme_close(p_item_res->h_bmp_right);	
	}
}

static void _setting_general_focus_res_reset(setting_general_attr_t *general_attr)
{

	setting_item_res_t  *p_item_res;

	switch (general_attr->new_focus)
	{
		case 0:
			p_item_res = &general_attr->res_language;
			break;
		case 1:
			p_item_res = &general_attr->res_topic;
			break;
		case 2:
			//p_item_res = &general_attr->res_product_info;
			p_item_res = &general_attr->res_backlight_level;
			break;
		case 3:
			p_item_res = &general_attr->res_back_light_off;
			//p_item_res = &general_attr->res_factory;
			break;
#if BEETLES_TVOUT_ENABLE
		case 4:
			p_item_res = &general_attr->res_output;
			break;
		case 5:
			p_item_res = &general_attr->res_product_info;
			break;
		case 6:
			p_item_res = &general_attr->res_factory;
			break;
#else
		case 4:
			p_item_res = &general_attr->res_voice;
			break;			
		case 5:
			p_item_res = &general_attr->res_product_info;
			break;
		case 6:
			p_item_res = &general_attr->res_factory;
			break;
#endif
		default:
			break;
	}

	get_menu_text(setting_focus_item_res_id[general_attr->new_focus].id_string_detail_prompt, general_attr->string_detail_prompt, 128);
	get_menu_text(setting_general_res_id[general_attr->new_focus].id_string_detail_prompt_ex, general_attr->string_detail_prompt_ex, 128);
	if(p_item_res->h_bmp_focus)
	{
		theme_close(p_item_res->h_bmp_focus);
		p_item_res->h_bmp_focus = theme_open(setting_focus_item_res_id[general_attr->new_focus].id_bmp_focus_item);
	}
	else
	{
		p_item_res->h_bmp_focus = theme_open(setting_focus_item_res_id[general_attr->new_focus].id_bmp_focus_item);
	}
	
}

static void _setting_general_content_res_reset(setting_general_attr_t *general_attr)
{
	
     __u32 	 *content_id; 
	setting_item_res_t  *p_item_res;

	switch (general_attr->new_focus)
	{
		case 0:
			p_item_res = &general_attr->res_language;
			content_id = content_lang_id;
			get_menu_text(content_id[p_item_res->content_num], p_item_res->string_content, 128); ///
			break;
		case 1:
			p_item_res = &general_attr->res_topic;
			content_id = content_style_id;
			get_menu_text(content_id[p_item_res->content_num], p_item_res->string_content, 128); ///
			p_item_res->content_nr = sizeof(content_style_id)/sizeof(__s32);
			break;
		case 2:
			//p_item_res = &general_attr->res_product_info;
			p_item_res = &general_attr->res_backlight_level;
			content_id = content_backlight_level_id;
			get_menu_text(content_id[p_item_res->content_num], p_item_res->string_content, 128);
			p_item_res->content_nr = sizeof(content_backlight_level_id)/sizeof(__s32);
			break;
		case 3:
			//p_item_res = &general_attr->res_factory;
			p_item_res = &general_attr->res_back_light_off;
			content_id = content_backlight_off_id;
			get_menu_text(content_id[p_item_res->content_num], p_item_res->string_content, 128);
			p_item_res->content_nr = sizeof(content_backlight_off_id)/sizeof(__s32);
			break;
#if BEETLES_TVOUT_ENABLE
		case 4:
			p_item_res = &general_attr->res_output;
			content_id = content_output_id;
			get_menu_text(content_id[p_item_res->content_num], p_item_res->string_content, 128); ///
			p_item_res->content_nr = sizeof(content_output_id)/sizeof(__s32);
			break;
		case 5:
			p_item_res = &general_attr->res_product_info;
			break;
		case 6:
			p_item_res = &general_attr->res_product_info;
			break;
#else
		case 4:
			p_item_res = &general_attr->res_voice;
			eLIBs_memset(p_item_res->string_content, 0, 128);
			eLIBs_int2str_dec(p_item_res->content_num, p_item_res->string_content);
			break;
		case 5:
			p_item_res = &general_attr->res_product_info;
			break;
		case 6:
			p_item_res = &general_attr->res_product_info;
			break;

#endif
		default:
			break;
	}

}



static __s32 setting_general_content_paint(__gui_msg_t *msg)
{
	setting_general_attr_t *general_attr;
	setting_item_res_t  *p_item_res;
	RECT rect;
	GUI_RECT gui_rect;

	general_attr = (setting_general_attr_t *)GUI_WinGetAddData(msg->h_deswin);
	if (general_attr==NULL)
		return EPDK_FAIL;
		
	if (GUI_LyrWinGetSta(general_attr->layer) == GUI_LYRWIN_STA_SUSPEND)
		return EPDK_FAIL;

	GUI_LyrWinSel(general_attr->layer);
	GUI_SetFont(general_attr->font);
	GUI_UC_SetEncodeUTF8();
	GUI_SetDrawMode(GUI_DRAWMODE_TRANS);

	GUI_LyrWinCacheOn();
	GUI_OpenAlphaBlend();
	GUI_LyrWinGetScnWindow(general_attr->layer, &rect);

	_setting_general_content_res_reset(general_attr);

	switch (general_attr->new_focus)
	{
		case 0:
			p_item_res = &general_attr->res_language;
			break;
		case 1:
			p_item_res = &general_attr->res_topic;
			break;
		case 2:
			//p_item_res = &general_attr->res_product_info;
			p_item_res = &general_attr->res_backlight_level;
			break;
		case 3:
			//p_item_res = &general_attr->res_factory;
			p_item_res = &general_attr->res_back_light_off;
			break;
#if BEETLES_TVOUT_ENABLE
		case 4:
			p_item_res = &general_attr->res_output;
			break;
		case 5:
			p_item_res = &general_attr->res_product_info;
			break;
		case 6:
			p_item_res = &general_attr->res_factory;
			break;
#else
		case 4:
			p_item_res = &general_attr->res_voice;
			break;
		case 5:
			p_item_res = &general_attr->res_product_info;
			break;
		case 6:
			p_item_res = &general_attr->res_factory;
			break;
#endif
		default:
			break;
	}

	
	GUI_SetBkColor(0x00);	
	GUI_ClearRect(p_item_res->string_content_rect.x,p_item_res->string_content_rect.y,p_item_res->string_content_rect.x+p_item_res->string_content_rect.width,p_item_res->string_content_rect.y+p_item_res->string_content_rect.height);
	
	GUI_SetColor(general_attr->focus_txt_color);
	gui_rect.x0 = SET_CONT_START_X;
	gui_rect.y0 = SET_CONT_START_Y + SET_CONT_H*general_attr->new_focus;
	gui_rect.x1 = gui_rect.x0+ SET_CONT_W;
	gui_rect.y1 = gui_rect.y0+SET_CONT_H;
	
	GUI_BMP_Draw(theme_hdl2buf(p_item_res->h_bmp_focus), p_item_res->bmp_focus_pos.x, p_item_res->bmp_focus_pos.y);

	GUI_BMP_Draw(theme_hdl2buf(p_item_res->h_bmp_left), p_item_res->bmp_left_pos.x, p_item_res->bmp_left_pos.y);
	GUI_BMP_Draw(theme_hdl2buf(p_item_res->h_bmp_right), p_item_res->bmp_right_pos.x, p_item_res->bmp_right_pos.y); 		

	GUI_DispStringAt(p_item_res->string_title, p_item_res->string_title_pos.x, p_item_res->string_title_pos.y);
	GUI_DispStringInRect(p_item_res->string_content, &gui_rect, GUI_TA_HCENTER|GUI_TA_VCENTER);  
	GUI_CloseAlphaBlend();
	GUI_LyrWinCacheOff();
	return EPDK_OK;
}

static __s32 setting_general_item_paint(__gui_msg_t *msg)
{
	setting_general_attr_t *general_attr;
	setting_item_res_t  *p_item_res;
	__s32 i;
	RECT rect;
    GUI_RECT gui_rect;    

	general_attr = (setting_general_attr_t *)GUI_WinGetAddData(msg->h_deswin);
	if (general_attr==NULL)
		return EPDK_FAIL;
		
	if (GUI_LyrWinGetSta(general_attr->layer) == GUI_LYRWIN_STA_SUSPEND)
		return EPDK_FAIL;

	GUI_LyrWinSel(general_attr->layer);
	GUI_SetFont(general_attr->font);
	GUI_UC_SetEncodeUTF8();
	GUI_SetDrawMode(GUI_DRAWMODE_TRANS);
	GUI_LyrWinCacheOn();
	GUI_OpenAlphaBlend();
	GUI_LyrWinGetScnWindow(general_attr->layer, &rect);

	_setting_general_focus_res_reset(general_attr);

	GUI_SetBkColor(0x00);
	GUI_SetColor(general_attr->focus_txt_color);
	GUI_ClearRect(general_attr->general_uipara->string_detail_prompt_rect.x,general_attr->general_uipara->string_detail_prompt_rect.y,general_attr->general_uipara->string_detail_prompt_rect.x+general_attr->general_uipara->string_detail_prompt_rect.width-1,general_attr->general_uipara->string_detail_prompt_rect.y+general_attr->general_uipara->string_detail_prompt_rect.height-1);
	GUI_DispStringAt(general_attr->string_detail_prompt, general_attr->general_uipara->string_detail_prompt_rect.x, general_attr->general_uipara->string_detail_prompt_rect.y);

	gui_rect.x0 = general_attr->general_uipara->string_detail_prompt_rect_ex.x;
	gui_rect.y0 = general_attr->general_uipara->string_detail_prompt_rect_ex.y;
	gui_rect.x1 = gui_rect.x0+general_attr->general_uipara->string_detail_prompt_rect_ex.width;
	gui_rect.y1 = gui_rect.y0+general_attr->general_uipara->string_detail_prompt_rect_ex.height;
	GUI_ClearRectEx(&gui_rect);
	GUI_DispStringInRect(general_attr->string_detail_prompt_ex,&gui_rect,GUI_TA_LEFT|GUI_TA_VCENTER);

	for(i=0; i<general_attr->item_nr; i++)
	{
		switch (i)
		{
			case 0:
				p_item_res = &general_attr->res_language;
				break;
			case 1:
				p_item_res = &general_attr->res_topic;
				break;
			case 2:
				//p_item_res = &general_attr->res_product_info;
				p_item_res = &general_attr->res_backlight_level;
				break;
			case 3:
				//p_item_res = &general_attr->res_factory;
				p_item_res = &general_attr->res_back_light_off;
				break;
#if BEETLES_TVOUT_ENABLE
			case 4:
				p_item_res = &general_attr->res_output;
				break;
			case 5:
				p_item_res = &general_attr->res_product_info;
				break;
			case 6:
				p_item_res = &general_attr->res_factory;
				break;
#else
			case 4:
				p_item_res = &general_attr->res_voice;
				break;
			case 5:
				p_item_res = &general_attr->res_product_info;
				break;
			case 6:
				p_item_res = &general_attr->res_factory;
				break;

#endif
			default:
				break;
		}

		if(i == general_attr->new_focus)
		{
			setting_general_content_paint( msg);
		}
		else if(i == general_attr->old_focus)
		{
			gui_rect.x0 = SET_ITEM_START_X;
			gui_rect.y0 = SET_ITEM_START_Y + SET_ITEM_H*i;
			gui_rect.x1 = gui_rect.x0+SET_ITEM_W;
			gui_rect.y1 = gui_rect.y0+SET_ITEM_H;
			GUI_ClearRectEx(&gui_rect);
			GUI_SetColor(general_attr->unfocus_txt_color);
			GUI_DispStringAt(p_item_res->string_title, p_item_res->string_title_pos.x, p_item_res->string_title_pos.y);

			gui_rect.x0 = SET_CONT_START_X;
			gui_rect.y0 = SET_CONT_START_Y + SET_CONT_H*i;
			gui_rect.x1 = gui_rect.x0+ SET_CONT_W;
			gui_rect.y1 = gui_rect.y0+SET_CONT_H;
			GUI_DispStringInRect(p_item_res->string_content, &gui_rect, GUI_TA_VCENTER|GUI_TA_HCENTER);    
		}
	}
	GUI_CloseAlphaBlend();
	GUI_LyrWinCacheOff();
	return EPDK_OK;
}

static __s32 setting_general_paint(__gui_msg_t *msg)
{
	setting_general_attr_t *general_attr;
	setting_item_res_t  *p_item_res;
	__s32 i;
	GUI_RECT gui_rect;
	RECT rect;

	general_attr = (setting_general_attr_t *)GUI_WinGetAddData(msg->h_deswin);
	if (general_attr==NULL)
		return EPDK_FAIL;
		
	if (GUI_LyrWinGetSta(general_attr->layer) == GUI_LYRWIN_STA_SUSPEND)
		return EPDK_FAIL;
	
	GUI_LyrWinSel(general_attr->layer);
	GUI_SetFont(general_attr->font);
	GUI_UC_SetEncodeUTF8();
	GUI_SetDrawMode(GUI_DRAWMODE_TRANS);
	
	GUI_LyrWinCacheOn();
	GUI_OpenAlphaBlend();

	get_menu_text(STRING_SET_PROMPT_COMMON, general_attr->string_prompt, 128);//set_common(通用设置)
	get_menu_text(setting_general_res_id[general_attr->new_focus].id_string_detail_prompt, general_attr->string_detail_prompt, 128);
	get_menu_text(setting_general_res_id[general_attr->new_focus].id_string_detail_prompt_ex, general_attr->string_detail_prompt_ex, 128);

	for (i=0; i<general_attr->item_nr; i++)
	{
		switch (i)
		{
			case 0:
				p_item_res = &general_attr->res_language;
				if(setting_reg_para)
				{
					if(setting_reg_para->language == EPDK_LANGUAGE_ENM_ENGLISH)
						p_item_res->content_num = 0;
					else if(setting_reg_para->language == EPDK_LANGUAGE_ENM_CHINESES)
						p_item_res->content_num = 1;					
					else if(setting_reg_para->language == EPDK_LANGUAGE_ENM_PORTUGUESE)
						p_item_res->content_num = 2;
					else if(setting_reg_para->language == EPDK_LANGUAGE_ENM_SPANISH)
						p_item_res->content_num = 3;
					else
						p_item_res->content_num = 0;
				}
				else
				{
					p_item_res->content_num = 0;
				}

				p_item_res->content_nr = sizeof(content_lang_id)/sizeof(__s32);
				get_menu_text(content_lang_id[p_item_res->content_num], p_item_res->string_content, 128);

				if(general_attr->init_flag)   //cai++ for setting modify or not
					general_attr->original_content_num[0] = p_item_res->content_num;
				else
					general_attr->new_content_num[0] = p_item_res->content_num;
				break;
			case 1:
				p_item_res = &general_attr->res_topic;
				if(setting_reg_para)
				{
					p_item_res->content_num = setting_reg_para->style; 
					if(p_item_res->content_num > 2)
						p_item_res->content_num = 0;
				}
				else
				{
					p_item_res->content_num = 0;
				}

				p_item_res->content_nr = sizeof(content_style_id)/sizeof(__s32);
				get_menu_text(content_style_id[p_item_res->content_num], p_item_res->string_content, 128);
				
				if(general_attr->init_flag)   //cai++ for setting modify or not
					general_attr->original_content_num[1] = p_item_res->content_num;
				else
					general_attr->new_content_num[1] = p_item_res->content_num;
				break;
			case 2:
				//p_item_res = &general_attr->res_product_info;
				p_item_res = &general_attr->res_backlight_level;
				if(setting_reg_para)
				{
					p_item_res->content_num = setting_reg_para->backlight; 
					if(p_item_res->content_num > 14)
						p_item_res->content_num = 0;
				}
				else
				{
					p_item_res->content_num = 0;
				}

				p_item_res->content_nr = sizeof(content_backlight_level_id)/sizeof(__s32);
				get_menu_text(content_backlight_level_id[p_item_res->content_num], p_item_res->string_content, 128);
				
				if(general_attr->init_flag)
					general_attr->original_content_num[2] = p_item_res->content_num;
				else
					general_attr->new_content_num[2] = p_item_res->content_num;
				break;
			case 3:
				//p_item_res = &general_attr->res_factory;
				p_item_res = &general_attr->res_back_light_off;
				if(setting_reg_para)
				{
					switch(setting_reg_para->backlightoff)
					{
					case 0:
						p_item_res->content_num = 0;
						break;
					case 10:
						p_item_res->content_num = 1;
						break;
					case 30:
						p_item_res->content_num = 2;
						break;
					case 60:
						p_item_res->content_num = 3;
						break;
					case 90:
						p_item_res->content_num = 4;
						break;
					default:
						p_item_res->content_num = 0;
						break;
					}
				}
				else
				{
					p_item_res->content_num = 0;
				}

				p_item_res->content_nr = sizeof(content_backlight_off_id)/sizeof(__s32);
				get_menu_text(content_backlight_off_id[p_item_res->content_num], p_item_res->string_content, 128);
				
				if(general_attr->init_flag)
					general_attr->original_content_num[3] = p_item_res->content_num;
				else
					general_attr->new_content_num[3] = p_item_res->content_num;				
				break;
#if BEETLES_TVOUT_ENABLE
			case 4:
				p_item_res = &general_attr->res_output;
				if(setting_reg_para)
				{
					p_item_res->content_num = setting_reg_para->pal_ntsc; //0 ntsc , 1 pal
					if(p_item_res->content_num > 1)
						p_item_res->content_num = 0;
				}
				else
				{
					p_item_res->content_num = 0;
				}

				p_item_res->content_nr = sizeof(content_output_id)/sizeof(__s32);
				get_menu_text(content_output_id[p_item_res->content_num], p_item_res->string_content, 128);
				
				if(general_attr->init_flag)   //cai++ for setting modify or not
					general_attr->original_content_num[4] = p_item_res->content_num;
				else
					general_attr->new_content_num[4] = p_item_res->content_num;
				break;
			case 5:
				p_item_res = &general_attr->res_product_info;
				if(general_attr->init_flag)
				{
					general_attr->original_content_num[5] = general_attr->new_content_num[5] =0;
				}
				break;
			case 6:
				p_item_res = &general_attr->res_factory;
				if(general_attr->init_flag)   //cai++ for setting modify or not
				{
					general_attr->original_content_num[6] = general_attr->new_content_num[6] =0;
				}
				break;
#else
			case 4:
				p_item_res = &general_attr->res_voice;
				p_item_res->content_num = dsk_volume_get();
				eLIBs_memset(p_item_res->string_content, 0, 128);
				eLIBs_int2str_dec(p_item_res->content_num, p_item_res->string_content);
				
				if(general_attr->init_flag)   //cai++ for setting modify or not
					general_attr->original_content_num[6] = p_item_res->content_num;
				else
					general_attr->new_content_num[6] = p_item_res->content_num;
				break;
			case 5:
				p_item_res = &general_attr->res_product_info;
				if(general_attr->init_flag)   //cai++ for setting modify or not
				{
					general_attr->original_content_num[7] = general_attr->new_content_num[7] =0;
				}
				break;
			case 6:
				p_item_res = &general_attr->res_factory;
				if(general_attr->init_flag)   //cai++ for setting modify or not
				{
					general_attr->original_content_num[8] = general_attr->new_content_num[8] =0;
				}
				break;

#endif
			default:
				break;
		}
		get_menu_text(setting_general_res_id[i].id_string_title, p_item_res->string_title, 128);

	}
	general_attr->init_flag = 0;//cai++ for setting modify or not
	
	GUI_LyrWinGetScnWindow(general_attr->layer, &rect);

	GUI_BMP_Draw(theme_hdl2buf(general_attr->h_bmp_big_setting), general_attr->general_uipara->bmp_big_setting_pos.x, general_attr->general_uipara->bmp_big_setting_pos.y);
	GUI_SetBkColor(0x00);
	//GUI_BMP_Draw(theme_hdl2buf(general_attr->h_bmp_bottom), general_attr->general_uipara->toolbar_rect.x,general_attr->general_uipara->toolbar_rect.y);
	//此处注释掉的三处是设置的下方的横条和文字通用设置
	//GUI_BMP_Draw(theme_hdl2buf(general_attr->h_bmp_prompt), general_attr->general_uipara->bmp_prompt_rect.x, general_attr->general_uipara->bmp_prompt_rect.y);
	GUI_SetColor(general_attr->unfocus_txt_color);
	//GUI_DispStringAt(general_attr->string_prompt, general_attr->general_uipara->string_prompt_pos.x, general_attr->general_uipara->string_prompt_pos.y);
	
	gui_rect.x0 = general_attr->general_uipara->string_detail_prompt_rect.x;
	gui_rect.y0 = general_attr->general_uipara->string_detail_prompt_rect.y;
	gui_rect.x1 = gui_rect.x0+general_attr->general_uipara->string_detail_prompt_rect.width;
	gui_rect.y1 = gui_rect.y0+general_attr->general_uipara->string_detail_prompt_rect.height;
	GUI_ClearRectEx(&gui_rect);
	GUI_DispStringInRect(general_attr->string_detail_prompt,&gui_rect,GUI_TA_LEFT);

	gui_rect.x0 = general_attr->general_uipara->string_detail_prompt_rect_ex.x;
	gui_rect.y0 = general_attr->general_uipara->string_detail_prompt_rect_ex.y;
	gui_rect.x1 = gui_rect.x0+general_attr->general_uipara->string_detail_prompt_rect_ex.width;
	gui_rect.y1 = gui_rect.y0+general_attr->general_uipara->string_detail_prompt_rect_ex.height;
	GUI_ClearRectEx(&gui_rect);
	GUI_DispStringInRect(general_attr->string_detail_prompt_ex,&gui_rect,GUI_TA_LEFT);

	for(i=0; i<general_attr->item_nr; i++)
	{
		switch (i)
		{
			case 0:
				p_item_res = &general_attr->res_language;
				break;
			case 1:
				p_item_res = &general_attr->res_topic;
				break;
			case 2:
				//p_item_res = &general_attr->res_product_info;
				p_item_res = &general_attr->res_backlight_level;
				break;
			case 3:
				//p_item_res = &general_attr->res_factory;
				p_item_res = &general_attr->res_back_light_off;
				break;
#if BEETLES_TVOUT_ENABLE
			case 4:
				p_item_res = &general_attr->res_output;
				break;
			case 5:
				p_item_res = &general_attr->res_product_info;
				break;
			case 6:
				p_item_res = &general_attr->res_factory;
				break;
#else
			case 4:
				p_item_res = &general_attr->res_voice;
				break;
			case 5:
				p_item_res = &general_attr->res_product_info;
				break;
			case 6:
				p_item_res = &general_attr->res_factory;
				break;
#endif
			default:
				break;
		}

		if(i == general_attr->new_focus)
		{ 
			setting_general_content_paint(msg);
		}
		else
		{
			gui_rect.x0 = SET_ITEM_START_X;
			gui_rect.y0 = SET_ITEM_START_Y + SET_ITEM_H*i;
			gui_rect.x1 = gui_rect.x0+SET_ITEM_W;
			gui_rect.y1 = gui_rect.y0+SET_ITEM_H;
			GUI_ClearRectEx(&gui_rect);

			GUI_SetColor(general_attr->unfocus_txt_color);
			GUI_DispStringAt(p_item_res->string_title, p_item_res->string_title_pos.x, p_item_res->string_title_pos.y);

			gui_rect.x0 = SET_CONT_START_X;
			gui_rect.y0 = SET_CONT_START_Y + SET_CONT_H*i;
			gui_rect.x1 = gui_rect.x0+ SET_CONT_W;
			gui_rect.y1 = gui_rect.y0+SET_CONT_H;
			GUI_DispStringInRect(p_item_res->string_content, &gui_rect, GUI_TA_HCENTER|GUI_TA_VCENTER);    
		}

	}
	GUI_CloseAlphaBlend();
	GUI_LyrWinCacheOff();
	return EPDK_OK;
}

static __s32  setting_general_key_proc(__gui_msg_t *msg)
{
	setting_general_attr_t *general_attr;
	static __s32 last_key = -1;
	general_attr = (setting_general_attr_t *)GUI_WinGetAddData(msg->h_deswin);

	if (KEY_UP_ACTION == msg->dwAddData2)
	{
		if (GUI_MSG_KEY_ENTER == last_key)
		{
			main_cmd2parent(msg->h_deswin, ID_OP_ENTER, general_attr->new_focus, 0);
		}
	}
	else
	{
		switch( msg->dwAddData1 )
		{

		case GUI_MSG_KEY_LEFT:		//映射为up
		case GUI_MSG_KEY_LONGLEFT:
		case GUI_MSG_KEY_IR_LEFT:
		case GUI_MSG_KEY_LONGIR_LEFT:
			{
				main_cmd2parent(msg->h_deswin, ID_OP_UP, general_attr->new_focus, 0);
			}
			break;
		case GUI_MSG_KEY_RIGHT:		//映射为down
		case GUI_MSG_KEY_LONGRIGHT:
		case GUI_MSG_KEY_IR_RIGHT:
		case GUI_MSG_KEY_LONGIR_RIGHT:
			{
				main_cmd2parent(msg->h_deswin, ID_OP_DOWN, general_attr->new_focus, 0);
			}
			break;
		case GUI_MSG_KEY_UP:
		case GUI_MSG_KEY_LONGUP:
		case GUI_MSG_KEY_IR_UP:
		case GUI_MSG_KEY_LONGIR_UP:
			main_cmd2parent(msg->h_deswin, ID_OP_UP, general_attr->new_focus, 0);
			break;
		case GUI_MSG_KEY_DOWN:
		case GUI_MSG_KEY_LONGDOWN:
		case GUI_MSG_KEY_IR_DOWN:
		case GUI_MSG_KEY_LONGIR_DOWN:
			main_cmd2parent(msg->h_deswin, ID_OP_DOWN, general_attr->new_focus, 0);
			break;	
		case GUI_MSG_KEY_ESCAPE:
			main_cmd2parent(msg->h_deswin, ID_OP_EXIT, general_attr->new_focus, 0);
			break;
		default:
			break;
			
		}
	}	
	
	if (KEY_UP_ACTION == msg->dwAddData2)
	{
		last_key = -1;	
	}
	else
	{
		last_key = msg->dwAddData1; 
	}

	return EPDK_OK;
}

/*
	回调
*/		
static __s32 _setting_general_Proc(__gui_msg_t *msg)
{
	switch(msg->id)
	{
	case GUI_MSG_CREATE:
		{
			setting_general_attr_t *general_attr;
			setting_general_para_t *general_para;
			SIZE screen_size;
			
			setting_reg_para = (reg_system_para_t*)dsk_reg_get_para_by_app(REG_APP_SYSTEM);
			general_para = (setting_general_para_t *)GUI_WinGetAttr(msg->h_deswin);
			general_attr = (setting_general_attr_t *)My_Balloc(sizeof(setting_general_attr_t));

			//__msg("------backlight---------%d\n",setting_reg_para->backlight);
			if(!general_attr)
			{
				__msg("memory balloc fail.........\n");
				return EPDK_FAIL;
			}
			eLIBs_memset(general_attr, 0, sizeof(setting_general_attr_t));

			dsk_display_get_size(&screen_size.width,&screen_size.height);
			general_attr->general_uipara = setting_get_general_para(screen_size);
			
			general_attr->new_focus = general_para->focus_id;
			general_attr->old_focus = general_para->focus_id;
			general_attr->general_para = general_para;
			_setting_general_res_init(general_attr);
			_setting_general_focus_res_reset(general_attr);
			_setting_general_content_res_reset(general_attr);

			general_attr->focus_txt_color = APP_COLOR_YELLOW;
			general_attr->unfocus_txt_color = APP_COLOR_WHITE;
			general_attr->font = general_para->font;
			general_attr->layer = general_para->layer;

			general_attr->init_flag	= 1; //cai++ for setting modify or not
			GUI_WinSetAddData(msg->h_deswin, (__u32)general_attr);
			GUI_LyrWinSetSta(general_attr->layer, GUI_LYRWIN_STA_ON);
		}
		return EPDK_OK;
	case GUI_MSG_CLOSE:
		{
			GUI_FrmWinDelete(msg->h_deswin);
		}
		return EPDK_OK;
	case GUI_MSG_DESTROY:
		{
			setting_general_attr_t *general_attr;
			setting_general_para_t *general_para;
			general_attr = (setting_general_attr_t *)GUI_WinGetAddData(msg->h_deswin);
			if(!general_attr)
			{
				__err("general_attr is null ......\n");
				return EPDK_FAIL;
			}
			_setting_general_res_uninit(general_attr);

			//dsk_reg_flush();	//写进文件里面
			//判断是否general setting是否修改//cai++
			if(general_attr->new_content_num[0] != general_attr->original_content_num[0]
			 || general_attr->new_content_num[1] != general_attr->original_content_num[1]
			 || general_attr->new_content_num[2] != general_attr->original_content_num[2]
			 || general_attr->new_content_num[3] != general_attr->original_content_num[3]
			 || general_attr->new_content_num[4] != general_attr->original_content_num[4]
			 || general_attr->new_content_num[5] != general_attr->original_content_num[5]
			 || general_attr->new_content_num[6] != general_attr->original_content_num[6])
			{
			__msg("general setting has modify\n");
				dsk_reg_flush();
			}
	
			
			general_para = general_attr->general_para;
			if(general_para)
			{
				My_Bfree(general_para, sizeof(setting_general_para_t));
			}
			if(general_attr)
			{
				My_Bfree(general_attr, sizeof(setting_general_attr_t));
			}
			
		}
		return EPDK_OK;
	case GUI_MSG_PAINT:
		setting_general_paint(msg);
		return EPDK_OK;
	case GUI_MSG_KEY:
		setting_general_key_proc(msg);
		return EPDK_OK;
	case GUI_MSG_TOUCH:
		return EPDK_OK;
	case MSG_OP_UP:
		{
			setting_general_attr_t *general_attr;
			general_attr = (setting_general_attr_t *)GUI_WinGetAddData(msg->h_deswin);
			general_attr->old_focus = general_attr->new_focus;
			if (general_attr->new_focus <= 0)
				general_attr->new_focus = general_attr->item_nr-1;
			else
				general_attr->new_focus--;
			setting_general_item_paint( msg);
		}
		return EPDK_OK;
	case MSG_OP_DOWN:
		{
			setting_general_attr_t *general_attr;
			general_attr = (setting_general_attr_t *)GUI_WinGetAddData(msg->h_deswin);

			general_attr->old_focus = general_attr->new_focus;
			if (general_attr->new_focus >= general_attr->item_nr-1)
				general_attr->new_focus = 0;
			else
				general_attr->new_focus++;
			setting_general_item_paint( msg);
		}
		return EPDK_OK;
	case MSG_OP_LEFT:
		{
			setting_general_attr_t *general_attr;
			general_attr = (setting_general_attr_t *)GUI_WinGetAddData(msg->h_deswin);
			general_attr->old_focus = general_attr->new_focus;
			if (general_attr->new_focus <= 0)
				general_attr->new_focus = general_attr->item_nr-1;
			else
				general_attr->new_focus--;
			setting_general_item_paint( msg);
		}
		return EPDK_OK;
	case MSG_OP_RIGHT:
		{
			setting_general_attr_t *general_attr;
			general_attr = (setting_general_attr_t *)GUI_WinGetAddData(msg->h_deswin);

			general_attr->old_focus = general_attr->new_focus;
			if (general_attr->new_focus >= general_attr->item_nr-1)
				general_attr->new_focus = 0;
			else
				general_attr->new_focus++;
			setting_general_item_paint( msg);
		}
		return EPDK_OK;
		
	case MSG_OP_ENTER:	
		{
			setting_general_attr_t *general_attr;
			setting_item_res_t  *p_item_res;
			
			general_attr = (setting_general_attr_t *)GUI_WinGetAddData(msg->h_deswin);
			switch (general_attr->new_focus)
			{
				case 0:
				{
					p_item_res = &general_attr->res_language;
					if (p_item_res->content_num >= p_item_res->content_nr-1)
						p_item_res->content_num = 0;
					else
						p_item_res->content_num++;

					if(setting_reg_para)
					{
						if(p_item_res->content_num == 0)
							setting_reg_para->language = EPDK_LANGUAGE_ENM_ENGLISH;
						if(p_item_res->content_num == 1)
							setting_reg_para->language = EPDK_LANGUAGE_ENM_CHINESES;
						else if(p_item_res->content_num == 2)
							setting_reg_para->language = EPDK_LANGUAGE_ENM_PORTUGUESE;
						else if(p_item_res->content_num == 3)
							setting_reg_para->language = EPDK_LANGUAGE_ENM_SPANISH;
						else
							setting_reg_para->language = EPDK_LANGUAGE_ENM_ENGLISH;
						dsk_langres_set_type(setting_reg_para->language);
						setting_general_paint(msg);
					}
					general_attr->new_content_num[0] = p_item_res->content_num;
				}
				break;
				
				case 1:
					{
						p_item_res = &general_attr->res_topic;
						__app_setting_change_to_default_bg();//sun++ for 回复默认，设置默认背景
						if (p_item_res->content_num >= p_item_res->content_nr-1)
							p_item_res->content_num = 0;
						else
							p_item_res->content_num++;

						setting_general_content_paint( msg);

						if(setting_reg_para)
						{
							__s32	bg_index;
							bg_index = gscene_bgd_get_default_bg_index();

							if(bg_index!=p_item_res->content_num)
								gscene_bgd_set_default_bg_index(p_item_res->content_num);

							setting_reg_para->style = p_item_res->content_num;
						}
						general_attr->new_content_num[1] = p_item_res->content_num;
					}
					break;
				case 2:
					{
						//main_cmd2parent(msg->h_deswin,ID_SWITCH_TO_TIPS,TIPS_PRODUCT_INFO,0);

						p_item_res = &general_attr->res_backlight_level;
						if (p_item_res->content_num >= p_item_res->content_nr-1)
							p_item_res->content_num = 0;
						else
							p_item_res->content_num++;
						
						setting_general_content_paint(msg);

						if(setting_reg_para)
						{
							setting_reg_para->backlight = p_item_res->content_num;
						}
						
						
						dsk_display_set_lcd_bright((__lion_bright_t)(p_item_res->content_num));

						general_attr->new_content_num[2] = p_item_res->content_num;
					}
					break;
				case 3:				//
					{
						__u32 close_scn_time;
						p_item_res = &general_attr->res_back_light_off;
						if (p_item_res->content_num >= p_item_res->content_nr-1)
							p_item_res->content_num = 0;
						else
							p_item_res->content_num++;
						
						setting_general_content_paint(msg);
						switch(p_item_res->content_num)
						{
							case BACKLIGHTOFF_NEVER:
								g_set_close_scn_time(0);
								close_scn_time = 0;
								break;
							case BACKLIGHTOFF_10SECOND:
								g_set_close_scn_time(10);//10s
								close_scn_time = 10;
								break;
							case BACKLIGHTOFF_30SECOND:
								g_set_close_scn_time(30);// 30s
								close_scn_time = 30;
								break;
							case BACKLIGHTOFF_60SECOND:
								g_set_close_scn_time(60);
								close_scn_time = 60;
								break;	
							case BACKLIGHTOFF_90SECOND:
								g_set_close_scn_time(90);
								close_scn_time = 90;
								break;	
							default:
								g_set_close_scn_time(0);
								close_scn_time = 0;
						}
						if(setting_reg_para)
						{
							setting_reg_para->backlightoff = close_scn_time;
						}
						general_attr->new_content_num[3] = p_item_res->content_num;
					}
					break;	
#if BEETLES_TVOUT_ENABLE
				case 4:
					{
						p_item_res = &general_attr->res_output;
						if (p_item_res->content_num >= p_item_res->content_nr-1)
							p_item_res->content_num = 0;
						else
							p_item_res->content_num++;

						setting_general_content_paint( msg);
						//__setting_switch_output(setting_reg_para);	//cai++		
						if(setting_reg_para)
							setting_reg_para->pal_ntsc = p_item_res->content_num;

					}
				      general_attr->new_content_num[4] = p_item_res->content_num;
					break;


				case 5:
					{
						main_cmd2parent(msg->h_deswin,ID_SWITCH_TO_TIPS,TIPS_PRODUCT_INFO,0);
					}
					break;
				case 6:		//恢复出厂设置
					{
						main_cmd2parent(msg->h_deswin,ID_SWITCH_TO_TIPS,TIPS_FACTORY_DEFAULT,0);
					}
					break;
#else
				case 4:
					{
						p_item_res = &general_attr->res_voice;

						p_item_res->content_num++;
						
						if (p_item_res->content_num > AUDIO_DEVICE_VOLUME_MAX)
							p_item_res->content_num = 0;
						setting_general_content_paint( msg);

						dsk_volume_set( p_item_res->content_num );
						//if(setting_reg_para)
						//	setting_reg_para->volume= p_item_res->content_num+1;
						
					}
					 general_attr->new_content_num[6] = p_item_res->content_num;
					break;					
				case 5:
					{
						main_cmd2parent(msg->h_deswin,ID_SWITCH_TO_TIPS,TIPS_PRODUCT_INFO,0);
					}
					break;
				case 6:		//恢复出厂设置
					{
						main_cmd2parent(msg->h_deswin,ID_SWITCH_TO_TIPS,TIPS_FACTORY_DEFAULT,0);
					}
					break;

#endif
				default:
					break;
			}
							
		}
		return EPDK_OK;
	case MSG_RESTORE_FACTORY:
		setting_restore_default(msg);
		dsk_reg_flush();  //cai++ for write to nor
		return EPDK_OK;

	case MSG_OP_EXIT:
		{
			__gui_msg_t my_msg;
			my_msg.id = GUI_MSG_CLOSE;
    		my_msg.h_srcwin = msg->h_deswin;
    		my_msg.h_deswin = GUI_WinGetParent(msg->h_deswin);
    		my_msg.dwAddData1 = 0;
    		my_msg.dwAddData2 = 0;
    		GUI_SendNotifyMessage(&my_msg);
		}
		return EPDK_OK;		
		
	case GUI_MSG_COMMAND:
		break;

	default:
		break;
	}
	
	return GUI_FrmWinDefaultProc(msg);	
}

H_WIN setting_general_win_create(H_WIN h_parent, setting_general_para_t *para)
{
	__gui_framewincreate_para_t framewin_para;
	setting_general_para_t  *general_para;
	FB fb;

	GUI_LyrWinGetFB(para->layer, &fb);

	general_para = (setting_general_para_t *)My_Balloc(sizeof(setting_general_para_t));
	if(!general_para)
	{
		__msg("memory balloc fail.........\n");
		return EPDK_FAIL;
	}
	eLIBs_memset((void*)general_para, 0, sizeof(setting_general_para_t));
	general_para->font = para->font;
	general_para->layer = para->layer;
	general_para->focus_id = para->focus_id;
	eLIBs_memset(&framewin_para, 0, sizeof(__gui_framewincreate_para_t));
	framewin_para.name =	"general setting win",
	framewin_para.dwExStyle = WS_EX_NONE;
	framewin_para.dwStyle = WS_NONE|WS_VISIBLE;
	framewin_para.spCaption =  NULL;
	framewin_para.hOwner	= NULL;
	framewin_para.id         = GENERAL_MENU_ID;
	framewin_para.hHosting = h_parent;
	framewin_para.FrameWinProc = (__pGUI_WIN_CB)esKRNL_GetCallBack((__pCBK_t)_setting_general_Proc);
	framewin_para.rect.x =0;
	framewin_para.rect.y = 0;
	framewin_para.rect.width= fb.size.width;
	framewin_para.rect.height = fb.size.height;
	framewin_para.BkColor.alpha =  0;
	framewin_para.BkColor.red = 0;
	framewin_para.BkColor.green = 0;
	framewin_para.BkColor.blue = 0;
	framewin_para.attr =  (void *)general_para;
	framewin_para.hLayer = para->layer;

	return (GUI_FrmWinCreate(&framewin_para));
}


__s32 setting_general_win_delete(H_WIN list_win)
{
	GUI_FrmWinDelete(list_win);
	return EPDK_OK;
}

