/*
************************************************************************************************************************
*											         ePDK
*						            the Easy Portable/Player Develop Kits
*									           desktop system 
*
*						        	 (c) Copyright 2007-2010, ANDY, China
*											 All Rights Reserved
*
* File    	: tbar_com.c
* By      	: Andy.zhang
* Func		: tbar component 
* Version	: v1.0
* ======================================================================================================================
* 2009-10-15 15:27:26 andy.zhang  create this file, implements the fundemental interface;
************************************************************************************************************************
*/

#include "apps.h"
#include "tbar_com.h"
#include "GlideCurve.h"
#include "..\\Sset.h"

//////////////////////////////////////////////////////////////////////////
#if 0 //(Ϊ�˴�ӡ��, �ɰ������ض����GUI����ʱ��������Ҫ��C�ļ���ʹ��)
//re define GUI_BMP_Draw
#ifdef GUI_BMP_Draw
#define MY_GUI_BMP_DRAW                      (ORANGEMOD_FUNC_ENTRY->ptr_GUI_BMP_Draw         	)
#undef GUI_BMP_Draw
#define GUI_BMP_Draw(_p, _x, _y)             do \
{\
	{__msg("    GUI_BMP_Draw(%d, %d)\n", _x, _y);} \
	MY_GUI_BMP_DRAW(_p, _x, _y); \
}while(0)
#endif

//re define GUI_DispStringAt
#ifdef GUI_DispStringAt
#define MY_GUI_DispStringAt                  (ORANGEMOD_FUNC_ENTRY->ptr_GUI_DispStringAt       	)
#undef GUI_DispStringAt
#define GUI_DispStringAt(_p, _x, _y)         do\
{ \
	{__msg("    GUI_DispStringAt(%d, %d)->( %s )\n", _x, _y, _p);} \
	MY_GUI_DispStringAt(_p, _x, _y); \
}while(0)
#endif

//re define GUI_DispStringInRect
#ifdef GUI_DispStringInRect
#define MY_GUI_DispStringInRect              (ORANGEMOD_FUNC_ENTRY->ptr_GUI_DispStringInRect   	)
#undef GUI_DispStringInRect
#define GUI_DispStringInRect(_p, _prt, _fmt)  do \
{ \
	{__msg("    GUI_DispStringInRect(%d, %d, %d, %d)->( %s )\n", (_prt)->x0, (_prt)->y0, (_prt)->x1, (_prt)->y1, _p);} \
	MY_GUI_DispStringInRect(_p, _prt, _fmt); \
} while (0)
#endif

#endif
//////////////////////////////////////////////////////////////////////////

/**********************************************************************************************************************/
#define 	TB_BACKCOLOR		0x4c6a3906
#define		TB_TV_BACKCOLOR		0x00000000
#define 	STEP_ARRAY			128

static __u32  	last_key    = 0xffffffff;
/**********************************************************************************************************************/
typedef enum tag_glide_state
{
	GLIDE_IDLE,
	GLIDE_START,
	GLIDE_MOVE,
	GLIDE_STOP	
}glide_state_t;

typedef struct tag_tbar_ctrl
{	
	tbar_para_t *data;		
	
	__s32		max_item;		// ���������ʾ item ��Ŀ
	__s32		scn_in;			// ��ǰ��Ļ����� item index;
	__s32		scn_out;		// ��ǰ��Ļ���ұ� item index;
	__s32		old_focus;		// �ɵĽ���index
	__s32		new_focus;		// �µĽ���index
		
	__bool		init_flag;			
//	__s32		min_x, max_x;	// ͼ��x����, �����Сֵ	
	__pos_t		win_pos;
	__bool      touch_down;
	
	/* �������� */
	int 		step[STEP_ARRAY];// touch ������������
	int			len;			 // ��������Ч���ݳ���
	int 		index;			 // ͼ���ƶ�����
	
	__u8		tid;			 // ͼ���ƶ��߳�
	__krnl_event_t *sem;		 // ͬ���ź���		
	__pos_t		win_start;		 // ��ʼ���Ӧ�Ĵ�������	
	H_LYR		lyr;	
	__gcuv_para_t gcurv;		 // gcurv ����		
	
	glide_state_t gstate;		 // tbar �ƶ�״̬
}tbar_ctrl_t;

/**********************************************************************************************************************/

static void  set_language_gui_charset( void )
{
	GUI_UC_SetEncodeUTF8( );
}

__epdk_charset_enm_e get_language_charset( void )
{
	return dsk_get_langres_charset();
}

static __s32 tbar_on_key(__gui_msg_t *msg)
{
	H_WIN 			layer;
	tbar_ctrl_t 	*pCtl;
	__s32 			ret = EPDK_OK;
		
	pCtl = (tbar_ctrl_t *)GUI_WinGetAttr(msg->h_deswin);
	
	if(KEY_UP_ACTION == msg->dwAddData2)
	{
//		switch( msg->dwAddData1 )
		switch( last_key )
		{
			case GUI_MSG_KEY_ENTER:
//			case GUI_MSG_KEY_LONGENTER:
			{
				__gui_notify_msg_t	notify_msg;									// ���Ϸ�֪ͨ��
	
            	//dsk_keytone_on();
            	last_key = 0xffffffff;
            	notify_msg.hWnd     = msg->h_deswin;
            	notify_msg.id       = GUI_WinGetItemId(msg->h_deswin);
            	notify_msg.msgcode  = TB_UNPUSHED;
            	notify_msg.dwAddData= pCtl->new_focus;//pCtl->data->item[pCtl->new_focus].type ;	// item id
                                
           		GUI_NotifyParent (&notify_msg);
				
				
				break;
			}
			default:
				break;
		}
	}
	else
	{
		switch( msg->dwAddData1 )
		{		
			case GUI_MSG_KEY_ENTER:
			{
				__gui_notify_msg_t	notify_msg;									// ���Ϸ�֪ͨ��
				 last_key = GUI_MSG_KEY_ENTER;
	            notify_msg.hWnd     = msg->h_deswin;
	            notify_msg.id       = GUI_WinGetItemId(msg->h_deswin);
	            notify_msg.msgcode  = TB_PUSHED;
	            notify_msg.dwAddData= pCtl->new_focus ;	// item id
	                
	            GUI_NotifyParent (&notify_msg);
	            
	           
			}
			break;
			
			case GUI_MSG_KEY_RIGHT:
			case GUI_MSG_KEY_LONGRIGHT:
			{			
				//dsk_keytone_on();
				
				if( pCtl->new_focus <(pCtl->data->item_nr -1) )
				{
					RECT 				rect;
					int					offset;
								
					layer = GUI_WinGetLyrWin(msg->h_deswin);
					pCtl->old_focus = pCtl->new_focus;
					pCtl->new_focus++;
					
					GUI_InvalidateRect (msg->h_deswin, NULL, EPDK_TRUE);
					
					GUI_LyrWinGetSrcWindow(layer, &rect);
					offset = (pCtl->new_focus+3)*pCtl->data->frame_size.width ;
					
					__msg("offset = %d, x = %d, width = %d \n", offset, rect.x, pCtl->data->scn_width);
					if( offset >(rect.x + pCtl->data->scn_width) )
					{
						GUI_LyrWinCacheOn();
						rect.x += pCtl->data->frame_size.width;
						GUI_LyrWinSetSrcWindow(layer, &rect);	
						GUI_LyrWinCacheOff();
					}								
					{
						__gui_notify_msg_t	notify_msg;
						
						notify_msg.hWnd     = msg->h_deswin;
			            notify_msg.id       = GUI_WinGetItemId(msg->h_deswin);
			            notify_msg.msgcode  = TB_SWITCH;
			            notify_msg.dwAddData= pCtl->new_focus;						// item id
						
						GUI_NotifyParent (&notify_msg);
					}
				}					
			}
			break;
		
			case GUI_MSG_KEY_LEFT:
			case GUI_MSG_KEY_LONGLEFT:
			{
				//dsk_keytone_on();
				
				if( pCtl->new_focus > 0 )
				{
					RECT 				rect;
					int					offset;
							
					layer = GUI_WinGetLyrWin(msg->h_deswin);
					pCtl->old_focus = pCtl->new_focus;	
					pCtl->new_focus--;
									
					GUI_InvalidateRect (msg->h_deswin, NULL, EPDK_TRUE);
					
					GUI_LyrWinGetSrcWindow(layer, &rect);				
					offset = (pCtl->new_focus+2)*(pCtl->data->frame_size.width);
					
					__msg("offset = %d, x = %d, width = %d \n", offset, rect.x, pCtl->data->scn_width);
					if( offset < rect.x)
					{										
						GUI_LyrWinCacheOn();	
						rect.x -= pCtl->data->frame_size.width;
						GUI_LyrWinSetSrcWindow(layer, &rect);					
						GUI_LyrWinCacheOff();
					}
					{
						__gui_notify_msg_t	notify_msg;
						
						notify_msg.hWnd     = msg->h_deswin;
			            notify_msg.id       = GUI_WinGetItemId(msg->h_deswin);
			            notify_msg.msgcode  = TB_SWITCH;
			            notify_msg.dwAddData= pCtl->new_focus;						// item id
						
						GUI_NotifyParent (&notify_msg);
					}			
				}
			}
			default:
				break;
		}	
	}
	return ret;
}


static __s32 point_to_item_id(tbar_ctrl_t *pCtl, __s32 x, __s32 y)
{
	__s32 index;
	tbar_para_t *data;
		
	data 	= pCtl->data;
	index  	= x/(data->frame_size.width);	
	
	if( index<0)
		index = 0;
	if( index>=pCtl->data->item_nr+1)
		index = pCtl->data->item_nr+1;
	
	return index;	
}


static void tbar_glide_thread(void *arg)
{
	tbar_ctrl_t *glide = (tbar_ctrl_t *)arg;
	__u8 		 err;
	int			 step;	
	RECT   	     rect;
	__bool		 flag = EPDK_FALSE;
	while(1)
	{		
		if(esKRNL_TDelReq(OS_PRIO_SELF) == OS_TASK_DEL_REQ)
        {                 
            esKRNL_TDel(OS_PRIO_SELF);            
            return;
        }
                
        esKRNL_SemPend(glide->sem, 0, &err);
        if( glide->index < glide->len )
    	{
    		 step = glide->step[glide->index];
    		 glide->index++;
    		 flag = EPDK_TRUE;
    	}           
        esKRNL_SemPost(glide->sem);
        if( flag == EPDK_TRUE )
        {
        	GUI_LyrWinGetSrcWindow(glide->lyr, &rect);
        	rect.x = 2*(glide->data->frame_size.width) - step;        	
			GUI_LyrWinSetSrcWindow(glide->lyr, &rect); 			
			flag = EPDK_FALSE; 
			continue;
        }
        else
        {
        	esKRNL_TimeDly(3);    /* delay 30ms */  
        }
	}
		
}

/* touch msg proc */
static __s32 tbar_on_touch(__gui_msg_t *msg)
{
	tbar_ctrl_t *pCtl;
	__s32 		 ret;
	__s32		 item_id;		
	__pos_t		 win_pos;	
	__pos_t		 scn_pos;
	RECT 		 src_rect;
		
	ret  = EPDK_OK;
	pCtl = (tbar_ctrl_t *)GUI_WinGetAttr(msg->h_deswin);
	
	win_pos.x = LOSWORD(msg->dwAddData2);
	win_pos.y = HISWORD(msg->dwAddData2);
	
	GUI_WindowToScreen(msg->h_deswin, &win_pos, &scn_pos);
	
	item_id = point_to_item_id(pCtl, win_pos.x , win_pos.y);		

	if( pCtl->gstate == GLIDE_IDLE )
	{
		switch( msg->dwAddData1 )
		{
			case GUI_MSG_TOUCH_DOWN:
			{
				if (GUI_WinGetCaptureWin() != msg->h_deswin)		// ����capture
				{
              		GUI_WinSetCaptureWin(msg->h_deswin);
				}
						        
		        if( pCtl->index != pCtl->len )
		        {
		        	__u8	err;
		        	
		        	esKRNL_SemPend(pCtl->sem, 0, &err);
		        	pCtl->index = pCtl->len;
		        	esKRNL_SemPost(pCtl->sem);	
		        	pCtl->gstate = GLIDE_STOP;
		        }
		        else
		        {
					pCtl->win_start.x = win_pos.x;
					pCtl->win_start.y = win_pos.y;		
				
					GUI_LyrWinGetSrcWindow(pCtl->lyr, &src_rect);
					
					pCtl->gcurv.fb_start = 2*(pCtl->data->frame_size.width)-src_rect.x;	
									
					glidecuv_init(&(pCtl->gcurv));
					glidecuv_start(scn_pos.x);
					
					//__msg("pCtl->gcurv.fb_start = %d \n", pCtl->gcurv.fb_start);
					//__msg(" glidecuv_start : scn_pos.x = %d \n", scn_pos.x);
					
					/*  ���ƽ��� */
					pCtl->old_focus = pCtl->new_focus;
					pCtl->new_focus = item_id-2;			
					GUI_InvalidateRect (msg->h_deswin, NULL, EPDK_TRUE);
					pCtl->gstate = GLIDE_START;
					{
						__gui_notify_msg_t	notify_msg;					// ���Ϸ�֪ͨ��
			
			            notify_msg.hWnd      = msg->h_deswin;
			            notify_msg.id        = GUI_WinGetItemId(msg->h_deswin);
			            notify_msg.msgcode   = TB_PUSHED;	         
			            notify_msg.dwAddData = pCtl->data->item[item_id -2].type ;
			               
			            GUI_NotifyParent (&notify_msg);
			        }		        		
		        }
			}
				break; 
			default:
				break;
		}		
	}
	else if( pCtl->gstate == GLIDE_START )
	{
		switch( msg->dwAddData1 )
		{
			case GUI_MSG_TOUCH_UP:
			{
				__pos_t scn_start;
				__u8	err;
				
				if( GUI_WinGetCaptureWin() == msg->h_deswin)				// ����capture
				{
              		GUI_WinReleaseCapture();
				}
				
				GUI_WindowToScreen(msg->h_deswin, &(pCtl->win_start), &scn_start);
								
				esKRNL_SemPend(pCtl->sem, 0, &err);
				pCtl->len = glidecuv_up(scn_pos.x, scn_start.x, LOSWORD(msg->dwReserved), HISWORD(msg->dwReserved), pCtl->step, STEP_ARRAY);
				pCtl->index = 0;
				esKRNL_SemPost(pCtl->sem);	
				pCtl->gstate = GLIDE_IDLE;
				{
					__gui_notify_msg_t	notify_msg;	
					
					//dsk_keytone_on();
					
					notify_msg.hWnd      = msg->h_deswin;
					notify_msg.id        = GUI_WinGetItemId(msg->h_deswin);
					notify_msg.msgcode   = TB_UNPUSHED;				
		        	notify_msg.dwAddData = item_id - 2;
		        	GUI_NotifyParent (&notify_msg);		
	       		}		       			       		
			}
				break;
			case GUI_MSG_TOUCH_MOVE:
			{
				__pos_t scn_start;
				__u8	err;
				
				GUI_WindowToScreen(msg->h_deswin, &(pCtl->win_start), &scn_start);
							
				esKRNL_SemPend(pCtl->sem, 0, &err);				
				pCtl->len = glidecuv_move(scn_pos.x, scn_start.x, pCtl->step, STEP_ARRAY);
				pCtl->index = 0;
				esKRNL_SemPost(pCtl->sem);	
				
				pCtl->gstate = GLIDE_MOVE;	
			}
				break;
				
			default:
				break;
		}
	}
	else if( pCtl->gstate == GLIDE_MOVE )
	{
		switch( msg->dwAddData1 )
		{
			case GUI_MSG_TOUCH_MOVE:
			{
				__pos_t scn_start;
				__u8	err;
				
				GUI_WindowToScreen(msg->h_deswin, &(pCtl->win_start), &scn_start);
							
				esKRNL_SemPend(pCtl->sem, 0, &err);				
				pCtl->len = glidecuv_move(scn_pos.x, scn_start.x, pCtl->step, STEP_ARRAY);
				pCtl->index = 0;
				esKRNL_SemPost(pCtl->sem);	
			}
				break;
							
			case GUI_MSG_TOUCH_UP:
			{
				__pos_t scn_start;
				__u8	err;
				
				if( GUI_WinGetCaptureWin() == msg->h_deswin)				// ����capture
				{
              		GUI_WinReleaseCapture();
				}
				
				GUI_WindowToScreen(msg->h_deswin, &(pCtl->win_start), &scn_start);
				pCtl->gstate = GLIDE_IDLE;				
				esKRNL_SemPend(pCtl->sem, 0, &err);
				pCtl->len = glidecuv_up(scn_pos.x, scn_start.x, LOSWORD(msg->dwReserved), HISWORD(msg->dwReserved), pCtl->step, STEP_ARRAY);
				pCtl->index = 0;
				esKRNL_SemPost(pCtl->sem);					
	       		
								
			}
				break;
			default:
				break;
		}
	}
	else if( pCtl->gstate == GLIDE_STOP )
	{
		switch( msg->dwAddData1 )
		{
			case GUI_MSG_TOUCH_MOVE:
			{
				__pos_t scn_start;
				__u8	err;
				
				pCtl->win_start.x = win_pos.x;
				pCtl->win_start.y = win_pos.y;		
			
				GUI_LyrWinGetSrcWindow(pCtl->lyr, &src_rect);
				
				pCtl->gcurv.fb_start = 2*(pCtl->data->frame_size.width)-src_rect.x;	
								
				glidecuv_init(&(pCtl->gcurv));
				glidecuv_start(scn_pos.x);
				
				GUI_WindowToScreen(msg->h_deswin, &(pCtl->win_start), &scn_start);
			
				esKRNL_SemPend(pCtl->sem, 0, &err);				
				pCtl->len = glidecuv_move(scn_pos.x, scn_start.x, pCtl->step, STEP_ARRAY);
				pCtl->index = 0;
				esKRNL_SemPost(pCtl->sem);	
				
				pCtl->gstate = GLIDE_MOVE;
			}
				break;
			case GUI_MSG_TOUCH_UP:
			{
				__pos_t scn_start;
				__u8	err;
				
				
				if( GUI_WinGetCaptureWin() == msg->h_deswin)				// ����capture
				{
              		GUI_WinReleaseCapture();
				}
				
				pCtl->win_start.x = win_pos.x;
				pCtl->win_start.y = win_pos.y;		
			
				GUI_LyrWinGetSrcWindow(pCtl->lyr, &src_rect);
				
				pCtl->gcurv.fb_start = 2*(pCtl->data->frame_size.width)-src_rect.x;	
								
				glidecuv_init(&(pCtl->gcurv));
				glidecuv_start(scn_pos.x);
				
				GUI_WindowToScreen(msg->h_deswin, &(pCtl->win_start), &scn_start);
				pCtl->gstate = GLIDE_IDLE;
				esKRNL_SemPend(pCtl->sem, 0, &err);
				pCtl->len = glidecuv_up(scn_pos.x, scn_start.x, LOSWORD(msg->dwReserved), HISWORD(msg->dwReserved), pCtl->step, STEP_ARRAY);
				pCtl->index = 0;
				esKRNL_SemPost(pCtl->sem);
				
				
			}
				break;
			
			default:
				break;
		}
	} 
	
	return ret;
}

static __s32 tbar_paint_bg(H_LYR layer, void* bg_bmp, __s32 src_pos)
{
    __s32 bmp_w;
    FB fb;

    GUI_LyrWinGetFB(layer, &fb);
	__here__
        
	if( GUI_LYRWIN_STA_SUSPEND == GUI_LyrWinGetSta(layer) )
	{
		return EPDK_OK;
	}

    bmp_w = GUI_BMP_GetXSize(bg_bmp);
    if(bmp_w > fb.size.width || bmp_w <= 0)
    {
        return EPDK_OK;
    }

    {
        __s32 i, n;

        n = fb.size.width/bmp_w;
        for(i = 0 ; i < n ; i++)
        {
            GUI_BMP_Draw(bg_bmp, src_pos+i*bmp_w, 0);
        }

        GUI_BMP_Draw(bg_bmp, src_pos+fb.size.width-bmp_w, 0);
    }
    
    return EPDK_OK;
}

static __s32 tbar_on_paint(__gui_msg_t *msg)
{
	H_LYR			layer;
	FB				fb;
	__gui_rect_t 	rcrect;
    RECT            fbrect;
    
    tbar_ctrl_t 	*pCtl;
    tbar_para_t		*data;
	__s32			ret = EPDK_OK;
	__s32			i;
	__s32			width;
	__s32			height;
	
	pCtl = (tbar_ctrl_t *)GUI_WinGetAttr(msg->h_deswin);
	data = pCtl->data;
	__here__
	layer= (H_LYR)GUI_WinGetLyrWin(msg->h_deswin);
	GUI_LyrWinGetFB(layer, &fb);
	__here__
	if( GUI_LYRWIN_STA_SUSPEND == GUI_LyrWinGetSta(layer) )
	{
		return EPDK_OK;
	}
	__here__
	fbrect.x = 0;
	fbrect.y = 0;
	fbrect.width  = fb.size.width;
	fbrect.height = fb.size.height;
	__here__
	GUI_LyrWinSel(layer);	
	__here__
	if(pCtl->init_flag == EPDK_TRUE)		// ��ʼ��ʱ����������icon
	{
		/* �屳�� */	
		__here__	
//		GUI_SetBkColor(TB_BACKCOLOR);
//		GUI_ClearRect(0,0,fbrect.width,1);
//		GUI_SetBkColor(0xBF4B4B4B);
//		GUI_ClearRect(0,1,fbrect.width,2);
		dsk_display_get_size(&width, &height);
		if(width == 800 && height == 480)
		{
		__here__
			GUI_SetBkColor(TB_BACKCOLOR);
		}
		else
		{
			GUI_SetBkColor(TB_TV_BACKCOLOR);
		}
		GUI_ClearRect(fbrect.x, fbrect.y, fbrect.x + fbrect.width -1, fbrect.y + fbrect.height -1);
		__here__

        if(data->bg_bmp)
        {
            tbar_paint_bg(layer, data->bg_bmp, data->src_start);
        }
        
		for( i=0; i< data->item_nr; i++ )
		{
			rcrect.left 	= fbrect.x + data->src_start + (i+2)*(data->frame_size.width);
			rcrect.top		= fbrect.y;
			rcrect.right 	= rcrect.left + data->frame_size.width -1;
			rcrect.bottom 	= rcrect.top  + data->frame_size.height -1; 
__here__
			if( pCtl->new_focus == i )
			{
				__here__
				if( data->item[i].focus_bmp)
				{	
					__here__			
					/*GUI_ARGB_Draw*/GUI_BMP_Draw(data->item[i].focus_bmp, rcrect.left + data->bmp_pos.x, rcrect.top+data->bmp_pos.y);
					__here__	
				}
				if(data->draw_font)
				{
					GUI_RECT txtrect;

                    __wrn("rcrect.top=%d\n", rcrect.top);
                    __wrn("rcrect.bottom=%d\n", rcrect.bottom);
					txtrect.x0 = rcrect.left;
					txtrect.y0 = rcrect.top + data->text_pos.y;
					txtrect.x1 = rcrect.right;
					txtrect.y1 = rcrect.bottom;
					
					GUI_OpenAlphaBlend();
					GUI_CharSetToEncode( dsk_get_langres_charset());
					GUI_SetFont(data->draw_font);
					GUI_SetDrawMode(GUI_DRAWMODE_TRANS);
					GUI_SetColor(data->focus_color);
                    __wrn("data->focus_color=0x%x\n", data->focus_color);
                    __wrn("data->item[i].text=%s\n", data->item[i].text);
					GUI_DispStringInRect( data->item[i].text, &txtrect, GUI_TA_HCENTER|GUI_TA_VCENTER);
					GUI_LyrWinFlushFB(layer);
					GUI_CloseAlphaBlend();
__here__
				}
			}
			else
			{
				if( data->item[i].unfocus_bmp)
				{			
					/*GUI_ARGB_Draw*/GUI_BMP_Draw(data->item[i].unfocus_bmp, rcrect.left + data->bmp_pos.x, rcrect.top + data->bmp_pos.y);	
				}
					
				if( data->draw_font )
				{
					GUI_RECT txtrect;
					
					txtrect.x0 = rcrect.left;
					txtrect.y0 = rcrect.top + data->text_pos.y;
					txtrect.x1 = rcrect.right;
					txtrect.y1 = rcrect.bottom;
					
					GUI_OpenAlphaBlend();
					GUI_CharSetToEncode( dsk_get_langres_charset());
					GUI_SetFont(data->draw_font);
					GUI_SetDrawMode(GUI_DRAWMODE_TRANS);
					GUI_SetColor(data->unfocus_color);
                    __wrn("data->unfocus_color=0x%x\n", data->unfocus_color);
                    __wrn("data->item[i].text=%s\n", data->item[i].text);
					GUI_DispStringInRect( data->item[i].text, &txtrect, GUI_TA_HCENTER|GUI_TA_VCENTER);
					GUI_LyrWinFlushFB(layer);
					GUI_CloseAlphaBlend();
				}  
			}
		}
		
		pCtl->init_flag = EPDK_FALSE;
	}
	else
	{        
		// ���� old_focus icon 
		rcrect.left 	= fbrect.x + data->src_start +(pCtl->old_focus + 2)*(data->frame_size.width);
		rcrect.top		= fbrect.y;
		rcrect.right 	= rcrect.left + data->frame_size.width -1;
		rcrect.bottom 	= rcrect.top  + data->frame_size.height-1; 
				
		if( data->item[pCtl->old_focus].unfocus_bmp )
		{		
			/*GUI_ARGB_Draw*/GUI_BMP_Draw(data->item[pCtl->old_focus].unfocus_bmp, rcrect.left + data->bmp_pos.x, rcrect.top + data->bmp_pos.y);
		}       
		if( data->draw_font )
		{
            GUI_RECT txtrect;
					
			txtrect.x0 = rcrect.left;
			txtrect.y0 = rcrect.top + data->text_pos.y;
			txtrect.x1 = rcrect.right;
			txtrect.y1 = rcrect.bottom;
			set_language_gui_charset();
			GUI_SetFont(data->draw_font);
			GUI_SetDrawMode(GUI_DRAWMODE_TRANS);
			GUI_SetColor(data->unfocus_color);
            GUI_DispStringInRect( data->item[pCtl->old_focus].text, &txtrect, GUI_TA_HCENTER|GUI_TA_VCENTER);			
		}
		
		// ���� new icon  
	   	rcrect.left 	= fbrect.x + data->src_start +(pCtl->new_focus+2)*(data->frame_size.width);
	    rcrect.top		= fbrect.y;
	    rcrect.right 	= rcrect.left + data->frame_size.width -1;
	    rcrect.bottom 	= rcrect.top  + data->frame_size.height-1; 
   		
	    if(data->item[pCtl->new_focus].focus_bmp)
   		{   		
   			/*GUI_ARGB_Draw*/GUI_BMP_Draw(data->item[pCtl->new_focus].focus_bmp, rcrect.left + data->bmp_pos.x, rcrect.top + data->bmp_pos.y);
   		}
   	    if(data->draw_font)
		{
			GUI_RECT txtrect;
					
			txtrect.x0 = rcrect.left;
			txtrect.y0 = rcrect.top + data->text_pos.y;
			txtrect.x1 = rcrect.right;
			txtrect.y1 = rcrect.bottom;
			set_language_gui_charset();
			GUI_SetFont(data->draw_font);
			GUI_SetDrawMode(GUI_DRAWMODE_TRANS);
			GUI_SetColor(data->focus_color);
			GUI_ClearRect(txtrect.x0,txtrect.y0,txtrect.x1,txtrect.y1);
            GUI_DispStringInRect( data->item[pCtl->new_focus].text, &txtrect, GUI_TA_HCENTER|GUI_TA_VCENTER);			
        }		
	}
	return ret;
}


/*
 * tool bar ������Ӧ����
 */
static int cb_tbar_frm(__gui_msg_t *msg)
{
	switch(msg->id)
	{
		case GUI_MSG_CREATE:		
			return 0;
			
		case GUI_MSG_DESTROY:
		{
			tbar_ctrl_t *p_ctr;	
			__u8		err;		
			
			p_ctr = (tbar_ctrl_t *)GUI_WinGetAttr(msg->h_deswin);	
			
			/* ɾ���߳� */
			while( esKRNL_TDelReq(p_ctr->tid) != OS_TASK_NOT_EXIST)
			{				
				esKRNL_TimeDly(2);
			}
			
			/* ɾ���ź���*/
			esKRNL_SemDel(p_ctr->sem, 0, &err);
			esMEMS_Mfree(0, p_ctr);			
		}
			return 0;
			
		case GUI_MSG_KEY:
		{			
			tbar_on_key(msg);				
		}			
			return 0;
			
		case GUI_MSG_TOUCH:	
		{
			tbar_on_touch(msg);
		}		
			return 0;
		
		case TB_SET_UPDATE_LANGUAGE:
		case GUI_MSG_WIN_WAKEUP:
		{
			tbar_ctrl_t *p_ctr;	
						
			p_ctr = (tbar_ctrl_t *)GUI_WinGetAttr(msg->h_deswin);
			p_ctr->init_flag = EPDK_TRUE;
			
			GUI_InvalidateRect(msg->h_deswin, NULL, EPDK_TRUE );
			return 0;
		}
				
		case GUI_MSG_PAINT:	
			__here__;
			tbar_on_paint(msg);
			__here__;
			return 0;
        case SSET_STRING_UPDATE:
		{
            //
		    tbar_ctrl_t 	*pCtl;
		    tbar_para_t		*data;
            char            title[GUI_TITLE_MAX];
			__gui_msg_t     dmsg;
			
						
			pCtl = (tbar_ctrl_t *)GUI_WinGetAttr(msg->h_deswin);
			data = pCtl->data;
			dsk_langres_get_menu_text(msg->dwAddData1,title,sizeof(title));
			eLIBs_memcpy(data->item[pCtl->new_focus].text,title,sizeof(title));

            //eLIBs_printf("msg->dwAddData1 = %08x pCtl->new_focus = %d title=%s    \n",msg->dwAddData1,pCtl->new_focus,title);
			tbar_on_paint(msg);
			
			
		}
		
		default:
			break;
	}
		
	return GUI_FrmWinDefaultProc(msg); 
}

/* ��������̬ tool bar */
H_WIN tbar_frm_create(H_WIN parent, H_WIN layer, tbar_para_t *tbar_para)
{
	H_WIN 						h_win;		
	__gui_framewincreate_para_t para;	
	tbar_ctrl_t 				*p_ctr;
	FB 							fb;
	__here__		
	eLIBs_memset(&para, 0, sizeof(__gui_framewincreate_para_t));	
	para.dwStyle          = WS_VISIBLE;
	para.dwExStyle        = 0;
	para.name			  = NULL;
	para.hOwner			  = 0;
	para.hHosting         = parent;
	para.FrameWinProc     = cb_tbar_frm;
	para.id				  = ID_TOOL_BAR;
	para.hLayer           = layer;
	
	GUI_LyrWinGetFB(layer, &fb);
	
	para.rect.x			  = 0;
	para.rect.y			  = 2;
	para.rect.width		  = fb.size.width;
	para.rect.height	  = fb.size.height-2;
	
	para.BkColor.alpha    = 0;
	para.BkColor.red      = 0;
	para.BkColor.green    = 0;
	para.BkColor.blue     = 0;
	
	p_ctr = (tbar_ctrl_t*)esMEMS_Malloc(0, sizeof(tbar_ctrl_t));
	if( !p_ctr )
	{
		__wrn(" tbar_ctrl_t maollc error \n");
		return NULL;
	}
	eLIBs_memset(p_ctr, 0, sizeof(tbar_ctrl_t));
	__here__				
	p_ctr->data 		= tbar_para;			
	p_ctr->init_flag 	= EPDK_TRUE;
	p_ctr->touch_down	= EPDK_FALSE;		
	
	p_ctr->max_item 	= p_ctr->data->scn_width/p_ctr->data->frame_size.width;
	p_ctr->scn_in		= 0;
	p_ctr->scn_out		= p_ctr->max_item-1;
	p_ctr->old_focus	= p_ctr->data->focus_item;			
	p_ctr->new_focus 	= p_ctr->old_focus;	
	
//	p_ctr->min_x		= para.rect.x;
//	p_ctr->max_x		= para.rect.x + (p_ctr->data->item_nr - p_ctr->max_item + 1)*p_ctr->data->frame_size.width;
		
	p_ctr->sem			= esKRNL_SemCreate(1);
	p_ctr->lyr			= layer;
	
	p_ctr->gcurv.screen		= tbar_para->scn_width;
	//p_ctr->gcurv.fb_len	= fb.size.width - 4*(p_ctr->data->frame_size.width);
	p_ctr->gcurv.fb_len   	= (tbar_para->item_nr)*(tbar_para->frame_size.width);
	p_ctr->gcurv.fb_start 	= 0;
	p_ctr->gcurv.frame		= p_ctr->data->frame_size.width;
	
	p_ctr->gstate		= GLIDE_IDLE;
	p_ctr->tid			= esKRNL_TCreate(tbar_glide_thread, (void *)p_ctr, 0x1000, KRNL_priolevel3);
	__here__
	para.attr           = (void *)p_ctr;	
	h_win = GUI_FrmWinCreate(&para);
	if( !h_win )
	{
		__err("tbar fwin create error ! \n");
	} 
	__here__
	return h_win;
}

/* ɾ������̬ tool bar */
__s32 tbar_frm_delete(H_WIN frmwin)
{
	GUI_FrmWinDelete(frmwin);
	return EPDK_TRUE;
}


