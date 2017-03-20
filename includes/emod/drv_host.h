/*
********************************************************************************************************************
*                                              usb host driver
*
*                              (c) Copyright 2007-2010, javen.China
*										All	Rights Reserved
*
* File Name 	: drv_host.h
*
* Author 		: javen
*
* Version 		: 2.0
*
* Date 			: 2009.07.17
*
* Description 	: 
*
* History 		:
*
* 
********************************************************************************************************************
*/

#ifndef    __DRV_HOST_H__
#define    __DRV_HOST_H__

//---------------------------------------------------------
//    Device Name
//---------------------------------------------------------
#define  USB_DEVICE_INFO_NAME		"USB_DEV_INFO"	/* USB�豸��Ϣ 					*/
#define  USB_STORAGE_DEV_NAME       "SCSI_DISK"		/* USB �洢���豸�� 			*/
#define	 USB_CDROM_DEV_NAME			"USB_CDROM" 	/* USB �����豸 				*/

//---------------------------------------------------------
//     USB HOST IOCTRL COMMAND MASK
//---------------------------------------------------------
#define  USB_USER_CTRL_CMD_MASK		0x0f0000

#define  USB_USER_CTRL_CMD_DEVICE_INFO_MASK	(USB_USER_CTRL_CMD_MASK | 0x1000)
#define  USB_USER_CTRL_CMD_STORAGE_MASK		(USB_USER_CTRL_CMD_MASK | 0x2000)
#define  USB_USER_CTRL_CMD_HID_MASK			(USB_USER_CTRL_CMD_MASK | 0x3000)

//---------------------------------------------------------
//     USB �洢���豸��չ�Ŀ����豸
//---------------------------------------------------------
#define	USB_DEVICE_INFO_USER_CMD_GET_DISK_STATUS	(USB_USER_CTRL_CMD_DEVICE_INFO_MASK | 0x00)	/* ��ȡ��ǰ����lun�Ķ�д״̬ 		*/
#define	USB_DEVICE_INFO_USER_CMD_GET_DEVICE_INFO	(USB_USER_CTRL_CMD_DEVICE_INFO_MASK | 0x01)	/* ����豸����Ϣ					*/

#define	USB_STORAGE_DEVICE_STATUS_IDLE			0x00 /* USB���ڿ���״̬ 	*/
#define	USB_STORAGE_DEVICE_STATUS_READ			0x01 /* USB���ڶ�״̬ 	*/
#define	USB_STORAGE_DEVICE_STATUS_WRITE			0x02 /* USB����д״̬ 	*/

/* �豸���͡����ƶ�Ӳ�̡�U�̡������� */
typedef  enum _USBStorDevType{
    USB_STORAGE_DEVICE_TYPE_UNKOWN = 0,     /* δ֪�豸     */
    USB_STORAGE_DEVICE_TYPE_HDD,            /* �ƶ�����     */
    USB_STORAGE_DEVICE_TYPE_UDISK,          /* U�̡�������  */
    USB_STORAGE_DEVICE_TYPE_CDROM           /* USB����      */
}USBStorDevType_t;

#define  USB_HOST_MAX_STRING_LEN		128
typedef struct _usbhDeivceInfo{
	char Vender[USB_HOST_MAX_STRING_LEN];	/* �豸������ 	*/
	char Product[USB_HOST_MAX_STRING_LEN];	/* �豸��Ʒ�� 	*/
	char Serial[USB_HOST_MAX_STRING_LEN];	/* ���к� 		*/

	__s32 HubPortNo;		/* �豸���ڵ�Hub�˿ں�. 0~7: Hub�˿ںţ�8~15: Hub������16~31: ������
	                           Hub����Ϊ0����ʾû��hub���� */
	__u32 DeviceType;       /* �豸���͡��ο�USBStorDevType_t */
	__u8 res[508];
}usbhDeivceInfo_t;

//---------------------------------------------------------
//     USB HOST STORAGE Class Driver IOCTRL COMMAND
//---------------------------------------------------------
#define	USB_STORAGE_USER_CTRL_CMD_GET_CDROM_INFO	(USB_USER_CTRL_CMD_STORAGE_MASK | 0x01)	/* ��ù���״̬ 					*/
#define USB_STORAGE_USER_CTRL_CMD_CDROM_PALY		(USB_USER_CTRL_CMD_STORAGE_MASK | 0x02)	/* �ù�����ָ����MSFλ�ÿ�ʼ����	*/
#define USB_STORAGE_USER_CTRL_CMD_CDROM_STOP		(USB_USER_CTRL_CMD_STORAGE_MASK | 0x03)	/* ֹͣ���� 						*/
#define USB_STORAGE_USER_CTRL_CMD_CDROM_EJECT		(USB_USER_CTRL_CMD_STORAGE_MASK | 0x04)	/* ��������							*/
#define USB_STORAGE_USER_CTRL_CMD_CDROM_LOAD		(USB_USER_CTRL_CMD_STORAGE_MASK | 0x05)	/* װ�ع�������� 					*/
#define USB_STORAGE_USER_CTRL_CMD_CDROM_PREVENT_MEDIUM_REMOVAL	(USB_USER_CTRL_CMD_STORAGE_MASK | 0x06)	/* ��ֹ�����ƶ�			*/
#define USB_STORAGE_USER_CTRL_CMD_CDROM_ALLOW_MEDIUM_REMOVAL	(USB_USER_CTRL_CMD_STORAGE_MASK | 0x07)	/* ��������ƶ� 		*/
#define USB_STORAGE_USER_CTRL_CMD_CDROM_GET_DRIVE_STATUS		(USB_USER_CTRL_CMD_STORAGE_MASK | 0x08)	/* ��ù�����״̬ 		*/

/* ���������� */
#define  USBH_CDROM_DRIVER_TYPE_UNKOWN			0x00	/* δ֪ 				*/
#define  USBH_CDROM_DRIVER_TYPE_CDR_RD			0x01	/* CD-R Read Only 		*/
#define  USBH_CDROM_DRIVER_TYPE_CDR_WR			0x02	/* CD-R WR 				*/
#define  USBH_CDROM_DRIVER_TYPE_CDE_RD			0x04	/* CD-E Read Only 		*/
#define  USBH_CDROM_DRIVER_TYPE_CDE_WR			0x08	/* CD-E WR 				*/
#define  USBH_CDROM_DRIVER_TYPE_DVD_ROM_RD		0x10	/* DVD-ROM Read Only 	*/
#define  USBH_CDROM_DRIVER_TYPE_DVDR_RD			0x20	/* DVD-R Read Only 		*/
#define  USBH_CDROM_DRIVER_TYPE_DVDR_WR			0x40	/* DVD-R WR 			*/
#define  USBH_CDROM_DRIVER_TYPE_DVD_RAM_RD		0x80	/* DVD-RAM Read Only 	*/
#define  USBH_CDROM_DRIVER_TYPE_DVD_RAM_WR		0x100	/* DVD-RAM WR 			*/

/* ��������� */
#define  USBH_MEDIUM_CD_UNKOWN					0x00	/* δ֪ 				*/
#define  USBH_MEDIUM_CD_DATA_ONLY				0x01	/* ���ݹ�� 			*/
#define  USBH_MEDIUM_CD_AUDIO_ONLY				0x02	/* ���ֹ�� 			*/
#define  USBH_MEDIUM_CD_DATA_AUDIO_COMBINED		0x03	/* �������ֻ�Ϲ�� 	*/
#define  USBH_MEDIUM_CD_PHOTO_CD				0x04	/* ͼƬ��� 			*/

/* ������״̬ */
#define  USBH_CDROM_DRIVE_STATUS_NO_INFORMATION		0x00	/* ȱʡֵ��δ֪�Ĺ���״̬		*/
#define  USBH_CDROM_DRIVE_STATUS_NO_DISC			0x01	/* �����رգ�����û�й��		*/
#define  USBH_CDROM_DRIVE_STATUS_TRAY_OPEN			0x02	/* �����Ѿ��� 				*/
#define  USBH_CDROM_DRIVE_STATUS_NOT_READY			0x03	/* ����Ѿ�װ�أ����ǽ���δ����	*/
#define  USBH_CDROM_DRIVE_STATUS_DISK_OK			0x04	/* ����Ѿ�װ�أ����ҽ��ʾ���	*/

/* USB �����͹����Ϣ */
typedef struct _usbh_CDROM_Info{
	__u32 DriveType;	/* ����������,ÿ��bit���Լ��ĺ��� */
	__u32 DiscType;		/* ��������� */

	__u8 res[504];
}usbh_CDROM_Info_t;


//---------------------------------------------------------
//     USB HOST HID Class Driver IOCTRL COMMAND
//---------------------------------------------------------
typedef void (* USBHMouse_CallBack)(void *p_arg);

/* USB������ר�� */
#define  USBH_HID_USER_CTRL_CMD_TEST_START	(USB_USER_CTRL_CMD_HID_MASK | 0x00) /* USB����������. ��ʼ */
#define  USBH_HID_USER_CTRL_CMD_TEST_STOP	(USB_USER_CTRL_CMD_HID_MASK | 0x01) /* USB����������. ���� */

typedef struct _USBHMouseTest{
	void *buffer;
    USBHMouse_CallBack CallBack;
}USBHMouseTest_t;

/* �û�ר�� */
#define USBH_HID_USER_CTRL_CMD_REG          (USB_USER_CTRL_CMD_HID_MASK | 0x10) /* ��USBע��ص����� */
#define USBH_HID_USER_CTRL_CMD_UNREG        (USB_USER_CTRL_CMD_HID_MASK | 0x11) /* ��USBע���ص����� */
typedef struct _USBHMouseEvent{
    struct{
        __u8 LeftButton     : 1;		/* ������ */
        __u8 RightButton    : 1;		/* ����Ҽ� */
        __u8 MiddleButton   : 1;		/* ����м� */
        __u8 Button4        : 1;
        __u8 Button5        : 1;
        __u8 Button6        : 1;
        __u8 Button7        : 1;
        __u8 Button8        : 1;
    }Button;

    __s8 X;				/* X ����, �������	*/
	__s8 Y;				/* Y ����, �������	*/
	__s8 Wheel;			/* ������, >0��ʾ���Ϲ���, <0��ʾ���¹��� */

	__u8 res[4];
}USBHMouseEvent_t;

#endif     //__DRV_UDISK_H__


