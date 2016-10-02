/*
************************************************************************************************************************
*											         ePDK
*						            the Easy Portable/Player Develop Kits
*									           desktop system 
*
*						        	 (c) Copyright 2007-2010, ANDY, China
*											 All Rights Reserved
*
* File    	: page_com.h
* By      	: Andy.zhang
* Func		: character show scene 
* Version	: v1.0
* ======================================================================================================================
* 2009-11-11 15:43:43  andy.zhang  create this file, implements the fundemental interface;
************************************************************************************************************************
*/

#include "page_com.h"
#include "ebook_uipara.h"
#define g_memset	memset
#define slide_bar_length 252
#define slide_bar_block_length 4
#define	max_volume		30

typedef struct tag_ebook_page_ctrl
{
	__page_create_para_t *page_para;
	__s32 name_paint_flag,draw_bg;

	__hdle	LongString_hdl ;	//长字符串的句柄
	__bool	charge_flag;
	power_level_e	battery;
	__s32	hour;
	__s32	min;
	__s32	txt_color;
	__s32	disp_type;
	__u8	volume_scene_flag;		//为1时，按左右键实现音量增减，0时按左右键实现上下翻页
	__u8	thread_id;

	HTHEME h_bmp_play;
	HTHEME h_bmp_pause;
	HTHEME h_bmp_battery;
	HTHEME h_bmp_volume_l;
	HTHEME h_bmp_volume_r;
	HTHEME h_bmp_mute;
	HTHEME h_bmp_slide_bar;
	HTHEME h_bmp_slide_block;
	HTHEME h_bmp_slide_block_l;
	HTHEME h_bmp_slide_block_r;
	HTHEME h_bmp_bottom;

#define SUPPORT_DIGIT_KEY
#define DIGIT_TIME_CNT		300

#ifdef SUPPORT_DIGIT_KEY
	//数字选页部分
	__s32 ndigit_page;
	__u8  digit_timmer_id;	//数字选页timmer id号
	H_LYR hdigit_layer;		//显示当前数字点歌数字的图层
#endif
	
}ebook_page_ctrl_t;

static __s32 battery_id[2][6] = 
{
	{
		ID_EBOOK_BATTERY0_BMP,
		ID_EBOOK_BATTERY1_BMP,
		ID_EBOOK_BATTERY3_BMP,
		ID_EBOOK_BATTERY5_BMP,
		ID_EBOOK_BATTERY7_BMP,
		ID_EBOOK_BATTERY9_BMP,
	},
	{
		ID_EBOOK_CHARGE0_BMP,
		ID_EBOOK_CHARGE1_BMP,
		ID_EBOOK_CHARGE3_BMP,
		ID_EBOOK_CHARGE5_BMP,
		ID_EBOOK_CHARGE7_BMP,
		ID_EBOOK_CHARGE9_BMP,
	},
};

enum 
{
	PAINT_PAGE_INFO,
	PAINT_PAGE_VOLUME,
};

static __s32 Ebook_PageTimeID = 0xff;
static __s32 Ebook_VolumeTimeID = 0xfe;

__s32 ebook_page_com_long_string_start( ebook_page_ctrl_t *page_ctrl ) ;
__s32 ebook_page_com_long_string_stop( ebook_page_ctrl_t *page_ctrl ) ;
__s32 ebook_page_com_long_string_init( ebook_page_ctrl_t *page_ctrl ) ;
__s32 ebook_page_com_long_string_uninit( ebook_page_ctrl_t *page_ctrl ) ;

static void ebook_page_init(ebook_page_ctrl_t *page_ctrl)
{

	page_ctrl->volume_scene_flag = 0;
	page_ctrl->name_paint_flag = 1;
	page_ctrl->draw_bg= 1;
	page_ctrl->h_bmp_play = dsk_theme_open(ID_EBOOK_PLAY_BMP);
	page_ctrl->h_bmp_pause= dsk_theme_open(ID_EBOOK_PAUSE_BMP);
	page_ctrl->h_bmp_volume_l= dsk_theme_open(ID_EBOOK_VOLUME_L_BMP);
	page_ctrl->h_bmp_volume_r= dsk_theme_open(ID_EBOOK_VOLUME_R_BMP);
	page_ctrl->h_bmp_mute= dsk_theme_open(ID_EBOOK_MUTE_BMP);
	page_ctrl->h_bmp_slide_bar= dsk_theme_open(ID_EBOOK_SLIDE_BAR_BMP);
	page_ctrl->h_bmp_slide_block= dsk_theme_open(ID_EBOOK_SLIDE_BLOCK_BMP);
	page_ctrl->h_bmp_slide_block_l= dsk_theme_open(ID_EBOOK_SLIDE_BLOCK_L_BMP);
	page_ctrl->h_bmp_slide_block_r= dsk_theme_open(ID_EBOOK_SLIDE_BLOCK_R_BMP);
	page_ctrl->h_bmp_bottom= dsk_theme_open(ID_EBOOK_BOTTOM_BMP);
#ifdef SUPPORT_DIGIT_KEY
	page_ctrl->digit_timmer_id = 0x16;  //cai++ for digit_num
#endif

}

static void ebook_page_uninit(ebook_page_ctrl_t *page_ctrl)
{
	dsk_theme_close(page_ctrl->h_bmp_play);
	dsk_theme_close(page_ctrl->h_bmp_pause);
	dsk_theme_close(page_ctrl->h_bmp_volume_l);
	dsk_theme_close(page_ctrl->h_bmp_volume_r);
	dsk_theme_close(page_ctrl->h_bmp_mute);
	dsk_theme_close(page_ctrl->h_bmp_slide_bar);
	dsk_theme_close(page_ctrl->h_bmp_slide_block);
	dsk_theme_close(page_ctrl->h_bmp_slide_block_l);
	dsk_theme_close(page_ctrl->h_bmp_slide_block_r);
	dsk_theme_close(page_ctrl->h_bmp_bottom);
	dsk_theme_close(page_ctrl->h_bmp_battery);
}
/*
	自动检测时间和电池
*/
static void page_autocheck_thread(void *arg)
{
	H_WIN hwin;
	__gui_msg_t setmsg;
	__time_t time;
	ebook_page_ctrl_t *page_ctrl;
	__s32 cnt=0;
	__s32 is_full = 0;
	
	hwin = (H_WIN)arg;
	page_ctrl = (ebook_page_ctrl_t *)GUI_WinGetAddData(hwin);
	while(1)
	{
		if(esKRNL_TDelReq(EXEC_prioself) == OS_TASK_DEL_REQ)
		{
			__here__;
			esKRNL_TDel(EXEC_prioself);
			return;	
		}
		esKRNL_TimeDly(10);  
		
		cnt++;
		if (cnt >10)
		{
			__bool charge_state;
			cnt = 0;
			charge_state = dsk_get_charge_state();
			if (charge_state == 1)	//充电状态
			{
				page_ctrl->charge_flag = 1;
			}
			else
			{
				page_ctrl->charge_flag = 0;
			}
			
#if (AW_SDK_PMU == 0)	 //不带PMU
				dsk_power_get_voltage_level(&page_ctrl->battery,&is_full);		
#else //带PMU
				dsk_power_get_battery_level(&page_ctrl->battery,&is_full);
#endif

			//page_ctrl->battery--;
//			__msg("page_ctrl->charge_flag=%d page_ctrl->battery=%d\n",page_ctrl->charge_flag,page_ctrl->battery);
			if (page_ctrl->disp_type == PAINT_PAGE_INFO)
			{
				if (page_ctrl->disp_type == PAINT_PAGE_INFO)
				{
					setmsg.id = GUI_MSG_PAINT;
					setmsg.h_deswin = hwin;
					GUI_SendNotifyMessage(&setmsg);
				}
			}
		}
	}
}
/*
	显示音量
*/
static __s32  page_paint_volume(__gui_msg_t *msg )
{
	GUI_MEMDEV_Handle	draw_mem;  
	ebook_uipara_t *ebook_uipara;
	__s32 i;
	ebook_page_ctrl_t *page_ctrl;
	
	page_ctrl = (ebook_page_ctrl_t *)GUI_WinGetAddData(msg->h_deswin);

	if (GUI_LyrWinGetSta( page_ctrl->page_para->volume_bar_layer ) == GUI_LYRWIN_STA_SUSPEND)
		return EPDK_FAIL;	

	ebook_uipara = (ebook_uipara_t *)get_ebook_uipara();
	
	GUI_LyrWinSel(page_ctrl->page_para->volume_bar_layer);
	GUI_SetBkColor(APP_COLOR_BLACK);

	draw_mem = GUI_MEMDEV_Create(0, 0,ebook_uipara->volume_bar_lyr.w-1, ebook_uipara->volume_bar_lyr.h-1);
	GUI_MEMDEV_Select(draw_mem);
	GUI_Clear();

	page_ctrl->disp_type = PAINT_PAGE_VOLUME;

	if (page_ctrl->page_para->volume == 0)	//音量为0,绘制静音图标
	{
		GUI_BMP_Draw(dsk_theme_hdl2buf(page_ctrl->h_bmp_mute), ebook_uipara->bmp_volume_l.x, ebook_uipara->bmp_volume_l.y);
		GUI_BMP_Draw(dsk_theme_hdl2buf(page_ctrl->h_bmp_mute), ebook_uipara->bmp_volume_r.x, ebook_uipara->bmp_volume_r.y);
	}
	else
	{
		GUI_BMP_Draw(dsk_theme_hdl2buf(page_ctrl->h_bmp_volume_l), ebook_uipara->bmp_volume_l.x, ebook_uipara->bmp_volume_l.y);
		GUI_BMP_Draw(dsk_theme_hdl2buf(page_ctrl->h_bmp_volume_r), ebook_uipara->bmp_volume_r.x, ebook_uipara->bmp_volume_r.y);
	}

	GUI_BMP_Draw(dsk_theme_hdl2buf(page_ctrl->h_bmp_slide_bar), ebook_uipara->bmp_slide_bar.x, ebook_uipara->bmp_slide_bar.y);

	for (i=0; i<((page_ctrl->page_para->volume*slide_bar_length)/(max_volume*slide_bar_block_length)); i++)
	{
		if (i == 0)
			GUI_BMP_Draw(dsk_theme_hdl2buf(page_ctrl->h_bmp_slide_block_l), ebook_uipara->bmp_slide_block_l.x, ebook_uipara->bmp_slide_block_l.y);	
		else if( i == (slide_bar_length/slide_bar_block_length-1) )
			GUI_BMP_Draw(dsk_theme_hdl2buf(page_ctrl->h_bmp_slide_block_r), ebook_uipara->bmp_slide_block_r.x, ebook_uipara->bmp_slide_block_r.y);
		else
			GUI_BMP_Draw(dsk_theme_hdl2buf(page_ctrl->h_bmp_slide_block),ebook_uipara->bmp_slide_block_l.x+ebook_uipara->bmp_slide_block_l.w+(i-1)*slide_bar_block_length, ebook_uipara->bmp_slide_block_l.y);
	}
	GUI_SetDrawMode(LCD_DRAWMODE_NORMAL);
	GUI_MEMDEV_CopyToLCD(draw_mem); 
	GUI_MEMDEV_Select( NULL );			
	GUI_MEMDEV_Delete(draw_mem);
	draw_mem = NULL;	

	return EPDK_OK ;
}

/*
************************************************************************************************************************
*                       				
*
*Description: 
*
*Arguments  : 
*
*Return     : 
*            
************************************************************************************************************************
*/
static __s32 page_paint_name(__gui_msg_t *msg )
{
	ebook_page_ctrl_t *page_ctrl;
	ebook_uipara_t *ebook_uipara;
	__show_info_move_t     show_info;
	GUI_RECT	gui_rect;
	
	
	page_ctrl = (ebook_page_ctrl_t *)GUI_WinGetAddData(msg->h_deswin);
	
	if (GUI_LyrWinGetSta(GUI_WinGetLyrWin(msg->h_deswin)) == GUI_LYRWIN_STA_SUSPEND)
		return EPDK_FAIL;

	eLIBs_memset( &show_info , 0 , sizeof(__show_info_move_t) ) ;
	GUI_LyrWinSel(page_ctrl->page_para->page_layer);
	GUI_SetFont(page_ctrl->page_para->font);
	GUI_SetColor(APP_COLOR_YELLOW);
	GUI_UC_SetEncodeUTF8();

	ebook_uipara = (ebook_uipara_t *)get_ebook_uipara();

	GUI_LyrWinCacheOn();

	GUI_SetBkColor(0xF0);
	gui_rect.x0 = ebook_uipara->page_name_pos.x;
	gui_rect.y0 = ebook_uipara->page_name_pos.y;
	gui_rect.x1 = gui_rect.x0 + ebook_uipara->page_name_pos.w-1;
	gui_rect.y1 = gui_rect.y0 + ebook_uipara->page_name_pos.h-1;
	GUI_ClearRectEx(&gui_rect);
	GUI_DispStringInRect(page_ctrl->page_para->name, &gui_rect, GUI_TA_HCENTER|GUI_TA_VCENTER);
					
	show_info.hlayer		= page_ctrl->page_para->page_layer;
	show_info.region.x 		= ebook_uipara->page_name_pos.x;
	show_info.region.y 		= ebook_uipara->page_name_pos.y;
	show_info.region.width 	= ebook_uipara->page_name_pos.w;
	show_info.region.height	= ebook_uipara->page_name_pos.h;
	show_info.encode_id	= EPDK_CHARSET_ENM_UTF8;
	show_info.pFont		= page_ctrl->page_para->font;
	show_info.string		= page_ctrl->page_para->name;
	show_info.bkColor		= 0xF0;
	show_info.fontColor		= APP_COLOR_YELLOW;
	show_info.align		= GUI_TA_LEFT| GUI_TA_VCENTER;
	show_info.bmp		= NULL;
	show_info.bmp_pos.x	= 0;
	show_info.bmp_pos.y	= 0;

	GUI_LongStringStop( page_ctrl->LongString_hdl );
	GUI_LongStringReset( page_ctrl->LongString_hdl , &show_info);
	GUI_LongStringStart( page_ctrl->LongString_hdl );

	GUI_LyrWinCacheOff();
		
	return EPDK_OK;
}

static __s32 page_paint_info(__gui_msg_t *msg)
{
	ebook_page_ctrl_t *page_ctrl;
	ebook_uipara_t *ebook_uipara;

	if (GUI_LyrWinGetSta(GUI_WinGetLyrWin(msg->h_deswin)) == GUI_LYRWIN_STA_SUSPEND)
		return EPDK_FAIL;	

	page_ctrl = (ebook_page_ctrl_t *)GUI_WinGetAddData(msg->h_deswin);

	page_ctrl->disp_type = PAINT_PAGE_INFO;		//设置显示类型

	ebook_uipara = (ebook_uipara_t *)get_ebook_uipara();

	{
		GUI_RECT	gui_rect;
		char		buf[128], buf1[128];
		GUI_MEMDEV_Handle	draw_mem;  
		GUI_LyrWinSel(page_ctrl->page_para->page_layer);
		draw_mem = GUI_MEMDEV_Create(0, 0, ebook_uipara->page_lyr.w, ebook_uipara->page_lyr.h);
		GUI_MEMDEV_Select(draw_mem);

		GUI_SetFont(page_ctrl->page_para->font);
		GUI_SetColor(page_ctrl->txt_color);
		GUI_UC_SetEncodeUTF8();
	//	GUI_SetDrawMode(GUI_DRAWMODE_TRANS);

		GUI_SetColor(page_ctrl->txt_color);

		GUI_SetBkColor(0xF0);

		if(page_ctrl->draw_bg==1)
		{
			gui_rect.x0 = 0;
			gui_rect.y0 = 0;
			gui_rect.x1 = gui_rect.x0 + ebook_uipara->page_lyr.w-1;
			gui_rect.y1 = gui_rect.y0 + ebook_uipara->page_lyr.h-1;
			GUI_ClearRectEx(&gui_rect);
			page_ctrl->draw_bg  = 0;
		}


		/*除了显示电子书名字的地方除外，全部清除*/
		gui_rect.x0 = 0;
		gui_rect.y0 = 0;
		gui_rect.x1 = ebook_uipara->page_name_pos.x-1;
		gui_rect.y1 = ebook_uipara->page_lyr.h-1;
		GUI_ClearRectEx(&gui_rect);
		
		gui_rect.x0 = ebook_uipara->page_name_pos.x+ebook_uipara->page_name_pos.w;
		gui_rect.y0 = 0;
		gui_rect.x1 = ebook_uipara->page_lyr.w-1;
		gui_rect.y1 = ebook_uipara->page_lyr.h-1;
		GUI_ClearRectEx(&gui_rect);

		/*paint play pause */
		GUI_SetBkColor(0xF0);
		gui_rect.x0 = ebook_uipara->bmp_play.x;
		gui_rect.y0 = ebook_uipara->bmp_play.y;
		gui_rect.x1 = gui_rect.x0 + ebook_uipara->bmp_play.w-1;
		gui_rect.y1 = gui_rect.y0 + ebook_uipara->bmp_play.h-1;
		if(page_ctrl->page_para->play_status == 0)
			GUI_BMP_Draw(dsk_theme_hdl2buf(page_ctrl->h_bmp_play), ebook_uipara->bmp_play.x, ebook_uipara->bmp_play.y);
		else
			GUI_BMP_Draw(dsk_theme_hdl2buf(page_ctrl->h_bmp_pause), ebook_uipara->bmp_play.x, ebook_uipara->bmp_play.y);

		/*paint battery*/
		
		gui_rect.x0 = ebook_uipara->bmp_battery.x;
		gui_rect.y0 = ebook_uipara->bmp_battery.y;
		gui_rect.x1 = gui_rect.x0 + ebook_uipara->bmp_battery.w-1;
		gui_rect.y1 = gui_rect.y0 + ebook_uipara->bmp_battery.h-1;

        __msg("page_ctrl->charge_flag=%d\n", page_ctrl->charge_flag);
        __msg("page_ctrl->battery=%d\n", page_ctrl->battery);
        __msg("id=%d\n", battery_id[page_ctrl->charge_flag][page_ctrl->battery]);
		if(page_ctrl->h_bmp_battery)
		{
			dsk_theme_close(page_ctrl->h_bmp_battery);
			page_ctrl->h_bmp_battery = dsk_theme_open(battery_id[page_ctrl->charge_flag][page_ctrl->battery]);
		}
		else
		{
			page_ctrl->h_bmp_battery = dsk_theme_open(battery_id[page_ctrl->charge_flag][page_ctrl->battery]);
		}
		GUI_BMP_Draw(dsk_theme_hdl2buf(page_ctrl->h_bmp_battery), ebook_uipara->bmp_battery.x, ebook_uipara->bmp_battery.y);

		gui_rect.x0 = ebook_uipara->bmp_volume_r.x;
		gui_rect.y0 = ebook_uipara->bmp_volume_r.y;
		gui_rect.x1 = gui_rect.x0 + ebook_uipara->bmp_volume_r.w-1;
		gui_rect.y1 = gui_rect.y0 + ebook_uipara->bmp_volume_r.h+5;
		GUI_BMP_Draw(dsk_theme_hdl2buf(page_ctrl->h_bmp_volume_r), ebook_uipara->bmp_volume_r.x, ebook_uipara->bmp_volume_r.y+4);

		gui_rect.x0 = ebook_uipara->page_volume_value_pos.x;
		gui_rect.y0 = ebook_uipara->page_volume_value_pos.y;
		gui_rect.x1 = gui_rect.x0 + ebook_uipara->page_volume_value_pos.w-1;
		gui_rect.y1 = gui_rect.y0 + ebook_uipara->page_volume_value_pos.h-1;
		app_sprintf(buf, "%d", page_ctrl->page_para->volume);
		GUI_DispStringInRect(buf, &gui_rect, GUI_TA_HCENTER|GUI_TA_VCENTER);
		/*page no*/
		gui_rect.x0 = ebook_uipara->page_no_pos.x;
		gui_rect.y0 = ebook_uipara->page_no_pos.y;
		gui_rect.x1 = gui_rect.x0 + ebook_uipara->page_no_pos.w-1;
		gui_rect.y1 = gui_rect.y0 + ebook_uipara->page_no_pos.h-1;
		app_sprintf(buf, "[%d/%d]", page_ctrl->page_para->cur_page, page_ctrl->page_para->total_page);
		GUI_DispStringInRect(buf, &gui_rect, GUI_TA_HCENTER|GUI_TA_VCENTER);

		GUI_SetDrawMode(LCD_DRAWMODE_NORMAL); 
		GUI_MEMDEV_CopyToLCD(draw_mem); 
		GUI_MEMDEV_Select( NULL );			
		GUI_MEMDEV_Delete(draw_mem);
		draw_mem = NULL;	
		if(page_ctrl->name_paint_flag==1)
		{
			page_paint_name(msg);
			page_ctrl->name_paint_flag = 0;
		}
	}
	return EPDK_OK;
}

#ifdef SUPPORT_DIGIT_KEY
__s32 ebook_delete_digit_layer(H_WIN h_win)
{
	ebook_page_ctrl_t *page_ctrl;
	
	page_ctrl = (ebook_page_ctrl_t *)GUI_WinGetAddData(h_win);
	
	if(NULL == page_ctrl)
	{
		return EPDK_FAIL;
	}

	if(page_ctrl->hdigit_layer)
	{
		GUI_LyrWinDelete(page_ctrl->hdigit_layer);
		page_ctrl->hdigit_layer = NULL;
	}

	if(page_ctrl->digit_timmer_id)
	{
		if(GUI_IsTimerInstalled(h_win, page_ctrl->digit_timmer_id))
		{
			GUI_KillTimer(h_win, page_ctrl->digit_timmer_id);
		}
	}

	return EPDK_OK;
	
}


static __s32 ebook_digit_layer_draw(ebook_page_ctrl_t *page_ctrl)
{   
    char text[64];
    __s32 i;
    __s32 j;
    RECT rect;
    GUI_RECT gui_rect;
	__s32 digits[16]={0};
	__s32	 ret;	
	HTHEME bg_hdl;

    if(NULL == page_ctrl)
    {
        __wrn("invalid para...\n");
        return EPDK_FAIL;
    }

    if(NULL == page_ctrl->hdigit_layer)
    {
        __wrn("digit layer is null...\n");
        return EPDK_FAIL;
    }

    if(GUI_LYRWIN_STA_ON != GUI_LyrWinGetSta(page_ctrl->hdigit_layer))
    {
        __wrn("digit layer is not on...\n");
        return EPDK_FAIL;
    }
    if(0)//(player_ctrl->ndigit_song_num > total_song_num)//112358 如果超出范围也显示歌曲编号
    {
        //dsk_langres_get_menu_text(STRING_MUSIC_NO_THIS_SONG, text, sizeof(text));
    }
    else
    {
        eLIBs_memset(text, 0, sizeof(text));
        eLIBs_int2str_dec(page_ctrl->ndigit_page, text);
    }
	GUI_LyrWinSel(page_ctrl->hdigit_layer);

	GUI_LyrWinGetScnWindow(page_ctrl->hdigit_layer, &rect);

	rect.x = 0;
	rect.y = 0;

	__wrn("rect.x=%d, rect.y=%d, rect.width=%d, rect.height=%d\n",
		 rect.x, rect.y, rect.width, rect.height);

#if 0
	GUI_SetBkColor(GUI_DARKGREEN);
	GUI_Clear();
#else
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
#endif

	__wrn("======text=%s======\n", text);

	gui_rect.x0 = 0;
	gui_rect.y0 = 6;
	gui_rect.x1 = gui_rect.x0+rect.width-1;
	gui_rect.y1 = gui_rect.y0+rect.height-1;

	ret = music_n2digits(page_ctrl->ndigit_page, digits);

	GUI_SetFont(SWFFont);

	GUI_SetColor(GUI_RED);
	get_menu_text(STRING_EBOOK_DIG_SEL,text, sizeof(text));
	GUI_UC_SetEncodeUTF8();
	GUI_DrawHLine((rect.y+rect.height/2-10),rect.x+5,rect.x+rect.width-11);
	GUI_SetColor(GUI_WHITE);
	GUI_DispStringInRect(text, &gui_rect, GUI_TA_HCENTER|GUI_TA_TOP);

	for(i = 0 ; i < ret ; i++)
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

		x = rect.x+rect.width/2-10*ret;
		for(i = 0 ; i < ret ; i++)
		{
			//__wrn("i=%d\n", i);
			//__wrn("ret-1-i=%d\n", ret-1-i);
			//__wrn("digits[ret-1-i]=%d\n", digits[ret-1-i]);
			//__wrn("theme_id[digits[ret-1-i]]=%d\n", theme_id[digits[ret-1-i]]);
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

			y = rect.y + 30 + ((__s32)(rect.height+32)-(__s32)bmp_get_height(hbmp))/ + 6;
			
			GUI_LyrWinSel(page_ctrl->hdigit_layer);
			__wrn("x=%d, y=%d\n", x, y);
			GUI_BMP_Draw(dsk_theme_hdl2buf(hdl), x, y);
			GUI_LyrWinSetTop(page_ctrl->hdigit_layer);
			
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

static __s32 page_key_proc(__gui_msg_t *msg)
{
	ebook_page_ctrl_t *page_ctrl=NULL;
	static __s32 last_key=-1 ,key_cnt=0;
	static __s32 menu_show_flag = 0;// 1 时表示菜单在显示，0 时表示隐藏

	page_ctrl = (ebook_page_ctrl_t *)GUI_WinGetAddData(msg->h_deswin);

	if(KEY_UP_ACTION==msg->dwAddData2)
	{
		if ((last_key == GUI_MSG_KEY_ENTER) || (last_key == GUI_MSG_KEY_LONGENTER))
		{
#ifdef SUPPORT_DIGIT_KEY
		
			if(page_ctrl->hdigit_layer)
			{
				__msg("come here to focus timmer id\n");

				Ebook_cmd2parent(msg->h_deswin, CMD_PAGE_SET_PAGE, page_ctrl->ndigit_page, 0);	//电子书跳到哪一页
		
				ebook_delete_digit_layer(msg->h_deswin);	
			}		
			else
#endif				
			{
				if (key_cnt < 5)
				{
					if (page_ctrl->page_para->play_status == 0)
						page_ctrl->page_para->play_status = 1;
					else
						page_ctrl->page_para->play_status = 0;

					Ebook_cmd2parent(msg->h_deswin, CMD_PAGE_SET_PLAY, page_ctrl->page_para->play_status, 0);	//更新播放状态

					page_paint_info(msg);
				}
				else
				{
					Ebook_cmd2parent(msg->h_deswin, CMD_PAGE_SET_SET, 0, 0);//连续按enter键时进入设置场景中
				}
				key_cnt = 0;
			}
		}
		else if(last_key ==GUI_MSG_KEY_ESCAPE)
		{
			if(page_ctrl->volume_scene_flag==1)//音量条在显示时，按menu键退出音量条，否则退出电子书
			{
				page_ctrl->volume_scene_flag = 0;
				GUI_LyrWinSetSta(page_ctrl->page_para->volume_bar_layer,GUI_LYR_STA_SUSPEND);
				//page_ctrl->page_para->volume_bar_layer = 0;
				GUI_KillTimer(msg->h_deswin, Ebook_VolumeTimeID);
				page_paint_info(msg);
			}
			else
			{
				Ebook_cmd2parent(msg->h_deswin, CMD_PAGE_SET_CLOSE, 0, 0);
			}
		}
#if BEETLES_KEY_REDEF
		else if(msg->dwAddData1 == GUI_MSG_KEY_MENU ||msg->dwAddData1 == GUI_MSG_KEY_LONGMENU || msg->dwAddData1 == GUI_MSG_KEY_IR_MENU || msg->dwAddData1 == GUI_MSG_KEY_LONGIR_MENU)//长按menu键弹出菜单
#else
		else if(last_key == GUI_MSG_KEY_MENU ||last_key == GUI_MSG_KEY_LONGMENU || last_key == GUI_MSG_KEY_IR_MENU || last_key == GUI_MSG_KEY_LONGIR_MENU)//长按menu键弹出菜单
#endif
		{
            if(page_ctrl->volume_scene_flag==1)//音量条在显示时，长按menu键退出音量条
			{
				page_ctrl->volume_scene_flag = 0;
				GUI_LyrWinSetSta(page_ctrl->page_para->volume_bar_layer,GUI_LYR_STA_SUSPEND);
				//page_ctrl->page_para->volume_bar_layer = 0;
				GUI_KillTimer(msg->h_deswin, Ebook_VolumeTimeID);
				page_paint_info(msg);
			}
			
			Ebook_cmd2parent(msg->h_deswin, CMD_PAGE_SET_SET, 0, 0);			
		}
#if 0
        else if(last_key == GUI_MSG_KEY_LONGVADD)//长按音量键弹起后弹出音量条  //音量键不用去管
        {
            GUI_LyrWinSetSta(page_ctrl->page_para->volume_bar_layer,GUI_LYRWIN_STA_ON);
			GUI_LyrWinSetTop(page_ctrl->page_para->volume_bar_layer);
			
			//create a timer to hide volume bar when timeout//		
			GUI_SetTimer(msg->h_deswin, Ebook_VolumeTimeID, 500, 0);
			page_paint_volume(msg);
			Ebook_cmd2parent(msg->h_deswin, CMD_PAGE_SET_VOLUME, page_ctrl->page_para->volume, 0);
			page_ctrl->volume_scene_flag = 1;	//set volume show flag
        }
#endif		
	else if(last_key == GUI_MSG_KEY_IR_MUTE || last_key == GUI_MSG_KEY_LONGIR_MUTE)
	{
		page_ctrl->page_para->volume = dsk_volume_get();
		//Ebook_cmd2parent(msg->h_deswin, CMD_PAGE_SET_VOLUME, page_ctrl->page_para->volume, 0);
	}

#ifdef 	SUPPORT_DIGIT_KEY
	else if(last_key == GUI_MSG_KEY_NUM0 || last_key == GUI_MSG_KEY_NUM1 || 	
			last_key == GUI_MSG_KEY_NUM2 || last_key == GUI_MSG_KEY_NUM3 || 			
			last_key == GUI_MSG_KEY_NUM4 || last_key == GUI_MSG_KEY_NUM5 || 			
			last_key == GUI_MSG_KEY_NUM6 || last_key == GUI_MSG_KEY_NUM7 || 	
			last_key == GUI_MSG_KEY_NUM8 || last_key == GUI_MSG_KEY_NUM9 )
	{
			__s32 cur_val;

			__msg("ready to create the hdigit_layer..................\n");
			if(NULL== page_ctrl->hdigit_layer)
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
				page_ctrl->hdigit_layer = com_layer_create(&rect, 1, PIXEL_COLOR_ARGB8888, GUI_LYRWIN_STA_ON, "digit layer");
				if(NULL == page_ctrl->hdigit_layer)
				{
					__wrn("digit layer create fail.....\n");
				}
				else
				{
					__wrn("digit layer create ok....\n");
				}
			  }
				__here__;
			  if(!GUI_IsTimerInstalled(msg->h_deswin, page_ctrl->digit_timmer_id))
			  {
				page_ctrl->ndigit_page = 0;
				GUI_SetTimer(msg->h_deswin, page_ctrl->digit_timmer_id, DIGIT_TIME_CNT, NULL);
			  }
			  else
			  {
				GUI_ResetTimer(msg->h_deswin, page_ctrl->digit_timmer_id, DIGIT_TIME_CNT, NULL);
			  }
					
			   cur_val = msg->dwAddData1 - GUI_MSG_KEY_NUM0;

			   if(page_ctrl->ndigit_page < 1000)
			   {
				page_ctrl->ndigit_page *= 10;
				page_ctrl->ndigit_page +=cur_val;
			   }
			   else
		   	{
				page_ctrl->ndigit_page =cur_val;
		   	}
			   
			__msg("------freq = %d\n", page_ctrl->ndigit_page);

			ebook_digit_layer_draw(page_ctrl);
	 }
#endif	
	}
	else
	{	
		switch(msg->dwAddData1)
		{
			case GUI_MSG_KEY_UP:	
			case GUI_MSG_KEY_LONGUP:
			case GUI_MSG_KEY_IR_UP:		//cai++ for ir
			case GUI_MSG_KEY_LONGIR_UP:
			{
				if(page_ctrl->volume_scene_flag)//volume decrease
				{
					if (page_ctrl->page_para->volume > 0)
						page_ctrl->page_para->volume--;
				
					GUI_LyrWinSetSta(page_ctrl->page_para->volume_bar_layer,GUI_LYRWIN_STA_ON);
					GUI_LyrWinSetTop(page_ctrl->page_para->volume_bar_layer);
					page_paint_volume(msg);
					GUI_ResetTimer(msg->h_deswin, Ebook_VolumeTimeID, 500, 0);
					/*send message to parent , set system volume*/
					Ebook_cmd2parent(msg->h_deswin, CMD_PAGE_SET_VOLUME, page_ctrl->page_para->volume, 0);
				}
				else
				{
					Ebook_cmd2parent(msg->h_deswin, CMD_PAGE_SET_PREV, 0, 0);//show previous page
				}
			}
				break;
			case GUI_MSG_KEY_DOWN:	
			case GUI_MSG_KEY_LONGDOWN:
			case GUI_MSG_KEY_IR_DOWN:
			case GUI_MSG_KEY_LONGIR_DOWN:
			{
				if(page_ctrl->volume_scene_flag)//volume add
				{
					if (page_ctrl->page_para->volume < 30)
						page_ctrl->page_para->volume++;
					
					GUI_LyrWinSetSta(page_ctrl->page_para->volume_bar_layer,GUI_LYRWIN_STA_ON);
					GUI_LyrWinSetTop(page_ctrl->page_para->volume_bar_layer);
					page_paint_volume(msg);
					GUI_ResetTimer(msg->h_deswin, Ebook_VolumeTimeID, 500, 0);
					
					/*send message to parent , set system volume*/
					Ebook_cmd2parent(msg->h_deswin, CMD_PAGE_SET_VOLUME, page_ctrl->page_para->volume, 0);
				}	
				else
				{
					Ebook_cmd2parent(msg->h_deswin, CMD_PAGE_SET_NEXT, 0, 0);// show next page
				}
			}
			break;
			
			case GUI_MSG_KEY_LEFT:
			case GUI_MSG_KEY_LONGLEFT:
			case GUI_MSG_KEY_IR_LEFT:
			case GUI_MSG_KEY_LONGIR_LEFT:
			{
#if 0				
				if(page_ctrl->volume_scene_flag)//volume decrease
				{
					if (page_ctrl->page_para->volume > 0)
						page_ctrl->page_para->volume--;
				
					GUI_LyrWinSetSta(page_ctrl->page_para->volume_bar_layer,GUI_LYRWIN_STA_ON);
					GUI_LyrWinSetTop(page_ctrl->page_para->volume_bar_layer);
					page_paint_volume(msg);
					GUI_ResetTimer(msg->h_deswin, Ebook_VolumeTimeID, 500, 0);
					
					/*send message to parent , set system volume*/
					Ebook_cmd2parent(msg->h_deswin, CMD_PAGE_SET_VOLUME, page_ctrl->page_para->volume, 0);
				}
				else
#endif					
				{
					Ebook_cmd2parent(msg->h_deswin, CMD_PAGE_SET_PREV, 0, 0);//show previous page
				}
			}
			break;
			
			case GUI_MSG_KEY_RIGHT:
			case GUI_MSG_KEY_LONGRIGHT://volume add
			case GUI_MSG_KEY_IR_RIGHT:
			case GUI_MSG_KEY_LONGIR_RIGHT:
			{
#if 0				
				if(page_ctrl->volume_scene_flag)
				{
					if(page_ctrl->page_para->volume < 30)
						page_ctrl->page_para->volume++;
					
					GUI_LyrWinSetSta(page_ctrl->page_para->volume_bar_layer,GUI_LYRWIN_STA_ON);
					GUI_LyrWinSetTop(page_ctrl->page_para->volume_bar_layer);
					page_paint_volume(msg);
					GUI_ResetTimer(msg->h_deswin, Ebook_VolumeTimeID, 500, 0);
					
					//send message to parent , set system volume//
					Ebook_cmd2parent(msg->h_deswin, CMD_PAGE_SET_VOLUME, page_ctrl->page_para->volume, 0);
				}	
				else
#endif					
				{
					Ebook_cmd2parent(msg->h_deswin, CMD_PAGE_SET_NEXT, 0, 0);//show next page
				}
			}
				break;
			case GUI_MSG_KEY_ENTER:	
			case GUI_MSG_KEY_LONGENTER:
			{
					key_cnt++;
			}
				break;
			case GUI_MSG_KEY_IR_VDEC:
			case GUI_MSG_KEY_LONGIR_VDEC:
			case GUI_MSG_KEY_VDEC:
			case GUI_MSG_KEY_LONGVDEC:
			{
				if(page_ctrl->volume_scene_flag == 0)
				{
					//GUI_LyrWinSetSta(page_ctrl->page_para->volume_bar_layer,GUI_LYRWIN_STA_ON);
					//GUI_LyrWinSetTop(page_ctrl->page_para->volume_bar_layer);
					
					//create a timer to hide volume bar when timeout//		
					//GUI_SetTimer(msg->h_deswin, Ebook_VolumeTimeID, 500, 0);
					//page_paint_volume(msg);
					Ebook_cmd2parent(msg->h_deswin, CMD_PAGE_SET_VOLUME, page_ctrl->page_para->volume, 0);
					page_ctrl->volume_scene_flag = 1;	//set volume show flag
				}
				else
				{
					if (page_ctrl->page_para->volume > 0)
						page_ctrl->page_para->volume--;
				
					//GUI_LyrWinSetSta(page_ctrl->page_para->volume_bar_layer,GUI_LYRWIN_STA_ON);
					//GUI_LyrWinSetTop(page_ctrl->page_para->volume_bar_layer);
					//page_paint_volume(msg);
					//GUI_ResetTimer(msg->h_deswin, Ebook_VolumeTimeID, 500, 0);
					
					/*send message to parent , set system volume*/
					Ebook_cmd2parent(msg->h_deswin, CMD_PAGE_SET_VOLUME, page_ctrl->page_para->volume, 0);				
				}
			}
				break;
			case GUI_MSG_KEY_IR_VADD:
			case GUI_MSG_KEY_LONGIR_VADD:
			case GUI_MSG_KEY_VADD:
			case GUI_MSG_KEY_LONGVADD:
			{
				if(page_ctrl->volume_scene_flag == 0)
				{
		           	//GUI_LyrWinSetSta(page_ctrl->page_para->volume_bar_layer,GUI_LYRWIN_STA_ON);
					//GUI_LyrWinSetTop(page_ctrl->page_para->volume_bar_layer);
					
					//create a timer to hide volume bar when timeout//		
					//GUI_SetTimer(msg->h_deswin, Ebook_VolumeTimeID, 500, 0);
					//page_paint_volume(msg);
					Ebook_cmd2parent(msg->h_deswin, CMD_PAGE_SET_VOLUME, page_ctrl->page_para->volume, 0);
					page_ctrl->volume_scene_flag = 1;	//set volume show flag
				}
				else
				{
					if(page_ctrl->page_para->volume < 30)
						page_ctrl->page_para->volume++;
					
					//GUI_LyrWinSetSta(page_ctrl->page_para->volume_bar_layer,GUI_LYRWIN_STA_ON);
					//GUI_LyrWinSetTop(page_ctrl->page_para->volume_bar_layer);
					//page_paint_volume(msg);
					//GUI_ResetTimer(msg->h_deswin, Ebook_VolumeTimeID, 500, 0);
					
					//send message to parent , set system volume//
					Ebook_cmd2parent(msg->h_deswin, CMD_PAGE_SET_VOLUME, page_ctrl->page_para->volume, 0);
				}
			}
				break;
			default:
				break;
		}
	}
	if(KEY_UP_ACTION==msg->dwAddData2)
	{
		last_key = -1;
	}
	else
	{
		last_key = msg->dwAddData1;
	}
	return EPDK_OK;
}
/*
************************************************************************************************************************
*                       				__cb_page_frmwin
*
*Description: 
*
*Arguments  : 
*
*Return     : 
*            
************************************************************************************************************************
*/
static __s32 __cb_page_frmwin(__gui_msg_t *msg)
{
	switch( msg->id )
	{
		case GUI_MSG_CREATE:
		{
			__s32 is_full = 0;
			ebook_page_ctrl_t *page_ctrl=NULL;
			__page_create_para_t  *page_para=NULL;
			
			page_para = (__page_create_para_t  *)GUI_WinGetAttr(msg->h_deswin);

			page_ctrl = (ebook_page_ctrl_t *)My_Balloc(sizeof(ebook_page_ctrl_t));
			if(!page_ctrl)
			{
				__err("memory balloc fail......\n");
				return EPDK_FAIL;
			}
			eLIBs_memset(page_ctrl, 0, sizeof(ebook_page_ctrl_t));
			GUI_WinSetAddData(msg->h_deswin, (__u32)page_ctrl);

			ebook_page_init(page_ctrl);
			
			page_ctrl->page_para = page_para;

			page_ctrl->txt_color = APP_EBOOK_FCS_TXT_COLOR;

			{		//  获取电池电量状态
				__bool charge_state;
				charge_state = dsk_get_charge_state();
				if (charge_state == 1)	//充电状态
				{
					page_ctrl->charge_flag = 1;
				}
				else
				{
					page_ctrl->charge_flag = 0;
				}
#if (AW_SDK_PMU == 0)	 //不带PMU
				dsk_power_get_voltage_level(&page_ctrl->battery,&is_full);		
#else //带PMU
				dsk_power_get_battery_level(&page_ctrl->battery,&is_full);
#endif
				
			}

			page_ctrl->thread_id = esKRNL_TCreate(page_autocheck_thread, (void*)msg->h_deswin, 0x2000, KRNL_priolevel3);

			ebook_page_com_long_string_init( page_ctrl );

			GUI_SetTimer(msg->h_deswin, Ebook_PageTimeID, 50, 0);

			return EPDK_OK;
		}			
		case GUI_MSG_DESTROY:
		{
			ebook_page_ctrl_t *page_ctrl;
			page_ctrl = (ebook_page_ctrl_t *)GUI_WinGetAddData(msg->h_deswin);

            __msg("*****page com destroy begin********\n");
			if(!page_ctrl)
			{
				__err("page_ctrl is null ......\n");
				return EPDK_FAIL;
			}		
            
            /* clsoe long string*/
            {
                __show_info_move_t     show_info;

                eLIBs_memset(&show_info, 0, sizeof(__show_info_move_t));
                GUI_LongStringReset(page_ctrl->LongString_hdl , &show_info);
                __msg("before GUI_LongStringDelete\n");
		ebook_page_com_long_string_uninit( page_ctrl );
		__msg("after GUI_LongStringDelete\n");
            }

            __here__;
			/* delete autocheck  thread */
			if( page_ctrl->thread_id != 0 )
			{
				while( esKRNL_TDelReq(page_ctrl->thread_id) != OS_TASK_NOT_EXIST )
					esKRNL_TimeDly(5);
				page_ctrl->thread_id = 0;
			}
            __here__;
			if (GUI_IsTimerInstalled(msg->h_deswin, Ebook_PageTimeID) == EPDK_TRUE)
			{				
				GUI_KillTimer(msg->h_deswin, Ebook_PageTimeID);
			}
            __here__;
			if (GUI_IsTimerInstalled(msg->h_deswin, Ebook_VolumeTimeID) == EPDK_TRUE)
			{				
				GUI_KillTimer(msg->h_deswin, Ebook_VolumeTimeID);
			}

            __here__;
			
            ebook_page_uninit(page_ctrl);

            __here__;
			if(page_ctrl->page_para)
			{
				My_Bfree(page_ctrl->page_para, sizeof(__page_create_para_t));
			}
            __here__;
			if(page_ctrl)
			{
				My_Bfree(page_ctrl, sizeof(ebook_page_ctrl_t));
			}
            __here__;
            __msg("*****page com destroy end********\n");
            return EPDK_OK;
		}			
			
		case GUI_MSG_CLOSE:		
		{
			Ebook_cmd2parent(msg->h_deswin, CMD_PAGE_SET_CLOSE, 0, 0);
		}
		return EPDK_OK;
			
		case GUI_MSG_PAINT:
			page_paint_info(msg);
			return EPDK_OK;
				
		case GUI_MSG_TOUCH:
			break;
				
		case GUI_MSG_KEY:
			page_key_proc(msg);
			return EPDK_OK;

		case GUI_MSG_TIMER:
		{
			ebook_page_ctrl_t *page_ctrl;
			page_ctrl = (ebook_page_ctrl_t *)GUI_WinGetAddData(msg->h_deswin);

			if(msg->dwAddData1==Ebook_PageTimeID)
			{
				if (page_ctrl->disp_type == PAINT_PAGE_VOLUME)
				{
					page_paint_info(msg);
				}
			}
			if(msg->dwAddData1==Ebook_VolumeTimeID)
			{
				page_ctrl->volume_scene_flag = 0;
				GUI_KillTimer(msg->h_deswin, Ebook_VolumeTimeID);
				GUI_LyrWinSetSta(page_ctrl->page_para->volume_bar_layer,GUI_LYR_STA_SUSPEND);
				page_paint_info(msg);
			}
#ifdef SUPPORT_DIGIT_KEY
			if(page_ctrl->digit_timmer_id && (page_ctrl->digit_timmer_id == msg->dwAddData1))
			{
				__msg("come here to focus timmer id\n");
				Ebook_cmd2parent(msg->h_deswin, CMD_PAGE_SET_PAGE, page_ctrl->ndigit_page, 0);	//电子书跳到哪一页
				
				ebook_delete_digit_layer(msg->h_deswin);
			}
#endif			
			
		}
			return EPDK_OK;
		case COM_CMD_UPDATE_PAGE:
		{
			ebook_page_ctrl_t *page_ctrl;
			page_ctrl = (ebook_page_ctrl_t *)GUI_WinGetAddData(msg->h_deswin);
			page_ctrl->page_para->cur_page = msg->dwAddData1;
			if (page_ctrl->disp_type == PAINT_PAGE_INFO)
				page_paint_info(msg);
		}	
			return EPDK_OK;
		case COM_CMD_SET_PLAY_STATUS:
		{
			ebook_page_ctrl_t *page_ctrl;
			page_ctrl = (ebook_page_ctrl_t *)GUI_WinGetAddData(msg->h_deswin);
			page_ctrl->page_para->play_status = msg->dwAddData1;
			if (page_ctrl->disp_type == PAINT_PAGE_INFO)
				page_paint_info(msg);
		}
			return EPDK_OK;
			
		case COM_CMD_STOP_LONG_STRING:
		{
			ebook_page_ctrl_t *page_ctrl;
			page_ctrl = (ebook_page_ctrl_t *)GUI_WinGetAddData(msg->h_deswin);

			ebook_page_com_long_string_stop( page_ctrl );
			//ebook_page_com_long_string_uninit( page_ctrl );
			return EPDK_OK;
		}
		case COM_CMD_START_LONG_STRING:
		{
			ebook_page_ctrl_t *page_ctrl;
			page_ctrl = (ebook_page_ctrl_t *)GUI_WinGetAddData(msg->h_deswin);

			__msg( " start long string \n");
			ebook_page_com_long_string_start( page_ctrl );
		//	ebook_page_com_long_string_init( page_ctrl ) ;
			page_paint_name(msg);
			return EPDK_OK;
		}
		case GUI_MSG_COMMAND:
			break;
			
		default:
			break;		
	}
	
	return GUI_FrmWinDefaultProc(msg);	
}


H_WIN page_frmwin_create(H_WIN parent, __page_create_para_t *para)
{	
	H_WIN 							h_win;
	__page_create_para_t			*p_para = NULL ;	
	__gui_framewincreate_para_t 	frm_create_para;
	FB  fb;
	
	p_para = (__page_create_para_t *)My_Balloc( sizeof(__page_create_para_t) );
	if( !p_para )
	{
		__err("balloc error \n");
		return 0;	
	}
	eLIBs_memset( p_para, 0, sizeof(__page_create_para_t) );	
	eLIBs_memcpy( (void *)p_para, (const void *)para, sizeof(__page_create_para_t) );

	GUI_LyrWinGetFB(para->page_layer, &fb);

	g_memset(&frm_create_para, 0, sizeof(__gui_framewincreate_para_t));
	
	frm_create_para.dwStyle          = WS_VISIBLE;
	frm_create_para.dwExStyle        = 0;
	frm_create_para.name			 = NULL;
	frm_create_para.hOwner			 = 0;
	frm_create_para.hHosting         = parent;
	frm_create_para.FrameWinProc     = __cb_page_frmwin;
	frm_create_para.id				 = EBOOK_PAGE_ID;
	frm_create_para.hLayer           = p_para->page_layer;
		
	frm_create_para.rect.x			  = 0;
	frm_create_para.rect.y			  = 0;
	frm_create_para.rect.width		  = fb.size.width;
	frm_create_para.rect.height		  = fb.size.height;
	
	frm_create_para.attr			  = (void *)p_para;
	
	h_win = GUI_FrmWinCreate(&frm_create_para);
	if( !h_win )
	{
		__err("app bar fwin create error ! \n");
	} 
	
	return h_win;
}

__s32 page_frmwin_delete(H_WIN h_win)
{
	if( h_win)
	{
		GUI_FrmWinDelete(h_win);
		h_win = 0;
	}
	return EPDK_OK;
}

__s32 ebook_page_com_long_string_init( ebook_page_ctrl_t *page_ctrl )
{
	if( page_ctrl->LongString_hdl == NULL)
	{
		__show_info_move_t show_info;

		eLIBs_memset(&show_info, 0, sizeof(__show_info_move_t));
		show_info.hlayer = page_ctrl->page_para->page_layer ;
		show_info.alpha_en = EPDK_FALSE;
		show_info.pFont = NULL;
		show_info.string = "";
		show_info.fontColor = APP_EBOOK_FCS_TXT_COLOR ;
		show_info.align = GUI_TA_LEFT | GUI_TA_VCENTER;
		show_info.bmp = NULL ;
		show_info.bmp_pos.x = 0;
		show_info.bmp_pos.y = 0;

		page_ctrl->LongString_hdl = GUI_LongStringCreate(&show_info);
		if( page_ctrl->LongString_hdl == NULL)
		{
			__wrn("create long string failed!\n");
		}
	}        
	else
	{
		__msg("long string scroll has already started!\n");
	}        
	return EPDK_OK;
}

__s32 ebook_page_com_long_string_uninit( ebook_page_ctrl_t *page_ctrl )
{
	if( page_ctrl->LongString_hdl != NULL)
	{
		GUI_LongStringDelete( page_ctrl->LongString_hdl );
		page_ctrl->LongString_hdl = NULL;
	}
	return EPDK_OK;
}

__s32 ebook_page_com_long_string_stop( ebook_page_ctrl_t *page_ctrl )
{
	if( page_ctrl->LongString_hdl != NULL)
	{
		GUI_LongStringStop( page_ctrl->LongString_hdl );
	}
	return EPDK_OK;
}

__s32 ebook_page_com_long_string_start( ebook_page_ctrl_t *page_ctrl )
{
	if( page_ctrl->LongString_hdl != NULL)
	{
		GUI_LongStringStart( page_ctrl->LongString_hdl );
	}
	return EPDK_OK;
}

