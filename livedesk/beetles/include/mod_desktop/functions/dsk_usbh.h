/*
************************************************************************************************************************
*                                                        usb host
*
*                                  Copyright(C), 2006-2010, AllWinner Technology Co., Ltd.
*											       All Rights Reserved
*
* File Name   : dsk_usbh.h
*
* Author      : Gary.Wang
*
* Version     : 1.0
*
* Date        : 2010.10.18
*
* Description :
* 
* Others      : None at present.
*
*
* History     :
*
*  <Author>        <time>       <version>      <description>
*
* Gary.Wang      2010.10.18        1.0         build the file
*
************************************************************************************************************************
*/
#ifndef  __DSK_USBH_H__
#define  __DSK_USBH_H__

#ifdef __cplusplus
extern "C" {
#endif


/*
****************************************************************************************************
*Name        : dsk_send_insmod_usbh_cmd
*Prototype   : __s32 dsk_send_insmod_usbh_cmd(__u32 hostno)
*Arguments   : void
*Return      : EPDK_OK     ���ͳɹ�
*              EPDK_FAIL   ����ʧ��
*Description : ��usbh_monitor���ͼ���host��������
*Other       : �������Ϻ�delay 2s ȷ�����سɹ�
****************************************************************************************************
*/
extern __s32 dsk_send_insmod_usbh_cmd(__u32 hostno);

/*
****************************************************************************************************
*Name        : dsk_send_rmmod_usbh_cmd
*Prototype   : __s32 dsk_send_rmmod_usbh_cmd(__u32 hostno)
*Arguments   : void
*Return      : EPDK_OK     ���ͳɹ�
*              EPDK_FAIL   ����ʧ��
*Description : ��usbh_monitor����ж��host��������
*Other       : �������Ϻ�delay 1s ȷ��ж�سɹ�
****************************************************************************************************
*/
extern __s32 dsk_send_rmmod_usbh_cmd(__u32 hostno);

/*
****************************************************************************************************
*Name        : dsk_usbh_is_working
*Prototype   : __bool dsk_usbh_is_working(void)
*Arguments   : void
*Return      : EPDK_TRUE     usb host is working
*              EPDK_FAIL     usb host is NOT working
*Description : check whether the usb host is working or not.
*Other       : 
****************************************************************************************************
*/
__bool dsk_usbh_is_working(__u32 hostno);



#ifdef __cplusplus
}
#endif

#endif     //  ifndef __DSK_USBH_H__

/* end of dsk_usbh.h */


