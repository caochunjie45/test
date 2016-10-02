

#include "progbars_template.h"


/*
************************************************************************************************************************
*                    
*
*Description: 创建一个progbars控件 
*
*Arguments  : 
*
*Return     : 
*            
************************************************************************************************************************
*/

int Get_Progbars_Source(progbars_info_t *progbars_info,__u32 ID,progbars_ui_t *progbars_ui)
{

	progbars_info->head_bmp    = theme_open(progbars_ui->bmp_progbars.head_bmp);
	progbars_info->tail_bmp    = theme_open(progbars_ui->bmp_progbars.tail_bmp);
	progbars_info->frame_bmp  = theme_open(progbars_ui->bmp_progbars.frame_bmp);
	
	progbars_info->progbars_attribute.progbars_paint.head_bmp_buf   = theme_hdl2buf(progbars_info->head_bmp);
	progbars_info->progbars_attribute.progbars_paint.tail_bmp_buf   = theme_hdl2buf(progbars_info->tail_bmp);
	progbars_info->progbars_attribute.progbars_paint.frame_bmp_buf  = theme_hdl2buf(progbars_info->frame_bmp);

	progbars_info->progbars_attribute.progbars_paint.text_color     = GUI_RED;
		
	progbars_info->progbars_attribute.progbars_paint.font           = SWFFont;
	
	return EPDK_OK;
}


int Relase_Progbars_Source(progbars_info_t *progbars_info,__u32 ID)
{
	theme_close(progbars_info->head_bmp);
	progbars_info->head_bmp = 0;
	
	progbars_info->progbars_attribute.progbars_paint.head_bmp_buf = 0;

	theme_close(progbars_info->tail_bmp);
	progbars_info->tail_bmp = 0;
	
	progbars_info->progbars_attribute.progbars_paint.tail_bmp_buf = 0;

	theme_close(progbars_info->frame_bmp);
	progbars_info->frame_bmp = 0;
	
	progbars_info->progbars_attribute.progbars_paint.frame_bmp_buf = 0;
	
	return EPDK_OK;
}

