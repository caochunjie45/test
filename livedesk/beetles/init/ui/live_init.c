/*
**************************************************************************************************************
*											         ePDK
*						            the Easy Portable/Player Develop Kits
*									           desktop system 
*
*						        	 (c) Copyright 2007-2010, ANDY, China
*											 All Rights Reserved
*
* File    	: live_init.c
* By      	: Andy.zhang
* Func		: Live main res init 
* Version	: v1.0
* ============================================================================================================
* 2009-7-20 8:51:52  andy.zhang  create this file, implements the fundemental interface;
**************************************************************************************************************
*/

#include "mod_init_i.h"
#include "live_init.h"

#define LANG_DIR			BEETLES_APP_ROOT"apps\\lang.bin"
#define THEME_DIR			BEETLES_APP_ROOT"apps\\theme.bin"

/**************************************************************************************************************/
GUI_FONT    *SWFFont = NULL;		// ����������

__s32 		LCD_WIDTH;
__s32 		LCD_HEIGHT;

/**************************************************************************************************************/

/**************************************************************************************************************/

/**
 * ��ʼ�� ϵͳ��Դ
 */
__s32 live_init(void)
{		
	__s32 ret;
	reg_system_para_t* para;

	dsk_set_fs_charset(EPDK_CHARSET_ENM_UTF8);
	dsk_set_isn_charset(EPDK_CHARSET_ENM_GBK);
	//create font
	SWFFont = GUI_SFT_CreateFont(14, BEETLES_APP_ROOT"res\\fonts\\font14.sft");
	if(!SWFFont)
	{
		__msg("CreateFont fail, set default font....\n");
		SWFFont = GUI_GetDefaultFont();
	}	
	__here__;
	//initialize walkman/keytone 

	dsk_wkm_init(DSK_WKM_MODE_AUDIO_MIN);	
#if BEETLES_KEYTONE_ENABLE
	dsk_keytone_init(BEETLES_APP_ROOT"res\\sounds\\chord.wav");		
#endif

	//init reg
	ret = dsk_reg_init_para();
	if(EPDK_FAIL == ret)
	{
		__msg("dsk_reg_init_para fail...\n");
	}

	//initialize lang/theme 
	dsk_langres_init(LANG_DIR);
	dsk_theme_init(THEME_DIR);	

	para = (reg_system_para_t*)dsk_reg_get_para_by_app(REG_APP_SYSTEM);
	if(para)
	{
		dsk_display_set_lcd_bright(para->backlight);		
		__msg("para->backlight=%d\n", para->backlight);
		dsk_display_set_panel_bright(para->brightness);
		__here__;
		dsk_display_set_lcd_constract(para->constract);
		__here__;
		dsk_display_set_lcd_saturation(para->saturation);
		__here__;
		dsk_display_set_lcd_hue(para->hue);

		dsk_volume_set(para->volume);	
		__msg("para->volume=%d\n", para->volume);

		g_set_close_scn_time(para->backlightoff);
		__msg("para->backlightoff=%d\n", para->backlightoff);

		//dsk_set_auto_off_time(para->poweroff);
		//__msg("para->poweroff=%d\n", para->poweroff);

		dsk_langres_set_type(para->language);
		__msg("para->language=%d\n", para->language);
		para->output = LION_DISP_LCD;   

#if BEETLES_KEYTONE_ENABLE
		if(1 == para->keytone)
		{
			dsk_keytone_set_state(SET_KEYTONE_ON);
		}
		else
		{
			dsk_keytone_set_state(SET_KEYTONE_OFF);
		}
#endif

#if BEETLES_RTC_EXIST
		if(1 == para->time_set)		//������ʱ������;
		{
			esTIME_SetTime(&para->time);
			esTIME_SetDate(&para->date);
			para->time_set = 0;
		}
#endif
		__here__;
	}
	else
	{
		dsk_display_set_lcd_bright(LION_BRIGHT_LEVEL10);
		__here__;
		dsk_display_set_panel_bright(50);
		__here__;
		dsk_display_set_lcd_constract(60);
		__here__;
		dsk_display_set_lcd_saturation(60);
		__here__;
		dsk_display_set_lcd_hue(90);
		__here__;
		
		dsk_volume_set(25);	
		//dsk_set_auto_off_time(0);
		g_set_close_scn_time(30);
		dsk_langres_set_type(EPDK_LANGUAGE_ENM_ENGLISH);
#if BEETLES_KEYTONE_ENABLE
		dsk_keytone_set_state(SET_KEYTONE_OFF);
#endif
	}
	
	return EPDK_OK; 
}


/**
 * �ͷ� ϵͳ��Դ
 */
__s32 live_exit(void)
{		
	/* �ͷ����� */
	GUI_SetFont(GUI_GetDefaultFont());
	if(SWFFont && (GUI_GetDefaultFont() != SWFFont))
	{
		GUI_SFT_ReleaseFont(SWFFont);
		SWFFont = NULL;        
	}
	
	//save system settings
	dsk_reg_flush();
    dsk_reg_deinit_para();
	
	//exit walkman/keytone
#if BEETLES_KEYTONE_ENABLE
	dsk_keytone_exit();		
#endif	
	dsk_wkm_exit();	

	//dsk_orchid_save_npl();
	
    //�ͷ�����ͼƬ��Դ
	dsk_theme_exit();
	dsk_langres_exit();	
	
	return EPDK_OK;
}

void init_screen_size(void)
{
	dsk_display_get_size(&LCD_WIDTH, &LCD_HEIGHT);
}


