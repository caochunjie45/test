/*
*********************************************************************************************************
*                                                    eMOD
*                                   the Easy Portable/Player Operation System
*                                              mod_mmp sub-system
*
*                                     (c) Copyright 2008-2009, Kevin.Z China
*                                              All Rights Reserved
*
* File   : audio_plybk.h
* Version: V1.0
* By     : Kevin.Z
* Description:
    audio_plybk的api头文件
*********************************************************************************************************
*/
#ifndef _AUDIO_PLYBK_H_
#define _AUDIO_PLYBK_H_

enum
{
    CEDAR_APLY_CMD_START=0x00,          /* start                                */
    CEDAR_APLY_CMD_STOP,                /* stop                                 */
    CEDAR_APLY_CMD_PAUSE,               /* pause                                */
    CEDAR_APLY_CMD_CONTI,               /* continue playing                     */
    CEDAR_APLY_CMD_FF,                  /* fast forward                         */
    CEDAR_APLY_CMD_RR,                  /* fast reverse                         */
    CEDAR_APLY_CMD_CLEAR_FFRR,          /* clear fast forward/reverse           */
    CEDAR_APLY_CMD_GET_STATUS,          /* 获取APLY模块的状态,aux=0:立刻返回, aux=1:等status平稳后再返回                   */
    CEDAR_APLY_CMD_JUMP,                /* jump                                 */
    CEDAR_APLY_CMD_CLEAR_JUMP,          /* clear jump                           */
    CEDAR_APLY_CMD_ENABLE_AB,           //通知aply模块，ab播放开始，正常情况下，一定是音频文件的jump方式的ab播放,aply只有在play状态下才会检测b点是否已到
    CEDAR_APLY_CMD_CLEAR_AB,            //通知aply模块，ab播放结束，不用再检测B点了
    CEDAR_APLY_CMD_SET_AB_B,                //设置AB播放时的B点的PTS, aux = pts, unit:ms
    CEDAR_APLY_CMD_CLEAR_NOTIFYBDONEFLAG,   //AB播放时，把通知B点到达的标记位清0.
    CEDAR_APLY_CMD_OPEN,                //分配资源
    CEDAR_APLY_CMD_CLOSE,               //释放资源
    CEDAR_APLY_CMD_SET_CEDARINI,         //把用户的定制信息传给audio_plybk模块.可以接受指针,并使用.因为该块内存由cedar主控分配,运行期间绝不会释放. pbuffer = CedarINI*

    CEDAR_APLY_CMD_SET_AVSHDL=0x20,     /* set A/V sync module handle, START之前设置,           */
    CEDAR_APLY_CMD_SET_CEDARHDL,        //可以被废弃了, 设置cedar主控模块的句柄,因为不再调用cedar的stop接口了
    CEDAR_APLY_CMD_SET_PCMSKTIN,        /* pcm buf from audio decoder, START之前设置,STOP时会清0          */
    CEDAR_APLY_CMD_SET_BITSPERSAMPLE,   /* set bits per sample，未实现                  */
    CEDAR_APLY_CMD_SET_FS,              /* set fs, 未实现                               */
    CEDAR_APLY_CMD_SET_CHN,             /* set channel number，未实现                   */
    CEDAR_APLY_CMD_SET_VOLUME,          /* set volume，未实现                           */
    CEDAR_APLY_CMD_GET_VOLUME,          /* get volume, 未实现                         */
    CEDAR_APLY_CMD_GET_MAXVOLUME,       /* get max volume, 未实现                       */
    CEDAR_APLY_CMD_SET_RAW_DATA_MODE,   /* aux=1,代表是raw data decode lib向aply传送数据，aux=0,代表是common decode lib向aply传送数据.
                                            一般情况下， raw data decode lib会传送raw data，但是也有特殊情况，如EAC3，传送的是pcm data.
                                            START之前设置*/
    //CEDAR_APLY_CMD_SET_FFRR2PLAY_DELAY_TIME,    //已经被废弃. 设置在FFRRJUMP->PLAY时需要delay的时间,用于缓冲数据， aux = time, unit:ms
    CEDAR_APLY_CMD_SET_CB_MSGQPOST,           //设置通向cedar主控模块的反馈消息通道,ab播放时，检测到B点已到，通过消息通道通知CEDAR主控模块去处理, pbuffer = __krnl_event_t  *CmdQ
    CEDAR_APLY_CMD_NEED_VIDEOCLOCK_START,       //是否需要启动video clock,aux =1:需要;因为在电视广播的情况下,如果用pcrpts校准, audio clock需要和video clock同步,那么就要启动video clock.在start之前设置
    
    CEDAR_APLY_CMD_GET_SAMPCNT = 0x30,  /* AVSYNC模块专用, 返回值是u32的，务必记住. 统计sample数的相关操作，得到当前播放出来的sample数，
                                        必须考虑dac的buffer缓冲的数据,ret = cnt, 返回的sample数就是原始的sample数，已修正了变速的影响，
                                        也已修正了dac buffer中缓冲的sample数, 可以直接计算,//最高bit可能为1,所以调用者要强转为__u32   */
    CEDAR_APLY_CMD_SET_SAMPCNT,         /* AVSYNC模块专用, 设置当前播放出来的sample计数的起始值. 必须考虑dac的buffer缓冲的数据, 
                                        aux = cnt, 防止误差;这样做是为了尽量避免关线程调度, aux = __u32 ，设置的cnt要求必须是按变速前计算的 */
    CEDAR_APLY_CMD_GET_CACHE_SAMPCNT,   //转换为变速前的sample数,再返回,ret = __s32, -1代表有错误
    CEDAR_APLY_CMD_GET_CACHE_SIZE,   //得到dac buffer中的字节数，直接返回, 不转换为变速前.所以是dac buffer中的真实的字节数. 返回值是__s32, -1代表失败
    
    CEDAR_APLY_CMD_REQUEST_SAMPLOCK,    //准备连续调多个sample相关的IO接口，又希望是原子操作，故提供本接口,ret = EPDK_OK. aux = 0:必须申请到lock才能返回
    CEDAR_APLY_CMD_RELEASE_SAMPLOCK,

    //CEDAR_APLY_CMD_SET_NEW_VIDEO_PTS,           //aux = ms.只有需要设置vclock时,aply才会设置video clock.在电视广播中,如果是音频广播,那么在pts回头后,aply就要负责设置vclock的新的时间,这个时间是经过前面的模块处理过的时间. vclock是作为stream clock看待的.
    CEDAR_APLY_CMD_SET_NEW_AUDIO_PTS = 0x40,           //pbuffer == __s64*. 有可能为负值,所以必须__s64. 同上,这是设置audio clock的pts.可以认为是换了时间轴,所以需要重设一次.需要注意的是:设置后,aply不会马上重设时间轴,这是因为adec有缓冲,aply会等到带着这个pts的音频帧到来后才设置audio clock.而且设置与否无关紧要，关键是videoclock的立即设置.
    CEDAR_APLY_CMD_SET_DTV_DELAY_TIME,              //在电视广播中,定义在cedar插件里面的设置的缓冲时间.不包括前端DMX/TSC的缓冲时间.单位ms,处理PTS回头时使用
       
    CEDAR_APLY_CMD_

};

#endif /*_AUDIO_PLYBCK_H_*/

