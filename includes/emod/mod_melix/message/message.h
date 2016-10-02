/*
**************************************************************************************************************************
*													        eMOD
*						            		the Easy Portable/Player Develop Kits
*									   				melix message module 
*
*                             		 Copyright(C), 2011-2012, Softwinner Microelectronic Co., Ltd.
*									               All Rights Reserved
*
* File name		:	message.h
* Author		: 	Derek
* Version 		: 	V1.00
* Date    		: 	2011-12-27
* Description	:
* Others 		:
* History 		:
* <Author>           <time>          <version>      <description>
* Derek Lai		    2011-12-27         1.0.0          Create File
*
**************************************************************************************************************************
*/
#ifndef __MESSAGE_H__
#define __MESSAGE_H__

//#define GUI_MSG_USER   0x2000

typedef enum GUI_MSGID
{
    GUI_MSG_TOUCH = 0,
    GUI_MSG_KEY,
    GUI_MSG_SYSTEM,
    GUI_MSG_TIMER,
    GUI_MSG_                            
}gui_msgid_e;

typedef enum GUI_BROADCAST_MSG_TYPE
{
	GUI_BROADCAST_MSG_SYSTEM 	= 0x01,
	GUI_BROADCAST_MSG_USER		= 0x02
}gui_broadcast_msg_type_e;

typedef enum GUI_MSG_KEYID
{
    GUI_MSG_KEY_UP = 0x0,
    GUI_MSG_KEY_DOWN,
    GUI_MSG_KEY_ENTER,
    GUI_MSG_KEY_RIGHT,
    GUI_MSG_KEY_LEFT,
    GUI_MSG_KEY_ESCAPE,
    GUI_MSG_KEY_VADD,
    GUI_MSG_KEY_VDEC,
    GUI_MSG_KEY_RISE,
    GUI_MSG_KEY_POWER,
    GUI_MSG_KEY_SOURCE,
    GUI_MSG_KEY_CLEAR,
    GUI_MSG_KEY_DISPLAY,
    GUI_MSG_KEY_NUM0,
    GUI_MSG_KEY_NUM1,
    GUI_MSG_KEY_NUM2,
    GUI_MSG_KEY_NUM3,
    GUI_MSG_KEY_NUM4,
    GUI_MSG_KEY_NUM5,
    GUI_MSG_KEY_NUM6,
    GUI_MSG_KEY_NUM7,
    GUI_MSG_KEY_NUM8,
    GUI_MSG_KEY_NUM9,
    GUI_MSG_KEY_REPEATE,
    GUI_MSG_KEY_BLOCK,
    GUI_MSG_KEY_PLAY_PAUSE,
    GUI_MSG_KEY_TITLE,
    GUI_MSG_KEY_MENU,
    GUI_MSG_KEY_SETUP,
    GUI_MSG_KEY_GOTO,
    GUI_MSG_KEY_SEL,
    GUI_MSG_KEY_SHIFT,
    GUI_MSG_KEY_DISC,
    GUI_MSG_KEY_ATT,
    GUI_MSG_KEY_POWEROFF,
    GUI_MSG_KEY_RST,
    GUI_MSG_KEY_CANCLE,
    GUI_MSG_KEY_ZOOM_UP,
    GUI_MSG_KEY_ZOOM_DOWN,
    GUI_MSG_KEY_HOLD,
	GUI_MSG_KEY_IR_VADD,
	GUI_MSG_KEY_IR_VDEC,
	GUI_MSG_KEY_IR_MENU,

    GUI_MSG_KEY_LONGUP = 0x50,
    GUI_MSG_KEY_LONGDOWN,
    GUI_MSG_KEY_LONGENTER,
    GUI_MSG_KEY_LONGRIGHT,
    GUI_MSG_KEY_LONGLEFT,
    GUI_MSG_KEY_LONGESCAPE,
    GUI_MSG_KEY_LONGVADD,
    GUI_MSG_KEY_LONGVDEC,
    GUI_MSG_KEY_LONGPOWER,
    GUI_MSG_KEY_LONGSOURCE,
    GUI_MSG_KEY_LONGCLEAR,
    GUI_MSG_KEY_LONGDISPLAY,
    GUI_MSG_KEY_LONGNUM0,
    GUI_MSG_KEY_LONGNUM1,
    GUI_MSG_KEY_LONGNUM2,
    GUI_MSG_KEY_LONGNUM3,
    GUI_MSG_KEY_LONGNUM4,
    GUI_MSG_KEY_LONGNUM5,
    GUI_MSG_KEY_LONGNUM6,
    GUI_MSG_KEY_LONGNUM7,
    GUI_MSG_KEY_LONGNUM8,
    GUI_MSG_KEY_LONGNUM9,
    GUI_MSG_KEY_LONGREPEATE,
    GUI_MSG_KEY_LONGBLOCK,
    GUI_MSG_KEY_LONGPLAY_PAUSE,
    GUI_MSG_KEY_LONGTITLE,
    GUI_MSG_KEY_LONGMENU,
    GUI_MSG_KEY_LONGSETUP,
    GUI_MSG_KEY_LONGGOTO,
    GUI_MSG_KEY_LONGSEL,
    GUI_MSG_KEY_LONGSHIFT,
    GUI_MSG_KEY_LONGDISC,
    GUI_MSG_KEY_LONGATT,
    GUI_MSG_KEY_LONGPOWEROFF,
    GUI_MSG_KEY_LONGRST,
    GUI_MSG_KEY_LONGCANCLE,
    GUI_MSG_KEY_LONGZOOM_UP,
    GUI_MSG_KEY_LONGZOOM_DOWN,
    GUI_MSG_KEY_LONGHOLD,
    GUI_MSG_KEY_LONGIR_VADD,
	GUI_MSG_KEY_LONGIR_VDEC,
	GUI_MSG_KEY_LONGIR_MENU,
    
}gui_msg_keyid_e;

typedef enum GUI_MSG_TOUCHID
{
   GUI_MSG_TOUCH_DOWN = 0x80,
   GUI_MSG_TOUCH_UP,
   GUI_MSG_TOUCH_MOVE,
   GUI_MSG_TOUCH_NCDOWN,
   GUI_MSG_TOUCH_NCUP,
   GUI_MSG_TOUCH_NCMOVE,
   GUI_MSG_TOUCH_OVERDOWN,
   GUI_MSG_TOUCH_OVERUP,
   GUI_MSG_TOUCH_OVERMOVE,
   GUI_MSG_TOUCH_LONGDOWN,
   GUI_MSG_TOUCH_OVERLONGDOWN
}gui_msg_touchid_e;

/*广播消息为系统触发生成的消息*/
typedef enum GUI_BROADCAST_MSG_SYSTEM_E
{
	GUI_MSG_FS_PART_PLUGIN 		= 0x00000001,		// 文件系统分区插入
	GUI_MSG_FS_PART_PLUGOUT 	= 0x00000002,		// 文件系统分区拔出
	GUI_MSG_USBD_PLUG_IN 		= 0x00000004,		// usb device connect pc
	GUI_MSG_USBD_PLUG_OUT 		= 0x00000008,		// usb device plug out pc
	GUI_MSG_APP_EXIT 			= 0x00000010,		// 一键回主界面 KMSG_USR_SYSTEM_SDMMC_PLUGOUT or KMSG_USR_SYSTEM_USBH_PLUGOUT
	GUI_MSG_TVDAC_PLUGIN 		= 0x00000020,		// tvdac plug in
    GUI_MSG_TVDAC_PLUGOUT 		= 0x00000040,		// tvdac plug out
    GUI_MSG_HDMI_PLUGIN 		= 0x00000080,		// hdmi plug in
    GUI_MSG_HDMI_PLUGOUT 		= 0x00000100,		// hdmi plug out
    GUI_MSG_STANDBY_WAKE_UP 	= 0x00000200,		//standby wake up
    GUI_MSG_STANDBY	 			= 0x00000400,		//enter standby
    GUI_MSG_HOLD				= 0x00000800,		// 锁键盘
    GUI_MSG_POWER_OFF			= 0x00001000,		// 关机
    GUI_MSG_LOW_POWER			= 0x00002000,		// 低电
    GUI_MSG_BAN_VOLUME			= 0x00004000,		// 禁音、开音
    GUI_MSG_INSTALL_USBH		= 0x00008000,		// usb host 检测到device插入
    GUI_MSG_UNSTALL_USBH		= 0x00010000,		// usb host 检测到device
    GUI_MSG_SET_AUTO_OFF		= 0x00020000,		// 设置关机时间
    GUI_MSG_SET_SCR_CLOSE		= 0x00040000,		// 设置关屏时间
    GUI_MSG_SCREEN_OPEN			= 0x00080000,		// 通知开屏
    GUI_MSG_SCREEN_CLOSE		= 0x00100000,		// 通知关屏
    GUI_MSG_SCREEN_OP_GATE_ON	= 0x00200000,		// 禁止自动关屏
    GUI_MSG_SCREEN_OP_GATE_OFF	= 0x00400000,		// 允许自动关屏
    GUI_MSG_REFRESH_CURSOR		= 0x00800000,		// 更新鼠标位置
    GUI_MSG_HIDE_CURSOR			= 0x01000000,		// 隐藏鼠标位置
	DSK_MSG_MAIN_CMD			= 0x02000000,		// MAIN消息传输管道	
	DSK_MSG_CHANG_LANGUAGE		= 0x04000000,		// 语言编码	
	DSK_MSG_FW_UPDATE			= 0x08000000,		// 固件升级
	DSK_MSG_SWITCH_CVBS			= 0x10000000,		// 切换到CVBS
	DSK_MSG_SWITCH_YPBPR		= 0x20000000,		// 切换到色差
	DSK_MSG_SWITCH_HDMI			= 0x40000000,		// 切换到HDMI
	//DSK_MSG_SCREEN_SWITCH		= 0x80000000		// 屏幕切换
}gui_broadcast_msg_system_e;

/*广播消息为用户定义的消息，用户定义的消息用于在Activity间通信，从GUI_MSG_USER消息开始用户最多可以定义26个此类消息，且各消息便于与或操作*/
typedef enum GUI_BROADCAST_MSG_USER_E
{
	GUI_MSG_HOME				= 0x00000001,		// 回到主界面
	GUI_MSG_KILL				= 0x00000002,		// 强制要求应用程序退出
	GUI_APP_RESTORE				= 0x00000004,		// 应用程序恢复前台
	GUI_MSG_SETTING_RESTORE		= 0x00000008,
	GUI_MSG_ORCHID_UPDATA_START	= 0x00000010,		// 开始更新orchid
	GUI_MSG_ORCHID_UPDATA_END	= 0x00000020,		// 更新orchid 结束
	GUI_MSG_USER				= 0x00000040		// 留给用户使用
}gui_broadcast_msg_user_e;


typedef struct GUI_MSG_INFO
{
    __u32       id;
    __u32		dwAddData1;
	__u32		dwAddData2;
    __u32       dwReserved;
    void*       p_arg;
}gui_msg_t;

typedef struct GUI_SYNC_MSG
{
	gui_msg_t		msg_base_info;
	Hview			dst_view;
}gui_sync_msg_t;

typedef struct GUI_NOTIFY_MSG
{
    gui_msg_t            msg;
    struct GUI_NOTIFY_MSG*       next;
}gui_notify_msg_t;

typedef struct GUI_TIMER_PROCPARA
{
    Hview                           view;		//每个timer对应一个view
    __u16                           id;
    __u32                           dwAddData;
    __u32                           dwReserved;                      
}gui_timer_procpara_t;

typedef enum GUI_MSG_SERVER_STATE
{
	gui_msg_server_on = 0,
	gui_msg_server_off
}gui_msg_server_state_e;


typedef gui_msg_t  *p_msg;

typedef __s32 (* pGUI_MSG_CB)     (gui_msg_t * msg);
typedef __s32 (* pGUI_TIMERPROC)  (gui_timer_procpara_t *para);


#endif

