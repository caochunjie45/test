
#include "search_ui.h"


RECT search_rect  = {0,0,800,480};


statics_ui_t search_text_ui = 
{
	{300,200,200,30},
	{0},	
	0,	
	{
		STRING_DTV_SEARCH_NOW
	},	
	1,

};

statics_ui_t valid_frequency_text_ui = 
{
	{300,230,200,30},
	{0},	
	0,	
	{
		STRING_DTV_SEARCH_VALID_CH
	},	
	1,

};


progbars_ui_t search_proc_ui =
{
	{200,280,300,50},
	{
		ID_WIDGET_100_SLIDER_HEAD_BMP,
		ID_WIDGET_100_SLIDER_TAIL_BMP,
		ID_WIDGET_100_SLIDER_CURSOR_BMP,
	},
};

