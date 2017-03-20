/*
**************************************************************************************************************
*											         ePDK
*						            the Easy Portable/Player Develop Kits
*									           desktop system 
*
*						        	 (c) Copyright 2007-2010, ANDY, China
*											 All Rights Reserved
*
* File    	: dsk_orchid.h
* By      	: Andy.zhang
* Func		: orchid op
* Version	: v1.0
* ============================================================================================================
* 2010-1-8 9:46:40  andy.zhang  create this file, implements the fundemental interface;
**************************************************************************************************************
*/

#ifndef __DSK_ORCHID_H__
#define __DSK_ORCHID_H__

///* ���usb ֮����Ҫ���˽ӿڸ���ý��� */
//__s32 dsk_orchid_notify_update(void);

/* ÿ����������Ҫ���˽ӿ�, �ؽ�ý��� */
__s32 dsk_orchid_check(void);

/* ÿ����������Ҫ���˽ӿ�, ��ʽ�� z �� */
__s32 dsk_orchid_createDisk(void);

__s32 dsk_orchid_save_npl(void);

__s32 dsk_orchid_load_last_npl(void);

#endif /* __DSK_ORCHID_H__ */
