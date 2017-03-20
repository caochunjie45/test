/*
*********************************************************************************************************
*											        ePDK
*						            the Easy Portable/Player Develop Kits
*									           dsk_radio  sample
*
*						        (c) Copyright 2006-2007, terry, China
*											All	Rights Reserved
*
* File    : dsk_radio.h
* By      : terry.zeng
* Version : V1.00
*********************************************************************************************************
*/

#ifndef __DSK_RADIO_H__
#define __DSK_RADIO_H__


#include "epdk.h"
#include "radio//dsk_radio_receive.h"
#include "radio//dsk_radio_send.h"


#define MAX_CHANNEL_NUM	50

typedef struct
{
	__u32 valid_num;
	dsk_radio_chan_t freq_list[MAX_CHANNEL_NUM];
}dsk_radio_ch_list_t;
/*!
  * \brief
  *		dsk_radio��������ö��
*/
typedef enum
{
    DSK_RADIO_BAND_JAPAN = 0x01,
    DSK_RADIO_BAND_SCHOOL,
    DSK_RADIO_BAND_US_EUP
}dsk_radio_band_mode_t;

/*!
  * \brief
  *		DSK_RADIO�ź�ǿ������ֵö��
*/
typedef enum
{
    DSK_RADIO_THRESHOLD_HIGH = 0x01,
    DSK_RADIO_THRESHOLD_MID,
    DSK_RADIO_THRESHOLD_LOW
}dsk_radio_threshold_mode_t;


/*!
  * \brief
  *		dsk_radio��������
*/
typedef enum
{
    DSK_RADIO_REC_SEARCH_UP=0x01,
    DSK_RADIO_REC_SEARCH_DOWN
}dsk_radio_rec_auto_search_mode_t;

typedef enum
{
    DSK_RADIO_SEARCH_AUTO =0x01,
    DSK_RADIO_SEARCH_MANUAL
}dsk_radio_search_mode_t;

/*!
  * \brief
  *		dsk_radio���ŵ�����ģʽ
*/
typedef enum
{
    DSK_RADIO_AUDIO_MONO=0x01,
    DSK_RADIO_AUDIO_STEREO,
    DSK_RADIO_AUDIO_EXIT   
}dsk_radio_audio_mode_t;

typedef enum
{
    DSK_RADIO_EVENT_SEARCH_SUCCESS  = 0,
	DSK_RADIO_EVENT_SEARCH_FAIL     = 1,
	DSK_RADIO_EVENT_SEARCH_OVER     = 2,
    DSK_RADIO_EVENT_SEARCH_FREQ     = 3,	//��ʾ�ڲ����������е�Ƶ��
    DSK_RADIO_EVENT_UNKNOWN         = -1
}dsk_radio_event_e;

/*!
  * \brief
  *		dsk_radio�����ṩ�Ĺ�������
*/
typedef enum
{

/*DSK_RADIO ������չ����ӿ�-----------------------------------------------------------*/


	/*! ���� param1=0,param2=0*/
    DSK_RADIO_MUTE = 0x01,
    /*! ȡ������ param1=0,param2=0*/ 			 
    DSK_RADIO_UNMUTE, 
    /*! ���ò��� param1=dsk_radio_band_mode_t,param2=0*/   
    DSK_RADIO_SET_BAND,
    /*! �����ź�ǿ������ֵ param1=dsk_radio_threshold_mode_t,param2=0*/   
    DSK_RADIO_SET_HTRESHOLD,
    /*! ��������ģʽ param1=dsk_radio_audio_mode_t,param2=0*/  
    DSK_RADIO_SET_AUDIO_MODE,
    
    /*! ��ȡ��ǰ���� param1=0,param2=(int)&band*/
    DSK_RADIO_GET_BAND,
    /*! ��ȡ��ǰƵ�ʣ�����ֵ��KHZΪ��λ param1=0,param2=(int)&freq*/
    DSK_RADIO_GET_FREQ,
    /*! ��ȡ����ģʽ  param1=0,param2=(int)&audio_mode*/
    DSK_RADIO_GET_AUDIO_MODE,
    /*! ��ȡ��ǰ������СƵ�� param1=0,param2=(int)&min_freq*/
    DSK_RADIO_GET_MIN_FREQ,
    /*! ��ȡ��ǰ�������Ƶ�� param1=0,param2=(int)&max_freq*/   
    DSK_RADIO_GET_MAX_FREQ,
    
    
/*DSK_RADIO ������չ����ӿ�-----------------------------------------------------------*/
    
/*DSK_RADIO ���ղ���------------------------------------------------------------------*/
		
	DSK_RADIO_REC_OPEN,  
	
    /*�������ܲ���*/
    DSK_RADIO_REC_SET_AUTO_SEARCH_MODE,    
    DSK_RADIO_REC_AUTO_SEARCH,    
    DSK_RADIO_REC_AUTO_SEARCH_ALL,    
    
    /*���Ź��ܲ���*/    
	DSK_RADIO_REC_SET_FREQ_PLAY,
    DSK_RADIO_REC_NEXT_FREQ_PLAY,
    DSK_RADIO_REC_PRE_FREQ_PLAY,                  
    
    /*�б������*/  
	DSK_RADIO_REC_SET_PLAY_LIST_TYPE,
	DSK_RADIO_REC_GET_PLAY_LIST_TYPE,

    DSK_RADIO_REC_GET_SEARCH_RESULT_VN,
	DSK_RADIO_REC_GET_FAVORITE_VN,    
    DSK_RADIO_REC_GET_SEARCH_RESULT_CHAN,    
    DSK_RADIO_REC_GET_FAVORITE_CHAN,    
    
    DSK_RADIO_REC_GET_FAVORITE,
    DSK_RADIO_REC_SAVE_FAVORITE,
    DSK_RADIO_REC_GET_SEARCH_RESULT,
    DSK_RADIO_REC_SAVE_SEARCH_RESULT,    
    
    DSK_RADIO_REC_ADD_CHAN_TO_FAVORITE,    
    DSK_RADIO_REC_DEL_CHAN_FROM_FAVORITE,
    
	DSK_RADIO_REC_CLOSE,
	
/*DSK_RADIO ���ղ���------------------------------------------------------------------*/	
	
/*DSK_RADIO ���䲿��------------------------------------------------------------------*/
	
	
	/*! ��DSK_RADIO���书�� param1=0,param2=0*/
	DSK_RADIO_SEND_OPEN,
	/*! ���÷����Ƶ�ʽ��в��� param1=freq,param2=0*/
	DSK_RADIO_SEND_SET_FREQ_PLAY,
	/*! ���÷�������棨������������С�� param1=(__u32)gain,param2=0*/
	DSK_RADIO_SEND_SET_GAIN,
	/*! ��ȡ��������棨������������С�� param1=0,param2=(__u32)&gain*/
	DSK_RADIO_SEND_GET_GAIN,
	/*! �ر�DSK_RADIO���书�� param1=0,param2=0*/
	DSK_RADIO_SEND_CLOSE
	
/*DSK_RADIO ���䲿��------------------------------------------------------------------*/
}dsk_radio_cmd_t;

/*
**********************************************************************************************************************
*                                               dsk_radio_open
*
* author:        terry.zeng   
*
* date:             2009-12-01
*
* Description:      dsk_radio �򿪺���(��ʼ��FM���Ź���) 
*
* parameters:      
*
* return:           if success return dsk_radio handle
*                   if fail return NULL
* modify history: 
**********************************************************************************************************************
*/
__s32 dsk_radio_open(void);

__bool dsk_radio_is_open(void);

__pCBK_t dsk_radio_set_cb( dsk_radio_event_e event, __pCBK_t cb, void * ctx );

__pCBK_t dsk_radio_get_cb( dsk_radio_event_e event );

__s32 dsk_radio_mute(void);

__s32 dsk_radio_unmute(void);

__s32 dsk_radio_set_band(dsk_radio_band_mode_t band_mode);

__s32 dsk_radio_set_threshold(dsk_radio_threshold_mode_t threshold);

__s32 dsk_radio_set_audio_mode(dsk_radio_audio_mode_t audio_mode);

__s32 dsk_radio_get_band(__s32 *cur_band);

__s32 dsk_radio_get_audio_mode(__s32 *audio_mode);

__s32 dsk_radio_get_min_freq(__s32 *min_freq);

__s32 dsk_radio_get_max_freq(__s32 *max_freq);

/*
**********************************************************************************************************************
*                                               dsk_radio_control
*
* author:        terry.zeng   
*
* date:             2009-12-01
*
* Description:      dsk_radio���ܲ����ӿ�,����1һ��Ϊ�������������2Ϊ�������
*
* parameters:      
*
* return:           if success return 0
*                   if fail return -1
* modify history: 
**********************************************************************************************************************
*/
//__s32 dsk_radio_control(__s32 cmd,__s32 param1,__s32 param2);
/*
**********************************************************************************************************************
*                                               dsk_radio_close
*
* author:        terry.zeng   
*
* date:             2009-12-01
*
* Description:       �ر�dsk_radio���ͷ������Դ
*
* parameters:      
*
* return:           if success return 0
*                   if fail return -1
* modify history: 
**********************************************************************************************************************
*/
__s32 dsk_radio_close(void);

#endif
