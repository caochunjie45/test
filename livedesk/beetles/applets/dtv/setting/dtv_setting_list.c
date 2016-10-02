
#include "dtv_setting_uipara.h"
#include "dtv_setting_list.h"


enum
{
	DTV_SETTING_PREVIEW_ID= 0,      //Ô¤ÀÀ
	DTV_SETTING_SEARCH_ID,          //ËÑË÷
	DTV_SETTING_DISPLAY_ID,         //ÏÔÊ¾
	DTV_SETTING_TRACK_ID,           //Òô¹ì
	DTV_SETTING_LCN_ID,             //LCN
	DTV_SETTING_SEVEN_DAYS_EPG_ID,  //EPG
	DTV_SETTING_ANTENNA_ID,         //ÌìÏß
	DTV_SETTING_MAX,
	
    DTV_SETTING_SSU_UPDATE_ID,
};

static __u32 setting_string_tab[] = 
{
	STRING_DTV_PROGRAM_PREVIEW,
	STRING_DTV_SETTING_SEARCH,	
	STRING_DTV_SETTING_DISPLAY,
	STRING_DTV_TRACK_TITLE,
	STRING_DTV_SET_LCN_TITLE,
	STRING_DTV_SEVEN_DAYS_EPG,
	STRING_DTV_ANTENNA_DE,
	STRING_DTV_ANTENNA_POWER,	
};

typedef struct tag_dtv_setting_list_focus_item_id
{
	__s32  id_bmp_focus_item;
	__s32  id_bmp_unfocus_item;
}dtv_setting_list_focus_item_id_t;

typedef struct tag_dtv_setting_list_attr
{

    HTHEME		h_bg_bmp;
	HTHEME     	h_list_top_bmp;
	HTHEME		h_item_focus_bmp;
	HTHEME      h_item_unfocus_bmp;

	__s32	focus_txt_color;
	__s32	unfocus_txt_color;

	H_LBAR	 h_listbar;
	H_WIN	layer;
	GUI_FONT *font;

	__s32 new_focus;
	
	__s32 old_focus;

	__s32 item_nr;
	
	dtv_setting_list_para_t *para;

	__s32 destroy_timerid;
	
	__s32 total;


	__hdle	long_str_handle;		//³¤×Ö·û´®¹ö¶¯¾ä±ú
	
}dtv_setting_list_attr_t;

extern void  main_cmd2parent(H_WIN hwin, __s32 id, __u32 data1, __u32 data2);


static __s32 dtv_setting_list_item_paint_ex(__lbar_draw_para_t *draw_param);

static void _dtv_setting_list_res_init(dtv_setting_list_attr_t *attr);

static void _dtv_setting_list_res_uninit(dtv_setting_list_attr_t *attr) ;


extern dtv_ctr_t  dtv_ctr;



static __s32 list_long_string_init(dtv_setting_list_attr_t *gattr)
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
 
 
 
 
 static __s32 list_long_string_uninit(dtv_setting_list_attr_t *gattr)
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
 	
     dtv_setting_list_attr_t *gattr = NULL;
     
     __u32 font_size = 0;
     
 
 	gattr = (dtv_setting_list_attr_t *)draw_param->attr;
 	
 	
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
 	    show_info.alpha_en = EPDK_FALSE;
 	    show_info.pFont = gattr->font;
 	    show_info.string = string;
 	    show_info.encode_id = encode;		
 	    
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
 
 
 
 static __s32 list_long_string_stop_roll(dtv_setting_list_attr_t *gattr)
 {
 	 if(gattr->long_str_handle!= NULL)
	 {    
	     return GUI_LongStringStop(gattr->long_str_handle);			
	 }
	 
 	 return EPDK_FAIL;
 }
 



static void dtv_setting_update_top_view(dtv_setting_list_attr_t *gattr)
{
	char tmp[128];
	GUI_RECT gui_rect;
	setting_uipara_t *uipara;

	uipara = setting_get_uipara();
	
	GUI_SetFont(gattr->font);
	GUI_UC_SetEncodeUTF8();
	GUI_SetDrawMode(GUI_DRAWMODE_NORMAL);

	GUI_BMP_Draw(theme_hdl2buf(gattr->h_list_top_bmp),uipara->title.x,uipara->title.y);
	gui_rect.x0 = uipara->title.x;
	gui_rect.y0 = uipara->title.y;
	gui_rect.x1 = gui_rect.x0 + uipara->title.width;
	gui_rect.y1 = gui_rect.y0 + uipara->title.height;
	
	dsk_langres_get_menu_text(STRING_DTV_SETTING,tmp,sizeof(tmp));
	
	GUI_SetColor(GUI_BLACK);
	
	GUI_DispStringInRect(tmp, &gui_rect, GUI_TA_HCENTER|GUI_TA_VCENTER); 

}

static __s32 dtv_setting_list_item_paint_ex(__lbar_draw_para_t *draw_param)
{
   	GUI_RECT gui_rect;
	dtv_setting_list_attr_t *gattr;
	maple_schdl_event_t *shdl_event = NULL;
	setting_uipara_t *uipara;
	
	char time[128];
	char tmp[128];
	
	char shdl_name[128];
	
	
	gattr = (dtv_setting_list_attr_t *)draw_param->attr;

	if (gattr==NULL)
		return EPDK_FAIL;
		
	if (GUI_LyrWinGetSta(gattr->layer) != GUI_LYRWIN_STA_ON)
		return EPDK_FAIL;

	uipara = setting_get_uipara();
	
	GUI_SetFont(gattr->font);
	
	GUI_UC_SetEncodeUTF8();
	
	GUI_SetDrawMode(GUI_DRAWMODE_NORMAL);

	switch(draw_param->mode)
	{
		case LBAR_MODE_NORMAL:
		{
			GUI_BMP_Draw(theme_hdl2buf(gattr->h_item_unfocus_bmp),draw_param->rect.x,draw_param->rect.y);

			GUI_SetColor(gattr->unfocus_txt_color);
			dsk_langres_get_menu_text(setting_string_tab[draw_param->index],shdl_name,sizeof(shdl_name));

			gui_rect.x0 = draw_param->rect.x;
			gui_rect.y0 = draw_param->rect.y;
			gui_rect.x1 = gui_rect.x0 + uipara->listbar.width;
			gui_rect.y1 = gui_rect.y0 + uipara->listbar.height;
			
			GUI_DispStringInRect(shdl_name, &gui_rect, GUI_TA_HCENTER|GUI_TA_VCENTER);    
		}
		break;
		
		case LBAR_MODE_FOCUS:
		{
			GUI_BMP_Draw(theme_hdl2buf(gattr->h_item_focus_bmp),draw_param->rect.x,draw_param->rect.y);

			GUI_SetColor(gattr->focus_txt_color);
			gui_rect.x0 = draw_param->rect.x;
			gui_rect.y0 = draw_param->rect.y;
			gui_rect.x1 = gui_rect.x0 + uipara->listbar.width;
			gui_rect.y1 = gui_rect.y0 + uipara->listbar.height;
			
			dsk_langres_get_menu_text(setting_string_tab[draw_param->index],shdl_name,sizeof(shdl_name));

			GUI_DispStringInRect(shdl_name, &gui_rect, GUI_TA_HCENTER|GUI_TA_VCENTER);   

			list_long_string_start_roll_fast(draw_param,shdl_name);

		}
		break;
		
		default:
			break;
			
	}
	return EPDK_OK;
}



static void dtv_setting_list_listbar_init(__gui_msg_t *msg)
{
	__listbar_config_t config; 
	RECT rect;
	dtv_setting_list_attr_t *attr;
	__s32 index;
	__s32 start_id,focus_id;
	setting_uipara_t *uipara;
	
	attr = (dtv_setting_list_attr_t *)GUI_WinGetAddData(msg->h_deswin);

	uipara = setting_get_uipara();

	{
		start_id = 0;
		focus_id = attr->new_focus;
	}
	
	__msg("attr->total = %d\n",attr->total);
	config.alpha_status = EPDK_FALSE;
	config.bk_color = 0;
	config.start_id = start_id;
	config.focus_id = focus_id;			
	config.item_cnt = attr->total;
	config.item_height = uipara->listbar.height;
	config.item_width  = uipara->listbar.width;
	config.list_rect.x = uipara->listbar.x;
	config.list_rect.y = uipara->listbar.y;
	config.list_rect.width = uipara->listbar_width;
	config.list_rect.height = attr->total*uipara->listbar.height;

	config.list_attr = (void*)attr;

	attr->h_listbar = LISTBAR_Create(msg->h_deswin); 
	LISTBAR_Config(attr->h_listbar, dtv_setting_list_item_paint_ex, &config);    
    LISTBAR_ShowPage(attr->h_listbar);              
	
}




/*
	ÉêÇë×ÊÔ´
*/
static void _dtv_setting_list_res_init(dtv_setting_list_attr_t *attr)
{
	__u32 	i; 
	__bool  restore_flag;

	attr->h_item_focus_bmp = theme_open(ID_DTV_PRELIST_F_BMP);	
	attr->h_item_unfocus_bmp= theme_open(ID_DTV_PRELIST_UF_BMP);		
	attr->h_list_top_bmp = theme_open(ID_DTV_SETTING_TOP_BMP);
	
}

static void _dtv_setting_list_res_uninit(dtv_setting_list_attr_t *attr)
{
    __u32 	i; 
         
	theme_close(attr->h_list_top_bmp);
		attr->h_list_top_bmp=0;
	theme_close(attr->h_item_focus_bmp);	
		attr->h_item_focus_bmp = 0;
	theme_close(attr->h_item_unfocus_bmp);
		attr->h_item_unfocus_bmp = 0;
	
}


static __s32 dtv_setting_send_cmd_to_parent(__gui_msg_t *msg,__u32 id,__u32 data1,__u32 data2)
{
	__gui_msg_t dmsg;

	dmsg.id = id;
	dmsg.h_deswin = GUI_WinGetParent(msg->h_deswin);
	dmsg.h_srcwin = NULL;
	dmsg.dwAddData1 = data1 ;
	dmsg.dwAddData2 = data2;
	dmsg.dwReserved = 0;

	GUI_SendNotifyMessage(&dmsg);

    return EPDK_OK;
}

static __s32 _dtv_setting_list_listbar_key_proc(__gui_msg_t *msg)
{    
    __s32 ret = EPDK_OK;
    
	dtv_setting_list_attr_t *attr;

	attr = (dtv_setting_list_attr_t *)GUI_WinGetAddData(msg->h_deswin);

   switch(msg->dwAddData1)
   {
       case GUI_MSG_KEY_UP:
       case GUI_MSG_KEY_LONGUP:
       case GUI_MSG_KEY_LEFT:
       case GUI_MSG_KEY_LONGLEFT:		   	
       {
           if((msg->dwAddData2 == KEY_DOWN_ACTION) || (msg->dwAddData2 == KEY_REPEAT_ACTION))
           {
   			    attr->old_focus = attr->new_focus;
                LISTBAR_PrevItem(attr->h_listbar);
				attr->new_focus = LISTBAR_GetFocusItem(attr->h_listbar);
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
   				   attr->old_focus = attr->new_focus;
                   LISTBAR_NextItem(attr->h_listbar);	
				   attr->new_focus = LISTBAR_GetFocusItem(attr->h_listbar);
      	   }
           break;
       }

       case GUI_MSG_KEY_ENTER:        
       case GUI_MSG_KEY_LONGENTER:        
       {
           if(msg->dwAddData2 == KEY_UP_ACTION)
           {
               attr->new_focus = LISTBAR_GetFocusItem(attr->h_listbar);
			   __msg("attr->new_focus = %d \n",attr->new_focus);
			   
			   switch(attr->new_focus)
			   {
					case DTV_SETTING_SEARCH_ID://ÖØÐÂËÑË÷
					{
						dtv_setting_send_cmd_to_parent(msg,GUI_MSG_COMMAND,DTV_SETTING_SEARCH,DTV_RESEARCH_SERVICE_LIST);
						break;
					}
					case DTV_SETTING_PREVIEW_ID:
					{
						dtv_setting_send_cmd_to_parent(msg,GUI_MSG_COMMAND,DTV_SETTING_PREVIEW,DTV_PREVIEW_SERVICE_LIST);
						break;
					}
					case DTV_SETTING_DISPLAY_ID:
					{
						dtv_setting_send_cmd_to_parent(msg,GUI_MSG_COMMAND,DTV_SETTING_DISPLAY,0);
						break;
					}
					case DTV_SETTING_TRACK_ID:
					{
						dtv_setting_send_cmd_to_parent(msg,GUI_MSG_COMMAND,DTV_SETTING_TRACK,0);
						break;
					}
					case DTV_SETTING_LCN_ID:
					{
						dtv_setting_send_cmd_to_parent(msg,GUI_MSG_COMMAND,DTV_SETTING_LCN,0);
						break;
					}
					case DTV_SETTING_SEVEN_DAYS_EPG_ID:
					{
						dtv_setting_send_cmd_to_parent(msg,GUI_MSG_COMMAND,DTV_SETTING_EPG,0);
						break;
					}
					case DTV_SETTING_ANTENNA_ID:
					{
						dtv_setting_send_cmd_to_parent(msg,GUI_MSG_COMMAND,DTV_SETTING_ANTENNA,0);
						break;
					}
					
                  
					case DTV_SETTING_SSU_UPDATE_ID:
					{
						dtv_setting_send_cmd_to_parent(msg,GUI_MSG_COMMAND,DTV_SETTING_SSU_UPDATE,0);
						break;
					}
					
			   }
           }
           break;
       }

	   case GUI_MSG_KEY_ESCAPE:
	   case GUI_MSG_KEY_LONGESCAPE:
	   {
			if(msg->dwAddData2 == KEY_UP_ACTION)
			{
			    __wrn("dtv_ctr.h_setting    : 0x%08x\n", dtv_ctr.h_setting);
				dtv_setting_send_cmd_to_parent(msg,GUI_MSG_COMMAND,DTV_SETTING_REPLAY,0);
			}
			break;
	   }
       default:
       {
           break;
       }
   }
    return EPDK_OK;
}

static __s32 _dtv_setting_list_listbar_touch_proc(__gui_msg_t *msg)
{
	__s32 x = 0, y = 0;  
	__s32 speed = 0, direct = 0;  
	__s32 index = 0;
	__s32 ret = EPDK_OK;        
	static __s32 is_touch_down = 0;
	static __s32 is_touch_moving = 0;
	dtv_setting_list_attr_t *attr;
	attr = (dtv_setting_list_attr_t *)GUI_WinGetAddData(msg->h_deswin);

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
				__here__;
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

static __s32  dtv_setting_list_touch_proc(__gui_msg_t *msg)
{
	return EPDK_OK;
}


static __s32 _dtv_setting_list_Proc(__gui_msg_t *msg)
{
	__msg("_dvt_setting_list_Proc()::msg->id=%08x, dwAddData1=%08x, dwAddData2=%08x\n",msg->id, msg->dwAddData1, msg->dwAddData2);

	switch(msg->id)
	{
		case GUI_MSG_CREATE:
		{
			dtv_setting_list_attr_t *attr;
			dtv_setting_list_para_t *para;
			HTHEME  bg_bmp;
			
			para = (dtv_setting_list_para_t *)GUI_WinGetAttr(msg->h_deswin);
			attr = (dtv_setting_list_attr_t *)My_Balloc(sizeof(dtv_setting_list_attr_t));
			if(!attr)
			{
				__msg("memory balloc fail.........\n");
				return EPDK_FAIL;
			}
			eLIBs_memset(attr, 0, sizeof(dtv_setting_list_attr_t));
			
			attr->new_focus = para->focus_id;
			attr->old_focus = para->focus_id;
			attr->para = para;

	
			_dtv_setting_list_res_init(attr);

			attr->focus_txt_color = dtv_SETTING_FOCUS_COLOR;
			
			attr->unfocus_txt_color = dtv_SETTING_UNFOCUS_COLOR;
			
			attr->font = para->font;
			
			attr->layer = para->layer;
			
			attr->total = DTV_SETTING_MAX;
			
			__msg("attr->total = %d\n",attr->total);
			

			GUI_WinSetAddData(msg->h_deswin, (__u32)attr);
			
			GUI_LyrWinSetSta(attr->layer, GUI_LYRWIN_STA_ON);
			
			GUI_LyrWinSetTop(attr->layer);

			dtv_setting_update_top_view(attr);

			dtv_setting_list_listbar_init(msg);
			
            list_long_string_init(attr);

		}
		return EPDK_OK;
		
		case GUI_MSG_CLOSE:
		{
            __wrn("-------------------------------------GUI_MSG_CLOSE enter, err.\n");
			GUI_FrmWinDelete(msg->h_deswin);
		}
		return EPDK_OK;

		case GUI_MSG_DESTROY:
		{
			dtv_setting_list_attr_t *attr;
			dtv_setting_list_para_t *para;
			__s32 ret;
			attr = (dtv_setting_list_attr_t *)GUI_WinGetAddData(msg->h_deswin);
			if(!attr)
			{
				__wrn("attr is null ......\n");
				return EPDK_FAIL;
			}

			ret = LISTBAR_SuspendPage(attr->h_listbar);    
			if(EPDK_FAIL == ret )
				__msg("listbar suspend fail\n");
    		ret = LISTBAR_Delete(attr->h_listbar);
			if(EPDK_FAIL == ret )
				__msg("listbar delete fail\n");
    		attr->h_listbar = NULL;

            list_long_string_uninit(attr);

			_dtv_setting_list_res_uninit(attr);


			para = attr->para;
			if(para)
			{
				My_Bfree(para, sizeof(dtv_setting_list_para_t));
				para = NULL;
			}
			if(attr)
			{
				My_Bfree(attr, sizeof(dtv_setting_list_attr_t));
				attr = NULL;
			}
			
		}
		return EPDK_OK;

		case GUI_MSG_PAINT:
		{
		}
		return EPDK_OK;

		case GUI_MSG_KEY:		
			return _dtv_setting_list_listbar_key_proc(msg);
		
		case GUI_MSG_TOUCH:		
			return dtv_setting_list_touch_proc(msg);
				
		case GUI_MSG_COMMAND:
		{

		}		
		break;

		default:
			break;
	}
	
	return GUI_FrmWinDefaultProc(msg);	
}

H_WIN dtv_setting_list_win_create(H_WIN h_parent, dtv_setting_list_para_t *list_para)
{
	__gui_framewincreate_para_t framewin_para;
	dtv_setting_list_para_t  *para;
	FB fb;

	GUI_LyrWinGetFB(list_para->layer, &fb);

	para = (dtv_setting_list_para_t *)My_Balloc(sizeof(dtv_setting_list_para_t));
	if(!para)
	{
		__msg("memory balloc fail.........\n");
		return EPDK_FAIL;
	}
	eLIBs_memset((void*)para, 0, sizeof(dtv_setting_list_para_t));
	para->font = SWFFont;
	para->layer = list_para->layer;
	para->focus_id = list_para->focus_id;
	eLIBs_memset(&framewin_para, 0, sizeof(__gui_framewincreate_para_t));
	framewin_para.name =	SUBMENU_DVB_SETTING_LST;
	framewin_para.dwExStyle = WS_EX_NONE;
	framewin_para.dwStyle = WS_NONE|WS_VISIBLE;
	framewin_para.spCaption =  NULL;
	framewin_para.hOwner	= NULL;
	framewin_para.id         = SUBMENU_DVB_SETTING_LIST_ID;
	framewin_para.hHosting = h_parent;
	framewin_para.FrameWinProc = (__pGUI_WIN_CB)esKRNL_GetCallBack((__pCBK_t)_dtv_setting_list_Proc);
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


__s32 dtv_setting_list_win_delete(H_WIN list_win)
{
	GUI_FrmWinDelete(list_win);
	return EPDK_OK;
}

