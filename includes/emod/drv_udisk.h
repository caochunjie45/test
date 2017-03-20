/*
********************************************************************************************************************
*                                              usbd udisk
*
*                              (c) Copyright 2007-2009, javen.China
*										All	Rights Reserved
*
* File Name 	: drv_udisk.h
*
* Author 		: javen
*
* Version 		: 1.0
*
* Date 			: 2009.06.08
*
* Description 	: 
*
* History 		:
*
********************************************************************************************************************
*/

#ifndef    __DRV_UDISK_H__
#define    __DRV_UDISK_H__

#define    USBD_UDISK_CMD_IDLE          0x00
#define    USBD_UDISK_CMD_GET_STATUS    0x01

//u��������״̬
typedef enum tag_udisk_status{
    UDISK_STATUS_IDLE = 0,    //Ĭ��
	UDISK_STATUS_INSERT,      //����
	UDISK_STATUS_REMOVE,      //�γ�
    UDISK_STATUS_CONNECT,     //��������״̬
    UDISK_STATUS_READ,        //��
    UDISK_STATUS_WRITE        //д
}udisk_status_t;

/* u�̵���Ϣ */
typedef struct tag_udisk_info{
	udisk_status_t udisk_status;
}udisk_info_t;


#endif     //__DRV_UDISK_H__
