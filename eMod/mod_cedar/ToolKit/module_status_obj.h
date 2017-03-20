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
* File   : module_status_obj.h
* Version: V1.0
* By     : Eric_wang
* Date   : 2011-1-21
* Description:
    ���ڴ���ģ��״̬ת���Ļ��࣬һ�㲻Ҫ����ModuleStatus_NotifyNewStatus֪ͨstop״̬.
stop������Ĳ���.
********************************************************************************
*/
#ifndef _MODULE_STATUS_OBJ_H_
#define _MODULE_STATUS_OBJ_H_
typedef enum tag_MODULE_STATUS_CHANGE_FLAG
{
    MODULE_STATUS_CHANGE_FLAG_0 = 0,    //û�иı�
    MODULE_STATUS_CHANGE_FLAG_1 = 1,    //��IO�ӿڴ���ı�
    MODULE_STATUS_CHANGE_FLAG_2 = 2,    //��IO�ӿڽ��ܸı�
    MODULE_STATUS_CHANGE_FLAG_3 = 3,    //�̼߳�⵽״̬�ı�,��û�д���.
    MODULE_STATUS_CHANGE_FLAG_4 = 4,    //�߳����ڴ���
    //0xFF��ʾ׼��stop��
}EnumModuleStatusChangeFlag;

typedef struct tag_MODULE_STATUS ModuleStatus;
typedef __s32   (*ModuleStatus_Initial)                         (ModuleStatus *thiz, __s32 status);
typedef __s32   (*ModuleStatus_GetStatusChangePermission)       (ModuleStatus *thiz); //�õ�����״̬ת�������,-1��ʾ��ǰ������״̬ת��
typedef __s32   (*ModuleStatus_ReleaseStatusChangePermission)   (ModuleStatus *thiz); //�黹����״̬ת������Ȩ
typedef __s32   (*ModuleStatus_NotifyNewStatus)                 (ModuleStatus *thiz, __s32 nNewStatus, __s32 nForceFlag); //֪ͨҪת����Ŀ��status, nforceflag��ʾ�Ƿ�ǿ��, 1ǿ��
typedef __s32   (*ModuleStatus_DetectNewStatus)                 (ModuleStatus *thiz); //�߳��в�����״̬�仯�����ñ�����������
typedef __s32   (*ModuleStatus_AcceptNewStatus)                 (ModuleStatus *thiz, __s32 uStatusChangeFlag); //�߳���ʽ������״̬
typedef __s32   (*ModuleStatus_DenyNewStatus)                   (ModuleStatus *thiz); //�ܾ߳̾�������״̬

typedef struct tag_MODULE_STATUS
{
    __u8                status;         //��ǰ״̬, audio play-back module status, MEDIA_STATUS_STOP, PLAY, PAUSE, FF,RR, һ����MEDIA_STATUS_STOP�ȣ�Ҳ����������������
    __u8                uWishStatus;    //��Ҫת����״̬,
    //__u8                uPrevStatus;    //��һ���ȶ���״̬. ������״̬ת��ʱ,�ѵ�ǰ��status����uPrevStatus.���ڻ�û������,adec_com���ܻ��õ���
    __u8                uStatusChangeFlag; /* EnumModuleStatusChangeFlag, 0:ƽ��״̬,1:����IO�ӿ���״̬ת������,2:IO�ӿڽ���״̬ת����Ҫ��,��ʼ����,3:task��⵽��״̬����δ����,��statusû����ΪuWishStatus, 4:�Ѽ�⵽��״̬������������,status�Ѿ���ΪuWishStatus,  0xFF:����ֵ����ʾǿ�Ƹ���״̬*/
    //CedarLock           StatusLock;     //����˫�̵߳Ļ���,����ʹ����.����ȥ��,Ϊʡ�¶���������.����status����ر��������Ļ���.
    ModuleStatus_Initial                        initial;
    ModuleStatus_GetStatusChangePermission      get_status_change_permission;
    ModuleStatus_ReleaseStatusChangePermission  release_status_change_permission;
    ModuleStatus_NotifyNewStatus                notify_new_status;   //֪ͨת����״̬
    ModuleStatus_DetectNewStatus                detect_new_status;
    ModuleStatus_AcceptNewStatus                accept_new_status;     //��ʽȷ����״̬
    ModuleStatus_DenyNewStatus                  deny_new_status;     //�ܾ���״̬,�ָ���״̬
}ModuleStatus;
extern __s32 ModuleStatus_Init(ModuleStatus *thiz); //ģ�⹹�캯������Ҫ�Ǹ���Ա������ֵ��Ҳ���Ը���Ա������ֵ�������಻�Ǳ�malloc����������ֱ�Ӵ���ʱ�����øú���
extern __s32 ModuleStatus_Exit(ModuleStatus *thiz); //��������,semphoreҪ����

#endif  /* _MODULE_STATUS_OBJ_H_ */

