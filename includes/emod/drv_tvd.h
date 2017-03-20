/*
*********************************************************************************************************
*                                                    MELIS
*                                    the Easy Portable/Player Develop Kits
*                                                 TVD Driver
*
*                                    (c) Copyright 2012-2016, Kingvan.Tong China
*                                             All Rights Reserved
*
* File    : drv_tvd.h
* By      : Kingvan
* Version : v1.0
* Date    : 2012-9-27
* Descript:
* Update  : date                auther         ver     notes
*           2012-9-27 11:09:13  Kingvan.Tong   2.0     build the file.
*********************************************************************************************************
*/
#ifndef __DRIVER_TVD_H__
#define __DRIVER_TVD_H__


#define TVD_BUFFER_NUM 6


typedef struct tag_TVD_MODE
{
    __rectsz_t          size;

    __pixel_yuvfmt_t    color_format; //__pixel_yuvfmt_t
    __u32               component_seq;//__yuv_seq_t + __bayer_seq_t
    __u32               store_mode;   //__yuv_mod_t + __bayer_store_mode_t

}__tvd_mode_t;

typedef struct tag_TVD_FRAME
{
    __s32               frame_id;
    
    __rect_t            src_rect;           // source valid size, �������ͼ���С, һ�����frame buffer��
    __rect_t            dst_rect;           // source display size,��ʵͼ���λ�ô�С
    __u32               addr[3];            // data buffer address
    
    __u16               color_format; //same with __tvd_mode_t
    __u32               component_seq;//same with __tvd_mode_t
    __u32               store_mode;   //same with __tvd_mode_t
    __u16               frame_rate;   //same with __tvd_mode_t

    
}__tvd_frame_t;

typedef struct tag_TVD_FRAME_QUEUE
{
    __s32           num;    //��Ч��֡������num <= TVD_BUFFER_NUM
    __tvd_frame_t   tvd_frame_array[TVD_BUFFER_NUM];
} __tvd_frame_queue_t;

typedef enum  e_TVD_SEL_CHANNEL
{
	TVD_CHANNEL_0,
	TVD_CHANNEL_1,
	TVD_CHANNEL_2,
	TVD_CHANNEL_3,

	TVD_SEL_CHANNEL_MAX

}__drv_TVD_sel_channel;

typedef enum  e_TVD_SEL_SOURCE
{
	TVD_SOURCE_CVBS,
	TVD_SOURCE_YPbPr,

	TVD_SEL_SOURCE_MAX

}__drv_TVD_sel_source;

typedef enum  e_TVD_PAL_NTSC
{
	TVD_SOURCE_NTSC,
	TVD_SOURCE_PAL,

	TVD_PAL_NTSC_MAX

}__drv_TVD_pal_ntsc;

typedef enum  e_TVD_YPbPr_SIZE
{
	TVD_YPbPr_480i,
	TVD_YPbPr_576i,

	TVD_YPbPr_SIZE_MAX

}__drv_TVD_YPbPr_size;

typedef enum  __DRV_TVD_CMD_SET
{
    DRV_TVD_CMD_NONE = 0,
    DRV_TVD_CMD_INIT, 						//ret = EPDK_OK/EPDK_FAIL
    DRV_TVD_CMD_EXIT, 						//ret = EPDK_OK/EPDK_FAIL
    DRV_TVD_CMD_SET_MODE, 					//pbuffer = __csi_mode_t*, ret = EPDK_OK/EPDK_FAIL
    DRV_TVD_CMD_GET_MODE, 					//pbuffer = __csi_mode_t*, ret = EPDK_OK/EPDK_FAIL
    DRV_TVD_CMD_GET_MODE_ALL, 				//pbuffer = __csi_mode_total_t *
    DRV_TVD_CMD_CAPTURE_ON,					//ret = EPDK_OK/EPDK_FAIL
    DRV_TVD_CMD_CAPTURE_OFF,  				//ret = EPDK_OK/EPDK_FAIL
    DRV_TVD_CMD_REQUEST_FRAME, 				//pbuffer= (__csi_frame_t**), ret = EPDK_OK/EPDK_FAIL
    DRV_TVD_CMD_RELEASE_FRAME, 				//aux = frame_id, ret = EPDK_OK/EPDK_FAIL
    DRV_TVD_CMD_SET_PTS, 					//**absence**//aux = pts(__u32)
    DRV_TVD_CMD_GET_PTS, 					//**absence**//ret=pts(__u32)
    DRV_TVD_CMD_SET_PTS_MODE,   			//**absence**//����PTS�Ļ�ȡ��ʽ��aux = 0:CSI�Լ���PTS; 1:ͨ���ص������õ�PTS; 2.����ҪPTS
    DRV_TVD_CMD_INSTALL_CALLBACK_GET_PTS, 	//**absence**//ע��ص�����,�õ�PTS����callback��ԭ��ΪCB_GetPTS,����*arg��__s64*��ָ��,��ʾPTS����λus. *pbuffer = callback
    DRV_TVD_CMD_SET_FRAME_QUEUE,        	//����frame queue.������capture_on֮ǰ���á���Ϊcapture off�����. pbuffer = __csi_frame_queue_t*,��Ҫ������buffer��frame_id�������Ĳ���������csidrv����.
    DRV_TVD_CMD_SET_COLOR_EFFECT,			//**absence**//aux = color effet
    DRV_TVD_CMD_GET_COLOR_EFFECT,			//**absence**//ret = color effet
    DRV_TVD_CMD_SET_AWB,					//**absence**//aux = awb
    DRV_TVD_CMD_GET_AWB,					//**absence**//ret = awb
    DRV_TVD_CMD_SET_AE,						//**absence**//aux = ae
    DRV_TVD_CMD_GET_AE,						//**absence**//ret = ae
    DRV_TVD_CMD_SET_BRIGHT,					//**absence**//aux = bright
    DRV_TVD_CMD_GET_BRIGHT,					//**absence**//ret = bright
    DRV_TVD_CMD_SET_CONTRAST,				//**absence**//aux = contrast
    DRV_TVD_CMD_GET_CONTRAST,				//**absence**//ret = contrast
    DRV_TVD_CMD_SET_BAND,					//**absence**//aux = band
    DRV_TVD_CMD_GET_BAND,					//**absence**//ret = band//add 2011-8-3 13:13:13
    DRV_TVD_CMD_GET_PROGRSSV_INFO,			//**absence**//ret = b_progr
    DRV_TVD_CMD_SET_MASTER_PORT = DRV_TVD_CMD_GET_PROGRSSV_INFO,//**absence**
    DRV_TVD_CMD_GET_MASTER_PORT,			//**absence**


    DRV_TVD_CMD_SEL_CHANNEL,				//aux = __drv_TVD_sel_channel
    DRV_TVD_CMD_SEL_SOURCE,					//aux = TVD_SOURCE_CVBS, TVD_SOURCE_YPbPr
    DRV_TVD_CMD_PAL_NTSC,					//if TVD_SOURCE_CVBS, aux = __drv_TVD_pal_ntsc
    DRV_TVD_CMD_YPbPr_SIZE,					//if TVD_SOURCE_YPbPr, aux = __drv_TVD_YPbPr_size

    DRV_TVD_CMD_,


}__drv_TVD_cmd_set_t;

#endif	// __DRIVER_TVD_H__

