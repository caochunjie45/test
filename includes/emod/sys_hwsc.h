/*
**********************************************************************************************************************
*											        ePOS
*								the Easy Portable/Player Operation System
*									           Krnl sub-system
*
*						        (c) Copyright 2006-2007, Steven.ZGJ China
*											All	Rights Reserved
*
* File   : sys_hwsc.h
* Version: V1.0
* By     : steven.zgj
**********************************************************************************************************************
*/
#ifndef _SYS_HWSC_H_
#define _SYS_HWSC_H_

#include "sys_dev.h"

/* hwsc - USB */
#define DEV_IOC_USR_HWSC_USBD_SUSPEND			(DEV_IOC_USR_BASE + 200)    /* ����                                         */
#define DEV_IOC_USR_HWSC_USBD_RESET			    (DEV_IOC_USR_BASE + 201)    /* ����                                         */
#define DEV_IOC_USR_HWSC_USBD_START			    (DEV_IOC_USR_BASE + 202)    /* ����                                         */

#define DEV_IOC_USR_HWSC_USBH_DISCONNECT		(DEV_IOC_USR_BASE + 208)    /* ����                                         */
#define DEV_IOC_USR_HWSC_USBH_CONNECT			(DEV_IOC_USR_BASE + 209)    /* ����                                         */
#define DEV_IOC_USR_HWSC_USBH_REMOVE			(DEV_IOC_USR_BASE + 210)    /* ����                                         */

#define DEV_IOC_USR_HWSC_APP_INSMOD_USBH		(DEV_IOC_USR_BASE + 216)    /* Ӧ�ó������ Deivce Driver                   */
#define DEV_IOC_USR_HWSC_APP_RMMOD_USBH			(DEV_IOC_USR_BASE + 217)    /* Ӧ�ó���ж�� Deivce Driver                   */
#define DEV_IOC_USR_HWSC_APP_INSMOD_USBD		(DEV_IOC_USR_BASE + 218)    /* Ӧ�ó������ Host Driver                     */
#define DEV_IOC_USR_HWSC_APP_RMMOD_USBD			(DEV_IOC_USR_BASE + 219)    /* Ӧ�ó���ж�� Host Driver                     */
#define DEV_IOC_USR_HWSC_APP_DRV_NULL			(DEV_IOC_USR_BASE + 220)    /* ����                                         */
#define DEV_IOC_USR_HWSC_TV_INSMOD_USBH			(DEV_IOC_USR_BASE + 221)    /* ����                                         */
#define DEV_IOC_USR_HWSC_TV_RMMOD_USBH			(DEV_IOC_USR_BASE + 222)    /* ����                                         */

#define DEV_IOC_USR_HWSC_GET_USB_INFO			(DEV_IOC_USR_BASE + 224)    /* ��õ�ǰUSB���������ͣ���deviceģʽ��Hostģʽ */
#define DEV_IOC_USR_HWSC_USBH_INSMOD_STATUS     (DEV_IOC_USR_BASE + 225)    /* Host Driver�Ƿ���أ�                        */

#define DEV_IOC_USR_HWSC_USBH_MSC_DEV_REG_SET   (DEV_IOC_USR_BASE + 233)    /* ����                                         */
#define DEV_IOC_USR_HWSC_USBH_MSC_DEV_REG_GET   (DEV_IOC_USR_BASE + 234)    /* ����                                         */
#define DEV_IOC_USR_START_USB_MONITOR           (DEV_IOC_USR_BASE + 235)    /* ϵͳ����������USB�Զ����                    */
#define DEV_IOC_USR_STOP_USB_MONITOR            (DEV_IOC_USR_BASE + 236)    /* ϵͳ�������ر�USB�Զ����                    */
#define DEV_IOC_USR_GET_USB_CHARGE_SOURCE       (DEV_IOC_USR_BASE + 237)    /* ���USB�������ͣ�PC���磬USB��ţ��磿       */

#define DEV_IOC_USR_HWSC_SET_USBH_WORK_STATUS   (DEV_IOC_USR_BASE + 238)    /* ����                                         */
#define DEV_IOC_USR_HWSC_GET_USBH_WORK_STATUS   (DEV_IOC_USR_BASE + 239)    /* ���USB Host driver�Ƿ��ڹ�����              */
#define DEV_IOC_USR_IS_NEED_APP_INSMOD_USBH     (DEV_IOC_USR_BASE + 240)    /* ����, �Ƿ���ҪӦ�ó������Host Driver?       */
#define DEV_IOC_USR_GET_CURRENT_PORT            (DEV_IOC_USR_BASE + 241)    /* ����, ���USB�Ķ˿ں�                        */

/* for suniii �� USB ������ 1 �� USB ������ 2 */
#define DEV_IOC_USR_HWSC_APP_INSMOD_USBH_1      (DEV_IOC_USR_BASE + 242)    /* Ӧ�ó������ usbc1 �� Host Driver            */
#define DEV_IOC_USR_HWSC_APP_RMMOD_USBH_1       (DEV_IOC_USR_BASE + 243)    /* Ӧ�ó���ж�� usbc1 �� Host Driver            */
#define DEV_IOC_USR_HWSC_APP_INSMOD_USBH_2      (DEV_IOC_USR_BASE + 244)    /* Ӧ�ó������ usbc2 �� Host Driver            */
#define DEV_IOC_USR_HWSC_APP_RMMOD_USBH_2       (DEV_IOC_USR_BASE + 245)    /* Ӧ�ó���ж�� usbc2 �� Host Driver            */
#define DEV_IOC_USR_HWSC_SET_USBH1_WORK_STATUS  (DEV_IOC_USR_BASE + 246)    /* ���� usbc1 Host Driver �Ĺ���״̬            */
#define DEV_IOC_USR_HWSC_GET_USBH1_WORK_STATUS  (DEV_IOC_USR_BASE + 247)    /* ��ѯ usbc1 Host Driver �Ĺ���״̬            */
#define DEV_IOC_USR_HWSC_SET_USBH2_WORK_STATUS  (DEV_IOC_USR_BASE + 248)    /* ���� usbc2 Host Driver �Ĺ���״̬            */
#define DEV_IOC_USR_HWSC_GET_USBH2_WORK_STATUS  (DEV_IOC_USR_BASE + 249)    /* ��ѯ usbc2 Host Driver �Ĺ���״̬            */

//define command to enable or disable device monitor
#define DEV_IOC_USR_HWSC_ENABLE_MONITOR         (DEV_IOC_USR_BASE + 250)
#define DEV_IOC_USR_HWSC_DISABLE_MONITOR        (DEV_IOC_USR_BASE + 251)

//����usbd��ģʽ:Ŀǰ������ģʽudisk or uvc
#define DEV_IOC_USR_SET_USBD_MODE            	(DEV_IOC_USR_BASE + 252)
#define DEV_IOC_USR_ADD_USBD_MODE            	(DEV_IOC_USR_BASE + 253)
#define DEV_IOC_USR_RMMOD_USBD_DEV            	(DEV_IOC_USR_BASE + 254)
#define DEV_IOC_USR_ENABLE_SD_MMC_CHECK			(DEV_IOC_USR_BASE + 255)


//-----------------------------------------------
//   sd add by weiziheng 2011-5-4 14:00
//-----------------------------------------------
#define DEV_IOC_USR_HWSC_SD_DRV_INSMOD_OK       (DEV_IOC_USR_BASE + 500)
#define DEV_IOC_USR_HWSC_SD_DRV_RMMOD_OK        (DEV_IOC_USR_BASE + 501)

#define DEV_IOC_USR_INIT_HOST_CONTROLER			(DEV_IOC_USR_BASE + 502)
#define DEV_IOC_USR_DEINIT_HOST_CONTROLER		(DEV_IOC_USR_BASE + 503)


/* usbd ���ֹ���ģʽ��Ŀǰ����:udisk or uvc or charge*/
typedef enum usbd_mode{
    USBD_UDISK = 0,
	USBD_UVC,
	USBD_CHARGE,
}usbm_usbd_mode_t;


typedef struct tag_usbm_ioctrl_para{
	__u32  usb_drv;                 /* ��ǰUSB����������                    */
    __u32  usbh_inmod_info;         /* usb host�����ļ������               */
    __u32  usbh_msc_dev_reg;        /* usb host�Ƿ����豸����ע��           */
    __u32  usb_cattle;              /* usb�����Դ, 0: usb��; 1: usb��ţ    */
}usbm_ioctrl_para_t;

/* usb host�����ļ������               */
typedef enum usbm_usbh_insmod{
    USBH_INSMOD_FAIL = 0,
	USBH_INSMOD_BUSY,
	USBH_INSMOD_SUCCESS
}usbm_usbh_insmod_t;


typedef struct __nand_hwsc_info
{
	__u8 id[8];
	__u8 chip_cnt;
	__u8 chip_connect;
	__u8 rb_cnt;
	__u8 rb_connect;
	__u32 good_block_ratio;
}__nand_hwsc_info_t;

#endif //#ifndef _SYS_HWSC_H_
