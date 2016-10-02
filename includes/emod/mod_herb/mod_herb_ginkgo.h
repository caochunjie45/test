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

#define OVERLAY_MAX_SRC_INGROUP 20              /* 一个block中最多包含的资源图片数目*/
#define OVERLAY_MAX_LIST_LEN 40                 /* 一个block中最多能够显示的图片数目*/

#define LOTUS_MUTE_ON       (1)
#define LOTUS_MUTE_OFF      (0)


typedef enum
{
    HERB_GINGKO_RECORD_MODE_NONE = 0,       //未设置，无意义
    HERB_GINGKO_RECORD_MODE_CAMCORDER,      //录像模式
    HERB_GINGKO_RECORD_MODE_SOUND_RECORD,   //录音模式
    HERB_GINGKO_RECORD_MODE_IMAGE,          //拍照模式
    HERB_GINGKO_RECORD_MODE_,
} __herb_gingko_record_mode_t;

typedef enum __HERB_CMD_SET           /* 媒体录制中间件命令列表(IOCtrol)                              */
{
    //文件格式，音视频编码格式设置，插件手动加载操作
    HERB_GINGKO_CMD_SET_MODE                    = 0x0100,       /* IDLE下调用，设置使用模式，分拍照、录音、录像模式, 
                                                                 * aux = __herb_gingko_record_mode_t，
                                                                 */
                                                                 
    HERB_GINGKO_CMD_SET_MEDIAFILE               = 0x0101,       /* IDLE,STANDBY设置录制的媒体文件的格式和参数信息。
                                                                 * pbuffer = (__herb_media_file_inf *)，
                                                                 * 必须在HERB_GINGKO_CMD_SET_MODE之后调用      
                                                                 */
                                                                 
    HERB_GINGKO_CMD_GET_OUT_MODE_ALL            = 0x0103,       /* 得到CSI_DRV支持的输出格式列表，
                                                                 * pic_size,frame_rate,color_format,store_mode,component_seq 的列表,
                                                                 * pbuffer = __csi_mode_all_t*,
                                                                 * ret = EPDK_OK/EPDK_FAIL */
                                                                 
    HERB_GINGKO_CMD_SET_PREVIEW_OUT_MODE        = 0x0104,       /* 设置preview下的图像的大小和帧率和其他信息, 
                                                                 * pbuffer = __csi_mode_t*，
                                                                 * 从__csi_mode_all_t中选一组设置即可 
                                                                 */
                                                                 
    HERB_GINGKO_CMD_NEED_PREVIEW_PIC_FOR_RECORD = 0x0106,       /* 设置录像时是否需要把第一帧编码为图片, 
                                                                 * aux = 1需要, =0不需要. 默认不需要; 
                                                                 * pbuffer = FB*, 应用程序自己配置好 
                                                                 */

    //控制命令
    HERB_GINGKO_CMD_START                       = 0x0200,       /* 状态:idle->standby,启动中间件,
                                                                 * 该命令执行后，用户可以看到图像
                                                                 */
    HERB_GINGKO_CMD_STOP                        = 0x0201,       /* 停止中间件，卸载video_plybk模块和其他所有模块，准备退出*/
    HERB_GINGKO_CMD_START_RECORD                = 0x0202,       /* 开始录像 */
    HERB_GINGKO_CMD_PAUSE_RECORD                = 0x0203,       /* 暂停录制 */
    HERB_GINGKO_CMD_CONTI_RECORD                = 0x0204,       /* 恢复录制 */
    HERB_GINGKO_CMD_STOP_RECORD                 = 0x0205,       /* 停止录制 */
    HERB_GINGKO_CMD_TAKE_IMAGE                  = 0x0206,       /* 拍照, aux = 连拍次数，现在没有用，默认为1 , 
                                                                 * pbuffer = 目标(FB*)，内存必须已分配，而且是ARGB格式，
                                                                 * 用于存放缩略图的buffer 
                                                                 */
    HERB_GINGKO_CMD_GET_STATUS                  = 0x0207,       /* 获取媒体录制中间件当前状态 */
    HERB_GINGKO_CMD_HOLD_ON                     = 0x0208,       /* 保存当前文件，同时保持录制 */
    HERB_GINGKO_CMD_SET_HOLDING_TIME            = 0x0209,       /* 设置存放在缓冲buffer中的编码帧的时间*/
    HERB_GINGKO_CMD_SEEK_TIME                   = 0x020A,       /* 用于设置开始录制的时间, 在开始录制之前调用, 
                                                                 * aux = time(ms),aux < 0表示从当前时间之前的aux ms开始
                                                                 * 录制 aux > 0暂时不支持*/


    
    HERB_GINGKO_CMD_GET_CUR_TIME                = 0x0300,       /* 获取当前录制时间 return = current encode time(ms) */
    
    //音频相关
    HERB_GINGKO_CMD_GET_ABSTYPE                 = 0x0400,       /* 获取音频流编码格式 return = __ginkgo_audio_fmt_t */
    HERB_GINGKO_CMD_MUTE                        = 0x0401,       /* 静音开/关, 设置后所有录像文件静音
                                                                 * (pbuffer = LOTUS_MUTE_ON/LOTUS_MUTE_OFF) 
                                                                 */

    //视频相关
    HERB_GINGKO_CMD_ADD_OVERLAY_SRC             = 0x0500,       /* 添加一组block资源,(pbuffer = overlay_src_init_t*) */
    HERB_GINGKO_CMD_SHOW_OVERLAY_BLK            = 0x0501,       /* 显示一个block,(pbuffer = overlay_dis_par_t*) */
    HERB_GINGKO_CMD_SNATCH_ARGB                 = 0x0502,       /* 在录像中抓拍 ARGB 数据, pbuffer = FB*  */
    
    VDRV_GINGKO_CMD_SET_MD_PARA                 = 0x0530,       /* 设置移动检测阈值, (pbuffer = __motion_dectect_para_t*) */
    VDRV_GINGKO_CMD_MD_ONOFF                    = 0x0531,       /* 开启/关闭移动检测, aux=1表示开启, 0表示关闭*/
    VDRV_GINGKO_CMD_GET_MD_DETECT               = 0x0532,       /* 获取移动检测结果, (*pbuffer = 1)表示检测到移动, 0表示未检测到移动*/

    //显示相关
    HERB_GINGKO_CMD_SET_VID_LAYERHDL            = 0x0600,       /* 设置图层, ret = EPDK_OK */
    HERB_GINGKO_CMD_SET_VID_SHOW_MODE           = 0x0601,       /* 设置显示模式, aux = __herb_vid_window_ratio_mode_t   */
    HERB_GINGKO_CMD_SET_VID_WINDOW              = 0x0602,       /* 设置显示的窗口大小 pbuffer = __rect_t *              */
    HERB_GINGKO_CMD_SWITCH_VID_SHOW             = 0x0603,       /* 是否显示视频, aux = 1,打开视频显示，aux = 0,关闭视频显示 */
    HERB_GINGKO_CMD_SHOW_HALF                   = 0x0604,       /* 多摄像头单路输入时，选择其中一路信号显示，
                                                                 * aux = (__herb_src_scn_set_t ) */
    
    //CSI设置相关
    HERB_GINGKO_CMD_SET_COLOR_EFFECT            = 0x0700,       /* aux = __csi_color_effect_t */
    HERB_GINGKO_CMD_GET_COLOR_EFFECT            = 0x0701,       /* ret = __csi_color_effect_t，得到当前的color effect */
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

typedef enum __HERB_GINGKO_FEEDBACK_MSG             /* 定义媒体录制中间件反馈给应用程序的消息类型       */
{
    HERB_GINGKO_ERROR_NONE                  =  0,   /* 空消息                                           */
    HERB_GINGKO_ERROR_FAIL                  = -1,   /* 普通错误，不关心错误类型     */

    //herGINGKO_b feed back error type
    HERB_GINGKO_ERROR_CREATE_FILE_FAIL      = -20,  /* 创建媒体文件失败             */
    HERB_GINGKO_ERROR_FILE_FMT_UNKNOWN      = -21,  /* 无法识别当前媒体文件格式     */
    HERB_GINGKO_ERROR_FILE_NOT_SUPPORT      = -22,  /* 不支持当前媒体文件格式录制   */
    HERB_GINGKO_ERROR_NO_FILE_INPUT         = -23,  /* 未设置要录制的媒体文件       */
    HERB_GINGKO_ERROR_WRITE_FILE_FAIL       = -24,  /* 写入文件数据失败             */
    HERB_GINGKO_ERROR_DISK_FULL             = -25,  /* 磁盘满                       */
    HERB_GINGKO_ERROR_PARAM                 = -26,  /* 参数错误                     */

    HERB_GINGKO_ERROR_START_PLG_FAIL        = -40,  /* 启动插件失败                 */
    HERB_GINGKO_ERROR_AENC_ERROR            = -41,  /* 音频编码器错误               */
    HERB_GINGKO_ERROR_VENC_ERROR            = -42,  /* 视频编码器错误               */

    HERB_GINGKO_ERROR_UNKOWN                = -60,  /* 未知错误                     */
    HERB_GINGKO_ERROR_REQ_MEMORY_FAIL       = -61,  /* 申请内存失败                 */

    //herb feed back status type
    HERB_GINGKO_FEDBAK_NO_ERROR             =  1,   /* 空消息                       */

    HERB_GINGKO_FEDBAK_

} __herb_gingko_feedback_msg_t;

#define HERB_GINGKO_STAT_TEMP_BIT      0x80
typedef enum __HERB_GINGKO_STATUS               /* the status of ginkgo mod     */
{
    HERB_GINGKO_STAT_IDLE = 0,                  /* idle                         */
    HERB_GINGKO_STAT_STANDBY,                   /* 处于待命状态，但并未录制     */
    HERB_GINGKO_STAT_RECORD,                    /* record，处于录制状态         */
    HERB_GINGKO_STAT_PAUSE,                     /* pause，处于录制暂停状态      */
    HERB_GINGKO_STAT_
} __herb_gingko_status_t;


typedef enum __HERB_GINGKO_VID_WINDOW_RATIO_MODE
{
    HERB_GINGKO_VID_WINDOW_BESTSHOW = 0x00,        /* 以图片本身的比例缩放至满窗口显示，图片不变形 */
    HERB_GINGKO_VID_WINDOW_ORIGINAL,               /* 以图片原始大小在窗口内显示，不能溢出窗口     */
    HERB_GINGKO_VID_WINDOW_FULLSCN,                /* 以窗口的比例缩放图片至满窗口显示，可能会变形 */
    HERB_GINGKO_VID_WINDOW_CUTEDGE,                /* 裁边模式，在srcFrame区域再裁掉上下黑边，裁边后，以bestshow模式显示         */
    HERB_GINGKO_VID_WINDOW_NOTCARE,                /* 不关心图片显示比例，以当前设置的比例         */
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

