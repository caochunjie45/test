/*
*********************************************************************************************************
*                                                    eMOD
*                                   the Easy Portable/Player Operation System
*                                              mod_mmp sub-system
*
*                                    (c) Copyright 2008-2009, Kevin.Z China
*                                              All Rights Reserved
*
* File   : dev_vdec.h
* Version: V1.0
* By     : Kevin.Z
*********************************************************************************************************
*/
#ifndef _DRV_VDEC_H_
#define _DRV_VDEC_H_

typedef enum __VIDEO_DECODE_DRV_COMMAND
{                                           // command list for video decode driver
    DRV_VDEC_CMD_START=0x00,                // start video decoder，必须在DRV_VDEC_CMD_SET_VBSINF设置完参数之后才能调用本IO接口，因为要根据参数设置硬件寄存器，分配buffer等。
    DRV_VDEC_CMD_STOP,                      // stop video decoder
    DRV_VDEC_CMD_PAUSE,                     // 没有用到, pause video decoder, 
    DRV_VDEC_CMD_CONTI,                     // 没有用到, continue video decoder, 
    DRV_VDEC_CMD_FF,                        // fast forward,                                    pause->ff, 
    DRV_VDEC_CMD_RR,                        // fast reverse,                                    pause->rr
    DRV_VDEC_CMD_CLEAR_FFRR,                // clear fast forward/reverse,                      pause->play
    DRV_VDEC_CMD_GET_STATUS,                // get status of video decode driver, aux = 1表示等到vdrv的状态为稳定状态才能返回, aux = 0 表示立即返回
    DRV_VDEC_CMD_JUMP,                      // 跳播, aux = 1: jump forward; aux=0:jump backward,play->pause
    DRV_VDEC_CMD_CLEAR_JUMP,                // 清除跳播状态,                                    pause->play

    DRV_VDEC_CMD_PREPARE_FF,                //play->pause, 所谓准备,就是清vbs buffer,清的方法是设置vdeclib新的状态，然后调一次decode()接口., ret = 1表示vdrv拒绝接受状态转换,因为已经设置了dataend了.
    DRV_VDEC_CMD_PREPARE_RR,                //play->pause
    DRV_VDEC_CMD_PREPARE_CLEAR_FFRR,        //ffrr->pause

    DRV_VDEC_CMD_OPEN,                      //分配资源， MOpen()只是负责最初始的malloc,资源并不分配.最大的作用为让vdrv模块的IO接口可以调用.
    DRV_VDEC_CMD_CLOSE,                     //释放资源
    DRV_VDEC_CMD_SET_CEDARINI,

    DRV_VDEC_CMD_SET_DATAINPUT=0x20,        // preview也使用.   set data input
    DRV_VDEC_CMD_QUERY_VBSBUFFER,           // preview也使用.   query free buffer
    DRV_VDEC_CMD_QUERY_FRAME,               // preview也使用.   query frame to show
    DRV_VDEC_CMD_RELEASE_FRAME,             // preview也使用.   release frame showing last time
    DRV_VDEC_CMD_DATA_END,                  // data end
    DRV_VDEC_CMD_QUERY_VBSBUFFER_USAGE,     //查询vbs buffer的使用率，即当前装载的数据和总buffer的百分比，ret = 0~100, 100代表100%
    DRV_VDEC_CMD_SET_DISPLAY_REQUEST_MUTEX_MODE, //参考CEDAR_VDEC_CMD_SET_DISPLAY_REQUEST_MUTEX_MODE, aux = 0:中断要帧,不需要互斥; aux = 1:线程要帧,需要互斥
    DRV_VDEC_CMD_ENABLE_AB,                 //通知vdrv 准备abplay, ret = EPDK_OK: vdrv接受; EPDK_FAIL:vdrv因为已经设置data_end给了解码库,无法状态转换了,故拒绝
    DRV_VDEC_CMD_CLEAR_AB,                  //清除abplay, ret = EPDK_OK;

    DRV_VDEC_CMD_SET_CB_WAITVBS=0x40,       // set call back function, wait video bitstream data
    DRV_VDEC_CMD_SET_CB_SENDMSG,            // set call back function, send message frame
    DRV_VDEC_CMD_SET_CB_FREEVBSBUF,         // set call back function, free video bitstream buffer
    DRV_VDEC_CMD_SET_VBSINF,                // preview也使用.   set video bitstream information for decode, 必须放在DRV_VDEC_CMD_START之前。pbuffer = __vdec_mpeg4_vbs_inf_t*
    DRV_VDEC_CMD_SET_AVSHDL,
    DRV_VDEC_CMD_GET_TAG,                   // 得到解码现场信息, pbuffer = __cedar_tag_inf_t*, 调用者分配内存
    DRV_VDEC_CMD_SET_TAG,                   // 设置解码现场信息, pbuffer = __cedar_tag_inf_t*, 调用者分配内存
    DRV_VDEC_CMD_SET_ROTATE,                // 设置解码图像旋转的角度.aux = 0:默认为原图,1:顺时针90度,2:180度,3:270度,4:水平镜像,5:垂直镜像
    DRV_VDEC_CMD_ENABLE_VIDEO_AUTO_SCALE,   //允许vdrv自动做scale,以减少带宽消耗. aux = 1:enable; aux=0:disable.默认为disable.只允许在播放前设置，播放过程中设置无效.
    DRV_VDEC_CMD_GET_FRMSIZE,               //CEDAR_VDEC_CMD_GET_FRMSIZE, 获取真实图像大小，return = (x<<16)|(y<<0)
    DRV_VDEC_CMD_SET_RESERVED_MEM_SIZE,     //设置预留的内存数量,当告诉解码器可用的内存数量时,把这部分内存预留下来.
    DRV_VDEC_CMD_CHKSPT,                    //检测是否支持编码格式, aux = CEDAR_VBS_TYPE_XVID, 默认在模块IO_Open()之前调用
    DRV_VDEC_CMD_ENABLE_MAF,                //aux = 1:支持maf
		DRV_VDEC_CMD_GET_VE_STATUS,						//获取解码库状态是否已经解码成功，返回的是一个结构体				
    //为缩略图增加的专用接口, 直通解码库.
    DRV_VDEC_CMD_OPEN_VDECLIB = 0x50,           //就是open_drv,加解码库的open,包含在DRV_VDEC_CMD_START中.
    DRV_VDEC_CMD_CLOSE_VDECLIB,
    DRV_VDEC_CMD_SET_VDECLIB_PREVIEW_MODE,      //置解码库的状态为preview.
    //DRV_VDEC_CMD_SET_VDECLIB_PLAY,            //置解码库的状态为PLAY.包含在DRV_VDEC_CMD_START中.不需要了
    DRV_VDEC_CMD_SET_VDECLIB_STOP,              //置解码库的状态为STOP.包含在DRV_VDEC_CMD_STOP中.
    DRV_VDEC_CMD_RESET_VDECLIB,                 //reset vdeclib
    DRV_VDEC_CMD_SET_VDECLIB_DATA_END,          //通知解码库dataend
    DRV_VDEC_CMD_DECODE,                        //调用一次解码库的decode(). ret = VdrvReturnValue,需要从__vdrv_pic_dec_result转到VdrvReturnValue.现在是一一对应
		DRV_VDEC_CMD_SET_TV_MODE,									//1: TV mode, 0:media mode
    DRV_VDEC_CMD_

} __video_decode_drv_command_t;

//定义视频解码驱动模块的返回值
typedef enum tag_VdrvReturnValue
{
    VDRV_OK             = EPDK_OK,        //no error
    VDRV_FAIL           = EPDK_FAIL,
    VDRV_ERR_VBS_FULL   = 6,   //vbs buffer已经满了,没有空闲的buffer.

    //从__vdrv_pic_dec_result(解码库适配类定义的返回值)转来的,值保证一样, VDRV_PIC_DEC_ERR_NONE
    VDRV_DEC_ERR_VE_BUSY         = -1,   // video engine is busy
    VDRV_DEC_ERR_VFMT_ERR        = -2,   // the format of the video is not supported, fatal error
    VDRV_DEC_ERR_COMMON          = -3,  
    VDRV_DEC_ERR_NONE            = EPDK_OK,    // decode one picture successed
    VDRV_DEC_ERR_FRMNOTDEC       = 1,    // decode one picture successed
    VDRV_DEC_ERR_VBS_UNDERFLOW   = 2,    // video bitstream underflow
    VDRV_DEC_ERR_NO_FRAMEBUFFER  = 3,    // there is no free frame buffer for decoder
    VDRV_DEC_ERR_DEC_VBSEND      = 4,    // decode all video bitstream end
    VDRV_DEC_I_FRAME             = 5,   // VC1 driver will return this decode result, 一般流格式的文件如mpg,vob,ts等在快进快退时, psr需要这个消息
    
    VDRV_ERR_
} VdrvReturnValue;

#endif /* _DRV_VDEC_H_ */

