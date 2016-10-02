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
* File   : itextdec.c
* Version: V1.0
* By     : Eric_wang
* Date   : 2011-9-27
* Description:
********************************************************************************
*/
#include "itextdec_s.h"
#include "itextdec.h"
__s32 ldec_itxt_decode(__cedar_subtitle_item_t *tmpItemInf, __u8 *curDataPtr, __u32 *usedBytes)
{
    __u32   i = 0;
    __u32   dstlen = 0;
    __u8    *pstart = NULL;
    __u8    *dststring = NULL;

    *usedBytes = tmpItemInf->uSubSize;
    pstart = curDataPtr;
    dststring = tmpItemInf->pSubtitle;

    for(i=0; i<*usedBytes; i++)
    {
        if(*curDataPtr == '<')
        {
            if(pstart < curDataPtr)
            {
                memcpy(dststring, pstart, curDataPtr - pstart);
                dststring += curDataPtr - pstart;
                dstlen += curDataPtr - pstart;
            }
            pstart = curDataPtr;
        }
        if(*curDataPtr == '>')
        {
            if(*pstart == '<')
            {
                if((*(pstart + 1)=='b') && (*(pstart + 2)=='r'))
                {
                    *dststring = '\n';
                    dststring++;
                    dstlen++;
                }
                pstart = curDataPtr + 1;
            }
        }
        curDataPtr++;
    }

    if(pstart < curDataPtr)
    {
        memcpy(dststring, pstart, curDataPtr - pstart);
        dstlen += curDataPtr - pstart;
    }
    tmpItemInf->uSubSize = dstlen;

    return LDEC_ERR_NONE;
}

__s32 ItextDec_CheckSupport(ItxtDec *thiz, __s32 nFileFmt)   //ret = EPDK_TRUE, EPDK_FALSE, nFileFmt = enum __CEDARLIB_LYRIC_FILE_FMT
{
    if(LYRIC_ITXT == nFileFmt)
    {
        return EPDK_TRUE;
    }
    else
    {
        return EPDK_FALSE;
    }
}

/*******************************************************************************
Function name: ItextDec_SubtitleDecode
Description: 
    只负责纯粹解码. 结果放到psubitem, 
    对于itext来说,一个block就是一条字幕,所以送入的data必须全用完

    itext主要是utf8格式,这里会处理< >的情况.< >内的内容如果是<br>,就转为换行\n

    原型函数:ldec_itxt_decode()
Parameters: 
    *usedBytes:送入前表示待解码的数据长度,函数执行完后表示用掉的待解码的数据长度.
    对于itext,它要求就是一条待解码字幕项的长度.所以执行完后就是全部用掉
    lbsChgFlg:表示是内置字幕流的头信息,对issa有用. itext不理会.
    
Return: 
    
Time: 2011/9/28
*******************************************************************************/
__s32 ItextDec_SubtitleDecode(ItxtDec *thiz, __cedar_subtitle_item_t *tmpItemInf, __u8 *curDataPtr, __u32 *usedBytes, __s8 lbsChgFlg)   //字幕解码
{
    if(*usedBytes != tmpItemInf->uSubSize)  //就以*usedBytes为准,现在判断是因为在外面uSubSize先给赋值了，赋成待解码的长度
    {
        WARNING("fatal error! check code! [%d][%d]\n", *usedBytes, tmpItemInf->uSubSize);
    }
    return ldec_itxt_decode(tmpItemInf, curDataPtr, usedBytes);
}

void ItextDec_ItxtDec_Destruct(ItxtDec *thiz)
{
    ItextDec *pDec = (ItextDec*)thiz;
    deleteItextDec(pDec);
}

__s32 ItextDec_Init(ItextDec *thiz)
{
    __s32   tmpret;
    //先初始化基类
    tmpret = ItxtDec_Init(&thiz->base, LYRIC_ITXT);
    if(tmpret != EPDK_OK)
    {
        WARNING("init ItxtDec fail\n");
        goto _err0;
    }
    //覆盖基类某些虚成员
    strcpy((char*)thiz->base.strDecName, "Itextdec");
    thiz->base.nLbsBufSize = ITEXTDEC_MAX_LBSBUF_SIZE;
    thiz->base.nLdecFrameSize = ITEXTDEC_MAX_FRM_SIZE;
    thiz->base.nanciDataBufSize = ITEXTDEC_MAX_ANCI_SIZE;
    thiz->base.CheckSupport = ItextDec_CheckSupport;
    thiz->base.SubtitleDecode = ItextDec_SubtitleDecode;
    thiz->base.Destruct = ItextDec_ItxtDec_Destruct;
    //初始化自己的成员

    return EPDK_OK;

_err0:
    return EPDK_FAIL;
}
__s32 ItextDec_Exit(ItextDec *thiz)
{
    //因为会覆盖基类元素的缘故. 一般先释放自己的,再处理基类的.
    ItxtDec_Exit(&thiz->base);
    return EPDK_OK;
}

ItextDec *newItextDec()
{
    __s32   tmpret;
    ItextDec *pItextDec = (ItextDec*)malloc(sizeof(ItextDec));
    if(NULL == pItextDec)
    {
        WARNING("malloc ItextDec fail\n");
        return NULL;
    }
    tmpret = ItextDec_Init(pItextDec);
    if(tmpret != EPDK_OK)
    {
        WARNING("ItextDec init fail\n");
        goto _err0;
    }
    return pItextDec;
_err0:
    free(pItextDec);
    return NULL;
}
void deleteItextDec(ItextDec *thiz)
{
    ItextDec_Exit(thiz);
    free(thiz);
}

