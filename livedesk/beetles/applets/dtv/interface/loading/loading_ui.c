
#include "loading_ui.h"



RECT loading_rect = {400,200,100,80};



statics_ui_t loading_ui = 
{
	{0,0,100,50},
	{
		ID_WIDGET_BATTERY0_BMP,
		ID_WIDGET_BATTERY1_BMP,
		ID_WIDGET_BATTERY2_BMP,
		ID_WIDGET_BATTERY3_BMP,
		ID_WIDGET_BATTERY4_BMP,
		ID_WIDGET_BATTERY5_BMP,
	},	
	6,	
	{0},	
	0,

};



statics_ui_t loading_text_ui = 
{
	{0,50,80,30},
	{0},	
	0,	
	{
		STRING_WIDGET_WAITING
	},	
	1,

};



