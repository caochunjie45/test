/*
**************************************************************************************************************
*											         ePDK
*						            the Easy Portable/Player Develop Kits
*									           desktop system 
*
*						        	 (c) Copyright 2007-2010, ANDY, China
*											 All Rights Reserved
*
* File    		: init.c
* By      		: Andy.zhang
* Func		: init thread
* Version	: v1.0
* ============================================================================================================
* 2009-7-20 8:51:52  andy.zhang  create this file, implements the fundemental interface;
**************************************************************************************************************
*/

#include "mod_init_i.h"
#include "ui\\init_server.h"

#define XML_PATH	BEETLES_APP_ROOT"apps\\desktop"

static H_WIN init_mainwin = NULL;

static void _close_audio_dev(void)
{
	ES_FILE		 * p_dac;
				
	p_dac = eLIBs_fopen("b:\\AUDIO\\CTRL", "r+");
    if(!p_dac)
    {        	
    	__err("open dac device fail!\n");
    }
            
	eLIBs_fioctrl(p_dac, AUDIO_DEV_CMD_CLOSE_DEV, AUDIO_DEV_PLAY, 0);
	eLIBs_fclose(p_dac);
}

/*
static void _close_lcd(void)
{
	ES_FILE *disp;
	__u32 outputtype;
	__u32 	arg[3];	
	
	disp = eLIBs_fopen("b:\\DISP\\DISPLAY", "r+");		
	if(!disp)
	{
		__wrn("open display fail\n");
		return ;
	}
	
	outputtype = eLIBs_fioctrl(disp,DISP_CMD_GET_OUTPUT_TYPE,0,(void *)arg);
	if(outputtype == DISP_OUTPUT_TYPE_LCD)
	{
		arg[0] = 0;
		arg[1] = 0;
		arg[2] = 0;
		eLIBs_fioctrl(disp,DISP_CMD_LCD_OFF,0,(void *)arg);
	}

	eLIBs_fclose(disp);
}
*/

static void _framework_init(void)
{
	AppletInfoManager	*info_manager = NULL;
	ActivityManager 	*activity_manager = NULL;

	info_manager = applet_info_manager_create();		
	applet_info_manager_load_dir(info_manager, XML_PATH);		
	//applet_info_manager_printf(info_manager);
	applet_info_manager_set_defualt(info_manager);	
	__here__;
	activity_manager = activity_manager_create();	
	activity_manager_set_default(activity_manager);

	return;
}

static void _framework_exit(void)
{
	AppletInfoManager 	*info_manager = applet_info_manager_get_default();
	ActivityManager 	*activity_manager = activity_manager_get_default(); 
	
	activity_manager_destroy(activity_manager);
	applet_info_manager_destroy(info_manager);
	applet_info_manager_set_defualt(NULL);
	activity_manager_set_default(NULL);

	return;
}

__s32 key_hook_cb(__gui_msg_t *msg)
{
	__init_ctl_t * init_ctr = (__init_ctl_t *)GUI_WinGetAttr(init_mainwin);
	
	init_lock(msg);	
	if( !init_ctr->key_msg_enble )
	{		
		init_unlock(msg);
		return -1;
	}
		
	if( init_ctr->screen_close_done )
	{		
		init_unlock(msg);
		if(KEY_UP_ACTION == msg->dwAddData2)		
		{			
			NOTIFY_MSG(DSK_MSG_SCREEN_OPEN, NULL, msg->h_deswin, 0, 0);			
		}
		__here__;		
		return -1;        
	}
		
	init_unlock(msg);
	esPWM_UsrEventNotify();
	    
	GUI_PostMessage(msg);

	return 0;
}

__s32 init_hook_cb(__gui_msg_t *msg)
{		
	__init_ctl_t * init_ctr = (__init_ctl_t *)GUI_WinGetAttr(init_mainwin);
	
	/* 请求开屏 */
	if( init_ctr->screen_close_done )
	{
		NOTIFY_MSG(DSK_MSG_SCREEN_OPEN, NULL, msg->h_deswin, 0, 0);
	}		

	//__msg("----init_hook_cb =%x =%d =%d =%d\n",msg->h_deswin, msg->id, msg->dwAddData1, msg->dwAddData2);   
    
	GUI_PostMessage(msg);		

	return 0;
}

static void _process_init(void)
{	
	__memit_hook_t 	emit_hook;		
	
	/* framework init */
	_framework_init();
    
	/* load main*/		
	activity_load_app("application://app_root");	

	/* messge server init*/		
	msg_emit_init();	

	emit_hook.key_hook	= key_hook_cb;
	emit_hook.init_hook 	= init_hook_cb;
	msg_emit_register_hook(&emit_hook);

	return;
}

static void _process_exit(void)
{    
	msg_emit_deinit();
	_framework_exit();
	_close_audio_dev();
	//_close_lcd();

	esDEV_Plugout("\\drv\\audio.drv", 0 );	
	esDEV_Plugout("\\drv\\usb_host1.drv", 1);

	return;
}

void application_init_process(void *arg)
{			
	__gui_msg_t 	msg;	
	__s32			ret = EPDK_FAIL;
	//power_level_e 	level;
	
	__inf("&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&\n");
	__inf("&           application_init_process enter!          		       &\n");	
	__inf("&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&\n");
    

	esDEV_Plugin("\\drv\\audio.drv", 0, 0, 1);
	

	init_mainwin = init_mainwin_create();	
	GUI_SetActiveManWin(init_mainwin);	
	message_loop_win_set_default(init_mainwin);
	
	__msg("init_mainwin = %x\n", init_mainwin);	
	//esKSRV_SysInfo();
	
	//load app root
	_process_init();	

	esDEV_Plugin("\\drv\\usb_host1.drv", 1, NULL, 0);
	esKRNL_TimeDly(1);
	
	/*enable device monitor */
	{
	    ES_FILE     *pHwsc;

	    pHwsc = eLIBs_fopen("b:\\HWSC\\hwsc", "rb+");
	    if(pHwsc)
	    {
	        eLIBs_fioctrl(pHwsc, DEV_IOC_USR_HWSC_ENABLE_MONITOR, 0, NULL);
	        eLIBs_fclose(pHwsc);
	        __inf(".start device monitor .......................................\n");
	    }
	    else
	    {
	        __err("try to open b:\\HWSC\\hwsc failed!\n");
	    }
	}

	__here__;

	while( GUI_GetMessageEx(&msg, init_mainwin) )
	{
		ret = GUI_DispatchMessage(&msg);
		if( msg.p_arg )
		{             	
			GUI_SetSyncMsgRetVal(&msg, ret);
			GUI_PostSyncSem(&msg);      
		}
	}		
	GUI_WinThreadCleanup(init_mainwin);		
	message_loop_win_set_default(NULL);
	
	_process_exit();
    
	
	__inf("&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&\n");
	__inf("&           application_init_process exit!                          &\n");
	__inf("&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&\n");

#if (AW_SDK_PMU == 0)
	///////////////
	while(1)
	{
		__here__;
		if(EPDK_TRUE == dsk_get_charge_state())
	    	{
			__here__;
			esKSRV_Reset();

		}
		esKRNL_TimeDly(60);
	}
	//////////////////
#endif

	/* 关闭电源 */
	esKSRV_PowerOff();
	
	return;
}


