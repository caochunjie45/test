
#include "tscDrv.h"
#include "tscHal.h"

//* Use in the dvb_rx_maintask to check if there is a request to quit,
//* if it is, quit the main task.
#define CHECK_EXIT_REQUEST  {\
        if(esKRNL_TDelReq(EXEC_prioself) == OS_TASK_DEL_REQ)\
            goto _exit_rx_main_task;\
        }


__s32 WaitDvbDataReady(void)
{
    __u8    err;
    esKRNL_SemPend(dvb_rx->pSemTsfDataReady, 50, &err);
    if(err == OS_NO_ERR)
        return 0;
    else
        return -1;
}


__s32 DvbDataReady(void)
{
    OS_SEM_DATA tmpSemDat;
    
    esKRNL_SemQuery(dvb_rx->pSemTsfDataReady, &tmpSemDat);
    if(tmpSemDat.OSCnt == 0)
        esKRNL_SemPost(dvb_rx->pSemTsfDataReady);
    return 0;
}

#if 0
void print_channel_info(__u32 chanStatus)
{
    __u32 chan;
    
    chan = 0;
    while(chanStatus)
    {
        //* print information of channel chan.
        if(chanStatus & 0x1)
            print_chan_status(chan);
        chanStatus >>= 1;
        chan++;
    }
}
#endif

void dvb_isr_cb(__u32 port0ChanIntrFlags, __u32 port0PcrIntrFlag, __u32 port1ChanIntrFlags, __u32 potr1PcrIntrFlag)
{
    __u32 pcr;
    
    if(port0ChanIntrFlags)
        dvb_rx->tsf_new_data = 1;
        
    dvb_rx->chanStatus |= port0ChanIntrFlags;
    
    if(port0PcrIntrFlag)
    {
        if(dvb_rx->pcrFilter.isDetectOpened == 1)
        {
            pcr = tsfGetPCR(TSF_PORT_0);
            if(dvb_rx->pcrFilter.callback)
            {
                esKRNL_CallBack(dvb_rx->pcrFilter.callback, (void *)(pcr/45));  //תΪms
            }
            else
            {
                //eLIBs_printf("[%d]\n", pcr/45);
            }
        }
    }
    
    if(port0ChanIntrFlags)
    {
        DvbDataReady();
//        print_channel_info(chanStatus & 0xffff);
    }
        
    return;
}


void dvb_rx_maintask(void *p_arg)
{
    __u32       cpu_sr;
    __u32       chan;
    __dvb_rx_t* p;
    __u32       tmpChanStatus;
    
    p = dvb_rx;
    
#if TSC_DEBUG
    WARNING("dvb_rx_maintask: start loop.\n");
#endif

    while(1)
    {
        CHECK_EXIT_REQUEST
        //* 1. Wait for Data.
        if(WaitDvbDataReady() != 0)
        {
            //* check if tsg has been pause
            continue;
        }
        DisableInterrupt();
        tmpChanStatus       = p->chanStatus;
        p->chanStatus  = 0;
        EnableInterrupt();
        //* 2. Get channel data
        tmpChanStatus &= 0x7fff;    //* don't process pcr data and overlap interrupts.
        chan = 0;
        while(tmpChanStatus)
        {
            if(tmpChanStatus & 0x1)
            {
                if(p->filters[chan].isOpened == 1)
                {
                    if(p->filters[chan].callback)
                        esKRNL_CallBack(p->filters[chan].callback, (void *)p->filters[chan].callbackparam);
                }
            }
            
            tmpChanStatus >>= 1;
            chan++;
        }
    }

_exit_rx_main_task:
#if TSC_DEBUG
    WARNING("tsc module maintask quit.\n");
#endif
    esKRNL_TDel(EXEC_prioself);
}

