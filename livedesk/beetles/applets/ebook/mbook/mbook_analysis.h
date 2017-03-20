/*
**************************************************************************************************************
*											        ePDK
*						            the Easy Portable/Player Develop Kits
*									           FM app sample
*
*						        	(c) Copyright 2006-2007, ANDY, China
*											 All Rights Reserved
*
* File    : mbook_analysis.h
* By      : lyn
* Version : V1.00
* time    : 2009-8-14
**************************************************************************************************************
*/
#ifndef __MBOOK_ANALYSIS_H__
#define __MBOOK_ANALYSIS_H__

#define     MAX_SHOW_PAGE       100000

typedef     void*   H_ANALYSIS; 

typedef struct tag_ONE_PAGE     // ҳ��12 byte
{
    __u32       page_no;        //ҳ�� 
    __u32       page_start;     //��ʼλ�� �ļ�ƫ����
    __u32       page_end;       //����λ�� �ļ�ƫ����   
}__one_page_t;

typedef struct tag_INFO_CONFIG
{
    __u32       show_width;     // �ı���ʾ�Ŀ��
    __u32       page_row;       // һҳ����ʾ������
    __u32       col_width;      // �����м�Ŀ��
    __s32*      char_table;     // �ַ��ֿ��
    
    __epdk_charset_enm_e    charset;
}__analysis_config_t;


H_ANALYSIS  MBOOK_Analysis_Init(ES_FILE * fd);

__s32   MBOOK_Analysis_Config(H_ANALYSIS hdle, __analysis_config_t * config);

__s32   MBOOK_Analysis_Work(H_ANALYSIS hdle);

__s32   MBOOK_Analysis_GetInfo(H_ANALYSIS hdle, __u32 page_no, __one_page_t * page_info);

__s32   MBOOK_Analysis_GetPage(H_ANALYSIS hdle, __u32 offset);

__s32   MBOOK_Analysis_GetTotalPage(H_ANALYSIS hdle);

__s32   MBOOK_Analysis_Uninit(H_ANALYSIS hdle);


#endif /* __MBOOK_ANALYSIS_H__ */

