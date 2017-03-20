/*
**************************************************************************************************************
*											         ePDK
*						            the Easy Portable/Player Develop Kits
*									           desktop system
*
*						        	 (c) Copyright 2007-2010, ANDY, China
*											 All Rights Reserved
*
* File    	: dsk_app.h
* By      	: Andy.zhang
* Func		:
* Version	: v1.0
* ============================================================================================================
* 2009-7-8 10:02:17  andy.zhang  create this file, implements the fundemental interface;
**************************************************************************************************************
*/

#ifndef __DSK_APP_H__
#define __DSK_APP_H__

/***************************************************************************************************
*Name        : dsk_load_app
*Prototype   : __s32 dsk_load_app( char *file_name)
*Arguments   : file_name : ����Ӧ�ó������ file_name �ļ���
*Return      : �ɹ� ��EPDK_OK
               ʧ�� : EPDK_FAIL
*Description : �ļ��ͼ���Ӧ�ó���󶨣��û������޸� desktop ע������ �ļ���Ӧ�ó���İ󶨹�ϵ;
               eg : aaa.mp3 �ļ���ϵͳ��Ĭ������ music.app Ӧ�ó���
               ע�� ��1. ���Ӧ�ó�����δ���У���ֱ�Ӽ���Ӧ�ó���; ���Ӧ�ó����Ѿ����У���ᷢ���첽��Ϣ DSK_APP_RESTORE;
                      2. ��������ļ���Ϊ "xxxx.app" ��ϵͳ������û���ֱ������"xxxx.app��Ӧ�ó������гɹ���ǰ���� "xxxx.app��
                         �Ѿ�������ϵͳע�����ע�ᡣ
               
*Other       :
***************************************************************************************************/
__s32 dsk_load_app( char *file_name);


/***************************************************************************************************
*Name        : dsk_kill_app
*Prototype   : __s32 dsk_kill_app( char *file_name)
*Arguments   : file_name : ��ɱ��Ӧ�ó������ƣ���ɱ��������Ӧ�ó���Ϊ  dsk_kill_app("ebook.app");
*Return      : �ɹ� ��EPDK_OK
               ʧ�� : EPDK_FAIL
*Description : file_name Ӧ�ó��������Ӧ DSK_MSG_KILL ������Ϣ;
*Other       :
***************************************************************************************************/
__s32 dsk_kill_app( char *file_name);

/***************************************************************************************************
*Name        : dsk_app_exist
*Prototype   : __bool dsk_app_exist(char *file_name);
*Arguments   : file_name : Ӧ�ó�������
*Return      :  
*Description : ��ѯӦ�ó����Ƿ���� 
*Other       :
***************************************************************************************************/
__bool dsk_app_exist(char *file_name);

/***************************************************************************************************
*Name        : dsk_set_load_arg
*Prototype   : __s32 dsk_set_load_arg(char *app_name, void *p_arg, __u32 size);
*Arguments   : app_name ��Ӧ�ó�������
               p_arg    : ���ݲ�����ַ
               size     : ���ݲ�����С
*Return      : 
*Description : ����Ӧ�ó�����ز�����ע�⣬Ŀǰ���ݲ������ܶ༶ָ�봫��
*Other       :
***************************************************************************************************/
__s32 dsk_set_load_arg(char *app_name, void *p_arg, __u32 size);

/***************************************************************************************************
*Name        : dsk_set_load_arg
*Prototype   : __s32 dsk_set_load_arg(char *app_name, void *p_arg, __u32 size);
*Arguments   : app_name ��Ӧ�ó�������
               p_arg    : ���ݲ�����ַ
               size     : ���ݲ�����С
*Return      : 
*Description : ��ѯӦ�ó�����ز���
*Other       :
***************************************************************************************************/
__s32 dsk_get_load_arg(char *app_name, void *p_arg, __u32 size);

/***************************************************************************************************
*Name        : dsk_set_load_args
*Prototype   : __s32 dsk_set_load_args(char *app_name, char *src_name, void *p_arg, __u32 size)
*Arguments   : app_name ��Ӧ�ó�������
			   src_name : ԴӦ�ó�������
               p_arg    : ���ݲ�����ַ
               size     : ���ݲ�����С
*Return      : 
*Description : ��ѯӦ�ó�����ز���
*Other       :
***************************************************************************************************/
__s32 dsk_set_load_args(char *app_name, char *src_name, void *p_arg, __u32 size);

/***************************************************************************************************
*Name        : dsk_set_load_args
*Prototype   : __s32 dsk_set_load_args(char *app_name, char *src_name, void *p_arg, __u32 size);
*Arguments   : app_name ��Ӧ�ó�������
			   src_name : ԴӦ�ó�������
               p_arg    : ���ݲ�����ַ
               size     : ���ݲ�����С
*Return      : 
*Description : ��ѯӦ�ó�����ز���
*Other       :
***************************************************************************************************/
__s32 dsk_get_load_args(char *app_name, char *src_name, void *p_arg, __u32 size);


/* �ص������� */
__s32 dsk_notify_app_home(void);

/* ������Ӧ�ó���(��������ϵͳ)�㲥��Ϣ */
void dsk_broadcast_msg(__gui_msg_t *msg);

/* ������Ӧ�ó���㲥��Ϣ */
void dsk_broadcast_app_msg(__gui_msg_t *msg);

__s32 dsk_set_standby_enable(char *app_name, __bool enable);

#endif /* __DSK_APP_H__ */

