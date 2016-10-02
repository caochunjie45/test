/*
**************************************************************************************************************
*											         ePDK
*						            the Easy Portable/Player Develop Kits
*									           desktop system 
*
*						        	 (c) Copyright 2007-2011, CHIPHD, China
*											 All Rights Reserved
*
* File    	: app_fm.c
* By      	: CQQ
* Func		: 
* Version	: v1.0
* ============================================================================================================
* 2011/07/24 22:51  create this file, implements the fundamental interface;
**************************************************************************************************************
*/
#include "app_fm_i.h"
#include "fm_sset.h"
#include "fm_ScaleBar.h"
#include "fm_main.h" 
#if 0
    #define __here__            eLIBs_printf("@L%d(%s)\n", __LINE__, __FILE__);
	#define __msg(...)    		(eLIBs_printf("MSG:L%d(%s):", __LINE__, __FILE__),                 \
							     eLIBs_printf(__VA_ARGS__)									        )
	#define __wrn(...)    		(eLIBs_printf("MSG:L%d(%s):", __LINE__, __FILE__),                 \
							     eLIBs_printf(__VA_ARGS__)									        )
	#define __err(...)    		(eLIBs_printf("MSG:L%d(%s):", __LINE__, __FILE__),                 \
							     eLIBs_printf(__VA_ARGS__)									        )
#else
    #define __msg(...)
#endif
//#undef FOR_800X480_UI
#define DIGIT_TIME_CNT		300////300
/************************************************************************/
// 
/************************************************************************/
H_WIN g_FMManWnd = NULL;
//////////////////////////////////////////////////////////////////////////
// 建立图层
#define fm_layer_create(_lyr, _prt, _name)   app_com_layer_create_default(_lyr, _prt, _name)

//////////////////////////////////////////////////////////////////////////
static __s32 _fm_op_notify(__s32 op_function);
#define fm_op_notify(_fun)    _fm_op_notify((__s32)_fun)

static __s32 g_fm_exit = 0;////1-exit;0-unexit
static __s32 g_is_searching = 0;

static __s32 app_root_show_dlg(H_WIN hwin, __s32 time)
{
	H_WIN dlg;
	__s32 lang_id[]={STRING_SET_CUE,STRING_SET_CUE};	
	__s32 null_disk;
	default_dialog(dlg, hwin, APP_COM_DLG_ID, ADLG_OK, lang_id);
    GUI_LyrWinCacheOff(); 
    if(0 != time)
    {
	    esKRNL_TimeDly(time);
    }
    else
    {
        esKRNL_TimeDly(150);
    }   
	app_dialog_destroy(dlg);
}

#if BEETLES_KEY_REDEF
static __s32 draw_digits_infostr(H_LYR hdigit_layer)
{
	RECT rect;
	char text[64] = {0};
	GUI_RECT gui_rect;

	GUI_LyrWinGetScnWindow(hdigit_layer, &rect);
	get_menu_text(STRING_FM_DIGIT_SELECT,text, sizeof(text));
	
	gui_rect.x0 = 0;
	gui_rect.y0 = 0;
	gui_rect.x1 = gui_rect.x0+rect.width-1;
	gui_rect.y1 = 20;

	GUI_LyrWinSel(hdigit_layer);
	GUI_SetFont(SWFFont);
	GUI_UC_SetEncodeUTF8();

    	GUI_SetColor(GUI_RED);
	GUI_DrawHLine(21,gui_rect.x0,gui_rect.x1);
	GUI_SetColor(GUI_WHITE);
	GUI_DispStringInRect(text, &gui_rect, GUI_TA_HCENTER|GUI_TA_TOP);

	GUI_LyrWinSetTop(hdigit_layer);

	return EPDK_OK;
}
#endif

static __s32 music_n2digits(__s32 num, __s32 * digits)
{
	__s32 i=0;

	do
	{
		digits[i]=num%10;
		num=num/10;
		i++;
	}
	while(num!=0);
	
	return i;
}

static __s32 fmplay_saveCH( FM_WND_T *wnd_para )//sun++ for save chFM_WND_T *wnd_para;
{
	reg_fm_para_t* para;
      __u32 i;
      i=wnd_para->curFM1_3AM1_2_id;
	para = (reg_fm_para_t*)dsk_reg_get_para_by_app(REG_APP_FM);

	para->curFM1_3AM1_2_id= wnd_para->curFM1_3AM1_2_id;
	para->curFM1_3AM1_2_index= wnd_para->curFM1_3AM1_2_index;
	__msg(".......save......para->curFM1_3AM1_2_id=%d..para->curFM1_3AM1_2_index=%d....\n",para->curFM1_3AM1_2_id,para->curFM1_3AM1_2_index);
	//para->reserverd[2]=fmplay_menu_attr->fmchannelfreq;
	para->FM1_3_BAND_Index[i]=wnd_para->curFM1_3AM1_2_index;
      
	return EPDK_OK;
}
static __s32 fmplay_loadCH( FM_WND_T *wnd_para)
{
	reg_fm_para_t* para;
	__u32 i;
	para = (reg_fm_para_t*)dsk_reg_get_para_by_app(REG_APP_FM);
__msg(".......load.......para->curFM1_3AM1_2_id=%d..para->curFM1_3AM1_2_index=%d....\n",para->curFM1_3AM1_2_id,para->curFM1_3AM1_2_index);
	wnd_para->curFM1_3AM1_2_id = para->curFM1_3AM1_2_id;
	wnd_para->curFM1_3AM1_2_index = para->curFM1_3AM1_2_index;
	//fmplay_menu_attr->fmchannelfreq=para->reserverd[2];
	for(i=0;i<3;i++)
      wnd_para->fm1_3band[i]=para->FM1_3_BAND_Index[i];
	return EPDK_OK;
}

//////////////////////////////////////////////////////////////////////////
//当前找到一个有效频率
static void on_fm_test_freq_ok(FM_CTRLDATA_T* fm_ctrl)
{
	if(fm_ctrl->search_mode == SRHMODE_MANUAL)//手动搜索
	{
		dsk_radio_rcv_get_cur_freq(&fm_ctrl->cur_freq);//by cky 
		__wrn("draw:fm_ctrl->cur_freq = %d\n",fm_ctrl->cur_freq);
	}
	else//SRHMODE_AUTO  自动搜索
	{
		if(dsk_radio_rcv_get_search_result_vn() != 0)
		{
			dsk_radio_chan_t chan_info;

			fm_ctrl->channel_count = dsk_radio_rcv_get_search_result_vn();
			fm_ctrl->channel_id = fm_ctrl->channel_count - 1;

			__wrn("@fm_ctrl->seach_channel_count = %d\n",fm_ctrl->channel_count);
			__wrn("@fm_ctrl->seach_channel_id = %d\n",fm_ctrl->channel_id);

			dsk_radio_rcv_get_search_result_chaninfo(fm_ctrl->channel_id, &chan_info);
			__wrn("@fm_ctrl->channel_id = %d\n",fm_ctrl->channel_id);
			fm_ctrl->cur_freq = chan_info.freq;
			__wrn("@:fm_ctrl->cur_freq = %d\n",fm_ctrl->cur_freq);
		}
	}
}

//当前找到的是无效频率
static void on_fm_test_freq_fail(FM_CTRLDATA_T* fm_ctrl)
{
	if(fm_ctrl->search_mode == SRHMODE_MANUAL)//手动搜索
	{
		dsk_radio_rcv_get_cur_freq(&fm_ctrl->cur_freq);
		__wrn("draw:fm_ctrl->cur_freq = %d\n",fm_ctrl->cur_freq);
	}
	else//SRHMODE_AUTO  自动搜索
	{
		dsk_radio_rcv_get_cur_freq(&fm_ctrl->cur_freq);
		__wrn("fm_ctrl->cur_freq = %d\n",fm_ctrl->cur_freq);
	}
}


//搜索结束
static void on_fm_test_freq_end(FM_CTRLDATA_T* fm_ctrl)
{
	dsk_radio_chan_t chan_info;
	if(fm_ctrl->search_mode == SRHMODE_MANUAL)//手动搜索
	{
		dsk_radio_rcv_get_cur_freq(&fm_ctrl->cur_freq); //by cky modify
		//dsk_radio_rcv_get_search_result_chaninfo(fm_ctrl->channel_id, &chan_info);
		//fm_ctrl->cur_freq = chan_info.freq;
		__wrn("fm_ctrl->channel_id = %d,fm_ctrl->cur_freq = %d\n",fm_ctrl->channel_id,fm_ctrl->cur_freq);
		
	}
	else//SRHMODE_AUTO  自动搜索
	{
		fm_ctrl->channel_id = 0;
		if(dsk_radio_rcv_get_search_result_vn() == 0)//no channel info
		{
			fm_ctrl->cur_freq = fm_ctrl->cur_min_freq;
			fm_ctrl->channel_count = 1;

			__wrn("find 0 channel, select min-freq %d as channel 1\n", fm_ctrl->cur_freq);
		}
		else
		{
			dsk_radio_rcv_get_search_result_chaninfo(fm_ctrl->channel_id, &chan_info);
			fm_ctrl->cur_freq = chan_info.freq;
			fm_ctrl->channel_count = dsk_radio_rcv_get_search_result_vn();
			__wrn("find %d channel(s), select %d as channel 1\n", fm_ctrl->channel_count, fm_ctrl->cur_freq);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//更新列表
	{
		reg_fm_para_t* para;
		get_reg_pointer(para, reg_fm_para_t, REG_APP_FM);

		dsk_radio_rcv_save_search_result(); //save to reg
		fm_reg_set_channel_count(para, fm_ctrl->channel_count);
	}
	//////////////////////////////////////////////////////////////////////////

	fm_ctrl->search_mode = SRHMODE_IDLE;
}

//////////////////////////////////////////////////////////////////////////
#define FM_CB_NEED_NOTIFY_FRW(_p) (_p && _p->h_main_lyr && GUI_LyrWinGetSta(_p->h_main_lyr) == GUI_LYRWIN_STA_ON)
__s32 cb_srhch_success(void *arg_p)
{
	FM_WND_T *wnd_para;
	H_WIN hwnd = (H_WIN)arg_p;
	FM_GetWndPara(wnd_para, FM_WND_T, hwnd);

    __wrn("*******cb_srhch_success********\n");
	if (wnd_para)
	{
		on_fm_test_freq_ok(&wnd_para->fm_ctrl);
		__wrn("draw:fm_ctrl->cur_freq = %d\n",wnd_para->fm_ctrl.cur_freq);
	}

	if (FM_CB_NEED_NOTIFY_FRW(wnd_para))
	{
		NOTIFY_MSG(CMD_AUTOSRH_FINDCH, hwnd, wnd_para->h_main_wnd, 0, 0, 0);
	}

	return 0;
}

__s32 cb_srhch_fail(void *arg_p)
{
	FM_WND_T *wnd_para;
	H_WIN hwnd = (H_WIN)arg_p;
	FM_GetWndPara(wnd_para, FM_WND_T, hwnd);

    __wrn("*******cb_srhch_fail********\n");
	if (wnd_para)
	{
		on_fm_test_freq_fail(&wnd_para->fm_ctrl);
		__wrn("draw:fm_ctrl->cur_freq = %d\n",wnd_para->fm_ctrl.cur_freq);
	}

	if (FM_CB_NEED_NOTIFY_FRW(wnd_para))
	{
		NOTIFY_MSG(CMD_AUTOSRH_FINDCHFAIL, hwnd, wnd_para->h_main_wnd, 0, 0, 0);
	}

	return 0;
}

__s32 cb_srhch_over(void *arg_p)
{
	FM_WND_T *wnd_para;
	H_WIN hwnd = (H_WIN)arg_p;
    __s32 auto_close_scn_time;
    
	FM_GetWndPara(wnd_para, FM_WND_T, hwnd);

    __wrn("*******cb_srhch_over********\n");

    g_is_searching = 0;

    auto_close_scn_time = get_auto_close_scn_time_fm();
    __wrn("auto_close_scn_time=%d\n", auto_close_scn_time);
    if(auto_close_scn_time > 0)//搜索完毕重新设置FM的自动关屏时间
    {
        g_set_close_scn_time(auto_close_scn_time);
    }
    
	if (wnd_para)
	{
		on_fm_test_freq_end(&wnd_para->fm_ctrl);
	}

	if (FM_CB_NEED_NOTIFY_FRW(wnd_para))
	{
		NOTIFY_MSG(CMD_AUTOSRH_OVER, hwnd, wnd_para->h_main_wnd, 0, 0, 0);
	}

	return 0;
}

/************************************************************************/
//通知关闭fm
/************************************************************************/
__s32 notify_to_close_fm(H_WIN hwnd, __s32 id, __s32 para)
{
	__gui_msg_t msg;

	msg.h_deswin   = GUI_WinGetParent(FM_GetManWnd());
	msg.h_srcwin   = hwnd;
	msg.id         = GUI_MSG_COMMAND;
	msg.dwAddData1 = MAKELONG(GUI_WinGetItemId(hwnd), id);
	msg.dwAddData2 = para;
	msg.dwReserved = 0;

	GUI_SendNotifyMessage(&msg);

	return EPDK_OK;
}

#ifdef SUPPORT_DIGIT_KEY
static __s32 fm_set_freq_play(FM_WND_T *wnd_para) //cai++ for 数字点歌
{
	__s32 cur_freq;
	__s32 cur_min_freq;
	__s32 cur_max_freq;
	__s32 digit_freq = 0;

/*
	if(wnd_para->ndigit_fm_freq/100 <= 0)
	{
		digit_freq = wnd_para->ndigit_fm_freq*1000;
	}
	else if(wnd_para->ndigit_fm_freq/100 > 0 && wnd_para->ndigit_fm_freq/1000<=0)
	{
		digit_freq = wnd_para->ndigit_fm_freq*100;
	}
	else if(wnd_para->ndigit_fm_freq/1000>0 && wnd_para->ndigit_fm_freq/10000<=0)
	{
		digit_freq = wnd_para*100;
	}
	else if()
*/
	//__msg("wnd_para->ndigit_fm_freq = %d, channel_count = %d \n", wnd_para->ndigit_fm_freq, wnd_para->fm_ctrl.channel_count);

	if (wnd_para->ndigit_fm_freq<50)
	{
		if(wnd_para->ndigit_fm_freq > (__s32)(wnd_para->fm_ctrl.channel_count))
		{
			return EPDK_FAIL; //channel_id = 0;
		}	
		__msg("fm_change_channelno\n");
		fm_change_channelno(&wnd_para->fm_ctrl, wnd_para->ndigit_fm_freq);
	  
	    return EPDK_OK;
	}
	dsk_radio_rcv_get_cur_freq(&cur_freq);
	__msg("cur_freq = %d\n", cur_freq);
	//在此先只做中国FM频段判断，其他外国频段自行修改
	if(wnd_para->ndigit_fm_freq/10>0 && wnd_para->ndigit_fm_freq/100 <=0)
	{
__here__;
		digit_freq = wnd_para->ndigit_fm_freq*1000;
	}
	else if(wnd_para->ndigit_fm_freq/100>0 && wnd_para->ndigit_fm_freq/1000<=0)
	{
__here__;
		if(wnd_para->ndigit_fm_freq>=875 && wnd_para->ndigit_fm_freq<=999)
		{
__here__;		
			digit_freq = wnd_para->ndigit_fm_freq*100;
		}
		else if(wnd_para->ndigit_fm_freq>=100 && wnd_para->ndigit_fm_freq<108)
		{
__here__;
			digit_freq = wnd_para->ndigit_fm_freq*1000;
		}
	}
	else if(wnd_para->ndigit_fm_freq/1000>0 && wnd_para->ndigit_fm_freq/10000<=0)
	{
__here__;
		digit_freq = wnd_para->ndigit_fm_freq*100;
	}
	else
	{
__here__;
		digit_freq = wnd_para->ndigit_fm_freq*1000;
	}
	__msg("digit_freq = %d\n", digit_freq);
	if(digit_freq< 87500 || digit_freq > 108000)  //超过预定值  不做处理
		return EPDK_FAIL;


	if(digit_freq == cur_freq)//如果等于当前正在播放的频率  不做处理
		return EPDK_OK;
__here__;	
	dsk_radio_rcv_set_freq_play(digit_freq);
__here__;
	//_UpdateFMNow(wnd_para->h_main_wnd);
	wnd_para->fm_ctrl.cur_freq = digit_freq;
	 FM_UpdateUI(wnd_para->h_main_wnd);
__here__;	
	//dsk_radio_get_min_freq(&cur_min_freq);
	//dsk_radio_get_max_freq(&cur_max_freq);

	return EPDK_OK;
}

#endif

/************************************************************************/
// 选择某个频道
//因为增加了退出一项，所以sel_id要减1
/************************************************************************/
static __s32 fm_change_channelno(FM_CTRLDATA_T *fm_ctrl, __s32 sel_id)
{
	if(dsk_radio_rcv_get_search_result_vn() != fm_ctrl->channel_count)
	{
		__wrn(" somewhere error,   check\n");
	}

	__msg("  channel_count = %d,   channel_id = %d, sel %d\n", 
		fm_ctrl->channel_count, fm_ctrl->channel_id, sel_id);
/*
	if(fm_ctrl->channel_count <= 1)
	{
		__wrn("  fm_ctrl->channel_count <= 1\n");
		return EPDK_OK;		
	}
*/
	if (fm_ctrl->channel_id == sel_id -1
		||sel_id > fm_ctrl->channel_count)
	{
		return EPDK_OK;
	}
	
	if (0 == sel_id)
	{
		app_fm_notify_delete_sub_scene(g_FMManWnd);
		return EPDK_OK;
	}	
	else
	{
		dsk_radio_chan_t chan_info;
		reg_fm_para_t* para;

		get_reg_pointer(para, reg_fm_para_t, REG_APP_FM);

		//dsk_radio_rcv_get_search_result_chaninfo(sel_id, &chan_info);
		dsk_radio_rcv_get_search_result_chaninfo(sel_id -1, &chan_info);
		__wrn(" sel freq = %d\n", chan_info.freq);
		fm_ctrl->cur_freq = chan_info.freq;
		if (dsk_radio_rcv_set_freq_play(chan_info.freq) == EPDK_OK)
		{
			fm_ctrl->channel_id = sel_id -1;
			fm_reg_set_sel_channel_id(para, sel_id -1);
		}

		dsk_radio_rcv_get_cur_freq(&fm_ctrl->cur_freq);//just for mistake
		__wrn(" get sel freq = %d\n", fm_ctrl->cur_freq);

		//refresh ui:
		fm_op_notify(fm_change_channelno);
		__wrn("  fm_change_channelno  return\n");
	}

	return EPDK_OK;
}

//手动搜索
static __s32 fm_manual_search_start(FM_CTRLDATA_T *fm_ctrl, __s32 way)
{
	__u32 search_flag;
	dsk_radio_get_search_flag (&search_flag) ;
	if (1==search_flag)//正在搜索过程中不允许中断，必须得搜索完后才能进行
	{
		__wrn("fm_manual_search_start:return\n");
		return EPDK_OK;
	}
	else
	{		  
		__wrn("fm_manual_search_start:\n");
		fm_ctrl->search_mode = SRHMODE_MANUAL;
		__wrn("FM::way = %d\n",way);//0:right;1:left
		dsk_radio_set_manual_way(way);
		dsk_radio_set_automanual_mode(DSK_RADIO_SEARCH_MANUAL);
		dsk_radio_rcv_set_play_list_type(PLAY_LIST_SEARCH_RESULT);
		__wrn(" fm_manual:fm_ctrl->cur_freq = %d, fm_ctrl->channel_id + 1=%d\n",fm_ctrl->cur_freq,fm_ctrl->channel_id + 1);		
		dsk_radio_rcv_search_all(fm_ctrl->cur_freq, fm_ctrl->channel_id + 1); //TODO:fm_ctrl->channel_id+1 ?
		return EPDK_OK;
	 }
}

//自动搜索
static __s32 fm_auto_search_start(FM_CTRLDATA_T *fm_ctrl)
{
	__u32 search_flag;

    g_is_searching = 1;
    
	dsk_radio_get_search_flag (&search_flag) ;
	if (1==search_flag)//正在搜索过程中不允许中断，必须得搜索完后才能进行
	{
		__wrn("fm_auto_search_start:return\n");
		return EPDK_OK;
	}
	else
	{
		set_record_is_play(EPDK_FALSE);
		//g_close_screen();
		g_set_close_scn_time(1);
		__wrn("fm_auto_search_start:\n");
		fm_ctrl->search_mode = SRHMODE_AUTO;
		dsk_radio_set_automanual_mode(DSK_RADIO_SEARCH_AUTO);
		dsk_radio_rcv_set_play_list_type(PLAY_LIST_SEARCH_RESULT);						
		__wrn("fm_auto_search_start:");
		dsk_radio_rcv_search_all(0, 0);
		return EPDK_OK;
	}
}

//初始化相关数据
static __s32 fm_init_data(FM_CTRLDATA_T *fm_ctrl)
{
	fm_ctrl->search_mode = SRHMODE_IDLE;
	fm_ctrl->thumb_show = FM_SHOW_THUMB_N;

	__wrn("init module#################################\n");
	if(dsk_radio_is_open())
	{
		dsk_radio_chan_t chan_info;

		reg_fm_para_t* para;
		get_reg_pointer(para, reg_fm_para_t, REG_APP_FM);

		fm_ctrl->channel_count = dsk_radio_rcv_get_search_result_vn();

		__wrn("fm_ctrl->channel_count = %d\n", fm_ctrl->channel_count);
		if(fm_ctrl->channel_count <= 0)//no channel info
		{
			__here__;
			fm_ctrl->channel_id = 0;
			fm_ctrl->cur_freq = MIN_CHANNEL_FREQ;
			fm_ctrl->channel_count = 0;
			//初始化reg_fm_para_t
			ZeroMemory(para, sizeof(reg_fm_para_t));
			fm_reg_set_sel_channel_id(para, 0);
// 			fm_reg_set_channel_(para, 0, fm_ctrl->cur_freq); //设置默认一个频道频率
// 			fm_reg_set_channel_count(para, fm_ctrl->channel_count);
			//dsk_radio_rcv_set_freq_play(MIN_CHANNEL_FREQ);
			//dsk_radio_rcv_set_freq_play(fm_ctrl->cur_freq);	/////0414
			dsk_radio_rcv_get_search_result(para);
		
		}
		else
		{
			__here__;
			if (ValueBetweenRange(MIN_CHANNEL_FREQ, fm_reg_get_curfreq(para), MAX_CHANNEL_FREQ)
				&& fm_reg_get_channel_count(para) == 1 && fm_ctrl->channel_count == 1
				&& fm_reg_get_channel_(para, 0) == MIN_CHANNEL_FREQ)
			{//曾经有手动设置频率(没搜索过)
				fm_ctrl->cur_freq = fm_reg_get_curfreq(para);
				__wrn("fm_init:fm_ctrl->cur_freq = %d\n",fm_ctrl->cur_freq);
				__wrn("fm_init:fm_ctrl->channel_count  = %d\n",fm_ctrl->channel_count );
				if ((fm_ctrl->cur_freq <MIN_CHANNEL_FREQ)&&(fm_ctrl->cur_freq>MIN_CHANNEL_FREQ))
				{
					return EPDK_FAIL;
				}
				else
				{	
					if (87500 == MIN_CHANNEL_FREQ)//只针对4702，FM play3次声音才最清晰
						{
							dsk_radio_rcv_set_freq_play(fm_ctrl->cur_freq +100);					
							dsk_radio_rcv_set_freq_play(fm_ctrl->cur_freq -100);		
						}
					dsk_radio_rcv_set_freq_play(fm_ctrl->cur_freq);					
				}
				
			} 
//			else
			{
				if (ValueInRange(0, para->channel_id, fm_ctrl->channel_count))
				{
					fm_ctrl->channel_id = fm_reg_get_sel_channel_id(para);
					__wrn("fm_ctrl->channel_id  = %d\n",fm_ctrl->channel_id );
				} 
				else
				{
					fm_ctrl->channel_id = 0;
					fm_reg_set_sel_channel_id(para, 0);
				}
				dsk_radio_rcv_get_search_result_chaninfo(fm_ctrl->channel_id, &chan_info);
				__wrn(" fm_ctrl->channel_id  = %d\n",fm_ctrl->channel_id );
				if (ValueOutOfRange(MIN_CHANNEL_FREQ, fm_ctrl->cur_freq, MAX_CHANNEL_FREQ))
				{
					fm_ctrl->cur_freq = chan_info.freq;
				}
				__wrn(" fm_ctrl->cur_freq = %d\n",fm_ctrl->cur_freq);
				if ((fm_ctrl->cur_freq <MIN_CHANNEL_FREQ)&&(fm_ctrl->cur_freq>MIN_CHANNEL_FREQ))
				{
					return EPDK_FAIL;
				}//FM play3次声音才最清晰
				else
				{						
					if (87500 == MIN_CHANNEL_FREQ)//只针对4702，FM play3次声音才最清晰
						{
							dsk_radio_rcv_set_freq_play(fm_ctrl->cur_freq +100);					
							dsk_radio_rcv_set_freq_play(fm_ctrl->cur_freq -100);		
						}				
					dsk_radio_rcv_set_freq_play(fm_ctrl->cur_freq);					
				}
				
			}
		}

		dsk_radio_get_min_freq(&fm_ctrl->cur_min_freq);
		dsk_radio_get_max_freq(&fm_ctrl->cur_max_freq);

		__msg("freq range = [%d, %d]\n", fm_ctrl->cur_min_freq, fm_ctrl->cur_max_freq);
	}

	return EPDK_OK;
}

//初始化模块
static __s32 fm_init_module(void* cb_arg, FM_CTRLDATA_T *fm_ctrl)
{
	reg_system_para_t* system_para;
	system_para = (reg_system_para_t*)dsk_reg_get_para_by_app(REG_APP_SYSTEM);

	if(!dsk_radio_is_open())
	{
	
		reg_fm_para_t* para;
		get_reg_pointer(para, reg_fm_para_t, REG_APP_FM);
		dsk_radio_open();
		if(system_para)
		    dsk_volume_set(system_para->volume);
		    
		dsk_radio_rcv_open();			
		dsk_radio_set_band(DSK_RADIO_BAND_US_EUP);

		//搜台灵敏度
		dsk_radio_set_threshold(DSK_RADIO_THRESHOLD_MID);	//DSK_RADIO_THRESHOLD_MID	//DSK_RADIO_THRESHOLD_LOW
		dsk_radio_set_audio_mode(DSK_RADIO_AUDIO_STEREO);
		dsk_radio_rcv_get_search_result(para);		
		dsk_radio_rcv_set_play_list_type(PLAY_LIST_SEARCH_RESULT);
	}

	dsk_radio_set_cb(DSK_RADIO_EVENT_SEARCH_SUCCESS, esKRNL_GetCallBack(cb_srhch_success), cb_arg);
	dsk_radio_set_cb(DSK_RADIO_EVENT_SEARCH_FAIL, esKRNL_GetCallBack(cb_srhch_fail), cb_arg);
	dsk_radio_set_cb(DSK_RADIO_EVENT_SEARCH_OVER, esKRNL_GetCallBack(cb_srhch_over), cb_arg);
	//dsk_radio_rcv_set_search_cb(esKRNL_GetCallBack(fm_auto_search_cb),(void *)fmplay_menu_attr); 
	fm_init_data(fm_ctrl);

	return EPDK_OK;
}

/************************************************************************/
// fm 操作通知
/************************************************************************/
#define IsOPFunc(_op, _opFun)  ((_op) == (__s32)_opFun)
static __s32 _fm_op_notify(__s32 op_function)
{
	__wrn("fm_change_channelno\n");
	if (IsOPFunc(op_function, fm_change_channelno))
	{
		FM_WND_T *wnd_para;
		FM_GetWndPara(wnd_para, FM_WND_T, FM_GetManWnd());

		if (wnd_para)
		{
			if (wnd_para->h_main_lyr && wnd_para->h_main_wnd)
			{
				FM_UpdateUI(wnd_para->h_main_wnd);
			}
		}
	}
	__wrn("fm_change_channelno end\n");
// 	else if (IsOPFunc(op_function, fm_change_channelno))
// 	{
// 	}

	return EPDK_OK;
}

void  fm_cmd2parent(H_WIN hwin, __s32 id, __s32 data1, __s32 data2)
{
	__gui_msg_t msg;

	msg.h_deswin = GUI_WinGetParent(hwin);
	msg.h_srcwin = NULL;
	msg.id = GUI_MSG_COMMAND;
	msg.dwAddData1 = MAKELONG(GUI_WinGetItemId(hwin), id);
	msg.dwAddData2 = data1;
	msg.dwReserved = data2;

	GUI_SendNotifyMessage(&msg);
}

#ifdef SUPPORT_DIGIT_KEY
__s32 radio_n2digits(__s32 num, __s32 * digits)
{
	__s32 i=0;

	do
	{
		digits[i]=num%10;
		num=num/10;
		i++;
	}
	while(num!=0);
	
	return i;
}

static __s32 radio_cal_digit_num(__s32 n)
{
	__s32 i;

	if(0 == n)
	{
		return 1;
	}
	
	i = 0;
	while(n)
	{
		i++;
		n /= 10;
	}

	return i;
}

static __s32 radio_digit_layer_draw(__gui_msg_t *msg)
{    
    __s32 	 i;
    __s32 	 j;
    RECT 	 rect;
	char     text[256], tmp_text[64];
    GUI_RECT gui_rect;
	__s32 	 total_song_num, cur_song_num;
	__s32 digits[16]={0};
  	__s32	 ret;		
	HTHEME bg_hdl;
    FM_WND_T *wnd_para;
	
	FM_GetWndPara(wnd_para, FM_WND_T, msg->h_deswin);    
  
    if(NULL == wnd_para->hdigit_layer)
    {
        __wrn("digit layer is null...\n");
        return EPDK_FAIL;
    }

    if(GUI_LYRWIN_STA_ON != GUI_LyrWinGetSta(wnd_para->hdigit_layer))
    {
        __wrn("digit layer is not on...\n");
        return EPDK_FAIL;
    }

	
    GUI_LyrWinSel(wnd_para->hdigit_layer);
    GUI_LyrWinGetScnWindow(wnd_para->hdigit_layer, &rect);

	rect.x = 0;
	rect.y = 0;
	

	bg_hdl = dsk_theme_open(ID_INIT_DIG_BG_BMP);
	if(NULL == bg_hdl)
	{
		__wrn("dsk_theme_open fail...\n");
		return EPDK_FAIL;
	}	
	GUI_BMP_Draw(dsk_theme_hdl2buf(bg_hdl), 0, 0);
	if(bg_hdl)
	{
		dsk_theme_close(bg_hdl);
		bg_hdl = NULL;
	}
	
    gui_rect.x0 = 0;
    gui_rect.y0 = 6;
    gui_rect.x1 = gui_rect.x0+rect.width-1;
    gui_rect.y1 = gui_rect.y0+rect.height-1;

	ret = radio_n2digits(wnd_para->ndigit_fm_freq, digits);


	//曲目: Cur/Total
	GUI_UC_SetEncodeUTF8();
	GUI_SetColor(GUI_RED);
	GUI_DrawHLine((rect.y+rect.height/2+10),rect.x+5,rect.x+rect.width-11);
	GUI_SetColor(GUI_WHITE);
	get_menu_text(STRING_FM_DIG_TIP1, text, sizeof(text));
	GUI_DispStringInRect(text, &gui_rect, GUI_TA_HCENTER|GUI_TA_TOP);
    gui_rect.y0 = gui_rect.y0 + 24;
    gui_rect.y1 = gui_rect.y0+rect.height-1;
	get_menu_text(STRING_FM_DIG_TIP2, text, sizeof(text));
	GUI_DispStringInRect(text, &gui_rect, GUI_TA_HCENTER|GUI_TA_TOP);
	
	
	{
		__s32 x, y;
		__s32 w;
		HBMP hbmp;
		HTHEME hdl;
		static __s32 theme_id[10]=
		{
			ID_INIT_D_0_BMP,
			ID_INIT_D_1_BMP,
			ID_INIT_D_2_BMP,
			ID_INIT_D_3_BMP,
			ID_INIT_D_4_BMP,
			ID_INIT_D_5_BMP,
			ID_INIT_D_6_BMP,
			ID_INIT_D_7_BMP,
			ID_INIT_D_8_BMP,
			ID_INIT_D_9_BMP,
		};

		//com_set_palette_by_id(ID_MOVIE_DIGIT_PAL_BMP);
		x = rect.x+rect.width/2-10*ret;
		for(i = 0 ; i < ret ; i++)
		{
			hdl = dsk_theme_open(theme_id[digits[ret-1-i]]);
			if(NULL == hdl)
			{
				__wrn("dsk_theme_open fail...\n");
				return EPDK_FAIL;
			}
			
			hbmp = bmp_open(dsk_theme_hdl2buf(hdl));
			if(NULL == hbmp)
			{
				__wrn("bmp_open fail...\n");
				if(NULL == hdl)
				{
					dsk_theme_close(hdl);
					hdl = NULL;
				}
				return EPDK_FAIL;
			}

			y = rect.y+((__s32)(rect.height+38)-(__s32)bmp_get_height(hbmp))/2 + 10;
			
			GUI_LyrWinSel(wnd_para->hdigit_layer);
			__wrn("x=%d, y=%d\n", x, y);
			GUI_BMP_Draw(dsk_theme_hdl2buf(hdl), x, y);
			GUI_LyrWinSetTop(wnd_para->hdigit_layer);
			
			x += bmp_get_width(hbmp);									

			if(hbmp)
			{
				bmp_close(hbmp);
				hbmp = NULL;
			}
			
			if(hdl)
			{
				dsk_theme_close(hdl);
				hdl = NULL;
			}
		}
	}

    return EPDK_OK;	
}

#endif
#ifdef SUPPORT_FM_VOLUME
#define g_memset	memset
#ifdef FOR_800X480_UI
#define slide_bar_length 240
#define slide_bar_block_length 8
#else
#define slide_bar_length 252
#define slide_bar_block_length 4
#endif
#define	max_volume		30
#if 0
static H_WIN vol_layer_create( RECT *rect)
{
	FB	fb = {
    	{0, 0},                                   		
    	{0, 0, 0},                                      
    	{FB_TYPE_RGB, {PIXEL_MONO_8BPP, 0, 0}},    
	};
	__disp_layer_para_t para = 
	{
    	DISP_LAYER_WORK_MODE_PALETTE,                   
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
		"layer",
		NULL,
		GUI_LYRWIN_STA_SUSPEND,
		0
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

#endif
static __s32  fm_paint_volume(__gui_msg_t *msg )
{
	GUI_MEMDEV_Handle	draw_mem;  
	FM_UI_PARA_T *fm_uipara;
	__s32 i;
    FM_WND_T *wnd_para; 
	__s32 X = 0, Y = 0;		
	GUI_RECT vol_rect;
	char vol_text[3];
    RECT rect;
    GUI_RECT gui_rect;
	
     FM_GetWndPara(wnd_para, FM_WND_T, msg->h_deswin);    

    if(NULL == wnd_para)
    {
        __wrn("invalid para...\n");
        return EPDK_FAIL;
    }

    if(NULL == wnd_para->h_vol_layer)
    {
        __wrn("h_vol_layer layer is null...\n");
        return EPDK_FAIL;
    }
	if (GUI_LyrWinGetSta( wnd_para->h_vol_layer ) != GUI_LYRWIN_STA_ON)
	{
		__msg("wnd_para->h_vol_layer!= GUI_LYRWIN_STA_ON\n");
		return EPDK_FAIL;	
	}
	fm_uipara = (FM_UI_PARA_T *)get_fm_uipara();
	
	GUI_LyrWinSel(wnd_para->h_vol_layer);
	GUI_SetBkColor(APP_COLOR_BLACK);
	GUI_LyrWinSetTop(wnd_para->h_vol_layer);
#ifdef FOR_800X480_UI
	GUI_Clear();
	com_set_palette_by_id(ID_FM_COLOURTABLE_BMP);
	GUI_LyrWinGetScnWindow(wnd_para->h_vol_layer, &rect);

	//rect.x = 0;
	//rect.y = 0;

	__wrn("rect.x=%d, rect.y=%d, rect.width=%d, rect.height=%d\n",
		 rect.x, rect.y, rect.width, rect.height);
	
     	GUI_SetFont(SWFFont);
   	GUI_SetColor(GUI_WHITE);
    	GUI_UC_SetEncodeUTF8();
	//GUI_DrawHLine((rect.y+rect.height/2-10),rect.x,rect.x+rect.width-1);
	vol_rect.x0 = fm_uipara->volume_value_pos.x;
	vol_rect.y0 = fm_uipara->volume_value_pos.y;
	vol_rect.x1 = vol_rect.x0 + fm_uipara->volume_value_pos.width;
	vol_rect.y1 = vol_rect.y0 + fm_uipara->volume_value_pos.height;
	GUI_ClearRect(vol_rect.x0, vol_rect.y0, vol_rect.x1,vol_rect.y1);

	wnd_para->volume=dsk_volume_get();

	__msg("###  222 volume = %d ###\n", dsk_volume_get());
	if (wnd_para->volume == 0)	//音量为0,绘制静音图标
	{
		GUI_BMP_Draw(dsk_theme_hdl2buf(wnd_para->h_bmp_mute), fm_uipara->bmp_volume_l.x, fm_uipara->bmp_volume_l.y);
		GUI_BMP_Draw(dsk_theme_hdl2buf(wnd_para->h_bmp_mute), fm_uipara->bmp_volume_r.x, fm_uipara->bmp_volume_r.y);
	}
	else
	{
		GUI_BMP_Draw(dsk_theme_hdl2buf(wnd_para->h_bmp_volume_l), fm_uipara->bmp_volume_l.x, fm_uipara->bmp_volume_l.y);
		GUI_BMP_Draw(dsk_theme_hdl2buf(wnd_para->h_bmp_volume_r), fm_uipara->bmp_volume_r.x, fm_uipara->bmp_volume_r.y);
	}

	GUI_BMP_Draw(dsk_theme_hdl2buf(wnd_para->h_bmp_slide_bar), fm_uipara->bmp_slide_bar.x, fm_uipara->bmp_slide_bar.y);

	for (i=0; i<((wnd_para->volume*slide_bar_length)/(max_volume*slide_bar_block_length)); i++)
	{
#if 0	
		if (i == 0)
			GUI_BMP_Draw(dsk_theme_hdl2buf(wnd_para->h_bmp_slide_block_l), fm_uipara->bmp_slide_block_l.x, fm_uipara->bmp_slide_block_l.y);	
		else if( i == (slide_bar_length/slide_bar_block_length-1) )
			GUI_BMP_Draw(dsk_theme_hdl2buf(wnd_para->h_bmp_slide_block_r), fm_uipara->bmp_slide_block_r.x, fm_uipara->bmp_slide_block_r.y);
		else
#endif			
			GUI_BMP_Draw(dsk_theme_hdl2buf(wnd_para->h_bmp_slide_block),fm_uipara->bmp_slide_block_l.x+fm_uipara->bmp_slide_block_l.width+(i-1)*slide_bar_block_length+4, fm_uipara->bmp_slide_block_l.y);
	}
	
	uint2str(wnd_para->volume, vol_text);
	
	__msg("vol_rect.x0=%d, vol_rect.y0=%d, vol_rect.x1=%d, vol_rect.y1=%d\n",
		 vol_rect.x0, vol_rect.y0, vol_rect.x1, vol_rect.y1);
	__msg("vol_text=%s\n",vol_text);

	GUI_DispStringInRect(vol_text, &vol_rect, GUI_TA_VCENTER);
	GUI_LyrWinSetTop(wnd_para->h_vol_layer);
#else
//	draw_mem = GUI_MEMDEV_Create(0, 0,fm_uipara->volume_bar_lyr.width-1, fm_uipara->volume_bar_lyr.height-1);
	draw_mem = GUI_MEMDEV_Create(fm_uipara->volume_bar_lyr.x, fm_uipara->volume_bar_lyr.y,fm_uipara->volume_bar_lyr.width-1, fm_uipara->volume_bar_lyr.height-1);
	ASSERT(draw_mem);
	GUI_MEMDEV_Select(draw_mem);
	GUI_Clear();

	//page_ctrl->disp_type = PAINT_PAGE_VOLUME;

	__here__;
	wnd_para->volume=dsk_volume_get();

	if (wnd_para->volume == 0)	//音量为0,绘制静音图标
	{
		GUI_BMP_Draw(dsk_theme_hdl2buf(wnd_para->h_bmp_mute), fm_uipara->bmp_volume_l.x, fm_uipara->bmp_volume_l.y);
		GUI_BMP_Draw(dsk_theme_hdl2buf(wnd_para->h_bmp_mute), fm_uipara->bmp_volume_r.x, fm_uipara->bmp_volume_r.y);
	}
	else
	{
		GUI_BMP_Draw(dsk_theme_hdl2buf(wnd_para->h_bmp_volume_l), fm_uipara->bmp_volume_l.x, fm_uipara->bmp_volume_l.y);
		GUI_BMP_Draw(dsk_theme_hdl2buf(wnd_para->h_bmp_volume_r), fm_uipara->bmp_volume_r.x, fm_uipara->bmp_volume_r.y);
	}

	GUI_BMP_Draw(dsk_theme_hdl2buf(wnd_para->h_bmp_slide_bar), fm_uipara->bmp_slide_bar.x, fm_uipara->bmp_slide_bar.y);

	for (i=0; i<((wnd_para->volume*slide_bar_length)/(max_volume*slide_bar_block_length)); i++)
	{
		if (i == 0)
			GUI_BMP_Draw(dsk_theme_hdl2buf(wnd_para->h_bmp_slide_block_l), fm_uipara->bmp_slide_block_l.x, fm_uipara->bmp_slide_block_l.y);	
		else if( i == (slide_bar_length/slide_bar_block_length-1) )
			GUI_BMP_Draw(dsk_theme_hdl2buf(wnd_para->h_bmp_slide_block_r), fm_uipara->bmp_slide_block_r.x, fm_uipara->bmp_slide_block_r.y);
		else
			GUI_BMP_Draw(dsk_theme_hdl2buf(wnd_para->h_bmp_slide_block),fm_uipara->bmp_slide_block_l.x+fm_uipara->bmp_slide_block_l.width+(i-1)*slide_bar_block_length, fm_uipara->bmp_slide_block_l.y);
	}
	
    	GUI_SetColor(GUI_WHITE);
	vol_rect.x0 = fm_uipara->volume_value_pos.x;
	vol_rect.y0 = fm_uipara->volume_value_pos.y;
	vol_rect.x1 = vol_rect.x0 + fm_uipara->volume_value_pos.width;
	vol_rect.y1 = vol_rect.y0 + fm_uipara->volume_value_pos.height;
	GUI_ClearRect(vol_rect.x0, vol_rect.y0, vol_rect.x1,vol_rect.y1);

	uint2str(wnd_para->volume, vol_text);

	GUI_DispStringInRect(vol_text, &vol_rect, GUI_TA_VCENTER);
	
	GUI_SetDrawMode(LCD_DRAWMODE_NORMAL);
	GUI_MEMDEV_CopyToLCD(draw_mem); 
	GUI_MEMDEV_Select( NULL );			
	GUI_MEMDEV_Delete(draw_mem);
	draw_mem = NULL;	
#endif	
__here__;
	return EPDK_OK ;
}
__s32 fm_delete_vol_layer(H_WIN fm_frame_win)
{
	FM_WND_T *wnd_para;
	FM_GetWndPara(wnd_para, FM_WND_T, fm_frame_win);

	if(NULL == wnd_para)
	{
		return EPDK_FAIL;
	}

	if(wnd_para->h_vol_layer)
	{
		dsk_theme_close(wnd_para->h_bmp_volume_l);
		dsk_theme_close(wnd_para->h_bmp_volume_r);
		dsk_theme_close(wnd_para->h_bmp_mute);
		dsk_theme_close(wnd_para->h_bmp_slide_bar);
		dsk_theme_close(wnd_para->h_bmp_slide_block);
		dsk_theme_close(wnd_para->h_bmp_slide_block_l);
		dsk_theme_close(wnd_para->h_bmp_slide_block_r);
		GUI_LyrWinDelete(wnd_para->h_vol_layer);
		wnd_para->h_vol_layer = NULL;
	}

	if(wnd_para->vol_timmer_id)
	{
		if(GUI_IsTimerInstalled(fm_frame_win, wnd_para->vol_timmer_id))
		{
			GUI_KillTimer(fm_frame_win, wnd_para->vol_timmer_id);
		}
	}
	
	dsk_reg_flush();	//wqx 20130917

	return EPDK_OK;
	
}
#endif

#if 0
static __s32 __fm_volume_scene_create( FM_WND_T *wnd_para)
{
	__s32 ret;	
	
	if (NULL == wnd_para)
	{
		__err("invalid para...\n");
		return EPDK_FAIL;
	}
	
 	{
 		fm_volume_create_para_t create_para;
 		eLIBs_memset(&create_para, 0, sizeof(fm_volume_create_para_t));
 		create_para.hparent = wnd_para->h_main_wnd;
 		create_para.scene_id = FM_VOLUME_ID;
		create_para.max_val = AUDIO_DEVICE_VOLUME_MAX;
		create_para.min_val = AUDIO_DEVICE_VOLUME_MIN;
		create_para.cur_val = dsk_volume_get();
 		wnd_para->volume_scene = fm_volume_scene_create(&create_para);
 		if (NULL == wnd_para->volume_scene)
 		{
 			__msg("movie_volume_scene_create fail...\n");
 			return EPDK_FAIL;
 		}
 	}
	
	return EPDK_OK;
}

static __s32 __fm_volume_scene_delete( FM_WND_T *wnd_para)
{
	__s32 ret;	
	
	if (NULL == wnd_para)
	{
		__err("invalid para...\n");
		return EPDK_FAIL;
	}
	
 	{
 		if (wnd_para->volume_scene)
 		{
 			fm_volume_scene_delete(wnd_para->volume_scene);
 			wnd_para->volume_scene = NULL;
 		}
 	}
	
	return EPDK_OK;
}


static __s32 __fm_volume_scene_set_focus(FM_WND_T *wnd_para)
{
	__s32 ret;	
	
	if (NULL == wnd_para)
	{
		__err("invalid para...\n");
		return EPDK_FAIL;
	}
	
	{
		if (wnd_para->volume_scene)
		{
			fm_volume_scene_set_focus(wnd_para->volume_scene);			
		}
	}
	
	return EPDK_OK;
}
#endif
//////////////////////////////////////////////////////////////////////////
//按键消息处理,主要因为在sset起来以后FM 的frm没收到按键消息
//返回true 为不需要向下发、否则需要
//////////////////////////////////////////////////////////////////////////
static __bool fm_key_proc(__gui_msg_t *msg)
{
	FM_WND_T *wnd_para;
	static __s32 manw_lastkey = -1;
	__bool ret = EPDK_FALSE;
	
	FM_GetWndPara(wnd_para, FM_WND_T, msg->h_deswin);
	//__wrn("msg->dwReserved=%d msg->dwAddData1=%d\n", msg->dwReserved,msg->dwAddData1);
	__wrn("---FM manwin-data 2 =%d, data1 = %d, lastkey = %d\n",msg->dwAddData2,msg->dwAddData1,manw_lastkey );   
	
	//////////////
	if(1 == g_fm_exit)
		return EPDK_TRUE;
	////////////
	
	if(wnd_para->h_sset)
	{	
		if(msg->dwAddData2 == KEY_UP_ACTION)
		{
#if BEETLES_KEY_REDEF
			switch(msg->dwAddData1)
#else
			switch(manw_lastkey)
#endif			
			{
				case GUI_MSG_KEY_LONGMENU:
				case GUI_MSG_KEY_MENU:
				{
					ret = EPDK_TRUE;
					break;
				}

				case GUI_MSG_KEY_ESCAPE:
				case GUI_MSG_KEY_LONGESCAPE:
				{	
					NOTIFY_MSG(FM_COMMAND, msg->h_deswin, FM_GetManWnd(), ID_FM_CMD_DESTROY_SSET, 0, 0);
					ret = EPDK_TRUE;
					break;
				}
				default:
				{	
					ret = EPDK_FALSE;
					break;
				}	
			}	
			manw_lastkey = -1;
		}
		else
		{
			switch(msg->dwAddData1)
			{
				case GUI_MSG_KEY_LONGMENU:
				case GUI_MSG_KEY_MENU:
				//{
				//	ret = EPDK_TRUE;
				//	break;
				//}
				case GUI_MSG_KEY_ESCAPE:
				case GUI_MSG_KEY_LONGESCAPE:
				{
#if 0					
					manw_lastkey = msg->dwAddData1;
					ret = EPDK_TRUE;
					break;
#else
					NOTIFY_MSG(FM_COMMAND, msg->h_deswin, FM_GetManWnd(), ID_FM_CMD_DESTROY_SSET, 0, 0);
					ret = EPDK_TRUE;
					break;
#endif
				}	
				default:
				{
					ret = EPDK_FALSE;
					break;
				}	
			}	
		}
	
	}
	else
	{
#ifdef SUPPORT_DIGIT_KEY
		if(msg->dwAddData2 == KEY_UP_ACTION)
		{
			switch(msg->dwAddData1)
			{
#if BEETLES_KEY_REDEF
				case GUI_MSG_KEY_CALENDAR:
				case GUI_MSG_KEY_LONGCALENDAR:
				{
					if(NULL== wnd_para->hdigit_layer)
					{
						RECT rect;
						__s32 width, height;
						__s32 scn_width = 480;
						__s32 scn_height = 272;
						//dsk_display_get_size(&scn_width, &scn_height);
						width = 132;////33*4;
						height = 68;

						rect.x = (scn_width - width)/2;
						rect.y = scn_height - height;
						rect.width = width;
						rect.height = height;
						wnd_para->hdigit_layer = com_layer_create(&rect, 1, PIXEL_COLOR_ARGB8888, GUI_LYRWIN_STA_ON, "digit layer");
						if(NULL == wnd_para->hdigit_layer)
						{
							__wrn("digit layer create fail.....\n");
						}
						else
						{
							__wrn("digit layer create ok....\n");
						}
						////////
						if(!GUI_IsTimerInstalled(msg->h_deswin, wnd_para->digit_timmer_id))
						{
							wnd_para->ndigit_fm_freq = 0;
							GUI_SetTimer(msg->h_deswin, wnd_para->digit_timmer_id, DIGIT_TIME_CNT, NULL);
						}
						else
						{
							GUI_ResetTimer(msg->h_deswin, wnd_para->digit_timmer_id, DIGIT_TIME_CNT, NULL);
						}
						__here__;
						draw_digits_infostr(wnd_para->hdigit_layer);
						//////////////
						{
							__gui_msg_t 		ntymsg;

							ntymsg.id			=  DSK_MSG_TIME_CHANGE;
							ntymsg.h_srcwin 		=  NULL;
							ntymsg.h_deswin 		=  GUI_WinGetHandFromName("beetles"); 
							ntymsg.dwAddData1	=  1;////0-digital lay disable;1-digital lay enable
							ntymsg.dwAddData2	=  0;
							ntymsg.dwReserved	=  NULL;

							GUI_SendMessage(&ntymsg); 
						}
						//////////////////////
					  }
					else
					{
						if(GUI_IsTimerInstalled(msg->h_deswin, wnd_para->digit_timmer_id))
						{
							wnd_para->ndigit_fm_freq = 0;
							GUI_KillTimer(msg->h_deswin, wnd_para->digit_timmer_id);
						}
						///////////
						fm_delete_digit_layer(msg->h_deswin);
					}
					ret = EPDK_TRUE;
					break;
				}
#endif
				case GUI_MSG_KEY_NUM0:
				case GUI_MSG_KEY_NUM1:
				case GUI_MSG_KEY_NUM2:
				case GUI_MSG_KEY_NUM3:
				case GUI_MSG_KEY_NUM4:
				case GUI_MSG_KEY_NUM5:
				case GUI_MSG_KEY_NUM6:
				case GUI_MSG_KEY_NUM7:
				case GUI_MSG_KEY_NUM8:
				case GUI_MSG_KEY_NUM9:
				{
					__s32 cur_val;
				
#if BEETLES_KEY_REDEF
					if(NULL== wnd_para->hdigit_layer)
					{
							__here__;
						if(GUI_IsTimerInstalled(msg->h_deswin, wnd_para->digit_timmer_id))
						{
							wnd_para->ndigit_fm_freq = 0;
							GUI_KillTimer(msg->h_deswin, wnd_para->digit_timmer_id);
						}
					}
					else
					{
							__here__;
						  if(!GUI_IsTimerInstalled(msg->h_deswin, wnd_para->digit_timmer_id))
						  {
							wnd_para->ndigit_fm_freq = 0;
							GUI_SetTimer(msg->h_deswin, wnd_para->digit_timmer_id, DIGIT_TIME_CNT, NULL);
						  }
						  else
						  {
							GUI_ResetTimer(msg->h_deswin, wnd_para->digit_timmer_id, DIGIT_TIME_CNT, NULL);
						  }
								
						   cur_val = msg->dwAddData1 - GUI_MSG_KEY_NUM0;

						   if(wnd_para->ndigit_fm_freq < 1000)
						   {
							wnd_para->ndigit_fm_freq *= 10;
							wnd_para->ndigit_fm_freq +=cur_val;
						   }
						   else
							wnd_para->ndigit_fm_freq =cur_val;
						   
						fm_digit_layer_draw(msg);

					}

					ret = EPDK_TRUE;
					break;   
#else
					if(NULL== wnd_para->hdigit_layer)
					{
						RECT rect;
						__s32 width, height;
						__s32 scn_width = 480;
						__s32 scn_height = 272;
						//dsk_display_get_size(&scn_width, &scn_height);
						width = 200;////33*4;
						height = 100;

						rect.x = (scn_width - width)/2;
						rect.y = scn_height - height;
						rect.width = width;
						rect.height = height;
						wnd_para->hdigit_layer = com_layer_create(&rect, 1, PIXEL_COLOR_ARGB8888, GUI_LYRWIN_STA_ON, "digit layer");
						if(NULL == wnd_para->hdigit_layer)
						{
							__wrn("digit layer create fail.....\n");
						}
						else
						{
							__wrn("digit layer create ok....\n");
						}
					  }
						__here__;
					  if(!GUI_IsTimerInstalled(msg->h_deswin, wnd_para->digit_timmer_id))
					  {
						wnd_para->ndigit_fm_freq = 0;
						GUI_SetTimer(msg->h_deswin, wnd_para->digit_timmer_id, DIGIT_TIME_CNT, NULL);
					  }
					  else
					  {
						GUI_ResetTimer(msg->h_deswin, wnd_para->digit_timmer_id, DIGIT_TIME_CNT, NULL);
					  }
							
					   cur_val = msg->dwAddData1 - GUI_MSG_KEY_NUM0;

					   if(wnd_para->ndigit_fm_freq < 1000)
					   {
						wnd_para->ndigit_fm_freq *= 10;
						wnd_para->ndigit_fm_freq +=cur_val;
					   }
					   else
					   	{
							wnd_para->ndigit_fm_freq =cur_val;
					   	}
					   
					__msg("------freq = %d\n", wnd_para->ndigit_fm_freq);

					radio_digit_layer_draw(msg);
					ret = EPDK_TRUE;
					break;   
#endif
				 }
				 
				
				case GUI_MSG_KEY_ENTER:
				{
					if(wnd_para->hdigit_layer)
					{
						__msg("come here to focus timmer id\n");
						fm_set_freq_play(wnd_para);
						fm_delete_digit_layer(msg->h_deswin);
						ret = EPDK_TRUE;
					}
					else
					{
						ret = EPDK_FALSE;
					}
					break;
				}
				default:
				{
					ret = EPDK_FALSE;
					break;
				}	
			}
		}
#endif
	}
#ifdef SUPPORT_FM_VOLUME
	if(msg->dwAddData2 == KEY_REPEAT_ACTION || msg->dwAddData2 == KEY_UP_ACTION)
	{
		switch(msg->dwAddData1)
		{
				case GUI_MSG_KEY_LONGVADD:
				case GUI_MSG_KEY_LONGVDEC:
				case GUI_MSG_KEY_VADD:
				case GUI_MSG_KEY_VDEC:	
				case GUI_MSG_KEY_IR_VADD:
				case GUI_MSG_KEY_IR_VDEC:
				{

				#if 0	
					FM_WND_T *wnd_para; 

					__msg("############  FM VOL ###########\n\n");
					__msg("### 111 volume = %d ###\n", dsk_volume_get());
					FM_GetWndPara(wnd_para, FM_WND_T, msg->h_deswin);
					if(wnd_para->hdigit_layer)
					{
				
						fm_delete_digit_layer(msg->h_deswin);	
					}

					//Added By Ethan;
					if(GUI_MSG_KEY_VADD== msg->dwAddData1	||	GUI_MSG_KEY_LONGVADD== msg->dwAddData1	||	GUI_MSG_KEY_IR_VADD== msg->dwAddData1)
			        {
						dsk_volume_inc();
			        }
					else if(GUI_MSG_KEY_VDEC == msg->dwAddData1	||	GUI_MSG_KEY_LONGVDEC == msg->dwAddData1	||	GUI_MSG_KEY_IR_VDEC== msg->dwAddData1)
			        {
						dsk_volume_dec();
			        }
					
					if(!wnd_para->h_vol_layer)
					{
						RECT rect;
						__s32 width, height;
						__s32 scn_width, scn_height;

						dsk_display_get_size(&scn_width, &scn_height);

						width = 400;
						height = 50;
#ifdef FOR_800X480_UI
						rect.x =30+180;// (scn_width - width)/2;
						rect.y = 220+120;//scn_height - height;
#else
						rect.x =30;// (scn_width - width)/2;
						rect.y = 220;//scn_height - height;
#endif						
						rect.width = width;
						rect.height = height;
						//wnd_para->h_vol_layer=vol_layer_create(&rect);
						wnd_para->h_vol_layer = com_layer_create(&rect, 1, PIXEL_COLOR_ARGB8888, GUI_LYRWIN_STA_ON, "fm vol layer");
						if(NULL == wnd_para->h_vol_layer)
						{
							__msg("vol layer create fail.....\n");
						}
						else
						{
							__msg("vol layer create ok....\n");
						}
						wnd_para->h_bmp_volume_l= dsk_theme_open(ID_MUSIC_VOL_SPEAKER_LEFT_BMP);
						wnd_para->h_bmp_volume_r= dsk_theme_open(ID_MUSIC_VOL_SPEAKER_RIGHT_BMP);
						wnd_para->h_bmp_mute= dsk_theme_open(ID_MUSIC_VOL_SHUT_DOWN_BMP);
						wnd_para->h_bmp_slide_bar= dsk_theme_open(ID_MUSIC_VOL_BAR_BODY_BMP);
						wnd_para->h_bmp_slide_block= dsk_theme_open(ID_MUSIC_VOL_BAR_POINT_BMP);	//ID_EBOOK_SLIDE_BLOCK_BMP
						wnd_para->h_bmp_slide_block_l= dsk_theme_open(ID_MUSIC_VOL_BAR_POINT_BMP);
						wnd_para->h_bmp_slide_block_r= dsk_theme_open(ID_MUSIC_VOL_BAR_POINT_BMP);
					  }
					else
					{
					__here__;
					}
            				GUI_LyrWinSetSta(wnd_para->h_vol_layer,GUI_LYRWIN_STA_ON);
					GUI_LyrWinSetTop(wnd_para->h_vol_layer);
			
					//create a timer to hide volume bar when timeout//		
					if(GUI_IsTimerInstalled(msg->h_deswin, wnd_para->vol_timmer_id))
					GUI_ResetTimer(msg->h_deswin, wnd_para->vol_timmer_id, 500, 0);
					else
					GUI_SetTimer(msg->h_deswin, wnd_para->vol_timmer_id, 500, 0);
					fm_paint_volume(msg);
					ret = EPDK_TRUE;
				#else
					//Added By Ethan;
					if(GUI_MSG_KEY_VADD== msg->dwAddData1	||	GUI_MSG_KEY_LONGVADD== msg->dwAddData1	||	GUI_MSG_KEY_IR_VADD== msg->dwAddData1)
			        {
						dsk_volume_inc();
			        }
					else if(GUI_MSG_KEY_VDEC == msg->dwAddData1	||	GUI_MSG_KEY_LONGVDEC == msg->dwAddData1	||	GUI_MSG_KEY_IR_VDEC== msg->dwAddData1)
			        {
						dsk_volume_dec();
			        }
				#endif
					break;
				}
				default:
					break;
		}
	}
#endif	
	return ret;
}

/************************************************************************/
// 处理sset命令
/************************************************************************/
static __s32 on_fm_sset_cmd(H_WIN hwnd, __s32 id, __s32 value)
{
	FM_WND_T *wnd_para;
	FM_GetWndPara(wnd_para, FM_WND_T, hwnd);    

	if(FM_SSET_NC_ID_UNKNOW == value)
	{
		__wrn("FM_SSET_NC_ID_UNKNOW\n");
		return EPDK_OK;
	}
    
	switch (id)
	{
	case FM_SSET_NC_EXIT:
		{
			NOTIFY_MSG(FM_COMMAND, hwnd, FM_GetManWnd(), ID_FM_CMD_DESTROY_SSET, 0, 0);
			notify_to_close_fm(hwnd, SWITCH_TO_MMENU, 0);
		}
		break;
	case FM_SSET_NC_CHANNEL:
		{//sset自己处理
		}
		break;
	case FM_SSET_NC_MSEARCH:
		{
			fm_manual_search_start(&wnd_para->fm_ctrl, 0);
		}
		break;
	case FM_SSET_NC_ASEARCH:
		{
			NOTIFY_MSG(FM_COMMAND, hwnd, FM_GetManWnd(), ID_FM_CMD_DESTROY_SSET, 0, 0);
			NOTIFY_MSG(FM_COMMAND, hwnd, FM_GetManWnd(), ID_FM_CMD_SHOW_ASEARCH_DLG, 0, 0);
			//fm_auto_search_start(&wnd_para->fm_ctrl);
		}
		break;
	case FM_SSET_NC_AUDIO:
		{
			dsk_radio_set_audio_mode(value); 
		}
		break;
	case FM_SSET_NC_RECORD:
		break;
	case FM_SSET_NC_SEL_CHANNEL:
		{
			__wrn("  fm_change_channelno:id = %d,    value = %d\n",id,value);
			fm_change_channelno(&wnd_para->fm_ctrl, value);
		}
		break;
	case FM_SSET_NC_SEL_AUDIO:
		{
			__gui_msg_t   mymsg;
			ASSERT(value == DSK_RADIO_AUDIO_STEREO || value == DSK_RADIO_AUDIO_MONO
				||value==DSK_RADIO_AUDIO_EXIT);
			if (value<3)//重刷图形，切换显示"mono"，"stereo"
				{
					__wrn("value = %d\n",value);
					dsk_radio_set_audio_mode(value);
					mymsg.h_deswin = wnd_para->h_main_wnd ;
					mymsg.id = GUI_MSG_PAINT;
					GUI_SendNotifyMessage(&mymsg);	
				}
			else //退出
				{
					NOTIFY_MSG(FM_COMMAND, NULL, hwnd, ID_FM_CMD_DESTROY_SSET, 0, 0);	
				}
			
		
		}
		break;
	case SSET_NC_TIMER:
		{
		}
		break;
	}

	return EPDK_OK;
}

/************************************************************************/
// 处理dialog命令
/************************************************************************/
static __s32 on_fm_auto_search_dlg_cmd(H_WIN hwnd, __s32 id, __s32 para)
{
	FM_WND_T *wnd_para;
	FM_GetWndPara(wnd_para, FM_WND_T, hwnd);

    switch (id)
	{
	case ADLG_CMD_EXIT:
		{
			app_dialog_destroy(wnd_para->h_dlg);
			wnd_para->h_dlg = NULL;
			if (para == ADLG_IDYES)
			{
				g_close_screen();
				fm_auto_search_start(&wnd_para->fm_ctrl);
			} 
			else
			{
			}
		}
		break;
	}

	return EPDK_OK;
}

//////////////////////////////////////////////////////////////////////////
//命令消息处理 FM_COMMAND
//////////////////////////////////////////////////////////////////////////
static __s32 on_fm_command(__gui_msg_t *msg)
{
	FM_WND_T *wnd_para;
	FM_GetWndPara(wnd_para, FM_WND_T, msg->h_deswin);

	switch (msg->dwAddData1)
	{
	case ID_FM_CMD_CREATE_SSET:
		{
			if (wnd_para->h_sset)
			{
				__msg("err, sset created \n");
			}
			else
			{
				wnd_para->sset_create_para = FM_SSET_Build_CreatePara(FM_GetManWnd());
				if (wnd_para->sset_create_para)
				{
					//创建sset
					wnd_para->h_sset = SSET_Create(wnd_para->sset_create_para);
					SSET_Set_Nodelist("fm", NODELIST_ON);

					__msg("sset created \n");
				}
			}
		}
		break;
	case ID_FM_CMD_DESTROY_SSET:
		{
			if (wnd_para->h_sset)
			{
				SSET_Destroy(wnd_para->h_sset);
				wnd_para->h_sset = NULL;
				if (wnd_para->sset_create_para)
				{
					FM_SSET_Destroy_CreatePara(wnd_para->sset_create_para);
					wnd_para->sset_create_para = NULL;
				}
				__msg("sset destroyed \n");
			}
			else
			{
				__msg("err, sset destroyed \n");
			}
		}
		break;
	case ID_FM_CMD_SHOW_ASEARCH_DLG:
		{
			__s32 str[] = {STRING_FM_CHSRH_TITLE, STRING_FM_CHSRH_TIPS};

			default_dialog(wnd_para->h_dlg, FM_GetManWnd(), ID_FM_IS_AUTO_SEARCH_DLG, ADLG_YESNO, str);
		}
		break;
	}

	return EPDK_OK;
}

//bincr: 1增加，0:减小
static __s32 __app_fm_channel_channel(__gui_msg_t* msg, __s32 bincr)
{
    H_WIN hwnd = msg->h_deswin;
	FM_WND_T *wnd_para;
	FM_GetWndPara(wnd_para, FM_WND_T, msg->h_deswin);
    
    if(1 == bincr)
    {
        if (wnd_para->fm_ctrl.search_mode == SRHMODE_AUTO
    								|| wnd_para->fm_ctrl.search_mode == SRHMODE_MANUAL)
    	{
    	}
    	else
    	{	
    		__s32  channel_id = wnd_para->fm_ctrl.channel_id + 1;
    		__wrn("channel_id = %d\n",channel_id);
    		if(channel_id >= (__s32)(wnd_para->fm_ctrl.channel_count))
    		{
    			channel_id = 0;
    		}	
    		__wrn("fm_change_channelno\n");
    		fm_change_channelno(&wnd_para->fm_ctrl, channel_id+1);
    	}
    }
    else
    {
        if (wnd_para->fm_ctrl.search_mode == SRHMODE_AUTO
			|| wnd_para->fm_ctrl.search_mode == SRHMODE_MANUAL)
		{
		}
		else
		{	
			__s32  channel_id = wnd_para->fm_ctrl.channel_id - 1;
			if(channel_id < 0)
			{
				channel_id = wnd_para->fm_ctrl.channel_count-1;
			}	

            if(channel_id < 0)
			{
				channel_id = 0;
			}
            
            if(channel_id >= (__s32)(wnd_para->fm_ctrl.channel_count))
			{
				channel_id = 0;
			}
            
			fm_change_channelno(&wnd_para->fm_ctrl, channel_id+1);
		}
    }

    return EPDK_OK;
}

//////////////////////////////////////////////////////////////////////////
//命令消息处理
//////////////////////////////////////////////////////////////////////////
static __s32 on_fm_manwnd_command(__gui_msg_t *msg)
{
	H_WIN hwnd = msg->h_deswin;
	FM_WND_T *wnd_para;
	__u16 index = 0,i;
	FM_GetWndPara(wnd_para, FM_WND_T, msg->h_deswin);

	APP_DEBUG_CBMSGEx; 

	switch (LOSWORD(msg->dwAddData1))
	{
	__wrn("msg->dwAddData1 == %d\n",msg->dwAddData1);
	case ID_FM_SCENE_SSET:
		return on_fm_sset_cmd(msg->h_deswin, HISWORD(msg->dwAddData1), msg->dwAddData2);

	case ID_FM_IS_AUTO_SEARCH_DLG:
		return on_fm_auto_search_dlg_cmd(msg->h_deswin, HISWORD(msg->dwAddData1), msg->dwAddData2);

	case FM_MAINWND_ID:
		{
			switch (HISWORD(msg->dwAddData1))
			{
			case ID_FM_MAINWND_NOTIFY_TOUCHUP:
				{
					if (wnd_para->fm_ctrl.search_mode == SRHMODE_AUTO
						|| wnd_para->fm_ctrl.search_mode == SRHMODE_MANUAL)
					{
					} 
					else
					{
						if (wnd_para->h_sset)
						{
							NOTIFY_MSG(FM_COMMAND, msg->h_deswin, FM_GetManWnd(), ID_FM_CMD_DESTROY_SSET, 0, 0);
						} 
						else
						{
							NOTIFY_MSG(FM_COMMAND, msg->h_deswin, FM_GetManWnd(), ID_FM_CMD_CREATE_SSET, 0, 0);
						}
					}
				}
				break;

			case  ID_FM_MAINWND_CLICKDOWN:
			{
				switch(msg->dwAddData2)
				{
	                case GUI_MSG_KEY_LONGENTER:
					{
							NOTIFY_MSG(FM_COMMAND, hwnd, FM_GetManWnd(), ID_FM_CMD_SHOW_ASEARCH_DLG, 0, 0);
						break;						
					}
						default:
							break;
				}
				
			}
				break;
			case  ID_FM_MAINWND_CLICKUP:
				{
					switch(msg->dwAddData2)
					{
                        #if 0
						case GUI_MSG_KEY_LONGMENU:
						{	
				                reg_fm_para_t* para;
							    __gui_msg_t   mymsg;
        			            get_reg_pointer(para, reg_fm_para_t, REG_APP_FM);
        	        
        	                            dsk_radio_rcv_save_cur_freq();

        	                            wnd_para->fm_ctrl.channel_count = dsk_radio_rcv_get_search_result_vn();
        	                            __wrn("wnd_para->fm_ctrl.channel_count=%d\n", wnd_para->fm_ctrl.channel_count);
        						
        	                            __wrn("wnd_para->fm_ctrl.cur_freq = %d, para->channel_id=%d\n", wnd_para->fm_ctrl.cur_freq,para->channel_id);
        	                            wnd_para->fm_ctrl.channel_id = para->channel_id;
        	                            
        	                            __wrn("wnd_para->fm_ctrl.channel_id=%d\n", wnd_para->fm_ctrl.channel_id);
								//重刷FM主界面图层，更新保存手动搜索的台数
								mymsg.h_deswin = wnd_para->h_main_wnd ;
								mymsg.id = GUI_MSG_PAINT;
								GUI_SendNotifyMessage(&mymsg);	
								
	                            app_fm_notify_delete_sub_scene(msg->h_deswin);
	                            
	                            {
	                                H_WIN dlg;
	                    			__s32 lang_id[]={STRING_FM_CUE, STRING_FM_SAVE_FREQ_OK};
	                                
	                    			default_dialog(dlg, msg->h_deswin, ID_FM_SAVE_FREQ_DLG, ADLG_OK, lang_id);
	                    			esKRNL_TimeDly(100);
	                    			app_dialog_destroy(dlg);
	                            }
				                            
							break;
						}
                        #endif
						case GUI_MSG_KEY_MENU:
						if (wnd_para->fm_ctrl.search_mode == SRHMODE_AUTO
							|| wnd_para->fm_ctrl.search_mode == SRHMODE_MANUAL)
						{
						} 
						else
						{
							if (wnd_para->h_sset)
							{
								NOTIFY_MSG(FM_COMMAND, msg->h_deswin, FM_GetManWnd(), ID_FM_CMD_DESTROY_SSET, 0, 0);
							} 
							else
							{
								NOTIFY_MSG(FM_COMMAND, msg->h_deswin, FM_GetManWnd(), ID_FM_CMD_CREATE_SSET, 0, 0);
							}
						}
						break;
					
						case GUI_MSG_KEY_ESCAPE:
						{
							if(wnd_para->h_sset)
							{	
								NOTIFY_MSG(FM_COMMAND, hwnd, FM_GetManWnd(), ID_FM_CMD_DESTROY_SSET, 0, 0);
							}
							
							notify_to_close_fm(hwnd, SWITCH_TO_MMENU, 0);
							break;
						}
                        #if 0
            			case GUI_MSG_KEY_ENTER:
            			{	
							if (wnd_para->fm_ctrl.search_mode == SRHMODE_AUTO
								|| wnd_para->fm_ctrl.search_mode == SRHMODE_MANUAL)
							{
							}
							else
							{	
								__s32  channel_id = wnd_para->fm_ctrl.channel_id + 1;
								__wrn("channel_id = %d\n",channel_id);
								if(channel_id >= wnd_para->fm_ctrl.channel_count)
								{
									channel_id = 0;
								}	
								__wrn("fm_change_channelno\n");
								fm_change_channelno(&wnd_para->fm_ctrl, channel_id+1);
							}	
							break;
						}	 
                        #endif
						case GUI_MSG_KEY_RIGHT:
						{
                            __wrn("GUI_MSG_KEY_RIGHT...\n");
                            __wrn("msg->dwReserved=%d\n", msg->dwReserved);
                            
                            if(0 == msg->dwReserved)//面板按键
                            {
                                __app_fm_channel_channel(msg, 1);
                            }
                            else//遥控按键
                            {
                                __app_fm_channel_channel(msg, 1);
                            }
							break;
						}
                        
      //                  case GUI_MSG_KEY_LONGENTER:
						//{
						//		NOTIFY_MSG(FM_COMMAND, hwnd, FM_GetManWnd(), ID_FM_CMD_SHOW_ASEARCH_DLG, 0, 0);
						//	break;						
						//}
                       
						case GUI_MSG_KEY_LONGRIGHT:
						{
							if (wnd_para->fm_ctrl.search_mode == SRHMODE_AUTO
								|| wnd_para->fm_ctrl.search_mode == SRHMODE_MANUAL)
							{
							}
							else
							{	
								fm_manual_search_start(&wnd_para->fm_ctrl, 0);
								//dsk_radio_rcv_set_search_cb(esKRNL_GetCallBack(fm_auto_search_cb),(void *)fmplay_menu_attr); 
							}	
							break;
						
						}
						case GUI_MSG_KEY_LONGLEFT:
						{
							if (wnd_para->fm_ctrl.search_mode == SRHMODE_AUTO
								|| wnd_para->fm_ctrl.search_mode == SRHMODE_MANUAL)
							{
							}
							else
							{	
								fm_manual_search_start(&wnd_para->fm_ctrl, 1);
								//dsk_radio_rcv_set_search_cb(esKRNL_GetCallBack(fm_auto_search_cb),(void *)fmplay_menu_attr); 
							}	
							break;
						
						}
                  
						case GUI_MSG_KEY_LEFT:
						{	 
                            __wrn("GUI_MSG_KEY_LEFT...\n");
                            __wrn("msg->dwReserved=%d\n", msg->dwReserved);
                            if(0 == msg->dwReserved)//面板按键
                            {
                                __app_fm_channel_channel(msg, 0);
                            }
                            else//遥控按键
                            {
                                __app_fm_channel_channel(msg, 0);
                            }    
							break;
						}	
						default:
							break;
					}
				}
				break;	
			fmplay_saveCH(wnd_para);
			 dsk_radio_rcv_save_search_result();	
			}
		}
		return EPDK_OK;
	}
	
	return EPDK_FAIL;
}

#ifdef SUPPORT_DIGIT_KEY
__s32 fm_delete_digit_layer(H_WIN fm_frame_win)
{
	FM_WND_T *wnd_para;
	FM_GetWndPara(wnd_para, FM_WND_T, fm_frame_win);

	if(NULL == wnd_para)
	{
		return EPDK_FAIL;
	}

	if(wnd_para->digit_timmer_id)
	{
		if(GUI_IsTimerInstalled(fm_frame_win, wnd_para->digit_timmer_id))
		{
			wnd_para->ndigit_fm_freq = 0;
			GUI_KillTimer(fm_frame_win, wnd_para->digit_timmer_id);
		}
	}

	if(wnd_para->hdigit_layer)
	{
		GUI_LyrWinDelete(wnd_para->hdigit_layer);
		wnd_para->hdigit_layer = NULL;
#if BEETLES_KEY_REDEF
		//////////////
		{
			__gui_msg_t 		ntymsg;

			ntymsg.id			=  DSK_MSG_TIME_CHANGE;
			ntymsg.h_srcwin 		=  NULL;
			ntymsg.h_deswin 		=  GUI_WinGetHandFromName("beetles"); 
			ntymsg.dwAddData1	=  0;////0-digital lay disable;1-digital lay enable
			ntymsg.dwAddData2	=  0;
			ntymsg.dwReserved	=  NULL;

			GUI_SendMessage(&ntymsg); 
		}
		//////////////////////
#endif
	}

	return EPDK_OK;
	
}

#endif

//////////////////////////////////////////////////////////////////////////
//主窗口销毁处理
//////////////////////////////////////////////////////////////////////////
static __s32 on_fm_destory(__gui_msg_t *msg)
{
	SIZE	screen_size;
	FM_WND_T *wnd_para;
	FM_GetWndPara(wnd_para, FM_WND_T, msg->h_deswin);
	
	//允许自动关屏
//	g_enable_close_scn();

	g_is_searching=0;
	
	if(dsk_radio_is_open())
	{
		dsk_radio_close();
	}
	
	if (wnd_para->h_sset)
	{
		SSET_Destroy(wnd_para->h_sset);
		wnd_para->h_sset = NULL;
	}
	if (wnd_para->sset_create_para)
	{
		FM_SSET_Destroy_CreatePara(wnd_para->sset_create_para);
		wnd_para->sset_create_para = NULL;
	}

	//g_enable_standby();

//	if (dsk_radio_rcv_get_search_result_vn() <= 1)
	{
		reg_fm_para_t* para;
		get_reg_pointer(para, reg_fm_para_t, REG_APP_FM);

//		ASSERT(wnd_para->fm_ctrl.channel_count == 1);
//		ASSERT(ValueBetweenRange(wnd_para->fm_ctrl.cur_min_freq, wnd_para->fm_ctrl.cur_freq, wnd_para->fm_ctrl.cur_max_freq));
//		fm_reg_set_channel_(para, 0, wnd_para->fm_ctrl.cur_freq);
		fm_reg_set_curfreq(para, wnd_para->fm_ctrl.cur_freq);
		__msg("cur_freq = %d, channel_count = %d\n", fm_reg_get_curfreq(para), fm_reg_get_channel_count(para));
	}
	//刷新注册表数据
	//update_to_reg();

	//TODO:release resource
#ifdef SUPPORT_DIGIT_KEY
	if(wnd_para->hdigit_layer)//cai++ for digit_num
	{
		fm_delete_digit_layer(msg->h_deswin);
	}

#endif
	if (wnd_para->h_dlg)
	{
		app_dialog_destroy(wnd_para->h_dlg);
		wnd_para->h_dlg = NULL;
	}

	if( wnd_para->h_frm_alarm_coming )
	{
//		app_alarm_frm_destory(wnd_para->h_frm_alarm_coming);
		wnd_para->h_frm_alarm_coming = NULL ;
	}
#ifdef SUPPORT_FM_VOLUME
	if(wnd_para->h_vol_layer)
	{
		fm_delete_vol_layer(msg->h_deswin);
	}

#endif
	app_com_layer_destroy(wnd_para->h_main_lyr);

	BFreeType(wnd_para, FM_WND_T);

	/////////
	//screen_size.width	 = 480;
	//screen_size.height	 = 272;
	//gscene_bgd_init(&screen_size, BGD_STATUS_SHOW, FB_TYPE_YUV);
	//gscene_bgd_set_state(BGD_STATUS_HIDE);
	/////////////////

	{//恢复系统自动关屏时间
		reg_system_para_t* para;
		para = (reg_system_para_t*)dsk_reg_get_para_by_app(REG_APP_SYSTEM);
		if(para)
		{
			g_set_close_scn_time(para->backlightoff);
			__msg("para->backlightoff=%d\n", para->backlightoff);
		}
	}   

	EndOfDebugAPPMEM();

	return 0;
}

static __s32 get_auto_close_scn_time_fm(void)
{
    __s32 bok;
    __s32 close_scn_time;
    
 
    close_scn_time = FM_AUTO_CLOSESCN;
    
    __wrn("close_scn_time=%d\n", close_scn_time);
    
    return close_scn_time;
}

//////////////////////////////////////////////////////////////////////////
//主窗口创建处理
//////////////////////////////////////////////////////////////////////////
static __s32 on_fm_create(__gui_msg_t *msg)
{
	FM_UI_PARA_T *ui;
	FM_WND_T *wnd_para; 
	__s32 auto_close_scn_time;
    
	FM_GetWndPara(wnd_para, FM_WND_T, msg->h_deswin);
	
	//禁止自动关屏
	//g_disable_close_scn();
	g_enable_close_scn();

	//g_disable_standby();
	//设置调色板
	com_set_palette_by_id(ID_FM_COLOURTABLE_BMP);
	//初始化radio
	fm_init_module((void*)msg->h_deswin, &wnd_para->fm_ctrl);
	
	//TODO:init wnd_para
	///gscene_bgd_set_state(BGD_STATUS_HIDE);
	//gscene_bgd_set_state(BGD_STATUS_SHOW); 	
	//gscene_bgd_deinit();	
	//////
	
	ui = get_fm_uipara();
	fm_layer_create(wnd_para->h_main_lyr, &ui->rt, "fm_main");
	wnd_para->h_main_wnd = fm_mainfrw_create(msg->h_deswin, wnd_para->h_main_lyr, (void*)&wnd_para->fm_ctrl);
	__msg("mainwnd = %x, lyr = %x\n", wnd_para->h_main_wnd, wnd_para->h_main_lyr);
    if(NULL == wnd_para->h_main_wnd)
    {
        return EPDK_FAIL;
    }
    
	_app_show_focus_wnd(wnd_para->h_main_wnd, wnd_para->h_main_lyr);

	GUI_LyrWinSetTop(wnd_para->h_main_lyr);   
	

#if BEETLES_FM_HEADBAR

#else
	gscene_hbar_set_state(HBAR_ST_SHOW);
#endif
	gscene_bgd_set_state(BGD_STATUS_SHOW);           
	gscene_bgd_set_bottom();
    auto_close_scn_time = get_auto_close_scn_time_fm();
    __wrn("auto_close_scn_time=%d\n", auto_close_scn_time);
    if(auto_close_scn_time > 0)//设置FM的自动关屏时间
    {
        g_set_close_scn_time(auto_close_scn_time);
    }
	fmplay_loadCH(wnd_para);
	//关背景
	//gscene_bgd_set_state(BGD_STATUS_HIDE);
	//gscene_bgd_set_state(BGD_STATUS_SHOW);

	//收音关屏提示
    {
        H_WIN dlg;
		__s32 lang_id[]={STRING_FM_CUE, STRING_FM_CLOSE_SCR_TIP};
        
		default_dialog(dlg, msg->h_deswin, ID_FM_SAVE_FREQ_DLG, ADLG_OK, lang_id);
		esKRNL_TimeDly(200);
		app_dialog_destroy(dlg);
    }	
	
#ifdef SUPPORT_DIGIT_KEY
	wnd_para->digit_timmer_id = 0x16;  //cai++ for digit_num
#endif

#ifdef SUPPORT_FM_VOLUME
	wnd_para->vol_timmer_id = 0x17; 
#endif
	return 0;
}

//主消息处理函数
static __s32 _app_fm_Proc(__gui_msg_t *msg)
{
	__bool ret = EPDK_FALSE;
	APP_DEBUG_CBMSGEx;

	switch(msg->id)
	{
	case GUI_MSG_CREATE:
		{
			esPWM_LockCpuFreq();//禁止CPU自动调节频率，防止FM杂音
			g_fm_exit = 0;
			g_is_searching = 0;
#if AB_D_UPEN
			pull_up_gpio(AB_D_PORT, AB_D_PORTNUM);//拉低LCD15，选择AB类功放///PIOA4 ab////PC13 mic det
#else
			set_record_is_play(EPDK_FALSE);
			dsk_speaker_onoff(BEETLES_SPEAK_OFF);			
			esKRNL_TimeDly(3);					
			pull_down_gpio(AB_D_PORT, AB_D_PORTNUM);//拉低LCD15，选择AB类功放///PIOA4 ab////PC13 mic det
			esKRNL_TimeDly(32);	
			set_record_is_play(EPDK_TRUE);
			dsk_speaker_onoff(BEETLES_SPEAK_ON);			
#endif
			g_FMManWnd = msg->h_deswin;
			//esCLK_Set_pll_sdram_pattern_ctl(CLK_MODE_3,CLK_ON);
			return on_fm_create(msg);
		}
	case GUI_MSG_DESTROY:
		{	
	           esPWM_UnlockCpuFreq();//允许CPU自动调节频率
			g_fm_exit = 1;
#if AB_D_UPEN
		pull_down_gpio(AB_D_PORT, AB_D_PORTNUM);
#else
		set_record_is_play(EPDK_FALSE);
		dsk_speaker_onoff(BEETLES_SPEAK_OFF);			
		esKRNL_TimeDly(5);					
		pull_up_gpio(AB_D_PORT, AB_D_PORTNUM);
		esKRNL_TimeDly(32);	
		set_record_is_play(EPDK_TRUE);
#endif
			//esCLK_Set_pll_sdram_pattern_ctl(CLK_MODE_0,CLK_OFF);
			return on_fm_destory(msg);
		}
	case DSK_MSG_HOME:
		{
			//////////
			FM_WND_T *wnd_para;

			FM_GetWndPara(wnd_para, FM_WND_T, msg->h_deswin);
			if (wnd_para->h_sset)
			{
				SSET_Destroy(wnd_para->h_sset);
				wnd_para->h_sset = NULL;
				if (wnd_para->sset_create_para)
				{
					FM_SSET_Destroy_CreatePara(wnd_para->sset_create_para);
					wnd_para->sset_create_para = NULL;
				}
				__msg("sset destroyed \n");
			}

			notify_to_close_fm(msg->h_deswin, SWITCH_TO_MMENU, 0);
		}
		return EPDK_OK;
	case DSK_MSG_KILL:
	case GUI_MSG_CLOSE:
		{
			GUI_ManWinDelete(msg->h_deswin);
		}
		break;
	case GUI_MSG_COMMAND:
		{
			if (on_fm_manwnd_command(msg) == EPDK_OK)
			{
				return EPDK_OK;
			}
		}
		break;
	case FM_COMMAND:
		{
			return on_fm_command(msg);
		}
	case GUI_MSG_KEY:	
		{
			ret = fm_key_proc(msg);
			if(EPDK_FALSE == ret)
				break;
			else
				return EPDK_OK;
		}
	case GUI_MSG_TIMER:  //cai++ for digit_num
		{
			FM_WND_T *wnd_para;

			FM_GetWndPara(wnd_para, FM_WND_T, msg->h_deswin);
#ifdef SUPPORT_DIGIT_KEY
			if(wnd_para->digit_timmer_id && (wnd_para->digit_timmer_id == msg->dwAddData1))
			{
				__msg("come here to focus timmer id\n");
				fm_set_freq_play(wnd_para);
				
				fm_delete_digit_layer(msg->h_deswin);
			}
#endif
#ifdef SUPPORT_FM_VOLUME
			if(wnd_para->vol_timmer_id && (wnd_para->vol_timmer_id == msg->dwAddData1))
			{
				__msg("come here to focus timmer id\n");
				
				fm_delete_vol_layer(msg->h_deswin);
			}
#endif			
		}
		break;
	default:
		break;
	}
	
	return GUI_ManWinDefaultProc(msg);
}


__s32 app_fm_create(root_para_t *para)
{
	__gui_manwincreate_para_t create_info;
	FM_WND_T *wnd_para;

	StartDebugAPPMEM();

	__inf("**************************************\n");
	__inf("********  enter fm app  **************\n");
	__inf("**************************************\n");

	BallocType(wnd_para, FM_WND_T);
	ZeroTypeMem(wnd_para, FM_WND_T);

	wnd_para->font      = para->font;
	wnd_para->root_type = para->root_type;

	eLIBs_memset(&create_info, 0, sizeof(__gui_manwincreate_para_t));

	create_info.name            = APP_FM;
	create_info.hParent         = para->h_parent;
	create_info.ManWindowProc   = (__pGUI_WIN_CB)esKRNL_GetCallBack((__pCBK_t)_app_fm_Proc); 
	create_info.attr            = (void*)wnd_para;
	create_info.id              = APP_FM_ID;
	create_info.hHosting        = NULL;

	return(GUI_ManWinCreate(&create_info));
}

__s32 app_fm_notify_delete_sub_scene(H_WIN hmanwin)
{
    if(NULL == hmanwin)
    {
        __wrn("invalid para...\n");
        return EPDK_FAIL;
    }
    
    SEND_MSG_EX(FM_COMMAND, NULL, hmanwin, ID_FM_CMD_DESTROY_SSET, 0, 0);
    
    return EPDK_OK;
}

__s32 app_fm_get_auto_search_state(void)
{
    return g_is_searching;
}

