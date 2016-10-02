/*
**************************************************************************************************************
*											         ePDK
*						            the Easy Portable/Player Develop Kits
*									           desktop system 
*
*						        	 (c) Copyright 2007-2010, ANDY, China
*											 All Rights Reserved
*
* File    	: headbar_com.h
* By      	: Andy.zhang
* Func		: headbar manage
* Version	: v1.0
* ============================================================================================================
* 2009-7-20 8:51:52  andy.zhang  create this file, implements the fundemental interface;
**************************************************************************************************************
*/

#ifndef __HEADBAR_COM_H__
#define __HEADBAR_COM_H__

#define H_APP_ROOT		"beetles"
#define H_APP_HOME		"app_home"

#define H_APP_MAINMENU_ID			0x1001
#define H_APP_EXPLORER_ID		0x2001
#define H_APP_MOVIE_ID			0x3001
#define H_APP_MUSIC_ID			0x4001
#define H_APP_PHOTO_ID			0x5001
#define H_APP_FM_ID				0x6001
#define H_APP_SETTING_ID		0x7001
#define H_APP_EBOOK_ID			0x8001
#define H_APP_CALENDAR_ID		0x9001
#define H_APP_RECORD_ID		0xA001
#define H_APP_LINEIN_ID			0xB001
#if EXTERN_KARAOKE_EXIST
#define H_APP_KARAOKE_ID     	0xC001
#endif

typedef enum
{    
    H_SWITCH_TO_OTHER_APP = 0,
    H_SWITCH_TO_MMENU,
    H_SWITCH_TO_EXPLORER,
    H_OPEN_BG_MUSIC,
    H_CLOSE_BG_MUSIC,
    H_SWITCH_TO_LINE_IN,
}headbar2root_cmd_t;

typedef enum
{	  
    	H_ID_MEDIA_START = 0,//记录movie photo music ebook、ID_MAINMENU_RECORD类型的开始下标
	H_ID_MAINMENU_MUSIC = H_ID_MEDIA_START,
	H_ID_MAINMENU_MOVIE,
	H_ID_MAINMENU_PHOTO,
	H_ID_MAINMENU_EBOOK,
	H_ID_MAINMENU_FM,
#if 1//EXTERN_KARAOKE_EXIST
	H_ID_MAINMENU_KARAOKE,
#endif	
	H_ID_MAINMENU_EXPLORER,
	H_ID_MAINMENU_SETTING,
	H_ID_MAINMENU_CALENDAR,
	H_ID_MAINMENU_LINEIN,
	H_ID_MAINMENU_RECORD,
	H_ID_MEDIA_END,
	H_ID_MAINMENU_OTHERS = H_ID_MEDIA_END,	
	H_ID_MAINMENU_MUTE,
	H_ID_MAINMENU_BACKGROUND,
	H_ID_MAINMENU_TFUSB,
	H_ID_MAINMENU_CLOSESCN,
	H_ID_MAINMENU_POWEROFF,
	H_ID_MAINMENU_RANDOM,
	H_ID_MAX_NUM,
}root_headbar_id_t;

typedef enum
{
	HB_PAINT_INIT,
	HB_PAINT_TIME,
	HB_PAINT_VOL,
	HB_PAINT_BRIGHT,
	HB_PAINT_VOLUME,
	HB_PAINT_TITLE,
	HB_PAINT_DISK,
	HB_PAINT_
}headbar_paint_e;

////////////////////////


/* 命令码 */
#define HBAR_TIME_PAINT				(GUI_MSG_USER_DEF+1)	// 绘制时间
#define HBAR_DATA_PAINT				(GUI_MSG_USER_DEF+2)	// 绘制日期
#define HBAR_VOL_PAINT				(GUI_MSG_USER_DEF+3)	// 绘制电池电量
#define HBAR_BRIGHT_PAINT			(GUI_MSG_USER_DEF+4)	// 绘制亮度
#define HBAR_VOLUME_PAINT			(GUI_MSG_USER_DEF+5)	// 绘制音量
#define HBAR_MUSIC_BG_PAINT			(GUI_MSG_USER_DEF+6)	// 绘制背景音乐
#define HBAR_TITLE_PAINT			(GUI_MSG_USER_DEF+7)	// 绘制标题
#define HBAR_DISK_PAINT				(GUI_MSG_USER_DEF+8)	// 绘制标题
#define HBAR_DISK_REPAINT			(GUI_MSG_USER_DEF+9)	// 绘制标题
#define HBAR_FORCE_VOLUME_PAINT 	(GUI_MSG_USER_DEF+10)	// 按键强制绘制音量

typedef enum _headbar_cmd_id
{		
	HEADBAR_CMD_SET_VOLUME = GUI_MSG_USER_DEF,
	HEADBAR_CMD_SET_BRIGHT,	
	HEADBAR_CMD_SET_FORMAT,
	HEADBAR_CMD_GET_SCN_POS,
	HEADBAR_CMD_GET_SIZE
}headbar_cmd_id;

typedef struct
{
	char*	name;		
	__u16 	id;	
	
	H_WIN   	parent;
	H_WIN   	layer;		
	RECT    	rect;
	GUI_FONT   *font;			// 字体句柄	
}__headbar_create_para;

H_WIN headbar_frm_create(H_WIN parent, H_LYR layer);
__s32 headbar_frm_delete(H_WIN frmwin); 

#endif /* __HEADBAR_COM_H__ */


