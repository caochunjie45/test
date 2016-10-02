/*
 * CopyRight: all rights reserved by cdtect shenzhen inc. 
 * 02.11.2014
*/
#include "demod\HDIC2311_User.h"
#include "tuner\rdatv_bb.h"

HDIC2311_Init_Info InitInfo;
UINT8 RS00_count=0;
char RS00_init(void)
{
    char err = HDIC_NO_ERROR;
	UINT8 reg_data = 0;
	UINT16 reg_addr = 0x00;
	UINT16 chipid;
	
	#if 1
	HDIC2311_OpenTunerI2C(RDA5880_I2C_addr_WR_BIT);
	chipid = RDATVGetChipID();
	HDIC2311_CloseTunerI2C();
	HDICTBX_Print("\n\nChipId = 0x%04X\n\n", chipid);	// 0x003A,0x0018
	#endif

	if(RS00_count == 0)
	{
		err = HDIC2311_Init();
		#if 0
		HDIC2311_OpenTunerI2C(RDA5880_I2C_addr_WR_BIT);
		RDATVM_WRITE(0xff,0);
		RDATVM_READ(0xe2,&chipid);
		HDIC2311_CloseTunerI2C();
		printf("ChipId = 0x%x\n", reg_data);
		#endif
		HDIC2311_OpenTunerI2C(RDA5880_I2C_addr_WR_BIT);
		devTunerInit();
		HDIC2311_CloseTunerI2C();
		RS00_count = 1;
		
		#if 1
		HDIC2311_OpenTunerI2C(RDA5880_I2C_addr_WR_BIT);
		chipid = RDATVGetChipID();
		HDIC2311_CloseTunerI2C();
		HDICTBX_Print("\n\nChipId = 0x%04X\n\n", chipid);	// 0x003A,0x0018
		#endif
	}
	return err;
}

void RS00_deinit(void)
{
	 /* deinit */
	RS00_count=0;
    return;
}
unsigned char RS00_tune(unsigned int frequency_KHz, unsigned int bandwidth_KHz)
{
	UINT8 err = HDIC_NO_ERROR;
	
	HDICTBX_Print("@@@@RS00_tune(%d, %d):begin\n", frequency_KHz, bandwidth_KHz);
	if(InitInfo.WorkMode == HDIC2311_DTMB_SELECT)
	{
		/**********************************************
			DTMB mode use different API to set freq
		**********************************************/
		HDIC2311_OpenTunerI2C(RDA5880_I2C_addr_WR_BIT);
		devDigitalTuner_SetFreq(frequency_KHz, bandwidth_KHz);
		HDIC2311_CloseTunerI2C();
		switch(bandwidth_KHz/1000)
		{
			case 6:
			{
				InitInfo.SystemBandwidth = HDIC2311_BANDWIDTH_6M; /*设置系统带宽*/
				break;
			}
			case 7:
			{
				InitInfo.SystemBandwidth = HDIC2311_BANDWIDTH_7M; /*设置系统带宽*/
				break;
			}
			case 8:
			{
				InitInfo.SystemBandwidth = HDIC2311_BANDWIDTH_8M; /*设置系统带宽*/
				break;
			}
			default:
				InitInfo.SystemBandwidth = HDIC2311_BANDWIDTH_8M; /*设置系统带宽*/
				break;
			
		}
		if(InitInfo.WorkMode == HDIC2311_DTMB_SELECT)
		{
			// HDIC2311_SetSytemBandwidth(InitInfo.SystemBandwidth,InitInfo.CrystalSelect);
			err = HDIC2311_DTMBSetFrequency(frequency_KHz);
		}
	}

	HDICTBX_Print("@@@@RS00_tune():over, err=%d\n", err);
	return err;
}

char RS00_lockstatus(void)
{
	UINT8 u8LockStatus = 0;  // 0 unlock  , 1 locked
	u8LockStatus = CDT_HDIC_CheckLockStatus();
	return u8LockStatus;
}

void RS00_signal_strength_quality(UINT8* signal_quality, UINT8* signal_strength)
{
	UINT8 err = HDIC_NO_ERROR;

	*signal_quality = 0;
	*signal_strength = 0;
	//err = HDIC2311_GetSignalStrength(signal_strength);
	HDIC2311_OpenTunerI2C(RDA5880_I2C_addr_WR_BIT);
	RDATVSingalStrength(signal_strength);
	HDIC2311_CloseTunerI2C();
	err |= HDIC2311_GetSignalQuality(signal_quality);
	if(err != HDIC_NO_ERROR)
	{
		*signal_quality = 0;
		*signal_strength = 0;
	}
}

