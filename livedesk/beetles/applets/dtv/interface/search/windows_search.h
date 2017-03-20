
#ifndef  __WINDOWS_SEARCH_H_
#define  __WINDOWS_SEARCH_H_



#include "..\interface_i.h"


#include "search_ui.h"



enum ctrl_search_ID
{
	statics_search_text_ID = search_windows + 1,
	
	statics_valid_frequency_ID,

	progbars_search_ID,
};


enum search_windows_command_ID
{
	Update_Frequency = Exit_Win + 1,	

	Update_Channel,

};

int Register_Windows_Search(__u32 ID,Man_Win *ManWin,RECT *windows_rect);



#endif

