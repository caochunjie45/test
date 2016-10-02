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
* File   : CedarIsubtitleDec.c
* Version: V1.0
* By     : Eric_wang
* Date   : 2011-10-16
* Description:
********************************************************************************
*/
#include "ldec_itxt_s.h"
#include "CedarIsubtitleDec.h"

/*******************************************************************************
Function name: cedar_strcpy
Description: 
    1. nDestSize是整个pdest的长度, 注意'\0'要占最后一个字节,不能越界.
    2. 在copy过程中,如果srcstring长度过长，就自动截断放入pDesString.
Parameters: 
    
Return: 
    
Time: 2011/4/11
*******************************************************************************/
__s8* Isubtitle_strcpy(__s8* pdest, __s32 nDestSize, const __s8* psrc)
{
    __s32 nSrcLen = strlen((const char*)psrc);
    __s8    *pstr;
    if(nDestSize <= 0)
    {
        WARNING("fatal error! check pdest and nDestSize[%d]!\n", nDestSize);
        return pdest;
    }
    if(nSrcLen <= nDestSize - 1)    // '\0'要占一个字节
    {
        pstr = (__s8*)strcpy((char*)pdest, (char*)psrc);
    }
    else
    {
        WARNING("src string length[%d] > nDestSize[%d]-1, fatal error! only copy [%d] bytes\n", nSrcLen, nDestSize, nDestSize - 1);
        pstr = (__s8*)strncpy((char*)pdest, (char*)psrc, nDestSize - 1);
        pdest[nDestSize-1] = '\0';
    }
    return pstr;
}

__s32 CedarIsubtitleDec_Init(CedarIsubtitleDec *thiz, __s32 nFileFmt)   //enum __CEDARLIB_LYRIC_FILE_FMT
{
    memset(thiz, 0, sizeof(CedarIsubtitleDec));
    thiz->subtitle_type = nFileFmt;
    return LDEC_ERROR_OK;
}
__s32 CedarIsubtitleDec_Exit(CedarIsubtitleDec *thiz)
{
    return LDEC_ERROR_OK;
}

CedarIsubtitleDec* CedarIsubtitleDec_Create(__s32 nFileFmt)   //enum __CEDARLIB_LYRIC_FILE_FMT
{
    CedarIsubtitleDec *pDecOal = NULL;
    switch(nFileFmt)
    {
        case LYRIC_ITXT:
        case LYRIC_ISUB:
        case LYRIC_ISSA:
        {
            pDecOal = (CedarIsubtitleDec*)newCedarItxtDec(nFileFmt);
            break;
        }
        case LYRIC_PGS:
        {
            pDecOal = (CedarIsubtitleDec*)newCedarIpgsDec();
            break;
        }
        case LYRIC_ISDB:
        {
            pDecOal = (CedarIsubtitleDec*)newCedarIisdbDec();
            break;
        }
        case LYRIC_DVBSUB:
        {
            pDecOal = (CedarIsubtitleDec*)newCedarIdvbsubDec();
            break;
        }
        default:
        {
            WARNING("not support subtitle[%d]\n", nFileFmt);
            pDecOal = NULL;
            break;
        }
    }
    if(NULL == pDecOal)
    {
        WARNING("malloc CedarIsubtitleDec fail\n");
    }
    return pDecOal;
    
}
void CedarIsubtitleDec_Destroy(CedarIsubtitleDec *thiz)
{
    thiz->Destruct(thiz);
}

