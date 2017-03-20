


#include "manager.h"





__s32 Manager_Ioctrl(H_WIN win,__u32 cmd, __s32 aux, void *pbuffer)
{
	__u8 ret;

	Man_Win *ManWin = NULL;

	ManWin = (Man_Win *)GUI_WinGetAttr(win);

	if(NULL == ManWin)
	{
		eDbug("ManWin is NULL \n");
		return EPDK_FAIL;
	}
	
	ret = Logic_Ioctrl(ManWin,cmd,aux,pbuffer);

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
			__u8 ret;

			Man_Win *ManWin;
			
			ManWin = (Man_Win *)GUI_WinGetAttr(msg->h_deswin);


			if(NULL == ManWin)
			{
				eDbug("ManWin is NULL \n");
				return EPDK_FAIL;
			}
			
			switch (LOWORD(msg->dwAddData1))
			{
				case search_windows:
				{
					switch(HIWORD(msg->dwAddData1))
					{
						case Exit_Win:
						{
							Delete_One_Windows(LOWORD(msg->dwAddData1),ManWin);
							return EPDK_OK;
						}
						break;


						default:
						{

						}
						break;
					}
				}
				break;
				
				case loading_windows:
				{
					switch(HIWORD(msg->dwAddData1))
					{
						case Exit_Win:
						{
							Delete_One_Windows(LOWORD(msg->dwAddData1),ManWin);
							return EPDK_OK;
						}
						break;


						default:
						{

						}
						break;
					}
				}
				break;


				case logic_windows:
				{
					switch(HIWORD(msg->dwAddData1))
					{
						case Creat_Search_Win:
						{
							ret = Register_Windows_Search(search_windows,ManWin,&search_rect);
							if(EPDK_FAIL == ret)
							{
								return EPDK_FAIL;	
							}

							ret = This_Windows_Show(search_windows,ManWin);
							if(EPDK_FAIL == ret)
							{
								return EPDK_FAIL;	
							}
							
						}
						break;

						case Update_Search_Frequency:
						{
							Command_TO_Child_Win(ManWin->man_win,search_windows,Update_Frequency,msg->dwAddData2,msg->dwReserved);

						}
						break;

						case Update_Search_Channel:
						{
							Command_TO_Child_Win(ManWin->man_win,search_windows,Update_Channel,msg->dwAddData2,msg->dwReserved);
						}
						break;

						case Destory_Search_Win:
						{
							Delete_One_Windows(search_windows,ManWin);
							return EPDK_OK;
						}
						break;


						case Creat_Loading_Win:
						{
							ret = Register_Windows_Loading(loading_windows,ManWin,&loading_rect);
							if(EPDK_FAIL == ret)
							{
								return EPDK_FAIL;	
							}
							
							ret = This_Windows_Show(loading_windows,ManWin);
							if(EPDK_FAIL == ret)
							{
								return EPDK_FAIL;	
							}
						
						}
						break;
						
						case Destory_Loading_Win:
						{
							Delete_One_Windows(loading_windows,ManWin);
							return EPDK_OK;
						}
						break;


						case Create_Singal_Win:
						{
						
						}
						break;
						
						case Destory_Singal_Win:
						{
							return EPDK_OK;
						}
						break;

						case Create_Dialog_Win:
						{
						
						}
						break;

						case Destory_Dialog_Win:
						{
						
						}
						break;


						case Creat_Destory_Win:
						{
						
						}
						break;

						case Destory_Destory_Win:
						{
						
						}
						break;


						case Create_Record_Win:
						{
						
						}
						break;

						case Destory_Record_Win:
						{
						
						}
						break;
						
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



H_WIN app_dtv_create(root_para_t *para)
{
	__u8 ret;
	
	Man_Win *ManWin;

	ManWin = In_Malloc(sizeof(Man_Win));
	if(NULL == ManWin)
	{
		return NULL;
	}


	gscene_hbar_set_state(HBAR_ST_HIDE);
	
	gscene_bgd_set_state(BGD_STATUS_HIDE);


	ret = Register_ManWin(APP_DTV_ID,ManWin,para->h_parent,Manager_Proc);
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

	
	return ManWin->man_win;

}

