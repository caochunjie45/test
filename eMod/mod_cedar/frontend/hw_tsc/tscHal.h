
#ifndef TSC_HAL_H
#define TSC_HAL_H

#include "tscConfig.h"

typedef enum TSF_CHAN_TYPE_E
{
    TSF_TP_CHAN             = 0,
    TSF_PES_CHAN            = 1
}tsf_chan_type_e;

typedef enum TSF_PORT_E
{
    TSF_PORT_0              = 0,
    TSF_PORT_1              = 1
}tsf_port_e;

typedef enum CA_TYPE_E
{
    CA_TYPE_DVBCSA_1_1      = 0,
    CA_TYPE_UNKNOW
}ca_type_e;

typedef enum CW_TYPE_E
{
    CW_ODD_LOW_32BITS       = 0,
    CW_ODD_HIGH_32BITS      = 1,
    CW_EVEN_LOW_32BITS      = 2,
    CW_EVEN_HIGH_32_BITS    = 3,
}cw_type_e;


//************************* TSC functions **************************//
typedef void (*TSC_ISR_CB)(
                    __u32   port0ChanIntrFlags, 
                    __u32   port0PcrIntrFlag, 
                    __u32   port1ChanIntrFlags,
                    __u32   potr1PcrIntrFlag
             );

void  tscInit(void);

void  tscExit(void);

void  tscSetIsrCbFunc(TSC_ISR_CB cb);

void  tscISR(void* param);


//************************* TSF functions **************************//
__s32 tsfOpenChan(
            __u32           pid, 
            __u8*           pBuf, 
            __u32           bufSize, 
            tsf_chan_type_e chanType, 
            tsf_port_e      portId, 
            __u32           chanId,
            __u32           intrEnable
      );
                  
void  tsfCloseChan(
            tsf_port_e      portId, 
            __u32           chanId
      );

__s32 tsfFlushChan(
            tsf_port_e      portId, 
            __u32           chanId, 
            __u32           byteCount
      );

__s32 tsfRequestData(
            tsf_port_e      portId, 
            __u32           chanId, 
            __u8**          ppData, 
            __u32*          pSize, 
            __u8**          ppRingData, 
            __u32*          pRingSize
      );
                     
__s32 tsfCheckDataSize(
            tsf_port_e      portId,
            __u32           chanId
      );

__u32 tsfGetPCR(tsf_port_e portId);

void  tsfOpenPCRDetect(
            tsf_port_e      portId,
            __u32           chanId
      );
      
void  tsfClosePCRDetect(tsf_port_e portId);

void  tsfSetCaEnable(
            tsf_port_e      portId,
            __u32           chanId,
            __u32           en
      );
      
void  tsfSetCwIdx(
            tsf_port_e      portId,
            __u32           chanId,
            __u32           cwIdx
      );

//************************* TSD functions **************************//
void  tsdSetCaType(ca_type_e caType);

void  tsdSetCW(
            __u32           cwIdx,
            cw_type_e       cwType,
            __u32           cwValue
      );            

#endif

