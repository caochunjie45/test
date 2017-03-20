/*
**************************************************************************************************************
*											         ePDK
*						            the Easy Portable/Player Develop Kits
*									             desktop system 
*
*						        	 (c) Copyright 2007-2010, ANDY, China
*											 All Rights Reserved
*
* File    	: activity.c
* By      	: Andy.zhang
* Func		: activity 
* Version	: v1.0
* ============================================================================================================
* 2010-9-6 19:43:50  andy.zhang  create this file, implements the fundemental interface;
**************************************************************************************************************
*/

#ifndef __ACTIVITY__
#define __ACTIVITY__

#include "applet_info_manager.h"

struct _Activity;
typedef struct _Activity Activity;

typedef __s32 (*AppletStart)			(Activity *thiz);
typedef __s32 (*AppletSuspend)		(Activity *thiz);
typedef __s32 (*AppletResume)		(Activity *thiz);
typedef __s32 (*AppletBackground)	(Activity *thiz);
typedef __s32 (*AppletStop)			(Activity *thiz);

typedef struct _AppletPlugin
{
	AppletStart 		start;
	AppletSuspend 	 	suspend;
	AppletResume 	 	resume;
	AppletBackground  	background;
	AppletStop		 	stop;	
	char				priv[1];
}AppletPlugin;

typedef enum _ActivityState
{
	ACTIVITY_ST_STOP,
	ACTIVITY_ST_TOPLEVEL,
	ACTIVITY_ST_SUSPEND,
	ACTIVITY_ST_BACKGROUND
}ActivityState;

enum
{
	APPLET_CMD_START,
	APPLET_CMD_SUSPEND,
	APPLET_CMD_RESUME,
	APPLET_CMD_BACKGROUND,
	APPLET_CMD_STOP,
	APPLET_CMD_
};

//Activity* activity_create(AppletInfo *info);					/* ����һ�� Activity */

AppletPlugin *activity_get_plugin(Activity *thiz);				/* ��ȡAppletPlugin�ӿ�*/

__s32 activity_set_attr(Activity *thiz, void *attr);			/* ����priv����  	 */

void* activity_get_attr(Activity *thiz);						/* ��ȡpriv����  	 */

__s32 activity_set_load_para(char * app_name, char *src_name, void *p_arg, __u32 size);	

__s32 activity_get_load_para(char *app_name, char *src_name, void *p_arg, __u32 size);	

__s32 activity_init(void);

__s32 activity_deinit(void);

__s32 activity_load_file(char * filename);		/* ����һ���ļ� 	 */	

__s32 activity_load_app(char * appname);			/* ����һ��Ӧ�ó���  */	

H_WIN activity_get_root_win(Activity *thiz);					/* ���Ӵ���			 */

__s32 activity_background_top(void);						/* ʹ��������̨״̬*/	
	    
__s32 activity_return_home(Activity *thiz);						/* ���������� 		 */	
				
void  activity_finish(Activity *thiz);							/* ɱ��Activity 	 */	

//void activity_kill_backgrd_except_home(void);			/* ɱ����home�������Activity 	 */	

//void activity_kill_all_backgrd(void);
void  activity_finish_all(void);

__s32 activity_suspend_top(void); 			/* ��toplevel��activity��Ϊsuspend 	*/	

__s32 activity_resume_top(void);					/* �ָ�toplevel��activity */

__s32 activity_notify_top(__gui_msg_t *pmsg);

__s32 activity_all_able_standby(void);	    /* ��ѯ�Ƿ��������е�activity������standby*/




#endif /* __ACTIVITY__*/

