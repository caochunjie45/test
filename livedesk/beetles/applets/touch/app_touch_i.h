
#ifndef __APP_TOUCH_I_H_
#define __APP_TOUCH_I_H_


//外部头文件
#include "beetles_app.h"




enum
{
	button_windows = APP_TEST_ID + 1,
	
	dialog_windows,
	
	loading_windows,
	
	digit_windows,
	
	logic_windows,
};


enum dialog_type
{
	DIALOG_YES_NO = 0,

	DIALOG_OK,
};


//内部头文件
#include "in_libs\command.h"
#include "in_libs\msg.h"
#include "in_libs\string.h"
#include "in_libs\ctrl\lib_button.h"
#include "in_libs\ctrl\lib_slider.h"
#include "in_libs\ctrl\lib_statics.h"
#include "in_libs\ctrl\lib_listbar.h"



#endif



