
#include "lib_progbars.h"



static int Compare_ID(void *key, void *data)
{
	return *(int *)key - ((progbars_t *)data)->widget_info.id;
}



int Set_Progbars_Return_CallBack(__u32 progbars_ID,windows_t *Windows,__pCBK_t Progbars_Return)
{
	__u8 ret;

	progbars_t progbars;

	dlist_t *list = Windows->progbars_head;

	if(NULL == list)
	{
		eDbug("list is NULL \n");
		return EPDK_FAIL;
	}


	ret = Dlist_Find_Element(list,&progbars,&progbars_ID,esKRNL_GetCallBack(Compare_ID));
	if(-1 == ret)
	{
		return EPDK_FAIL;
	}

	progbars.progbars_info.progbars_attribute.progbars_action.return_action = Progbars_Return;


	ret = Dlist_Modify(list,&progbars,ret);
	if(-1 == ret)
	{
		return EPDK_FAIL;
	}

	return EPDK_OK;
}



int Create_Progbars(__u32 progbars_id,H_WIN patent,progbars_t *progbars,progbars_ui_t *progbars_ui)
{
	int ret;

	ret = Get_Progbars_Source(&(progbars->progbars_info),progbars_id,progbars_ui);
	
	if(EPDK_FAIL == ret)
	{
		eDbug("Get Progbars Source failed \n");
		return EPDK_FAIL;
	}
	
	progbars->widget_info.id                = progbars_id;
	progbars->widget_info.hParent           = patent;
    progbars->widget_info.ctl_rect.x		= progbars_ui->progbars_location.x;
	progbars->widget_info.ctl_rect.y		= progbars_ui->progbars_location.y;
	progbars->widget_info.ctl_rect.width	= progbars_ui->progbars_location.width;
	progbars->widget_info.ctl_rect.height	= progbars_ui->progbars_location.height;
	progbars->widget_info.spClassName 	    = CTRL_PROGBAR;
	progbars->widget_info.dwStyle 		    = WS_VISIBLE|SDS_RESPOND_OUTDEV;
	progbars->widget_info.dwExStyle         = 0;
	progbars->widget_info.attr 			    = &(progbars->progbars_info.progbars_attribute);
	
	progbars->ctrlwin = GUI_CtrlWinCreate(&(progbars->widget_info));
	if(NULL == progbars->ctrlwin)
	{
		eDbug("create progbars ctrlwin failed \n");
		return EPDK_FAIL;		
	}
	
	eDbug("progbars->ctrlwin is 0x%x \n",progbars->ctrlwin);
	
	return EPDK_OK;
}


int Release_Progbars(progbars_t *progbars)
{
	int ret;

	ret = Relase_Progbars_Source(&(progbars->progbars_info),progbars->widget_info.id);

	return ret;
}


int Delete_Progbars(void *data)
{
	progbars_t *progbars = NULL;

	windows_t *Windows = NULL;

	progbars = (progbars_t *)data;

	if(NULL == progbars)
	{
		return EPDK_FAIL;
	}


	Windows = (windows_t *)GUI_WinGetAttr(progbars->widget_info.hParent);
	
	if(NULL != Windows)
	{
		if(progbars == Windows->focus_progbars)
		{
			eDbug("free Windows->focus_progbars \n");

			Windows->focus_progbars = NULL;		
		}

		Windows = NULL; 		
	}

	
	if(NULL != progbars->ctrlwin)
	{
        GUI_LyrWinSel(progbars->widget_info.hLayer);

	    GUI_ClearRect(progbars->widget_info.ctl_rect.x,progbars->widget_info.ctl_rect.y,
	    progbars->widget_info.ctl_rect.x + progbars->widget_info.ctl_rect.width,
	    progbars->widget_info.ctl_rect.y + progbars->widget_info.ctl_rect.height); 

		GUI_CtrlWinDelete(progbars->ctrlwin);	
		
		progbars->ctrlwin = NULL;
	}

	
	Release_Progbars(progbars);
	
	In_Free(progbars,sizeof(progbars_t));


	return EPDK_OK;

}





int Delete_Hosted_Progbars(void *data)
{
	progbars_t *progbars = NULL;

	progbars = (progbars_t *)data;

	if(NULL == progbars)
	{
		return EPDK_FAIL;
	}

	eDbug("delete progbars \n");

	
	if(NULL != progbars->ctrlwin)
	{
        GUI_LyrWinSel(progbars->widget_info.hLayer);
        
	    GUI_ClearRect(progbars->widget_info.ctl_rect.x,progbars->widget_info.ctl_rect.y,
	    progbars->widget_info.ctl_rect.x + progbars->widget_info.ctl_rect.width,
	    progbars->widget_info.ctl_rect.y + progbars->widget_info.ctl_rect.height); 

		eDbug("progbars->ctrlwin is 0x%x \n",progbars->ctrlwin);
		
		GUI_CtrlWinDelete(progbars->ctrlwin);
		
		progbars->ctrlwin = NULL;
	}
	
	Release_Progbars(progbars);
	
	return EPDK_OK;
}




dlist_t *Create_Progbars_List(void)
{
	return Dlist_Create(sizeof(progbars_t),esKRNL_GetCallBack(Delete_Progbars));
}



int Register_Progbars(__u32 ID,windows_t *Windows,progbars_ui_t *progbars_ui)
{
	__u8 ret;
	
	progbars_t *progbars = NULL;


	ret = Dlist_Find(Windows->progbars_head,&ID,esKRNL_GetCallBack(Compare_ID));
	if(EPDK_FAIL != ret)
	{
		eDbug("progbars have been register \n");
		return EPDK_OK;
	}


	progbars = In_Malloc(sizeof(progbars_t));
	if(NULL == progbars)
	{
		return EPDK_FAIL;
	}
	
	ret = Create_Progbars(ID,Windows->framewin,progbars,progbars_ui);
	if(EPDK_FAIL == ret)
	{
		eDbug("create progbars failed \n");
		return EPDK_FAIL;
	}
	
	ret = PROGBAR_SetRange(progbars->ctrlwin, (__s32)0, (__s32)30);
	if(EPDK_FAIL == ret)
	{
		eDbug("progbars SetRange failed \n");
		return EPDK_FAIL;
	}
	
	ret = PROGBAR_SetLineSize(progbars->ctrlwin,10);
	if(EPDK_FAIL == ret)
	{
		eDbug("progbars SetLineSize failed \n");
		return EPDK_FAIL;
	}
	
	ret = PROGBAR_SetValue(progbars->ctrlwin,3);
	if(EPDK_FAIL == ret)
	{
		eDbug("progbars SetValue failed \n");
		return EPDK_FAIL;
	}

	Dlist_Append(Windows->progbars_head,progbars);

	return EPDK_OK;

}






int Register_Progbars_HostedFrom_Button(__u32 ID,windows_t *Windows,progbars_ui_t *progbars_ui)
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


	ret = Create_Progbars(ID,Windows->focus_button->widget_info.hParent,&(mixctrl->mix_ctrl.Progbars),progbars_ui);
	if(EPDK_FAIL == ret)
	{
		eDbug("create progbars failed \n");
		return EPDK_FAIL;
	}

	ret = PROGBAR_SetRange(mixctrl->mix_ctrl.Progbars.ctrlwin, (__s32)0, (__s32)30);
	if(EPDK_FAIL == ret)
	{
		eDbug("progbars SetRange failed \n");
		return EPDK_FAIL;
	}
	
	ret = PROGBAR_SetLineSize(mixctrl->mix_ctrl.Progbars.ctrlwin,10);
	if(EPDK_FAIL == ret)
	{
		eDbug("progbars SetLineSize failed \n");
		return EPDK_FAIL;
	}

	
	ret = PROGBAR_SetValue(mixctrl->mix_ctrl.Progbars.ctrlwin,10);
	if(EPDK_FAIL == ret)
	{
		eDbug("progbars SetValue failed \n");
		return EPDK_FAIL;
	}

	
	mixctrl->ctrlwin = mixctrl->mix_ctrl.Progbars.ctrlwin;
	
	if(NULL == mixctrl->ctrlwin)
	{
		eDbug("register a progbars hostfrom button is failed \n");
		return EPDK_FAIL;		
	}

	eDbug("mixctrl->ctrlwin is 0x%x \n",mixctrl->ctrlwin);
	
	mixctrl->type_ctrl = TYPE_PROGBARS;

	mixctrl->ID        = ID;
	
	mixctrl->destory   = esKRNL_GetCallBack(Delete_Hosted_Progbars);

		
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




