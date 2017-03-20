
#include "logic_manager.h"



static __u32 Windows_Ctrl(Logic_Win *LogicWin, __u8 command,__u32 data1,__u32 data2)
{
	__u32 ret;
	
	ret = Command_TO_Parent(LogicWin->parent,command,data1,data2);

	return ret;
}





static __s32 record_disk_cb(void *res)
{
	H_WIN hwin;	
    __s32 result = 0;

    result = (__s32)res;
	
    switch (result)
    {
        case MAPLE_REC_STAT_NO_DISK:
        {

        }
        break;
        
        case MAPLE_REC_STAT_DISK_FULL:
        {

        }
        break;

        case MAPLE_REC_STAT_DISK_SPACE_NOT_ENOUGH:
        {

        }
        break;
        
        case MAPLE_REC_STAT_ABOVE_MAX:
        {

        }
        break;
            
        default :
		{

		}
        break;
		
    }
}



static __s32 UnInit_Logic(Logic_Win *LogicWin)
{
	__u8 ret;

	logic_data_t        *PulicData = NULL;

	PulicData = (logic_data_t *)(LogicWin->pulic_data);

	
	Windows_Ctrl(LogicWin, Creat_Destory_Win,0,0);


	if(0 != PulicData->search_task_id)
	{
		__u8 err;
		
		__here__;
		Maple_Break_Search();
		__here__;
		Maple_Search_Delete(PulicData->search_task_id);
		
		PulicData->search_task_id = 0;
	}
	else
	{
		__here__;
		Maple_Stop_Program(CLOSE_VIDEO_LAYER);
	}
	__here__;

	dsk_speaker_onoff(SPEAKER_OFF);
	__here__;
	__here__;
	

	ret = Maple_Reg_Flush();
	if(EPDK_FAIL == ret)
	{
		eDbug("Maple_Reg_Flush is failed \n");
		return EPDK_FAIL;
	}

	
	__here__;
	ret = Maple_Close();
	if(EPDK_FAIL == ret)
	{
		eDbug("Maple_Close is failed \n");
		return EPDK_FAIL;
	}

		
	audio_mode_switch(AUDIO_NOR_MODE);
	
	esKRNL_TimeDly(40);//延时一段时间，怀疑模块掉电，会影响到nor的读。

	
	Windows_Ctrl(LogicWin, Destory_Destory_Win,0,0);



	esPWM_UnlockCpuFreq();
	
	set_input_gpio(1, 0, 0);//spi1 cs
	set_input_gpio(1, 1, 0);//spi1 in
	set_input_gpio(1, 2, 0);//ip1 clk
	set_input_gpio(1, 3, 0);//spi1 out
	set_input_gpio(1, 4, 0);//int pin


	dsk_speaker_onoff(SPEAKER_ON);

	return EPDK_OK;
}



static __s32 Init_Logic(Logic_Win *LogicWin)
{
	__u8 ret;

	logic_data_t        *PulicData = NULL;

	if(NULL == LogicWin)
	{
		eDbug("LogicWin is NULL \n");
		return EPDK_FAIL;
	}


	PulicData = (logic_data_t *)(LogicWin->pulic_data);
	if(NULL == PulicData)
	{
		eDbug("PulicData is NULL \n");
		return EPDK_FAIL;		
	}


	
	dsk_speaker_onoff(SPEAKER_OFF);

	// 唤醒CPU
	esPWM_UsrEventNotify();
	esKRNL_TimeDly(5);


	esPWM_LockCpuFreq();

	esPWM_UsrEventNotify();


	audio_mode_switch(AUDIO_FM_MODE);


	Windows_Ctrl(LogicWin, Creat_Loading_Win,0,0);

	Maple_Check_Disk_Format();

	Maple_Set_Logic_Win(LogicWin->logic_win);

	ret = Maple_Open(); 

	if(EPDK_FAIL == ret)
	{
		__wrn("maple open fail...\n");
	   goto error0;
	}
	
	Windows_Ctrl(LogicWin, Destory_Loading_Win,0,0);


	ret = Maple_Register_Search_Callback(Maple_Search_List_Event_Callback,Maple_Search_List_Finish_Callback);
	if(EPDK_FAIL == ret)
	{
		eDbug("Maple_Register_Search_Callback is failed \n");
		return EPDK_FAIL;
	}
	

	ret = Maple_Set_Default_Char_Coding_Type();
	if(EPDK_FAIL == ret)
	{
		eDbug("Maple_Set_Default_Char_Coding_Type is failed \n");
		return EPDK_FAIL;
	}


	ret = Maple_Program_Load(DTV_DATABASE_NAME);

	PulicData->save_data = Maple_Get_Save_Data();


	if(EPDK_OK !=  ret)
	{
		ret = Windows_Ctrl(LogicWin, Creat_Search_Win,0,0);
		if(EPDK_FAIL == ret)
		{
		   goto error2;
		}
		
		ret = Maple_Search(&(PulicData->search_task_id));

		if(EPDK_FAIL == ret)
		{
			return EPDK_FAIL;
		}
		
	}
	else
	{
		ret = Maple_Set_Current_Channel(Maple_Get_Current_Channel_Index());
		if(EPDK_FAIL == ret)
		{
			eDbug("Maple_Set_Current_Channel is failed \n");
			return EPDK_FAIL;
		}

				
		ret = Maple_Stop_Program(NOT_CLOSE_VIDEO_LAYER);
		if(EPDK_FAIL == ret)
		{
			eDbug("Maple_Stop_Program is failed \n");
			return EPDK_FAIL;
		}

		
		ret = Maple_Play_Program();
		if(EPDK_FAIL == ret)
		{
			eDbug("Maple_Play_Program is failed \n");
			return EPDK_FAIL;
		}

		
		if(Maple_Get_Total_Services() >= 1)
		{
			dsk_speaker_onoff(SPEAKER_ON);
		}

	}

	return EPDK_OK;


error2:
	Windows_Ctrl(LogicWin, Destory_Search_Win,0,0);

	Maple_Close();
	
error0:
		
	Windows_Ctrl(LogicWin, Destory_Loading_Win,0,0);

	audio_mode_switch(AUDIO_NOR_MODE);
	
	return EPDK_FAIL;
	
}




__s32 Logic_Ioctrl(Man_Win *ManWin,__u32 cmd, __s32 aux, void* pbuffer)
{
    __s32               result = EPDK_OK;

	logic_data_t        *PulicData = NULL;

	Logic_Win           *LogicWin = NULL;
	
	LogicWin = ManWin->Logic;

	if(NULL == LogicWin)
	{
		eDbug("LogicWin is NULL \n");
		return EPDK_FAIL;
	}

	
	PulicData = (logic_data_t *)Win_Get_Public_Data(ManWin);
	
	if(NULL == PulicData)
	{
		eDbug("PulicData is NULL \n");
		return EPDK_FAIL;
	}

	
    switch(cmd)
    {        
        case LOGIC_GET_VOLUME:
		{
			*(__s32 *)pbuffer = dsk_volume_get();

			eDbug("*(__s32 *)pbuffer is %d \n",*(__s32 *)pbuffer);
			
			return EPDK_OK;
		}
    	break;

		case LOGIC_SET_VLOUME:
		{
			result = dsk_volume_set(aux);

			return result;
		}
		break;

		case LOGIC_GET_VOLTAGE:
		{
		}
		break;


		case LOGIC_GET_SINGAL_STATUS:
		{
			*(__bool *)pbuffer = PulicData->weak_singal;

			eDbug("*(__bool *)pbuffer is %d \n",*(__bool *)pbuffer);

			return EPDK_OK;
		}
		break;


		case LOGIC_SET_ANTNNA_STATUS:
		{
			Maple_Set_Antenna_State(aux);

			PulicData->save_data->antenna_status = aux;
			
			return EPDK_OK;
		}
		break;

			
		case LOGIC_GET_ANTENA_STATUS:
		{
			*(__bool *)pbuffer = PulicData->save_data->antenna_status;
		
			eDbug("*(__bool *)pbuffer is %d \n",*(__bool *)pbuffer);

			return EPDK_OK;
		}
		break;


		case LOGIC_SET_LCN_STATUS:
		{
			PulicData->save_data->LCN_status = aux;
			
			return EPDK_OK;
		}
		break;

			
		case LOGIC_GET_LCN_STATUS:
		{
			*(__bool *)pbuffer = PulicData->save_data->LCN_status;
		
			eDbug("*(__bool *)pbuffer is %d \n",*(__bool *)pbuffer);

			return EPDK_OK;
		}
		break;

		
		case LOGIC_GET_SINGNAL_STRENGTH:
		{
			*(__s32 *)pbuffer = PulicData->strength;

			eDbug("*(__s32 *)pbuffer is %d \n",*(__s32 *)pbuffer);

			return EPDK_OK;
		}
		break;


		case LOGIC_GET_SINGNAL_STRENGTH_LEVEL:
		{
			if(PulicData->strength <= SINGAL_LEVEL0)
			{
				*(__s32 *)pbuffer = 0;
			}
			else if(PulicData->strength > SINGAL_LEVEL0 && PulicData->strength <= SINGAL_LEVEL1)
			{
				*(__s32 *)pbuffer = 1;
			}
			else if(PulicData->strength > SINGAL_LEVEL1 && PulicData->strength <= SINGAL_LEVEL2)
			{
				*(__s32 *)pbuffer = 2;
			}
			else if(PulicData->strength > SINGAL_LEVEL2 && PulicData->strength <= SINGAL_LEVEL3)
			{
				*(__s32 *)pbuffer = 3;
			}
			else if(PulicData->strength > SINGAL_LEVEL3 && PulicData->strength <= SINGAL_LEVEL4)
			{
				*(__s32 *)pbuffer = 4;
			}	
			else if(PulicData->strength > SINGAL_LEVEL4 && PulicData->strength <= SINGAL_LEVEL5)
			{
				*(__s32 *)pbuffer = 5;
			}	
			else
			{
				eDbug("the value[%d] of g_strength is out 0~100!\n",PulicData->strength);
			}

			eDbug("*(__s32 *)pbuffer is %d \n",*(__s32 *)pbuffer);

			return EPDK_OK;
		}
		break;

		
		case LOGIC_GET_SINGNAL_QUALITY:
		{
			*(__s32 *)pbuffer = PulicData->quality;

			eDbug("*(__s32 *)pbuffer is %d \n",*(__s32 *)pbuffer);

			return EPDK_OK;		
		}
		break;


		case LOGIC_GET_TITLE_CHANNEL_NAME:
		{
			char *name;
			
			if(PulicData->save_data->channel_total <= 0)
			{
				dsk_langres_get_menu_text(STRING_DTV_NO_CHANNEL,name,sizeof(name));
			}
			else
			{
				name = Maple_Get_Name_Of_Channel(Maple_Get_Current_Channel_Index());
				if(NULL == name)
				{
					eDbug("name is NULL \n");
					return EPDK_FAIL;
				}
			}

			eLIBs_strncpy((char *)pbuffer,name,STRING_MAX);

			eDbug("name is %s \n",name);

			return EPDK_OK;
		}
		break;


		case LOGIC_GET_LIST_CHANNEL_NAME:
		{
			char *name;
			
			char server_name[STRING_MAX] = {0};

			__s32  nRemoteControlKeyId = 0;

			name = Maple_Get_Name_Of_Channel(Maple_Get_Current_Channel_Index());
			if(NULL == name)
			{
				eDbug("name is NULL \n");
				return EPDK_FAIL;
			}

			if(EPDK_TRUE == PulicData->save_data->LCN_status)
			{
				nRemoteControlKeyId = Maple_Get_Current_Channel_LCN_ID(aux);
				if(EPDK_FAIL == nRemoteControlKeyId)
				{
					eDbug("nRemoteControlKeyId is NULL \n");
					return EPDK_FAIL;
				}
				
				eLIBs_sprintf(server_name," %d %s", nRemoteControlKeyId,name);
			}
			else
			{
				eLIBs_sprintf(server_name," %d %s",aux + 1,name);
			}
			
			eLIBs_strncpy((char *)pbuffer,server_name,STRING_MAX);

			eDbug("server_name is %s \n",server_name);
			
			return EPDK_OK;
		}
		break;


		case LOGIC_GET_SIMPLE_EPG:
		{
			maple_schdl_event_t *shdl_event = NULL;
		
			shdl_event = Maple_Get_Schedule_Event(0,aux);
			if(NULL == shdl_event)
			{
				eDbug("shdl_event is NULL \n");
				
				return EPDK_FAIL;
			}
					
			eLIBs_strncpy((char *)pbuffer,shdl_event->name,STRING_MAX);
		
			eDbug("shdl_event->name is %s \n",shdl_event->name);
			
			return EPDK_OK;
		}
		break;


		case LOGIC_GET_LONG_EPG:
		{
			maple_schdl_event_t *shdl_event = NULL;
		
			shdl_event = Maple_Get_Schedule_Event(0,aux);
			if(NULL == shdl_event)
			{
				eDbug("shdl_event is NULL \n");
				
				return EPDK_FAIL;
			}
					
			eLIBs_strncpy((char *)pbuffer,shdl_event->eventContent,1024);
		
			eDbug("shdl_event->name is %s \n",shdl_event->eventContent);
			
			return EPDK_OK;
		}
		break;

		
		case LOGIC_GET_TOTAL_FREQUENCY:
		{
			*(__s32 *)pbuffer = Maple_Get_Total_Frequency();

			eDbug("*(__s32 *)pbuffer is %d \n",*(__s32 *)pbuffer);

			return EPDK_OK;		
		}
		break;


		case LOGIC_GET_CURRENT_FREQUENCY:
		{
			*(__s32 *)pbuffer = Maple_Get_Current_Search_Frequency(aux);
		
			eDbug("*(__s32 *)pbuffer is %d \n",*(__s32 *)pbuffer);

			return EPDK_OK; 	
		}
		break;


		case LOGIC_PREV_CHANNEL:
		{
			__u8 index = 0;
			
			result = Logic_Ioctrl(ManWin,LOGIC_STOP_RECORD,0,NULL);
			if(EPDK_FAIL == result)
			{
				eDbug("Logic_Ioctrl is failed \n");
				return EPDK_FAIL;
			}
			
			index = Maple_Get_Current_Channel_Index();
			
			index--;
			
			if(index < 0)
			{
				index = Maple_Get_Total_Services() - 1;
				if(index < 0)
				{
					index = 0;
				}
			}
			
			result = Maple_Set_Current_Channel(index);
			if(EPDK_FAIL == result)
			{
				eDbug("Maple_Set_Current_Channel is failed \n");
				return EPDK_FAIL;
			}
			
			result = Maple_Stop_Program(CLOSE_VIDEO_LAYER);
			if(EPDK_FAIL == result)
			{
				eDbug("Maple_Stop_Program is failed \n");
				return EPDK_FAIL;
			}
			
			result = Maple_Play_Program();
			if(EPDK_FAIL == result)
			{
				eDbug("Maple_Play_Program is failed \n");
				return EPDK_FAIL;
			}
			
			Windows_Ctrl(LogicWin,Create_Singal_Win,0,0);

			return EPDK_OK; 	
		}
		break;
		
		case LOGIC_NEXT_CHANNEL:
		{
			__u8 index = 0;

			result = Logic_Ioctrl(ManWin,LOGIC_STOP_RECORD,0,NULL);
			if(EPDK_FAIL == result)
			{
				eDbug("Logic_Ioctrl is failed \n");
				return EPDK_FAIL;
			}
			
			index = Maple_Get_Current_Channel_Index();
			
			index++;
			
			if(index > (Maple_Get_Total_Services() - 1))
			{
				index = 0;
			}
			
			result = Maple_Set_Current_Channel(index);
			if(EPDK_FAIL == result)
			{
				eDbug("Maple_Set_Current_Channel is failed \n");
				return EPDK_FAIL;
			}
			
			result = Maple_Stop_Program(CLOSE_VIDEO_LAYER);
			if(EPDK_FAIL == result)
			{
				eDbug("Maple_Stop_Program is failed \n");
				return EPDK_FAIL;
			}
			
			result = Maple_Play_Program();
			if(EPDK_FAIL == result)
			{
				eDbug("Maple_Play_Program is failed \n");
				return EPDK_FAIL;
			}
			
			Windows_Ctrl(LogicWin,Create_Singal_Win,0,0);

			return EPDK_OK; 	
		}
		break;


		case LOGIC_CHANGE_CHANNEL:
		{
			result = Logic_Ioctrl(ManWin,LOGIC_STOP_RECORD,0,NULL);
			if(EPDK_FAIL == result)
			{
				eDbug("Logic_Ioctrl is failed \n");
				return EPDK_FAIL;
			}
			
			result = Maple_Set_Current_Channel(aux);
			if(EPDK_FAIL == result)
			{
				eDbug("Maple_Set_Current_Channel is failed \n");
				return EPDK_FAIL;
			}
			
			result = Maple_Stop_Program(CLOSE_VIDEO_LAYER);
			if(EPDK_FAIL == result)
			{
				eDbug("Maple_Stop_Program is failed \n");
				return EPDK_FAIL;
			}
			
			result = Maple_Play_Program();
			if(EPDK_FAIL == result)
			{
				eDbug("Maple_Play_Program is failed \n");
				return EPDK_FAIL;
			}

			Windows_Ctrl(LogicWin,Create_Singal_Win,0,0);
		
			return EPDK_OK; 	
		}
		break;


		case LOGIC_DIGIT_SELECT_CHANNEL:
		{
			
			__s32 ret;
			
			__s32 number = 0;
			
			__s32 channel_number = 0;


			number = aux;
			
			
			if(EPDK_TRUE == PulicData->save_data->LCN_status)
			{
				ret = Maple_Get_Current_Channel_Index_By_LCN(number);
				
			 	if(EPDK_FAIL == ret)
			 	{
					Windows_Ctrl(LogicWin, Create_Dialog_Win,0,0);
			 	}
			 	else
			 	{
					channel_number = ret + 1; // 下面减了1
					
					result = Logic_Ioctrl(ManWin,LOGIC_STOP_RECORD,0,NULL);
					if(EPDK_FAIL == result)
					{
						eDbug("Logic_Ioctrl is failed \n");
						return EPDK_FAIL;
					}

				 	result = Maple_Set_Current_Channel(channel_number - 1);
					if(EPDK_FAIL == result)
					{
						eDbug("Maple_Set_Current_Channel is failed \n");
						return EPDK_FAIL;
					}
					
				 	dsk_speaker_onoff(SPEAKER_OFF);

					result = Maple_Stop_Program(CLOSE_VIDEO_LAYER);
					if(EPDK_FAIL == result)
					{
						eDbug("Maple_Stop_Program is failed \n");
						return EPDK_FAIL;
					}

				 	result = Maple_Play_Program();
					if(EPDK_FAIL == result)
					{
						eDbug("Maple_Play_Program is failed \n");
						return EPDK_FAIL;
					}

				 	if(Maple_Get_Total_Services() >= 1)
				 	{
					 	dsk_speaker_onoff(SPEAKER_ON);
				 	}
					
					Windows_Ctrl(LogicWin,Create_Singal_Win,0,0);
				 
			 	}
			}
		
			else
			{
				channel_number = number;
				
				if(channel_number !=0 && channel_number <= Maple_Get_Total_Services())
				{
					result = Logic_Ioctrl(ManWin,LOGIC_STOP_RECORD,0,NULL);
					if(EPDK_FAIL == result)
					{
						eDbug("Logic_Ioctrl is failed \n");
						return EPDK_FAIL;
					}
					 
					result = Maple_Set_Current_Channel(channel_number - 1);
					if(EPDK_FAIL == result)
					{
						eDbug("Logic_Ioctrl is failed \n");
						return EPDK_FAIL;
					}

					
					dsk_speaker_onoff(SPEAKER_OFF);
					
					result = Maple_Stop_Program(CLOSE_VIDEO_LAYER);
					if(EPDK_FAIL == result)
					{
						eDbug("Maple_Stop_Program is failed \n");
						return EPDK_FAIL;
					}
					
					result = Maple_Play_Program();
					if(EPDK_FAIL == result)
					{
						eDbug("Maple_Play_Program is failed \n");
						return EPDK_FAIL;
					}
					
					if(Maple_Get_Total_Services() >= 1)
					{
						dsk_speaker_onoff(SPEAKER_ON);
					}
					
					Windows_Ctrl(LogicWin,Create_Singal_Win,0,0);
					 
				}
				else
				{
					Windows_Ctrl(LogicWin, Create_Dialog_Win,0,0);
				}
			}

			return EPDK_OK; 

		}
		break;


		case LOGIC_START_RECORD:
		{
			
			result = Maple_Check_Device();
			
			if(NULL == result)
			{
				Windows_Ctrl(LogicWin, Create_Dialog_Win,0,0);
				
				return EPDK_OK;
			}
			
			Windows_Ctrl(LogicWin,Create_Record_Win,0,0);
			
			
			//启动录制
			if(EPDK_TRUE == Maple_Control_TS_Record_Start())
			{
				PulicData->record_status = EPDK_TRUE;
			}
			
			result = Maple_Set_Record_Disk_Full_CB(record_disk_cb);
			if(EPDK_FAIL == result)
			{
				eDbug("Maple_Set_Record_Disk_Full_CB is failed \n");
				return EPDK_FAIL;
			}
			
			return EPDK_OK;

		}
		break;

		case LOGIC_STOP_RECORD:
		{
			if(EPDK_TRUE == PulicData->record_status)
			{
				if(EPDK_TRUE == Maple_Control_TS_Record_Stop())
				{
					PulicData->record_status = EPDK_FALSE;						
				}
				
				rat_set_modify_flag(RAT_MEDIA_TYPE_VIDEO, 1);
						
				Windows_Ctrl(LogicWin, Create_Dialog_Win,0,0);

				return EPDK_OK;
				
			}
		
			return EPDK_OK;

		}
		break;
    }
    
    return result;
}

static __s32 Logic_Proc(__gui_msg_t* msg)
{


	switch(msg->id)
	{
		case GUI_MSG_CREATE:
		{
			eDbug("create logic win \n");
			
			return EPDK_OK;
		}
		break;
		
		case GUI_MSG_DESTROY:
		{
			__u8 ret;
			
			Logic_Win           *LogicWin = NULL;

			eDbug("destory logic win \n");

			LogicWin = (Logic_Win *)GUI_WinGetAttr(msg->h_deswin);
		
			if(NULL == LogicWin)
			{
				eDbug("LogicWin is NULL \n");
				return EPDK_FAIL;
			}


			ret = UnInit_Logic(LogicWin);
			if(EPDK_FAIL == ret)
			{
				eDbug("Init_Logic is failed \n");
				return EPDK_FAIL;
			}

			
			return EPDK_OK;
		}
		break;

		
		case GUI_MSG_COMMAND:
		{
			Logic_Win           *LogicWin = NULL;

			logic_data_t		*PulicData = NULL;
			
			LogicWin = (Logic_Win *)GUI_WinGetAttr(msg->h_deswin);
		
			if(NULL == LogicWin)
			{
				eDbug("LogicWin is NULL \n");
				return EPDK_FAIL;
			}
			
			
			PulicData = (logic_data_t *)(LogicWin->pulic_data);
			if(NULL == PulicData)
			{
				eDbug("PulicData is NULL \n");
				return EPDK_FAIL;				
			}

			
			switch (LOWORD(msg->dwAddData1))
			{
				case DELETE_SEARCH_THREAD:
				{
					Windows_Ctrl(LogicWin, Destory_Search_Win,0,0);

					Maple_Search_Delete(PulicData->search_task_id);
					
					PulicData->search_task_id = 0;
					
					Maple_Play_Program();

				}
				break;

				case UPDATE_FREQ:
				{
					Windows_Ctrl(LogicWin, Update_Search_Frequency,HIWORD(msg->dwAddData1),msg->dwAddData2);

				}
				break;

				case UPDATE_CHANNEL:
				{
					Windows_Ctrl(LogicWin, Update_Search_Channel,HIWORD(msg->dwAddData1),msg->dwAddData2);

				}
				break;
			}
			
			return EPDK_OK;
		}
		break;

		default:
		{

		}
		break;
		
	}
		
}


int Free_Logic_Data(void *pulic_data)
{

	logic_data_t *PulicData = NULL;
	
	if(NULL == pulic_data)
	{
		eDbug("pulic_data is NULL \n");
		return EPDK_FAIL;
	}

	PulicData = (logic_data_t *)pulic_data;	

	eDbug("free pulicdata \n");
	
	In_Free(PulicData,sizeof(logic_data_t));
	
}


int Register_One_Logic_Win(__u32 ID,Man_Win *ManWin)
{
	__u8 ret;
	
	logic_data_t *logic_data = NULL;


	logic_data = In_Malloc(sizeof(logic_data_t));

	if(NULL == logic_data)
	{
		eDbug("logic_data is NULL \n");
		return EPDK_FAIL;		
	}

	
	ret = Register_LogicWin(ID,ManWin,esKRNL_GetCallBack(Logic_Proc));

	if(EPDK_FAIL == ret)
	{
		eDbug("register LogicWin is failed \n");
		return EPDK_FAIL;
	}


	
	ret = Win_Set_Public_Data(ManWin,(void *)logic_data);
	if(EPDK_FAIL == ret)
	{
		eDbug("Win_Set_Public_Data is failed \n");
		return EPDK_FAIL;
	}


	
	ret = Logic_Win_Set_Destory_CallBack(ManWin,esKRNL_GetCallBack(Free_Logic_Data));
	if(EPDK_FAIL == ret)
	{
		eDbug("Logic_Win_Set_Destory_CallBack is failed \n");
		return EPDK_FAIL;
	}
	

	ret = Init_Logic(ManWin->Logic);
	if(EPDK_FAIL == ret)
	{
		eDbug("Init_Logic is failed \n");
		return EPDK_FAIL;
	}


	
	return EPDK_OK;

	
}

