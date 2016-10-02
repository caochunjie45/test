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
* File   : CedarIpgsDec.c
* Version: V1.0
* By     : Eric_wang
* Date   : 2011-10-16
* Description:
********************************************************************************
*/
#include "ldec_itxt_s.h"
#include "CedarIpgsDec.h"

static void scaleDownSubtitle(__u8* pSrc, __u32 srcWidth, __u32 srcHeight,
                              __u8* pDst, __u32 widthRatio, __u32 heightRatio)
{
    __u32 i;
    __u32 j;
    __u32 k;
    __u8* ptr;
    __u32 newWidth;
    
    newWidth = srcWidth / widthRatio;
    
    for(i=0, k=0; i<srcHeight; i+=heightRatio, k++)
    {
        ptr = pDst + k*newWidth;
        for(j=0; j<srcWidth; j+=widthRatio)
        {
            *ptr++ = pSrc[i*srcWidth + j];
        }
    }
    
    return;
}

/*******************************************************************************
Function name: CedarItxtDec_Open,  Impl_ItxtDec_Open
Description: 
    1. 给lbsbuf和framequeue分配内存
Parameters: 
    
Return: 
    
Time: 2011/9/26
*******************************************************************************/
__s32 CedarIpgsDec_Open(CedarIsubtitleDec *thiz)
{
    CedarIpgsDec *pIpgsDecOal = (CedarIpgsDec*)thiz;
    __s32   tmpret;
    tmpret = pIpgsDecOal->pPgsDec->open(pIpgsDecOal->pPgsDec);
    if(tmpret != EPDK_OK)
    {
        WARNING("PgsDec open fail[%x]\n", tmpret);
        return LDEC_ERROR_FAIL;
    }
    else
    {
        return LDEC_ERROR_OK;
    }
}

__s32 CedarIpgsDec_Close(CedarIsubtitleDec *thiz)
{
    CedarIpgsDec *pIpgsDecOal = (CedarIpgsDec*)thiz;
    __s32   tmpret;
    tmpret = pIpgsDecOal->pPgsDec->close(pIpgsDecOal->pPgsDec);
    if(tmpret != EPDK_OK)
    {
        WARNING("PgsDec close fail[%x]\n", tmpret);
        return LDEC_ERROR_FAIL;
    }
    else
    {
        return LDEC_ERROR_OK;
    }
}

__s32 CedarIpgsDec_IoCtrl(CedarIsubtitleDec *thiz, __s32 cmd, __s32 aux, void *pbuffer)
{
    __s32   ret = LDEC_ERROR_FAIL;
    CedarIpgsDec *pIpgsDecOal = (CedarIpgsDec*)thiz;
    switch(cmd)
    {
        case CEDARISUBTITLEDEC_CMD_SET_SCREEN_SIZE:
        {
            pIpgsDecOal->screenWidth  = aux>>16;
            pIpgsDecOal->screenHeight = aux & 0xffff;
            ret = LDEC_ERROR_OK;
            break;
        }
        case CEDARISUBTITLEDEC_CMD_SET_SUBTITLE_SOURCE:
        {
            pIpgsDecOal->pPgsDec->PgsIoCtrl(pIpgsDecOal->pPgsDec, PGS_CMD_SET_SUBTITLE_SOURCE, aux, pbuffer);
            ret = LDEC_ERROR_OK;
            break;
        }
        case CEDARISUBTITLEDEC_CMD_NOTIFY_SYNCSTAT:
        {
            pIpgsDecOal->pPgsDec->PgsIoCtrl(pIpgsDecOal->pPgsDec, PGS_CMD_NOTIFY_SYNCSTAT, aux, pbuffer);
            ret = LDEC_ERROR_OK;
            break;
        }
        case CEDARISUBTITLEDEC_CMD_CHECK_SYNCSTAT:
        {
            ret = pIpgsDecOal->pPgsDec->PgsIoCtrl(pIpgsDecOal->pPgsDec, PGS_CMD_CHECK_SYNCSTAT, aux, pbuffer);
            break;
        }
        default:
        {
            WARNING("Unknown cmd[%x]\n", cmd);
            ret = LDEC_ERROR_FAIL;
            break;
        }
    }
    return ret;
}

__s32 CedarIpgsDec_RequestWrite(CedarIsubtitleDec *thiz, __u32 uSizeRequested, __u8 **ppData0, __u32 *puSizeGot0, __u8 **ppData1, __u32 *puSizeGot1)
{
    __s32   tmpret;
    CedarIpgsDec *pIpgsDecOal = (CedarIpgsDec*)thiz;
    tmpret = pIpgsDecOal->pPgsDec->RequestWrite(pIpgsDecOal->pPgsDec, uSizeRequested, ppData0, puSizeGot0, ppData1, puSizeGot1);
    if(tmpret != PGS_ERR_NONE)
    {
        return LDEC_ERROR_FAIL;
    }
    else
    {
        return LDEC_ERROR_OK;
    }
}

__s32 CedarIpgsDec_UpdatePointer(CedarIsubtitleDec *thiz, __lbs_datctlblk_t *pLbsDataCtrlBlk,__u32 uDataSize)
{
    __s32   tmpret;
    CedarIpgsDec *pIpgsDecOal = (CedarIpgsDec*)thiz;
    tmpret = pIpgsDecOal->pPgsDec->UpdatePointer(pIpgsDecOal->pPgsDec, pLbsDataCtrlBlk, uDataSize);
    if(tmpret != EPDK_OK)
    {
        return LDEC_ERROR_FAIL;
    }
    else
    {
        return LDEC_ERROR_OK;
    }
}

__cedar_subtitle_item_t* CedarIpgsDec_GetLrcItem(CedarIsubtitleDec *thiz, __u32 nPts, __s32 *pRedecodeFlag)    //ret = pOutputItem, NULL表示没有找到, *pRedecodeFlag表示是否需要重新解码
{
    __s32               tmpret;
    __u32               videoWidth = 0;     //指帧的宽度
    __u32               videoHeight = 0;    //帧的高度
    __u32               widthRatio = 0;     //帧的宽度的压缩倍数
    __u32               heightRatio = 0;    //帧的高度的压缩倍数
    __u32               needScaleDown = 0;  //是否需要做scaledown, 压缩倍数大于1就需要做
    __u32               pts = 0;
    subtitle_frame_t    frame;
    CedarIpgsDec        *pIpgsDecOal = (CedarIpgsDec*)thiz;
    memset(&frame, 0, sizeof(subtitle_frame_t));
    
    *pRedecodeFlag = 0;
    //result = SubtitleDecIOCtrl(mp, SUBTITLE_CMD_GET_NEXT_PTS, 0, &pts);
    tmpret = pIpgsDecOal->pPgsDec->PgsIoCtrl(pIpgsDecOal->pPgsDec, PGS_CMD_GET_NEXT_PTS, 0, &pts);
    if(tmpret != EPDK_OK)
    {
        *pRedecodeFlag = 1;
        if(NULL != pIpgsDecOal->outItem.pSubtitle && 0!=pIpgsDecOal->outItem.uPts)   //表示上次已有字幕显示出去,但是其后一段时间都没有字幕?
        {
            WARNING("cur true subitem no next item follow![%d]\n", pIpgsDecOal->outItem.uPts);
            return &pIpgsDecOal->outItem;
        }
        else
        {
            return NULL;
        }
    }
    if(pts >= nPts)
    {
        if(NULL != pIpgsDecOal->outItem.pSubtitle && 0!=pIpgsDecOal->outItem.uPts)   //表示上次已有实字幕显示出去,新的字幕时间又没到,那么仍然显示上个字幕
        {
            return &pIpgsDecOal->outItem;
        }
        else
        {
            return NULL;
        }
    }
    
    do
    {
        //result = SubtitleDecIOCtrl(mp, SUBTITLE_CMD_REQUEST_FRAME, 0, &frame);
        tmpret = pIpgsDecOal->pPgsDec->RequestFrame(pIpgsDecOal->pPgsDec, &frame);
        if(tmpret != 0)
        {
            WARNING("fatal error! request frame fail\n");
            return NULL;
        }
        //WARNING("[%d][%d]\n", (__u32)frame.ptsVal, pts);
//                    if(g_oalCtx->bSubtitleOut == 0 && frame.isFull == 0)
//                    {
//                        WARNING("invalid error frame.\n");
//                        continue;
//                    }
        
        break;
        
    }while(1);
    
    if(frame.isFull == 0) //* get a message to tell outside that the last subtitle is time out.
    {
        WARNING("PGS meet empty frame!\n");
        pIpgsDecOal->outItem.uPts          = 0;  //0;(__u32)frame.ptsVal
        pIpgsDecOal->outItem.uDuration     = 0;
        pIpgsDecOal->outItem.uDataType     = LYRIC_SUB_ENCODE_BITMAP;
        pIpgsDecOal->outItem.pSubtitle     = NULL;
        pIpgsDecOal->outItem.nPicWidth     = 0;
        pIpgsDecOal->outItem.nPicHeight    = 0;
        //g_oalCtx->bSubtitleOut          = 0;
        //SubtitleDecIOCtrl(mp, SUBTITLE_CMD_RELEASE_FRAME, 0, &frame);
        pIpgsDecOal->pPgsDec->ReleaseFrame(pIpgsDecOal->pPgsDec, &frame);
        *pRedecodeFlag = 1;
        return NULL;
    }
    else
    {
        videoWidth  = frame.videoWidth;
        videoHeight = frame.videoHeight;
        widthRatio  = (videoWidth  + (pIpgsDecOal->screenWidth >>1)) / pIpgsDecOal->screenWidth;
        heightRatio = (videoHeight + (pIpgsDecOal->screenHeight>>1)) / pIpgsDecOal->screenHeight;
        
        if(widthRatio == 0)
            widthRatio = 1;
            
        if(heightRatio == 0)
            heightRatio = 1;
            
        if(widthRatio > 1 || heightRatio > 1)
            needScaleDown = 1;
        else
            needScaleDown = 0;
                
        pIpgsDecOal->outItem.uPts          = (__u32)frame.ptsVal;
        pIpgsDecOal->outItem.uDuration     = frame.timeDuration;
        pIpgsDecOal->outItem.uDataType     = LYRIC_SUB_ENCODE_BITMAP;
        
        if(needScaleDown)
        {
            pIpgsDecOal->outItem.nPicWidth     = frame.subWidth / widthRatio;
            pIpgsDecOal->outItem.nPicHeight    = frame.subHeight / heightRatio;
            
            scaleDownSubtitle(frame.pSubtitle, frame.subWidth, frame.subHeight,
                              pIpgsDecOal->pBitmapData, widthRatio, heightRatio);
                            
            pIpgsDecOal->outItem.pSubtitle     = pIpgsDecOal->pBitmapData;
        }
        else    //如果不做scale,就交换内存,避免拷贝
        {
            pIpgsDecOal->outItem.nPicWidth     = frame.subWidth;
            pIpgsDecOal->outItem.nPicHeight    = frame.subHeight;
            pIpgsDecOal->outItem.pSubtitle     = frame.pSubtitle;
            
            frame.pSubtitle                 = pIpgsDecOal->pBitmapData;
            pIpgsDecOal->pBitmapData           = pIpgsDecOal->outItem.pSubtitle;
        }
        
        //g_oalCtx->bSubtitleOut = 1;
        
        //SubtitleDecIOCtrl(mp, SUBTITLE_CMD_RELEASE_FRAME, 0, &frame);
        pIpgsDecOal->pPgsDec->ReleaseFrame(pIpgsDecOal->pPgsDec, &frame);
        *pRedecodeFlag = 1;
    }
    
    return &pIpgsDecOal->outItem;
}

__s32 IpgsDec_convert_cedar_subtitle_item_2_CCedarSubtitleFrame(CCedarSubtitleFrame *pFrame, __cedar_subtitle_item_t *pItem)
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
    pFrame->subData[0] = pItem->pSubtitle;
    //RECT                subClearScreen[10];    //subtitle clear screen;
    return EPDK_OK;
}

CCedarSubtitleFrame* CedarIpgsDec_GetSubtitleFrame(CedarIsubtitleDec *thiz, __u32 nPts, __s32 *pRedecodeFlag)
{
    __cedar_subtitle_item_t *poutItem;
    CedarIpgsDec        *pIpgsDecOal = (CedarIpgsDec*)thiz;
    poutItem = thiz->GetLrcItem(thiz, nPts, pRedecodeFlag);
    if(poutItem)
    {
        IpgsDec_convert_cedar_subtitle_item_2_CCedarSubtitleFrame(&pIpgsDecOal->outputSubtitleFrame, poutItem);
        return &pIpgsDecOal->outputSubtitleFrame;
    }
    else
    {
        return NULL;
    }
}


/*******************************************************************************
Function name: CedarIpgsDec_Decode
Description: 
    1.目前的设计是:必须解出一帧,或者lbs underflow, frame underflow
Parameters: 
    
Return: 
    优先返回lbs underflow.
Time: 2011/10/19
*******************************************************************************/
__s32 CedarIpgsDec_Decode(CedarIsubtitleDec *thiz)   //字幕解码
{
    //__s32   ret;
    __s32   tmpret;
    CedarIpgsDec *pIpgsDecOal = (CedarIpgsDec*)thiz;
    tmpret = pIpgsDecOal->pPgsDec->decode(pIpgsDecOal->pPgsDec);
    if(PGS_ERR_NONE == tmpret)
    {
        return LDEC_ERROR_OK;
    }
    else if(PGS_ERR_LBS_UNDERFLOW == tmpret)
    {
        return LDEC_ERROR_LBS_UNDERFLOW;
    }
    else if(PGS_ERR_NO_FRAMEBUFFER == tmpret)
    {
        return LDEC_ERROR_NO_FRAMEBUFFER;
    }
    else if(PGS_ERR_FAIL == tmpret)
    {
        return LDEC_ERROR_FAIL;
    }
    else
    {
        WARNING("impossible? unknown result[%x]\n!", tmpret);
        return LDEC_ERROR_FAIL;
    }
}

__s32 CedarIpgsDec_CheckSupport(CedarIsubtitleDec *thiz, __s32 nFileFmt)   //ret = EPDK_TRUE, EPDK_FALSE, nFileFmt = enum __CEDARLIB_LYRIC_FILE_FMT
{
    if(LYRIC_PGS == nFileFmt)
    {
        return EPDK_TRUE;
    }
    else
    {
        return EPDK_FALSE;
    }
}

void CedarIpgsDec_Destruct(CedarIsubtitleDec *thiz)
{
    CedarIpgsDec *pIpgsDecOal = (CedarIpgsDec*)thiz;
    deleteCedarIpgsDec(pIpgsDecOal);
}

__s32 CedarIpgsDec_Init(CedarIpgsDec *thiz)
{
    __s32   ret;
    CedarIpgsDec *pIpgsDecOal = thiz;
    memset(thiz, 0, sizeof(CedarIpgsDec));
    //先初始化基类
    ret = CedarIsubtitleDec_Init(&thiz->base, LYRIC_PGS);
    if(LDEC_ERROR_OK != ret)
    {
        WARNING("CedarIsubtitleDec Init fail\n");
        goto _err0;
    }
    //覆盖虚函数
    thiz->base.open = CedarIpgsDec_Open;
    thiz->base.close = CedarIpgsDec_Close;
    thiz->base.CedarISubtitleDecIoCtrl = CedarIpgsDec_IoCtrl;
    thiz->base.RequestWrite = CedarIpgsDec_RequestWrite;
    thiz->base.UpdatePointer = CedarIpgsDec_UpdatePointer;
    thiz->base.GetLrcItem = CedarIpgsDec_GetLrcItem;
    thiz->base.decode = CedarIpgsDec_Decode;
    thiz->base.CheckSupport = CedarIpgsDec_CheckSupport;
    thiz->base.Destruct = CedarIpgsDec_Destruct;
    thiz->base.GetSubtitleFrame = CedarIpgsDec_GetSubtitleFrame;
    
    //初始化子类变量
    pIpgsDecOal->nBitmapDataSize = PGS_FRAME_SIZE;
    pIpgsDecOal->pBitmapData = (__u8 *)PHY_MALLOC(sizeof(__u8)*pIpgsDecOal->nBitmapDataSize, 1024);
    if(pIpgsDecOal->pBitmapData == NULL)
    {
        WARNING("Ipgs malloc outputItem pSubtitlesize[%d] fail\n", pIpgsDecOal->nBitmapDataSize);
        goto _err1;
    }
    pIpgsDecOal->pPgsDec = newPgsDecInterface();
    if(NULL == pIpgsDecOal->pPgsDec)
    {
        WARNING("malloc PgsDec fail\n");
        goto _err2;
    }
    //这里用真正解码类的字符串赋值适配类基类的字符串.
    Isubtitle_strcpy(pIpgsDecOal->base.strDecName, strDecNameSize, pIpgsDecOal->pPgsDec->strDecName);
    return LDEC_ERROR_OK;

_err2:
    PHY_FREE(pIpgsDecOal->pBitmapData);
    pIpgsDecOal->pBitmapData = NULL;
_err1:
    CedarIsubtitleDec_Exit(&thiz->base);
_err0:
    return LDEC_ERROR_FAIL;
}
__s32 CedarIpgsDec_Exit(CedarIpgsDec *thiz)
{
    //__s32   ret;
    //因为会覆盖基类元素的缘故. 一般先释放自己的,再处理基类的.
    if(thiz->pPgsDec)
    {
        deletePgsDecInterface(thiz->pPgsDec);
    }
    else
    {
        WARNING("fatal error! thiz->pPgsDec = NULL!\n");
    }
    if(thiz->pBitmapData)
    {
        PHY_FREE(thiz->pBitmapData);
        thiz->pBitmapData = NULL;
    }
    CedarIsubtitleDec_Exit(&thiz->base);
    return LDEC_ERROR_OK;
}
CedarIpgsDec* newCedarIpgsDec()
{
    __s32   tmpret;
    CedarIpgsDec *pIpgsDecOal;
    pIpgsDecOal = (CedarIpgsDec*)malloc(sizeof(CedarIpgsDec));
    if(NULL == pIpgsDecOal)
    {
        WARNING("malloc CedarIpgsDec fail\n");
        return NULL;
    }
    tmpret = CedarIpgsDec_Init(pIpgsDecOal);
    if(tmpret != LDEC_ERROR_OK)
    {
        WARNING("CedarIpgsDec init fail\n");
        goto _err0;
    }
    return pIpgsDecOal;
    
_err0:
    free(pIpgsDecOal);
    return NULL;
}
void deleteCedarIpgsDec(CedarIpgsDec *thiz)
{
    CedarIpgsDec_Exit(thiz);
    free(thiz);
}

