#ifndef _RDATV_BB_DEFS_H_
#define _RDATV_BB_DEFS_H_

#include "../DataType.h"



	

//#define	RDATV_XTAL						(26000)
#define BAND_VH_START				(130000)
#define BAND_U_START				(330000)
#define RDATV_IF_FREQ  			        (4500)

#define RDA5880H_TUNER_I2C_ADDR 0xc0
#define MB86A22_I2C_ADDR 0x20

#define sigle_lna

typedef enum {
	RDA5880_OK  =  0,	/* function execution was successful */
	RDA5880_ERR =  1	/* I2C bus communication problem */
} RDA5880_ERR_MSG ;


typedef struct{
	UINT8  add;
	UINT16 data;
	UINT8 delay;
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

typedef struct
{
 UINT32 Fre;
 UINT16 Delta;
 RDATV_ADC_INDEX Adc; 
}PATCH_ADC_BY_FRE;

typedef enum
{
	BW_MODE_6MHZ=0,
	BW_MODE_7MHZ,
	BW_MODE_8MHZ,
}RDATV_BANDWIDTH;

typedef struct
{
	UINT32 RDATV_XTAL ;
	UINT16 TVReg240,TVReg242,TVReg62,TVReg64,TVReg250,TVReg24e,TVReg232,TVReg66;
	UINT8  TVReg46_2_X,TVreg1c_0,TVReg10_8_10,TVReg14_10_15;
	UINT8  lnamode, demod,tvmode,ifout_mode,freq_adc_index,mode_t;
	UINT32	freq_ifout;
	UINT16 I2VFLOOR,I2VLTH,I2VHTH;
	//UINT32 gu32FreqKHz;
	//UINT32 gu32BandwidthKHz;
} RDA5880_PARAM ;

#endif //_RDATV_BB_DEFS_H_

