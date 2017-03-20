/*
**************************************************************************************************************
*											         ePDK
*						            the Easy Portable/Player Develop Kits
*									           desktop system 
*
*						        	 (c) Copyright 2007-2011, CHIPHD, China
*											 All Rights Reserved
*
* File    	: app_dialog.h
* By      	: CQQ
* Func		: 
* Version	: v1.0
* ============================================================================================================
* 2011/06/02 20:56  create this file, implements the fundamental interface;
**************************************************************************************************************
*/
#ifndef __APP_DIALOG_H__2C160E70_63CA_4019_A468_4515BA77B392__
#define __APP_DIALOG_H__2C160E70_63CA_4019_A468_4515BA77B392__

#include "beetles_app.h"

#define ADLG_BIT_(_i)                     (1 << (_i))

//��Ϣ����
typedef enum tag_APP_DIALOG_E
{
	ADLG_OK                           = 0, //��Ϣ�����һ����ť��OK
	ADLG_OKCANCEL                     = 1, //��Ϣ�����������ť��OK��Cancel
	ADLG_YESNO                        = 2, //��Ϣ�����������ť��Yes��No
	ADLG_RETRYCANCEL                  = 3, //��Ϣ�����������ť��Retry��Cancel
	ADLG_ABORTRETRYIGNORE             = 4, //��Ϣ�����������ť��Abort��Retry��Ignore
	ADLG_YESNOCANCEL                  = 5  //��Ϣ�����������ť��Yes��No��Cancel
}APP_DIALOG_E;
#define ADLG_BTN_STYLE_BIT            0x0000000fL

//TODO:to be continue
#define ADLG_ICONHAND                 0x00000010L       //����Ϣ������ʾֹͣ��־ͼ��
#define ADLG_ICONQUESTION             0x00000020L       //����Ϣ������ʾ�ʺ�ͼ��
#define ADLG_ICONEXCLAMATION          0x00000030L       //����Ϣ������ʾ��̾��ͼ��
#define ADLG_ICONASTERISK             0x00000040L       //����Ϣ������ʾһ��Բ��Χ����ĸ��i����ͼ��
#define ADLG_ICONSTOP                 ADLG_ICONHAND     //����Ϣ������ʾֹͣ��־ͼ��
#define ADLG_ICONINFORMATION          ADLG_ICONASTERISK //����Ϣ������ʾһ��Բ��Χ����ĸ��i����ͼ��

#define ADLG_ICON_STYLE_BIT           0x000000f0L

//�Ի�������
enum
{
	ADLG_IDOK                         = 1,
	ADLG_IDCANCEL                     = 2,
	ADLG_IDABORT                      = 3,
	ADLG_IDRETRY                      = 4,
	ADLG_IDIGNORE                     = 5,
	ADLG_IDYES                        = 6,
	ADLG_IDNO                         = 7,
	ADLG_IDM_TIMEROUT                 = 8
};

//align(base screen)
#define ADLG_TA_LEFT                   ADLG_BIT_(16)
#define ADLG_TA_RIGHT                  ADLG_BIT_(17)
#define ADLG_TA_HCENTER                (ADLG_TA_LEFT | ADLG_TA_RIGHT)
#define ADLG_TA_HORIZONTAL             ADLG_TA_HCENTER

#define ADLG_TA_TOP                    ADLG_BIT_(18)
#define ADLG_TA_BOTTOM                 ADLG_BIT_(19)
#define ADLG_TA_VCENTER                (ADLG_TA_TOP | ADLG_TA_BOTTOM)
#define ADLG_TA_VERTICAL               ADLG_TA_VCENTER

#define ADLG_TA_CENTER                 (ADLG_TA_HCENTER | ADLG_TA_VCENTER)
#define ADLG_TA_(_H, _V)               ((_H) | (_V))

#define ADLG_DS_VISIBLE                ADLG_BIT_(20)

#define ADLG_MAKE_WND_STYLE(_i, _b)    ((_i) | (_b)) //icon and btn style compose
#define ADLG_GET_BTN_STYLE(_s)         ((_s) & ADLG_BTN_STYLE_BIT)
#define ADLG_GET_ICON_STYLE(_s)        ((_s) & ADLG_ICON_STYLE_BIT)
#define ADLG_GET_WND_STYLE(_s)         LOWORD(_s)

#define ADLG_GET_ALIGN(_s)             ((_s) & (ADLG_TA_HORIZONTAL | ADLG_TA_VERTICAL))//HIWORD(_s)
#define ADLG_GET_ALIGN_H(_s)           ((_s) & ADLG_TA_HORIZONTAL)
#define ADLG_GET_ALIGN_V(_s)           ((_s) & ADLG_TA_VERTICAL)

#define ADLG_STYLE_(_align, _wndstyle) (_align | _wndstyle)
//for example : ADLG_STYLE_(ADLG_TA_CENTER, ADLG_YESNO)
 

//����ͼƬ��Դid˳��
enum
{
	ADLG_BMP_SZID_BG = 0, //����
	ADLG_BMP_SZID_BTN_F,  //��ť����
	ADLG_BMP_SZID_BTN_N,  //��ť��ͨ
	ADLG_BMP_SZID_ICON,   //ͼ��
	ADLG_BMP_MAX_SZID     //id����
};

//�����ַ�����Դid˳��
enum
{
	ADLG_STR_SZID_TITLE = 0,   //��Ϣ�����
	ADLG_STR_SZID_CONTENT,     //��ʾ����
	ADLG_STR_MAX_SZID,         //id����
};

#define ADLG_STR_TITLE_MEM_SIZE        (16 * 3)
#define ADLG_STR_CONTENT_MEM_SIZE      (48 * 3)//((64 - 16) * 3)

enum
{
	ADLG_CMD_EXIT = 0,
	ADLG_CMD_SET_UI,
	ADLG_END_CMD,
};

//�����Ի���
/************************************************************************/
// h_parent --- dialog parent window handle (must be main window)
// win_id   --- dialog window id of dialog parent win (size: __u16)
// style    --- style of button and alignment
// bmp_id   --- bmp32 res id array (array size -- ADLG_BMP_MAX_SZID)
// str_id   --- string res id array (array size -- ADLG_STR_MAX_SZID)
// timeout  --- auto close dialog time (xxxx ms), 0 for infinitude
// return value --- dialog frame window handle
// remark : to show dialog, need to use app_dialog_set_ui(), see app_dialog_set_ui remark
/************************************************************************/
H_WIN app_dialog_create(H_WIN h_parent, __s32 win_id, __u32 style, 
						__s32 bmp_id[], __s32 str_id[], __u32 timeout, const char* str);

#define app_center_dialog_create(_h, _id, _wnd_s, _bmp, _str)  \
	app_dialog_create(_h, _id, (ADLG_TA_CENTER | ADLG_DS_VISIBLE | (_wnd_s)), _bmp, _str, 0)

#define default_dialog(_dialog, _parent, _id, _btn_s, _str)  do \
{\
	__s32 _bmp[] = {ID_DIALOG_MSGBOX_BMP, ID_DIALOG_MSGBOX_BTN_F_BMP, ID_DIALOG_MSGBOX_BTN_N_BMP, NULL}; \
	_dialog = app_dialog_create(_parent, _id, (ADLG_TA_CENTER | ADLG_DS_VISIBLE | ADLG_MAKE_WND_STYLE(ADLG_ICONINFORMATION, _btn_s)), _bmp, _str, 0, 0); \
}while (0)
#define default_dialog_ex(_dialog, _parent, _id, _btn_s, _str, str_ex)  do \
{\
	__s32 _bmp[] = {ID_DIALOG_MSGBOX_BMP, ID_DIALOG_MSGBOX_BTN_F_BMP, ID_DIALOG_MSGBOX_BTN_N_BMP, NULL}; \
	_dialog = app_dialog_create(_parent, _id, (ADLG_TA_CENTER | ADLG_DS_VISIBLE | ADLG_MAKE_WND_STYLE(ADLG_ICONINFORMATION, _btn_s)), _bmp, _str, 0, str_ex); \
}while (0)

#define default_dialog_timeout(_dialog, _parent, _id, _btn_s, _str, _time)  do \
{\
	__s32 _bmp[] = {ID_DIALOG_MSGBOX_BMP, ID_DIALOG_MSGBOX_BTN_F_BMP, ID_DIALOG_MSGBOX_BTN_N_BMP, NULL}; \
	_dialog = app_dialog_create(_parent, _id, (ADLG_TA_CENTER | ADLG_DS_VISIBLE | ADLG_MAKE_WND_STYLE(ADLG_ICONINFORMATION, _btn_s)), _bmp, _str, _time, 0); \
}while (0)

//�����Ի���
/************************************************************************/
// h_parent  --- dialog parent handle (must be main window)
// win_id    --- dialog window id of dialog parent window (size: __u16)
// style     --- style of button
// bmp_id    --- bmp32 res id array (array size -- ADLG_BMP_MAX_SZID)
// str_id    --- string res id array (array size -- ADLG_STR_MAX_SZID)
// timeout   --- auto close dialog time (xxxx ms), 0 for infinitude
// dialog_rt --- dialog screen rect
// return value --- dialog frame window handle
/************************************************************************/
//TODO:to be continue
H_WIN app_dialog_create_ex(H_WIN h_parent, __s32 win_id, __u32 btn_style, 
						   __s32 bmp_id[], __s32 str_id[], __u32 timeout, GUI_RECT *dialog_rt);

//���ٶԻ���
/************************************************************************/
// msg --- message of dialog parent window callback function received
// msg->h_deswin   --- dialog parent window
// msg->h_srcwin   --- dialog frame window handle
// msg->id         --- GUI_MSG_COMMAND
// msg->dwAddData1 --- LOWORD : dialog window id; HIWORD : ADLG_CMD_EXIT
// msg->dwAddData2 --- result of dialog(such as ADLG_IDYES...)
// msg->dwReserved --- reserve
// return value    --- 0
// remark : 
//       1. when dialog is to be closed, dialog send this message to it's parent,
//          dialog parent window should call this function to delete dialog.
//       2. app should set focus after this function if necessary.
/************************************************************************/
__s32 app_dialog_destroy(H_WIN h_dialog);

// ���öԻ���ui����
enum
{                          // coordinate base on dialog window
	ADLG_UI_CMD_TITLE = 0, //reset title, (wParam, lParam) -> (x, y)
	ADLG_UI_CMD_CONTENT,   //reset content, lParam : point of RECT(x, y, width, height)
	ADLG_UI_CMD_ICON,      //reset icon, (wParam, lParam) -> (x, y)
	ADLG_UI_CMD_START_BTN, //reset the first button, (wParam, lParam) -> (x, y)
	ADLG_END_UI_CMD,       //use this command to show dialog
	ADLG_MAX_UI_CMD
};
#define ADLG_UI_OK          ADLG_END_UI_CMD
/************************************************************************/
// ui_id  --- such as ADLG_UI_CMD_TITLE ...
// wParam --- depend on ui_id
// lParam --- depend on ui_id
// return --- 0 
/************************************************************************/
__s32 app_dialog_set_ui(H_WIN h_dialog, __s32 ui_id, __s32 wParam, __s32 lParam);


/* //app use dialog, for example:
__s32 xxx_win_cb(__gui_msg_t *msg)
{
	switch(msg->id)
	{
	case GUI_MSG_COMMAND:
		{
			__u32 result;
			if (LOWORD(msg->dwAddData1) == ID_DIALOG)//(ID_DIALOG range [0, 0xffff])
			{
				switch(HIWORD(msg->dwAddData1))
				{
				case ADLG_CMD_EXIT:
					{
						result = msg->dwAddData2;
						//to delete dialog
						app_dialog_destroy(msg);
						//do something for result	
					}
					return 0;
				case ADLG_CMD_SET_UI:
					{//set dialog ui para
						app_dialog_set_ui(msg->h_srcwin, ADLG_UI_OK, 0, 0);
					}
					break;
				}
			}
		}
		break;
	}
	return GUI_FrmWinDefaultProc(msg);
}
*/

#endif //__APP_DIALOG_H__2C160E70_63CA_4019_A468_4515BA77B392__
//End of this file

