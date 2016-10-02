/* diseqcapi.h */
/********************************************************************\


\********************************************************************/

#include "Atbm781x.h"
#ifndef ATBM781x_DISEQC_API_H_DEFINED
#define ATBM781x_DISEQC_API_H_DEFINED

/* ================================== */
/* Definitions of commonly used types */
/* ================================== */

typedef int BOOL;

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#define ATBM781x_DISEQC_RTS_TRIALS 500 /* repetitions of  checking times if DiSEqC write is not finished */
#if 0
typedef enum
{
  ATBM781x_SUCCESSFUL = 0,         /* No error */
  ATBM781x_FAILED,                 /* General error */  
  ATBM781x_DISEQC_INITIALIZED,
  ATBM781x_DISEQC_NOT_INITIALIZED,
  ATBM781x_NO_ACKNOWLEDGE,         /* No acknowledge from I2C bus */
  ATBM781x_NO_SLAVE_ACK            /* No acknowledge from devices on the GPIO I2C bus */
} ATBM781x_STATUS; 
#endif
typedef enum						/* DiSEqC Modes: 0-5 */
{
	ATBM781x_DISEQC_ONLY = 0,
	ATBM781x_DISEQC_CONT_AND_BURST,			/* DiSEqC with continuous, tone burst and GPIO */
	ATBM781x_DISEQC_CONT,					/* DiSEqC with continuous tone and GPIO (no burst )*/
	ATBM781x_DISEQC_BURST,					/* DiSEqC with tone burst and GPIO (no continuous) */
	ATBM781x_CONT_TONE,						/* no DISEQC only 22KHz cont tone and GPIO port selection */
	ATBM781x_ONLY_GPIO						/* set GPIO pin only - for other purposes - no DiSEqC or SAT function */
}ATBM781x_DISEQC_MOD;

typedef enum
{
	ATBM781x_TONE_LEVEL_0DOT65V = 0,
	ATBM781x_TONE_LEVEL_0DOT75V ,
	ATBM781x_TONE_LEVEL_0DOT85V,
	ATBM781x_TONE_LEVEL_0DOT95V ,
	ATBM781x_TONE_LEVEL_1DOT05V,
	ATBM781x_TONE_LEVEL_1DOT15V ,
	ATBM781x_TONE_LEVEL_1DOT25V,
	ATBM781x_TONE_LEVEL_1DOT35V
}ATBM781x_DISEQC_TONE_LEVEL;


typedef enum
{
	ATBM781x_DISEQC_22KHZ = 0,
	ATBM781x_DISEQC_BIN,
	ATBM781x_DISEQC_BIN_INVERTED
}ATBM781x_DISEQC_MODULATION_MOD_E;

typedef struct ATBM781x_DISEQC_WRITE_PARAM
{
	uint8  NoOfBytesToSend;
	uint8  TransmitByte[8];
	uint16 delayBeforeDiseqc;		/* delay1 in ms before and in case of ToneBurst also behind DiSEqC */
	uint16 delayBeforeCont;		    /* delay2 in ms before cont tone starts again */
}ATBM781x_DISEQC_WRITE_PARAM;

typedef struct ATBM781x_DISEQC_OUT_PARAM
{
	ATBM781x_DISEQC_MOD diseqc_mod_e;		/* see description in ATBM781x_enums */
	BOOL contToneVal;				/* zero (22kHz off) selects low band, high (22kHz on) selects high band */
	BOOL toneBurstVal;				/* zero selects LNB a, high selects LNB b */
	BOOL polarizationPortVal;		/* GPIO Port 3 low-high switch */
}ATBM781x_DISEQC_OUT_PARAM;


typedef struct ATBM781x_DISEQC_READ_PARAM
 {
	 BOOL binaryModulation;			/* binary demodulated or 22kHz modulated */
	 BOOL readBinInvert;				/* only for V2: inverted binary input signal */
	 uint8 NoOfBytesToRead;
	 uint8 MagThreshold;
	 /* uint8 bitDuration; */
	 BOOL DirPinByDiseqc;			/* DISEQC Direction pin controlled by DiSEqC if TRUE */
	 BOOL DirPinPolarity;
}ATBM781x_DISEQC_READ_PARAM;				

typedef struct ATBM781x_DISEQC_PARAM
{
	ATBM781x_DISEQC_WRITE_PARAM write_params;			/*DiSEqC transmission params */
	ATBM781x_DISEQC_OUT_PARAM out_params;				/*analog signal specification */
	ATBM781x_DISEQC_READ_PARAM read_params;				/*DiSEqC read settings*/
	ATBM781x_DISEQC_MODULATION_MOD_E diseqcModulation_e;
	BOOL DiseqcInitialized;
}ATBM781x_DISEQC_PARAM;

typedef struct ATBM781x_DISEQC_REPLY_INFO
{
	BOOL parityError;
	uint8 receivedBytesCount;
	uint8 receivedBytes[8];	/* contains the DiSEqC read bytes */
}ATBM781x_DISEQC_REPLY_INFO;

typedef struct ATBM781x_DISEQC_PARAMS_INPUT
{
	uint8 ContinuousToneEnable;
	uint8 ToneBurstEnable;
	uint8 ToneSignal;	
	uint8 PolarityInverted;
	uint8 ToneBurstValue; /*Tone burst A,or Tone burst B*/
	uint8 ContinuousToneValue; /*0:off, 1:on*/
	uint32 SysClockHz;
	ATBM781x_DISEQC_WRITE_PARAM write_params;	
	ATBM781x_DISEQC_READ_PARAM read_params;				/*DiSEqC read settings*/
    ATBM781x_DISEQC_REPLY_INFO reply_info;
}ATBM781x_DISEQC_PARAMS_INPUT_T;


/****************************************************************************
Function:   ATBM781xSetLNBByGPIO
Parameters: u8LnbPinLevel, 1 Set LNB High, 0 Set LNB low.                  
Return:     ATBM781x_STATUS.
Description:
	The LNB voltage or polarity can be controlled by GPIO	
******************************************************************************/
ATBM781x_STATUS ATBM781xSetLNBByGPIO(uint8  u8LnbPinLevel);

/****************************************************************************
Function:   ATBM781xGetLNBStatusFromGPIO
Parameters: u8LnbPinLevel,  0 LNB low, 1 LNB High.               
Return:     ATBM781x_STATUS.
Description:
	The LNB voltage or polarity can be controlled by GPIO	
	at the same time, the GPIO state which indicating LNB status can 
	be read from register 	
******************************************************************************/
ATBM781x_STATUS ATBM781xGetLNBStatusFromGPIO(uint8 *pu8LnbPinLevel);

/****************************************************************************
Function:   ATBM781xSetContinuousTone
Parameters: ContTone22KOnOff, 0: 22KHz tone off,  1:22KHz tone on,            
Return:     ATBM781x_STATUS.
Description:
	Set 22K continuous tone on or off to control LNB or 22K switcher. 
	for example
	Stop  22KHz tone, call ATBM781xSetContinuousTone(0); 22K continuous tone stopped
	Start 22KHz tone, call ATBM781xSetContinuousTone(1); 22K continuous tone start
	attention: 22KHz Continuous tone is modulated signal, no Binary control signal.
******************************************************************************/
ATBM781x_STATUS ATBM781xSetContinuousTone(BOOL ContTone22KOnOff);

/****************************************************************************
Function:   ATBM781xSendToneBurst
Parameters: u8ToneBurstType, 0:Tone Burst'0'(usually satellite A), 1,Tone burst'1'(usually satellite B).                  
Return:     ATBM781x_STATUS.
Description:
	Send 22KHz tone burst, there are Tone Burst0 and Tone Burst1.
	Call ATBM781xSendToneBurst(0); Send one time tone Burst 0
    Call ATBM781xSendToneBurst(1); Send one time tone Burst 1	
******************************************************************************/
ATBM781x_STATUS ATBM781xSendToneBurst(BOOL u8ToneBurstType);

/****************************************************************************
Function:   ATBM781xSetToneLevel
Parameters: ToneLevel.                  
Return:     ATBM781x_STATUS.
Description:
	The Tone output level voltage controlled by ATBM781xSetToneLevel
	Default 0.65V,  some device need increase the voltage can call this function.
	Available selection:
	0.65V;0.75V;0.85V;0.95V;1.05V;1.15V;1.25V;1.35V
******************************************************************************/
ATBM781x_STATUS ATBM781xSetToneLevel(ATBM781x_DISEQC_TONE_LEVEL  ToneLevel);

/****************************************************************************
Function:   ATBM781xSendDiseqc
Parameters: pu8DiseqcTxBuffer, pointer to DiSEqC command Buffer in Byte.   
            u8TxBufferLength,  DiSEqC command length in Byte

Return:     ATBM781x_STATUS.
Description:
	Send DiSEqC command to slave device.
	Take care this function only send one way DiSEqC command, 
	if DiSEqC reply required please call ATBM781xSendDiseqcWithReply.
******************************************************************************/
ATBM781x_STATUS ATBM781xSendDiseqc(uint8 *pu8DiseqcTxBuffer,  uint8 u8TxBufferLength);

/****************************************************************************
Function:   ATBM781xSendDiseqcContTone
Parameters: pu8DiseqcTxBuffer, pointer to DiSEqC command Buffer in Byte.   
            u8TxBufferLength,  DiSEqC command length in Byte
            ConsToneValue,     Continuous tone on or off, 1:on, 0:off

Return:     ATBM781x_STATUS.
Description:
	Send DiSEqC command to slave device. then Send Continuous tone if ConsToneOnOff == 1
	After DiSEqC command send output continuous tone. the sequence is as following:
	
	--DiSEqC command---Continuous tone------

    if no Continuous Tone needed, just set Parameter ConsToneOnOff == 0 or call ATBM781xSendDiseqc    
   Take care this function send one way DiSEqC command and analog Tone, no reply
******************************************************************************/
ATBM781x_STATUS ATBM781xSendDiseqcContTone(uint8 *pu8DiseqcTxBuffer,  uint8 u8TxBufferLength, uint8 ConsToneValue);

/****************************************************************************
Function:   ATBM781xSendDiseqcBurstToneContTone
Parameters: pu8DiseqcTxBuffer, pointer to DiSEqC command Buffer in Byte.   
            u8TxBufferLength,  DiSEqC command length in Byte
            u8ToneBurstType,   Tone Burst Type, can be 0(Burst tone0) or 1(Burst tone1)
            ConsToneValue,     Continuous tone on or off, 1:on, 0:off

Return:     ATBM781x_STATUS.
Description:
	Send DiSEqC command to slave device.
	After DiSEqC command send one time Tone Burst and continuous tone. the sequence is as following:
	
	--DiSEqC command---Burst Tone---Continuous tone------

    if no Continuous Tone needed, just set Parameter ConsToneOnOff == 0
    if no Burst tone needed, please call ATBM781xSendDiseqcContTone
    if no continuous tone and  No burst tone needed, please call ATBM781xSendDiseqc

Take care this function send one way DiSEqC command and analog Tone, no reply
******************************************************************************/
ATBM781x_STATUS ATBM781xSendDiseqcBurstToneContTone(uint8*pu8DiseqcTxBuffer, uint8 u8TxBufferLength, uint8 u8ToneBurstType,  uint8 ConsToneValue);



ATBM781x_STATUS ATBM781xSendDiseqcWithReply(uint8*pu8DiseqcTxBuffer, uint8 u8TxBufferLength,  uint8*pu8DiseqcReplyBuffer, uint8* pu8DiseqcReplyLength);

/****************************************************************************
Function:   ATBM781xSetBinaryMode
Parameters: u8BinaryInverted, 1 Binary inverted, 0 not inverted.                  
Return:     ATBM781x_STATUS.Description:
	This function set DiSEqC binary inverted mode.
	Take care, only valid when Binary mode is used, 
	This function should not be used when tone mode is used.
******************************************************************************/
ATBM781x_STATUS ATBM781xSetBinaryMode(uint8 u8BinaryInverted);







#endif  /* #ifndef ATBM781x_DISEQC_API_H_DEFINED */
