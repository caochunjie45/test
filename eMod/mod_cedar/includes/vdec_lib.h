/*
*********************************************************************************************************
*                                                    eMOD
*                                   the Easy Portable/Player Operation System
*                                               mod_mmp sub-system
*                                                 vdec module
*
*                                    (c) Copyright 2008-2009, kevin China
*                                                All    Rights Reserved
*
* File   : mp4declib_api.h
* Version: V1.0
* By     : mengzh
* Date   : 2008-6-28 13:57
    [cedarlib]

*********************************************************************************************************
*/
#ifndef _VDEC_LIB_H2_
#define _VDEC_LIB_H2_

//参照__vdrv_msg_type进行配置
typedef enum __VDEC_LIB_MSG_TYPE
{
    VDEC_LIB_MSG_TYPE_NONE = 0,     // = VDRV_MSG_TYPE_NONE,             // no message
    VDEC_LIB_MSG_SIZE_CHANGE,       // = VDRV_MSG_SIZE_CHANGE,               // picture frame change
    VDEC_LIB_MSG_I_DECODE,          // = VDRV_MSG_I_DECODE,                  // decode i frame
    VDEC_LIB_MSG_DATA_END,          // = VDRV_MSG_DATA_END,                  // vbs decode end
    
    VDEC_LIB_MSG_RELEASE_FRM_BUF,           // 通知外面，vdrv的显示帧队列释放了一个frame buffer.
    VDEC_LIB_MSG_FREE_VBS_BUF,              //通知外面，vdrv释放了部分vbs buffer.
    
    VDEC_LIB_MSG_PREPARE_DECODE,              //通知外面，VDRV准备好了vbs和free frame buffer，准备解码
    VDEC_LIB_MSG_DECODE_ONE_FRAME_DONE,       //通知外面，VDRV完成了一帧的解码,结果是成功还是失败不重要

    VDEC_LIB_MSG_SEND_VEREADY,              //发送ve ready的semaphore.让等待semaphore的线程pend到
    VDEC_LIB_MSG_,
} __vdec_lib_msg_type;

typedef enum VDEC_IO_CMD {
    VDEC_IDLE_CMD = 0,
    VDEC_PAUSE_CMD,
    VDEC_STEP_CMD,
    VDEC_PLAY_CMD,
    VDEC_FORWARD_CMD,
    VDEC_BACKWARD_CMD,
    VDEC_STOP_CMD,
    VDEC_GET_STATUS_CMD,    //查询dec_lib状态，MEDIA_STATUS_IDLE, 
    
    VDEC_DATAEND_CMD,
    VDEC_SC_CMD,    //scale命令
    VDEC_ROT_CMD,   //旋转命令:设置解码图像旋转的角度.aux = 0:默认为原图,1:顺时针90度,2:180度,3:270度,4:水平镜像,5:垂直镜像
    VDEC_REGCB_CMD,     //aux = callback type(VDEC_CB_TYPE_REGISTER_VE_FINISH_ISR), pbuffer = 对应的*fn
    VDEC_AUTOSC_CMD,    //自适应的做scale. aux = 1:允许自适应scale; aux=0:不允许
    
    VDEC_VBS_USE_PTS_CMD,//VDEC_DECODE_ALL_VBS_FRAME_CMD,
    VDEC_SELECT_FRAME_USE_PTS_CMD,//VDEC_OUTPUT_ALL_DISPLAY_FRAME_CMD,
    VDEC_QUERY_VBSBUFFER_USAGE_CMD, //查询vbs buffer的使用率，即当前装载的数据和总buffer的百分比，ret = 0~100, 100代表100%
    VDEC_SET_VBS_SIZE_CMD,  //设置vdeclib的vbs buffer size的大小
    VDEC_SET_FRAME_NUM_CMD, //设置vdeclib的frame的数量
    VDEC_GET_VFORMAT_CMD,   //得到video codec format, pbuffer = __video_codec_format_t*, app malloc内存;
	VDEC_GET_OFQ_VALID_SIZE_CMD,   //得到output frame queue中的有效帧的个数
    VDEC_SWITCH_DE_SCALE2_CMD,  // 在解码过程中，把资源转给DE SCALER2,主要是SRAM使用权，并保证VE CLOCK GATE打开。要保证在解码库没有解码的情况下，调用该IO接口。
    VDEC_SWITCH_DEFAULT_CMD,    //  在解码过程中，把资源收回，转给VE自己,主要是SRAM使用权，并保证VE的状态处于默认状态。默认状态下VE CLOCK GATE关闭，这是为了降低功耗
    
    VDEC_MP4_DBLK_CMD = 0x20,
    //VDEC_RMG2_CMD,
    VDEC_IO_CMD_
} __vdec_io_cmd_t;
/*
================================================================================
                            RETURN VALUE MODULE
================================================================================
*/
#ifdef VDRV_TRUE
#undef VDRV_TRUE
#endif

#ifdef VDRV_FALSE
#undef VDRV_FALSE
#endif

#ifdef VDRV_SUCCESS
#undef VDRV_SUCCESS
#endif

#ifdef VDRV_FAIL
#undef VDRV_FAIL
#endif

#ifdef VDRV_NULL
#undef VDRV_NULL
#endif

#define VDRV_TRUE                   (1)
#define VDRV_FALSE                  (0)
#define VDRV_SUCCESS                (0)
#define VDRV_FAIL                   (-1)
#define VDRV_NULL                   0

//define the result of frame decode
typedef enum __PIC_DEC_RESULT
{
    PIC_DEC_ERR_VE_BUSY         = -1,   // video engine is busy
    PIC_DEC_ERR_VFMT_ERR        = -2,   // the format of the video is not supported, fatal error

    PIC_DEC_ERR_NONE            = 0,    // decode one picture successed
    PIC_DEC_ERR_FRMNOTDEC       = 1,    // decode one picture successed
    PIC_DEC_ERR_VBS_UNDERFLOW   = 2,    // video bitstream underflow
    PIC_DEC_ERR_NO_FRAMEBUFFER  = 3,    // there is no free frame buffer for decoder
    PIC_DEC_ERR_DEC_VBSEND      = 4,    // decode all video bitstream end

    PIC_DEC_ERR_

} __pic_dec_result;

typedef struct{
    __u8 MACC_INT_VE_FINISH_FLG;
    __u8 MACC_INT_VE_ERROR_FLG;
    __u8 MACC_INT_VE_MEMREQ_FLG;
    __u8 MACC_INT_VE_DBLK_FLG;
} __ve_interupt_status_t;

typedef struct _VDEC_PORT_ VDEC_PORT;
typedef struct _VDEC_PORT_
{
    //__s16       (*vdec_port_exit)(VDEC_PORT *p);
    __s16       (*open)(VDEC_PORT *p);
    __s16       (*close)(VDEC_PORT *p);
    __s16       (*decode)(VDEC_PORT *p);
    __s16       (*IoCtrl)(VDEC_PORT *p, __u32 uCmd, __u32 uParam, void *pbuffer);   //uCmd: __vdec_io_cmd_t
    __s16       (*RequestWrite)(VDEC_PORT *pDev, __u32 uSizeRequested, __u8 **ppData0, __u32 *puSizeGot0, __u8 **ppData1, __u32 *puSizeGot1);
    __s16       (*UpdatePointer)(VDEC_PORT *pDev, struct VDecDataCtrlBlk * pVDataCtrlBlk,__u32 uDataSize);
    __s16       (*DisplayRequest)(VDEC_PORT *pDev, struct Display_Info *pDisplay_Info);
    __s16       (*DisplayRelease)(VDEC_PORT *pDev, __u8 frm_idx);
    __s32       (*SetVbsInf)(VDEC_PORT *pDev, void *pVbsInf);   //pVbsInf = __vdec_mpeg4_vbs_inf_t*
    __s32       (*ProcessVEInterupt)(VDEC_PORT *pDev, __ve_interupt_status_t  *pVEStatus);   //具体负责在VE中断到来后，读取寄存器

//    __s32       (*CbSendMsg)(__s32 nMstType, void *pMsg);   //nMstType:__vdec_lib_msg_type
//    __u32       (*CbGetVideoClkTime)(void);  //得到系统时间轴的时间，选帧时使用，返回值的单位是ms
//    __s32       (*CbRegisterVEFinishISR)(__u32 irqno, void *pArg);  //irqno:VE_FINISH的中断号, pArg暂时没用
//    __s32       (*CbUnRegisterVEFinishISR)(__u32 irqno, void *pArg);  //irqno:VE_FINISH的中断号, pArg暂时没用
//    __s32       (*CbWaitVeReady)(void); //等VEReady.因为是等semaphore,系统相关，所以设为callback.由适配类具体实现.
} VDEC_PORT;
//extern VDEC_PORT* VDEC_PORT_init(__s32 * ret);
extern VDEC_PORT* newVDEC_PORTChild(void);
extern void deleteVDEC_PORTChild(VDEC_PORT* thiz);

//需要外部实现的函数
extern int Vdeclib_DisplayFrameQueue_Lock(void);    
extern int Vdeclib_DisplayFrameQueue_Unlock(void);
extern int SysCb_SendMsg(int nMsgType, void *pMsg); //nMstType:__vdec_lib_msg_type
extern unsigned int SysCb_GetVideoClkTime(void);    //得到系统时间轴的时间，选帧时使用，返回值的单位是ms
extern int SysCb_RegisterVEFinishISR(unsigned int irqno, void *pArg);  //irqno:VE_FINISH的中断号, pArg暂时没用
extern int SysCb_UnRegisterVEFinishISR(unsigned int irqno, void *pArg);  //irqno:VE_FINISH的中断号
extern int SysCb_WaitVeReady(void); //等VEReady.因为是等semaphore,系统相关，所以设为callback.由适配类具体实现.

#endif  /* _VDEC_LIB_H2_ */

