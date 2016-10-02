/*
**************************************************************************************************************
*											        ePDK
*						            the Easy Portable/Player Develop Kits
*									            desktop system 
*
*						        	 (c) Copyright 2007-2010, ANDY, China
*											  All Rights Reserved
*
* File    	: headbar_com.c
* By      	: Andy.zhang
* Func		: app bar view
* Version	: v1.0
* ============================================================================================================
* 2009-9-24 15:12:06  andy.zhang  create this file, implements the fundemental interface;
**************************************************************************************************************
*/
#include "mod_init_i.h"
#include "headbar_uipara.h"
#include "headbar_com.h"
#include "..\\live_init.h"

/**********************************************************************************************************************/
#define HEADBAR_COM_ID		1002

#define MAX_VOL_LEVEL			6
#define MAX_APP_CNT			6
#define HEADBAR_TIMER_ID		1000
#define HEADBAR_TIMER_CNT		70
#define MESS_TIMER_CNT			2

#define HEADBAR_STR_COLOR		0xffEFEFEF

#define ValueBetweenRangeEx(_min, _v1, _v2, _max) (((_min) <= (_v1)) && ((_v1) < (_v2)) && ((_v2) <= (_max)))
#define DATA_OF_ADRR(t, p, x, y, w) (*( (t*)(p)+(y)*(w)+(x) ))

/***********************************************************************************************************************/

static __s32 bat_cnt = 0;

/***********************************************************************************************************************/

typedef struct 
{
	__s32 		vol_level;			// 电压级别，供电池显示
	__s32   		is_full;             //电池是否充满
	__bool      	charge_sta;         //充电状态
	__date_t 		date;				// 日期
	
	__s32		volume;				// 标识当前音量大小
	__s32		bright;				// 标识当前亮度	
	
}headbar_data_t;

typedef struct 
{
	H_WIN	hbar;						// bar窗口	

	RECT	frm_rect;				// 音量坐标
	RECT	volume_rect;				// 音量坐标
	GUI_RECT	volume_data_rect;			// 音量数据坐标
	RECT 	vol_rect;					// 电池坐标	
	GUI_RECT	strtf_rect;
	GUI_RECT	strusb_rect;
	GUI_RECT	strmess_rect;

	HTHEME	vol_htheme[MAX_VOL_LEVEL];
	HTHEME	vol_htheme_charge[MAX_VOL_LEVEL];
	HTHEME	volume_htheme;	
	HTHEME	volumemute_htheme;	

	__u32 	vol_id[MAX_VOL_LEVEL];		// 电池资源	
	__u32 	vol_id_charge[MAX_VOL_LEVEL];// 充电电池资源	
	void*	vol_bmp[MAX_VOL_LEVEL];		// 电池图片
	void*	vol_bmp_charge[MAX_VOL_LEVEL];//充电电池图片

	RECT 		bright_rect;				// 亮度坐标		
	GUI_RECT	bright_data_rect;			// 亮度数据坐标
	HTHEME		bright_htheme;
	void*		bright_bmp;					// 背光强度图片
	char			bright_buf[16];				// 亮度

	HTHEME		hbarbg_htheme;
	void*		hbarbg_bmp;					

	void*	volume_bmp;
	void*	volumemute_bmp;
	char 	volume_buf[16];

	__u32 	cur_disk_id;
	__u32 	mess_timecnt;
	__bool	disk_exist_mediafile;	//mainmenu enter app
	char		strtf_buf[16];
	char		strusb_buf[16];
	char		strmess_buf[32];				// 应用程序title or tf/usb
	char		strmedia_buf[32];
	char		strinsert_buf[32];
	char		strinsertmic_buf[32];

	headbar_data_t 	data;				// 状态数据
	GUI_FONT 		*font;	
}headbar_para_t;

typedef struct
{	
	headbar_para_t	para;	
	__u32			res_init; //index 1-init,0-deinit
}headbar_ctl_t;
/**********************************************************************************************************************/
static void  init_headbar_para(__gui_msg_t *p_msg, headbar_para_t 	*para,__u32 initflag);
static void  deinit_headbar_para(headbar_para_t *para);

static __s32 power_level_2_display(power_level_e level);
static __s32 bright_level_2_display(__lion_bright_t	bright);


//////////////////////////////////////////////////////////////////////////

static __s32 copy_rect(__u32* pDst, __s32 dst_x, __s32 dst_y, __s32 dst_line_w, __u32* pSrc,
                __s32 src_x, __s32 src_y, __s32 src_line_w, __s32 w, __s32 h)
{
    __s32 i, j;

    for(i = 0 ; i < w ; i++)
    {
        for(j = 0 ; j < h ; j++)
        {
            DATA_OF_ADRR(__u32, pDst, dst_x+i, dst_y+j, dst_line_w) = DATA_OF_ADRR(__u32, pSrc, src_x+i, src_y+j, src_line_w);
        }
    }
    
    return EPDK_OK;
}

/************************************************************************/
// pFileData --- 资源函数获得的数据(也是32位bmp文件数据)
// x，y      --- 目标坐标
// x0, y0, x1, y1 --- 源区域(即bmp图片某一区域)
/************************************************************************/
static __s32 APP_BMP_Draw(H_LYR hlyr, const void * pFileData, int x, int y, int x0, int y0, int x1, int y1)
{
	__s32 Ret = EPDK_OK;
	__u32 *pSrc;
	int bmp_w, bmp_h;
	int w, h;
    GUI_RECT rt;    
    FB fb;
    __u32* pfb_addr;
    __s32 i, j;

    if(!hlyr)
    {
        __err("invalid para...\n");
        return EPDK_FAIL;
    }
    
    GUI_LyrWinGetFB(hlyr, &fb);
    
	//检查区域范围
	bmp_w = GUI_BMP_GetXSize(pFileData);
	if ( ! ValueBetweenRangeEx(0, x0, x1, bmp_w))
	{
		return EPDK_FAIL;
	}

	bmp_h = GUI_BMP_GetYSize(pFileData);
	if ( ! ValueBetweenRangeEx(0, y0, y1, bmp_h))
	{
		return EPDK_FAIL;
	}	

    if(x1-x0+x >= fb.size.width
        || y1-y0+y >= fb.size.height)
    {
        return EPDK_FAIL;
    }

    pSrc = (__u32*)((char*)pFileData + 54);
    pfb_addr = (__u32*)(fb.addr[0]);       

    copy_rect(pfb_addr, x, y, fb.size.width, pSrc, x0, y0, bmp_w, x1-x0+1, y1-y0+1);       

	return Ret;
}

static void init_headbar_data(headbar_para_t *para)
{
	__s32 is_full;
	power_level_e	 	level;
	__lion_bright_t	bright;
	
	/////////
#if (AW_SDK_PMU == 0)	 //不带PMU
	dsk_power_get_voltage_level(&level,&is_full);		
#else //带PMU
	dsk_power_get_battery_level(&level,&is_full);
#endif

	para->data.vol_level = power_level_2_display(level);	//电池级别	
	para->data.is_full = is_full;
	para->data.charge_sta = dsk_get_charge_state();         //充电状态
	////
	para->data.volume = dsk_volume_get();			    	//音量	
	eLIBs_memset(para->volume_buf, 0, 16);
	eLIBs_int2str_dec(para->data.volume, para->volume_buf);
	//////
	bright = dsk_display_get_lcd_bright();		
	para->data.bright = bright_level_2_display(bright);		//背光亮度	
	eLIBs_memset(para->bright_buf, 0, 16);
	eLIBs_int2str_dec(para->data.bright, para->bright_buf);

	//CAI++ FOR 初始化  磁盘部分的变量	
	 //需要添加  for 无对应媒体文件
 	dsk_langres_get_menu_text(STRING_NO_DISK, para->strmess_buf, 32); 
	dsk_langres_get_menu_text(STRING_NO_MEDIAFILE, para->strmedia_buf, 32); 
	dsk_langres_get_menu_text(STRING_INSERT_DISK, para->strinsert_buf, 32); 
	dsk_langres_get_menu_text(STRING_MICINSERT, para->strinsertmic_buf, 32); 

	if(RAT_UNKNOWN == para->cur_disk_id )
	{
		para->disk_exist_mediafile = 0;
		dsk_langres_get_menu_text(STRING_NO_DISK, para->strmess_buf, 32);
	}
	else
	{
		
		if (para->cur_disk_id == RAT_TF)
		{
			dsk_langres_get_menu_text(STRING_TF_CARD, para->strtf_buf, 16); 
		}
		if(para->cur_disk_id == RAT_USB)
		{
			dsk_langres_get_menu_text(STRING_USB_DISK, para->strusb_buf, 16); 
		}
		if(para->cur_disk_id == RAT_UNKNOWN)
		{
			dsk_langres_get_menu_text(STRING_NO_DISK, para->strmess_buf, 32);
		}
		para->disk_exist_mediafile = 1;  //刚进入时初始化为1
	}

	return;
}

static void init_headbar_para(__gui_msg_t *p_msg, headbar_para_t *para,__u32 initflag)
{
	int 				i;	
	
	para->hbar   =  p_msg->h_deswin;
	///////////
	if(1 == initflag)
	{
		para->frm_rect.x 			= headbar_uipara.hbar_frmwin.x;
		para->frm_rect.y 			= headbar_uipara.hbar_frmwin.y;
		para->frm_rect.width 		= headbar_uipara.hbar_frmwin.w;
		para->frm_rect.height		= headbar_uipara.hbar_frmwin.h;
		
		para->volume_rect.x 		= headbar_uipara.volume_icon.x;
		para->volume_rect.y 		= headbar_uipara.volume_icon.y;
		para->volume_rect.width 	= headbar_uipara.volume_icon.w;
		para->volume_rect.height	= headbar_uipara.volume_icon.h;
		
		para->volume_data_rect.x0 	= headbar_uipara.volume_txt.x;
		para->volume_data_rect.y0 	= headbar_uipara.volume_txt.y;
		para->volume_data_rect.x1		= para->volume_data_rect.x0+headbar_uipara.volume_txt.w-1;
		para->volume_data_rect.y1 	= para->volume_data_rect.y0+headbar_uipara.volume_txt.h-1;
		
		para->bright_rect.x 			= headbar_uipara.bright_icon.x;
		para->bright_rect.y 			= headbar_uipara.bright_icon.y;
		para->bright_rect.width  		= headbar_uipara.bright_icon.w;
		para->bright_rect.height 		= headbar_uipara.bright_icon.h;
		
		para->bright_data_rect.x0 		= headbar_uipara.bright_txt.x;
		para->bright_data_rect.y0 		= headbar_uipara.bright_txt.y;
		para->bright_data_rect.x1  		= para->bright_data_rect.x0+headbar_uipara.bright_txt.w-1;
		para->bright_data_rect.y1 		= para->bright_data_rect.y0+headbar_uipara.bright_txt.h-1;
		
		para->vol_rect.x 				= headbar_uipara.battery.x;
		para->vol_rect.y 				= headbar_uipara.battery.y;
		para->vol_rect.width  	    		= headbar_uipara.battery.w;
		para->vol_rect.height    		= headbar_uipara.battery.h;

		para->strtf_rect.x0			= headbar_uipara.strtf.x;
		para->strtf_rect.y0			= headbar_uipara.strtf.y;
		para->strtf_rect.x1			= para->strtf_rect.x0+headbar_uipara.strtf.w-1;
		para->strtf_rect.y1			= para->strtf_rect.y0+headbar_uipara.strtf.h-1;

		para->strusb_rect.x0			= headbar_uipara.strusb.x;
		para->strusb_rect.y0			= headbar_uipara.strusb.y;
		para->strusb_rect.x1			= para->strusb_rect.x0+headbar_uipara.strusb.w-1;
		para->strusb_rect.y1			= para->strusb_rect.y0+headbar_uipara.strusb.h-1;

		para->strmess_rect.x0			= headbar_uipara.strmess.x;
		para->strmess_rect.y0			= headbar_uipara.strmess.y;
		para->strmess_rect.x1			= para->strmess_rect.x0+headbar_uipara.strmess.w-1;
		para->strmess_rect.y1			= para->strmess_rect.y0+headbar_uipara.strmess.h-1;

		para->vol_id[0] = headbar_uipara.res_id.vol_id0;
		para->vol_id[1] = headbar_uipara.res_id.vol_id1;
		para->vol_id[2] = headbar_uipara.res_id.vol_id2;
		para->vol_id[3] = headbar_uipara.res_id.vol_id3;
		para->vol_id[4] = headbar_uipara.res_id.vol_id4;
		para->vol_id[5] = headbar_uipara.res_id.vol_id5;

	    	para->vol_id_charge[0] = headbar_uipara.res_id.charge_vol_id0;
		para->vol_id_charge[1] = headbar_uipara.res_id.charge_vol_id1;
		para->vol_id_charge[2] = headbar_uipara.res_id.charge_vol_id2;
		para->vol_id_charge[3] = headbar_uipara.res_id.charge_vol_id3;
		para->vol_id_charge[4] = headbar_uipara.res_id.charge_vol_id4;
		para->vol_id_charge[5] = headbar_uipara.res_id.charge_vol_id5;

	}
	
	for( i=0; i< MAX_VOL_LEVEL; i++ )
	{
		para->vol_htheme[i] 	= dsk_theme_open(para->vol_id[i]);
		para->vol_bmp[i]		= dsk_theme_hdl2buf(para->vol_htheme[i]);
		
		para->vol_htheme_charge[i]	= dsk_theme_open(para->vol_id_charge[i]);
		para->vol_bmp_charge[i]		= dsk_theme_hdl2buf(para->vol_htheme_charge[i]);
	}	
	////////
	para->volume_htheme		= dsk_theme_open(headbar_uipara.res_id.volume_id);
	para->volume_bmp		= dsk_theme_hdl2buf(para->volume_htheme);

	para->volumemute_htheme		= dsk_theme_open(headbar_uipara.res_id.volumemute_id);
	para->volumemute_bmp		= dsk_theme_hdl2buf(para->volumemute_htheme);

	para->bright_htheme		= dsk_theme_open(headbar_uipara.res_id.bright_id);
	para->bright_bmp			= dsk_theme_hdl2buf(para->bright_htheme);

	para->hbarbg_htheme		= dsk_theme_open(headbar_uipara.res_id.hbarbg_id);
	para->hbarbg_bmp			= dsk_theme_hdl2buf(para->hbarbg_htheme);

	if(1 == initflag)
	{
		para->cur_disk_id = RAT_UNKNOWN;
		para->mess_timecnt = 0;
		dsk_langres_get_menu_text(STRING_NO_DISK, para->strmess_buf, 32); 
		dsk_langres_get_menu_text(STRING_NO_MEDIAFILE, para->strmedia_buf, 32); 
		dsk_langres_get_menu_text(STRING_INSERT_DISK, para->strinsert_buf, 32); 
		dsk_langres_get_menu_text(STRING_MICINSERT, para->strinsertmic_buf, 32); 

		para->font				= headbar_uipara.font;
	}
	
	return;
}

static void deinit_headbar_para(headbar_para_t *para)
{
	int i;
	
	for( i=0; i<MAX_VOL_LEVEL; i++)
	{	
		dsk_theme_close(para->vol_htheme[i]);
		dsk_theme_close(para->vol_htheme_charge[i]);
	}
	dsk_theme_close(para->volume_htheme);
	dsk_theme_close(para->volumemute_htheme);
	dsk_theme_close(para->bright_htheme);
	dsk_theme_close(para->hbarbg_htheme);

	return;
}

static __s32 bright_level_2_display(__lion_bright_t	bright)
{
	int ret = 0;
	
	switch( bright )
	{
		case LION_BRIGHT_LEVEL1:
			ret = 1;
			break;
		case LION_BRIGHT_LEVEL2:
			ret = 2;
			break;
		case LION_BRIGHT_LEVEL3:
			ret = 3;
			break;
		case LION_BRIGHT_LEVEL4:
			ret = 4;
			break;
		case LION_BRIGHT_LEVEL5:
			ret = 5;
			break;
		case LION_BRIGHT_LEVEL6:
			ret = 6;
			break;
		case LION_BRIGHT_LEVEL7:
			ret = 7;
			break;
		case LION_BRIGHT_LEVEL8:
			ret = 8;
			break;
		case LION_BRIGHT_LEVEL9:
			ret = 9;
			break;
		case LION_BRIGHT_LEVEL10:
			ret = 10;
			break;
		case LION_BRIGHT_LEVEL11:
			ret = 11;
			break;
		case LION_BRIGHT_LEVEL12:
			ret = 12;
			break;
		case LION_BRIGHT_LEVEL13:
			ret = 13;
			break;
		case LION_BRIGHT_LEVEL14:
			ret = 14;
			break;
		case LION_BRIGHT_LEVEL15:
			ret = 15;
			break;				
		case LION_BRIGHT_LEVEL16:
			ret = 16;
			break;
			
		case LION_BRIGHT_:
			ret = 0;
			break;
	}
	
	return ret;
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

static __s32 headbar_on_paint(__gui_msg_t *msg, headbar_paint_e mode)
{
	H_LYR		   	layer;
	GUI_FONT*		font = NULL;
	headbar_ctl_t*	p_ctrl = NULL;	
	headbar_para_t*	headbar_para = NULL;

	p_ctrl = (headbar_ctl_t *)GUI_WinGetAddData(msg->h_deswin);
	headbar_para = &p_ctrl->para;
	font = headbar_para->font;	
	layer = (H_LYR)GUI_WinGetLyrWin(msg->h_deswin);
	
	if( GUI_LYRWIN_STA_ON != GUI_LyrWinGetSta(layer) )
	{
		return EPDK_OK;
	}
	
	//__msg("----%d\n",mode);
	GUI_LyrWinSel(layer);
	switch(mode)
	{
		case HB_PAINT_INIT:
		{			
			//draw bg
			GUI_SetDrawMode(GUI_DRAWMODE_NORMAL);
			GUI_ARGB_Draw(headbar_para->hbarbg_bmp, 0, 0);
			//////////////

			/* 画电池图标 */
			if(EPDK_TRUE == headbar_para->data.charge_sta && (EPDK_TRUE != headbar_para->data.is_full))
			{	
				GUI_BMP_Draw(headbar_para->vol_bmp_charge[headbar_para->data.vol_level], headbar_para->vol_rect.x, headbar_para->vol_rect.y);	
			}
			else
			{
				if(headbar_para->data.vol_level == 5)
				{
					GUI_BMP_Draw(dsk_theme_hdl2buf(dsk_theme_open(ID_INIT_ENERGY_5_BMP)), headbar_para->vol_rect.x, headbar_para->vol_rect.y);	
				}
				else
					GUI_BMP_Draw(headbar_para->vol_bmp[headbar_para->data.vol_level], headbar_para->vol_rect.x, headbar_para->vol_rect.y);	
			}
             
			//bright&volume & menu
			if(dsk_volume_is_on())
				GUI_BMP_Draw_Trans(headbar_para->volume_bmp, headbar_para->volume_rect.x, headbar_para->volume_rect.y,0,0);
			else
				GUI_BMP_Draw_Trans(headbar_para->volumemute_bmp, headbar_para->volume_rect.x, headbar_para->volume_rect.y,0,0);
				
			GUI_OpenAlphaBlend();		
			GUI_SetFont(font);
			GUI_SetDrawMode(GUI_DRAWMODE_TRANS);
			GUI_SetColor(HEADBAR_STR_COLOR);	
			GUI_UC_SetEncodeUTF8();

			GUI_DispStringInRect(headbar_para->volume_buf, &headbar_para->volume_data_rect, GUI_TA_VCENTER|GUI_TA_HCENTER);
			////////sd&usb
			if(headbar_para->cur_disk_id == RAT_UNKNOWN)
			{
				;
			}
			else
			{
				if(headbar_para->cur_disk_id == RAT_TF)
				{
					GUI_SetColor(HEADBAR_STR_COLOR);
					//GUI_DispStringInRect(headbar_para->strtf_buf, &headbar_para->strtf_rect, GUI_TA_VCENTER|GUI_TA_HCENTER);
					GUI_DispStringInRect("TF", &headbar_para->strtf_rect, GUI_TA_VCENTER|GUI_TA_HCENTER);

					if(0 != headbar_para->strusb_buf[0])
					{
						GUI_SetColor(GUI_GRAY);
						//GUI_DispStringInRect(headbar_para->strusb_buf, &headbar_para->strusb_rect, GUI_TA_VCENTER|GUI_TA_HCENTER);
						GUI_DispStringInRect("USB", &headbar_para->strusb_rect, GUI_TA_VCENTER|GUI_TA_HCENTER);
					}

				}

				if(headbar_para->cur_disk_id == RAT_USB)
				{
					GUI_SetColor(HEADBAR_STR_COLOR);
					//GUI_DispStringInRect(headbar_para->strusb_buf, &headbar_para->strusb_rect, GUI_TA_VCENTER|GUI_TA_HCENTER);
					GUI_DispStringInRect("USB", &headbar_para->strusb_rect, GUI_TA_VCENTER|GUI_TA_HCENTER);

					if(0 != headbar_para->strtf_buf[0])
					{
						GUI_SetColor(GUI_GRAY);
						//GUI_DispStringInRect(headbar_para->strtf_buf, &headbar_para->strtf_rect, GUI_TA_VCENTER|GUI_TA_HCENTER);
						GUI_DispStringInRect("TF", &headbar_para->strtf_rect, GUI_TA_VCENTER|GUI_TA_HCENTER);
					}

				}
			}

			GUI_LyrWinFlushFB(layer);
			GUI_CloseAlphaBlend();
			
			return EPDK_OK;
		}
		case HB_PAINT_TIME:
		{
			return EPDK_OK;
		}
		
		case HB_PAINT_VOL:
		{
			__here__;

			if((EPDK_TRUE == dsk_get_charge_state()) && (EPDK_TRUE != headbar_para->data.is_full))
			{
				bat_cnt ++;
				if(bat_cnt > DSK_POWER_LEVEL_5)
				{
					bat_cnt = headbar_para->data.vol_level - 1;
				}
			
				__msg(" ##  bat_cnt = %d, vol_level = %d  ##\n", bat_cnt, headbar_para->data.vol_level);
				GUI_BMP_Draw(headbar_para->vol_bmp_charge[bat_cnt], headbar_para->vol_rect.x, headbar_para->vol_rect.y);
			}
			else
			{
				GUI_BMP_Draw(headbar_para->vol_bmp[headbar_para->data.vol_level], headbar_para->vol_rect.x, headbar_para->vol_rect.y);	
			}
			GUI_LyrWinFlushFB(layer);
			
			return EPDK_OK;	
		}
		
		
		case HB_PAINT_VOLUME:
		{

			GUI_SetDrawMode(GUI_DRAWMODE_NORMAL);
			APP_BMP_Draw(layer, headbar_para->hbarbg_bmp, 
				headbar_para->volume_rect.x, headbar_para->volume_rect.y, 
				headbar_para->volume_rect.x, headbar_para->volume_rect.y,
				headbar_para->volume_data_rect.x1,headbar_para->volume_data_rect.y1);

			//if(0 == headbar_para->data.volume)
			if(dsk_volume_is_on())
				GUI_BMP_Draw_Trans(headbar_para->volume_bmp, headbar_para->volume_rect.x, headbar_para->volume_rect.y,0,0);
			else
				GUI_BMP_Draw_Trans(headbar_para->volumemute_bmp, headbar_para->volume_rect.x, headbar_para->volume_rect.y,0,0);

			GUI_OpenAlphaBlend();		
			GUI_SetFont(font);
			GUI_SetDrawMode(GUI_DRAWMODE_TRANS);
			GUI_SetColor(HEADBAR_STR_COLOR);
			GUI_UC_SetEncodeUTF8();
			GUI_DispStringInRect(headbar_para->volume_buf ,&headbar_para->volume_data_rect, GUI_TA_VCENTER|GUI_TA_HCENTER);
			GUI_LyrWinFlushFB(layer);
			GUI_CloseAlphaBlend();
			
			return EPDK_OK;
		}
		
		case HB_PAINT_DISK:
		{
			GUI_SetDrawMode(GUI_DRAWMODE_NORMAL);
			APP_BMP_Draw(layer, headbar_para->hbarbg_bmp, 
				headbar_para->strmess_rect.x0, headbar_para->strmess_rect.y0, 
				headbar_para->strmess_rect.x0, headbar_para->strmess_rect.y0,
				headbar_para->strmess_rect.x1,headbar_para->strmess_rect.y1);

			GUI_OpenAlphaBlend();		
			GUI_SetFont(font);
			GUI_SetDrawMode(GUI_DRAWMODE_TRANS);
			GUI_UC_SetEncodeUTF8();
			////////sd&usb
			__here__;
			if(headbar_para->cur_disk_id == RAT_UNKNOWN)
			{
				//GUI_SetColor(HEADBAR_STR_COLOR);
				//GUI_DispStringInRect(headbar_para->strmess_buf, &headbar_para->strmess_rect, GUI_TA_VCENTER|GUI_TA_HCENTER);
			}
			else
			{
				if(headbar_para->cur_disk_id == RAT_TF)
				{
					if(headbar_para->disk_exist_mediafile)
					{
						GUI_SetColor(HEADBAR_STR_COLOR);
						GUI_DispStringInRect("TF", &headbar_para->strtf_rect, GUI_TA_VCENTER|GUI_TA_HCENTER);

						if(0 != headbar_para->strusb_buf[0])
						{
							GUI_SetColor(GUI_GRAY);
							GUI_DispStringInRect("USB", &headbar_para->strusb_rect, GUI_TA_VCENTER|GUI_TA_HCENTER);
						}

					}
					else
					{
						//GUI_SetColor(HEADBAR_STR_COLOR);
						//GUI_DispStringInRect(headbar_para->strmess_buf, &headbar_para->strmess_rect, GUI_TA_VCENTER|GUI_TA_HCENTER);						
					}
				}

				if(headbar_para->cur_disk_id == RAT_USB)
				{
					if(headbar_para->disk_exist_mediafile)
					{
						GUI_SetColor(HEADBAR_STR_COLOR);
						GUI_DispStringInRect("USB", &headbar_para->strusb_rect, GUI_TA_VCENTER|GUI_TA_HCENTER);

						if(0 != headbar_para->strtf_buf[0])
						{
							GUI_SetColor(GUI_GRAY);
							GUI_DispStringInRect("TF", &headbar_para->strtf_rect, GUI_TA_VCENTER|GUI_TA_HCENTER);
						}

					}
					else
					{
						//GUI_SetColor(HEADBAR_STR_COLOR);
						//GUI_DispStringInRect(headbar_para->strmess_buf, &headbar_para->strmess_rect, GUI_TA_VCENTER|GUI_TA_HCENTER);							
					}
				}
			}

			GUI_LyrWinFlushFB(layer);
			GUI_CloseAlphaBlend();
			
			return EPDK_OK;
		}
		default:
			break;
	}
	
	return EPDK_OK;	
}

static __s32 cb_headbar_framewin(__gui_msg_t *p_msg)
{	


	switch( p_msg->id )
	{
		case GUI_MSG_CREATE:
		{			
			headbar_ctl_t *p_ctrl;
			__here__;
			p_ctrl = (headbar_ctl_t *)esMEMS_Malloc(0, sizeof(headbar_ctl_t));
			if( !p_ctrl )
			{
				__err("headbar_para_t malloc error \n");
			} 
			eLIBs_memset(p_ctrl, 0, sizeof(headbar_ctl_t));	
			GUI_WinSetAddData(p_msg->h_deswin, (__u32)p_ctrl);	

			///get positon & bmp 
			if(0 == p_ctrl->res_init)
			{
				init_headbar_para(p_msg, &(p_ctrl->para),1);	
				p_ctrl->res_init = 1;
			}
			
			GUI_SetTimer(p_msg->h_deswin, HEADBAR_TIMER_ID, HEADBAR_TIMER_CNT, NULL);
		}
			return EPDK_OK;
		
		case GUI_MSG_DESTROY:
		{
			headbar_ctl_t *p_ctrl = NULL;	
			
			p_ctrl = (headbar_ctl_t *)GUI_WinGetAddData(p_msg->h_deswin);
			if( GUI_IsTimerInstalled(p_msg->h_deswin, HEADBAR_TIMER_ID))		
				GUI_KillTimer(p_msg->h_deswin, HEADBAR_TIMER_ID);

			if(1 == p_ctrl->res_init)
			{
				deinit_headbar_para( &(p_ctrl->para) );	
				p_ctrl->res_init = 0;
			}
			esMEMS_Mfree(0, p_ctrl);	
					
			return EPDK_OK;
		}
		
		case GUI_MSG_CLOSE:
		{
			GUI_FrmWinDelete(p_msg->h_deswin);
			
			return EPDK_OK;
		}

		case GUI_MSG_KEY:
		{			
			return EPDK_OK;
		}

		case GUI_MSG_TIMER:
		{
			if( p_msg->dwAddData1 == HEADBAR_TIMER_ID )
			{
				__s32 is_full = 0;
				H_LYR 		lyr = NULL;
				__lion_bright_t	bright;
				power_level_e 	level;
				headbar_ctl_t*	p_ctrl = NULL;
				headbar_data_t	headbar_data;

				lyr = GUI_WinGetLyrWin(p_msg->h_deswin);
				p_ctrl = (headbar_ctl_t *)GUI_WinGetAddData(p_msg->h_deswin);

				if(GUI_LYRWIN_STA_ON == GUI_LyrWinGetSta(lyr))
				{
					headbar_data.volume   	= dsk_volume_get();
#if (AW_SDK_PMU == 0)	 //不带PMU
					dsk_power_get_voltage_level(&level,&is_full);		
#else //带PMU
					dsk_power_get_battery_level(&level,&is_full);
#endif
					headbar_data.vol_level = power_level_2_display(level);
					headbar_data.is_full = is_full;
					headbar_data.charge_sta = dsk_get_charge_state();
			        	bright = dsk_display_get_lcd_bright();	
			        	headbar_data.bright = bright_level_2_display(bright);

					//__msg("----------%d,%d\n",headbar_data.volume,p_ctrl->para.data.volume);
					//volume
					if(headbar_data.volume != p_ctrl->para.data.volume)
					{
						eLIBs_memset(p_ctrl->para.volume_buf, 0, 16);
						p_ctrl->para.data.volume = headbar_data.volume;
						eLIBs_int2str_dec(p_ctrl->para.data.volume, p_ctrl->para.volume_buf);
						{
							__gui_msg_t 	msg;
							
							msg.id 			= HBAR_VOLUME_PAINT;
							msg.h_srcwin 		= NULL;
							msg.h_deswin 	= p_ctrl->para.hbar;
							msg.dwAddData1 	= 0;
							msg.dwAddData2 	= 0;
							msg.dwReserved 	= 0;
							msg.p_arg	   	= NULL;

							GUI_SendNotifyMessage(&msg); 
						}
					}
					//bright
					if(headbar_data.bright!= p_ctrl->para.data.bright)
					{
						eLIBs_memset(p_ctrl->para.bright_buf, 0, 16);
						p_ctrl->para.data.bright = headbar_data.bright;
						eLIBs_int2str_dec(p_ctrl->para.data.bright, p_ctrl->para.bright_buf);
						{
							__gui_msg_t 	msg;
							
							msg.id 			= HBAR_BRIGHT_PAINT;
							msg.h_srcwin 		= NULL;
							msg.h_deswin 	= p_ctrl->para.hbar;
							msg.dwAddData1 	= 0;
							msg.dwAddData2 	= 0;
							msg.dwReserved 	= 0;
							msg.p_arg	   	= NULL;

							GUI_SendNotifyMessage(&msg); 
						}
					}
					
					__msg("----------headbar_data.charge_sta = %d, para.data.charge_sta = %d\n",headbar_data.charge_sta,p_ctrl->para.data.charge_sta);
					
					//battery
					if((headbar_data.vol_level != p_ctrl->para.data.vol_level) /*|| (headbar_data.charge_sta != p_ctrl->para.data.charge_sta)*/ || \
							headbar_data.is_full != p_ctrl->para.data.is_full)
					{
						p_ctrl->para.data.vol_level = headbar_data.vol_level;
						p_ctrl->para.data.is_full = headbar_data.is_full;
						//p_ctrl->para.data.charge_sta = headbar_data.charge_sta;
						{
							__gui_msg_t 	msg;
							
							msg.id 			= HBAR_VOL_PAINT;
							msg.h_srcwin 		= NULL;
							msg.h_deswin 	= p_msg->h_deswin;
							msg.dwAddData1 	= 0;
							msg.dwAddData2 	= 0;
							msg.dwReserved 	= 0;
							msg.p_arg	   		= NULL;

							GUI_SendNotifyMessage(&msg); 
						}
					}
					if(headbar_data.charge_sta)
					{
						__gui_msg_t 	msg;

						__here__;
						p_ctrl->para.data.vol_level = headbar_data.vol_level;
						p_ctrl->para.data.is_full = headbar_data.is_full;
						p_ctrl->para.data.charge_sta = headbar_data.charge_sta;
						

						
						msg.id 			= HBAR_VOL_PAINT;
						msg.h_srcwin 		= NULL;
						msg.h_deswin 	= p_msg->h_deswin;
						msg.dwAddData1 	= 0;
						msg.dwAddData2 	= 0;
						msg.dwReserved 	= 0;
						msg.p_arg	   		= NULL;

						GUI_SendNotifyMessage(&msg); 
					}
					else
					{
						if(p_ctrl->para.data.charge_sta != headbar_data.charge_sta)
						{
							p_ctrl->para.data.charge_sta = headbar_data.charge_sta;
							p_ctrl->para.data.vol_level = headbar_data.vol_level;
							p_ctrl->para.data.is_full = headbar_data.is_full;	
							{
								__gui_msg_t 	msg;
								
								msg.id 			= HBAR_VOL_PAINT;
								msg.h_srcwin 		= NULL;
								msg.h_deswin 	= p_msg->h_deswin;
								msg.dwAddData1 	= 0;
								msg.dwAddData2 	= 0;
								msg.dwReserved 	= 0;
								msg.p_arg	   		= NULL;

								GUI_SendNotifyMessage(&msg); 
							}
						}

					}
					
					if((0 == eLIBs_strcmp(p_ctrl->para.strinsert_buf, p_ctrl->para.strmess_buf)) || 
					   (0 == eLIBs_strcmp(p_ctrl->para.strmedia_buf, p_ctrl->para.strmess_buf)) ||
					   (0 == eLIBs_strcmp(p_ctrl->para.strinsertmic_buf, p_ctrl->para.strmess_buf)))
					{
						if(p_ctrl->para.mess_timecnt > 0)
							p_ctrl->para.mess_timecnt--;
						
						if(p_ctrl->para.mess_timecnt == 0)
						{
							__gui_msg_t 	msg;
							
							msg.id 			= HBAR_DISK_REPAINT;
							msg.h_srcwin 		= NULL;
							msg.h_deswin 	= p_msg->h_deswin;
							msg.dwAddData1 	= 0;
							msg.dwAddData2 	= 0;
							msg.dwReserved 	= 0;
							msg.p_arg	   		= NULL;

							GUI_SendNotifyMessage(&msg); 
						}
					}
				}     		
			}	

			return EPDK_OK;
		}
		
		case GUI_MSG_PAINT:
		{
			return EPDK_OK;		
		}
									
		case HBAR_TIME_PAINT:
		{
			return EPDK_OK;
		}
				
		case HBAR_VOL_PAINT:
		{
			headbar_on_paint(p_msg, HB_PAINT_VOL);

			return EPDK_OK;
		}

		case HBAR_BRIGHT_PAINT:
		{
			headbar_on_paint(p_msg, HB_PAINT_BRIGHT);

			return EPDK_OK;
		}

		case HBAR_VOLUME_PAINT:
		{
			headbar_on_paint(p_msg, HB_PAINT_VOLUME);

			return EPDK_OK;
		}
				
		case HBAR_DISK_PAINT:
		{
			__u32 sd_root =0;
			__u32 usb_root = 0;
			__u32 root_type = 0; 
			__u32 cur_root=0;
			headbar_ctl_t *p_ctrl = NULL;
			headbar_para_t*	headbar_para = NULL;
			p_ctrl = (headbar_ctl_t *)GUI_WinGetAddData(p_msg->h_deswin);
			headbar_para = &p_ctrl->para;

			///////
			if(GUI_IsTimerInstalled(p_msg->h_deswin, HEADBAR_TIMER_ID))
			{
				GUI_ResetTimer(p_msg->h_deswin, HEADBAR_TIMER_ID, HEADBAR_TIMER_CNT, NULL);
			}
			/////////////
			cur_root = p_msg->dwAddData1;
			root_type = p_msg->dwAddData2;
			//__msg("----root_type = 0x%x,\n",root_type);

			if(root_type == 0xff)
			{
				eLIBs_memset( headbar_para->strtf_buf, 0, 16);
				eLIBs_memset( headbar_para->strusb_buf, 0, 16);
				///////
				headbar_para->cur_disk_id = RAT_UNKNOWN;
				headbar_para->mess_timecnt = MESS_TIMER_CNT;
				headbar_para->disk_exist_mediafile = 0;
				dsk_langres_get_menu_text(STRING_INSERT_DISK, headbar_para->strmess_buf, 32); 
			}
			else if(root_type == 0xf)  //no media file cur_disk_id ...???
			{
				//eLIBs_memset( headbar_para->strtf_buf, 0, 16);
				//eLIBs_memset( headbar_para->strusb_buf, 0, 16);
				///////
				headbar_para->mess_timecnt = MESS_TIMER_CNT;
				headbar_para->disk_exist_mediafile = 0;
				dsk_langres_get_menu_text(STRING_NO_MEDIAFILE, headbar_para->strmess_buf, 32); 
			}
			else if(root_type == 0xe)  //insert mic 
			{
				//eLIBs_memset( headbar_para->strtf_buf, 0, 16);
				//eLIBs_memset( headbar_para->strusb_buf, 0, 16);
				///////
				headbar_para->mess_timecnt = MESS_TIMER_CNT;
				headbar_para->disk_exist_mediafile = 0;
				dsk_langres_get_menu_text(STRING_MICINSERT, headbar_para->strmess_buf, 32); 
			}
			else
			{
				eLIBs_memset( headbar_para->strtf_buf, 0, 16);
				eLIBs_memset( headbar_para->strusb_buf, 0, 16);
				///////
				sd_root = (root_type >> 8)&0x00ff;
				usb_root = (root_type)&0x00ff; 
				headbar_para->cur_disk_id = cur_root;
				headbar_para->disk_exist_mediafile = 1;
				__here__;
				if (sd_root == RAT_TF)
				{
					//dsk_langres_get_menu_text("TF", headbar_para->strtf_buf, 16); 
				}
				if(usb_root == RAT_USB)
				{
					//dsk_langres_get_menu_text("USB", headbar_para->strusb_buf, 16); 
				}
				if(headbar_para->cur_disk_id == RAT_UNKNOWN)
				{
					dsk_langres_get_menu_text(STRING_NO_DISK, headbar_para->strmess_buf, 32);
				}
			}
			headbar_on_paint(p_msg, HB_PAINT_DISK);
			
			return EPDK_OK;
		}

		case HBAR_DISK_REPAINT:
		{
			headbar_ctl_t *p_ctrl = NULL;
			headbar_para_t*	headbar_para = NULL;
			
			p_ctrl = (headbar_ctl_t *)GUI_WinGetAddData(p_msg->h_deswin);
			headbar_para = &p_ctrl->para;
			headbar_para->disk_exist_mediafile = 1;
			__here__;
			///////
			if(GUI_IsTimerInstalled(p_msg->h_deswin, HEADBAR_TIMER_ID))
			{
				GUI_ResetTimer(p_msg->h_deswin, HEADBAR_TIMER_ID, HEADBAR_TIMER_CNT, NULL);
			}
			/////////////
			dsk_langres_get_menu_text(STRING_NO_DISK, headbar_para->strmess_buf, 32);

			headbar_on_paint(p_msg, HB_PAINT_DISK);
			
			return EPDK_OK;
		}
#if 0		
		case HBAR_FORCE_VOLUME_PAINT:
		{
			headbar_ctl_t*	p_ctrl = NULL;
			p_ctrl = (headbar_ctl_t *)GUI_WinGetAddData(p_msg->h_deswin);

			eLIBs_printf(" -- %s, %d, HBAR_FORCE_VOLUME_PAINT -- \n", __FILE__, __LINE__);
			eLIBs_memset(p_ctrl->para.volume_buf, 0, 16);
			p_ctrl->para.data.volume = dsk_volume_get();;
			eLIBs_int2str_dec(p_ctrl->para.data.volume, p_ctrl->para.volume_buf);
						
			headbar_on_paint(p_msg, HB_PAINT_VOLUME);
			return EPDK_OK;
		}
#endif		
		case GUI_MSG_WIN_WAKEUP://paint->creat
		{
			headbar_ctl_t *p_ctrl = NULL;	

			__here__;
			p_ctrl = (headbar_ctl_t *)GUI_WinGetAddData(p_msg->h_deswin);
			if(GUI_IsTimerInstalled(p_msg->h_deswin, HEADBAR_TIMER_ID))
			{
				GUI_ResetTimer(p_msg->h_deswin, HEADBAR_TIMER_ID, HEADBAR_TIMER_CNT, NULL);
			}
			else
			{
				GUI_SetTimer(p_msg->h_deswin, HEADBAR_TIMER_ID, HEADBAR_TIMER_CNT, NULL);
			}

#if 0
			if(0 == p_ctrl->res_init)
			{
				__here__;
				init_headbar_para(p_msg, &(p_ctrl->para),0);	
				p_ctrl->res_init = 1;
			}
#endif			
			////////////
			init_headbar_data(&(p_ctrl->para));
			headbar_on_paint(p_msg, HB_PAINT_INIT);
			
			return EPDK_OK;
		}
		case GUI_MSG_WIN_SLEEP:
		{
			headbar_ctl_t *p_ctrl = NULL;	

			__here__;
			p_ctrl = (headbar_ctl_t *)GUI_WinGetAddData(p_msg->h_deswin);
			if( GUI_IsTimerInstalled(p_msg->h_deswin, HEADBAR_TIMER_ID))		
				GUI_KillTimer(p_msg->h_deswin, HEADBAR_TIMER_ID);

#if 0
			if(1 == p_ctrl->res_init)
			{
				deinit_headbar_para( &(p_ctrl->para) );	
				p_ctrl->res_init = 0;
			}
#endif
			return EPDK_OK;
		}
		default:
			break;		
	}
	
	return EPDK_OK;
}

H_WIN headbar_frm_create(H_WIN parent, H_LYR layer)
{
	H_WIN 					h_win;		
	__gui_framewincreate_para_t 	para;
	
	eLIBs_memset(&para, 0, sizeof(__gui_framewincreate_para_t));
	
	para.dwStyle          		= WS_VISIBLE;
	para.dwExStyle        		= 0;
	para.name			  	= NULL;
	para.hOwner			  	= 0;
	para.hHosting         		= parent;
	para.FrameWinProc     		= cb_headbar_framewin;
	para.id				  	= HEADBAR_COM_ID;
	para.hLayer           		= layer;
	
	para.rect.x			  	= headbar_uipara.hbar_frmwin.x;
	para.rect.y			  	= headbar_uipara.hbar_frmwin.y;
	para.rect.width		  	= headbar_uipara.hbar_frmwin.w;
	para.rect.height	  		= headbar_uipara.hbar_frmwin.h;	
	
	h_win = GUI_FrmWinCreate(&para);
	if( !h_win )
	{
		__err("headbar frmwin create error ! \n");
	} 
	
	return h_win;
}

__s32 headbar_frm_delete(H_WIN frmwin)
{
	if( frmwin )
	{
		GUI_FrmWinDelete(frmwin);
	}
	
	return EPDK_FAIL;
}

