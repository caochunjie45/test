
#include "lcd_panel_cfg.h"



static void LCD_cfg_panel_info(__panel_para_t * info)
{
    memset(info,0,sizeof(__panel_para_t));

    info->lcd_x             = 480;
    info->lcd_y             = 272;
    info->lcd_dclk_freq     = 7;		//���������ݵĴ���Ƶ��	MHzΪ��λ
    info->lcd_srgb          = 0x00202020;
    info->lcd_swap          = 1;
    
    info->lcd_pwm_not_used  = 0;
    info->lcd_pwm_ch        = 0;
    info->lcd_pwm_freq      = 10000;     //Hz
    info->lcd_pwm_pol       = 1;

    info->lcd_if            = 0;        //0:hv(sync+de); 1:8080; 2:ttl; 3:lvds

    info->lcd_hbp           = 40;      //hsync back porch
    info->lcd_ht            = 525;     //hsync total cycle
    info->lcd_hv_hspw       = 0;        //hsync plus width
    info->lcd_vbp           = 8;       //vsync back porch
    info->lcd_vt            = (2 * 288);  //vysnc total cycle *2
    info->lcd_hv_vspw       = 0;        //vysnc plus width

    info->lcd_hv_if         = 0;        //0:hv parallel 1:hv serial 
    info->lcd_hv_smode      = 0;        //0:RGB888 1:CCIR656
    info->lcd_hv_s888_if    = 0;        //serial RGB format
    info->lcd_hv_syuv_if    = 0;        //serial YUV format

    info->lcd_cpu_if        = 0;        //0:18bit 4:16bit
    info->lcd_frm           = 0;        //0: disable; 1: enable rgb666 dither; 2:enable rgb656 dither

    info->lcd_lvds_ch       = 0;        //0:single channel; 1:dual channel
    info->lcd_lvds_mode     = 0;        //0:NS mode; 1:JEIDA mode
    info->lcd_lvds_bitwidth = 0;        //0:24bit; 1:18bit
    info->lcd_lvds_io_cross = 0;        //0:normal; 1:pn cross

    info->lcd_io_cfg0       = 0x02000000;//0x10000000;
    
}

static __s32 LCD_POWER_EN_A13(__u32 sel, __bool b_en)
{
	__s32 ret;
	ES_FILE *h_power = eLIBs_fopen("b:\\HID\\POWER", "w");

	
	if( b_en )
		ret = eLIBs_fioctrl(h_power, DRV_POWER_CMD_PWREPT_SET, POWER_EPT_1, (void *)(0x11));
	else
		ret = eLIBs_fioctrl(h_power, DRV_POWER_CMD_PWREPT_SET, POWER_EPT_1, (void *)(0x10));
	if(ret)
	{
		__log("LCD_POWER_EN_A13 fail\n");
	}
	else
	{
		eLIBs_fclose(h_power);
   	}
	return 0;
}

static __s32 LCD_BL_EN_A13(__u32 sel, __bool b_en)
{
	__s32 ret;
	ES_FILE *h_power = eLIBs_fopen("b:\\HID\\POWER", "w");


	if( b_en )
		ret = eLIBs_fioctrl(h_power, DRV_POWER_CMD_PWREPT_SET, POWER_EPT_2, (void *)(0x11));
	else
		ret = eLIBs_fioctrl(h_power, DRV_POWER_CMD_PWREPT_SET, POWER_EPT_2, (void *)(0x10));
	if(ret)
	{
		__log("LCD_BL_EN_A13 fail\n");
	}
	else
	{
		eLIBs_fclose(h_power);
   	}
	return 0;
}

static __s32 LCD_open_flow(__u32 sel)
{
	LCD_OPEN_FUNC(sel, LCD_power_on, 50);   //open lcd power, and delay 50ms
	LCD_OPEN_FUNC(sel, TCON_open, 500);     //open lcd controller, and delay 500ms
	LCD_OPEN_FUNC(sel, LCD_bl_open, 0);     //open lcd backlight, and delay 0ms

	return 0;
}

static __s32 LCD_close_flow(__u32 sel)
{	
	LCD_CLOSE_FUNC(sel, LCD_bl_close, 0);       //close lcd backlight, and delay 0ms
	LCD_CLOSE_FUNC(sel, TCON_close, 0);         //close lcd controller, and delay 0ms
	LCD_CLOSE_FUNC(sel, LCD_power_off, 1000);   //close lcd power, and delay 1000ms

	return 0;
}

static void LCD_power_on(__u32 sel)
{
    LCD_POWER_EN_A13(sel, 1);//config lcd_power pin to open lcd power
}

static void LCD_power_off(__u32 sel)
{
    LCD_POWER_EN_A13(sel, 0);//config lcd_power pin to close lcd power
}

static void LCD_bl_open(__u32 sel)
{
    LCD_PWM_EN(sel, 1);//open pwm module
    LCD_BL_EN_A13(sel, 1);//config lcd_bl_en pin to open lcd backlight
}

static void LCD_bl_close(__u32 sel)
{
	LCD_BL_EN_A13(sel, 0);//config lcd_bl_en pin to close lcd backlight
    LCD_PWM_EN(sel, 0);//close pwm module
}

static __s32 LCD_user_defined_func(__u32 sel, __u32 para1, __u32 para2, __u32 para3)
{
    return 0;
}


void LCD_get_panel_funs_0(__lcd_panel_fun_t * fun)
{
    fun->cfg_panel_info = LCD_cfg_panel_info;//delete this line if you want to use the lcd para define in sys_config1.fex

    fun->cfg_open_flow = LCD_open_flow;
    fun->cfg_close_flow = LCD_close_flow;
    fun->lcd_user_defined_func = LCD_user_defined_func;
}
