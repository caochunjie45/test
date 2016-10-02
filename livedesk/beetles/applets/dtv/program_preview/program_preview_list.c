/*
**************************************************************************************************************
*											         ePDK
*						            the Easy Portable/Player Develop Kits
*									           desktop system 
*
*						        	 (c) Copyright 2007-2010, ANDY, China
*											 All Rights Reserved
*
* File    	: program_preview_frmgeneral.c
* By      	: Hot.lee
* Func		: desk main thread
* Version	: v1.0
* ============================================================================================================
* 2013-3-30 8:51:52  Hot.lee  create this file, implements the fundemental interface;
**************************************************************************************************************
*/


#include "program_preview_uipara.h"

#include "program_preview_list.h"

extern H_WIN g_dtv_mainwin;

#define DISPLAY_OFFSET 30

typedef struct tag_program_preview_frmfocus_item_id
{
	__s32  id_bmp_focus_item;
	
	__s32  id_bmp_unfocus_item;
	
}program_preview_frmfocus_item_id_t;

typedef struct tag_program_preview_frmattr
{
	HTHEME     	h_title_bmp;
	HTHEME		h_item_focus_bmp,h_item_unfocus_bmp;

	__s32	focus_txt_color;
	__s32	unfocus_txt_color;

	H_LBAR	 h_listbar;
	
	H_LYR	layer;
	
	GUI_FONT *font;

	__s32 new_focus;
	
	__s32 old_focus;

	program_preview_frmpara_t *para;

	char curprogramname[1024];

	__s32 total;

	__hdle	long_str_handle;
	
 }program_preview_frmattr_t;

extern void  main_cmd2parent(H_WIN hwin, __s32 id, __u32 data1, __u32 data2);


static __s32 program_preview_frmwin_item_paint(__lbar_draw_para_t *draw_param);

static void _program_preview_frmres_init(program_preview_frmattr_t *attr);

static void _program_preview_frmres_uninit(program_preview_frmattr_t *attr) ;


static void __draw_entpy_listbar(program_preview_frmattr_t *gattr);



 static __s32 list_long_string_init(program_preview_frmattr_t *gattr)
 {
	 if(gattr->long_str_handle == NULL)
	 {
		__show_info_move_t show_info;

		eLIBs_memset(&show_info, 0, sizeof(__show_info_move_t));
		show_info.hlayer = gattr->layer;
		show_info.alpha_en = EPDK_TRUE;
		show_info.pFont = NULL;
		show_info.string = "";
		show_info.fontColor = gattr->focus_txt_color;
		show_info.align = GUI_TA_LEFT | GUI_TA_VCENTER;

		gattr->long_str_handle = GUI_LongStringCreate(&show_info);
		if(gattr->long_str_handle == NULL)
		{
			eDbug("create long string failed!\n");
		}
	}
	
	else
	{
		eDbug("long string scroll has already started!\n");
	} 
 	
     return EPDK_OK;
 }
 
 
 
 
static  __s32 list_long_string_uninit(program_preview_frmattr_t *gattr)
 {
 	if(gattr->long_str_handle != NULL)
 	{
 		GUI_LongStringDelete(gattr->long_str_handle);
 		
 		gattr->long_str_handle = NULL;
 	}
 	
 	return EPDK_OK;
 }
 
 
 
 
 static __s32 list_long_string_start_roll_fast(__lbar_draw_para_t *draw_param, char *string)	
 {
 	__show_info_move_t show_info;
 	
 	__epdk_charset_enm_e encode = EPDK_CHARSET_ENM_UTF8;
 	
 	RECT roll_rect;
 	
 	RECT text_rect;
 	
     program_preview_frmattr_t *gattr = NULL;
     
     __u32 font_size = 0;
     
 
 	gattr = (program_preview_frmattr_t *)draw_param->attr;
 	
 	
 	if(gattr->long_str_handle != NULL)
 	{
 		eLIBs_memset(&show_info, 0, sizeof(__show_info_move_t));
 		
     	roll_rect.x      = draw_param->rect.x;
 		roll_rect.y      = draw_param->rect.y;
 		roll_rect.width  = draw_param->rect.width;
 		roll_rect.height = draw_param->rect.height;
 
 		font_size = GUI_GetFontDistY();
 
 		
 	    show_info.hlayer        = gattr->layer;
 	    show_info.region.x      = roll_rect.x;
 	    show_info.region.y      = roll_rect.y;
 	    show_info.region.width  = roll_rect.width;
 	    show_info.region.height = roll_rect.height;
 	    show_info.alpha_en      = EPDK_FALSE;
 	    show_info.pFont         = gattr->font;
 	    show_info.string        = string;
 	    show_info.encode_id     = encode;		
 	    
 	    show_info.fontColor = gattr->focus_txt_color;
 	    show_info.align = GUI_TA_LEFT | GUI_TA_VCENTER;
 
		 show_info.bmp = theme_hdl2buf(gattr->h_item_focus_bmp);
		 show_info.bmp_pos.x = 0;
		 show_info.bmp_pos.y = 0;
 
 	    GUI_LongStringStop(gattr->long_str_handle);
 	    GUI_LongStringReset(gattr->long_str_handle,&show_info);
 	    GUI_LongStringStart(gattr->long_str_handle);	
 		
 	}
 	return EPDK_OK;
 }
 
 
 
 static __s32 list_long_string_stop_roll(program_preview_frmattr_t *gattr)
 {
 	if(gattr->long_str_handle!= NULL)
	{    
		return GUI_LongStringStop(gattr->long_str_handle);			
	}
	
 	return EPDK_FAIL;
 }
 


static void program_preview_update_topview(program_preview_frmattr_t *gattr)
{
	char tmp[128];
	GUI_RECT gui_rect;
	
	
	program_preview_uipara_t *uipara = NULL; 

	uipara = program_preview_get_uipara();

	GUI_SetFont(gattr->font);
	GUI_UC_SetEncodeUTF8();
	GUI_SetDrawMode(GUI_DRAWMODE_NORMAL);

    //顶部更新
	GUI_BMP_Draw(theme_hdl2buf(gattr->h_title_bmp),uipara->title.x,uipara->title.y);
	
	gui_rect.x0 = uipara->title.x;
	gui_rect.y0 = uipara->title.y;
	gui_rect.x1 = gui_rect.x0+uipara->title.width;
	gui_rect.y1 = gui_rect.y0+uipara->title.height;

	dsk_langres_get_menu_text(STRING_DTV_PROGRAM_PREVIEW,tmp,sizeof(tmp));
	GUI_SetColor(GUI_BLACK);
	GUI_DispStringInRect(tmp, &gui_rect, GUI_TA_HCENTER|GUI_TA_VCENTER); 

	//此处还有节目数
	program_preview_update_bottom_view(gattr);

}

static __s32 program_preview_update_bottom_view(program_preview_frmattr_t *gattr)
{
	char *cur, *next;
	GUI_RECT gui_rect;
	program_preview_uipara_t *uipara = NULL; 

	uipara = program_preview_get_uipara();

	GUI_SetFont(gattr->font);
	GUI_UC_SetEncodeUTF8();
	GUI_SetDrawMode(GUI_DRAWMODE_NORMAL);

    maple_sepg_search();
    
	//底部电视节目
	cur = maple_get_channel_of_curprogramname();
	next = maple_get_channel_of_nextprogramname();

    __msg("gattr->curprogramname = %s, cur = %s\n",gattr->curprogramname,cur);
	if(gattr->curprogramname[0] && cur[0] && eLIBs_strcmp(gattr->curprogramname,cur) == 0)
	{
		return EPDK_OK;
	}
    
	eLIBs_strcpy(gattr->curprogramname,cur);

	GUI_SetColor(GUI_DARKGREEN);	
	gui_rect.x0 = uipara->cur_next_prog[0].x;
	gui_rect.y0 = uipara->cur_next_prog[0].y;
	gui_rect.x1 = gui_rect.x0+uipara->cur_next_prog[0].width;
	gui_rect.y1 = gui_rect.y0+uipara->cur_next_prog[0].height;
	GUI_FillRect(gui_rect.x0,gui_rect.y0,gui_rect.x1,gui_rect.y1);

	GUI_SetColor(GUI_DARKGREEN);	
	gui_rect.x0 = uipara->cur_next_prog[1].x;
	gui_rect.y0 = uipara->cur_next_prog[1].y;
	gui_rect.x1 = gui_rect.x0+uipara->cur_next_prog[1].width;
	gui_rect.y1 = gui_rect.y0+uipara->cur_next_prog[1].height;
	GUI_FillRect(gui_rect.x0,gui_rect.y0,gui_rect.x1,gui_rect.y1);

	GUI_SetColor(GUI_BLACK); 
	gui_rect.x0 = uipara->cur_next_prog[0].x + DISPLAY_OFFSET;
	gui_rect.y0 = uipara->cur_next_prog[0].y;
	gui_rect.x1 = gui_rect.x0+uipara->cur_next_prog[0].width;
	gui_rect.y1 = gui_rect.y0+uipara->cur_next_prog[0].height;
	
	GUI_DispStringInRect(cur, &gui_rect, GUI_TA_LEFT|GUI_TA_VCENTER);  

	gui_rect.x0 = uipara->cur_next_prog[1].x + DISPLAY_OFFSET;
	gui_rect.y0 = uipara->cur_next_prog[1].y;
	gui_rect.x1 = gui_rect.x0+uipara->cur_next_prog[1].width;
	gui_rect.y1 = gui_rect.y0+uipara->cur_next_prog[1].height;	
	GUI_DispStringInRect(next, &gui_rect, GUI_TA_LEFT|GUI_TA_VCENTER); 

}

static __s32 program_preview_frmwin_item_paint(__lbar_draw_para_t *draw_param)
{
	__s32 i;
	
	RECT rect;
	
    GUI_RECT gui_rect;
  	
	program_preview_frmattr_t *gattr = NULL;
	
	maple_schdl_event_t *shdl_event = NULL;
	
	program_preview_uipara_t *uipara = NULL;
	
	char server_name[128] = {0};	
	
	char *tmp = NULL;
	
	
	__s32  nRemoteControlKeyId = 0;
	
    __bool lcn_state = 0;
    
	char shdl_name[MAPLE_MAX_NAME_LEN] = {0};
	
	
	gattr = (program_preview_frmattr_t *)draw_param->attr;
	
	if (gattr == NULL)
		return EPDK_FAIL;
		
	if (GUI_LyrWinGetSta(gattr->layer) != GUI_LYRWIN_STA_ON)
		return EPDK_FAIL;

	uipara = program_preview_get_uipara();
	
	GUI_SetFont(gattr->font);
	
	GUI_UC_SetEncodeUTF8();
	
	GUI_SetDrawMode(GUI_DRAWMODE_NORMAL);
	

    //获取节目名称
	tmp = maple_get_name_of_channel(draw_param->index);
	
    lcn_state = maple_get_lcn_state();
    
    if(lcn_state == EPDK_TRUE)
    {
        nRemoteControlKeyId = maple_get_cur_channel_lcn_id(draw_param->index);
        eLIBs_sprintf(server_name," %d %s", nRemoteControlKeyId, tmp);        
    }
    else
    {
        eLIBs_sprintf(server_name," %d %s",draw_param->index + 1,tmp);
    }
    
	switch(draw_param->mode)
	{
		case LBAR_MODE_NORMAL:
		{
			GUI_BMP_Draw(theme_hdl2buf(gattr->h_item_unfocus_bmp),draw_param->rect.x,draw_param->rect.y);

			GUI_SetColor(gattr->unfocus_txt_color);
			
 			gui_rect.x0 = draw_param->rect.x;
			gui_rect.y0 = draw_param->rect.y;
			gui_rect.x1 = gui_rect.x0 + uipara->listbar.width;
			gui_rect.y1 = gui_rect.y0 + uipara->listbar_height;
			
			GUI_DispStringInRect(server_name, &gui_rect, GUI_TA_LEFT |GUI_TA_VCENTER);    
  		}
		break;
		
		case LBAR_MODE_FOCUS:
		{
			GUI_BMP_Draw(theme_hdl2buf(gattr->h_item_focus_bmp),draw_param->rect.x,draw_param->rect.y);

			GUI_SetColor(gattr->focus_txt_color);

 			gui_rect.x0 = draw_param->rect.x;
			gui_rect.y0 = draw_param->rect.y;
			gui_rect.x1 = gui_rect.x0 + uipara->listbar.width;
			gui_rect.y1 = gui_rect.y0 + uipara->listbar_height;
			
			GUI_DispStringInRect(server_name, &gui_rect, GUI_TA_LEFT |GUI_TA_VCENTER);    
			
			list_long_string_start_roll_fast(draw_param,server_name);
			
		}
		break;
        
		default:
		{
		
		}
		break;
			
	}
    
	
	return EPDK_OK;
}



static void program_preview_frmlistbar_init(__gui_msg_t *msg)
{
	__listbar_config_t config; 
	RECT rect;
	program_preview_frmattr_t *attr;
	__s32 index;
	__s32 start_id,focus_id;
	program_preview_uipara_t *uipara = NULL; 
	
	
	attr = (program_preview_frmattr_t *)GUI_WinGetAddData(msg->h_deswin);
	
	if(NULL == attr)
	{
        return ;
	}
	
	uipara = program_preview_get_uipara();	

	
	index = maple_get_cur_program_index();
    
	if(index < (uipara->listbar.height/uipara->listbar_height))
	{
		start_id = 0;
		focus_id = index;
	}
	else
	{
		start_id = index -(uipara->listbar.height/uipara->listbar_height) + 1;
		focus_id = index;
	}

    if(focus_id >= attr->total)
    {
        focus_id = attr->total-1;
    }
    
    if(start_id >= attr->total)
    {
        start_id = attr->total-1;
    }
    if(focus_id < 0)
    {
        focus_id = 0;
    }

    if(start_id < 0)
    {
        start_id = 0;
    }

	config.alpha_status = EPDK_FALSE;
	config.bk_color = 0;
	config.start_id = start_id;
	config.focus_id = focus_id;			
	config.item_cnt = attr->total;
	config.item_height = uipara->listbar_height;
	config.item_width = uipara->listbar.width;
	config.list_rect.x = uipara->listbar.x;
	config.list_rect.y = uipara->listbar.y;

	if(attr->total >= (uipara->listbar.height/uipara->listbar_height))	/* LISTBAR_ITEM_NUM */
	{
		config.list_rect.width = uipara->listbar.width;
		config.list_rect.height = uipara->listbar.height;
	}
	else
	{
        config.list_rect.width = uipara->listbar.width;
		config.list_rect.height = uipara->listbar_height * attr->total;	
	}
	
	config.list_attr = (void*)attr;
	attr->h_listbar = LISTBAR_Create(msg->h_deswin); 
    
	if(NULL == attr->h_listbar)
	{
		__wrn("listbar create fail\n");
		return ;
	}
    
	LISTBAR_Config(attr->h_listbar, program_preview_frmwin_item_paint, &config);  
	
    LISTBAR_ShowPage(attr->h_listbar);     
	
}




/*
	申请资源
*/
static void _program_preview_frmres_init(program_preview_frmattr_t *attr)
{
	program_preview_uipara_t *uipara = NULL;
    uipara = program_preview_get_uipara();
    
	attr->h_item_focus_bmp = theme_open(uipara->list_focus_bmp);
	
	attr->h_item_unfocus_bmp= theme_open(uipara->list_unfocus_bmp);	
	
	attr->h_title_bmp = theme_open(uipara->top_bmp);
}

static void _program_preview_frmres_uninit(program_preview_frmattr_t *attr)
{
    __u32 	i; 
         
	theme_close(attr->h_title_bmp);
		attr->h_title_bmp=0;
		
	theme_close(attr->h_item_focus_bmp);	
		attr->h_item_focus_bmp = 0;
		
	theme_close(attr->h_item_unfocus_bmp);
		attr->h_item_unfocus_bmp = 0;
	
}

static __s32 _program_preview_frmlistbar_key_proc(__gui_msg_t *msg)
{    
    	__s32 ret = EPDK_OK;    
	program_preview_frmattr_t *attr;

	__wrn("_program_preview_frmlistbar_key_proc() : msg->dwAddData1 = %d, msg->dwAddData2 = %d\n", msg->dwAddData1, msg->dwAddData2);

	attr = (program_preview_frmattr_t *)GUI_WinGetAddData(msg->h_deswin);
    
    switch(msg->dwAddData1)
    {
       case GUI_MSG_KEY_UP:
       case GUI_MSG_KEY_LONGUP:
	   case GUI_MSG_KEY_LEFT:
	   case GUI_MSG_KEY_LONGLEFT:
       {
           if((msg->dwAddData2 == KEY_DOWN_ACTION) || (msg->dwAddData2 == KEY_REPEAT_ACTION))
           {
                __s32 index;					
				
				maple_sepg_clean();											
   			    attr->old_focus = attr->new_focus;
                LISTBAR_PrevItem(attr->h_listbar);
                index = LISTBAR_GetFocusItem(attr->h_listbar);
				
				attr->new_focus = index;
                
				{
					__gui_msg_t dmsg={0};

					dmsg.id = GUI_MSG_COMMAND;
					dmsg.h_deswin = g_dtv_mainwin;
					dmsg.dwAddData1 = CMD_CHANGE_CHANNEL;
					dmsg.dwAddData2 = index;
					dmsg.dwReserved = 0;

					GUI_SendNotifyMessage(&dmsg);
				}
				
				program_preview_update_topview(attr);

       	   }
           break;
       }
       case GUI_MSG_KEY_DOWN:
       case GUI_MSG_KEY_LONGDOWN:
	   case GUI_MSG_KEY_RIGHT:
	   case GUI_MSG_KEY_LONGRIGHT:
       {
           if((msg->dwAddData2 == KEY_DOWN_ACTION) || (msg->dwAddData2 == KEY_REPEAT_ACTION))
           {
               __s32 index;

			   maple_sepg_clean();	
				   attr->old_focus = attr->new_focus;
               LISTBAR_NextItem(attr->h_listbar);	
			   
			   index = LISTBAR_GetFocusItem(attr->h_listbar);
			   attr->new_focus = index;

				{
					__gui_msg_t dmsg={0};

					dmsg.id = GUI_MSG_COMMAND;
					dmsg.h_deswin = g_dtv_mainwin;
					dmsg.dwAddData1 = CMD_CHANGE_CHANNEL;
					dmsg.dwAddData2 = index;
					dmsg.dwReserved = 0;

					GUI_SendNotifyMessage(&dmsg);
				}
				
               program_preview_update_topview(attr);
				
      	   }
           break;
       }

       case GUI_MSG_KEY_ENTER:        
       {
           if(msg->dwAddData2 == KEY_DOWN_ACTION)
           {
               attr->new_focus = LISTBAR_GetFocusItem(attr->h_listbar);
           }
           break;
       }
       case GUI_MSG_KEY_MENU: 
       case GUI_MSG_KEY_LONGMENU:
       {
			if((msg->dwAddData2 == KEY_DOWN_ACTION) || (msg->dwAddData2 == KEY_REPEAT_ACTION))
			{
			
			}
            break;
       }
	   
       default:
       {
           break;
       }
   }
   
   return ret;
}

static __s32 _program_preview_frmlistbar_touch_proc(__gui_msg_t *msg)
{
	__s32 x = 0, y = 0;  
	__s32 speed = 0, direct = 0;  
	__s32 index = 0;
	__s32 ret = EPDK_OK;        
	static __s32 is_touch_down = 0;
	static __s32 is_touch_moving = 0;
	program_preview_frmattr_t *attr;
	attr = (program_preview_frmattr_t *)GUI_WinGetAddData(msg->h_deswin);

	x = LOSWORD(msg->dwAddData2);
	y = HISWORD(msg->dwAddData2);    
	speed = LOSWORD(msg->dwReserved);
	direct = HISWORD(msg->dwReserved);
	
	switch(msg->dwAddData1)
	{
		case GUI_MSG_TOUCH_DOWN:
		case GUI_MSG_TOUCH_LONGDOWN:
		{
			GUI_WinSetCaptureWin(msg->h_deswin);      
			ret = LISTBAR_TouchDown(attr->h_listbar, x, y, direct, speed); 
			is_touch_down = 1;
			break;
		}
		case GUI_MSG_TOUCH_MOVE:
		{
			if(is_touch_down == 1)
			{                
			    LISTBAR_TouchMove(attr->h_listbar, x, y, direct, speed);   
			   is_touch_moving = 1;
			}            
			break;
		}
		case GUI_MSG_TOUCH_UP:
		{
			__gui_msg_t send;
			RECT rect;
			__bool chg = EPDK_FALSE;

			if( is_touch_down == 0)
			{
			    break;
			}
			else
			{
			    is_touch_down = 0;
			}

			if(GUI_WinGetCaptureWin() == msg->h_deswin)
			{
			    GUI_WinReleaseCapture();
			}


			LISTBAR_TouchUp(attr->h_listbar, x, y, direct, speed);            


			{
			    index = LISTBAR_GetFocusItem(attr->h_listbar);
			}

			LISTBAR_GetItemRect(attr->h_listbar, index, &rect);
			if( attr->old_focus == attr->new_focus )
				chg = EPDK_TRUE;
			attr->old_focus = attr->new_focus ;
			if((x < rect.x) || (x > rect.x + rect.width) || (y < rect.y) || (y > rect.y + rect.height))
			{
				__here__
			    break;
			}
			else if(index != attr->new_focus)
			{
				attr->new_focus = index;
			}


			break;
		}
		default:
		{
			break;
		}
	}
	return ret;
}

static __s32  program_preview_frmtouch_proc(__gui_msg_t *msg)
{
	return EPDK_OK;
}



/*
	回调
*/		
static __s32 _program_preview_frmProc(__gui_msg_t *msg)
{
	switch(msg->id)
	{
	    case GUI_MSG_CREATE:
		{
			program_preview_frmattr_t *attr = NULL;
			
			program_preview_frmpara_t *para = NULL;
			
			HTHEME  bg_bmp;
			
			para = (program_preview_frmpara_t *)GUI_WinGetAttr(msg->h_deswin);
			
			attr = (program_preview_frmattr_t *)My_Balloc(sizeof(program_preview_frmattr_t));


			if(!attr)
			{
				__msg("memory balloc fail.........\n");
				return EPDK_FAIL;
			}
			eLIBs_memset(attr, 0, sizeof(program_preview_frmattr_t));
			
			attr->new_focus = para->focus_id;
			
			attr->old_focus = para->focus_id;
			
			attr->para = para;

	
			_program_preview_frmres_init(attr);

			attr->focus_txt_color   = PROGRAM_FOCUS_COLOR;
			
			attr->unfocus_txt_color = PROGRAM_UNFOCUS_COLOR;
			
			attr->font = para->font;
			
			attr->layer = para->layer;
			
			attr->total = maple_get_cur_service_list_num();

			
			GUI_WinSetAddData(msg->h_deswin, (__u32)attr);

			GUI_LyrWinSetSta(attr->layer, GUI_LYRWIN_STA_ON);

			GUI_LyrWinSetTop(attr->layer);  

			program_preview_update_topview(attr);

			if(attr->total > 0)
			{
				program_preview_frmlistbar_init(msg);

 	            list_long_string_init(attr);
		
			}
			
		}
		return EPDK_OK;

		
        case GUI_MSG_TIMER:       
    		return EPDK_OK;

	    case GUI_MSG_CLOSE:
		{
			GUI_FrmWinDelete(msg->h_deswin);
		}
		return EPDK_OK;


	    case GUI_MSG_DESTROY:
		{
			program_preview_frmattr_t *attr = NULL;
			
			program_preview_frmpara_t *para = NULL;
			
			__s32 ret;

			attr = (program_preview_frmattr_t *)GUI_WinGetAddData(msg->h_deswin);
			if(!attr)
			{
				__wrn("attr is null ......\n");
				return EPDK_FAIL;
			}

         	if(attr->total > 0)
			{
	            list_long_string_uninit(attr);
	         	
				ret = LISTBAR_SuspendPage(attr->h_listbar);    
				if(EPDK_FAIL == ret)
				{
					__msg("listbar suspend fail\n");
				}
				
	    		ret = LISTBAR_Delete(attr->h_listbar);
				if(EPDK_FAIL == ret )
				{
					__msg("listbar delete fail\n");
				}
					
	    		attr->h_listbar = NULL;
			}

			_program_preview_frmres_uninit(attr);

			para = attr->para;
			if(para)
			{
			    __wrn("...\n");
				My_Bfree(para, sizeof(program_preview_frmpara_t));
			}
			if(attr)
			{
			    __wrn("...\n");
				My_Bfree(attr, sizeof(program_preview_frmattr_t));
			}
			
		}
		return EPDK_OK;

	
	case GUI_MSG_KEY:		
		return _program_preview_frmlistbar_key_proc(msg);
		
	case GUI_MSG_TOUCH:		
		return program_preview_frmtouch_proc(msg);
				
	case GUI_MSG_COMMAND:
	{
	}		
		break;

	default:
		break;
	}
	
	return GUI_FrmWinDefaultProc(msg);	
}

H_WIN program_list_win_create(H_WIN h_parent, program_preview_frmpara_t *list_para)
{
	__gui_framewincreate_para_t framewin_para;
	program_preview_frmpara_t  *para;
	FB fb;

	__msg("*********enter  general program_preview_list win proc *****************\n");
	GUI_LyrWinGetFB(list_para->layer, &fb);

	para = (program_preview_frmpara_t *)My_Balloc(sizeof(program_preview_frmpara_t));
	if(!para)
	{
		__msg("memory balloc fail.........\n");
		return EPDK_FAIL;
	}
	eLIBs_memset((void*)para, 0, sizeof(program_preview_frmpara_t));
	para->font = SWFFont;
	para->layer = list_para->layer;
	para->focus_id = list_para->focus_id;
	eLIBs_memset(&framewin_para, 0, sizeof(__gui_framewincreate_para_t));
	framewin_para.name =	SUBMENU_DVB_PREVIEW_LST;
	framewin_para.dwExStyle = WS_EX_NONE;
	framewin_para.dwStyle = WS_NONE|WS_VISIBLE;
	framewin_para.spCaption =  NULL;
	framewin_para.hOwner	= NULL;
	framewin_para.id         = SUBMENU_DVB_PREVIEW_LIST_ID;
	framewin_para.hHosting = h_parent;
	framewin_para.FrameWinProc = (__pGUI_WIN_CB)esKRNL_GetCallBack((__pCBK_t)_program_preview_frmProc);
	framewin_para.rect.x =0;
	framewin_para.rect.y = 0;
	framewin_para.rect.width= fb.size.width;
	framewin_para.rect.height = fb.size.height;
	framewin_para.BkColor.alpha =  0;
	framewin_para.BkColor.red = 0;
	framewin_para.BkColor.green = 0;
	framewin_para.BkColor.blue = 0;
	framewin_para.attr =  (void *)para;
	framewin_para.hLayer = para->layer;

	return (GUI_FrmWinCreate(&framewin_para));
}


__s32 program_preview_frmwin_delete(H_WIN list_win)
{
	GUI_FrmWinDelete(list_win);
	return EPDK_OK;
}

