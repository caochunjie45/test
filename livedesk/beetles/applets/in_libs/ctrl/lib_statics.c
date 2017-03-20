
#include "lib_statics.h"




static int Compare_ID(void *key, void *data)
{
	return *(int *)key - ((statics_t *)data)->widget_info.id;
}



int Set_Statics_Bmp(__u32 statics_ID,windows_t *Windows,__u32 bmp_index)
{
	__u8 ret;

	statics_t statics;

	void *bmp = NULL;
	
	dlist_t *list = Windows->statics_head;

	if(NULL == list)
	{
		eDbug("list is NULL \n");
		return EPDK_FAIL;
	}


	ret = Dlist_Find_Element(list,&statics,&statics_ID,esKRNL_GetCallBack(Compare_ID));
	if(-1 == ret)
	{
		return EPDK_FAIL;
	}

	bmp = theme_hdl2buf(statics.statics_info.bmp_data[bmp_index]);
	
	GUI_StaticSetUnFocusBmp(statics.ctrlwin,bmp);

	

	return EPDK_OK;

}


int Set_Statics_Text(__u32 statics_ID,windows_t *Windows,char *text,__u32 text_index)
{
	__u8 ret;

	statics_t statics;
	
	dlist_t *list = Windows->statics_head;

	if(NULL == list)
	{
		eDbug("list is NULL \n");
		return EPDK_FAIL;
	}


	ret = Dlist_Find_Element(list,&statics,&statics_ID,esKRNL_GetCallBack(Compare_ID));
	if(-1 == ret)
	{
		return EPDK_FAIL;
	}


	eLIBs_strncpy(statics.statics_info.strings[text_index],text,STRING_MAX);
	
	GUI_StaticSetText(statics.ctrlwin,statics.statics_info.strings[text_index]);

	
	return EPDK_OK;

}



int Create_Statics(__u32 statics_id,H_WIN patent,statics_t *statics,statics_ui_t *statics_ui)
{
	int ret;

	ret = Get_Statics_Source(&(statics->statics_info),statics_id,statics_ui);
	
	if(EPDK_FAIL == ret)
	{
		eDbug("Get Statics Source failed \n");
		return EPDK_FAIL;
	}
	
	statics->widget_info.id               = statics_id;
	statics->widget_info.hParent          = patent;
    statics->widget_info.ctl_rect.x		  = statics_ui->statics_location.x;
	statics->widget_info.ctl_rect.y		  = statics_ui->statics_location.y;
	statics->widget_info.ctl_rect.width	  = statics_ui->statics_location.width;
	statics->widget_info.ctl_rect.height  = statics_ui->statics_location.height;
	statics->widget_info.spClassName 	  = CTRL_STATIC;
	statics->widget_info.dwExStyle 	      = WS_NONE; 	
	statics->widget_info.dwStyle 		  = WS_VISIBLE;
	statics->widget_info.spCaption	      = NULL; 		
	statics->widget_info.attr 			  = &(statics->statics_info.statics_attribute);
	
	statics->ctrlwin = GUI_CtrlWinCreate(&(statics->widget_info));
	if(NULL == statics->ctrlwin)
	{
		eDbug("create statics ctrlwin failed \n");
		return EPDK_FAIL;
	}
	eDbug("statics->ctrlwin is 0x%x \n",statics->ctrlwin);

	return EPDK_OK;
}


int Release_Statics(statics_t *statics)
{
	int ret;

	ret = Relase_Statics_Source(&(statics->statics_info),statics->widget_info.id);

	return ret;
}



int Delete_Statics(void *data)
{
	statics_t *statics = NULL;

	windows_t *Windows = NULL;

	statics = (statics_t *)data;

	if(NULL == statics)
	{
		return EPDK_FAIL;
	}


	Windows = (windows_t *)GUI_WinGetAttr(statics->widget_info.hParent);
	
	if(NULL != Windows)
	{
		if(statics == Windows->focus_statics)
		{
			eDbug("free Windows->focus_statics \n");

			Windows->focus_statics = NULL;		
		}

		Windows = NULL; 		
	}

	
	if(NULL != statics->ctrlwin)
	{
        GUI_LyrWinSel(statics->widget_info.hLayer);

	    GUI_ClearRect(statics->widget_info.ctl_rect.x,statics->widget_info.ctl_rect.y,
	    statics->widget_info.ctl_rect.x + statics->widget_info.ctl_rect.width,
	    statics->widget_info.ctl_rect.y + statics->widget_info.ctl_rect.height); 


		GUI_CtrlWinDelete(statics->ctrlwin);
		
		statics->ctrlwin = NULL;
	}

	Release_Statics(statics);

	
	In_Free(statics,sizeof(statics_t));

	return EPDK_OK;
}




int Delete_Hosted_Statics(void *data)
{
	statics_t *statics = NULL;

	statics = (statics_t *)data;

	if(NULL == statics)
	{
		return EPDK_FAIL;
	}

	eDbug("delete statics \n");

	
	if(NULL != statics->ctrlwin)
	{
        GUI_LyrWinSel(statics->widget_info.hLayer);
        
	    GUI_ClearRect(statics->widget_info.ctl_rect.x,statics->widget_info.ctl_rect.y,
	    statics->widget_info.ctl_rect.x + statics->widget_info.ctl_rect.width,
	    statics->widget_info.ctl_rect.y + statics->widget_info.ctl_rect.height); 

		eDbug("statics->ctrlwin is 0x%x \n",statics->ctrlwin);
		
		GUI_CtrlWinDelete(statics->ctrlwin);
		
		statics->ctrlwin = NULL;
	}

	
	Release_Statics(statics);
	

	return EPDK_OK;
}





dlist_t *Create_Statics_List(void)
{
	return Dlist_Create(sizeof(statics_t),esKRNL_GetCallBack(Delete_Statics));
}



int Register_Statics(__u32 ID,windows_t *Windows,statics_ui_t *statics_ui)
{
	__u8 ret;
	
	statics_t *statics  = NULL;


	ret = Dlist_Find(Windows->statics_head,&ID,esKRNL_GetCallBack(Compare_ID));
	if(EPDK_FAIL != ret)
	{
		eDbug("statics have been register \n");
		return EPDK_OK;
	}


	statics = In_Malloc(sizeof(statics_t));
	if(NULL == statics)
	{
		eDbug("malloc is failed \n");
		return EPDK_FAIL;
	}
	
	ret = Create_Statics(ID,Windows->framewin,statics,statics_ui);
	if(EPDK_FAIL == ret)
	{
		eDbug("create statics failed \n");
		return EPDK_FAIL;
	}
	
	Dlist_Append(Windows->statics_head,statics);

	return EPDK_OK;

}





int Register_Statics_HostedFrom_Button(__u32 ID,windows_t *Windows,statics_ui_t *statics_ui)
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
		eDbug("malloc is failed \n");
		return EPDK_FAIL;
	}


	ret = Create_Statics(ID,Windows->focus_button->widget_info.hParent,&(mixctrl->mix_ctrl.Statics),statics_ui);
	if(EPDK_FAIL == ret)
	{
		eDbug("create statics failed \n");
		return EPDK_FAIL;
	}

	
	mixctrl->ctrlwin = mixctrl->mix_ctrl.Statics.ctrlwin;
	
	if(NULL == mixctrl->ctrlwin)
	{
		eDbug("register a statics hostfrom button is failed \n");
		return EPDK_FAIL;		
	}

	eDbug("mixctrl->ctrlwin is 0x%x \n",mixctrl->ctrlwin);

	
	mixctrl->type_ctrl = TYPE_STATICS;

	mixctrl->ID        = ID;
	
	mixctrl->destory   = esKRNL_GetCallBack(Delete_Hosted_Statics);

		
	ret = Add_New_Hosted_Win(&(Windows->focus_button->pFirstHosted),mixctrl);
	if(EPDK_FAIL == ret)
	{
		eDbug("Add_New_Hosted_Win is failed \n");
		return EPDK_FAIL;
	}

	eDbug("button->pFirstHosted is 0x%x \n",Windows->focus_button->pFirstHosted);


	Windows->focus_button->FocusHosted = mixctrl;


	GUI_WinSetFocusChild(mixctrl->ctrlwin);

	
	return EPDK_OK;

}


