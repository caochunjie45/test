/*
*********************************************************************************************************
*                                                    MELIS
*                                    the Easy Portable/Player Develop Kits
*                                                Display Driver
*
*                                    (c) Copyright 2011-2014, tyle China
*                                             All Rights Reserved
*
* File    : drv_display.h
* By      : tyle
* Version : v1.0
* Date    : 2011-12-23
* Descript: display driver head file.
* Update  : date                auther      ver     notes
*           2011-12-23 15:07    tyle        1.0     Create this file.
*********************************************************************************************************
*/

#ifndef __DRV_DISPLAY_H__
#define __DRV_DISPLAY_H__

typedef enum
{
    FB_MODE_SCREEN0 = 0,
    FB_MODE_SCREEN1 = 1,
    FB_MODE_DUAL_SAME_SCREEN_TB = 2,//two screen, top buffer for screen0, bottom buffer for screen1
    FB_MODE_DUAL_DIFF_SCREEN_SAME_CONTENTS = 3,//two screen, they have same contents;
}__fb_mode_t;

typedef struct
{
	__fb_mode_t                 fb_mode;
	__disp_layer_work_mode_t    mode;
	__u32                       buffer_num;
	__u32                       width;
	__u32                       height;
	
	__u32                       output_width;//used when scaler mode 
	__u32                       output_height;//used when scaler mode
	
	__u32                       primary_screen_id;//used when FB_MODE_DUAL_DIFF_SCREEN_SAME_CONTENTS
	__u32                       aux_output_width;//used when FB_MODE_DUAL_DIFF_SCREEN_SAME_CONTENTS
	__u32                       aux_output_height;//used when FB_MODE_DUAL_DIFF_SCREEN_SAME_CONTENTS

//maybe not used anymore
	__u32                       line_length;//in byte unit
	__u32                       smem_len;
	__u32                       ch1_offset;//use when PLANAR or UV_COMBINED mode
	__u32                       ch2_offset;//use when PLANAR mode
}__disp_fb_create_para_t;

typedef enum
{
    DISP_INIT_MODE_SCREEN0 = 0,//fb0 for screen0
    DISP_INIT_MODE_SCREEN1 = 1,//fb0 for screen1
    DISP_INIT_MODE_TWO_DIFF_SCREEN = 2,//fb0 for screen0 and fb1 for screen1
    DISP_INIT_MODE_TWO_SAME_SCREEN = 3,//fb0(up buffer for screen0, down buffer for screen1)
    DISP_INIT_MODE_TWO_DIFF_SCREEN_SAME_CONTENTS = 4,//fb0 for two different screen(screen0 layer is normal layer, screen1 layer is scaler layer);
}__disp_init_mode_t;


typedef struct
{
    __bool                  b_init;
    __disp_init_mode_t      disp_mode;//0:single screen0(fb0); 1:single screen1(fb0);  2:dual diff screen(fb0, fb1); 3:dual same screen(fb0 up and down); 4:dual diff screen same contents(fb0)

    //for screen0 and screen1
    __disp_output_type_t    output_type[2];
    __disp_tv_mode_t        tv_mode[2];
    __disp_vga_mode_t       vga_mode[2];

    //for fb0 and fb1
    __u32                   buffer_num[2];
    __bool                  scaler_mode[2];
    __disp_pixel_fmt_t      format[2];
    __disp_pixel_seq_t      seq[2];
    __bool                  br_swap[2];
}__disp_init_t;


typedef enum tag_DISP_CMD
{
//----disp global----
    DISP_CMD_RESERVE0 = 0x00,
    DISP_CMD_RESERVE1 = 0x01,
    DISP_CMD_SET_BKCOLOR = 0x3f,//fail when the value is 0x02 in linux,why???
    DISP_CMD_GET_BKCOLOR = 0x03,
    DISP_CMD_SET_COLORKEY = 0x04,
    DISP_CMD_GET_COLORKEY = 0x05,
    DISP_CMD_SET_PALETTE_TBL = 0x06,
    DISP_CMD_GET_PALETTE_TBL = 0x07,
    DISP_CMD_SCN_GET_WIDTH = 0x08,
    DISP_CMD_SCN_GET_HEIGHT = 0x09,
    DISP_CMD_GET_OUTPUT_TYPE = 0x0a,
    DISP_CMD_SET_EXIT_MODE = 0x0c,
    DISP_CMD_SET_GAMMA_TABLE = 0x0d,
    DISP_CMD_GAMMA_CORRECTION_ON = 0x0e,
    DISP_CMD_GAMMA_CORRECTION_OFF = 0x0f,
    DISP_CMD_START_CMD_CACHE =0x10,
    DISP_CMD_EXECUTE_CMD_AND_STOP_CACHE = 0x11,
    DISP_CMD_SET_BRIGHT = 0x12,
    DISP_CMD_SET_CONTRAST = 0x13,
    DISP_CMD_SET_SATURATION = 0x14,
    DISP_CMD_GET_BRIGHT = 0x16,
    DISP_CMD_GET_CONTRAST = 0x17,
    DISP_CMD_GET_SATURATION = 0x18,
    DISP_CMD_ENHANCE_ON = 0x1a,
    DISP_CMD_ENHANCE_OFF = 0x1b,
    DISP_CMD_GET_ENHANCE_EN = 0x1c,
    DISP_CMD_CLK_ON = 0x1d,
    DISP_CMD_CLK_OFF = 0x1e,
    DISP_CMD_SET_SCREEN_SIZE = 0x1f,//when the screen is not used to display(lcd/tv/vga/hdmi) directly, maybe capture the screen and scaler to dram, or as a layer of another screen
    DISP_CMD_CAPTURE_SCREEN = 0x20,//caputre screen and scaler to dram
    DISP_CMD_DE_FLICKER_ON = 0x21,
    DISP_CMD_DE_FLICKER_OFF = 0x22,
    DISP_CMD_GET_DE_FLICKER_EN = 0x23,
    DISP_CMD_DRC_ON = 0x24,
    DISP_CMD_DRC_OFF = 0x25,
    DISP_CMD_GET_DRC_EN = 0x26,
    DISP_CMD_DE_FLICKER_SET_WINDOW = 0x27,
    DISP_CMD_DRC_SET_WINDOW = 0x28,
    DISP_CMD_SET_HUE = 0x29,
    DISP_CMD_GET_HUE = 0x2a,
    DISP_CMD_SET_DE_FLICKER=0x2b,

//----layer----
    DISP_CMD_LAYER_REQUEST = 0x40,
    DISP_CMD_LAYER_RELEASE = 0x41,
    DISP_CMD_LAYER_OPEN = 0x42,
    DISP_CMD_LAYER_CLOSE = 0x43,
    DISP_CMD_LAYER_SET_FB = 0x44,
    DISP_CMD_LAYER_GET_FB = 0x45,
    DISP_CMD_LAYER_SET_SRC_WINDOW = 0x46,
    DISP_CMD_LAYER_GET_SRC_WINDOW = 0x47,
    DISP_CMD_LAYER_SET_SCN_WINDOW = 0x48,
    DISP_CMD_LAYER_GET_SCN_WINDOW = 0x49,
    DISP_CMD_LAYER_SET_PARA = 0x4a,
    DISP_CMD_LAYER_GET_PARA = 0x4b,
    DISP_CMD_LAYER_ALPHA_ON = 0x4c,
    DISP_CMD_LAYER_ALPHA_OFF = 0x4d,
    DISP_CMD_LAYER_GET_ALPHA_EN = 0x4e,
    DISP_CMD_LAYER_SET_ALPHA_VALUE = 0x4f,
    DISP_CMD_LAYER_GET_ALPHA_VALUE = 0x50,
    DISP_CMD_LAYER_CK_ON = 0x51,
    DISP_CMD_LAYER_CK_OFF = 0x52,
    DISP_CMD_LAYER_GET_CK_EN = 0x53,
    DISP_CMD_LAYER_SET_PIPE = 0x54,
    DISP_CMD_LAYER_GET_PIPE = 0x55,
    DISP_CMD_LAYER_TOP = 0x56,
    DISP_CMD_LAYER_BOTTOM = 0x57,
    DISP_CMD_LAYER_GET_PRIO = 0x58,
    DISP_CMD_LAYER_SET_SMOOTH = 0x59,
    DISP_CMD_LAYER_GET_SMOOTH = 0x5a,
    DISP_CMD_LAYER_SET_BRIGHT = 0x5b,//亮度
    DISP_CMD_LAYER_SET_CONTRAST = 0x5c,//对比度
    DISP_CMD_LAYER_SET_SATURATION = 0x5d,//饱和度
    DISP_CMD_LAYER_SET_HUE = 0x5e,//色调,色度
    DISP_CMD_LAYER_GET_BRIGHT = 0x5f,
    DISP_CMD_LAYER_GET_CONTRAST = 0x60,
    DISP_CMD_LAYER_GET_SATURATION = 0x61,
    DISP_CMD_LAYER_GET_HUE = 0x62,
    DISP_CMD_LAYER_ENHANCE_ON = 0x63,
    DISP_CMD_LAYER_ENHANCE_OFF = 0x64,
    DISP_CMD_LAYER_GET_ENHANCE_EN = 0x65,
    DISP_CMD_LAYER_VPP_ON = 0x67,
    DISP_CMD_LAYER_VPP_OFF = 0x68,
    DISP_CMD_LAYER_GET_VPP_EN = 0x69,
    DISP_CMD_LAYER_SET_LUMA_SHARP_LEVEL = 0x6a,
    DISP_CMD_LAYER_GET_LUMA_SHARP_LEVEL = 0x6b,
    DISP_CMD_LAYER_SET_CHROMA_SHARP_LEVEL = 0x6c,
    DISP_CMD_LAYER_GET_CHROMA_SHARP_LEVEL = 0x6d,
    DISP_CMD_LAYER_SET_WHITE_EXTEN_LEVEL = 0x6e,
    DISP_CMD_LAYER_GET_WHITE_EXTEN_LEVEL = 0x6f,
    DISP_CMD_LAYER_SET_BLACK_EXTEN_LEVEL = 0x70,
    DISP_CMD_LAYER_GET_BLACK_EXTEN_LEVEL = 0x71,

//----scaler----
    DISP_CMD_SCALER_REQUEST = 0x80,
    DISP_CMD_SCALER_RELEASE = 0x81,
    DISP_CMD_SCALER_EXECUTE = 0x82,

//----hwc----
    DISP_CMD_HWC_OPEN = 0xc0,
    DISP_CMD_HWC_CLOSE = 0xc1,
    DISP_CMD_HWC_SET_POS = 0xc2,
    DISP_CMD_HWC_GET_POS = 0xc3,
    DISP_CMD_HWC_SET_FB = 0xc4,
    DISP_CMD_HWC_SET_PALETTE_TABLE = 0xc5,

//----video----
    DISP_CMD_VIDEO_START = 0x100,
    DISP_CMD_VIDEO_STOP = 0x101,
    DISP_CMD_VIDEO_SET_FB = 0x102,
    DISP_CMD_VIDEO_GET_FRAME_ID = 0x103,
    DISP_CMD_VIDEO_GET_DIT_INFO = 0x104,

//----lcd----
    DISP_CMD_LCD_ON = 0x140,
    DISP_CMD_LCD_OFF = 0x141,
    DISP_CMD_LCD_SET_BRIGHTNESS = 0x142,
    DISP_CMD_LCD_GET_BRIGHTNESS = 0x143,
    DISP_CMD_LCD_CPUIF_XY_SWITCH = 0x146,
    DISP_CMD_LCD_CHECK_OPEN_FINISH = 0x14a,
    DISP_CMD_LCD_CHECK_CLOSE_FINISH = 0x14b,
    DISP_CMD_LCD_SET_SRC = 0x14c,
    DISP_CMD_LCD_USER_DEFINED_FUNC = 0x14d,

//----tv----
    DISP_CMD_TV_ON = 0x180,
    DISP_CMD_TV_OFF = 0x181,
    DISP_CMD_TV_SET_MODE = 0x182,
    DISP_CMD_TV_GET_MODE = 0x183,
    DISP_CMD_TV_AUTOCHECK_ON = 0x184,
    DISP_CMD_TV_AUTOCHECK_OFF = 0x185,
    DISP_CMD_TV_GET_INTERFACE = 0x186,
    DISP_CMD_TV_SET_SRC = 0x187,
    DISP_CMD_TV_GET_DAC_STATUS = 0x188,
    DISP_CMD_TV_SET_DAC_SOURCE = 0x189,
    DISP_CMD_TV_GET_DAC_SOURCE = 0x18a,

//----hdmi----
    DISP_CMD_HDMI_ON = 0x1c0,
    DISP_CMD_HDMI_OFF = 0x1c1,
    DISP_CMD_HDMI_SET_MODE = 0x1c2,
    DISP_CMD_HDMI_GET_MODE = 0x1c3,
    DISP_CMD_HDMI_SUPPORT_MODE = 0x1c4,
    DISP_CMD_HDMI_GET_HPD_STATUS = 0x1c5,
	DISP_CMD_HDMI_SET_SRC = 0x1c6,

//----vga----
    DISP_CMD_VGA_ON = 0x200,
    DISP_CMD_VGA_OFF = 0x201,
    DISP_CMD_VGA_SET_MODE = 0x202,
    DISP_CMD_VGA_GET_MODE = 0x203,
	DISP_CMD_VGA_SET_SRC = 0x204,

//----sprite----
    DISP_CMD_SPRITE_OPEN = 0x240,
    DISP_CMD_SPRITE_CLOSE = 0x241,
    DISP_CMD_SPRITE_SET_FORMAT = 0x242,
    DISP_CMD_SPRITE_GLOBAL_ALPHA_ENABLE = 0x243,
    DISP_CMD_SPRITE_GLOBAL_ALPHA_DISABLE = 0x244,
    DISP_CMD_SPRITE_GET_GLOBAL_ALPHA_ENABLE = 0x252,
    DISP_CMD_SPRITE_SET_GLOBAL_ALPHA_VALUE = 0x245,
    DISP_CMD_SPRITE_GET_GLOBAL_ALPHA_VALUE = 0x253,
    DISP_CMD_SPRITE_SET_ORDER = 0x246,
    DISP_CMD_SPRITE_GET_TOP_BLOCK = 0x250,
    DISP_CMD_SPRITE_GET_BOTTOM_BLOCK = 0x251,
    DISP_CMD_SPRITE_SET_PALETTE_TBL = 0x247,
    DISP_CMD_SPRITE_GET_BLOCK_NUM = 0x259,
    DISP_CMD_SPRITE_BLOCK_REQUEST = 0x248,
    DISP_CMD_SPRITE_BLOCK_RELEASE = 0x249,
    DISP_CMD_SPRITE_BLOCK_OPEN = 0x257,
    DISP_CMD_SPRITE_BLOCK_CLOSE = 0x258,
    DISP_CMD_SPRITE_BLOCK_SET_SOURCE_WINDOW = 0x25a,
    DISP_CMD_SPRITE_BLOCK_GET_SOURCE_WINDOW = 0x25b,
    DISP_CMD_SPRITE_BLOCK_SET_SCREEN_WINDOW = 0x24a,
    DISP_CMD_SPRITE_BLOCK_GET_SCREEN_WINDOW = 0x24c,
    DISP_CMD_SPRITE_BLOCK_SET_FB = 0x24b,
    DISP_CMD_SPRITE_BLOCK_GET_FB = 0x24d,
    DISP_CMD_SPRITE_BLOCK_SET_PARA = 0x25c,
    DISP_CMD_SPRITE_BLOCK_GET_PARA = 0x25d,
    DISP_CMD_SPRITE_BLOCK_SET_TOP = 0x24e,
    DISP_CMD_SPRITE_BLOCK_SET_BOTTOM = 0x24f,
    DISP_CMD_SPRITE_BLOCK_GET_PREV_BLOCK = 0x254,
    DISP_CMD_SPRITE_BLOCK_GET_NEXT_BLOCK = 0x255,
    DISP_CMD_SPRITE_BLOCK_GET_PRIO = 0x256,
//---for Displayer Test --------	
	DISP_CMD_PRINT_REG = 0x2e0,
	DISP_CMD_PRINT_LCD_INFO = 0x2e1,
//---pwm --------	
    DISP_CMD_PWM_SET_PARA = 0x300,
    DISP_CMD_PWM_GET_PARA = 0x301,
}__disp_cmd_t;

#endif


