/*
**************************************************************************************************************************
*													        eMOD
*						            		the Easy Portable/Player Develop Kits
*									   				includes melix module 
*
*                             		 Copyright(C), 2011-2012, Softwinner Microelectronic Co., Ltd.
*									               All Rights Reserved
*
* File name		:	gui_config.h
* Author		: 	Derek
* Version 		: 	V1.00
* Date    		: 	2012-02-14
* Description	:
* Others 		:
* History 		:
* <Author>           <time>          <version>      <description>
* Derek Lai		    2012-02-14         1.0.0          Create File
*
**************************************************************************************************************************
*/
#ifndef __GUI_CONFIG_H__
#define __GUI_CONFIG_H__

/*操作系统定义*/
#define GUI_OS_LINUX        0x10
#define GUI_OS_EPOS         0x11
#define GUI_OS_WINCE        0x12

#define GUI_WIN_OS      GUI_OS_EPOS

/*GUI_DISPDIR: config the display direction					*/
/*if	0x0: 0 degree direction								*/
/*		0x1: 90 degree direction							*/
/*		0x2: 180 degree direction							*/
/*		0x3: 270 degree direction							*/
#define GUI_DISPDIR		0x0

/*GUI_SUPPORT_MP: config the mp that isn't support			*/
/*if	0: not support										*/
/*		1: support											*/
#define GUI_SUPPORT_MP	1

/*DISP_LAYER_WORK_MODE: config the display layer work mode 	*/
/*if 	0x0:normal work mode							   	*/
/*		0x1:palette work mode								*/
/*		0x2:internal frame buffer work mode				   	*/
#define GUI_DISP_LAYER_WORK_MODE	0x0

/*DISP_PIXEL_FORMAT: config the display pixel format		*/
/*if	0x0: 1bpp											*/
/*		0x1: 2bpp											*/
/*		0x2: 4bpp											*/
/*		0x3: 8bpp											*/
/*		0x4: RGB655											*/
/*		0x5: RGB565											*/
/*		0x6: RGB556											*/
/*		0x7: ARGB1555										*/
/*		0x8: RGBA5551										*/
/*		0x9: RGB888											*/
/*		0xa: ARGB8888										*/
#define GUI_DISP_PIXEL_FORMAT	0xa

/*DISP_PIXEL_SEQ: config the display pixel bits sequence	*/
/*if	0x0: ARGB,A在高位									*/
/*		0x2: BGRA											*/
/*		0xd: 16bpp P10,p1在高位								*/
/*		0xe: 16bpp P01,p0在高位								*/
/*		0xf: 8bpp P3210, p3在高位							*/
/*		0x10: 8bpp P0123, p0在高位							*/
#define GUI_DISP_PIXEL_SEQ	0x0

/*DISP_COLOR_SPACE_MODE: config the display color space mode*/
/*if	0x0: BT601,用于标清视频								*/
/*		0x1: BT709,用于高清视频								*/
/*		0x2: YCC,一般用于图片								*/
/*		0x3: VXYCC,保留										*/
/*		0x4: MONO,黑白模式空间								*/
#define GUI_DISP_COLOR_SPACE_MODE	0x0


#endif	/*__GUI_CONFIG_H__*/

