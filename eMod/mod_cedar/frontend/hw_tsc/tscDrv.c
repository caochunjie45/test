/*
*********************************************************************************************************
*                                                    eMOD
*                                   the Easy Portable/Player Operation System
*                                               mod_mmp sub-system
*                                               cmmb receive module
*
*                                    (c) Copyright 2008-2009, clif.c China
*                                               All Rights Reserved
*
* File   : cmmb_rx.c
* Version: V1.0
* By     : clif.c
* Date   : 2009-7-15 18:43
*********************************************************************************************************
*/
#include "tscDrv.h"
#include "tscHal.h"
#include "tscClk.h"


__dvb_rx_t *dvb_rx;

static __inline __s32 lock_filter_mutex(void)
{
    __u8 err;
    
    esKRNL_SemPend(dvb_rx->pMutexTsf, 50, &err);
    
    if(err == OS_NO_ERR)
    {
        return 0;
    }
    else
    {
        return -1;
    }
}

static __inline void unlock_filter_mutex(void)
{
    OS_SEM_DATA tmpSemDat;
    
    esKRNL_SemQuery(dvb_rx->pMutexTsf, &tmpSemDat);
    if(tmpSemDat.OSCnt == 0)
    {
        esKRNL_SemPost(dvb_rx->pMutexTsf);
    }
    else
    {
        WARNING("xxxxxxxxxxxxxxxxxxxxxxxxx more than one semaphore.\n");
    }
}


/*******************************************************************************
Function name: open_filter
Description: 
    
Parameters: 
    
Return: 
    channel_id:Ó²¼þfilterµÄ±àºÅ
Time: 2011/1/5
*******************************************************************************/
static __s32 open_filter(filter_register_t* filter_register)
{
    __u32           chan;
    __u32           maxChanCnt;
    pid_filter_t    *filters;
    
    if(filter_register->type == PID_FILTER_TYPE_VIDEO)
    {
        maxChanCnt  = MAX_VIDEO_CHAN;
        filters     = dvb_rx->vdChan;
    }
    else if(filter_register->type == PID_FILTER_TYPE_AUDIO)
    {
        maxChanCnt  = MAX_AUDIO_CHAN;
        filters     = dvb_rx->adChan;
    }
    else if(filter_register->type == PID_FILTER_TYPE_SUBTITLE)
    {
        maxChanCnt  = MAX_SUBTITLE_CHAN;
        filters     = dvb_rx->subtChan;
    }
    else if(filter_register->type == PID_FILTER_TYPE_SECTION)
    {
        maxChanCnt  = MAX_SECTION_CHAN;
        filters     = dvb_rx->sectionChan;
    }
    else if(filter_register->type == PID_FILTER_TYPE_TS_DATA)
    {
        maxChanCnt  = MAX_TS_DATA_CHAN;
        filters     = dvb_rx->tsChan;
    }
    else
    {
#if TSC_DEBUG
        WARNING("filter type error when open filter.\n");
#endif
        return -1;
    }
    
    //* one pid can not be opened in two different channel, here i check.
    for(chan = 0; chan < MAX_CHAN_NUM - 1; chan++)
    {
        if(dvb_rx->filters[chan].pid == filter_register->pid)
        {
            if(dvb_rx->filters[chan].isOpened == 1)
            {
#if TSC_DEBUG
                WARNING("can not open filter, because filter of the same pid has been opened.\n");
#endif
                return -1;
            }
        }
    }
    
    if(dvb_rx->pcrFilter.isReservedChanOpened == 1)
    {
        if(dvb_rx->pcrFilter.pid == filter_register->pid)
        {
#if TSC_DEBUG
            WARNING("can not open filter, because filter of the same pid has been opened.\n");
#endif
            return -1;
        }
    }
    
    if(lock_filter_mutex() != 0)
    {
#if TSC_DEBUG
        WARNING("lock mutex fail when open filter.\n");
#endif
        return -1;
    }
    
    for(chan = 0; chan < maxChanCnt; chan++)
    {
        if(filters[chan].isOpened == 0)
            break;
    }
    
    if(chan == maxChanCnt)
    {
#if TSC_DEBUG
        WARNING("lock mutex fail when open filter.\n");
#endif
        goto _err_open_filter;
    }
    
    filters[chan].pid           = filter_register->pid;
    filters[chan].callback      = filter_register->callback;
    filters[chan].callbackparam = filter_register->callbackparam;
    
    tsfOpenChan(filters[chan].pid,      //* pid.
                filters[chan].buf,      //* buffer for channel.
                filters[chan].bufSize,  //* the buffer size in bytes.
                TSF_TP_CHAN,            //* TS packet mode.
                TSF_PORT_0,             //* use port 0.
                filters[chan].chan,     //* channel id.
                1);
                
    filters[chan].isOpened = 1;
    
    unlock_filter_mutex();
    
    return filters[chan].chan;
    
_err_open_filter:
    unlock_filter_mutex();
    return -1;
    
    
}


static void close_filter(__u32 chan)
{
    pid_filter_t    *filters;
    
    filters = dvb_rx->filters;
    
    if(lock_filter_mutex() != 0)
    {
#if TSC_DEBUG
        WARNING("lock mutex fail when close filter.\n");
#endif
        return;
    }
    
    if(filters[chan].isOpened == 1)
    {
        filters[chan].isOpened      = 0;
        
        tsfCloseChan(TSF_PORT_0, chan);
        
        filters[chan].pid           = 0;
        filters[chan].callback      = NULL;
        filters[chan].callbackparam = NULL;
    }
    unlock_filter_mutex();
    
    return ;
}


static void open_pcr_detect(__u32 pid, __pCBK_t callback)
{
    __u32           chan;
    pid_filter_t    *filters;
    pcr_filter_t    *pcrFilter;
    
    filters     = dvb_rx->filters;
    pcrFilter   = &dvb_rx->pcrFilter;
    
    if(lock_filter_mutex() != 0)
    {
#if TSC_DEBUG
        WARNING("lock mutex fail when close filter.\n");
#endif
        return;
    }
    
    for(chan = 0; chan < MAX_CHAN_NUM - 2; chan++)
    {
        if((filters[chan].pid == pid) && (filters[chan].isOpened == 1))
            break;
    }
    
    if(pcrFilter->isDetectOpened == 1)
    {
        WARNING("fatal error! tsc_pcr_detect has already open!check code!\n");
        tsfClosePCRDetect(TSF_PORT_0);
        pcrFilter->isDetectOpened = 0;
    }
    
    if(pcrFilter->isReservedChanOpened == 1)
    {
        WARNING("fatal error! tsc_pcr_detect reserved channel has already open!check code!\n");
        tsfCloseChan(TSF_PORT_0, pcrFilter->chan);
        pcrFilter->isReservedChanOpened = 0;
    }
    
    pcrFilter->pid          = pid;
    pcrFilter->callback     = callback;
    pcrFilter->attachChan   = chan;
    
    if(chan == MAX_CHAN_NUM - 2)
    {
        WARNING("Be careful! tsc open reserved PCR channel[%d]! pid[0x%x]\n", chan, pid);
        tsfOpenChan(pid, pcrFilter->buf, pcrFilter->bufSize, TSF_TP_CHAN, TSF_PORT_0, pcrFilter->chan, 0);
        pcrFilter->isReservedChanOpened = 1;
    }
    else
    {
        WARNING("Be careful! PCR channel[%d] has already exist! pid[0x%x]\n", chan, pid);
    }
    
    tsfOpenPCRDetect(TSF_PORT_0, chan);
    pcrFilter->isDetectOpened = 1;
    
    unlock_filter_mutex();
}


static void close_pcr_detect(void)
{
    pcr_filter_t    *pcrFilter;
    
    pcrFilter = &dvb_rx->pcrFilter;
    
    if(lock_filter_mutex() != 0)
    {
#if TSC_DEBUG
        WARNING("lock mutex fail when close filter.\n");
#endif
        return;
    }
    
    if(pcrFilter->isDetectOpened == 1)
    {
        tsfClosePCRDetect(TSF_PORT_0);
        pcrFilter->isDetectOpened = 0;
    }
    
    if(pcrFilter->isReservedChanOpened == 1)
    {
        WARNING("close independent pcrpid channel[%d]!\n", pcrFilter->chan);
        tsfCloseChan(TSF_PORT_0, pcrFilter->chan);
        pcrFilter->isReservedChanOpened = 0;
    }
    else
    {
        WARNING("pcrpid channel[%d] is not independent!", pcrFilter->attachChan);
    }
    
    pcrFilter->pid          = 0;
    pcrFilter->callback     = NULL;
    pcrFilter->attachChan   = 0;
    
    unlock_filter_mutex();
}

static void open_chan_31(void)
{
    if(lock_filter_mutex() != 0)
    {
#if TSC_DEBUG
        WARNING("lock mutex fail when close filter.\n");
#endif
        return;
    }
    
    tsfOpenChan(PID_ALL, dvb_rx->chan31Filter.buf, dvb_rx->chan31Filter.bufSize, TSF_TP_CHAN, TSF_PORT_0, 31, 0);
    
    unlock_filter_mutex();
}

static void close_chan_31(void)
{
    tsfCloseChan(TSF_PORT_0, 31);
}


/*
*********************************************************************************************************
*                       DVB RECEIVE DRIVER MODULE INIT
*
*Description: dvb receive driver module init.
*
*Arguments  : None.
*
*Return     : Always return successful.
*
*/
__s32 MInit(void)
{
    dvb_rx = (__dvb_rx_t *)0;
    
    return EPDK_OK;
}


/*
*********************************************************************************************************
*                       DVB RECEIVE DRIVER MODULE EXIT
*
*Description: dvb receive driver module exit.
*
*Arguments  : None.
*
*Return     : Return successful if the driver has been closed.
*
*/
__s32 MExit(void)
{
    if(dvb_rx)
    {
#if TSC_DEBUG
        WARNING("tsc module has not been close yet, MExit fail.\n");
#endif
        return EPDK_FAIL;
    }
    
    return EPDK_OK;
}


/*
*********************************************************************************************************
*                       DVB RECEIVE DRIVER MODULE OPEN
*
*Description: cmmb receive driver module open.
*
*Arguments  : mid   the module id of dvb receive driver;
*             mode  the type of open.
*
*Return     : the handle of the video decode driver.
*                   != 0    open video decode driver module successful;
*                   == 0    open video decode driver module fialed.
*
*/

static __u32 sizeMulArr[4] = {2, 4, 8, 16};
__mp *MOpen(__u32 mid, __u32 mode)
{
    __u8  err;
    __u32 chan;
    __u8* pBuf;
    __u32 totalBufferSize;
    __u32 videoBufferSize;
    __u32 audioBufferSize;
    __u32 subtitleBufferSize;
    __u32 sectionBufferSize;
    __u32 tsBufferSize;
    __u32 pcrBufferSize;
    __u32 chan32BufferSize;
    __u32 sizeMultiply;
        
    pBuf = NULL;

    if(dvb_rx)
    {
#if TSC_DEBUG
        WARNING("tsc module has been opened already, MOpen fail.\n");
#endif
        return (__mp*)0;
    }

    //* create module handle
    dvb_rx = (__dvb_rx_t *)malloc(sizeof(__dvb_rx_t));
    if(dvb_rx == NULL)
    {
#if TSC_DEBUG
        WARNING("malloc fail, MOpen fail.\n");
#endif
        goto _err_open;
    }
    
    memset(dvb_rx, 0, sizeof(__dvb_rx_t));
    dvb_rx->mid       = mid;
    
    //* create semaphore for waitint tsf data
    dvb_rx->pSemTsfDataReady = esKRNL_SemCreate(0);
    if(!dvb_rx->pSemTsfDataReady)
    {
#if TSC_DEBUG
        WARNING("create semaphore fail, MOpen fail.\n");
#endif
        goto _err_open;
    }

    //* create mutex for tsf control.
    dvb_rx->pMutexTsf = esKRNL_SemCreate(1);
    if(!dvb_rx->pMutexTsf)
    {
#if TSC_DEBUG
        WARNING("create mutex fail, MOpen fail.\n");
#endif
        goto _err_open;
    }
    
    //* malloc memory for all pid filter.
    sizeMultiply = sizeMulArr[TSF_INTR_THRESHOLD];

    videoBufferSize     = (VIDEO_NOTIFY_PKT_NUM_HW * 188 * sizeMultiply + 0x3ff) & ~0x3ff;
    audioBufferSize     = (AUDIO_NOTIFY_PKT_NUM * 188 * sizeMultiply + 0x3ff) & ~0x3ff;
    subtitleBufferSize  = (SUBTITLE_NOTIFY_PKT_NUM * 188 * sizeMultiply + 0x3ff) & ~0x3ff;
    sectionBufferSize   = (SECTION_NOTIFY_PKT_NUM * 188 * sizeMultiply + 0x3ff) & ~0x3ff;
    tsBufferSize        = (TS_DATA_NOTIFY_PKT_NUM * 188 * sizeMultiply + 0x3ff) & ~0x3ff;
    pcrBufferSize       = (1 * 188 * sizeMultiply + 0x3ff) & ~0x3ff;
    chan32BufferSize    = (1 * 188 * sizeMultiply + 0x3ff) & ~0x3ff;
    
    totalBufferSize = videoBufferSize * MAX_VIDEO_CHAN + 
                      audioBufferSize * MAX_AUDIO_CHAN + 
                      subtitleBufferSize * MAX_SUBTITLE_CHAN + 
                      sectionBufferSize * MAX_SECTION_CHAN + 
                      tsBufferSize * MAX_TS_DATA_CHAN + 
                      pcrBufferSize + 
                      chan32BufferSize;

    pBuf = (__u8*)palloc((totalBufferSize+1023)/1024, 0); //MEMS_PALLOC_MODE_BND_NONE | MEMS_PALLOC_MODE_BNK_NONE | MEMS_PALLOC_MODE_AREA_NONE
    if(pBuf == NULL)
    {
#if TSC_DEBUG
        WARNING("palloc fail, MOpen fail.\n");
#endif
        goto _err_open;
    }
    
    dvb_rx->buf             = pBuf;
    dvb_rx->totalBufSize    = totalBufferSize;
    
    dvb_rx->vdChan          = &dvb_rx->filters[VIDEO_CHAN_BASE];
    dvb_rx->adChan          = &dvb_rx->filters[AUDIO_CHAN_BASE];
    dvb_rx->subtChan        = &dvb_rx->filters[SUBTITLE_CHAN_BASE];
    dvb_rx->sectionChan     = &dvb_rx->filters[SECTION_CHAN_BASE];
    dvb_rx->tsChan          = &dvb_rx->filters[TS_DATA_CHAN_BASE];
    
    for(chan = 0; chan < MAX_VIDEO_CHAN; chan++)
    {
        dvb_rx->vdChan[chan].chan       = chan + VIDEO_CHAN_BASE;
        dvb_rx->vdChan[chan].type       = PID_FILTER_TYPE_VIDEO;
        dvb_rx->vdChan[chan].buf        = pBuf;
        dvb_rx->vdChan[chan].bufEnd     = pBuf + VIDEO_NOTIFY_PKT_NUM_HW * 188 * sizeMultiply;
        dvb_rx->vdChan[chan].bufSize    = VIDEO_NOTIFY_PKT_NUM_HW * sizeMultiply * 188;
        pBuf += ((videoBufferSize + 0x3f) & ~0x3f);
    }
    
    for(chan = 0; chan < MAX_AUDIO_CHAN; chan++)
    {
        dvb_rx->adChan[chan].chan       = chan + AUDIO_CHAN_BASE;
        dvb_rx->adChan[chan].type       = PID_FILTER_TYPE_AUDIO;
        dvb_rx->adChan[chan].buf        = pBuf;
        dvb_rx->adChan[chan].bufEnd     = pBuf + AUDIO_NOTIFY_PKT_NUM * 188 * sizeMultiply;
        dvb_rx->adChan[chan].bufSize    = AUDIO_NOTIFY_PKT_NUM * sizeMultiply * 188;
        pBuf += ((audioBufferSize + 0x3f) & ~0x3f);
    }
    
    for(chan = 0; chan < MAX_SUBTITLE_CHAN; chan++)
    {
        dvb_rx->subtChan[chan].chan     = chan + SUBTITLE_CHAN_BASE;
        dvb_rx->subtChan[chan].type     = PID_FILTER_TYPE_SUBTITLE;
        dvb_rx->subtChan[chan].buf      = pBuf;
        dvb_rx->subtChan[chan].bufEnd   = pBuf + SUBTITLE_NOTIFY_PKT_NUM * 188 * sizeMultiply;
        dvb_rx->subtChan[chan].bufSize  = SUBTITLE_NOTIFY_PKT_NUM * sizeMultiply * 188;
        pBuf += ((subtitleBufferSize + 0x3f) & ~0x3f);
    }
    
    for(chan = 0; chan < MAX_SECTION_CHAN; chan++)
    {
        dvb_rx->sectionChan[chan].chan      = chan + SECTION_CHAN_BASE;
        dvb_rx->sectionChan[chan].type      = PID_FILTER_TYPE_SECTION;
        dvb_rx->sectionChan[chan].buf       = pBuf;
        dvb_rx->sectionChan[chan].bufEnd    = pBuf + SECTION_NOTIFY_PKT_NUM * 188 * sizeMultiply;
        dvb_rx->sectionChan[chan].bufSize   = SECTION_NOTIFY_PKT_NUM * sizeMultiply * 188;
        pBuf += ((sectionBufferSize + 0x3f) & ~0x3f);
    }
    
    for(chan = 0; chan < MAX_TS_DATA_CHAN; chan++)
    {
        dvb_rx->tsChan[chan].chan       = chan + TS_DATA_CHAN_BASE;
        dvb_rx->tsChan[chan].type       = PID_FILTER_TYPE_TS_DATA;
        dvb_rx->tsChan[chan].buf        = pBuf;
        dvb_rx->tsChan[chan].bufEnd     = pBuf + TS_DATA_NOTIFY_PKT_NUM * 188 * sizeMultiply;
        dvb_rx->tsChan[chan].bufSize    = TS_DATA_NOTIFY_PKT_NUM * sizeMultiply * 188;
        pBuf += ((sectionBufferSize + 0x3f) & ~0x3f);
    }
    
    dvb_rx->pcrFilter.chan      = MAX_CHAN_NUM - 2;
    dvb_rx->pcrFilter.buf       = pBuf;
    dvb_rx->pcrFilter.bufEnd    = pBuf + 1 * 188 * sizeMultiply;
    dvb_rx->pcrFilter.bufSize   = 1 * sizeMultiply * 188;
    
    pBuf += (((1 * 188 * sizeMultiply) + 0x3f) & ~0x3f);
    
    dvb_rx->chan31Filter.chan    = MAX_CHAN_NUM - 1;
    dvb_rx->chan31Filter.buf     = pBuf;
    dvb_rx->chan31Filter.bufEnd  = pBuf + 1 * 188 * sizeMultiply;
    dvb_rx->chan31Filter.bufSize = 1 * sizeMultiply * 188;
    
    pBuf += (((1 * 188 * sizeMultiply) + 0x3f) & ~0x3f);
    
    
    //* crate the main task
    dvb_rx->maintskprio   = esKRNL_TCreate(dvb_rx_maintask,
                                           0,
                                           0x1000,
                                           (dvb_rx->mid << 8) | KRNL_priolevel2);

    if(!dvb_rx->maintskprio)
    {
#if TSC_DEBUG
        WARNING("create maintask fail, MOpen fail.\n");
#endif
        goto _err_open;
    }

    //* install tsc isr
    esINT_InsISR(INTC_IRQNO_TS, (__pISR_t)tscISR, (__s32 *)0);
    esINT_EnableINT(INTC_IRQNO_TS);
    
    //* enable hardware
    //tscInit();
    //open_chan_31();
    
    //* install interrupt service function
    //tscSetIsrCbFunc(dvb_isr_cb);

    return (__mp *)dvb_rx;
    
_err_open:
    if(dvb_rx)
    {
        //* release memory.
        if(dvb_rx->buf)
        {
            pfree(dvb_rx->buf, (dvb_rx->totalBufSize+1023)/1024);
            dvb_rx->buf = NULL;
        }
        
        //* release semaphore
        if(dvb_rx->pSemTsfDataReady)
        {
            esKRNL_SemDel(dvb_rx->pSemTsfDataReady, OS_DEL_ALWAYS, &err);
            dvb_rx->pSemTsfDataReady = NULL;
        }
        
        //* release mutex
        if(dvb_rx->pMutexTsf)
        {
            esKRNL_SemDel(dvb_rx->pMutexTsf, OS_DEL_ALWAYS, &err);
            dvb_rx->pMutexTsf = NULL;
        }
        
        free(dvb_rx);
        dvb_rx = NULL;
    }

    return NULL;
}


/*
*********************************************************************************************************
*                       DVB RECEIVE DRIVER MODULE CLOSE
*
*Description: dvb receive driver module close.
*
*Arguments  : mp    the handle of the dvb driver;
*
*Return     : Always return successful.
*
*/
__s32 MClose(__mp *mp)
{
    __u8  err;

    if(mp != (__mp *)dvb_rx)
    {
#if TSC_DEBUG
        WARNING("tsc module MClose: module handle is invalid when close!");
#endif
        return EPDK_FAIL;
    }

    if(!dvb_rx)
    {
#if TSC_DEBUG
        WARNING("tsc module MClose: receiver has been closed by some one, error!\n");
#endif
        return EPDK_FAIL;
    }

    if(dvb_rx)
    {
        //* disable hardware.
        close_chan_31();
        tscExit();

        if(dvb_rx->maintskprio)
        {
            esKRNL_TimeDlyResume((dvb_rx->mid << 8) | dvb_rx->maintskprio);
            while(esKRNL_TDelReq((dvb_rx->mid << 8) | dvb_rx->maintskprio) != OS_TASK_NOT_EXIST)
            {
                esKRNL_TimeDlyResume((dvb_rx->mid << 8) | dvb_rx->maintskprio);
                esKRNL_TimeDly(1);
            }
            dvb_rx->maintskprio = 0;
        }

        //* release memory.
        if(dvb_rx->buf)
        {
            pfree(dvb_rx->buf, (dvb_rx->totalBufSize+1023)/1024);
            dvb_rx->buf = NULL;
        }
        
        //* release semaphore
        if(dvb_rx->pSemTsfDataReady)
        {
            esKRNL_SemDel(dvb_rx->pSemTsfDataReady, OS_DEL_ALWAYS, &err);
            dvb_rx->pSemTsfDataReady = NULL;
        }
        
        //* release mutex
        if(dvb_rx->pMutexTsf)
        {
            esKRNL_SemDel(dvb_rx->pMutexTsf, OS_DEL_ALWAYS, &err);
            dvb_rx->pMutexTsf = NULL;
        }
        
        free(dvb_rx);
        dvb_rx = NULL;

        //* uninstall tsc isr
        esINT_DisableINT(INTC_IRQNO_TS);
        esINT_UniISR(INTC_IRQNO_TS, (__pISR_t)tscISR);
    }
    
    return EPDK_OK;
}


/*
*********************************************************************************************************
*                       DVB RECEIVE DRIVER MODULE READ
*
*Description: cmmb receive driver read, do nothing.
*
*Arguments  :
*
*Return     :
*
*/
__u32 MRead(void *pdata, __u32 pktNumRequire, __u32 chan, __mp *mp)
{
    __u8* pTsData0;
    __u8* pTsData1;
    __u32 pktNum0;
    __u32 pktNum1;
    __u8* pDst;
    
    if(mp != (__mp *)dvb_rx)
    {
        WARNING("dvb receiver MRead: module handle is invalid when close!");
        return 0;
    }
    
    if(chan >= MAX_CHAN_NUM)
        return 0;
        
    tsfRequestData(TSF_PORT_0, chan, &pTsData0, &pktNum0, &pTsData1, &pktNum1);
    pktNum0 /= 188;
    pktNum1 /= 188;
    
    if(pktNum0 + pktNum1 == 0)
        return 0;
        
    if(pktNumRequire == 0)
        return 0;
    
    pDst = (__u8*)pdata;
    
    if(pktNum0 >= pktNumRequire)
    {
        memcpy(pDst, pTsData0, pktNumRequire * 188);
        tsfFlushChan(TSF_PORT_0, chan, pktNumRequire * 188);
        return pktNumRequire;
    }
    else if(pktNum1 == 0)
    {
        memcpy(pDst, pTsData0, pktNum0 * 188);
        tsfFlushChan(TSF_PORT_0, chan, pktNum0 * 188);
        return pktNum0;
    }
    else if(pktNum0 + pktNum1 >= pktNumRequire)
    {
        memcpy(pDst, pTsData0, pktNum0 * 188);
        memcpy(pDst + pktNum0 * 188, 
               pTsData1, 
               (pktNumRequire-pktNum0) * 188);
        tsfFlushChan(TSF_PORT_0, chan, pktNumRequire * 188);
        return pktNumRequire;
    }
    else
    {
        memcpy(pDst, pTsData0, pktNum0 * 188);
        memcpy(pDst + pktNum0 * 188, 
               pTsData1, 
               pktNum1 * 188);
               
        tsfFlushChan(TSF_PORT_0, chan, (pktNum0 + pktNum1) * 188);
        
        return (pktNum0+pktNum1);
    }
}


/*
*********************************************************************************************************
*                       DVB RECEIVE DRIVER MODULE WRITE
*
*Description: cmmb receive driver write, do nothing.
*
*Arguments  :
*
*Return     :
*
*/
__u32 MWrite(const void *pdata, __u32 size, __u32 n, __mp *mp)
{
    return n;
}


/*
*********************************************************************************************************
*                       DVB RECEIVE DRIVER MODULE IO CONTROL
*
*Description: cmmb receive driver module IO control, process command from other module.
*
*Arguments  : mp        video decode driver handle;
*             cmd       the command need send to video decode driver;
*             aux       user define;
*             pbuffer   user define.
*
*Return     :
*
*/           

__s32 MIoctrl(__mp *mp, __u32 cmd, __s32 aux, void *pbuffer)
{
    __dvb_rx_t *p = (__dvb_rx_t*) mp;
    if(!p || (p != dvb_rx))
    {
#if TSC_DEBUG
        WARNING("invalid tsc module handle, MIoctrl fail.\n");
#endif
        return EPDK_FAIL;
    }

    switch(cmd)
    {
        case TSC_CMD_SET_FECONFIG:
        {
            p->pFEConfig = (CFEConfig*)pbuffer;
            return EPDK_OK;
        }
        case TSC_CMD_OPEN:
        {
            //* enable hardware
            tscInit();
            open_chan_31();
            
            //* install interrupt service function
            tscSetIsrCbFunc(dvb_isr_cb);
            return EPDK_OK;
        }
        case TSC_CMD_OPEN_CHAN:
        {
            filter_register_t * filter_register;
            filter_register = (filter_register_t*)pbuffer;
            
            return open_filter(filter_register);
        }
        
        case TSC_CMD_CLOSE_CHAN:
        {
            __u32 chan;
            chan = aux;
            close_filter(chan);
            return EPDK_OK;
        }
        
        case TSC_CMD_REQUEST_CHAN_DATA:
        {
			__u32 		chan;
			tsf_data_t	*pTsfData;

			chan        = (__u32)aux;
			pTsfData    = (tsf_data_t*)pbuffer;
			
    		if(chan >= (MAX_CHAN_NUM-1))
        		return EPDK_FAIL;
        		
            if(lock_filter_mutex() != 0)
                return EPDK_FAIL;
                
            if(p->filters[chan].isOpened == 1)
            {
		        tsfRequestData(TSF_PORT_0, chan, &pTsfData->pData, &pTsfData->pktNum, &pTsfData->pRingData, &pTsfData->ringPktNum);
		        pTsfData->pktNum /= 188;
		        pTsfData->ringPktNum  /= 188;
		        unlock_filter_mutex();
		        return EPDK_OK;
		    }
		    else
		    {
		        pTsfData->pData      = NULL;
		        pTsfData->pktNum     = 0;
		        pTsfData->pRingData  = NULL;
		        pTsfData->ringPktNum = 0;
		        unlock_filter_mutex();
		        return EPDK_FAIL;
		    }
        }
        
        case TSC_CMD_FLUSH_CHAN_DATA:
        {
			__u32 chan;
			__u32 pktNum;
			
			chan 	= (__u32)aux;
			pktNum  = (__u32)pbuffer;
			
    		if(chan >= (MAX_CHAN_NUM-1))
        		return EPDK_FAIL;

            if(lock_filter_mutex() != 0)
                return EPDK_FAIL;
            if(p->filters[chan].isOpened == 1)
			    tsfFlushChan(TSF_PORT_0, chan, pktNum * 188);
			unlock_filter_mutex();
			
			return EPDK_OK;
        }
        
        case TSC_CMD_GET_CHAN_STATUS:
        {
            __s32 result;
            __u32 chan;
            
            chan = aux;
            if(chan >=  (MAX_CHAN_NUM-1))
                return EPDK_FAIL;
                
            if(lock_filter_mutex() != 0)
                return 0;
            if(p->filters[chan].isOpened == 1)
            {
                result = tsfCheckDataSize(TSF_PORT_0, chan);
                result /= 188;
            }
            else
                result = 0;
            unlock_filter_mutex();
            return result;
        }
        
        case TSC_CMD_OPEN_PCR_DETECT:
        {
            __u32       pid;
            __pCBK_t    callback;
            
            pid      = aux;
            callback = (__pCBK_t)pbuffer;
            open_pcr_detect(pid, callback);
            return EPDK_OK;
        }
        
        case TSC_CMD_CLOSE_PCR_DETECT:
        {
            close_pcr_detect();
            return EPDK_OK;
        }
        case TSC_CMD_SET_DEMODDRV:
		{
			return EPDK_OK;
        }
        default:
        {
#if TSC_DEBUG
            WARNING("invalid tsc command, MIoctrl fail.\n");
#endif
            return EPDK_FAIL;
        }
    }
}

