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
    ֻ���𴿴����. ����ŵ�psubitem, 
    ����itext��˵,һ��block����һ����Ļ,���������data����ȫ����

    itext��Ҫ��utf8��ʽ,����ᴦ��< >�����.< >�ڵ����������<br>,��תΪ����\n

    ԭ�ͺ���:ldec_itxt_decode()
Parameters: 
    *usedBytes:����ǰ��ʾ����������ݳ���,����ִ������ʾ�õ��Ĵ���������ݳ���.
    ����itext,��Ҫ�����һ����������Ļ��ĳ���.����ִ��������ȫ���õ�
    lbsChgFlg:��ʾ��������Ļ����ͷ��Ϣ,��issa����. itext�����.
    
Return: 
    
Time: 2011/9/28
*******************************************************************************/
__s32 ItextDec_SubtitleDecode(ItxtDec *thiz, __cedar_subtitle_item_t *tmpItemInf, __u8 *curDataPtr, __u32 *usedBytes, __s8 lbsChgFlg)   //��Ļ����
{
    if(*usedBytes != tmpItemInf->uSubSize)  //����*usedBytesΪ׼,�����ж�����Ϊ������uSubSize�ȸ���ֵ�ˣ����ɴ�����ĳ���
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
    //�ȳ�ʼ������
    tmpret = ItxtDec_Init(&thiz->base, LYRIC_ITXT);
    if(tmpret != EPDK_OK)
    {
        WARNING("init ItxtDec fail\n");
        goto _err0;
    }
    //���ǻ���ĳЩ���Ա
    strcpy((char*)thiz->base.strDecName, "Itextdec");
    thiz->base.nLbsBufSize = ITEXTDEC_MAX_LBSBUF_SIZE;
    thiz->base.nLdecFrameSize = ITEXTDEC_MAX_FRM_SIZE;
    thiz->base.nanciDataBufSize = ITEXTDEC_MAX_ANCI_SIZE;
    thiz->base.CheckSupport = ItextDec_CheckSupport;
    thiz->base.SubtitleDecode = ItextDec_SubtitleDecode;
    thiz->base.Destruct = ItextDec_ItxtDec_Destruct;
    //��ʼ���Լ��ĳ�Ա

    return EPDK_OK;

_err0:
    return EPDK_FAIL;
}
__s32 ItextDec_Exit(ItextDec *thiz)
{
    //��Ϊ�Ḳ�ǻ���Ԫ�ص�Ե��. һ�����ͷ��Լ���,�ٴ�������.
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

