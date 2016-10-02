
#include "man_win.h"



int Delete_ManWin(Man_Win *ManWin)
{

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

	
	Dlist_Destroy(ManWin->windows_head);
	
	ManWin->windows_head  = NULL;

	ManWin->focus_windows = NULL;

	if(NULL != ManWin->Logic)
	{
		
		Delete_LogicWin(ManWin->Logic);
		
		ManWin->Logic = NULL;

	}


ManWin:

	In_Free(ManWin,sizeof(Man_Win));				


	Leak_Info();	

	return EPDK_OK;

	
}


H_WIN Manwin_Create(Man_Win *ManWin,__pGUI_WIN_CB proc)
{
	__gui_manwincreate_para_t create_info;
    H_WIN hManWin;

    
	eLIBs_memset(&create_info, 0, sizeof(__gui_manwincreate_para_t));
	
	create_info.name            = ManWin->name;
	create_info.hParent         = ManWin->parent;
	create_info.ManWindowProc   = (__pGUI_WIN_CB)esKRNL_GetCallBack((__pCBK_t)proc); 
	create_info.attr            = (void*)ManWin;
	create_info.id				= ManWin->id;
	create_info.hHosting        = NULL;

    hManWin = GUI_ManWinCreate(&create_info);
    
	return hManWin;
}




int Register_ManWin(__u32 ID,Man_Win *ManWin,H_WIN parent,__pGUI_WIN_CB Proc)
{


	ManWin->id = ID;
	
	strcpy(ManWin->name,"app touch");
	
	ManWin->parent = parent;
	
	if(NULL == ManWin->parent)
	{
		eDbug("ManWin->parent is NULL \n");
		return EPDK_FAIL;
	}
	
	ManWin->windows_head = Create_Windows_List();
	
	if(NULL == ManWin->windows_head)
	{
		eDbug("ManWin->windows_head is NULL \n");
		return EPDK_FAIL;
	}
	
	ManWin->windows_focus_id = 0;
	
	ManWin->man_win = Manwin_Create(ManWin,Proc);	
	if(NULL == ManWin->man_win)
	{
		eDbug("ManWin->man_win is NULL \n");
		return EPDK_FAIL;
	}
	
	eDbug("ManWin->man_win is 0x%x \n",ManWin->man_win);

	return EPDK_OK;

}



