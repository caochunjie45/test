
#ifndef FBM_H
#define FBM_H

#define FRAME_BUFFER_NUM	(8)

//#include "subtitle_config.h"

typedef struct __FRAME_INFO
{
    __u8*	data;
	__u32	dataLen;    //data的大小,可能是valid数据的长度

	__u32	x;
	__u32	y;
	__u32	width;
	__u32	height;
	__u32   videoWidth;
	__u32   videoHeight;
    __u32   startDispTime;
    __u32   endDispTime;

	__u8	usedByDecoder; //表示是否正被decoder使用, decoder解码过程中一直占据该帧.将一直置1
	__u8	usedByDsp;      //是否正在被display使用.被要出去显示时,只要没有归还,就一直置1.在display队列里的帧被要出去显示时,一般usedByDsp置1,同时inQueue=0,表示从display队列出去了.
	__u8	inQueue;        //表示是否在等待display的队列里.
	
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
    __u32      	maxFrmNum;  //使用的frmInfo[]数量, <= FRAME_BUFFER_NUM,指实帧数量,空帧与实帧配对,所以也是空帧数量
    __u32       frame_buffer_size;
    
    frm_info_t 	frmInfo[FRAME_BUFFER_NUM];  //实帧
    frm_info_t  emptyFrameArray[FRAME_BUFFER_NUM];  //空帧数量和实帧数量一样就行
    
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

frm_info_t* fbm_empty_index_to_ptr(__u8 idx, fbm_t* fbm);  //空帧的idx

__u8 fbm_empty_ptr_to_index(frm_info_t* ptr, fbm_t* fbm);  //空帧的ptr
#endif

