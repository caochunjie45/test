/*
*********************************************************************************************************
*                                                    eMOD
*                                   the Easy Portable/Player Operation System
*                                              mod_mmp sub-system
*
*                                    (c) Copyright 2008-2009, Kevin.Z China
*                                              All Rights Reserved
*
* File   : adec.h
* Version: V1.0
* By     : Kevin.Z
*********************************************************************************************************
*/
#ifndef _ADEC_H_
#define _ADEC_H_

typedef enum __AUDIO_DECODER_COMMAND
{
    CEDAR_ADEC_CMD_START=0x00,          /* 启动解码                             */
    CEDAR_ADEC_CMD_STOP,                /* 停止解码                             */
    CEDAR_ADEC_CMD_PAUS,                /* 停止解码                             */
    CEDAR_ADEC_CMD_CONT,                /* 停止解码                             */
    CEDAR_ADEC_CMD_FF,                  /* 设置快进模式,        pause->ff                         */
    CEDAR_ADEC_CMD_RR,                  /* 设置快退模式,        pause->rr                         */
    CEDAR_ADEC_CMD_CLEAR_FFRR,          /* 清除快进快退模式,    pause->play                     */
    CEDAR_ADEC_CMD_GET_STATUS,           /* 获取adec当前状态,目前是立即返回              */
    CEDAR_ADEC_CMD_JUMP,                /* 跳播模式, aux = des_tm,ms; pbuffer = 1代表要求精确跳播, play->pause                         */
    CEDAR_ADEC_CMD_CLEAR_JUMP,          /* 清除跳播状态,        pause->play                     */
    
    CEDAR_ADEC_CMD_PREPARE_FF,      //快进前的准备, PLAY->pause, 视频文件的adec和音频文件的adec处理方式不同
    CEDAR_ADEC_CMD_PREPARE_RR,      // play->pause
    CEDAR_ADEC_CMD_PREPARE_CLEAR_FFRR,  //从快进到正常播放的准备, FFRR->pause,

    CEDAR_ADEC_CMD_SET_ABSSKTIN=0x20,   /* 设置audio bit stream 队列            */
    CEDAR_ADEC_CMD_GET_PCMSKTOUT,       /* 获取输出的pcm double buffer 地址     */
    CEDAR_ADEC_CMD_GET_RDRTYPE,
    CEDAR_ADEC_CMD_CHKSPT,              //aux = CEDAR_ABS_TYPE_MPEG12_LAYER1, pbuffer = nRawDataFlag,只是表示能否支持,不代表当前已经加载了adrv或者加载的adrv就是当前查询的adrv
    CEDAR_ADEC_CMD_SET_SPEED,           /* 设置播放速度,speed=0:PLAY,<0:RR,>0:FF*/
    CEDAR_ADEC_CMD_SET_AVSHDL,
    CEDAR_ADEC_CMD_GET_ADRVHDL,         /* get audio driver handle              */
    CEDAR_ADEC_CMD_SET_WITH_VIDEO,      /* PLAY WITH VIDEO,如果是视频psr,就设置为1, 不管文件是否封了视频数据                      */
    CEDAR_ADEC_CMD_IS_RAWDATA,          /* 检查本模块是否是RAW_DATA decode的模块, EPDK_TRUE表示是RAW_data, EPDK_FALSE表示不是RAW_data*/
    CEDAR_ADEC_CMD_QUERY_ABSBUFFER_USAGE, //查询abs buffer的使用率，即当前装载的数据和总buffer的百分比，ret = 0~100, 100代表100%

    CEDAR_ADEC_CMD_GET_TIME=0x40,       /* 获取当前播放时间                     */
    CEDAR_ADEC_CMD_GET_SPEED,           /* 获取当前播放速度                     */
    CEDAR_ADEC_CMD_GET_BITRATE,         /* 获取当前播放bit rate                 */
    CEDAR_ADEC_CMD_GET_BITSPERSAMPLE,   /* 获取bits per second                  */
    CEDAR_ADEC_CMD_GET_FS,              /* 获取播放频率                         */
    CEDAR_ADEC_CMD_GET_CHN,             /* 获取声道数                           */
    CEDAR_ADEC_CMD_SET_CHN,             /* 设置声道数                           */

    CEDAR_ADEC_CMD_SET_AB_A,            /* 设置A-B播放模式的A点, aux = ms,都是为音频文件的fseek方式的ab播放准备的接口函数                 */
    CEDAR_ADEC_CMD_SET_AB_B,            /* 设置A-B播放模式的B点, aux = BPointPts, ms                 */
    CEDAR_ADEC_CMD_SET_AB_LOOPCNT,      /* 设置A-B播放的循环次数                */
    CEDAR_ADEC_CMD_CLEAR_AB,            /* 清除A-B播放模式                      */
    CEDAR_ADEC_CMD_ENABLE_AB,           /* 激活AB播放                           */
    //CEDAR_ADEC_CMD_SET_AB_MODE,         /* 设置A-B播放的实现方式,aux = ADRV_AB_MODE_JUMP等  */ 

    
    CEDAR_ADEC_CMD_GET_TAG,             /* 获取音频播放的标签信息               */
    CEDAR_ADEC_CMD_SET_FRSPEED,         /* 设置快进、快退速度                   */
    CEDAR_ADEC_CMD_GET_FRSPEED,         /* 获取快进、快退速度                   */

    
    CEDAR_ADEC_CMD_OPEN = 0x60,                //分配资源, aux, pbuffer现在没有意义了. aux = CEDAR_ABS_TYPE_MPEG12_LAYER1, pbuffer = 1表示是rawdata
    CEDAR_ADEC_CMD_CLOSE,               //释放资源
    //CEDAR_ADEC_CMD_SET_ABSTYPE,         //该接口不知所云,已删除. 设置abstype，aux =CEDAR_ABS_TYPE_MPEG12_LAYER1,  一般在解码库存在的情况下才有效，ret = EPDK_OK表示解码库接受了
    CEDAR_ADEC_CMD_UPDATE_ADRV,         //通知adec模块更新加载要求的adrv, aux = CEDAR_ABS_TYPE_MPEG12_LAYER1, pbuffer = nRawDataFlag
    CEDAR_ADEC_CMD_GET_ADRV_STATUS,     /* 得到adrv模块的状态, aux = 1:表示一定要返回稳定状态; aux=0表示立即返回.   */
    CEDAR_ADEC_CMD_SET_CEDARINI,        //设置cedarini, 要求在IOOpen之后设置
    

    CEDAR_ADEC_CMD_

} __audio_decoder_command_t;

//typedef enum __ADEC_INTERNAL_COMMAND
//{
//    CEDAR_ADEC_INTERNAL_CMD_IS_RAWDATA = 0x00,      /* 检查是否是raw data mode的adec plugin, 是:ret=EPDK_TRUE, 否:ret=EPDK_FALSE */
//    CEDAR_ADEC_INTERNAL_CMD_
//
//} __adec_internal_command_t;

#endif /*_ADEC_H_*/
