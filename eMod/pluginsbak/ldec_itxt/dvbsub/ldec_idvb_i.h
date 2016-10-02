
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

//需要的外部头文件
#include "ldec_itxt_s.h"


#define DVBDEC_MAX_LBSBUF_SIZE    (1024 * 128)  //lbsbufsize, 不同的字幕解码器,值不一样
#define DVBDEC_MAX_FRM_SIZE       (1024 * 32)  //一帧的长度,对于itext来说,1024字节足够了,也可以表示字幕项的一行的buffer的大小
#define DVBDEC_MAX_ANCI_SIZE      (1024 * 6)  //处理lbsbuffer回头的buffer,对于字符编码的字幕,和frame一样大就够了.

#define LDEC_MAX_LBS_BUF_SIZE     (1024 * 128)
#define LDEC_MAX_LBS_SUB_SIZE     (1024 * 128)  //lbsbufsize, 不同的字幕解码器,值不一样,Isdb-T是32/64 ?
#define LDEC_MAX_LBS_ANCI_SIZE    (1024 * 6)     //防止解码时数据回头
#define LDEC_MAX_FRM_SIZE     (1024)        //一帧的长度,对于itext来说,1024字节足够了,也可以表示字幕项的一行的buffer的大小

#define LDEC_MAX_LBS_SUB_NUM    (8)  //Isdb-T是4
#define LDEC_MAX_LBS_DATA_NUM   (64) //lbs分块的个数,同LDEC_MAX_TXT_DATA_NUM
#define DVBSUBDEC_MAX_LINE_NUM    4   //一个字幕项的最大行数

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
	__s32                   subDataLen;//字幕的一行buffer里存储的有效数据长度，不包括\0.不能超过subData[]的buffer的长度.
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
    DVBSUBDEC_CMD_SET_SUBTITLE_SOURCE,    //设置字幕来源,一般是ts,也有可能是mkv, CEDARLIB_FILE_FMT_TS等
    DVBSUBDEC_CMD_CHECK_DSP_TIME,      //* get the presentation time of next subtitle frame; ret = -1表示没有下一帧,0表示找到下一帧, pbuffer =__u32*;
    DVBSUBDEC_CMD_NOTIFY_SYNCSTAT,    //通知pgsdec准备进行状态同步,其实就是在decode()里清buffer.
    DVBSUBDEC_CMD_CHECK_SYNCSTAT,     //查询是否状态同步完成了. ret = 0:完成, ret = 1:仍然在syncstat
    DVBSUBDEC_CMD_SET_CHAR_CODING_TYPE,//设置字符编码格式.
    DVBSUBDEC_CMD_,
}DvbsubDecCmd;



/*******************************************************************************
                            成员函数说明
EtxtDec_GetLrcItem:
  nType:CEDAR_GET_SUB_INF_ALL/CEDAR_GET_SUB_INF_ITEM, 
  ret:NULL:没有解析完;__cedar_buf_inf_t:如果nType=CEDAR_GET_SUB_INF_ALL;__cedar_subtitle_item_t:如果nType=CEDAR_GET_SUB_INF_ITEM;  

    
Parameters: 
    
Return: 
    
Time: 2011/8/3
*******************************************************************************/
typedef struct __SUBTITLE_DEC __ldec_idvbsub_t;
typedef __s32   (*DvbsubDec_IoCtrl)(__ldec_idvbsub_t *thiz, __s32 cmd, __s32 aux, void *pbuffer);  //cmd = ItxtDecCmd
//typedef __s32   (*ItxtDec_VirtualIoCtrl)(ItxtDec *thiz, __s32 cmd, __s32 aux, void *pbuffer);  //cmd = EtxtDecCmd
typedef __s32   (*DvbsubDec_CheckSupport)(__ldec_idvbsub_t *thiz, __s32 nFileFmt);   //ret = EPDK_TRUE, EPDK_FALSE, nFileFmt = enum __CEDARLIB_LYRIC_FILE_FMT
typedef __s32   (*DvbsubDec_GetLrcItem)(__ldec_idvbsub_t *thiz, __u32 nPts, CCedarSubtitleFrame *pOutputItem, __s32 nOutputSubtitleSize);    //pOutputItem:app送进的subtitle_item.ItxtDec深拷贝填写，然后就释放解码帧了. ret = EPDK_OK:找到并拷贝; EPDK_FAIL:没有找到,nOutputSubtitleSize:outputItem的pSubtitle的长度
typedef __s32   (*DvbsubDec_Decode)(__ldec_idvbsub_t *thiz);   //字幕解码
//usedBytes:送入时表示pLbs的长度,函数返回时表示解码用了多少字节. lbsChgFlg:原义是是否换字幕了,现在给issa使用,表示是否是ssa的第一笔数据也就是头信息
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
    //实成员
    __s32               subtitle_bs_src;    //目前用到的有:CEDARLIB_FILE_FMT_MKV, CEDARLIB_FILE_FMT_VOB, CEDARLIB_FILE_FMT_TS, 表示内置字幕来源
    __s32               subtitle_type;      // srt, sub, etc. enum __CEDARLIB_LYRIC_FILE_FMT
    //__s32               frm_rate;           //video frame rate. be careful : 数值放大了1000倍!, for sub decode
    //__s32               width;              //for sub decode
    //__s32               height;             //for sub decode
    //__s32               tvType;             //sub字幕的解码，需要tvSystemType, TV_SYSTEM_PAL
    //__u8                lastColorCode[4];
    //__u8                lastContrast[4];
    //__s32               screenWidth;    //显示窗口的宽高
    //__s32               screenHeight;

    __s32               nOneSegFlag;       //0:fullseg, 1:oneseg
    enum __CEDARLIB_LYRIC_ENCODE_TYPE   nCharEncodeType;

    //__s8                decFlag;            //解码flag, 表示是否正在解码,起互斥作用的.不用了
    //__s8                useAnciBuf;     //是否使用了ancibuf,来处理回头.Isub会使用,其他基本不用这个变量
    __s8                nSyncStatFlag;   //nResetLdecBufFlag,表示重新初始化所有buffer,准备重新解码
    
    __s8                strDecName[16];     //存储字幕解码器的名字: srt, txt, smi, ssa, lrc
    CedarLock           LbsBufLock;         //对__ldec_lbs_buf_t    ldecLbsBuf操作时的互斥锁
    CedarLock           FrmMgrLock;     //对__ldec_manager_t    ldecManager操作时的互斥锁
		
    __u32               nLbsBufSize;    //指示ldecLbsBuf的buffer的大小,在初始化时由子类覆盖赋值.
    __u32               nLdecFrameSize;  //指示ldecManager的frame的大小,在初始化时由子类覆盖赋值. 文本字幕都会很小, 图像字幕会很大.
    __u32               nanciDataBufSize;   //指示anciDataBuf的大小,基类会给一个值,子类根据需要可以覆盖.
   
//=================== DVB-T Demo Data =========================
     void                *privInfo;
    __u8                 subIdxBuf[LDEC_MAX_LBS_SUB_SIZE];
    __ldec_lbs_buf_t     ldecLbsBuf;
    __Ldec_manager_t     ldecManager;
    __ldec_display_t     ldecDisplay;
//=============================================================

    DvbsubDec_IoCtrl          DvbsubDecIoCtrl;  //基类会提供实现,用于设置基类一些成员的值
    DvbsubDec_RequestWrite    RequestWrite;
    DvbsubDec_UpdatePointer   UpdatePointer;
    DvbsubDec_GetLrcItem      GetLrcItem;     //基类会提供实现,子类根据需要可以用自己的函数覆盖.
    DvbsubDec_Open            open;           //基类会提供实现,
    DvbsubDec_Close           close;          //同上
    DvbsubDec_Decode          CommonDecode; //基类通用实现, 解码部分调用虚函数SubtitleDecode完成. isub会覆盖,使用自己的decode()函数来接吗.

    //虚成员
    DvbsubDec_CheckSupport    CheckSupport;   //基类置NULL.检查文件类型是否支持, ret = EPDK_TRUE:支持;
    DvbsubDec_SubtitleDecode  SubtitleDecode; //基类不实现, CommonDecode()会调用它.
    //ItxtDec_VirtualIoCtrl   EtxtDecVirtualIoCtrl;   //有的函数不同子类只是有些微改动,不想覆盖重写,故用它来实现.

    //析构函数
    DvbsubDec_Destruct        Destruct;   //基类提供实现,一个警告信息,子类必须覆盖
}__ldec_idvbsub_t;
extern __s32 DvbsubDec_Init(__ldec_idvbsub_t *thiz);
extern __s32 DvbsubDec_Exit(__ldec_idvbsub_t *thiz);
extern __ldec_idvbsub_t *newDvbsubDec(void);
extern void deleteDvbsubDec(__ldec_idvbsub_t *thiz);
extern __s32 DvbsubDec_RequirePts(__ldec_idvbsub_t* thiz, __u32* pPts);

extern void ldec_dvbsub_init_info(__ldec_idvbsub_t *ldec_itxt);
//extern __u8 *ldec_process_chunk_cover_bufEnd(__ldec_lbs_buf_t *pLbsBufManager, __u8 **ppanciDataBuf, __u32 *pnanciDataBufSize);

#endif

