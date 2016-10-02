/*
**************************************************************************************************************
*											         ePDK
*						            the Easy Portable/Player Develop Kits
*									           desktop system 
*
*						        	 (c) Copyright 2007-2010, ANDY, China
*											 All Rights Reserved
*
* File    		: app_root_win_scene.c
* By      		: Bayden
* Func		: app_root applets plugin interface define
* Version	: v1.0
* ============================================================================================================
* 2010-9-28 17:11:20  andy.zhang  create this file, implements the fundemental interface;
**************************************************************************************************************
*/

#include "beetles_app.h"
#include "app_root.h"
#include "app_root_init.h"
#include "app_root_scene.h"

#include "mainmenu/mainmenu.h"
#include "explorer/explorer.h"
#include "movie/app_movie.h"
#include "music/music.h"
#include "photo/app_photo.h"
#include "setting/app_setting.h"
#include "fm/app_fm.h"
#include "ebook/app_ebook.h"
#include "record/app_record.h"
#if EXTERN_KARAOKE_EXIST
#include "karaoke/app_karaoke.h"
#endif
#include "dtv/dtv_i.h"

#if BEETLES_LINEIN_DETECT
typedef struct tag_root_globe_check
{
	__u8 	globe_thread_id;
	__hdle	hdl;
	__bool	linein_In;// linein

}root_globe_check;
#endif



typedef struct tag_root_ctrl
{
	H_LYR	lyr_app_mainmenu;		
	H_WIN	h_app_mainmenu;
	H_WIN	h_app_music;
	H_WIN	h_app_movie;
	H_WIN	h_app_photo;
	H_WIN	h_app_ebook;
	H_WIN	h_app_explorer;
	H_WIN	h_app_fm;
	H_WIN	h_app_setting;		
	H_WIN	h_app_record;
	H_WIN	h_app_tv;
#if EXTERN_KARAOKE_EXIST
	H_WIN	h_app_karaoke;
#endif

#if ANTENNA_MODE_SELECT
	H_WIN	h_antenna_select_dialog;
#endif
	root_para_t *root_para;


}root_ctrl_t;


/*******************************************************************************************************************************/
#if BEETLES_KEY_REDEF
static __u32	key_switch = 0;
static __bool 	menu_key_delay = EPDK_TRUE;

//static __u32	key_esc2menu = 0;////2>= menu
#else
static __bool 	menu_key_delay = EPDK_TRUE;
#endif


#if BEETLES_LINEIN_DETECT
static __bool g_linin_is_first_read = EPDK_TRUE;

//test_tv  ����
//__s32	tv_play_once = 0;

//goble detect init
static __s32 app_root_globe_check_init( root_ctrl_t *para )
{
	__s32            ret;
	user_gpio_set_t  gpio_set[1];

	g_linin_is_first_read = EPDK_TRUE;   
	eLIBs_memset(gpio_set, 0, sizeof(user_gpio_set_t));	

	// gpio_set->port = 1;				//PA
	// gpio_set->port_num = 0;			//0
	// gpio_set->mul_sel = 0;			//input
	// gpio_set->pull = 2;				//pull up
	// gpio_set->drv_level = 1;
	// gpio_set->data = 0;				//low

	ret = esCFG_GetKeyValue("linein_para", "linein", (int*)gpio_set, sizeof(user_gpio_set_t)/sizeof(int));
	if(EPDK_OK != ret)
	{
	    __wrn("----read linein para fail\n");
	    return EPDK_FAIL;
	}    

	para->globe_check.hdl = esPINS_PinGrpReq((__user_pin_set_t*)gpio_set, 1);
	if(NULL == para->globe_check.hdl)
	{
	    __wrn("----request linein pin fail\n");
	    return EPDK_FAIL;
	}

	ret = esPINS_SetPinPull(para->globe_check.hdl, PIN_PULL_UP, NULL);
	if (ret)
	{
		__wrn("----pull input_row1 failed\n");
		return EPDK_FAIL;
	}

	return EPDK_OK;
}

static __s32 app_root_globe_check_uninit( root_ctrl_t *para )
{
	if(NULL == para || NULL == para->globe_check.hdl)
	{
		__wrn("request linein pin fail...\n");
		return EPDK_FAIL;
	}

	esPINS_PinGrpRel(para->globe_check.hdl, 0);
	para->globe_check.hdl = NULL;

	return EPDK_OK;
}

static __s32 app_root_get_linein_state( root_ctrl_t *para )
{    
	static __s32 recorded_cur_data = 1;
	static __s32 last_read_data = 1;
	static s32 cntDly = 0;
	__s32 cur_read_data;
	__s32 ret = 0;
	__s32 line_in_delay = 3;

	if(NULL == para->globe_check.hdl)
	{
		return 0;
	}

	cur_read_data = esPINS_ReadPinData(para->globe_check.hdl, 0);    
	if(-1 == cur_read_data)
	{
		return 0;
	}

	if(EPDK_TRUE == g_linin_is_first_read)
	{
		last_read_data = cur_read_data;
		g_linin_is_first_read = EPDK_FALSE;
		return 0;
	}

	if(cur_read_data == last_read_data)
	{
		cntDly++;
	}
	else
	{
		cntDly = 0;
	}
	//__wrn("cntDly=%d\n", cntDly);
	__wrn("linein pin data=%d\n", cur_read_data);  

	last_read_data = cur_read_data;    
	if(cntDly >= line_in_delay)
	{        
		if(0 == cur_read_data)
		{
			if(1 == recorded_cur_data)
			{
				para->globe_check.linein_In = EPDK_TRUE;
				ret = -1;
			}
		}
		else if(1 == cur_read_data)
		{
			if(0 == recorded_cur_data)
			{
				para->globe_check.linein_In = EPDK_TRUE;
				ret = 1;
			}
		}     

		recorded_cur_data = cur_read_data;
		cntDly = 0;
	}   

	return ret;
}


static void  app_root_cmd2self(root_ctrl_t *para, __s32 lword, __s32 hword, __s32 data2, __s32 reserved)
{
	__gui_msg_t msg;

    eLIBs_memset(&msg, 0, sizeof(__gui_msg_t));

	msg.h_deswin = para->root_para->h_parent;
	msg.h_srcwin = NULL;
	msg.id = GUI_MSG_COMMAND;
	msg.dwAddData1 = MAKELONG(lword, hword);
	msg.dwAddData2 = data2;
	msg.dwReserved = reserved;

	GUI_SendMessage(&msg);
}

static __s32 app_root_linein_change( root_ctrl_t *para, __s32 lineinSt )
{
	__s32 ret;
	__gui_msg_t 	msg;

	if( lineinSt == 1 )// in
	{
		__here__;
		if( NULL == para->h_app_linin )
		{            
			__here__;
			//delete all active & switch to line in
			app_root_cmd2self(para,APP_LINEIN_ID,SWITCH_TO_LINE_IN,0,0);                 
		}
	}
	else if( lineinSt == -1 )//out
	{
		__here__;
		if( para->h_app_linin )
		{            
			__here__;
			//delete all active & switch to main menu
			app_root_cmd2self(para,APP_LINEIN_ID,SWITCH_TO_MMENU,0,0);                 
		}
	}

	return EPDK_OK;
}



static void app_root_globe_thread(void *arg)
{
	__bool	loopFlag = EPDK_TRUE;
	__s32	lineinSt = 0;//0 none, 1 in, -1 out
	root_ctrl_t* para = NULL;
	root_para_t* root_para = NULL;

	__here__;
	if( arg != NULL )
	{
		para = (root_ctrl_t *)arg;
		root_para = (root_para_t *)para->root_para;
		app_root_globe_check_init(para);
	}
	else
	{
		loopFlag = EPDK_FALSE;
	}

	while(loopFlag)
	{
		if( esKRNL_TDelReq(OS_PRIO_SELF) == OS_TASK_DEL_REQ )
		{
			__here__;
			loopFlag = EPDK_FALSE;
		}

		//Line in check
		lineinSt = app_root_get_linein_state(para);
		//__wrn("lineinSt=%d\n", lineinSt);
		//app_root_linein_change( para, lineinSt );

		esKRNL_TimeDly(20);
	}

	__here__;
	app_root_globe_check_uninit(para);
	esKRNL_TDel(OS_PRIO_SELF);
}
#endif
///////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

static __u32 root_check_disk(void)
{
	__s32 ret;
	__u32 root_type = RAT_UNKNOWN;
	char diskname[RAT_MAX_PARTITION][4] = {0x0};

	__msg("------root_check_disk-----\n");
	ret = rat_get_partition_name(RAT_USB_DISK, diskname);
	if(EPDK_OK == ret)
	{
		__msg("get USB fs part\n");	
		
		if (diskname[0][0])
		{
			__msg("USB : diskname=%s\n", diskname[0]);
			root_type += RAT_USB;
		}
	}
	ret = rat_get_partition_name(RAT_SD_CARD, diskname);
	if(EPDK_OK == ret)
	{    
		__msg("get SD fs part\n");		

		if (diskname[0][0])
		{
			__msg("SD : diskname=%s\n", diskname[0]);
			root_type += RAT_TF<<8;
		}
	}
	__msg("get NULL fs part\n");

	return root_type;   
}

//¼ì²é¶ÔÓ¦µÄÅÌ·ûÊÇ·ñ´æÔÚÖ¸¶¨µÄÃ½ÌåÎÄ¼þ
#if EXTERN_KARAOKE_EXIST  //add by hot.lee
static __s32 root_karaoke_check(__s32 root_type)
{
	H_KAK  hkak = NULL;
	__s32 total = 0;
	__s32 ret = 0;
	char 	search_path[128];
	char        kak_path[128];
	
__here__;
	ret = root_rat_get_search_path(root_type, search_path);
	if(ret == EPDK_FAIL)
	{
		return EPDK_FAIL;
	}
__here__;
	if(eLIBs_strlen(search_path) == 0)
	{
		return EPDK_FAIL;
	}
__here__;
eLIBs_sprintf(kak_path,"%c%s",search_path[0],KARAOKE_KAK_DATA_PATH);
	hkak = (H_KAK)kak_open(kak_path);
	if(NULL == hkak)
	{
		return EPDK_FAIL;
	}
__here__;
#if  0
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
	__wrn("list_para->rat.total\n");
#endif
__here__;
	total = kak_get_total_song_count(hkak);////////////////rat_get_cur_scan_cnt(list_para->rat.handle);;
	__msg("list_para->rat.total = %d\n",total);
	if(total)
	{
		kak_close(hkak);
		return EPDK_OK;
	}
__wrn("list_para->rat.total ok\n");
	kak_close(hkak);
return EPDK_FAIL;
}
#endif



static __s32 app_root_show_dlg(H_WIN hwin, __s32 str_id, __s32 time)
{
	H_WIN dlg;
	__s32 lang_id[2]={STRING_SET_CUE,STRING_SET_CUE};	
	__s32 null_disk;

	lang_id[1] = str_id;

	GUI_LyrWinCacheOn();
	
	default_dialog(dlg, hwin, APP_COM_DLG_ID, ADLG_OK, lang_id);
    GUI_LyrWinCacheOff(); 
    if(0 != time)
    {
	    esKRNL_TimeDly(time);
    }
    else
    {
        esKRNL_TimeDly(150);
    }   
	app_dialog_destroy(dlg);
}


static __s32 root_check_mediafile_exist(__s32 root_type, rat_media_type_t type)
{
	__s32 ret, total;
	HRAT  h_rat = 0;
	char path[RAT_MAX_FULL_PATH_LEN];

	eLIBs_memset(path, 0, sizeof(path));
	ret = root_rat_get_search_path(root_type, path);	
	if(ret == EPDK_FAIL)
	{
		return EPDK_FAIL;
	}
	if(eLIBs_strlen(path) == 0)
	{
		return EPDK_FAIL;
	}
	
	h_rat  = rat_open(path, type, 0);
	
	if(h_rat == NULL)
	{
		return EPDK_FAIL;
	}	
	//rat_move_cursor_to_first(list_para->rat.handle);
	
	total = rat_get_cur_scan_cnt(h_rat);
	
	if(total)
	{
		return EPDK_OK;	
	}
	return EPDK_FAIL;
}

static __s32  root_rat_get_search_path(__s32 root_type, char* path)
{
	__s32 ret = 0;
	char s_root_type[32];
	char disk_name[RAT_MAX_PARTITION][4];
	
	eLIBs_memset(s_root_type, 0, sizeof(s_root_type));
	eLIBs_memset(disk_name, 0, sizeof(disk_name));	

	switch(root_type)
	{
		case RAT_LOCAL:
			eLIBs_strcpy(s_root_type,RAT_LOCAL_DISK);				
			break;
		case RAT_TF:
			eLIBs_strcpy(s_root_type,RAT_SD_CARD);				
			break;
		case RAT_USB:
			eLIBs_strcpy(s_root_type,RAT_USB_DISK);				
			break;
		default:
			eLIBs_strcpy(s_root_type,RAT_SD_CARD);
			break;
	}
	ret = rat_get_partition_name(s_root_type, disk_name);
	eLIBs_strcpy(path,disk_name[0]);	
	    
	return ret;
}
__s32 cai_check_mediafile_esist(__s32 root_type,  __u32  media_app)
{
	__s32 ret = 0;
	switch(media_app)
	{
#if EXTERN_KARAOKE_EXIST
		case ID_MAINMENU_KARAOKE:
		{
		__here__;
			ret = root_karaoke_check(root_type);
			if(0 == ret)
			{
				return EPDK_OK;
			}
			else
			{
				gscene_hbar_current_disk(0xf,0xf);
			}
			break;
		}
#endif		
		case ID_MAINMENU_MUSIC:
		{
		__here__;
			ret = root_check_mediafile_exist(root_type, RAT_MEDIA_TYPE_AUDIO);
			if(0 == ret)
			{
				return EPDK_OK;
			}
			else
			{
				gscene_hbar_current_disk(0xf,0xf);   //没有对应的媒体文件
			}
			break;
		}
		case ID_MAINMENU_MOVIE:
		{
		__here__;	
			ret = root_check_mediafile_exist(root_type, RAT_MEDIA_TYPE_VIDEO);
			if(0 == ret)
			{
				return EPDK_OK;
			}
			else
			{
				gscene_hbar_current_disk(0xf,0xf);   //没有对应的媒体文件
			}
			break;
		}
		case ID_MAINMENU_EBOOK:
		{
		__here__;	
			ret = root_check_mediafile_exist(root_type, RAT_MEDIA_TYPE_EBOOK);
			if(0 == ret)
			{
				return EPDK_OK;
			}
			else
			{
				gscene_hbar_current_disk(0xf,0xf);   //没有对应的媒体文件
			}
			break;
		}
		case ID_MAINMENU_PHOTO:
		{
		__here__;	
			ret = root_check_mediafile_exist(root_type, RAT_MEDIA_TYPE_PIC);
			if(0 == ret)
			{
				return EPDK_OK;
			}
			else
			{
				gscene_hbar_current_disk(0xf,0xf);   //没有对应的媒体文件
			}
			break;
		}
		default:
			break;
	}
	return EPDK_FAIL;
}

static __s32 delete_all_app(root_ctrl_t   *root_ctrl)
{

#if BEETLES_KEY_REDEF
	key_switch = 0;
	menu_key_delay = EPDK_TRUE;
#endif
	__here__;   
	if(root_ctrl->h_app_music!= NULL)
	{
		__here__;
		GUI_ManWinDelete(root_ctrl->h_app_music);	
		root_ctrl->h_app_music = 0; 
	}
	if(root_ctrl->h_app_movie!= NULL)
	{
		__here__;
		GUI_ManWinDelete(root_ctrl->h_app_movie);
		root_ctrl->h_app_movie = 0; 
	}
	if(root_ctrl->h_app_photo!= NULL)
	{
		__here__;
		GUI_ManWinDelete(root_ctrl->h_app_photo);
		root_ctrl->h_app_photo = 0; 
	}
	if(root_ctrl->h_app_explorer!= NULL)
	{
		__here__;
		GUI_ManWinDelete(root_ctrl->h_app_explorer);
		root_ctrl->h_app_explorer = 0; 
	}
	if(root_ctrl->h_app_fm!= NULL)
	{
		__here__;
		GUI_ManWinDelete(root_ctrl->h_app_fm);	
		root_ctrl->h_app_fm = 0; 
#if BEETLES_KEYTONE_ENABLE
		//keytone reinit
		{
			__set_keytone_t state;

			dsk_keytone_get_state(&state);
			dsk_keytone_exit();
			dsk_keytone_init(BEETLES_APP_ROOT"res\\sounds\\chord.wav");                    
			dsk_keytone_set_state(state);
		}
#endif
	}
	if(root_ctrl->h_app_record!= NULL)
	{
		__here__;
		GUI_ManWinDelete(root_ctrl->h_app_record);	
		root_ctrl->h_app_record = 0; 
	}
	if(root_ctrl->h_app_setting != NULL)
	{
		__here__;
		GUI_ManWinDelete(root_ctrl->h_app_setting);
		root_ctrl->h_app_setting =0;
	}
	if(root_ctrl->h_app_ebook != NULL)
	{
		__here__;
		GUI_ManWinDelete(root_ctrl->h_app_ebook);
		root_ctrl->h_app_ebook = 0;
	}
	if(root_ctrl->h_app_tv != NULL)
	{
		__here__;
		GUI_ManWinDelete(root_ctrl->h_app_tv);
		root_ctrl->h_app_tv= 0;
		//////????
		//dsk_volume_set(25);
	}
#if EXTERN_KARAOKE_EXIST
	if(root_ctrl->h_app_karaoke != NULL)
	{
		__here__;
		GUI_ManWinDelete(root_ctrl->h_app_karaoke);
		root_ctrl->h_app_karaoke = 0;
	}
#endif
#if ANTENNA_MODE_SELECT
	if(root_ctrl->h_antenna_select_dialog)
	{
		app_dialog_destroy(root_ctrl->h_antenna_select_dialog);
		root_ctrl->h_antenna_select_dialog = NULL;	
	}
#endif
	if(GUI_LYRWIN_STA_ON == GUI_LyrWinGetSta(root_ctrl->lyr_app_mainmenu))
	{
		__here__;
		{
			__gui_msg_t 		ntymsg;
			ntymsg.id			=  GUI_MSG_SET_UNFOCUS;
			ntymsg.h_srcwin 		=  NULL;
			ntymsg.h_deswin 		=  root_ctrl->h_app_mainmenu; 
			ntymsg.dwAddData1	=  NULL;
			ntymsg.dwAddData2	=  NULL;
			GUI_SendMessage(&ntymsg); 
		}
		GUI_LyrWinSetSta(root_ctrl->lyr_app_mainmenu, GUI_LYRWIN_STA_SUSPEND);
	}
	__here__;

	return EPDK_OK;
}

static __s32 delete_all_app_except_music(root_ctrl_t   *root_ctrl)
{

#if BEETLES_KEY_REDEF
	key_switch = 0;
	menu_key_delay = EPDK_TRUE;
#endif
	__here__;   
/*
	if(root_ctrl->h_app_music!= NULL)
	{
		__here__;
		GUI_ManWinDelete(root_ctrl->h_app_music);	
		root_ctrl->h_app_music = 0; 
	}
*/	
#if ANTENNA_MODE_SELECT
	if(root_ctrl->h_antenna_select_dialog)
	{
		app_dialog_destroy(root_ctrl->h_antenna_select_dialog);
		root_ctrl->h_antenna_select_dialog = NULL;	
	}
#endif

	if(root_ctrl->h_app_movie!= NULL)
	{
		__here__;
		GUI_ManWinDelete(root_ctrl->h_app_movie);
		root_ctrl->h_app_movie = 0; 
	}
	if(root_ctrl->h_app_photo!= NULL)
	{
		__here__;
		GUI_ManWinDelete(root_ctrl->h_app_photo);
		root_ctrl->h_app_photo = 0; 
	}
	if(root_ctrl->h_app_explorer!= NULL)
	{
		__here__;
		GUI_ManWinDelete(root_ctrl->h_app_explorer);
		root_ctrl->h_app_explorer = 0; 
	}
	if(root_ctrl->h_app_fm!= NULL)
	{
		__here__;
		GUI_ManWinDelete(root_ctrl->h_app_fm);	
		root_ctrl->h_app_fm = 0; 
#if BEETLES_KEYTONE_ENABLE
		//keytone reinit
		{
			__set_keytone_t state;

			dsk_keytone_get_state(&state);
			dsk_keytone_exit();
			dsk_keytone_init(BEETLES_APP_ROOT"res\\sounds\\chord.wav");                    
			dsk_keytone_set_state(state);
		}
#endif
	}
	if(root_ctrl->h_app_record!= NULL)
	{
		__here__;
		GUI_ManWinDelete(root_ctrl->h_app_record);	
		root_ctrl->h_app_record = 0; 
	}
	if(root_ctrl->h_app_setting != NULL)
	{
		__here__;
		GUI_ManWinDelete(root_ctrl->h_app_setting);
		root_ctrl->h_app_setting =0;
	}
	if(root_ctrl->h_app_ebook != NULL)
	{
		__here__;
		GUI_ManWinDelete(root_ctrl->h_app_ebook);
		root_ctrl->h_app_ebook = 0;
	}
	if(root_ctrl->h_app_tv != NULL)
	{
		__here__;
		GUI_ManWinDelete(root_ctrl->h_app_tv);
		root_ctrl->h_app_tv= 0;
		//////????
		//dsk_volume_set(25);
	}
#if EXTERN_KARAOKE_EXIST
	if(root_ctrl->h_app_karaoke != NULL)
	{
		__here__;
		GUI_ManWinDelete(root_ctrl->h_app_karaoke);
		root_ctrl->h_app_karaoke = 0;
	}
#endif	
	if(GUI_LYRWIN_STA_ON == GUI_LyrWinGetSta(root_ctrl->lyr_app_mainmenu))
	{
		__here__;
		{
			__gui_msg_t 		ntymsg;
			ntymsg.id			=  GUI_MSG_SET_UNFOCUS;
			ntymsg.h_srcwin 		=  NULL;
			ntymsg.h_deswin 		=  root_ctrl->h_app_mainmenu; 
			ntymsg.dwAddData1	=  NULL;
			ntymsg.dwAddData2	=  NULL;
			GUI_SendMessage(&ntymsg); 
		}
		GUI_LyrWinSetSta(root_ctrl->lyr_app_mainmenu, GUI_LYRWIN_STA_SUSPEND);
	}
	__here__;

	return EPDK_OK;
}

static __s32 delete_all_app_except_home(root_ctrl_t   *root_ctrl)
{

#if BEETLES_KEY_REDEF
	key_switch = 0;
	menu_key_delay = EPDK_TRUE;
#endif
	__here__;

#if ANTENNA_MODE_SELECT
	if(root_ctrl->h_antenna_select_dialog)
	{
		app_dialog_destroy(root_ctrl->h_antenna_select_dialog);
		root_ctrl->h_antenna_select_dialog = NULL;	
	}
#endif

	if(root_ctrl->h_app_music!= NULL)
	{
		__here__;
		GUI_ManWinDelete(root_ctrl->h_app_music);	
		root_ctrl->h_app_music = 0; 
	}
	if(root_ctrl->h_app_movie!= NULL)
	{
		__here__;
		GUI_ManWinDelete(root_ctrl->h_app_movie);
		root_ctrl->h_app_movie = 0; 	
	}
	if(root_ctrl->h_app_photo!= NULL)
	{
		__here__;
		GUI_ManWinDelete(root_ctrl->h_app_photo);
		root_ctrl->h_app_photo = 0; 	
	}
	if(root_ctrl->h_app_explorer!= NULL)
	{
		__here__;
		GUI_ManWinDelete(root_ctrl->h_app_explorer);
		root_ctrl->h_app_explorer = 0; 	
	}
	if(root_ctrl->h_app_fm!= NULL)
	{
		__here__;
		GUI_ManWinDelete(root_ctrl->h_app_fm);	
		root_ctrl->h_app_fm = 0; 
#if BEETLES_KEYTONE_ENABLE
		//keytone reinit
		{
			__set_keytone_t state;

			dsk_keytone_get_state(&state);
			dsk_keytone_exit();
			dsk_keytone_init(BEETLES_APP_ROOT"res\\sounds\\chord.wav");                    
			dsk_keytone_set_state(state);
		}
#endif
	}
	if(root_ctrl->h_app_record!= NULL)
	{
		__here__;
		GUI_ManWinDelete(root_ctrl->h_app_record);	
		root_ctrl->h_app_record = 0; 		
	}
	if(root_ctrl->h_app_setting != NULL)
	{
		__here__;
		GUI_ManWinDelete(root_ctrl->h_app_setting);
		root_ctrl->h_app_setting =0;
	}
	if(root_ctrl->h_app_ebook != NULL)
	{
		__here__;
		GUI_ManWinDelete(root_ctrl->h_app_ebook);
		root_ctrl->h_app_ebook = 0;
	}
	if(root_ctrl->h_app_tv != NULL)
	{
		__here__;
		GUI_ManWinDelete(root_ctrl->h_app_tv);
		root_ctrl->h_app_tv= 0;
		//////????
		//dsk_volume_set(25);
	}
#if EXTERN_KARAOKE_EXIST
	if(root_ctrl->h_app_karaoke != NULL)
	{
		__here__;
		GUI_ManWinDelete(root_ctrl->h_app_karaoke);
		root_ctrl->h_app_karaoke = 0;
	}
#endif
	__here__;

	return EPDK_OK;
}

static void app_home_restore(root_ctrl_t   *root_ctrl)
{

#if BEETLES_KEY_REDEF
	key_switch = 0;
	menu_key_delay = EPDK_TRUE;
#endif
	if(!dsk_audio_echo_is_open())
	{
		if(EPDK_FALSE == robin_is_open())
		dsk_speaker_onoff(BEETLES_SPEAK_OFF);
	}
	else
	{
		if(EPDK_FALSE == dsk_speaker_is_on())
			dsk_speaker_onoff(BEETLES_SPEAK_ON);
	}
	__here__;
	gscene_bgd_set_state(BGD_STATUS_SHOW);           
	gscene_bgd_set_bottom();
	if(GUI_LYRWIN_STA_ON != GUI_LyrWinGetSta(root_ctrl->lyr_app_mainmenu))
	{
		GUI_LyrWinSetSta(root_ctrl->lyr_app_mainmenu, GUI_LYRWIN_STA_ON);
	}
	gscene_hbar_set_state(HBAR_ST_SHOW);
		__here__;
	GUI_WinSetFocusChild(root_ctrl->h_app_mainmenu);

	return;
}

static __s32  app_root_get_search_path(rat_root_t rat_root_type, char* search_path)
{
	__s32 ret = EPDK_OK;
	char str_type[50];
	char disk_name[RAT_MAX_PARTITION][4];
	
	eLIBs_memset(str_type, 0, sizeof(str_type));
	eLIBs_memset(disk_name, 0, sizeof(disk_name));
	eLIBs_memset(search_path, 0, sizeof(search_path));
		
	switch(rat_root_type)
	{
		case RAT_LOCAL:
			eLIBs_strcpy(str_type,RAT_LOCAL_DISK);				
			break;
		case RAT_TF:
			eLIBs_strcpy(str_type,RAT_SD_CARD);				
			break;
		case RAT_USB:
			eLIBs_strcpy(str_type,RAT_USB_DISK);				
			break;
		default:
            return EPDK_FAIL;			
	}
    //__wrn("=========str_type=%s\n", str_type);
	//ret = rat_get_partition_name(str_type, disk_name, 0);    
	ret = rat_get_partition_name(str_type, disk_name);   
	eLIBs_strcpy(search_path,disk_name[0]);	
   // __wrn("=========rat root type=%d\n", rat_root_type);
    __wrn("=========search_path=%s\n", search_path);
    if(0 == search_path[0])
    {
        return EPDK_FAIL;
    }
    
    return ret;
}


static __s32 app_root_set_playfile(rat_root_t rat_root_type, rat_media_type_t rat_media_type)
{
	char file_path[RAT_MAX_FULL_PATH_LEN];
	HRAT hrat = NULL;
	__u32 total = 0;
	__u32 last_file_index = 0;
	char *last_file_path = NULL;
	reg_root_para_t* para;
	int ret;
	rat_entry_t ItemInfo;
		
	if(RAT_UNKNOWN== rat_root_type || RAT_MEDIA_TYPE_UNKNOWN == rat_media_type)
	{
		return EPDK_FAIL;
	}
	
	eLIBs_memset(file_path, 0, sizeof(file_path));
	
	ret = app_root_get_search_path(rat_root_type, file_path);
	if(EPDK_FAIL == ret || eLIBs_strlen(file_path) == 0)	
	{
		return EPDK_FAIL;
	}
	
	hrat = rat_open(file_path, rat_media_type, 0);	
   // hrat = rat_open_ex(rat_root_type, rat_media_type, 0);	
	if(hrat == NULL)
	{
		return EPDK_FAIL;
	}
	
	total = rat_get_cur_scan_cnt(hrat);
	if(total == 0)
	{
		return EPDK_FAIL;
	}
	
	para = (reg_root_para_t*)dsk_reg_get_para_by_app(REG_APP_ROOT);
	if(para == NULL)
	{
		return EPDK_FAIL;
	}

    if(RAT_MEDIA_TYPE_VIDEO == rat_media_type)
    {
    	if(rat_root_type == RAT_TF)
    	{			
    		last_file_index = para->last_movie_index_sd;
    		last_file_path = para->last_movie_path_sd;
    	}
    	else
    	{
    		last_file_index = para->last_movie_index_ud;
    		last_file_path = para->last_movie_path_ud;	
    	}
    }
    else if(RAT_MEDIA_TYPE_AUDIO == rat_media_type)
    {
        if(rat_root_type == RAT_TF)
    	{			
    		last_file_index = para->last_music_index_sd;
    		last_file_path = para->last_music_path_sd;
    	}
    	else
    	{
    		last_file_index = para->last_music_index_ud;
    		last_file_path = para->last_music_path_ud;	
    	}
    }
    else if(RAT_MEDIA_TYPE_EBOOK == rat_media_type)
    {
        if(rat_root_type == RAT_TF)
    	{			
    		last_file_index = para->last_ebook_index_sd;
    		last_file_path = para->last_ebook_path_sd;
    	}
    	else
    	{
    		last_file_index = para->last_ebook_index_ud;
    		last_file_path = para->last_ebook_path_ud;	
    	}
    }
    else if(RAT_MEDIA_TYPE_PIC == rat_media_type)
    {
        if(rat_root_type == RAT_TF)
    	{			
    		last_file_index = para->last_photo_index_sd;
    		last_file_path = para->last_photo_path_sd;
    	}
    	else
    	{
    		last_file_index = para->last_photo_index_ud;
    		last_file_path = para->last_photo_path_ud;	
    	}
    }
	
	eLIBs_memset(&ItemInfo,0,sizeof(rat_entry_t));				
	ret = rat_get_item_info_by_index(hrat, last_file_index, &ItemInfo);	
	
	if(ret == EPDK_OK)
	{
		eLIBs_memset(file_path, 0, sizeof(file_path));
		eLIBs_strcpy(file_path, ItemInfo.Path);
		ret = eLIBs_strcmp(file_path, last_file_path);
		if(ret == 0)
		{
			rat_set_file_for_play(hrat,last_file_path);
			return EPDK_OK;
		}
	}		
	
	ret = rat_get_item_info_by_index(hrat, 0, &ItemInfo);
	if(ret == EPDK_OK)
	{
		eLIBs_memset(file_path, 0, sizeof(file_path));
		eLIBs_strcpy(file_path, ItemInfo.Path);
		rat_set_file_for_play(hrat,file_path);
		return EPDK_OK;
	}	
	
	return EPDK_FAIL;		
}


static void key2app(__gui_msg_t *msg, key_to_app_t app_flag)
{
	__u32 root_type = 0, temp_root=0;
	__u8 sd_root=0, usb_root = 0, device_cnt = 0;
	root_ctrl_t   *root_ctrl;
	root_para_t* root_para = NULL;
	char str[256]={0};
	H_WIN hmusic;

	__here__;
	root_ctrl = (root_ctrl_t *)GUI_WinGetAddData(msg->h_deswin);
	root_para = root_ctrl->root_para;

	if(app_flag !=KEY_TO_FM && app_flag !=KEY_TO_SETTING && app_flag != KEY_TO_TV)
	{
		root_type = root_check_disk();	
		sd_root = (root_type >> 8)&0x00ff;
		usb_root = (root_type)&0x00ff; 

		__msg("----sd root is %d, usb_root is %d\n", sd_root, usb_root);			
		if(sd_root)
		{
			if(usb_root)	
			{
				if(root_ctrl->root_para->root_type == sd_root)
				{			
					temp_root = RAT_USB;
				}
				else
				{
					temp_root = RAT_TF;		
				}
				
				device_cnt = 2;

				temp_root = root_ctrl->root_para->root_type;
			}	
			else
			{
				temp_root = RAT_TF;
				device_cnt = 1;
			}

		}
		
		else if(usb_root)		
		{
			temp_root = RAT_USB;
			device_cnt = 1;
		}
		else
		{
			if(GUI_LYRWIN_STA_ON == GUI_LyrWinGetSta(root_ctrl->lyr_app_mainmenu))
			{
				gscene_hbar_current_disk(0xff,0xff);

				app_root_show_dlg(root_ctrl->root_para->h_parent, STRING_NO_DISK, 0);

				GUI_WinSetFocusChild(root_ctrl->h_app_mainmenu);
			}
			return;
		}
	}
	
	__here__;
	switch(app_flag)
	{
		case KEY_TO_TV:
		{
			delete_all_app(root_ctrl);
			__here__;
			gscene_hbar_set_state(HBAR_ST_HIDE);
			dsk_speaker_onoff(BEETLES_SPEAK_ON);
			root_ctrl->root_para->h_parent= msg->h_deswin;
			root_ctrl->h_app_tv =  app_dtv_create(root_para);
			GUI_WinSetFocusChild(root_ctrl->h_app_tv);

			break;
		}
		
		case KEY_TO_MOVIE:
		{
			if(app_root_set_playfile(temp_root,RAT_MEDIA_TYPE_VIDEO))
			{
				__here__;

				
				if(GUI_LYRWIN_STA_ON == GUI_LyrWinGetSta(root_ctrl->lyr_app_mainmenu))
				{
					gscene_hbar_current_disk(0xf,0xf);
					GUI_WinSetFocusChild(root_ctrl->h_app_mainmenu);
				}
				return;
			}
			else
			{
				__here__;
				delete_all_app(root_ctrl);
			}		
			
			gscene_hbar_set_state(HBAR_ST_HIDE);

			root_ctrl->root_para->root_type = temp_root; 
			root_ctrl->root_para->explr_root = root_ctrl->root_para->root_type;  
			root_ctrl->root_para->data = KEY_SW_TO_MOVIE;
			root_ctrl->root_para->h_parent =  msg->h_deswin;               
			root_ctrl->h_app_movie = app_movie_create(root_ctrl->root_para);					
			GUI_WinSetFocusChild(root_ctrl->h_app_movie);

			break;	
		}

		case KEY_TO_EXPLOER:
		{
			if(app_root_set_playfile(temp_root,RAT_MEDIA_TYPE_ALL))
			{
				__here__;

				if(GUI_LYRWIN_STA_ON == GUI_LyrWinGetSta(root_ctrl->lyr_app_mainmenu))
				{
					gscene_hbar_current_disk(0xf,0xf);
					GUI_WinSetFocusChild(root_ctrl->h_app_mainmenu);
				}
				return;
			}
			else
			{
				__here__;
				delete_all_app(root_ctrl);
			}		
			
			gscene_hbar_set_state(HBAR_ST_HIDE);
			GUI_LyrWinSetSta(root_ctrl->lyr_app_mainmenu, GUI_LYRWIN_STA_SUSPEND);


			root_para->root_type = root_ctrl->root_para->root_type;
			root_para->explr_root = root_ctrl->root_para->root_type;
			root_para->data = ID_EXPLORER_ALL;
			root_ctrl->root_para->h_parent= msg->h_deswin;
			root_ctrl->h_app_explorer =  app_explorer_create(root_para);

			GUI_WinSetFocusChild(root_ctrl->h_app_explorer);

			break;	
		}
		
		case KEY_TO_MUSIC:
		{
			if(root_check_mediafile_exist(temp_root, RAT_MEDIA_TYPE_AUDIO))
			{
				__here__;

				if(GUI_LYRWIN_STA_ON == GUI_LyrWinGetSta(root_ctrl->lyr_app_mainmenu))
				{
					gscene_hbar_current_disk(0xf,0xf);
					GUI_WinSetFocusChild(root_ctrl->h_app_mainmenu);
				}
				return;
			}
			else
			{
				__s32 exist;
				H_WIN hmusic;
				
				exist = app_root_is_bg_music_exist(msg,&hmusic);

				if(exist)
				{
					__here__;
					delete_all_app_except_music(root_ctrl);

					gscene_hbar_set_state(HBAR_ST_HIDE);
					
					root_ctrl->root_para->root_type = temp_root; 
					root_ctrl->root_para->explr_root = root_ctrl->root_para->root_type;  
					root_ctrl->root_para->data = MUSICPLAYER_NORMAL;
					root_ctrl->root_para->h_parent =  msg->h_deswin;
					//root_ctrl->h_app_music = app_music_create(root_ctrl->root_para);
					app_music_restore(root_ctrl->h_app_music, root_ctrl->root_para);
					GUI_WinSetFocusChild(root_ctrl->h_app_music);
				}
				else
				{
					__here__;
					delete_all_app(root_ctrl);

					gscene_hbar_set_state(HBAR_ST_HIDE);
					
					root_ctrl->root_para->root_type = temp_root; 
					root_ctrl->root_para->explr_root = root_ctrl->root_para->root_type;  
					root_ctrl->root_para->data = MUSICPLAYER_NORMAL;
					root_ctrl->root_para->h_parent =  msg->h_deswin;
					root_ctrl->h_app_music = app_music_create(root_ctrl->root_para);					
					GUI_WinSetFocusChild(root_ctrl->h_app_music);

				}
			}		

			break;
		}

		
		case KEY_TO_FM:
		{
			delete_all_app(root_ctrl);
			__here__;
#if BEETLES_FM_HEADBAR
			gscene_hbar_set_state(HBAR_ST_HIDE);
#endif
			dsk_speaker_onoff(BEETLES_SPEAK_ON);
			root_ctrl->root_para->h_parent= msg->h_deswin;
			root_ctrl->h_app_fm =  app_fm_create(root_para);
			GUI_WinSetFocusChild(root_ctrl->h_app_fm);

			break;
		}
		case KEY_TO_RECORD:
		{
#if BEETLES_MICIN_DISABLE
			if(!dsk_audio_echo_is_open())
			{
				delete_all_app_except_home(root_ctrl);
				app_home_restore(root_ctrl);
				/////
				gscene_hbar_current_disk(0xe,0xe);//insert mic
				GUI_WinSetFocusChild(root_ctrl->h_app_mainmenu);
				return;
			}
			else
#endif			
			{
				delete_all_app(root_ctrl);
				__here__;
				gscene_hbar_set_state(HBAR_ST_HIDE);
				//dsk_speaker_onoff(BEETLES_SPEAK_ON);
				root_ctrl->root_para->root_type = temp_root;//RAT_USB»òÕßRAT_TF
				root_ctrl->root_para->record_opt = 0;//Õý³£Â¼Òô
				root_ctrl->root_para->h_parent= msg->h_deswin;
				root_ctrl->h_app_record=  app_record_create(root_para);
				GUI_WinSetFocusChild(root_ctrl->h_app_record);	
			}
			break;
		}
		case KEY_TO_PHOTO:
		{
			__here__;
			if(app_root_set_playfile(temp_root, RAT_MEDIA_TYPE_PIC))  //¼ì²é¶ÔÓ¦ÅÌÊÇ·ñ´æÔÚ¶ÔÓ¦µÄÒôÆµÎÄ¼þ
			{
				__here__;

				if(GUI_LYRWIN_STA_ON == GUI_LyrWinGetSta(root_ctrl->lyr_app_mainmenu))
				{
					gscene_hbar_current_disk(0xf,0xf);
					GUI_WinSetFocusChild(root_ctrl->h_app_mainmenu);
				}
				return;
			}
			else
			{		
				delete_all_app(root_ctrl);
			}

			gscene_hbar_set_state(HBAR_ST_HIDE);
			root_para->root_type = temp_root;
			root_para->explr_root = root_ctrl->root_para->root_type;
			root_para->data = KEY_SW_TO_PHOTO;
			root_ctrl->root_para->h_parent= msg->h_deswin;
			root_ctrl->h_app_photo =  app_photo_create(root_para);
			GUI_WinSetFocusChild(root_ctrl->h_app_photo);	

			break;
		}
		case KEY_TO_SETTING:
		{
			delete_all_app(root_ctrl);
			__here__;
			gscene_hbar_set_state(HBAR_ST_HIDE);
			root_ctrl->root_para->setting_opt = 0;
			root_ctrl->root_para->h_parent= msg->h_deswin;
			root_ctrl->h_app_setting=  app_setting_create(root_para);
			GUI_WinSetFocusChild(root_ctrl->h_app_setting);	

			break;
		}
		default:
			break;
	}		

	return;
}

static __s32 cmd_to_app_key_proc(__gui_msg_t *msg)
{
	root_ctrl_t   	*root_ctrl;
	root_ctrl = (root_ctrl_t *)GUI_WinGetAddData(msg->h_deswin);	
	
	__here__;


	eDbug("msg->dwAddData1 is %d \n",msg->dwAddData1);


	switch(msg->dwAddData1)
	{
		case GUI_MSG_KEY_TV:
		{
			__here__;
			if(root_ctrl->h_app_tv == NULL && KEY_UP_ACTION == msg->dwAddData2)
			{
				#if ANTENNA_MODE_SELECT
					delete_all_app(root_ctrl);
					app_root_antenna_select(msg);
				#else
					key2app(msg,KEY_TO_TV);
				#endif
			}
			
			return EPDK_OK;
		}
		case GUI_MSG_KEY_MOVIE:
		{
			__here__;
			if(root_ctrl->h_app_movie == NULL && KEY_UP_ACTION == msg->dwAddData2)
			{
				key2app(msg,KEY_TO_MOVIE);
			}
			
			return EPDK_OK;
		}

		
		case GUI_MSG_KEY_MUSIC:
		{
			__s32 exist;
			H_WIN hmusic;

			exist = app_root_is_bg_music_exist(msg,&hmusic);
			
			if(GUI_LYRWIN_STA_ON == GUI_LyrWinGetSta(root_ctrl->lyr_app_mainmenu))
			{
				__here__;

				if(KEY_UP_ACTION == msg->dwAddData2)
				{
					key2app(msg,KEY_TO_MUSIC);
				}
			}
			else
			{				
				__here__;
				if(1 == exist)
				{
					if((root_ctrl->h_app_photo != NULL) || (root_ctrl->h_app_ebook != NULL) && (KEY_UP_ACTION == msg->dwAddData2))
					{
						key2app(msg,KEY_TO_MUSIC);
					}
				}
				else
				{
					if(root_ctrl->h_app_music == NULL && KEY_UP_ACTION == msg->dwAddData2)
					{
						key2app(msg,KEY_TO_MUSIC);
					}
				}
			}
			return EPDK_OK;
		}
		
		case GUI_MSG_KEY_FM:
		{
			__here__;
			if(root_ctrl->h_app_fm == NULL && KEY_UP_ACTION == msg->dwAddData2)
			{
				key2app(msg, KEY_TO_FM);
			}
			
			return EPDK_OK;
		}


        
		case GUI_MSG_KEY_PICTURE:
		{
			__here__;
			if(root_ctrl->h_app_photo == NULL && KEY_UP_ACTION == msg->dwAddData2)
			{
				key2app(msg, KEY_TO_PHOTO);
			}
			
			return EPDK_OK;
		}
				
		case GUI_MSG_KEY_SETTING:
		{
			__here__;
			if(root_ctrl->h_app_setting == NULL && KEY_UP_ACTION ==msg->dwAddData2)
			{
				key2app(msg,KEY_TO_SETTING);
			}
			
			return EPDK_OK;
		}

		case GUI_MSG_KEY_VDEC:
		case GUI_MSG_KEY_LONGVDEC:
		{
			if((KEY_DOWN_ACTION == msg->dwAddData2) || (KEY_REPEAT_ACTION == msg->dwAddData2))
			{
				dsk_volume_dec();
				__msg("dsk_volume_get() is %d \n",dsk_volume_get());
			}
			return EPDK_FAIL;
		}
		
		case GUI_MSG_KEY_VADD:
		case GUI_MSG_KEY_LONGVADD:
		//case GUI_MSG_KEY_IR_VADD:
		//case GUI_MSG_KEY_LONGIR_VADD:
		{
			if((KEY_DOWN_ACTION == msg->dwAddData2) || (KEY_REPEAT_ACTION == msg->dwAddData2))
			{
				dsk_volume_inc();
			}
			return EPDK_FAIL;
		}
		
		case GUI_MSG_KEY_POWER:
		case GUI_MSG_KEY_LONGPOWER:
		{
			__here__;
			if((KEY_UP_ACTION ==msg->dwAddData2))
			{
				{
					__gui_msg_t msg;

					msg.id 			= DSK_MSG_SCREEN_CLOSE;
					msg.h_srcwin 		= NULL;
					msg.h_deswin		= GUI_WinGetHandFromName("init");
					msg.dwAddData1	= (__u32)0;
					msg.dwAddData2	= (__u32)0;
					
					GUI_SendNotifyMessage(&msg);

				}
				//////////
			}
			
			return EPDK_OK;
		}
		case GUI_MSG_KEY_MUTE:
		case GUI_MSG_KEY_LONGMUTE:
		{
			__here__;
			if(KEY_DOWN_ACTION == msg->dwAddData2)
			{
				if(dsk_volume_is_on())
				{
					dsk_volume_off();
				}
				else
					dsk_volume_on();
			}
			
			return EPDK_FAIL;
		}
	}

	return EPDK_FAIL;
}



static __s32 app_root_fs_plugin_restore(__gui_msg_t *msg) //CAI ++ FOR 连接USB后退出后没有磁盘了
{
	__u32 root_type = 0, temp_root=0;
	__u8 sd_root =0, usb_root = 0, device_cnt = 0;//device_cnt, 当前设备的个数	
	root_ctrl_t   *root_ctrl = NULL;

	root_ctrl = (root_ctrl_t *)GUI_WinGetAddData(msg->h_deswin);
	root_type = root_check_disk();	
	sd_root = (root_type >> 8)&0x00ff;
	usb_root = (root_type)&0x00ff; 
	__msg("----sd_root %d, usb_root %d\n", sd_root, usb_root);
	if(sd_root)
	{
		if(usb_root)	                    //两个都在,比较得知新插入的设备
		{
			//__msg("----root_type %d\n", root_ctrl->root_para->root_type);	
			if(root_ctrl->root_para->root_type == sd_root)//
			{			
				temp_root = RAT_USB;        //新插入的设备为USB
			}
			else
			{
				temp_root = RAT_TF;			//新插入的设备为SD	
			}
			device_cnt = 2;
		}	
		else
		{
			temp_root = RAT_TF;
			device_cnt = 1;
		}				
	}			
	else if(usb_root)	                    //插入的是USB设备,当前只有USB设备		
	{
		temp_root = RAT_USB;
		device_cnt = 1;
	}		
	else						//没检测到????	
	{
		return EPDK_FAIL;
	}
	__msg("----temp_root=%d----\n",temp_root);
      	root_ctrl->root_para->root_type = temp_root; 
	root_ctrl->root_para->root_type_cnt = device_cnt; 
	gscene_hbar_current_disk(temp_root,root_type);
	return EPDK_OK;
}    

static __s32 app_root_fs_device_plugin(__gui_msg_t *msg)
{
	ES_FILE* fp = NULL;
	int ret = EPDK_FAIL;
	char update_path[32] = {0};
	__u32 root_type = 0, temp_root=0;
	__u8 sd_root =0, usb_root = 0, device_cnt = 0;	
	root_ctrl_t   *root_ctrl = NULL;

	root_ctrl = (root_ctrl_t *)GUI_WinGetAddData(msg->h_deswin);
	root_type = root_check_disk();	
	
	sd_root = (root_type >> 8)&0x00ff;
	usb_root = (root_type)&0x00ff; 
	__msg("----sd_root %d, usb_root %d\n", sd_root, usb_root);

	if(sd_root)
	{
		if(usb_root)	                    
		{
			//__msg("----root_type %d\n", root_ctrl->root_para->root_type);	
			if(root_ctrl->root_para->root_type == sd_root)//
			{			
				temp_root = RAT_USB;
			}
			else
			{
				temp_root = RAT_TF;
			}
			device_cnt = 2;
		}	
		else
		{
			temp_root = RAT_TF;
			device_cnt = 1;
		}				
	}			
	else if(usb_root)		
	{
		temp_root = RAT_USB;
		device_cnt = 1;
	}		
	else
	{
		return EPDK_FAIL;
	}

	////auto update sd/usb sys/update.img
	//if(0 == para->time_set)
	//{
		if(temp_root >= RAT_TF)
		{
			ret = app_root_get_search_path(temp_root, update_path);
			if(EPDK_OK == ret)	
			{
			#if CAI_ADD_WRITE_DETECT
				spinor_wrsr(0<<2,0<<6);
			#endif
				eLIBs_strcat(update_path, "\\sys\\update.img");
				//__msg("2222&&&&&&&&&&update_path=%s\n",update_path);

				fp = eLIBs_fopen(update_path, "r");
				if(fp)
				{
					eLIBs_fclose(fp);
					fp = NULL;

					//para->time_set = 1;
					//dsk_reg_flush();
					
					esKRNL_TimeDly(50);
					dsk_start_fw_update(update_path);
					__here__;
					return EPDK_OK;
				}
				__here__;
			}
		}
	//}
	////////////
	
	__msg("----temp_root=%d----\n",temp_root);

	
	gscene_hbar_current_disk(temp_root,root_type);

#if BEETLES_DEVICE_PLUGIN_NOPLAY
	__here__;
	root_ctrl->root_para->root_type = temp_root; 
	root_ctrl->root_para->root_type_cnt = device_cnt;
#else

	if(app_root_set_playfile(temp_root,RAT_MEDIA_TYPE_VIDEO))
	{
		__here__;
		root_ctrl->root_para->root_type = temp_root; 
		root_ctrl->root_para->root_type_cnt = device_cnt;
		///////////
		if(root_check_mediafile_exist(temp_root, RAT_MEDIA_TYPE_AUDIO))
		{
			__here__;
			return EPDK_FAIL;
		}
		else
		{
			__here__;
			delete_all_app(root_ctrl);
			//rat_clear_all_list();
			///////
			gscene_hbar_set_state(HBAR_ST_HIDE);
			root_ctrl->root_para->root_type = temp_root; 
			root_ctrl->root_para->root_type_cnt = device_cnt; 
			root_ctrl->root_para->explr_root = root_ctrl->root_para->root_type;  
			root_ctrl->root_para->data = MUSICPLAYER_NORMAL;
			root_ctrl->root_para->h_parent =  msg->h_deswin;               
			root_ctrl->h_app_music = app_music_create(root_ctrl->root_para);					
			GUI_WinSetFocusChild(root_ctrl->h_app_music);

			return EPDK_OK;
		}
	}
	else
	{
		__here__;
		delete_all_app(root_ctrl);
		//rat_clear_all_list();
		///////
		gscene_hbar_set_state(HBAR_ST_HIDE);
		root_ctrl->root_para->root_type = temp_root; 
		root_ctrl->root_para->root_type_cnt = device_cnt; 
		root_ctrl->root_para->explr_root = root_ctrl->root_para->root_type;  
		root_ctrl->root_para->data = KEY_SW_TO_MOVIE;
		root_ctrl->root_para->h_parent =  msg->h_deswin;               
		root_ctrl->h_app_movie = app_movie_create(root_ctrl->root_para);					
		GUI_WinSetFocusChild(root_ctrl->h_app_movie);
	}
#endif

	return EPDK_OK;
}                

static __s32 app_root_fs_device_plugout(__gui_msg_t * msg)
{
	__u32 root_type = 0, temp_root=0;
	__u8 sd_root =0, usb_root = 0;	
	root_ctrl_t   *root_ctrl = NULL;
	
	root_ctrl = (root_ctrl_t *)GUI_WinGetAddData(msg->h_deswin);
	root_type = root_check_disk();	
	
	sd_root = (root_type >> 8)&0x00ff;
	usb_root = (root_type)&0x00ff; 

	__msg("----sd_root %d, usb_root %d\n", sd_root, usb_root);			
	if (sd_root == RAT_TF)
		temp_root = RAT_TF;
	else if(usb_root == RAT_USB)
		temp_root = RAT_USB;
	//////////////////

////////////////
if((RAT_TF == temp_root) || (0 == root_type))
	g_enable_standby();
//////////////////

	if(root_type == 0)
	{				
		gscene_hbar_current_disk(temp_root,root_type);
		root_ctrl->root_para->root_type = root_type; 
		root_ctrl->root_para->root_type_cnt = 0; 

		if(root_ctrl->h_app_setting != NULL || root_ctrl->h_app_tv != NULL || root_ctrl->h_app_fm != NULL)
		{
			rat_clear_all_list();

            if(root_ctrl->h_app_tv != NULL)
            {
				__gui_msg_t 		ntymsg;

				ntymsg.id			=  GUI_MSG_COMMAND;
				ntymsg.h_srcwin 	=  msg->h_deswin;
				ntymsg.h_deswin 	=  GUI_WinGetHandFromName("APP_DTV"); 
				ntymsg.dwAddData1	=  CMD_FS_PART_PLUGOUT;
				ntymsg.dwAddData2	=  NULL;
				ntymsg.dwReserved	=  NULL;
				
				GUI_SendNotifyMessage(&ntymsg); 

            }
			return EPDK_FAIL;
		}
		else
		{
			delete_all_app_except_home(root_ctrl);
			
			rat_clear_all_list();

			app_home_restore(root_ctrl);
			
			return EPDK_FAIL;
		}
	}	

	if(root_ctrl->root_para->root_type != temp_root)
	{

		gscene_hbar_current_disk(temp_root,root_type);
		root_ctrl->root_para->root_type = temp_root; 
		root_ctrl->root_para->root_type_cnt = 1; 
		root_ctrl->root_para->explr_root = root_ctrl->root_para->root_type;   
		#if 1
		delete_all_app_except_home(root_ctrl);
		rat_clear_all_list_by_root((void*)(msg->dwAddData2));
		app_home_restore(root_ctrl);
		#else
		delete_all_app(root_ctrl);
		rat_clear_all_list_by_root((void*)(msg->dwAddData2));
		gscene_hbar_set_state(HBAR_ST_HIDE);
		root_ctrl->root_para->data = MUSICPLAYER_NORMAL;
		root_ctrl->root_para->h_parent =  msg->h_deswin;               
		root_ctrl->h_app_music = app_music_create(root_ctrl->root_para);					

		GUI_WinSetFocusChild(root_ctrl->h_app_music);
		#endif
		
		return EPDK_OK;
	}				

	return EPDK_OK;
}

static void app_root_create_bg_music(__gui_msg_t *msg)
{
	H_WIN root, child;
	char  winname[128];  
	__bool exist = EPDK_FALSE;
	root_para_t *root_para;
	root_ctrl_t   *root_ctrl;

	root_ctrl = (root_ctrl_t   *)GUI_WinGetAddData(msg->h_deswin);
	root_para = root_ctrl->root_para;
	root  = GUI_WinGetHandFromName(APP_ROOT);	
	child = GUI_WinGetFirstChild(root);
	
	while( child != NULL )
	{
		eLIBs_memset(winname, 0, 128);
		GUI_WinGetName(child, winname);

		__msg("winname = %s \n", winname);

		if( 0 == eLIBs_strcmp( APP_MUSIC, winname))		
		{
			exist = 1;
			break;
		}

		child = GUI_WinGetNextBro(child);
	}

	if(EPDK_FALSE == exist)
	{        				
		root_para->data = MUSICPLAYER_BACKGROUND;
		root_ctrl->root_para->h_parent= GUI_WinGetHandFromName(APP_ROOT);                                       
		root_ctrl->h_app_music =  app_music_create(root_para);		
	}

	return;
}

static void app_root_delete_bg_music(__gui_msg_t *msg)
{
	H_WIN root, child, next_child;
	char  winname[128]; 
	root_para_t *root_para;
	root_ctrl_t   *root_ctrl;

	root_ctrl = (root_ctrl_t   *)GUI_WinGetAddData(msg->h_deswin);
	root_para = root_ctrl->root_para;

	root  = GUI_WinGetHandFromName(APP_ROOT);	
	if(!root)
	{
		__err("GUI_WinGetHandFromName fail...\n");
		return ;
	}
	child = GUI_WinGetFirstChild(root);

	while( child != NULL )
	{
		eLIBs_memset(winname, 0, 128);
		GUI_WinGetName(child, winname);

		next_child = GUI_WinGetNextBro(child);

		__msg("winname = %s \n", winname);

		if( 0 == eLIBs_strcmp( APP_MUSIC, winname))		
		{
			GUI_ManWinDelete(child);
			root_ctrl->h_app_music = NULL;
			break;
		}
		child = next_child;
	} 	
	
	return;
}

static __s32 app_root_is_bg_music_exist(__gui_msg_t *msg, H_WIN* hMusic)  //CAI++
{
	H_WIN root, child;
	char  winname[256];  
	__bool exist;
	root  = GUI_WinGetHandFromName(APP_ROOT);	
	child = GUI_WinGetFirstChild(root);
	exist = 0;
	*hMusic = 0;
	while( child != NULL )
	{
		eLIBs_memset(winname, 0, 256);
		GUI_WinGetName(child, winname);
		__msg("winname = %s \n", winname);
		if( 0 == eLIBs_strcmp( APP_MUSIC, winname))		
		{
			*hMusic = child;
			exist = 1;
			break;
		}
		child = GUI_WinGetNextBro(child);
	}
	return exist;
}

static __s32 cmd_mainmenu_app_switch(__gui_msg_t *msg)
{
	root_para_t *root_para;
	root_ctrl_t   *root_ctrl;

	root_ctrl = (root_ctrl_t   *)GUI_WinGetAddData(msg->h_deswin);
	root_para = root_ctrl->root_para;

	__here__;
	switch(HIWORD(msg->dwAddData1))
	{
		case SWITCH_TO_OTHER_APP:
		{
			switch(msg->dwAddData2)
			{										
				case ID_MAINMENU_MUSIC:
				{       
					if(root_ctrl->root_para->root_type == RAT_UNKNOWN)
					{
						gscene_hbar_current_disk(0xff,0xff);//no disk exist

						app_root_show_dlg(root_ctrl->root_para->h_parent,STRING_NO_DISK, 0);
						
						GUI_WinSetFocusChild(root_ctrl->h_app_mainmenu);
					}
					else
					{
						__s32 exist;
						H_WIN hmusic;
						if(-1 == cai_check_mediafile_esist(root_para->root_type, msg->dwAddData2))
						{
							GUI_WinSetFocusChild(root_ctrl->h_app_mainmenu);
							break;
						}
						exist = app_root_is_bg_music_exist(msg,&hmusic);
						
						gscene_hbar_set_state(HBAR_ST_HIDE);
						GUI_LyrWinSetSta(root_ctrl->lyr_app_mainmenu, GUI_LYRWIN_STA_SUSPEND);


						root_para->explorer_play_music = 0;
						root_para->root_type = root_ctrl->root_para->root_type;                     
						root_para->data = MUSICPLAYER_NORMAL;
						root_ctrl->root_para->h_parent= msg->h_deswin;
						if(exist)
							app_music_restore(root_ctrl->h_app_music, root_ctrl->root_para);
						else
							root_ctrl->h_app_music =  app_music_create(root_para);
						
						GUI_WinSetFocusChild(root_ctrl->h_app_music);
					}
					break;
				}

				case ID_MAINMENU_MOVIE:
				{   
					if(root_ctrl->root_para->root_type == RAT_UNKNOWN)
					{
						gscene_hbar_current_disk(0xff,0xff);//no disk exist

						app_root_show_dlg(root_ctrl->root_para->h_parent,STRING_NO_DISK, 0);

						GUI_WinSetFocusChild(root_ctrl->h_app_mainmenu);
					}
					else
					{
						if(-1 == cai_check_mediafile_esist(root_para->root_type, msg->dwAddData2))
						{
							GUI_WinSetFocusChild(root_ctrl->h_app_mainmenu);
							break;
						}
						gscene_hbar_set_state(HBAR_ST_HIDE);
						GUI_LyrWinSetSta(root_ctrl->lyr_app_mainmenu, GUI_LYRWIN_STA_SUSPEND);
						
						//__msg("----movie explorer =%d\n", root_ctrl->root_para->root_type);
						root_para->root_type = root_ctrl->root_para->root_type;
						root_para->explr_root = root_ctrl->root_para->root_type;
						root_para->data = ID_EXPLORER_MOVIE;
						root_ctrl->root_para->h_parent= msg->h_deswin;
						root_ctrl->h_app_explorer =  app_explorer_create(root_para);

						GUI_WinSetFocusChild(root_ctrl->h_app_explorer);
					}
					break;							
				}
				
				case ID_MAINMENU_PHOTO:
				{               
					if(root_ctrl->root_para->root_type == RAT_UNKNOWN)
					{
						gscene_hbar_current_disk(0xff,0xff);//no disk exist

						app_root_show_dlg(root_ctrl->root_para->h_parent,STRING_NO_DISK, 0);

						GUI_WinSetFocusChild(root_ctrl->h_app_mainmenu);
					}
					else
					{
						if(-1 == cai_check_mediafile_esist(root_para->root_type, msg->dwAddData2))
						{
							GUI_WinSetFocusChild(root_ctrl->h_app_mainmenu);
							break;
						}
						gscene_hbar_set_state(HBAR_ST_HIDE);
						GUI_LyrWinSetSta(root_ctrl->lyr_app_mainmenu, GUI_LYRWIN_STA_SUSPEND);


						root_para->root_type = root_ctrl->root_para->root_type;
						root_para->explr_root = root_ctrl->root_para->root_type;
						root_para->data = ID_EXPLORER_PHOTO;
						root_ctrl->root_para->h_parent= msg->h_deswin;
						root_ctrl->h_app_explorer =  app_explorer_create(root_para);

						GUI_WinSetFocusChild(root_ctrl->h_app_explorer);
					}
					break;
				}
				case ID_MAINMENU_FM:
				{                          
					H_WIN hmusic;

					if(app_root_is_bg_music_exist(msg,&hmusic))
					{
						app_root_delete_bg_music(msg);
					}
#if BEETLES_FM_HEADBAR
					gscene_hbar_set_state(HBAR_ST_HIDE);
#endif
					GUI_LyrWinSetSta(root_ctrl->lyr_app_mainmenu, GUI_LYRWIN_STA_SUSPEND);

					dsk_speaker_onoff(BEETLES_SPEAK_ON);
					//__msg("----fm\n");
					root_ctrl->root_para->h_parent = msg->h_deswin;
					root_ctrl->h_app_fm =  app_fm_create(root_para);

					GUI_WinSetFocusChild(root_ctrl->h_app_fm);
					
					break;                          
				}
				
				case ID_MAINMENU_EBOOK:
				{                 
					if(root_ctrl->root_para->root_type == RAT_UNKNOWN)
					{
						gscene_hbar_current_disk(0xff,0xff);//no disk exist

						app_root_show_dlg(root_ctrl->root_para->h_parent,STRING_NO_DISK, 0);

						GUI_WinSetFocusChild(root_ctrl->h_app_mainmenu);
					}
					else
					{
						if(-1 == cai_check_mediafile_esist(root_para->root_type, msg->dwAddData2))
						{
							GUI_WinSetFocusChild(root_ctrl->h_app_mainmenu);
							break;
						}
		                            gscene_hbar_set_state(HBAR_ST_HIDE);
						GUI_LyrWinSetSta(root_ctrl->lyr_app_mainmenu, GUI_LYRWIN_STA_SUSPEND);

						//__msg("----enter ebook =%d\n", root_ctrl->root_para->root_type);
						root_para->root_type = root_ctrl->root_para->root_type;
						root_para->explr_root = root_ctrl->root_para->root_type;
						root_para->data = ID_EXPLORER_EBOOK;
						root_ctrl->root_para->h_parent= msg->h_deswin;
						root_ctrl->h_app_explorer=  app_explorer_create(root_para);

						GUI_WinSetFocusChild(root_ctrl->h_app_explorer);
					}
					break;
				}
				
#if EXTERN_KARAOKE_EXIST
				case ID_MAINMENU_KARAOKE:
				{	
					if(root_ctrl->root_para->root_type == RAT_UNKNOWN)
					{
						gscene_hbar_current_disk(0xff,0xff);//no disk exist

						app_root_show_dlg(root_ctrl->root_para->h_parent,STRING_NO_DISK, 0);

						GUI_WinSetFocusChild(root_ctrl->h_app_mainmenu);
					}
					else
					{
						if(-1 == cai_check_mediafile_esist(root_para->root_type, msg->dwAddData2))
						{
							GUI_WinSetFocusChild(root_ctrl->h_app_mainmenu);
							break;
						}
						gscene_hbar_set_state(HBAR_ST_HIDE);
						GUI_LyrWinSetSta(root_ctrl->lyr_app_mainmenu, GUI_LYRWIN_STA_SUSPEND);

						//__msg("----enter karaoke =%d\n", root_ctrl->root_para->root_type);
						root_para->root_type = root_ctrl->root_para->root_type;//RAT_TF;
						root_para->explr_root = root_ctrl->root_para->root_type;
						root_para->data = ID_EXPLORER_KARAOKE;
						root_ctrl->root_para->h_parent= msg->h_deswin;
						root_ctrl->h_app_explorer =  app_explorer_create(root_para);

						GUI_WinSetFocusChild(root_ctrl->h_app_explorer);
					}
					
					break;							
				}
#endif
				case ID_MAINMENU_EXPLORER:
				{
					if(root_ctrl->root_para->root_type == RAT_UNKNOWN)
					{
						gscene_hbar_current_disk(0xff,0xff);//no disk exist

						app_root_show_dlg(root_ctrl->root_para->h_parent,STRING_NO_DISK, 0);

						GUI_WinSetFocusChild(root_ctrl->h_app_mainmenu);
					}
					else
					{
						gscene_hbar_set_state(HBAR_ST_HIDE);
						GUI_LyrWinSetSta(root_ctrl->lyr_app_mainmenu, GUI_LYRWIN_STA_SUSPEND);

						//__msg("----all explorer=%d\n", root_ctrl->root_para->root_type);
						root_para->root_type = root_ctrl->root_para->root_type;
						root_para->explr_root = root_ctrl->root_para->root_type;
						root_para->data = ID_EXPLORER_ALL;
						root_ctrl->root_para->h_parent= msg->h_deswin;
						root_ctrl->h_app_explorer =  app_explorer_create(root_para);

						GUI_WinSetFocusChild(root_ctrl->h_app_explorer);
					}
					break;
				}

				case ID_MAINMENU_SETTING:
				{
					gscene_hbar_set_state(HBAR_ST_HIDE);
					GUI_LyrWinSetSta(root_ctrl->lyr_app_mainmenu, GUI_LYRWIN_STA_SUSPEND);


					root_ctrl->root_para->setting_opt = 0;
					root_ctrl->root_para->h_parent= msg->h_deswin;
					root_ctrl->h_app_setting=  app_setting_create(root_para);

					GUI_WinSetFocusChild(root_ctrl->h_app_setting);

					break;
				}

            #if 0
				case ID_MAINMENU_RECORD:
				{
					if(root_ctrl->root_para->root_type == RAT_UNKNOWN)
					{
						gscene_hbar_current_disk(0xff,0xff);//no disk exist
						GUI_WinSetFocusChild(root_ctrl->h_app_mainmenu);
					}
					else
					{
#if BEETLES_MICIN_DISABLE
						if(!dsk_audio_echo_is_open())
						{
							gscene_hbar_current_disk(0xe,0xe);//insert mic
							GUI_WinSetFocusChild(root_ctrl->h_app_mainmenu);
						}
						else
#endif						
						{
							H_WIN hmusic;

							if(app_root_is_bg_music_exist(msg,&hmusic))
							{
								app_root_delete_bg_music(msg);
							}

							gscene_hbar_set_state(HBAR_ST_HIDE);
							GUI_LyrWinSetSta(root_ctrl->lyr_app_mainmenu, GUI_LYRWIN_STA_SUSPEND);

							//James++ disable back music play
							//__msg("----enter ebook =%d\n", root_ctrl->root_para->root_type);
							root_ctrl->root_para->root_type = root_ctrl->root_para->root_type;
							root_ctrl->root_para->record_opt = 0;
							root_ctrl->root_para->h_parent= msg->h_deswin;
							root_ctrl->h_app_record=  app_record_create(root_para);

							GUI_WinSetFocusChild(root_ctrl->h_app_record);
						}
					}
					break;
				}
            #endif
            
				case ID_MAINMENU_LINEIN:
				{
					break;
				}
				
				case ID_MAINMENU_TV:
				{
					H_WIN hmusic;
					#if ANTENNA_MODE_SELECT
						delete_all_app(root_ctrl);
						app_root_antenna_select(msg);
					#else
					{
						if(app_root_is_bg_music_exist(msg,&hmusic))
						{
							app_root_delete_bg_music(msg);
						}
						
						gscene_hbar_set_state(HBAR_ST_HIDE);
						GUI_LyrWinSetSta(root_ctrl->lyr_app_mainmenu, GUI_LYRWIN_STA_SUSPEND);
						
						dsk_speaker_onoff(BEETLES_SPEAK_ON);
						__here__;
						
						root_ctrl->root_para->h_parent = msg->h_deswin;
						root_ctrl->h_app_tv = app_dtv_create(root_para);
						
						GUI_WinSetFocusChild(root_ctrl->h_app_tv);

						break;
					}
					#endif

					break;
				}				

				case ID_MAINMENU_MUTE:
				{
					__here__;
					if(dsk_volume_is_on())
					{
						dsk_volume_off();
					}	
					else
						dsk_volume_on();
					
					break;
				}

				case ID_MAINMENU_BACKGROUND:
				{
					__s32	bg_index;
					bg_index = gscene_bgd_get_default_bg_index();

					//background pic  2
					if(bg_index == 1)
						bg_index = 0;
					else
						bg_index++;
					
					gscene_bgd_set_default_bg_index(bg_index);
					dsk_reg_flush();

					break;
				}
				
				case ID_MAINMENU_TFUSB:
				{
					if(root_ctrl->root_para->root_type_cnt == 2)
					{
						if(root_ctrl->root_para->root_type == RAT_USB)
							root_ctrl->root_para->root_type = RAT_TF;
						else
							root_ctrl->root_para->root_type = RAT_USB;

						gscene_hbar_current_disk(root_ctrl->root_para->root_type,0x00000203);
					}
					break;
				}
				
				case ID_MAINMENU_CLOSESCN:
				{
					break;
				}
				
				case ID_MAINMENU_POWEROFF:
				{
					__here__;
					{
						__gui_msg_t msg;

						msg.id 			= DSK_MSG_POWER_OFF;
						msg.h_srcwin 		= NULL;
						msg.h_deswin		= GUI_WinGetHandFromName("init");
						msg.dwAddData1	= (__u32)0;
						msg.dwAddData2	= (__u32)0;
						
						GUI_SendNotifyMessage(&msg);
						
					}
					break;
				}

				case ID_MAINMENU_RANDOM:
				{
					__here__;
					{
						__gui_msg_t 		ntymsg;

						ntymsg.id			=  DSK_MSG_MENU_RANDOM;
						ntymsg.h_srcwin 		=  msg->h_deswin;
						ntymsg.h_deswin 		=  root_ctrl->h_app_mainmenu; 
						ntymsg.dwAddData1	=  NULL;
						ntymsg.dwAddData2	=  NULL;
						
						GUI_SendNotifyMessage(&ntymsg); 
					}
					break;
				}
				
				default:
					break;
			}
			break;
		}
			
		default:
			break;
	}

	return 0;
}

static __s32 cmd_explorer_app_switch(__gui_msg_t *msg)
{
	root_para_t *root_para;
	root_ctrl_t   *root_ctrl;

	root_ctrl = (root_ctrl_t   *)GUI_WinGetAddData(msg->h_deswin);
	root_para = root_ctrl->root_para;

	__here__;
	switch(HIWORD(msg->dwAddData1))
	{
		case SWITCH_TO_OTHER_APP:
		{
			switch( msg->dwAddData2 )
			{
				case EXPLR_SW_TO_MAIN:
				{                            
					__msg("----EXPLR_SW_TO_MAIN\n");
					
					if(root_ctrl->h_app_explorer)
					{
						GUI_ManWinDelete(root_ctrl->h_app_explorer);
						root_ctrl->h_app_explorer = 0;
					}
					app_home_restore(root_ctrl);
					
					break;
				}
				
				case EXPLR_SW_TO_MOVIE:
				{                            
					H_WIN hmusic;

					if(app_root_is_bg_music_exist(msg,&hmusic))
					{
						app_root_delete_bg_music(msg);
					}

					__msg("----EXPLR_SW_TO_MOVIE\n");
					//dsk_speaker_onoff(BEETLES_SPEAK_ON);
					//root_para->root_type = msg->dwReserved;
					root_para->data = 0;
					root_para->root_type = root_para->explr_root  ;
					root_ctrl->h_app_movie = app_movie_create(root_para);
					GUI_WinSetFocusChild(root_ctrl->h_app_movie);
					
					break;
				}
				
				case EXPLR_SW_TO_PHOTO:
				{
					__msg("----EXPLR_SW_TO_PHOTO\n");

					//root_para->root_type = msg->dwReserved;
					root_para->data = 0;
					root_para->root_type = root_para->explr_root  ;
					root_ctrl->h_app_photo = app_photo_create(root_para);
					GUI_WinSetFocusChild(root_ctrl->h_app_photo);
					
					break;
				}
				
				case EXPLR_SW_TO_MUSIC:
				{       
					H_WIN hmusic;

					if(app_root_is_bg_music_exist(msg,&hmusic))
					{
						app_root_delete_bg_music(msg);
					}

					__msg("----EXPLR_SW_TO_MUSIC\n");
					
					root_para->explorer_play_music = 1;
					root_para->root_type = root_para->explr_root  ;     
					root_para->data = MUSICPLAYER_NORMAL;
					root_ctrl->root_para->h_parent= msg->h_deswin;                                                      
					root_ctrl->h_app_music =  app_music_create(root_para);							    
					GUI_WinSetFocusChild(root_ctrl->h_app_music);                                                       

					break ;
				}
				
				case EXPLR_SW_TO_EBOOK:
				{                            
					__msg("----EXPLR_SW_TO_EBOOK\n");

					//root_para->root_type = msg->dwReserved;
					root_para->data = 0;
					root_para->root_type = root_para->explr_root  ;   
					root_ctrl->h_app_ebook= app_ebook_create(root_para);
					GUI_WinSetFocusChild(root_ctrl->h_app_ebook);
					
					break;
				}						

#if EXTERN_KARAOKE_EXIST
				case EXPLR_SW_TO_KARAOKE:
				{							 
					__msg("----EXPLR_SW_TO_KARAOKE =%d\n", msg->dwReserved);
					
					root_para->explr_root = msg->dwReserved; 
					root_para->root_type = msg->dwReserved;
					root_para->data = 0;
					root_ctrl->h_app_karaoke = app_karaoke_create(root_para);
					if(root_ctrl->h_app_karaoke)
					{
						__here__;
						GUI_WinSetFocusChild(root_ctrl->h_app_karaoke); 						
					}
					else
					{
						__here__;
						app_home_restore(root_ctrl);
					}
					
					break;
				}
#endif

				default:
					break;
			}
			root_ctrl->h_app_explorer = NULL;////????
			
			break;
		}
		
		default:
			break;
	}

	return 0;
}

static __s32 cmd_movie_app_switch(__gui_msg_t *msg)
{
	root_para_t *root_para;
	root_ctrl_t   *root_ctrl;

	root_ctrl = (root_ctrl_t   *)GUI_WinGetAddData(msg->h_deswin);
	root_para = root_ctrl->root_para;

	if(msg->dwAddData2 == 1)  //CAI++ for tv2lcd
	{
		app_root_fs_plugin_restore(msg);
	}
	__here__;
	switch(HIWORD(msg->dwAddData1))
	{
		case SWITCH_TO_MMENU:
		{
			__msg("----SWITCH_TO_MMENU\n");
			GUI_ManWinDelete(root_ctrl->h_app_movie);
			root_ctrl->h_app_movie = 0;
			app_home_restore(root_ctrl);

			break;
		}

		case SWITCH_TO_EXPLORER:
		{
			__msg("----SWITCH_TO_EXPLORER\n");
			GUI_ManWinDelete(root_ctrl->h_app_movie);
			root_ctrl->h_app_movie = 0;
			root_para->root_type = root_para->explr_root;
			root_para->data = ID_EXPLORER_MOVIE;
			root_ctrl->root_para->h_parent= msg->h_deswin;
			root_ctrl->h_app_explorer =  app_explorer_create(root_para);
			GUI_WinSetFocusChild(root_ctrl->h_app_explorer);
			
			break;
		}
				
		default:
			break;
	}

	return 0;
}

static __s32 cmd_music_app_switch(__gui_msg_t *msg)
{
	root_para_t *root_para;
	root_ctrl_t   *root_ctrl;

	root_ctrl = (root_ctrl_t   *)GUI_WinGetAddData(msg->h_deswin);
	root_para = root_ctrl->root_para;

	__here__;
	switch(HIWORD(msg->dwAddData1))
	{
		case SWITCH_TO_MMENU:
		{
			__msg("----SWITCH_TO_MMENU\n");
			if(MUSICPLAYER_BACKGROUND == msg->dwReserved)
			{
				__here__;
				//GUI_ManWinDelete(root_ctrl->h_app_music);////
				//root_ctrl->h_app_music = 0;////
			}
			else
			{
				__here__;
				GUI_ManWinDelete(root_ctrl->h_app_music);
				root_ctrl->h_app_music = 0;                                  
			}   
			app_home_restore(root_ctrl);

			break;
		}
		
		case CLOSE_BG_MUSIC:
		{
			__msg("----CLOSE_BG_MUSIC\n");
			if(root_ctrl->h_app_music)
			{
				GUI_ManWinDelete(root_ctrl->h_app_music);
				root_ctrl->h_app_music = 0;  
			}
			
			break;
		}
		
		default:
			break;
	}

	return 0;
}

static __s32 cmd_photo_app_switch(__gui_msg_t *msg)
{
	root_para_t *root_para;
	root_ctrl_t   *root_ctrl;

	root_ctrl = (root_ctrl_t   *)GUI_WinGetAddData(msg->h_deswin);
	root_para = root_ctrl->root_para;

	__here__;
	switch(HIWORD(msg->dwAddData1))
	{
		case SWITCH_TO_MMENU:
		{
			H_WIN hmusic;

			if(app_root_is_bg_music_exist(msg,&hmusic))
			{
				app_root_delete_bg_music(msg);
			}
			__msg("----SWITCH_TO_MMENU\n");

			GUI_ManWinDelete(root_ctrl->h_app_photo);
			root_ctrl->h_app_photo= 0;
			app_home_restore(root_ctrl);
			GUI_WinSetFocusChild(root_ctrl->h_app_mainmenu);
			
			break;
		}

		case SWITCH_TO_EXPLORER:
		{
			H_WIN hmusic;
			if(app_root_is_bg_music_exist(msg,&hmusic))
			{
				app_root_delete_bg_music(msg);
			}
			__msg("----SWITCH_TO_EXPLORER\n");

			GUI_ManWinDelete(root_ctrl->h_app_photo);
			root_ctrl->h_app_photo = 0;
			gscene_bgd_set_flag(EPDK_TRUE);
			root_para->root_type = root_para->explr_root;
			root_para->data = ID_EXPLORER_PHOTO;
			root_ctrl->root_para->h_parent= msg->h_deswin;
			root_ctrl->h_app_explorer =  app_explorer_create(root_para);
			GUI_WinSetFocusChild(root_ctrl->h_app_explorer);
			
			break;
		}
		
		case OPEN_BG_MUSIC:
		{
			__msg("----OPEN_BG_MUSIC\n");
			
			app_root_create_bg_music(msg);
			
			break;
		}
		
		case CLOSE_BG_MUSIC:
		{
			__msg("----CLOSE_BG_MUSIC\n");
			
			app_root_delete_bg_music(msg);
			
			break;
		}
				
		default:
			break;
	}

	return 0;
}

static __s32 cmd_ebook_app_switch(__gui_msg_t *msg)
{
	root_para_t *root_para;
	root_ctrl_t   *root_ctrl;

	root_ctrl = (root_ctrl_t   *)GUI_WinGetAddData(msg->h_deswin);
	root_para = root_ctrl->root_para;

	__here__;
	switch(HIWORD(msg->dwAddData1))
	{
		case SWITCH_TO_MMENU:
		{
			H_WIN hmusic;
			if(app_root_is_bg_music_exist(msg,&hmusic))
			{
				app_root_delete_bg_music(msg);
			}
			__msg("----SWITCH_TO_MMENU\n");
			GUI_ManWinDelete(root_ctrl->h_app_ebook);
			root_ctrl->h_app_ebook = 0;
			app_home_restore(root_ctrl);

			break;
		}

		case SWITCH_TO_EXPLORER:
		{
			H_WIN hmusic;
			if(app_root_is_bg_music_exist(msg,&hmusic))
			{
				app_root_delete_bg_music(msg);
			}
			__msg("----SWITCH_TO_EXPLORER\n");

			GUI_ManWinDelete(root_ctrl->h_app_ebook);
			root_ctrl->h_app_ebook = 0;

			root_para->root_type = root_para->explr_root;
			root_para->data = ID_EXPLORER_EBOOK;
			root_ctrl->h_app_explorer =  app_explorer_create(root_para);
			GUI_WinSetFocusChild(root_ctrl->h_app_explorer);
			
			break;
		}
		
		case OPEN_BG_MUSIC:
		{
			__msg("----OPEN_BG_MUSIC\n");
			app_root_create_bg_music(msg);
			
			break;
		}
		
		case CLOSE_BG_MUSIC:
		{
			__msg("----CLOSE_BG_MUSIC\n");
			app_root_delete_bg_music(msg);
			
			break;
		}
				
		default:
			break;
	}

	return 0;
}

static __s32 cmd_fm_app_switch(__gui_msg_t *msg)
{
	root_para_t *root_para;
	root_ctrl_t   *root_ctrl;

	root_ctrl = (root_ctrl_t   *)GUI_WinGetAddData(msg->h_deswin);
	root_para = root_ctrl->root_para;

	__here__;
	switch(HIWORD(msg->dwAddData1))
	{
		case SWITCH_TO_MMENU:
		{
			__msg("----SWITCH_TO_MMENU\n");

			GUI_ManWinDelete(root_ctrl->h_app_fm);
			root_ctrl->h_app_fm = 0;
			app_home_restore(root_ctrl);

#if BEETLES_KEYTONE_ENABLE
			//keytone reinit
			{
				__set_keytone_t state;

				dsk_keytone_get_state(&state);
				dsk_keytone_exit();
				dsk_keytone_init(BEETLES_APP_ROOT"res\\sounds\\chord.wav");                    
				dsk_keytone_set_state(state);
			}
#endif
			break;
		}
		case SWITCH_TO_OTHER_APP:
		{
			__here__;
			switch(LOWORD(msg->dwAddData2))
			{
				case FM_SW_TO_RECORD:
				{
					__msg("----FM_SW_TO_RECORD\n");
					
					break;
				}
				
				default:
					break;
			}
		}
		
		default:
			break;
	}

	return 0;
}

static __s32 cmd_record_app_switch(__gui_msg_t *msg)
{
	root_para_t *root_para;
	root_ctrl_t   *root_ctrl;

	root_ctrl = (root_ctrl_t   *)GUI_WinGetAddData(msg->h_deswin);
	root_para = root_ctrl->root_para;

	__here__;
	switch(HIWORD(msg->dwAddData1))
	{
		case SWITCH_TO_MMENU:
		{
			__msg("----SWITCH_TO_MMENU\n");
			
			GUI_ManWinDelete(root_ctrl->h_app_record);
			root_ctrl->h_app_record = 0;
			app_home_restore(root_ctrl);

			break;
		}
		
		case SWITCH_TO_OTHER_APP:
		{
			__here__;
			switch(LOWORD(msg->dwAddData2))
			{
				case RECORD_SW_TO_FM:
				{
					__msg("----RECORD_SW_TO_FM\n");
					dsk_speaker_onoff(BEETLES_SPEAK_ON);
					root_ctrl->root_para->h_parent= GUI_WinGetParent(root_ctrl->h_app_record);
					GUI_ManWinDelete(root_ctrl->h_app_record);
					root_ctrl->h_app_record = 0;
					root_ctrl->h_app_fm = app_fm_create(root_para);
					GUI_WinSetFocusChild(root_ctrl->h_app_fm);
					
					break;
				}
				
				default:
					break;
			}                    
		}
		
		default:
			break;
	}

	return 0;
}

static __s32 cmd_setting_app_switch(__gui_msg_t *msg)
{
	root_para_t *root_para;
	root_ctrl_t   *root_ctrl;

	root_ctrl = (root_ctrl_t   *)GUI_WinGetAddData(msg->h_deswin);
	root_para = root_ctrl->root_para;

	__here__;
	switch(HIWORD(msg->dwAddData1))
	{
		case SWITCH_TO_OTHER_APP:
		{
			switch(msg->dwAddData2)
			{
				case SETTING_SW_TO_MAIN:
				{
					__msg("----SETTING_SW_TO_MAIN\n");
					
					GUI_ManWinDelete(root_ctrl->h_app_setting);
					root_ctrl->h_app_setting= 0;
					
					if(root_para->lan_type != dsk_langres_get_type())
					{
						root_para->lan_type = dsk_langres_get_type();
						{
							__gui_msg_t 		ntymsg;
							ntymsg.id			=  DSK_MSG_LAN_CHANGE;
							ntymsg.h_srcwin 		=  msg->h_deswin;
							ntymsg.h_deswin 		=  root_ctrl->h_app_mainmenu; 
							ntymsg.dwAddData1	=  NULL;
							ntymsg.dwAddData2	=  NULL;
							GUI_SendMessage(&ntymsg); 
						}
					}
					app_home_restore(root_ctrl);
					
					break;
				}
				
				case SETTING_SW_TO_MULTIMEDIA:
				case SETTING_SW_TO_PROMPT_UPGRADE:
				case SETTING_SW_TO_PROMPT_DEFAULT:
				case EXPLR_SW_TO_UPGRADE:
					break;
				
				default:
					break;
			}
			root_ctrl->h_app_setting = NULL;
			
			break;
		}
		
		default:
			break;
	}

	return 0;
}

static __s32 cmd_linein_app_switch(__gui_msg_t *msg)
{
	__here__;
	switch(HIWORD(msg->dwAddData1))
	{
                case SWITCH_TO_MMENU:
                case SWITCH_TO_LINE_IN:
                default:
                    break;
	}

	return 0;
}

static __s32 cmd_tv_app_switch(__gui_msg_t *msg)
{
	root_para_t *root_para;
	root_ctrl_t   *root_ctrl;

	root_ctrl = (root_ctrl_t   *)GUI_WinGetAddData(msg->h_deswin);
	root_para = root_ctrl->root_para;

	__here__;
	switch(HIWORD(msg->dwAddData1))
	{
		case SWITCH_TO_MMENU:
		{
			__msg("----SWITCH_TO_MMENU\n");
			
			GUI_ManWinDelete(root_ctrl->h_app_tv);
			root_ctrl->h_app_tv = 0;
			app_home_restore(root_ctrl);
			break;
		}
		
		case SWITCH_TO_OTHER_APP:
		{
			__here__;
		}
		
		default:
			break;
	}

	return 0;
}


#if EXTERN_KARAOKE_EXIST
static __s32 cmd_karaoke_app_switch(__gui_msg_t *msg)
{
	root_para_t *root_para;
	root_ctrl_t   *root_ctrl;

	root_ctrl = (root_ctrl_t   *)GUI_WinGetAddData(msg->h_deswin);
	root_para = root_ctrl->root_para;

	__here__;
	switch(HIWORD(msg->dwAddData1))
	{
		case SWITCH_TO_EXPLORER:
		{
			reg_root_para_t *root_kak_para = NULL;
			root_kak_para = dsk_reg_get_para_by_app(REG_APP_ROOT);

			__msg("----SWITCH_TO_EXPLORER\n");
			__msg("----last_krk_index_ud=%d\n",root_kak_para->last_krk_index_ud);
			GUI_ManWinDelete(root_ctrl->h_app_karaoke);
			gscene_bgd_set_state(BGD_STATUS_HIDE);
			root_ctrl->h_app_karaoke = 0;
			root_para->root_type = root_para->explr_root;
			if(root_para->root_type == RAT_TF)
			{
				__here__;
				root_kak_para->last_krk_index_sd =msg->dwAddData2;
			}
			else if(root_para->root_type == RAT_USB)
			{
				__here__;
				root_kak_para->last_krk_index_ud =msg->dwAddData2;
			}
			
			root_para->data = ID_EXPLORER_KARAOKE;
			root_ctrl->root_para->h_parent = msg->h_deswin;
			root_ctrl->h_app_explorer =  app_explorer_create(root_para);
			GUI_WinSetFocusChild(root_ctrl->h_app_explorer);
			
			break;
		}
		
		case SWITCH_TO_MMENU:
		{
			__msg("----SWITCH_TO_MMENU\n");
			
			GUI_ManWinDelete(root_ctrl->h_app_karaoke);
			root_ctrl->h_app_karaoke= 0;
			app_home_restore(root_ctrl);

			break;
		}
		
		default:
			break;
	}

	return 0;
}
#endif

#if  ANTENNA_MODE_SELECT
static __s32 app_root_antenna_select(__gui_msg_t *msg)
{
	root_para_t *root_para;
	root_ctrl_t   *root_ctrl;

	root_ctrl = (root_ctrl_t   *)GUI_WinGetAddData(msg->h_deswin);
	root_para = root_ctrl->root_para;


	if(root_ctrl->h_antenna_select_dialog == NULL)
	{
		//������hbar  ����
		default_dialog(root_ctrl->h_antenna_select_dialog, msg->h_deswin, APP_ANTENNA_SELECT_ID,ADLG_EXTERNAL, NULL);
	}
	else
	{
		__msg("nothing to do --->\n");
	}

		return EPDK_OK;	
}

static __s32	app_root_eixt_antenna_select(__gui_msg_t *msg)
{
	root_para_t *root_para;
	root_ctrl_t   *root_ctrl;
	__gui_msg_t notify_msg;

	root_ctrl = (root_ctrl_t   *)GUI_WinGetAddData(msg->h_deswin);
	root_para = root_ctrl->root_para;

	if(root_ctrl->h_antenna_select_dialog)
	{
		app_dialog_destroy(root_ctrl->h_antenna_select_dialog);
		root_ctrl->h_antenna_select_dialog = NULL;
		app_home_restore(root_ctrl);		
	}

	return EPDK_OK;
}

static __s32 cmd_antenna_select_app_switch(__gui_msg_t *msg)
{
	root_para_t *root_para;
	root_ctrl_t   *root_ctrl;

	root_ctrl = (root_ctrl_t   *)GUI_WinGetAddData(msg->h_deswin);
	root_para = root_ctrl->root_para;

	__here__;
	switch(HIWORD(msg->dwAddData1))
	{
		case ADLG_CMD_EXIT:
		{
			if( ADLG_EXTERNAL_ANTENNA == msg->dwAddData2 )	// ѡ��ϵ��Ķ�
			{
				//��app_root_scene������Ӧ
				app_dialog_destroy(root_ctrl->h_antenna_select_dialog);
				root_ctrl->h_antenna_select_dialog = NULL;

				pull_down_gpio(RF_SW_PORT , RF_SW_PORTNUM);

				key2app(msg,KEY_TO_TV);						
			}
			else if ( ADLG_TELESCOPIC_ANTENNA == msg->dwAddData2 )	// ѡ��ӵ�һҳ��ʼ�Ķ�
			{
				//��app_root_scene������Ӧ
				app_dialog_destroy(root_ctrl->h_antenna_select_dialog);
				root_ctrl->h_antenna_select_dialog = NULL;
				
				pull_up_gpio(RF_SW_PORT , RF_SW_PORTNUM);

				key2app(msg,KEY_TO_TV);
			}
			else
			{
				//�˳��Ի���
				app_root_eixt_antenna_select(msg);
			}
		}
		break;
		default:
			break;
	}

	return 0;
}
#endif

static __s32 app_root_command_proc(__gui_msg_t *msg)
{
	__here__;
	switch (LOWORD(msg->dwAddData1))
	{		
#if ANTENNA_MODE_SELECT
		case APP_ANTENNA_SELECT_ID:
		{
			cmd_antenna_select_app_switch(msg);
			break;
		}
		break;
#endif	
	
		case APP_MAINMENU_ID:
		{
			cmd_mainmenu_app_switch(msg);
			break;
		}
		
		case APP_EXPLORER_ID:
		{
			cmd_explorer_app_switch(msg);
			break;
		}
		
		case APP_MOVIE_ID:
		{
			cmd_movie_app_switch(msg);
			break;
		}
		
		case APP_MUSIC_ID:
		{
			cmd_music_app_switch(msg);
			break;
		}
				
		case APP_PHOTO_ID:
		{
			cmd_photo_app_switch(msg);
			break;
		}
		case APP_EBOOK_ID:
		{
			cmd_ebook_app_switch(msg);
			break;
		}
				
		case APP_FM_ID:
		{
			cmd_fm_app_switch(msg);
			break;
		}
		
	        case APP_RECORD_ID:
		{
			cmd_record_app_switch(msg);
			break;
		}

	        case APP_CALENDAR_ID:
		{
			break;
		}

		case APP_SETTING_ID:
		{
			cmd_setting_app_switch(msg);
			break;
		}

		case APP_LINEIN_ID:
		{
			cmd_linein_app_switch(msg);
			break;
		}

		case APP_DTV_ID:
		{
			cmd_tv_app_switch(msg);
			break;
		}
		
#if EXTERN_KARAOKE_EXIST
		case APP_KARAOKE_ID:
		{
			cmd_karaoke_app_switch(msg);
			break;
		}
#endif
		default:
			break;
	}

	return EPDK_OK;
}

static __s32 app_root_broadcast_msg(__gui_msg_t *msg)
{
	H_WIN root, child;
	char  winname[128];  
	__gui_msg_t set_msg;

	eLIBs_memcpy(&set_msg, msg, sizeof(__gui_msg_t));

	root  = msg->h_deswin;	
	child = GUI_WinGetFirstChild(root);

	while( child != NULL )
	{
		eLIBs_memset(winname, 0, 128);
		GUI_WinGetName(child, winname);

		__msg("----app_root_broadcast_msg =%d, = %s \n", msg->id, winname);

		set_msg.h_deswin = child;
		GUI_SendMessage(&set_msg);
		// GUI_SendNotifyMessage(&set_msg);
		__here__;

		child = GUI_WinGetNextBro(child);
	} 	

	return EPDK_OK;
}

static __s32 app_root_win_proc(__gui_msg_t *msg)
{
	switch(msg->id)
	{
		case GUI_MSG_CREATE:
		{			
			root_para_t *root_para;
			root_ctrl_t   *root_ctrl;				

			__here__;
			root_para = esMEMS_Balloc(sizeof(root_para_t));
			if(!root_para)
			{
				return EPDK_FAIL;
			}
			eLIBs_memset(root_para, 0, sizeof(root_para_t));
			///////////
			
			root_ctrl = esMEMS_Balloc(sizeof(root_ctrl_t));
			if(!root_ctrl)
			{
				esMEMS_Bfree(root_para, sizeof(root_para_t));
				return EPDK_FAIL;
			}
			eLIBs_memset(root_ctrl, 0, sizeof(root_ctrl_t));           
			/////////////
#if BEETLES_KEY_REDEF
			key_switch = 0;
			menu_key_delay = EPDK_TRUE;
#endif
			root_para->h_parent		= msg->h_deswin;
			root_para->font			= SWFFont;
			root_para->root_type		= 0;
			root_para->root_type_cnt	= 0;
			root_para->lan_type		= dsk_langres_get_type();
			root_ctrl->h_app_mainmenu	= app_mainmenu_create(root_para);
			root_ctrl->lyr_app_mainmenu = get_mainmenu_lyrwin(root_ctrl->h_app_mainmenu);
#if BEETLES_LINEIN_DETECT
			root_ctrl->globe_check.globe_thread_id =  esKRNL_TCreate( app_root_globe_thread, (root_ctrl), 0x800, KRNL_priolevel5);
			if( root_ctrl->globe_check.globe_thread_id == NULL )
			{
				__msg("esKRNL_TCreate  globe_thread_id fail! \n");
			}      			
#endif
			root_ctrl->root_para = root_para;
			GUI_WinSetAddData(msg->h_deswin, (__u32)root_ctrl);                       
			app_root_fs_plugin_restore(msg);
			///////////////
			gscene_bgd_set_state(BGD_STATUS_SHOW);
			gscene_bgd_set_bottom();
			gscene_hbar_set_state(HBAR_ST_SHOW);
			
			GUI_LyrWinSetSta(root_ctrl->lyr_app_mainmenu, GUI_LYRWIN_STA_ON);
			GUI_LyrWinSetTop(root_ctrl->lyr_app_mainmenu);
			GUI_WinSetFocusChild(root_ctrl->h_app_mainmenu);

			return EPDK_OK;
		}
		
		case GUI_MSG_CLOSE:
		{
			__here__;
			GUI_ManWinDelete(msg->h_deswin);

			return EPDK_OK;
		}
		
		case GUI_MSG_DESTROY:
		{
			root_para_t *root_para;
			root_ctrl_t   *root_ctrl;

            reg_system_para_t* para;
            para = (reg_system_para_t*)dsk_reg_get_para_by_app(REG_APP_SYSTEM);
            if(para)
            {
                para->volume = dsk_volume_get();
                dsk_reg_flush();
            }

			__here__;
#if BEETLES_KEY_REDEF
			key_switch = 0;
			menu_key_delay = EPDK_TRUE;
#endif
			root_ctrl = (root_ctrl_t *)GUI_WinGetAddData(msg->h_deswin);
			if (!root_ctrl)
			{
				__here__;
				return EPDK_OK;
			}

			root_para = (root_para_t *)root_ctrl->root_para;
			if (!root_para)
			{
				__here__;
				return EPDK_OK;
			}

#if BEETLES_LINEIN_DETECT
			if( root_ctrl->globe_check.globe_thread_id != NULL )
			{
				__here__;
				while( esKRNL_TDelReq(root_ctrl->globe_check.globe_thread_id) != OS_TASK_NOT_EXIST )
					esKRNL_TimeDly(5);
				root_ctrl->globe_check.globe_thread_id = NULL;
			}	
#endif
			esMEMS_Bfree(root_para, sizeof(root_para_t));
			esMEMS_Bfree(root_ctrl, sizeof(root_ctrl_t));
						
			return EPDK_OK;
		}
		
		case GUI_MSG_COMMAND:
 		{           
			__here__;
 			app_root_command_proc(msg);
            
			return EPDK_OK;
 		}
		
		case GUI_MSG_KEY:
		{
#if BEETLES_KEY_REDEF
			__gui_msg_t msg_redef;


			msg_redef.id= msg->id;
			msg_redef.h_srcwin= msg->h_srcwin;
			msg_redef.h_deswin= msg->h_deswin;
			msg_redef.dwAddData1 = msg->dwAddData1;
			msg_redef.dwAddData2 = msg->dwAddData2;
			msg_redef.dwReserved= msg->dwReserved;
#endif

#if BEETLES_KEY_REDEF
			//app key
			if(cmd_to_app_key_proc(msg,&msg_redef) == EPDK_OK)
			{
				return EPDK_OK;
			}			

			return GUI_ManWinDefaultProc(&msg_redef); 

#else
			//app key
			if(cmd_to_app_key_proc(msg) == EPDK_OK)
			{
				return EPDK_OK;
			}			

			break;

#endif
		}
		
		case GUI_MSG_TOUCH:			
		{
			__here__;
			return EPDK_OK;
		}
		
		case DSK_MSG_FS_PART_PLUGOUT:
		{
			if(('D' == msg->dwAddData2)&&('E' == msg->dwAddData2)&&('Z' == msg->dwAddData2))
			{
				__msg("internal disk, not need kill apps, msg->dwAddData1=%d\n", msg->dwAddData2);
				return EPDK_OK;
			}           

			////__msg("msg->dwAddData1=%x, msg->dwAddData2=%x\n",msg->dwAddData1, msg->dwAddData2);
			__here__;
			app_root_fs_device_plugout(msg);		

			return EPDK_OK;
		}
		
		case DSK_MSG_FS_PART_PLUGIN:
		{
			if(('D' == msg->dwAddData2)&&('E' == msg->dwAddData2)&&('Z' == msg->dwAddData2))
			{
				__msg("internal disk, not need notify home app, msg->dwAddData1=%d\n", msg->dwAddData1);
				return EPDK_OK;
			}
			esKRNL_TimeDly(2);
			////plugin new fs ,clear list,return home,enter new fs and play video
			__here__;
			app_root_fs_device_plugin(msg);
			
			return EPDK_OK;
		}
		
		case DSK_MSG_VOLTAGE_CHANGE:        
		{
			__msg("----DSK_MSG_VOLTAGE_CHANGE =%d\n", msg->dwAddData1);
			app_root_broadcast_msg(msg);
			
			return EPDK_OK;
		}
#if BEETLES_KEY_REDEF
		case DSK_MSG_TIME_CHANGE:        
		{
			__here__;
			if(1 == msg->dwAddData1)
				key_switch = 1;
			else
				key_switch = 0;
				
			return EPDK_OK;
		}
#endif
		case DSK_MSG_TVDAC_PLUGOUT:
		case DSK_MSG_HDMI_PLUGOUT:
		{
			__msg("----DSK_MSG_TVDAC_PLUGOUT =%d\n", msg->dwAddData1);
			app_root_broadcast_msg(msg);
			
			return EPDK_OK;
		}
		
		case DSK_MSG_BOX_STANDBY_EXIT:
		{
			root_ctrl_t   *root_ctrl;
			root_ctrl = (root_ctrl_t *)GUI_WinGetAddData(msg->h_deswin);
			
			delete_all_app_except_home(root_ctrl);
			app_home_restore(root_ctrl);

			return EPDK_OK;
		}
		
        case DSK_MSG_LOW_POWER_TIP:
		{
			root_ctrl_t   *root_ctrl;

			root_ctrl = (root_ctrl_t *)GUI_WinGetAddData(msg->h_deswin);

			app_root_show_dlg(root_ctrl->root_para->h_parent,STRING_LOW_POWER,0);

    		return EPDK_OK;

		}
				
	}
	
	return GUI_ManWinDefaultProc(msg); 
}

H_WIN app_root_wincreate(Activity *activity)
{
	H_WIN  						hManWin;
	__gui_manwincreate_para_t  	create_info;	

	eLIBs_memset(&create_info, 0, sizeof(__gui_manwincreate_para_t));

	create_info.attr            		= activity; 
	create_info.hParent		= activity_get_root_win(activity) ;
	create_info.hHosting 		= NULL;   
	create_info.ManWindowProc  = (__pGUI_WIN_CB)esKRNL_GetCallBack((__pCBK_t)app_root_win_proc);
	create_info.name            	= APP_ROOT; 	
	hManWin 					= GUI_ManWinCreate(&create_info);

	__msg("root win = %x\n", hManWin);

	if(hManWin == NULL)
	{        
		__wrn("home: create movie windows failed!\n");      
		return NULL;
	}

	return hManWin;
}

