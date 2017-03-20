
/*
**************************************************************************************************************
*											         ePDK
*						            the Easy Portable/Player Develop Kits
*									           desktop system 
*
* File    	: 	Browser.c,
*			Browser.h
* By      	: 	Libaiao
* Func	: 
* Version	: 	v1.0
* ============================================================================================================
* 2010-4-14 15:24:52  Libaiao  create this file, implements the fundemental interface;
**************************************************************************************************************
*/


#include "beetles_app.h"

#include "Explorer.h"
#include "Explorer_UI.h"
#include "Explorer_List.h"

#include "FileList.h"
#if EXTERN_KARAOKE_EXIST
#include "karaoke_inc.h"
#endif
static   __u32 ExplorerTimerId = 0x16;
#define	 C_EXP_TIMER_INTERVAL	300				// 2second

#define KARAOKE_KAK_DATA_PATH  ":\\rootfs\\kak_s0.kak"


H_WIN explorer_list_win_create(H_WIN h_parent, explr_list_para_t *para);
static __s32 _explorer_list_win_cb(__gui_msg_t *msg);			//�»��߿�ʼ��ʾΪ�ص�����

static __s32 explorer_listview_create(H_WIN  list_win);
static __s32 explorer_listview_onpaint(H_WIN  list_win);
static __s32  explorer_list_win_on_close(__gui_msg_t *msg);
static __s32  explorer_list_win_on_destroy(__gui_msg_t *msg);
static __s32  explorer_list_win_key_proc(__gui_msg_t *msg);
	
static __s32 explorer_rat_init(explr_list_para_t *list_para);
static __s32 explorer_rat_uninit(explr_list_para_t *list_para);

static void explorer_listbar_init(H_WIN  list_win);
static void explorer_listbar_uninit(H_WIN  list_win);

static __s32 _explorer_listbar_item_draw(__lbar_draw_para_t *draw_param);
static __s32 draw_miniature_view_item(__lbar_draw_para_t *draw_param);
static __s32 draw_listview_item(__lbar_draw_para_t *draw_param);

__s32 ExplorerListWinOnEnterKey(explr_list_para_t *list_para);
__s32 ExplorerListWinOnNextItem(explr_list_para_t *list_para);
__s32 ExplorerListWinOnPreviousItem(explr_list_para_t *list_para);
__s32 ExplorerListWinOnNextRow(explr_list_para_t *list_para);
__s32 ExplorerListWinOnPreviousRow(explr_list_para_t *list_para);
__s32 ExplorerListWinOnNextPage(explr_list_para_t *list_para);
__s32 ExplorerListWinOnPreviousPage(explr_list_para_t *list_para);


void  ExplorerListWinGetSearchPath(explr_list_para_t *para);


static __s32 ExtractFileName(char *FileName, char *FilePath);
static __s32 GetListItemFileFullPath(explr_list_para_t *list_para, __s32 ItemIndex, char *FilePath);
static __s32 GetListItemFileName(explr_list_para_t *list_para, __s32 ItemIndex, char *FileName);
rat_media_type_t GetListItemFileMediaType(HRAT rat_handle, __s32 ItemIndex);
void ExplorerSendMessage(H_WIN hwin, __s32 id, __s32 data1, __s32 data2);

//�ı���ʾ��ʽ
void ExplorerSetViewMode(explr_list_para_t *list_para, explorer_view_mode_e view_mode);



//���ļ���������ʾ
__s32 explorer_list_long_string_init(explr_list_para_t *this);
__s32 explorer_list_long_string_uninit(explr_list_para_t *this);
__s32 explorer_list_long_string_start_roll_fast(__lbar_draw_para_t *draw_param, char *string)	;
__s32 explorer_list_long_string_start_roll(explr_list_para_t *this);
static __s32 explorer_list_long_string_stop_roll(explr_list_para_t *this);


//��ñ�����һ������Ĳ���
static __s32 explorer_get_last_para(explr_list_para_t *list_para);
static __s32 explorer_save_last_para(explr_list_para_t *list_para)	;

__s32 explorer_draw_file_info(explr_list_para_t *list_para);
void explorer_list_on_timer(__gui_msg_t *msg);

__s32 File_manager_play_file(explr_list_para_t *list_para)
{
	__s32 ret;
	char FileName[RAT_MAX_FULL_PATH_LEN] = { 0 };	//ȫ·��
	rat_media_type_t media_type ;
	__u8 root_type = 0;
	__u8  fatdirattr;					//add by libaiao,2011.8.24
	file_item_t* file_item = NULL;
	char  path[RAT_MAX_FULL_PATH_LEN] = { 0 };

	file_item = list_para->cur_file_list->cur_item;
	eLIBs_strcpy(path,list_para->cur_file_list->file_path);
	eLIBs_strcat(path, "\\");
	eLIBs_strcat(path,file_item->name);	
	__msg("File_manager_play_file  path = %s \n " , path );
	
	eLIBs_strcpy( FileName , path ) ; 
	__msg("full_file_name=%s\n", FileName );

	fatdirattr = list_para->top_file_list->cur_item->fatdirattr;
	if(fatdirattr == RAT_MEDIA_TYPE_USB_DEVICE)
	{
		root_type = RAT_USB;
	}
	else if(fatdirattr == RAT_MEDIA_TYPE_SD_DEVICE)
	{
		root_type = RAT_TF;
	}
	else
	{
		root_type = 0;
	}

	media_type = rat_get_file_type( file_item->name );
	__msg( "media_type = %d\n" , media_type );
	if( RAT_MEDIA_TYPE_AUDIO == media_type )
	{
		__msg("audio media type break \n");
		return EPDK_OK ;
	}
	list_para->root_type = root_type ;
	ExplorerListWinGetSearchPath( list_para );
	list_para->rat.handle = rat_open(list_para->search_path , media_type , 0 );	//ȫ������
	list_para->rat.total = rat_get_cur_scan_cnt ( list_para->rat.handle ) ; 

	ret = rat_set_file_for_play(list_para->rat.handle, FileName );

	if(EPDK_FAIL == ret)
	{
		__msg( " rat set file for play fail \n");
		return EPDK_FAIL;
	}
	return EPDK_OK;
}
static __s32  file_manager_set_current_file_for_play ( __gui_msg_t *msg )
{
	__s32 ret;
	H_WIN hExpWin = NULL;
	explr_list_para_t *list_para;	
	rat_media_type_t media_type;
	file_item_t* file_item ;

	__u8 root_type = 0;
	__u8  fatdirattr;					//add by libaiao,2011.8.24
	
	list_para = (explr_list_para_t *)GUI_WinGetAttr(msg->h_deswin);
	ret = File_manager_play_file(list_para);					
	if(EPDK_FAIL == ret)
	{
		return EPDK_FAIL ;
	}
	__here__ ;
	
	//add by libaiao,2011.8.24
	fatdirattr = list_para->top_file_list->cur_item->fatdirattr;
	if(fatdirattr == RAT_MEDIA_TYPE_USB_DEVICE)
	{
		root_type = RAT_USB;
	}
	else if(fatdirattr == RAT_MEDIA_TYPE_SD_DEVICE)
	{
		root_type = RAT_TF;
	}
	else
	{
		root_type = 0;
	}
	list_para->root_para->explr_root = root_type ;	
	__msg("file_manager_set_current_file_for_play root_type = %d\n" , root_type );
	file_item = get_file_list_item( list_para->cur_file_list, list_para->rat.index );
	if( ! file_item )
	{
		__msg( " file item is null \n" );
		return EPDK_FAIL ;
	}
	__msg("list_para->rat.index = %d file_item->name = %s \n " , list_para->rat.index , file_item->name );
	
	media_type = rat_get_file_type( file_item->name );
	__msg( "media_type = %d\n" , media_type );

	//media_type = get_file_list_item_file_type(list_para->file_item);
	switch( media_type )
	{
		case RAT_MEDIA_TYPE_ALL:
			explorer_cmd2parent(msg->h_deswin, SWITCH_TO_OTHER_APP, EXPLR_SW_TO_MAIN, 0);
			break;			
		case RAT_MEDIA_TYPE_PIC:
			explorer_cmd2parent(msg->h_deswin, SWITCH_TO_OTHER_APP , EXPLR_SW_TO_PHOTO , 0);
			break;
		case RAT_MEDIA_TYPE_AUDIO:
		{
			//char full_file_name[RAT_MAX_FULL_PATH_LEN] = { 0 } ;
			file_item_t* file_item = NULL;
			char  path[RAT_MAX_FULL_PATH_LEN] = { 0 };

			__here__ ;
			file_item = list_para->cur_file_list->cur_item;
			//eLIBs_memset(path, 0, sizeof(path));
			eLIBs_strcpy(path,list_para->cur_file_list->file_path);
			eLIBs_strcat(path, "\\");
			eLIBs_strcat(path,file_item->name);	
			__msg("file_manager_set_current_file_for_play  path = %s \n " , path );
		//	eLIBs_memset(list_para->music_play_file,0,sizeof(list_para->music_play_file));
			
		//	eLIBs_strcpy(list_para->music_play_file , path ) ; 
			eLIBs_strcpy( list_para->root_para->music_file_name , path ) ; 

			__msg("full_file_name=%s\n", list_para->root_para->music_file_name );
			explorer_cmd2parent(msg->h_deswin, SWITCH_TO_OTHER_APP, EXPLR_SW_TO_MUSIC , 0);
		}
		break;
		case RAT_MEDIA_TYPE_VIDEO:
			explorer_cmd2parent(msg->h_deswin, SWITCH_TO_OTHER_APP, EXPLR_SW_TO_MOVIE , 0 );
			break;
		case RAT_MEDIA_TYPE_EBOOK:
			explorer_cmd2parent(msg->h_deswin, SWITCH_TO_OTHER_APP, EXPLR_SW_TO_EBOOK,  0);
			break;	
		default:
			break;
	}
	
	hExpWin = GUI_WinGetParent(msg->h_deswin);
	ExplorerSendMessage(hExpWin,GUI_MSG_CLOSE, 0, 0);
	return EPDK_OK;
}

//
/*
************************************************************************************************************************
*Function	:           H_WIN explorer_list_win_create(H_WIN h_parent, explr_list_para_t *para)
*
*Description	:		����ManagerWin���ݹ����Ĳ�������FrameWIn����
*					
*Arguments  	: 		
*
*Return     	: 	
*
************************************************************************************************************************
*/
H_WIN explorer_list_win_create(H_WIN h_parent, explr_list_para_t *para)
{
	__gui_framewincreate_para_t framewin_para;
	explr_list_para_t *list_para;
	FB fb;
	
	GUI_LyrWinGetFB(para->list_layer, &fb);
	
	list_para = (explr_list_para_t *)esMEMS_Balloc(sizeof(explr_list_para_t));
	eLIBs_memset((void*)list_para, 0, sizeof(explr_list_para_t));
	
	list_para->explr_list_font = para->explr_list_font;
	//list_para->BG_layer = para->BG_layer;
	list_para->list_layer = para->list_layer;
	list_para->core = para->core;
	
	list_para->media_type = para->media_type;		//photo, music, movie
	
	list_para->root_type = para->root_type;			//SD or USB device	
	
	//list_para->last_focused_id = para->last_focused_id;
	//list_para->last_start_id = para->last_start_id;
	//list_para->last_filename = para->last_filename;
	
	list_para->view_mode = para->view_mode;			//list or square
	list_para->root_para = para->root_para ;
	ExplorerListWinGetSearchPath(list_para);
	
	//eLIBs_memcpy(list_para->search_path,"e:", sizeof("e:"));

	eLIBs_memset(&framewin_para, 0, sizeof(__gui_framewincreate_para_t));
	framewin_para.name =	"Explorer List window",
	framewin_para.dwExStyle = WS_EX_NONE;
	framewin_para.dwStyle = WS_NONE|WS_VISIBLE;
	framewin_para.spCaption =  NULL;
	framewin_para.hOwner	= NULL;
	framewin_para.id        = EXPLR_LIST_WIN_ID;
	framewin_para.hHosting = h_parent;
	framewin_para.FrameWinProc = (__pGUI_WIN_CB)esKRNL_GetCallBack((__pCBK_t)_explorer_list_win_cb);
	framewin_para.rect.x =0;
	framewin_para.rect.y = 0;
	framewin_para.rect.width= fb.size.width;
	framewin_para.rect.height = fb.size.height;
	framewin_para.BkColor.alpha =  0;
	framewin_para.BkColor.red = 0;
	framewin_para.BkColor.green = 255;
	framewin_para.BkColor.blue = 0;
	framewin_para.attr =  (void *)list_para;
	framewin_para.hLayer = list_para->list_layer;			//�ᱻlistbar����
	return (GUI_FrmWinCreate(&framewin_para));
}

/*
************************************************************************************************************************
*Function	:           static __s32 explorer_listview_onpaint(H_WIN  list_win)
*
*Description	:		OnPaint������,������ͼ��ǰ��ListBar showpage, ����iconԪ��
*					
*Arguments  	: 		
*
*Return     	: 	
*
************************************************************************************************************************
*/
static __s32 explorer_listview_onpaint(H_WIN  list_win)
{
	
	//GUI_RECT title_rect;
	__s32 ret = 0;
	explr_list_para_t *list_para;	
	list_para = (explr_list_para_t *)GUI_WinGetAttr(list_win);	
	if (GUI_LyrWinGetSta(list_para->list_layer) == GUI_LYRWIN_STA_SUSPEND)
		return EPDK_FAIL;
	if (GUI_LyrWinGetSta(list_para->list_layer) == GUI_LYRWIN_STA_ON)
	{
		__msg("to make sure layer status on\n");
		
	}

	//__wait__;
	
	//if (GUI_LyrWinGetSta(list_para->BG_layer) == GUI_LYRWIN_STA_SUSPEND)
	//	return EPDK_FAIL;	
	//__wait__;		
	//eLIBs_memset(explorer_title_str, 0,sizeof(explorer_title_str));	

	//GUI_LyrWinSetBottom(list_para->BG_layer);
	GUI_LyrWinSetTop(list_para->list_layer);
	
	//GUI_LyrWinSel(list_para->BG_layer);	
	
	//get_menu_text(STRING_EXPLR_TITLE,explorer_title_str, GUI_TITLE_MAX);
	
	GUI_SetFont(SWFFont);
	//GUI_SetFont(GUI_GetDefaultFont());
	GUI_UC_SetEncodeUTF8();
	//GUI_SetBkColor(GUI_LIGHTGREEN);	
	//GUI_SetColor(GUI_RED);
	GUI_SetDrawMode(GUI_DRAWMODE_NORMAL);	
	
	//GUI_SetColor(APP_COLOR_WHITE);		
		
	//GUI_ClearRect(0,0, 400, 240);
	
	//title_rect.x0 = 0;
	//title_rect.y0 = 0;
	//title_rect.x1 = 400;
	//title_rect.y1 = 20;
	
	//ret = com_set_palette_by_id(ID_EXPLORER_PALETTE_BMP);	
	//__msg("set palette return value = %d", ret);
	
	//explorer_draw_background(list_para);			//8bpp����ͼ��Drawʱ��Ҫ������palette
	
	//GUI_DrawLine(0,100,400,100);			//only for test
	
	//GUI_ClearRect(title_rect.x0,title_rect.y0, title_rect.x1, title_rect.y1);
	//GUI_DispStringInRect(explorer_title_str, &title_rect, GUI_TA_HCENTER|GUI_TA_VCENTER);//ˮƽ�ʹ�ֱ�������
	//GUI_LyrWinFlushFB(list_para->list_layer);     //flush display data to frame buffer
		
	//__wait__;	
	//GUI_BMP_Draw(theme_hdl2buf(explorer_list_bmp[0]), 0, 50);	
	GUI_WinSetFocusChild(list_win);		//frame win get focus
	
	//GUI_LyrWinSetSta(list_para->list_layer, GUI_LYRWIN_STA_ON);	//set picture layer status as ON
	//GUI_LyrWinSetTop(list_para->list_layer);						//set picture layer as top level	
	//create listbar component
	//LISTBAR_Config
	//explorer_listbar_init(list_win);	
	GUI_LyrWinSel(list_para->list_layer);	

	
	
	LISTBAR_ShowPage(list_para->listbar_handle);              
	//LISTBAR_LostFocus(list_para->listbar_handle);	
	//LISTBAR_SetFocusItem(list_para->listbar_handle, list_para->rat.index);
		
	if(list_para->media_type == RAT_MEDIA_TYPE_ALL)
	{
		explorer_draw_file_manager_title();
	}
	else
	{
		explorer_draw_FileTypeIcon(list_para, list_para->media_type);	
	}
	return EPDK_OK;
}

/*
************************************************************************************************************************
*Function	:           static __s32  explorer_list_win_on_close(__gui_msg_t *msg)
*
*Description	:		OnClose���������ͷ�FrameWin
*					
*Arguments  	: 		
*
*Return     	: 	
*
************************************************************************************************************************
*/
static __s32  explorer_list_win_on_close(__gui_msg_t *msg)
{
	//theme_close();
	GUI_FrmWinDelete(msg->h_deswin);			//send destroy message
	return EPDK_OK;
}

/*
************************************************************************************************************************
*Function	:           static __s32  explorer_list_win_on_destroy(__gui_msg_t *msg)
*
*Description	:		OnDestroy���������ͷ�framewin �����������Դ
*					
*Arguments  	: 		
*
*Return     	: 	
*
************************************************************************************************************************
*/
static __s32  explorer_list_win_on_destroy(__gui_msg_t *msg)
{	
	explr_list_para_t *list_para;	
	list_para = (explr_list_para_t *)GUI_WinGetAttr(msg->h_deswin);	
	
	if(GUI_IsTimerInstalled(list_para->list_win, ExplorerTimerId))
	{
		GUI_KillTimer(list_para->list_win, ExplorerTimerId);
	}
	
	if(list_para->h_dialog!=NULL)
	{
		app_dialog_destroy(list_para->h_dialog); 		//to delete dialog	
	}
	
 	if(list_para->h_dialog_msg!=NULL)
	{
		app_dialog_destroy(list_para->h_dialog_msg); 		//to delete dialog	
	}	
#if EXTERN_KARAOKE_EXIST

	/*if(list_para->ppstr)
	{
	    __s32 i;
		__kak_song_t  psong_info;
		for(i = 0 ; i < list_para->rat.total ; i++)
		{
			if(list_para->ppstr[i])
			{
				esMEMS_Bfree(list_para->ppstr[i],(strlen(psong_info.str_song_name)+1));
				__wrn("bfree %d\n",i);
			}
		}
		esMEMS_Bfree(list_para->ppstr,sizeof(char*)*(list_para->rat.total));
		__wrn("bfree ppstr\n");;
	}*/
#endif
	if(list_para->media_type == RAT_MEDIA_TYPE_ALL)
	{
        __here__;
		explorer_file_list_uninit(list_para);
        __here__;
	}
	else
	{        
        __here__;
		explorer_save_last_para(list_para);
	}
    __here__;
	
	explorer_list_long_string_uninit(list_para);		//�ͷ�long string scroll

    __here__;
    explorer_listbar_uninit(msg->h_deswin); 

    if(list_para->media_type == RAT_MEDIA_TYPE_ALL)
	{        
	}
	else
	{   
#if EXTERN_KARAOKE_EXIST
		if(RAT_MEDIA_TYPE_KARAOKE == list_para->media_type)
		{
			explorer_karaoke_uninit(list_para);
		}
		else
#endif			
		{
			explorer_rat_uninit(list_para);
		}	
	}
    
	__here__;
	explorer_free_listview_icon_res();	
    __here__;	

	esMEMS_Bfree(list_para, sizeof(explr_list_para_t));
    __here__;
	//explorer_gui_long_string_deinit();
    //_browser_scene_layer_deinit();
    //cat_core_close(browser_param->cat_handle); 
    //browser_param->cat_handle = NULL;	    
    
    return EPDK_OK;
}

/*
************************************************************************************************************************
*Function	:          __s32 ExplorerListWinOnEnterKey(explr_list_para_t *list_para)
*
*Description	:		EnterKey������
*					
*Arguments  	: 		
*
*Return     	: 	
*
************************************************************************************************************************
*/
__s32 ExplorerListWinOnEnterKey(explr_list_para_t *list_para)
{
	char FileName[RAT_MAX_FULL_PATH_LEN];
	__s32 ret;
#if EXTERN_KARAOKE_EXIST
	reg_karaoke_para_t* karaoke_para;
#endif

	eLIBs_memset(FileName,0,sizeof(FileName));	
#if EXTERN_KARAOKE_EXIST // add by hot.lee
	if(list_para->media_type == RAT_MEDIA_TYPE_KARAOKE)
	{
		GetKaraokeListItemFileFullPath(list_para, list_para->rat.index, FileName);		
	}
	else
#endif
	{
	ret = GetListItemFileFullPath(list_para, list_para->rat.index, FileName);		
	}
    if(EPDK_FAIL == ret)
    {
        return EPDK_FAIL;
    }

#if EXTERN_KARAOKE_EXIST // add by hot.lee
	if(list_para->media_type == RAT_MEDIA_TYPE_KARAOKE)
	{
		//ֻ��Ҫindex
		karaoke_para = (reg_karaoke_para_t*)dsk_reg_get_para_by_app(REG_APP_KARAOKE);
		karaoke_para->focus_item_id = list_para->rat.index;
	}
	else	
#endif
	{
	rat_set_file_for_play(list_para->rat.handle, FileName);
	}
	__msg("~~~~Set play file name is  = %s\n", FileName);
/*
	if(list_para->view_mode == EXPLR_LIST_MODE)
	{
		ExplorerSetViewMode(list_para, EXPLR_SQUARE_MODE);
	}
	else if(list_para->view_mode == EXPLR_SQUARE_MODE)
	{
		ExplorerSetViewMode(list_para, EXPLR_LIST_MODE);
	}	
*/	
	return EPDK_OK;
}
/*
************************************************************************************************************************
*Function	:          __s32 ExplorerListWinOnNextItem(explr_list_para_t *list_para)
*
*Description	:		����һ����Ŀ������,��NextKey�б�����
*					
*Arguments  	: 		
*
*Return     	: 	
*
************************************************************************************************************************
*/

__s32 ExplorerListWinOnNextItem(explr_list_para_t *list_para)
{
	__s32 ret;
	__s32 nStep = 1;
	//__s32 PageCount = LISTBAR_GetPageItemCount(list_para->listbar_handle);	//���һ�е���Ŀ��	

/*
	if(list_para->rat.index + 1 >= list_para->rat.total)
	{
		return EPDK_FAIL;			//only for test 
	}
	ret = rat_move_cursor_forward(list_para->rat.handle, nStep);//����ʵ���ƶ��Ĳ���
	if(ret == 0)	//0����ʾ�Ѿ��Ҳ����ļ���
	{
		if(rat_is_cursor_end(list_para->rat.handle))
		{
			return EPDK_FAIL;
		}			
		else
		{
			__wrn("there is a exception in rat!!!\n");
			return EPDK_FAIL;
		}
	}	
*/
	{
		__listbar_scene_t ListBarScene;		
		
		LISTBAR_NextItem(list_para->listbar_handle);
		
		LISTBAR_GetScene(list_para->listbar_handle, &ListBarScene);
		__msg("listbar start id = %d\n", ListBarScene.start_id);
		__msg("listbar focus id = %d\n", ListBarScene.focus_id);		
		list_para->rat.index = ListBarScene.focus_id;
	}	
	return EPDK_OK;	
}
/*
************************************************************************************************************************
*Function	:          __s32 ExplorerListWinOnNextItem(explr_list_para_t *list_para)
*
*Description	:		����һ����Ŀ������,��PreviousKey�б�����
*					
*Arguments  	: 		
*
*Return     	: 	
*
************************************************************************************************************************
*/
__s32 ExplorerListWinOnPreviousItem(explr_list_para_t *list_para)
{
	__s32 ret;
	__s32 nStep = 1;
	/*
	if(list_para->rat.index <= 0)
	{
		return EPDK_FAIL;
	}
	
	ret = rat_move_cursor_backward(list_para->rat.handle, nStep);	
	if(ret == 0)//0����ʾ�Ѿ��Ҳ����ļ���
	{
		return EPDK_FAIL;
	}
	//list_para->rat.index--;
	*/
	{
		__listbar_scene_t ListBarScene;
		
		LISTBAR_PrevItem(list_para->listbar_handle);
		LISTBAR_GetScene(list_para->listbar_handle, &ListBarScene);
		__msg("listbar start id = %d\n", ListBarScene.start_id);
		__msg("listbar focus id = %d\n", ListBarScene.focus_id);
		list_para->rat.index = ListBarScene.focus_id;
	}	
	
	return EPDK_OK;
}

/*
************************************************************************************************************************
*Function	:          __s32 ExplorerListWinOnNextItem(explr_list_para_t *list_para)
*
*Description	:		��Ӧ��һ����Ŀ������,��PreviousKey�б�����
*					
*Arguments  	: 		
*
*Return     	: 	
*
************************************************************************************************************************
*/
__s32 ExplorerListWinOnNextRow(explr_list_para_t *list_para)
{
	__s32 ret;
	__s32 RowCount = LISTBAR_GetRowItemCount(list_para->listbar_handle);	//���һ�е���Ŀ��


	ret = rat_move_cursor_forward(list_para->rat.handle, RowCount);
	if(ret == 0)
	{
		if(rat_is_cursor_end(list_para->rat.handle))
		{
			return EPDK_FAIL;
		}			
		else
		{
			__wrn("there is a exception in rat!!!\n");
			return EPDK_FAIL;
		}
	}

	list_para->rat.index += ret;			//retΪʵ�������ƶ�����Ŀ��
	
	if(list_para->rat.index >= list_para->rat.total)
	{
		list_para->rat.total = list_para->rat.index;
		LISTBAR_SetTotalItem(list_para->listbar_handle,list_para->rat.total);		
	}	
	
	LISTBAR_NextRow(list_para->listbar_handle);
	return EPDK_OK;
}

/*
************************************************************************************************************************
*Function	:          __s32 ExplorerListWinOnNextItem(explr_list_para_t *list_para)
*
*Description	:		����һ����Ŀ������,��PreviousKey�б�����
*					
*Arguments  	: 		
*
*Return     	: 	
*
************************************************************************************************************************
*/
__s32 ExplorerListWinOnPreviousRow(explr_list_para_t *list_para)
{
	__s32 ret;
	__s32 RowCount = LISTBAR_GetRowItemCount(list_para->listbar_handle);	//���һ�е���Ŀ��

	if(list_para->rat.index <= 0)
	{
		return EPDK_FAIL;
	}
	
	
	ret = rat_move_cursor_backward(list_para->rat.handle, RowCount);
	if(ret == 0)
	{
		if(rat_is_cursor_end(list_para->rat.handle))
		{
			return EPDK_FAIL;
		}			
		else
		{
			__wrn("there is a exception in rat!!!\n");
			return EPDK_FAIL;
		}
	}
	if(list_para->rat.index >= ret)
	{
		list_para->rat.index -= ret;			//retΪʵ�������ƶ�����Ŀ��
	}
	else
	{
		list_para->rat.index = 0;				//�Ѿ��ڵ�һҳ����λ����һ����Ŀ
		LISTBAR_SetFocusItem(list_para->listbar_handle, list_para->rat.index);		
		//__wrn("Explorer_list.c, line296, there is a exception with listview!!!\n");
		return EPDK_OK;	
	}
	
	LISTBAR_PrevRow(list_para->listbar_handle);
	return EPDK_OK;
}


//����һҳ������
__s32 ExplorerListWinOnNextPage(explr_list_para_t *list_para)
{
	__s32 ret;
	__s32 PageCount = LISTBAR_GetPageItemCount(list_para->listbar_handle);	//���һ�е���Ŀ��

	if(list_para->rat.index + PageCount >= list_para->rat.total)
	{
		return EPDK_FAIL;			//only for test
	}
	ret = rat_move_cursor_forward(list_para->rat.handle, PageCount);
	
	if(ret == 0)
	{
		if(rat_is_cursor_end(list_para->rat.handle))
		{
			return EPDK_FAIL;
		}			
		else
		{
			__wrn("there is a exception in rat!!!\n");
			return EPDK_FAIL;
		}
	}
	/*	
	if(list_para->rat.index + ret >= list_para->rat.total)
	{
		list_para->rat.total = list_para->rat.index;
		LISTBAR_SetTotalItem(list_para->listbar_handle,list_para->rat.total);		
	}	
	*/
	{
		__listbar_scene_t ListBarScene;
		LISTBAR_GetScene(list_para->listbar_handle, &ListBarScene);
		__msg("listbar start id = %d\n", ListBarScene.start_id);
		__msg("listbar focus id = %d\n", ListBarScene.focus_id);		
	}	
	
	//list_para->rat.index += ret;			//retΪʵ�������ƶ�����Ŀ��	
	{
		__listbar_scene_t ListBarScene;
		LISTBAR_NextPage(list_para->listbar_handle);
		
		LISTBAR_GetScene(list_para->listbar_handle, &ListBarScene);
		__msg("listbar start id = %d\n", ListBarScene.start_id);
		__msg("listbar focus id = %d\n", ListBarScene.focus_id);		
					
		list_para->rat.index = ListBarScene.focus_id;		
	}	
	//LISTBAR_SetFocusItem(list_para->listbar_handle, list_para->rat.index);	
	return EPDK_OK;
}


//����һҳ������
__s32 ExplorerListWinOnPreviousPage(explr_list_para_t *list_para)
{
	__s32 ret;
	__s32 PageCount = LISTBAR_GetPageItemCount(list_para->listbar_handle);	//���һ�е���Ŀ��
	
	if(list_para->rat.index <= 0)
	{
		return EPDK_FAIL;
	}
	if(list_para->rat.index < PageCount)		//only for test
	{			
		return EPDK_FAIL;
	}
	
	ret = rat_move_cursor_backward(list_para->rat.handle, PageCount);
	if(ret == 0)
	{
		if(rat_is_cursor_end(list_para->rat.handle))
		{
			return EPDK_FAIL;
		}			
		else
		{
			__wrn("there is a exception in rat!!!\n");
			return EPDK_FAIL;
		}
	}
	
	{
		__listbar_scene_t ListBarScene;
		LISTBAR_PrevPage(list_para->listbar_handle);	
		
		LISTBAR_GetScene(list_para->listbar_handle, &ListBarScene);
		__msg("listbar start id = %d\n", ListBarScene.start_id);
		__msg("listbar focus id = %d\n", ListBarScene.focus_id);		
		list_para->rat.index = ListBarScene.focus_id;
	}

	//LISTBAR_SetFocusItem(list_para->listbar_handle, list_para->rat.index);	
	return EPDK_OK;
}


__s32 explorer_create_delete_failed_dialog(H_WIN list_win)
{
	explr_list_para_t *list_para;	
	
	if(list_win == NULL)
	{
		return EPDK_FAIL;
	}
	list_para = (explr_list_para_t *)GUI_WinGetAttr(list_win);
	
	
	list_para->del_dlg_open = EPDK_TRUE;	
	explorer_draw_delete_file_failed();
	__msg("--------------draw del file dialog!\n");
	
	if(GUI_IsTimerInstalled(list_win, ExplorerTimerId))
	{
		GUI_ResetTimer(list_win, ExplorerTimerId, C_EXP_TIMER_INTERVAL, NULL);
		//GUI_KillTimer(list_win, ExplorerTimerId);
		__msg("GUI_reset timer\n");
	}	
}

static __s32  file_manager_list_on_delete(__gui_msg_t *msg)
{
	explr_list_para_t *list_para;
	file_item_t* file_item = NULL;
	int ret = 0;
	__listbar_scene_t ListBarScene;
	char  path[RAT_MAX_FULL_PATH_LEN];
	
	list_para = (explr_list_para_t *)GUI_WinGetAttr(msg->h_deswin);
	if(list_para == NULL)
	{
		return EPDK_FAIL;
	}
	if(list_para->cur_file_list == NULL)
	{
		return EPDK_FAIL;
	}


	if(list_para->cur_file_list->total < 1)
	{
		return EPDK_FAIL;
	}
	file_item = list_para->cur_file_list->cur_item;
	if(file_item == NULL)
	{
		return EPDK_FAIL;
	}
	eLIBs_memset(path, 0, sizeof(path));
	eLIBs_strcpy(path,list_para->cur_file_list->file_path);
	eLIBs_strcat(path, "\\");
	eLIBs_strcat(path,file_item->name);	
	
	if(file_item->fatdirattr & FSYS_ATTR_DIRECTORY)
	{
		if(file_item->name_len != 0)
		{
			//ret = eLIBs_rmdir(path);
			{
				file_op_info_t op_info;
				eLIBs_memset(&op_info, 0, sizeof(file_op_info_t));
				ret = get_item_num_in_dir(path, &(op_info.total_num));
				if(EPDK_FAIL == ret)
				{
					return EPDK_FAIL;
				}
				//op_info.OnDraw = explorer_delete_file_draw_process;
				explorer_delete_file_draw_process_ext();
				ret = delete_dir(path, &op_info);
				if(ret != 0)
				{
					#if 0
					{
						__s32 str[] = {0, STRING_EXPLR_DELETE_FAILED};
						H_WIN parent = GUI_WinGetParent(msg->h_deswin);
						//__wait__;
						default_dialog_timeout(list_para->h_dialog_msg,parent, DELETE_FILE_DIALOG_ID, ADLG_OK, str,18000);					
					}	
					#endif
					//explorer_draw_delete_file_failed();
					LISTBAR_ShowPage(list_para->listbar_handle);
					explorer_create_delete_failed_dialog(list_para->list_win);
					//LISTBAR_ShowPage(list_para->listbar_handle);
						__here__;
					return EPDK_FAIL;	
				}
			}
		}
	}
	else
	{
		if(file_item->name_len != 0)
		{
			explorer_delete_file_draw_process_ext();
			ret = eLIBs_remove(path);	
			if(ret != 0)
			{
				#if 0			
				{
					__s32 str[] = {0, STRING_EXPLR_DELETE_FAILED};
					H_WIN parent = GUI_WinGetParent(msg->h_deswin);
					//__wait__;
					default_dialog_timeout(list_para->h_dialog_msg,parent, DELETE_FILE_DIALOG_ID, ADLG_OK, str,18000);					
				}				
				#endif
				
				//explorer_draw_delete_file_failed();
				LISTBAR_ShowPage(list_para->listbar_handle);
				explorer_create_delete_failed_dialog(list_para->list_win);
				//LISTBAR_ShowPage(list_para->listbar_handle);	
					__here__;
				return EPDK_FAIL;	
			}						
		}		
	}	
	__here__;
	explorer_clean_delete_file_hint_area();
	
	//�����ֳ�	
	LISTBAR_GetScene(list_para->listbar_handle, &ListBarScene);
	list_para->cur_file_list->start_id = ListBarScene.start_id;
	list_para->cur_file_list->focus_id = ListBarScene.focus_id;			

	list_para->cur_file_list->total -= 1;
	list_para->rat.total = list_para->cur_file_list->total;			//only for test here
		
	list_para->last_start_id = ListBarScene.start_id;
	list_para->last_focused_id = ListBarScene.focus_id;
	if(list_para->last_focused_id >= list_para->rat.total)
	{
		if(list_para->rat.total > 1)
		{
			if(list_para->last_start_id > 0)
			{
				list_para->last_start_id--;
			}
			list_para->last_focused_id = list_para->rat.total - 1;
		}
		else
		{
			list_para->last_focused_id = 0;
		}
	}
	
	ret = delete_file_item_from_list(file_item, list_para->cur_file_list);	// ɾ����Ŀ
		
	explorer_listbar_uninit(list_para->list_win);							//���´���listbar,��ΪSquareΪȫ��ģʽ
	explorer_listbar_init(list_para->list_win);
	LISTBAR_ShowPage(list_para->listbar_handle);	
	return EPDK_OK;
}

//�ļ�����ص���һ���˵�
static __s32  file_manager_list_on_backspace(__gui_msg_t *msg)
{
	explr_list_para_t *list_para;
	file_list_t* parent_file_list = NULL;
	list_para = (explr_list_para_t *)GUI_WinGetAttr(msg->h_deswin);
	if(list_para == NULL)
	{
		return EPDK_FAIL;
	}
	if(list_para->cur_file_list == NULL)
	{
		return EPDK_FAIL;
	}
	
	if(list_para->cur_file_list == list_para->top_file_list)
	{
        __here__;
		parent_file_list = list_para->cur_file_list->parent;
		
        delete_file_list_nod(list_para->cur_file_list);	    
		list_para->cur_file_list = parent_file_list;	
        if(list_para->cur_file_list)
        {
            list_para->cur_file_list->child = NULL;
        }
		explorer_cmd2parent(msg->h_deswin, SWITCH_TO_OTHER_APP, EXPLR_SW_TO_MAIN, 0);
        __here__;
		return EPDK_OK;
	}
	if(list_para->cur_file_list->parent == NULL)
	{
        __here__;
		explorer_cmd2parent(msg->h_deswin, SWITCH_TO_OTHER_APP, EXPLR_SW_TO_MAIN, 0);
        __here__;
		return EPDK_OK;	
	}
	
	parent_file_list = list_para->cur_file_list->parent;
	
    delete_file_list_nod(list_para->cur_file_list);   
	list_para->cur_file_list = parent_file_list;
    if(list_para->cur_file_list)
    {
        list_para->cur_file_list->child = NULL;
    }
	list_para->last_start_id = list_para->cur_file_list->start_id;
	list_para->last_focused_id = list_para->cur_file_list->focus_id;
	list_para->rat.index = list_para->last_focused_id;
	list_para->rat.total = list_para->cur_file_list->total;
	
	explorer_listbar_uninit(list_para->list_win);		
	explorer_listbar_init(list_para->list_win);
	LISTBAR_ShowPage(list_para->listbar_handle);
    __here__;
	return EPDK_OK;
}

//�ļ����������һ���˵�
static __s32  file_manager_list_on_enter_key(__gui_msg_t *msg)
{	
	explr_list_para_t *list_para;	
	__listbar_scene_t ListBarScene;
	file_list_t *temp_file_list = NULL;
	char  path[RAT_MAX_FULL_PATH_LEN];
	__u8  fatdirattr;
	list_para = (explr_list_para_t *)GUI_WinGetAttr(msg->h_deswin);

	if(list_para == NULL)
	{
		return EPDK_FAIL;
	}
	if(list_para->cur_file_list == NULL)
	{
		return EPDK_FAIL;
	}
	if(list_para->cur_file_list->cur_item== NULL)
	{
		return EPDK_FAIL;
	}
	fatdirattr = list_para->cur_file_list->cur_item->fatdirattr;
	if((fatdirattr & FSYS_ATTR_DIRECTORY)
		||(fatdirattr == RAT_MEDIA_TYPE_USB_DEVICE)
		||(fatdirattr == RAT_MEDIA_TYPE_SD_DEVICE))
	{
		explorer_clean_delete_file_hint_area();
		explorer_list_long_string_stop_roll(list_para);		
			//������һ�����ļ���
		LISTBAR_GetScene(list_para->listbar_handle, &ListBarScene);
		list_para->cur_file_list->start_id = ListBarScene.start_id;
		list_para->cur_file_list->focus_id = ListBarScene.focus_id;			
		if(list_para->cur_file_list == list_para->top_file_list)
		{
			eLIBs_strcpy(path,list_para->cur_file_list->cur_item->name);			
			//eLIBs_strcat(path, "\\");
		}
		else
		{
			eLIBs_strcpy(path,list_para->cur_file_list->file_path);
			eLIBs_strcat(path, "\\");
			eLIBs_strcat(path,list_para->cur_file_list->cur_item->name);			
		}

		__msg("\n\n~~~ new path = %s ~~~\n\n", path);
		temp_file_list = new_file_list_nod(path, NULL);		
		if(temp_file_list == NULL)
		{		
			return EPDK_FAIL;
		}
		
		list_para->rat.total = temp_file_list->total;			//only for test here
		list_para->cur_file_list->child = temp_file_list;
		temp_file_list->parent = list_para->cur_file_list;	
		list_para->cur_file_list = temp_file_list;
		
		list_para->last_start_id = 0;
		list_para->last_focused_id = 0;
		list_para->rat.index = 0;
		
		explorer_listbar_uninit(list_para->list_win);		//���´���listbar,��ΪSquareΪȫ��ģʽ
		explorer_listbar_init(list_para->list_win);
		LISTBAR_ShowPage(list_para->listbar_handle);
		
	}
	else	
	{        
        rat_media_type_t media_type;
        file_item_t* file_item ;

        file_item = get_file_list_item( list_para->cur_file_list, list_para->rat.index );
        media_type = rat_get_file_type( file_item->name );

        if( (media_type == RAT_MEDIA_TYPE_AUDIO) ||(media_type == RAT_MEDIA_TYPE_VIDEO)
                || (media_type == RAT_MEDIA_TYPE_PIC) || (media_type == RAT_MEDIA_TYPE_EBOOK ) )
        {
            file_manager_set_current_file_for_play( msg );
        }
        else if(media_type == RAT_MEDIA_TYPE_FIRMWARE)// �̼�����
        {        
            if(1)
            {
                file_item_t* file_item = NULL;
                char  path[RAT_MAX_FULL_PATH_LEN] = { 0 };

    			__here__ ;
    			file_item = list_para->cur_file_list->cur_item;
    			//eLIBs_memset(path, 0, sizeof(path));
    			eLIBs_strcpy(path,list_para->cur_file_list->file_path);
    			eLIBs_strcat(path, "\\");
    			eLIBs_strcat(path,file_item->name);	
                eLIBs_strcpy(list_para->fw_update_file, path);
                
                {                     
            		__s32 str[] = {0, STRING_EXPLR_FW_UPDATE_CONFIRM};
            		H_WIN parent = GUI_WinGetParent(list_para->list_win);    		
            		default_dialog(list_para->h_fw_update_dlg,parent, FW_UPDATE_DIALOG_ID, ADLG_YESNO, str);					    		
        		}
                
                return EPDK_FAIL;
            }
            
        }
	}	
    
	return EPDK_OK;
}

static __s32  explorer_list_win_on_enter_key(__gui_msg_t *msg)
{
    __s32 ret;
	H_WIN hExpWin = NULL;
	explr_list_para_t *list_para;	
	list_para = (explr_list_para_t *)GUI_WinGetAttr(msg->h_deswin);
	ret = ExplorerListWinOnEnterKey(list_para);					
    if(EPDK_FAIL == ret)
    {
        return EPDK_OK;
    }

	switch(list_para->media_type)
	{
		case RAT_MEDIA_TYPE_ALL:
			explorer_cmd2parent(msg->h_deswin, SWITCH_TO_OTHER_APP, EXPLR_SW_TO_MAIN, 0);
			break;			
		case RAT_MEDIA_TYPE_PIC:
			explorer_cmd2parent(msg->h_deswin, SWITCH_TO_OTHER_APP, EXPLR_SW_TO_PHOTO, 0);
			break;
		case RAT_MEDIA_TYPE_AUDIO:
            explorer_cmd2parent(msg->h_deswin, SWITCH_TO_OTHER_APP, EXPLR_SW_TO_MUSIC, 0);
			break;
		case RAT_MEDIA_TYPE_VIDEO:
			explorer_cmd2parent(msg->h_deswin, SWITCH_TO_OTHER_APP, EXPLR_SW_TO_MOVIE, 0);
			break;
		case RAT_MEDIA_TYPE_EBOOK:
			explorer_cmd2parent(msg->h_deswin, SWITCH_TO_OTHER_APP, EXPLR_SW_TO_EBOOK, 0);
			break;	
#if EXTERN_KARAOKE_EXIST // add by hot.lee
		case RAT_MEDIA_TYPE_KARAOKE:
			//..__wrn("RAT_MEDIA_TYPE_KARAOKE1\n");
			{
			reg_root_para_t* reg_root_para;
			
			reg_root_para = (reg_root_para_t*)dsk_reg_get_para_by_app(REG_APP_ROOT);
			if(reg_root_para)
			{
				reg_root_para->rat_index = list_para->rat.index;
			}
			//..reg_root_para->rat_index = list_para->rat.index;
			__wrn("RAT_MEDIA_TYPE_KARAOKE1\n");
			explorer_cmd2parent(msg->h_deswin, SWITCH_TO_OTHER_APP, EXPLR_SW_TO_KARAOKE, list_para->root_type);
			break;
			}
#endif
		default:
			break;
	}
	
	hExpWin = GUI_WinGetParent(msg->h_deswin);
	ExplorerSendMessage(hExpWin,GUI_MSG_CLOSE, 0, 0);
	return EPDK_OK;
}


static __s32  explorer_list_win_on_command(__gui_msg_t *msg)
{
	explr_list_para_t *list_para;	
	list_para = (explr_list_para_t *)GUI_WinGetAttr(msg->h_deswin);
	if (LOWORD(msg->dwAddData1) == DELETE_FILE_DIALOG_ID)
	{
		__here__;		
		switch(HIWORD(msg->dwAddData1))
		{
			case ADLG_CMD_EXIT:
				if(list_para->h_dialog!=NULL)
				{
					app_dialog_destroy(list_para->h_dialog); 		//to delete dialog				
					if (msg->dwAddData2 == ADLG_IDYES)
	 				{
	 					explorer_list_long_string_stop_roll(list_para);
	 					file_manager_list_on_delete(msg);			//ɾ��һ����Ŀ
	 					rat_set_modify_flag_all(1);
	 				}
	 				else//ADLG_IDNO
	 				{
	 				}								
					list_para->h_dialog = NULL;
				}
				if(list_para->h_dialog_msg!=NULL)
				{
					app_dialog_destroy(list_para->h_dialog_msg); 		//to delete dialog				
					list_para->h_dialog_msg = NULL;
					if(list_para->listbar_handle != NULL)
					{
						LISTBAR_ShowPage(list_para->listbar_handle);
					}
				}
				break;
			case ADLG_CMD_SET_UI:
				{//set dialog ui para
					//app_dialog_set_ui(msg->h_srcwin, ADLG_UI_OK, 0, 0);
				}
				break;
			default:
				break;
		}

	}	
    else if (LOWORD(msg->dwAddData1) == FW_UPDATE_DIALOG_ID)
	{
		__here__;		
		switch(HIWORD(msg->dwAddData1))
		{
			case ADLG_CMD_EXIT:
				if(list_para->h_fw_update_dlg!=NULL)
				{
					app_dialog_destroy(list_para->h_fw_update_dlg); 		//to delete dialog				
					if (msg->dwAddData2 == ADLG_IDYES)
	 				{
                        __msg("fw update, file=%s\n", list_para->fw_update_file);
	 					dsk_start_fw_update(list_para->fw_update_file);
	 				}
	 				else//ADLG_IDNO
	 				{
                        __msg("fw not update...\n");
	 				}								
					list_para->h_fw_update_dlg = NULL;
				}
				break;
			case ADLG_CMD_SET_UI:
				{//set dialog ui para
					
				}
				break;
			default:
				break;
		}

	}
	return EPDK_OK;
}
/*
************************************************************************************************************************
*Function	:          static __s32  explorer_list_win_key_proc(__gui_msg_t *msg)
*
*Description	:		framewin ��Ӧ��������
*					
*Arguments  	: 		
*
*Return     	: 	
*
************************************************************************************************************************
*/
static __s32  explorer_list_win_key_proc(__gui_msg_t *msg)
{
	explr_list_para_t *list_para;	
	static __u32 last_key = 0;
	list_para = (explr_list_para_t *)GUI_WinGetAttr(msg->h_deswin);

	if((KEY_DOWN_ACTION == msg->dwAddData2)||(KEY_REPEAT_ACTION == msg->dwAddData2))
	{
		switch(msg->dwAddData1)
		{
			case GUI_MSG_KEY_RIGHT:
			case GUI_MSG_KEY_LONGRIGHT:					
			case GUI_MSG_KEY_DOWN:
			case GUI_MSG_KEY_LONGDOWN:
			case GUI_MSG_KEY_IR_RIGHT:
			case GUI_MSG_KEY_LONGIR_RIGHT:
			case GUI_MSG_KEY_IR_DOWN:
			case GUI_MSG_KEY_LONGIR_DOWN:
				__msg("----explorer list window on UP key----\n");
				//_long_string_stop_roll();
				
				explorer_list_long_string_stop_roll(list_para);
				ExplorerListWinOnNextItem(list_para);				
				break;
			case GUI_MSG_KEY_LEFT:	
			case GUI_MSG_KEY_LONGLEFT:
			case GUI_MSG_KEY_UP:
			case GUI_MSG_KEY_LONGUP:
			case GUI_MSG_KEY_IR_LEFT:
			case GUI_MSG_KEY_LONGIR_LEFT:
			case GUI_MSG_KEY_IR_UP:
			case GUI_MSG_KEY_LONGIR_UP:
				__msg("----explorer list window on DOWN key----\n");
				explorer_list_long_string_stop_roll(list_para);
				ExplorerListWinOnPreviousItem(list_para);
				break;
#if 0	//cai++
			case GUI_MSG_KEY_VADD:				
				//ExplorerListWinOnNextPage(list_para);
				//__wait__;
				__msg("��������\n");
				if(RAT_MEDIA_TYPE_PIC == list_para->media_type)
				{
    				explorer_list_long_string_stop_roll(list_para);
    				if(list_para->view_mode == EXPLR_LIST_MODE)   //������ʾģʽ
    				{
    					ExplorerSetViewMode(list_para, EXPLR_SQUARE_MODE);
    				}
    				else if(list_para->view_mode == EXPLR_SQUARE_MODE)
    				{
    					ExplorerSetViewMode(list_para, EXPLR_LIST_MODE);  //����ͼ��ʾģʽ   ��ģʽ��ʱ��ʾ������ �Ժ��޸ļ���ȥ
    				}
				}
				break;
#endif
			case GUI_MSG_KEY_VDEC:
				//ExplorerListWinOnPreviousPage(list_para);
				break;				
			case GUI_MSG_KEY_ENTER:
				last_key = GUI_MSG_KEY_ENTER;
				
				if(list_para->media_type == RAT_MEDIA_TYPE_ALL)
				{
					list_para->del_dlg_open = EPDK_FALSE;
					file_manager_list_on_enter_key(msg);
				}
				else
				{
					explorer_list_long_string_stop_roll(list_para);		
					explorer_list_win_on_enter_key(msg);
				}
				break;
			case GUI_MSG_KEY_ESCAPE:
				last_key = GUI_MSG_KEY_ESCAPE;
				//explorer_list_long_string_stop_roll(list_para);
				//explorer_cmd2parent(msg->h_deswin, SWITCH_TO_OTHER_APP, EXPLR_SW_TO_MAIN, 0);
				break;
			case GUI_MSG_KEY_RISE:
				break;
				
			case GUI_MSG_KEY_MENU:
				last_key = GUI_MSG_KEY_MENU;
				break;
				
			default:
				break;				
		}
	}
	else if(KEY_UP_ACTION == msg->dwAddData2)
	{
		switch(msg->dwAddData1)
		{
			case GUI_MSG_KEY_ESCAPE:
				if(last_key == GUI_MSG_KEY_ESCAPE)		//�ص���һ���˵�.
				{
                    __here__;
					explorer_list_long_string_stop_roll(list_para);
                    __here__;
					if(list_para->media_type == RAT_MEDIA_TYPE_ALL)
					{
                        __here__;
						file_manager_list_on_backspace(msg);
                        __here__;
					}
					else
					{
						explorer_cmd2parent(msg->h_deswin, SWITCH_TO_OTHER_APP, EXPLR_SW_TO_MAIN, 0);	
					}
				}
				break;
			case GUI_MSG_KEY_MENU:
			case GUI_MSG_KEY_LONGMENU:
#if BEETLES_KEY_REDEF
#else
				if(last_key == GUI_MSG_KEY_MENU)		//ɾ���ļ��л��ļ�
#endif
				{
					if(list_para->cur_file_list == NULL)
					{
						return EPDK_OK;
					}
					if(list_para->cur_file_list == list_para->top_file_list)
					{
						return EPDK_OK;					//����Ŀ¼������ɾ��
					}					
					if(list_para->cur_file_list->total < 1)
					{
						return EPDK_OK;
					}
					
					//explorer_list_on_timer(msg);					
					__here__;
					if(list_para->del_dlg_open)
					{
						list_para->del_dlg_open = EPDK_FALSE;
						explorer_draw_file_info(list_para);	
						__msg("explorer list destory del file failed dialog\n");
					}
					__here__;
					{
					__s32 str[] = {0, STRING_EXPLR_DELETE_CONFIRM};
					H_WIN parent = GUI_WinGetParent(msg->h_deswin);
					//__wait__;
					default_dialog(list_para->h_dialog,parent, DELETE_FILE_DIALOG_ID, ADLG_YESNO, str);					
					//file_manager_list_on_delete(msg);	//ɾ��һ����Ŀ
					}
				}
				break;
		}
	}
	return EPDK_OK;
}
/*
************************************************************************************************************************
*Function	:          static __s32 draw_list_vacant_item(__lbar_draw_para_t *draw_param)
*
*Description	:		���հ���Ŀ
*					
*Arguments  	: 		
*
*Return     	: 	
*
************************************************************************************************************************
*/
static __s32 draw_list_vacant_item(__lbar_draw_para_t *draw_param)
{
	GUI_RECT disp_rect;
	//char *filename;
	//int picX, picY, picW, picH;		//picture X, Y, width, height
	//void *pic_buf;	
	
	//explr_list_para_t *list_para;
	//list_para = (explr_list_para_t *)draw_param->attr;		//for draw the picture  in different media type

	//GUI_LyrWinSel(list_para->list_layer);
	
	//GUI_SetBkColor(GUI_LIGHTGREEN);	
	//GUI_SetColor(GUI_RED);
	
	GUI_SetDrawMode(GUI_DRAWMODE_NORMAL);

	__msg("Vacant item id =: %d\n", draw_param->index);		
	
	disp_rect.x0 = draw_param->rect.x;
	disp_rect.y0 = draw_param->rect.y;
	disp_rect.x1 = draw_param->rect.x + draw_param->rect.width;
	disp_rect.y1 = draw_param->rect.y + draw_param->rect.height;		
	GUI_ClearRect(disp_rect.x0,disp_rect.y0, disp_rect.x1, disp_rect.y1);
	return EPDK_OK;	
}
/*
************************************************************************************************************************
*Function	:          static __s32 ExtractFileName(char *FileName, char *FilePath)
*
*Description	:		��ȫ·���л�ȡ�ļ���
*					
*Arguments  	: 		
*
*Return     	: 	
*
************************************************************************************************************************
*/
static __s32 ExtractFileName(char *FileName, char *FilePath)
{
	char *pFileName;
	pFileName = eLIBs_strchrlast(FilePath, '\\');
	pFileName++;
	eLIBs_strcpy(FileName, pFileName);	
	
	return EPDK_OK;
}

//Ŀǰʹ��ItemInfo.format����mediatype
rat_media_type_t GetListItemFileMediaType(HRAT rat_handle, __s32 ItemIndex)
{

	int ret;
	rat_entry_t ItemInfo;	
	eLIBs_memset(&ItemInfo,0,sizeof(rat_entry_t));		
	ret = rat_get_item_info_by_index(rat_handle, ItemIndex, &ItemInfo);	
	
	return ItemInfo.Format;
}
/*
************************************************************************************************************************
*Function	:          static __s32 GetListItemFileFullPath(explr_list_para_t *list_para, __s32 ItemIndex, char *FilePath)
*
*Description	:		���������Ż����Ŀ�ļ���ȫ·��
*					
*Arguments  	: 		
*
*Return     	: 	
*
************************************************************************************************************************
*/
#if EXTERN_KARAOKE_EXIST // add by hot.lee
static __s32 GetKaraokeListItemFileFullPath(explr_list_para_t *list_para, __s32 ItemIndex, char *FilePath)
{
	//
	int ret;
	__kak_song_t  psong_info;
	eLIBs_memset(&psong_info,0,sizeof(__kak_song_t));
	ret = kak_get_song_info_by_id((H_KAK)list_para->rat.handle, ItemIndex,  &psong_info);
	if(ret == EPDK_FAIL)
		{
			__wrn("get file information form rat fail!!");
			return EPDK_FAIL;
		}	
	
		eLIBs_strcpy(FilePath, psong_info.Path);			//��ȥ����rat �ڴ�
	/*if(list_para->ppstr)
	{
	     list_para->ppstr[ItemIndex] = esMEMS_Balloc(strlen(psong_info.str_song_name)+1);
		 if(list_para->ppstr[ItemIndex])
		 {
			 eLIBs_strcpy(list_para->ppstr[ItemIndex], psong_info.str_song_name);
			 __wrn("list_para->ppstr[%d] = %s\n",ItemIndex,psong_info.str_song_name);
			 }
		 }*/
			//..eLIBs_strcpy(FilePath, "text");
		__wrn("FilePath = %s\n",FilePath);
			return EPDK_OK;

	return EPDK_OK;
}

#endif
static __s32 GetListItemFileFullPath(explr_list_para_t *list_para, __s32 ItemIndex, char *FilePath)
{
	int ret;
	rat_entry_t ItemInfo;
	
	eLIBs_memset(&ItemInfo,0,sizeof(rat_entry_t));	
	
	ret = rat_get_item_info_by_index(list_para->rat.handle, ItemIndex, &ItemInfo);	
	if(ret == EPDK_FAIL)
	{
		__wrn("get file information form rat fail!!");
		return EPDK_FAIL;
	}		
	eLIBs_strcpy(FilePath, ItemInfo.Path);			//��ȥ����rat �ڴ�

	return EPDK_OK;
}

/*
************************************************************************************************************************
*Function	:         static __s32 GetListItemFileName(explr_list_para_t *list_para, __s32 ItemIndex, char *FileName)
*
*Description	:		���������Ż����Ŀ�ļ���(ֻ���ļ���������ʾ)
*					
*Arguments  	: 		
*
*Return     	: 	
*
************************************************************************************************************************
*/
#if EXTERN_KARAOKE_EXIST //add by hot.lee
static __s32 get_krk_listitem_filename(explr_list_para_t *list_para, __s32 ItemIndex, char *FileName)
{
	
	int ret;
	__kak_song_t  psong_info;
	eLIBs_memset(&psong_info,0,sizeof(__kak_song_t));
	/*if(list_para->ppstr[ItemIndex])
	{
		__wrn("ItemIndex = %d\n",ItemIndex);
		eLIBs_strcpy(FileName, list_para->ppstr[ItemIndex]);
	}
	else
	{
		list_para->ppstr[ItemIndex] = esMEMS_Balloc(strlen(psong_info.str_song_name)+1);
		ret = kak_get_song_info_by_id((H_KAK)list_para->rat.handle, ItemIndex,  &psong_info);
	    if(ret == EPDK_FAIL)
		{
			__wrn("get file information form rat fail!!");
			return EPDK_FAIL;
		}
		if(list_para->ppstr[ItemIndex])
		{
			eLIBs_strcpy(list_para->ppstr[ItemIndex], psong_info.str_song_name);
			eLIBs_strcpy(FileName, list_para->ppstr[ItemIndex]);
			__wrn("list_para->ppstr[%d] = %s\n",ItemIndex,psong_info.str_song_name);
		}
		else
		{	
			__wrn("Balloc list_para->ppstr[%d] fail!!\n",ItemIndex);
			return EPDK_FAIL;
		}
		
	}*/
	ret = kak_get_song_info_by_id((H_KAK)list_para->rat.handle, ItemIndex,  &psong_info);
	if(ret == EPDK_FAIL)
		{
			__wrn("get file information form rat fail!!");
			return EPDK_FAIL;
		}	
	
	//..__wrn("psong_info.name =%s\n",psong_info.str_song_name);
	
	
	eLIBs_strcpy(FileName, psong_info.str_song_name);
	
	//eLIBs_strcpy(FileName, "textttttttttttttt");
	__wrn("FileName =%s\n",FileName);

	return EPDK_OK;
}

#endif
static __s32 GetListItemFileName(explr_list_para_t *list_para, __s32 ItemIndex, char *FileName)
{
	int ret;
	char FilePath[RAT_MAX_FULL_PATH_LEN];
	eLIBs_memset(FilePath,0,sizeof(FilePath));
	
	ret = GetListItemFileFullPath(list_para, ItemIndex, FilePath);
	if(ret == EPDK_FAIL)
	{
		return EPDK_FAIL;
	}
	
	ExtractFileName(FileName, FilePath);
	return EPDK_OK;
}
/*
************************************************************************************************************************
*Function	:         static __s32 draw_miniature_view_item(__lbar_draw_para_t *draw_param)
*
*Description	:		������ͼin square mode
*					
*Arguments  	: 		
*
*Return     	: 	
*
************************************************************************************************************************
*/
static __s32 draw_miniature_view_item(__lbar_draw_para_t *draw_param)
{
	explr_list_para_t *list_para;	
	
	char FilePath[RAT_MAX_FULL_PATH_LEN];
	int ret = 0;
 	void *pBuf= NULL;
	//explorer_viewer_ui_t* ui_param;
	
	rat_miniature_para_t in_para;
	rat_pic_info_t out_para;
	RECT miniature_rect;

	list_para = (explr_list_para_t *)draw_param->attr;		//for draw the picture  in different media type
	if(!(list_para->media_type == RAT_MEDIA_TYPE_PIC))		//ͼƬʱ��������ͼ
	{
		return EPDK_FAIL;
	}

	if(list_para->view_mode == EXPLR_LIST_MODE)
	{
		return EPDK_FAIL;
	}
		
	eLIBs_memset(FilePath,0,sizeof(FilePath));	
#if EXTERN_KARAOKE_EXIST
	if(RAT_MEDIA_TYPE_KARAOKE == list_para->media_type)
	{
		GetKaraokeListItemFileFullPath(list_para, draw_param->index, FilePath);
	}
	else
#endif		
	{
  		ret = GetListItemFileFullPath(list_para, draw_param->index, FilePath);	
	}
	if(ret == EPDK_FAIL)
	{
		return EPDK_FAIL;
	}

	ret = explorer_get_item_miniature_rect(list_para, &miniature_rect);
		
	eLIBs_strcpy(in_para.file, FilePath);
	in_para.format = PIXEL_COLOR_ARGB8888;
	in_para.width = miniature_rect.width;
	in_para.height = miniature_rect.height;
	in_para.mode = 0;	// 1Ϊ����ģʽ
	

	pBuf = esMEMS_Balloc(in_para.width*in_para.height*4);	//ARGB8888,���4����С
	if(pBuf == NULL)										//���Էŵ���ʼ���룬�˳�ʱ�ͷ�
	{		
		__wrn("memory is not enough!!");
		return EPDK_FAIL;
	}
	
	//rat_start_miniature_decode();	
	
	eLIBs_memset(&out_para, 0, sizeof(out_para));
	eLIBs_memset(pBuf, 0, in_para.width*in_para.height*4);
	out_para.miniature.buf = pBuf;
	
	ret = EPDK_FAIL;//rat_get_pic_info(&in_para, &out_para);	
	
	//rat_stop_miniature_decode();	
	if (EPDK_OK == ret)
	{
		FB fb;
		eLIBs_memset(&fb, 0, sizeof(FB));
		
	    	fb.size.width = in_para.width;
	        fb.size.height = in_para.height;
	        fb.addr[0] = out_para.miniature.buf ;
	        fb.fmt.type = FB_TYPE_RGB;
	        fb.fmt.fmt.rgb.pixelfmt = PIXEL_COLOR_ARGB8888;
	        fb.fmt.fmt.rgb.br_swap = 0;
	        fb.fmt.fmt.rgb.pixseq = (__rgb_seq_t)0;
	        GUI_BitString_DrawEx(&fb, 	miniature_rect.x + draw_param->rect.x, miniature_rect.y +draw_param->rect.y);								
	}	
	else		// ��ȡ����ͼʧ��
	{
		void * p_bmp_buf ; 
		__msg( "ge miniature fail \n" );
		p_bmp_buf = explorer_get_listview_icon_res( ID_EXP_ERROR_BMP ) ;
		if( !p_bmp_buf )
		{
			__msg("get error bmp res fail\n" );
		}
		GUI_BMP_Draw(p_bmp_buf , miniature_rect.x + draw_param->rect.x, miniature_rect.y +draw_param->rect.y );
		/*
		FB fb;
		eLIBs_memset(&fb, 0, sizeof(FB));
		__msg( "ge miniature fail \n" );
	    	fb.size.width = in_para.width;
	        fb.size.height = in_para.height;
	        fb.addr[0] = explorer_get_listview_icon_res( ID_EXP_ERROR_BMP );
		if( !fb.addr[0] )
		{
			__msg( "get error bmp fail\n" );
		}
	        fb.fmt.type = FB_TYPE_RGB;
	        fb.fmt.fmt.rgb.pixelfmt = PIXEL_COLOR_ARGB8888;
	        fb.fmt.fmt.rgb.br_swap = 0;
	        fb.fmt.fmt.rgb.pixseq = (__rgb_seq_t)0;
	        GUI_BitString_DrawEx(&fb, 	miniature_rect.x + draw_param->rect.x, miniature_rect.y +draw_param->rect.y);								
		*/
	}
	//tractFileName(FileName, FilePath);	
	esMEMS_Bfree(pBuf, in_para.width*in_para.height*4);		
	return EPDK_OK;
}

/*
************************************************************************************************************************
*Function	:        static __s32 draw_listview_item_icon(__lbar_draw_para_t *draw_param)
*
*Description	:		��item iconͼ��(Item��ߵ�Icon,����������ͼIcon)
*					
*Arguments  	: 		
*
*Return     	: 	
*
************************************************************************************************************************
*/
static __s32 draw_listview_item_icon(__lbar_draw_para_t *draw_param,rat_media_type_t media_type)
{
	RECT IconRect;
	void *pic_buf = NULL;		
	//int ret;
	__s32 picX = 0, picY = 0;
	explr_list_para_t *this = NULL;	
	
		
	this = (explr_list_para_t *)draw_param->attr;		
	explorer_get_item_icon_rect(this, &IconRect);

	/*
	if(this->media_type == RAT_MEDIA_TYPE_ALL)		//�ļ�����
	{
		//media_type = get_file_list_item_file_type(this->cur_file_list->cur_item);
	}
	else
	{
		media_type = GetListItemFileMediaType(this->rat.handle, draw_param->index);
	}
	*/	
	if(draw_param->mode == LBAR_MODE_NORMAL)
	{
		pic_buf = explorer_get_file_item_icon(this->view_mode,media_type, UNFOCUSED);
	}
	else if(draw_param->mode == LBAR_MODE_FOCUS)
	{
		pic_buf = explorer_get_file_item_icon(this->view_mode,media_type, FOCUSED);
	}
	
	if(pic_buf != NULL)
	{
		picX = draw_param->rect.x + IconRect.x;
		picY = draw_param->rect.y + IconRect.y;
		GUI_BMP_Draw(pic_buf, picX, picY);		
	}
	return EPDK_OK;
}
/*
************************************************************************************************************************
*Function	:       static __s32 draw_square_item_focus_icon(__lbar_draw_para_t *draw_param)
*
*Description	:		������ͼ��ý���ʱ��ʾ��ͼ��
*					
*Arguments  	: 		
*
*Return     	: 	
*
************************************************************************************************************************
*/
static __s32 draw_square_item_focus_icon(__lbar_draw_para_t *draw_param)
{
	RECT FocusIconRect;
	//void *pic_buf;		
	int ret;
	//__s32 picX, picY;
	__s32 X0, Y0, X1, Y1;
	explr_list_para_t *list_para;	
	
	list_para = (explr_list_para_t *)draw_param->attr;	
	
	ret = explorer_get_item_focus_icon_rect(list_para, &FocusIconRect);
	
	/*
	pic_buf = explorer_get_listview_icon_res(ID_EXP_SQUARE_FOCUS);			//draw select border
	
	//picW =  GUI_BMP_GetXSize(pic_buf);										//���ͼƬ��
	//picH =  GUI_BMP_GetYSize(pic_buf);										//���ͼƬ�߶�

	picX = draw_param->rect.x + FocusIconRect.x;
	picY = draw_param->rect.y + FocusIconRect.y;
	GUI_BMP_Draw(pic_buf, picX, picY);				
	*/

	//�����㷽��
	GUI_SetColor(GUI_YELLOW);
	X0 = draw_param->rect.x + FocusIconRect.x;
	Y0 = draw_param->rect.y + FocusIconRect.y;
	X1 = X0 + FocusIconRect.width;
	Y1 = Y0 + FocusIconRect.height;
	GUI_DrawLine(X0, Y0, X1, Y0);
	GUI_DrawLine(X0, Y0+1, X1, Y0+1);
	GUI_DrawLine(X0, Y0+2, X1, Y0+2);		

	GUI_DrawLine(X0, Y0, X0, Y1);
	GUI_DrawLine(X0+1, Y0, X0+1, Y1);
	GUI_DrawLine(X0+2, Y0, X0+2, Y1);	

	GUI_DrawLine(X0, Y1, X1, Y1);
	GUI_DrawLine(X0, Y1-1, X1, Y1-1);	
	GUI_DrawLine(X0, Y1-2, X1, Y1-2);

	GUI_DrawLine(X1, Y0, X1, Y1);
	GUI_DrawLine(X1-1, Y0, X1-1, Y1);
	GUI_DrawLine(X1-2, Y0, X1-2, Y1);	
	
	return ret;
}


//Draw focus icon, �Ž��������������Ż�Ϊһ������
static __s32 draw_list_item_focus_icon(__lbar_draw_para_t *draw_param)
{
	RECT FocusIconRect;
	void *pic_buf;		
	int ret;
	__s32 picX, picY;
	explr_list_para_t *list_para;	
	
	list_para = (explr_list_para_t *)draw_param->attr;	
	
	ret = explorer_get_item_focus_icon_rect(list_para, &FocusIconRect);
	
	pic_buf = explorer_get_listview_icon_res(ID_EXP_LIST_ITEM_FOCUS_BG);			//draw select border
	//picW =  GUI_BMP_GetXSize(pic_buf);										//���ͼƬ��
	//picH =  GUI_BMP_GetYSize(pic_buf);										//���ͼƬ�߶�

	picX = draw_param->rect.x + FocusIconRect.x;
	picY = draw_param->rect.y + FocusIconRect.y;
	GUI_BMP_Draw(pic_buf, picX, picY);				
	return ret;
}
/*
************************************************************************************************************************
*Function	:       static __s32 draw_listview_item_text(__lbar_draw_para_t *draw_param)
*
*Description	:	 ��ʾ��Ŀ�ı�
*					
*Arguments  	: 		
*
*Return     	: 	
*
************************************************************************************************************************
*/
static __s32 draw_listview_item_text(__lbar_draw_para_t *draw_param)
{
	int ret;
	explr_list_para_t *list_para;		
	char FileName[RAT_MAX_FULL_PATH_LEN];
	char *name = NULL;
	GUI_RECT GUI_TextRect;
	RECT TextRect;
	file_item_t *file_item = NULL;
	
	list_para = (explr_list_para_t *)draw_param->attr;		

	if(list_para->media_type == RAT_MEDIA_TYPE_ALL)
	{
		//name = get_file_list_item_name(list_para->cur_file_list, draw_param->index);
		file_item = get_file_list_item(list_para->cur_file_list, draw_param->index);
		if(file_item != NULL)
		{
			list_para->file_item = file_item;
			if((file_item->fatdirattr == RAT_MEDIA_TYPE_USB_DEVICE)
				||(file_item->fatdirattr == RAT_MEDIA_TYPE_SD_DEVICE))
			{
				get_root_device_disp_name(FileName, file_item->fatdirattr);
			}
			else
			{
				eLIBs_strcpy(FileName, file_item->name);	
			}							
			
			if(draw_param->mode == LBAR_MODE_FOCUS)
			{
				list_para->cur_file_list->cur_item = file_item;		//���浱ǰ��Ŀ��Ϣ
			}
		}		
		else
		{
			__msg("error in file list\n");
			return EPDK_FAIL;
		}
	}
	else
	{
		eLIBs_memset(FileName,0,sizeof(FileName));
#if EXTERN_KARAOKE_EXIST //add by hot.lee
		if(RAT_MEDIA_TYPE_KARAOKE == list_para->media_type)
		{
			//__wrn("draw_listview_item_text2\n");
			get_krk_listitem_filename(list_para, draw_param->index, FileName);
		}
		else
#endif
		{
			ret = GetListItemFileName(list_para, draw_param->index, FileName);
		}
		if(ret == EPDK_FAIL)
		{
			__wrn("get file information form rat fail!!");
			return EPDK_FAIL;
		}
	}
	__wrn("draw_listview_item_text3\n");
	ret = explorer_get_item_text_rect(list_para,&TextRect);
	
	GUI_SetFont(SWFFont);
	//GUI_SetFont(GUI_GetDefaultFont());
#if EXTERN_KARAOKE_EXIST
	if(RAT_MEDIA_TYPE_KARAOKE ==list_para->media_type)
	{
		GUI_UC_SetEncodeGBK();
	}
	else
#endif		
	{
		GUI_UC_SetEncodeUTF8();
	}
	//GUI_SetBkColor(GUI_LIGHTGREEN);	
	GUI_SetColor(GUI_WHITE);
	GUI_SetDrawMode(GUI_DRAWMODE_NORMAL);
	
	GUI_TextRect.x0 = draw_param->rect.x  + TextRect.x;
	GUI_TextRect.y0 = draw_param->rect.y  + TextRect.y;
	GUI_TextRect.x1 = draw_param->rect.x  + (TextRect.x + TextRect.width);
	GUI_TextRect.y1 = draw_param->rect.y  + (TextRect.y + TextRect.height);
	__wrn("FileName =%s\n",FileName);
	GUI_DispStringInRect(FileName, &GUI_TextRect, GUI_TA_VCENTER);	
	__wrn("draw_listview_item_text4\n");
	if(draw_param->mode == LBAR_MODE_FOCUS)
	{
		explorer_list_long_string_start_roll_fast(draw_param, FileName);	
	}
	__wrn("draw_listview_item_text5\n");
	return EPDK_OK;
}

__s32 explorer_draw_file_info(explr_list_para_t *list_para)
{
	rat_media_type_t media_type;
	char filename[RAT_MAX_FULL_PATH_LEN];
	file_item_t *cur_item = NULL;
	if(list_para == NULL)
	{
		return EPDK_FAIL;
	}
	
	cur_item = list_para->cur_file_list->cur_item;
	if(cur_item == NULL)
	{
		return EPDK_FAIL;
	}
	__wrn("explorer_draw_file_info1\n");
#if EXTERN_KARAOKE_EXIST //add by hot.ee
	if(ID_EXPLORER_KARAOKE == list_para->root_para->data)
	{	
		__wrn("explorer_draw_file_info2\n");
		media_type = RAT_MEDIA_TYPE_KARAOKE;
	}
	else
#endif
	{
		media_type = get_file_list_item_file_type(cur_item);
	}
	__wrn("explorer_draw_file_info3\n");
	explorer_draw_FileTypeIcon(list_para, media_type);

	if((media_type & FSYS_ATTR_DIRECTORY)
		||(media_type == RAT_MEDIA_TYPE_SD_DEVICE)
		||(media_type == RAT_MEDIA_TYPE_USB_DEVICE))
	{
		explorer_clear_filesize_area();
		explorer_clear_file_create_time_area();					
	}
	else		//�ļ�����ʱ��ʾ�ļ���Ϣ
	{
		explorer_draw_FileSize(cur_item);							
		eLIBs_strcpy(filename, list_para->cur_file_list->file_path);
		eLIBs_strcat(filename, "\\");
		eLIBs_strcat(filename, cur_item->name);
		explorer_draw_file_create_time(filename);
	}
	return EPDK_OK;
}
//draw item in list mode, ��Ҫ��Listbar�Ļص�������ѡ��ͼ��
static __s32 draw_listview_item(__lbar_draw_para_t *draw_param)
{
	//int ret = 0;
	explr_list_para_t *list_para;	
	rat_media_type_t media_type;
		
	__msg("listbar item id =: %d\n", draw_param->index);		//focus id							
	list_para = (explr_list_para_t *)draw_param->attr;				//for draw the picture  in different media type	
	
	//GUI_ClearRect(draw_param->rect.x,draw_param->rect.y, draw_param->rect.x + draw_param->rect.width, draw_param->rect.y + draw_param->rect.height);

	if(draw_param->mode == LBAR_MODE_FOCUS)
	{
		if(list_para->view_mode == EXPLR_LIST_MODE)
		{
			draw_list_item_focus_icon(draw_param);		//��focus item ������ͼ
		}
		else if(list_para->view_mode == EXPLR_SQUARE_MODE)
		{
			draw_square_item_focus_icon(draw_param);
		}
		
				
	}	
	
	//if((list_para->view_mode != EXPLR_LIST_MODE)
	//	&&(list_para->media_type != ))
	{
		//draw_listview_item_icon(draw_param);	
	}
	
	//if(list_para->media_type != RAT_MEDIA_TYPE_PIC)	//picture mode ����Ҫ��ʾ����
	//if(list_para->view_mode == EXPLR_LIST_MODE)
	if((list_para->view_mode == EXPLR_SQUARE_MODE) 
			&& (list_para->media_type == RAT_MEDIA_TYPE_PIC))
	{
		explorer_list_long_string_stop_roll(list_para);		//ֹͣ���ļ�������
	}
	else
	{
	
		draw_listview_item_text(draw_param);	
		if(list_para->media_type != RAT_MEDIA_TYPE_ALL)
		{
#if EXTERN_KARAOKE_EXIST
			if(RAT_MEDIA_TYPE_KARAOKE == list_para->media_type)
			{
				media_type = RAT_MEDIA_TYPE_KARAOKE;
			}
			else
#endif				
			{
		     	media_type = GetListItemFileMediaType(list_para->rat.handle, draw_param->index);
			}
		}
		else
		{
#if EXTERN_KARAOKE_EXIST //add by hot.ee
			if(ID_EXPLORER_KARAOKE == list_para->root_para->data)
			{	
				//__wrn("draw_listview_item2\n");
				media_type = RAT_MEDIA_TYPE_KARAOKE;
			}
			else
#endif
			{
				media_type = get_file_list_item_file_type(list_para->file_item);
			}
		}
		{
			draw_listview_item_icon(draw_param, media_type);	
		}
				
		//explorer_draw_FileTypeIcon(list_para);		
	}
	
	if(draw_param->mode == LBAR_MODE_FOCUS)
	{
		if(list_para->media_type == RAT_MEDIA_TYPE_ALL)		//�ļ�����
		{
			explorer_draw_file_info(list_para);
			/*
			{
				char filename[RAT_MAX_FULL_PATH_LEN];
				file_item_t *cur_item = list_para->cur_file_list->cur_item;
				if(cur_item == NULL)
				{
					return EPDK_FAIL;
				}
				media_type = get_file_list_item_file_type(cur_item);
				explorer_draw_FileTypeIcon(list_para, media_type);

				if((media_type & FSYS_ATTR_DIRECTORY)
					||(media_type == RAT_MEDIA_TYPE_SD_DEVICE)
					||(media_type == RAT_MEDIA_TYPE_USB_DEVICE))
				{
					explorer_clear_filesize_area();
					explorer_clear_file_create_time_area();					
				}
				else		//�ļ�����ʱ��ʾ�ļ���Ϣ
				{
					explorer_draw_FileSize(cur_item);							
					eLIBs_strcpy(filename, list_para->cur_file_list->file_path);
					eLIBs_strcat(filename, "\\");
					eLIBs_strcat(filename, cur_item->name);
					explorer_draw_file_create_time(filename);
				}
			}
			*/
		}	
	}	
	
	return EPDK_OK;	
}

/*
************************************************************************************************************************
*Function	:       static __s32 _explorer_listbar_item_draw(__lbar_draw_para_t *draw_param)
*
*Description	:	 ListBar������Ϣ������
*					
*Arguments  	: 		
*
*Return     	: 	
*
************************************************************************************************************************
*/
static __s32 _explorer_listbar_item_draw(__lbar_draw_para_t *draw_param)
{

	explr_list_para_t *list_para;
	list_para = (explr_list_para_t *)draw_param->attr;		//for draw the picture  in different media type
	//__wait__;
	//__msg("###########  item index X =: %d\n", draw_param->index);
	//__msg("###########  item X =: %d\n", draw_param->rect.x);
	//__msg("###########  item Y =: %d\n", draw_param->rect.y);
	//__msg("###########  item width =: %d\n", draw_param->rect.width);
	//__msg("###########  item Height =: %d\n", draw_param->rect.height);
	if(list_para->rat.total <= 0)
	{
		return EPDK_FAIL;
	}
	
	switch(draw_param->mode)	
	{
		case LBAR_MODE_NORMAL:
			__msg("\n~~~~draw listbar in normal mode!~~~~");	
			draw_listview_item(draw_param);
			break;
		case LBAR_MODE_FOCUS:
			__msg("\n~~~~draw focus item in listbar!~~~~");
			//GUI_LyrWinCacheOn();
			draw_listview_item(draw_param);
			draw_miniature_view_item(draw_param);			
			//GUI_LyrWinCacheOff();
			break;
		case LBAR_MODE_INVALID:
			__msg("\n~~~~draw invalid item in listbar!~~~~");		
			break;
		case LBAR_MODE_VACANT:
			__msg("\n~~~~draw vacant item in listbar!~~~~");		
			//draw_list_vacant_item(draw_param);
			break;
		case LBAR_MODE_DIFFUSE:
			__msg("listbar item id =: %d\n", draw_param->index);		//focus id
			//draw_listview_item(draw_param);			
			draw_miniature_view_item(draw_param);
			__msg("\n~~~~draw diffuse item in listbar!~~~~");		
			break;		
	}	
	return EPDK_OK;
}

/*
************************************************************************************************************************
*Function	:       void explorer_listbar_init(H_WIN  list_win)
*
*Description	:	 ListBar ��ʼ������,ע������֮ǰRat�����Ѿ�����ʼ��
*					
*Arguments  	: 		
*
*Return     	: 	
*
************************************************************************************************************************
*/
static void explorer_listbar_init(H_WIN  list_win)
{
	explr_list_para_t *list_para;
    __listbar_config_t config;    						
    __scroll_bar_t scroll_param;						//new scroll bar
    __s32 item_width = 0, item_height = 0;
    
    explorer_viewer_ui_t *ui_param = explorer_get_viewer_ui_param();			//�������AP��UI��Ʋ���
    
    __msg("\n~~~~~~~~~~Listbar is initializing~~~~~~~~~~~\n");
	
    eLIBs_memset(&config, 0, sizeof(__listbar_config_t));
    eLIBs_memset(&scroll_param, 0, sizeof(__scroll_bar_t));

	list_para = (explr_list_para_t *)GUI_WinGetAttr(list_win);
		
	if(list_para->view_mode == EXPLR_LIST_MODE)
	{
    	item_width = ui_param->list_item_ui_param.item_rect.width;						//����list or squareģʽ��ͬ
	    item_height = ui_param->list_item_ui_param.item_rect.height;			
	    config.list_rect.x = ui_param->list_item_ui_param.ListBarArea.x;			//ui_param->menu == listbar area
    	config.list_rect.y = ui_param->list_item_ui_param.ListBarArea.y;
    	config.list_rect.width = ui_param->list_item_ui_param.ListBarArea.width;
    	config.list_rect.height = ui_param->list_item_ui_param.ListBarArea.height;			
	}
	else if(list_para->view_mode == EXPLR_SQUARE_MODE)
	{
        item_width = ui_param->square_item_ui_param.item_rect.width;
        item_height = ui_param->square_item_ui_param.item_rect.height;						
	    config.list_rect.x = ui_param->square_item_ui_param.ListBarArea.x;			//ui_param->menu == listbar area
    	config.list_rect.y = ui_param->square_item_ui_param.ListBarArea.y;
    	config.list_rect.width = ui_param->square_item_ui_param.ListBarArea.width;
    	config.list_rect.height = ui_param->square_item_ui_param.ListBarArea.height;			
	}
		
    config.item_width = item_width;	
    config.item_height = item_height;				//these are numbers only for test
    
    //__msg("----view_param->playing_media_type: %d\n", view_param->playing_media_type);
    config.start_id = list_para->last_start_id;
    //config.focus_id = list_para->rat.index; 				//removed by libaiao, 2010.6.6    
    config.focus_id = list_para->last_focused_id;
    config.bk_color = 0;
    config.alpha_status = 1;						//��Alpha����
    config.list_attr = (void *)list_para;
	
    //list_para->rat.index = 0;		//��ʼ����ʼ��Ŀ		
    //view_param->total_num = cat_get_explr_num(view_param->cat_handle, view_param->cat_explr);
	config.item_cnt = list_para->rat.total;   //Rat.total�����Ѿ���� 
	
    scroll_param.show_rect.x = ui_param->scroll_bg_rect.x;
    scroll_param.show_rect.y = ui_param->scroll_bg_rect.y;
    scroll_param.show_rect.width = ui_param->scroll_bg_rect.width;
    scroll_param.show_rect.height = ui_param->scroll_bg_rect.height;		//����Scoll bar��ʾ����Head, body,tail
	
    scroll_param.scroll_rect.x = ui_param->scroll_rect.x;
    scroll_param.scroll_rect.y = ui_param->scroll_rect.y;
    scroll_param.scroll_rect.width = ui_param->scroll_rect.width;
    scroll_param.scroll_rect.height = ui_param->scroll_rect.height;			//����������(��ȥhead,body,tail)
    
    scroll_param.head_height = ui_param->scroll_head_height;    
    scroll_param.tail_height = ui_param->scroll_tail_height;				//head, tail
	
    scroll_param.back_bmp = explorer_get_listview_icon_res(ID_EXP_LIST_SCROLL_BG);
    scroll_param.head_bmp = explorer_get_listview_icon_res(ID_EXP_LIST_SCROLL_HEAD);
    scroll_param.body_bmp = explorer_get_listview_icon_res(ID_EXP_LIST_SCROLL_BODY);
    scroll_param.tail_bmp = explorer_get_listview_icon_res(ID_EXP_LIST_SCROLL_TAIL);    
	
    __msg("----back_bmp: %x, head_bmp: %x, body_bmp: %x, tail_bmp: %x\n", scroll_param.back_bmp, scroll_param.head_bmp, scroll_param.body_bmp, scroll_param.tail_bmp);
        
    //TestTimerReport(__FILE__, __LINE__);
    //rat_start_miniature_decode();					

    list_para->listbar_handle = LISTBAR_Create(list_win);    				//new listbar    
    LISTBAR_Config(list_para->listbar_handle, _explorer_listbar_item_draw, &config);    //config parametters for listbar
    LISTBAR_ScrollConfig(list_para->listbar_handle, &scroll_param); 	//scrollbar belongs to listbar
    //LISTBAR_ShowPage(list_para->listbar_handle);              	
    
}

/*
************************************************************************************************************************
*Function	:       void explorer_listbar_uninit(H_WIN  list_win)
*
*Description	:	 �ͷ�Listbar
*					
*Arguments  	: 		
*
*Return     	: 	
*
************************************************************************************************************************
*/
static void explorer_listbar_uninit(H_WIN  list_win)
{
	explr_list_para_t *list_para;
	list_para = (explr_list_para_t *)GUI_WinGetAttr(list_win);
	if(list_para->listbar_handle != NULL)
	{
    	__msg("----LISTBAR_Delete begin: %x\n", list_para->listbar_handle);
    	LISTBAR_SuspendPage(list_para->listbar_handle);    
    	LISTBAR_Delete(list_para->listbar_handle);	
		list_para->listbar_handle = NULL;
	}
	return;
}

/*
************************************************************************************************************************
*Function	:       void ExplorerSetViewMode(explr_list_para_t *list_para, explorer_view_mode_e view_mode)
*
*Description	:	 �ı�Explorer��ʾ��ʽList mode or square mode
*					
*Arguments  	: 		
*
*Return     	: 	
*
************************************************************************************************************************
*/
void ExplorerSetViewMode(explr_list_para_t *list_para, explorer_view_mode_e view_mode)
{
	
	if(view_mode == EXPLR_LIST_MODE)
	{
		list_para->view_mode = EXPLR_LIST_MODE;		
	}
	else if(view_mode == EXPLR_SQUARE_MODE)
	{
		list_para->view_mode = EXPLR_SQUARE_MODE;        
	}

	explorer_save_last_para( list_para );
	explorer_get_last_para( list_para) ;

	explorer_listbar_uninit(list_para->list_win);		//���´���listbar,��ΪSquareΪȫ��ģʽ
	GUI_LyrWinSel(list_para->list_layer);
	GUI_SetBkColor(0);
	GUI_Clear();
	explorer_listbar_init(list_para->list_win);
	//rat_move_cursor_to_first(list_para->rat.handle);
	LISTBAR_ShowPage(list_para->listbar_handle);
	//LISTBAR_ChangeMode(	list_para->listbar_handle,item_width,item_height);	
	//LISTBAR_UpdatePage(list_para->listbar_handle);
	if(list_para->view_mode == EXPLR_LIST_MODE)
	{
		explorer_draw_FileTypeIcon(list_para, list_para->media_type);	
	}
	return;
}

//�����ļ��б����ϵ�
static __s32 explorer_file_list_init(explr_list_para_t *list_para)
{
	__u32 total = 0;
	if(list_para == NULL)	
	{
		return EPDK_FAIL;
	}
	
	if(list_para->top_file_list == NULL)
	{
		#if 1
		list_para->top_file_list = new_file_root_list_nod(NULL);
		if(list_para->top_file_list == NULL)
		{
			return EPDK_FAIL;
		}
		list_para->cur_file_list = list_para->top_file_list;				
		list_para->rat.total = list_para->cur_file_list->total;		//only for test, for listbar initial				
		#else
		list_para->top_file_list = new_file_list_nod(list_para->search_path, NULL);
		if(list_para->top_file_list == NULL)
		{		
			return EPDK_FAIL;
		}		
		list_para->cur_file_list = list_para->top_file_list;				
		list_para->rat.total = list_para->cur_file_list->total;		//only for test, for listbar initial		
		#endif
	}
#if EXTERN_KARAOKE_EXIST
	if(RAT_MEDIA_TYPE_KARAOKE!=list_para->media_type)
	{
	 //rat_start_miniature_decode();	//��ʼ����ͼ�߳�	
	}
#endif	
	return EPDK_OK;
}


//ɾ�������ļ��б�
static __s32 explorer_file_list_uninit(explr_list_para_t *list_para)
{
	if(list_para->top_file_list != NULL)
	{
        __here__;
		delete_file_list_chain(list_para->top_file_list);
        __here__;
		list_para->cur_file_list = NULL;		
	}
    __here__;
	//rat_stop_miniature_decode();	//ֹͣ ����ͼ�߳�	
	__here__;
	return EPDK_FAIL;
}

/*
************************************************************************************************************************
*Function	:       static __s32 explorer_rat_init(explr_list_para_t *list_para)
*
*Description	:	 �����ļ�Module, Rat ��ʼ��
*					
*Arguments  	: 		
*
*Return     	: 	
*
************************************************************************************************************************
*/
#if EXTERN_KARAOKE_EXIST  //add by hot.lee
static __s32 explorer_karaoke_init(explr_list_para_t *list_para)
{
	
	eLIBs_sprintf(list_para->kak_path,"%c%s",list_para->search_path[0],KARAOKE_KAK_DATA_PATH);
	//eLIBs_strcpy(list_para->kak_path, "f:\\rootfs\\kak_s0.kak");
	
		__wrn("2222list_para->search_path=%s\n", list_para->kak_path);
		__here__;
	list_para->hkak = kak_open(list_para->kak_path);

	if(list_para->rat.handle == NULL)
	{
	    
		__wrn("list_para->search_path=%s\n", list_para->search_path);
		 
		list_para->rat.handle =(HRAT)list_para->hkak;/////////////rat_open(list_para->search_path, list_para->media_type, 0);		
        __wrn("list_para->rat.handle= %08x\n",list_para->rat.handle);
		if(list_para->rat.handle == NULL)
		{
			__wrn("open rat failed\n");
		}
		else
		{
			__msg("open rat successed\n");
		}
	}
	else
	{
		__wrn("!!!!!!!!!!!!!please dont open rat again!!!!!!!!!!!!!!!!!\n");
	}
	
	if(list_para->rat.handle == NULL)
	{
		__wrn("!!!!!!!!!!!!!open rat fail!!!!!!!!!!!!!!!!!\n");
		return EPDK_FAIL;
	}

		
	list_para->rat.index = 0;
	//rat_move_cursor_to_first(list_para->rat.handle);
	__wrn("list_para->rat.total\n");
	list_para->rat.total = kak_get_total_song_count(list_para->hkak);////////////////rat_get_cur_scan_cnt(list_para->rat.handle);;
	/*list_para->ppstr = esMEMS_Balloc(sizeof(char*)*(list_para->rat.total));
	eLIBs_memset(list_para->ppstr,0,sizeof(char*)*(list_para->rat.total));
	if(!list_para->ppstr)
	{
		__wrn("balloc list_para->ppstr wrong\n");
	}*/
	//list_para->rat.total = rat_get_total_number(list_para->rat.handle);	
	__wrn("list_para->rat.total = %d\n",list_para->rat.total);

//..rat_start_miniature_decode();	//��ʼ����ͼ�߳�	
//__wrn("list_para->rat.total ok\n");

return EPDK_OK;

}
#endif

static __s32 explorer_rat_init(explr_list_para_t *list_para)
{
	/*	
	{		
		file_item_t *item_list = NULL;
		__u32 total = 0;
		//__wait__;
		total = new_file_list(list_para->search_path,&item_list, NULL);
		delete_file_list(item_list,total);
	}
	*/
		if(list_para->rat.handle == NULL)
		{
			list_para->rat.handle = rat_open(list_para->search_path, list_para->media_type, 0);		
			if(list_para->rat.handle == NULL)
			{
				__wrn("open rat failed\n");
			}
			else
			{
				__msg("open rat successed\n");
			}
		}
		else
		{
			__wrn("!!!!!!!!!!!!!please dont open rat again!!!!!!!!!!!!!!!!!\n");
		}
		
		if(list_para->rat.handle == NULL)
		{
			__wrn("!!!!!!!!!!!!!open rat fail!!!!!!!!!!!!!!!!!\n");
			return EPDK_FAIL;
		}

			
		list_para->rat.index = 0;
		//rat_move_cursor_to_first(list_para->rat.handle);
		list_para->rat.total = rat_get_cur_scan_cnt(list_para->rat.handle);;
		//list_para->rat.total = rat_get_total_number(list_para->rat.handle);	
	
	//rat_start_miniature_decode();	//��ʼ����ͼ�߳�	
	return EPDK_OK;
}
/*
************************************************************************************************************************
*Function	:      static __s32 explorer_rat_uninit(explr_list_para_t *list_para)
*
*Description	:	 �ͷ�rat
*					
*Arguments  	: 		
*
*Return     	: 	
*
************************************************************************************************************************
*/
#if EXTERN_KARAOKE_EXIST  //add by hot.lee

static __s32 explorer_karaoke_uninit(explr_list_para_t *list_para)
{
	if(list_para->rat.handle != NULL)
	{
		//rat_close(list_para->rat.handle); kak_close
		kak_close(list_para->hkak);
		list_para->hkak = NULL;
		list_para->rat.handle = NULL;
	}
	//rat_stop_miniature_decode();	
	return EPDK_OK;
}
#endif


static __s32 explorer_rat_uninit(explr_list_para_t *list_para)
{
	if(list_para->rat.handle != NULL)
	{
		//rat_close(list_para->rat.handle);
	}
	//rat_stop_miniature_decode();	
	return EPDK_OK;
}


static __s32 explorer_get_last_para(explr_list_para_t *list_para)		
{
	__u32 page_item_cnt = 0; 				//ÿҳ����Ŀ����
	char FileName[RAT_MAX_FULL_PATH_LEN];
	int ret = 0;
	reg_root_para_t *root_para = NULL;	

	//page_item_cnt = LISTBAR_GetPageItemCount(list_para->listbar_handle);
	page_item_cnt = explorer_get_listbar_pagenum(list_para);	
	root_para = dsk_reg_get_para_by_app(REG_APP_ROOT);
	
	if(list_para->rat.handle != NULL)
	{	
		switch(list_para->media_type)
		{
			case RAT_MEDIA_TYPE_ALL:
				list_para->last_start_id = 0;
				break;			
			case RAT_MEDIA_TYPE_PIC:
				//list_para->last_start_id = photo_start_id;
				if(list_para->root_type == RAT_TF)
				{
					list_para->last_start_id = root_para->last_photo_start_index_sd;
					list_para->last_focused_id = root_para->last_photo_index_sd;
					list_para->last_filename = root_para->last_photo_path_sd;
				}
				else if(list_para->root_type == RAT_USB)
				{
					list_para->last_start_id = root_para->last_photo_start_index_ud;
					list_para->last_focused_id = root_para->last_photo_index_ud;
					list_para->last_filename = root_para->last_photo_path_ud;
				}						
				break;
			case RAT_MEDIA_TYPE_AUDIO:						
				if(list_para->root_type == RAT_TF)
				{
					list_para->last_start_id = root_para->last_music_start_index_sd;
					list_para->last_focused_id = root_para->last_music_index_sd;
					list_para->last_filename = root_para->last_music_path_sd;
				}
				else if(list_para->root_type == RAT_USB)
				{
					list_para->last_start_id = root_para->last_music_start_index_ud;
					list_para->last_focused_id = root_para->last_music_index_ud;
					list_para->last_filename = root_para->last_music_path_ud;							
				}						
				break;
			case RAT_MEDIA_TYPE_VIDEO:
				if(list_para->root_type == RAT_TF)
				{
					list_para->last_start_id = root_para->last_movie_start_index_sd;
					list_para->last_focused_id = root_para->last_movie_index_sd;
					list_para->last_filename = root_para->last_movie_path_sd;
				}
				else if(list_para->root_type == RAT_USB)
				{
					list_para->last_start_id = root_para->last_movie_start_index_ud;
					list_para->last_focused_id = root_para->last_movie_index_ud;
					list_para->last_filename = root_para->last_movie_path_ud;
				}						
				break;
			case RAT_MEDIA_TYPE_EBOOK:
				if(list_para->root_type == RAT_TF)
				{
					list_para->last_start_id = root_para->last_ebook_start_index_sd;
					list_para->last_focused_id = root_para->last_ebook_index_sd;
					list_para->last_filename = root_para->last_ebook_path_sd;
				}
				else if(list_para->root_type == RAT_USB)
				{
					list_para->last_start_id = root_para->last_ebook_start_index_ud;
					list_para->last_focused_id = root_para->last_ebook_index_ud;
					list_para->last_filename = root_para->last_ebook_path_ud;
				}						
				break;	
#if EXTERN_KARAOKE_EXIST // add by hot.lee
			case RAT_MEDIA_TYPE_KARAOKE:	
				__wrn("list_para->root_type == %d\n",list_para->root_type);
				if(list_para->root_type == RAT_TF)
				{
					list_para->last_start_id = root_para->last_krk_start_index_sd;
					list_para->last_focused_id = root_para->last_krk_index_sd;
					list_para->last_filename = root_para->last_krk_path_sd;
				}
				else if(list_para->root_type == RAT_USB)
				{
					list_para->last_start_id = root_para->last_krk_start_index_ud;
					list_para->last_focused_id = root_para->last_krk_index_ud;
					list_para->last_filename = root_para->last_krk_path_ud;							
				}						
				break;

#endif
				
			default:
			break;
		}				
		
		if((list_para->last_focused_id < list_para->rat.total)
			&&(list_para->last_start_id < list_para->rat.total))
		{
			eLIBs_memset(FileName,0,sizeof(FileName));	
#if EXTERN_KARAOKE_EXIST
			if(RAT_MEDIA_TYPE_KARAOKE == list_para->media_type)
			{
				GetKaraokeListItemFileFullPath(list_para, list_para->last_focused_id, FileName);
			}
			else
#endif
			{
		  		ret = GetListItemFileFullPath(list_para, list_para->last_focused_id, FileName);	
			}
			//..GetListItemFileFullPath(list_para, list_para->last_focused_id, FileName);			
			ret = eLIBs_strcmp(FileName,list_para->last_filename);
			if(ret == 0)				
			{	
				if(list_para->last_focused_id < list_para->last_start_id)
				{
					list_para->last_start_id = list_para->last_focused_id;
					list_para->rat.index = list_para->last_focused_id;			//��λ����һ����ʼ��ʾ
				}
				else if((list_para->last_focused_id - list_para->last_start_id) >= page_item_cnt)
				{
					list_para->last_start_id = list_para->last_focused_id;		//
					list_para->rat.index = list_para->last_focused_id;			//��λ����һ����ʼ��ʾ
				}
				else
				{
					list_para->rat.index = list_para->last_focused_id;//�����λ����һ��������ļ�
				}				
				return EPDK_OK;	
			}
		}
		
	}
	list_para->last_start_id = 0;
	list_para->last_focused_id = 0;
	list_para->rat.index = 0;
	return EPDK_OK;
}


static __s32 explorer_save_last_para(explr_list_para_t *list_para)		
{
	__s32 ret = 0;
	reg_app_e AppId = 0;	
	char FileName[RAT_MAX_FULL_PATH_LEN];
	__listbar_scene_t ListBarScene = {0,0};		
	reg_root_para_t *root_para = NULL;
	
	root_para = dsk_reg_get_para_by_app(REG_APP_ROOT);

	//__wait__;
	LISTBAR_GetScene(list_para->listbar_handle, &ListBarScene);	
	eLIBs_memset(FileName,0,sizeof(FileName));	
	
	switch(list_para->media_type)
	{
		case RAT_MEDIA_TYPE_VIDEO:
			AppId = REG_APP_MOVIE;	
			//movie_start_id = ListBarScene.start_id;
			if(list_para->root_type == RAT_TF)
			{
				root_para->last_movie_start_index_sd = ListBarScene.start_id;
			}			
			else if(list_para->root_type == RAT_USB)
			{
				root_para->last_movie_start_index_ud = ListBarScene.start_id;
			}
			break;
		case RAT_MEDIA_TYPE_PIC:
			AppId = REG_APP_PHOTO;
			if(list_para->root_type == RAT_TF)
			{
				root_para->last_photo_start_index_sd = ListBarScene.start_id;
			}			
			else if(list_para->root_type == RAT_USB)
			{
				root_para->last_photo_start_index_ud = ListBarScene.start_id;
			}
			break;
		case RAT_MEDIA_TYPE_AUDIO:
			AppId = REG_APP_MUSIC;	
			if(list_para->root_type == RAT_TF)
			{
				root_para->last_music_start_index_sd = ListBarScene.start_id;
			}			
			else if(list_para->root_type == RAT_USB)
			{
				root_para->last_music_start_index_ud = ListBarScene.start_id;
			}
			break;
		case RAT_MEDIA_TYPE_EBOOK:
			AppId = REG_APP_EBOOK;
			if(list_para->root_type == RAT_TF)
			{
				root_para->last_ebook_start_index_sd = ListBarScene.start_id;
			}			
			else if(list_para->root_type == RAT_USB)
			{
				root_para->last_ebook_start_index_ud = ListBarScene.start_id;
			}
			break;
			
#if EXTERN_KARAOKE_EXIST // add by hot.lee
		case RAT_MEDIA_TYPE_KARAOKE:
			AppId = REG_APP_KARAOKE;	
			if(list_para->root_type == RAT_TF)
			{
				root_para->last_krk_start_index_sd = ListBarScene.start_id;
			}			
			else if(list_para->root_type == RAT_USB)
			{
				root_para->last_krk_start_index_ud = ListBarScene.start_id;
			}
			break;
#endif
		default:			
			break;
	}	
#if EXTERN_KARAOKE_EXIST	
	if(RAT_MEDIA_TYPE_KARAOKE == list_para->media_type)
	{
		GetKaraokeListItemFileFullPath(list_para, ListBarScene.focus_id, FileName);
	}
	else
#endif		
	{
  		ret = GetListItemFileFullPath(list_para, ListBarScene.focus_id, FileName);	
	}
	//..ret = GetListItemFileFullPath(list_para, ListBarScene.focus_id, FileName);	
	if(ret == EPDK_OK)
	{	
		dsk_reg_save_cur_play_info(AppId, ListBarScene.focus_id, FileName, list_para->root_type);				
		//dsk_reg_save_cur_play_info_ext(AppId,ListBarScene.start_id,list_para->rat.index, FileName, list_para->root_type);				
	}	
	return EPDK_OK;	
}
static __s32 explorer_listview_create(H_WIN  list_win)
{	
	explr_list_para_t *list_para;		
	
	list_para = (explr_list_para_t *)GUI_WinGetAttr(list_win);
	list_para->list_win = list_win;		//���洰�ھ��

	list_para->font = SWFFont;
	__msg("explorer is initializing!\n");
	//__wait__;	
	
	if(list_para->media_type == RAT_MEDIA_TYPE_ALL)
	{
		explorer_file_list_init(list_para);
		GUI_SetTimer(list_win, ExplorerTimerId, C_EXP_TIMER_INTERVAL, NULL);		
	}
	else
	{
#if EXTERN_KARAOKE_EXIST //add by hot.lee
		if(RAT_MEDIA_TYPE_KARAOKE == list_para->media_type)
		{
			explorer_karaoke_init(list_para);
			__wrn("explorer_listbar_init\n");
			explorer_get_last_para(list_para);
		}
		else
#endif
		{
			explorer_rat_init(list_para);
			explorer_get_last_para(list_para);
		}
	}	
	__wrn("explorer_listbar_init\n");
	explorer_listbar_init(list_win);	
	__wrn("explorer_list_long_string_init\n");
	explorer_list_long_string_init(list_para);
	//GUI_WinSetFocusChild(list_win);		//frame win get focus.
	return 0;
}

//only for file manager delete file dialog timer
void explorer_list_on_timer(__gui_msg_t *msg)
{
	explr_list_para_t *list_para;	
	list_para = (explr_list_para_t *)GUI_WinGetAttr(msg->h_deswin);
	if(list_para == NULL)
	{
		return;
	}
	/*
	if(GUI_IsTimerInstalled(list_para->list_win, ExplorerTimerId))
	{
		GUI_KillTimer(list_para->list_win, ExplorerTimerId);
		if(list_para->h_dialog != NULL)
		{
			return;
		}
		explorer_draw_file_info(list_para);
	}
	*/
	__msg("explorer list on timer\n");
	if(list_para->del_dlg_open)
	{
		list_para->del_dlg_open = EPDK_FALSE;
		__msg("2explorer list on timer-draw file info\n");
		explorer_draw_file_info(list_para);
	}
	//LISTBAR_ShowPage(list_para->listbar_handle);		
	return;
}

/*
************************************************************************************************************************
*Function	:          static __s32 _explorer_list_win_cb(__gui_msg_t *msg)
*
*Description	: 		ListWindow contrl block
*					
*Arguments  	: 		
*					
*Return     	: 	
*
************************************************************************************************************************
*/		
static __s32 _explorer_list_win_cb(__gui_msg_t *msg)
{    
    __s32 ret = EPDK_OK;    
    __gui_msg_t send;                             
    
    eLIBs_memset(&send, 0, sizeof(__gui_msg_t));
    
    switch(msg->id)
    {
        case GUI_MSG_CREATE:                             
            __msg("----explorer frame window GUI_MSG_CREATE begin----\n");
			explorer_listview_create(msg->h_deswin);                
            return EPDK_OK;
		case GUI_MSG_PAINT:
			__msg("----explorer frame window GUI_MSG_ONPAINT----\n");
			explorer_listview_onpaint(msg->h_deswin);			
			return EPDK_OK;
        case GUI_MSG_CLOSE:
			__msg("----explorer frame window GUI_MSG_CLOSE begin----\n"); 
			explorer_list_win_on_close(msg);			
            return EPDK_OK;          
        case GUI_MSG_DESTROY:	
            __msg("----explorer frame window GUI_MSG_DESTROY begin----\n"); 			
			explorer_list_win_on_destroy(msg);			
            __msg("----explorer frame window GUI_MSG_DESTROY end----\n");
            return EPDK_OK;
        case GUI_MSG_COMMAND:
            __here__;
			explorer_list_win_on_command(msg);
			return EPDK_OK;
		case GUI_MSG_KEY:
			explorer_list_win_key_proc(msg);
			return EPDK_OK;		
		case GUI_MSG_TIMER:
			explorer_list_on_timer(msg);
			break;
        default:
            ret = GUI_FrmWinDefaultProc(msg);
            break;
    }    
    return ret;
}


//�ͷ�Listwindow
__s32 explorer_list_win_delete(H_WIN list_win)
{
	GUI_FrmWinDelete(list_win);
	return EPDK_OK;
}


/*
************************************************************************************************************************
*Function	:          void  explorer_cmd2parent(H_WIN hwin, __s32 id, __s32 data1, __s32 data2)
*
*Description	: 		�򸸴��ڴ���������Ϣ
*					
*Arguments  	: 		hwin, �����ھ��
*					id,     �������
*Return     	: 	
*
************************************************************************************************************************
*/
void  explorer_cmd2parent(H_WIN hwin, __s32 id, __s32 data1, __s32 data2)
{
	__gui_msg_t msg;
	char buf[128];
	H_WIN hExpWin;
	hExpWin = GUI_WinGetParent(hwin);
	msg.h_deswin = GUI_WinGetParent(hExpWin);
	//msg.h_srcwin = NULL;
	msg.h_srcwin = hwin;	
    __here__;
	msg.id = GUI_MSG_COMMAND;
	msg.dwAddData1 = MAKELONG(GUI_WinGetItemId(hExpWin), id);//
	msg.dwAddData2 = data1;
	msg.dwReserved = data2;
	
	GUI_WinGetName(msg.h_deswin, buf);
	
	__msg("%s\n", buf);	
	GUI_SendNotifyMessage(&msg);
}

/*
************************************************************************************************************************
*Function	:          void ExplorerSendMessage(H_WIN hwin, __s32 id, __s32 data1, __s32 data2)
*
*Description	: 		SendMessage,����ͬ����Ϣ������Ϣ�ص��������ᱻ����ִ��
*					
*Arguments  	: 		�첽��Ϣ&NotifyMessage ���ᱻ���뵽��Ϣ�����У����ᱻ����ִ��
*				
*Return     	: 	
*
************************************************************************************************************************
*/
void ExplorerSendMessage(H_WIN hwin, __s32 id, __s32 data1, __s32 data2)
{
	__gui_msg_t msg;
	char buf[128];
	GUI_WinGetName(hwin, buf);	
	__msg("%s\n", buf);	
	
	msg.h_srcwin = NULL;	
	msg.h_deswin = hwin;
	msg.id = id;
	msg.dwAddData1 = data1;//
	msg.dwAddData2 = data2;
	GUI_SendMessage(&msg);
	return; 
}

/*
************************************************************************************************************************
*Function	:          void  ExplorerListWinGetSearchPath(explr_list_para_t *para)
*
*Description	: 		����RootType ���Search path
*					
*Arguments  	: 	
*				
*Return     	: 	
*
************************************************************************************************************************
*/
void  ExplorerListWinGetSearchPath(explr_list_para_t *para)
{
	__s32 ret = 0;
	char root_type[50];
	char disk_name[RAT_MAX_PARTITION][4];
	
	eLIBs_memset(root_type, 0, sizeof(root_type));
	eLIBs_memset(disk_name, 0, sizeof(disk_name));
	eLIBs_memset(para->search_path, 0, sizeof(para->search_path));
	//ret = rat_get_partition_name(RAT_SD_CARD, para->search_path);
	
	switch(para->root_type)
	{
		case RAT_LOCAL:
			eLIBs_strcpy(root_type,RAT_LOCAL_DISK);				
			break;
		case RAT_TF:
			eLIBs_strcpy(root_type,RAT_SD_CARD);				
			break;
		case RAT_USB:
			eLIBs_strcpy(root_type,RAT_USB_DISK);				
			break;
		default:
			break;
	}

    __msg("root_type=%s\n", root_type);
	ret = rat_get_partition_name(root_type, disk_name);
    __msg("para->search_path=%s\n", para->search_path);
	eLIBs_strcpy(para->search_path,disk_name[0]);	
	//__msg("");
	
	//eLIBs_memcpy(para->search_path,"f:", sizeof("f:"));	
	return ;
}


__s32 explorer_list_long_string_init(explr_list_para_t *this)
{
    if(this->long_str_handle == NULL)
    {
        __show_info_move_t show_info;
          
        eLIBs_memset(&show_info, 0, sizeof(__show_info_move_t));
        show_info.hlayer = this->list_layer;
        show_info.alpha_en = EPDK_TRUE;
        show_info.pFont = NULL;
        show_info.string = "";
        show_info.fontColor = GUI_WHITE;
        show_info.align = GUI_TA_LEFT | GUI_TA_VCENTER;
        show_info.bmp = explorer_get_listview_icon_res(ID_EXP_LIST_ITEM_FOCUS_BG);
        show_info.bmp_pos.x = 0;
        show_info.bmp_pos.y = 0;

        this->long_str_handle = GUI_LongStringCreate(&show_info);
		if(this->long_str_handle == NULL)
		{
			__wrn("create long string failed!\n");
		}
    }        
	else
	{
		__msg("long string scroll has already started!\n");
	}        
    return EPDK_OK;
}

__s32 explorer_list_long_string_uninit(explr_list_para_t *this)
{
	if(this->long_str_handle != NULL)
	{
		GUI_LongStringDelete(this->long_str_handle);
		this->long_str_handle = NULL;
	}
	return EPDK_OK;
}

__s32 explorer_list_long_string_start_roll_fast(__lbar_draw_para_t *draw_param, char *string)	
{
	__show_info_move_t show_info;
	__epdk_charset_enm_e encode = EPDK_CHARSET_ENM_UTF8;
	RECT roll_rect, text_rect;
	explorer_viewer_ui_t* ui_para;
	
	explr_list_para_t *this;
	__u32 font_size = 0;
	this = (explr_list_para_t *)draw_param->attr;		//for draw the picture  in different media type
	
	
	if(this->long_str_handle != NULL)
	{
		eLIBs_memset(&show_info, 0, sizeof(__show_info_move_t));
		
		ui_para = explorer_get_viewer_ui_param();			
				
		//roll_rect.x = draw_param->rect.x  + ui_para->list_item_ui_param.text_rect.x;
		//roll_rect.y = draw_param->rect.y  + ui_para->list_item_ui_param.text_rect.y;
		//roll_rect.width = ui_para->list_item_ui_param.text_rect.width+1;
		//roll_rect.height = ui_para->list_item_ui_param.text_rect.height;
		
		explorer_get_item_text_rect(this,&text_rect);	//���text rectangle
		
		roll_rect.x = draw_param->rect.x  + text_rect.x;
		roll_rect.y = draw_param->rect.y  + text_rect.y;
		roll_rect.width = text_rect.width+1;				
		roll_rect.height = text_rect.height+1;				
		font_size = GUI_GetFontDistY();

		
	    show_info.hlayer = this->list_layer;
	    show_info.region.x = roll_rect.x;
	    show_info.region.y = roll_rect.y;
	    show_info.region.width = roll_rect.width;
	    show_info.region.height = roll_rect.height;
	    show_info.alpha_en = EPDK_FALSE;
	    show_info.pFont = this->font;
	    show_info.string = string;
	    show_info.encode_id = encode;		
	    
	    show_info.fontColor = GUI_WHITE;
		//show_info.bkColor =GUI_BLACK;
	    show_info.align = GUI_TA_LEFT | GUI_TA_VCENTER;

		if(this->view_mode == EXPLR_LIST_MODE)
		{
			show_info.bmp = explorer_get_listview_icon_res(ID_EXP_LONG_STR_SCROLL_BG);
	    	show_info.bmp_pos.x = 0;
		   	show_info.bmp_pos.y = ui_para->list_item_ui_param.focus_icon_rect.y;
		}
	    else
	    {
			show_info.bmp = NULL;
	    	show_info.bmp_pos.x = 0;
		   	show_info.bmp_pos.y = 0;	    
	    }
		
	    GUI_LongStringStop(this->long_str_handle);
	    GUI_LongStringReset(this->long_str_handle,&show_info);
	    GUI_LongStringStart(this->long_str_handle);	
		
	}
	return EPDK_OK;
}
//
__s32 explorer_list_long_string_start_roll(explr_list_para_t *this)
{

	return EPDK_OK;
}

static __s32 explorer_list_long_string_stop_roll(explr_list_para_t *this)
{
	if(this->long_str_handle!= NULL)
    {    
        return GUI_LongStringStop(this->long_str_handle);			
    }
	return EPDK_FAIL;
}

