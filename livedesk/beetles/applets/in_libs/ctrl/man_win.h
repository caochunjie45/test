

#ifndef __MAN_WIN_H_
#define __MAN_WIN_H_



#include "beetles_app.h"

#include "dlist.h"

#include "mem.h"

#include "lib_win.h"

#include "string.h"




extern dlist_t *Create_Windows_List(void);



enum
{
	WINDOWS_SLEEP = 0,
	WINDOWS_ON    = 1,
};


enum
{
	TYPE_NULL = 0,
	TYPE_BUTTON,
	TYPE_SLIDER,
	TYPE_STATICS,
	TYPE_LISTBAR,
	TYPE_PROGBARS,
};



typedef struct
{
	__u32                       focus_bmp;

	__u32                       unfocus_bmp;
	
}bmp_t;


typedef struct
{
	RECT                        button_location;

	bmp_t	                    bmp_button;

	__u32                       string_ID;
	
}button_ui_t;


typedef struct
{
	__u32                       head_bmp;

	__u32                       tail_bmp;

	__u32                       cursor_bmp;
	
}bmp_slider_t;


typedef struct
{
	RECT                        slider_location;

	bmp_slider_t	            bmp_slider;
	
}slider_ui_t;


typedef struct
{
	__u32                       head_bmp;

	__u32                       tail_bmp;

	__u32                       frame_bmp;
	
}bmp_progbars_t;


typedef struct
{
	RECT                        progbars_location;

	bmp_progbars_t	            bmp_progbars;
	
}progbars_ui_t;






typedef struct
{
	RECT                        statics_location;

	__u32          	            bmps[10];

	__u8                        bmp_total;

	__u32                       string_ID[5];
	
	__u8                        string_total;
	
}statics_ui_t;



typedef struct
{
	__u32                       focus_bmp;

	__u32                       unfocus_bmp;
	
}bmp_listbar_t;



typedef struct
{
	RECT                        listbar_location;

	bmp_listbar_t	            bmp_listbar;

	__u8                        item_height;

	__u8                        item_width;

	__u8                        total;

	__u32                       string_ID[10];
	
	__s32	                    focus_color;
	
	__s32	                    unfocus_color;
		
}listbar_ui_t;



typedef union mix_ctrl_s        mix_ctrl_t;

typedef struct Mixed_CtrlWin_S  Mixed_CtrlWin_T;


typedef struct button_info
{
	button_attribute_t			button_attribute;
	
	RECT                        rect;
	
	HTHEME     	                focus_bmp;
	
	HTHEME     	                unfocus_bmp;

	char                        *strings;
	
}button_info_t;


typedef struct button
{
	button_info_t				button_info;
	
	__gui_ctlwincreate_para_t	widget_info;
	
    H_WIN                       ctrlwin;
    
	Mixed_CtrlWin_T             *pFirstHosted;
	
	Mixed_CtrlWin_T             *FocusHosted;

    
}button_t;


typedef struct slider_info
{
	slider_attribute_t			slider_attribute;
	
	RECT                        rect;
	
	HTHEME     	                head_bmp;
	
	HTHEME     	                tail_bmp;
	
	HTHEME     	                cursor_bmp;
	
}slider_info_t;


typedef struct slider
{
	slider_info_t				slider_info;
	
	__gui_ctlwincreate_para_t	widget_info; 
	
    H_WIN                       ctrlwin;
    
	Mixed_CtrlWin_T             *pFirstHosted;
    
 	Mixed_CtrlWin_T             *FocusHosted;

    __u8                        slider_value;


}slider_t;


typedef struct progbars_info
{
	progbars_attribute_t		progbars_attribute;
	
	RECT                        rect;
	
	HTHEME     	                head_bmp;
	
	HTHEME     	                tail_bmp;
	
	HTHEME     	                frame_bmp;
	
}progbars_info_t;


typedef struct progbars
{
	progbars_info_t				progbars_info;
	
	__gui_ctlwincreate_para_t	widget_info; 
	
    H_WIN                       ctrlwin;
    
	Mixed_CtrlWin_T             *pFirstHosted;
    
 	Mixed_CtrlWin_T             *FocusHosted;

    __u8                        progbars_value;


}progbars_t;






typedef struct statics_info
{

	statics_attribute_t			statics_attribute;
	
	HTHEME	                    *bmp_data;

	__u8                        bmp_totals;

	char                        (*strings)[STRING_MAX];

	__u8                        string_totals;

	
}statics_info_t;


typedef struct statics
{
	statics_info_t				statics_info;
	
	__gui_ctlwincreate_para_t	widget_info;
	
    H_WIN                       ctrlwin;

	Mixed_CtrlWin_T             *pFirstHosted;

    
}statics_t;



typedef struct
{
	HTHEME		                focus_bmp;
	
	HTHEME                      unfocus_bmp;
	
	__s32	                    focus_txt_color;
	
	__s32	                    unfocus_txt_color;
	
	__u32                       *text;
	
	__u32                       listbar_ID;

	__u32                       Index;
	
	H_WIN                       parent;
	
}listbar_draw_t;



typedef struct listbar_ctrlwin_info
{
	statics_attribute_t			listbar_attribute;//listbar 没有控件，借用的是statics的控件
	
}listbar_info_t;


typedef struct listbar
{
	listbar_info_t		        listbar_info;
	
	__gui_ctlwincreate_para_t	widget_info;
	
    H_WIN                       ctrlwin;

	
	listbar_draw_t              listbar_draw;
	
	__listbar_config_t			listbar_config;

	H_LBAR	                    h_listbar;

	Mixed_CtrlWin_T             *pFirstHosted;

    
}listbar_t;



typedef union mix_ctrl_s
{
	button_t                    Button;
	
	slider_t                    Slider;
	
	statics_t                   Statics;
	
	listbar_t                   Listbar;

	progbars_t	                Progbars;
	
}mix_ctrl_t;


typedef struct Mixed_CtrlWin_S
{
	mix_ctrl_t                  mix_ctrl;

    H_WIN                       ctrlwin;
	
	__u8                        type_ctrl;

	__u32                       ID;
	
	__pCBK_t                    destory;
	
	Mixed_CtrlWin_T             *pNextHosted;
	
}Mixed_CtrlWin_T;


typedef struct
{
	__u32                       id;
	
	char                        name[32];
	
	H_WIN                       framewin;
	
	H_WIN                       parent;
	
	H_LYR                       layer;
	
	RECT                        ui_rect;


	
	__pCBK_t   		            show;
	
	__pCBK_t                    hide;
	
	__pCBK_t                    update;




	dlist_t                     *button_head;
	
	__u8                        button_focus_id;
	
	button_t                    *focus_button;


	
	dlist_t                     *slider_head;
	
	__u8                        slider_focus_id;
	
	slider_t                    *focus_slider;




	dlist_t                     *statics_head;
	
	__u8                        statics_focus_id;
	
	statics_t                   *focus_statics;



	dlist_t                     *listbar_head;
	
	__u8                        listbar_focus_id;
	
	listbar_t                   *focus_listbar;


	dlist_t                     *progbars_head;
	
	__u8                        progbars_focus_id;
	
	progbars_t                  *focus_progbars;



	__u8                        window_state;



	__s32                       time_ID;

	__s32                       time_count;

	
}windows_t;






typedef struct
{
	__u32                       id;
	
	char                        name[16];
	
	H_WIN                       logic_win;
	
	H_WIN                       parent;

	void	                    *pulic_data;
	
	__krnl_event_t              *sem;

	__pCBK_t   		            destory;
	
	
}Logic_Win;



typedef struct
{
	__u32                       id;
	
	char                        name[32];
	
	H_WIN                       man_win;
	
	H_WIN                       parent;

	
	

	dlist_t                     *windows_head;
	
	__u8                        windows_focus_id;
	
	windows_t                   *focus_windows;

	Logic_Win                   *Logic;               

	
}Man_Win;




int Delete_ManWin(Man_Win *ManWin);



int Register_ManWin(__u32 ID,Man_Win *ManWin,H_WIN parent,__pGUI_WIN_CB Proc);



#endif


