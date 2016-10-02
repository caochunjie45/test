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
    内置字幕解码基类,主要抽取了itext, issa, isub共同部分做的基类
********************************************************************************
*/
#ifndef _ITXT_DECODE_H_
#define _ITXT_DECODE_H_

#define TV_SYSTEM_PAL       0
#define TV_SYSTEM_NTSC      1

#define TOP_FIELD_FLAG      0
#define BOTTOM_FIELD_FLAG   1

//以下这些变量, 都只是做参考而已,不同的内置字幕解码器自行设定的参数可以不一样.
#define LDEC_MAX_TXT_BUF_SIZE     (1024 * 128)  //lbsbufsize, 不同的字幕解码器,值不一样
#define LDEC_MAX_TXT_DATA_SIZE    (1024 * 512)  //sub解码会用到
#define LDEC_MAX_TXT_FRM_SIZE     (1024 * 256)   //一帧的长度,不同的字幕解码器,值不一样. ldec_itxt->outputItem.pSubtitle按此标准malloc内存. 这是以sub为标准,文本类型不会malloc这么大的.
#define LDEC_MAX_TXT_ANCI_SIZE    (1024 * 6)     //防止解码时数据回头,

#define LDEC_MAX_TXT_FRM_NUM       (4)  //frame的个数, isdb原来是32,也可以改为4.
//#define LDEC_MAX_TXT_DATA_NUM      (64) //lbs分块的个数
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
    ITXTDEC_CMD_SET_FRMRATE = 0,    //设置帧率, 单位为1000. aux = framerate
    ITXTDEC_CMD_SET_FRMSIZE,        //设置frame size. aux = width << 16 | height
    ITXTDEC_CMD_SET_SCREEN_SIZE,    //设置screen size.aux = width << 16 | height
    ITXTDEC_CMD_SET_SUBTITLE_SOURCE,    //设置内置字幕的来源,从哪种视频文件送过来. __subtitle_codec_format_t->subtitle_bs_src,
    //ITXTDEC_CMD_SET_POST_PROCESS,   //设置是否进行文本字幕后处理
    ITXTDEC_CMD_NOTIFY_SYNCSTAT,     //设置reset标记,当lyricdec的decode()发现该标记位时,会先reset所有buffer.然后再解码.
    ITXTDEC_CMD_CHECK_SYNCSTAT,    
    //ITXTDEC_CMD_SHOW_FREE_LBSBUF,   //查看剩余的lbsbuffer的长度, ret = left buflen

    //ITXTDEC_CMD_GETLRCITEM_LOCK = 0x20,   //有的decoder如smi,取字幕项时需要互斥.
    //ITXTDEC_CMD_GETLRCITEM_UNLOCK,

    ITXTDEC_CMD_SET_ONESEGFLAG, //aux = 0:fullseg, 1:onesegISDB制式的电视流, service分为fullseg和oneseg两种,两种service包含的字幕虽然都是aribJIS编码,但还是有不同的默认设置.所以必须区分.
    ITXTDEC_CMD_SET_CHAR_CODING_TYPE,   //设置字符编码格式.aux = __CEDARLIB_LYRIC_ENCODE_TYPE(LYRIC_SUB_ENCODE_UTF8)
    ITXTDEC_CMD_,
}ItxtDecCmd;

typedef struct LDEC_MANAGR
{
    __u8   reconBufIdx; //表示当前subItem[]的一个空闲的将用于存储解码帧的元素的下标号,
	__u8   subIdxBuf[LDEC_MAX_TXT_FRM_NUM]; //将subItem[]排序的数组
    __u8   subIsFull[LDEC_MAX_TXT_FRM_NUM]; //和subItem[]一一对应, 表示是否装了帧
    __s16  validSize;   //当前装帧的subitem的个数
    __u8   subReadIdx;  //针对的是subIdxBuf[]
    __u8   subWriteIdx; //针对的是subIdxBuf[]
    __cedar_subtitle_item_t subItem[LDEC_MAX_TXT_FRM_NUM];
}__ldec_manager_t;



/*******************************************************************************
                            成员函数说明
EtxtDec_GetLrcItem:
  nType:CEDAR_GET_SUB_INF_ALL/CEDAR_GET_SUB_INF_ITEM, 
  ret:NULL:没有解析完;__cedar_buf_inf_t:如果nType=CEDAR_GET_SUB_INF_ALL;__cedar_subtitle_item_t:如果nType=CEDAR_GET_SUB_INF_ITEM;  

    
Parameters: 
    
Return: 
    
Time: 2011/8/3
*******************************************************************************/
typedef struct tag_ItxtDec ItxtDec;
typedef __s32   (*ItxtDec_IoCtrl)(ItxtDec *thiz, __s32 cmd, __s32 aux, void *pbuffer);  //cmd = ItxtDecCmd
//typedef __s32   (*ItxtDec_VirtualIoCtrl)(ItxtDec *thiz, __s32 cmd, __s32 aux, void *pbuffer);  //cmd = EtxtDecCmd
typedef __s32   (*ItxtDec_CheckSupport)(ItxtDec *thiz, __s32 nFileFmt);   //ret = EPDK_TRUE, EPDK_FALSE, nFileFmt = enum __CEDARLIB_LYRIC_FILE_FMT
typedef __s32   (*ItxtDec_GetLrcItem)(ItxtDec *thiz, __u32 nPts, __cedar_subtitle_item_t *pOutputItem, __s32 nOutputSubtitleSize);    //pOutputItem:app送进的subtitle_item.ItxtDec深拷贝填写，然后就释放解码帧了. ret = EPDK_OK:找到并拷贝; EPDK_FAIL:没有找到,nOutputSubtitleSize:outputItem的pSubtitle的长度
typedef __s32   (*ItxtDec_Decode)(ItxtDec *thiz);   //字幕解码
//usedBytes:送入时表示pLbs的长度,函数返回时表示解码用了多少字节. lbsChgFlg:原义是是否换字幕了,现在给issa使用,表示是否是ssa的第一笔数据也就是头信息
typedef __s32   (*ItxtDec_SubtitleDecode)(ItxtDec *thiz, __cedar_subtitle_item_t *psubitem, __u8 *pLbs, __u32 *usedBytes, __s8 lbsChgFlg);
typedef __s32   (*ItxtDec_Open)(ItxtDec *thiz);
typedef __s32   (*ItxtDec_Close)(ItxtDec *thiz);
typedef __s32   (*ItxtDec_RequestWrite)(ItxtDec *thiz, __u32 uSizeRequested, __u8 **ppData0, __u32 *puSizeGot0, __u8 **ppData1, __u32 *puSizeGot1);
typedef __s32   (*ItxtDec_UpdatePointer)(ItxtDec *thiz, __lbs_datctlblk_t * pLbsDataCtrlBlk,__u32 uDataSize);
typedef void    (*ItxtDec_Destruct)(ItxtDec *thiz);

typedef struct tag_ItxtDec
{
    //实成员
    __s32               subtitle_bs_src;    //目前用到的有:CEDARLIB_FILE_FMT_MKV, CEDARLIB_FILE_FMT_VOB, CEDARLIB_FILE_FMT_TS, 表示内置字幕来源
    __s32               subtitle_type;      // srt, sub, etc. enum __CEDARLIB_LYRIC_FILE_FMT
    __s32               frm_rate;           //video frame rate. be careful : 数值放大了1000倍!, for sub decode
    __s32               width;              //for sub decode
    __s32               height;             //for sub decode
    __s32               tvType;             //sub字幕的解码，需要tvSystemType, TV_SYSTEM_PAL
    __u8                lastColorCode[4];
    __u8                lastContrast[4];
    __s32               screenWidth;    //显示窗口的宽高
    __s32               screenHeight;

    //__s8                decFlag;            //解码flag, 表示是否正在解码,起互斥作用的.不用了
    __s8                useAnciBuf;     //是否使用了ancibuf,来处理回头.Isub会使用,其他基本不用这个变量
    __s8                nSyncStatFlag;   //nResetLdecBufFlag,表示重新初始化所有buffer,准备重新解码
    
    __s8                strDecName[16];     //存储字幕解码器的名字: srt, txt, smi, ssa, lrc
    CedarLock           LbsBufLock;         //对__ldec_lbs_buf_t    ldecLbsBuf操作时的互斥锁
    CedarLock           FrmMgrLock;     //对__ldec_manager_t    ldecManager操作时的互斥锁
    __ldec_lbs_buf_t    ldecLbsBuf;     //解码类
    __ldec_manager_t    ldecManager;    //解码类
    __u8                *anciDataBuf;   //解码类, 解码时处理lbsbuf回头用的.
    __u32               nLbsBufSize;    //指示ldecLbsBuf的buffer的大小,在初始化时由子类覆盖赋值.
    __u32               nLdecFrameSize;  //指示ldecManager的frame的大小,在初始化时由子类覆盖赋值. 文本字幕都会很小, 图像字幕会很大.
    __u32               nanciDataBufSize;   //指示anciDataBuf的大小,基类会给一个值,子类根据需要可以覆盖.
    
    ItxtDec_IoCtrl          ItxtDecIoCtrl;  //基类会提供实现,用于设置基类一些成员的值
    ItxtDec_RequestWrite    RequestWrite;
    ItxtDec_UpdatePointer   UpdatePointer;
    ItxtDec_GetLrcItem      GetLrcItem;     //基类会提供实现,子类根据需要可以用自己的函数覆盖.
    ItxtDec_Open            open;           //基类会提供实现,
    ItxtDec_Close           close;          //同上
    ItxtDec_Decode          CommonDecode; //基类通用实现, 解码部分调用虚函数SubtitleDecode完成. isub会覆盖,使用自己的decode()函数来接吗.

    //虚成员
    ItxtDec_CheckSupport    CheckSupport;   //基类置NULL.检查文件类型是否支持, ret = EPDK_TRUE:支持;
    ItxtDec_SubtitleDecode  SubtitleDecode; //基类不实现, CommonDecode()会调用它.
    //ItxtDec_VirtualIoCtrl   EtxtDecVirtualIoCtrl;   //有的函数不同子类只是有些微改动,不想覆盖重写,故用它来实现.

    //析构函数
    ItxtDec_Destruct        Destruct;   //基类提供实现,一个警告信息,子类必须覆盖
}ItxtDec;
extern __s32 ItxtDec_Init(ItxtDec *thiz, __s32 nFileFmt);   //LYRIC_ITXT等
extern __s32 ItxtDec_Exit(ItxtDec *thiz);
extern ItxtDec* ItxtDec_Create(__s32 nFileFmt);   //enum __CEDARLIB_LYRIC_FILE_FMT
extern void ItxtDec_Destroy(ItxtDec *thiz);


extern void ldec_itxt_init_info(ItxtDec *ldec_itxt);
//extern __u8 *ldec_process_chunk_cover_bufEnd(__ldec_lbs_buf_t *pLbsBufManager, __u8 **ppanciDataBuf, __u32 *pnanciDataBufSize);

#endif  /* _ITXT_DECODE_H_ */

