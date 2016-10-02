/*

Version 1.0   2013.09.13
 The  draft version

 Version 1.1 2013.09.26
 Modify the RDATVDownLoad in rdatv_bb.c for Solving the FEF issue.

   Version 1.2 2013.10.21
 Modify the RDATVDownLoad in rdatv_bb.c for Solving the FEF issue.

*/
#ifndef	_RDATVBB_H
#define	_RDATVBB_H

//#define	RDATV_TEST
#define   RDA5880_TUNER_I2C_addr 0x60
#define   RDA5880_I2C_addr_WR_BIT 0xC0  // add read write bit

#ifdef	RDATV_TEST
#include "i2c_drv.h"
#include "rdaglobe.h"


#define	RDATVM_WRITE(add,tdata)		
#define	RDATVM_READ(add,pdata)		
#define	RDADelayMs(x)				       		
#else
#include "..\DataType.h"
/*
#ifndef	UINT32
#define	UINT32		unsigned long
#endif
#ifndef	S32
#define	S32		long
#endif
#ifndef	UINT16
#define	UINT16		unsigned short
#endif
#ifndef	UINT16
#define	S16		short
#endif
#ifndef	UINT8
#define	UINT8		unsigned char
#endif
#ifndef	S8
#define	S8		char
#endif
#define	mrom

#define	RDATVM_WRITE(add,tdata)		
#define	RDATVM_READ(add,pdata)		
#define	RDADelayMs(x)		
*/
#endif

#define	RDATV_XTAL						(27000)
#define 	BAND_VH_START					(130000)
#define 	BAND_U_START					(330000)
#define 	RDATV_IF_FREQ  			        (4500)

typedef struct{
	UINT16 add;
	UINT16 data;
	UINT16 delay;
}RDATVM_REG;

typedef enum
{
	SINGLE = 0,
	DIFF
}RDATV_LNA_MODE;


typedef enum
{
	MSB7819 = 0,
	MSB7818,
	MSB71236D,
	NOV,
	PAN88472,
	AVAILINK,
	ALTOBEAM,
	X
}RDATV_IF_POWER;





typedef enum
{
	DK = 0,
	I,
	GH,
	B,
	N,
	M,
	L,
	L1,
	DTV,
	DTV_X
}RDATV_MODE;

typedef enum
{
	mauto=0,
	manual
}RDATV_IFOUT_MODE;

typedef enum
{
	ATV_IF=1,
	ATV_BB,
	DTV_BB,
	DTV_IF
}RDATV_OUT_MODE_T;

typedef enum
{
	ADC_151=0,
	ADC_162,
	ADC_172,
	ADC_N,
	ADC_141,
	ADC_162X,
	ADC_182
}RDATV_ADC_INDEX;


typedef struct PATCH_ADC_BY_FRE_T
{
 UINT32 Fre;
 UINT16 Delta;
 RDATV_ADC_INDEX Adc; 
}PATCH_ADC_BY_FRE;

typedef enum {
	RDA5880_OK  =  0,	/* function execution was successful */
	RDA5880_ERR =  1	/* I2C bus communication problem */
} RDA5880_ERR_MSG ;

UINT16 RDATVGetChipID(void);

#ifndef CLOSE_TUNER_UNNECESSARY_FUNCTION

int RDATVDetected(void);
#endif

void	RDATVPowerOn(void);
void RDATVSetBP(void);
UINT32 RDATVGetLnaGain(void);
void RDATVGetI2VTH(void);
UINT32	RDATVSetFreq(UINT32 freq,UINT32 bw);
void RDATVSleepnoloop(void);
void RDATVSleepwithloop(void);
// UINT16 RDATVSingalStrength(void);
RDA5880_ERR_MSG RDATVSingalStrength( UINT8 *strength );
void RDATVLnaMode(UINT8 mode);
void RDATVsetdemod(UINT8 mode)	;
void RDATVSetMode(UINT8 mode);		
void RDATVSetIfOutMode(UINT8 mode);
void RDATVSetModeOutT(UINT8 mode);
void RDATVSetAdcIndex(UINT8 index);
void RDATVSetOutMode(UINT8 mode);
void RDATVsetlna(UINT32 freq);
void RDATVSet51Sta(UINT8 Status);
void RDATVDownLoad(void);
UINT32 devTunerInit(void);
UINT32 devDigitalTuner_SetFreq (UINT32 freq , UINT32 eBandWidth );

#endif


