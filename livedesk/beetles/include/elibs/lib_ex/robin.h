/*
************************************************************************************************************************
*                                                        cuckoo_core
*
*                             Copyright(C), 2009-2010, SoftWinners Microelectronic Co., Ltd.
*											       All Rights Reserved
*
* File Name   : cuckoo_core.h
*
* Author      : Gary.Wang
*
* Version     : 1.1.0
*
* Date        : 2009.09.29
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
* Gary.Wang      2009.09.29       1.1.0        build the file
*
************************************************************************************************************************
*/
#ifndef  __robin_h__
#define  __robin_h__


#include "robin\\ab\\robin_ab.h"
#include "robin\\channel\\robin_channel.h"
#include "robin\\eq\\robin_eq.h"
#include "robin\\ff_rr_speed\\robin_ff_rr_speed.h"
#include "robin\\fsm_ctrl\\robin_fsm_ctrl.h"
#include "robin\\lyric\\robin_lyric.h"
#include "robin\\media_info\\robin_media_info.h"
#include "robin\\npl\\robin_npl.h"
#include "robin\\play_mode\\robin_play_mode.h"
#include "robin\\play_speed\\robin_play_speed.h"
#include "robin\\spectrum\\robin_spectrum.h"
#include "robin\\star\\robin_star.h"
#include "robin\\subtitle\\robin_subtitle.h"
#include "robin\\track\\robin_track.h"
#include "robin\\video_layer\\robin_video_layer.h"
#include "robin\\video_win\\robin_video_win.h"
#include "robin\\vision_effect\\robin_vision_effect.h"
#include "robin\\volume\\robin_volume.h"
#include "robin\\zoom\\robin_zoom.h"
#include "robin\\disp_output\\robin_disp_output.h"
#include "robin\\palette\\robin_palette.h"


typedef enum
{
	ROBIN_MODE_AUDIO_MIN  = 0,
	ROBIN_MODE_AUDIO_MAX     ,
	ROBIN_MODE_VIDEO_MIN     ,
	ROBIN_MODE_VIDEO_MAX     ,
	
	ROBIN_MODE_UNKNOWN    = -1
}robin_mode_e;

typedef struct
{
	__u32 reserve_mem_size;//ָ��cedarԤ����Ӧ�õ��ڴ棬���ֽ�Ϊ��λ��һ��Ϊ500K-1M���㹻��
	CedarStopMode cedar_stop_mode;//CEDAR_STOP_MODE_KEEP_PLUGINS:ֹͣ����ʱ�������в����CEDAR_STOP_MODE_UNINSTALL_PLUGINSֹͣ����ʱж�����в��
	CedarFileSwitchVplyMode switch_vplay_mode;// CEDAR_FILE_SWITCH_VPLY_MODE_GAP:2���ļ��л�ʱ�����ϵģ��м��к���, CEDAR_FILE_SWITCH_VPLY_MODE_SEAMLESS:�޷�, ������.
}robin_open_arg_t;

	
/***************************************************************************************************
*Name        : robin_open
*Prototype   : __s32  robin_open( robin_mode_e robin_mode, void *arg_p )
*Arguments   : robin_mode    input. the work mode of the cuckoo_core module.
*              arg_p       input. arguments, if NULL��default value will be set.
*Return      : ==0    succeed
*              !=0    fail
*Description : open cuckoo_core.
*Other       :
***************************************************************************************************/
extern __s32  robin_open( robin_mode_e robin_mode, robin_open_arg_t *arg_p );

/***************************************************************************************************
*Name        : robin_close
*Prototype   : void  robin_close( void )
*Arguments   : void
*Return      : void
*Description : close cuckoo_core.
*Other       :
***************************************************************************************************/
extern void  robin_close( void );

/***************************************************************************************************
*Name        : robin_set_mode
*Prototype   : __s32  robin_set_mode( robin_mode_e robin_mode )
*Arguments   : robin_mode    input. the work mode of the cuckoo_core module.
*Return      : ==0    succeed
*              !=0    fail
*Description : set the work mode of the cuckoo_core module.
*Other       :
***************************************************************************************************/
extern __s32  robin_set_mode( robin_mode_e robin_mode, robin_open_arg_t *arg_p  );

/***************************************************************************************************
*Name        : robin_get_mode
*Prototype   : robin_mode_e  robin_get_mode( void )
*Arguments   : void
*Return      : the work mode of the cuckoo_core module.
*Description : get the work mode of the cuckoo_core module.
*Other       :
***************************************************************************************************/
extern robin_mode_e  robin_get_mode( void );

extern __bool robin_is_open(void);


#endif     //  ifndef __robin_h__

/* end of __robin_h__.h */
