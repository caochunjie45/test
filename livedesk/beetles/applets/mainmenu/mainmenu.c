/*
**************************************************************************************************************
*											         ePDK
*						            the Easy Portable/Player Develop Kits
*									           desktop system 
*
*						        	 (c) Copyright 2007-2010, ANDY, China
*											 All Rights Reserved
*
* File    	: mainmenu.c
* By      	: James.lu
* Func		: desk main thread
* Version	: v1.0
* ============================================================================================================
* 2009-7-20 8:51:52  andy.zhang  create this file, implements the fundemental interface;
**************************************************************************************************************
*/

#include "mainmenu_private.h"


static mainmenu_res_id_t mainmenu_res_english_id[] =
{	
	{STRING_MAINMENU_TV,    ID_MAINMENU_ENGLISH_TV_FOCUS_BMP,      ID_MAINMENU_ENGLISH_TV_BMP},		
	{STRING_MAINMENU_MOVIE, ID_MAINMENU_ENGLISH_MOVIE_FOCUS_BMP, ID_MAINMENU_ENGLISH_MOVIE_BMP},		
	{STRING_MAINMENU_MUSIC, ID_MAINMENU_ENGLISH_MUSIC_FOCUS_BMP, ID_MAINMENU_ENGLISH_MUSIC_BMP},	
	{STRING_MAINMENU_PHOTO, ID_MAINMENU_ENGLISH_PHOTO_FOCUS_BMP, ID_MAINMENU_ENGLISH_PHOTO_BMP},
	{STRING_MAINMENU_FM,    ID_MAINMENU_ENGLISH_FM_FOCUS_BMP,    ID_MAINMENU_ENGLISH_FM_BMP},
	{STRING_MAINMENU_EBOOK, ID_MAINMENU_ENGLISH_EBOOK_FOCUS_BMP, ID_MAINMENU_ENGLISH_EBOOK_BMP},
	{STRING_MAINMENU_EXPLORER,ID_MAINMENU_ENGLISH_EXPLORER_FOCUS_BMP,ID_MAINMENU_ENGLISH_EXPLORER_BMP},
	{STRING_MAINMENU_SETTING,ID_MAINMENU_ENGLISH_SETTING_FOCUS_BMP,ID_MAINMENU_ENGLISH_SETTING_BMP},
};





static __u32 mainmenu_index_key2item[10] = {0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};


void alpha_fadeout( void *buf, __u32 len)
{
	__u32 i;
	__u8 alpha;	
	
	for (i = 0; i < len; i++)
	{
		alpha = *((__u8 *)buf+ i * 4 + 3);				
		*((__u8 *)buf + i * 4 + 3) =(__u8)(3*alpha/4);
	}

	return;
}

H_LYR mainmenu_lyr_create(__mainmenu_layer_para_t* layer)
{
	FB	fb = {
	    	{0, 0},                                   		
	    	{0, 0, 0},                                      
	    	{FB_TYPE_RGB, {PIXEL_COLOR_ARGB8888, 0, RGB_SEQ_ARGB}},    
	};
	__disp_layer_para_t para = 
	{
	    	DISP_LAYER_WORK_MODE_NORMAL,                   
	    	0, 
	    	0, 
	    	0xcc, 
	    	
	    	1, 
	    	0xff,
	    	{0, 0, 0, 0},
	    	{0, 0, 0, 0},
	    	DISP_LAYER_OUTPUT_CHN_DE_CH1,
	    	NULL
	};
	
	__layerwincreate_para_t create_info = 
	{
		"mainmenu layer",
		NULL,
		GUI_LYRWIN_STA_SUSPEND,
		GUI_LYRWIN_NORMAL
	};

	fb.size.width  		= layer->fb_w;
	fb.size.height 		= layer->fb_h;

	para.pipe			= layer->pipe;
	
	para.scn_win.x 		= layer->scn_x;
	para.scn_win.y 		= layer->scn_y;
	para.scn_win.width 	= layer->scn_w;	
	para.scn_win.height 	= layer->scn_h;	

	para.src_win.x 		= layer->src_x;
	para.src_win.y 		= layer->src_y;
	para.src_win.width 		= layer->src_w;	
	para.src_win.height 	= layer->src_h;	

	para.fb = &fb;
	create_info.lyrpara = &para;	
	
	return(GUI_LyrWinCreate(&create_info));
}

H_LYR get_mainmenu_lyrwin(H_WIN hWnd)
{
	mainmenu_para_t *mainmenu_para;
	mainmenu_para = (mainmenu_para_t *)GUI_WinGetAddData(hWnd);
	
	return	mainmenu_para->mainmenu_layer;
}

__s32 mainmenu_get_curitem(mainmenu_para_t *mainmenu_para)
{
	__u32 i = 0,k = 0;
	__mainmenu_frm_para_t* frm_para;
	
	frm_para = mainmenu_para->uipara->mainmenu_para.frm_para;
	for(i = 0; i < ITEM_NUM_ALL; i++)
	{
		if(frm_para[i].state == 1)
		{
			mainmenu_index_key2item[k] = frm_para[i].res_index;
			k++;
		}
	}
	
	mainmenu_para->cur_item_cnt = k;
	mainmenu_para->cur_item = mainmenu_index_key2item[0];

	return EPDK_OK;
}

__s32 reinit_mainmenu_lan(mainmenu_para_t *mainmenu_para)
{
	__u32 	i = 0;    
	reg_system_para_t* para;

	para = (reg_system_para_t*)dsk_reg_get_para_by_app(REG_APP_SYSTEM);

	mainmenu_para->first_flag = 1;

	for(i = 0; i < ITEM_NUM_ALL; i++)
	{       		
		if(mainmenu_para->uipara->mainmenu_para.frm_para[i].state)
		{
			dsk_langres_get_menu_text(mainmenu_res_english_id[i].lang_id, mainmenu_para->item_str[i], GUI_NAME_MAX);
		}
	}
	esKRNL_TimeDly(2);
	
	return EPDK_OK;
}

__s32 init_mainmenu_res(mainmenu_para_t *mainmenu_para)
{
	__u32 	i = 0;    
	reg_system_para_t* para;

	para = (reg_system_para_t*)dsk_reg_get_para_by_app(REG_APP_SYSTEM);

	mainmenu_para->fccolor = GUI_YELLOW;
	mainmenu_para->ufcolor = MAINMENU_STR_COLOR;
	mainmenu_para->breath_flag = 0;
	mainmenu_para->first_flag = 1;
	mainmenu_para->uipara = mainmenu_init_uipara();

	mainmenu_get_curitem(mainmenu_para);


	for(i = 0; i < ITEM_NUM_ALL; i++)
	{
		if(mainmenu_para->uipara->mainmenu_para.frm_para[i].state)
			dsk_langres_get_menu_text(mainmenu_res_english_id[i].lang_id, mainmenu_para->item_str[i], GUI_NAME_MAX);
	}
	esKRNL_TimeDly(2);

	for(i = 0; i < ITEM_NUM_ALL; i++)
	{       		
		if(mainmenu_para->uipara->mainmenu_para.frm_para[i].state)
		{
			mainmenu_para->focus_bmp[i] = dsk_theme_open(mainmenu_res_english_id[i].focus_bmp_id);	
			mainmenu_para->focus_bmp_hdl[i] = dsk_theme_hdl2buf(mainmenu_para->focus_bmp[i]);

			mainmenu_para->unfocus_bmp[i] = dsk_theme_open(mainmenu_res_english_id[i].unfocus_bmp_id);
			mainmenu_para->unfocus_bmp_hdl[i] = dsk_theme_hdl2buf(mainmenu_para->unfocus_bmp[i]);
		}
	}
    
	return EPDK_OK;
}

__s32 uninit_mainmenu_res(mainmenu_para_t *mainmenu_para)
{
	  __u32 	i; 

	mainmenu_deinit_uipara();
	for (i=0; i<ITEM_NUM_ALL; i++)
	{
		if(mainmenu_para->focus_bmp[i])
		{
			dsk_theme_close(mainmenu_para->focus_bmp[i]);
			mainmenu_para->focus_bmp[i] = NULL;
		}
		if(mainmenu_para->unfocus_bmp[i])
		{
			dsk_theme_close(mainmenu_para->unfocus_bmp[i]);
			mainmenu_para->unfocus_bmp[i] = NULL;
		}
	}

	return EPDK_OK;
}

static H_WIN mainmenu_get_index2win(mainmenu_para_t *mainmenu_para)
{
	H_WIN	h_win = NULL;

	switch(mainmenu_para->cur_item)
	{
		case RESID_INDEX_TV:
		{
			h_win = mainmenu_para->tv_fwin;
			
			break;
		}
		case RESID_INDEX_MOVIE:
		{
			h_win = mainmenu_para->movie_fwin;
			
			break;
		}
		case RESID_INDEX_MUSIC:
		{
			h_win = mainmenu_para->music_fwin;
			
			break;
		}
		case RESID_INDEX_PHOTO:
		{
			h_win = mainmenu_para->photo_fwin;
			
			break;
		}
		case RESID_INDEX_EBOOK:
		{
			h_win = mainmenu_para->ebook_fwin;
			
			break;
		}
		case RESID_INDEX_FM:
		{
			h_win = mainmenu_para->fm_fwin;
			
			break;
		}

		case RESID_INDEX_RECORD:
		{
			h_win = mainmenu_para->record_fwin;
			
			break;
		}

		case RESID_INDEX_EXPLORER:
		{
			h_win = mainmenu_para->explorer_fwin;
			
			break;
		}

		case RESID_INDEX_SETTING:
		{
			h_win = mainmenu_para->setting_fwin;
			
			break;
		}
		default:
			break;
			
	}

	return h_win;
}

static __u32 mainmenu_get_index2fid(mainmenu_para_t *mainmenu_para)
{
	__u32	fid = ID_MAINMENU_MOVIE;

	switch(mainmenu_para->cur_item)
	{
		case RESID_INDEX_TV:
		{
			fid = ID_MAINMENU_TV;
			
			break;
		}
		case RESID_INDEX_MOVIE:
		{
			fid = ID_MAINMENU_MOVIE;
			
			break;
		}
		case RESID_INDEX_MUSIC:
		{
			fid = ID_MAINMENU_MUSIC;
			
			break;
		}
		case RESID_INDEX_PHOTO:
		{
			fid = ID_MAINMENU_PHOTO;
			
			break;
		}
		case RESID_INDEX_EBOOK:
		{
			fid = ID_MAINMENU_EBOOK;
			
			break;
		}
		case RESID_INDEX_FM:
		{
			fid = ID_MAINMENU_FM;
			
			break;
		}
		case RESID_INDEX_KARAOKE:
		{
			fid = ID_MAINMENU_KARAOKE;
			
			break;
		}
		case RESID_INDEX_RECORD:
		{
			fid = ID_MAINMENU_RECORD;
			
			break;
		}
		case RESID_INDEX_EXPLORER:
		{
			fid = ID_MAINMENU_EXPLORER;
			
			break;
		}
		case RESID_INDEX_SETTING:
		{
			fid = ID_MAINMENU_SETTING;
			
			break;
		}
		case RESID_INDEX_CALENDAR:
		{
			fid = ID_MAINMENU_CALENDAR;
			
			break;
		}
		default:
			break;
			
	}

	return fid;
}

static __s32 mainmenu_get_fid2winrec(mainmenu_para_t *mainmenu_para,__u32 fid,RECT* Win_Rect)
{
	__u32	i;
	__mainmenu_uipara_t*	uipara;
	
	uipara = mainmenu_para->uipara;
	for(i = 0; i < uipara->mainmenu_para.all_item; i++)
	{
		if(uipara->mainmenu_para.frm_para[i].fid == fid)
			break;
	}

	Win_Rect->x = uipara->mainmenu_para.frm_para[i].posx;
	Win_Rect->y = uipara->mainmenu_para.frm_para[i].posy;
	Win_Rect->width = uipara->mainmenu_para.frm_para[i].posw;
	Win_Rect->height = uipara->mainmenu_para.frm_para[i].posh;
	return EPDK_OK;
}

static __s32 mainmenu_setwin_unfocus(H_WIN hWnd)
{
	__gui_msg_t msg;

	msg.h_deswin = hWnd;
	msg.h_srcwin = NULL;
	msg.id = GUI_MSG_SET_UNFOCUS;
	msg.dwAddData1 = 0;
	msg.dwAddData2 = 0;

	GUI_SendNotifyMessage(&msg);

	return EPDK_OK;
}

static __s32 mainmenu_breath_restart(mainmenu_para_t *mainmenu_para)
{
	return EPDK_OK;
}

static __s32 mainmenu_breath_start(mainmenu_para_t *mainmenu_para,__s32 bres_index)
{	
	return EPDK_OK;
}

static __s32 mainmenu_breath_stop(mainmenu_para_t *mainmenu_para)
{

	return EPDK_OK;
}


static __s32 mainmenu_setwin_wakeup(H_WIN hWnd)
{
	__gui_msg_t msg;

	msg.h_deswin = hWnd;
	msg.h_srcwin = NULL;
	msg.id = GUI_MSG_WIN_WAKEUP;
	msg.dwAddData1 = 0;
	msg.dwAddData2 = 0;

	GUI_SendNotifyMessage(&msg);

	return EPDK_OK;
}

static __s32 app_mainmenu_proc(__gui_msg_t *msg)
{
	switch(msg->id)
	{
		case GUI_MSG_CREATE:
		{
			if(BREATH_Init() == EPDK_FAIL)
			{
				__wrn("----init breath ail\n");
			}
			
			return EPDK_OK;
		}
		
		case GUI_MSG_CLOSE:
		{
			GUI_ManWinDelete(msg->h_deswin);

			return EPDK_OK;
		}
		
		case GUI_MSG_DESTROY:
		{
			mainmenu_para_t *mainmenu_para;

			mainmenu_para = (mainmenu_para_t *)GUI_WinGetAddData(msg->h_deswin);

			mainmenu_breath_stop(mainmenu_para);
			BREATH_Exit();
			__here__;
			uninit_mainmenu_res(mainmenu_para);
			if(mainmenu_para->tv_fwin)
			{
				tvfrm_frm_delete(mainmenu_para->tv_fwin);
				mainmenu_para->tv_fwin = NULL;
			}
			if(mainmenu_para->movie_fwin)
			{
				moviefrm_frm_delete(mainmenu_para->movie_fwin);
				mainmenu_para->movie_fwin = NULL;
			}
			if(mainmenu_para->music_fwin)
			{
				moviefrm_frm_delete(mainmenu_para->music_fwin);
				mainmenu_para->music_fwin = NULL;
			}
			if(mainmenu_para->photo_fwin)
			{
				photofrm_frm_delete(mainmenu_para->photo_fwin);
				mainmenu_para->photo_fwin = NULL;
			}
			if(mainmenu_para->ebook_fwin)
			{
				ebookfrm_frm_delete(mainmenu_para->ebook_fwin);
				mainmenu_para->ebook_fwin = NULL;
			}
			if(mainmenu_para->fm_fwin)
			{
				fmfrm_frm_delete(mainmenu_para->fm_fwin);
				mainmenu_para->fm_fwin = NULL;
			}
			if(mainmenu_para->record_fwin)
			{
				recordfrm_frm_delete(mainmenu_para->record_fwin);
				mainmenu_para->record_fwin = NULL;
			}
			if(mainmenu_para->explorer_fwin)
			{
				explorerfrm_frm_delete(mainmenu_para->explorer_fwin);
				mainmenu_para->explorer_fwin = NULL;
			}

			if(mainmenu_para->setting_fwin)
			{
				settingfrm_frm_delete(mainmenu_para->setting_fwin);
				mainmenu_para->setting_fwin = NULL;
			}
			if(mainmenu_para->mainmenu_layer)
			{
				GUI_LyrWinDelete(mainmenu_para->mainmenu_layer);
				mainmenu_para->mainmenu_layer = NULL;
			}

			esMEMS_Bfree(mainmenu_para, sizeof(mainmenu_para_t));

			return EPDK_OK;
		}
		
		case GUI_MSG_KEY:
		{
			__u32 i = 0;
			mainmenu_para_t *mainmenu_para;
			
			mainmenu_para = (mainmenu_para_t *)GUI_WinGetAddData(msg->h_deswin);

			switch(msg->dwAddData1)
			{
				case GUI_MSG_KEY_LEFT:
				case GUI_MSG_KEY_LONGLEFT:
				{
					if(KEY_UP_ACTION == msg->dwAddData2)
					{
						mainmenu_setwin_unfocus(mainmenu_get_index2win(mainmenu_para));
						if(mainmenu_para->cur_item == mainmenu_index_key2item[0])
						{
							mainmenu_para->cur_item = mainmenu_index_key2item[mainmenu_para->cur_item_cnt - 1];
							i = mainmenu_para->cur_item_cnt - 1;
						}
						else
						{
							for(i = 0; i < 10; i++)//get cur array index
							{

								if(mainmenu_para->cur_item == mainmenu_index_key2item[i])
									break;
							}
							mainmenu_para->cur_item = mainmenu_index_key2item[--i];
						}
						GUI_WinSetFocusChild(mainmenu_get_index2win(mainmenu_para));
					}
					return EPDK_OK;
				}
				
				case GUI_MSG_KEY_RIGHT:
				case GUI_MSG_KEY_LONGRIGHT:
				{
					if(KEY_UP_ACTION == msg->dwAddData2)
					{
						mainmenu_setwin_unfocus(mainmenu_get_index2win(mainmenu_para));
						if(mainmenu_para->cur_item == mainmenu_index_key2item[mainmenu_para->cur_item_cnt - 1])
						{
							mainmenu_para->cur_item = mainmenu_index_key2item[0];
							i = 0;
						}
						else
						{
							for(i = 0; i < 10; i++)//get cur array index
							{
								if(mainmenu_para->cur_item == mainmenu_index_key2item[i])
									break;
							}
							mainmenu_para->cur_item = mainmenu_index_key2item[++i];
                            eDbug("mainmenu_para->cur_item is %d \n",mainmenu_para->cur_item);
						}

						GUI_WinSetFocusChild(mainmenu_get_index2win(mainmenu_para));
					}
					return EPDK_OK;
				}
                
				case GUI_MSG_KEY_UP:
				case GUI_MSG_KEY_LONGUP:
				case GUI_MSG_KEY_DOWN:
				case GUI_MSG_KEY_LONGDOWN:
				{
					if(KEY_UP_ACTION == msg->dwAddData2)
					{
						i = 0 ;
						mainmenu_setwin_unfocus(mainmenu_get_index2win(mainmenu_para));
						eDbug("mainmenu_para->cur_item = %d \n",mainmenu_para->cur_item);
						for(i = 0; i < 10; i++)//get cur array index
						{
							if(mainmenu_para->cur_item == mainmenu_index_key2item[i])
								break;
						}
                        if(mainmenu_para->cur_item < mainmenu_index_key2item[mainmenu_para->cur_item_cnt/2])
                        {
                            i = i + 4;
                            mainmenu_para->cur_item = mainmenu_index_key2item[i]; 
                        }
                        else
                        {
                            i = i - 4;
                            mainmenu_para->cur_item = mainmenu_index_key2item[i]; 
                        }    
                        eDbug("mainmenu_para->cur_item is %d \n",mainmenu_para->cur_item);
						
						GUI_WinSetFocusChild(mainmenu_get_index2win(mainmenu_para));

					}
					return EPDK_OK;
				}

				
				case GUI_MSG_KEY_ENTER:
				case GUI_MSG_KEY_LONGENTER:
				{
					if(KEY_UP_ACTION != msg->dwAddData2)
					{
						mainmenu_breath_stop(mainmenu_para);
					}

					break;
				}
				
				default:
					break;
			}

			break;
		}
		
		case GUI_MSG_TOUCH:
			return EPDK_OK;
						
		case GUI_MSG_COMMAND:
		{
			mainmenu_para_t *mainmenu_para;
			__here__;
			mainmenu_para = (mainmenu_para_t *)GUI_WinGetAddData(msg->h_deswin);
			mainmenu_breath_stop(mainmenu_para);
			/////////
			{
				__gui_msg_t 		ntymsg;

				ntymsg.id			=  GUI_MSG_COMMAND;
				ntymsg.h_srcwin 		=  msg->h_deswin;
				ntymsg.h_deswin 		=  GUI_WinGetParent(msg->h_deswin); 
				ntymsg.dwAddData1	=  (__u32)MAKELONG(GUI_WinGetItemId(msg->h_deswin), SWITCH_TO_OTHER_APP);
				ntymsg.dwAddData2	=  msg->dwAddData1;
				ntymsg.dwReserved	=  NULL;
				
				GUI_SendNotifyMessage(&ntymsg); 
			}
			return EPDK_OK;
		}

		case GUI_MSG_SET_FOCUS:
		{
			__u32 i = 0;
			mainmenu_para_t *mainmenu_para;

			__here__;
		}

		case GUI_MSG_SET_UNFOCUS:
		{
			mainmenu_para_t *mainmenu_para;

			__here__;
			mainmenu_para = (mainmenu_para_t *)GUI_WinGetAddData(msg->h_deswin);
			
			break;
		}


		case DSK_MSG_LAN_CHANGE://paint->creat
		{
			mainmenu_para_t *mainmenu_para;
			
			mainmenu_para = (mainmenu_para_t *)GUI_WinGetAddData(msg->h_deswin);
			__here__;
			reinit_mainmenu_lan(mainmenu_para);
			
			break;
		}

		default:
			break;
		
	}

	return GUI_ManWinDefaultProc(msg);	
}

H_WIN app_mainmenu_create(root_para_t *root_para)
{
	H_WIN			h_win = NULL;
	mainmenu_para_t *mainmenu_para;
	__gui_manwincreate_para_t create_info = {0};
	
	__here__;
	mainmenu_para = (mainmenu_para_t *)esMEMS_Balloc(sizeof(mainmenu_para_t));
	if(!mainmenu_para)	
	{
		return NULL;
	}	
	eLIBs_memset(mainmenu_para, 0, sizeof(mainmenu_para_t));

	//init mainmenu res
	init_mainmenu_res(mainmenu_para);
	/////////
	create_info.name            		= APP_HOME;
	create_info.hParent         		= root_para->h_parent;
	create_info.ManWindowProc   	= (__pGUI_WIN_CB)esKRNL_GetCallBack((__pCBK_t)app_mainmenu_proc); 
	create_info.attr            			= (void*)root_para;
	create_info.id					= APP_MAINMENU_ID;
	create_info.hHosting        		= NULL;
	
	h_win = GUI_ManWinCreate(&create_info);
	if(!h_win)
	{
		__wrn("create mainwin fail\n");
		return NULL;
	}
	GUI_WinSetAddData(h_win, (__u32)mainmenu_para);
	__here__;
	mainmenu_para->mainmenu_layer   = mainmenu_lyr_create(&mainmenu_para->uipara->mainmenu_lyr);

	if(mainmenu_para->uipara->mainmenu_para.frm_para[0].state)//tv
		mainmenu_para->tv_fwin = tvfrm_frm_create(h_win, mainmenu_para->mainmenu_layer,ID_MAINMENU_TV,MAINMENU_LYRTYPE_NORMAL);

	if(mainmenu_para->uipara->mainmenu_para.frm_para[1].state)//movie
		mainmenu_para->movie_fwin = moviefrm_frm_create(h_win, mainmenu_para->mainmenu_layer,ID_MAINMENU_MOVIE,MAINMENU_LYRTYPE_NORMAL);

	if(mainmenu_para->uipara->mainmenu_para.frm_para[2].state)//music
		mainmenu_para->music_fwin = musicfrm_frm_create(h_win, mainmenu_para->mainmenu_layer,ID_MAINMENU_MUSIC,MAINMENU_LYRTYPE_NORMAL);
	
	if(mainmenu_para->uipara->mainmenu_para.frm_para[3].state)//photo
		mainmenu_para->photo_fwin = photofrm_frm_create(h_win, mainmenu_para->mainmenu_layer,ID_MAINMENU_PHOTO,MAINMENU_LYRTYPE_NORMAL);

    if(mainmenu_para->uipara->mainmenu_para.frm_para[4].state)//fm
		mainmenu_para->fm_fwin = fmfrm_frm_create(h_win, mainmenu_para->mainmenu_layer,ID_MAINMENU_FM,MAINMENU_LYRTYPE_NORMAL);

	if(mainmenu_para->uipara->mainmenu_para.frm_para[5].state)//ebook
		mainmenu_para->ebook_fwin = ebookfrm_frm_create(h_win, mainmenu_para->mainmenu_layer,ID_MAINMENU_EBOOK,MAINMENU_LYRTYPE_NORMAL);

	

	if(mainmenu_para->uipara->mainmenu_para.frm_para[6].state)//explore
		mainmenu_para->explorer_fwin = explorerfrm_frm_create(h_win, mainmenu_para->mainmenu_layer,ID_MAINMENU_EXPLORER,MAINMENU_LYRTYPE_NORMAL);

	if(mainmenu_para->uipara->mainmenu_para.frm_para[7].state)//setting
		mainmenu_para->setting_fwin = settingfrm_frm_create(h_win, mainmenu_para->mainmenu_layer,ID_MAINMENU_SETTING,MAINMENU_LYRTYPE_NORMAL);
	

	GUI_WinSetFocusChild(mainmenu_get_index2win(mainmenu_para));

	return h_win;	

}

__s32 app_mainmenu_destroy(H_WIN h_win)
{	
	if(h_win)
	{
		GUI_ManWinDelete(h_win);
		h_win = 0;
	}
	
	return EPDK_OK;
}


