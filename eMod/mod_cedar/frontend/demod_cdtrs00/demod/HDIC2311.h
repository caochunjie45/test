/* ----------------------------------------------------------------------------
 File Name: Hdic2311.h

 Description:

 Version 1.0 : Created   2012.03.21  Sharp Wang
  			 2.0 ：Modified  2013.03.05  Sharp Wang	 

 ---------------------------------------------------------------------------- */ 
#ifndef __HDIC2311_H__
#define __HDIC2311_H__

#include "HDICType.h"

/* ylzhu Add */
#include "..\DataType.h"

/*HD2311的I2C设备地址*/
#define HDIC2311_I2C_ADDRESS				0x70
#define HDIC2311_I2C_RW_ADDRESS				0xE0

typedef struct HDIC2311_Init_Info_t
{
	UINT8 WorkMode;					/*工作模式，地面或有线*/
	UINT8 TunerType;				/*高频头类型，零中频或其他*/
	double TunerIF;					/*高频头中频输出频率，单位MHZ*/
	UINT8 CrystalSelect;		/*晶振选择*/	
	UINT8 TsType;						/*串行或并行*/
	UINT8 TsClockPolarity;	/*数据采样是在时钟上升沿或下降沿*/
	UINT8 TsClockValid;			/*时钟有效方式，一直有效或随valid信号*/
	UINT8 TsNullPacket;			/*空包使能与否*/
	UINT8 TsSerialOutPut;		/*串行TS时，输出在TS_D7或TS_D0*/
	UINT8 SystemBandwidth;        /*系统带宽*/
}HDIC2311_Init_Info;

/*
HD2311 内部使用相关函数
*/
UINT8 HDIC2311_SetWorkMode(UINT8 WorkMode);
UINT8 HDIC2311_SetAutoMode(void);
UINT8 HDIC2311_SetNCOValue(UINT32 NCOValue);
UINT8 HDIC2311_SetTunerType(UINT8 Type);
UINT8 HDIC2311_SetTsFormat(UINT8 Serial, UINT8 CLK_Polarity, UINT8 CLK_Valid, UINT8 Null_Packet, UINT8 Serial_Output);
UINT8 HDIC2311_SetManualMode(UINT8 CarrierMode, UINT8 mode,UINT8 rate, UINT8 interleaverLength,UINT8 pnmode,UINT8 PnVariable);
UINT8 HDIC2311_GetNCOValue(UINT32 *NCOValue);
UINT8 HDIC2311_GetParameters(UINT8 *CarrierMode, UINT8 *pmode,UINT8 *prate, UINT8 *pinterleaverLength,UINT8 *Pnmode,UINT8 *PnVariable);
UINT8 HDIC2311_GetSignalSNRSC(double *SignalSNR);
UINT8 HDIC2311_GetSignalSNRMC(double *SignalSNR);
UINT8 HDIC2311_GetLdpcBER(double *pLdpcBER);
UINT8 HDIC2311_GetSignalBER(double *pSignalBER);
#ifdef OPEN_DVB_C_MODE
UINT8 HDIC2311_GetCableSignalSNR(double *SignalSNR);
UINT8 HDIC2311_GetCableLdpcBER(double *pLdpcBER);
UINT8 HDIC2311_GetCableSignalBER (double *pSignalBER);
#endif
UINT8 HDIC2311_GetFieldStrength(UINT32 *FieldStrength);
UINT8 HDIC2311_GetSignalSNR(double *SignalSNR);
UINT8 HDIC2311_OpenTunerI2C(UINT8 Address);
UINT8 HDIC2311_CloseTunerI2C(void);
UINT8 HDIC2311_CalculateNCOValue(double TunerIF,UINT8 CrystalSelect,UINT32 *NCOValue);
UINT8 HDIC2311_SoftReset(void);
#ifdef OPEN_DVB_C_MODE
UINT8 HDIC2311_SetCableMode(UINT8 Mode);
UINT8 HDIC2311_SetCableRate(UINT16 Rate);
#endif
UINT8 HDIC2311_SetSytemBandwidth(UINT8 SystemBandwidth,UINT8 CrystalSelect);
#endif
