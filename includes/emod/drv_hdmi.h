
#ifndef __DRV_HDMI_H__
#define __DRV_HDMI_H__

typedef struct hdmi_audio
{
	__u8    hw_intf;        /* 0:iis  1:spdif 2:pcm */
	__u16	fs_between;     /* fs */
	__u32   sample_rate;    /*sample rate*/
	__u8    clk_edge;       /* 0:*/
	__u8    ch0_en;         /* 1 */
	__u8    ch1_en;         /* 0 */
	__u8 	ch2_en;         /* 0 */
	__u8 	ch3_en;         /* 0 */
	__u8	word_length;    /* 32 */
	__u8    shift_ctl;      /* 0 */
	__u8    dir_ctl;        /* 0 */
	__u8    ws_pol;
	__u8    just_pol;
	__u32	channel_num;
}hdmi_audio_t;

typedef enum
{
    HDMI_CMD_SET_VIDEO_MOD,
    HDMI_CMD_GET_VIDEO_MOD,
    HDMI_CMD_OPEN,
    HDMI_CMD_CLOSE,
    HDMI_CMD_MOD_SUPPORT,
    HDMI_CMD_GET_HPD_STATUS,
    HDMI_CMD_AUDIO_ENABLE,
    HDMI_CMD_SET_AUDIO_PARA,
    HDMI_CMD_SET_PLL
}__hdmi_cmd_t;

#endif
