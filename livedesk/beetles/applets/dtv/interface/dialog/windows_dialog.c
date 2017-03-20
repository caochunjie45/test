
#include "windows_dialog.h"




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

		case button_yes_ID:
		{

		}
		break;

		case button_no_ID:
		{

		}
		break;


		case button_ok_ID:
		{

		}
		break;

		default:
		break;
	}

	
	Command_TO_Parent(Windows->framewin,Exit_Win,0,0);


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

	
	Command_TO_Parent(Windows->framewin,Exit_Win,0,0);

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

	eDbug("show window \n");

	
	GUI_LyrWinSetSta(Windows->layer, GUI_LYRWIN_STA_ON);
	GUI_LyrWinSetTop(Windows->layer);


	ret = Register_One_Statics(statics_bg_ID,Windows,&bg_ui);
	if(EPDK_FAIL == ret)
	{
		eDbug("Register statics failed \n");
		return EPDK_FAIL;
	}

	ret = Register_One_Statics(statics_icon_ID,Windows,&icon_ui);
	if(EPDK_FAIL == ret)
	{
		eDbug("Register statics failed \n");
		return EPDK_FAIL;
	}


	ret = Register_One_Button(button_yes_ID,Windows,&yes_ui);
	if(EPDK_FAIL == ret)
	{
		eDbug("Register button failed \n");
		return EPDK_FAIL;
	}
	
		
	ret = Register_One_Button(button_no_ID,Windows,&no_ui);
	if(EPDK_FAIL == ret)
	{
		eDbug("Register button failed \n");
		return EPDK_FAIL;
	}
	

	This_Windows_Button_Show(button_yes_ID,Windows);

	
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

	eDbug("hide window \n");
	
	if(GUI_LYRWIN_STA_ON == GUI_LyrWinGetSta(Windows->layer))
	{
		GUI_LyrWinSetSta(Windows->layer, GUI_LYRWIN_STA_SLEEP);
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

	eDbug("Update_Windows \n");
	

	return EPDK_OK;

}






static __s32 Windows_Dialog_Proc(__gui_msg_t *msg)
{
	switch(msg->id)
	{
		case GUI_MSG_CREATE:
		{
			eDbug("create Windows Dialog \n");
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
			
			eDbug("destory Windows Dialog \n");
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


int Register_Windows_Dialog(__u32 ID,Man_Win *ManWin,RECT *windows_rect,__u8 dialog_type)
{	
	int ret;

	int x = 0,y = 0,width = 0,height = 0;

	x      = windows_rect->x;
	y      = windows_rect->y;
	width  = windows_rect->width;
	height = windows_rect->height;

	
	ret = Register_Windows(ID,ManWin,esKRNL_GetCallBack(Windows_Dialog_Proc),x,y,width,height);	
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

