/*
*********************************************************************************************************
*                                                    eMOD
*                                   the Easy Portable/Player Operation System
*                                               mod_mmp sub-system
*
*                                (c) Copyright 2010-2011, Xiaoyan.Liu China
*                                               All Rights Reserved
*
* File   : mod_dvdnavi.h
* Version: V1.0
* By     : Xiaoyan. Liu
* Date   : Xiaoyan.Liu  1010_06_01
*********************************************************************************************************
*/
#ifndef _MOD_DVDNAVI_H_
#define _MOD_DVDNAVI_H_

#include "typedef.h"
#include "cfgs.h"

#define YULAN_TAG_INF_SIZE      (256)      /* buffer size for audio tag information                    */
#define YULAN_FPATH_SIZE        (1024)    /* ý���ļ�·�����������ֵ                                 */

//define variable for audio render interface
#define MAX_SPEC_GROUP_NUM      (10)    /* Ƶ����������������Ӧ�ó���ο���ֵ����ռ�                 */
#define USER_DEFINE_EQ_SIZE     (10)    /* �û��Զ���EQʱ����Ҫ�����һ���������                       */

#define AUDIO_VPS_SLOWEST       (-4)    /* ��Ƶ���ٲ��ŵ������ٶ�ֵ                                     */
#define AUDIO_VPS_NOMAL         (0)     /* ��Ƶ���������ٶ�ֵ                                           */
#define AUDIO_VPS_FASTEST       (10)    /* ��Ƶ���ٲ��ŵ�����ٶ�ֵ                                     */


#define MIN_VOLUME_VAL          (0)     /* ������Сֵ����ʱΪ����״̬                                   */
#define MAX_VOLUME_VAL          (30)    /* �������ֵ��ֻ����������Χ�ڵ�ֵ����Ϊ��Ч������Ϊ�Ƿ�ֵ     */

#define YULAN_FASTEST_FRSPEED   (128)   /* ��������˵������                                         */
#define YULAN_SLOWEST_FRSPEED   (1)     /* ��������˵���С����                                         */


#define YULAN_FRPIC_HOLDTIME_LEAST      (30)    /* ���������״̬�£�ͼƬ��ʾͣ����Сʱ�䣬(30ms)       */
#define YULAN_FRPIC_HOLDTIME_LARGEST    (5000)  /* ���������״̬�£�ͼƬ��ʾͣ�����ʱ�䣬(5000ms)     */

#define YULAN_MAX_LANG_CHAR_SIZE             (32)
#define YULAN_MAX_AUDIO_STREAM_NUM           (8)
#define YULAN_MAX_SUBTITLE_STREAM_NUM        (32)
#define YULAN_MAX_MENU_BTN_NUM               (36)
#define YULAN_MAX_PALETTE_CNT                 16
#define YULAN_MAX_CMD_LEN                     8

typedef enum __YULAN_CMD_SET            /* ý�岥���м�������б�(IOCtrol)                                         */
{
    YULAN_CMD_SET_MEDIAFILE=0x0100,     /* ���ô����ŵ�ý���ļ�·�����ϵ���Ϣ, pbuffer = (__yulan_media_file_inf *) */
    YULAN_CMD_GET_MESSAGE_CHN,          /* ��ȡ������Ϣͨ����return = __krnl_event_t*, (__yulan_feedback_msg_t)     */
    YULAN_CMD_GET_ERROR_TYPE,           /* ��ȡý�岥�����Ĵ�������, error in __yulan_feedback_msg_t                */

    YULAN_CMD_PLAY=0x0200,              /* ��������                                                                 */
    YULAN_CMD_STOP,                     /* ֹͣ����                                                                 */
    YULAN_CMD_PAUSE,                    /* ��ͣ����                                                                 */
    YULAN_CMD_FF,                       /* ���                                                                     */
    YULAN_CMD_REV,                      /* ����                                                                     */
    YULAN_CMD_GET_STATUS,               /* ��ȡ����ģʽ     return = __yulan_status_t                               */

    YULAN_CMD_GET_TOTAL_TIME=0x300,     /* ��ȡ������ʱ��   return = total time(ms),������ֵΪ-1������ʾ�ܲ���ʱ��*/
    YULAN_CMD_GET_CUR_TIME,             /* ��ȡ��ǰ����ʱ�� return = current play time(ms)                           */
    YULAN_CMD_GET_TAG,                  /* ��ȡ��ǰ����ʱ���ı�ǩ��Ϣ                                              */
    YULAN_CMD_SET_FRSPEED,              /* ���ÿ�������ٶ� aux    = YULAN_SLOWEST_FRSPEED ~ YULAN_FASTEST_FRSPEED  */
    YULAN_CMD_GET_FRSPEED,              /* ��ȡ��������ٶ� return = speed                                           */

    YULAN_CMD_GET_ABSTYPE=0x0400,       /* ��ȡ��ǰ���ڲ��ŵ���Ƶ������ʽ return = __yulan_audio_fmt_t               */
    YULAN_CMD_GET_AUDBPS,               /* ��ȡ��Ƶ�������� return = bps                                             */
    YULAN_CMD_GET_SAMPRATE,             /* ��ȡ��Ƶ������   return = sample rate                                     */
    YULAN_CMD_SET_CHN,                  /* ������������     aux = __yulan_audio_chn_t                                */
    YULAN_CMD_GET_CHN,                  /* ��ȡ��Ƶ��ͨ����                                                          */
    YULAN_CMD_SET_VOL,                  /* ��������         aux = MIN_VOLUME_VAL~MAX_VOLUME_VAL, return = new vol    */
    YULAN_CMD_GET_VOL,                  /* ��ȡ����         return = volume                                          */
    YULAN_CMD_VOLUP,                    /* ����ֵ�ϵ�һ��                                                            */
    YULAN_CMD_VOLDOWN,                  /* ����ֵ�µ�һ����                                                          */
    YULAN_CMD_SET_EQ,                   /* ����EQ           aux = __yulan_audio_eq_t                                 */
    YULAN_CMD_GET_EQ,                   /* ��ȡEQ           return = __yulan_audio_eq_t                              */
  //YULAN_CMD_SET_VPS,                  /* ���ñ��ٲ����ٶ� aux    = -4~10��=0-normal; <0-slow; >0-fast              */
  //YULAN_CMD_GET_VPS,                  /* ��ȡ���ٲ����ٶ� return = -4~10                                           */
  //YULAN_CMD_SET_AB_A,                 /* ����A-B���ŵ���ʼ��(A��)                                     */
  //YULAN_CMD_SET_AB_B,                 /* ����A-B���ŵĽ�����(B��)                                     */
  //YULAN_CMD_SET_AB_LOOPCNT,           /* ����A-B���ŵ�ѭ������                                        */
  //YULAN_CMD_CLEAR_AB,                 /* ȡ��A-B����                                                  */
    YULAN_CMD_SEL_AUDSTREAM,            /* ѡ����Ƶ��. aux = �����±�, ret = EPDK_OK/EPDK_FAIL����ʾ�Ƿ�����͸�YUAN.������ʾYULAN����ɹ� */
    YULAN_CMD_GET_AUDSTREAM,            /* �õ���ǰ���ڲ��ŵ���Ƶ�����±��, ret = suffix or -1(if not find)          */
    YULAN_CMD_GET_AUDSTREAM_PROFILE,    /* ��ȡ������Ƶ����˵����Ϣ:
                                            pbuffer = __audstream_profile_t, app malloc it.
                                            return = EPDK_OK or EPDK_FAIL*/
    YULAN_CMD_GET_VBSTYPE=0x0500,       /* ��ȡ��Ƶ������ʽ                                                           */
    YULAN_CMD_GET_VIDBITRATE,           /* ��ȡ��Ƶ��������                                                           */
    YULAN_CMD_GET_VIDFPS,               /* ��ȡͼ��֡��     return = frame rate                                       */
    YULAN_CMD_GET_FRAMESIZE,            /* frame size       return = (width << 16) + height                           */
    YULAN_CMD_SET_VID_LAYERHDL,         /* ������Ƶͼ���� pbuffer = video_layer_hdl                                 */
    YULAN_CMD_SET_VID_WINDOW,           /* ������Ƶ�������� pbuffer = (__rect_t *)                                    */
    YULAN_CMD_GET_VID_WINDOW,           /* ��ȡ��Ƶ�������� return = (__rect_t *)                                     */
    YULAN_CMD_SET_VID_SHOW_MODE,        /* ������Ƶͼ����ʾ��ģʽ��aux = __yulan_vide_window_ratio_mode_t,
                                           return = ���ú�ĵ�ǰ��ʾģʽ                                              */
    YULAN_CMD_GET_VID_SHOW_MODE,        /* ��ȡ��Ƶͼ����ʾģʽ��return = __yulan_vide_window_ratio_mode_t            */
    YULAN_CMD_SWITCH_VID_SHOW,          /* �򿪻�ر�ͼƬ��ʾ��aux = 0:�ر�ͼƬ��ʾ��aux = 1:��ͼƬ��ʾ             */
    YULAN_CMD_SET_FRPIC_SHOWTIME,       /* ���ÿ������״̬�£���Ƶ֡��ʾ�ĳ���ʱ��                                   */
    YULAN_CMD_GET_FRPIC_SHOWTIME,       /* ��ȡ�������״̬�£�ͼƵ֡��ʾ�ĳ���ʱ��                                   */

    YULAN_CMD_GET_LBSTYPE=0x0600,       /* ��ȡ��Ļ������ʽ                                                           */
    YULAN_CMD_GET_SUB_INFO,             /* ��ȡ��Ļ����Ŀ��Ϣ, aux = time, pbuffer = __yulan_get_sub_inf_t,
                                           (return = __yulan_subtitle_item_t *)                                       */
    YULAN_CMD_GET_SUBTITLE_PROFILE,              /* ��ȡ������Ļ��˵����Ϣ:���磬���ڵ�stream_idx,���Ե�
                                            pbuffer = __subtitle_profile_t, app malloc it
                                            return = EPDK_OK or EPDK_FAIL*/
    YULAN_CMD_SELECT_SUBTITLE,          /* ѡ��һ����Ļ���ţ������л���Ļ, aux:��Ļ�±�in __subtitle_profile_t        */
    YULAN_CMD_GET_SUBTITLE,             /* �õ���ǰ���ڲ�����Ļ���±꣬if not find, return -1                         */

    YULAN_CMD_CAPTURE_PIC=0x0700,       /* ��ͼ, pbuffer = __yulan_pic_info_t     */
    YULAN_CMD_ASK_PIC_BUFSIZE,          /* ��YULAN��ѯ��Ҫ����buffer�Ա�APP���䣬pbuffer = __yulan_pic_info_t, ret = size, -1����ʧ��*/
    YULAN_CMD_BACK_LAST_MENU   = 0x0800,    /*������һ��˵� */
    YULAN_CMD_BACK_MAIN_MENU,               /*�������˵�     */
    YULAN_CMD_CHECK_BUTTON,            /*���button �Ƿ���ڣ�return EPDK_OK/EPDK_FAIL����ΪEPDK_OK,��ʾ�ɹ���⵽button*/
    YULAN_CMD_GET_BUTTON,              /*���btn����Ϣ�pbuffer = (__YULAN_menuBtn_info_t *)*/
    YULAN_CMD_SEL_UPPER_BUTTON,        /*���û��԰�ť�Ĳ��������м���tͬʱ����µİ�ť��λ����Ϣ buffer = (__yulan_menuBtn_pos_t *)*/
    YULAN_CMD_SEL_LOWER_BUTTON,        /*���û��԰�ť�Ĳ��������м���tͬʱ����µİ�ť��λ����Ϣ buffer = (__yulan_menuBtn_pos_t *)*/
    YULAN_CMD_SEL_LEFT_BUTTON,         /*���û��԰�ť�Ĳ��������м���tͬʱ����µİ�ť��λ����Ϣ buffer = (__yulan_menuBtn_pos_t *)*/
    YULAN_CMD_SEL_RIGHT_BUTTON,        /*���û��԰�ť�Ĳ��������м���tͬʱ����µİ�ť��λ����Ϣ buffer = (__yulan_menuBtn_pos_t *)*/
    YULAN_CMD_PRESS_BUTTON,            /*���û��԰�ť�Ĳ��������м���t                                                             */
    YULAN_CMD_GOTO_CHAPTR,             /*��ת��ĳ��chapter���ţ� aux=�û�������½ںţ�(>=1)                            */
    YULAN_CMD_GOTO_TIME,               /*��ת��ĳ��ʱ�̽��в���,aux = �û������ʱ�䣬��λs                             */
	YULAN_CMD_GOTO_TITLE,              /*��ת��ĳ��title���в��ţ�aux=�û������title����>=1                            */
    YULAN_CMD_GET_CUR_CHAPTER,         /*��ȡ��ǰ���ڲ��ŵ�chapter��                                                    */
    YULAN_CMD_GET_CUR_TITLE,           /*��ȡ��ǰ���ڲ��ŵ�title��                                                      */
    YULAN_CMD_GET_TOTAL_CHAPTER,       /*��ȡ��ǰtitle������chapter��                                                   */
    YULAN_CMD_GET_TOTAL_TITLE,         /*��ȡ��ǰ��Ƭ������title��                                                      */
    YULAN_CMD_GET_PIC_SHOW_INFO,       /*��õ�ǰͼ�����ʾ��Ϣ                                                         */
    YULAN_CMD_SET_PLAY_MODE,           /*���ò���ģʽ:�ظ����ŵ�ǰtitle���ظ�����chapter ��ر��ظ�����                 */
    YULAN_CMD_GET_PLAY_MODE,
    YULAN_CMD_GET_CUR_ANGLE,           /*��ȡ��ǰ���ڲ��ŵ�angle��                                                      */
    YULAN_CMD_GET_TOTAL_ANGLE,         /*��ȡ��ǰtitle������angle��                                                     */
    YULAN_CMD_

} __yulan_cmd_set_t;


typedef enum __YULAN_FEEDBACK_MSG           /* ����ý�岥���м��������Ӧ�ó������Ϣ����               */
{
    YULAN_ERROR_NOP                 =   0,  /* ����Ϣ                                                   */

    //YULAN feed back error type
    YULAN_ERROR_OPEN_FILE_FAIL      = -20,  /* �򿪵�ǰý���ļ�ʧ��                                     */
    YULAN_ERROR_FILE_FMT_UNKNOWN,           /* �޷�ʶ��ǰý���ļ���ʽ                                 */
    YULAN_ERROR_FILE_NOT_SUPPORT,           /* ��ǰý���ļ���֧��                                       */
    YULAN_ERROR_FILE_DATA_ERROR,            /* ý���ļ����ݴ��޷�����                                 */
    YULAN_ERROR_NO_FILE_INPUT,              /* δ����Ҫ���ŵ�ý���ļ�                                   */
    YULAN_ERROR_READ_FILE_FAIL,             /* ��ȡ�ļ�����ʧ��                                         */

    YULAN_ERROR_START_PLG_FAIL      = -40,  /* �������ʧ��                                             */
    YULAN_ERROR_ADEC_ERROR,                 /* ��Ƶ�������������                                     */
    YULAN_ERROR_VDEC_ERROR,                 /* ��Ƶ�������������                                     */
    YULAN_ERROR_TDEC_ERROR,                 /* ��Ļ�������������                                     */

    YULAN_ERROR_PIC_SIZE_TOO_LARGE  = -60,  /* ͼƬ�ߴ�̫���޷�֧�ֽ���                               */
    YULAN_ERROR_REQ_MEMORY_FAIL,            /* �����ڴ�ʧ��                                             */

    YULAN_ERROR_UNKOWN              = -80,  /* δ֪����                                                 */

    //YULAN feed back status type
    YULAN_FEDBAK_NO_ERROR           =  1,   /* ����Ϣ                                                   */
    YULAN_FEDBAK_PLAY_END,                  /* ����ý���ļ�����                                         */

    YULAN_FEDBAK_

} __yulan_feedback_msg_t;


typedef enum __YULAN_VID_WINDOW_RATIO_MODE
{
    YULAN_VID_WINDOW_NOTCARE = 0x00,        /* ������ͼƬ��ʾ�������Ե�ǰ���õı���         */
    YULAN_VID_WINDOW_ORIGINAL,              /* ��ͼƬԭʼ��С�ڴ�������ʾ�������������     */
    YULAN_VID_WINDOW_BESTSHOW,              /* ��ͼƬ����ı�����������������ʾ��ͼƬ������ */
    YULAN_VID_WINDOW_FULLSCN,               /* �Դ��ڵı�������ͼƬ����������ʾ�����ܻ���� */
    YULAN_VID_WINDOW_4R3MODE,               /* ǿ����4:3��ģʽ������ʾ��Ƶͼ��ͼ������  */
    YULAN_VID_WINDOW_16R9MODE,              /* ǿ����16:9��ģʽ������ʾ��Ƶͼ��ͼ������ */
    YULAN_VID_WINDOW_CUTEDGE,               /* ͼƬ�����кڱߣ���õ�                       */

    YULAN_VID_WINDOW_

}__yulan_vide_window_ratio_mode_t;

typedef enum __YULAN_STATUS             /* the status os YULAN
                  */
{
    YULAN_STAT_PLAY=0,                  /* play                                 */
    YULAN_STAT_PAUSE,                   /* pause                                */
    YULAN_STAT_STOP,                    /* stop                                 */
    YULAN_STAT_FF,                      /* fast forward                         */
    YULAN_STAT_RR,                      /* fast backward                        */
    YULAN_STAT_PLAYEND,                /* play end,Ӧ�ò���Ҫ���¼��total time����Ļ�ȵ���Ϣ*/
    YULAN_STAT_

} __yulan_status_t;

typedef enum __YULAN_SELECT_BUTTON       /*ѡ��ť                             */
{
    YULAN_SEL_UPPER_BTN = 0,            /* select the upper button              */
    YULAN_SEL_LOWER_BTN,                /* select the lower button              */
    YULAN_SEL_LEFT_BTN,                 /* select the left button               */
	YULAN_SEL_RIGHT_BTN,                /* select the right button              */
	YULAN_SEL_PRESS_BTN,                /* press the button                     */
	YULAN_SEL_BUTTON_
} __yulan_sel_btn_t;

typedef enum __YULAN_AUDIO_FMT
{
    YULAN_ABS_TYPE_NONE = 0,            /* û����Ƶ��                           */
    YULAN_ABS_TYPE_UNKNOWN,             /* ����Ƶ��,���޷�ʶ��                  */

    YULAN_ABS_TYPE_MPEG12_LAYER1,
    YULAN_ABS_TYPE_MPEG12_LAYER2,
    YULAN_ABS_TYPE_MPEG12_LAYER3,
    YULAN_ABS_TYPE_MPEG_AAC_LC,
    YULAN_ABS_TYPE_AC3,                 // AC-3
    YULAN_ABS_TYPE_DTS,                 // DTS audio for DVD-Video
    YULAN_ABS_TYPE_LPCM_V,
    YULAN_ABS_TYPE_LPCM_A,
    YULAN_ABS_TYPE_ADPCM,
    YULAN_ABS_TYPE_PCM,                 // pcm
    YULAN_ABS_TYPE_

} __yulan_audio_fmt_t;


typedef enum __YULAN_VIDEO_FMT
{
    YULAN_VBS_TYPE_NONE = 0,            /* û����Ƶ��                           */
    YULAN_VBS_TYPE_UNKNOWN,              /* ����Ƶ��,���޷�ʶ��                  */

    YULAN_VBS_TYPE_MPEG1_ES,
    YULAN_VBS_TYPE_MPEG2_ES,
    YULAN_VBS_TYPE_

} __yulan_video_fmt_t;

typedef enum __YULAN_LYRIC_FMT
{
    YULAN_LRC_TYPE_NONE=0,              /* û����Ļ/�����                      */
    YULAN_LRC_TYPE_NOTSPT,              /* ����Ļ/�����,���޷�ʶ��             */
    YULAN_LRC_TYPE_ISUB,                /* ��ǶSUB��ʽ��Ļ��                    */
    YULAN_LRC_TYPE_MSUB,                /* vob�˵���Ļ��                        */
    YULAN_LRC_TYPE_

} __yulan_lyric_fmt_t;


typedef enum __YULAN_PIC_FMT
{
    YULAN_PIC_TYPE_NONE=0,              /* û��ͼƬ��                           */
    YULAN_PIC_TYPE_NOTSPT,              /* ��ͼƬ��,���޷�ʶ��                  */

    YULAN_PIC_TYPE_

} __yulan_pic_fmt_t;


//�û��Զ�����Ч�˲�����Ƶ�������û���Ҫ�����趨��Ƶ�ε��˲�ϵ��
#define USR_EQ_BAND_CNT             (10)
#define USR_EQ_NEGATIVE_PEAK_VALUE  (-12)
#define USR_EQ_POSITIVE_PEAK_VALUE  (+12)

typedef enum __YULAN_AUDIO_EQ           /* ��Ч���Ͷ���                         */
{
    YULAN_AUD_EQ_TYPE_NORMAL=0,         /* ��Ȼ                                 */
    YULAN_AUD_EQ_TYPE_DBB,              /* �ص���                               */
    YULAN_AUD_EQ_TYPE_POP,              /* ����                                 */
    YULAN_AUD_EQ_TYPE_ROCK,             /* ҡ��                                 */
    YULAN_AUD_EQ_TYPE_CLASSIC,          /* �ŵ�                                 */
    YULAN_AUD_EQ_TYPE_JAZZ,             /* ��ʿ                                 */
    YULAN_AUD_EQ_TYPE_VOCAL,            /* ����                                 */
    YULAN_AUD_EQ_TYPE_DANCE,            /* ����                                 */
    YULAN_AUD_EQ_TYPE_SOFT,             /* ���                                 */
    YULAN_AUD_EQ_TYPE_USR_MODE=0xff,    /* �û�ģʽ, pbuffer = __s8 UsrEqArray[USR_EQ_BAND_CNT],
                                           ÿ�����ֵΪ:
                                           USR_EQ_POSITIVE_PEAK_VALUE~USR_EQ_NEGATIVE_PEAK_VALUE        */
    YULAN_AUD_EQ_TYPE_

} __yulan_audio_eq_t;


typedef enum __YULAN_SUBTITLE_ENCODE
{
    YULAN_SUB_ENCODE_UNKNOWN=-1,// unknown subtitle encode type

    YULAN_SUB_ENCODE_NONE = 0,  // none subtitle bitstream
    YULAN_SUB_ENCODE_BITMAP,    // subtitle encode type is bitmap
    YULAN_SUB_ENCODE_UTF8,      // subtitle encode type is UTF8
    YULAN_SUB_ENCODE_GB2312,    // subtitle encode type is GB2312
    YULAN_SUB_ENCODE_UTF16LE,   // subtitle encode type is UTF16-LE
    YULAN_SUB_ENCODE_UTF16BE,   // subtitle encode type is UTF16-BE
    YULAN_SUB_ENCODE_UTF32LE,   // subtitle encode type is UTF32-LE
    YULAN_SUB_ENCODE_UTF32BE,   // subtitle encode type is UTF32-BE
    YULAN_SUB_ENCODE_BIG5,      // subtitle encode type is BIG5
    YULAN_SUB_ENCODE_GBK,       // subtitle encode type is GBK
    YULAN_SUB_ENCODE_ANSI,      // subtitle encode type is text, unknown character encode type

    YULAN_SUB_ENCODE_

} __yulan_subtitle_encode_t;


typedef enum __YULAN_GET_SUB_INF
{
    YULAN_GET_SUB_INF_NONE = 0, //invalid, return no data

    YULAN_GET_SUB_INF_ALL,      //get all subtitle item information
    YULAN_GET_SUB_INF_ITEM,     //get subtitle item information according to the given time

    YULAN_GET_SUB_INF_

} __yulan_get_sub_inf_t;

typedef struct __YULAN_TAG_INF
{
    __u8    tag_magic[4];            /* ý���ļ���ǩ�ı�־��'A'/'V','T','A','G' */

    __s32   file_offset;                /* ý���ļ��Ķϵ����ļ��ڵ�ƫ��         */
    __s32   time_offset;                /* ý���ļ��Ķϵ��ʱ��ƫ��             */
    __u8    decinf[YULAN_TAG_INF_SIZE]; /* ���������ֳ��ָ���Ϣ                 */

}__yulan_tag_inf_t;


//the information of the media file which will be input to play
typedef struct __YULAN_MEDIA_FILE_INF
{
    char    file_path[YULAN_FPATH_SIZE];/* ý���ļ��ľ���·����                 */
    __u32   tag_inf_validflag;          /* �ļ��ϵ���Ϣ�Ƿ���Ч�ı�־, =0(invalid), =1(valid)           */
    __yulan_tag_inf_t   tag_inf;        /* ý���ļ��ϵ㲥�ŵ������Ϣ           */
} __yulan_media_file_inf;


//the setting information for YULAN sub-system
typedef struct __YULAN_SETTING_INF
{
    //��Ƶ���ŵ��������
    __u32       volume;                     /* ����ֵ                           */
    __u32       eq;                         /* ��Ч����                         */
    __u32       channel;                    /* ����ģʽ                         */
    __u32       vpsspeed;                   /* �����ٶ�                         */
    __u32       ab_loopcnt;                 /* A-B����ѭ������                  */
    __u32       spectrum;                   /* �Ƿ�ʹ��Ƶ�׽���                 */
    __s16       usr_eq_filter[USR_EQ_BAND_CNT];
                                            /* �û��Զ�����Ч���˲���Ƶ�ε��˲�ϵ�� */

    __u8        volume_flag;                /* volume�����Ƿ���Ч�ı�־         */
    __u8        eq_flag;                    /* eq�����Ƿ���Ч�ı�־             */
    __u8        channel_flag;               /* channel�����Ƿ���Ч�ı�־        */
    __u8        vpsspeed_flag;              /* vpsspeed�����Ƿ���Ч�ı�־       */
    __u8        ab_loopcnt_flag;            /* ab_loopcnt�����Ƿ���Ч�ı�־     */
    __s32       aud_suffix;                 /* ��ǰ���ŵ���Ƶ������Ƶ��������±�*/

    //��Ƶ���ŵ��������
    __s32       vid_layer_hdl;              /* ��Ƶ����ͼ����                 */
    __rect_t    vid_window;                 /* ��Ƶͼ�񴰿�����                 */
    __s32       vid_show_mode;              /* ��Ƶͼ����ʾ����ģʽ             */
    __u32       frpic_hold_time;            /* ���������״̬����ƵͼƬ��ͣ��ʱ��                   */

    __s8        vid_layer_hdl_flag;         /* vid_layer_hdl�����Ƿ���Ч�ı�־                      */
    __s8        vid_window_flag;            /* vid_window�����Ƿ���Ч�ı�־                         */
    __s8        vid_show_mode_flag;         /* vid_show_mode�����Ƿ���Ч�ı�־  */
    __s8        frpic_hold_time_flag;       /* frpic_hold_time�����Ƿ���Ч�ı�־                    */

    //������Ƶ���Ź�����������
    __u32       frspeed;                    /* ����������ٶ�                                       */
    __u8        frspeed_flag;               /* frspeed�����Ƿ���Ч�ı�־                            */

} __yulan_setting_inf_t;



//the information from media file
typedef struct __YUAN_MEDIA_FILE_INFO
{
    __s8        nHasAudio;          //ý���ļ��Ƿ�����Ƶ��
    __s8        nHasLrc;            //ý���ļ��Ƿ�����Ƶ��
    __s8        nHasVideo;          //ý���ļ��Ƿ�����Ļ��

    __u8        nStreamNum;         //ý���ļ�����������
    __u8        nAudStrmNum;        //ý���ļ�����Ƶ���ĸ���
    __u8        nVidStrmNum;        //ý���ļ�����Ƶ���ĸ���
    __u8        nLrcStrmNum;        //ý���ļ�����Ļ���ĸ���

    __u32       nDuration;          //ý���ļ��ĳ�����ʱ��

    __audio_bs_info_t          pAudStrmList[YULAN_MAX_AUDIO_STREAM_NUM];    //��Ƶ����Ϣ����
    __video_bs_info_t          pVidStrmList;                               //��Ƶ����Ϣ����
    __subtitle_stream_info_t   pSubStrmList[YULAN_MAX_SUBTITLE_STREAM_NUM];

} __yulan_media_file_info_t;

typedef struct __YULAN_SUBTITLE_ITEM
{
    __u32           uPts;           // time stamp of the subtitle item, (ms)
    __u32           uDuration;      // duration of the item, (ms)
    __u16           uDataType;      // subtitle data type, text,bitmap,encode type,
                                //__cedar_subtitle_encode_t
    __s32           nPicWidth;      //�����SUB��ͼ���ʽ����Ļ����Ҫ���Ϳ����Ϣ
    __s32           nPicHeight;
    __colorcode_t   colorCode;  //�����SUB��ͼ���ʽ����Ļ����Ҫ����color code�Ա�ӵ�ɫ��ѡ����ɫ
    __contrast_t    contrast;   //�����SUB��ͼ���ʽ����Ļ����Ҫ����contrast ��ȷ���Ƿ���ʾ
    __u8            *pSubtitle;     // subtitle data buffer
    __s32           nSrcWidth;      //��Ļ��������
    __s32           nSrcHeight;     //��Ļ���������
    __u8            isFstSubFlag;
} __yulan_subtitle_item_t;



typedef struct ___YULAN_MENUBTN_POS
{
    __u8      colorTabIdx;
    __u32     btnStartX;
    __u32     btnStartY;
    __u32     btnEndX;
    __u32     btnEndY;
}__yulan_menuBtn_pos_t;


typedef struct  __YULAN_MEUBTN_INFO_ITEM
{
   __u8                   nValidBtn;
   __u8                   slBtnColorCode[3][4];                //ѡ��ť��colorcodeֵ
   __u8                   slBtnContrast[3][4];                 //ѡ��ť��contrastֵ
   __u8                   acBtnColorCode[3][4];                //���ť��colorcodeֵ
   __u8                   acBtnContrast[3][4];                 //���ť��contrastֵ
   __yulan_menuBtn_pos_t  selBtnPosInfo;                       //ѡ��ť��λ����Ϣ
   __yulan_menuBtn_pos_t  totalBtnPosInfo;                    //���а�ť��λ����Ϣ����С����ʼ��ַ�����Ľ�����ַ

}__yulan_menuBtn_info_t;


/* play mode */
typedef enum __YULAN_PLAY_MODE
{
    YULAN_CLOSE_REPEAT_MODE      = 0,
	YULAN_REPEAT_TITLE_MODE      = 1,
	YULAN_REPEAT_CHAPTER_MODE    = 2,
	YULAN_REPEAT_MODE
}yulan_play_mode_t;

#endif  /* _MOD_DVDNAVI_H_ */


