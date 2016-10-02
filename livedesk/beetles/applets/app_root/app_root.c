/*
**************************************************************************************************************
*											         ePDK
*						            the Easy Portable/Player Develop Kits
*									           desktop system 
*
*						        	 (c) Copyright 2007-2010, ANDY, China
*											 All Rights Reserved
*
* File    		: app_root_win.c
* By      		: Bayden
* Func		: app_root applets plugin interface define
* Version	: v1.0
* ============================================================================================================
* 2010-9-28 17:11:20  andy.zhang  create this file, implements the fundemental interface;
**************************************************************************************************************
*/

#include "beetles_app.h"
#include "app_root.h"
#include "app_root_init.h"
#include "app_root_scene.h"
/**********************************************************************************************************************/


/**********************************************************************************************************************
插件接口注册
**********************************************************************************************************************/
/* 声明 */
static __s32 app_root_start(Activity  *thiz);
static __s32 app_root_suspend(Activity *thiz);
static __s32 app_root_resume(Activity *thiz);
static __s32 app_root_background(Activity *thiz);
static __s32 app_root_stop(Activity *thiz);


/* 注册插件接口 */
AppletPlugin APPLET_PLUGIN_INTERFACE =
{
	app_root_start,
	app_root_suspend,
	app_root_resume,
	app_root_background,
	app_root_stop,
};

Activity * root_activity = NULL;//应用程序框架插件指针
H_WIN happ_root_manwin = NULL;//应用程序框架manwin句柄


/**********************************************************************************************************************
插件接口实现 
**********************************************************************************************************************/
static __s32 app_root_start(Activity *activity)
{	
	__here__;
	//test_rat_partition(RAT_LOCAL_DISK);

	app_root_init_res();
	root_activity = activity;
	
	happ_root_manwin = app_root_wincreate(activity);
	if (NULL == happ_root_manwin)
	{
		__msg("	 app_root_wincreate fail! \n");
		return EPDK_FAIL;
	}
	else
	{		
		GUI_WinSetFocusChild(happ_root_manwin);
		__msg("	 app_root_wincreate success! \n");
		return EPDK_OK;
	}	
    
}

static __s32 app_root_suspend(Activity *activity)
{
	__here__;
	
	if(happ_root_manwin)
	{
		GUI_ManWinDelete(happ_root_manwin);
		happ_root_manwin = NULL;
		app_root_deinit_res();
	}

	return EPDK_OK;
}

static __s32 app_root_resume(Activity *activity)
{
	__here__;
	
	app_root_init_res();
	root_activity = activity;
	
	happ_root_manwin = app_root_wincreate(activity);
	if (NULL == happ_root_manwin)
	{
		__msg("	 app_root_wincreate fail! \n");
		return EPDK_FAIL;
	}
	else
	{		
		__msg("	 app_root_wincreate success! \n");
		return EPDK_OK;
	}	
	
}

static __s32 app_root_background(Activity *activity)
{
	__here__;

	if(happ_root_manwin)
	{
		GUI_ManWinDelete(happ_root_manwin);
		happ_root_manwin = NULL;
		app_root_deinit_res();
	}
	
	return EPDK_OK;
}

static __s32 app_root_stop(Activity *activity)
{	
	__here__;

	if(happ_root_manwin)
	{
		GUI_ManWinDelete(happ_root_manwin);
		happ_root_manwin = NULL;
		app_root_deinit_res();
	}
	
	return EPDK_OK;
}

