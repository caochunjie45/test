/*
**************************************************************************************************************************
*													        eMOD
*						            		the Easy Portable/Player Develop Kits
*									   			includes melix activity module 
*
*                             		 Copyright(C), 2011-2012, Softwinner Microelectronic Co., Ltd.
*									               All Rights Reserved
*
* File name		:	activity.h
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
#ifndef __ACTIVITY_H__
#define __ACTIVITY_H__

typedef void*   Activity;

typedef __s32   (*ActivityStart)        (Activity thiz);
typedef __s32   (*ActivitySuspend)      (Activity thiz);
typedef __s32   (*ActivityResume)       (Activity thiz);
typedef __s32   (*ActivityStop)			(Activity thiz);

typedef struct GUI_ACTIVITY_CREATE
{
	ActivityStart			start;
	ActivitySuspend			suspend;
	ActivityResume			resume;
	ActivityStop			stop;
}gui_activity_create_t;

//typedef __s32	(*A_LISTENER)				(gui_broadcast_msg_type_e msg_type, void *pdata);
typedef __s32	(*A_LISTENER)				(p_msg msg);


/**************************************************************************************/
/*ע��*/
Activity		gui_activity_register(gui_activity_create_t *para);

/*ע��*/
__s32       	gui_activity_unregister(Activity hdle);

/*����*/
__s32       	gui_activity_start(Activity hdle);

/*�ö�*/
__s32       	gui_activity_set_top(Activity hdle);

/*��ȡRootView*/
Hview			gui_activity_get_rootview(Activity hdle);

/*���ü���*/
__s32       	gui_activity_set_listener(Activity hdle, p_msg msg, A_LISTENER proc);			//����㲥��Ϣ

/**************************************************************************************/


#endif	/*__ACTIVITY_H__*/

