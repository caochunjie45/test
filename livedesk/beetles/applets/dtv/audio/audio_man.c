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

#include "audio_man.h"

extern dtv_ctr_t  dtv_ctr;

static __s32 _audio_preview_Proc(__gui_msg_t *msg)
{
	audio_preview_ctrl_t *audio_preview_ctrl;
	audio_preview_ctrl	 = (audio_preview_ctrl_t *)GUI_WinGetAttr(msg->h_deswin);
	if(!audio_preview_ctrl)
	{
		__wrn("audio_preview_ctrl is null ......\n");
		return EPDK_FAIL;
	}


	switch(msg->id)
	{
		case GUI_MSG_CREATE:
		{
			dtv_track_para_t list_para;
			
			list_para.layer = audio_preview_ctrl->layer;
			
			audio_preview_ctrl->h_frm_list = audio_win_create(msg->h_deswin,&list_para);

			GUI_WinSetFocusChild(audio_preview_ctrl->h_frm_list);

			GUI_SetTimer(msg->h_deswin, SUBMENU_DVB_TRACE_TIMERID,500, NULL);
            
		}
		return EPDK_OK;

		case GUI_MSG_CLOSE:
		{
            __wrn("-------------------------------------GUI_MSG_CLOSE enter, err.\n");
			GUI_FrmWinDelete(audio_preview_ctrl->h_frm_list);
		}
		return EPDK_OK;
		case GUI_MSG_DESTROY:
		{

			if(GUI_IsTimerInstalled(msg->h_deswin,SUBMENU_DVB_TRACE_TIMERID))
			{
				GUI_KillTimer(msg->h_deswin, SUBMENU_DVB_TRACE_TIMERID);
			}
					
			if(dtv_ctr.h_menu_layer)
			{
				GUI_LyrWinDelete(audio_preview_ctrl->layer);
				dtv_ctr.h_menu_layer = NULL ;
				audio_preview_ctrl->layer = NULL;
				audio_preview_ctrl->h_frm_list = NULL;
			}		
			My_Bfree(audio_preview_ctrl, sizeof(audio_preview_ctrl_t));
		}
		return EPDK_OK;
		case GUI_MSG_KEY:
		{
			if(GUI_IsTimerInstalled(msg->h_deswin,SUBMENU_DVB_TRACE_TIMERID))
			{
				GUI_ResetTimer(msg->h_deswin, SUBMENU_DVB_TRACE_TIMERID,500, NULL);
			}

			switch( msg->dwAddData1 )
			{
				case GUI_MSG_KEY_ESCAPE:
				case GUI_MSG_KEY_ENTER:
				case GUI_MSG_KEY_LONGESCAPE:
				case GUI_MSG_KEY_LONGENTER:
				{
					if((msg->dwAddData2 == KEY_UP_ACTION)) 
					{
						__gui_msg_t my_msg;
						
						my_msg.id = GUI_MSG_COMMAND;
						my_msg.h_srcwin = NULL;
						my_msg.h_deswin = GUI_WinGetParent(msg->h_deswin);
						my_msg.dwAddData1 = DTV_BACK2PLAY_PROGRAM;
						my_msg.dwAddData2 = 0;
						GUI_SendNotifyMessage(&my_msg);
						return EPDK_OK;
					}
					break;
				}
				default :
					break;
			}
			
		}
		break;
		case GUI_MSG_TIMER:
		{
			if(msg->dwAddData1 == SUBMENU_DVB_TRACE_TIMERID)
			{
				__gui_msg_t my_msg;
				
				my_msg.id = GUI_MSG_COMMAND;
				my_msg.h_srcwin = NULL;
				my_msg.h_deswin = GUI_WinGetParent(msg->h_deswin);
				my_msg.dwAddData1 = DTV_BACK2PLAY_PROGRAM;
				my_msg.dwAddData2 = 0;
				GUI_SendNotifyMessage(&my_msg);
				return EPDK_OK;
			}
			break;
		}
		case GUI_MSG_COMMAND:
		{
	    }	
		break;
	default:
		break;
	}
	return GUI_ManWinDefaultProc(msg);	
}


H_WIN audio_create(H_WIN	h_parent,H_LYR layer)

{
	__gui_manwincreate_para_t create_info;
	audio_preview_ctrl_t *audio_preview_ctrl;

	__msg("****************************************************\n");
	__msg("********  enter program_preview menu  **************\n");
	__msg("****************************************************\n");

	audio_preview_ctrl = (audio_preview_ctrl_t *)My_Balloc(sizeof(audio_preview_ctrl_t));
	if(!audio_preview_ctrl)
	{
		__msg("memory balloc fail.........\n");
		return EPDK_FAIL;
	}
	eLIBs_memset(audio_preview_ctrl, 0, sizeof(audio_preview_ctrl_t));

	audio_preview_ctrl->audio_preview_font = SWFFont;
	audio_preview_ctrl->layer = layer;

	eLIBs_memset(&create_info, 0, sizeof(__gui_manwincreate_para_t));
	
	create_info.name            = SUBMENU_DVB_TRACE;
	create_info.hParent         = h_parent;
	create_info.ManWindowProc   = (__pGUI_WIN_CB)esKRNL_GetCallBack((__pCBK_t)_audio_preview_Proc); 
	create_info.attr            = (void*)audio_preview_ctrl;
	
	create_info.id			= SUBMENU_DVB_TRACKID;
	create_info.hHosting        = NULL;
	
	return(GUI_ManWinCreate(&create_info));
}

