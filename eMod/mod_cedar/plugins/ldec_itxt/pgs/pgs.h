
#ifndef PGS_H
#define PGS_H

//#include "subtitle_config.h"
//#include "fbm.h"

typedef enum PGS_DECODE_STATE
{
    NEED_PRESENTATION_SEGMENT,
    NEED_WINDOW_SEGMENT,
    NEED_PALLETE_SEGMENT,
    NEED_PICTURE_SEGMENT,
    NEED_DISPLAY_SEGMENT
}pgs_dec_state;

typedef struct PGS_SUBTITLE_PRESENTATION 
{
    __s32 x;
    __s32 y;
    __u32 video_w;
    __u32 video_h;
    __s32 id_number;
}PGSSubPresentation;

typedef struct PGS_SUBTITLE_PICTURE 
{
    __s32	w;
    __s32	h;
    __u32	rle_buffer_size;
	__u32	rle_data_len;
    __u8	rle[PGS_MAX_RLE_SIZE];
} PGSSubPicture;

#define MAX_NEG_CROP	1024
typedef struct PGS_SUBTITLE_CONTEXT 
{
    pgs_dec_state       state;  //��ʾ�´���Ҫ�ĸ�segment
    __u32               blockId;    //��ʾ����Ļ�Ƿ��ǿ���Ļ, 0x80:������; 0x0:����Ļ. ��presentation segment���ʶ,����һ��׼ȷ.
    __u32               lastDecodedPts; //����Ļ����һ��segmentЯ����PTS
    __u32               pictureSegmentPts;
    PGSSubPresentation	presentation;
    PGSSubPicture		picture;
#if !USE_16_COLOR_PALLETE
    __u32				clut[256];
	__u8				cropTbl[256 + 2*MAX_NEG_CROP];
#else
    __u8				clut[256];
#endif
}PGSSubContext;

PGSSubContext*  pgs_dec_create(void);
__s32			pgs_dec_destroy(PGSSubContext** ppCtx);
__s32			pgs_dec_decode(PGSSubContext* pCtx, __u8* data, __u32 len, frm_info_t* fb, __u32 pts);
__s32           pgs_dec_reset(PGSSubContext* pCtx);

#if USE_16_COLOR_PALLETE
void            pgs_dec_set_clut_offset(PGSSubContext* pCtx, __u32 offset);
#endif

#endif

