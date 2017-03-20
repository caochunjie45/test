/*
**************************************************************************************************************
*											         ePDK
*						            the Easy Portable/Player Develop Kits
*									           desktop system 
*
*						        	 (c) Copyright 2007-2010, ANDY, China
*											 All Rights Reserved
*
* File    	: dsk_message.h
* By      	: Andy.zhang
* Func		: desk message define
* Version	: v1.0
* ============================================================================================================
* 2009-7-8 10:02:17  andy.zhang  create this file, implements the fundemental interface;
**************************************************************************************************************
*/

#ifndef __DSK_MESSAGE_H__
#define __DSK_MESSAGE_H__

typedef enum
{
	DSK_MSG_START = GUI_MSG_DESKTOP,
	
	DSK_MSG_HOME,				// �ص�������
	DSK_MSG_KILL,				// ǿ��Ҫ��Ӧ�ó����˳�
	DSK_APP_RESTORE,			// Ӧ�ó���ָ�ǰ̨
		
	DSK_MSG_SYS_START = DSK_MSG_START + 0x100,
	
	/* ������Ϣ��Ϊ����ϵͳ �� ������ �յ� */	
	DSK_MSG_REFRESH_CURSOR,		// �������λ��
	DSK_MSG_HIDE_CURSOR,		// �������λ��
	DSK_MSG_MAIN_CMD,			// MAIN��Ϣ����ܵ�	
	DSK_MSG_CHANG_LANGUAGE,		// ���Ա���	
	DSK_MSG_FW_UPDATE,			// �̼�����
	DSK_MSG_FW_START_UPDATE,	// �̼�����
	
	DSK_MSG_SCREEN_SWITCH,		// ��Ļ�л�
	DSK_MSG_SWITCH_YPBPR,		// �л���ɫ��
	DSK_MSG_SWITCH_HDMI,		// �л���HDMI
	DSK_MSG_SWITCH_CVBS,		// �л���CVBS
	DSK_MSG_ASSISTANT_SCENE_CREATE,
	DSK_MSG_ASSISTANT_SCENE_DELETE,
	DSK_MSG_SETTING_RESTORE,	
	
	DSK_MSG_STANDBY_WAKE_UP,
	DSK_MSG_STANDBY,
		
	DSK_MSG_HOLD,				// ������
	DSK_MSG_APP_EXIT,			// һ����������
	DSK_MSG_POWER_OFF,			// �ػ�
	DSK_MSG_LOW_POWER,			// �͵�
	
	DSK_MSG_BAN_VOLUME,			// ����������
	
	DSK_MSG_USBD_PLUG_IN,		// usb device connect pc
	DSK_MSG_USBD_PLUG_OUT,		// usb device plug out pc
	
	DSK_MSG_ORCHID_UPDATE_START,// ��ʼ����orchid	
	DSK_MSG_ORCHID_UPDATE_END,	// ����orchid ����
	
	DSK_MSG_INSTALL_USBH,		// usb host ��⵽device����
	DSK_MSG_UNSTALL_USBH,		// usb host ��⵽device
		
	DSK_MSG_SET_AUTO_OFF,		// ���ùػ�ʱ��
	DSK_MSG_SET_SCN_CLOSE,		// ���ù���ʱ��
	
	DSK_MSG_SCREEN_OPEN,		// ֪ͨ����
	DSK_MSG_SCREEN_CLOSE,		// ֪ͨ����
	DSK_MSG_CLOSE_SCN_GATE_ON,	// �����Զ�����
	DSK_MSG_CLOSE_SCN_GATE_OFF,	// ��ֹ�Զ�����	

	DSK_MSG_TVDAC_PLUGIN,		// tvdac plug in
	DSK_MSG_TVDAC_PLUGOUT,		// tvdac plug out
	DSK_MSG_HDMI_PLUGIN,		// hdmi plug in
	DSK_MSG_HDMI_PLUGOUT,		// hdmi plug out 
	
	DSK_MSG_FS_PART_PLUGIN,		// �ļ�ϵͳ��������
	DSK_MSG_FS_PART_PLUGOUT,	// �ļ�ϵͳ�����γ�

	DSK_MSG_VOLTAGE_CHANGE,     //��ص����仯��Ϣ(dwAddData1Ϊpower_level)
	DSK_MSG_TIME_CHANGE,
	DSK_MSG_MENU_RANDOM,
	DSK_MSG_LAN_CHANGE,
	DSK_MSG_UPDATE_BIGAPP,

	// BOX���� ������Ϣ
	DSK_MSG_BOX_STANDBY_START,	// ֪ͨ���� standby
	DSK_MSG_BOX_STANDBY_END,	
	DSK_MSG_BOX_STANDBY_EXIT,
	DSK_MSG_SWITCH_AUDIO_IF,

    DSK_MSG_ENABLE_STANDBY,	    // ����standby
	DSK_MSG_DISABLE_STANDBY,	// ��ֹstandby
	DSK_MSG_GET_STANDBY_FLAG,	// ��ѯ�Ƿ�����standby
	
	DSK_MSG_ALARM,
	DSK_MSG_HANDLE_PLUGOUT,
    DSK_MSG_LOW_POWER_TIP,
	DSK_MSG_LAST = GUI_MSG_DESKTOP + 0x1000 -1	
}__msg_desktop_e;


#endif /* __DSK_MESSAGE_H__ */





