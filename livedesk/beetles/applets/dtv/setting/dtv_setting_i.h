/*
**************************************************************************************************************
*											         ePDK
*						            the Easy Portable/Player Develop Kits
*									           desktop system 
*
*						        	 (c) Copyright 2007-2010, ANDY, China
*											 All Rights Reserved
*
* File    	: dtv_setting_i.h
* By      	: hot.lee
* Func		: desk main thread
* Version	: v1.0
* ============================================================================================================
* 2009-7-20 8:51:52  hot.lee  create this file, implements the fundemental interface;
**************************************************************************************************************
*/
#ifndef __APP_dtv_SETTINGS_I_H_
#define  __APP_dtv_SETTINGS_I_H_

#include "beetles_app.h"
#include "..//dtv_i.h"


#define dtv_SETTING_FOCUS_COLOR   GUI_YELLOW

#define dtv_SETTING_UNFOCUS_COLOR  GUI_BLACK

enum
{
	DTV_SETTING_PREVIEW = 0x0,
	DTV_SETTING_SEARCH,
	DTV_SETTING_DISPLAY,
	DTV_SETTING_TRACK,
	DTV_SETTING_LCN,
	DTV_SETTING_EPG,
	DTV_SETTING_ANTENNA,
	DTV_SETTING_ANTENNA_POWER,
	DTV_SETTING_SSU_UPDATE,

	DTV_SETTING_BACK2LIST,//从子项返回list
	DTV_SETTING_REPLAY,

};


#endif



