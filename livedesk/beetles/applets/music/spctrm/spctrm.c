/*
*********************************************************************************************************
*											        ePDK
*						            the Easy Portable/Player Develop Kits
*									           willow app sample
*
*						        (c) Copyright 2006-2007, SoftWinners Microelectronic Co., Ltd.
*											All	Rights Reserved
*
* File    : spectrum.c
* By      : john.fu
* Version : V1.00
*********************************************************************************************************
*/
#include "beetles_app.h"

#include "spectrum.h"

#include "spctrm.h"
#include "spctrm_uipara.h"

#include "spctrm_i.h"


H_WIN hwin_spectrum;


static spectrum_para_t *spc_para = 0; 

/*
**********************************************************************************************************************
*                                           _update_spectrum
*
* Description:
*
* Arguments  :
*
* Returns    :
*
* Notes      :
*
**********************************************************************************************************************
*/
static void _update_spectrum(void)
{
	__u16  temp_level_array[SPECT_GROUP_WIDTH];
	__s32  real_level_array[NUM_SPCTRM];	
	__s32  ret;
	ret = robin_get_spectrum_info(temp_level_array);
	//ret = dsk_wkm_get_spectrum_info(temp_level_array);
	if( ret )
	{
		__wrn("No spectrum.\n");
		SPECTRUM_SetValue(hwin_spectrum, NULL);
	}
	else
	{
		__u32  i;
		
		for( i = 0;  i < NUM_SPCTRM;  i++ )
			real_level_array[i] = temp_level_array[i];
		
		SPECTRUM_SetValue(hwin_spectrum, real_level_array);				
	}
}


//add by libaiao,2011.5.10
void SPCTRM_Clear(__u32 value)
{
	__s32  real_level_array[NUM_SPCTRM];	
	eLIBs_memset(real_level_array, value, sizeof(real_level_array));
	SPECTRUM_SetValue(hwin_spectrum, real_level_array);
	return;
}

/*
**********************************************************************************************************************
*                                           SPECTRM_FrameProc
*
* Description:
*
* Arguments  :
*
* Returns    :
*
* Notes      :
*
**********************************************************************************************************************
*/


static __s32 _FrameProc(__gui_msg_t *msg)
{
	switch(msg->id)
	{
		case GUI_MSG_CREATE:
		{
			/*create spctrum*/
			__gui_ctlwincreate_para_t ctlwin_create_info;
			__spctrm_uipara_t*  ui_para;
			__spctrm_item* spctrm_item;
			__s32 			screen_width;
			__s32 			screen_height;
			
			dsk_display_get_size(&screen_width, &screen_height);
			
			ui_para = spctrm_get_uipara();	
			__here__;

			spctrm_item = esMEMS_Malloc(0, sizeof(__spctrm_item));
			if (0 == spctrm_item)
			{
				__msg("malloc fail...\n");
				return EPDK_FAIL;
			}
			eLIBs_memset(spctrm_item, 0, sizeof(__spctrm_item));

			spc_para = esMEMS_Malloc(0, sizeof(spectrum_para_t));
			if (0 == spc_para)
			{
				__msg("malloc fail...\n");
				if (spctrm_item)
				{
					esMEMS_Mfree(0, spctrm_item);
					spctrm_item = 0;
				}
				return EPDK_FAIL;
			}
			eLIBs_memset(spc_para, 0, sizeof(spectrum_para_t));

			spc_para->min = 0;
			spc_para->max = 16;
			spc_para->particle_nr = NUM_SPCTRM;
			
			__here__;
			spctrm_item->h_theme_particle_bmp = theme_open(ui_para->partical_bmp_id);
			spctrm_item->h_theme_buoy_bmp = theme_open(ui_para->ball_bmp_id);
			__here__;			
			spc_para->particle_bmp_buf = theme_hdl2buf(spctrm_item->h_theme_particle_bmp);
			spc_para->buoy_bmp_buf = theme_hdl2buf(spctrm_item->h_theme_buoy_bmp);	
	
			__here__;	
			ctlwin_create_info.name = "spctrum";
			ctlwin_create_info.hParent = msg->h_deswin;
			ctlwin_create_info.spClassName = CTRL_SPECTRUM;
			ctlwin_create_info.spCaption= NULL; 		
			ctlwin_create_info.dwStyle = WS_VISIBLE; 
			ctlwin_create_info.dwExStyle = WS_NONE; 
			ctlwin_create_info.id = ID_SPCTRUM;
			ctlwin_create_info.attr = spc_para;

			ctlwin_create_info.ctl_rect.x			= ui_para->para_specctr.x; ;	
			ctlwin_create_info.ctl_rect.y			= ui_para->para_specctr.y;;
			ctlwin_create_info.ctl_rect.width		= ui_para->para_specctr.width;
			ctlwin_create_info.ctl_rect.height		= ui_para->para_specctr.height;
			__here__;
			hwin_spectrum = GUI_CtrlWinCreate(&ctlwin_create_info);		
			__here__;
			GUI_WinSetAttr(msg->h_deswin, (void*)spctrm_item);
			__here__;

			//app_print_memory_usage("spectrum create window finished", NULL);	
			break;
		}
		case GUI_MSG_COMMAND:
		{
			break;
		}
		case GUI_MSG_PAINT:
		{
			break;
		}
		case GUI_MSG_DESTROY:
		{			
			__spctrm_item* spctrm_item;
			//app_print_memory_usage("spectrum start to destroy", NULL);	
			spctrm_item = GUI_WinGetAttr(msg->h_deswin);
			if (spctrm_item)
			{
				theme_close(spctrm_item->h_theme_particle_bmp);
				theme_close(spctrm_item->h_theme_buoy_bmp);				
				esMEMS_Mfree(0, spctrm_item);
				spctrm_item = 0;								
			}	
			
			if (spc_para)
			{
				esMEMS_Mfree(0, spc_para);
				spc_para = 0;
			}
			//app_print_memory_usage("spectrum window destroy finished", NULL);	
			break;
		}
		case GUI_MSG_CLOSE:
		{
			GUI_FrmWinDelete(msg->h_deswin);
			return 0;
		}
		
		case SPCTRM_ID_UPDATE:
		{
		    _update_spectrum();
		    break;
		}    
		default:
			GUI_FrmWinDefaultProc(msg);
			break;
	}
	return 0;
}




/*
**********************************************************************************************************************
*                                               SPECTRM_FrameWinCreate
*
* Description:
*
* Arguments  :
*
* Returns    :
*
* Notes      :
*
**********************************************************************************************************************
*/


H_WIN SPCTRM_SceneCreate(H_WIN h_parent, H_WIN lyrwin)
{
	__gui_framewincreate_para_t framewin_para;
	__spctrm_uipara_t*  ui_para;
	
	//app_print_memory_usage("spectrum win on create message", NULL);
	
	ui_para = spctrm_get_uipara();	

	eLIBs_memset(&framewin_para, 0, sizeof(__gui_framewincreate_para_t));
	
	framewin_para.name          = "spectrum frame win",
	framewin_para.dwExStyle     = WS_EX_NONE;
	framewin_para.dwStyle       = WS_NONE|WS_VISIBLE;
	framewin_para.spCaption     =  NULL;
	framewin_para.hOwner	    = NULL;
	framewin_para.hHosting      = h_parent;
	framewin_para.FrameWinProc  = (__pGUI_WIN_CB)esKRNL_GetCallBack((__pCBK_t)_FrameProc);

	framewin_para.rect.x        = ui_para->para_frmwin.x;
	framewin_para.rect.y        = ui_para->para_frmwin.y; 
	framewin_para.rect.width    = ui_para->para_frmwin.width;
	framewin_para.rect.height   = ui_para->para_frmwin.height;

	framewin_para.BkColor.alpha = 0;
	framewin_para.BkColor.red   = 0;
	framewin_para.BkColor.green = 255;
	framewin_para.BkColor.blue  = 0;
	framewin_para.attr          =  NULL;
	framewin_para.hLayer        = lyrwin;

	
	//GUI_SetBkColor(GUI_BLACK);
	
	return (GUI_FrmWinCreate(&framewin_para));		
}
/*
**********************************************************************************************************************
*                                               SPECTRM_Update
*
* Description: 
*
* Arguments  : 
*
* Returns    : 
*
* Notes      : 
*
**********************************************************************************************************************
*/
void SPCTRM_Update(H_WIN hwin)
{
    _update_spectrum();
    return;
}
