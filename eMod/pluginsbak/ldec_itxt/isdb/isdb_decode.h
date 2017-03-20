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
    isdb��Ļ������
********************************************************************************
*/
#ifndef _ISDB_DECODE_H_
#define _ISDB_DECODE_H_

#define ISDBDEC_MAX_LBSBUF_SIZE    (1024 * 32)  //lbsbufsize, ��ͬ����Ļ������,ֵ��һ��
#define ISDBDEC_MAX_FRM_SIZE       (1024)  //һ֡�ĳ���,����itext��˵,1024�ֽ��㹻��,Ҳ���Ա�ʾ��Ļ���һ�е�buffer�Ĵ�С
#define ISDBDEC_MAX_ANCI_SIZE      (1024)  //����lbsbuffer��ͷ��buffer,�����ַ��������Ļ,��frameһ����͹���.
#define ISDBDEC_LBSBUF_SIZE        188
#define ISDBDEC_LBSBUF_HEADER      6

//#define ISDBDEC_MAX_LINE_NUM        10   //һ����Ļ����������

typedef struct tag_CLdecFrameManager
{
    __u8   reconBufIdx; //��ʾ��ǰsubItem[]��һ�����еĽ����ڴ洢����֡��Ԫ�ص��±��,
	__u8   subIdxBuf[LDEC_MAX_TXT_FRM_NUM]; //��subItem[]���������
    __u8   subIsFull[LDEC_MAX_TXT_FRM_NUM]; //��subItem[]һһ��Ӧ, ��ʾ�Ƿ�װ��֡
    __s16  validSize;   //��ǰװ֡��subitem�ĸ���
    __u8   subReadIdx;  //��Ե���subIdxBuf[]
    __u8   subWriteIdx; //��Ե���subIdxBuf[]
    //__cedar_subtitle_item_t subItem[LDEC_MAX_TXT_FRM_NUM];
    CCedarSubtitleFrame subItem[LDEC_MAX_TXT_FRM_NUM];
}CLdecFrameManager;

/*******************************************************************************
                            ��Ա����˵��
EtxtDec_GetLrcItem:
  nType:CEDAR_GET_SUB_INF_ALL/CEDAR_GET_SUB_INF_ITEM, 
  ret:NULL:û�н�����;__cedar_buf_inf_t:���nType=CEDAR_GET_SUB_INF_ALL;__cedar_subtitle_item_t:���nType=CEDAR_GET_SUB_INF_ITEM;  

    
Parameters: 
    
Return: 
    
Time: 2011/8/3
*******************************************************************************/
typedef struct tag_IsdbDec IsdbDec;
typedef __s32   (*IsdbDec_IoCtrl)(IsdbDec *thiz, __s32 cmd, __s32 aux, void *pbuffer);  //cmd = ItxtDecCmd
//typedef __s32   (*ItxtDec_VirtualIoCtrl)(ItxtDec *thiz, __s32 cmd, __s32 aux, void *pbuffer);  //cmd = EtxtDecCmd
typedef __s32   (*IsdbDec_CheckSupport)(IsdbDec *thiz, __s32 nFileFmt);   //ret = EPDK_TRUE, EPDK_FALSE, nFileFmt = enum __CEDARLIB_LYRIC_FILE_FMT
typedef __s32   (*IsdbDec_GetLrcItem)(IsdbDec *thiz, __u32 nPts, CCedarSubtitleFrame *pOutputItem, __s32 nOutputSubtitleSize);    //pOutputItem:app�ͽ���subtitle_item.ItxtDec�����д��Ȼ����ͷŽ���֡��. ret = EPDK_OK:�ҵ�������; EPDK_FAIL:û���ҵ�,nOutputSubtitleSize:outputItem��pSubtitle�ĳ���
typedef __s32   (*IsdbDec_Decode)(IsdbDec *thiz);   //��Ļ����
//usedBytes:����ʱ��ʾpLbs�ĳ���,��������ʱ��ʾ�������˶����ֽ�. lbsChgFlg:ԭ�����Ƿ���Ļ��,���ڸ�issaʹ��,��ʾ�Ƿ���ssa�ĵ�һ������Ҳ����ͷ��Ϣ
typedef __s32   (*IsdbDec_SubtitleDecode)(IsdbDec *thiz, CCedarSubtitleFrame *psubitem, __u8 *pLbs, __u32 *usedBytes, __s8 lbsChgFlg, __u8 *privateData);
typedef __s32   (*IsdbDec_Open)(IsdbDec *thiz);
typedef __s32   (*IsdbDec_Close)(IsdbDec *thiz);
typedef __s32   (*IsdbDec_RequestWrite)(IsdbDec *thiz, __u32 uSizeRequested, __u8 **ppData0, __u32 *puSizeGot0, __u8 **ppData1, __u32 *puSizeGot1);
typedef __s32   (*IsdbDec_UpdatePointer)(IsdbDec *thiz, __lbs_datctlblk_t * pLbsDataCtrlBlk,__u32 uDataSize);
typedef void    (*IsdbDec_Destruct)(IsdbDec *thiz);

typedef struct tag_IsdbDec
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
    __ldec_lbs_buf_t    ldecLbsBuf;     //������
    CLdecFrameManager    ldecManager;    //������
    __u8                *anciDataBuf;   //������, ����ʱ����lbsbuf��ͷ�õ�.
    __u32               nLbsBufSize;    //ָʾldecLbsBuf��buffer�Ĵ�С,�ڳ�ʼ��ʱ�����า�Ǹ�ֵ.
    __u32               nLdecFrameSize;  //ָʾldecManager��frame�Ĵ�С,�ڳ�ʼ��ʱ�����า�Ǹ�ֵ. �ı���Ļ�����С, ͼ����Ļ��ܴ�.
    __u32               nanciDataBufSize;   //ָʾanciDataBuf�Ĵ�С,������һ��ֵ,���������Ҫ���Ը���.
    
    IsdbDec_IoCtrl          IsdbDecIoCtrl;  //������ṩʵ��,�������û���һЩ��Ա��ֵ
    IsdbDec_RequestWrite    RequestWrite;
    IsdbDec_UpdatePointer   UpdatePointer;
    IsdbDec_GetLrcItem      GetLrcItem;     //������ṩʵ��,���������Ҫ�������Լ��ĺ�������.
    IsdbDec_Open            open;           //������ṩʵ��,
    IsdbDec_Close           close;          //ͬ��
    IsdbDec_Decode          CommonDecode; //����ͨ��ʵ��, ���벿�ֵ����麯��SubtitleDecode���. isub�Ḳ��,ʹ���Լ���decode()����������.

    //���Ա
    IsdbDec_CheckSupport    CheckSupport;   //������NULL.����ļ������Ƿ�֧��, ret = EPDK_TRUE:֧��;
    IsdbDec_SubtitleDecode  SubtitleDecode; //���಻ʵ��, CommonDecode()�������.
    //ItxtDec_VirtualIoCtrl   EtxtDecVirtualIoCtrl;   //�еĺ�����ͬ����ֻ����Щ΢�Ķ�,���븲����д,��������ʵ��.

    //��������
    IsdbDec_Destruct        Destruct;   //�����ṩʵ��,һ��������Ϣ,������븲��
    
}IsdbDec;
extern __s32 IsdbDec_Init(IsdbDec *thiz);
extern __s32 IsdbDec_Exit(IsdbDec *thiz);
extern IsdbDec *newIsdbDec(void);
extern void deleteIsdbDec(IsdbDec *thiz);


extern void ldec_isdb_init_info(IsdbDec *ldec_itxt);
//extern __u8 *ldec_process_chunk_cover_bufEnd(__ldec_lbs_buf_t *pLbsBufManager, __u8 **ppanciDataBuf, __u32 *pnanciDataBufSize);


#endif  /* _ISDB_DECODE_H_ */

