/*
*******************************************************************************************************************
*                                                         	Listmenu  Framework
*                                         			the display dirver support module
*
*                             				 Copyright(C), 2006-2008, Softwinner Microelectronic Co., Ltd.
*											               All Rights Reserved
*
*File Name�� 	message.h
*
*Author�� 		William Wang
*
*Version :   	1.1.0
*
*Date:  		2009-7-22
*
*Description : 	message definition and function implement 
*
*Others : 		None at present.
*
* History :
*
* <Author>          <time>        <version>     <description>
*
* William Wang     2009-7-22         1.1.0          Create File
*
*******************************************************************************************************************
*/
#ifndef __MESSAGE_H__
#define __MESSAGE_H__

#define GUI_MSG_USER   0x2000

#define GUI_MSG_SHORTKEY (GUI_MSG_USER_DEF+1)
#define GUI_MSG_LINEIN (GUI_MSG_USER_DEF+2)
#define GUI_MSG_ROOT_CMD (GUI_MSG_USER_DEF+3)
#define GUI_MSG_HOME_CMD (GUI_MSG_USER_DEF+3)
#define GUI_POWER_TO_MAIN (GUI_MSG_USER_DEF+5)
#define GUI_MSG_RECORD_LIST (GUI_MSG_USER_DEF+6)
#define GUI_MSG_KEY_MODESET (GUI_MSG_USER_DEF+7)
#define SYSTEM_MSG_RESTORE_FACTORY  (GUI_MSG_USER_DEF+8)


typedef enum __GUI_MSGID
{
    GUI_MSG_CREATE = 0,
    GUI_MSG_DESTROY,
    GUI_MSG_PAINT,                                /* draw */
    GUI_MSG_CLOSE,
    GUI_MSG_QUIT,
    GUI_MSG_KEY,
    GUI_MSG_TOUCH,
    GUI_MSG_COMMAND,
    GUI_MSG_ENABLE,
    GUI_MSG_STSTEM,                                 /*�㲥��Ϣ��ע��ؼ���Ϣ�ȣ�(GUIϵͳ����)*/
    GUI_MSG_NOTIFY_CHILD_DELETED,
    GUI_MSG_SET_FOCUS,
    GUI_MSG_SET_UNFOCUS,
    GUI_MSG_INVALID,
    GUI_MSG_TOUCHACTIVE,
    GUI_MSG_TOUCHINACTIVE,
    GUI_MSG_INITDIALOG,
    GUI_MSG_NCPAINT,
    GUI_MSG_ERASEBKGND,
    GUI_MSG_TIMER,
    
    GUI_MSG_WIN_SLEEP=100,
    GUI_MSG_WIN_WAKEUP,
    GUI_MSG_LYR_MOVE,
    
    GUI_MSG_KEYTONE,

    GUI_MSG_CONTROL = 0x1000,                       /*�ؼ�*/

    GUI_MSG_DESKTOP = 0x2000,


    
	GUI_MSG_USER_DEF = 0x3000,
    
    GUI_MSG_                            
}__gui_msgid_t;

typedef enum __GUI_MSG_KEYID
{
    GUI_MSG_KEY_UP = 0x0,
    GUI_MSG_KEY_DOWN,
    GUI_MSG_KEY_ENTER,
    GUI_MSG_KEY_RIGHT,
    GUI_MSG_KEY_LEFT,
    GUI_MSG_KEY_ESCAPE,
	GUI_MSG_KEY_VDEC,
	GUI_MSG_KEY_VADD,
    GUI_MSG_KEY_RISE,
    GUI_MSG_KEY_POWER,
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
    GUI_MSG_KEY_TITLE,
    GUI_MSG_KEY_MENU,
    GUI_MSG_KEY_SETUP,
    GUI_MSG_KEY_SEL,
    GUI_MSG_KEY_SHIFT,
    GUI_MSG_KEY_POWEROFF,
	GUI_MSG_KEY_AUDIO,
	GUI_MSG_KEY_VIDEO,
    GUI_MSG_KEY_LR,
    GUI_MSG_KEY_SWITCH,
    GUI_MSG_KEY_PREV,
    GUI_MSG_KEY_NEXT,
    GUI_MSG_KEY_FF,
    GUI_MSG_KEY_RR,
	GUI_MSG_KEY_EPG,
	GUI_MSG_KEY_SCAN,
	GUI_MSG_KEY_SUBMENU,
	GUI_MSG_KEY_SUBTITLE,//��Ļ�л�����
	GUI_MSG_KEY_LIST,	
	GUI_MSG_KEY_DTVREC,
	GUI_MSG_KEY_TV,
	GUI_MSG_KEY_MUSIC,
	GUI_MSG_KEY_MOVIE,
	GUI_MSG_KEY_TVOUT,
	GUI_MSG_KEY_SETTING,
	GUI_MSG_KEY_PICTURE,
	GUI_MSG_KEY_EBOOK,
	GUI_MSG_KEY_RECORD,
	GUI_MSG_KEY_FM,
	GUI_MSG_KEY_SCN_ON_OFF,
	GUI_MSG_KEY_ENTER_IR,
	GUI_MSG_KEY_MUTE,
	GUI_MSG_KEY_EQ, 
	GUI_MSG_KEY_SEL_TIME,	
	GUI_MSG_KEY_SHOW_TIME,	
	GUI_MSG_KEY_AUDIO_CH,	
	GUI_MSG_KEY_AUDIO_TRACK, 
	GUI_MSG_KEY_CH_UP,
	GUI_MSG_KEY_CH_DOWN,	
	GUI_MSG_KEY_NUM_SEL,
	GUI_MSG_KEY_LOOP,
	GUI_MSG_KEY_MODE,
	GUI_MSG_KEY_CALENDAR,
    GUI_MSG_KEY_INFO,
    GUI_MSG_KEY_TV_RIGHT,
    GUI_MSG_KEY_TV_LEFT,
	GUI_MSG_KEY_IR_VADD,
	GUI_MSG_KEY_IR_VDEC,
	GUI_MSG_KEY_IR_MENU,
	GUI_MSG_KEY_IR_LEFT,
	GUI_MSG_KEY_IR_RIGHT,
	GUI_MSG_KEY_IR_UP,
	GUI_MSG_KEY_IR_DOWN,
	GUI_MSG_KEY_IR_MUTE,

	
    GUI_MSG_KEY_LONGUP = 0x50,
    GUI_MSG_KEY_LONGDOWN,
    GUI_MSG_KEY_LONGENTER,
    GUI_MSG_KEY_LONGRIGHT,
    GUI_MSG_KEY_LONGLEFT,
    GUI_MSG_KEY_LONGESCAPE,
	GUI_MSG_KEY_LONGVDEC,
	GUI_MSG_KEY_LONGVADD,
    GUI_MSG_KEY_LONGRISE,
    GUI_MSG_KEY_LONGPOWER,
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
    GUI_MSG_KEY_LONGTITLE,
    GUI_MSG_KEY_LONGMENU,
    GUI_MSG_KEY_LONGSETUP,
    GUI_MSG_KEY_LONGSEL,
    GUI_MSG_KEY_LONGSHIFT,
    GUI_MSG_KEY_LONGPOWEROFF,
    GUI_MSG_KEY_LONGAUDIO,
    GUI_MSG_KEY_LONGVIDEO,
    GUI_MSG_KEY_LONGLR,    
    GUI_MSG_KEY_LONGSWITCH,
    GUI_MSG_KEY_LONGPREV,
    GUI_MSG_KEY_LONGNEXT,
    GUI_MSG_KEY_LONGFF,
    GUI_MSG_KEY_LONGRR,
	GUI_MSG_KEY_LONGEPG,
	GUI_MSG_KEY_LONGSCAN,
	GUI_MSG_KEY_LONGSUBMENU,
	GUI_MSG_KEY_LONGSUBTITLE,//��Ļ�л�����
	GUI_MSG_KEY_LONGLIST,	
	GUI_MSG_KEY_LONGDTVREC,   
	GUI_MSG_KEY_LONGTV,
	GUI_MSG_KEY_LONGMUSIC,	/*40*/
	GUI_MSG_KEY_LONGMOVIE,
	GUI_MSG_KEY_LONGTVOUT,
	GUI_MSG_KEY_LONGSETTING,
	GUI_MSG_KEY_LONGPICTURE,
	GUI_MSG_KEY_LONGEBOOK,
	GUI_MSG_KEY_LONGRECORD,
	GUI_MSG_KEY_LONGFM,
	GUI_MSG_KEY_LONGSCN_ON_OFF,
	GUI_MSG_KEY_LONGENTER_IR,
	GUI_MSG_KEY_LONGMUTE,
	GUI_MSG_KEY_LONGEQ, 
	GUI_MSG_KEY_LONGSEL_TIME,	
	GUI_MSG_KEY_LONGSHOW_TIME,	
	GUI_MSG_KEY_LONGAUDIO_CH,	
	GUI_MSG_KEY_LONGAUDIO_TRACK, 
	GUI_MSG_KEY_LONGCH_UP,
	GUI_MSG_KEY_LONGCH_DOWN,	
	GUI_MSG_KEY_LONGNUM_SEL,
	GUI_MSG_KEY_LONGLOOP,
	GUI_MSG_KEY_LONGMODE,
	GUI_MSG_KEY_LONGCALENDAR,
    GUI_MSG_KEY_LONGINFO,
    GUI_MSG_KEY_LONGTV_RIGHT,
    GUI_MSG_KEY_LONGTV_LEFT,
    GUI_MSG_KEY_LONGIR_VADD,
	GUI_MSG_KEY_LONGIR_VDEC,
	GUI_MSG_KEY_LONGIR_MENU,
	GUI_MSG_KEY_LONGIR_LEFT,
	GUI_MSG_KEY_LONGIR_RIGHT,
	GUI_MSG_KEY_LONGIR_UP,
	GUI_MSG_KEY_LONGIR_DOWN,
	GUI_MSG_KEY_LONGIR_MUTE,
}__gui_msg_keyid_t;

typedef enum __GUI_MSG_TOUCHID
{
   GUI_MSG_TOUCH_DOWN = 0x80,
   GUI_MSG_TOUCH_UP,
   GUI_MSG_TOUCH_MOVE,
   GUI_MSG_TOUCH_ZOOMIN,
   GUI_MSG_TOUCH_ZOOMOUT,
   GUI_MSG_TOUCH_NCDOWN,
   GUI_MSG_TOUCH_NCUP,
   GUI_MSG_TOUCH_NCMOVE,
   GUI_MSG_TOUCH_OVERDOWN,
   GUI_MSG_TOUCH_OVERUP,
   GUI_MSG_TOUCH_OVERMOVE,
   GUI_MSG_TOUCH_OVERZOOMIN,
   GUI_MSG_TOUCH_OVERZOOMOUT,
   GUI_MSG_TOUCH_LONGDOWN,
   GUI_MSG_TOUCH_OVERLONGDOWN
}__gui_msg_touchid_t;

typedef struct __GUI_KEY_INFO
{
    __u32  key;                                     /* key value                                                    */
    __u32  presscnt;
}__gui_keyinfo_t;


typedef struct __GUI_MSG
{
    __u32       id;
    H_WIN       h_srcwin;
    H_WIN       h_deswin;
    __u32		dwAddData1;
	__u32		dwAddData2;
    __u32       dwReserved;
    void*       p_arg;
}__gui_msg_t;

typedef __gui_msg_t  *pmsg;

/*�Ӵ��������򸸴��ڵ�֪ͨ��Ϣ�ṹ*/
typedef struct __GUI_NOTIFY_MSG
{
   H_WIN        hWnd;                       /*�Ӵ��ھ��*/            
   __u32        id;                         /*�Ӵ����ڸ������е�Ψһ��ʶ*/
   __u32        msgcode;                    /*֪ͨ��Ϣ��֪ͨ��*/
   __u32        dwAddData;                  /*֪ͨ��Ϣ�ĸ���ֵ*/
   __u32        dwReserved;                 /*֪ͨ��Ϣ�ı�������ֵ*/
}__gui_notify_msg_t;

extern __bool   GUI_HavePendingMessage  (H_WIN hWnd);                           /**/
extern __s32    GUI_BroadcastMessage    (__gui_msg_t *pmsg);
extern __s32    GUI_PostSyncMessage     (__gui_msg_t *pmsg);
extern __s32    GUI_SendMessage         (__gui_msg_t *msg);
extern __s32    GUI_SendMessageTimeout  (__gui_msg_t *msg,__u32 timeout);
extern __s32    GUI_PostMessage         (__gui_msg_t *msg);
extern __s32    GUI_SendNotifyMessage   (__gui_msg_t *msg);
extern __s32    GUI_GetMessage          (__gui_msg_t *msg,H_WIN  hManWin);
extern __s32    GUI_GetMessageEx        (__gui_msg_t *msg,H_WIN  hManWin);
extern __s32    GUI_MsgSetRepeatTimes   (H_WIN  hManWin,__u32 count);
extern __bool   GUI_QueryMessage        (H_WIN  hManWin);
extern __s32    GUI_PeekMessage         (__gui_msg_t *msg,H_WIN  hManWin,__u32 len);
extern __s32    GUI_DispatchMessage     (__gui_msg_t *msg);
extern __s32    GUI_PostQuitMessage     (H_WIN  hWnd);
extern __s32    GUI_SendAsyncMessage    (__gui_msg_t *msg);
extern __s32    GUI_ThrowAwayMessages   (H_WIN hWnd);
extern __s32    GUI_SetSyncMsgRetVal    (__gui_msg_t *msg,__s32 ret);
extern __s32    GUI_PostSyncSem         (__gui_msg_t *msg);
#endif
