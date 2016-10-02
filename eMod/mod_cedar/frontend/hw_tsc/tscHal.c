
#include "tscHal.h"
#include "tscRegs.h"
#include "tscClk.h"

#include "tscDrv.h"

#define TSC_IO_BASE         0x01c04000
#define SetValue(X, value)  (*((__u32*)&(X)) = (value))
#define readl(reg)			(*(volatile unsigned long *)(reg))

typedef struct PORT_STATUS_T
{
    __u32   chanOpenFlag;   //本来是一个bit对应一个filter.代码暂时写错了,但没有用到,无所谓.
    __u32   chanPesFlag;    //一个bit对应一个filter,表示该filter是不是取pes包的,代码暂时写错了,但没有用到,无所谓
    __u32   chanBufSize[TSC_CHAN_NUM];
    __u8*   chanBuf[TSC_CHAN_NUM];
    __u8*   chanBufEnd[TSC_CHAN_NUM];
}port_status_t;

typedef struct TSC_CONTEXT_T
{
    tsc_reg_list_t*     tscRegList;
    TSC_ISR_CB          isrCbFunc;
    port_status_t       port[2];
}tsc_ctx_t;

tsc_ctx_t   gTscCtx;


#if USE_MMU
static __inline __u32 getPAddr(__u32 vAddr)
{
    return (__u32)VirAddr2PhyAddr(vAddr);
}

static __inline __u32 getVAddr(__u32 pAddr)
{
    pAddr |= 0xf0000000;
    return pAddr;
}
#endif


static __inline tsc_reg_list_t* getTscRegList(void)
{
#if USE_MMU
    return (tsc_reg_list_t*)getVAddr(TSC_IO_BASE);
#else
    return (tsc_reg_list_t*)TSC_IO_BASE;
#endif
}

//************************* TSC functions **************************//

void  tscInit(void)
{
    __s32           i;
    __u32           tmp;
	__u32 ret;
    tsc_reg_list_t* pRegList;
    
    memset(&gTscCtx, 0, sizeof(gTscCtx));
    
    //* open clock to tsc module.
    TSC_OpenClk();
    //* delay some time for clock stable.
    for(i=0; i<100; i++);       
    
    //* get the register list of TS controller
    pRegList = getTscRegList();
    gTscCtx.tscRegList = pRegList;

    //* set tsc _10_port_ctrl.
//#if (TS_INTERFACE_TYPE == 0)
    if(0 == dvb_rx->pFEConfig->nTS_INTERFACE_TYPE)
    {
        SetValue(pRegList->tsc._10_port_ctrl, 0x0);     //* enable data receive, use spi port, both for port0 and port1.
		*((volatile __u32 *)(0xf1c20890)) = 0x22222222;
		*((volatile __u32 *)(0xf1c20894)) = 0x00002222;	
	}
//#else
    else
    {
        __u32 val = 0;
        
        SetValue(pRegList->tsc._10_port_ctrl, 0x3);     //* enable data receive, use ssi port, both for port0 and port1.
//SSI管脚使能复用功能
//aw1620
//	*((volatile __u32 *)(0xf1c20894)) = 0x22220000;
//	*((volatile __u32 *)(0xf1c20898)) = 0x00000002;		
//aw1625
    #if 0
	    *((volatile __u32 *)(0xf1c20890)) = 0x00022222;
    #else //串行TS流的时候其他 没有用到的IO口
        WARNING("0xf1c20890:0x%x, 0xf1c20894:0x%x\n", *((volatile __u32 *)(0xf1c20890)), *((volatile __u32 *)(0xf1c20894)));
        //D1-D7 are used as normal IO port, so can't change their value 
        val = *((volatile __u32 *)(0xf1c20890));
        val &= 0xfff00000;
        val |= 0x00022222;
        *((volatile __u32 *)(0xf1c20890)) = val;        
        WARNING("0xf1c20890:0x%x, 0xf1c20894:0x%x\n", *((volatile __u32 *)(0xf1c20890)), *((volatile __u32 *)(0xf1c20894)));
    #endif
    }
//#endif

    //* set tsc _14_port_param.
    tmp = 0;
//#if (TS_INTERFACE_PSYNC_SIGNAL_POLARITY == 1)  //* Data Valid signal is low level active.
    if(1 == dvb_rx->pFEConfig->TS_INTERFACE_PSYNC_SIGNAL_POLARITY)
    {
        tmp |= 0x0101;
    }
//#endif
//#if (TS_INTERFACE_DATA_VALID_SIGNAL_POLARITY == 1)  //* Data Valid signal is low level active.
    if(1 == dvb_rx->pFEConfig->TS_INTERFACE_DATA_VALID_SIGNAL_POLARITY)
    {
        tmp |= 0x0202;
    }
//#endif
//#if (TS_INTERFACE_ERROR_SIGNAL_POLARITY == 1)       //* Error signal is low level active.
    if(1 == dvb_rx->pFEConfig->TS_INTERFACE_ERROR_SIGNAL_POLARITY)
    {
        tmp |= 0x0404;
    }
//#endif
//#if (TS_INTERFACE_CLOCK_SIGNAL_POLARITY == 1)       //* Falling edge capture.
    if(1 == dvb_rx->pFEConfig->TS_INTERFACE_CLOCK_SIGNAL_POLARITY)
    {
        tmp |= 0x0808;
    }
//#endif
//#if (TS_INTERFACE_SSI_DATA_ORDER == 1)              //* LSB first when using SSI mode.
    if(1 == dvb_rx->pFEConfig->TS_INTERFACE_SSI_DATA_ORDER)
    {
        tmp |= 0x1010;
    }
//#endif
    SetValue(pRegList->tsc._14_port_param, tmp);
    
    //* set tsc _20_in_mux_ctrl. 
    //* both port0 and port1 get input from input port 0, to support sigle tuner recording.
    SetValue(pRegList->tsc._20_in_mux_ctrl, 0x11);
//    SetValue(pRegList->tsc._20_in_mux_ctrl, 0x22);  //研科方案ts1连接demod, ts0连接csi.
    
    //* eanble tsf0 and tsf1.
    pRegList->tsf[0]._00_ctrl.enable = 1;
    pRegList->tsf[0]._00_ctrl.reset  = 1;

	ret = readl(0xf1c20060);
    WARNING("reg 0xf1c20060 = %x\n",ret);
	ret = readl(0xf1C20100);
    WARNING("reg 0xf1C20100 = %x\n",ret);
	ret = readl(0xf1C20098);
    WARNING("reg 0xf1C20098 = %x\n",ret);
	ret = readl(0xf1C012A4);
    WARNING("reg 0xf1C012A4 = %x\n",ret);

		
    while(pRegList->tsf[0]._00_ctrl.reset);
    
    pRegList->tsf[1]._00_ctrl.enable = 1;
    pRegList->tsf[1]._00_ctrl.reset  = 1;
    while(pRegList->tsf[1]._00_ctrl.reset);
    
    //* set tsf _04_pkt_param
    tmp = 0;
    WARNING("TS_INTERFACE_PACKET_SYNC_METHOD = %d\n", dvb_rx->pFEConfig->TS_INTERFACE_PACKET_SYNC_METHOD);
//#if (TS_INTERFACE_PACKET_SYNC_METHOD == 1)          //* by 0x47 sync byte.
    if(1 == dvb_rx->pFEConfig->TS_INTERFACE_PACKET_SYNC_METHOD)
    {
        tmp |= (1<<8);
    }
//#elif (TS_INTERFACE_PACKET_SYNC_METHOD == 2)        //* by both PSync signal and sync byte.
    else if(2 == dvb_rx->pFEConfig->TS_INTERFACE_PACKET_SYNC_METHOD)
    {
        tmp |= (2<<8);
    }
//#endif
    tmp |= 0x47<<16;    //* sync byte value.
    tmp |= 0x22<<24;    //* two packet to lock synchronization and unlock synchronization.
    SetValue(pRegList->tsf[0]._04_pkt_param, tmp);
    SetValue(pRegList->tsf[1]._04_pkt_param, tmp);
    
    //* set tsf _08_status to enable global DMA interrupt
    pRegList->tsf[0]._08_status.dma_intr_en = 1;    //* only enable dma interrupt, disable overlap interrupt.
    pRegList->tsf[1]._08_status.dma_intr_en = 1;    //* only enable dma interrupt, disable overlap interrupt.
    
    for(i=0; i<32; i++)
    {
        pRegList->tsf[0]._3c_chan_idx.chan_idx  = i;
        pRegList->tsf[0]._58_chan_wt_pos.wt_pos = 0;
        pRegList->tsf[0]._5c_chan_rd_pos.rd_pos = 0;
#if NEW_SWITCH_METHOD
        SetValue(pRegList->tsf[0]._4c_chan_pid, (0<<16 | 0x1fff));
#endif
        pRegList->tsf[1]._3c_chan_idx.chan_idx  = i;
        pRegList->tsf[1]._58_chan_wt_pos.wt_pos = 0;
        pRegList->tsf[1]._5c_chan_rd_pos.rd_pos = 0;
#if NEW_SWITCH_METHOD
        SetValue(pRegList->tsf[1]._4c_chan_pid, (0<<16 | 0x1fff));
#endif
        pRegList->tsf[0]._50_chan_buf_addr.addr = 0;
        pRegList->tsf[0]._54_chan_buf_size.size = 0;
        pRegList->tsf[1]._50_chan_buf_addr.addr = 0;
        pRegList->tsf[1]._54_chan_buf_size.size = 0;
    }
    
#if NEW_SWITCH_METHOD
    SetValue(pRegList->tsf[0]._30_chan_en, 0xffffffff);
    SetValue(pRegList->tsf[1]._30_chan_en, 0xffffffff);
#endif

    return;
}


void  tscExit(void)
{
    if(gTscCtx.tscRegList != NULL)
    {
        //* disable tsf0 and tsf1.
        gTscCtx.tscRegList->tsf[0]._00_ctrl.enable = 0;
        gTscCtx.tscRegList->tsf[1]._00_ctrl.enable = 0;
        esKRNL_TimeDly(1);      //* delay some time for dma dram command finish.
        TSC_CloseClk();
    }
    
    return;
}


void  tscSetIsrCbFunc(TSC_ISR_CB cb)
{
    gTscCtx.isrCbFunc = cb;
    return;
}


void  tscISR(void* param)
{
    __u32           port0ChanIntrFlags;
    __u32           port0PcrIntrFlag;
    __u32           port1ChanIntrFlags;
    __u32           port1PcrIntrFlag;
    tsc_reg_list_t* pRegList;
    
    pRegList = gTscCtx.tscRegList;
    //读寄存器
    port0PcrIntrFlag   = pRegList->tsf[0]._08_status.pcr_found;
    port0ChanIntrFlags = pRegList->tsf[0]._18_dma_status.dma_status;
    port1PcrIntrFlag   = pRegList->tsf[1]._08_status.pcr_found;
    port1ChanIntrFlags = pRegList->tsf[1]._18_dma_status.dma_status;
    //清寄存器
    pRegList->tsf[0]._18_dma_status.dma_status = port0ChanIntrFlags;
    //pRegList->tsf[0]._08_status                = pRegList->tsf[0]._08_status; //禁止用这种方式,会被编译器优化掉.导致没有写寄存器.
    pRegList->tsf[0]._08_status.pcr_found      = port0PcrIntrFlag;
    pRegList->tsf[1]._18_dma_status.dma_status = port1ChanIntrFlags;
    //pRegList->tsf[1]._08_status                = pRegList->tsf[1]._08_status;
    pRegList->tsf[1]._08_status.pcr_found      = port1PcrIntrFlag;
    
    if(gTscCtx.isrCbFunc)
        gTscCtx.isrCbFunc(port0ChanIntrFlags, port0PcrIntrFlag, port1ChanIntrFlags, port1PcrIntrFlag);
}


//************************* TSF functions **************************//
__s32 tsfOpenChan(
            __u32           pid, 
            __u8*           pBuf, 
            __u32           bufSize, 
            tsf_chan_type_e chanType, 
            tsf_port_e      portId, 
            __u32           chanId,
            __u32           intrEnable
      )
{
    tsc_reg_list_t* pRegList;
    
    pRegList = gTscCtx.tscRegList;
    
    gTscCtx.port[portId].chanOpenFlag        = 1;
    gTscCtx.port[portId].chanPesFlag         = (chanType == TSF_PES_CHAN) ? 1 : 0;
    gTscCtx.port[portId].chanBufSize[chanId] = bufSize;
    gTscCtx.port[portId].chanBuf[chanId]     = pBuf;
    gTscCtx.port[portId].chanBufEnd[chanId]  = pBuf + bufSize;
    
    //* 1. set tsf _3c_chan_idx to choose a channel.
    pRegList->tsf[portId]._3c_chan_idx.chan_idx = chanId;
    
#if !NEW_SWITCH_METHOD
    //* 2. set tsf _4c_chan_pid to set pid value.
    if(pid == PID_ALL)
        SetValue(pRegList->tsf[portId]._4c_chan_pid, 0);    //* pass all pid
    else
        SetValue(pRegList->tsf[portId]._4c_chan_pid, (0x1fff<<16 | pid));
#endif
    
    //* 3. set tsf _50_chan_buf_addr to set buffer address.
#if USE_MMU
    pRegList->tsf[portId]._50_chan_buf_addr.addr = getPAddr((__u32)pBuf);
#else
    pRegList->tsf[portId]._50_chan_buf_addr.addr = (__u32)pBuf;
#endif
    
    //* 4. set tsf _54_chan_buf_size to set buffer size and interrupt threshold.
    SetValue(pRegList->tsf[portId]._54_chan_buf_size, (TSF_INTR_THRESHOLD<<24 | (bufSize-16)));
    
    //* 5. set tsf _5c_chan_rd_pos.
    pRegList->tsf[portId]._5c_chan_rd_pos.rd_pos = pRegList->tsf[portId]._58_chan_wt_pos.wt_pos;
    
    //* 6. set tsf _10_dma_intr_en to enable channel dma interrupt.
    if(intrEnable)
        pRegList->tsf[portId]._10_dma_intr_en.en_ctrl_bits |= (1<<chanId);
    else
        pRegList->tsf[portId]._10_dma_intr_en.en_ctrl_bits &= ~(1<<chanId);
    
    //* 7. set tsf _34_chan_pes_en to enable or disable pes mode.
    if(chanType == TSF_PES_CHAN)
        pRegList->tsf[portId]._34_chan_pes_en.pes_en_ctrl_bits |= (1<<chanId);
    else
        pRegList->tsf[portId]._34_chan_pes_en.pes_en_ctrl_bits &= (pRegList->tsf[portId]._34_chan_pes_en.pes_en_ctrl_bits & ~(1<<chanId));
    
    //* 8. set tsf _30_chan_en to enable the channel.
#if !NEW_SWITCH_METHOD
    pRegList->tsf[portId]._30_chan_en.filter_en_ctrl_bits |= (1<<chanId);
#else
    if(pid == PID_ALL)
        SetValue(pRegList->tsf[portId]._4c_chan_pid, 0);    //* pass all pid
    else
        SetValue(pRegList->tsf[portId]._4c_chan_pid, (0x1fff<<16 | pid));
#endif
    
    return 0;
}

                  
void  tsfCloseChan(
            tsf_port_e      portId, 
            __u32           chanId
      )
{
    tsc_reg_list_t* pRegList;
    
    pRegList = gTscCtx.tscRegList;
    //* 1. set tsf _3c_chan_idx to choose a channel.
    pRegList->tsf[portId]._3c_chan_idx.chan_idx = chanId;
    
    //* set tsf _10_dma_intr_en to disable dma interrupt of the channel.
    pRegList->tsf[portId]._10_dma_intr_en.en_ctrl_bits &= ~(1<<chanId);
    
#if !NEW_SWITCH_METHOD
    //* set tsf _30_chan_en to disable the channel.
    pRegList->tsf[portId]._30_chan_en.filter_en_ctrl_bits &= ~(1<<chanId);
#else
    SetValue(pRegList->tsf[portId]._4c_chan_pid, (0<<16 | 0x1fff));
#endif
    
    gTscCtx.port[portId].chanOpenFlag        &= ~(1<<chanId);
    gTscCtx.port[portId].chanPesFlag         &= ~(1<<chanId);
    gTscCtx.port[portId].chanBufSize[chanId] = 0;
    gTscCtx.port[portId].chanBuf[chanId]     = 0;
    gTscCtx.port[portId].chanBufEnd[chanId]  = 0;
    
    return;
}

__s32 tsfFlushChan(
            tsf_port_e      portId, 
            __u32           chanId, 
            __u32           byteCount
      )
{
    __u32           rdPos;
    __u32           wtPos;
    __u32           validDataSize;
    tsc_reg_list_t* pRegList;
    
    pRegList = gTscCtx.tscRegList;
    
    //* selete a channel to operate.
    pRegList->tsf[portId]._3c_chan_idx.chan_idx = chanId;
    
    //* get readPos and writePos.
    rdPos = pRegList->tsf[portId]._5c_chan_rd_pos.rd_pos;
    wtPos = pRegList->tsf[portId]._58_chan_wt_pos.wt_pos;
    
    //* calculate total bytes
    if(wtPos >= rdPos)
        validDataSize = wtPos - rdPos;
    else
        validDataSize = wtPos + gTscCtx.port[portId].chanBufSize[chanId] - rdPos;
    
    if(byteCount > validDataSize)
        byteCount = validDataSize;
    
    //* readPos += byteCount, if readPos > bufferSize, readPos -= bufferSize
    rdPos += byteCount;
    if(rdPos > gTscCtx.port[portId].chanBufSize[chanId])
        rdPos -= gTscCtx.port[portId].chanBufSize[chanId];
    
    //* set readPos to register
    pRegList->tsf[portId]._5c_chan_rd_pos.rd_pos = rdPos;
    
    return 0;
}

__s32 tsfRequestData(
            tsf_port_e      portId, 
            __u32           chanId, 
            __u8**          ppData, 
            __u32*          pSize, 
            __u8**          ppRingData, 
            __u32*          pRingSize
      )
{
    __u32           rdPos;
    __u32           wtPos;
    __u32           validDataSize;
    __u32           chanBufSize;
    __u8*           chanBuf;
    tsc_reg_list_t* pRegList;
    
    chanBufSize = gTscCtx.port[portId].chanBufSize[chanId];
    chanBuf     = gTscCtx.port[portId].chanBuf[chanId];
    pRegList    = gTscCtx.tscRegList;
    
    //* selete a channel to operate.
    pRegList->tsf[portId]._3c_chan_idx.chan_idx = chanId;
    
    //* get readPos and writePos.
    rdPos = pRegList->tsf[portId]._5c_chan_rd_pos.rd_pos;
    wtPos = pRegList->tsf[portId]._58_chan_wt_pos.wt_pos;
    
    //* calculate total bytes
    if(wtPos >= rdPos)
        validDataSize = wtPos - rdPos;
    else
        validDataSize = wtPos + chanBufSize - rdPos;
        
    if(validDataSize > 0 && chanBuf != NULL)
    {
        if(validDataSize + rdPos > chanBufSize)
        {
            *ppData     = chanBuf + rdPos;
            *pSize      = chanBufSize - rdPos;
            *ppRingData = chanBuf;
            *pRingSize  = wtPos;
        }
        else
        {
            *ppData     = chanBuf + rdPos;
            *pSize      = validDataSize;
            *ppRingData = NULL;
            *pRingSize  = 0;
        }
    }
    else
    {
        *ppData     = NULL;
        *pSize      = 0;
        *ppRingData = NULL;
        *pRingSize  = 0;
    }
    
    return 0;
}
                     
__s32 tsfCheckDataSize(
            tsf_port_e      portId,
            __u32           chanId
      )
{
    __u32           rdPos;
    __u32           wtPos;
    __u32           validDataSize;
    __u32           chanBufSize;
    __u8*           chanBuf;
    tsc_reg_list_t* pRegList;
    
    chanBufSize = gTscCtx.port[portId].chanBufSize[chanId];
    chanBuf     = gTscCtx.port[portId].chanBuf[chanId];
    pRegList    = gTscCtx.tscRegList;
    
    if(chanBuf == NULL || chanBufSize == 0)
        return 0;
    
    //* selete a channel to operate.
    pRegList->tsf[portId]._3c_chan_idx.chan_idx = chanId;
    
    //* get readPos and writePos.
    rdPos = pRegList->tsf[portId]._5c_chan_rd_pos.rd_pos;
    wtPos = pRegList->tsf[portId]._58_chan_wt_pos.wt_pos;
    
    //* calculate total bytes
    if(wtPos >= rdPos)
        validDataSize = wtPos - rdPos;
    else
        validDataSize = wtPos + chanBufSize - rdPos;
    
    return validDataSize;
}

__u32 tsfGetPCR(tsf_port_e portId)
{
    __u32           prcHigh32Bits;
    tsc_reg_list_t* pRegList;
    
    pRegList      = gTscCtx.tscRegList;
    prcHigh32Bits = pRegList->tsf[portId]._24_pcr_data.pcr_value;
    
    return prcHigh32Bits;
}

void  tsfOpenPCRDetect(
            tsf_port_e      portId,
            __u32           chanId
      )
{
    tsf20_pcr_ctrl_reg_t    tmpPcrCtrlReg;
    tsc_reg_list_t*         pRegList;
    
    pRegList = gTscCtx.tscRegList;
    
    //* set pcr detect enable bit and the channel index to detect PCR.
    tmpPcrCtrlReg = pRegList->tsf[portId]._20_pcr_ctrl;
    tmpPcrCtrlReg.pcr_chan_idx  = chanId;
    tmpPcrCtrlReg.pcr_detect_en = 1;
    pRegList->tsf[portId]._20_pcr_ctrl = tmpPcrCtrlReg;
    
    //* enable pcr interrupt of this port.
    pRegList->tsf[portId]._08_status.pcr_intr_en = 1;
    
    return;
}
      
void  tsfClosePCRDetect(tsf_port_e portId)
{
    tsf20_pcr_ctrl_reg_t    tmpPcrCtrlReg;
    tsc_reg_list_t*         pRegList;
    
    pRegList = gTscCtx.tscRegList;
    
    //* set pcr detect enable bit to zero to disable pcr detect.
    tmpPcrCtrlReg = pRegList->tsf[portId]._20_pcr_ctrl;
    tmpPcrCtrlReg.pcr_detect_en = 0;
    pRegList->tsf[portId]._20_pcr_ctrl = tmpPcrCtrlReg;
    
    //* disable pcr interrupt of this port.
    pRegList->tsf[portId]._08_status.pcr_intr_en = 0;
    
    return;
}

void  tsfSetCaEnable(
            tsf_port_e      portId,
            __u32           chanId,
            __u32           en
      )
{
    tsc_reg_list_t*         pRegList;
    
    pRegList = gTscCtx.tscRegList;
    
    if(en)
        pRegList->tsf[portId]._38_chan_descramble_en.descramble_en_ctrl_bits |= 1<<chanId;
    else
        pRegList->tsf[portId]._38_chan_descramble_en.descramble_en_ctrl_bits &= ~(1<<chanId);
    
    return;
}
      
void  tsfSetCwIdx(
            tsf_port_e      portId,
            __u32           chanId,
            __u32           cwIdx
      )
{
    tsc_reg_list_t*         pRegList;
    
    pRegList = gTscCtx.tscRegList;
    
    pRegList->tsf[portId]._3c_chan_idx.chan_idx = chanId;
    pRegList->tsf[portId]._48_chan_cw_idx.related_ctrl_word_idx = cwIdx;
    
    return;
}

//************************* TSD functions **************************//
void  tsdSetCaType(ca_type_e caType)
{
    tsc_reg_list_t*         pRegList;
    
    pRegList = gTscCtx.tscRegList;
    
    pRegList->tsd._00_ctrl.descramble_method = caType;
    
    return;
}

void  tsdSetCW(
            __u32           cwIdx,
            cw_type_e       cwType,
            __u32           cwValue
      )
{
    tsd1c_ctrl_word_idx_reg_t tmpCWIdxReg;
    tsc_reg_list_t*           pRegList;
    
    pRegList = gTscCtx.tscRegList;
    
    //* set cw index register.
    tmpCWIdxReg                 = pRegList->tsd._1c_cw_idx;
    tmpCWIdxReg.cw_idx          = cwIdx;
    tmpCWIdxReg.cw_internal_idx = cwType;
    pRegList->tsd._1c_cw_idx    = tmpCWIdxReg;
    
    //* set cw value.
    pRegList->tsd._20_cw.cw_content = cwValue;
    
    return;
}
