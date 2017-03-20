/*
*********************************************************************************************************
*											        ePDK
*						            the Easy Portable/Player Develop Kits
*									           willow app sample
*
*						        (c) Copyright 2006-2007, SoftWinners Microelectronic Co., Ltd.
*											All	Rights Reserved
*
* File    : photo_spsc.c
* By      : john.fu
* Version : V1.00
*********************************************************************************************************
*/

#include "photo_spsc.h"

typedef struct tag_photo_spsc_res
{
	PHOTO_BMP_RES_T bmp_big_pause;//void *bmp_big_pause;
	PHOTO_BMP_RES_T bmp_play;//void *bmp_play;
	PHOTO_BMP_RES_T bmp_pause;//void *bmp_pause;
	PHOTO_BMP_RES_T bmp_next;//void *bmp_next;
	PHOTO_BMP_RES_T bmp_prev;//void *bmp_prev;
	PHOTO_BMP_RES_T bmp_slide_bg;//void *bmp_slide_bg;
	PHOTO_BMP_RES_T bmp_slide_bar;//void *bmp_slide_bar;
	PHOTO_BMP_RES_T bmp_slide_block;//void *bmp_slide_block;
	PHOTO_BMP_RES_T bmp_slide_block_l;//void *bmp_slide_block_l;
	PHOTO_BMP_RES_T bmp_slide_block_r;//void *bmp_slide_block_r;
	PHOTO_BMP_RES_T bmp_volume_l;//void *bmp_volume_l;
	PHOTO_BMP_RES_T bmp_volume_r;//void *bmp_volume_r;
	PHOTO_BMP_RES_T bmp_mute;//void *bmp_mute;
	PHOTO_BMP_RES_T bmp_bg; 

	char taken_time[32];
	char file_size[32];
	char place[32];
	char size[32];
	char resolution[32];
	char camera_model[32];
	char exposurebais[32];
	char flash[32];
	char focus_length[32];
	char color_space[32];

}photo_spsc_res_t;

typedef enum tag_PAINT_PHOTO_SPSC_T
{
	PAINT_PHOTO_SPSC_NULL,
	PAINT_PHOTO_SPSC_INFO,
	PAINT_PHOTO_SPSC_PLAY_STATUS,
	PAINT_PHOTO_SPSC_VOLUME
}PAINT_PHOTO_SPSC_T;

typedef struct tag_photo_spsc_ctrl
{
	photo_spsc_para_t  *spsc_para;
	photo_spsc_res_t   spsc_res;
	__s32              txt_color;
	PAINT_PHOTO_SPSC_T disp_type;
}photo_spsc_ctrl_t;


#define SPSC_PLAY_STATUS_X         299//600
#define SPSC_PLAY_STATUS_Y         18//50
#define SPSC_PLAY_STATUS_W         81//96
#define SPSC_PLAY_STATUS_H         36//42
#define SPSC_PLAY_STATUS_STR_W     50

#define SPSC_INFO_MENU_X           40//200
#define SPSC_INFO_MENU_Y           40//100

#define SPSC_VOLUME_X              18//20
#define SPSC_VOLUME_Y              12//30

#define SPSC_VOLUME_MUTE_X_OFFSET  (36-18)
#define SPSC_VOLUME_MUTE_Y_OFFSET  (18-12)
#define SPSC_VOLUME_MUTE_X_R_OFFSET 5

#define SPSC_VOLUME_X_OFFSET        SPSC_VOLUME_MUTE_X_OFFSET
#define SPSC_VOLUME_Y_OFFSET        SPSC_VOLUME_MUTE_Y_OFFSET

#define SPSC_VOLUME_X_OFFSET_1      SPSC_VOLUME_X_OFFSET
#define SPSC_VOLUME_Y_OFFSET_1      (SPSC_VOLUME_Y_OFFSET + 4)

#define SPSC_VOLUME_TXT_W           30
#define SPSC_VOLUME_TXT_Y           (SPSC_VOLUME_Y + (SPSC_VOLUME_Y_OFFSET_1 / 2))

static __s8 Timer_SpscStaId = 0xff;
#define DELAY_SPSCSTAID             DELAY_HIDE_MENU

/*
	������Դ
*/
static void  photo_spsc_init(photo_spsc_ctrl_t *spsc_ctrl)
{
	photo_spsc_res_t *spsc_res;
	
	spsc_res = &spsc_ctrl->spsc_res;

	create_bmp_res(ID_PHOTO_INFO_ICON_BMP, spsc_res->bmp_big_pause);

	create_bmp_res(ID_PHOTO_PLAY_BMP, spsc_res->bmp_play);

	create_bmp_res(ID_PHOTO_PAUSE_BMP, spsc_res->bmp_pause);

	create_bmp_res(ID_PHOTO_PREV_BMP, spsc_res->bmp_prev);
	
	create_bmp_res(ID_PHOTO_NEXT_BMP, spsc_res->bmp_next);

	create_bmp_res(ID_PHOTO_VOLUME_L_BMP, spsc_res->bmp_volume_l);

	create_bmp_res(ID_PHOTO_VOLUME_R_BMP, spsc_res->bmp_volume_r);

	create_bmp_res(ID_PHOTO_MUTE_BMP, spsc_res->bmp_mute);

	create_bmp_res(ID_PHOTO_SLIDE_BG_BMP, spsc_res->bmp_slide_bg);

	create_bmp_res(ID_PHOTO_SLIDE_BAR_BMP, spsc_res->bmp_slide_bar);

	create_bmp_res(ID_PHOTO_SLIDE_BLOCK_BMP, spsc_res->bmp_slide_block);
	
	create_bmp_res(ID_PHOTO_SLIDE_BLOCK_L_BMP, spsc_res->bmp_slide_block_l);

	create_bmp_res(ID_PHOTO_SLIDE_BLOCK_R_BMP, spsc_res->bmp_slide_block_r);

	create_bmp_res(ID_PHOTO_MAIN_BG_BMP, spsc_res->bmp_bg);

	//load string
	get_lang_res(STRING_PHOTO_TAKEN_TIME, spsc_res->taken_time);
	get_lang_res(STRING_PHOTO_FILE_SIZE, spsc_res->file_size);
	get_lang_res(STRING_PHOTO_PLACE, spsc_res->place);
	get_lang_res(STRING_PHOTO_SIZE, spsc_res->size);
	get_lang_res(STRING_PHOTO_RESOLUTION, spsc_res->resolution);
	get_lang_res(STRING_PHOTO_CAMERA_MODEL, spsc_res->camera_model);
	get_lang_res(STRING_PHOTO_EXPOSUREBAIS, spsc_res->exposurebais);
	get_lang_res(STRING_PHOTO_FLASH, spsc_res->flash);
	get_lang_res(STRING_PHOTO_FOCUS_LENGTH, spsc_res->focus_length);
	get_lang_res(STRING_PHOTO_COLOR_SPACE, spsc_res->color_space);
}
/*
	�ͷ���Դ
*/
static void photo_spsc_uninit(photo_spsc_ctrl_t *spsc_ctrl)
{
	photo_spsc_res_t *spsc_res;
	
	spsc_res = &spsc_ctrl->spsc_res;

	destroy_bmp_res(spsc_res->bmp_big_pause);

	destroy_bmp_res(spsc_res->bmp_play);

	destroy_bmp_res(spsc_res->bmp_pause);

	destroy_bmp_res(spsc_res->bmp_prev);

	destroy_bmp_res(spsc_res->bmp_next);

	destroy_bmp_res(spsc_res->bmp_volume_l);

	destroy_bmp_res(spsc_res->bmp_volume_r);

	destroy_bmp_res(spsc_res->bmp_mute);

	destroy_bmp_res(spsc_res->bmp_slide_bg);

	destroy_bmp_res(spsc_res->bmp_slide_bar);

	destroy_bmp_res(spsc_res->bmp_slide_block);

	destroy_bmp_res(spsc_res->bmp_slide_block_l);

	destroy_bmp_res(spsc_res->bmp_slide_block_r);

	destroy_bmp_res(spsc_res->bmp_bg);

}

__s32 photo_spsc_set_no(__gui_msg_t* msg)
{
    photo_spsc_ctrl_t *spsc_ctrl;
    spsc_ctrl = (photo_spsc_ctrl_t *)GUI_WinGetAddData(msg->h_deswin);

    if(!spsc_ctrl)
    {
        return EPDK_FAIL;
    }
    spsc_ctrl->spsc_para->cur_no = msg->dwAddData1;
    spsc_ctrl->spsc_para->total_no = msg->dwAddData2;
    if (spsc_ctrl->disp_type == PAINT_PHOTO_SPSC_PLAY_STATUS)
    {
        photo_spsc_pait(msg, PAINT_PHOTO_SPSC_PLAY_STATUS);
    }
    return EPDK_OK;
}

/*
	��ʾ
*/
static void  photo_spsc_show(photo_spsc_ctrl_t *spsc_ctrl)
{
	
	GUI_LyrWinSetSta(spsc_ctrl->spsc_para->layer, GUI_LYRWIN_STA_ON);
	GUI_LyrWinSetTop(spsc_ctrl->spsc_para->layer);
}
/*
	��ʧ
*/
static void  photo_spsc_hide(photo_spsc_ctrl_t *spsc_ctrl)
{
	GUI_LyrWinSetSta(spsc_ctrl->spsc_para->layer, GUI_LYRWIN_STA_SUSPEND);
	
	spsc_ctrl->spsc_para->isShowVolume = 0;
}
/*
	����
*/
static void photo_spsc_clear(photo_spsc_ctrl_t *spsc_ctrl)
{
	GUI_LyrWinSel(spsc_ctrl->spsc_para->layer);
	GUI_SetBkColor(0);
	GUI_Clear();
	//GUI_BMP_RES_Draw(spsc_ctrl->spsc_res.bmp_bg, 0, 0);
}
/*
	��ʾ����״̬
*/
static void  phot_spsc_paint_play_status(photo_spsc_ctrl_t *spsc_ctrl)
{
#if _USE_MEMDEV_Handle_
	GUI_MEMDEV_Handle	draw_mem;  
#endif
	GUI_RECT gui_rect;
	char buf[32];

	spsc_ctrl->spsc_para->isShowVolume = 0;

	GUI_LyrWinSel(spsc_ctrl->spsc_para->layer);
	GUI_SetFont(SWFFont);
	GUI_UC_SetEncodeUTF8();
	GUI_SetColor(spsc_ctrl->txt_color);

#if _USE_MEMDEV_Handle_
	draw_mem = GUI_MEMDEV_Create(SPSC_PLAY_STATUS_X, SPSC_PLAY_STATUS_Y, SPSC_PLAY_STATUS_W, SPSC_PLAY_STATUS_H);
	GUI_MEMDEV_Select(draw_mem);
#endif
	
	if (spsc_ctrl->spsc_para->play_status == PHOTO_STAT_PLAY)
		GUI_BMP_RES_Draw(spsc_ctrl->spsc_res.bmp_play, SPSC_PLAY_STATUS_X, SPSC_PLAY_STATUS_Y);
	else  if (spsc_ctrl->spsc_para->play_status == PHOTO_STAT_PAUSE)
		GUI_BMP_RES_Draw(spsc_ctrl->spsc_res.bmp_pause, SPSC_PLAY_STATUS_X, SPSC_PLAY_STATUS_Y);
	else if (spsc_ctrl->spsc_para->play_status == PHOTO_STAT_PREV)
		GUI_BMP_RES_Draw(spsc_ctrl->spsc_res.bmp_prev, SPSC_PLAY_STATUS_X, SPSC_PLAY_STATUS_Y);
	else
		GUI_BMP_RES_Draw(spsc_ctrl->spsc_res.bmp_next, SPSC_PLAY_STATUS_X, SPSC_PLAY_STATUS_Y);

	GUI_OpenAlphaBlend();
	app_sprintf(buf, "%d", spsc_ctrl->spsc_para->cur_no+1);
	/*gui_rect.x0 = SPSC_PLAY_STATUS_X+5;
	gui_rect.y0 = SPSC_PLAY_STATUS_Y+0;
	gui_rect.x1 = SPSC_PLAY_STATUS_X+90;
	gui_rect.y1 = SPSC_PLAY_STATUS_Y+21;*/
	gui_rect.x1 = SPSC_PLAY_STATUS_X + SPSC_PLAY_STATUS_W - 1;
	gui_rect.x0 = gui_rect.x1 - SPSC_PLAY_STATUS_STR_W;
	gui_rect.y0 = SPSC_PLAY_STATUS_Y + 0;
	gui_rect.y1 = SPSC_PLAY_STATUS_Y + SPSC_PLAY_STATUS_H / 2;
	if (spsc_ctrl->spsc_para->play_status == PHOTO_STAT_NEXT)
	{
		GUI_DispStringInRect(buf, &gui_rect, GUI_TA_LEFT);
	}
	else
	{
		GUI_DispStringInRect(buf, &gui_rect, GUI_TA_RIGHT);
	}
	
	__msg("now play[%d, %d]\n",  spsc_ctrl->spsc_para->cur_no, spsc_ctrl->spsc_para->total_no);

	app_sprintf(buf, "%d", spsc_ctrl->spsc_para->total_no);
	/*gui_rect.x0 = SPSC_PLAY_STATUS_X+5;
	gui_rect.y0 = SPSC_PLAY_STATUS_Y+20;
	gui_rect.x1 = SPSC_PLAY_STATUS_X+90;
	gui_rect.y1 = SPSC_PLAY_STATUS_Y+41;*/
	gui_rect.y0 = gui_rect.y1;
	gui_rect.y1 = SPSC_PLAY_STATUS_Y + SPSC_PLAY_STATUS_H - 2;
	if (spsc_ctrl->spsc_para->play_status == PHOTO_STAT_NEXT)
	{
		GUI_DispStringInRect(buf, &gui_rect, GUI_TA_LEFT);
	}
	else
	{
		GUI_DispStringInRect(buf, &gui_rect, GUI_TA_RIGHT);
	}
	GUI_CloseAlphaBlend();
#if _USE_MEMDEV_Handle_
	GUI_MEMDEV_CopyToLCD_Ex(draw_mem); 
	GUI_MEMDEV_Select( NULL );			
	GUI_MEMDEV_Delete(draw_mem);
	draw_mem = NULL;
#endif
}

/*
�����ļ���Ϣ
*/
__s32 update_pic_info(__s32 index, photo_spsc_para_t *spsc_para)
{
	__s32 ret;
	__anole_pic_info_t info;
	ZeroMemory(&info, sizeof(info));
	ret = ANOLE_GetPicInfo(index, &info);
	spsc_para->width = info.size.width;
	spsc_para->height =info.size.height;
	spsc_para->file_size = info.file_len;
	
	__msg("%s \n", info.name);
	eLIBs_GetFileCTime(info.name, &spsc_para->time);
	
	eLIBs_strncpy(spsc_para->place,   info.name, eLIBs_strlen(info.name)-eLIBs_strlen(eLIBs_strchrlast(info.name, '\\')));
	eLIBs_strcpy(spsc_para->name,  eLIBs_strchrlast(info.name, '\\')+1);
	
	return ret;
}

/*
	��ʾ��Ϣ�˵� 
*/
static __s32 phot_spsc_paint_info(photo_spsc_ctrl_t *spsc_ctrl)
{
//	GUI_RECT gui_rect;
	char buf[128], buf1[128];
	__s32 x, y;
	__s32 font_h = 20, sp_h = 30;

	spsc_ctrl->spsc_para->isShowVolume = 0;
	
	update_pic_info(ANOLE_GetCurPicIndex(), spsc_ctrl->spsc_para);
	
	GUI_LyrWinSel(spsc_ctrl->spsc_para->layer);
	GUI_UC_SetEncodeUTF8();
	GUI_SetFont(SWFFont);
	GUI_SetColor(spsc_ctrl->txt_color);

	x = SPSC_INFO_MENU_X;
	y = SPSC_INFO_MENU_Y;
	GUI_BMP_RES_Draw(spsc_ctrl->spsc_res.bmp_big_pause, x, y);

	x += 40;//TODO:Macro
	GUI_DispStringAt(spsc_ctrl->spsc_para->name, x, y);
	y+= sp_h;
	eLIBs_strcpy(buf, spsc_ctrl->spsc_res.taken_time);
	eLIBs_strcat(buf, " : ");
	app_sprintf(buf1, "%d/%d/%d %d%d:%d%d", spsc_ctrl->spsc_para->time.year, spsc_ctrl->spsc_para->time.month, spsc_ctrl->spsc_para->time.day,
						spsc_ctrl->spsc_para->time.hour/10, spsc_ctrl->spsc_para->time.hour%10, spsc_ctrl->spsc_para->time.minute/10, spsc_ctrl->spsc_para->time.minute%10);
	eLIBs_strcat(buf, buf1);
	GUI_DispStringAt(buf, x, y);
	y+= font_h;
	eLIBs_strcpy(buf, spsc_ctrl->spsc_res.file_size);
	eLIBs_strcat(buf, " : ");
	filesize2str(spsc_ctrl->spsc_para->file_size, buf1);
	eLIBs_strcat(buf, buf1);
	GUI_DispStringAt(buf, x, y);
	y+= font_h;
	eLIBs_strcpy(buf, spsc_ctrl->spsc_res.place);
	eLIBs_strcat(buf, " : ");
	eLIBs_strcat(buf, spsc_ctrl->spsc_para->place);
	GUI_DispStringAt(buf, x, y);
	y+= sp_h;
	app_sprintf(buf, "%s : %d x %d", spsc_ctrl->spsc_res.size, spsc_ctrl->spsc_para->width, spsc_ctrl->spsc_para->height);
	GUI_DispStringAt(buf, x, y);
	/*y+= font_h;
	eLIBs_strcpy(buf, spsc_ctrl->spsc_res.resolution);
	eLIBs_strcat(buf, " : ");
	GUI_DispStringAt(buf, x, y);
	y+= font_h;
	eLIBs_strcpy(buf, spsc_ctrl->spsc_res.camera_model);
	eLIBs_strcat(buf, " : ");
	GUI_DispStringAt(buf, x, y);
	y+= font_h;
	eLIBs_strcpy(buf, spsc_ctrl->spsc_res.exposurebais);
	eLIBs_strcat(buf, " : ");
	GUI_DispStringAt(buf, x, y);
	y+= font_h;
	eLIBs_strcpy(buf, spsc_ctrl->spsc_res.flash);
	eLIBs_strcat(buf, " : ");
	GUI_DispStringAt(buf, x, y);
	y+= font_h;
	eLIBs_strcpy(buf, spsc_ctrl->spsc_res.focus_length);
	eLIBs_strcat(buf, " : ");
	GUI_DispStringAt(buf, x, y);
	y+= font_h;
	eLIBs_strcpy(buf, spsc_ctrl->spsc_res.color_space);
	eLIBs_strcat(buf, " : ");
	GUI_DispStringAt(buf, x, y);*/
	return 0;
}
/*
	��ʾ����
*/
static void photo_spsc_paint_volume(photo_spsc_ctrl_t *spsc_ctrl)
{
#if _USE_MEMDEV_Handle_
	GUI_MEMDEV_Handle	draw_mem;  
#endif
	photo_spsc_res_t   *spsc_res;
	photo_uipara_t *photo_uipara;
//	__s32 i;
	__s32 block_head_w, block_tail_w, block_w;
	__s32 prog_start; //���ȿ�ʼλ��
	__s32 prog_end;   //���Ƚ���λ��
	char  buf[8];
	
	spsc_ctrl->spsc_para->isShowVolume = 1;

	photo_uipara = get_photo_uipara();
	spsc_res = 	&spsc_ctrl->spsc_res;

	GUI_LyrWinSel(spsc_ctrl->spsc_para->layer);
	GUI_SetBkColor(APP_COLOR_CLEAR);
	GUI_SetFont(SWFFont);
#if _USE_MEMDEV_Handle_	
	draw_mem = GUI_MEMDEV_Create(SPSC_VOLUME_X, SPSC_VOLUME_Y, photo_uipara->bmp_slide_bg.w, photo_uipara->bmp_slide_bg.h);
	GUI_MEMDEV_Select(draw_mem);
#endif
	GUI_ClearRect(SPSC_VOLUME_X , SPSC_VOLUME_Y, 
		SPSC_VOLUME_X+photo_uipara->bmp_slide_bg.w, SPSC_VOLUME_Y+photo_uipara->bmp_slide_bg.h);

	GUI_BMP_RES_Draw(spsc_res->bmp_slide_bg, SPSC_VOLUME_X , SPSC_VOLUME_Y);
//	GUI_OpenAlphaBlend();
	if (spsc_ctrl->spsc_para->volume == 0)
	{
		GUI_BMP_RES_Draw(spsc_res->bmp_mute, SPSC_VOLUME_X+SPSC_VOLUME_MUTE_X_OFFSET, SPSC_VOLUME_Y+SPSC_VOLUME_MUTE_Y_OFFSET);
		GUI_BMP_RES_Draw(spsc_res->bmp_mute, SPSC_VOLUME_X+SPSC_VOLUME_MUTE_X_OFFSET+photo_uipara->bmp_volume.w+photo_uipara->bmp_slide_bar.w + SPSC_VOLUME_MUTE_X_R_OFFSET, SPSC_VOLUME_Y+SPSC_VOLUME_MUTE_Y_OFFSET);
	}
	else
	{
		GUI_BMP_RES_Draw(spsc_res->bmp_volume_l,  SPSC_VOLUME_X+SPSC_VOLUME_X_OFFSET, SPSC_VOLUME_Y+SPSC_VOLUME_Y_OFFSET);
		GUI_BMP_RES_Draw(spsc_res->bmp_volume_r, SPSC_VOLUME_X+SPSC_VOLUME_X_OFFSET+photo_uipara->bmp_volume.w+photo_uipara->bmp_slide_bar.w + SPSC_VOLUME_MUTE_X_R_OFFSET, SPSC_VOLUME_Y+SPSC_VOLUME_Y_OFFSET);
	}

	//�����ȱ���
	GUI_BMP_RES_Draw(spsc_res->bmp_slide_bar, SPSC_VOLUME_X+(SPSC_VOLUME_X_OFFSET_1)+photo_uipara->bmp_volume.w, SPSC_VOLUME_Y+(SPSC_VOLUME_Y_OFFSET_1));

	//������
	prog_start = SPSC_VOLUME_X+SPSC_VOLUME_X_OFFSET_1+photo_uipara->bmp_volume.w;
	prog_end   = prog_start + spsc_ctrl->spsc_para->volume * photo_uipara->bmp_slide_bar.w / AUDIO_DEVICE_VOLUME_MAX;
	if (prog_start < prog_end)
	{
		//Ŀǰ��ui��ͷblock��βblock���м��block�Ŀ������
		block_w      = photo_uipara->bmp_slide_block.w;
		block_head_w = block_w;
		block_tail_w = block_w;
		ASSERT(prog_start + block_head_w <= prog_end);
		ASSERT(prog_start + block_tail_w <= prog_end);
		//������ͷ
		GUI_BMP_RES_Draw(spsc_res->bmp_slide_block_l, prog_start, SPSC_VOLUME_Y+SPSC_VOLUME_Y_OFFSET_1);
		prog_start += block_head_w;
		//������β
		prog_end -= block_tail_w;
		GUI_BMP_RES_Draw(spsc_res->bmp_slide_block_r, prog_end, SPSC_VOLUME_Y+SPSC_VOLUME_Y_OFFSET_1);
		//����м䲿��
		while (prog_start < prog_end)
		{
			ASSERT(prog_start + block_w < prog_end + block_tail_w);
			GUI_BMP_RES_Draw(spsc_res->bmp_slide_block, prog_start, SPSC_VOLUME_Y+SPSC_VOLUME_Y_OFFSET_1);
			prog_start += block_w;
		}
	}

	GUI_SetColor(spsc_ctrl->txt_color);
	app_sprintf(buf, "%d", spsc_ctrl->spsc_para->volume);
	GUI_DispStringAt(buf, SPSC_VOLUME_X+photo_uipara->bmp_slide_bg.w - SPSC_VOLUME_TXT_W, SPSC_VOLUME_TXT_Y);


// 	for (i=0; i<((spsc_ctrl->spsc_para->volume*photo_uipara->bmp_slide_bar.w)/((AUDIO_DEVICE_VOLUME_MAX + 1)*photo_uipara->bmp_slide_block.w)); i++)
// 	{
// 		if (i == 0)
// 			GUI_BMP_RES_Draw(spsc_res->bmp_slide_block_l, SPSC_VOLUME_X+SPSC_VOLUME_X_OFFSET_1+photo_uipara->bmp_volume.w+i*photo_uipara->bmp_slide_block.w,  SPSC_VOLUME_Y+SPSC_VOLUME_Y_OFFSET_1);
// 		else if (i == ((photo_uipara->bmp_slide_bar.w)/(photo_uipara->bmp_slide_block.w)-1))
// 			GUI_BMP_RES_Draw(spsc_res->bmp_slide_block_r, SPSC_VOLUME_X+SPSC_VOLUME_X_OFFSET_1+photo_uipara->bmp_volume.w+i*photo_uipara->bmp_slide_block.w,  SPSC_VOLUME_Y+SPSC_VOLUME_Y_OFFSET_1);	
// 		else
// 			GUI_BMP_RES_Draw(spsc_res->bmp_slide_block, SPSC_VOLUME_X+SPSC_VOLUME_X_OFFSET_1+photo_uipara->bmp_volume.w+i*photo_uipara->bmp_slide_block.w,  SPSC_VOLUME_Y+SPSC_VOLUME_Y_OFFSET_1);
// 	}

//	GUI_CloseAlphaBlend();
#if _USE_MEMDEV_Handle_
	GUI_MEMDEV_CopyToLCD_Ex(draw_mem); 
	GUI_MEMDEV_Select( NULL );			
	GUI_MEMDEV_Delete(draw_mem);
	draw_mem = NULL;
#endif
}

#ifdef SUPPORT_DIGIT_KEY
__s32 photo_delete_digit_layer(H_WIN h_win)
{
	photo_spsc_ctrl_t *photo_ctrl;
	
	photo_ctrl = (photo_spsc_ctrl_t *)GUI_WinGetAddData(h_win);
	
	if(NULL == photo_ctrl)
	{
		return EPDK_FAIL;
	}

	if(photo_ctrl->spsc_para->hdigit_layer)
	{
		GUI_LyrWinDelete(photo_ctrl->spsc_para->hdigit_layer);
		photo_ctrl->spsc_para->hdigit_layer = NULL;
	}

	if(photo_ctrl->spsc_para->digit_timmer_id)
	{
		if(GUI_IsTimerInstalled(h_win, photo_ctrl->spsc_para->digit_timmer_id))
		{
			GUI_KillTimer(h_win, photo_ctrl->spsc_para->digit_timmer_id);
		}
	}

	return EPDK_OK;
	
}

static __s32 photo_n2digits(__s32 num, __s32 * digits)
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

static __s32 photo_digit_layer_draw(	photo_spsc_ctrl_t *spsc_ctrl)
{   
    char text[64];
    __s32 i;
    RECT rect;
	__s32 digits[16]={0};
	__s32	 ret;	
 	HTHEME bg_hdl;
	GUI_RECT gui_rect;
  

    if(NULL == spsc_ctrl->spsc_para->hdigit_layer)
    {
        __wrn("digit layer is null...\n");
        return EPDK_FAIL;
    }

    if(GUI_LYRWIN_STA_ON != GUI_LyrWinGetSta(spsc_ctrl->spsc_para->hdigit_layer))
    {
        __wrn("digit layer is not on...\n");
        return EPDK_FAIL;
    }
    if(0)//(player_ctrl->ndigit_song_num > total_song_num)//112358 ���������ΧҲ��ʾ�������
    {
        //dsk_langres_get_menu_text(STRING_MUSIC_NO_THIS_SONG, text, sizeof(text));
    }
    else
    {
        eLIBs_memset(text, 0, sizeof(text));
        eLIBs_int2str_dec(spsc_ctrl->spsc_para->ndigit_page, text);
    }
	GUI_LyrWinSel(spsc_ctrl->spsc_para->hdigit_layer);

	GUI_LyrWinGetScnWindow(spsc_ctrl->spsc_para->hdigit_layer, &rect);

	rect.x = 0;
	rect.y = 0;

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

	gui_rect.x0 = 0;
	gui_rect.y0 = 6;
	gui_rect.x1 = gui_rect.x0+rect.width-1;
	gui_rect.y1 = gui_rect.y0+rect.height-1;

	ret = music_n2digits(spsc_ctrl->spsc_para->ndigit_page, digits);

	GUI_SetFont(SWFFont);

	GUI_SetColor(GUI_RED);
	get_menu_text(STRING_PHOTO_DIG_SEL,text, sizeof(text));
	GUI_UC_SetEncodeUTF8();
	GUI_DrawHLine((rect.y+rect.height/2-10),rect.x+5,rect.x+rect.width-11);
	GUI_SetColor(GUI_WHITE);
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

		//com_set_palette_by_id(ID_FM_COLOURTABLE_BMP);
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

			y = rect.y+((__s32)(rect.height+32)-(__s32)bmp_get_height(hbmp))/2;
			
			GUI_LyrWinSel(spsc_ctrl->spsc_para->hdigit_layer);
			GUI_BMP_Draw(dsk_theme_hdl2buf(hdl), x, y);
			GUI_LyrWinSetTop(spsc_ctrl->spsc_para->hdigit_layer);
			
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


/*
	��ʾ
*/
static __s32 photo_spsc_pait(__gui_msg_t * msg, PAINT_PHOTO_SPSC_T disp_type)
{
	photo_spsc_ctrl_t *spsc_ctrl;

	DEBUG_photo("photo_spsc_pait(%d)\n", disp_type);
	
	spsc_ctrl = (photo_spsc_ctrl_t *)GUI_WinGetAddData(msg->h_deswin);
	if (spsc_ctrl->disp_type != disp_type)
	{
		photo_spsc_show( spsc_ctrl);
		photo_spsc_clear(spsc_ctrl);
		spsc_ctrl->disp_type = disp_type;
	}
	
	switch (disp_type)
	{
		case PAINT_PHOTO_SPSC_INFO:
		{
			phot_spsc_paint_info(spsc_ctrl);
			break;
		}
		case PAINT_PHOTO_SPSC_PLAY_STATUS:
		{
			phot_spsc_paint_play_status(spsc_ctrl);
			break;
		}
		case PAINT_PHOTO_SPSC_VOLUME:
		{
			photo_spsc_paint_volume(spsc_ctrl);
			break;
		}
		default:
			break;
	}
	return EPDK_OK;
}
/*
	��������
*/
static __s32 photo_spsc_key_proc(__gui_msg_t *msg)
{
	static __s32 last_key=0xffffffff, key_cnt=0;
	photo_spsc_ctrl_t *spsc_ctrl;

	DEBUG_CBMSGEx(photo_spsc_key_proc);
	
	spsc_ctrl = (photo_spsc_ctrl_t *)GUI_WinGetAddData(msg->h_deswin);

	//////////////////////////////////////////////////////////////////////////
	{
		if (msg->dwAddData2 != KEY_UP_ACTION)
		{
			if (spsc_ctrl->spsc_para->isShowVolume != 0
				&& (msg->dwAddData1 == GUI_MSG_KEY_LONGLEFT || msg->dwAddData1 == GUI_MSG_KEY_LONGRIGHT))
			{//��������
			} 
			else
			{
				DEBUG_photo(" do nothing \n");
				return EPDK_OK;
			}
		}

		if (msg->dwAddData1 == VK_POP_VOLUME)
		{
			GUI_ResetTimer(msg->h_deswin, Timer_SpscStaId, DELAY_SPSCSTAID, 0);
			photo_spsc_pait(msg, PAINT_PHOTO_SPSC_VOLUME);
			//photo_cmd2parent(msg->h_deswin, CMD_PHOTO_SPSC_VOLUME, spsc_ctrl->spsc_para->volume, 0);
			//last_key = msg->dwAddData1;
		}
		else 
		{
			if (spsc_ctrl->spsc_para->isShowVolume == 0)
			{
				if (msg->dwAddData1 == GUI_MSG_KEY_LEFT || msg->dwAddData1 == GUI_MSG_KEY_LONGLEFT
					|| msg->dwAddData1 == GUI_MSG_KEY_IR_LEFT || msg->dwAddData1 == GUI_MSG_KEY_LONGIR_LEFT)//cai++
				{
					DEBUG_photo("\n _app_ GUI_MSG_KEY_LEFT -> GUI_MSG_KEY_UP _atv_ \n");
					msg->dwAddData1 = GUI_MSG_KEY_UP;
				}
				else if (msg->dwAddData1 == GUI_MSG_KEY_RIGHT || msg->dwAddData1 == GUI_MSG_KEY_LONGRIGHT
					|| msg->dwAddData1 == GUI_MSG_KEY_IR_RIGHT || msg->dwAddData1 == GUI_MSG_KEY_LONGIR_RIGHT)
				{
					DEBUG_photo("\n _app_ GUI_MSG_KEY_RIGHT -> GUI_MSG_KEY_DOWN _atv_ \n");
					msg->dwAddData1 = GUI_MSG_KEY_DOWN;
				}
			}
			else
			{
				if (msg->dwAddData1 == GUI_MSG_KEY_ESCAPE || last_key == GUI_MSG_KEY_MENU)
				{
					key_cnt = 0;	
					last_key = 0xffffffff;
					photo_spsc_hide(spsc_ctrl);
					spsc_ctrl->disp_type = PAINT_PHOTO_SPSC_NULL;
					return EPDK_OK;
				}
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////
	
	switch (msg->dwAddData1)
	{
		case GUI_MSG_KEY_LEFT:
		case GUI_MSG_KEY_LONGLEFT:
		case GUI_MSG_KEY_IR_LEFT:    //cai++ for ir 
		case GUI_MSG_KEY_LONGIR_LEFT:   
		{
			if (AUDIO_DEVICE_VOLUME_MIN < spsc_ctrl->spsc_para->volume)
			{
				spsc_ctrl->spsc_para->volume--;
				photo_spsc_pait(msg, PAINT_PHOTO_SPSC_VOLUME);
				photo_cmd2parent(msg->h_deswin, CMD_PHOTO_SPSC_VOLUME, spsc_ctrl->spsc_para->volume, 0);
			}
			__msg("spsc_ctrl->spsc_para->volume = %d\n", spsc_ctrl->spsc_para->volume);
			GUI_ResetTimer(msg->h_deswin, Timer_SpscStaId, DELAY_SPSCSTAID, 0);
			last_key = msg->dwAddData1;
			break;
		}
		case GUI_MSG_KEY_RIGHT:
		case GUI_MSG_KEY_LONGRIGHT:
		case GUI_MSG_KEY_IR_RIGHT:
		case GUI_MSG_KEY_LONGIR_RIGHT:
		{
			if (spsc_ctrl->spsc_para->volume < AUDIO_DEVICE_VOLUME_MAX)
			{
				spsc_ctrl->spsc_para->volume++;
				photo_spsc_pait(msg, PAINT_PHOTO_SPSC_VOLUME);
				photo_cmd2parent(msg->h_deswin, CMD_PHOTO_SPSC_VOLUME, spsc_ctrl->spsc_para->volume, 0);
			}
			DEBUG_photo("spsc_ctrl->spsc_para->volume = %d\n", spsc_ctrl->spsc_para->volume);
			GUI_ResetTimer(msg->h_deswin, Timer_SpscStaId, DELAY_SPSCSTAID, 0);
			last_key = msg->dwAddData1;
			break;
		}
		case GUI_MSG_KEY_UP:
		case GUI_MSG_KEY_LONGUP:
		case GUI_MSG_KEY_IR_UP:
		case GUI_MSG_KEY_LONGIR_UP:
		{
			if (spsc_ctrl->spsc_para->play_status ==  PHOTO_STAT_PLAY)
				break;
			if(spsc_ctrl->spsc_para->cur_no <=0)
				spsc_ctrl->spsc_para->cur_no = spsc_ctrl->spsc_para->total_no-1;
			else
				spsc_ctrl->spsc_para->cur_no--;
			spsc_ctrl->spsc_para->play_status = PHOTO_STAT_PREV;
			GUI_ResetTimer(msg->h_deswin, Timer_SpscStaId, DELAY_SPSCSTAID/*100*/, 0);
			photo_spsc_pait(msg, PAINT_PHOTO_SPSC_PLAY_STATUS);
			last_key = msg->dwAddData1;
			break;
		}
		case GUI_MSG_KEY_DOWN:
		case GUI_MSG_KEY_LONGDOWN:
		case GUI_MSG_KEY_IR_DOWN:
		case GUI_MSG_KEY_LONGIR_DOWN:
		{
			if (spsc_ctrl->spsc_para->play_status ==  PHOTO_STAT_PLAY)
				break;
			
			if(spsc_ctrl->spsc_para->cur_no >= spsc_ctrl->spsc_para->total_no-1)
				spsc_ctrl->spsc_para->cur_no = 0;
			else
				spsc_ctrl->spsc_para->cur_no++;
			spsc_ctrl->spsc_para->play_status = PHOTO_STAT_NEXT;
			GUI_ResetTimer(msg->h_deswin, Timer_SpscStaId, DELAY_SPSCSTAID/*100*/, 0);
			photo_spsc_pait(msg, PAINT_PHOTO_SPSC_PLAY_STATUS);
			last_key = msg->dwAddData1;
			break;
		}
		case GUI_MSG_KEY_ENTER:
		case GUI_MSG_KEY_LONGENTER:
		{
			key_cnt++;
			last_key = msg->dwAddData1;

			break;
		}
		case GUI_MSG_KEY_MENU:
		{
			last_key = msg->dwAddData1;
			break;
		}
		case GUI_MSG_KEY_ESCAPE:
		{
			last_key = msg->dwAddData1;
			break;
		}

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
			if (spsc_ctrl->spsc_para->play_status ==  PHOTO_STAT_PLAY)
				break;
			
			last_key = msg->dwAddData1;
			break;
		}
		
		default:
			break;
	}
	//////////////////////////////////////////////////////////////////////////
	if (KEY_UP_ACTION == msg->dwAddData2)
	{
		if (last_key == GUI_MSG_KEY_ESCAPE)
		{
            ANOLE_BackNormal();//����Ϊ�ǲ���״̬�����Զ������̲߳���ʱֱ���˳�
			photo_cmd2parent(msg->h_deswin, CMD_PHOTO_SPSC_CLOSE, 0, 0);
		}
		else if (last_key == GUI_MSG_KEY_LONGUP ||last_key == GUI_MSG_KEY_UP 
			||last_key == GUI_MSG_KEY_DOWN ||last_key == GUI_MSG_KEY_LONGDOWN
			|| last_key == GUI_MSG_KEY_IR_UP || last_key == GUI_MSG_KEY_LONGIR_UP
			|| last_key == GUI_MSG_KEY_IR_DOWN || last_key == GUI_MSG_KEY_LONGIR_DOWN)
		{
			GUI_ResetTimer(msg->h_deswin, Timer_SpscStaId, DELAY_SPSCSTAID/*100*/, 0);
			photo_spsc_pait(msg, PAINT_PHOTO_SPSC_PLAY_STATUS);
			photo_cmd2parent(msg->h_deswin, CMD_PHOTO_SPSC_PLAYFILE, spsc_ctrl->spsc_para->cur_no, 0);
		}
		else if (last_key == GUI_MSG_KEY_ENTER || last_key == GUI_MSG_KEY_LONGENTER)
		{
			if (key_cnt <5)
			{

			
				spsc_ctrl->spsc_para->cur_no = ANOLE_GetCurPicIndex();
				spsc_ctrl->spsc_para->total_no = ANOLE_GetPicNum();

				if(spsc_ctrl->spsc_para->hdigit_layer)
				{

					photo_delete_digit_layer(msg->h_deswin);
					if((spsc_ctrl->spsc_para->cur_no != (spsc_ctrl->spsc_para->ndigit_page-1)) && \
						(spsc_ctrl->spsc_para->total_no >= spsc_ctrl->spsc_para->ndigit_page))
					{
						spsc_ctrl->spsc_para->cur_no = spsc_ctrl->spsc_para->ndigit_page - 1;
						photo_spsc_pait(msg, PAINT_PHOTO_SPSC_PLAY_STATUS);
						if(spsc_ctrl->spsc_para->ndigit_page > 0)
							photo_cmd2parent(msg->h_deswin, CMD_PHOTO_SPSC_PLAYFILE, spsc_ctrl->spsc_para->ndigit_page-1, 0);
					}
					return;
				}
				
				if (spsc_ctrl->spsc_para->play_status == PHOTO_STAT_PLAY)
				{
					spsc_ctrl->spsc_para->play_status = PHOTO_STAT_PAUSE;
					GUI_ResetTimer(msg->h_deswin, Timer_SpscStaId, DELAY_SPSCSTAID, 0);
					photo_cmd2parent(msg->h_deswin, CMD_PHOTO_SPSC_PAUSE, 0, 0);
					photo_spsc_pait(msg, PAINT_PHOTO_SPSC_INFO);
				}
				else
				{
					spsc_ctrl->spsc_para->play_status = PHOTO_STAT_PLAY;
					GUI_ResetTimer(msg->h_deswin, Timer_SpscStaId, DELAY_SPSCSTAID, 0);
					photo_cmd2parent(msg->h_deswin, CMD_PHOTO_SPSC_PLAY, 0, 0);
					photo_spsc_pait(msg, PAINT_PHOTO_SPSC_PLAY_STATUS);
				}
			}
			else
			{
				photo_spsc_hide(spsc_ctrl);
				spsc_ctrl->disp_type = PAINT_PHOTO_SPSC_NULL;
				photo_cmd2parent(msg->h_deswin, CMD_PHOTO_SPSC_SET, (__s32)spsc_ctrl->spsc_para, 0);
			}
		}
#if BEETLES_KEY_REDEF
		else if (msg->dwAddData1== GUI_MSG_KEY_MENU)
#else
		else if (last_key == GUI_MSG_KEY_MENU)
#endif
		{
			photo_spsc_hide(spsc_ctrl);
			spsc_ctrl->disp_type = PAINT_PHOTO_SPSC_NULL;
			DEBUG_SYSMEM();
			photo_cmd2parent(msg->h_deswin, CMD_PHOTO_SPSC_SET, (__s32)spsc_ctrl->spsc_para, 0);
		}

		else if(last_key == GUI_MSG_KEY_NUM0 || last_key == GUI_MSG_KEY_NUM1 || 
			   last_key == GUI_MSG_KEY_NUM2 || last_key == GUI_MSG_KEY_NUM3 || 
			   last_key == GUI_MSG_KEY_NUM4 || last_key == GUI_MSG_KEY_NUM5 ||
			   last_key == GUI_MSG_KEY_NUM6 || last_key == GUI_MSG_KEY_NUM7 || 
			   last_key == GUI_MSG_KEY_NUM8 || last_key == GUI_MSG_KEY_NUM9)
		{
			__s32 cur_val;

			if(NULL == spsc_ctrl->spsc_para->hdigit_layer)
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
				spsc_ctrl->spsc_para->hdigit_layer = com_layer_create(&rect, 1, PIXEL_COLOR_ARGB8888, GUI_LYRWIN_STA_ON, "digit layer");
				if(NULL == spsc_ctrl->spsc_para->hdigit_layer)
				{
					__wrn("digit layer create fail.....\n");
				}
				else
				{
					__wrn("digit layer create ok....\n");
				}
			  }

				__here__;
			  if(!GUI_IsTimerInstalled(msg->h_deswin, spsc_ctrl->spsc_para->digit_timmer_id))
			  {
				spsc_ctrl->spsc_para->ndigit_page = 0;
				GUI_SetTimer(msg->h_deswin, spsc_ctrl->spsc_para->digit_timmer_id, DIGIT_TIME_CNT, NULL);
			  }
			  else
			  {
				GUI_ResetTimer(msg->h_deswin, spsc_ctrl->spsc_para->digit_timmer_id, DIGIT_TIME_CNT, NULL);
			  }
					
			   cur_val = msg->dwAddData1 - GUI_MSG_KEY_NUM0;

			   if(spsc_ctrl->spsc_para->ndigit_page < 1000)
			   {
					spsc_ctrl->spsc_para->ndigit_page *= 10;
					spsc_ctrl->spsc_para->ndigit_page +=cur_val;
			   }
			   else
				{
				spsc_ctrl->spsc_para->ndigit_page =cur_val;
				}
			   
			eLIBs_printf("------ndigit_page = %d\n", spsc_ctrl->spsc_para->ndigit_page);

			photo_digit_layer_draw(spsc_ctrl);
		}
		key_cnt = 0;	
		last_key = 0xffffffff;
	}
	//////////////////////////////////////////////////////////////////////////
	return 0;
}
/*
	�ص�����
*/
static __s32 photo_spsc_Proc(__gui_msg_t *msg)
{
    __here__;
    DEBUG_CBMSGEx(photo_spsc_Proc);
    __here__;
	switch(msg->id)
	{
	case GUI_MSG_CREATE:
		{
			photo_spsc_ctrl_t *spsc_ctrl;
			photo_spsc_para_t *spsc_para;

			spsc_para = (photo_spsc_para_t *)GUI_WinGetAttr(msg->h_deswin);
			BallocType(spsc_ctrl, photo_spsc_ctrl_t);
			spsc_ctrl->spsc_para = spsc_para;
			photo_spsc_init(spsc_ctrl);
			spsc_ctrl->txt_color = APP_COLOR_WHITE;

			GUI_SetTimer(msg->h_deswin, Timer_SpscStaId, DELAY_SPSCSTAID, 0);
			GUI_WinSetAddData(msg->h_deswin, (__u32)spsc_ctrl);
			//////////////////////////////////////////////////////////////////////////
			{
				anole_set_play_order(/*ANOLE_PLAY_NEXT*/ANOLE_PLAY_RANDOM);
				//anole_npl_playing_seek(0);
				//ANOLE_GetCurPicIndex();
				
				//ANOLE_Play();		//�Զ�����
				
				//spsc_ctrl->disp_type = PAINT_PHOTO_SPSC_PLAY_STATUS;
				if (ANOLE_GetSta() == ANOLE_MODE_PLAY)
				{
					spsc_ctrl->spsc_para->play_status = PHOTO_STAT_PLAY;
				}
				else
				{
					spsc_ctrl->spsc_para->play_status = PHOTO_STAT_PAUSE;
				}
			}
			//////////////////////////////////////////////////////////////////////////
#ifdef SUPPORT_DIGIT_KEY
	spsc_ctrl->spsc_para->digit_timmer_id = 0x16;  //cai++ for digit_num
	spsc_ctrl->spsc_para->hdigit_layer = NULL;	
#endif

	}
		return EPDK_OK;
	case GUI_MSG_CLOSE:
		{
			GUI_FrmWinDelete(msg->h_deswin);
		}
		return EPDK_OK;
	case GUI_MSG_DESTROY:
	{
		photo_spsc_ctrl_t *spsc_ctrl;
		spsc_ctrl = (photo_spsc_ctrl_t *)GUI_WinGetAddData(msg->h_deswin);
		photo_spsc_uninit(spsc_ctrl);

		if(spsc_ctrl->spsc_para->hdigit_layer)
		{
			photo_delete_digit_layer(msg->h_deswin);
		}
		
		GUI_KillTimer(msg->h_deswin, Timer_SpscStaId);
		BFreeType(spsc_ctrl->spsc_para, photo_spsc_para_t);
		BFreeType(spsc_ctrl, photo_spsc_ctrl_t);
	}
		return EPDK_OK;
	case GUI_MSG_PAINT:
		
		return EPDK_OK;
	case GUI_MSG_KEY:
		photo_spsc_key_proc(msg);
		return EPDK_OK;
	case GUI_MSG_TOUCH:
		break;
	case GUI_MSG_TIMER:
		{
			photo_spsc_ctrl_t *spsc_ctrl;
			spsc_ctrl = (photo_spsc_ctrl_t *)GUI_WinGetAddData(msg->h_deswin);

			if (spsc_ctrl->disp_type == PAINT_PHOTO_SPSC_NULL)
				break;

			if (spsc_ctrl->spsc_para->play_status == PHOTO_STAT_NEXT || spsc_ctrl->spsc_para->play_status == PHOTO_STAT_PREV)
			{
				photo_cmd2parent(msg->h_deswin, CMD_PHOTO_SPSC_GET_INFO, (__s32)spsc_ctrl->spsc_para, 0);
				if (spsc_ctrl->spsc_para->play_status == PHOTO_STAT_PLAY)
				{
					photo_spsc_pait(msg, PAINT_PHOTO_SPSC_PLAY_STATUS);
					GUI_ResetTimer(msg->h_deswin, Timer_SpscStaId, DELAY_SPSCSTAID, 0);
				}
				else
				{
					photo_spsc_pait(msg, PAINT_PHOTO_SPSC_INFO);
					GUI_ResetTimer(msg->h_deswin, Timer_SpscStaId, DELAY_SPSCSTAID, 0);
				}
			}
			else if (spsc_ctrl->disp_type == PAINT_PHOTO_SPSC_INFO)
			{
				photo_spsc_pait(msg, PAINT_PHOTO_SPSC_PLAY_STATUS);
			}
			else if (spsc_ctrl->disp_type == PAINT_PHOTO_SPSC_VOLUME)
			{
// 				if (spsc_ctrl->spsc_para->play_status == PHOTO_STAT_PLAY)
// 				{
// 					photo_spsc_pait(msg, PAINT_PHOTO_SPSC_INFO);
// 					GUI_ResetTimer(msg->h_deswin, Timer_SpscStaId, DELAY_SPSCSTAID, 0);
// 				}
// 				else
// 				{
// 					photo_spsc_pait(msg, PAINT_PHOTO_SPSC_PLAY_STATUS);
// 					GUI_ResetTimer(msg->h_deswin, Timer_SpscStaId, DELAY_SPSCSTAID, 0);
// 				}
			}
			else if (spsc_ctrl->spsc_para->play_status == PHOTO_STAT_PLAY)
			{
				photo_spsc_pait(msg, PAINT_PHOTO_SPSC_NULL);
			}
#ifdef SUPPORT_DIGIT_KEY
			else if(spsc_ctrl->spsc_para->digit_timmer_id && (spsc_ctrl->spsc_para->digit_timmer_id == msg->dwAddData1))
			{
				photo_delete_digit_layer(msg->h_deswin);
				spsc_ctrl->spsc_para->cur_no = ANOLE_GetCurPicIndex();
				spsc_ctrl->spsc_para->total_no = ANOLE_GetPicNum();
				
				if((spsc_ctrl->spsc_para->cur_no != (spsc_ctrl->spsc_para->ndigit_page-1)) && \
					(spsc_ctrl->spsc_para->total_no > spsc_ctrl->spsc_para->ndigit_page))
				{
					spsc_ctrl->spsc_para->cur_no = spsc_ctrl->spsc_para->ndigit_page - 1;
					photo_spsc_pait(msg, PAINT_PHOTO_SPSC_PLAY_STATUS);
					if(spsc_ctrl->spsc_para->ndigit_page > 0)
						photo_cmd2parent(msg->h_deswin, CMD_PHOTO_SPSC_PLAYFILE, spsc_ctrl->spsc_para->ndigit_page-1, 0);
				}
				return;
			}
#endif			
		}
		return EPDK_OK;
	case GUI_MSG_COMMAND:
		break;
	case GUI_MSG_WIN_WAKEUP:
		GUI_InvalidateRect(msg->h_deswin, NULL, EPDK_TRUE);
		return EPDK_OK;
	case MSG_PHOTO_SPSC_SET_NO:
		{
			photo_spsc_ctrl_t *spsc_ctrl;
			spsc_ctrl = (photo_spsc_ctrl_t *)GUI_WinGetAddData(msg->h_deswin);

			spsc_ctrl->spsc_para->cur_no = msg->dwAddData1;
			spsc_ctrl->spsc_para->total_no = msg->dwAddData2;
			if (spsc_ctrl->disp_type == PAINT_PHOTO_SPSC_PLAY_STATUS)
				photo_spsc_pait(msg, PAINT_PHOTO_SPSC_PLAY_STATUS);
			return EPDK_OK;
		}
	default:
		break;
	}
	
	return GUI_FrmWinDefaultProc(msg);	
}

H_WIN photo_spsc_win_create(H_WIN h_parent, photo_spsc_para_t *para)
{
	__gui_framewincreate_para_t framewin_para;
	photo_spsc_para_t *spsc_para;
	FB fb;

	BallocType(spsc_para, photo_spsc_para_t);
	eLIBs_memset(spsc_para, 0, sizeof(photo_spsc_para_t));

	eLIBs_memcpy(spsc_para, para, sizeof(photo_spsc_para_t));

	spsc_para->isShowVolume = 0;
	
	GUI_LyrWinGetFB(para->layer, &fb);

	eLIBs_memset(&framewin_para, 0, sizeof(__gui_framewincreate_para_t));
	framewin_para.name          = "photo spsc win";
	framewin_para.dwExStyle     = WS_EX_NONE;
	framewin_para.dwStyle       = WS_NONE|WS_VISIBLE;
	framewin_para.spCaption     =  NULL;
	framewin_para.hOwner	    = NULL;
	framewin_para.id            = PHOTO_SPSC_ID;
	framewin_para.hHosting      = h_parent;
	framewin_para.FrameWinProc  = (__pGUI_WIN_CB)esKRNL_GetCallBack((__pCBK_t)photo_spsc_Proc);
	framewin_para.rect.x        = 0;
	framewin_para.rect.y        = 0;
	framewin_para.rect.width    = fb.size.width;
	framewin_para.rect.height   = fb.size.height;
	framewin_para.BkColor.alpha =  0;
	framewin_para.BkColor.red   = 0;
	framewin_para.BkColor.green = 0;
	framewin_para.BkColor.blue  = 0;
	framewin_para.attr          = (void *)spsc_para;
	framewin_para.hLayer        = para->layer;

	return (GUI_FrmWinCreate(&framewin_para));
}

__s32 photo_spsc_win_delete(H_WIN spsc_win)
{
	GUI_FrmWinDelete(spsc_win);
	return EPDK_OK;
}

