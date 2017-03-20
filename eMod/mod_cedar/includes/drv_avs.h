/*
*********************************************************************************************************
*                                                    eMOD
*                                   the Easy Portable/Player Operation System
*                                              mod_mmp sub-system
*
*                                     (c) Copyright 2008-2009, Kevin.Z China
*                                              All Rights Reserved
*
* File   : drv_avs.h
* Version: V1.0
* By     : kevin.z
* Update : 2008-12-27 11:22
*********************************************************************************************************
*/
#ifndef _DRV_AVS_H_
#define _DRV_AVS_H_

//time delay of display show a picture
//#define DISPLAY_TIME_DELAY          (0x0000)        //delay 0 us
//#define FRM_SHOW_LOW_THRESHOLD      (10)            //low threshold is -10ms
//#define FRM_SHOW_HIGH_THRESHOLD     (20)            //high threshold is 20ms
//#define FRM_SHOW_SCALE_CNT          (3)             //һ֡����ʾʱ��ĵȷ���
//#define FRM_SHOW_SCALE_LOW          (1)             //low_threshold��ʱ��ȣ���FRM_SHOW_SCALE_CNTΪ��ĸ
//#define FRM_SHOW_SCALE_HIGH         (2)             //high_threshold��ʱ���



#define AUDIO_CODEC_IF              (0x01)          //audio codec interface
#define DIGITAL_AUD_IF              (0x02)          //digital audio(i2s) interface

#define AVS_VIDEO                   (0)             //set parameter for video module
#define AVS_AUDIO                   (1)             //set parameter for audio module
#define AVS_AUDVID                  (2)             //set parameter both for audio and video

#define AVS_ENABLE                  (1)             //enable audio and video sync operation
#define AVS_DISABLE                 (0)             //disable audio and video sync operation


typedef enum __AVS_TIME_TYPE
{                                   // define parameter type for get and set time
    DRV_AVS_TIME_TOTAL=0,           // total time, base + offset
    DRV_AVS_TIME_BASE,              // base time, video time start value
    DRV_AVS_TIME_OFFSET,            // time offset, play time offset
    DRV_AVS_TIME_

} __avs_time_type_t;

typedef enum tag_SyncMode
{
    DRV_AVS_SYNCMODE_V2A =0,           // ��Ƶ׷��Ƶ,����ƵΪ׼.
    DRV_AVS_SYNCMODE_A2V,              // ��Ƶ׷��Ƶ,����ƵΪ׼
    DRV_AVS_SYNCMODE_

}SyncMode;

typedef enum __AVS_DRIVER_COMMAND
{                                   // command list for av sync
    DRV_AVS_CMD_MODULE_START = 0x00,    //��ʼ������, ���߳�,��DRV_AVS_CMD_START֮ǰ����
    DRV_AVS_CMD_STOP,               // stop avsync,���߳�
    DRV_AVS_CMD_START,              // start avsync��һ��ýӿ�ֻ��������video,auido������������ֻ��avscounter�򿪣�����Ϊ����
    DRV_AVS_CMD_STOP_CLOCK,         // aux=AVS_VIDEO��,���ýӿ�Ŀǰֻ�����ر�video clock. audio��������. video clock�ر�ʱ��Ҫ��avscounter��.��DRV_AVS_CMD_ENABLE_CLOCK��֮ͬ����������ӿ�����Ҫ��avscounter��
    DRV_AVS_CMD_PAUSE,              // pause avsync
    DRV_AVS_CMD_CONTI,              // continue
    DRV_AVS_CMD_RESET,              // reset avsync
    DRV_AVS_CMD_FF,                 // fast forward
    DRV_AVS_CMD_RR,                 // fast reverse
    DRV_AVS_CMD_CLEAR_FFRR,         // clear fast forward/reverse
    DRV_AVS_CMD_GET_STATUS,         // get the status of avsync
    DRV_AVS_CMD_JUMP,                 // fast reverse
    DRV_AVS_CMD_CLEAR_JUMP,         // clear fast forward/reverse

    DRV_AVS_CMD_OPEN,       //������Դ
    DRV_AVS_CMD_CLOSE,      //�ͷ���Դ

    //�ص��ע��Щ�ӿڣ��漰��aply�Ĳ���
    DRV_AVS_CMD_GET_VID_TIME=0x20,  // get video time, based on ms,����ֵʵ������u32
    DRV_AVS_CMD_SET_VID_TIME,       // set video time, based on ms
    DRV_AVS_CMD_GET_AUD_TIME,       // get audio time, based on ms, aply��ʱ���ѻ���ɱ���֮ǰ��ʱ��
    DRV_AVS_CMD_SET_AUD_TIME,       // set audio time, based on ms, aply
    DRV_AVS_CMD_GET_CUR_TIME,       // get play time based on ms, ������aply, ���ų���ʱ��,offset, ������BaseTime
    
    DRV_AVS_CMD_GET_AUD_CACHE_TIME, // get audio cache time for skip video frame,ʱ���Ѿ�����ɱ���֮ǰ��ʱ��
    DRV_AVS_CMD_GET_AUDIO_CACHE_STAT,   // ��ѯdac buffer�е�audio���ݵ����������ret = 1:buffer�е�audio_data����;0:audio_data��û���ˡ�������ǰ��ʱ������
    DRV_AVS_CMD_CHK_AUDIO_PLAY_END, // check if all audio sample has play end, aux = 1,��ʾһ��ҪDAC BUFFERΪ�ղ���PLAY_END��aux = 0:��ʾDAC BUFFER������С��8k�Ϳ�����Ϊplay end�ˡ�����ʵ�����ǲ�ת��Ϊ����ǰ��
    DRV_AVS_CMD_SET_PCR,            // ����pcrpts��ֵ,��������У׼videoclock. һ����pcr�ж�һ��������֪ͨavsyncģ��. ��Ȼ�������жϴ�����������, ���Կ�����Ϊ�ýӿڱ�������ʵʱ��. EPDK_FAIL��ʾsetpcrû�гɹ�
            

    DRV_AVS_CMD_ENABLE_AVSYNC=0x40, // enable or disable audio video sync operation
    DRV_AVS_CMD_SET_FORCE_SYNC,     // set force sync, need adjust video clock
    
    DRV_AVS_CMD_REGIST_CLOCK,       // regist audio or video clock
    DRV_AVS_CMD_CHECK_CLOCK,        // check if audio or video clock is exist
    DRV_AVS_CMD_ENABLE_CLOCK,       // enable/disable audio clock or video clock�����漰��avs_state�ĸı䣬����һ��audio����������Ϊ��ʼ�ͽ���
    DRV_AVS_CMD_SET_VID_CLK_SPEED,  // set video clock speed(.../-3/-2/-1/1/2/3/...)
    DRV_AVS_CMD_GET_VID_VPS,        //0��ʾ�����ٶ�,10��ʾ����10%���ͷǱ�������µ�����������Ƚ�
    
    DRV_AVS_CMD_SET_APLYHDL,        //����aplyģ��ľ��,���ڲ�ѯ��ǰ���ų�������Ƶ���ݶ�Ӧ��ԭʼsample��.��startʱ����, stopʱaply�������Ϊ0
    DRV_AVS_CMD_SET_AUD_CHAN_CNT,   // set audio channel count
    DRV_AVS_CMD_SET_PCM_SAMPRATE,   // set audio sample rate, for calculate audio time
    DRV_AVS_CMD_SET_AUD_VPS,        //  set audio variable speed, for calculate audio time��aux = -40~100. ��Ϊavsync����Ҫ������Ƶ����,��ardr��aply����. avsync�鵽��sample��һ����ԭʼ��sample��,���ܱ������.���ʵ��������ӿ������������������ֵ,������Ƶʱ����,����ɱ�������µ�avͬ�����ܵ�.����Ƶ�Ѿ�û�й�ϵ��
    DRV_AVS_CMD_GET_AUD_VPS,        //  ���������audio render��Ķ���һ��, �ο�[PostProcessCom.h]�� PostProcessSt->VPS -40 ~ 100, mod_cedar.h�ж����AUDIO_VPS_SLOWEST���ǳ���10��.����0.1����Ϊ��λ
                                    // �������Ƶ����ô�ͱ������AudVps������Ƶʱ�ӵ�Ŀ���Ƶϵ�����Դﵽavͬ����Ŀ��. get_aud_vps, ret = -40~100
    DRV_AVS_CMD_SET_DACDEVHDL,      // ׼��ɾ��,set audio device handle, set or clear
    DRV_AVS_CMD_FORCE_DMA,          // ׼��ɾ��, ǿ������audio device��dma�����佫buffer�е���������. audio device����dma�������Զ���ԭ, ret = EPDK_OK/EPDK_FAIL;
    //    DRV_AVS_CMD_SET_WAIT_FLG,  //����Ҫ��ȴ��ı��,Ŀǰ��audio playbackģ����,video playbackģ���.aux = 1(��),0(���õ�)
    //    DRV_AVS_CMD_GET_WAIT_FLG, //��ѯaudio playbackģ���Ƿ���ͣ
    DRV_AVS_CMD_SET_ARDRHDL,        //����ardrģ��ľ��,��ΪavsyncҪʵ��audio׷video�Ļ���,��Ҫ����ardrģ��ı��ٹ��ܡ�
    DRV_AVS_CMD_SET_SYNCMODE,       //aux = SyncMode, DRV_AVS_SYNCMODE_V2A��
    
    DRV_AVS_CMD_SET_AV_END_FLAG=0x60,   // set audio or video end flag
    DRV_AVS_CMD_GET_AV_END_FLAG,        // get audio or video end flag
    DRV_AVS_CMD_SET_FFRR_FLAG,          // set flag if ff/rr is allowed, ��AVSYNCģ���Ƿ�����FFRR��ģ�鱾������νFFRR�Ƿ�������ʵ��VDEC��ADEC�Ƿ�����
    DRV_AVS_CMD_GET_FFRR_FLAG,          // get flag if ff/rr is allowed
    
    DRV_AVS_CMD_

} __avs_driver_command_t;


#endif /* _DRV_AVS_H_ */

