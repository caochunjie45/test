/*
************************************************************************************************************************
*                                                       voice
*
*                                  Copyright(C), 2006-2010, AllWinner Technology Co., Ltd.
*											       All Rights Reserved
*
* File Name   : dsk_voice.h
*
* Author      : Gary.Wang
*
* Version     : 1.0
*
* Date        : 2010.10.13
*
* Description :
* 
* Others      : None at present.
*
*
* History     :
*
*  <Author>        <time>       <version>      <description>
*
* Gary.Wang      2010.10.13        1.0         build the file
*
************************************************************************************************************************
*/
#ifndef  __DSK_VOLUME_H__
#define  __DSK_VOLUME_H__

#ifdef __cplusplus
extern "C" {
#endif


/*
****************************************************************************************************
*Name        : dsk_voice_set_volume
*Prototype   : __s32 dsk_voice_set_volume( __s32 volume)
*Arguments   : volume     input. volume value, ranges AUDIO_DEVICE_VOLUME_MIN~~AUDIO_DEVICE_VOLUME_MAX.
*Return      : EPDK_OK    succeed
*              EPDK_FAIL  fail
*Description : set the volume.
*Other       :
****************************************************************************************************
*/
extern __s32 dsk_volume_set( __s32 volume );

/*
****************************************************************************************************
*Name        : dsk_voice_get_volume
*Prototype   : __s32 dsk_voice_get_volume(void)
*Arguments   : void
*Return      : the current volume value. -1 means fail.
*Description : get the curret volume value.
*Other       :
****************************************************************************************************
*/
extern __s32 dsk_volume_get( void );

/*
****************************************************************************************************
*Name        : dsk_voice_inc_volume
*Prototype   : __s32 dsk_voice_inc_volume(void);
*Arguments   : void
*Return      : the current volume value. -1 means fail.
*Description : increase the volume.
*Other       :
****************************************************************************************************
*/
extern __s32 dsk_volume_inc(void);

/*
****************************************************************************************************
*Name        : dsk_voice_dec_volume
*Prototype   : __s32 dsk_voice_dec_volume(void);
*Arguments   : void
*Return      : the current volume value. -1 means fail.
*Description : increase the volume.
*Other       :
****************************************************************************************************
*/
extern __s32 dsk_volume_dec(void);

/*
****************************************************************************************************
*Name        : dsk_voice_on
*Prototype   : __s32 dsk_voice_on(void);
*Arguments   : void
*Return      : EPDK_OK    succeed
*              EPDK_FAIL  fail
*Description : make the voice on.
*Other       :
****************************************************************************************************
*/
extern __s32 dsk_volume_on( void );

/*
****************************************************************************************************
*Name        : dsk_voice_off
*Prototype   : __s32 dsk_voice_off(void);
*Arguments   : void
*Return      : EPDK_OK    succeed
*              EPDK_FAIL  fail
*Description : make the voice off(mute).
*Other       :
****************************************************************************************************
*/
extern __s32 dsk_volume_off( void );

/*
****************************************************************************************************
*Name        : dsk_voice_is_on
*Prototype   : __bool dsk_voice_is_on( void )
*Arguments   : void
*Return      : the status of the voice, on or off(mute).
*Description : get the status of the voice, on or off(mute).
*Other       : 
****************************************************************************************************
*/
extern __bool dsk_volume_is_on( void );

extern void  dsk_speaker_onoff(__u32 onoff);

extern __bool dsk_speaker_is_on( void );

extern void set_record_is_play(__bool val);

extern __bool	dsk_record_is_play(void);

extern void dsk_set_fsm_statu(__s32 sta);

extern __s32 dsk_get_fsm_statu(void);

extern __s32 dsk_set_is_phone_in(__bool val);

extern __bool dsk_get_is_phone_in(void);

extern __s32 dsk_set_is_mic_in(__bool val);

extern __bool dsk_get_is_mic_in(void);


#ifdef __cplusplus
}
#endif

#endif     //  ifndef __DSK_VOICE_H__

/* end of dsk_voice.h */


