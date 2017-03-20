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
* File   : CedarITxtDec.h
* Version: V1.0
* By     : Eric_wang
* Date   : 2011-10-16
* Description:
    ������Ļ�����ܻ��ڸû��࿪��.
    ����ֵ����:__ldec_return_val_t, LDEC_ERROR_OK��
********************************************************************************
*/
#ifndef _CEDARISUBTITLEDEC_H_
#define _CEDARISUBTITLEDEC_H_

#define strDecNameSize  (16)

typedef enum tag_CedarIsubtitleDecCmd
{
    CEDARISUBTITLEDEC_CMD_SET_FRMRATE = 0,    //����֡��, ��λΪ1000. aux = framerate
    CEDARISUBTITLEDEC_CMD_SET_FRMSIZE,        //����frame size. aux = width << 16 | height
    CEDARISUBTITLEDEC_CMD_SET_SCREEN_SIZE,    //����screen size.aux = width << 16 | height
    CEDARISUBTITLEDEC_CMD_SET_SUBTITLE_SOURCE,    //����������Ļ����Դ,��������Ƶ�ļ��͹���. __subtitle_codec_format_t->subtitle_bs_src,
    //ITXTDEC_CMD_SET_POST_PROCESS,   //�����Ƿ�����ı���Ļ����
    CEDARISUBTITLEDEC_CMD_NOTIFY_SYNCSTAT,     //����reset���,��lyricdec��decode()���ָñ��λʱ,����reset����buffer.Ȼ���ٽ���.
    CEDARISUBTITLEDEC_CMD_CHECK_SYNCSTAT,       //�鿴syncstat��״̬�Ƿ����. ret = 0:���, ret = 1:��Ȼ��syncstat
    //ITXTDEC_CMD_SHOW_FREE_LBSBUF,   //�鿴ʣ���lbsbuffer�ĳ���, ret = left buflen

    //ITXTDEC_CMD_GETLRCITEM_LOCK = 0x20,   //�е�decoder��smi,ȡ��Ļ��ʱ��Ҫ����.
    //ITXTDEC_CMD_GETLRCITEM_UNLOCK,

    CEDARISUBTITLEDEC_CMD_SET_ONESEGFLAG,   //aux = 0:fullseg, 1:onesegISDB��ʽ�ĵ�����, service��Ϊfullseg��oneseg����,����service��������Ļ��Ȼ����aribJIS����,�������в�ͬ��Ĭ������.���Ա�������.
    CEDARISUBTITLEDEC_CMD_SET_CHAR_CODING_TYPE, //�����ı���Ļ���ַ������ʽ. aux = __CEDARLIB_LYRIC_ENCODE_TYPE(LYRIC_SUB_ENCODE_UTF8)
    
    CEDARISUBTITLEDEC_CMD_,
}CedarIsubtitleDecCmd;

/*******************************************************************************
                            ��Ա����˵��
EtxtDec_GetLrcItem:
  nType:CEDAR_GET_SUB_INF_ALL/CEDAR_GET_SUB_INF_ITEM, 
  ret:NULL:û�н�����;__cedar_buf_inf_t:���nType=CEDAR_GET_SUB_INF_ALL;__cedar_subtitle_item_t:���nType=CEDAR_GET_SUB_INF_ITEM;  

    
Parameters: 
    
Return: 
    
Time: 2011/8/3
*******************************************************************************/
typedef struct tag_CedarIsubtitleDec CedarIsubtitleDec;
typedef __s32   (*CedarIsubtitleDec_IoCtrl)(CedarIsubtitleDec *thiz, __s32 cmd, __s32 aux, void *pbuffer);  //cmd = CedarIsubtitleDecCmd
typedef __s32   (*CedarIsubtitleDec_CheckSupport)(CedarIsubtitleDec *thiz, __s32 nFileFmt);   //ret = EPDK_TRUE, EPDK_FALSE, nFileFmt = enum __CEDARLIB_LYRIC_FILE_FMT
typedef __cedar_subtitle_item_t* (*CedarIsubtitleDec_GetLrcItem)(CedarIsubtitleDec *thiz, __u32 nPts, __s32 *pRedecodeFlag);    //ret = pOutputItem, NULL��ʾû���ҵ�, *pRedecodeFlag��ʾ�Ƿ���Ҫ���½���
typedef __s32   (*CedarIsubtitleDec_Decode)(CedarIsubtitleDec *thiz);   //��Ļ����
//usedBytes:����ʱ��ʾpLbs�ĳ���,��������ʱ��ʾ�������˶����ֽ�. lbsChgFlg:ԭ�����Ƿ���Ļ��,���ڸ�issaʹ��,��ʾ�Ƿ���ssa�ĵ�һ������Ҳ����ͷ��Ϣ
typedef __s32   (*CedarIsubtitleDec_Open)(CedarIsubtitleDec *thiz);
typedef __s32   (*CedarIsubtitleDec_Close)(CedarIsubtitleDec *thiz);
typedef __s32   (*CedarIsubtitleDec_RequestWrite)(CedarIsubtitleDec *thiz, __u32 uSizeRequested, __u8 **ppData0, __u32 *puSizeGot0, __u8 **ppData1, __u32 *puSizeGot1);
typedef __s32   (*CedarIsubtitleDec_UpdatePointer)(CedarIsubtitleDec *thiz, __lbs_datctlblk_t * pLbsDataCtrlBlk,__u32 uDataSize);
typedef void    (*CedarIsubtitleDec_Destruct)(CedarIsubtitleDec *thiz);
typedef CCedarSubtitleFrame* (*CedarIsubtitleDec_GetSubtitleFrame)(CedarIsubtitleDec *thiz, __u32 nPts, __s32 *pRedecodeFlag);    //ret = pOutputItem, NULL��ʾû���ҵ�, *pRedecodeFlag��ʾ�Ƿ���Ҫ���½���

typedef struct tag_CedarIsubtitleDec
{
    //ʵ��Ա
    __s32               subtitle_bs_src;    //Ŀǰ�õ�����:CEDARLIB_FILE_FMT_MKV, CEDARLIB_FILE_FMT_VOB, CEDARLIB_FILE_FMT_TS, ��ʾ������Ļ��Դ
    __s32               subtitle_type;      // srt, sub, etc. enum __CEDARLIB_LYRIC_FILE_FMT,
    __s8                strDecName[strDecNameSize];     //�洢��Ļ������������: srt, txt, smi, ssa, lrc, ���ำֵ
    
    //���Ա,ȫ����Ϊ�麯��
    CedarIsubtitleDec_Open            open;           //
    CedarIsubtitleDec_Close           close;          //

    CedarIsubtitleDec_IoCtrl          CedarISubtitleDecIoCtrl;  //���಻ʵ��, cmd = CedarIsubtitleDecCmd
    CedarIsubtitleDec_RequestWrite    RequestWrite;             //���಻ʵ��
    CedarIsubtitleDec_UpdatePointer   UpdatePointer;            //���಻ʵ��.ret = LDEC_ERROR_OK��
    CedarIsubtitleDec_GetLrcItem      GetLrcItem;               //���಻���ṩʵ��,���������Ҫ�������Լ��ĺ�������.�Ϸ�ʽ��Ҫ֡,���ݽṹ__cedar_subtitle_item_t*
    CedarIsubtitleDec_Decode          decode;                   //���಻�ṩͨ��ʵ��
    CedarIsubtitleDec_CheckSupport    CheckSupport;             //���಻ʵ��.������NULL.����ļ������Ƿ�֧��, ret = EPDK_TRUE:֧��;
    CedarIsubtitleDec_GetSubtitleFrame  GetSubtitleFrame;       //���಻ʵ��.Ҫ֡�ĵڶ��ֽӿ�,����CCedarSubtitleFrame.��SubtitlePlybkģ�����.

    //��������
    CedarIsubtitleDec_Destruct        Destruct;   //�����ṩʵ��,һ��������Ϣ,������븲��
}CedarIsubtitleDec;

extern __s32 CedarIsubtitleDec_Init(CedarIsubtitleDec *thiz, __s32 nFileFmt);   //LYRIC_ITXT��
extern __s32 CedarIsubtitleDec_Exit(CedarIsubtitleDec *thiz);
extern CedarIsubtitleDec* CedarIsubtitleDec_Create(__s32 nFileFmt);   //enum __CEDARLIB_LYRIC_FILE_FMT
extern void CedarIsubtitleDec_Destroy(CedarIsubtitleDec *thiz);

extern __s8* Isubtitle_strcpy(__s8* pdest, __s32 nDestSize, const __s8* psrc);

#endif  /* _CEDARISUBTITLEDEC_H_ */

