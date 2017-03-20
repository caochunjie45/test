/*
**************************************************************************************************************
*											         ePDK
*						            the Easy Portable/Player Develop Kits
*									           desktop system 
*
*						        	 (c) Copyright 2007-2010, ANDY, China
*											 All Rights Reserved
*
* File    	: dtv_setting_lcd_general.c
* By      	: hot.lee
* Func		: desk main thread
* Version	: v1.0
* ============================================================================================================
* 2009-7-20 8:51:52  hot.lee  create this file, implements the fundemental interface;
**************************************************************************************************************
*/


#include "dtv_setting_uipara.h"
#include "dtv_setting_lcd.h"


static __s32 content_backlight_level_id[][2] = //ITEM1 (��������)
{
	 {STRING_DTV_POWER_BGT_LEVEL1_CON,	    LION_BRIGHT_LEVEL1,},       //* ������ͼƬ��ʾ�������Ե�ǰ���õı���                  
	 {STRING_DTV_POWER_BGT_LEVEL2_CON,	    LION_BRIGHT_LEVEL2,    },           //* ��ͼƬԭʼ��С�ڴ�������ʾ�������������       
	 {STRING_DTV_POWER_BGT_LEVEL3_CON,	    LION_BRIGHT_LEVEL3,    },           //* ��ͼƬ����ı�����������������ʾ��ͼƬ������   
	 {STRING_DTV_POWER_BGT_LEVEL4_CON,	    LION_BRIGHT_LEVEL4,   },            //* �Դ��ڵı�������ͼƬ����������ʾ�����ܻ����   
	 {STRING_DTV_POWER_BGT_LEVEL5_CON,	    LION_BRIGHT_LEVEL5,    },           //* ǿ����4:3��ģʽ������ʾ��Ƶͼ��ͼ������    
	 {STRING_DTV_POWER_BGT_LEVEL6_CON,	    LION_BRIGHT_LEVEL6,    },           //* ǿ����16:9��ģʽ������ʾ��Ƶͼ��ͼ������   
	  {STRING_DTV_POWER_BGT_LEVEL7_CON,     LION_BRIGHT_LEVEL7,  },
      {STRING_DTV_POWER_BGT_LEVEL8_CON,	    LION_BRIGHT_LEVEL8,  },
#if BETTLES_BRIGHT_ON_TO_MAX      
       {STRING_DTV_POWER_BGT_LEVEL9_CON,	    LION_BRIGHT_LEVEL9,  },
      {STRING_DTV_POWER_BGT_LEVEL10_CON,	LION_BRIGHT_LEVEL10, },
      {STRING_DTV_POWER_BGT_LEVEL11_CON,	LION_BRIGHT_LEVEL11, },
      {STRING_DTV_POWER_BGT_LEVEL12_CON,	LION_BRIGHT_LEVEL12, },
      {STRING_DTV_POWER_BGT_LEVEL13_CON,	LION_BRIGHT_LEVEL13, },
      {STRING_DTV_POWER_BGT_LEVEL14_CON,	LION_BRIGHT_LEVEL14, },
      {STRING_DTV_POWER_BGT_LEVEL15_CON,	LION_BRIGHT_LEVEL15, },
#endif

}; 


typedef struct tag_dtv_setting_lcd_focus_item_id
{
	__s32  id_bmp_focus_item;
	__s32  id_bmp_unfocus_item;
}dtv_setting_lcd_focus_item_id_t;

typedef struct tag_dtv_setting_lcd_attr
{
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
	dtv_setting_lcd_para_t *para;

	__s32 total;
}dtv_setting_lcd_attr_t;

extern void  main_cmd2parent(H_WIN hwin, __s32 id, __u32 data1, __u32 data2);


static __s32 dtv_setting_lcd_item_paint_ex(__lbar_draw_para_t *draw_param);
static void _dtv_setting_lcd_res_init(dtv_setting_lcd_attr_t *attr);
static void _dtv_setting_lcd_res_uninit(dtv_setting_lcd_attr_t *attr) ;

static void dtv_setting_update_top_view(dtv_setting_lcd_attr_t *gattr)
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

static __s32 dtv_setting_lcd_item_paint_ex(__lbar_draw_para_t *draw_param)
{
	__s32 i;
	RECT rect;
   	GUI_RECT gui_rect;
	dtv_setting_lcd_attr_t *gattr;
	maple_schdl_event_t *shdl_event = NULL;
	setting_uipara_t *uipara;

	char time[128];
	char tmp[128];
	
	char shdl_name[128];
	
	
	gattr = (dtv_setting_lcd_attr_t *)draw_param->attr;

	if (gattr==NULL)
		return EPDK_FAIL;
		
	if (GUI_LyrWinGetSta(gattr->layer) != GUI_LYRWIN_STA_ON)
		return EPDK_FAIL;

	uipara = setting_get_uipara();
	
	GUI_SetFont(gattr->font);
	GUI_UC_SetEncodeUTF8();
	GUI_SetDrawMode(GUI_DRAWMODE_NORMAL);
	GUI_LyrWinCacheOn();

	switch(draw_param->mode)
	{
		case LBAR_MODE_NORMAL:
		{
			GUI_BMP_Draw(theme_hdl2buf(gattr->h_item_unfocus_bmp),draw_param->rect.x,draw_param->rect.y);

			GUI_SetColor(gattr->unfocus_txt_color);
			dsk_langres_get_menu_text(content_backlight_level_id[draw_param->index][0],shdl_name,sizeof(shdl_name));

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

            dsk_langres_get_menu_text(content_backlight_level_id[draw_param->index][0],shdl_name,sizeof(shdl_name));
			gui_rect.x0 = draw_param->rect.x;
			gui_rect.y0 = draw_param->rect.y;
			gui_rect.x1 = gui_rect.x0 + uipara->listbar.width;
			gui_rect.y1 = gui_rect.y0 + uipara->listbar.height;

			GUI_DispStringInRect(shdl_name, &gui_rect, GUI_TA_HCENTER|GUI_TA_VCENTER);   

		}
		break;
		default:
			break;
			
	}
	GUI_LyrWinCacheOff();
	return EPDK_OK;
}



static void dtv_setting_lcd_listbar_init(__gui_msg_t *msg)
{
	__listbar_config_t config; 
	RECT rect;
	dtv_setting_lcd_attr_t *attr;
	__s32 index;
	__s32 start_id,focus_id;
	reg_system_para_t	*system_para;
	setting_uipara_t *uipara;
	
	attr = (dtv_setting_lcd_attr_t *)GUI_WinGetAddData(msg->h_deswin);
		
	uipara = setting_get_uipara();

    system_para = dsk_reg_get_para_by_app(REG_APP_SYSTEM);
	{
        if(system_para->backlight>4)
            start_id = 3;
         else	
		   start_id = system_para->backlight - system_para->backlight%5;
		focus_id = system_para->backlight;
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
	LISTBAR_Config(attr->h_listbar, dtv_setting_lcd_item_paint_ex, &config);    
    LISTBAR_ShowPage(attr->h_listbar);              
	
}




/*
	������Դ
*/
static void _dtv_setting_lcd_res_init(dtv_setting_lcd_attr_t *attr)
{
	attr->h_item_focus_bmp = theme_open(ID_DTV_PRELIST_F_BMP);	
	attr->h_item_unfocus_bmp= theme_open(ID_DTV_PRELIST_F_BMP);		
	attr->h_list_top_bmp = theme_open(ID_DTV_SETTING_TOP_BMP);
	
}

static void _dtv_setting_lcd_res_uninit(dtv_setting_lcd_attr_t *attr)
{
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

static __s32 _dtv_setting_lcd_listbar_key_proc(__gui_msg_t *msg)
{    
    	__s32 ret = EPDK_OK;    
	dtv_setting_lcd_attr_t *attr;

	attr = (dtv_setting_lcd_attr_t *)GUI_WinGetAddData(msg->h_deswin);

    	switch(msg->dwAddData1)
       {
           case GUI_MSG_KEY_UP:
           case GUI_MSG_KEY_LONGUP:
           case GUI_MSG_KEY_LEFT:
           case GUI_MSG_KEY_LONGLEFT:		   	
           {
               if((msg->dwAddData2 == KEY_DOWN_ACTION) || (msg->dwAddData2 == KEY_REPEAT_ACTION))
               {
					reg_system_para_t	*system_para;
			   
       			    attr->old_focus = attr->new_focus;
                    LISTBAR_PrevItem(attr->h_listbar);
					attr->new_focus = LISTBAR_GetFocusItem(attr->h_listbar);

    				system_para = dsk_reg_get_para_by_app(REG_APP_SYSTEM);	
					system_para->backlight = content_backlight_level_id[attr->new_focus][1];
					dsk_display_set_lcd_bright(content_backlight_level_id[attr->new_focus][1]*2);
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
                   reg_system_para_t	*system_para;
				   
   				   attr->old_focus = attr->new_focus;
                   LISTBAR_NextItem(attr->h_listbar);	
				   attr->new_focus = LISTBAR_GetFocusItem(attr->h_listbar);

    				system_para = dsk_reg_get_para_by_app(REG_APP_SYSTEM);	
					system_para->backlight = content_backlight_level_id[attr->new_focus][1];					   
				   dsk_display_set_lcd_bright(content_backlight_level_id[attr->new_focus][1]*2);
          	   }
               break;
           }

           case GUI_MSG_KEY_ENTER:        
           {
               if((msg->dwAddData2 == KEY_UP_ACTION))
               {
				   dtv_setting_send_cmd_to_parent(msg,GUI_MSG_COMMAND,DTV_SETTING_BACK2LIST,0);
               }

               break;
           }

			case GUI_MSG_KEY_ESCAPE:
			case GUI_MSG_KEY_LONGESCAPE:
		   {
		   	    if((msg->dwAddData2 == KEY_UP_ACTION))
		   	    {
					dtv_setting_send_cmd_to_parent(msg,GUI_MSG_COMMAND,DTV_SETTING_BACK2LIST,0);
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

static __s32 _dtv_setting_lcd_listbar_touch_proc(__gui_msg_t *msg)
{
	__s32 x = 0, y = 0;  
	__s32 speed = 0, direct = 0;  
	__s32 index = 0;
	__s32 ret = EPDK_OK;        
	static __s32 is_touch_down = 0;
	static __s32 is_touch_moving = 0;
	dtv_setting_lcd_attr_t *attr;
	attr = (dtv_setting_lcd_attr_t *)GUI_WinGetAddData(msg->h_deswin);

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

static __s32  dtv_setting_lcd_touch_proc(__gui_msg_t *msg)
{
	return EPDK_OK;
}


/*
	�ص�
*/		
static __s32 _dtv_setting_lcd_Proc(__gui_msg_t *msg)
{
	switch(msg->id)
	{
		case GUI_MSG_CREATE:
		{
			dtv_setting_lcd_attr_t *attr;
			dtv_setting_lcd_para_t *para;
			HTHEME  bg_bmp;

			
			para = (dtv_setting_lcd_para_t *)GUI_WinGetAttr(msg->h_deswin);
			attr = (dtv_setting_lcd_attr_t *)My_Balloc(sizeof(dtv_setting_lcd_attr_t));
			if(!attr)
			{
				__msg("memory balloc fail.........\n");
				return EPDK_FAIL;
			}
			eLIBs_memset(attr, 0, sizeof(dtv_setting_lcd_attr_t));
			
			attr->new_focus = para->focus_id;
			attr->old_focus = para->focus_id;
			attr->para = para;

	
			_dtv_setting_lcd_res_init(attr);

			attr->focus_txt_color = dtv_SETTING_FOCUS_COLOR;
			attr->unfocus_txt_color = dtv_SETTING_UNFOCUS_COLOR;
			attr->font = para->font;
			attr->layer = para->layer;
			attr->total = sizeof(content_backlight_level_id)/sizeof(content_backlight_level_id[0]);
			__msg("attr->total = %d\n",attr->total);

			
			GUI_WinSetAddData(msg->h_deswin, (__u32)attr);
			GUI_LyrWinSetSta(attr->layer, GUI_LYRWIN_STA_ON);
			GUI_LyrWinSetTop(attr->layer);

			dtv_setting_update_top_view(attr);
			{
				dtv_setting_lcd_listbar_init(msg);
			}
	
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
			dtv_setting_lcd_attr_t *attr;
			dtv_setting_lcd_para_t *para;
			__s32 ret;
			attr = (dtv_setting_lcd_attr_t *)GUI_WinGetAddData(msg->h_deswin);

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

			_dtv_setting_lcd_res_uninit(attr);

			para = attr->para;
			if(para)
			{
				My_Bfree(para, sizeof(dtv_setting_lcd_para_t));
				para = NULL;
			}
			if(attr)
			{
				My_Bfree(attr, sizeof(dtv_setting_lcd_attr_t));
				attr = NULL;
			}
			
		}
		return EPDK_OK;

		case GUI_MSG_PAINT:		
			return EPDK_OK;

		case GUI_MSG_KEY:		
			return _dtv_setting_lcd_listbar_key_proc(msg);
		
		case GUI_MSG_TOUCH:		
			return dtv_setting_lcd_touch_proc(msg);////
				
		case GUI_MSG_COMMAND:
		{
		}		
		break;

		default:
			break;
	}
	
	return GUI_FrmWinDefaultProc(msg);	
}

H_WIN dtv_setting_lcd_win_create(H_WIN h_parent, dtv_setting_lcd_para_t *list_para)
{
	__gui_framewincreate_para_t framewin_para;
	dtv_setting_lcd_para_t  *para;
	FB fb;

	__msg("*********enter  general dtv_setting_lcd win proc *****************\n");
	GUI_LyrWinGetFB(list_para->layer, &fb);

	para = (dtv_setting_lcd_para_t *)My_Balloc(sizeof(dtv_setting_lcd_para_t));
	if(!para)
	{
		__msg("memory balloc fail.........\n");
		return NULL;
	}
	eLIBs_memset((void*)para, 0, sizeof(dtv_setting_lcd_para_t));
	para->font = SWFFont;
	para->layer = list_para->layer;
	para->focus_id = list_para->focus_id;
	eLIBs_memset(&framewin_para, 0, sizeof(__gui_framewincreate_para_t));
	framewin_para.name =	SUBMENU_DVB_SETTING_LCD;
	framewin_para.dwExStyle = WS_EX_NONE;
	framewin_para.dwStyle = WS_NONE|WS_VISIBLE;
	framewin_para.spCaption =  NULL;
	framewin_para.hOwner	= NULL;
	framewin_para.id         = SUBMENU_DVB_SETTING_LCD_ID;
	framewin_para.hHosting = h_parent;
	framewin_para.FrameWinProc = (__pGUI_WIN_CB)esKRNL_GetCallBack((__pCBK_t)_dtv_setting_lcd_Proc);
    __wrn("framewin_para.name:%s, framewin_para.FrameWinProc:0x%x\n", framewin_para.name, framewin_para.FrameWinProc);
    
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


__s32 dtv_setting_lcd_win_delete(H_WIN list_win)
{
	GUI_FrmWinDelete(list_win);
	return EPDK_OK;
}

