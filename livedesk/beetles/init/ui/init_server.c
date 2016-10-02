/*
************************************************************************************************************************
*											         ePDK
*						            the Easy Portable/Player Develop Kits
*									           desktop system 
*
*						          (c) Copyright 2007-2010, Jackie, China
*										All Rights Reserved
*
* File    		: init_server.c
* By      		: Jackie.chen
* Func		: init server 
* Version	: v1.0
* ======================================================================================================================
* 2010-10-25  Jackie.chen  create this file, implements the fundemental interface;
************************************************************************************************************************
*/

//#include "apps.h"
#include "mod_init_i.h"
#include "init_server.h"
#include "live_init.h"
#include "dialog_scene\\dialog_scene.h"
#include "headbar\\headbar_com.h"


#define CLOSE_SCN_TIME_ID		1001
#define LOW_POWER_CHECK_TIME_ID	1002
#define IO_CTRL_TIME_ID	1003
#define IO_ECHO_TIME_ID	1004
#define VOLUME_SAVE_TIME_ID  1005
#define POWER_OFF_PMU_CHECK_TIME_ID  1006

#define LOW_POWER_CHECK_TIME_CNT		120
#define IO_CTRL_CHECK_TIME_CNT		    100
#define IO_ECHO_CHECK_TIME_CNT		    80////150
#define VOLUME_SAVE_TIME_CNT            400
#define POWER_OFF_PMU_CHECK_TIME_CNT    3

#if BEETLES_UPDATE_ENABLE
#define INIT_FW_UPDATE_MAX_ICON_RES_NUM 2
#endif

#define PIOC_MEM_BASE   (0xf1c20800)
#define IOCTRL_PIO_REG_CFG(n, i)               ((volatile unsigned int *)( PIOC_MEM_BASE + ((n)-1)*0x24 + ((i)<<2) + 0x00))
#define IOCTRL_PIO_REG_PULL(n, i)              ((volatile unsigned int *)( PIOC_MEM_BASE + ((n)-1)*0x24 + ((i)<<2) + 0x1C))
#define IOCTRL_PIO_REG_DATA(n) 	            ((volatile unsigned int *)( PIOC_MEM_BASE + ((n)-1)*0x24 + 0x10))


extern GUI_FONT    *SWFFont;

#if BEETLES_UPDATE_ENABLE
typedef struct  
{
	__s32 x;
	__s32 y;
	__s32 w;
	__s32 h;
	__s32 res_id[INIT_FW_UPDATE_MAX_ICON_RES_NUM];
	HTHEME res_hdl[INIT_FW_UPDATE_MAX_ICON_RES_NUM];	
}init_fw_update_rect_t;

typedef enum
{		
    init_fw_update_prog_bg=0,
	init_fw_update_prog_left,
	init_fw_update_prog_mid,
	init_fw_update_prog_cursor,	
	init_fw_update_prog_icon_num
}init_fw_update_icon_t;
#endif

void init_lock(__gui_msg_t *msg)
{
	__u8 err;
	__init_ctl_t * init_ctr = (__init_ctl_t *)GUI_WinGetAttr(msg->h_deswin);	
	esKRNL_SemPend(init_ctr->state_sem, 0, &err);	
	return;
}

void init_unlock(__gui_msg_t *msg)
{
	__init_ctl_t * init_ctr = (__init_ctl_t *)GUI_WinGetAttr(msg->h_deswin);
	esKRNL_SemPost(init_ctr->state_sem);
}

__bool usb_power_supply(void)
{
	__s32	ret;
	user_gpio_set_t	gpio_set[1];
	__u32		usb_detect;
	eLIBs_memset(gpio_set, 0, sizeof(user_gpio_set_t));
	
	ret = esCFG_GetKeyValue("usbc0", "usb_det_vbus_gpio", (int *)gpio_set, sizeof(user_gpio_set_t)/4);

	if(!ret)
	{
		usb_detect = esPINS_PinGrpReq((__user_pin_set_t*)gpio_set,1);
		if(!usb_detect)
		{
			__msg("request usb_detect io failed \n");
			return EPDK_FAIL;
		}
	}
	else
	{
		__msg("read from sys_config file failed !\n");
		return EPDK_FAIL;
	}
	if(esPINS_ReadPinData(usb_detect, 0))
	{
	__here__;
		ret = 1;
		return ret;
	}
	else
	{
	__here__;
		ret = 0;
		return ret;
	}
	
}

//mic 直通功能开，mic  采样信号(可能经过微小放大)直接通过功放输出
//进音乐和视频加载混响会有问题，所以提供
//进音乐和视频时必须先调用dsk_com_mic_close,再调用此函数
static void dsk_audio_direct_output_onoff(__u8 flag)
{
	__u32 val;

	if(0 == flag)
	{
		val = MIC_DAC_CTL | (0x200019C0);
		MIC_DAC_CTL = val;

	    val = MIC_ADC_CTL & ~(1U<<4);
		MIC_ADC_CTL = val;		
	}
	else
	{
		val = MIC_DAC_CTL | (0x200019C0);
		MIC_DAC_CTL = val;
		
		val = MIC_ADC_CTL | (1U<<4);
		MIC_ADC_CTL = val;
	}
}

static __s32 ioctrl_app_linein_start(void)
{    
/*
    ES_FILE *paudio;
    
    paudio = eLIBs_fopen("b:\\AUDIO\\FM", "r+");
    if(paudio)
    {
        __wrn("\n              LINE IN START succeed!\n");
        eLIBs_fioctrl(paudio, AUDIO_DEV_CMD_START, 0, 0);
        eLIBs_fclose(paudio);

        *(volatile unsigned long*)(0xf1c23c00 + 0x10) |= (0x5<<23);
       // __wrn("*(volatile unsigned long*)(0xf1c23c00 + 0x10)=0x%x\n", *(volatile unsigned long*)(0xf1c23c00 + 0x10));
    }
*/
	{
        //__wrn("\n              LINE IN START succeed!\n");

	   SET_REG_BIT(READ_REG32(0xf1c22c10), DAC_ANA_LLIN_TO_LMIX_BIT);
	   SET_REG_BIT(READ_REG32(0xf1c22c10), DAC_ANA_RLIN_TO_RMIX_BIT);

		dsk_audio_direct_output_onoff(1);	   
		//SET_REG_BIT(READ_REG32(0xf1c22c10), DAC_ANA_PA_MUTE_BIT);
		//SET_REG_BIT(READ_REG32(0xf1c22c10), DAC_ANA_MIX_TO_PA_MUTE_BIT);

        //*(volatile unsigned long*)(0xf1c22c10) |= (0x0<<26);  //linein 增益 1:0db  0:-1.5db
        //__wrn("*(volatile unsigned long*)(0xf1c22c10 )=0x%x\n", *(volatile unsigned long*)(0xf1c22c10));
    }

	dsk_speaker_onoff(BEETLES_SPEAK_ON);
    return EPDK_OK;
}

static __s32 ioctrl_app_linein_stop(void)
{    
/*
    ES_FILE *paudio;
    
    paudio = eLIBs_fopen("b:\\AUDIO\\FM", "r+");
    if(paudio)
	{
		__inf("\n              LINE IN STOP succeed!\n");
	    eLIBs_fioctrl(paudio, AUDIO_DEV_CMD_STOP, 0, 0);
        eLIBs_fclose(paudio);

        //change to default
        *(volatile unsigned long*)(0xf1c23c00 + 0x10) |= (0x3<<23);
        //__wrn("*(volatile unsigned long*)(0xf1c23c00 + 0x10)=0x%x\n", *(volatile unsigned long*)(0xf1c23c00 + 0x10));
    }
*/
	{
		//__inf("\n              LINE IN STOP succeed!\n");

		CLEAR_REG_BIT(READ_REG32(0xf1c22c10), DAC_ANA_RFM_TO_RMIX_BIT);
		CLEAR_REG_BIT(READ_REG32(0xf1c22c10), DAC_ANA_LFM_TO_LMIX_BIT);

		dsk_audio_direct_output_onoff(0);	  
        //change to default
        //*(volatile unsigned long*)(0xf1c22c10) |= (0x1<<26);
        //__wrn("*(volatile unsigned long*)(0xf1c22c10 )=0x%x\n", *(volatile unsigned long*)(0xf1c22c10 ));
    }

	dsk_speaker_onoff(BEETLES_SPEAK_OFF);
    return EPDK_OK;
}

static __s32 ioctrl_get_gpio_status(__u32 port, __u32 port_num, __u32 pull)
{
	volatile  __u32    *tmp_addr;
	__u32               tmp_data;
	__s32               ret;
	__u32               tmp1;
	__u32               tmp2;

	//设置为输入
	tmp1 = (port_num - ((port_num>>3)<<3))<<2;
	tmp_addr = IOCTRL_PIO_REG_CFG(port, (port_num>>3));
	tmp_data = *tmp_addr;
	tmp_data &= ~(0x07 << tmp1);
	tmp_data |=  (0x00 << tmp1);
	*tmp_addr = tmp_data;

	if(pull)
	{
		//设置为上拉
		tmp2 = (port_num - ((port_num>>4)<<4))<<1;
		tmp_addr = IOCTRL_PIO_REG_PULL(port, (port_num>>4));
		tmp_data = *tmp_addr;
		tmp_data &= ~(0x03 << tmp2);
		tmp_data |=  (0x01 << tmp2);
		*tmp_addr = tmp_data; 
	}
	else
	{
		//设置为下拉
		tmp2 = (port_num - ((port_num>>4)<<4))<<1;
		tmp_addr = IOCTRL_PIO_REG_PULL(port, (port_num>>4));
		tmp_data = *tmp_addr;
		tmp_data &= ~(0x03 << tmp2);
		tmp_data |=  (0x02 << tmp2);
		*tmp_addr = tmp_data;
	}


	tmp_addr = IOCTRL_PIO_REG_DATA(port);
	tmp_data = *tmp_addr;

	ret = (tmp_data & (1 << port_num))>>port_num;

	return ret;
}

static __s32 init_low_power_proc(__gui_msg_t *msg)
{
	__init_ctl_t * init_ctr = (__init_ctl_t *)GUI_WinGetAttr(msg->h_deswin);
	
	if( !init_ctr->usb_connect && init_ctr->usb_power_support != 1)
	{		
		/* 退出应用程序*/
		__here__;
		activity_finish_all();
		/* low power dialog */		
		{
			init_scene_t *p_scene;
			p_scene = (init_scene_t *)GUI_WinGetAddData(msg->h_deswin);
			p_scene->low_power = EPDK_TRUE;
			scene_on_dialog(msg);
		}
	}
	
	return EPDK_OK;
}

static __s32 init_lo_ctrl_proc(__gui_msg_t *msg)
{
	__init_ctl_t * init_ctr = (__init_ctl_t *)GUI_WinGetAttr(msg->h_deswin);
	
	if( !init_ctr->usb_connect)
	{		
		/* 退出应用程序*/
		__here__;
		activity_finish_all();
		//activity_return_home(NULL);
		gscene_bgd_set_state(BGD_STATUS_SHOW);
		gscene_hbar_set_state(HBAR_ST_HIDE);

		/* io ctrl dialog */		
		{
			init_scene_t *p_scene;
			p_scene = (init_scene_t *)GUI_WinGetAddData(msg->h_deswin);
			p_scene->io_ctrl = EPDK_TRUE;
			scene_on_dialog(msg);
		}
	}
	
	return EPDK_OK;
}

static __s32 init_power_off_proc(__gui_msg_t * msg)
{	
	__init_ctl_t * init_ctr = (__init_ctl_t *)GUI_WinGetAttr(msg->h_deswin);
	
	if( !init_ctr->usb_connect && init_ctr->usb_power_support != 1)
	{	
		init_scene_t *p_scene;
		activity_finish_all();
		gscene_bgd_set_state(BGD_STATUS_SHOW);
		gscene_hbar_set_state(HBAR_ST_HIDE);
		/* poweroff dialog */		
		p_scene = (init_scene_t *)GUI_WinGetAddData(msg->h_deswin);
		p_scene->power_off = EPDK_TRUE;
		scene_on_dialog(msg);		
		__here__;
		if(EPDK_FALSE == p_scene->usb_plug_in)//usb连接到来把关机对话框关闭后，程序不应该往下执行，只有usb未连接才能关机
		{
			__here__;
			//notify "init" to exit
			NOTIFY_MSG(GUI_MSG_CLOSE, 0, msg->h_deswin, 0, 0);
		}
	}

	return EPDK_OK;
}

static __s32 init_usb_device_plugin_proc(__gui_msg_t * msg)
{
	__init_ctl_t * init_ctr = (__init_ctl_t *)GUI_WinGetAttr(msg->h_deswin);
    
	/* 通知除主界面外所有应用程序退出 */	
	init_ctr->usb_connect = EPDK_TRUE;	

	
	//activity_return_home(NULL);	
	//activity_kill_all_backgrd();	
	//activity_suspend_top();
	{
		__gui_msg_t 		ntymsg;

		ntymsg.id			=  DSK_MSG_BOX_STANDBY_EXIT;
		ntymsg.h_srcwin 		=  msg->h_deswin;
		ntymsg.h_deswin 		=  GUI_WinGetHandFromName("beetles"); 
		ntymsg.dwAddData1	=  NULL;
		ntymsg.dwAddData2	=  NULL;
		ntymsg.dwReserved	=  NULL;
		
		GUI_SendMessage(&ntymsg); 
	}
	activity_finish_all();
	if(dsk_wkm_is_open())
	{
		dsk_wkm_close();
	}
	gscene_bgd_set_state(BGD_STATUS_SHOW);
	gscene_hbar_set_state(HBAR_ST_HIDE);
	/* usb dialog */		
	{
		init_scene_t *p_scene;
		p_scene = (init_scene_t *)GUI_WinGetAddData(msg->h_deswin);
		p_scene->usb_connect= EPDK_TRUE;
		scene_on_dialog(msg);
	}
	
	return EPDK_OK;
}

static __s32 init_usb_device_plugout_proc(__gui_msg_t *msg)
{
	__init_ctl_t*   init_ctr = NULL;
	init_scene_t* p_scene = NULL;
	
	init_ctr = (__init_ctl_t *)GUI_WinGetAttr(msg->h_deswin);
	p_scene = (init_scene_t *)GUI_WinGetAddData(msg->h_deswin);

	init_ctr->usb_connect = EPDK_FALSE;
	
	p_scene->usb_plug_in = EPDK_FALSE;
	p_scene->usb_connect= EPDK_FALSE;
	scene_on_dialog(msg);
	
	return EPDK_OK;
}


/**
 * 开屏
 */
static __s32 init_open_screen(__gui_msg_t *msg)
{	
	__bool exist;
	__disp_output_type_t output;
	__init_ctl_t * init_ctr = (__init_ctl_t *)GUI_WinGetAttr(msg->h_deswin);
	init_scene_t * p_scene 	= (init_scene_t *)GUI_WinGetAddData(msg->h_deswin);

	output = dsk_display_get_output_type();

	__here__;
	if( (output == DISP_OUTPUT_TYPE_NONE)||(output == DISP_OUTPUT_TYPE_LCD) )
	{		
		init_lock(msg);
		__here__;
		if( init_ctr->screen_close_done )	/* 屏幕已经关闭 */
		{		
			__here__;
			init_unlock(msg); 
			dsk_display_lcd_on();				/* 调用显示驱动, 开屏*/
			
			init_lock(msg);
			init_ctr->screen_close_done = EPDK_FALSE;/* 	清关屏状态位 */
			init_unlock(msg);
		}
		else									/* 屏幕还没关闭 */
		{		
			init_unlock(msg);			
		}	

		exist = GUI_IsTimerInstalled(msg->h_deswin, init_ctr->close_scn_time_id);
		if( exist == EPDK_TRUE )
			GUI_ResetTimer(msg->h_deswin, init_ctr->close_scn_time_id, init_ctr->closescn_timeout, NULL);/* 从新设置定时器*/
		
	}

	
	return EPDK_OK;
}

static __s32 init_close_screen(__gui_msg_t *msg)
{	
	__disp_output_type_t output;
	__bool audio_engine_is_open = EPDK_FALSE;
	__init_ctl_t * init_ctr = (__init_ctl_t *)GUI_WinGetAttr(msg->h_deswin);

	__here__;
	output = dsk_display_get_output_type();
	if( output == DISP_OUTPUT_TYPE_LCD )
	{
		/* 置关屏状态位 */
		init_lock(msg);

		if( !init_ctr->screen_close_done )
		{
			init_ctr->screen_close_done = EPDK_TRUE;
			
			init_unlock(msg);
			dsk_display_lcd_off();	

			//__msg("----activity =%d\n", activity_all_able_standby());
			//__msg("----standby =%d\n", g_is_enable_standby());
			//__msg("----playing =%d\n", dsk_wkm_is_playing());
			//__msg("----change =%d\n", dsk_get_charge_state());

			////////////////////////
			//adjust fm exist??
			if(!audio_engine_is_open)
			{
				if(0x0 != GUI_WinGetHandFromName("app_fm"))
				{
					audio_engine_is_open = EPDK_TRUE;
				}
			}
			//adjust audio exist??
			if(!audio_engine_is_open)
			{
				if(0x0 != GUI_WinGetHandFromName("app_music"))
				{
					audio_engine_is_open = EPDK_TRUE;
				}
			}
			//adjust audio exist??
			if(!audio_engine_is_open)
			{
				if(0x0 != GUI_WinGetHandFromName("app_record"))
				{
					audio_engine_is_open = EPDK_TRUE;
				}
			}
			/*
			//adjust video exist??
			if(!audio_engine_is_open)
			{
				if(0x0 != GUI_WinGetHandFromName("app_movie"))
				{
					audio_engine_is_open = EPDK_TRUE;
				}
			}
			//adjust tv exist??
			if(!audio_engine_is_open)
			{
				if(0x0 != GUI_WinGetHandFromName("APP_DTV"))
				{
					audio_engine_is_open = EPDK_TRUE;
				}
			}
			*/
			///////////////

			/* 进入 standby */
			#if 1
			//if(EPDK_TRUE == activity_all_able_standby() && !dsk_wkm_is_playing() && !dsk_radio_is_open() && g_is_enable_standby())
			if(EPDK_FALSE == dsk_get_charge_state() && g_is_enable_standby() &&(EPDK_FALSE == audio_engine_is_open)&&(0 == dsk_audio_echo_is_open()))
			{
				ES_FILE *pfile = NULL;

				//进入standby之前卸载host驱动，防止standby唤醒后认不了host
				init_ctr->usb_host_is_working_before_standy = dsk_usbh_is_working(1);
				//__msg(">>>>>>>>>>>>>>=%d\n",init_ctr->usb_host_is_working_before_standy);
				if(init_ctr->usb_host_is_working_before_standy)
				{                    
					__s32 cnt = 0;
#if 0                
					dsk_send_rmmod_usbh_cmd(1);
#else
					esDEV_Plugout("\\drv\\usb_host1.drv", 1);
#endif
					while(dsk_usbh_is_working(1))
					{
						esKRNL_TimeDly(20);
						cnt++;
						if(cnt > 50)
						{
							break;
						}
					}
				}
				__here__;
				pfile = eLIBs_fopen("b:\\AUDIO\\CTRL", "r+");
				if(pfile)
				{
					//system will enter standby, set audio device to standby mode for power save	                
					//dsk_speaker_turnoff();	                
					eLIBs_fioctrl(pfile, AUDIO_DEV_CMD_ENTER_STANDBY, 0, 0);
				}

		         	__here__;		
				esPWM_EnterStandby(0);
		         	__here__;
				
				if(pfile)
				{
					//active audio device
					eLIBs_fioctrl(pfile, AUDIO_DEV_CMD_EXIT_STANDBY, 0, 0);	                
					esKRNL_TimeDly(5);
					//dsk_speaker_resume();	                
					eLIBs_fclose(pfile);
					pfile = NULL;
				}

				__here__;
				init_open_screen(msg);

				//退出之后重新加载host driver
				if(init_ctr->usb_host_is_working_before_standy)
				{
#if 0                
					dsk_send_insmod_usbh_cmd(1);
#else
					esDEV_Plugin("\\drv\\usb_host1.drv", 1, NULL, 0);
#endif
					init_ctr->usb_host_is_working_before_standy = 0;
				}
			}			
			#endif
		}
		else
		{
			init_unlock(msg);
		}

	}


	return EPDK_OK;
}

/** 
 * 设置自动关屏时间
 */
static __s32 init_set_close_scn(__gui_msg_t *msg)
{	
	__bool exist;
	__init_ctl_t * init_ctr = (__init_ctl_t *)GUI_WinGetAttr(msg->h_deswin);
	
	if( msg->dwAddData1 == 0 )
	{        
		init_ctr->closescn_timeout = 0;
		exist = GUI_IsTimerInstalled(msg->h_deswin, init_ctr->close_scn_time_id);
		if( exist == EPDK_TRUE )
		{
			GUI_KillTimer(msg->h_deswin, init_ctr->close_scn_time_id);
		}				
	}	
	else if( msg->dwAddData1 >0 )
	{
		init_ctr->closescn_timeout = msg->dwAddData1/10;
		exist = GUI_IsTimerInstalled(msg->h_deswin, init_ctr->close_scn_time_id);		
		if( exist == EPDK_TRUE )
		{
			GUI_ResetTimer(msg->h_deswin, init_ctr->close_scn_time_id, init_ctr->closescn_timeout, NULL);
		}
		else
		{
			GUI_SetTimer(msg->h_deswin, init_ctr->close_scn_time_id, init_ctr->closescn_timeout, NULL);
		}		
	}
	else
	{
		__wrn(" close screen time < 0, set fail \n");
	}
		
	return EPDK_OK;
}

static __s32 init_scene_create(__gui_msg_t *msg)
{
	SIZE	screen_size;
		
	init_screen_size();	

	//dialog		
	dialog_init(msg->h_deswin);
	
	//headbar		
	gscene_hbar_create(msg->h_deswin, SWFFont);	

	//backgrd		
	screen_size.width	 = LCD_WIDTH;
	screen_size.height	 = LCD_HEIGHT;
	gscene_bgd_init(&screen_size, BGD_STATUS_HIDE, FB_TYPE_YUV);
	
	__msg("----width = %d,height = %d\n", screen_size.width, screen_size.height);
	
	return EPDK_OK;
}

static __s32 init_scene_destroy(__gui_msg_t *msg)
{
	init_scene_t * p_scene = (init_scene_t *)GUI_WinGetAddData(msg->h_deswin);
	
	dialog_deinit(msg->h_deswin);
	__here__;
	gscene_hbar_delete();
	__here__;
	gscene_bgd_deinit();	
	
	return EPDK_OK;
}

static __s32 init_voltage_check(__gui_msg_t *msg)
{
	__s32 is_full;
	__gui_msg_t msgex;
	static power_level_e old_vol_level = DSK_POWER_LEVEL_;
	static __bool old_charge_state = 0;

	power_level_e	level;
	__bool charge_state;

#if (AW_SDK_PMU == 0)	 //不带PMU
	dsk_power_get_voltage_level(&level,&is_full);		
#else //带PMU
	dsk_power_get_battery_level(&level,&is_full);
#endif

	charge_state = dsk_get_charge_state();
	if(old_vol_level != level || 
	    old_charge_state != charge_state)
	{
		msgex.id 			= DSK_MSG_VOLTAGE_CHANGE;
		msgex.h_srcwin 		= NULL;
		msgex.h_deswin 		= msg->h_deswin;
		msgex.dwAddData1 	= (__u32)level;
		msgex.dwAddData2 	= 0;
		msgex.dwReserved 	= 0;
		msgex.p_arg	   		= NULL;
		
		activity_notify_top(&msgex);          
	}
#if 0////BEETLES_KEY_REDEF
	else////James++ for mainmen calendar time refresh
	{
		__gui_msg_t 		ntymsg;

		ntymsg.id			=  DSK_MSG_TIME_CHANGE;
		ntymsg.h_srcwin 		=  NULL;
		ntymsg.h_deswin 		=  GUI_WinGetHandFromName("beetles"); 
		ntymsg.dwAddData1	=  0;
		ntymsg.dwAddData2	=  0;
		ntymsg.dwReserved	=  NULL;

		GUI_SendNotifyMessage(&ntymsg); 
	}
#endif
	//__msg("level=%d\n", level);

	old_vol_level = level;
	old_charge_state = charge_state;

	return EPDK_OK;
}

#if BEETLES_UPDATE_ENABLE
static __s32 __init_prog_draw_progress(init_fw_update_rect_t* prog_bg, init_fw_update_rect_t* prog_left, 
    init_fw_update_rect_t* prog_mid, init_fw_update_rect_t* prog_cursor,
    __s32 min, __s32 max, __s32 cur)
{
    //画进度条背景
	{
		void* pbmp;

		if (!prog_bg->res_hdl[0])
		{
			__msg("prog_bg->res_hdl[0] is null...\n");
			return EPDK_FAIL;				
		}		
		pbmp = dsk_theme_hdl2buf(prog_bg->res_hdl[0]);
		if (!pbmp)
		{
			__msg("pbmp is null...\n");
			return EPDK_FAIL;
		}		
		GUI_BMP_Draw(pbmp, prog_bg->x, prog_bg->y);
	}

    //画进度条左边的图标
	{
		void* pbmp;
        __s32 focus;

        focus = 0;
		
		if (!prog_left->res_hdl[focus])
		{
			__msg("prog_left->res_hdl[focus] is null...\n");			
			return EPDK_FAIL;				
		}		
		pbmp = dsk_theme_hdl2buf(prog_left->res_hdl[focus]);
		if (!pbmp)
		{
			__msg("pbmp is null...\n");			
			return EPDK_FAIL;
		}		
		GUI_BMP_Draw(pbmp, prog_left->x, prog_left->y);				
	}

	//画进度条和cursor
	if(cur > max)//保护一下
    {
        cur = max;
    }
    
    if(cur < min)//保护一下
    {
        cur = min;
    }
    
	if(cur >= min
		&& cur <= max)
	{
		void* pbmp;
		
		if (!prog_mid->res_hdl[0])
		{
			__msg("prog_mid->res_hdl[0] is null...\n");			
			return EPDK_FAIL;				
		}		
		pbmp = dsk_theme_hdl2buf(prog_mid->res_hdl[0]);
		if (!pbmp)
		{
			__msg("pbmp is null...\n");			
			return EPDK_FAIL;
		}		

		{
			__s32 i;
			__s32 n;
            __s32 mid_w;
            __s32 max_mid_w;
            __s32 cursor_w;
            __s32 cursor_pos;
            __s32 bg_pos;
            __s32 bg_w;

			if (cur == min)
			{
				n = 0;
			}
			else
			{
				n = (prog_bg->w-prog_cursor->w)*(cur-min)
					/prog_mid->w/(max - min);
			}

            mid_w = prog_mid->w;
            max_mid_w = prog_bg->w-prog_cursor->w;
            cursor_w = prog_cursor->w;
            bg_pos = prog_bg->x;
            bg_w = prog_bg->w;
            //防止非整数倍时画不满
            if(max_mid_w-n*mid_w < mid_w)
            {
                n++;
                cursor_pos = bg_pos + bg_w - cursor_w;
            }
            else
            {
                cursor_pos = bg_pos + n*mid_w;
            }
            
			for (i = 1 ; i < n ; i++)
			{
				GUI_BMP_Draw(pbmp, prog_mid->x+i*prog_mid->w
					, prog_mid->y);
			}

            //画cursor
            if (!prog_cursor->res_hdl[0])
    		{
    			__msg("prog_cursor->res_hdl[0] is null...\n");			
    			return EPDK_FAIL;				
    		}		
    		pbmp = dsk_theme_hdl2buf(prog_cursor->res_hdl[0]);
    		if (!pbmp)
    		{
    			__msg("pbmp is null...\n");			
    			return EPDK_FAIL;
    		}		
    		GUI_BMP_Draw(pbmp, cursor_pos, prog_cursor->y);				
		}		
	}   
    
    return EPDK_OK;
}

static void fw_update(__gui_msg_t *msg, char* fw_path)
{
	__u8 	mid_update;
	__mp* 	mod_update;
    __s32 progress;  
    RECT rect;
    char str_text[128];
    GUI_RECT gui_rect;
    init_scene_t * p_scene ;
    GUI_MEMDEV_Handle memdev_hdl = NULL;
    
    init_fw_update_rect_t prog_ui[init_fw_update_prog_icon_num];

    __msg("fw_path=%s\n", fw_path);
    p_scene = (init_scene_t *)GUI_WinGetAddData(msg->h_deswin);	    

    GUI_LyrWinSetSta(p_scene->dialog_layer, GUI_LYRWIN_STA_ON);
    GUI_LyrWinSel(p_scene->dialog_layer);     
    GUI_LyrWinSetTop(p_scene->dialog_layer);
    GUI_LyrWinGetScnWindow(p_scene->dialog_layer, &rect);
    dsk_langres_get_menu_text(STRING_FW_UPDATE, str_text, sizeof(str_text));   
    
    gui_rect.x0 = 0;
    gui_rect.y0 = 50;
    gui_rect.x1 = rect.width-1;
    gui_rect.y1 = gui_rect.y0 + 32;
    GUI_SetFont(SWFFont);				
	GUI_SetDrawMode(GUI_DRAWMODE_TRANS);
	GUI_SetColor(GUI_WHITE);														
	GUI_CharSetToEncode(EPDK_CHARSET_ENM_UTF8);	
    GUI_DispStringInRect(str_text, &gui_rect, GUI_TA_HCENTER|GUI_TA_VCENTER);

    prog_ui[init_fw_update_prog_bg].x = 182;//61;
    prog_ui[init_fw_update_prog_bg].y = 130;//rect.height - 20;
    prog_ui[init_fw_update_prog_bg].w = 134;
    prog_ui[init_fw_update_prog_bg].h = 15;
    prog_ui[init_fw_update_prog_bg].res_id[0] = ID_INIT_PROG_BG_BMP;
    prog_ui[init_fw_update_prog_bg].res_id[1] = 0;
    
    prog_ui[init_fw_update_prog_left].x = 182;//61;
    prog_ui[init_fw_update_prog_left].y = 130;//rect.height - 20;
    prog_ui[init_fw_update_prog_left].w = 7;
    prog_ui[init_fw_update_prog_left].h = 15;
    prog_ui[init_fw_update_prog_left].res_id[0] = ID_INIT_PROG_LEFT_BMP;
    prog_ui[init_fw_update_prog_left].res_id[1] = 0;

    prog_ui[init_fw_update_prog_mid].x = 182;//61;
    prog_ui[init_fw_update_prog_mid].y = 130;//rect.height - 20;
    prog_ui[init_fw_update_prog_mid].w = 7;
    prog_ui[init_fw_update_prog_mid].h = 15;
    prog_ui[init_fw_update_prog_mid].res_id[0] = ID_INIT_PROG_MID_BMP;
    prog_ui[init_fw_update_prog_mid].res_id[1] = 0;

    prog_ui[init_fw_update_prog_cursor].x = 182;//61;
    prog_ui[init_fw_update_prog_cursor].y = 130;//rect.height - 20;
    prog_ui[init_fw_update_prog_cursor].w = 20;
    prog_ui[init_fw_update_prog_cursor].h = 15;
    prog_ui[init_fw_update_prog_cursor].res_id[0] = ID_INIT_PROG_CURSOR_BMP;
    prog_ui[init_fw_update_prog_cursor].res_id[1] = 0;

    {
		int i;
		int j;
		
		for (i = 0 ; i < init_fw_update_prog_icon_num ; i++)
		{
			for (j = 0 ; j < INIT_FW_UPDATE_MAX_ICON_RES_NUM ; j++)
			{
				if (prog_ui[i].res_id[j])
				{
					prog_ui[i].res_hdl[j] = dsk_theme_open(prog_ui[i].res_id[j]);
					if (!prog_ui[i].res_hdl[j])
					{
						__msg("dsk_theme_open fail...\n");
					}
                    else//先读取nor数据占住到内存
                    {
                        dsk_theme_hdl2buf(prog_ui[i].res_hdl[j]);
                    }
				}
                else
                {
                    prog_ui[i].res_hdl[j] = NULL;
                }
			}
		}
	}	     

    __init_prog_draw_progress(&prog_ui[init_fw_update_prog_bg],
                &prog_ui[init_fw_update_prog_left], 
                &prog_ui[init_fw_update_prog_mid],
                &prog_ui[init_fw_update_prog_cursor],
                0, 100, 0);

    mid_update = esMODS_MInstall(BEETLES_APP_ROOT"mod\\update.mod", 0);
	if( mid_update == 0 )
	{
		__err("update mod install error \n");
		goto err1;
	} 
	
	mod_update = esMODS_MOpen(mid_update, 0);
	if( mod_update == NULL)
	{
		__err("open update mode error \n");
		esMODS_MUninstall(mid_update);
		goto err1;
	}
	
	__msg("mod_update = %x,file=%s\n",mod_update, fw_path);
	
	esMODS_MIoctrl(mod_update, UPDATE_CMD_START, 0, (void *)fw_path);	
	
	__msg(" ioctrl mod update complete \n");
    
    while (1)
	{
	    esKRNL_TimeDly(50);
	    progress = esMODS_MIoctrl(mod_update, UPDATE_CMD_CHECK_PROG, 0, 0);
	    if (-1  == progress)
	    {
	        __err("error occur\n");
	        break;
	    }

        GUI_LyrWinSel(p_scene->dialog_layer);
        memdev_hdl = GUI_MEMDEV_Create(
            prog_ui[init_fw_update_prog_bg].x,
            prog_ui[init_fw_update_prog_bg].y,
            prog_ui[init_fw_update_prog_bg].w,
            prog_ui[init_fw_update_prog_bg].h);
    	if (!memdev_hdl)
    	{
            __msg("GUI_MEMDEV_Create fail...\n");
    		continue ;
    	}

        GUI_MEMDEV_Select(memdev_hdl);
        __init_prog_draw_progress(&prog_ui[init_fw_update_prog_bg],
            &prog_ui[init_fw_update_prog_left], 
            &prog_ui[init_fw_update_prog_mid],
            &prog_ui[init_fw_update_prog_cursor],
            0, 100, progress);

        GUI_MEMDEV_CopyToLCD(memdev_hdl); 
    	GUI_MEMDEV_Select( NULL );			
    	GUI_MEMDEV_Delete(memdev_hdl);
	    __msg("current progress is %d%%\n", progress);
	    if (100 == progress)
	    {
            __msg("update complete\n");
            esMODS_MIoctrl(mod_update, UPDATE_CMD_STOP, 0, 0);
	    }
	}
	
	esMODS_MClose(mod_update);
	
	esMODS_MUninstall(mid_update);   

err1:;    
    
	return ;
}
#endif


static __s32 init_mainwin_cb(__gui_msg_t *msg)
{
	switch( msg->id )
	{
		case GUI_MSG_CREATE:				
		{			
			__init_ctl_t * init_ctr;
			init_scene_t * p_scene;
            __u8 ret;
			__here__;
			init_ctr = (__init_ctl_t *)eLIBs_malloc(sizeof(__init_ctl_t));
			if(!init_ctr)
			{
				__err("init_ctr malloc error \n");
				return EPDK_FAIL;
			}
			eLIBs_memset( init_ctr, 0, sizeof(__init_ctl_t));
			GUI_WinSetAttr(msg->h_deswin, (void*)init_ctr);

			p_scene = (init_scene_t *)eLIBs_malloc(sizeof(init_scene_t));
			if(!p_scene)
			{
				__err("p_scene malloc error \n");
				return EPDK_FAIL;
			}
			eLIBs_memset( p_scene, 0, sizeof(init_scene_t));
			GUI_WinSetAddData(msg->h_deswin, (__u32)p_scene);
			p_scene->init_win = msg->h_deswin;


			init_ctr->low_power_check_time_id	= LOW_POWER_CHECK_TIME_ID;
			init_ctr->close_scn_time_id	   		= CLOSE_SCN_TIME_ID;
			init_ctr->io_ctrl_time_id	   			= IO_CTRL_TIME_ID;
			init_ctr->io_echo_time_id	   		= IO_ECHO_TIME_ID;
			init_ctr->volume_save_check_time_id	= VOLUME_SAVE_TIME_ID;
            
			init_ctr->low_power_check_timeout	= LOW_POWER_CHECK_TIME_CNT;
			init_ctr->io_ctrl_timeout			= IO_CTRL_CHECK_TIME_CNT;
			init_ctr->io_echo_timeout			= IO_ECHO_CHECK_TIME_CNT;
			init_ctr->volume_save_check_timeout	= VOLUME_SAVE_TIME_CNT;

            init_ctr->power_off_check_pmu_time_id = POWER_OFF_PMU_CHECK_TIME_ID;
            init_ctr->power_off_check_pmu_timeout = POWER_OFF_PMU_CHECK_TIME_CNT;
            
			GUI_SetTimer(msg->h_deswin, init_ctr->low_power_check_time_id, init_ctr->low_power_check_timeout, NULL);
#if BEETLES_IOCTRL_CHECK
			GUI_SetTimer(msg->h_deswin, init_ctr->io_ctrl_time_id, init_ctr->io_ctrl_timeout, NULL);
#endif
#if BEETLES_IOECHO_CHECK
			GUI_SetTimer(msg->h_deswin, init_ctr->io_echo_time_id, init_ctr->io_echo_timeout, NULL);
#endif

#if BEETLES_POWEROFF_CHEAK
            ret = esCFG_GetKeyValue("power_check_para", "POWER_CHECK", (int *)&(init_ctr->power_off_check_pmu_gpio), sizeof(user_gpio_set_t)/4);
        	if (!ret)
        	{
    	    	init_ctr->power_off_check_pmu_pin_req = esPINS_PinGrpReq((__user_pin_set_t*)&init_ctr->power_off_check_pmu_gpio, 1);
    	    	if (!init_ctr->power_off_check_pmu_pin_req)
    	    	{
    	    		eDbug("request power_off_check_pmu pin failed\n");
    	    	}
    	    	else
    				GUI_SetTimer(msg->h_deswin, init_ctr->power_off_check_pmu_time_id, init_ctr->power_off_check_pmu_timeout, NULL);
        	}
    		else
    		{
    			eDbug("read power_off_check_pmu pin failed\n");
    		}
#endif

			if( init_ctr->closescn_timeout != 0 )
			{
				GUI_SetTimer(msg->h_deswin, init_ctr->close_scn_time_id, init_ctr->closescn_timeout, NULL);
			}
			
			/* 创建信号量 */
			init_ctr->state_sem = esKRNL_SemCreate(1);
			init_ctr->closescn_gate_on 	= EPDK_TRUE;
			
			init_ctr->usb_connect 	   			= EPDK_FALSE;
			init_ctr->screen_close_done 		= EPDK_FALSE;	
			init_ctr->key_msg_enble	   		= EPDK_TRUE;
			init_ctr->closescn_timer_exist	   	= EPDK_FALSE;
			init_ctr->usb_host_is_working_before_standy	   	= EPDK_FALSE;
			init_ctr->power_off	   			= EPDK_FALSE;
			init_ctr->usb_power_support	   		= EPDK_FALSE;
			init_ctr->g_b_enable_standby	   	= EPDK_TRUE;

			//d -ab
			//set_input_gpio(1, 4, 0);
			//pull speaker hight
			dsk_speaker_onoff(BEETLES_SPEAK_OFF);
			//dtmb 1.1v off
			dsk_speaker_turnoff();
			//dsk_audio_echo_set_mic_pre_gain(0);
			//dsk_audio_echo_set_mic_gain(7);

			//initialize res
			live_init();
			//fifo_init();
			//create hbar,bgd,dialog
			init_scene_create(msg);

			break;
		}
		
		case GUI_MSG_DESTROY:
		{
			__u8 err;				
			__init_ctl_t * init_ctr = (__init_ctl_t *)GUI_WinGetAttr(msg->h_deswin);
			init_scene_t * p_scene = (init_scene_t *)GUI_WinGetAddData(msg->h_deswin);	

			__here__;
			if( GUI_IsTimerInstalled(msg->h_deswin, init_ctr->low_power_check_time_id))		
				GUI_KillTimer(msg->h_deswin, init_ctr->low_power_check_time_id);
#if BEETLES_IOCTRL_CHECK
			if( GUI_IsTimerInstalled(msg->h_deswin, init_ctr->io_ctrl_time_id))		
				GUI_KillTimer(msg->h_deswin, init_ctr->io_ctrl_time_id);
#endif
#if BEETLES_IOECHO_CHECK
			if( GUI_IsTimerInstalled(msg->h_deswin, init_ctr->io_echo_time_id))		
				GUI_KillTimer(msg->h_deswin, init_ctr->io_echo_time_id);
#endif
			if( GUI_IsTimerInstalled(msg->h_deswin, init_ctr->close_scn_time_id))		
				GUI_KillTimer(msg->h_deswin, init_ctr->close_scn_time_id);

#if BEETLES_POWEROFF_CHEAK
			if( GUI_IsTimerInstalled(msg->h_deswin, init_ctr->power_off_check_pmu_time_id))		
				GUI_KillTimer(msg->h_deswin, init_ctr->power_off_check_pmu_time_id);

            if(init_ctr->power_off_check_pmu_pin_req)
            {
                esPINS_PinGrpRel(init_ctr->power_off_check_pmu_pin_req,0);
                init_ctr->power_off_check_pmu_pin_req = NULL;
            }
#endif
			esKRNL_SemDel(init_ctr->state_sem, 0, &err);

			//James++ for power off close scn
			dsk_display_lcd_off();
			
			init_scene_destroy(msg);	
			//fifo_deinit();		
			live_exit();			
			
			eLIBs_free(init_ctr);			
			eLIBs_free(p_scene);
			
			break;
		}
		
		case GUI_MSG_CLOSE:
		{
			__here__;
			GUI_ManWinDelete(msg->h_deswin);
			
			break;		
		}

		case GUI_MSG_TIMER:
		{
			__s32 iostatu = 1;
			__s32 ioecho = 1;
			HBarState p_state;
			reg_system_para_t* para;
			init_scene_t *p_scene;
			__init_ctl_t * init_ctr;
			__bool audio_engine_is_open = EPDK_FALSE;            
            static __s32 low_power_cnt = 0;
			//__bool fm_engine_is_open = EPDK_FALSE;            
			init_ctr= (__init_ctl_t *)GUI_WinGetAttr(msg->h_deswin);			 
			p_scene = (init_scene_t *)GUI_WinGetAddData(msg->h_deswin);

			if( msg->dwAddData1 == init_ctr->low_power_check_time_id )//低电检查
			{
				if( dsk_power_is_low() == EPDK_TRUE )//低电
				{
                    low_power_cnt++;
                    if((low_power_cnt < 60) && (low_power_cnt %2 == 0))
                    {
            			{
            				__gui_msg_t 		ntymsg;

            				ntymsg.id			=  DSK_MSG_LOW_POWER_TIP;
            				ntymsg.h_srcwin 	=  NULL;
            				ntymsg.h_deswin 	=  GUI_WinGetHandFromName("beetles"); 
            				ntymsg.dwAddData1	=  NULL;
            				ntymsg.dwAddData2	=  NULL;
            				ntymsg.dwReserved	=  NULL;
            				
            				GUI_SendNotifyMessage(&ntymsg); 
            			}			

                    }
                    else if(low_power_cnt >= 60)
                    {
    					if((0x0 != GUI_WinGetHandFromName("app_movie")) || (0x0 != GUI_WinGetHandFromName("APP_DTV")) || (0x0 != GUI_WinGetHandFromName("app_music")))
    					{
    						{
    							__gui_msg_t 		ntymsg;

    							ntymsg.id			=  DSK_MSG_BOX_STANDBY_EXIT;
    							ntymsg.h_srcwin 		=  msg->h_deswin;
    							ntymsg.h_deswin 		=  GUI_WinGetHandFromName("beetles"); 
    							ntymsg.dwAddData1	=  NULL;
    							ntymsg.dwAddData2	=  NULL;
    							ntymsg.dwReserved	=  NULL;
    							
    							GUI_SendNotifyMessage(&ntymsg); 
    						}
    					}
    					else
    					{
    						__here__;
    						GUI_KillTimer(msg->h_deswin, init_ctr->low_power_check_time_id);
    						SEND_MSG(DSK_MSG_LOW_POWER, NULL, msg->h_deswin, 0, 0);
    					}
                    }
				}
                
				init_voltage_check(msg);
			}

#if BEETLES_POWEROFF_CHEAK
            else if(msg->dwAddData1 == init_ctr->power_off_check_pmu_time_id)
            {
                __bool power_off_gpio_staue;
                power_off_gpio_staue = ioctrl_get_gpio_status(init_ctr->power_off_check_pmu_gpio.port,init_ctr->power_off_check_pmu_gpio.port_num,1);
                
                if(power_off_gpio_staue)
                {
                    dsk_volume_set(0);
                    dsk_speaker_onoff(BEETLES_SPEAK_OFF);
                }
            }
#endif


#if BEETLES_IOCTRL_CHECK
			else if( msg->dwAddData1 == IO_CTRL_TIME_ID)
			{
				iostatu = ioctrl_get_gpio_status(LINEIN_PORT, LINEIN_PORTNUM,0);//pc14
				eDbug("----%d\n",iostatu);
								
				if(!iostatu)
				{
					if(p_scene->io_ctrl == EPDK_FALSE)
					{
						gscene_hbar_get_state(&p_state);
						//gscene_hbar_get_appstate(&appstate);
						//if((HBAR_ST_HIDE == p_state) && (HBAR_APP_HIDE == appstate))
						if((HBAR_ST_HIDE == p_state) || (0x0 != GUI_WinGetHandFromName("app_fm")))
						{
							init_open_screen(msg);
							__here__;
							{
								__gui_msg_t 		ntymsg;

								ntymsg.id			=  DSK_MSG_BOX_STANDBY_EXIT;
								ntymsg.h_srcwin 		=  msg->h_deswin;
								ntymsg.h_deswin 		=  GUI_WinGetHandFromName("beetles"); 
								ntymsg.dwAddData1	=  NULL;
								ntymsg.dwAddData2	=  NULL;
								ntymsg.dwReserved	=  NULL;
								
								GUI_SendNotifyMessage(&ntymsg); 
							}
						}
						else
						{
							SEND_MSG(DSK_MSG_ORCHID_UPDATE_START, NULL, msg->h_deswin, 0, 0);
						}
						
					}
				}
				else
				{
					if(p_scene->io_ctrl == EPDK_TRUE)
					{
						SEND_MSG(DSK_MSG_ORCHID_UPDATE_END, NULL, msg->h_deswin, 0, 0);
					}
				}
			}	
#endif


#if BEETLES_IOECHO_CHECK
			else if( msg->dwAddData1 == IO_ECHO_TIME_ID)
			{
				if(!((p_scene->usb_connect)||(p_scene->io_ctrl)||(p_scene->low_power)||(p_scene->power_off)))
				{
					ioecho = ioctrl_get_gpio_status(ECHO_PORT, ECHO_PORTNUM,0);

					if(!ioecho)
					{
						if(!dsk_audio_echo_is_open())
						{
							esPWM_UsrEventNotify();
							
							dsk_speaker_onoff(BEETLES_SPEAK_ON);
							
							dsk_audio_echo_close(1);
							dsk_audio_echo_open(AUDIO_REC_USR_MIC,1);
							
							esPWM_LockCpuFreq();

							dsk_audio_echo_set_delay_num(0);
							dsk_audio_echo_set_delay_time(0);
							
							dsk_volume_set(dsk_volume_get());

						}
					}
					else
					{
						if(dsk_audio_echo_is_open())
						{
							
							if(!audio_engine_is_open)
							{
								if(0x0 != GUI_WinGetHandFromName("app_fm"))
								{
									audio_engine_is_open = EPDK_TRUE;
								}
							}
							//adjust audio exist??
							if(!audio_engine_is_open)
							{
								if(0x0 != GUI_WinGetHandFromName("app_music"))
								{
									audio_engine_is_open = EPDK_TRUE;
								}
							}
							//adjust video exist??
							if(!audio_engine_is_open)
							{
								if(0x0 != GUI_WinGetHandFromName("app_movie"))
								{
									audio_engine_is_open = EPDK_TRUE;
								}
							}
							//adjust tv exist??
							if(!audio_engine_is_open)
							{
								if(0x0 != GUI_WinGetHandFromName("APP_DTV"))
								{
									audio_engine_is_open = EPDK_TRUE;
								}
							}
							

							if(!audio_engine_is_open)
							{
								dsk_audio_echo_close(1);
							}
							else
							{
								dsk_audio_echo_close(0);
							}

							
							if(!audio_engine_is_open)
							{
								dsk_speaker_onoff(BEETLES_SPEAK_OFF);
							}
							

#if BEETLES_MICIN_DISABLE
							if(0x0 != GUI_WinGetHandFromName("app_record"))
							{
								{
									__gui_msg_t 		ntymsg;

									ntymsg.id			=  DSK_MSG_BOX_STANDBY_EXIT;
									ntymsg.h_srcwin 		=  msg->h_deswin;
									ntymsg.h_deswin 		=  GUI_WinGetHandFromName("beetles"); 
									ntymsg.dwAddData1	=  NULL;
									ntymsg.dwAddData2	=  NULL;
									ntymsg.dwReserved	=  NULL;
									
									GUI_SendNotifyMessage(&ntymsg); 
								}
							}
#endif							

						}
					}
				}
			}					
#endif
			else if( msg->dwAddData1 == LOW_POWER_DIALOG_TIME_ID )//低电对话框存在时间
			{
				//__here__;
				p_scene->low_power = EPDK_FALSE;
				GUI_KillTimer(msg->h_deswin, LOW_POWER_DIALOG_TIME_ID);
				scene_on_dialog(msg);
			}

 			else if( msg->dwAddData1 == init_ctr->volume_save_check_time_id )
			{
                reg_system_para_t* para;
				GUI_KillTimer(msg->h_deswin, init_ctr->volume_save_check_time_id);
                para = (reg_system_para_t*)dsk_reg_get_para_by_app(REG_APP_SYSTEM);
                if(para)
                {
                    para->volume = dsk_volume_get();
                }
                dsk_reg_flush();
			}
           
			else if( msg->dwAddData1 == POWER_OFF_DIALOG_TIME_ID )//关机对话框存在时间
			{
				__here__;
				p_scene->power_off = EPDK_FALSE;
				GUI_KillTimer(msg->h_deswin, POWER_OFF_DIALOG_TIME_ID);
				scene_on_dialog(msg);
			}	
			else if( msg->dwAddData1 == init_ctr->close_scn_time_id )//自动关屏时间
			{		
				__here__;
				SEND_MSG(DSK_MSG_SCREEN_CLOSE, NULL, msg->h_deswin, 0, 0);
			}	
			else
			{
				__wrn(" unknown msg timer id = %d \n", msg->dwAddData1);
			}
			
			break;
		}			

		case GUI_MSG_KEY:
		{
			__bool exist;
			__init_ctl_t * init_ctr;

			init_ctr= (__init_ctl_t *)GUI_WinGetAttr(msg->h_deswin);	

			eDbug("----GUI_MSG_KEY %d, %d\n", msg->dwAddData1, msg->dwAddData2);

			exist = GUI_IsTimerInstalled(msg->h_deswin, init_ctr->close_scn_time_id);
			if( exist == EPDK_TRUE )
			{
				GUI_ResetTimer(msg->h_deswin, init_ctr->close_scn_time_id, init_ctr->closescn_timeout, NULL);
			}		

            if(msg->dwAddData1 == GUI_MSG_KEY_TV_LEFT|| msg->dwAddData1 == GUI_MSG_KEY_TV_RIGHT)
            {
				if(0x0 == GUI_WinGetHandFromName("APP_DTV"))
				{
                    switch(msg->dwAddData1)
                    {
                        case GUI_MSG_KEY_TV_LEFT:
                            msg->dwAddData1 = GUI_MSG_KEY_LEFT;
                            break;
                        case GUI_MSG_KEY_TV_RIGHT:
                            msg->dwAddData1 = GUI_MSG_KEY_RIGHT;
                            break;

                        default:
                            break;
                    }
				}

            }

            if(msg->dwAddData1 == GUI_MSG_KEY_VDEC || msg->dwAddData1 == GUI_MSG_KEY_LONGVDEC
                || msg->dwAddData1 == GUI_MSG_KEY_VADD || msg->dwAddData1 == GUI_MSG_KEY_LONGVADD)
            {
    			if(KEY_DOWN_ACTION == msg->dwAddData2 || KEY_REPEAT_ACTION == msg->dwAddData2)
    			{
        			if( GUI_IsTimerInstalled(msg->h_deswin, init_ctr->volume_save_check_time_id))		
					    GUI_KillTimer(msg->h_deswin, init_ctr->volume_save_check_time_id);
                }

                
    			if(KEY_UP_ACTION ==msg->dwAddData2)
    			{
        			if( GUI_IsTimerInstalled(msg->h_deswin, init_ctr->volume_save_check_time_id))		
        				GUI_ResetTimer(msg->h_deswin, init_ctr->volume_save_check_time_id,init_ctr->volume_save_check_timeout, NULL);

                    else
                        GUI_SetTimer(msg->h_deswin, init_ctr->volume_save_check_time_id, init_ctr->volume_save_check_timeout, NULL);

                }

            }
            
			return GUI_ManWinDefaultProc(msg);
		}
		
		case DSK_MSG_USBD_PLUG_IN:	
		{                       
			__init_ctl_t*  init_ctr = (__init_ctl_t *)GUI_WinGetAttr(msg->h_deswin);
			init_scene_t*  p_scene = NULL;
			__bool        other_dlg_exist = EPDK_FALSE;//连接usb时，是否存在其它模式框

			p_scene = (init_scene_t *)GUI_WinGetAddData(msg->h_deswin);

			//提前设置usb连接标志，防止usb连接时，低电或者关机对话框被强制关闭后程序还往下跑，导致应用被结束
			p_scene->usb_plug_in = EPDK_TRUE;

			init_ctr->closescn_timer_exist = GUI_IsTimerInstalled(msg->h_deswin, init_ctr->close_scn_time_id);
			if(EPDK_TRUE == init_ctr->closescn_timer_exist )
			{
				GUI_KillTimer(msg->h_deswin, init_ctr->close_scn_time_id);
			}

			///////////////
			////////////headbar hide
			{
				HBarApp appstate;

				gscene_hbar_get_appstate(&appstate);
				if(HBAR_APP_SHOW == appstate)
					gscene_hbar_set_appstate(HBAR_APP_HIDE);
			}
			//////
			if(p_scene->io_ctrl == EPDK_TRUE)
				SEND_MSG(DSK_MSG_ORCHID_UPDATE_END, NULL, msg->h_deswin, 0, 0);
			/////////////////////

			//USB连接到来的时候可能此时提示正在关机或者低电对话框，先结束掉他们
			//并且设置标记不让它往下跑，从而不关机。
			__here__;
			if (p_scene->power_off == EPDK_TRUE)
			{
			    other_dlg_exist = EPDK_TRUE;
			    __here__;
			    p_scene->power_off = EPDK_FALSE;
			    if(GUI_IsTimerInstalled(msg->h_deswin, POWER_OFF_DIALOG_TIME_ID))
			    {
			        __here__;
			        GUI_KillTimer(msg->h_deswin, POWER_OFF_DIALOG_TIME_ID);
			        scene_on_dialog(msg);
			    }                
			}
			if (p_scene->low_power == EPDK_TRUE)
			{
			    other_dlg_exist = EPDK_TRUE;
			    __here__;
			    p_scene->low_power = EPDK_FALSE;
			    if(GUI_IsTimerInstalled(msg->h_deswin, LOW_POWER_DIALOG_TIME_ID))
			    {
			        __here__;
			        GUI_KillTimer(msg->h_deswin, LOW_POWER_DIALOG_TIME_ID);
			        scene_on_dialog(msg);
			    }
			}

	            //如果连接usb时，存在其它模式框，则必须投递消息的方式通知usb连接，
	            //否则关闭其它对话框后，对话框消息循环未结束，会造成死锁
	            //create usb dlg
	            if(EPDK_TRUE == other_dlg_exist)
	            {
	                __gui_msg_t set_msg;
					
	                __here__;
	                eLIBs_memcpy(&set_msg, msg, sizeof(__gui_msg_t));
	                GUI_SendNotifyMessage(&set_msg);
	            }
	            else
	            {
	                __here__;
	                init_usb_device_plugin_proc(msg);
	            }
            			
			break;
		}
		
		case DSK_MSG_USBD_PLUG_OUT:
		{
			__init_ctl_t * init_ctr = (__init_ctl_t *)GUI_WinGetAttr(msg->h_deswin);

			__here__;
			////////////headbar hide
			{
				HBarApp appstate;

				gscene_hbar_get_appstate(&appstate);
				if(HBAR_APP_SHOW == appstate)
					gscene_hbar_set_appstate(HBAR_APP_HIDE);
			}
			/////////////////////
			
			init_usb_device_plugout_proc(msg);

			//usb连接拔出后，重置自动关机和自动关屏定时器                        
			if(EPDK_TRUE == init_ctr->closescn_timer_exist )
			{
				if(GUI_IsTimerInstalled(msg->h_deswin, init_ctr->close_scn_time_id))
				{
					GUI_ResetTimer(msg->h_deswin, init_ctr->close_scn_time_id, init_ctr->closescn_timeout, NULL);
				}
				else
				{
					GUI_SetTimer(msg->h_deswin, init_ctr->close_scn_time_id, init_ctr->closescn_timeout, NULL);
				}
			}

			//低电提示框弹出后插入usb，重新拔出后
			//该定时器已被删除，需重置一下
			if(!GUI_IsTimerInstalled(msg->h_deswin, init_ctr->low_power_check_time_id))		
			{
				GUI_SetTimer(msg->h_deswin, init_ctr->low_power_check_time_id, init_ctr->low_power_check_timeout, NULL);			
			}
			else
			{
				GUI_ResetTimer(msg->h_deswin, init_ctr->low_power_check_time_id, init_ctr->low_power_check_timeout, NULL);                
			}
			////////////
			activity_load_app("application://app_root");

			break;		
		}

		//redefine for io ctrl
		case DSK_MSG_ORCHID_UPDATE_START:
		{
			__init_ctl_t*  init_ctr = (__init_ctl_t *)GUI_WinGetAttr(msg->h_deswin);
			init_ctr->closescn_timer_exist = GUI_IsTimerInstalled(msg->h_deswin, init_ctr->close_scn_time_id);
			__here__;
			
			if(EPDK_TRUE == init_ctr->closescn_timer_exist )
			{
				GUI_KillTimer(msg->h_deswin, init_ctr->close_scn_time_id);
			}
			////////////headbar hide
			{
				HBarApp appstate;

				gscene_hbar_get_appstate(&appstate);
				if(HBAR_APP_SHOW == appstate)
					gscene_hbar_set_appstate(HBAR_APP_HIDE);
			}
			/////////////////////
			ioctrl_app_linein_start();
			__here__;
			init_lo_ctrl_proc(msg);				
			break;
		}

		case DSK_MSG_ORCHID_UPDATE_END:			
		{
			init_scene_t*  p_scene = NULL;
			__init_ctl_t*  init_ctr = (__init_ctl_t *)GUI_WinGetAttr(msg->h_deswin);
			p_scene = (init_scene_t *)GUI_WinGetAddData(msg->h_deswin);
			
			__here__;
			if(EPDK_TRUE == init_ctr->closescn_timer_exist )
			{
				if(GUI_IsTimerInstalled(msg->h_deswin, init_ctr->close_scn_time_id))
				{
					GUI_ResetTimer(msg->h_deswin, init_ctr->close_scn_time_id, init_ctr->closescn_timeout, NULL);
				}
				else
				{
					GUI_SetTimer(msg->h_deswin, init_ctr->close_scn_time_id, init_ctr->closescn_timeout, NULL);
				}
			}
			ioctrl_app_linein_stop();
			p_scene->io_ctrl = EPDK_FALSE;
			scene_on_dialog(msg);
			/////////////
			activity_load_app("application://app_root");

			break;
		}
		
		case DSK_MSG_FS_PART_PLUGIN:
		{
			//char diskname[4];
			__gui_msg_t msgex;
			init_scene_t*  p_scene = NULL;
			p_scene = (init_scene_t *)GUI_WinGetAddData(msg->h_deswin);
			
			//diskname[0] = msg->dwAddData2;
			//diskname[1] = '\0';
			
			//__msg("----disk %s plug in\n", diskname);
			if((p_scene->usb_connect)||(p_scene->low_power)||(p_scene->power_off))
			{
				return 0;
			}

			if( (msg->dwAddData2 == 'D')||(msg->dwAddData2 == 'E')||(msg->dwAddData2 == 'Z') )
			{
				return 0;
			}
			////////////headbar hide
			{
				HBarApp appstate;

				gscene_hbar_get_appstate(&appstate);
				if(HBAR_APP_SHOW == appstate)
					gscene_hbar_set_appstate(HBAR_APP_HIDE);
			}
			/////////////////////

			//notify top	
			msgex.id 			= DSK_MSG_FS_PART_PLUGIN;
			msgex.h_srcwin 		= 0;
			msgex.h_deswin 		= 0;
			msgex.dwAddData1 	= msg->dwAddData1;
			msgex.dwAddData2 	= msg->dwAddData2;
			activity_notify_top(&msgex);
			
			break;
		}
		
		case DSK_MSG_FS_PART_PLUGOUT:	
		{
			char diskname[4];
			__gui_msg_t msgex;
			init_scene_t*  p_scene = NULL;
			p_scene = (init_scene_t *)GUI_WinGetAddData(msg->h_deswin);

			diskname[0] = msg->dwAddData2;
			diskname[1] = '\0';
			
			//__msg("----disk %s plug out\n", diskname);
			if((p_scene->usb_connect)||(p_scene->low_power)||(p_scene->power_off))
			{
				return 0;
			}
			
			if( (msg->dwAddData2 == 'D')||(msg->dwAddData2 == 'E')||(msg->dwAddData2 == 'Z') )
			{
				return 0;
			}
			////////////headbar hide
			{
				HBarApp appstate;

				gscene_hbar_get_appstate(&appstate);
				if(HBAR_APP_SHOW == appstate)
					gscene_hbar_set_appstate(HBAR_APP_HIDE);
			}
			/////////////////////

			//notify top				
			msgex.id 			= DSK_MSG_FS_PART_PLUGOUT;
			msgex.h_srcwin 		= 0;
			msgex.h_deswin 		= 0;
			msgex.dwAddData1 	= msg->dwAddData1;
			msgex.dwAddData2 	= msg->dwAddData2;
			activity_notify_top(&msgex);	

			if(dsk_wkm_is_open())
			{				
				char filename[512];
				
				dsk_wkm_get_now_playing_file(filename);
				__msg("filename = %s\n", filename);
				if(0 == eLIBs_strnicmp(filename, diskname, 1))
				{
					__here__;
					dsk_wkm_close();
				}
			}
			
			break;
		}
		
		case DSK_MSG_SCREEN_OPEN:
		{
			init_open_screen(msg);
			break;				
		}
	
		case DSK_MSG_LOW_POWER:
		{
			__init_ctl_t * init_ctr = (__init_ctl_t *)GUI_WinGetAttr(msg->h_deswin);

			if(init_ctr->power_off)
			{
				break;
			}
			init_ctr->power_off = EPDK_TRUE;
			init_ctr->usb_power_support = usb_power_supply();
			
			////////////headbar hide
			{
				HBarApp appstate;

				gscene_hbar_get_appstate(&appstate);
				if(HBAR_APP_SHOW == appstate)
					gscene_hbar_set_appstate(HBAR_APP_HIDE);
			}
			/////////////////////
			
			__here__;
			init_low_power_proc(msg);	
			__here__;
			init_power_off_proc(msg);
			
			init_ctr->power_off = EPDK_FALSE;

			break;
		}
		
		case DSK_MSG_ENABLE_STANDBY:
		{
			__init_ctl_t* init_ctr = (__init_ctl_t *)GUI_WinGetAttr(msg->h_deswin);

			init_lock(msg);
			init_ctr->g_b_enable_standby = EPDK_TRUE;
			init_unlock(msg);
			
			break;
		}

		case DSK_MSG_DISABLE_STANDBY:
		{
			__init_ctl_t* init_ctr = (__init_ctl_t *)GUI_WinGetAttr(msg->h_deswin);

			init_lock(msg);
			init_ctr->g_b_enable_standby = EPDK_FALSE;
			init_unlock(msg);
			
			break;
		}
		
		case DSK_MSG_GET_STANDBY_FLAG:
		{
			__bool enable_standby;
			__init_ctl_t* init_ctr = (__init_ctl_t *)GUI_WinGetAttr(msg->h_deswin);
			
			init_lock(msg);
			enable_standby = init_ctr->g_b_enable_standby;
			init_unlock(msg);

			return enable_standby;
		}
		
		case DSK_MSG_POWER_OFF:
		{            
			__init_ctl_t * init_ctr = (__init_ctl_t *)GUI_WinGetAttr(msg->h_deswin);

			if(init_ctr->power_off)
			{
				__here__;
				break;
			}

			////////////headbar hide
			{
				HBarApp appstate;

				gscene_hbar_get_appstate(&appstate);
				if(HBAR_APP_SHOW == appstate)
					gscene_hbar_set_appstate(HBAR_APP_HIDE);
			}
			/////////////////////

			//key power off
			if(msg->dwAddData2 == KEY_UP_ACTION)
			{            
				init_ctr->power_off = EPDK_TRUE;

				if( GUI_IsTimerInstalled(msg->h_deswin, init_ctr->close_scn_time_id))		
					GUI_KillTimer(msg->h_deswin, init_ctr->close_scn_time_id);
				__here__;
				init_power_off_proc(msg);

				init_ctr->power_off = EPDK_FALSE;
			}
			
			break;
		}
		
		case DSK_MSG_STANDBY_WAKE_UP:
		{
			__here__;
			init_open_screen(msg);
			
			break;
		}
		
		case DSK_MSG_TVDAC_PLUGIN:
		{
			__msg("----DSK_MSG_TVDAC_PLUGIN\n");
			
			break;
		}
			
		case DSK_MSG_TVDAC_PLUGOUT:
		case DSK_MSG_HDMI_PLUGOUT:
		{		
			__msg("----DSK_MSG_TVDAC_PLUGOUT\n");

			if(dsk_display_get_output_type() != DISP_OUTPUT_TYPE_LCD)
			{               
				__gui_msg_t msgex;

				__here__;
				//notify top				
				msgex.id 			= msg->id;
				msgex.h_srcwin 		= 0;
				msgex.h_deswin 		= 0;
				msgex.dwAddData1 	= msg->dwAddData1;
				msgex.dwAddData2 	= msg->dwAddData2;
				activity_notify_top(&msgex);
			}

			break;		
		}

		case DSK_MSG_SCREEN_SWITCH:
		{			
			__here__;
			//init_scene_destroy(msg);			
			//init_scene_create(msg);				
			
			break;
		}
		
		case DSK_MSG_SET_SCN_CLOSE:
		{
			__here__;
			init_set_close_scn(msg);
			
			break;
		}

		case DSK_MSG_SCREEN_CLOSE:	
		{
			__init_ctl_t * init_ctr = (__init_ctl_t *)GUI_WinGetAttr(msg->h_deswin);

			__here__;
			if(init_ctr->closescn_gate_on == EPDK_TRUE)
			{
				init_close_screen(msg);
			}
			
			break;
		}

		case DSK_MSG_CLOSE_SCN_GATE_OFF:
		{
			__init_ctl_t * init_ctr = (__init_ctl_t *)GUI_WinGetAttr(msg->h_deswin);

			__here__;
			init_ctr->closescn_gate_on = EPDK_FALSE; 
			
			break;
		}

		case DSK_MSG_CLOSE_SCN_GATE_ON:
		{
			__bool exist;
			__init_ctl_t * init_ctr = (__init_ctl_t *)GUI_WinGetAttr(msg->h_deswin);

			__here__;
			exist = GUI_IsTimerInstalled(msg->h_deswin, init_ctr->close_scn_time_id);
			if( exist == EPDK_TRUE )
				GUI_ResetTimer(msg->h_deswin, init_ctr->close_scn_time_id, init_ctr->closescn_timeout, NULL);/* 从新设置定时器*/
			
			init_ctr->closescn_gate_on = EPDK_TRUE; 
			
			break;
		}

		case DSK_MSG_FW_UPDATE:
		{
			__here__;
#if BEETLES_UPDATE_ENABLE
			/* 执行升级过程 */
			dsk_fw_update();				

			esKRNL_TimeDly(200);					
#endif
			break;
		}
		
		case DSK_MSG_FW_START_UPDATE: //升级固件 
		{
#if BEETLES_UPDATE_ENABLE
			__init_ctl_t * init_ctr = (__init_ctl_t *)GUI_WinGetAttr(msg->h_deswin);			

			__here__;

			if( GUI_IsTimerInstalled(msg->h_deswin, init_ctr->low_power_check_time_id))		
				GUI_KillTimer(msg->h_deswin, init_ctr->low_power_check_time_id);

			if( GUI_IsTimerInstalled(msg->h_deswin, init_ctr->close_scn_time_id))		
				GUI_KillTimer(msg->h_deswin, init_ctr->close_scn_time_id);

#if BEETLES_IOCTRL_CHECK
			if( GUI_IsTimerInstalled(msg->h_deswin, init_ctr->io_ctrl_time_id))		
				GUI_KillTimer(msg->h_deswin, init_ctr->io_ctrl_time_id);
#endif
#if BEETLES_IOECHO_CHECK
			if( GUI_IsTimerInstalled(msg->h_deswin, init_ctr->io_echo_time_id))		
				GUI_KillTimer(msg->h_deswin, init_ctr->io_echo_time_id);
#endif

			////////////headbar hide
			{
				HBarApp appstate;

				gscene_hbar_get_appstate(&appstate);
				if(HBAR_APP_SHOW == appstate)
					gscene_hbar_set_appstate(HBAR_APP_HIDE);
			}
			/////////////////////
			if(0x0 != GUI_WinGetHandFromName("app_explorer"))
			{
				{
					__gui_msg_t 		ntymsg;

					ntymsg.id			=  DSK_MSG_BOX_STANDBY_EXIT;
					ntymsg.h_srcwin 		=  msg->h_deswin;
					ntymsg.h_deswin 		=  GUI_WinGetHandFromName("beetles"); 
					ntymsg.dwAddData1	=  NULL;
					ntymsg.dwAddData2	=  NULL;
					ntymsg.dwReserved	=  NULL;
					
					GUI_SendMessage(&ntymsg); 
				}
			}
			/////////////////////

			activity_finish_all();
			__here__;
			gscene_bgd_set_flag(EPDK_TRUE);
			__here__;
			gscene_bgd_refresh();
			__here__;
			gscene_hbar_set_state(HBAR_ST_HIDE);
			__here__;            
			fw_update(msg, (char*)(msg->dwAddData1));
#endif
			break;
		}

		default:
		{
			GUI_ManWinDefaultProc(msg);
			break;
		}
	}

	return EPDK_OK;
}

H_WIN init_mainwin_create(void)
{
	H_WIN  						hManWin;	
	__gui_manwincreate_para_t  	create_info;	

	eLIBs_memset(&create_info, 0, sizeof(__gui_manwincreate_para_t));

	create_info.attr            = NULL; 
	create_info.hParent			= NULL;
	create_info.hHosting 		= NULL;   
	create_info.ManWindowProc   = init_mainwin_cb;
	create_info.name            = "init"; 	
	hManWin 					= GUI_ManWinCreate(&create_info);

	if(hManWin == NULL)
	{        
		__wrn("init: create main windows failed!\n");      
		return NULL;
	}	

	return hManWin;
}

