/*
*********************************************************************************************************
*                                                    eMOD
*                                   the Easy Portable/Player Operation System
*                                           mod_cedarcore sub-system
*
*                                (c) Copyright 2009-20010, Clif.Chen China
*                                               All Rights Reserved
*
* File   : play_ctrl.h
* Version: V1.0
* By     : clif.c
* update : 
*********************************************************************************************************
*/

#ifndef _PLAY_CTRL_H
#define _PLAY_CTRL_H

//#include "mod_dvb_i.h"
#include "mod_cedar_i.h"
#include "audio_plybk.h"
#include "video_plybk.h"
#include "subtitle_plybk.h"
//#include "psr_2_ldec.h"

#define DVBC_CMDQ_SIZE             (8)  //play_ctrl�ڲ���cmdqueue���е�size
#define DVBC_MSGQ_SIZE             (4)  //���ϲ��feedback msgQ��size
#define DVBC_TMP_STATUS_FLAG       (1<<7)
#define DVBC_TMP_STATUS_RESET_FALG (1<<6)

#define DEBUG_SAVE_VIDEO_STREAM		(0)
#define DEBUG_SAVE_AUDIO_STREAM		(0)
#define DEBUG_SAVE_SUBTITLE_STREAM  (0)

#define PLAY_VIDEO_STREAM			(1) //�ڲ�����,��Ӱ��dmx
#define PLAY_AUDIO_STREAM			(1)
#define PLAY_SUBTITLE_STREAM        (1)
#define DEBUG_TIME					(0)
#define DRAW_RADIO_BACKGROUND       (0)


#define SYSTEM_CLOCK_START_TIME		(10000)		//* start point in ms, Ҳ����˵:����dmx���������ݵ�pts�Ƕ���,ͳһ��10s��ʼ����,�����ڲ�ʱ�䣬����һֱ����ͷ.
#define TIME_BEFORE_PLAY			(1000)

#define DVBC_TMP_STATUS_BIT			(0x8000)

//���ֵ����������
#define DTV_DELAY_TIME (300)   //������cedar�����������õĻ���ʱ��.������ǰ��DMX/TSC�Ļ���ʱ��.��λms,����PTS��ͷʱʹ��

//* the setting information for cedarcore sub-system
typedef struct __DVBC_SETTING_INF
{
    //* ��Ƶ���ŵ��������
    __u32       volume;                     //* ����ֵ
    __u32       eq;                         //* ��Ч����
    __u32       channel;                    //* ����ģʽ
    __u32       vpsspeed;                   //* �����ٶ�
    __u32       ab_loopcnt;                 //* A-B����ѭ������
    __u32       spectrum;                   //* �Ƿ�ʹ��Ƶ�׽���
    __s32	      swgain;
    __s8        usr_eq_filter[USR_EQ_BAND_CNT];

    //* �û��Զ�����Ч���˲���Ƶ�ε��˲�ϵ��
    __u8        volume_flag;                //* volume�����Ƿ���Ч�ı�־
    __u8        eq_flag;                    //* eq�����Ƿ���Ч�ı�־
    __u8        channel_flag;               //* channel�����Ƿ���Ч�ı�־
    __u8        vpsspeed_flag;              //* vpsspeed�����Ƿ���Ч�ı�־
    __u8        ab_loopcnt_flag;            //* ab_loopcnt�����Ƿ���Ч�ı�־
    __u8        spectrum_flag;              //* spectrum�����Ƿ���Ч�ı�־
    __s32       aud_suffix;                 //* ��ǰ���ŵ���Ƶ������Ƶ��������±�

    //* ��Ƶ���ŵ��������
    __s32       vid_layer_hdl;              //* ��Ƶ����ͼ����
    __rect_t    vid_window;                 //* ��Ƶͼ�񴰿�����
    __s32       vid_show_mode;              //* ��Ƶͼ����ʾ����ģʽ
    __u32       frpic_hold_time;            //* ���������״̬����ƵͼƬ��ͣ��ʱ��
    __s32       nCkEnableValue;             //* VirtualDE_DispLayerInfo->ck_enable�Ƿ���1,��1��ʾ��Ƶͼ������colorKey��͸��.���������app������Ҫ����,Ĭ��Ϊ0.

    __s8        vid_layer_hdl_flag;         //* vid_layer_hdl�����Ƿ���Ч�ı�־
    __s8        vid_window_flag;            //* vid_window�����Ƿ���Ч�ı�־
    __s8        vid_show_mode_flag;         //* vid_show_mode�����Ƿ���Ч�ı�־
    __s8        frpic_hold_time_flag;       //* frpic_hold_time�����Ƿ���Ч�ı�־

    //* ������Ƶ���Ź�����������
    __u32       frspeed;                    //* ����������ٶ�
    __u8        frspeed_flag;               //* frspeed�����Ƿ���Ч�ı�־
    
    __u8        vid_layer_alpha;
    __u8        alpha_blending_on;

} __dvbc_setting_inf_t;


typedef struct __DVB_CTRL_BLK                   //* ý�岥���м�����
{
    __u32               mid;
    __u8                maintskprio;
    __u8                status;                 //* ý�岥������ǰ״̬, __cedar_status_t

    __md_src_t          md_src;                 //* media source type;

    __dvbc_setting_inf_t     setting_inf;            //* ý�岥������������Ϣ
    __s32           nOneSegFlag;        //��ǰ�˵���ģʽ��isdbʱ,ָʾ��service��oneseg����fullseg�����.��������ģʽ��dvb,atsc,������������.��0����.  
    __s32           nMafEnable;                 //��¼maf�Ƿ�Ҫ��. 2:leve2��deinterlace, ���������Ὺʼ��������,de������ѡ��mafģʽ,����ռ�����;1:level1��deinterlace,de����Ӧ�û�ѡ��maf_bobģʽ,����ռ���Դ�һЩ. 0:�ر�.Ĭ�Ϲر�.
    __pCBK_t        VplyCB_SetSrcScnWinPara;    //������ʾģʽnVidShowMode,�����CEDAR_VID_WINDOW_USERDEFģʽ,����Ҫ����app��������callback.
    __pCBK_t        CB_ShowDraw;                // subtitle moudle callback function;

    __krnl_event_t          *pFeedBackMsgQ;      /* ���ϲ����ģ��ķ�����Ϣͨ��, ��maple���� */
    
    //__krnl_event_t      *CmdQ;                  //* cedarcore��������Ķ���
    MsgQueue            *CedarCmdQ;                 /* msg�����ݽṹ��CedarMsg, mmplayer��������Ķ��У����˽���cedarģ�鷢�͵���Ϣ֮�⣬����Ҳ����vply��aply���͵�ab�����ѵ�B�����Ϣ           */
    __s8                pic_show_flag;          //* ��־�Ƿ���Ҫ����Ƶͼ��

    __hdle              pABSSktout;             //* ��Ƶ������, �Լ�����.
    __hdle              pVBSSktout;             //* ��Ƶ������, �Լ�����.
    __hdle              pLBSSktout;             //* ��Ļ���������Լ�����.
    
    __krnl_sktfrm_t     *vbs_frm;   //�洢socket, request_mem()�õ�
    __krnl_sktfrm_t     *abs_frm;
    __krnl_sktfrm_t     *sbs_frm;   //

    __ad_codec_fmt_t    adFmt;      //��maple���ص���DVB_CMD_SET_MEDIA_INFOʱ����
    __vd_codec_fmt_t    vdFmt;      //ͬadFmt.
    __subt_codec_fmt_t  subtFmt;    //ͬadFmt.
    __s8                getAdFmt;   //��maple���ص���DVB_CMD_SET_MEDIA_INFO����audio��Ϣʱ���ô˱��λ.
    __s8                getVdFmt;   //ͬgetAdFmt
    __s8                getSubtFmt; //ͬgetAdFmt

    CedarLock           *pLockStreamPts;   //����StreamStartPts��AccumulateStartPts���ź���,�𻥳�����.
    __s64               StreamStartPts; //һ��stream�е���ʼ��pts,ȡԭʼֵ. һ��ȡvideo����audio��һ��,����video.��Ϊvideo clock��Ҫ��Ϊstream clock��,���ܳ���.
    __s64               AccumulateStartPts; //�ۻ���ʼʱ��,ÿ�λ�ͷ�Ժ���Ҫ�޸�.��ʱ��������videoΪ׼,������Ҫ��ȥptsBase, ���ǻ�ͷ.
    __s8                ptsBaseGet; //��ʾ�Ƿ��õ���ȷ�����ܵ�basetime,��update_data()�лḳֵΪ1.
    __s64               ptsBase;    //��λms.����Ϊ��ֵ.�ܵ�basetime,��ʹ��ͷҲ�������,�ڲ���ʱ���Ǽ�ȥ���basetime��,�������ԴӺ�С����ֵ��ʼ����,��ʹ����ͷ,__display_info_t��__u32Ҳ���Լ��20����.
    __s64               nAVPtsInterval;    //����Ƶ��ʱ���, ��һ���õ�����Ƶ��pts����Ƶpts�Ĳ�ֵ.videoPts - audioPts. Ϊ����ʾvideo����.��Ҫaudio����׷video.

	__s8				hasVideo;   //ͬhasAudio
	__s8				nGetVdFlag; //����Ƿ��õ�����Ƶ����
	__s64				vdStreamStartPts; //vdPtsBase; ����stream�еĵ�һ��video��ԭʼֵ,����ֻ�Ǽ�¼,û���õ�.������Ϊ��ts stream��һ��pts���ϵ�����stream,���pts��ͷ,����Ϊ����һ��stream. video����,����videoΪ׼.
    __s64				vdStreamPts;    //Ҳ��ԭʼֵ,��¼��ǰ�͵�����ȥ��pts,�´�����ʱ��,�����Ƚ�,���Ƿ��ͷ.��λms
	__s64				vdPts;          //��¼׼���͵������vdec��ֵ,��ȥptsBase,���һ�Ҫ���ǻ�ͷ.���ptsBase=0,���Ǵӵ�һ��pts��ʼ��ʱ.
	//__s64				firstVdPts;
	__s64               preVdPts[2];    //Ҳ��ת��pts�õ�,���ڷ�����.
	
	__s8				hasAudio;   //��maple���ص���DVB_CMD_SET_MEDIA_INFO����audio��Ϣʱ���ô˱��λ.��ʾ��audio����
	__s8				nGetAdFlag; //����Ƿ��õ�����Ƶ����
	__s64				adStreamStartPts;   //adPtsBase;ͬvdStreamStartPts
	__s64				adStreamPts;    //Ҳ��ԭʼֵ,��¼��ǰ�͵�����ȥ��pts,�´�����ʱ��,�����Ƚ�,���Ƿ��ͷ, ͬvdStreamPts
	__s64				adPts;          //ͬvdPts
	//__s64				firstAdPts;
	__s64               preAdPts[2];    //ͬpreVdPts[]
	
//#if PLAY_SUBTITLE_STREAM
	__s8                hasSubtitle;    //ͬhasAudio
	__s8                nGetSubFlag; //����Ƿ��õ���Ļ����
	__s64				subStreamStartPts;   //subPtsBase;ͬvdStreamStartPts
	__s64				subStreamPts;    //����ԭʼֵ, ͬvdStreamPts
	__s64				subPts;          //ͬvdPts
	__s64               preSubPts[2];    //ͬpreVdPts[]
	
//	__s64               subtPtsBase;
//	__s64               subtPts;
//	__s64               firstSubtPts;
//#endif
	__u32				pcrpts;
	__u32				prepcrpts[2];
    //* plug-in module id
    __u32               adec_mid;
    __u32               ardr_mid;
    __u32               aply_mid;
    __mp                *adec;
    __mp                *aply;
    
    __u32               vdec_mid;
    __u32               vply_mid;
    __mp                *vdec;
    __mp                *ardr;
    __mp                *vply;
    
#if PLAY_SUBTITLE_STREAM
    __u32               sdec_mid;
    __u32               sply_mid;
    __mp                *sdec;
    __mp                *sply;
#endif
    
    __u32               avsync_mid;
    __mp                *avsync;

#if DEBUG_TIME
	__u32 				time;
	__u32				newTime;
#endif

#if	DEBUG_SAVE_VIDEO_STREAM
	ES_FILE*            fpVideoStream;  //ͬfpAudioStream
#endif

#if DEBUG_SAVE_AUDIO_STREAM
	ES_FILE*            fpAudioStream;  //����dmx���������audio���ݵ�һ���ļ�
#endif

#if DEBUG_SAVE_SUBTITLE_STREAM
	ES_FILE*            fpSubtitleStream;  //����dmx���������subtitle���ݵ�һ���ļ�
#endif


#if ((PLAY_AUDIO_STREAM == 0) || (PLAY_VIDEO_STREAM == 0))
	__u8*				tmpBuf; //������ƭdmx��,�����ֹvideo,��ô��û��vdec��buf��dmx��,��ô��Ҫ��һ��buf����.��������player�ڲ�debug��.
#endif

#if (PLAY_SUBTITLE_STREAM == 0)
	__u8*				tmpSubtitleBuf; //������ƭdmx��,�����ֹsubtitle,��ô��Ҫ��һ��buf����.��������player�ڲ�debug��.
#endif

#if DRAW_RADIO_BACKGROUND
    __u8*               radio_bg;
    ES_FILE*            disFile;
	__u32               disLayer;
#endif

    __s32           aud_raw_data_mode;   //0:����raw data, 1:��raw dataģʽ��
    
    //�û������ýű���Ϣ
    //CedarINI        CustomCedarINI;   //�ͻ����Ƶ�������Ϣ
    CMapleINI       *pMapleINI;

    __u32           nReservedMemSize;   //������vdeclib����ʹ�õ��ڴ�����ʱ,Ԥ�����ڴ�����,Ĭ��Ϊ600*1024
    __s32           nStopMode;              //CEDAR_STOP_MODE_KEEP_PLUGINS��
    __s32           nFileSwitchVplyMode;    //CEDAR_FILE_SWITCH_VPLY_MODE_GAP��
    __u8						tvMode;								//1:tvmode,0:����ý��ģʽ�����TV TS����ʽ¼�Ƴɱ���ý�岥��ʱ�ļ���
    __s32           rotate_mode;    //������Ƶ����ģ�����ͼ��ĽǶ�. 0:Ĭ��Ϊԭͼ,1:˳ʱ��90��,2:180��,3:270��,4:ˮƽ����,5:��ֱ����
} __dvbc_t;

extern __dvbc_t *dvbc;
//extern __cedar_vply_t *cedar_vply;
//extern __cedar_aply_t *cedar_aply;
__s32 FileStop_process(__dvbc_t * dvbc);
void uninstall_plugins(__dvbc_t *dvbc);

__s32 play_media(__dvbc_t *dvbc);

extern __s32 CB_CedarMsgQPost(void *arg);   //����ԭ��__pCBK_t

#endif  //* _DVB_CTRL_H

