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
* File   : MsgQueue.h
* Version: V1.0
* By     : Eric_wang
* Date   : 2011-6-18
* Description:
    根据需要封装,现在只需要post和pend.
    
    封装epos的MessageQueue.自动带buffer.
    返回值:
    比较重要的有:
    OS_NO_ERR.
    KRNL_Q_FULL :msgqueue满了, 无法post.

    opt:OS_DEL_NO_PEND或OS_DEL_ALWAYS
        OS_DEL_NO_PEND: 不用等krnl_event都释放就可以删除.
        OS_DEL_ALWAYS:等所有的__krnl_event_t都释放,再删除.

    虽然可以允许按默认方式直接传指针,但是现在为简单起见,不提供该方式的实现,
要求使用者设置的nBufSize > 0，否则报错.
    
********************************************************************************
*/
#ifndef _MSGQUEUE_H_
#define _MSGQUEUE_H_
//__krnl_event_t  *esKRNL_QCreate (__u16 size);
//__krnl_event_t  *esKRNL_QDel (__krnl_event_t *pevent, __u8 opt, __u8 *err);
//void  *esKRNL_QPend (__krnl_event_t *pevent, __u16 timeout, __u8 *err);
//__u8  esKRNL_QPost (__krnl_event_t *pevent, void *msg);
typedef struct tag_MsgQBuf
{
    __s32   nUseFlag;   //是否在使用中
    void    *priv;      //这是根据nBufSize来分配的buffer,但具体的数据结构由MsgQueue的使用者来决定.如果nBufSize==0,就不分配内存了,priv直接存指针值.
}MsgQBuf;   //队列的一个msg的buf的数据结构
extern MsgQBuf* newMsgQBuf(__s32 nPrivSize);
extern void deleteMsgQBuf(MsgQBuf* thiz);

//关于MsgQueue_QPend的说明:
//其返回值正常情况下就是传进来的msg.如果没有pend到,就返回NULL.
typedef struct tag_MsgQueue MsgQueue;
typedef void* (*MsgQueue_QPend)(MsgQueue *thiz, __u16 timeout, __u8 *err, void *msg);  //msg对应MsgQBuf->priv, 内存由调用者分配,MsgQueue负责浅拷贝.如果nBufSize为0,那么MsgQueue会认为msg的类型为void**,
typedef __u32 (*MsgQueue_QPost)(MsgQueue *thiz, void *msg); //ret = OS_NO_ERR或KRNL_Q_FULL或其他,如果nBufSize为0,那么会按Queue默认方式处理,传值.
typedef struct tag_MsgQueue
{
    __krnl_event_t  *pMsgQ;
    __s32           nQSize; //消息队列长度
    __s32           nBufSize;   //msg是指针,指定它所挂的buffer的大小,自动分配.挂载在MsgQBuf的priv上.如果为0,意思是不要MsgQueue分配内存.
    DArray          *MsgQBufArray; //规定内部元素void*指向的数据结构是MsgQBuf.

    CedarLock       locker; //因为会有多个线程调用post函数,所以要加lock.
    MsgQueue_QPend  QPend;
    MsgQueue_QPost  QPost;
}MsgQueue;
extern MsgQueue* newMsgQueue(__s32 nQSize, __s32 nBufSize);
extern void deleteMsgQueue(MsgQueue *thiz, __u8 opt, __u8 *err);    //opt:OS_DEL_NO_PEND或OS_DEL_ALWAYS, err:OS_NO_ERR或KRNL_Q_FULL或其他

#endif  /* _MSGQUEUE_H_ */

