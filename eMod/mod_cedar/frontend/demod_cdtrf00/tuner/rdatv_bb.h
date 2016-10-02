#ifndef _RDATV_BB_H_
#define _RDATV_BB_H_

RDA5880_ERR_MSG RDATVM_WRITE(UINT8 reg, UINT16 tdata) ;
RDA5880_ERR_MSG RDATVM_READ(UINT8 reg, UINT16* pdata) ;
void            RDADelayMs(UINT32 delay_ms) ;
UINT16 RDATVGetChipID(void);
#ifndef CLOSE_TUNER_UNNECESSARY_FUNCTION
RDA5880_ERR_MSG RDATVDetected( int *detect ) ;
#endif
RDA5880_ERR_MSG RDATVPowerOn(void);
RDA5880_ERR_MSG RDATVSet51Sta(UINT8 Status) ;
RDA5880_ERR_MSG RDATVGetI2VTH( RDA5880_PARAM *rf_param ) ;
RDATV_ADC_INDEX GetADCPatchByFre(UINT32 freq) ;
RDA5880_ERR_MSG RDATVDownLoad(void) ;

static          UINT32 RDATVMulDivCompensate(UINT32 par1,UINT32 par2,UINT32 par3) ;
static void     RDATVSetFreCalPar(UINT32 freq, RDA5880_PARAM *rf_param) ;
RDA5880_ERR_MSG RDATVSetFreq(UINT32 freq,UINT32 bw, RDA5880_PARAM *rf_param) ;
RDA5880_ERR_MSG RDATVGetIntensity(UINT32 *level ) ;
RDA5880_ERR_MSG RDATVSingalStrength( UINT8 *strength ) ;
void            RDATVLnaMode(UINT8 mode, RDA5880_PARAM *rf_param);	
RDA5880_ERR_MSG RDATVsetdemod(UINT8 mode, RDA5880_PARAM *rf_param);
void            RDATVSetMode(UINT8 mode, RDA5880_PARAM *rf_param);		
void            RDATVSetIfOutMode(UINT8 mode, RDA5880_PARAM *rf_param);
void            RDATVSetModeOutT(UINT8 mode, RDA5880_PARAM *rf_param);
void            RDATVSetAdcIndex(UINT8 index, RDA5880_PARAM *rf_param);
RDA5880_ERR_MSG RDATVsetlna(UINT32 freq, RDA5880_PARAM *rf_param) ;

RDA5880_ERR_MSG RDATVRfBp(UINT8 on);
RDA5880_ERR_MSG RDATVdelIM(UINT8 on);
RDA5880_ERR_MSG RDATVSleepnoloop(void) ;
RDA5880_ERR_MSG RDATVSleepwithloop(void) ;
//bool SAL_TUNER_RDA5880H_Init(void);
//bool SAL_TUNER_ProgramTuner_RDA5880H(UINT32 u32FreqKHz, UINT32 u32BandwidthKHz, RDA5880_PARAM *rf_param);
//bool SAL_TUNER_RDA5880H_PowerDown(bool b8enable);
//void Tuner_RDA5880h_DisplayCMD(UINT8 u8Val );
//void Tuner_RDA5880h_Monitor(bool b8LockStatus);
RDA5880_ERR_MSG devTunerInit( RDA5880_PARAM *rf_param, UINT32 Xtal_freq ) ;
RDA5880_ERR_MSG devDigitalTuner_SetFreq (UINT32 Freq , UINT8 eBandWidth, RDA5880_PARAM *rf_param ) ;

#endif

