/*
**************************************************************************************************************
*											         ePDK
*						            the Easy Portable/Player Develop Kits
*									           desktop system 
*
*						        	 (c) Copyright 2007-2010, ANDY, China
*											 All Rights Reserved
*
* File    	: app_settings.c
* By      	: Andy.zhang
* Func		: desk main thread
* Version	: v1.0
* ============================================================================================================
* 2009-7-20 8:51:52  andy.zhang  create this file, implements the fundemental interface;
**************************************************************************************************************
*/

#include "App_setting.h"
#include "app_setting_i.h"
#include "setting_para.h"
#include "setting_general.h"
#include "setting_power.h"
#include "setting_tips.h"
/***********************************************************************************************************
	建立图层
************************************************************************************************************/
static H_LYR setting_layer_palette_create(RECT *rect)
{
	H_LYR layer = NULL;
	FB  fb =
	{
	    {0, 0},                                   		/* size      */
	    {0, 0, 0},                                      /* buffer    */
	    {FB_TYPE_RGB, {PIXEL_MONO_8BPP, 0, (__rgb_seq_t)0}},    /* fmt       */
	};

	__disp_layer_para_t lstlyr =
	{
	    DISP_LAYER_WORK_MODE_PALETTE,                    /* mode      */
	    0,                                              /* ck_mode   */
	    0,                                              /* alpha_en  */
	    0,                                              /* alpha_val */
	    1,                                              /* pipe      */
	    0xff,                                           /* prio      */
	    {0, 0, 0, 0},                           		/* screen    */
	    {0, 0, 0, 0},                               	/* source    */
	    DISP_LAYER_OUTPUT_CHN_DE_CH1,                   /* channel   */
	    NULL                                            /* fb        */
	};

	__layerwincreate_para_t lyrcreate_info =
	{
	    "setting layer",
	    NULL,
	    GUI_LYRWIN_STA_SUSPEND,
	    GUI_LYRWIN_NORMAL
	};
	
	fb.size.width		= rect->width;
	fb.size.height		= rect->height;	
	
	lstlyr.src_win.x  		= 0;
	lstlyr.src_win.y  		= 0;
	lstlyr.src_win.width 	= rect->width;
	lstlyr.src_win.height 	= rect->height;
	
	lstlyr.scn_win.x		= rect->x;
	lstlyr.scn_win.y		= rect->y;
	lstlyr.scn_win.width  	= rect->width;
	lstlyr.scn_win.height 	= rect->height;
	
	lstlyr.pipe = 1;
	lstlyr.fb = &fb;
	lyrcreate_info.lyrpara = &lstlyr;
	
	layer = GUI_LyrWinCreate(&lyrcreate_info);
	if( !layer )
	{
		__err("app bar layer create error !\n");
	} 
		
	return layer;	
}

static __s32 setting_win_create(__gui_msg_t *msg)
{
	setting_ctrl_t	*setting_ctrl;
	RECT rect;
	SIZE screen_size;

	setting_ctrl = (setting_ctrl_t *)GUI_WinGetAttr(msg->h_deswin);

	rect.x = 0;
	rect.y = 0;
	dsk_display_get_size(&screen_size.width,&screen_size.height);
	rect.width = screen_size.width;
	rect.height = screen_size.height;
	
	setting_ctrl->lyr_setting = setting_layer_palette_create(&rect);
		
	if (setting_ctrl->root_type == SETTING_GENERAL)
	{
		setting_general_para_t general_para;
		
		general_para.layer = setting_ctrl->lyr_setting;
		general_para.font = setting_ctrl->setting_font;
		general_para.focus_id = 0;
		setting_ctrl->h_frm_general = setting_general_win_create(msg->h_deswin, &general_para);
		GUI_WinSetFocusChild(setting_ctrl->h_frm_general);
	}
	else if (setting_ctrl->root_type == SETTING_POWER_MANAGE)
	{
		setting_power_para_t power_para;
		
		power_para.layer = setting_ctrl->lyr_setting;
		power_para.font = setting_ctrl->setting_font;
		power_para.focus_id = 0;
		setting_ctrl->h_frm_power = setting_power_win_create(msg->h_deswin, &power_para);
		GUI_WinSetFocusChild(setting_ctrl->h_frm_power);
	}

	return EPDK_OK;
}

static __s32 _app_setting_Proc(__gui_msg_t *msg)
{
	switch(msg->id)
	{
		case GUI_MSG_CREATE:
		{
			gscene_bgd_set_state(BGD_STATUS_SHOW);   //cai++
			gscene_bgd_set_bottom();  //cai++
			com_set_palette_by_id(ID_SETTING_WKM_BMP);
			setting_win_create(msg);
		}
		return EPDK_OK;
		case GUI_MSG_CLOSE:
		{
			main_cmd2parent(msg->h_deswin, SWITCH_TO_OTHER_APP, SETTING_SW_TO_MAIN, 0);
		}
		return EPDK_OK;
		case GUI_MSG_DESTROY:
		{
			setting_ctrl_t	*setting_ctrl;
			__msg( "setting destory begin ............\n");
			setting_ctrl	 = (setting_ctrl_t *)GUI_WinGetAttr(msg->h_deswin);
			if(!setting_ctrl)
			{
				__err("setting_ctrl is null ......\n");
				return EPDK_FAIL;
			}
			//判断产品信息提示框是否存在，存在的话删除该提示框
			if(setting_ctrl->h_frm_tips)
			{
				H_LYR h_lyr ;
				h_lyr = GUI_WinGetLyrWin( setting_ctrl->h_frm_tips );
				if( !h_lyr )
				{
					__msg( "h_lyr is null \n");
					return EPDK_FAIL ;
				}
				GUI_LyrWinDelete( h_lyr );
				setting_ctrl->h_frm_tips = NULL ;
			}
			__here__;
			//判断恢复出厂设置提示框是否存在，存在的话删除该提示框
			if( setting_ctrl->h_dialoag_win )
			{
				app_dialog_destroy( setting_ctrl->h_dialoag_win );
				setting_ctrl->h_dialoag_win = NULL ;
			}
			if( setting_ctrl->lyr_setting)
			{
				__here__ ;
				__msg(" lyr_setting isn't null, delete it \n");
				GUI_LyrWinDelete(setting_ctrl->lyr_setting);
				__here__ ;
				setting_ctrl->lyr_setting = NULL ;
			}
			My_Bfree(setting_ctrl, sizeof(setting_ctrl_t));
		//	esKSRV_SysInfo();
		}
		return EPDK_OK;
		case GUI_MSG_KEY:
		{
		
			switch( msg->dwAddData1 )
			{			
				case GUI_MSG_KEY_ESCAPE:
				{
					__gui_msg_t my_msg;

					my_msg.id = GUI_MSG_CLOSE;
		    		my_msg.h_srcwin = msg->h_deswin;
		    		my_msg.h_deswin = msg->h_deswin;
		    		my_msg.dwAddData1 = 0;
		    		my_msg.dwAddData2 = 0;
		    		GUI_SendNotifyMessage(&my_msg);
					return EPDK_OK;
				}
			}
			
		}
		break;
		case GUI_MSG_COMMAND:
		{
			setting_ctrl_t *setting_ctrl;

			setting_ctrl = (setting_ctrl_t *)GUI_WinGetAttr(msg->h_deswin);
			
			switch(LOWORD(msg->dwAddData1))
			{			
				case GENERAL_MENU_ID:
				{
					
					switch(HIWORD(msg->dwAddData1))
					{
						case ID_OP_LEFT:
						{
							__gui_msg_t mymsg;
							mymsg.h_deswin = setting_ctrl->h_frm_general;
							mymsg.h_srcwin = NULL;
							mymsg.id = MSG_OP_LEFT;
							GUI_SendMessage(&mymsg);
						}
						break;
						case ID_OP_RIGHT:
						{
							__gui_msg_t mymsg;
							mymsg.h_deswin = setting_ctrl->h_frm_general;
							mymsg.h_srcwin = NULL;
							mymsg.id = MSG_OP_RIGHT;
							GUI_SendMessage(&mymsg);
						}
						break;
						case ID_OP_UP:
						{
							__gui_msg_t mymsg;
							mymsg.h_deswin = setting_ctrl->h_frm_general;
							mymsg.h_srcwin = NULL;
							mymsg.id = MSG_OP_UP;
							GUI_SendMessage(&mymsg);
						}
						break;
						case ID_OP_DOWN:
						{
							__gui_msg_t mymsg;
							mymsg.h_deswin = setting_ctrl->h_frm_general;
							mymsg.h_srcwin = NULL;
							mymsg.id = MSG_OP_DOWN;
							GUI_SendMessage(&mymsg);
							
						}
						break;	
						case ID_OP_ENTER:
						{
							__gui_msg_t mymsg;
							mymsg.h_deswin = setting_ctrl->h_frm_general;
							mymsg.h_srcwin = NULL;
							mymsg.id = MSG_OP_ENTER;
							GUI_SendMessage(&mymsg);
						}
						break;
						case ID_OP_EXIT:
						{
							__gui_msg_t mymsg;
							mymsg.h_deswin = setting_ctrl->h_frm_general;
							mymsg.h_srcwin = NULL;
							mymsg.id = MSG_OP_EXIT;
							GUI_SendMessage(&mymsg);
						}
						break;
						case ID_SWITCH_TO_TIPS:
						{
							if(msg->dwAddData2 ==TIPS_FACTORY_DEFAULT)	//恢复出厂设置
							{
								 __s32 lang_id[]={STRING_SET_CUE , STRING_SET_COMMON_RESTORE };
           							 default_dialog(setting_ctrl->h_dialoag_win , msg->h_deswin , SETTING_TIPS_ID, ADLG_YESNO, lang_id);
							}
							else
							{		// 产品信息提示框
								setting_tip_para_t tips_para;
								RECT rect;						
								
								rect.x = 0;
								rect.y = 0;
								rect.width = SET_TIPS_W;
								rect.height = SET_TIPS_H;
								
								tips_para.msgbox_font = setting_ctrl->setting_font;
								tips_para.layer = setting_tips_layer_create(&rect);
								setting_ctrl->h_frm_tips = setting_tips_win_create(GUI_WinGetHandFromName(APP_SETTING),&tips_para);
								GUI_WinSetFocusChild(setting_ctrl->h_frm_tips);
							}
						}
						break;
					default:
						break;
					}
				}
					return EPDK_OK;

				case POWER_MANAGE_MENU_ID:
				{
					
					switch(HIWORD(msg->dwAddData1))
					{
						case ID_OP_LEFT:
						{
							__gui_msg_t mymsg;
							mymsg.h_deswin = setting_ctrl->h_frm_power;
							mymsg.h_srcwin = NULL;
							mymsg.id = MSG_OP_LEFT;
							GUI_SendMessage(&mymsg);
						}
						break;
						case ID_OP_RIGHT:
						{
							__gui_msg_t mymsg;
							mymsg.h_deswin = setting_ctrl->h_frm_power;
							mymsg.h_srcwin = NULL;
							mymsg.id = MSG_OP_RIGHT;
							GUI_SendMessage(&mymsg);
						}
						break;
						case ID_OP_UP:
						{
							__gui_msg_t mymsg;
							mymsg.h_deswin = setting_ctrl->h_frm_power;
							mymsg.h_srcwin = NULL;
							mymsg.id = MSG_OP_UP;
							GUI_SendMessage(&mymsg);
						}
						break;
						case ID_OP_DOWN:
						{
							__gui_msg_t mymsg;
							mymsg.h_deswin = setting_ctrl->h_frm_power;
							mymsg.h_srcwin = NULL;
							mymsg.id = MSG_OP_DOWN;
							GUI_SendMessage(&mymsg);
							
						}
						break;	
						case ID_OP_ENTER:
						{
							__gui_msg_t mymsg;
							mymsg.h_deswin = setting_ctrl->h_frm_power;
							mymsg.h_srcwin = NULL;
							mymsg.id = MSG_OP_ENTER;
							GUI_SendMessage(&mymsg);
						}
						break;
						case ID_OP_EXIT:
						{
							__gui_msg_t mymsg;
							mymsg.h_deswin = setting_ctrl->h_frm_power;
							mymsg.h_srcwin = NULL;
							mymsg.id = MSG_OP_EXIT;
							GUI_SendMessage(&mymsg);
						}
						break;
						
					default:
						break;
					}
				}
				return EPDK_OK;
				case GENERAL_TIPS_ID:
				{
					switch(HIWORD(msg->dwAddData1))
					{
						case ID_OP_LEFT:
						{
							__gui_msg_t mymsg;
							mymsg.h_deswin = setting_ctrl->h_frm_tips;
							mymsg.h_srcwin = NULL;
							mymsg.id = MSG_OP_LEFT;
							GUI_SendMessage(&mymsg);
						}
						break;
						case ID_OP_RIGHT:
						{
							__gui_msg_t mymsg;
							mymsg.h_deswin = setting_ctrl->h_frm_tips;
							mymsg.h_srcwin = NULL;
							mymsg.id = MSG_OP_RIGHT;
							GUI_SendMessage(&mymsg);
						}
						break;
						case ID_OP_UP:
						{
							__gui_msg_t mymsg;
							mymsg.h_deswin = setting_ctrl->h_frm_tips;
							mymsg.h_srcwin = NULL;
							mymsg.id = MSG_OP_UP;
							GUI_SendMessage(&mymsg);
						}
						break;
						case ID_OP_DOWN:
						{
							__gui_msg_t mymsg;
							mymsg.h_deswin = setting_ctrl->h_frm_tips;
							mymsg.h_srcwin = NULL;
							mymsg.id = MSG_OP_DOWN;
							GUI_SendMessage(&mymsg);
							
						}
						break;	
						case ID_OP_ENTER:
						{
							__gui_msg_t mymsg;
							mymsg.h_deswin = setting_ctrl->h_frm_tips;
							mymsg.h_srcwin = NULL;
							mymsg.id = MSG_OP_ENTER;
							GUI_SendMessage(&mymsg);
						}
						break;
						case ID_OP_EXIT:
						{
							__gui_msg_t mymsg;
							mymsg.h_deswin = setting_ctrl->h_frm_tips;
							mymsg.h_srcwin = NULL;
							mymsg.id = MSG_OP_EXIT;
							GUI_SendMessage(&mymsg);
						}
						break;
						case ID_SWITCH_TO_GENERAL:
						{
							H_LYR tips_win_layer;
							if(setting_ctrl==NULL)
							{
								__msg("setting_ctrl error.........\n");
								return EPDK_FAIL;
							}
							if( setting_ctrl->h_frm_tips == NULL )
							{
								__msg(" setting_ctrl->h_frm_tips is null \n");
								GUI_WinSetFocusChild(setting_ctrl->h_frm_general);
								return EPDK_FAIL ;
							}
							tips_win_layer = GUI_WinGetLyrWin(setting_ctrl->h_frm_tips);
							if(tips_win_layer == NULL )
							{
								__msg( " tips_win layer is null \n" );
								return EPDK_FAIL ;
							}
							GUI_LyrWinDelete(tips_win_layer);
							setting_ctrl->h_frm_tips = NULL ;
							GUI_WinSetFocusChild(setting_ctrl->h_frm_general);
						}
						break;
					default:
						break;
					}
				}
				return EPDK_OK;
				
				case SETTING_TIPS_ID:
				{	
					__here__;
					switch(HIWORD(msg->dwAddData1))
					{
						case ADLG_CMD_EXIT:
						{
							__here__ ;

							app_dialog_destroy( setting_ctrl->h_dialoag_win) ;
							setting_ctrl->h_dialoag_win = NULL ;

							if(ADLG_IDYES == msg->dwAddData2 )
							{
								__gui_msg_t mymsg;
								__msg("***********restore factory setting*******\n");
								mymsg.h_deswin = setting_ctrl->h_frm_general;
								mymsg.id = MSG_RESTORE_FACTORY;
								GUI_SendMessage(&mymsg);

							}
							return EPDK_OK ;
						}
						default:
						{
							break;
						}
					}
					break ;
				}
			default:
				break ; 
			}	
			return EPDK_OK;
		}
	default:
		break;
	}
	return GUI_ManWinDefaultProc(msg);	
}

H_WIN app_setting_create(root_para_t *para)

{
	__gui_manwincreate_para_t create_info;
	setting_ctrl_t *setting_ctrl;

	__inf("****************************************************************************************\n");
	__inf("********  enter setting app  **************\n");
	__inf("****************************************************************************************\n");

	//g_disable_standby();
	
	//__wait__;
	setting_ctrl = (setting_ctrl_t *)My_Balloc(sizeof(setting_ctrl_t));
	if(!setting_ctrl)
	{
		__msg("memory balloc fail.........\n");
		return NULL;
	}
	eLIBs_memset(setting_ctrl, 0, sizeof(setting_ctrl_t));

	setting_ctrl->setting_font = para->font;
	setting_ctrl->root_type = (setting_root_type_t)para->setting_opt;
	eLIBs_memset(&create_info, 0, sizeof(__gui_manwincreate_para_t));
	
	create_info.name            = APP_SETTING;
	create_info.hParent         = para->h_parent;
	create_info.ManWindowProc   = (__pGUI_WIN_CB)esKRNL_GetCallBack((__pCBK_t)_app_setting_Proc); 
	create_info.attr            = (void*)setting_ctrl;
	
	create_info.id			= APP_SETTING_ID;
	create_info.hHosting        = NULL;
	
	return(GUI_ManWinCreate(&create_info));
}

