/*
**************************************************************************************************************************
*													        eMOD
*						            		the Easy Portable/Player Develop Kits
*									   			includes melix gview module 
*
*                             		 Copyright(C), 2011-2012, Softwinner Microelectronic Co., Ltd.
*									               All Rights Reserved
*
* File name		:	gview.h
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
#ifndef __GVIEW_H__
#define __GVIEW_H__

/*HVIEW is view_data_t's handle
typedef void*   Hview;*/

//typedef struct  GUI_EVENT_PROCESS           E_Process;

/*typedef __s32   (*PROCESS_KEY_UP)       (Activity *thiz, __u32 key_value);
typedef __s32   (*PROCESS_KEY_DOWN)     (Activity *thiz, __u32 key_value);
typedef __s32   (*PROCESS_KEY_REPEAT)   (Activity *thiz, __u32 key_value);
typedef __s32   (*PROCESS_TOUCH_EVENT)  (Activity *thiz, motion_event_t event);

typedef struct  GUI_EVENT_PROCESS
{
    PROCESS_KEY_UP          onKeyUp;
    PROCESS_KEY_DOWN        onKeyDown;
    PROCESS_KEY_REPEAT      onKeyRepeat;
    PROCESS_TOUCH_EVENT     onTouchEvent;
}gui_event_process_t;*/

typedef __s32	(*GUI_EVENT_PROCESS)		(p_msg msg);
//typedef GUI_EVENT_PROCESS	gui_event_process_t;

/**************************************************************************************/
/*���� Ĭ��ֻ����child group View*/
Hview gui_gview_create(RECT view_rect);

/*ɾ�� Ĭ��ֻɾ��child group View*/
__s32 gui_gview_delete(Hview g_view_hdle);

/*��� g_view_handle�ȿ�����child group View,Ҳ������root view*/
__s32 gui_gview_add_view(Hview g_view_hdle, Hview view, GUI_EVENT_PROCESS proc);

/*�Ƴ� g_view_handle�ȿ�����child group View,Ҳ������root view*/
__s32 gui_gview_remove_view(Hview g_view_hdle, Hview view);

/*���ý���view, g_view_handle�ȿ�����child group View�� Ҳ������root view*/
__s32 gui_gview_set_focus_view(Hview g_view_hdle, Hview view);

Hview gui_gview_get_focus_view(Hview g_view_hdle);
void* gui_get_view_proc(Hview view);

/*����view��״̬, g_view_handleֻ������child group View*/
//__s32			gui_gview_set_view_status(Hview g_view_hdle, __s32 state);

/*����view, g_view_handleֻ������child group View*/
//__s32			gui_gview_update_view(Hview g_view_hdle, __s32 state);

/**************************************************************************************/

#endif	/*__GVIEW_H__*/
