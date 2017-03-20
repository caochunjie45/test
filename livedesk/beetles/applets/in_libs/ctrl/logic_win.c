

#include "logic_win.h"




void *Win_Get_Public_Data(Man_Win *ManWin)
{
	if(NULL == ManWin)
	{
		eDbug("LogicWin is NULL \n");
		return NULL;
	}

	if(NULL == ManWin->Logic)
	{
		eDbug("ManWin->Logic is NULL \n");
		return NULL;
	}

	if(NULL == ManWin->Logic->pulic_data)
	{
		eDbug("ManWin->Logic->pulic_data is NULL \n");
		return NULL;
	}
	
    return (ManWin->Logic->pulic_data);
    
}   



__s32 Win_Set_Public_Data(Man_Win *ManWin,void *Data)
{
	if(NULL == ManWin)
	{
		eDbug("LogicWin is NULL \n");
		return EPDK_FAIL;
	}

	if(NULL == ManWin->Logic)
	{
		eDbug("ManWin->Logic is NULL \n");
		return EPDK_FAIL;
	}


	esKRNL_SemPend(ManWin->Logic->sem,0,NULL);

	ManWin->Logic->pulic_data = Data;

	esKRNL_SemPost(ManWin->Logic->sem);

	return EPDK_OK;
    
}


__s32 Logic_Win_Set_Destory_CallBack(Man_Win *ManWin,__pCBK_t Destory)
{
	if(NULL == ManWin)
	{
		eDbug("LogicWin is NULL \n");
		return EPDK_FAIL;
	}

	if(NULL == ManWin->Logic)
	{
		eDbug("ManWin->Logic is NULL \n");
		return EPDK_FAIL;
	}


	ManWin->Logic->destory = Destory;

	return EPDK_OK;
    
}


H_WIN LogicWin_Create(Logic_Win *LogicWin,__pGUI_WIN_CB proc)
{
	__gui_manwincreate_para_t create_info;
	
    H_WIN hLogicWin;

    
	eLIBs_memset(&create_info, 0, sizeof(__gui_manwincreate_para_t));
	
	create_info.name            = LogicWin->name;
	create_info.hParent         = LogicWin->parent;
	create_info.ManWindowProc   = (__pGUI_WIN_CB)esKRNL_GetCallBack((__pCBK_t)proc); 
	create_info.attr            = (void*)LogicWin;
	create_info.id				= LogicWin->id;
	create_info.hHosting        = NULL;


    hLogicWin = GUI_ManWinCreate(&create_info);
    
	return hLogicWin;
}


int Delete_LogicWin(Logic_Win *LogicWin)
{
	__u8 err;

	if(NULL == LogicWin)
	{
		eDbug("LogicWin is NULL \n");
		return EPDK_FAIL;
	}

	if(NULL != LogicWin->logic_win)
	{
		GUI_ManWinDelete(LogicWin->logic_win);

		LogicWin->logic_win = NULL;
	}

	if(NULL != LogicWin->pulic_data)
	{
		LogicWin->destory(LogicWin->pulic_data);

		LogicWin->pulic_data = NULL;
	}
	
	esKRNL_SemDel(LogicWin->sem, OS_DEL_ALWAYS, &err);

	LogicWin->sem = NULL;
	
	In_Free(LogicWin,sizeof(Logic_Win));

	return EPDK_OK;

	
}



int Register_LogicWin(__u32 ID,Man_Win *ManWin,__pGUI_WIN_CB Proc)
{

	Logic_Win *LogicWin = NULL;

	LogicWin = In_Malloc(sizeof(Logic_Win));

	
	if(NULL == LogicWin)
	{
		return EPDK_FAIL;
	}


	LogicWin->id = ID;
	
	eLIBs_strcpy(LogicWin->name,"Logic");
	
	LogicWin->parent = ManWin->man_win;
	
	if(NULL == LogicWin->parent)
	{
		eDbug("LogicWin->parent is NULL \n");
		return EPDK_FAIL;
	}
	
	
	LogicWin->logic_win = LogicWin_Create(LogicWin,Proc);
	
	if(NULL == LogicWin->logic_win)
	{
		eDbug("LogicWin->logic_win is NULL \n");
		return EPDK_FAIL;
	}

	ManWin->Logic = LogicWin;
	
	eDbug("LogicWin->logic_win is 0x%x \n",LogicWin->logic_win);

	LogicWin->sem = esKRNL_SemCreate(1);

	return EPDK_OK;

}

