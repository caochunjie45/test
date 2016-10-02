
#ifndef _TS_DEMUX_INTERFACE_H
#define _TS_DEMUX_INTERFACE_H

//#include "maple_common.h"
#include "dmx_data.h"

#define TS_DEMUX_PLUGIN_FILE_PATH  CEDAR_PLUGINS_DIRECTORY"demux.mod"

typedef enum DVB_DEMUX_TYPE
{
    DMX_TYPE_NONE,  //没有类型,用来表示没有使用.
    
    DMX_TYPE_VIDEO, //拼pes包,以pes包为单位传输
    DMX_TYPE_AUDIO,
    DMX_TYPE_SUBTITLE,
    DMX_TYPE_SECTION,   //拼si section,以section为单位传输.

    DMX_TYPE_TS_DATA_VIDEO,     //video数据,但是不拼pes包,直接传输
    DMX_TYPE_TS_DATA_AUDIO,
    DMX_TYPE_TS_DATA_SUBTITLE,
    DMX_TYPE_TS_DATA_SECTION,   //当做普通的si表的ts包.不拼section,直接传输.
    DMX_TYPE_TS_DATA,   //其他.
}dvb_dmx_type_t;


typedef struct DVB_DEMUX_CHAN_PARAM
{
    dvb_dmx_type_t  dmx_type;           //* [in],demux type for different type of tv data. DMX_TYPE_SECTION
    __pCBK_t        request_buffer_cb;  //* [in],request buffer callback function for video/audio/subtitle demux type. dmx的使用者注册. 参数一般是__md_buf_t*
    __pCBK_t        update_data_cb;     //* [in],update data callback function for video/audio/subtitle demux type.dmx的使用者注册. 参数一般是__u32*, 表示有效数据长度
    __u32			servId;							//搜索PMT表时用到
    __s32           nVirtualChanId;    //[out] 返回给调用者,虚通道的id号.决定nVirtualChanId = chan <<16 | 0,1,2,3...
}dvb_dmx_chan_param_t;


typedef enum DVB_DEMUX_CMD
{
    DVB_DMX_CMD_OPEN_FILTER,          //* open one filter to get section or pes data.
                                    //* aux = pid, pbuffer = pointer to __dmx_chan_param;并且虚通道的id号在dvb_dmx_chan_param_t中返回.
                                    
    DVB_DMX_CMD_CLOSE_FILTER,         //* close filter.
                                    //* aux = pid, pbuffer = nVirtualChanId;会校验.
                                    
    DVB_DMX_CMD_OPEN_PCR_DETECT,      //* open PCR detection.
                                    //* aux = pid, pbuffer = pcr notify callback function
                                    
    DVB_DMX_CMD_CLOSE_PCR_DETECT,     //* close PCR detection.
                                    //* aux = 0, pbuffer = NULL

    DVB_DMX_CMD_SET_FECONFIG,       //设置前端的配置情况, pbuffer = (CFEConfig*);

    DVB_DMX_CMD_OPEN,               // DMX的IO_Open, IO_Close暂时不需要
    
		DVB_DMX_CMD_SET_DEMODDRV,      // 由MAPLE主控传递下来的DEMOD驱动的句柄，再传递给TSC模块。
	
    DVB_DMX_CMD_NUM

}dvb_demux_cmd_t;

#endif

