/*
**************************************************************************************************************************
*													        eMOD
*						            		the Easy Portable/Player Develop Kits
*									   				melix includes module 
*
*                             		 Copyright(C), 2011-2012, Softwinner Microelectronic Co., Ltd.
*									               All Rights Reserved
*
* File name		:	listview.h
* Author		: 	Derek
* Version 		: 	V1.00
* Date    		: 	2012-03-14
* Description	:
* Others 		:
* History 		:
* <Author>           <time>          <version>      <description>
* Derek Lai		    2012-03-14         1.0.0          Create File
*
**************************************************************************************************************************
*/
#ifndef __LISTVIEW_H__
#define __LISTVIEW_H__

typedef     void*   HLIST; 
typedef  __hdle  H_ITEM;

typedef enum GUI_LIST_MSG
{
	LIST_ITEM_ENTER = GUI_MSG_ + 100,			//进入下一级目录
	LIST_MOVE_STOP,					//滑动停止
}gui_list_msg_e;

/*typedef enum GUI_LIST_STATE
{
	LIST_STATE_MOVING,
	LIST_STATE_STOP
}gui_list_state_e;*/

typedef enum GUI_LIST_DRAM_MODE
{
    LIST_MODE_NORMAL    = 0,                    // 普通条目绘制
    LIST_MODE_FOCUS,                            // 焦点条目绘制
    LIST_MODE_INVALID,                          // 无效条目绘制
    LIST_MODE_VACANT,                           // 空白条目绘制
    LIST_MODE_DIFFUSE,                          // 扩散条目绘制
}gui_list_draw_mode_e;


typedef struct ITEM_DRAW_PARA
{
    __s32	                        item;
    gui_list_draw_mode_e            mode;
    //__u32                           aux;
    RECT                            invalidrect;	//绘制区域的坐标是以list_buffer起始点为起始坐标的相对坐标       
}item_draw_para_t;

/*typedef struct GUI_LIST_DRAW_PARA
{
    gui_list_draw_mode_e  	mode;                   // 绘制的模式
    __s32               	index;                  // 绘制条目的编号
    RECT                	rect;                   // 绘制条目的区域
    void*               	attr;                   // 应用的传入的私有数据
}gui_list_draw_para_t;*/

typedef struct GUI_LIST_SCENE
{
    __s32                   start_id;           // 页面的起始位置条目    
    __s32                   focus_id;           // 页面的焦点条目
}gui_list_scene_t;

typedef struct GUI_SCROLL_BAR
{
    RECT                    show_rect;          // 显示区域
    RECT                    scroll_rect;        // SCROLL区域
    __u32                   head_height;        // HEAD图标高度
    __u32                   tail_height;        // TAIL图标高度
        
    void*                   back_bmp;           // BACK图标
    void*                   head_bmp;           // HEAD图标
    void*                   body_bmp;           // BODY图标(高为一个像素)
    void*                   tail_bmp;           // TAIL图标
}gui_scroll_bar_t;

/*
**********************************************************************************************************************
*                                               
*
* Description: 每个条目的绘制回调
*
* Arguments  : para: 绘制的参数
*
*
**********************************************************************************************************************
*/
typedef __s32 (* p_DrawItem)(item_draw_para_t * para);


typedef enum GUI_LIST_MODE
{
	GUI_LIST_ITEM_MODE = 0,				//item模式
	GUI_LIST_SQUARE_MODE,				//九宫格模式
}gui_list_mode_e;

typedef struct GUI_LIST_CONFIG
{
    RECT                    list_rect;          // listview所在窗口区域
    __s32                   item_width;         // list条目的宽
    __s32                   item_height;        // list条目的高

	gui_list_mode_e			list_mode;			// list模式
	
    __s32                   start_id;           // 页面的起始位置条目
    __s32                   focus_id;           // 页面的焦点条目
    __u32                   bk_color;           // 背景色
    //__s32                   alpha_status;       // Alpha开关
    
    __u32                   item_cnt;           // item 的条目总数
    void*                   list_attr;          // list用户数据
	p_DrawItem				item_cb;			// item回调
}gui_list_para_t;

typedef gui_list_para_t gui_list_config_t;

//set_item_cb(__s32 item, void cb, void parg);

#endif	/*ifndef __LISTVIEW_H__*/

