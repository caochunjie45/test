/*
**************************************************************************************************************
*											         ePDK
*						            the Easy Portable/Player Develop Kits
*									           desktop system 
*
*						        	 (c) Copyright 2007-2011, CHIPHD, China
*											 All Rights Reserved
*
* File    	: fm_main.c
* By      	: CQQ
* Func		: 
* Version	: v1.0
* ============================================================================================================
* 2011/07/22 21:23  create this file, implements the fundamental interface;
**************************************************************************************************************
*/
#include "fm_main.h"
#include "fm_sset.h"
//////////////////////////////////////////////////////////////////////////

/************************************************************************/
//
/************************************************************************/

/************************************************************************/
//������Դ
/************************************************************************/
static __s32 fm_mainfrw_create_res(FM_MAINWND_RES_T* res)
{
	//create_bmp_res(ID_FM_BG_BMP, res->bg);

	fm_freq_scale_create_res(&res->scale);

	return EPDK_OK;
}

/************************************************************************/
//�ͷ���Դ
/************************************************************************/
static __s32 fm_mainfrw_destroy_res(FM_MAINWND_RES_T* res)
{
	fm_freq_scale_destroy_res(&res->scale);

	//destroy_bmp_res(res->bg);

	return EPDK_OK;
}


/************************************************************************/
//������Ϣ����
/************************************************************************/
static __s32 fm_mainfrw_key_proc(__gui_msg_t *msg)
{
	FM_MAINWND_T *wnd_para;
	static __s32  lastkey = -1;
	FM_GetWndPara(wnd_para, FM_MAINWND_T, msg->h_deswin);

//eLIBs_printf("----fm frm �յ�������Ϣ lastkey = %d, msg->dwAddData2 = %d , msg->dwAddData1 = %d  \n",lastkey ,msg->dwAddData2,msg->dwAddData1);
	if(msg->dwAddData2 == KEY_UP_ACTION)
	{
		switch(lastkey)
		{
			case VK_LEFT:			
			case VK_RIGHT:			
			case VK_ESCAPE:
			case VK_ENTER:
			case VK_MENU:
			case VK_LEFT_REPEAT:
			case VK_RIGHT_REPEAT:
	            case VK_ENTER_REPEAT:
				SEND_MSG_EX(GUI_MSG_COMMAND, msg->h_deswin, FM_GetManWnd(), 
					MAKELONG(FM_MAINWND_ID, ID_FM_MAINWND_CLICKUP), lastkey, msg->dwReserved);
				break;
		}	
		lastkey = -1;
	}
	else
	{
		switch(msg->dwAddData1)
		{	
            case VK_ENTER_REPEAT:
				SEND_MSG_EX(GUI_MSG_COMMAND, msg->h_deswin, FM_GetManWnd(), 
				MAKELONG(FM_MAINWND_ID, ID_FM_MAINWND_CLICKDOWN), msg->dwAddData1, msg->dwReserved);
				break;
		}
		lastkey = msg->dwAddData1;
	}		
	return 0;
}

/************************************************************************/
//������Ϣ����
/************************************************************************/
static __s32 on_fm_mainfrw_command(__gui_msg_t *msg)
{
	FM_MAINWND_T *wnd_para;
	FM_GetWndPara(wnd_para, FM_MAINWND_T, msg->h_deswin);

// 	switch (LOSWORD(msg->dwAddData1))
// 	{
// 	case ID_FM_SCENE_SSET:
// 		return do_fm_mainfrw_sset_cmd(msg->h_deswin, HISWORD(msg->dwAddData1), msg->dwAddData2);
// 	case ID_FM_IS_AUTO_SEARCH_DLG:
// 		return do_fm_mainfrw_auto_search_dlg_cmd(msg->h_deswin, HISWORD(msg->dwAddData1), msg->dwAddData2);
// 	}
	
	return EPDK_FAIL;
}

/************************************************************************/
//������Ϣ���´���
/************************************************************************/
// static __s32 on_fm_mainfrw_touch_down(H_WIN hwnd, int x, int y)
// {
// 	FM_MAINWND_T *wnd_para;
// 	FM_GetWndPara(wnd_para, FM_MAINWND_T, hwnd);
// 
// 	
// 	return EPDK_OK;
// }

/************************************************************************/
//������Ϣ�ƶ�����
/************************************************************************/
// static __s32 on_fm_mainfrw_touch_move(H_WIN hwnd, int x, int y)
// {
// 	FM_MAINWND_T *wnd_para;
// 	FM_GetWndPara(wnd_para, FM_MAINWND_T, hwnd);
// 
// 	return EPDK_OK;
// }

/************************************************************************/
//������Ϣ������
/************************************************************************/
// static __s32 on_fm_mainfrw_touch_up(H_WIN hwnd, int x, int y)
// {
// 	FM_MAINWND_T *wnd_para;
// 	FM_GetWndPara(wnd_para, FM_MAINWND_T, hwnd);
// 
// 	SEND_MSG(GUI_MSG_COMMAND, hwnd, FM_GetManWnd(), 
// 		MAKELONG(FM_MAINWND_ID, ID_FM_MAINWND_NOTIFY_TOUCHUP), MAKELONG(x, y));
// 
// 	return EPDK_OK;
// }


/************************************************************************/
//������Ϣ����
/************************************************************************/
static __s32 fm_mainfrw_touch_proc(__gui_msg_t *msg)
{
	int x = 0, y = 0;
	FM_MAINWND_T *wnd_para;
	FM_GetWndPara(wnd_para, FM_MAINWND_T, msg->h_deswin);

	if (wnd_para->fm_ctrl->search_mode == SRHMODE_AUTO
		|| wnd_para->fm_ctrl->search_mode == SRHMODE_MANUAL)
	{
		return EPDK_OK;
	}

	x = LOSWORD(msg->dwAddData2);
	y = HISWORD(msg->dwAddData2);

	__msg("fm_mainfrw_touch_proc %d (%d, %d)\n", msg->dwAddData1, x, y);

	if (msg->dwAddData1 == GUI_MSG_TOUCH_DOWN || msg->dwAddData1 == GUI_MSG_TOUCH_LONGDOWN)
	{
		GUI_WinSetCaptureWin(msg->h_deswin);
	} 
	else if (msg->dwAddData1 == GUI_MSG_TOUCH_UP || msg->dwAddData1 == GUI_MSG_TOUCH_OVERUP)
	{
		if (GUI_WinGetCaptureWin() == msg->h_deswin)
		{
			GUI_WinReleaseCapture();
		}
	}

	if (on_freq_scale_touch(msg->h_deswin, x, y, msg->dwAddData1))
	{
		return EPDK_OK;
	}

	if (GUI_MSG_TOUCH_UP == msg->dwAddData1)
	{
		SEND_MSG_EX(GUI_MSG_COMMAND, msg->h_deswin, FM_GetManWnd(), 
			MAKELONG(FM_MAINWND_ID, ID_FM_MAINWND_NOTIFY_TOUCHUP), MAKELONG(x, y), 0);
	}

	return EPDK_OK;
}

/************************************************************************/
// ��Ƶ������ǰƵ���α�
/************************************************************************/
// static __s32 draw_freq_scale(H_WIN hwnd, H_LYR layer, FM_UI_MAINWND *ui)
// {
// 	char buf[8 * 2] = {0};
// 	int x;
// 	GUI_RECT rt;
// 	FM_CTRLDATA_T*   fm_ctrl;
// 	FM_MAINWND_RES_T *res;
// 	FM_MAINWND_T *wnd_para;
// 	
// 	FM_GetWndPara(wnd_para, FM_MAINWND_T, hwnd);
// 
// 	res = &wnd_para->res;
// 
// 	fm_ctrl = wnd_para->fm_ctrl;
// 
// 	GUI_LyrWinSel(layer);
// 
// 	GUI_LyrWinCacheOn();
// 
// 	//�����
// 	GUI_BMP_RES_Draw(res->bmp_scale, ui->scale.x, ui->scale.y);
// 
// 	//���α�
// 	x = RANGE_MAPPING(ui->scale.x, ui->scale.x + ui->scale.width, 
// 		fm_ctrl->cur_min_freq, fm_ctrl->cur_max_freq, fm_ctrl->cur_freq);
// 
// 	GUI_BMP_RES_Draw(res->bmp_cursor_sel, x, ui->scale.y + ui->cursor_y);
// 
// 	//
// 	GUI_SetFont(SWFFont);
// 	GUI_UC_SetEncodeUTF8();
// 	GUI_SetColor(ui->clr_txt_n);
// 
// 	rt.x0 = ui->scale.x;
// 	rt.x1 = rt.x0 + ui->scale.width;
// 	rt.y0 = ui->scale.y + ui->txt_y;
// 	rt.y1 = ui->scale.y + ui->scale.height;
// 
// 	MAKE_STR_CHANNEL_FREQ(buf, fm_ctrl->cur_min_freq);
// 	GUI_DispStringInRect(STR_MIN_CHANNEL_FREQ, &rt, GUI_TA_LEFT);
// 
// 	MAKE_STR_CHANNEL_FREQ(buf, fm_ctrl->cur_freq);
// 	GUI_DispStringInRect(buf, &rt, GUI_TA_CENTER);
// 
// 	MAKE_STR_CHANNEL_FREQ(buf, fm_ctrl->cur_max_freq);
// 	GUI_DispStringInRect(STR_MAX_CHANNEL_FREQ, &rt, GUI_TA_RIGHT);
// 
// 	GUI_LyrWinCacheOff();
// 
// 	return EPDK_OK;
// }

/***********************************************************************************
���ô��ڰ�ť�α�����4702ģ���ڲ�����������ʾ����
*************************************************************************************/
/*
__s32 fm_auto_search_cb(FM_WND_T *fmplay_menu_attr)
{
    
	if (GUI_LyrWinGetSta(fmplay_menu_attr->layer) == GUI_LYRWIN_STA_SUSPEND)
	{
		__msg("&&&&&&&&&&&&&&&&&&&   GUI_LYRWIN_STA_SUSPEND! &&&&&&&&&&&&&&&&&&& \n");
		return;
	}

	GUI_LyrWinSel(fmplay_menu_attr->layer);
	//GUI_SetFont(fmplay_menu_attr->font);
	//GUI_UC_SetEncodeUTF8();    
    
    //fm_view_show(fmplay_menu_attr);
    draw_freq_scale(fmplay_menu_attr);
    return EPDK_OK;
}
*/

/************************************************************************/
// paint window
/************************************************************************/
static __s32 on_fm_mainfrw_paint(__gui_msg_t *msg)
{
	H_LYR layer = GUI_WinGetLyrWin(msg->h_deswin);

	if (GUI_LyrWinGetSta(layer) == GUI_LYRWIN_STA_ON)
	{
		FM_UI_MAINWND *ui;
		GUI_MEMDEV_Handle draw_mem;
		FM_MAINWND_T *wnd_para;

		FM_GetWndPara(wnd_para, FM_MAINWND_T, msg->h_deswin);

		GUI_LyrWinSel(layer);

		ui = &get_fm_uipara()->main;
		draw_mem = GUI_MEMDEV_Create(0, 0, ui->rt.width, ui->rt.height);
		GUI_MEMDEV_Select(draw_mem);

//		GUI_LyrWinCacheOn();
		GUI_SetBkColor(0);
		GUI_Clear();
		//GUI_BMP_RES_Draw(wnd_para->res.bg, 0, 0);
		draw_freq_scale(msg->h_deswin, layer, ui);

//		GUI_LyrWinCacheOff();

		GUI_MEMDEV_CopyToLCD_Ex(draw_mem);
		GUI_MEMDEV_Select(NULL);
		GUI_MEMDEV_Delete(draw_mem);
		draw_mem = NULL;

		return EPDK_OK;
	}
	else
	{
		return EPDK_FAIL;
	}
}

/************************************************************************/
//�������ٴ���
/************************************************************************/
static __s32 on_fm_mainfrw_destory(__gui_msg_t *msg)
{
	FM_MAINWND_T *wnd_para;
	FM_GetWndPara(wnd_para, FM_MAINWND_T, msg->h_deswin);
	//TODO:release resource
	fm_mainfrw_destroy_res(&wnd_para->res);
	
	BFreeType(wnd_para, FM_MAINWND_T);

	return 0;
}

/************************************************************************/
//���ڴ�������
/************************************************************************/
static __s32 on_fm_mainfrw_create(__gui_msg_t *msg)
{
	FM_MAINWND_T *wnd_para;
	FM_CTRLDATA_T *init_para;
	init_para = (FM_CTRLDATA_T *)GUI_WinGetAttr(msg->h_deswin);
	BallocType(wnd_para, FM_MAINWND_T);
	ASSERT(wnd_para != NULL);
	ZeroTypeMem(wnd_para, FM_MAINWND_T);
	GUI_WinSetAttr(msg->h_deswin, wnd_para);
	//TODO:init wnd_para
	wnd_para->fm_ctrl = init_para;
	__here__;
	fm_mainfrw_create_res(&wnd_para->res);

	return 0;
}

//��Ϣ������
static __s32 _fm_mainfrw_proc(__gui_msg_t *msg)
{
	//APP_DEBUG_CBMSGEx;

	switch(msg->id)
	{
	case GUI_MSG_CREATE:
		{
			return on_fm_mainfrw_create(msg);
		}
	case GUI_MSG_DESTROY:
		{	
			set_record_is_play(EPDK_TRUE);
			//dsk_reg_flush();
			on_fm_mainfrw_destory(msg);
			{
				__gui_msg_t my_msg;
				my_msg.h_deswin =  GUI_WinGetHandFromName("beetles"); 						
				my_msg.h_srcwin = msg->h_deswin;	
				my_msg.id = DSK_MSG_SWITCH_AUDIO_IF;

				GUI_SendNotifyMessage(&my_msg);
			}
			return EPDK_OK;
		}
	case DSK_MSG_HOME:
		notify_to_close_fm(msg->h_deswin, SWITCH_TO_MMENU, 0);
		return EPDK_OK;
	case DSK_MSG_KILL:
	case GUI_MSG_CLOSE:
		{
			GUI_ManWinDelete(msg->h_deswin);
		}
		return EPDK_OK;
	case GUI_MSG_COMMAND:
		{
			if (on_fm_mainfrw_command(msg) == EPDK_OK)
			{
				return EPDK_OK;
			}
		}
		break;
	case GUI_MSG_KEY:	
		{
			fm_mainfrw_key_proc(msg);
		}
		return EPDK_OK;
	case GUI_MSG_TOUCH:
		{
			fm_mainfrw_touch_proc(msg);
		}
		return EPDK_OK;
	case GUI_MSG_PAINT:	
		{
			on_fm_mainfrw_paint(msg);
		}
		return EPDK_OK;
	case CMD_AUTOSRH_FINDCH:
		{
			//�Զ�����״̬��������һ��Ƶ��:
			FM_UpdateUI(msg->h_deswin);
		}
		return EPDK_OK;
	case CMD_AUTOSRH_FINDCHFAIL:
		{			
			FM_UpdateUI(msg->h_deswin);
		}
		return EPDK_OK;
	case CMD_AUTOSRH_OVER:
		{//�Զ��������
			dsk_reg_flush();
			set_record_is_play(EPDK_TRUE);
			FM_UpdateUI(msg->h_deswin);
			//Turn On BackLight;	Ethan;
			{
				__gui_msg_t msg;

				msg.id 			= DSK_MSG_SCREEN_OPEN;
				msg.h_srcwin 		= NULL;
				msg.h_deswin		= GUI_WinGetHandFromName("init");
				msg.dwAddData1	= (__u32)0;
				msg.dwAddData2	= (__u32)0;
				
				GUI_SendNotifyMessage(&msg);
			}
		}
		
		return EPDK_OK;
	default:
		break;
	}
	
	return GUI_FrmWinDefaultProc(msg);
}

//���ڴ���
H_WIN fm_mainfrw_create(H_WIN h_parent, H_LYR layer, void *para)
{
	__gui_framewincreate_para_t framewin_para;
	FB fb;

	GUI_LyrWinGetFB(layer, &fb);
	ZeroTypeMem(&framewin_para, __gui_framewincreate_para_t);

	framewin_para.name          = FM_MAINWND_NAME;
	framewin_para.dwExStyle     = WS_EX_NONE;
	framewin_para.dwStyle       = WS_NONE | WS_VISIBLE;
	framewin_para.spCaption     = NULL;
	framewin_para.hOwner        = NULL;
	framewin_para.id            = FM_MAINWND_ID;
	framewin_para.hHosting      = h_parent;
	framewin_para.FrameWinProc  = (__pGUI_WIN_CB)esKRNL_GetCallBack((__pCBK_t)_fm_mainfrw_proc);
	framewin_para.rect.x        = 0;
	framewin_para.rect.y        = 0;
	framewin_para.rect.width    = fb.size.width;
	framewin_para.rect.height   = fb.size.height;
	framewin_para.BkColor.alpha = 0;
	framewin_para.BkColor.red   = 0;
	framewin_para.BkColor.green = 0;
	framewin_para.BkColor.blue  = 0;
	framewin_para.attr          = para;
	framewin_para.hLayer        = layer;

	return (GUI_FrmWinCreate(&framewin_para));
}
//////////////////////////////////////////////////////////////////////////
// end of file


