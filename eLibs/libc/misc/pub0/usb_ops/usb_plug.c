/*
*************************************************************************************
*                         		            Melis
*					                USB Host Driver Operation
*
*				        (c) Copyright 2006-2010, All winners Co,Ld.
*							       All Rights Reserved
*
* File Name 	: usb_plug.c
*
* Author 		: javen
*
* Description 	: usb �����⡣
*
* Note          : ��������ɲο� "D:\winners\ePDK\includes\emod\sys_hwsc.h"
*                                "D:\winners\ePDK\includes\emod\drv_host.h"
*                                "D:\winners\ePDK\includes\emod\drv_udisk.h"
*
* History 		:
*      <author>    		<time>       	<version >    		<desc>
*       javen     	   2010-12-13          1.0          create this file
*
*************************************************************************************
*/

#include  "epdk.h"

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
__s32 App_InsmodUsbDeivceDriver(__u32 usbc_no)
{
    ES_FILE *usbm_dev_hadle = NULL;
    __s32 ret = 0;

	__inf("%s %d :App_InsmodUsbDeivceDriver usbc_no = %d\n", __FUNCTION__, __LINE__, usbc_no);

    /* �� hwsc �豸 */
    usbm_dev_hadle = eLIBs_fopen("b:\\HWSC\\hwsc", "r");
    if(usbm_dev_hadle == NULL){
        __wrn("ERR: eLIBs_fopen failed\n");
        return EPDK_FAIL;
    }

    /* ���ͼ���host���� */
    ret = eLIBs_fioctrl(usbm_dev_hadle, DEV_IOC_USR_HWSC_APP_INSMOD_USBD, 0, NULL);
    if(ret == EPDK_FAIL){
        __inf("ERR: execute DEV_IOC_USR_HWSC_APP_INSMOD_USBD failed\n");
        
        eLIBs_fclose(usbm_dev_hadle);
        usbm_dev_hadle = NULL;
        
        return EPDK_FAIL;
    }

    /* �ر� hwsc �豸 */
    eLIBs_fclose(usbm_dev_hadle);
    usbm_dev_hadle = NULL;

	/*wait to install host driver less than 2 second*/
//	esKRNL_TimeDly(200);

    return EPDK_OK;
}

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
__s32 App_RmmodUsbDeivceDriver(__u32 usbc_no)
{
    ES_FILE *usbm_dev_hadle = NULL;
    __s32 ret = 0;

	__inf("%s %d :App_RmmodUsbDeivceDriver usbc_no = %d\n", __FUNCTION__, __LINE__, usbc_no);

    /* �� hwsc �豸 */
    usbm_dev_hadle = eLIBs_fopen("b:\\HWSC\\hwsc", "r");
    if(usbm_dev_hadle == NULL){
        __wrn("ERR: eLIBs_fopen failed\n");
        return EPDK_FAIL;
    }

    /* ���ͼ���host���� */
    ret = eLIBs_fioctrl(usbm_dev_hadle, DEV_IOC_USR_HWSC_APP_RMMOD_USBD, 0, NULL);
    if(ret == EPDK_FAIL){
        __inf("ERR: execute DEV_IOC_USR_HWSC_APP_RMMOD_USBD failed\n");
        
        eLIBs_fclose(usbm_dev_hadle);
        usbm_dev_hadle = NULL;
        
        return EPDK_FAIL;
    }
    
    /* �ر� hwsc �豸 */
    eLIBs_fclose(usbm_dev_hadle);
    usbm_dev_hadle = NULL;

	/* delay 1s ȷ��usb_monitor���㹻��ʱ��ж�� */
//	esKRNL_TimeDly(200);
    
    return EPDK_OK;
}

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
__s32 App_InsmodUsbHostDriver(__u32 usbc_no)
{
    ES_FILE *usbm_dev_hadle = NULL;
    __s32 ret = 0;

	__inf("%s %d : App_InsmodUsbHostDriver usbc_no = %d\n", __FUNCTION__, __LINE__, usbc_no);

    /* �� hwsc �豸 */
    usbm_dev_hadle = eLIBs_fopen("b:\\HWSC\\hwsc", "r");
    if(usbm_dev_hadle == NULL){
        __wrn("ERR: eLIBs_fopen failed\n");
        return EPDK_FAIL;
    }

    /* ���ͼ���host���� */
    if(usbc_no == 0){
        ret = eLIBs_fioctrl(usbm_dev_hadle, DEV_IOC_USR_HWSC_APP_INSMOD_USBH, 0, NULL);
    }else if(usbc_no == 1){
        ret = eLIBs_fioctrl(usbm_dev_hadle, DEV_IOC_USR_HWSC_APP_INSMOD_USBH_1, 0, NULL);
    }else if(usbc_no == 2){
        ret = eLIBs_fioctrl(usbm_dev_hadle, DEV_IOC_USR_HWSC_APP_INSMOD_USBH_2, 0, NULL);
    }else{
        __wrn("ERR: unkown usb controller number, usbc_no = %d\n", usbc_no);
    }
    
    if(ret == EPDK_FAIL){
        __inf("ERR: execute DEV_IOC_USR_HWSC_APP_INSMOD_USBH failed\n");
        
        eLIBs_fclose(usbm_dev_hadle);
        usbm_dev_hadle = NULL;
        
        return EPDK_FAIL;
    }

    /* �ر� hwsc �豸 */
    eLIBs_fclose(usbm_dev_hadle);
    usbm_dev_hadle = NULL;

	/*wait to install host driver less than 2 second*/
//	esKRNL_TimeDly(200);

    return EPDK_OK;
}

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
__s32 App_RmmodUsbHostDriver(__u32 usbc_no)
{
    ES_FILE *usbm_dev_hadle = NULL;
    __s32 ret = 0;

	__inf("%s %d : App_RmmodUsbHostDriver usbc_no = %d\n", __FUNCTION__, __LINE__, usbc_no);

    /* �� hwsc �豸 */
    usbm_dev_hadle = eLIBs_fopen("b:\\HWSC\\hwsc", "r");
    if(usbm_dev_hadle == NULL){
        __wrn("ERR: eLIBs_fopen failed\n");
        return EPDK_FAIL;
    }

    /* ���ͼ���host���� */
    if(usbc_no == 0){
        ret = eLIBs_fioctrl(usbm_dev_hadle, DEV_IOC_USR_HWSC_APP_RMMOD_USBH, 0, NULL);
    }else if(usbc_no == 1){
        ret = eLIBs_fioctrl(usbm_dev_hadle, DEV_IOC_USR_HWSC_APP_RMMOD_USBH_1, 0, NULL);
    }else if(usbc_no == 2){
        ret = eLIBs_fioctrl(usbm_dev_hadle, DEV_IOC_USR_HWSC_APP_RMMOD_USBH_2, 0, NULL);
    }else{
        __wrn("ERR: unkown usb controller number, usbc_no = %d\n", usbc_no);
    }

    if(ret == EPDK_FAIL){
        __inf("ERR: execute DEV_IOC_USR_HWSC_APP_RMMOD_USBH failed\n");
        
        eLIBs_fclose(usbm_dev_hadle);
        usbm_dev_hadle = NULL;
        
        return EPDK_FAIL;
    }
    
    /* �ر� hwsc �豸 */
    eLIBs_fclose(usbm_dev_hadle);
    usbm_dev_hadle = NULL;

	/* delay 1s ȷ��usb_monitor���㹻��ʱ��ж�� */
//	esKRNL_TimeDly(200);
    
    return EPDK_OK;
}

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
__u32 App_IsUsbHostReomveComplete(__u32 usbc_no)
{
    ES_FILE *pHdle = NULL;
    __s32 ret = 0;
    __u32 status = 0;

    /* �� hwsc �豸 */
    pHdle = eLIBs_fopen("b:\\HWSC\\hwsc", "r");
    if(pHdle == NULL){
        __wrn("ERR: eLIBs_fopen failed\n");
        return 0;
    }

    /* �������� */
    if(usbc_no == 0){
        ret = eLIBs_fioctrl(pHdle, DEV_IOC_USR_HWSC_GET_USBH_WORK_STATUS, 0, NULL);
    }else if(usbc_no == 1){
        ret = eLIBs_fioctrl(pHdle, DEV_IOC_USR_HWSC_GET_USBH1_WORK_STATUS, 0, NULL);
    }else if(usbc_no == 2){
        ret = eLIBs_fioctrl(pHdle, DEV_IOC_USR_HWSC_GET_USBH2_WORK_STATUS, 0, NULL);
    }else{
        __wrn("ERR: unkown usb controller number, usbc_no = %d\n", usbc_no);
    }

    /* �ر� hwsc �豸 */
    eLIBs_fclose(pHdle);
    pHdle = NULL;
 
    /* ���ݷ���״̬��������Ӧ���ж� */
    if(!ret){
        status = 1;
    }else{
        status = 0;
    }

    return status;
}

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
__s32 App_WaitForUsbHostReomveComplete(__u32 usbc_no)
{
    __u32 time = 200;
    
    /* wait for usb host driver remove complete */
    while(time){
        if(App_IsUsbHostReomveComplete(usbc_no)){
            __msg("remove usb%d host complete\n", usbc_no);
            break;
        }
        
        esKRNL_TimeDly(100);
        
        time--;
    }

    /* timeout */
    if(time == 0){
        __wrn("remove usb%d host timeout\n", usbc_no);
        return EPDK_FAIL;
    }

    return EPDK_OK;
}
