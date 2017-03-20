/*
*********************************************************************************************************
*											        ePDK
*						            the Easy Portable/Player Develop Kits
*									           LARK sample
*
*						        (c) Copyright 2006-2007, terry, China
*											All	Rights Reserved
*
* File    : lark_p.h
* By      : terry.zeng
* Version : V1.00
*********************************************************************************************************
*/

#ifndef __DSK_RADIO_I_H__
#define __DSK_RADIO_I_H__

#include "..\\engine_i.h"
//#include ".\receive\dsk_radio_receive_i.h"
//#include ".\send\dsk_radio_send_i.h"
typedef struct
{
	ES_FILE 				*fm_drv;
	
	__s32 					drv_status;	
	
	/*��ǰƵ��*/
    __s32 					cur_freq;
    /*��ǰ����*/
    __s32 					cur_band;
    /*��ǰ���ζ�Ӧ��Ƶ�ʷ�Χ*/
    __drv_fm_area_freq_t 	freq_range;
    /*��ǰ��������*/
    dsk_radio_audio_mode_t 	audio_mode;
    
    //void					*rcv_handle;
    
    //void 					*send_handle;    
    
}dsk_radio_t;

extern dsk_radio_t *h_radio;

#endif

