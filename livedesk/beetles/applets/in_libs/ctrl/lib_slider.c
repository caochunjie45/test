
#include "lib_slider.h"



static int Compare_ID(void *key, void *data)
{
	return *(int *)key - ((slider_t *)data)->widget_info.id;
}


int Set_Slider_Return_CallBack(__u32 slider_ID,windows_t *Windows,__pCBK_t Slider_Return)
{
	__u8 ret;

	slider_t slider;

	dlist_t *list = Windows->slider_head;

	if(NULL == list)
	{
		eDbug("list is NULL \n");
		return EPDK_FAIL;
	}


	ret = Dlist_Find_Element(list,&slider,&slider_ID,esKRNL_GetCallBack(Compare_ID));
	if(-1 == ret)
	{
		return EPDK_FAIL;
	}

	slider.slider_info.slider_attribute.slider_action.return_action = Slider_Return;


	ret = Dlist_Modify(list,&slider,ret);
	if(-1 == ret)
	{
		return EPDK_FAIL;
	}

	return EPDK_OK;
}


int Set_Slider_Enter_CallBack(__u32 slider_ID,windows_t *Windows,__pCBK_t Slider_Enter)
{
	__u8 ret;

	slider_t slider;

	dlist_t *list = Windows->slider_head;

	if(NULL == list)
	{
		eDbug("list is NULL \n");
		return EPDK_FAIL;
	}


	ret = Dlist_Find_Element(list,&slider,&slider_ID,esKRNL_GetCallBack(Compare_ID));
	if(-1 == ret)
	{
		return EPDK_FAIL;
	}

	slider.slider_info.slider_attribute.slider_action.enter_action = Slider_Enter;


	ret = Dlist_Modify(list,&slider,ret);
	if(-1 == ret)
	{
		return EPDK_FAIL;
	}

	return EPDK_OK;
}




int Set_Slider_HostedFrom_Button_Return_CallBack(__u32 slider_ID,windows_t *Windows,__pCBK_t Slider_Return)
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

	pHosted = Find_Hosted_Win(Windows->focus_button->pFirstHosted,slider_ID);

	if(NULL == pHosted)
	{
		eDbug("can't find the hosted win \n");
		return EPDK_FAIL;
	}


	if(TYPE_SLIDER != pHosted->type_ctrl)
	{
		eDbug("the ctrlwin type is error \n");
		return EPDK_FAIL;
	}


	pHosted->mix_ctrl.Slider.slider_info.slider_attribute.slider_action.return_action = Slider_Return;


	return EPDK_OK;
}




int Create_Slider(__u32 slider_id,H_WIN patent,slider_t *slider,slider_ui_t *slider_ui)
{
	int ret;

	ret = Get_Slider_Source(&(slider->slider_info),slider_id,slider_ui);
	
	if(EPDK_FAIL == ret)
	{
		eDbug("Get Slider Source failed \n");
		return EPDK_FAIL;
	}
	
	slider->widget_info.id              = slider_id;
	slider->widget_info.hParent         = patent;
    slider->widget_info.ctl_rect.x		= slider_ui->slider_location.x;
	slider->widget_info.ctl_rect.y		= slider_ui->slider_location.y;
	slider->widget_info.ctl_rect.width	= slider_ui->slider_location.width;
	slider->widget_info.ctl_rect.height	= slider_ui->slider_location.height;
	slider->widget_info.spClassName 	= CTRL_SLIDER;
	slider->widget_info.dwStyle 		= WS_VISIBLE|SDS_RESPOND_OUTDEV;
	slider->widget_info.dwExStyle       = 0;
	slider->widget_info.attr 			= &(slider->slider_info.slider_attribute);
	
	slider->ctrlwin = GUI_CtrlWinCreate(&(slider->widget_info));
	if(NULL == slider->ctrlwin)
	{
		eDbug("create slider ctrlwin failed \n");
		return EPDK_FAIL;		
	}
	eDbug("slider->ctrlwin is 0x%x \n",slider->ctrlwin);
	
	return EPDK_OK;
}


int Release_Slider(slider_t *slider)
{
	int ret;

	ret = Relase_Slider_Source(&(slider->slider_info),slider->widget_info.id);

	return ret;
}


int Delete_Slider(void *data)
{
	slider_t *slider = NULL;

	windows_t *Windows = NULL;

	slider = (slider_t *)data;

	if(NULL == slider)
	{
		return EPDK_FAIL;
	}


	Windows = (windows_t *)GUI_WinGetAttr(slider->widget_info.hParent);
	
	if(NULL != Windows)
	{
		if(slider == Windows->focus_slider)
		{
			eDbug("free Windows->focus_slider \n");

			Windows->focus_slider = NULL;		
		}

		Windows = NULL; 		
	}

	
	if(NULL != slider->ctrlwin)
	{
        GUI_LyrWinSel(slider->widget_info.hLayer);

	    GUI_ClearRect(slider->widget_info.ctl_rect.x,slider->widget_info.ctl_rect.y,
	    slider->widget_info.ctl_rect.x + slider->widget_info.ctl_rect.width,
	    slider->widget_info.ctl_rect.y + slider->widget_info.ctl_rect.height); 

		GUI_CtrlWinDelete(slider->ctrlwin);	
		
		slider->ctrlwin = NULL;
	}

	
	Release_Slider(slider);
	
	In_Free(slider,sizeof(slider_t));


	return EPDK_OK;

}





int Delete_Hosted_Slider(void *data)
{
	slider_t *slider = NULL;

	slider = (slider_t *)data;

	if(NULL == slider)
	{
		return EPDK_FAIL;
	}

	eDbug("delete slider \n");

	
	if(NULL != slider->ctrlwin)
	{
        GUI_LyrWinSel(slider->widget_info.hLayer);
        
	    GUI_ClearRect(slider->widget_info.ctl_rect.x,slider->widget_info.ctl_rect.y,
	    slider->widget_info.ctl_rect.x + slider->widget_info.ctl_rect.width,
	    slider->widget_info.ctl_rect.y + slider->widget_info.ctl_rect.height); 

		eDbug("slider->ctrlwin is 0x%x \n",slider->ctrlwin);
		
		GUI_CtrlWinDelete(slider->ctrlwin);	
		slider->ctrlwin = NULL;
	}
	
	Release_Slider(slider);
	
	return EPDK_OK;
}




dlist_t *Create_Slider_List(void)
{
	return Dlist_Create(sizeof(slider_t),esKRNL_GetCallBack(Delete_Slider));
}



int Register_Slider(__u32 ID,windows_t *Windows,slider_ui_t *slider_ui)
{
	__u8 ret;
	
	slider_t *slider = NULL;

	ret = Dlist_Find(Windows->slider_head,&ID,esKRNL_GetCallBack(Compare_ID));
	if(EPDK_FAIL != ret)
	{
		eDbug("slider have been register \n");
		return EPDK_OK;
	}


	slider = In_Malloc(sizeof(slider_t));
	if(NULL == slider)
	{
		return EPDK_FAIL;
	}
	
	ret = Create_Slider(ID,Windows->framewin,slider,slider_ui);
	if(EPDK_FAIL == ret)
	{
		eDbug("create slider failed \n");
		return EPDK_FAIL;
	}
	
	ret = SLIDER_SetRange(slider->ctrlwin, (__s32)0, (__s32)30);
	if(EPDK_FAIL == ret)
	{
		eDbug("slider SetRange failed \n");
		return EPDK_FAIL;
	}
	
	ret = SLIDER_SetLineSize(slider->ctrlwin,10);
	if(EPDK_FAIL == ret)
	{
		eDbug("slider SetLineSize failed \n");
		return EPDK_FAIL;
	}
	
	ret = SLIDER_SetValue(slider->ctrlwin,3);
	if(EPDK_FAIL == ret)
	{
		eDbug("slider SetValue failed \n");
		return EPDK_FAIL;
	}

	Dlist_Append(Windows->slider_head,slider);

	return EPDK_OK;

}






int Register_Slider_HostedFrom_Button(__u32 ID,windows_t *Windows,slider_ui_t *slider_ui)
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


	ret = Create_Slider(ID,Windows->focus_button->widget_info.hParent,&(mixctrl->mix_ctrl.Slider),slider_ui);
	if(EPDK_FAIL == ret)
	{
		eDbug("create slider failed \n");
		return EPDK_FAIL;
	}

	ret = SLIDER_SetRange(mixctrl->mix_ctrl.Slider.ctrlwin, (__s32)0, (__s32)30);
	if(EPDK_FAIL == ret)
	{
		eDbug("slider SetRange failed \n");
		return EPDK_FAIL;
	}
	
	ret = SLIDER_SetLineSize(mixctrl->mix_ctrl.Slider.ctrlwin,10);
	if(EPDK_FAIL == ret)
	{
		eDbug("slider SetLineSize failed \n");
		return EPDK_FAIL;
	}
	
	ret = SLIDER_SetValue(mixctrl->mix_ctrl.Slider.ctrlwin,10);
	if(EPDK_FAIL == ret)
	{
		eDbug("slider SetValue failed \n");
		return EPDK_FAIL;
	}

	
	mixctrl->ctrlwin = mixctrl->mix_ctrl.Slider.ctrlwin;
	
	if(NULL == mixctrl->ctrlwin)
	{
		eDbug("register a slider hostfrom button is failed \n");
		return EPDK_FAIL;		
	}

	eDbug("mixctrl->ctrlwin is 0x%x \n",mixctrl->ctrlwin);
	
	mixctrl->type_ctrl = TYPE_SLIDER;

	mixctrl->ID        = ID;
	
	mixctrl->destory   = esKRNL_GetCallBack(Delete_Hosted_Slider);

		
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




