/*
*********************************************************************************************************
*                                                    eMOD
*                                   the Easy Portable/Player Operation System
*                                              mod_mmp sub-system
*
*                                    (c) Copyright 2008-2009, Kevin.Z China
*                                              All Rights Reserved
*
* File   : ldec.h
* Version: V1.0
* By     : Kevin.Z
*********************************************************************************************************
*/
#ifndef _LDEC_H_
#define _LDEC_H_

typedef enum __LYRIC_DECODER_COMMAND    // lyric decoder command
{
    CEDAR_LDEC_CMD_START = 0x00,        // start decoder
    CEDAR_LDEC_CMD_STOP,                // stop decoder
    CEDAR_LDEC_CMD_PAUS,                // 暂时无用, pause decoder
    CEDAR_LDEC_CMD_CONT,                // 暂时无用, continue decoder
    CEDAR_LDEC_CMD_FF,                  // 暂时无用,fast forward. 对内置字幕解码器有用, pgs除外.
    CEDAR_LDEC_CMD_RR,                  // 暂时无用, fast reverse 对内置字幕解码器有用, pgs除外.
    CEDAR_LDEC_CMD_CLEAR_FFRR,          // 暂时无用, clear fast forward/reverse 对内置字幕解码器有用, pgs除外.
    CEDAR_LDEC_CMD_JUMP,                // 暂时无用,jump 对内置字幕解码器有用, pgs除外.
    CEDAR_LDEC_CMD_CLEAR_JUMP,          // 暂时无用,clear jump 对内置字幕解码器有用, pgs除外.
    CEDAR_LDEC_CMD_GET_STATUS,          // 暂时无用,get lyric decoder status 对内置字幕解码器有用, pgs除外.但都要置status.

    CEDAR_LDEC_CMD_OPEN = 0x20,
    CEDAR_LDEC_CMD_CLOSE,
    CEDAR_LDEC_CMD_SET_LBSSKTIN,        // 内置字幕有用.set socket for lyric bitstream input, 
    CEDAR_LDEC_CMD_GET_LBSSKTOUT,       // 内置字幕有用.get lyric ouput socket, .
    CEDAR_LDEC_CMD_GET_RDRTYPE,         // 暂时无用,get lyric render type
    CEDAR_LDEC_CMD_CHKSPT,              // check if current decoder can support the bitstream,该接口要求只在模块刚初始化时调用.
    CEDAR_LDEC_CMD_SET_LBSTYPE,   //设置字幕文件类型,主要针对内置字幕解码模块,外挂字幕模块不实现该命令, aux = LYRIC_ITXT等
    CEDAR_LDEC_CMD_SET_AVSHDL,          // set avsync handle
    CEDAR_LDEC_CMD_GET_LRCITEM,         // get lyric item, ret = __cedar_subtitle_item_t*或者__cedar_buf_inf_t*, aux = pts, pbuffer = __cedar_get_sub_inf_t.
    CEDAR_LDEC_CMD_GET_SUBTITLEFRAME,   //要帧的第二种接口.ret = CCedarSubtitleFrame*, aux = pts, pbuffer = __cedar_get_sub_inf_t.但是lrc的整体给字幕项暂不实现.
    CEDAR_LDEC_CMD_SEEK_LRCITEM,        // 暂时无用, seek lyric item
    CEDAR_LDEC_CMD_SELECT_SUBTITLE,     // 对select subtitle的行为作出处理.只针对同一个外挂字幕内部切换. aux:新的字幕流下标号
                                        //外挂字幕的ldec模块会清空frame队列，
                                        //内嵌字幕的ldec模块不响应该命令。
    CEDAR_LDEC_CMD_SET_VIDFRMRATE,      //设置video frame.
    CEDAR_LDEC_CMD_SET_VIDFRMSIZE,      //设置video frame的宽高信息，high16bit:width, low16bit:height.
    CEDAR_LDEC_CMD_SET_SCREEN_SIZE,     //set screen size，width<<16 | height, if the subtitle bitmap , PGS需要
                                        //exceed the screen size, decoder should scale it down to fit the screen.
    CEDAR_LDEC_CMD_SET_POST_PROCESS,    //是否进行字幕后处理, aux =1表示进行, aux=0表示不进行,内置字幕解码模块暂不处理
    CEDAR_LDEC_CMD_SET_FILE_PATH,       //外挂字幕解码模块需要这个路径,自己打开文件进行解析.如果是idxsub，输入的idx文件路径就可以了.pbuffer = __s8*

    CEDAR_LDEC_CMD_SET_ReqSubtitleFrameMode,    //aux = ReqSubtitleFrameMode;

    CEDAR_LDEC_CMD_SET_ONESEGFLAG = 0x40,   //aux = 0:fullseg, 1:onesegISDB制式的电视流, service分为fullseg和oneseg两种,两种service包含的字幕虽然都是aribJIS编码,但还是有不同的默认设置.所以必须区分.

    CEDAR_LDEC_CMD_
} __lyric_decoder_command_t;

//typedef enum __SUBTITLE_SOURCE{
//    SUBTITLE_POS_NONE   = 0,    //no subtitle exist
//    SUBTITLE_POS_EMBED  = 1,    //embeded subtilte in video file
//    SUBTITLE_POS_EXT    = 2,    //extern subtitle in subtitle file
//    SUBTITLE_POS_,
//} __subtitle_source_t;

typedef enum __LDEC_RETURN_VAL
{
    LDEC_ERROR_OK             = 0,        //no error
    //LDEC_ERROR_DECODE_FAIL    = 1,        //this is not fatal error。
    LDEC_ERROR_LBS_UNDERFLOW  = 1,
    LDEC_ERROR_NO_FRAMEBUFFER = 2,
    LDEC_ERROR_FAIL           = -1,       //common fail, not care fail reason.
    LDEC_ERROR_LFMT_ERR       = -2,       //some parameter error
    LDEC_ERROR_REQMEM_FAIL    = -7,       //request memory failed.
//    FILE_PARSER_OPEN_FILE_FAIL  = -2,       //open media file failed
//    FILE_PARSER_READ_FILE_FAIL  = -3,       //read media file data failed
//    FILE_PARSER_FILE_FMT_ERR    = -4,       //media file format error
//    FILE_PARSER_NO_AV           = -5,       //neither audio nor video supported in media file
//    FILE_PARSER_END_OF_MOVI     = -6,       //movi data end
//    FILE_PARSER_REQMEM_FAIL     = -7,       //request memory failed
//    FILE_PARSER_EXCEPTION       = -8,       //some impossible case happens.
    LDEC_ERROR_

} __ldec_return_val_t;

typedef enum tag_ReqSubtitleFrameMode
{
    ReqSubtitleFrameMode_AppReq             = 0,        //老方式,app直接要帧,直接画字幕.
    ReqSubtitleFrameMode_SubtitleplybkReq   = 1,        //新方式,由subtitle plybk模块要帧,画字幕.app通过设置callback实现具体的画字幕的方法.
    ReqSubtitleFrameMode_,
} ReqSubtitleFrameMode;


typedef struct tag_CCedarSubtitleFrame
{
    //自定义
//    __u32       uPts;           // time stamp of the subtitle item, (ms)
//    __u32       uDuration;      // duration of the item, (ms)
//    __u16       uDataType;      // subtitle data type, text,bitmap,encode type,字幕项内容
                                //__cedar_subtitle_encode_t
//    __s32       nPicWidth;      //如果是SUB等图像格式的字幕，需要传送宽高信息
//    __s32       nPicHeight;
//    __colorcode_t   colorCode;  //如果是SUB等图像格式的字幕，需要传送color code以便从调色板选择颜色
//    __contrast_t    contrast;   //如果是SUB等图像格式的字幕，需要传送contrast 以确定是否显示
//    __s32       uPtsSize;       // size of the time stamp offset
//    __s32       uSubSize;       // size of the subtitle data
//    __s16       *pPtsOffset;    // time stamp offset of every character
//    __u8        *pSubtitle;     // subtitle data buffer
      //CSubtitleDrawModeISDB   isdb;

    __u8        isFull;   //1:有数据; 0:空帧
    __s8        subNum; //字幕的行数, <=10
    
    __s8        position_flags;       //1:有位置信息; 0:随意放置.
    //__s8        Clear_screen_flags;   //画字幕用,告诉app清还是画.1: clear screen；   0: showdraw;  
    
    __u32       uPts;           // time stamp of the subtitle item, (ms)
    __u32       uDuration;      // duration of the item, (ms)
    __u16       uDataType;      // subtitle data type, text,bitmap,encode type,字幕项内容,__cedar_subtitle_encode_t, CEDAR_SUB_ENCODE_UTF8

    maple_subtitle_display_mode_t       displayMode;        //HD; SD; SD Wide; Mobile; mobile表示没有位置要求了,在程序中,根据它决定videoWidth等有对应关系.
    maple_subtitle_color_mode_t         subColor[ISDBDEC_MAX_LINE_NUM];        //color;   
    maple_subtitle_style_size_t         subStyleSize[ISDBDEC_MAX_LINE_NUM];    //style size;SSZ,MSZ等,程序中,subSize是单独得到的值,

    //当position_flags = 0时,表示自由画,这时下面的变量很多是不看的,可以不赋值.字的大小,字幕区域,视频图像区域都无效了.
    __s16               subSize;        //一个字的宽高, SSM指定. 16代表16*16; 36代表36*36. 默认是36个像素,单独得到的值,但是并没有使用,很多字幕数据里也没有带这个值.一个字的大小以subStyleSize[]为准.
    __s16               subWidth;       //画字幕的总区域的宽, ISDB解码得出. 如果是sub，pgs等图像字幕,则表示画字幕的区域.nPicWidth
    __s16               subHeight;      //画字幕的总区域的高, sub等图像字幕,nPicHeight
    __s16               startX;         //解码得出. 画字幕的总区域的位置,相对于字幕格式自己假定的视频图像的区域的左上角. videoWidth, videoHeight, 最后画字幕按比例缩放
    __s16               startY;

    //字幕总区域划分为若干行,行高由maple_subtitle_style_size_t决定,large = 60个像素.
    __s16               line[ISDBDEC_MAX_LINE_NUM];       //字幕的每行字在字幕总区域的第几行开始,字幕行标号从0开始。
    __s16               forward[ISDBDEC_MAX_LINE_NUM];    //字幕的每行字，前面空几个字,0表示不空,1表示空1个字.默认大字的宽为40个像素,小字20个像素.
    __s16               videoWidth;     // ISDB字幕格式假定的视频图像的宽高, 由maple_subtitle_display_mode_t决定HD:960*540; SD:720*480;解码器已经赋好值了.
    __s16               videoHeight;
    
    __s32               subDataLen[ISDBDEC_MAX_LINE_NUM]; //字幕的一行buffer里存储的有效数据长度，不包括\0.不能超过subData[]的buffer的长度.
    __u8*               subData[ISDBDEC_MAX_LINE_NUM];    //存储字幕的一行的buffer起始地址,解码前分配.大小是ISDBDEC_MAX_FRM_SIZE,也可以定为其他值.不同的解码器不一样.
    //RECT                subClearScreen[10];    //画字幕用,subtitle clear screen;给画字幕的app用的,供app保存那条字幕的区域.清字幕时会用到.
    RECT                subImageRect[DVBSUBDEC_MAX_LINE_NUM];    //DVB-T Image display restangle

    __s32                  uSubSize;       // size of the subtitle data,一般解码前表示为待解码数据的长度。解码过程中也不修改了。本来是解码之后的字幕项的字节长度， 
    __u8                   *pSubtitle;     //用来存放解码前的一个字幕pes包的数据，实际用来debug.解码前分配内存。
		__s32									 tmpSubvalidSize;//一个字幕完整字幕PES去除包头有效总长度
		__s32									 tmpSubSize;		//字幕PES包分别由两个TS包组成时存放首个TS中的PES数据段长度	
    //新增的sub和pgs等图像字幕解码后的ARGB格式图的信息.
    __colorcode_t   colorCode;  //如果是SUB等图像格式的字幕，需要传送color code以便从调色板选择颜色
    __contrast_t    contrast;   //如果是SUB等图像格式的字幕，需要传送contrast 以确定是否显示
    //__u32                  reserve[32];

		__u32 			nCharEncodeType;//ts file
} CCedarSubtitleFrame;  //__cedar_subtitle_item_t, __maple_isdb_item_t



#endif  //_LDEC_H_


