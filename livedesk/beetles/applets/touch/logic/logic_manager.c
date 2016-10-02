
#include "logic_manager.h"



static __s32 Logic_Proc(__gui_msg_t* msg)
{

	switch(msg->id)
	{
		case GUI_MSG_CREATE:
		{
			eDbug("create logic win \n");
			
			return EPDK_OK;
		}
		
		case GUI_MSG_DESTROY:
		{
			eDbug("destory logic win \n");
			
			return EPDK_OK;
		}
	}
		
}


int Free_Pulic_Data(void *pulic_data)
{

	logic_pulic_data_t *PulicData = NULL;
	
	if(NULL == pulic_data)
	{
		eDbug("pulic_data is NULL \n");
		return EPDK_FAIL;
	}

	PulicData = (logic_pulic_data_t *)pulic_data;	

	eDbug("free pulicdata \n");
	
	In_Free(PulicData,sizeof(logic_pulic_data_t));
	
}


int Register_One_Logic_Win(__u32 ID,Man_Win *ManWin)
{
	__u8 ret;
	
	logic_pulic_data_t *logic_data = NULL;


	logic_data = In_Malloc(sizeof(logic_pulic_data_t));

	if(NULL == logic_data)
	{
		eDbug("logic_data is NULL \n");
		return EPDK_FAIL;		
	}

	logic_data->light = 12;

	logic_data->volume = 15;

	logic_data->voltage = 1;

	
	ret = Register_LogicWin(ID,ManWin,esKRNL_GetCallBack(Logic_Proc));

	if(EPDK_FAIL == ret)
	{
		eDbug("register LogicWin is failed \n");
		return EPDK_FAIL;
	}


	
	ret = WinSetPublicData(ManWin,(void *)logic_data);
	if(EPDK_FAIL == ret)
	{
		eDbug("WinSetPublicData is failed \n");
		return EPDK_FAIL;
	}
	
	ret = LodicWinSetDestoryCallBack(ManWin,esKRNL_GetCallBack(Free_Pulic_Data));
	if(EPDK_FAIL == ret)
	{
		eDbug("LodicWinSetDestoryCallBack is failed \n");
		return EPDK_FAIL;
	}
	
	
	return EPDK_OK;

	
}

