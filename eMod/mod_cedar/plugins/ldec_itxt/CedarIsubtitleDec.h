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
    内置字幕解码框架基于该基类开发.
    返回值类型:__ldec_return_val_t, LDEC_ERROR_OK等
********************************************************************************
*/
#ifndef _CEDARISUBTITLEDEC_H_
#define _CEDARISUBTITLEDEC_H_

#define strDecNameSize  (16)

typedef enum tag_CedarIsubtitleDecCmd
{
    CEDARISUBTITLEDEC_CMD_SET_FRMRATE = 0,    //设置帧率, 单位为1000. aux = framerate
    CEDARISUBTITLEDEC_CMD_SET_FRMSIZE,        //设置frame size. aux = width << 16 | height
    CEDARISUBTITLEDEC_CMD_SET_SCREEN_SIZE,    //设置screen size.aux = width << 16 | height
    CEDARISUBTITLEDEC_CMD_SET_SUBTITLE_SOURCE,    //设置内置字幕的来源,从哪种视频文件送过来. __subtitle_codec_format_t->subtitle_bs_src,
    //ITXTDEC_CMD_SET_POST_PROCESS,   //设置是否进行文本字幕后处理
    CEDARISUBTITLEDEC_CMD_NOTIFY_SYNCSTAT,     //设置reset标记,当lyricdec的decode()发现该标记位时,会先reset所有buffer.然后再解码.
    CEDARISUBTITLEDEC_CMD_CHECK_SYNCSTAT,       //查看syncstat的状态是否完成. ret = 0:完成, ret = 1:仍然在syncstat
    //ITXTDEC_CMD_SHOW_FREE_LBSBUF,   //查看剩余的lbsbuffer的长度, ret = left buflen

    //ITXTDEC_CMD_GETLRCITEM_LOCK = 0x20,   //有的decoder如smi,取字幕项时需要互斥.
    //ITXTDEC_CMD_GETLRCITEM_UNLOCK,

    CEDARISUBTITLEDEC_CMD_SET_ONESEGFLAG,   //aux = 0:fullseg, 1:onesegISDB制式的电视流, service分为fullseg和oneseg两种,两种service包含的字幕虽然都是aribJIS编码,但还是有不同的默认设置.所以必须区分.
    CEDARISUBTITLEDEC_CMD_SET_CHAR_CODING_TYPE, //设置文本字幕的字符编码格式. aux = __CEDARLIB_LYRIC_ENCODE_TYPE(LYRIC_SUB_ENCODE_UTF8)
    
    CEDARISUBTITLEDEC_CMD_,
}CedarIsubtitleDecCmd;

/*******************************************************************************
                            成员函数说明
EtxtDec_GetLrcItem:
  nType:CEDAR_GET_SUB_INF_ALL/CEDAR_GET_SUB_INF_ITEM, 
  ret:NULL:没有解析完;__cedar_buf_inf_t:如果nType=CEDAR_GET_SUB_INF_ALL;__cedar_subtitle_item_t:如果nType=CEDAR_GET_SUB_INF_ITEM;  

    
Parameters: 
    
Return: 
    
Time: 2011/8/3
*******************************************************************************/
typedef struct tag_CedarIsubtitleDec CedarIsubtitleDec;
typedef __s32   (*CedarIsubtitleDec_IoCtrl)(CedarIsubtitleDec *thiz, __s32 cmd, __s32 aux, void *pbuffer);  //cmd = CedarIsubtitleDecCmd
typedef __s32   (*CedarIsubtitleDec_CheckSupport)(CedarIsubtitleDec *thiz, __s32 nFileFmt);   //ret = EPDK_TRUE, EPDK_FALSE, nFileFmt = enum __CEDARLIB_LYRIC_FILE_FMT
typedef __cedar_subtitle_item_t* (*CedarIsubtitleDec_GetLrcItem)(CedarIsubtitleDec *thiz, __u32 nPts, __s32 *pRedecodeFlag);    //ret = pOutputItem, NULL表示没有找到, *pRedecodeFlag表示是否需要重新解码
typedef __s32   (*CedarIsubtitleDec_Decode)(CedarIsubtitleDec *thiz);   //字幕解码
//usedBytes:送入时表示pLbs的长度,函数返回时表示解码用了多少字节. lbsChgFlg:原义是是否换字幕了,现在给issa使用,表示是否是ssa的第一笔数据也就是头信息
typedef __s32   (*CedarIsubtitleDec_Open)(CedarIsubtitleDec *thiz);
typedef __s32   (*CedarIsubtitleDec_Close)(CedarIsubtitleDec *thiz);
typedef __s32   (*CedarIsubtitleDec_RequestWrite)(CedarIsubtitleDec *thiz, __u32 uSizeRequested, __u8 **ppData0, __u32 *puSizeGot0, __u8 **ppData1, __u32 *puSizeGot1);
typedef __s32   (*CedarIsubtitleDec_UpdatePointer)(CedarIsubtitleDec *thiz, __lbs_datctlblk_t * pLbsDataCtrlBlk,__u32 uDataSize);
typedef void    (*CedarIsubtitleDec_Destruct)(CedarIsubtitleDec *thiz);
typedef CCedarSubtitleFrame* (*CedarIsubtitleDec_GetSubtitleFrame)(CedarIsubtitleDec *thiz, __u32 nPts, __s32 *pRedecodeFlag);    //ret = pOutputItem, NULL表示没有找到, *pRedecodeFlag表示是否需要重新解码

typedef struct tag_CedarIsubtitleDec
{
    //实成员
    __s32               subtitle_bs_src;    //目前用到的有:CEDARLIB_FILE_FMT_MKV, CEDARLIB_FILE_FMT_VOB, CEDARLIB_FILE_FMT_TS, 表示内置字幕来源
    __s32               subtitle_type;      // srt, sub, etc. enum __CEDARLIB_LYRIC_FILE_FMT,
    __s8                strDecName[strDecNameSize];     //存储字幕解码器的名字: srt, txt, smi, ssa, lrc, 子类赋值
    
    //虚成员,全部作为虚函数
    CedarIsubtitleDec_Open            open;           //
    CedarIsubtitleDec_Close           close;          //

    CedarIsubtitleDec_IoCtrl          CedarISubtitleDecIoCtrl;  //基类不实现, cmd = CedarIsubtitleDecCmd
    CedarIsubtitleDec_RequestWrite    RequestWrite;             //基类不实现
    CedarIsubtitleDec_UpdatePointer   UpdatePointer;            //基类不实现.ret = LDEC_ERROR_OK等
    CedarIsubtitleDec_GetLrcItem      GetLrcItem;               //基类不会提供实现,子类根据需要可以用自己的函数覆盖.老方式的要帧,数据结构__cedar_subtitle_item_t*
    CedarIsubtitleDec_Decode          decode;                   //基类不提供通用实现
    CedarIsubtitleDec_CheckSupport    CheckSupport;             //基类不实现.基类置NULL.检查文件类型是否支持, ret = EPDK_TRUE:支持;
    CedarIsubtitleDec_GetSubtitleFrame  GetSubtitleFrame;       //基类不实现.要帧的第二种接口,返回CCedarSubtitleFrame.供SubtitlePlybk模块调用.

    //析构函数
    CedarIsubtitleDec_Destruct        Destruct;   //基类提供实现,一个警告信息,子类必须覆盖
}CedarIsubtitleDec;

extern __s32 CedarIsubtitleDec_Init(CedarIsubtitleDec *thiz, __s32 nFileFmt);   //LYRIC_ITXT等
extern __s32 CedarIsubtitleDec_Exit(CedarIsubtitleDec *thiz);
extern CedarIsubtitleDec* CedarIsubtitleDec_Create(__s32 nFileFmt);   //enum __CEDARLIB_LYRIC_FILE_FMT
extern void CedarIsubtitleDec_Destroy(CedarIsubtitleDec *thiz);

extern __s8* Isubtitle_strcpy(__s8* pdest, __s32 nDestSize, const __s8* psrc);

#endif  /* _CEDARISUBTITLEDEC_H_ */

