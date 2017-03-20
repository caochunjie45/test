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

#ifndef __DSK_BACKGRD_H__
#define __DSK_BACKGRD_H__

typedef enum
{
	BGD_STATUS_SHOW,
	BGD_STATUS_HIDE
}bgd_status_t;

/***************************************************************************************************
*Name        : dsk_bgd_init
*Prototype   : __s32 dsk_bgd_init(SIZE *p_size, bgd_status_t status, __fb_type_t ftype);
*Arguments   : p_size	�������ߴ�
               status   : ��ʼ�������Ƿ���ʾ
               ftype    : ����ͼ���ʽ FB_TYPE_RGB�� FB_TYPE_YUV(��Լ����
*Return      : 
*Description : ��ʼ������ͼ��(mainӦ�ó�����ã�
*Other       :
***************************************************************************************************/
__s32 		 dsk_bgd_init(SIZE *p_size, bgd_status_t status, __fb_type_t ftype);

/***************************************************************************************************
*Name        : dsk_bgd_set_state
*Prototype   : void dsk_bgd_set_state(bgd_status_t status);
*Arguments   :            
*Return      : 
*Description : ���ñ���״̬
*Other       :
***************************************************************************************************/
void 		 dsk_bgd_set_state(bgd_status_t status);

/***************************************************************************************************
*Name        : dsk_bgd_get_state
*Prototype   : bgd_status_t dsk_bgd_get_state(void);
*Arguments   : 
*Return      : 
*Description : ��ѯ����״̬
*Other       :
***************************************************************************************************/
bgd_status_t dsk_bgd_get_state(void);

/***************************************************************************************************
*Name        : dsk_bgd_set_top
*Prototype   : void dsk_bgd_set_top(void);
*Arguments   : 
*Return      : 
*Description : ����ͼ�����ȼ��ö�
*Other       :
***************************************************************************************************/
void 		 dsk_bgd_set_top(void);

/***************************************************************************************************
*Name        : dsk_bgd_set_bottom
*Prototype   : void dsk_bgd_set_bottom(void);
*Arguments   : 
*Return      : 
*Description : ����ͼ�����ȼ��õ�
*Other       :
***************************************************************************************************/
void 		 dsk_bgd_set_bottom(void);

/***************************************************************************************************
*Name        : dsk_bgd_save_fb
*Prototype   : void dsk_bgd_save_fb(FB *fb);
*Arguments   : 
*Return      : 
*Description : ���汳��ͼƬ
*Other       :
***************************************************************************************************/
void 		 dsk_bgd_save_fb(FB *fb);

/***************************************************************************************************
*Name        : dsk_bgd_set_fb_type
*Prototype   : __s32 dsk_set_load_arg(char *app_name, void *p_arg, __u32 size);
*Arguments   : 
*Return      : 
*Description : ���ñ���ͼ���ʽ
*Other       :
***************************************************************************************************/
void 		 dsk_bgd_set_fb_type(__fb_type_t ftype);

/***************************************************************************************************
*Name        : dsk_bgd_refresh
*Prototype   : void dsk_bgd_refresh(void);
*Arguments   : 
*Return      : 
*Description : ˢ�±���ͼ�㣬���»��Ʊ���ͼƬ
*Other       :
***************************************************************************************************/
void 		 dsk_bgd_refresh(void);

/***************************************************************************************************
*Name        : dsk_bgd_restore
*Prototype   : void dsk_bgd_refresh(void);
*Arguments   : 
*Return      : 
*Description : �ָ�Ĭ�ϱ���ͼ
*Other       :
***************************************************************************************************/
void 		 dsk_bgd_restore(void);
/***************************************************************************************************
*Name        : dsk_bgd_deinit
*Prototype   : __s32 dsk_bgd_deinit(void);
*Arguments   :
*Return      : 
*Description : main Ӧ�ó������
*Other       :
***************************************************************************************************/
__s32 		 dsk_bgd_deinit(void);


#endif /* __DSK_BACKGRD_H__ */

