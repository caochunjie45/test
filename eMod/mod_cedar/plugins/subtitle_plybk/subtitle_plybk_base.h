/*
********************************************************************************
*                                    eMOD
*                   the Easy Portable/Player Develop Kits
*                               mod_cedar sub-system
*                          (module name, e.g.mpeg4 decoder plug-in) module
*
*          (c) Copyright 2010-2012, Allwinner Microelectronic Co., Ltd.
*                              All Rights Reserved
*
* File   : video_plybk_base.h
* Version: V1.0
* By     : Eric_wang
* Date   : 2011-2-23
* Description:
********************************************************************************
*/
#ifndef _SUBTITLE_PLYBK_BASE_H_
#define _SUBTITLE_PLYBK_BASE_H_


typedef struct __CEDAR_SPLY __cedar_sply_t;


//实成员函数
typedef __s32 (*CedarSply_Open)(__cedar_sply_t *thiz);  //创建各种所需资源， 如semaphore, msg_queue等
typedef __s32 (*CedarSply_Close)(__cedar_sply_t *thiz);
typedef __s32 (*CedarSply_Start)(__cedar_sply_t *thiz); //启动线程
typedef __s32 (*CedarSply_Stop)(__cedar_sply_t *thiz);
typedef __s32 (*CedarSply_SetFrameRect)(__cedar_sply_t *thiz);
typedef __s32 (*CedarSply_DrawSubtitle)(__cedar_sply_t *thiz, CSubtitleDrawItem *tmpSubInf, __s32 enable);
typedef __s32 (*CedarSply_ClearSubtitle)(__cedar_sply_t *thiz, CSubtitleDrawItem *tmpSubInf, __s32 enable);

typedef __s32 (*CedarSply_QueryFrame)(__cedar_sply_t *thiz, CSubtitleDrawItem *tmpSubInf);
#define SPLY_SUB_FRM_Q_SIZE     8


typedef struct __CEDAR_SPLY
{
    __u32               mid;            /* video play back模块ID                */
    __u8                status;         /* video play back当前的播放状态, play, ff, rr, jump, pause, stop        */
    __u8                main_tsk_prio;  /* video play back主线程优先级          */
    __u8                stop_flag;      /* video play back main task stop flag  */
    __s8                sub_show_flag;  /* picture display switch flag，标记是否要显示图像          */
    __s8                frame_show_flgs;        //第一帧的标识，置1表示正在申请的帧是第一帧，置0表示正在申请的帧是第一帧之后的帧,第一帧已经申请成功.初始化为1.在第一帧申请成功后，立刻置为0


    __sub_inf           sub_inf;         //字幕信息；

//    __s8                de_layer_can_be_use;    //表示de_layer是否可以使用了，vply规定只有在DISP_CMD_LAYER_SET_PARA后才能使用该图层
//    __s32               de_layer_hdl;   /* video 图层句柄                       */
//    __s32               vid_wind_ratio; /* video 图片显示比例模式, 就是nVidShowMode, enum __CEDAR_VID_WINDOW_RATIO_MODE, CEDAR_VID_WINDOW_BESTSHOW等               */
//    __pCBK_t            CB_SetSrcScnWinPara;    //设置srcwin, scnwin的callback函数,由应用程序设置下来.
//    __rect_t            video_window;   /* 视频窗体的信息,即在屏幕上划定的用于显示的区域的位置和宽高，都是以像素为单位                       */
//    __rect_t            video_window_show_rect; //记录视频窗体用于显示的区域
//    __rect_t            src_pic_rect;   //src_win;       /* 实际视频图片在framebuf的位置                         */
//    __rect_t            src_pic_show_rect;  //记录帧buffer中的显示区域
//    __rectsz_t          src_pic_size;   //true_picture_size;   /* 实际视频图片的大小，因为解码后的framebuffer的宽高都是宏块对齐的，framebuffer的大小不一定是实际图片的大小 */
//    __rectsz_t          src_pic_show_size;  //picture_size;   /* 实际视频图片的要显示部分的大小                         */
//    
//    __u16               color_format;   //记录当前frame的color format, 当发现新帧的color format改变时，重设DE.类型为PIC_CFMT_YCBCR_420,PIXEL_YUV420
//    __s32               pic_pix_ratio;  /* 图片象素宽高比例，从视频文件解码得到,是解码图的像素宽高比. 像素本身没有宽高概念。其真正的意义是:在高度的像素个数相等的情况下, 原图的宽度的像素个数与编码后的图像的宽度的像素个数的比值，解码后显示时，最好把像素个数调整到与原图相同，否则图像变形                     */
//    __u8                horizontal_scale_ratio; //vdrv做解码时水平方向scale的倍数, what ratio this picture has been scaled down at horizon size, 0: 1/1, 1: 1/2, 2: 1/4, 3: 1/8; 0xff:无效
//    __u8                vertical_scale_ratio;   //vdrv做解码时垂直方向scale的倍数, what ratio this picture has been scaled down at vetical size, 0: 1/1, 1: 1/2, 2: 1/4, 3: 1/8; 0xff:无效

    __mp                *ldec;          /* subtitle decode 模块句柄                */
    __mp                *avsync;        /* av sync模块句柄                      */

    __hdle                pSubSktin;      /* 和视频解码插件间的通讯插槽           */
//    __krnl_event_t      *pMsgChn;       //通向cedar主控的消息通道
//    MsgQueue            *pCedarMsgChn;    //通向cedar主控的消息通道
    __pCBK_t              CB_MsgQPost;  //通向cedar主控的消息通道,用callback来post.
    __pCBK_t              CB_ShowDraw;  //画字幕
    
    //__s8                call_back_flag; /* call-back函数时否处于注册状态        */
//    __s8                layer_open_flag;/* 记录图层当前的真正状态：open/close         */
//    __s8                layer_status;   /* 视频图层当前应该的状态               */
//    __s8                pic_set_flag;   /* 图片大小设置标志,表示已拿到第一帧并setpara了,文件播放结束后清0                     */
//
    //__vply_pic_frm_q_t  pic_frm_q;      /* 图象帧队列信息                       */
    //__krnl_event_t      *pSemReleaseFrmbuf;

    //VplyExtraFrameManage    ExtraFrameMgr;  //负责额外帧的各种操作，用于文件切换的无缝模式

    //__s8                forbid_set_fb;  /* 禁止调用DE的SET_FB接口，视频播放过程中，如果截屏，在截屏过程中，不能SET_FB，但是现在已经废除截屏操作了,而且转用线程要帧模式,故废弃该变量*/
//    __s8                set_fb_flg;     //指示是否真调用DISP_CMD_VIDEO_SET_FB了，因为如果图层无效，是默认设置成功，但并没有真的set_fb.该变量就是记录是否set_fb的. 1:设置了fb, 0:没有设置，这是针对线程要帧模式设计的
//    __s8                start_req_frame_loop_flg;   // 1:已经进入要帧循环; 0:正在等待
//    __s8                scale_frame_request_flg;    // 1表示申请对当前正在显示的frame做scale.vply_task()作完scale后，会把该变量置0.
    //FB                  *scale_fb;                  // 由上层模块传入指针，直接使用
    //VirtualDE_DispFb    scale_fb;   //存储上层模块传入的信息

    //AB播放的控制,vply所能参与的ab播放正常情况下只能是视频文件的jump方式的ab播放
//    __s32               nABEnableFlag;  //0:diable, 1:enable
//    __s32               nBPointPts;     //单位:ms, B点的PTS
//    __s32               nNotifyBDoneFlag;       //已经从消息通道发送B点到达的消息, 1:已发送, 0:未发送
//
//    __bool              first_frame_flg;        //第一帧的标识，置1表示正在申请的帧是第一帧，置0表示正在申请的帧是第一帧之后的帧,第一帧已经申请成功.初始化为1.在第一帧申请成功后，立刻置为0
  
//    __s32               nFileSwitchVplyMode;    // CEDAR_FILE_SWITCH_VPLY_MODE_GAP等
//
//    __s32               nNewVideoPtsValidFlag;    //表示新设置的pts是否已经设给时间轴了,刚设置下来时,变量置1. 当设置给时间轴后,变量清0.
//    __u32               nNewVideoPts;       //当回头时,可以认为时间轴要调整,也可以认为要换pts.
//    __s32               nDtvDelayTime;       //在电视广播中,定义在cedar插件里面的设置的缓冲时间.不包括前端DMX/TSC的缓冲时间.单位ms,处理PTS回头时使用
//    __s32               nDtvAVInterval;            //在电视广播中,第一次收到的视频pts和音频pts的差值,videoPts - audioPts, 为正表示video领先, audio需要加速追video.

    CedarSply_Open              open;
    CedarSply_Close             close;
    CedarSply_Start             start;
    CedarSply_Stop              stop;
    CedarSply_SetFrameRect      SetFrameRect;    
    CedarSply_QueryFrame        QueryFrame;
    CedarSply_DrawSubtitle      DrawSubtitle; 
    CedarSply_ClearSubtitle     ClearSubtitle; 
   

  
} __cedar_sply_t;
extern __cedar_sply_t   *cedar_sply;

extern __s32 Sply_Init(__cedar_sply_t *cedar_sply, __u32 mid);
extern __s32 Sply_Exit(__cedar_sply_t *thiz);
extern __cedar_sply_t* newSply(__u32 mid);
extern void deleteSply(__cedar_sply_t *thiz);

#endif  /* _SUBTITLE_PLYBK_BASE_H_ */

