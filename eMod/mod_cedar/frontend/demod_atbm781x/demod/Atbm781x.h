

#ifndef ATBM781X_H
#define ATBM781X_H

#include "mod_cedar_i.h"
/**********************SDK Redefine Data Type ********************************/
/*If conflict, please comment or replace the name*/
typedef unsigned char  uint8;
typedef unsigned short uint16;
typedef unsigned int   uint32;

typedef char			int8;
typedef short           int16;                        
typedef int             int32;

typedef unsigned char ATBM781x_STATUS;

/**********************debugPrintf Define ********************************/
#define  ATBM781xDEBUGPRINTENABLE     1
#if ATBM781xDEBUGPRINTENABLE
#define ATBM781xDebugPrint   WARNING//printf
#else
#define ATBM781xDebugPrint   
#endif



/**********************Next is define the T and T2 Bandwidth Macro*******************************/
#define  ATBM781x_DVBT_BANDWIDTH_1DOT7MHZ     0/* only support for T2 */
#define  ATBM781x_DVBT_BANDWIDTH_5MHZ         1
#define  ATBM781x_DVBT_BANDWIDTH_6MHZ         2
#define  ATBM781x_DVBT_BANDWIDTH_7MHZ         3
#define  ATBM781x_DVBT_BANDWIDTH_8MHZ         4
#define  ATBM781x_DVBT_BANDWIDTH_10MHZ        5

/**********************Chip Support Mode *******************************************************/
#define ATBM781x_DVBC_MODE                 0    
#define ATBM781x_RSV_MODE                  1     /*Reserved*/
#define ATBM781x_DVBT_MODE                 2
#define ATBM781x_DVBT2_MODE                3  
#define ATBM781x_DVBS_MODE                 4  
#define ATBM781x_DVBS2_MODE                5  
#define ATBM781x_DVB_UNKNOWN               255
/***********************USB Port Read/Write Return Status ****************************************/
#define ATBM781x_SUCCESSFUL                0     /* No error */
#define	ATBM781x_FAILED                    1     /* General error, usually ATBM781x APIs not correctly used or DTV system error */  
#define	ATBM781x_I2C_ERROR                 2     /*General I2C error,usually from I2C interface */
#define	ATBM781x_NO_ACKNOWLEDGE            4     /* No acknowledge from I2C bus */
#define	ATBM781x_NO_SLAVE_ACK              8     /* No acknowledge from devices on the GPIO I2C bus */


/********************************************************************************************************************************** 
struct ATBM781x_MPEGTSMode 
@ui8TSTransferType: TS stream transfer type, can be set to parallel(8 bit data bus) or serial(1 bit data bus) mode
@ui8OutputEdge: demodulator will output TS data on this edge of TS stream clock
@ui8TSSPIMSBSelection: TS MSB on bit 0 or bit selection
@ui8TSSSIOutputSelection: TS output on bit 0 or bit 7 selection
@ui8SPIClockConstantOutput: TS stream clock can be set outputting all the time or only during TS data valid (188 bytes) 
**********************************************************************************************************************************/
/*****************ui8TSTransferType Option Value***************************/
#define ATBM781x_TS_PARALLEL_MODE             1
#define ATBM781x_TS_SERIAL_MODE               0
/**********************ui8OutputEdge Option Value***************************/
#define ATBM781x_TS_OUTPUT_FALLING_EDGE       1
#define ATBM781x_TS_OUTPUT_RISING_EDGE        0
/**********************ui8TSSPIMSBSelection Option Value******************/
#define ATBM781x_TS_SPI_MSB_ON_DATA_BIT7      1
#define ATBM781x_TS_SPI_MSB_ON_DATA_BIT0      0
/**********************ui8TSSSIOutputSelection Option Value***************/
#define ATBM781x_TS_SSI_OUTPUT_ON_DATA_BIT7   1
#define ATBM781x_TS_SSI_OUTPUT_ON_DATA_BIT0   0
/**********************ui8SPIClockConstantOutput Option Value*************/
#define ATBM781x_TS_CLOCK_CONST_OUTPUT        1
#define ATBM781x_TS_CLOCK_VALID_OUTPUT        0	
typedef struct ATBM781x_MPEG_TS_MODE_STRUCT 
{
	uint8 ui8TSTransferType;
	uint8 ui8OutputEdge; 
	uint8 ui8TSSPIMSBSelection;
	uint8 ui8TSSSIOutputSelection;
	uint8 ui8SPIClockConstantOutput; 		
}ATBM781x_MPEGTSMode;


#define   ATBM781x_INPUT_IQ_SWAP           1
#define   ATBM781x_INPUT_IQ_NORMAL        0

#define  ATBM781x_IQ_AUTO_ENABLE 1
#define  ATBM781x_IQ_AUTO_DISABLE 0
typedef enum
{
	ATBM781x_PIN_IF_AGC_T = 0,
	ATBM781x_PIN_IF_AGC_S,
	ATBM781x_PIN_RF_AGC_T,
	ATBM781x_PIN_GPO_POLARITY,
	ATBM781x_PIN_TS_LOCK,
	ATBM781x_PIN_TS_ERROR,
	ATBM781x_PIN_FEC_LOCK,
	ATBM781x_PIN_FEC_ERROR,
	ATBM781x_PIN_AGC_FREEZE,
	ATBM781x_PIN_RESERVED
}ATBM781x_GPIO_PIN_TYPE;

typedef struct ATBM781x_DVBT_TUNER_CONFIG_STRUCT
{
	uint8   ui8IQSwapMode;	
	uint8   ui8BandwidthType; /*eg.ATBM781x_DVBT_BANDWIDTH_8MHZ*/
	/*uint8   ui8BandwidthMHz;//  5,6,7,8*/
	uint32 u32IFFrequencyKHz;  /*Tuner IF frequency for DVB-T,DVB-T2 and DVB-C, Unit is KHz*/
}ATBM781x_DVBT_TUNER_INPUT_T;

typedef struct DVBC_TUNER_CONFIG_STRUCT
{
	uint8  ui8IQSwapMode;	
	uint32 u32IFFrequencyKHz;
	uint32 u32SymbolRateK; /*Unit is K, DVB-C symbol rate auto detection. this is legacy code.*/
}ATBM781x_DVBC_TUNER_INPUT_T;

typedef struct DVBS_TUNER_CONFIG_STRUCT
{
	uint8   ui8IQSwapMode;	
	uint8   ui8IQAutoDetection;
	uint32  ui32SymbolRateK; /*DVB-S/S2 symbol rate, actually not used. auto detection.*/
}ATBM781x_DVBS_TUNER_INPUT_T;


typedef struct ATBM781x_PIN_CONFIG_STRUCT
{
	ATBM781x_GPIO_PIN_TYPE GPOPinStatus;    /*0x0023 bit[7:4]  */
	ATBM781x_GPIO_PIN_TYPE GPO1PinAgcDvbS;   /*0x0021 bit[3:0] = 3*/
	ATBM781x_GPIO_PIN_TYPE GPO2PinRFAgcDvbT; /*0x0021 bit[7:4] //Not used set 0*/
	ATBM781x_GPIO_PIN_TYPE GPO3PinAgcDvbT;   /*0x0022 bit[3:0] = 1*/
	ATBM781x_GPIO_PIN_TYPE GPO4PinStatus;   /*0x0037 bit[7:4]:  RegGPO7 */
	ATBM781x_GPIO_PIN_TYPE GPO5PinStatus;   /*0x0037 bit[3:0]   RegGPO6*/
}ATBM781x_PIN_CONFIG_T;


typedef struct ATBM781x_DVB_CONFIG_STRUCT
{
	/*Common setting*/
	ATBM781x_MPEGTSMode stMPEGTSMode;      /*TS interface configuration	*/
	uint32 ui32DemodClkKHz;/*Demodulator clock Select Unit is KHz*/

	ATBM781x_PIN_CONFIG_T GpioPinConfigs;

	/*Set DVB-T(T2),  DVB-C, DVB-S, DVB-S2 mode*/
	uint8  ui8ConfigDVBType;    /*eg. ATBM781x_DVBT_MODE(contains auto detection of DVB-T2), ATBM781x_DVBC_MODE*/

	/*DVB-T,T2 setting*/
	ATBM781x_DVBT_TUNER_INPUT_T DvbTxTunerInput; /*DVB-T and DVB-T2	*/

	/*DVB-S/S2 setting*/
	ATBM781x_DVBS_TUNER_INPUT_T DvbSxTunerInput;
	uint32 ui32DVBSSysClkKHz; /*for DVB-S/S2 System clock,  all unit is KHz*/


	/*DVB-C setting*/
	ATBM781x_DVBC_TUNER_INPUT_T DvbCTunerInput;
	uint8              ui8DualDemod;
}stATBM781xDvbConfig;


#define ATBM781x_DVBT2_MAX_PLP_NUMBER         255     /* DVBT2 PLP maximum number*/

/* DVB-T FFT mode*/
typedef enum {
	ATBM781x_DVBT_MODE_2K = 0,                      
	ATBM781x_DVBT_MODE_8K,   
	ATBM781x_DVBT_MODE_RESERVED3,
	ATBM781x_DVBT_MODE_UNKNOWN          
}ATBM781x_DVBT_FFT_MODE_TYPE;

/* DVB-T guard interval*/
typedef enum{
	ATBM781x_DVBT_GI_1_32 = 0,                  
	ATBM781x_DVBT_GI_1_16,                    
	ATBM781x_DVBT_GI_1_8,                     
	ATBM781x_DVBT_GI_1_4,    
	ATBM781x_DVBT_GI_UNKNOWN 
}ATBM781x_DVBT_GUARD_INTERVAL_TYPE;

/*DVB-T constellation*/
typedef enum{
	ATBM781x_DVBT_CONSTELLATION_QPSK = 0,           
	ATBM781x_DVBT_CONSTELLATION_16QAM,          
	ATBM781x_DVBT_CONSTELLATION_64QAM,           
	ATBM781x_DVBT_CONSTELLATION_UNKNOWN
}ATBM781x_DVBT_CONSTELLATION_TYPE;

/*DVB-T code rate*/
typedef enum{
	ATBM781x_DVBT_CODE_RATE_1_2 = 0,                 
	ATBM781x_DVBT_CODE_RATE_2_3,                
	ATBM781x_DVBT_CODE_RATE_3_4,               
	ATBM781x_DVBT_CODE_RATE_5_6,                
	ATBM781x_DVBT_CODE_RATE_7_8,          
	ATBM781x_DVBT_CODE_RATE_RESERVED5,
	ATBM781x_DVBT_CODE_RATE_RESERVED6,
	ATBM781x_DVBT_CODERATE_UNKNOWN        
}ATBM781x_DVBT_CODE_RATE_TYPE;

/*DVB-T hierarchy*/
typedef enum{
	ATBM781x_DVBT_HIERARCHY_NON = 0,   /*Non-hierarchical*/
	ATBM781x_DVBT_HIERARCHY_1,                 
	ATBM781x_DVBT_HIERARCHY_2,                   
	ATBM781x_DVBT_HIERARCHY_4,  
	ATBM781x_DVBT_HIERARCHY_UNKNOWN
}ATBM781x_DVBT_HIERARCHY_TYPE;

/* DVB-T HP, LP profile*/
typedef enum{
	ATBM781x_DVBT_HP_PROFILE = 0,              
	ATBM781x_DVBT_LP_PROFILE,
	ATBM781x_DVBT_PROFILE_UNKNOWN
}ATBM781x_DVBT_PROFILE_TYPE;



/***********************************************************/
/*                 DVB-T2 parameters                                                          */
/************************************************************/
/* DVB-T2 FFT mode*/
typedef enum{
	ATBM781x_DVBT2_MODE_1K = 0, 
	ATBM781x_DVBT2_MODE_2K,                      
	ATBM781x_DVBT2_MODE_4K,  
	ATBM781x_DVBT2_MODE_8K, 
	ATBM781x_DVBT2_MODE_16K, 
	ATBM781x_DVBT2_MODE_32K, 
	ATBM781x_DVBT2_MODE_RESERVED, 
	ATBM781x_DVBT2_MODE_UNKNOWN          
}ATBM781x_DVBT2_FFT_MODE_TYPE;


typedef enum
{
	ATBM781x_DVBT2_GI_1_32 = 0,
	ATBM781x_DVBT2_GI_1_16,
	ATBM781x_DVBT2_GI_1_8,
	ATBM781x_DVBT2_GI_1_4,
	ATBM781x_DVBT2_GI_1_128,
	ATBM781x_DVBT2_GI_19_128,
	ATBM781x_DVBT2_GI_19_256,
	ATBM781x_DVBT2_GI_UNKNOWN
}ATBM781x_DVBT2_GUARD_INTERVAL_TYPE;


typedef enum {
	ATBM781x_DVBT2_BASE_PROFILE = 0,       
	ATBM781x_DVBT2_LITE_PROFILE,       
	ATBM781x_DVBT2_BASE_LITE_UNKNOWN
} ATBM781x_DVBT2_BASE_OR_LITE_TYPE;

typedef enum {
	ATBM781x_DVBT2_V111 = 0,       
	ATBM781x_DVBT2_V121,       
	ATBM781x_DVBT2_V131,
	ATBM781x_DVBT2_VERSION_UNKNOWN
} ATBM781x_DVBT2_VERSION_TYPE;


/* DVBT2 S1 signaling definition */
typedef enum {
	ATBM781x_DVBT2_SISO = 0,                       
	ATBM781x_DVBT2_MISO,        
	ATBM781x_DVBT2_NON_DVBT2,                         /* Non DVBT2 */
	ATBM781x_DVBT2_LITE_SISO,                      /* Reserved */
	ATBM781x_DVBT2_LITE_MISO,                          
	ATBM781x_DVBT2_S1_RESERVED5,                            
	ATBM781x_DVBT2_S1_RESERVED6,                            
	ATBM781x_DVBT2_SISO_MISO_UNKNOWN                            

} ATBM781x_DVBT2_SISO_MISO_TYPE;


typedef enum {
	ATBM781x_DVBT2_BANDWIDTH_NO_EXT= 0,                       
	ATBM781x_DVBT2_BANDWIDTH_EXTENDED,                             
	ATBM781x_DVBT2_BANDWIDTH_EXT_UNKNOWN
} ATBM781x_DVBT2_BANDWIDTH_EXT_TYPE;

typedef enum 
{
	ATBM781x_DVBT2_QPSK = 0,
	ATBM781x_DVBT2_QAM_16,
	ATBM781x_DVBT2_QAM_64,
	ATBM781x_DVBT2_QAM_256,
	ATBM781x_DVBT2_QAM_RESERVED4,                        
	ATBM781x_DVBT2_QAM_RESERVED5,                          
	ATBM781x_DVBT2_QAM_RESERVED6,                       
	ATBM781x_DVBT2_QAM_RESERVED7,                           
	ATBM781x_DVBT2_QAM_UNKNOWN
}ATBM781x_DVBT2_CONSTELLATION_TYPE;



typedef enum
{
	ATBM781x_DVBT2_CODE_RATE_1_2 = 0,
	ATBM781x_DVBT2_CODE_RATE_3_5,
	ATBM781x_DVBT2_CODE_RATE_2_3,
	ATBM781x_DVBT2_CODE_RATE_3_4,
	ATBM781x_DVBT2_CODE_RATE_4_5,
	ATBM781x_DVBT2_CODE_RATE_5_6,
	ATBM781x_DVBT2_CODE_RATE_1_3, /*for t2 lite*/
	ATBM781x_DVBT2_CODE_RATE_2_5, /*for t2 lite*/
	ATBM781x_DVBT2_CODE_RATE_UNKNOWN
}ATBM781x_DVBT2_CODE_RATE_TYPE;


typedef enum 
{
	ATBM781x_DVBT2_PP1 =0,
	ATBM781x_DVBT2_PP2,
	ATBM781x_DVBT2_PP3,
	ATBM781x_DVBT2_PP4,
	ATBM781x_DVBT2_PP5,
	ATBM781x_DVBT2_PP6,
	ATBM781x_DVBT2_PP7,
	ATBM781x_DVBT2_PP8,
	ATBM781x_DVBT2_PP_RESERVED8,       /* Reserved */
	ATBM781x_DVBT2_PP_RESERVED9,                           
	ATBM781x_DVBT2_PP_RESERVED10,                           
	ATBM781x_DVBT2_PP_RESERVED11,                           
	ATBM781x_DVBT2_PP_RESERVED12,                            
	ATBM781x_DVBT2_PP_RESERVED13,                          
	ATBM781x_DVBT2_PP_RESERVED14,                            
	ATBM781x_DVBT2_PP_RESERVED15,      /* Reserved */
	ATBM781x_DVBT2_PP_UNKNOWN
}ATBM781x_DVBT2_PILOT_PATTERN_TYPE;

typedef enum 
{
	ATBM781x_DVBT2_PAYLOAD_GFPS =0,
	ATBM781x_DVBT2_PAYLOAD_GCS,
	ATBM781x_DVBT2_PAYLOAD_GSE,
	ATBM781x_DVBT2_PAYLOAD_TS,
	ATBM781x_DVBT2_PAYLOAD_UNKNOWN
}ATBM781x_DVBT2_PLP_PAYLOAD_TYPE;

typedef enum {
	ATBM781x_DVBT2_COMMON_PLP = 0,                       /* DVBT2 PLP Common Type */
	ATBM781x_DVBT2_DATA_PLP_TYPE1,                       /* DVBT2 PLP Data Type 1 */
	ATBM781x_DVBT2_DATA_PLP_TYPE2,                       /* DVBT2 PLP Data Type 2 */
	ATBM781x_DVBT2_PLP_TYPE_RSVD3,                       /*Reserved */
	ATBM781x_DVBT2_PLP_TYPE_RSVD4,                       
	ATBM781x_DVBT2_PLP_TYPE_RSVD5,                       
	ATBM781x_DVBT2_PLP_TYPE_RSVD6,                       
	ATBM781x_DVBT2_PLP_TYPE_RSVD7,                       /* Reserved */
	ATBM781x_DVBT2_PLP_TYPE_UNKNOWN                  
} ATBM781x_DVBT2_PLP_TYPE;

typedef enum
{
	ATBM781x_DVBT2_ISSY_NO = 0,
	ATBM781x_DVBT2_ISSY_SHORT,
	ATBM781x_DVBT2_ISSY_LONG,
	ATBM781x_DVBT2_ISSY_UNKNOWN
}ATBM781x_DVBT2_ISSY_TYPE;

typedef enum
{
	ATBM781x_DVBT2_LDPC_SHORT =0,
	ATBM781x_DVBT2_LDPC_NORMAL,
	ATBM781x_DVBT2_LDPC_RESERVED,
	ATBM781x_DVBT2_LDPC_LENGTH_UNKNOWN
}ATBM781x_DVBT2_FEC_LENGTH_TYPE;


typedef enum
{
	ATBM781x_DVBT2_NORMAL_MODE =0,
	ATBM781x_DVBT2_HEM_MODE,
	ATBM781x_DVBT2_STREAM_MODE_UNKNOWN
}ATBM781x_DVBT2_STREAM_MODE_TYPE;


typedef struct ATBM781x_DVBT_TPS_STRUCT
{
	ATBM781x_DVBT_FFT_MODE_TYPE       DvbtFFTMode;
	ATBM781x_DVBT_GUARD_INTERVAL_TYPE DvbtGuardInterval;
	ATBM781x_DVBT_CONSTELLATION_TYPE  DvbtConstellation;
	ATBM781x_DVBT_CODE_RATE_TYPE       DvbtHpCodeRate;
	ATBM781x_DVBT_CODE_RATE_TYPE       DvbtLpCodeRate;
	ATBM781x_DVBT_HIERARCHY_TYPE      DvbtHierarchy;
	ATBM781x_DVBT_PROFILE_TYPE        DvbtProfile;
}ATBM781x_DVBT_PARAMS;

typedef struct ATBM781x_DVBT2_PARAMS_STRUCT
{
	/*DVB-T2 Transmit params */
	ATBM781x_DVBT2_FFT_MODE_TYPE            Dvbt2FFTMode;
	ATBM781x_DVBT2_BANDWIDTH_EXT_TYPE  Dvbt2BandwidthExt;
	ATBM781x_DVBT2_GUARD_INTERVAL_TYPE    Dvbt2GuardInterval;
	ATBM781x_DVBT2_PILOT_PATTERN_TYPE       Dvbt2PilotPattern;
	ATBM781x_DVBT2_SISO_MISO_TYPE              Dvbt2SisoMiso;
	ATBM781x_DVBT2_VERSION_TYPE                  Dvbt2Version;
	ATBM781x_DVBT2_BASE_OR_LITE_TYPE             Dvbt2BaseOrLiteType;
	uint16                       Dvbt2CellId;
	uint16                       Dvbt2NetworkId;
	uint16                       Dvbt2T2SystemId;

	uint8 u8FefType;
	uint32 u32FefLength;  
	uint8   u8FefInterval;


	/*
	DVB-T2 PLP Parameters

	For single PLP T2 mode:
	u8DataPlpNumber == 1, PLP Index =0.
	the PLP_ID is default set by demodulator, usually used like DVB-T.

	For multi-PLP application:
	u8DataPlpNumber is the data PLP number. the maximum value is 255.
	PLP index is the index of PLP ID list start from 0.

	u8DataPlpIdArray contains all Data PLP ID.  common PLP is not listed in, as common plp is processed by demodulator internally
	*/	
	uint8 u8DataPlpNumber;
	uint8 u8DataPlpIdArray[255];

	/*
	When Single PLP used, DVB-T2  demodulator output  single PLP.
	When Multi-PLP used, DVB-T2 demodulator output only one active data PLP 
	u8ActivePlpIdis the current received Data PLP's PLP ID
	*/
	uint8                                           u8ActivePlpId;
	ATBM781x_DVBT2_PLP_TYPE                         Dvbt2DataPLPType;  /*Data PLP Type1 or Data PLP  Type2*/
	ATBM781x_DVBT2_PLP_PAYLOAD_TYPE          Dvbt2PayloadType;	
	ATBM781x_DVBT2_CONSTELLATION_TYPE       Dvbt2Constellation;
	ATBM781x_DVBT2_CODE_RATE_TYPE           Dvbt2CodeRate;
	uint8                          u8ConstellationRotation;	
	ATBM781x_DVBT2_FEC_LENGTH_TYPE         Dvbt2FecLengthType;
	ATBM781x_DVBT2_ISSY_TYPE                   Dvbt2IssyType;
	ATBM781x_DVBT2_STREAM_MODE_TYPE     Dvbt2StreamType;
}ATBM781x_DVBT2_PARAMS;

typedef struct ATBM781x_DVBT_T2_SIGNAL_PARAMS_STRUCT
{  
	uint8 DVBTxType;
	ATBM781x_DVBT_PARAMS         DvbtSignalParams;
	ATBM781x_DVBT2_PARAMS       Dvbt2SignalParams;
}ATBM781x_DVB_T_T2_SIGNAL_PARAMS;


/*DVBS, DVB-S2*/
typedef enum 
{
	ATBM781x_DVBSx_MODU_QPSK = 0,		
	ATBM781x_DVBSx_MODU_8PSK,		
	ATBM781x_DVBSx_MODU_16APSK,	
	ATBM781x_DVBSx_MODU_32APSK,	
	ATBM781x_DVBSx_MODU_UNKNOWN		
}ATBM781x_DVBS_S2_CONSTELLATION_TYPE;

typedef enum
{
	ATBM781x_DVBS_CODE_RATE_1_2 = 0,		/*Code rate 1/2*/
	ATBM781x_DVBS_CODE_RATE_2_3 = 1,		/*Code rate 2/3*/
	ATBM781x_DVBS_CODE_RATE_3_4 = 2,		/*Code rate 3/4*/
	ATBM781x_DVBS_CODE_RATE_5_6 = 3,		/*Code rate 5/6*/
	ATBM781x_DVBS_CODE_RATE_7_8 = 4,		/*Code rate 7/8*/
	ATBM781x_DVBS_CODE_RATE_UNKNOWN
}ATBM781x_DVBS_CODE_RATE_TYPE;

typedef enum
{
	ATBM781x_DVBS2_CODE_RATE_DUMMY = 0,
	ATBM781x_DVBS2_CODE_RATE_1_4 = 1,		
	ATBM781x_DVBS2_CODE_RATE_1_3 = 2,		
	ATBM781x_DVBS2_CODE_RATE_2_5 = 3,		
	ATBM781x_DVBS2_CODE_RATE_1_2 = 4,		
	ATBM781x_DVBS2_CODE_RATE_3_5 = 5,		
	ATBM781x_DVBS2_CODE_RATE_2_3 = 6,		
	ATBM781x_DVBS2_CODE_RATE_3_4 = 7,		
	ATBM781x_DVBS2_CODE_RATE_4_5 = 8,		
	ATBM781x_DVBS2_CODE_RATE_5_6 = 9,		
	ATBM781x_DVBS2_CODE_RATE_8_9 = 10,		
	ATBM781x_DVBS2_CODE_RATE_9_10 = 11,
	ATBM781x_DVBS2_CODE_RATE_UNKNOWN
}ATBM781x_DVBS2_CODE_RATE_TYPE;

typedef enum
{
	ATBM781x_DVBSx_SIGNAL_SPECTRUM_NORMAL = 0,	
	ATBM781x_DVBSx_SIGNAL_SPECTRUM_INVERSE = 1,
    ATBM781x_DVBSx_SIGNAL_SPECTRUM_UNKNOWN
}ATBM781x_DVBSx_SIGNAL_SPECTRUM;

typedef struct ATBM781x_DVBSx_SIGNAL_PARAMS_STRUCT
{
	ATBM781x_DVBS_S2_CONSTELLATION_TYPE DVBSxModu; /*Constellation*/
	uint8 DVBSxType;                        /*ATBM781x_DVBS_MODE or ATBM781x_DVBS2_MODE*/
	ATBM781x_DVBS_CODE_RATE_TYPE             DVBSCodeRate;
	ATBM781x_DVBS2_CODE_RATE_TYPE           DVBS2CodeRate; 
	uint32                                         DVBSxSymbolRateK;
}ATBM781x_DVBSx_SIGNAL_PARAMS;
typedef struct st64Data
{
	uint32 ui32Low,ui32High;
}ATBM781x64Data;

/**/
ATBM781x_STATUS   ATBM781xGetSignalParamsT(ATBM781x_DVBT_PARAMS *pDvbTParams);
ATBM781x_STATUS   ATBM781xPrintSignalParamsT(ATBM781x_DVBT_PARAMS*pDvbTParams );
ATBM781x_STATUS  ATBM781xGetDVBTxSigalStrength(uint8 *ui8TxSSI);/*only for 2176 specific*/


ATBM781x_STATUS   ATBM781xGetSignalParamsT2(ATBM781x_DVBT2_PARAMS *pDvbT2Params);
ATBM781x_STATUS   ATBM781xPrintSignalParamsT2(ATBM781x_DVBT2_PARAMS *pDvbT2Params);

ATBM781x_STATUS   ATBM781xLockFlagTAndT2(uint8 *LockStatus);


#if 0
/****************************function for gui,in the file MerlinSDK.c ***************************/
uint8         ATBM781xRead(uint8 ui8BaseAddr, uint8 ui8RegisterAddr);        /*For GUI Version test*/
uint32        ATBM781xDVBSRead(uint16 ui16Addr);                              /*For GUI Version*/
uint8         ATBM781xGetRegSize(uint8 ui8BitWidth); 	/*used for gui*/
/*void          ATBM781xGetRegInfor(uint32 ui32ReadSize,uint8 ui8BaseStart,uint8 ui8SubStart,uint8* ui8PRegWd,uint32 *ui32PReturnValue,CString *pstr);//used for gui */
uint8         ATBM781xGetPLPInfor(uint8 *ui8PAPLPValue);
#endif


/*The following three functions should be used only in DVB-Tx Auto or manual scanning.*/
ATBM781x_STATUS ATBM781xSetDVBT2DisablePLPDecode(void);  
ATBM781x_STATUS ATBM781xSetDVBT2EnablePLPDecode(void);
ATBM781x_STATUS ATBM781xSetDVBT2BaseAndLiteProfile(void);   /*DVB-T2 Base and Lite auto detection, random receiving*/

/****************************DVB-T and DVB-T2  Set functions ***************************/
ATBM781x_STATUS  ATBM781xSetDVBTxMode(uint32   u32bandwidthKHz); /*Common API for DVB-T and DVB-T2*/
ATBM781x_STATUS   ATBM781xSetFreqAndRateRatio(stATBM781xDvbConfig stPara); /*Internal function*/
ATBM781x_STATUS   ATBM781xSetBandWidthPara(stATBM781xDvbConfig stPara); /*Internal function*/
ATBM781x_STATUS   ATBM781xSetADC(uint8 ui8Adc);
ATBM781x_STATUS   ATBM781xSwapIQ(uint8 ui8IQ);

ATBM781x_STATUS ATBM781xSetDVBT2BaseProfile(void);                 /*Only receive DVB-T2 Base */
ATBM781x_STATUS ATBM781xSetDVBT2LiteProfile(void);                    /*Only receive DVB-T2 Lite */

ATBM781x_STATUS ATBM781xSetPLPForMultiPLPDVBT2(uint8 u8PLPid );
ATBM781x_STATUS ATBM781xSetAutoPLPT2(void);

ATBM781x_STATUS ATBM781xSetHierarchyLpT(void ); /*DVB-T Low profile*/
ATBM781x_STATUS ATBM781xSetDefaultHierarchyHpT(void);	  /*DVB-T high profile*/

/*DVBTx API functions can be used for DVB-T and DVB-T2*/
ATBM781x_STATUS ATBM781xDVBTxSigalParamsLockCheck(uint8 *pu8LockStatus);
ATBM781x_STATUS ATBM781xDVBTxModeDetected(uint8 *pu8DVBTxMode);
ATBM781x_STATUS ATBM781xDVBTxChannelLockCheck(uint8 *pu8LockStatus);
ATBM781x_STATUS ATBM781xDVBTxChannelLockCheckForManual(uint8 *LockStatus);

ATBM781x_STATUS ATBM781xTSLockFlag(uint8 *LockStatus);  //TS lock flag 
ATBM781x_STATUS ATBM781xFECLockFlag(uint8 *LockStatus); //FEC lock flag

ATBM781x_STATUS ATBM781xDVBTxSignalIFAGC(uint16 *ui16CalcValue);
ATBM781x_STATUS ATBM781xDVBTxSignalQuality(uint8 *pu8SQI);
ATBM781x_STATUS ATBM781xGetDVBTxPER(uint32 *pu32PERx10E9); /*PER is error rate after FEC bit corrected.*/
ATBM781x_STATUS ATBM781xGetDVBTxBER(uint32 *pu32BERx10E9); /*BER is error rate before FEC bit corrected.*/
ATBM781x_STATUS ATBM781xGetDVBTxSNR(uint32 *pu32SNRx10);	
ATBM781x_STATUS ATBM781xGetDVBTxCarrierOffset(int *pi32Value)	;
ATBM781x_STATUS ATBM781xGetDVBTxSampleOffsetPPM(int*pi32Value);

/*DVB-T2 Get status functions*/
ATBM781x_STATUS   ATBM781xGetDVBT2SignalQuality(uint8 *ui8T2SQ);
ATBM781x_STATUS   ATBM781xGetDVBT2SampleOffsetPPM(int*i32Value);
ATBM781x_STATUS   ATBM781xGetDVBT2CFO(int*i32Value);
ATBM781x_STATUS   ATBM781xGetDVBT2SNR(uint32 *ui32SNRValue);
ATBM781x_STATUS   ATBM781xGetDVBT2BER(uint32 *ui32T2BER);
ATBM781x_STATUS   ATBM781xDVBT2ThreadProcess(void); 
ATBM781x_STATUS  ATBM781xGetDVBT2PostBCHBlockER(uint32 *ui32T2BlockER);
/*DVB-T get Status Functions*/
ATBM781x_STATUS   ATBM781xGetDVBTSigalQuality(uint8 *ui8TSQ);
ATBM781x_STATUS   ATBM781xGetDVBTSampleOffsetPPM(int*i32Value);
ATBM781x_STATUS   ATBM781xGetDVBTCFO(int*i32Value);
ATBM781x_STATUS   ATBM781xGetDVBTSNR(uint32 *ui32SNRValue);
ATBM781x_STATUS   ATBM781xGetDVBTBER(uint32 *ui32BER);
ATBM781x_STATUS   ATBM781xGetDVBTPER(uint32 *ui32PER);

/****************************DVB-C functions************************************/
ATBM781x_STATUS   ATBM781xSetDVBCMode(void );
ATBM781x_STATUS   ATBM781xDVBCSet( stATBM781xDvbConfig stPara);
ATBM781x_STATUS   ATBM781xDVBCSetQAM(void);
ATBM781x_STATUS   ATBM781xDVBCSetCarrier(uint32 ui32OSCFreq, uint32 ui32SymbolRateK);
ATBM781x_STATUS   ATBM781xDVBCSetSymbolRate(uint32 ui32OSCFreq, uint32 ui32SymbolRateK);
/*DVB-C get status Functions,*/
/*ATBM781x_STATUS   ATBM781xTSLockFlag(uint8 *LockStatus);*/
ATBM781x_STATUS   ATBM781xDVBCChannelLockCheck(uint8 *LockStatus);
ATBM781x_STATUS   ATBM781xGetDVBCSymbolRate(uint32 *ui32SymRate);
ATBM781x_STATUS   ATBM781xGetDVBCCarrierOffset(int *i32COValue);
ATBM781x_STATUS   ATBM781xGetDVBCQAM(uint8 *ui8QAMIndex);
ATBM781x_STATUS   ATBM781xGetDVBCSNR(int *i32SNR);
//ATBM781x_STATUS   ATBM781xGetDVBCSignalStrength(int *i32SigStr);
ATBM781x_STATUS   ATBM781xGetDVBCSignalQuality(uint8 *ui8DvbcSQI);
ATBM781x_STATUS   ATBM781xGetDVBCBER(uint32 *ui32BER);
ATBM781x_STATUS   ATBM781xGetDVBCPER(uint32 *ui32PER);




/****************************DVB-S, DVB-S2 Functions******************************/

ATBM781x_STATUS  ATBM781xSetDvbSMode(void);  /*DVB-S only*/
ATBM781x_STATUS  ATBM781xSetDvbS2Mode(void); /*DVB-S2 only*/
ATBM781x_STATUS  ATBM781xSetDvbSxAutoMode(void );  /*Auto detection*/
ATBM781x_STATUS  ATBM781xSetDvbSxMode(void);  /*Set DVB-S/Sx auto mode with all registers, called by S/S2 mode set functions.*/
ATBM781x_STATUS  ATBM781xSetDvbSxSpectrumAuto(void);  //Used after demodulator set done and tuner tuned.
ATBM781x_STATUS  ATBM781xCalibrateDVBSxSymbolRate(void);

ATBM781x_STATUS  ATBM781xSetDvbSxSpectrumManual(ATBM781x_DVBSx_SIGNAL_SPECTRUM  SignalSpectrum);
ATBM781x_STATUS  ATBM781xDVBSxChannelLockCheckForAR(uint8 *pu8DvbSxLockStatus); /*FAE used function*/
/*ATBM781x_STATUS   ATBM781xTSLockFlag(uint8 *LockStatus);*/
ATBM781x_STATUS   ATBM781xDVBSxModeDetected(uint8 *pu8DVBSxMode);
ATBM781x_STATUS   ATBM781xDVBSxChannelLockCheck(uint8 *pu8DvbSxLockStatus);
ATBM781x_STATUS   ATBM781xDVBSxChannelLockCheckBlindScan(uint8 *LockStatus);

ATBM781x_STATUS   ATBM781xDVBSxSNR(int32 *pu32SNRx10);
ATBM781x_STATUS   ATBM781xDVBSxSignalQuality(uint8 *pu8SQI);
ATBM781x_STATUS   ATBM781xDVBSxPER(uint32 *pui32PER);  /*PER is error rate after FEC bit corrected.*/
ATBM781x_STATUS   ATBM781xDVBSxBER(uint32 *pui32BER);  /*BER is error rate before FEC bit corrected.*/
ATBM781x_STATUS   ATBM781xDVBSxSignalParams(ATBM781x_DVBSx_SIGNAL_PARAMS *pstDVBSxParams);
ATBM781x_STATUS   ATBM781xGetDVBSxCarrierOffset(int32 *i32CarrierOfst);
ATBM781x_STATUS   ATBM781xGetDVBSxSymbolRate(uint32 *pui32SymbolRate);
ATBM781x_STATUS   ATBM781xGetDVBSxLockedCFO(int *i32LockedCFO);
/****************************Status Function,for for DVBS  ******************************/
ATBM781x_STATUS   ATBM781xGetDVBSSNR(int32 *ui32SNR);
ATBM781x_STATUS   ATBM781xGetDVBSBER(uint32 *ui32BER);
ATBM781x_STATUS   ATBM781xGetDVBSUncorrectablePER(uint32 *ui32PER);
/****************************Status Function,for for DVBS2  ******************************/
ATBM781x_STATUS   ATBM781xGetDVBS2BER(uint32 *ui32S2BER);
ATBM781x_STATUS   ATBM781xGetDVBS2PostBCHPER(uint32 *ui32S2PER);
ATBM781x_STATUS   ATBM781xGetDVBS2SNR(int32 *i32SNR);
ATBM781x_STATUS   ATBM781xDVBS2CheckingThread(void);
ATBM781x_STATUS   ATBM781xDvbSxSymbolRateReset(void);
ATBM781x_STATUS   ATBM781xGetDvbSxSpectrumAuto(void); 
ATBM781x_STATUS   ATBM781xGetDvbSxSpectrumManual(ATBM781x_DVBSx_SIGNAL_SPECTRUM  *pSignalSpectrum);
/*****************************DVBS S2 Blind scan used*************************************/
typedef struct ATBM781xDvbSxChannel
{
	uint32 u32FrequencyKHz;		/*Center frequency in units of kHz. */
	uint32 u32SymbolRateK;		/*The symbol rate. */
	ATBM781x_DVBSx_SIGNAL_PARAMS stDvbSxSignalPrams;
}stATBM781xDvbSxChannel;

#define MAX_SCANED_CHANNEL_NUMBER 500

struct ATBM781xDvbSxChannelScanParams
{
	uint32  u32ScanStartFrequencyKHz;  /*blind scan start frequency*/
	uint32 u32ScanStopFrequencyKHz;   /*blind scan stop frequency*/

	uint32 u32MinSymbolRateK;             /*min symbol rate can be detected. >1M is default setting*/
	uint32 u32MaxSymbolRateK;            /*max symbol rate can be detected <=45M is default setting*/
	uint32 u32BlindScanStepKHz;                /*Blind Scan step in KHz*/
	uint32 u32BlindScanFineStepKHz;                /*Blind Scan step in KHz when signal checked may be locked*/
	uint32 u32CurrentWorkFreKHz;                /*in KHz*/
	uint32 u32MaxCarrierOffsetKHz;
	uint8  u8SingalMayBeChecked;              /*Unknown signal,assuming signal may be checked set 1, no symbol get set 0.*/
	int32  i32ScanCheckedCFOKHz;
	uint32 u32ScanPreCheckedFreqKHz;          /*Recored checked frequency.*/
	uint32 u32PreTunedAbsCFOKHz;
	uint32 u32PreTunedSymbolRateK;
	uint32 u32PreTunedCorseSymbolRateK;
	uint32 u32PreTunedRaciPower;

	stATBM781xDvbSxChannel  ATBM781xDvbSxChannel[MAX_SCANED_CHANNEL_NUMBER]; /*channel list used by SDK blind scan module*/
	uint32 u32ValidChannelNumber,ui32ScanedValideChannelNum; /*record current valid channel counter.*/

	/*Tuner parameters and  functions*/
	uint32 u32TunerLPF;
	ATBM781x_STATUS (*pFuncTunerSetFrequency)(uint32 CenterFrequency, uint32 u32SymbolRate);
	ATBM781x_STATUS (*pFuncTunerLockStatus)(uint8 *pLockFlag);
};


/****************************Common Function,for DVB-T, DVB-T2, DVB-C, DVB-S and DVB-S2 operation***************************/
/*int ATBM781xLog2(int i32Data);*/
uint32 ATBM781xLog10ConvertdB(uint32 i32InValue);  //convert value to dB
uint32 ATBM781xLog10ConvertdBx10(uint32 i32InValue);
uint32 ATBM781xFixPow(uint32 x,uint32 y);
void ATBM781x64Add(ATBM781x64Data *pstSum,uint32 ui32Addend);
void ATBM781x64Mult(ATBM781x64Data *pstRst,uint32 m1, uint32 m2);
uint32 ATBM781x64Div(ATBM781x64Data stDivisor,ATBM781x64Data stDividend);
void ATBM781xPrintStr(const char *StrType, const char *StrValue);

ATBM781x_STATUS    ATBM781xSetTSMode(ATBM781x_MPEGTSMode stTSMode); /*internal function*/
/*ATBM781x_STATUS ATBM781xTSLockFlag(uint8 *LockStatus);*/

ATBM781x64Data ATBM781x64DivReturn64(ATBM781x64Data stDivisor,ATBM781x64Data stDividend);

ATBM781x_STATUS   ATBM781xRead(uint8 ui8BaseAddr, uint8 ui8RegisterAddr,uint8 *ui8RegValue);
ATBM781x_STATUS   ATBM781xWrite(uint8 ui8BaseAddr, uint8 ui8RegisterAddr, uint8 ui8data);
ATBM781x_STATUS   ATBM781xDVBCRead(uint32 uAddress,uint32 *ui32RData);
ATBM781x_STATUS   ATBM781xDVBCWrite(uint32 ui32Address, uint32 ui32Data);
ATBM781x_STATUS   ATBM781xDVBSWrite(uint16 ui16Addr,uint32 ui32Data);
ATBM781x_STATUS   ATBM781xDVBSRead(uint16 ui16Addr,uint32 *ui32RegData);	
ATBM781x_STATUS   ATBM781xWriteRegArray(uint8 *pRegTable, int iTableLen);
ATBM781x_STATUS   ATBM781xLatchOn(void);
ATBM781x_STATUS   ATBM781xLatchOff(void);
ATBM781x_STATUS   ATBM781xTunerI2CViaDemodOn(void);
ATBM781x_STATUS   ATBM781xTunerI2CViaDemodOff(void);
ATBM781x_STATUS   ATBM781xI2CPassThroughFloating(void); /*for tuner connect directly with host*/

ATBM781x_STATUS   ATBM781xHoldDSP(void);
ATBM781x_STATUS   ATBM781xStartDSP(void);
ATBM781x_STATUS   ATBM781xChipID(uint8 *ui8ChipID);

ATBM781x_STATUS ATBM781xPowerOnInit(void);
ATBM781x_STATUS ATBM781xSetSysClkClockDVBSx(void);/*internal function*/
ATBM781x_STATUS ATBM781xSetSysClkClockDVBTx(void);/*internal function*/
ATBM781x_STATUS ATBM781xSetSysClkClockDVBC(void);/*internal function*/
ATBM781x_STATUS ATBM781xStandBy(void);
ATBM781x_STATUS ATBM781xStandByWakeUp(void);
ATBM781x_STATUS ATBM781xSusPend(void);
ATBM781x_STATUS ATBM781xSetGpioPins(void);/*internal function*/
/*ATBM781x_STATUS DVBSS2PhaseNoise(void); not used*/
ATBM781x_STATUS  ATBM781xGetDVBS2RightCCIPower(uint32 *ui32SxRcciPwr);
ATBM781x_STATUS ATBM781xDVBSxCoarseSymbolRate(uint32 *CoarseSymbolFakeDetectedK);

ATBM781x_STATUS ATBM781xSetGPIOHighLowGPOStatus(uint8 attribute);
ATBM781x_STATUS ATBM781xSetGPIOHighLowGPO2(uint8 attribute);
ATBM781x_STATUS ATBM781xSetGPIOHighLowGPO4(uint8 attribute);
ATBM781x_STATUS ATBM781xSetGPIOHighLowGPO5(uint8 attribute);



extern unsigned char gATBM781xDvbSxInitFlag;
extern unsigned char gATBM781xPowerOnInited;
/****************************I2C and delay  Function, should be implemented *********************************/
ATBM781x_STATUS   ATBM781xSleep(uint32 ui32ms);
ATBM781x_STATUS   ATBM781xMutexIni(void);
ATBM781x_STATUS   ATBM781xMutexLock(void);
ATBM781x_STATUS   ATBM781xMutexUnlock(void);
ATBM781x_STATUS   ATBM781xI2CRead(uint8 ui8BaseAddr, uint8 ui8RegisterAddr,uint8 *ui8RegValue);
ATBM781x_STATUS   ATBM781xI2CWrite(uint8 ui8BaseAddr, uint8 ui8RegisterAddr, uint8 ui8data);


ATBM781x_STATUS ATBM781xDynamicCheckProcess(void); /*API should be called on 500ms period.*/
/*Check SDK code porting for FAE.*/
ATBM781x_STATUS ATBM781xCheckSDKPortForFAE(void); /*API should be called as function's description         */
/*ATBM781x_STATUS ATBM781xPowerSavingProcess(); Internal API*/
ATBM781x_STATUS ATBM781xRestorRegisters(void);
extern uint8  u8ATBM781xSoftwareVersion; 
extern   stATBM781xDvbConfig gstATBM781xDvbConfig;
#endif
