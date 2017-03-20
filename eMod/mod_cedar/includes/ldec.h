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
    CEDAR_LDEC_CMD_PAUS,                // ��ʱ����, pause decoder
    CEDAR_LDEC_CMD_CONT,                // ��ʱ����, continue decoder
    CEDAR_LDEC_CMD_FF,                  // ��ʱ����,fast forward. ��������Ļ����������, pgs����.
    CEDAR_LDEC_CMD_RR,                  // ��ʱ����, fast reverse ��������Ļ����������, pgs����.
    CEDAR_LDEC_CMD_CLEAR_FFRR,          // ��ʱ����, clear fast forward/reverse ��������Ļ����������, pgs����.
    CEDAR_LDEC_CMD_JUMP,                // ��ʱ����,jump ��������Ļ����������, pgs����.
    CEDAR_LDEC_CMD_CLEAR_JUMP,          // ��ʱ����,clear jump ��������Ļ����������, pgs����.
    CEDAR_LDEC_CMD_GET_STATUS,          // ��ʱ����,get lyric decoder status ��������Ļ����������, pgs����.����Ҫ��status.

    CEDAR_LDEC_CMD_OPEN = 0x20,
    CEDAR_LDEC_CMD_CLOSE,
    CEDAR_LDEC_CMD_SET_LBSSKTIN,        // ������Ļ����.set socket for lyric bitstream input, 
    CEDAR_LDEC_CMD_GET_LBSSKTOUT,       // ������Ļ����.get lyric ouput socket, .
    CEDAR_LDEC_CMD_GET_RDRTYPE,         // ��ʱ����,get lyric render type
    CEDAR_LDEC_CMD_CHKSPT,              // check if current decoder can support the bitstream,�ýӿ�Ҫ��ֻ��ģ��ճ�ʼ��ʱ����.
    CEDAR_LDEC_CMD_SET_LBSTYPE,   //������Ļ�ļ�����,��Ҫ���������Ļ����ģ��,�����Ļģ�鲻ʵ�ָ�����, aux = LYRIC_ITXT��
    CEDAR_LDEC_CMD_SET_AVSHDL,          // set avsync handle
    CEDAR_LDEC_CMD_GET_LRCITEM,         // get lyric item, ret = __cedar_subtitle_item_t*����__cedar_buf_inf_t*, aux = pts, pbuffer = __cedar_get_sub_inf_t.
    CEDAR_LDEC_CMD_GET_SUBTITLEFRAME,   //Ҫ֡�ĵڶ��ֽӿ�.ret = CCedarSubtitleFrame*, aux = pts, pbuffer = __cedar_get_sub_inf_t.����lrc���������Ļ���ݲ�ʵ��.
    CEDAR_LDEC_CMD_SEEK_LRCITEM,        // ��ʱ����, seek lyric item
    CEDAR_LDEC_CMD_SELECT_SUBTITLE,     // ��select subtitle����Ϊ��������.ֻ���ͬһ�������Ļ�ڲ��л�. aux:�µ���Ļ���±��
                                        //�����Ļ��ldecģ������frame���У�
                                        //��Ƕ��Ļ��ldecģ�鲻��Ӧ�����
    CEDAR_LDEC_CMD_SET_VIDFRMRATE,      //����video frame.
    CEDAR_LDEC_CMD_SET_VIDFRMSIZE,      //����video frame�Ŀ����Ϣ��high16bit:width, low16bit:height.
    CEDAR_LDEC_CMD_SET_SCREEN_SIZE,     //set screen size��width<<16 | height, if the subtitle bitmap , PGS��Ҫ
                                        //exceed the screen size, decoder should scale it down to fit the screen.
    CEDAR_LDEC_CMD_SET_POST_PROCESS,    //�Ƿ������Ļ����, aux =1��ʾ����, aux=0��ʾ������,������Ļ����ģ���ݲ�����
    CEDAR_LDEC_CMD_SET_FILE_PATH,       //�����Ļ����ģ����Ҫ���·��,�Լ����ļ����н���.�����idxsub�������idx�ļ�·���Ϳ�����.pbuffer = __s8*

    CEDAR_LDEC_CMD_SET_ReqSubtitleFrameMode,    //aux = ReqSubtitleFrameMode;

    CEDAR_LDEC_CMD_SET_ONESEGFLAG = 0x40,   //aux = 0:fullseg, 1:onesegISDB��ʽ�ĵ�����, service��Ϊfullseg��oneseg����,����service��������Ļ��Ȼ����aribJIS����,�������в�ͬ��Ĭ������.���Ա�������.

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
    //LDEC_ERROR_DECODE_FAIL    = 1,        //this is not fatal error��
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
    ReqSubtitleFrameMode_AppReq             = 0,        //�Ϸ�ʽ,appֱ��Ҫ֡,ֱ�ӻ���Ļ.
    ReqSubtitleFrameMode_SubtitleplybkReq   = 1,        //�·�ʽ,��subtitle plybkģ��Ҫ֡,����Ļ.appͨ������callbackʵ�־���Ļ���Ļ�ķ���.
    ReqSubtitleFrameMode_,
} ReqSubtitleFrameMode;


typedef struct tag_CCedarSubtitleFrame
{
    //�Զ���
//    __u32       uPts;           // time stamp of the subtitle item, (ms)
//    __u32       uDuration;      // duration of the item, (ms)
//    __u16       uDataType;      // subtitle data type, text,bitmap,encode type,��Ļ������
                                //__cedar_subtitle_encode_t
//    __s32       nPicWidth;      //�����SUB��ͼ���ʽ����Ļ����Ҫ���Ϳ����Ϣ
//    __s32       nPicHeight;
//    __colorcode_t   colorCode;  //�����SUB��ͼ���ʽ����Ļ����Ҫ����color code�Ա�ӵ�ɫ��ѡ����ɫ
//    __contrast_t    contrast;   //�����SUB��ͼ���ʽ����Ļ����Ҫ����contrast ��ȷ���Ƿ���ʾ
//    __s32       uPtsSize;       // size of the time stamp offset
//    __s32       uSubSize;       // size of the subtitle data
//    __s16       *pPtsOffset;    // time stamp offset of every character
//    __u8        *pSubtitle;     // subtitle data buffer
      //CSubtitleDrawModeISDB   isdb;

    __u8        isFull;   //1:������; 0:��֡
    __s8        subNum; //��Ļ������, <=10
    
    __s8        position_flags;       //1:��λ����Ϣ; 0:�������.
    //__s8        Clear_screen_flags;   //����Ļ��,����app�廹�ǻ�.1: clear screen��   0: showdraw;  
    
    __u32       uPts;           // time stamp of the subtitle item, (ms)
    __u32       uDuration;      // duration of the item, (ms)
    __u16       uDataType;      // subtitle data type, text,bitmap,encode type,��Ļ������,__cedar_subtitle_encode_t, CEDAR_SUB_ENCODE_UTF8

    maple_subtitle_display_mode_t       displayMode;        //HD; SD; SD Wide; Mobile; mobile��ʾû��λ��Ҫ����,�ڳ�����,����������videoWidth���ж�Ӧ��ϵ.
    maple_subtitle_color_mode_t         subColor[ISDBDEC_MAX_LINE_NUM];        //color;   
    maple_subtitle_style_size_t         subStyleSize[ISDBDEC_MAX_LINE_NUM];    //style size;SSZ,MSZ��,������,subSize�ǵ����õ���ֵ,

    //��position_flags = 0ʱ,��ʾ���ɻ�,��ʱ����ı����ܶ��ǲ�����,���Բ���ֵ.�ֵĴ�С,��Ļ����,��Ƶͼ��������Ч��.
    __s16               subSize;        //һ���ֵĿ��, SSMָ��. 16����16*16; 36����36*36. Ĭ����36������,�����õ���ֵ,���ǲ�û��ʹ��,�ܶ���Ļ������Ҳû�д����ֵ.һ���ֵĴ�С��subStyleSize[]Ϊ׼.
    __s16               subWidth;       //����Ļ��������Ŀ�, ISDB����ó�. �����sub��pgs��ͼ����Ļ,���ʾ����Ļ������.nPicWidth
    __s16               subHeight;      //����Ļ��������ĸ�, sub��ͼ����Ļ,nPicHeight
    __s16               startX;         //����ó�. ����Ļ���������λ��,�������Ļ��ʽ�Լ��ٶ�����Ƶͼ�����������Ͻ�. videoWidth, videoHeight, �����Ļ����������
    __s16               startY;

    //��Ļ�����򻮷�Ϊ������,�и���maple_subtitle_style_size_t����,large = 60������.
    __s16               line[ISDBDEC_MAX_LINE_NUM];       //��Ļ��ÿ��������Ļ������ĵڼ��п�ʼ,��Ļ�б�Ŵ�0��ʼ��
    __s16               forward[ISDBDEC_MAX_LINE_NUM];    //��Ļ��ÿ���֣�ǰ��ռ�����,0��ʾ����,1��ʾ��1����.Ĭ�ϴ��ֵĿ�Ϊ40������,С��20������.
    __s16               videoWidth;     // ISDB��Ļ��ʽ�ٶ�����Ƶͼ��Ŀ��, ��maple_subtitle_display_mode_t����HD:960*540; SD:720*480;�������Ѿ�����ֵ��.
    __s16               videoHeight;
    
    __s32               subDataLen[ISDBDEC_MAX_LINE_NUM]; //��Ļ��һ��buffer��洢����Ч���ݳ��ȣ�������\0.���ܳ���subData[]��buffer�ĳ���.
    __u8*               subData[ISDBDEC_MAX_LINE_NUM];    //�洢��Ļ��һ�е�buffer��ʼ��ַ,����ǰ����.��С��ISDBDEC_MAX_FRM_SIZE,Ҳ���Զ�Ϊ����ֵ.��ͬ�Ľ�������һ��.
    //RECT                subClearScreen[10];    //����Ļ��,subtitle clear screen;������Ļ��app�õ�,��app����������Ļ������.����Ļʱ���õ�.
    RECT                subImageRect[DVBSUBDEC_MAX_LINE_NUM];    //DVB-T Image display restangle

    __s32                  uSubSize;       // size of the subtitle data,һ�����ǰ��ʾΪ���������ݵĳ��ȡ����������Ҳ���޸��ˡ������ǽ���֮�����Ļ����ֽڳ��ȣ� 
    __u8                   *pSubtitle;     //������Ž���ǰ��һ����Ļpes�������ݣ�ʵ������debug.����ǰ�����ڴ档
		__s32									 tmpSubvalidSize;//һ����Ļ������ĻPESȥ����ͷ��Ч�ܳ���
		__s32									 tmpSubSize;		//��ĻPES���ֱ�������TS�����ʱ����׸�TS�е�PES���ݶγ���	
    //������sub��pgs��ͼ����Ļ������ARGB��ʽͼ����Ϣ.
    __colorcode_t   colorCode;  //�����SUB��ͼ���ʽ����Ļ����Ҫ����color code�Ա�ӵ�ɫ��ѡ����ɫ
    __contrast_t    contrast;   //�����SUB��ͼ���ʽ����Ļ����Ҫ����contrast ��ȷ���Ƿ���ʾ
    //__u32                  reserve[32];

		__u32 			nCharEncodeType;//ts file
} CCedarSubtitleFrame;  //__cedar_subtitle_item_t, __maple_isdb_item_t



#endif  //_LDEC_H_


