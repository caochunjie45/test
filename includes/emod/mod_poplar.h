/*
*********************************************************************************************************
*                                                    eMOD
*                                   the Easy Portable/Player Operation System
*                                               mod_mmp sub-system
*
*                                      (c) Copyright 2009-2010, kevin.z China
*                                               All Rights Reserved
*
* File   : mod_poplar.h
* Version: V1.0
* By     : kevin.z
* Date   : 2009-6-24 16:30
*********************************************************************************************************
*/
#ifndef _MOD_POPLAR_H_
#define _MOD_POPLAR_H_

#include "typedef.h"
#include "cfgs.h"


typedef enum __POPLAR_SUBTITLE_ENCODE
{
    POPLAR_CHAR_ENCODE_GB2312,    // subtitle encode type is GB2312
    POPLAR_CHAR_ENCODE_GB18030,
    POPLAR_CHAR_ENCODE_GB13000,
    POPLAR_CHAR_ENCODE_UTF8,      // subtitle encode type is UTF8

    POPLAR_CHAR_ENCODE_

} __poplar_subtitle_encode_t;

typedef enum __POPLAR_CMD_SET           /* CMMB�м�������б�(IOCtrol)                                  */
{
    POPLAR_CMD_GET_MESSAGE_CHN=0x100,   /* ��ȡ������Ϣͨ����return = __krnl_event_t*, (__poplar_feedback_msg_t)    */
    POPLAR_CMD_GET_ERROR_TYPE,          /* ��ȡý�岥�����Ĵ�������, error in __poplar_feedback_msg_t   */
    POPLAR_CMD_SET_FREQ_INFO,         /* �������ֹ㲥��Ƶ��, pbuffer = __freq_info_t, 
                                        buffer�ɵ�����malloc, return = Ƶ����, -1�����쳣,  
                                        Ƶ���Ųο�CMMB���߲���, ��B.1, Page 11,
                                        ���������Ƶ�㣬ϵͳ���з���һ����ŷ��ء�*/

    POPLAR_CMD_FREQ_SCAN,            /* Ƶ������,��ȡ��Ƶ�εĽ�Ŀ�б�, aux = Ƶ�����,  return = �Ƿ�ɹ�*/
    POPLAR_CMD_NETWORK_SCAN,            /* �������������ݵ�ǰƵ�ε�������Ϣ�������������������Ƶ��*/
    POPLAR_CMD_AUTO_SCAN,               /* äɨ����Ԥ��Ƶ��*/
    POPLAR_CMD_STOP_SCAN,               /* ��ֹ������channel scan֮�������ֹ��chanel scan ��������ֹ*/
    POPLAR_CMD_CHECK_SCAN_PROGRESS,     /* ���scan���ȣ�0:0%, 100:100%, -1:����ʧ��*/
    POPLAR_CMD_GET_FREQ_LIST,           /* ��ȡƵ����Ϣ��, pbuffer = (__freq_list_t**),  ret = EPDK_OK*/

    POPLAR_CMD_GET_PROG_LIST,           /* ��ȡ���еĽ�Ŀ��, pbuffer =  *__service_list_t��APP��buffer        */
    POPLAR_CMD_GET_FREQ_PROG_LIST,      /* ��ȡָ��Ƶ����Ѵ洢�Ľ�Ŀ��, aux = Ƶ����, pbuffer =  *__service_list_t */
    POPLAR_CMD_SET_PROG_ITEM,           /* ����Ҫ���ŵ����ֹ㲥�Ľ�Ŀ����                               */

    POPLAR_CMD_PLAY=0x0200,             /* ��������ָ�������ֹ㲥��Ŀ, aux= ItemID(service_ent_t)*/
    POPLAR_CMD_STOP,                    /* ֹͣ�������ֹ㲥��Ŀ                                         */
    POPLAR_CMD_GET_STATUS,              /* ��ȡ���ֹ㲥�м����ǰ��״̬                                 */
    POPLAR_CMD_GET_MJD_UTC,             /* ��ȡ��ǰ������ʱ��                                           */

    POPLAR_CMD_GET_ABSTYPE=0x0400,      /* ��ȡ��Ƶ������ʽ return = __poplar_audio_fmt_t               */
    POPLAR_CMD_GET_ABS_CNT,             /* ��ȡ��ǰ���ֹ㲥��Ŀ����Ƶ������                             */
    POPLAR_CMD_SET_ABS_NO,              /* ָ����ǰҪ���ŵ����ֹ㲥����Ƶ����default = NO.0             */
    POPLAR_CMD_GET_ABPS,                /* ��ȡ��Ƶ�������� return = bps                                */
    POPLAR_CMD_GET_SAMPRATE,            /* ��ȡ��Ƶ������   return = sample rate                        */
    POPLAR_CMD_SET_CHN,                 /* ������������     aux = __poplar_audio_chn_t                   */
    POPLAR_CMD_GET_CHN,                 /* ��ȡ��Ƶ��ͨ����                                             */
    //POPLAR_CMD_SEL_AUD_TRACK,           /* ��������*/

    POPLAR_CMD_GET_VBSTYPE=0x0500,      /* ��ȡ��Ƶ������ʽ                                             */
    POPLAR_CMD_GET_VBS_CNT,             /* ��ȡ��ǰ���ֹ㲥��Ŀ����Ƶ������                             */
    POPLAR_CMD_SET_VBS_NO,              /* ָ����ǰҪ���ŵ����ֹ㲥����Ƶ����default = NO.0             */
    POPLAR_CMD_GET_VBPS,                /* ��ȡ��Ƶ��������                                             */
    POPLAR_CMD_GET_VFPS,                /* ��ȡͼ��֡��     return = frame rate                         */
    POPLAR_CMD_GET_FRAMESIZE,           /* frame size       return = (width << 16) + height             */
    POPLAR_CMD_SET_VIDEO_LAYERHDL,      /* ������Ƶͼ���� pbuffer = video_layer_hdl                   */
    POPLAR_CMD_SET_VIDEO_WINDOW,        /* ������Ƶ�������� pbuffer = (__rect_t *)                      */
    POPLAR_CMD_GET_VIDEO_WINDOW,        /* ��ȡ��Ƶ�������� return = (__rect_t *)                       */
    POPLAR_CMD_SET_DISP_RATIO_MODE,     /* ������Ƶͼ����ʾ��ģʽ��aux = __poplar_vide_window_ratio_mode_t,
                                           return = ���ú�ĵ�ǰ��ʾģʽ                                    */
    POPLAR_CMD_GET_DISP_RATIO_MODE,     /* ��ȡ��Ƶͼ����ʾģʽ��return = __poplar_vide_window_ratio_mode_t */
    POPLAR_CMD_SWITCH_VIDEO_SHOW,       /* �򿪻�ر�ͼƬ��ʾ��aux = 0:�ر�ͼƬ��ʾ��aux = 1:��ͼƬ��ʾ   */

    POPLAR_CMD_UPDATE_ESG,              /* ���µ�ǰƵ���ESG��Ϣ*/
    POPLAR_CMD_GET_SERV_ESG,            /* ��ȡһ��ҵ���ESG��Ϣ, aux = servId, pbuffer = esg_serv_sched_list_t**, ret = EPDK_OK */
    POPLAR_CMD_RELEASE_ESG,             /* �ͷ�ESG��Ϣ���˳���ĿԤ��˵�ʱ���ã��ͷ��ڴ�*/
    
    POPLAR_CMD_

} __poplar_cmd_set_t;


typedef enum __POPLAR_FEEDBACK_MSG              /* ����ý�岥���м��������Ӧ�ó������Ϣ����               */
{
    POPLAR_ERROR_NOP                 =  0,      /* ����Ϣ                                                   */

    POPLAR_ERROR_UNKOWN              = -1,      /* δ֪����                                                 */

    POPLAR_ERROR_START_PLG_FAIL      =-40,      /* �������ʧ��                                             */
    POPLAR_ERROR_ADEC_ERROR,                    /* ��Ƶ�������������                                     */
    POPLAR_ERROR_VDEC_ERROR,                    /* ��Ƶ�������������                                     */
    POPLAR_ERROR_REQ_MEMORY_FAIL,               /* �����ڴ�ʧ��                                             */

    POPLAR_FEDBAK_NO_ERROR           =  1,      /* ����Ϣ                                                   */

    POPLAR_FEDBAK_

} __poplar_feedback_msg_t;


typedef enum __POPLAR_VID_WINDOW_RATIO_MODE
{
    POPLAR_VIDEO_WINDOW_NOTCARE = 0x00, /* ������ͼƬ��ʾ�������Ե�ǰ���õı���         */
    POPLAR_VIDEO_WINDOW_ORIGINAL,       /* ��ͼƬԭʼ��С�ڴ�������ʾ�������������     */
    POPLAR_VIDEO_WINDOW_BESTSHOW,       /* ��ͼƬ����ı�����������������ʾ��ͼƬ������ */
    POPLAR_VIDEO_WINDOW_FULLSCN,        /* �Դ��ڵı�������ͼƬ����������ʾ�����ܻ���� */
    POPLAR_VIDEO_WINDOW_

}__poplar_vide_window_ratio_mode_t;

#define POPLAR_TMP_STATUS_FLAG 0x80     //��ʾ״̬ת�����м�״̬
typedef enum __POPLAR_STATUS            /* the status os poplar                 */
{
    POPLAR_STAT_PLAY=0,                 /* play                                 */
    POPLAR_STAT_IDLE,                   /* stop                                 */
    POPLAR_STAT_PAUSE,                  /* pause����ʱ��֧��*/
    POPLAR_STAT_SCAN,                   /* ����ҵ���״̬*/
    POPLAR_STAT_
} __poplar_status_t;

typedef enum __POPLAR_AUDIO_FMT
{
    POPLAR_ABS_TYPE_NONE = 0,           /* û����Ƶ��                           */
    POPLAR_ABS_TYPE_UNKNOWN,            /* ����Ƶ��,���޷�ʶ��                  */

    POPLAR_ABS_TYPE_AAC_LC,

    POPLAR__ABS_TYPE_

} __poplar_audio_fmt_t;


typedef enum __POPLAR_VIDEO_FMT
{
    POPLAR_VBS_TYPE_NONE = 0,           /* û����Ƶ��                           */
    POPLAR_VBS_TYPE_UNKNOWN,            /* ����Ƶ��,���޷�ʶ��                  */

    POPLAR_VBS_TYPE_H264,

    POPLAR_VBS_TYPE_

} __poplar_video_fmt_t;


typedef enum __POPLAR_AUDIO_CHANNEL
{
    POPLAR_AUDIO_CHN_STEREO = 0,        /* ������ģʽ                           */
    POPLAR_AUDIO_CHN_LEFT,              /* ������ģʽ                           */
    POPLAR_AUDIO_CHN_RIGHT,             /* ������ģʽ                           */
    POPLAR_AUDIO_CHN_

} __poplar_audio_channel_t;


//the setting information for poplar sub-system
typedef struct __POPLAR_SETTING_INF
{
    //��Ƶ���ŵ��������
    __s8        audio_index;            /* ��Ƶ�����*/
    __s8        audio_chn_flag;         /* audio_chn�����Ƿ���Ч�ı�־          */

    __u32       audio_chn;              /* ��Ƶ����ģʽ                         */
    //��Ƶ���ŵ��������
    __s8        video_index;            /* ��Ƶ�����*/
    __s8        video_layer_hdl_flag;   /* video_layer_hdl�����Ƿ���Ч�ı�־    */
    __s8        video_window_flag;      /* video_window�����Ƿ���Ч�ı�־       */
    __s8        video_ratio_mode_flag;  /* video_show_mode�����Ƿ���Ч�ı�־    */

    __s32       video_layer_hdl;        /* ��Ƶ����ͼ����                     */
    __rect_t    video_window;           /* ��Ƶͼ�񴰿�����                     */
    __s32       video_ratio_mode;       /* ��Ƶͼ����ʾ����ģʽ,__cedar_vide_window_ratio_mode_t*/
} __poplar_setting_inf_t;


#define MAX_SERVICE_NAME_LEN            (64)

typedef enum BROADCAST_TYPE_T
{
    BC_RESERVE = 0,
    BC_TV,
    BC_RADIO,
    BC_SHORT_PROGRAM,
    BC_DOWNLOAD,
    BC_SOFTWARE_MANAGE,
    BC_ESG,
    BC_DATA_STREAM,
    
    BC_TYPE_NUM
}BC_TYPE;

typedef struct SERVICE_ENTRY
{
    __s16   ItemID;                               //* ��item��Ψһ��ʶ�ţ��ڲ����
    __u16   servID;                               //* ҵ���ʶ�ţ������н����õ�
    __u8    freqID;                               //* ҵ������Ƶ�ε�Ƶ����
    __u8    ts_start;                             //* ����֡����ʼʱ϶��
    __u8    ts_num;                               //* ����֡ռ�õ�ʱ϶����
    __u8    chanParam;                            //* �ŵ����ò�����IF202ʹ��
    __u8    MF_ID;                                //* ҵ�����ڸ���֡�ı�ʶ�ź�
    __u8    subFrmID;                             //* ҵ�����ڸ�����֡�ı�ʶ��
    char    ServiceName[MAX_SERVICE_NAME_LEN];    //* ҵ�����ƣ���"CCTV-1", "CCTV-2"��
    __u8    valid;
    BC_TYPE bcType;                               //* �㲥����

} service_ent_t;

typedef struct SERVICE_LIST 
{
    __s32           num;
    __poplar_subtitle_encode_t charType;
    service_ent_t*  pServEnt;
    
} __service_list_t;


typedef struct FREQUENCY_ENTRY
{
    __u32   freqId;             //* Ƶ����
    __u32   freq;               //* Ƶ���Ƶ��ֵ
    __s8    city[16];           //* ������
}freq_ent_t;


typedef struct FREQUENCY_LIST
{
    __s32       num;
    freq_ent_t* pFreqEnt;
    
}__freq_list_t;


typedef struct ESG_EVENT 
{
    __u16 servId;
    __u8  lang[8];
    __u8  title[64];
    __u8  date[16];
    __u8  time[16];
    __u8  free;
    __u8  isLive;
    __u8  isRepeate;

}esg_event_t;

typedef struct ESG_SERV_SCHEDULE
{
    __u32        servId;
    __u32        eventNum;
    esg_event_t* events;
    __u32        charType;
}esg_serv_sched_t;


typedef struct ESG_SCHEDULE
{
    __u32               eventNum;
    esg_event_t*        events;

    __u32               numServSched;
    esg_serv_sched_t*   servSched;
    
    __u32        charType;

}esg_serv_sched_list_t;

typedef enum
{
    POPLAR_ERR_NONE         = EPDK_OK,        //no error
    POPLAR_ERR_FAIL         = EPDK_FAIL,       //common fail, not care reason.
    POPLAR_ERR_PARA_ERR     = -2,       //some parameter error
    POPLAR_ERR_HARDWARE_ERR = -3,       //Ӳ������
    POPLAR_ERR_REQMEM_FAIL  = -7,       //request memory failed
    POPLAR_ERR_POST_FAIL    = -8,
    POPLAR_ERR_EXCEPTION    = -9,
    POPLAR_ERR_IGNORE       = 1,        //������ԣ�ֱ�ӷ��ء�����״̬�����м�̬�����������
    POPLAR_ERR_STOPSCAN     = 2,        //��ֹscan
//    FILE_PARSER_READ_END        = 1,        //read file or idx table end.�Զ�����������Ϊ��ʶ
//    FILE_PARSER_BUF_LACK        = 2,        //BUFFER is not enough to contain data.
//    FILE_PARSER_GET_NONE        = 3,        //read file success for (>0) bytes, but get nothing.
//    FILE_PARSER_ERROR_IGNORE    = 4,        //some exception happens, but can continue process.
//    FILE_PARSER_PAUSE_GET_DATA  = 5,        //pause get data from file, when in FF,RR mode, 
//    
//    FILE_PARSER_PARA_ERR        = -1,       //some parameter error
//    FILE_PARSER_OPEN_FILE_FAIL  = -2,       //open media file failed
//    FILE_PARSER_READ_FILE_FAIL  = -3,       //read media file data failed
//    FILE_PARSER_FILE_FMT_ERR    = -4,       //media file format error
//    FILE_PARSER_NO_AV           = -5,       //neither audio nor video supported in media file
//    FILE_PARSER_END_OF_MOVI     = -6,       //movi data end
//    
//    FILE_PARSER_EXCEPTION       = -8,       //some impossible case happens.
    POPLAR_ERR_

} __poplar_return_val_t;

#endif  /* _MOD_POPLAR_H_ */


