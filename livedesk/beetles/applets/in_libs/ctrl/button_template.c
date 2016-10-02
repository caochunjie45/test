#include "button_template.h"


/*
************************************************************************************************************************
*                    
*
*Description: 创建一个button控件 
*
*Arguments  : 
*
*Return     : 
*            
************************************************************************************************************************
*/

int Get_Button_Source(button_info_t *buttton_info,__u32 ID,button_ui_t *button_ui)
{
		buttton_info->focus_bmp   = theme_open(button_ui->bmp_button.focus_bmp);
		buttton_info->unfocus_bmp = theme_open(button_ui->bmp_button.unfocus_bmp);
		
		buttton_info->button_attribute.button_paint.focus_bmp   = theme_hdl2buf(buttton_info->focus_bmp);
		buttton_info->button_attribute.button_paint.unfocus_bmp = theme_hdl2buf(buttton_info->unfocus_bmp);
		
		buttton_info->button_attribute.button_paint.bmp_pos.x   = 0;
		buttton_info->button_attribute.button_paint.bmp_pos.y   = 0;

		if(button_ui->string_ID > 0)
		{

			buttton_info->strings = In_Malloc(STRING_MAX);
			if(NULL == buttton_info->strings)
			{
				eDbug("buttton_info->strings malloc is failed \n");
				goto ERROR;
			}

			dsk_langres_get_menu_text(button_ui->string_ID,buttton_info->strings,STRING_MAX);
			
			buttton_info->button_attribute.button_paint.text         = buttton_info->strings;
			buttton_info->button_attribute.button_paint.txt_align    = GUI_TA_VCENTER|GUI_TA_HCENTER;
			buttton_info->button_attribute.button_paint.text_pos.x   = 0;
			buttton_info->button_attribute.button_paint.text_pos.y   = 0;		
			buttton_info->button_attribute.button_paint.ftxt_color   = GUI_RED;
			buttton_info->button_attribute.button_paint.uftxt_color  = GUI_BLACK;

		}

			
		buttton_info->button_attribute.button_paint.draw_code        = EPDK_CHARSET_ENM_UTF8;
		buttton_info->button_attribute.button_paint.draw_font        = SWFFont;
		buttton_info->button_attribute.button_paint.alpha_en         = 1;
		buttton_info->button_attribute.button_paint.bk_color         = 0x0;


	return EPDK_OK;


ERROR:
	theme_close(buttton_info->focus_bmp);
	
	theme_close(buttton_info->unfocus_bmp);

	return EPDK_FAIL;
	
}


int Relase_Button_Source(button_info_t *buttton_info,__u32 ID)
{
	theme_close(buttton_info->focus_bmp);
	buttton_info->focus_bmp = 0;
	buttton_info->button_attribute.button_paint.focus_bmp = 0;

	theme_close(buttton_info->unfocus_bmp);
	buttton_info->unfocus_bmp = 0;
	buttton_info->button_attribute.button_paint.unfocus_bmp = 0;


	if(NULL != buttton_info->strings)
	{
		In_Free(buttton_info->strings,STRING_MAX);
	}
	
	return EPDK_OK;
		
}

