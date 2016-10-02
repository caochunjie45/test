/*
********************************************************************************
*                                    eMOD
*                   the Easy Portable/Player Develop Kits
*                               mod_cedar sub-system
*                          (module name, e.g.mpeg4 decoder plug-in) module
*
*          (c) Copyright 2010-2012, Allwinner Microelectronic Co., Ltd.
*                              All Rights Reserved
*
* File   : CedarItxtDec.c
* Version: V1.0
* By     : Eric_wang
* Date   : 2011-10-16
* Description:
********************************************************************************
*/
#include "ldec_itxt_s.h"
#include "CedarItxtDec.h"

__s32 convertLdecResult_Lib2Cedar(__s32 nLibRet)
{
    __s32   ret;
    switch(nLibRet)
    {
        case LDEC_ERR_NONE:
        {
            ret = LDEC_ERROR_OK;
            break;
        }
        case LDEC_ERR_LBS_UNDERFLOW:
        {
            ret = LDEC_ERROR_LBS_UNDERFLOW;
            break;
        }
        case LDEC_ERR_NO_FRAMEBUFFER:
        {
            ret = LDEC_ERROR_NO_FRAMEBUFFER;
            break;
        }
        case LDEC_ERR_DEC_SUBTITLE:
        {
            ret = LDEC_ERROR_FAIL;
            break;
        }
        case LDEC_ERR_LFMT_ERR:
        {
            ret = LDEC_ERROR_LFMT_ERR;
            break;
        }
        default:
        {
            WARNING("impossible![%x]\n", nLibRet);
            ret = nLibRet;
            break;
        }
    }
    return ret;
}

/*******************************************************************************
Function name: CedarItxtDec_Open,  Impl_ItxtDec_Open
Description: 
    1. 给lbsbuf和framequeue分配内存
Parameters: 
    
Return: 
    
Time: 2011/9/26
*******************************************************************************/
__s32 CedarItxtDec_Open(CedarIsubtitleDec *thiz)
{
    CedarItxtDec *pItxtDecOal = (CedarItxtDec*)thiz;
    __s32   tmpret;
    tmpret = pItxtDecOal->pItxtDec->open(pItxtDecOal->pItxtDec);
    if(tmpret != LDEC_ERR_NONE)
    {
        WARNING("ItxtDec open fail[%x]\n", tmpret);
        return LDEC_ERROR_FAIL;
    }
    else
    {
        return LDEC_ERROR_OK;
    }
}

__s32 CedarItxtDec_Close(CedarIsubtitleDec *thiz)
{
    CedarItxtDec *pItxtDecOal = (CedarItxtDec*)thiz;
    __s32   tmpret;
    tmpret = pItxtDecOal->pItxtDec->close(pItxtDecOal->pItxtDec);
    if(tmpret != LDEC_ERR_NONE)
    {
        WARNING("ItxtDec close fail[%x]\n", tmpret);
        return LDEC_ERROR_FAIL;
    }
    else
    {
        return LDEC_ERROR_OK;
    }
}

__s32 CedarItxtDec_IoCtrl(CedarIsubtitleDec *thiz, __s32 cmd, __s32 aux, void *pbuffer)
{
    __s32   ret = EPDK_FAIL;
    CedarItxtDec *pItxtDecOal = (CedarItxtDec*)thiz;
    __s32   nItxtDecCmd;
    switch(cmd)
    {
        case CEDARISUBTITLEDEC_CMD_SET_FRMRATE:
        {
            nItxtDecCmd = ITXTDEC_CMD_SET_FRMRATE;
            break;
        }
        case CEDARISUBTITLEDEC_CMD_SET_FRMSIZE:
        {
            nItxtDecCmd = ITXTDEC_CMD_SET_FRMSIZE;
            break;
        }
        case CEDARISUBTITLEDEC_CMD_SET_SCREEN_SIZE:
        {
            nItxtDecCmd = ITXTDEC_CMD_SET_SCREEN_SIZE;
            break;
        }
        case CEDARISUBTITLEDEC_CMD_SET_SUBTITLE_SOURCE:
        {
            nItxtDecCmd = ITXTDEC_CMD_SET_SUBTITLE_SOURCE;
            break;
        }
        case CEDARISUBTITLEDEC_CMD_NOTIFY_SYNCSTAT:
        {
            nItxtDecCmd = ITXTDEC_CMD_NOTIFY_SYNCSTAT;
            break;
        }
        case CEDARISUBTITLEDEC_CMD_CHECK_SYNCSTAT:
        {
            nItxtDecCmd = ITXTDEC_CMD_CHECK_SYNCSTAT;
            break;
        }
        default:
        {
            WARNING("Unknown cmd[%x]\n", cmd);
            return EPDK_FAIL;
        }
    }
    ret = pItxtDecOal->pItxtDec->ItxtDecIoCtrl(pItxtDecOal->pItxtDec, nItxtDecCmd, aux, pbuffer);
    if(ret != LDEC_ERR_NONE)
    {
        return LDEC_ERROR_FAIL;
    }
    else
    {
        return LDEC_ERROR_OK;
    }
}

__s32 CedarItxtDec_RequestWrite(CedarIsubtitleDec *thiz, __u32 uSizeRequested, __u8 **ppData0, __u32 *puSizeGot0, __u8 **ppData1, __u32 *puSizeGot1)
{
    __s32   tmpret;
    CedarItxtDec *pItxtDecOal = (CedarItxtDec*)thiz;
    tmpret = pItxtDecOal->pItxtDec->RequestWrite(pItxtDecOal->pItxtDec, uSizeRequested, ppData0, puSizeGot0, ppData1, puSizeGot1);
    if(tmpret != LDEC_ERR_NONE)
    {
        return LDEC_ERROR_FAIL;
    }
    else
    {
        return LDEC_ERROR_OK;
    }
}

__s32 CedarItxtDec_UpdatePointer(CedarIsubtitleDec *thiz, __lbs_datctlblk_t *pLbsDataCtrlBlk,__u32 uDataSize)
{
    __s32   tmpret;
    CedarItxtDec *pItxtDecOal = (CedarItxtDec*)thiz;
    tmpret = pItxtDecOal->pItxtDec->UpdatePointer(pItxtDecOal->pItxtDec, pLbsDataCtrlBlk, uDataSize);
    if(tmpret != LDEC_ERR_NONE)
    {
        return LDEC_ERROR_FAIL;
    }
    else
    {
        return LDEC_ERROR_OK;
    }
}

__cedar_subtitle_item_t* CedarItxtDec_GetLrcItem(CedarIsubtitleDec *thiz, __u32 nPts, __s32 *pRedecodeFlag)    //ret = pOutputItem, NULL表示没有找到, *pRedecodeFlag表示是否需要重新解码
{
    __s32 tmpret;
    CedarItxtDec *pItxtDecOal = (CedarItxtDec*)thiz;
    if(0 != pItxtDecOal->outputItem.uPts)   //说明已经拷贝过一个字幕帧到outputItem里了, 那么先查看该帧是否符合要求.
    {
        if(nPts >= pItxtDecOal->outputItem.uPts && nPts < pItxtDecOal->outputItem.uPts + pItxtDecOal->outputItem.uDuration)
        {
            //WARNING("same nPts[%d], [%d][%d]\n", nPts, pItxtDecOal->outputItem.uPts, pItxtDecOal->outputItem.uDuration);
            *pRedecodeFlag = 0;
            return &pItxtDecOal->outputItem;
        }
    }
    tmpret = pItxtDecOal->pItxtDec->GetLrcItem(pItxtDecOal->pItxtDec, nPts, &pItxtDecOal->outputItem, pItxtDecOal->nOutputSubtitleSize);
    if(pItxtDecOal->pItxtDec->ldecManager.validSize < LDEC_MAX_TXT_FRM_NUM) //如果有空闲帧
    {
        *pRedecodeFlag = 1;
    }
    else
    {
        *pRedecodeFlag = 0;
    }
    if(LDEC_ERR_NONE == tmpret) //找到了
    {
        return &pItxtDecOal->outputItem;
    }
    else
    {
        return NULL;
    }
}
/*******************************************************************************
Function name: ItxtDec_convert_cedar_subtitle_item_2_CCedarSubtitleFrame
Description: 
    1. ItxtDec, pItxtDecOal的item和frame是共用字幕内存的.
Parameters: 
    
Return: 
    
Time: 2012/5/30
*******************************************************************************/
__s32 ItxtDec_convert_cedar_subtitle_item_2_CCedarSubtitleFrame(CCedarSubtitleFrame *pFrame, __cedar_subtitle_item_t *pItem)
{
    //CCedarSubtitleFrame *pFrame = &pItxtDecOal->outputSubtitleFrame;
    //__cedar_subtitle_item_t *pItem = &pItxtDecOal->outputItem;

    pFrame->isFull = 1;
    pFrame->subNum = 1;
    
    pFrame->position_flags = 0;
    //pFrame->Clear_screen_flags = 0;
    
    pFrame->uPts = pItem->uPts;
    pFrame->uDuration = pItem->uDuration;
    pFrame->uDataType = pItem->uDataType;

    pFrame->colorCode = pItem->colorCode;
    pFrame->contrast = pItem->contrast;
                        
    pFrame->displayMode = MAPLE_SUB_DISPLAY_MODE_MOBILE;
    pFrame->subColor[0] = MAPLE_SUB_COLOR_WHITE;
    pFrame->subStyleSize[0] = MAPLE_SUB_STYLE_NORMAL_SIZE;
        
    pFrame->subSize = 0;
    pFrame->subWidth = pItem->nPicWidth;
    pFrame->subHeight = pItem->nPicHeight;
    pFrame->startX = 0;
    pFrame->startY = 0;

    pFrame->line[0] = 0;
    pFrame->forward[0] = 0;
    pFrame->videoWidth = 0;
    pFrame->videoHeight = 0;
    
    pFrame->subDataLen[0] = pItem->uSubSize;
    if(pFrame->subData[0] != pItem->pSubtitle)
    {
        WARNING("fatal error! Itxt, subData[0] [0x%x]== pSubtitle [0x%x]!\n", pFrame->subData[0], pItem->pSubtitle);
    }
    //RECT                subClearScreen[10];    //subtitle clear screen;
    return EPDK_OK;
}

CCedarSubtitleFrame* CedarItxtDec_GetSubtitleFrame(CedarIsubtitleDec *thiz, __u32 nPts, __s32 *pRedecodeFlag)    //ret = pOutputItem, NULL表示没有找到, *pRedecodeFlag表示是否需要重新解码
{
    CedarItxtDec *pItxtDecOal = (CedarItxtDec*)thiz;
    __cedar_subtitle_item_t *poutItem;
    poutItem = thiz->GetLrcItem(thiz, nPts, pRedecodeFlag);
    if(poutItem)
    {
        ItxtDec_convert_cedar_subtitle_item_2_CCedarSubtitleFrame(&pItxtDecOal->outputSubtitleFrame, poutItem);
        return &pItxtDecOal->outputSubtitleFrame;
    }
    else
    {
        return NULL;
    }
}

__s32 CedarItxtDec_Decode(CedarIsubtitleDec *thiz)   //字幕解码
{
    __s32   ret;
    __s32   tmpret;
    __s32   nSyncStatFlag;
    CedarItxtDec *pItxtDecOal = (CedarItxtDec*)thiz;
    nSyncStatFlag = pItxtDecOal->pItxtDec->ItxtDecIoCtrl(pItxtDecOal->pItxtDec, ITXTDEC_CMD_CHECK_SYNCSTAT, 0, NULL);
    tmpret = pItxtDecOal->pItxtDec->CommonDecode(pItxtDecOal->pItxtDec);
    ret = convertLdecResult_Lib2Cedar(tmpret);
    if(nSyncStatFlag)
    {
        WARNING("ItxtDecOal will clear output frame!\n");
        pItxtDecOal->outputItem.uPts = 0;
    }
    return ret;
}

__s32 CedarItxtDec_CheckSupport(CedarIsubtitleDec *thiz, __s32 nFileFmt)   //ret = EPDK_TRUE, EPDK_FALSE, nFileFmt = enum __CEDARLIB_LYRIC_FILE_FMT
{
    CedarItxtDec *pItxtDecOal = (CedarItxtDec*)thiz;
    return pItxtDecOal->pItxtDec->CheckSupport(pItxtDecOal->pItxtDec, nFileFmt);
}

void CedarItxtDec_Destruct(CedarIsubtitleDec *thiz)
{
    CedarItxtDec *pItxtDecOal = (CedarItxtDec*)thiz;
    deleteCedarItxtDec(pItxtDecOal);
}

/*******************************************************************************
Function name: CedarItxtDec_Init
Description: 
    1.outputItem的buffer分配也放在这里
Parameters: 
    
Return: 
    
Time: 2011/10/17
*******************************************************************************/
__s32 CedarItxtDec_Init(CedarItxtDec *thiz, __s32 nFileFmt)   //LYRIC_ITXT等
{
    __s32   ret;
    CedarItxtDec *pItxtDecOal = thiz;
    memset(thiz, 0, sizeof(CedarItxtDec));
    //先初始化基类
    ret = CedarIsubtitleDec_Init(&thiz->base, nFileFmt);
    if(LDEC_ERROR_OK != ret)
    {
        WARNING("CedarIsubtitleDec Init fail\n");
        goto _err0;
    }
    //覆盖虚函数
    thiz->base.open = CedarItxtDec_Open;
    thiz->base.close = CedarItxtDec_Close;
    thiz->base.CedarISubtitleDecIoCtrl = CedarItxtDec_IoCtrl;
    thiz->base.RequestWrite = CedarItxtDec_RequestWrite;
    thiz->base.UpdatePointer = CedarItxtDec_UpdatePointer;
    thiz->base.GetLrcItem = CedarItxtDec_GetLrcItem;
    thiz->base.decode = CedarItxtDec_Decode;
    thiz->base.CheckSupport = CedarItxtDec_CheckSupport;
    thiz->base.Destruct = CedarItxtDec_Destruct;
    thiz->base.GetSubtitleFrame = CedarItxtDec_GetSubtitleFrame;
    
    //初始化子类变量
    switch(nFileFmt)
    {
        case LYRIC_ITXT:
        {
            pItxtDecOal->nOutputSubtitleSize = ITEXTDEC_MAX_FRM_SIZE;
            break;
        }
        case LYRIC_ISSA:
        {
            pItxtDecOal->nOutputSubtitleSize = ISSADEC_MAX_FRM_SIZE;
            break;
        }
        case LYRIC_ISUB:
        {
            pItxtDecOal->nOutputSubtitleSize = LDEC_MAX_TXT_FRM_SIZE;
            break;
        }
        default:
        {
            WARNING("fatal error! Itxt meet type[%x]\n", pItxtDecOal->base.subtitle_type);
            pItxtDecOal->nOutputSubtitleSize = LDEC_MAX_TXT_FRM_SIZE;
            break;
        }
    }
    pItxtDecOal->outputItem.pSubtitle = (__u8 *)PHY_MALLOC(sizeof(__u8)*pItxtDecOal->nOutputSubtitleSize, 1024);
    if(pItxtDecOal->outputItem.pSubtitle == NULL)
    {
        WARNING("Itxt malloc outputItem pSubtitlesize[%d] fail\n", pItxtDecOal->nOutputSubtitleSize);
        goto _err1;
    }
    pItxtDecOal->outputSubtitleFrame.subData[0] = pItxtDecOal->outputItem.pSubtitle;
    pItxtDecOal->pItxtDec = ItxtDec_Create(nFileFmt);
    if(NULL == pItxtDecOal->pItxtDec)
    {
        WARNING("malloc ItxtDec fail\n");
        goto _err2;
    }
    //这里用真正解码类的字符串赋值适配类基类的字符串.
    Isubtitle_strcpy(pItxtDecOal->base.strDecName, strDecNameSize, pItxtDecOal->pItxtDec->strDecName);
    return LDEC_ERROR_OK;

_err2:
    PHY_FREE(pItxtDecOal->outputItem.pSubtitle);
    pItxtDecOal->outputItem.pSubtitle = NULL;
_err1:
    CedarIsubtitleDec_Exit(&thiz->base);
_err0:
    return LDEC_ERROR_FAIL;

}
__s32 CedarItxtDec_Exit(CedarItxtDec *thiz)
{
    //__s32   ret;
    //因为会覆盖基类元素的缘故. 一般先释放自己的,再处理基类的.
    if(thiz->pItxtDec)
    {
        ItxtDec_Destroy(thiz->pItxtDec);
    }
    else
    {
        WARNING("fatal error! thiz->pItxtDec = NULL!\n");
    }
    if(thiz->outputItem.pSubtitle)
    {
        PHY_FREE(thiz->outputItem.pSubtitle);
        thiz->outputItem.pSubtitle = NULL;
        thiz->outputSubtitleFrame.subData[0] = NULL; //因为现在设计为共用内存.
    }
    CedarIsubtitleDec_Exit(&thiz->base);
    return LDEC_ERROR_OK;
}
CedarItxtDec* newCedarItxtDec(__s32 nFileFmt)   //LYRIC_ITXT等
{
    __s32   tmpret;
    CedarItxtDec *pItxtDecOal;
    pItxtDecOal = (CedarItxtDec*)malloc(sizeof(CedarItxtDec));
    if(NULL == pItxtDecOal)
    {
        WARNING("malloc CedarItxtDec fail\n");
        return NULL;
    }
    tmpret = CedarItxtDec_Init(pItxtDecOal, nFileFmt);
    if(tmpret != LDEC_ERROR_OK)
    {
        WARNING("CedarItxtDec init fail\n");
        goto _err0;
    }
    return pItxtDecOal;
    
_err0:
    free(pItxtDecOal);
    return NULL;
    
}
void deleteCedarItxtDec(CedarItxtDec *thiz)
{
    CedarItxtDec_Exit(thiz);
    free(thiz);
}

