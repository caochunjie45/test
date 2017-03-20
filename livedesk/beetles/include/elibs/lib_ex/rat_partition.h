#ifndef __RAT_PARTITION_H
#define __RAT_PARTITION_H

#define RAT_CD_ROM		"ExternalDisk2"
#define RAT_USB_DISK	"ExternalDisk1"
#define RAT_SD_CARD		"ExternalDisk0"
#define RAT_LOCAL_DISK  "LocalDisk"

#define RAT_MAX_PARTITION		11


/*!
* \brief
*     ��������
*/
typedef enum
{
    RAT_READONLY = 0x01,    //ֻ��
	RAT_READWRITE           //�ɶ�д
}rat_pati_attr_t;

/*
**********************************************************************************************************************
*                                               
*
* author:          
*
* date:             
*
* Description:       ����̷���Ϣ 
*
* parameters:       disk_type: RAT_SD_CARD RAT_USB_DISK RAT_LOCAL_DISK
					disk_name: ���ص��̷���
*
* return:           0 	get part success
					-1	part not exist
					-2	part not formatted
* modify history: 
**********************************************************************************************************************
*/
__s32 rat_get_partition_name(char* disk_type, char disk_name[RAT_MAX_PARTITION][4]);

#endif//__RAT_PARTITION_H
