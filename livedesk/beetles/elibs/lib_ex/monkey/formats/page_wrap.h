/*
**************************************************************************************************************
*											         ePDK
*						            the Easy Portable/Player Develop Kits
*									           desktop system 
*
*						        	 (c) Copyright 2007-2010, ANDY, China
*											 All Rights Reserved
*
* File    	: page_wrap.h
* By      	: Andy.zhang
* Version	: v1.0
* ============================================================================================================
* 2009-11-3 9:39:42  andy.zhang  create this file, implements the fundemental interface;
**************************************************************************************************************
*/

#ifndef __PAGE_WRAP_H__
#define __PAGE_WRAP_H__


struct  tag_PageContent;
typedef struct tag_PageContent PageContent;

/* ��ҳ�����ò��� */
typedef struct
{
	ES_FILE		*fd;				/* �ļ���� 	*/
	int          len;				/* �ļ�����		*/	
	H_LYR       hlyr;				/* ͼ����� 	*/
	GUI_FONT    *pFont;				/* ͼ��������	*/
	__s32		lineWidth;			/* �п� 		*/	
	__s32		lineNum;			/* ҳ���� 		*/
	MkEncode	defEncodeStyle;		/* Ĭ�ϱ��뷽ʽ	*/
	__s32		startOffset;		/* ��ʼƫ����	*/
}PageConfig;

/* page wrap ���� */
PageContent* pagewrap_create( PageConfig *para );

/* page wrap ���� */
__s32  	pagewrap_setting(PageContent *content, PageConfig *para);

/* ����ʼҳ���� */
__s32 	pagewrap_start(PageContent *content);

/* ��ȡ��һҳ���� */
__s32  	pagewrap_next(PageContent *content);

/* ��ȡ��һҳ���� */
__s32  	pagewrap_prev(PageContent *content);

/* ��ת��offset�� */
__s32  	pagewrap_fseek(PageContent *content,  int offset);

/* ��ѯ����Ϣ�� */
MkLine *pagewrap_getLine(PageContent *content);

/* ��ʾһ���ַ�*/
void 	pagewrap_displayStr(PageContent *content, char *s, int len , int x, int y);

/* ��ѯҳͷƫ���� */
__s32	pagewrap_getStartOffset(PageContent *content);

/* ��ѯҳβƫ���� */
__s32	pagewrap_getEndOffset(PageContent *content);

/* ��ѯ�Ƿ������ʼ��*/
__bool	pagewrap_isHead(PageContent *content);

/* ��ѯ�Ƿ����½�β�� */
__bool	pagewrap_isTail(PageContent *content);

/* ɾ�� page wrap ��� */
__s32	pagewrap_delete(PageContent *content);

#endif /* __PAGE_WRAP_H__ */

