/*
************************************************************************************************************************
*											         ePDK
*						            the Easy Portable/Player Develop Kits
*									           desktop system 
*
*						        	 (c) Copyright 2007-2010, ANDY, China
*											 All Rights Reserved
*
* File    	: recordfrm.c
* By      	: James.lu
* Func	: recordfrm 
* Version	: v1.0
* ======================================================================================================================
* 2009-10-15 15:27:26 andy.zhang  create this file, implements the fundemental interface;
************************************************************************************************************************
*/
#include "recordfrm_private.h"


static __s32 recordfrm_on_key(__gui_msg_t *msg)
{		
	recordfrm_attr_t*	frmwin_attr;

	frmwin_attr = (recordfrm_attr_t *)GUI_WinGetAttr(msg->h_deswin);
	
	switch(msg->dwAddData1)
	{
		case GUI_MSG_KEY_ENTER:
		case GUI_MSG_KEY_LONGENTER:
		{
			if(KEY_UP_ACTION == msg->dwAddData2)
			{
				__gui_msg_t 		ntymsg;

				ntymsg.id			=  GUI_MSG_COMMAND;
				ntymsg.h_srcwin 		=  msg->h_deswin;
				ntymsg.h_deswin 		=  GUI_WinGetParent(msg->h_deswin); 
				ntymsg.dwAddData1	=  frmwin_attr->fid;
				ntymsg.dwAddData2	=  NULL;
				ntymsg.dwReserved	=  NULL;
				
				GUI_SendNotifyMessage(&ntymsg); 								
			}
			break;
		}
		default:
			break;
	}
	
	return EPDK_OK;
}

static __s32 recordfrm_on_paint(__gui_msg_t *msg,__u32 winsta)
{
	H_LYR			layer;
	RECT			rect;
	__pos_t		 	offset;	

	recordfrm_attr_t*	frmwin_attr;

	layer= (H_LYR)GUI_WinGetLyrWin(msg->h_deswin);
	if( GUI_LYRWIN_STA_ON != GUI_LyrWinGetSta(layer) )
	{
		return EPDK_OK;
	}

	GUI_LyrWinSel(layer);	
	GUI_WinGetWinRECT(msg->h_deswin, &rect);
	frmwin_attr = (recordfrm_attr_t *)GUI_WinGetAttr(msg->h_deswin);
	
	{		
		offset.x = (rect.width - frmwin_attr->icon_size.width)/2;
		offset.y = 0;

		if(winsta == MAINMENU_WINSTA_FCS)
		{
			GUI_BMP_Draw(frmwin_attr->res_normal.focus_bmp, rect.x + offset.x, rect.y + offset.y); 
		}
		else
		{
			GUI_BMP_Draw(frmwin_attr->res_normal.unfocus_bmp, rect.x + offset.x, rect.y + offset.y); 
		}

		eDbug("rect.x + offset.x is %d,rect.y + offset.y is %d \n",rect.x + offset.x ,rect.y + offset.y);
		//draw text
		{
			GUI_RECT txtrect;

			txtrect.x0 = rect.x + offset.x + ( frmwin_attr->icon_size.width - 6*strlen(frmwin_attr->res_normal.item_str))/2;
			txtrect.y0 = rect.y + offset.y + frmwin_attr->icon_size.height;

			GUI_UC_SetEncodeUTF8( );
			GUI_SetFont(frmwin_attr->bfont);
			if(winsta == MAINMENU_WINSTA_FCS)
				GUI_SetColor(frmwin_attr->fccolor);
			else
				GUI_SetColor(frmwin_attr->ufcolor);
				
			GUI_DispStringAt(frmwin_attr->res_normal.item_str,txtrect.x0,txtrect.y0);
		
			eDbug("txtrect.x0 is %d,txtrect.y0 is %d \n",txtrect.x0 ,txtrect.y0);
			eDbug("frmwin_attr->res_normal.item_str is %s \n",frmwin_attr->res_normal.item_str);

		}  
	}
	

	return EPDK_OK;
}


static __s32 cb_recordfrm_frm(__gui_msg_t *msg)
{	
	__msg("msg->id=%d\n",msg->id);
	switch(msg->id)
	{
		case GUI_MSG_CREATE:
		{
			break;
		}
			
		case GUI_MSG_DESTROY:
		{
			RECT		rect;
			recordfrm_attr_t*	frmwin_attr;

			frmwin_attr = (recordfrm_attr_t *)GUI_WinGetAttr(msg->h_deswin);

			if(frmwin_attr->lyr_type != MAINMENU_LYRTYPE_SPRITE)
			{
				GUI_LyrWinSel(frmwin_attr->cur_lyr);
				GUI_WinGetWinRECT(msg->h_deswin, &rect);
				GUI_SetBkColor(0x00);
				GUI_ClearRect(rect.x, rect.y, rect.x+rect.width-1, rect.y+rect.height-1);
			}
			if(frmwin_attr)
			{
				esMEMS_Mfree(0, frmwin_attr);	
				frmwin_attr = NULL;
			}
			break;
		}
			
		case GUI_MSG_KEY:
		{			
			recordfrm_on_key(msg);
			
			break;
		}			
			
		case GUI_MSG_PAINT:	
		{
			break;
		}
		
		case GUI_MSG_SET_FOCUS:	
		{
			recordfrm_on_paint(msg,MAINMENU_WINSTA_FCS);
			
			break;
		}
		
		case GUI_MSG_SET_UNFOCUS:	
		{	
			recordfrm_on_paint(msg,MAINMENU_WINSTA_UNFCS);
			break;
		}

		case GUI_MSG_WIN_WAKEUP://paint->creat
		{
			recordfrm_on_paint(msg,MAINMENU_WINSTA_UNFCS);			
			break;
		}
			
		case GUI_MSG_WIN_SLEEP:
		{
			break;			
		}
			
		default:
			break;
	}
		
	return EPDK_OK; 
}

H_WIN recordfrm_frm_create(H_WIN parent, H_WIN layer,__u16 fid,__u32 lyr_type)
{
	__u32 						i;
	RECT                    				rect;
	H_WIN 						h_win;
	__gui_framewincreate_para_t 	frame_info;
	root_para_t*					root_para;
	mainmenu_para_t*				mainmenu_para;
	__mainmenu_uipara_t*			mainmenu_uipara;
	recordfrm_attr_t*				frmwin_attr;

	root_para = (root_para_t *)GUI_WinGetAttr(parent);		
	mainmenu_para = (mainmenu_para_t *)GUI_WinGetAddData(parent);
	mainmenu_uipara = mainmenu_para->uipara;
	
	for(i = 0; i < mainmenu_uipara->mainmenu_para.all_item; i++)
	{
		if(mainmenu_uipara->mainmenu_para.frm_para[i].fid == fid)
			break;
	}
	
	if(lyr_type == MAINMENU_LYRTYPE_NORMAL)
	{
		rect.x		  = mainmenu_uipara->mainmenu_para.frm_para[i].posx;
		rect.y		  = mainmenu_uipara->mainmenu_para.frm_para[i].posy;
		rect.width	  = mainmenu_uipara->mainmenu_para.frm_para[i].posw;
		rect.height   = mainmenu_uipara->mainmenu_para.frm_para[i].posh;
	}
	else
	{
		rect.x		  = 0;
		rect.y		  = 0;
		rect.width	  = (mainmenu_uipara->mainmenu_para.frm_para[i].posw/80)*64;
		rect.height   = (mainmenu_uipara->mainmenu_para.frm_para[i].posh/74)*64;
	}
	
	__msg("%d,%d,%d,%d\n",rect.x,rect.y,rect.width,rect.height);

	////para
	frmwin_attr = (recordfrm_attr_t *)esMEMS_Malloc(0, sizeof(recordfrm_attr_t));
	if(frmwin_attr == NULL)
	{
		__wrn("malloc is error! \n");
		return NULL;
	}
	eLIBs_memset(frmwin_attr, 0, sizeof(recordfrm_attr_t));

	////init frm data para
	frmwin_attr->fid = fid;
	frmwin_attr->lyr_type = lyr_type;
	frmwin_attr->pwin = parent;
	frmwin_attr->cur_lyr = layer;
	
	frmwin_attr->fccolor = mainmenu_para->fccolor;
	frmwin_attr->ufcolor = mainmenu_para->ufcolor;
	frmwin_attr->bfont = root_para->font;
	frmwin_attr->sfont = root_para->font;

	frmwin_attr->icon_size.width = mainmenu_uipara->mainmenu_para.frm_para[i].iconw;
	frmwin_attr->icon_size.height = mainmenu_uipara->mainmenu_para.frm_para[i].iconh;
	frmwin_attr->frame_size.width = mainmenu_uipara->mainmenu_para.frm_para[i].posw;
	frmwin_attr->frame_size.height = mainmenu_uipara->mainmenu_para.frm_para[i].posh;

	frmwin_attr->res_normal.sprite_buf = mainmenu_para->sprite_buf;
	frmwin_attr->res_normal.focus_bmp = mainmenu_para->focus_bmp_hdl[i];
	frmwin_attr->res_normal.unfocus_bmp = mainmenu_para->unfocus_bmp_hdl[i];
	frmwin_attr->res_normal.item_str = mainmenu_para->item_str[i];		

	////creat frame win
	eLIBs_memset(&frame_info, 0, sizeof(__gui_framewincreate_para_t));	

	frame_info.name 			= "frame win";
	frame_info.id				= NULL;
	frame_info.dwStyle          = WS_VISIBLE;
	frame_info.hHosting         = parent;
	frame_info.hLayer			= layer;		
	frame_info.FrameWinProc     = cb_recordfrm_frm;
	frame_info.rect.x			= rect.x;
	frame_info.rect.y			= rect.y;
	frame_info.rect.width		= rect.width;
	frame_info.rect.height		= rect.height;
	frame_info.attr           = (void *)frmwin_attr;

	h_win = GUI_FrmWinCreate(&frame_info);
	if( !h_win )
	{
		__err("fwin create error ! \n");
	} 
	
	return h_win;
}


__s32 recordfrm_frm_delete(H_WIN frmwin)
{
	if(frmwin)
	{
		GUI_FrmWinDelete(frmwin);
	}	

	return EPDK_OK;
}


