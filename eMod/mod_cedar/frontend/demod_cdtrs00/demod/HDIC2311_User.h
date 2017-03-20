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
����ƽ̨������û���Ҫ�Լ�ʵ�ֵĺ���
**********************************/
/*��STB_N/PLL_RST�ܽŸ�λ����ʹ��GPIO�����ϵ�ʱ��standby�ܽŸ�λ*/
UINT8 HDIC2311_PLLReset();

/*������Ϣ��ӡ*/
#if 0
#define HDICTBX_Print	eLIBs_printf
#else
#define HDICTBX_Print(...)
#endif

void HDIC2311_MonitorTask(void);
UINT8 HDIC2311_WriteRegister(UINT16 Register,UINT8 Data);
UINT8 HDIC2311_ReadRegister(UINT16 Register,UINT8 *Data);
void HDIC2311_Wait(UINT16 millisecond);
/*оƬӲ��λ�������Ӳ�����ʵ��*/
void HDIC2311_HWReset(void);

/*���ø�Ƶͷ����������ݲ�ͬ��Ƶͷʵ�֣���Ƶͷ��ס�źŷ���TRUE*/
BOOL HDIC2311_SetTuner(UINT32 Frequency);


/***********************************
�û���Ҫ���õĺ���
***********************************/
/*
HD2311��ȡ�ź�������ǿ�Ⱥ���
*/
/*�����Ƶͷ�������ṩ�ź�ǿ�ȵĽӿڣ���ʹ�ø�Ƶͷ�����Ľӿ�*/
UINT8 HDIC2311_GetSignalStrength(UINT8 *SignalStrength);
UINT8 HDIC2311_GetSignalQuality(UINT8 *SignalQuality);

/*
ϵͳ��ʼ����������Ҫ���ݿͻ�ƽ̨������޸�
*/
UINT8 HDIC2311_Init();

/*
HD2311��Ƶ����
*/
/*
	�����м�ӿ�, �ж��ź�����. return 0 unlock, 1 Locked.
*/
char CDT_HDIC_CheckLockStatus();
/*�Զ�ģʽ�����ź���Ƶ����*/
BOOL HDIC2311_DTMBSetFrequency(UINT32 Frequency);
/*�����ź���Ƶ��������Ҫ����QAMģʽ�ͷ�����,���������HDICType.h*/
BOOL HDIC2311_CableSetFrequency(UINT32 Frequency,UINT8 CableMode,UINT16 CableRate);
/*�ֶ�ģʽ�����ź���Ƶ��������Ҫ�����ź�ģʽ�Ȳ���,���������HDICType.h*/
BOOL HDIC2311_DTMBManualSetFrequency(UINT32 Frequency,UINT8 CarrierMode,UINT8 mode,UINT8 rate,UINT8 interleaverLength,UINT8 pnmode,UINT8 PnVariable);

/*
HD2311�ж��Ƿ���Ƶ�ɹ�����
*/
BOOL HDIC2311_IsDemodLocked(BOOL PrintInfo,UINT8 *presult,UINT8 *locked, UINT8 *Status);
#endif

