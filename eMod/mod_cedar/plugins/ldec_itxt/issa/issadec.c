/*
*********************************************************************************************************
*                                                    SSA DECODE LIB
*                                                      *.SSA Format
*
*                                       (c) Copyright 2008-2009, xiechr China
*                                                   All Rights Reserved
*
* File   : ssadec.c
* Version: V1.0
* By     : xiechuanrong
* Date   : 2009-8-25 15:01
*********************************************************************************************************
*/

#include "ldec_issa_s.h"
#include "issadec.h"
//#include "ldec_itxt_i.h"

#define SSA_TMP_BUFFER_SIZE (5*1024)
static __ssa_dec_info_t g_ssaDecInfo;
static __u8 g_tmpbuffer[SSA_TMP_BUFFER_SIZE];   //如果送下来的ssa字幕条目需要转换成utf8,转换后的buffer就放到这个buffer里.

static __s32 _NewSsaDialogueNode(__ssa_dec_info_t *pDecInfo);
static __s32 _SkipFields(__u8 **pLbs, __u32 *pLbsSize, __ssa_dec_info_t *pDecInfo, __u8 nFileds);
static __s32 _GetDialogueEvent(__u8 **pLbs, __u32 *pLbsSize, __ssa_dec_info_t *pDecInfo);
static __s32 _SaveDialogueStr(__u8 *ptxtStart, __u8 *ptxtEnd, __ssa_dec_info_t *pDecInfo);
static __s32 _SaveDialogueChar(__u8 dialogueChar, __ssa_dec_info_t *pDecInfo);
static __s32 _GetDialogueText(__u8 **pLbs, __u32 *pLbsSize, __ssa_dec_info_t *pDecInfo);


static __u32 _ParseBOM(__u8 *pBOMBuf)
{
    __u8    *tmpPtr = (__u8 *)pBOMBuf;
    __u32   tmpBOM;
    __u32   nTxtEncType;

    //parse the smi file character encode type
    tmpBOM = (tmpPtr[0]<<0) | (tmpPtr[1]<<8) | (tmpPtr[2]<<16) | (tmpPtr[3]<<24);
    if((tmpBOM&0x0000FFFF) == 0xFFFE)
    {
        nTxtEncType = CEDAR_SUB_ENCODE_UTF16BE;
    }
    else if((tmpBOM&0x0000FFFF) == 0xFEFF)
    {
        nTxtEncType = CEDAR_SUB_ENCODE_UTF16LE;
    }
    else if((tmpBOM&0x00FFFFFF) == 0x00BFBBEF)
    {
        nTxtEncType = CEDAR_SUB_ENCODE_UTF8;
    }
    else
    {
        nTxtEncType = CEDAR_SUB_ENCODE_UTF8;
    }
    return nTxtEncType;
}

/*
*********************************************************************************************************
*                       SMI DECODER MODULE CONVERT ENCODING
*
*Description: 查找最后一个换行符，并将之前的文本转换为UTF8格式，之后的文本保存在punsettled中。
*
*Arguments  : encoding   原编码方式;
*             pLbs           待转换的数据流;
*             nLbsSize       数据流大小;
*
*Return     : decoder result;
*               =   0 ,   成功;
*               =   -1 ,  失败
*
*Note       : 1. 如果需要转换，函数会重新分配内存给pLbs，并返回buffer大小
*********************************************************************************************************
*/
static __s32 _Ssa_Lbs_Convert_Encoding(__s32 encoding, __u8 **pLbs, __u32 *nLbsSize)
{
    __u8 *ptmpLbs;
    __u8 *ptmpNewLbsBuf;
    __u32 tmpLbsSize;
    __s32 tmpEncoding;
    __u32 tmpNewLbsSize;

    ptmpLbs = *pLbs;
    tmpLbsSize = *nLbsSize;

    //translate the encode type to UTF8
    if(encoding == CEDAR_SUB_ENCODE_UTF16BE)
    {
        tmpEncoding = EPDK_CHAR_ENCODING_UTF16BE;
    }
    else if(encoding == CEDAR_SUB_ENCODE_UTF16LE)
    {
        tmpEncoding = EPDK_CHAR_ENCODING_UTF16LE;
    }
    else
    {
        tmpEncoding = EPDK_CHAR_ENCODING_UTF8;
    }

    if (tmpEncoding != EPDK_CHAR_ENCODING_UTF8)
    {
        tmpNewLbsSize = eLIBs_encoding_stream_convert_size(tmpEncoding, EPDK_CHAR_ENCODING_UTF8, (char *)ptmpLbs, tmpLbsSize);
        if(tmpNewLbsSize == 0)
        {
            //no valid text bitstream after convert
            return -1;;
        }
        if (tmpNewLbsSize > SSA_TMP_BUFFER_SIZE)
        {
            WARNING("convert size[%d] > [%d], phy_malloc buffer!\n", tmpNewLbsSize, SSA_TMP_BUFFER_SIZE);
            ptmpNewLbsBuf = (__u8 *)phy_malloc(tmpNewLbsSize + 1, 1024);
        }
        else
        {
            ptmpNewLbsBuf = g_tmpbuffer;
        }
        if(!ptmpNewLbsBuf)
        {
            WARNING("Request lbs buffer failed!\n");
            return -1;
        }
        //convert smi file encode type to UTF8
        tmpNewLbsSize = eLIBs_encoding_stream_convert(tmpEncoding, EPDK_CHAR_ENCODING_UTF8,
                                (char *)ptmpLbs, tmpLbsSize, (char *)ptmpNewLbsBuf, tmpNewLbsSize + 1);
        *(ptmpNewLbsBuf + tmpNewLbsSize) = 0;
    }
    else
    {
        tmpNewLbsSize = tmpLbsSize;
        if (tmpNewLbsSize > SSA_TMP_BUFFER_SIZE)
        {
            ptmpNewLbsBuf = (__u8 *)phy_malloc(tmpNewLbsSize, 1024);
        }
        else
        {
            ptmpNewLbsBuf = g_tmpbuffer;
        }
        if(!ptmpNewLbsBuf)
        {
            WARNING("Request lbs buffer failed!\n");
            return -1;;
        }
        memcpy (ptmpNewLbsBuf, ptmpLbs, tmpNewLbsSize);
    }
    *pLbs = ptmpNewLbsBuf;
    *nLbsSize = tmpLbsSize;
    return 0;
}




/*
*********************************************************************************************************
*Description: 跳过一行
*
*Arguments  : pLbs          ssa文件数据流;
*             nLbsSize      ssa文件数据块的大小;
*             pDecInfo      ssa文件信息句柄;
*
*Return     : decoder result;
*               < 0  ,  出错;
*               = 0  ,  成功;
*
*Note       :
*
*********************************************************************************************************
*/
static void _SkipALine(__u8 **pLbs, __u32 *pLbsSize, __ssa_dec_info_t *pDecInfo)
{
    __u8 *ptmpLbs;
    __u32 ntmpLbsSize;
    ptmpLbs = *pLbs;
    ntmpLbsSize = *pLbsSize;
    while ((ntmpLbsSize > 0) &&(*ptmpLbs != '\r') && (*ptmpLbs != '\n'))
    {
        ptmpLbs++;
        ntmpLbsSize--;
    }
    while ((ntmpLbsSize > 0) &&((*ptmpLbs == '\r') || (*ptmpLbs == '\n')))
    {
        ptmpLbs++;
        ntmpLbsSize--;
    }

    *pLbs = ptmpLbs;
    *pLbsSize = ntmpLbsSize;

    return;
}


/*
*********************************************************************************************************
*Description: 跳过空格
*
*Arguments  : pLbs          ssa文件数据流;
*             nLbsSize      ssa文件数据块的大小;
*
*********************************************************************************************************
*/
static void _SkipSpace(__u8 **pLbs, __u32 *pLbsSize)
{
    __u8 *ptmpLbs;
    __u32 ntmpLbsSize;
    ptmpLbs = *pLbs;
    ntmpLbsSize = *pLbsSize;
    while ((ntmpLbsSize > 0) && (*ptmpLbs == ' '))
    {
        ptmpLbs++;
        ntmpLbsSize--;
    }
    *pLbs = ptmpLbs;
    *pLbsSize = ntmpLbsSize;
}



/*
*********************************************************************************************************
*Description: 获取行描述符信息
*
*Arguments  : pLbs          ssa文件数据流;
*             nLbsSize      ssa文件数据块的大小;
*             pDecInfo      ssa文件信息句柄;
*
*Return     :
*
*Note       :
*********************************************************************************************************
*/
static void _GetDescriptor(__u8 **pLbs, __u32 *nLbsSize, __ssa_dec_info_t *pDecInfo)
{
    __u8  *tmpLbs;
    __u32 tmpLbsSize;
    __u8 tmpString[20];
    __u8 stringSize;
    __u8 endChar = ':';

    tmpLbs = *pLbs;
    tmpLbsSize = *nLbsSize;
    pDecInfo->curDescriptor = SSA_LINE_DESCRIPTOR_IGNORED;

    /*section 描述符以'['开始*/
    if (*tmpLbs == '[')
    {
        tmpLbs ++;
        tmpLbsSize --;
        endChar = ']';
    }

    //try to get descriptor string
    stringSize = 1;
    memset(tmpString, 0, sizeof(tmpString));
    while(!((*tmpLbs == '\r') || (*tmpLbs == '\n')) && (tmpLbsSize > 0) && (stringSize < 20))
    {
        if (!isalpha(*tmpLbs))
        {
            break;
        }
        else
        {
            tmpString[stringSize - 1] = tolower(*tmpLbs);
        }
        tmpLbs++;
        tmpLbsSize--;
        stringSize++;
    }

    while((tmpLbsSize > 0) && (*tmpLbs == ' '))
    {
        tmpLbs++;
        tmpLbsSize--;
    }

    if ((tmpLbsSize == 0) || (*tmpLbs != endChar))
    {
        return ;
    }

    *pLbs = tmpLbs + 1;
    *nLbsSize = tmpLbsSize - 1;

    __wrn("_GetDescriptor:%s\n",tmpString);
    if (strncmp((char *)tmpString, "dialogue", 8) == 0)
    {
        pDecInfo->curDescriptor = SSA_LINE_DESCRIPTOR_DIALOGUE;
    }
    if (strncmp((char *)tmpString, "style", 8) == 0)
    {
        pDecInfo->curDescriptor = SSA_LINE_DESCRIPTOR_STYLE;
    }
    return ;
}


static __s32 _GetStyle(__u8 **pLbs, __u32 *pLbsSize, __ssa_stytle_item_t *pStyle)
{
    __u8 *ptmpLbs;
    __u32 ntmpLbsSize;
    __s32 result;
    __u8 i;

    ptmpLbs = *pLbs;
    ntmpLbsSize = *pLbsSize;

    memset(pStyle, 0, sizeof(__ssa_stytle_item_t));

    /*style 各字段的顺序为 name,Fontname, Fontsize, PrimaryColour, SecondaryColour,
    TertiaryColour, BackColour, Bold, Italic, BorderStyle, Outline, Shadow, Alignment,
    MarginL, MarginR, MarginV, AlphaLevel, Encoding*/
    _SkipSpace(&ptmpLbs, &ntmpLbsSize);
    i = 0;
    while((*ptmpLbs != ',') && (i < SSA_MAX_NAME_LENGTH - 1))
    {
        ntmpLbsSize--;
        if (*ptmpLbs == '*')
        {
            ptmpLbs++;
            continue;
        }
        pStyle->stylename[i++] = *ptmpLbs++;
    }
    if (*ptmpLbs != ',')
    {
        result = _SkipFields(&ptmpLbs, &ntmpLbsSize, NULL, 1);
    }
    else
    {
        ntmpLbsSize--;
        ptmpLbs++;
    }
    result = _SkipFields(&ptmpLbs, &ntmpLbsSize, NULL, 16);
    if (result < 0)
    {
        goto FORMAT_ERR;
    }

    _SkipSpace(&ptmpLbs, &ntmpLbsSize);
    i = 0;
    while((*ptmpLbs != '\r') && (*ptmpLbs != '\n') && (i < 10))
    {
        if (*ptmpLbs < '0' || *ptmpLbs > '9')
        {
            break;
        }
        pStyle->encoding *= 10;
        pStyle->encoding += *ptmpLbs - '0';
        ptmpLbs++;
        ntmpLbsSize--;
        i++;
    }

    /*0: English (Western, ANSI) Windows，1: 缺省，2:符号，128:Japanese-Shift-JIS(日文)，
    129:韩文，134:GB2312(简体中文)，136:BIG5(繁体中文)，161:希腊语，162:土耳其语，
    163:越南语，177:希伯来语，178:阿拉伯语，186:波罗的语，222:泰国语， 204:俄语
    */
    switch (pStyle->encoding)
    {
        case 134:
            pStyle->encoding = CEDAR_SUB_ENCODE_GBK;
            __wrn("\nstyle name:%s, encoding:CEDAR_SUB_ENCODE_GBK\n",pStyle->stylename);
            break;
        case 136:
            pStyle->encoding = CEDAR_SUB_ENCODE_BIG5;
            __wrn("\nstyle name:%s, encoding:CEDAR_SUB_ENCODE_BIG5\n",pStyle->stylename);
            break;
        default:
            pStyle->encoding = CEDAR_SUB_ENCODE_ANSI;
            __wrn("\nstyle name:%s, encoding:default\n",pStyle->stylename);
            break;
    }
    while ((ntmpLbsSize > 0) &&((*ptmpLbs == '\r') || (*ptmpLbs == '\n')))
    {
        ptmpLbs++;
        ntmpLbsSize--;
    }

    *pLbs = ptmpLbs;
    *pLbsSize = ntmpLbsSize;
    return 0;

FORMAT_ERR:
    MESSAGE("ssa file style event format err!");
    memset(pStyle, 0, sizeof(__ssa_stytle_item_t));
    _SkipALine(&ptmpLbs, &ntmpLbsSize, NULL);
    *pLbs = ptmpLbs;
    *pLbsSize = ntmpLbsSize;
    return -1;
}




/*
*********************************************************************************************************
*                       DECODE ONE SSA FILE DATA BUFFER
*
*Description: 解析ssa文件的数据流，并将结果保存在指定的buffer中
*
*Arguments  : pLbs          ssa文件数据流;
*             nLbsSize      ssa文件数据块的大小;
*             pSsaInfo      用于保存解析ssa文件后相关信息的buffer;
*             nInfoBufSize  用于保存解析ssa文件后相关信息的buffer的大小;
*
*Return     : decoder result;
*               <  0 ,  解析数据流出错;
*               =  0 ,  成功
*
*********************************************************************************************************
*/
__s32 SsaDecodeHead(__u8 **pLbs, __u32 *nLbsSize, __ssa_dec_info_t *pDecInfo)
{
    __u8 *ptmpLbs;
    __u32 ntmpLbsSize;
    __ssa_stytle_item_t tmpstyle;
    __s32 result = 0;

    ptmpLbs = *pLbs;
    ntmpLbsSize = *nLbsSize;
    __wrn("====================\nhead:%s\n==============\n",ptmpLbs);
    while ((ntmpLbsSize > 0) && (result >= 0))
    {
        _GetDescriptor(&ptmpLbs, &ntmpLbsSize, pDecInfo);
        if (pDecInfo->curDescriptor == SSA_LINE_DESCRIPTOR_STYLE)
        {
            result = _GetStyle(&ptmpLbs,  &ntmpLbsSize, &tmpstyle);
            if (result < 0)
            {
                break;
            }
            if (strncmp((void *)tmpstyle.stylename, "Default", 8) == 0)
            {
                if (pDecInfo->style_num > SSA_MAX_STYLE_NUM)
                {
                    pDecInfo->style_num--;
                }
                pDecInfo->default_style = pDecInfo->style_num;
            }
            if (pDecInfo->style_num < SSA_MAX_STYLE_NUM)
            {
                strncpy((void *)pDecInfo->style_item[pDecInfo->style_num].stylename, (void *)tmpstyle.stylename, SSA_MAX_NAME_LENGTH);
                pDecInfo->style_item[pDecInfo->style_num].encoding = tmpstyle.encoding;
                pDecInfo->style_num ++;
            }
        }
        else
        {
            _SkipALine(&ptmpLbs, &ntmpLbsSize, pDecInfo);
        }
    }
    return 0;
}


static __s32 _NewSsaDialogueNode(__ssa_dec_info_t *pDecInfo)
{
    __u8 *wordalign;
    __ssa_dialogue_node_t * ptmpNode;
    __s32 tmpSize;

    /*当前节点为空，可以回收利用*/
    if (pDecInfo->pCurDialogueNode != NULL)
    {
        if (pDecInfo->pCurDialogueNode->dialogueItem.nTextLen == 0)
        {
            memset(pDecInfo->pCurDialogueNode, 0, sizeof(__ssa_dialogue_node_t));
            pDecInfo->pCurDialogueNode->dialogueItem.pText = pDecInfo->pCurBufPos;
            return 0;
        }
    }

    wordalign = (__u8 *)pDecInfo->pCurBufPos;
    wordalign = (__u8 *)(((__u32)wordalign + 3) & ~0x03);
    tmpSize = sizeof(__ssa_dialogue_node_t) + (wordalign - pDecInfo->pCurBufPos);
    if (pDecInfo->nSpareBufSize < tmpSize)
    {
        WARNING("ssa item buffer overflow!\n");
        pDecInfo->nSpareBufSize = 0;
        return -1;
    }
    pDecInfo->nSpareBufSize -= tmpSize;
    ptmpNode = (__ssa_dialogue_node_t *)wordalign;
    pDecInfo->pCurBufPos = wordalign + sizeof(__ssa_dialogue_node_t);
    memset(ptmpNode, 0, sizeof(__ssa_dialogue_node_t));
    ptmpNode->dialogueItem.pText = pDecInfo->pCurBufPos;
    pDecInfo->pCurDialogueNode = ptmpNode;
    pDecInfo->pCurDialogueNode->pNext = NULL;
    pDecInfo->pCurDialogueNode->pRev = NULL;
    return 0;

}


/*
*********************************************************************************************************
*Description: 忽略nFileds个字段内容。
*
*Arguments  : pLbs          ssa文件数据流;
*             nLbsSize      ssa文件数据块的大小;
*             pDecInfo      ssa文件信息句柄;
*             nFileds       需要忽略的字段个数
*
*Return     : decoder result;
*               = 0   ,  成功;
*               = -1  ,  出错;
*
*Note       :
*
*********************************************************************************************************
*/
static __s32 _SkipFields(__u8 **pLbs, __u32 *pLbsSize, __ssa_dec_info_t *pDecInfo, __u8 nFileds)
{
    __u8 *ptmpLbs;
    __u32 ntmpLbsSize;
    ptmpLbs = *pLbs;
    ntmpLbsSize = *pLbsSize;
    while ((ntmpLbsSize > 0) && (*ptmpLbs != '\r') && (*ptmpLbs != '\n') && (nFileds > 0))
    {
        if (*ptmpLbs == ',')
        {
            nFileds--;
        }
        ptmpLbs++;
        ntmpLbsSize--;
    }
    if ((0 == ntmpLbsSize) || (*ptmpLbs == '\r') || (*ptmpLbs == '\n'))
    {
        return -1;;
    }

    *pLbs = ptmpLbs;
    *pLbsSize = ntmpLbsSize;
    return 0;

}

static __s32 _SaveDialogueStr(__u8 *ptxtStart, __u8 *ptxtEnd, __ssa_dec_info_t *pDecInfo)
{
    __u32 ntmpSize;

    if (ptxtStart == NULL || (ptxtEnd < ptxtStart))
    {
        return -1;
    }
    ntmpSize = ptxtEnd - ptxtStart;
    if (pDecInfo->nSpareBufSize < ntmpSize)
    {
        pDecInfo->nSpareBufSize = 0;
        return -1;
    }
    if (pDecInfo->pCurDialogueNode->dialogueItem.pText == NULL)
    {
        pDecInfo->pCurDialogueNode->dialogueItem.pText = pDecInfo->pCurBufPos;
    }

    memcpy(pDecInfo->pCurBufPos, ptxtStart, ntmpSize);

    pDecInfo->pCurBufPos += ntmpSize;
    pDecInfo->nSpareBufSize -= ntmpSize;
    pDecInfo->pCurDialogueNode->dialogueItem.nTextLen += ntmpSize;
    return 0;
}

static __s32 _SaveDialogueChar(__u8 dialogueChar, __ssa_dec_info_t *pDecInfo)
{

    if (pDecInfo->nSpareBufSize == 0)
    {
        return -1;
    }
    if (pDecInfo->pCurDialogueNode->dialogueItem.pText == NULL)
    {
        pDecInfo->pCurDialogueNode->dialogueItem.pText = pDecInfo->pCurBufPos;
    }
    *(pDecInfo->pCurBufPos) = dialogueChar;
    pDecInfo->pCurBufPos++;
    pDecInfo->nSpareBufSize -= 1;
    if (dialogueChar != '\0')
    {
        pDecInfo->pCurDialogueNode->dialogueItem.nTextLen += 1;
    }
    return 0;
}



/*
*********************************************************************************************************
*Description: 获取dialogue事件的文本
*
*Arguments  : pLbs          ssa文件数据流;
*             nLbsSize      ssa文件数据块的大小;
*             pDecInfo      ssa文件信息句柄;
*
*Return     : decoder result;
*               = 0   ,  解析数据流成功;
*               = -1  ,  解析数据流出错;
*
*Note       : 函数会改变*pLbs的值
*
*********************************************************************************************************
*/
static __s32 _GetDialogueText(__u8 **pLbs, __u32 *pLbsSize, __ssa_dec_info_t *pDecInfo)
{
    __u8 *ptmpLbs, *ptextStart;
    __u32 ntmpLbsSize;
    __u32 ntmpLbsSize1;
    __s32 result;

    ptmpLbs = *pLbs;
    ptextStart = *pLbs;
    ntmpLbsSize = *pLbsSize;

    while ((ntmpLbsSize > 0) && (*ptmpLbs != '\r') && (*ptmpLbs != '\n'))
    {
        if (*ptmpLbs == '{')
        {
            result = _SaveDialogueStr(ptextStart, ptmpLbs, pDecInfo);
            if (result < 0)
            {
                return -1;
            }
            ptextStart = ptmpLbs;
            ntmpLbsSize1 = ntmpLbsSize;
            while ((ntmpLbsSize > 0) && (*ptmpLbs != '\r') && (*ptmpLbs != '\n') && (*ptmpLbs != '}'))
            {
                ptmpLbs++;
                ntmpLbsSize--;
            }
            if (*ptmpLbs == '}')
            {
                /*可在此处添加解析特效代码*/
                ptextStart = ptmpLbs + 1;
            }
            else  /*未找到'}'，从'{'重新查找，防止'{'后出现'\n'*/
            {
                ptmpLbs = ptextStart;
                ntmpLbsSize = ntmpLbsSize1;
            }
        }

        if ((*ptmpLbs == '\\') && (ntmpLbsSize > 1) && ((ptmpLbs[1] == 'n') || (ptmpLbs[1] == 'N')))
        {
            _SaveDialogueStr(ptextStart, ptmpLbs, pDecInfo);
            result = _SaveDialogueChar('\n', pDecInfo);
            if (result < 0)
            {
                return -1;
            }
            ptextStart = ptmpLbs + 2;
        }

        ptmpLbs++;
        ntmpLbsSize--;
    }

    if (ptextStart < ptmpLbs)
    {
        _SaveDialogueStr(ptextStart, ptmpLbs, pDecInfo);
    }
    result = _SaveDialogueChar('\0', pDecInfo);

    while ((ntmpLbsSize > 0) &&((*ptmpLbs == '\r') || (*ptmpLbs == '\n')))
    {
        ptmpLbs++;
        ntmpLbsSize--;
    }
    *pLbs = ptmpLbs;
    *pLbsSize = ntmpLbsSize;
    return result;
}

/*
*********************************************************************************************************
*Description: 获取Dialogue事件信息
*
*Arguments  : pLbs          ssa文件数据流;
*             nLbsSize      ssa文件数据块的大小;
*             pDecInfo      ssa文件信息句柄;
*
*Return     :
*               = 0   ,  解析数据流成功;
*               = -1  ,  解析数据流出错;
*
*Note       :
*********************************************************************************************************
*/
static __s32 _GetDialogueEvent(__u8 **pLbs, __u32 *pLbsSize, __ssa_dec_info_t *pDecInfo)
{
    __u8 *ptmpLbs;
    __u32 ntmpLbsSize;
    __u8 stylename[SSA_MAX_NAME_LENGTH];  //style name
    __u32 i;
    __s32 result;

    ptmpLbs = *pLbs;
    ntmpLbsSize = *pLbsSize;

    /*dialogue 各字段的顺序为 Marked, Start, End, Style, Name, MarginL, MarginR, MarginV, Effect, Text*/
    /*mkv格式中内嵌ssa数据无mark字段 -??*/
    result = _SkipFields(&ptmpLbs, &ntmpLbsSize, pDecInfo, 2);
    if (result < 0)
    {
        MESSAGE("ssa file dialogue event format err!");
        *pLbs = ptmpLbs;
        *pLbsSize = ntmpLbsSize;
        return 0;
    }

    _SkipSpace(&ptmpLbs, &ntmpLbsSize);
    i = 0;
    memset(stylename, 0, sizeof(stylename));
    while((*ptmpLbs != ',') && (i < SSA_MAX_NAME_LENGTH - 1))
    {
        ntmpLbsSize--;
        if (*ptmpLbs == '*')
        {
            ptmpLbs++;
            continue;
        }
        stylename[i++] = *ptmpLbs++;
    }
    if (*ptmpLbs != ',')
    {
        _SkipFields(&ptmpLbs, &ntmpLbsSize, pDecInfo, 6);
    }
    else
    {
        ptmpLbs++;
        ntmpLbsSize--;
    }

    for (i = 0; i < pDecInfo->style_num; i++)
    {
        if (strncmp((void *)stylename, (void *)pDecInfo->style_item[i].stylename, SSA_MAX_NAME_LENGTH) == 0)
        {
            pDecInfo->pCurDialogueNode->dialogueItem.style = i;
            break;
        }
    }
    if (i == pDecInfo->style_num)
    {
        pDecInfo->pCurDialogueNode->dialogueItem.style = pDecInfo->default_style;
        __wrn("!! not find style:%s\n",stylename);
    }
    /* Name, MarginL, MarginR, MarginV, Effect*/
    _SkipFields(&ptmpLbs, &ntmpLbsSize, pDecInfo, 5);

    result = _GetDialogueText(&ptmpLbs, &ntmpLbsSize, pDecInfo);
    __wrn("item:%s, stytle:%d\n",ptmpLbs, pDecInfo->pCurDialogueNode->dialogueItem.style);

    *pLbs = ptmpLbs;
    *pLbsSize = ntmpLbsSize;
    return result;
}

/*******************************************************************************
Function name: ldec_issa_decode
Description: 
    1.
Parameters: 
    lbsChgFlg:原来表示是否换字幕,现在的含义其实是是否是ssa头信息
    nLdecFrameSize: 表示psubitem的frame的最大长度, __cedar_subtitle_item_t->pSubtitle的长度
Return: 
    
Time: 2011/10/8
*******************************************************************************/
__s32 ldec_issa_decode(__cedar_subtitle_item_t *psubitem, __u32 nLdecFrameSize, __u8 *pLbs, __u32 *usedBytes,  __s8 lbsChgFlg)
{
   __ssa_dec_info_t *pDecInfo;
   __u32 nLbsSize = 0;
   __u32 ntmpLbsSize;
   __s32 result;
   __u8 *ptmpLbs;
   __u8 *ptmpLbsStart;

   *usedBytes = psubitem->uSubSize;
    nLbsSize  = psubitem->uSubSize;

    pDecInfo = &g_ssaDecInfo;

    ptmpLbs = pLbs;
    ntmpLbsSize = nLbsSize;
    if(lbsChgFlg == 1)
    {
        pDecInfo->txtEncType = _ParseBOM(ptmpLbs);
    }

    result = _Ssa_Lbs_Convert_Encoding(pDecInfo->txtEncType, &ptmpLbs, &ntmpLbsSize);
    if (result < 0)
    {
        return LDEC_ERR_DEC_SUBTITLE;
    }
    ptmpLbsStart = ptmpLbs;
    if (lbsChgFlg == 1)
    {
        result = SsaDecodeHead(&ptmpLbs, &ntmpLbsSize, pDecInfo);
        result = LDEC_ERR_DEC_SUBTITLE;
        goto DEC_END;
    }
    else
    {
        //pDecInfo->nSpareBufSize = nLbsSize;
        pDecInfo->nSpareBufSize = nLdecFrameSize;
        pDecInfo->pBufStart = pLbs;
        pDecInfo->pCurBufPos  = psubitem->pSubtitle;
        pDecInfo->pCurDialogueNode = NULL;
        pDecInfo->curDescriptor = SSA_LINE_DESCRIPTOR_DIALOGUE;
        result = _NewSsaDialogueNode(pDecInfo);
        if (result < 0)
        {
            result = LDEC_ERR_DEC_SUBTITLE;
            goto DEC_END;
        }
        result = _GetDialogueEvent(&ptmpLbs, &ntmpLbsSize, pDecInfo);
        if (result < 0)
        {
            result = LDEC_ERR_DEC_SUBTITLE;
            goto DEC_END;
        }
        psubitem->pSubtitle = pDecInfo->pCurDialogueNode->dialogueItem.pText;
        psubitem->uSubSize = pDecInfo->pCurDialogueNode->dialogueItem.nTextLen;

        if (pDecInfo->txtEncType == CEDAR_SUB_ENCODE_ANSI && pDecInfo->style_num > pDecInfo->pCurDialogueNode->dialogueItem.style)
        {
            psubitem->uDataType = pDecInfo->style_item[pDecInfo->pCurDialogueNode->dialogueItem.style].encoding;
        }
        else
        {
            psubitem->uDataType = pDecInfo->txtEncType;
        }
    }
    result = LDEC_ERR_NONE;
DEC_END:
    if (ptmpLbsStart != g_tmpbuffer)
    {
        phy_free(ptmpLbsStart);
    }
    return result;

}

__s32 IssaDec_CheckSupport(ItxtDec *thiz, __s32 nFileFmt)   //ret = EPDK_TRUE, EPDK_FALSE, nFileFmt = enum __CEDARLIB_LYRIC_FILE_FMT
{
    if(LYRIC_ISSA == nFileFmt)
    {
        return EPDK_TRUE;
    }
    else
    {
        return EPDK_FALSE;
    }
}

/*******************************************************************************
Function name: IssaDec_SubtitleDecode
Description: 
    只负责纯粹解码. 结果放到psubitem, 
    对于issa来说,一个chunk就是一条字幕,所以送入的data必须全用完

    issa有信息头,这里以lbsChgFlg标记是否是信息头

    原型函数:ldec_itxt_decode()

    要注意:ldec_issa_decode()解出的是一个字幕项__ssa_dialogue_node_t, 这个数据结构之后才存放字幕内容.都是放在tmpItemInfo->pSubtitle
    这片内存里的.所以要再处理一下.
Parameters: 
    *usedBytes:送入前表示待解码的数据长度,函数执行完后表示用掉的待解码的数据长度.
    对于issa,它要求就是一条待解码字幕项的长度.所以执行完后就是全部用掉
    lbsChgFlg:表示是内置字幕流的头信息,对issa有用. itext不理会.
    
Return: 
    LDEC_ERR_DEC_SUBTITLE
    LDEC_ERR_NONE
Time: 2011/9/28
*******************************************************************************/
__s32 IssaDec_SubtitleDecode(ItxtDec *thiz, __cedar_subtitle_item_t *tmpItemInf, __u8 *curDataPtr, __u32 *usedBytes, __s8 lbsChgFlg)   //字幕解码
{
    __s32   ret;
    __s32   i;
    __u8    *pSubtitle;
    __u8    *pssaString;
    if(*usedBytes != tmpItemInf->uSubSize)  //就以*usedBytes为准,现在判断是因为在外面uSubSize先给赋值了，赋成待解码的长度
    {
        WARNING("fatal error! check code! [%d][%d]\n", *usedBytes, tmpItemInf->uSubSize);
    }
    pSubtitle = tmpItemInf->pSubtitle;
    ret = ldec_issa_decode(tmpItemInf, thiz->nLdecFrameSize, curDataPtr, usedBytes, lbsChgFlg);
    if(pSubtitle != tmpItemInf->pSubtitle)
    {
        //WARNING("Issa address change, [%x][%x]\n", pSubtitle, tmpItemInf->pSubtitle);
        pssaString = tmpItemInf->pSubtitle;
        tmpItemInf->pSubtitle = pSubtitle;
        for(i=0; i<tmpItemInf->uSubSize; i++)
        {
            tmpItemInf->pSubtitle[i] = pssaString[i];
        }
        //tmpItemInf->pSubtitle[i] = 0;
        //WARNING("item:%s\n",tmpItemInf->pSubtitle);
    }
    else
    {
        WARNING("Issa same address!\n");
    }
    return ret;
}
void IssaDec_ItxtDec_Destruct(ItxtDec *thiz)
{
    IssaDec *pIssaDec = (IssaDec*)thiz;
    deleteIssaDec(pIssaDec);
}

__s32 IssaDec_Init(IssaDec *thiz)
{
    __s32   tmpret;
    //先初始化基类
    tmpret = ItxtDec_Init(&thiz->base, LYRIC_ISSA);
    if(tmpret != EPDK_OK)
    {
        WARNING("init ItxtDec fail\n");
        goto _err0;
    }
    //覆盖基类某些虚成员
    strcpy((char*)thiz->base.strDecName, "Issadec");
    thiz->base.nLbsBufSize = ISSADEC_MAX_LBSBUF_SIZE;
    thiz->base.nLdecFrameSize = ISSADEC_MAX_FRM_SIZE;
    thiz->base.nanciDataBufSize = ISSADEC_MAX_ANCI_SIZE;
    thiz->base.CheckSupport = IssaDec_CheckSupport;
    thiz->base.SubtitleDecode = IssaDec_SubtitleDecode;
    thiz->base.Destruct = IssaDec_ItxtDec_Destruct;
    //初始化自己的成员
    memset(&g_ssaDecInfo, 0, sizeof(__ssa_dec_info_t));
	g_ssaDecInfo.txtEncType = CEDAR_SUB_ENCODE_UTF8;
    return EPDK_OK;

_err0:
    return EPDK_FAIL;
}
__s32 IssaDec_Exit(IssaDec *thiz)
{
    //因为会覆盖基类元素的缘故. 一般先释放自己的,再处理基类的.
    ItxtDec_Exit(&thiz->base);
    return EPDK_OK;
}

IssaDec *newIssaDec()
{
    __s32   tmpret;
    IssaDec *pIssaDec = (IssaDec*)malloc(sizeof(IssaDec));
    if(NULL == pIssaDec)
    {
        WARNING("malloc IssaDec fail\n");
        return NULL;
    }
    tmpret = IssaDec_Init(pIssaDec);
    if(tmpret != EPDK_OK)
    {
        WARNING("IssaDec init fail\n");
        goto _err0;
    }
    return pIssaDec;
_err0:
    free(pIssaDec);
    return NULL;
}
void deleteIssaDec(IssaDec *thiz)
{
    IssaDec_Exit(thiz);
    free(thiz);
}

