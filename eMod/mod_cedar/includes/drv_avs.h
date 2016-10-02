/*
*********************************************************************************************************
*                                                    eMOD
*                                   the Easy Portable/Player Operation System
*                                              mod_mmp sub-system
*
*                                     (c) Copyright 2008-2009, Kevin.Z China
*                                              All Rights Reserved
*
* File   : drv_avs.h
* Version: V1.0
* By     : kevin.z
* Update : 2008-12-27 11:22
*********************************************************************************************************
*/
#ifndef _DRV_AVS_H_
#define _DRV_AVS_H_

//time delay of display show a picture
//#define DISPLAY_TIME_DELAY          (0x0000)        //delay 0 us
//#define FRM_SHOW_LOW_THRESHOLD      (10)            //low threshold is -10ms
//#define FRM_SHOW_HIGH_THRESHOLD     (20)            //high threshold is 20ms
//#define FRM_SHOW_SCALE_CNT          (3)             //一帧的显示时间的等分数
//#define FRM_SHOW_SCALE_LOW          (1)             //low_threshold的时间比，以FRM_SHOW_SCALE_CNT为分母
//#define FRM_SHOW_SCALE_HIGH         (2)             //high_threshold的时间比



#define AUDIO_CODEC_IF              (0x01)          //audio codec interface
#define DIGITAL_AUD_IF              (0x02)          //digital audio(i2s) interface

#define AVS_VIDEO                   (0)             //set parameter for video module
#define AVS_AUDIO                   (1)             //set parameter for audio module
#define AVS_AUDVID                  (2)             //set parameter both for audio and video

#define AVS_ENABLE                  (1)             //enable audio and video sync operation
#define AVS_DISABLE                 (0)             //disable audio and video sync operation


typedef enum __AVS_TIME_TYPE
{                                   // define parameter type for get and set time
    DRV_AVS_TIME_TOTAL=0,           // total time, base + offset
    DRV_AVS_TIME_BASE,              // base time, video time start value
    DRV_AVS_TIME_OFFSET,            // time offset, play time offset
    DRV_AVS_TIME_

} __avs_time_type_t;

typedef enum tag_SyncMode
{
    DRV_AVS_SYNCMODE_V2A =0,           // 视频追音频,以音频为准.
    DRV_AVS_SYNCMODE_A2V,              // 音频追视频,以视频为准
    DRV_AVS_SYNCMODE_

}SyncMode;

typedef enum __AVS_DRIVER_COMMAND
{                                   // command list for av sync
    DRV_AVS_CMD_MODULE_START = 0x00,    //初始化变量, 起线程,在DRV_AVS_CMD_START之前调用
    DRV_AVS_CMD_STOP,               // stop avsync,关线程
    DRV_AVS_CMD_START,              // start avsync，一般该接口只用来启动video,auido不从这启动，只有avscounter打开，才认为启动
    DRV_AVS_CMD_STOP_CLOCK,         // aux=AVS_VIDEO等,但该接口目前只用来关闭video clock. audio不从这走. video clock关闭时是要关avscounter的.和DRV_AVS_CMD_ENABLE_CLOCK不同之处在于这个接口是真要关avscounter的
    DRV_AVS_CMD_PAUSE,              // pause avsync
    DRV_AVS_CMD_CONTI,              // continue
    DRV_AVS_CMD_RESET,              // reset avsync
    DRV_AVS_CMD_FF,                 // fast forward
    DRV_AVS_CMD_RR,                 // fast reverse
    DRV_AVS_CMD_CLEAR_FFRR,         // clear fast forward/reverse
    DRV_AVS_CMD_GET_STATUS,         // get the status of avsync
    DRV_AVS_CMD_JUMP,                 // fast reverse
    DRV_AVS_CMD_CLEAR_JUMP,         // clear fast forward/reverse

    DRV_AVS_CMD_OPEN,       //分配资源
    DRV_AVS_CMD_CLOSE,      //释放资源

    //重点关注这些接口，涉及到aply的操作
    DRV_AVS_CMD_GET_VID_TIME=0x20,  // get video time, based on ms,返回值实际上是u32
    DRV_AVS_CMD_SET_VID_TIME,       // set video time, based on ms
    DRV_AVS_CMD_GET_AUD_TIME,       // get audio time, based on ms, aply，时间已换算成变速之前的时间
    DRV_AVS_CMD_SET_AUD_TIME,       // set audio time, based on ms, aply
    DRV_AVS_CMD_GET_CUR_TIME,       // get play time based on ms, 可能问aply, 播放持续时间,offset, 不考虑BaseTime
    
    DRV_AVS_CMD_GET_AUD_CACHE_TIME, // get audio cache time for skip video frame,时间已经换算成变速之前的时间
    DRV_AVS_CMD_GET_AUDIO_CACHE_STAT,   // 查询dac buffer中的audio数据的数量情况，ret = 1:buffer中的audio_data充足;0:audio_data快没有了。按变速前的时间来算
    DRV_AVS_CMD_CHK_AUDIO_PLAY_END, // check if all audio sample has play end, aux = 1,表示一定要DAC BUFFER为空才算PLAY_END。aux = 0:表示DAC BUFFER的数据小于8k就可以认为play end了。所以实际上是不转换为变速前的
    DRV_AVS_CMD_SET_PCR,            // 设置pcrpts的值,并且立刻校准videoclock. 一般是pcr中断一来就立刻通知avsync模块. 虽然不是在中断处理程序里调用, 但仍可以认为该接口被调用是实时的. EPDK_FAIL表示setpcr没有成功
            

    DRV_AVS_CMD_ENABLE_AVSYNC=0x40, // enable or disable audio video sync operation
    DRV_AVS_CMD_SET_FORCE_SYNC,     // set force sync, need adjust video clock
    
    DRV_AVS_CMD_REGIST_CLOCK,       // regist audio or video clock
    DRV_AVS_CMD_CHECK_CLOCK,        // check if audio or video clock is exist
    DRV_AVS_CMD_ENABLE_CLOCK,       // enable/disable audio clock or video clock，不涉及到avs_state的改变，所以一般audio从这里设置为开始和结束
    DRV_AVS_CMD_SET_VID_CLK_SPEED,  // set video clock speed(.../-3/-2/-1/1/2/3/...)
    DRV_AVS_CMD_GET_VID_VPS,        //0表示正常速度,10表示加速10%，和非变速情况下的正常播放相比较
    
    DRV_AVS_CMD_SET_APLYHDL,        //设置aply模块的句柄,用于查询当前播放出来的音频数据对应的原始sample数.在start时设置, stop时aply会把它设为0
    DRV_AVS_CMD_SET_AUD_CHAN_CNT,   // set audio channel count
    DRV_AVS_CMD_SET_PCM_SAMPRATE,   // set audio sample rate, for calculate audio time
    DRV_AVS_CMD_SET_AUD_VPS,        //  set audio variable speed, for calculate audio time，aux = -40~100. 因为avsync不需要处理音频变速,由ardr和aply处理. avsync查到的sample数一定是原始的sample数,不管变速与否.因此实际上这个接口是用来设置整体变速值,调节视频时间轴,以完成变速情况下的av同步功能的.和音频已经没有关系了
    DRV_AVS_CMD_GET_AUD_VPS,        //  参数意义和audio render库的定义一样, 参考[PostProcessCom.h]的 PostProcessSt->VPS -40 ~ 100, mod_cedar.h中定义的AUDIO_VPS_SLOWEST等是除以10的.即以0.1倍速为单位
                                    // 如果有视频，那么就必须根据AudVps调整视频时钟的目标分频系数，以达到av同步的目的. get_aud_vps, ret = -40~100
    DRV_AVS_CMD_SET_DACDEVHDL,      // 准备删除,set audio device handle, set or clear
    DRV_AVS_CMD_FORCE_DMA,          // 准备删除, 强行启动audio device的dma，让其将buffer中的数据送完. audio device启动dma送完后会自动复原, ret = EPDK_OK/EPDK_FAIL;
    //    DRV_AVS_CMD_SET_WAIT_FLG,  //设置要求等待的标记,目前是audio playback模块设,video playback模块查.aux = 1(等),0(不用等)
    //    DRV_AVS_CMD_GET_WAIT_FLG, //查询audio playback模块是否暂停
    DRV_AVS_CMD_SET_ARDRHDL,        //设置ardr模块的句柄,因为avsync要实现audio追video的机制,需要调用ardr模块的变速功能。
    DRV_AVS_CMD_SET_SYNCMODE,       //aux = SyncMode, DRV_AVS_SYNCMODE_V2A等
    
    DRV_AVS_CMD_SET_AV_END_FLAG=0x60,   // set audio or video end flag
    DRV_AVS_CMD_GET_AV_END_FLAG,        // get audio or video end flag
    DRV_AVS_CMD_SET_FFRR_FLAG,          // set flag if ff/rr is allowed, 即AVSYNC模块是否允许FFRR。模块本身无所谓FFRR是否允许，其实是VDEC或ADEC是否还允许
    DRV_AVS_CMD_GET_FFRR_FLAG,          // get flag if ff/rr is allowed
    
    DRV_AVS_CMD_

} __avs_driver_command_t;


#endif /* _DRV_AVS_H_ */

