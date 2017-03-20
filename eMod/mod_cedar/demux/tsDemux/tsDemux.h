
#ifndef _TS_DEMUX_H
#define _TS_DEMUX_H

//#include "mod_cedar_i.h"
//#include "tsDemux_interface.h"
//#include "tsc_interface.h"

#define AV_RB16(p)  ((*(p))<<8 | (*((p)+1)))
#define AV_RB32(p)  ((*(p))<<24 | (*((p)+1))<<16 | (*((p)+2))<<8 | (*((p)+3)))

#define NB_PID_MAX                  8192
#define MAX_SECTION_SIZE            4096    //EIT section�����4096�ֽ�
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


typedef struct PESFILTER    //��Ӧһ����ͨ����ƴpes����filter.
{
    __u8*				cur_ptr;    //copy���ݵ�Ŀ�ĵ�ַ, �Ӻ����vdec�ȹ���.
    __u32				free_size;  //cur_ptr��ʣ��ռ�
    __u32				valid_size; //��¼pes����ȥ����ͷ���ݵ�copy��Ŀ�ĵ�ַ�Ļ�û�д��������������PES_packet_data_byte������.ES
    __u32               payload_size;   //* pes packet payload size.��¼��ǰ��û���ͳ�ȥ��payload���ֽ���
    __u32				ctrl_bits;
    __u32               rap_flag;       //* random access point flag
    
    __md_data_info_t    data_info;
    __md_buf_t          md_buf;

    __pCBK_t            RequestDataCB;
    __pCBK_t            UpdateDateCB;

    __s32				pid;
    __mpegts_state_t	state;  //��ʼ��ʱΪMPEGTS_HEADER.����handle_pkt_payload()�ж�,�������header,��skip.�տ�ʼ������ʱһ�㲻��ǡ���յ�header,��ô��Ҫskip.
    
    /* used to get the format */
		__u8				pts_dts_flag;
    __s64            	pts;    //ֱ�Ӵ���������������ֵ,����ֱ�ӵ�ʱ��,/90������msΪ��λ��ʱ��.��Ϊ������27M/300��
    __s64						pre_pts;
    __s64               pts_to_send;
    F_PUSH_AVPES_DATA	    push_avpes_data;  //push_avpes_data()  ����Ƶ����
    F_PUSH_SUBPES_DATA	    push_subpes_data;  //push_subpes_data()  ��Ļ����
    __s32          		data_index; //pes����ǰ�Ķ�ָ��ָ��ĵ�ַ
    __s32            	total_size; //pes�����ܳ���. Ϊ0��ʾpes��û�б߽�.
    __s32             pes_header_size;    //Ҳ����pes����ʼ��9�ֽ�
    __u8            	header[MAX_PES_HEADER_SIZE];

}__pes_filter_t;



//* define MpegTSSectionFilter

typedef struct SECTIONFILTER    //��Ӧһ����ͨ����ƴsi section��filter.
{
    __u32       need_start; //��1��ʾ��Ҫһ�������Ŀ�ʼ.��ʼ��1.ʵ�������ý����Ǳ�ʾfilter�ոտ�ʼȡ����,������.
    __s32       section_index;  //��¼��ǰsection���Ѿ�copy���ֽ���
    __s32       section_h_size; //section�ܵ�size.section_length + 3
   __md_buf_t   md_buf; //��������buffer�����ݽṹ.push_section_data().
    __s32       check_crc;
    __s32       end_of_section_reached; //�������section�Ƿ����.
    __s32       last_cc;    //ָ����ts���е�continuity_couner��ֵ,��һ������. -1��ʾû����һ����,�����ǵ�һ����.
    

   __u8*				cur_ptr;    //��ʼ��¼�������뵽��buffer����ʼ��ַ,�����,��ʱָ��buffer��ǰ�Ŀ��е�ַ
   __u32				free_size;  //��ʼ��¼�������뵽��buffer���ܳ�,�����ʣ��ĳ���
   __s32				 pid;
   __s32				 servId;		//����PMTʱ��������
   __pCBK_t              RequestDataCB; //dmx��ʹ����ע��, dvb_dmx_chan_param_t->request_buffer_cb, ������������push_section_data()��
   __pCBK_t              UpdateDateCB;  //dmx��ʹ����ע��, dvb_dmx_chan_param_t->update_data_cb
   F_PUSH_SECTION_DATA	 push_section_data; //push_section_data()
   
}__section_filter_t;

//* define MpegTSRawDataFilter
typedef struct tag_TSDATAFILTER    //��Ӧһ����ͨ����ֱ�Ӵ���ts packet��filter.
{
   __md_buf_t   md_buf; //��������buffer�����ݽṹ.push_ts_data().ֱ������ts packet.

   __u8*				cur_ptr;    //��ʼ��¼�������뵽��buffer����ʼ��ַ,�����,��ʱָ��buffer��ǰ�Ŀ��е�ַ,ʵ����û���õ�
   __u32				free_size;  //��ʼ��¼�������뵽��buffer���ܳ�,�����ʣ��ĳ���,ʵ����û���õ�
   __s32				 pid;
   __pCBK_t              RequestDataCB; //dmx��ʹ����ע��, dvb_dmx_chan_param_t->request_buffer_cb, ������������push_ts_data()��
   __pCBK_t              UpdateDateCB;  //dmx��ʹ����ע��, dvb_dmx_chan_param_t->update_data_cb
   F_PUSH_TS_DATA	    push_ts_data; //push_ts_data()
   
}__tsdata_filter_t;

#if 0
//* define MpegTSFilter
typedef struct MPEGTSFILTER     //����Ӳ��filter����channel�����ݽṹ
{
    __s32               pid;
    __s32               chan;   //channel_id,Ŀǰ����Ӳ��filter�ı��.
    dvb_dmx_type_t      dmxType;

    __mp*               tscDrv;     //tscdrv�ľ��
    __krnl_event_t*     dataSem;    //filterͨ����semaphore, tscdrv���ж���ʱpost, filtertask���Ⲣ����.һ��Ӳ��filter��Ӧһ��semaphore.��channel���빲��һ��semaphore!
    __u8                filterTaskId;   //filtertask��id��, pes_main_task()����section_main_task()
    
    union 
    {
        __pes_filter_t     pes_filter;      //video, audio, subtitle.
        __section_filter_t section_filter;  //һ������SI��
    }u; //�������������channel�ĸ���,�������һ����channel����Ӧ��Ĭ�ϵĴ�����.ƴpes������si section, DMX_TYPE_VIDEO��DMX_TYPE_SECTION��

}__filter_t;    //Ŀǰʵchannel����channelû������. ������channelȡ��pid��һ����, һ��Ӳ��filterӦ�ÿ��Զ�Ӧ�����channel,.
#endif

#define VCHANCNT    (4)
typedef struct tag_CVirtualChannel
{
    __s32               nVirtualChanId;    //��ͨ����id��.����nVirtualChanId = chan <<16 | 0,1,2,3...
    dvb_dmx_type_t      dmxType;    //��ͨ��, DMX_TYPE_VIDEO, PID_FILTER_TYPE_VIDEO, pid_filter_type_t, DMX_TYPE_NONE��ʾû��ʹ��.
    __s32               nRequestCloseFlag; //Ҫ����ͨ���رյı��.��1��ʾҪ��ر�, �رպ���0.
    union 
    {
        __pes_filter_t     pes_filter;      //video, audio, subtitle.
        __section_filter_t section_filter;  //һ������SI��
        __tsdata_filter_t  tsdata_filter;   //ֱ�Ӵ���ts packet.
    }vfilter; //��ͨ��,�������һ����channel����Ӧ��Ĭ�ϵĴ�����.ƴpes������si section, DMX_TYPE_VIDEO��DMX_TYPE_SECTION��
}CVirtualChannel;
//* define MpegTSFilter
typedef struct MPEGTSFILTERV2     //����Ӳ��filter����channel�����ݽṹ
{
    __s32               pid;    //ʵͨ��pid
    __s32               chan;   //ʵͨ��, channel_id,Ŀǰ����Ӳ��filter�ı��.

    __mp*               tscDrv;     //ʵͨ��,tscdrv�ľ��,
    __krnl_event_t*     dataSem;    //ʵͨ��,filterͨ����semaphore, tscdrv���ж���ʱpost, filtertask���Ⲣ����.һ��Ӳ��filter��Ӧһ��semaphore.��channel���빲��һ��semaphore!
    __u8                filterTaskId;   //ʵͨ��, filtertask��id��, pes_main_task()����section_main_task()

    CVirtualChannel     vChanArray[VCHANCNT];  //һ��ʵͨ����Ӧ4����ͨ��.

}__filter_v2_t;    //Ŀǰʵchannel����channelû������. ������channelȡ��pid��һ����, һ��Ӳ��filterӦ�ÿ��Զ�Ӧ�����channel.

typedef struct MPEGTSCONTEXT 
{
    __u32           mid;
    __u32           PCRPid;
    __u32           detectPCR;  //��ʾpcr����Ƿ��
    __pCBK_t        pcrCallback;

    __krnl_event_t* mutex;  //����filterʱ��Ҫ����,��Ҫ�Ƕ�__filter_t*     pids[NB_PID_MAX]���л���

    __mp*           drvTsc; //����tsc����
	__mp*			drvDemod;//���ݸ�TSC��DEMOD�������
    //__filter_t*     pids[NB_PID_MAX];   //֧��8192��filter,�㹻��, Լ���±�ž���pid.
    __filter_v2_t*     pids[NB_PID_MAX];   //֧��8192��filter,�㹻��, Լ���±�ž���pid.

    CFEConfig       *pFEConfig;

}__mpegts_context_t;


__filter_v2_t* OpenFilter(__u32 pid, dvb_dmx_chan_param_t* par, __mpegts_context_t* ctx);

void CloseFilter(__filter_v2_t* filter, __s32 nVirtualChanId, __mpegts_context_t* ctx);

void CloseEntireFilter(__filter_v2_t* filter, __mpegts_context_t* ctx);

void OpenPCRDetect(__u32 pid, __mpegts_context_t* ctx);

void ClosePCRDetect(__u32 pid, __mpegts_context_t* ctx);


__s32 ReadTsPackets(__filter_v2_t* filter, __u8* buf, __u32 size);

#endif

