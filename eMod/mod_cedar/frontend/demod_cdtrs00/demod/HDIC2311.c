/* ----------------------------------------------------------------------------
 File Name: Hdic2311.c

 Description:

 Version 1.0 : Created   2012.03.21  Sharp Wang
         1.1 ：Modified HDIC2311_SetTsFormat and HDIC2311_SetTunerType.
         2.0 ：Modified  2013.03.05  Sharp Wang	 
         2.1 ：Modified  2013.04.11  Sharp Wang, Modify WorkMode Define.
         2.2 ：Modified  2013.05.20  Sharp Wang, Add some registers init.
         2.3 ：Modified  HDIC2311_SetTsFormat 2013.05.30
         2.4 : Modified  2013.06.17  cc.xue
         2.5 : Modified  2013.06.19  cc.xue
         2.6 : Modified  2013.08.19  Sharp Wang,Updated init registers,add log10.
         2.7 : Modified  2013.10.31  cc.xue,Updated HDIC2311_SetWorkMode.
         2.8 : Modified  2013.12.02  cc.xue,Add System bandwidth function
 ---------------------------------------------------------------------------- */ 
#include "HDICType.h"
#include "HDIC2311.h"
#include "HDIC2311_User.h"

/**********************************************
如果系统不支持log10运算，可以打开这个宏定义，
通过查表获得10*log10的运算结果
**********************************************/
#define SYSTEM_NOT_SUPPORT_LOG10	0

/**********************************************
调试信息开关
**********************************************/
#define HDIC2311_DEBUG_INFO_OPEN	0

/**********************************************
** 配置2311的工作模式，
** 地面：HDIC2311_DTMB_SELECT 
** 或 有线：HDIC2311_CABLE_SELECT
**********************************************/
static UINT8 HD2311_WorkMode = HDIC2311_DTMB_SELECT;

#if SYSTEM_NOT_SUPPORT_LOG10 
static UINT16 SNR_log_table_int[]=
{   
    0, 100,
    2, 104,
    4, 109,
    6, 114,
    8, 120, 
    10, 125, 
    12, 131, 
    14, 138,  
    16, 144,   
    18, 151,    
    20, 158,  
    22, 166,   
    24, 173,   
    26, 182,  
    28, 190,  
    30, 199,   
    32, 208 
};

static UINT16 Left_Table(UINT16 InValue)
{   
    UINT16 OutValue = 0;
    UINT16 TableSize = sizeof(SNR_log_table_int)/sizeof(UINT16);
    UINT16 i;       
    for(i=0; i<TableSize; i+=2) 
    {           
        if(InValue<=SNR_log_table_int[i+1])     
        {                   
            OutValue = SNR_log_table_int[i];      
            return OutValue;        
        }       
    } 

    OutValue = 32;  
    return OutValue;
}

static UINT32 Log10_Convert(UINT32 InValue)
{   
    UINT32 u32_temp = 1;
    UINT8  indx = 0;
    UINT32 InValue_left;
    UINT32 log_ret;     
    
    do
    {
        u32_temp = u32_temp << 1;
        if (InValue < u32_temp)
            break;          
    }while(++indx < 32);
    
    log_ret = (301*indx+50)/100;
    InValue_left = InValue*100/(1<<indx);
    log_ret = log_ret + Left_Table(InValue_left)/10;
    return log_ret; 
}
#endif

UINT8 HDIC2311_SetWorkMode(UINT8 mode)
{
	UINT8 err = HDIC_NO_ERROR;
	UINT8 ReadData = 0,WriteData = 0;

	HDIC2311_ReadRegister(0x0001,&ReadData);
	WriteData = ReadData&0xFE;
	WriteData |= mode;
	HDIC2311_WriteRegister(0x0001,WriteData);
	
	if(mode == HDIC2311_DTMB_SELECT)
	{
		err = HDIC2311_WriteRegister(0x0018,0x55);
		err |= HDIC2311_WriteRegister(0x004B,0x0A);
		err |= HDIC2311_WriteRegister(0x004C,0x48);
		err |= HDIC2311_WriteRegister(0x004D,0x84);
		err |= HDIC2311_WriteRegister(0x0086,0x08);
		err |= HDIC2311_WriteRegister(0x0087,0x03);
		err |= HDIC2311_WriteRegister(0x0088,0x0A);
		err |= HDIC2311_WriteRegister(0x0089,0x03);
		err |= HDIC2311_WriteRegister(0x024A,0x28);
		err |= HDIC2311_WriteRegister(0x0294,0x04);
		err |= HDIC2311_WriteRegister(0x0296,0x04);
		err |= HDIC2311_WriteRegister(0x00BA,0x94);
		err |= HDIC2311_WriteRegister(0x008A,0x2B);
		err |= HDIC2311_WriteRegister(0x02DC,0x40);
		err |= HDIC2311_WriteRegister(0x02D1,0x0F);
		err |= HDIC2311_WriteRegister(0x0063,0x60);
		err |= HDIC2311_WriteRegister(0x00EE,0x1F);
		err |= HDIC2311_WriteRegister(0x00EF,0xFF);
		err |= HDIC2311_WriteRegister(0x0018,0x55); 
		err |= HDIC2311_WriteRegister(0x0021,0x40);//作用是更改AGC的检测功率方式，原先是在8MHz带宽滤波后检测中频功率；更改后是在8MHz带宽滤波前检测中频功率.
		HDICTBX_Print("2311 DTMB Selected...\n");
	}
	else if(mode == HDIC2311_CABLE_SELECT)
	{
		HDICTBX_Print("2311 Cable Selected...\n");
	}
	
	HD2311_WorkMode = mode;

	return (err);
}

/************************************
** 自动设置地面情况下信号格式
*************************************/
UINT8 HDIC2311_SetAutoMode(void)
{
	UINT8 err = HDIC_NO_ERROR;

	HDICTBX_Print("Set AutoMode \n");

	/*配置为自动模式*/
	err = HDIC2311_WriteRegister(0x0002,0x00);
	return err;	
}

/*
*********************************************
判断信号是否锁住
return 1: locked  0: no clocked
*********************************************
*/
BOOL HDIC2311_IsDemodLocked(BOOL PrintInfo,UINT8 *presult,UINT8 *locked, UINT8 *Status)
{
	UINT8 ReadData = 0, CarrierMode = 0, WriteData = 0;
#if HDIC2311_DEBUG_INFO_OPEN
	double SNR,BER,LBER;
	UINT8 Carrier,Mode,Rate,InterleaverLength,PnMode,PnVariable;
	UINT32 FieldStrength,NCOValue;
#endif	
	/*获取工作模式,DTMB或Cable*/
	if(HD2311_WorkMode == HDIC2311_DTMB_SELECT)
	{
		HDIC2311_ReadRegister(0x0011,locked);
		HDIC2311_ReadRegister(0x0010,Status);
		
  	  	if(((*Status)&0xf0) == 0x80)
	  	{
			HDIC2311_ReadRegister(0x0005,&ReadData);
			CarrierMode = ReadData&0x10;
			CarrierMode >>= 4;
			
			HDIC2311_ReadRegister(0x0022,&ReadData);
			WriteData = ReadData&0x0f;
			WriteData |= 0x50;
			if(CarrierMode == HDIC_CARRIER_SINGLE)
			{
				WriteData |= 0x70;
			}
			
			if((ReadData!=WriteData)&&(ReadData!=0))
			{
				HDICTBX_Print("==>Write 0x0022: %02x\n",WriteData);
				HDIC2311_WriteRegister(0x0022,WriteData);
			}
		}
#if HDIC2311_DEBUG_INFO_OPEN
		HDIC2311_GetSignalSNR(&SNR);
		HDIC2311_GetSignalBER(&BER);
		HDIC2311_GetParameters(&Carrier,&Mode, &Rate, &InterleaverLength,&PnMode,&PnVariable);
		HDIC2311_GetFieldStrength(&FieldStrength);
		HDIC2311_GetLdpcBER(&LBER);
		HDIC2311_GetNCOValue(&NCOValue);
		HDICTBX_Print("SNR: %f,BER: %f,LBER: %f,FS: %d,NCO: 0x%06x \n",SNR,BER,LBER,FieldStrength,NCOValue);

		HDICTBX_Print("Mode:");
		switch(Mode)
		{
		case HDIC_DTMB_4QAM_NR:
			HDICTBX_Print("4QAM_NR, ");
			break;
		case HDIC_DTMB_4QAM:
			HDICTBX_Print("4QAM, ");
			break;
		case HDIC_DTMB_16QAM:
			HDICTBX_Print("16QAM, ");
			break;
		case HDIC_DTMB_32QAM:
			HDICTBX_Print("32QAM, ");
			break;
		case HDIC_DTMB_64QAM:
			HDICTBX_Print("64QAM, ");
			break;
		default:
			break;
		}

		HDICTBX_Print("PNMode:");
		switch(PnMode)
		{
		case HDIC_PN_945:
			HDICTBX_Print("945, ");
			break;
		case HDIC_PN_595:
			HDICTBX_Print("595, ");
			break;
		case HDIC_PN_420:
			HDICTBX_Print("420, ");
			break;
		default:
			break;
		}
		
		HDICTBX_Print("Rate:");
		switch(Rate)
		{
		case HDIC_RATE_04:
			HDICTBX_Print("0.4, ");
			break;
		case HDIC_RATE_06:
			HDICTBX_Print("0.6, ");
			break;
		case HDIC_RATE_08:
			HDICTBX_Print("0.8, ");
			break;
		default:
			break;
		}

		HDICTBX_Print("Carrier:");	
		switch(Carrier)
		{
		case HDIC_CARRIER_SINGLE:
			HDICTBX_Print("Single, ");
			break;
		case HDIC_CARRIER_MULTI:
			HDICTBX_Print("Multi, ");
			break;
		default:
			break;
		}
		
		HDICTBX_Print("Interleave:");		
		switch(InterleaverLength)
		{
		case HDIC_INTERLEAVER_720:
			HDICTBX_Print("720, ");
			break;
		case HDIC_INTERLEAVER_240:
			HDICTBX_Print("240, ");
			break;
		default:
			break;
		}
		
		HDICTBX_Print("PNVariable:");
		switch(PnVariable)
		{
		case HDIC_PN_VARIABLE:
			HDICTBX_Print("Variable ");
			break;
		case HDIC_PN_FIXED:
			HDICTBX_Print("Fixed ");
			break;
		default:
			break;
		}
				
		HDICTBX_Print("\n");
#endif		
	}
	#ifdef OPEN_DVB_C_MODE
	else if(HD2311_WorkMode == HDIC2311_CABLE_SELECT)
	{
		HDIC2311_ReadRegister(0x0110,Status);
		*locked = *Status;
	}
	#endif
	if(PrintInfo)
		HDICTBX_Print("0x%02x 0x%02x!\n",*Status,*locked);
		
	if ((*locked)&0x01)
	{
		*presult = 1;
		return TRUE;
	}
	else
	{
		*presult = 0;
		return FALSE;
	}
}

/************************************
** 设置系统带宽： 
**HDIC2311_BANDWIDTH_8M,
**HDIC2311_BANDWIDTH_7M,
**HDIC2311_BANDWIDTH_6M		
*************************************/
UINT8 HDIC2311_SetSytemBandwidth(UINT8 SystemMode,UINT8 CrystalSelect)
{
	UINT8 err = HDIC_NO_ERROR;
	UINT8 ReadData = 0,WriteData = 0;

	if(SystemMode == HDIC2311_BANDWIDTH_8M)
	{
		return err; /* do nothing, default */
	}
	else
	{
		HDIC2311_ReadRegister(0x0022,&ReadData);
		ReadData |= 0x08;
		HDIC2311_WriteRegister(0x0022,ReadData);
	}
	
	if((SystemMode == HDIC2311_BANDWIDTH_7M))
	{
		if(CrystalSelect == HDIC2311_CRYSTAL_27M)
		{
			err = HDIC2311_WriteRegister(0x0038,0x09);
			err = HDIC2311_WriteRegister(0x0039,0x78);
			err = HDIC2311_WriteRegister(0x003A,0x29);
			err = HDIC2311_WriteRegister(0x003B,0xCB);
		}
		else if(CrystalSelect == HDIC2311_CRYSTAL_304M)
		{
			err = HDIC2311_WriteRegister(0x0038,0x09);
			err = HDIC2311_WriteRegister(0x0039,0x87);
			err = HDIC2311_WriteRegister(0x003A,0xA4);
			err = HDIC2311_WriteRegister(0x003B,0xAB);
		}
		else if(CrystalSelect == HDIC2311_CRYSTAL_24M)
		{
			err = HDIC2311_WriteRegister(0x0038,0x0A);
			err = HDIC2311_WriteRegister(0x0039,0x60);
			err = HDIC2311_WriteRegister(0x003A,0x5C);
			err = HDIC2311_WriteRegister(0x003B,0xE1);
		}
		else if(CrystalSelect == HDIC2311_CRYSTAL_16M)
		{
			err = HDIC2311_WriteRegister(0x0038,0x09);
			err = HDIC2311_WriteRegister(0x0039,0xC5);
			err = HDIC2311_WriteRegister(0x003A,0x90);
			err = HDIC2311_WriteRegister(0x003B,0x28);
		}
	}
	else if((SystemMode == HDIC2311_BANDWIDTH_6M))
	{
		if(CrystalSelect == HDIC2311_CRYSTAL_27M)
		{
			err = HDIC2311_WriteRegister(0x0038,0x15);
			err = HDIC2311_WriteRegister(0x0039,0xB6);
			err = HDIC2311_WriteRegister(0x003A,0xDB);
			err = HDIC2311_WriteRegister(0x003B,0x6D);
		}
		else if(CrystalSelect == HDIC2311_CRYSTAL_304M)
		{
			err = HDIC2311_WriteRegister(0x0038,0x15);
			err = HDIC2311_WriteRegister(0x0039,0xC8);
			err = HDIC2311_WriteRegister(0x003A,0xEA);
			err = HDIC2311_WriteRegister(0x003B,0xC7);
		}
		else if(CrystalSelect == HDIC2311_CRYSTAL_24M)
		{
			err = HDIC2311_WriteRegister(0x0038,0x16);
			err = HDIC2311_WriteRegister(0x0039,0xC5);
			err = HDIC2311_WriteRegister(0x003A,0xC1);
			err = HDIC2311_WriteRegister(0x003B,0xB1);
		}
		else if(CrystalSelect == HDIC2311_CRYSTAL_16M)
		{
			err = HDIC2311_WriteRegister(0x0038,0x16);
			err = HDIC2311_WriteRegister(0x0039,0x11);
			err = HDIC2311_WriteRegister(0x003A,0x28);
			err = HDIC2311_WriteRegister(0x003B,0x2E);
		}
	}

	return err;
}

/************************************
** 设置有线情况下的信号模式： 
**	HDIC2311_CABLE_16QAM,			
**	HDIC2311_CABLE_32QAM,			
**	HDIC2311_CABLE_64QAM,			
**	HDIC2311_CABLE_128QAM,			
**	HDIC2311_CABLE_256QAM			
*************************************/
#ifdef OPEN_DVB_C_MODE
UINT8 HDIC2311_SetCableMode(UINT8 Mode)
{
	UINT8 err = HDIC_NO_ERROR;
	
	err = HDIC2311_WriteRegister(0x0170,Mode);

	return err;
}

/************************************
** 设置有线情况下的符号率： 
** Rate:单位K, 如6975或6900
*************************************/
UINT8 HDIC2311_SetCableRate(UINT16 Rate)
{
	UINT8 err = HDIC_NO_ERROR;
	UINT32 Value=0;

	Value = Rate;
	Value *= 4000;

	HDIC2311_WriteRegister(0x0130,(UINT8)((Value&0xFF000000)>>24));
	HDIC2311_WriteRegister(0x0131,(UINT8)((Value&0xFF0000)>>16));
	HDIC2311_WriteRegister(0x0132,(UINT8)((Value&0xFF00)>>8));
	HDIC2311_WriteRegister(0x0133,(UINT8)(Value&0xFF));
	return err;
}
#endif

/************************************
** 手动设置地面情况下信号格式
*************************************/
UINT8 HDIC2311_SetManualMode(UINT8 CarrierMode, UINT8 mode,UINT8 rate, UINT8 interleaverLength,UINT8 pnmode,UINT8 PnVariable)
{
	UINT8 err = HDIC_NO_ERROR;
	UINT8 ReadData = 0,WriteData = 0;

	HDICTBX_Print("Set Manual Mode !\n");

	/*配置为手动模式*/
	err = HDIC2311_WriteRegister(0x0002,0xFF);

	err |=HDIC2311_ReadRegister(0x0003,&ReadData);
	WriteData = ReadData&0xE8;
	WriteData |= pnmode;
	WriteData |= (PnVariable<<2);
	WriteData |= (CarrierMode<<4);
	err |= HDIC2311_WriteRegister(0x0003,WriteData);
	HDICTBX_Print("0x0003  0x%02x !\n",WriteData);

	err |= HDIC2311_ReadRegister(0x0004,&ReadData);
	WriteData = ReadData&0x88;
	WriteData |= mode;
	WriteData |= (rate<<4);
	WriteData |= (interleaverLength<<6);
	err |= HDIC2311_WriteRegister(0x0004,WriteData);
	HDICTBX_Print("0x0004  0x%02x !\n",WriteData);

	return err;	
}

/************************************
** 获取正常工作时的NCO
*************************************/
UINT8 HDIC2311_GetNCOValue(UINT32 *NCOValue)
{
	UINT8 err = HDIC_NO_ERROR;
	UINT8 ReadData = 0;

	/*获取工作模式,DTMB或Cable*/
	if(HD2311_WorkMode == HDIC2311_DTMB_SELECT)
	{	
		err = HDIC2311_ReadRegister(0x0059,&ReadData);
		*NCOValue = ReadData;

		err |= HDIC2311_ReadRegister(0x005A,&ReadData);
		*NCOValue <<= 8;
		*NCOValue |= ReadData;

		err |= HDIC2311_ReadRegister(0x005B,&ReadData);
		*NCOValue <<= 8;
		*NCOValue |= ReadData;
	}
	else if(HD2311_WorkMode == HDIC2311_CABLE_SELECT)
	{
		err =  HDIC2311_ReadRegister(0x0197,&ReadData);
		*NCOValue = ReadData;

		err |= HDIC2311_ReadRegister(0x0198,&ReadData);
		*NCOValue <<= 8;
		*NCOValue |= ReadData;

		err |= HDIC2311_ReadRegister(0x0199,&ReadData);
		*NCOValue <<= 8;
		*NCOValue |= ReadData;
	}
	*NCOValue = (UINT32)(0x800000-*NCOValue);
	//HDICTBX_Print("NCO %x\n",*NCOValue);
	return err;
}

/************************************
** 设置初始工作时的NCO
*************************************/
UINT8 HDIC2311_SetNCOValue(UINT32 NCOValue)
{
	UINT8 err = HDIC_NO_ERROR;
	UINT8 temp1=0,temp2=0,temp3=0;

	temp1 = (UINT8)((NCOValue&0x00ff0000)>>16);
	temp2 = (UINT8)((NCOValue&0x0000ff00)>>8);
	temp3 = (UINT8)((NCOValue&0x000000ff));

	HDICTBX_Print("Updage NCO 0x%02x,0x%02x,0x%02x\n",temp3,temp2,temp1);

	/*设置DTMB相关寄存器*/
	err = HDIC2311_WriteRegister(0x0028,temp1);
	err |= HDIC2311_WriteRegister(0x0029,temp2);
	err |= HDIC2311_WriteRegister(0x002A,temp3);

	/*设置Cable相关寄存器*/
	err |= HDIC2311_WriteRegister(0x0111,temp1);
	err |= HDIC2311_WriteRegister(0x0112,temp2);
	err |= HDIC2311_WriteRegister(0x0113,temp3);
	
	return err;
}

static double HDIC2311_ABS(double x)
{
    return x < 0 ? -x : x;
}

/************************************
** 根据晶振和tuner中频计算NCO
*************************************/
UINT8 HDIC2311_CalculateNCOValue(double TunerIF,UINT8 CrystalSelect,UINT32 *NCOValue)
{
	UINT8 err = HDIC_NO_ERROR;
	double FrequencySample=0;
	double Temp;

	if(TunerIF == 0)
	{
		*NCOValue = 0;
		return err;
	}

	if(CrystalSelect == HDIC2311_CRYSTAL_27M )
	{
		FrequencySample = 30.375;
	}
	else if(CrystalSelect == HDIC2311_CRYSTAL_304M )
	{
		FrequencySample = 30.4;
	}
	else if(CrystalSelect == HDIC2311_CRYSTAL_24M )
	{
		FrequencySample = 30.75;
	}
	else if(CrystalSelect == HDIC2311_CRYSTAL_16M )
	{
		FrequencySample = 30.5;
	}

	Temp = HDIC2311_ABS(FrequencySample - TunerIF);
	if(Temp<TunerIF)
	{
		*NCOValue =(UINT32)(Temp/FrequencySample * 0x800000);
	}
	else
	{
		*NCOValue =(UINT32)(TunerIF/FrequencySample * 0x800000);
	}
	return err;
}

/************************************
** 获取地面实际信号的格式
*************************************/
UINT8 HDIC2311_GetParameters(UINT8 *CarrierMode, UINT8 *pmode,
		UINT8 *prate, UINT8 *pinterleaverLength,UINT8 *Pnmode,UINT8 *PnVariable)
{
	UINT8 err = HDIC_NO_ERROR;
	UINT8 ReadData1,ReadData2;

	err = HDIC2311_ReadRegister(0x0005,&ReadData1);
	err |= HDIC2311_ReadRegister(0x0006,&ReadData2);
	
	*Pnmode = ReadData1&0x03;
	*PnVariable = ReadData1&0x04;
	*PnVariable >>= 2;
	*CarrierMode = ReadData1&0x10;
	*CarrierMode >>= 4;

	*pmode = ReadData2&0x07;
	*prate = ReadData2&0x30;
	*prate >>= 4;
	*pinterleaverLength = ReadData2&0x40;
	*pinterleaverLength >>= 6;

	return err;
}


/****************************
多载波情况下，获取信噪比
****************************/
UINT8 HDIC2311_GetSignalSNRMC(double *SignalSNR)
{
	UINT8 err = HDIC_NO_ERROR;
	UINT8 ReadData1 = 0, ReadData2 = 0;
	UINT32 value_reg;

	err = HDIC2311_ReadRegister(0x02C9,&ReadData1);
	err |= HDIC2311_ReadRegister(0x02C8,&ReadData2);
	
	value_reg = ReadData1*256+ReadData2;

	if((value_reg!=0)&&(value_reg!=0xFFFF))
	{
		if(value_reg<16)
		{
			value_reg=16;
		}
#if SYSTEM_NOT_SUPPORT_LOG10		
		*SignalSNR = Log10_Convert((UINT32)(value_reg/16));
#else
		*SignalSNR = 10*log10((double)(value_reg)/16);
#endif		
	}

	//HDICTBX_Print("SNR_MC %d ",(UINT8)(*SignalSNR));

	return err;
}

/****************************
单载波情况下，获取信噪比
****************************/
UINT8 HDIC2311_GetSignalSNRSC(double *SignalSNR)
{
	UINT8 err = HDIC_NO_ERROR;
	UINT8 ReadData1 = 0, ReadData2 = 0;
	UINT32 value_reg;
	UINT8 Pnmode;
	UINT32 PnValue = 0;
	double value_temp=0;

	err = HDIC2311_ReadRegister(0x0005,&ReadData1);
	Pnmode = ReadData1&0x03;

	if(Pnmode == HDIC_PN_945)
	{
		PnValue = 945;
	}
	else if(Pnmode == HDIC_PN_595)
	{
		PnValue = 595;
	}
	else if(Pnmode == HDIC_PN_420)
	{
		PnValue = 420;
	}

	err |= HDIC2311_ReadRegister(0x02A5,&ReadData1);
	err |= HDIC2311_ReadRegister(0x02A4,&ReadData2);
	
	value_reg = ReadData1*256+ReadData2;
	if((value_reg!=0)&&(value_reg!=0xFFFF))
	{
		value_temp=40.5*(double)(PnValue)/value_reg;
	    if(value_temp<1)
		{
			value_temp=1;
		}
#if SYSTEM_NOT_SUPPORT_LOG10	
		*SignalSNR = Log10_Convert((UINT32)value_temp);
#else
		*SignalSNR = 10*log10(value_temp);
#endif
	}
	else
	{
		*SignalSNR = 0;
	}

	//HDICTBX_Print("SNR_SC %d ",(UINT8)(*SignalSNR));
	return (err);
}

/****************************
获取信噪比
****************************/
UINT8 HDIC2311_GetSignalSNR(double *SignalSNR)
{
	UINT8 err = HDIC_NO_ERROR;
	UINT8 ReadData1 = 0;
	UINT8 CarrierMode;

	/*获取工作模式,DTMB或Cable*/
	if(HD2311_WorkMode == HDIC2311_DTMB_SELECT)
	{
		err =HDIC2311_ReadRegister(0x0005,&ReadData1);
		CarrierMode = ReadData1&0x10;
		CarrierMode >>= 4;

		if(CarrierMode == HDIC_CARRIER_SINGLE)
		{
			err |=HDIC2311_GetSignalSNRSC(SignalSNR);
		}
		else if(CarrierMode == HDIC_CARRIER_MULTI)
		{
			err |=HDIC2311_GetSignalSNRMC(SignalSNR);
		}
	}
	#ifdef OPEN_DVB_C_MODE
	else if(HD2311_WorkMode == HDIC2311_CABLE_SELECT)
	{
		err |= HDIC2311_GetCableSignalSNR(SignalSNR);
	}
	#endif
	return err;
}

/****************************
有线信号情况下获取信噪比
****************************/
#ifdef OPEN_DVB_C_MODE
UINT8 HDIC2311_GetCableSignalSNR(double *SignalSNR)
{
	UINT8 err = HDIC_NO_ERROR;
	UINT8 ReadData1 = 0;

	err = HDIC2311_ReadRegister(0x0196,&ReadData1);
	
#if SYSTEM_NOT_SUPPORT_LOG10	
	*SignalSNR = 37.35 - Log10_Convert(ReadData1 + 1);
#else
	*SignalSNR = 37.35 - 10*log10((double)(ReadData1 + 1));
#endif
	
	return (err);
}

/****************************
有线信号情况下获取LDPC误码率
****************************/
UINT8 HDIC2311_GetCableLdpcBER(double *pLdpcBER)
{
	UINT8 err = HDIC_NO_ERROR;
	UINT8 ReadData1 = 0, ReadData2 = 0;

	err = HDIC2311_ReadRegister(0x0194,&ReadData1);
	err |= HDIC2311_ReadRegister(0x0195,&ReadData2);
	//HDICTBX_Print("Block BER 0x94,0x95: %02x,%02x,",ReadData1,ReadData2);
	
	*pLdpcBER =(double)((double)(ReadData1*256+ReadData2)/65536);

	return (err);
}

/****************************
有线信号情况下获取误码率
****************************/
UINT8 HDIC2311_GetCableSignalBER (double *pSignalBER)
{
	UINT8 err = HDIC_NO_ERROR; 
	UINT8 ReadData1 = 0, ReadData2 = 0,ReadData3 = 0;

	HDIC2311_ReadRegister(0x0191,&ReadData1);
	HDIC2311_ReadRegister(0x0192,&ReadData2);
	HDIC2311_ReadRegister(0x0193,&ReadData3);

//	HDICTBX_Print("BER 0x91,0x92,0x93: %02x,%02x,%02x",ReadData1,ReadData2,ReadData3);
	*pSignalBER = (double)((double)((ReadData1%0x0f)*65536+ReadData2*256+ReadData3)/96468992);

	return (err);
}
#endif

/****************************
地面信号情况下获取LDPC误码率
****************************/
UINT8 HDIC2311_GetLdpcBER(double *pLdpcBER)
{
	UINT8 err = HDIC_NO_ERROR;
	UINT8 ReadData1 = 0, ReadData2 = 0;

	/*获取工作模式,DTMB或Cable*/
	if(HD2311_WorkMode == HDIC2311_DTMB_SELECT)
	{
		HDIC2311_ReadRegister(0x00AA,&ReadData1);
		HDIC2311_ReadRegister(0x00A9,&ReadData2);
	
		*pLdpcBER = (double)((ReadData2&0x0f)*256+ReadData1)/4096;
	}
	#ifdef OPEN_DVB_C_MODE
	else if(HD2311_WorkMode == HDIC2311_CABLE_SELECT)
	{
		HDIC2311_GetCableLdpcBER(pLdpcBER);
	}
	#endif
	return (err);
}

/****************************
地面信号情况下获取误码率
****************************/
UINT8 HDIC2311_GetSignalBER (double *pSignalBER)
{
	UINT8 err = HDIC_NO_ERROR;
	UINT8 ReadData1 = 0, ReadData2 = 0;

	/*获取工作模式,DTMB或Cable*/
	if(HD2311_WorkMode == HDIC2311_DTMB_SELECT)
	{
		HDIC2311_ReadRegister(0x00A7,&ReadData1);
		HDIC2311_ReadRegister(0x00A8,&ReadData2);

		*pSignalBER = (double)(ReadData1*256+ReadData2)/6666664;
	}
	#ifdef OPEN_DVB_C_MODE
	else if(HD2311_WorkMode == HDIC2311_CABLE_SELECT)
	{
		HDIC2311_GetCableSignalBER(pSignalBER);
	}
	#endif
	return (err);
}
/****************************
地面信号情况下获取信号场强
****************************/
UINT8 HDIC2311_GetFieldStrength(UINT32 *FieldStrength)
{
	UINT8 err = HDIC_NO_ERROR;
	UINT8 ReadData1 = 0, ReadData2 = 0;

	HDIC2311_ReadRegister(0x0061,&ReadData1);
	HDIC2311_ReadRegister(0x0062,&ReadData2);
	
	*FieldStrength = (UINT16)(ReadData1*256 + ReadData2);

	//HDICTBX_Print("FieldStrength %d\n",*FieldStrength);
	
	return (err);
}


/************************************
** 芯片软复位
*************************************/
UINT8 HDIC2311_SoftReset(void)
{
	UINT8 err = HDIC_NO_ERROR;
	UINT8 ReadData = 0,ReadData1=0;

	/*获取工作模式,DTMB或Cable*/
	if(HD2311_WorkMode == HDIC2311_DTMB_SELECT)
	{
		HDIC2311_WriteRegister(0x0009,0x10);
		HDIC2311_Wait(10); /* wait 1 ms */
		HDIC2311_WriteRegister(0x0009,0x00);	
	}
	else if(HD2311_WorkMode == HDIC2311_CABLE_SELECT)
	{
		HDIC2311_WriteRegister(0x0103,0x01);
		HDIC2311_Wait(10); /* wait 10 ms */
		HDIC2311_WriteRegister(0x0103,0x00);
	}
	return (err);
}

/************************************
** 设置tuner类型，
** 零中频HDIC_TUNER_ZERO_IF
** 或其他
*************************************/
UINT8 HDIC2311_SetTunerType(UINT8 Type)
{
	UINT8 err = HDIC_NO_ERROR;
	UINT8 ReadData = 0,WriteData = 0;
	
	err = HDIC2311_ReadRegister(0x0007,&ReadData);
	if(Type==HDIC_TUNER_ZERO_IF)
	{
		WriteData = ReadData|0x20;
	}
	else
	{
		WriteData = ReadData&0xDF;
	}
	err |= HDIC2311_WriteRegister(0x0007,WriteData);

	return (err);
}
/************************************
** 设置TS的输出格式
*************************************/
UINT8 HDIC2311_SetTsFormat(UINT8 Serial, UINT8 CLK_Polarity, UINT8 CLK_Valid, UINT8 Null_Packet, UINT8 Serial_Output)
{
	UINT8 err = HDIC_NO_ERROR;
	UINT8 ReadData = 0,WriteData = 0;

	err = HDIC2311_ReadRegister(0x0007,&ReadData);
	WriteData = ReadData&0xE0;

	if(Serial==HDIC_TS_OUTPUT_SERIAL)
	{
		WriteData |= 0x01;
	}

	if(CLK_Polarity==HDIC_TS_DATA_SAMPLE_FALLING)
	{
		WriteData |= 0x02;
	}
	
	if(CLK_Valid==HDIC_TS_CLK_WITH_TSVLD)
	{
		WriteData |= 0x04;
	}	

	if(Null_Packet == HDIC_TS_NULLPACKET_DELETED )
	{
		WriteData |= 0x08;
	}

	if(Serial_Output == HDIC_TS_SERIAL_OUTPUT_D0)
	{
		WriteData |= 0x10;
	}

	err |= HDIC2311_WriteRegister(0x0007,WriteData);

	HDICTBX_Print("TS Format 0x%02x\n",WriteData);	

	return (err);
}

/****************************
打开I2C转发功能
****************************/
UINT8 HDIC2311_OpenTunerI2C(UINT8 Address)
{
	UINT8 err = HDIC_NO_ERROR;
	UINT8 ReadData = 0;

	//HDICTBX_Print("Set 2311 Tuner I2C Address 0x%02x!!\n",Address);
	Address &= 0xFE;
	err = HDIC2311_WriteRegister(0x000F,Address);
	err |= HDIC2311_WriteRegister(0x000E,0x01);
	return (err);

}
/****************************
关闭I2C转发功能
****************************/
UINT8 HDIC2311_CloseTunerI2C(void)
{
	UINT8 err = HDIC_NO_ERROR;

	//HDICTBX_Print("2311 Close Tuner I2C!\n");

	err = HDIC2311_WriteRegister(0x000E,0x00);

	return (err);
}


/********************************
获取信号强度
如果tuner的AGC是自控的，请调用tuner内部的信号强度函数
********************************/
UINT8 HDIC2311_GetSignalStrength(UINT8 *SignalStrength)
{
	UINT8 err = HDIC_NO_ERROR;
	UINT32 FieldStrength;

	err = HDIC2311_GetFieldStrength(&FieldStrength);

	/*可以调整这个计算公式，去得到期望的合理的SignalStrength值，归一化到0～100
	*/
	if(FieldStrength>=0x800)
		FieldStrength = 0x800;
	if(FieldStrength<=0x418)
		FieldStrength = 0x418;
	*SignalStrength = (UINT8)((0x800 - FieldStrength)/12);	

	HDICTBX_Print("SignalStrength %d !!!\n",*SignalStrength);
	
	return err;
}

/********************************
获取信号质量
********************************/
UINT8 HDIC2311_GetSignalQuality(UINT8 *SignalQuality)
{
	UINT8 err = HDIC_NO_ERROR;
	double SignalSNR;

	err = HDIC2311_GetSignalSNR(&SignalSNR);

	*SignalQuality = (UINT8)SignalSNR;
	(*SignalQuality) *= 3;
	if(*SignalQuality>=98)
	{
		*SignalQuality = 98;
	}
	HDICTBX_Print("SignalQuality %d !!!\n",*SignalQuality);
	return err;
}

/********************************
芯片初始化，在系统初始化时调用
********************************/
UINT8 HDIC2311_Init()
{
	UINT8 err = HDIC_NO_ERROR;
	UINT32 NCOValue = 0;
	#if 0  // use global parameter
	HDIC2311_Init_Info InitInfo;
	#else
	extern HDIC2311_Init_Info InitInfo;
	#endif
	
	/*初始化配置HD2311*/
	InitInfo.WorkMode = HDIC2311_DTMB_SELECT;/*工作模式：地面和有线*/
	InitInfo.TunerType = HDIC_TUNER_NORMAL; /*高频头类型，零中频或其他 */ 
	/*高频头的中频，单位MHz，零中频高频头请直接赋值0*/
	InitInfo.TunerIF = 4.5;  /*高频头的中频，单位MHz，零中频高频头请直接赋值0*/
	InitInfo.CrystalSelect = HDIC2311_CRYSTAL_27M;/*晶振选择，单位MHZ，有16MHZ 24MHZ 27MHZ 30.4MHZ*/
	InitInfo.TsType = HDIC_TS_OUTPUT_SERIAL;/*TS流输出方式：串行或者并行两种*/
	InitInfo.TsClockPolarity = HDIC_TS_DATA_SAMPLE_FALLING; /*数据采样是在时钟上升沿或下降沿*/
	InitInfo.TsClockValid = HDIC_TS_CLK_WITH_TSVLD;/*时钟有效方式，一直有效或随valid信号*/
	InitInfo.TsNullPacket = HDIC_TS_NULLPACKET_DELETED; /*空包使能与否*/
	InitInfo.TsSerialOutPut = HDIC_TS_SERIAL_OUTPUT_D0; /*串行TS时，输出在TS_D7或TS_D0*/
	InitInfo.SystemBandwidth = HDIC2311_BANDWIDTH_8M; /*设置系统带宽*/
	
	/*对STB_N/PLL_RST管脚复位，在上电时对standby管脚复位*/
    err = HDIC2311_PLLReset();

	/*硬复位2311*/
	//HDIC2311_HWReset();
   	/*设置系统带宽*/
	err |= HDIC2311_SetSytemBandwidth(InitInfo.SystemBandwidth,InitInfo.CrystalSelect);
	
	/*设置工作模式，地面或有线*/
	err |=HDIC2311_SetWorkMode(InitInfo.WorkMode);
	/*根据tuner IF和晶振计算NCO*/
	err |=HDIC2311_CalculateNCOValue(InitInfo.TunerIF,InitInfo.CrystalSelect,&NCOValue);
	/*设置NCO*/
	err |=HDIC2311_SetNCOValue(NCOValue);
	/*设置tuner类型，零中频或其他*/
	err |=HDIC2311_SetTunerType(InitInfo.TunerType);
	/*设置TS输出格式*/
	err |=HDIC2311_SetTsFormat(InitInfo.TsType,InitInfo.TsClockPolarity,InitInfo.TsClockValid,InitInfo.TsNullPacket,InitInfo.TsSerialOutPut);
	/*设置工作模式，地面或有线*/
	//HDIC2311_SetWorkMode(InitInfo.WorkMode);
	
	
	/*设置tuner类型，零中频或其他*/
	//HDIC2311_SetTunerType(InitInfo.TunerType);
	/*设置TS输出格式*/
	//HDIC2311_SetTsFormat(InitInfo.TsType,InitInfo.TsClockPolarity,InitInfo.TsClockValid,InitInfo.TsNullPacket,InitInfo.TsSerialOutPut);
	
	return (err);
}

/********************************
检测信号锁定状态
return:  0 unlocked, 1 locked
********************************/
char CDT_HDIC_CheckLockStatus()
{
	UINT8 err = HDIC_NO_ERROR;
	UINT8 CheckLockTemp,LockResult,locked,status;
	UINT8 SignalDetect = 0;
	
	HDICTBX_Print("@@@@CDT_HDIC_CheckLockStatus():begin\n");
	for (CheckLockTemp=0; CheckLockTemp<50; CheckLockTemp++)
	{
		HDIC2311_IsDemodLocked(0,&LockResult,&locked,&status); // ylzhu not print info.
		
		if((status&0xf0) > 0x20)
		{
			SignalDetect = 1;
		}
		else
		{
			SignalDetect = 0;
		}
		
		if((SignalDetect == 0)&&(CheckLockTemp == 15)) // change by ylzhu orig valid : CheckLockTemp == 20
		{
			HDICTBX_Print("---> [2311] 2311 DTMB No Signal \n");
			// unlock : 0;
			err = 0;
			break;
		}
		if (LockResult == 1)
		{
			HDICTBX_Print("---> [2311] 2311 DTMB is locked \n");
			// lock : 1;
			err = 1;
			break;
		}
		else if((SignalDetect == 1) && (CheckLockTemp == 25))
		{
			// 信号弱, 或在有信号附近频道.
			// unlock : 0;
			err = 0;
			break;
		}
		HDIC2311_Wait(100); /* wait 100 ms */
	}
	return err;
}

/********************************
地面信号时锁频函数
********************************/
BOOL HDIC2311_DTMBSetFrequency(UINT32 Frequency)
{
	UINT8 CheckLockTemp,LockResult,locked,status;
	BOOL SignalDetect = FALSE;
	UINT8 err = HDIC_NO_ERROR;
	UINT8 SignalLockStatus = 0;  // 0 unlocked; 1 locked

	#if 0  // note ylzhu change as EVB control
	/*设置HD2311为地面工作模式*/
	HDIC2311_SetWorkMode(HDIC2311_DTMB_SELECT);

	/********************
	** 需要首先设置高频头
	********************/
	if(HDIC2311_SetTuner(Frequency)!=TRUE)
	{
		return FALSE;
	}
	#endif
	/*设置为自动工作模式*/
	err = HDIC2311_SetAutoMode();
	#if 0  // note ylzhu change as EVB control
	HDIC2311_SoftReset();
	#endif
	
	#if 0   // set freq, don't need to check signal lock, use other API. ylzhu change 2014/02/17
	for (CheckLockTemp=0; CheckLockTemp<50; CheckLockTemp++)
	{
		HDIC2311_IsDemodLocked(FALSE,&LockResult,&locked,&status); // ylzhu not print info.
		HDICTBX_Print("LockResult=0x%X,locked=0x%X,status=0x%X\n", LockResult, locked, status);
		if((status&0xf0) > 0x20)
		{
			SignalDetect = 1;
		}
		else
		{
			SignalDetect = 0;
		}
		
		if((SignalDetect == 0)&&(CheckLockTemp == 15)) //change by ylzhu orig valid : CheckLockTemp == 20
		{
			HDICTBX_Print("---> [2311] 2311 DTMB No Signal \n");
			err = HDIC_OTHER_ERROR;
			// return HDIC_OTHER_ERROR;	
			break;
		}
		if (LockResult == 1)
		{
			HDICTBX_Print("---> [2311] 2311 DTMB is locked \n");
			err = HDIC_NO_ERROR;
			break;
			//return HDIC_NO_ERROR;
		}
		else if((SignalDetect == 1) && (CheckLockTemp == 25))
		{
			// 信号弱, 或在有信号附近频道.
			err = HDIC_OTHER_ERROR;
			break;
		}
		HDIC2311_Wait(100); /* wait 100 ms */
	}
	#else
	SignalLockStatus = CDT_HDIC_CheckLockStatus();
	if(SignalLockStatus == 0)
	{
		err = 1;
	}
	else
	{
		err = 0;
	}
	#endif

	HDICTBX_Print("HDIC2311_DTMBSetFrequency():over, err=0x%X\n", err);
	return (BOOL)err;
}

/********************************
手动设置地面情况下信号格式
********************************/
BOOL HDIC2311_DTMBManualSetFrequency(UINT32 Frequency, UINT8 CarrierMode, UINT8 mode,UINT8 rate, UINT8 interleaverLength,UINT8 pnmode,UINT8 PnVariable)
{
	UINT8 CheckLockTemp,LockResult,locked,status;
	BOOL SignalDetect = FALSE;
	
	/*设置HD2311为地面工作模式*/
	HDIC2311_SetWorkMode(HDIC2311_DTMB_SELECT);

	/********************
	** 需要首先设置高频头
	********************/
	if(HDIC2311_SetTuner(Frequency)!=TRUE)
	{
		return FALSE;
	}

	/*设置为手动工作模式*/
	HDIC2311_SetManualMode(CarrierMode, mode,rate, interleaverLength,pnmode,PnVariable);
	HDIC2311_SoftReset();
		
	for (CheckLockTemp=0; CheckLockTemp<50; CheckLockTemp++)
	{
		HDIC2311_IsDemodLocked(TRUE,&LockResult,&locked,&status);
		HDIC2311_Wait(100); /* wait 100 ms */
		if((status&0xf0) > 0x20) /*无信号2秒退出*/ 
		{
			SignalDetect = TRUE;
		}
		if((SignalDetect == FALSE)&&(CheckLockTemp == 20))
		{
			HDICTBX_Print("---> [2311] 2311 DTMB No Signal \n");
			return FALSE;
		}
		if (LockResult == 1)
		{
			HDICTBX_Print("---> [2311] 2311 DTMB is locked \n");
			return TRUE;
		}
	}
	return FALSE;
}

/********************************
有线信号时锁频函数
********************************/
#ifdef OPEN_DVB_C_MODE
BOOL HDIC2311_CableSetFrequency(UINT32 Frequency, UINT8 CableMode, UINT16 CableRate)
{
	UINT8 CheckLockTemp,LockResult,locked,status;

	/*设置HD2311为有线工作模式*/
	HDIC2311_SetWorkMode(HDIC2311_CABLE_SELECT);
	
	/********************
	** 需要首先设置高频头
	********************/
	if(HDIC2311_SetTuner(Frequency)!=TRUE)
	{
		return FALSE;
	}
	
	/*设置信号参数*/
	HDIC2311_SetCableMode(CableMode);
	HDIC2311_SetCableRate(CableRate);
	HDIC2311_SoftReset();
	for (CheckLockTemp=0; CheckLockTemp<40; CheckLockTemp++)
	{
		HDIC2311_IsDemodLocked(TRUE,&LockResult,&locked,&status);
		HDIC2311_Wait(50); /* wait 50 ms */

		if (LockResult == 1)
		{
			HDICTBX_Print("---> [2311] 2311 Cable is locked \n");
			return TRUE;
		}
	}
	return FALSE;
}
#endif

