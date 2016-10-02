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
    isdb字幕解码类
********************************************************************************
*/
#ifndef _ISDB_DECODE_H_
#define _ISDB_DECODE_H_

#define ISDBDEC_MAX_LBSBUF_SIZE    (1024 * 32)  //lbsbufsize, 不同的字幕解码器,值不一样
#define ISDBDEC_MAX_FRM_SIZE       (1024)  //一帧的长度,对于itext来说,1024字节足够了,也可以表示字幕项的一行的buffer的大小
#define ISDBDEC_MAX_ANCI_SIZE      (1024)  //处理lbsbuffer回头的buffer,对于字符编码的字幕,和frame一样大就够了.
#define ISDBDEC_LBSBUF_SIZE        188
#define ISDBDEC_LBSBUF_HEADER      6

//#define ISDBDEC_MAX_LINE_NUM        10   //一个字幕项的最大行数

typedef struct tag_CLdecFrameManager
{
    __u8   reconBufIdx; //表示当前subItem[]的一个空闲的将用于存储解码帧的元素的下标号,
	__u8   subIdxBuf[LDEC_MAX_TXT_FRM_NUM]; //将subItem[]排序的数组
    __u8   subIsFull[LDEC_MAX_TXT_FRM_NUM]; //和subItem[]一一对应, 表示是否装了帧
    __s16  validSize;   //当前装帧的subitem的个数
    __u8   subReadIdx;  //针对的是subIdxBuf[]
    __u8   subWriteIdx; //针对的是subIdxBuf[]
    //__cedar_subtitle_item_t subItem[LDEC_MAX_TXT_FRM_NUM];
    CCedarSubtitleFrame subItem[LDEC_MAX_TXT_FRM_NUM];
}CLdecFrameManager;

/*******************************************************************************
                            成员函数说明
EtxtDec_GetLrcItem:
  nType:CEDAR_GET_SUB_INF_ALL/CEDAR_GET_SUB_INF_ITEM, 
  ret:NULL:没有解析完;__cedar_buf_inf_t:如果nType=CEDAR_GET_SUB_INF_ALL;__cedar_subtitle_item_t:如果nType=CEDAR_GET_SUB_INF_ITEM;  

    
Parameters: 
    
Return: 
    
Time: 2011/8/3
*******************************************************************************/
typedef struct tag_IsdbDec IsdbDec;
typedef __s32   (*IsdbDec_IoCtrl)(IsdbDec *thiz, __s32 cmd, __s32 aux, void *pbuffer);  //cmd = ItxtDecCmd
//typedef __s32   (*ItxtDec_VirtualIoCtrl)(ItxtDec *thiz, __s32 cmd, __s32 aux, void *pbuffer);  //cmd = EtxtDecCmd
typedef __s32   (*IsdbDec_CheckSupport)(IsdbDec *thiz, __s32 nFileFmt);   //ret = EPDK_TRUE, EPDK_FALSE, nFileFmt = enum __CEDARLIB_LYRIC_FILE_FMT
typedef __s32   (*IsdbDec_GetLrcItem)(IsdbDec *thiz, __u32 nPts, CCedarSubtitleFrame *pOutputItem, __s32 nOutputSubtitleSize);    //pOutputItem:app送进的subtitle_item.ItxtDec深拷贝填写，然后就释放解码帧了. ret = EPDK_OK:找到并拷贝; EPDK_FAIL:没有找到,nOutputSubtitleSize:outputItem的pSubtitle的长度
typedef __s32   (*IsdbDec_Decode)(IsdbDec *thiz);   //字幕解码
//usedBytes:送入时表示pLbs的长度,函数返回时表示解码用了多少字节. lbsChgFlg:原义是是否换字幕了,现在给issa使用,表示是否是ssa的第一笔数据也就是头信息
typedef __s32   (*IsdbDec_SubtitleDecode)(IsdbDec *thiz, CCedarSubtitleFrame *psubitem, __u8 *pLbs, __u32 *usedBytes, __s8 lbsChgFlg, __u8 *privateData);
typedef __s32   (*IsdbDec_Open)(IsdbDec *thiz);
typedef __s32   (*IsdbDec_Close)(IsdbDec *thiz);
typedef __s32   (*IsdbDec_RequestWrite)(IsdbDec *thiz, __u32 uSizeRequested, __u8 **ppData0, __u32 *puSizeGot0, __u8 **ppData1, __u32 *puSizeGot1);
typedef __s32   (*IsdbDec_UpdatePointer)(IsdbDec *thiz, __lbs_datctlblk_t * pLbsDataCtrlBlk,__u32 uDataSize);
typedef void    (*IsdbDec_Destruct)(IsdbDec *thiz);

typedef struct tag_IsdbDec
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
    __ldec_lbs_buf_t    ldecLbsBuf;     //解码类
    CLdecFrameManager    ldecManager;    //解码类
    __u8                *anciDataBuf;   //解码类, 解码时处理lbsbuf回头用的.
    __u32               nLbsBufSize;    //指示ldecLbsBuf的buffer的大小,在初始化时由子类覆盖赋值.
    __u32               nLdecFrameSize;  //指示ldecManager的frame的大小,在初始化时由子类覆盖赋值. 文本字幕都会很小, 图像字幕会很大.
    __u32               nanciDataBufSize;   //指示anciDataBuf的大小,基类会给一个值,子类根据需要可以覆盖.
    
    IsdbDec_IoCtrl          IsdbDecIoCtrl;  //基类会提供实现,用于设置基类一些成员的值
    IsdbDec_RequestWrite    RequestWrite;
    IsdbDec_UpdatePointer   UpdatePointer;
    IsdbDec_GetLrcItem      GetLrcItem;     //基类会提供实现,子类根据需要可以用自己的函数覆盖.
    IsdbDec_Open            open;           //基类会提供实现,
    IsdbDec_Close           close;          //同上
    IsdbDec_Decode          CommonDecode; //基类通用实现, 解码部分调用虚函数SubtitleDecode完成. isub会覆盖,使用自己的decode()函数来接吗.

    //虚成员
    IsdbDec_CheckSupport    CheckSupport;   //基类置NULL.检查文件类型是否支持, ret = EPDK_TRUE:支持;
    IsdbDec_SubtitleDecode  SubtitleDecode; //基类不实现, CommonDecode()会调用它.
    //ItxtDec_VirtualIoCtrl   EtxtDecVirtualIoCtrl;   //有的函数不同子类只是有些微改动,不想覆盖重写,故用它来实现.

    //析构函数
    IsdbDec_Destruct        Destruct;   //基类提供实现,一个警告信息,子类必须覆盖
    
}IsdbDec;
extern __s32 IsdbDec_Init(IsdbDec *thiz);
extern __s32 IsdbDec_Exit(IsdbDec *thiz);
extern IsdbDec *newIsdbDec(void);
extern void deleteIsdbDec(IsdbDec *thiz);


extern void ldec_isdb_init_info(IsdbDec *ldec_itxt);
//extern __u8 *ldec_process_chunk_cover_bufEnd(__ldec_lbs_buf_t *pLbsBufManager, __u8 **ppanciDataBuf, __u32 *pnanciDataBufSize);


#endif  /* _ISDB_DECODE_H_ */

