

#include "lib_button.h"



static int Compare_ID(void *key, void *data)
{
	return *(int *)key - ((button_t *)data)->widget_info.id;
}



int Set_Button_Left_CallBack(__u32 button_ID,windows_t *Windows,__pCBK_t Button_Left)
{
	__u8 ret;

	button_t button;


	dlist_t *list = Windows->button_head;

	if(NULL == list)
	{
		eDbug("list is NULL \n");
		return EPDK_FAIL;
	}


	ret = Dlist_Find_Element(list,&button,&button_ID,esKRNL_GetCallBack(Compare_ID));
	if(-1 == ret)
	{
		return EPDK_FAIL;
	}

	button.button_info.button_attribute.button_action.left_action = Button_Left;

	ret = Dlist_Modify(list,&button,ret);
	if(-1 == ret)
	{
		return EPDK_FAIL;
	}

	return EPDK_OK;
}


int Set_Button_Right_CallBack(__u32 button_ID,windows_t *Windows,__pCBK_t Button_Right)
{
	__u8 ret;

	button_t button;

	dlist_t *list = Windows->button_head;

	if(NULL == list)
	{
		eDbug("list is NULL \n");
		return EPDK_FAIL;
	}


	ret = Dlist_Find_Element(list,&button,&button_ID,esKRNL_GetCallBack(Compare_ID));
	if(-1 == ret)
	{
		return EPDK_FAIL;
	}

	button.button_info.button_attribute.button_action.right_action = Button_Right;

	ret = Dlist_Modify(list,&button,ret);
	if(-1 == ret)
	{
		return EPDK_FAIL;
	}

	return EPDK_OK;
}


int Set_Button_Enter_CallBack(__u32 button_ID,windows_t *Windows,__pCBK_t Button_Enter)
{
	__u8 ret;

	button_t button;

	dlist_t *list = Windows->button_head;

	if(NULL == list)
	{
		eDbug("list is NULL \n");
		return EPDK_FAIL;
	}


	ret = Dlist_Find_Element(list,&button,&button_ID,esKRNL_GetCallBack(Compare_ID));
	if(-1 == ret)
	{
		return EPDK_FAIL;
	}

	button.button_info.button_attribute.button_action.enter_action = Button_Enter;


	ret = Dlist_Modify(list,&button,ret);
	if(-1 == ret)
	{
		return EPDK_FAIL;
	}

	return EPDK_OK;
}


int Set_Button_Return_CallBack(__u32 button_ID,windows_t *Windows,__pCBK_t Button_Return)
{
	__u8 ret;

	button_t button;

	dlist_t *list = Windows->button_head;

	if(NULL == list)
	{
		eDbug("list is NULL \n");
		return EPDK_FAIL;
	}


	ret = Dlist_Find_Element(list,&button,&button_ID,esKRNL_GetCallBack(Compare_ID));
	if(-1 == ret)
	{
		return EPDK_FAIL;
	}

	button.button_info.button_attribute.button_action.return_action = Button_Return;

	ret = Dlist_Modify(list,&button,ret);
	if(-1 == ret)
	{
		return EPDK_FAIL;
	}

	return EPDK_OK;
}



int Create_Button(__u32 button_id,H_WIN patent,button_t *button,button_ui_t *button_ui)
{
	int ret;

	ret = Get_Button_Source(&(button->button_info),button_id,button_ui);
	
	if(EPDK_FAIL == ret)
	{
		return EPDK_FAIL;
	}
	
	button->widget_info.id              = button_id;
	button->widget_info.hParent         = patent;
    button->widget_info.ctl_rect.x		= button_ui->button_location.x;
	button->widget_info.ctl_rect.y		= button_ui->button_location.y;
	button->widget_info.ctl_rect.width	= button_ui->button_location.width;
	button->widget_info.ctl_rect.height	= button_ui->button_location.height;
	button->widget_info.spClassName 	= CTRL_BUTTON;
	button->widget_info.dwStyle 		= WS_VISIBLE|WS_MEMDEV;
	button->widget_info.attr 			= &(button->button_info.button_attribute);
	
	button->ctrlwin = GUI_CtrlWinCreate(&(button->widget_info));

	if(NULL == button->ctrlwin)
	{
		eDbug("create button ctrlwin failed \n");
		return EPDK_FAIL;
	}
	
	eDbug("button->ctrlwin is 0x%x \n",button->ctrlwin);

	return EPDK_OK;
}


int Release_Button(button_t *button)
{
	int ret;

	ret = Relase_Button_Source(&(button->button_info),button->widget_info.id);

	return ret;
}


int Delete_Button(void *data)
{
	button_t *button = NULL;

	windows_t *Windows = NULL;

	button = (button_t *)data;

	if(NULL == button)
	{
		return EPDK_FAIL;
	}

	eDbug("delete button\n");


	Windows = (windows_t *)GUI_WinGetAttr(button->widget_info.hParent);
	
	if(NULL != Windows)
	{
		if(button == Windows->focus_button)
		{
			eDbug("free Windows->focus_button \n");

			Windows->focus_button = NULL;		
		}

		Windows = NULL; 		
	}

	
	if(NULL != button->pFirstHosted)
	{
		destory_Hosting_Win(&(button->pFirstHosted));
		
		button->pFirstHosted = NULL;
		
		button->FocusHosted = NULL;
		
	}
	
	if(NULL != button->ctrlwin)
	{
	    GUI_ClearRect(button->widget_info.ctl_rect.x,button->widget_info.ctl_rect.y,
	    button->widget_info.ctl_rect.x + button->widget_info.ctl_rect.width,
	    button->widget_info.ctl_rect.y + button->widget_info.ctl_rect.height); 

		eDbug("button->ctrlwin is 0x%x \n",button->ctrlwin);
		
		GUI_CtrlWinDelete(button->ctrlwin);
		
		button->ctrlwin = NULL;
	}
	
	Release_Button(button);

	In_Free(button,sizeof(button_t));

	return EPDK_OK;
}



int Delete_Hosted_Button(void *data)
{
	button_t *button = NULL;

	button = (button_t *)data;

	if(NULL == button)
	{
		return EPDK_FAIL;
	}

	eDbug("delete button\n");

	
	if(NULL != button->ctrlwin)
	{
        GUI_LyrWinSel(button->widget_info.hLayer);
        
	    GUI_ClearRect(button->widget_info.ctl_rect.x,button->widget_info.ctl_rect.y,
	    button->widget_info.ctl_rect.x + button->widget_info.ctl_rect.width,
	    button->widget_info.ctl_rect.y + button->widget_info.ctl_rect.height); 

		eDbug("button->ctrlwin is 0x%x \n",button->ctrlwin);
		
		GUI_CtrlWinDelete(button->ctrlwin);
		
		button->ctrlwin = NULL;
	}
	
	Release_Button(button);

	return EPDK_OK;
}



dlist_t *Create_Button_List(void)
{
	return Dlist_Create(sizeof(button_t),esKRNL_GetCallBack(Delete_Button));
}



int Register_Button(__u32 ID,windows_t *Windows,button_ui_t *button_ui)
{
	__u8 ret;
	
	button_t *button = NULL;

	button = In_Malloc(sizeof(button_t));
	if(NULL == button)
	{
		eDbug("malloc is failed \n");
		return EPDK_FAIL;
	}
		
	ret = Create_Button(ID,Windows->framewin,button,button_ui);
	if(EPDK_FAIL == ret)
	{
		eDbug("create button failed \n");
		return EPDK_FAIL;
	}
	
	Dlist_Append(Windows->button_head,button);

	return EPDK_OK;

}


int Register_Button_HostedFrom_Button(__u32 ID,windows_t *Windows,button_ui_t *button_ui)
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


	ret = Create_Button(ID,Windows->focus_button->widget_info.hParent,&(mixctrl->mix_ctrl.Button),button_ui);

	if(EPDK_FAIL == ret)
	{
		eDbug("create button failed \n");
		return EPDK_FAIL;
	}

	
	mixctrl->ctrlwin = mixctrl->mix_ctrl.Button.ctrlwin;

	if(NULL == mixctrl->ctrlwin)
	{
		eDbug("register a button hostfrom button is failde \n");
		return EPDK_FAIL;		
	}

	eDbug("mixctrl->ctrlwin is 0x%x \n",mixctrl->ctrlwin);
	
	mixctrl->type_ctrl = TYPE_BUTTON;

	mixctrl->ID        = ID;
	
	mixctrl->destory   = esKRNL_GetCallBack(Delete_Hosted_Button);

		
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



