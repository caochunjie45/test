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
* File   : ldec_lbs.h
* Version: V1.0
* By     : Eric_wang
* Date   : 2011-10-8
* Description:
    lbs buffer的管理类.单独列出.
********************************************************************************
*/
#ifndef _LDEC_LBS_H_
#define _LDEC_LBS_H_

#define LDEC_MAX_TXT_DATA_NUM      (64) //lbs分块的个数

#define LDEC_BUF_PRIVATE_DATA_LEN (16)

typedef struct LDEC_BUFFER_CTRL
{
    __u8   *startAddr;  //数据块的起始地址
    __u8    coverBufEnd;    //表示是否占据了buf的最后一个字节. 1:占据;0:没有占据.如果本数据块正好占完最后一个字节,那么也是置1的.
    __u8    hasPtsFlag; //是否带了pts.
    __u16   dataType;   //LYRIC_SUB_ENCODE_UTF8等,表示描述字幕项内容的编码格式.如果字幕项是文本,一般就是文本的字符编码类型;如果是图像,一般就是LYRIC_SUB_ENCODE_BITMAP
    __u32   ptsVal;     //该字幕条目的pts
    __u32   duration;   //该字幕条目的持续时间
    __u32   dataLen;    //本chunk的字节长度
    __s8    lbsChgFlg;  //表示是否换字幕了,仍然需要,对于issa来说,这表示本数据块是不是ssa的头信息. 其他字幕类型不需要. 而换字幕会由主控模块控制ldec_itxt进行.ldec_itxt自己不会主动处理了.
    __u8    privateData[LDEC_BUF_PRIVATE_DATA_LEN];
}__ldec_buffer_ctrl_t;


typedef struct LDEC_DATA_BUF
{
    __u8    *ldecBuf;   //lbsbuf的起始地址
    __u8    *ldecBufEnd;    //指向lbsbuf的最后一个字节
    __u8    *readAddr;  //decode将要读的字节
    __u8    *writeAddr; //指向将要写的字节
    __ldec_buffer_ctrl_t ldecBufCrl[LDEC_MAX_TXT_DATA_NUM]; //lbs的块管理
    __u16   ldecReadIdx;    //decoder将要读哪个lbs块
    __u16   ldecWriteIdx;   //将要写的lbs块的序号
    __u16   ldecIdxSize;    //有效的lbs块的数量
    __u32   validSize;  //有效的字节数

}__ldec_lbs_buf_t;


extern void  ldec_update_read_pointer(__ldec_lbs_buf_t *ldecLbsBuf, __u32 uread_byte);
extern __u8 *ldec_process_chunk_cover_bufEnd(__ldec_lbs_buf_t *pLbsBufManager, __u8 **ppanciDataBuf, __u32 *pnanciDataBufSize);

#endif  /* _LDEC_LBS_H_ */

