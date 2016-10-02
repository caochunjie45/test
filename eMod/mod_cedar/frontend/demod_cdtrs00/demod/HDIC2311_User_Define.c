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
 以下函数是客户根据自己平台情况，需要改写的函数。
*****************************************************************************/
#define NUMBER_LOST_BEFORE_UNLOCK 6
#define MONITOR_DELAY_MS					500

static UINT8 HDIC_2311_LockConut = NUMBER_LOST_BEFORE_UNLOCK;

/**********************************************************
** 监控任务
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
			/*信号失锁，报告给应用程序？
			请根据平台实际设计实现。
			*/
		}
		HDIC2311_Wait(MONITOR_DELAY_MS);
	}
}
/********************************
 设置高频头
********************************/
BOOL HDIC2311_SetTuner(UINT32 Frequency)
{
	UINT8 Address = 0xC0;/*高频头的I2C设备地址，需要根据客户使用的高频头修改*/
	
	UINT8 TunerData[6],Temp;
	UINT16 tunerDivi;
	UINT8 ReadData = 0;

	/*设置高频头I2C转发，传入高频头的I2C设备地址*/
	/*是否需要设置I2C转发，需要根据客户硬件平台确认*/
	HDIC2311_OpenTunerI2C(Address);

	/****************************
	*
	* 此处应是tuner的锁频操作，具体
	* 需根据客户的实际情况。
	*
	****************************/

	HDIC2311_CloseTunerI2C();
	/*返回高频头锁定状态，锁住返回TRUE，无信号返回FALSE*/
	return TRUE;
}

/********************************
 I2C写寄存器
********************************/
UINT8 HDIC2311_WriteRegister(UINT16 Register, UINT8 Data)
{
	UINT8 err = HDIC_NO_ERROR;
	UINT8 RegisterData[3];
	UINT8 u8RetryTimes = 10, u8tmp = 0;

	RegisterData[0] = (UINT8)((Register&0xFF00)>>8);
	RegisterData[1] = (UINT8)(Register&0xFF);
	RegisterData[2] = Data;

	/* 需根据实际平台实现	*/
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
 I2C读寄存器
********************************/
UINT8 HDIC2311_ReadRegister(UINT16 Register, UINT8 *Data)
{
	UINT8 err = HDIC_NO_ERROR;
	UINT8 RegisterData[2];
	UINT8 u8RetryTimes = 10, u8tmp = 0;

	RegisterData[0] = (UINT8)((Register&0xFF00)>>8);
	RegisterData[1] = (UINT8)(Register&0xFF);
	
	/* 需根据实际平台实现	*/
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
延时函数
********************************/
void HDIC2311_Wait(UINT16 millisecond)
{
	/* 需根据实际平台实现	*/
	msleep(millisecond);
}

/********************************
芯片硬复位
********************************/
void HDIC2311_HWReset(void)
{
	/*芯片硬复位，可根据硬件情况实现，低电平复位，保持10MS*/
	/* 需根据实际平台实现	*/
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
对STB_N/PLL_RST管脚复位，可使用GPIO，在上电时对standby管脚复位
********************************/
UINT8 HDIC2311_PLLReset()
{
	UINT8 err = HDIC_NO_ERROR;

	HDICTBX_Print("STB_N/PLL_RST pin reset\n");

	return err;	
}

