
#include "lib_windows.h"




static int Compare_Windows_ID(void *key, void *data)
{
	return *(__u32 *)key - ((windows_t *)data)->id;
}

static int Compare_Button_ID(void *key, void *data)
{
	return *(int *)key - ((button_t *)data)->widget_info.id;
}

static int Compare_Slider_ID(void *key, void *data)
{
	return *(int *)key - ((slider_t *)data)->widget_info.id;
}

static int Compare_Statics_ID(void *key, void *data)
{
	return *(int *)key - ((statics_t *)data)->widget_info.id;
}

static int Compare_Listbar_ID(void *key, void *data)
{
	return *(int *)key - ((listbar_t *)data)->widget_info.id;
}

static int Compare_Progbars_ID(void *key, void *data)
{
	return *(int *)key - ((progbars_t *)data)->widget_info.id;
}



int Get_Windows_Ctrl_Number(windows_t *Windows)
{
	int num = 0;


	if(NULL != Windows->button_head)
	{
		num += Windows->button_head->cnt;
	}

	if(NULL != Windows->slider_head)
	{
		num += Windows->slider_head->cnt;
	}

	if(NULL != Windows->statics_head)
	{
		num += Windows->statics_head->cnt;
	}

	if(NULL != Windows->listbar_head)
	{
		num += Windows->listbar_head->cnt;
	}

	if(NULL != Windows->progbars_head)
	{
		num += Windows->progbars_head->cnt;
	}


	eDbug("Windows ctrl number is %d \n",num);
	
	return num;
}



int Search_All_Windows_Element(__u32 ID,windows_t *Windows,__u8 *Ctrl_Type)
{
	__u8 ret;
	
	if(NULL == Windows)
	{
		eDbug("Windows is NULL");
		return EPDK_FAIL;
	}

	if(NULL != Windows->button_head)
	{
		
		ret = Dlist_Check_Element(Windows->button_head,&ID,esKRNL_GetCallBack(Compare_Button_ID));
		if(-1 != ret)
		{
			*Ctrl_Type = TYPE_BUTTON;
			eDbug("this ID is a button,is the %d element \n",ret);
			return ret;
		}			
	}

	if(NULL != Windows->slider_head)
	{
		
		ret = Dlist_Check_Element(Windows->slider_head,&ID,esKRNL_GetCallBack(Compare_Slider_ID));
		if(-1 != ret)
		{
			*Ctrl_Type = TYPE_SLIDER;
			eDbug("this ID is a slider,is the %d element \n",ret);
			return ret;
		}			
	}

	if(NULL != Windows->statics_head)
	{
		
		ret = Dlist_Check_Element(Windows->statics_head,&ID,esKRNL_GetCallBack(Compare_Statics_ID));
		if(-1 != ret)
		{
			*Ctrl_Type = TYPE_STATICS;
			eDbug("this ID is a statics,is the %d element \n",ret);
			return ret;
		}			
	}

	if(NULL != Windows->listbar_head)
	{
		
		ret = Dlist_Check_Element(Windows->listbar_head,&ID,esKRNL_GetCallBack(Compare_Listbar_ID));
		if(-1 != ret)
		{
			*Ctrl_Type = TYPE_LISTBAR;
			eDbug("this ID is a listbar,is the %d element \n",ret);
			return ret;
		}			
	}

	if(NULL != Windows->progbars_head)
	{
		
		ret = Dlist_Check_Element(Windows->progbars_head,&ID,esKRNL_GetCallBack(Compare_Progbars_ID));
		if(-1 != ret)
		{
			*Ctrl_Type = TYPE_PROGBARS;
			eDbug("this ID is a progbars_head,is the %d element \n",ret);
			return ret;
		}			
	}


	return EPDK_FAIL;
		
}



int Set_Windows_Show_CallBack(__u32 Windows_ID,Man_Win *ManWin,__pCBK_t Windows_Show)
{
	__u8 ret;

	windows_t Windows;

	dlist_t *list = ManWin->windows_head;

	if(NULL == list)
	{
		eDbug("list is NULL \n");
		return EPDK_FAIL;
	}


	ret = Dlist_Find_Element(list,&Windows,&Windows_ID,esKRNL_GetCallBack(Compare_Windows_ID));
	if(-1 == ret)
	{
		return EPDK_FAIL;
	}

	Windows.show = Windows_Show;
	
	ret = Dlist_Modify(list,&Windows,ret);
	if(-1 == ret)
	{
		return EPDK_FAIL;
	}
	
	return EPDK_OK;
}


int Set_Windows_Hide_CallBack(__u32 Windows_ID,Man_Win *ManWin,__pCBK_t Windows_Hide)
{
	__u8 ret;

	windows_t Windows;

	dlist_t *list = ManWin->windows_head;

	if(NULL == list)
	{
		eDbug("list is NULL \n");
		return EPDK_FAIL;
	}


	ret = Dlist_Find_Element(list,&Windows,&Windows_ID,esKRNL_GetCallBack(Compare_Windows_ID));
	if(-1 == ret)
	{
		return EPDK_FAIL;
	}

	Windows.hide = Windows_Hide;

	ret = Dlist_Modify(list,&Windows,ret);
	if(-1 == ret)
	{
		return EPDK_FAIL;
	}

	return EPDK_OK;

}


int Set_Windows_Update_CallBack(__u32 Windows_ID,Man_Win *ManWin,__pCBK_t Windows_Update)
{
	__u8 ret;

	windows_t Windows;

	dlist_t *list = ManWin->windows_head;

	if(NULL == list)
	{
		eDbug("list is NULL \n");
		return EPDK_FAIL;
	}


	ret = Dlist_Find_Element(list,&Windows,&Windows_ID,esKRNL_GetCallBack(Compare_Windows_ID));
	if(-1 == ret)
	{
		return EPDK_FAIL;
	}

	Windows.update = Windows_Update;

	ret = Dlist_Modify(list,&Windows,ret);
	if(-1 == ret)
	{
		return EPDK_FAIL;
	}

	return EPDK_OK;

}


int Set_Windows_Button_Next(windows_t *Windows)
{
	__u8 ret;

	button_t button;

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
	
	if( Windows->button_focus_id >= ( (Windows->button_head->cnt) - 1) )
	{
		Windows->button_focus_id = 0;
	}

	else
	{
		Windows->button_focus_id++;
	}


	Windows->focus_button = (button_t *)Dlist_Retrieve_Pointer(Windows->button_head,Windows->button_focus_id);

	if(NULL == Windows->focus_button)
	{
		eDbug("can't find button element \n");
		return EPDK_FAIL;
	}

	
	GUI_WinSetFocusChild(Windows->focus_button->ctrlwin);
	
	
	return EPDK_OK;
}


int This_Windows_Button_Show(__u32 Button_ID,windows_t *Windows)
{
	int ret;

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



	ret = Dlist_Find(Windows->button_head,&Button_ID,esKRNL_GetCallBack(Compare_Button_ID));

	if(-1 == ret)
	{
		eDbug("can't find button element \n");
		return EPDK_FAIL;
	}
	
	Windows->button_focus_id = ret;
	

	Windows->focus_button = (button_t *)Dlist_Find_Pointer(Windows->button_head,&Button_ID,esKRNL_GetCallBack(Compare_Button_ID));

	if(NULL == Windows->focus_button)
	{
		eDbug("can't find button element \n");
		return EPDK_FAIL;
	}

	eDbug("Windows->focus_button is 0x%x \n",Windows->focus_button);



	GUI_WinSetFocusChild(Windows->focus_button->ctrlwin);


	return EPDK_OK;
}


int Set_Windows_Button_Prev(windows_t *Windows)
{
	__u8 ret;

	button_t button;

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
	
	if(Windows->button_focus_id <= 0)
	{
		Windows->button_focus_id = (Windows->button_head->cnt) - 1;
	}

	else
	{
		Windows->button_focus_id--;
	}


	Windows->focus_button = (button_t *)Dlist_Retrieve_Pointer(Windows->button_head,Windows->button_focus_id);

	if(NULL == Windows->focus_button)
	{
		eDbug("can't find button element \n");
		return EPDK_FAIL;
	}
	
	


	GUI_WinSetFocusChild(Windows->focus_button->ctrlwin);

	
	return EPDK_OK;
}




int Set_Windows_Slider_Next(windows_t *Windows)
{
	__u8 ret;

	slider_t slider;
	
	if(NULL == Windows)
	{
		eDbug("Windows is NULL \n");
		return EPDK_FAIL;
	}

	if(NULL == Windows->slider_head)
	{
		eDbug("Windows->slider_head is NULL \n");
		return EPDK_FAIL;
	}
	
	if( Windows->slider_focus_id >= ( (Windows->slider_head->cnt) - 1) )
	{
		Windows->slider_focus_id = 0;
	}

	else
	{
		Windows->slider_focus_id++;
	}

	Windows->focus_slider = (slider_t *)Dlist_Retrieve_Pointer(Windows->slider_head,Windows->slider_focus_id);

	if(NULL == Windows->focus_slider)
	{
		eDbug("can't find slider element \n");
		return EPDK_FAIL;
	}
	
	


	GUI_WinSetFocusChild(Windows->focus_slider->ctrlwin);

	
	return EPDK_OK;
}




int This_Windows_Slider_Show(__u32 Slider_ID,windows_t *Windows)
{
	int ret;
	
	slider_t slider;

	if(NULL == Windows)
	{
		eDbug("Windows is NULL \n");
		return EPDK_FAIL;
	}

	if(NULL == Windows->slider_head)
	{
		eDbug("Windows->slider_head is NULL \n");
		return EPDK_FAIL;
	}

	
	ret = Dlist_Find(Windows->slider_head,&Slider_ID,esKRNL_GetCallBack(Compare_Slider_ID));

	if(-1 == ret)
	{
		eDbug("can't find slider element \n");
		return EPDK_FAIL;
	}
	
	Windows->slider_focus_id = ret;
	

	Windows->focus_slider = (slider_t *)Dlist_Find_Pointer(Windows->slider_head,&Slider_ID,esKRNL_GetCallBack(Compare_Slider_ID));

	if(NULL == Windows->focus_slider)
	{
		eDbug("can't find slider element \n");
		return EPDK_FAIL;
	}



	GUI_WinSetFocusChild(Windows->focus_slider->ctrlwin);

	return EPDK_OK;

}


int Set_Windows_Slider_Prev(windows_t *Windows)
{
	__u8 ret;

	slider_t slider;

	if(NULL == Windows)
	{
		eDbug("wins is NULL \n");
		return EPDK_FAIL;
	}

	if(NULL == Windows->slider_head)
	{
		eDbug("Windows->slider_head is NULL \n");
		return EPDK_FAIL;
	}
	
	if(Windows->slider_focus_id <= 0)
	{
		Windows->slider_focus_id = (Windows->slider_head->cnt) - 1;
	}

	else
	{
		Windows->slider_focus_id--;
	}


	Windows->focus_slider = (slider_t *)Dlist_Retrieve_Pointer(Windows->slider_head,Windows->slider_focus_id);

	if(NULL == Windows->focus_slider)
	{
		eDbug("can't find slider element \n");
		return EPDK_FAIL;
	}
	
	

	GUI_WinSetFocusChild(Windows->focus_slider->ctrlwin);


	
	return EPDK_OK;
}






int Set_Windows_Statics_Next(windows_t *Windows)
{
	__u8 ret;

	statics_t statics;

	if(NULL == Windows)
	{
		eDbug("Windows is NULL \n");
		return EPDK_FAIL;
	}

	if(NULL == Windows->statics_head)
	{
		eDbug("Windows->statics_head is NULL \n");
		return EPDK_FAIL;
	}
	
	if( Windows->statics_focus_id >= ( (Windows->statics_head->cnt) - 1) )
	{
		Windows->statics_focus_id = 0;
	}

	else
	{
		Windows->statics_focus_id++;
	}


	Windows->focus_statics = (statics_t *)Dlist_Retrieve_Pointer(Windows->statics_head,Windows->statics_focus_id);

	if(NULL == Windows->focus_statics)
	{
		eDbug("can't find statics element \n");
		return EPDK_FAIL;
	}
	
	

	GUI_WinSetFocusChild(Windows->focus_statics->ctrlwin);


	
	return EPDK_OK;
}



int This_Windows_Statics_Show(__u32 Statics_ID,windows_t *Windows)
{
	int ret;
	
	
	statics_t statics;

	if(NULL == Windows)
	{
		eDbug("Windows is NULL \n");
		return EPDK_FAIL;
	}

	if(NULL == Windows->statics_head)
	{
		eDbug("Windows->statics_head is NULL \n");
		return EPDK_FAIL;
	}


	ret = Dlist_Find(Windows->statics_head,&Statics_ID,esKRNL_GetCallBack(Compare_Statics_ID));

	if(-1 == ret)
	{
		eDbug("can't find statics element \n");
		return EPDK_FAIL;
	}
	
	Windows->statics_focus_id = ret;
	

	Windows->focus_statics = (statics_t *)Dlist_Find_Pointer(Windows->statics_head,&Statics_ID,esKRNL_GetCallBack(Compare_Statics_ID));

	if(NULL == Windows->focus_statics)
	{
		eDbug("can't find statics element \n");
		return EPDK_FAIL;
	}



	GUI_WinSetFocusChild(Windows->focus_statics->ctrlwin);



	return EPDK_OK;

}

int Set_Windows_Statics_Prev(windows_t *Windows)
{
	__u8 ret;


	statics_t statics;

	if(NULL == Windows)
	{
		eDbug("wins is NULL \n");
		return EPDK_FAIL;
	}

	if(NULL == Windows->statics_head)
	{
		eDbug("Windows->statics_head is NULL \n");
		return EPDK_FAIL;
	}
	
	if(Windows->statics_focus_id <= 0)
	{
		Windows->statics_focus_id = (Windows->statics_head->cnt) - 1;
	}

	else
	{
		Windows->statics_focus_id--;
	}

	Windows->focus_statics = (statics_t *)Dlist_Retrieve_Pointer(Windows->statics_head,Windows->statics_focus_id);

	if(NULL == Windows->focus_statics)
	{
		eDbug("can't find statics element \n");
		return EPDK_FAIL;
	}
	
	

	GUI_WinSetFocusChild(Windows->focus_statics->ctrlwin);

	
	return EPDK_OK;
}



int Set_Windows_Listbar_Next(windows_t *Windows)
{
	__u8 ret;

	listbar_t listbar;

	if(NULL == Windows)
	{
		eDbug("Windows is NULL \n");
		return EPDK_FAIL;
	}

	if(NULL == Windows->listbar_head)
	{
		eDbug("Windows->listbar_head is NULL \n");
		return EPDK_FAIL;
	}
	
	if( Windows->listbar_focus_id >= ( (Windows->listbar_head->cnt) - 1) )
	{
		Windows->listbar_focus_id = 0;
	}

	else
	{
		Windows->listbar_focus_id++;
	}


	Windows->focus_listbar = (listbar_t *)Dlist_Retrieve_Pointer(Windows->listbar_head,Windows->listbar_focus_id);

	if(NULL == Windows->focus_listbar)
	{
		eDbug("can't find listbar element \n");
		return EPDK_FAIL;
	}
	
	

	GUI_WinSetFocusChild(Windows->focus_listbar->ctrlwin);

	
	return EPDK_OK;
}





int This_Windows_Listbar_Show(__u32 Listbar_ID,windows_t *Windows)
{
	int ret;
	
	listbar_t listbar;


	if(NULL == Windows)
	{
		eDbug("Windows is NULL \n");
		return EPDK_FAIL;
	}

	if(NULL == Windows->listbar_head)
	{
		eDbug("Windows->listbar_head is NULL \n");
		return EPDK_FAIL;
	}



	ret = Dlist_Find(Windows->listbar_head,&Listbar_ID,esKRNL_GetCallBack(Compare_Listbar_ID));

	if(-1 == ret)
	{
		eDbug("can't find listbar element \n");
		return EPDK_FAIL;
	}
	
	Windows->listbar_focus_id = ret;
	

	Windows->focus_listbar = (listbar_t *)Dlist_Find_Pointer(Windows->listbar_head,&Listbar_ID,esKRNL_GetCallBack(Compare_Listbar_ID));

	if(NULL == Windows->focus_listbar)
	{
		eDbug("can't find listbar element \n");
		return EPDK_FAIL;
	}




	GUI_WinSetFocusChild(Windows->focus_listbar->ctrlwin);

	
	esKRNL_TimeDly(5);					

    ret = LISTBAR_ShowPage(Windows->focus_listbar->h_listbar); 
    
	if(EPDK_FAIL == ret)
	{
		eDbug("listbar show page is failed \n");
		return EPDK_FAIL;
	}

	
	return EPDK_OK;

}



int Set_Windows_Listbar_Prev(windows_t *Windows)
{
	__u8 ret;

	listbar_t listbar;

	if(NULL == Windows)
	{
		eDbug("wins is NULL \n");
		return EPDK_FAIL;
	}

	if(NULL == Windows->listbar_head)
	{
		eDbug("Windows->listbar_head is NULL \n");
		return EPDK_FAIL;
	}
	
	if(Windows->listbar_focus_id <= 0)
	{
		Windows->listbar_focus_id = (Windows->listbar_head->cnt) - 1;
	}

	else
	{
		Windows->listbar_focus_id--;
	}


	Windows->focus_listbar = (listbar_t *)Dlist_Retrieve_Pointer(Windows->listbar_head,Windows->listbar_focus_id);

	if(NULL == Windows->focus_listbar)
	{
		eDbug("can't find listbar element \n");
		return EPDK_FAIL;
	}
	
	

	GUI_WinSetFocusChild(Windows->focus_listbar->ctrlwin);

	
	return EPDK_OK;
}


int This_Win_Delete_Hosted_CtrlWin(windows_t *Windows)
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
		eDbug("button.pFirstHosted is NULL \n");
		return EPDK_OK;		
	}

	
	destory_Last_Hosted_Win(&(Windows->focus_button->pFirstHosted));


	eDbug("button.pFirstHosted is 0x%x \n",Windows->focus_button->pFirstHosted);


	if(NULL == Windows->focus_button->pFirstHosted)
	{
		Windows->focus_button->FocusHosted = NULL;
		
		GUI_WinSetFocusChild(Windows->focus_button->ctrlwin);
	}
	else
	{
		
  		pHosted = Get_Last_Hosted_Win(Windows->focus_button->pFirstHosted);

		eDbug("pHosted->ctrlwin is 0x%x \n",pHosted->ctrlwin);

		Windows->focus_button->FocusHosted = pHosted;
				
  		GUI_WinSetFocusChild(pHosted->ctrlwin);
	}

	
}




int This_Win_Delete_All_Hosted_CtrlWin(windows_t *Windows)
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
		eDbug("button.pFirstHosted is NULL \n");
		return EPDK_OK;		
	}


	destory_Hosting_Win(&(Windows->focus_button->pFirstHosted));

	Windows->focus_button->FocusHosted = NULL;
	
	GUI_WinSetFocusChild(Windows->focus_button->ctrlwin);
	
}



int Set_Windows_Next(Man_Win *ManWin)
{
	__u8 ret;

	windows_t Windows;


	if(NULL == ManWin)
	{
		eDbug("ManWin is NULL \n");
		return EPDK_FAIL;
	}

	if(NULL == ManWin->windows_head)
	{
		eDbug("ManWin->windows_head is NULL \n");
		return EPDK_FAIL;
	}


	ret = Dlist_Retrieve(ManWin->windows_head,&Windows,ManWin->windows_focus_id);
	if(-1 == ret)
	{
		eDbug("can't find Windows element \n");
		return EPDK_FAIL;
	}
	
	//hide操作只是将图层隐藏没有做其他的操作，数据没有改变，不需要将它重新写回链表	
	if(NULL != Windows.hide)
	{
		ret = Windows.hide(&Windows);
		if(EPDK_FAIL == ret)
		{
			eDbug("Windows hide failed \n");
			return EPDK_FAIL;
		}
	}
	else
	{
		eDbug("Windows.hide is NULL \n");
	}


	
	if(ManWin->windows_focus_id >= ( (ManWin->windows_head->cnt) - 1) )
	{
		ManWin->windows_focus_id = 0;
	}

	else
	{
		ManWin->windows_focus_id++;
	}

	
	ManWin->focus_windows = (windows_t *)Dlist_Retrieve_Pointer(ManWin->windows_head,ManWin->windows_focus_id);

	if(NULL == ManWin->focus_windows)
	{
		eDbug("can't find Windows element \n");
		return EPDK_FAIL;
	}

	
	if(WINDOWS_SLEEP == ManWin->focus_windows->window_state)
	{
		if(NULL == ManWin->focus_windows->show)
		{
			eDbug("ManWin->focus_windows->show is NULL \n");
			return EPDK_FAIL;
			
		}
		
		ret = ManWin->focus_windows->show(ManWin->focus_windows);
		if(EPDK_FAIL == ret)
		{
			eDbug("Windows show failed \n");
			return EPDK_FAIL;
		}
		
		ManWin->focus_windows->window_state = WINDOWS_ON;
	}
	
	else if(WINDOWS_ON == ManWin->focus_windows->window_state)
	{
		if(NULL != ManWin->focus_windows->update)
		{
			ret = ManWin->focus_windows->update(ManWin->focus_windows);
			if(EPDK_FAIL == ret)
			{
				eDbug("Windows update failed \n");
				return EPDK_FAIL;
			}
		}
		else
		{
			eDbug("Windows.update is NULL \n");
		}
	}
	else
	{
		;
	}
	

	
	return EPDK_OK;
}



int This_Windows_Show(__u32 Windows_ID,Man_Win *ManWin)
{
	int ret;

	int i;
	
	windows_t Windows;

	
	if(NULL == ManWin)
	{
		eDbug("ManWin is NULL \n");
		return EPDK_FAIL;
	}

	if(NULL == ManWin->windows_head)
	{
		eDbug("ManWin->windows_head is NULL \n");
		return EPDK_FAIL;
	}

	eDbug("ManWin->windows_head->cnt is %d \n",ManWin->windows_head->cnt);
	
	for(i = 0; i < ManWin->windows_head->cnt;i++)
	{
		ret = Dlist_Retrieve(ManWin->windows_head,&Windows,i);
		if(-1 == ret)
		{
			eDbug("can't find Windows element \n");
			return EPDK_FAIL;
		}

		eDbug("Windows_ID is %d,Windows.id is %d \n",Windows_ID,Windows.id);

		if(Windows_ID == Windows.id)
		{
			ManWin->windows_focus_id = i;
			continue;
		}

		//hide操作只是将图层隐藏没有做其他的操作，数据没有改变，不需要将它重新写回链表	
		if(NULL != Windows.hide)
		{
			ret = Windows.hide(&Windows);
			if(EPDK_FAIL == ret)
			{
				eDbug("Windows hide failed \n");
				return EPDK_FAIL;
			}
		}
		else
		{
			eDbug("Windows.hide is NULL \n");
		}			
	}



	ManWin->focus_windows = (windows_t *)Dlist_Find_Pointer(ManWin->windows_head,&Windows_ID,esKRNL_GetCallBack(Compare_Windows_ID));

	if(NULL == ManWin->focus_windows)
	{
		eDbug("can't find Windows element \n");
		return EPDK_FAIL;
	}
	
	
	if(NULL == ManWin->focus_windows->show)
	{
		eDbug("ManWin->focus_windows->show is NULL \n");
		return EPDK_FAIL;
	}

	
	ret = ManWin->focus_windows->show(ManWin->focus_windows);
	
	if(EPDK_FAIL == ret)
	{
		eDbug("Windows show failed \n");
		return EPDK_FAIL;
	}

	ManWin->focus_windows->window_state = WINDOWS_ON;

	

	return EPDK_OK;

}




int Set_Windows_Prev(Man_Win *ManWin)
{
	__u8 ret;

	windows_t Windows;


	if(NULL == ManWin)
	{
		eDbug("ManWin is NULL \n");
		return EPDK_FAIL;
	}

	if(NULL == ManWin->windows_head)
	{
		eDbug("ManWin->windows_head is NULL \n");
		return EPDK_FAIL;
	}


	ret = Dlist_Retrieve(ManWin->windows_head,&Windows,ManWin->windows_focus_id);
	if(-1 == ret)
	{
		eDbug("can't find Windows element \n");
		return EPDK_FAIL;
	}
	
	//hide操作只是将图层隐藏没有做其他的操作，数据没有改变，不需要将它重新写回链表	
	if(NULL != Windows.hide)
	{
		ret = Windows.hide(&Windows);
		if(EPDK_FAIL == ret)
		{
			eDbug("Windows hide failed \n");
			return EPDK_FAIL;
		}
	}
	else
	{
		eDbug("Windows.hide is NULL \n");
	}


	
	if(ManWin->windows_focus_id <= 0 )
	{
		ManWin->windows_focus_id = ((ManWin->windows_head->cnt) - 1);
	}

	else
	{
		ManWin->windows_focus_id--;
	}


	ManWin->focus_windows = (windows_t *)Dlist_Retrieve_Pointer(ManWin->windows_head,ManWin->windows_focus_id);

	if(NULL == ManWin->focus_windows)
	{
		eDbug("can't find Windows element \n");
		return EPDK_FAIL;
	}

	
	if(WINDOWS_SLEEP == ManWin->focus_windows->window_state)
	{
		if(NULL == ManWin->focus_windows->show)
		{
			eDbug("ManWin->focus_windows->show is NULL \n");
			return EPDK_FAIL;
			
		}
		
		ret = ManWin->focus_windows->show(ManWin->focus_windows);
		if(EPDK_FAIL == ret)
		{
			eDbug("Windows show failed \n");
			return EPDK_FAIL;
		}
		
		ManWin->focus_windows->window_state = WINDOWS_ON;
	}
	
	else if(WINDOWS_ON == ManWin->focus_windows->window_state)
	{
		if(NULL != ManWin->focus_windows->update)
		{
			ret = ManWin->focus_windows->update(ManWin->focus_windows);
			if(EPDK_FAIL == ret)
			{
				eDbug("Windows update failed \n");
				return EPDK_FAIL;
			}
		}
		else
		{
			eDbug("Windows.update is NULL \n");
		}
	}
	else
	{
		;
	}


	return EPDK_OK;
}




H_WIN Windows_Create(windows_t *win,__pGUI_WIN_CB proc)
{
	__gui_framewincreate_para_t frameWins;
	FB fb;

	GUI_LyrWinGetFB(win->layer, &fb);
		
	eLIBs_memset(&frameWins, 0, sizeof(__gui_framewincreate_para_t));

	frameWins.name 			= win->name;
	frameWins.dwExStyle 	= WS_EX_NONE;
	frameWins.dwStyle 		= WS_NONE|WS_VISIBLE;
	frameWins.spCaption 	= NULL;
	frameWins.hOwner		= NULL;
	frameWins.hHosting 		= win->parent;
	frameWins.id        	= win->id;
	frameWins.FrameWinProc 	= (__pGUI_WIN_CB)esKRNL_GetCallBack((__pCBK_t)proc);
	frameWins.rect.x 		= 0;
	frameWins.rect.y 		= 0;
	frameWins.rect.width	= fb.size.width;
	frameWins.rect.height 	= fb.size.height;
	frameWins.BkColor.alpha = 0;
	frameWins.BkColor.red 	= 0;
	frameWins.BkColor.green = 0;
	frameWins.BkColor.blue 	= 0;
	frameWins.attr 			=(void *)win;
	frameWins.hLayer 		= win->layer;

	return (GUI_FrmWinCreate(&frameWins));
}


int Delete_Windows(void *data)
{
	windows_t *Windows = NULL;

	Man_Win *ManWin = NULL;
	
	Windows = (windows_t *)data;

	if(NULL == Windows)
	{
		eDbug("Windows is NULL \n");
		return EPDK_FAIL;
	}


	ManWin = (Man_Win *)GUI_WinGetAttr(Windows->parent);
	
	if(NULL != ManWin)
	{
		if(Windows == ManWin->focus_windows)
		{
			eDbug("free ManWin->focus_windows \n");

			ManWin->focus_windows = NULL;		
		}

		ManWin = NULL; 		
	}

	
	if(NULL != Windows->button_head)
	{
		Dlist_Destroy(Windows->button_head);
		Windows->button_head = NULL;
		Windows->focus_button = NULL;
	}

	if(NULL != Windows->slider_head)
	{
		Dlist_Destroy(Windows->slider_head);
		Windows->slider_head = NULL;
		Windows->focus_slider = NULL;
	}

	if(NULL != Windows->statics_head)
	{
		Dlist_Destroy(Windows->statics_head);
		Windows->statics_head = NULL;
		Windows->focus_statics = NULL;
	}

	if(NULL != Windows->listbar_head)
	{
		Dlist_Destroy(Windows->listbar_head);
		Windows->listbar_head = NULL;
		Windows->focus_listbar = NULL;
	}

	if(NULL != Windows->progbars_head)
	{
		Dlist_Destroy(Windows->progbars_head);
		Windows->progbars_head  = NULL;
		Windows->focus_progbars = NULL;
	}


	if(NULL != Windows->layer)
	{
		GUI_LyrWinDelete(Windows->layer);
		Windows->layer = NULL;
	}



	In_Free(Windows,sizeof(windows_t));

	return EPDK_OK;

}


dlist_t *Create_Windows_List(void)
{
	return Dlist_Create(sizeof(windows_t),esKRNL_GetCallBack(Delete_Windows));
}


int Delete_One_Windows(__u32 Windows_ID,Man_Win *ManWin)
{
	int ret;

	if(NULL == ManWin)
	{
		eDbug("ManWin is NULL \n");
		return EPDK_FAIL;
	}

	if(NULL == ManWin->windows_head)
	{
		eDbug("ManWin->windows_head is NULL \n");
		return EPDK_FAIL;
	}

	ret = Dlist_Delby_Key(ManWin->windows_head,&Windows_ID,esKRNL_GetCallBack(Compare_Windows_ID));
	if(-1 == ret)
	{
		eDbug("delete Windows failed \n");
		return EPDK_FAIL;
	}

	return EPDK_OK;
}



int Register_Windows(__u32 ID,Man_Win *ManWin,__pCBK_t callback,int x,int y,int width,int height)
{	
	int ret;
	
	windows_t *Windows = NULL;


	ret = Dlist_Find(ManWin->windows_head,&ID,esKRNL_GetCallBack(Compare_Windows_ID));
	if(EPDK_FAIL != ret)
	{
		eDbug("windows have been register \n");
		return EPDK_OK;
	}

	
	Windows = In_Malloc(sizeof(windows_t));
	if(NULL == Windows)
	{
		eDbug("Windows is NULL \n");
		return EPDK_FAIL;
	}
	
	Windows->id = ID;
	
	Windows->ui_rect.x       = x;
	Windows->ui_rect.y       = y;
	Windows->ui_rect.width   = width;
	Windows->ui_rect.height  = height;

	Windows->parent          = ManWin->man_win;
	if(NULL == Windows->parent)
	{
		eDbug("Windows->parent is NULL \n");
		return EPDK_FAIL;
	}
	eDbug("Windows->parent is 0x%x \n",Windows->parent);

	
	Windows->layer           = Layer_Create(&(Windows->ui_rect));
	if(NULL == Windows->layer)
	{
		eDbug("Windows->layer is NULL \n");
		return EPDK_FAIL;
	}
	eDbug("Windows->layer is 0x%x \n",Windows->layer);


	
	Windows->framewin        = Windows_Create(Windows,callback);
	if(NULL == Windows->framewin)
	{
		eDbug("Windows->framewin is NULL \n");
		return EPDK_FAIL;
	}
	eDbug("Windows->framewin is 0x%x \n",Windows->framewin);


	
	Windows->button_head     = Create_Button_List();
	if(NULL == Windows->button_head)
	{
		eDbug("Windows->button_head is NULL \n");
		return EPDK_FAIL;
	}
	
	Windows->button_focus_id = 0;



	Windows->slider_head     = Create_Slider_List();
	if(NULL == Windows->slider_head)
	{
		eDbug("Windows->slider_head is NULL \n");
		return EPDK_FAIL;
	}
	
	Windows->slider_focus_id = 0;



	Windows->statics_head    = Create_Statics_List();
	if(NULL == Windows->statics_head)
	{
		eDbug("Windows->statics_head is NULL \n");
		return EPDK_FAIL;
	}
	
	Windows->statics_focus_id = 0;



	Windows->listbar_head    = Create_Listbar_List();
	if(NULL == Windows->listbar_head)
	{
		eDbug("Windows->listbar_head is NULL \n");
		return EPDK_FAIL;
	}
	
	Windows->listbar_focus_id = 0;


	Windows->progbars_head   = Create_Progbars_List();
	if(NULL == Windows->progbars_head)
	{
		eDbug("Windows->progbars_head is NULL \n");
		return EPDK_FAIL;
	}
	
	Windows->progbars_focus_id = 0;



	Windows->window_state = WINDOWS_SLEEP;
	
	ret = Dlist_Append(ManWin->windows_head,Windows);
	if(EPDK_FAIL == ret)
	{
		eDbug("Dlist_Append is failed \n");
		return EPDK_FAIL;
	}


	return EPDK_OK;
}

