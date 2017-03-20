/*
*********************************************************************************************************
*                                                    ePDK
*                                    the Easy Portable/Player Develop Kits
*                                               hello world sample
*
*                                (c) Copyright 2006-2007, Steven.ZGJ China
*                                            All    Rights Reserved
*
* File    : drv_audio.h
* By      : kevin
* Version : V1.00
* Date    : 2009-4-6 18:34
*********************************************************************************************************
*/
#ifndef _DRV_AUDIO_H_
#define _DRV_AUDIO_H_

#include "typedef.h"
#include "cfgs.h"


#define AUDIO_DEVICE_VOLUME_MIN     (0)     /* ��Ƶ�豸������Сֵ               */
#define AUDIO_DEVICE_VOLUME_MAX     (30)    /* ��Ƶ�豸�������ֵ               */


typedef enum __AUDIO_DEV_CMD_SET
{
    AUDIO_DEV_CMD_NONE = 0,         /* ��Ƶ�豸�����������Ӧ                 */
    AUDIO_DEV_CMD_START,            /* ������Ƶ�豸                             */
    AUDIO_DEV_CMD_STOP,             /* ֹͣ��Ƶ�豸                             */
    AUDIO_DEV_CMD_PAUSE,            /* ��ͣ��Ƶ�豸                             */
    AUDIO_DEV_CMD_CONTINUE,         /* ������Ƶ�豸                             */

    AUDIO_DEV_CMD_GET_SAMPCNT=20,   /* ��ȡ��Ƶ�Ĳ�������, return = sample count    */
    AUDIO_DEV_CMD_SET_SAMPCNT,      /* ������Ƶ�Ĳ�������, aux = sample count       */
    AUDIO_DEV_CMD_GET_PARA,         /* �����û�����, pbuffer = (__audio_dev_para_t *)   */
    AUDIO_DEV_CMD_SET_PARA,         /* ��ȡ�û�����, pbuffer = (__audio_dev_para_t *)   */
    AUDIO_DEV_CMD_GET_VOLUME,       /* ��ȡ����, pbuffer = __audio_device_type_t, return = volume                   */
    AUDIO_DEV_CMD_SET_VOLUME,       /* ��������, aux = volume, pbuffer = __audio_device_type_t, return = new volume */

    AUDIO_DEV_CMD_REG_USERMODE=40,  /* ��Ƶ�û�ע���û�ģʽ, aux = __audio_play_user_t / __audio_rec_user_t */
    AUDIO_DEV_CMD_CHANGE_IF,        /* �û��ı䲥��ģʽ��aux = AUDIO_DEV_IF_IIS, AUDIO_DEV_IF_CODEC */
    AUDIO_DEV_CMD_FLUSH_BUF,        /* �����Ƶ�豸�û��������ڵ���Ƶ����           */
    AUDIO_DEV_CMD_QUERY_BUFSIZE,    /* ��ѯ�û���������ؿռ����,aux = __audio_dev_query_buf_size_type_t   */
    AUDIO_DEV_CMD_RESIZE_BUF,       /* ����ָ���û��������Ĵ�С, aux = buffer size, be careful for use      */
    AUDIO_DEV_CMD_CLOSE_DEV,        /* �ر����豸,��ر�Ӳ���ӿڣ����ã�����, aux = __audio_device_type_t   */
    AUDIO_DEV_CMD_READ_DATA,        /* ����Ƶ�豸��ȡ���ݣ�aux=data size, pbuffer=buffer    */
    AUDIO_DEV_CMD_WRITE_DATA,       /* ����Ƶ�豸д�����ݣ�aux=data size, pbuffer=buffer    */
    AUDIO_DEV_CMD_DATA_FINISH,      /* ֪ͨ��Ƶ�豸��ǰ�û��������Ѿ�������� */

    AUDIO_DEV_CMD_SET_PROTECT_VOL=60,  /* ������������ֵ, aux = max volume value,(0-16)        */
    AUDIO_DEV_CMD_GET_PROTECT_VOL,  /* ��ȡ��������ֵ, return = current max volume value    */

    AUDIO_DEV_CMD_SET_CHAN_MODE,    /* ��������ģʽ(���ҡ�������), aux = __audio_dev_chan_mode_t, return = EPDK_OK/EPDK_FAIL  */
    AUDIO_DEV_CMD_GET_CHAN_MODE,    /* ��ȡ����ģʽ, return = __audio_dev_chan_mode_t               */

    AUDIO_DEV_CMD_ENTER_STANDBY=80, /* ���� standbyģʽ       */
    AUDIO_DEV_CMD_EXIT_STANDBY,     /* �˳� standbyģʽ       */

    AUDIO_DEV_CMD_REG_CALLBACK= 90, /* ע��һ���ص�����       */
    AUDIO_DEV_CMD_UNREG_CALLBACK,   /* ж��һ���ص�����       */
    AUDIO_DEV_CMD_GET_INTERFACE,    /* ȡ�õ�ǰ��Ƶ���ŵĽӿ�  return = __audio_dev_interface_t */

    AUDIO_DEV_CMD_SET_PLAYMODE= 100,/* ���ṩ��RECʹ�ã�����¼��ʱ���Ƿ���Ҫͬʱ���ţ�Ĭ�ϲ�����, aux = __audio_dev_rec_mode_t        */
                                    /* ���ṩ��PLAYʹ�ã�Ŀǰ����SPDIFģʽ���Ƿ���RAWDATAģʽ     aux = __audio_dev_spdif_playmode_t  */
    AUDIO_DEV_CMD_SET_DAC_MAX_GAIN = 110,//use "B:\\AUDIO\\PLAY" to set maximum value of Power Amplifier gain to @aux, must <=63
    AUDIO_DEV_CMD_GET_DAC_MAX_GAIN,      //get maximum value of Power Amplifier gain with "B:\\AUDIO\\CTRL" 
    
    AUDIO_DEV_CMD_SET_SW_VOL_MAX,      //use "B:\\AUDIO\\CTRL" to change maximum value of software volume to @aux, this value is the maximum value for end users
    AUDIO_DEV_CMD_GET_SW_VOL_MAX,      //get maximum value of software volume with "b:\\audio\\rec", this value is the maximum value which end users use

    AUDI_DEV_CMD_SET_HPCOM_DRIVE_MODE,      //set HeadPhone driver mode to @aux,  0 means AC mode, 1 means DC mode, other values are reserved(use "B:\\AUDIO\\CTRL")
    AUDIO_DEV_CMD_SWAP_OUTPUT_CHANNELS,     //Swap output channels, if @aux is 1, left dac output to right channel and right dac to left channel. if 0, not swap(use "B:\\AUDIO\\CTRL")

    AUDIO_DEV_CD_SET_USE_USER_VOLUME_MAP,	/*ʹ���û���������ӳ��pbuffer = ����ΪAUDIO_DEVICE_VOLUME_MAX+1�����飬�����±������value,Ŀǰ��֧��record����ӳ��*/
    
    AUDIO_DEV_CMD_MUTE,//aux: 0: not mute, 1:mute
    AUDIO_DEV_CMD_SET_MIC_PRE_GAIN,     //set MIC pregain val: 0<=aux<=3 0db 35db 38db 41db   default:41db
    AUDIO_DEV_CMD_MIC_PRE_GAIN_ONOFF,   //MIC pregain onoff: aux=0:off  aux=1:on     default:1
    AUDIO_DEV_CMD_ENABLE_ECHO=120,      /*�����Ƿ�֧�ֻ��죬aux:Ϊ0����֧�֣�Ϊ1����֧�֣�Ĭ��֧��*/
    AUDIO_DEV_CMD_SET_ECHO_BUF_MODE,    /*����BUFFERģʽ��aux:0�������Զ����buffer(¼��Ӧ��)��Ϊ1�����Զ����buffer(��¼����Ӧ��)��Ĭ�ϲ����Զ����*/
    AUDIO_DEV_CMD_SET_ECHO_REC_GAIN,    /*(-20, 20)*/
    AUDIO_DEV_CMD_GET_ECHO_REC_GAIN,
    AUDIO_DEV_CMD_SET_ECHO_DELAY_NUM,   /*aux: 0<= aux <= 10 ���û�����ʱ������Ĭ��Ϊ3*/
    AUDIO_DEV_CMD_SET_ECHO_DELAY_TIME,    /*aux:10<= aux <= 3000,Ĭ��Ϊ100���Ժ���Ϊ��λ*/
    AUDIO_DEV_CMD_ECHO_SET_PITCH_MODE,  /*�ýӿ���ʱ���ã�CPU�ܲ��������ñ��ģʽ:*/                                        
    AUDIO_DEV_CMD_ENABLE_ECHO_CANCEL,  /*��������ʹ�ܣ�aux,0:�ر� 1:��*/
    AUDIO_DEV_CMD_

} __audio_dev_cmd_set_t;


typedef enum __AUDIO_DEVICE_TYPE
{
    AUDIO_DEV_NONE=0,           /* δ������Ƶ�豸                               */
    AUDIO_DEV_PLAY,             /* ��Ƶ�����豸                                 */
    AUDIO_DEV_REC,              /* ��Ƶ�����豸                                 */
    AUDIO_DEV_FM,               /* FM�豸                                       */
    AUDIO_DEV_

} __audio_device_type_t;


typedef enum __AUDIO_PLAY_USER
{
    AUDIO_PLAY_USR_NONE =0,     /* ��Ƶ�����û�δ����ģʽ                       */
    AUDIO_PLAY_USR_MASTER,      /* �û�ʹ��masterģʽ������Ƶ                   */
    AUDIO_PLAY_USR_SLAVE,       /* �û�ʹ��slaveģʽ������Ƶ                    */
    AUDIO_PLAY_USR_KEY,         /* �û�ʹ��key tone������Ƶ                     */
    AUDIO_PLAY_USR_

} __audio_play_user_t;

typedef enum __AUDIO_REC_USER
{
	AUDIO_REC_USR_NONE = 0,    /* ¼���û�δ����ģʽ                           */
	AUDIO_REC_USR_LINEIN,      /* ¼���û�LINE INģʽ                          */
	AUDIO_REC_USR_FMIN,        /* ¼���û�FM INģʽ                            */
	AUDIO_REC_USR_MIC1,         /* ¼���û�MIC1ģʽ                              */
	AUDIO_REC_USR_MIC=AUDIO_REC_USR_MIC1,         /* ¼���û�MICģʽ ,��ǰ����,modify by hot.lee                             */
	AUDIO_REC_USR_MIC2,         /* ¼���û�MIC2ģʽ                              */
	AUDIO_REC_USR_MIC1_AND_MIC2, /*¼���û�������MIC1��������MIC2*/
	AUDIO_REC_USR_MIXER,       /* ¼���û�MIXERģʽ                            */
	AUDIO_REC_USR_
} __audio_rec_user_t;

typedef enum __AUDIO_CHN_MODE
{
    AUDIO_CHN_MODE_NONE = 0,    /* ��Ƶͨ��ģʽδ����                           */
    AUDIO_CHN_MODE_MONO,        /* ��ͨ��ģʽ                                   */
    AUDIO_CHN_MODE_STEREO,      /* ������ģʽ                                   */
    AUDIO_CHN_MODE_

} __audio_chn_mode_t;


//�û��������ݽṹ
typedef struct __AUDIO_DEV_PARA
{
    __u8        chn;            /* ��Ƶͨ����, __audio_chn_mode_t               */
    __u8        bps;            /* ��Ƶ������λ�� 8/16/24                     */
    __u32       fs;             /* ��Ƶ������                                   */

} __audio_dev_para_t;


typedef struct __AUDIO_IIS_TO_HDMI
{
	__u16  mclk_div;            /* Mclock��Ƶϵ��                               */
	__u8   channel;             /* ��Ƶͨ����                                   */
	__u8   bitpersample;        /* ��Ƶ������λ��                               */

}__audio_iis_to_hdmi_t;


typedef	struct __AUDIO_PCM_INFO
{
//input infor
	__s32	SampleRate;
	__u16*	PCMPtr;
	__s32	PcmLen;//sample num;    inputA��inputB�ڵ���ǰ��Ҫmix��sample��,���ú���ʣ��û��mix��sample��.
	__s32	Chan;  //channel num

//reserved
    int		preamp;//-20 -- 20 db	
}__audio_pcm_info_t;



typedef enum __AUDIO_DEV_INTERFACE
{
    AUDIO_DEV_IF_NONE=0,        /* δ������Ƶ�豸�ӿ�                           */
    AUDIO_DEV_IF_CODEC,         /* ����audio codec��Ϊ��Ƶ�豸�ӿ�              */
    AUDIO_DEV_IF_IIS,           /* ����IIS������Ϊ��Ƶ�豸�ӿ�                  */
    AUDIO_DEV_IF_SPDIF,         /* ����SPDIF������Ϊ��Ƶ�豸�ӿ�                */
	AUDIO_DEV_IF_HDMI,			/* ����HDMI AUDIOϵͳ�ӿ�*/
	
    AUDIO_DEV_INTERFACE_

} __audio_dev_interface_t;


typedef enum __AUDIO_DEV_QUERY_BUF_SIZE_TYPE
{
    AUDIO_DEV_QUERY_BUF_SIZE_NONE = 0,      /* δ����Ҫ��ȡ�ռ������           */
    AUDIO_DEV_QUERY_BUF_SIZE_DATA,          /* ��ȡ�������ڵ�����size           */
    AUDIO_DEV_QUERY_BUF_SIZE_FREE,          /* ��ȡ�������ڵĿ��пռ�size       */
    AUDIO_DEV_QUERY_BUF_SIZE_TOTAL,         /* ��ȡ�������ܿռ�                 */

    AUDIO_DEV_QUERY_BUF_SIZE_

} __audio_dev_query_buf_size_type_t;


typedef enum __AUDIO_DEV_CHAN_MODE
{
    AUDIO_DEV_CHANNEL_STEREO = 0,           /* ������ģʽ                       */
    AUDIO_DEV_CHANNEL_LEFT,                 /* ����ģʽ                         */
    AUDIO_DEV_CHANNEL_RIGHT,                /* ����ģʽ                         */
    AUDIO_DEV_CHANNEL_EXIT,
    AUDIO_DEV_CHANNEL_

} __audio_dev_chan_mode_t;

typedef enum __AUDIO_DEV_REC_PLAYMODE
{
    AUDIO_DEV_REC_NOPLAY = 0,               /* ¼��ʱ����Ҫ����               */
    AUDIO_DEV_REC_WITHPLAY,                 /* ¼��ʱ����Ҫ����                 */

    AUDIO_DEV_REC_

} __audio_dev_rec_playmode_t;

typedef enum __AUDIO_DEV_SPDIF_PLAYMODE
{
    AUDIO_DEV_SPDIF_NORMALPLAY = 0,         /* SPDIFʱ�򲥷Ž�������            */
    AUDIO_DEV_SPDIF_RAWDATAPLAY,            /* SPDIFʱ�򲥷�RWDATA����          */

    AUDIO_DEV_SPDIF_

} __audio_dev_spdif_playmode_t;


//����AUDIO_PLAY_AUX_WAV_FILE��ģʽ
typedef enum __AUDIO_PLAY_AUX_WAV_MODE
{
    AUDIO_PLAY_AUX_WAV_MODE_APPEND,//�����ǰ�Ѿ���δ������ϵ�AUX WAV���ݣ���ǰ����׷�ӵ������ݵĺ��沥��
    AUDIO_PLAY_AUX_WAV_MODE_TRUNCATE,//�����ǰ�Ѿ���δ������ϵ�AUX WAV���ݣ��������ǰ���ݣ����ŵ�ǰ���õ�����
    AUDIO_PLAY_AUX_WAV_MODE_MIX,//�����ǰ�Ѿ���δ������ϵ�AUX WAV���ݣ�����ǰ�����ݺ͵�ǰ���õ����ݽ��л���
    AUDIO_PLAY_AUX_WAV_MODE_IGNORE,//�����ǰ�Ѿ���δ������ϵ�AUX WAV���ݣ�����Ե�ǰ���õ�����
    AUDIO_PLAY_AUX_WAV_MODE_MAX
}__audio_play_aux_wav_mode_t;


#endif /* _DRV_AUDIO_H_ */

