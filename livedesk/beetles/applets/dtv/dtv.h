/*
*********************************************************************************************************
*											        ePDK
*						            the Easy Portable/Player Develop Kits
*									           calendar app sample
*
*						        	(c) Copyright 2006-2009, ANDY, China
*											 All Rights Reserved
*
* File    : movie.h
* By      :kaiyuanyang@sina.com
* Version : V1.00
*********************************************************************************************************
*/
#ifndef __DVBPLAY_H__
#define __DVBPLAY_H__

#include "beetles_app.h"
#include "dtv_mid.h"
#define  APP_DTV  "APP_DTV"


typedef enum
{
    TV_SPEAKER_OFF,
    
    TV_SPEAKER_ON,
    
}tv_speaker_state;

typedef enum
{
    NOT_CLOSE_VIDEO_LAYER,
    
    CLOSE_VIDEO_LAYER,   
    
}tv_video_layer_state;

typedef enum
{
    NORMAL_RECORD_DESTORY = 0,
    
    FORCE_RECORD_DESTORY,  
    
}tv_record_destory;


enum
{
	CUR_MENU_SEARCH = 1,
	CUR_MENU_SETTING,
	CUR_MENU_PREVIEW,
	CUR_MENU_EPG,	
};

typedef enum
{
	DTV_DESTORY_AUDIO_WIN = 0x00000001,
	DTV_DESTORY_CHANNEL_LIST_WIN = 0x00000002,
	DTV_DESTORY_EPG_WIN = 0x00000004,
	DTV_DESTORY_SETTING_WIN = 0x00000008,
	DTV_DESTORY_SCAN_WIN = 0x00000010,
	DTV_DESTORY_MENU_LAYER = 0x00000020,
	
	DTV_DESTORY_DIALOG_WIN = 0x00000040,
	
	DTV_DESTORY_DIGIT_WIN = 0x00000080,
	DTV_DESTORY_DIGIT_LAYER = 0x00000100,
	
	DTV_DESTORY_SINGAL_LAYER = 0x00000200,
	DTV_CLOSE_SINGAL_LAYER = 0x00000400,
	DTV_DESTORY_SINGAL_WIN = 0x00000800,
	
	DTV_DESTORY_RECORD_LAYER = 0x00001000,
	DTV_CLOSE_RECORD_LAYER = 0x00002000,
	
	DTV_DESTORY_SUBTITLE_LAYER = 0x00004000,
	DTV_CLOSE_SUBTITLE_LAYER = 0x00008000,
	DTV_DESTORY_SUBTITLE_WIN = 0x00010000,

	DTV_DESTORY_ALL_MENU_SCENE = 0x0000003F,
	DTV_DESTORY_DIGIT_SCENE = 0x00000180,
	DTV_DESTORY_SINGAL_SCENE = 0x00000A00,
	DTV_DESTORY_RECORD_SCENE = 0x00001000,
	DTV_DESTORY_SUBTITLE_SCENE = 0x0001C000,

	DTV_DESTORY_ALL_SCENE = 0x0001dbff,
};

#define  MAX_CHARGE_LEVEL  6




#define CMMB_PATH "Z:\\CMMB"
#define CMMB_INDEX_PATH "Z:\\CMMB\\index.html"
#define  SINGAL_Q   20//指示无信号 modify140327 //0    // SHOULD > 7 FOR CMMB
#define  SINGAL_Q_2   60//指示有信号可以播放或者强信号恢复 modify140327
#define  SINGAL_STRENGTH_MAX  96    

#define  KEY_NUM_DELAY    (5000/10)
#define  DTV_FLUSH_DELAY    (100)



extern maple_serv_list_t *service_list;
extern maple_serv_list_t* search_service_list;
extern __s32 g_is_searching;
extern char * sepg_string_c;
extern char * sepg_string_n;
extern __u32  sl_task_id;
extern __bool b_full_screen;
extern __s32 g_strength;
extern __s32 g_quality;
extern __s32 g_weak_singal;
extern __s32 g_record_flag;
extern __mp* maple;


typedef struct dtv_s
{
    __u8        thread_id;

    __hdle      vedio_handl;


	__krnl_event_t* sem;

	
	H_LYR 		h_singal_layer;       //信号状态的图层管理

	H_WIN 		h_singal;
	
	H_LYR 		h_menu_layer;

    H_WIN 		h_scan;             //搜索
    
	H_WIN 		h_serverlist;      //频道列表
	
    H_WIN 		h_epg;             //当前节目列表信息
    
	H_WIN 		h_setting;         //设置界面
	
    H_WIN       h_audio;
    
    H_WIN		h_dialog;

	H_LYR       h_subtitle_layer;
	
	H_WIN		h_subtitle;
	
	H_LYR 		digit_layer;
	
	H_WIN       h_digit_framewin;
	
	H_LYR 		h_destroy_layer;
	
	H_LYR 		h_record_layer;
	
   

	__s32 		cur_user_index;

	__s32 		singal_timer;    //信号检测定时器

	__s32 		vol_timer;


	power_level_e battery_level;
	
	__bool      antenna_state;
	

		
}dtv_ctr_t;


/***********************************************************************************/

__s32  dtv_get_cur_user_index(void);
static H_WIN dvb_man_proc_layer_create( RECT *rect);
 __s32 search_list_finish_callback(void* arg);
 __s32 search_list_event_callback(void* arg);
__s32 search_sepg_finish_callback(void* arg);
static __bool dtv_has_sub_scene(void);
static __bool dtv_is_scan_scene_exsit(void);

static __bool is_on_tv(void);
static __s32 __dtv_switch_output(void);
static __s32 __root_switch_to_tv_or_lcd(__gui_msg_t *msg);
static __s32 dtv_switch_tvout(__gui_msg_t *msg);
static void dtv_check_cmmb_disk_format(void);
static void dtv_init();
static void dtv_uninit(void);
__s32 dtv_singal_win(__s32 flag);
void stop_record(void);
static start_record_proc(__gui_msg_t *msg);
static stop_record_proc(__gui_msg_t *msg);
static __s32 dtv_singal_strength_detect(H_WIN hand);
static __s32 dtv_delete_scene(__u32 scene_id);
static __bool dtv_is_singer_win_exsit(void);
static __s32 dtv_key_proc(__gui_msg_t *msg);
static __s32  dtv_layer_create(void);
static __s32 dtv_search_win_create(__gui_msg_t  *msg);
static __s32 dtv_schedule_win_create(__gui_msg_t  *msg);
static __s32 dtv_program_win_create(__gui_msg_t  *msg);
static __s32 dtv_audio_win_create(__gui_msg_t *msg);
static __s32 dtv_setting_win_create(__gui_msg_t  *msg);
static __s32 dtv_main_create_proc(__gui_msg_t *msg);
static __s32 dtv_command_proc(__gui_msg_t *msg);
static __s32 dtv_main_destroy_proc(__gui_msg_t *msg);
static __s32 dtv_timer_proc(__gui_msg_t *msg);
static __s32 dtv_main_proc(__gui_msg_t  *msg);
static H_WIN dtv_mainwin_create(root_para_t  *para);
__s32 app_dtv_create(root_para_t  *para);


#endif	/* __DVBPLAY_H__ */


