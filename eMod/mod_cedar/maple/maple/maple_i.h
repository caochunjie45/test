
#ifndef MAPLE_I_H
#define MAPLE_I_H

//需要的外部头文件
#include "maple_cfg.h"

#if (MAPLE_TV_STANDAR == MAPLE_TV_DVB_T)
#include "dvb_interface.h"
#endif
#if (MAPLE_TV_STANDAR == MAPLE_TV_CMMB)
#include "cmmb_interface.h"
#endif
#if (MAPLE_TV_STANDAR == MAPLE_TV_ISDB_T)
#include "isdb_interface.h"
#endif
#if (MAPLE_TV_STANDAR == MAPLE_TV_DTMB)
#include "dtmb_interface.h"
#endif
#if (MAPLE_TV_STANDAR == MAPLE_TV_ATSC)
#include "atsc_interface.h"
#endif
#if (MAPLE_TV_STANDAR == MAPLE_TV_DVB_T2)
#include "dvbt2_interface.h"
#endif
//给别人使用的头文件
//#include "mod_maple.h"

//内部共用头文件
#include "script.h"

#define DEMUX_VIDEO_PKT_BUF_SIZE    128         //* demux send data when video has more than 128 ts packets
#define DEMUX_AUDIO_PKT_BUF_SIZE    16          //* demux send data when audio has more than 16 ts packets
#define DEMUX_SECTION_PKT_BUF_SIZE  1           //* demux send data when section has more than 1 ts packets

#define MAPLE_CMDQ_SIZE             (8)

#define MIN_FILE_SPACE    (16*1024*1024)  //当空间小于MIN_FILE_SPACE时，准备结束录制
#define MAPLE_RECORD_WTBUFFER_SIZE  (8*1024*1024)          //CEDARFS的缓冲区,定为8M.测试时定为128M.

//maple status尽量和cedar的值保持一致
//typedef enum __MAPLE_STATUS             /* the status os cedar                  */
//{
//    MAPLE_STAT_PLAY=0,                  /* play                                 */
//    MAPLE_STAT_PAUSE,                   /* pause                                */
//    MAPLE_STAT_STOP,                    /* stop                                 */
    //MAPLE_STAT_FF,                      /* fast forward                         */
    //MAPLE_STAT_RR,                      /* fast backward                        */
    //MAPLE_STAT_JUMP,                    /* jump                                 */
//    MAPLE_STAT_
//} __maple_status_t;

typedef struct tag_RecordServInfo
{   
	__u16               PmtPid;
	__u16			    VideoPid;
    __u16			    AudioPid;
    __u16			    SubtitlePid;
}RecordServInfo;

typedef struct MAPLE
{
    __u32                   mid;
    CMapleINI               MapleINI;
    CedarHerbMediaFileInf   record_media_file;     /* 录制的媒体文件的绝对路径名,例如:e:\\dtv_service0.ts */
    
    __u32                   demux_mid;
    __mp*                   demux;
    __u32                   demod_mid;
    __mp*                   demod;
    __u32                   player_mid;
    __mp*                   player;

    __u8                    status;                 /* maple当前状态, __maple_status_t */
    __u8                    nRecordStatus;          /* maple当前录制的状态, __maple_status_t */
    __pCBK_t                record_cb;              /*录制回调函数，提供录制信息给用户层*/
    __u32                   nRecordProgId;  // 当前正在录制的service的内部标识号.内部对所有service的统一的标识号,绝不重复.-1表示没有.和channelId是同样的意思,换个名字.
    __u8                    maintskprio;            //maple的线程
    SemObj                  *pSemStopPlayDone;      //表示stop play完成的信号量.用于STOP接口的同步操作
    SemObj                  *pSemStopRecordDone;      //表示stop record完成的信号量.用于STOP_RECORD接口的同步操作
    __krnl_event_t          *CmdQ;                  /* maple接受命令的队列，除了接收maple IO接口发送的消息之外，现在也接收play_ctrl发来的stop的消息 */

    __pCBK_t                sl_finish_cb;           //* search service list finish callback function
    maple_sl_cb_param_t*    sl_finish_cb_param;     //* search service list finish callback param
    __pCBK_t                sl_event_cb;            //* search one service item callback function
    maple_sl_cb_param_t*    sl_event_cb_param;      //* search one service item callback param.
    
    __pCBK_t                sepg_finish_cb;         //* search simple EPG finish callback function
    maple_sepg_cb_param_t*  sepg_cb_param;          //* search simple EPG finish callback param.
    
    __pCBK_t                schdl_finish_cb;        //* search schedule finish callback function
    maple_schdl_cb_param_t* schdl_finish_cb_param;  //* search schedule finish callback param
    __pCBK_t                schdl_event_cb;         //* search one schedule event callback function
    maple_schdl_cb_param_t* schdl_event_cb_param;   //* search one schedule event callback param.
    
    __pCBK_t                lock_cb;                //* lock frequency point callback function
    maple_lock_param_t*     lock_event_cb_param;    //* lock frequency point event callback param.
    
    __s32                   cmmbDmxChan;            //* demux channel id of cmmb playing.
    maple_serv_item_t       curServItem;
    RecordServInfo          curRecordServInfo;      // 当前正在录制的service的信息
    
    __s32                   audioIdx;
    
#if (MAPLE_TV_STANDAR == MAPLE_TV_CMMB)

    __pCBK_t                cmmb_data_svc_update_begin_cb;
    __pCBK_t                cmmb_data_svc_update_end_cb;
    
    maple_cmmb_datasvc_cb_param_t *cmmb_datasvc_updatebegin_cb_param; 
    maple_cmmb_datasvc_cb_param_t *cmmb_datasvc_updateend_cb_param; 

#endif   
    //这是pcrpts校准机制要用到的若干成员,playService()时分配.在MClose()时会再检查一下.
    __u8                pcrDetecttskprio;   //检测pcrpts的线程,会调用avsync的接口设置pcrpts.
    SemObj*             pSemPcrDetect;      //通信用的semaphore, __krnl_event_t*
    __u32               pcrcnt;             //记录pcr中断的次数
    __u32               PcrPts;             //在playService(),启动PCRDetectTask()之后,会记录当前送来的pts, 单位ms
		__u32 							PrePcrPts1;					//在当前PcrPts之前的一次PCR值
		__u32 							PrePcrPts2;					//在PrePcrPts1之前的一次PCR值
		__u32 							Adjustpcrcnt;				//异常PCR计数
    //记录dmx返回的虚通道的id号,为关闭做准备.
    __s32       nVideoVChanId;
    __s32       nAudioVChanId;
    __s32       nSubtitleVChanId;

    //记录app设置的字符编码格式, 默认EPDK_CHARSET_ENM_ISO_8859_15
    __epdk_charset_enm_e    charCodeType;

    //录制所需的变量
    FILE        *pRecordFile;
    SemObj      *pSemWriteFS;      //表示录制时写cedarfs的信号量,因为dmx的通道是并发的,但是不允许并发写,所以用一个semaphore控制.
    __s32       nRecordPatVChanId;
    __s32       nRecordPmtVChanId;
    __s32       nRecordVideoVChanId;
    __s32       nRecordAudioVChanId;
    __s32       nRecordSubtitleVChanId;

    __u64       nWtSize;    //已经写入的字节数.
    __u64       nDiskSpace; //开始写的时候,磁盘的剩余空间.
}maple_t;

#endif

