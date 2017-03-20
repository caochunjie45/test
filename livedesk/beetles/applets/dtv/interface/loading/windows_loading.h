
#ifndef  __WINDOWS_LOADING_H_
#define  __WINDOWS_LOADING_H_



#include "..\interface_i.h"


#include "loading_ui.h"



enum ctrl_loading_ID
{
	statics_loading_ID = loading_windows + 1,
	
	statics_loading_text_ID,
	
};


int Register_Windows_Loading(__u32 ID,Man_Win *ManWin,RECT *windows_rect);



#endif

