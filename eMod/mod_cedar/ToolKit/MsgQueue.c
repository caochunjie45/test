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
* File   : MsgQueue.c
* Version: V1.0
* By     : Eric_wang
* Date   : 2011-6-18
* Description:
********************************************************************************
*/
#include "ToolKit_s.h"
#include "MsgQueue.h"

MsgQBuf* newMsgQBuf(__s32 nPrivSize)
{
    MsgQBuf* pQBuf = (MsgQBuf*)malloc(sizeof(MsgQBuf));
    if(NULL == pQBuf)
    {
        WARNING("malloc qbuf fail\n");
        return NULL;
    }
    memset(pQBuf, 0, sizeof(MsgQBuf));
    pQBuf->priv = malloc(nPrivSize);
    if(NULL == pQBuf->priv)
    {
        WARNING("malloc qbuf priv fail\n");
        goto _err0;
    }
    memset(pQBuf->priv, 0, nPrivSize);
    return pQBuf;

_err0:
    free(pQBuf);
    return NULL;
}
extern void deleteMsgQBuf(MsgQBuf* thiz)
{
    if(NULL == thiz)
    {
        WARNING("fatal error! QBuf = NULL\n");
        return;
    }
    if(thiz->priv)
    {
        free(thiz->priv);
    }
    free(thiz);
    return;
}


/*******************************************************************************
Function name: MsgQBufDestroyFunc
Description: 
    1.ʵ�ֵĺ���ԭ��Ϊ:
        typedef void    (*DataDestroyFunc)(void* ctx, void* data); //����void*Ԫ��data��ָ���MsgQBuf, ctx�������õ������ģ����ʼ��ʱ�ʹ�����ָ�룬�����޸���ָ���struct
Parameters: 
    
Return: 
    
Time: 2011/6/18
*******************************************************************************/
void MsgQBufDestroyFunc(void* ctx, void* data)
{
    deleteMsgQBuf((MsgQBuf*)data);
    return;
}


/*******************************************************************************
Function name: MsgQBufCompareFunc_FindEmpty
Description: 
    1.����DArray_FindElem()�Ļص�����, ����Ϊ��һ��û�б�ʹ�õ�MsgQBuf,Ȼ�����̱��Ϊʹ����.
    2.����Ҫ������.���Բ���ctx��ֵ.
    3. ����ԭ��: typedef int     (*DataCompareFunc)(void* ctx, void* data); //ret = 0��ʾƥ��. data����void*Ԫ��, ctx��ƥ��ֵ���û���ʱ����������ģ�һ���void*ָ���struct��������ͬ��.����DArray_FindElem(), ctx >= data, ����ֵΪ>=0.
Parameters: 
    
Return: 
    
Time: 2011/6/18
*******************************************************************************/
int MsgQBufCompareFunc_FindEmpty(void* ctx, void* data) //ret = 0��ʾƥ��. data����void*Ԫ��, ctx��ƥ��ֵ���û���ʱ����������ģ�һ���void*ָ���struct��������ͬ��.����DArray_FindElem(), ctx >= data, ����ֵΪ>=0.
{
    MsgQBuf *pQBuf = (MsgQBuf*)data;
    if(0 == pQBuf->nUseFlag)
    {
        pQBuf->nUseFlag = 1;
        //WARNING("find empty msgQbuf\n");
        return 0;
    }
    else
    {
        return -1;
    }
}


/*******************************************************************************
Function name: Impl_MsgQueue_QPend
Description: 
    
Parameters: 
    msg:�û������ڴ沢����,����������ǳ����. msg��Ӧ����MsgQueue->priv.
Return: 
    OS_NO_ERR
    OS_TIMEOUT
    OS_ERR_PDATA_NULL  ��
Time: 2011/6/18
*******************************************************************************/
void* Impl_MsgQueue_QPend(MsgQueue *thiz, __u16 timeout, __u8 *err, void *msg)  //msg�ڴ��ɵ����߷���,MsgQueue����ǳ����.���nBufSizeΪ0,��ôMsgQueue����Ϊmsg������Ϊvoid**,
{
    MsgQBuf *pQBuf;
    if(NULL == msg)
    {
        WARNING("msg == NULL, check code!\n");
        *err = OS_ERR_PDATA_NULL;
        return NULL;
    }
    pQBuf = (MsgQBuf*)esKRNL_QPend(thiz->pMsgQ, timeout, err);
    if(OS_NO_ERR == *err)
    {
        if(pQBuf)
        {
            memcpy(msg, pQBuf->priv, thiz->nBufSize);
            pQBuf->nUseFlag = 0;
            return msg;
        }
        else
        {
            WARNING("fatal error! Qpend but pQbuf == NULL\n");
            return NULL;
        }
        
    }
    else
    {
        if(pQBuf)
        {
            WARNING("fatal error! err[%x], buf pQBuf[%x]!=NULL\n", *err, pQBuf);
            pQBuf->nUseFlag = 0;
        }
        return NULL;
    }
}

/*******************************************************************************
Function name: Impl_MsgQueue_QPost
Description: 
    1.msg���û������, ����������ǳ����.msg��Ӧ����MsgQBuf->priv.
    2. ���ǵ��ж���߳�Ҫ���ñ�����,����Ҫ����.
Parameters: 
    
Return: 
    OS_NO_ERR
    OS_ERR_PDATA_NULL
    OS_ERR_INVALID_OPT
    KRNL_Q_FULL ��
Time: 2011/6/18
*******************************************************************************/
__u32 Impl_MsgQueue_QPost(MsgQueue *thiz, void *msg)
{
    MsgQBuf *pQBuf = NULL;
    __u32   idx;
    __u32   nSize;
    __s32   tmpret;
    if(NULL == msg)
    {
        WARNING("param wrong, msg == NULL\n");
        return OS_ERR_PDATA_NULL;
    }
    //��thiz->MsgQBufArray���ҵ�һ���յ�Ԫ��,���û��,��ô����KRNL_Q_FULL.
    thiz->locker.lock(&thiz->locker);
    idx = thiz->MsgQBufArray->FindElem(thiz->MsgQBufArray, MsgQBufCompareFunc_FindEmpty, NULL);
    nSize = thiz->MsgQBufArray->GetLength(thiz->MsgQBufArray);
    thiz->locker.unlock(&thiz->locker);
    if(idx == nSize)
    {
        return KRNL_Q_FULL;
    }
    else if(idx > nSize)
    {
        WARNING("fatal error! MsgQBuf FindElem idx[%x], nSize[%x] check code!\n", idx, nSize);
        return OS_ERR_INVALID_OPT;
    }
    else    //��Ϊ����
    {
        tmpret = thiz->MsgQBufArray->GetElemByIndex(thiz->MsgQBufArray, idx, (void**)&pQBuf);
        if(tmpret!=EPDK_OK)
        {
            WARNING("fatal error! GetElemByIndex, pQBuf[%x], check code!\n", pQBuf);
        }
        memcpy(pQBuf->priv, msg, thiz->nBufSize);
        tmpret = esKRNL_QPost(thiz->pMsgQ, (void*)pQBuf);
        if(tmpret!=OS_NO_ERR)
        {
            WARNING("fatal error! QPost ret[%x]\n", tmpret);
            pQBuf->nUseFlag = 0;    //�������QBufԪ��
        }
        return tmpret;
    }
}

/*******************************************************************************
Function name: MsgQueue_Init
Description: 
    1. nQSize, nBufSize > 0, ��newMsgQueue()����,���ﲻ�����.
Parameters: 
    
Return: 
    
Time: 2011/6/18
*******************************************************************************/
__s32 MsgQueue_Init(MsgQueue *thiz, __s32 nQSize, __s32 nBufSize)
{
    __u8    err;
    __s32   i;
    __s32   tmpret;
    MsgQBuf *pMsgQBuf;
    
    thiz->nQSize = nQSize;
    thiz->nBufSize = nBufSize;

    //0.��ʼ����ͨ��
    tmpret = CedarLock_Init(&thiz->locker);
    if(tmpret != EPDK_OK)
    {
        return EPDK_FAIL;
    }
    //1.������Ϣ����,
    thiz->pMsgQ = esKRNL_QCreate(nQSize);
    if(thiz->pMsgQ == (__krnl_event_t*)0)
    {
        WARNING("QCreate fail\n");
        goto _err0;
    }
    //2.����QBuf����
    thiz->MsgQBufArray = newDArray(MsgQBufDestroyFunc, NULL, nQSize);
    if(NULL == thiz->MsgQBufArray)
    {
        WARNING("DArray create fail\n");
        goto _err1;
    }
    for(i=0; i<nQSize; i++)
    {
        pMsgQBuf = newMsgQBuf(nBufSize);
        if(NULL == pMsgQBuf)
        {
            WARNING("create MsgQBufArray fail, i=[%d]\n", i);
            goto _err2;            
        }
        tmpret = thiz->MsgQBufArray->AppendElem(thiz->MsgQBufArray, pMsgQBuf);
        if(tmpret != EPDK_OK)
        {
            WARNING("MsgQueue append fail\n");
            goto _err2;
        }
    }
    //3.��Ա��ֵ
    thiz->QPend = Impl_MsgQueue_QPend;
    thiz->QPost = Impl_MsgQueue_QPost;
    return EPDK_OK;

    
_err2:
    deleteDArray(thiz->MsgQBufArray);
    thiz->MsgQBufArray = NULL;
_err1:
    esKRNL_QDel(thiz->pMsgQ, OS_DEL_NO_PEND, &err);
    thiz->pMsgQ = NULL;
_err0:
    CedarLock_Exit(&thiz->locker);
    return EPDK_FAIL;
}
__s32 MsgQueue_Exit(MsgQueue *thiz)
{
    __u8    err;
    if(thiz->MsgQBufArray)
    {
        deleteDArray(thiz->MsgQBufArray);
        thiz->MsgQBufArray = NULL;
    }
    if(thiz->pMsgQ)
    {
        esKRNL_QDel(thiz->pMsgQ, OS_DEL_NO_PEND, &err);
        thiz->pMsgQ = NULL;
    }
    CedarLock_Exit(&thiz->locker);
    return EPDK_OK;
}

MsgQueue* newMsgQueue(__s32 nQSize, __s32 nBufSize)
{
    __s32   ret;
    MsgQueue    *pQueue;
    if(nQSize <= 0 || nBufSize <= 0)
    {
        WARNING("MsgQueue create fail, param wrong! [%d][%d]\n", nQSize, nBufSize);
        return NULL;
    }
    pQueue = malloc(sizeof(MsgQueue));
    if(NULL == pQueue)
    {
        WARNING("malloc pQ fail\n");
        return NULL;
    }
    memset(pQueue, 0, sizeof(MsgQueue));
    ret = MsgQueue_Init(pQueue, nQSize, nBufSize);
    if(ret != EPDK_OK)
    {
        WARNING("MsgQueue init fail\n");
        goto _err0;
    }
    return pQueue;

_err0:
    free(pQueue);
    return NULL;
}
/*******************************************************************************
Function name: deleteMsgQueue
Description: 
    //opt:OS_DEL_NO_PEND��OS_DEL_ALWAYS, err:OS_NO_ERR��KRNL_Q_FULL������
Parameters: 
    
Return: 
    
Time: 2011/6/18
*******************************************************************************/
void deleteMsgQueue(MsgQueue *thiz, __u8 opt, __u8 *err)
{
    __s32   ret;
    ret = MsgQueue_Exit(thiz);
    if(EPDK_OK != ret)
    {
        WARNING("MsgQueue Exit() fail\n");
    }
    free(thiz);
}

