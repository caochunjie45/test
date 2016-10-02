/*
**************************************************************************************************************
*											         ePDK
*						            the Easy Portable/Player Develop Kits
*									           desktop system 
*
*						        	 (c) Copyright 2007-2010, ANDY, China
*											 All Rights Reserved
*
* File    	: schedule_list_general.c
* By      	: Andy.zhang
* Func		: desk main thread
* Version	: v1.0
* ============================================================================================================
* 2009-7-20 8:51:52  andy.zhang  create this file, implements the fundemental interface;
**************************************************************************************************************
*/


#include "schedule_uipara.h"
#include "schedule_list.h"


extern __s32 g_weak_singal;

typedef struct tag_schedule_list_focus_item_id
{
	__s32  id_bmp_focus_item;
	
	__s32  id_bmp_unfocus_item;
	
}schedule_list_focus_item_id_t;

typedef struct tag_schedule_list_attr
{

	HTHEME     	h_list_top_bmp;
	
	HTHEME     	h_epg_roll_bg_bmp;	
	
	HTHEME		h_item_focus_bmp;
	
	HTHEME		h_item_unfocus_bmp;
	
	HTHEME		h_epg_content;

	__s32	focus_txt_color;
	
	__s32	unfocus_txt_color;

	H_LBAR	 h_listbar;
	
	H_WIN	layer;
	
	GUI_FONT *font;

	__s32 new_focus;
	
	__s32 old_focus;

    RECT epg_content;
    
	__s32 item_nr;
	
	schedule_list_para_t *para;
	
	__hdle	long_str_handle;		//长字符串滚动句柄

	__s32 timer_scan_schedule_id;
	
	__s32 total;
	
}schedule_list_attr_t;

extern void  main_cmd2parent(H_WIN hwin, __s32 id, __u32 data1, __u32 data2);


static __s32 schedule_list_item_paint_ex(__lbar_draw_para_t *draw_param);
static void _schedule_list_res_init(schedule_list_attr_t *attr);
static void _schedule_list_res_uninit(schedule_list_attr_t *attr) ;


static __s32 list_long_string_init(schedule_list_attr_t *gattr)
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


static __s32 list_long_string_uninit(schedule_list_attr_t *gattr)
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
	RECT roll_rect, text_rect;
	
    schedule_list_attr_t *gattr;
    __u32 font_size = 0;
    schedule_uipara_t *schedule_uipara;

	schedule_uipara = schedule_get_uipara();

	gattr = (schedule_list_attr_t *)draw_param->attr;
	
	
	if(gattr->long_str_handle != NULL)
	{
		eLIBs_memset(&show_info, 0, sizeof(__show_info_move_t));
		
    	roll_rect.x = draw_param->rect.x + schedule_uipara->epg_content_x_left_offset;
		roll_rect.y = draw_param->rect.y;
		roll_rect.width = schedule_uipara->epg_content_x_right_roll_offset;
		roll_rect.height = schedule_uipara->listbar.height;

		font_size = GUI_GetFontDistY();

		
	    show_info.hlayer = gattr->layer;
	    show_info.region.x = roll_rect.x;
	    show_info.region.y = roll_rect.y;
	    show_info.region.width = roll_rect.width;
	    show_info.region.height = roll_rect.height;
	    show_info.alpha_en = EPDK_FALSE;
	    show_info.pFont = gattr->font;
	    show_info.string = string;
	    show_info.encode_id = encode;		
	    
	    show_info.fontColor = gattr->focus_txt_color;
	    show_info.align = GUI_TA_LEFT | GUI_TA_VCENTER;

        show_info.bmp = theme_hdl2buf(gattr->h_epg_roll_bg_bmp);
        show_info.bmp_pos.x = 0;
        show_info.bmp_pos.y = 0;

	    GUI_LongStringStop(gattr->long_str_handle);
	    GUI_LongStringReset(gattr->long_str_handle,&show_info);
	    GUI_LongStringStart(gattr->long_str_handle);	
		
	}
	return EPDK_OK;
}


static __s32 list_long_string_stop_roll(schedule_list_attr_t *gattr)
{
	if(gattr->long_str_handle!= NULL)
    {    
        return GUI_LongStringStop(gattr->long_str_handle);			
    }
	return EPDK_FAIL;
}


static void schedule_update_top_view(schedule_list_attr_t *gattr)
{
	char tmp[128];
	GUI_RECT gui_rect;
	schedule_uipara_t *schedule_uipara;

	schedule_uipara = schedule_get_uipara();

	GUI_SetFont(gattr->font);
	GUI_UC_SetEncodeUTF8();
	GUI_SetDrawMode(GUI_DRAWMODE_NORMAL);
	
	GUI_BMP_Draw(theme_hdl2buf(gattr->h_list_top_bmp),schedule_uipara->title.x,schedule_uipara->title.y);
    
	gui_rect.x0 = schedule_uipara->title.x;
	gui_rect.y0 = schedule_uipara->title.y;
	gui_rect.x1 = gui_rect.x0 + schedule_uipara->title.width;
	gui_rect.y1 = gui_rect.y0 + schedule_uipara->title.height;

	dsk_langres_get_menu_text(STRING_DTV_SEVEN_DAYS_EPG,tmp,sizeof(tmp));
	GUI_SetColor(GUI_BLACK);
	GUI_DispStringInRect(tmp, &gui_rect, GUI_TA_HCENTER|GUI_TA_VCENTER); 

	

}

static __s32 schedule_list_item_paint_ex(__lbar_draw_para_t *draw_param)
{
	__s32 i;
	RECT rect;
   	GUI_RECT gui_rect;
	schedule_list_attr_t *gattr;
	maple_schdl_event_t *shdl_event = NULL;
	char time[128];
	char tmp[128];
	schedule_uipara_t *schedule_uipara;
	
	char shdl_name[MAPLE_MAX_NAME_LEN];
	
	gattr = (schedule_list_attr_t *)draw_param->attr;
	schedule_uipara = schedule_get_uipara();

	if (gattr==NULL)
		return EPDK_FAIL;
		
	if (GUI_LyrWinGetSta(gattr->layer) != GUI_LYRWIN_STA_ON)
		return EPDK_FAIL;

	GUI_SetFont(gattr->font);
	GUI_UC_SetEncodeUTF8();
	GUI_SetDrawMode(GUI_DRAWMODE_NORMAL);
	GUI_LyrWinCacheOn();

    eLIBs_memset(time,0,sizeof(time));
    eLIBs_memset(tmp,0,sizeof(tmp));
    eLIBs_memset(shdl_name,0,sizeof(shdl_name));

    //获取节目安排信息
	shdl_event = maple_epg_get_schdl_event(0,draw_param->index);
    if(shdl_event == NULL)
        return EPDK_FAIL;
    

    eLIBs_sprintf(time,"%02d",shdl_event->startTime.month);    
	eLIBs_strcat(time,"/");
	eLIBs_sprintf(tmp,"%02d  ",shdl_event->startTime.day);        
	eLIBs_strcat(time,tmp);
	eLIBs_sprintf(tmp,"%02d:",shdl_event->startTime.hour);
	eLIBs_strcat(time,tmp);
	eLIBs_sprintf(tmp,"%02d",shdl_event->startTime.min);
	eLIBs_strcat(time,tmp);
    
	eLIBs_strcpy(shdl_name,shdl_event->name);
        
	switch(draw_param->mode)
	{
		case LBAR_MODE_NORMAL:
		{
			GUI_BMP_Draw(theme_hdl2buf(gattr->h_item_unfocus_bmp),draw_param->rect.x,draw_param->rect.y);

			gui_rect.x0 = draw_param->rect.x + schedule_uipara->epg_data_x_left_offset;
			gui_rect.y0 = draw_param->rect.y;
			gui_rect.x1 = gui_rect.x0 + schedule_uipara->epg_data_x_right_offset;
			gui_rect.y1 = gui_rect.y0 + schedule_uipara->listbar.height;

			GUI_SetColor(gattr->unfocus_txt_color);
			GUI_DispStringInRect(time, &gui_rect, GUI_TA_LEFT|GUI_TA_VCENTER);    

			gui_rect.x0 = draw_param->rect.x + schedule_uipara->epg_content_x_left_offset;
			gui_rect.y0 = draw_param->rect.y;
			gui_rect.x1 = gui_rect.x0 + schedule_uipara->epg_content_x_right_offset;
			gui_rect.y1 = gui_rect.y0 + schedule_uipara->listbar.height;
			
			GUI_DispStringInRect(shdl_name, &gui_rect,GUI_TA_VCENTER);    
		}
		break;
        
		case LBAR_MODE_FOCUS:
		{            
			GUI_BMP_Draw(theme_hdl2buf(gattr->h_item_focus_bmp),draw_param->rect.x,draw_param->rect.y);

			gui_rect.x0 = draw_param->rect.x + schedule_uipara->epg_data_x_left_offset;
			gui_rect.y0 = draw_param->rect.y;
			gui_rect.x1 = gui_rect.x0 + schedule_uipara->epg_data_x_right_offset;
			gui_rect.y1 = gui_rect.y0 + schedule_uipara->listbar.height;

			GUI_SetColor(gattr->focus_txt_color);
			GUI_DispStringInRect(time, &gui_rect, GUI_TA_LEFT|GUI_TA_VCENTER);    

			gui_rect.x0 = draw_param->rect.x + schedule_uipara->epg_content_x_left_offset;
			gui_rect.y0 = draw_param->rect.y;
			gui_rect.x1 = gui_rect.x0 + schedule_uipara->epg_content_x_right_offset;
			gui_rect.y1 = gui_rect.y0 + schedule_uipara->listbar.height;
			
			GUI_DispStringInRect(shdl_name, &gui_rect,GUI_TA_VCENTER); 

			list_long_string_start_roll_fast(draw_param,shdl_name);
		}
		break;
		default:
			break;
			
	}
	GUI_LyrWinCacheOff();

	return EPDK_OK;
}


static __s32 schedule_list_enventcontent_paint(schedule_list_attr_t *gattr,__u32 draw_index)
{
   	GUI_RECT gui_rect;
	maple_schdl_event_t *shdl_event = NULL;
	schedule_uipara_t *schedule_uipara;
	
	if (gattr==NULL)
		return EPDK_FAIL;
		
	if (GUI_LyrWinGetSta(gattr->layer) != GUI_LYRWIN_STA_ON)
		return EPDK_FAIL;

	schedule_uipara = schedule_get_uipara();
	
	GUI_SetFont(gattr->font);
	GUI_UC_SetEncodeUTF8();
	GUI_SetDrawMode(GUI_DRAWMODE_NORMAL);
    GUI_LyrWinSel(gattr->layer);

	GUI_LyrWinCacheOn();


	shdl_event = maple_epg_get_schdl_event(0,draw_index);
    if(shdl_event == NULL)
        return EPDK_FAIL;

    if(draw_index < 0)
        return EPDK_FAIL;
        
    if(shdl_event->eventContentLen > 1024)
        return EPDK_FAIL;

    eDbug("shdl_event->eventContent is %s \n",shdl_event->eventContent);
    
	{
        GUI_BMP_Draw(theme_hdl2buf(gattr->h_epg_content),schedule_uipara->epg_detailed_content.x,schedule_uipara->epg_detailed_content.y);
        
		gui_rect.x0 = schedule_uipara->epg_detailed_content_text.x;
		gui_rect.y0 = schedule_uipara->epg_detailed_content_text.y;
		gui_rect.x1 = gui_rect.x0 + schedule_uipara->epg_detailed_content_text.width;
		gui_rect.y1 = gui_rect.y0 + schedule_uipara->epg_detailed_content_text.height;
        GUI_SetColor(GUI_BLACK);
		GUI_DispStringInRectWrap(shdl_event->eventContent, &gui_rect,GUI_TA_VCENTER,GUI_WRAPMODE_WORD); 
	}
	GUI_LyrWinCacheOff();

	return EPDK_OK;

}

static __s32 schedule_list_enventcontent_clear(void)
{
   	GUI_RECT gui_rect;
	schedule_uipara_t *schedule_uipara;

	schedule_uipara = schedule_get_uipara();

    gui_rect.x0 = schedule_uipara->epg_detailed_content.x;
	gui_rect.y0 = schedule_uipara->epg_detailed_content.y;
	gui_rect.x1 = gui_rect.x0 + schedule_uipara->epg_detailed_content.width;
	gui_rect.y1 = gui_rect.y0 + schedule_uipara->epg_detailed_content.height;

    GUI_ClearRect(gui_rect.x0, gui_rect.y0, gui_rect.x1, gui_rect.y1); 

	return EPDK_OK;

}

static void schedule_list_listbar_init(__gui_msg_t *msg)
{
	__listbar_config_t config; 
	schedule_list_attr_t *attr;
	__s32 index;
	__s32 start_id,focus_id;
	schedule_uipara_t *schedule_uipara;
	
	attr = (schedule_list_attr_t *)GUI_WinGetAddData(msg->h_deswin);
	schedule_uipara = schedule_get_uipara();
	
	
	index = maple_get_cur_schedule_index(0);
    eDbug("index is %d \n",index);
	if(index < schedule_uipara->listbar_number)
	{
		start_id = 0;
		focus_id = index;
	}
	else
	{
		start_id = index - schedule_uipara->listbar_number + 1;
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
	config.item_height = schedule_uipara->listbar.height;
	config.item_width = schedule_uipara->listbar.width;
	config.list_rect.x = schedule_uipara->listbar.x;
	config.list_rect.y = schedule_uipara->listbar.y;
	if(attr->total >= schedule_uipara->listbar_number)
	{
		config.list_rect.width = schedule_uipara->listbar_width;
		config.list_rect.height = schedule_uipara->listbar.height * schedule_uipara->listbar_number ;
	}
	else
	{
		config.list_rect.width = schedule_uipara->listbar_width;
		config.list_rect.height = schedule_uipara->listbar.height * attr->total;	
	}
	
	config.list_attr = (void*)attr;

	attr->h_listbar = LISTBAR_Create(msg->h_deswin); 
	LISTBAR_Config(attr->h_listbar, schedule_list_item_paint_ex, &config);    
    LISTBAR_ShowPage(attr->h_listbar);              
	
}




/*
	申请资源
*/
static void _schedule_list_res_init(schedule_list_attr_t *attr)
{
	__u32 	i; 
	__bool  restore_flag;

	attr->h_item_focus_bmp     = theme_open(ID_DTV_LIST_F_BMP);	
	attr->h_item_unfocus_bmp   = theme_open(ID_DTV_LIST_UF_BMP);		
	attr->h_list_top_bmp       = theme_open(ID_DTV_EPG_TOP_BMP);
    attr->h_epg_content        = theme_open(ID_DTV_EPG_CONTENT_BMP);
    attr->h_epg_roll_bg_bmp    = theme_open(ID_DTV_EPG_ROLL_BG_BMP);
}

static void _schedule_list_res_uninit(schedule_list_attr_t *attr)
{
         
	theme_close(attr->h_list_top_bmp);
		attr->h_list_top_bmp=0;
	theme_close(attr->h_item_focus_bmp);	
		attr->h_item_focus_bmp = 0;
	theme_close(attr->h_item_unfocus_bmp);
		attr->h_item_unfocus_bmp = 0;
 	theme_close(attr->h_epg_content);
		attr->h_epg_content = 0;
 	theme_close(attr->h_epg_roll_bg_bmp);
		attr->h_epg_roll_bg_bmp = 0;
		
}


static __s32 _schedule_list_listbar_key_proc(__gui_msg_t *msg)
{    
    	__s32 ret = EPDK_OK;    
	schedule_list_attr_t *attr;

	attr = (schedule_list_attr_t *)GUI_WinGetAddData(msg->h_deswin);
    
    	switch(msg->dwAddData1)
       {
           case GUI_MSG_KEY_UP:
           case GUI_MSG_KEY_LONGUP:
		   case GUI_MSG_KEY_LEFT:
		   case GUI_MSG_KEY_LONGLEFT:		   	
           {
               if((msg->dwAddData2 == KEY_DOWN_ACTION) || (msg->dwAddData2 == KEY_REPEAT_ACTION))
               {
                    //schedule_list_enventcontent_clear();
                    list_long_string_stop_roll(attr);
                    attr->old_focus = attr->new_focus;
                    LISTBAR_PrevItem(attr->h_listbar);

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
                   //schedule_list_enventcontent_clear();
                   list_long_string_stop_roll(attr);
   				   attr->old_focus = attr->new_focus;
                   LISTBAR_NextItem(attr->h_listbar);					   

          	   }
               break;
           }
           case GUI_MSG_KEY_ENTER:        
           {
               if(msg->dwAddData2 == KEY_UP_ACTION)
               {
                   list_long_string_stop_roll(attr);
                   attr->new_focus = LISTBAR_GetFocusItem(attr->h_listbar);
                   eDbug("attr->new_focus is %d \n",attr->new_focus);
                   //schedule_list_enventcontent_paint(attr,attr->new_focus);
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




static __s32 _schedule_list_Proc(__gui_msg_t *msg)
{
	switch(msg->id)
	{
		case GUI_MSG_CREATE:
		{
			schedule_list_attr_t *attr;
			schedule_list_para_t *para;
			__s32 i;

			
			para = (schedule_list_para_t *)GUI_WinGetAttr(msg->h_deswin);
			attr = (schedule_list_attr_t *)My_Balloc(sizeof(schedule_list_attr_t));
			if(!attr)
			{
				__msg("memory balloc fail.........\n");
				return EPDK_FAIL;
			}
			eLIBs_memset(attr, 0, sizeof(schedule_list_attr_t));
			
			attr->new_focus = para->focus_id;
			
			attr->old_focus = para->focus_id;
			
			attr->para = para;

	
			_schedule_list_res_init(attr);
			
			attr->focus_txt_color = SCHEDULE_FOCUS_COLOR;
			
			attr->unfocus_txt_color = SCHEDULE_UNFOCUS_COLOR;
			
			attr->font = para->font;
			
			attr->layer = para->layer;
			
			__msg("attr->total = %d\n",attr->total);
			
            list_long_string_init(attr);
			GUI_WinSetAddData(msg->h_deswin, (__u32)attr);
			GUI_LyrWinSetSta(attr->layer, GUI_LYRWIN_STA_ON);
			GUI_LyrWinSetTop(attr->layer);            


			attr->timer_scan_schedule_id = SUBMENU_DVB_SCHUDULE_TIMER_ID;
			GUI_SetTimer(msg->h_deswin, attr->timer_scan_schedule_id, 100, NULL);			
		}
		return EPDK_OK;
		
		case GUI_MSG_CLOSE:
		{
            schedule_list_attr_t *attr;
			
			attr = (schedule_list_attr_t *)GUI_WinGetAddData(msg->h_deswin);
			if(!attr)
			{
				__wrn("attr is null ......\n");
				return EPDK_FAIL;
			}			
			GUI_LyrWinDelete(attr->layer);
			attr->layer = NULL;
		
		}
		return EPDK_OK;
		
		case GUI_MSG_PAINT:
		{
			schedule_list_attr_t *attr;
			__s32 i;
			schedule_uipara_t *schedule_uipara;
			
			attr = (schedule_list_attr_t *)GUI_WinGetAddData(msg->h_deswin);
			if(!attr)
			{
				__wrn("attr is null ......\n");
				return EPDK_FAIL;
			}		

			if(GUI_LyrWinGetSta(attr->layer) != GUI_LYRWIN_STA_ON)
			{
				return EPDK_FAIL;
			}

			schedule_uipara = schedule_get_uipara();
			
			attr->total = maple_epg_get_eventnum(0);//只获取当天的

			schedule_update_top_view(attr);	                   
			if(attr->total > 0)//无节目列表时不创建listbar
			{
				schedule_list_listbar_init(msg);
			}
            else
            {
            	for(i=0; i< schedule_uipara->listbar_number; i++)
            	{
            		GUI_BMP_Draw(theme_hdl2buf(attr->h_item_unfocus_bmp),schedule_uipara->listbar.x,schedule_uipara->listbar.y + schedule_uipara->listbar.height *(i));
            	}					 
            }

			return EPDK_OK;
		}
		case GUI_MSG_DESTROY:
		{
			schedule_list_attr_t *attr;
			schedule_list_para_t *para;
			__s32 ret;

            
			attr = (schedule_list_attr_t *)GUI_WinGetAddData(msg->h_deswin);
			if(!attr)
			{
				__wrn("attr is null ......\n");
				return EPDK_FAIL;
			}

			if(GUI_IsTimerInstalled(msg->h_deswin,attr->timer_scan_schedule_id))
			{
				GUI_KillTimer(msg->h_deswin,attr->timer_scan_schedule_id);
			}
			if(attr->layer)
			{
				GUI_LyrWinSel(attr->layer);
                if(GUI_LYRWIN_STA_ON != GUI_LyrWinGetSta(attr->layer)&&GUI_LYRWIN_STA_SLEEP != GUI_LyrWinGetSta(attr->layer))
                {
                    __wrn("------------------------Err Frame Buf fail.\n");            
                    
                }
                else
                {
                    GUI_Clear();
                }
				
			}
			

			ret = LISTBAR_SuspendPage(attr->h_listbar);    
			if(EPDK_FAIL == ret )
			__msg("listbar suspend fail\n");
    			ret = LISTBAR_Delete(attr->h_listbar);
			if(EPDK_FAIL == ret )
				__msg("listbar delete fail\n");
    		attr->h_listbar = NULL;

			_schedule_list_res_uninit(attr);
            list_long_string_uninit(attr);


			para = attr->para;
			if(para)
			{
				My_Bfree(para, sizeof(schedule_list_para_t));
			}
			if(attr)
			{
				My_Bfree(attr, sizeof(schedule_list_attr_t));
			}
			
		}
		return EPDK_OK;


	case GUI_MSG_TIMER:
	{
		schedule_list_attr_t *attr;
		__s32 schudule_num;
		__s32 i;
		schedule_uipara_t *schedule_uipara;

		attr = (schedule_list_attr_t *)GUI_WinGetAddData(msg->h_deswin);
		if(!attr)
		{
			__wrn("attr is null ......\n");
			return EPDK_FAIL;
		}
		schedule_uipara = schedule_get_uipara();
		
		if(attr->timer_scan_schedule_id == msg->dwAddData1)
		{
		    GUI_ResetTimer(msg->h_deswin, attr->timer_scan_schedule_id, 100, NULL);
					    
		    schudule_num = maple_epg_get_eventnum(0);
			
			if(attr->total != schudule_num || 0 == attr->total)
			{
				if(g_weak_singal == EPDK_FALSE)
			        maple_epg_getdata();
                else
                    maple_epg_reset_schdl_list_day(0);

                schudule_num = maple_epg_get_eventnum(0);

				 GUI_LyrWinSel(attr->layer);
                 if(GUI_LYRWIN_STA_ON != GUI_LyrWinGetSta(attr->layer)&& GUI_LYRWIN_STA_SLEEP != GUI_LyrWinGetSta(attr->layer))
                 {
                    __wrn("------------------------Err Frame Buf fail.\n");            
                 }
                 attr->total = schudule_num;
				 LISTBAR_Delete(attr->h_listbar);
				 attr->h_listbar = NULL;
                 
                 schedule_update_top_view(attr);                 
				 if(attr->total > 0)//无节目列表时不创建listbar
				 {
        		    schedule_list_listbar_init(msg);
				 }
                 
                 else
                 {
	            	for(i=0; i< schedule_uipara->listbar_number; i++)
	            	{
	            		GUI_BMP_Draw(theme_hdl2buf(attr->h_item_unfocus_bmp),schedule_uipara->listbar.x,schedule_uipara->listbar.y + schedule_uipara->listbar.height *(i));
	            	}					 
                 }
                     
			}
		}
		
	}
		
	case GUI_MSG_KEY:		
		return _schedule_list_listbar_key_proc(msg);
		
	case GUI_MSG_TOUCH:		
	{
	}		
	break;
				
	case GUI_MSG_COMMAND:
	{
	}		
	break;

	default:
		break;
	}
	
	return GUI_FrmWinDefaultProc(msg);	
}

H_WIN schedule_list_win_create(H_WIN h_parent, schedule_list_para_t *list_para)
{
	__gui_framewincreate_para_t framewin_para;
	schedule_list_para_t  *para;
	FB fb;

	__msg("*********enter  general schedule_list win proc *****************\n");
	GUI_LyrWinGetFB(list_para->layer, &fb);

	para = (schedule_list_para_t *)My_Balloc(sizeof(schedule_list_para_t));
	if(!para)
	{
		__msg("memory balloc fail.........\n");
		return NULL;
	}
	eLIBs_memset((void*)para, 0, sizeof(schedule_list_para_t));
	para->font = SWFFont;
	para->layer = list_para->layer;
	para->focus_id = list_para->focus_id;
	eLIBs_memset(&framewin_para, 0, sizeof(__gui_framewincreate_para_t));
	framewin_para.name =	SUBMENU_DVB_SCHUDULE_LST;
	framewin_para.dwExStyle = WS_EX_NONE;
	framewin_para.dwStyle = WS_NONE|WS_VISIBLE;
	framewin_para.spCaption =  NULL;
	framewin_para.hOwner	= NULL;
	framewin_para.id         = SUBMENU_DVB_SCHUDULE_LIST_ID;
	framewin_para.hHosting = h_parent;
	framewin_para.FrameWinProc = (__pGUI_WIN_CB)esKRNL_GetCallBack((__pCBK_t)_schedule_list_Proc);
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


__s32 schedule_list_win_delete(H_WIN list_win)
{
	GUI_FrmWinDelete(list_win);
	return EPDK_OK;
}

