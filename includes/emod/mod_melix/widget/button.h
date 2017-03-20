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
    RECT						ctrl_rect;		/*button����Ļ�еľ���λ�ú�����*/
	
    void        				*focus_bmp;		/*button �Ľ���ͼƬ*/
    void        				*unfocus_bmp;	/*button �ķǽ���ͼƬ*/
	GuiPos						bmp_pos;		/*button ͼƬ����λ�õ���ʼ��,���������ctrl_rect*/
    char        				*text;			/*button ������ı�*/
    GuiPos	     				text_pos;		/*button �ı���ʾ��λ�õ���ʼ��,���������ctrl_rect*/
    __u32			   			ftxt_color;		/*button �ı�������ʾ��ɫ*/
    __u32		   				uftxt_color;	/*button �ı��ǽ�����ʾ��ɫ*/
	GUI_FONT*                   draw_font;		/*button �ı���ʾʹ�õ�����*/
    __epdk_charset_enm_e        draw_code;   	/*�ַ�����*/
    __u32                       bk_color;		/*����ɫ*/
    __u32                       txt_align;		/*�ı����뷽ʽ*/
    __bool                      alpha_en;		/*�������ݲ���*/
}gui_button_create_para_t;



HBUTTON  ButtonCreate(gui_button_create_para_t * para);

__s32   ButtonDelete(HBUTTON hdle);

__s32   ButtonSetTextId(HBUTTON hdle, __u32 id);

__s32   ButtonSetTextColor(HBUTTON hdle, __u32 color);

__s32   ButtonSetFocusIcon(HBUTTON hdle, __u32 id);

__s32   ButtonSetUnFocusIcon(HBUTTON hdle, __u32 id);

#endif	/*__BUTTON_H__*/

