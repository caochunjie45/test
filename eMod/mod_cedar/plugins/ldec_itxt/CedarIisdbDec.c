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
#if PLAY_ISDBT_CLOSECAPTION
#include "ldec_itxt_s.h"
#include "CedarIisdbDec.h"

static __s32 convertLdecResult_Lib2Cedar(__s32 nLibRet)
{
    __s32   ret;
    switch(nLibRet)
    {
        case LDEC_ERR_NONE:
        case LDEC_ERR_IGNORE:
        {
            ret = LDEC_ERROR_OK;
            break;
        }
        case LDEC_LBS_EMPTY:
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
Function name: CedarIisdbDec_Open,  Impl_ItxtDec_Open
Description: 
    1. 给lbsbuf和framequeue分配内存
Parameters: 
    
Return: 
    
Time: 2011/9/26
*******************************************************************************/
__s32 CedarIisdbDec_Open(CedarIsubtitleDec *thiz)
{
    CedarIisdbDec *pIsdbDecOal = (CedarIisdbDec*)thiz;
    __s32   tmpret;
    tmpret = pIsdbDecOal->pIsdbDec->open(pIsdbDecOal->pIsdbDec);
    if(tmpret != LDEC_ERR_NONE)
    {
        WARNING("IsdbDec open fail[%x]\n", tmpret);
        return LDEC_ERROR_FAIL;
    }
    else
    {
        return LDEC_ERROR_OK;
    }
}

__s32 CedarIisdbDec_Close(CedarIsubtitleDec *thiz)
{
    CedarIisdbDec *pIsdbDecOal = (CedarIisdbDec*)thiz;
    __s32   tmpret;
    tmpret = pIsdbDecOal->pIsdbDec->close(pIsdbDecOal->pIsdbDec);
    if(tmpret != LDEC_ERR_NONE)
    {
        WARNING("IsdbDec close fail[%x]\n", tmpret);
        return LDEC_ERROR_FAIL;
    }
    else
    {
        return LDEC_ERROR_OK;
    }
}

__s32 CedarIisdbDec_IoCtrl(CedarIsubtitleDec *thiz, __s32 cmd, __s32 aux, void *pbuffer)
{
    __s32   ret = EPDK_FAIL;
    CedarIisdbDec *pIsdbDecOal = (CedarIisdbDec*)thiz;
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
        case CEDARISUBTITLEDEC_CMD_SET_ONESEGFLAG:
        {
            nItxtDecCmd = ITXTDEC_CMD_SET_ONESEGFLAG;
            break;
        }
        case CEDARISUBTITLEDEC_CMD_SET_CHAR_CODING_TYPE:
        {
            nItxtDecCmd = ITXTDEC_CMD_SET_CHAR_CODING_TYPE;
            break;
        }
        default:
        {
            WARNING("Unknown cmd[%x]\n", cmd);
            return EPDK_FAIL;
        }
    }
    ret = pIsdbDecOal->pIsdbDec->IsdbDecIoCtrl(pIsdbDecOal->pIsdbDec, nItxtDecCmd, aux, pbuffer);
    if(ret != LDEC_ERR_NONE)
    {
        return LDEC_ERROR_FAIL;
    }
    else
    {
        return LDEC_ERROR_OK;
    }
}

__s32 CedarIisdbDec_RequestWrite(CedarIsubtitleDec *thiz, __u32 uSizeRequested, __u8 **ppData0, __u32 *puSizeGot0, __u8 **ppData1, __u32 *puSizeGot1)
{
    __s32   tmpret;
    CedarIisdbDec *pIsdbDecOal = (CedarIisdbDec*)thiz;
    tmpret = pIsdbDecOal->pIsdbDec->RequestWrite(pIsdbDecOal->pIsdbDec, uSizeRequested, ppData0, puSizeGot0, ppData1, puSizeGot1);
    if(tmpret != LDEC_ERR_NONE)
    {
        return LDEC_ERROR_FAIL;
    }
    else
    {
        return LDEC_ERROR_OK;
    }
}

__s32 CedarIisdbDec_UpdatePointer(CedarIsubtitleDec *thiz, __lbs_datctlblk_t *pLbsDataCtrlBlk,__u32 uDataSize)
{
    __s32   tmpret;
    CedarIisdbDec *pIsdbDecOal = (CedarIisdbDec*)thiz;
    tmpret = pIsdbDecOal->pIsdbDec->UpdatePointer(pIsdbDecOal->pIsdbDec, pLbsDataCtrlBlk, uDataSize);
    if(tmpret != LDEC_ERR_NONE)
    {
        return LDEC_ERROR_FAIL;
    }
    else
    {
        return LDEC_ERROR_OK;
    }
}

/*******************************************************************************
Function name: IsdbDec_convert_CCedarSubtitleFrame_2_cedar_subtitle_item
Description: 
    1. IsdbDec, pIsdbDecOal的item和frame不共用字幕内存.因此需要拷贝.
Parameters: 
    nOutputSubtitleSize: pItem的pSubtitle的buffer的总长度.
Return: 
    
Time: 2012/5/30
*******************************************************************************/
__s32 IsdbDec_convert_CCedarSubtitleFrame_2_cedar_subtitle_item(__cedar_subtitle_item_t *pItem, __s32 nOutputSubtitleSize, CCedarSubtitleFrame *pFrame)
{
    __s32   i;
    __s32   tmpLen;
    if(0 == pFrame->isFull)
    {
        WARNING("fatal error! isdb subtitle can't isFull = 0\n");
    }
    if(0 == pFrame->subNum)
    {
        WARNING("fatal error! isdb subtitle can't subNum = 0\n");
    }
    if(pFrame->subNum > ISDBDEC_MAX_LINE_NUM)
    {
        WARNING("fatal error! isdb subtitle can't has %d lines!\n", pFrame->subNum);
        pFrame->subNum = ISDBDEC_MAX_LINE_NUM;
    }
    pItem->uPts = pFrame->uPts;
    pItem->uDuration = pFrame->uDuration;
    pItem->uDataType = pFrame->uDataType;
    pItem->nPicWidth = pFrame->subWidth;    //isdb的这一项对于老方式来说没有意义
    pItem->nPicHeight= pFrame->subHeight;   //isdb的这一项对于老方式来说没有意义
    pItem->colorCode = pFrame->colorCode;   //isdb一般是字符串,这一项无意义.
    pItem->contrast = pFrame->contrast;     //isdb一般是字符串,这一项无意义.
    pItem->uPtsSize = 0;
    pItem->pPtsOffset = NULL;
    
    pItem->uSubSize = 0;
    for(i=0;i<pFrame->subNum;i++)
    {
        pItem->uSubSize += pFrame->subDataLen[i];
    }
    
    if(pItem->uSubSize > nOutputSubtitleSize - 1)
    {
        WARNING("fatal error! isdb string is too long, [%d] > [%d]-1\n", pItem->uSubSize, nOutputSubtitleSize);
        pItem->uSubSize = 0;
        memset(pItem->pSubtitle, 0, nOutputSubtitleSize);
    }
    else
    {
        tmpLen = 0;
        for(i=0;i<pFrame->subNum;i++)
        {
            memcpy(pItem->pSubtitle + tmpLen, pFrame->subData[i], pFrame->subDataLen[i]);
            tmpLen += pFrame->subDataLen[i];
        }
        pItem->pSubtitle[pItem->uSubSize] = 0;
    }
    
    return EPDK_OK;
}

__cedar_subtitle_item_t* CedarIisdbDec_GetLrcItem(CedarIsubtitleDec *thiz, __u32 nPts, __s32 *pRedecodeFlag)    //ret = pOutputItem, NULL表示没有找到, *pRedecodeFlag表示是否需要重新解码
{
    CedarIisdbDec *pIsdbDecOal = (CedarIisdbDec*)thiz;
    CCedarSubtitleFrame *poutFrame;
    poutFrame = thiz->GetSubtitleFrame(thiz, nPts, pRedecodeFlag);
    if(poutFrame)
    {
        IsdbDec_convert_CCedarSubtitleFrame_2_cedar_subtitle_item(&pIsdbDecOal->outputItem, pIsdbDecOal->nOutputSubtitleSize, poutFrame);
        return &pIsdbDecOal->outputItem;
    }
    else
    {
        return NULL;
    }
}

CCedarSubtitleFrame* CedarIisdbDec_GetSubtitleFrame(CedarIsubtitleDec *thiz, __u32 nPts, __s32 *pRedecodeFlag)    //ret = pOutputItem, NULL表示没有找到, *pRedecodeFlag表示是否需要重新解码
{
    __s32 tmpret;
    CedarIisdbDec *pIsdbDecOal = (CedarIisdbDec*)thiz;
    if(0 != pIsdbDecOal->outputSubtitleFrame.uPts)   //说明已经拷贝过一个字幕帧到outputItem里了, 那么先查看该帧是否符合要求.
    {
//        if(nPts >= pItxtDecOal->outputItem.uPts && nPts < pItxtDecOal->outputItem.uPts + pItxtDecOal->outputItem.uDuration)
//        {
            //WARNING("same nPts[%d], [%d][%d]\n", nPts, pItxtDecOal->outputItem.uPts, pItxtDecOal->outputItem.uDuration);
//            *pRedecodeFlag = 0;
//            return &pItxtDecOal->outputItem;
//        }
    }

    
    tmpret = pIsdbDecOal->pIsdbDec->GetLrcItem(pIsdbDecOal->pIsdbDec, nPts, &pIsdbDecOal->outputSubtitleFrame, pIsdbDecOal->nOutputSubtitleSize);
    if(pIsdbDecOal->pIsdbDec->ldecManager.validSize < LDEC_MAX_TXT_FRM_NUM) //如果有空闲帧
    {
        *pRedecodeFlag = 1;
    }
    else
    {
        *pRedecodeFlag = 0;
    }
    if(LDEC_ERR_NONE == tmpret) //找到了
    {
        if(pIsdbDecOal->outputSubtitleFrame.isFull)
        {
            return &pIsdbDecOal->outputSubtitleFrame;
        }
        else
        {
            WARNING("ReqSubtitle: ISDB empty subtitle!\n");
            return NULL;
        }
    }
    else
    {
        return NULL;
    }
}

__s32 CedarIisdbDec_Decode(CedarIsubtitleDec *thiz)   //字幕解码
{
    __s32   ret;
    __s32   tmpret;
    __s32   nSyncStatFlag;
    CedarIisdbDec *pIsdbDecOal = (CedarIisdbDec*)thiz;
    nSyncStatFlag = pIsdbDecOal->pIsdbDec->IsdbDecIoCtrl(pIsdbDecOal->pIsdbDec, ITXTDEC_CMD_CHECK_SYNCSTAT, 0, NULL);
    tmpret = pIsdbDecOal->pIsdbDec->CommonDecode(pIsdbDecOal->pIsdbDec);
    ret = convertLdecResult_Lib2Cedar(tmpret);
    if(nSyncStatFlag)
    {
        WARNING("pIsdbDecOal will clear output frame!\n");
        pIsdbDecOal->outputItem.uPts = 0;
        pIsdbDecOal->outputSubtitleFrame.uPts = 0;
    }
    return ret;
}

__s32 CedarIisdbDec_CheckSupport(CedarIsubtitleDec *thiz, __s32 nFileFmt)   //ret = EPDK_TRUE, EPDK_FALSE, nFileFmt = enum __CEDARLIB_LYRIC_FILE_FMT
{
    CedarIisdbDec *pIsdbDecOal = (CedarIisdbDec*)thiz;
    return pIsdbDecOal->pIsdbDec->CheckSupport(pIsdbDecOal->pIsdbDec, nFileFmt);
}

void CedarIisdbDec_Destruct(CedarIsubtitleDec *thiz)
{
    CedarIisdbDec *pIsdbDecOal = (CedarIisdbDec*)thiz;
    deleteCedarIisdbDec(pIsdbDecOal);
}

/*******************************************************************************
Function name: CedarIisdbDec_Init
Description: 
    1.outputItem的buffer分配也放在这里
Parameters: 
    
Return: 
    
Time: 2011/10/17
*******************************************************************************/
__s32 CedarIisdbDec_Init(CedarIisdbDec *thiz)   //LYRIC_ITXT等
{
    __s32   i;
    __s32   ret;
    CedarIisdbDec *pIsdbDecOal = thiz;
    memset(thiz, 0, sizeof(CedarIisdbDec));
    //先初始化基类
    ret = CedarIsubtitleDec_Init(&thiz->base, LYRIC_ISDB);
    if(LDEC_ERROR_OK != ret)
    {
        WARNING("CedarIsubtitleDec Init fail\n");
        goto _err0;
    }
    //覆盖虚函数
    thiz->base.open = CedarIisdbDec_Open;
    thiz->base.close = CedarIisdbDec_Close;
    thiz->base.CedarISubtitleDecIoCtrl = CedarIisdbDec_IoCtrl;
    thiz->base.RequestWrite = CedarIisdbDec_RequestWrite;
    thiz->base.UpdatePointer = CedarIisdbDec_UpdatePointer;
    thiz->base.GetLrcItem = CedarIisdbDec_GetLrcItem;
    thiz->base.GetSubtitleFrame = CedarIisdbDec_GetSubtitleFrame;
    thiz->base.decode = CedarIisdbDec_Decode;
    thiz->base.CheckSupport = CedarIisdbDec_CheckSupport;
    thiz->base.Destruct = CedarIisdbDec_Destruct;
    
    //初始化子类变量
    pIsdbDecOal->nOutputSubtitleSize = ISDBDEC_MAX_FRM_SIZE;    //LDEC_MAX_TXT_FRM_SIZE
    pIsdbDecOal->outputItem.pSubtitle = (__u8 *)PHY_MALLOC(sizeof(__u8)*pIsdbDecOal->nOutputSubtitleSize, 1024);
    if(pIsdbDecOal->outputItem.pSubtitle == NULL)
    {
        WARNING("Iisdb malloc outputItem pSubtitlesize[%d] fail\n", pIsdbDecOal->nOutputSubtitleSize);
        goto _err1;
    }
    pIsdbDecOal->pIsdbDec = newIsdbDec();
    if(NULL == pIsdbDecOal->pIsdbDec)
    {
        WARNING("malloc IsdbDec fail\n");
        goto _err2;
    }
    for(i=0; i<ISDBDEC_MAX_LINE_NUM; i++)
    {
        pIsdbDecOal->outputSubtitleFrame.subData[i]= (__u8 *)malloc(ISDBDEC_MAX_FRM_SIZE);
        if (pIsdbDecOal->outputSubtitleFrame.subData[i] == NULL)
        {
            WARNING("malloc buffer for outputSubtitleFrame is fail!\n");
            goto _err3;
        }
    }
    pIsdbDecOal->outputSubtitleFrame.pSubtitle = (__u8 *)PHY_MALLOC(sizeof(__u8)*pIsdbDecOal->nOutputSubtitleSize, 1024);
    if(pIsdbDecOal->outputSubtitleFrame.pSubtitle == NULL)
    {
        WARNING("Iisdb malloc outputSubtitleFrame pSubtitlesize[%d] fail\n", pIsdbDecOal->nOutputSubtitleSize);
        goto _err3;
    }
    //这里用真正解码类的字符串赋值适配类基类的字符串.
    Isubtitle_strcpy(pIsdbDecOal->base.strDecName, strDecNameSize, pIsdbDecOal->pIsdbDec->strDecName);
    return LDEC_ERROR_OK;

_err3:
    for(i=0; i<ISDBDEC_MAX_LINE_NUM; i++)
    {
        if (pIsdbDecOal->outputSubtitleFrame.subData[i] != NULL )
        {
            free(pIsdbDecOal->outputSubtitleFrame.subData[i]);
            pIsdbDecOal->outputSubtitleFrame.subData[i] = NULL;
        }
    }
_err2:
    PHY_FREE(pIsdbDecOal->outputItem.pSubtitle);
    pIsdbDecOal->outputItem.pSubtitle = NULL;
_err1:
    CedarIsubtitleDec_Exit(&thiz->base);
_err0:
    return LDEC_ERROR_FAIL;
}

__s32 CedarIisdbDec_Exit(CedarIisdbDec *thiz)
{
    __s32 i;
    //__s32   ret;
    //因为会覆盖基类元素的缘故. 一般先释放自己的,再处理基类的.
    if(thiz->pIsdbDec)
    {
        deleteIsdbDec(thiz->pIsdbDec);
    }
    else
    {
        WARNING("fatal error! thiz->pIsdbDec = NULL!\n");
    }
    if(thiz->outputItem.pSubtitle)
    {
        PHY_FREE(thiz->outputItem.pSubtitle);
        thiz->outputItem.pSubtitle = NULL;
    }
	if(thiz->outputSubtitleFrame.pSubtitle)
    {
        PHY_FREE(thiz->outputSubtitleFrame.pSubtitle);
        thiz->outputSubtitleFrame.pSubtitle = NULL;
    }
    for(i=0; i<ISDBDEC_MAX_LINE_NUM; i++)
    {
        if (thiz->outputSubtitleFrame.subData[i] != NULL )
        {
            free(thiz->outputSubtitleFrame.subData[i]);
            thiz->outputSubtitleFrame.subData[i] = NULL;
        }
    }
    CedarIsubtitleDec_Exit(&thiz->base);
    return LDEC_ERROR_OK;
}

CedarIisdbDec* newCedarIisdbDec() //LYRIC_ITXT等
{
    __s32   tmpret;
    CedarIisdbDec *pIsdbDecOal;
    pIsdbDecOal = (CedarIisdbDec*)malloc(sizeof(CedarIisdbDec));
    if(NULL == pIsdbDecOal)
    {
        WARNING("malloc CedarIisdbDec fail\n");
        return NULL;
    }
    tmpret = CedarIisdbDec_Init(pIsdbDecOal);
    if(tmpret != LDEC_ERROR_OK)
    {
        WARNING("CedarIisdbDec init fail\n");
        goto _err0;
    }
    return pIsdbDecOal;
    
_err0:
    free(pIsdbDecOal);
    return NULL;
}
void deleteCedarIisdbDec(CedarIisdbDec *thiz)
{
    CedarIisdbDec_Exit(thiz);
    free(thiz);
}


#endif
