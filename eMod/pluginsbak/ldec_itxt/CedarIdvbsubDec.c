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
* File   : CedarIdvbsubDec.c
* Version: V1.0
* By     : chenkeye
* Date   : 2012-11-24
* Description:
********************************************************************************
*/
#include "ldec_itxt_s.h"
#include "CedarIdvbsubDec.h"

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
Function name: CedarItxtDec_Open,  Impl_ItxtDec_Open
Description: 
    1. 给lbsbuf和framequeue分配内存
Parameters: 
    
Return: 
    
Time: 2011/9/26
*******************************************************************************/
__s32 CedarIdvbsubDec_Open(CedarIsubtitleDec *thiz)
{
    CedarIdvbsubDec *pIdvbsubDecOal = (CedarIdvbsubDec*)thiz;
    __s32   tmpret;
    tmpret = pIdvbsubDecOal->pdvbsubDec->open(pIdvbsubDecOal->pdvbsubDec);
    if(tmpret != LDEC_ERR_NONE)
    {
        WARNING("dvbsubDec open fail[%x]\n", tmpret);
        return LDEC_ERROR_FAIL;
    }
    else
    {
        return LDEC_ERROR_OK;
    }
}

__s32 CedarIdvbsubDec_Close(CedarIsubtitleDec *thiz)
{
    CedarIdvbsubDec *pIdvbsubDecOal = (CedarIdvbsubDec*)thiz;
    __s32   tmpret;
    tmpret = pIdvbsubDecOal->pdvbsubDec->close(pIdvbsubDecOal->pdvbsubDec);
    if(tmpret != LDEC_ERR_NONE)
    {
        WARNING("dvbsubDec close fail[%x]\n", tmpret);
        return LDEC_ERROR_FAIL;
    }
    else
    {
        return LDEC_ERROR_OK;
    }
}

__s32 CedarIdvbsubDec_IoCtrl(CedarIsubtitleDec *thiz, __s32 cmd, __s32 aux, void *pbuffer)
{
    __s32   ret = EPDK_FAIL;
    CedarIdvbsubDec *pIdvbsubDecOal = (CedarIdvbsubDec*)thiz;
    __s32   nItxtDecCmd;
    switch(cmd)
    {
        case CEDARISUBTITLEDEC_CMD_SET_SUBTITLE_SOURCE:
        {
            nItxtDecCmd = DVBSUBDEC_CMD_SET_SUBTITLE_SOURCE;
            break;
        }
        case CEDARISUBTITLEDEC_CMD_NOTIFY_SYNCSTAT:
        {
            nItxtDecCmd = DVBSUBDEC_CMD_NOTIFY_SYNCSTAT;
            break;
        }
        case CEDARISUBTITLEDEC_CMD_CHECK_SYNCSTAT:
        {
            nItxtDecCmd = DVBSUBDEC_CMD_CHECK_SYNCSTAT;
	    break;
        }
        case CEDARISUBTITLEDEC_CMD_SET_CHAR_CODING_TYPE:
        {
            nItxtDecCmd = DVBSUBDEC_CMD_SET_CHAR_CODING_TYPE;
            break;
        }
        default:
        {
            WARNING("Unknown cmd[%x]\n", cmd);
            return LDEC_ERROR_FAIL;
        }
    }
    ret = pIdvbsubDecOal->pdvbsubDec->DvbsubDecIoCtrl(pIdvbsubDecOal->pdvbsubDec, nItxtDecCmd, aux, pbuffer);
    if(ret != LDEC_ERR_NONE)
    {
        return LDEC_ERROR_FAIL;
    }
    else
    {
        return LDEC_ERROR_OK;
    }
}

__s32 CedarIdvbsubDec_RequestWrite(CedarIsubtitleDec *thiz, __u32 uSizeRequested, __u8 **ppData0, __u32 *puSizeGot0, __u8 **ppData1, __u32 *puSizeGot1)
{
    __s32   tmpret;
    CedarIdvbsubDec *pIdvbsubDecOal = (CedarIdvbsubDec*)thiz;
    tmpret = pIdvbsubDecOal->pdvbsubDec->RequestWrite(pIdvbsubDecOal->pdvbsubDec, uSizeRequested, ppData0, puSizeGot0, ppData1, puSizeGot1);
    if(tmpret != LDEC_ERR_NONE)
    {
        return LDEC_ERROR_FAIL;
    }
    else
    {
        return LDEC_ERROR_OK;
    }
}

__s32 CedarIdvbsubDec_UpdatePointer(CedarIsubtitleDec *thiz, __lbs_datctlblk_t *pLbsDataCtrlBlk,__u32 uDataSize)
{
    __s32   tmpret;
    CedarIdvbsubDec *pIdvbsubDecOal = (CedarIdvbsubDec*)thiz;
    tmpret = pIdvbsubDecOal->pdvbsubDec->UpdatePointer(pIdvbsubDecOal->pdvbsubDec, pLbsDataCtrlBlk, uDataSize);
    if(tmpret != EPDK_OK)
    {
        return LDEC_ERROR_FAIL;
    }
    else
    {
        return LDEC_ERROR_OK;
    }
}

__cedar_subtitle_item_t* CedarIdvbsubDec_GetLrcItem(CedarIsubtitleDec *thiz, __u32 nPts, __s32 *pRedecodeFlag)    //ret = pOutputItem, NULL表示没有找到, *pRedecodeFlag表示是否需要重新解码
{
        return NULL;
}

CCedarSubtitleFrame* CedarIdvbsubDec_GetSubtitleFrame(CedarIsubtitleDec *thiz, __u32 nPts, __s32 *pRedecodeFlag)
{
    __s32 tmpret;
    CedarIdvbsubDec        *pIdvbsubDecOal = (CedarIdvbsubDec*)thiz;

    if(0 != pIdvbsubDecOal->outputSubtitleFrame.uPts)   //说明已经拷贝过一个字幕帧到outputItem里了, 那么先查看该帧是否符合要求.
    {
//        if(nPts >= pItxtDecOal->outputItem.uPts && nPts < pItxtDecOal->outputItem.uPts + pItxtDecOal->outputItem.uDuration)
//        {
            //WARNING("same nPts[%d], [%d][%d]\n", nPts, pItxtDecOal->outputItem.uPts, pItxtDecOal->outputItem.uDuration);
//            *pRedecodeFlag = 0;
//            return &pItxtDecOal->outputItem;
//        }
    }
    tmpret = pIdvbsubDecOal->pdvbsubDec->GetLrcItem(pIdvbsubDecOal->pdvbsubDec, nPts, &pIdvbsubDecOal->outputSubtitleFrame, pIdvbsubDecOal->nOutputSubtitleSize);
    if(pIdvbsubDecOal->pdvbsubDec->ldecManager.validSize < LDEC_MAX_LBS_SUB_NUM)
    {
        *pRedecodeFlag = 1;
    }
    else
    {
        *pRedecodeFlag = 0;
    }
    if(LDEC_ERR_NONE == tmpret) //找到了
    {
        if(pIdvbsubDecOal->outputSubtitleFrame.isFull)
        {
            return &pIdvbsubDecOal->outputSubtitleFrame;
        }
        else
        {
            WARNING("ReqSubtitle: dvb empty subtitle!\n");
            return NULL;
        }
    }
    else
    {
        return NULL;
    }
}

/*******************************************************************************
Function name: CedarIdvbsubDec_Decode
Description: 
    1.目前的设计是:必须解出一帧,或者lbs underflow, frame underflow
Parameters: 
    
Return: 
    优先返回lbs underflow.
Time: 2011/10/19
*******************************************************************************/
__s32 CedarIdvbsubDec_Decode(CedarIsubtitleDec *thiz)   //字幕解码
{
    __s32   ret;
    __s32   tmpret;
    CedarIdvbsubDec *pIdvbsubDecOal = (CedarIdvbsubDec*)thiz;
    tmpret = pIdvbsubDecOal->pdvbsubDec->CommonDecode(pIdvbsubDecOal->pdvbsubDec);
    ret = convertLdecResult_Lib2Cedar(tmpret);
    return ret;
}

__s32 CedarIdvbsubDec_CheckSupport(CedarIsubtitleDec *thiz, __s32 nFileFmt)   //ret = EPDK_TRUE, EPDK_FALSE, nFileFmt = enum __CEDARLIB_LYRIC_FILE_FMT
{
    CedarIdvbsubDec *pIdvbsubDecOal = (CedarIdvbsubDec*)thiz;
    return pIdvbsubDecOal->pdvbsubDec->CheckSupport(pIdvbsubDecOal->pdvbsubDec, nFileFmt);
}

void CedarIdvbsubDec_Destruct(CedarIsubtitleDec *thiz)
{
    CedarIdvbsubDec *pIdvbsubDecOal = (CedarIdvbsubDec*)thiz;
    deleteCedarIdvbsubDec(pIdvbsubDecOal);
}

__s32 CedarIdvbsubDec_Init(CedarIdvbsubDec *thiz)
{
    __s32   i;
    __s32   ret;
    CedarIdvbsubDec *pIdvbsubDecOal = thiz;
    memset(thiz, 0, sizeof(CedarIdvbsubDec));
    //先初始化基类
    ret = CedarIsubtitleDec_Init(&thiz->base, LYRIC_DVBSUB);
    if(LDEC_ERROR_OK != ret)
    {
        WARNING("CedarIsubtitleDec Init fail\n");
        goto _err0;
    }
    //覆盖虚函数
    thiz->base.open = CedarIdvbsubDec_Open;
    thiz->base.close = CedarIdvbsubDec_Close;
    thiz->base.CedarISubtitleDecIoCtrl = CedarIdvbsubDec_IoCtrl;
    thiz->base.RequestWrite = CedarIdvbsubDec_RequestWrite;
    thiz->base.UpdatePointer = CedarIdvbsubDec_UpdatePointer;
    thiz->base.GetLrcItem = CedarIdvbsubDec_GetLrcItem;
    thiz->base.GetSubtitleFrame = CedarIdvbsubDec_GetSubtitleFrame;
    thiz->base.decode = CedarIdvbsubDec_Decode;
    thiz->base.CheckSupport = CedarIdvbsubDec_CheckSupport;
    thiz->base.Destruct = CedarIdvbsubDec_Destruct;
    
    //初始化子类变量
    pIdvbsubDecOal->nOutputSubtitleSize = LDEC_MAX_FRM_SIZE;    //LDEC_MAX_TXT_FRM_SIZE
    pIdvbsubDecOal->outputSubtitleFrame.pSubtitle = (__u8 *)PHY_MALLOC(sizeof(__u8)*pIdvbsubDecOal->nOutputSubtitleSize, 1024);
    if(pIdvbsubDecOal->outputSubtitleFrame.pSubtitle == NULL)
    {
        WARNING("Idvbsub malloc outputSubtitleFrame pSubtitlesize[%d] fail\n", pIdvbsubDecOal->nOutputSubtitleSize);
        goto _err1;
    }
    pIdvbsubDecOal->pdvbsubDec = newDvbsubDec();
    if(NULL == pIdvbsubDecOal->pdvbsubDec)
    {
        WARNING("malloc DvbsubDec fail\n");
        goto _err2;
    }
    //这里用真正解码类的字符串赋值适配类基类的字符串.
    Isubtitle_strcpy(pIdvbsubDecOal->base.strDecName, strDecNameSize, pIdvbsubDecOal->pdvbsubDec->strDecName);
    return LDEC_ERROR_OK;

_err2:
    PHY_FREE(thiz->outputSubtitleFrame.pSubtitle);
    thiz->outputSubtitleFrame.pSubtitle = NULL;
_err1:
    CedarIsubtitleDec_Exit(&thiz->base);
_err0:
    return LDEC_ERROR_FAIL;
}

__s32 CedarIdvbsubDec_Exit(CedarIdvbsubDec *thiz)
{
    __s32 i;
    //__s32   ret;
    //因为会覆盖基类元素的缘故. 一般先释放自己的,再处理基类的.
    if(thiz->pdvbsubDec)
    {
        deleteDvbsubDec(thiz->pdvbsubDec);
    }
    else
    {
        WARNING("fatal error! thiz->pdvbsubDec = NULL!\n");
    }
    if(thiz->outputSubtitleFrame.pSubtitle)
    {
        PHY_FREE(thiz->outputSubtitleFrame.pSubtitle);
        thiz->outputSubtitleFrame.pSubtitle = NULL;
    }
    CedarIsubtitleDec_Exit(&thiz->base);
    return LDEC_ERROR_OK;
}
CedarIdvbsubDec* newCedarIdvbsubDec()
{
    __s32   tmpret;
    CedarIdvbsubDec *pIdvbsubDecOal;
    pIdvbsubDecOal = (CedarIdvbsubDec*)malloc(sizeof(CedarIdvbsubDec));
    if(NULL == pIdvbsubDecOal)
    {
        WARNING("malloc CedarIdvbsubDec fail\n");
        return NULL;
    }
    tmpret = CedarIdvbsubDec_Init(pIdvbsubDecOal);
    if(tmpret != LDEC_ERROR_OK)
    {
        WARNING("CedarIdvbsubDec init fail\n");
        goto _err0;
    }
    return pIdvbsubDecOal;
    
_err0:
    free(pIdvbsubDecOal);
    return NULL;
}
void deleteCedarIdvbsubDec(CedarIdvbsubDec *thiz)
{
    CedarIdvbsubDec_Exit(thiz);
    free(thiz);
}

