#ifndef  __INTERFACE_H_
#define  __INTERFACE_H_



#include "..\apps_i.h"


enum
{
	search_windows = APP_DTV_ID + 1,
	
	dialog_windows,
	
	loading_windows,
	
	digit_windows,
	
	button_windows,
};



enum windows_command_ID
{
	Exit_Win = 0,
};




#include "button\windows_button.h"
#include "dialog\windows_dialog.h"
#include "loading\windows_loading.h"
#include "search\windows_search.h"


#endif


