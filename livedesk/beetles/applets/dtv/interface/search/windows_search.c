
#include "windows_search.h"



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


static __s32 Progbars_Return(__gui_msg_t * msg)
{
	__u8 ret;

	windows_t *Windows;
	

	Windows = (windows_t *)GUI_WinGetAttr(msg->h_deswin);


	if(NULL == Windows)
	{
		eDbug("Windows is NULL \n");
		return EPDK_FAIL;
	}

	eDbug("into progbars return callback \n");

	
	Command_TO_Parent(Windows->framewin,Exit_Win,0,0);

	return EPDK_OK;
}


static __s32 Register_One_Progbars(__u32 ID,windows_t *Windows,progbars_ui_t *progbars_ui)
{
	__u8 ret;

	ret = Register_Progbars(ID,Windows,progbars_ui);
	
	if(EPDK_FAIL == ret)
	{
		eDbug("Register progbars failed \n");
		return EPDK_FAIL;
	}	

	ret = Set_Progbars_Return_CallBack(ID,Windows,esKRNL_GetCallBack(Progbars_Return));
	if(EPDK_FAIL == ret)
	{
		eDbug("Set Button Left CallBack failed \n");
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


	ret = Register_One_Statics(statics_search_text_ID,Windows,&search_text_ui);
	if(EPDK_FAIL == ret)
	{
		eDbug("Register statics failed \n");
		return EPDK_FAIL;
	}


	ret = Register_One_Statics(statics_valid_frequency_ID,Windows,&valid_frequency_text_ui);
	if(EPDK_FAIL == ret)
	{
		eDbug("Register statics failed \n");
		return EPDK_FAIL;
	}

	ret = Register_One_Progbars(progbars_search_ID,Windows,&search_proc_ui);
	if(EPDK_FAIL == ret)
	{
		eDbug("Register progbars failed \n");
		return EPDK_FAIL;
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


	eDbug("Update_Windows \n");
	


	return EPDK_OK;

}






static __s32 Windows_Search_Proc(__gui_msg_t *msg)
{
	switch(msg->id)
	{
		case GUI_MSG_CREATE:
		{
			eDbug("create Windows search \n");
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
			
			
			eDbug("destory Windows search \n");
		}
		return EPDK_OK;

		case GUI_MSG_COMMAND:
		{
			windows_t *Windows;
			
			Windows = (windows_t *)GUI_WinGetAttr(msg->h_deswin);

			if(NULL == Windows)
			{
				eDbug("Windows is NULL \n");
				return EPDK_FAIL;
			}

			switch(LOWORD(msg->dwAddData1))
			{
				case Update_Frequency:
				{
					char string[STRING_MAX];

					char frequency[STRING_MAX];
					
					dsk_langres_get_menu_text(STRING_DTV_SEARCH_NOW,string,STRING_MAX);

					eLIBs_strcat(string,":");
					
					eLIBs_sprintf(frequency,"%d.%03d MHz",HIWORD(msg->dwAddData1)/1000,HIWORD(msg->dwAddData1)%1000);

					eLIBs_strcat(string,frequency);
					
					Set_Statics_Text(statics_search_text_ID,Windows,string,0);
					
				}
				break;
				
				case Update_Channel:
				{
					char string[STRING_MAX];
					
					dsk_langres_get_menu_text(STRING_DTV_SEARCH_VALID_CH,string,STRING_MAX);
					
					eLIBs_sprintf(string,"%s:%d\n",string,HIWORD(msg->dwAddData1));
					
					Set_Statics_Text(statics_valid_frequency_ID,Windows,string,0);

				}
				break;

				default:
				{

				}
				break;
			}

		}
		break;

		default:
		{

		}
		break;
		
	}


	
	return GUI_FrmWinDefaultProc(msg);

	
}


int Register_Windows_Search(__u32 ID,Man_Win *ManWin,RECT *windows_rect)
{	
	int ret;

	int x = 0,y = 0,width = 0,height = 0;

	x      = windows_rect->x;
	y      = windows_rect->y;
	width  = windows_rect->width;
	height = windows_rect->height;

	
	ret = Register_Windows(ID,ManWin,esKRNL_GetCallBack(Windows_Search_Proc),x,y,width,height);	
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

