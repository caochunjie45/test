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
* By      : Hot.lee
* Version : v1.0
* Date    : 
**********************************************************************************************************************
*/

#ifndef __PROGRAM_PREVIEW_PARA_H__
#define __PROGRAM_PREVIEW_PARA_H__


#include "program_preview_uipara.h"




program_preview_uipara_t program_pre_480_272 = 
{
	{0,0,300,32},	               //title
	
	{330,32,150,20*10},             //listbar
	
	{
		{0,272-36,480,18},         //cur_next_prog[0]
		{0,272-18,480,18},         //cur_next_prog[1]
	},
	10,                             // LISTBAR_ITEM_NUM
	20,
	ID_DTV_LIST_TOP_480_BMP,
	ID_DTV_PROGRAM_PRELIST_F_BMP,
    ID_DTV_PROGRAM_PRELIST_UF_BMP,
};


program_preview_uipara_t program_pre_800_480 = 
{
	{0,0,300,44},	               //title

	{580,44,220,38*10},            //listbar
	{
		{0,392+44,800,22}, //cur_next_prog[0]
		{0,414+44,800,22},//cur_next_prog[1]
	},
	10,							   // LISTBAR_ITEM_NUM
	38,
	ID_DTV_LIST_TOP_800_BMP,
	ID_DTV_PROGRAM_PRELIST_F_BMP,
    ID_DTV_PROGRAM_PRELIST_UF_BMP,

};

program_preview_uipara_t program_pre_1024_600 = 
{
	{0,0,300,44},	               //title

	{804,44,220,38*10},            //listbar
	
	{
		{0,600-22-22,1024,22},        //cur_next_prog[0]
		{0,600-22,1024,22},        //cur_next_prog[1]
	},
	10,							   // LISTBAR_ITEM_NUM
	38,
	ID_DTV_LIST_TOP_1024_BMP,
    ID_DTV_PROGRAM_PRELIST_F_BMP,
    ID_DTV_PROGRAM_PRELIST_UF_BMP,
	
};


static program_preview_uipara_t program_pre_720_576 =
{
	{0,0,300,32},	               //title
	
	{570,32,150,20*10},			   //listbar
	
	{
		{0,576-36,720,18}, //cur_next_prog[0]
		{0,576-18,720,18},//cur_next_prog[1]
	},
	10,  						   // LISTBAR_ITEM_NUM
    20,
    ID_DTV_LIST_TOP_720_BMP,
    ID_DTV_PROGRAM_PRELIST_F_BMP,
    ID_DTV_PROGRAM_PRELIST_UF_BMP,
    
};


static program_preview_uipara_t program_pre_720_480 =
{
	{0,0,300,32},	               //title
	
	{570,32,150,20*10},			   //listbar
	
	{
		{0,480-36,720,18}, //cur_next_prog[0]
		{0,480-18,720,18},//cur_next_prog[1]
	},
	10,  						   // LISTBAR_ITEM_NUM
    20,
    ID_DTV_LIST_TOP_720_BMP,
    ID_DTV_PROGRAM_PRELIST_F_BMP,
    ID_DTV_PROGRAM_PRELIST_UF_BMP,
   
};



program_preview_uipara_t* program_preview_get_uipara(void)
{
	SIZE rect;

	dsk_display_get_size(&rect.width,&rect.height);

	if((rect.width == 480) && (rect.height == 272))
	{
		return &program_pre_480_272;
	}

	else if((rect.width == 800) && (rect.height == 480))
	{
		return &program_pre_800_480;
	}

	else if((rect.width == 720) && (rect.height == 576))
	{
		return &program_pre_720_576;
	}

	else if((rect.width == 720) && (rect.height == 480))
	{
		return &program_pre_720_480;
	}

	else if((rect.width == 1024) && (rect.height == 600))
	{
		return &program_pre_1024_600;
	}
	
	else
	{
		return &program_pre_800_480;
	}
}

#endif;


