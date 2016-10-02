/*
*********************************************************************************************************
*                                                    MELIS
*                                    the Easy Portable/Player Develop Kits
*                                            ADDA Bsp Module
*
*                                    (c) Copyright 2011-2015, terry.zeng China
*                                             All Rights Reserved
*
* File    : bsp_adda.h
* By      : terry.zeng
* Version : v1.0
* Date    : 2011-12-28 14:10:13
* Update  :       date                auther      ver     notes
*           2011-12-28 14:10:13	    terry.zeng    v1.0    	
*********************************************************************************************************
*/

#ifndef __BSP_ADDA_H__
#define __BSP_ADDA_H__

//------------------------------------------------------------------------------
//          ADDA physic device type
//------------------------------------------------------------------------------

#define BSP_AUDIO_DEV_TYPE_PLAY     (1<<0)  //physic audio play device
#define BSP_AUDIO_DEV_TYPE_REC      (1<<1)  //physic audio record device
#define BSP_AUDIO_DEV_TYPE_FM       (1<<2)  //physic audio fm device
#define BSP_AUDIO_DEV_TYPE_CTRL     (1<<3)  //physic audio control device
//------------------------------------------------------------------------------
//          record source
//------------------------------------------------------------------------------
typedef enum
{
	BSP_ADDA_REC_ERR,
	BSP_ADDA_REC_MIC1,		//record source:mic
	BSP_ADDA_REC_MIC2,
	BSP_ADDA_REC_MIC1_AND_MIC2,
	BSP_ADDA_REC_MIXER,
	BSP_ADDA_REC_FM,				//record source:FM
	BSP_ADDA_REC_LINEIN			//record source:line_in
}bsp_adda_rec_source_e;
//------------------------------------------------------------------------------
//          when recording,if play back
//------------------------------------------------------------------------------
typedef enum
{
	BSP_ADDA_REC_PLAYBACK,		//recording and play back
	BSP_ADDA_REC_NO_PLAYBACK		//recording without play back
}bsp_adda_rec_ifplayback_e;

/*
*********************************************************************************************************
*                                          BSP_ADDA_Init
*
* Description: BSP ADDA init.
*
* Parameters : NULL.
*
* Returns    : EPDK_OK if succeeded, others if failed.
*
* Note       : 
*********************************************************************************************************
*/
__s32 BSP_ADDA_Init(void);
/*
*********************************************************************************************************
*                                          BSP_ADDA_Exit
*
* Description: BSP ADDA Exit.
*
* Parameters : NULL.
*
* Returns    : EPDK_OK if succeeded, others if failed.
*
* Note       : 
*********************************************************************************************************
*/
__s32 BSP_ADDA_Exit(void);
/*
*********************************************************************************************************
*                                          BSP_ADDA_Open
*
* Description: BSP ADDA Open.
*
* Parameters : ADDA physic device type BSP_AUDIO_DEV_TYPE_PLAY\BSP_AUDIO_DEV_TYPE_REC\BSP_AUDIO_DEV_TYPE_FM\BSP_AUDIO_DEV_TYPE_CTRL.
*
* Returns    : EPDK_OK if succeeded, others if failed.
*
* Note       : 
*********************************************************************************************************
*/
__s32 BSP_ADDA_Open(__u32 uDevType);
/*
*********************************************************************************************************
*                                          BSP_ADDA_Close
*
* Description: BSP ADDA Close.
*
* Parameters : ADDA physic device type BSP_AUDIO_DEV_TYPE_PLAY\BSP_AUDIO_DEV_TYPE_REC\BSP_AUDIO_DEV_TYPE_FM\BSP_AUDIO_DEV_TYPE_CTRL.
*
* Returns    : EPDK_OK if succeeded, others if failed.
*
* Note       : 
*********************************************************************************************************
*/
__s32 BSP_ADDA_Close(__u32 uDevType);
/*
*********************************************************************************************************
*                                          BSP_ADDA_Start
*
* Description: BSP ADDA Start.
*
* Parameters : ADDA physic device type BSP_AUDIO_DEV_TYPE_PLAY\BSP_AUDIO_DEV_TYPE_REC\BSP_AUDIO_DEV_TYPE_FM.
*
* Returns    : EPDK_OK if succeeded, others if failed.
*
* Note       : 
*********************************************************************************************************
*/
__s32 BSP_ADDA_Start(__u32 uDevType);
/*
*********************************************************************************************************
*                                          BSP_ADDA_Stop
*
* Description: BSP ADDA Stop.
*
* Parameters : ADDA physic device type BSP_AUDIO_DEV_TYPE_PLAY\BSP_AUDIO_DEV_TYPE_REC\BSP_AUDIO_DEV_TYPE_FM.
*
* Returns    : EPDK_OK if succeeded, others if failed.
*
* Note       : 
*********************************************************************************************************
*/
__s32 BSP_ADDA_Stop(__u32 uDevType);
/*
*********************************************************************************************************
*                                          BSP_ADDA_Pause
*
* Description: BSP ADDA Pause.
*
* Parameters : ADDA physic device type BSP_AUDIO_DEV_TYPE_PLAY\BSP_AUDIO_DEV_TYPE_REC\BSP_AUDIO_DEV_TYPE_FM.
*
* Returns    : EPDK_OK if succeeded, others if failed.
*
* Note       : 
*********************************************************************************************************
*/
__s32 BSP_ADDA_Pause(__u32 uDevType);
/*
*********************************************************************************************************
*                                          BSP_ADDA_Continue
*
* Description: BSP ADDA Continue.
*
* Parameters : ADDA physic device type BSP_AUDIO_DEV_TYPE_PLAY\BSP_AUDIO_DEV_TYPE_REC\BSP_AUDIO_DEV_TYPE_FM.
*
* Returns    : EPDK_OK if succeeded, others if failed.
*
* Note       : 
*********************************************************************************************************
*/
__s32 BSP_ADDA_Continue(__u32 uDevType);
/*
*********************************************************************************************************
*                                          BSP_ADDA_SetSampleWidth
*
* Description: SET the physic sample width of ADDA device.
*
* Parameters : uDevType:ADDA physic device type BSP_AUDIO_DEV_TYPE_PLAY\BSP_AUDIO_DEV_TYPE_REC.
*			   nSampWidth:sample width
*
* Returns    : EPDK_OK if succeeded, others if failed.
*
* Note       : 
*********************************************************************************************************
*/
__s32 BSP_ADDA_SetSampleWidth(__u32 uDevType, __u32 nSampWidth);
/*
*********************************************************************************************************
*                                          BSP_ADDA_GetSampleWidth
*
* Description: Get the physic sample width of ADDA device.
*
* Parameters : uDevType:ADDA physic device type BSP_AUDIO_DEV_TYPE_PLAY\BSP_AUDIO_DEV_TYPE_REC.
*
* Returns    : the sample width of ADDA device if succeeded, -1:fail.
*
* Note       : 
*********************************************************************************************************
*/
__s32 BSP_ADDA_GetSampleWidth(__u32 uDevType);
/*
*********************************************************************************************************
*                                          BSP_ADDA_SetSampRate
*
* Description: SET the physic sample rate of ADDA device..
*
* Parameters : uDevType:ADDA physic device type BSP_AUDIO_DEV_TYPE_PLAY\BSP_AUDIO_DEV_TYPE_REC.
*			   nSampRate:sample rate
*
* Returns    : EPDK_OK if succeeded, others if failed.
*
* Note       : 
*********************************************************************************************************
*/
__s32 BSP_ADDA_SetSampRate(__u32 uDevType, __u32 nSampRate);
/*
*********************************************************************************************************
*                                          BSP_ADDA_GetSampRate
*
* Description: Get the physic sample rate of ADDA device..
*
* Parameters : uDevType:ADDA physic device type BSP_AUDIO_DEV_TYPE_PLAY\BSP_AUDIO_DEV_TYPE_REC.
*
* Returns    : the sample width of ADDA device if succeeded, -1:fail.
*
* Note       : 
*********************************************************************************************************
*/
__s32 BSP_ADDA_GetSampRate(__u32 uDevType);
/*
*********************************************************************************************************
*                                          BSP_ADDA_SetChannel
*
* Description: SET the physic channel of ADDA device..
*
* Parameters : uDevType:ADDA physic device type BSP_AUDIO_DEV_TYPE_PLAY\BSP_AUDIO_DEV_TYPE_REC\BSP_AUDIO_DEV_TYPE_FM.
*			   nChnCnt:channel count
*
* Returns    : EPDK_OK if succeeded, others if failed.
*
* Note       : 
*********************************************************************************************************
*/
__s32 BSP_ADDA_SetChannel(__u32 uDevType, __u32 nChnCnt);
/*
*********************************************************************************************************
*                                          BSP_ADDA_GetChannel
*
* Description: SET the physic channel of ADDA device..
*
* Parameters : uDevType:ADDA physic device type BSP_AUDIO_DEV_TYPE_PLAY\BSP_AUDIO_DEV_TYPE_REC\BSP_AUDIO_DEV_TYPE_FM.
*
* Returns    : the channel of ADDA device if succeeded, -1:fail.
*
* Note       : 
*********************************************************************************************************
*/
__s32 BSP_ADDA_GetChannel(__u32 uDevType);
/*
*********************************************************************************************************
*                                          BSP_ADDA_SetSampleCnt
*
* Description: reset the sample count of ADDA device.
*
* Parameters : uDevType:ADDA physic device type BSP_AUDIO_DEV_TYPE_PLAY\BSP_AUDIO_DEV_TYPE_REC.
*			   nChnCnt:sample count
*
* Returns    : EPDK_OK if succeeded, others if failed.
*
* Note       : 
*********************************************************************************************************
*/
__s32 BSP_ADDA_SetSampleCnt(__u32 uDevType, __u32 nSampleCnt);
/*
*********************************************************************************************************
*                                          BSP_ADDA_GetSampleCnt
*
* Description: get the sample count of ADDA device.
*
* Parameters : uDevType:ADDA physic device type BSP_AUDIO_DEV_TYPE_PLAY\BSP_AUDIO_DEV_TYPE_REC.
*
* Returns    : the sample count of ADDA device if succeeded, -1:fail.
*
* Note       : 
*********************************************************************************************************
*/
__s32 BSP_ADDA_GetSampleCnt(__u32 uDevType);
/*
*********************************************************************************************************
*                                          BSP_ADDA_SetVolume
*
* Description: set the volume of ADDA device.
*
* Parameters : uDevType:ADDA physic device type BSP_AUDIO_DEV_TYPE_PLAY\BSP_AUDIO_DEV_TYPE_REC\BSP_AUDIO_DEV_TYPE_FM.
*
* Returns    : EPDK_OK if succeeded, others if failed.
*
* Note       : 
*********************************************************************************************************
*/
__s32 BSP_ADDA_SetVolume(__u32 uDevType, __s32 nVolume);
/*
*********************************************************************************************************
*                                          BSP_ADDA_GetVolume
*
* Description: get the volume of ADDA device.
*
* Parameters : uDevType:ADDA physic device type BSP_AUDIO_DEV_TYPE_PLAY\BSP_AUDIO_DEV_TYPE_REC\BSP_AUDIO_DEV_TYPE_FM.
*
* Returns    : the volume of ADDA device if succeeded, -1:fail.
*
* Note       : 
*********************************************************************************************************
*/
__s32 BSP_ADDA_GetVolume(__u32 uDevType);
/*
*********************************************************************************************************
*                                          BSP_ADDA_SetSource
*
* Description: set the source of record device.
*
* Parameters : source value.
*
* Returns    : EPDK_OK if succeeded, others if failed.
*
* Note       : 
*********************************************************************************************************
*/
__s32 BSP_ADDA_SetRecSource(bsp_adda_rec_source_e source);
/*
*********************************************************************************************************
*                                          BSP_ADDA_GetRecSource
*
* Description: Get the source of adda recorder.
*
* Parameters : BSP_ADDA_REC_MIC\BSP_ADDA_REC_FM\BSP_ADDA_REC_LINEIN.
*
* Returns    :BSP_ADDA_REC_MIC\BSP_ADDA_REC_FM\BSP_ADDA_REC_LINEIN;-1,failed.
*
* Note       : 
*********************************************************************************************************
*/
bsp_adda_rec_source_e BSP_ADDA_GetRecSource(void);
/*
*********************************************************************************************************
*                                          BSP_ADDA_SetRecSourceGain
*
* Description: set the source of record device.
*
* Parameters : source value.
*
* Returns    : EPDK_OK if succeeded, others if failed.
*
* Note       : 
*********************************************************************************************************
*/
__s32 BSP_ADDA_SetRecSourceGain(__s32 SourceGain);
/*
*********************************************************************************************************
*                                          BSP_ADDA_SetRecAdcGain
*
* Description: set the adc gain of record device.
*
* Parameters : GAIN value.
*
* Returns    : EPDK_OK if succeeded, others if failed.
*
* Note       : 
*********************************************************************************************************
*/
__s32 BSP_ADDA_SetRecAdcGain(__s32 AdcGain);
/*
*********************************************************************************************************
*                                          BSP_ADDA_SetRecIfplayback
*
* Description: if the play back when recording.
*
* Parameters : BSP_ADDA_REC_PLAYBACK\BSP_ADDA_REC_NO_PLAYBACK.
*
* Returns    :EPDK_OK if succeeded, others if failed.
*
* Note       : 
*********************************************************************************************************
*/
__s32 BSP_ADDA_SetRecIfPlayBack(bsp_adda_rec_ifplayback_e IfPlayBack);
/*
*********************************************************************************************************
*                                          BSP_ADDA_GetFifoAddr
*
* Description: GET fifo ADDR.
*
* Parameters : .
*
* Returns    :EPDK_OK if succeeded, others if failed.
*
* Note       : 
*********************************************************************************************************
*/
__s32 BSP_ADDA_GetFifoAddr(__u32 uDevType);
/*
*********************************************************************************************************
*                                          BSP_ADDA_EnterStandby
*
* Description: adda enter stabdby.
*
* Parameters : 
*
* Returns    :EPDK_OK if succeeded, others if failed.
*
* Note       : 
*********************************************************************************************************
*/
__s32 BSP_ADDA_EnterStandby(void);
/*
*********************************************************************************************************
*                                          BSP_ADDA_ExitStandby
*
* Description: adda exit stabdby.
*
* Parameters : 
*
* Returns    :EPDK_OK if succeeded, others if failed.
*
* Note       : 
*********************************************************************************************************
*/
__s32 BSP_ADDA_ExitStandby(void);



__s32 BSP_ADDA_MUTE(__s32 mute);
__s32 BSP_ADDA_FlushFifo(__u32 uDevType);


#endif  //__BSP_ADDA_H__
