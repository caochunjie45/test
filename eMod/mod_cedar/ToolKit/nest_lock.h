/*
*******************************************************************************************************************
*                                                         		Mod orange
*                                         			the embedded graphic support module
*
*                             				 Copyright(C), 2006-2008, Softwinner Microelectronic Co., Ltd.
*											               All Rights Reserved
*
*File Name： 	GUI_Lock.h
*
*Author： 		William Wang
*
*Version :   	1.1.0
*
*Date:  		2009-10-29
*
*Description : 	gui task definition 
*
*Others : 		None at present.
*
* History :
*
* <Author>          <time>         <version>     <description>
*
* William Wang     2009-10-29         1.1.0          Create File
*
* 对外的API文件
*******************************************************************************************************************
*/
#ifndef _NEST_LOCK_H_
#define _NEST_LOCK_H_

//#include "GUI_ToolKit_i.h"

#define KRNL_LOCKVAL        (0) //表示初始化时是否锁住
#define KRNL_UNLOCKVAL      (1)

#define NESTLOCK_MAXTASK (8)    //16, 最多允许几个线程申请使用这个嵌套锁.超出数量就要强制清context了.

/*GUI嵌套锁数据结构*/
/*******************************************************************************
Function name: tag_CEDAR_NESTLOCK
Members: 
    关于TaskContext的说明:
    线程的上下文，一般如果当前线程拿到了嵌套锁,那么要把当前的上下文信息保存给刚失去(或上次自己放弃)嵌套锁的线程,这样当失去嵌套锁的线程
    重新拿到嵌套锁,就可以恢复当初失去嵌套锁时的上下文的情况(或自动放弃嵌套锁后，嵌套锁又被另外的线程申请时的上下文情况).这个上下文信息
    是由使用者自己决定的,例如可以记录当前线程的pid,这样当恢复时,就可以知道当初是哪个线程占去了本线程的嵌套锁;也可以是其他信息.
    第一次pend到lock的线程不保存带入的上下文，如果该线程第二次重新pend到lock, 就可以查到上次被其他线程剥夺嵌套锁(或自动放弃后，
    被其他线程申请到嵌套锁)时的上下文了.而如果是该线程自己反复申请释放申请到嵌套锁,那么context也不保存.在funk_lock返回后得到的context还是
    带入的值.

    所以函数func_lock()的参数context,送入时是当前申请嵌套锁线程的上下文,返回时,就是这个线程上次被剥夺嵌套锁时的上下文了.
    如果是该线程第一次拿到嵌套锁,context会原封不动.如果该线程反复申请释放再申请拿到的嵌套锁,那么context也会原封不动.
    
    因为目前使用该数据结构不是malloc的，所以new和delete暂时不写.init()和exit()也不加入成员函数中
    init()和exit()作为对外函数公开出来.
*******************************************************************************/

typedef struct tag_CEDAR_NESTLOCK CedarNestLock;
typedef void       (*CedarNestLock_Unlock)         (CedarNestLock *thiz);
typedef void       (*CedarNestLock_Lock)           (CedarNestLock *thiz, void *context);    //必须lock到
typedef __s32      (*CedarNestLock_Accept)         (CedarNestLock *thiz, void *context);    //lock不到，就返回FAILURE
typedef struct tag_CEDAR_NESTLOCK
{
    pthread_mutex_t     lock;       //锁,这里就是semaphore，在构造时必须创建
    __u32               TaskID[NESTLOCK_MAXTASK];
    __s32               _CurrentTaskNo; //正在占用nestlock的线程在数组TaskContext[]的下标号
    __s32               _EntranceCnt;   //申请到nestlock的线程中申请的总次数(重复申请)
    __s32               _TaskIDLock;    //正在占用nestlock的线程ID号
    void                *TaskContext[NESTLOCK_MAXTASK]; //线程的上下文，一般存储这个线程切换到的下一个线程的信息，
    __u32               ContextSize;    //上下文的size，初始化时指定

    CedarNestLock_Unlock    unlock;
    CedarNestLock_Lock      func_lock;  //必须lock到
    CedarNestLock_Accept    accept; //lock不到，就返回FAILURE
    
}CedarNestLock;
extern __s32 Cedar_NestLockInit(CedarNestLock *nestlock, __u32 ContextSize);
extern __s32 Cedar_NestLockExit(CedarNestLock *nestlock);
extern CedarNestLock* newCedarNestLock(__u32 ContextSize);
extern void deleteCedarNestLock(CedarNestLock *thiz);

#endif  /* _NEST_LOCK_H_ */

