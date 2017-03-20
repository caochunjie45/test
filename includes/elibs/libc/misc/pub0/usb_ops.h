/*
*************************************************************************************
*                         		            Melis
*					                USB Host Driver Operation
*
*				        (c) Copyright 2006-2010, All winners Co,Ld.
*							       All Rights Reserved
*
* File Name 	: usb_ops.h
*
* Author 		: javen
*
* Description 	:
*
* History 		:
*      <author>    		<time>       	<version >    		<desc>
*       javen     	   2010-12-13          1.0          create this file
*
*************************************************************************************
*/
#ifndef  __USB_OPS_H__
#define  __USB_OPS_H__

#include ".\..\..\..\..\emod\drv_host.h"

//---------------------------------------------------------------
//  ���� / ж��  host / device ����
//---------------------------------------------------------------

/*
*********************************************************************
*                     App_InsmodUsbDeivceDriver
*
* Description:
*    �� usbh_monitor ���ͼ��� deivce ��������
*
* Arguments:
*    usbc_no   :  input. USB���������
*
* Returns:
*    EPDK_OK   :  ���ͳɹ�
*    EPDK_FAIL :  ����ʧ��
*
* note:
*    �������Ϻ�delay 2s ȷ�����سɹ�
*
*********************************************************************
*/
__s32 App_InsmodUsbDeivceDriver(__u32 usbc_no);

/*
*********************************************************************
*                     App_RmmodUsbDeivceDriver
*
* Description:
*    �� usbh_monitor ����ж�� device ��������
*
* Arguments:
*    usbc_no   :  input. USB���������
*
* Returns:
*    EPDK_OK   :  ���ͳɹ�
*    EPDK_FAIL :  ����ʧ��
*
* note:
*    �������Ϻ�delay 1s ȷ��ж�سɹ�
*
*********************************************************************
*/
__s32 App_RmmodUsbDeivceDriver(__u32 usbc_no);

/*
*********************************************************************
*                     App_InsmodUsbHostDriver
*
* Description:
*    ��usbh_monitor���ͼ���host��������
*
* Arguments:
*    usbc_no   :  input. USB���������
*
* Returns:
*    EPDK_OK   :  ���ͳɹ�
*    EPDK_FAIL :  ����ʧ��
*
* note:
*    �������Ϻ�delay 2s ȷ�����سɹ�
*
*********************************************************************
*/
__s32 App_InsmodUsbHostDriver(__u32 usbc_no);

/*
*********************************************************************
*                     App_RmmodUsbHostDriver
*
* Description:
*    ��usbh_monitor����ж��host��������
*
* Arguments:
*    usbc_no   :  input. USB���������
*
* Returns:
*    EPDK_OK   :  ���ͳɹ�
*    EPDK_FAIL :  ����ʧ��
*
* note:
*    �������Ϻ�delay 1s ȷ��ж�سɹ�
*
*********************************************************************
*/
__s32 App_RmmodUsbHostDriver(__u32 usbc_no);

/*
*******************************************************************************
*                     App_IsUsbHostReomveComplete
*
* Description:
*    ��ѯ usb host �����Ƿ�ж�����
*
* Parameters:
*    usbc_no   :  input. USB���������
*
* Return value:
*    0  : û����ȫж��
*    !0 : �Ѿ���ȫж����
*
* note:
*    ���豸���� : ж��ʱ��С�� 1s
*    �ƶ�Ӳ��   : ж��ʱ����� 10s
*    ����       : ж��ʱ����� 10s
*    U��        : ж��ʱ��С�� 1s
*    ������     : ж��ʱ��С�� 1s
*
*******************************************************************************
*/
__u32 App_IsUsbHostReomveComplete(__u32 usbc_no);

/*
*******************************************************************************
*                     App_WaitForUsbHostReomveComplete
*
* Description:
*    �ȴ� USB HOST ����ж����ϡ���Ϊ�����ƶ�Ӳ�̲��������£�host����ж��
* ��Ҫ�� 10s ��ʱ�䣬���ж�ص�ʱ����� 10s ��
*
* Parameters:
*    usbc_no   :  input. USB���������
*
* Return value:
*    EPDK_OK    :  �ɹ�
*    EPDK_FAIL  :  ʧ��
*
* note:
*    ������ڹػ��������ж�� host ������������ delay �ڼ�ȥ�ر� LCD������
* ���� host ����ж�ع��ã���ɹػ�ʱ��̫����
*
*******************************************************************************
*/
__s32 App_WaitForUsbHostReomveComplete(__u32 usbc_no);

//---------------------------------------------------------------
//  USB ��������
//---------------------------------------------------------------

/*
*******************************************************************************
*                     USB_GetCDROMInfo
*
* Description:
*    ��ù�������Ϣ
*
* Parameters:
*    Path  :  input. �豸��·��. ��"b:\\DISK\\USB_CDROM_xx"��
*    Info  :  output. ��������Ϣ
*
* Return value:
*    ���سɹ�����ʧ�ܡ�
*
* note:
*    ��
*
*******************************************************************************
*/
__s32 USB_GetCDROMInfo(char *Path, usbh_CDROM_Info_t *Info);

/*
*******************************************************************************
*                     USB_GetDriveStatus
*
* Description:
*    ��ù�����״̬
*
* Parameters:
*    Path  :  input. �豸��·��. ��"b:\\DISK\\USB_CDROM_xx"��
*
* Return value:
*    ���ع�����״̬
*
* note:
*    ��
*
*******************************************************************************
*/
__u32 USB_GetDriveStatus(char *Path);

/*
*******************************************************************************
*                     USB_CDROM_Eject
*
* Description:
*    �������
*
* Parameters:
*    Path  :  input. �豸��·��. ��"b:\\DISK\\USB_CDROM_xx"��
*
* Return value:
*    ���سɹ�����ʧ�ܡ�
*
* note:
*    ��
*
*******************************************************************************
*/
__s32 USB_CDEject(char *Path);

/*
*******************************************************************************
*                     USB_CDROM_Load
*
* Description:
*    װ�ع��
*
* Parameters:
*    Path  :  input. �豸��·��. ��"b:\\DISK\\USB_CDROM_xx"��
*
* Return value:
*    ���سɹ�����ʧ�ܡ�
*
* note:
*    ��
*
*******************************************************************************
*/
__s32 USB_CDLoad(char *Path);

/*
*******************************************************************************
*                     USB_PreventMediumRemoval
*
* Description:
*    ��ֹ�����ƶ�������ֹ���������
*
* Parameters:
*    Path  :  input. �豸��·��. ��"b:\\DISK\\USB_CDROM_xx"��
*
* Return value:
*    ���سɹ�����ʧ�ܡ�
*
* note:
*    ������ڲ�����Ƶ�����ֵȳ��ϣ���ֹ�û��ڴ�ʱͨ���������ذѹ��������
*
*******************************************************************************
*/
__s32 USB_PreventMediumRemoval(char *Path);

/*
*******************************************************************************
*                     USB_AllowMediumRemoval
*
* Description:
*    ��������ƶ�
*
* Parameters:
*    Path  :  input. �豸��·��. ��"b:\\DISK\\USB_CDROM_xx"��
*
* Return value:
*    ���سɹ�����ʧ�ܡ�
*
* note:
*    ��
*
*******************************************************************************
*/
__s32 USB_AllowMediumRemoval(char *Path);

#endif   //__USB_OPS_H__
