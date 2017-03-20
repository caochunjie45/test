
/*
********************************************************************************
*                                    eMOD
*                   the Easy Portable/Player Develop Kits
*                               mod_mmp sub-system
*                           ldec_txt decoder module
*
*          (c) Copyright 2009-2010, Allwinner Microelectronic Co., Ltd.
*                              All Rights Reserved
*
* File   : ldec_itxt_i.h
* Version: V1.0
* By     : xyliu
* Date   : 2010-4-26
********************************************************************************
*/


#ifndef _LDEC_IDVB_I_H_
#define _LDEC_IDVB_I_H_

//��Ҫ���ⲿͷ�ļ�
#include "ldec_itxt_s.h"


#define DVBDEC_MAX_LBSBUF_SIZE    (1024 * 128)  //lbsbufsize, ��ͬ����Ļ������,ֵ��һ��
#define DVBDEC_MAX_FRM_SIZE       (1024 * 32)  //һ֡�ĳ���,����itext��˵,1024�ֽ��㹻��,Ҳ���Ա�ʾ��Ļ���һ�е�buffer�Ĵ�С
#define DVBDEC_MAX_ANCI_SIZE      (1024 * 6)  //����lbsbuffer��ͷ��buffer,�����ַ��������Ļ,��frameһ����͹���.

#define LDEC_MAX_LBS_BUF_SIZE     (1024 * 128)
#define LDEC_MAX_LBS_SUB_SIZE     (1024 * 128)  //lbsbufsize, ��ͬ����Ļ������,ֵ��һ��,Isdb-T��32/64 ?
#define LDEC_MAX_LBS_ANCI_SIZE    (1024 * 6)     //��ֹ����ʱ���ݻ�ͷ
#define LDEC_MAX_FRM_SIZE     (1024)        //һ֡�ĳ���,����itext��˵,1024�ֽ��㹻��,Ҳ���Ա�ʾ��Ļ���һ�е�buffer�Ĵ�С

#define LDEC_MAX_LBS_SUB_NUM    (8)  //Isdb-T��4
#define LDEC_MAX_LBS_DATA_NUM   (64) //lbs�ֿ�ĸ���,ͬLDEC_MAX_TXT_DATA_NUM
#define DVBSUBDEC_MAX_LINE_NUM    4   //һ����Ļ����������

//typedef enum __SUBTITLE_DEC_RESULT
//{
//    LDEC_ERR_NONE               =  0,       // decode one subtitle successed
//	LDEC_ERR_LBS_UNDERFLOW      =  1,       // subtitle bitstream underflow
//	LDEC_ERR_NO_FRAMEBUFFER     =  2,       // there is no free subtitle buffer for decoder
//	LDEC_ERR_DEC_SUBTITLE       = -1,       // some error has happend when decode subtitle
//	LDEC_ERR_LFMT_ERR           = -2,       // the format of the video is not supported, fatal error
//	LDEC_ERR_
//}__ldec_result;

typedef struct __SUBTITLE_ITEM
{   
	__u8                    isFull;
	__u16                   subWidth;
	__u16                   subHeight;
	__u16                   startX;    
	__u16                   startY;
    __u16                   picWidth;
    __u16                   picHeight;
    __u32                   ptsVal;
	__u32                   timeDuration;
	__u16                   uDataType;
	__s32                   subDataLen;//��Ļ��һ��buffer��洢����Ч���ݳ��ȣ�������\0.���ܳ���subData[]��buffer�ĳ���.
	__u8                    *pSubtitle;
    struct __SUBTITLE_ITEM  *nextItem;
}__subtitle_item_t;

typedef struct __SUBTITLE_MANAGER
{
    __s8                reconBufIdx;
	__u8                startBufIdx;
	__u8                subIdxBuf[LDEC_MAX_LBS_SUB_NUM];
    __s16               validSize;
    __u8                subReadIdx;
    __u8                subWriteIdx;
    __subtitle_item_t   subItem[LDEC_MAX_LBS_SUB_NUM];
}__Ldec_manager_t;//__ldec_manager_t

typedef struct __SUBTITLE_DISPLAY
{
	__u8                 readDispIdx;
	__u8                 writeDispIdx;
    __u8                 dispSubIdx[LDEC_MAX_LBS_SUB_NUM];
	__u8                 dispSubNs[LDEC_MAX_LBS_SUB_NUM];
}__ldec_display_t;

typedef enum tag_DvbsubDecCmd
{
    DVBSUBDEC_CMD_SET_SUBTITLE_SOURCE,    //������Ļ��Դ,һ����ts,Ҳ�п�����mkv, CEDARLIB_FILE_FMT_TS��
    DVBSUBDEC_CMD_CHECK_DSP_TIME,      //* get the presentation time of next subtitle frame; ret = -1��ʾû����һ֡,0��ʾ�ҵ���һ֡, pbuffer =__u32*;
    DVBSUBDEC_CMD_NOTIFY_SYNCSTAT,    //֪ͨpgsdec׼������״̬ͬ��,��ʵ������decode()����buffer.
    DVBSUBDEC_CMD_CHECK_SYNCSTAT,     //��ѯ�Ƿ�״̬ͬ�������. ret = 0:���, ret = 1:��Ȼ��syncstat
    DVBSUBDEC_CMD_SET_CHAR_CODING_TYPE,//�����ַ������ʽ.
    DVBSUBDEC_CMD_,
}DvbsubDecCmd;



/*******************************************************************************
                            ��Ա����˵��
EtxtDec_GetLrcItem:
  nType:CEDAR_GET_SUB_INF_ALL/CEDAR_GET_SUB_INF_ITEM, 
  ret:NULL:û�н�����;__cedar_buf_inf_t:���nType=CEDAR_GET_SUB_INF_ALL;__cedar_subtitle_item_t:���nType=CEDAR_GET_SUB_INF_ITEM;  

    
Parameters: 
    
Return: 
    
Time: 2011/8/3
*******************************************************************************/
typedef struct __SUBTITLE_DEC __ldec_idvbsub_t;
typedef __s32   (*DvbsubDec_IoCtrl)(__ldec_idvbsub_t *thiz, __s32 cmd, __s32 aux, void *pbuffer);  //cmd = ItxtDecCmd
//typedef __s32   (*ItxtDec_VirtualIoCtrl)(ItxtDec *thiz, __s32 cmd, __s32 aux, void *pbuffer);  //cmd = EtxtDecCmd
typedef __s32   (*DvbsubDec_CheckSupport)(__ldec_idvbsub_t *thiz, __s32 nFileFmt);   //ret = EPDK_TRUE, EPDK_FALSE, nFileFmt = enum __CEDARLIB_LYRIC_FILE_FMT
typedef __s32   (*DvbsubDec_GetLrcItem)(__ldec_idvbsub_t *thiz, __u32 nPts, CCedarSubtitleFrame *pOutputItem, __s32 nOutputSubtitleSize);    //pOutputItem:app�ͽ���subtitle_item.ItxtDec�����д��Ȼ����ͷŽ���֡��. ret = EPDK_OK:�ҵ�������; EPDK_FAIL:û���ҵ�,nOutputSubtitleSize:outputItem��pSubtitle�ĳ���
typedef __s32   (*DvbsubDec_Decode)(__ldec_idvbsub_t *thiz);   //��Ļ����
//usedBytes:����ʱ��ʾpLbs�ĳ���,��������ʱ��ʾ�������˶����ֽ�. lbsChgFlg:ԭ�����Ƿ���Ļ��,���ڸ�issaʹ��,��ʾ�Ƿ���ssa�ĵ�һ������Ҳ����ͷ��Ϣ
typedef __s32   (*DvbsubDec_SubtitleDecode)(__ldec_idvbsub_t *thiz, CCedarSubtitleFrame *psubitem, __u8 *pLbs, __u32 *usedBytes, __s8 lbsChgFlg, __u8 *privateData);
typedef __s32   (*DvbsubDec_Open)(__ldec_idvbsub_t *thiz);
typedef __s32   (*DvbsubDec_Close)(__ldec_idvbsub_t *thiz);
typedef __s32   (*DvbsubDec_RequestWrite)(__ldec_idvbsub_t *thiz, __u32 uSizeRequested, __u8 **ppData0, __u32 *puSizeGot0, __u8 **ppData1, __u32 *puSizeGot1);
typedef __s32   (*DvbsubDec_UpdatePointer)(__ldec_idvbsub_t *thiz, __lbs_datctlblk_t * pLbsDataCtrlBlk,__u32 uDataSize);
typedef __s32   (*DvbsubDec_RequestFrame)(__ldec_idvbsub_t *thiz, __subtitle_item_t **pFrame);   //ret = EPDK_OK;
typedef __s32   (*DvbsubDec_ReleaseFrame)(__ldec_idvbsub_t *thiz, __subtitle_item_t *pFrame);   //ret = EPDK_OK;
typedef void    (*DvbsubDec_Destruct)(__ldec_idvbsub_t *thiz);

//***************************************************//
typedef struct __SUBTITLE_DEC
{   
    //ItxtDec base;
    //ʵ��Ա
    __s32               subtitle_bs_src;    //Ŀǰ�õ�����:CEDARLIB_FILE_FMT_MKV, CEDARLIB_FILE_FMT_VOB, CEDARLIB_FILE_FMT_TS, ��ʾ������Ļ��Դ
    __s32               subtitle_type;      // srt, sub, etc. enum __CEDARLIB_LYRIC_FILE_FMT
    //__s32               frm_rate;           //video frame rate. be careful : ��ֵ�Ŵ���1000��!, for sub decode
    //__s32               width;              //for sub decode
    //__s32               height;             //for sub decode
    //__s32               tvType;             //sub��Ļ�Ľ��룬��ҪtvSystemType, TV_SYSTEM_PAL
    //__u8                lastColorCode[4];
    //__u8                lastContrast[4];
    //__s32               screenWidth;    //��ʾ���ڵĿ��
    //__s32               screenHeight;

    __s32               nOneSegFlag;       //0:fullseg, 1:oneseg
    enum __CEDARLIB_LYRIC_ENCODE_TYPE   nCharEncodeType;

    //__s8                decFlag;            //����flag, ��ʾ�Ƿ����ڽ���,�𻥳����õ�.������
    //__s8                useAnciBuf;     //�Ƿ�ʹ����ancibuf,�������ͷ.Isub��ʹ��,�������������������
    __s8                nSyncStatFlag;   //nResetLdecBufFlag,��ʾ���³�ʼ������buffer,׼�����½���
    
    __s8                strDecName[16];     //�洢��Ļ������������: srt, txt, smi, ssa, lrc
    CedarLock           LbsBufLock;         //��__ldec_lbs_buf_t    ldecLbsBuf����ʱ�Ļ�����
    CedarLock           FrmMgrLock;     //��__ldec_manager_t    ldecManager����ʱ�Ļ�����
		
    __u32               nLbsBufSize;    //ָʾldecLbsBuf��buffer�Ĵ�С,�ڳ�ʼ��ʱ�����า�Ǹ�ֵ.
    __u32               nLdecFrameSize;  //ָʾldecManager��frame�Ĵ�С,�ڳ�ʼ��ʱ�����า�Ǹ�ֵ. �ı���Ļ�����С, ͼ����Ļ��ܴ�.
    __u32               nanciDataBufSize;   //ָʾanciDataBuf�Ĵ�С,������һ��ֵ,���������Ҫ���Ը���.
   
//=================== DVB-T Demo Data =========================
     void                *privInfo;
    __u8                 subIdxBuf[LDEC_MAX_LBS_SUB_SIZE];
    __ldec_lbs_buf_t     ldecLbsBuf;
    __Ldec_manager_t     ldecManager;
    __ldec_display_t     ldecDisplay;
//=============================================================

    DvbsubDec_IoCtrl          DvbsubDecIoCtrl;  //������ṩʵ��,�������û���һЩ��Ա��ֵ
    DvbsubDec_RequestWrite    RequestWrite;
    DvbsubDec_UpdatePointer   UpdatePointer;
    DvbsubDec_GetLrcItem      GetLrcItem;     //������ṩʵ��,���������Ҫ�������Լ��ĺ�������.
    DvbsubDec_Open            open;           //������ṩʵ��,
    DvbsubDec_Close           close;          //ͬ��
    DvbsubDec_Decode          CommonDecode; //����ͨ��ʵ��, ���벿�ֵ����麯��SubtitleDecode���. isub�Ḳ��,ʹ���Լ���decode()����������.

    //���Ա
    DvbsubDec_CheckSupport    CheckSupport;   //������NULL.����ļ������Ƿ�֧��, ret = EPDK_TRUE:֧��;
    DvbsubDec_SubtitleDecode  SubtitleDecode; //���಻ʵ��, CommonDecode()�������.
    //ItxtDec_VirtualIoCtrl   EtxtDecVirtualIoCtrl;   //�еĺ�����ͬ����ֻ����Щ΢�Ķ�,���븲����д,��������ʵ��.

    //��������
    DvbsubDec_Destruct        Destruct;   //�����ṩʵ��,һ��������Ϣ,������븲��
}__ldec_idvbsub_t;
extern __s32 DvbsubDec_Init(__ldec_idvbsub_t *thiz);
extern __s32 DvbsubDec_Exit(__ldec_idvbsub_t *thiz);
extern __ldec_idvbsub_t *newDvbsubDec(void);
extern void deleteDvbsubDec(__ldec_idvbsub_t *thiz);
extern __s32 DvbsubDec_RequirePts(__ldec_idvbsub_t* thiz, __u32* pPts);

extern void ldec_dvbsub_init_info(__ldec_idvbsub_t *ldec_itxt);
//extern __u8 *ldec_process_chunk_cover_bufEnd(__ldec_lbs_buf_t *pLbsBufManager, __u8 **ppanciDataBuf, __u32 *pnanciDataBufSize);

#endif

