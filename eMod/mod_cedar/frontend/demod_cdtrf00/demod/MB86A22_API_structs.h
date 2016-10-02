/*
***************************************************************************
*
*  Front-end device driver for Fujitsu MB86A22 ISDB-T frontend.
*
*  Copyright (c) 2010 Fujitsu Semiconductor 
*  All rights reserved. 
*
*  Created 12/14/2010
*
***************************************************************************
*/
#ifndef _MB86A22_API_STRUCTS_H_
#define _MB86A22_API_STRUCTS_H_

//include "..\X86DataType.h"
#include "../tuner/Tuner.h" 

/*
***************************************************************************
*	MB86A21 VERSION Parameter
***************************************************************************
*/
typedef struct
{
	UINT8  LSI_VERSION ;	/* Demodulator version */
	UINT16 DRV_VERSION ;	/* Driver version */
} MB86A2x_VERSION_INFO;


/*
***************************************************************************
*	MB86A21 NIM Parameter
***************************************************************************
*/
// Editing for MxL
//typedef struct 
//{
//	TUNER_PARAM     tuner_param ;
//	MB86A2x_DEV_ADR MB86A2x_Adr ;	/* tuner device address */
//	UINT8           IQ_pol ;		/* IQ polarity */
//	UINT8           POWER_DOWN ;	/* Power Down */
//	UINT8           RF_CONT_EN ;	/* RF control enable(Win software only) */
//} MB86A2x_NIM_PARAM ;

typedef struct 
{
	RDA5880_TunerConfigS tuner_param ;
	MB86A2x_DEV_ADR      MB86A2x_Adr ;	/* tuner device address */
	UINT8                IQ_pol ;		/* IQ polarity */
	UINT8                POWER_DOWN ;	/* Power Down */
	UINT8                RF_CONT_EN ;	/* RF control enable(Win software only) */
} MB86A2x_NIM_PARAM ;


/*
***************************************************************************
*	MB86A21 PLL Parameter
***************************************************************************
*/
typedef struct 
{
	UINT32          INPUT_CLK_FREQ_kHz ;    /* input clock frequency */
	UINT32          PLLPASS ;				/* PLL bypass enable */
	MB86A2x_BANDSEL BANDSEL ;				
	MB86A2x_FREQSEL FREQSEL ;				
	MB86A2x_REFSEL  REFSEL ;			    /* reference frequency selection */
	UINT32          MASTER_CLK_FREQ_kHz ;	/* mater clock frequency */
} MB86A2x_PLL_PARAM ;


/*
***************************************************************************
*	MB86A21 Segment Parameter
***************************************************************************
*/
typedef struct 
{
	MB86A2x_RECEP_LAYER RECEP_LAYER ;	/* recepion layer */
} MB86A2x_LAYER_PARAM ;


/*
***************************************************************************
*	MB86A21 Recovery Parameter
***************************************************************************
*/
typedef struct 
{
	UINT32 CRR_OFS ;	/* carrier recovery offset value */
	UINT32 STR_OFS ;	/* symbol timing recovery offset value */
} MB86A2x_RECOV_PARAM ;


/*
***************************************************************************
*	MB86A21 AGC Parameter
***************************************************************************
*/
typedef struct 
{
	UINT8 MAIF ;		/* IFAGC Manual setting */
	UINT8 IFAGC_POL ;	/* IFAGC polarity */
	UINT8 RFAGC_ON ;	/* General purpose DAC Switch */
} MB86A2x_AGC_PARAM ;


/*
***************************************************************************
*	MB86A21 IFAGC Parameter
***************************************************************************
*/
typedef struct 
{
	UINT32 VIFREF ;		/* IFAGC reference value */
	UINT8  AGAIN ;		/* IFAGC gain setting */
	UINT32 DTS ;		/* filter coefficient of amplitude detector */
	UINT32 IFA ;		/* filter coefficient of IFAGC (alpha) */
	UINT32 IFB ;		/* filter coefficient of IFAGC (beta) */
	UINT8  IFAGCO ;		/* IFAGC offset value */
	UINT8  MAXIFAGC ;	/* maximum output value of AGC */
	UINT8  IFSAMPLE ;	/* sampling frequency of amplitude detector */
	UINT8  OUTSAMPLE ;	/* sampling frequency of loop filter */
	UINT8  VMAIF ;      /* IFAGCDAC manual setting value */
} MB86A2x_IFAGC_PARAM ;


/*
***************************************************************************
*	MB86A21 RFAGC Parameter
***************************************************************************
*/
typedef struct 
{
	UINT8  VMARF ;      /* General purpose DAC output value */
} MB86A2x_RFAGC_PARAM ;


/*
***************************************************************************
*	MB86A21 Tuner Dependent Parameter
***************************************************************************
*/
typedef struct 
{
	UINT32 S1CNT ;	/* timer setting of sequencer state 1 */
} MB86A2x_TU_DEP_PARAM ;


/*
***************************************************************************
*	MB86A21 TS Output Parameter
***************************************************************************
*/
typedef struct 
{
	MB86A2x_TS_OUT_SEL   TS_OUT_SEL ;	/* TS output selection (serial or parallel) */
	MB86A2x_ERR_PAC_MASK TS_ERR_MASK ;	/* converting error packet to null packet */
	MB86A2x_TS_LAYER     TS_LAYER ;		/* TS output layer selection */
	MB86A2x_TS_CLK       TS_CLK ;		/* TS clock polarity */
	MB86A2x_TS_RS_PARITY TS_PARITY ;	/* TS parity byte output enable */
	MB86A2x_TS_SYNC      TS_SYNC ;		/* TS sync byte output enable */
} MB86A2x_TSOUT_PARAM ;


/*
***************************************************************************
*	MB86A21 Monitor Timer Parameter
***************************************************************************
*/
typedef struct 
{
	UINT32 VBER_A_NUM ;	/* bit number setting of pre-Viterbi BER of layer-A */
	UINT32 VBER_B_NUM ;	/* bit number setting of pre-Viterbi BER of layer-B */
	UINT32 VBER_C_NUM ;	/* bit number setting of pre-Viterbi BER of layer-C */
	UINT32 SBER_A_NUM ;	/* packet number setting of post-Viterbi BER of layer-A */
	UINT32 SBER_B_NUM ;	/* packet number setting of post-Viterbi BER of layer-B */
	UINT32 SBER_C_NUM ;	/* packet number setting of post-Viterbi BER of layer-C */
	UINT32 PBER_A_NUM ;	/* packet number setting of packet error rate of layer-A */
	UINT32 PBER_B_NUM ;	/* packet number setting of packet error rate of layer-B */
	UINT32 PBER_C_NUM ;	/* packet number setting of packet error rate of layer-C */
	UINT32 CN_SYMBOL ;	/* symbol number setting of C/N monitor */
	UINT32 MER_SYMBOL ;	/* symbol number setting of C/N monitor */
} MB86A2x_MON_TIM_PARAM ;


/*
***************************************************************************
*	MB86A21 Channel Search Parameter
***************************************************************************
*/
typedef struct 
{
	UINT32 START_CH ;			/* channel serch start channel */
	UINT32 STOP_CH ;			/* channel serch stop channel */
	UINT32 WAIT_CH_SCH_STAT ;	/* wait time to check whether the first stage of channel search has finished or not [ms]*/
	UINT32 WAIT_CH_SCH_SEQ ;	/* wait time to check whether the state of the sequencer has reached to 8 or not [ms] */
	UINT32 SCH_LOOP_MAX ;	/* maximum repeat time to check the state of the sequencer */
} MB86A2x_CH_SCH_PARAM ;


/*
***************************************************************************
*	MB86A21 Configuration
***************************************************************************
*/
typedef struct 
{
	MB86A2x_NIM_PARAM     nim_param ;
	MB86A2x_PLL_PARAM     pll_param ;
	MB86A2x_LAYER_PARAM   layer_param ;
	MB86A2x_RECOV_PARAM   recov_param ;
	MB86A2x_AGC_PARAM     agc_param ; 
	MB86A2x_IFAGC_PARAM   ifagc_param ;
	MB86A2x_RFAGC_PARAM   rfagc_param ;
	MB86A2x_TU_DEP_PARAM  tun_dep_param ;
	MB86A2x_TSOUT_PARAM   tsout_param ;
	MB86A2x_MON_TIM_PARAM mon_tim_param ;
	MB86A2x_CH_SCH_PARAM  ch_sch_param ;
} MB86A2x_PARAM ;


/*
***************************************************************************
*	MB86A21 Monitor value
***************************************************************************
*/

typedef struct
{
	UINT8 STATE ;	/* state value of the sequencer */
	UINT8 S8_REACH ;
	UINT8 S8_REACH_his ;
} MB86A2x_LOCKNG_INFO ;


typedef struct 
{
	UINT32 IF_INPUT_LEVEL ;	/* IF input level */
	UINT8  IFAGCDAC ;		/* DAC output value of IFAGC */
	UINT8  RFAGCDAC ;		/* DAC output value of IFAGC */
} MB86A2x_AGC_INFO ;


typedef struct
{
	SINT32 STR_freq_error_Hz ;	/* frequency error detected  by symbol timing recovery */
	SINT32 STR_ppm_error ;
	SINT32 CRR_freq_error_kHz ;	/* frequency error detected  by carrier recovery */
} MB86A2x_FREQ_ERR_INFO ;


typedef struct
{
	UINT8 MODE_current ;	/* mode */
	UINT8 GUARD_current ;	/* guard interval */
	UINT8 SYSTEM_current ;	/* symstem */
	UINT8 EMERG_current ;	/* emergency flag */
	UINT8 CNTDN_current ;	/* tmcc count dwon */
	UINT8 PARTIAL_current ;	/* partila reception */
	UINT8 MOD_A_current ;	/* modulation of layer-A */
	UINT8 MOD_B_current ;	/* modulation of layer-B */
	UINT8 MOD_C_current ;	/* modulation of layer-C */
	UINT8 RATE_A_current ;	/* code rate of layer-A */
	UINT8 RATE_B_current ;	/* code rate of layer-B */
	UINT8 RATE_C_current ;	/* code rate of layer-C */
	UINT8 IL_A_current ;	/* interleave depth of layer-A */
	UINT8 IL_B_current ;	/* interleave depth of layer-B */
	UINT8 IL_C_current ;	/* interleave depth of layer-C */
	UINT8 NUM_A_current ;	/* segment number of layer-A */
	UINT8 NUM_B_current ;	/* segment number of layer-B */
	UINT8 NUM_C_current ;	/* segment number of layer-C */
	UINT32 STR_current ;
} MB86A2x_TMCC_INFO ;


typedef struct
{
	UINT8  VBER_A_update ;	/* Pre-Viterbi BER count update flag of layer-A */
	UINT8  VBER_B_update ;	/* Pre-Viterbi BER count update flag of layer-B */
	UINT8  VBER_C_update ;	/* Pre-Viterbi BER count update flag of layer-C */
	UINT8  SBER_A_update ;	/* Post-Viterbi BER count update flag of layer-A */
	UINT8  SBER_B_update ;	/* Post-Viterbi BER count update flag of layer-B */
	UINT8  SBER_C_update ;	/* Post-Viterbi BER count update flag of layer-C */
	UINT8  PBER_A_update ;	/* packet error count update flag of layer-A */
	UINT8  PBER_B_update ;	/* packet error count update flag of layer-B */
	UINT8  PBER_C_update ;	/* packet error count update flag of layer-C */
	UINT8  CN_update ;		/* C/N monitor couunt update flag */
	UINT8  MER_update ;		/* MER monitor couunt update flag */
	UINT32 V_ERR_A ;		/* Pre-Viterbi Error number of layer-A */
	UINT32 V_ERR_B ;		/* Pre-Viterbi Error number of layer-B */
	UINT32 V_ERR_C ;		/* Pre-Viterbi Error number of layer-C */
	UINT32 S_ERR_A ;		/* Post-Viterbi Error number of layer-A */
	UINT32 S_ERR_B ;		/* Post-Viterbi Error number of layer-B */
	UINT32 S_ERR_C ;		/* Post-Viterbi Error number of layer-C */
	UINT32 P_ERR_A ;		/* Packet Error number of layer-A */
	UINT32 P_ERR_B ;		/* Packet Error number of layer-B */
	UINT32 P_ERR_C ;		/* Packet Error number of layer-C */
	UINT32 CN_MON ;			/* C/N monitor value*/
	UINT32 CN_current ;     /* estimated C/N from C/N monitor */
	UINT32 MER_MON_A ;		/* MER monitor value of layer-A */
	UINT32 MER_MON_B ;		/* MER monitor value of layer-B */
	UINT32 MER_MON_C ;		/* MER monitor value of layer-C */
} MB86A2x_SIG_QUAL_INFO ;


typedef struct
{
	UINT8  CH_SCH_status ;		/* channel search status (first stage finished or not) */
	UINT8  CH_SCH_result ;		/* channel search result of fitst stage */ 
	UINT8  Detected_ch_num ;    /* the number of detected channel */
	UINT8  CH_num[70] ;			/* channel number for detected channel */
	UINT8  MODE[70] ;			/* channel search result (mode) */
	UINT8  GUARD[70] ;			/* channel search result (guard) */
	UINT8  MOD_A[70] ;			/* channel search result (layer-A modulation) */
	UINT8  MOD_B[70] ;			/* channel search result (layer-B modulation) */
	UINT8  MOD_C[70] ;			/* channel search result (layer-C modulation) */
	UINT8  RATE_A[70] ;			/* channel search result (layer-A code rate) */
	UINT8  RATE_B[70] ;			/* channel search result (layer-B code rate) */
	UINT8  RATE_C[70] ;			/* channel search result (layer-C code rate) */
	UINT8  IL_A[70] ;			/* channel search result (layer-A interleave depth) */
	UINT8  IL_B[70] ;			/* channel search result (layer-B interleave depth) */
	UINT8  IL_C[70] ;			/* channel search result (layer-C interleave depth) */
	UINT8  NUM_A[70] ;			/* channel search result (layer-A segment number) */
	UINT8  NUM_B[70] ;			/* channel search result (layer-B segment number) */
	UINT8  NUM_C[70] ;			/* channel search result (layer-C segment number) */
	UINT32 STR[70] ;
} MB86A2x_CH_SCH_INFO ;


#endif

