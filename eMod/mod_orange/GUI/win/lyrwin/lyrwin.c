/*
**********************************************************************************************************************
*                                                    ePDK
*                                    the Easy Portable/Player Develop Kits
*                                              Lion Sub-System
*
*                                   (c) Copyright 2007-2009, Steven.ZGJ.China
*                                             All Rights Reserved
*
* Moudle  : win
* File    : win.c
*
* By      : Steven
* Version : v1.0
* Date    : 2008-10-4 11:24:32
**********************************************************************************************************************
*/
#include "lyrwin_i.h"

/*
**********************************************************************************************************************
*                                               InitZOrderInfo
*
* author:           
*
* date:             2009-7-28
*
* Description:      initzorderinfo 
*
* parameters:       
*
* return:           if success return ERDK_OK
*                   if fail return the number of fail
* modify history: 
**********************************************************************************************************************
*/

static void InitZOrderInfo(pzorderinfo pZOrderInfo, H_WIN hHost)
{
    pZOrderInfo->nNumber    = 0;
    pZOrderInfo->hWnd       = hHost;
    pZOrderInfo->pTopMost   = NULL;
}

/*
**********************************************************************************************************************
*                                               _LyrOn
*
* Description: 
*
* Arguments  : 
*
* Returns    : 
*
* Notes      : 
*
**********************************************************************************************************************
*/
static __s32 _LyrOn(__gui_lyrwin_t   *p_lyr)
{
    __s32          ret;
    __bool         wakeup = ORANGE_FALSE;
	__u32 		   pdispbuffer[3];

	__disp_fb_t    temp_disp_fb;

#if GUI_CHECK_ARG_EN > 0
    if(!p_lyr)
    {
        ORANGE_WRN("h_lyr empty!\n");
        return ORANGE_FAIL;
    }
#endif
    /* request hardware layer resource              */
    if(p_lyr->status == GUI_LYRWIN_STA_ON)
    {
        return EPDK_OK;
    }
    
    if(p_lyr->status == GUI_LYRWIN_STA_OFF)
    {
        goto exit;
    }

    /* resume fb, request software layer            */
    if(p_lyr->status == GUI_LYRWIN_STA_SUSPEND)
    {
        ret = GUI_FBResume(p_lyr->lyrpara.fb);
        if(ret != EPDK_OK)
        {
            ORANGE_WRN("framebuffer resume failed!\n");
            return ORANGE_FAIL;
        }

        p_lyr->h_swlyr = GUI_CreateLayer(p_lyr->lyrpara.fb->addr[0], 
                                         p_lyr->lyrpara.fb->size.width, 
                                         p_lyr->lyrpara.fb->size.height, 
                                         p_lyr->lyrpara.fb->fmt.fmt.rgb.pixelfmt);
        if(p_lyr->h_swlyr == (__hdle)(-1))
        {
            GUI_FBSuspend(p_lyr->lyrpara.fb);
            ORANGE_WRN("no software layer fit!\n");
            return ORANGE_FAIL;
        }
    }
    
    /* request hw layer                             */
    if(1)
    {
        ES_FILE      * p_disp = (ES_FILE *)0;
        /*open display driver handle*/
        p_disp = eLIBs_fopen("b:\\DISP\\DISPLAY", "r+");
        if(!p_disp)
        {
            ORANGE_WRN("open display device fail!\n");
            return ORANGE_FAIL;
        }
        p_lyr->devfp = p_disp;

        if(p_lyr->type == GUI_LYRWIN_NORMAL)
        {
            /*request hardware layer*/
            p_lyr->h_hwlyr = GUI_LogicLayerRequest(gui_scndir,p_disp,0,&(p_lyr->lyrpara));
            if(!p_lyr->h_hwlyr)
            {
                eLIBs_fclose(p_lyr->devfp);
                p_lyr->devfp = 0;
                if(p_lyr->status == GUI_LYRWIN_STA_SUSPEND)
                {
                    GUI_ReleaseLayer(p_lyr->h_swlyr);
                    p_lyr->h_swlyr = 0;
                    GUI_FBSuspend(p_lyr->lyrpara.fb);
                }
                ORANGE_WRN("request hardware layer failed!\n");
                return ORANGE_FAIL;
            } 
        }
        else
        {
            #if ORANGE_SUPPORT_SPRITE
            __disp_sprite_block_para_t   spritepara;

			GUI_Fb2Dispfb(p_lyr->lyrpara.fb, &temp_disp_fb);

            spritepara.fb       = temp_disp_fb; //p_lyr->lyrpara.fb modified by Derek,2010-12-13 16:21:04
            spritepara.src_win.x = p_lyr->lyrpara.src_win.x;
            spritepara.src_win.y = p_lyr->lyrpara.src_win.y;

			spritepara.src_win.width = p_lyr->lyrpara.src_win.width;  //modified by Derek,2011-1-8 14:15:41
			spritepara.src_win.height = p_lyr->lyrpara.src_win.height;
			
            spritepara.scn_win.x = p_lyr->lyrpara.scn_win.x;
            spritepara.scn_win.y = p_lyr->lyrpara.scn_win.y;
            spritepara.scn_win.width = p_lyr->lyrpara.scn_win.width;
            spritepara.scn_win.height = p_lyr->lyrpara.scn_win.height;

            eLIBs_fioctrl(p_disp, DISP_CMD_SPRITE_OPEN, SEL_SCREEN, NULL); //modified by Derek,2011-1-7 14:56:07
            /*request hardware layer*/
			pdispbuffer[0] = spritepara.fb.format;
			pdispbuffer[1] = spritepara.fb.seq;
			pdispbuffer[2] = 0;
			eLIBs_fioctrl(p_disp, DISP_CMD_SPRITE_SET_FORMAT, SEL_SCREEN, (void *)pdispbuffer);
			
			pdispbuffer[0] = (__u32)&spritepara;
			pdispbuffer[1] = 0;
			pdispbuffer[2] = 0;
            p_lyr->h_hwlyr = eLIBs_fioctrl(p_disp, DISP_CMD_SPRITE_BLOCK_REQUEST, SEL_SCREEN, (void *)pdispbuffer);

			pdispbuffer[0] = p_lyr->h_hwlyr;
			pdispbuffer[1] = 0;
			pdispbuffer[2] = 0;
            eLIBs_fioctrl(p_disp, DISP_CMD_SPRITE_BLOCK_OPEN, SEL_SCREEN, (void *)pdispbuffer);
            
            if(!p_lyr->h_hwlyr)
            {
                eLIBs_fclose(p_lyr->devfp);
                p_lyr->devfp = 0;
                if(p_lyr->status == GUI_LYRWIN_STA_SUSPEND)
                {
                    GUI_ReleaseLayer(p_lyr->h_swlyr);
                    p_lyr->h_swlyr = 0;
                    GUI_FBSuspend(p_lyr->lyrpara.fb);
                }
                ORANGE_WRN("request hardware layer failed!\n");
            }
            #endif
        }
    }
    
exit:
    if(p_lyr->type == GUI_LYRWIN_NORMAL)
    {
        /* open layer  modified by Derek,2010.12.07.16:05                                 */
		pdispbuffer[0] = p_lyr->h_hwlyr;
		pdispbuffer[1] = 0;
		pdispbuffer[2] = 0;
		
        ret = eLIBs_fioctrl(p_lyr->devfp, DISP_CMD_LAYER_OPEN, SEL_SCREEN, (void *)pdispbuffer);
        if(ret == ORANGE_FAIL)
        {
            ORANGE_WRN("lyr cannot be opened!\n");
            return ORANGE_FAIL;
        }
    }
    else
    {
        #if ORANGE_SUPPORT_SPRITE
        eLIBs_fioctrl(p_lyr->devfp, DISP_CMD_SPRITE_OPEN, SEL_SCREEN, NULL);
        /* open layer                                   */
		pdispbuffer[0] = p_lyr->h_hwlyr;
		pdispbuffer[1] = 0;
		pdispbuffer[2] = 0;
        ret = eLIBs_fioctrl(p_lyr->devfp, DISP_CMD_SPRITE_BLOCK_OPEN, SEL_SCREEN, (void *)pdispbuffer);
        if(ret == ORANGE_FAIL)
        {
            ORANGE_WRN("lyr cannot be opened!\n");
            return ORANGE_FAIL;
        }
        #endif
    }
    

    if(p_lyr->status != GUI_LYRWIN_STA_ON)
    {
        Layer_DeattachFromInVisList(GUI_LyrP2H(p_lyr));
        Layer_AttachToVisableList(GUI_LyrP2H(p_lyr));
    }

    if(p_lyr->status == GUI_LYRWIN_STA_SUSPEND)
    {
        wakeup = ORANGE_TRUE;
    }

    p_lyr->status = GUI_LYRWIN_STA_ON;

    /* tell all the wins                            */
    if(wakeup)
    {
        __gui_win_t * p_win;
        __gui_msg_t msg;

        if(p_lyr->ManWinZOrder.pTopMost)
        {
            p_win        = GUI_WinH2P(p_lyr->ManWinZOrder.pTopMost->hWnd);
            msg.id       = GUI_MSG_WIN_WAKEUP;
            msg.h_srcwin = GUI_WinP2H(0);
            msg.p_arg    = 0;
            while(p_win)
            {
                msg.h_deswin = GUI_WinP2H(p_win);
                GUI_SendMessage(&msg);
                if(p_win->pZOrderNode->pNext)
                {
                    p_win = GUI_WinH2P(p_win->pZOrderNode->pNext->hWnd);
                }
                else
                {
                    break;
                }
            }
        }

        if(p_lyr->TopMostWinZOrder.pTopMost)
        {
            p_win        = GUI_WinH2P(p_lyr->TopMostWinZOrder.pTopMost->hWnd);
            msg.id       = GUI_MSG_WIN_WAKEUP;
            msg.h_srcwin = GUI_WinP2H(0);
            msg.p_arg    = 0;
            while(p_win)
            {
                msg.h_deswin = GUI_WinP2H(p_win);
                GUI_SendMessage(&msg);
                if(p_win->pZOrderNode->pNext)
                {
                    p_win = GUI_WinH2P(p_win->pZOrderNode->pNext->hWnd);
                }
                else
                {
                    break;
                }
            }
        }
    } 

    return EPDK_OK;
}

/*
**********************************************************************************************************************
*                                               _LyrOff
*
* Description: 
*
* Arguments  : 
*
* Returns    : 
*
* Notes      : 
*
**********************************************************************************************************************
*/
static __s32 _LyrOff(__gui_lyrwin_t   *p_lyr)
{
    __s32          ret;
    __bool         wakeup = ORANGE_FALSE;
	__u32		   pdispbuffer[3];

	__disp_fb_t    temp_disp_fb;

#if GUI_CHECK_ARG_EN > 0
    if(!p_lyr)
    {
        ORANGE_WRN("h_lyr empty!\n");
        return ORANGE_FAIL;
    }
#endif
    /* request hardware layer resource              */
    if(p_lyr->status == GUI_LYRWIN_STA_OFF)
    {
        return EPDK_OK;
    }

    if(p_lyr->status == GUI_LYRWIN_STA_ON)
    {
        if(p_lyr->type == GUI_LYRWIN_NORMAL)
        {
        	pdispbuffer[0] = p_lyr->h_hwlyr;
			pdispbuffer[1] = 0;
			pdispbuffer[2] = 0;
			
            ret = eLIBs_fioctrl(p_lyr->devfp, DISP_CMD_LAYER_CLOSE, SEL_SCREEN, (void *)pdispbuffer);
            if(ret == ORANGE_FAIL)
            {
                ORANGE_WRN("lyr cannot be opened!\n");
                return ORANGE_FAIL;
            }
        }
        else
        {
            #if ORANGE_SUPPORT_SPRITE
			pdispbuffer[0] = p_lyr->h_hwlyr;
			pdispbuffer[1] = 0;
			pdispbuffer[2] = 0;
            ret = eLIBs_fioctrl(p_lyr->devfp, DISP_CMD_SPRITE_BLOCK_CLOSE, SEL_SCREEN, (void *)pdispbuffer);
            if(ret == ORANGE_FAIL)
            {
                ORANGE_WRN("lyr cannot be opened!\n");
                return ORANGE_FAIL;
            }
            #endif
        }

        goto exit;
        
    }


    /* resume fb, request software layer            */
    if(p_lyr->status == GUI_LYRWIN_STA_SUSPEND)
    {
        ret = GUI_FBResume(p_lyr->lyrpara.fb);
        if(ret != EPDK_OK)
        {
            ORANGE_WRN("framebuffer resume failed!\n");
            return ORANGE_FAIL;
        }

        p_lyr->h_swlyr = GUI_CreateLayer(p_lyr->lyrpara.fb->addr[0], 
                                         p_lyr->lyrpara.fb->size.width, 
                                         p_lyr->lyrpara.fb->size.height, 
                                         p_lyr->lyrpara.fb->fmt.fmt.rgb.pixelfmt);
        if(p_lyr->h_swlyr == (__hdle)(-1))
        {
            GUI_FBSuspend(p_lyr->lyrpara.fb);
            ORANGE_WRN("no software layer fit!\n");
            return ORANGE_FAIL;
        }
    }
    
    /* request hw layer                             */
    if(1)
    {
        ES_FILE      * p_disp = (ES_FILE *)0;
        /*open display driver handle*/
        p_disp = eLIBs_fopen("b:\\DISP\\DISPLAY", "r+");
        if(!p_disp)
        {
            ORANGE_WRN("open display device fail!\n");
            return ORANGE_FAIL;
        }
        p_lyr->devfp = p_disp;
            
        if(p_lyr->type == GUI_LYRWIN_NORMAL)
        {
            /*request hardware layer*/
            p_lyr->h_hwlyr = GUI_LogicLayerRequest(gui_scndir,p_disp,0,&(p_lyr->lyrpara));
            if(!p_lyr->h_hwlyr)
            {
                eLIBs_fclose(p_lyr->devfp);
                p_lyr->devfp = 0;
                if(p_lyr->status == GUI_LYRWIN_STA_SUSPEND)
                {
                    GUI_ReleaseLayer(p_lyr->h_swlyr);
                    p_lyr->h_swlyr = 0;
                    GUI_FBSuspend(p_lyr->lyrpara.fb);
                }
                ORANGE_WRN("request hardware layer failed!\n");
                return ORANGE_FAIL;
            } 
        }
        else
        {
            #if ORANGE_SUPPORT_SPRITE
            __disp_sprite_block_para_t   spritepara;

			GUI_Fb2Dispfb(p_lyr->lyrpara.fb, &temp_disp_fb);

            spritepara.fb       = temp_disp_fb; //p_lyr->lyrpara.fb; modified by Derek,2010-12-13 16:23:26
            spritepara.src_win.x = p_lyr->lyrpara.src_win.x;
            spritepara.src_win.y = p_lyr->lyrpara.src_win.y;
			
			spritepara.src_win.width = p_lyr->lyrpara.src_win.width;  //modified by Derek,2011-1-8 14:15:41
			spritepara.src_win.height = p_lyr->lyrpara.src_win.height;
			
            spritepara.scn_win.x = p_lyr->lyrpara.scn_win.x;
            spritepara.scn_win.y = p_lyr->lyrpara.scn_win.y;
            spritepara.scn_win.width = p_lyr->lyrpara.scn_win.width;
            spritepara.scn_win.height = p_lyr->lyrpara.scn_win.height;

            eLIBs_fioctrl(p_disp, DISP_CMD_SPRITE_OPEN, SEL_SCREEN, NULL);
            /*request hardware layer*/
			pdispbuffer[0] = spritepara.fb.format;
			pdispbuffer[1] = spritepara.fb.seq;
			pdispbuffer[2] = 0;
			eLIBs_fioctrl(p_disp, DISP_CMD_SPRITE_SET_FORMAT, SEL_SCREEN, (void *)pdispbuffer);
			
			pdispbuffer[0] = (__u32)&spritepara;
			pdispbuffer[1] = 0;
			pdispbuffer[2] = 0;
            p_lyr->h_hwlyr = eLIBs_fioctrl(p_disp, DISP_CMD_SPRITE_BLOCK_REQUEST, SEL_SCREEN, (void *)pdispbuffer);

			pdispbuffer[0] = p_lyr->h_hwlyr;
			pdispbuffer[1] = 0;
			pdispbuffer[2] = 0;
            eLIBs_fioctrl(p_disp, DISP_CMD_SPRITE_BLOCK_CLOSE, SEL_SCREEN, (void *)pdispbuffer);
            if(!p_lyr->h_hwlyr)
            {
                eLIBs_fclose(p_lyr->devfp);
                p_lyr->devfp = 0;
                if(p_lyr->status == GUI_LYRWIN_STA_SUSPEND)
                {
                    GUI_ReleaseLayer(p_lyr->h_swlyr);
                    p_lyr->h_swlyr = 0;
                    GUI_FBSuspend(p_lyr->lyrpara.fb);
                }
                ORANGE_WRN("request hardware layer failed!\n");
                return ORANGE_FAIL;
            } 
            #endif
        }      
    }
    
exit:
    if(p_lyr->status == GUI_LYRWIN_STA_ON)
    {
        Layer_DeattachFromVisList(GUI_LyrP2H(p_lyr));
        Layer_AttachToInVisList(GUI_LyrP2H(p_lyr));
    }
    
    if(p_lyr->status == GUI_LYRWIN_STA_SUSPEND)
    {
        wakeup = ORANGE_TRUE;
    }

    p_lyr->status = GUI_LYRWIN_STA_OFF;

    /* tell all the wins                            */
    if(wakeup)
    {
        __gui_win_t * p_win;
        __gui_msg_t msg;

        if(p_lyr->ManWinZOrder.pTopMost)
        {
            p_win        = GUI_WinH2P(p_lyr->ManWinZOrder.pTopMost->hWnd);
            msg.id       = GUI_MSG_WIN_WAKEUP;
            msg.h_srcwin = GUI_WinP2H(0);
            msg.p_arg    = 0;
            while(p_win)
            {
                msg.h_deswin = GUI_WinP2H(p_win);
                GUI_SendMessage(&msg);
                if(p_win->pZOrderNode->pNext)
                {
                    p_win = GUI_WinH2P(p_win->pZOrderNode->pNext->hWnd);
                }
                else
                {
                    break;
                }
            }
        }

        if(p_lyr->TopMostWinZOrder.pTopMost)
        {
            p_win        = GUI_WinH2P(p_lyr->TopMostWinZOrder.pTopMost->hWnd);
            msg.id       = GUI_MSG_WIN_WAKEUP;
            msg.h_srcwin = GUI_WinP2H(0);
            msg.p_arg    = 0;
            while(p_win)
            {
                msg.h_deswin = GUI_WinP2H(p_win);
                GUI_SendMessage(&msg);
                if(p_win->pZOrderNode->pNext)
                {
                    p_win = GUI_WinH2P(p_win->pZOrderNode->pNext->hWnd);
                }
                else
                {
                    break;
                }
            }
        }
    }            

    return EPDK_OK;
}

/*
**********************************************************************************************************************
*                                               _LyrSleep
*
* Description: 
*
* Arguments  : 
*
* Returns    : 
*
* Notes      : 
*
**********************************************************************************************************************
*/
static __s32 _LyrSleep(__gui_lyrwin_t   *p_lyr)
{
    __s32          ret;
    __bool         wakeup = ORANGE_FALSE;
	__u32 		   pbuffer[3];

#if GUI_CHECK_ARG_EN > 0
    if(!p_lyr)
    {
        ORANGE_WRN("h_lyr empty!\n");
        return ORANGE_FAIL;
    }
#endif

    /* check if layer already in sleep status       */
    if(p_lyr->status == GUI_LYRWIN_STA_SLEEP)
    {
        return EPDK_OK;
    }
 
    /* if suspend status, resume fb                 */
    if(p_lyr->status == GUI_LYRWIN_STA_SUSPEND)
    {
        ret = GUI_FBResume(p_lyr->lyrpara.fb);
        if(ret != EPDK_OK)
        {
            ORANGE_WRN("framebuffer resume failed!\n");
            return ORANGE_FAIL;
        }

        p_lyr->h_swlyr = GUI_CreateLayer(p_lyr->lyrpara.fb->addr[0], 
                                         p_lyr->lyrpara.fb->size.width, 
                                         p_lyr->lyrpara.fb->size.height, 
                                         p_lyr->lyrpara.fb->fmt.fmt.rgb.pixelfmt);
        if(p_lyr->h_swlyr == (__hdle)(-1))
        {
            GUI_FBSuspend(p_lyr->lyrpara.fb);
            ORANGE_WRN("no software layer fit!\n");
            return ORANGE_FAIL;
        }
    }   
    else    /* if on/off status                     */
    {   
        if(p_lyr->type == GUI_LYRWIN_NORMAL)
        {
            /*release hardware layer*/
			pbuffer[0] = p_lyr->h_hwlyr; //modified by Derek,2010.12.07.16:00
			pbuffer[1] = 0;
			pbuffer[2] = 0;
			
            ret = eLIBs_fioctrl(p_lyr->devfp, DISP_CMD_LAYER_CLOSE, SEL_SCREEN, (void *)pbuffer);
            if(ret == ORANGE_FAIL)
            {
                ORANGE_WRN("lyr cannot be closed!\n");
                return ORANGE_FAIL;
            }
			
            ret = eLIBs_fioctrl(p_lyr->devfp, DISP_CMD_LAYER_RELEASE, SEL_SCREEN, (void *)pbuffer);
            if(ret != EPDK_OK)
            {
                ORANGE_WRN("release hardware layer failed!\n");
                return ORANGE_FAIL;
            }
            eLIBs_fclose(p_lyr->devfp);
            p_lyr->devfp = (ES_FILE*)0;
            p_lyr->h_hwlyr = 0;

            esKRNL_TimeDly(3);//add by bayden, ��ʱ30ms���ȴ���ʾ����V�жϵ���
        }
        else
        {
            #if ORANGE_SUPPORT_SPRITE
            /*release hardware layer*/
			pbuffer[0] = p_lyr->h_hwlyr; //modified by Derek,2011-1-7 15:07:23
			pbuffer[1] = 0;
			pbuffer[2] = 0;
            ret = eLIBs_fioctrl(p_lyr->devfp, DISP_CMD_SPRITE_BLOCK_RELEASE, SEL_SCREEN, (void *)pbuffer);
            if(ret != EPDK_OK)
            {
                ORANGE_WRN("release hardware layer failed!\n");
                return ORANGE_FAIL;
            }
            eLIBs_fclose(p_lyr->devfp);
            p_lyr->devfp = (ES_FILE*)0;
            p_lyr->h_hwlyr = 0;
            #endif
        }         
    }

    if(p_lyr->status == GUI_LYRWIN_STA_ON)
    {
        Layer_DeattachFromVisList(GUI_LyrP2H(p_lyr));
        Layer_AttachToInVisList(GUI_LyrP2H(p_lyr));
    }

    if(p_lyr->status == GUI_LYRWIN_STA_SUSPEND)
    {
        wakeup = ORANGE_TRUE;
    }
    /* set status                                   */ 
    p_lyr->status = GUI_LYRWIN_STA_SLEEP;
 
    /* tell all the wins                            */
    if(wakeup)
    {
        __gui_win_t * p_win;
        __gui_msg_t msg;

        if(p_lyr->ManWinZOrder.pTopMost)
        {
            p_win        = GUI_WinH2P(p_lyr->ManWinZOrder.pTopMost->hWnd);
            msg.id       = GUI_MSG_WIN_WAKEUP;
            msg.h_srcwin = GUI_WinP2H(0);
            msg.p_arg    = 0;
            while(p_win)
            {
                msg.h_deswin = GUI_WinP2H(p_win);
                GUI_SendMessage(&msg);
                if(p_win->pZOrderNode->pNext)
                {
                    p_win = GUI_WinH2P(p_win->pZOrderNode->pNext->hWnd);
                }
                else
                {
                    break;
                }
            }
        }

        if(p_lyr->TopMostWinZOrder.pTopMost)
        {
            p_win        = GUI_WinH2P(p_lyr->TopMostWinZOrder.pTopMost->hWnd);
            msg.id       = GUI_MSG_WIN_WAKEUP;
            msg.h_srcwin = GUI_WinP2H(0);
            msg.p_arg    = 0;
            while(p_win)
            {
                msg.h_deswin = GUI_WinP2H(p_win);
                GUI_SendMessage(&msg);
                if(p_win->pZOrderNode->pNext)
                {
                    p_win = GUI_WinH2P(p_win->pZOrderNode->pNext->hWnd);
                }
                else
                {
                    break;
                }
            }
        }
    }         
  
    return EPDK_OK;
}
      

/*
**********************************************************************************************************************
*                                               _LyrSuspend
*
* Description: sleep layer, release framebuffer memory, sleep all the windows in the layer.
*
* Arguments  : layer handle
*
* Returns    : EPDK_OK/FAIL
*
* Notes      : 
*
**********************************************************************************************************************
*/
static __s32 _LyrSuspend(__gui_lyrwin_t   *p_lyr)
{
    __s32          ret;
    __bool         wakeup = ORANGE_FALSE;
	__u32		   pbuffer[3];

#if GUI_CHECK_ARG_EN > 0
    if(!p_lyr)
    {
        ORANGE_WRN("h_lyr empty!\n");
        return ORANGE_FAIL;
    }
#endif

    /* check if layer already in suspend status     */
    if(p_lyr->status == GUI_LYRWIN_STA_SUSPEND)
    {
        __msg("layer already in suspend status!\n");
        return EPDK_OK;
    }

    /*release hardware layer                        */
    if(p_lyr->h_hwlyr)
    {
        if(p_lyr->type == GUI_LYRWIN_NORMAL)
        {
        	pbuffer[0] = p_lyr->h_hwlyr;
			pbuffer[1] = 0;
			pbuffer[2] = 0;

            ret = eLIBs_fioctrl(p_lyr->devfp, DISP_CMD_LAYER_CLOSE, SEL_SCREEN, (void *)pbuffer);
            if(ret == ORANGE_FAIL)
            {
                ORANGE_WRN("lyr cannot be closed!\n");
                return ORANGE_FAIL;
            }
			
            ret = eLIBs_fioctrl(p_lyr->devfp, DISP_CMD_LAYER_RELEASE, SEL_SCREEN, (void *)pbuffer);
            if(ret != EPDK_OK)
            {
                __err("release hardware layer failed!\n");
            }
            eLIBs_fclose(p_lyr->devfp);
            p_lyr->devfp = (ES_FILE*)0;
            p_lyr->h_hwlyr = 0;

            esKRNL_TimeDly(3);//add by bayden, ��ʱ30ms���ȴ���ʾ����V�жϵ���
        }
        else
        {
            #if ORANGE_SUPPORT_SPRITE
            /*release hardware layer*/
			pbuffer[0] = p_lyr->h_hwlyr;
			pbuffer[1] = 0;
			pbuffer[2] = 0;
            ret = eLIBs_fioctrl(p_lyr->devfp, DISP_CMD_SPRITE_BLOCK_RELEASE, SEL_SCREEN, (void *)pbuffer);
            if(ret != EPDK_OK)
            {
                ORANGE_WRN("release hardware layer failed!\n");
                return ORANGE_FAIL;
            }
            eLIBs_fclose(p_lyr->devfp);
            p_lyr->devfp = (ES_FILE*)0;
            p_lyr->h_hwlyr = 0;
            #endif
        } 
    }
    
    /* release framebuffer                          */
    if(GUI_FBGetStatus(p_lyr->lyrpara.fb) == GUI_FB_STA_SLEEP)
    {
        ORANGE_WRN("fp already sleep!\n");
        return ORANGE_FAIL;
    }

    ret = GUI_FBSuspend(p_lyr->lyrpara.fb);
    if(ret != EPDK_OK)
    {
        ORANGE_WRN("framebuffer suspend failed!\n");
        return ORANGE_FAIL;
    }

    /* release software layer                       */
    if(p_lyr->h_swlyr)
    {
        GUI_ReleaseLayer(p_lyr->h_swlyr);
        p_lyr->h_swlyr = 0;
    }

    if(p_lyr->status == GUI_LYRWIN_STA_ON)
    {
        Layer_DeattachFromVisList(GUI_LyrP2H(p_lyr));
        Layer_AttachToInVisList(GUI_LyrP2H(p_lyr));
    }

    if(p_lyr->status != GUI_LYRWIN_STA_SUSPEND)
    {
        wakeup = ORANGE_TRUE;
    }
    
    /* set status to suspend                        */
    p_lyr->status = GUI_LYRWIN_STA_SUSPEND;

    /* tell all the wins                            */
    if(wakeup)
    {
        __gui_win_t * p_win;
        __gui_msg_t msg;

        if(p_lyr->ManWinZOrder.pTopMost)
        {
            p_win        = GUI_WinH2P(p_lyr->ManWinZOrder.pTopMost->hWnd);
            msg.id       = GUI_MSG_WIN_SLEEP;
            msg.h_srcwin = GUI_WinP2H(0);
            msg.p_arg    = 0;
            while(p_win)
            {
                msg.h_deswin = GUI_WinP2H(p_win);
                GUI_SendMessage(&msg);
                if(p_win->pZOrderNode->pNext)
                {
                    p_win = GUI_WinH2P(p_win->pZOrderNode->pNext->hWnd);
                }
                else
                {
                    break;
                }
            }
        }

        if(p_lyr->TopMostWinZOrder.pTopMost)
        {
            p_win        = GUI_WinH2P(p_lyr->TopMostWinZOrder.pTopMost->hWnd);
            msg.id       = GUI_MSG_WIN_SLEEP;
            msg.h_srcwin = GUI_WinP2H(0);
            msg.p_arg    = 0;
            while(p_win)
            {
                msg.h_deswin = GUI_WinP2H(p_win);
                GUI_SendMessage(&msg);
                if(p_win->pZOrderNode->pNext)
                {
                    p_win = GUI_WinH2P(p_win->pZOrderNode->pNext->hWnd);
                }
                else
                {
                    break;
                }
            }
        }
    } 

    return EPDK_OK;
}

/*
**********************************************************************************************************************
*                                               GUI_LyrWinSetSrcWindow
*
* Description: 
*
* Arguments  : 
*
* Returns    : 
*
* Notes      : 
*
**********************************************************************************************************************
*/
__s32  GUI_LyrWinSetSrcWindow (H_LYR h_lyr, const RECT * rect)
{
	__s32					ret;
    __gui_lyrwin_t           *lyr_attr   = (__gui_lyrwin_t *)h_lyr;

#if GUI_CHECK_ARG_EN > 0
    if(!lyr_attr)
    {
        ORANGE_WRN("h_lyr empty!\n");
        return ORANGE_FAIL;
    }
    if(!rect)
    {
        ORANGE_WRN("rect empty!\n");
        return ORANGE_FAIL;
    }
#endif
    
    GUI_LyrLock();
    lyr_attr->lyrpara.src_win = *rect;

    /* free hardware layer resource                 */
    if(lyr_attr->h_hwlyr)
    {
        ret = GUI_LogicLayerSetSrcWin(gui_scndir,lyr_attr->devfp,lyr_attr->h_hwlyr,(__rect_t *)rect);
        if(ret != EPDK_OK)
        {
            ORANGE_WRN("layer set source windows failed!\n");
        }
#ifdef SIM_PC_WIN
	  RefreshDeContent(0);
#endif
    }
    GUI_LyrUnlock();
    return EPDK_OK;
}
/*
**********************************************************************************************************************
*                                               GUI_LyrWinSetScnWindow
*
* Description: 
*
* Arguments  : 
*
* Returns    : 
*
* Notes      : 
*
**********************************************************************************************************************
*/
__s32  GUI_LyrWinSetScnWindow (H_WIN h_lyr, const RECT * rect)
{
    __u32          			ret;
    __gui_lyrwin_t           *lyr_attr   = (__gui_lyrwin_t *)h_lyr;

	__disp_rect_t				disp_rect;

	__u32 		   pdispbuffer[3];

#if GUI_CHECK_ARG_EN > 0
    if(!lyr_attr)
    {
        ORANGE_WRN("h_lyr empty!\n");
        return ORANGE_FAIL;
    }
    if(!rect)
    {
        ORANGE_WRN("rect empty!\n");
        return ORANGE_FAIL;
    }
#endif
    GUI_LyrLock();
    
    lyr_attr->lyrpara.scn_win = *rect;

    if(lyr_attr->h_hwlyr)
    {
        if(lyr_attr->type == GUI_LYRWIN_NORMAL)
        {
            ret = GUI_LogicLayerSetScnWin(gui_scndir,lyr_attr->devfp,lyr_attr->h_hwlyr,(__rect_t *)rect);
            if(ret != EPDK_OK)
            {
                ORANGE_WRN("layer set source windows failed!\n");
                GUI_LyrUnlock();
                return ORANGE_FAIL;
            }
        }
        else
        {
            #if ORANGE_SUPPORT_SPRITE
            /*release hardware layer*/
			disp_rect.x 		= rect->x;
			disp_rect.y 		= rect->y;
			disp_rect.width 	= rect->width;
			disp_rect.height 	= rect->height;

			pdispbuffer[0] = lyr_attr->h_hwlyr;
			pdispbuffer[1] = (__u32)&disp_rect;
			pdispbuffer[2] = 0;
            ret = eLIBs_fioctrl(lyr_attr->devfp, DISP_CMD_SPRITE_BLOCK_SET_SCREEN_WINDOW, SEL_SCREEN, (void *)pdispbuffer);
            if(ret != EPDK_OK)
            {
                ORANGE_WRN("release hardware layer failed!\n");
                GUI_LyrUnlock();
                return ORANGE_FAIL;
            }
            #endif
        } 
#ifdef SIM_PC_WIN
	  RefreshDeContent(0);
#endif
    }

    GUI_LyrUnlock();
    return EPDK_OK;
}
/*
**********************************************************************************************************************
*                                               GUI_LyrWinGetSrcWindow
*
* Description: 
*
* Arguments  : 
*
* Returns    : 
*
* Notes      : 
*
**********************************************************************************************************************
*/
__s32  GUI_LyrWinGetSrcWindow (H_WIN h_lyr, RECT * rect)
{
    __gui_lyrwin_t           *lyr_attr   = (__gui_lyrwin_t *)h_lyr;

#if GUI_CHECK_ARG_EN > 0
    if(!lyr_attr)
    {
        ORANGE_WRN("h_lyr empty!\n");
        return ORANGE_FAIL;
    }
    if(!rect)
    {
        ORANGE_WRN("rect empty!\n");
        return ORANGE_FAIL;
    }
#endif
    GUI_LyrLock();

    /* free hardware layer resource                 */
    *rect = lyr_attr->lyrpara.src_win;

    GUI_LyrUnlock();
    
    return EPDK_OK;
}
/*
**********************************************************************************************************************
*                                               GUI_LyrWinGetScnWindow
*
* Description: 
*
* Arguments  : 
*
* Returns    : 
*
* Notes      : 
*
**********************************************************************************************************************
*/
__s32  GUI_LyrWinGetScnWindow (H_WIN h_lyr, RECT * rect)
{
    __gui_lyrwin_t           *lyr_attr   = (__gui_lyrwin_t *)h_lyr;
    
#if GUI_CHECK_ARG_EN > 0
    if(!lyr_attr)
    {
        ORANGE_WRN("h_lyr empty!\n");
        return ORANGE_FAIL;
    }
    if(!rect)
    {
        ORANGE_WRN("rect empty!\n");
        return ORANGE_FAIL;
    }
#endif

    GUI_LyrLock();

    *rect = lyr_attr->lyrpara.scn_win;

    GUI_LyrUnlock();
    
    return EPDK_OK;
}


/*
**********************************************************************************************************************
*                                               GUI_LyrWinGetFB
*
* Description: 
*
* Arguments  : 
*
* Returns    : 
*
* Notes      : 
*
**********************************************************************************************************************
*/
__s32  GUI_LyrWinGetFB(H_LYR h_lyr, FB * fb)
{
    __gui_lyrwin_t           *lyr_attr   = (__gui_lyrwin_t *)h_lyr;

#if GUI_CHECK_ARG_EN > 0
    if(!h_lyr)
    {
        ORANGE_WRN("h_lyr empty!\n");
        return ORANGE_FAIL;
    }
    if(!fb)
    {
        ORANGE_WRN("fb empty!\n");
        return ORANGE_FAIL;
    }
#endif
    GUI_LyrLock();
    *fb = *(lyr_attr->lyrpara.fb);
    GUI_LyrUnlock();
    return EPDK_OK;
}


/*
**********************************************************************************************************************
*                                               GUI_LyrWinSetFB
*
* Description: 
*
* Arguments  : 
*
* Returns    : 
*
* Notes      : 
*
**********************************************************************************************************************
*/
__s32  GUI_LyrWinSetFB(H_LYR h_lyr, const FB * fb)
{
    __gui_lyrwin_t           *lyr_attr   = (__gui_lyrwin_t *)h_lyr;

#if GUI_CHECK_ARG_EN > 0
    if(!lyr_attr)
    {
        ORANGE_WRN("h_lyr empty!\n");
        return ORANGE_FAIL;
    }
    if(!fb)
    {
        ORANGE_WRN("fb empty!\n");
        return ORANGE_FAIL;
    }
#endif
	GUI_LyrLock();
	if(lyr_attr->status != GUI_LYRWIN_STA_SUSPEND)
	{
		GUI_LyrUnlock();
		return ORANGE_FAIL;
	}
	
	if(fb->addr[0] != NULL)
	{
		GUI_LyrUnlock();
		return ORANGE_FAIL;
	}
	
    *(lyr_attr->lyrpara.fb) = *fb;
    GUI_LyrUnlock();
    return EPDK_OK;
}

/*
**********************************************************************************************************************
*                                               GUI_LyrWinGetSta
*
* Description: 
*
* Arguments  : 
*
* Returns    : 
*
* Notes      : 
*
**********************************************************************************************************************
*/
__gui_lyrwin_sta_t GUI_LyrWinGetSta(H_LYR h_lyr)
{
    __gui_lyrwin_t           *lyr_attr   = (__gui_lyrwin_t *)h_lyr;
    
    __gui_lyrwin_sta_t        ret;
    
#if GUI_CHECK_ARG_EN > 0
    if(!lyr_attr)
    {
        ORANGE_WRN("h_lyr empty!\n");
        return GUI_LYRWIN_STA_ERR;
    }
#endif  
    GUI_LyrLock();

    ret = (__gui_lyrwin_sta_t)lyr_attr->status;
    
    GUI_LyrUnlock();
    
    return ret;


}
/*
**********************************************************************************************************************
*                                               GUI_LyrWinSetSta
*
* Description: 
*
* Arguments  : 
*
* Returns    : 
*
* Notes      : 
*
**********************************************************************************************************************
*/
__s32 GUI_LyrWinSetSta(H_LYR h_lyr, __gui_lyrwin_sta_t status)
{
	__s32					ret_val;
    __gui_lyrwin_t           *lyr_attr   = (__gui_lyrwin_t *)h_lyr;

#if GUI_CHECK_ARG_EN > 0
    if(!lyr_attr)
    {
        ORANGE_WRN("h_lyr empty!\n");
        return ORANGE_FAIL;
    }
#endif
    ret_val  = EPDK_OK;
    
    GUI_LyrLock();
    switch(status)
    {
        case GUI_LYRWIN_STA_ON:
             _LyrOn(lyr_attr);
#ifdef SIM_PC_WIN
			RefreshDeContent(1);
#endif
             break;

        case GUI_LYRWIN_STA_OFF:
             _LyrOff(lyr_attr);
#ifdef SIM_PC_WIN
			RefreshDeContent(1);
#endif
             break;

        case GUI_LYRWIN_STA_SLEEP:
             _LyrSleep(lyr_attr);
#ifdef SIM_PC_WIN
			RefreshDeContent(1);
#endif
             break;

        case GUI_LYRWIN_STA_SUSPEND:
             _LyrSuspend(lyr_attr);
#ifdef SIM_PC_WIN
			RefreshDeContent(1);
#endif
             break;
        default:
            ORANGE_WRN("sta not support! sta = %x\n", status);
            ret_val = ORANGE_FAIL;
            break;
    }
    GUI_LyrUnlock();
    return ret_val;
}
/*
**********************************************************************************************************************
*                                               GUI_LyrWinSel
*
* Description: 
*
* Arguments  : 
*
* Returns    : 
*
* Notes      : 
*
**********************************************************************************************************************
*/
__s32  GUI_LyrWinSel (H_LYR h_lyr)
{
	__gui_lyrwin_t           *lyr_attr   = (__gui_lyrwin_t *)h_lyr;
	if(lyr_attr == NULL)
		return EPDK_OK;

    GUI_Lock();
    GUI_SelLayer(lyr_attr->h_swlyr);
    GUI_Unlock();
    return EPDK_OK;
}
/*
**********************************************************************************************************************
*                                               GUI_LyrWinFlushFB
*
* Description: 
*
* Arguments  : 
*
* Returns    : 
*
* Notes      : 
*
**********************************************************************************************************************
*/
__s32 GUI_LyrWinFlushFB(H_LYR h_lyr)
{
	__gui_lyrwin_t           *lyr_attr   = (__gui_lyrwin_t *)h_lyr;

#if GUI_CHECK_ARG_EN > 0
    if(!lyr_attr)
    {
        ORANGE_WRN("h_lyr empty!\n");
        return ORANGE_FAIL;
    }
#endif    
    return GUI_FBFlushCache(lyr_attr->lyrpara.fb);
    
}
      
/*
**********************************************************************************************************************
*                                               GUI_LyrWinCreate
*
* author:           
*
* date:             2009-7-25
*
* Description:      gui layerwincreate 
*
* parameters:       
*
* return:           if success return ERDK_OK
*                   if fail return the number of fail
* modify history: 
**********************************************************************************************************************
*/
H_LYR  GUI_LyrWinCreate(__layerwincreate_para_t *create_info)
{
	__gui_lyrwin_t   *pLayer   = NULL;

    /* alloc win struct memory                      */
    pLayer = (__gui_lyrwin_t *)orange_malloc(sizeof(__gui_lyrwin_t));
    if(!pLayer)
    {
        ORANGE_WRN("alloc win memory failed!\n");

        return NULL;
    }
    
    /* init win struct contents                     */
    /* name                                         */
    eLIBs_memclr (pLayer->name, GUI_NAME_MAX);
    eLIBs_strncpy(pLayer->name, create_info->name, GUI_NAME_MAX - 1);
    
    /* copy lyrpara into struct                     */    
    pLayer->lyrpara      = *create_info->lyrpara;
    pLayer->lyrpara.fb   = (FB*)orange_malloc(sizeof(FB));
    *pLayer->lyrpara.fb  = *create_info->lyrpara->fb;
    pLayer->status       = GUI_LYRWIN_STA_SUSPEND;
    pLayer->h_swlyr      = GUI_LyrP2H(0);
    pLayer->h_hwlyr      = GUI_LyrP2H(0);
    pLayer->h_uselyr     = GUI_LyrP2H(0);
    pLayer->h_exlyr      = GUI_LyrP2H(0);
    pLayer->type         = create_info->type;
    pLayer->Next         = NULL;

    /*init z order*/
    InitZOrderInfo(&pLayer->ManWinZOrder, NULL);
    InitZOrderInfo(&pLayer->TopMostWinZOrder, NULL);

    //Layer_AttachToInVisList(GUI_LyrP2H(pLayer));  
    
    /* init status                                  */
    if(GUI_LyrWinSetSta(GUI_LyrP2H(pLayer), create_info->initsta) != EPDK_OK)
    {
        ORANGE_WRN("lyr win cannot be set status!\n");

        goto err;
    }

    /*����ͼ����ڲ�ʹ��״̬*/
    pLayer->use_sts = LAYER_DRAW;
    
    return GUI_LyrP2H(pLayer);

err:
    if(pLayer->lyrpara.fb)
    {
        orange_mfree(pLayer->lyrpara.fb);
    }
	if(pLayer)
	{
		orange_mfree (pLayer);
	}

    return HWND_INVALID;
}
/*
**********************************************************************************************************************
*                                               GUI_LyrWinDelete
*
* author:           
*
* date:             2009-7-25
*
* Description:      gui layerwindelete 
*
* parameters:       
*
* return:           if success return ERDK_OK
*                   if fail return the number of fail
* modify history: 
**********************************************************************************************************************
*/

__s32  GUI_LyrWinDelete(H_LYR  h_lyr)
{
    __gui_win_t             *next;    // hosted window list.
    __gui_win_t             *p_firstchild;
    __gui_lyrwin_t          *pLayer   = (__gui_lyrwin_t *)h_lyr;

    if(!h_lyr)
    {
        ORANGE_WRN("layer win handle error!\n");

        return ORANGE_FAIL;
    }

    if(!pLayer)
    {
        ORANGE_WRN("layer attr handle error!\n");

        return ORANGE_FAIL;
    }


    GUI_WinLock();

    if(pLayer->ManWinZOrder.pTopMost)
    {
        p_firstchild = (__gui_win_t *)pLayer->ManWinZOrder.pTopMost->hWnd;
        /* delete child win                             */
        while(1)
        { 
            /* if child list is empty, no win need be delete                                                            */
            if(!p_firstchild)
            {
                break;
            }        

            if(p_firstchild->pZOrderNode->pNext)
            {
                next = GUI_WinH2P(p_firstchild->pZOrderNode->pNext->hWnd);
            }
            else
            {
                next = NULL;
            }
            
            if(p_firstchild->WinType == TYPE_FRAMEWIN)
            {
            	#if GUI_WIN_FRMWIN_EN
                if(GUI_FrmWinDelete((H_WIN)p_firstchild) != ORANGE_OK)
                {
                    ORANGE_WRN("Not all child window can be deleted!\n");
                    GUI_WinUnlock();
                    return EPDK_FAIL;
                }
				#endif
            }
            else if(p_firstchild->WinType == TYPE_CONTROL)
            {
            	#if GUI_WIN_CTRLWIN_EN
                if(GUI_CtrlWinDelete((H_WIN)p_firstchild) != ORANGE_OK)
                {
                    ORANGE_WRN("Not all child window can be deleted!\n");
                    GUI_WinUnlock();
                    return EPDK_FAIL;
                }
				#endif
            }

			if(NULL == p_firstchild->pZOrderNode->pNext)
            {
                p_firstchild = NULL;
            }
            else
            {
                p_firstchild = next;
            }

            //p_firstchild = next;
        }
    }

    GUI_WinUnlock();

    /* release hw layer                             */
    if(GUI_LyrWinSetSta(h_lyr, GUI_LYRWIN_STA_SUSPEND) != EPDK_OK)
    {
        ORANGE_WRN("layer cannot be suspended!\n");
        
        return ORANGE_FAIL;
    };

    Layer_DeattachFromInVisList(h_lyr);

    /*free layer parameter framebuffer*/
    if(pLayer->lyrpara.fb)
    {
        orange_mfree((void *)pLayer->lyrpara.fb);
    }
    
	orange_mfree((void *)pLayer);

    pLayer   = 0;

    return EPDK_OK;
    
}
/*************************************************** end of file ****************************************************/
