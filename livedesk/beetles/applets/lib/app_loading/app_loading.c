/*
**************************************************************************************************************
*											         ePDK
*						            the Easy Portable/Player Develop Kits
*									           desktop system 
*
*						        	 (c) Copyright 2007-2010, ANDY, China
*											 All Rights Reserved
*
* File    	: msgbox.c
* By      	: Andy.zhang
* Func		: desk main thread
* Version	: v1.0
* ============================================================================================================
* 2009-7-20 8:51:52  andy.zhang  create this file, implements the fundemental interface;
**************************************************************************************************************
*/

#include "app_loading.h"
#include "app_loading_uipara.h"
#define LOADING_BMP_NUM	10

typedef struct tag_loading_ctrl
{
	app_loading_para_t *loading_para;
	HTHEME h_bmp_loading;
	__s32 focus_txt_color;
	char  loading_tips_text[128];
	__u8  loading_bmp_cnt;
	__u8  loading_task;
	__u8  loading_txt_init_flag;
}app_loading_ctrl_t;

static __s32 loading_bmp_id[LOADING_BMP_NUM] = 
{
	ID_LOADING_BUFF1_BMP,
	ID_LOADING_BUFF2_BMP,
	ID_LOADING_BUFF3_BMP,
	ID_LOADING_BUFF4_BMP,
	ID_LOADING_BUFF5_BMP,
	ID_LOADING_BUFF6_BMP,
	ID_LOADING_BUFF7_BMP,
	ID_LOADING_BUFF8_BMP,
	ID_LOADING_BUFF9_BMP,
	ID_LOADING_BUFF10_BMP,
};


/**********************************************************************************************************************
* 图层创建接口
**********************************************************************************************************************/
static H_WIN app_loading_layer_create( RECT *rect)
{
	FB	fb = {
    	{0, 0},                                   		
    	{0, 0, 0},                                      
    	{FB_TYPE_RGB, {PIXEL_MONO_8BPP, 0, 0}},    
	};
	__disp_layer_para_t para = 
	{
    	DISP_LAYER_WORK_MODE_PALETTE,                   
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
		"app_loading_layer",
		NULL,
		GUI_LYRWIN_STA_SUSPEND,
		0
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


static __s32 create_flag = 0;


static __s32 loading_paint(H_WIN h_win)
{
	app_loading_ctrl_t *loading_ctrl;
	GUI_RECT gui_rect;
	app_loading_uipara_t *app_loading_uipara;

	if (GUI_LyrWinGetSta(GUI_WinGetLyrWin(h_win)) == GUI_LYRWIN_STA_SUSPEND)
		return EPDK_FAIL;

    if(1 != create_flag)
    {
        __msg("---------------Err, frm not created------\n");
    }
	
	loading_ctrl = (app_loading_ctrl_t *)GUI_WinGetAddData(h_win);
	app_loading_uipara = (app_loading_uipara_t *)get_app_loading_uipara();

	GUI_LyrWinSel(loading_ctrl->loading_para->loading_layer);
	GUI_SetFont(loading_ctrl->loading_para->font);
	GUI_UC_SetEncodeUTF8();

	if(loading_ctrl->h_bmp_loading)
	{
		dsk_theme_close(loading_ctrl->h_bmp_loading);
		if(loading_ctrl->loading_bmp_cnt >= LOADING_BMP_NUM)
			loading_ctrl->loading_bmp_cnt = 0;
		loading_ctrl->h_bmp_loading = dsk_theme_open(loading_bmp_id[loading_ctrl->loading_bmp_cnt]);
		loading_ctrl->loading_bmp_cnt++;
	}
	else
	{
		loading_ctrl->h_bmp_loading = dsk_theme_open(loading_bmp_id[loading_ctrl->loading_bmp_cnt]);
	}
	GUI_BMP_Draw(theme_hdl2buf(loading_ctrl->h_bmp_loading), app_loading_uipara->loading_bmp_pos.x,app_loading_uipara->loading_bmp_pos.y);
	

	if(loading_ctrl->loading_txt_init_flag==0)
	{
		GUI_SetColor(APP_COLOR_YELLOW);
		get_menu_text(STRING_LOADING,loading_ctrl->loading_tips_text,128);
		gui_rect.x0 = app_loading_uipara->loading_txt_pos.x;
		gui_rect.y0 = app_loading_uipara->loading_txt_pos.y;
		gui_rect.x1 = gui_rect.x0 + app_loading_uipara->loading_txt_pos.w;
		gui_rect.y1 = gui_rect.y0 + app_loading_uipara->loading_txt_pos.h;
		GUI_DispStringInRectWrap(loading_ctrl->loading_tips_text, &gui_rect, GUI_TA_VCENTER | GUI_TA_HCENTER, GUI_WRAPMODE_WORD);
		loading_ctrl->loading_txt_init_flag = 1;
	}

	return EPDK_OK;
}

static void __show_task(void *p_arg)
{
	H_WIN		hwin;
	hwin = (H_WIN)p_arg;

	while(1)
	{
		if(esKRNL_TDelReq(EXEC_prioself) == OS_TASK_DEL_REQ)
		{
   			goto EXIT_DEC_TASK;
		}
		loading_paint(hwin);
		esKRNL_TimeDly(10); 
	}
	
EXIT_DEC_TASK:	
	esKRNL_TDel(EXEC_prioself);
	
	return;	
}

static __s32 _app_loading_proc(__gui_msg_t *msg)
{
	switch(msg->id)
	{
	case GUI_MSG_CREATE:
		{
			app_loading_ctrl_t *loading_ctrl;
			app_loading_para_t *loading_para;
			
			loading_ctrl = (app_loading_ctrl_t *)My_Balloc(sizeof(app_loading_ctrl_t));
			if(!loading_ctrl)
			{
				__msg("memory balloc fail.........\n");
				return EPDK_FAIL;
			}
			eLIBs_memset(loading_ctrl, 0, sizeof(app_loading_ctrl_t));
    
			loading_para = (app_loading_para_t *)GUI_WinGetAttr(msg->h_deswin);

			loading_ctrl->loading_para = loading_para;
			GUI_WinSetAddData(msg->h_deswin, (__u32)loading_ctrl);

            create_flag = 1;
                            ////12345
			com_set_palette_by_id(ID_LOADING_WKM_BMP);/// modified by Bayden, clear bk color, 14.02.21
			__here__;

                           
			GUI_LyrWinCacheOn();
            __here__;
			GUI_LyrWinSetSta(loading_ctrl->loading_para->loading_layer, GUI_LYRWIN_STA_ON);

            __here__;
                    /// modified by Bayden, clear bk color, 14.02.21
			GUI_LyrWinSel(loading_ctrl->loading_para->loading_layer);
			
			GUI_SetBkColor(0x00);
            
			GUI_Clear();
            __here__;
			
			GUI_LyrWinSetTop(loading_ctrl->loading_para->loading_layer);
			GUI_LyrWinCacheOff();
            __here__;

			dsk_display_set_backcolor(0x00);
            __here__;

			loading_ctrl->loading_task = esKRNL_TCreate(__show_task,(void*)(msg->h_deswin),0x1000, KRNL_priolevel3);
            __here__;
            
		}	
		return EPDK_OK;
	case GUI_MSG_CLOSE:
		return EPDK_OK;
	case GUI_MSG_DESTROY:
		{
			app_loading_ctrl_t *loading_ctrl;
			loading_ctrl = (app_loading_ctrl_t *)GUI_WinGetAddData(msg->h_deswin);
			if(!loading_ctrl)
			{
				__err("loading_ctrl is null.......\n");
				return EPDK_FAIL;
			}
			if(loading_ctrl->h_bmp_loading)
			{
				dsk_theme_close(loading_ctrl->h_bmp_loading);
				loading_ctrl->h_bmp_loading = 0;
			}

			if(loading_ctrl->loading_task != 0)
			{
				while( esKRNL_TDelReq(loading_ctrl->loading_task) != OS_TASK_NOT_EXIST )
					esKRNL_TimeDly(5);
				loading_ctrl->loading_task = 0;
			}
			
			if(loading_ctrl->loading_para)
			{
				My_Bfree(loading_ctrl->loading_para, sizeof(app_loading_para_t));
			}
			
			if(loading_ctrl)
			{
				My_Bfree(loading_ctrl, sizeof(app_loading_ctrl_t));
			}

            create_flag--;
            if(0 != create_flag)
            {
                __msg("-------------------------------------destroy err. create_flag:%d\n", create_flag);
            }
		}
		return EPDK_OK;
	case GUI_MSG_PAINT:
		return EPDK_OK;
	case GUI_MSG_KEY:
		return EPDK_OK;
	case GUI_MSG_TOUCH:
		break;
	case GUI_MSG_TIMER:
		return EPDK_OK;
	default:
		break;
	}
	
	return GUI_FrmWinDefaultProc(msg);	
}

/*
	建立窗口
*/
H_WIN app_loading_frm_win_create(H_WIN h_parent, app_loading_para_t *para)
{
	__gui_framewincreate_para_t framewin_para;
	app_loading_para_t *loading_para;
	FB fb;

	GUI_LyrWinGetFB(para->loading_layer, &fb);
	loading_para = (app_loading_para_t *)My_Balloc(sizeof(app_loading_para_t));
	if(!loading_para)
	{
		__msg("memory balloc fail.........\n");
		return NULL;
	}
	loading_para->font= para->font;
	loading_para->loading_layer = para->loading_layer;
	
	eLIBs_memset(&framewin_para, 0, sizeof(__gui_framewincreate_para_t));

	framewin_para.name =	"app_loading_win";
	framewin_para.dwExStyle = WS_EX_NONE;
	framewin_para.dwStyle = WS_NONE|WS_VISIBLE;
	framewin_para.spCaption =  NULL;
	framewin_para.hOwner	= NULL;
	framewin_para.hHosting = h_parent;
	framewin_para.id         = loading_para->id ;
	framewin_para.FrameWinProc = (__pGUI_WIN_CB)esKRNL_GetCallBack((__pCBK_t)_app_loading_proc);
	framewin_para.rect.x = 0;
	framewin_para.rect.y = 0;
	framewin_para.rect.width =  fb.size.width;
	framewin_para.rect.height =  fb.size.height;
	framewin_para.BkColor.alpha =  0;
	framewin_para.BkColor.red = 0;
	framewin_para.BkColor.green = 0;
	framewin_para.BkColor.blue = 0;
	framewin_para.attr =  (void *)loading_para;
	framewin_para.hLayer = para->loading_layer;

	return (GUI_FrmWinCreate(&framewin_para));
}

void  app_loading_win_delete(H_WIN loading_win  )
{
	H_LYR h_layer;
	h_layer = GUI_WinGetLyrWin(loading_win);
	if(h_layer != NULL)
	{
		GUI_LyrWinDelete(h_layer);
	}
}
H_WIN app_loading_win_create(H_WIN hparent , __u16 id )
{

	app_loading_para_t	 app_loading_para;
	RECT rect;
	H_WIN h_wnd;
	rect.x = 0;
	rect.y = 0;
	dsk_display_get_size(&rect.width, &rect.height);

	app_loading_para.loading_layer = app_loading_layer_create(&rect);
	app_loading_para.font = SWFFont;
	app_loading_para.id = id ;
	h_wnd = app_loading_frm_win_create(hparent,&app_loading_para);
	if(h_wnd)
	{
		__msg("app_loading_frm_win_create succeed\n");
		return h_wnd;
	}
	else
	{
		__msg("app_loading_frm_win_create fail\n");
		return NULL;
	}

}
