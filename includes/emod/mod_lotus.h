/*
********************************************************************************
*                                    eMOD
*                   the Easy Portable/Player Develop Kits
*                               mod_lotus sub-system
*
*          (c) Copyright 2009-2010, Allwinner Microelectronic Co., Ltd.
*                              All Rights Reserved
*
* File   : mod_lotus.h
* Version: V1.0
* By     : Eric_wang
* Date   : 2009-12-21
********************************************************************************
*/
#ifndef _MOD_LOTUS_H_
#define _MOD_LOTUS_H_

#define OVERLAY_MAX_SRC_INGROUP 20              /* һ��block������������ԴͼƬ��Ŀ*/
#define OVERLAY_MAX_LIST_LEN 40                 /* һ��block������ܹ���ʾ��ͼƬ��Ŀ*/
#define LOTUS_FPATH_SIZE         (1024*4)       /* ý���ļ�·�����������ֵ  */
#define LOTUS_FILE_TITLE_SIZE    (1024)
#define LOTUS_FRAME_RATE_BASE         1000

#define LOTUS_MUTE_ON       (1)
#define LOTUS_MUTE_OFF      (0)

/*******************************************************************************
* type name: __lotus_cmd_set_t
* Description: 
*    ý��¼���м�������б�(IOCtrol), ���ƺ���ԭ��:
*    __s32 esMODS_MIoctrl(__mp *mp, __u32 cmd, __s32 aux, void *pbuffer)
*    mp: lotus�м��ģ����
*    cmd: __lotus_cmd_set_t�����������
*    aux: lotus object, �� LOTUS_CMD_NEW_OBJ �����ȡ
*    pbuffer: �������������/�������, ��ͬ���������벻ͬ��
*             һ����˵������ǽṹ����Ҫ����ָ�룬ö��ֱֵ�Ӵ������ֵ
*    ret = EPDK_OK/EPDK_FAIL 
*******************************************************************************/
typedef enum __LOTUS_CMD_SET           
{
    LOTUS_CMD_NEW_OBJ                     = 0x1100,       /* �½�һ��lotus object , ret = lotus object*/
    LOTUS_CMD_DEL_OBJ                     = 0x1101,       /* �½�һ��lotus object , ret = lotus object*/
    LOTUS_CMD_SET_CSI                     = 0x1102,       /* ����csi �豸��lotus obj, pbuffer = index of csi dev */
    LOTUS_CMD_ENABLE_PLAYBACK             = 0x1103,       /* �Ƿ���ҪԤ��, pbuffer = 0��ʾ�ر�Ԥ��, 1��ʾ��Ԥ��*/



    //�ļ���ʽ������Ƶ�����ʽ���ã�����ֶ����ز���
    LOTUS_CMD_SET_MODE                    = 0x1104,       /* IDLE�µ��ã�����ʹ��ģʽ�������ա�¼����¼��ģʽ, 
                                                           * pbuffer = __lotus_record_mode_t��
                                                           */
                                                                 
    LOTUS_CMD_SET_MEDIAFILE               = 0x1105,       /* IDLE,STANDBY����¼�Ƶ�ý���ļ��ĸ�ʽ�Ͳ�����Ϣ��
                                                           * pbuffer = (__lotus_media_file_inf *)��
                                                           * ������LOTUS_CMD_SET_MODE֮�����      
                                                           */
                                                                 
    LOTUS_CMD_GET_OUT_MODE_ALL            = 0x1106,       /* �õ�CSI_DRV֧�ֵ������ʽ�б�
                                                           * (pbuffer = __csi_mode_all_t*),
                                                           */
                                                                 
    LOTUS_CMD_SET_PREVIEW_OUT_MODE        = 0x1107,       /* ����preview�µ�ͼ��Ĵ�С��֡�ʺ�������Ϣ, 
                                                           * (pbuffer = __csi_mode_t*)��
                                                           * ��__csi_mode_all_t��ѡһ�����ü��� 
                                                           */
                                                                 
    LOTUS_CMD_NEED_PREVIEW_PIC_FOR_RECORD = 0x1108,       /* ����¼��ʱ�Ƿ���Ҫ�ѵ�һ֡����ΪͼƬ, 
                                                           * pbuffer = FB*, Ӧ�ó����Լ����ú� 
                                                           */

    //��������
    LOTUS_CMD_START                       = 0x1200,       /* ״̬:idle->standby,�����м��,
                                                           * ������ִ�к��û����Կ���ͼ��
                                                           */
    LOTUS_CMD_STOP                        = 0x1201,       /* ֹͣ�м����ж��video_plybkģ�����������ģ�飬׼���˳�*/
    LOTUS_CMD_START_RECORD                = 0x1202,       /* ��ʼ¼�� */
    LOTUS_CMD_PAUSE_RECORD                = 0x1203,       /* ��ͣ¼�� */
    LOTUS_CMD_CONTI_RECORD                = 0x1204,       /* �ָ�¼�� */
    LOTUS_CMD_STOP_RECORD                 = 0x1205,       /* ֹͣ¼�� */
    LOTUS_CMD_TAKE_IMAGE                  = 0x1206,       /* ����, 
                                                           * pbuffer = Ŀ��(FB*)���ڴ�����ѷ��䣬������ARGB��ʽ��
                                                           * ���ڴ������ͼ��buffer 
                                                           */
    LOTUS_CMD_GET_STATUS                  = 0x1207,       /* ��ȡý��¼���м����ǰ״̬ */
    LOTUS_CMD_HOLD_ON                     = 0x1208,       /* ���浱ǰ�ļ���ͬʱ����¼�� */
    LOTUS_CMD_SET_HOLDING_TIME            = 0x1209,       /* ���ô���ڻ���buffer�еı���֡��ʱ��*/
    LOTUS_CMD_SEEK_TIME                   = 0x120A,       /* �������ÿ�ʼ¼�Ƶ�ʱ��, �ڿ�ʼ¼��֮ǰ����, 
                                                           * pbuffer = time(ms),pbuffer < 0��ʾ�ӵ�ǰʱ��֮ǰ��pbuffer ms��ʼ
                                                           * ¼�� pbuffer > 0��ʱ��֧��*/


    
    LOTUS_CMD_GET_CUR_TIME                = 0x1300,       /* ��ȡ��ǰ¼��ʱ�� , 
                                                           * return = current encode time(ms) 
                                                           */
    LOTUS_CMD_GET_REC_FILELEN             = 0x1301,       /* ��ȡ��ǰ¼�����ݳ��� , 
                                                           * pbuffer = (__s64 *),current length(bytes) 
                                                           */

    // ��Ƶ���
    LOTUS_CMD_MUTE                        = 0x1400,       /* ������/��, ���ú�����¼���ļ�����
                                                           * (pbuffer = LOTUS_MUTE_ON/LOTUS_MUTE_OFF) 
                                                           */
    LOTUS_CMD_SET_REC_VOLUME			= 0x1401,		/* ����¼��ʱ¼�������Ĵ�Сpbuffer = volume*/
    
    //��Ƶ���
    LOTUS_CMD_ADD_OVERLAY_SRC             = 0x1500,       /* ���һ��block��Դ, 
                                                           * (pbuffer = __lotus_overlay_src_init_t*) 
                                                           */
    LOTUS_CMD_SHOW_OVERLAY_BLK            = 0x1501,       /* ��ʾһ��block, 
                                                           * (pbuffer = __lotus_overlay_dis_par_t*) 
                                                           */
    
    LOTUS_CMD_SET_MD_PARA                 = 0x1530,       /* �����ƶ������ֵ, 
                                                           * (pbuffer = __lotus_motion_dectect_para_t*) 
                                                           */
    LOTUS_CMD_MD_ONOFF                    = 0x1531,       /* ����/�ر��ƶ����, 
                                                           * (pbuffer = 1)��ʾ����, (pbuffer = 0)��ʾ�ر�
                                                           */
    LOTUS_CMD_GET_MD_DETECT               = 0x1532,       /* ��ȡ�ƶ������, 
                                                           * (*pbuffer = 1)��ʾ��⵽�ƶ�,(*pbuffer = 0)��ʾδ��⵽�ƶ�
                                                           */
    LOTUS_CMD_REG_SYNC_CB                 = 0x1533,       /* ����ͬ���ص�����, 
                                                           * �ڿ�ʼ�����, ÿ����һ֡�����GetCurExtData��ȡ��������,
                                                           * muxerд�ļ���Ϻ�����DealWithExtData
                                                           * (pbuffer = __lotus_sync_callback_t *)
                                                           */
    LOTUS_CMD_SET_PRESCALE                = 0x1544,       /* ������Ƶ֡�ɼ�ϵ��
                                                           * (pbuffer = prescale)
                                                           * ���ú�ÿprescale֡����һ֡, ��prescale<0, ��ʾÿ֡����prescale��
                                                           */

    //��ʾ���
    LOTUS_CMD_SET_VID_LAYERHDL            = 0x1600,       /* ����ͼ��, pbuffer = layer handle ,*/
    LOTUS_CMD_SET_VID_SHOW_MODE           = 0x1601,       /* ������ʾģʽ (pbuffer = __lotus_vid_window_ratio_mode_t)*/
    LOTUS_CMD_SET_VID_WINDOW              = 0x1602,       /* ������ʾ�Ĵ��ڴ�С,(pbuffer = __rect_t *)*/
    LOTUS_CMD_SWITCH_VID_SHOW             = 0x1603,       /* �Ƿ���ʾ��Ƶ, 
                                                           * (pbuffer = 1),����Ƶ��ʾ��(pbuffer = 0),�ر���Ƶ��ʾ 
                                                           */
    LOTUS_CMD_SHOW_HALF                   = 0x1604,       /* ������ͷ��·����ʱ��ѡ������һ·�ź���ʾ��
                                                           * pbuffer = (__lotus_src_scn_set_t ) 
                                                           */
    LOTUS_CMD_SET_PREVIEW_PRESCALE		= 0x1605,		/*	����ÿpbuffer ֡����ʾһ֡*/
    //CSI�������
    LOTUS_CMD_SET_COLOR_EFFECT            = 0x1700,       /* pbuffer = __csi_color_effect_t */
    LOTUS_CMD_GET_COLOR_EFFECT            = 0x1701,       /* ret = __csi_color_effect_t */
    LOTUS_CMD_SET_AWB                     = 0x1702,       /* pbuffer = __csi_awb_t */
    LOTUS_CMD_GET_AWB                     = 0x1703,       /* ret = __csi_awb_t */
    LOTUS_CMD_SET_AE                      = 0x1704,       /* pbuffer = __csi_ae_t */
    LOTUS_CMD_GET_AE                      = 0x1705,       /* ret = __csi_ae_t */
    LOTUS_CMD_SET_BRIGHT                  = 0x1706,       /* pbuffer = __csi_bright_t */
    LOTUS_CMD_GET_BRIGHT                  = 0x1707,       /* ret = __csi_bright_t */
    LOTUS_CMD_SET_CONTRAST                = 0x1708,       /* pbuffer = __csi_contrast_t */
    LOTUS_CMD_GET_CONTRAST                = 0x1709,       /* ret = __csi_contrast_t */
    LOTUS_CMD_SET_BAND                    = 0x170A,       /* pbuffer = __csi_band_t */
    LOTUS_CMD_GET_BAND                    = 0x170B,       /* ret = __csi_band_t */

    /* muxer ������*/
    LOTUS_CMD_SET_MUXER_OUT               = 0x1800,       /* (pbuffer = __lotus_muxer_type_t) */
    LOTUS_CMD_UNKNOWN                     = 0xFFFF,       
} __lotus_cmd_set_t;

typedef enum
{
    LOTUS_RECORD_MODE_NONE = 0,       //δ���ã�������
    LOTUS_RECORD_MODE_CAMCORDER,      //¼��ģʽ
    LOTUS_RECORD_MODE_SOUND_RECORD,   //¼��ģʽ
    LOTUS_RECORD_MODE_IMAGE,          //����ģʽ
    LOTUS_RECORD_MODE_,
} __lotus_record_mode_t;


typedef enum 
{
    LOTUS_SHOW_ALL           = 0,
    LOTUS_SHOW_HALF_UP       = 1,
    LOTUS_SHOW_HALF_DOWN     = 2,
    LOTUS_SHOW_HALF_RIGHT    = 3,
    LOTUS_SHOW_HALF_LEFT     = 4,
    LOTUS_SHOW_HALF_ERR      = 5
}__lotus_src_scn_set_t;

typedef enum __LOTUS_VID_WIN_RATIO_MODE
{
    LOTUS_VID_WIN_BESTSHOW = 0x00,        /* ��ͼƬ����ı�����������������ʾ��ͼƬ������ */
    LOTUS_VID_WIN_ORIGINAL,               /* ��ͼƬԭʼ��С�ڴ�������ʾ�������������     */
    LOTUS_VID_WIN_FULLSCN,                /* �Դ��ڵı�������ͼƬ����������ʾ�����ܻ���� */
    LOTUS_VID_WIN_CUTEDGE,                /* �ñ�ģʽ����srcFrame�����ٲõ����ºڱߣ��ñߺ���bestshowģʽ��ʾ         */
    LOTUS_VID_WIN_NOTCARE,                /* ������ͼƬ��ʾ�������Ե�ǰ���õı���         */
    LOTUS_VID_WIN_UNKNOWN
}__lotus_vid_win_ratio_mode_t;

typedef struct __LOTUS_AUDIO_INF
{
    __u32       uAbsFmt;                /* ��Ƶ�������ͣ�if(uAbsFmt == LOTUS_ABS_TYPE_NONE), ��
                                           �������������Ƶ����                             */
    __u32       uVolume;                /* ¼��������ֵ��������÷Ƿ��������defaultֵ,MIN_VOLUME_VAL~MAX_VOLUME_VAL      */
    __u32       uChn;                   /* ¼��������ģʽ��������÷Ƿ��������defaultֵ,LOTUS_AUDIO_CHN_MONO    */
    __u32       uSampWidth;             /* ¼���������λ��������÷Ƿ��������defaultֵ  */
    __u32       uAudioBps;              /* ��������Ƶ���������ʣ�������÷Ƿ��������defaultֵ����λ:bit/s    */
    __u32       uAudioSampRate;         /* ��Ƶ�����Ĳ����ʣ�������÷Ƿ��������defaultֵ  */
    __u32       uAudioSource;           /* ��ƵԴ��������÷Ƿ��������defaultֵ            */
    __s32       frame_style;            /* for 0:add head,1:raw data                        */

    //����Ĳ���ҪӦ�ó�����д����Ϣ,��Щ��Ϣ��lotus�ڲ���д�������ڲ�������ģ�顣
    __s32       sample_cnt_per_frame;   /* ÿ��audio frame������sample������ */
} __lotus_audio_inf_t;

typedef enum __LOTUS_AUDIO_SRC         /* audio source type                    */
{
    LOTUS_AUDIO_SRC_NONE=0,            /* none audio source                    */
    LOTUS_AUDIO_SRC_MIC,               /* microphone record                    */
    LOTUS_AUDIO_SRC_LINEIN,            /* line-in record                       */
    LOTUS_AUDIO_SRC_FM,                /* fm record                            */
    LOTUS_AUDIO_SRC_

} __lotus_audio_src_t;

typedef struct lotus_overlay_pic_info
{
    __u8                ID;                 //src id
    __rectsz_t          size;               //for the size of one picture
    __u8*               pOverlayPixel;           //the index of the RGB data
}__lotus_overlay_pic_info_t;

typedef struct lotus_overlay_src_init
{
    __u8                srcPicNum;                 //src id
    __lotus_overlay_pic_info_t  srcPic[OVERLAY_MAX_SRC_INGROUP];
    __u32               ppalette_yuv[16];              //the palette of yuv format
    __u8                color_num;                 //the color number of the palette
}__lotus_overlay_src_init_t;

typedef enum __LOTUS_STATUS               /* the status of ginkgo mod     */
{
    LOTUS_STAT_IDLE = 0,                  /* idle                         */
    LOTUS_STAT_STANDBY,                   /* ���ڴ���״̬������δ¼��     */
    LOTUS_STAT_RECORD,                    /* record������¼��״̬         */
    LOTUS_STAT_PAUSE,                     /* pause������¼����ͣ״̬      */
    LOTUS_STAT_UNKNOWN
} __lotus_status_t;

typedef struct LOTUS_MOTION_DETECT_PARA
{
    __u32   complexThreshold;   // 0~4096, default:512
    __u32   ratioThreahold;     // 0~1024, default:128
} __lotus_motion_dectect_para_t;

typedef struct 
{
    OFFSET   pos;               //the position of the log
    __u8     total;
    __u8     IDlist[OVERLAY_MAX_LIST_LEN];    //the index of the display of the picture
}__lotus_overlay_dis_par_t;


typedef enum __LOTUS_AUDIO_CHN         /* audio channel type                   */
{
    LOTUS_AUDIO_CHN_NONE=0,            /* none audio channel                   */
    LOTUS_AUDIO_CHN_MONO,              /* mono                                 */
    LOTUS_AUDIO_CHN_STEREO,            /* stereo                               */
    LOTUS_AUDIO_CHN_
} __lotus_audio_chn_t;


typedef enum __LOTUS_AUDIO_SAMP_WIDTH
{
    //NOTE:we only support 16bits right now!!!
    LOTUS_AUDIO_SAMP_WIDTH_0   = 0,      /* invalid audio sample width         */
    LOTUS_AUDIO_SAMP_WIDTH_8   = 8,      /* 8 bits audio sample                */
    LOTUS_AUDIO_SAMP_WIDTH_16  =16,      /* 16 bits audio sample               */
    LOTUS_AUDIO_SAMP_WIDTH_24  =24,      /* 24 bits audio sample               */
    LOTUS_AUDIO_SAMP_WIDTH_32  =32,      /* 32 bits audio sample               */
} __lotus_audio_samp_width_t;

#define ENCODER_CFG_ENC_FRAME 0X0
#define ENCODER_CFG_ENC_FIELD 0X1

#define ENCODER_CFG_ENC_MODE_MASK 0X1


typedef struct __LOTUS_VIDEO_INF
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
    __s16       maxKeyInterval;         /*keyframe Interval��ָ2���ؼ�֮֡���P֡���������             */
    __s16       encoderCfg;             /* encoder ������������*/

    //__u32       uVideoSource;         /* ��ƵԴ��������÷Ƿ��������defaultֵ                      */
} __lotus_video_inf_t;

//the information of the media file which will be input to play
typedef struct __LOTUS_MEDIA_FILE_INF
{
    //Ҫ¼�Ƶ��ļ���ʽ
    __u32       uFileFmt;                   /* Ҫ¼�Ƶ��ļ���ʽ,__herb_media_file_fmt_t                         */
    char        file_path[LOTUS_FPATH_SIZE]; /* ¼�Ƶ�ý���ļ�����ľ���·���������δ���ã��򱣴浽default·��  */

    //��Ƶ�����Ϣ
    __lotus_audio_inf_t    audio_inf;        /* ��Ƶ¼�Ƹ�ʽ��Ϣ                                         */

    //��Ƶ�����Ϣ
    __lotus_video_inf_t    video_inf;        /* ��Ƶ¼�Ƹ�ʽ��Ϣ,�ο�__video_encode_format_t             */

    //���⣬ʱ���
    char        title[LOTUS_FILE_TITLE_SIZE];
    __s32       create_time;                /* ���ֵĹ���壬��mux������Ӧ�ó�������                  */
} __lotus_media_file_inf;

typedef struct __LOTUS_CALLBACK_INFO
{
    __pCBK_t    GetCurExtData;      /* __s32 GetCurExtData(__lotus_sync_cb_para_t *psyncPara),����ֵΪ��ǰ��data����*/
    __pCBK_t    DealWithExtData;    /* void GetCurExtData(__lotus_sync_cb_para_t *psyncPara) */
    __u32       ExtDatalenPerFrame; /* ��֡ext data������󳤶� */
    __u32       ExtDatalenTotal;    /* �����ļ�ext data�����ܳ������ֵ */
    void*       pcontex;
}__lotus_sync_callback_t;

typedef struct __LOTUS_SYNC_CB_PARA
{
    void*   pbuffer;
    __s32   bufLen;
    void*   pcontex;
}__lotus_sync_cb_para_t;

typedef enum
{
    LOTUS_MUXER_TYPE_FILE = 0,            //������ļ�
    LOTUS_MUXER_TYPE_NULL = 1,                //muxer�����
    LOTUS_MUXER_TYPE_ERR  = 2,                //muxer�����
} __lotus_muxer_type_t;


#endif  


