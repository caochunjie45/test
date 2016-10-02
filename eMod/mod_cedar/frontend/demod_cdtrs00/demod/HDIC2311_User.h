/* ----------------------------------------------------------------------------
 File Name: Hdic2311_User_Define.h

 Description:

 Version 2.0 : Created   2013.03.05  Sharp Wang	 
				 2.2 : Modified  2013.05.20  Sharp Wang
 ---------------------------------------------------------------------------- */ 
#ifndef __HDIC2311_USER_H__
#define __HDIC2311_USER_H__
 
#include "HDICType.h"
#include "HDIC2311.h"
#include "mod_cedar_i.h"

/**********************************
根据平台情况，用户需要自己实现的函数
**********************************/
/*对STB_N/PLL_RST管脚复位，可使用GPIO，在上电时对standby管脚复位*/
UINT8 HDIC2311_PLLReset();

/*调试信息打印*/
#if 0
#define HDICTBX_Print	eLIBs_printf
#else
#define HDICTBX_Print(...)
#endif

void HDIC2311_MonitorTask(void);
UINT8 HDIC2311_WriteRegister(UINT16 Register,UINT8 Data);
UINT8 HDIC2311_ReadRegister(UINT16 Register,UINT8 *Data);
void HDIC2311_Wait(UINT16 millisecond);
/*芯片硬复位，请根据硬件设计实现*/
void HDIC2311_HWReset(void);

/*设置高频头函数，需根据不同高频头实现，高频头锁住信号返回TRUE*/
BOOL HDIC2311_SetTuner(UINT32 Frequency);


/***********************************
用户需要调用的函数
***********************************/
/*
HD2311获取信号质量和强度函数
*/
/*如果高频头驱动能提供信号强度的接口，请使用高频头驱动的接口*/
UINT8 HDIC2311_GetSignalStrength(UINT8 *SignalStrength);
UINT8 HDIC2311_GetSignalQuality(UINT8 *SignalQuality);

/*
系统初始化函数，需要根据客户平台情况做修改
*/
UINT8 HDIC2311_Init();

/*
HD2311锁频函数
*/
/*
	增加中间接口, 判定信号锁定. return 0 unlock, 1 Locked.
*/
char CDT_HDIC_CheckLockStatus();
/*自动模式地面信号锁频函数*/
BOOL HDIC2311_DTMBSetFrequency(UINT32 Frequency);
/*有线信号锁频函数，需要传入QAM模式和符号率,参数定义见HDICType.h*/
BOOL HDIC2311_CableSetFrequency(UINT32 Frequency,UINT8 CableMode,UINT16 CableRate);
/*手动模式地面信号锁频函数，需要传入信号模式等参数,参数定义见HDICType.h*/
BOOL HDIC2311_DTMBManualSetFrequency(UINT32 Frequency,UINT8 CarrierMode,UINT8 mode,UINT8 rate,UINT8 interleaverLength,UINT8 pnmode,UINT8 PnVariable);

/*
HD2311判断是否锁频成功函数
*/
BOOL HDIC2311_IsDemodLocked(BOOL PrintInfo,UINT8 *presult,UINT8 *locked, UINT8 *Status);
#endif

