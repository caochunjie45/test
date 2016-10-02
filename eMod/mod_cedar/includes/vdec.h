/*
*********************************************************************************************************
*                                                    eMOD
*                                   the Easy Portable/Player Operation System
*                                              mod_mmp sub-system
*
*                                     (c) Copyright 2008-2009, Kevin.z China
*                                              All Rights Reserved
*
* File   : vdec.h
* Version: V1.0
* By     : Kevin.Z
*********************************************************************************************************
*/
#ifndef _VDEC_H_
#define _VDEC_H_

typedef enum __VIDEO_DECODER_COMMAND
{
    CEDAR_VDEC_CMD_START=0x00,      /* 启动解码                                 */
    CEDAR_VDEC_CMD_STOP,            /* 停止解码                                 */
    CEDAR_VDEC_CMD_PAUS,            /* 没有用到, 暂停解码, 调用者不会主动调用pause接口的,但其他接口会引发pause状态 */
    CEDAR_VDEC_CMD_CONT,            /* 没有用到, 恢复解码, 调用者不会主动调用continue接口,  */
    CEDAR_VDEC_CMD_FF,              /* 设置快进模式,pause->ff                             */
    CEDAR_VDEC_CMD_RR,              /* 设置快退模式, pause->rr                       */
    CEDAR_VDEC_CMD_CLEAR_FFRR,      /* 清除快进快退模式，将来是pause->play                         */
    CEDAR_VDEC_CMD_GET_STATUS,  //aux = 1一定要返回平稳状态;aux=0立即返回
    CEDAR_VDEC_CMD_JUMP,            /* 设置跳播模式,aux = 目的时间点,play->pause            */
    CEDAR_VDEC_CMD_CLEAR_JUMP,      /* 清除跳播状态，将来是pause->play                         */

    CEDAR_VDEC_CMD_PREPARE_FF,      //从play转到ff,rr,需要先让vdec做好准备,准备好后，会进入pause状态待命.包括jump也是进入pause状态待命. play->pause
    CEDAR_VDEC_CMD_PREPARE_RR,      //play->pause
    CEDAR_VDEC_CMD_PREPARE_CLEAR_FFRR,  //ffrr->pause
    

    CEDAR_VDEC_CMD_ENABLE_AB,   //通知vdrv准备ab播放.如果vdrv已经设置了dataend给解码库,那么返回EPDK_FAIL;
    CEDAR_VDEC_CMD_CLEAR_AB,    //清除abplay, ret = EPDK_OK;

    CEDAR_VDEC_CMD_CHKSPT=0x20,     //aux = CEDAR_VBS_TYPE_MPEG12_LAYER1, 只是表示能否支持,不代表当前已经加载了vdrv或者加载的vdrv就是当前查询的vdrv type
    CEDAR_VDEC_CMD_SET_VBSSKTIN,    /* 设置video bit stream 队列                */
    CEDAR_VDEC_CMD_GET_PICSKTOUT,   /* 获取输出的 picture data buffer 地址      */
    CEDAR_VDEC_CMD_GET_TIME,        /* 获取当前播放时间                         */
    CEDAR_VDEC_CMD_SET_AVSHDL,      /* 设置AV Sync模块句柄                      */
    CEDAR_VDEC_CMD_GET_FRMSIZE,     /* 获取帧大小，return = (x<<16)|(y<<0)      */
    CEDAR_VDEC_CMD_GET_FRMRATE,     /* 获取视频帧率                             */
    CEDAR_VDEC_CMD_GET_TAG,         /* 获取解码的现场信息, pbuffer = __cedar_tag_inf_t*, 调用者分配内存 */
    CEDAR_VDEC_CMD_SET_TAG,         /* 设置恢复解码的现场信息，在decode开始前做, pbuffer = __cedar_tag_inf_t*，调用者分配内存 */
    CEDAR_VDEC_CMD_SET_ROTATE,      /* 设置解码图像旋转的角度.aux = 0:默认为原图,1:顺时针90度,2:180度,3:270度,4:水平镜像,5:垂直镜像*/
    CEDAR_VDEC_CMD_QUERY_VBSBUFFER_USAGE,   //查询vbs buffer的使用率，即当前装载的数据和总buffer的百分比，ret = 0~100, 100代表100%
    CEDAR_VDEC_CMD_SET_DISPLAY_REQUEST_MUTEX_MODE,  //为vdrv设置displayrequest是否需要用semaphore互斥.对应中断要帧和线程要帧, aux = 0:不需要; aux = 1:需要互斥
    CEDAR_VDEC_CMD_OPEN,            //vdec开始分配资源，目前主要是open vdrv.其余的资源在MOpen()时就分配好了, aux = CEDAR_VBS_TYPE_XVID等
    CEDAR_VDEC_CMD_CLOSE,           //vdec开始释放资源，close vdrv
    CEDAR_VDEC_SET_VBSTYPE,         //不知所云,已删除.设置编码类型, aux = CEDAR_VBS_TYPE_XVID等, ret = EPDK_OK表示成功,解码器接受了
    CEDAR_VDEC_CMD_ENABLE_VIDEO_AUTO_SCALE,  //允许vdrv自动做scale,以减少带宽消耗. aux = 1:enable; aux=0:disable.默认为disable.只允许在播放前设置，播放过程中设置无效.
    CEDAR_VDEC_CMD_SET_RESERVED_MEM_SIZE,   //设置预留的内存数量,当告诉解码器可用的内存数量时,把这部分内存预留下来.
    CEDAR_VDEC_CMD_UPDATE_VDRV,     //更新加载解码驱动, aux = CEDAR_VBS_TYPE_XVID等
    CEDAR_VDEC_CMD_ENABLE_MAF,      //是否打开maf功能, aux = 1,打开, aux = 0,关闭, 默认打开.
		CEDAR_VDEC_CMD_SET_TV_MODE,			// 1:TV MODE 0:MEDIA MODE
		CEDAR_VDEC_CMD_GET_VE_STATUS, //获取解码库工作状态，是否解码成功等，返回的是一个指向指针强制转换成S32的结构体
    CEDAR_VDEC_CMD_SET_CEDARINI,    //设置cedarini, 要求在IOOpen之后设置

    CEDAR_VDEC_CMD_

} __video_decoder_command_t;


#endif /*_VDEC_H_*/
