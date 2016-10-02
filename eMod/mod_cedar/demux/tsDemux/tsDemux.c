
#include "tsDemux_s.h"

//#include "tsDemux.h"

__u32 tsc_mid = 0;

__s32 MInit(void)
{
    tsc_mid = esMODS_MInstall(TSC_DRIVER_FILE_PATH, 0);
    if(tsc_mid == 0)
        return -1;
    else
        return 0;
}



__s32 MExit(void)
{
    if(tsc_mid)
        esMODS_MUninstall(tsc_mid);
    tsc_mid = 0;
    return 0;
}

#if (DEBUG_DUMPDATA)
CDumpData       *pCDumpData;
#endif

__mp* MOpen(__u32 mid, __u32 mode)
{
    __mpegts_context_t* mp;
    //__u8                err;
    
    if(tsc_mid == 0)
    {
        WARNING("tsc driver has not been install yet, demux open fail.\n");
        return NULL;
    }
    
    mp = (__mpegts_context_t*)malloc(sizeof(__mpegts_context_t));
    if (!mp)
        return NULL;

    memset(mp, 0, sizeof(__mpegts_context_t));

    mp->mutex = esKRNL_SemCreate(1);
    if (mp->mutex == NULL)
    {
        free(mp);
        return NULL;
    }

//    if(tsc_mid)
//        mp->drvTsc = esMODS_MOpen(tsc_mid, 0);
//    
//    if (mp->drvTsc == NULL)
//    {
//        esKRNL_SemDel(mp->mutex, OS_DEL_ALWAYS, &err);
//        free(mp);
//        return NULL;
//    }

    mp->mid = mid;

#if (DEBUG_DUMPDATA)
    pCDumpData = newCDumpData(DUMPBUF_SIZE);
#endif
    return (__mp*)mp;
}



__s32 MClose(__mp* mp)
{
    __u32 i;
    __u8  err;
    __mpegts_context_t* p;

    p = (__mpegts_context_t*)mp;

    for (i=0; i<NB_PID_MAX; i++)
    {
        if (p->pids[i])
        {
            CloseEntireFilter(p->pids[i], p);
            p->pids[i] = NULL;
        }
    }

    if (p->detectPCR)
    {
        ClosePCRDetect(p->PCRPid, p);
        p->detectPCR = 0;
    }

    if(p->drvTsc)
    {
        WARNING("tsc drv is closed now!\n");
        esMODS_MClose(p->drvTsc);
        p->drvTsc = NULL;
    }
    
    esKRNL_SemDel(p->mutex, OS_DEL_ALWAYS, &err);

#if (DEBUG_DUMPDATA)
    if(pCDumpData)
    {
        deleteCDumpData(pCDumpData);
        pCDumpData = NULL;
    }
#endif     
    free(p);

    return 0;
}



__u32 MRead(void* pdata, __u32 size, __u32 pid, __mp* mp)
{
    __u32               readSize;
    __filter_v2_t*         filter;
    __mpegts_context_t* p;

    p = (__mpegts_context_t*)mp;

    filter = p->pids[pid];

    readSize = 0;
    if(filter)
    {
            readSize = ReadTsPackets(filter, pdata, size);
    }

    return readSize;
}



__u32 MWrite(const void* pdata, __u32 size, __u32 pid, __mp* mp)
{
    return 0;
}



__s32 MIoctrl(__mp* mp, __u32 cmd, __s32 aux, void* pbuffer)
{
    __u32                   pid;
    __mpegts_context_t*     p;
    dvb_dmx_chan_param_t*   par;
    __s32                   nVirtualChanId;

    p = (__mpegts_context_t*)mp;

    switch (cmd)
    {
        case DVB_DMX_CMD_SET_FECONFIG:
        {
            p->pFEConfig = (CFEConfig*)pbuffer;
            if(p->drvTsc)
            {
                WARNING("fatal error! drvtsc[0x%x] is exist!\n", p->drvTsc);
                return EPDK_FAIL;
            }
            return EPDK_OK;
        }
        case DVB_DMX_CMD_OPEN:
        {
            if(p->drvTsc)
            {
                WARNING("why tsc[0x%x] is exist?\n", p->drvTsc);
                return EPDK_OK;
            }
            if(tsc_mid)
                p->drvTsc = esMODS_MOpen(tsc_mid, 0);
            
            if (p->drvTsc == NULL)
            {
                //esKRNL_SemDel(mp->mutex, OS_DEL_ALWAYS, &err);
                //free(mp);
                //return NULL;
                WARNING("fatal error! tsc MOpen fail!\n");
                return EPDK_FAIL;
            }
            esMODS_MIoctrl(p->drvTsc, TSC_CMD_SET_FECONFIG, 0, p->pFEConfig);
            if(EPDK_OK != esMODS_MIoctrl(p->drvTsc, TSC_CMD_OPEN, 0, NULL))
            {
                WARNING("tsc IOOpen fail\n");
                return EPDK_FAIL;
            }
            return EPDK_OK;
        }
        case DVB_DMX_CMD_OPEN_FILTER:
        {
            pid = aux;
            par = (dvb_dmx_chan_param_t*)pbuffer;
            
            if(!p || !par)
                return EPDK_FAIL;
                
            OpenFilter(pid, par, p);
            
            if (p->pids[pid] == NULL)
                return EPDK_FAIL;
            else
                return EPDK_OK;
        }
        
        case DVB_DMX_CMD_CLOSE_FILTER:
        {
            pid = aux;
            nVirtualChanId = (__s32)pbuffer;
            if(0 == nVirtualChanId)
            {
                WARNING("Be careful! DVB_DMX_CMD_CLOSE_FILTER, vchanid = 0, pat?\n");
            }
            if (p->pids[pid])
            {
                CloseFilter(p->pids[pid], nVirtualChanId, p);
                //p->pids[pid] = NULL;
            }
            
            return EPDK_OK;
        }
        
        case DVB_DMX_CMD_OPEN_PCR_DETECT:
        {
            if (p->detectPCR)
            {
                WARNING("fatal error! pcr detect has already open!check code!\n");
                ClosePCRDetect(p->PCRPid, p);
            }

            p->detectPCR    = 1;
            p->PCRPid       = aux;
            p->pcrCallback  = (__pCBK_t)pbuffer;
            OpenPCRDetect(p->PCRPid, p);
            return EPDK_OK;
        }
        
        case DVB_DMX_CMD_CLOSE_PCR_DETECT:
        {
            if (p->detectPCR)
            {
                ClosePCRDetect(p->PCRPid, p);
                p->detectPCR    = 0;
                p->PCRPid       = 0;
                p->pcrCallback  = NULL;
            }
            return EPDK_OK;
        }
        case DVB_DMX_CMD_SET_DEMODDRV:
		{
			p->drvDemod = pbuffer;
			esMODS_MIoctrl(p->drvTsc, TSC_CMD_SET_DEMODDRV, 0, p->drvDemod);
            return EPDK_OK;
        }
    default:
        return EPDK_FAIL;
    }
}

