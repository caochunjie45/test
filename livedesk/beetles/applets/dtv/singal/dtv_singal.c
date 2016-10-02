/*
***************************************
* hot.lee  2013 05 27
*
***************************************
*/

#include "apps.h"
#include "dtv_singal.h"
#include "dtv_singal_uipara.h"
#include "..//dtv_i.h"


extern __s32 g_strength;
extern __s32 g_quality;
extern dtv_ctr_t  dtv_ctr;

static  char cur[128], next[128];

#define DISPLAY_OFFSET 30


typedef struct tag_dtv_singal_ctrl
{
	dtv_singal_para_t *scene_para;

	HTHEME           theme_bg;
	
    HTHEME           theme_volume;
    
    HTHEME           theme_volume_mute;
    
	HTHEME           theme_strength[MAX_SINGAL_LEVEL];    

	HTHEME           theme_charge[MAX_CHARGE_LEVEL];
	
	HTHEME           theme_battery[MAX_CHARGE_LEVEL];

	
    __u32            signal_timeid;
    
    __u32            volume_timeid;
    
}dtv_singal_ctrl_t;


__s32 dtv_singal_cmd2parent(H_WIN hwin, __s32 id, __s32 data1, __s32 data2)
{
	__gui_msg_t msg;
	char buf[128];

	msg.h_deswin   = GUI_WinGetParent(hwin);
	msg.h_srcwin   = NULL;
	msg.id         = GUI_MSG_COMMAND;
	msg.dwAddData1 = MAKELONG(GUI_WinGetItemId(hwin), id);
	msg.dwAddData2 = data1;
	msg.dwReserved = data2;

	return (GUI_SendMessage(&msg));
}

/**********************************************************************************************************************
* 图层创建接口
**********************************************************************************************************************/
static H_WIN dtv_singal_layer_create( RECT *rect)
{
	FB	fb = {
    	{0, 0},                                   		
    	{0, 0, 0},
    	{FB_TYPE_RGB, {PIXEL_COLOR_ARGB8888, 0, 0}}, 
	};
	__disp_layer_para_t para = 
	{
    	//DISP_LAYER_WORK_MODE_PALETTE,  
		DISP_LAYER_WORK_MODE_NORMAL,
    	0, 
    	0, 
    	0, 
    	
    	0, 
    	0xff,
    	{0, 0, 0, 0},
    	{0, 0, 0, 0},
    	DISP_LAYER_OUTPUT_CHN_DE_CH1,
    	NULL
	};
	
	__layerwincreate_para_t create_info = 
	{
		"dtv_singal layer",
		NULL,
		GUI_LYRWIN_STA_SUSPEND,
		GUI_LYRWIN_NORMAL
	};
		
	fb.size.width  = rect->width;
	fb.size.height = rect->height;

	para.pipe			= 1;
	
	para.scn_win.x 		= rect->x;
	para.scn_win.y 		= rect->y;
	para.scn_win.width 	= rect->width;	
	para.scn_win.height = rect->height;	

	para.src_win.x 		= 0;
	para.src_win.y 		= 0;
	para.src_win.width 	= rect->width;	
	para.src_win.height = rect->height;	

	para.fb = &fb;
	create_info.lyrpara = &para;	
	return(GUI_LyrWinCreate(&create_info));
}


static __s32 power_level_2_display(power_level_e level)
{
	int ret = 0;
	
	switch(level)
	{
		case DSK_POWER_LEVEL_0:
			ret = 0;
			break;
		case DSK_POWER_LEVEL_1:
			ret = 1;
			break;
		case DSK_POWER_LEVEL_2:	
			ret = 2;
			break;			
		case DSK_POWER_LEVEL_3:				
			ret = 3;
			break;						
		case DSK_POWER_LEVEL_4:
			ret = 4;
			break;
			
		case DSK_POWER_LEVEL_5:
			ret = 5;
			break;
			
		default:
			ret = 0;
			break;
	}
	
	return ret;	
}



#define SINGAL_LEVEL0  0
#define SINGAL_LEVEL1  20
#define SINGAL_LEVEL2  40
#define SINGAL_LEVEL3  60
#define SINGAL_LEVEL4  80
#define SINGAL_LEVEL5  100

static __s32 dtv_get_singal_strength(void)
{

	__s32 index = 0 ;
	if(g_strength <= SINGAL_LEVEL0)
	{
		index = 0;
	}
	else if(g_strength > SINGAL_LEVEL0 && g_strength <= SINGAL_LEVEL1)
	{
		index = 1;
	}
	else if(g_strength > SINGAL_LEVEL1 && g_strength <= SINGAL_LEVEL2)
	{
		index = 2;
	}
	else if(g_strength > SINGAL_LEVEL2 && g_strength <= SINGAL_LEVEL3)
	{
		index = 3;
	}
	else if(g_strength > SINGAL_LEVEL3 && g_strength <= SINGAL_LEVEL4)
	{
		index = 4;
	}	
	else if(g_strength > SINGAL_LEVEL4 && g_strength <= SINGAL_LEVEL5)
	{
		index = 5;
	}	
	else
	{
		__wrn("the value[%d] of g_strength is out 0~100!\n",g_strength);
	}

	return index;	
}

static show_dtv_signal_status(dtv_singal_uipara_t* uipara)
{
	GUI_RECT gui_rect_signal;
	char text[128];
	H_THEME bg_bmp;
	void * bmp_buffer;

    gui_rect_signal.x0 = uipara->dtv_no_signal.x;
	gui_rect_signal.y0 = uipara->dtv_no_signal.y;
	gui_rect_signal.x1 = gui_rect_signal.x0 + uipara->dtv_no_signal.width;
	gui_rect_signal.y1 = gui_rect_signal.y0 + uipara->dtv_no_signal.height;
    
	bg_bmp = dsk_theme_open(ID_DTV_TIPS_BG_BMP);
	if(bg_bmp)
	{
		bmp_buffer = theme_hdl2buf(bg_bmp);
		if(bmp_buffer)
		{
			GUI_SetDrawMode(GUI_DRAWMODE_NORMAL);
			GUI_BMP_Draw(bmp_buffer, gui_rect_signal.x0, gui_rect_signal.y0);
		}
		dsk_theme_close(bg_bmp);
	}

	GUI_SetColor(GUI_WHITE);
	dsk_langres_get_menu_text(STRING_DTV_NO_SINGALS, text,sizeof(text));
	GUI_DispStringInRect(text , &gui_rect_signal , GUI_TA_HCENTER|GUI_TA_VCENTER);
}


static flush_dtv_signal_status(dtv_singal_uipara_t* uipara)
{
	GUI_RECT gui_rect_signal_text;
	char text[128];
	
    gui_rect_signal_text.x0 = uipara->dtv_no_signal.x;
	gui_rect_signal_text.y0 = uipara->dtv_no_signal.y;
	gui_rect_signal_text.x1 = gui_rect_signal_text.x0 + uipara->dtv_no_signal.width;
	gui_rect_signal_text.y1 = gui_rect_signal_text.y0 + uipara->dtv_no_signal.height;
    

	GUI_SetColor(GUI_WHITE);
	dsk_langres_get_menu_text(STRING_DTV_NO_SINGALS, text,sizeof(text));
	GUI_DispStringInRect(text , &gui_rect_signal_text , GUI_TA_HCENTER|GUI_TA_VCENTER);

}


static clear_dtv_signal_status(dtv_singal_uipara_t* uipara)
{
   	GUI_RECT gui_rect_signal;

    gui_rect_signal.x0 = uipara->dtv_no_signal.x;
	gui_rect_signal.y0 = uipara->dtv_no_signal.y;
	gui_rect_signal.x1 = gui_rect_signal.x0 + uipara->dtv_no_signal.width;
	gui_rect_signal.y1 = gui_rect_signal.y0 + uipara->dtv_no_signal.height;

    GUI_ClearRect(gui_rect_signal.x0, gui_rect_signal.y0, gui_rect_signal.x1, gui_rect_signal.y1); 

	return EPDK_OK;

}


static __s32 dtv_singal_paint(H_WIN h_win)
{
	dtv_singal_ctrl_t *singal_ctrl;
	dtv_singal_uipara_t* uipara;
	GUI_RECT gui_rect_channal;
	GUI_RECT gui_rect_quality;
	GUI_RECT gui_rect_strength;
    GUI_RECT vol_rect;
        	
    
	__s32 index;
	__s32 total = 0;
	char text[256]={0}; 
	char *tmp = text;


	if (GUI_LyrWinGetSta(GUI_WinGetLyrWin(h_win)) != GUI_LYRWIN_STA_ON)
	{
		return EPDK_FAIL;
	}
	
	singal_ctrl = (dtv_singal_ctrl_t *)GUI_WinGetAddData(h_win);
	
	GUI_LyrWinSel(singal_ctrl->scene_para->layer);
    

    __here__;
	uipara = (dtv_singal_uipara_t*)dtv_singal_get_uipara();


	GUI_SetColor(GUI_WHITE);
	
	GUI_SetFont(SWFFont);
	GUI_UC_SetEncodeUTF8();

	total  = maple_get_cur_service_list_num();

	{
		//draw bg 
		{
			void* pbmp;
		
			pbmp = dsk_theme_hdl2buf(singal_ctrl->theme_bg);
			if(pbmp)
			{
				GUI_BMP_Draw(pbmp, 0,0);
			}
		}

		//画信号强度图片
		if(total > 0)
		{
			index = dtv_get_singal_strength();
			{
				void* pbmp;
			
				pbmp = dsk_theme_hdl2buf(singal_ctrl->theme_strength[index]);
				if(pbmp)
				{
					GUI_BMP_Draw(pbmp, uipara->dtv_singal_strength_bmp.x,uipara->dtv_singal_strength_bmp.y);
				}
			}	

		}

		
		//画信号质量和信号强度
		if(total > 0)
		{ 
			{
			    eLIBs_sprintf(text,"Q:%d",g_quality);
				gui_rect_quality.x0 = uipara->dtv_singal_quality.x;
				gui_rect_quality.y0 = uipara->dtv_singal_quality.y;
				gui_rect_quality.x1 = gui_rect_quality.x0+uipara->dtv_singal_quality.width;
				gui_rect_quality.y1 = gui_rect_quality.y0+uipara->dtv_singal_quality.height;
		
				GUI_DispStringInRect(text, &gui_rect_quality, GUI_TA_VCENTER|GUI_TA_HCENTER);		/* GUI_TA_VCENTER,GUI_TA_TOP  */
			}
			{
				eLIBs_sprintf(text,"S:%d",g_strength);
				gui_rect_strength.x0 = uipara->dtv_singal_strength.x;
				gui_rect_strength.y0 = uipara->dtv_singal_strength.y;
				gui_rect_strength.x1 = gui_rect_strength.x0+uipara->dtv_singal_strength.width;
				gui_rect_strength.y1 = gui_rect_strength.y0+uipara->dtv_singal_strength.height;
			
				GUI_DispStringInRect(text, &gui_rect_strength, GUI_TA_VCENTER|GUI_TA_HCENTER);		/* GUI_TA_VCENTER,GUI_TA_TOP */
			}
            if(g_quality >= 10)
            {
                clear_dtv_signal_status(uipara);
            }

            else
            {
                show_dtv_signal_status(uipara);
            }
                
		}

		//画节目名称
		{ 
			//获取节目名称
			eLIBs_memset(text, 0, sizeof(text));		
			if(total <= 0)
			{
				dsk_langres_get_menu_text(STRING_DTV_NO_CHANNEL,text,sizeof(text));
			}
			else
			{
				tmp = maple_get_name_of_channel(maple_get_cur_program_index());
				eLIBs_sprintf(text,"%s",tmp);
			}
			
			__wrn("channel name = %s \n",text);
			{
                gui_rect_channal.x0 = uipara->dtv_title.x + DISPLAY_OFFSET;
				gui_rect_channal.y0 = uipara->dtv_title.y;
				gui_rect_channal.x1 = gui_rect_channal.x0+uipara->dtv_title.width;
				gui_rect_channal.y1 = gui_rect_channal.y0+uipara->dtv_title.height;
    			GUI_SetColor(0xffaf4815);
        	    GUI_FillRect(gui_rect_channal.x0,gui_rect_channal.y0,gui_rect_channal.x1,gui_rect_channal.y1);
                GUI_SetColor(GUI_WHITE);   		
		
				GUI_DispStringInRect(text, &gui_rect_channal, GUI_TA_VCENTER|GUI_TA_LEFT);
			}
		}
        //画音量图标
        {
        	void *pic_buf = NULL;
        	char vol_text[3];
        	
        	if(dsk_volume_is_on() == EPDK_TRUE)
        	{
        		pic_buf = dsk_theme_hdl2buf(singal_ctrl->theme_volume);	
        	}
        	else
        	{
        		pic_buf = dsk_theme_hdl2buf(singal_ctrl->theme_volume_mute);	
        	}

        	
        	if(pic_buf != NULL)
        	{
        		GUI_BMP_Draw(pic_buf,uipara->dtv_volume.x,uipara->dtv_volume.y);	
        	}
        	if(dsk_volume_is_on() == EPDK_TRUE)
        	{
            	eLIBs_memset(vol_text, 0, sizeof(vol_text));                
            	eLIBs_sprintf(vol_text,"%d", dsk_volume_get());
            	vol_rect.x0 = uipara->dtv_volume_txt.x;
            	vol_rect.y0 = uipara->dtv_volume_txt.y;
            	vol_rect.x1 = uipara->dtv_volume_txt.x + uipara->dtv_volume_txt.width;
            	vol_rect.y1 = uipara->dtv_volume_txt.y + uipara->dtv_volume_txt.height;	
    			GUI_SetColor(0xffaf4815);
            	GUI_FillRect(vol_rect.x0,vol_rect.y0,vol_rect.x1,vol_rect.y1);
                GUI_SetColor(GUI_WHITE);

                GUI_DispStringInRect(vol_text, &vol_rect, GUI_TA_VCENTER);	
        	}
            else
            {
            	vol_rect.x0 = uipara->dtv_volume_txt.x;
            	vol_rect.y0 = uipara->dtv_volume_txt.y;
            	vol_rect.x1 = uipara->dtv_volume_txt.x + uipara->dtv_volume_txt.width;
            	vol_rect.y1 = uipara->dtv_volume_txt.y + uipara->dtv_volume_txt.height;	
    			GUI_SetColor(0xffaf4815);
        	    GUI_FillRect(vol_rect.x0,vol_rect.y0,vol_rect.x1,vol_rect.y1);
                GUI_SetColor(GUI_WHITE);
            }

        }
        //画底层EPG信息
        {
        	GUI_RECT gui_rect;
        	static  char *cur_init, *next_init;

        	eLIBs_memset(cur,0,sizeof(cur));
        	eLIBs_memset(next,0,sizeof(next));
        	
            maple_sepg_search();
        	//底部电视节目
        	cur_init = maple_get_channel_of_curprogramname();
        	next_init = maple_get_channel_of_nextprogramname();
            
        	GUI_SetColor(GUI_DARKGREEN);	
        	gui_rect.x0 = uipara->cur_next_prog[0].x;
        	gui_rect.y0 = uipara->cur_next_prog[0].y;
        	gui_rect.x1 = gui_rect.x0+uipara->cur_next_prog[0].width;
        	gui_rect.y1 = gui_rect.y0+uipara->cur_next_prog[0].height;
        	GUI_FillRect(gui_rect.x0,gui_rect.y0,gui_rect.x1,gui_rect.y1);

        	GUI_SetColor(GUI_DARKGREEN);	
        	gui_rect.x0 = uipara->cur_next_prog[1].x;
        	gui_rect.y0 = uipara->cur_next_prog[1].y;
        	gui_rect.x1 = gui_rect.x0+uipara->cur_next_prog[1].width;
        	gui_rect.y1 = gui_rect.y0+uipara->cur_next_prog[1].height;
        	GUI_FillRect(gui_rect.x0,gui_rect.y0,gui_rect.x1,gui_rect.y1);

        	GUI_SetColor(GUI_BLACK); 
        	gui_rect.x0 = uipara->cur_next_prog[0].x + DISPLAY_OFFSET;
        	gui_rect.y0 = uipara->cur_next_prog[0].y;
        	gui_rect.x1 = gui_rect.x0+uipara->cur_next_prog[0].width;
        	gui_rect.y1 = gui_rect.y0+uipara->cur_next_prog[0].height;        	
        	GUI_DispStringInRect(cur_init, &gui_rect, GUI_TA_LEFT|GUI_TA_VCENTER);  

        	gui_rect.x0 = uipara->cur_next_prog[1].x + DISPLAY_OFFSET;
        	gui_rect.y0 = uipara->cur_next_prog[1].y;
        	gui_rect.x1 = gui_rect.x0+uipara->cur_next_prog[1].width;
        	gui_rect.y1 = gui_rect.y0+uipara->cur_next_prog[1].height;	
        	GUI_DispStringInRect(next_init, &gui_rect, GUI_TA_LEFT|GUI_TA_VCENTER); 
        }

		{
			void * bmp_buffer;
			
            __s32 is_full = 0;

            if(dsk_get_charge_state())
			{
				dtv_ctr.battery_level++;
				
				if(dtv_ctr.battery_level >= 6)
				{
				    dtv_ctr.battery_level = 0;
				}
				
			}
       
			else
			{
#if (AW_SDK_PMU == 0)	 //不带PMU
				dsk_power_get_voltage_level(&dtv_ctr.battery_level,&is_full);
#else //带PMU
				dsk_power_get_battery_level(&dtv_ctr.battery_level,&is_full);
#endif
			}

			if(EPDK_FALSE == dsk_get_charge_state())
			{
				if(singal_ctrl->theme_battery[dtv_ctr.battery_level] == NULL)
				{
					return;
				}
				
				bmp_buffer = theme_hdl2buf(singal_ctrl->theme_battery[dtv_ctr.battery_level]);
			}
			else
			{
				if(singal_ctrl->theme_charge[dtv_ctr.battery_level] == NULL)
				{
					return;
				}
				
				bmp_buffer = theme_hdl2buf(singal_ctrl->theme_charge[dtv_ctr.battery_level]);
			}

			
			if(bmp_buffer)
			{
				GUI_BMP_Draw(bmp_buffer, uipara->dtv_charge.x, uipara->dtv_charge.y);
			}
		}

	}

	return EPDK_OK;
}

static __s32 dtv_singal_res_init(__gui_msg_t *msg)
{
	__s32 total;

	int i = 0;
	
	dtv_singal_ctrl_t *singal_ctrl = NULL;
	
	dtv_singal_uipara_t *uipara = NULL;
	
	singal_ctrl = (dtv_singal_ctrl_t *)GUI_WinGetAddData(msg->h_deswin);
	if(!singal_ctrl)
	{
		__wrn("singal_ctrl is null.......\n");
		return EPDK_FAIL;
	}	

	uipara = (dtv_singal_uipara_t*)dtv_singal_get_uipara();

	total = MAX_SINGAL_LEVEL;
	
	for(i = 0; i < total; i++)
	{
		singal_ctrl->theme_strength[i] = dsk_theme_open(uipara->dtv_singal_bmp[i]);
	}

	singal_ctrl->theme_bg = dsk_theme_open(uipara->id_list_bar);		/* ID_DTV_LIST_TOP_BMP */

    singal_ctrl->theme_volume = dsk_theme_open(uipara->dtv_volume_state);

    singal_ctrl->theme_volume_mute = dsk_theme_open(uipara->dtv_volume_mute);

    //电量
	for( i=0; i< MAX_CHARGE_LEVEL; i++ )
	{
        singal_ctrl->theme_charge[i] = dsk_theme_open(uipara->id_charge[i]);

        singal_ctrl->theme_battery[i] = dsk_theme_open(uipara->id_battery[i]);
	}


	return EPDK_OK;

}

static __s32 dtv_singal_res_uninit(__gui_msg_t *msg)
{
    __s32 i;
	//__s32 total;
	dtv_singal_ctrl_t *singal_ctrl=NULL;
	
	singal_ctrl = (dtv_singal_ctrl_t *)GUI_WinGetAddData(msg->h_deswin);
	if(!singal_ctrl)
	{
		__wrn("singal_ctrl is null.......\n");
		return EPDK_FAIL;
	}

	for(i=0; i < MAX_SINGAL_LEVEL; i++)
	{
	    if(singal_ctrl->theme_strength[i])
	    {
			dsk_theme_close(singal_ctrl->theme_strength[i]);
			
			singal_ctrl->theme_strength[i] = NULL;
		}
	}

	if(singal_ctrl->theme_bg)
	{
	    dsk_theme_close(singal_ctrl->theme_bg);
	    
		singal_ctrl->theme_bg = NULL;
	}
    
	if(singal_ctrl->theme_volume)
	{
	    dsk_theme_close(singal_ctrl->theme_volume);	
	    
		singal_ctrl->theme_volume = NULL;
	}
    
	if(singal_ctrl->theme_volume_mute)
	{
	    dsk_theme_close(singal_ctrl->theme_volume_mute);
	    
		singal_ctrl->theme_volume_mute = NULL;
	}

	//电量
	for( i=0; i< MAX_CHARGE_LEVEL; i++ )
	{
		if(singal_ctrl->theme_charge[i])
		{
			dsk_theme_close(singal_ctrl->theme_charge[i]);
			singal_ctrl->theme_charge[i] = NULL ;
		}
		
		if(singal_ctrl->theme_battery[i])
		{
			dsk_theme_close(singal_ctrl->theme_battery[i]);
			singal_ctrl->theme_battery[i]	= NULL;
		}
	}	


	return EPDK_OK;
}



static __s32 _dtv_singal_proc(__gui_msg_t *msg)
{
	switch(msg->id)
	{
		case GUI_MSG_CREATE:
		{
			dtv_singal_ctrl_t *singal_ctrl = NULL;
			
			dtv_singal_para_t *dtv_singal_para = NULL;

            
			singal_ctrl = (dtv_singal_ctrl_t *)My_Malloc(0,sizeof(dtv_singal_ctrl_t));
			if(!singal_ctrl)
			{
				__msg("memory Malloc fail.........\n");
				return EPDK_FAIL;
			}
			eLIBs_memset(singal_ctrl, 0, sizeof(dtv_singal_ctrl_t));

			dtv_singal_para = (dtv_singal_para_t *)GUI_WinGetAttr(msg->h_deswin);				

			singal_ctrl->scene_para = dtv_singal_para;
			
			GUI_WinSetAddData(msg->h_deswin, (__u32)singal_ctrl);
			
			dtv_singal_res_init(msg);
			
			singal_ctrl->signal_timeid = 0x10;
			
			singal_ctrl->volume_timeid = 0x12;

			
		}	
		return EPDK_OK;

		case GUI_MSG_CLOSE:
		{

		}
		return EPDK_OK;
		
		case GUI_MSG_DESTROY:
		{
			dtv_singal_ctrl_t *singal_ctrl;

			singal_ctrl = (dtv_singal_ctrl_t *)GUI_WinGetAddData(msg->h_deswin);
			if(!singal_ctrl)
			{
				__wrn("singal_ctrl is null.......\n");
				return EPDK_FAIL;
			}
			dtv_singal_res_uninit(msg);
            
        	if (GUI_IsTimerInstalled(msg->h_deswin,singal_ctrl->signal_timeid))
        	{
        		GUI_KillTimer(msg->h_deswin,singal_ctrl->signal_timeid);
        	}  
        	
        	if (GUI_IsTimerInstalled(msg->h_deswin,singal_ctrl->volume_timeid))
        	{
        		GUI_KillTimer(msg->h_deswin,singal_ctrl->volume_timeid);
        	}            

           
			if(singal_ctrl->scene_para)
			{
				My_Mfree(0,singal_ctrl->scene_para);
			}
			
			if(singal_ctrl)
			{
				My_Mfree(0,singal_ctrl);
			}
		}
		return EPDK_OK;
		
		case GUI_MSG_PAINT:
		{

		}
		return EPDK_OK;
		
		case GUI_MSG_KEY:
		{

		}
		return EPDK_OK;
		
		case GUI_MSG_TOUCH:
		{

		}
		return EPDK_OK;
        
    	case GUI_MSG_TIMER:
	    {
			dtv_singal_ctrl_t *singal_ctrl;
			
			singal_ctrl = (dtv_singal_ctrl_t *)GUI_WinGetAddData(msg->h_deswin);
			if(!singal_ctrl)
			{
				return EPDK_FAIL;
			}
	       
	        if(msg->dwAddData1 == singal_ctrl->signal_timeid && GUI_LYRWIN_STA_ON == GUI_LyrWinGetSta(singal_ctrl->scene_para->layer) )
	        {
	        	dtv_singal_uipara_t* uipara;
	        	
	        	GUI_RECT gui_rect_channal;
	        	
	        	GUI_RECT gui_rect_quality;
	        	
	        	GUI_RECT gui_rect_strength;
	        	
	            GUI_RECT vol_rect;
	            
	        	__s32 index;
	        	
	        	__s32 total = 0;
	        	
	        	char text[256]={0}; 
	        	
	        	char *tmp = text;

	        	uipara = (dtv_singal_uipara_t*)dtv_singal_get_uipara();

	        	GUI_SetColor(GUI_WHITE);
	        	
	        	GUI_SetFont(SWFFont);
	        	GUI_UC_SetEncodeUTF8();

	        	total  = maple_get_cur_service_list_num();

	    		//画信号强度图片

	            if(total > 0)
	    		{
	    			index = dtv_get_singal_strength();
	    			{
	    				void* pbmp;
	    			
	    				pbmp = dsk_theme_hdl2buf(singal_ctrl->theme_strength[index]);
	    				if(pbmp)
	    				{
	    					GUI_BMP_Draw(pbmp, uipara->dtv_singal_strength_bmp.x,uipara->dtv_singal_strength_bmp.y);
	    				}
	    			}	

	    		}

	    		//画信号质量和信号强度
	    		if(total > 0)
	    		{ 
	    			{
	    			    eLIBs_sprintf(text,"Q:%d",g_quality);
	    				gui_rect_quality.x0 = uipara->dtv_singal_quality.x;
	    				gui_rect_quality.y0 = uipara->dtv_singal_quality.y;
	    				gui_rect_quality.x1 = gui_rect_quality.x0+uipara->dtv_singal_quality.width;
	    				gui_rect_quality.y1 = gui_rect_quality.y0+uipara->dtv_singal_quality.height;
	        			GUI_SetColor(0xffaf4815);
	            	    GUI_FillRect(gui_rect_quality.x0,gui_rect_quality.y0,gui_rect_quality.x1,gui_rect_quality.y1);
	                    GUI_SetColor(GUI_WHITE);
	    		
	    				GUI_DispStringInRect(text, &gui_rect_quality, GUI_TA_VCENTER|GUI_TA_HCENTER);		/* GUI_TA_VCENTER,GUI_TA_TOP  */
	    			}
	    			{
	    				eLIBs_sprintf(text,"S:%d",g_strength);
	    				gui_rect_strength.x0 = uipara->dtv_singal_strength.x;
	    				gui_rect_strength.y0 = uipara->dtv_singal_strength.y;
	    				gui_rect_strength.x1 = gui_rect_strength.x0+uipara->dtv_singal_strength.width;
	    				gui_rect_strength.y1 = gui_rect_strength.y0+uipara->dtv_singal_strength.height;
	        			GUI_SetColor(0xffaf4815);
	            	    GUI_FillRect(gui_rect_strength.x0,gui_rect_strength.y0,gui_rect_strength.x1,gui_rect_strength.y1);
	                    GUI_SetColor(GUI_WHITE);
	    			
	    				GUI_DispStringInRect(text, &gui_rect_strength, GUI_TA_VCENTER|GUI_TA_HCENTER);		/* GUI_TA_VCENTER,GUI_TA_TOP */
	    			}

	                if(g_quality > 0)
	                {
	                    clear_dtv_signal_status(uipara);
	                }

	                else
	                {
	                    flush_dtv_signal_status(uipara);
	                }
	                   
	    		}
	            

	            //画节目名称
	    		{ 
	    			//获取节目名称
	    			eLIBs_memset(text, 0, sizeof(text));		
	    					
	    			if(total <= 0)
	    			{
	    				dsk_langres_get_menu_text(STRING_DTV_NO_CHANNEL,text,sizeof(text));
	    			}
	    			else
	    			{
	    				tmp = maple_get_name_of_channel(maple_get_cur_program_index());
	    				eLIBs_sprintf(text,"%s",tmp);
	    			}
	    			

	    			{
	    				gui_rect_channal.x0 = uipara->dtv_title.x + DISPLAY_OFFSET;
	    				gui_rect_channal.y0 = uipara->dtv_title.y;
	    				gui_rect_channal.x1 = gui_rect_channal.x0+uipara->dtv_title.width;
	    				gui_rect_channal.y1 = gui_rect_channal.y0+uipara->dtv_title.height;
	        			GUI_SetColor(0xffaf4815);
	            	    GUI_FillRect(gui_rect_channal.x0,gui_rect_channal.y0,gui_rect_channal.x1,gui_rect_channal.y1);
	                    GUI_SetColor(GUI_WHITE);   		
	    				GUI_DispStringInRect(text, &gui_rect_channal, GUI_TA_VCENTER|GUI_TA_LEFT);
	    			}
	    		}
			
	            //画底层EPG信息
	            {
	            	char *cur_update, *next_update;
	            	GUI_RECT gui_rect;


	                maple_sepg_search();
	                
	            	//底部电视节目
	            	cur_update = maple_get_channel_of_curprogramname();
	            	next_update = maple_get_channel_of_nextprogramname();

	                if(eLIBs_strcmp(cur_update,cur) != 0)
	                {
	                    strcpy(cur,cur_update);
	                	gui_rect.x0 = uipara->cur_next_prog[0].x + DISPLAY_OFFSET;
	                	gui_rect.y0 = uipara->cur_next_prog[0].y;
	                	gui_rect.x1 = gui_rect.x0+uipara->cur_next_prog[0].width;
	                	gui_rect.y1 = gui_rect.y0+uipara->cur_next_prog[0].height;
	                	GUI_SetColor(GUI_DARKGREEN);	
	                	GUI_FillRect(gui_rect.x0,gui_rect.y0,gui_rect.x1,gui_rect.y1);
	                	GUI_SetColor(GUI_BLACK);             	
	                	GUI_DispStringInRect(cur_update, &gui_rect, GUI_TA_LEFT|GUI_TA_VCENTER);  
	                }

	                if(eLIBs_strcmp(next_update,next) != 0)
	                {
	                    strcpy(next,next_update);
	                	gui_rect.x0 = uipara->cur_next_prog[1].x + DISPLAY_OFFSET;
	                	gui_rect.y0 = uipara->cur_next_prog[1].y;
	                	gui_rect.x1 = gui_rect.x0+uipara->cur_next_prog[1].width;
	                	gui_rect.y1 = gui_rect.y0+uipara->cur_next_prog[1].height;	
	                	GUI_SetColor(GUI_DARKGREEN);	
	                	GUI_FillRect(gui_rect.x0,gui_rect.y0,gui_rect.x1,gui_rect.y1);
	                	GUI_SetColor(GUI_BLACK);
	                	GUI_DispStringInRect(next_update, &gui_rect, GUI_TA_LEFT|GUI_TA_VCENTER); 
	                }
	            }
	            
	    		{
	    			void * bmp_buffer;
	    			
	                __s32 is_full = 0;

	                if(dsk_get_charge_state())
	    			{
	    				dtv_ctr.battery_level++;
	    				if(dtv_ctr.battery_level >= 6)
	    				{
	    				    dtv_ctr.battery_level = 0;
	    				}
	    			}
	           
	    			else
	    			{
#if (AW_SDK_PMU == 0)	 //不带PMU
	    				dsk_power_get_voltage_level(&dtv_ctr.battery_level,&is_full);
#else //带PMU
	    				dsk_power_get_battery_level(&dtv_ctr.battery_level,&is_full);
#endif
	    			}

					if(EPDK_FALSE == dsk_get_charge_state())
					{
						if(singal_ctrl->theme_battery[dtv_ctr.battery_level] == NULL)
						{
							return;
						}
						
						bmp_buffer = theme_hdl2buf(singal_ctrl->theme_battery[dtv_ctr.battery_level]);
					}
					else
					{
						if(singal_ctrl->theme_charge[dtv_ctr.battery_level] == NULL)
						{
							return;
						}
						
						bmp_buffer = theme_hdl2buf(singal_ctrl->theme_charge[dtv_ctr.battery_level]);
					}
					
	    			
	    			if(bmp_buffer)
	    			{
	    				GUI_BMP_Draw(bmp_buffer, uipara->dtv_charge.x, uipara->dtv_charge.y);
	    			}
	    		}
	        }
	        
	        if(msg->dwAddData1 == singal_ctrl->volume_timeid && GUI_LYRWIN_STA_ON == GUI_LyrWinGetSta(singal_ctrl->scene_para->layer) )
	        {
	        	dtv_singal_uipara_t* uipara;
	            GUI_RECT vol_rect;

	        	uipara = (dtv_singal_uipara_t*)dtv_singal_get_uipara();

	        	GUI_SetColor(GUI_WHITE);       	
	        	GUI_SetFont(SWFFont);
	        	GUI_UC_SetEncodeUTF8();

	            //画音量图标
	            {
	            	void *pic_buf = NULL;
	            	char vol_text[3];
	            	
	            	if(dsk_volume_is_on() == EPDK_TRUE)
	            	{
	            		pic_buf = dsk_theme_hdl2buf(singal_ctrl->theme_volume);	
	            	}
	            	else
	            	{
	            		pic_buf = dsk_theme_hdl2buf(singal_ctrl->theme_volume_mute);	
	            	}

	            	
	            	if(pic_buf != NULL)
	            	{
	            		GUI_BMP_Draw(pic_buf,uipara->dtv_volume.x,uipara->dtv_volume.y);	
	            	}
	            	
	            	if(dsk_volume_is_on() == EPDK_TRUE)
	            	{
	                	eLIBs_memset(vol_text, 0, sizeof(vol_text));                
	                	eLIBs_sprintf(vol_text,"%d", dsk_volume_get());
	                	vol_rect.x0 = uipara->dtv_volume_txt.x;
	                	vol_rect.y0 = uipara->dtv_volume_txt.y;
	                	vol_rect.x1 = uipara->dtv_volume_txt.x + uipara->dtv_volume_txt.width;
	                	vol_rect.y1 = uipara->dtv_volume_txt.y + uipara->dtv_volume_txt.height;	
	        			GUI_SetColor(0xffaf4815);
	            	    GUI_FillRect(vol_rect.x0,vol_rect.y0,vol_rect.x1,vol_rect.y1);
	                    GUI_SetColor(GUI_WHITE);
	                	GUI_DispStringInRect(vol_text, &vol_rect, GUI_TA_VCENTER);	
	            	}
	            	
	                else
	                {
	                	vol_rect.x0 = uipara->dtv_volume_txt.x;
	                	vol_rect.y0 = uipara->dtv_volume_txt.y;
	                	vol_rect.x1 = uipara->dtv_volume_txt.x + uipara->dtv_volume_txt.width;
	                	vol_rect.y1 = uipara->dtv_volume_txt.y + uipara->dtv_volume_txt.height;	
	        			GUI_SetColor(0xffaf4815);
	            	    GUI_FillRect(vol_rect.x0,vol_rect.y0,vol_rect.x1,vol_rect.y1);
	                    GUI_SetColor(GUI_WHITE);
	                }
	            }            
	        }
	        
	    }
    	return EPDK_OK;


		default:
			break;
	}
	
	return GUI_FrmWinDefaultProc(msg);	
}

/*
	建立窗口
*/
static H_WIN dtv_singal_win_create(H_WIN h_parent,dtv_singal_para_t* para)
{
	__gui_framewincreate_para_t framewin_para;
	dtv_singal_para_t *scene_para;
	FB fb;


	GUI_LyrWinGetFB(para->layer, &fb);
	
	scene_para = (dtv_singal_para_t *)My_Malloc(0,sizeof(dtv_singal_para_t));
	if(!scene_para)
	{
		__msg("memory balloc fail.........\n");
		return NULL;
	}
	
	scene_para->layer = para->layer;
	__here__;
	eLIBs_memset(&framewin_para, 0, sizeof(__gui_framewincreate_para_t));

	framewin_para.name 			= SUBMENU_DTV_SG_STRENGTH;
	framewin_para.dwExStyle 	= WS_EX_NONE;
	framewin_para.dwStyle 		= WS_NONE|WS_VISIBLE;
	framewin_para.spCaption 	= NULL;
	framewin_para.hOwner		= NULL;
	framewin_para.hHosting 		= h_parent;
	framewin_para.id        	= SUBMENU_DVB_SINGAL_ID;
	framewin_para.FrameWinProc 	= (__pGUI_WIN_CB)esKRNL_GetCallBack((__pCBK_t)_dtv_singal_proc);
	framewin_para.rect.x 		= 0;
	framewin_para.rect.y 		= 0;
	framewin_para.rect.width	= fb.size.width;
	framewin_para.rect.height 	= fb.size.height;
	framewin_para.BkColor.alpha =  0;
	framewin_para.BkColor.red 	= 0;
	framewin_para.BkColor.green = 0;
	framewin_para.BkColor.blue 	= 0;
	framewin_para.attr 			=  (void *)scene_para;
	framewin_para.hLayer 		= para->layer;

	return (GUI_FrmWinCreate(&framewin_para));
}


//创建图层但使进入sleep状态，on状态由外部控制
 H_WIN dtv_singal_create(H_WIN hparent,H_LYR *layer)
{

	dtv_singal_para_t scene_para;
	
	dtv_singal_uipara_t* uipara;
	
	H_WIN h_wnd;

	uipara = dtv_singal_get_uipara();
    
	scene_para.layer = dtv_singal_layer_create(&uipara->dtv_singal_scene);
	
	
	if(NULL == scene_para.layer)
	{
	    __wrn("create layer fail....\n");
		return NULL;
	}


	*layer = scene_para.layer;

	h_wnd = dtv_singal_win_create(hparent,&scene_para);
	
	return h_wnd;	
}



//图层由sleep进入on状态,或由on进入sleep状态
__s32 dtv_singal_onff(H_WIN win, __gui_lyrwin_sta_t status)
{
	dtv_singal_ctrl_t *singal_ctrl;
	
	singal_ctrl = (dtv_singal_ctrl_t *)GUI_WinGetAddData(win);
	
	if(!singal_ctrl)
	{
		__wrn("singal_ctrl is null.......\n");
		return EPDK_FAIL;
	}
	
	if(singal_ctrl->scene_para == NULL ||singal_ctrl->scene_para->layer == NULL)
	{
		return EPDK_FAIL;
	}

	
	switch(status)
	{
		case GUI_LYRWIN_STA_OFF:
		{
		    
            if(GUI_LYRWIN_STA_OFF == GUI_LyrWinGetSta(singal_ctrl->scene_para->layer))
            {
                return EPDK_OK;
            }

		    
        	if(GUI_IsTimerInstalled(win,singal_ctrl->signal_timeid))
        	{
        		GUI_KillTimer(win,singal_ctrl->signal_timeid);
        	}            

        	if (GUI_IsTimerInstalled(win,singal_ctrl->volume_timeid))
        	{
        		GUI_KillTimer(win,singal_ctrl->volume_timeid);
        	}            

			GUI_LyrWinSetSta(singal_ctrl->scene_para->layer, GUI_LYRWIN_STA_OFF);
		
		}
		break;
		
		case GUI_LYRWIN_STA_ON:
		{	
			__here__;

            if(GUI_LYRWIN_STA_ON != GUI_LyrWinGetSta(singal_ctrl->scene_para->layer))
            {
    			GUI_LyrWinSetSta(singal_ctrl->scene_para->layer, GUI_LYRWIN_STA_ON);
    			
    			GUI_LyrWinSetTop(singal_ctrl->scene_para->layer);
    			
    			dtv_singal_paint(win);

    			
    		    if(!GUI_IsTimerInstalled(win,singal_ctrl->signal_timeid))
    	      	{
    	      		dtv_ctr.singal_timer = GUI_SetTimer(win,singal_ctrl->signal_timeid,70,NULL);
    	      	}
    	      	else
    	      	{
    	            GUI_ResetTimer(win,singal_ctrl->signal_timeid,70,NULL);
    	      	}
    	      	
     		    if (!GUI_IsTimerInstalled(win,singal_ctrl->volume_timeid))
    	      	{
    	      		dtv_ctr.vol_timer = GUI_SetTimer(win,singal_ctrl->volume_timeid,10,NULL);
    	      	}
    	      	else
    	      	{
    	            GUI_ResetTimer(win,singal_ctrl->volume_timeid,10,NULL);
    	      	}
   	      	
            }
		}
		break;

		
		default:
			break;

	}

	return EPDK_OK;
	
}


