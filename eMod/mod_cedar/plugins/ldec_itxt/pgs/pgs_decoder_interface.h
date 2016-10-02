
#ifndef _PGS_DECODER_INTERFACE_H_
#define _PGS_DECODER_INTERFACE_H_

//#include "subtitle_config.h"

//#define PTS_VALID_BIT           (1<<1)  //* whether pts is valid.
//#define FIRST_PART_BIT          (1<<3)  //* media data is the first part of a new frame.
//#define LAST_PART_BIT           (1<<4)  //* media data is the last part of a frame.

//#define REQUEST_BUFFER_SIZE     64*1024     //* request 32KB buffer once

typedef struct SUBTITLE_FRAME subtitle_frame_t;

typedef struct SUBTITLE_FRAME
{   
	__u8				index;
    __u8                isFull; //是否装数据, 有可能是空白帧,0表示空白帧.
    __u16               subWidth;
    __u16               subHeight;
    __u16               startX;    
    __u16               startY;
    __u16               videoWidth;
    __u16               videoHeight;
    __s64               ptsVal; //ms
    __u32               timeDuration;   //持续时间,一般不准,因为pgs得不到持续时间
    __u8                *pSubtitle;
    subtitle_frame_t    *nextItem;
}subtitle_frame_t;

//typedef struct __MEDIA_BUFFER
//{
//    __u8*   buf;
//    __u32   bufSize;
//}__md_buf_t;
//
//typedef struct __MEDIA_DATA_INFO
//{
//    __s64   pts;
//    __u32   dataLen;
//    __u32   ctrlBits;
//}__md_data_info_t;


//typedef enum SUBTITLE_CMD
//{
//    SUBTITLE_CMD_START,             //* start decode task, aux = 0, pbuffer = NULL;
//
//    SUBTITLE_CMD_STOP,              //* stop decode task, aux = 0, pbuffer = NULL;
//
//    SUBTITLE_CMD_REQUEST_BUFFER,    //* request bit stream buffer from decoder;
                                    //* aux = 0, pbuffer = __md_buf_t*
//
//    SUBTITLE_CMD_UPDATE_DATA,       //* update bit stream data to decoder;
                                    //* aux = 0, pbuffer = __md_data_info_t*
//
//    SUBTITLE_CMD_REQUEST_FRAME,     //* request subtitle frames from decoder;
                                    //* aux = 0, pbuffer = subtitle_frame_t*;, ret = -1表示没找到帧
//
//    SUBTITLE_CMD_RELEASE_FRAME,     //* release subtitle frames to decoder;
                                    //* aux = 0, pbuffer = subtitle_frame_t*; 作用是指示该帧不再被显示了,就可以被解码器使用了.因为frame在display_request时已经出了display队列,所以这里就不做出队列操作,空帧无意义,所以出了队列就不用释放了.
//                                    
//    SUBTITLE_CMD_GET_NEXT_PTS,      //* get the presentation time of next subtitle frame; ret = -1表示没有下一帧,0表示找到下一帧
//                                    
//    SUBTITLE_CMD_RESET,             //* reset subtitle decoder;
//    
//    SUBTITLE_CMD_NUM
//
//}subtitle_cmd_t;


//void  SubtitleDecInit(void);
//void  SubtitleDecExit(void);
//__mp* SubtitleDecOpen(__u32 mid, __u32 mode);
//__s32 SubtitleDecClose(__mp* mp);
//__u32 SubtitleDecRead(void* pdata, __u32 size, __u32 pid, __mp* mp);
//__u32 SubtitleDecWrite(const void* pdata, __u32 size, __u32 pid, __mp* mp);
//__s32 SubtitleDecIOCtrl(__mp* mp, __u32 cmd, __s32 aux, void* pbuffer);


#define PGS_FRAME_SIZE		        (300*1024)
#define PGS_MAX_RLE_SIZE	        (128*1024)
#define PGS_VBV_BUFFER_SIZE		    (1024*332)  //(1024*128)
#define PGS_VBV_BUFFFR_ANCI_SIZE    (1024*96)     //防止解码时数据回头,
#define PGS_FRAME_BUFFER_NUM	    (3) //(4)

typedef enum tag_PgsResult
{
    PGS_ERR_NONE               =  0,       // decode one subtitle successed
    PGS_ERR_LBS_UNDERFLOW      =  1,       // subtitle bitstream underflow
    PGS_ERR_NO_FRAMEBUFFER     =  2,       // there is no free subtitle buffer for decoder
    PGS_ERR_DEC_ONE_FRAME      =  3,        //解码出一帧.
    PGS_ERR_FAIL               = -1,       // some error has happend when decode subtitle
    PGS_ERR_LFMT_ERR           = -2,       // the format of the video is not supported, fatal error
    PGS_ERR_
} PgsResult;

typedef enum tag_PgsDecCmd
{
    PGS_CMD_SET_SUBTITLE_SOURCE,    //设置字幕来源,一般是ts,也有可能是mkv, CEDARLIB_FILE_FMT_TS等
    PGS_CMD_GET_NEXT_PTS,      //* get the presentation time of next subtitle frame; ret = -1表示没有下一帧,0表示找到下一帧, pbuffer =__u32*;
    PGS_CMD_NOTIFY_SYNCSTAT,    //通知pgsdec准备进行状态同步,其实就是在decode()里清buffer.
    PGS_CMD_CHECK_SYNCSTAT,     //查询是否状态同步完成了. ret = 0:完成, ret = 1:仍然在syncstat
    
    PGS_CMD_,
}PgsDecCmd;

/*******************************************************************************
                            成员函数说明
EtxtDec_GetLrcItem:
  nType:CEDAR_GET_SUB_INF_ALL/CEDAR_GET_SUB_INF_ITEM, 
  ret:NULL:没有解析完;__cedar_buf_inf_t:如果nType=CEDAR_GET_SUB_INF_ALL;__cedar_subtitle_item_t:如果nType=CEDAR_GET_SUB_INF_ITEM;  

    
Parameters: 
    
Return: 
    
Time: 2011/8/3
*******************************************************************************/
typedef struct tag_PgsDecInterface PgsDecInterface;
typedef __s32   (*PgsDecInterface_Open)(PgsDecInterface *thiz);
typedef __s32   (*PgsDecInterface_Close)(PgsDecInterface *thiz);
typedef __s32   (*PgsDecInterface_RequestWrite)(PgsDecInterface *thiz, __u32 uSizeRequested, __u8 **ppData0, __u32 *puSizeGot0, __u8 **ppData1, __u32 *puSizeGot1);
typedef __s32   (*PgsDecInterface_UpdatePointer)(PgsDecInterface *thiz, __lbs_datctlblk_t * pLbsDataCtrlBlk,__u32 uDataSize);
typedef __s32   (*PgsDecInterface_RequestFrame)(PgsDecInterface *thiz, subtitle_frame_t *pFrame);   //ret = EPDK_OK;
typedef __s32   (*PgsDecInterface_ReleaseFrame)(PgsDecInterface *thiz, subtitle_frame_t *pFrame);   //ret = EPDK_OK;
typedef __s32   (*PgsDecInterface_Decode)(PgsDecInterface *thiz);   //字幕解码
typedef __s32   (*PgsDecInterface_IoCtrl)(PgsDecInterface *thiz, __s32 cmd, __s32 aux, void *pbuffer);  //cmd = ItxtDecCmd

typedef struct tag_PgsDecInterface
{
    //实成员
    __s32               subtitle_bs_src;    //目前用到的有:CEDARLIB_FILE_FMT_MKV, CEDARLIB_FILE_FMT_VOB, CEDARLIB_FILE_FMT_TS, 表示内置字幕来源
    __s32               subtitle_type;      // srt, sub, etc. enum __CEDARLIB_LYRIC_FILE_FMT
    __s8                strDecName[16];     //存储字幕解码器的名字: srt, txt, smi, ssa, lrc

    PgsDecInterface_Open            open;           //基类会提供实现,
    PgsDecInterface_Close           close;          //同上
    PgsDecInterface_RequestWrite    RequestWrite;
    PgsDecInterface_UpdatePointer   UpdatePointer;
    PgsDecInterface_RequestFrame    RequestFrame;
    PgsDecInterface_ReleaseFrame    ReleaseFrame;
    PgsDecInterface_Decode          decode; //基类通用实现, 解码部分调用虚函数SubtitleDecode完成. isub会覆盖,使用自己的decode()函数来接吗.
    PgsDecInterface_IoCtrl          PgsIoCtrl;  //基类会提供实现,用于设置基类一些成员的值, PgsDecCmd
}PgsDecInterface;
extern __s32 PgsDecInterface_Init(PgsDecInterface *thiz);   //LYRIC_ITXT等
extern __s32 PgsDecInterface_Exit(PgsDecInterface *thiz);
extern PgsDecInterface* newPgsDecInterface(void);   //enum __CEDARLIB_LYRIC_FILE_FMT
extern void deletePgsDecInterface(PgsDecInterface *thiz);

#endif  /* _PGS_DECODER_INTERFACE_H_ */

