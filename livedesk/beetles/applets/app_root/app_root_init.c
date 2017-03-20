/*
**************************************************************************************************************
*											         ePDK
*						            the Easy Portable/Player Develop Kits
*									           desktop system 
*
*						        	 (c) Copyright 2007-2010, ANDY, China
*											 All Rights Reserved
*
* File    	: live_init.c
* By      	: Andy.zhang
* Func		: Live main res init 
* Version	: v1.0
* ============================================================================================================
* 2009-7-20 8:51:52  andy.zhang  create this file, implements the fundemental interface;
**************************************************************************************************************
*/

#include "beetles_app.h"

GUI_FONT    *SWFFont = NULL;

/**
 * ��ʼ�� ϵͳ��Դ
 */
__s32 app_root_init_res(void)
{
	rat_init();
	 	
	SWFFont = GUI_SFT_CreateFont(14, BEETLES_APP_ROOT"res\\fonts\\font14.sft");
	if(SWFFont == NULL)
	{
	    __err("create font fail...\n");
	    return EPDK_FAIL;
	}
	GUI_SetFont(SWFFont);

	return EPDK_OK; 
}

/**
 * �ͷ� ϵͳ��Դ
 */
__s32 app_root_deinit_res(void)
{	    
	rat_deinit();
	if (SWFFont)
	{		
		GUI_SetFont(GUI_GetDefaultFont());
		GUI_SFT_ReleaseFont(SWFFont);
		SWFFont = NULL;
	}

	return EPDK_OK;
}

