

#include "listbar_template.h"


/*
************************************************************************************************************************
*                    
*
*Description: 创建一个listbar控件 
*
*Arguments  : 
*
*Return     : 
*            
************************************************************************************************************************
*/

int Get_Listbar_Height(__u32 ID)
{
	switch(ID)
	{
		case 1001:
		{

		}
		break;
		default:
		break;
	}

	return 30;
}



int Get_Listbar_Source(listbar_t *listbar,__u32 ID,listbar_ui_t *listbar_ui,__u8 focus_ID)
{
	listbar->listbar_draw.focus_bmp           = theme_open(listbar_ui->bmp_listbar.focus_bmp);
	listbar->listbar_draw.unfocus_bmp         = theme_open(listbar_ui->bmp_listbar.unfocus_bmp);

	listbar->listbar_draw.focus_txt_color     = listbar_ui->focus_color;
	listbar->listbar_draw.unfocus_txt_color   = listbar_ui->unfocus_color;

	listbar->listbar_config.alpha_status        = EPDK_FALSE;
	listbar->listbar_config.bk_color            = 0;
	listbar->listbar_config.start_id            = 0;
	listbar->listbar_config.focus_id            = focus_ID;			
	listbar->listbar_config.item_height         = listbar_ui->item_height;
	listbar->listbar_config.item_width          = listbar_ui->item_width;
	listbar->listbar_config.item_cnt            = listbar_ui->total;
	listbar->listbar_config.list_rect.x         = listbar_ui->listbar_location.x;
	listbar->listbar_config.list_rect.y         = listbar_ui->listbar_location.y;
	listbar->listbar_config.list_rect.width     = listbar_ui->item_width;
	listbar->listbar_config.list_rect.height    = listbar_ui->item_height * listbar_ui->total;
	
	listbar->listbar_config.list_attr           = (void *)(& (listbar->listbar_draw) );

	
	return EPDK_OK;
}


int Relase_Listbar_Source(listbar_t *listbar,__u32 ID)
{
	theme_close(listbar->listbar_draw.focus_bmp);
	listbar->listbar_draw.focus_bmp = 0;

	theme_close(listbar->listbar_draw.unfocus_bmp);
	listbar->listbar_draw.unfocus_bmp = 0;

	return EPDK_OK;
		
}

