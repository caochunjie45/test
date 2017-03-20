/*
************************************************************************************************************************
*                                                        usb host
*
*                                  Copyright(C), 2006-2010, AllWinner Technology Co., Ltd.
*											       All Rights Reserved
*
* File Name   : dsk_usbh.c
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
#ifndef  __DSK_USBH_C__
#define  __DSK_USBH_C__


#include "..\\functions_i.h"


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
__s32 dsk_send_insmod_usbh_cmd(__u32 hostno)
{
    ES_FILE *usbm_dev_hadle = NULL;
    __s32 ret = 0;

	__msg("%s %d :send_insmod_usbh_cmd \n", __FUNCTION__, __LINE__);
    /* �� hwsc �豸 */
    usbm_dev_hadle = eLIBs_fopen("b:\\HWSC\\hwsc", "r");
    if(usbm_dev_hadle == NULL){
        __wrn("ERR: send_insmod_usbh_cmd: eLIBs_fopen failed\n");
        return EPDK_FAIL;
    }
    
    /* ���ͼ���host���� */
	if(0 == hostno)
	{
		ret = eLIBs_fioctrl(usbm_dev_hadle, DEV_IOC_USR_HWSC_APP_INSMOD_USBH, 0, NULL);
	}
	else if(1 == hostno)
	{
		ret = eLIBs_fioctrl(usbm_dev_hadle, DEV_IOC_USR_HWSC_APP_INSMOD_USBH_1, 0, NULL);
	}
	else if(2 == hostno)
	{
		ret = eLIBs_fioctrl(usbm_dev_hadle, DEV_IOC_USR_HWSC_APP_INSMOD_USBH_2, 0, NULL);
	}	
		
    
    if(ret == EPDK_FAIL){
        __msg("ERR: send_insmod_usbh_cmd: execute DEV_IOC_USR_HWSC_APP_INSMOD_USBH failed\n");
        
        eLIBs_fclose(usbm_dev_hadle);
        usbm_dev_hadle = NULL;
        
        return EPDK_FAIL;
    }
    
    /* �ر� hwsc �豸 */
    eLIBs_fclose(usbm_dev_hadle);
    usbm_dev_hadle = NULL;

	/*wait to install host driver less than 2 second*/
	esKRNL_TimeDly(400);
    
    return EPDK_OK;
}



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
__s32 dsk_send_rmmod_usbh_cmd(__u32 hostno)
{
    ES_FILE *usbm_dev_hadle = NULL;
    __s32 ret = 0;

	__msg("%s %d :send_rmmod_usbh_cmd \n", __FUNCTION__, __LINE__);
    /* �� hwsc �豸 */
    usbm_dev_hadle = eLIBs_fopen("b:\\HWSC\\hwsc", "r");
    if(usbm_dev_hadle == NULL){
        __wrn("ERR: send_rmmod_usbh_cmd: eLIBs_fopen failed\n");
        return EPDK_FAIL;
    }

    /* ���ͼ���host���� */
	if(0 == hostno)
	{
		ret = eLIBs_fioctrl(usbm_dev_hadle, DEV_IOC_USR_HWSC_APP_RMMOD_USBH, 0, NULL);
	}
	else if(1 == hostno)
	{		
		ret = eLIBs_fioctrl(usbm_dev_hadle, DEV_IOC_USR_HWSC_APP_RMMOD_USBH_1, 0, NULL);
	}
	else if(2 == hostno)
	{		
		ret = eLIBs_fioctrl(usbm_dev_hadle, DEV_IOC_USR_HWSC_APP_RMMOD_USBH_2, 0, NULL);
	}	
	else
	{
		__wrn("arm just have 2 host , use default 0\n");
		ret = eLIBs_fioctrl(usbm_dev_hadle, DEV_IOC_USR_HWSC_APP_RMMOD_USBH, 0, NULL);
	}
		
    
    if(ret == EPDK_FAIL){
        __msg("ERR: send_rmmod_usbh_cmd: execute DEV_IOC_USR_HWSC_APP_RMMOD_USBH failed\n");
        
        eLIBs_fclose(usbm_dev_hadle);
        usbm_dev_hadle = NULL;
        
        return EPDK_FAIL;
    }
    
    /* �ر� hwsc �豸 */
    eLIBs_fclose(usbm_dev_hadle);
    usbm_dev_hadle = NULL;

	/* delay 1s ȷ��usb_monitor���㹻��ʱ��ж�� */
	esKRNL_TimeDly(200);
    
    return EPDK_OK;
}



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
__bool dsk_usbh_is_working(__u32 hostno)
{
     ES_FILE *usbm_dev_hadle = NULL;
     __s32 ret = 0;

    /* �� hwsc �豸 */
    usbm_dev_hadle = eLIBs_fopen("b:\\HWSC\\hwsc", "r");
    if(usbm_dev_hadle == NULL){
        __wrn("ERR: AppUsb_IsNeedAppInsmodUsbh: eLIBs_fopen failed\n");
        return EPDK_FALSE;
    }

    /* ��ȡusb host�Ĺ���״̬ */
	if(0 == hostno)
	{
		ret = eLIBs_fioctrl(usbm_dev_hadle, DEV_IOC_USR_HWSC_GET_USBH_WORK_STATUS, 0, NULL);
	}
	else if(1 == hostno)
	{
		__msg("1 == hostno\n");
		ret = eLIBs_fioctrl(usbm_dev_hadle, DEV_IOC_USR_HWSC_GET_USBH1_WORK_STATUS, 0, NULL);
	}
	else if(2 == hostno)
	{
		ret = eLIBs_fioctrl(usbm_dev_hadle, DEV_IOC_USR_HWSC_GET_USBH2_WORK_STATUS, 0, NULL);
	}	
	else 
	{
		__wrn("use usb host 0 default\n");
		ret = eLIBs_fioctrl(usbm_dev_hadle, DEV_IOC_USR_HWSC_GET_USBH_WORK_STATUS, 0, NULL);	
	}
    
 
    /* �ر� hwsc �豸 */
    eLIBs_fclose(usbm_dev_hadle);
    usbm_dev_hadle = NULL;

    return ret;
}




#endif     //  ifndef __DSK_USBH_C__

/* end of dsk_usbh.c */
