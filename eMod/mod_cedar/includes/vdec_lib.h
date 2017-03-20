/*
*********************************************************************************************************
*                                                    eMOD
*                                   the Easy Portable/Player Operation System
*                                               mod_mmp sub-system
*                                                 vdec module
*
*                                    (c) Copyright 2008-2009, kevin China
*                                                All    Rights Reserved
*
* File   : mp4declib_api.h
* Version: V1.0
* By     : mengzh
* Date   : 2008-6-28 13:57
    [cedarlib]

*********************************************************************************************************
*/
#ifndef _VDEC_LIB_H2_
#define _VDEC_LIB_H2_

//����__vdrv_msg_type��������
typedef enum __VDEC_LIB_MSG_TYPE
{
    VDEC_LIB_MSG_TYPE_NONE = 0,     // = VDRV_MSG_TYPE_NONE,             // no message
    VDEC_LIB_MSG_SIZE_CHANGE,       // = VDRV_MSG_SIZE_CHANGE,               // picture frame change
    VDEC_LIB_MSG_I_DECODE,          // = VDRV_MSG_I_DECODE,                  // decode i frame
    VDEC_LIB_MSG_DATA_END,          // = VDRV_MSG_DATA_END,                  // vbs decode end
    
    VDEC_LIB_MSG_RELEASE_FRM_BUF,           // ֪ͨ���棬vdrv����ʾ֡�����ͷ���һ��frame buffer.
    VDEC_LIB_MSG_FREE_VBS_BUF,              //֪ͨ���棬vdrv�ͷ��˲���vbs buffer.
    
    VDEC_LIB_MSG_PREPARE_DECODE,              //֪ͨ���棬VDRV׼������vbs��free frame buffer��׼������
    VDEC_LIB_MSG_DECODE_ONE_FRAME_DONE,       //֪ͨ���棬VDRV�����һ֡�Ľ���,����ǳɹ�����ʧ�ܲ���Ҫ

    VDEC_LIB_MSG_SEND_VEREADY,              //����ve ready��semaphore.�õȴ�semaphore���߳�pend��
    VDEC_LIB_MSG_,
} __vdec_lib_msg_type;

typedef enum VDEC_IO_CMD {
    VDEC_IDLE_CMD = 0,
    VDEC_PAUSE_CMD,
    VDEC_STEP_CMD,
    VDEC_PLAY_CMD,
    VDEC_FORWARD_CMD,
    VDEC_BACKWARD_CMD,
    VDEC_STOP_CMD,
    VDEC_GET_STATUS_CMD,    //��ѯdec_lib״̬��MEDIA_STATUS_IDLE, 
    
    VDEC_DATAEND_CMD,
    VDEC_SC_CMD,    //scale����
    VDEC_ROT_CMD,   //��ת����:���ý���ͼ����ת�ĽǶ�.aux = 0:Ĭ��Ϊԭͼ,1:˳ʱ��90��,2:180��,3:270��,4:ˮƽ����,5:��ֱ����
    VDEC_REGCB_CMD,     //aux = callback type(VDEC_CB_TYPE_REGISTER_VE_FINISH_ISR), pbuffer = ��Ӧ��*fn
    VDEC_AUTOSC_CMD,    //����Ӧ����scale. aux = 1:��������Ӧscale; aux=0:������
    
    VDEC_VBS_USE_PTS_CMD,//VDEC_DECODE_ALL_VBS_FRAME_CMD,
    VDEC_SELECT_FRAME_USE_PTS_CMD,//VDEC_OUTPUT_ALL_DISPLAY_FRAME_CMD,
    VDEC_QUERY_VBSBUFFER_USAGE_CMD, //��ѯvbs buffer��ʹ���ʣ�����ǰװ�ص����ݺ���buffer�İٷֱȣ�ret = 0~100, 100����100%
    VDEC_SET_VBS_SIZE_CMD,  //����vdeclib��vbs buffer size�Ĵ�С
    VDEC_SET_FRAME_NUM_CMD, //����vdeclib��frame������
    VDEC_GET_VFORMAT_CMD,   //�õ�video codec format, pbuffer = __video_codec_format_t*, app malloc�ڴ�;
	VDEC_GET_OFQ_VALID_SIZE_CMD,   //�õ�output frame queue�е���Ч֡�ĸ���
    VDEC_SWITCH_DE_SCALE2_CMD,  // �ڽ�������У�����Դת��DE SCALER2,��Ҫ��SRAMʹ��Ȩ������֤VE CLOCK GATE�򿪡�Ҫ��֤�ڽ����û�н��������£����ø�IO�ӿڡ�
    VDEC_SWITCH_DEFAULT_CMD,    //  �ڽ�������У�����Դ�ջأ�ת��VE�Լ�,��Ҫ��SRAMʹ��Ȩ������֤VE��״̬����Ĭ��״̬��Ĭ��״̬��VE CLOCK GATE�رգ�����Ϊ�˽��͹���
    
    VDEC_MP4_DBLK_CMD = 0x20,
    //VDEC_RMG2_CMD,
    VDEC_IO_CMD_
} __vdec_io_cmd_t;
/*
================================================================================
                            RETURN VALUE MODULE
================================================================================
*/
#ifdef VDRV_TRUE
#undef VDRV_TRUE
#endif

#ifdef VDRV_FALSE
#undef VDRV_FALSE
#endif

#ifdef VDRV_SUCCESS
#undef VDRV_SUCCESS
#endif

#ifdef VDRV_FAIL
#undef VDRV_FAIL
#endif

#ifdef VDRV_NULL
#undef VDRV_NULL
#endif

#define VDRV_TRUE                   (1)
#define VDRV_FALSE                  (0)
#define VDRV_SUCCESS                (0)
#define VDRV_FAIL                   (-1)
#define VDRV_NULL                   0

//define the result of frame decode
typedef enum __PIC_DEC_RESULT
{
    PIC_DEC_ERR_VE_BUSY         = -1,   // video engine is busy
    PIC_DEC_ERR_VFMT_ERR        = -2,   // the format of the video is not supported, fatal error

    PIC_DEC_ERR_NONE            = 0,    // decode one picture successed
    PIC_DEC_ERR_FRMNOTDEC       = 1,    // decode one picture successed
    PIC_DEC_ERR_VBS_UNDERFLOW   = 2,    // video bitstream underflow
    PIC_DEC_ERR_NO_FRAMEBUFFER  = 3,    // there is no free frame buffer for decoder
    PIC_DEC_ERR_DEC_VBSEND      = 4,    // decode all video bitstream end

    PIC_DEC_ERR_

} __pic_dec_result;

typedef struct{
    __u8 MACC_INT_VE_FINISH_FLG;
    __u8 MACC_INT_VE_ERROR_FLG;
    __u8 MACC_INT_VE_MEMREQ_FLG;
    __u8 MACC_INT_VE_DBLK_FLG;
} __ve_interupt_status_t;

typedef struct _VDEC_PORT_ VDEC_PORT;
typedef struct _VDEC_PORT_
{
    //__s16       (*vdec_port_exit)(VDEC_PORT *p);
    __s16       (*open)(VDEC_PORT *p);
    __s16       (*close)(VDEC_PORT *p);
    __s16       (*decode)(VDEC_PORT *p);
    __s16       (*IoCtrl)(VDEC_PORT *p, __u32 uCmd, __u32 uParam, void *pbuffer);   //uCmd: __vdec_io_cmd_t
    __s16       (*RequestWrite)(VDEC_PORT *pDev, __u32 uSizeRequested, __u8 **ppData0, __u32 *puSizeGot0, __u8 **ppData1, __u32 *puSizeGot1);
    __s16       (*UpdatePointer)(VDEC_PORT *pDev, struct VDecDataCtrlBlk * pVDataCtrlBlk,__u32 uDataSize);
    __s16       (*DisplayRequest)(VDEC_PORT *pDev, struct Display_Info *pDisplay_Info);
    __s16       (*DisplayRelease)(VDEC_PORT *pDev, __u8 frm_idx);
    __s32       (*SetVbsInf)(VDEC_PORT *pDev, void *pVbsInf);   //pVbsInf = __vdec_mpeg4_vbs_inf_t*
    __s32       (*ProcessVEInterupt)(VDEC_PORT *pDev, __ve_interupt_status_t  *pVEStatus);   //���帺����VE�жϵ����󣬶�ȡ�Ĵ���

//    __s32       (*CbSendMsg)(__s32 nMstType, void *pMsg);   //nMstType:__vdec_lib_msg_type
//    __u32       (*CbGetVideoClkTime)(void);  //�õ�ϵͳʱ�����ʱ�䣬ѡ֡ʱʹ�ã�����ֵ�ĵ�λ��ms
//    __s32       (*CbRegisterVEFinishISR)(__u32 irqno, void *pArg);  //irqno:VE_FINISH���жϺ�, pArg��ʱû��
//    __s32       (*CbUnRegisterVEFinishISR)(__u32 irqno, void *pArg);  //irqno:VE_FINISH���жϺ�, pArg��ʱû��
//    __s32       (*CbWaitVeReady)(void); //��VEReady.��Ϊ�ǵ�semaphore,ϵͳ��أ�������Ϊcallback.�����������ʵ��.
} VDEC_PORT;
//extern VDEC_PORT* VDEC_PORT_init(__s32 * ret);
extern VDEC_PORT* newVDEC_PORTChild(void);
extern void deleteVDEC_PORTChild(VDEC_PORT* thiz);

//��Ҫ�ⲿʵ�ֵĺ���
extern int Vdeclib_DisplayFrameQueue_Lock(void);    
extern int Vdeclib_DisplayFrameQueue_Unlock(void);
extern int SysCb_SendMsg(int nMsgType, void *pMsg); //nMstType:__vdec_lib_msg_type
extern unsigned int SysCb_GetVideoClkTime(void);    //�õ�ϵͳʱ�����ʱ�䣬ѡ֡ʱʹ�ã�����ֵ�ĵ�λ��ms
extern int SysCb_RegisterVEFinishISR(unsigned int irqno, void *pArg);  //irqno:VE_FINISH���жϺ�, pArg��ʱû��
extern int SysCb_UnRegisterVEFinishISR(unsigned int irqno, void *pArg);  //irqno:VE_FINISH���жϺ�
extern int SysCb_WaitVeReady(void); //��VEReady.��Ϊ�ǵ�semaphore,ϵͳ��أ�������Ϊcallback.�����������ʵ��.

#endif  /* _VDEC_LIB_H2_ */

