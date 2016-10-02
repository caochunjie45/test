/*
*******************************************************************************************************************
*                                                         		Mod orange
*                                         			the embedded graphic support module
*
*                             				 Copyright(C), 2006-2008, Softwinner Microelectronic Co., Ltd.
*											               All Rights Reserved
*
*File Name： 	GUI_Lock.c
*
*Author： 		William Wang
*
*Version :   	1.1.0
*
*Date:  		2009-11-2
*
*Description : 	gui kernel implement function 
*
*Others : 		None at present.
*
* History :
*
* <Author>         <time>         <version>     <description>
*
* William Wang     2009-11-2         1.1.0          Create File
*
*******************************************************************************************************************
*/
#include "ToolKit_s.h"
#include "nest_lock.h"
#define MAX_LOCK_NESTING (48)   /* If a lot of nested dialogs are used, this may not be sufficient. */      
#define TASKIDNONE      (0)     //表示没有线程的id值.
/*
**********************************************************************************************************************
*                                               _GetTaskNo
*
* author:           
*
* date:             2009-10-29
*
* Description:       gettaskno 
*
* parameters:       
*
* return:           if success return GUI_OK
*                   if fail return the number of fail
                    当前的线程在数组TaskID[]的下标号，如果没有，就在TaskID[]找一个空闲的元素存储当前的线程ID号
* modify history: 
**********************************************************************************************************************
*/

static __s32 _GetTaskNo(CedarNestLock *nestlock) 
{
	__s32 i;
    __u32 TaskId = 0;
  	for (i=0; i< NESTLOCK_MAXTASK; i++) 
  	{
    	TaskId = pthread_self();
        
    	if (nestlock->TaskID[i] == TaskId)
    	{
      		return i;
    	}
    	if (nestlock->TaskID[i] == TASKIDNONE) 
    	{
      		nestlock->TaskID[i] = TaskId;
      		return i;
    	}
  	}
    //如果数组满了，就把第一个元素让出来.并把它对应的上下文清空，避免调试出错
    __wrn("task array full\n");
    nestlock->TaskID[0] = TaskId;
    if(nestlock->ContextSize)
    {
        memset(nestlock->TaskContext[0], 0, nestlock->ContextSize);
    }
  	return 0;
}

/* global */ 
/*
**********************************************************************************************************************
*                                               GUI_Unlock
*
* Description: 
*
* Arguments  : 
*
* Returns    : 
*
* Notes      : 
*
**********************************************************************************************************************
*/
void Cedar_NestUnlock(CedarNestLock *nestlock) 
{
    if (--nestlock->_EntranceCnt == 0) 
	{
    	pthread_mutex_unlock(nestlock->lock);
  	}
  	/* Test if _EntranceCnt is in reasonable range ... Not required in release builds */
  	if(nestlock->_EntranceCnt < 0)
  	{
  	    __err("Cedar_LOCK.c: Cedar_Unlock() _EntranceCnt underflow \n");
  	}
}

/*
**********************************************************************************************************************
*                                               GUI_Lock
*
* Description: 
*
* Arguments  : 
*
* Returns    : 
*
* Notes      : 
*
**********************************************************************************************************************
*/
void Cedar_NestLock(CedarNestLock *nestlock,void *context)
{
    if (nestlock->_EntranceCnt == 0) 
	{
    	pthread_mutex_lock(nestlock->lock);
        
    	nestlock->_TaskIDLock = pthread_self();            /* Save task ID */
  	} 
  	else 
  	{
  		if (nestlock->_TaskIDLock != pthread_self()) 
  		{
      		  pthread_mutex_lock(nestlock->lock);
              
      		  nestlock->_TaskIDLock  = pthread_self();        /* Save task ID */
    	}
  	}
  	
  	nestlock->_EntranceCnt++;
  	
  	if (nestlock->_EntranceCnt == 1) 
  	{
    	int TaskNo = _GetTaskNo(nestlock);
        
    	if (TaskNo != nestlock->_CurrentTaskNo)
    	{
      		/* Save data of current task            */
      		if (nestlock->_CurrentTaskNo >= 0)
      		{   
      		    /* Make sure _CurrentTaskNo is valid*/
                if(nestlock->ContextSize)
                {
                    memcpy(nestlock->TaskContext[nestlock->_CurrentTaskNo],context,nestlock->ContextSize);
                    /* Load data of this task           */
                    memcpy(context,nestlock->TaskContext[TaskNo],nestlock->ContextSize);
                }
      		}
      		nestlock->_CurrentTaskNo = TaskNo;
    	}
  	}
  	/* Test if _EntranceCnt is in reasonable range ... Not required in release builds */
  	if(nestlock->_EntranceCnt > MAX_LOCK_NESTING)
  	{
  	    __err("nest_lock.c: Cedar_Lock() _EntranceCnt overflow \n");
  	}
  	
}     

/*
**********************************************************************************************************************
*                                               GUI_LockAccept
*
* Description: 
*
* Arguments  : 
*
* Returns    : 
*
* Notes      : 
*
**********************************************************************************************************************
*/
__s32 Cedar_NestLockAccept(CedarNestLock *nestlock,void *context) 
{
  	if (nestlock->_EntranceCnt == 0) 
	{
    	if(!pthread_mutex_accept(nestlock->lock))
    	{
    	    return EPDK_FAIL;
    	}
    	nestlock->_TaskIDLock = pthread_self();            /* Save task ID */
  	} 
  	else 
  	{
  		if (nestlock->_TaskIDLock != pthread_self()) 
  		{
      		if(!pthread_mutex_accept(nestlock->lock))
      	    {
      		    return EPDK_FAIL;
      		}
      		nestlock->_TaskIDLock  = pthread_self();        /* Save task ID */
    	}
  	}
  	
  	nestlock->_EntranceCnt++;
  	
  	if (nestlock->_EntranceCnt == 1) 
  	{
    	int TaskNo = _GetTaskNo(nestlock);
        
    	if (TaskNo != nestlock->_CurrentTaskNo)
    	{
      		/* Save data of current task            */
      		if (nestlock->_CurrentTaskNo >= 0)
      		{   
      		    /* Make sure _CurrentTaskNo is valid*/
                if(nestlock->ContextSize)
                {
                    memcpy(nestlock->TaskContext[nestlock->_CurrentTaskNo],context,nestlock->ContextSize);
                    /* Load data of this task           */
                    memcpy(context,nestlock->TaskContext[TaskNo],nestlock->ContextSize);
                }
      		}
      		nestlock->_CurrentTaskNo = TaskNo;
    	}
  	}
  	/* Test if _EntranceCnt is in reasonable range ... Not required in release builds */
  	if(nestlock->_EntranceCnt > MAX_LOCK_NESTING)
  	{
  	    __err("Cedar_Lock() _EntranceCnt overflow \n");
  	}
  	    
  	return EPDK_OK;
}

/*
**********************************************************************************************************************
*                                               GUI_NestLockInit
*
* author:           
*
* date:             2009-10-29
*
* Description:      gui nestlockinit 
*
* parameters:       
*
* return:           if success return GUI_OK
*                   if fail return the number of fail
* modify history: 
**********************************************************************************************************************
*/
__s32 Cedar_NestLockInit(CedarNestLock *nestlock, __u32 ContextSize)
{
    __u32   i;
    __u32   j;
    
    //memset(nestlock, 0, sizeof(CedarNestLock));
    
    nestlock->unlock = Cedar_NestUnlock;
    nestlock->func_lock = Cedar_NestLock;
    nestlock->accept = Cedar_NestLockAccept;
    
    pthread_mutex_init(&nestlock->lock,(void *)KRNL_UNLOCKVAL);

    for(i = 0;i < NESTLOCK_MAXTASK;i++)
    {
        nestlock->TaskID[i] = TASKIDNONE;
        nestlock->TaskContext[i]= 0;
    }
    nestlock->_CurrentTaskNo = -1;
    nestlock->_EntranceCnt   =  0;
    nestlock->_TaskIDLock    =  0;
    nestlock->ContextSize   = ContextSize;
    if(nestlock->ContextSize != 0)
    {
        nestlock->TaskContext[0]= (void *)malloc(NESTLOCK_MAXTASK * nestlock->ContextSize);
        if(NULL == nestlock->TaskContext[0])
        {
            __wrn("malloc fail\n");
            goto _err0;
        }
        else
        {
            memset(nestlock->TaskContext[0], 0, sizeof(NESTLOCK_MAXTASK * nestlock->ContextSize));
            for(j=0;j<NESTLOCK_MAXTASK;j++)
            {
                nestlock->TaskContext[j] = (void*)((__u8*)nestlock->TaskContext[0] + nestlock->ContextSize*j);
            }
        }
    }
    return EPDK_OK;

_err0:
    pthread_mutex_exit(nestlock->lock);
    return EPDK_FAIL;

}

/*
**********************************************************************************************************************
*                                               GUITASK_Init
*
* Description: 
*
* Arguments  : 
*
* Returns    : 
*
* Notes      : 
*
**********************************************************************************************************************
*/
__s32 Cedar_NestLockExit(CedarNestLock *nestlock)
{
    if(nestlock->lock)
    {
        pthread_mutex_exit(nestlock->lock);
        nestlock->lock = NULL;
    }
    
    if(nestlock->ContextSize != 0)
    {
        //gui_bfree(nestlock->TaskContext[0],GUI_MAXTASK * nestlock->ContextSize);
        if(nestlock->TaskContext[0])
        {
            free(nestlock->TaskContext[0]);
            nestlock->TaskContext[0] = NULL;
        }
        nestlock->ContextSize = 0;
    }
    nestlock->unlock = NULL;
    nestlock->func_lock = NULL;
    nestlock->accept = NULL;
    return EPDK_OK;
}

CedarNestLock* newCedarNestLock(__u32 ContextSize)
{
    __s32   tmpret;
    CedarNestLock *pNestLock = (CedarNestLock*)malloc(sizeof(CedarNestLock));
    if(NULL == pNestLock)
    {
        WARNING("NestLock malloc fail\n");
        return NULL;
    }
    //初始化成员变量
    tmpret = Cedar_NestLockInit(pNestLock, ContextSize);
    if(tmpret != EPDK_OK)
    {
        goto _err0;
    }
    return pNestLock;
    
_err0:
    free(pNestLock);
    pNestLock = NULL;
    return NULL;
}
void deleteCedarNestLock(CedarNestLock *thiz)
{
    Cedar_NestLockExit(thiz);
    free(thiz);
}

