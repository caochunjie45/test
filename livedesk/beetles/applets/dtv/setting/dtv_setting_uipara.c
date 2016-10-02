/*
**********************************************************************************************************************
*                                                    ePDK
*                                    the Easy Portable/Player Develop Kits
*                                              LiveTouch Sub-System
*
*                                   (c) Copyright 2007-2009, Steven.ZGJ.China
*                                             All Rights Reserved
*
* Moudle  : 
* File    : 
*
* By      : hot.lee
* Version : v1.0
* Date    : 
**********************************************************************************************************************
*/

#ifndef __dtv_SETTING_PARA_H__
#define __dtv_SETTING_PARA_H__
#include "dtv_setting_uipara.h"




setting_uipara_t setting_480_272 = 
{
	{(480 - 120)/2,0,120,24},
	{(480 - 120)/2,24,120,20},
	10,
	20 * 10,
	120
};

setting_uipara_t setting_800_480 = 
{
	{(800 - 180)/2,0,180,44},
	{(800 - 180)/2,44,180,38},
	10,
	38 * 10,
	180
};

setting_uipara_t setting_720_576 = 
{
	{(720 - 120)/2,0,120,24},
	{(720 - 120)/2,24,120,20},
	10,
	20 * 10,
	120
};

setting_uipara_t setting_720_480 = 
{
	{(720 - 120)/2,0,120,24},
	{(720 - 120)/2,24,120,20},
	10,
	20 * 10,
	120
};

setting_uipara_t setting_1024_600 = 
{
	{(1024 - 180)/2,0,180,44},
	{(1024 - 180)/2,44,180,38},
	10,
	38 * 10,
	180
};

setting_uipara_t* setting_get_uipara(void)
{
	SIZE rect;

	dsk_display_get_size(&rect.width,&rect.height);

	if((rect.width == 800) && (rect.height == 480))
	{
		return &setting_800_480;
	}

	else if((rect.width == 480) && (rect.height == 272))
	{
		return &setting_480_272;
	}	
	
	else if((rect.width == 720) && (rect.height == 576))
	{
		return &setting_720_576;
	}	
	else if((rect.width == 720) && (rect.height == 480))
	{
		return &setting_720_480;
	}	
	else if((rect.width == 1024) && (rect.height == 600))
	{
		return &setting_1024_600;
	}	
	else
	{
		return &setting_800_480;
	}
}

#endif;


