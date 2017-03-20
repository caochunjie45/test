/*
********************************************************************************
*                                    eMOD
*                   the Easy Portable/Player Develop Kits
*                               mod_cedar sub-system
*                          (module name, e.g.mpeg4 decoder plug-in) module
*
*          (c) Copyright 2010-2012, Allwinner Microelectronic Co., Ltd.
*                              All Rights Reserved
*
* File   : module_status_obj.c
* Version: V1.0
* By     : Eric_wang
* Date   : 2011-1-21
* Description:
********************************************************************************
*/
//#include "audio_plybk_s.h"
#include "ToolKit_s.h"
#include "module_status_obj.h"

/*******************************************************************************
Function name: Impl_ModuleStatus_Initial
Description: 
    1.�ô����״̬��ʼ��������
Parameters: 
    
Return: 
    
Time: 2011/1/21
*******************************************************************************/
__s32 Impl_ModuleStatus_Initial(ModuleStatus *thiz, __s32 status)
{
    thiz->status = thiz->uWishStatus = status;
    thiz->uStatusChangeFlag = 0;
    return 0;
}

/*******************************************************************************
Function name: Impl_ModuleStatus_GetStatusChangePermission
Description: 
    1���õ�״̬ת������Ȩ
Parameters: 
    
Return: 
    
Time: 2011/1/21
*******************************************************************************/
__s32 Impl_ModuleStatus_GetStatusChangePermission(ModuleStatus *thiz) //�õ�����״̬ת�������,����ֵ-1��ʾ��ǰ������״̬ת��
{
    if(0 == thiz->uStatusChangeFlag)
    {
        if(thiz->status != thiz->uWishStatus)
        {
            WARNING("fatal error! status[%d], wishStatus[%d]\n", thiz->status, thiz->uWishStatus);
            return -1;
        }
        thiz->uStatusChangeFlag = 1;
        return 0;
    }
    else
    {
        WARNING("uStatusChangeFlag[%d], status[%d], wishStatus[%d]\n", thiz->uStatusChangeFlag, thiz->status, thiz->uWishStatus);
        return -1;
    }
}

/*******************************************************************************
Function name: ModuleStatus_ReleaseStatusChangePermission
Description: 
    1.�黹��Ȩ
Parameters: 
    
Return: 
    
Time: 2011/1/21
*******************************************************************************/
__s32 Impl_ModuleStatus_ReleaseStatusChangePermission (ModuleStatus *thiz) //�黹����״̬ת������Ȩ
{
    thiz->uStatusChangeFlag = 0;
    return 0;
}


/*******************************************************************************
Function name: Impl_ModuleStatus_NotifyNewStatus
Description: 
    1.֪ͨҪת����Ŀ��status,��״̬ת��
    2. �ȵõ���Ȩ��Ȼ���ٵ��ñ�������������ִ�����,uStatusChangeFlag = 2
    3. ���û����Ȩ������ǿ������״̬��һ������ת��stop״̬.
Parameters: 
    
Return: 
    0:success
    -1:fail
Time: 2011/1/21
*******************************************************************************/
__s32 Impl_ModuleStatus_NotifyNewStatus(ModuleStatus *thiz, __s32 nNewStatus, __s32 nForceFlag)
{
    if(nForceFlag == 1)
    {
        WARNING("FORCE set status, uStatusChangeFlag[%d], status[%d], uWishStatus[%d], newStatus[%d]\n", 
            thiz->uStatusChangeFlag, thiz->status, thiz->uWishStatus, nNewStatus);
        thiz->uWishStatus = nNewStatus;
        thiz->uStatusChangeFlag = 0xFF;
        return 0;
    }
    if(0==thiz->uStatusChangeFlag)
    {
        WARNING("no permission!\n");
        return -1;
    }
    if(thiz->uStatusChangeFlag != 1)
    {
        WARNING("uStatusChangeFlag[%d] fatal error!, status[%d], uWishStatus[%d]\n", thiz->uStatusChangeFlag, thiz->status, thiz->uWishStatus);
        return -1;
    }
    thiz->uWishStatus = nNewStatus;
    thiz->uStatusChangeFlag = 2;
    return 0;
}
__s32 Impl_ModuleStatus_DetectNewStatus(ModuleStatus *thiz) //�߳��в�����״̬�仯�����ñ�����������
{
    if(2 == thiz->uStatusChangeFlag)
    {
        thiz->uStatusChangeFlag = 3;
        return 0;
    }
    else
    {
        WARNING("ModuleStatus : detect new status fatal error! uStatusChangeFlag[%d]\n", thiz->uStatusChangeFlag);
        return -1;
    }
}

/*******************************************************************************
Function name: Impl_ModuleStatus_AcceptNewStatus
Description: 
    1.���uStatusChangeFlag == 4, �Ȳ�Ҫ��uWishStatus����status,��Ϊ�߳�ֻ�����ڴ���,�������ǰ��û��תΪuWishStatus.
Parameters: 
    
Return: 
    
Time: 2011/6/12
*******************************************************************************/
__s32 Impl_ModuleStatus_AcceptNewStatus(ModuleStatus *thiz, __s32 uStatusChangeFlag) //��ʽ�����µ�status,ͬʱ����uStatusChangeFlag��ֵ����Ϊ��ͬ�����uStatusChangeFlag���ܵ���0��Ҳ���ܵ���4.���Ե������Լ�ȷ��
{
    if(uStatusChangeFlag!=MODULE_STATUS_CHANGE_FLAG_0 && uStatusChangeFlag!=MODULE_STATUS_CHANGE_FLAG_4)
    {
        WARNING("fatal error! AcceptNewStatus(), uStatusChangeFlag[%x]\n", uStatusChangeFlag);
        return -1;
    }
    if(MODULE_STATUS_CHANGE_FLAG_2 == thiz->uStatusChangeFlag || MODULE_STATUS_CHANGE_FLAG_3 == thiz->uStatusChangeFlag)
    {
        if(MODULE_STATUS_CHANGE_FLAG_0 == uStatusChangeFlag)
        {
            thiz->status = thiz->uWishStatus;
        }
        thiz->uStatusChangeFlag = uStatusChangeFlag;
    }
    else if(MODULE_STATUS_CHANGE_FLAG_4 == thiz->uStatusChangeFlag)
    {
        if(MODULE_STATUS_CHANGE_FLAG_0 == uStatusChangeFlag)
        {
            thiz->status = thiz->uWishStatus;
        }
        else
        {
            WARNING("accept new status fatal error! uStatusChangeFlag[%d]\n", uStatusChangeFlag);
        }
        thiz->uStatusChangeFlag = uStatusChangeFlag;
    }
    else
    {
        WARNING("fatal error!accept new status: [%x][%x][%x]\n", thiz->uStatusChangeFlag, thiz->status, thiz->uWishStatus);
    }
    return 0;
}

//__s32 Impl_ModuleStatus_AcceptNewStatus0(ModuleStatus *thiz, __s32 uStatusChangeFlag) //��ʽ�����µ�status,ͬʱ����uStatusChangeFlag��ֵ����Ϊ��ͬ�����uStatusChangeFlag���ܵ���0��Ҳ���ܵ���4.���Ե������Լ�ȷ��
//{
//    thiz->status = thiz->uWishStatus;
//    if(2 == thiz->uStatusChangeFlag || 3 == thiz->uStatusChangeFlag)
//    {
//        thiz->uStatusChangeFlag = uStatusChangeFlag;
//    }
//    else if(MODULE_STATUS_CHANGE_FLAG_4 == thiz->uStatusChangeFlag)
//    {
//        if(0 == uStatusChangeFlag)
//        {
//        }
//        else
//        {
//            WARNING("accept new status error! uStatusChangeFlag[%d]\n", uStatusChangeFlag);
//        }
//        thiz->uStatusChangeFlag = uStatusChangeFlag;
//    }
//    else
//    {
//        WARNING("accept new status: [%x][%x][%x]\n", thiz->uStatusChangeFlag, thiz->status, thiz->uWishStatus);
//    }
//    return 0;
//}

__s32 Impl_ModuleStatus_DenyNewStatus(ModuleStatus *thiz)
{
    if(MODULE_STATUS_CHANGE_FLAG_2 == thiz->uStatusChangeFlag 
        || MODULE_STATUS_CHANGE_FLAG_3 == thiz->uStatusChangeFlag
        || MODULE_STATUS_CHANGE_FLAG_4 == thiz->uStatusChangeFlag)
    {
        WARNING("deny newstatus[%x], [%x][%x][%x]\n", thiz->uWishStatus, 
            thiz->uStatusChangeFlag, thiz->status, thiz->uWishStatus);
        thiz->uWishStatus = thiz->status;
        thiz->uStatusChangeFlag = 0;
    }
    else
    {
        WARNING("deny new status fatal error![%x][%x][%x]\n", 
            thiz->uStatusChangeFlag, thiz->status, thiz->uWishStatus);
        thiz->uWishStatus = thiz->status;
    }
    return 0;
}

__s32 ModuleStatus_Init(ModuleStatus *thiz)
{
    __s32   tmpret = EPDK_OK;
    memset(thiz, 0, sizeof(ModuleStatus));
    //tmpret = CedarLock_Init(&thiz->StatusLock);
    if(tmpret != EPDK_OK)
    {
        WARNING("cedarlock init fail\n");
        return -1;
    }
    thiz->initial = Impl_ModuleStatus_Initial;
    thiz->get_status_change_permission = Impl_ModuleStatus_GetStatusChangePermission;
    thiz->release_status_change_permission = Impl_ModuleStatus_ReleaseStatusChangePermission;
    thiz->notify_new_status = Impl_ModuleStatus_NotifyNewStatus;
    thiz->detect_new_status = Impl_ModuleStatus_DetectNewStatus;
    thiz->accept_new_status = Impl_ModuleStatus_AcceptNewStatus;     //��ʽȷ����״̬
    thiz->deny_new_status = Impl_ModuleStatus_DenyNewStatus;
    return 0;
}

__s32 ModuleStatus_Exit(ModuleStatus *thiz) //��������,semphoreҪ����
{
    //CedarLock_Exit(&thiz->StatusLock);
    return 0;
}

