/*
**************************************************************************************************************************
*													        eMOD
*						            		the Easy Portable/Player Develop Kits
*									   				melix includes module 
*
*                             		 Copyright(C), 2011-2012, Softwinner Microelectronic Co., Ltd.
*									               All Rights Reserved
*
* File name		:	button.h
* Author		: 	Derek
* Version 		: 	V1.00
* Date    		: 	2012-02-20
* Description	:
* Others 		:
* History 		:
* <Author>           <time>          <version>      <description>
* Derek Lai		    2012-02-20         1.0.0          Create File
*
**************************************************************************************************************************
*/
#ifndef __BUTTON_H__
#define __BUTTON_H__


typedef void*   HBUTTON;

typedef enum GUI_BUTTON_PAINT_FLAG
{
	GUI_BUTTON_PAINT_FLAGS_CLOSE 	= 0,
	GUI_BUTTON_PAINT_FLAGS_OPEN 	= 1
}gui_button_paint_flag_e;

typedef struct GUI_BUTTON_CREATE_PARA
{
	__u32						ctrl_id;		/*button id*/
    RECT						ctrl_rect;		/*button在屏幕中的矩形位置和区域*/
	
    void        				*focus_bmp;		/*button 的焦点图片*/
    void        				*unfocus_bmp;	/*button 的非焦点图片*/
	GuiPos						bmp_pos;		/*button 图片所在位置的起始点,坐标相对于ctrl_rect*/
    char        				*text;			/*button 的输出文本*/
    GuiPos	     				text_pos;		/*button 文本显示的位置的起始点,坐标相对于ctrl_rect*/
    __u32			   			ftxt_color;		/*button 文本焦点显示颜色*/
    __u32		   				uftxt_color;	/*button 文本非焦点显示颜色*/
	GUI_FONT*                   draw_font;		/*button 文本显示使用的字体*/
    __epdk_charset_enm_e        draw_code;   	/*字符编码*/
    __u32                       bk_color;		/*背景色*/
    __u32                       txt_align;		/*文本对齐方式*/
    __bool                      alpha_en;		/*保留，暂不用*/
}gui_button_create_para_t;



HBUTTON  ButtonCreate(gui_button_create_para_t * para);

__s32   ButtonDelete(HBUTTON hdle);

__s32   ButtonSetTextId(HBUTTON hdle, __u32 id);

__s32   ButtonSetTextColor(HBUTTON hdle, __u32 color);

__s32   ButtonSetFocusIcon(HBUTTON hdle, __u32 id);

__s32   ButtonSetUnFocusIcon(HBUTTON hdle, __u32 id);

#endif	/*__BUTTON_H__*/

