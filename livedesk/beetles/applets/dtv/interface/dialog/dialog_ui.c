
#include "dialog_ui.h"



RECT dialog_rect  = {300,200,195,105};


statics_ui_t bg_ui = 
{
	{0,0,195,105},
	{
		ID_WIDGET_DIALOG_BMP,
	},	
	1,
	
	{0},	
	0,

};

statics_ui_t icon_ui = 
{
	{10,5,30,28},
	{
		ID_WIDGET_DIALOG_ICON_BMP,
	},	
	1,
	
	{0},	
	0,

};


button_ui_t yes_ui = 
{
	{20,70,45,25},
	{
		ID_WIDGET_DIALOG_BUTTON_FOCUS_BMP,
		ID_WIDGET_DIALOG_BUTTON_UNFOCUS_BMP
	},
	STRING_WIDGET_YES,
};


button_ui_t no_ui = 
{
	{130,70,45,25},
	{
		ID_WIDGET_DIALOG_BUTTON_FOCUS_BMP,
		ID_WIDGET_DIALOG_BUTTON_UNFOCUS_BMP
	},
	STRING_WIDGET_NO,
};


button_ui_t ok_ui = 
{
	{110,70,45,25},
	{
		ID_WIDGET_DIALOG_BUTTON_FOCUS_BMP,
		ID_WIDGET_DIALOG_BUTTON_UNFOCUS_BMP
	},
	STRING_WIDGET_OK,
};





