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
* File   : itxt_decode.h
* Version: V1.0
* By     : Eric_wang
* Date   : 2011-9-24
* Description:
    ������Ļ�������,��Ҫ��ȡ��itext, issa, isub��ͬ�������Ļ���
********************************************************************************
*/
#ifndef _ITXT_DECODE_H_
#define _ITXT_DECODE_H_

#define TV_SYSTEM_PAL       0
#define TV_SYSTEM_NTSC      1

#define TOP_FIELD_FLAG      0
#define BOTTOM_FIELD_FLAG   1

//������Щ����, ��ֻ�����ο�����,��ͬ��������Ļ�����������趨�Ĳ������Բ�һ��.
#define LDEC_MAX_TXT_BUF_SIZE     (1024 * 128)  //lbsbufsize, ��ͬ����Ļ������,ֵ��һ��
#define LDEC_MAX_TXT_DATA_SIZE    (1024 * 512)  //sub������õ�
#define LDEC_MAX_TXT_FRM_SIZE     (1024 * 256)   //һ֡�ĳ���,��ͬ����Ļ������,ֵ��һ��. ldec_itxt->outputItem.pSubtitle���˱�׼malloc�ڴ�. ������subΪ��׼,�ı����Ͳ���malloc��ô���.
#define LDEC_MAX_TXT_ANCI_SIZE    (1024 * 6)     //��ֹ����ʱ���ݻ�ͷ,

#define LDEC_MAX_TXT_FRM_NUM       (4)  //frame�ĸ���, isdbԭ����32,Ҳ���Ը�Ϊ4.
//#define LDEC_MAX_TXT_DATA_NUM      (64) //lbs�ֿ�ĸ���
//
//#define LDEC_BUF_PRIVATE_DATA_LEN (16)

typedef enum __SUBTITLE_DEC_RESULT
{
    LDEC_ERR_NONE               =  0,       // decode one subtitle successed
    LDEC_ERR_LBS_UNDERFLOW      =  1,       // subtitle bitstream underflow
    LDEC_ERR_NO_FRAMEBUFFER     =  2,       // there is no free subtitle buffer for decoder
    LDEC_LBS_EMPTY              =  3,       // there is no subtitle data
    LDEC_ERR_IGNORE             =  4,       // some error which can be ignore
    LDEC_ERR_DEC_SUBTITLE       = -1,       // some error has happend when decode subtitle
    LDEC_ERR_LFMT_ERR           = -2,       // the format of the video is not supported, fatal error
    LDEC_ERR_
} __subtitle_dec_result;

typedef enum tag_ItxtDecCmd
{
    ITXTDEC_CMD_SET_FRMRATE = 0,    //����֡��, ��λΪ1000. aux = framerate
    ITXTDEC_CMD_SET_FRMSIZE,        //����frame size. aux = width << 16 | height
    ITXTDEC_CMD_SET_SCREEN_SIZE,    //����screen size.aux = width << 16 | height
    ITXTDEC_CMD_SET_SUBTITLE_SOURCE,    //����������Ļ����Դ,��������Ƶ�ļ��͹���. __subtitle_codec_format_t->subtitle_bs_src,
    //ITXTDEC_CMD_SET_POST_PROCESS,   //�����Ƿ�����ı���Ļ����
    ITXTDEC_CMD_NOTIFY_SYNCSTAT,     //����reset���,��lyricdec��decode()���ָñ��λʱ,����reset����buffer.Ȼ���ٽ���.
    ITXTDEC_CMD_CHECK_SYNCSTAT,    
    //ITXTDEC_CMD_SHOW_FREE_LBSBUF,   //�鿴ʣ���lbsbuffer�ĳ���, ret = left buflen

    //ITXTDEC_CMD_GETLRCITEM_LOCK = 0x20,   //�е�decoder��smi,ȡ��Ļ��ʱ��Ҫ����.
    //ITXTDEC_CMD_GETLRCITEM_UNLOCK,

    ITXTDEC_CMD_SET_ONESEGFLAG, //aux = 0:fullseg, 1:onesegISDB��ʽ�ĵ�����, service��Ϊfullseg��oneseg����,����service��������Ļ��Ȼ����aribJIS����,�������в�ͬ��Ĭ������.���Ա�������.
    ITXTDEC_CMD_SET_CHAR_CODING_TYPE,   //�����ַ������ʽ.aux = __CEDARLIB_LYRIC_ENCODE_TYPE(LYRIC_SUB_ENCODE_UTF8)
    ITXTDEC_CMD_,
}ItxtDecCmd;

typedef struct LDEC_MANAGR
{
    __u8   reconBufIdx; //��ʾ��ǰsubItem[]��һ�����еĽ����ڴ洢����֡��Ԫ�ص��±��,
	__u8   subIdxBuf[LDEC_MAX_TXT_FRM_NUM]; //��subItem[]���������
    __u8   subIsFull[LDEC_MAX_TXT_FRM_NUM]; //��subItem[]һһ��Ӧ, ��ʾ�Ƿ�װ��֡
    __s16  validSize;   //��ǰװ֡��subitem�ĸ���
    __u8   subReadIdx;  //��Ե���subIdxBuf[]
    __u8   subWriteIdx; //��Ե���subIdxBuf[]
    __cedar_subtitle_item_t subItem[LDEC_MAX_TXT_FRM_NUM];
}__ldec_manager_t;



/*******************************************************************************
                            ��Ա����˵��
EtxtDec_GetLrcItem:
  nType:CEDAR_GET_SUB_INF_ALL/CEDAR_GET_SUB_INF_ITEM, 
  ret:NULL:û�н�����;__cedar_buf_inf_t:���nType=CEDAR_GET_SUB_INF_ALL;__cedar_subtitle_item_t:���nType=CEDAR_GET_SUB_INF_ITEM;  

    
Parameters: 
    
Return: 
    
Time: 2011/8/3
*******************************************************************************/
typedef struct tag_ItxtDec ItxtDec;
typedef __s32   (*ItxtDec_IoCtrl)(ItxtDec *thiz, __s32 cmd, __s32 aux, void *pbuffer);  //cmd = ItxtDecCmd
//typedef __s32   (*ItxtDec_VirtualIoCtrl)(ItxtDec *thiz, __s32 cmd, __s32 aux, void *pbuffer);  //cmd = EtxtDecCmd
typedef __s32   (*ItxtDec_CheckSupport)(ItxtDec *thiz, __s32 nFileFmt);   //ret = EPDK_TRUE, EPDK_FALSE, nFileFmt = enum __CEDARLIB_LYRIC_FILE_FMT
typedef __s32   (*ItxtDec_GetLrcItem)(ItxtDec *thiz, __u32 nPts, __cedar_subtitle_item_t *pOutputItem, __s32 nOutputSubtitleSize);    //pOutputItem:app�ͽ���subtitle_item.ItxtDec�����д��Ȼ����ͷŽ���֡��. ret = EPDK_OK:�ҵ�������; EPDK_FAIL:û���ҵ�,nOutputSubtitleSize:outputItem��pSubtitle�ĳ���
typedef __s32   (*ItxtDec_Decode)(ItxtDec *thiz);   //��Ļ����
//usedBytes:����ʱ��ʾpLbs�ĳ���,��������ʱ��ʾ�������˶����ֽ�. lbsChgFlg:ԭ�����Ƿ���Ļ��,���ڸ�issaʹ��,��ʾ�Ƿ���ssa�ĵ�һ������Ҳ����ͷ��Ϣ
typedef __s32   (*ItxtDec_SubtitleDecode)(ItxtDec *thiz, __cedar_subtitle_item_t *psubitem, __u8 *pLbs, __u32 *usedBytes, __s8 lbsChgFlg);
typedef __s32   (*ItxtDec_Open)(ItxtDec *thiz);
typedef __s32   (*ItxtDec_Close)(ItxtDec *thiz);
typedef __s32   (*ItxtDec_RequestWrite)(ItxtDec *thiz, __u32 uSizeRequested, __u8 **ppData0, __u32 *puSizeGot0, __u8 **ppData1, __u32 *puSizeGot1);
typedef __s32   (*ItxtDec_UpdatePointer)(ItxtDec *thiz, __lbs_datctlblk_t * pLbsDataCtrlBlk,__u32 uDataSize);
typedef void    (*ItxtDec_Destruct)(ItxtDec *thiz);

typedef struct tag_ItxtDec
{
    //ʵ��Ա
    __s32               subtitle_bs_src;    //Ŀǰ�õ�����:CEDARLIB_FILE_FMT_MKV, CEDARLIB_FILE_FMT_VOB, CEDARLIB_FILE_FMT_TS, ��ʾ������Ļ��Դ
    __s32               subtitle_type;      // srt, sub, etc. enum __CEDARLIB_LYRIC_FILE_FMT
    __s32               frm_rate;           //video frame rate. be careful : ��ֵ�Ŵ���1000��!, for sub decode
    __s32               width;              //for sub decode
    __s32               height;             //for sub decode
    __s32               tvType;             //sub��Ļ�Ľ��룬��ҪtvSystemType, TV_SYSTEM_PAL
    __u8                lastColorCode[4];
    __u8                lastContrast[4];
    __s32               screenWidth;    //��ʾ���ڵĿ��
    __s32               screenHeight;

    //__s8                decFlag;            //����flag, ��ʾ�Ƿ����ڽ���,�𻥳����õ�.������
    __s8                useAnciBuf;     //�Ƿ�ʹ����ancibuf,�������ͷ.Isub��ʹ��,�������������������
    __s8                nSyncStatFlag;   //nResetLdecBufFlag,��ʾ���³�ʼ������buffer,׼�����½���
    
    __s8                strDecName[16];     //�洢��Ļ������������: srt, txt, smi, ssa, lrc
    CedarLock           LbsBufLock;         //��__ldec_lbs_buf_t    ldecLbsBuf����ʱ�Ļ�����
    CedarLock           FrmMgrLock;     //��__ldec_manager_t    ldecManager����ʱ�Ļ�����
    __ldec_lbs_buf_t    ldecLbsBuf;     //������
    __ldec_manager_t    ldecManager;    //������
    __u8                *anciDataBuf;   //������, ����ʱ����lbsbuf��ͷ�õ�.
    __u32               nLbsBufSize;    //ָʾldecLbsBuf��buffer�Ĵ�С,�ڳ�ʼ��ʱ�����า�Ǹ�ֵ.
    __u32               nLdecFrameSize;  //ָʾldecManager��frame�Ĵ�С,�ڳ�ʼ��ʱ�����า�Ǹ�ֵ. �ı���Ļ�����С, ͼ����Ļ��ܴ�.
    __u32               nanciDataBufSize;   //ָʾanciDataBuf�Ĵ�С,������һ��ֵ,���������Ҫ���Ը���.
    
    ItxtDec_IoCtrl          ItxtDecIoCtrl;  //������ṩʵ��,�������û���һЩ��Ա��ֵ
    ItxtDec_RequestWrite    RequestWrite;
    ItxtDec_UpdatePointer   UpdatePointer;
    ItxtDec_GetLrcItem      GetLrcItem;     //������ṩʵ��,���������Ҫ�������Լ��ĺ�������.
    ItxtDec_Open            open;           //������ṩʵ��,
    ItxtDec_Close           close;          //ͬ��
    ItxtDec_Decode          CommonDecode; //����ͨ��ʵ��, ���벿�ֵ����麯��SubtitleDecode���. isub�Ḳ��,ʹ���Լ���decode()����������.

    //���Ա
    ItxtDec_CheckSupport    CheckSupport;   //������NULL.����ļ������Ƿ�֧��, ret = EPDK_TRUE:֧��;
    ItxtDec_SubtitleDecode  SubtitleDecode; //���಻ʵ��, CommonDecode()�������.
    //ItxtDec_VirtualIoCtrl   EtxtDecVirtualIoCtrl;   //�еĺ�����ͬ����ֻ����Щ΢�Ķ�,���븲����д,��������ʵ��.

    //��������
    ItxtDec_Destruct        Destruct;   //�����ṩʵ��,һ��������Ϣ,������븲��
}ItxtDec;
extern __s32 ItxtDec_Init(ItxtDec *thiz, __s32 nFileFmt);   //LYRIC_ITXT��
extern __s32 ItxtDec_Exit(ItxtDec *thiz);
extern ItxtDec* ItxtDec_Create(__s32 nFileFmt);   //enum __CEDARLIB_LYRIC_FILE_FMT
extern void ItxtDec_Destroy(ItxtDec *thiz);


extern void ldec_itxt_init_info(ItxtDec *ldec_itxt);
//extern __u8 *ldec_process_chunk_cover_bufEnd(__ldec_lbs_buf_t *pLbsBufManager, __u8 **ppanciDataBuf, __u32 *pnanciDataBufSize);

#endif  /* _ITXT_DECODE_H_ */

