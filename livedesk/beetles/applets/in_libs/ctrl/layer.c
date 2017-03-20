
#include "layer.h"




/**********************************************************************************************************************
* 图层创建接口
**********************************************************************************************************************/
H_WIN Layer_Create(RECT *rect)
{
	FB	fb = {
    	{0, 0},                                   		
    	{0, 0, 0},
    	{FB_TYPE_RGB, {PIXEL_COLOR_ARGB8888, 0, 0}}, 
	};
	__disp_layer_para_t para = 
	{
		DISP_LAYER_WORK_MODE_NORMAL,
    	0, 
    	0, 
    	0, 
    	
    	0, 
    	0xff,
    	{0, 0, 0, 0},
    	{0, 0, 0, 0},
    	DISP_LAYER_OUTPUT_CHN_DE_CH1,
    	NULL
	};
	
	__layerwincreate_para_t create_info = 
	{
		"linein layer",
		NULL,
		GUI_LYRWIN_STA_SUSPEND,
		GUI_LYRWIN_NORMAL
	};
		
	fb.size.width  = rect->width;
	fb.size.height = rect->height;

	para.pipe			= 1;
	
	para.scn_win.x 		= rect->x;
	para.scn_win.y 		= rect->y;
	para.scn_win.width 	= rect->width;	
	para.scn_win.height = rect->height;	

	para.src_win.x 		= 0;
	para.src_win.y 		= 0;
	para.src_win.width 	= rect->width;	
	para.src_win.height = rect->height;	

	para.fb = &fb;
	create_info.lyrpara = &para;
	
	return(GUI_LyrWinCreate(&create_info));
}

