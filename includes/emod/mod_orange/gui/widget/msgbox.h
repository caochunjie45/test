/*
*******************************************************************************************************************
*                                                         	Module Orange
*                                         			the embedded Graphical module
*
*                             				 Copyright(C), 2006-2008, Softwinner Microelectronic Co., Ltd.
*											               All Rights Reserved
*
*File Name�� 	gmsgbox.h
*
*Author�� 		William Wang
*
*Version :   	1.1.0
*
*Date:  		2009-10-19
*
*Description : 	global message box definition 
*
*Others : 		None at present.
*
* History :
*
* <Author>          <time>          <version>     <description>
*
* William Wang     2009-10-19         1.1.0          Create File
*
*******************************************************************************************************************
*/

#ifndef _GUI_GMSGBOX_H_
#define _GUI_GMSGBOX_H_

typedef struct taggmsgboxpara
{
    void        				*bk_bmp;
    char        				*text;
    __pos_t     				text_pos;
    __u32		   				txt_color;
	GUI_FONT*                   draw_font;
    __epdk_charset_enm_e        draw_code;   /*�ַ�����*/
    __bool                      alpha_en;
    __u32                       timeout;
}msgbox_para_t;

typedef struct __GMSGBOXCREATE
{
    char                            *name;                  /** ȫ����Ϣ�Ի������� */

    /*��msgbox���ԣ�ǰ���������button��para��Ϣ*/
    __gui_dlgtemplate_t             *pDlgTemplate;          /** ȫ����Ϣ�Ի��򴴽�ģ�� */
    H_WIN                           hOwner;                 /** ȫ����Ϣ�Ի���Ի���Ĵ������ھ�� */
    H_WIN                           hHosting;               /* ȫ����Ϣ�Ի���Ի��������ھ��*/
    H_WIN                           hLayer;                 /** ȫ����Ϣ�Ի����ͼ���� */
    __u32                           type;                   /*  ȫ����Ϣ�Ի��������*/
    msgbox_para_t                   *skin;                  /** ȫ����Ϣ�Ի��򴴽��������� */
}__gui_gmsgboxcreate_para_t;

typedef __gui_gmsgboxcreate_para_t *pmsgboxcreate;


__u32  GUI_GMsgBox (__gui_gmsgboxcreate_para_t *create_info);
__s32  GUI_GMsgBoxSetText(H_WIN hbox,char *text);
__s32  GUI_GMsgBoxSetTextPos(H_WIN hbox,__pos_t *pos);

#endif /* #ifndef _GUI_MSGBOX_H_ */
