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
* File   : video_plybk_base.h
* Version: V1.0
* By     : Eric_wang
* Date   : 2011-2-23
* Description:
********************************************************************************
*/
#ifndef _SUBTITLE_PLYBK_BASE_H_
#define _SUBTITLE_PLYBK_BASE_H_


typedef struct __CEDAR_SPLY __cedar_sply_t;


//ʵ��Ա����
typedef __s32 (*CedarSply_Open)(__cedar_sply_t *thiz);  //��������������Դ�� ��semaphore, msg_queue��
typedef __s32 (*CedarSply_Close)(__cedar_sply_t *thiz);
typedef __s32 (*CedarSply_Start)(__cedar_sply_t *thiz); //�����߳�
typedef __s32 (*CedarSply_Stop)(__cedar_sply_t *thiz);
typedef __s32 (*CedarSply_SetFrameRect)(__cedar_sply_t *thiz);
typedef __s32 (*CedarSply_DrawSubtitle)(__cedar_sply_t *thiz, CSubtitleDrawItem *tmpSubInf, __s32 enable);
typedef __s32 (*CedarSply_ClearSubtitle)(__cedar_sply_t *thiz, CSubtitleDrawItem *tmpSubInf, __s32 enable);

typedef __s32 (*CedarSply_QueryFrame)(__cedar_sply_t *thiz, CSubtitleDrawItem *tmpSubInf);
#define SPLY_SUB_FRM_Q_SIZE     8


typedef struct __CEDAR_SPLY
{
    __u32               mid;            /* video play backģ��ID                */
    __u8                status;         /* video play back��ǰ�Ĳ���״̬, play, ff, rr, jump, pause, stop        */
    __u8                main_tsk_prio;  /* video play back���߳����ȼ�          */
    __u8                stop_flag;      /* video play back main task stop flag  */
    __s8                sub_show_flag;  /* picture display switch flag������Ƿ�Ҫ��ʾͼ��          */
    __s8                frame_show_flgs;        //��һ֡�ı�ʶ����1��ʾ���������֡�ǵ�һ֡����0��ʾ���������֡�ǵ�һ֮֡���֡,��һ֡�Ѿ�����ɹ�.��ʼ��Ϊ1.�ڵ�һ֡����ɹ���������Ϊ0


    __sub_inf           sub_inf;         //��Ļ��Ϣ��

//    __s8                de_layer_can_be_use;    //��ʾde_layer�Ƿ����ʹ���ˣ�vply�涨ֻ����DISP_CMD_LAYER_SET_PARA�����ʹ�ø�ͼ��
//    __s32               de_layer_hdl;   /* video ͼ����                       */
//    __s32               vid_wind_ratio; /* video ͼƬ��ʾ����ģʽ, ����nVidShowMode, enum __CEDAR_VID_WINDOW_RATIO_MODE, CEDAR_VID_WINDOW_BESTSHOW��               */
//    __pCBK_t            CB_SetSrcScnWinPara;    //����srcwin, scnwin��callback����,��Ӧ�ó�����������.
//    __rect_t            video_window;   /* ��Ƶ�������Ϣ,������Ļ�ϻ�����������ʾ�������λ�úͿ�ߣ�����������Ϊ��λ                       */
//    __rect_t            video_window_show_rect; //��¼��Ƶ����������ʾ������
//    __rect_t            src_pic_rect;   //src_win;       /* ʵ����ƵͼƬ��framebuf��λ��                         */
//    __rect_t            src_pic_show_rect;  //��¼֡buffer�е���ʾ����
//    __rectsz_t          src_pic_size;   //true_picture_size;   /* ʵ����ƵͼƬ�Ĵ�С����Ϊ������framebuffer�Ŀ�߶��Ǻ�����ģ�framebuffer�Ĵ�С��һ����ʵ��ͼƬ�Ĵ�С */
//    __rectsz_t          src_pic_show_size;  //picture_size;   /* ʵ����ƵͼƬ��Ҫ��ʾ���ֵĴ�С                         */
//    
//    __u16               color_format;   //��¼��ǰframe��color format, ��������֡��color format�ı�ʱ������DE.����ΪPIC_CFMT_YCBCR_420,PIXEL_YUV420
//    __s32               pic_pix_ratio;  /* ͼƬ���ؿ�߱���������Ƶ�ļ�����õ�,�ǽ���ͼ�����ؿ�߱�. ���ر���û�п�߸����������������:�ڸ߶ȵ����ظ�����ȵ������, ԭͼ�Ŀ�ȵ����ظ����������ͼ��Ŀ�ȵ����ظ����ı�ֵ���������ʾʱ����ð����ظ�����������ԭͼ��ͬ������ͼ�����                     */
//    __u8                horizontal_scale_ratio; //vdrv������ʱˮƽ����scale�ı���, what ratio this picture has been scaled down at horizon size, 0: 1/1, 1: 1/2, 2: 1/4, 3: 1/8; 0xff:��Ч
//    __u8                vertical_scale_ratio;   //vdrv������ʱ��ֱ����scale�ı���, what ratio this picture has been scaled down at vetical size, 0: 1/1, 1: 1/2, 2: 1/4, 3: 1/8; 0xff:��Ч

    __mp                *ldec;          /* subtitle decode ģ����                */
    __mp                *avsync;        /* av syncģ����                      */

    __hdle                pSubSktin;      /* ����Ƶ���������ͨѶ���           */
//    __krnl_event_t      *pMsgChn;       //ͨ��cedar���ص���Ϣͨ��
//    MsgQueue            *pCedarMsgChn;    //ͨ��cedar���ص���Ϣͨ��
    __pCBK_t              CB_MsgQPost;  //ͨ��cedar���ص���Ϣͨ��,��callback��post.
    __pCBK_t              CB_ShowDraw;  //����Ļ
    
    //__s8                call_back_flag; /* call-back����ʱ����ע��״̬        */
//    __s8                layer_open_flag;/* ��¼ͼ�㵱ǰ������״̬��open/close         */
//    __s8                layer_status;   /* ��Ƶͼ�㵱ǰӦ�õ�״̬               */
//    __s8                pic_set_flag;   /* ͼƬ��С���ñ�־,��ʾ���õ���һ֡��setpara��,�ļ����Ž�������0                     */
//
    //__vply_pic_frm_q_t  pic_frm_q;      /* ͼ��֡������Ϣ                       */
    //__krnl_event_t      *pSemReleaseFrmbuf;

    //VplyExtraFrameManage    ExtraFrameMgr;  //�������֡�ĸ��ֲ����������ļ��л����޷�ģʽ

    //__s8                forbid_set_fb;  /* ��ֹ����DE��SET_FB�ӿڣ���Ƶ���Ź����У�����������ڽ��������У�����SET_FB�����������Ѿ��ϳ�����������,����ת���߳�Ҫ֡ģʽ,�ʷ����ñ���*/
//    __s8                set_fb_flg;     //ָʾ�Ƿ������DISP_CMD_VIDEO_SET_FB�ˣ���Ϊ���ͼ����Ч����Ĭ�����óɹ�������û�����set_fb.�ñ������Ǽ�¼�Ƿ�set_fb��. 1:������fb, 0:û�����ã���������߳�Ҫ֡ģʽ��Ƶ�
//    __s8                start_req_frame_loop_flg;   // 1:�Ѿ�����Ҫ֡ѭ��; 0:���ڵȴ�
//    __s8                scale_frame_request_flg;    // 1��ʾ����Ե�ǰ������ʾ��frame��scale.vply_task()����scale�󣬻�Ѹñ�����0.
    //FB                  *scale_fb;                  // ���ϲ�ģ�鴫��ָ�룬ֱ��ʹ��
    //VirtualDE_DispFb    scale_fb;   //�洢�ϲ�ģ�鴫�����Ϣ

    //AB���ŵĿ���,vply���ܲ����ab�������������ֻ������Ƶ�ļ���jump��ʽ��ab����
//    __s32               nABEnableFlag;  //0:diable, 1:enable
//    __s32               nBPointPts;     //��λ:ms, B���PTS
//    __s32               nNotifyBDoneFlag;       //�Ѿ�����Ϣͨ������B�㵽�����Ϣ, 1:�ѷ���, 0:δ����
//
//    __bool              first_frame_flg;        //��һ֡�ı�ʶ����1��ʾ���������֡�ǵ�һ֡����0��ʾ���������֡�ǵ�һ֮֡���֡,��һ֡�Ѿ�����ɹ�.��ʼ��Ϊ1.�ڵ�һ֡����ɹ���������Ϊ0
  
//    __s32               nFileSwitchVplyMode;    // CEDAR_FILE_SWITCH_VPLY_MODE_GAP��
//
//    __s32               nNewVideoPtsValidFlag;    //��ʾ�����õ�pts�Ƿ��Ѿ����ʱ������,����������ʱ,������1. �����ø�ʱ�����,������0.
//    __u32               nNewVideoPts;       //����ͷʱ,������Ϊʱ����Ҫ����,Ҳ������ΪҪ��pts.
//    __s32               nDtvDelayTime;       //�ڵ��ӹ㲥��,������cedar�����������õĻ���ʱ��.������ǰ��DMX/TSC�Ļ���ʱ��.��λms,����PTS��ͷʱʹ��
//    __s32               nDtvAVInterval;            //�ڵ��ӹ㲥��,��һ���յ�����Ƶpts����Ƶpts�Ĳ�ֵ,videoPts - audioPts, Ϊ����ʾvideo����, audio��Ҫ����׷video.

    CedarSply_Open              open;
    CedarSply_Close             close;
    CedarSply_Start             start;
    CedarSply_Stop              stop;
    CedarSply_SetFrameRect      SetFrameRect;    
    CedarSply_QueryFrame        QueryFrame;
    CedarSply_DrawSubtitle      DrawSubtitle; 
    CedarSply_ClearSubtitle     ClearSubtitle; 
   

  
} __cedar_sply_t;
extern __cedar_sply_t   *cedar_sply;

extern __s32 Sply_Init(__cedar_sply_t *cedar_sply, __u32 mid);
extern __s32 Sply_Exit(__cedar_sply_t *thiz);
extern __cedar_sply_t* newSply(__u32 mid);
extern void deleteSply(__cedar_sply_t *thiz);

#endif  /* _SUBTITLE_PLYBK_BASE_H_ */

