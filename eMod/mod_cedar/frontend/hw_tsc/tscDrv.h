
#ifndef TSC_DRV_H
#define TSC_DRV_H

#include "tscConfig.h"
#include "tsc_interface.h"

typedef struct PID_FILTER   //Ӧ���Ƕ�ӦӲ��filter�����ݽṹ,tsc������channel
{
    __u32               isOpened;   //Ӳ��filter�Ƿ����
    __u32               pid;
    __pCBK_t            callback;       //dataArriveCallBack(),֪ͨ���ݵ���
    void*               callbackparam;  //__krnl_event_t*     dataSem;
    
    __u32               chan;   //Ӳ��filter��channel_id
    pid_filter_type_t   type;
    __u8*               buf;    //����pid_filter_type_t��MOpen()ʱ�Ͷ���
    __u8*               bufEnd; //ָ��buf����һ���ֽڣ�Ҳ������һ��buf����ʼ��ַ
    __u32               bufSize;       
}pid_filter_t;


typedef struct PCR_FILTER
{
    __u32               isReservedChanOpened;   //��������Ϊpcr������pcrfilter,���ڶ�Ϊchannel30,(channel��Ŵ�0��ʼ),��1
    __u32               isDetectOpened; //1:����detect pcr
    __u32               attachChan; //����detect pcrpid��channel��id��
    __u32               pid;
    __pCBK_t            callback;
    
    __u32               chan;   //MOpen()ʱ�ͷ������, ������30.���Ҫ������һ��filter��pcr,��ô�������filter.
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

    __krnl_event_t*     pSemTsfDataReady;   //tsf���ж�isr��post,��һ���߳���pend.pend���Ϳ��԰�������
    __krnl_event_t*     pMutexTsf;  //�޸������filters[MAX_CHAN_NUM - 1]ʱ,���뻥��,��semaphore���ڻ���
    
    pid_filter_t        filters[MAX_CHAN_NUM - 1];
    pid_filter_t*       vdChan; //�������filters[]ѡһ����ʼ��ַ
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



