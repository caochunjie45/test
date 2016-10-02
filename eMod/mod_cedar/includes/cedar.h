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
* File   : cedar.h
* Version: V1.0
* By     : Eric_wang
* Date   : 2011-2-10
* Description:
    cedar主控模块的对外头文件(在cedar中间件的范围内),也可以当做是各插件定义的反馈消息类型
    定义通向cedar线程的反馈消息的类型
********************************************************************************
*/
#ifndef _CEDAR_H_
#define _CEDAR_H_
enum
{
    _CEDAR_CMD_PLAY=1,              /* play                                     */
    _CEDAR_CMD_PAUSE,               /* pause                                    */
    _CEDAR_CMD_STOP,                /* stop                                     */
    _CEDAR_CMD_SET_PLAYLIST,        /* dir or playlist/playlistfile             */
    _CEDAR_CMD_FF,                  /* fast forward                             */
    _CEDAR_CMD_RR,                  /* fast backward                            */
    _CEDAR_CMD_CLEAR_FFRR,          /* clear fast forward/backword              */

    _CEDAR_CMD_STOP_RECORD = 0x20,
    
    _CEDAR_CMD_PLYEND = 0x80,       /* current file play end                    */
    _CEDAR_CMD_STEP,
    _CEDAR_CMD_JUMP,                //用户命令jump
    //_CEDAR_CMD_JUMP_ABPLAY,         //ABPlay所引起的jump
    _CEDAR_CMD_SEL_AUDSTREAM,       /* change aud stream                        */
    _CEDAR_CMD_SEL_SUBTITLE,        //选择字幕流

//以下是各模块的反馈消息
    _CEDAR_CMD_APLYMSG_AB_B_DONE=0xa0,    //aply模块在ab播放时，遇到了B点
    _CEDAR_CMD_APLYMSG_STOP,

    _CEDAR_CMD_VPLYMSG_AB_B_DONE=0xb0,      //vply模块在ab播放时，遇到了B点
    _CEDAR_CMD_VPLYMSG_STOP,

    _CEDAR_CMD_DISK_FULL=0xd0,  //录制时,muxer模块发现磁盘满,发送的消息

	_CEDAR_CMD_SET_PANEL_SELECT,	//双屏显示的选择，0,LCD0; 1,LCD1

    _CEDAR_CMD_NOP,                 /* empty command, weekup cedar main task    */

    

    _CEDAR_CMD_
};

//该数据结构是给cedar->CedarCmdQ的消息队列制定的消息的数据结构
typedef struct tag_CedarMsg
{
    __u32   nMsgType;   //_CEDAR_CMD_PLAY等,各模块的反馈消息类型绝对要共用;主控模块自己的消息类型可以不同,但决不能和插件群的反馈消息类型值冲突.

    //下面是一些不同命令所需要携带的信息.目前_CEDAR_CMD_JUMP命令需要携带信息
    //_CEDAR_CMD_JUMP需要携带的信息
    __s32           accurate_jump_flag; //是否要求精确跳播, 1:精确跳播, 0:不要求精确跳播
    __u32           des_tm;             //跳播时的目标时间点，以offset time为基准
    __s32           nJumpReason;        //引起跳播的原因. 0:用户命令; 1:内部处理ab播放引起的跳播.

    //_CEDAR_CMD_VPLYMSG_STOP或_CEDAR_CMD_APLYMSG_STOP需要携带的信息
    __s32           fatal_error_flag;

    //_CEDAR_CMD_SEL_SUBTITLE所携带的信息
    __s32           nSubtitleStreamIndex;   //要更换到的字幕流的下标号
}CedarMsg;

#define CEDAR_PLUGINS_DIRECTORY   "d:\\mod\\cedar\\"
#define GINKGO_PLUGINS_DIRECTORY  "d:\\mod\\cedar\\"


#endif  /* _CEDAR_H_ */

