/*
**********************************************************************************************************************
*                                                    ePDK
*                                    the Easy Portable/Player Develop Kits
*                                              LiveTouch Sub-System
*
*                                   (c) Copyright 2007-2009, Steven.ZGJ.China
*                                             All Rights Reserved
*
* Moudle  : ebook
* File    : ebook_uipara.c
*
* By      : Bayden.chen
* Version : v1.0
* Date    : 2009-12-25 14:53:05
**********************************************************************************************************************
*/

#ifndef __SCHEDULE_PARA_H__
#define __SCHEDULE_PARA_H__
#include "schedule_uipara.h"



schedule_uipara_t schedule_pre_480_272 = 
{
	{(480-240)/2,(272-26-120)/2,240,26},
	{(480-240)/2,(272-26-120)/2 + 26,240,20},
	{0,0,0,0},
	{0,0,0,0},
	10,
	90,
	100,
	130,
	130,
	6,
	20*6,
	240,
};

schedule_uipara_t schedule_pre_800_480 = 
{
	{(800-480-188)/2,(480-44-228)/2,480,44},
	{(800-480-188)/2,(480-44-228)/2 + 44,480,38},
	{(800-480-188)/2 + 481,(480-44-228)/2 + 44,188,228},
	{(800-480-188)/2 + 491,(480-44-228)/2 + 44 + 30,188 - 10,228-30},
	10,
	160,
	190,
	250,
	250,
	6,
	38*6,
	480,
};

schedule_uipara_t schedule_pre_720_576 = 
{
	{(720-240)/2,(576-26-120)/2,240,26},
	{(720-240)/2,(576-26-120)/2 + 26,240,20},
	{0,0,0,0},
	{0,0,0,0},
	10,
	90,
	100,
	130,
	130,
	6,
	20*6,
	240,
};

schedule_uipara_t schedule_pre_720_480 = 
{
	{(720-240)/2,(480-26-120)/2,240,26},
	{(720-240)/2,(480-26-120)/2 + 26,240,20},
	{0,0,0,0},
	{0,0,0,0},
	10,
	90,
	100,
	130,
	130,
	6,
	20*6,
	240,
};

schedule_uipara_t schedule_pre_1024_600 = 
{
	{(1024-480-188)/2,(600-44-228)/2,480,44},
	{(1024-480-188)/2,(600-44-228)/2 + 44,480,38},
	{(1024-480-188)/2 + 481,(600-44-228)/2 +44,188,228},
	{(1024-480-188)/2 + 491,(600-44-228)/2 +44 + 30,188 - 10,228-30},
	10,
	160,
	190,
	250,
	250,
	6,
	38*6,
	480,
};


schedule_uipara_t* schedule_get_uipara(void)
{
	SIZE rect;

	dsk_display_get_size(&rect.width,&rect.height);

	if((rect.width == 800) && (rect.height == 480))
	{
		return &schedule_pre_800_480;
	}
	else if((rect.width == 480) && (rect.height == 272))
	{
		return &schedule_pre_480_272;
	}			
	else if((rect.width == 720) && (rect.height == 576))
	{
		return &schedule_pre_720_576;
	}	
	else if((rect.width == 720) && (rect.height == 480))
	{
		return &schedule_pre_720_480;
	}	
	else if((rect.width == 1024) && (rect.height == 600))
	{
		return &schedule_pre_1024_600;
	}	
	else
	{
		return &schedule_pre_800_480;
	}
}

#endif


