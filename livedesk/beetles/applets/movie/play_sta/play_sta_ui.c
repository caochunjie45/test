
/*
**************************************************************************************************************
*											         ePDK
*						            the Easy Portable/Player Develop Kits
*									           desktop system 
*
*						        	 (c) Copyright 2007-2010, ANDY, China
*											 All Rights Reserved
*
* File    	: playsta_ui.c
* By      	: Bayden.chen
* Func		: 
* Version	: v1.0
* ============================================================================================================
* 2011-05-05  Bayden.chen  create this file
**************************************************************************************************************
*/

#include "play_sta_ui.h"

static movie_playsta_uipara_t uipara_400_240 = 
{
	1,
	{
		26, 22, 18, 18		
	},
	{
		0, 0, 18, 18		
	},
	{
		0, 0, 18, 18,		        
		ID_MOVIE_PAUSESTA_BMP,//ID_MOVIE_PLAYSTA_BMP,  //CAI MODIFY  FOR  movie tu pian 
		ID_MOVIE_PLAYSTA_BMP,//ID_MOVIE_PAUSESTA_BMP
	}
};

static movie_playsta_uipara_t uipara_720_576 = 
{
	1,
	{
		40, 60, 18, 18		
	},
	{
		0, 0, 18, 18		
	},
	{
		0, 0, 18, 18,		        
		ID_MOVIE_PLAYSTA_BMP,
		ID_MOVIE_PAUSESTA_BMP
	}
};

static movie_playsta_uipara_t uipara_720_480 = 
{
	1,
	{
		40, 60, 18, 18		
	},
	{
		0, 0, 18, 18		
	},
	{
		0, 0, 18, 18,		        
		ID_MOVIE_PLAYSTA_BMP,
		ID_MOVIE_PAUSESTA_BMP
	}
};


movie_playsta_uipara_t* movie_playsta_get_uipara(__s32 rotate)
{
	__s32 			screen_width;
	__s32 			screen_height;
	
	/* get lcd size*/
	dsk_display_get_size(&screen_width, &screen_height);
	switch(rotate)
	{
	case GUI_SCNDIR_NORMAL:
	case GUI_SCNDIR_ROTATE180:
		{
			if((screen_width == 400 )&&( screen_height == 240))
				return &uipara_400_240;	
			else if((screen_width == 720 )&&( screen_height == 576))
				return &uipara_720_576;	
			else if((screen_width == 720 )&&( screen_height == 480))
				return &uipara_720_480;			
			
		}		
	case GUI_SCNDIR_ROTATE90:
	case GUI_SCNDIR_ROTATE270:
		
		break;
	}	
	
	return &uipara_400_240;		
}
