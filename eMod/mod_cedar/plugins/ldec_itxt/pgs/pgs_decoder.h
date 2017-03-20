
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
    
    //__s8                useAnciBuf;     //�Ƿ�ʹ����ancibuf,�������ͷ.
    __s8                nSyncStatFlag;   //nResetLdecBufFlag,��ʾ���³�ʼ������buffer,׼�����½���
    
    CedarLock           LbsBufLock;         //��__ldec_lbs_buf_t    ldecLbsBuf����ʱ�Ļ�����
    CedarLock           FrmMgrLock;     //��__ldec_manager_t    ldecManager����ʱ�Ļ�����
    __ldec_lbs_buf_t    ldecLbsBuf;     //lbsbuf�Ĺ�����
    __u8                *anciDataBuf;   //������, ����ʱ����lbsbuf��ͷ�õ�.
    __u32               nLbsBufSize;    //ָʾldecLbsBuf��buffer�Ĵ�С,�ڳ�ʼ��ʱ�����า�Ǹ�ֵ.
    __u32               nanciDataBufSize;   //ָʾanciDataBuf�Ĵ�С,������һ��ֵ,���������Ҫ���Ը���.

    //__krnl_event_t* frameBufSem;

    PGSSubContext*  pgsDecCtx;
    fbm_t*          fbm;    //framebuf�Ĺ�����

//    __u8*           vbvBuf; //PGS_VBV_BUFFER_SIZE,128k
//    __u8*           vbvBufEnd;  //ָ��vbvbuf����һ���ֽ�
//    __u32           vbvBufSize; //vbvbuf���ܳ�
//    __u32           vbvDataSize;    //һ�δ����data�ĳ���
//    __s64           pts;    //һ��chunk��pts,����ts,һ��һ��segment�ͷ���һ��chunk��
    
    //frm_info_t      emptyFrame;   //ת��fbm_t����

}sub_decoder_t;
extern __s32 PgsDec_Init(sub_decoder_t *thiz);
extern __s32 PgsDec_Exit(sub_decoder_t *thiz);

#endif  /* _PGS_DECODER_H_ */

