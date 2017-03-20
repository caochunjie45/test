/*
**************************************************************************************************************
*											         ePDK
*						            the Easy Portable/Player Develop Kits
*									           desktop system 
*
*						        	 (c) Copyright 2007-2010, ANDY, China
*											 All Rights Reserved
*
* File    	: gscene_backgrd.c
* By      	: Andy.zhang
* Func		: desktop toolbar control interface
* Version	: v1.0
* ============================================================================================================
* 2009-7-20 8:51:52  andy.zhang  create this file, implements the fundemental interface;
**************************************************************************************************************
*/

#ifndef __GSCENE_BACKGRD_H__
#define __GSCENE_BACKGRD_H__

#define BG_MAX_NUM          3

typedef enum
{
	BGD_STATUS_SHOW,
	BGD_STATUS_HIDE
}bgd_status_t;

/*�����ʾĬ�ϱ��������û����õı����ı��
 flag : EPDK_TRUE : Ĭ�ϱ���ͼƬ
 	  : EPDK_FALSE: ���ñ���ͼƬ
 */
__bool gscene_bgd_get_flag(void);


/*������ʾĬ�ϱ��������û����õı����ı��
 flag : EPDK_TRUE : Ĭ�ϱ���ͼƬ
 	  : EPDK_FALSE: ���ñ���ͼƬ
 */
void gscene_bgd_set_flag(__bool flag);


/*���Ĭ�ϱ���ͼƬ�ı�� index:0-BG_MAX_NUM-1
 */
__s32 gscene_bgd_get_default_bg_index(void);


/*����Ĭ�ϱ���ͼƬ�ı�� index:0-BG_MAX_NUM-1
 */
__s32 gscene_bgd_set_default_bg_index(__u32 index);




/***************************************************************************************************
*Name        : gscene_bgd_init
*Prototype   : __s32 gscene_bgd_init(SIZE *p_size, bgd_status_t status, __fb_type_t ftype);
*Arguments   : p_size	�������ߴ�
               status   : ��ʼ�������Ƿ���ʾ
               ftype    : ����ͼ���ʽ FB_TYPE_RGB�� FB_TYPE_YUV(��Լ����
*Return      : 
*Description : ��ʼ������ͼ��(mainӦ�ó�����ã�
*Other       :
***************************************************************************************************/
__s32 		 gscene_bgd_init(SIZE *p_size, bgd_status_t status, __fb_type_t ftype);

/***************************************************************************************************
*Name        : gscene_bgd_set_state
*Prototype   : void gscene_bgd_set_state(bgd_status_t status);
*Arguments   :            
*Return      : 
*Description : ���ñ���״̬
*Other       :
***************************************************************************************************/
void 		 gscene_bgd_set_state(bgd_status_t status);

/***************************************************************************************************
*Name        : gscene_bgd_get_state
*Prototype   : bgd_status_t gscene_bgd_get_state(void);
*Arguments   : 
*Return      : 
*Description : ��ѯ����״̬
*Other       :
***************************************************************************************************/
bgd_status_t gscene_bgd_get_state(void);

/***************************************************************************************************
*Name        : gscene_bgd_set_top
*Prototype   : void gscene_bgd_set_top(void);
*Arguments   : 
*Return      : 
*Description : ����ͼ�����ȼ��ö�
*Other       :
***************************************************************************************************/
void 		 gscene_bgd_set_top(void);

/***************************************************************************************************
*Name        : gscene_bgd_set_bottom
*Prototype   : void gscene_bgd_set_bottom(void);
*Arguments   : 
*Return      : 
*Description : ����ͼ�����ȼ��õ�
*Other       :
***************************************************************************************************/
void 		 gscene_bgd_set_bottom(void);

/***************************************************************************************************
*Name        : gscene_bgd_save_fb
*Prototype   : __s32 gscene_bgd_save_fb(FB *fb, char* path);
*Arguments   : 
*Return      : 
*Description : ���汳��ͼƬ
*Other       :
***************************************************************************************************/
__s32 		 gscene_bgd_save_fb(FB *fb, char* path);

/***************************************************************************************************
*Name        : gscene_bgd_set_fb_type
*Prototype   : __s32 gscene_set_load_arg(char *app_name, void *p_arg, __u32 size);
*Arguments   : 
*Return      : 
*Description : ���ñ���ͼ���ʽ
*Other       :
***************************************************************************************************/
void 		 gscene_bgd_set_fb_type(__fb_type_t ftype);

/***************************************************************************************************
*Name        : gscene_bgd_refresh
*Prototype   : void gscene_bgd_refresh(void);
*Arguments   : 
*Return      : 
*Description : ˢ�±���ͼ�㣬���»��Ʊ���ͼƬ
*Other       :
***************************************************************************************************/
void 		 gscene_bgd_refresh(void);

/***************************************************************************************************
*Name        : gscene_bgd_restore
*Prototype   : void gscene_bgd_refresh(void);
*Arguments   : 
*Return      : 
*Description : �ָ�Ĭ�ϱ���ͼ
*Other       :
***************************************************************************************************/
void 		 gscene_bgd_restore(void);
/***************************************************************************************************
*Name        : gscene_bgd_deinit
*Prototype   : __s32 gscene_bgd_deinit(void);
*Arguments   :
*Return      : 
*Description : main Ӧ�ó������
*Other       :
***************************************************************************************************/
__s32 		 gscene_bgd_deinit(void);


#endif /* __GSCENE_BACKGRD_H__ */

