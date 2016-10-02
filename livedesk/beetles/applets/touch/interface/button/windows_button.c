
#include "windows_button.h"




static __s32 Button_Left(__gui_msg_t * msg)
{
	__u8 ret;

	windows_t *Windows;
	

	Windows = (windows_t *)GUI_WinGetAttr(msg->h_deswin);


	if(NULL == Windows)
	{
		eDbug("Windows is NULL \n");
		return EPDK_FAIL;
	}

	eDbug("into button left callback \n");

	
	ret = Set_Windows_Button_Prev(Windows);
	
	if(EPDK_FAIL == ret)
	{
		eDbug("Set Windows Button Prev failed \n");
		return EPDK_FAIL;
	}

	return EPDK_OK;
}




static __s32 Button_Right(__gui_msg_t * msg)
{
	__u8 ret;

	windows_t *Windows;
	

	Windows = (windows_t *)GUI_WinGetAttr(msg->h_deswin);


	if(NULL == Windows)
	{
		eDbug("Windows is NULL \n");
		return EPDK_FAIL;
	}

	eDbug("into button right callback \n");

	
	ret = Set_Windows_Button_Next(Windows);	
	
	if(EPDK_FAIL == ret)
	{
		eDbug("Set Windows Button Next failed \n");
		return EPDK_FAIL;
	}

	return EPDK_OK;
}



static __s32 Button_Enter(__gui_msg_t * msg)
{
	__u8 ret;

	windows_t *Windows;
	

	Windows = (windows_t *)GUI_WinGetAttr(msg->h_deswin);


	if(NULL == Windows)
	{
		eDbug("Windows is NULL \n");
		return EPDK_FAIL;
	}

	eDbug("into button enter callback \n");

	switch(msg->id)
	{
		case button_menu_ID:
		{
			ret = Register_One_Listbar_HostedFrom_Button(listbar_setting_ID,Windows,&setting_ui,0);
			if(EPDK_FAIL == ret)
			{
				eDbug("Register listbar failed \n");
				return EPDK_FAIL;
			}

		}
		break;




		default:
		break;
	}

	return EPDK_OK;
}



static __s32 Button_Return(__gui_msg_t * msg)
{
	__u8 ret;

	windows_t *Windows;
	

	Windows = (windows_t *)GUI_WinGetAttr(msg->h_deswin);


	if(NULL == Windows)
	{
		eDbug("Windows is NULL \n");
		return EPDK_FAIL;
	}

	eDbug("into button return callback \n");

	
	Command_TO_Parent(Windows->framewin,SWITCH_TO_MMENU,0,0);

	return EPDK_OK;
}



static __s32 Register_One_Button(__u32 ID,windows_t *Windows,button_ui_t *button_ui)
{
	__u8 ret;

	ret = Register_Button(ID,Windows,button_ui);
	if(EPDK_FAIL == ret)
	{
		eDbug("Register button failed \n");
		return EPDK_FAIL;
	}

	
	ret = Set_Button_Left_CallBack(ID,Windows,esKRNL_GetCallBack(Button_Left));
	if(EPDK_FAIL == ret)
	{
		eDbug("Set Button Left CallBack failed \n");
		return EPDK_FAIL;
	}
	
	ret = Set_Button_Right_CallBack(ID,Windows,esKRNL_GetCallBack(Button_Right));
	if(EPDK_FAIL == ret)
	{
		eDbug("Set Button Right CallBack failed \n");
		return EPDK_FAIL;
	}

	ret = Set_Button_Enter_CallBack(ID,Windows,esKRNL_GetCallBack(Button_Enter));
	if(EPDK_FAIL == ret)
	{
		eDbug("Set Button Enter CallBack failed \n");
		return EPDK_FAIL;
	}

	ret = Set_Button_Return_CallBack(ID,Windows,esKRNL_GetCallBack(Button_Return));
	if(EPDK_FAIL == ret)
	{
		eDbug("Set Button Return CallBack failed \n");
		return EPDK_FAIL;
	}

	return EPDK_OK;

}


static __s32 Slider_Return(__gui_msg_t * msg)
{
	__u8 ret;

	windows_t *Windows;
	

	Windows = (windows_t *)GUI_WinGetAttr(msg->h_deswin);


	if(NULL == Windows)
	{
		eDbug("Windows is NULL \n");
		return EPDK_FAIL;
	}

	eDbug("into slider return callback \n");

	switch(msg->id)
	{
		case slider_volume_ID:
		{
			This_Win_Delete_Hosted_CtrlWin(Windows);
		}
		break;

		default:
		{
			Command_TO_Parent(Windows->framewin,SWITCH_TO_MMENU,0,0);
		}
		break;
	}	

	return EPDK_OK;
}


static __s32 Register_One_Slider_HostedFrom_Button(__u32 ID,windows_t *Windows,slider_ui_t *slider_ui)
{
	__u8 ret;

	ret = Register_Slider_HostedFrom_Button(ID,Windows,slider_ui);
	if(EPDK_FAIL == ret)
	{
		eDbug("Register slider failed \n");
		return EPDK_FAIL;
	}


	ret = Set_Slider_HostedFrom_Button_Return_CallBack(ID,Windows,esKRNL_GetCallBack(Slider_Return));

	if(EPDK_FAIL == ret)
	{
		eDbug("Set Slider HostedFrom ButtonReturn CallBack failed \n");
		return EPDK_FAIL;
	}
	
	return EPDK_OK;

}



static __s32 Register_One_Statics(__u32 ID,windows_t *Windows,statics_ui_t *statics_ui)
{
	__u8 ret;

	ret = Register_Statics(ID,Windows,statics_ui);
	
	if(EPDK_FAIL == ret)
	{
		eDbug("Register slider failed \n");
		return EPDK_FAIL;
	}	

	return EPDK_OK;

}




static __s32 Listbar_Left(__gui_msg_t * msg)
{
	__u8 ret;

	windows_t *Windows;
	

	Windows = (windows_t *)GUI_WinGetAttr(msg->h_deswin);


	if(NULL == Windows)
	{
		eDbug("Windows is NULL \n");
		return EPDK_FAIL;
	}


	eDbug("into Listbar Left callback \n");


	LISTBAR_PrevItem(Windows->focus_button->FocusHosted->mix_ctrl.Listbar.h_listbar);				

	return EPDK_OK;
}


static __s32 Listbar_Right(__gui_msg_t * msg)
{
	__u8 ret;

	windows_t *Windows;
	

	Windows = (windows_t *)GUI_WinGetAttr(msg->h_deswin);


	if(NULL == Windows)
	{
		eDbug("Windows is NULL \n");
		return EPDK_FAIL;
	}

	eDbug("into Listbar Right callback \n");


	LISTBAR_NextItem(Windows->focus_button->FocusHosted->mix_ctrl.Listbar.h_listbar);				


	return EPDK_OK;
}


static __s32 Listbar_Enter(__gui_msg_t * msg)
{
	__u8 ret;

	windows_t *Windows;
	

	Windows = (windows_t *)GUI_WinGetAttr(msg->h_deswin);


	if(NULL == Windows)
	{
		eDbug("Windows is NULL \n");
		return EPDK_FAIL;
	}

	eDbug("into Statics enter callback \n");


	switch(msg->id)
	{
		case listbar_setting_ID:
		{
			if(1 == Windows->focus_button->FocusHosted->mix_ctrl.Listbar.listbar_draw.Index)
			{
				int ret;
				
				ret = Register_One_Slider_HostedFrom_Button(slider_volume_ID,Windows,&volume_ui);
				if(EPDK_FAIL == ret)
				{
					eDbug("Register Slider failed \n");
					return EPDK_FAIL;
				}
			}
		}
		break;


		default:
		break;


	}	

	return EPDK_OK;
}


static __s32 Listbar_Return(__gui_msg_t * msg)
{
	__u8 ret;

	windows_t *Windows;
	

	Windows = (windows_t *)GUI_WinGetAttr(msg->h_deswin);


	if(NULL == Windows)
	{
		eDbug("Windows is NULL \n");
		return EPDK_FAIL;
	}

	eDbug("into Listbar return callback \n");


	switch(msg->id)
	{
		case listbar_setting_ID:
		{
			This_Win_Delete_Hosted_CtrlWin(Windows);
		}
		break;

		default:
		break;
	}	

	return EPDK_OK;
}




static __s32 Register_One_Listbar_HostedFrom_Button(__u32 ID,windows_t *Windows,listbar_ui_t *listbar_ui,__u8 focus_ID)
{
	__u8 ret;

	ret = Register_Listbar_HostedFrom_Button(ID,Windows,listbar_ui,focus_ID);
	
	if(EPDK_FAIL == ret)
	{
		eDbug("Register slider failed \n");
		return EPDK_FAIL;
	}


	ret = Set_Listbar_HostedFrom_Button_Left_CallBack(ID,Windows,esKRNL_GetCallBack(Listbar_Left));

	if(EPDK_FAIL == ret)
	{
		eDbug("Set Slider HostedFrom ButtonReturn CallBack failed \n");
		return EPDK_FAIL;
	}

	ret = Set_Listbar_HostedFrom_Button_Right_CallBack(ID,Windows,esKRNL_GetCallBack(Listbar_Right));

	if(EPDK_FAIL == ret)
	{
		eDbug("Set Slider HostedFrom ButtonReturn CallBack failed \n");
		return EPDK_FAIL;
	}

	ret = Set_Listbar_HostedFrom_Button_Enter_CallBack(ID,Windows,esKRNL_GetCallBack(Listbar_Enter));

	if(EPDK_FAIL == ret)
	{
		eDbug("Set Slider HostedFrom ButtonReturn CallBack failed \n");
		return EPDK_FAIL;
	}


	ret = Set_Listbar_HostedFrom_Button_Return_CallBack(ID,Windows,esKRNL_GetCallBack(Listbar_Return));

	if(EPDK_FAIL == ret)
	{
		eDbug("Set Slider HostedFrom ButtonReturn CallBack failed \n");
		return EPDK_FAIL;
	}


	return EPDK_OK;
	
}




static __s32 Show_Windows(void *ptr)
{
	__u8 ret;
	
	windows_t *Windows = (windows_t *)ptr;
	
	if(NULL == Windows)
	{
		eDbug("Windows is NULL \n");
		return EPDK_FAIL;
	}

	if(NULL == Windows->layer)
	{
		eDbug("Windows->layer is NULL \n");
		return EPDK_FAIL;
	}

	eDbug("show button window \n");

	eDbug("Windows->framewin is 0x%x \n",Windows->framewin);

	
	GUI_LyrWinSetSta(Windows->layer, GUI_LYRWIN_STA_ON);
	GUI_LyrWinSetTop(Windows->layer);


	ret = Register_One_Button(button_menu_ID,Windows,&menu_ui);
	if(EPDK_FAIL == ret)
	{
		eDbug("Register button failed \n");
		return EPDK_FAIL;
	}
	
		
	ret = Register_One_Button(button_edit_ID,Windows,&edit_ui);
	if(EPDK_FAIL == ret)
	{
		eDbug("Register button failed \n");
		return EPDK_FAIL;
	}
	
	ret = Register_One_Button(button_help_ID,Windows,&help_ui);
	if(EPDK_FAIL == ret)
	{
		eDbug("Register button failed \n");
		return EPDK_FAIL;
	}
	
	ret = Register_One_Button(button_project_ID,Windows,&project_ui);
	if(EPDK_FAIL == ret)
	{
		eDbug("Register button failed \n");
		return EPDK_FAIL;
	}

	ret = Register_One_Statics(statics_voltage_ID,Windows,&voltage_ui);
	if(EPDK_FAIL == ret)
	{
		eDbug("Register statics failed \n");
		return EPDK_FAIL;
	}


	This_Windows_Button_Show(button_menu_ID,Windows);

	Windows->time_ID    = 0x10;
	Windows->time_count = 100;

	if(EPDK_FALSE == GUI_IsTimerInstalled(Windows->framewin,Windows->time_ID))
	{
		GUI_SetTimer(Windows->framewin,Windows->time_ID,Windows->time_count,NULL);
	}

	
	return EPDK_OK;

}


static __s32 Hide_Windows(void *ptr)
{
	windows_t *Windows = (windows_t *)ptr;

	if(NULL == Windows)
	{
		return EPDK_FAIL;
	}

	if(NULL == Windows->layer)
	{
		return EPDK_FAIL;
	}

	eDbug("hide button window \n");
	
	if(GUI_LYRWIN_STA_ON == GUI_LyrWinGetSta(Windows->layer))
	{
		GUI_LyrWinSetSta(Windows->layer, GUI_LYRWIN_STA_SLEEP);
	}

	if(GUI_IsTimerInstalled(Windows->framewin,Windows->time_ID))
	{
		GUI_KillTimer(Windows->framewin,Windows->time_ID);
	}
	
	return EPDK_OK;

}



static __s32 Update_Windows(void *ptr)
{
	static int i;

	windows_t *Windows = (windows_t *)ptr;


	if(NULL == Windows)
	{
		return EPDK_FAIL;
	}

	if(NULL == Windows->layer)
	{
		return EPDK_FAIL;
	}
	
	if(GUI_LYRWIN_STA_ON != GUI_LyrWinGetSta(Windows->layer))
	{
		GUI_LyrWinSetSta(Windows->layer, GUI_LYRWIN_STA_ON);
		GUI_LyrWinSetTop(Windows->layer);		
	}

	if(EPDK_FALSE == GUI_IsTimerInstalled(Windows->framewin,Windows->time_ID))
	{
		GUI_SetTimer(Windows->framewin,Windows->time_ID,Windows->time_count,NULL);
	}


	eDbug("Update_Windows \n");
	
	Set_Statics_Bmp(statics_voltage_ID,Windows,i);

	i++;

	if(i > voltage_ui.bmp_total)
	{
		i = 0;
	}

	
	return EPDK_OK;

}






static __s32 Windows_B_Proc(__gui_msg_t *msg)
{
	switch(msg->id)
	{
		case GUI_MSG_CREATE:
		{
			eDbug("create Windows B \n");
		}
		return EPDK_OK;
		
		case GUI_MSG_DESTROY:
		{
			windows_t *Windows;
			
			Windows = (windows_t *)GUI_WinGetAttr(msg->h_deswin);

			if(NULL == Windows)
			{
				eDbug("Windows is NULL \n");
				return EPDK_FAIL;
			}
			if(GUI_IsTimerInstalled(msg->h_deswin,Windows->time_ID))
			{
				GUI_KillTimer(msg->h_deswin,Windows->time_ID);
			}
			
			eDbug("destory Windows B \n");
		}
		return EPDK_OK;


		case GUI_MSG_TIMER:
		{
			windows_t *Windows;
			
			Windows = (windows_t *)GUI_WinGetAttr(msg->h_deswin);

			if(NULL == Windows)
			{
				eDbug("Windows is NULL \n");
				return EPDK_FAIL;
			}
			
			if(msg->dwAddData1 == Windows->time_ID)
			{
				Update_Windows(Windows);			
			}

		}
		return EPDK_OK;
		
		default:
			break;
	}


	
	return GUI_FrmWinDefaultProc(msg);

	
}


int Register_Windows_B(__u32 ID,Man_Win *ManWin,RECT *windows_rect)
{	
	int ret;

	int x = 0,y = 0,width = 0,height = 0;

	x      = windows_rect->x;
	y      = windows_rect->y;
	width  = windows_rect->width;
	height = windows_rect->height;

	
	ret = Register_Windows(ID,ManWin,esKRNL_GetCallBack(Windows_B_Proc),x,y,width,height);	
	if(EPDK_FAIL == ret)
	{
		eDbug("Register Windows failed \n");
		return EPDK_FAIL;
	}

	eDbug("Show_Windows is 0x%x \n",Show_Windows);
	
	ret = Set_Windows_Show_CallBack(ID,ManWin,esKRNL_GetCallBack(Show_Windows));
	if(EPDK_FAIL == ret)
	{
		eDbug("Set Windows Show CallBack failed \n");
		return EPDK_FAIL;
	}
	
	ret = Set_Windows_Hide_CallBack(ID,ManWin,esKRNL_GetCallBack(Hide_Windows));
	if(EPDK_FAIL == ret)
	{
		eDbug("Set Windows hide CallBack failed \n");
		return EPDK_FAIL;
	}
	
	ret = Set_Windows_Update_CallBack(ID,ManWin,esKRNL_GetCallBack(Update_Windows));
	if(EPDK_FAIL == ret)
	{
		eDbug("Set Windows update CallBack failed \n");
		return EPDK_FAIL;
	}

	return EPDK_OK;
}

