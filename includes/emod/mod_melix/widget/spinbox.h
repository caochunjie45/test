/*
**************************************************************************************************************************
*													        eMOD
*						            		the Easy Portable/Player Develop Kits
*									   				melix widget module 
*
*                             		 Copyright(C), 2011-2012, Softwinner Microelectronic Co., Ltd.
*									               All Rights Reserved
*
* File name		:	spinbox.h
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

#ifndef __SPINBOX_H__
#define __SPINBOX_H__

typedef void *HSPINBOX;

typedef enum GUI_SPINBOX_PAINT_STATE
{
	DOWN = 0,
	UP
}gui_spinbox_paint_state_e;

typedef enum GUI_SPINBOX_PAINT_FLAG
{
	DEC = 0,
	INC
}gui_spinbox_paint_flag_e;

typedef struct GUI_SPINBOX_CREATE_PARA
{
	RECT			ctrl_rect;
	void			*fdn_bmp;
	void			*fup_bmp;
	void			*ufdn_bmp;
	void			*ufup_bmp;

	__u32			dn_width;
	__u32			dn_height;
	__u32			up_width;
	__u32			up_height;
	
	GuiPos			upbmp_pos;
	GuiPos			dnbmp_pos;
	GuiPos			text_pos;

	__u32			text_width;
	__u32			text_height;

	__s32			cur_value;
	__s32			max_value;
	__s32			min_value;

	GuiColor		ftxt_color;
	GuiColor		uftxt_color;
	GuiColor        bk_color;
	
	GUI_FONT		*draw_font;	
	GuiCharset		draw_code;
	
}gui_spinbox_create_para_t;

HSPINBOX gui_spinbox_create(gui_spinbox_create_para_t *para);

__s32 gui_spinbox_delete(HSPINBOX hspinbox);

__s32 gui_spinbox_paint(HSPINBOX hspinbox);

__bool gui_spinbox_touch_dec(HSPINBOX hspinbox, __u32 x, __u32 y);

__bool gui_spinbox_touch_inc(HSPINBOX hspinbox, __u32 x, __u32 y);

void gui_spinbox_set_paint_flag(gui_spinbox_paint_state_e state, gui_spinbox_paint_flag_e flag);

__s32 gui_spinbox_get_curvalue(HSPINBOX hspinbox, __s32 *value);

__s32 gui_spinbox_set_curvalue(HSPINBOX hspinbox, __s32 value);

__s32 gui_spinbox_set_maxvalue(HSPINBOX hspinbox, __s32 value);

__s32 gui_spinbox_set_minvalue(HSPINBOX hspinbox, __s32 value);

__s32 gui_spinbox_set_bkcolor(HSPINBOX hspinbox, GuiColor color);

__s32 gui_spinbox_set_uftxt_color(HSPINBOX hspinbox, GuiColor color);

__s32 gui_spinbox_set_ftxt_color(HSPINBOX hspinbox, __s32 color);

__s32 gui_spinbox_set_fdn_bmp(HSPINBOX hspinbox, HBITMAP bmp);

__s32 gui_spinbox_set_ufdn_bmp(HSPINBOX hspinbox, HBITMAP bmp);

__s32 gui_spinbox_set_fup_bmp(HSPINBOX hspinbox, HBITMAP bmp);

__s32 gui_spinbox_set_ufup_bmp(HSPINBOX hspinbox, HBITMAP bmp);


#endif
