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
    ������Ҫ��װ,����ֻ��Ҫpost��pend.
    
    ��װepos��MessageQueue.�Զ���buffer.
    ����ֵ:
    �Ƚ���Ҫ����:
    OS_NO_ERR.
    KRNL_Q_FULL :msgqueue����, �޷�post.

    opt:OS_DEL_NO_PEND��OS_DEL_ALWAYS
        OS_DEL_NO_PEND: ���õ�krnl_event���ͷžͿ���ɾ��.
        OS_DEL_ALWAYS:�����е�__krnl_event_t���ͷ�,��ɾ��.

    ��Ȼ��������Ĭ�Ϸ�ʽֱ�Ӵ�ָ��,��������Ϊ�����,���ṩ�÷�ʽ��ʵ��,
Ҫ��ʹ�������õ�nBufSize > 0�����򱨴�.
    
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
    __s32   nUseFlag;   //�Ƿ���ʹ����
    void    *priv;      //���Ǹ���nBufSize�������buffer,����������ݽṹ��MsgQueue��ʹ����������.���nBufSize==0,�Ͳ������ڴ���,privֱ�Ӵ�ָ��ֵ.
}MsgQBuf;   //���е�һ��msg��buf�����ݽṹ
extern MsgQBuf* newMsgQBuf(__s32 nPrivSize);
extern void deleteMsgQBuf(MsgQBuf* thiz);

//����MsgQueue_QPend��˵��:
//�䷵��ֵ��������¾��Ǵ�������msg.���û��pend��,�ͷ���NULL.
typedef struct tag_MsgQueue MsgQueue;
typedef void* (*MsgQueue_QPend)(MsgQueue *thiz, __u16 timeout, __u8 *err, void *msg);  //msg��ӦMsgQBuf->priv, �ڴ��ɵ����߷���,MsgQueue����ǳ����.���nBufSizeΪ0,��ôMsgQueue����Ϊmsg������Ϊvoid**,
typedef __u32 (*MsgQueue_QPost)(MsgQueue *thiz, void *msg); //ret = OS_NO_ERR��KRNL_Q_FULL������,���nBufSizeΪ0,��ô�ᰴQueueĬ�Ϸ�ʽ����,��ֵ.
typedef struct tag_MsgQueue
{
    __krnl_event_t  *pMsgQ;
    __s32           nQSize; //��Ϣ���г���
    __s32           nBufSize;   //msg��ָ��,ָ�������ҵ�buffer�Ĵ�С,�Զ�����.������MsgQBuf��priv��.���Ϊ0,��˼�ǲ�ҪMsgQueue�����ڴ�.
    DArray          *MsgQBufArray; //�涨�ڲ�Ԫ��void*ָ������ݽṹ��MsgQBuf.

    CedarLock       locker; //��Ϊ���ж���̵߳���post����,����Ҫ��lock.
    MsgQueue_QPend  QPend;
    MsgQueue_QPost  QPost;
}MsgQueue;
extern MsgQueue* newMsgQueue(__s32 nQSize, __s32 nBufSize);
extern void deleteMsgQueue(MsgQueue *thiz, __u8 opt, __u8 *err);    //opt:OS_DEL_NO_PEND��OS_DEL_ALWAYS, err:OS_NO_ERR��KRNL_Q_FULL������

#endif  /* _MSGQUEUE_H_ */

