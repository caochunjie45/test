

#ifndef  __WINDOWS_DIALOG_H_
#define  __WINDOWS_DIALOG_H_



#include "..\interface_i.h"

#include "dialog_ui.h"



enum ctrl_dialog_ID
{
	statics_bg_ID = dialog_windows + 1,
	
	statics_icon_ID,
	
	button_yes_ID,
	
	button_no_ID,
	
	button_ok_ID,
	
};




enum dialog_type
{
	DIALOG_YES_NO = 0,

	DIALOG_OK,
};



int Register_Windows_Dialog(__u32 ID,Man_Win *ManWin,RECT *windows_rect,__u8 dialog_type);


#endif

