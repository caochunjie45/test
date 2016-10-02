/*
**************************************************************************************************************
*											         ePDK
*						            the Easy Portable/Player Develop Kits
*									           desktop system 
*
*						        	 (c) Copyright 2007-2010, ANDY, China
*											 All Rights Reserved
*
* File    	: gscene_headbar.c
* By      	: Andy.zhang
* Func		: headbar manage
* Version	: v1.0
* ============================================================================================================
* 2009-7-20 8:51:52  andy.zhang  create this file, implements the fundemental interface;
**************************************************************************************************************
*/
#include "mod_init_i.h"
#include "headbar_uipara.h"
#include "headbar_com.h"

/**********************************************************************************************************************/
#define GSCENE_HEADBAR_ID		4000

/**********************************************************************************************************************/
static H_WIN headbar_mainwin = NULL;
static __u32 hb_cur_type = NULL;
static __u32 hb_root_type = NULL;

typedef struct 
{
	H_LYR		hbar_lyr;
	H_WIN 		hbar_frm;
	//__s32		hbar_app;////1-show,0-hide
}shbar_ctrl_t;


H_LYR headbar_layer_create(void)
{
	int width;
	int height;		
	H_LYR layer = NULL;
	FB  fb =
	{
	    {0, 0},                                   		/* size      */
	    {0, 0, 0},                                      /* buffer    */
	    {FB_TYPE_RGB, {PIXEL_COLOR_ARGB8888, 0, (__rgb_seq_t)0}},    /* fmt       */
	};

	__disp_layer_para_t lstlyr =
	{
	    DISP_LAYER_WORK_MODE_NORMAL,                    /* mode      */
	    0,                                              /* ck_mode   */
	    0,                                              /* alpha_en  */
	    0,                                              /* alpha_val */
	    1,                                              /* pipe      */
	    0xff,                                           /* prio      */
	    {0, 0, 0, 0},                           		/* screen    */
	    {0, 0, 0, 0},                               	/* source    */
	    DISP_LAYER_OUTPUT_CHN_DE_CH1,                   /* channel   */
	    NULL                                            /* fb        */
	};

	__layerwincreate_para_t lyrcreate_info =
	{
	    "headbar layer",
	    NULL,
	    GUI_LYRWIN_STA_SUSPEND,
	    GUI_LYRWIN_NORMAL
	};

	dsk_display_get_size(&width, &height);
	
	fb.size.width			= headbar_uipara.hbar_lyr.fb_w;
	fb.size.height			= headbar_uipara.hbar_lyr.fb_h;		

	lstlyr.src_win.x  		= headbar_uipara.hbar_lyr.src_x;
	lstlyr.src_win.y  		= headbar_uipara.hbar_lyr.src_y;
	lstlyr.src_win.width 	= headbar_uipara.hbar_lyr.src_w;
	lstlyr.src_win.height 	= headbar_uipara.hbar_lyr.src_h;
	
	lstlyr.scn_win.x		= headbar_uipara.hbar_lyr.scn_x;
	lstlyr.scn_win.y		= headbar_uipara.hbar_lyr.scn_y;
	lstlyr.scn_win.width  	= headbar_uipara.hbar_lyr.scn_w;
	lstlyr.scn_win.height 	= headbar_uipara.hbar_lyr.scn_h;
	
	lstlyr.pipe 			= headbar_uipara.hbar_lyr.pipe;
	lstlyr.fb 				= &fb;
	lyrcreate_info.lyrpara 	= &lstlyr;
	
	layer = GUI_LyrWinCreate(&lyrcreate_info);
	if( !layer )
	{
		__err("headbar layer create error !\n");
	} 
		
	return layer;
}

static __s32 cb_shbar_mwin(__gui_msg_t *msg)
{
	//__msg("---cb_shbar_mwin--%d\n",msg->id);
	switch( msg->id )
	{
		case GUI_MSG_CREATE:
		{
			shbar_ctrl_t	*scene;
			GUI_FONT *font = NULL;
			
			font = (GUI_FONT*)GUI_WinGetAttr(msg->h_deswin);
			
			scene = (shbar_ctrl_t *)esMEMS_Malloc(0, sizeof(shbar_ctrl_t));
			if( !scene )
			{
				__err(" ----GUI_MSG_CREATE\n");
				return EPDK_FAIL;
			} 
			eLIBs_memset(scene, 0, sizeof(shbar_ctrl_t));				
			GUI_WinSetAddData(msg->h_deswin, (__u32)scene);	
			headbar_init_uipara(font);
			__here__;
			scene->hbar_lyr		= headbar_layer_create();
			scene->hbar_frm 		= headbar_frm_create(msg->h_deswin, scene->hbar_lyr);
			
			return EPDK_OK;
		}
			
		case GUI_MSG_DESTROY:
		{
			shbar_ctrl_t	*scene = NULL;
			
			scene = (shbar_ctrl_t *)GUI_WinGetAddData(msg->h_deswin);

			if(scene->hbar_frm)
			{
				headbar_frm_delete(scene->hbar_frm);
				scene->hbar_frm = NULL;
			}
			if(scene->hbar_lyr)
			{
				GUI_LyrWinDelete(scene->hbar_lyr);
				scene->hbar_lyr = NULL;
			}
			
			esMEMS_Mfree(0, scene);

			return EPDK_OK;
		}
			
		case GUI_MSG_CLOSE:
		{
			GUI_ManWinDelete(msg->h_deswin);	
			
			return EPDK_OK;
		}

		case GUI_MSG_KEY:
		{
			return EPDK_OK;
		}

		case GUI_MSG_COMMAND:
		{			
			return EPDK_OK;
		}
		
		case HBAR_DISK_PAINT:
		{
			__gui_msg_t msgex;
			shbar_ctrl_t	*scene;
			__here__;
			scene = (shbar_ctrl_t *)GUI_WinGetAddData(msg->h_deswin);
			eLIBs_memset(&msgex, 0, sizeof(__gui_msg_t));

			msgex.id 			= HBAR_DISK_PAINT;
			msgex.h_srcwin 		= msg->h_deswin;
			msgex.h_deswin		= scene->hbar_frm;
			msgex.dwAddData1	= msg->dwAddData1;
			msgex.dwAddData2	= msg->dwAddData2;	
			//__msg("-----0x%x,0x%x\n",msgex.dwAddData1,msgex.dwAddData2);

			GUI_SendMessage(&msgex);

			return EPDK_OK;
		}
		
		default:		
			break;	 	
	}	

	return EPDK_OK;
}


__s32 gscene_hbar_create(H_WIN parent, GUI_FONT *font)
{
	H_WIN  						hManWin;	
	__gui_manwincreate_para_t  	create_info;	
	
	eLIBs_memset(&create_info, 0, sizeof(__gui_manwincreate_para_t));
	
	create_info.id					= GSCENE_HEADBAR_ID;
	create_info.attr            			= (void *)font; 
	create_info.hParent			= parent;
	create_info.hHosting 			= NULL;   
	create_info.ManWindowProc   	= cb_shbar_mwin;
	create_info.name            		= "headbar"; 	
	hManWin 						= GUI_ManWinCreate(&create_info);
	
	if(hManWin == NULL)
	{        
	    __wrn("headbar: create main windows failed!\n");      
	    return NULL;
	}
	headbar_mainwin = hManWin;
    
	return EPDK_OK;
}

__s32 gscene_hbar_delete(void)
{
	if(headbar_mainwin)
	{
		GUI_ManWinDelete(headbar_mainwin);	
	        headbar_mainwin = NULL;
	}	
	return EPDK_OK;
}

__s32 gscene_hbar_set_state(HBarState state)
{	
	shbar_ctrl_t	*pscene 	= (shbar_ctrl_t *)GUI_WinGetAddData(headbar_mainwin);
	H_WIN		hbar_layer 	= pscene->hbar_lyr;

	switch(state)
	{
		case HBAR_ST_SHOW:
			//GUI_LyrWinCacheOn();
			GUI_LyrWinSetSta(hbar_layer, GUI_LYRWIN_STA_ON);
			GUI_LyrWinSetTop(hbar_layer);	
			//GUI_LyrWinCacheOff();
			break;
		case HBAR_ST_HIDE:
			GUI_LyrWinSetSta(hbar_layer, GUI_LYRWIN_STA_SUSPEND);
			break;
		default:
			__wrn("HBarState is invalid! \n");
			break;				
	}
	
	return EPDK_OK;
}

__s32 gscene_hbar_get_state(HBarState *p_state)
{
	__gui_lyrwin_sta_t 	state;	
	shbar_ctrl_t		*pscene 	= (shbar_ctrl_t *)GUI_WinGetAddData(headbar_mainwin);
	H_WIN 			hbar_layer 	= pscene->hbar_lyr;
	
	state =  GUI_LyrWinGetSta(hbar_layer);
	if( state == GUI_LYRWIN_STA_ON)
	{
		*p_state = HBAR_ST_SHOW;
	}
	else
	{
		*p_state = HBAR_ST_HIDE;
	}
	
	return EPDK_OK;
}

__s32 gscene_hbar_set_title(char *title, __u32 len)
{
	
	return EPDK_OK;
}

__s32 gscene_hbar_current_disk(__u32 cur_type, __u32 root_type)
{
	__gui_msg_t msgex;

	eLIBs_memset(&msgex, 0, sizeof(__gui_msg_t));

	msgex.id 			= HBAR_DISK_PAINT;
	msgex.h_srcwin 		= NULL;
	msgex.h_deswin		= headbar_mainwin;
	msgex.dwAddData1	= cur_type;
	msgex.dwAddData2	= root_type;	
	
	GUI_SendNotifyMessage(&msgex);
	
	return EPDK_OK;
}

__s32 gscene_hbar_set_appstate(HBarApp appstate)
{	
	
	return EPDK_OK;
}

__s32 gscene_hbar_get_appstate(HBarApp *appstate)
{
	*appstate = HBAR_APP_HIDE;
	
	return EPDK_OK;
}

__s32 gscene_hbar_set_disk(__u32 cur_type, __u32 root_type)
{
	hb_cur_type = cur_type;
	hb_root_type = root_type;

	return EPDK_OK;
}

__s32 gscene_hbar_get_disk(__u32 *cur_type, __u32 *root_type)
{
	*cur_type = hb_cur_type;
	*root_type = hb_root_type;
	
	//eLIBs_printf("### %s, %d, cur_type = %x, root_type = %x ###\n", __FILE__, __LINE__, *cur_type, *root_type);

	return EPDK_OK;
}


