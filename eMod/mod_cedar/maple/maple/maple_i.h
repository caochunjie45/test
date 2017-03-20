
#ifndef MAPLE_I_H
#define MAPLE_I_H

//��Ҫ���ⲿͷ�ļ�
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
//������ʹ�õ�ͷ�ļ�
//#include "mod_maple.h"

//�ڲ�����ͷ�ļ�
#include "script.h"

#define DEMUX_VIDEO_PKT_BUF_SIZE    128         //* demux send data when video has more than 128 ts packets
#define DEMUX_AUDIO_PKT_BUF_SIZE    16          //* demux send data when audio has more than 16 ts packets
#define DEMUX_SECTION_PKT_BUF_SIZE  1           //* demux send data when section has more than 1 ts packets

#define MAPLE_CMDQ_SIZE             (8)

#define MIN_FILE_SPACE    (16*1024*1024)  //���ռ�С��MIN_FILE_SPACEʱ��׼������¼��
#define MAPLE_RECORD_WTBUFFER_SIZE  (8*1024*1024)          //CEDARFS�Ļ�����,��Ϊ8M.����ʱ��Ϊ128M.

//maple status������cedar��ֵ����һ��
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
    CedarHerbMediaFileInf   record_media_file;     /* ¼�Ƶ�ý���ļ��ľ���·����,����:e:\\dtv_service0.ts */
    
    __u32                   demux_mid;
    __mp*                   demux;
    __u32                   demod_mid;
    __mp*                   demod;
    __u32                   player_mid;
    __mp*                   player;

    __u8                    status;                 /* maple��ǰ״̬, __maple_status_t */
    __u8                    nRecordStatus;          /* maple��ǰ¼�Ƶ�״̬, __maple_status_t */
    __pCBK_t                record_cb;              /*¼�ƻص��������ṩ¼����Ϣ���û���*/
    __u32                   nRecordProgId;  // ��ǰ����¼�Ƶ�service���ڲ���ʶ��.�ڲ�������service��ͳһ�ı�ʶ��,�����ظ�.-1��ʾû��.��channelId��ͬ������˼,��������.
    __u8                    maintskprio;            //maple���߳�
    SemObj                  *pSemStopPlayDone;      //��ʾstop play��ɵ��ź���.����STOP�ӿڵ�ͬ������
    SemObj                  *pSemStopRecordDone;      //��ʾstop record��ɵ��ź���.����STOP_RECORD�ӿڵ�ͬ������
    __krnl_event_t          *CmdQ;                  /* maple��������Ķ��У����˽���maple IO�ӿڷ��͵���Ϣ֮�⣬����Ҳ����play_ctrl������stop����Ϣ */

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
    RecordServInfo          curRecordServInfo;      // ��ǰ����¼�Ƶ�service����Ϣ
    
    __s32                   audioIdx;
    
#if (MAPLE_TV_STANDAR == MAPLE_TV_CMMB)

    __pCBK_t                cmmb_data_svc_update_begin_cb;
    __pCBK_t                cmmb_data_svc_update_end_cb;
    
    maple_cmmb_datasvc_cb_param_t *cmmb_datasvc_updatebegin_cb_param; 
    maple_cmmb_datasvc_cb_param_t *cmmb_datasvc_updateend_cb_param; 

#endif   
    //����pcrptsУ׼����Ҫ�õ������ɳ�Ա,playService()ʱ����.��MClose()ʱ���ټ��һ��.
    __u8                pcrDetecttskprio;   //���pcrpts���߳�,�����avsync�Ľӿ�����pcrpts.
    SemObj*             pSemPcrDetect;      //ͨ���õ�semaphore, __krnl_event_t*
    __u32               pcrcnt;             //��¼pcr�жϵĴ���
    __u32               PcrPts;             //��playService(),����PCRDetectTask()֮��,���¼��ǰ������pts, ��λms
		__u32 							PrePcrPts1;					//�ڵ�ǰPcrPts֮ǰ��һ��PCRֵ
		__u32 							PrePcrPts2;					//��PrePcrPts1֮ǰ��һ��PCRֵ
		__u32 							Adjustpcrcnt;				//�쳣PCR����
    //��¼dmx���ص���ͨ����id��,Ϊ�ر���׼��.
    __s32       nVideoVChanId;
    __s32       nAudioVChanId;
    __s32       nSubtitleVChanId;

    //��¼app���õ��ַ������ʽ, Ĭ��EPDK_CHARSET_ENM_ISO_8859_15
    __epdk_charset_enm_e    charCodeType;

    //¼������ı���
    FILE        *pRecordFile;
    SemObj      *pSemWriteFS;      //��ʾ¼��ʱдcedarfs���ź���,��Ϊdmx��ͨ���ǲ�����,���ǲ�������д,������һ��semaphore����.
    __s32       nRecordPatVChanId;
    __s32       nRecordPmtVChanId;
    __s32       nRecordVideoVChanId;
    __s32       nRecordAudioVChanId;
    __s32       nRecordSubtitleVChanId;

    __u64       nWtSize;    //�Ѿ�д����ֽ���.
    __u64       nDiskSpace; //��ʼд��ʱ��,���̵�ʣ��ռ�.
}maple_t;

#endif

