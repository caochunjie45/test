/*
**************************************************************************************************************
*											         ePDK
*						            the Easy Portable/Player Develop Kits
*									           desktop system 
*
*						        	 (c) Copyright 2007-2011, CHIPHD, China
*											 All Rights Reserved
*
* File    	: fm_ScaleBar.c
* By      	: CQQ
* Func		: 
* Version	: v1.0
* ============================================================================================================
* 2011/08/19 15:27  create this file, implements the fundamental interface;
**************************************************************************************************************
*/

#include "fm_ScaleBar.h"
#include "fm_main.h" 
//////////////////////////////////////////////////////////////////////////
#if 0 //(Ϊ�˴�ӡ��, �ɰ������ض����GUI����ʱ��������Ҫ��C�ļ���ʹ��)
//re define GUI_BMP_Draw
#ifdef GUI_BMP_Draw
#define MY_GUI_BMP_DRAW                      (ORANGEMOD_FUNC_ENTRY->ptr_GUI_BMP_Draw         	)
#undef GUI_BMP_Draw
#define GUI_BMP_Draw(_p, _x, _y)             do \
{\
	{__msg("    GUI_BMP_Draw(%d, %d)\n", _x, _y);} \
	MY_GUI_BMP_DRAW(_p, _x, _y); \
}while(0)
#endif

//re define GUI_DispStringAt
#ifdef GUI_DispStringAt
#define MY_GUI_DispStringAt                  (ORANGEMOD_FUNC_ENTRY->ptr_GUI_DispStringAt       	)
#undef GUI_DispStringAt
#define GUI_DispStringAt(_p, _x, _y)         do\
{ \
	{__msg("    GUI_DispStringAt(%d, %d)->( %s )\n", _x, _y, _p);} \
	MY_GUI_DispStringAt(_p, _x, _y); \
}while(0)
#endif

//re define GUI_DispStringInRect
#ifdef GUI_DispStringInRect
#define MY_GUI_DispStringInRect              (ORANGEMOD_FUNC_ENTRY->ptr_GUI_DispStringInRect   	)
#undef GUI_DispStringInRect
#define GUI_DispStringInRect(_p, _prt, _fmt)  do \
{ \
	{__msg("    GUI_DispStringInRect(%d, %d, %d, %d)->( %s )\n", (_prt)->x0, (_prt)->y0, (_prt)->x1, (_prt)->y1, _p);} \
	MY_GUI_DispStringInRect(_p, _prt, _fmt); \
} while (0)
#endif

#endif
//////////////////////////////////////////////////////////////////////////
//Դ��Χ�е�ֵ_src_valӳ�䵽Ŀ�귶Χ��
#define RANGE_MAPPING(_dst_min, _dst_max, _src_min, _src_max, _src_val)  \
	(((_src_val) - (_src_min)) * ((_dst_max) - (_dst_min)) / ((_src_max) - (_src_min)) + (_dst_min))

/************************************************************************/
//������Դ
/************************************************************************/
__s32 fm_freq_scale_create_res(FM_FREQ_SCALE_RES_T* res)
{
	create_bmp_res(ID_FM_SCALE_THUMB_N_BMP, res->bmp_btn_thumb[BTN_NORMAL]);
	create_bmp_res(ID_FM_SCALE_THUMB_F_BMP, res->bmp_btn_thumb[BTN_FOCUS]);
	#if (FM_SEARCH_CHN_MIN_FREQ == 87500)
	create_bmp_res(ID_FM_87_5FREQSCALE_BMP, res->bmp_scale);
	create_bmp_res(ID_FM_87_5MHZ_BMP, res->bmp_87_5MHZ);
	#else if (FM_SEARCH_CHN_MIN_FREQ == 87000)	
	create_bmp_res(ID_FM_87FREQSCALE_BMP, res->bmp_scale);
	create_bmp_res(ID_FM_87MHZ_BMP, res->bmp_87MHZ);
	#endif
	create_bmp_res(ID_FM_CURSOR_SEL_BMP, res->bmp_cursor_sel);
	create_bmp_res(ID_FM_POINT_BMP, res->bmp_point);
		
	create_bmp_res( ID_FM_108MHZ_BMP, res->bmp_108MHZ);
	
	
	create_bmp_res(ID_FM_CH_BMP, res->bmp_ch);
	create_bmp_res(ID_FM_MHZ_BMP, res->bmp_MHZ);
	create_bmp_res(ID_FM_PRESET_BMP, res->bmp_preset);
	create_bmp_res(ID_FM_SLASH_BMP, res->bmp_slash);
	create_bmp_res(ID_FM_STEREO_BMP, res->bmp_stereo);
	create_bmp_res(ID_FM_MONO_BMP, res->bmp_mono);
	
	create_bmp_res(ID_FM_L0_BMP,res->bmp_l[0]);
	create_bmp_res(ID_FM_L1_BMP,res->bmp_l[1]);
	create_bmp_res(ID_FM_L2_BMP,res->bmp_l[2]);
	create_bmp_res(ID_FM_L3_BMP,res->bmp_l[3]);
	create_bmp_res(ID_FM_L4_BMP,res->bmp_l[4]);
	create_bmp_res(ID_FM_L5_BMP,res->bmp_l[5]);
	create_bmp_res(ID_FM_L6_BMP,res->bmp_l[6]);
	create_bmp_res(ID_FM_L7_BMP,res->bmp_l[7]);
	create_bmp_res(ID_FM_L8_BMP,res->bmp_l[8]);
	create_bmp_res(ID_FM_L9_BMP,res->bmp_l[9]);
	
	create_bmp_res(ID_FM_M0_BMP,res->bmp_m[0]);
	create_bmp_res(ID_FM_M1_BMP,res->bmp_m[1]);
	create_bmp_res(ID_FM_M2_BMP,res->bmp_m[2]);
	create_bmp_res(ID_FM_M3_BMP,res->bmp_m[3]);
	create_bmp_res(ID_FM_M4_BMP,res->bmp_m[4]);
	create_bmp_res(ID_FM_M5_BMP,res->bmp_m[5]);
	create_bmp_res(ID_FM_M6_BMP,res->bmp_m[6]);
	create_bmp_res(ID_FM_M7_BMP,res->bmp_m[7]);
	create_bmp_res(ID_FM_M8_BMP,res->bmp_m[8]);
	create_bmp_res(ID_FM_M9_BMP,res->bmp_m[9]);
	
	create_bmp_res(ID_FM_B0_BMP,res->bmp_b[0]);
	create_bmp_res(ID_FM_B1_BMP,res->bmp_b[1]);
	create_bmp_res(ID_FM_B2_BMP,res->bmp_b[2]);
	create_bmp_res(ID_FM_B3_BMP,res->bmp_b[3]);
	create_bmp_res(ID_FM_B4_BMP,res->bmp_b[4]);
	create_bmp_res(ID_FM_B5_BMP,res->bmp_b[5]);
	create_bmp_res(ID_FM_B6_BMP,res->bmp_b[6]);
	create_bmp_res(ID_FM_B7_BMP,res->bmp_b[7]);
	create_bmp_res(ID_FM_B8_BMP,res->bmp_b[8]);
	create_bmp_res(ID_FM_B9_BMP,res->bmp_b[9]);
	
	return EPDK_OK;
}

/************************************************************************/
//�ͷ���Դ
/************************************************************************/
__s32 fm_freq_scale_destroy_res(FM_FREQ_SCALE_RES_T* res)
{
	destroy_bmp_res(res->bmp_btn_thumb[BTN_NORMAL]);
	destroy_bmp_res(res->bmp_btn_thumb[BTN_FOCUS]);
	#if (FM_SEARCH_CHN_MIN_FREQ == 87500)
	destroy_bmp_res(res->bmp_scale);
	destroy_bmp_res(res->bmp_87_5MHZ);
	#else if (FM_SEARCH_CHN_MIN_FREQ == 87000)	
	destroy_bmp_res(res->bmp_scale);
	destroy_bmp_res(res->bmp_87MHZ);
	#endif
	//destroy_bmp_res(res->bmp_scale);//�Ƿ������ڴ�
	destroy_bmp_res(res->bmp_cursor_sel);
	destroy_bmp_res( res->bmp_point);
	
	destroy_bmp_res(res->bmp_108MHZ);
	destroy_bmp_res(res->bmp_ch);
	destroy_bmp_res(res->bmp_MHZ);
	destroy_bmp_res(res->bmp_preset);
	destroy_bmp_res(res->bmp_slash);
	destroy_bmp_res(res->bmp_stereo);
	destroy_bmp_res(res->bmp_mono);
	
	destroy_bmp_res(res->bmp_l[0]);
	destroy_bmp_res(res->bmp_l[1]);
	destroy_bmp_res(res->bmp_l[2]);
	destroy_bmp_res(res->bmp_l[3]);
	destroy_bmp_res(res->bmp_l[4]);
	destroy_bmp_res(res->bmp_l[5]);
	destroy_bmp_res(res->bmp_l[6]);
	destroy_bmp_res(res->bmp_l[7]);
	destroy_bmp_res(res->bmp_l[8]);
	destroy_bmp_res(res->bmp_l[9]);
	
	destroy_bmp_res(res->bmp_m[0]);
	destroy_bmp_res(res->bmp_m[1]);
	destroy_bmp_res(res->bmp_m[2]);
	destroy_bmp_res(res->bmp_m[3]);
	destroy_bmp_res(res->bmp_m[4]);
	destroy_bmp_res(res->bmp_m[5]);
	destroy_bmp_res(res->bmp_m[6]);
	destroy_bmp_res(res->bmp_m[7]);
	destroy_bmp_res(res->bmp_m[8]);
	destroy_bmp_res(res->bmp_m[9]);
	
	destroy_bmp_res(res->bmp_b[0]);
	destroy_bmp_res(res->bmp_b[1]);
	destroy_bmp_res(res->bmp_b[2]);
	destroy_bmp_res(res->bmp_b[3]);
	destroy_bmp_res(res->bmp_b[4]);
	destroy_bmp_res(res->bmp_b[5]);
	destroy_bmp_res(res->bmp_b[6]);
	destroy_bmp_res(res->bmp_b[7]);
	destroy_bmp_res(res->bmp_b[8]);
	destroy_bmp_res(res->bmp_b[9]);
	return EPDK_OK;
}

__s32 fm_auto_search_cb(FM_WND_T *fmplay_menu_attr)
{
    __msg("auto_seach@@@@@@@@  ");
    return EPDK_OK;
}

/************************************************************************/
// ��Ƶ������ǰƵ���α�
/************************************************************************/
__s32 draw_freq_scale(H_WIN hwnd, H_LYR layer, FM_UI_MAINWND *ui)
{
	char buf[8 * 2] = {0};
	int x;
	GUI_RECT rt;
	FM_CTRLDATA_T*   fm_ctrl;
	FM_FREQ_SCALE_RES_T *res;
	FM_MAINWND_T *wnd_para; 
	__u8 a = 0,b = 0,c = 0,d = 0;

		__u32 cur_freq ;
		__u32 channel_id ;
		__u32 channel_count ;
		__u32 audiomode ;	
	FM_GetWndPara(wnd_para, FM_MAINWND_T, hwnd);	

	res = &wnd_para->res.scale;
	fm_ctrl = wnd_para->fm_ctrl;	

//	GUI_LyrWinSel(layer);
	/*
	{//for debug
		__u32 cur_freq ;
		__u32 channel_id ;
		__u32 channel_count ;
		__u32 audiomode ;
	 
		dsk_radio_rcv_get_cur_freq(&cur_freq);
		channel_count = dsk_radio_rcv_get_search_result_vn();
		dsk_radio_get_audio_mode(&audiomode);
		__msg("draw:dsk_radio_rcv_get_cur_freq() = %d\n", cur_freq);
		__msg("draw:dsk_radio_rcv_get_search_result_vn() = %d\n", channel_count);
		__msg("draw:audiomode = %d \n",audiomode); //cky 			
	}
	*/
	//�����(����)
	//#if 0
    
    #if 1
    {
        __s32 i;
        
        GUI_SetColor(APP_COLOR_WHITE+0x0f);

        for(i = ui->scale.d0 ; i <= ui->scale.d1 ; i += 10)
        {
            GUI_DrawLine(i, ui->scale_bg.y, i, ui->scale_bg.y+12);
        }

        i = ui->scale.d1;
        GUI_DrawLine(i, ui->scale_bg.y, i, ui->scale_bg.y+12);
    }
    #else
    GUI_BMP_RES_Draw(res->bmp_scale, ui->scale_bg.x, ui->scale_bg.y);
    #endif

	//���α�	
	x = RANGE_MAPPING(ui->scale.d0, ui->scale.d1, 
		fm_ctrl->cur_min_freq, fm_ctrl->cur_max_freq, fm_ctrl->cur_freq);

	GUI_BMP_RES_Draw(res->bmp_cursor_sel, x, ui->scale_bg.y + ui->cursor_y);

   // #endif
		
	//����ť
	if (IsFmShowThumbBtn(fm_ctrl->thumb_show))
	{
		x -= (ui->btn_thumb.width >> 1);
		if (IsFmFocusThumbBtn(fm_ctrl->thumb_show))
		{			
			GUI_BMP_RES_Draw(res->bmp_btn_thumb[BTN_FOCUS], x, ui->scale_bg.y - ui->btn_thumb.height);			
		} 
		else
		{
			GUI_BMP_RES_Draw(res->bmp_btn_thumb[BTN_FOCUS], x, ui->scale_bg.y - ui->btn_thumb.height);	
		}
	}
//a:��λ��b:ʮλ��c:��λ��d:С�����һλ ��
//��ʾ���Ͻ�����
//��ǰƵ��:
	GUI_BMP_RES_Draw(res->bmp_ch, 35, 38);//��ʾ"ch"
	if ( fm_ctrl->channel_count > 0)
		{
			b =(fm_ctrl->channel_id +1) /10;
			c =(fm_ctrl->channel_id +1) % 10;	
		}
	else
		{
			b =(fm_ctrl->channel_id ) /10;
			c =(fm_ctrl->channel_id ) % 10;	
		}
		
	GUI_BMP_RES_Draw(res->bmp_l[b], 70, 38);//��ʾʮλ
	GUI_BMP_RES_Draw(res->bmp_l[c], 82, 38);//��ʾ��λ
	
//Ƶ������:	
	
	GUI_BMP_RES_Draw(res->bmp_slash, 90, 38);  //��ʾб��"/"
	b = fm_ctrl->channel_count /10;
	c = fm_ctrl->channel_count % 10;
	GUI_BMP_RES_Draw(res->bmp_l[b], 102, 38);//��ʾʮλ
	GUI_BMP_RES_Draw(res->bmp_l[c], 114, 38);//��ʾ��λ

//��ʾ���Ͻ�����
//��ǰƵ��:
	GUI_BMP_RES_Draw(res->bmp_preset, 325, 36);//��ʾ"preset"
	if ( fm_ctrl->channel_count > 0)
		{
			b =(fm_ctrl->channel_id +1) /10;
			c =(fm_ctrl->channel_id +1) % 10;	
		}
	else
		{
			b =(fm_ctrl->channel_id ) /10;
			c =(fm_ctrl->channel_id ) % 10;	
		}
	GUI_BMP_RES_Draw(res->bmp_m[b],405,38 );	//��ʾʮλ
	GUI_BMP_RES_Draw(res->bmp_m[c],420,38);	//��ʾ��λ
//��ʾ��������
//87000 ~ 108000KHZ�����87MHZ ~ 108MHZ
//87500 ~ 108000KHZ�����87MHZ ~ 108MHZ
//��ǰƵ��:
	a = (fm_ctrl->cur_freq)  / 100000;  //��ʾUI�ϵİ�λ
	b = (fm_ctrl->cur_freq) % 100000 /10000;
	c = (fm_ctrl->cur_freq) % 10000 /1000;
	d = (fm_ctrl->cur_freq) % 1000 /100;
	//__wrn("draw:fm_ctrl->cur_freq = %d\n",fm_ctrl->cur_freq);
	
	if(a>0)
	{		
		GUI_BMP_RES_Draw(res->bmp_b[a], 165,98);	// ��ʾ��λ
	}
		
	GUI_BMP_RES_Draw(res->bmp_b[b], 185,98);		//��ʾʮλ
	GUI_BMP_RES_Draw(res->bmp_b[c], 205,98);		//��ʾ��λ
	GUI_BMP_RES_Draw(res->bmp_point, 225,118);		//��ʾС����
	GUI_BMP_RES_Draw(res->bmp_b[d], 230,98);		//��ʾС�����һλ	
	GUI_BMP_RES_Draw(res->bmp_MHZ, 252,97);   		//��ʾ"MHZ"
	
//������Ƶ������
#if  (FM_SEARCH_CHN_MIN_FREQ == 87500)	
	GUI_BMP_RES_Draw(res->bmp_87_5MHZ, 13,172);		//��ʾ"87.5MHZ"	
#else if  (FM_SEARCH_CHN_MIN_FREQ == 87000)	
	GUI_BMP_RES_Draw(res->bmp_87MHZ, 13,172);		//��ʾ"87MHZ"	
#endif
	GUI_BMP_RES_Draw(res->bmp_108MHZ, 393,172);	//��ʾ"108MHZ"
	
//����ģʽ����	
	dsk_radio_get_audio_mode(&fm_ctrl->audio_mode);
	__wrn("fm_ctrl->audio_mode = %d\n",fm_ctrl->audio_mode);
	if(fm_ctrl->audio_mode == 1)
		{
			GUI_BMP_RES_Draw(res->bmp_mono,209,172);		//��ʾ"mono"
		}
	else if(fm_ctrl->audio_mode == 2)
		{
			GUI_BMP_RES_Draw(res->bmp_stereo,209,172);		//��ʾ"stereo"
		}

/*
	//��ʾ����
	GUI_SetFont(SWFFont);
	GUI_UC_SetEncodeUTF8();
	GUI_SetColor(ui->clr_txt_n);

	rt.x0 = ui->scale_bg.x;
	rt.x1 = rt.x0 + ui->scale_bg.width;
	rt.y0 = ui->scale_bg.y + ui->txt_y;
	rt.y1 = ui->scale_bg.y + ui->scale_bg.height;

	MAKE_STR_CHANNEL_FREQ(buf, fm_ctrl->cur_min_freq);
	GUI_DispStringInRect(STR_MIN_CHANNEL_FREQ, &rt, GUI_TA_LEFT);

	MAKE_STR_CHANNEL_FREQ(buf, fm_ctrl->cur_freq);
	GUI_DispStringInRect(buf, &rt, GUI_TA_CENTER);

	MAKE_STR_CHANNEL_FREQ(buf, fm_ctrl->cur_max_freq);
	GUI_DispStringInRect(STR_MAX_CHANNEL_FREQ, &rt, GUI_TA_RIGHT);
*/

	return EPDK_OK;
}

static __s32 scale_set_freq(H_WIN hwnd, __s16 x, __s16 y, __s32 action)
{
	__s16          y0, y1;
	FM_UI_MAINWND *ui;
	FM_CTRLDATA_T *fm_ctrl;
	FM_MAINWND_T *wnd_para;

	FM_GetWndPara(wnd_para, FM_MAINWND_T, hwnd);
	fm_ctrl = wnd_para->fm_ctrl;

    //__wrn("scale_set_freq...\n");
    
	ui = &get_fm_uipara()->main;
	y0 = ui->scale_bg.y - ui->btn_thumb.height;
	y1 = ui->scale_bg.y;
	if (1)//(ValueInRange(y0, y, y1))
	{
		__u32 old_freq;
		__s32 thumb_show;
		//__wrn("draw:fm_ctrl->cur_freq = %d\n",fm_ctrl->cur_freq);
		old_freq = fm_ctrl->cur_freq;
		thumb_show = fm_ctrl->thumb_show;

        __here__;
		if (action != GUI_MSG_TOUCH_DOWN)//downʱ������freq
		{
            __here__;
			if (x < ui->scale.d0)
			{
				x = ui->scale.d0;
			}
			else if (ui->scale.d1 < x)
			{
				x = ui->scale.d1;
			}
			fm_ctrl->cur_freq = RANGE_MAPPING(fm_ctrl->cur_min_freq, fm_ctrl->cur_max_freq,
				ui->scale.d0, ui->scale.d1, x);
			__msg("(x, y) -> calc cur_freq = %d (%d, %d)\n", 
				x, y, fm_ctrl->cur_freq, fm_ctrl->cur_min_freq, fm_ctrl->cur_max_freq);
			
		}

		switch (action)
		{
		case GUI_MSG_TOUCH_DOWN:
			{
				if (IsFmShowThumbBtn(fm_ctrl->thumb_show))
				{
					fm_ctrl->thumb_show = FM_SHOW_THUMB_F;
				}
			}
			break;
		case GUI_MSG_TOUCH_MOVE://moveʱ������Ƶ��
// 			{
// 				if (IsFmFocusThumbBtn(fm_ctrl->thumb_show))
// 				{
// 					dsk_radio_rcv_set_freq_play(fm_ctrl->cur_freq);
// 					dsk_radio_rcv_get_cur_freq(&fm_ctrl->cur_freq);
// 				}
// 			}
			break;
		case GUI_MSG_TOUCH_UP:
			{
                __wrn("scale bar GUI_MSG_TOUCH_UP...\n");
				if (IsFmFocusThumbBtn(fm_ctrl->thumb_show))
				{
                    __here__;
                    __wrn("fm_ctrl->cur_freq=%d\n", fm_ctrl->cur_freq);
					dsk_radio_rcv_set_freq_play(fm_ctrl->cur_freq);
                    __here__;
					dsk_radio_rcv_get_cur_freq(&fm_ctrl->cur_freq);
					fm_ctrl->thumb_show = FM_SHOW_THUMB_N;
				}
			}
			break;
		}

		if (old_freq != fm_ctrl->cur_freq || thumb_show != fm_ctrl->thumb_show)
		{
			UI_UpdateFMNow(hwnd);
		}

		return EPDK_OK;
	}

	return EPDK_FAIL;
}

//�����Ƿ�������ť,����TOUCH_UI_INVALID_ID, *pFlagΪ�е㵽x��λ��
static __s32 hit_test_scale_set_freq_btn(H_WIN hwnd, __s16 x, __s16 y, __s32 *pFlag)
{
	FM_CTRLDATA_T*   fm_ctrl;
	FM_MAINWND_T *wnd_para;

	FM_GetWndPara(wnd_para, FM_MAINWND_T, hwnd);
	if (wnd_para)
	{
		fm_ctrl = wnd_para->fm_ctrl;
		if (IsFmShowThumbBtn(fm_ctrl->thumb_show))
		{
			RECT btn_rt;
			FM_UI_MAINWND *ui;

			ui = &get_fm_uipara()->main;

			btn_rt.width  = ui->btn_thumb.width;
			btn_rt.height = ui->btn_thumb.height;
			btn_rt.x = RANGE_MAPPING(ui->scale.d0, ui->scale.d1, 
				fm_ctrl->cur_min_freq, fm_ctrl->cur_max_freq, fm_ctrl->cur_freq); //�е�

			if (pFlag)
			{
				*pFlag = x - btn_rt.x;
			}

			btn_rt.x -= (ui->btn_thumb.width >> 1);
			btn_rt.y = ui->scale_bg.y - ui->btn_thumb.height;

			if (XYInRect(x, y, btn_rt))
			{
				return 0;
			}
		}
	}	

	return TOUCH_UI_INVALID_ID;
}


/************************************************************************/
// ΢��fm����Ƶ��
// step -- Ƶ��λ��,
// return value -- 1:����Ƶ�ʸı�, 0:����Ƶ�ʲ���
/************************************************************************/
__s32 fm_freq_inching(FM_CTRLDATA_T *fm_ctrl, __s32 step)
{
	__u32 cur_freq;

	ASSERT(fm_ctrl);
	if (fm_ctrl->search_mode == SRHMODE_AUTO || fm_ctrl->search_mode == SRHMODE_MANUAL)
	{
		return 0;
	}

	dsk_radio_rcv_get_cur_freq(&cur_freq);
	
	cur_freq = ((__s32)cur_freq + step);

	if (cur_freq < fm_ctrl->cur_min_freq)
	{
		cur_freq = fm_ctrl->cur_min_freq;
	} 
	else if (fm_ctrl->cur_max_freq < cur_freq)
	{
		cur_freq = fm_ctrl->cur_max_freq;
	}
	//__wrn("draw:fm_ctrl->cur_freq = %d\n",fm_ctrl->cur_freq);
	if (cur_freq != fm_ctrl->cur_freq)
	{
		fm_ctrl->cur_freq = cur_freq;
		dsk_radio_rcv_set_freq_play(fm_ctrl->cur_freq);
		dsk_radio_rcv_get_cur_freq(&fm_ctrl->cur_freq); //for safe
		return 1;
	} 
	else
	{
		return 0;
	}
}

/************************************************************************/
// ��������
/************************************************************************/
__s32 on_freq_scale_touch(H_WIN hwnd, __s16 x, __s16 y, __s32 touch_action)
{
 	static __s32 last_touch_action = GUI_MSG_TOUCH_DOWN;
 	static __pos_t last_pos = {0, 0};

#define EmptyTouchAction()      {last_touch_action = GUI_MSG_TOUCH_DOWN; last_pos.x = 0; last_pos.y = 0;}
#define RecordTouchPos(_x, _y)  {last_pos.x = _x; last_pos.y = _y;}
#define RecordTouch(_a, _x, _y) {last_touch_action = _a; RecordTouchPos(_x, _y);}

	static __s32 old_id = TOUCH_UI_INVALID_ID;
	__s32 ret = 0;
	__s32 id;
	__s32 flag;
	FM_UI_MAINWND *ui;
	FM_CTRLDATA_T *fm_ctrl;
	FM_MAINWND_T  *wnd_para;

	FM_GetWndPara(wnd_para, FM_MAINWND_T, hwnd);
	fm_ctrl = wnd_para->fm_ctrl;

	ui = &get_fm_uipara()->main;

	id = hit_test_scale_set_freq_btn(hwnd, x, y, &flag);
	__msg("hit_test_scale_set_freq_btn = %d, focus = %d\n", id, fm_ctrl->thumb_show);

    //__wrn("touch_action=%d\n", touch_action);
    //if(GUI_MSG_TOUCH_UP == touch_action
    //    ||GUI_MSG_TOUCH_OVERUP == touch_action)
    //{
    //    __here__;
    //    if(GUI_WinGetCaptureWin() == hwnd)
     //   {
     //       __here__;
    //        GUI_WinReleaseCapture();
     //   }
   // }

	//��Ƶ��Χ��
	if (ValueInRange(ui->scale_bg.y - ui->btn_thumb.height-30, y, ui->scale_bg.y + ui->scale_bg.height+30))	
	{
		ret = 1;

		switch (touch_action)
		{
		case GUI_MSG_TOUCH_DOWN:
		case GUI_MSG_TOUCH_LONGDOWN:
			{
                //GUI_WinSetCaptureWin(hwnd);
               // __wrn("on_freq_scale_touch, GUI_MSG_TOUCH_DOWN\n");
	
				if (TouchValidID(id))
				{
					scale_set_freq(hwnd, x, y, GUI_MSG_TOUCH_DOWN);
					
				}
			}
			break;
		case GUI_MSG_TOUCH_MOVE:
			{
				if (IsFmFocusThumbBtn(fm_ctrl->thumb_show))
				{
					scale_set_freq(hwnd, x, y, GUI_MSG_TOUCH_MOVE);
					
				}
			}
			break;
		case GUI_MSG_TOUCH_UP:
			{
                //__wrn("on_freq_scale_touch, GUI_MSG_TOUCH_UP\n");
                

                __here__;
				if (IsFmFocusThumbBtn(fm_ctrl->thumb_show))
				{
                    __here__;
					if ((last_touch_action == GUI_MSG_TOUCH_DOWN || last_touch_action == GUI_MSG_TOUCH_LONGDOWN)
						&& (id == old_id && TouchValidID(id)))
					{//click btn,΢��
					    __here__;
						if (fm_freq_inching(fm_ctrl, 
							(flag < 0) ? -FM_FREQ_INCHING_VALUE : FM_FREQ_INCHING_VALUE))
						{
							fm_ctrl->thumb_show = FM_SHOW_THUMB_N;
							UI_UpdateFMNow(hwnd);
						}
					}
					else if (scale_set_freq(hwnd, x, y, GUI_MSG_TOUCH_UP) != EPDK_OK)
					{
                        __here__;
						fm_ctrl->thumb_show = FM_SHOW_THUMB_N;
						UI_UpdateFMNow(hwnd);
					}
				}
			}
			break;
		}
	}
	else if (IsFmFocusThumbBtn(fm_ctrl->thumb_show))
	{
		ret = 1;

		if (GUI_MSG_TOUCH_UP == touch_action)
		{
			fm_ctrl->thumb_show = FM_SHOW_THUMB_N;
			UI_UpdateFMNow(hwnd);
		}
	}
	
	old_id = id;
	
	RecordTouch(touch_action, x, y)

	return ret;
}

//////////////////////////////////////////////////////////////////////////
//end of file

