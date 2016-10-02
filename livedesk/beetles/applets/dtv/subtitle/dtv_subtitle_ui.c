/*
**********************************************************************************************************************
*                                                    ePDK
*                                    the Easy Portable/Player Develop Kits
*                                              LiveTouch Sub-System
*
*                                   (c) Copyright 2007-2009, Steven.ZGJ.China
*                                             All Rights Reserved
*
* Moudle  : 
* File    : 
*
* By      : hot.lee
* Version : 
* Date    : 
**********************************************************************************************************************
*/
#include "dtv_subtitle_ui.h"

#if((MAPLE_TV_STANDAR == MAPLE_TV_ISDB_T)||(MAPLE_TV_STANDAR == MAPLE_TV_DTMB)\
		|| (MAPLE_TV_STANDAR == MAPLE_TV_DVB_T)|| (MAPLE_TV_STANDAR == MAPLE_TV_ATSC))


typedef struct subtitle_ctrl_s
{
	H_LYR        layer;
}subtitle_ctrl_t;

typedef struct subtitle_uipara_s
{
	RECT    subtitle;
}__subtitle_uipara_t;

static __subtitle_uipara_t subtitle_480_272 =
{
	{0,0,480,272},
};

static __subtitle_uipara_t subtitle_800_480 =
{
	{0,0,800,480},
};

static __subtitle_uipara_t subtitle_1024_600 =
{
	{0,0,1024,600},
};

static H_LYR  g_layer = NULL;


__subtitle_uipara_t* subtitle_get_uipara(void)
{
	ES_FILE			*disp;
	__s32 			width;
	__s32 			height;
	
	/* get lcd size*/
	dsk_display_get_size(&width,&height);

	if ((width == 480)&&(height == 272))
		return &subtitle_480_272;
	else if((width == 800)&&(height == 480))
		return &subtitle_800_480;
    else if((width == 1024)&&(height == 600))
		return &subtitle_1024_600;
	else
        ;
}


static __s32 _subtitle_paint(CSubtitleDrawItem *pSub )
{
    __s32 i=0;
    CSubtitleDrawModeISDB* sub;
	__subtitle_uipara_t *uipara;
	GUI_RECT rect;
	char text[128];

    sub = (CSubtitleDrawModeISDB *)&pSub->unionDrawMode.isdb;
	uipara = subtitle_get_uipara();

	if(NULL == g_layer)
	{
		__wrn("NULL == g_layer\n");
		return EPDK_FAIL;
	}
	
	GUI_LyrWinSetSta(g_layer,GUI_LYRWIN_STA_ON);
	GUI_LyrWinSel(g_layer);
    
    if(GUI_LYRWIN_STA_ON != GUI_LyrWinGetSta(g_layer) && GUI_LYRWIN_STA_SLEEP != GUI_LyrWinGetSta(g_layer))
	{
		  __wrn("------------------------Err Frame Buf fail.\n");            
		  return EPDK_FAIL;
	}

	GUI_LyrWinSetTop(g_layer);
	GUI_SetDrawMode(GUI_DRAWMODE_TRANS);
	
	GUI_SetFont(SWFFont);
	GUI_UC_SetEncodeUTF8();
	
	GUI_SetColor(GUI_WHITE);
	GUI_SetBkColor(0xDB000000);

	rect.x0 = uipara->subtitle.x;
	rect.y0 = uipara->subtitle.y-uipara->subtitle.y;
	rect.x1 = rect.x0+uipara->subtitle.width;
	rect.y1 = rect.y0+uipara->subtitle.height;
	GUI_ClearRectEx(&rect);	
    
	for ( i=0; i<sub->subNum; i++)
	{
		//此处y坐标变成了相对图层的坐标，暂时不明原因
		rect.x0 = uipara->subtitle.x;
		rect.y0 = uipara->subtitle.y + i*uipara->subtitle.height/sub->subNum-uipara->subtitle.y;
		rect.x1 = rect.x0+uipara->subtitle.width;
		rect.y1 = rect.y0+uipara->subtitle.height/sub->subNum-1;

		eDbug("x0=%d y0=%d x1=%d y1=%d\n",rect.x0,rect.y0,rect.x1,rect.y1);
		eLIBs_strcpy(text,pSub->unionDrawMode.isdb.subData[i]);

		GUI_DispStringInRect(text, &rect, GUI_TA_VCENTER|GUI_TA_HCENTER);	


		eDbug("text = %s \n",text);
	}
	return EPDK_OK;
}

static __s32 _subtitle_clean(CSubtitleDrawItem *pSub )
{
    //unsigned int i;
    GUI_RECT rect;
	__subtitle_uipara_t *uipara;

    //CSubtitleDrawModeISDB *sub = (CSubtitleDrawModeISDB *)&pSub->unionDrawMode.isdb;
	if(NULL == g_layer)
	{
		return EPDK_FAIL;
	}
    GUI_LyrWinSel(g_layer);
    
    if(GUI_LYRWIN_STA_ON != GUI_LyrWinGetSta(g_layer) && GUI_LYRWIN_STA_SLEEP != GUI_LyrWinGetSta(g_layer))
	{
		  __wrn("------------------------Err Frame Buf fail.\n");            
		  return EPDK_FAIL;
	}
	GUI_SetBkColor(0xDB000000);

	uipara = subtitle_get_uipara();
	
	rect.x0 = uipara->subtitle.x;
	rect.y0 = uipara->subtitle.y-uipara->subtitle.y;
	rect.x1 = rect.x0+uipara->subtitle.width;
	rect.y1 = rect.y0+uipara->subtitle.height;
	GUI_ClearRectEx(&rect);		

	GUI_LyrWinSetBottom(g_layer);

	return EPDK_OK;
}


 __s32 dtv_subtitle_paint(void *arg)
{
    CSubtitleDrawItem *pSub = NULL;
    CSubtitleDrawModeISDB *sub = NULL;


    pSub = (CSubtitleDrawItem*)arg;
    sub = (CSubtitleDrawModeISDB *)&pSub->unionDrawMode.isdb;

	eDbug("sub->Clear_screen_flags = %d\n",sub->Clear_screen_flags);
    if(pSub->nDrawMode != SUBTITLE_DRAWMODE_ISDB)
    {
        if ( sub->Clear_screen_flags == 0 )
        {    	
            return _subtitle_paint( pSub );
        }
        else
        {
            return _subtitle_clean( pSub );
        }	
    }
}

static __s32 subtitle_frmwin_proc(__gui_msg_t *msg)
{
	switch(msg->id)
	{
		case GUI_MSG_CREATE:
		{
            subtitle_ctrl_t* sub_ctrl = NULL;
			RECT rect;

            sub_ctrl = esMEMS_Malloc(0,sizeof(subtitle_ctrl_t));
            if(NULL == sub_ctrl)
            {
                __wrn("mem not enough...\n");
                return EPDK_FAIL;
            }			
            eLIBs_memset(sub_ctrl, 0, sizeof(subtitle_ctrl_t));

			sub_ctrl = (subtitle_ctrl_t *)GUI_WinGetAttr(msg->h_deswin);
			if(NULL == sub_ctrl)
			{
				return EPDK_FAIL;
			}
			
			if(NULL == sub_ctrl->layer)
			{
				return EPDK_FAIL;
			}
			g_layer = sub_ctrl->layer;

			GUI_LyrWinSel(sub_ctrl->layer);             
			GUI_LyrWinSetTop(sub_ctrl->layer);
		
            GUI_WinSetAddData(msg->h_deswin, (__u32)sub_ctrl); 
            
			return EPDK_OK;
		}
		break;

		case GUI_MSG_CLOSE:
			return EPDK_OK;
			
		case GUI_MSG_DESTROY:		
		{
			subtitle_ctrl_t* sub_ctrl = NULL;

			sub_ctrl = (subtitle_ctrl_t *)GUI_WinGetAddData(msg->h_deswin);
			if(NULL == sub_ctrl)
			{
				return EPDK_FAIL;
			}
			
			
			if(NULL != sub_ctrl)
			{
				esMEMS_Mfree(0,sub_ctrl);
				sub_ctrl = NULL;
			}
			
			return EPDK_OK;
		}
		
		case GUI_MSG_PAINT:
			return EPDK_OK;
		case GUI_MSG_KEY:
			return EPDK_OK;
		case GUI_MSG_TOUCH:
			return EPDK_OK;
		default:
			break;
	}
	
	return GUI_FrmWinDefaultProc(msg);	
}

/*
	建立窗口
*/
H_WIN subtitle_create(H_WIN h_parent,H_LYR layer)
{
	__gui_framewincreate_para_t framewin_para;
	subtitle_ctrl_t* sub_ctrl = NULL;

	FB fb;

	if(layer == NULL)
	{
		return NULL;
	}
	GUI_LyrWinGetFB(layer, &fb);


	sub_ctrl = (subtitle_ctrl_t *)My_Balloc(sizeof(subtitle_ctrl_t));
	if(!sub_ctrl)
	{
		__msg("memory balloc fail.........\n");
		return EPDK_FAIL;
	}
	eLIBs_memset((void*)sub_ctrl, 0, sizeof(subtitle_ctrl_t));
	sub_ctrl->layer = layer;

	eLIBs_memset(&framewin_para, 0, sizeof(__gui_framewincreate_para_t));

    
	framewin_para.name 			= SUBTITLE_FRMWIN;
	framewin_para.dwExStyle 	= WS_EX_NONE;
	framewin_para.dwStyle 		= WS_NONE|WS_VISIBLE;
	framewin_para.spCaption 	= NULL;
	framewin_para.hOwner		= NULL;
	framewin_para.hHosting 		= h_parent;
	framewin_para.id        	= SUBTITLE_FRMWIN_ID;
	framewin_para.FrameWinProc 	= (__pGUI_WIN_CB)esKRNL_GetCallBack((__pCBK_t)subtitle_frmwin_proc);
	framewin_para.rect.x 		= 0;
	framewin_para.rect.y 		= 0;
	framewin_para.rect.width	= fb.size.width;
	framewin_para.rect.height 	= fb.size.height;
	framewin_para.BkColor.alpha =  0;
	framewin_para.BkColor.red 	= 0;
	framewin_para.BkColor.green = 0;
	framewin_para.BkColor.blue 	= 0;
	framewin_para.attr 			=  sub_ctrl;
	framewin_para.hLayer 		= layer;

	return (GUI_FrmWinCreate(&framewin_para));
}


/**********************************************************************************************************************
* 图层创建接口
**********************************************************************************************************************/
 H_WIN dvb_subtitle_layer_create(void)
{
	__subtitle_uipara_t * subtitle_uipara;

	FB	fb = {
    	{0, 0},                                   		
    	{0, 0, 0},
    	{FB_TYPE_RGB, {PIXEL_COLOR_ARGB8888, 0, 0}}, 
	};
	__disp_layer_para_t para =  
	{
    	//DISP_LAYER_WORK_MODE_PALETTE,  
		DISP_LAYER_WORK_MODE_NORMAL,
    	0, 
    	1, 
    	0x7fff, 
    	
    	1, 
    	1,
    	{0, 0, 0, 0},
    	{0, 0, 0, 0},
    	DISP_LAYER_OUTPUT_CHN_DE_CH1,
    	NULL
	};
	
	__layerwincreate_para_t create_info = 
	{
		"dvb subtitle layer",
		NULL,
		GUI_LYRWIN_STA_SUSPEND,
		GUI_LYRWIN_NORMAL
	};

	subtitle_uipara = subtitle_get_uipara();
	
	fb.size.width  = subtitle_uipara->subtitle.width;
	fb.size.height = subtitle_uipara->subtitle.height;

	//para.pipe			= 1;
	
	para.scn_win.x 		= subtitle_uipara->subtitle.x;
	para.scn_win.y 		= subtitle_uipara->subtitle.y;
	para.scn_win.width 	= subtitle_uipara->subtitle.width;	
	para.scn_win.height = subtitle_uipara->subtitle.height;	

	para.src_win.x 		= 0;
	para.src_win.y 		= 0;
	para.src_win.width 	= subtitle_uipara->subtitle.width;	
	para.src_win.height = subtitle_uipara->subtitle.height;	

	para.fb = &fb;
	create_info.lyrpara = &para;	
	return(GUI_LyrWinCreate(&create_info));
}

#endif
