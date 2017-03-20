/*
**************************************************************************************************************
*											        ePDK
*						            the Easy Portable/Player Develop Kits
*									           FM app sample
*
*						        	(c) Copyright 2006-2007, ANDY, China
*											 All Rights Reserved
*
* File    : mbook_show.h
* By      : lyn
* Version : V1.00
* time    : 2009-8-14
**************************************************************************************************************
*/
#ifndef __MBOOK_SHOW_H__
#define __MBOOK_SHOW_H__

typedef     void*   H_SHOW; 

typedef struct tag_SHOW_CONFIG
{
    __u32       show_start;             // ��ʾ����ʼλ��
    __u32       show_width;             // ����ʾ�Ŀ��
    __u32	      scroll_width;
    __u32       page_row;               // һҳ��ʾ������
    __u32       row_height;             // �����м�߶�
    __u32       col_width;              // �����еĿ��
    __u32       font_color;             // �������ɫ
    __u32       back_color;             // ��������ɫ
    GUI_FONT*   char_font;              // �ֿ���
    __s32*      char_table;             // �ַ��ֿ��
    
    __epdk_charset_enm_e    charset;    // �ַ�����
}__show_config_t;


H_SHOW  MBOOK_Show_Init(void);

__s32   MBOOK_Show_Config(H_SHOW hdle, __show_config_t * config);

__s32   MBOOK_Show_Page(H_SHOW hdle, __hdle lyr, void * data);

__s32   MBOOK_Show_Uninit(H_SHOW hdle);

#endif /* __MBOOK_SHOW_H__ */


