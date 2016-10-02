

#include "statics_template.h"

/*
************************************************************************************************************************
*                    
*
*Description: 创建一个statics控件 
*
*Arguments  : 
*
*Return     : 
*            
************************************************************************************************************************
*/

int Get_Statics_Source(statics_info_t *statics_info,__u32 ID,statics_ui_t *statics_ui)
{

	int i = 0;

	if(statics_ui->bmp_total > 0)
	{
		statics_info->bmp_data = In_Malloc(sizeof(HTHEME) * statics_ui->bmp_total);
		
		if(NULL == statics_info->bmp_data)
		{
			eDbug("statics_info->bmp_data malloc is failed \n");

			goto error;
		}

		for(i = 0; i < statics_ui->bmp_total; i++)
		{
			if(statics_ui->bmps[i] > 0)
			{
				statics_info->bmp_data[i]   = theme_open(statics_ui->bmps[i]);
			}
		}
		
		statics_info->bmp_totals = statics_ui->bmp_total;
			
	}


	if(statics_ui->string_total > 0)
	{
		statics_info->strings = In_Malloc(STRING_MAX * statics_ui->string_total);
		
		if(NULL == statics_info->strings)
		{
			eDbug("statics_info->strings malloc is failed \n");

			goto error1;
		}

		for(i = 0; i < statics_ui->string_total; i++)
		{
			dsk_langres_get_menu_text(statics_ui->string_ID[i],statics_info->strings[i],STRING_MAX);
		}
		
		statics_info->string_totals = statics_ui->string_total;
	}

	if(statics_ui->bmp_total > 0)
	{
		statics_info->statics_attribute.statics_paint.focus_bmp   = theme_hdl2buf(statics_info->bmp_data[0]);
		statics_info->statics_attribute.statics_paint.unfocus_bmp = theme_hdl2buf(statics_info->bmp_data[0]);
		
		statics_info->statics_attribute.statics_paint.bmp_pos.x   = 0;
		statics_info->statics_attribute.statics_paint.bmp_pos.y   = 0;
	}


	if(statics_ui->string_total > 0)
	{
		statics_info->statics_attribute.statics_paint.text         = statics_info->strings[0];
		statics_info->statics_attribute.statics_paint.txt_align    = GUI_TA_VCENTER|GUI_TA_HCENTER;
		statics_info->statics_attribute.statics_paint.text_pos.x   = 0;
		statics_info->statics_attribute.statics_paint.text_pos.y   = 0;		
		statics_info->statics_attribute.statics_paint.ftxt_color   = GUI_RED;
		statics_info->statics_attribute.statics_paint.uftxt_color  = GUI_BLACK;
	}
	else
	{
		statics_info->statics_attribute.statics_paint.text         = NULL;
	}

	
	statics_info->statics_attribute.statics_paint.draw_code        = EPDK_CHARSET_ENM_UTF8;
	statics_info->statics_attribute.statics_paint.draw_font        = SWFFont;
	statics_info->statics_attribute.statics_paint.alpha_en         = 0;
	statics_info->statics_attribute.statics_paint.bk_color         = 0;


	return EPDK_OK;

	
error1:
	if(statics_ui->bmp_total > 0)
	{
		for(i = 0; i < statics_ui->bmp_total; i++)
		{
			if(NULL != statics_info->bmp_data[i])
			{
				theme_close(statics_info->bmp_data[i]);
			}
		}
		
	}


	if(NULL != statics_info->bmp_data)
	{
		In_Free(statics_info->bmp_data,sizeof(HTHEME) * statics_info->bmp_totals);
	}


error:

	return EPDK_FAIL;

}


int Relase_Statics_Source(statics_info_t *statics_info,__u32 ID)
{
	int i;
	

	if(NULL != statics_info->bmp_data)
	{

		for(i = 0; i < statics_info->bmp_totals; i++)
		{
			if(NULL != statics_info->bmp_data[i])
			{
				theme_close(statics_info->bmp_data[i]);
			}
		}
	
		In_Free(statics_info->bmp_data,sizeof(HTHEME) * statics_info->bmp_totals);
	}
	
	
	if(NULL != statics_info->strings)
	{
		In_Free(statics_info->strings,STRING_MAX * statics_info->string_totals);
	}


	
	return EPDK_OK;
}

