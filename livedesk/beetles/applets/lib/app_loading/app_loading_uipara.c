/*
*********************************************************************************************************
*											        ePDK
*						            the Easy Portable/Player Develop Kits
*									           willow app sample
*
*						        (c) Copyright 2006-2007, SoftWinners Microelectronic Co., Ltd.
*											All	Rights Reserved
*
* File    :  ebook_uipara.c
* By      : john.fu
* Version : V1.00
*********************************************************************************************************
*/

#include "app_loading_uipara.h"

app_loading_uipara_t loading_480_272 = 
{
	{(480-30)/2,(272-30)/2 - 10,30,30},
	{(480-200)/2,(272-30)/2 + 52,200,52},	
};

app_loading_uipara_t loading_800_480 = 
{
	{(800-30)/2,(480-30)/2 - 10,30,30},
	{(800-150)/2,(480-30)/2 + 52,150,52},	
};

app_loading_uipara_t loading_1024_600 = 
{
	{(1024-30)/2,(600-30)/2 - 10,30,30},
	{(1024-150)/2,(600-30)/2 + 52,150,52},	
};

app_loading_uipara_t* get_app_loading_uipara(void)
{
	SIZE rect;

	dsk_display_get_size(&rect.width,&rect.height);

	if((rect.width == 800) && (rect.height == 480))
	{
		return &loading_800_480;
	}
	else if((rect.width == 480) && (rect.height == 272))
	{
		return &loading_480_272;
	}	
	
	else if((rect.width == 1024) && (rect.height == 600))
	{
		return &loading_1024_600;
	}	
	else
	{
		return &loading_800_480;
	}
}


