#ifndef __GUI_STATIC_C__
#define __GUI_STATIC_C__

#include "static_i.h"

static __u32  last_key = 0xffffffff;

static int StaticCtrlProc (__gui_msg_t  *msg);
      
/*
**********************************************************************************************************************
*                                               RegisterstaticControl
*
* author:           
*
* date:             2009-8-4
*
* Description:      registerstaticcontrol 
*
* parameters:       
*
* return:           if success return ERDK_OK
*                   if fail return the number of fail
* modify history: 
**********************************************************************************************************************
*/

__bool RegisterStaticControl (void)
{
    __gui_winclass_t        WndClass;

    WndClass.spClassName    = CTRL_STATIC;
    WndClass.dwStyle        = WS_NONE;
    WndClass.dwExStyle      = WS_EX_NONE;
    WndClass.WinProc        = StaticCtrlProc;

    return GUI_AddNewControlClass (&WndClass) == ORANGE_OK;
}
      
/*
**********************************************************************************************************************
*                                               staticCtrlProc
*
* author:           
*
* date:             2009-8-4
*
* Description:      staticctrlproc 
*
* parameters:       
*
* return:           if success return ERDK_OK
*                   if fail return the number of fail
* modify history: 
**********************************************************************************************************************
*/

static int StaticCtrlProc (__gui_msg_t *msg)
{
    __u32	            	dwStyle;
    static_data_t*			pData;

    dwStyle = GUI_WinGetStyle(msg->h_deswin);

	
    switch(msg->id)
    {
        case GUI_MSG_CREATE:
        {
            pData = (static_data_t*) orange_malloc(sizeof(static_data_t));
            if (pData == NULL) 
            {
                return ORANGE_FAIL;
            }

            pData->status       = 0;
            pData->hmem         = 0;
            pData->str_move     = 0;
            pData->draw_sts     = 0;
            pData->data         = (__u32)GUI_WinGetAttr(msg->h_deswin);
            GUI_CtrlWinSetAddData(msg->h_deswin,(__u32)pData);
       	}
        return 0;

       
        case GUI_MSG_DESTROY:
        {
            pData = (static_data_t*)GUI_CtrlWinGetAddData(msg->h_deswin);
            if(pData->str_move)
            {
                GUI_LongStringDelete(pData->str_move);
            }
            orange_mfree ((void *)GUI_CtrlWinGetAddData(msg->h_deswin));
        }
        return 0;

        
        case GUI_MSG_ENABLE:
        {
            if (msg->dwAddData1 && (dwStyle & WS_DISABLED))
            {
                dwStyle &= ~WS_DISABLED;

                GUI_WinSetStyle(msg->h_deswin,dwStyle);
            }
            else if (!msg->dwAddData1 && !(dwStyle & WS_DISABLED))
            {
                dwStyle |= WS_DISABLED;

                GUI_WinSetStyle(msg->h_deswin,dwStyle);
            }
            else
            {
                return 0;
            }
		}
        return 0;

            
        case GUI_MSG_SET_UNFOCUS:
        {
            if((STATIC_DRAWSTATUS(msg->h_deswin) & STATICST_FOCUS))
            {
                STATIC_DRAWSTATUS(msg->h_deswin) &= (~STATICST_FOCUS);
                
    			GUI_InvalidateRect (msg->h_deswin, NULL, ORANGE_TRUE);
            }
		}
        return 0;


        case GUI_MSG_SET_FOCUS:
        {
            if(!(STATIC_DRAWSTATUS(msg->h_deswin) & STATICST_FOCUS))
            {
                STATIC_DRAWSTATUS(msg->h_deswin) |= STATICST_FOCUS;

    			GUI_InvalidateRect (msg->h_deswin, NULL, ORANGE_TRUE);
            }
		}
        return 0;

        
        case GUI_MSG_KEY:
        {		
			statics_attribute_t          *statics_attribute;
			
			statics_action_t             statics_action;


			statics_attribute = (statics_attribute_t *)GUI_WinGetAttr(msg->h_deswin);
			
			if(NULL == statics_attribute)
			{
				eDbug("statics_attribute is NULL \n");
                return 0;
				
			}
			
			statics_action = statics_attribute->statics_action;


			switch(msg->dwAddData1)
			{                               
				case GUI_MSG_KEY_ENTER:
				{
					if(msg->dwAddData2 == KEY_UP_ACTION)
					{
				        __gui_msg_t            notifymsg;

						if(NULL == statics_action.enter_action)
						{
							eDbug("statics_action.enter_action is NULL \n");
 			                return 0;
						}
						
				        notifymsg.h_srcwin     = (__u32)msg->h_deswin;
				        notifymsg.h_deswin     = GUI_WinGetParent(msg->h_deswin);
				        notifymsg.id           = GUI_WinGetItemId(msg->h_deswin);
				        notifymsg.dwAddData1   = 0;
				        notifymsg.dwAddData2   = 0;
				        notifymsg.dwReserved   = 0;
				        notifymsg.p_arg        = NULL;

						statics_action.enter_action(&notifymsg);
					}

		            
				}
                return 0;

                
				case GUI_MSG_KEY_RIGHT:
				{
					if(msg->dwAddData2 == KEY_UP_ACTION)
					{
				        __gui_msg_t            notifymsg;

						if(NULL == statics_action.right_action)
						{
							eDbug("statics_action.right_action is NULL \n");
                			return 0;
						}
						
				        notifymsg.h_srcwin     = (__u32)msg->h_deswin;
				        notifymsg.h_deswin     = GUI_WinGetParent(msg->h_deswin);
				        notifymsg.id           = GUI_WinGetItemId(msg->h_deswin);
				        notifymsg.dwAddData1   = 0;
				        notifymsg.dwAddData2   = 0;
				        notifymsg.dwReserved   = 0;
				        notifymsg.p_arg        = NULL;

						statics_action.right_action(&notifymsg);
					}					
                }
                return 0;

                
                case GUI_MSG_KEY_LEFT:
				{
					if(msg->dwAddData2 == KEY_UP_ACTION)
					{
				        __gui_msg_t            notifymsg;

						if(NULL == statics_action.left_action)
						{
							eDbug("statics_action.left_action is NULL \n");
			                return 0;
						}
						
				        notifymsg.h_srcwin     = (__u32)msg->h_deswin;
				        notifymsg.h_deswin     = GUI_WinGetParent(msg->h_deswin);
				        notifymsg.id           = GUI_WinGetItemId(msg->h_deswin);
				        notifymsg.dwAddData1   = 0;
				        notifymsg.dwAddData2   = 0;
				        notifymsg.dwReserved   = 0;
				        notifymsg.p_arg        = NULL;

						statics_action.left_action(&notifymsg);
					}

                }
                return 0;


                case GUI_MSG_KEY_ESCAPE:
				{
					if(msg->dwAddData2 == KEY_UP_ACTION)
					{
				        __gui_msg_t            notifymsg;

						if(NULL == statics_action.return_action)
						{
							eDbug("statics_action.return_action is NULL \n");
			                return 0;
						}
						
				        notifymsg.h_srcwin     = (__u32)msg->h_deswin;
				        notifymsg.h_deswin     = GUI_WinGetParent(msg->h_deswin);
				        notifymsg.id           = GUI_WinGetItemId(msg->h_deswin);
				        notifymsg.dwAddData1   = 0;
				        notifymsg.dwAddData2   = 0;
				        notifymsg.dwReserved   = 0;
				        notifymsg.p_arg        = NULL;

						statics_action.return_action(&notifymsg);
					}

                }
                return 0;

               
				default:                
				break;
					
			}

        }    
        return 0;
        
        case GUI_MSG_TOUCH:
		{
			switch(msg->dwAddData1)
			{
				case GUI_MSG_TOUCH_DOWN:
				{
					{
						__gui_notify_msg_t      notify_msg;

		                notify_msg.hWnd     = msg->h_deswin;
		                notify_msg.id       = GUI_WinGetItemId(msg->h_deswin);
		                notify_msg.msgcode  = STN_PUSHED;
		                notify_msg.dwAddData= 0;
		                    
		                GUI_NotifyParent (&notify_msg);
					}		            
				}
				break;
				
				case GUI_MSG_TOUCH_UP:
				{
					{
						__gui_notify_msg_t      notify_msg;

		                notify_msg.hWnd     = msg->h_deswin;
		                notify_msg.id       = GUI_WinGetItemId(msg->h_deswin);
		                notify_msg.msgcode  = STN_CLICKED;
		                notify_msg.dwAddData= 0;
		                    
		                GUI_NotifyParent (&notify_msg);
			         }
				}
				break;	
			}
		}
            
        return 0;

        case GUI_MSG_WIN_WAKEUP:
        case GUI_MSG_PAINT:
        {
			__gui_rect_t 	        rcrect;
			
            RECT                    fbrect;
            
            static_data_t           *static_data;
            
			statics_attribute_t  	*statics_attribute;

			statics_attribute = NULL;

			static_data       = NULL;

			
			if(dwStyle & WS_DISABLED)
			{
				eDbug("ctrlwin is disable \n");
				
                GUI_WinSetStyle(msg->h_deswin,dwStyle);
                
				return 0;
			}
			
			statics_attribute = (statics_attribute_t *)(((static_data_t *)GUI_CtrlWinGetAddData(msg->h_deswin))->data);

            static_data = (static_data_t *)GUI_CtrlWinGetAddData(msg->h_deswin);


            GUI_LyrWinSel(GUI_LyrP2H(GUI_WinGetLyrWin(msg->h_deswin)));
            
            GUI_WinGetClientFBRect(msg->h_deswin,&fbrect);
            
            if(!static_data->hmem && (GUI_WinGetStyle(msg->h_deswin) & WS_MEMDEV))
            {
            	#if GUI_CORE_MEMDEV_EN		//avoid complier error, by Derek
            	{
                	static_data->hmem = GUI_MEMDEV_Create(fbrect.x,fbrect.y,fbrect.width,fbrect.height);

					eDbug("create memdev \n");
				}
				#endif
            }
            
			SysRectToGuiRect(&fbrect,&rcrect);

			if(static_data->hmem)
			{
				#if GUI_CORE_MEMDEV_EN		//avoid complier error, by Derek
            	GUI_MEMDEV_Select(static_data->hmem);           // start memory device
            	#endif
        	}


        	if (STATIC_DRAWSTATUS(msg->h_deswin) & STATICST_FOCUS)
            {	
                GUI_RECT        rect;


               
                GUI_SetBkColor(statics_attribute->statics_paint.bk_color);
                
            	GUI_ClearRect(rcrect.left,rcrect.top,rcrect.right,rcrect.bottom);

                if(statics_attribute->statics_paint.focus_bmp)
                {                	
                    GUI_BMP_Draw(statics_attribute->statics_paint.focus_bmp, rcrect.left + statics_attribute->statics_paint.bmp_pos.x, 
    					rcrect.top + statics_attribute->statics_paint.bmp_pos.y);
                }
                
				if(statics_attribute->statics_paint.alpha_en)
                {
                    GUI_OpenAlphaBlend();
                }
                
  				             
				GUI_CharSetToEncode(statics_attribute->statics_paint.draw_code);
				
				GUI_SetFont(statics_attribute->statics_paint.draw_font);

				GUI_SetDrawMode(GUI_DRAWMODE_TRANS);
				
			    GUI_SetColor(statics_attribute->statics_paint.ftxt_color);

                rect.x0 = rcrect.left;
                rect.y0 = rcrect.top;
                rect.x1 = rcrect.right;
                rect.y1 = rcrect.bottom;

				if(NULL == statics_attribute->statics_paint.text)
				{
					return EPDK_OK;
				}
				
                if(!statics_attribute->statics_paint.txt_align)
                {
                	if(statics_attribute->statics_paint.text[0])
                	{
	    			    GUI_DispStringAt(statics_attribute->statics_paint.text,
	    			                     rcrect.left + statics_attribute->statics_paint.text_pos.x, 
	    					             rcrect.top + statics_attribute->statics_paint.text_pos.y);
    				}
                }
                else
                {
                	if(statics_attribute->statics_paint.text[0])
                	{
	            		GUI_DispStringInRect(statics_attribute->statics_paint.text,&rect,statics_attribute->statics_paint.txt_align);
	            	}
                }

            }
            else
            {
                GUI_RECT        rect;


                GUI_SetBkColor(statics_attribute->statics_paint.bk_color);
				
            	GUI_ClearRect(rcrect.left,rcrect.top,rcrect.right,rcrect.bottom);

                if(statics_attribute->statics_paint.unfocus_bmp)
                {                 	
                   	GUI_BMP_Draw(statics_attribute->statics_paint.unfocus_bmp, rcrect.left + statics_attribute->statics_paint.bmp_pos.x, 
    					rcrect.top + statics_attribute->statics_paint.bmp_pos.y);
                }
				
				if(statics_attribute->statics_paint.alpha_en)
                {
                    GUI_OpenAlphaBlend();
                }

                
				GUI_CharSetToEncode(statics_attribute->statics_paint.draw_code);
				
				GUI_SetFont(statics_attribute->statics_paint.draw_font);

				GUI_SetDrawMode(GUI_DRAWMODE_TRANS);
				
			    GUI_SetColor(statics_attribute->statics_paint.uftxt_color);


                rect.x0 = rcrect.left;
                rect.y0 = rcrect.top;
                rect.x1 = rcrect.right;
                rect.y1 = rcrect.bottom;

				if(NULL == statics_attribute->statics_paint.text)
				{
					return EPDK_OK;
				}

			    if(!statics_attribute->statics_paint.txt_align)
                {
                	if(statics_attribute->statics_paint.text[0])
                	{
	    			    GUI_DispStringAt(statics_attribute->statics_paint.text,
	    			                     rcrect.left + statics_attribute->statics_paint.text_pos.x, 
	    					             rcrect.top + statics_attribute->statics_paint.text_pos.y);
    				}
                }
                else
                {
                	if(statics_attribute->statics_paint.text[0])
                	{
	            		GUI_DispStringInRect(statics_attribute->statics_paint.text,&rect,statics_attribute->statics_paint.txt_align);
	            	}
				}
          	
            }
            
          
            if(static_data->hmem)
            {
            	#if GUI_CORE_MEMDEV_EN		//avoid complier error, by Derek
	            GUI_MEMDEV_CopyToLCD( static_data->hmem );          // copy from memory device to fb
		        GUI_MEMDEV_Select( NULL );                    // stop memory device
		        GUI_MEMDEV_Delete(static_data->hmem);
				#endif
	            static_data->hmem = NULL;
        	}

            if(statics_attribute->statics_paint.alpha_en)
            {
                GUI_CloseAlphaBlend();
            }

            GUI_LyrWinFlushFB(GUI_WinGetLyrWin(msg->h_deswin));

        }
        return 0;

        default:
        break;
    }
    
    return GUI_CtrlWinDefaultProc (msg);
}
      
/*
**********************************************************************************************************************
*                                               GUI_StaticSetFocusBmp
*
* author:           
*
* date:             2009-9-4
*
* Description:      设置static的focus bmp 
*
* parameters:       
*
* return:           if success return ERDK_OK
*                   if fail return the number of fail
* modify history: 
**********************************************************************************************************************
*/

__s32  GUI_StaticSetFocusBmp(H_WIN hstatic,void *bmp)
{

	statics_attribute_t  	*statics_attribute;


    if((!hstatic) || (!bmp))
    {
        __wrn("input parameter error!\n");

        return ORANGE_FAIL;
    }

	statics_attribute = (statics_attribute_t *)(((static_data_t *)GUI_CtrlWinGetAddData(hstatic))->data);


    statics_attribute->statics_paint.focus_bmp = bmp;

    GUI_InvalidateRect (hstatic, NULL, ORANGE_TRUE);

    return ORANGE_OK;
}
      
/*
**********************************************************************************************************************
*                                               GUI_StaticSetUnFocusBmp
*
* author:           
*
* date:             2009-9-4
*
* Description:      设置static的un focus bmp 
*
* parameters:       
*
* return:           if success return ERDK_OK
*                   if fail return the number of fail
* modify history: 
**********************************************************************************************************************
*/

__s32  GUI_StaticSetUnFocusBmp(H_WIN hstatic,void *bmp)
{
	statics_attribute_t  	*statics_attribute;


    if((!hstatic) || (!bmp))
    {
        __wrn("input parameter error!\n");

        return ORANGE_FAIL;
    }

	statics_attribute = (statics_attribute_t *)(((static_data_t *)GUI_CtrlWinGetAddData(hstatic))->data);

    statics_attribute->statics_paint.unfocus_bmp = bmp;

    GUI_InvalidateRect (hstatic, NULL, ORANGE_TRUE);

    return ORANGE_OK;
}
      
/*
**********************************************************************************************************************
*                                               GUI_StaticSetText
*
* author:           
*
* date:             2009-9-4
*
* Description:      设置static的Text 
*
* parameters:       
*
* return:           if success return ERDK_OK
*                   if fail return the number of fail
* modify history: 
**********************************************************************************************************************
*/

__s32  GUI_StaticSetText(H_WIN hstatic,char *text)
{
	statics_attribute_t  	*statics_attribute;


    if((!hstatic) || (!text))
    {
        __wrn("input parameter error!\n");

        return ORANGE_FAIL;
    }

	statics_attribute = (statics_attribute_t *)(((static_data_t *)GUI_CtrlWinGetAddData(hstatic))->data);

    statics_attribute->statics_paint.text = text;

    GUI_InvalidateRect (hstatic, NULL, ORANGE_TRUE);

    return ORANGE_OK;
}
      
/*
**********************************************************************************************************************
*                                               GUI_StaticSetUnFcsTxtColor
*
* author:           
*
* date:             2009-9-4
*
* Description:      gui staticsetunfcstxtcolor 
*
* parameters:       
*
* return:           if success return ERDK_OK
*                   if fail return the number of fail
* modify history: 
**********************************************************************************************************************
*/

__s32  GUI_StaticSetUnFcsTxtColor(H_WIN hstatic,__u32 color)
{
	statics_attribute_t  	*statics_attribute;


    if(!hstatic)
    {
        __wrn("input parameter error!\n");

        return ORANGE_FAIL;
    }

	statics_attribute = (statics_attribute_t *)(((static_data_t *)GUI_CtrlWinGetAddData(hstatic))->data);

    statics_attribute->statics_paint.uftxt_color = color;

    GUI_InvalidateRect (hstatic, NULL, ORANGE_TRUE);

    return ORANGE_OK;
}
      
/*
**********************************************************************************************************************
*                                               GUI_StaticSetFcsTxtColor
*
* author:           
*
* date:             2009-9-4
*
* Description:      gui staticsetfcstxtcolor 
*
* parameters:       
*
* return:           if success return ERDK_OK
*                   if fail return the number of fail
* modify history: 
**********************************************************************************************************************
*/

__s32  GUI_StaticSetFcsTxtColor(H_WIN hstatic,__u32  color)
{
	static_data_t           *pData;
	
	statics_attribute_t  	*statics_attribute;
	
    RECT                    fbrect;
    
    __gui_rect_t 	        rcrect;

    if((!hstatic))
    {
        __wrn("input parameter error!\n");

        return ORANGE_FAIL;
    }
    
    pData                   = (static_data_t *)GUI_CtrlWinGetAddData(hstatic);
    
	statics_attribute       = (statics_attribute_t *)(((static_data_t *)GUI_CtrlWinGetAddData(hstatic))->data);

    statics_attribute->statics_paint.ftxt_color = color;
    
    if(pData->str_move)
    {
    	__show_info_move_t  str_move;
    	
    	GUI_WinGetClientFBRect(hstatic,&fbrect);
        SysRectToGuiRect(&fbrect,&rcrect);

        str_move.bkColor         = statics_attribute->statics_paint.bk_color;
        str_move.fontColor       = statics_attribute->statics_paint.ftxt_color;
        str_move.hlayer          = GUI_WinGetLyrWin(hstatic);
        str_move.string          = statics_attribute->statics_paint.text;
        str_move.region.x        = rcrect.left;
        str_move.region.y        = rcrect.top;
        str_move.region.width    = rcrect.right - rcrect.left + 1;
        str_move.region.height   = rcrect.bottom - rcrect.top + 1;
        str_move.encode_id       = statics_attribute->statics_paint.draw_code;
        str_move.pFont           = statics_attribute->statics_paint.draw_font;
        str_move.bmp             = NULL;
        str_move.align           = statics_attribute->statics_paint.txt_align;
        str_move.alpha_en        = statics_attribute->statics_paint.alpha_en;

        GUI_LongStringReset(pData->str_move,&str_move);
    }

    return ORANGE_OK;
}
      
/*
**********************************************************************************************************************
*                                               GUI_StaticSetBmpPos
*
* author:           
*
* date:             2009-9-4
*
* Description:      gui staticsetbmppos 
*
* parameters:       
*
* return:           if success return ERDK_OK
*                   if fail return the number of fail
* modify history: 
**********************************************************************************************************************
*/

__s32  GUI_StaticSetBmpPos(H_WIN hstatic,__pos_t *pos)
{
	statics_attribute_t  	*statics_attribute;


    if((!hstatic) || (!pos))
    {
        __wrn("input parameter error!\n");

        return ORANGE_FAIL;
    }

	statics_attribute = (statics_attribute_t *)(((static_data_t *)GUI_CtrlWinGetAddData(hstatic))->data);


    statics_attribute->statics_paint.bmp_pos.x = pos->x;
    
    statics_attribute->statics_paint.bmp_pos.y = pos->y;

    GUI_InvalidateRect (hstatic, NULL, ORANGE_TRUE);

    return ORANGE_OK;
}
      
/*
**********************************************************************************************************************
*                                               GUI_StaticSetTxtPos
*
* author:           
*
* date:             2009-9-4
*
* Description:      gui staticsettxtpos 
*
* parameters:       
*
* return:           if success return ERDK_OK
*                   if fail return the number of fail
* modify history: 
**********************************************************************************************************************
*/

__s32  GUI_StaticSetTxtPos(H_WIN hstatic,__pos_t *pos)
{
	statics_attribute_t  	*statics_attribute;


    if((!hstatic) || (!pos))
    {
        __wrn("input parameter error!\n");

        return ORANGE_FAIL;
    }

	statics_attribute = (statics_attribute_t *)(((static_data_t *)GUI_CtrlWinGetAddData(hstatic))->data);


    statics_attribute->statics_paint.text_pos.x = pos->x;

    
    statics_attribute->statics_paint.text_pos.y = pos->y;

   GUI_InvalidateRect (hstatic, NULL, ORANGE_TRUE);

   return ORANGE_OK;
}
      
/*
**********************************************************************************************************************
*                                               GUI_StaticStartMove
*
* author:           
*
* date:             2009-11-24
*
* Description:      gui staticstartmove 
*
* parameters:       
*
* return:           if success return ORANGE_OK
*                   if fail return the number of fail
* modify history: 
**********************************************************************************************************************
*/

__s32  GUI_StaticStartMove(H_WIN hstatic)
{
    static_data_t           *pData;
    
	statics_attribute_t  	*statics_attribute;
	
    RECT                    fbrect;
    
    __gui_rect_t 	        rcrect;
    
    __s32                   ret;
    
    if(!hstatic)
    {
        ORANGE_WRN("input parameter error!\n");

        return ORANGE_FAIL;
    }

    pData               = (static_data_t *)GUI_CtrlWinGetAddData(hstatic);
    
    statics_attribute   = (statics_attribute_t *)(pData->data);
    
    if(pData->str_move)
    {
    	__show_info_move_t  str_move;
    	
    	GUI_WinGetClientFBRect(hstatic,&fbrect);
        SysRectToGuiRect(&fbrect,&rcrect);

        str_move.bkColor         = statics_attribute->statics_paint.bk_color;
        str_move.fontColor       = statics_attribute->statics_paint.ftxt_color;
        str_move.hlayer          = GUI_WinGetLyrWin(hstatic);
        str_move.string          = statics_attribute->statics_paint.text;
        str_move.region.x        = rcrect.left;
        str_move.region.y        = rcrect.top;
        str_move.region.width    = rcrect.right - rcrect.left + 1;
        str_move.region.height   = rcrect.bottom - rcrect.top + 1;
        str_move.encode_id       = statics_attribute->statics_paint.draw_code;
        str_move.pFont           = statics_attribute->statics_paint.draw_font;
        str_move.align           = statics_attribute->statics_paint.txt_align;
		
        str_move.bmp             = statics_attribute->statics_paint.unfocus_bmp;
        str_move.bmp_pos.x		 = 0;
		str_move.bmp_pos.y		 = 0;
		
        str_move.alpha_en        = statics_attribute->statics_paint.alpha_en;
        GUI_LongStringReset(pData->str_move,&str_move);
    	
        GUI_LongStringStart(pData->str_move);
    }
    else
    {
        __show_info_move_t  str_move;

        GUI_WinGetClientFBRect(hstatic,&fbrect);
        SysRectToGuiRect(&fbrect,&rcrect);

        str_move.bkColor         = statics_attribute->statics_paint.bk_color;
        str_move.fontColor       = statics_attribute->statics_paint.ftxt_color;
        str_move.hlayer          = GUI_WinGetLyrWin(hstatic);
        str_move.string          = statics_attribute->statics_paint.text;
        str_move.region.x        = rcrect.left;
        str_move.region.y        = rcrect.top;
        str_move.region.width    = rcrect.right - rcrect.left + 1;
        str_move.region.height   = rcrect.bottom - rcrect.top + 1;
        str_move.encode_id       = statics_attribute->statics_paint.draw_code;
        str_move.pFont           = statics_attribute->statics_paint.draw_font;
        str_move.align           = statics_attribute->statics_paint.txt_align;
		
        str_move.bmp             = statics_attribute->statics_paint.unfocus_bmp;//NULL;modified by Derek,2011-1-21 15:30:14
        str_move.bmp_pos.x		 = 0;
		str_move.bmp_pos.y		 = 0;

        str_move.alpha_en        = statics_attribute->statics_paint.alpha_en;
        pData->str_move          = GUI_LongStringCreate(&str_move);
        
        if(!pData->str_move)
        {
            ORANGE_WRN("create move handle failed!\n");

            return ORANGE_FAIL;
        }

        ret                      = GUI_LongStringStart(pData->str_move);
        
        if(ret != ORANGE_OK)
        {
           ORANGE_WRN("start move handle failed!\n");

            return ORANGE_FAIL; 
        }
    }

    return ORANGE_OK;
}
      
/*
**********************************************************************************************************************
*                                               GUI_StaticStopMove
*
* author:           
*
* date:             2009-11-24
*
* Description:      gui staticstopmove 
*
* parameters:       
*
* return:           if success return ORANGE_OK
*                   if fail return the number of fail
* modify history: 
**********************************************************************************************************************
*/

__s32  GUI_StaticStopMove(H_WIN hstatic)
{
    static_data_t   *pData;
    
    if(!hstatic)
    {
        ORANGE_WRN("input parameter error!\n");

        return ORANGE_FAIL;
    }

    pData       = (static_data_t *)GUI_CtrlWinGetAddData(hstatic);
    if(pData->str_move)
    {
        GUI_LongStringStop(pData->str_move);
    }

    return ORANGE_OK;
}
      
/*
**********************************************************************************************************************
*                                               GUI_StaticSetCharSet
*
* author:           
*
* date:             2010-1-15
*
* Description:      gui staticsetcharset 
*
* parameters:       
*
* return:           if success return ORANGE_OK
*                   if fail return the number of fail
* modify history: 
**********************************************************************************************************************
*/

__s32  GUI_StaticSetCharSet(H_WIN hstatic,__epdk_charset_enm_e draw_code)
{
	statics_attribute_t  	*statics_attribute;


    if(!hstatic)
    {
        __wrn("input parameter error!\n");

        return ORANGE_FAIL;
    }

	statics_attribute = (statics_attribute_t *)(((static_data_t *)GUI_CtrlWinGetAddData(hstatic))->data);


    statics_attribute->statics_paint.draw_code = draw_code;

    GUI_InvalidateRect (hstatic, NULL, ORANGE_TRUE);

    return ORANGE_OK;
}
#endif
