
#ifndef _TS_DEMUX_INTERFACE_H
#define _TS_DEMUX_INTERFACE_H

//#include "maple_common.h"
#include "dmx_data.h"

#define TS_DEMUX_PLUGIN_FILE_PATH  CEDAR_PLUGINS_DIRECTORY"demux.mod"

typedef enum DVB_DEMUX_TYPE
{
    DMX_TYPE_NONE,  //û������,������ʾû��ʹ��.
    
    DMX_TYPE_VIDEO, //ƴpes��,��pes��Ϊ��λ����
    DMX_TYPE_AUDIO,
    DMX_TYPE_SUBTITLE,
    DMX_TYPE_SECTION,   //ƴsi section,��sectionΪ��λ����.

    DMX_TYPE_TS_DATA_VIDEO,     //video����,���ǲ�ƴpes��,ֱ�Ӵ���
    DMX_TYPE_TS_DATA_AUDIO,
    DMX_TYPE_TS_DATA_SUBTITLE,
    DMX_TYPE_TS_DATA_SECTION,   //������ͨ��si���ts��.��ƴsection,ֱ�Ӵ���.
    DMX_TYPE_TS_DATA,   //����.
}dvb_dmx_type_t;


typedef struct DVB_DEMUX_CHAN_PARAM
{
    dvb_dmx_type_t  dmx_type;           //* [in],demux type for different type of tv data. DMX_TYPE_SECTION
    __pCBK_t        request_buffer_cb;  //* [in],request buffer callback function for video/audio/subtitle demux type. dmx��ʹ����ע��. ����һ����__md_buf_t*
    __pCBK_t        update_data_cb;     //* [in],update data callback function for video/audio/subtitle demux type.dmx��ʹ����ע��. ����һ����__u32*, ��ʾ��Ч���ݳ���
    __u32			servId;							//����PMT��ʱ�õ�
    __s32           nVirtualChanId;    //[out] ���ظ�������,��ͨ����id��.����nVirtualChanId = chan <<16 | 0,1,2,3...
}dvb_dmx_chan_param_t;


typedef enum DVB_DEMUX_CMD
{
    DVB_DMX_CMD_OPEN_FILTER,          //* open one filter to get section or pes data.
                                    //* aux = pid, pbuffer = pointer to __dmx_chan_param;������ͨ����id����dvb_dmx_chan_param_t�з���.
                                    
    DVB_DMX_CMD_CLOSE_FILTER,         //* close filter.
                                    //* aux = pid, pbuffer = nVirtualChanId;��У��.
                                    
    DVB_DMX_CMD_OPEN_PCR_DETECT,      //* open PCR detection.
                                    //* aux = pid, pbuffer = pcr notify callback function
                                    
    DVB_DMX_CMD_CLOSE_PCR_DETECT,     //* close PCR detection.
                                    //* aux = 0, pbuffer = NULL

    DVB_DMX_CMD_SET_FECONFIG,       //����ǰ�˵��������, pbuffer = (CFEConfig*);

    DVB_DMX_CMD_OPEN,               // DMX��IO_Open, IO_Close��ʱ����Ҫ
    
		DVB_DMX_CMD_SET_DEMODDRV,      // ��MAPLE���ش���������DEMOD�����ľ�����ٴ��ݸ�TSCģ�顣
	
    DVB_DMX_CMD_NUM

}dvb_demux_cmd_t;

#endif

