

#include "lib_win.h"



H_WIN GUI_WinGetLastFocusChild (H_WIN hWnd)
{
	H_WIN focus_win = NULL;
	
	while(NULL != GUI_WinGetFocusChild(hWnd))
    {
		focus_win = GUI_WinGetFocusChild(hWnd);
    }
    
    return focus_win;
}


int This_Win_Have_Focus_Child(H_WIN hWnd)
{
	if(NULL != GUI_WinGetFocusChild(hWnd))
	{
		return EPDK_TRUE;
	}
	
	return EPDK_FALSE;
}
