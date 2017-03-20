/*
*******************************************************************************************************************
*                                                         		Mod orange
*                                         			the embedded graphic support module
*
*                             				 Copyright(C), 2006-2008, Softwinner Microelectronic Co., Ltd.
*											               All Rights Reserved
*
*File Name�� 	GUI_Lock.h
*
*Author�� 		William Wang
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
* �����API�ļ�
*******************************************************************************************************************
*/
#ifndef _NEST_LOCK_H_
#define _NEST_LOCK_H_

//#include "GUI_ToolKit_i.h"

#define KRNL_LOCKVAL        (0) //��ʾ��ʼ��ʱ�Ƿ���ס
#define KRNL_UNLOCKVAL      (1)

#define NESTLOCK_MAXTASK (8)    //16, ����������߳�����ʹ�����Ƕ����.����������Ҫǿ����context��.

/*GUIǶ�������ݽṹ*/
/*******************************************************************************
Function name: tag_CEDAR_NESTLOCK
Members: 
    ����TaskContext��˵��:
    �̵߳������ģ�һ�������ǰ�߳��õ���Ƕ����,��ôҪ�ѵ�ǰ����������Ϣ�������ʧȥ(���ϴ��Լ�����)Ƕ�������߳�,������ʧȥǶ�������߳�
    �����õ�Ƕ����,�Ϳ��Իָ�����ʧȥǶ����ʱ�������ĵ����(���Զ�����Ƕ������Ƕ�����ֱ�������߳�����ʱ�����������).�����������Ϣ
    ����ʹ�����Լ�������,������Լ�¼��ǰ�̵߳�pid,�������ָ�ʱ,�Ϳ���֪���������ĸ��߳�ռȥ�˱��̵߳�Ƕ����;Ҳ������������Ϣ.
    ��һ��pend��lock���̲߳��������������ģ�������̵߳ڶ�������pend��lock, �Ϳ��Բ鵽�ϴα������̰߳���Ƕ����(���Զ�������
    �������߳����뵽Ƕ����)ʱ����������.������Ǹ��߳��Լ����������ͷ����뵽Ƕ����,��ôcontextҲ������.��funk_lock���غ�õ���context����
    �����ֵ.

    ���Ժ���func_lock()�Ĳ���context,����ʱ�ǵ�ǰ����Ƕ�����̵߳�������,����ʱ,��������߳��ϴα�����Ƕ����ʱ����������.
    ����Ǹ��̵߳�һ���õ�Ƕ����,context��ԭ�ⲻ��.������̷߳��������ͷ��������õ���Ƕ����,��ôcontextҲ��ԭ�ⲻ��.
    
    ��ΪĿǰʹ�ø����ݽṹ����malloc�ģ�����new��delete��ʱ��д.init()��exit()Ҳ�������Ա������
    init()��exit()��Ϊ���⺯����������.
*******************************************************************************/

typedef struct tag_CEDAR_NESTLOCK CedarNestLock;
typedef void       (*CedarNestLock_Unlock)         (CedarNestLock *thiz);
typedef void       (*CedarNestLock_Lock)           (CedarNestLock *thiz, void *context);    //����lock��
typedef __s32      (*CedarNestLock_Accept)         (CedarNestLock *thiz, void *context);    //lock�������ͷ���FAILURE
typedef struct tag_CEDAR_NESTLOCK
{
    pthread_mutex_t     lock;       //��,�������semaphore���ڹ���ʱ���봴��
    __u32               TaskID[NESTLOCK_MAXTASK];
    __s32               _CurrentTaskNo; //����ռ��nestlock���߳�������TaskContext[]���±��
    __s32               _EntranceCnt;   //���뵽nestlock���߳���������ܴ���(�ظ�����)
    __s32               _TaskIDLock;    //����ռ��nestlock���߳�ID��
    void                *TaskContext[NESTLOCK_MAXTASK]; //�̵߳������ģ�һ��洢����߳��л�������һ���̵߳���Ϣ��
    __u32               ContextSize;    //�����ĵ�size����ʼ��ʱָ��

    CedarNestLock_Unlock    unlock;
    CedarNestLock_Lock      func_lock;  //����lock��
    CedarNestLock_Accept    accept; //lock�������ͷ���FAILURE
    
}CedarNestLock;
extern __s32 Cedar_NestLockInit(CedarNestLock *nestlock, __u32 ContextSize);
extern __s32 Cedar_NestLockExit(CedarNestLock *nestlock);
extern CedarNestLock* newCedarNestLock(__u32 ContextSize);
extern void deleteCedarNestLock(CedarNestLock *thiz);

#endif  /* _NEST_LOCK_H_ */

