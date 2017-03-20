/*
*********************************************************************************************************
*                                                    eMOD
*                                   the Easy Portable/Player Operation System
*                                               mod_mmp sub-system
*
*                                (c) Copyright 2006-2007, Steven.ZGJ China
*                                               All Rights Reserved
*
* File   : mod_cedar.h
* Version: V1.0
* By     : steven.zgj
* update : kevin.z  2008-11-2 15:43
* update : kevin.z  2008-12-12 16:10
*********************************************************************************************************
*/
#ifndef _MOD_CEDAR_H_
#define _MOD_CEDAR_H_

#include "typedef.h"
#include "cfgs.h"

#define CEDAR_TAG_INF_SIZE      (240)       //(256)ע��:ʡ�µ�16�ֽڱ���Ҳ����       /* AUD_TAG_INF_SIZE, buffer size for audio tag information                    */
#define CEDAR_FPATH_SIZE        (1024*4)    /* ý���ļ�·�����������ֵ                                 */

//define variable for audio render interface
#define SPECT_GROUP_WIDTH       (32)    /* һ��Ƶ���ڵ�Ƶ������Ӧ�ó���ο���ֵ����Ƶ�׿��             */
#define MAX_SPEC_GROUP_NUM      (10)    /* Ƶ����������������Ӧ�ó���ο���ֵ����ռ�                 */
#define USER_DEFINE_EQ_SIZE     (10)    /* �û��Զ���EQʱ����Ҫ�����һ���������                       */

#define AUDIO_VPS_SLOWEST       (-4)    /* ��Ƶ���ٲ��ŵ������ٶ�ֵ                                     */
#define AUDIO_VPS_NOMAL         (0)     /* ��Ƶ���������ٶ�ֵ                                           */
#define AUDIO_VPS_FASTEST       (10)    /* ��Ƶ���ٲ��ŵ�����ٶ�ֵ                                     */

#define MIN_VOLUME_VAL          (0)     /* ������Сֵ����ʱΪ����״̬                                   */
#define MAX_VOLUME_VAL          (30)    /* �������ֵ��ֻ����������Χ�ڵ�ֵ����Ϊ��Ч������Ϊ�Ƿ�ֵ     */

#define CEDAR_FASTEST_FRSPEED   (128)   /* ��������˵������                                         */
#define CEDAR_SLOWEST_FRSPEED   (1)     /* ��������˵���С����                                         */


#define CEDAR_FRPIC_HOLDTIME_LEAST      (30)    /* ���������״̬�£�ͼƬ��ʾͣ����Сʱ�䣬(30ms)       */
#define CEDAR_FRPIC_HOLDTIME_LARGEST    (5000)  /* ���������״̬�£�ͼƬ��ʾͣ�����ʱ�䣬(5000ms)     */

#define MAX_LANG_CHAR_SIZE  (32)

typedef enum __CEDAR_CMD_SET            /* ý�岥���м�������б�(IOCtrol)                              */
{
    CEDAR_CMD_SET_MEDIAFILE=0x0100,     /* ���ô����ŵ�ý���ļ�·�����ϵ���Ϣ, pbuffer = (__cedar_media_file_inf *) */
    CEDAR_CMD_GET_MESSAGE_CHN,          /* ��ȡ������Ϣͨ����return = __krnl_event_t*, (__cedar_feedback_msg_t)     */
    CEDAR_CMD_GET_ERROR_TYPE,           /* ��ȡý�岥�����Ĵ�������, error in __cedar_feedback_msg_t    */

    CEDAR_CMD_PLAY=0x0200,              /* ��������,  */
    CEDAR_CMD_STOP,                     /* ֹͣ����                                                     */
    CEDAR_CMD_PAUSE,                    /* ��ͣ����                                                     */
    CEDAR_CMD_FF,                       /* ���                                                         */
    CEDAR_CMD_REV,                      /* ����                                                         */
    CEDAR_CMD_JUMP,                     /* ����, aux = time, unit:ms, ret = EPDK_OK/EPDK_FAIL, pbuffer = 1��ʾҪ��ȷ����           */
    CEDAR_CMD_GET_STATUS,               /* ��ȡ����ģʽ,aux = 1��ʾ��������(����״̬��������ʱ̬),aux=0��ʾ��״̬�ȶ��ٷ���, return = __cedar_status_t                   */
    CEDAR_CMD_AUDIO_RAW_DATA_ENABLE,    /* ��Ƶ�Ƿ���raw data�����aux = 1:enalbe; aux=0;disable*/
    CEDAR_CMD_SET_STOP_MODE,            //aux = CedarStopMode, stopʱ�Ǳ������в��,����ж�����в��.aux = CEDAR_STOP_MODE_KEEP_PLUGINS:�������,��ӿ��ļ��л��ٶ�,Ĭ��ʹ�ñ��������ģʽ.aux = CEDAR_STOP_MODE_UNINSTALL_PLUGINS:ж�����в��,������ģʽ��,�޷��л���Ч

    CEDAR_CMD_GET_TOTAL_TIME=0x300,     /* ��ȡ������ʱ��   return = total time(ms)                     */
    CEDAR_CMD_GET_CUR_TIME,             /* ��ȡ��ǰ����ʱ�� return = current play time(ms)              */
    CEDAR_CMD_GET_TAG,                  /* ��ȡ��ǰ����ʱ���ı�ǩ��Ϣ, pbuffer = __cedar_tag_inf_t*, app�����ڴ�                                 */
    CEDAR_CMD_SET_FRSPEED,              /* ���ÿ�������ٶ� aux    = CEDAR_SLOWEST_FRSPEED ~ CEDAR_FASTEST_FRSPEED  */
    CEDAR_CMD_GET_FRSPEED,              /* ��ȡ��������ٶ� return = speed                              */
    CEDAR_CMD_SET_TAG,                  // ����ǰ����tag��Ϣ,��ΪCEDAR_CMD_SET_MEDIAFILE������tag��Ϣ���������Ҫ�������ã������ڵ���CEDAR_CMD_SET_MEDIAFILE����CEDAR_CMD_SET_USER_FILEOP֮��,������Ч

    CEDAR_CMD_GET_ABSTYPE=0x0400,       /* ��ȡ��ǰ���ڲ��ŵ���Ƶ������ʽ return = __cedar_audio_fmt_t                */
    CEDAR_CMD_GET_AUDBPS,               /* ��ȡ��Ƶ�������� return = bps                                */
    CEDAR_CMD_GET_SAMPRATE,             /* ��ȡ��Ƶ������   return = sample rate                        */
    CEDAR_CMD_SET_CHN,                  /* ������������     aux = __cedar_audio_chn_t                   */
    CEDAR_CMD_GET_CHN,                  /* ��ȡ��Ƶ��ͨ����                                             */
    CEDAR_CMD_SET_VOL,                  /* ��������         aux = MIN_VOLUME_VAL~MAX_VOLUME_VAL, return = new vol   */
    CEDAR_CMD_GET_VOL,                  /* ��ȡ����         return = volume                             */
    CEDAR_CMD_VOLUP,                    /* ����ֵ�ϵ�һ��                                               */
    CEDAR_CMD_VOLDOWN,                  /* ����ֵ�µ�һ����                                             */
    CEDAR_CMD_SET_EQ,                   /* ����EQ           aux = __cedar_audio_eq_t                    */
    CEDAR_CMD_GET_EQ,                   /* ��ȡEQ           return = __cedar_audio_eq_t                 */
    CEDAR_CMD_SET_VPS,                  /* ���ñ��ٲ����ٶ� aux    = -4~10��=0-normal; <0-slow; >0-fast, ��0.1��Ϊ��λ����,��˱��ٷ�ΧΪԭ�ٵ�0.6~2�� */
    CEDAR_CMD_GET_VPS,                  /* ��ȡ���ٲ����ٶ� return = -4~10                              */
    CEDAR_CMD_SET_AB_A,                 /* ����A-B���ŵ���ʼ��(A��)����������Ƶ�ļ�ab����                                     */
    CEDAR_CMD_SET_AB_B,                 /* ����A-B���ŵĽ�����(B��)                                     */
    CEDAR_CMD_SET_AB_LOOPCNT,           /* ����A-B���ŵ�ѭ������                                        */
    CEDAR_CMD_CLEAR_AB,                 /* ȡ��A-B����                                                  */
    CEDAR_CMD_SET_SPECTRUM,             /* �����Ƿ��Ƶ�׽����� 0-disable; !0-enable                  */
    CEDAR_CMD_GET_SPECTRUM,             /* ��ȡʵʱspectrum (i/o)pbuffer = spectrum (return ok or fail) */
    CEDAR_CMD_SEL_AUDSTREAM,            /* ѡ����Ƶ��. aux = �����±�, ret = EPDK_OK/EPDK_FAIL����ʾ�Ƿ�����͸�CEDAR.������ʾCEDAR����ɹ� */
    CEDAR_CMD_GET_AUDSTREAM,            /* �õ���ǰ���ڲ��ŵ���Ƶ�����±��, ret = suffix or -1(if not find)*/
    CEDAR_CMD_GET_AUDSTREAM_PROFILE,    /* ��ȡ������Ƶ����˵����Ϣ: pbuffer = __audstream_profile_t, app malloc it. return = EPDK_OK or EPDK_FAIL*/
    CEDAR_CMD_GET_AUDSTREAM_CNT,         /* �õ���Ƶ���������� ret = ����(-1����ʧ��)�����CEDAR_CMD_GET_AUDSTREAM_PROFILE_V2һ��ʹ��; ���ʹ��CEDAR_CMD_GET_AUDSTREAM_PROFILE,�Ͳ�Ҫʹ�ñ��ӿڵõ���Ƶ���� */
    CEDAR_CMD_GET_AUDSTREAM_PROFILE_V2,  /* �õ�������Ƶ����˵����Ϣ��ʹ�÷���ͬCEDAR_CMD_GET_AUDSTREAM_PROFILE����pbuffer = __audstream_profile_v2_t������AudStrmListArrayҲ��app���䣬�ô�����Ƶ������������*/
    CEDAR_CMD_QUERY_BUFFER_USAGE,        /* ��ѯCEDAR�ڲ��Ļ���buffer��ռ����, ����vbs buffer,abs buffer��ret = EPDK_OK/EPDK_FAIL, pbuffer = __buffer_usage_t*,app�����ڴ� */
    
    //A-B���ŵڶ����ʵ�ֽӿڣ��͵�һ��Ľӿڲ��ܻ���. �ڶ���֧������Ƶ�ļ�����һ��ֻ��֧����Ƶ�ļ�
    //���������Ƶ�ļ����FSEEKģʽ, ��ô������PLAY״̬������A,B��, ����A���ʱ���������Ч��cedar���Լ�ȷ��A���ʱ��
    //��Ƶ�ļ�����Ƶ�ļ���JUMPģʽ����״̬û��Ҫ��
    CEDAR_CMD_SET_AB_A_V2,                 /* ����A-B���ŵ���ʼ��(A��),��cedar״̬û��Ҫ�󣬵�Ҫ���ļ��򿪺�����, aux= APointPts, ms, aux=-1:��ʾ��cedar�ѵ�ǰ���ŵ���ΪA                                     */
    CEDAR_CMD_SET_AB_B_V2,                 /* ����A-B���ŵĽ�����(B��),��cedar״̬û��Ҫ��, ��Ҫ���ļ��򿪺�����,aux= BPointPts       aux=-1:��ʾ��cedar�ѵ�ǰ���ŵ���ΪB                            */
    CEDAR_CMD_SET_AB_LOOPCNT_V2,           /* ����A-B���ŵ�ѭ������, ��cedar״̬û��Ҫ��,��Ҫ���ļ��򿪺�����,                                        */
    CEDAR_CMD_CLEAR_AB_V2,                 /* ȡ��A-B����,                                                  */
    CEDAR_CMD_ENABLE_AB_V2,                /*  A-B������Ч������һЩ�����Ϸ����ж�,�Ϸ�������Ч      */
    CEDAR_CMD_SET_AUDIO_AB_MODE_V2,        /*  ������Ƶ�ļ�A-B���ŵ�ʵ�ַ�ʽ,��ʽ0:jump; ��ʽ1Ϊfseek��Ĭ��Ϊ��ʽ0. ��cedar����ģ���֮��Ϳ������� */
    CEDAR_CMD_SET_SWGAIN,		   /* �����������������*/
    CEDAR_CMD_GET_SWGAIN,		   /* �����������������*/   
    
    CEDAR_CMD_GET_VBSTYPE=0x0500,       /* ��ȡ��Ƶ������ʽ                                             */
    CEDAR_CMD_GET_VIDBITRATE,           /* ��ȡ��Ƶ��������                                             */
    CEDAR_CMD_GET_VIDFPS,               /* ��ȡͼ��֡��     return = frame rate                         */
    CEDAR_CMD_GET_FRAMESIZE,            /* frame size       return = (width << 16) + height             */
    CEDAR_CMD_SET_VID_LAYERHDL,         /* ������Ƶͼ���� pbuffer = video_layer_hdl                   */
    CEDAR_CMD_SET_VID_WINDOW,           /* ������Ƶ�������� pbuffer = (__rect_t *)                      */
    CEDAR_CMD_GET_VID_WINDOW,           /* ��ȡ��Ƶ�������� return = (__rect_t *)                       */
    CEDAR_CMD_SET_VID_SHOW_MODE,        /* ������Ƶͼ����ʾ��ģʽ��aux = __cedar_vide_window_ratio_mode_t, return = ���ú�ĵ�ǰ��ʾģʽ, ��� aux = CEDAR_VID_WINDOW_USERDEF, pbuffer = __pCBK_t, ����������SrcScnWinPara */
    CEDAR_CMD_GET_VID_SHOW_MODE,        /* ��ȡ��Ƶͼ����ʾģʽ��return = __cedar_vide_window_ratio_mode_t  */
    CEDAR_CMD_SWITCH_VID_SHOW,          /* �򿪻�ر�ͼƬ��ʾ��aux = 0:�ر�ͼƬ��ʾ��aux = 1:��ͼƬ��ʾ   */
    CEDAR_CMD_SET_FRPIC_SHOWTIME,       /* ���ÿ������״̬�£���Ƶ֡��ʾ�ĳ���ʱ��                     */
    CEDAR_CMD_GET_FRPIC_SHOWTIME,       /* ��ȡ�������״̬�£�ͼƵ֡��ʾ�ĳ���ʱ��                     */
    CEDAR_CMD_SET_ROTATE,               /* ���ý���ͼ����ת�ĽǶ�.aux = 0:Ĭ��Ϊԭͼ,1:˳ʱ��90��,2:180��,3:270��,4:ˮƽ����,5:��ֱ����,ע������:ֻ�������ļ�����֮ǰ���ã����Ź�����(�����������)������Ч*/
    CEDAR_CMD_INVALID_VIDEOLAYER,       /* ֪ͨcedar video layer��Ч. aux = 1, ��Ч; aux = 0,�ָ���Ч */
    CEDAR_CMD_SET_FILE_SWITCH_VPLY_MODE,     //aux = CEDAR_FILE_SWITCH_VPLY_MODE_GAP��,�����ļ��л�����ʱ��ģʽ,��ͨģʽҪ����, �޷�ģʽ��һֱ����,����һ����Ƶ�ļ������һ֡ͼ����ʾֱ�����ļ��ĵ�һ֡��ʼ��ʾ.��Open cedar֮�������
//    CEDAR_CMD_SET_VDRV_SCALE_PARA,      //����vdrv����ʱ��ԭʼͼ���ѹ������,������֮���ͼ�������Դͼ���ѹ������, aux = (width_scale << 16) | (height_scale), width_scale = 2, 4; height_scale = 2. ��������: width_scale = 4��ʾ�����С4��,�������ͼ��Ŀ����ԭͼ��1/4.height_scale�Դ�����.
                                        //cedar������ò���.ֻ���ڲ���ǰ����,���Ź�����������Ч,���ڵ���CEDAR_CMD_PLAY֮ǰ���ò���.�����scaleû��Ҫ��,����ǰ��Ҫ���ñ��ӿ�
    CEDAR_CMD_ENABLE_VIDEO_AUTO_SCALE,   //����vdrv�Զ���scale,�Լ��ٴ�������. aux = 1:enable; aux=0:disable.Ĭ��Ϊdisable.ֻ�����ڲ���ǰ���ã����Ź�����������Ч.
    CEDAR_CMD_SET_RESERVED_MEM_SIZE,    //����Ԥ�����ڴ�����,�����߽��������õ��ڴ�����ʱ,���ⲿ���ڴ�Ԥ������.
		CEDAR_CMD_SET_TV_MODE,							/*aux = 1:TV mode,aux = 0: ����ý�岥��ģʽ��������е��ӹ���¼�Ƶ�TS��ʽ�ļ�������Ϊaux = 1*/
    //֧��3D����ģʽ
    CEDAR_CMD_GET_VIDEO_DISPLAY_MODE_INFO,           /* ��ȡ��Ƶ����ģʽ��Ϣ��__cedar_display_mode_info   */
    CEDAR_CMD_SET_VIDEO_DISPLAY_MODE_OUT,            /* ������Ƶ����ģʽ��Ϣ��__cedar_display_mode_Out parseģ���趨; aux=1,�û�����  */
    CEDAR_CMD_GET_VIDEO_DISPLAY_MODE_OUT,            /* ��ȡ��Ƶ����ģʽ��Ϣ��__cedar_display_mode_Out    */

    CEDAR_CMD_ENABLE_DEINTERLACE,               //����deinterlace����, aux = 1:enable; aux=0:disable;
	CEDAR_CMD_CONFIG_VIDEO_LYR_CK_ENABLE,       //�������ø�cedar����Ƶͼ���ck_enable������ֵ, aux = 1:enable; aux = 0:disable

    CEDAR_CMD_GET_LBSTYPE=0x0600,       /* ��ȡ��Ļ������ʽ,������Ļ,�ļ����ͺͱ�������Ŀǰ�ǻ�Ϊһ̸��. ret = __cedar_lyric_fmt_t                                             */
    CEDAR_CMD_GET_SUB_INFO,             /* ��ȡ��Ļ����Ŀ��Ϣ, aux = time, pbuffer = __cedar_get_sub_inf_t, (return = __cedar_subtitle_item_t*��__cedar_buf_inf_t*)                         */
    CEDAR_CMD_GET_SUBTITLE_PROFILE,     /* ��ȡ������Ļ��˵����Ϣ:���磬���ڵ�stream_idx,���Եȣ�pbuffer = __subtitle_profile_t, app malloc it. return = EPDK_OK or EPDK_FAIL*/
    CEDAR_CMD_SELECT_SUBTITLE,          /* ѡ��һ����Ļ���ţ������л���Ļ, aux:��Ļ�±�in __subtitle_profile_t*/
    CEDAR_CMD_GET_SUBTITLE,             /* �õ���ǰ���ڲ�����Ļ���±꣬if not find, return -1*/
    CEDAR_CMD_GET_SUBTITLE_CNT,         /* �õ���Ļ����������ret = ����,ret=-1����û�н��������,����֪���м�����Ļ��.Ӧ�ó�����Եȴ�һ��ʱ���ٲ�, ���CEDAR_CMD_GET_SUBTITLE_PROFILE_V2ʹ��, ���ʹ��CEDAR_CMD_GET_SUBTITLE_PROFILE�ӿڣ��Ͳ�Ҫ�ñ��ӿ�*/
    CEDAR_CMD_GET_SUBTITLE_PROFILE_V2,  /* ͬCEDAR_CMD_GET_SUBTITLE_PROFILE����ʹ��__subtitle_profile_v2_t��SubtitleStreamArrayҲ��app���䣬�ô�����Ļ������������ */
    CEDAR_CMD_SET_SUBTITLE_ITEM_POST_PROCESS, /* �����Ƿ������Ļ�ĺ��������ļ���Ļ��������ȥ�������ַ�;����ͼ����Ļ����ʱû�к���, aux=1:���к���; aux=0�����к���, �����CEDAR_CMD_PLAYǰ���� */
    CEDAR_CMD_ENABLE_EXTERN_SUBTITLE,       //���������Ļ���, aux = 1:����; aux =0:���������Ļ, ���ļ�����ǰ����,�����CEDAR_CMD_SET_MEDIAFILE֮��͵��øýӿڣ�Ĭ�ϼ�������Ļ
    CEDAR_CMD_ENABLE_EMBEDED_SUBTITLE,     //�Ƿ񼤻�������Ļ. AUX = 1:����, aux =0:��ֹ. Ĭ�ϼ���.

    CEDAR_CMD_CAPTURE_PIC=0x0700,       /* �ѷ���, ����, pbuffer = __cedar_pic_info_t     */
    CEDAR_CMD_ASK_PIC_BUFSIZE,          /* �ѷ���, ��CEDAR��ѯ��Ҫ����buffer�Ա�APP���䣬pbuffer = __cedar_pic_info_t, ret = size, -1����ʧ��*/
    CEDAR_CMD_GET_FRAME_PIC,            /* ��ͼ����IO�ӿ���ͬ��������. �ѵ�ǰ������ʾ��frame��picture��ȡ������pbuffer = Ŀ��(FB*)���ڴ�����ѷ���ã��Ƽ�ʹ��ARGB��ʽ��FB�����в�����������ȷ,cedar����� */

    CEDAR_CMD_PLAY_AUX_WAV_FILE=0x800,      /*����������Ƶ���ŵ�ʱ�򲥷�һЩ���ӵ�wav�������ݣ������ݸ�������Ƶ���ݻ���, aux=__cedar_play_aux_wav_mode_t, pbuffer=full filename */
    CEDAR_CMD_PLAY_AUX_WAV_BUFFER,          /*����ͬCEDAR_CMD_PLAY_WAV_FILE��������һ����aux=__cedar_play_aux_wav_mode_t, pbuffer=(__cedar_pcm_info_t*) */    
    CEDAR_CMD_GET_AUX_WAV_BUFFER_SIZE,      /*��ȡ�������ݲ���buffer��С, ioctrlֱ�ӷ���buffer size*/
    CEDAR_CMD_SET_AUX_WAV_BUFFER_SIZE,           /*�����������ݲ���buffer��С, aux=buffer size*/
    CEDAR_CMD_SET_PITCH_LEVEL=0x810,        /*���ñ��������aux:���ϵ����-12<=aux<=12�� default: 0*/
    CEDAR_CMD_GET_PITCH_LEVEL,              /*��ȡ�������*/

    //��������ͼ�м��
    CEDAR_DUCKWEED_CMD_OPEN_MEDIAFILE = 0x1100,   /* ���ļ�, û�в���, �ļ�·��������ͨ��CEDAR_CMD_SET_MEDIAFILE��CEDAR_CMD_SET_USER_FILEOP */
    CEDAR_DUCKWEED_CMD_CLOSE_MEDIAFILE,           /* �رյ�ǰ��ý���ļ������ܻ�Ҫ�رս���� */
    CEDAR_DUCKWEED_CMD_GET_FILE_FORMAT  = 0x1200, /* �õ��ļ���Ϣ�������ļ���ʽ */
    CEDAR_DUCKWEED_CMD_GET_FILE_SIZE,             /* �õ��ļ�����ʵ��С, pbuffer = __s64*, ret = EPDK_OK,����odf�����ļ���ʵ�ʵ��ļ���С*/
    CEDAR_DUCKWEED_CMD_GET_TOTAL_TIME,            /* ��ȡ������ʱ��   return = total time(ms)                     */
    CEDAR_DUCKWEED_CMD_GET_VIDSTREAM_CNT,         /* �õ���Ƶ���������� ret = ����(-1����ʧ��) */
    CEDAR_DUCKWEED_CMD_GET_VIDSTREAM_PROFILE,     /* ��ȡ������Ƶ����˵����Ϣ: pbuffer = __vidstream_profile_v2_t, ����VidStrmListArrayҲ��app����. return = EPDK_OK or EPDK_FAIL */
    CEDAR_DUCKWEED_CMD_GET_AUDSTREAM_CNT,         /* �õ���Ƶ���������� ret = ����(-1����ʧ��)*/
    CEDAR_DUCKWEED_CMD_GET_AUDSTREAM_PROFILE,     /* ��ȡ������Ƶ����˵����Ϣ: pbuffer = __audstream_profile_v2_t, ����AudStrmListArrayҲ��app����. return = EPDK_OK or EPDK_FAIL */
    CEDAR_DUCKWEED_CMD_GET_PREVIEW_FB = 0x1300,       /* �õ�Ԥ��ͼ, pbuffer = FB*��FB�Ĳ�����Ҫ��,����ֻ֧���õ�һ֡��һ���ļ���һ֡������ٵ��ýӿڣ�ֱ�ӷ���ʧ�� */
    CEDAR_DUCKWEED_CMD_GET_PREVIEW_FB_BY_PTS,         /* �õ�Ԥ��ͼ, pbuffer = FB*��FB�Ĳ�����Ҫ��,aux = pts(ms),��ʾ��Ҫ�ڸ�PTS�����Ĺؼ�֡ */


    CEDAR_CMD_SET_USER_FILEOP = 0x05000,     /* �����û���context,��ص�����,�ļ����ȵ�, (pbuffer = __cedar_usr_file_op_t *) */
    CEDAR_CMD_SET_PANEL_SELECT,					  //˫����ʾ��ѡ��0,LCD0; 1,LCD1
    CEDAR_CMD_

} __cedar_cmd_set_t;

typedef enum tag_CedarFileSwitchVplyMode
{
    CEDAR_FILE_SWITCH_VPLY_MODE_GAP = 0x00,         //2���ļ��л�ʱ�����ϵģ��м��к���
    CEDAR_FILE_SWITCH_VPLY_MODE_SEAMLESS,           //�޷�, ������.
    CEDAR_FILE_SWITCH_VPLY_MODE_,
}CedarFileSwitchVplyMode;

typedef enum tag_CedarStopMode
{
    CEDAR_STOP_MODE_KEEP_PLUGINS = 0x00,         //ֹͣ����ʱ,�������
    CEDAR_STOP_MODE_UNINSTALL_PLUGINS,           //ֹͣ����ʱ,ж�����в��
    CEDAR_STOP_MODE_,
}CedarStopMode;

typedef enum __CEDAR_FEEDBACK_MSG           /* ����ý�岥���м��������Ӧ�ó������Ϣ����               */
{
    CEDAR_ERROR_NOP                 =   0,  /* ����Ϣ                                                   */

    //cedar feed back error type
    CEDAR_ERROR_OPEN_FILE_FAIL      = -20,  /* �򿪵�ǰý���ļ�ʧ��                                     */
    CEDAR_ERROR_FILE_FMT_UNKNOWN,           /* �޷�ʶ��ǰý���ļ���ʽ                                 */
    CEDAR_ERROR_FILE_NOT_SUPPORT,           /* ��ǰý���ļ���֧��                                       */
    CEDAR_ERROR_FILE_DATA_ERROR,            /* ý���ļ����ݴ��޷�����                                 */
    CEDAR_ERROR_NO_FILE_INPUT,              /* δ����Ҫ���ŵ�ý���ļ�                                   */
    CEDAR_ERROR_READ_FILE_FAIL,             /* ��ȡ�ļ�����ʧ��                                         */

    CEDAR_ERROR_START_PLG_FAIL      = -40,  /* �������ʧ��                                             */
    CEDAR_ERROR_ADEC_ERROR,                 /* ��Ƶ�������������                                     */
    CEDAR_ERROR_VDEC_ERROR,                 /* ��Ƶ�������������                                     */
    CEDAR_ERROR_TDEC_ERROR,                 /* ��Ļ�������������                                     */

    CEDAR_ERROR_PIC_SIZE_TOO_LARGE  = -60,  /* ͼƬ�ߴ�̫���޷�֧�ֽ���                               */
    CEDAR_ERROR_REQ_MEMORY_FAIL,            /* �����ڴ�ʧ��                                             */

    CEDAR_ERROR_UNKOWN              = -80,  /* δ֪����                                                 */

    //cedar feed back status type
    CEDAR_FEDBAK_NO_ERROR           =  1,   /* ����Ϣ                                                   */
    CEDAR_FEDBAK_PLAY_END,                  /* ����ý���ļ�����                                         */

    CEDAR_FEDBAK_

} __cedar_feedback_msg_t;


typedef enum __CEDAR_VID_WINDOW_RATIO_MODE
{
    CEDAR_VID_WINDOW_NOTCARE = 0x00,        /* ������ͼƬ��ʾ�������Ե�ǰ���õı���         */
    CEDAR_VID_WINDOW_ORIGINAL,              /* ��ͼƬԭʼ��С�ڴ�������ʾ�������������     */
    CEDAR_VID_WINDOW_BESTSHOW,              /* ��ͼƬ����ı�����������������ʾ��ͼƬ������ */
    CEDAR_VID_WINDOW_FULLSCN,               /* �Դ��ڵı�������ͼƬ����������ʾ�����ܻ���� */
    CEDAR_VID_WINDOW_4R3MODE,               /* ǿ����4:3��ģʽ������ʾ��Ƶͼ��ͼ������  */
    CEDAR_VID_WINDOW_16R9MODE,              /* ǿ����16:9��ģʽ������ʾ��Ƶͼ��ͼ������ */
    CEDAR_VID_WINDOW_CUTEDGE,               /* ͼƬ�����кڱߣ���õ�                       */
    CEDAR_VID_WINDOW_USERDEF,               //�û��Զ���,�Լ�ȷ��srcwin, scnwin�Ĳ���,��Ҫcallback����

    CEDAR_VID_WINDOW_

}__cedar_vide_window_ratio_mode_t;

typedef enum __CEDAR_STATUS             /* the status os cedar                  */
{
    CEDAR_STAT_PLAY=0,                  /* play                                 */
    CEDAR_STAT_PAUSE,                   /* pause                                */
    CEDAR_STAT_STOP,                    /* stop                                 */
    CEDAR_STAT_FF,                      /* fast forward                         */
    CEDAR_STAT_RR,                      /* fast backward                        */
    CEDAR_STAT_JUMP,                    /* jump                                 */
    CEDAR_STAT_

} __cedar_status_t;


typedef enum __CEDAR_AUDIO_FMT
{
    CEDAR_ABS_TYPE_NONE = 0,            /* û����Ƶ��                           */
    CEDAR_ABS_TYPE_UNKNOWN,             /* ����Ƶ��,���޷�ʶ��                  */

    CEDAR_ABS_TYPE_MPEG12_LAYER1,
    CEDAR_ABS_TYPE_MPEG12_LAYER2,
    CEDAR_ABS_TYPE_MPEG12_LAYER3,
    CEDAR_ABS_TYPE_MPEG_AAC_LC,
    CEDAR_ABS_TYPE_AC3,                 // AC-3
    CEDAR_ABS_TYPE_DTS,                 // DTS audio for DVD-Video
    CEDAR_ABS_TYPE_LPCM_V,
    CEDAR_ABS_TYPE_LPCM_A,
    CEDAR_ABS_TYPE_ADPCM,
    CEDAR_ABS_TYPE_PCM,                 // pcm
    CEDAR_ABS_TYPE_WMA_STANDARD,        // standard WMA audio
    CEDAR_ABS_TYPE_FLAC,
    CEDAR_ABS_TYPE_APE,
    CEDAR_ABS_TYPE_OGG,
    CEDAR_ABS_TYPE_RAAC,
    CEDAR_ABS_TYPE_COOK,
    CEDAR_ABS_TYPE_SIPR,
    CEDAR_ABS_TYPE_ATRC,
    CEDAR_ABS_TYPE_AMR,
    CEDAR_ABS_TYPE_RA,
    CEDAR_ABS_TYPE_MID,
    CEDAR_ABS_TYPE_ALAC,

    
    CEDAR_ABS_TYPE_DRA = 0x0100,    //�����ʽ,cmmb����.

    CEDAR_ABS_TYPE_

} __cedar_audio_fmt_t;


typedef enum __CEDAR_VIDEO_FMT
{
    CEDAR_VBS_TYPE_NONE = 0,            /* û����Ƶ��                           */
    CEDAR_VBS_TYPE_UNKNOWN,              /* ����Ƶ��,���޷�ʶ��                  */

    CEDAR_VBS_TYPE_JPEG,
    CEDAR_VBS_TYPE_MJPEG,
    CEDAR_VBS_TYPE_MPEG1_ES,
    CEDAR_VBS_TYPE_MPEG2_ES,
    CEDAR_VBS_TYPE_XVID,
    CEDAR_VBS_TYPE_DIVX3,
    CEDAR_VBS_TYPE_DIVX4,
    CEDAR_VBS_TYPE_DIVX5,
    CEDAR_VBS_TYPE_SORENSSON_H263,
    CEDAR_VBS_TYPE_RMG2,
    CEDAR_VBS_TYPE_RMVB8,
    CEDAR_VBS_TYPE_RMVB9,
    CEDAR_VBS_TYPE_H264,
    CEDAR_VBS_TYPE_VC1,
    CEDAR_VBS_TYPE_AVS,
    CEDAR_VBS_TYPE_H263,
    CEDAR_VBS_TYPE_VP6,
    CEDAR_VBS_TYPE_WMV1,
    CEDAR_VBS_TYPE_WMV2,    
    CEDAR_VBS_TYPE_DIVX2,
    CEDAR_VBS_TYPE_DIVX1,

    CEDAR_VBS_TYPE_

} __cedar_video_fmt_t;

typedef enum __CEDAR_SUBTITLE_ENCODE
{
    CEDAR_SUB_ENCODE_UNKNOWN=-1,// unknown subtitle encode type

    CEDAR_SUB_ENCODE_NONE = 0,  // none subtitle bitstream
    CEDAR_SUB_ENCODE_BITMAP,    // subtitle encode type is bitmap, 4 colors supported.
    CEDAR_SUB_ENCODE_UTF8,      // subtitle encode type is UTF8
    CEDAR_SUB_ENCODE_GB2312,    // subtitle encode type is GB2312
    CEDAR_SUB_ENCODE_UTF16LE,   // subtitle encode type is UTF16-LE
    CEDAR_SUB_ENCODE_UTF16BE,   // subtitle encode type is UTF16-BE
    CEDAR_SUB_ENCODE_UTF32LE,   // subtitle encode type is UTF32-LE
    CEDAR_SUB_ENCODE_UTF32BE,   // subtitle encode type is UTF32-BE
    CEDAR_SUB_ENCODE_BIG5,      // subtitle encode type is BIG5
    CEDAR_SUB_ENCODE_GBK,       // subtitle encode type is GBK
    CEDAR_SUB_ENCODE_ANSI,      // subtitle encode type is text, unknown character encode type

    CEDAR_SUB_ENCODE_

} __cedar_subtitle_encode_t;    //��Ļ�ļ����ַ��������ͻ�ͼ��

typedef enum __CEDAR_LYRIC_FMT
{
    CEDAR_LRC_TYPE_NONE=0,              /* û����Ļ/�����                      */
    CEDAR_LRC_TYPE_NOTSPT,              /* ����Ļ/�����,���޷�ʶ��             */

    CEDAR_LRC_TYPE_LRC,                 /* LRC��ʽ��Ļ��                        */
    CEDAR_LRC_TYPE_SRT,                 /* SRT��ʽ��Ļ��                        */
    CEDAR_LRC_TYPE_KSC,                 /* KSC��ʽ��Ļ��                        */
    CEDAR_LRC_TYPE_SMI,                 /* SMI��ʽ��Ļ��                        */
    CEDAR_LRC_TYPE_SSA,                 /* SSA��ʽ��Ļ��                        */
    CEDAR_LRC_TYPE_TXT,                 /* TXT��ʽ��Ļ��                        */
    CEDAR_LRC_TYPE_SUB,                 /* SUB��ʽ��Ļ��                        */
    CEDAR_LRC_TYPE_IDXSUB,              /* IDX+SUB��ʽ��Ļ��                    */
    CEDAR_LRC_TYPE_ITXT,                /* ��Ƕ�ı���ʽ��Ļ��(mkv)              */
    CEDAR_LRC_TYPE_ISUB,                /* ��ǶSUB��ʽ��Ļ��                    */
    CEDAR_LRC_TYPE_ISSA,                /* ��ǶSSA��ʽ��Ļ��                    */
    CEDAR_LRC_TYPE_PGS,                 /* blueray dvd subtitle for m2ts        */

    //CEDAR_LRC_TYPE_OTHER_I = 0x200,     //������������Ļһ�ɴ�0x200��ʼ�������Ļ��0��ʼ
    CEDAR_LRC_TYPE_DVBSUB = 0x200, //�������ֵ��ӹ㲥��dvb subtitle ��Ļ,Ҳ��Ϊ������Ļ����
    CEDAR_LRC_TYPE_ISDB        = 0x201, 
    CEDAR_LRC_TYPE_TTXT = 202,  // DVB teletext��Ļ
    CEDAR_LRC_TYPE_

} __cedar_lyric_fmt_t;


typedef enum __CEDAR_MEDIA_FILE_FMT
{
    CEDAR_MEDIA_FILE_FMT_NONE = 0x0000,

    //define audio file format
    CEDAR_MEDIA_FILE_FMT_MP3=0x0001,    /* *.mp3, ��Ƶ�ļ�                      */
    CEDAR_MEDIA_FILE_FMT_WAV,           /* *.wav, ��Ƶ�ļ�                      */
    CEDAR_MEDIA_FILE_FMT_WMA,           /* *.wmv, Windows media��/��Ƶ�ļ�      */
    CEDAR_MEDIA_FILE_FMT_APE,           /* *.ape, APE������Ƶ                   */
    CEDAR_MEDIA_FILE_FMT_FLAC,          /* *.flac, FLAC��Ƶ                     */
    CEDAR_MEDIA_FILE_FMT_OGG,           /* *.ogg, OGG Vorbis��Ƶ�ļ�            */
    CEDAR_MEDIA_FILE_FMT_RA,            /* *.ra, Real��Ƶ                       */
    CEDAR_MEDIA_FILE_FMT_MP1,           /* *.mp1, mp2/mpa��Ƶ                   */
    CEDAR_MEDIA_FILE_FMT_MP2,           /* *.mp2, mp2/mpa��Ƶ                   */
    CEDAR_MEDIA_FILE_FMT_AAC,           /* *.aac, MPEG4��Ƶ                     */
    CEDAR_MEDIA_FILE_FMT_AC3,           /* *.ac3, ��Ƶ�ļ�                      */
    CEDAR_MEDIA_FILE_FMT_DTS,           /* *.dts, DVD��Ƶ                       */
    CEDAR_MEDIA_FILE_FMT_AIF,           /* *.aif, Mac��Ƶ                       */
    CEDAR_MEDIA_FILE_FMT_AIFF,          /* *.aiff, Mac��Ƶ                      */
    CEDAR_MEDIA_FILE_FMT_AIFC,          /* *.aifc, Mac��Ƶ                      */
    CEDAR_MEDIA_FILE_FMT_AMR,           /* *.amr, �ƶ���Ƶ                      */
    CEDAR_MEDIA_FILE_FMT_MAC,           /* *.mac, APE������Ƶ                   */
    CEDAR_MEDIA_FILE_FMT_TTA,           /* *.tta, TTA��Ƶ                       */
    CEDAR_MEDIA_FILE_FMT_M4A,           /* *.m4a, MPEG4��Ƶ                     */
    CEDAR_MEDIA_FILE_FMT_CDA,           /* *.cda, CD��Ƶ                        */
    CEDAR_MEDIA_FILE_FMT_AU,            /* *.au, Sunѹ����Ƶ                    */
    CEDAR_MEDIA_FILE_FMT_ACC,           /* *.acc, ��Ƶ                          */
    CEDAR_MEDIA_FILE_FMT_MIDI,          /* *.midi, mid��Ƶ�ļ�                  */
    CEDAR_MEDIA_FILE_FMT_MID,           /* *.mid, mid��Ƶ�ļ�                   */
    CEDAR_MEDIA_FILE_FMT_RMI,           /* *.rmi, mid��Ƶ�ļ�                   */
    CEDAR_MEDIA_FILE_FMT_MP5,           /* *.mp5, ��Ƶ�ļ�                      */
    CEDAR_MEDIA_FILE_FMT_MPA,           /* *.mpa, mpeg��Ƶ�ļ�                  */
    CEDAR_MEDIA_FILE_FMT_MPGA,          /* *.mpga, mp3��Ƶ�ļ�                  */
    CEDAR_MEDIA_FILE_FMT_ACT,           /* *.act, ��Ƶ�ļ�                      */
    CEDAR_MEDIA_FILE_FMT_ATRC,          /* *.omg,oma,.aa3, sony openMG��Ƶ�ļ�  */

    //define video file file format
    CEDAR_MEDIA_FILE_FMT_AVI = 0x1000,  /* *.avi, ��Ӱ��Ƶ�ļ�                  */
    CEDAR_MEDIA_FILE_FMT_FLV,           /* *.flv, flash video��Ƶ�ļ�           */
    CEDAR_MEDIA_FILE_FMT_MPEG,          /* *.mpeg, mpegӰƬ��Ƶ�ļ�             */
    CEDAR_MEDIA_FILE_FMT_MPG,           /* *.mpg, mpegӰƬ��Ƶ�ļ�              */
    CEDAR_MEDIA_FILE_FMT_VOB,           /* *.vob, mpegӰƬ��Ƶ�ļ�              */
    CEDAR_MEDIA_FILE_FMT_DAT,           /* *.dat, VCD��Ƶ�ļ�                   */
    CEDAR_MEDIA_FILE_FMT_MP4,           /* *.mp4, mpeg-4ӰƬ��Ƶ�ļ�            */
    CEDAR_MEDIA_FILE_FMT_3GP,           /* *.3gp, �ƶ���Ƶ                      */
    CEDAR_MEDIA_FILE_FMT_MOV,           /* *.mov, QuickTimeӰƬ                 */
    CEDAR_MEDIA_FILE_FMT_WMV,           /* *.wmv, Windows media��/��Ƶ�ļ�      */
    CEDAR_MEDIA_FILE_FMT_ASF,           /* *.asf, Windows media��/��Ƶ�ļ�      */
    CEDAR_MEDIA_FILE_FMT_RM,            /* *.rm, realӰƬ                       */
    CEDAR_MEDIA_FILE_FMT_RMVB,          /* *.rmvb, realӰƬ                     */
    CEDAR_MEDIA_FILE_FMT_RAM,           /* *.ram, realӰƬ                      */
    CEDAR_MEDIA_FILE_FMT_SCM,           /* *.scm, realӰƬ                      */
    CEDAR_MEDIA_FILE_FMT_DIVX,          /* *.divx, ��Ӱ��Ƶ                     */
    CEDAR_MEDIA_FILE_FMT_MKV,           /* *.mkv, matroskaӰƬ��Ƶ�ļ�          */
    CEDAR_MEDIA_FILE_FMT_OGM,           /* *.ogm, oggӰƬ��Ƶ�ļ�               */
    CEDAR_MEDIA_FILE_FMT_M4V,           /* *.m4v, mpeg-4ӰƬ��Ƶ                */
    CEDAR_MEDIA_FILE_FMT_M4P,           /* *.m4p, mpeg-4ӰƬ��Ƶ                */
    CEDAR_MEDIA_FILE_FMT_M4B,           /* *.m4b, mpeg-4ӰƬ��Ƶ                */
    CEDAR_MEDIA_FILE_FMT_TP,            /* *.tp, mpeg-2ӰƬ��Ƶ�ļ�             */
    CEDAR_MEDIA_FILE_FMT_TPR,           /* *.tpr, mpegӰƬ��Ƶ�ļ�              */
    CEDAR_MEDIA_FILE_FMT_TS,            /* *.ts, mpeg-2ӰƬ��Ƶ�ļ�             */
    CEDAR_MEDIA_FILE_FMT_PVA,           /* *.pva, mpegӰƬ��Ƶ�ļ�              */
    CEDAR_MEDIA_FILE_FMT_PSS,           /* *.pss, mpegӰƬ��Ƶ�ļ�              */
    CEDAR_MEDIA_FILE_FMT_MPE,           /* *.mpe, mpegӰƬ��Ƶ�ļ�              */
    CEDAR_MEDIA_FILE_FMT_WV,            /* *.wv, mpegӰƬ��Ƶ�ļ�               */
    CEDAR_MEDIA_FILE_FMT_M2TS,          /* *.m2ts, m2ts��Ƶ                     */
    CEDAR_MEDIA_FILE_FMT_EVO,           /* *.evo, HD-DVD��Ƶ                    */
    CEDAR_MEDIA_FILE_FMT_RPM,           /* *.rpm, RealӰƬ                      */
    CEDAR_MEDIA_FILE_FMT_3GPP,          /* *.3gp, �ƶ���Ƶ                      */
    CEDAR_MEDIA_FILE_FMT_3G2,           /* *.3gp, �ƶ���Ƶ                      */
    CEDAR_MEDIA_FILE_FMT_3GP2,          /* *.3gp, �ƶ���Ƶ                      */
    CEDAR_MEDIA_FILE_FMT_QT,            /* *.qt, QuickTimeӰƬ                  */
    CEDAR_MEDIA_FILE_FMT_WMP,           /* *.wmp, Windows media��/��Ƶ�ļ�      */
    CEDAR_MEDIA_FILE_FMT_WM,            /* *.wm, Windows media��/��Ƶ�ļ�       */
    CEDAR_MEDIA_FILE_FMT_AMV,           /* *.avm, ѹ����Ƶ                      */
    CEDAR_MEDIA_FILE_FMT_DSM,           /* *.dsm, NDS��Ƶ                       */
    CEDAR_MEDIA_FILE_FMT_M1V,           /* *.m1v, ӰƬ��Ƶ                      */
    CEDAR_MEDIA_FILE_FMT_M2V,           /* *.m2v, ӰƬ��Ƶ                      */
    CEDAR_MEDIA_FILE_FMT_PMP,           /* *.pmp, ӰƬ��Ƶ                      */
    CEDAR_MEDIA_FILE_FMT_SMK,           /* *.smk, ӰƬ��Ƶ                      */
    CEDAR_MEDIA_FILE_FMT_BIK,           /* *.bik, BinkӰƬ��Ƶ                  */
    CEDAR_MEDIA_FILE_FMT_RAT,           /* *.rat, ratDVDӰƬ��Ƶ                */
    CEDAR_MEDIA_FILE_FMT_VG2,           /* *.vg2, XVDӰƬ��Ƶ                   */
    CEDAR_MEDIA_FILE_FMT_IVF,           /* *.ivf, IndeoӰƬ��Ƶ                 */
    CEDAR_MEDIA_FILE_FMT_VP6,           /* *.vp6, VP6ӰƬ��Ƶ                   */
    CEDAR_MEDIA_FILE_FMT_VP7,           /* *.vp7, VP7ӰƬ��Ƶ                   */
    CEDAR_MEDIA_FILE_FMT_D2V,           /* *.d2v, DVD2AVI ProjectӰƬ��Ƶ       */
    CEDAR_MEDIA_FILE_FMT_MOD,           /* *.mod, �������ʽ                    */
    CEDAR_MEDIA_FILE_FMT_M2P,           /* *.m2p, ӰƬ��Ƶ                      */
    CEDAR_MEDIA_FILE_FMT_VID,           /* *.vid, mp4�ֻ�����ӰƬ��Ƶ           */
    CEDAR_MEDIA_FILE_FMT_PMP2,          /* *.pmp2, ӰƬ��Ƶ                     */
    CEDAR_MEDIA_FILE_FMT_MTS,           /* *.mts, DV��Ƶ                        */
    CEDAR_MEDIA_FILE_FMT_F4V,           /* *.f4v, flash video��Ƶ�ļ�           */

    

    //user define format type
    CEDAR_MEDIA_FILE_FMT_PSR = 0x2000,  /* *.psr, �û��Զ����ý���ļ���ʽ      */
    CEDAR_MEDIA_FILE_FMT_DEV = 0x2001,  /* *.dev, ���豸�ļ�*/

    //�û��Զ���ĸ��ּ��ܸ�ʽ�ļ���������ʽ
    //CEDAR_MEDIA_FILE_FMT_USR_HGV = 0x2100,  //hgv��ʽ��ʵ������avi��video chunk�ļ���.

    //�û��Զ���3D�ļ�
    CEDAR_MEDIA_FILE_FMT_3DV = 0x2200,  /* *.3dv, 3DV video��Ƶ�ļ�, ά���Զ���3d��Ƶ��ʽ */

    //define unknown format type
    CEDAR_MEDIA_FILE_FMT_UNKNOWN = 0xffff, /* ý���ļ���ʽ�޷�ʶ��              */

    CEDAR_MEDIA_FILE_FMT_

} __cedar_media_file_fmt_t;


typedef enum __CEDAR_PIC_FMT
{
    CEDAR_PIC_TYPE_NONE=0,              /* û��ͼƬ��                           */
    CEDAR_PIC_TYPE_NOTSPT,              /* ��ͼƬ��,���޷�ʶ��                  */

    CEDAR_PIC_TYPE_

} __cedar_pic_fmt_t;


//�û��Զ�����Ч�˲�����Ƶ�������û���Ҫ�����趨��Ƶ�ε��˲�ϵ��
#define USR_EQ_BAND_CNT             (10)
#define USR_EQ_NEGATIVE_PEAK_VALUE  (-12)
#define USR_EQ_POSITIVE_PEAK_VALUE  (+12)
typedef enum __CEDAR_AUDIO_EQ           /* ��Ч���Ͷ���                         */
{
    CEDAR_AUD_EQ_TYPE_NORMAL=0,         /* ��Ȼ                                 */
    CEDAR_AUD_EQ_TYPE_DBB,              /* �ص���                               */
    CEDAR_AUD_EQ_TYPE_POP,              /* ����                                 */
    CEDAR_AUD_EQ_TYPE_ROCK,             /* ҡ��                                 */
    CEDAR_AUD_EQ_TYPE_CLASSIC,          /* �ŵ�                                 */
    CEDAR_AUD_EQ_TYPE_JAZZ,             /* ��ʿ                                 */
    CEDAR_AUD_EQ_TYPE_VOCAL,            /* ����                                 */
    CEDAR_AUD_EQ_TYPE_DANCE,            /* ����                                 */
    CEDAR_AUD_EQ_TYPE_SOFT,             /* ���                                 */
    CEDAR_AUD_EQ_TYPE_USR_MODE=0xff,    /* �û�ģʽ, pbuffer = __s8 UsrEqArray[USR_EQ_BAND_CNT],
                                           ÿ�����ֵΪ:
                                           USR_EQ_POSITIVE_PEAK_VALUE~USR_EQ_NEGATIVE_PEAK_VALUE        */
    CEDAR_AUD_EQ_TYPE_

} __cedar_audio_eq_t;

typedef enum  __CEDAR_3D_OUT_MODE_T
{
    CEDAR_DISP_3D_OUT_MODE_LR = 0x00,//LRLR
    CEDAR_DISP_3D_OUT_MODE_RL = 0x01,//RLRL
    CEDAR_DISP_3D_OUT_MODE_MIX_1 = 0x02,//
    CEDAR_DISP_3D_OUT_MODE_MIX_2 = 0x03,//
}__cedar_3d_out_mode_t;

typedef enum __CEDAR_DISPLAY_MODE_T
{
    CEDAR_DISP_2D_MODE_    = 0,
    CEDAR_DISP_3D_MODE_PLR = 1,
    CEDAR_DISP_3D_MODE_PRL = 2,
    CEDAR_DISP_3D_MODE_PTD = 3,
    CEDAR_DISP_3D_MODE_PDT = 4,
    CEDAR_DISP_3D_MODE_IAB = 5,
    CEDAR_DISP_3D_MODE_IBA = 6,
    CEDAR_DISP_3D_MODE_URL = 7,
    CEDAR_DISP_3D_MODE_MVC = 8,
    CEDAR_DISP_3D_MODE_UNKNOWN
}__cedar_display_mode_t;

//video display mode inforamtion
typedef struct __CEDAR_DISPLAY_MODE_INFO     //3dv��Ƶ�ļ�ͷ��Ϣ
{
    __cedar_display_mode_t          display_mode;	  //   DISP_2D_MODE_��DISP_3D_MODE_PLR��DISP_3D_MODE_PRL 
    __u16			                display_width;
	__u16			                display_height;
    __u16			                mvcdisplay_width;
	__u16			                mvcdisplay_height;
    __u32                           reserved[13];
} __cedar_display_mode_info;

typedef struct __CEDAR_DISPLAY_MODE_OUT
{
    __u32                         trd_display_enable;  //  1) 0: 2D��ʾ;  2) 1: 3D��ʾ
    __cedar_3d_out_mode_t         trd_mode;      //  DISP_3D_OUT_MODE_LR/DISP_3D_MODE_RL
    __u32                         half_mode;    //         1) 0: ���;  2) 1: ȫ��
    __u32                         reserved[5];
}__cedar_display_mode_Out; 



typedef enum __CEDAR_GET_SUB_INF
{
    CEDAR_GET_SUB_INF_NONE = 0, //invalid, return no data

    CEDAR_GET_SUB_INF_ALL,      //get all subtitle item information
    CEDAR_GET_SUB_INF_ITEM,     //get subtitle item information according to the given time

    CEDAR_GET_SUB_INF_

} __cedar_get_sub_inf_t;

typedef struct __BUFFER_USAGE
{
    __u8    vbs_flg;            //vbs �Ƿ���Ч
    __u8    abs_flg;            //abs �Ƿ���Ч
    __u8    ext_flg[2];         //��չ��
    
    __s32   vbs_usage;          // vbsռ����, 0~100, ����0%~100%
    __s32   abs_usage;          // absռ����, 0~100, ����0%~100%
    __s32   ext[2];            //��չ��
}__buffer_usage_t;

typedef struct __CEDAR_TAG_INF
{
    __u8    tag_magic[4];            /* ý���ļ���ǩ�ı�־��'A'/'V','T','A','G' */

    __s32   file_offset;                /* ý���ļ��Ķϵ����ļ��ڵ�ƫ��         */
    __s32   time_offset;                /* ý���ļ��Ķϵ��ʱ��ƫ��, ms             */
    __u8    decinf[CEDAR_TAG_INF_SIZE]; /* ���������ֳ��ָ���Ϣ                 */
    __u32   total_time;                 //�ļ����ŵ���ʱ��
    __s32   reserved[3];                //������չ��

}__cedar_tag_inf_t;     //ע��__cedarlib_tag_inf_t


//the information of the media file which will be input to play
typedef struct __CEDAR_MEDIA_FILE_INF
{
    char    file_path[CEDAR_FPATH_SIZE];/* ý���ļ��ľ���·����                 */

    __u32   tag_inf_validflag;          /* �ļ��ϵ���Ϣ�Ƿ���Ч�ı�־, =0(invalid), =1(valid)           */
    __cedar_tag_inf_t   tag_inf;        /* ý���ļ��ϵ㲥�ŵ������Ϣ           */

} __cedar_media_file_inf;

typedef struct __CEDAR_AUDIO_INF
{
    __u32       uAbsFmt;                /* ��Ƶ�������ͣ�if(uAbsFmt == HERB_ABS_TYPE_NONE), ��
                                           �������������Ƶ����                             */
    __u32       uVolume;                /* ¼��������ֵ��������÷Ƿ��������defaultֵ,MIN_VOLUME_VAL~MAX_VOLUME_VAL      */
    __u32       uChn;                   /* ¼��������ģʽ��������÷Ƿ��������defaultֵ,HERB_AUDIO_CHN_MONO    */
    __u32       uSampWidth;             /* ¼���������λ��������÷Ƿ��������defaultֵ  */
    __u32       uAudioBps;              /* ��������Ƶ���������ʣ�������÷Ƿ��������defaultֵ����λ:bit/s    */
    __u32       uAudioSampRate;         /* ��Ƶ�����Ĳ����ʣ�������÷Ƿ��������defaultֵ  */
    __u32       uAudioSource;           /* ��ƵԴ��������÷Ƿ��������defaultֵ            */
    __s32       frame_style;            /* for 0:add head,1:raw data                        */

    //����Ĳ���ҪӦ�ó�����д����Ϣ,��Щ��Ϣ��herb�ڲ���д�������ڲ�������ģ�顣
    __s32       sample_cnt_per_frame;   /* ÿ��audio frame������sample������ */
} __cedar_audio_inf_t;

typedef struct __CEDAR_VIDEO_INF
{
    __u32       uVbsFmt;                /* ��Ƶ������, if(uVbsFmt == SW_CODEC_TYPE_NONE), ������������ص���Ƶ����, __herb_codec_t */
    __s32       nWidth;                 /* Ҫ�������Ƶͼ��Ŀ�� */
    __s32       nHeight;                /* Ҫ�������Ƶͼ��ĸ߶� */
    __u32       uVideoFrmRate;          /* ��Ƶ������֡��,������÷Ƿ��������defaultֵ, ��ֵ�Ŵ�1000��     */
    __s32       nVideoBps;              /* ��Ƶ����������,������÷Ƿ��������defaultֵ����λ:bit/s     */

    __s32       color_format;           /* yuv, bayerRGB(4��), PIXEL_YUV420*/
    __s32       color_space;            /* BT601, bt709*/

    __s16       qp_max;
    __s16       qp_min;
    __s32       maxKeyInterval;         /*keyframe Interval��ָ2���ؼ�֮֡���P֡���������             */

    //__u32       uVideoSource;         /* ��ƵԴ��������÷Ƿ��������defaultֵ                      */
} __cedar_video_inf_t;

typedef struct tag_CedarHerbMediaFileInf
{
    //Ҫ¼�Ƶ��ļ���ʽ
    __u32       uFileFmt;                   /* Ҫ¼�Ƶ��ļ���ʽ,__cedar_media_file_fmt_t                         */
    char        file_path[CEDAR_FPATH_SIZE]; /* ¼�Ƶ�ý���ļ�����ľ���·���������δ���ã��򱣴浽default·��, HERB_FPATH_SIZE  */
		 //��Ƶ�����Ϣ
    __cedar_audio_inf_t    audio_inf;        /* ��Ƶ¼�Ƹ�ʽ��Ϣ                                         */

    //��Ƶ�����Ϣ
    __cedar_video_inf_t    video_inf;        /* ��Ƶ¼�Ƹ�ʽ��Ϣ,�ο�__video_encode_format_t             */

    //���⣬ʱ���
    char        title[CEDAR_FPATH_SIZE];
    __s32       create_time;                /* ���ֵĹ���壬��mux������Ӧ�ó�������                  */

		__u8        reserved[2048];              // ����2k�ֽ�,�Ժ���չ
} CedarHerbMediaFileInf;    //����¼�Ƶ��ļ���Ϣ

//the setting information for cedar sub-system
//typedef struct __CEDAR_SETTING_INF
//{
    //��Ƶ���ŵ��������
//    __u32       volume;                     /* ����ֵ, ֻ�Ǽ�¼,ʵ������                           */
//    __u32       eq;                         /* ��Ч����, ��playmediafile()�ͻ�����ʱ���ardr      */
//    __u32       channel;                    /* ����ģʽ,����                         */
//    __u32       vpsspeed;                   /* �����ٶ�, ��playmedifile(),������,�ͱ�������ʱֱ�����ardr                         */
//    __u32       ab_loopcnt;                 /* A-B����ѭ������,��playmedifile()�ͻ�����ʱ���adec,ʵ�����Ѿ�������.                  */
//    __u32       spectrum;                   /* �Ƿ�ʹ��Ƶ�׽���,���ļ���������ʱ,������ardr                 */
//    __s32	    swgain;				//�������ֵ
//    __s16       usr_eq_filter[USR_EQ_BAND_CNT]; /* �û��Զ�����Ч���˲���Ƶ�ε��˲�ϵ�� */
//
//    __u8        volume_flag;                /* volume�����Ƿ���Ч�ı�־,������1,ʵ������         */
//    __u8        eq_flag;                    /* eq�����Ƿ���Ч�ı�־,������1,��Զ��Ч             */
//    __u8        channel_flag;               /* channel�����Ƿ���Ч�ı�־,��1,ʵ������        */
//    __u8        vpsspeed_flag;              /* vpsspeed�����Ƿ���Ч�ı�־,��1,ʵ��û��       */
//    __u8        ab_loopcnt_flag;            /* ab_loopcnt�����Ƿ���Ч�ı�־,ʵ����������     */
//    __u8        spectrum_flag;              /* spectrum�����Ƿ���Ч�ı�־,��1,ʵ��û��       */
//    __s32       aud_suffix;                 /* ��ǰ���ŵ���Ƶ������Ƶ��������±�,����   */
//
    //��Ƶ���ŵ��������
//    __s32       vid_layer_hdl;              /* ��Ƶ����ͼ����                 */
//    __rect_t    vid_window;                 /* ��Ƶͼ�񴰿�����, ��Ƶ�������Ϣ,������Ļ�ϻ�����������ʾ�������λ�úͿ�� */
//    __s32       vid_show_mode;              /* ��Ƶͼ����ʾ����ģʽ             */
//    __u32       frpic_hold_time;            /* ���������״̬����ƵͼƬ��ͣ��ʱ��                   */
//    __s32       nCkEnableValue;             //* VirtualDE_DispLayerInfo->ck_enable�Ƿ���1,��1��ʾ��Ƶͼ������colorKey��͸��.���������app������Ҫ����,Ĭ��Ϊ0.
//
//    __s8        vid_layer_hdl_flag;         /* vid_layer_hdl�����Ƿ���Ч�ı�־                      */
//    __s8        vid_window_flag;            /* vid_window�����Ƿ���Ч�ı�־                         */
//    __s8        vid_show_mode_flag;         /* vid_show_mode�����Ƿ���Ч�ı�־                      */
//    __s8        frpic_hold_time_flag;       /* frpic_hold_time�����Ƿ���Ч�ı�־                    */
//
    //������Ƶ���Ź�����������
//    __u32       frspeed;                    /* ����������ٶ�                                       */
//    __u8        frspeed_flag;               /* frspeed�����Ƿ���Ч�ı�־                            */
//
//} __cedar_setting_inf_t;


//audio format information
typedef struct  __AUDIO_BS_INFO
{
    __s32       codec_type;     //__cedar_audio_fmt_t
    __s32       channels;
    __s32       bits_per_sample;
    __s32       sample_per_second;
    __s32       avg_bit_rate;
    __s32       max_bit_rate;

    __s32       data_encode_type;    //__cedar_subtitle_encode_t,��ʾlang[]���ַ���������.
    __u8        lang[MAX_LANG_CHAR_SIZE];
} __audio_bs_info_t;


//video format inforamtion
typedef struct VIDEO_BS_INFO
{
    __u16       codec_type; //__cedar_video_fmt_t, CEDAR_VBS_TYPE_VC1
    __u16       width;
    __u16       height;
    __u16       frame_rate;
    __s32       avg_bit_rate;
    __s32       max_bit_rate;
} __video_bs_info_t;
typedef struct{
    __u8        nVidStrmNum;    //video number, <= nVidStrmMaxCnt
    __s32       nVidStrmMaxCnt; //����VidStrmListArray��Ԫ�ظ���
    __video_bs_info_t        *VidStrmListArray;
} __vidstream_profile_v2_t;


#define PALETTE_CNT         (32)
typedef struct{
    __s8    valid_flg;
    __s32   palette_num;        //valid palette element
    __s32   palette_ary[PALETTE_CNT];//RGB��ʽ�ĵ�ɫ��
}__palette_t;

//typedef enum tag_CEDAR_SUBTITLE_SOURCE{
//    CEDAR_SUBTITLE_POS_NONE   = 0,    //no subtitle exist
//    CEDAR_SUBTITLE_POS_EMBED  = 1,    //embeded subtilte in video file
//    CEDAR_SUBTITLE_POS_EXT    = 2,    //extern subtitle in subtitle file
//    CEDAR_SUBTITLE_POS_,
//} __cedar_subtitle_source_t;

//subtitle format information
typedef struct {
    __s32       subtitle_coming_type;  //embeded subtitle or extern subtitle, __cedar_subtitle_source_t(__subtitle_source_t)
    __s32       subtitle_type;  // srt, sub, etc.__cedar_lyric_fmt_t
    __palette_t palette_info;   //�����SUB��ʽ����Ļ����Ҫ����Ϣ
    __s32       stream_idx;     //in one file(e.g,sub),there will exist several streams, but in srt file, stream_idx means nothing.
    __s32       data_encode_type;    //__cedar_subtitle_encode_t
    __u8        lang[MAX_LANG_CHAR_SIZE];
} __subtitle_stream_info_t;

#define MAX_AUDIO_STREAM_NUM    (16)    //(4), (16)
//information for audio stream.
typedef struct{
    __u8        nAudStrmNum;    //audio number.<=MAX_AUDIO_STREAM_NUM.
    __audio_bs_info_t        AudStrmList[MAX_AUDIO_STREAM_NUM];
} __audstream_profile_t;

typedef struct{
    __u8        nAudStrmNum;    //audio number, <= nAudStrmMaxCnt
    __s32       nAudStrmMaxCnt; //����AudStrmListArray��Ԫ�ظ���
    __audio_bs_info_t        *AudStrmListArray;
} __audstream_profile_v2_t;

//the information from media file, 20111229ע��
//typedef struct __MEDIA_FILE_INFO
//{
//    __s8        nHasAudio;          //ý���ļ��Ƿ�����Ƶ��
//    __s8        nHasLrc;            //ý���ļ��Ƿ�����Ƶ��
//    __s8        nHasVideo;          //ý���ļ��Ƿ�����Ļ��
//
//    __u8        nStreamNum;         //ý���ļ�����������
//    __u8        nAudStrmNum;        //ý���ļ�����Ƶ���ĸ���
//    __u8        nVidStrmNum;        //ý���ļ�����Ƶ���ĸ���
//    __u8        nLrcStrmNum;        //ý���ļ�����Ļ���ĸ���
//
//    __u32       nDuration;          //ý���ļ��ĳ�����ʱ��
//
//    __audio_bs_info_t   *pAudStrmList;  //��Ƶ����Ϣ���е�ͷָ��
//    __video_bs_info_t   *pVidStrmList;  //��Ƶ����Ϣ���е�ͷָ��
//
//    __u8        *pAuthor;           //���߱�ǩ
//    __s32       nAuthorLen;         //���߱�ǩ����
//    __u8        *pCopyRight;        //��Ȩ��ǩ
//    __s32       nCopyRightLen;      //��Ȩ��ǩ����
//    __u8        *pTitle;            //�����ǩ
//    __s32       nTitleLen;          //�����ǩ����
//    __u8        *pGenre;            //���ɱ�ǩ
//    __s32       nGenreLen;          //���ɱ�ǩ����
//
//} __media_file_info_t;


//information for lyric/sutitle item
typedef struct{
    __u8        backpixelColorCode;              //background pixel colorcode
	__u8        patpixelColorCode;               //pattern pixel colorcode
	__u8        pixel1ColorCode;                 //emphasis pixel-1 colorcode
    __u8        pixel2ColorCode;                  //emphasis pixel-2 colorcode
} __colorcode_t;


typedef struct{
    __u8        backpixelContrast;              //background pixel contrast
	__u8        patpixelContrast;               //pattern pixel contrast
	__u8        pixel1Contrast;                 //emphasis pixel-1 contrast
    __u8        pixel2Contrast;                 //emphasis pixel-2 contrast
} __contrast_t;


typedef struct __CEDAR_SUBTITLE_ITEM
{
    __u32       uPts;           // time stamp of the subtitle item, (ms)
    __u32       uDuration;      // duration of the item, (ms)
    __u16       uDataType;      // subtitle data type, text,bitmap,encode type,��Ļ������
                                //__cedar_subtitle_encode_t
    __s32       nPicWidth;      //�����SUB��ͼ���ʽ����Ļ����Ҫ���Ϳ����Ϣ
    __s32       nPicHeight;
    __colorcode_t   colorCode;  //�����SUB��ͼ���ʽ����Ļ����Ҫ����color code�Ա�ӵ�ɫ��ѡ����ɫ
    __contrast_t    contrast;   //�����SUB��ͼ���ʽ����Ļ����Ҫ����contrast ��ȷ���Ƿ���ʾ
    __s32       uPtsSize;       // size of the time stamp offset
    __s32       uSubSize;       // size of the subtitle data
    __s16       *pPtsOffset;    // time stamp offset of every character
    __u8        *pSubtitle;     // subtitle data buffer

} __cedar_subtitle_item_t;

//information for all lyric stream's description in videofile.
#define SUBTITLE_MAX_NUM    (8)
typedef struct{
    __u8        nSubtitleStrmNum;    //lyric number.<=SUBTITLE_MAX_NUM.
    __subtitle_stream_info_t        SubtitleStreamList[SUBTITLE_MAX_NUM];
} __subtitle_profile_t;

typedef struct{
    __u8        nSubtitleStrmNum;    //lyric number.<=nSubtitleStrmMaxCnt.
    __s32       nSubtitleStrmMaxCnt;    //����SubtitleStreamArray��Ԫ�ظ���
    __subtitle_stream_info_t    *SubtitleStreamArray;
} __subtitle_profile_v2_t;

//* subtitle 


typedef enum MAPLE_SUBTITLE_COLOR_MODE
{
    MAPLE_SUB_COLOR_BLACK       = 0x80,    
    MAPLE_SUB_COLOR_RED         = 0x81,  
    MAPLE_SUB_COLOR_GREEN       = 0x82,  
    MAPLE_SUB_COLOR_YELLOW      = 0x83,  
    MAPLE_SUB_COLOR_BLUE        = 0x84,  
    MAPLE_SUB_COLOR_MAGENTA     = 0x85,  
    MAPLE_SUB_COLOR_CYAN        = 0x86,  
    MAPLE_SUB_COLOR_WHITE       = 0x87,  

    MAPLE_SUB_COLOR_
}maple_subtitle_color_mode_t;

typedef enum MAPLE_SUBTITLE_STYLE_SIZE_MODE
{
    MAPLE_SUB_STYLE_SMALL_SIZE        = 0x88,  
    MAPLE_SUB_STYLE_MIDDLE_SIZE       = 0x89,    
    MAPLE_SUB_STYLE_NORMAL_SIZE       = 0x8a,    
    
    MAPLE_SUB_STYLE_SIZE
}maple_subtitle_style_size_t;


typedef enum MAPLE_SUBTITLE_DISPLAY_MODE
{
    MAPLE_SUB_DISPLAY_MODE_HD_SIDE            = 0x01,    
    MAPLE_SUB_DISPLAY_MODE_SD_SIDE            = 0x02,    
    MAPLE_SUB_DISPLAY_MODE_SD_WIDE_SIDE       = 0x03,    
    MAPLE_SUB_DISPLAY_MODE_MOBILE             = 0x04,   

    MAPLE_SUB_DISPLAY_MODE_UNKNOW,  
    MAPLE_SUB_DISPLAY_MODE_
}maple_subtitle_display_mode_t;

#define ISDBDEC_MAX_LINE_NUM        10   //һ����Ļ����������
#define DVBSUBDEC_MAX_LINE_NUM        10   //һ����Ļ����������
#define DVBTTXDEC_MAX_LINE_NUM				10

//picture info
typedef struct {
    __s32   pic_type;   //ͼ���ʽ��JPEG, BMP�ȣ� IMG_FORMAT_BMP
    SIZE    pic_area;   //ָ��Ҫ���ͼ����, ������4�ı���
    __s32   pic_size;   //sizeof(pic_priv)
    __s32   valid_len;  //ת�����ͼ���ļ�ʵ��ռ�õ�buffer��С, <= pic_size
    void    *pic_priv;  //��������������PALLOC���洢ת���õ�ͼ��
} __cedar_pic_info_t;

//define cedar buffer parmater
typedef struct __CEDAR_BUF_INF
{
    void        *buf;           // buffer pointer, __cedar_subtitle_item_tΪ��λ.
    __u32       size;           // item size, based on item, item size is user define

} __cedar_buf_inf_t;


typedef struct __CEDAR_USR_FOP
{
    __pCBK_t usr_fread;                     /*�ص�����(���ļ�)*/
    __u32 fp;                               /*�ļ����*/
    __cedar_media_file_fmt_t media_fmt;     /*��ý���ļ��ļ���ʽ*/
    __s32 file_size;                        /*�ļ�����*/
    __u32 flag;
    __pCBK_t usr_fseek;                     //���Բ�ʵ��,��usr_freadͳһ����Ϳ���.
    __s8    *pFilePath;                     //�ļ�·��+�ļ���, ����cedar��Ѱ�����Ļ����Ҫ
    __pCBK_t usr_fwrite;                     /*�ص�����(���ļ�)*/
    __u32 reserved[2];                      /*����,������Ϊ0,*/
} __cedar_usr_file_op_t;


typedef struct
{
    __u32 fp;
    void *buffer;
    __s32 size;
    __s32 pos;
}__cedar_usr_file_op_para_t;

typedef struct tag_SrcScnWinPara
{
    __s32       nSrcPixRatio;   //ԭͼ�����ؿ�߱�,1000��ʾ1:1.
    __rect_t    SrcWin; //ԭͼ������, app���û����, ת�����ֵҲ��������
    __rect_t    ScnWin; //��ʾ����, app���û����, ת�����ֵҲ��������
}SrcScnWinPara; //��CEDAR_CMD_SET_VID_SHOW_MODEʹ�õ�callback���������ݽṹ.


typedef	struct __CEDAR_PCM_INFO
{
//input infor
	__s32	SampleRate;
	__u16*	PCMPtr;
	__s32	PcmLen;//sample num;    inputA��inputB�ڵ���ǰ��Ҫmix��sample��,���ú���ʣ��û��mix��sample��.
	__s32	Chan;  //channel num

//reserved
    int		preamp;//-20 -- 20 db	
}__cedar_pcm_info_t;

//����CEDAR_PLAY_AUX_WAV_FILE��ģʽ
typedef enum __CEDAR_PLAY_AUX_WAV_MODE
{
    CEDAR_PLAY_AUX_WAV_MODE_APPEND,//�����ǰ�Ѿ���δ������ϵ�AUX WAV���ݣ���ǰ����׷�ӵ������ݵĺ��沥��
    CEDAR_PLAY_AUX_WAV_MODE_TRUNCATE,//�����ǰ�Ѿ���δ������ϵ�AUX WAV���ݣ��������ǰ���ݣ����ŵ�ǰ���õ�����
    CEDAR_PLAY_AUX_WAV_MODE_MIX,//�����ǰ�Ѿ���δ������ϵ�AUX WAV���ݣ�����ǰ�����ݺ͵�ǰ���õ����ݽ��л���
    CEDAR_PLAY_AUX_WAV_MODE_IGNORE,//�����ǰ�Ѿ���δ������ϵ�AUX WAV���ݣ�����Ե�ǰ���õ�����
    CEDAR_PLAY_AUX_WAV_MODE_MAX
}__cedar_play_aux_wav_mode_t;

typedef enum __CEDAR_AUDIO_DEV_QUERY_BUF_SIZE_TYPE
{
    CEDAR_AUDIO_DEV_QUERY_BUF_SIZE_NONE = 0,      /* δ����Ҫ��ȡ�ռ������           */
    CEDAR_AUDIO_DEV_QUERY_BUF_SIZE_DATA,          /* ��ȡ�������ڵ�����size           */
    CEDAR_AUDIO_DEV_QUERY_BUF_SIZE_FREE,          /* ��ȡ�������ڵĿ��пռ�size       */
    CEDAR_AUDIO_DEV_QUERY_BUF_SIZE_TOTAL,         /* ��ȡ�������ܿռ�                 */
    CEDAR_AUDIO_DEV_QUERY_BUF_SIZE_
} __cedar_audio_dev_query_buf_size_type_t;

#endif  /* _MOD_CEDAR_H_ */

