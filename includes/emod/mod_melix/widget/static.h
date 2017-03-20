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
    RECT						ctrl_rect;		/*static ����Ļ�еľ���λ�ú�����*/	
    void        				*focus_bmp;		/*static �Ľ���ͼƬ*/
    void        				*unfocus_bmp;	/*static �ķǽ���ͼƬ*/
	GUI_POINT					bmp_pos;		/*static ͼƬ����λ�õ���ʼ��,���������ctrl_rect*/
    char        				*text;			/*static ������ı�*/
    GUI_POINT     				text_pos;		/*static �ı���ʾ��λ�õ���ʼ��,���������ctrl_rect*/
    GuiColor			   		ftxt_color;		/*static �ı�������ʾ��ɫ*/
    GuiColor		   			uftxt_color;	/*static �ı��ǽ�����ʾ��ɫ*/
	GUI_FONT*                   draw_font;		/*static �ı���ʾʹ�õ�����*/
    GuiCharset					draw_code;   	/*�ַ�����*/
    GuiColor                    bk_color;		/*����ɫ*/
    __u32                       txt_align;		/*�ı����뷽ʽ*/
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

