/*
**************************************************************************************************************
*											         ePDK
*						            the Easy Portable/Player Develop Kits
*									           desktop system 
*
*						        	 (c) Copyright 2007-2010, ANDY, China
*											 All Rights Reserved
*
* File    	: msgbox.c
* By      	: Andy.zhang
* Func		: desk main thread
* Version	: v1.0
* ============================================================================================================
* 2009-7-20 8:51:52  andy.zhang  create this file, implements the fundemental interface;
**************************************************************************************************************
*/

#include "ebook_tips.h"
#include "ebook_uipara.h"
typedef struct tag_tip_ctrl
{
	ebook_tip_para_t *tips_para;

	__s32 focus_txt_color;
	__s32 unfocus_txt_color;
	__s32 tips_old_focus;
	__s32 tips_new_focus;
	H_THEME tip_bmp;
	H_THEME tip_f_bmp;
	H_THEME tip_uf_bmp;
	char no[128];		//string no
	char yes[128];		//string yes
	char tips[128];		//breakpoint tips
}ebook_tips_ctrl_t;

static void ebook_tip_init(ebook_tips_ctrl_t *tips_ctrl)
{
	tips_ctrl->tip_bmp = theme_open(ID_EBOOK_SET_INFO_BMP);
	tips_ctrl->tip_uf_bmp= theme_open(ID_EBOOK_TIP_UF_BMP);
	tips_ctrl->tip_f_bmp= theme_open(ID_EBOOK_TIP_F_BMP);

	get_menu_text(STRING_EBOOK_TIPS_NO, tips_ctrl->no, 128);	//
	get_menu_text(STRING_EBOOK_TIPS_YES, tips_ctrl->yes, 128);//
	get_menu_text(STRING_EBOOK_BKPOINT, tips_ctrl->tips, 128);//

	tips_ctrl->tips_new_focus = TIPS_YES;
	tips_ctrl->tips_old_focus = TIPS_YES;
	
}

static __s32 ebook_tips_paint_item(__gui_msg_t *msg)
{
	ebook_tips_ctrl_t *tips_ctrl;
	GUI_RECT gui_rect;
	ebook_uipara_t *ebook_uipara;
	

	if (GUI_LyrWinGetSta(GUI_WinGetLyrWin(msg->h_deswin)) == GUI_LYRWIN_STA_SUSPEND)
		return EPDK_FAIL;
	
	tips_ctrl = (ebook_tips_ctrl_t *)GUI_WinGetAddData(msg->h_deswin);
	ebook_uipara = (ebook_uipara_t *)get_ebook_uipara();

	GUI_LyrWinSel(tips_ctrl->tips_para->layer);
	GUI_SetFont(tips_ctrl->tips_para->font);
	GUI_UC_SetEncodeUTF8();

	if(tips_ctrl->tips_new_focus==TIPS_YES)
	{
		GUI_SetColor(tips_ctrl->focus_txt_color);
		gui_rect.x0 = ebook_uipara->tip_yes_bmp_pos.x ;
		gui_rect.y0 = ebook_uipara->tip_yes_bmp_pos.y;
		gui_rect.x1 = gui_rect.x0 + ebook_uipara->tip_yes_bmp_pos.w;
		gui_rect.y1 = gui_rect.y0 + ebook_uipara->tip_yes_bmp_pos.h; 
		GUI_BMP_Draw(theme_hdl2buf(tips_ctrl->tip_f_bmp), ebook_uipara->tip_yes_bmp_pos.x,ebook_uipara->tip_yes_bmp_pos.y);
		GUI_DispStringInRect(tips_ctrl->yes,&gui_rect,GUI_TA_HCENTER|GUI_TA_VCENTER);

		GUI_SetColor(tips_ctrl->unfocus_txt_color);
		gui_rect.x0 = ebook_uipara->tip_no_bmp_pos.x ;
		gui_rect.y0 = ebook_uipara->tip_no_bmp_pos.y;
		gui_rect.x1 = gui_rect.x0 + ebook_uipara->tip_no_bmp_pos.w;
		gui_rect.y1 = gui_rect.y0 + ebook_uipara->tip_no_bmp_pos.h; 
		GUI_BMP_Draw(theme_hdl2buf(tips_ctrl->tip_uf_bmp), ebook_uipara->tip_no_bmp_pos.x,ebook_uipara->tip_no_bmp_pos.y);
		GUI_DispStringInRect(tips_ctrl->no,&gui_rect,GUI_TA_HCENTER|GUI_TA_VCENTER);
	}
	else
	{
		GUI_SetColor(tips_ctrl->focus_txt_color);
		gui_rect.x0 = ebook_uipara->tip_no_bmp_pos.x ;
		gui_rect.y0 = ebook_uipara->tip_no_bmp_pos.y;
		gui_rect.x1 = gui_rect.x0 + ebook_uipara->tip_no_bmp_pos.w;
		gui_rect.y1 = gui_rect.y0 + ebook_uipara->tip_no_bmp_pos.h; 
		GUI_BMP_Draw(theme_hdl2buf(tips_ctrl->tip_f_bmp), ebook_uipara->tip_no_bmp_pos.x,ebook_uipara->tip_no_bmp_pos.y);
		GUI_DispStringInRect(tips_ctrl->no,&gui_rect,GUI_TA_HCENTER|GUI_TA_VCENTER);
		
		GUI_SetColor(tips_ctrl->unfocus_txt_color);
		gui_rect.x0 = ebook_uipara->tip_yes_bmp_pos.x ;
		gui_rect.y0 = ebook_uipara->tip_yes_bmp_pos.y;
		gui_rect.x1 = gui_rect.x0 + ebook_uipara->tip_yes_bmp_pos.w;
		gui_rect.y1 = gui_rect.y0 + ebook_uipara->tip_yes_bmp_pos.h; 
		GUI_BMP_Draw(theme_hdl2buf(tips_ctrl->tip_uf_bmp), ebook_uipara->tip_yes_bmp_pos.x,ebook_uipara->tip_yes_bmp_pos.y);
		GUI_DispStringInRect(tips_ctrl->yes,&gui_rect,GUI_TA_HCENTER|GUI_TA_VCENTER);
	}
	
	return EPDK_OK;
}

static __s32 ebook_tips_paint_all(__gui_msg_t *msg)
{
	ebook_tips_ctrl_t *tips_ctrl;
	GUI_RECT gui_rect;
	ebook_uipara_t *ebook_uipara;

	if (GUI_LyrWinGetSta(GUI_WinGetLyrWin(msg->h_deswin)) == GUI_LYRWIN_STA_SUSPEND)
		return EPDK_FAIL;
	
	tips_ctrl = (ebook_tips_ctrl_t *)GUI_WinGetAddData(msg->h_deswin);
	ebook_uipara = (ebook_uipara_t *)get_ebook_uipara();

	GUI_LyrWinSel(tips_ctrl->tips_para->layer);
	GUI_SetFont(tips_ctrl->tips_para->font);
	GUI_UC_SetEncodeUTF8();

//	GUI_BMP_Draw(theme_hdl2buf(tips_ctrl->tip_bmp), ebook_uipara->tips_bmp_pos.x,ebook_uipara->tips_bmp_pos.y);
	GUI_BMP_Draw(theme_hdl2buf(tips_ctrl->tip_bmp), 0,0);

	if(tips_ctrl->tips_new_focus == TIPS_YES)
	{
		GUI_SetBkColor(0x00);
		gui_rect.x0 = ebook_uipara->tips_txt_pos.x;
		gui_rect.y0 = ebook_uipara->tips_txt_pos.y;
		gui_rect.x1 = gui_rect.x0 + ebook_uipara->tips_txt_pos.w;
		gui_rect.y1 = gui_rect.y0 + ebook_uipara->tips_txt_pos.h; 
		
		GUI_SetColor(tips_ctrl->focus_txt_color);
		GUI_DispStringInRect(tips_ctrl->tips,&gui_rect,GUI_TA_HCENTER|GUI_TA_VCENTER);

		gui_rect.x0 = ebook_uipara->tip_yes_bmp_pos.x ;
		gui_rect.y0 = ebook_uipara->tip_yes_bmp_pos.y;
		gui_rect.x1 = gui_rect.x0 + ebook_uipara->tip_yes_bmp_pos.w;
		gui_rect.y1 = gui_rect.y0 + ebook_uipara->tip_yes_bmp_pos.h; 
		GUI_BMP_Draw(theme_hdl2buf(tips_ctrl->tip_f_bmp), ebook_uipara->tip_yes_bmp_pos.x,ebook_uipara->tip_yes_bmp_pos.y);
		GUI_DispStringInRect(tips_ctrl->yes,&gui_rect,GUI_TA_HCENTER|GUI_TA_VCENTER);

		GUI_SetColor(tips_ctrl->unfocus_txt_color);
		gui_rect.x0 = ebook_uipara->tip_no_bmp_pos.x ;
		gui_rect.y0 = ebook_uipara->tip_no_bmp_pos.y;
		gui_rect.x1 = gui_rect.x0 + ebook_uipara->tip_no_bmp_pos.w;
		gui_rect.y1 = gui_rect.y0 + ebook_uipara->tip_no_bmp_pos.h; 
		
		GUI_BMP_Draw(theme_hdl2buf(tips_ctrl->tip_uf_bmp), ebook_uipara->tip_no_bmp_pos.x,ebook_uipara->tip_no_bmp_pos.y);
		GUI_DispStringInRect(tips_ctrl->no,&gui_rect,GUI_TA_HCENTER|GUI_TA_VCENTER);
	}
	else if(tips_ctrl->tips_new_focus == TIPS_NO)
	{
		GUI_SetColor(tips_ctrl->focus_txt_color);
		
		GUI_SetBkColor(0x00);
		gui_rect.x0 = ebook_uipara->tips_txt_pos.x;
		gui_rect.y0 = ebook_uipara->tips_txt_pos.y;
		gui_rect.x1 = gui_rect.x0 + ebook_uipara->tips_txt_pos.w;
		gui_rect.y1 = gui_rect.y0 + ebook_uipara->tips_txt_pos.h; 
		GUI_SetColor(tips_ctrl->focus_txt_color);
		GUI_DispStringInRect(tips_ctrl->tips,&gui_rect,GUI_TA_HCENTER|GUI_TA_VCENTER);

		gui_rect.x0 = ebook_uipara->tip_no_bmp_pos.x ;
		gui_rect.y0 = ebook_uipara->tip_no_bmp_pos.y;
		gui_rect.x1 = gui_rect.x0 + ebook_uipara->tip_no_bmp_pos.w;
		gui_rect.y1 = gui_rect.y0 + ebook_uipara->tip_no_bmp_pos.h; 
		GUI_BMP_Draw(theme_hdl2buf(tips_ctrl->tip_f_bmp), ebook_uipara->tip_no_bmp_pos.x,ebook_uipara->tip_no_bmp_pos.y);
		GUI_DispStringInRect(tips_ctrl->no,&gui_rect,GUI_TA_HCENTER|GUI_TA_VCENTER);
		
		GUI_SetColor(tips_ctrl->unfocus_txt_color);
		gui_rect.x0 = ebook_uipara->tip_yes_bmp_pos.x ;
		gui_rect.y0 = ebook_uipara->tip_yes_bmp_pos.y;
		gui_rect.x1 = gui_rect.x0 + ebook_uipara->tip_yes_bmp_pos.w;
		gui_rect.y1 = gui_rect.y0 + ebook_uipara->tip_yes_bmp_pos.h; 
		GUI_BMP_Draw(theme_hdl2buf(tips_ctrl->tip_uf_bmp), ebook_uipara->tip_yes_bmp_pos.x,ebook_uipara->tip_yes_bmp_pos.y);
		GUI_DispStringInRect(tips_ctrl->yes,&gui_rect,GUI_TA_HCENTER|GUI_TA_VCENTER);
	}
	
	return EPDK_OK;
}

static __s32  ebook_tips_key_proc(__gui_msg_t *msg)
{
	static __s32 last_key = -1;
	ebook_tips_ctrl_t *tips_ctrl;
	
	tips_ctrl = (ebook_tips_ctrl_t *)GUI_WinGetAddData(msg->h_deswin);

	if (KEY_UP_ACTION == msg->dwAddData2)
	{
		if (GUI_MSG_KEY_ENTER == last_key)
		{
			if(tips_ctrl->tips_new_focus==TIPS_YES)
			{
				Ebook_cmd2parent(msg->h_deswin, TIPS_YES, 0, 0);
			}
			else
			{
				Ebook_cmd2parent(msg->h_deswin, TIPS_NO, 0, 0);
			}
		}
		
	}
	else
	{
		switch( msg->dwAddData1 )
		{
		case GUI_MSG_KEY_LEFT:		//ӳ��Ϊup
		case GUI_MSG_KEY_LONGLEFT:
		case GUI_MSG_KEY_IR_LEFT:		//cai++ for ir
		case GUI_MSG_KEY_LONGIR_LEFT:
		{
			if(tips_ctrl->tips_new_focus == TIPS_NO)
			{
				tips_ctrl->tips_new_focus = TIPS_YES;
				ebook_tips_paint_item(msg);	
			}
			else if(tips_ctrl->tips_new_focus == TIPS_YES)
			{
				tips_ctrl->tips_new_focus = TIPS_NO;
				ebook_tips_paint_item(msg);	
			}
		}
			break;
		case GUI_MSG_KEY_RIGHT: 	//ӳ��Ϊdown
		case GUI_MSG_KEY_LONGRIGHT:
		case GUI_MSG_KEY_IR_RIGHT:
		case GUI_MSG_KEY_LONGIR_RIGHT:
		{
			if(tips_ctrl->tips_new_focus == TIPS_NO)
			{
				tips_ctrl->tips_new_focus = TIPS_YES;
				ebook_tips_paint_item(msg);	
			}
			else if(tips_ctrl->tips_new_focus == TIPS_YES)
			{
				tips_ctrl->tips_new_focus = TIPS_NO;
				ebook_tips_paint_item(msg);	
			}
		}
			break;
		case GUI_MSG_KEY_UP:
		case GUI_MSG_KEY_LONGUP:
		case GUI_MSG_KEY_IR_UP:
		case GUI_MSG_KEY_LONGIR_UP:
		{
			if(tips_ctrl->tips_new_focus == TIPS_NO)
			{
				tips_ctrl->tips_new_focus = TIPS_YES;
				ebook_tips_paint_item(msg);	
			}
			else if(tips_ctrl->tips_new_focus == TIPS_YES)
			{
				tips_ctrl->tips_new_focus = TIPS_NO;
				ebook_tips_paint_item(msg);	
			}
		}
		break;
		case GUI_MSG_KEY_DOWN:
		case GUI_MSG_KEY_LONGDOWN:
		case GUI_MSG_KEY_IR_DOWN:
		case GUI_MSG_KEY_LONGIR_DOWN:
		{
			if(tips_ctrl->tips_new_focus == TIPS_NO)
			{
				tips_ctrl->tips_new_focus = TIPS_YES;
				ebook_tips_paint_item(msg);	
			}
			else if(tips_ctrl->tips_new_focus == TIPS_YES)
			{
				tips_ctrl->tips_new_focus = TIPS_NO;
				ebook_tips_paint_item(msg);	
			}
		}
			break;	
		case GUI_MSG_KEY_ESCAPE:
		case GUI_MSG_KEY_MENU:
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
	�ص�
*/
static __s32 _ebook_tips_proc(__gui_msg_t *msg)
{
	switch(msg->id)
	{
	case GUI_MSG_CREATE:
		{
			ebook_tips_ctrl_t *tips_ctrl;
			ebook_tip_para_t *tips_para;
			
			tips_ctrl = (ebook_tips_ctrl_t *)My_Balloc(sizeof(ebook_tips_ctrl_t));
			if(!tips_ctrl)
			{
				__err("memory balloc fail........\n");
				return EPDK_FAIL;
			}
			eLIBs_memset(tips_ctrl, 0, sizeof(ebook_tips_ctrl_t));

			tips_para = (ebook_tip_para_t *)GUI_WinGetAttr(msg->h_deswin);

			tips_ctrl->focus_txt_color = APP_COLOR_YELLOW;
			tips_ctrl->unfocus_txt_color = APP_COLOR_WHITE;
			tips_ctrl->tips_para = tips_para;
			ebook_tip_init(tips_ctrl);
			GUI_WinSetAddData(msg->h_deswin, (__u32)tips_ctrl);

			GUI_LyrWinCacheOn();
			GUI_LyrWinSetSta(tips_ctrl->tips_para->layer, GUI_LYRWIN_STA_ON);
			GUI_LyrWinSetTop(tips_ctrl->tips_para->layer);
			GUI_LyrWinCacheOff();
		}	
		break;
	case GUI_MSG_CLOSE:
		{
			ebook_tips_ctrl_t *tips_ctrl;
			
			tips_ctrl = (ebook_tips_ctrl_t *)GUI_WinGetAddData(msg->h_deswin);
			
			if(tips_ctrl->tips_new_focus==TIPS_YES)
			{
				Ebook_cmd2parent(msg->h_deswin, TIPS_YES, 0, 0);
			}
			else
			{
				Ebook_cmd2parent(msg->h_deswin, TIPS_NO, 0, 0);
			}
		}
		break;
	case GUI_MSG_DESTROY:
		{
			ebook_tips_ctrl_t *tips_ctrl;
			tips_ctrl = (ebook_tips_ctrl_t *)GUI_WinGetAddData(msg->h_deswin);
			if(!tips_ctrl)
			{
				__err("tips_ctrl is null ......\n");
				return EPDK_FAIL;
			}

			if(tips_ctrl->tip_bmp)
			{
				theme_close(tips_ctrl->tip_bmp);	
			}
			if(tips_ctrl->tip_uf_bmp)
			{
				theme_close(tips_ctrl->tip_uf_bmp);	
			}
			if(tips_ctrl->tip_f_bmp)
			{
				theme_close(tips_ctrl->tip_f_bmp);	
			}
			My_Bfree(tips_ctrl->tips_para, sizeof(ebook_tip_para_t));
			My_Bfree(tips_ctrl, sizeof(ebook_tips_ctrl_t));
		}
		break;
	case GUI_MSG_PAINT:
		ebook_tips_paint_all( msg);
		break;
	case GUI_MSG_KEY:
		ebook_tips_key_proc(msg);
		break;
	case GUI_MSG_TOUCH:
		break;
	case GUI_MSG_TIMER:
		break;
	default:
		break;
	}
	
	return GUI_FrmWinDefaultProc(msg);	
}

/*
	��������
*/
H_WIN ebook_tips_win_create(H_WIN h_parent, ebook_tip_para_t *para)
{
	__gui_framewincreate_para_t framewin_para;
	ebook_tip_para_t *tips_para;
	FB fb;

	GUI_LyrWinGetFB(para->layer, &fb);

	tips_para = (ebook_tip_para_t *)My_Balloc(sizeof(ebook_tip_para_t));
	if(!tips_para)
	{
		__msg("memory balloc fail.........\n");
		return EPDK_FAIL;
	}
	tips_para->font= para->font;
	tips_para->layer = para->layer;
	
	eLIBs_memset(&framewin_para, 0, sizeof(__gui_framewincreate_para_t));

	framewin_para.name =	"ebook_tips_win";
	framewin_para.dwExStyle = WS_EX_NONE;
	framewin_para.dwStyle = WS_NONE|WS_VISIBLE;
	framewin_para.spCaption =  NULL;
	framewin_para.hOwner	= NULL;
	framewin_para.hHosting = h_parent;
	framewin_para.id         = EBOOK_TIPS_ID;
	framewin_para.FrameWinProc = (__pGUI_WIN_CB)esKRNL_GetCallBack((__pCBK_t)_ebook_tips_proc);
	framewin_para.rect.x = 0;
	framewin_para.rect.y = 0;
	framewin_para.rect.width=  fb.size.width;
	framewin_para.rect.height =  fb.size.height;
	framewin_para.BkColor.alpha =  0;
	framewin_para.BkColor.red = 0;
	framewin_para.BkColor.green = 0;
	framewin_para.BkColor.blue = 0;
	framewin_para.attr =  (void *)tips_para;
	framewin_para.hLayer = para->layer;

	return (GUI_FrmWinCreate(&framewin_para));
}

void  ebook_tips_win_delete(H_WIN tips_win)
{
	GUI_FrmWinDelete(tips_win);
}

