/*
**************************************************************************************************************************
*													        eMOD
*						            		the Easy Portable/Player Develop Kits
*									   				melix widget module 
*
*                             		 Copyright(C), 2011-2012, Softwinner Microelectronic Co., Ltd.
*									               All Rights Reserved
*
* File name		:	slider.h
* Author			: 	Kiden
* Version 		: 	V1.00
* Date    		: 	2012-04-10
* Description	:
* Others 		:
* History 		:
* <Author>           <time>          <version>      <description>
* Kiden		    2012-04-10         1.0.0          Create File
*
**************************************************************************************************************************
*/

#ifndef __SLIDER_H__
#define __SLIDER_H__

typedef void*	HSLIDER;
typedef __u32  HBMP;

typedef enum
{
	VERT = 0,
	HORI = 1,
}type_flag_e;

typedef struct GUI_SLIDER_CREATE_PARA
{
	RECT	disp_rect;				//显示区域
	void	*head_bmp_buf;			//头部图片buffer
	void	*tail_bmp_buf;			//尾部图片buffer
	void	*cursor_bmp_buf;		//指针图片buffer
	__u32	ft_color;				//前景颜色
	__u32	bk_color;				//背景颜色
	__u32	cursor_width;			//指针宽度
	__u32	cursor_height;			//指针高度
	__s32	min;					//范围最小值
	__s32	max;					//范围最大值
	__s32   value;					//初始化值
	__s32	type;					//水平或垂直
}gui_slider_create_para_t;

HSLIDER gui_slider_create(gui_slider_create_para_t *para);
__s32 gui_slider_delete(HSLIDER hslider);
__s32 gui_slider_paint(HSLIDER hslider);
__s32 gui_slider_setvalue(HSLIDER hslider, __s32 value);
__s32 gui_slider_setrange(HSLIDER hslider, __s32 max, __s32 min);
__s32 gui_slider_getmax(HSLIDER hslider);
__s32 gui_slider_getmin(HSLIDER hslider);
__s32 gui_slider_getvalue(HSLIDER hslider);
__s32 gui_slider_length_inc(HSLIDER hslider, __u32 size);
__s32 gui_slider_length_dec(HSLIDER hslider, __u32 size);
__s32 gui_slider_length_cal(HSLIDER hslider, __u32 x, __u32 y);

#endif

