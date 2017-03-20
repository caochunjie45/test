/*
********************************************************************************************************************
*                         		            Melis
*					                USB Host Driver Operation
*
*				        (c) Copyright 2006-2010, All winners Co,Ld.
*							        All Rights Reserved
*
* File Name 	: usb_cd.c
*
* Author 		: javen
*
* Description 	: USB��������ز���
*
* History 		:
*      <author>    		<time>       	<version >    		<desc>
*       javen     	   2010-12-3          1.0          create this file
*
* Note			: 
*  1��USB��������������ο�"..\ePDK\includes\emod\drv_host.h"
*  2��USB�����й�������Ѿ�ע����豸ʱ����ͨ��"b:\\DISK\\USB_CDROM_xx"�ҵ�USB�����豸
*  3��USB����û�й��ʱ����ͨ��"b:\\USERDEF\\USB_CDROM_xx"�ҵ�USB�����豸
*  4��"USB_CDROM_xx"��"xx"��ʾ��ţ���������USB�������ڣ���ô����λ"USB_CDROM_x00"��"USB_CDROM_01"
*
********************************************************************************************************************
*/

#include  "epdk.h"

/*
*******************************************************************************
*                     USB_GetCDROMInfo
*
* Description:
*    ��ù�������Ϣ
*
* Parameters:
*    Path  :  input. �豸��·��. ��"b:\\DISK\\USB_CDROM_xx"��
*    Info  :  input. ��������Ϣ
*
* Return value:
*    ���سɹ�����ʧ�ܡ�
*
* note:
*    ��
*
*******************************************************************************
*/
__s32 USB_GetCDROMInfo(char *Path, usbh_CDROM_Info_t *Info)
{
    ES_FILE * pfile = NULL;
	__s32 ret = 0;

	pfile = eLIBs_fopen(Path, "r+");
	if(pfile == NULL){
		__wrn("ERR: fopen %s failed\n", Path);
		return EPDK_FAIL;
	}

	ret = eLIBs_fioctrl(pfile, 
		          		USB_STORAGE_USER_CTRL_CMD_GET_CDROM_INFO, 
		          		0, 
		          		Info);
	if(ret != EPDK_OK){
		__wrn("ERR: execute USB_STORAGE_USER_CTRL_CMD_GET_CDROM_INFO failed\n");
		eLIBs_fclose(pfile);
		return EPDK_FAIL;
	}

	eLIBs_fclose(pfile);

    return EPDK_OK;
}

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
__u32 USB_GetDriveStatus(char *Path)
{
    ES_FILE * pfile = NULL;
	__s32 ret = 0;
	__u32 Status = 0;

	pfile = eLIBs_fopen(Path, "r+");
	if(pfile == NULL){
		__wrn("ERR: fopen %s failed\n", Path);
		return USBH_CDROM_DRIVE_STATUS_NO_INFORMATION;
	}

	ret = eLIBs_fioctrl(pfile, 
		          		USB_STORAGE_USER_CTRL_CMD_CDROM_GET_DRIVE_STATUS, 
		          		0, 
		          		&Status);
	if(ret != EPDK_OK){
		__wrn("ERR: execute USB_STORAGE_USER_CTRL_CMD_CDROM_GET_DRIVE_STATUS failed\n");
		eLIBs_fclose(pfile);
		return USBH_CDROM_DRIVE_STATUS_NO_INFORMATION;
	}

	eLIBs_fclose(pfile);

    return Status;
}

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
__s32 USB_CDEject(char *Path)
{
    ES_FILE * pfile = NULL;
	__s32 ret = 0;

	pfile = eLIBs_fopen(Path, "r+");
	if(pfile == NULL){
		__wrn("ERR: fopen %s failed\n", Path);
		return EPDK_FAIL;
	}

	ret = eLIBs_fioctrl(pfile, 
		          		USB_STORAGE_USER_CTRL_CMD_CDROM_EJECT, 
		          		0, 
		          		NULL);
	if(ret != EPDK_OK){
		__wrn("ERR: execute USB_STORAGE_USER_CTRL_CMD_CDROM_EJECT failed\n");
		eLIBs_fclose(pfile);
		return EPDK_FAIL;
	}

	eLIBs_fclose(pfile);

	return EPDK_OK;
}

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
__s32 USB_CDLoad(char *Path)
{
    ES_FILE * pfile = NULL;
	__s32 ret = 0;

	pfile = eLIBs_fopen(Path, "r+");
	if(pfile == NULL){
		__wrn("ERR: fopen %s failed\n", Path);
		return EPDK_FAIL;
	}

	ret = eLIBs_fioctrl(pfile, 
		          		USB_STORAGE_USER_CTRL_CMD_CDROM_LOAD, 
		          		0, 
		          		NULL);
	if(ret != EPDK_OK){
		__wrn("ERR: execute USB_STORAGE_USER_CTRL_CMD_CDROM_LOAD failed\n");
		eLIBs_fclose(pfile);
		return EPDK_FAIL;
	}

	eLIBs_fclose(pfile);

	return EPDK_OK;
}

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
__s32 USB_PreventMediumRemoval(char *Path)
{
    ES_FILE * pfile = NULL;
	__s32 ret = 0;

	pfile = eLIBs_fopen(Path, "r+");
	if(pfile == NULL){
		__wrn("ERR: fopen %s failed\n", Path);
		return EPDK_FAIL;
	}

	ret = eLIBs_fioctrl(pfile, 
		          		USB_STORAGE_USER_CTRL_CMD_CDROM_PREVENT_MEDIUM_REMOVAL, 
		          		0, 
		          		NULL);
	if(ret != EPDK_OK){
		__wrn("ERR: execute USB_STORAGE_USER_CTRL_CMD_CDROM_PREVENT_MEDIUM_REMOVAL failed\n");
		eLIBs_fclose(pfile);
		return EPDK_FAIL;
	}

	eLIBs_fclose(pfile);

	return EPDK_OK;
}

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
__s32 USB_AllowMediumRemoval(char *Path)
{
    ES_FILE * pfile = NULL;
	__s32 ret = 0;

	pfile = eLIBs_fopen(Path, "r+");
	if(pfile == NULL){
		__wrn("ERR: fopen %s failed\n", Path);
		return EPDK_FAIL;
	}

	ret = eLIBs_fioctrl(pfile, 
		          		USB_STORAGE_USER_CTRL_CMD_CDROM_ALLOW_MEDIUM_REMOVAL, 
		          		0, 
		          		NULL);
	if(ret != EPDK_OK){
		__wrn("ERR: execute USB_STORAGE_USER_CTRL_CMD_CDROM_ALLOW_MEDIUM_REMOVAL failed\n");
		eLIBs_fclose(pfile);
		return EPDK_FAIL;
	}

	eLIBs_fclose(pfile);

	return EPDK_OK;
}





