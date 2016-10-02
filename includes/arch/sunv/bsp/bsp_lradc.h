/*
*********************************************************************************************************
*                                                    MELIS
*                                    the Easy Portable/Player Develop Kits
*                                                   BSP LRADC
*
*                                    (c) Copyright 2011-2014, Jackie China
*                                             All Rights Reserved
*
* File    : bsp_lradc.h
* By      : Jackie
* Version : v1.0
* Date    : 2011-11-21
* Descript: bsp lradc head file.
* Update  : date                auther      ver     notes
*           2011-11-24 15:07    Jackie      1.0     Create this file.
*********************************************************************************************************
*/

#ifndef __BSP_LRADC_H__
#define __BSP_LRADC_H__

#define SCANCODE_MAX 		(0x3f)

typedef enum
{
	LRADC_CHANNEL_0,
	LRADC_CHANNEL_1
}__lradc_channel_e;

//interrupt type
typedef enum
{
    LRADC_DATA_INT          = 0x01,
    LRADC_KEYDOWN_INT       = 0x02,
    LRADC_HOLD_INT          = 0x04,
    LRADC_ALRDY_HOLD_INT    = 0x08,
    LRADC_KEYUP_INT     	= 0x10,
    LRADC_ALL_INT           = 0x1f 
}__lradc_int_type_e;

//sample frequency
typedef enum
{
    LRADC_SAMPLE_RATE_250HZ  = 0x0,  //4ms
    LRADC_SAMPLE_RATE_125HZ  = 0x1,  //8ms
    LRADC_SAMPLE_RATE_63HZ   = 0x2,  //16ms 
    LRADC_SAMPLE_RATE_32HZ   = 0x3,  //32ms   
}__lradc_sample_rate_e;

//key mode
typedef enum
{
    LRADC_KEY_MODE_NORMAL   = 0x0,
    LRADC_KEY_MODE_SINGLE   = 0x1,
    LRADC_KEY_MODE_CONTINUE = 0x2,
}__lradc_key_mode_e;

//level b voltage
typedef enum
{
    LRADC_LEVELB_1_9V  =  0x0,
    LRADC_LEVELB_1_8V  =  0x1,
    LRADC_LEVELB_1_7V  =  0x2,
    LRADC_LEVELB_1_6V  =  0x3,
}__lradc_level_b_e;


/*
*********************************************************************************************************
*                                           LRADC REQUEST
*
* Description: Lradc request.
*
* Parameters : adc_chan : 
						LRADC_CHANNEL_0 : ADC0.  
						LRADC_CHANNEL_1 : ADC1. 
*
* Returns    : adc handle if succeed, NULL if fail.
*
* Note       : 
*********************************************************************************************************
*/
__hdle BSP_LRADC_Request(__u32 adc_chan);


/*
*********************************************************************************************************
*                                           LRADC RELEASE
*
* Description: Lradc release.
*
* Parameters : adc_hdl : adc handle to release. 
*
* Returns    : EPDK_OK if succeed, EPDK_FAIL if fail.
*
* Note       : 
*********************************************************************************************************
*/
__s32 BSP_LRADC_Release(__hdle adc_hdl);


/*
*********************************************************************************************************
*                                           LRADC SET SAMPLE RATE
*
* Description: Lradc set sample rate.
*
* Parameters : sample_rate: 
                           LRADC_SAMPLE_RATE_250HZ:250Hz.
                           LRADC_SAMPLE_RATE_125HZ:125Hz.
                           LRADC_SAMPLE_RATE_63HZ :62.5Hz.
                           LRADC_SAMPLE_RATE_32HZ :32.25Hz.
*
* Returns    : EPDK_OK if succeed, EPDK_FAIL if fail.
*
* Note       : 
*********************************************************************************************************
*/
__s32 BSP_LRADC_SetSampleRate(__hdle adc_hdl, __u32 sample_rate);


/*
*********************************************************************************************************
*                                           LRADC KEY CONFIG
*
* Description: Lradc key config.
*
* Parameters : key_mode     : LRADC_KEY_MODE_NORMAL, LRADC_KEY_MODE_SINGLE, LRADC_KEY_MODE_CONTINUE.
               continue_time: should between 0 and 15, when LRADC_KEY_MODE_CONTINUE selected, one of 8*(continue_time + 1) samples as a valuable data.
               levelB       :
                              LRADC_LEVELB_1_9V : 1.9V.
                              LRADC_LEVELB_1_8V : 1.8V.
                              LRADC_LEVELB_1_7V : 1.7V.
                              LRADC_LEVELB_1_6V : 1.6V.
               levelA_B_cnt : should between 0 and 15, levelA to levelB time threshold is (levelA_B_cnt + 1).
*
* Returns    : EPDK_OK if succeed, EPDK_FAIL if fail.
*
* Note       : 
*********************************************************************************************************
*/
__s32 BSP_LRADC_KeyConfig(__hdle adc_hdl, __u32 key_mode, __u32 continue_time, __u32 levelB, __u32 levelA_B_cnt);


/*
*********************************************************************************************************
*                                           LRADC ENABLE INT
*
* Description: Lradc enable interrupt.
*
* Parameters : int_type : interrupt type, such as LRADC_DATA_INT defined in bsp_lradc.h.              
*
* Returns    : EPDK_OK if succeed, EPDK_FAIL if fail.
*
* Note       : 
*********************************************************************************************************
*/
__s32 BSP_LRADC_EnableInt(__hdle adc_hdl, __u32 int_type);


/*
*********************************************************************************************************
*                                           LRADC DISABLE INT
*
* Description: Lradc disable interrupt.
*
* Parameters : int_type : interrupt type, such as LRADC_DATA_INT defined in bsp_lradc.h.               
*
* Returns    : EPDK_OK if succeed, EPDK_FAIL if fail.
*
* Note       : 
*********************************************************************************************************
*/
__s32 BSP_LRADC_DisableInt(__hdle adc_hdl, __u32 int_type);


/*
*********************************************************************************************************
*                                           LRADC QUERY INT
*
* Description: Lradc query interrupt status.
*
* Parameters : adc_hdl : adc handle.
*
* Returns    : Interrupt pending type, such as LRADC_DATA_INT defined in bsp_lradc.h.
*
* Note       : 
*********************************************************************************************************
*/
__u32 BSP_LRADC_QueryInt(__hdle adc_hdl);


/*
*********************************************************************************************************
*                                           LRADC CLEAR INT PENDING
*
* Description: Lradc clear interrupt pending.
*
* Parameters : int_type : the interrupt pending type to be cleared.              
*
* Returns    : EPDK_OK if succeed, EPDK_FAIL if fail.
*
* Note       : 
*********************************************************************************************************
*/
__s32 BSP_LRADC_ClearIntPending(__hdle adc_hdl, __u32 int_type);


/*
*********************************************************************************************************
*                                           LRADC READ DATA
*
* Description: Lradc read data.
*
* Parameters : adc_hdl : adc handle.
*
* Returns    : Lradc data.
*
* Note       : 
*********************************************************************************************************
*/
__u32 BSP_LRADC_ReadData(__hdle adc_hdl);

#endif  //__BSP_LRADC_H__

