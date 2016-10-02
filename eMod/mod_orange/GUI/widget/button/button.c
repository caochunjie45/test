/*
*******************************************************************************************************************
*                                                         	Listmenu  Framework
*                                         			the display dirver support module
*
*                             				 Copyright(C), 2006-2008, Softwinner Microelectronic Co., Ltd.
*											               All Rights Reserved
*
*File Name： 	button.c
*
*Author： 		William Wang
*
*Version :   	1.1.0
*
*Date:  		2009-8-17
*
*Description : 	button control implement 
*
*Others : 		None at present.
*
* History :
*
* <Author>          <time>         <version>     <description>
*
* William Wang     2009-8-17         1.1.0          Create File
*
*******************************************************************************************************************
*/
#ifndef __GUI_BUTTON_C__
#define __GUI_BUTTON_C__

#include "button_i.h"

static __u32  last_key = 0xffffffff;

static int ButtonCtrlProc (__gui_msg_t  *msg);

static H_WIN  Win_Self = NULL;

/*
**********************************************************************************************************************
*                                               RegisterButtonControl
*
* author:           
*
* date:             2009-8-4
*
* Description:      registerbuttoncontrol 
*
* parameters:       
*
* return:           if success return ERDK_OK
*                   if fail return the number of fail
* modify history: 
**********************************************************************************************************************
*/

__bool RegisterButtonControl (void)
{
    __gui_winclass_t  WndClass;

    WndClass.spClassName = CTRL_BUTTON;
    WndClass.dwStyle     = WS_NONE;
    WndClass.dwExStyle   = WS_EX_NONE;
    WndClass.WinProc     = ButtonCtrlProc;
    
    return GUI_AddNewControlClass (&WndClass) == ORANGE_OK;
}
      
/*
**********************************************************************************************************************
*                                               ButtonCtrlProc
*
* author:           
*
* date:             2009-8-4
*
* Description:      buttonctrlproc 
*
* parameters:       
*
* return:           if success return ERDK_OK
*                   if fail return the number of fail
* modify history: 
**********************************************************************************************************************
*/

static int ButtonCtrlProc (__gui_msg_t *msg)
{
    __u32	            	dwStyle;
    
    button_data_t*			pData;


    dwStyle = GUI_WinGetStyle(msg->h_deswin);


    switch(msg->id)
    {
        case GUI_MSG_CREATE:
        {
            pData = (button_data_t*) orange_malloc(sizeof(button_data_t));
            if (pData == NULL) 
            {
                return ORANGE_FAIL;
            }

            pData->draw_sts     = 0;
            pData->status       = 0;
            pData->hmem         = 0;
            pData->draw_sts     = 0;
            pData->data         = (__u32)GUI_WinGetAttr(msg->h_deswin);
            GUI_CtrlWinSetAddData(msg->h_deswin,(__u32)pData);

            Win_Self = msg->h_deswin;
		}
        return 0;
       
        case GUI_MSG_DESTROY:
        {
            pData = (button_data_t*)GUI_CtrlWinGetAddData(msg->h_deswin);
            orange_mfree ((void *)GUI_CtrlWinGetAddData(msg->h_deswin));
		}
        return 0;

  
        case BM_CLICK:
        {
            __gui_notify_msg_t      notify_msg;

            notify_msg.hWnd         = msg->h_deswin;
            notify_msg.id           = GUI_WinGetItemId(msg->h_deswin);
            notify_msg.msgcode      = BN_CLICKED;
            notify_msg.dwAddData    = 0;
                
            GUI_NotifyParent(&notify_msg);
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


        case GUI_MSG_TOUCHINACTIVE:
        {      
            if(BUTTON_DRAWSTATUS(msg->h_deswin) & BST_FOCUS)
            {
                BUTTON_DRAWSTATUS(msg->h_deswin) &= (~BST_FOCUS);
                

    			GUI_InvalidateRect (msg->h_deswin, NULL, ORANGE_TRUE);
            }
        }
        return 0;

    
        case GUI_MSG_SET_UNFOCUS:
        {
            if (GUI_WinGetCaptureWin() == msg->h_deswin) 
            {
                GUI_WinReleaseCapture();
                
                BUTTON_STATUS(msg->h_deswin) &= (~BST_PUSHED);
            }

            {
                __gui_notify_msg_t      notify_msg;

                notify_msg.hWnd         = msg->h_deswin;
                notify_msg.id           = GUI_WinGetItemId(msg->h_deswin);
                notify_msg.msgcode      = BN_KILLFOCUS;
                notify_msg.dwAddData    = 0;
                    
                GUI_NotifyParent(&notify_msg);
            }
            
            if(BUTTON_DRAWSTATUS(msg->h_deswin) & BST_FOCUS)
            {
                BUTTON_DRAWSTATUS(msg->h_deswin) &= (~BST_FOCUS);
                
    			GUI_InvalidateRect (msg->h_deswin, NULL, ORANGE_TRUE);
            }
		}
        return 0;

        case GUI_MSG_SET_FOCUS:
        {
            {
                __gui_notify_msg_t      notify_msg;

                notify_msg.hWnd         = msg->h_deswin;
                notify_msg.id           = GUI_WinGetItemId(msg->h_deswin);
                notify_msg.msgcode      = BN_SETFOCUS;
                notify_msg.dwAddData    = 0;
                    
                GUI_NotifyParent(&notify_msg);
            }
            
            if(!(BUTTON_DRAWSTATUS(msg->h_deswin) & BST_FOCUS))
            {
                BUTTON_DRAWSTATUS(msg->h_deswin) |= BST_FOCUS;

    			GUI_InvalidateRect (msg->h_deswin, NULL, ORANGE_TRUE);
            }
		}
        return 0;
        
        case GUI_MSG_KEY:
        {
            button_attribute_t     *button_attribute;

			button_attribute = (button_attribute_t *)(((button_data_t *)GUI_CtrlWinGetAddData(msg->h_deswin))->data);
        
			switch(msg->dwAddData1)
			{                

 				case GUI_MSG_KEY_LEFT:
				{
					if(msg->dwAddData2 == KEY_UP_ACTION)
					{
						BUTTON_STATUS(msg->h_deswin) &= ~BST_PUSHED;
						{
							if(NULL == button_attribute->button_action.left_action)
							{
								break;
							}

							{
						        __gui_msg_t           notifymsg;
    
						        notifymsg.h_srcwin   = (__u32)msg->h_deswin;
						        notifymsg.h_deswin   = GUI_WinGetParent(msg->h_deswin);
						        notifymsg.id         = GUI_WinGetItemId(msg->h_deswin);
						        notifymsg.dwAddData1 = 0;
						        notifymsg.dwAddData2 = 0;
						        notifymsg.dwReserved = 0;
						        notifymsg.p_arg      = NULL;

								button_attribute->button_action.left_action(&notifymsg);
						        
							}
		                    
		            	}
					}

					if(msg->dwAddData2 == KEY_DOWN_ACTION)
					{
						BUTTON_STATUS(msg->h_deswin) |= BST_PUSHED;
					}
		            
				}		
                break;

                
 				case GUI_MSG_KEY_RIGHT:
				{
					if(msg->dwAddData2 == KEY_UP_ACTION)
					{
						BUTTON_STATUS(msg->h_deswin) &= ~BST_PUSHED;
						{
							if(NULL == button_attribute->button_action.right_action)
							{
								break;
							}

							{
						        __gui_msg_t           notifymsg;
    
						        notifymsg.h_srcwin   = (__u32)msg->h_deswin;
						        notifymsg.h_deswin   = GUI_WinGetParent(msg->h_deswin);
						        notifymsg.id         = GUI_WinGetItemId(msg->h_deswin);
						        notifymsg.dwAddData1 = 0;
						        notifymsg.dwAddData2 = 0;
						        notifymsg.dwReserved = 0;
						        notifymsg.p_arg      = NULL;

								button_attribute->button_action.right_action(&notifymsg);
						        
							}
		                    
		            	}
					}

					if(msg->dwAddData2 == KEY_DOWN_ACTION)
					{
						BUTTON_STATUS(msg->h_deswin) |= BST_PUSHED;
					}
		            
				}		
                break;


  				case GUI_MSG_KEY_ESCAPE:
				{
					if(msg->dwAddData2 == KEY_UP_ACTION)
					{
						BUTTON_STATUS(msg->h_deswin) &= ~BST_PUSHED;
						{
							if(NULL == button_attribute->button_action.return_action)
							{
								break;
							}

							{
						        __gui_msg_t            notifymsg;
    
						        notifymsg.h_srcwin     = (__u32)msg->h_deswin;
						        notifymsg.h_deswin     = GUI_WinGetParent(msg->h_deswin);
						        notifymsg.id           = GUI_WinGetItemId(msg->h_deswin);
						        notifymsg.dwAddData1   = 0;
						        notifymsg.dwAddData2   = 0;
						        notifymsg.dwReserved   = 0;
						        notifymsg.p_arg        = NULL;

								button_attribute->button_action.return_action(&notifymsg);
						        
							}
		                    
		            	}
					}

					if(msg->dwAddData2 == KEY_DOWN_ACTION)
					{
						BUTTON_STATUS(msg->h_deswin) |= BST_PUSHED;
					}
		            
				}		
                break;


  				case GUI_MSG_KEY_ENTER:
				{
					if(msg->dwAddData2 == KEY_UP_ACTION)
					{
						BUTTON_STATUS(msg->h_deswin) &= ~BST_PUSHED;
						{
							if(NULL == button_attribute->button_action.enter_action)
							{
								break;
							}

							{
						        __gui_msg_t            notifymsg;
    
						        notifymsg.h_srcwin     = (__u32)msg->h_deswin;
						        notifymsg.h_deswin     = GUI_WinGetParent(msg->h_deswin);
						        notifymsg.id           = GUI_WinGetItemId(msg->h_deswin);
						        notifymsg.dwAddData1   = 0;
						        notifymsg.dwAddData2   = 0;
						        notifymsg.dwReserved   = 0;
						        notifymsg.p_arg        = NULL;

								button_attribute->button_action.enter_action(&notifymsg);
						        
							}
		                    
		            	}
					}

					if(msg->dwAddData2 == KEY_DOWN_ACTION)
					{
						BUTTON_STATUS(msg->h_deswin) |= BST_PUSHED;
					}
		            
				}		
                break;

              
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
					if (GUI_WinGetCaptureWin () == msg->h_deswin)
					{
		                break;
					}
		            
		            GUI_WinSetCaptureWin(msg->h_deswin);
		                
		            BUTTON_STATUS(msg->h_deswin) |= BST_PUSHED;
		            if(!(BUTTON_DRAWSTATUS(msg->h_deswin) & BST_FOCUS))
                    {
                        BUTTON_DRAWSTATUS(msg->h_deswin) |= BST_FOCUS;

            			GUI_InvalidateRect (msg->h_deswin, NULL, ORANGE_TRUE);
                    }

					{
						__gui_notify_msg_t      notify_msg;

		                notify_msg.hWnd         = msg->h_deswin;
		                notify_msg.id           = GUI_WinGetItemId(msg->h_deswin);
		                notify_msg.msgcode      = BN_PUSHED;
		                notify_msg.dwAddData    = GUI_MSG_TOUCH_DOWN;
		                    
		                GUI_NotifyParent (&notify_msg);
		            }

                    			            
				}
				break;

                case GUI_MSG_TOUCH_LONGDOWN:
				{
                    __gui_notify_msg_t      notify_msg;
                    
		            BUTTON_STATUS(msg->h_deswin) |= BST_PUSHED;
		            if(!(BUTTON_DRAWSTATUS(msg->h_deswin) & BST_FOCUS))
                    {
                        BUTTON_DRAWSTATUS(msg->h_deswin) |= BST_FOCUS;

            			GUI_InvalidateRect (msg->h_deswin, NULL, ORANGE_TRUE);
                    }

					notify_msg.hWnd     = msg->h_deswin;
	                notify_msg.id       = GUI_WinGetItemId(msg->h_deswin);
	                notify_msg.msgcode  = BN_PUSHED;
	                notify_msg.dwAddData= GUI_MSG_TOUCH_LONGDOWN;
	                    
	                GUI_NotifyParent (&notify_msg);			            
				}
				break;
				
				case GUI_MSG_TOUCH_MOVE:
				{
                    __gui_notify_msg_t      notify_msg;
                    
		            if (GUI_WinGetCaptureWin() != msg->h_deswin)
		            {
		                break;
		            }

                    if ( !(BUTTON_STATUS(msg->h_deswin) & BST_PUSHED) ) 
					{
	                    BUTTON_STATUS(msg->h_deswin) |= BST_PUSHED;
	                    if(!(BUTTON_DRAWSTATUS(msg->h_deswin) & BST_FOCUS))
                        {
                            BUTTON_DRAWSTATUS(msg->h_deswin) |= BST_FOCUS;

                			GUI_InvalidateRect (msg->h_deswin, NULL, ORANGE_TRUE);
                        }
	                }
                    else 
					{
                        if(!(BUTTON_DRAWSTATUS(msg->h_deswin) & BST_FOCUS))
                        {
                            BUTTON_DRAWSTATUS(msg->h_deswin) |= BST_FOCUS;

                			GUI_InvalidateRect (msg->h_deswin, NULL, ORANGE_TRUE);
                        }       
		            }

                    notify_msg.hWnd     = msg->h_deswin;
	                notify_msg.id       = GUI_WinGetItemId(msg->h_deswin);
	                notify_msg.msgcode  = BN_PUSHED;
	                notify_msg.dwAddData= GUI_MSG_TOUCH_MOVE;
	                GUI_NotifyParent (&notify_msg);
				}
				break;
				
				case GUI_MSG_TOUCH_UP:
				{
					__s32               x;
                    __s32               y;
		            RECT                client_rect;
                    __gui_rect_t        screct;

		            if (GUI_WinGetCaptureWin() != msg->h_deswin)
		            {
		                break;
		            }

		            GUI_WinReleaseCapture ();

		            x = LOSWORD(msg->dwAddData2);
		            y = HISWORD(msg->dwAddData2);
					
		            GUI_WinGetClientRECT(GUI_WinGetParent (msg->h_deswin), &client_rect);
		            SysRectToGuiRect(&client_rect,&screct);
		            
		            if (PtInRect ((__gui_rect_t *)&screct, x, y))
		            {			                
		                BUTTON_STATUS(msg->h_deswin) &= ~BST_PUSHED;

		                {
							__gui_notify_msg_t      notify_msg;

			                notify_msg.hWnd     = msg->h_deswin;
			                notify_msg.id       = GUI_WinGetItemId(msg->h_deswin);
			                notify_msg.msgcode  = BN_CLICKED;
			                notify_msg.dwAddData= last_key;
			                    
			                GUI_NotifyParent (&notify_msg);
			            }
		                
		            }
		            else if (BUTTON_STATUS(msg->h_deswin) & BST_PUSHED) 
					{
		                BUTTON_STATUS(msg->h_deswin) &= ~BST_PUSHED;

						{
							__gui_notify_msg_t      notify_msg;

			                notify_msg.hWnd     = msg->h_deswin;
			                notify_msg.id       = GUI_WinGetItemId(msg->h_deswin);
			                notify_msg.msgcode  = BN_CLICKED;
			                notify_msg.dwAddData= last_key;
			                    
			                GUI_NotifyParent (&notify_msg);
			            }
		            }
				}
				break;	
			}
		}

        if(msg->dwAddData1 != GUI_MSG_TOUCH_UP)
        {
            last_key = msg->dwAddData1;
        }
        else
        {
            last_key = 0xffffffff;
        }
            
        return 0;

        case GUI_MSG_WIN_WAKEUP:
        case GUI_MSG_PAINT:
        {
			__gui_rect_t 	       rcrect;
            RECT                   fbrect;
            button_data_t          *button_data;
            button_attribute_t     *button_attribute;
            
            GUI_RECT               rect;


			button_attribute = (button_attribute_t *)(((button_data_t *)GUI_CtrlWinGetAddData(msg->h_deswin))->data);

            button_data = (button_data_t *)GUI_CtrlWinGetAddData(msg->h_deswin);

            GUI_LyrWinSel(GUI_LyrP2H(GUI_WinGetLyrWin(msg->h_deswin)));
            
            GUI_WinGetClientFBRect(msg->h_deswin,&fbrect);
            if(GUI_WinGetStyle(msg->h_deswin) & WS_MEMDEV)
            {
                if(!button_data->hmem)
                {
                	#if GUI_CORE_MEMDEV_EN
                    button_data->hmem = GUI_MEMDEV_Create(fbrect.x,fbrect.y,fbrect.width,fbrect.height);
					#endif
                }
            }
			SysRectToGuiRect(&fbrect,&rcrect);

            if(button_data->hmem)
            {
            	#if GUI_CORE_MEMDEV_EN
                GUI_MEMDEV_Select(button_data->hmem); 
				#endif
            }// start memory device
            if (BUTTON_DRAWSTATUS(msg->h_deswin) & BST_FOCUS)
            {
                GUI_SetBkColor(button_attribute->button_paint.bk_color);
                
            	GUI_ClearRect(rcrect.left,rcrect.top,
                 			rcrect.right,rcrect.bottom);

                if(button_attribute->button_paint.focus_bmp)
                {
                    GUI_BMP_Draw(button_attribute->button_paint.focus_bmp, rcrect.left + button_attribute->button_paint.bmp_pos.x, 
    					rcrect.top + button_attribute->button_paint.bmp_pos.y);
                }
				
				if(button_attribute->button_paint.alpha_en)
                {
                    GUI_OpenAlphaBlend();
                }
                
				GUI_CharSetToEncode(button_attribute->button_paint.draw_code);
				GUI_SetFont(button_attribute->button_paint.draw_font);

				GUI_SetDrawMode(GUI_DRAWMODE_TRANS);
			    GUI_SetColor(button_attribute->button_paint.ftxt_color);
			    rect.x0 = rcrect.left;
                rect.y0 = rcrect.top;
                rect.x1 = rcrect.right;
                rect.y1 = rcrect.bottom;
                if(!button_attribute->button_paint.txt_align)
                {
    			    GUI_DispStringAt(button_attribute->button_paint.text,
    			                     rcrect.left + button_attribute->button_paint.text_pos.x, 
    					            rcrect.top + button_attribute->button_paint.text_pos.y);
                }
                else
                {
                    GUI_DispStringInRect(button_attribute->button_paint.text,&rect,button_attribute->button_paint.txt_align);
                }
				
            }
            else
            {
                GUI_SetBkColor(button_attribute->button_paint.bk_color);
				
            	GUI_ClearRect(rcrect.left,rcrect.top,
                 			rcrect.right,rcrect.bottom);

                if(button_attribute->button_paint.unfocus_bmp)
                {
                    GUI_BMP_Draw(button_attribute->button_paint.unfocus_bmp, rcrect.left + button_attribute->button_paint.bmp_pos.x, 
    					rcrect.top + button_attribute->button_paint.bmp_pos.y);
                }

                if(button_attribute->button_paint.alpha_en)
                {
                    GUI_OpenAlphaBlend();
                }
				
                GUI_CharSetToEncode(button_attribute->button_paint.draw_code);
                
				GUI_SetFont(button_attribute->button_paint.draw_font);

				GUI_SetDrawMode(GUI_DRAWMODE_TRANS);
			    GUI_SetColor(button_attribute->button_paint.uftxt_color);
			    rect.x0 = rcrect.left;
                rect.y0 = rcrect.top;
                rect.x1 = rcrect.right;
                rect.y1 = rcrect.bottom;
                if(!button_attribute->button_paint.txt_align)
                {
    			    GUI_DispStringAt(button_attribute->button_paint.text,
    			                     rcrect.left + button_attribute->button_paint.text_pos.x, 
    					            rcrect.top + button_attribute->button_paint.text_pos.y);
                }
                else
                {
                    GUI_DispStringInRect(button_attribute->button_paint.text,&rect,button_attribute->button_paint.txt_align);
                }
            }
            if(button_data->hmem)
            {
            	#if GUI_CORE_MEMDEV_EN
                GUI_MEMDEV_CopyToLCD( button_data->hmem );          // copy from memory device to fb
	            GUI_MEMDEV_Select( NULL );                    // stop memory device
                GUI_MEMDEV_Delete(button_data->hmem);
				#endif
                button_data->hmem = NULL;
            }

            if(button_attribute->button_paint.alpha_en)
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
*                                               GUI_ButtonSetFocusBmp
*
* author:           
*
* date:             2009-9-4
*
* Description:      设置button的focus bmp 
*
* parameters:       
*
* return:           if success return ERDK_OK
*                   if fail return the number of fail
* modify history: 
**********************************************************************************************************************
*/

__s32  GUI_ButtonSetFocusBmp(H_WIN hbutton,void *bmp)
{
    
    button_attribute_t *button_attribute;

    if((!hbutton) || (!bmp))
    {
        __wrn("input parameter error!\n");

        return ORANGE_FAIL;
    }
    button_attribute = (button_attribute_t *)(((button_data_t *)GUI_CtrlWinGetAddData(hbutton))->data);

    button_attribute->button_paint.focus_bmp = bmp;


    GUI_InvalidateRect(Win_Self, NULL, ORANGE_TRUE);


    return ORANGE_OK;
}
      
/*
**********************************************************************************************************************
*                                               GUI_ButtonSetUnFocusBmp
*
* author:           
*
* date:             2009-9-4
*
* Description:      设置button的un focus bmp 
*
* parameters:       
*
* return:           if success return ERDK_OK
*                   if fail return the number of fail
* modify history: 
**********************************************************************************************************************
*/

__s32  GUI_ButtonSetUnFocusBmp(H_WIN hbutton,void *bmp)
{

    button_attribute_t *button_attribute;

    if((!hbutton) || (!bmp))
    {
        __wrn("input parameter error!\n");

        return ORANGE_FAIL;
    }
    
    button_attribute = (button_attribute_t *)(((button_data_t *)GUI_CtrlWinGetAddData(hbutton))->data);

    button_attribute->button_paint.unfocus_bmp = bmp;

    GUI_InvalidateRect(Win_Self, NULL, ORANGE_TRUE);

    return ORANGE_OK;
}
      
/*
**********************************************************************************************************************
*                                               GUI_ButtonSetText
*
* author:           
*
* date:             2009-9-4
*
* Description:      设置button的Text 
*
* parameters:       
*
* return:           if success return ERDK_OK
*                   if fail return the number of fail
* modify history: 
**********************************************************************************************************************
*/

__s32  GUI_ButtonSetText(H_WIN hbutton,char *text)
{

    button_attribute_t *button_attribute;

    if((!hbutton) || (!text))
    {
        __wrn("input parameter error!\n");

        return ORANGE_FAIL;
    }
    button_attribute = (button_attribute_t *)(((button_data_t *)GUI_CtrlWinGetAddData(hbutton))->data);

    button_attribute->button_paint.text = text;

    GUI_InvalidateRect(Win_Self, NULL, ORANGE_TRUE);

    return ORANGE_OK;
}
      
/*
**********************************************************************************************************************
*                                               GUI_ButtonSetUnFcsTxtColor
*
* author:           
*
* date:             2009-9-4
*
* Description:      gui buttonsetunfcstxtcolor 
*
* parameters:       
*
* return:           if success return ERDK_OK
*                   if fail return the number of fail
* modify history: 
**********************************************************************************************************************
*/

__s32  GUI_ButtonSetUnFcsTxtColor(H_WIN hbutton,__u32 color)
{

    button_attribute_t *button_attribute;

    if((!hbutton))
    {
        __wrn("input parameter error!\n");

        return ORANGE_FAIL;
    }
    button_attribute = (button_attribute_t *)(((button_data_t *)GUI_CtrlWinGetAddData(hbutton))->data);

    button_attribute->button_paint.uftxt_color = color;


    GUI_InvalidateRect(Win_Self, NULL, ORANGE_TRUE);


    return ORANGE_OK;
}
      
/*
**********************************************************************************************************************
*                                               GUI_ButtonSetFcsTxtColor
*
* author:           
*
* date:             2009-9-4
*
* Description:      gui buttonsetfcstxtcolor 
*
* parameters:       
*
* return:           if success return ERDK_OK
*                   if fail return the number of fail
* modify history: 
**********************************************************************************************************************
*/

__s32  GUI_ButtonSetFcsTxtColor(H_WIN hbutton,__u32  color)
{

    button_attribute_t *button_attribute;

    if((!hbutton))
    {
        __wrn("input parameter error!\n");

        return ORANGE_FAIL;
    }

    
    button_attribute = (button_attribute_t *)(((button_data_t *)GUI_CtrlWinGetAddData(hbutton))->data);

    button_attribute->button_paint.ftxt_color = color;


    GUI_InvalidateRect(Win_Self, NULL, ORANGE_TRUE);


    return ORANGE_OK;
}
  
      
/*
**********************************************************************************************************************
*                                               GUI_ButtonSetBkColor
*
* author:           
*
* date:             2009-9-4
*
* Description:      gui buttonsetfcstxtcolor 
*
* parameters:       
*
* return:           if success return ERDK_OK
*                   if fail return the number of fail
* modify history: 
**********************************************************************************************************************
*/

__s32  GUI_ButtonSetBkColor(H_WIN hbutton,__u32  color)
{

    button_attribute_t *button_attribute;

    if((!hbutton))
    {
        __wrn("input parameter error!\n");

        return ORANGE_FAIL;
    }
    button_attribute = (button_attribute_t *)(((button_data_t *)GUI_CtrlWinGetAddData(hbutton))->data);

    button_attribute->button_paint.bk_color = color;
    

    GUI_InvalidateRect(Win_Self, NULL, ORANGE_TRUE);


    return ORANGE_OK;
}  
      
/*
**********************************************************************************************************************
*                                               GUI_ButtonSetBmpPos
*
* author:           
*
* date:             2009-9-4
*
* Description:      gui buttonsetbmppos 
*
* parameters:       
*
* return:           if success return ERDK_OK
*                   if fail return the number of fail
* modify history: 
**********************************************************************************************************************
*/

__s32  GUI_ButtonSetBmpPos(H_WIN hbutton,__pos_t *pos)
{

    button_attribute_t *button_attribute;

    if((!hbutton) || (!pos))
    {
        __wrn("input parameter error!\n");

        return ORANGE_FAIL;
    }
    button_attribute = (button_attribute_t *)(((button_data_t *)GUI_CtrlWinGetAddData(hbutton))->data);

    button_attribute->button_paint.bmp_pos.x = pos->x;
    
    button_attribute->button_paint.bmp_pos.y = pos->y;


    GUI_InvalidateRect(Win_Self, NULL, ORANGE_TRUE);

    return ORANGE_OK;
}
      
/*
**********************************************************************************************************************
*                                               GUI_ButtonSetTxtPos
*
* author:           
*
* date:             2009-9-4
*
* Description:      gui buttonsettxtpos 
*
* parameters:       
*
* return:           if success return ERDK_OK
*                   if fail return the number of fail
* modify history: 
**********************************************************************************************************************
*/

__s32  GUI_ButtonSetTxtPos(H_WIN hbutton,__pos_t *pos)
{

    button_attribute_t *button_attribute;

    if((!hbutton) || (!pos))
    {
        __wrn("input parameter error!\n");

        return ORANGE_FAIL;
    }
    
    button_attribute = (button_attribute_t *)(((button_data_t *)GUI_CtrlWinGetAddData(hbutton))->data);

    button_attribute->button_paint.text_pos.x = pos->x;
    
    button_attribute->button_paint.text_pos.y = pos->y;


    GUI_InvalidateRect(Win_Self, NULL, ORANGE_TRUE);


    return ORANGE_OK;
}
#endif
