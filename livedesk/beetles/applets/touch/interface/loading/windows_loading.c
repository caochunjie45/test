
#include "windows_loading.h"



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


	ret = Register_One_Statics(statics_loading_ID,Windows,&loading_ui);
	if(EPDK_FAIL == ret)
	{
		eDbug("Register statics failed \n");
		return EPDK_FAIL;
	}


	ret = Register_One_Statics(statics_loading_text_ID,Windows,&loading_text_ui);
	if(EPDK_FAIL == ret)
	{
		eDbug("Register statics failed \n");
		return EPDK_FAIL;
	}

	Windows->time_ID    = 0x10;
	Windows->time_count = 6;

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

	eDbug("hide window \n");

	
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
	static int i = 0;

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
	

	Set_Statics_Bmp(statics_loading_ID,Windows,i);

	i++;

	if(i > loading_ui.bmp_total)
	{
		i = 0;
	}

	return EPDK_OK;

}






static __s32 Windows_Loading_Proc(__gui_msg_t *msg)
{
	switch(msg->id)
	{
		case GUI_MSG_CREATE:
		{
			eDbug("create Windows Loading \n");
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
			
			eDbug("destory Windows Loading \n");
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
		{

		}
		break;
		
	}


	
	return GUI_FrmWinDefaultProc(msg);

	
}


int Register_Windows_Loading(__u32 ID,Man_Win *ManWin,RECT *windows_rect)
{	
	int ret;

	int x = 0,y = 0,width = 0,height = 0;

	x      = windows_rect->x;
	y      = windows_rect->y;
	width  = windows_rect->width;
	height = windows_rect->height;

	
	ret = Register_Windows(ID,ManWin,esKRNL_GetCallBack(Windows_Loading_Proc),x,y,width,height);	
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

