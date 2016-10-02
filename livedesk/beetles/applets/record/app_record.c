/*
**************************************************************************************************************
*											         ePDK
*						            the Easy Portable/Player Develop Kits
*									           desktop system
*
*
* File    	: record.c
* By      	: Kingvan
* Func		: record main thread
* Version	: v1.0
* ============================================================================================================
* 2009-7-20 8:51:52  Kingvan  create this file, implements the fundemental interface;
**************************************************************************************************************
*/
#include "app_record.h"


#define CONTROL_TEST	0

#if CONTROL_TEST
#include "control.h"
H_WIN g_controlWin = NULL;
#endif

/*************************************************************************************************************
拉高PC8脚，打开麦克风
*************************************************************************************************************/
#if 0
void set_mic_on(void)   //cai++
{
	__s32	ret;
	user_gpio_set_t	gpio_set[1];
	__u32 power_on = 0;
	
	eLIBs_memset(gpio_set, 0, sizeof(user_gpio_set_t));	
		ret = esCFG_GetKeyValue("audio_para", "power_on", (int *)gpio_set, sizeof(user_gpio_set_t)/4);
__msg(".....................静音...........................\n");
		if(!ret)
		{
			power_on = esPINS_PinGrpReq(gpio_set,1);
			if(!power_on)
			{
				__msg("request pa_shdn io failed \n");
				return;
			}
			esPINS_WritePinData(power_on,1,NULL);//拉高时不工作，静音
			__msg("mute .......静音........\n");
		}
		else
		{
			__msg("read from sys_config file failed !\n");
			return;
		}
	return;
}
#endif		


static void app_record_path( precord_ctrl_t precord_ctrl )
{
	char root_type[50];
	char disk_name[RAT_MAX_PARTITION][4];

	eLIBs_memset(root_type, 0, sizeof(root_type));
	eLIBs_memset(disk_name, 0, sizeof(disk_name));

	switch(precord_ctrl->root_type)
	{
		case RAT_LOCAL:
			eLIBs_strcpy(root_type,RAT_LOCAL_DISK);
			break;
		case RAT_TF:
			eLIBs_strcpy(root_type,RAT_SD_CARD);
			break;
		case RAT_USB:
			eLIBs_strcpy(root_type,RAT_USB_DISK);
			break;
		default:
			break;
	}
	if( rat_get_partition_name(root_type, disk_name) == EPDK_OK )
	{
		precord_ctrl->root_type_ch = disk_name[0][0];
		if( precord_ctrl->root_type_ch > 'a' && precord_ctrl->root_type_ch < 'z')
			precord_ctrl->root_type_ch -= 32;
	}
	else
		precord_ctrl->root_type_ch = '\0';
}

static void app_record_sendMsg(H_WIN hWin, __u32 id)
{
	__gui_msg_t sendMsg;

	sendMsg.h_deswin = hWin;
	sendMsg.h_srcwin = NULL;
	sendMsg.id = id;
	sendMsg.dwAddData1 = 0;
	sendMsg.dwAddData2 = 0;
	sendMsg.dwReserved = 0;

	GUI_PostMessage(&sendMsg);
}
void  app_record_cmd2parent(H_WIN hwin, __s32 id, __s32 data1, __s32 data2)
{
	__gui_msg_t msg;

	msg.h_deswin = GUI_WinGetParent(hwin);
	__msg("msg.h_deswin=%x\n", msg.h_deswin);
	msg.h_srcwin = NULL;
	msg.id = GUI_MSG_COMMAND;
	msg.dwAddData1 = MAKELONG(GUI_WinGetItemId(hwin), id);
	__msg("msg.dwAddData1=%x\n", msg.dwAddData1);
	msg.dwAddData2 = data1;
	msg.dwReserved = data2;

	GUI_SendNotifyMessage(&msg);
}
static H_LYR record_8bpp_layer_create(void)
{
	H_LYR layer = NULL;
	RECT LayerRect = {0,0,400,242};

	FB  fb =
	{
	    {0, 0},                                   					/* size      */
	    {0, 0, 0},                                      			/* buffer    */
	    {FB_TYPE_RGB, {PIXEL_MONO_8BPP, 0, (__rgb_seq_t)0}},   /* fmt       */

	};
	__disp_layer_para_t lstlyr =
	{
		DISP_LAYER_WORK_MODE_PALETTE,
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
        "app_record",
      	NULL,
      	GUI_LYRWIN_STA_SUSPEND,
      	GUI_LYRWIN_NORMAL
    };

    fb.size.width			= LayerRect.width;
    fb.size.height			= LayerRect.height;

	lstlyr.src_win.x  		= LayerRect.x;
	lstlyr.src_win.y  		= LayerRect.y;
	lstlyr.src_win.width 	= LayerRect.width;
	lstlyr.src_win.height 	= LayerRect.height;

	lstlyr.scn_win.x		= LayerRect.x+40;
	lstlyr.scn_win.y		= LayerRect.y+15;
	lstlyr.scn_win.width  	= LayerRect.width;
	lstlyr.scn_win.height 	= LayerRect.height;

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

static __s32 app_record_on_create(__gui_msg_t *msg)
{
	precord_ctrl_t precord_para;

	precord_para = (precord_ctrl_t)GUI_WinGetAttr(msg->h_deswin);


    init_record_res( &precord_para->rec_ui );

	precord_para->rec_view.pui 		= &precord_para->rec_ui;
	precord_para->rec_listView.pui 	= &precord_para->rec_ui;
	precord_para->layer = record_8bpp_layer_create();

	gscene_bgd_set_state(BGD_STATUS_SHOW);

	precord_para->curOpenFrmType = RECORD_NONE;
	precord_para->frm = rec_view_frm_create(msg->h_deswin, precord_para);
	if( precord_para->frm == NULL )
	{
		__inf("app_record_on_create() return to home!\n");
		GUI_LyrWinSetSta(precord_para->layer, GUI_LYRWIN_STA_SUSPEND);
		app_record_cmd2parent(msg->h_deswin, SWITCH_TO_MMENU, 0, 0);
	}
	else
	{
		GUI_WinSetFocusChild(precord_para->frm);
	}

	return EPDK_OK;
}
static __s32 app_record_on_close(__gui_msg_t *msg)
{
	GUI_ManWinDelete(msg->h_deswin);

	return EPDK_OK;
}
static __s32 app_record_on_destory(__gui_msg_t *msg)
{
	precord_ctrl_t precord_para;

	precord_para = (precord_ctrl_t)GUI_WinGetAttr(msg->h_deswin);

	uninit_record_res( &precord_para->rec_ui );

	GUI_LyrWinDelete(precord_para->layer);

#if CONTROL_TEST
	g_controlWin = NULL;
#endif

	precord_para->layer = 0;

	esMEMS_Bfree(precord_para, sizeof(record_ctrl_t));

    rat_set_modify_flag(RAT_MEDIA_TYPE_AUDIO, 1);

    //g_enable_standby();

	return EPDK_OK;
}

static __s32 app_record_on_key(__gui_msg_t *msg)
{
#if CONTROL_TEST
	precord_ctrl_t precord_para;
	control_para para;
	precord_para = (precord_ctrl_t)GUI_WinGetAttr(msg->h_deswin);
	if (KEY_UP_ACTION != msg->dwAddData2)
	{
		switch( msg->dwAddData1 )
		{
		case GUI_MSG_KEY_LEFT:
		{
			if( g_controlWin != NULL )
				break;

			para.controlType = CONTROL_VOLUME;
			para.controlID = APP_RECORD_ID+3;
			para.parentWin = msg->h_deswin;
			g_controlWin = aux_control_frm_create( &para );
			if( g_controlWin == NULL )
			{
				__inf("control_frm_create() return to home!\n");
				GUI_LyrWinSetSta(precord_para->layer, GUI_LYRWIN_STA_SUSPEND);
				app_record_cmd2parent(msg->h_deswin, SWITCH_TO_MMENU, 0, 0);
			}
			else
			{
				__inf("control_frm_create() OK!\n");
			}
		}
			break;
		}
	}
#endif

	return EPDK_OK;
}
static __s32 app_record_on_command(__gui_msg_t *msg)
{
	return EPDK_OK;
}

static __s32 _record_manager_win_proc(__gui_msg_t *msg)
{
	//__msg("_record_manager_win_proc msg->id=%d, msg->h_deswin=%x, msg->dwAddData1=%d, msg->dwAddData2=%d\n" ,msg->id, msg->h_deswin, msg->dwAddData1, msg->dwAddData2);
	switch(msg->id)
	{
		case GUI_MSG_CREATE:
		{
			set_record_is_play(EPDK_FALSE);

			if(dsk_get_fsm_statu() != 2)
				dsk_set_fsm_statu(2);

			return app_record_on_create(msg);
		}
		case GUI_MSG_CLOSE:
			set_record_is_play(EPDK_TRUE);
			__msg("----record manager window GUI_MSG_CLOSE begin----\n");
			return app_record_on_close(msg);
		case GUI_MSG_DESTROY:
			__msg("----record manager window GUI_MSG_DESTROY begin----\n");
			set_record_is_play(EPDK_TRUE);
			return app_record_on_destory(msg);
		case GUI_MSG_KEY:
			app_record_on_key(msg);
			break;//注意!!!
		case GUI_MSG_TOUCH:
			break;
		case GUI_MSG_COMMAND:
			return app_record_on_command(msg);
		case GUI_MSG_NOTIFY_CHILD_DELETED:
		{
			precord_ctrl_t precord_para;
			precord_para = (precord_ctrl_t)GUI_WinGetAttr(msg->h_deswin);

			if( precord_para->curOpenFrmType == RECORD_FRM )
			{
				precord_para->curOpenFrmType = RECORD_NONE;
				if( precord_para->bIsFMRecord )
					dsk_radio_unmute();
				precord_para->frm = rec_view_frm_create(msg->h_deswin, precord_para);
				if( precord_para->frm == NULL )
				{
					GUI_LyrWinSetSta(precord_para->layer, GUI_LYRWIN_STA_SUSPEND);
					app_record_cmd2parent(msg->h_deswin, SWITCH_TO_MMENU, 0, 0);
				}
				else
				{
					GUI_WinSetFocusChild(precord_para->frm);
				}
			}
			else if( precord_para->curOpenFrmType == RECORD_LIST_FRM )
			{
				precord_para->curOpenFrmType = RECORD_NONE;
				if( precord_para->bIsFMRecord )
					dsk_radio_mute();
				precord_para->frm = rec_list_view_frm_create(msg->h_deswin, precord_para);
				if( precord_para->frm == NULL )
				{
					GUI_LyrWinSetSta(precord_para->layer, GUI_LYRWIN_STA_SUSPEND);
					app_record_cmd2parent(msg->h_deswin, SWITCH_TO_MMENU, 0, 0);
				}
				else
				{
					GUI_WinSetFocusChild(precord_para->frm);
				}
			}
			else if( precord_para->curOpenFrmType == MAIN_HOME )
			{
				precord_para->curOpenFrmType = RECORD_NONE;
				GUI_LyrWinSetSta(precord_para->layer, GUI_LYRWIN_STA_SUSPEND);
				app_record_cmd2parent(msg->h_deswin, SWITCH_TO_MMENU, 0, 0);
			}
			else if( precord_para->curOpenFrmType == TO_FM )
			{
				precord_para->curOpenFrmType = RECORD_NONE;
				if( precord_para->bIsFMRecord )
					dsk_radio_unmute();
				GUI_LyrWinSetSta(precord_para->layer, GUI_LYRWIN_STA_SUSPEND);
				app_record_cmd2parent(msg->h_deswin, SWITCH_TO_OTHER_APP, MAKELONG(RECORD_SW_TO_FM, precord_para->root_type), 0);
			}
		}
			break;
		case RECORD_TO_LIST_VIEW_FRAME:
		{
			precord_ctrl_t precord_para;
			precord_para = (precord_ctrl_t)GUI_WinGetAttr(msg->h_deswin);

			precord_para->curOpenFrmType = RECORD_LIST_FRM;
			app_record_sendMsg(precord_para->frm, GUI_MSG_CLOSE );
		}
			return EPDK_OK;
		case RECORD_TO_VIEW_FRAME:
		{
			precord_ctrl_t precord_para;
			precord_para = (precord_ctrl_t)GUI_WinGetAttr(msg->h_deswin);

			precord_para->curOpenFrmType = RECORD_FRM;
			app_record_sendMsg(precord_para->frm, GUI_MSG_CLOSE );
		}
			return EPDK_OK;
		case RECORD_TO_HOME:
		{
			precord_ctrl_t precord_para;
			precord_para = (precord_ctrl_t)GUI_WinGetAttr(msg->h_deswin);

			if( precord_para->bIsFMRecord )
				precord_para->curOpenFrmType = TO_FM;
			else
				precord_para->curOpenFrmType = MAIN_HOME;
			app_record_sendMsg(precord_para->frm, GUI_MSG_CLOSE );
		}
			return EPDK_OK;
	}
	return GUI_ManWinDefaultProc(msg);
}

__s32 app_record_create(root_para_t  *para)
{
	__gui_manwincreate_para_t 	create_info;
	precord_ctrl_t				precord_ctrl = NULL;
	__inf("****************************************************************************************\n");
	__inf("********  enter record application  root_type %d**************\n", para->root_type);
	__inf("****************************************************************************************\n");

	//g_disable_standby();
	
	precord_ctrl = (precord_ctrl_t)esMEMS_Balloc(sizeof(record_ctrl_t));
	if( precord_ctrl == NULL )
	{
		__msg("esMEMS_Balloc fail\n");
		return NULL;
	}
	eLIBs_memset(precord_ctrl, 0, sizeof(record_ctrl_t));
	eLIBs_memset(&create_info, 0, sizeof(__gui_manwincreate_para_t));

	precord_ctrl->pfont = para->font;
	precord_ctrl->bIsFMRecord = (para->record_opt == 1);
	precord_ctrl->root_type = para->root_type;
	app_record_path(precord_ctrl);

	create_info.name            = APP_RECORD;
	create_info.hParent         = para->h_parent;
	create_info.ManWindowProc   = (__pGUI_WIN_CB)esKRNL_GetCallBack((__pCBK_t)_record_manager_win_proc);
	create_info.attr            = (void*)precord_ctrl;
	create_info.id				= APP_RECORD_ID;
	create_info.hHosting        = NULL;

	return(GUI_ManWinCreate(&create_info));
}





