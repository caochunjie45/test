/*
*********************************************************************************************************
*											        ePDK
*						            the Easy Portable/Player Develop Kits
*									           record app sample
*
*						        	(c) Copyright 2006-2009, China
*											 All Rights Reserved
*
* File    : mbook_font.c
* By      : lyn
* Version : V1.00
*********************************************************************************************************
*/

#include "mbook_private.h"

typedef struct tag_MBOOK_FONT
{
    __s32       font_size;
    __s32*      font_table;
    GUI_FONT*   char_font;
}__mbook_font_t;


/*
************************************************************************************************************************
*                       				MBOOK_Font_Init
*
*Description: ��������������
*
*Arguments  : path: �ֿ���·��
*             size������Ĵ�С  
*             
*
*Return     : �ɹ������ز������
*			  ʧ�ܣ�NULL
*
************************************************************************************************************************
*/
H_FONT  MBOOK_Font_Init(GUI_FONT *ebook_font, __u32 size)
{
    __u32               i;
    __mbook_font_t*     p_font;
    
    // �����ڴ�
	p_font = (__mbook_font_t *)My_Malloc(0, sizeof(__mbook_font_t));	
	if(p_font == NULL)
	{
		__err(" malloc error \n");
		return NULL;
	} 
  
	p_font->char_font = ebook_font;
    // �����ַ��ֿ����ڴ�
	p_font->font_table = (__s32 *)My_Malloc(0, 1024);	// �����ڴ�
	if(p_font->font_table == NULL)
	{
		__err(" malloc error \n");
		goto FONT_ERROR_2;
	}
	// ����  
    GUI_SetFont(p_font->char_font);
	for(i = 0; i < 256; i++)
	{
	    *(p_font->font_table + i) = GUI_GetCharDistX(i);
	}
    return p_font; 
    
FONT_ERROR_2:
   // GUI_SetFont(&GUI_Font6x8);
	GUI_TTF_Done(p_font->char_font);
	p_font->char_font = NULL;

FONT_ERROR_1:
    My_Mfree(0, p_font);
    
	return NULL;      
}

/*
************************************************************************************************************************
*                       				MBOOK_Font_Uninit
*
*Description: �ͷ�����������
*
*Arguments  : hdle������������ 
*             
*
*Return     : �ɹ���EPDK_OK
*			  ʧ�ܣ�EPDK_FAIL
*
************************************************************************************************************************
*/
__s32   MBOOK_Font_Uninit(H_FONT hdle)
{
    __mbook_font_t*     p_font;
    
    if(hdle == NULL)
    {
        return EPDK_FAIL;
    }
    p_font = (__mbook_font_t *)hdle;
    if(p_font->font_table)
    {
        My_Mfree(0, p_font->font_table);
        p_font->font_table = NULL;
    } 
    My_Mfree(0, p_font);   
    
    return EPDK_OK;
}

/*
************************************************************************************************************************
*                       				MBOOK_Font_GetTable
*
*Description: ��ȡ�ַ����ֿ��
*
*Arguments  : hdle������������ 
*             
*
*Return     : �ɹ����ַ��ֿ��
*			  ʧ�ܣ�NULL
*
************************************************************************************************************************
*/
__s32*  MBOOK_Font_GetTable(H_FONT hdle)
{
    __mbook_font_t*     p_font;
    
    if(hdle == NULL)
    {
        return NULL;
    }
    p_font = (__mbook_font_t *)hdle;
    
    return p_font->font_table;    
}

/*
************************************************************************************************************************
*                       				MBOOK_Font_GetTable
*
*Description: ��ȡ�ֿ���
*
*Arguments  : hdle������������ 
*             
*
*Return     : �ɹ����ֿ���
*			  ʧ�ܣ�NULL
*
************************************************************************************************************************
*/
GUI_FONT*   MBOOK_Font_GetFont(H_FONT hdle)
{
    __mbook_font_t*     p_font;
    
    if(hdle == NULL)
    {
        return NULL;
    }
    p_font = (__mbook_font_t *)hdle;
    
    return p_font->char_font;  
}




