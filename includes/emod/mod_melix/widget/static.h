/*
**************************************************************************************************************************
*													        eMOD
*						            		the Easy Portable/Player Develop Kits
*									   				melix widget module 
*
*                             		 Copyright(C), 2011-2012, Softwinner Microelectronic Co., Ltd.
*									               All Rights Reserved
*
* File name		:	static.h
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

#ifndef __STATIC_H__
#define __STATIC_H__


typedef void*   HSTATIC;

typedef enum GUI_STATIC_PAINT_FLAG
{
	GUI_STATIC_PAINT_FLAGS_CLOSE 	= 0,
	GUI_STATIC_PAINT_FLAGS_OPEN 	= 1
}gui_static_paint_flag_e;

typedef struct GUI_STATIC_CREATE_PARA
{
    RECT						ctrl_rect;		/*static 在屏幕中的矩形位置和区域*/	
    void        				*focus_bmp;		/*static 的焦点图片*/
    void        				*unfocus_bmp;	/*static 的非焦点图片*/
	GUI_POINT					bmp_pos;		/*static 图片所在位置的起始点,坐标相对于ctrl_rect*/
    char        				*text;			/*static 的输出文本*/
    GUI_POINT     				text_pos;		/*static 文本显示的位置的起始点,坐标相对于ctrl_rect*/
    GuiColor			   		ftxt_color;		/*static 文本焦点显示颜色*/
    GuiColor		   			uftxt_color;	/*static 文本非焦点显示颜色*/
	GUI_FONT*                   draw_font;		/*static 文本显示使用的字体*/
    GuiCharset					draw_code;   	/*字符编码*/
    GuiColor                    bk_color;		/*背景色*/
    __u32                       txt_align;		/*文本对齐方式*/
	gui_wrap_mode_t				wrap_mode;
}gui_static_create_para_t;

HSTATIC gui_static_create(gui_static_create_para_t *para);

__s32 gui_static_delete(HSTATIC hstatic);

__s32 gui_static_paint(HSTATIC hstatic);

void  gui_static_set_paint_flag(gui_static_paint_flag_e flag);

__s32 gui_static_set_focus_bmp(HSTATIC hstatic, HBITMAP bmp);

__s32 gui_static_set_unfocus_bmp(HSTATIC hstatic, HBITMAP bmp);

__s32 gui_static_set_text(HSTATIC hstatic, char * text);

__s32 gui_static_set_focus_text_color(HSTATIC hstatic, GuiColor color);

__s32 gui_static_set_unfocus_text_color(HSTATIC hstatic, GuiColor color);

__s32 gui_static_set_draw_font(HSTATIC hstatic, GUI_FONT * p_Font);

__s32 gui_static_set_draw_code(HSTATIC hstatic, GuiCharset code);

__s32 gui_static_set_draw_bkcolor(HSTATIC hstatic, GuiColor color);

__s32 gui_static_start_move(HSTATIC hstatic);

__s32 gui_static_stop_move(HSTATIC hstatic);


#endif	/*__static_H__*/

