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
    用于处理模块状态转换的基类，一般不要调用ModuleStatus_NotifyNewStatus通知stop状态.
stop是特殊的操作.
********************************************************************************
*/
#ifndef _MODULE_STATUS_OBJ_H_
#define _MODULE_STATUS_OBJ_H_
typedef enum tag_MODULE_STATUS_CHANGE_FLAG
{
    MODULE_STATUS_CHANGE_FLAG_0 = 0,    //没有改变
    MODULE_STATUS_CHANGE_FLAG_1 = 1,    //在IO接口处理改变
    MODULE_STATUS_CHANGE_FLAG_2 = 2,    //在IO接口接受改变
    MODULE_STATUS_CHANGE_FLAG_3 = 3,    //线程检测到状态改变,但没有处理.
    MODULE_STATUS_CHANGE_FLAG_4 = 4,    //线程正在处理
    //0xFF表示准备stop了
}EnumModuleStatusChangeFlag;

typedef struct tag_MODULE_STATUS ModuleStatus;
typedef __s32   (*ModuleStatus_Initial)                         (ModuleStatus *thiz, __s32 status);
typedef __s32   (*ModuleStatus_GetStatusChangePermission)       (ModuleStatus *thiz); //得到进行状态转换的许可,-1表示当前不允许状态转换
typedef __s32   (*ModuleStatus_ReleaseStatusChangePermission)   (ModuleStatus *thiz); //归还进行状态转换的授权
typedef __s32   (*ModuleStatus_NotifyNewStatus)                 (ModuleStatus *thiz, __s32 nNewStatus, __s32 nForceFlag); //通知要转到的目标status, nforceflag表示是否强制, 1强制
typedef __s32   (*ModuleStatus_DetectNewStatus)                 (ModuleStatus *thiz); //线程中部发现状态变化，调用本函数做处理
typedef __s32   (*ModuleStatus_AcceptNewStatus)                 (ModuleStatus *thiz, __s32 uStatusChangeFlag); //线程正式接受新状态
typedef __s32   (*ModuleStatus_DenyNewStatus)                   (ModuleStatus *thiz); //线程拒绝接受新状态

typedef struct tag_MODULE_STATUS
{
    __u8                status;         //当前状态, audio play-back module status, MEDIA_STATUS_STOP, PLAY, PAUSE, FF,RR, 一般是MEDIA_STATUS_STOP等，也可以是其他的类型
    __u8                uWishStatus;    //将要转到的状态,
    //__u8                uPrevStatus;    //上一个稳定的状态. 当发生状态转换时,把当前的status赋给uPrevStatus.现在还没有启用,adec_com可能会用到的
    __u8                uStatusChangeFlag; /* EnumModuleStatusChangeFlag, 0:平稳状态,1:正在IO接口做状态转换处理,2:IO接口接受状态转换的要求,开始处理,3:task检测到新状态，但未处理,即status没有置为uWishStatus, 4:已检测到新状态，正在做处理,status已经置为uWishStatus,  0xFF:特殊值，表示强制更改状态*/
    //CedarLock           StatusLock;     //用于双线程的互斥,本身不使用它.子类去用,为省事而放在这里.用于status和相关变量操作的互斥.
    ModuleStatus_Initial                        initial;
    ModuleStatus_GetStatusChangePermission      get_status_change_permission;
    ModuleStatus_ReleaseStatusChangePermission  release_status_change_permission;
    ModuleStatus_NotifyNewStatus                notify_new_status;   //通知转换新状态
    ModuleStatus_DetectNewStatus                detect_new_status;
    ModuleStatus_AcceptNewStatus                accept_new_status;     //正式确定新状态
    ModuleStatus_DenyNewStatus                  deny_new_status;     //拒绝新状态,恢复老状态
}ModuleStatus;
extern __s32 ModuleStatus_Init(ModuleStatus *thiz); //模拟构造函数，主要是给成员函数赋值，也可以给成员变量赋值，当该类不是被malloc出来，而是直接创建时，调用该函数
extern __s32 ModuleStatus_Exit(ModuleStatus *thiz); //析构函数,semphore要消除

#endif  /* _MODULE_STATUS_OBJ_H_ */

