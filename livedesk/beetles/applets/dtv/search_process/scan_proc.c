
#include "scan_proc_uipara.h"
#include "scan_proc.h"
#include "..//dtv_mid.h"
#include "..//dtv_i.h"



typedef struct tag_dvb_scan_proc_ctrl
{
	dvb_scan_proc_para_t *dvb_scan_proc_scene_para;

	__s32  total_ch;//搜索到的台数
	
	HTHEME h_bmp_loading; //进度条图片
	HTHEME h_bmp_step;    //步进长度
}dvb_scan_proc_ctrl_t;

__s32 dvb_scan_proc_cmd2parent(H_WIN hwin, __s32 id, __s32 data1, __s32 data2)
{
	__gui_msg_t msg;
	char buf[128];

	msg.h_deswin = GUI_WinGetParent(hwin);
	msg.h_srcwin = NULL;
	msg.id = GUI_MSG_COMMAND;
	msg.dwAddData1 = MAKELONG(GUI_WinGetItemId(hwin), id);
	msg.dwAddData2 = data1;
	msg.dwReserved = data2;

	return (GUI_SendMessage(&msg));
}




//msg->dwAddData1存的是搜索到的台数
//msg->dwAddData2高16位存的是搜索的频点数，低16位存是当前正在搜索的第几个频点

static __s32 dvb_scan_proc_loading_paint(__gui_msg_t *msg )
{
	dvb_scan_proc_ctrl_t *dvb_scan_proc_ctrl;
	dvb_scan_proc_uipara_t* dvb_scan_proc_uipara;
	GUI_RECT gui_rect;
	char txt[1024];
	H_WIN h_win = msg->h_deswin;
	__s32 search_ch_total = 0;
	__s32 total = maple_get_cur_search_freq_tatol();
	__s32 cur_index = 0;
	__s32 cur_freq;

	if (GUI_LyrWinGetSta(GUI_WinGetLyrWin(h_win)) != GUI_LYRWIN_STA_ON)
		return EPDK_FAIL;
	__here__;
	
	dvb_scan_proc_ctrl = (dvb_scan_proc_ctrl_t *)GUI_WinGetAddData(h_win);
	dvb_scan_proc_uipara = (dvb_scan_proc_uipara_t*)submenu_dvb_scan_proc_get_uipara();

	 if(GUI_LYRWIN_STA_ON != GUI_LyrWinGetSta(dvb_scan_proc_ctrl->dvb_scan_proc_scene_para->layer)
        &&GUI_LYRWIN_STA_SLEEP != GUI_LyrWinGetSta(dvb_scan_proc_ctrl->dvb_scan_proc_scene_para->layer))
        {
            __wrn("------------------------Err Frame Buf fail.\n");            
            return EPDK_FAIL;
        }

    GUI_LyrWinSel(dvb_scan_proc_ctrl->dvb_scan_proc_scene_para->layer);
   
	GUI_SetFont(dvb_scan_proc_ctrl->dvb_scan_proc_scene_para->font);
	GUI_SetColor(GUI_WHITE);
	GUI_UC_SetEncodeUTF8();
	GUI_SetDrawMode(LCD_DRAWMODE_NORMAL);
	__here__;

    //先画图片，再写字
	if(dvb_scan_proc_ctrl->h_bmp_loading)
	{
	    __s32 width;
		__s32 x,y;
		x = dvb_scan_proc_uipara->dvb_scan_proc_bmp_pos.x;
		y = dvb_scan_proc_uipara->dvb_scan_proc_bmp_pos.y;
		GUI_BMP_Draw(theme_hdl2buf(dvb_scan_proc_ctrl->h_bmp_loading), x,y);
	}
	else
	{
		dvb_scan_proc_ctrl->h_bmp_loading = dsk_theme_open(ID_DTV_SCAN_PROC_BMP);
		GUI_BMP_Draw(theme_hdl2buf(dvb_scan_proc_ctrl->h_bmp_loading), dvb_scan_proc_uipara->dvb_scan_proc_bmp_pos.x,dvb_scan_proc_uipara->dvb_scan_proc_bmp_pos.y);
		
	}


    GUI_SetColor(GUI_WHITE);
    //显示"正在搜索"
	dsk_langres_get_menu_text(STRING_DTV_SEARCH_NOW,txt,sizeof(txt));
	__wrn("txt = %s\n",txt);
	gui_rect.x0 = dvb_scan_proc_uipara->dvb_scan_str.x ;
	gui_rect.y0 = dvb_scan_proc_uipara->dvb_scan_str.y ;
	gui_rect.x1 = gui_rect.x0 + dvb_scan_proc_uipara->dvb_scan_str.width - 1 ;
	gui_rect.y1 = gui_rect.y0 + dvb_scan_proc_uipara->dvb_scan_str.height - 1 ;
	GUI_DispStringInRect(txt , &gui_rect , GUI_TA_HCENTER|GUI_TA_VCENTER );

    //显示频点
	//dsk_langres_get_menu_text(STRING_DTV_SEARCH_NOW,txt,sizeof(txt));
	cur_freq = maple_get_cur_search_freq(cur_index);
	eLIBs_sprintf(txt,"%d.%03d MHz",cur_freq/1000,cur_freq%1000);
	__wrn("txt = %s\n",txt);
	gui_rect.x0 = dvb_scan_proc_uipara->dvb_scan_freq.x ;
	gui_rect.y0 = dvb_scan_proc_uipara->dvb_scan_freq.y ;
	gui_rect.x1 = gui_rect.x0 + dvb_scan_proc_uipara->dvb_scan_freq.width - 1 ;
	gui_rect.y1 = gui_rect.y0 + dvb_scan_proc_uipara->dvb_scan_freq.height - 1 ;
	GUI_DispStringInRect(txt , &gui_rect , GUI_TA_HCENTER|GUI_TA_VCENTER );	
	
	//搜索到的台数
	dsk_langres_get_menu_text(STRING_DTV_SEARCH_VALID_CH,txt,sizeof(txt));
	
	eLIBs_sprintf(txt,"%s%d\n",txt,search_ch_total);
	__wrn("txt = %s\n",txt);
	gui_rect.x0 = dvb_scan_proc_uipara->dvb_scan_ch.x ;
	gui_rect.y0 = dvb_scan_proc_uipara->dvb_scan_ch.y ;
	gui_rect.x1 = gui_rect.x0 + dvb_scan_proc_uipara->dvb_scan_ch.width - 1 ;
	gui_rect.y1 = gui_rect.y0 + dvb_scan_proc_uipara->dvb_scan_ch.height - 1 ;
	GUI_DispStringInRect(txt , &gui_rect , GUI_TA_HCENTER|GUI_TA_VCENTER );	

	return EPDK_OK;
}

static __s32 scan_cmd_proc(__gui_msg_t *msg)
{
	dvb_scan_proc_ctrl_t *dvb_scan_proc_ctrl;
	dvb_scan_proc_uipara_t* dvb_scan_proc_uipara;
	GUI_RECT gui_rect;
    GUI_RECT scan_proc;
	char txt[128];
	H_WIN h_win = msg->h_deswin;

	dvb_scan_proc_ctrl = (dvb_scan_proc_ctrl_t *)GUI_WinGetAddData(msg->h_deswin);
	dvb_scan_proc_uipara = (dvb_scan_proc_uipara_t*)submenu_dvb_scan_proc_get_uipara();

    if(NULL == dvb_scan_proc_ctrl)
    {
        __wrn("NULL == dvb_scan_proc_ctrl.\n");
        return EPDK_OK;
    }

    if(NULL == dvb_scan_proc_ctrl->dvb_scan_proc_scene_para)
    {
        __wrn("NULL == dvb_scan_proc_ctrl->dvb_scan_proc_scene_para.\n");
        return EPDK_OK;
    }

    if(NULL == dvb_scan_proc_ctrl->dvb_scan_proc_scene_para->layer)
    {
        __wrn("NULL == dvb_scan_proc_ctrl->dvb_scan_proc_scene_para->layer.\n");
        return EPDK_OK;
    }
	
    if(GUI_LYRWIN_STA_ON != GUI_LyrWinGetSta(dvb_scan_proc_ctrl->dvb_scan_proc_scene_para->layer)
        &&GUI_LYRWIN_STA_SLEEP != GUI_LyrWinGetSta(dvb_scan_proc_ctrl->dvb_scan_proc_scene_para->layer))
        {
            __wrn("------------------------Err Frame Buf fail.\n");            
            return EPDK_FAIL;
        }

    GUI_LyrWinSel(dvb_scan_proc_ctrl->dvb_scan_proc_scene_para->layer);
    

	GUI_LyrWinSetTop(dvb_scan_proc_ctrl->dvb_scan_proc_scene_para->layer);
	GUI_SetFont(dvb_scan_proc_ctrl->dvb_scan_proc_scene_para->font);	
	GUI_UC_SetEncodeUTF8();
	GUI_SetDrawMode(LCD_DRAWMODE_NORMAL);
	
	dvb_scan_proc_ctrl->h_bmp_step = dsk_theme_open(ID_DTV_PROC_STEP_BMP);

    switch(msg->dwAddData1&0xffff)
    {
		case SLIDE_UPDATE_FREQ:
		{
			__s32 freq = msg->dwAddData2;//当前搜索频点
			__s32 i,j;
			__s32 width;
			__s32 x,y;
			__s32 total = maple_get_cur_search_freq_tatol();
			
			__s32 cur_index = HIWORD(msg->dwAddData1);
			
            x = dvb_scan_proc_uipara->dvb_scan_proc_bmp_pos.x ;
            y = dvb_scan_proc_uipara->dvb_scan_proc_bmp_pos.y;
			//贴图方式		
			for(i=0; i< cur_index + 1; i++) 
			{
		        GUI_BMP_Draw(theme_hdl2buf(dvb_scan_proc_ctrl->h_bmp_step), x+i*((dvb_scan_proc_uipara->search_proc_width)/total ),y);
			}

			GUI_SetColor(GUI_WHITE);
			eLIBs_sprintf(txt,"%d.%03d MHz",freq/1000,freq%1000);
			__wrn("txt = %s\n",txt);
			gui_rect.x0 = dvb_scan_proc_uipara->dvb_scan_freq.x ;
			gui_rect.y0 = dvb_scan_proc_uipara->dvb_scan_freq.y ;
			gui_rect.x1 = gui_rect.x0 + dvb_scan_proc_uipara->dvb_scan_freq.width - 1 ;
			gui_rect.y1 = gui_rect.y0 + dvb_scan_proc_uipara->dvb_scan_freq.height - 1 ;
			GUI_ClearRect(gui_rect.x0,gui_rect.y0,gui_rect.x1,gui_rect.y1);
			GUI_DispStringInRect(txt , &gui_rect , GUI_TA_HCENTER|GUI_TA_VCENTER ); 

		}
		break;

		case SLIDE_UPDATE_CH:
		{			
            GUI_LyrWinSel(dvb_scan_proc_ctrl->dvb_scan_proc_scene_para->layer);

			//搜索到的台数
			dsk_langres_get_menu_text(STRING_DTV_SEARCH_VALID_CH,txt,sizeof(txt));
	
			if(msg->dwAddData2 > 0)
			{
				dvb_scan_proc_ctrl->total_ch++;
				eLIBs_sprintf(txt,"%s%d\n",txt,dvb_scan_proc_ctrl->total_ch);
			}

			gui_rect.x0 = dvb_scan_proc_uipara->dvb_scan_ch.x ;
			gui_rect.y0 = dvb_scan_proc_uipara->dvb_scan_ch.y ;
			gui_rect.x1 = gui_rect.x0 + dvb_scan_proc_uipara->dvb_scan_ch.width - 1 ;
			gui_rect.y1 = gui_rect.y0 + dvb_scan_proc_uipara->dvb_scan_ch.height - 1 ;
			GUI_SetColor(GUI_BLACK);
			GUI_FillRect(gui_rect.x0,gui_rect.y0,gui_rect.x1,gui_rect.y1);
			
			GUI_SetColor(GUI_WHITE);
			GUI_DispStringInRect(txt , &gui_rect , GUI_TA_HCENTER|GUI_TA_VCENTER ); 

		}
		break;		
	}

	return EPDK_OK;

}




static __s32 _dvb_scan_proc_proc(__gui_msg_t *msg)
{
	switch(msg->id)
	{
		case GUI_MSG_CREATE:
		{
			dvb_scan_proc_ctrl_t *dvb_scan_proc_ctrl;
			dvb_scan_proc_para_t *dvb_scan_proc_para;
			__here__;

			dvb_scan_proc_ctrl = (dvb_scan_proc_ctrl_t *)My_Malloc(0,sizeof(dvb_scan_proc_ctrl_t));
			if(!dvb_scan_proc_ctrl)
			{
				__msg("memory balloc fail.........\n");
				return EPDK_FAIL;
			}
			eLIBs_memset(dvb_scan_proc_ctrl, 0, sizeof(dvb_scan_proc_ctrl_t));

			dvb_scan_proc_para = (dvb_scan_proc_para_t *)GUI_WinGetAttr(msg->h_deswin);

			dvb_scan_proc_ctrl->dvb_scan_proc_scene_para = dvb_scan_proc_para;
			GUI_WinSetAddData(msg->h_deswin, (__u32)dvb_scan_proc_ctrl);

			GUI_LyrWinSetSta(dvb_scan_proc_ctrl->dvb_scan_proc_scene_para->layer, GUI_LYRWIN_STA_ON);
			GUI_LyrWinSetTop(dvb_scan_proc_ctrl->dvb_scan_proc_scene_para->layer);
		}	
		return EPDK_OK;
		
		case GUI_MSG_CLOSE:
		{
            dvb_scan_proc_ctrl_t *dvb_scan_proc_ctrl;
			dvb_scan_proc_ctrl = (dvb_scan_proc_ctrl_t *)GUI_WinGetAddData(msg->h_deswin);

			if(!dvb_scan_proc_ctrl)
			{
				__wrn("dvb_scan_proc_ctrl is null.......\n");
				return EPDK_FAIL;
			}
			if(dvb_scan_proc_ctrl->dvb_scan_proc_scene_para->layer)
			{
				GUI_LyrWinSetBottom(dvb_scan_proc_ctrl->dvb_scan_proc_scene_para->layer);
				GUI_LyrWinDelete(dvb_scan_proc_ctrl->dvb_scan_proc_scene_para->layer);
				dvb_scan_proc_ctrl->dvb_scan_proc_scene_para->layer = NULL;
			}
		}
		return EPDK_OK;
		
		case GUI_MSG_DESTROY:
		{
			dvb_scan_proc_ctrl_t *dvb_scan_proc_ctrl;
			dvb_scan_proc_ctrl = (dvb_scan_proc_ctrl_t *)GUI_WinGetAddData(msg->h_deswin);

			if(!dvb_scan_proc_ctrl)
			{
				__wrn("dvb_scan_proc_ctrl is null.......\n");
				return EPDK_FAIL;
			}

			__wrn("GUI_MSG_DESTROY...\n");
			if(dvb_scan_proc_ctrl->dvb_scan_proc_scene_para->layer)
			{
				GUI_LyrWinSetBottom(dvb_scan_proc_ctrl->dvb_scan_proc_scene_para->layer);
	            GUI_LyrWinSel(dvb_scan_proc_ctrl->dvb_scan_proc_scene_para->layer);
	            GUI_LyrWinSetSta(dvb_scan_proc_ctrl->dvb_scan_proc_scene_para->layer,GUI_LYRWIN_STA_SUSPEND);				
			}

			
			if(dvb_scan_proc_ctrl->h_bmp_loading)
			{
				dsk_theme_close(dvb_scan_proc_ctrl->h_bmp_loading);
				dvb_scan_proc_ctrl->h_bmp_loading = 0;
			}
			if(dvb_scan_proc_ctrl->h_bmp_step)
			{
				dsk_theme_close(dvb_scan_proc_ctrl->h_bmp_step);
				dvb_scan_proc_ctrl->h_bmp_step = 0;
			}
            
			if(dvb_scan_proc_ctrl->dvb_scan_proc_scene_para)
			{
			    __wrn("...\n");
				My_Mfree(0,dvb_scan_proc_ctrl->dvb_scan_proc_scene_para);
				dvb_scan_proc_ctrl->dvb_scan_proc_scene_para = NULL;
			}
			
			if(dvb_scan_proc_ctrl)
			{
				My_Mfree(0,dvb_scan_proc_ctrl);
				dvb_scan_proc_ctrl = NULL;
			}
		}
		return EPDK_OK;
		
		case GUI_MSG_PAINT:
			__here__;
			return dvb_scan_proc_loading_paint(msg);
	    case GUI_MSG_COMMAND:
		{
			 scan_cmd_proc(msg);
	         return EPDK_OK;
		}
		
		case GUI_MSG_KEY:
		{
			switch( msg->dwAddData1 )
			{			
				case GUI_MSG_KEY_ESCAPE:
				case GUI_MSG_KEY_LONGESCAPE:
				{
					if(msg->dwAddData2 == KEY_UP_ACTION)
					{
						__gui_msg_t my_msg;
						my_msg.id = GUI_MSG_COMMAND;
			    		my_msg.h_srcwin = NULL;
			    		my_msg.h_deswin = GUI_WinGetParent(msg->h_deswin);
			    		my_msg.dwAddData1 = DTV_BACK2PLAY_PROGRAM;
			    		my_msg.dwAddData2 = 0;
			    		GUI_SendNotifyMessage(&my_msg);
			    	}	
				}
				default :
					break;
			}		

		}
		return EPDK_OK;
		case GUI_MSG_TOUCH:
			return EPDK_OK;
		default:
			break;
	}
	
	return GUI_FrmWinDefaultProc(msg);	
}

/*
	建立窗口
*/
static H_WIN scan_win_create(H_WIN h_parent,dvb_scan_proc_para_t* para)
{
	__gui_framewincreate_para_t framewin_para;
	dvb_scan_proc_para_t *dvb_scan_proc_scene_para;
	FB fb;

	__wrn("****************************************************************************************\n");
	__wrn("********  enter dvb scan proc application  **************\n");
	__wrn("****************************************************************************************\n");

	GUI_LyrWinGetFB(para->layer, &fb);
	__here__;
	dvb_scan_proc_scene_para = (dvb_scan_proc_para_t *)My_Malloc(0,sizeof(dvb_scan_proc_para_t));
	if(!dvb_scan_proc_scene_para)
	{
		__msg("memory balloc fail.........\n");
		return NULL;
	}
	__here__;
	eLIBs_memset(dvb_scan_proc_scene_para,0,sizeof(dvb_scan_proc_para_t));	
	dvb_scan_proc_scene_para->font= para->font;
	dvb_scan_proc_scene_para->layer = para->layer;
	
	__here__;
	eLIBs_memset(&framewin_para, 0, sizeof(__gui_framewincreate_para_t));
	__here__;

	framewin_para.name 			= SUBMENU_DVB_SCAN;
	framewin_para.dwExStyle 	= WS_EX_NONE;
	framewin_para.dwStyle 		= WS_NONE|WS_VISIBLE;
	framewin_para.spCaption 	= NULL;
	framewin_para.hOwner		= NULL;
	framewin_para.hHosting 		= h_parent;
	framewin_para.id        	= SUBMENU_DVB_SCAN_ID;
	framewin_para.FrameWinProc 	= (__pGUI_WIN_CB)esKRNL_GetCallBack((__pCBK_t)_dvb_scan_proc_proc);
	framewin_para.rect.x 		= 0;
	framewin_para.rect.y 		= 0;
	framewin_para.rect.width	= fb.size.width;
	framewin_para.rect.height 	= fb.size.height;
	framewin_para.BkColor.alpha =  0;
	framewin_para.BkColor.red 	= 0;
	framewin_para.BkColor.green = 0;
	framewin_para.BkColor.blue 	= 0;
	framewin_para.attr 			=  (void *)dvb_scan_proc_scene_para;
	framewin_para.hLayer 		= para->layer;
	__here__;

	return (GUI_FrmWinCreate(&framewin_para));
}

void  dvb_scan_proc_win_delete(H_WIN dvb_scan_proc_win)
{
	if(dvb_scan_proc_win != NULL)
	{
		GUI_LyrWinDelete(GUI_WinGetLyrWin(dvb_scan_proc_win));
	}
}


H_WIN search_create(H_WIN hparent,H_LYR layer)
{
	dvb_scan_proc_para_t dvb_scan_proc_scene_para;
	
	dvb_scan_proc_uipara_t* dvb_scan_proc_uipara;
	
	H_WIN h_wnd;
	
	dvb_scan_proc_scene_para.layer = layer;
	
	dvb_scan_proc_scene_para.font = SWFFont;
	
	h_wnd = scan_win_create(hparent,&dvb_scan_proc_scene_para);
	
	return h_wnd;
	
}
