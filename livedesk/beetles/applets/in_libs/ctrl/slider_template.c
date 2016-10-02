

#include "slider_template.h"


/*
************************************************************************************************************************
*                    
*
*Description: 创建一个slider控件 
*
*Arguments  : 
*
*Return     : 
*            
************************************************************************************************************************
*/

int Get_Slider_Source(slider_info_t *slider_info,__u32 ID,slider_ui_t *slider_ui)
{
	slider_info->head_bmp    = theme_open(slider_ui->bmp_slider.head_bmp);
	slider_info->tail_bmp    = theme_open(slider_ui->bmp_slider.tail_bmp);
	slider_info->cursor_bmp  = theme_open(slider_ui->bmp_slider.cursor_bmp);
	
	slider_info->slider_attribute.slider_paint.head_bmp_buf   = theme_hdl2buf(slider_info->head_bmp);
	slider_info->slider_attribute.slider_paint.tail_bmp_buf   = theme_hdl2buf(slider_info->tail_bmp);
	slider_info->slider_attribute.slider_paint.cursor_bmp_buf = theme_hdl2buf(slider_info->cursor_bmp);

	slider_info->slider_attribute.slider_paint.text_pos.x     = 30;
	slider_info->slider_attribute.slider_paint.text_pos.y     = 30;		
	slider_info->slider_attribute.slider_paint.txt_color     = GUI_RED;
		
	slider_info->slider_attribute.slider_paint.draw_font      = SWFFont;
	
	return EPDK_OK;
}


int Relase_Slider_Source(slider_info_t *slider_info,__u32 ID)
{
	theme_close(slider_info->head_bmp);
	slider_info->head_bmp = 0;
	
	slider_info->slider_attribute.slider_paint.head_bmp_buf = 0;

	theme_close(slider_info->tail_bmp);
	slider_info->tail_bmp = 0;
	
	slider_info->slider_attribute.slider_paint.tail_bmp_buf = 0;

	theme_close(slider_info->cursor_bmp);
	slider_info->cursor_bmp = 0;
	
	slider_info->slider_attribute.slider_paint.cursor_bmp_buf = 0;
	
	return EPDK_OK;
}

