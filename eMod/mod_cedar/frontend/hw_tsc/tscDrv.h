
#ifndef TSC_DRV_H
#define TSC_DRV_H

#include "tscConfig.h"
#include "tsc_interface.h"

typedef struct PID_FILTER   //应该是对应硬件filter的数据结构,tsc不管虚channel
{
    __u32               isOpened;   //硬件filter是否打开了
    __u32               pid;
    __pCBK_t            callback;       //dataArriveCallBack(),通知数据到来
    void*               callbackparam;  //__krnl_event_t*     dataSem;
    
    __u32               chan;   //硬件filter的channel_id
    pid_filter_type_t   type;
    __u8*               buf;    //根据pid_filter_type_t在MOpen()时就定好
    __u8*               bufEnd; //指向buf的下一个字节，也就是下一块buf的起始地址
    __u32               bufSize;       
}pid_filter_t;


typedef struct PCR_FILTER
{
    __u32               isReservedChanOpened;   //如果真打开了为pcr保留的pcrfilter,现在定为channel30,(channel编号从0开始),置1
    __u32               isDetectOpened; //1:打开了detect pcr
    __u32               attachChan; //真正detect pcrpid的channel的id号
    __u32               pid;
    __pCBK_t            callback;
    
    __u32               chan;   //MOpen()时就分配好了, 现在是30.如果要单独开一个filter拿pcr,那么就用这个filter.
    __u8*               buf;
    __u8*               bufEnd;
    __u32               bufSize;
    
}pcr_filter_t;


typedef struct CHAN32_FILTER
{
    __u32               chan;
    __u8*               buf;
    __u8*               bufEnd;
    __u32               bufSize;
}chan32_filter_t;


typedef struct __DVB_RX
{
    __u32               mid;            //* DVB receive driver module id
    __u8                maintskprio;
    
    __u8                tsf_new_data;
    __u32               chanStatus;

    __krnl_event_t*     pSemTsfDataReady;   //tsf的中断isr会post,另一个线程在pend.pend到就可以搬数据了
    __krnl_event_t*     pMutexTsf;  //修改下面的filters[MAX_CHAN_NUM - 1]时,必须互斥,该semaphore用于互斥
    
    pid_filter_t        filters[MAX_CHAN_NUM - 1];
    pid_filter_t*       vdChan; //从上面的filters[]选一个起始地址
    pid_filter_t*       adChan;
    pid_filter_t*       subtChan;
    pid_filter_t*       sectionChan;
    pid_filter_t*       tsChan;
    
    pcr_filter_t        pcrFilter;
    chan32_filter_t     chan31Filter;
    
    __u8*               buf;
    __u32               totalBufSize;

    CFEConfig           *pFEConfig;

} __dvb_rx_t;

extern __dvb_rx_t *dvb_rx;


//* task in dvb_rx_maintask.c module.
extern void dvb_rx_maintask(void *p_arg);

extern void dvb_isr_cb(__u32 port0ChanIntrFlags, __u32 port0PcrIntrFlag, __u32 port1ChanIntrFlags, __u32 potr1PcrIntrFlag);

extern __s32 WaitDvbDataReady(void);

extern __s32 DvbDataReady(void);

#endif	/* TSC_DRV_H */



