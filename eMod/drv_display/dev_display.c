#include "drv_display_i.h"

static __u32    de_ext_hd;
extern __disp_drv_t    g_disp_drv;

__s32 disp_get_free_event(__u32 sel)
{
    __u32 i = 0;

    for(i=0; i<MAX_EVENT_SEM; i++)
    {
        if(!g_disp_drv.event_used[sel][i])
        {   
            g_disp_drv.event_used[sel][i] = 1;
            return i;
        }
    }
    return -1;
}

__s32 disp_cmd_before(__u32 sel)
{
    if(g_disp_drv.b_cache[sel] == 0)
    {
        __s32 event_id = 0;

        event_id = disp_get_free_event(sel);
        if(event_id >= 0)
        {
            g_disp_drv.event_sem[sel][event_id] = esKRNL_SemCreate(0);
            if(!g_disp_drv.event_sem[sel][event_id])
            {
                __wrn("create event_sem fail!\n");
                return EPDK_FAIL;
            }
            //__inf("de:0\n");
        }
        else
        {
            __wrn("disp_get_free_event() fail!\n");
        }
        return event_id;
    }
    return 0;
    
}

void disp_cmd_after(__u32 sel, __s32 event_id)
{
    if(g_disp_drv.b_cache[sel] == 0)
    {
        __u8 err;
		__u32 timeout;
		__u32 hz = 1000; //1000ms
		__u32 framerate;

        if(event_id >= 0 && g_disp_drv.event_sem[sel][event_id] != NULL)
        {
            if (BSP_disp_get_output_type(sel) != DISP_OUTPUT_TYPE_NONE)
			{
					framerate = BSP_disp_get_frame_rate(sel);
					timeout = (2*hz + framerate>>1)/framerate;
					g_disp_drv.event_status[sel][event_id] = 1;
					__msg("timeout = %d\n",timeout);
					esKRNL_SemPend(g_disp_drv.event_sem[sel][event_id],(timeout+5)/10,&err);
					//esKRNL_SemPend(g_disp_drv.event_sem[sel][event_id],0x50,&err);
		            //__inf("de:2\n");

		            if(err != OS_NO_ERR)
		            {
		                __wrn("pend event sem overtime!\n");
		            }
            }
            esKRNL_SemDel(g_disp_drv.event_sem[sel][event_id], 0, &err);
            g_disp_drv.event_sem[sel][event_id] = NULL;
            g_disp_drv.event_used[sel][event_id] = 0;
        }
        else
        {
            __wrn("no event sem in disp_cmd_after!\n");
        }
    }
}

__s32 disp_int_process(__u32 sel)
{
    __u32 i = 0;
    
	for(i=0; i<MAX_EVENT_SEM; i++)
	{
	    if((g_disp_drv.event_sem[sel][i] != NULL) && (g_disp_drv.event_status[sel][i] == 1))
	    {
	    	//line init
	    	//eLIBs_printf("cary line int\n");
	        esKRNL_SemPost(g_disp_drv.event_sem[sel][i]);
	        //__inf("de:1\n");
	        //g_disp_drv.event_used[sel][i] = 0;
	    }
	}
    return EPDK_OK;

}

__hdle DEV_DISP_MOpen(void * open_arg, __u32 mode)
{
    return (__hdle)&de_ext_hd;
}

__s32 DEV_DISP_MClose(__hdle     hdev)
{
    return EPDK_OK;
}

__u32 DEV_DISP_MRead(void *pBuffer, __u32 size, __u32 count, __hdle hdev)
{
    return EPDK_OK;
}

__u32 DEV_DISP_MWrite(const void *pBuffer, __u32 size, __u32 count, __hdle hdev)
{
    return EPDK_OK;
}

__s32 DEV_DISP_MIoctrl(__hdle hDev, __u32 cmd, __s32 aux, void *pbuffer)
{
    __u32 para0=0, para1=0, para2=0, para3;
    __s32 ret = 0;
    __s32 event_id = 0;
    

    if(pbuffer != NULL)
    {
        para0 = *((__u32*)pbuffer);
        para1 = *((__u32*)((__u32)pbuffer+4));
        para2 = *((__u32*)((__u32)pbuffer+8));
    }

   if(cmd !=  DISP_CMD_GET_OUTPUT_TYPE && cmd != DISP_CMD_SCN_GET_WIDTH && cmd != DISP_CMD_SCN_GET_HEIGHT && cmd != DISP_CMD_LCD_GET_BRIGHTNESS)
   {
      // __inf("de:%x\n",cmd);
    }

    if(g_disp_drv.reg_operate_vaild != 1 && cmd != DISP_CMD_CLK_ON)
    {
    	__wrn("Registers CAN'T READ OR WRITE\n",cmd);
	    return EPDK_FAIL;
    }
    
    switch(cmd)
	{
//----disp global----
	case DISP_CMD_SET_BKCOLOR:
		ret = BSP_disp_set_bk_color(aux, (__disp_color_t*)para0);
		break;

	case DISP_CMD_GET_BKCOLOR:
		ret = BSP_disp_get_bk_color(aux, (__disp_color_t*)para0);
		break;
		
	case DISP_CMD_SET_COLORKEY:
		ret = BSP_disp_set_color_key(aux, (__disp_colorkey_t *)para0);
		break;

	case DISP_CMD_SET_PALETTE_TBL:
		para3 = 1;//*((__u32*)((__u32)pbuffer+0x0c));
		ret = BSP_disp_set_palette_table(aux, (__u32 *)para0, para1, para2, para3);
		break;

	case DISP_CMD_GET_PALETTE_TBL:
		para3 = 1;//*((__u32*)((__u32)pbuffer+0x0c));
		ret = BSP_disp_get_palette_table(aux, (__u32 *)para0, para1, para2,para3);
		break;

	case DISP_CMD_START_CMD_CACHE:
		ret = BSP_disp_cmd_cache(aux);
		g_disp_drv.b_cache[aux] = 1;
		break;

	case DISP_CMD_EXECUTE_CMD_AND_STOP_CACHE:
	    g_disp_drv.b_cache[aux] = 0;
	    event_id = disp_cmd_before(aux);
		ret = BSP_disp_cmd_submit(aux);
		disp_cmd_after(aux, event_id);
		break;

	case DISP_CMD_GET_OUTPUT_TYPE:
		ret =  BSP_disp_get_output_type(aux);
		break;

	case DISP_CMD_SCN_GET_WIDTH:
		ret = BSP_disp_get_screen_width(aux);  //112350
		break;

	case DISP_CMD_SCN_GET_HEIGHT:
		ret = BSP_disp_get_screen_height(aux); //112350
		break;
	/*
	case DISP_CMD_SET_EXIT_MODE:
	  ret =  DRV_set_exit_mode(aux);
	  break;
	*/
	case DISP_CMD_SET_GAMMA_TABLE:
		ret = BSP_disp_set_gamma_table(aux, (__u32 *)para0, para1);
		break;

	case DISP_CMD_GAMMA_CORRECTION_ON:
		ret = BSP_disp_gamma_correction_enable(aux);
		break;

	case DISP_CMD_GAMMA_CORRECTION_OFF:
		ret = BSP_disp_gamma_correction_disable(aux);
		break;
	case DISP_CMD_SET_BRIGHT:
        ret = BSP_disp_set_bright(aux, para0);
		break;

    case DISP_CMD_GET_BRIGHT:
        ret = BSP_disp_get_bright(aux);
		break;

    case DISP_CMD_SET_CONTRAST:
        ret = BSP_disp_set_contrast(aux, para0);
		break;

    case DISP_CMD_GET_CONTRAST:
        ret = BSP_disp_get_contrast(aux);
		break;

    case DISP_CMD_SET_SATURATION:
        ret = BSP_disp_set_saturation(aux, para0);
		break;

    case DISP_CMD_GET_SATURATION:
        ret = BSP_disp_get_saturation(aux);
		break;

	case DISP_CMD_SET_HUE:
        ret = BSP_disp_set_hue(aux, para0);
		break;

    case DISP_CMD_GET_HUE:
        ret = BSP_disp_get_hue(aux);
		break;

		
	case DISP_CMD_CAPTURE_SCREEN:
		ret = BSP_disp_capture_screen(aux, (__disp_capture_screen_para_t *)para0);
		break;
		
	case DISP_CMD_SET_SCREEN_SIZE:
		ret = BSP_disp_set_screen_size(aux, (__disp_rectsz_t *)para0);
		break;
		
	//----iep----
        case DISP_CMD_DE_FLICKER_ON:
            ret = BSP_disp_iep_deflicker_enable(aux, 1);
            break;

        case DISP_CMD_DE_FLICKER_OFF:
            ret = BSP_disp_iep_deflicker_enable(aux, 0);
            break;

        case DISP_CMD_GET_DE_FLICKER_EN:
        	ret = BSP_disp_iep_get_deflicker_enable(aux);
        	break;

	case DISP_CMD_SET_DE_FLICKER:
		event_id = disp_cmd_before(aux);
		ret = BSP_disp_iep_deflicker_enable(aux, para0);
		disp_cmd_after(aux, event_id);
		break;			
        	
        case DISP_CMD_DRC_ON:
        	ret = BSP_disp_iep_drc_enable(aux, 1);
        	break;

        case DISP_CMD_DRC_OFF:
			ret = BSP_disp_iep_drc_enable(aux, 0);
        	break;

		case DISP_CMD_GET_DRC_EN:
			ret = BSP_disp_iep_get_drc_enable(aux);
			break;
			
        case DISP_CMD_DE_FLICKER_SET_WINDOW:
			{
				ret = BSP_disp_iep_set_demo_win(aux, 1, (__disp_rect_t*)&para0);
				break;
			}


        case DISP_CMD_DRC_SET_WINDOW:
		{
			ret = BSP_disp_iep_set_demo_win(aux, 2, (__disp_rect_t*)&para0);
			break;
		}

//----layer----
	case DISP_CMD_LAYER_REQUEST:
		ret = BSP_disp_layer_request(aux, (__disp_layer_work_mode_t)para0);
		break;
		
	case DISP_CMD_LAYER_RELEASE:
		ret = BSP_disp_layer_release(aux, para0);
		break;
		
	case DISP_CMD_LAYER_OPEN:
		ret = BSP_disp_layer_open(aux, para0);
		break;
		
	case DISP_CMD_LAYER_CLOSE:
		event_id = disp_cmd_before(aux);
		ret = BSP_disp_layer_close(aux, para0);
		disp_cmd_after(aux, event_id);
		break;
		
	case DISP_CMD_LAYER_SET_FB:
	    event_id = disp_cmd_before(aux);
		ret = BSP_disp_layer_set_framebuffer(aux, para0, (__disp_fb_t*)para1);
		disp_cmd_after(aux, event_id);
		break;

	case DISP_CMD_LAYER_GET_FB:
		ret = BSP_disp_layer_get_framebuffer(aux, para0, (__disp_fb_t*)para1);
		break;

	case DISP_CMD_LAYER_SET_SRC_WINDOW:
	    event_id = disp_cmd_before(aux);
		ret = BSP_disp_layer_set_src_window(aux,para0, (__disp_rect_t*)para1);
		disp_cmd_after(aux, event_id);
		break;

	case DISP_CMD_LAYER_GET_SRC_WINDOW:
		ret = BSP_disp_layer_get_src_window(aux,para0, (__disp_rect_t*)para1);
		break;

	case DISP_CMD_LAYER_SET_SCN_WINDOW:
	    event_id = disp_cmd_before(aux);
		ret = BSP_disp_layer_set_screen_window(aux,para0, (__disp_rect_t*)para1);
		disp_cmd_after(aux, event_id);
		break;

	case DISP_CMD_LAYER_GET_SCN_WINDOW:
		ret = BSP_disp_layer_get_screen_window(aux,para0, (__disp_rect_t*)para1);
		break;

	case DISP_CMD_LAYER_SET_PARA:
			{
		__disp_layer_info_t *pp=(__disp_layer_info_t *)para1;
		__here__;
	    event_id = disp_cmd_before(aux);
		__inf("mode = %d \n",pp->mode);
		__inf("fb.format = %d \n",pp->fb.format);
		__inf("fb.seq = %d \n",pp->fb.seq);
		__inf("scn_win.width = %d \n",pp->scn_win.width);
		__inf("scn_win.height = %d \n",pp->scn_win.height);
		__inf("alpha_en = %d \n",pp->alpha_en);
		__inf("fb.mode = %d \n",pp->fb.mode);
		__inf("fb.trd_mode = %d \n",pp->fb.trd_mode);
		__inf("b_from_screen = %d\n",pp->b_from_screen);

		//__wait__;
		ret = BSP_disp_layer_set_para(aux, para0, (__disp_layer_info_t *)para1);
		disp_cmd_after(aux, event_id);
		}
		break;

	case DISP_CMD_LAYER_GET_PARA:
		ret = BSP_disp_layer_get_para(aux, para0, (__disp_layer_info_t *)para1);
		break;

	case DISP_CMD_LAYER_TOP:
		ret = BSP_disp_layer_set_top(aux, para0);
		break;

	case DISP_CMD_LAYER_BOTTOM:
		ret = BSP_disp_layer_set_bottom(aux, para0);
		break;

	case DISP_CMD_LAYER_ALPHA_ON:
		ret = BSP_disp_layer_alpha_enable(aux, para0, 1);
		break;

	case DISP_CMD_LAYER_ALPHA_OFF:
		ret = BSP_disp_layer_alpha_enable(aux, para0, 0);
		break;

	case DISP_CMD_LAYER_SET_ALPHA_VALUE:
	    event_id = disp_cmd_before(aux);
		ret = BSP_disp_layer_set_alpha_value(aux, para0, para1);
		disp_cmd_after(aux, event_id);
		break;

	case DISP_CMD_LAYER_CK_ON:
		ret = BSP_disp_layer_colorkey_enable(aux, para0, 1);
		break;

	case DISP_CMD_LAYER_CK_OFF:
		ret = BSP_disp_layer_colorkey_enable(aux, para0, 0);
		break;

	case DISP_CMD_LAYER_SET_PIPE:
		ret = BSP_disp_layer_set_pipe(aux, para0, para1);
		break;

	case DISP_CMD_LAYER_GET_ALPHA_VALUE:
		ret = BSP_disp_layer_get_alpha_value(aux, para0);
		break;

	case DISP_CMD_LAYER_GET_ALPHA_EN:
		ret = BSP_disp_layer_get_alpha_enable(aux, para0);
		break;

	case DISP_CMD_LAYER_GET_CK_EN:
		ret = BSP_disp_layer_get_colorkey_enable(aux, para0);
		break;

	case DISP_CMD_LAYER_GET_PRIO:
		ret = BSP_disp_layer_get_piro(aux, para0);
		break;

	case DISP_CMD_LAYER_GET_PIPE:
		ret = BSP_disp_layer_get_pipe(aux, para0);
		break;

    case DISP_CMD_LAYER_SET_SMOOTH:
        ret = BSP_disp_layer_set_smooth(aux, para0,(__disp_video_smooth_t) para1);
		break;

    case DISP_CMD_LAYER_GET_SMOOTH:
        ret = BSP_disp_layer_get_smooth(aux, para0);
		break;

    case DISP_CMD_LAYER_SET_BRIGHT:
        ret = BSP_disp_layer_set_bright(aux, para0, para1);
		break;

    case DISP_CMD_LAYER_GET_BRIGHT:
        ret = BSP_disp_layer_get_bright(aux, para0);
		break;

    case DISP_CMD_LAYER_SET_CONTRAST:
        ret = BSP_disp_layer_set_contrast(aux, para0, para1);
		break;

    case DISP_CMD_LAYER_GET_CONTRAST:
        ret = BSP_disp_layer_get_contrast(aux, para0);
		break;

    case DISP_CMD_LAYER_SET_SATURATION:
        ret = BSP_disp_layer_set_saturation(aux, para0, para1);
		break;

    case DISP_CMD_LAYER_GET_SATURATION:
        ret = BSP_disp_layer_get_saturation(aux, para0);
		break;

    case DISP_CMD_LAYER_SET_HUE:
        ret = BSP_disp_layer_set_hue(aux, para0, para1);
		break;

    case DISP_CMD_LAYER_GET_HUE:
        ret = BSP_disp_layer_get_hue(aux, para0);
		break;

    case DISP_CMD_LAYER_ENHANCE_ON:
        ret = BSP_disp_layer_enhance_enable(aux, para0, 1);
		break;

    case DISP_CMD_LAYER_ENHANCE_OFF:
        ret = BSP_disp_layer_enhance_enable(aux, para0, 0);
		break;

    case DISP_CMD_LAYER_GET_ENHANCE_EN:
        ret = BSP_disp_layer_get_enhance_enable(aux, para0);
		break;

	case DISP_CMD_LAYER_VPP_ON:
	    ret = BSP_disp_layer_vpp_enable(aux, para0, 1);
		break;

    case DISP_CMD_LAYER_VPP_OFF:
        ret = BSP_disp_layer_vpp_enable(aux, para0, 0);
		break;

    case DISP_CMD_LAYER_GET_VPP_EN:
        ret = BSP_disp_layer_get_vpp_enable(aux, para0);
		break;

    case DISP_CMD_LAYER_SET_LUMA_SHARP_LEVEL:
        ret = BSP_disp_layer_set_luma_sharp_level(aux, para0, para1);
		break;

    case DISP_CMD_LAYER_GET_LUMA_SHARP_LEVEL:
        ret = BSP_disp_layer_get_luma_sharp_level(aux, para0);
		break;

    case DISP_CMD_LAYER_SET_CHROMA_SHARP_LEVEL:
        ret = BSP_disp_layer_set_chroma_sharp_level(aux, para0, para1);
		break;

    case DISP_CMD_LAYER_GET_CHROMA_SHARP_LEVEL:
        ret = BSP_disp_layer_get_chroma_sharp_level(aux, para0);
		break;

    case DISP_CMD_LAYER_SET_WHITE_EXTEN_LEVEL:
        ret = BSP_disp_layer_set_white_exten_level(aux, para0, para1);
		break;

    case DISP_CMD_LAYER_GET_WHITE_EXTEN_LEVEL:
        ret = BSP_disp_layer_get_white_exten_level(aux, para0);
		break;

    case DISP_CMD_LAYER_SET_BLACK_EXTEN_LEVEL:
        ret = BSP_disp_layer_set_black_exten_level(aux, para0, para1);
		break;

    case DISP_CMD_LAYER_GET_BLACK_EXTEN_LEVEL:
        ret = BSP_disp_layer_get_black_exten_level(aux, para0);
		break;


//----scaler----
	case DISP_CMD_SCALER_REQUEST:
			event_id = disp_cmd_before(aux);
			ret = BSP_disp_scaler_request();
			disp_cmd_after(aux, event_id);
		break;

	case DISP_CMD_SCALER_RELEASE:   
			event_id = disp_cmd_before(aux);
			ret = BSP_disp_scaler_release(para0);
			disp_cmd_after(aux, event_id);
		break;

	case DISP_CMD_SCALER_EXECUTE:
		ret = BSP_disp_scaler_start(para0,(__disp_scaler_para_t *)para1);
		break;

//----hwc----
	case DISP_CMD_HWC_OPEN:
		ret =  BSP_disp_hwc_enable(aux, 1);
		break;

	case DISP_CMD_HWC_CLOSE:
		ret =  BSP_disp_hwc_enable(aux, 0);
		break;

	case DISP_CMD_HWC_SET_POS:
		ret = BSP_disp_hwc_set_pos(aux, (__disp_pos_t*)para0);
		break;

	case DISP_CMD_HWC_GET_POS:
		ret = BSP_disp_hwc_get_pos(aux, (__disp_pos_t *)para0);
		break;

	case DISP_CMD_HWC_SET_FB:
		ret = BSP_disp_hwc_set_framebuffer(aux, (__disp_hwc_pattern_t *)para0);
		break;

	case DISP_CMD_HWC_SET_PALETTE_TABLE:
		ret = BSP_disp_hwc_set_palette(aux, (void*)para0, para1, para2);
		break;


//----video----
	case DISP_CMD_VIDEO_START:
		ret = BSP_disp_video_start(aux, para0);
		break;

	case DISP_CMD_VIDEO_STOP:
		ret = BSP_disp_video_stop(aux, para0);
		break;

	case DISP_CMD_VIDEO_SET_FB:
		if( DISP_OUTPUT_TYPE_TV == BSP_disp_get_output_type(aux))
		{
			( (__disp_video_fb_t *)para1)->interlace =  0 ;
		}
		ret = BSP_disp_video_set_fb(aux, para0, (__disp_video_fb_t *)para1);
		break;

    case DISP_CMD_VIDEO_GET_FRAME_ID:
        ret = BSP_disp_video_get_frame_id(aux, para0);
		break;

    case DISP_CMD_VIDEO_GET_DIT_INFO:
        ret = BSP_disp_video_get_dit_info(aux, para0,(__disp_dit_info_t *)para1);
		break;

//----lcd----
	case DISP_CMD_LCD_ON:
		ret = DRV_lcd_open(aux);
		break;

	case DISP_CMD_LCD_OFF:
		ret = DRV_lcd_close(aux);
		break;

	case DISP_CMD_LCD_SET_BRIGHTNESS:
		__msg("bright = %d\n",(__u32)para0);
		ret = BSP_disp_lcd_set_bright(aux, (__u32)para0,0);
		break;

	case DISP_CMD_LCD_GET_BRIGHTNESS:
		ret = BSP_disp_lcd_get_bright(aux);
		__msg("bright = %d\n",ret);
		break;

	case DISP_CMD_LCD_CPUIF_XY_SWITCH:
		ret = BSP_disp_lcd_xy_switch(aux, para0);	      
		break;

	case DISP_CMD_LCD_SET_SRC:
		ret = BSP_disp_lcd_set_src(aux, (__disp_lcdc_src_t)para0);	      
		break;
	case DISP_CMD_LCD_USER_DEFINED_FUNC:
    ret = BSP_disp_lcd_user_defined_func(aux, para0, para1, para2);
    break;
//----pwm----
  case DISP_CMD_PWM_SET_PARA:
      ret = pwm_set_para(aux, (__pwm_info_t *)para0);
      break;

  case DISP_CMD_PWM_GET_PARA:
      ret = pwm_get_para(aux, (__pwm_info_t *)para0);
      break;

//----tv----
	case DISP_CMD_TV_ON:
		ret = BSP_disp_tv_open(aux);
		break;

	case DISP_CMD_TV_OFF:
		ret = BSP_disp_tv_close(aux);
		break;

	case DISP_CMD_TV_SET_MODE:
		ret = BSP_disp_tv_set_mode(aux, (__disp_tv_mode_t)para0);
		break;

	case DISP_CMD_TV_GET_MODE:
		ret = BSP_disp_tv_get_mode(aux);
		break;

	case DISP_CMD_TV_AUTOCHECK_ON:
		ret = BSP_disp_tv_auto_check_enable(aux);
		break;

	case DISP_CMD_TV_AUTOCHECK_OFF:
		ret = BSP_disp_tv_auto_check_disable(aux);
		break;

	case DISP_CMD_TV_GET_INTERFACE:
		ret = BSP_disp_tv_get_interface(aux);
		break;

	case DISP_CMD_TV_SET_SRC:
		ret = BSP_disp_tv_set_src(aux, (__disp_lcdc_src_t)para0);	      
		break;

  case DISP_CMD_TV_GET_DAC_STATUS:
      ret =  BSP_disp_tv_get_dac_status(aux, para0);
	  //__msg("state ========================= %d\n",ret);
      break;

  case DISP_CMD_TV_SET_DAC_SOURCE:
      ret =  BSP_disp_tv_set_dac_source(aux, para0, (__disp_tv_dac_source)para1);
      break;
      
  case DISP_CMD_TV_GET_DAC_SOURCE:
      ret =  BSP_disp_tv_get_dac_source(aux, para0);
      break;

//----hdmi----
	case DISP_CMD_HDMI_ON:
		ret = BSP_disp_hdmi_open(aux);
		break;

	case DISP_CMD_HDMI_OFF:
		ret = BSP_disp_hdmi_close(aux);
		break;

	case DISP_CMD_HDMI_SET_MODE:
		ret = BSP_disp_hdmi_set_mode(aux, (__disp_tv_mode_t)para0);
		break;

	case DISP_CMD_HDMI_GET_MODE:
		ret = BSP_disp_hdmi_get_mode(aux);
		break;

	case DISP_CMD_HDMI_GET_HPD_STATUS:
	    ret = BSP_disp_hdmi_get_hpd_status(aux);
		break;

	case DISP_CMD_HDMI_SUPPORT_MODE:
		ret = BSP_disp_hdmi_check_support_mode(aux, para0);
		break;

	case DISP_CMD_HDMI_SET_SRC:
		ret = BSP_disp_hdmi_set_src(aux, (__disp_lcdc_src_t)para0);	      
		break;

//----vga----
	case DISP_CMD_VGA_ON:
		ret = BSP_disp_vga_open(aux);
		break;

	case DISP_CMD_VGA_OFF:
		ret = BSP_disp_vga_close(aux);
		break;

	case DISP_CMD_VGA_SET_MODE:
		ret = BSP_disp_vga_set_mode(aux, (__disp_vga_mode_t)para0);
		break;

	case DISP_CMD_VGA_GET_MODE:
		ret = BSP_disp_vga_get_mode(aux);
		break;

	case DISP_CMD_VGA_SET_SRC:
		ret = BSP_disp_vga_set_src(aux, (__disp_lcdc_src_t)para0);	      
		break;

//----sprite----
	case DISP_CMD_SPRITE_OPEN:
		ret = BSP_disp_sprite_open(aux);
		break;

	case DISP_CMD_SPRITE_CLOSE:
		ret = BSP_disp_sprite_close(aux);
		break;

	case DISP_CMD_SPRITE_SET_FORMAT:
		ret = BSP_disp_sprite_set_format(aux, (__disp_pixel_fmt_t)para0, (__disp_pixel_seq_t)para1);
		break;

	case DISP_CMD_SPRITE_GLOBAL_ALPHA_ENABLE:
		ret = BSP_disp_sprite_alpha_enable(aux);
		break;

	case DISP_CMD_SPRITE_GLOBAL_ALPHA_DISABLE:
		ret = BSP_disp_sprite_alpha_disable(aux);
		break;

	case DISP_CMD_SPRITE_GET_GLOBAL_ALPHA_ENABLE:
		ret = BSP_disp_sprite_get_alpha_enable(aux);
		break;

	case DISP_CMD_SPRITE_SET_GLOBAL_ALPHA_VALUE:
		ret = BSP_disp_sprite_set_alpha_vale(aux, para0);
		break;

	case DISP_CMD_SPRITE_GET_GLOBAL_ALPHA_VALUE:
		ret = BSP_disp_sprite_get_alpha_value(aux);
		break;

	case DISP_CMD_SPRITE_SET_ORDER:
		ret = BSP_disp_sprite_set_order(aux, para0,para1);
		break;

	case DISP_CMD_SPRITE_GET_TOP_BLOCK:
		ret = BSP_disp_sprite_get_top_block(aux);
		break;

	case DISP_CMD_SPRITE_GET_BOTTOM_BLOCK:
		ret = BSP_disp_sprite_get_bottom_block(aux);
		break;

	case DISP_CMD_SPRITE_SET_PALETTE_TBL:
		ret =  BSP_disp_sprite_set_palette_table(aux, (__u32 * )para0,para1,para2);
		break;

	case DISP_CMD_SPRITE_GET_BLOCK_NUM:
		ret = BSP_disp_sprite_get_block_number(aux);
		break;

	case DISP_CMD_SPRITE_BLOCK_REQUEST:
		ret = BSP_disp_sprite_block_request(aux, (__disp_sprite_block_para_t*)para0);
		break;

	case DISP_CMD_SPRITE_BLOCK_RELEASE:
		event_id = disp_cmd_before(aux);//
		ret = BSP_disp_sprite_block_release(aux, para0);
		disp_cmd_after(aux, event_id);
		break;

	case DISP_CMD_SPRITE_BLOCK_SET_SCREEN_WINDOW:
		ret = BSP_disp_sprite_block_set_screen_win(aux, para0,(__disp_rect_t *)para1);
		break;

	case DISP_CMD_SPRITE_BLOCK_GET_SCREEN_WINDOW:
		ret = BSP_disp_sprite_block_get_srceen_win(aux, para0,(__disp_rect_t *)para1);
		break;

	case DISP_CMD_SPRITE_BLOCK_SET_SOURCE_WINDOW:
		ret = BSP_disp_sprite_block_set_src_win(aux, para0,(__disp_rect_t *)para1);
		break;

	case DISP_CMD_SPRITE_BLOCK_GET_SOURCE_WINDOW:
		ret = BSP_disp_sprite_block_get_src_win(aux, para0,(__disp_rect_t *)para1);
		break;

	case DISP_CMD_SPRITE_BLOCK_SET_FB:
		ret = BSP_disp_sprite_block_set_framebuffer(aux, para0,(__disp_fb_t *)para1);
		break;

	case DISP_CMD_SPRITE_BLOCK_GET_FB:
		ret = BSP_disp_sprite_block_get_framebufer(aux, para0,(__disp_fb_t *)para1);
		break;

	case DISP_CMD_SPRITE_BLOCK_SET_TOP:
		ret = BSP_disp_sprite_block_set_top(aux, para0);
		break;

	case DISP_CMD_SPRITE_BLOCK_SET_BOTTOM:
		ret = BSP_disp_sprite_block_set_bottom(aux, para0);
		break;

	case DISP_CMD_SPRITE_BLOCK_GET_PREV_BLOCK:
		ret = BSP_disp_sprite_block_get_pre_block(aux, para0);
		break;

	case DISP_CMD_SPRITE_BLOCK_GET_NEXT_BLOCK:
		ret = BSP_disp_sprite_block_get_next_block(aux, para0);
		break;

	case DISP_CMD_SPRITE_BLOCK_GET_PRIO:
		ret = BSP_disp_sprite_block_get_prio(aux, para0);
		break;

	case DISP_CMD_SPRITE_BLOCK_OPEN:
		ret = BSP_disp_sprite_block_open(aux, para0);
		break;

	case DISP_CMD_SPRITE_BLOCK_CLOSE:
		ret = BSP_disp_sprite_block_close(aux, para0);
		break;

	case DISP_CMD_SPRITE_BLOCK_SET_PARA:
		ret = BSP_disp_sprite_block_set_para(aux, para0,(__disp_sprite_block_para_t*)para1);
		break;

	case DISP_CMD_SPRITE_BLOCK_GET_PARA:
		ret = BSP_disp_sprite_block_get_para(aux, para0,(__disp_sprite_block_para_t*)para1);
		break;
//for test
	case DISP_CMD_PRINT_REG:
	    ret = BSP_disp_print_reg(1, para0);
	    break;
	case DISP_CMD_PRINT_LCD_INFO:
	    ret = BSP_disp_print_reg(1,aux);
	    break;
	default:
	    __wrn("not supported display cmd:%x\n",cmd);
	    ret = EPDK_FAIL;
	}
	return ret;
}

__dev_devop_t dispdev_ops =
{
    DEV_DISP_MOpen,
    DEV_DISP_MClose,
    DEV_DISP_MRead,
    DEV_DISP_MWrite,
    DEV_DISP_MIoctrl
};
