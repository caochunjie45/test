/*
*********************************************************************************************************
*											        ePDK
*						            the Easy Portable/Player Develop Kits
*									           willow app sample
*
*						        (c) Copyright 2006-2007, JOHN, China
*											All	Rights Reserved
*
* File    : dtv_uipara.h
* By      : john.fu
* Version : V1.00
*********************************************************************************************************
*/

#ifndef _DTV_UIPARA_H_
#define _DTV_UIPARA_H_

#include "apps.h"

//#include "dtv_ui_i.h"

typedef struct DTV_UIPARA_LAYER
{
    __pixel_rgbfmt_t    fb_fmt;             //图层格式
    
    SIZE 	fb_size;					  	//framebuffer大小
    
    RECT    src_rect;					  	//srcwin区域	
    
    RECT 	scn_rect;						//scnwin区域
    
	__u32  reserved[32];					//预留
	
}__dtv_uipara_layer_t;

typedef struct DTV_UIPARA
{
    __dtv_uipara_layer_t    main_layer;

	RECT                    record_staue_layer;
	
}__dtv_uipara_t;


__dtv_uipara_t * dtv_get_uipara(void);

#endif //_DTV_UIPARA_H_
