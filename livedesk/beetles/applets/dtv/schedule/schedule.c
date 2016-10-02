/*
**************************************************************************************************************
*											         ePDK
*						            the Easy Portable/Player Develop Kits
*									           desktop system 
*
*						        	 (c) Copyright 2007-2010, ANDY, China
*											 All Rights Reserved
*
* File    	: schedules.c
* By      	: 
* Func		: 
* Version	: v1.0
* ============================================================================================================
*
**************************************************************************************************************
*/

#include "schedule.h"

#include "schedule_i.h"

#include "schedule_uipara.h"

extern dtv_ctr_t  dtv_ctr;


static __s32 _schedule_Proc(__gui_msg_t *msg)
{
	schedule_ctrl_t *schedule_ctrl;
	schedule_ctrl	 = (schedule_ctrl_t *)GUI_WinGetAttr(msg->h_deswin);
	if(!schedule_ctrl)
	{
		return EPDK_FAIL;
	}


	switch(msg->id)
	{
		case GUI_MSG_CREATE:
		{
			schedule_list_para_t list_para;
           
			list_para.focus_id = 0;
			list_para.old_focus = 0;
			list_para.layer = schedule_ctrl->layer;
			
			schedule_ctrl->h_frm_list = schedule_list_win_create(msg->h_deswin,&list_para);

			if(NULL == schedule_ctrl->h_frm_list)
			{
				return EPDK_FAIL;
			}
			GUI_WinSetFocusChild(schedule_ctrl->h_frm_list);
			schedule_ctrl->destroy_timerid = SUBMENU_DVB_SCHUDULE_DESTROY_TIMERID;
			if(!GUI_IsTimerInstalled(msg->h_deswin,schedule_ctrl->destroy_timerid))
			{
				GUI_SetTimer(msg->h_deswin,schedule_ctrl->destroy_timerid,1000,NULL);
			}
		}
		return EPDK_OK;

		case GUI_MSG_CLOSE:
		{
            __wrn("-------------------------------------GUI_MSG_CLOSE enter, err.\n");
			GUI_FrmWinDelete(schedule_ctrl->h_frm_list);
		}
		return EPDK_OK;
		
		case GUI_MSG_DESTROY:
		{
			if(GUI_IsTimerInstalled(msg->h_deswin,schedule_ctrl->destroy_timerid))
			{
				GUI_KillTimer(msg->h_deswin,schedule_ctrl->destroy_timerid);
				schedule_ctrl->destroy_timerid = 0;
			}			
			if(dtv_ctr.h_menu_layer)
			{
				GUI_LyrWinDelete(schedule_ctrl->layer);
				schedule_ctrl->layer = NULL;
				dtv_ctr.h_menu_layer = NULL ;
				schedule_ctrl->h_frm_list = 0;
			}		
			My_Bfree(schedule_ctrl, sizeof(schedule_ctrl_t));

		}
		return EPDK_OK;
		case GUI_MSG_KEY:
		{
			if(GUI_IsTimerInstalled(msg->h_deswin,schedule_ctrl->destroy_timerid))
			{
				GUI_ResetTimer(msg->h_deswin,schedule_ctrl->destroy_timerid,1000,NULL);
			}
			switch( msg->dwAddData1 )
			{			
				case GUI_MSG_KEY_ESCAPE:
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
    					return EPDK_OK;
					}
				}
				default:
					break;

			}
			
		}
		break;

		case GUI_MSG_TIMER:
		{	
			if(msg->dwAddData1 == schedule_ctrl->destroy_timerid)
			{
				GUI_KillTimer(msg->h_deswin,schedule_ctrl->destroy_timerid);
				schedule_ctrl->destroy_timerid = 0;
				{
					__gui_msg_t my_msg;

					my_msg.id         = GUI_MSG_COMMAND;
		    		my_msg.h_srcwin   = NULL;
		    		my_msg.h_deswin   = GUI_WinGetParent(msg->h_deswin);
		    		my_msg.dwAddData1 = DTV_BACK2PLAY_PROGRAM;
		    		my_msg.dwAddData2 = 0;
		    		GUI_SendNotifyMessage(&my_msg);
					return EPDK_OK;
				}

				
			}
		}
		break;
		
		case GUI_MSG_COMMAND:
		{
	    }	
		break;
	default:
		break;
	}
	return GUI_ManWinDefaultProc(msg);	
}

H_WIN schedule_create(H_WIN	h_parent,H_LYR layer)
{
	__gui_manwincreate_para_t create_info;
	schedule_ctrl_t *schedule_ctrl;

	__msg("****************************************************************************************\n");
	__msg("********  enter schedule menu  **************\n");
	__msg("****************************************************************************************\n");

	schedule_ctrl = (schedule_ctrl_t *)My_Balloc(sizeof(schedule_ctrl_t));
	if(!schedule_ctrl)
	{
		__msg("memory balloc fail.........\n");
		return EPDK_FAIL;
	}
	eLIBs_memset(schedule_ctrl, 0, sizeof(schedule_ctrl_t));

	schedule_ctrl->schedule_font = SWFFont;
	schedule_ctrl->layer = layer;

	eLIBs_memset(&create_info, 0, sizeof(__gui_manwincreate_para_t));
	
	create_info.name            = SUBMENU_DVB_SCHUDULE;
	create_info.hParent         = h_parent;
	create_info.ManWindowProc   = (__pGUI_WIN_CB)esKRNL_GetCallBack((__pCBK_t)_schedule_Proc); 
	create_info.attr            = (void*)schedule_ctrl;
	
	create_info.id			= SUBMENU_DVB_SCHUDULE_ID;
	create_info.hHosting        = NULL;

	return(GUI_ManWinCreate(&create_info));
}

