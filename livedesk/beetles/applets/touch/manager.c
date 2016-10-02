


#include "manager.h"



RECT button_rect = {0,0,800,480};

RECT dialog_rect = {300,200,195,105};


RECT loading_rect = {400,200,100,80};


static __s32 Show_Windows(Man_Win *ManWin)
{
	__u8 ret;
	
	if(NULL == ManWin)
	{
		return EPDK_FAIL;
	}

	if(NULL == ManWin->man_win)
	{
		return EPDK_FAIL;
	}


#if 0
    ret = Register_Windows_B(button_windows,ManWin,&button_rect);
	if(EPDK_FAIL == ret)
	{
		return EPDK_FAIL;	
	}
#endif


#if 0
    ret = Register_Windows_Dialog(dialog_windows,ManWin,&dialog_rect,DIALOG_YES_NO);
	if(EPDK_FAIL == ret)
	{
		return EPDK_FAIL;	
	}
#endif


#if 1
    ret = Register_Windows_Loading(loading_windows,ManWin,&loading_rect);
	if(EPDK_FAIL == ret)
	{
		return EPDK_FAIL;	
	}
#endif


	ret = This_Windows_Show(loading_windows,ManWin);
	if(EPDK_FAIL == ret)
	{
		return EPDK_FAIL;	
	}

	
	return EPDK_OK;

}


static __s32 Manager_Proc(__gui_msg_t* msg)
{

	switch(msg->id)
	{
		case GUI_MSG_CREATE:
		{
			eDbug("create manager \n");
			
			return EPDK_OK;
		}
		
		case GUI_MSG_DESTROY:
		{
			int ret;
			
			Man_Win *ManWin;

			
			ManWin = (Man_Win *)GUI_WinGetAttr(msg->h_deswin);


			if(NULL == ManWin)
			{
				eDbug("ManWin is NULL \n");
				return EPDK_FAIL;
			}
			
			eDbug("destory manager \n");


			ret = Delete_ManWin(ManWin);
			
			if(EPDK_FAIL == ret)
			{
				eDbug("delete manwin is failed \n");
				return EPDK_FAIL;
			}

						
			return EPDK_OK;
		}
		
        case GUI_MSG_KEY:
        {
 			Man_Win *ManWin;
	
			__u8 ret;
			
			ManWin = (Man_Win *)GUI_WinGetAttr(msg->h_deswin);

			if(NULL == ManWin)
			{
				eDbug("ManWin is NULL \n");
				return EPDK_FAIL;
			}		    
		}
		break;


		case GUI_MSG_COMMAND:
		{			
			switch (LOWORD(msg->dwAddData1))
			{
				case button_windows:
				case dialog_windows:
				{
					if(HIWORD(msg->dwAddData1) == SWITCH_TO_MMENU)
					{		
		 	            Command_TO_Parent(msg->h_deswin,SWITCH_TO_MMENU,0,0);
					}
				}
				break;
				
				default:break;
			}

		}
		break;

				
		default:
		{
			break;
		}
	}

	return GUI_ManWinDefaultProc(msg);
}



H_WIN App_Touch_Create(root_para_t *para)
{
	__u8 ret;
	
	Man_Win *ManWin;

	ManWin = In_Malloc(sizeof(Man_Win));
	if(NULL == ManWin)
	{
		return NULL;
	}

	ret = Register_ManWin(APP_TEST_ID,ManWin,para->h_parent,Manager_Proc);
	if(EPDK_FAIL == ret)
	{
		eDbug("register manwin is failed \n");
		return NULL;
	}


	ret = Register_One_Logic_Win(logic_windows,ManWin);
	if(EPDK_FAIL == ret)
	{
		eDbug("register manwin is failed \n");
		return NULL;
	}

	
	ret = Show_Windows(ManWin);
	if(EPDK_FAIL == ret)
	{
		eDbug("Show_Windows is failed \n");
		return NULL;
	}
	
	return ManWin->man_win;
}

