
#ifndef  __WINDOWS_BUTTON_H_
#define  __WINDOWS_BUTTON_H_

#include "beetles_app.h"

#include "..\app_touch_i.h"

#include "ui.h"



enum ctrl_ID
{
	button_menu_ID = button_windows + 1,
	
	button_edit_ID,
	
	button_help_ID,
	
	button_project_ID,
	
	listbar_setting_ID,
	
	slider_volume_ID,

	statics_voltage_ID,
	
};


int Register_Windows_B(__u32 ID,Man_Win *ManWin,RECT *windows_rect);


#endif

