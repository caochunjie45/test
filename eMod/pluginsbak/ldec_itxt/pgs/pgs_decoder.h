
#ifndef _PGS_DECODER_H_
#define _PGS_DECODER_H_

//#include "subtitle_config.h"
#include "pgs_decoder_interface.h"
//#include "pgs.h"
//#include "fbm.h"

typedef struct SUBTITLE_DECODER
{
    PgsDecInterface base;
    
    //__u32           mid;
    
    //__s8                useAnciBuf;     //是否使用了ancibuf,来处理回头.
    __s8                nSyncStatFlag;   //nResetLdecBufFlag,表示重新初始化所有buffer,准备重新解码
    
    CedarLock           LbsBufLock;         //对__ldec_lbs_buf_t    ldecLbsBuf操作时的互斥锁
    CedarLock           FrmMgrLock;     //对__ldec_manager_t    ldecManager操作时的互斥锁
    __ldec_lbs_buf_t    ldecLbsBuf;     //lbsbuf的管理器
    __u8                *anciDataBuf;   //解码类, 解码时处理lbsbuf回头用的.
    __u32               nLbsBufSize;    //指示ldecLbsBuf的buffer的大小,在初始化时由子类覆盖赋值.
    __u32               nanciDataBufSize;   //指示anciDataBuf的大小,基类会给一个值,子类根据需要可以覆盖.

    //__krnl_event_t* frameBufSem;

    PGSSubContext*  pgsDecCtx;
    fbm_t*          fbm;    //framebuf的管理器

//    __u8*           vbvBuf; //PGS_VBV_BUFFER_SIZE,128k
//    __u8*           vbvBufEnd;  //指向vbvbuf的下一个字节
//    __u32           vbvBufSize; //vbvbuf的总长
//    __u32           vbvDataSize;    //一次传输的data的长度
//    __s64           pts;    //一个chunk的pts,对于ts,一般一个segment就放在一个chunk里
    
    //frm_info_t      emptyFrame;   //转到fbm_t里面

}sub_decoder_t;
extern __s32 PgsDec_Init(sub_decoder_t *thiz);
extern __s32 PgsDec_Exit(sub_decoder_t *thiz);

#endif  /* _PGS_DECODER_H_ */

