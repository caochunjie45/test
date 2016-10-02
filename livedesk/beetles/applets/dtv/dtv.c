/*
*********************************************************************************************************
*											        ePDK
*						            the Easy Portable/Player Develop Kits
*									           willow app sample
*
*						        (c) Copyright 2006-2007, SoftWinners Microelectronic Co., Ltd.
*											All	Rights Reserved
*
* File    : movie.c
* By      : john.fu
* Version : V1.00
*********************************************************************************************************
*/


#include "dtv_i.h"

#include "beetles_app.h"


dtv_ctr_t  dtv_ctr;

H_WIN g_dtv_mainwin = NULL;

__s32 cur_playing_index = -1;


__bool save_flag = EPDK_FALSE;


static __s32  g_wate_cnt = 0;

static __s32 g_has_key_msg = 0;


/*
**********************************************************************************************************************
*                                               dtv_init
*
* Description:
*
* Arguments  :
*
* Returns    :
*
* Notes      :
*
**********************************************************************************************************************
*/
static void dtv_init()
{	

	/*create semphore*/
	dtv_ctr.sem = esKRNL_SemCreate(1);

	
	dtv_check_cmmb_disk_format();
	

}



/*
**********************************************************************************************************************
*                                               dtv_uninit
*
* Description:
*
* Arguments  :
*
* Returns    :
*
* Notes      :
*
**********************************************************************************************************************
*/

static void dtv_uninit(void)
{
	__u8 err;
	__s32 i;
	
	esKRNL_SemDel(dtv_ctr.sem, OS_DEL_ALWAYS, &err);
	
	/*enable auto sleep*/
	g_enable_close_scn();
	
}




static void dtv_check_cmmb_disk_format(void)
{
	ES_FILE *fp=NULL;
	//存贮盘符是否存在，不存在则需要格式盘符
	fp = eLIBs_fopen(DTV_BASEINFO_NAME, "rb");
	if(fp == NULL)
	{
		fp = eLIBs_fopen(DTV_BASEINFO_NAME, "wb");
		if(fp == NULL)//写不成功时才格式化Z盘
		{
			eLIBs_format("z:\\","fat",0);
			esKRNL_TimeDly(10);
		}
		else
		{
			//创建文件只为测试盘符是否已经格式化
			eLIBs_fclose(fp);
			fp = NULL;
			eLIBs_remove(DTV_BASEINFO_NAME);
		}
	}
	else
	{
		//文件存在，证明盘符已经格式化
		eLIBs_fclose(fp);
		fp = NULL;
	}	

}


//添加服务列表，并更新搜台总数
 __s32 search_list_finish_callback(void* arg)
{
        maple_sl_cb_param_t* p=NULL;
		__s32 program_num = 0;
		__s32 i,count=0;
        
        p = (maple_sl_cb_param_t*)arg;        

        program_num = maple_program_add_srvlist();  

//ISDBT在搜索某个频率结束时才知道台数
		esMODS_MIoctrl(maple, MAPLE_CMD_SL_GET, 0, (void*)search_service_list);
		for(i=0; i<program_num; i++)
		{
			if(search_service_list->pServItems[i].pmtId <= 0x1fcf)			
			{
				count++;
			}
			
		}
		
		program_num = count;
		
		if(program_num > 0)
		{
		   if(dtv_ctr.h_scan)
		   {
			   __gui_msg_t	   setmsg;
			   
			   setmsg.id		   =  GUI_MSG_COMMAND; 	  
			   setmsg.h_srcwin	   =  NULL;
			   setmsg.h_deswin	   =  dtv_ctr.h_scan;	
			   setmsg.dwAddData1   = SLIDE_UPDATE_CH;
			   setmsg.dwAddData2   = program_num;
			   setmsg.p_arg 	   =  NULL;
			   GUI_SendNotifyMessage(&setmsg);
			   esKRNL_TimeDly(50);
		   }			
		}


        if(p && p->pOpaque)
        {
            esKRNL_SemPost((__krnl_event_t*)p->pOpaque);
            __wrn("search service finish.\n");
        }
      
        return EPDK_OK;
}



//用于更新当前搜台数，每搜到一个台会触发一次
 __s32 search_list_event_callback(void* arg)
{
    maple_sl_cb_param_t* p;

    p = (maple_sl_cb_param_t*)arg;
   
    if(p && (p->serv_item.channelId != 0))
    {
        __msg("search one service item, channelid = %d name is: %s.\n",p->serv_item.channelId, p->serv_item.servName);
		__msg("channelId = %d pmtId = %d servId = %d tsId = %d networkId = %d vdStreamNum=%d adStreamNum=%d\n",\
			p->serv_item.channelId,p->serv_item.pmtId,\
			p->serv_item.servId,p->serv_item.tsId,p->serv_item.networkId,p->serv_item.vdStreamNum,p->serv_item.adStreamNum);			
	    //update current search result
	    __msg("p->serv_item.isCAFree = %d \n",p->serv_item.isCAFree);

#if (MAPLE_TV_STANDAR == MAPLE_TV_CMMB)		

#if IS_CA_EXIST
	//do nothing
#else
		//if(0 == p->serv_item.isCAFree)
    	{
    		__s32 servid = p->serv_item.servId;
			//if(1 == search_service_list->pServItems[i].isCAFree)//CMMB协议未定义
			//所以现在判断频道是否是收费只能通过serverid

			//排除4个固定的收费频率
			if(		CCTV1 == servid
				||  QING_CHAI_DIAN_YIN == servid
				||  CCTV5 == servid
				||  QING_CHAI_DIAN_TIANXIA == servid)			
				{
				    __here__;			        
			        return EPDK_OK;				
				}
		}
#endif//#if IS_CA_EXIST

#endif //#if (MAPLE_TV_STANDAR == MAPLE_TV_CMMB)

	   if(dtv_ctr.h_scan)
	   {
		   __gui_msg_t	   setmsg;
		   setmsg.id		   =  GUI_MSG_COMMAND; 	  
		   setmsg.h_srcwin	   =  NULL;
		   setmsg.h_deswin	   =  dtv_ctr.h_scan;	
		   setmsg.dwAddData1   = SLIDE_UPDATE_CH;
		   setmsg.dwAddData2   = 0;
		   setmsg.p_arg 	   =  NULL;
		   GUI_SendNotifyMessage(&setmsg);
		   //esKRNL_TimeDly(50);
	   }			
    }	
      
    return EPDK_OK;
}



__s32 search_sepg_finish_callback(void* arg)
{

}



//判断当前是否处在TV-OUT模式
static __bool is_on_tv(void)
{

    __disp_output_type_t output;

    output = dsk_display_get_output_type();

    if(DISP_OUTPUT_TYPE_LCD == output || DISP_OUTPUT_TYPE_NONE == output)
    {
        __wrn("not on tv now...\n");
        return EPDK_FALSE;
    }
    else
    {
        __wrn("on tv now...\n");
        return EPDK_TRUE;
    }
}


static __s32 __dtv_switch_output(void)
{
    //切换输出
	{
		reg_system_para_t* para;

		para = (reg_system_para_t*)dsk_reg_get_para_by_app(REG_APP_SYSTEM);
		if (para)
		{
			__msg("para->pal_ntsc = %d\n", para->pal_ntsc);
			if(para->output != LION_DISP_LCD)
			{
				g_display_switch_output(LION_DISP_LCD);	
			}
			else if (0 == para->pal_ntsc)
			{
				g_display_switch_output(LION_DISP_TV_NTSC);
			}
			else
			{
				g_display_switch_output(LION_DISP_TV_PAL);
			}
		}
	}
    return EPDK_OK;
}


static __s32 __root_switch_to_tv_or_lcd(__gui_msg_t *msg)
{        
    H_WIN    h_dialog = NULL; 
    
	if(EPDK_TRUE == is_on_tv())
	{
        __s32 lang_id[]={STRING_MOVIE_TIPS, STRING_DTV_EX_TVOUT };
		
        default_dialog(h_dialog, msg->h_deswin, DTV_TVOUT_ID, ADLG_OK, lang_id);
    }
	else
	{
        __s32 lang_id[]={STRING_MOVIE_TIPS, STRING_DTV_TVOUT};		
        
        default_dialog(h_dialog, msg->h_deswin, DTV_TVOUT_ID, ADLG_OK, lang_id);		
	}
	
	esKRNL_TimeDly(100);
	
	if (h_dialog)
	{
		app_dialog_destroy(h_dialog);
		h_dialog = NULL;
	}	

    __dtv_switch_output(); 
   
    return EPDK_OK;
}


static __s32 dtv_switch_tvout(__gui_msg_t *msg)
{
	__s32 cur_menu_index = 0;
	__s32 ret;

	//1、删除所有图层
	if(0 != sl_task_id)//搜索时需要还原搜索
	{
		__u8 err;

        return EPDK_OK;
        
		maple_break_search_task();
		__here__;
		maple_thread_delete(sl_task_id);
		sl_task_id = 0;
		cur_menu_index = CUR_MENU_SEARCH;
	}
	else
	{
		if(dtv_ctr.h_epg)
		{
			cur_menu_index = CUR_MENU_EPG;
		}
		else if(dtv_ctr.h_setting)
		{
			cur_menu_index = CUR_MENU_SETTING;
		}
		else if(dtv_ctr.h_serverlist)
		{
			cur_menu_index = CUR_MENU_PREVIEW;
		}				
	
	}

	dtv_delete_scene(DTV_DESTORY_ALL_SCENE);
	

    stop_record();

	//2、停止播放
	maple_stop_program(NOT_CLOSE_VIDEO_LAYER);
	
	//3、切换tv-out
	__root_switch_to_tv_or_lcd(msg);
	
	//4、重建上面删除的图层
	dtv_ctr.h_singal = dtv_singal_create(msg->h_deswin,&dtv_ctr.h_singal_layer);

	maple_play_program();

    return EPDK_OK;
}


//只在刚播放一个节目和弱信号时显示信号状态
//当然可以在想显示信号状态的时候
__s32 dtv_singal_win(__s32 status)
{
	if(NULL == dtv_ctr.h_singal_layer)
	{
	    eDbug("singal layer == NULL \n");
		return EPDK_FAIL;
	}

	{
		__gui_msg_t dmsg ={0};

		dmsg.id          = GUI_MSG_COMMAND;
		dmsg.h_deswin    = g_dtv_mainwin;
		dmsg.dwAddData1  = CMD_TO_UPDATE_SINGAL_STATUS;
		dmsg.dwAddData2  = status;
		dmsg.dwReserved  = 0;

		
		GUI_SendMessage(&dmsg);
	}

	return EPDK_OK;
}



static __s32 record_disk_cb(void *res)
{
	H_WIN hwin;	
    __s32 result = 0;

    result = (__s32)res;
    switch (result)
    {
        case MAPLE_REC_STAT_NO_DISK:
        {

        }
        break;
        
        case MAPLE_REC_STAT_DISK_FULL:
        {

        }
        break;

        case MAPLE_REC_STAT_DISK_SPACE_NOT_ENOUGH:
        {
            hwin = GUI_WinGetHandFromName(APP_DTV);
            
        	g_record_flag = 0;						

            dtv_delete_scene(DTV_DESTORY_DIALOG_WIN);
            
			esKRNL_TimeDly(2); 
			
            {
                __s32 lang_id[]={0,STRING_DTV_SPACE_IS_OVER};
        		default_dialog(dtv_ctr.h_dialog,hwin,DTV_DIALOG_RECORD_ID, ADLG_OK, lang_id);		
            }
        	return EPDK_OK;	

        }
        break;
        
        case MAPLE_REC_STAT_ABOVE_MAX:
        {

        }
        break;
            
        default :
            break;
    }
}

static start_record_proc(__gui_msg_t *msg)
{
    __u8 ret = 0;

    
    ret = maple_check_device();
    
    if( 0 == ret)
    {
        __s32 lang_id[]={0,STRING_INSERT_DISK};
		default_dialog(dtv_ctr.h_dialog,msg->h_deswin,DTV_DIALOG_RECORD_ID, ADLG_OK, lang_id);
		
        return EPDK_OK;
    }
    
    show_dtv_record_staue_win();

    //启动录制
	if(TRUE == maple_control_TS_record_start())
	{
    	g_record_flag = 1;
	}
	
    maple_set_record_disk_full_cb(record_disk_cb);

    return EPDK_OK;

}

void stop_record(void)
{
    if(g_record_flag == 1)
    {
        if(TRUE == maple_control_TS_record_stop())
        {
    		g_record_flag = 0;						
		}
		
        rat_set_modify_flag(RAT_MEDIA_TYPE_VIDEO, 1);
    }
    
}

static stop_record_proc(__gui_msg_t *msg)
{
    if(g_record_flag == 1)
    {
    	if(TRUE == maple_control_TS_record_stop())
    	{
    		g_record_flag = 0;						
		}
		
        rat_set_modify_flag(RAT_MEDIA_TYPE_VIDEO, 1);

		dtv_delete_scene(DTV_DESTORY_RECORD_LAYER);
   
        {
            __s32 lang_id[]={0,STRING_DTV_STOP_RECORDING};
    		default_dialog(dtv_ctr.h_dialog,msg->h_deswin,DTV_DIALOG_RECORD_ID, ADLG_OK, lang_id);
    		
        }
    }
    
    return EPDK_OK;

}

//信号的强弱检测
//根据信号强弱给出弱信号提示或频道播放都放在此函数中
//之所以放在GUI TIMER 定时器中检测，因为线程的优先级取太高太低都不合适
//放在定时器中可以确保消息得到及时处理，减小换台间隔
static __s32 dtv_singal_strength_detect(H_WIN hand)
{
	__s32 		ret;
	
	OS_SEM_DATA p_sem_data;
	
    __maple_status_t sta;
    
    __s32 cur_av_inf = 0;
    
    __s32  n = 0;

	static __u32 weak_status = 0;

	static __u8 signal_hide_timer = 0;


	if(dtv_is_scan_scene_exsit() == EPDK_FALSE)
	{	 
		esKRNL_SemQuery(dtv_ctr.sem,&p_sem_data);
		if(p_sem_data.OSCnt <= 0)
		{
			return EPDK_FAIL;
		}
		esKRNL_SemPend(dtv_ctr.sem,0,NULL);

	    if(maple_get_cur_service_list_num() <= 0)
	    {			        
			//无台
			if(EPDK_FALSE == dtv_has_sub_scene())
			{
				dtv_singal_win(DTV_SIGNAL_LAYER_ON);
			}						

			
			if(dsk_speaker_is_on() == EPDK_TRUE)
			{
			    dsk_speaker_onoff(TV_SPEAKER_OFF);
			}
			
			esKRNL_SemPost(dtv_ctr.sem);
			
			return EPDK_OK;
		}

	
		maple_get_ss();
		
		__msg("g_quality is %d ,g_strength is %d \n",g_quality,g_strength);

		if((g_quality < 5))
		{
	        if(g_quality <= 0)
	        {
                g_quality = 0;
                g_strength = 0;
	        }
            else
            {
                g_quality = 5;
                g_strength = 8;
            }
			g_weak_singal = EPDK_TRUE;
		}
		
		else
		{
			g_weak_singal = EPDK_FALSE;
		}



		if(EPDK_FALSE == g_weak_singal)
		{
			//信号强度达到播放要求，删除对话框
			__msg("singal reach level can play\n");
			
			weak_status	= 0;
			
			if(EPDK_TRUE == dtv_is_singer_win_exsit())
			{

				if(signal_hide_timer == 5)
				{
					signal_hide_timer = 0;
					
					dtv_delete_scene(DTV_CLOSE_SINGAL_LAYER);
					
                    if(NULL != dtv_ctr.h_record_layer)
                    {
                        if(GUI_LYRWIN_STA_ON == GUI_LyrWinGetSta(dtv_ctr.h_record_layer))
                        {
                            return EPDK_OK;
                        }

                        GUI_LyrWinSetSta(dtv_ctr.h_record_layer, GUI_LYRWIN_STA_ON);
                        
                		GUI_LyrWinSetTop(dtv_ctr.h_record_layer);					
                    }

				}

				signal_hide_timer ++;
				
			}
			
            if(dsk_speaker_is_on() == EPDK_FALSE)
            {
			    dsk_speaker_onoff(TV_SPEAKER_ON);
			}
		}


		else if(EPDK_TRUE == g_weak_singal)
		{
			//信号不满足播放要求，显示提示框
			eDbug("weak singal!\n");

			signal_hide_timer = 0;
			
			if(EPDK_FALSE == dtv_has_sub_scene())
			{			
				dtv_singal_win(DTV_SIGNAL_LAYER_ON);
			}

            if(dsk_speaker_is_on() == EPDK_TRUE)
            {
			    dsk_speaker_onoff(TV_SPEAKER_OFF);
			}

			if(10 == weak_status)
			{
				weak_status = 0;
				
				dtv_delete_scene(DTV_DESTORY_ALL_MENU_SCENE | DTV_DESTORY_DIALOG_WIN | DTV_DESTORY_DIGIT_SCENE | DTV_CLOSE_SINGAL_LAYER | DTV_CLOSE_RECORD_LAYER | DTV_DESTORY_SUBTITLE_SCENE);

				esKRNL_TimeDly(2); 
				
				{
					__gui_msg_t dmsg ={0};

					dmsg.id          = GUI_MSG_COMMAND;
					dmsg.h_deswin    = g_dtv_mainwin;
					dmsg.dwAddData1  = CMD_SWITCH_EXTERNAL;
					dmsg.dwAddData2  = 0;
					dmsg.dwReserved  = 0;

					GUI_SendNotifyMessage(&dmsg);
				}					
			}

		
			weak_status++;

		}

        //dtv_ctr.sigal_dialog存在的判断用于在弱信号时切换下一频道后，
		//中间件获取的信号质量一直为0不能播放的问题
		//预览时如果检测maple_get_decode_status会不准确，因为这时数据可能还在解码过程中
		sta = maple_get_play_status();
		
        cur_av_inf = maple_get_current_av_info();


        if(2 == cur_av_inf || 4 == cur_av_inf || 6 == cur_av_inf)//无音视频数据
        {
            n = 60;//60次
        }
    	else//如果是强信号，则延时8次再调用播放，不需要太频繁
    	{
    	   n = 4;
    	}
        
        if(1 == g_has_key_msg) //如果有按键按下,延迟播放节目
        {
            g_has_key_msg = 0;
            
            if(2 == cur_av_inf || 4 == cur_av_inf || 6 == cur_av_inf)//无音视频数据
            {
                if(g_wate_cnt > n/2)
                {
                    g_wate_cnt = n/2+1;
                }
            }
            else if(g_quality >= 10)//g_wate_cnt重新归为(n/2+1),相当于延时
            {
                if(g_wate_cnt > n/2)
                {
                    g_wate_cnt = n/2+1;
                }
            }
        }
                
        __msg("-------strength:%d g_quality:%d, cur_av_inf:%d, g_wate_cnt:%d\n", g_strength,g_quality, cur_av_inf, g_wate_cnt);
    
		if(g_weak_singal == EPDK_FALSE && (sta != MAPLE_STAT_PLAY) )       
		{           
	    //满足上述条件，尝试播放频道
			
            eDbug("n:%d, g_wate_cnt:%d\n", n, g_wate_cnt);
            
			if((g_wate_cnt++ %n) == 0)
			{
                g_wate_cnt = 0;
                
                __msg("cur_playing_index:%d, service_list->curServIdx:%d\n", cur_playing_index, service_list->curServIdx);

                if(service_list->curServIdx == cur_playing_index && sta == MAPLE_STAT_PLAY)
                {
                    __msg("alreadly playing，cur_playing_index:%d.\n", cur_playing_index);
                }
                else
                {
    				maple_stop_program(NOT_CLOSE_VIDEO_LAYER);
    				maple_play_program();
                }
			}
			
		}
				
		esKRNL_SemPost(dtv_ctr.sem);
	}

	return EPDK_OK;
}


/*
**********************************************************************************************************************
*                                               _VideoScreenInit
*
* Description:
*
* Arguments  :
*
* Returns    :
*
* Notes      :
*
**********************************************************************************************************************
*/
//是否存在子场景
static __bool dtv_has_sub_scene(void)
{
	if(dtv_ctr.h_menu_layer)
	{
		return EPDK_TRUE;
	}
	
	if(dtv_ctr.digit_layer)
	{
		return EPDK_TRUE;
	}
	
	if(dtv_ctr.h_dialog)
	{
		return EPDK_TRUE;
	}
	
	if(dtv_ctr.h_subtitle_layer)
	{
		return EPDK_TRUE;
	}
	
	if(GUI_LYRWIN_STA_ON == GUI_LyrWinGetSta(dtv_ctr.h_record_layer))
	{
		return EPDK_TRUE;
	}


	if(GUI_LYRWIN_STA_ON == GUI_LyrWinGetSta(dtv_ctr.h_singal_layer))
	{
		return EPDK_TRUE;
	}
	
	return EPDK_FALSE;
}


static __bool dtv_exit_scene(void)
{
	if(dtv_ctr.h_menu_layer)
	{
		return EPDK_FALSE;
	}
	
	if(dtv_ctr.digit_layer)
	{
		return EPDK_FALSE;
	}
	
	if(dtv_ctr.h_dialog)
	{
		return EPDK_FALSE;
	}
	
	if(dtv_ctr.h_subtitle_layer)
	{
		return EPDK_FALSE;
	}
	

	return EPDK_TRUE;
}


static __bool dtv_is_singer_win_exsit(void)
{
	if(GUI_LYRWIN_STA_ON == GUI_LyrWinGetSta(dtv_ctr.h_singal_layer))
	{
		return EPDK_TRUE;
	}
	
	return EPDK_FALSE;
}


static __bool dtv_is_scan_scene_exsit(void)
{
	if(dtv_ctr.h_scan)
	{
		return EPDK_TRUE;
	}
	return EPDK_FALSE;
}

//删除所有的子场景
static __s32 dtv_delete_scene(__u32 scene_id)
{
	if((scene_id & DTV_CLOSE_SINGAL_LAYER) == DTV_CLOSE_SINGAL_LAYER)
	{
		dtv_singal_onff(dtv_ctr.h_singal,GUI_LYRWIN_STA_OFF);
	}

	if((scene_id & DTV_CLOSE_RECORD_LAYER) == DTV_CLOSE_RECORD_LAYER)
	{
		if(NULL != dtv_ctr.h_record_layer)
		{
			if(GUI_LYRWIN_STA_OFF == GUI_LyrWinGetSta(dtv_ctr.h_record_layer))
			{
				return EPDK_OK;
			}
		
			GUI_LyrWinSetSta(dtv_ctr.h_record_layer, GUI_LYRWIN_STA_OFF);
		}
	}

	
	if((scene_id & DTV_DESTORY_ALL_MENU_SCENE) == DTV_DESTORY_ALL_MENU_SCENE)
	{
		if(NULL != dtv_ctr.h_menu_layer)
		{
			GUI_LyrWinSetBottom(dtv_ctr.h_menu_layer);
		}
		
		if(NULL != dtv_ctr.h_scan)
		{
			GUI_ManWinDelete(dtv_ctr.h_scan);
			dtv_ctr.h_scan = NULL;
		}
		
		if(NULL != dtv_ctr.h_epg)
		{
			GUI_ManWinDelete(dtv_ctr.h_epg);
			dtv_ctr.h_epg = NULL;
		}	
		if(NULL != dtv_ctr.h_audio)
		{
			GUI_ManWinDelete(dtv_ctr.h_audio);
			dtv_ctr.h_audio = NULL;
		}	
		
		if(NULL != dtv_ctr.h_serverlist)
		{
			GUI_ManWinDelete(dtv_ctr.h_serverlist);
			dtv_ctr.h_serverlist = NULL;
		}
		
		if(NULL != dtv_ctr.h_setting)
		{
			GUI_ManWinDelete(dtv_ctr.h_setting);
			dtv_ctr.h_setting = NULL;
		}

		if(NULL != dtv_ctr.h_menu_layer)
		{
			GUI_LyrWinDelete(dtv_ctr.h_menu_layer);
			dtv_ctr.h_menu_layer = NULL;
		}

	}


	if((scene_id & DTV_DESTORY_DIALOG_WIN) == DTV_DESTORY_DIALOG_WIN)
	{
		if(NULL != dtv_ctr.h_dialog)
		{
			app_dialog_destroy(dtv_ctr.h_dialog);
			dtv_ctr.h_dialog = NULL;
		}
	}

	
	if((scene_id & DTV_DESTORY_DIGIT_SCENE) == DTV_DESTORY_DIGIT_SCENE)
	{
		if(NULL != dtv_ctr.h_digit_framewin)
		{
			GUI_FrmWinDelete(dtv_ctr.h_digit_framewin);
			dtv_ctr.h_digit_framewin = NULL;
		
		}
		
		if(NULL != dtv_ctr.digit_layer)
		{
			GUI_LyrWinDelete(dtv_ctr.digit_layer);
			dtv_ctr.digit_layer = NULL;
		}
	}

	
	if((scene_id & DTV_DESTORY_SINGAL_SCENE) == DTV_DESTORY_SINGAL_SCENE)
	{
		if(NULL != dtv_ctr.h_singal)
		{
			GUI_FrmWinDelete(dtv_ctr.h_singal);
			dtv_ctr.h_singal = NULL;
		
		}

		if(NULL != dtv_ctr.h_singal_layer)
		{
			GUI_LyrWinDelete(dtv_ctr.h_singal_layer);
			dtv_ctr.h_singal_layer = NULL;
		}

	}


	if((scene_id & DTV_DESTORY_RECORD_SCENE) == DTV_DESTORY_RECORD_SCENE)
	{
		if(NULL != dtv_ctr.h_record_layer)
		{
			GUI_LyrWinDelete(dtv_ctr.h_record_layer);
			dtv_ctr.h_record_layer = NULL;
		}
	}


	if((scene_id & DTV_DESTORY_SUBTITLE_SCENE) == DTV_DESTORY_SUBTITLE_SCENE)
	{
		if(NULL != dtv_ctr.h_subtitle)
		{
			GUI_FrmWinDelete(dtv_ctr.h_subtitle);
			dtv_ctr.h_subtitle = NULL;
		}
		
		if(NULL != dtv_ctr.h_subtitle_layer)
		{
			GUI_LyrWinDelete(dtv_ctr.h_subtitle_layer);
			dtv_ctr.h_subtitle_layer = NULL;
		}
	}


	return EPDK_OK;

}


static __s32  dtv_layer_create(void)
{
	RECT rect;
    __dtv_uipara_t* dtv_uipara;
    
    dtv_uipara = dtv_get_uipara();
    
    rect.x      = dtv_uipara->main_layer.scn_rect.x;
    rect.y      = dtv_uipara->main_layer.scn_rect.y;
    rect.width  = dtv_uipara->main_layer.scn_rect.width;
    rect.height = dtv_uipara->main_layer.scn_rect.height;
    
	dtv_ctr.h_menu_layer = layer_win_create(&rect);
	if(!dtv_ctr.h_menu_layer)
	{	   
		return EPDK_FAIL;
	}
		
	return EPDK_OK;
}


static __s32 dtv_search_win_create(__gui_msg_t  *msg)
{
    __s32 ret;

    g_is_searching = 1;


	dsk_speaker_onoff(TV_SPEAKER_OFF);
	
	dtv_delete_scene(DTV_DESTORY_ALL_SCENE);
	
	if(dtv_ctr.h_menu_layer == NULL)
	{
		ret = dtv_layer_create();
		if(EPDK_FAIL == ret)
		{
		   return EPDK_FAIL;
		}
	}
	
	dtv_ctr.h_scan = search_create(msg->h_deswin,dtv_ctr.h_menu_layer);
	
	if(NULL != dtv_ctr.h_scan)
	{
		GUI_WinSetFocusChild(dtv_ctr.h_scan);
	}
	else
	{
		return EPDK_FAIL;
	}

	ret = maple_area_search();
	
	return ret;

}



static __s32 dtv_schedule_win_create(__gui_msg_t  *msg)
{
    __s32 ret;

	
	dtv_delete_scene(DTV_DESTORY_ALL_MENU_SCENE | DTV_DESTORY_DIALOG_WIN | DTV_DESTORY_DIGIT_SCENE | DTV_CLOSE_SINGAL_LAYER | DTV_CLOSE_RECORD_LAYER | DTV_DESTORY_SUBTITLE_SCENE);

	if(dtv_ctr.h_menu_layer == NULL)
	{

		ret = dtv_layer_create();
		if(EPDK_FAIL == ret)
		{
		   return EPDK_FAIL;
		}
	}

	dtv_ctr.h_epg = schedule_create(msg->h_deswin,dtv_ctr.h_menu_layer);
	
	if(NULL != dtv_ctr.h_epg)
	{
		GUI_WinSetFocusChild(dtv_ctr.h_epg);
	}
	else
	{
	    dtv_singal_win(DTV_SIGNAL_LAYER_ON);
		return EPDK_FAIL;
	}

	return ret;

}


static __s32 dtv_program_win_create(__gui_msg_t  *msg)
{
    __s32 ret;

	dtv_delete_scene(DTV_DESTORY_ALL_MENU_SCENE | DTV_DESTORY_DIALOG_WIN | DTV_DESTORY_DIGIT_SCENE | DTV_CLOSE_SINGAL_LAYER | DTV_CLOSE_RECORD_LAYER | DTV_DESTORY_SUBTITLE_SCENE);

	if(dtv_ctr.h_menu_layer == NULL)
	{
		ret = dtv_layer_create();
		
		if(EPDK_FAIL == ret)
		{
		   return EPDK_FAIL;
		}
	}

	dtv_ctr.h_serverlist = program_create(msg->h_deswin,dtv_ctr.h_menu_layer);
	
	if(NULL != dtv_ctr.h_serverlist)
	{
		GUI_WinSetFocusChild(dtv_ctr.h_serverlist);
	}
	else
	{	    
	    dtv_singal_win(DTV_SIGNAL_LAYER_ON);
		return EPDK_FAIL;
	}

	return ret;

}




static __s32 dtv_audio_win_create(__gui_msg_t *msg)
{
    __s32 ret;

    
	dtv_delete_scene(DTV_DESTORY_ALL_MENU_SCENE | DTV_DESTORY_DIALOG_WIN | DTV_DESTORY_DIGIT_SCENE | DTV_CLOSE_SINGAL_LAYER | DTV_CLOSE_RECORD_LAYER | DTV_DESTORY_SUBTITLE_SCENE);

	if(dtv_ctr.h_menu_layer == NULL)
	{
		ret = dtv_layer_create();
		
		if(EPDK_FAIL == ret)
		{
		   return EPDK_FAIL;
		}
	}    
	
	dtv_ctr.h_audio = audio_create(msg->h_deswin,dtv_ctr.h_menu_layer);
	if(NULL != dtv_ctr.h_audio)
	{
		GUI_WinSetFocusChild(dtv_ctr.h_audio);
	}
	else
	{
	    dtv_singal_win(DTV_SIGNAL_LAYER_ON);
		return EPDK_FAIL;
	}

	return ret;

}


static __s32 dtv_setting_win_create(__gui_msg_t  *msg)
{
    __s32 ret;

    
	dtv_delete_scene(DTV_DESTORY_ALL_MENU_SCENE | DTV_DESTORY_DIALOG_WIN | DTV_DESTORY_DIGIT_SCENE | DTV_CLOSE_SINGAL_LAYER | DTV_CLOSE_RECORD_LAYER | DTV_DESTORY_SUBTITLE_SCENE);

	if(dtv_ctr.h_menu_layer == NULL)
	{
		ret = dtv_layer_create();
		if(EPDK_FAIL == ret)
		{
		   return EPDK_FAIL;
		}
	}    
	
	dtv_ctr.h_setting = setting_create(msg->h_deswin,dtv_ctr.h_menu_layer);
	
	if(NULL != dtv_ctr.h_setting)
	{
		GUI_WinSetFocusChild(dtv_ctr.h_setting);
	}
	else
	{
	    dtv_singal_win(DTV_SIGNAL_LAYER_ON);
		return EPDK_FAIL;
	}


	return ret;

}


static __s32 dtv_subtitle_create(__gui_msg_t  *msg)
{
    __s32 ret;

	dtv_delete_scene(DTV_DESTORY_ALL_MENU_SCENE | DTV_DESTORY_DIALOG_WIN | DTV_DESTORY_DIGIT_SCENE | DTV_CLOSE_SINGAL_LAYER | DTV_CLOSE_RECORD_LAYER);


	if(dtv_ctr.h_subtitle_layer == NULL)
	{
		dtv_ctr.h_subtitle_layer = dvb_subtitle_layer_create();
		if(EPDK_FAIL == dtv_ctr.h_subtitle_layer)
		{
		   return EPDK_FAIL;
		}
	}    

	dtv_ctr.h_subtitle = subtitle_create(msg->h_deswin,dtv_ctr.h_subtitle_layer);
	
	if(NULL != dtv_ctr.h_subtitle)
	{
		GUI_WinSetFocusChild(dtv_ctr.h_subtitle);
	}
	else
	{
	    dtv_singal_win(DTV_SIGNAL_LAYER_ON);
		return EPDK_FAIL;
	}


	return ret;

}

/*
**********************************************************************************************************************
*                                               dtv_main_proc
*
* Description:
*
* Arguments  :
*
* Returns    :
*
* Notes      :
*
**********************************************************************************************************************
*/

static __s32 dtv_main_create_proc(__gui_msg_t *msg)
{
	
	__s32 ret;
	
	H_WIN h_loading = NULL;	
	
	dsk_speaker_onoff(TV_SPEAKER_OFF);
	
	// 唤醒CPU
	esPWM_UsrEventNotify();
	esKRNL_TimeDly(5);


	esPWM_LockCpuFreq();
	
	esPWM_UsrEventNotify();

	
	audio_mode_switch(AUDIO_FM_MODE);


	//启动加载动画//
	h_loading = app_loading_win_create(msg->h_deswin,DVB_LOADING_ID);


	dtv_init();

	//可以在此添加提示正在加载的动画
	ret = maple_open(); 
	
	if(EPDK_FAIL == ret)
	{
		__wrn("maple open fail...\n");
	   goto err0;
	}

	//加载maple模块成功后，需要在此释放动画
	if(h_loading)
	{
		app_loading_win_delete(h_loading);
		h_loading = NULL;
	}


	maple_register_search_callback(search_list_event_callback,search_list_finish_callback);
	maple_register_sepg_callback(search_sepg_finish_callback);
	maple_set_init_screensize();

	maple_set_default_char_coding_type();

	
	{
		__pCBK_t			callback;

		
	    callback = esKRNL_GetCallBack(dtv_subtitle_paint);

		esMODS_MIoctrl( maple, MAPLE_CMD_SET_SPLY_DRAW_CB, NULL, (void*)callback );
	}	


	ret = maple_program_load(DTV_DATABASE_NAME);


    if(EPDK_OK !=  ret)
	{
		ret = dtv_search_win_create(msg);			
		if(EPDK_FAIL == ret)
		{
		   goto err2;
		}
	}
	else
	{
		maple_epg_search_stop();
		
		maple_set_cur_channel(maple_get_cur_program_index());
		
		maple_search_stop();
		
		maple_stop_program(NOT_CLOSE_VIDEO_LAYER);
		
		maple_play_program();
		
		if(maple_get_cur_service_list_num() >= 1)
		{
			dsk_speaker_onoff(TV_SPEAKER_ON);
		}

	}


	dtv_ctr.h_singal = dtv_singal_create(msg->h_deswin,&dtv_ctr.h_singal_layer);

	
	if (!GUI_IsTimerInstalled(msg->h_deswin, DVB_SINGAL_TIMER_ID))
	{
		dtv_ctr.singal_timer = GUI_SetTimer(msg->h_deswin,DVB_SINGAL_TIMER_ID,100,NULL);
	}

	
	return EPDK_OK;


	
err2:
	GUI_LyrWinDelete(dtv_ctr.h_menu_layer);
	
	dtv_ctr.h_menu_layer = NULL;
err1:			
	maple_close();
	__here__;

	
err0:
	dtv_uninit();
	
	if(h_loading)
	{
		app_loading_win_delete(h_loading);	
	}

	audio_mode_switch(AUDIO_NOR_MODE);
	
	return EPDK_FAIL;

}


////dtv_key_proc
//如果当前处于搜索界面，除返回键外不响应任何其它按键
static __s32 dtv_key_proc(__gui_msg_t *msg)
{
	__s32 ret = 1;

	__msg("dtv_key_proc() : msg->dwAddData1 = %d, msg->dwAddData2 = %d\n", msg->dwAddData1, msg->dwAddData2);

    g_has_key_msg = 1;

    if(1 == g_is_searching || NULL != dtv_ctr.h_scan)
    {
        if(GUI_MSG_KEY_TVOUT == msg->dwAddData1 || GUI_MSG_KEY_LONGTVOUT == msg->dwAddData1
            ||GUI_MSG_KEY_MOVIE == msg->dwAddData1 || GUI_MSG_KEY_LONGMOVIE == msg->dwAddData1
            ||GUI_MSG_KEY_MUSIC == msg->dwAddData1 || GUI_MSG_KEY_LONGMUSIC == msg->dwAddData1
            ||GUI_MSG_KEY_FM == msg->dwAddData1 || GUI_MSG_KEY_LONGFM == msg->dwAddData1
            ||GUI_MSG_KEY_MENU == msg->dwAddData1 || GUI_MSG_KEY_LONGMENU == msg->dwAddData1
            ||GUI_MSG_KEY_ESCAPE)
        {
            
        }
        else//搜台过程屏蔽不需要的按键。
        {
            return EPDK_OK;
        }
    }
    
    eDbug("---key enter-- \n");

    if (KEY_DOWN_ACTION == msg->dwAddData2)
    {
        if(dtv_is_scan_scene_exsit() == EPDK_FALSE && msg->dwAddData1 >= GUI_MSG_KEY_NUM0 && msg->dwAddData1 <= GUI_MSG_KEY_NUM9)
        {				
			if(NULL == dtv_ctr.digit_layer)
			{
				digit_frmwin_para_t digit_para;

				dtv_delete_scene(DTV_DESTORY_ALL_MENU_SCENE | DTV_DESTORY_DIALOG_WIN | DTV_CLOSE_SINGAL_LAYER | DTV_CLOSE_RECORD_LAYER | DTV_DESTORY_SUBTITLE_SCENE);
				
				dtv_ctr.digit_layer = digit_layer_create();
				
				if(NULL == dtv_ctr.digit_layer)
				{
					return EPDK_FAIL;
				}
				
				digit_para.digit_layer = dtv_ctr.digit_layer;
				
				dtv_ctr.h_digit_framewin = digit_win_create(msg->h_deswin,&digit_para);
				
				GUI_WinSetFocusChild(dtv_ctr.h_digit_framewin);
				
				return EPDK_FAIL;
			}
			
		}

            
		switch(msg->dwAddData1)
		{
			case GUI_MSG_KEY_SCAN://搜台
			{
				if(dtv_is_scan_scene_exsit() == EPDK_FALSE)
				{
					maple_set_cur_channel(0);//isdbt默认播放第一个频点的第一个台
					
					stop_record();
					
					maple_epg_search_stop();
					
		            maple_search_stop();
		            
					maple_stop_program(CLOSE_VIDEO_LAYER);						
					  					    
					dtv_search_win_create(msg);
				}
				else
				{
					__msg("....................................................\n");
				}
				return EPDK_OK;
			}

			case GUI_MSG_KEY_EPG:
			{
				if(dtv_is_scan_scene_exsit() == EPDK_FALSE && service_list->servNum > 0  && !dtv_ctr.h_epg)
				{
					if(g_weak_singal == EPDK_FALSE)
					{
				        maple_epg_getdata();
				    }
                    else
                    {
                        maple_epg_reset_schdl_list_day(0);
					}
					
					dtv_schedule_win_create(msg);
					
					return EPDK_OK;
				}
				else
				{
		            __msg("...............\n");
				}
				return EPDK_FAIL;
			}

            
			case GUI_MSG_KEY_INFO:
			{
				if(EPDK_FALSE == dtv_has_sub_scene())
				{
					dtv_singal_win(DTV_SIGNAL_LAYER_ON);
				}
				return EPDK_OK;
			}


			case GUI_MSG_KEY_LIST:
			{
				if(dtv_is_scan_scene_exsit() == EPDK_FALSE && service_list->servNum > 0 && !dtv_ctr.h_serverlist )
				{
					dtv_program_win_create(msg);
				}
			
				return EPDK_OK;
			
			}


			case GUI_MSG_KEY_AUDIO_CH:
			{
				if(dtv_is_scan_scene_exsit() == EPDK_FALSE && service_list->servNum > 0 && !dtv_ctr.h_audio)
				{
					dtv_audio_win_create(msg);
				}
				return EPDK_OK;
			}

			
			case GUI_MSG_KEY_MENU:	//设置
			{
				if(dtv_is_scan_scene_exsit() == EPDK_FALSE && !dtv_ctr.h_setting)
				{
				   
					dtv_setting_win_create(msg);
					
					return EPDK_OK;
				}
				return EPDK_FAIL;
			}

			case GUI_MSG_KEY_DTVREC:
			{
				if(dtv_is_scan_scene_exsit() == EPDK_TRUE)
				{
					return EPDK_FAIL;
				}
				
				if((EPDK_FALSE == g_weak_singal)&&( g_record_flag == 0))
				{
					dtv_delete_scene(DTV_DESTORY_ALL_MENU_SCENE | DTV_DESTORY_DIALOG_WIN | DTV_CLOSE_SINGAL_LAYER | DTV_DESTORY_SUBTITLE_SCENE);

					start_record_proc(msg);
				}
				else
				{
					if(g_record_flag == 1)
					{
						dtv_delete_scene(DTV_DESTORY_ALL_MENU_SCENE | DTV_DESTORY_DIALOG_WIN | DTV_CLOSE_SINGAL_LAYER | DTV_DESTORY_SUBTITLE_SCENE);

						stop_record_proc(msg);
					}
				}
				return EPDK_OK;
			}
			break;

		
			case GUI_MSG_KEY_VADD:
            case GUI_MSG_KEY_VDEC:
            case GUI_MSG_KEY_MUTE:
			{
				if(dtv_is_scan_scene_exsit() == EPDK_FALSE && GUI_LYRWIN_STA_ON != GUI_LyrWinGetSta(dtv_ctr.h_singal_layer))
				{
					//音量和字幕坐标如果重叠需要暂停字幕
					dtv_delete_scene(DTV_DESTORY_ALL_MENU_SCENE | DTV_DESTORY_DIALOG_WIN | DTV_DESTORY_DIGIT_SCENE | DTV_CLOSE_RECORD_LAYER | DTV_DESTORY_SUBTITLE_SCENE);

					esKRNL_TimeDly(2);
					
                    dtv_singal_win(DTV_SIGNAL_LAYER_ON);
				}
				return EPDK_OK;
			}
			

		    case GUI_MSG_KEY_SUBTITLE: //字幕开关
			{
				if(dtv_is_scan_scene_exsit() == EPDK_FALSE && service_list->servNum > 0)
				{
                    if(dtv_ctr.h_subtitle)
					{
                        dtv_delete_scene(DTV_DESTORY_SUBTITLE_SCENE);
					}
					else
					{
						dtv_subtitle_create(msg);
					}
					
				}			
				return EPDK_OK;
			}
          
			default:
				return EPDK_FAIL;
		}
	}
	
    else if(KEY_REPEAT_ACTION == msg->dwAddData2)
    {

		switch(msg->dwAddData1)
		{
			case GUI_MSG_KEY_LONGVADD:
            case GUI_MSG_KEY_LONGVDEC:
			{
				if(dtv_is_scan_scene_exsit() == EPDK_FALSE)
				{
					if(GUI_LYRWIN_STA_ON != GUI_LyrWinGetSta(dtv_ctr.h_singal_layer))
                    	dtv_singal_win(DTV_SIGNAL_LAYER_ON);
				}
				return EPDK_OK;
			}

			case GUI_MSG_KEY_LONGESCAPE:
			{
				if(EPDK_TRUE == is_on_tv())
				{
					dtv_switch_tvout(msg);					
				}
			
				return EPDK_OK;
			}

		}
    }
    
	else if(KEY_UP_ACTION == msg->dwAddData2)
	{
		switch(msg->dwAddData1)
		{
            case GUI_MSG_KEY_CH_DOWN:
			case GUI_MSG_KEY_LONGCH_DOWN:
			{
		      	if(GUI_IsTimerInstalled(msg->h_deswin, DVB_SINGAL_TIMER_ID))
		      	{
					GUI_KillTimer(msg->h_deswin, DVB_SINGAL_TIMER_ID);
		      	}

				if(dtv_is_scan_scene_exsit() == EPDK_FALSE && service_list->servNum > 0)//不存在才响应上下换台键
				{
					__s32 index = 0;
					
					dtv_delete_scene(DTV_DESTORY_ALL_MENU_SCENE | DTV_DESTORY_DIALOG_WIN | DTV_CLOSE_SINGAL_LAYER | DTV_DESTORY_RECORD_LAYER |DTV_DESTORY_SUBTITLE_SCENE);

					esKRNL_TimeDly(2); 
					
                    stop_record();
                    
					index = maple_get_cur_program_index();
					
					index--;
					
					if(index < 0)
					{
						index = maple_get_cur_service_list_num() - 1;
                        if(index < 0)
                        {
                            index = 0;
                        }
					}
					
					maple_set_cur_channel(index);
					
					maple_stop_program(CLOSE_VIDEO_LAYER);
					
					maple_play_program();
					
					dtv_singal_win(DTV_SIGNAL_LAYER_ON);

    		      	if (!GUI_IsTimerInstalled(msg->h_deswin, DVB_SINGAL_TIMER_ID))
    		      	{
    		      		dtv_ctr.singal_timer = GUI_SetTimer(msg->h_deswin,DVB_SINGAL_TIMER_ID,100,NULL);
    		      	}
    		      	else
    		      	{
    		            GUI_ResetTimer(msg->h_deswin,DVB_SINGAL_TIMER_ID,100,NULL);
    		      	}
    		      	
					return EPDK_OK;
				}
				return EPDK_FAIL;
			}
			

            case GUI_MSG_KEY_CH_UP:
			case GUI_MSG_KEY_LONGCH_UP:	
			{
		      	if(GUI_IsTimerInstalled(msg->h_deswin, DVB_SINGAL_TIMER_ID))
		      	{
					GUI_KillTimer(msg->h_deswin, DVB_SINGAL_TIMER_ID);
		      	}


				if(dtv_is_scan_scene_exsit() == EPDK_FALSE && service_list->servNum > 0)//不存在才响应上下换台键
				{
					__s32 index = 0;

					dtv_delete_scene(DTV_DESTORY_ALL_MENU_SCENE | DTV_DESTORY_DIALOG_WIN | DTV_CLOSE_SINGAL_LAYER | DTV_DESTORY_RECORD_LAYER |DTV_DESTORY_SUBTITLE_SCENE);

					esKRNL_TimeDly(2); 
					
                    stop_record();

					index = maple_get_cur_program_index();
					
					index++;
					
					if(index > (maple_get_cur_service_list_num() - 1))
					{
						index = 0;
					}
					
					maple_set_cur_channel(index);
					
					maple_stop_program(CLOSE_VIDEO_LAYER);
					
                    maple_play_program();
                    
					dtv_singal_win(DTV_SIGNAL_LAYER_ON);
					

    		      	if (!GUI_IsTimerInstalled(msg->h_deswin, DVB_SINGAL_TIMER_ID))
    		      	{
    		      		dtv_ctr.singal_timer = GUI_SetTimer(msg->h_deswin,DVB_SINGAL_TIMER_ID,100,NULL);
    		      	}
    		      	else
    		      	{
    		            GUI_ResetTimer(msg->h_deswin,DVB_SINGAL_TIMER_ID,100,NULL);
    		      	}

					return EPDK_OK;
				}	
				return EPDK_FAIL;
			}
            
            
			case GUI_MSG_KEY_ESCAPE:
			{
                if(dtv_is_scan_scene_exsit() == EPDK_TRUE)
                {
                    if(0 != sl_task_id)
                    {
                		__u8 err;
                		
                    	__here__;
                        maple_break_search_task();
                        __here__;
                		maple_thread_delete(sl_task_id);
                		
                		sl_task_id = 0;
                		
                	}

                    maple_service_list_get(MAPLE_SERV_GROUP_TYPE_TV);
                    
                	if(service_list->servNum > 0)//搜台结束之后做一次保存
                	{
                        esMODS_MIoctrl(maple, MAPLE_CMD_SORT_SERV_LIST, 0, NULL);
                        
                        maple_service_list_get(MAPLE_SERV_GROUP_TYPE_TV);
                        
                        maple_cmmb_reg_flush();
                	}
            		
            		GUI_LyrWinDelete(dtv_ctr.h_menu_layer);
            		
            		dtv_ctr.h_scan = NULL;
            		
            		dtv_ctr.h_menu_layer = NULL;

                    maple_play_program();
                    
                	if(maple_get_cur_service_list_num() >= 1)
                	{
                        if(dsk_speaker_is_on() == EPDK_FALSE)
                        {
                			dsk_speaker_onoff(TV_SPEAKER_ON);
                		}
                	}
					
					dtv_ctr.h_singal = dtv_singal_create(msg->h_deswin,&dtv_ctr.h_singal_layer);										
                }
                                
				else if(EPDK_TRUE == dtv_exit_scene())
				{
					__s32 str[] = {0, STRING_DTV_WHETHER_EXIT_FROM_TV};

					dtv_delete_scene(DTV_DESTORY_DIALOG_WIN | DTV_CLOSE_SINGAL_LAYER | DTV_CLOSE_RECORD_LAYER);
					
					esKRNL_TimeDly(2); 
					
					default_dialog(dtv_ctr.h_dialog,msg->h_deswin, DTV_DIALOG_ESCAPE_ID, ADLG_YESNO, str);					
				}
				
			}
			break;
            
			case GUI_MSG_KEY_TVOUT:
			case GUI_MSG_KEY_LONGTVOUT:
            {
            	if(EPDK_TRUE == is_on_tv())
            	{
                    return EPDK_OK;
            	}
            	else
            	{
                    dtv_switch_tvout(msg);
            	}


                return EPDK_OK;
            }
			break;
			            
			case GUI_MSG_KEY_ENTER:
			{
				if(EPDK_FALSE == dtv_has_sub_scene())
				{
					dtv_singal_win(DTV_SIGNAL_LAYER_ON);
				}
				
				return EPDK_FAIL;
			}


			default:
			{
				return EPDK_FAIL;
			}
		}

	}
    return EPDK_OK;
}

static __s32 dtv_command_proc(__gui_msg_t *msg)
{
    if(LOWORD(msg->dwAddData1) == DTV_DIALOG_ESCAPE_ID)
	{
		switch(HIWORD(msg->dwAddData1))
		{
			case ADLG_CMD_EXIT:
            {
    			if(dtv_ctr.h_dialog != NULL)
    			{
    				app_dialog_destroy(dtv_ctr.h_dialog);
    				
    				dtv_ctr.h_dialog = NULL;
    				
    				if (msg->dwAddData2 == ADLG_IDYES)
     				{
    					__gui_msg_t dmsg;
    					
    					__here__;
    					dmsg.id = GUI_MSG_COMMAND;
    					
    					dmsg.h_deswin = GUI_WinGetParent(msg->h_deswin);
    					
    					dmsg.dwAddData1 = MAKELONG(GUI_WinGetItemId(msg->h_deswin), SWITCH_TO_MMENU) ;
    				
    					GUI_SendNotifyMessage(&dmsg);
     				}
                    
     				else
     				{
     				
     				}
     				
                    GUI_WinSetFocusChild(msg->h_deswin);

    			}
			}
            break;

			default:
				break;
		}

	}


    else if(LOWORD(msg->dwAddData1) == DTV_DIALOG_SWITCH_ANTENNA_ID)
	{
		switch(HIWORD(msg->dwAddData1))
		{
			case ADLG_CMD_EXIT:
            {
    			if(dtv_ctr.h_dialog != NULL)
    			{
    				app_dialog_destroy(dtv_ctr.h_dialog);
    				
    				dtv_ctr.h_dialog = NULL;
    				
    				if (msg->dwAddData2 == ADLG_IDYES)
     				{
						maple_set_antenna_state(EPDK_FALSE);
     				}
                    
     				else
     				{
 						maple_set_antenna_state(EPDK_TRUE);    				
     				}
     				
                    GUI_WinSetFocusChild(msg->h_deswin);
                    
                	dtv_main_create_proc(msg);	
    			}
			}
            break;

			default:
				break;
		}

	}

    else if(LOWORD(msg->dwAddData1) == DTV_DIALOG_RECORD_ID || LOWORD(msg->dwAddData1) == DTV_DIALOG_CHANNEL_ERROR_ID)
	{
		switch(HIWORD(msg->dwAddData1))
		{
			case ADLG_CMD_EXIT:
            {
    			if(dtv_ctr.h_dialog != NULL)
    			{
    				if (msg->dwAddData2 == ADLG_IDOK)
     				{
        				app_dialog_destroy(dtv_ctr.h_dialog);
        				
        				dtv_ctr.h_dialog = NULL;
        				
                        GUI_WinSetFocusChild(msg->h_deswin);

                        return EPDK_OK;
     				}                    
    			}
			}
            break;

			default:
				break;
		}

	}	


	/*common notify msg*/		
	switch(msg->dwAddData1)
	{
		//从菜单发来的命令不需要处理字幕信息
		case DTV_RESEARCH_SERVICE_LIST:
		{
			if(dtv_is_scan_scene_exsit() == EPDK_FALSE)
			{
			
	            stop_record();
				
				maple_epg_search_stop();
				
				maple_search_stop();
				
				dsk_speaker_onoff(TV_SPEAKER_OFF);
				
				maple_stop_program(CLOSE_VIDEO_LAYER);
										
				dtv_search_win_create(msg);
			}
			
			return EPDK_OK;
		}


		case DTV_PREVIEW_SERVICE_LIST:
		{
			if(dtv_is_scan_scene_exsit() == EPDK_FALSE && service_list->servNum > 0 && !dtv_ctr.h_serverlist)
			{
				dtv_program_win_create(msg);
			}

			return EPDK_OK;

		}

        case DTV_EPG:
        {
			if(dtv_is_scan_scene_exsit() == EPDK_FALSE && service_list->servNum > 0 && !dtv_ctr.h_epg)
			{
				if(g_weak_singal == EPDK_FALSE)
				{
			        maple_epg_getdata();
			    }
                else
                {
                    maple_epg_reset_schdl_list_day(0);
				}
				
				dtv_schedule_win_create(msg);
				
				return EPDK_OK;
			}
			else
			{
	           __msg("...............\n");
			}
			return EPDK_OK;
        }

        case DTV_AUDIO:
        {
			if(dtv_is_scan_scene_exsit() == EPDK_FALSE && service_list->servNum > 0 && !dtv_ctr.h_audio)
			{
				dtv_audio_win_create(msg);
			}
			return EPDK_OK;

        }
        
		//从各子界面退回到正常播放模式时，都是经过这里
		//包括各子界面定时消失的情况
		//所以在ISDBT中在此恢复字幕场景
		case DTV_BACK2PLAY_PROGRAM:
		{			
			dtv_delete_scene(DTV_DESTORY_ALL_MENU_SCENE);
			
			dtv_singal_win(DTV_SIGNAL_LAYER_ON);
			
			GUI_WinSetFocusChild(msg->h_deswin);

			return EPDK_OK;
		}
		
		case CMD_TO_UPDATE_SINGAL_STATUS:
		{
			if(DTV_SIGNAL_LAYER_OFF == msg->dwAddData2)
			{		
				dtv_singal_onff(dtv_ctr.h_singal,GUI_LYRWIN_STA_OFF);
			}
			else if(DTV_SIGNAL_LAYER_ON == msg->dwAddData2)
			{
				dtv_singal_onff(dtv_ctr.h_singal,GUI_LYRWIN_STA_ON);
			}
			
			return EPDK_OK;
		}
		break;

		
		case CMD_CHANGE_CHANNEL:
		{
	      	if (GUI_IsTimerInstalled(msg->h_deswin, DVB_SINGAL_TIMER_ID))
	      	{
				GUI_KillTimer(msg->h_deswin, DVB_SINGAL_TIMER_ID);
	      	}

			{
				dtv_delete_scene(DTV_DESTORY_RECORD_LAYER);
				
                stop_record();
                
				maple_set_cur_channel(msg->dwAddData2);
				
				maple_stop_program(CLOSE_VIDEO_LAYER);
				
                maple_play_program();
                
		      	if (!GUI_IsTimerInstalled(msg->h_deswin, DVB_SINGAL_TIMER_ID))
		      	{
		      		dtv_ctr.singal_timer = GUI_SetTimer(msg->h_deswin,DVB_SINGAL_TIMER_ID,100,NULL);
		      	}
		      	else
		      	{
		            GUI_ResetTimer(msg->h_deswin,DVB_SINGAL_TIMER_ID,100,NULL);
		      	}

				return EPDK_OK;
			}
			
			return EPDK_FAIL;

		}
		break;


		case CMD_NUMBER_SELECT_CHANNAL:
		{
			__bool lcn_state;
			
			__s32 ret;
			
			__s32 number = 0;
			
			__s32 channel_number = 0;

			
			if(dtv_is_scan_scene_exsit() == EPDK_TRUE)
			{
				return EPDK_OK;
			}
			
			dtv_delete_scene(DTV_DESTORY_ALL_MENU_SCENE | DTV_DESTORY_DIALOG_WIN | DTV_DESTORY_DIGIT_SCENE | DTV_CLOSE_SINGAL_LAYER | DTV_DESTORY_RECORD_SCENE | DTV_DESTORY_SUBTITLE_SCENE);


			number = msg->dwAddData2;
			
			lcn_state = maple_get_lcn_state();
			
			if(lcn_state == EPDK_TRUE)
			{
				ret = maple_get_cur_channel_index_by_lcn(number);
				
			 	if(ret == -1)
			 	{
					__s32 lang_id[]={0,STRING_DTV_SEL_ERR};
					default_dialog(dtv_ctr.h_dialog,msg->h_deswin,DTV_DIALOG_CHANNEL_ERROR_ID, ADLG_OK, lang_id);

	                esKRNL_TimeDly(150);

	                if (dtv_ctr.h_dialog)
	                {
	                	app_dialog_destroy(dtv_ctr.h_dialog);
	                	dtv_ctr.h_dialog = NULL;
	                }

			 	}
			 	else
			 	{
					channel_number = ret + 1; // 下面减了1
					
				 	stop_record();

				 	maple_set_cur_channel(channel_number - 1);
				 	dsk_speaker_onoff(TV_SPEAKER_OFF); 	 // mute on
				 	maple_stop_program(CLOSE_VIDEO_LAYER);
				 	maple_play_program();
				 	if(maple_get_cur_service_list_num() >= 1)
				 	{
					 	dsk_speaker_onoff(TV_SPEAKER_ON);
				 	}
				 	dtv_singal_win(DTV_SIGNAL_LAYER_ON);
				 
			 	}
			}
		
			else
			{
				channel_number = number;
				
				if(channel_number !=0 && channel_number <= maple_get_cur_service_list_num())
				{
					stop_record();
					 
					maple_set_cur_channel(channel_number - 1);
					
					dsk_speaker_onoff(TV_SPEAKER_OFF);
					
					maple_stop_program(CLOSE_VIDEO_LAYER);
					
					maple_play_program();
					
					if(maple_get_cur_service_list_num() >= 1)
					{
						dsk_speaker_onoff(TV_SPEAKER_ON);
					}
					dtv_singal_win(DTV_SIGNAL_LAYER_ON);
					 
				}
				else
				{
					__s32 lang_id[]={0,STRING_DTV_SEL_ERR};
					default_dialog(dtv_ctr.h_dialog,msg->h_deswin,DTV_DIALOG_CHANNEL_ERROR_ID, ADLG_OK, lang_id);

	                esKRNL_TimeDly(150);

	                if (dtv_ctr.h_dialog)
	                {
	                	app_dialog_destroy(dtv_ctr.h_dialog);
	                	dtv_ctr.h_dialog = NULL;
	                }

				}
			}
			return EPDK_OK; 

		}
		break;

		
		case CMD_TO_DELETE_SEARCH_THREAD:
		{            
			if(dtv_ctr.h_scan)
			{ 
				GUI_LyrWinSetBottom(dtv_ctr.h_menu_layer);
				GUI_LyrWinDelete(dtv_ctr.h_menu_layer);
				dtv_ctr.h_scan = NULL;
				dtv_ctr.h_menu_layer = NULL;
			}
			
			maple_search_stop();
    
			maple_play_program();
			
			if(maple_get_cur_service_list_num() >= 1)
			{
				dsk_speaker_onoff(TV_SPEAKER_ON);
			}

			dtv_ctr.h_singal = dtv_singal_create(msg->h_deswin,&dtv_ctr.h_singal_layer);
			
			dtv_singal_win(DTV_SIGNAL_LAYER_ON);
			
			return EPDK_OK;
		}
		break;


        case CMD_FS_PART_PLUGOUT:
        {
            stop_record_proc(msg);
        }
		break;

		case CMD_SWITCH_EXTERNAL:
        {
			__s32 lang_id[]={0,STRING_DTV_SWITCH_TO_EXTERNAL_ANTENNA};

			maple_set_antenna_state(EPDK_TRUE);
			
			default_dialog(dtv_ctr.h_dialog,msg->h_deswin,DTV_DIALOG_SWITCH_EXTERNAL_ID, ADLG_OK, lang_id);

            esKRNL_TimeDly(100);

            if (dtv_ctr.h_dialog)
            {
            	app_dialog_destroy(dtv_ctr.h_dialog);
            	dtv_ctr.h_dialog = NULL;
            }
        }	
		break; 

	}
	
	return EPDK_OK;
}




static __s32 dtv_timer_proc(__gui_msg_t *msg)
{
    __bool lcn_state;
    __s32 ret;

	if(1 == g_is_searching || dtv_is_scan_scene_exsit() == EPDK_TRUE)
	{
	    return EPDK_OK;
	}

   	switch(msg->dwAddData1)
	{

	   case DVB_SINGAL_TIMER_ID://信号状态
	   {
	   		__s32 ret;
	        
			ret = dtv_singal_strength_detect(msg->h_deswin);
			
		    return EPDK_OK;
	   }


	    case DVB_FLUSH_ID:
		{
		    static  __s32 count = 0;
		    
			count++;
			
			if(count%10 == 0)
			{
				GUI_KillTimer(msg->h_deswin,DVB_FLUSH_ID);
				
		        if(save_flag == EPDK_TRUE)
		        {
		            save_flag = EPDK_FALSE;
		            maple_cmmb_reg_flush();
		        }
		        count = 0;

			}           
		    return EPDK_OK;
		}

	   default:
		   break;
	}

   return EPDK_FAIL;

}

static __s32 dtv_main_destroy_proc(__gui_msg_t *msg)
{
	
    if (GUI_IsTimerInstalled(msg->h_deswin, DVB_SINGAL_TIMER_ID))
    {
		GUI_KillTimer(msg->h_deswin, DVB_SINGAL_TIMER_ID);
	}
    
	if (GUI_IsTimerInstalled(msg->h_deswin, DVB_FLUSH_ID))
	{
		GUI_KillTimer(msg->h_deswin, DVB_FLUSH_ID);
	}


    show_dtv_destory_win();

    if(0 != sl_task_id)
    {
		__u8 err;
		
    	__here__;
        maple_break_search_task();
        __here__;
		maple_thread_delete(sl_task_id);
		
		sl_task_id = 0;
	}
	else
	{
		__here__;
		maple_stop_program(CLOSE_VIDEO_LAYER);
	}
	__here__;

	dsk_speaker_onoff(TV_SPEAKER_OFF);
	__here__;
	__here__;
	
	dtv_delete_scene(DTV_DESTORY_ALL_SCENE);


	maple_cmmb_reg_flush();
	
	__here__;
	maple_close();
	
	dtv_uninit();
	
	audio_mode_switch(AUDIO_NOR_MODE);
	
    esKRNL_TimeDly(40);//延时一段时间，怀疑模块掉电，会影响到nor的读。

    
	if(dtv_ctr.h_destroy_layer)
    {
		GUI_LyrWinDelete(dtv_ctr.h_destroy_layer);
		dtv_ctr.h_destroy_layer = NULL;
	}


	g_dtv_mainwin = NULL;

	esPWM_UnlockCpuFreq();
	
    set_input_gpio(1, 0, 0);//spi1 cs
    set_input_gpio(1, 1, 0);//spi1 in
    set_input_gpio(1, 2, 0);//ip1 clk
    set_input_gpio(1, 3, 0);//spi1 out
    //set_input_gpio(3, 11, 0);//reset pin
    set_input_gpio(1, 4, 0);//int pin


	dsk_speaker_onoff(TV_SPEAKER_ON);

	return EPDK_OK;
}


static __s32 dtv_main_proc(__gui_msg_t  *msg)
{	
			
	switch(msg->id)
    {
		case GUI_MSG_CREATE:
		{			
			pull_down_gpio(AB_D_PORT, AB_D_PORTNUM);//拉低LCD15，选择AB类功放

            if(is_on_tv() == EPDK_TRUE)
		    {
                H_WIN h_dialog = NULL;
                
                __s32 lang_id[]={STRING_MOVIE_TIPS, STRING_DTV_NO_TVOUT};
                
                g_display_switch_output(LION_DISP_LCD);//将LCD 切换回LCD显示
                
                default_dialog(h_dialog, msg->h_deswin, DTV_TVOUT_ID, ADLG_OK, lang_id);

                esKRNL_TimeDly(100);

                if (h_dialog)
                {
                	app_dialog_destroy(h_dialog);
                	h_dialog = NULL;
                }
                
            }

			{
				__s32 str[] = {0, STRING_DTV_WHETHER_SWITCH_TO_ROD_ANTENNA};
				
				default_dialog(dtv_ctr.h_dialog,msg->h_deswin, DTV_DIALOG_SWITCH_ANTENNA_ID, ADLG_YESNO, str);					
			}
			
            return EPDK_OK;


		}	

		case GUI_MSG_CLOSE:			
		{			
			__gui_msg_t dmsg;
		        
			dmsg.id         = GUI_MSG_COMMAND;
			dmsg.h_deswin   = GUI_WinGetParent(msg->h_deswin);
			dmsg.dwAddData1 = MAKELONG(GUI_WinGetItemId(msg->h_deswin), SWITCH_TO_MMENU) ;
		        
			GUI_SendNotifyMessage(&dmsg);
		
			return EPDK_OK;
		
		}
		
		case GUI_MSG_DESTROY:
		{
#if AB_D_UPEN
    		pull_down_gpio(AB_D_PORT, AB_D_PORTNUM);
#else
    		pull_up_gpio(AB_D_PORT, AB_D_PORTNUM);
#endif
			
            stop_record();
            
            if(EPDK_TRUE == is_on_tv())
        	{
                dtv_switch_tvout(msg);                  
        	}

			return dtv_main_destroy_proc(msg);
		}

        
		case GUI_MSG_KEY:
		{
            if(msg->dwAddData1 == GUI_MSG_KEY_TV_LEFT|| msg->dwAddData1 == GUI_MSG_KEY_TV_RIGHT)
            {
				if(NULL != dtv_ctr.h_menu_layer || NULL != dtv_ctr.h_dialog)
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
                else
				{
                    switch(msg->dwAddData1)
                    {
                        case GUI_MSG_KEY_TV_LEFT:
                            msg->dwAddData1 = GUI_MSG_KEY_CH_DOWN;
                            break;
                        case GUI_MSG_KEY_TV_RIGHT:
                            msg->dwAddData1 = GUI_MSG_KEY_CH_UP;
                            break;
                        default:
                            break;
                    }
				}
                   

            }

            if(msg->dwAddData1 == GUI_MSG_KEY_CH_DOWN || msg->dwAddData1 == GUI_MSG_KEY_CH_UP
                || msg->dwAddData1 == GUI_MSG_KEY_DOWN || msg->dwAddData1 == GUI_MSG_KEY_UP)
            {
    			if(KEY_UP_ACTION == msg->dwAddData2)
    			{
            		if (!GUI_IsTimerInstalled(msg->h_deswin, DVB_FLUSH_ID))
            		{
            			GUI_SetTimer(msg->h_deswin,DVB_FLUSH_ID,DTV_FLUSH_DELAY,NULL);
            		}
            		else
            		{
            			GUI_ResetTimer(msg->h_deswin,DVB_FLUSH_ID,DTV_FLUSH_DELAY,NULL);
                    }
                }

            }

			dtv_key_proc(msg);

		}
		break;

		
		case GUI_MSG_COMMAND:
		{
			dtv_command_proc(msg);
            
            return EPDK_OK;
		}

        case GUI_MSG_TIMER:
        {
            __s32 ret;            
            
           ret = dtv_timer_proc(msg);
        }
		break;
        
		default:
	     	break;
	}
    return GUI_ManWinDefaultProc(msg);		
   
}

/*
**********************************************************************************************************************
*                                               dtv_mainwin_create
*
* Description:
*
* Arguments  :
*
* Returns    :
*
* Notes      :
*
**********************************************************************************************************************
*/
static H_WIN dtv_mainwin_create(root_para_t  *para)
{
	__gui_manwincreate_para_t create_info;
	
	eLIBs_memset(&create_info, 0, sizeof(__gui_manwincreate_para_t));

    
	create_info.name     = APP_DTV;
	create_info.id       = APP_DTV_ID;
	create_info.hParent  = para->h_parent;
	create_info.ManWindowProc = (__pGUI_WIN_CB)esKRNL_GetCallBack((__pCBK_t)dtv_main_proc);
	create_info.attr     = NULL;
	create_info.hHosting = NULL;

	return(GUI_ManWinCreate(&create_info));
}




__s32 app_dtv_create(root_para_t  *para)
{	
	
	H_WIN dtv_mainwin;
	
	__s32 ret;
	

	gscene_hbar_set_state(HBAR_ST_HIDE);
	
	gscene_bgd_set_state(BGD_STATUS_HIDE);
	
		
    eLIBs_memset(&dtv_ctr,0,sizeof(dtv_ctr_t));
    
	dtv_mainwin = dtv_mainwin_create(para);
    
	if (dtv_mainwin == NULL)
	{
		__wrn("cann't creat dtv mainwin\n");
       		return NULL;
	}

	GUI_WinSetFocusChild(dtv_mainwin);

	g_dtv_mainwin = dtv_mainwin;

	return dtv_mainwin;
}

