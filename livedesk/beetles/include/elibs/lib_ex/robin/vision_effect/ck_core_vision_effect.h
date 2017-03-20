/*
************************************************************************************************************************
*                                                        cuckoo_core
*
*                             Copyright(C), 2009-2010, SoftWinners Microelectronic Co., Ltd.
*											       All Rights Reserved
*
* File Name   : ck_core_vision_effect.h
*
* Author      : Gary.Wang
*
* Version     : 1.1.0
*
* Date        : 2009.09.27
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
* Gary.Wang      2009.09.27       1.1.0        build the file
*
************************************************************************************************************************
*/
#ifndef  __ck_core_vision_effect_h
#define  __ck_core_vision_effect_h


/***************************************************************************************************
*Name        : ck_core_set_vision_effect
*Prototype   : __s32  ck_core_set_vision_effect( __disp_video_smooth_t  vision_effect )
*Arguments   : vision_effect    input. vision effect mode.
*Return      : ==0       succeed
*              !=0       fail
*Description : set vision effect mode, such natual, sharp and so on.
*Other       :
***************************************************************************************************/
__s32  ck_core_set_vision_effect( __disp_video_smooth_t  vision_effect );


/***************************************************************************************************
*Name        : ck_core_get_vision_effect
*Prototype   : __disp_video_smooth_t  ck_core_get_vision_effect( void )
*Arguments   : void
*Return      : the current vision effect.
*Description : get the current vision effect .
*Other       :
***************************************************************************************************/
__disp_video_smooth_t  ck_core_get_vision_effect( void );



#endif     //  ifndef __ck_core_vision_effect_h

/* end of ck_core_vision_effect.h */
