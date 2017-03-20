/*
**************************************************************************************************************
*											         ePDK
*						            the Easy Portable/Player Develop Kits
*									           desktop system 
*
*						        	 (c) Copyright 2007-2010, ANDY, China
*											 All Rights Reserved
*
* File    	: dsk_backgrd.c
* By      	: Andy.zhang
* Func		: desktop toolbar control interface
* Version	: v1.0
* ============================================================================================================
* 2009-7-20 8:51:52  andy.zhang  create this file, implements the fundemental interface;
**************************************************************************************************************
*/

#ifndef __DSK_HEADBAR_H__
#define __DSK_HEADBAR_H__

typedef enum tag_SET_HBAR
{
	SET_HBAR_SHOW = 1,
	SET_HBAR_HIDE	
}__set_hbar_t;

typedef enum tag_HBAR_FORMAT
{
	HBAR_FOARMAT_8BPP,
	HBAR_FOARMAT_32BPP	
}__hbar_format_t;

__s32 dsk_hbar_set_state(__set_hbar_t state);		/* ����״̬ */
__set_hbar_t dsk_hbar_get_state(void);				/* ��ѯ״̬ */

__s32 dsk_hbar_set_format(__hbar_format_t format);	/* ����hbar format */


/* note : title Ϊutf8�����ʽ�ַ�����len<=32 */
__s32 dsk_hbar_set_title(char *title, __u32 len);	/* ����hbar title �����ַ���*/

__s32 dsk_hbar_set_volume(__u32 volume);	/* ����hbar ��ʾ���� */
__s32 dsk_hbar_set_bright(__u32 bright);	/* ����hbar ��ʾ���� */

__s32 dsk_hbar_set_scn_pos(__s16 x,  __s16 y);
__s32 dsk_hbar_get_scn_pos(__s16 *x, __s16 *y);



#endif /* __DSK_HEADBAR_H__ */


