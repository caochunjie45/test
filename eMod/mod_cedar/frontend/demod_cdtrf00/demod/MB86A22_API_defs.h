/*n.*
*
*  Front-end device driver for Fujitsu MB86A22 ISDB-T frontend.
*
*  Copyright (c) 2010 Fujitsu Semiconductor 
*  All rights reserved. 
*
*  Created 12/14/2010
*
****************************************************************************/
#ifndef _MB86A22_API_DEFS_H_
#define _MB86A22_API_DEFS_H_

//#include <Os_adapter.h>


/****************************************************************************
*       Imports and definitions for WIN32                             
****************************************************************************/
//typedef unsigned char  UINT8 ;
//typedef unsigned short UINT16 ;
//typedef unsigned int   UINT32 ;
typedef char           SINT8 ;
typedef short          SINT16 ;
typedef int            SINT32 ;

/*
****************************************************************************
*	MB86A21 Error message
****************************************************************************
*/

typedef enum {
	MB86A2x_OK         =  0,	/* function execution was successful */
	MB86A2x_ERR_INIT   =  1,	/* I2C bus communication problem */
	MB86A2x_ERR_RFTUNE =  2,	/* I2C bus communication problem in RF tuning */
	MB86A2x_ERR_OTHERS = 10		/* other problem */
} MB86A2x_ERR_MSG ;

/*
****************************************************************************
*	MB86A21 Device Address
***************************************************************************
*/
typedef enum {
	MB86A2x_DEV_ADR_20 = 0x20,	//ADR1=0, ADR0=0
	MB86A2x_DEV_ADR_22 = 0x22,	//ADR1=0, ADR0=1
	MB86A2x_DEV_ADR_24 = 0x24,	//ADR1=1, ADR0=0
	MB86A2x_DEV_ADR_26 = 0x26	//ADR1=1, ADR0=1
} MB86A2x_DEV_ADR ;


/*
***************************************************************************
*	MB86A21 IF Frequency
***************************************************************************
*/
typedef enum {
	MB86A2x_IF_FREQ_00_000kHz =     0,	/* IF frequency =  0.0MHz (IQ) */
	MB86A2x_IF_FREQ_03_300kHz =  3300,	/* IF frequency =  3.3MHz */
	MB86A2x_IF_FREQ_03_650kHz =  3650,	/* IF frequency =  3.3MHz */
	MB86A2x_IF_FREQ_04_000kHz =  4000,	/* IF frequency =  4.0MHz */
	MB86A2x_IF_FREQ_04_500kHz =  4500,	/* IF frequency =  4.5MHz */
	MB86A2x_IF_FREQ_05_000kHz =  5000,	/* IF frequency =  5.0MHz */
	MB86A2x_IF_FREQ_36_000kHz = 36000,	/* IF frequency = 36.0MHz */
	MB86A2x_IF_FREQ_44_000kHz = 44000,	/* IF frequency = 44.0MHz */
	MB86A2x_IF_FREQ_57_000kHz = 57000		/* IF frequency = 57.0MHz */
} MB86A2x_IF_IFREQ ;


/*
***************************************************************************
*	MB86A21 Reception Layer Setting
***************************************************************************
*/
typedef enum {
	MB86A2x_RECEP_LAYER_A = 0,	/* Layer-A */
	MB86A2x_RECEP_LAYER_B = 1,	/* Layer-B */
	MB86A2x_RECEP_LAYER_C = 2,	/* Layer-C */
	MB86A2x_RECEP_HIGHEST = 3	/* Highest Layer */
} MB86A2x_RECEP_LAYER ;


/*
***************************************************************************
*	MB86A21 Channel Unit
***************************************************************************
*/
typedef enum {
	MB86A2x_CH_UNIT_JP = 0,		/* Channel unit = Japan */
	MB86A2x_CH_UNIT_BZ = 1,		/* Channel unit = Brazil */
	MB86A2x_CH_UNIT_kHz = 2		/* Channel unit = kHz */
} MB86A2x_CH_UNIT ;


/*
***************************************************************************
*	MB86A21 PLL Reference Clock Frequency
***************************************************************************
*/
typedef enum {
	MB86A2x_REFSEL_04MHz = 0,	/* PLL reference clock =  4MHz */
	MB86A2x_REFSEL_16MHz = 1,	/* PLL reference clock = 16MHz */
	MB86A2x_REFSEL_26MHz = 2,	/* PLL reference clock = 26MHz */
	MB86A2x_REFSEL_32MHz = 3	/* PLL reference clock = 32MHz */

} MB86A2x_REFSEL ;

/*
***************************************************************************
*	MB86A21 BANDSEL
***************************************************************************
*/
typedef enum {
	MB86A2x_BANDSEL_ISDBT_6MHz = 0,	
	MB86A2x_BANDSEL_ISDBT_7MHz = 1,	
	MB86A2x_BANDSEL_ISDBT_8MHz = 2,		
	MB86A2x_BANDSEL_DVBT_6MHz = 3,	
	MB86A2x_BANDSEL_DVBT_7MHz = 4,	
	MB86A2x_BANDSEL_DVBT_8MHz = 5
} MB86A2x_BANDSEL ;

/*
***************************************************************************
*	MB86A21 FREQSEL
***************************************************************************
*/
typedef enum {
	MB86A2x_FREQSEL_0 = 0,	
	MB86A2x_FREQSEL_1 = 1,	
	MB86A2x_FREQSEL_2 = 2,	
	MB86A2x_FREQSEL_x = 0	

} MB86A2x_FREQSEL ;

/*
***************************************************************************
*	MB86A21 TS Output Selection
***************************************************************************
*/
typedef enum {
	MB86A2x_TS_OUT_PARALLEL = 0,	/* TS Output = Parallel */
	MB86A2x_TS_OUT_SERIAL   = 1		/* TS Output = Serial */
} MB86A2x_TS_OUT_SEL ;


/*
***************************************************************************
*	MB86A21 Error Packet Mask Function Selection
***************************************************************************
*/
typedef enum {
	MB86A2x_TS_ERR_MASK_OFF = 0,	/* Error packet = as it is */
	MB86A2x_TS_ERR_MASK_ON  = 1		/* Error packet = changed to null packet */
} MB86A2x_ERR_PAC_MASK ;


/*
***************************************************************************
*	MB86A21 Parallel TS Output Layer Selection
***************************************************************************
*/
typedef enum {
	MB86A2x_TS_LAYER_NULL = 0,	/* TS Output layer = null */
	MB86A2x_TS_LAYER_A    = 1,	/* TS Output layer = layer-A */
	MB86A2x_TS_LAYER_B    = 2,	/* TS Output layer = layer-B */
	MB86A2x_TS_LAYER_C    = 3,	/* TS Output layer = layer-C */
	MB86A2x_TS_LAYER_AB   = 4,	/* TS Output layer = layer-A,B */
	MB86A2x_TS_LAYER_AC   = 5,	/* TS Output layer = layer-A,C */
	MB86A2x_TS_LAYER_BC   = 6,	/* TS Output layer = layer-B,C */
	MB86A2x_TS_LAYER_ALL  = 7	/* TS Output layer = layer-A,B,C and null */
} MB86A2x_TS_LAYER ;


/*
***************************************************************************
*	MB86A21 TS Output Clock Polarity Setting
***************************************************************************
*/
typedef enum {
	MB86A2x_TS_CLK_NEG = 0,	/* TS Output clock = negative edge */
	MB86A2x_TS_CLK_POS = 1	/* TS Output clock = positive edge */
} MB86A2x_TS_CLK ;


/*
***************************************************************************
*	MB86A21 TS Output RS Parity Byte Setting
***************************************************************************
*/
typedef enum {
	MB86A2x_TS_RS_PARITY_OFF = 0,	/* TS Parity Output = Off */
	MB86A2x_TS_RS_PARITY_ON  = 1	/* TS Parity Output = On */
} MB86A2x_TS_RS_PARITY ;


/*
***************************************************************************
*	MB86A21 TS Output Sync Byte Setting
***************************************************************************
*/
typedef enum {
	MB86A2x_TS_SYNC_OFF = 0,	/* TS Sync Byte Output = Off */
	MB86A2x_TS_SYNC_ON  = 1		/* TS Sync Byte Output = On */
} MB86A2x_TS_SYNC ;



#endif

