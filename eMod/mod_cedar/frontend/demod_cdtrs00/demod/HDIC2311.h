/* ----------------------------------------------------------------------------
 File Name: Hdic2311.h

 Description:

 Version 1.0 : Created   2012.03.21  Sharp Wang
  			 2.0 ��Modified  2013.03.05  Sharp Wang	 

 ---------------------------------------------------------------------------- */ 
#ifndef __HDIC2311_H__
#define __HDIC2311_H__

#include "HDICType.h"

/* ylzhu Add */
#include "..\DataType.h"

/*HD2311��I2C�豸��ַ*/
#define HDIC2311_I2C_ADDRESS				0x70
#define HDIC2311_I2C_RW_ADDRESS				0xE0

typedef struct HDIC2311_Init_Info_t
{
	UINT8 WorkMode;					/*����ģʽ�����������*/
	UINT8 TunerType;				/*��Ƶͷ���ͣ�����Ƶ������*/
	double TunerIF;					/*��Ƶͷ��Ƶ���Ƶ�ʣ���λMHZ*/
	UINT8 CrystalSelect;		/*����ѡ��*/	
	UINT8 TsType;						/*���л���*/
	UINT8 TsClockPolarity;	/*���ݲ�������ʱ�������ػ��½���*/
	UINT8 TsClockValid;			/*ʱ����Ч��ʽ��һֱ��Ч����valid�ź�*/
	UINT8 TsNullPacket;			/*�հ�ʹ�����*/
	UINT8 TsSerialOutPut;		/*����TSʱ�������TS_D7��TS_D0*/
	UINT8 SystemBandwidth;        /*ϵͳ����*/
}HDIC2311_Init_Info;

/*
HD2311 �ڲ�ʹ����غ���
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
