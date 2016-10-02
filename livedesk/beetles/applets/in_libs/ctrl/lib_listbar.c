
#include "lib_listbar.h"




static int Compare_ID(void *key, void *data)
{
	return *(int *)key - ((listbar_t *)data)->widget_info.id;
}



int Set_Listbar_HostedFrom_Button_Left_CallBack(__u32 Listbar_ID,windows_t *Windows,__pCBK_t Listbar_Left)
{
	__u8 ret;

	Mixed_CtrlWin_T *pHosted = NULL;

	
	if(NULL == Windows)
	{
		eDbug("Windows is NULL \n");
		return EPDK_FAIL;
	}

	if(NULL == Windows->button_head)
	{
		eDbug("Windows->button_head is NULL \n");
		return EPDK_FAIL;
	}


	if(NULL == Windows->focus_button)
	{
		eDbug("Windows->focus_button is NULL \n");
		return EPDK_FAIL;
	}

	if(NULL == Windows->focus_button->pFirstHosted)
	{
		eDbug("Windows->focus_button->pFirstHosted is NULL \n");
		return EPDK_FAIL;
	}


	pHosted = Find_Hosted_Win(Windows->focus_button->pFirstHosted,Listbar_ID);

	if(NULL == pHosted)
	{
		eDbug("can't find the hosted win \n");
		return EPDK_FAIL;
	}


	if(TYPE_LISTBAR != pHosted->type_ctrl)
	{
		eDbug("the ctrlwin type is error \n");
		return EPDK_FAIL;
	}


	pHosted->mix_ctrl.Listbar.listbar_info.listbar_attribute.statics_action.left_action = Listbar_Left;


	return EPDK_OK;
}




int Set_Listbar_HostedFrom_Button_Right_CallBack(__u32 Listbar_ID,windows_t *Windows,__pCBK_t Listbar_Right)
{
	__u8 ret;

	Mixed_CtrlWin_T *pHosted = NULL;

	
	if(NULL == Windows)
	{
		eDbug("Windows is NULL \n");
		return EPDK_FAIL;
	}

	if(NULL == Windows->button_head)
	{
		eDbug("Windows->button_head is NULL \n");
		return EPDK_FAIL;
	}


	if(NULL == Windows->focus_button)
	{
		eDbug("Windows->focus_button is NULL \n");
		return EPDK_FAIL;
	}

	if(NULL == Windows->focus_button->pFirstHosted)
	{
		eDbug("Windows->focus_button->pFirstHosted is NULL \n");
		return EPDK_FAIL;
	}


	pHosted = Find_Hosted_Win(Windows->focus_button->pFirstHosted,Listbar_ID);

	if(NULL == pHosted)
	{
		eDbug("can't find the hosted win \n");
		return EPDK_FAIL;
	}


	if(TYPE_LISTBAR != pHosted->type_ctrl)
	{
		eDbug("the ctrlwin type is error \n");
		return EPDK_FAIL;
	}


	pHosted->mix_ctrl.Listbar.listbar_info.listbar_attribute.statics_action.right_action = Listbar_Right;


	return EPDK_OK;
}





int Set_Listbar_HostedFrom_Button_Enter_CallBack(__u32 Listbar_ID,windows_t *Windows,__pCBK_t Listbar_Enter)
{
	__u8 ret;

	Mixed_CtrlWin_T *pHosted = NULL;

	
	if(NULL == Windows)
	{
		eDbug("Windows is NULL \n");
		return EPDK_FAIL;
	}


	if(NULL == Windows->button_head)
	{
		eDbug("Windows->button_head is NULL \n");
		return EPDK_FAIL;
	}


	if(NULL == Windows->focus_button)
	{
		eDbug("Windows->focus_button is NULL \n");
		return EPDK_FAIL;
	}

	if(NULL == Windows->focus_button->pFirstHosted)
	{
		eDbug("Windows->focus_button->pFirstHosted is NULL \n");
		return EPDK_FAIL;
	}


	pHosted = Find_Hosted_Win(Windows->focus_button->pFirstHosted,Listbar_ID);

	if(NULL == pHosted)
	{
		eDbug("can't find the hosted win \n");
		return EPDK_FAIL;
	}


	if(TYPE_LISTBAR != pHosted->type_ctrl)
	{
		eDbug("the ctrlwin type is error \n");
		return EPDK_FAIL;
	}


	pHosted->mix_ctrl.Listbar.listbar_info.listbar_attribute.statics_action.enter_action = Listbar_Enter;


	return EPDK_OK;
}



int Set_Listbar_HostedFrom_Button_Return_CallBack(__u32 Listbar_ID,windows_t *Windows,__pCBK_t Listbar_Return)
{
	__u8 ret;

	Mixed_CtrlWin_T *pHosted = NULL;

	
	if(NULL == Windows)
	{
		eDbug("Windows is NULL \n");
		return EPDK_FAIL;
	}

	if(NULL == Windows->button_head)
	{
		eDbug("Windows->button_head is NULL \n");
		return EPDK_FAIL;
	}


	if(NULL == Windows->focus_button)
	{
		eDbug("Windows->focus_button is NULL \n");
		return EPDK_FAIL;
	}

	if(NULL == Windows->focus_button->pFirstHosted)
	{
		eDbug("Windows->focus_button->pFirstHosted is NULL \n");
		return EPDK_FAIL;
	}


	pHosted = Find_Hosted_Win(Windows->focus_button->pFirstHosted,Listbar_ID);

	if(NULL == pHosted)
	{
		eDbug("can't find the hosted win \n");
		return EPDK_FAIL;
	}


	if(TYPE_LISTBAR != pHosted->type_ctrl)
	{
		eDbug("the ctrlwin type is error \n");
		return EPDK_FAIL;
	}


	pHosted->mix_ctrl.Listbar.listbar_info.listbar_attribute.statics_action.return_action = Listbar_Return;


	return EPDK_OK;
}



int Init_Listbar_Source(listbar_info_t *listbar_info)
{

	listbar_info->listbar_attribute.statics_paint.text          = NULL;
	listbar_info->listbar_attribute.statics_paint.text_pos.x    = 0;
	listbar_info->listbar_attribute.statics_paint.text_pos.y    = 0;		
	listbar_info->listbar_attribute.statics_paint.ftxt_color    = GUI_RED;
	listbar_info->listbar_attribute.statics_paint.uftxt_color   = GUI_BLACK;
		
	listbar_info->listbar_attribute.statics_paint.draw_code     = EPDK_CHARSET_ENM_UTF8;
	listbar_info->listbar_attribute.statics_paint.draw_font     = SWFFont;
	listbar_info->listbar_attribute.statics_paint.alpha_en      = 0;
	listbar_info->listbar_attribute.statics_paint.bk_color      = 0;


	return EPDK_OK;
}




static __s32 Listbar_Draw_CB(__lbar_draw_para_t *draw_param)
{
	listbar_draw_t *draw_t = NULL;

 	char string_buf[64] = {0};

	draw_t = (listbar_draw_t *)(draw_param->attr);


 	eLIBs_memset(string_buf,0,sizeof(string_buf));

 	
	switch(draw_param->mode)
	{
		case LBAR_MODE_NORMAL:
		{
			GUI_BMP_Draw(theme_hdl2buf(draw_t->unfocus_bmp),draw_param->rect.x,draw_param->rect.y);

			GUI_SetColor(draw_t->unfocus_txt_color);

			dsk_langres_get_menu_text(draw_t->text[draw_param->index],string_buf,sizeof(string_buf));
			
			GUI_DispStringAt(string_buf,draw_param->rect.x,draw_param->rect.y);    
		}
		break;
		
		case LBAR_MODE_FOCUS:
		{
			GUI_BMP_Draw(theme_hdl2buf(draw_t->focus_bmp),draw_param->rect.x,draw_param->rect.y);
			
			GUI_SetColor(draw_t->focus_txt_color);

			dsk_langres_get_menu_text(draw_t->text[draw_param->index],string_buf,sizeof(string_buf));
			
			GUI_DispStringAt(string_buf,draw_param->rect.x,draw_param->rect.y);    

			draw_t->Index = draw_param->index;
			 			
		}		
		break;

		
		default:
			break;
			
	}
	GUI_LyrWinCacheOff();

	return EPDK_OK;
}





int Create_Listbar(__u32 Listbar_id,H_WIN patent,listbar_t *listbar,listbar_ui_t *listbar_ui,__u8 focus_ID)
{
	int ret;


	ret = Get_Listbar_Source(listbar,Listbar_id,listbar_ui,focus_ID);
	
	if(EPDK_FAIL == ret)
	{
		return EPDK_FAIL;
	}
	

	listbar->listbar_draw.text                     = listbar_ui->string_ID;
	
	listbar->listbar_draw.listbar_ID               = Listbar_id;
	
	listbar->listbar_draw.parent                   = patent;

	
	listbar->h_listbar                             = LISTBAR_Create(patent);
	
	if(NULL == listbar->h_listbar)
	{
		eDbug("listbar create fail\n");
		return EPDK_FAIL;
	}


	eDbug("listbar->h_listbar is 0x%x \n",listbar->h_listbar);
   
	ret = LISTBAR_Config(listbar->h_listbar,Listbar_Draw_CB,&(listbar->listbar_config) );  

	if(EPDK_FAIL == ret)
	{
		eDbug("listbar config is failed \n");
		return EPDK_FAIL;
	}


	return EPDK_OK;
}



int Create_Listbar_CtrlWin(__u32 listbar_id,H_WIN patent,listbar_t *listbar,listbar_ui_t *listbar_ui)
{
	int ret;

	ret = Init_Listbar_Source(&(listbar->listbar_info));
	
	if(EPDK_FAIL == ret)
	{
		eDbug("Get Statics Source failed \n");
		return EPDK_FAIL;
	}

	
	listbar->widget_info.id               = listbar_id;
	listbar->widget_info.hParent          = patent;
    listbar->widget_info.ctl_rect.x		  = listbar_ui->listbar_location.x;
	listbar->widget_info.ctl_rect.y		  = listbar_ui->listbar_location.y;
	listbar->widget_info.ctl_rect.width	  = listbar_ui->listbar_location.width;
	listbar->widget_info.ctl_rect.height  = listbar_ui->listbar_location.height;
	listbar->widget_info.spClassName 	  = CTRL_STATIC;
	listbar->widget_info.dwExStyle 	      = WS_NONE; 	
	listbar->widget_info.dwStyle 		  = WS_DISABLED;
	listbar->widget_info.spCaption	      = NULL; 		
	listbar->widget_info.attr 			  = &(listbar->listbar_info.listbar_attribute);

	listbar->ctrlwin = GUI_CtrlWinCreate(&(listbar->widget_info));
	
	if(NULL == listbar->ctrlwin)
	{
		eDbug("create listbar ctrlwin failed \n");
		return EPDK_FAIL;
	}

	
	eDbug("listbar->ctrlwin is 0x%x \n",listbar->ctrlwin);

	return EPDK_OK;
}




int Release_Listbar(listbar_t *listbar)
{
	int ret;

	ret = Relase_Listbar_Source(listbar,listbar->widget_info.id);

	return ret;
}


int Delete_Listbar(void *data)
{
	listbar_t *listbar = NULL;

	windows_t *Windows = NULL;

	__u8 ret;

	listbar = (listbar_t *)data;

	if(NULL == listbar)
	{
		eDbug("listbar is NULL \n");
		return EPDK_FAIL;
	}


	Windows = (windows_t *)GUI_WinGetAttr(listbar->widget_info.hParent);
	
	if(NULL != Windows)
	{
		if(listbar == Windows->focus_listbar)
		{
			eDbug("free Windows->focus_listbar \n");

			Windows->focus_listbar = NULL;		
		}

		Windows = NULL; 		
	}

	
	ret = LISTBAR_SuspendPage(listbar->h_listbar);
	if(EPDK_FAIL == ret)
	{
		eDbug("listbar suspend page failed \n");
		return EPDK_FAIL;
	}



	ret = LISTBAR_Delete(listbar->h_listbar);
	if(EPDK_FAIL == ret)
	{
		eDbug("delete listbar failed \n");
		return EPDK_FAIL;
	}
	listbar->h_listbar = NULL;


	
	ret = Release_Listbar(listbar);
	if(EPDK_FAIL == ret)
	{
		eDbug("release listbar failed \n");
		return EPDK_FAIL;		
	}


	if(NULL != listbar->ctrlwin)
	{
        GUI_LyrWinSel(listbar->widget_info.hLayer);
        
	    GUI_ClearRect(listbar->widget_info.ctl_rect.x,listbar->widget_info.ctl_rect.y,
	    listbar->widget_info.ctl_rect.x + listbar->widget_info.ctl_rect.width,
	    listbar->widget_info.ctl_rect.y + listbar->widget_info.ctl_rect.height); 
		
		GUI_CtrlWinDelete(listbar->ctrlwin);
		
		listbar->ctrlwin = NULL;
	}



	In_Free(listbar,sizeof(listbar_t));


	return EPDK_OK;

}




int Delete_Hosted_Listbar(void *data)
{
	__u8 ret;

	listbar_t *listbar = NULL;

	listbar = (listbar_t *)data;

	if(NULL == listbar)
	{
		return EPDK_FAIL;
	}

	eDbug("delete listbar \n");


	ret = LISTBAR_SuspendPage(listbar->h_listbar);
	if(EPDK_FAIL == ret)
	{
		eDbug("listbar suspend page failed \n");
		return EPDK_FAIL;
	}

	ret = LISTBAR_Delete(listbar->h_listbar);
	if(EPDK_FAIL == ret)
	{
		eDbug("delete listbar failed \n");
		return EPDK_FAIL;
	}
	listbar->h_listbar = NULL;


	ret = Release_Listbar(listbar);
	if(EPDK_FAIL == ret)
	{
		eDbug("release listbar failed \n");
		return EPDK_FAIL;		
	}

	
	if(NULL != listbar->ctrlwin)
	{	
        GUI_LyrWinSel(listbar->widget_info.hLayer);

	    GUI_ClearRect(listbar->widget_info.ctl_rect.x,listbar->widget_info.ctl_rect.y,
	    listbar->widget_info.ctl_rect.x + listbar->widget_info.ctl_rect.width,
	    listbar->widget_info.ctl_rect.y + listbar->widget_info.ctl_rect.height); 

		eDbug("listbar->ctrlwin is 0x%x \n",listbar->ctrlwin);
		
		GUI_CtrlWinDelete(listbar->ctrlwin);
		
		listbar->ctrlwin = NULL;
	}
	

	return EPDK_OK;
}




dlist_t *Create_Listbar_List(void)
{
	return Dlist_Create(sizeof(listbar_t),esKRNL_GetCallBack(Delete_Listbar));
}



int Register_Listbar(__u32 ID,windows_t *Windows,listbar_ui_t *listbar_ui,__u8 focus_ID)
{
	__u8 ret;
	
	listbar_t *listbar = NULL;

	listbar = In_Malloc(sizeof(listbar_t));
	if(NULL == listbar)
	{
		eDbug("malloc is failed \n");
		return EPDK_FAIL;
	}


	ret = Create_Listbar_CtrlWin(ID,Windows->framewin,listbar,listbar_ui);
	if(EPDK_FAIL == ret)
	{
		eDbug("create listbar ctrlwin failed \n");
		return EPDK_FAIL;
	}
	

	
	ret = Create_Listbar(ID,Windows->framewin,listbar,listbar_ui,focus_ID);
	if(EPDK_FAIL == ret)
	{
		eDbug("create listbar failed \n");
		return EPDK_FAIL;
	}
	
	Dlist_Append(Windows->listbar_head,listbar);

	return EPDK_OK;

}




int Register_Listbar_HostedFrom_Button(__u32 ID,windows_t *Windows,listbar_ui_t *listbar_ui,__u8 focus_ID)
{
	__u8 ret;

	
	Mixed_CtrlWin_T *mixctrl = NULL;


	if(NULL == Windows)
	{
		eDbug("Windows is NULL \n");
		return EPDK_FAIL;
	}


	if(NULL == Windows->button_head)
	{
		eDbug("Windows->button_head is NULL \n");
		return EPDK_FAIL;
	}


	if(NULL == Windows->focus_button)
	{
		eDbug("Windows->focus_button is NULL \n");
		return EPDK_FAIL;
	}


	mixctrl = In_Malloc(sizeof(Mixed_CtrlWin_T));
	if(NULL == mixctrl)
	{
		eDbug("malloc mixctrl is failed \n");
		return EPDK_FAIL;
	}



	ret = Create_Listbar_CtrlWin(ID,Windows->focus_button->widget_info.hParent,&(mixctrl->mix_ctrl.Listbar),listbar_ui);
	if(EPDK_FAIL == ret)
	{
		eDbug("create listbar ctrlwin failed \n");
		return EPDK_FAIL;
	}
	


	
	ret = Create_Listbar(ID,Windows->focus_button->widget_info.hParent,&(mixctrl->mix_ctrl.Listbar),listbar_ui,focus_ID);
	if(EPDK_FAIL == ret)
	{
		eDbug("create listbar failed \n");
		return EPDK_FAIL;
	}

	
	mixctrl->ctrlwin = mixctrl->mix_ctrl.Listbar.ctrlwin;
	
	if(NULL == mixctrl->ctrlwin)
	{
		eDbug("register a Listbar hostfrom button is failde \n");
		return EPDK_FAIL;		
	}

	eDbug("mixctrl->ctrlwin is 0x%x \n",mixctrl->ctrlwin);
	
	mixctrl->type_ctrl = TYPE_LISTBAR;

	mixctrl->ID        = ID;
	
	mixctrl->destory   = esKRNL_GetCallBack(Delete_Hosted_Listbar);

		
	ret = Add_New_Hosted_Win(&(Windows->focus_button->pFirstHosted),mixctrl);
	if(EPDK_FAIL == ret)
	{
		eDbug("Add_New_Hosted_Win is failed \n");
		return EPDK_FAIL;
	}

	eDbug("button->pFirstHosted is 0x%x \n",Windows->focus_button->pFirstHosted);

	Windows->focus_button->FocusHosted = mixctrl;

	GUI_WinSetFocusChild(mixctrl->ctrlwin);


	esKRNL_TimeDly(5);					
	
    ret = LISTBAR_ShowPage(mixctrl->mix_ctrl.Listbar.h_listbar);     
	if(EPDK_FAIL == ret)
	{
		eDbug("listbar show page is failed \n");
		return EPDK_FAIL;
	}

	
	return EPDK_OK;

}

