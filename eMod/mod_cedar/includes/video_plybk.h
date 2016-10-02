/*
*********************************************************************************************************
*                                                    eMOD
*                                   the Easy Portable/Player Operation System
*                                              mod_mmp sub-system
*
*                                     (c) Copyright 2008-2009, Kevin.Z China
*                                              All Rights Reserved
*
* File   : video_plybk.h
* Version: V1.0
* By     : Kevin.Z
    对外API
*********************************************************************************************************
*/
#ifndef _VIDEO_PLYBK_H_
#define _VIDEO_PLYBK_H_

//#include "vdec_2_vply.h"
//0表示必须使用中断要帧机制, 1表示优先使用线程主动要帧，如果DEDRV不支持，再使用
//中断要帧机制，参考__VPLY_WORK_MODE.目前默认使用中断要帧机制，因为vply_task()和
//video decode task()在操作显示帧队列时要互斥，而原先的vdrv_lib是根据中断要帧设计的,
//没有做线程间互斥的处理.如果要正式启动线程主动要帧，必须修改各vdrv_lib的decode()
//函数，整个decode()函数是不能全部用sema互斥的。因为包含了等VEFINISH中断的过程，
//对于1080p解码，耗时20多ms,这对于vply_task()无法接受。现在的做法是在vply线程要帧时关线程调度以解决该问题

#define VPLY_WORK_MODE_VALUE    (1) 
enum __VPLY_WORK_MODE
{
    VPLY_WORK_MODE_ISR = 0, //  中断要帧设帧模式
    VPLY_WORK_MODE_TASK_ACTIVE = 1, //task主动要帧设帧模式
};
enum
{
    CEDAR_VPLY_CMD_START=0x00,          /* start                                */
    CEDAR_VPLY_CMD_STOP,                /* stop                                 */
    CEDAR_VPLY_CMD_PAUSE,               /* pause                                */
    CEDAR_VPLY_CMD_CONTI,               /* continue playing                     */
    CEDAR_VPLY_CMD_FF,                  /* fast forward                         */
    CEDAR_VPLY_CMD_RR,                  /* fast reverse                         */
    CEDAR_VPLY_CMD_CLEAR_FFRR,          /* clear fast forward/reverse           */
    CEDAR_VPLY_CMD_JUMP,                /* jump                         */
    CEDAR_VPLY_CMD_CLEAR_JUMP,          /* clear jump           */
    CEDAR_VPLY_CMD_ENABLE_AB,           //通知aply模块，ab播放开始，正常情况下，一定是音频文件的jump方式的ab播放,aply只有在play状态下才会检测b点是否已到
    CEDAR_VPLY_CMD_CLEAR_AB,            //通知aply模块，ab播放结束，不用再检测B点了
    CEDAR_VPLY_CMD_SET_AB_B,                //设置AB播放时的B点的PTS, aux = pts, unit:ms
    CEDAR_VPLY_CMD_CLEAR_NOTIFYBDONEFLAG,   //AB播放时，把通知B点到达的标记位清0.

    CEDAR_VPLY_CMD_OPEN = 0x20,     //分配资源
    CEDAR_VPLY_CMD_CLOSE,           //释放资源
    CEDAR_VPLY_CMD_SET_AVSHDL,     /* set avsync module handle             */
    CEDAR_VPLY_CMD_SET_PICSKTIN,        /* picture buf from video decoder       */
    CEDAR_VPLY_CMD_SET_VIDLAYERHDL,     /* set video layer handle               */
    CEDAR_VPLY_CMD_SET_VIDWINDOW,       /* set video layer window information,还有新模式   */
    CEDAR_VPLY_CMD_SWITCH_PIC_SHOW,     /* switch picture show                  */
    CEDAR_VPLY_CMD_INVALID_VIDEOLAYER,       /* 禁止使用DE LAYER HANDLE, aux = 1, 禁止使用; aux = 0,恢复使用*/
    CEDAR_VPLY_CMD_GET_START_REQ_FRAME_LOOP_FLG,       /* 查询是否进入要帧循环, ret = 1表示已经进入 */
    CEDAR_VPLY_CMD_SET_WORK_MODE,           /* aux = enum __VPLY_WORK_MODE, 在mopen()以后，第一个做的操作必须是这个IO接口，设置工作模式! 然后再mclose()之前不允许再改 */
    //CEDAR_VPLY_CMD_SET_MSG_QUEUE,           //设置通向cedar主控模块的反馈消息通道,ab播放时，检测到B点已到，通过消息通道通知CEDAR主控模块去处理, pbuffer = __krnl_event_t  *CmdQ
    CEDAR_VPLY_CMD_SET_CB_MSGQPOST,           //设置通向cedar主控模块的反馈消息通道,ab播放时，检测到B点已到，通过消息通道通知CEDAR主控模块去处理, pbuffer = __krnl_event_t  *CmdQ
    CEDAR_VPLY_CMD_SET_FILE_SWITCH_VPLY_MODE,   //aux = CEDAR_FILE_SWITCH_VPLY_MODE_GAP等
    CEDAR_VPLY_CMD_SET_CB_SETSRCSCNWINPARA, //设置callback函数,用于决定srcwin和scnwin的显示区域. pbuffer = __pCBK_t.
    CEDAR_VPLY_CMD_CHECK_PIC_FRAME_REDUNDANT, //查询vply的帧队列是否有多余的帧,EPDK_TRUE:表示vply帧队列里帧的数量大于dedrv所要求的帧数, EPDK_FALSE:小于等于dedrv所要求的帧数(dedrv可能要求最少2帧)
    CEDAR_VPLY_CMD_SET_DISPLAY_MODE_OUT,      //设置视频播放模式 2D/3D  
    CEDAR_VPLY_CMD_SET_ROTATE,                  //设置旋转角度,aux = pCedar->rotate_mode, 
    CEDAR_VPLY_CMD_SET_DEINTERLACE_LEVEL,       //因为目前video_start接口清空了变量.在接口中直接设置也可以,但不能保证video_start已经调用过.记录deinterlace的等级. aux = 2:leve2的deinterlace, 解码驱动会开始做处理了,de驱动会选择maf模式,带宽占用最大;1:level1的deinterlace,de驱动应该会选择maf_bob模式,带宽占用稍大一些. 0:关闭.默认关闭.
    //CEDAR_VPLY_CMD_CAPTURE_PIC = 0x30,  /* 截屏, pbuffer = __cedar_pic_info_t     */
    //CEDAR_VPLY_CMD_ASK_PIC_BUFSIZE,     /* 向CEDAR查询需要多少buffer以便APP分配，pbuffer = __cedar_pic_info_t, ret = size, -1代表失败*/
    CEDAR_VPLY_CMD_SCALE_FRAME = 0x40,           /* 截图, 同步操作. 把当前正在显示的frame做一次scale, pbuffer = FB*, FB已经分配好内存，参数全部配好，可以直接用*/
    CEDAR_VPLY_CMD_DISABLE_ALPHA_BLENDING,
    CEDAR_VPLY_CMD_ENABLE_ALPHA_BLENDING,
    CEDAR_VPLY_CMD_CONFIG_VIDEO_LYR_CK_ENABLE,  // 配置设置给maple的视频图层的ck_enable参数的值, aux = 1:enable; aux = 0:disable, 
    CEDAR_VPLY_CMD_CHECK_PICTURE_OUT,      //ret = EPDK_TRUE,显示.当播放视频时，查询第一帧图像是否已经显示出来;如果是音频,则返回值始终为EPDK_FALSE.


    CEDAR_VPLY_CMD_SET_NEW_VIDEO_PTS = 0x50,           //aux = ms.在电视广播中,在pts回头后,vply就要负责设置vclock的新的时间,这个时间是经过前面的模块处理过的时间. vclock是作为stream clock看待的.vply会在标记那个时间戳的帧到来后,才重设时间轴.其实相当于平滑的更换时间轴了.
    CEDAR_VPLY_CMD_SET_DTV_DELAY_TIME,                  //在电视广播中,定义在cedar插件里面的设置的缓冲时间.不包括前端DMX/TSC的缓冲时间.单位ms,处理PTS回头时使用
    CEDAR_VPLY_CMD_SET_AV_INTERVAL,                 //在电视广播中,第一次收到的视频pts和音频pts的差值,videoPts - audioPts, 为正表示video领先, audio需要加速追video.
	
    CEDAR_VPLY_CMD_
};
//extern __cedar_vply_t   *cedar_vply;

typedef enum tag_VplyRetVal
{
    VPLY_OK             = EPDK_OK,
    VPLY_FAIL           = EPDK_FAIL,
    VPLY_ERR_TASK_STOP  = 1,    //线程准备停止
    VPLY_ERR_,
} VplyRetVal;

extern __s32   vply_MInit(void);
extern __s32   vply_MExit(void);
extern __mp    *vply_MOpen(__u32 mid, __u32 mode);
extern __s32   vply_MClose(__mp *mp);
extern __u32 vply_MRead(void *pdata, __u32 size, __u32 n, __mp *mp);
extern __u32 vply_MWrite(const void *pdata, __u32 size, __u32 n, __mp *mp);
extern __s32   vply_MIoctrl(__mp *mp, __u32 cmd, __s32 aux, void *pbuffer);

#endif /*_VIDEO_PLYBK_H_*/

