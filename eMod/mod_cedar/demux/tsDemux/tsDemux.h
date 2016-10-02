
#ifndef _TS_DEMUX_H
#define _TS_DEMUX_H

//#include "mod_cedar_i.h"
//#include "tsDemux_interface.h"
//#include "tsc_interface.h"

#define AV_RB16(p)  ((*(p))<<8 | (*((p)+1)))
#define AV_RB32(p)  ((*(p))<<24 | (*((p)+1))<<16 | (*((p)+2))<<8 | (*((p)+3)))

#define NB_PID_MAX                  8192
#define MAX_SECTION_SIZE            4096    //EIT section最多是4096字节
/* TS stream handling */
typedef enum MPEGTSSTATE 
{
    MPEGTS_HEADER = 0,
    MPEGTS_PESHEADER_FILL,
    MPEGTS_PAYLOAD,
    MPEGTS_SKIP,
}__mpegts_state_t;

/* enough for PES header + length */
#define PES_START_SIZE      9
#define MAX_PES_HEADER_SIZE (9 + 255)


//* define MpegTSPESFilter
typedef struct PESFILTER  PESFILTER;
typedef struct SECTIONFILTER SECTIONFILTER;
typedef struct tag_TSDATAFILTER __tsdata_filter_t;

typedef __s32 (*F_PUSH_AVPES_DATA)(__u8* data, __u32 len, __u32 new_frm, PESFILTER* pes);
typedef __s32 (*F_PUSH_SUBPES_DATA)(__u8* data, __u32 len, __u32 new_frm, PESFILTER* pes);
typedef __s32 (*F_PUSH_SECTION_DATA)(__u8* data, __u32 len, __u32 new_frm, SECTIONFILTER* section);
typedef __s32 (*F_PUSH_TS_DATA)(__u8* data, __u32 len, __tsdata_filter_t* tspacket);


typedef struct PESFILTER    //对应一个虚通道的拼pes包的filter.
{
    __u8*				cur_ptr;    //copy数据的目的地址, 从后面的vdec等过来.
    __u32				free_size;  //cur_ptr的剩余空间
    __u32				valid_size; //记录pes包的去除包头数据的copy到目的地址的还没有传到后面解码器的PES_packet_data_byte的数量.ES
    __u32               payload_size;   //* pes packet payload size.记录当前还没有送出去的payload的字节数
    __u32				ctrl_bits;
    __u32               rap_flag;       //* random access point flag
    
    __md_data_info_t    data_info;
    __md_buf_t          md_buf;

    __pCBK_t            RequestDataCB;
    __pCBK_t            UpdateDateCB;

    __s32				pid;
    __mpegts_state_t	state;  //初始化时为MPEGTS_HEADER.会在handle_pkt_payload()判断,如果不是header,就skip.刚开始收数据时一般不会恰好收到header,那么就要skip.
    
    /* used to get the format */
		__u8				pts_dts_flag;
    __s64            	pts;    //直接从码流读出来的数值,不是直接的时间,/90才是以ms为单位的时间.因为计数是27M/300。
    __s64						pre_pts;
    __s64               pts_to_send;
    F_PUSH_AVPES_DATA	    push_avpes_data;  //push_avpes_data()  音视频数据
    F_PUSH_SUBPES_DATA	    push_subpes_data;  //push_subpes_data()  字幕数据
    __s32          		data_index; //pes包当前的读指针指向的地址
    __s32            	total_size; //pes包的总长度. 为0表示pes包没有边界.
    __s32             pes_header_size;    //也包括pes包起始的9字节
    __u8            	header[MAX_PES_HEADER_SIZE];

}__pes_filter_t;



//* define MpegTSSectionFilter

typedef struct SECTIONFILTER    //对应一个虚通道的拼si section的filter.
{
    __u32       need_start; //置1表示需要一个完整的开始.初始置1.实际上作用仅仅是表示filter刚刚开始取数据,很有限.
    __s32       section_index;  //记录当前section的已经copy的字节数
    __s32       section_h_size; //section总的size.section_length + 3
   __md_buf_t   md_buf; //向后端申请buffer的数据结构.push_section_data().
    __s32       check_crc;
    __s32       end_of_section_reached; //标记整个section是否读完.
    __s32       last_cc;    //指的是ts包中的continuity_couner的值,上一个包的. -1表示没有上一个包,本包是第一个包.
    

   __u8*				cur_ptr;    //初始记录向后端申请到的buffer的起始地址,会递增,随时指向buffer当前的空闲地址
   __u32				free_size;  //初始记录向后端申请到的buffer的总长,随后是剩余的长度
   __s32				 pid;
   __s32				 servId;		//搜索PMT时附带过滤
   __pCBK_t              RequestDataCB; //dmx的使用者注册, dvb_dmx_chan_param_t->request_buffer_cb, 真正调用是在push_section_data()里
   __pCBK_t              UpdateDateCB;  //dmx的使用者注册, dvb_dmx_chan_param_t->update_data_cb
   F_PUSH_SECTION_DATA	 push_section_data; //push_section_data()
   
}__section_filter_t;

//* define MpegTSRawDataFilter
typedef struct tag_TSDATAFILTER    //对应一个虚通道的直接处理ts packet的filter.
{
   __md_buf_t   md_buf; //向后端申请buffer的数据结构.push_ts_data().直接推送ts packet.

   __u8*				cur_ptr;    //初始记录向后端申请到的buffer的起始地址,会递增,随时指向buffer当前的空闲地址,实际上没有用到
   __u32				free_size;  //初始记录向后端申请到的buffer的总长,随后是剩余的长度,实际上没有用到
   __s32				 pid;
   __pCBK_t              RequestDataCB; //dmx的使用者注册, dvb_dmx_chan_param_t->request_buffer_cb, 真正调用是在push_ts_data()里
   __pCBK_t              UpdateDateCB;  //dmx的使用者注册, dvb_dmx_chan_param_t->update_data_cb
   F_PUSH_TS_DATA	    push_ts_data; //push_ts_data()
   
}__tsdata_filter_t;

#if 0
//* define MpegTSFilter
typedef struct MPEGTSFILTER     //连接硬件filter和虚channel的数据结构
{
    __s32               pid;
    __s32               chan;   //channel_id,目前就是硬件filter的编号.
    dvb_dmx_type_t      dmxType;

    __mp*               tscDrv;     //tscdrv的句柄
    __krnl_event_t*     dataSem;    //filter通道的semaphore, tscdrv在中断来时post, filtertask会检测并处理.一个硬件filter对应一个semaphore.虚channel必须共用一个semaphore!
    __u8                filterTaskId;   //filtertask的id号, pes_main_task()或者section_main_task()
    
    union 
    {
        __pes_filter_t     pes_filter;      //video, audio, subtitle.
        __section_filter_t section_filter;  //一般用于SI表
    }u; //将来如果增加虚channel的概念,这里就是一个虚channel所对应的默认的处理函数.拼pes包或者si section, DMX_TYPE_VIDEO和DMX_TYPE_SECTION等

}__filter_t;    //目前实channel和虚channel没有区分. 如果多个channel取的pid是一样的, 一个硬件filter应该可以对应多个虚channel,.
#endif

#define VCHANCNT    (4)
typedef struct tag_CVirtualChannel
{
    __s32               nVirtualChanId;    //虚通道的id号.决定nVirtualChanId = chan <<16 | 0,1,2,3...
    dvb_dmx_type_t      dmxType;    //虚通道, DMX_TYPE_VIDEO, PID_FILTER_TYPE_VIDEO, pid_filter_type_t, DMX_TYPE_NONE表示没有使用.
    __s32               nRequestCloseFlag; //要求本虚通道关闭的标记.置1表示要求关闭, 关闭后清0.
    union 
    {
        __pes_filter_t     pes_filter;      //video, audio, subtitle.
        __section_filter_t section_filter;  //一般用于SI表
        __tsdata_filter_t  tsdata_filter;   //直接处理ts packet.
    }vfilter; //虚通道,这里就是一个虚channel所对应的默认的处理函数.拼pes包或者si section, DMX_TYPE_VIDEO和DMX_TYPE_SECTION等
}CVirtualChannel;
//* define MpegTSFilter
typedef struct MPEGTSFILTERV2     //连接硬件filter和虚channel的数据结构
{
    __s32               pid;    //实通道pid
    __s32               chan;   //实通道, channel_id,目前就是硬件filter的编号.

    __mp*               tscDrv;     //实通道,tscdrv的句柄,
    __krnl_event_t*     dataSem;    //实通道,filter通道的semaphore, tscdrv在中断来时post, filtertask会检测并处理.一个硬件filter对应一个semaphore.虚channel必须共用一个semaphore!
    __u8                filterTaskId;   //实通道, filtertask的id号, pes_main_task()或者section_main_task()

    CVirtualChannel     vChanArray[VCHANCNT];  //一个实通道对应4个虚通道.

}__filter_v2_t;    //目前实channel和虚channel没有区分. 如果多个channel取的pid是一样的, 一个硬件filter应该可以对应多个虚channel.

typedef struct MPEGTSCONTEXT 
{
    __u32           mid;
    __u32           PCRPid;
    __u32           detectPCR;  //表示pcr检测是否打开
    __pCBK_t        pcrCallback;

    __krnl_event_t* mutex;  //操作filter时需要互斥,主要是对__filter_t*     pids[NB_PID_MAX]进行互斥

    __mp*           drvTsc; //控制tsc驱动
	__mp*			drvDemod;//传递给TSC的DEMOD驱动句柄
    //__filter_t*     pids[NB_PID_MAX];   //支持8192个filter,足够了, 约定下标号就是pid.
    __filter_v2_t*     pids[NB_PID_MAX];   //支持8192个filter,足够了, 约定下标号就是pid.

    CFEConfig       *pFEConfig;

}__mpegts_context_t;


__filter_v2_t* OpenFilter(__u32 pid, dvb_dmx_chan_param_t* par, __mpegts_context_t* ctx);

void CloseFilter(__filter_v2_t* filter, __s32 nVirtualChanId, __mpegts_context_t* ctx);

void CloseEntireFilter(__filter_v2_t* filter, __mpegts_context_t* ctx);

void OpenPCRDetect(__u32 pid, __mpegts_context_t* ctx);

void ClosePCRDetect(__u32 pid, __mpegts_context_t* ctx);


__s32 ReadTsPackets(__filter_v2_t* filter, __u8* buf, __u32 size);

#endif

