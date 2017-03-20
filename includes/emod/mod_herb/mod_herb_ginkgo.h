/*
********************************************************************************
*                                    eMOD
*                   the Easy Portable/Player Develop Kits
*                               mod_herb sub-system
*
*          (c) Copyright 2009-2010, Allwinner Microelectronic Co., Ltd.
*                              All Rights Reserved
*
* File   : mod_herb_ginkgo.h
* Version: V1.0
* By     : Eric_wang
* Date   : 2009-12-21
********************************************************************************
*/
#ifndef _MOD_HERB_GINGKO_H_
#define _MOD_HERB_GINGKO_H_
#include "typedef.h"
#include "cfgs.h"

#define OVERLAY_MAX_SRC_INGROUP 20              /* һ��block������������ԴͼƬ��Ŀ*/
#define OVERLAY_MAX_LIST_LEN 40                 /* һ��block������ܹ���ʾ��ͼƬ��Ŀ*/

#define LOTUS_MUTE_ON       (1)
#define LOTUS_MUTE_OFF      (0)


typedef enum
{
    HERB_GINGKO_RECORD_MODE_NONE = 0,       //δ���ã�������
    HERB_GINGKO_RECORD_MODE_CAMCORDER,      //¼��ģʽ
    HERB_GINGKO_RECORD_MODE_SOUND_RECORD,   //¼��ģʽ
    HERB_GINGKO_RECORD_MODE_IMAGE,          //����ģʽ
    HERB_GINGKO_RECORD_MODE_,
} __herb_gingko_record_mode_t;

typedef enum __HERB_CMD_SET           /* ý��¼���м�������б�(IOCtrol)                              */
{
    //�ļ���ʽ������Ƶ�����ʽ���ã�����ֶ����ز���
    HERB_GINGKO_CMD_SET_MODE                    = 0x0100,       /* IDLE�µ��ã�����ʹ��ģʽ�������ա�¼����¼��ģʽ, 
                                                                 * aux = __herb_gingko_record_mode_t��
                                                                 */
                                                                 
    HERB_GINGKO_CMD_SET_MEDIAFILE               = 0x0101,       /* IDLE,STANDBY����¼�Ƶ�ý���ļ��ĸ�ʽ�Ͳ�����Ϣ��
                                                                 * pbuffer = (__herb_media_file_inf *)��
                                                                 * ������HERB_GINGKO_CMD_SET_MODE֮�����      
                                                                 */
                                                                 
    HERB_GINGKO_CMD_GET_OUT_MODE_ALL            = 0x0103,       /* �õ�CSI_DRV֧�ֵ������ʽ�б�
                                                                 * pic_size,frame_rate,color_format,store_mode,component_seq ���б�,
                                                                 * pbuffer = __csi_mode_all_t*,
                                                                 * ret = EPDK_OK/EPDK_FAIL */
                                                                 
    HERB_GINGKO_CMD_SET_PREVIEW_OUT_MODE        = 0x0104,       /* ����preview�µ�ͼ��Ĵ�С��֡�ʺ�������Ϣ, 
                                                                 * pbuffer = __csi_mode_t*��
                                                                 * ��__csi_mode_all_t��ѡһ�����ü��� 
                                                                 */
                                                                 
    HERB_GINGKO_CMD_NEED_PREVIEW_PIC_FOR_RECORD = 0x0106,       /* ����¼��ʱ�Ƿ���Ҫ�ѵ�һ֡����ΪͼƬ, 
                                                                 * aux = 1��Ҫ, =0����Ҫ. Ĭ�ϲ���Ҫ; 
                                                                 * pbuffer = FB*, Ӧ�ó����Լ����ú� 
                                                                 */

    //��������
    HERB_GINGKO_CMD_START                       = 0x0200,       /* ״̬:idle->standby,�����м��,
                                                                 * ������ִ�к��û����Կ���ͼ��
                                                                 */
    HERB_GINGKO_CMD_STOP                        = 0x0201,       /* ֹͣ�м����ж��video_plybkģ�����������ģ�飬׼���˳�*/
    HERB_GINGKO_CMD_START_RECORD                = 0x0202,       /* ��ʼ¼�� */
    HERB_GINGKO_CMD_PAUSE_RECORD                = 0x0203,       /* ��ͣ¼�� */
    HERB_GINGKO_CMD_CONTI_RECORD                = 0x0204,       /* �ָ�¼�� */
    HERB_GINGKO_CMD_STOP_RECORD                 = 0x0205,       /* ֹͣ¼�� */
    HERB_GINGKO_CMD_TAKE_IMAGE                  = 0x0206,       /* ����, aux = ���Ĵ���������û���ã�Ĭ��Ϊ1 , 
                                                                 * pbuffer = Ŀ��(FB*)���ڴ�����ѷ��䣬������ARGB��ʽ��
                                                                 * ���ڴ������ͼ��buffer 
                                                                 */
    HERB_GINGKO_CMD_GET_STATUS                  = 0x0207,       /* ��ȡý��¼���м����ǰ״̬ */
    HERB_GINGKO_CMD_HOLD_ON                     = 0x0208,       /* ���浱ǰ�ļ���ͬʱ����¼�� */
    HERB_GINGKO_CMD_SET_HOLDING_TIME            = 0x0209,       /* ���ô���ڻ���buffer�еı���֡��ʱ��*/
    HERB_GINGKO_CMD_SEEK_TIME                   = 0x020A,       /* �������ÿ�ʼ¼�Ƶ�ʱ��, �ڿ�ʼ¼��֮ǰ����, 
                                                                 * aux = time(ms),aux < 0��ʾ�ӵ�ǰʱ��֮ǰ��aux ms��ʼ
                                                                 * ¼�� aux > 0��ʱ��֧��*/


    
    HERB_GINGKO_CMD_GET_CUR_TIME                = 0x0300,       /* ��ȡ��ǰ¼��ʱ�� return = current encode time(ms) */
    
    //��Ƶ���
    HERB_GINGKO_CMD_GET_ABSTYPE                 = 0x0400,       /* ��ȡ��Ƶ�������ʽ return = __ginkgo_audio_fmt_t */
    HERB_GINGKO_CMD_MUTE                        = 0x0401,       /* ������/��, ���ú�����¼���ļ�����
                                                                 * (pbuffer = LOTUS_MUTE_ON/LOTUS_MUTE_OFF) 
                                                                 */

    //��Ƶ���
    HERB_GINGKO_CMD_ADD_OVERLAY_SRC             = 0x0500,       /* ���һ��block��Դ,(pbuffer = overlay_src_init_t*) */
    HERB_GINGKO_CMD_SHOW_OVERLAY_BLK            = 0x0501,       /* ��ʾһ��block,(pbuffer = overlay_dis_par_t*) */
    HERB_GINGKO_CMD_SNATCH_ARGB                 = 0x0502,       /* ��¼����ץ�� ARGB ����, pbuffer = FB*  */
    
    VDRV_GINGKO_CMD_SET_MD_PARA                 = 0x0530,       /* �����ƶ������ֵ, (pbuffer = __motion_dectect_para_t*) */
    VDRV_GINGKO_CMD_MD_ONOFF                    = 0x0531,       /* ����/�ر��ƶ����, aux=1��ʾ����, 0��ʾ�ر�*/
    VDRV_GINGKO_CMD_GET_MD_DETECT               = 0x0532,       /* ��ȡ�ƶ������, (*pbuffer = 1)��ʾ��⵽�ƶ�, 0��ʾδ��⵽�ƶ�*/

    //��ʾ���
    HERB_GINGKO_CMD_SET_VID_LAYERHDL            = 0x0600,       /* ����ͼ��, ret = EPDK_OK */
    HERB_GINGKO_CMD_SET_VID_SHOW_MODE           = 0x0601,       /* ������ʾģʽ, aux = __herb_vid_window_ratio_mode_t   */
    HERB_GINGKO_CMD_SET_VID_WINDOW              = 0x0602,       /* ������ʾ�Ĵ��ڴ�С pbuffer = __rect_t *              */
    HERB_GINGKO_CMD_SWITCH_VID_SHOW             = 0x0603,       /* �Ƿ���ʾ��Ƶ, aux = 1,����Ƶ��ʾ��aux = 0,�ر���Ƶ��ʾ */
    HERB_GINGKO_CMD_SHOW_HALF                   = 0x0604,       /* ������ͷ��·����ʱ��ѡ������һ·�ź���ʾ��
                                                                 * aux = (__herb_src_scn_set_t ) */
    
    //CSI�������
    HERB_GINGKO_CMD_SET_COLOR_EFFECT            = 0x0700,       /* aux = __csi_color_effect_t */
    HERB_GINGKO_CMD_GET_COLOR_EFFECT            = 0x0701,       /* ret = __csi_color_effect_t���õ���ǰ��color effect */
    HERB_GINGKO_CMD_SET_AWB                     = 0x0702,       /* aux = awb, aux = __csi_awb_t */
    HERB_GINGKO_CMD_GET_AWB                     = 0x0703,       /* ret = awb, ret = __csi_awb_t */
    HERB_GINGKO_CMD_SET_AE                      = 0x0704,       /* aux = ae, aux = __csi_ae_t */
    HERB_GINGKO_CMD_GET_AE                      = 0x0705,       /* ret = ae, ret = __csi_ae_t */
    HERB_GINGKO_CMD_SET_BRIGHT                  = 0x0706,       /* aux = bright, aux = __csi_bright_t */
    HERB_GINGKO_CMD_GET_BRIGHT                  = 0x0707,       /* ret = bright, ret = __csi_bright_t */
    HERB_GINGKO_CMD_SET_CONTRAST                = 0x0708,       /* aux = contrast, aux = __csi_contrast_t */
    HERB_GINGKO_CMD_GET_CONTRAST                = 0x0709,       /* ret = contrast, ret = __csi_contrast_t */
    HERB_GINGKO_CMD_SET_BAND                    = 0x070A,       /* aux = band, aux = __csi_band_t */
    HERB_GINGKO_CMD_GET_BAND                    = 0x070B,       /* ret = band, ret = __csi_band_t */

    HERB_GINGKO_CMD_,
} __herb_gingko_cmd_set_t;

typedef enum __HERB_GINGKO_FEEDBACK_MSG             /* ����ý��¼���м��������Ӧ�ó������Ϣ����       */
{
    HERB_GINGKO_ERROR_NONE                  =  0,   /* ����Ϣ                                           */
    HERB_GINGKO_ERROR_FAIL                  = -1,   /* ��ͨ���󣬲����Ĵ�������     */

    //herGINGKO_b feed back error type
    HERB_GINGKO_ERROR_CREATE_FILE_FAIL      = -20,  /* ����ý���ļ�ʧ��             */
    HERB_GINGKO_ERROR_FILE_FMT_UNKNOWN      = -21,  /* �޷�ʶ��ǰý���ļ���ʽ     */
    HERB_GINGKO_ERROR_FILE_NOT_SUPPORT      = -22,  /* ��֧�ֵ�ǰý���ļ���ʽ¼��   */
    HERB_GINGKO_ERROR_NO_FILE_INPUT         = -23,  /* δ����Ҫ¼�Ƶ�ý���ļ�       */
    HERB_GINGKO_ERROR_WRITE_FILE_FAIL       = -24,  /* д���ļ�����ʧ��             */
    HERB_GINGKO_ERROR_DISK_FULL             = -25,  /* ������                       */
    HERB_GINGKO_ERROR_PARAM                 = -26,  /* ��������                     */

    HERB_GINGKO_ERROR_START_PLG_FAIL        = -40,  /* �������ʧ��                 */
    HERB_GINGKO_ERROR_AENC_ERROR            = -41,  /* ��Ƶ����������               */
    HERB_GINGKO_ERROR_VENC_ERROR            = -42,  /* ��Ƶ����������               */

    HERB_GINGKO_ERROR_UNKOWN                = -60,  /* δ֪����                     */
    HERB_GINGKO_ERROR_REQ_MEMORY_FAIL       = -61,  /* �����ڴ�ʧ��                 */

    //herb feed back status type
    HERB_GINGKO_FEDBAK_NO_ERROR             =  1,   /* ����Ϣ                       */

    HERB_GINGKO_FEDBAK_

} __herb_gingko_feedback_msg_t;

#define HERB_GINGKO_STAT_TEMP_BIT      0x80
typedef enum __HERB_GINGKO_STATUS               /* the status of ginkgo mod     */
{
    HERB_GINGKO_STAT_IDLE = 0,                  /* idle                         */
    HERB_GINGKO_STAT_STANDBY,                   /* ���ڴ���״̬������δ¼��     */
    HERB_GINGKO_STAT_RECORD,                    /* record������¼��״̬         */
    HERB_GINGKO_STAT_PAUSE,                     /* pause������¼����ͣ״̬      */
    HERB_GINGKO_STAT_
} __herb_gingko_status_t;


typedef enum __HERB_GINGKO_VID_WINDOW_RATIO_MODE
{
    HERB_GINGKO_VID_WINDOW_BESTSHOW = 0x00,        /* ��ͼƬ����ı�����������������ʾ��ͼƬ������ */
    HERB_GINGKO_VID_WINDOW_ORIGINAL,               /* ��ͼƬԭʼ��С�ڴ�������ʾ�������������     */
    HERB_GINGKO_VID_WINDOW_FULLSCN,                /* �Դ��ڵı�������ͼƬ����������ʾ�����ܻ���� */
    HERB_GINGKO_VID_WINDOW_CUTEDGE,                /* �ñ�ģʽ����srcFrame�����ٲõ����ºڱߣ��ñߺ���bestshowģʽ��ʾ         */
    HERB_GINGKO_VID_WINDOW_NOTCARE,                /* ������ͼƬ��ʾ�������Ե�ǰ���õı���         */
    HERB_GINGKO_VID_WINDOW_
}__herb_gingko_vid_window_ratio_mode_t;

typedef struct MOTION_DETECT_PARA
{
    __u32   complexThreshold;   // 0~4096, default:512
    __u32   ratioThreahold;     // 0~1024, default:128
} __motion_dectect_para_t;

typedef enum 
{
    HERB_SHOW_ALL           = 0,
    HERB_SHOW_HALF_UP       = 1,
    HERB_SHOW_HALF_DOWN     = 2,
    HERB_SHOW_HALF_RIGHT    = 3,
    HERB_SHOW_HALF_LEFT     = 4,
    HERB_SHOW_HALF_ERR      = 5
}__herb_src_scn_set_t;

typedef struct overlay_pic_info
{
    __u8                ID;                 //src id
    __rectsz_t          size;               //for the size of one picture
    __u8*               pOverlayPixel;           //the index of the RGB data
}overlay_pic_info_t;

typedef struct overlay_src_init
{
    __u8                srcPicNum;                 //src id
    overlay_pic_info_t  srcPic[OVERLAY_MAX_SRC_INGROUP];
    __u32               ppalette_yuv[16];              //the palette of yuv format
    __u8                color_num;                 //the color number of the palette
}overlay_src_init_t;


typedef struct dis_par
{
    OFFSET   pos;               //the position of the log
    __u8     total;
    __u8     IDlist[OVERLAY_MAX_LIST_LEN];    //the index of the display of the picture
}overlay_dis_par_t;
#endif  /* _MOD_HERB_GINGKO_H_ */

