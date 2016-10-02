/*
*********************************************************************************************************
*                                                    eMOD
*                                   the Easy Portable/Player Operation System
*                                              mod_mmp sub-system
*
*                                     (c) Copyright 2006-2007, kevin China
*                                              All Rights Reserved
*
* File   : subtitle_plybk.c
* Version: V1.0
* By     : kevin
* Date   : 2008-7-14 13:37
* Update : 2008-11-22 14:56 by kevin
*********************************************************************************************************
*/



#include "subtitle_plybk_s.h"

__cedar_sply_t   *cedar_sply;

static void  cedar_sply_maintsk(void *p_arg);
static __s32 CB_Print(void *arg);



/*
*********************************************************************************************************
*                                     MInit
*
* Description: module init function,
*
* Arguments  : void
*
* Returns    : if success return EPDK_OK
*               esle return EPDK_FAIL
*********************************************************************************************************
*/
__s32 MInit(void)
{
    MESSAGE("Cedar:splay: sply_MInit\n");

    cedar_sply = (__cedar_sply_t *)0;

    return EPDK_OK;
}
/*
*********************************************************************************************************
*                                     MExit
*
* Description: module exit function,
*
* Arguments  : p_arg    cedar handler
*
* Returns    : void
*
*********************************************************************************************************
*/
__s32 MExit(void)
{
    MESSAGE("Cedar:splay: sply_MExit\n");

    if(cedar_sply)
    {
        MClose((__mp *)cedar_sply);
    }
    return EPDK_OK;
}
/*
*********************************************************************************************************
*                                     MOpen
*
* Description: module open function
*
* Arguments  : mid， mod
*
* Returns    : module handle
*
*********************************************************************************************************
*/
__mp *MOpen(__u32 mid, __u32 mode)
{
    //__u8        err;
    __s32       ret;
    MESSAGE("Open subtitle play back module\n");
    
    CEDAR_HEAPINIT();
    CEDAR_PHEAPINIT();
    //create subtitle playback module handle
    if(cedar_sply != (__cedar_sply_t *)0)
    {
        WARNING("Cedar:sply: subtitle play back has been opended already!\n");
        goto _err0_mopen;
    }
    cedar_sply = (__cedar_sply_t*)malloc(sizeof(__cedar_sply_t));
    if(NULL == cedar_sply)
    {
        WARNING("cedar_sply = NULL, malloc fail\n");
        return NULL;
    }
    ret = Sply_Init(cedar_sply, mid);
    if(EPDK_OK != ret)
    {
        goto _err1_mopen;
    }

    return (__mp *)cedar_sply;

_err1_mopen:
    FREE(cedar_sply);
    cedar_sply = NULL;
_err0_mopen:
    CEDAR_HEAPEXIT();
    CEDAR_PHEAPEXIT();
    return (__mp *)0;
}


/*
*********************************************************************************************************
*                                     MClose
*
* Description: main task of module cedar
*
* Arguments  : mp   module handle
*
* Returns    : if success return EPDK_OK
*               esle return EPDK_FAIL
*********************************************************************************************************
*/
__s32 MClose(__mp* mp)
{
    //__u8        err;
    
    if(cedar_sply == (__cedar_sply_t *)0)
    {
        WARNING("subtitle playback has been closed already!\n");
        return EPDK_FAIL;
    }

    //check if the handle is valid
    if(mp != (__mp *)cedar_sply)
    {
        WARNING("subtitle play-back handle is invalid!\n");
        return EPDK_FAIL;
    }

    cedar_sply->close(cedar_sply);
    if(EPDK_OK != Sply_Exit(cedar_sply))
    {
        WARNING("Sply_Exit() fail\n");
    }
    free(cedar_sply);
    cedar_sply = NULL;
    CEDAR_HEAPEXIT();
    CEDAR_PHEAPEXIT();
    return EPDK_OK;
}


/*
*********************************************************************************************************
*                                     vply_MRead
*
* Description: do nothing
*
* Arguments  : none
*
* Returns    : n
*
*********************************************************************************************************
*/
__u32 MRead(void *pdata, __u32 size, __u32 n, __mp *mp)
{
    return n;
}


/*
*********************************************************************************************************
*                                     vply_MWrite
*
* Description: do nothing
*
* Arguments  : none
*
* Returns    : n
*
*********************************************************************************************************
*/
__u32 MWrite(const void *pdata, __u32 size, __u32 n, __mp *mp)
{
    return n;
}


/*
*********************************************************************************************************
*                                     MIoctrl
*
* Description: module control
*
* Arguments  : mp           module handle
*              cmd
*              aux
*              pbuffer
*
* Returns    : ?
*
*********************************************************************************************************
*/
__s32 MIoctrl(__mp *mp, __u32 cmd, __s32 aux, void *pbuffer)
{
    __s32 ret;
   // CedarMsg    tmpCedarMsgInst;
    if(NULL == mp)
    {
        WARNING("fatal error! mp = NULL\n");
    }
    switch(cmd)
    {
        case CEDAR_SPLY_CMD_SET_SUBSKTIN:
        {
            MESSAGE("CEDAR_SPLY_CMD_SET_SUBSKTIN:%x\n", pbuffer);
            cedar_sply->pSubSktin = (__hdle)pbuffer;
            return EPDK_OK;
        }
         case CEDAR_SPLY_CMD_SET_AVSHDL:
        {
            cedar_sply->avsync = (__mp *)pbuffer;
            return EPDK_OK;
        }
         case CEDAR_SPLY_CMD_SET_LDECHDL:
        {
            cedar_sply->ldec = (__mp *)pbuffer;
            return EPDK_OK;
        }
        case CEDAR_SPLY_CMD_SET_CB_MSGQPOST:
        {
            cedar_sply->CB_MsgQPost = (__pCBK_t)pbuffer;
            return EPDK_OK;
        }
         case CEDAR_SPLY_CMD_SET_CB_SHOWDRAW:
        {
            cedar_sply->CB_ShowDraw= (__pCBK_t)pbuffer;
            //cedar_sply->CB_ShowDraw= esKRNL_GetCallBack((__pCBK_t)CB_Print);
            return EPDK_OK;
        }
        case CEDAR_SPLY_CMD_OPEN:
        {
            return cedar_sply->open(cedar_sply);
        }
        case CEDAR_SPLY_CMD_CLOSE:
        {
            return cedar_sply->close(cedar_sply);
        }
        case CEDAR_SPLY_CMD_START:
        {
            MESSAGE("CEDAR_SPLY_CMD_START, sply_MIoctrl:%x\n", cmd);
            ret = cedar_sply->start(cedar_sply);
            return ret;
        }
        case CEDAR_SPLY_CMD_STOP:
        {
            ret = cedar_sply->stop(cedar_sply);
            return ret;

        }
        case CEDAR_SPLY_CMD_PAUSE:
        {
            if(cedar_sply->status != MEDIA_STATUS_PLAY)
            {
                WARNING("cedar_SPLY->status[0x%x] != MEDIA_STATUS_PLAY, check code!\n", cedar_sply->status);
            }
            cedar_sply->status = MEDIA_STATUS_PAUSE;
            return EPDK_OK;
        }
        case CEDAR_SPLY_CMD_CONTI:
        {
            if(cedar_sply->status != MEDIA_STATUS_PAUSE)
            {
                WARNING("cedar_sply->status[0x%x] != MEDIA_STATUS_PAUSE, check code!\n", cedar_sply->status);
            }
            cedar_sply->status = MEDIA_STATUS_PLAY;
            return EPDK_OK;
        }
     
        case CEDAR_SPLY_CMD_JUMP:
        {
            if(cedar_sply->status == MEDIA_STATUS_PLAY)
            {
                cedar_sply->status = MEDIA_STATUS_JUMP;
            }
            break;
        }
        case CEDAR_SPLY_CMD_CLEAR_FFRR:
        {
            if((cedar_sply->status == MEDIA_STATUS_FORWARD)     \
               || (cedar_sply->status == MEDIA_STATUS_BACKWARD))
            {
                cedar_sply->status = MEDIA_STATUS_PLAY;
            }
            break;
        }
        case CEDAR_SPLY_CMD_CLEAR_JUMP:
        {
            if(cedar_sply->status == MEDIA_STATUS_JUMP)
            {
                cedar_sply->status = MEDIA_STATUS_PLAY;
            }
            break;
        }

        case CEDAR_SPLY_CMD_SWITCH_SUB_SHOW:
        {
            return EPDK_OK;
        }
        case CEDAR_SPLY_CMD_SET_NEW_VIDEO_PTS:
        {
            return EPDK_OK;
        }
        default:
            WARNING("Subtitle play back command (%d) not recognized!\n", cmd);
            return EPDK_FAIL;
    }

    return EPDK_OK;
}


static void cedar_sply_maintsk(void *p_arg)
{    
    __s32                       result;
   
    INFORMATION("--------------------------- cedar_sply_maintsk_mode1 start!\n");
    
    cedar_sply->frame_show_flgs = 0;
    
    do
    {
        
        result = cedar_sply->QueryFrame(cedar_sply, cedar_sply->sub_inf.cur_isdb_item);
        if(result == EPDK_OK)
        {           
            cedar_sply->DrawSubtitle(cedar_sply, cedar_sply->sub_inf.cur_isdb_item, 0);                  
        }
        else
        {
            cedar_sply->ClearSubtitle(cedar_sply, cedar_sply->sub_inf.last_isdb_item, 0);
        }
        esKRNL_TimeDly(20);
        
        if(cedar_sply->stop_flag)
        {
            goto _sply_main_task_exit0;
        }

    } while(1);
     
    _sply_main_task_exit0:
      
    while(1)
    {
        if(cedar_sply->stop_flag)
        {
            break;
        }
        else
        {
            esKRNL_TimeDly(10);
            WARNING("sply task only wait quit...\n");
        }
    }
    
    INFORMATION("--------------------------- cedar_lply_maintsk end!\n");

    esKRNL_TDel(EXEC_prioself);
}



/*******************************************************************************
Function name: SetFrameRect
Description: 
    1.对于DE而言，可以设置frame buffer的显示部分的位置和宽高,仅把这部分显示在屏幕上。
    该函数就是设置frame buffer的显示部分的位置和宽高的。
    2. 设置src_pic_show_size和src_pic_show_rect， src_pic_size和src_pic_rect默认已经赋值了

    3.现在纯粹只是调用显示驱动的接口, 不在做其他事了.
Parameters: 
    
Return: 
    
Time: 2010/10/14
*******************************************************************************/
static __s32 Impl_CedarSplySetFrameRect(__cedar_sply_t *cedar_sply)
{

    return EPDK_OK;
}


/*******************************************************************************
Function name: VplyQueryFrame
Description: 
    1.该函数除了要帧之外，还会做delay处理，该函数结束后，如果要帧成功,必须立刻设置给DEdrv
Parameters: 
    
Return: 
    
Time: 2011/4/28
*******************************************************************************/
__s32 Impl_CedarSply_ClearSubtitle(__cedar_sply_t *cedar_sply, CSubtitleDrawItem  *tmpSubInf, __s32 enable)   //VplyQueryFrame
{
    __u32 nCurTm;
    __s32 ret;

    nCurTm =  esMODS_MIoctrl(cedar_sply->avsync, DRV_AVS_CMD_GET_CUR_TIME, 0, 0);
    //WARNING("## Clear Screen!\n");
      
    if(cedar_sply->frame_show_flgs == 1)
    {
        WARNING("## Clear Screen successful!\n");
        ret = SUB_ClearShowRect(cedar_sply, tmpSubInf);
        cedar_sply->frame_show_flgs = 0;
    }
  
    return EPDK_OK;
}

/*******************************************************************************
Function name: VplyQueryFrame
Description: 
    1.该函数除了要帧之外，还会做delay处理，该函数结束后，如果要帧成功,必须立刻设置给DEdrv
Parameters: 
    
Return: 
    
Time: 2011/4/28
*******************************************************************************/
__s32 Impl_CedarSply_DrawSubtitle(__cedar_sply_t *cedar_sply, CSubtitleDrawItem  *tmpSubInf, __s32 enable)   //VplyQueryFrame
{
    __u32 nCurTm;
    __s32 ret;

    nCurTm =  esMODS_MIoctrl(cedar_sply->avsync, DRV_AVS_CMD_GET_CUR_TIME, 0, 0);
    
    if (tmpSubInf->unionDrawMode.isdb.isFull == 0)  //必须有字幕数据时，才去画字幕信息；
    {
        if(cedar_sply->frame_show_flgs == 1)
        {
            ret = SUB_ClearShowRect(cedar_sply, cedar_sply->sub_inf.last_isdb_item);
            cedar_sply->frame_show_flgs = 0;
        }
            
        return EPDK_OK;
    }
    if ((tmpSubInf->unionDrawMode.isdb.uPts != cedar_sply->sub_inf.last_isdb_item->unionDrawMode.isdb.uPts) || (cedar_sply->sub_inf.last_isdb_item->unionDrawMode.isdb.isFull == 0))
    {
        WARNING("## last_uPts = [%d], cur_uPts = [%d]\n", cedar_sply->sub_inf.last_isdb_item->unionDrawMode.isdb.uPts, tmpSubInf->unionDrawMode.isdb.uPts);
        if(cedar_sply->frame_show_flgs == 1)
        {
            ret = SUB_ClearShowRect(cedar_sply, cedar_sply->sub_inf.last_isdb_item);
            cedar_sply->frame_show_flgs = 0;
       
        }
        ret = SUB_ShowDraw(cedar_sply, tmpSubInf);
        cedar_sply->frame_show_flgs = 1;

    }       
    return EPDK_OK;
}


/*******************************************************************************
Function name: VplyQueryFrame
Description: 
    1.该函数除了要帧之外，还会做delay处理，该函数结束后，如果要帧成功,必须立刻设置给DEdrv
Parameters: 
    
Return: 
    
Time: 2011/4/28
*******************************************************************************/
__s32 Impl_CedarSplyQueryFrame(__cedar_sply_t *cedar_sply, CSubtitleDrawItem *tmpSubInf)   //VplyQueryFrame
{
    //__s32   tmpret;
    __s32   ret = EPDK_FAIL;
    //__u32   cur_pts;
    __u32   nCurTm;
    
    CSubtitleDrawItem *pIsdbItem = (CSubtitleDrawItem *)tmpSubInf;
    
    if(MEDIA_STATUS_PAUSE == cedar_sply->status)
    {
        return EPDK_FAIL;
    }
    if(MEDIA_STATUS_JUMP == cedar_sply->status)
    {
        return EPDK_FAIL;
    }
    
    if(cedar_sply->status == MEDIA_STATUS_BACKWARD)
    {
         return EPDK_FAIL;
        
    }
    else if(cedar_sply->status == MEDIA_STATUS_FORWARD)   //如果是快退，要反过来处理
    {
         return EPDK_FAIL;
        
    }

    nCurTm =  esMODS_MIoctrl(cedar_sply->avsync, DRV_AVS_CMD_GET_CUR_TIME, 0, 0);
    ret = SUB_GetItem(cedar_sply, nCurTm, pIsdbItem);
    if (EPDK_OK == ret)
    {
        return EPDK_OK;
    }
//    if ((EPDK_OK == ret) && (pIsdb->isFull != 0))
//    {
//        return EPDK_OK;
//    }

  
    return EPDK_FAIL;
}


__s32 Impl_CedarSply_Open(__cedar_sply_t *cedar_sply)
{
    __u8    err;   
    if(EPDK_FAIL == SUB_ShowInit(cedar_sply))
    {
        WARNING(" sub show init failed!\n");
        return EPDK_FAIL;
    }

    //set subtitle window information to default value, 

    cedar_sply->sub_show_flag = 1;

    cedar_sply->status = MEDIA_STATUS_STOP;

    cedar_sply->stop_flag = 0;

    //request power resource
    esPWM_RequestPwmMode(SYS_PWM_MODE_LEVEL1);
    esMEM_RequestDramUsrMode(DRAM_USER_MODE_VIDEO);
    return EPDK_OK;
}
__s32 Impl_CedarSply_Close(__cedar_sply_t *cedar_sply)
{
    //__u8    err;
   
    SUB_ShowUninit(cedar_sply);
    
    cedar_sply->avsync = NULL;
    cedar_sply->ldec   = NULL;
 

    esPWM_ReleasePwmMode(SYS_PWM_MODE_LEVEL1);
    esMEM_ReleaseDramUsrMode(DRAM_USER_MODE_VIDEO);
      
    SetSplyInitValue(cedar_sply);
    return EPDK_OK;
}

__s32 Impl_CedarSply_Start(__cedar_sply_t *cedar_sply)
{
    //__s32   ret;
   
    //WARNING("Lply start begin!");
    if(cedar_sply->avsync == NULL)
    {
        WARNING("avs handle has not been set!\n");
        return EPDK_FAIL;
    }
    if(cedar_sply->ldec == NULL)
    {
        WARNING("ldec handle has not been set!!\n");
        return EPDK_FAIL;
    }
    if(cedar_sply->status != MEDIA_STATUS_IDLE && cedar_sply->status != MEDIA_STATUS_STOP)
    {
        WARNING("lply->status[%x] wrong, cannot start\n", cedar_sply->status);
        return EPDK_FAIL;
    }

    //start video play back main task
    cedar_sply->status = MEDIA_STATUS_PLAY;
    cedar_sply->stop_flag = 0;
    MESSAGE("Create subtitle player-back main task...\n");
   
    cedar_sply->main_tsk_prio = esKRNL_TCreate( cedar_sply_maintsk,
                                                (void *)0,
                                                0x400,
                                                (EPOS_rsvid << 8) | LPLY_MAINTASK_PRIO );

    if(!cedar_sply->main_tsk_prio)
    {
        WARNING("Create task for subtitle playback failed!\n");
        return EPDK_FAIL;
    }

    MESSAGE("Create subtitle player-back main task(%d) finish\n", cedar_sply->main_tsk_prio);
    return EPDK_OK;
}
__s32 Impl_CedarSply_Stop(__cedar_sply_t *cedar_sply)
{
    if(cedar_sply->status != MEDIA_STATUS_STOP)
    {
        //wait video play-back main task exit
        cedar_sply->stop_flag = 1;

        if(cedar_sply->main_tsk_prio)
        {
            //wait video play back main task exit
            esKRNL_TimeDlyResume(cedar_sply->main_tsk_prio);
            while(esKRNL_TDelReq(cedar_sply->main_tsk_prio) != OS_TASK_NOT_EXIST)
            {
                esKRNL_TimeDlyResume(cedar_sply->main_tsk_prio);
                esKRNL_TimeDly(1);
            }

            cedar_sply->main_tsk_prio = 0;
        }
        //sply的stop接口,目前暂时还加上初始化相关成员的操作,因为vply很可能不关.
        //cedar_sply->avsync = NULL;
        //cedar_sply->sdec = NULL;
        
        cedar_sply->status = MEDIA_STATUS_STOP;
    }

    return EPDK_OK;
}

__s32 Sply_Init(__cedar_sply_t *cedar_sply, __u32 mid)
{
   
    MEM_SET(cedar_sply, 0, sizeof(__cedar_sply_t));
    cedar_sply->mid = mid;
    cedar_sply->open                = Impl_CedarSply_Open;
    cedar_sply->close               = Impl_CedarSply_Close;
    cedar_sply->start               = Impl_CedarSply_Start;
    cedar_sply->stop                = Impl_CedarSply_Stop;
    cedar_sply->SetFrameRect        = Impl_CedarSplySetFrameRect; 
    cedar_sply->DrawSubtitle        = Impl_CedarSply_DrawSubtitle;
    cedar_sply->ClearSubtitle       = Impl_CedarSply_ClearSubtitle;  
    cedar_sply->QueryFrame          = Impl_CedarSplyQueryFrame;
 
    SetSplyInitValue(cedar_sply);

    return EPDK_OK;

_err0:
    return EPDK_FAIL;
}
__s32 Sply_Exit(__cedar_sply_t *thiz)
{
    return EPDK_OK;
}


/*******************************************************************************
Function name: SetInitValue
Description: 
    1.在Init时，一些成员变量的初始值的设置,一些在初始化时已经确定的值不会在这个函数里赋值.
    在IO_Close()时也会调用
Parameters: 
    
Return: 
    
Time: 2011/10/7
*******************************************************************************/
__s32 SetSplyInitValue(__cedar_sply_t *thiz)
{
    thiz->stop_flag = 0;
    thiz->sub_show_flag = 0;
    thiz->ldec = NULL;
    thiz->avsync = NULL;
    thiz->pSubSktin = 0;
    thiz->CB_MsgQPost = NULL;
    thiz->frame_show_flgs = 0;
    
    thiz->status = MEDIA_STATUS_STOP;
    return EPDK_OK;
}

__s32 SubFrameTurntoDrawItem(CSubtitleDrawItem      * pSubItem, CCedarSubtitleFrame *pSubFrame)
{
    __s32   i;
    __s32   ret;
    CSubtitleDrawModeISDB   *pIsdb;
    
    if (pSubItem != NULL && pSubFrame != NULL)
    {
        if(pSubFrame->uDataType != CEDAR_SUB_ENCODE_BITMAP)
        {
            pSubItem->nDrawMode = SUBTITLE_DRAWMODE_ISDB;
            //memcpy(&pSubItem->unionDrawMode.isdb, pSubFrame, sizeof(CCedarSubtitleFrame));//* 暂时: CCedarSubtitleFrame == CSubtitleDrawModeISDB;
            pIsdb = &pSubItem->unionDrawMode.isdb;
            
            pIsdb->isFull = pSubFrame->isFull;
            pIsdb->subNum = pSubFrame->subNum;
            pIsdb->position_flags = pSubFrame->position_flags;
            //pIsdb->Clear_screen_flags;
            pIsdb->uPts = pSubFrame->uPts;
            pIsdb->uDuration = pSubFrame->uDuration;
            pIsdb->uDataType = pSubFrame->uDataType;
            pIsdb->displayMode = pSubFrame->displayMode;
            for(i=0;i<10;i++)
            {
                pIsdb->subColor[i] = pSubFrame->subColor[i];
                pIsdb->subStyleSize[i] = pSubFrame->subStyleSize[i];
            }
            pIsdb->subSize = pSubFrame->subSize;
            pIsdb->subWidth = pSubFrame->subWidth;
            pIsdb->subHeight = pSubFrame->subHeight;
            pIsdb->startX = pSubFrame->startX;
            pIsdb->startY = pSubFrame->startY;
            for(i=0;i<10;i++)
            {
                pIsdb->line[i] = pSubFrame->line[i];
                pIsdb->forward[i] = pSubFrame->forward[i];
            }
            pIsdb->videoWidth = pSubFrame->videoWidth;
            pIsdb->videoHeight = pSubFrame->videoHeight;
            for(i=0;i<10;i++)
            {
                pIsdb->subDataLen[i] = pSubFrame->subDataLen[i];
                pIsdb->subData[i] = pSubFrame->subData[i];
                //pIsdb->subClearScreen[10];    //subtitle clear screen;
            }
            pIsdb->uSubSize = pSubFrame->uSubSize;
            pIsdb->pSubtitle = pSubFrame->pSubtitle;
    		ret = EPDK_OK;
        }
        else
        {
            WARNING("we do not support sub and pts now!\n");    //对于图像字幕的新要帧方式的做法,以后再讨论.
            pSubItem->nDrawMode = SUBTITLE_DRAWMODE_BITMAP;
//#if PLAY_DVBT_DVBSUBTITLE
            pIsdb = &pSubItem->unionDrawMode.isdb;
            
            pIsdb->isFull = pSubFrame->isFull;
            pIsdb->subNum = pSubFrame->subNum;
            pIsdb->position_flags = pSubFrame->position_flags;
            //pIsdb->Clear_screen_flags;
            pIsdb->uPts = pSubFrame->uPts;
            pIsdb->uDuration = pSubFrame->uDuration;
            pIsdb->uDataType = pSubFrame->uDataType;
            WARNING("DVB-T subNum=%d, uPts = %d, uDuration = %d\n", pIsdb->subNum, pIsdb->uPts, pIsdb->uDuration);

            for(i=0;i<pIsdb->subNum;i++)
            {
                pIsdb->subImageRect[i].x = pSubFrame->subImageRect[i].x;
                pIsdb->subImageRect[i].y = pSubFrame->subImageRect[i].y;
                pIsdb->subImageRect[i].width = pSubFrame->subImageRect[i].width;
                pIsdb->subImageRect[i].height = pSubFrame->subImageRect[i].height;
                pIsdb->subData[i] = pSubFrame->subData[i];
            }
//#endif						
	    ret = EPDK_OK;
        }
    }
    else
        ret = EPDK_FAIL;
    
    return ret;
}


/***************************************************************************************************
*Name        : robin_get_subtitle_item
*Prototype   : __s32  robin_get_subtitle_item( __u32 time, __cedar_subtitle_item_t *subtitle_item_p );
*Arguments   : time              input. unit : ms.
*              subtitle_item_p   output. the subtitle item information corresponding to the time.
*Return      : ==0    succeed
*              !=0    there is no subtitle item information corresponding to the time.
*Description : get the subtitle item information corresponding to the time.
*Other       :
***************************************************************************************************/
__s32  SUB_GetItem( __cedar_sply_t *cedar_sply, __u32 time, CSubtitleDrawItem  *pIsdbItem )
{
	CCedarSubtitleFrame   *pLric  = NULL;
    CSubtitleDrawItem     *pIsdb = (CSubtitleDrawItem *)pIsdbItem;	
    __s32 result;
		
	if(pIsdb == NULL)
		return -1;
		

    if(cedar_sply->ldec)
    {
        // get lyric item information
        pLric = (CCedarSubtitleFrame *)esMODS_MIoctrl(cedar_sply->ldec, CEDAR_LDEC_CMD_GET_SUBTITLEFRAME, time, (void *)CEDAR_GET_SUB_INF_ITEM);
    }

	if( pLric == NULL )           // no subtitle information at present
	{
		return -1;
	}
	else
	{ 
        result = SubFrameTurntoDrawItem(pIsdb, pLric);
        if(result == EPDK_OK)
            return 0;
        else
            return -1;
	}
}


__s32 SUB_ShowInit(__cedar_sply_t *cedar_sply)
{
    __s32 ret = EPDK_OK;
    cedar_sply->sub_inf.sub_list = (__subtitle_profile_t *)malloc(sizeof(__subtitle_profile_t));
	cedar_sply->sub_inf.cur_isdb_item = (CSubtitleDrawItem*)malloc(sizeof(CSubtitleDrawItem));
    cedar_sply->sub_inf.last_isdb_item = (CSubtitleDrawItem*)malloc(sizeof(CSubtitleDrawItem));
    if((NULL == cedar_sply->sub_inf.sub_list) || (NULL == cedar_sply->sub_inf.cur_isdb_item) ||
        (NULL == cedar_sply->sub_inf.last_isdb_item))
    {
        WARNING("fatal error! malloc failed!\n");     
        if(cedar_sply->sub_inf.sub_list)
        {
            free(cedar_sply->sub_inf.sub_list);
            cedar_sply->sub_inf.sub_list = NULL;
        }
        if(cedar_sply->sub_inf.cur_isdb_item)
        {
            free(cedar_sply->sub_inf.cur_isdb_item);
            cedar_sply->sub_inf.cur_isdb_item = NULL;
        }
        if(cedar_sply->sub_inf.last_isdb_item)
        {
            free(cedar_sply->sub_inf.last_isdb_item);
            cedar_sply->sub_inf.last_isdb_item = NULL;
        }
        ret = EPDK_FAIL;   
    }
    else
    {
        memset(cedar_sply->sub_inf.sub_list, 0, sizeof(__subtitle_profile_t));
        memset(cedar_sply->sub_inf.cur_isdb_item, 0, sizeof(CSubtitleDrawItem));
        memset(cedar_sply->sub_inf.last_isdb_item, 0, sizeof(CSubtitleDrawItem));
    }
	return ret;
}



__s32 SUB_ShowUninit(__cedar_sply_t *cedar_sply)
{
	//__u8 err;
	free(cedar_sply->sub_inf.sub_list);
        cedar_sply->sub_inf.sub_list = NULL;
	free(cedar_sply->sub_inf.cur_isdb_item);
        cedar_sply->sub_inf.cur_isdb_item = NULL;
    free(cedar_sply->sub_inf.last_isdb_item);
        cedar_sply->sub_inf.last_isdb_item = NULL;
    return EPDK_OK;
}


/*
**********************************************************************************************************************
*                                           SUB_ModifyPalette
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
__s32  SUB_ModifyPalette( __u32 palette_index, const __u32 *buf, __u32 nr )
{
	
	return EPDK_OK;
}


__s32 SUB_ClearShowRect(__cedar_sply_t *cedar_sply,  CSubtitleDrawItem *tmpSubInf)
{
    __s32 ret;
    
    if (NULL == tmpSubInf)
	{
		return EPDK_FAIL;
	}
   tmpSubInf->unionDrawMode.isdb.Clear_screen_flags = 1;
   if (cedar_sply->CB_ShowDraw != NULL)
   {
        if(EPDK_OK == esKRNL_CallBack(cedar_sply->CB_ShowDraw, (void *)tmpSubInf))
        {
            memset(tmpSubInf, 0, sizeof( CSubtitleDrawItem));
            ret = EPDK_OK;
        }
        else
        {
            memset(tmpSubInf, 0, sizeof( CSubtitleDrawItem));
            ret = EPDK_FAIL;
        }
   }
   else
   {
       memset(tmpSubInf, 0, sizeof( CSubtitleDrawItem));
       ret = EPDK_OK;
   }
	return ret;
}

/*
**********************************************************************************************************************
*                                           SUBTITLE_DRAW
*
* Description:
*
* Arguments  :lyrwin 绘制的图层  bFourceDraw强制刷新（如果得到的字幕和上次一样也刷新）
*
* Returns    :
*
* Notes      :
*
**********************************************************************************************************************
*/
//__s32 SUB_ShowDraw(H_WIN lyrwin, __bool bFourceDraw)

__s32 SUB_ShowDraw(__cedar_sply_t *cedar_sply,  CSubtitleDrawItem *tmpSubInf)
{
	__s32 ret;
	if (NULL == tmpSubInf)
	{
		return EPDK_FAIL;
	}
    tmpSubInf->unionDrawMode.isdb.Clear_screen_flags= 0;
    
    if (cedar_sply->CB_ShowDraw != NULL)
    {
        if(EPDK_OK == esKRNL_CallBack(cedar_sply->CB_ShowDraw, (void *)tmpSubInf))
        {
            memcpy(cedar_sply->sub_inf.last_isdb_item, tmpSubInf, sizeof(CSubtitleDrawItem));
            memset(tmpSubInf, 0, sizeof(CSubtitleDrawItem));
            ret = EPDK_OK;
        }
        else
        {
            memcpy(cedar_sply->sub_inf.last_isdb_item, tmpSubInf, sizeof(CSubtitleDrawItem));
            memset(tmpSubInf, 0, sizeof(CSubtitleDrawItem));
            ret = EPDK_FAIL;
        }
    }
    else
    {
        ret = EPDK_FAIL;
    }
	return ret;		
}

/*
**********************************************************************************************************************
*                                           SUB_SetSize
*
* Description:
*
* Arguments  :type: 0.normal 1.big  2.huge
*
* Returns    :
*
* Notes      :
*
**********************************************************************************************************************
*/
__s32 SUB_SetSize(__u16* subsize)
{

	return EPDK_OK;
}

/*
**********************************************************************************************************************
*                                           SUB_SetColor
*
* Description:
*
* Arguments  :
*
* Returns    :
*
* Notes      :自定义区0xC0和0xC1已经使用，使用0xC3
*
**********************************************************************************************************************
*/
__s32 SUB_SetColor(__s32 gui_color)
{
	return EPDK_OK;
}

/*
**********************************************************************************************************************
*                                           SUB_SetFrameColor
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
__s32 SUB_SetFrameColor(__s32 gui_color)
{
	return EPDK_OK;
}

static __s32 CB_Print(void *arg)
{
    //__u8        err;
    __wrn("## call-back function: Show draw subtitle bitstreams!\n");  
    return EPDK_OK; 
}

