/*
**************************************************************************************************************
*											         ePDK
*						            the Easy Portable/Player Develop Kits
*									           desktop system 
*
*						        	 (c) Copyright 2007-2010, Hot.lee, China
*											 All Rights Reserved
*
* File    	: program_previews.c
* By      	: 
* Func		: 
* Version	: v1.0
* ============================================================================================================
*
**************************************************************************************************************
*/

#include "dtv_paint.h"



void show_dtv_record_staue_win(void)
{
	GUI_RECT gui_rect;
	char text[128];
	H_THEME bg_bmp;
	void * bmp_buffer;

	RECT rect;
    __dtv_uipara_t* dtv_uipara;
    
    dtv_uipara = dtv_get_uipara();
    rect.x = dtv_uipara->record_staue_layer.x;
    rect.y = dtv_uipara->record_staue_layer.y;
    rect.width = dtv_uipara->record_staue_layer.width;
    rect.height = dtv_uipara->record_staue_layer.height;

    gui_rect.x0 = rect.x;
	gui_rect.y0 = rect.y;
	gui_rect.x1 = gui_rect.x0 + rect.width;
	gui_rect.y1 = gui_rect.y0 + rect.height;
    
    rect.x = dtv_uipara->main_layer.scn_rect.x;
    rect.y = dtv_uipara->main_layer.scn_rect.y;
    rect.width = dtv_uipara->main_layer.scn_rect.width;
    rect.height = dtv_uipara->main_layer.scn_rect.height;
	dtv_ctr.h_record_layer = layer_win_create(&rect);
    
	{
		GUI_LyrWinSetSta(dtv_ctr.h_record_layer, GUI_LYRWIN_STA_ON);
		GUI_LyrWinSetTop(dtv_ctr.h_record_layer);
	}
	GUI_LyrWinSel(dtv_ctr.h_record_layer);
	GUI_UC_SetEncodeUTF8();
	GUI_Clear();
	GUI_SetFont(SWFFont);

	bg_bmp = dsk_theme_open(ID_DTV_TIPS_BG_BMP);
	if(bg_bmp)
	{
		__msg("... show tips frame ...\n");
		bmp_buffer = theme_hdl2buf(bg_bmp);
		if(bmp_buffer)
		{
			__msg("... theme_hdl2buf() => 0x%08x ...\n", bmp_buffer);
			GUI_SetDrawMode(GUI_DRAWMODE_NORMAL);
			GUI_BMP_Draw(bmp_buffer, gui_rect.x0, gui_rect.y0);
		}
		dsk_theme_close(bg_bmp);
	}

	GUI_SetColor(GUI_RED);
	dsk_langres_get_menu_text(STRING_DTV_IS_RECORDING, text,sizeof(text));
	GUI_DispStringInRect(text , &gui_rect , GUI_TA_HCENTER|GUI_TA_VCENTER);
}





void show_dtv_destory_win(void)
{
	GUI_RECT gui_rect;
	H_THEME bg_bmp;
	void * bmp_buffer;
	char text[128];
	RECT rect;
    __dtv_uipara_t* dtv_uipara;
    
    dtv_uipara = dtv_get_uipara();
    rect.x = dtv_uipara->main_layer.scn_rect.x;
    rect.y = dtv_uipara->main_layer.scn_rect.y;
    rect.width = dtv_uipara->main_layer.scn_rect.width;
    rect.height = dtv_uipara->main_layer.scn_rect.height;
	dtv_ctr.h_destroy_layer = layer_win_create(&rect);
	
	__msg("... show wait ...\n");
	{
		GUI_LyrWinSetSta(dtv_ctr.h_destroy_layer, GUI_LYRWIN_STA_ON);
		GUI_LyrWinSetTop(dtv_ctr.h_destroy_layer);
	}
	GUI_LyrWinSel(dtv_ctr.h_destroy_layer);
	GUI_UC_SetEncodeUTF8();
	GUI_Clear();
	GUI_SetFont(SWFFont);

	gui_rect.x0 = (rect.width-157)/2;
	gui_rect.y0 = (rect.height-88)/2;
	gui_rect.x1 = gui_rect.x0 + 157-1;
	gui_rect.y1 = gui_rect.y0 + 88 -1;
	
	bg_bmp = dsk_theme_open(ID_DTV_TIPS_BG_BMP);
	if(bg_bmp)
	{
		__msg("... show tips frame ...\n");
		bmp_buffer = theme_hdl2buf(bg_bmp);
		if(bmp_buffer)
		{
			__msg("... theme_hdl2buf() => 0x%08x ...\n", bmp_buffer);
			GUI_SetDrawMode(GUI_DRAWMODE_NORMAL);
			GUI_BMP_Draw(bmp_buffer, gui_rect.x0, gui_rect.y0);
		}
		dsk_theme_close(bg_bmp);
	}
	GUI_SetColor(GUI_WHITE);
	dsk_langres_get_menu_text(STRING_DTV_WAIT, text,sizeof(text));
	GUI_DispStringInRect(text , &gui_rect , GUI_TA_HCENTER|GUI_TA_VCENTER);

}





/**********************************************************************************************************************
* 图层创建接口
**********************************************************************************************************************/
H_WIN layer_win_create( RECT *rect)
{
	FB	fb = {
    	{0, 0},                                   		
    	{0, 0, 0},
    	{FB_TYPE_RGB, {PIXEL_COLOR_ARGB8888, 0, 0}}, 
	};
	__disp_layer_para_t para = 
	{
		DISP_LAYER_WORK_MODE_NORMAL,					 /* mode	  */
		0,												/* ck_mode	 */
		0,												/* alpha_en  */
		0,												/* alpha_val */
		1,												/* pipe 	 */
		0xff,											/* prio 	 */
		{0, 0, 0, 0},									/* screen	 */
		{0, 0, 0, 0},									/* source	 */
		DISP_LAYER_OUTPUT_CHN_DE_CH1,					/* channel	 */
		NULL											/* fb		 */
	};
	
	__layerwincreate_para_t create_info = 
	{
		"dvb_tips_layer",
		NULL,
		GUI_LYRWIN_STA_SUSPEND,
		GUI_LYRWIN_NORMAL
	};
		
	fb.size.width  = rect->width;
	fb.size.height = rect->height;

	para.pipe			= 1;
	
	para.scn_win.x 		= rect->x;
	para.scn_win.y 		= rect->y;
	para.scn_win.width 	= rect->width;	
	para.scn_win.height = rect->height;	

	para.src_win.x 		= 0;
	para.src_win.y 		= 0;
	para.src_win.width 	= rect->width;	
	para.src_win.height = rect->height;	

	para.fb = &fb;
	create_info.lyrpara = &para;	
	return(GUI_LyrWinCreate(&create_info));
}

