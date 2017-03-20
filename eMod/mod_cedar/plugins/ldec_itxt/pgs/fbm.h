
#ifndef FBM_H
#define FBM_H

#define FRAME_BUFFER_NUM	(8)

//#include "subtitle_config.h"

typedef struct __FRAME_INFO
{
    __u8*	data;
	__u32	dataLen;    //data�Ĵ�С,������valid���ݵĳ���

	__u32	x;
	__u32	y;
	__u32	width;
	__u32	height;
	__u32   videoWidth;
	__u32   videoHeight;
    __u32   startDispTime;
    __u32   endDispTime;

	__u8	usedByDecoder; //��ʾ�Ƿ�����decoderʹ��, decoder���������һֱռ�ݸ�֡.��һֱ��1
	__u8	usedByDsp;      //�Ƿ����ڱ�displayʹ��.��Ҫ��ȥ��ʾʱ,ֻҪû�й黹,��һֱ��1.��display�������֡��Ҫ��ȥ��ʾʱ,һ��usedByDsp��1,ͬʱinQueue=0,��ʾ��display���г�ȥ��.
	__u8	inQueue;        //��ʾ�Ƿ��ڵȴ�display�Ķ�����.
	
    __u32	uPTS;               // presentation time stamp
    __u32	frmDecMode;
    
}frm_info_t;

typedef struct __DISPLAY_QUEUE
{
    __u32 		readIdx;
    __u32 		writeIdx;
    __u32 		frmNum;
    __u32 		maxFrmNum;
    frm_info_t* frmId[FRAME_BUFFER_NUM];
}dsp_queue_t;


typedef struct __FRAME_BUFFER_MANAGER
{
    __u32      	maxFrmNum;  //ʹ�õ�frmInfo[]����, <= FRAME_BUFFER_NUM,ָʵ֡����,��֡��ʵ֡���,����Ҳ�ǿ�֡����
    __u32       frame_buffer_size;
    
    frm_info_t 	frmInfo[FRAME_BUFFER_NUM];  //ʵ֡
    frm_info_t  emptyFrameArray[FRAME_BUFFER_NUM];  //��֡������ʵ֡����һ������
    
    dsp_queue_t dsp_queue;
    
}fbm_t;

fbm_t* fbm_create(__u32 frmBufSize, __u32 maxFrmNum);
				  
__s32 fbm_destroy(fbm_t **ppfbm);

void fbm_reset(fbm_t* fbm, __u8 bReserveDspFrm);

__s32 fbm_decoder_request_frm(frm_info_t **ppFrm, fbm_t* fbm);
__s32 fbm_decoder_request_empty_frm(frm_info_t **ppFrm, fbm_t* fbm);

__s32 fbm_decoder_return_invalid_frm(frm_info_t **ppFrm, fbm_t* fbm);

__s32 fbm_decoder_return_valid_frm(frm_info_t** ppFrm, fbm_t* fbm);

__s32 fbm_decoder_share_frm(frm_info_t* pFrm, fbm_t* fbm);

__s32 fbm_dsp_request_frm(frm_info_t** ppFrm, fbm_t* fbm);

__s32 fbm_dsp_return_frm(frm_info_t* pFrm, fbm_t* fbm);

__s32 fbm_dsp_get_show_time(__u32 *pts, fbm_t* fbm);

frm_info_t* fbm_index_to_ptr(__u8 idx, fbm_t* fbm);

__u8 fbm_ptr_to_index(frm_info_t* ptr, fbm_t* fbm);

frm_info_t* fbm_empty_index_to_ptr(__u8 idx, fbm_t* fbm);  //��֡��idx

__u8 fbm_empty_ptr_to_index(frm_info_t* ptr, fbm_t* fbm);  //��֡��ptr
#endif

