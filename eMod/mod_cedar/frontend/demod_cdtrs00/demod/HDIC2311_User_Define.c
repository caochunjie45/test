/* ----------------------------------------------------------------------------
 File Name: Hdic2311_User_Define.c

 Description:

 Version 2.0 : Created   2013.03.05  Sharp Wang	
				 2.2 : Modified  2013.05.20  Sharp Wang
 ---------------------------------------------------------------------------- */ 
#include "HDICType.h"
#include "HDIC2311.h"
#include "HDIC2311_User.h"
#include "../Os_adapter.h"

/* ylzhu add */
//#include "..\Pfdriver.h"
//#include <Windows.h>

/*****************************************************************************
 ���º����ǿͻ������Լ�ƽ̨�������Ҫ��д�ĺ�����
*****************************************************************************/
#define NUMBER_LOST_BEFORE_UNLOCK 6
#define MONITOR_DELAY_MS					500

static UINT8 HDIC_2311_LockConut = NUMBER_LOST_BEFORE_UNLOCK;

/**********************************************************
** �������
**
**********************************************************/
void HDIC2311_MonitorTask()
{
	UINT8 LockResult,locked,status;
	while(1)
	{
		HDIC2311_IsDemodLocked(TRUE,&LockResult,&locked,&status);
		if(LockResult==1)
		{
			HDIC_2311_LockConut = NUMBER_LOST_BEFORE_UNLOCK;
			/*Locked*/
		}
		else
		{
			HDIC_2311_LockConut--;
		}
		if(HDIC_2311_LockConut==0)
		{
			/*�ź�ʧ���������Ӧ�ó���
			�����ƽ̨ʵ�����ʵ�֡�
			*/
		}
		HDIC2311_Wait(MONITOR_DELAY_MS);
	}
}
/********************************
 ���ø�Ƶͷ
********************************/
BOOL HDIC2311_SetTuner(UINT32 Frequency)
{
	UINT8 Address = 0xC0;/*��Ƶͷ��I2C�豸��ַ����Ҫ���ݿͻ�ʹ�õĸ�Ƶͷ�޸�*/
	
	UINT8 TunerData[6],Temp;
	UINT16 tunerDivi;
	UINT8 ReadData = 0;

	/*���ø�ƵͷI2Cת���������Ƶͷ��I2C�豸��ַ*/
	/*�Ƿ���Ҫ����I2Cת������Ҫ���ݿͻ�Ӳ��ƽ̨ȷ��*/
	HDIC2311_OpenTunerI2C(Address);

	/****************************
	*
	* �˴�Ӧ��tuner����Ƶ����������
	* ����ݿͻ���ʵ�������
	*
	****************************/

	HDIC2311_CloseTunerI2C();
	/*���ظ�Ƶͷ����״̬����ס����TRUE�����źŷ���FALSE*/
	return TRUE;
}

/********************************
 I2Cд�Ĵ���
********************************/
UINT8 HDIC2311_WriteRegister(UINT16 Register, UINT8 Data)
{
	UINT8 err = HDIC_NO_ERROR;
	UINT8 RegisterData[3];
	UINT8 u8RetryTimes = 10, u8tmp = 0;

	RegisterData[0] = (UINT8)((Register&0xFF00)>>8);
	RegisterData[1] = (UINT8)(Register&0xFF);
	RegisterData[2] = Data;

	/* �����ʵ��ƽ̨ʵ��	*/
	for(u8tmp = 0; u8tmp < u8RetryTimes; u8tmp++)
	{
		err = iic_write(HDIC2311_I2C_ADDRESS,RegisterData, 2 ,&Data, 1);
		if(err == 0)
		{
			break;
		}
		//HDIC2311_Wait(1);
	}
	return (err);
}

/********************************
 I2C���Ĵ���
********************************/
UINT8 HDIC2311_ReadRegister(UINT16 Register, UINT8 *Data)
{
	UINT8 err = HDIC_NO_ERROR;
	UINT8 RegisterData[2];
	UINT8 u8RetryTimes = 10, u8tmp = 0;

	RegisterData[0] = (UINT8)((Register&0xFF00)>>8);
	RegisterData[1] = (UINT8)(Register&0xFF);
	
	/* �����ʵ��ƽ̨ʵ��	*/
	for(u8tmp = 0; u8tmp < u8RetryTimes; u8tmp++)
	{
		//err = iic_write(HDIC2311_I2C_ADDRESS,RegisterData, 1, &(RegisterData[1]), 1);//Or you should use Write_I2C_WithoutStop()
		err |= iic_read(HDIC2311_I2C_ADDRESS,RegisterData, 2,Data, 1);
		if(err == 0)
		{
			break;
		}
		//HDIC2311_Wait(1);
	}
	return (err);
}

/********************************
��ʱ����
********************************/
void HDIC2311_Wait(UINT16 millisecond)
{
	/* �����ʵ��ƽ̨ʵ��	*/
	msleep(millisecond);
}

/********************************
оƬӲ��λ
********************************/
void HDIC2311_HWReset(void)
{
	/*оƬӲ��λ���ɸ���Ӳ�����ʵ�֣��͵�ƽ��λ������10MS*/
	/* �����ʵ��ƽ̨ʵ��	*/
	#if 0
    SetPinAH();
    Sleep(30);
    SetPinAL();
    Sleep(40);
	#endif
	
	HDICTBX_Print("2311 Demod H/W Reset !!!\n");
	return;
}

/********************************
��STB_N/PLL_RST�ܽŸ�λ����ʹ��GPIO�����ϵ�ʱ��standby�ܽŸ�λ
********************************/
UINT8 HDIC2311_PLLReset()
{
	UINT8 err = HDIC_NO_ERROR;

	HDICTBX_Print("STB_N/PLL_RST pin reset\n");

	return err;	
}

