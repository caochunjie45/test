/***************************************************************************** 
Copyright 2012, AltoBeam Inc. All rights reserved.
     
File Name: AtbmCommon.c
   
Version: 1.2.2
Released 2015-03-19

Description: Example programming interface for ATBM781x

Important struct:

gstATBM781xDvbConfig
           All hardware dependent parameters should be customized according to system design          

Important functions:

ATBM781xPowerOnInit()
           This function is the first function should be called. default DVB-T/T2 8MHz Bandwidth. 
           ATBM781xSetDvbtBandwidth(unsigned char u8bandwidthMHz) function can be used to set Bandwidth for DVB-T/T2.

ATBM781xSetDVBTxMode( uint8   ui8DvbTxBandwidthType)
           This function sets the demodulator working in DVB-T/T2 auto detection mode.
	    The demodulator can receive DVB-T and DVB-T2 without discrimination on register setting.
           For example, it can be also used in mode switching between DVB-T/T2 and DVB-C
		   
           
ATBM781xSetDVBCMode()
           This function sets the demodulator working in DVB-C mode.
           For example, it can be used in mode switching between DVB-T/T2 and DVB-C

ATBM781xSetDvbSxMode() 
          This function sets the demodulator working in DVB-S/S2 mode. DVB-S/S2 auto detection.

ATBM781xTSLockFlag(uint8 *LockStatus);
           This function checks the demodulator TS locking status.   
******************************************************************************/
//#include <stdio.h>
//#include "Atbm781x.h"
#include "drv_atbm_i.h"

#define ATBM781x_DVB_SS2_SUPPORT    1 /*if DVB-S/S2 not used, set 0*/
/*#define ATBM781x_DVB_DVBTT2_SUPPORT 1 */
/*#define ATBM781x_DVB_DVBC_SUPPORT   1 */


extern 	unsigned int gATBM781xDiseqcClockKHz;
/*ATBM781x_DVBT2_PARAMS gDvbt2Params;
ATBM781x_DVBT_PARAMS gDvbtParams;*/
stATBM781xDvbConfig gstATBM781xDvbConfig;

unsigned char gATBM781xDvbSxInitFlag = 0; /*Should not be changed, means in init process or not in init process. */

unsigned char gATBM781xPowerOnInited = 0;
unsigned char ui8ATBM781xSlaveAddress = 0x80;

uint8    ui8ATBM781xSlaveAddr         = 0x80;   /*either 0x80 or 0x88 */

/*If version number > 2.55, should add another reserved register, current only one register used. */
uint8         u8ATBM781xSoftwareVersion = 122;     /*SW guide version = ui8ATBM781xSoftwareVersion, just record it */

int i32SNRLogTable[]=
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

/****************************************************************************
Function:    ATBM781xSleep
Parameters:  ui32ms :Unit is Millisecond
Return:      Read  Status:ATBM781x_SUCCESSFUL is Normal and other is abnormal.
Description:
This function implement CPU time delay unit is millisecond ,it should be implemented by user	 	  
****************************************************************************/
ATBM781x_STATUS   ATBM781xSleep(uint32 ui32ms)
{
	ATBM781x_STATUS ui8Status =  ATBM781x_SUCCESSFUL; 

 	__u32 delay_time = 0;
    delay_time = ui32ms/10;
    esKRNL_TimeDly(delay_time);
    
	return ui8Status;
}
/****************************************************************************
Function:    ATBM781xMutexIni
Parameters:  none
Return:      Read  Status:ATBM781x_SUCCESSFUL is Normal and other is abnormal.
Description:
This function initialize the mutex for   ATBM781xMutexLock and ATBM781xMutexUnlock call
        it should be implement by user.generally ,user can define an handle(windows)
		or a variable(Linux) in  current file as a global parameter,then initialize
		it in the function

		or using the following sample code in Linux

For example:static pthread_mutex_t atbm_mutex = PTHREAD_MUTEX_INITIALIZER;
****************************************************************************/
ATBM781x_STATUS ATBM781xMutexIni(void)
{
	/*pthread_mutex_init(&atbm_mutex, NULL);*/
	return ATBM781x_SUCCESSFUL;
}
/****************************************************************************
Function:    ATBM781xMutexLock
Parameters:  none
Return:      none
Description:
This function is to get the mutex for I2C Read,	 it should be implement by user	.
         if you implement the function ATBM781xMutexIni,you can get the mutex
		 in this function.
****************************************************************************/
ATBM781x_STATUS ATBM781xMutexLock(void)
{
/*pthread_mutex_lock(&atbm_mutex);*/
	return ATBM781x_SUCCESSFUL;
}
/****************************************************************************
Function:    ATBM781xMutexUnlock
Parameters:  none
Return:      Read  Status:ATBM781x_SUCCESSFUL is Normal and other is abnormal.
Description:
This function is to get the mutex for I2C Write,it should be implement by user	  
              if you implement the function ATBM781xMutexIni,you can release the mutex
			  in this function.
****************************************************************************/
ATBM781x_STATUS ATBM781xMutexUnlock(void)
{
  /* pthread_mutex_unlock(&atbm_mutex);*/
	return ATBM781x_SUCCESSFUL;
}

extern int32 I2CReadOneStep(uint8 ui8I2CSlaveAddr, uint16 addr_length, uint8 *addr_dat,  uint16 data_length, uint8 *reg_dat)
{
	__s32	u8tmp,ret = ATBM781x_FAILED;
	__u16	slave_addr;
	
	slave_addr = ui8I2CSlaveAddr>>1;
	for(u8tmp = 0; u8tmp < 3; u8tmp++)
	{
		ret = iic_read(slave_addr,addr_dat, 2,reg_dat, data_length);
		if(0 == ret)
		{
			ret = ATBM781x_SUCCESSFUL;
			break;
		}
		else
		{
			ret = ATBM781x_FAILED;
			eDbug("read IIC failed \n");
			esKRNL_TimeDly(1);
		}
	}
	
	return ret;
}



extern int32 I2CWriteWithRestart(uint8 ui8I2CSlaveAddr, uint8 addr_length, uint8 *addr_dat,  uint8 data_length, uint8 *reg_dat)
{
	/*I2C write in one step format, please port this function if needed*/	
	__s32	u8tmp,ret = ATBM781x_FAILED;
	__u16	slave_addr;
	
	slave_addr = ui8I2CSlaveAddr>>1;
	for(u8tmp = 0; u8tmp < 3; u8tmp++)
	{
		ret = iic_write(slave_addr,addr_dat, 2 ,reg_dat, data_length);
		if(0 == ret)
		{
			ret = ATBM781x_SUCCESSFUL;
			break;
		}
		else
		{
			ret = ATBM781x_FAILED;
			eDbug("write IIC failed \n");
			esKRNL_TimeDly(1);
		}
	}
	
	return ret;
	
}

/****************************************************************************
Function:    ATBM781xI2CRead
Parameters:   ui8BaseAddr register Base Address, ui8RegisterAddr:register offset Address;
ui8RegValue:The Pointer Which Return The Data Read Form Chip.
Return:      Read  Status:ATBM781x_SUCCESSFUL is Normal and other is abnormal.

Description:
This function used I2CRead to read  data form chip register, it should be implemented by user	 	  
****************************************************************************/
ATBM781x_STATUS   ATBM781xI2CRead(uint8 ui8BaseAddr, uint8 ui8RegisterAddr,uint8 *ui8RegValue)
{
	/*CI2CBUS::read_demod_bda(gDemod_addr,ui8BaseAddr, ui8RegisterAddr,1,ui8RegValue);*/
	ATBM781x_STATUS ui8Status =  ATBM781x_SUCCESSFUL; 
    uint8 ui8AddrData[2];

	ui8AddrData[0] = ui8BaseAddr;
	ui8AddrData[1] = ui8RegisterAddr;


	  /*Debug ATBM781x I2C used  if((ui8BaseAddr == 0x00)&&(ui8RegisterAddr == 0x00))  *ui8RegValue ?= 0xa4; */

	  /*One step I2C format, please port this function according to I2C format of this function..*/
	/*return I2CRead(ui8ATBM781xSlaveAddr, 2, ui8AddrData, 1, ui8RegValue);*/
    ui8Status = I2CReadOneStep(ui8ATBM781xSlaveAddr, 2, ui8AddrData, 1, ui8RegValue);	

	return ui8Status;
}
/****************************************************************************
Function:    ATBM781xI2CWrite
Parameters:  ui8BaseAddr register Base Address, ui8RegisterAddr:register offset Address;
                    ui8data:  Data which will be write to chip.
Return:         Write status:ATBM781x_SUCCESSFUL is normal and other is abnormal.

Description:
This function used I2CWrite to write data to chip register,it should be implemented by user	 	  
****************************************************************************/
ATBM781x_STATUS   ATBM781xI2CWrite(uint8 ui8BaseAddr, uint8 ui8RegisterAddr, uint8 ui8data)
{ 
	/*CI2CBUS::write_demod_bda(gDemod_addr,ui8BaseAddr, ui8RegisterAddr,1,ui8data);*/
	ATBM781x_STATUS ui8Status =  ATBM781x_SUCCESSFUL; 
	uint8 ui8I2CData[2]; 
	uint8 reg_data[1];

	ui8I2CData[0] = ui8BaseAddr;
	ui8I2CData[1] = ui8RegisterAddr;
	reg_data[0] = ui8data;	

	ui8Status = I2CWriteWithRestart(ui8ATBM781xSlaveAddr, 2, ui8I2CData, 1, reg_data);

	return ui8Status;	
}
/****************************************************************************
Function:    ATBM781xRead
Parameters:  ui8BaseAddr Chip Base Address,eg.0x88; ui8RegisterAddr:Chip Sub Address;
ui8RegValue:The Pointer Which Return The Data Read Form Chip.
Return:      Read  Status:ATBM781x_SUCCESSFUL is Normal and other is abnormal.

Description:
This function used to read  data form chip register	 	  
****************************************************************************/
ATBM781x_STATUS   ATBM781xRead(uint8 ui8BaseAddr, uint8 ui8RegisterAddr,uint8 *ui8RegValue)
{
	ATBM781x_STATUS ui8Status = ATBM781x_SUCCESSFUL;

	ui8Status |= ATBM781xMutexLock();
	ui8Status |= ATBM781xI2CRead( ui8BaseAddr, ui8RegisterAddr, ui8RegValue);
    ui8Status |= ATBM781xMutexUnlock();
	return ui8Status;
}


/****************************************************************************
Function:    ATBM781xWrite
Parameters:  ui8BaseAddr Chip Base Address,eg.0x88; ui8RegisterAddr:Chip Sub Address;
ui8data:Data which will be write to chip.
Return:      Write status:ATBM781x_SUCCESSFUL is normal and other is abnormal.

Description:
This function used to write data to chip register 
****************************************************************************/
ATBM781x_STATUS   ATBM781xWrite(uint8 ui8BaseAddr, uint8 ui8RegisterAddr, uint8 ui8data)
{ 
	ATBM781x_STATUS ui8Status = ATBM781x_SUCCESSFUL;
	ui8Status |=ATBM781xMutexLock();

	ui8Status |=ATBM781xI2CWrite(ui8BaseAddr,ui8RegisterAddr,ui8data);    
	ui8Status |=ATBM781xMutexUnlock();
	//ATBM781xDebugPrint("%2x  %2x  %2x\n",ui8BaseAddr,ui8RegisterAddr,ui8data);
	return ui8Status;
}



/****************************************************************************
Function:   ATBM781xDVBCRead
Parameters: uAddress: Register physical address;ui32RData - data returned by this pointer 
Return:    read status:ATBM781x_SUCCESSFUL is normal and other is abnormal.
Description:
For DVB-C register read	 
******************************************************************************/

ATBM781x_STATUS  ATBM781xDVBCRead(uint32 uAddress,uint32 *ui32RData)
{

	ATBM781x_STATUS ui8Status = ATBM781x_SUCCESSFUL;
	uint8 ui8PData[4];
	ui8Status |=ATBM781xMutexLock();
	ui8Status |= ATBM781xI2CWrite(0x15,0x03,0x0);
	ui8Status |= ATBM781xI2CWrite(0x15,0x01,(uint8)(uAddress&0xff));
	ui8Status |= ATBM781xI2CWrite(0x15,0x02,(uint8)((uAddress>>8)&0xff));

	ui8Status |= ATBM781xI2CWrite(0x01, 0x0d, 1);/*ro_latch_on*/
	ui8Status |= ATBM781xI2CRead(0x15, 0x08, &ui8PData[0]);
	ui8Status |= ATBM781xI2CRead(0x15, 0x09, &ui8PData[1]);
	ui8Status |= ATBM781xI2CRead(0x15, 0x0a, &ui8PData[2]);
	ui8Status |= ATBM781xI2CRead(0x15, 0x0b,  &ui8PData[3]);
	ui8Status |= ATBM781xI2CWrite(0x01, 0x0d,0 );/*ro_latch_off*/
	ui8Status |= ATBM781xMutexUnlock();
	*ui32RData = (ui8PData[3]<<24) + (ui8PData[2]<<16) + (ui8PData[1]<<8) +ui8PData[0];

	return ui8Status;
}
/****************************************************************************
Function:   ATBM781xDVBCWrite
Parameters: ui32Address: Register physical address;ui32Data - 32bit data 
Return:    Write status:ATBM781x_SUCCESSFUL is normal and other is abnormal.
Description:
For DVB-C register write		 
******************************************************************************/
ATBM781x_STATUS  ATBM781xDVBCWrite(uint32 ui32Address, uint32 ui32Data)
{
	/* if(  (true == CI2CBUS::m_RecordSettings)&&(CI2CBUS::m_sw))*/
	/* 	{*/
	/* 		CString str;*/
	/* 		str.Format(L"%x  %x\n",ui32Address,ui32Data);*/
	/* 		CI2CBUS::m_sw.WriteString(str);*/
	/* 		CI2CBUS::m_WriteDvbcFlag=true;*/
	/* 	}*/

	/*-------------------- write --------------------------*/
	ATBM781x_STATUS ui8Status = ATBM781x_SUCCESSFUL;

	 ui8Status |=ATBM781xMutexLock();
	

	ui8Status = ATBM781xI2CWrite(0x15,0x03,0x0);
	ui8Status |= ATBM781xI2CWrite(0x15,0x01,(uint8)(ui32Address&0xff));
	ui8Status |= ATBM781xI2CWrite(0x15,0x02,(uint8)((ui32Address>>8)&0xff));

	ui8Status |= ATBM781xI2CWrite(0x15,0x04,(uint8)(ui32Data&0xff));
	ui8Status |= ATBM781xI2CWrite(0x15,0x05,(uint8)((ui32Data>>8)&0xff));
	ui8Status |= ATBM781xI2CWrite(0x15,0x06,(uint8)((ui32Data>>16)&0xff));
	ui8Status |= ATBM781xI2CWrite(0x15,0x07,(uint8)((ui32Data>>24)&0xff));
	ui8Status |= ATBM781xI2CWrite(0x15, 0x03,0x1);

	ui8Status |= ATBM781xI2CWrite(0x15, 0x03,0x0);
	//ATBM781xDebugPrint("%4x  %8x\n",ui32Address,ui32Data);
	 ui8Status |=ATBM781xMutexUnlock();
	/* 	if(  true ==CI2CBUS::m_RecordSettings)*/
	/* 	{*/
	/* 	   CI2CBUS::m_WriteDvbcFlag=false;*/
	/* 	}*/
	return ui8Status;
}

/****************************************************************************
Function:   ATBM781xDVBSWrite
Parameters: ui16Addr: Register physical address;ui32Data - 32bit data 
Return:    Write status:ATBM781x_SUCCESSFUL is normal and other is abnormal.
Description:
For DVBS and DVBS2 mode register write		 
******************************************************************************/
ATBM781x_STATUS  ATBM781xDVBSWrite(uint16 ui16Addr,uint32 ui32Data)
{

	/*-------------------- write --------------------------*/
	ATBM781x_STATUS atbmStatus = ATBM781x_SUCCESSFUL;
    uint8 u8TSFlag;
/*	uint8 ui8PSetting[] =
	{
		0x29,0x02,0x0,
		0x29,0x00,ui16Addr&0xff,     address low part,
		0x29,0x01,(ui16Addr>>8)&0xff,address high part
		0x29,0x03,(uint8)(ui32Data&0xff),
		0x29,0x4,(uint8)((ui32Data>>8)&0xff),
		0x29,0x5,(uint8)((ui32Data>>16)&0xff),
		0x29,0x6,(uint8)((ui32Data>>24)&0xff), 
		0x29,0x02,0x1,
	};*/
	/* 	if(  (true == CI2CBUS::m_RecordSettings))*/
	/* 	{*/
	/* 		CString str;*/
	/* 		str.Format(L"%x  %x\n",ui16Addr,ui32Data);*/
	/* 		CI2CBUS::m_sw.WriteString(str);*/
	/* 		CI2CBUS::m_WriteDvbcFlag=true;*/
	/* 	}*/
	atbmStatus |= ATBM781xMutexLock();
	atbmStatus |= ATBM781xI2CWrite(0x29,0x02,0x00);/**/
	atbmStatus |= ATBM781xI2CWrite(0x29,0x00,(uint8)(ui16Addr&0xff));/**/
	atbmStatus |= ATBM781xI2CWrite(	0x29,0x01,(uint8)((ui16Addr>>8)&0xff));/**/

	atbmStatus |= ATBM781xI2CWrite(0x29,0x03,(uint8)(ui32Data&0xff));/**/
	atbmStatus |= ATBM781xI2CWrite(0x29,0x4,(uint8)((ui32Data>>8)&0xff));/**/
	atbmStatus |= ATBM781xI2CWrite(0x29,0x5,(uint8)((ui32Data>>16)&0xff));/**/
	atbmStatus |= ATBM781xI2CWrite(0x29,0x6,(uint8)((ui32Data>>24)&0xff));/**/
	atbmStatus |= ATBM781xI2CWrite(0x29,0x02,0x01);/**/
	//ATBM781xDebugPrint("%2x  %2x\n",ui16Addr,ui32Data);
    atbmStatus |= ATBM781xI2CWrite(0x29,0x02,0x00);/**/

	if(!gATBM781xDvbSxInitFlag)
	{
		atbmStatus |= ATBM781xI2CWrite(0x29,0x02,0x00);/**/
		atbmStatus |= ATBM781xI2CWrite(0x29,0x00,0x1c);/**/
		atbmStatus |= ATBM781xI2CWrite(0x29,0x01,0x04);/**/

		atbmStatus |= ATBM781xI2CWrite(0x29,0x03,0);/**/
		atbmStatus |= ATBM781xI2CWrite(0x29,0x04,0);/**/
		atbmStatus |= ATBM781xI2CWrite(0x29,0x05,0);/**/
		atbmStatus |= ATBM781xI2CWrite(0x29,0x06,0);/**/

		atbmStatus |=ATBM781xI2CRead(0x18, 0x2a,&u8TSFlag);
		if(!u8TSFlag)
		{			
			atbmStatus |= ATBM781xI2CWrite(0x29,0x02,0x01);/**/
			atbmStatus |= ATBM781xI2CWrite(0x29,0x02,0x00);/**/
		}

	}
	atbmStatus |=ATBM781xMutexUnlock();

	return atbmStatus;

}
/****************************************************************************
Function:   ATBM781xDVBSRead
Parameters: ui16Addr: Register physical address;ui32RegData - 32bit data read from chip register
Return:    Read status:ATBM781x_SUCCESSFUL is normal and other is abnormal.
Description:
For DVBS and DVBS2 mode register Read		 
******************************************************************************/
ATBM781x_STATUS  ATBM781xDVBSRead(uint16 ui16Addr,uint32 *ui32RegData)
{
	ATBM781x_STATUS atbmStatus = ATBM781x_SUCCESSFUL;
	uint8 u8TSFlag, data[4];

	atbmStatus |=ATBM781xMutexLock();

	atbmStatus |=ATBM781xI2CWrite(0x29,0x02,0x00);/**/
	atbmStatus |= ATBM781xI2CWrite(0x29,0x00,(uint8)(ui16Addr&0xff));/**/
	atbmStatus |= ATBM781xI2CWrite(0x29,0x01,(uint8)((ui16Addr>>8)&0xff));/**/

	atbmStatus |= ATBM781xI2CWrite(0x01, 0x0d,1);/*ro_latch_on*/
	atbmStatus |=ATBM781xI2CRead(0x29, 0x7,&data[0]);
	atbmStatus |=ATBM781xI2CRead(0x29, 0x8,&data[1]);
	atbmStatus |=ATBM781xI2CRead(0x29, 0x9,&data[2]);
	atbmStatus |=ATBM781xI2CRead(0x29, 0xa,&data[3]);
	atbmStatus |=ATBM781xI2CWrite(0x01, 0x0d, 0);/*ro_latch_off*/
	atbmStatus |= ATBM781xI2CWrite(0x29,0x00,0x1c);/*Set default address*/
	atbmStatus |= ATBM781xI2CWrite(0x29,0x01,0x04);/*Set default address*/

	atbmStatus |=ATBM781xI2CRead(0x18, 0x2a,&u8TSFlag);
	if(!u8TSFlag)
	{	
		atbmStatus |= ATBM781xI2CWrite(0x29,0x02,0x00);/**/
		atbmStatus |= ATBM781xI2CWrite(0x29,0x03,0);/**/
		atbmStatus |= ATBM781xI2CWrite(0x29,0x04,0);/**/
		atbmStatus |= ATBM781xI2CWrite(0x29,0x05,0);/**/
		atbmStatus |= ATBM781xI2CWrite(0x29,0x06,0);/**/
		atbmStatus |= ATBM781xI2CWrite(0x29,0x02,0x01);/**/
		atbmStatus |= ATBM781xI2CWrite(0x29,0x02,0x00);/**/
	}
	atbmStatus |=ATBM781xMutexUnlock();
	*ui32RegData = (data[3]<<24) + (data[2]<<16) + (data[1]<<8) +data[0];
	return atbmStatus;
}
/****************************************************************************
Function:    ATBM781xWriteRegArray
Parameters:  *ui8ARegTable, i32TableLen
Return:      I2C Write Status   0 is Success,Other is Failed

Description:
This function writes a group of registers to demodulator	 	 
****************************************************************************/
ATBM781x_STATUS  ATBM781xWriteRegArray(uint8 *ui8ARegTable, int i32TableLen)
{
	ATBM781x_STATUS ui8Status = ATBM781x_SUCCESSFUL;
	int i32Iter;
	for(i32Iter=0;i32Iter<i32TableLen;i32Iter+=3)
	{
		if((i32Iter+2) < i32TableLen)
		{	
			ui8Status |= ATBM781xWrite(ui8ARegTable[i32Iter],ui8ARegTable[i32Iter+1],ui8ARegTable[i32Iter+2]);
		}
		/*ATBM781xDebugPrint("%x %x %x\n", ui8ARegTable[i32Iter],ui8ARegTable[i32Iter+1], ui8ARegTable[i32Iter+2]); 		*/
	}
	return ui8Status;
}


/****************************************************************************
Function:   LeftTable
Description:
use for ATBM781xLog10ConvertdB			 
******************************************************************************/

int LeftTable(int i32InValue)
{
	int i32OutValue = 0;
	int i32Iter;	
	int i32TableSize = sizeof(i32SNRLogTable)/sizeof(int);
	for(i32Iter=0; i32Iter<i32TableSize; i32Iter+=2)
	{	
		if((i32Iter +1) < i32TableSize)
		{		
			if(i32InValue<=i32SNRLogTable[i32Iter+1])
			{		
				i32OutValue = i32SNRLogTable[i32Iter];	
				return i32OutValue;
			}	
		}
	/*	printf("iter = %d, size:%d\n", i32Iter,  i32TableSize);*/
	}
	i32OutValue = 32;
	return i32OutValue;
}
/**********************same as  100 * log10(i32InValue)*************************/

uint32 ATBM781xLog10ConvertdBx10(uint32 i32InValue)
{
	uint8       		ui8Index = 0;
	uint32		        ui32Tmp = 1;
	uint32              ui32InValueLeft = 1;
	uint32              ui32LogValue;    
	uint32              ui32Dvalue = 0;
	if(i32InValue == 0)
	{
		return 0;
	}

	do 
	{	
		ui32Tmp = ui32Tmp << 1;
		if (i32InValue < ui32Tmp)
		{
			break;	
		}
	}
	while(++ui8Index < 32);
	ui32LogValue = 301*ui8Index/100;
	ui32Dvalue = (1<<ui8Index);
	if(ui32Dvalue != 0)
	{	
		ui32InValueLeft = i32InValue*100/ui32Dvalue;
	}
	ui32LogValue = ui32LogValue*10 + LeftTable(ui32InValueLeft);
	/*ATBM781xDebugPrint(" i32InValue = %d  log ret = %d\n", i32InValue,  ui32LogValue);*/
	return ui32LogValue;	
}

uint32 ATBM781xLog10ConvertdB(uint32 i32InValue)
{
	uint8       		ui8Index = 0;
	uint32		        ui32Tmp = 1;
	uint32              ui32InValueLeft = 1;
	uint32              ui32LogValue;  
    uint32              ui32Dvalue = 0;
	if(i32InValue == 0)
	{
		return 0;
	}

	do 
	{	
		ui32Tmp = ui32Tmp << 1;
		if (i32InValue < ui32Tmp)
		{
			break;	
		}
	}
	while(++ui8Index < 32);
	ui32LogValue = 301*ui8Index/100;

	ui32Dvalue = (1<<ui8Index);
	if(ui32Dvalue != 0)
	{	
		ui32InValueLeft = i32InValue*100/ui32Dvalue;
	}
	ui32LogValue = ui32LogValue + LeftTable(ui32InValueLeft)/10;
	/*ATBM781xDebugPrint(" i32InValue = %d  log ret = %d\n", i32InValue,  ui32LogValue);*/
	return ui32LogValue;	
}
/**********************same as  pow(x,y), y should greater than 0**************/

uint32  ATBM781xFixPow(uint32 x,uint32 y)
{
	uint32 ui32Rst = x;
	uint32 ui32Iter;
	if (y == 0)
	{
		return 1;
	}

	for (ui32Iter = 1; ui32Iter<y;ui32Iter++)
	{
		ui32Rst*= x;
	}
	return ui32Rst;
}
/**********************if add calculation overflow ,use this function*************************/
void ATBM781x64Add(ATBM781x64Data *pstSum,uint32 ui32Addend)
{
	uint32 ui32LowPart;
	ui32LowPart = pstSum->ui32Low;
	pstSum->ui32Low += ui32Addend;
	if (ui32LowPart > pstSum->ui32Low)
	{
		pstSum->ui32High++;
	}
}

/**********************2 32bit data multiply, use this function *****************/
void ATBM781x64Mult(ATBM781x64Data *pstRst,uint32 m1, uint32 m2)
{
	uint32 ui32LowPart,ui32Tmp;
	pstRst->ui32Low = (m1&0xffff) * (m2&0xffff);
	pstRst->ui32High = (m1>>16) * (m2>>16);

	ui32LowPart = pstRst->ui32Low;
	ui32Tmp = (m1>>16) * (m2&0xffff);
	pstRst->ui32Low  += (ui32Tmp<<16);
	pstRst->ui32High += (ui32Tmp>>16);
	if(ui32LowPart > pstRst->ui32Low)
	{
		pstRst->ui32High++;
	}

	ui32LowPart = pstRst->ui32Low;
	ui32Tmp = (m2>>16) * (m1&0xffff);
	pstRst->ui32Low  += (ui32Tmp<<16);
	pstRst->ui32High += (ui32Tmp>>16);
	if(ui32LowPart > pstRst->ui32Low)
	{
		pstRst->ui32High++;
	}
}

uint8  ATBM781x64ComPare(ATBM781x64Data stPara,ATBM781x64Data stPara1)
{
	uint8 ui8Flag = 0;/*default :stPara<=stPara1*/
	if (stPara.ui32Low>=stPara1.ui32Low)
	{
		if (stPara.ui32High >= stPara1.ui32High)
		{
			ui8Flag = 1;
		}
	}
	else
	{
		if (stPara.ui32High > stPara1.ui32High)
		{
			ui8Flag = 1;
		}
	}
	return ui8Flag;

}
uint32 ATBM781x64Div(ATBM781x64Data stDivisor,ATBM781x64Data stDividend)
{
	uint8  ui8DivLeftShit = 0;
	uint32 ui32Rst = 0;
	unsigned int ui32flag = 1;
	ATBM781x64Data stDataTmp = stDividend;
	if (stDivisor.ui32High == 0&&stDivisor.ui32Low == 0)
	{
		return 0;
	}
	else if (stDividend.ui32High == 0&&stDividend.ui32Low == 0)
	{
		return 0;
	}
	else if (stDividend.ui32High > stDivisor.ui32High)
	{
		return 0;
	}
	else if (!ATBM781x64ComPare(stDivisor,stDividend))
	{
		return 0;
	}
	else if (stDividend.ui32High == 0 && stDivisor.ui32High == 0)
	{
		return stDivisor.ui32Low/stDividend.ui32Low;
	}
	else/* normal condition  stDivisor >= stDividend*/
	{
		while(ui32flag)
		{
			++ui8DivLeftShit;
			stDataTmp.ui32High<<=1;
			if(stDataTmp.ui32Low&0x80000000)
			{
				stDataTmp.ui32High +=1;
			}
			stDataTmp.ui32Low<<=1;

			if (!ATBM781x64ComPare(stDivisor,stDataTmp)||stDataTmp.ui32High>=0x80000000)
			{
				if (stDataTmp.ui32High<0x80000000)
				{
					stDataTmp.ui32Low>>=1;
					if (stDataTmp.ui32High&1)
					{
						stDataTmp.ui32Low |= 0x80000000;
					}
					stDataTmp.ui32High>>=1;
					ui8DivLeftShit--;
				}


				stDivisor.ui32High -= stDataTmp.ui32High;
				if(stDivisor.ui32Low >= stDataTmp.ui32Low)
				{
					stDivisor.ui32Low -= stDataTmp.ui32Low;
				}
				else
				{
					stDivisor.ui32Low +=0xffffffff - stDataTmp.ui32Low;
					stDivisor.ui32Low += 1;
					stDivisor.ui32High -= 1;
				}
				ui32Rst = (1<<ui8DivLeftShit);;
				break;
			}
		}
		ui32Rst += ATBM781x64Div(stDivisor,stDividend);
	}
	return ui32Rst;
}
ATBM781x64Data ATBM781x64DivReturn64(ATBM781x64Data stDivisor,ATBM781x64Data stDividend)
{
	uint8  ui8DivLeftShit = 0;
	uint32 ui32LowPart;
	unsigned int ui32flag = 1;
	ATBM781x64Data i64Data,i64Data1;
	ATBM781x64Data stDataTmp = stDividend;
	i64Data.ui32High = 0;
	i64Data.ui32Low = 0;
	if (stDivisor.ui32High == 0&&stDivisor.ui32Low == 0)
	{
		return i64Data;
	}
	else if (stDividend.ui32High == 0&&stDividend.ui32Low == 0)
	{
		return i64Data;
	}
	else if (stDividend.ui32High > stDivisor.ui32High)
	{
		return i64Data;
	}
	else if (!ATBM781x64ComPare(stDivisor,stDividend))
	{
		return i64Data;
	}
	else if (stDividend.ui32High == 0 && stDivisor.ui32High == 0)
	{
		i64Data.ui32Low = stDivisor.ui32Low/stDividend.ui32Low;
		return i64Data;
	}
	else/* normal condition  stDivisor >= stDividend*/
	{
		while(ui32flag)
		{
			++ui8DivLeftShit;
			stDataTmp.ui32High<<=1;
			if(stDataTmp.ui32Low&0x80000000)
			{
				stDataTmp.ui32High +=1;
			}
			stDataTmp.ui32Low<<=1;
			if (!ATBM781x64ComPare(stDivisor,stDataTmp)||stDataTmp.ui32High>=0x80000000)
			{
				if (stDataTmp.ui32High<0x80000000)
				{
					stDataTmp.ui32Low>>=1;
					if (stDataTmp.ui32High&1)
					{
						stDataTmp.ui32Low |= 0x80000000;
					}
					stDataTmp.ui32High>>=1;
					ui8DivLeftShit--;
				}


				stDivisor.ui32High -= stDataTmp.ui32High;
				if(stDivisor.ui32Low >= stDataTmp.ui32Low)
				{
					stDivisor.ui32Low -= stDataTmp.ui32Low;
				}
				else
				{
					stDivisor.ui32Low +=0xffffffff - stDataTmp.ui32Low;
					stDivisor.ui32Low += 1;
					stDivisor.ui32High -= 1;
				}
				if (ui8DivLeftShit>=32)
				{
					i64Data.ui32High = (1<<(ui8DivLeftShit-32));
					i64Data.ui32Low = 0; 
				}
				else
				{
					i64Data.ui32Low = (1<<ui8DivLeftShit);
					i64Data.ui32High = 0;
				}
				break;
			}
		}
		i64Data1 = ATBM781x64DivReturn64(stDivisor,stDividend);
		ui32LowPart = i64Data.ui32Low;
		i64Data.ui32Low += i64Data1.ui32Low;
		i64Data.ui32High += i64Data1.ui32High;
		if (ui32LowPart>i64Data.ui32Low)
		{
			i64Data.ui32High+=1;
		}

	}
	return i64Data;
}

/****************************************************************************
Function:   ATBM781xLatchOn
Parameters: none
Return:    Read status:ATBM781x_SUCCESSFUL is normal and other is abnormal.
Description:
latch on the register for reading,make sure the register group during reading period not change		 
******************************************************************************/
ATBM781x_STATUS  ATBM781xLatchOn(void)
{
	return ATBM781xWrite(0x01,0x0d,1);
}
/****************************************************************************
Function:   ATBM781xLatchOff
Parameters: none
Return:    Read status:ATBM781x_SUCCESSFUL is normal and other is abnormal.
Description:
latch off the register for reading		 
******************************************************************************/
ATBM781x_STATUS  ATBM781xLatchOff(void)
{
	return ATBM781xWrite(0x01,0x0d,0);
}
/****************************************************************************
Function:    ATBM781xTunerI2CViaDemodOn
Parameters:  none
Return:      I2C Write Status   0 is Success,Other is Failed
Description:      
Enable demodulator's I2C gate function to pass I2C commands between tuner and host.  
****************************************************************************/
ATBM781x_STATUS  ATBM781xTunerI2CViaDemodOn(void)
{
	/* 1: Enable demodulator's I2C gate to pass I2C commands between tuner and host. */
	return ATBM781xWrite(0x01,0x03, 1); 
}

/****************************************************************************
Function:    ATBM781xTunerI2CViaDemodOff
Parameters:  none
Return:      I2C Write Status   0 is Success,Other is Failed
Description:      
Disable the I2C pass-through. when I2C via demodulator is used,
Tuner is disconnected from the I2C BUS after this function is called. 
****************************************************************************/
ATBM781x_STATUS  ATBM781xTunerI2CViaDemodOff(void)
{
	/*0: Disable demodulator's I2C gate function to pass I2C commands between tuner and host*/
	return ATBM781xWrite(0x01,0x03, 0); 
}

/****************************************************************************
Function:    ATBM781xI2CPassThroughFloating
Parameters:  none
Return:      I2C Write Status   0 is Success,Other is Failed
Description:      
When tuner's I2C connecting directly with host, I2C via demodulator is not used,
the demodulator disable I2C pass-through and output High level.
****************************************************************************/
ATBM781x_STATUS  ATBM781xI2CPassThroughFloating(void)
{
	ATBM781x_STATUS status = ATBM781x_SUCCESSFUL;
	uint8 Reg22Value =0, Reg23Value = 0;
	uint8 Reg30Value = 0;	
	status |= ATBM781xRead(0x00, 0x22, &Reg22Value);
	status |= ATBM781xRead(0x00, 0x23, &Reg23Value);
	status |= ATBM781xRead(0x00, 0x30, &Reg30Value);	
	if((Reg22Value&0xf0)!= 0x40)
	{		
		Reg22Value&= 0x0f;  //Clear bit[7:4]
		Reg22Value|= 0x40; //Set bit[7:4] 
	}
	if((Reg23Value&0x0f)!= 0x04)
	{		
		Reg23Value&= 0xf0;  //Clear bit[3:0]
		Reg23Value|= 0x04; //Set bit[3:0] 
	}
	Reg30Value|= 0x30;
	status |= ATBM781xWrite(0x00, 0x22, Reg22Value);
	status |= ATBM781xWrite(0x00, 0x23, Reg23Value);
	status |= ATBM781xWrite(0x00, 0x30, Reg30Value);
	return status;
}

/****************************************************************************
Function:    ATBM781xHoldDSP
Parameters:  none
Return:      I2C Write Status   0 is Success,Other is Failed  

Description:      
This function should be called before set tuner  frequency.
****************************************************************************/
ATBM781x_STATUS  ATBM781xHoldDSP(void)
{	 
	return ATBM781xWrite(0x00, 0x05, 0x01); 
}

/****************************************************************************
Function:    ATBM781xStartDSP
Parameters:  none
Return:      I2C Write Status   0 is Success,Other is Failed  

Description:      
This function should be called after set tuner  frequency.
Attention: ATBM781xHoldDSP and ATBM781xStartDSP should be used together.
For example:
ATBM781xHoldDSP();
SetTunerFrequency(Frequency_KHz);
ATBM781xStartDSP();        
****************************************************************************/
ATBM781x_STATUS  ATBM781xStartDSP(void)
{
	ATBM781x_STATUS status = ATBM781x_SUCCESSFUL;
	uint8 ui8SxMode = 0;
	status = ATBM781xRead(0x00, 0x2b, &ui8SxMode);
	status |=ATBM781xRestorRegisters(); 
	if(ui8SxMode&0x01) /*S/S2 mode*/
	{
#if ATBM781x_DVB_SS2_SUPPORT
		status |= ATBM781xDvbSxSymbolRateReset( );
#endif
	}
	status |= ATBM781xWrite(0x00, 0x05, 0x00); 
	status |= ATBM781xWrite(0x00, 0x05, 0x01);
	status |= ATBM781xWrite(0x00, 0x05, 0x00); 	
	status |= ATBM781xDynamicCheckProcess();

	return status;
}
/****************************************************************************
Function:    ATBM781xChipID
Parameters:  ui8ChipID:chip id will return by this pointer
Return:      I2C read Status   0 is Success,Other is Failed
Description:      
Get the chip id
****************************************************************************/
ATBM781x_STATUS  ATBM781xChipID(uint8 *ui8ChipID)
{
	return ATBM781xRead(0x00, 0x00, ui8ChipID); 

}
/*
int ATBM781xLog2(int i32Data)//对数换底公式，作为内部函数使用
{
	int i32Tmp = 2;
	return 10*ATBM781xLog10ConvertdB(i32Data)/ATBM781xLog10ConvertdB(i32Tmp);
}*/





ATBM781x_STATUS ATBM781xSetTSMode( ATBM781x_MPEGTSMode stTSMode)
{
	uint8 ui8SerialEnabled =0x0;/*parallel clock out */
    uint8 ui8ChipID;
	uint8 ui8TSOutputEdegeMode = stTSMode.ui8OutputEdge;  
	uint8 ui8TsClockMode = stTSMode.ui8SPIClockConstantOutput;  
	ATBM781x_STATUS status = ATBM781x_SUCCESSFUL;

       status |= ATBM781xRead(0x00, 0x00, &ui8ChipID);
	/*0x00: SPI data align with rising edge of TSBCLK; 0x01: SPI data align with falling edge of TSBCLK*/
	status |=ATBM781xWrite(0x18,0x02, ui8TSOutputEdegeMode); 	
	status |=ATBM781xWrite(0x18,0x03, ui8TsClockMode);  /*either free running or only active when data are valid*/

	if(ui8ChipID == 0xa0)
	{
		status |=ATBM781xWrite(0x00,0x25, 0xff); 	
		status |=ATBM781xWrite(0x00,0x26, 0x7);  /*either free running or only active when data are valid*/
	}
	if((ui8ChipID == 0xa4))
	{
		status |=ATBM781xWrite(0x26,0x24, 0x05); 
		status |=ATBM781xWrite(0x26,0x25, 0xff); 	
		status |=ATBM781xWrite(0x26,0x26, 0x7);  /*either free running or only active when data are valid*/
	}
	if((ui8ChipID == 0xa2))
	{	
		status |=ATBM781xWrite(0x26,0x24, 0x05); 
		status |=ATBM781xWrite(0x26,0x25, 0xff); 	
		status |=ATBM781xWrite(0x26,0x26, 0x7);  /*either free running or only active when data are valid*/
	}
	if(stTSMode.ui8TSTransferType==ATBM781x_TS_SERIAL_MODE)
	{    
		ui8SerialEnabled = 0x01;    /*enable serial TS mode */   

		/*SSI_sync_hold_cycle = 0x00; //0x00:1bit sync; 0x01: 8bit sync*/
		/*0: serial sync  holds 1 cycle; 1: serial sync holds 8 cycles*/
		/*ATBM781xWrite(0x17,0xb0, SSI_sync_hold_cycle);  */

		/*0:disable serial mode; 1: enable serial mode */  
		status |=ATBM781xWrite(0x18,0x01, ui8SerialEnabled);      
		if(stTSMode.ui8TSSSIOutputSelection == ATBM781x_TS_SSI_OUTPUT_ON_DATA_BIT0)
		{
			status |=ATBM781xWrite(0x18, 0x04, 0x00); 
		}else
		{
			status |=ATBM781xWrite(0x18, 0x04, 0x01); 
		}
		if(ui8ChipID == 0xa0)
		{
			status |=ATBM781xWrite(0x00, 0x27, 0x00);
		}
		if((ui8ChipID == 0xa2)||(ui8ChipID == 0xa4))
		{

			status |=ATBM781xWrite(0x26, 0x27, 0x00); /*TS delay clock*/
			status |=ATBM781xWrite(0x26, 0x29, 0x03); /*Disable TS delay*/
		}		
	}
	else	
	{	  
		ui8SerialEnabled =0x0; /*parallel clock output */
		/*0: disable serial mode; 1:enable serial mode */	
		status |=ATBM781xWrite(0x18,0x01, ui8SerialEnabled);      		
		if(stTSMode.ui8TSSPIMSBSelection == ATBM781x_TS_SPI_MSB_ON_DATA_BIT7)
		{
			status |=ATBM781xWrite(0x18, 0x04, 0x00); 
		}else
		{
			status |=ATBM781xWrite(0x18, 0x04, 0x01); 
		}
		if(ui8ChipID == 0xa0)
		{
			status |=ATBM781xWrite(0x00, 0x27, 0x03);
		}
		if((ui8ChipID == 0xa2)||(ui8ChipID == 0xa4))
		{
			status |=ATBM781xWrite(0x26, 0x27, 0x00);
		}
		status |=ATBM781xWrite(0x26, 0x29, 0x03);
	}
	return status;
}

/****************************************************************************
Function:    ATBM781xTSLockFlag()
Parameters: 
	LockStatus: pointer to lock status
Return:      status 
Description: Get TS lock status. 		
****************************************************************************/
ATBM781x_STATUS ATBM781xTSLockFlag(uint8 *LockStatus)
{
	/*
	Lock Flag indicate current lock status
	1, Must be locked, TS output or FEC locked in stable state.
	2, For occasional TS error, should give lock flag.
	*/
	ATBM781x_STATUS status = ATBM781x_SUCCESSFUL;
	uint8 u8TSLockFlag = 0;
	uint8 u8Iter = 0, u8lockedCount = 0;
	*LockStatus = 0;
	for(u8Iter = 0; u8Iter < 5; u8Iter++)
	{
		status |= ATBM781xRead(0x18, 0x2A, &u8TSLockFlag);
		if((u8TSLockFlag&0x01)&&(status == ATBM781x_SUCCESSFUL))
		{
			u8lockedCount = u8lockedCount + 1;
		}
	}
	/*TS locked*/
	if(u8lockedCount >= 2)
	{
		*LockStatus = 1;		
	}
	return status;
}

/****************************************************************************
Function:    ATBM781xFECLockFlag()
Parameters: 
	LockStatus: pointer to FEC lock status
Return:      status 
Description: 
          Get FEC lock status. can be used for DVB-T/T2, DVB-C, DVBS/S2 FEC 
		  lock flag check.		
****************************************************************************/
ATBM781x_STATUS ATBM781xFECLockFlag(uint8 *LockStatus)
{
	ATBM781x_STATUS status = ATBM781x_SUCCESSFUL;
	uint8 u8DvbcMode = 0, u8DVBSxMode = 0;
#if ATBM781x_DVB_SS2_SUPPORT
	uint8 u8SS2Mode = 0;
	uint32 u32DvbsStatus = 0;
#endif
	uint8 u8BlockFEC = 0;
	uint32 u32DvbcStatus = 0;
	uint8  u8DVBTxMode = 0, u8TPSDone = 0, u8DvbtFecLock = 0;

	status |= ATBM781xRead(0x00, 0x13, &u8DvbcMode);
	status |= ATBM781xRead(0x00, 0x2b, &u8DVBSxMode);

	*LockStatus = 0;
	if(u8DvbcMode&0x01)
	{
		status |= ATBM781xDVBCRead(0x08, &u32DvbcStatus);       //read DVB-C internal register        
		if(u32DvbcStatus&0x20) //bit 5
		{                           
			*LockStatus = 1;	
			return status;           //DVB-C FEC locked
		}    
	}
	else if(u8DVBSxMode&0x01)
	{
#if ATBM781x_DVB_SS2_SUPPORT
		status |= ATBM781xDVBSxModeDetected(&u8SS2Mode);
		if(u8SS2Mode == ATBM781x_DVBS_MODE)
		{		 
			status |= ATBM781xDVBSRead(0x08, &u32DvbsStatus);       //read DVB-S internal register        
			if(u32DvbsStatus&0x20) //bit 5
			{                           
				*LockStatus = 1;	
				return status;           //DVB-S FEC locked
			}    
		}
		if(u8SS2Mode == ATBM781x_DVBS2_MODE)
		{	
			status |= ATBM781xRead(0x17, 0xEC, &u8BlockFEC);
			if(u8BlockFEC&0x01)
			{
				*LockStatus = 1;	
				return status;           //DVB-S2 FEC locked
			}
		}
#endif		
	}
	else
	{
		status |=  ATBM781xDVBTxModeDetected(&u8DVBTxMode);
		if((u8DVBTxMode ==ATBM781x_DVBT_MODE)&&(status== ATBM781x_SUCCESSFUL))
		{
			status |=ATBM781xRead(0x28, 0x2c, &u8TPSDone);
			status |=ATBM781xRead(0x95, 0x37, &u8DvbtFecLock);
			if((u8DvbtFecLock&0x01)&&(u8TPSDone&0x01)) /*TPS done and FEC lock, DVB-T locked*/
			{
				*LockStatus = 1;
				return status;
			}
		}
		if((u8DVBTxMode ==ATBM781x_DVBT2_MODE)&&(status== ATBM781x_SUCCESSFUL))
		{
			status |= ATBM781xRead(0x17, 0xEC, &u8BlockFEC);
			if(u8BlockFEC&0x01)
			{
				*LockStatus = 1;	
				return status;           //DVB-T2 FEC locked
			}
		}
	}
	return status;
}

/*#define  ATBM781xSUCCESS(status)   (status == ATBM781x_SUCCESSFUL)*/


void ATBM781xPrintStr(const char *StrType, const char *StrValue)
{
	ATBM781xDebugPrint ("%-20s = %s\r\n", StrType, StrValue);
}


/*
Functions: 
ATBM781xSetGPIOHighLowGPOStatus
ATBM781xSetGPIOHighLowGPO2
ATBM781xSetGPIOHighLowGPO4
ATBM781xSetGPIOHighLowGPO5
          Used to Set GPO pin High level output or low level output
gpio_pin:
The following pins may be used as GPO pin, refer to Hardware design.
ATBM781x_GPIO_PIN_TYPE GPOPinStatus;     //0x0023 bit[7:4]
ATBM781x_GPIO_PIN_TYPE GPO2PinRFAgcDvbT; //0x0021 bit[7:4] //Not used set 0
ATBM781x_GPIO_PIN_TYPE GPO4PinStatus;    //0x0037 bit[7:4]:  RegGPO7
ATBM781x_GPIO_PIN_TYPE GPO5PinStatus;    //0x0037 bit[3:0]   RegGPO6

attribute:
 1: High level
 0: Low level
*/

ATBM781x_STATUS ATBM781xSetGPIOHighLowGPOStatus(uint8 attribute)
{
	ATBM781x_STATUS status = ATBM781x_SUCCESSFUL;
	uint8 Reg23Value = 0;
	uint8 Reg30Value = 0;
    status |= ATBM781xRead(0x00, 0x30, &Reg30Value);
	if(attribute)
	{
		status |= ATBM781xRead(0x00, 0x23, &Reg23Value);
		if((Reg23Value&0xf0)!=0x40)
		{	
			Reg23Value&=0x0f;  /*Clear bit[7:4]*/
			Reg23Value|=0x40; /*Set bit[7:4] */
		}
		Reg30Value|=0x01;
		status |= ATBM781xWrite(0x00, 0x23, Reg23Value); 
	}else
	{
		status |= ATBM781xRead(0x00, 0x23, &Reg23Value);
		if((Reg23Value&0xf0)!=0x40)
		{	
			Reg23Value&=0x0f;  /*Clear bit[7:4]*/
			Reg23Value|=0x40; /*Set bit[7:4] */
		}
		Reg30Value&=0xfe;
		status |= ATBM781xWrite(0x00, 0x23, Reg23Value);
	}
	status |= ATBM781xWrite(0x00, 0x30, Reg30Value);
	return status;
}
ATBM781x_STATUS ATBM781xSetGPIOHighLowGPO2(uint8 attribute)
{
	ATBM781x_STATUS status = ATBM781x_SUCCESSFUL;
	uint8 Reg21Value = 0;
	uint8 Reg30Value = 0;
	status |=  ATBM781xRead(0x00, 0x30, &Reg30Value);
	if(attribute)
	{
		status |= ATBM781xRead(0x00, 0x21, &Reg21Value);
		if((Reg21Value&0xf0)!=0x40)
		{
			Reg21Value&=0x0f;  /*Clear bit[7:4]*/
			Reg21Value|=0x40; /*Set bit[7:4] */
		}
		Reg30Value|=0x04;
		status |=  ATBM781xWrite(0x00, 0x21, Reg21Value); 
	}else
	{

		status |= ATBM781xRead(0x00, 0x21, &Reg21Value);
		if((Reg21Value&0xf0)!=0x40)
		{
			Reg21Value&=0x0f;  /*Clear bit[7:4]*/
			Reg21Value|=0x40; /*Set bit[7:4] */
		}	
		Reg30Value&= 0xfb;
		status |= ATBM781xWrite(0x00, 0x21, Reg21Value);
	}
	status |= ATBM781xWrite(0x00, 0x30, Reg30Value);
	return status;
}
ATBM781x_STATUS ATBM781xSetGPIOHighLowGPO4(uint8 attribute)
{
	ATBM781x_STATUS status = ATBM781x_SUCCESSFUL;
	uint8 Reg37Value =0;
	uint8 Reg30Value = 0;
	 status |= ATBM781xRead(0x00, 0x30, &Reg30Value);
	if(attribute)
	{
		status |= ATBM781xRead(0x00, 0x37, &Reg37Value);
		if((Reg37Value&0xf0)!= 0x40)
		{		
			Reg37Value&= 0x0f;  /*Clear bit[7:4]*/
			Reg37Value|= 0x40; /*Set bit[7:4] */
		}
		Reg30Value|= 0x80;
		status |= ATBM781xWrite(0x00, 0x37, Reg37Value); 
	}else
	{
		status |= ATBM781xRead(0x00, 0x37, &Reg37Value);
		if((Reg37Value&0xf0)!= 0x40)
		{		
			Reg37Value&= 0x0f;  /*Clear bit[7:4]*/
			Reg37Value|= 0x40; /*Set bit[7:4] */
		}
		Reg30Value&= 0x7f;
		status |= ATBM781xWrite(0x00, 0x37, Reg37Value);
	}
	status |= ATBM781xWrite(0x00, 0x30, Reg30Value);
	return status;
}

ATBM781x_STATUS ATBM781xSetGPIOHighLowGPO5(uint8 attribute)
{
	ATBM781x_STATUS status = ATBM781x_SUCCESSFUL;
	uint8 Reg37Value =0;
	uint8 Reg30Value = 0;
	status |= ATBM781xRead(0x00, 0x30, &Reg30Value);
	if(attribute)
	{
		status |= ATBM781xRead(0x00, 0x37, &Reg37Value);
		if((Reg37Value&0x0f)!= 0x04)
		{		
			Reg37Value&= 0xf0;  /*Clear bit[3:0]*/
			Reg37Value|= 0x04; /*Set bit[3:0] */
		}
		Reg30Value|= 0x40;
		status |= ATBM781xWrite(0x00, 0x37, Reg37Value);
	}else
	{
		status |= ATBM781xRead(0x00, 0x37, &Reg37Value);
		if((Reg37Value&0x0f)!= 0x04)
		{		
			Reg37Value&= 0xf0;  /*Clear bit[3:0]*/
			Reg37Value|= 0x04; /*Set bit[3:0] */
		}		
		Reg30Value&= 0xbf;
		status |=  ATBM781xWrite(0x00, 0x37, Reg37Value);
	}
	status |= ATBM781xWrite(0x00, 0x30, Reg30Value);
	return status;
}
/*
typedef struct ATBM781x_PIN_CONFIG_STRUCT
{
ATBM781x_GPIO_PIN_TYPE GPOPinStatus;    //0x0023 bit[7:4]  
ATBM781x_GPIO_PIN_TYPE GPO1PinAgcDvbS;   //0x0021 bit[3:0] = 3
ATBM781x_GPIO_PIN_TYPE GPO2PinRFAgcDvbT; //0x0021 bit[7:4] //Not used set 0
ATBM781x_GPIO_PIN_TYPE GPO3PinAgcDvbT;   //0x0022 bit[3:0] = 1
ATBM781x_GPIO_PIN_TYPE GPO4PinStatus;   //0x0037 bit[7:4]
ATBM781x_GPIO_PIN_TYPE GPO5PinStatus;   //0x0037 bit[3:0]
}ATBM781x_PIN_CONFIG_T;
*/
/********************************************************************************
function    : ATBM781xSetGPOProperty
parameter   : 
PinName:       Pin name as ATBM781x_PIN_CONFIG_STRUCT 
uiRegAddress:  Register addr for Pin Name
HighBits:      1: Set high 4bits, 0: set low 4 bits.
state:         0: set High-Z state, 1: set as GPO state.
return      :Operate  Status:ATBM781x_SUCCESSFUL is Normal and other is abnormal.
Description:
Internal function setting GPO pins
*********************************************************************************/
ATBM781x_STATUS	ATBM781xSetGPOProperty(ATBM781x_GPIO_PIN_TYPE PinName, uint16 uiRegAddress, uint8 HighBits, uint8 state)
{
	ATBM781x_STATUS status = ATBM781x_SUCCESSFUL;
	uint8 u8RegValue = 0;
	uint8 u8BaseAddress, u8OffsetAddress;
	u8BaseAddress = (unsigned char)(uiRegAddress/256);
	u8OffsetAddress = (unsigned char)(uiRegAddress&0xff);
	status |= ATBM781xRead(u8BaseAddress, u8OffsetAddress, &u8RegValue);

	if(HighBits == 0) /*Low 4 Bits*/
	{
		switch(PinName)
		{
		case ATBM781x_PIN_IF_AGC_T:
			if((gstATBM781xDvbConfig.ui8ConfigDVBType == ATBM781x_DVBT2_MODE)||(gstATBM781xDvbConfig.ui8ConfigDVBType == ATBM781x_DVBT_MODE)||(gstATBM781xDvbConfig.ui8ConfigDVBType  == ATBM781x_DVBC_MODE))
			{
				if(state)
				{
					u8RegValue|= 0x01;
				}else
				{
					u8RegValue&= 0xf0;
				}
			}
			break;	
		case ATBM781x_PIN_TS_ERROR:
			if(state)
			{
				u8RegValue|= 0x07;
			}else
			{
				u8RegValue&= 0xf0;
			}
			break;
		case ATBM781x_PIN_TS_LOCK:
			if(state)
			{
				u8RegValue|= 0x08;
			}else
			{
				u8RegValue&= 0xf0;
			}
			break;
		case ATBM781x_PIN_FEC_ERROR:
			if(state)
			{
				u8RegValue|= 0x09;
			}else
			{
				u8RegValue&= 0xf0;
			}
			break;
		case ATBM781x_PIN_FEC_LOCK:
			if(state)
			{
				u8RegValue|= 0x0a;
			}else
			{
				u8RegValue&= 0xf0;
			}
			break;
		case ATBM781x_PIN_AGC_FREEZE:
			if((gstATBM781xDvbConfig.ui8ConfigDVBType == ATBM781x_DVBT2_MODE)||(gstATBM781xDvbConfig.ui8ConfigDVBType == ATBM781x_DVBT_MODE))
			{
				if(state)
				{
					u8RegValue|= 0x05;		
				}else
				{
					u8RegValue&= 0xf0;
				}
			}
			break;
		case ATBM781x_PIN_IF_AGC_S:
			if((gstATBM781xDvbConfig.ui8ConfigDVBType == ATBM781x_DVBS_MODE)||(gstATBM781xDvbConfig.ui8ConfigDVBType == ATBM781x_DVBS2_MODE))
			{
				if(state)
				{
					u8RegValue |= 0x03;  /*DVB-S/S2 IF AGC*/
				}else
				{
					u8RegValue&= 0xf0;
				}
			}
			break;
		default:
			break;
		}	
	}
	if(HighBits == 1) /*High 4bits*/
	{
		switch(PinName)
		{
		case ATBM781x_PIN_IF_AGC_T:
			if((gstATBM781xDvbConfig.ui8ConfigDVBType == ATBM781x_DVBT2_MODE)||(gstATBM781xDvbConfig.ui8ConfigDVBType == ATBM781x_DVBT_MODE)||(gstATBM781xDvbConfig.ui8ConfigDVBType  == ATBM781x_DVBC_MODE))
			{
				if(state)
				{
					u8RegValue|= 0x10;
				}else
				{
					u8RegValue&= 0x0f;
				}
			}
			break;	
		case ATBM781x_PIN_TS_ERROR:
			if(state)
			{
				u8RegValue|= 0x70;
			}else
			{
				u8RegValue&= 0x0f;
			}
			break;
		case ATBM781x_PIN_TS_LOCK:
			if(state)
			{
				u8RegValue|= 0x80;
			}else
			{
				u8RegValue&= 0x0f;
			}
			break;
		case ATBM781x_PIN_FEC_ERROR:
			if(state)
			{
				u8RegValue|= 0x90;
			}else
			{
				u8RegValue&= 0x0f;
			}
			break;
		case ATBM781x_PIN_FEC_LOCK:
			if(state)
			{
				u8RegValue|= 0xa0;
			}else
			{
				u8RegValue&= 0x0f;
			}
			break;
		case ATBM781x_PIN_AGC_FREEZE:
			if((gstATBM781xDvbConfig.ui8ConfigDVBType == ATBM781x_DVBT2_MODE)||(gstATBM781xDvbConfig.ui8ConfigDVBType == ATBM781x_DVBT_MODE))
			{
				if(state)
				{
					u8RegValue|= 0x50;		
				}else
				{
					u8RegValue&= 0x0f;
				}
			}
			break;
		case ATBM781x_PIN_IF_AGC_S:
			if((gstATBM781xDvbConfig.ui8ConfigDVBType == ATBM781x_DVBS_MODE)||(gstATBM781xDvbConfig.ui8ConfigDVBType == ATBM781x_DVBS2_MODE))
			{
				if(state)
				{
					u8RegValue |= 0x30;  /*DVB-S/S2 IF AGC*/
				}else
				{
					u8RegValue&= 0x0f;
				}
			}
			break;
		default:
			break;
		}	
	}
	status |= ATBM781xWrite(u8BaseAddress, u8OffsetAddress, u8RegValue);
	return status;
}

ATBM781x_STATUS ATBM781xSetGpioPins(void)
{
	ATBM781x_STATUS status = ATBM781x_SUCCESSFUL;
	uint16 u16Reg21 = 0x0021, u16Reg22 = 0x0022, u16Reg23 = 0x0023, u16Reg37 =0x0037;

	status |= ATBM781xSetGPOProperty(gstATBM781xDvbConfig.GpioPinConfigs.GPOPinStatus,     u16Reg23,  1, 1);	
	status |=ATBM781xSetGPOProperty(gstATBM781xDvbConfig.GpioPinConfigs.GPO2PinRFAgcDvbT, u16Reg21,  1, 1); 
	status |=ATBM781xSetGPOProperty(gstATBM781xDvbConfig.GpioPinConfigs.GPO1PinAgcDvbS,   u16Reg21,  0, 1); 
	status |=ATBM781xSetGPOProperty(gstATBM781xDvbConfig.GpioPinConfigs.GPO3PinAgcDvbT,   u16Reg22,  0, 1); 
	if(gstATBM781xDvbConfig.ui8DualDemod == 0)
	{
		status |=ATBM781xSetGPOProperty(gstATBM781xDvbConfig.GpioPinConfigs.GPO4PinStatus, u16Reg37,  1, 1);
		status |=ATBM781xSetGPOProperty(gstATBM781xDvbConfig.GpioPinConfigs.GPO5PinStatus, u16Reg37,  0, 1); 
	}
	if(gstATBM781xDvbConfig.ui8DualDemod == 1) /*ATBM781x7815*/
	{
		status |=ATBM781xSetGPOProperty(gstATBM781xDvbConfig.GpioPinConfigs.GPO4PinStatus, u16Reg22,  1, 1); 
		status |=ATBM781xSetGPOProperty(gstATBM781xDvbConfig.GpioPinConfigs.GPO5PinStatus, u16Reg23,  0, 1);
	}
	return status;
}
ATBM781x_STATUS ATBM781xSetGpioPinsHiZ(void)
{
	ATBM781x_STATUS status = ATBM781x_SUCCESSFUL;
	uint16 u16Reg21 = 0x0021, u16Reg22 = 0x0022, u16Reg23 = 0x0023, u16Reg37 =0x0037;

	status |=ATBM781xSetGPOProperty(gstATBM781xDvbConfig.GpioPinConfigs.GPOPinStatus,     u16Reg23,  1, 0);	
	status |=ATBM781xSetGPOProperty(gstATBM781xDvbConfig.GpioPinConfigs.GPO2PinRFAgcDvbT, u16Reg21,  1, 0); 
	status |=ATBM781xSetGPOProperty(gstATBM781xDvbConfig.GpioPinConfigs.GPO1PinAgcDvbS,   u16Reg21,  0, 0); 
	status |=ATBM781xSetGPOProperty(gstATBM781xDvbConfig.GpioPinConfigs.GPO3PinAgcDvbT,   u16Reg22,  0, 0); 
	if(gstATBM781xDvbConfig.ui8DualDemod == 0)
	{
		status |=ATBM781xSetGPOProperty(gstATBM781xDvbConfig.GpioPinConfigs.GPO4PinStatus, u16Reg37,  1, 0);
		status |=ATBM781xSetGPOProperty(gstATBM781xDvbConfig.GpioPinConfigs.GPO5PinStatus, u16Reg37,  0, 0); 
	}
	if(gstATBM781xDvbConfig.ui8DualDemod == 1) /*ATBM781x7815*/
	{
		status |=ATBM781xSetGPOProperty(gstATBM781xDvbConfig.GpioPinConfigs.GPO4PinStatus, u16Reg22,  1, 0); 
		status |=ATBM781xSetGPOProperty(gstATBM781xDvbConfig.GpioPinConfigs.GPO5PinStatus, u16Reg23,  0, 0);
	}
	return status;
}
/****************************************************************************
Function:    ATBM781xPowerOnInit()
Parameters:  none
Return:      status 
Description: This function is used to initialize default configuration. 		
****************************************************************************/
ATBM781x_STATUS ATBM781xPowerOnInit(void)
{
	ATBM781x_STATUS status = ATBM781x_SUCCESSFUL;

	//Demodulator TS interface  setting	
	gstATBM781xDvbConfig.stMPEGTSMode.ui8TSTransferType           = ATBM781x_TS_PARALLEL_MODE; //TS_PARALLEL_MODE并行	  
	gstATBM781xDvbConfig.stMPEGTSMode.ui8OutputEdge               = ATBM781x_TS_OUTPUT_RISING_EDGE;  //TS_OUTPUT_FALLING_EDGE 下降沿  
	gstATBM781xDvbConfig.stMPEGTSMode.ui8SPIClockConstantOutput   = ATBM781x_TS_CLOCK_VALID_OUTPUT;
	gstATBM781xDvbConfig.stMPEGTSMode.ui8TSSPIMSBSelection        = ATBM781x_TS_SPI_MSB_ON_DATA_BIT0; //TS_SPI_MSB_ON_DATA_BIT7
	gstATBM781xDvbConfig.stMPEGTSMode.ui8TSSSIOutputSelection     = ATBM781x_TS_SSI_OUTPUT_ON_DATA_BIT7; //TS_SSI_OUTPUT_ON_DATA_BIT0

    /*Set demodulator PIN for AGC and status indicator*/
    gstATBM781xDvbConfig.GpioPinConfigs.GPO3PinAgcDvbT   = ATBM781x_PIN_IF_AGC_T; /*IF AGC used for DVB/T/T2 DVB-C*/
    gstATBM781xDvbConfig.GpioPinConfigs.GPO1PinAgcDvbS   = ATBM781x_PIN_IF_AGC_S; /*IF AGC used for DVB-S/S2*/
    gstATBM781xDvbConfig.GpioPinConfigs.GPO2PinRFAgcDvbT = ATBM781x_PIN_RESERVED; 
    gstATBM781xDvbConfig.GpioPinConfigs.GPOPinStatus    = ATBM781x_PIN_RESERVED;    /*Can be used as ATBM781x_PIN_FEC_LOCK, if not used set as reserved*/
	gstATBM781xDvbConfig.GpioPinConfigs.GPO4PinStatus   = ATBM781x_PIN_RESERVED;
	gstATBM781xDvbConfig.GpioPinConfigs.GPO5PinStatus   = ATBM781x_PIN_RESERVED;
	/*gstATBM781xDvbConfig.GpioPinConfigs.GPO5PinStatus   = ATBM781x_PIN_AGC_FREEZE; //Si2178 may need AGC freeze*/
/* 	//Demodulator DVB-T/T2 setting*/
	gstATBM781xDvbConfig.DvbTxTunerInput.ui8IQSwapMode = ATBM781x_INPUT_IQ_SWAP; 
	gstATBM781xDvbConfig.DvbTxTunerInput.u32IFFrequencyKHz = 5000; /*5MHz, Here the unit is KHz,  Should be equal to the input of Tuner IF frequency.*/
	gstATBM781xDvbConfig.DvbTxTunerInput.ui8BandwidthType = ATBM781x_DVBT_BANDWIDTH_8MHZ;

	/*Demodulator DVB-C setting*/
	gstATBM781xDvbConfig.DvbCTunerInput.ui8IQSwapMode = ATBM781x_INPUT_IQ_SWAP;
	gstATBM781xDvbConfig.DvbCTunerInput.u32IFFrequencyKHz = 5000; /*5MHz,  Here the unit is KHz,  Should be equal to the input of Tuner IF frequency.*/
	gstATBM781xDvbConfig.DvbCTunerInput.u32SymbolRateK = 6875;  /*6.875MsymbolPerSeconds,This value is not required to be accurate, Demodulator Auto detection.*/

	/*Demodulator DVB-S/S2 setting*/
	/*
	Please refer to the following steps to make sure IQ configuration is right.
	1,  Set gstATBM781xDvbConfig.DvbSxTunerInput.ui8IQAutoDetection = ATBM781x_IQ_AUTO_DISABLE;             //Shutdown IQ auto detection
	2,  Set gstATBM781xDvbConfig.DvbSxTunerInput.ui8IQSwapMode = ATBM781x_INPUT_IQ_SWAP or ATBM781x_INPUT_IQ_NORMAL, //make sure demodulator works.
	3,  Set gstATBM781xDvbConfig.DvbSxTunerInput.ui8IQAutoDetection = ATBM781x_IQ_AUTO_ENABLE;              //open IQ auto detection
	*/
	gstATBM781xDvbConfig.DvbSxTunerInput.ui8IQSwapMode = ATBM781x_INPUT_IQ_SWAP; 
   	gstATBM781xDvbConfig.DvbSxTunerInput.ui8IQAutoDetection = ATBM781x_IQ_AUTO_ENABLE; 
	gstATBM781xDvbConfig.DvbSxTunerInput.ui32SymbolRateK = 27000; /*Actually not used, demodulator auto detection.*/

	/*Demodulator Clock Setting           */
	gstATBM781xDvbConfig.ui32DemodClkKHz = 24000; /*KHz, 24MHz, or 30400 for 30.4MHz*/

	gstATBM781xDvbConfig.ui8DualDemod = 0;
/* 	ATBM781xSetSysClkClock();*/
	ATBM781xSetSysClkClockDVBTx();
	status |= ATBM781xWrite(0x00, 0x04, 0x00); /*set register to default value,  ATBM782xPowerOnInit should be called once after power up */
    gATBM781xPowerOnInited = 1;
	return status;
}

/****************************************************************************
Function:    ATBM781xStandBy()
Parameters:  none
Return:      status 
Description: Set chip in standby working mode. I2C still works.
****************************************************************************/
ATBM781x_STATUS ATBM781xStandBy(void)
{
	ATBM781x_STATUS ui8Status = ATBM781x_SUCCESSFUL;

    uint8 u8SAdc, u8TAdc, u8RssiAdc, u8PLL;

	ui8Status = ATBM781xSetGpioPinsHiZ();          /*Pull AGC pin Hi-Z  state.*/

	ui8Status |= ATBM781xWrite(0x00, 0x05, 0x01); /*DSP Reset down, TS FEC will not be locked.*/
	ui8Status |= ATBM781xWrite(0x01, 0x0C, 0x00); /*I2C on crystal*/

	ui8Status |= ATBM781xWrite(0x26, 0x25, 0x00); /*TS data OE, Disable TS*/
	ui8Status |= ATBM781xWrite(0x26, 0x26, 0x00); /*TS sync OE, Disable TS*/

     ui8Status |= ATBM781xRead(0x06, 0x1c,&u8SAdc);
	 ui8Status |= ATBM781xRead(0x06, 0x00,&u8TAdc);
	 ui8Status |= ATBM781xRead(0x06, 0x05,&u8RssiAdc);
	 ui8Status |= ATBM781xRead(0x06, 0x08,&u8PLL);

     u8SAdc|= 0x01;     /*Set bit0 = 1*/
     u8TAdc|= 0x02;     /*Set bit1 = 1*/
     u8RssiAdc |= 0x02; /*Set bit1 = 1*/
     u8PLL|= 0x01;      /*Set bit0 = 1*/

	 ui8Status |= ATBM781xWrite(0x06, 0x1c,u8SAdc);
	 ui8Status |= ATBM781xWrite(0x06, 0x00,u8TAdc);
	 ui8Status |= ATBM781xWrite(0x06, 0x05,u8RssiAdc);
	 ui8Status |= ATBM781xWrite(0x06, 0x08,u8PLL);

	return ui8Status;
}

/********************************************************************************
function    : ATBM781xStandByWakeUp
parameter   :none 
return      :Operate  Status:ATBM781x_SUCCESSFUL is Normal and other is abnormal.
Description:
*********************************************************************************/
ATBM781x_STATUS  ATBM781xStandByWakeUp(void)
{
	ATBM781x_STATUS ui8Status = ATBM781x_SUCCESSFUL;
	uint8  ui8Reg0630 =0, ui8CfgDone = 0;

	uint8 u8SAdc = 0, u8TAdc = 0, u8PLL = 0;
    /*uint8 u8RssiAdc;*/
	ui8Status |= ATBM781xRead(0x06, 0x1c,&u8SAdc);
	ui8Status |= ATBM781xRead(0x06, 0x00,&u8TAdc);
	/*ui8Status |= ATBM781xRead(0x06, 0x05,&u8RssiAdc);*/
	ui8Status |= ATBM781xRead(0x06, 0x08,&u8PLL);

	if((gstATBM781xDvbConfig.ui8ConfigDVBType == ATBM781x_DVBS_MODE)||(gstATBM781xDvbConfig.ui8ConfigDVBType == ATBM781x_DVBS2_MODE))
	{
		u8SAdc &= 0xFE;     /*Set bit0 = 0 Open S S2 ADC		*/
	}
	u8TAdc &= 0xFD;     /*Set bit1 = 0 Open T/T2 C S/S2 ADC	Switch*/
	/*u8RssiAdc &= 0xFD;  //Set bit1 = 0, Currently RSSI ADC is not used.*/
	u8PLL &= 0xFE;;     /*Set bit0 = 0*/
	ui8Status |= ATBM781xWrite(0x06, 0x08,u8PLL);
	ui8Status |= ATBM781xWrite(0x06, 0x1c,u8SAdc);
	ui8Status |= ATBM781xWrite(0x06, 0x00,u8TAdc);

	ui8Status |= ATBM781xWrite(0x26, 0x25, 0xFF); /*TS data OE, Enable TS output*/
	ui8Status |= ATBM781xWrite(0x26, 0x26, 0x07); /*TS sync OE, Enable TS output*/

	ui8Status |= ATBM781xSetGpioPins(); /*Make sure AGC pin works from standby state*/
	ui8Status |= ATBM781xWrite(0x00, 0x05, 0x00); /*DSP Reset up.*/
	ui8Status |= ATBM781xSleep(1); /*Delay 1 millisecond.*/
	/*ui8Status |= ATBM781xWrite(0x06, 0x05,u8RssiAdc);*/
	ui8Status |= ATBM781xRead(0x06, 0x30, &ui8Reg0630);
	ui8Status |= ATBM781xRead(0x00, 0x04, &ui8CfgDone);
	if(((ui8Reg0630&0x01)!= 0x01)&&((ui8CfgDone&0x01)== 0x01)) //Check PLL is locked.
	{		
		ui8Status |= ATBM781xSleep(10); /*Delay 1 millisecond.*/	
		ui8Status |= ATBM781xRead(0x06, 0x30, &ui8Reg0630);
	}
	if(((ui8Reg0630&0x01)== 0x01)&&((ui8CfgDone&0x01)== 0x01)) //Check PLL is locked.
	{		
		ui8Status |= ATBM781xWrite(0x01, 0x0C, 0x01); /*I2C on PLL*/
	}	
	return ui8Status;
}

/****************************************************************************
Function:    ATBM781xSusPend()
Parameters:  none
Return:      status 
Description: Set chip in suspend working mode.I2C will not work, Hardware reset or Power on is required to active the demodulator.
****************************************************************************/
ATBM781x_STATUS ATBM781xSusPend(void)
{
	ATBM781x_STATUS status = ATBM781x_SUCCESSFUL;

	uint8 u8CrystalPd = 0;
	status = ATBM781xStandBy();

	/*After clock is shut,I2C is not work, Chip can only be started up by using reset PIN or Power Reset.*/
	status |= ATBM781xRead(0x06, 0x0E, &u8CrystalPd); 
	u8CrystalPd |= 0x01;
	status |=ATBM781xWrite(0x06, 0x0E, u8CrystalPd); 

	return status;
}


ATBM781x_STATUS ATBM781xSetSysClkClockDVBTx(void)
{

	ATBM781x_STATUS ui8Status = ATBM781x_SUCCESSFUL;
    uint8 ui8ChipID = 0, ui8AnaReg = 0, u8RssiAdc =0;
	/*I2C using Crystal clock*/
	ui8Status   =  ATBM781xWrite(0x01, 0x0c, 0x00);
	ui8Status |=ATBM781xSleep(1); /*Delay 1 millisecond.*/

    ui8Status |= ATBM781xRead(0x00, 0x00, &ui8ChipID);
	if((ui8ChipID == 0xa2)||(ui8ChipID == 0xa4))
	{

		ui8Status |=ATBM781xWrite(0x06, 0x07, 0x01); //3->1, 8.26
		ui8Status |=ATBM781xWrite(0x06, 0x1b, 0x08); // 8.26
		ui8Status |=ATBM781xWrite(0x06, 0x18, 0x03); //1->3, 8.26
		ui8Status |=ATBM781xWrite(0x06, 0x06, 0x06); //6  //960M
		ui8Status |=ATBM781xWrite(0x06, 0x13, 0x02);
		ui8Status |=ATBM781xWrite(0x06, 0x0B, 0x02); 
		ui8Status |=ATBM781xWrite(0x06, 0x0A, 0x0b); /*TS*/
		ui8Status |=ATBM781xWrite(0x06, 0x1C, 0x41);
		ui8Status |=ATBM781xWrite(0x06, 0x09, 0x01); //PLL related Temperature range, 7.25
	    ui8Status |=ATBM781xWrite(0x06, 0x21, 0x73);
	}
	if(ui8ChipID == 0xa2)
	{
		ui8Status |= ATBM781xRead(0x06, 0x1A, &ui8AnaReg);
		ui8AnaReg = ui8AnaReg + 58; /*0x40 - 6*/
		ui8Status |= ATBM781xWrite(0x06, 0x19, ui8AnaReg);
	}
	if((ui8ChipID == 0xa2)||(ui8ChipID == 0xa4))
	{
		ui8Status |=ATBM781xSleep(1); /*Delay 1 millisecond.	*/
		ui8Status  |= ATBM781xWrite(0x03, 0x06, 0x00);    /* ADC 1x mode*/
		ui8Status  |= ATBM781xWrite(0x26, 0x29, 0x00); /*bypass TS delay*/
	    ui8Status |= ATBM781xRead(0x06, 0x05,&u8RssiAdc);
		u8RssiAdc |= 0x02; /*Set bit1 = 1*/
		ui8Status |= ATBM781xWrite(0x06, 0x05,u8RssiAdc);
	}
#if ATBM781x_DVB_SS2_SUPPORT
   gATBM781xDiseqcClockKHz = 60000;
#endif
	return ui8Status;
}

ATBM781x_STATUS ATBM781xSetSysClkClockDVBSx(void)
{

	ATBM781x_STATUS ui8Status = ATBM781x_SUCCESSFUL;
	uint8 ui8ChipID = 0;
    uint8 u8RssiAdc =0;

	/*I2C using Crystal clock*/
	ui8Status   =  ATBM781xWrite(0x01, 0x0c, 0x00);
	ui8Status |= ATBM781xRead(0x00, 0x00, &ui8ChipID);
	ui8Status |=ATBM781xSleep(1); /*Delay 1 millisecond.	*/
	if((ui8ChipID == 0xa2)||(ui8ChipID == 0xa4))/**/
	{
		ui8Status |=ATBM781xWrite(0x06, 0x20, 0x00);
		ui8Status |=ATBM781xWrite(0x06, 0x07, 0x00);
		ui8Status |=ATBM781xWrite(0x06, 0x1b, 0x08); // 8.26
		ui8Status |=ATBM781xWrite(0x06, 0x18, 0x00); 
		ui8Status |=ATBM781xWrite(0x06, 0x06, 0x27); 
		ui8Status |=ATBM781xWrite(0x06, 0x13, 0x02);
		ui8Status |=ATBM781xWrite(0x06, 0x0b, 0x02);  /*sys */
		ui8Status |=ATBM781xWrite(0x06, 0x0a, 0x06); /*TS 123M*/
		ui8Status |=ATBM781xWrite(0x06, 0x09, 0x01); //PLL related Temperature range, 7.25
		ui8Status |=ATBM781xWrite(0x06, 0x21, 0x73);
		ui8Status |=ATBM781xWrite(0x06, 0x10, 0x02); /*ADC*/
		/*ui8Status |=ATBM781xWrite(0x06, 0x20, 0x22); //ADC*/
		ui8Status |=ATBM781xWrite(0x06, 0x1C, 0x00);
		ui8Status |=ATBM781xWrite(0x06, 0x05, 0x10);
		ui8Status |=ATBM781xWrite(0x06, 0x23, 0x03);
		ui8Status |= ATBM781xRead(0x06, 0x05,&u8RssiAdc);
		u8RssiAdc |= 0x02; /*Set bit1 = 1*/
		ui8Status |= ATBM781xWrite(0x06, 0x05,u8RssiAdc);
	}
#if ATBM781x_DVB_SS2_SUPPORT
	gATBM781xDiseqcClockKHz = 61500; /*Should use calculated value.*/
#endif
	 ui8Status |=ATBM781xSleep(1); /*Delay 1 millisecond.	*/
	return ui8Status;
}


ATBM781x_STATUS ATBM781xSetSysClkClockDVBC(void)
{

	ATBM781x_STATUS ui8Status = ATBM781x_SUCCESSFUL;
	uint8 ui8ChipID = 0, ui8AnaReg = 0, u8RssiAdc =0;
	/*I2C using Crystal clock*/
	ui8Status   =  ATBM781xWrite(0x01, 0x0c, 0x00);
	ui8Status |= ATBM781xSleep(1); /*Delay 1 millisecond.*/
	ui8Status |= ATBM781xRead(0x00, 0x00, &ui8ChipID);
	if((ui8ChipID == 0xa2)||(ui8ChipID == 0xa4))
	{
		/*DVB-C mode for 24MHz Crystal, System clock=72MHz*/
		/*ui8Status  |= ATBM781xWrite(0x00, 0x19, 0x01);	*/
		/*ui8Status  |= ATBM781xWrite(0x06, 0x06, 0x34);	FB_div=52*/
		/*ui8Status  |= ATBM781xWrite(0x06, 0x0a, 0x12);	*/
		/*ui8Status  |= ATBM781xWrite(0x06, 0x0B, 0x07);	SYS_DIV=7*/
		/*ui8Status  |= ATBM781xWrite(0x06, 0x0C, 0x19);	DVIP_div=25*/
	    /*ui8Status  |= ATBM781xWrite(0x06, 0x1C, 0x41);	change ADC clock to OSC clock*/
		/*ui8Status  |= ATBM781xWrite(0x06, 0x21, 0x73);*/
		/*ui8Status |= ATBM781xWrite(0x03, 0x06, 0x00);     ADC 1x mode*/

		ui8Status |=ATBM781xWrite(0x06, 0x07, 0x00);  //3-> 8_15 PLL
		ui8Status |=ATBM781xWrite(0x06, 0x1b, 0x08);  //add  8_15 PLL stable at low temperature
		ui8Status |=ATBM781xWrite(0x06, 0x18, 0x01);
		ui8Status |=ATBM781xWrite(0x06, 0x06, 0x26);
		ui8Status |=ATBM781xWrite(0x06, 0x13, 0x02);
		ui8Status |=ATBM781xWrite(0x06, 0x09, 0x01); //PLL related Temperature range, 7.25
		ui8Status |=ATBM781xWrite(0x06, 0x21, 0x73);
		ui8Status |=ATBM781xWrite(0x06, 0x0A, 0x0F); //56.4 MHz TS clock, 960M
		ui8Status |=ATBM781xWrite(0x06, 0x0B, 0x22); // set system clock to 120M
		ui8Status |=ATBM781xWrite(0x06, 0x0B, 0x02); // set system clock to 120M
		ui8Status |=ATBM781xWrite(0x06, 0x1C, 0x41);			
		ui8Status |= ATBM781xRead(0x06, 0x05,&u8RssiAdc);
		u8RssiAdc |= 0x02; /*Set bit1 = 1*/
		ui8Status |= ATBM781xWrite(0x06, 0x05,u8RssiAdc);
		ui8Status |=ATBM781xSleep(1); /*Delay 1 millisecond.	*/
		ui8Status |=ATBM781xWrite(0x01, 0x0C, 0x01); // using system clock div4 to set i2c
		ui8Status |=ATBM781xWrite(0x06, 0x0C, 0x32); // power down IP clock
		ui8Status |=ATBM781xWrite(0x06, 0x0C, 0x12); // power up IP clock
		ui8Status |=ATBM781xWrite(0x06, 0x0B, 0x05); // set system clock to 68M
	}
	if(ui8ChipID == 0xa2)
	{
		ui8Status |= ATBM781xRead(0x06, 0x1A, &ui8AnaReg);
		ui8AnaReg = ui8AnaReg + 58; /*0x40 - 6*/
		ui8Status |= ATBM781xWrite(0x06, 0x19, ui8AnaReg);
	}
	if((ui8ChipID == 0xa2)||(ui8ChipID == 0xa4))
	{
		ui8Status |=ATBM781xWrite(0x03, 0x06, 0x00);    /* ADC 1x mode*/
	}
#if ATBM781x_DVB_SS2_SUPPORT
	gATBM781xDiseqcClockKHz = 36000;
#endif
	return ui8Status;
}

/*Internal function
This function is for DVB-S/S2 mode if used.*/
ATBM781x_STATUS ATBM781xDVBS2CheckingThread(void)
{
	unsigned char u8TSFlag = 0;

	ATBM781x_STATUS status = ATBM781x_SUCCESSFUL;

	status |= ATBM781xRead(0x18, 0x2a, &u8TSFlag);
	if((u8TSFlag&0x01)!= 0x01)
	{	
		status |= ATBM781xDVBSWrite(0x41c,0x00);
	}
	return status;
}

/*Internal function.*/
/*This function can be used in thread or process periodically be called.*/
/*Can save some power consuming when working on DVB-T,DVB-C,DVB-S/S2 Mode*/
ATBM781x_STATUS ATBM781xPowerSavingProcess(void)
{
	ATBM781x_STATUS ui8Status = ATBM781x_SUCCESSFUL;
	uint8 ui8ConfigDone = 0, ui8TSLockFlag = 0, u8DvbtFecLock = 0;
	uint8 ui8TxMode = 0, ui8SxMode = 0, ui8DVBCMode =0;
	uint8 ui8CELDPCReg002a = 0;
	uint32 u32DvbcStatus = 0;
	/*Sx mode:002b = 1*/
	/*C mode: 0013 = 1*/

	ui8Status |= ATBM781xRead(0x00, 0x04, &ui8ConfigDone);
	if(ui8ConfigDone&0x01) /*Config done*/
	{
		ui8Status |=  ATBM781xTSLockFlag(&ui8TSLockFlag);
		ui8Status |= ATBM781xRead(0x00, 0x2b, &ui8SxMode);
		ui8Status |= ATBM781xRead(0x00, 0x13, &ui8DVBCMode);
		ui8Status |= ATBM781xRead(0x00, 0x2a, &ui8CELDPCReg002a);
		if(ui8SxMode&0x01) /*S/S2 mode*/
		{
#if ATBM781x_DVB_SS2_SUPPORT			
			ui8Status |= ATBM781xDVBSxModeDetected(&ui8SxMode);
#endif
			if(ui8TSLockFlag) /*DVB-S/S2 Locked*/
			{	
				if(ui8SxMode == ATBM781x_DVBS_MODE)
				{
					if(ui8CELDPCReg002a&0x03)
					{
						ui8CELDPCReg002a &=0xfc;
						ui8Status |= ATBM781xWrite(0x00,0x2a,ui8CELDPCReg002a); 

					}                            /*Shut CE and LDPC*/
				}
				if(ui8SxMode == ATBM781x_DVBS2_MODE)
				{					
					/*Shut CE	*/
					if(ui8CELDPCReg002a&0x01)
					{
						ui8CELDPCReg002a &=0xfe;
						ui8Status |= ATBM781xWrite(0x00,0x2a,ui8CELDPCReg002a); 

					}       
				}
			}
			else /*DVB-Sx not locked*/
			{
				/*open CE and LDPC*/
				if((ui8CELDPCReg002a&0x03)!=0x03)
				{
					ui8CELDPCReg002a |= 0x03;
					ui8Status |= ATBM781xWrite(0x00,0x2a,ui8CELDPCReg002a); 

				}
			} 
		} /*End of DVB-S/S2 mode*/

		else if(ui8DVBCMode&0x01) /*DVB-C mode*/
		{
			ui8Status |=  ATBM781xDVBCRead(0x08, &u32DvbcStatus);       /*read DVB-C internal register        */
			if(u32DvbcStatus&0x20) /*DVB-C FEC lock */
			{	
				if(ui8CELDPCReg002a&0x03)
				{
					ui8CELDPCReg002a &=0xfc;
					ui8Status |= ATBM781xWrite(0x00,0x2a,ui8CELDPCReg002a); 
				}                         /*Shut CE and LDPC*/
			}
			else /*DVB-C not locked*/
			{
				/*open CE */
				if((ui8CELDPCReg002a&0x01)!=0x01)
				{
					ui8CELDPCReg002a |= 0x01;
					ui8Status |= ATBM781xWrite(0x00,0x2a,ui8CELDPCReg002a); 
				}
			} 
		}
		else  /*DVB-Tx mode*/
		{
			ui8Status |= ATBM781xDVBTxModeDetected(&ui8TxMode);
			if(ui8TSLockFlag) /*DVB-T/T2 Locked*/
			{
				if(ui8TxMode == ATBM781x_DVBT_MODE)
				{					
					ui8Status |= ATBM781xRead(0x95, 0x37, &u8DvbtFecLock);
					/*Shut LDPC	*/
					if((ui8CELDPCReg002a&0x02)&&(u8DvbtFecLock&0x01))
					{
						ui8CELDPCReg002a &=0xfd;
						ui8Status |= ATBM781xWrite(0x00,0x2a,ui8CELDPCReg002a); 

					}       
				}
			}
			else
			{
				/*open CE and LDPC*/
				if((ui8CELDPCReg002a&0x03)!=0x03)
				{
					ui8CELDPCReg002a |= 0x03;
					ui8Status |= ATBM781xWrite(0x00,0x2a,ui8CELDPCReg002a); 
				}
			}
		}

	}
	else  /*Not setting*/
	{

	}
	return ui8Status;
}

/*Internal function call by ATBM781xHoldDSP*/
ATBM781x_STATUS ATBM781xRestorRegisters(void)
{
	ATBM781x_STATUS ui8Status = ATBM781x_SUCCESSFUL;
	uint8 ui8ChipID = 0;
	uint8 ui8Reg002A = 0, ui8Reg0013 = 0;
	ui8Status |= ATBM781xChipID(&ui8ChipID);
	if(ui8ChipID == 0xa4)
	{
		ui8Status |= ATBM781xRead(0x00, 0x2a, &ui8Reg002A);
		ui8Status |= ATBM781xRead(0x00, 0x13, &ui8Reg0013);
		if(ui8Reg0013&0x01)
		{
			ui8Reg002A|=0x01;
		}
		else
		{
			ui8Reg002A|=0x03;
		}

		ui8Status |= ATBM781xWrite(0x00, 0x2a, ui8Reg002A);
	}
	return ui8Status;
}
/*Internal function.*/
ATBM781x_STATUS ATBM781xPllLockCheck(void)
{
	ATBM781x_STATUS ui8Status = ATBM781x_SUCCESSFUL;
	uint8 ui8ChipID = 0;
	uint8 ui8Reg0630 = 0;
	uint8 ui8PllPd = 0;
	uint8 uiCfgDone = 0;
	uint8 ui8SxMode = 0;
	ui8Status |= ATBM781xChipID(&ui8ChipID);
	ui8Status |= ATBM781xRead(0x00, 0x04, &uiCfgDone);
	ui8Status |= ATBM781xRead(0x06, 0x08, &ui8PllPd);
	if((ui8ChipID == 0xa4)&&((uiCfgDone&0x01) ==0x01)&&((ui8PllPd&0x01) == 0x00))
	{
		ui8Status |= ATBM781xRead(0x06, 0x30, &ui8Reg0630);		
		if(ui8Reg0630 == 0x00)
		{
			ui8Status |= ATBM781xWrite(0x06, 0x21, 0x7b);				
			ui8Status |= ATBM781xWrite(0x06, 0x21, 0x73);
			ui8Status |= ATBM781xSleep(10);          
			ui8Status |= ATBM781xRead(0x00, 0x2b, &ui8SxMode);
			ui8Status |= ATBM781xRestorRegisters(); 
			if(ui8SxMode&0x01) /*S/S2 mode*/
			{
#if ATBM781x_DVB_SS2_SUPPORT
				ui8Status |= ATBM781xDvbSxSymbolRateReset( );
#endif
			}
			ui8Status |=ATBM781xWrite(0x00, 0x05, 0x01);
			ui8Status |=ATBM781xWrite(0x00, 0x05, 0x00);			
			
		}	
	}
	return ui8Status;
}
/********************************************************************************
function    : ATBM781xDynamicCheckProcess
parameter   :none 
return      :Operate  Status:ATBM781x_SUCCESSFUL is Normal and other is abnormal.
Description:
This  function should be called dynamically every 500 milliseconds.
will check signal status and save some power. or process non-standard compatible signal.
*********************************************************************************/
ATBM781x_STATUS ATBM781xDynamicCheckProcess(void)/*API should be called on 500ms period.*/
{
	ATBM781x_STATUS status = ATBM781x_SUCCESSFUL;
	uint8 ui8ConfigDone = 0;
	uint8 ui8SxMode = 0, ui8DVBCMode =0;
	uint8 ui8S2Fec = 0, ui8S2Tmp0 = 0, ui8S2Tmp1 = 0, ui8TSLockFlag = 0;
    uint8 ui8ChipID = 0;
	status = ATBM781xRead(0x00, 0x04, &ui8ConfigDone);
	status = ATBM781xRead(0x00, 0x00, &ui8ChipID);
	/*if(ui8ChipID!= 0xa4)*/
	/*{*/
	/*	return status;*/
	/*}*/

	if(ui8ConfigDone&0x01) /*Config done*/
	{
		status |= ATBM781xPllLockCheck();
		status |= ATBM781xRead(0x00, 0x2b, &ui8SxMode);
		status |= ATBM781xRead(0x00, 0x13, &ui8DVBCMode);
		if(((ui8DVBCMode&0x01)!= 0x01)&&((ui8SxMode&0x01)!=0x01))
		{
			status |= ATBM781xDVBT2ThreadProcess();
		}
		if(ui8SxMode&0x01) /*S/S2 mode*/
		{
#if ATBM781x_DVB_SS2_SUPPORT
			status |= ATBM781xCalibrateDVBSxSymbolRate();
#endif
			status |= ATBM781xRead(0x17, 0xec, &ui8S2Fec);
			status |= ATBM781xTSLockFlag(&ui8TSLockFlag);
			if((ui8S2Fec&0x01)&&(!ui8TSLockFlag))
			{
				status |= ATBM781xRead(0x17,0xd4,&ui8S2Tmp0);				
				status |= ATBM781xRead(0x17,0xd3,&ui8S2Tmp1);
				if((ui8S2Tmp0 ==0)&&(ui8S2Tmp1 == 0))
				{
					status |= ATBM781xWrite(0x00, 0x05, 0x01);
					status |= ATBM781xWrite(0x00, 0x05, 0x00);
					/*ATBM781xHoldDSP();*/
					/*ATBM781xStartDSP();*/
				}
			}
#if ATBM781x_DVB_SS2_SUPPORT
			status |= ATBM781xDVBS2CheckingThread();			
			status |= ATBM781xGetDvbSxSpectrumAuto();
#endif
		}
		status |= ATBM781xPowerSavingProcess();		
	}
	return status;
}
/********************************************************************************
function    :ATBM781xCheckSDKPortForFAE
parameter   :none 
return      :Operate Status:ATBM781x_SUCCESSFUL is Normal and other is abnormal.
Description:
This function can be used for FAE to check SDK porting.
DVB-T2: DVB-T2 MPLP signal should be used. then receive the first PLP.
        This function should be called after TS locked.

DVB-T:  After TS locked call this function.

DVB-S/S2: After TS locked call this function.
*********************************************************************************/
ATBM781x_STATUS ATBM781xCheckSDKPortForFAE(void)
{
	ATBM781x_STATUS ui8Status = ATBM781x_SUCCESSFUL;
	uint8 ui8TSLockFlag = 0;
	uint8 u8DVBTxMode = 0, ui8SxMode = 0, ui8DVBCMode =0, u8DataPlpNumber =0;
	uint8 ui8AutoMode = 0;
	uint8 u8iqState = 0, ui8CELDPCReg002a;
	int i32SmpOffset = 0;
	uint32 u32Reg = 0;
	ui8Status |= ATBM781xTSLockFlag(&ui8TSLockFlag);
	ui8Status |= ATBM781xRead(0x00, 0x2b, &ui8SxMode);
	ui8Status |= ATBM781xRead(0x00, 0x13, &ui8DVBCMode);
	if(ui8TSLockFlag == 1)
	{
		if(((ui8DVBCMode&0x01)!= 0x01)&&((ui8SxMode&0x01)!=0x01)) /*Locked on DVB-T/T2*/
		{			
			ui8Status |= ATBM781xGetDVBTxSampleOffsetPPM(&i32SmpOffset);
			if((i32SmpOffset >100)||(i32SmpOffset < -100))
			{
				printf("Crystal Sample Offset not calibrated\n");
			}

			ui8Status |= ATBM781xDVBTxModeDetected(&u8DVBTxMode);
			if((u8DVBTxMode ==ATBM781x_DVBT_MODE)&&(ui8Status== ATBM781x_SUCCESSFUL))
			{
				ui8Status |= ATBM781xRead(0x00, 0x2a, &ui8CELDPCReg002a);
				if((ui8CELDPCReg002a&0x03)==0x03)
				{
					printf("Check Power saving, not in power saving state\n");
				}                         /*Shutdown CE or LDPC*/
				printf("PLP test Should use MPLP DVB-T2 signal then receive the first PLP\n");
			}
			if((u8DVBTxMode ==ATBM781x_DVBT2_MODE)&&(ui8Status== ATBM781x_SUCCESSFUL))
			{
				ui8Status |=ATBM781xRead(0x24, 0x05,  &u8DataPlpNumber);
				if(u8DataPlpNumber < 2)
				{
					printf("Should use MPLP DVB-T2 signal and receive the first PLP\n");
				}
				ui8Status |=ATBM781xRead(0x24, 0x03,  &ui8AutoMode);
				if(ui8AutoMode!=0)
				{
					printf("The first PLP of DVB-T2 should use auto PLP mode\n");
				}
			}
		}
		if(((ui8SxMode&0x01) == 0x01)) /*Locked on DVB-S/S2*/
		{
			ui8Status |= ATBM781xRead(0x15,0xe1,&u8iqState); /*Lock on inversed IQ*/
			if(u8iqState&0x01)
			{
				printf("IQ inversed, should set correct IQ state\n");
			}
			ui8Status |= ATBM781xRead(0x00, 0x2a, &ui8CELDPCReg002a);
			if(ui8CELDPCReg002a&0x03)
			{
				printf("Check Power saving, not in power saving state\n");
			}                         /*Shutdown CE or LDPC*/
		}
	}
	else /*TS not locked*/
	{   /*DVB-T/T2*/
		if(((ui8DVBCMode&0x01)!= 0x01)&&((ui8SxMode&0x01)!=0x01)) /*DVB-T/T2 mode*/
		{
			printf("Signal not locked, Check function requires signal locked\n");
			return ui8Status;
		}
		/*DVB-S/S2*/
		if(((ui8SxMode&0x01) == 0x01)) /*DVB-S/S2 mode TS not locked*/
		{
			ui8Status |=ATBM781xDVBSRead(0x41c,&u32Reg);
			if(u32Reg!=0)
			{
				printf("When signal not locked the value of reg41c should be zero\n");
			}
		}
	}
	return ui8Status;
}
/*Sample Code*/
int reginitmain()
{
	ATBM781x_STATUS ui8Status = ATBM781x_SUCCESSFUL;
	/*ATBM781xPowerOnInit() should be called before any other demodulator function , client setting according to hardware was put in this function.*/
	ui8Status |= ATBM781xPowerOnInit(); 	
	ui8Status |= ATBM781xSetDVBTxMode(8000);   /*1.7MHz, 5MHz,6MHz,7MHz,8MHz Bandwidth support*/
	/* ui8Status |= ATBM781xSetDVBCMode();*/      /* DVB-C symbol rate auto detection on 8MHz Bandwidth*/
	/*ui8Status |= ATBM781xSetDvbSxMode();*/    /* DVB-S/S2 symbol rate auto detection. up to 45Msymbol rate*/
	if(ui8Status != ATBM781x_SUCCESSFUL)
	{
		/*ATBM781xDebugPrint("ATBM781xPowerOnInit or Mode set failed\n"); */
	}
	return 0;
}
