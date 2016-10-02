/*
*********************************************************************************************************
*											        ePDK
*						            the Easy Portable/Player Develop Kits
*									           willow app sample
*
*						        (c) Copyright 2006-2007, JOHN, China
*											All	Rights Reserved
*
* File    : spsc_uipara.c
* By      : john.fu
* Version : V1.00
*********************************************************************************************************
*/

#include "dtv_uipara.h"

static __dtv_uipara_t dtv_uipara_800_480 =
{
    {//sub
    	PIXEL_COLOR_ARGB8888, 	//fmt
    	{800, 480},		  		//fb_size	
    	{0, 0, 800, 480}, 	  	//src_rect
 		{0, 0, 800, 480},     	//scr_rect
    },
    {600,50,157,88},			// recording layer
   
};


static __dtv_uipara_t dtv_uipara_480_272 =
{
    {
    	PIXEL_COLOR_ARGB8888, 	//fmt
    	{480, 272},		  		//fb_size	
    	{0, 0, 480, 272}, 	  	//src_rect
 		{0, 0, 480, 272},     	//scr_rect
    },
    
    {300,30,157,88},			// recording layer

};

static __dtv_uipara_t dtv_uipara_720_576 =
{
    {
    	PIXEL_COLOR_ARGB8888, 	//fmt
    	{720, 576},		  		//fb_size	
    	{0, 0,720, 576}, 	  	//src_rect
 		{0, 0, 720, 576},     //scr_rect
    },
    {500,50,157,88},			// recording layer
        
};

static __dtv_uipara_t dtv_uipara_720_480 =
{
    {
    	PIXEL_COLOR_ARGB8888, 	//fmt
    	{720, 480},		  		//fb_size	
    	{0, 0, 720, 480}, 	  	//src_rect
 		{0, 0, 720, 480},     //scr_rect
    },
    {600,50,157,88},			// recording layer
        
};

static __dtv_uipara_t dtv_uipara_1024_600 =
{
    {
    	PIXEL_COLOR_ARGB8888, 	//fmt
    	{1024, 600},		  	//fb_size	
    	{0, 0, 1024, 600}, 	  	//src_rect
 		{0, 0, 1024, 600},       //scr_rect
    },
    {850,50,157,88},			// recording layer
        
};



__dtv_uipara_t* dtv_get_uipara(void)
{
	ES_FILE			*disp;
	__s32 			screen_width;
	__s32 			screen_height;
	
	/* get lcd size*/
	disp = eLIBs_fopen("b:\\DISP\\DISPLAY", "r+");
	if ( disp == ((ES_FILE *)0) )
	{
		return NULL;	
	}	

	screen_height = eLIBs_fioctrl(disp, DISP_CMD_SCN_GET_HEIGHT, 0, 0);
	screen_width  = eLIBs_fioctrl(disp, DISP_CMD_SCN_GET_WIDTH, 0, 0);
	
	eLIBs_fclose(disp);	
	
	if((screen_width == 800 )&&( screen_height == 480))
		return &dtv_uipara_800_480;
		
	else if((screen_width == 720)&&( screen_height == 576))
		return &dtv_uipara_720_576;	
	else if((screen_width == 720)&&(screen_height == 480))
		return &dtv_uipara_720_480;
	else if ((screen_width == 480)&&(screen_height == 272))
		return &dtv_uipara_480_272;
		
	else if ((screen_width == 1024)&&(screen_height == 600))
		return &dtv_uipara_1024_600;
		
	else
		return &dtv_uipara_720_480;
			
}
