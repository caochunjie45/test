/*
**************************************************************************************************************************
*													        eMOD
*						            		the Easy Portable/Player Develop Kits
*									   				melix includes module 
*
*                             		 Copyright(C), 2011-2012, Softwinner Microelectronic Co., Ltd.
*									               All Rights Reserved
*
* File name		:	gui.h
* Author		: 	Derek
* Version 		: 	V1.00
* Date    		: 	2012-02-07
* Description	:
* Others 		:
* History 		:
* <Author>           <time>          <version>      <description>
* Derek Lai		    2012-02-07         1.0.0          Create File
*
**************************************************************************************************************************
*/
#ifndef __GUI_H__
#define __GUI_H__

typedef enum
{
    GUI_LAYER_WORK_MODE_NORMAL     = 0,    //normal work mode
    GUI_LAYER_WORK_MODE_PALETTE    = 1,    //palette work mode 
    GUI_LAYER_WORK_MODE_INTER_BUF  = 2,    //internal frame buffer work mode
    GUI_LAYER_WORK_MODE_GAMMA      = 3,    //gamma correction work mode
    GUI_LAYER_WORK_MODE_SCALER     = 4,    //scaler work mode
}gui_layer_work_mode_e;

typedef enum
{
    GUI_FORMAT_1BPP        =0x0,
    GUI_FORMAT_2BPP        =0x1,
    GUI_FORMAT_4BPP        =0x2,
    GUI_FORMAT_8BPP        =0x3,
    GUI_FORMAT_RGB655      =0x4,
    GUI_FORMAT_RGB565      =0x5,
    GUI_FORMAT_RGB556      =0x6,
    GUI_FORMAT_ARGB1555    =0x7,
    GUI_FORMAT_RGBA5551    =0x8,
    GUI_FORMAT_RGB888      =0x9,
    GUI_FORMAT_ARGB8888    =0xa,

    GUI_FORMAT_YUV444      =0xb,
    GUI_FORMAT_YUV422      =0xc,
    GUI_FORMAT_YUV420      =0xd,
    GUI_FORMAT_YUV411      =0xe,
    GUI_FORMAT_CSIRGB      =0xf,
}gui_pixel_fmt_e;

typedef enum
{
//for interleave argb8888
    GUI_SEQ_ARGB   =0x0,//A在高位
    GUI_SEQ_BGRA   =0x2,
    
//for nterleaved yuv422
    GUI_SEQ_UYVY   =0x3,  
    GUI_SEQ_YUYV   =0x4,
    GUI_SEQ_VYUY   =0x5,
    GUI_SEQ_YVYU   =0x6,
    
//for interleaved yuv444
    GUI_SEQ_AYUV   =0x7,  
    GUI_SEQ_VUYA   =0x8,
    
//for uv_combined yuv420
    GUI_SEQ_UVUV   =0x9,  
    GUI_SEQ_VUVU   =0xa,
    
//for 16bpp rgb
    GUI_SEQ_P10    = 0xd,//p1在高位
    GUI_SEQ_P01    = 0xe,//p0在高位
    
//for planar format or 8bpp rgb
    GUI_SEQ_P3210  = 0xf,//p3在高位
    GUI_SEQ_P0123  = 0x10,//p0在高位
    
//for 4bpp rgb
    GUI_SEQ_P76543210  = 0x11,
    GUI_SEQ_P67452301  = 0x12,
    GUI_SEQ_P10325476  = 0x13,
    GUI_SEQ_P01234567  = 0x14,
    
//for 2bpp rgb
    GUI_SEQ_2BPP_BIG_BIG       = 0x15,//15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0
    GUI_SEQ_2BPP_BIG_LITTER    = 0x16,//12,13,14,15,8,9,10,11,4,5,6,7,0,1,2,3
    GUI_SEQ_2BPP_LITTER_BIG    = 0x17,//3,2,1,0,7,6,5,4,11,10,9,8,15,14,13,12
    GUI_SEQ_2BPP_LITTER_LITTER = 0x18,//0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15
    
//for 1bpp rgb
    GUI_SEQ_1BPP_BIG_BIG       = 0x19,//31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0
    GUI_SEQ_1BPP_BIG_LITTER    = 0x1a,//24,25,26,27,28,29,30,31,16,17,18,19,20,21,22,23,8,9,10,11,12,13,14,15,0,1,2,3,4,5,6,7
    GUI_SEQ_1BPP_LITTER_BIG    = 0x1b,//7,6,5,4,3,2,1,0,15,14,13,12,11,10,9,8,23,22,21,20,19,18,17,16,31,30,29,28,27,26,25,24
    GUI_SEQ_1BPP_LITTER_LITTER = 0x1c,//0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
}gui_pixel_seq_t;

typedef enum
{
    GUI_BT601  = 0,
    GUI_BT709  = 1,
    GUI_YCC    = 2,
    GUI_VXYCC  = 3,
    GUI_MONO   = 4,
}gui_cs_mode_t;

typedef struct GUI_XFB
{
	SIZE 			xfb_size;
	void 			*xfb_addr;
	gui_pixel_fmt_e pixel_fmt;
}gui_xfb_t;


#include "common\\common.h"

#include "message\\message.h"

#include "gdi\\gdi.h"
#include "gdi\\animation.h"

#include "activity\\activity.h"
#include "activity\\activity_manager.h"

#include "gview\\gview.h"

//#include "message\\message.h"

#include "widget\\widget.h"
#include "widget\\button.h"
#include "widget\\listview.h"
#include "widget\\longstring.h"
#include "widget\\slider.h"
#include "widget\\spinbox.h"
#include "widget\\static.h"



#include "resource\\LangDec.h"
#include "resource\\themdec.h"

#endif	/*__GUI_H__*/

