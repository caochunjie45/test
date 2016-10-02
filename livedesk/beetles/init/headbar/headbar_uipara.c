/*
**************************************************************************************************************
*											         ePDK
*						            the Easy Portable/Player Develop Kits
*									           desktop system 
*
*						        	 (c) Copyright 2007-2010, ANDY, China
*											 All Rights Reserved
*
* File    	: headbar_com.h
* By      	: Andy.zhang
* Func		: headbar manage
* Version	: v1.0
* ============================================================================================================
* 2009-7-20 8:51:52  andy.zhang  create this file, implements the fundemental interface;
**************************************************************************************************************
*/
#include "mod_init_i.h"
#include "headbar_uipara.h"

__headbar_uipara_t  headbar_uipara;

void headbar_init_uipara(GUI_FONT *font)
{
	int width;
	int height;	
	int screen_type;
	
	//dsk_display_get_size(&width, &height);
	width = 480;
	height = 272;
	
	if( (width == 480)&&(height == 272) )
		screen_type = SCREEN_TYPE_480_272;
	else if( ( width == 800 )&&( height == 480 ) )
		screen_type = SCREEN_TYPE_800_480;	
	else
		screen_type = SCREEN_TYPE_LAST;	
	
	switch( screen_type )
	{
        case SCREEN_TYPE_480_272:
		{
			//layer
			headbar_uipara.hbar_lyr.fb_w  = 480;
			headbar_uipara.hbar_lyr.fb_h  = 24;
			
			headbar_uipara.hbar_lyr.src_x = 0;
			headbar_uipara.hbar_lyr.src_y = 0;
			headbar_uipara.hbar_lyr.src_w = 480;
			headbar_uipara.hbar_lyr.src_h = 24;
			
			headbar_uipara.hbar_lyr.scn_x = 0;
			headbar_uipara.hbar_lyr.scn_y = 0;	
			headbar_uipara.hbar_lyr.scn_w = 480;
			headbar_uipara.hbar_lyr.scn_h = 24;
			
			headbar_uipara.hbar_lyr.pipe  = 1; 
			//
			headbar_uipara.hbar_frmwin.x = 0;		
			headbar_uipara.hbar_frmwin.y = 0;
			headbar_uipara.hbar_frmwin.w = 480;
			headbar_uipara.hbar_frmwin.h = 24;				
									
			headbar_uipara.strtf.x	= 10;
			headbar_uipara.strtf.y	= 2;
			headbar_uipara.strtf.w	= 20;
			headbar_uipara.strtf.h	= 20;

			headbar_uipara.strusb.x	= 40;
			headbar_uipara.strusb.y	= 2;
			headbar_uipara.strusb.w	= 30;
			headbar_uipara.strusb.h	= 20;

			headbar_uipara.strmess.x	= 2;
			headbar_uipara.strmess.y	= 2;
			headbar_uipara.strmess.w	= 100;//80;
			headbar_uipara.strmess.h	= 20;
			
			headbar_uipara.volume_icon.x 	= 360;
			headbar_uipara.volume_icon.y 	= 4;
			headbar_uipara.volume_icon.w = 17;
			headbar_uipara.volume_icon.h 	= 17;
			
			headbar_uipara.volume_txt.x 	= headbar_uipara.volume_icon.x + headbar_uipara.volume_icon.w + 1;		
			headbar_uipara.volume_txt.y 	= 2;
			headbar_uipara.volume_txt.w 	= 32;
			headbar_uipara.volume_txt.h 	= 20;			

			headbar_uipara.bright_icon.x 	= 280;		
			headbar_uipara.bright_icon.y 	= 4;
			headbar_uipara.bright_icon.w 	= 17;
			headbar_uipara.bright_icon.h 	= 17;	
			
			headbar_uipara.bright_txt.x 	= headbar_uipara.bright_icon.x + headbar_uipara.bright_icon.w + 1;		
			headbar_uipara.bright_txt.y 	= 2;
			headbar_uipara.bright_txt.w 	= 32;
			headbar_uipara.bright_txt.h 	= 20;
			
			headbar_uipara.battery.x 		= 440;
			headbar_uipara.battery.y 		= 6;
			headbar_uipara.battery.w 		= 30;
			headbar_uipara.battery.h 		= 12;

			headbar_uipara.res_id.vol_id0	= ID_INIT_ENERGY_0_BMP;
			headbar_uipara.res_id.vol_id1	= ID_INIT_ENERGY_1_BMP;
			headbar_uipara.res_id.vol_id2	= ID_INIT_ENERGY_2_BMP;
			headbar_uipara.res_id.vol_id3	= ID_INIT_ENERGY_3_BMP;
			headbar_uipara.res_id.vol_id4	= ID_INIT_ENERGY_4_BMP;
			headbar_uipara.res_id.vol_id5 	= ID_INIT_ENERGY_5_BMP;

			headbar_uipara.res_id.charge_vol_id0	= ID_INIT_CHARGE_ENERGY_0_BMP;
			headbar_uipara.res_id.charge_vol_id1	= ID_INIT_CHARGE_ENERGY_1_BMP;
			headbar_uipara.res_id.charge_vol_id2	= ID_INIT_CHARGE_ENERGY_2_BMP;
			headbar_uipara.res_id.charge_vol_id3	= ID_INIT_CHARGE_ENERGY_3_BMP;
			headbar_uipara.res_id.charge_vol_id4	= ID_INIT_CHARGE_ENERGY_4_BMP;
			headbar_uipara.res_id.charge_vol_id5 	= ID_INIT_CHARGE_ENERGY_5_BMP;
			headbar_uipara.res_id.volume_id		= ID_INIT_VOLUME_STATUS_BMP;
			headbar_uipara.res_id.volumemute_id	= ID_INIT_VOLUME_MUTE_BMP;
			headbar_uipara.res_id.bright_id		= ID_INIT_BRIGHT_BMP;
			headbar_uipara.res_id.hbarbg_id		= ID_INIT_HBAR_BG_BMP;

			headbar_uipara.font 				= font;
			break;
		}
			
		case SCREEN_TYPE_LAST:
		case SCREEN_TYPE_800_480:
		{
			//layer
			headbar_uipara.hbar_lyr.fb_w  = 480;
			headbar_uipara.hbar_lyr.fb_h  = 28+56;
			
			headbar_uipara.hbar_lyr.src_x = 0;
			headbar_uipara.hbar_lyr.src_y = 0;
			headbar_uipara.hbar_lyr.src_w = 480;
			headbar_uipara.hbar_lyr.src_h = 28+56;
			
			headbar_uipara.hbar_lyr.scn_x = 0;
			headbar_uipara.hbar_lyr.scn_y = -56;	
			headbar_uipara.hbar_lyr.scn_w = 480;
			headbar_uipara.hbar_lyr.scn_h = 28+56;
			
			headbar_uipara.hbar_lyr.pipe  = 1; 
			//
			headbar_uipara.hbar_frmwin.x = 0;		
			headbar_uipara.hbar_frmwin.y = 0;
			headbar_uipara.hbar_frmwin.w = 480;
			headbar_uipara.hbar_frmwin.h = 28+56;				
									
			headbar_uipara.strtf.x	= 10;
			headbar_uipara.strtf.y	= 4+56;
			headbar_uipara.strtf.w	= 20;
			headbar_uipara.strtf.h	= 20;

			headbar_uipara.strusb.x	= 30;
			headbar_uipara.strusb.y	= 4+56;
			headbar_uipara.strusb.w	= 30;
			headbar_uipara.strusb.h	= 20;

			headbar_uipara.strmess.x	= 2;
			headbar_uipara.strmess.y	= 4+56;
			headbar_uipara.strmess.w	= 100;//80;
			headbar_uipara.strmess.h	= 20;
			
			headbar_uipara.volume_icon.x 	= 310;
			headbar_uipara.volume_icon.y 	= 6+56;
			headbar_uipara.volume_icon.w = 17;
			headbar_uipara.volume_icon.h 	= 17;
			
			headbar_uipara.volume_txt.x 	= headbar_uipara.volume_icon.x + headbar_uipara.volume_icon.w + 1;		
			headbar_uipara.volume_txt.y 	= 4+56;
			headbar_uipara.volume_txt.w 	= 32;
			headbar_uipara.volume_txt.h 	= 20;			

			headbar_uipara.bright_icon.x 	= 120;		
			headbar_uipara.bright_icon.y 	= 6+55;
			headbar_uipara.bright_icon.w 	= 18;
			headbar_uipara.bright_icon.h 	= 18;	
			
			headbar_uipara.bright_txt.x 	= headbar_uipara.bright_icon.x + headbar_uipara.bright_icon.w + 1;		
			headbar_uipara.bright_txt.y 	= 4+56;
			headbar_uipara.bright_txt.w 	= 32;
			headbar_uipara.bright_txt.h 	= 20;
			
			headbar_uipara.battery.x 		= 430;
			headbar_uipara.battery.y 		= 8+56;
			headbar_uipara.battery.w 		= 30;
			headbar_uipara.battery.h 		= 12;

			headbar_uipara.res_id.vol_id0	= ID_INIT_ENERGY_0_BMP;
			headbar_uipara.res_id.vol_id1	= ID_INIT_ENERGY_1_BMP;
			headbar_uipara.res_id.vol_id2	= ID_INIT_ENERGY_2_BMP;
			headbar_uipara.res_id.vol_id3	= ID_INIT_ENERGY_3_BMP;
			headbar_uipara.res_id.vol_id4	= ID_INIT_ENERGY_4_BMP;
			headbar_uipara.res_id.vol_id5 	= ID_INIT_ENERGY_5_BMP;

			headbar_uipara.res_id.charge_vol_id0	= ID_INIT_CHARGE_ENERGY_0_BMP;
			headbar_uipara.res_id.charge_vol_id1	= ID_INIT_CHARGE_ENERGY_1_BMP;
			headbar_uipara.res_id.charge_vol_id2	= ID_INIT_CHARGE_ENERGY_2_BMP;
			headbar_uipara.res_id.charge_vol_id3	= ID_INIT_CHARGE_ENERGY_3_BMP;
			headbar_uipara.res_id.charge_vol_id4	= ID_INIT_CHARGE_ENERGY_4_BMP;
			headbar_uipara.res_id.charge_vol_id5 	= ID_INIT_CHARGE_ENERGY_5_BMP;
			headbar_uipara.res_id.volume_id		= ID_INIT_VOLUME_STATUS_BMP;
			headbar_uipara.res_id.volumemute_id	= ID_INIT_VOLUME_MUTE_BMP;
			headbar_uipara.res_id.bright_id		= ID_INIT_BRIGHT_BMP;
			headbar_uipara.res_id.hbarbg_id		= ID_INIT_HBAR_BG_BMP;

			headbar_uipara.font 				= font;
			break;
		}										
		default:
			__err(" unknown screen type \n");
			break;
	}		
}


