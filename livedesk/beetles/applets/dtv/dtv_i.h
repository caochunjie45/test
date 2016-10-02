/*
*********************************************************************************************************
*											        ePDK
*						            the Easy Portable/Player Develop Kits
*									           willow app sample
*
*						        (c) Copyright 2006-2007, JOHN, China
*											All	Rights Reserved
*
* File    : movie_i.h
* By      : john.fu
* Version : V1.00
*********************************************************************************************************
*/
#ifndef _DVBPLAY_I_H_
#define _DVBPLAY_I_H_


#include "epdk.h"
#include "beetles_app.h"
#include "dtv.h"
#include "dtv_mid.h"
#include "dtv_singal.h"
#include "program_preview.h"
#include "schedule.h"
#include "scan_proc.h"
#include "dtv_setting.h"
#include "dtv_subtitle_ui.h"
#include "app_loading.h"
#include "audio_man.h"
#include "digit.h"
#include "dtv_paint.h"

//#include "dtv_ui.h"

#include "dtv_uipara.h"

#define SUBMENU_DVB_SCAN            "SCAN SUB MENU"        //搜索

#define SUBMENU_DVB_SCHUDULE        "SCHUDULE  MENU"    // 节目安排
#define SUBMENU_DVB_SCHUDULE_LST        "SCHUDULE SUB MENU LIST"    // 

#define SUBMENU_DVB_PREVIEW        "PREVIEW  MENU"    // 预览
#define SUBMENU_DVB_PREVIEW_LST    "PREVIEW SUB MENU LIST"    //

#define SUBMENU_DVB_TRACE        "SETTING SUB TRACK" 
#define SUBMENU_DVB_TRACE_LIST        "SETTING SUB TRACK LIST" 


#define SUBMENU_DVB_SETTING             "SETTING MENU"    //设置
#define SUBMENU_DVB_SETTING_LST        "SETTING SUB MENU LIST"    
#define SUBMENU_DVB_SETTING_DISP        "SETTING SUB MENU DISP" 
#define SUBMENU_DVB_SETTING_LCD        "SETTING SUB MENU LCD" 
#define SUBMENU_DVB_SETTING_CA        "SETTING SUB MENU CA" 
#define SUBMENU_DVB_SETTING_LCN        "SETTING SUB MENU LCN" 
#define SUBMENU_DVB_SETTING_ANTENNA     "SETTING SUB MENU ANTENNA"


#if EN_DTV_RECORD
#define SUBMENU_DVB_SETTING_DTVREC	"SETTING SUB MENU DTVREC"   //电视录制
#endif

#define SUBMENU_DTV_SG_STRENGTH        "SINGAL STRENGTH"


#define DTV_SUBTITLE                   "DTV SUBTITLE"
#define SUBTITLE_FRMWIN                "SUBTITLE FRMWIN"

#define DIGIT                          "DIGIT"

#define SUBMENU_DVB_SCAN_ID                     (APP_DTV_ID + 0x10)

#define SUBMENU_DVB_SCHUDULE_ID                 (APP_DTV_ID + 0x20)
#define SUBMENU_DVB_SCHUDULE_LIST_ID            (SUBMENU_DVB_SCHUDULE_ID + 0x01)
#define SUBMENU_DVB_SCHUDULE_TIMER_ID           (SUBMENU_DVB_SCHUDULE_ID + 0x02)
#define SUBMENU_DVB_SCHUDULE_DESTROY_TIMERID    (SUBMENU_DVB_SCHUDULE_ID + 0x03)


#define SUBMENU_DVB_PREVIEW_ID			        (APP_DTV_ID + 0x30)
#define SUBMENU_DVB_PREVIEW_LIST_ID             (SUBMENU_DVB_PREVIEW_ID + 0x01)
#define SUBMENU_DVB_PREVIEW_TIMERID             (SUBMENU_DVB_PREVIEW_ID + 0x02)
#define SUBMENU_DVB_PREVIEW_UPDATEID            (SUBMENU_DVB_PREVIEW_ID + 0x03)


#define SUBMENU_DVB_SETTING_ID                  (APP_DTV_ID + 0x40)
#define SUBMENU_DVB_SETTING_LIST_ID             (SUBMENU_DVB_SETTING_ID + 0x01)
#define SUBMENU_DVB_SETTING_DISP_ID             (SUBMENU_DVB_SETTING_ID + 0x02)
#define SUBMENU_DVB_SETTING_LCD_ID              (SUBMENU_DVB_SETTING_ID + 0x03)
#define SUBMENU_DVB_SETTING_TIMERID             (SUBMENU_DVB_SETTING_ID + 0x04)
#define SUBMENU_DVB_SETTING_LCNID               (SUBMENU_DVB_SETTING_ID + 0x05)
#define SUBMENU_DVB_SETTING_ANTENNAID           (SUBMENU_DVB_SETTING_ID + 0x06)


#if EN_DTV_RECORD
#define DVB_DTVREC_ID	                (SUBMENU_DVB_SETTING_ID + 0x07)
#endif

#define SUBMENU_DVB_TRACKID                     (APP_DTV_ID + 0x50)
#define SUBMENU_DVB_TRACE_LIST_ID               (SUBMENU_DVB_TRACKID + 0x01)
#define SUBMENU_DVB_TRACE_TIMERID               (SUBMENU_DVB_TRACKID + 0x02)


#define SUBMENU_DVB_SINGAL_ID                   (APP_DTV_ID + 0x60)
#define DVB_SINGAL_TIMER_ID                     (SUBMENU_DVB_SINGAL_ID + 0x01)//信号状态定时器，控制频道播放、弱信号时的处理

#define SUBMENU_DVB_CA_ID                       (APP_DTV_ID + 0x70)


#define DTV_SUBTITLE_ID                         (APP_DTV_ID + 0x80)
#define SUBTITLE_FRMWIN_ID                      (DTV_SUBTITLE_ID + 0x01)


#define  DTV_TVOUT_ID                           (APP_DTV_ID + 0x90)
#define  DVB_LOADING_ID		                    (DTV_TVOUT_ID+1)      //启动加载动画定时器
#define  DVB_PROT_DIALOG_ID		                (DTV_TVOUT_ID+3)      //no use any more
#define  DVB_FLUSH_ID			                (DTV_TVOUT_ID+6)

#define  DTV_DIALOG_ESCAPE_ID                   (DTV_TVOUT_ID + 7)
#define  DTV_DIALOG_RECORD_ID                   (DTV_TVOUT_ID + 8)
#define  DTV_DIALOG_CHANNEL_ERROR_ID            (DTV_TVOUT_ID + 9)
#define  DTV_DIALOG_SWITCH_EXTERNAL_ID          (DTV_TVOUT_ID + 10)
#define  DTV_DIALOG_SWITCH_ANTENNA_ID           (DTV_TVOUT_ID + 11)


#define DIGIT_ID                               (APP_DTV_ID + 0xA0)

enum 
{
	DTV_RESEARCH_SERVICE_LIST = GUI_MSG_USER_DEF+0x2000,//重新搜台
	DTV_PREVIEW_SERVICE_LIST,        //进预览界面
	DTV_BACK2PLAY_PROGRAM,          //回归正常播放状态

	DTV_CLOSE_DIALOG,//废弃
	DTV_EPG,//废弃
    DTV_AUDIO,

	CMD_TO_UPDATE_SINGAL_STATUS, //更新信号状态
	CMD_TO_UPDATE_VOLUME, //更新音量进度条
	CMD_TO_UPDATE_SYSTIME, //更新系统时间

	CMD_TO_DELETE_SEARCH_THREAD, //删除搜台线程

	CMD_FS_PART_PLUGOUT,
	
    CMD_SWITCH_TVOUT_MODE,//切换TVOUT

	CMD_CHANGE_CHANNEL,
	
	CMD_NUMBER_SELECT_CHANNAL,

	CMD_SWITCH_EXTERNAL,
};


enum 
{
	DTV_SIGNAL_LAYER_OFF = 0,       // 完全关闭信号状态条
	DTV_SIGNAL_BATTERY,             // 仅显示电池状态
	DTV_SIGNAL_LAYER_ON,            // 显信号状态示和电池状态
	DTV_SIGNAL_UPDATE
};

#endif/*_MOVIE_I_H_*/

