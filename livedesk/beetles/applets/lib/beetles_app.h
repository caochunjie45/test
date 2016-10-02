/*
************************************************************************************************************************
*                                                        CUCKOO
*                                                   the Audio Player
*
*                             Copyright(C), 2006-2009, SoftWinners Microelectronic Co., Ltd.
*											       All Rights Reserved
*
* File Name   :ResDecode.h
*
* Author      : Gary.Wang
*
* Version     : 1.1.0
*
* Date        : 2008.11.08
*
* Description :
* 
* Others      : None at present.
*
*
* History     :
*
*  <Author>        <time>       <version>      <description>
*
* Gary.Wang      2008.11.08       1.1.0        build the file
*
*/

#ifndef __BEETLES_APP_H_
#define __BEETLES_APP_H_

#include "apps.h"
#include "lib/common/palette.h"
#include "lib/common/misc.h"
#include "lib/common/mem_dev.h"
#include "lib/sset/toolbar/GlideCurve.h"
#include "lib/sset/toolbar/tbar_com.h"
#include "lib/sset/sset_e.h"
#include "lib/sset/sset_uipara.h"

#include "lib/app_dialog/app_dialog.h"
#include "lib/app_loading/app_loading.h"
#include "lib/fifo_db/fifo_db.h"


//#define __here__  eLIBs_printf("@L%d(%s)\n", __LINE__, __FILE__);test_flush_nor(0);//112350
//#define __msg(...)    		(eLIBs_printf("MSG:L%d(%s):", __LINE__, __FILE__),                 \
//							     eLIBs_printf(__VA_ARGS__));test_flush_nor(0);//112350									        )

#define SUPPORT_MUSIC_BG_MODE 0

#define MOVIE_ENTER_FLUSH 0
#define MUSIC_ENTER_FLUSH 0
#define MOVIE_AUTO_FLUSH_FIRST_TIME 0//(200*10)//10秒钟
#define MUSIC_AUTO_FLUSH_FIRST_TIME 0//(200*10)//10秒钟

#define MOVIE_AUTO_FLUSH_TIME 0//(200*60*20)//20分钟
#define MUSIC_AUTO_FLUSH_TIME 0//(200*60*20)//20分钟


extern GUI_FONT    *SWFFont;

#define APP_ROOT		"beetles"
#define APP_HOME		"app_home"
#define APP_EXPLORER	"app_explorer"
#define APP_MOVIE		"app_movie"
#define APP_MUSIC		"app_music"
#define APP_PHOTO		"app_photo"
#define APP_FM			"app_fm"
#define APP_SETTING		"app_setting"
#define APP_EBOOK		"app_ebook"
#define APP_CALENDAR	"app_calendar"
#define APP_RECORD	    	"app_record"
#define APP_LINEIN		"app_linein"
#define APP_DV	    	"app_dv"
#define APP_DTV  		"APP_DTV"
#define APP_TEST        "APP_TEST"

#if EXTERN_KARAOKE_EXIST
#define APP_KARAOKE     "APP_KARAOKE"
#endif


#define APP_MAINMENU_ID		0x1001
#define APP_EXPLORER_ID		0x2001
#define APP_MOVIE_ID		0x3001
#define APP_MUSIC_ID		0x4001
#define APP_PHOTO_ID		0x5001
#define APP_FM_ID			0x6001
#define APP_SETTING_ID		0x7001
#define APP_EBOOK_ID		0x8001
#define APP_CALENDAR_ID		0x9001
#define APP_RECORD_ID		0x9002
#define APP_LOADING_ID		0x9100
#define APP_LINEIN_ID		0x9200
#define APP_ALARM_ID		0x9300
#define APP_DTV_ID		    0x9400
#define APP_ROOT_ID 		0x9500
#define COM_VOLUME_ID       0x9600
#define APP_DV_ID			0x9700
#define APP_COM_DLG_ID		0x9800

#if ANTENNA_MODE_SELECT
#define APP_ANTENNA_SELECT_ID 0x9900
#endif

#define APP_TEST_ID        0xA001




#if EXTERN_KARAOKE_EXIST
#define TF_ENCRYPTION_EX  0  //karaoke  数据是否使用加密T  卡形式, 0: 不使用加密T  卡 ,1:使用加密T   卡

//full path = 盘符+ 文件夹+ 文件名
#define  KARAOKE_DATA_NAME "kak_s0.kak"
#define  KARAOKE_FOULDER_PATH "rootfs"


typedef enum
{
	ENCRYPTION_PARTION_NULL = 0,
	ENCRYPTION_PARTION_EXIST,
	ENCRYPTION_PARTION_UNKOWN,
}encryption_partion_e;
#endif

#define MUSIC_BACKGROUND_FOR_SETING	(3)

#define MI_ON	0
#define MI_OFF  1

#define AUDIO_FM_MODE	(0)	
#define AUDIO_NOR_MODE	(1)


typedef enum
{    
    SWITCH_TO_OTHER_APP = 0,
    SWITCH_TO_MMENU,
    SWITCH_TO_EXPLORER,
    OPEN_BG_MUSIC,
    CLOSE_BG_MUSIC,
    SWITCH_DISK,
    SWITCH_TO_LINE_IN,
    CLOSE_AUDIO_APP,
}app2root_cmd_t;

#define BMP_LINE_ROUND(x)	(((__u32)x+3)/4*4)

#define BEETLES_TBL_SIZE(a)	(sizeof(a)/sizeof(a[0]))

typedef enum
{	  
    ID_MEDIA_START = 0,//记录movie photo music ebook、ID_MAINMENU_RECORD类型的开始下标
	ID_MAINMENU_TV=ID_MEDIA_START,
	ID_MAINMENU_MOVIE,
	ID_MAINMENU_MUSIC,
	ID_MAINMENU_PHOTO,
	ID_MAINMENU_EBOOK,
	ID_MAINMENU_FM,
#if 1////EXTERN_KARAOKE_EXIST
	ID_MAINMENU_KARAOKE,
#endif	
	ID_MAINMENU_RECORD,
	ID_MAINMENU_EXPLORER,
	ID_MAINMENU_SETTING,
	ID_MAINMENU_CALENDAR,
	ID_MAINMENU_LINEIN,
	ID_MEDIA_END,
	ID_MAINMENU_OTHERS = ID_MEDIA_END,	
	ID_MAINMENU_MUTE,
	ID_MAINMENU_BACKGROUND,
	ID_MAINMENU_TFUSB,
	ID_MAINMENU_CLOSESCN,
	ID_MAINMENU_POWEROFF,
	ID_MAINMENU_RANDOM,
	ID_MAX_NUM,
}root_mainmenu_id_t;

typedef enum
{
	ID_FM_FMPLAY=0,
	ID_FM_FMSRH,
}root_fm_id_t;

typedef enum
{
	ID_EXPLORER_ALL=0,
	ID_EXPLORER_MOVIE,
	ID_EXPLORER_PHOTO,	
	ID_EXPLORER_MUSIC,
	ID_EXPLORER_EBOOK,
#if EXTERN_KARAOKE_EXIST // add by hot.ee
	ID_EXPLORER_KARAOKE,
#endif	
	ID_EXPLORER_RESTORE,
}root_explorer_id_t;

typedef enum
{
	EXPLR_SW_TO_MAIN=0,
	EXPLR_SW_TO_MOVIE,
	EXPLR_SW_TO_PHOTO,	
	EXPLR_SW_TO_MUSIC,
	EXPLR_SW_TO_EBOOK,
#if EXTERN_KARAOKE_EXIST // add by hot.lee
	EXPLR_SW_TO_KARAOKE,
#endif
}explorer_sw_to_t;

typedef enum
{
	ID_OTHERS_EXPLORER=0,
	ID_OTHERS_EBOOK,
	ID_OTHERS_CALENDAR,
}root_others_id_t;
typedef enum karaoke_win_msg_s
{
	APP_KARAOKE_ENTER_ALPHABER_FRM = 0,
	APP_KARAOKE_ENTER_DIGITAL_FRM,
	APP_KARAOKE_ENTER_SIGNER_FRM,   

	KARAOKE_MENU_FRM_TO_SINGER_FRM,//进入歌星类型选择界面，此为第一级子菜单
	KARAOKE_SINGER_FRM_TO_MENU_FRM,//从歌星类型选择界面返回
	KARAOKE_SINGER_FRM_TO_SUBMENU_FRM,//从歌星类型选择界面进入歌手选择，即进入第二级子菜单
	KARAOKE_SUBMENU_FRM_TO_SINGER_FRM,//从歌手选择界面返回歌星类型选择界面

	KARAOKE_MENU_FRM_TO_ALPHABET_FRM,
	KARAOKE_ALPHABET_FRM_TO_MENU_FRM,		

	KARAOKE_MENU_FRM_TO_DIGITAL_FRM,
	KARAOKE_DIGITAL_FRM_TO_MENU_FRM,	

	KARAOKE_MENU_FRM_TO_TOP10_FRM,
	KARAOKE_TOP10_FRM_TO_MENU_FRM,

	KARAOKE_MENU_FRM_TO_NEWSONG_FRM,
	KARAOKE_NEWSONG_FRM_TO_MENU_FRM,

	KARAOKE_MENU_FRM_TO_FAVOR_FRM,
	KARAOKE_FAVOR_FRM_TO_MENU_FRM,
	
	KARAOKE_MENU_FRM_TO_LIST_FRM,
	KARAOKE_LIST_FRM_TO_MENU_FRM,
	
	KARAOKE_SIGNER_FRM_TO_MENU_FRM,

	KARAOKE_SINGER_MANAGER_TO_MENU_FRM, // 30234384
	KARAOKE_SINGER_MANAGER_TO_LIST_FRM, // 30234384
	
	APP_KARAOKE_MAINWIN_CLOSE,
	APP_KARAOKE_FRMWIN_CLOSE,
	KARAOKE_MENU_MAINWIN_CLOSE,
	KARAOKE_MENU_ALPHABET_FRM_CLOSE,
	KARAOKE_MENU_DIGITAL_FRM_CLOSE,


	KARAOKE_LIST_FRM_CUT_SONG,
	KARAOKE_LIST_FRM_SET_TOP,
	KARAOKE_LIST_FRM_DEL_ONE_SONG,

	KARAOKE_PLAY_FAVOR_SONG,  //在favor  frm  点击确定播放选中的favor  song
	KARAOKE_CLOSE_MENU_MAINWIN,

	KARAOKE_PLAY_NEW_SONG,    //newsong frm  向app  karaoke  发送播放新歌命令
	
	KARAOKE_DELETE_MENU_CMD,   //通知10s  后删除菜单
	
	KARAOKE_SWITCH_ROOT_TYPE,

	//KARAOKE_ERROR_RETURN_HOME, //karaoke  非正常退出
	
	KARAOKE_WIN_MSG_MAX_
}karaoke_win_msg_t;

typedef enum
{
	ID_SETTING_COMMON=0,
	ID_SETTING_POWER,
	ID_SETTING_TIME,
	ID_SETTING_MULTIMEDIA,
}root_setting_id_t;

enum{
	MENU_PAINT_ALL,//draw all
	MENU_PAINT_ITEMSW,//item switch
	MENU_PAINT_VALUESW,//value change
};

typedef enum
{
	SETTING_SW_TO_MAIN=0,	
	SETTING_SW_TO_PROMPT_UPGRADE,
	SETTING_SW_TO_PROMPT_DEFAULT,
	EXPLR_SW_TO_UPGRADE,
	SETTING_SW_TO_MULTIMEDIA,
	SETTING_SW_TO_POWER, //add by tot.lee
	SETTING_SW_TO_GEN_MAN,
	
}setting_sw_to_t;

typedef enum
{
	FM_SW_TO_RECORD=0,
}fm_sw_to_t;

typedef enum
{
	RECORD_SW_TO_FM=0,
}record_sw_to_t;


//
//MUSICPLAYER_NORMAL: 		正常进入音乐播放器
//MUSICPLAYER_BACKGROUND: 	音乐播放器作为后台播放
typedef enum 
{
	MUSICPLAYER_NORMAL = 0,
	MUSICPLAYER_BACKGROUND = 1,	
	MUSICPLAYER_BACKGROUND_HOME = 2,

	MUSICPLAYER_MUSIC_PAUSE,
	MUSICPLAYER_MUSIC_PLAY,
}musicplayer_mode_e;

typedef enum
{
	KEY_SW_TO_APP  = 0x11,
	KEY_SW_TO_FM,
	KEY_SW_TO_RECORD,
	KEY_SW_TO_MOVIE,
	KEY_SW_TO_MUSIC,
	KEY_SW_TO_EBOOK,
	KEY_SW_TO_PHOTO,
	KEY_SW_TO_SETTING,
}key_sw_to_app;

typedef enum 
{
	LINEIN_MODE_NORMAL = 0,
	LINEIN_MODE_BACKGROUND,
	LINEIN_MODE_INVALID
}linein_mode_e;

typedef enum
{
    MOVIE_MODE_NORMAL = 0,//
    MOVIE_MODE_TV,
    MOVIE_MODE_DESKTOP,
}__movie_create_mode_e;

typedef struct tag_root_para
{
	GUI_FONT*	font;
	H_WIN		h_parent;
	H_WIN		src_win;
	rat_root_t 	explr_root;
	char  		music_file_name[RAT_MAX_FULL_PATH_LEN];
	
	__s32		data;   
	__s32		lan_type;
	__u32		root_type; // //新插入的设备	
	__u32		root_type_cnt;////设备个数
	__u32 		setting_opt;//为0代表通用设置，为1代表电源管理
	__u32 		record_opt;//为0代表正常进入录音，为1代表fm录音

	__s32  		explorer_play_music;
}root_para_t;


#define H_THEME HTHEME
#define get_menu_text   dsk_langres_get_menu_text
#define get_menu_len    dsk_langres_get_menu_len
#define theme_open      dsk_theme_open
#define theme_hdl2buf   dsk_theme_hdl2buf
#define theme_hdl2buf_ufo   dsk_theme_hdl2buf_ufo
#define theme_hdl2size  dsk_theme_hdl2size
#define theme_close     dsk_theme_close


//register operations //112350
#define readb(reg)			(*(volatile unsigned char *)(reg))
#define readw(reg)			(*(volatile unsigned short *)(reg))
#define readl(reg)			(*(volatile unsigned long *)(reg))


#endif// __BEETLES_APP_H_
