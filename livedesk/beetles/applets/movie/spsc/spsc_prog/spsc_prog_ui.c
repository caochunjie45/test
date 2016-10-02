
/*
**************************************************************************************************************
*											         ePDK
*						            the Easy Portable/Player Develop Kits
*									           desktop system 
*
*						        	 (c) Copyright 2007-2010, ANDY, China
*											 All Rights Reserved
*
* File    	: spsc_prog_ui.c
* By      	: Bayden.chen
* Func		: 
* Version	: v1.0
* ============================================================================================================
* 2011-05-05  Bayden.chen  create this file
**************************************************************************************************************
*/

#include "spsc_prog_ui.h"

static movie_spsc_prog_uipara_t uipara_400_240 = 
{	
	{
		10, 77+22, 354, 16
	},
	{
		{
			70, 75+28, 30, 15,
			ID_MOVIE_SPSC_PROG_PLAYSTA_PAUSE_BMP,
			ID_MOVIE_SPSC_PROG_PLAYSTA_PLAY_BMP,
		},
		{
			110, 80+28, 146, 4,
			ID_MOVIE_SPSC_PROG_BG_UF_BMP
		},
		{
			110, 80+28, 5, 4,
			ID_MOVIE_SPSC_PROG_CURSOR_UF_BMP
		}
	},
	{
		{		
			0+6, 75+26, 16, 16					
		},
		{		
			24+6, 75+26, 16, 16					
		},
		{		
			48+6, 75+26, 16, 16					
		},
	},
	{
		{		
			16+6, 75+26, 8, 16					
		},
		{		
			40+6, 75+26, 8, 16					
		}
	},
	{
		{		
			260+0+6, 75+26, 16, 16					
		},
		{		
			260+24+6, 75+26, 16, 16					
		},
		{		
			260+48+6, 75+26, 16, 16					
		},
	},
	{
		{		
			260+16+6, 75+26, 8, 16					
		},
		{		
			260+40+6, 75+26, 8, 16					
		}
	},	
};

movie_spsc_prog_uipara_t* movie_spsc_prog_get_uipara(__s32 rotate)
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
			
		}		
	case GUI_SCNDIR_ROTATE90:
	case GUI_SCNDIR_ROTATE270:
		
		break;
	}	
	
	return &uipara_400_240;		
}
