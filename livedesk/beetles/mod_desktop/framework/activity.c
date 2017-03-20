/*
**************************************************************************************************************
*											         ePDK
*						            the Easy Portable/Player Develop Kits
*									           desktop system 
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

#include "mod_desktop_i.h"
#include "applet_plugin_module.h"

#define ACTIVITY_FINISH		( GUI_MSG_USER_DEF+1 )
#define MAX_ATTR_LENGTH		1024*10

#define MAX_STRING_LEN		128

#define MAX_PARA_BUFF		1024*10


/* һ��Activity����һ��Ӧ�ó��� */
struct _Activity
{	
	ActivityState   state;			// Activity״̬	
	AppletPlugin 	*plugin;		// ����ӿ�
	AppletInfo		*info;			// desktop�ļ�ע����Ϣ	
	void 			*attr;
	H_WIN			 win;			// ���ڣ��첽���á�
};

typedef struct tag_load_para
{
	char app_name[MAX_STRING_LEN];
	char src_name[MAX_STRING_LEN];
	char buff[MAX_PARA_BUFF];
}load_para_t;

load_para_t load_para;
static __krnl_event_t	*load_sem;

/***********************************************************************************************************************/
/***********************************************************************************************************************/

static void __activity_do_finish(Activity *thiz);
static void __activity_delete(Activity *thiz);

static __s32 __activity_main_win_cb(__gui_msg_t *msg)
{	
	switch(msg->id)
	{
		case ACTIVITY_FINISH:			
			__activity_do_finish( (Activity *)(msg->dwAddData1) );	// �첽ж��ģ�� 		
			return EPDK_OK;
			
		default:
			break; 
	}
	
	return GUI_ManWinDefaultProc(msg);		
}

static H_WIN __activity_mainwin_create(char *name)
{	
	H_WIN  						hManWin;	
	__gui_manwincreate_para_t  	create_info;	
		
	eLIBs_memset(&create_info, 0, sizeof(__gui_manwincreate_para_t));
	
	create_info.attr            = NULL; 
	create_info.hParent			= message_loop_win_get_default();
	create_info.hHosting 		= NULL;   
    create_info.ManWindowProc   = __activity_main_win_cb;
    create_info.name            = name; 	
	hManWin 					= GUI_ManWinCreate(&create_info);
	
	if(hManWin == NULL)
    {        
        __wrn(" _activity_mainwin_create failed!\n");      
        return NULL;
    }
    
    return hManWin;
}

/* activity ���� */
static Activity* __activity_create(AppletInfo *info)
{
	Activity *thiz = NULL;
	
	thiz = (Activity *)MALLOC(sizeof(Activity));
	return_val_if_fail(thiz!=NULL, NULL);
	
	memset(thiz, 0, sizeof(Activity));
			
	thiz->plugin = applet_plugin_module_create(info->exec);	
	thiz->win 	 = __activity_mainwin_create(info->name);
	thiz->state	 = ACTIVITY_ST_STOP;	
	thiz->info	 = info;
	thiz->attr	 = NULL;
	
	return thiz;	
}

/*  ɾ�� activity */
static void __activity_delete(Activity *thiz)
{
	if( thiz!= NULL )
	{		
		(*thiz->plugin->stop)(thiz);			
		GUI_ManWinDelete(thiz->win);		
		FREE(thiz);				
	}	
}

static void __activity_do_finish(Activity *thiz)
{
	Activity *top = activity_manager_top( activity_manager_get_default() );
	
	if( thiz != top )	/* ��̨״̬ */
	{		
		/* ɾ����ǰ activity */		
		activity_manager_delete_backgrd( activity_manager_get_default(), thiz);		
		__activity_delete(thiz);		
	}
	else
	{		
		/* ����activity_manager ��Ϣ */
		activity_manager_pop( activity_manager_get_default() );	
			
		/* ɾ����ǰ activity */
		__activity_delete(thiz);		
			
		/* ��ѯtop-level activity */
		top = activity_manager_top(activity_manager_get_default());
			
		/* �ָ� top ״̬ */
		if( top != NULL )
		{
			__msg("top name = %s\n", top->info->name);
			top->state = ACTIVITY_ST_TOPLEVEL;
			(*top->plugin->resume)(top);			
		}		
	}	
}

static __s32 __get_applet_info_from_suffix(AppletInfo *info, void *ctx1, void *ctx2)
{
	AppletInfo **p_info   = (AppletInfo **)ctx1;
	char    	*filetype = (char *)ctx2;	
	Suffix  	*loading  = NULL;	
	int i;
		
	if( info->loading == NULL )
		return 0;
			
	loading = info->loading;
	for( i=0; i< loading->nr; i++ )
	{
		if( !eLIBs_stricmp(filetype, loading->suffix[i]) )
		{
			*p_info = info;
			return -1;
		}
	}

	return 0;
}

static __s32 __get_applet_info_from_name(AppletInfo *info, void *ctx1, void *ctx2)
{
	AppletInfo **p_info = (AppletInfo **)ctx1;
	char *name = (char *)ctx2;
	
	if( !strcmp(info->name, name) )
	{
		*p_info = info;
		return -1;
	}
	
	return 0;
}

static __s32 __get_activity_from_name(Activity *activity, ActivityState state,void *ctx1, void *ctx2)
{
	char *name = (char*)ctx1;
	Activity **activity_found = (Activity **)ctx2;	
	
	if( !strcmp(name, activity->info->name) )
	{
		*activity_found = activity;
		return -1;
	}
	
	return 0;	
}

/*static __s32 applet_is_running( Activity *activity, ActivityState state,void *ctx1, void *ctx2)
{
	char *name = (char*)ctx1;
	Activity **load = (Activity **)ctx2;
	
	if( state != ACTIVITY_ST_BACKGROUND )
		return 0;
		
	if( !strcmp(name, activity->info->name) )
	{
		*load = activity;
		return -1;
	}
	
	return 0;	
}*/

/*static __s32 applet_suspend_to_stop_except( Activity *activity, ActivityState state, void *ctx1, void *ctx2)
{
	char *name = (char*)ctx1;	
			
	if( strcmp(name, activity->info->name) )
	{
		if( state == ACTIVITY_ST_SUSPEND )
		{
			__activity_delete(activity);			
		}
		else if( state == ACTIVITY_ST_TOPLEVEL)
		{
			activity_background_top();
		}
	}
	
	return 0;	
}*/

/*static __s32 applet_backgrd_to_stop_except(Activity *activity, ActivityState state, void *ctx1, void *ctx2)
{
	char *name = (char*)ctx1;
	
	if( strcmp(name, activity->info->name) )
	{	
		if( state == ACTIVITY_ST_BACKGROUND)
		{				
			activity_manager_delete_backgrd(activity_manager_get_default(), activity);
			__activity_delete(activity);
		}		
	}
	
	return 0;	
}*/

/*static __s32 __applet_kill_backgrd(Activity *activity, ActivityState state, void *ctx1, void *ctx2)
{
	//char *name = (char*)ctx1;
	
	//if( state == ACTIVITY_ST_BACKGROUND)
	{				
		activity_manager_delete_backgrd(activity_manager_get_default(), activity);
		__activity_delete(activity);
	}	
	
	return 0;	
}*/

static __s32 __applet_kill_backgrd(Activity *activity, ActivityState state, void *ctx1, void *ctx2)
{		
	if(state == ACTIVITY_ST_BACKGROUND)
	{				
		__msg("kill %s\n", activity->info->name);
		activity_manager_delete_backgrd(activity_manager_get_default(), activity);
		__activity_delete(activity);
	}	
	
	return 0;	
}


static __s32 __applet_permission_mutex(Activity *activity, ActivityState state, void *ctx1, void *ctx2)
{
	int permission = (int)ctx1;
	Activity **mutex = (Activity **)ctx2;
	
	if( (permission == activity->info->permission)&&( state==ACTIVITY_ST_BACKGROUND ) )
	{
		*mutex = activity;
		return -1;
	}
	
	return 0;
}

static __s32 __applet_not_able_standby(Activity *activity, ActivityState state, void *ctx1, void *ctx2)
{	
	Activity **not_able_standby = (Activity **)ctx2;

	if(state==ACTIVITY_ST_SUSPEND)
	{
		return 0;
	}

	if(EPDK_FALSE == activity->info->standby)
	{
		*not_able_standby = activity;
		return -1;
	}

	return 0;
}

static __s32 __activity_load_common(AppletInfo *info)
{
	Activity   *top = NULL;
	Activity   *load = NULL;
	Activity   *mutex= NULL;
	
	top = activity_manager_top(activity_manager_get_default());	
	
	if(top)
	{		
		if(!g_strcmp(top->info->name, info->name))//������Ӧ�ó�����Ϊtoplevel������
		{
			return EPDK_OK;	
		}
		/* ��ǰӦ�ó���suspend */	
		else
		{	
			top->state = ACTIVITY_ST_SUSPEND;
			(*top->plugin->suspend)(top);			
		}
	}	
		
	/* ��ѯ������Ӧ�ó����Ƿ����� */
	activity_manager_foreach( activity_manager_get_default(), __get_activity_from_name, info->name, &load);
	
	if( !load )
	{	
		/* ��ѯ�Ƿ���Ȩ�޻����Ӧ�ó������� */
		if( info->permission != PERMISSON_ )
		{
			/* ɱ������Ӧ�ó��� */
			activity_manager_foreach( activity_manager_get_default(), __applet_permission_mutex, (void*)info->permission, &mutex);
			if( mutex!= NULL )
			{
				activity_manager_delete_backgrd( activity_manager_get_default(), mutex);
				__activity_delete(mutex);			
			}			
		}
		if( info->permission == PERMISSON_AUDIO )
		{
			if(eLIBs_strcmp(info->name, "application://music") && dsk_wkm_is_open())
			{
				dsk_wkm_close();//�ر�Walkman
			}
			if(eLIBs_strcmp(info->name, "application://fm") && dsk_radio_rcv_is_open())
			{
				dsk_radio_rcv_close();
			}
		}
		
		/* ����Ӧ�ó��� */			
		load = __activity_create(info);
		
		if(load)
		{		
			activity_manager_push(activity_manager_get_default(), load);
			load->state = ACTIVITY_ST_TOPLEVEL;	
			(*load->plugin->start)(load);	
		}
	}
	else
	{		
		activity_manager_backgrd_to_top(activity_manager_get_default(), load);
		load->state = ACTIVITY_ST_TOPLEVEL;		
		(*load->plugin->resume)(load);			
	}
	
	return EPDK_OK;	
}

__s32 activity_init(void)
{
	load_sem = esKRNL_SemCreate(1);
	if( !load_sem )
	{
		__err(" create load_sem error \n");
		return EPDK_FAIL;
	}

	return EPDK_OK;
}

__s32 activity_deinit(void)
{
	__u8 err;
	
	esKRNL_SemDel(load_sem, 0, &err);
	load_sem = NULL;
	return EPDK_OK;
}

/* ����һ���ļ� */
__s32 activity_load_file(char *filename)		
{		
	AppletInfo *info = NULL;
	char 	   *filetype = NULL;
	
	/* ��ѯ�ܹ����ظ��ļ���Ӧ�ó���*/
	filetype = eLIBs_strchrlast(filename, '.');
	filetype++;		
	applet_info_manager_foreach( applet_info_manager_get_default(), __get_applet_info_from_suffix, &info, filetype );

	if(info == NULL)
	{
		__wrn("file type .%s is not support!\n", filetype);
		return EPDK_FAIL;
	}
	
	__activity_load_common(info);		
	return EPDK_OK;
}

/* ����һ��Ӧ�ó��� */
__s32 activity_load_app(char *appname)			
{	
	AppletInfo *info = NULL;
	
	/* ��ѯӦ�ó���ִ���ļ�·�� */
	applet_info_manager_foreach( applet_info_manager_get_default(), __get_applet_info_from_name, &info, appname );

	if(info == NULL)
	{
		__wrn("the .desktop file of %s is not exist!\n", appname);
		return EPDK_FAIL;
	}
	
	__activity_load_common(info);		
	return EPDK_OK;	
}

/* ʹ��������̨״̬*/	
__s32 activity_background_top(void)						
{
	Activity *top = NULL;
	top = activity_manager_top(activity_manager_get_default());
	if(!top)
	{
		__msg("top is NULL, background top fail\n");
		return EPDK_FAIL;
	}
	/* ��ǰ��������̨*/	
	top->state = ACTIVITY_ST_BACKGROUND;	
	(*top->plugin->background)(top);	
	
	/* ����activity_manager ��Ϣ*/							
	activity_manager_top_to_backgrd(activity_manager_get_default());
	
	/* ��ѯ��ǰ top_level activity*/
	top = activity_manager_top(activity_manager_get_default());
    if(!top)
    {
        return EPDK_FAIL;
    }
    
	/* �ָ� top_level activity ״̬*/
	top->state = ACTIVITY_ST_TOPLEVEL;
	(*top->plugin->resume)(top);	
	
	return EPDK_OK;
}

/* ����������	*/
__s32 activity_return_home(Activity *thiz)						
{
	Activity *top = NULL;
	Activity *home = NULL;	
	__s32     home_depth = 0;	
	
	while(1)
	{
		/* ��ѯ top_level Activity*/
		top = activity_manager_top(activity_manager_get_default());

		if(top == NULL)
		{
			return EPDK_FAIL;
		}
		
		/* �ж��Ƿ������� activity */
		if( !strcmp(top->info->name, "application://app_root") )
		{
			home = top;
			break;
		}		
		else
		{
			/* ��̨ */
			top->state = ACTIVITY_ST_BACKGROUND;
			/* ����activity_manager ��Ϣ */
			activity_manager_top_to_backgrd(activity_manager_get_default());
			
			(*top->plugin->background)(top);
			
			++home_depth;
		}
	}

	if(home_depth > 0)
	{
		/* �ָ�������״̬ */
		home->state = ACTIVITY_ST_TOPLEVEL;
		(*home->plugin->resume)(home);		
	}
	
	return EPDK_OK;
}

/* �˳���ǰ Activity */
void activity_finish(Activity *thiz)		
{
	__gui_msg_t msg;	
	
	msg.id = ACTIVITY_FINISH;
	msg.h_srcwin = NULL;
	msg.h_deswin = thiz->win;
	msg.dwAddData1 = (__u32)thiz;
	msg.dwAddData2 = 0;
		
	GUI_SendNotifyMessage(&msg);	
}

/*void activity_kill_backgrd_except_home(void)
{	
	ActivityManager * activity_manager = activity_manager_get_default();	
	activity_manager_foreach_backgrd(activity_manager, applet_backgrd_to_stop_except, "application://home", NULL);
}*/

/*void activity_kill_all_backgrd(void)
{	
	ActivityManager * activity_manager = activity_manager_get_default();	
	activity_manager_foreach_backgrd(activity_manager, __applet_kill_backgrd, NULL, NULL);
}*/

void activity_finish_all(void)
{
	Activity *top = NULL;
	Activity *home = NULL;	
	ActivityManager * activity_manager = activity_manager_get_default();	
	
	//ɱ��background��activity
	activity_manager_foreach_backgrd(activity_manager, __applet_kill_backgrd, NULL, NULL);

	//ɱ��ջ���activity
	while(1)
	{
		/* ��ѯ top_level Activity*/
		top = activity_manager_top(activity_manager);

		if(top == NULL)
		{
			return ;
		}
		
		/* �ж��Ƿ������� activity */
		if( !strcmp(top->info->name, "application://home") )
		{
			home = top;
			break;
		}		
		else
		{			
			__msg("kill %s\n", top->info->name);		
			activity_manager_pop(activity_manager);			
			__activity_delete(top);				
		}
	}
	//ɱ��home
	__msg("kill %s\n", home->info->name);		
	activity_manager_pop(activity_manager);			
	__activity_delete(home);
	
	return;
}

__s32 activity_set_attr(Activity *thiz, void *attr)			// ����priv����
{
	return_val_if_fail(thiz!=NULL, -1);
	
	thiz->attr = attr;
	
	return EPDK_OK;
}

void* activity_get_attr(Activity *thiz)						// ��ȡpriv���� 
{
	if( thiz == NULL)
		return NULL;
	else
		return thiz->attr;	
}

__s32 activity_set_load_para(char * app_name, char *src_name, void *p_arg, __u32 size)			// ����
{
	__u8 err;
	
	if( size > MAX_PARA_BUFF )
	{
		__wrn("the arg size is too big \n");
		return EPDK_FAIL;
	}	
	if(app_name == NULL)
	{
		__wrn("the app name is NULL \n");
		return EPDK_FAIL;
	}
	esKRNL_SemPend(load_sem, 0, &err);
	eLIBs_memset(&load_para, 0, sizeof(load_para_t));
	eLIBs_strcpy(load_para.app_name, app_name);
	if(src_name)
	{
		eLIBs_strcpy(load_para.src_name, src_name);
	}
	eLIBs_memcpy(load_para.buff, p_arg, size);
	esKRNL_SemPost(load_sem);
	
	return EPDK_OK;
}

__s32 activity_get_load_para(char *app_name, char *src_name, void *p_arg, __u32 size)						// ��ȡ 
{
	__u8 err;

	if( 0 == eLIBs_strcmp(app_name, load_para.app_name) )
	{
		esKRNL_SemPend(load_sem, 0, &err);
		if(src_name)
		{
			eLIBs_strcpy(src_name, load_para.src_name);
		}
		eLIBs_memcpy(p_arg, load_para.buff, size);		
		esKRNL_SemPost(load_sem);
		return EPDK_OK;
	}
	else
	{
		__wrn(" %s load para not found \n", app_name);
		
		return EPDK_FAIL;
	}
}

/* �������� Activity �ĸ����� */
H_WIN activity_get_root_win(Activity *thiz)							
{	
	return thiz->win;	
}


AppletPlugin *activity_get_plugin(Activity *thiz)
{
	return thiz->plugin;
}

__s32 activity_suspend_top(void)
{
	Activity *top = activity_manager_top( activity_manager_get_default() );
	if(top)
	{
		top->state = ACTIVITY_ST_SUSPEND;
		(*top->plugin->suspend)(top);		
	}

	return EPDK_OK;
}

__s32 activity_resume_top(void)
{
	Activity *top = activity_manager_top( activity_manager_get_default() );
	if(top)
	{
		top->state = ACTIVITY_ST_TOPLEVEL;
		(*top->plugin->resume)(top);		
	}

	return EPDK_OK;
}

__s32 activity_notify_top(__gui_msg_t *pmsg)
{
	__gui_msg_t msg;
	Activity 	*top; 

	top = activity_manager_top(activity_manager_get_default());

	if(top)
	{
		eLIBs_memcpy(&msg, pmsg, sizeof(__gui_msg_t));		
		msg.h_deswin = GUI_WinGetFocusChild(activity_get_root_win(top));	
		
		GUI_SendNotifyMessage(&msg);	
	}
	return EPDK_OK;
}

/*��ѯ�Ƿ�����activity������standby*/
__s32 activity_all_able_standby(void)
{
	Activity * not_able_standby = NULL;
	
	activity_manager_foreach(activity_manager_get_default(), __applet_not_able_standby, NULL, &not_able_standby);

	if(not_able_standby)
	{
		return EPDK_FALSE;
	}
	else
	{
		return EPDK_TRUE;
	}
}


