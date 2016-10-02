/*
**************************************************************************************************************
*											         ePDK
*						            the Easy Portable/Player Develop Kits
*									           desktop system 
*
* File    	: 	Explorer.c,
*			Explorer.h
* By      	: 	Libaiao
* Func	: 
* Version	: 	v1.0
* ============================================================================================================
* 2010-4-14 15:24:52  Libaiao  create this file, implements the fundemental interface;
**************************************************************************************************************
*/

#include "digit.h"
#include "digit_ui.h"
#include "dtv_setting_i.h"


extern H_WIN g_dtv_mainwin;

/*
************************************************************************************************************************
*Function	:          static __s32 digit_send_cmd_to_parent(__gui_msg_t *msg,__u32 id,__u32 data1,__u32 data2)
*
*Description	:
*					
*Arguments  	: 		
*
*Return     	: 	
*
************************************************************************************************************************
*/

static __s32 digit_send_cmd_to_parent(__gui_msg_t *msg,__u32 id,__u32 data1,__u32 data2)
{
	__gui_msg_t dmsg;

	dmsg.id = GUI_MSG_COMMAND;
	dmsg.h_deswin = GUI_WinGetParent(msg->h_deswin);
	dmsg.h_srcwin = NULL;
	dmsg.dwAddData1 = MAKELONG(GUI_WinGetItemId(msg->h_deswin), id);
	dmsg.dwAddData2 = data1;
	dmsg.dwReserved = data2;

	GUI_SendNotifyMessage(&dmsg);

    return EPDK_OK;
}

/*
************************************************************************************************************************
*Function	:          static void digit_frmwin_res_init(digit_frmwin_attr_t *attr)
*
*Description	:
*					
*Arguments  	: 		
*
*Return     	: 	
*
************************************************************************************************************************
*/

static void digit_frmwin_res_init(digit_frmwin_attr_t *attr)
{
	__u8 i;
	digit_uipara_t *uipara = NULL;
    uipara = get_digit_uipara();
	attr->background_bmp = theme_open(uipara->background_bmp);	
	attr->no_number_bmp = theme_open(uipara->no_number_bmp);

	for(i = 0; i < DIGIT_COUNT; i++)
	{
		attr->digit_bmp[i] = theme_open(uipara->digit_bmp[i]);
	}

}


/*
************************************************************************************************************************
*Function	:          static void digit_frmwin_res_uninit(digit_frmwin_attr_t *attr)
*
*Description	:
*					
*Arguments  	: 		
*
*Return     	: 	
*
************************************************************************************************************************
*/

static void digit_frmwin_res_uninit(digit_frmwin_attr_t *attr)
{
    __u32 	i; 
         
	theme_close(attr->background_bmp);
		attr->background_bmp = 0;

	
	theme_close(attr->no_number_bmp);
		attr->no_number_bmp = 0;

	for(i = 0; i < DIGIT_COUNT; i++)
	{
		theme_close(attr->digit_bmp[i]);
		attr->digit_bmp[i] = 0;
	}	
}

/*
************************************************************************************************************************
*Function	:          static __s32 explorer_draw_digits_background(H_LYR hdigit_layer)
*
*Description	:		
*					
*Arguments  	: 		
*
*Return     	: 	
*
************************************************************************************************************************
*/

__s32 draw_digits_background(digit_frmwin_attr_t *attr)
{

	GUI_SetDrawMode(GUI_DRAWMODE_TRANS);
	
	GUI_BMP_Draw(dsk_theme_hdl2buf(attr->background_bmp), 0, 0);


	return EPDK_OK;
}


__s32 draw_title(digit_frmwin_attr_t *attr)
{
	RECT rect;
	char text[64] = {0};
	GUI_RECT gui_rect;
	digit_uipara_t *uipara = NULL;
	
    uipara = get_digit_uipara();

	GUI_SetFont(SWFFont);
	GUI_SetDrawMode(GUI_DRAWMODE_TRANS);
	GUI_UC_SetEncodeUTF8();
	
	get_menu_text(STRING_DTV_SEL_CH,text, sizeof(text));
		
	gui_rect.x0 = uipara->digit_title_rect.x;
	gui_rect.y0 = uipara->digit_title_rect.y;
	gui_rect.x1 = gui_rect.x0 + uipara->digit_title_rect.width;
	gui_rect.y1 = gui_rect.y0 + uipara->digit_title_rect.height;

	GUI_SetColor(GUI_WHITE);
	GUI_DispStringInRect(text, &gui_rect, GUI_TA_HCENTER|GUI_TA_VCENTER);


	return EPDK_OK;
}

/*
************************************************************************************************************************
*Function	:          static __s32 n2digits(__s32 num, __s32 * digits)
*
*Description	:		

*Arguments  	: 		
*
*Return     	: 	
*
************************************************************************************************************************
*/

static __s32 n2digits(__s32 num, __s32 * digits)
{
	__s32 i=0;

	do
	{
		digits[i]=num%10;
		num=num/10;
		i++;
	}
	while(num!=0);
	
	return i;
}



/*
************************************************************************************************************************
*Function	:          static __s32 draw_number(explr_digit_para_t *digit_attr)
*
*Description	:		
*					
*Arguments  	: 		
*
*Return     	: 	
*
************************************************************************************************************************
*/

__s32 draw_number(digit_frmwin_attr_t *attr)
{   
    __s32 i;
	GUI_RECT gui_rect;
	__s32 digits[16]={0};
	__s32	 ret;	
    __s32 save_back_color;
	digit_uipara_t *uipara = NULL;
	
    uipara = get_digit_uipara();
	
	gui_rect.x0 = uipara->digit_number_rect.x;
	gui_rect.y0 = uipara->digit_number_rect.y;
	gui_rect.x1 = gui_rect.x0 + uipara->digit_number_rect.width;
	gui_rect.y1 = gui_rect.y0 + uipara->digit_number_rect.height;

	GUI_SetColor(background_color);
    GUI_FillRect(gui_rect.x0,gui_rect.y0,gui_rect.x1,gui_rect.y1);
 	GUI_SetColor(GUI_WHITE);
   
	ret = n2digits(attr->ndigit_index, digits);

    {
    	__s32 x, y;
		HBMP hbmp;
        __s32 digit_bmp_width;

		hbmp = bmp_open(dsk_theme_hdl2buf(attr->digit_bmp[0]));
		if(NULL == hbmp)
		{
			__wrn("bmp_open fail...\n");
			return EPDK_FAIL;
		}

        digit_bmp_width = bmp_get_height(hbmp);
        
    	x = gui_rect.x0 +(uipara->digit_number_rect.width - digit_bmp_width *ret)/2;
        y = gui_rect.y0;

    	for(i = 0 ; i < ret ; i++)
    	{			
    		GUI_BMP_Draw(dsk_theme_hdl2buf(attr->digit_bmp[digits[ret-1-i]]), x, y);			
    		x += digit_bmp_width;		
    	}
    }
	
    return EPDK_OK;	
}


/*
************************************************************************************************************************
*Function	:          __u32 digit_key_proc(__gui_msg_t *msg)
*
*Description	:		
*					
*Arguments  	: 		
*
*Return     	: 	
*
************************************************************************************************************************
*/

__u32 digit_key_proc(__gui_msg_t *msg)
{
	digit_frmwin_attr_t *digit_attr;


	digit_attr = (digit_frmwin_attr_t *)GUI_WinGetAddData(msg->h_deswin);
	if(!digit_attr)
	{
		__wrn("attr is null ......\n");
		return EPDK_FAIL;
	}
	
	if(KEY_DOWN_ACTION == msg->dwAddData2)
	{
		switch(msg->dwAddData1)
		{
			case GUI_MSG_KEY_NUM0:
			case GUI_MSG_KEY_NUM1:
			case GUI_MSG_KEY_NUM2:
			case GUI_MSG_KEY_NUM3:
			case GUI_MSG_KEY_NUM4:
			case GUI_MSG_KEY_NUM5:
			case GUI_MSG_KEY_NUM6:
			case GUI_MSG_KEY_NUM7:
			case GUI_MSG_KEY_NUM8:
			case GUI_MSG_KEY_NUM9:				
			{
			    __u8 cur_val = 0;
					
				cur_val = msg->dwAddData1 - GUI_MSG_KEY_NUM0;

				if(digit_attr->ndigit_index < 1000)
				{
					digit_attr->ndigit_index *= 10;
					digit_attr->ndigit_index += cur_val;
				}
				else
				{
					digit_attr->ndigit_index = cur_val;
				}

				draw_number(digit_attr);
				
                if(GUI_IsTimerInstalled(msg->h_deswin, digit_attr->digit_timmer_id))
                {
	                GUI_ResetTimer(msg->h_deswin, digit_attr->digit_timmer_id,digit_attr->digit_timmer_cnt, NULL);
                }

			}
			break;
			
			case GUI_MSG_KEY_ENTER:
			{
				__gui_msg_t dmsg={0};
			
				dmsg.id         = GUI_MSG_COMMAND;
				dmsg.h_deswin   = g_dtv_mainwin;
				dmsg.dwAddData1 = CMD_NUMBER_SELECT_CHANNAL;
				dmsg.dwAddData2 = digit_attr->ndigit_index;
				dmsg.dwReserved = 0;
			
				GUI_SendNotifyMessage(&dmsg);
			}
			break;

			case GUI_MSG_KEY_ESCAPE:
			{
				__gui_msg_t dmsg={0};
			
				dmsg.id         = GUI_MSG_COMMAND;
				dmsg.h_deswin   = g_dtv_mainwin;
				dmsg.dwAddData1 = DTV_BACK2PLAY_PROGRAM;
				dmsg.dwAddData2 = 0;
				dmsg.dwReserved = 0;
			
				GUI_SendNotifyMessage(&dmsg);
			}
			break;
			
		}
	}
}

__u32 digit_touch_proc(__gui_msg_t *msg)
{

}

/*
************************************************************************************************************************
*Function	:          static __s32 digit_Proc(__gui_msg_t *msg)
*
*Description	:		
*					
*Arguments  	: 		
*
*Return     	: 	
*
************************************************************************************************************************
*/

static __s32 digit_Proc(__gui_msg_t *msg)
{
	switch(msg->id)
	{
	    case GUI_MSG_CREATE:
		{
            digit_frmwin_para_t *digit_para = NULL;
			digit_frmwin_attr_t *digit_attr = NULL;
			
			eDbug(" \n create digit frmwin \n");

			digit_para = (digit_frmwin_para_t *)GUI_WinGetAttr(msg->h_deswin);
			digit_attr = (digit_frmwin_attr_t *)My_Balloc(sizeof(digit_frmwin_attr_t));

			if(!digit_attr)
			{
				__wrn("attr is null ......\n");
				return EPDK_FAIL;
			}
			eLIBs_memset(digit_attr, 0, sizeof(digit_frmwin_attr_t));
			digit_attr->digit_para = digit_para;
			digit_attr->digit_layer = digit_para->digit_layer;
            digit_attr->ndigit_index = 0;
 			digit_attr->digit_timmer_id = 0x20;
			digit_attr->digit_timmer_cnt = 300;          
			digit_frmwin_res_init(digit_attr);
	        GUI_WinSetAddData(msg->h_deswin, (__u32)digit_attr);	        
		

			GUI_LyrWinSel(digit_attr->digit_layer);						
			GUI_LyrWinSetSta(digit_attr->digit_layer, GUI_LYRWIN_STA_ON);
			GUI_LyrWinSetTop(digit_attr->digit_layer);
			
			if(!GUI_IsTimerInstalled(msg->h_deswin, digit_attr->digit_timmer_id))
			{
				GUI_SetTimer(msg->h_deswin, digit_attr->digit_timmer_id, digit_attr->digit_timmer_cnt, NULL);
			}
		}
		return EPDK_OK;
		
		case GUI_MSG_PAINT:
		{
			digit_frmwin_attr_t *digit_attr = NULL;
			digit_attr = (digit_frmwin_attr_t *)GUI_WinGetAddData(msg->h_deswin);
			if(!digit_attr)
			{
				__wrn("attr is null ......\n");
				return EPDK_FAIL;
			}
			if(GUI_LyrWinGetSta(digit_attr->digit_layer) != GUI_LYRWIN_STA_ON)
			{
				return EPDK_FAIL;
			}
			eDbug(" \n paint digit frmwin \n");
			
			draw_digits_background(digit_attr);

			draw_title(digit_attr);	
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
            digit_frmwin_para_t *digit_para = NULL;
			digit_frmwin_attr_t *digit_attr = NULL;

			eDbug(" \n into delete digit frmwin \n");            

			digit_attr = (digit_frmwin_attr_t *)GUI_WinGetAddData(msg->h_deswin);
			if(!digit_attr)
			{
				__wrn("attr is null ......\n");
				return EPDK_FAIL;
			}

			digit_frmwin_res_uninit(digit_attr);

			if(GUI_IsTimerInstalled(msg->h_deswin, digit_attr->digit_timmer_id))
			{
				GUI_KillTimer(msg->h_deswin, digit_attr->digit_timmer_id);
			}

			digit_para = digit_attr->digit_para;
			if(digit_para)
			{
				My_Bfree(digit_para, sizeof(digit_frmwin_para_t));
				digit_para = NULL;
			}
			
			if(digit_attr)
			{
				My_Bfree(digit_attr, sizeof(digit_frmwin_attr_t));
				digit_attr = NULL;
			}
			
		}
		return EPDK_OK;

		case GUI_MSG_TIMER:
		{
			digit_frmwin_attr_t *digit_attr;
			

			digit_attr = (digit_frmwin_attr_t *)GUI_WinGetAddData(msg->h_deswin);
			if(!digit_attr)
			{
				__wrn("attr is null ......\n");
				return EPDK_FAIL;
			}
			
			if(digit_attr->digit_timmer_id == msg->dwAddData1)
			{
				__gui_msg_t dmsg={0};
			
				dmsg.id = GUI_MSG_COMMAND;
				dmsg.h_deswin = g_dtv_mainwin;
				dmsg.dwAddData1 = CMD_NUMBER_SELECT_CHANNAL;
				dmsg.dwAddData2 = digit_attr->ndigit_index;
				dmsg.dwReserved = 0;
			
				GUI_SendNotifyMessage(&dmsg);
			}
		}
		return EPDK_OK;

		case GUI_MSG_KEY:		
			return digit_key_proc(msg);
		
		case GUI_MSG_TOUCH:		
			return digit_touch_proc(msg);
				
    	case GUI_MSG_COMMAND:
    	{

    	}		
    		break;

    	default:
    		break;
	}
	
	return GUI_FrmWinDefaultProc(msg);	
}

/*
************************************************************************************************************************
*Function	:          H_WIN digit_win_create(H_WIN h_parent, digit_frmwin_attr_t *para)
*
*Description	:
*					
*Arguments  	: 		
*
*Return     	: 	
*
************************************************************************************************************************
*/

H_WIN digit_win_create(H_WIN h_parent, digit_frmwin_para_t *para)
{
	__gui_framewincreate_para_t framewin_para;
	digit_frmwin_para_t  *digit_para;
	FB fb;

	GUI_LyrWinGetFB(para->digit_layer, &fb);
	
	digit_para = (digit_frmwin_para_t *)My_Balloc(sizeof(digit_frmwin_para_t));
	if(!digit_para)
	{
		__msg("memory balloc fail.........\n");
		return EPDK_FAIL;
	}
	eLIBs_memset((void*)digit_para, 0, sizeof(digit_frmwin_para_t));
	digit_para->digit_layer = para->digit_layer;
	eLIBs_memset(&framewin_para, 0, sizeof(__gui_framewincreate_para_t));
	framewin_para.name =	"DIGIT";
	framewin_para.dwExStyle = WS_EX_NONE;
	framewin_para.dwStyle = WS_NONE|WS_VISIBLE;
	framewin_para.spCaption =  NULL;
	framewin_para.hOwner	= NULL;
	framewin_para.id         = DIGIT_ID;
	framewin_para.hHosting = h_parent;
	framewin_para.FrameWinProc = (__pGUI_WIN_CB)esKRNL_GetCallBack((__pCBK_t)digit_Proc);
	framewin_para.rect.x =0;
	framewin_para.rect.y = 0;
	framewin_para.rect.width= fb.size.width;
	framewin_para.rect.height = fb.size.height;
	framewin_para.BkColor.alpha =  0;
	framewin_para.BkColor.red = 0;
	framewin_para.BkColor.green = 0;
	framewin_para.BkColor.blue = 0;
	framewin_para.attr =  (void *)digit_para;
	framewin_para.hLayer = digit_para->digit_layer;

	return (GUI_FrmWinCreate(&framewin_para));
}

/*
************************************************************************************************************************
*Function	:          __s32 digit_win_delete(H_WIN list_win)
*
*Description	:		
*					
*Arguments  	: 		
*
*Return     	: 	
*
************************************************************************************************************************
*/

__s32 digit_win_delete(H_WIN list_win)
{
	GUI_FrmWinDelete(list_win);
	return EPDK_OK;
}



/*
************************************************************************************************************************
*Function	:          H_LYR explorer_digit_layer_create(void)
*
*Description	: 		前景图层，主要用来显示前景图片，ListBar组件item, 缩略图
*					    注意与背景图应处于不同的pipe
*					
*Arguments  	: 		
*
*Return     	: 	     图层句柄
*
************************************************************************************************************************
*/
H_LYR digit_layer_create(void)
{
	H_LYR layer = NULL;
	RECT LayerRect;
	FB  fb =
	{
	    {0, 0},                                   		/* size      */
	    {0, 0, 0},                                      /* buffer    */
	    {FB_TYPE_RGB, {PIXEL_COLOR_ARGB8888, 0, (__rgb_seq_t)0}},    /* fmt       */
	    
	};

	__disp_layer_para_t lstlyr =
	{
	    DISP_LAYER_WORK_MODE_NORMAL,
	    0,                                              /* ck_mode   */
	    0,                                              /* alpha_en  */
	    0,                                              /* alpha_val */
	    1,                                              /* pipe      */
	    0xff,                                           /* prio      */
	    {0, 0, 0, 0},                           		/* screen    */
	    {0, 0, 0, 0},                               	/* source    */
	    DISP_LAYER_OUTPUT_CHN_DE_CH1,                   /* channel   */
	   NULL                                            /* fb        */
    };	                                      
    __layerwincreate_para_t lyrcreate_info =  
    {                                         
        "APP_EXPLORER",                       
      	NULL,                                 
      	GUI_LYRWIN_STA_SUSPEND,               
      	GUI_LYRWIN_NORMAL                     
    };	
	
	get_digit_layer_rect(&LayerRect);
	
    fb.size.width		= LayerRect.width;            
    fb.size.height		= LayerRect.height;	        
    fb.fmt.fmt.rgb.pixelfmt = PIXEL_COLOR_ARGB8888; //PIXEL_MONO_8BPP; //PIXEL_COLOR_ARGB8888;
	
	lstlyr.src_win.x  		= 0;
	lstlyr.src_win.y  		= 0;
	lstlyr.src_win.width 	= LayerRect.width;
	lstlyr.src_win.height 	= LayerRect.height;
	
	lstlyr.scn_win.x		= LayerRect.x;
	lstlyr.scn_win.y		= LayerRect.y;
	lstlyr.scn_win.width  	= LayerRect.width;
	lstlyr.scn_win.height 	= LayerRect.height;
	
	lstlyr.pipe = 1;
	lstlyr.fb = &fb;
	lyrcreate_info.lyrpara = &lstlyr;
	
	layer = GUI_LyrWinCreate(&lyrcreate_info);
	
	if( !layer )
	{
		__err("app bar layer create error !\n");
	}
	return layer;	
}

