
#include "ui.h"



RECT button_rect  = {0,0,800,480};


button_ui_t menu_ui = 
{
	{0,0,80,30},
	{
		ID_WIDGET_10_BUTTON_FOCUS_BMP,
		ID_WIDGET_10_BUTTON_UNFOCUS_BMP
	},
};


button_ui_t edit_ui = 
{
	{80,0,80,30},
	{
		ID_WIDGET_11_BUTTON_FOCUS_BMP,
		ID_WIDGET_11_BUTTON_UNFOUCS_BMP
	},	
};


button_ui_t help_ui = 
{
	{160,0,80,30},
	{
		ID_WIDGET_12_BUTTON_FOCUS_BMP,
		ID_WIDGET_12_BUTTON_UNFOCUS_BMP
	},
};


button_ui_t project_ui = 
{
	{240,0,80,30},
	{	
		ID_WIDGET_13_BUTTON_FOCUS_BMP,
		ID_WIDGET_13_BUTTON_UNFOCUS_BMP
	},
};




slider_ui_t volume_ui =
{
	{200,250,346,50},
	{
		ID_WIDGET_100_SLIDER_HEAD_BMP,
		ID_WIDGET_100_SLIDER_TAIL_BMP,
		ID_WIDGET_100_SLIDER_CURSOR_BMP,
	},
};



listbar_ui_t setting_ui = 
{
	{0,32,130,150},
	{
		ID_WIDGET_200_LISTBAR_FOCUS_BMP,
		ID_WIDGET_200_LISTBAR_UNFOCUS_BMP,		
	},
	
	30,
	130,
	5,
	{
		STRING_MAINMENU_MOVIE,
		STRING_MAINMENU_PHOTO,
		STRING_MAINMENU_MUSIC,
		STRING_MAINMENU_SETTING,
		STRING_MAINMENU_EXPLORER,				
	},

	GUI_RED,
	GUI_BLACK,
};



statics_ui_t voltage_ui = 
{
	{600,2,100,50},
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



