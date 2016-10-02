/*
**************************************************************************************************************
*											         ePDK
*						            the Easy Portable/Player Develop Kits
*									           desktop system 
*
*						        	 (c) Copyright 2007-2010, ANDY, China
*											 All Rights Reserved
*
* File    	: dtv_settings.c
* By      	: 
* Func		: 
* Version	: v1.0
* ============================================================================================================
*
**************************************************************************************************************
*/

#include "dtv_setting.h"
#include "dtv_setting_i.h"
#include "dtv_setting_uipara.h"


extern dtv_ctr_t  dtv_ctr;


static __s32 _dtv_setting_Proc(__gui_msg_t *msg)
{
	dtv_setting_ctrl_t *dtv_setting_ctrl;
	dtv_setting_ctrl	 = (dtv_setting_ctrl_t *)GUI_WinGetAttr(msg->h_deswin);
	if(!dtv_setting_ctrl)
	{
		__wrn("dtv_setting_ctrl is null ......\n");
		return EPDK_FAIL;
	}


	switch(msg->id)
	{
		case GUI_MSG_CREATE:
		{
			dtv_setting_list_para_t list_para={0};
			list_para.focus_id = 0;
			list_para.old_focus = 0;
			list_para.layer = dtv_setting_ctrl->layer;
			dtv_setting_ctrl->h_frm_list = dtv_setting_list_win_create(msg->h_deswin,&list_para);
			if(dtv_setting_ctrl->h_frm_list)
			{
				GUI_WinSetFocusChild(dtv_setting_ctrl->h_frm_list);
				GUI_SetTimer(msg->h_deswin,SUBMENU_DVB_SETTING_TIMERID,500,NULL);
				__msg("set timer ok...\n");
			}
			else
			{
				return EPDK_FAIL;
			}
		}
		return EPDK_OK;

		case GUI_MSG_CLOSE:
		{
			
    		__gui_msg_t my_msg;
            __wrn("-------------------------------------GUI_MSG_CLOSE enter, err.\n");

    		my_msg.id = GUI_MSG_COMMAND;
    		my_msg.h_srcwin = NULL;
    		my_msg.h_deswin = GUI_WinGetParent(msg->h_deswin);
    		my_msg.dwAddData1 = DTV_BACK2PLAY_PROGRAM;
    		my_msg.dwAddData2 = 0;
    		GUI_SendNotifyMessage(&my_msg);											
		}
		return EPDK_OK;
		case GUI_MSG_DESTROY:
		{
			GUI_KillTimer(msg->h_deswin,SUBMENU_DVB_SETTING_TIMERID);	
			if(dtv_ctr.h_menu_layer)
			{
                dtv_setting_list_win_delete(dtv_setting_ctrl->h_frm_list);
				GUI_LyrWinDelete(dtv_setting_ctrl->layer);
				dtv_setting_ctrl->layer = NULL;
				dtv_ctr.h_menu_layer = NULL ;
				dtv_setting_ctrl->h_frm_list = NULL;
			}
			__wrn("...\n");
			if(dtv_setting_ctrl)
			{
				My_Bfree(dtv_setting_ctrl, sizeof(dtv_setting_ctrl_t));
				dtv_setting_ctrl = NULL;
			}

		}
		return EPDK_OK;
		case GUI_MSG_KEY:
		{
			if(GUI_IsTimerInstalled(msg->h_deswin,SUBMENU_DVB_SETTING_TIMERID))
			{
				GUI_ResetTimer(msg->h_deswin,SUBMENU_DVB_SETTING_TIMERID,500,NULL);
			}
		}
		break;
		
		case GUI_MSG_TIMER:
		{
			__msg("timer id = %d \n",msg->dwAddData1);
			//500秒无操作时通知上层删除设置界面
			if(msg->dwAddData1 == SUBMENU_DVB_SETTING_TIMERID)
			{
				{
					__gui_msg_t dmsg;
				
					dmsg.id = GUI_MSG_COMMAND;
					dmsg.h_deswin = GUI_WinGetParent(msg->h_deswin);
					dmsg.dwAddData1 = DTV_BACK2PLAY_PROGRAM ;
				
					GUI_SendNotifyMessage(&dmsg);
				}
			}							
		}
		return EPDK_OK;
		
		case GUI_MSG_COMMAND:
		{
			switch( msg->dwAddData1)
			{
				case DTV_SETTING_SEARCH:
				{
					{
						__gui_msg_t dmsg;
					
						dmsg.id = GUI_MSG_COMMAND;
						dmsg.h_deswin = GUI_WinGetParent(msg->h_deswin);
						dmsg.dwAddData1 = DTV_RESEARCH_SERVICE_LIST ;
					
						GUI_SendNotifyMessage(&dmsg);
					}

					return EPDK_OK;
				}
				case DTV_SETTING_PREVIEW:
				{
					{
						__gui_msg_t dmsg;
					
						dmsg.id = GUI_MSG_COMMAND;
						dmsg.h_deswin = GUI_WinGetParent(msg->h_deswin);
						dmsg.dwAddData1 = DTV_PREVIEW_SERVICE_LIST ;
					
						GUI_SendNotifyMessage(&dmsg);
					}

					return EPDK_OK;
				}				
				case DTV_SETTING_DISPLAY:
				{
					dtv_setting_disp_para_t list_para={0};

					GUI_LyrWinSel(dtv_setting_ctrl->layer);
                    
                    if(GUI_LYRWIN_STA_ON != GUI_LyrWinGetSta(dtv_setting_ctrl->layer)
                           &&GUI_LYRWIN_STA_SLEEP != GUI_LyrWinGetSta(dtv_setting_ctrl->layer))
                   	{
                        __wrn("------------------------Err Frame Buf fail.\n");                                         
                   	}
                    
	                else
	                {
	                	GUI_Clear();
	                }

					
                    GUI_FrmWinDelete(dtv_setting_ctrl->h_frm_list);
					dtv_setting_ctrl->h_frm_list = NULL;
					dtv_setting_ctrl->focus_id = DTV_SETTING_DISPLAY;
					
					list_para.focus_id = maple_get_video_window_ratio_mode()-1;
					list_para.old_focus = maple_get_video_window_ratio_mode()-1;
					list_para.layer = dtv_setting_ctrl->layer;					
					dtv_setting_ctrl->h_frm_list = dtv_setting_disp_win_create(msg->h_deswin,&list_para);//创建新的listbar

                    
					GUI_WinSetFocusChild(dtv_setting_ctrl->h_frm_list);
					if(GUI_IsTimerInstalled(msg->h_deswin,SUBMENU_DVB_SETTING_TIMERID))
					{
						GUI_ResetTimer(msg->h_deswin,SUBMENU_DVB_SETTING_TIMERID,500,NULL);
					}
					return EPDK_OK;
				}

				case DTV_SETTING_BACK2LIST:
				{
					dtv_setting_list_para_t list_para;
					__here__;

					GUI_LyrWinSel(dtv_setting_ctrl->layer);
                    
                    if(GUI_LYRWIN_STA_ON != GUI_LyrWinGetSta(dtv_setting_ctrl->layer)
                          &&GUI_LYRWIN_STA_SLEEP != GUI_LyrWinGetSta(dtv_setting_ctrl->layer))
                    {
                        __wrn("------------------------Err Frame Buf fail.\n");            
                      
                    }
                    
                    else
                    {
                        GUI_Clear();
                    }
					
                    
                    GUI_FrmWinDelete(dtv_setting_ctrl->h_frm_list);
					dtv_setting_ctrl->h_frm_list = NULL;

                    
					list_para.focus_id = dtv_setting_ctrl->focus_id;
					list_para.old_focus = dtv_setting_ctrl->focus_id;
					list_para.layer = dtv_setting_ctrl->layer;
					__here__;
					dtv_setting_ctrl->h_frm_list = dtv_setting_list_win_create(msg->h_deswin,&list_para);
                    
					GUI_WinSetFocusChild(dtv_setting_ctrl->h_frm_list);
					if(GUI_IsTimerInstalled(msg->h_deswin,SUBMENU_DVB_SETTING_TIMERID))
					{
						GUI_ResetTimer(msg->h_deswin,SUBMENU_DVB_SETTING_TIMERID,500,NULL);
					}
					return EPDK_OK;
				}		

				case DTV_SETTING_REPLAY:
				{
					__gui_msg_t dmsg={0};

				    __wrn("dtv_ctr.h_setting    : 0x%08x\n", dtv_ctr.h_setting);
					dmsg.id = GUI_MSG_COMMAND;
					dmsg.h_deswin = GUI_WinGetParent(msg->h_deswin);
					dmsg.dwAddData1 = DTV_BACK2PLAY_PROGRAM ;
				
					GUI_SendNotifyMessage(&dmsg);

					return EPDK_OK;
				}

				case DTV_SETTING_TRACK:
				{
					__gui_msg_t dmsg;
				
					dmsg.id = GUI_MSG_COMMAND;
					dmsg.h_deswin = GUI_WinGetParent(msg->h_deswin);
					dmsg.dwAddData1 = DTV_AUDIO;
				
					GUI_SendNotifyMessage(&dmsg);
					return EPDK_OK;
				}
                
				case DTV_SETTING_LCN:
				{
					dtv_setting_lcn_para_t list_para={0};

					GUI_LyrWinSel(dtv_setting_ctrl->layer);
                    
                    if(GUI_LYRWIN_STA_ON != GUI_LyrWinGetSta(dtv_setting_ctrl->layer)
                           &&GUI_LYRWIN_STA_SLEEP != GUI_LyrWinGetSta(dtv_setting_ctrl->layer))
                   	{
                       		__wrn("------------------------Err Frame Buf fail.\n");                                         
                   	}		
	                else
	                {
	                	GUI_Clear();
	                }

					
                    GUI_FrmWinDelete(dtv_setting_ctrl->h_frm_list);
					dtv_setting_ctrl->h_frm_list = NULL;
					dtv_setting_ctrl->focus_id = DTV_SETTING_LCN;
					
					list_para.focus_id = maple_get_lcn_state();
					list_para.old_focus = maple_get_lcn_state();
					list_para.layer = dtv_setting_ctrl->layer;					
					dtv_setting_ctrl->h_frm_list = dtv_setting_lcn_win_create(msg->h_deswin,&list_para);//创建新的listbar

                    
					GUI_WinSetFocusChild(dtv_setting_ctrl->h_frm_list);
					if(GUI_IsTimerInstalled(msg->h_deswin,SUBMENU_DVB_SETTING_TIMERID))
					{
						GUI_ResetTimer(msg->h_deswin,SUBMENU_DVB_SETTING_TIMERID,500,NULL);
					}
					return EPDK_OK;

				}


				case DTV_SETTING_ANTENNA:
				{
					dtv_setting_antenna_para_t list_para={0};

					GUI_LyrWinSel(dtv_setting_ctrl->layer);
                    
                    if(GUI_LYRWIN_STA_ON != GUI_LyrWinGetSta(dtv_setting_ctrl->layer)
                           &&GUI_LYRWIN_STA_SLEEP != GUI_LyrWinGetSta(dtv_setting_ctrl->layer))
                   	{
                       		__wrn("------------------------Err Frame Buf fail.\n");                                         
                   	}		
	                else
	                {
	                	GUI_Clear();
	                }

					
                    GUI_FrmWinDelete(dtv_setting_ctrl->h_frm_list);
					dtv_setting_ctrl->h_frm_list = NULL;
					dtv_setting_ctrl->focus_id = DTV_SETTING_ANTENNA;
					
					list_para.focus_id = maple_get_antenna_state();
					list_para.old_focus = maple_get_antenna_state();
					list_para.layer = dtv_setting_ctrl->layer;					
					dtv_setting_ctrl->h_frm_list = dtv_setting_antenna_win_create(msg->h_deswin,&list_para);//创建新的listbar                    
					GUI_WinSetFocusChild(dtv_setting_ctrl->h_frm_list);
					if(GUI_IsTimerInstalled(msg->h_deswin,SUBMENU_DVB_SETTING_TIMERID))
					{
						GUI_ResetTimer(msg->h_deswin,SUBMENU_DVB_SETTING_TIMERID,500,NULL);
					}
					
					return EPDK_OK;

				}

				case DTV_SETTING_EPG:
				{
					__gui_msg_t dmsg;
				
					dmsg.id = GUI_MSG_COMMAND;
					dmsg.h_deswin = GUI_WinGetParent(msg->h_deswin);
					dmsg.dwAddData1 = DTV_EPG;
				
					GUI_SendNotifyMessage(&dmsg);

					return EPDK_OK;
				}


				case DTV_SETTING_SSU_UPDATE:
				{
					return EPDK_OK;
				}



			}
			
	    }	
		return EPDK_OK;
		
		default:
			break;
	}
	return GUI_ManWinDefaultProc(msg);	
}

H_WIN setting_create(H_WIN	h_parent,H_LYR layer)

{
	__gui_manwincreate_para_t create_info;
	dtv_setting_ctrl_t *dtv_setting_ctrl;

	
	__inf("************************************************\n");
	__inf("********  enter dtv_setting menu  **************\n");
	__inf("************************************************\n");

	dtv_setting_ctrl = (dtv_setting_ctrl_t *)My_Balloc(sizeof(dtv_setting_ctrl_t));
	if(!dtv_setting_ctrl)
	{
		__msg("memory balloc fail.........\n");
		return NULL;
	}
	eLIBs_memset(dtv_setting_ctrl, 0, sizeof(dtv_setting_ctrl_t));

	dtv_setting_ctrl->dtv_setting_font = SWFFont;
	dtv_setting_ctrl->layer = layer;

	eLIBs_memset(&create_info, 0, sizeof(__gui_manwincreate_para_t));
	
	create_info.name            = SUBMENU_DVB_SETTING;
	create_info.hParent         = h_parent;
	create_info.ManWindowProc   = (__pGUI_WIN_CB)esKRNL_GetCallBack((__pCBK_t)_dtv_setting_Proc); 
	create_info.attr            = (void*)dtv_setting_ctrl;
	
	create_info.id			= SUBMENU_DVB_SETTING_ID;
	create_info.hHosting        = NULL;
	
	return(GUI_ManWinCreate(&create_info));
}

