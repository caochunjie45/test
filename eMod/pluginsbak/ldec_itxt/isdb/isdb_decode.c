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
* File   : itxt_decode.c
* Version: V1.0
* By     : Eric_wang
* Date   : 2011-9-25
* Description:
********************************************************************************
*/
#include "isdbdec_s.h"
#include "isdb_decode.h"

/*******************************************************************************
Function name: ldec_itxt_init_info
Description: 
    1.LBSBUF, FrameQueue, ����һЩbuf�͹���buf�����ݽṹ�ĳ�ʼ��
Parameters: 
    
Return: 
    
Time: 2011/9/26
*******************************************************************************/
void ldec_isdb_init_info(IsdbDec *ldec_itxt)
{
    __u8    i = 0;

    //memset(ldec_itxt->anciDataBuf, 0, LDEC_MAX_TXT_ANCI_SIZE);
    memset(ldec_itxt->anciDataBuf, 0, ldec_itxt->nanciDataBufSize);
    memset(ldec_itxt->ldecLbsBuf.ldecBuf, 0, ldec_itxt->nLbsBufSize);
   
//    ldec_itxt->outputItem.uPts = 0;
//    ldec_itxt->outputItem.uDuration = 0;
//    memset(ldec_itxt->outputItem.pSubtitle, 0, LDEC_MAX_TXT_FRM_SIZE);
    
    ldec_itxt->ldecLbsBuf.readAddr = ldec_itxt->ldecLbsBuf.ldecBuf;
    ldec_itxt->ldecLbsBuf.writeAddr = ldec_itxt->ldecLbsBuf.ldecBuf;
    ldec_itxt->ldecLbsBuf.ldecIdxSize = 0;
    ldec_itxt->ldecLbsBuf.ldecWriteIdx = 0;
    ldec_itxt->ldecLbsBuf.ldecReadIdx  = 0;
    ldec_itxt->ldecLbsBuf.validSize   = 0;

    ldec_itxt->ldecManager.validSize = 0;
    ldec_itxt->ldecManager.reconBufIdx = 0;
    ldec_itxt->ldecManager.subReadIdx = 0;
    ldec_itxt->ldecManager.subWriteIdx = 0;

    //ldec_itxt->decFlag = 0;
    //ldec_itxt->useAnciBuf = 0;

    for(i = 0; i<LDEC_MAX_TXT_FRM_NUM; i++)
    {
        ldec_itxt->ldecManager.subIsFull[i] = 0;
    }
//    for(i=0; i<4; i++)
//    {
//        ldec_itxt->lastColorCode[i] = 0;
//        ldec_itxt->lastContrast[i] = 0;
//    }
}

static void ldec_fullCopy_subItem(CCedarSubtitleFrame *pdes, CCedarSubtitleFrame *psrc)
{
    //__s16   *pTmpPtsOffset = pdes->pPtsOffset;
    __s32   i = 0;
    __u8    *pTmpSubtitle = pdes->pSubtitle;
    __u8*   pTmpSubData[ISDBDEC_MAX_LINE_NUM];
    for(i=0; i<ISDBDEC_MAX_LINE_NUM; i++)
    {
        pTmpSubData[i] = pdes->subData[i];
    }   
    memcpy(pdes, psrc, sizeof(CCedarSubtitleFrame));
    //pdes->pPtsOffset = pTmpPtsOffset;
    //pdes->pSubtitle = pTmpSubtitle;
    for(i=0; i<ISDBDEC_MAX_LINE_NUM; i++)
    {
        pdes->subData[i] = pTmpSubData[i];
        
        if(psrc->subData[i]!=NULL && pdes->subData[i]!=NULL)
        {
            memset(pdes->subData[i], 0, ISDBDEC_MAX_FRM_SIZE);
            if(psrc->subDataLen[i]< 0 || psrc->subDataLen[i] > ISDBDEC_MAX_FRM_SIZE )
            {
                WARNING("fatal error! psrc->subDataLen[i]\n", psrc->subDataLen[i]);
            }
            else
            {
                memcpy(pdes->subData[i], psrc->subData[i], psrc->subDataLen[i]);
            }
        }        
    }  
    pdes->pSubtitle = pTmpSubtitle;
//    if(psrc->pPtsOffset !=NULL && pdes->pPtsOffset!=NULL)
//    {
//        memcpy(pdes->pPtsOffset, psrc->pPtsOffset, psrc->uPtsSize*sizeof(__s16));
//    }
    if(psrc->pSubtitle!=NULL && pdes->pSubtitle!=NULL)
    {
        if(psrc->uSubSize < 0 || psrc->uSubSize > LDEC_MAX_TXT_FRM_SIZE)
        {
            WARNING("fatal error! psrc->uSubSize[%x]\n", psrc->uSubSize);
        }
        else
        {
            memcpy(pdes->pSubtitle, psrc->pSubtitle, psrc->uSubSize);
        }
    }
}

__s32 Impl_IsdbDec_IoCtrl(IsdbDec *thiz, __s32 cmd, __s32 aux, void *pbuffer)  //cmd = ItxtDecCmd
{
    __s32   ret = EPDK_FAIL;
    
    switch(cmd)
    {
        case ITXTDEC_CMD_SET_SUBTITLE_SOURCE:
        {
            thiz->subtitle_bs_src = aux;
            ret = EPDK_OK;
            break;
        }
        case ITXTDEC_CMD_NOTIFY_SYNCSTAT:
        {
            if(thiz->nSyncStatFlag)
            {
                WARNING("fatal error! thiz->nSyncStatFlag[%d]\n", thiz->nSyncStatFlag);
            }
            thiz->nSyncStatFlag = 1;
            ret = EPDK_OK;
            break;
        }
        case ITXTDEC_CMD_CHECK_SYNCSTAT:
        {
            ret = thiz->nSyncStatFlag;
            break;
        }
        case ITXTDEC_CMD_SET_ONESEGFLAG:
        {
            thiz->nOneSegFlag = aux;
            ret = EPDK_OK;
            break;
        }
        case ITXTDEC_CMD_SET_CHAR_CODING_TYPE:
        {
            thiz->nCharEncodeType = (enum __CEDARLIB_LYRIC_ENCODE_TYPE)aux;
            ret = EPDK_OK;
            break;
        }
        case ITXTDEC_CMD_SET_FRMRATE:
        case ITXTDEC_CMD_SET_FRMSIZE:
        case ITXTDEC_CMD_SET_SCREEN_SIZE:
        default:
        {
            WARNING("Unknown cmd[%x]\n", cmd);
            ret = EPDK_FAIL;
            break;
        }
    }
    return ret;
}

/*******************************************************************************
Function name: Impl_ItxtDec_RequestWrite
Description: 
    1. bufsize����,����fail; buf����Ŀ���������Ҳ����fail
    2. requestwrite��ȡ������,���Ч��
Parameters: 
    
Return: 
    EPDK_FAIL;
    EPDK_OK
Time: 2011/9/26
*******************************************************************************/
__s32 Impl_IsdbDec_RequestWrite(IsdbDec *thiz, __u32 uSizeRequested, __u8 **ppData0, __u32 *puSizeGot0, __u8 **ppData1, __u32 *puSizeGot1)
{
    __s32   ret;
    __u32 remainBufSize;
    __u32   nSize0 = 0, nSize1 = 0;
    
    __u16   ldecIdxSize; //lbs buffer�Ĺ������Ѿ�ʹ�õĸ���
    __u32   validSize; //LBS buffer����Ч�ֽ���, ���������ִ���ڼ�,validsizeֻ���ܱ��,��������ν
    __u8    *writeAddr;    //��ǰ��д��ַ, �ڴ��ڼ�, writeAddrһֱ�����

    //�������,��Ϊ������ֻһ��.ֻ��ͨ����������decode()����.
    thiz->LbsBufLock.lock(&thiz->LbsBufLock);
    ldecIdxSize = thiz->ldecLbsBuf.ldecIdxSize; //lbs buffer�Ĺ������Ѿ�ʹ�õĸ���
    validSize = thiz->ldecLbsBuf.validSize; //LBS buffer����Ч�ֽ���, ���������ִ���ڼ�,validsizeֻ���ܱ��,��������ν
    writeAddr = thiz->ldecLbsBuf.writeAddr;    //��ǰ��д��ַ, �ڴ��ڼ�, writeAddrһֱ�����
    remainBufSize = thiz->nLbsBufSize - validSize;
    if(remainBufSize < uSizeRequested)
    {
        WARNING("lbs buf underflow! [%d][%d]\n", remainBufSize, uSizeRequested);
        ret = EPDK_FAIL;
        goto _exit0;
    }
    if(ldecIdxSize >= LDEC_MAX_TXT_DATA_NUM)
    {
        WARNING("[%d] >= LDEC_MAX_TXT_DATA_NUM[%d]\n", ldecIdxSize, LDEC_MAX_TXT_DATA_NUM);
        ret = EPDK_FAIL;
        goto _exit0;
    }
    //��ʼ��д����Ĳ���
    nSize0 = thiz->ldecLbsBuf.ldecBufEnd + 1 - writeAddr;
    if(nSize0 >= uSizeRequested)
    {
        *ppData0 = writeAddr;
        *puSizeGot0 = uSizeRequested;
        *ppData1 = NULL;
        *puSizeGot1 = 0;
    }
    else
    {
        nSize1 = uSizeRequested - nSize0;
        *ppData0 = writeAddr;
        *puSizeGot0 = nSize0;
        *ppData1 = thiz->ldecLbsBuf.ldecBuf;
        *puSizeGot1 = nSize1;
    }
    ret = EPDK_OK;
    goto _exit0;

_exit0:
    thiz->LbsBufLock.unlock(&thiz->LbsBufLock);
    return ret;
}

/*******************************************************************************
Function name: Impl_ItxtDec_UpdatePointer
Description: 
    1.����LbsBuf
    2. ����mkv��mpg psr��û����lastPartctrl, ֻ��tspsr����.
        ��mkv,mpg�����subtitle type�� itxt, issa, isub,������3�ָ�ʽ�ڱ������Զ�����
        firstctrlbit��lastctrlbit.
        ϣ���Ժ���ͳһ����firstctrlbit��lastctrlbit.
        isub��������ʵ�ǲ�����firstctrlbit�ȵ�

        ����Ժ�mkvҪ��pgs��Ļ֧��,�ͱ����޸�mkv_psr������firstctrlbit��lastctrlbit

    3. ��Ҫ����

    4. chunk��Ϣ�����︳��lbsbuf manager.
Parameters: 
    uDataSize:��д��buffer�����ݳ���
Return: 
    
Time: 2011/9/26
*******************************************************************************/
__s32 Impl_IsdbDec_UpdatePointer(IsdbDec *thiz, __lbs_datctlblk_t * pdatctl,__u32 uDataSize)
{
    __u16 writeIdx; //�ڴ��ڼ䲻���
    __s32 remainBufSize;
    //���Ȱ�LDEC_FIRST_PART_CTRL��������, �����Դ��mkv����vob, �Զ�����,Ŀǰ������,��Ϊmkv, mpg psrû��first ctrl bit
//    if(CEDARLIB_FILE_FMT_MKV == thiz->subtitle_bs_src || CEDARLIB_FILE_FMT_VOB == thiz->subtitle_bs_src)
//    {
//        pdatctl->CtrlBits |= LDEC_FIRST_PART_CTRL;
//        pdatctl->CtrlBits |= LDEC_LAST_PART_CTRL;
//    }
    //�����������
    thiz->LbsBufLock.lock(&thiz->LbsBufLock);
    if(thiz->ldecLbsBuf.ldecIdxSize >= LDEC_MAX_TXT_DATA_NUM)
    {
        WARNING("fatal error! ldecIdxSize[%d]>=LDEC_MAX_TXT_DATA_NUM[%d] \n", thiz->ldecLbsBuf.ldecIdxSize, LDEC_MAX_TXT_DATA_NUM);
    }
    writeIdx = thiz->ldecLbsBuf.ldecWriteIdx;
    if(pdatctl->CtrlBits & LDEC_FIRST_PART_CTRL)
    {
        memset(&thiz->ldecLbsBuf.ldecBufCrl[writeIdx],0x00, sizeof( __ldec_buffer_ctrl_t));
        thiz->ldecLbsBuf.ldecBufCrl[writeIdx].startAddr = thiz->ldecLbsBuf.writeAddr;
        thiz->ldecLbsBuf.ldecBufCrl[writeIdx].lbsChgFlg = pdatctl->LbsChgFlg;
    }
    else
    {
        WARNING("not meet FIRST_PART_CTRL.uDataSize[%d]\n", uDataSize);
    }

    if(pdatctl->CtrlBits & LDEC_PTS_VALID_CTRL)
    {
        thiz->ldecLbsBuf.ldecBufCrl[writeIdx].hasPtsFlag = 1;
        thiz->ldecLbsBuf.ldecBufCrl[writeIdx].ptsVal = pdatctl->uPTS;
        //WARNING("updatepointer: lbs chunk pts[%d]\n", (__s32)pdatctl->uPTS);
    }
    
    if(pdatctl->CtrlBits & LDEC_DURATION_VALID_CTRL)
    {
        thiz->ldecLbsBuf.ldecBufCrl[writeIdx].duration = pdatctl->uDuration;
        //WARNING("updatepointer: lbs chunk duration[%d]\n", (__s32)pdatctl->uDuration);
    }

    if(pdatctl->privateData != NULL)
    {
        memcpy(thiz->ldecLbsBuf.ldecBufCrl[writeIdx].privateData, pdatctl->privateData, LDEC_PRIVATE_DATA_LEN);
    }
   
    if(pdatctl->CtrlBits & LDEC_DATATYPE_VALID_CTRL)
    {
        thiz->ldecLbsBuf.ldecBufCrl[writeIdx].dataType = pdatctl->uDatatype;
    }
    
    //thiz->ldecLbsBuf.validSize += uDataSize;
    thiz->ldecLbsBuf.ldecBufCrl[writeIdx].dataLen += uDataSize;
    if((thiz->ldecLbsBuf.writeAddr+uDataSize) > thiz->ldecLbsBuf.ldecBufEnd)
    {
        remainBufSize = thiz->ldecLbsBuf.ldecBufEnd - thiz->ldecLbsBuf.writeAddr+1;
        thiz->ldecLbsBuf.writeAddr = thiz->ldecLbsBuf.ldecBuf + uDataSize - remainBufSize;
        thiz->ldecLbsBuf.ldecBufCrl[writeIdx].coverBufEnd = 1;
    }
    else
    {
        if(thiz->ldecLbsBuf.writeAddr + uDataSize == thiz->ldecLbsBuf.ldecBufEnd + 1)
        {
            thiz->ldecLbsBuf.writeAddr = thiz->ldecLbsBuf.ldecBuf;
        }
        else
        {
            thiz->ldecLbsBuf.writeAddr += uDataSize;
        }
        //thiz->ldecLbsBuf.ldecBufCrl[writeIdx].coverBufEnd = 0;
    }

    if(pdatctl->CtrlBits & LDEC_LAST_PART_CTRL)
    {
        //WARNING("update chunk![%d][%d][%d]\n", thiz->ldecLbsBuf.ldecWriteIdx, thiz->ldecLbsBuf.ldecBufCrl[writeIdx].dataLen, uDataSize);
        thiz->ldecLbsBuf.validSize += thiz->ldecLbsBuf.ldecBufCrl[writeIdx].dataLen;
        if(thiz->ldecLbsBuf.ldecWriteIdx + 1 == LDEC_MAX_TXT_DATA_NUM)
        {
            thiz->ldecLbsBuf.ldecWriteIdx = 0;
        }
        else
        {
            thiz->ldecLbsBuf.ldecWriteIdx++;
        }
        thiz->ldecLbsBuf.ldecIdxSize ++;
    }
    else
    {
        WARNING("not meet LAST_PART_CTRL.uDataSize[%d]\n", uDataSize);
    }
    thiz->LbsBufLock.unlock(&thiz->LbsBufLock);

    return EPDK_OK;
}

/*******************************************************************************
Function name: compareWithPrevSubtitle
Description: 
    1.�Ƚ���һ������ȥ����Ļ�͵�ǰ�ҵ�����Ļ�Ƿ���ͬһ��
    ע�����isdb��Ļ,����һ�����:����������Ļ����һ��,������ʼpts��һ��.
    ��һ��û��ʱ����Ϣ��ʾһֱ����,�ڶ�����ʱ����Ϣ�˸����˼����ĳ���ʱ��.���ǰ���������µ�������Ļ��Ϊͬһ����Ļ.
    ����:[ISR]TMCC_003.TS
Parameters: 
    pOutputItem: ��һ������ȥ����Ļ��
    pCurFrame:��ǰ�ҵ����µ���Ļ��
Return: 
    EPDK_OK:ͬһ��
    EPDK_FAIL: ����ͬһ��
    1: ��Ȼ��ʼpts����ͬ,����ǰһ���ĳ���ʱ��Ϊ����,��һ�������ݺ�ǰһ����ȫһ��.
        ������ָ��ɫ,�����С, λ��, �;����ַ�������.
Time: 2012/7/28
*******************************************************************************/
__s32 compareWithPrevSubtitle(CCedarSubtitleFrame *pOutputItem, CCedarSubtitleFrame *pCurFrame)
{
    __s32   i;
    __s32   nDiffFlag = 0;
    if(0 == pOutputItem->uPts)
    {
        WARNING("SubDec: Outputframe pts == 0!\n");
        return EPDK_FAIL;
    }
    if(pOutputItem->uPts == pCurFrame->uPts)
    {
        if(pOutputItem->uDuration == pCurFrame->uDuration)    //�Ƚ�һ���ǲ���ͬһ����Ļ��
        {
            return EPDK_OK;
        }
        else
        {
            WARNING("SubDec:same pts[%d], diff uDuration[%d][%d]\n", pOutputItem->uPts, pOutputItem->uDuration, pCurFrame->uDuration);
            return EPDK_FAIL;
        }
    }
    //���pts��һ��,��Ҫ��������
    //1.�ȿ���һ����Ļ�ĳ���ʱ��,��Ϊ���޳�,����Ϊ������Ļ��һ��.
    if(pOutputItem->uDuration < ISDB_FRAME_MAX_DURATION)
    {
        return EPDK_FAIL;
    }
    //2.��������޳�,����ʽ��ʼ�Ƚ�����
    if(pOutputItem->subNum != pCurFrame->subNum) //�����ȱȽ�.
    {
        return EPDK_FAIL;
    }
    if(pOutputItem->position_flags != pCurFrame->position_flags)    //����λ��
    {
        WARNING("impossible? subtitle position is diff![%d][%d]\n", pOutputItem->position_flags, pCurFrame->position_flags);
        return EPDK_FAIL;
    }
    //���бȽ�, ��ɫ,�����С, λ��, �����ַ�������.
    for(i=0; i<pCurFrame->subNum; i++)
    {
        if(pOutputItem->subColor[i] != pCurFrame->subColor[i])  //��ɫ
        {
            nDiffFlag = 1;
            break;
        }
        if(pOutputItem->subStyleSize[i] != pCurFrame->subStyleSize[i])  //�����С
        {
            nDiffFlag = 1;
            break;
        }
        if(pCurFrame->position_flags)   //��λ����Ϣ,�ͱȽϸ���λ��
        {
            if(pOutputItem->line[i] != pCurFrame->line[i]) //�е�λ��
            {
                nDiffFlag = 1;
                break;
            }
            if(pOutputItem->forward[i] != pCurFrame->forward[i]) //ÿһ�е��ֵ���ʼλ��
            {
                nDiffFlag = 1;
                break;
            }
        }
        if(pOutputItem->subDataLen[i] != pCurFrame->subDataLen[i])  //�ַ�������
        {
            nDiffFlag = 1;
            break;
        }
        if(memcmp(pOutputItem->subData[i], pCurFrame->subData[i], pCurFrame->subDataLen[i]))
        { //eLIBs_memcmp():��ͬ����1,��ͬ����0. ϵͳ��memcmp():��ͬ����0,��ͬ���ز�ֵ.
            nDiffFlag = 1;
            break;
        }
    }
    if(nDiffFlag)
    {
        return EPDK_FAIL;
    }
    else
    {
        WARNING("prevSubPts[%d],dur[%d], curPts[%d],dur[%d], two subtitle content is same! We will think they are one!\n", 
            pOutputItem->uPts, pOutputItem->uDuration, pCurFrame->uPts, pCurFrame->uDuration);
        return 1;
    }
}

/*******************************************************************************
Function name: Impl_ItxtDec_GetLrcItem
Description: 
    1.ԭ����: ����ж�����Ļ��ʱ�䷶Χ����Ҫ��,ȡ���һ���ͳ�,����ĺ��Ե�.
    2. Ĭ�ϵ�֡���еĹ�����:
        ��ʱ��˳�����.
    3.������.
        pLyricDecoder���п��ܱ�destroy.Ҳû������.
    4.ȡ��һ����Ļ��,����Ļû�����̶���,��Ϊ����Ҫ���pts�ķ�Χ��. ��ʹpOutputItem�����,Ҳ������.
    5.�Ի����ԭ����:�����ݲ�����,��д����ʱ����.����������ݵ�ֵ�ı�,��Ӱ�������ȷ��ǰ����.
        ����,��Ϊû��ʲô�ȴ�����,ȫ������,����Ӱ������.
Parameters: 
    pOutputItem:�����������item,���.
Return: 
    EPDK_OK;
Time: 2011/9/26
*******************************************************************************/
__s32 Impl_IsdbDec_GetIsdbItem(IsdbDec *thiz, __u32 nPts, CCedarSubtitleFrame *pOutputItem, __s32 nOutputSubtitleSize)
{
    __u8    idx = 0;    //һ�����subItem[]
    __u32   timeSmall = 0;  //һ����Ļ�����ʼʱ��
    __s32   timeLarge = 0;  //һ����Ļ��Ľ���ʱ��
    __s32   ret = EPDK_FAIL;
    __s32   nFindFlag = 0;  //��ʶ�Ƿ��ҵ����ʵ���Ļ��,���������Ļ����Ҫ��ȡ���һ����Ļ��
    __s32   nNewsubReadIdxFlag = 0; //��ʾ�Ѿ�ȷ�����µĿ�ʼ����Ļ���idx
    __s32   validSize;
    __s32   subReadIdx;     //���__u8   subIdxBuf[].
    __s32   nNewsubReadIdx = 0;   //ͨ��while()ѭ�����Һ�,�µ���Ч��subReadIdx.����֮ǰ��idxȫ��ɾ��.�����__u8   subIdxBuf[]. �µ���Ч����Ļ�һ���Ǻ��ʵķ���ʱ�䷶Χ����Ļ��,�п����Ҳ������ʵ���Ļ��.
    __s32   nSuitsubReadIdx = 0;   //�ҵ��ĺ��ʵ���Ļ���idx, ���__u8   subIdxBuf[].
    //__s32   tmp;
    __s32   i;
    __s32   nSubDiffFlag = 0;
    
    //osal_switch_disable();
    thiz->FrmMgrLock.lock(&thiz->FrmMgrLock);
    validSize = thiz->ldecManager.validSize;
    //WARNING(" validSize = [%d]\n", thiz->ldecManager.validSize);
    subReadIdx = thiz->ldecManager.subReadIdx;
    if(validSize > 0)
    {
        idx = thiz->ldecManager.subIdxBuf[subReadIdx];
        timeSmall = thiz->ldecManager.subItem[idx].uPts;
        //WARNING(" timeSmall = [%d];  nPts  = [%d]!\n", timeSmall, nPts);
//        if(nPts < timeSmall)
//        {
//            WARNING("## No SuitItem! subReadIdx[%d], idx[%d], validSize[%d], pts[%d] > desPts[%d]\n", subReadIdx, idx, thiz->ldecManager.validSize, timeSmall, nPts);
            //ʱ��δ��,�����ٲ���,Ҳ��ɾ��
//            ret = EPDK_FAIL;
//            goto _exit0;
//        }
    }
    else
    {
        WARNING("subitem validSize == 0!\n");
        ret = EPDK_FAIL;
        goto _exit0;
    }
    //ͨ��while()ѭ��,ȷ����Щ��Ļ��ĿҪ����,ȷ���Ƿ��к��ʵ���Ļ��, �����һ���ʺϵ���Ļ��.
    while(validSize > 0)
    {
        idx = thiz->ldecManager.subIdxBuf[subReadIdx];
        timeSmall = thiz->ldecManager.subItem[idx].uPts;
        timeLarge = thiz->ldecManager.subItem[idx].uPts + thiz->ldecManager.subItem[idx].uDuration;
        //WARNING(" subtitle: timeSmall = [%d];  nPts  = [%d]!\n", timeSmall, nPts);
        if(nPts < timeSmall)
        {
            if(0 == nNewsubReadIdxFlag)
            {
                nNewsubReadIdx = subReadIdx;
                nNewsubReadIdxFlag = 1;
            }
        }
        else if(nPts >= timeSmall && nPts <= timeLarge)
        {
            if(nFindFlag)
            {
                WARNING("find suitable subitem again!\n");
            }
           
            nFindFlag = 1;
            nSuitsubReadIdx = subReadIdx;
            if(nNewsubReadIdxFlag)
            {
                WARNING("find NewsubReadIdx subitem again!\n");
            }
            nNewsubReadIdxFlag = 1;
            nNewsubReadIdx = subReadIdx;
         }
        validSize--;
        subReadIdx++;
        if(subReadIdx == LDEC_MAX_TXT_FRM_NUM)
        {
            subReadIdx = 0;
        }
    }
    
    if(nFindFlag)
    {
        idx = thiz->ldecManager.subIdxBuf[nSuitsubReadIdx];
        if(nOutputSubtitleSize <=thiz->ldecManager.subItem[idx].uSubSize)
        {
            WARNING("FATAL error! subtitleSize exceed!idx[%d], [%d][%d]\n", idx, nOutputSubtitleSize, thiz->ldecManager.subItem[idx].uSubSize);
        }

        //�ж���Ҫ������Ļ����һ�θ���ȥ����Ļ�ǲ���ͬһ��,
        //ע�����isdb��Ļ,����һ�����:����������Ļ����һ��,������ʼpts��һ��.
        //��һ��û��ʱ����Ϣ��ʾһֱ����,�ڶ�����ʱ����Ϣ�˸����˼����ĳ���ʱ��.���ǰ���������µ�������Ļ��Ϊͬһ����Ļ.
        //����:[ISR]TMCC_003.TS
//        if(pOutputItem->uPts!=0 && pOutputItem->uPts == thiz->ldecManager.subItem[idx].uPts 
//            && pOutputItem->uDuration == thiz->ldecManager.subItem[idx].uDuration)    //�Ƚ�һ���ǲ���ͬһ����Ļ��
        nSubDiffFlag = compareWithPrevSubtitle(pOutputItem, &thiz->ldecManager.subItem[idx]);
        if(EPDK_OK == nSubDiffFlag)
        {
            //��ͬ����Ļ��,���ÿ���
        }
        else if(1 == nSubDiffFlag)
        {
            //�ı䵱ǰ��Ļ�����ʼ��pts����Ϊ������Ļ��pts,��ʾ��ͬһ����Ļ.
            __u32   nTmpPts = thiz->ldecManager.subItem[idx].uPts;
            __u32   ntmpDuration = thiz->ldecManager.subItem[idx].uDuration;
            if(nTmpPts > pOutputItem->uPts)
            {
                ntmpDuration = nTmpPts - pOutputItem->uPts + ntmpDuration;
            }
            else
            {
                WARNING("fatal error! two same successive subtitle pts[%d]>=[%d]\n", pOutputItem->uPts, nTmpPts);
            }
            thiz->ldecManager.subItem[idx].uPts = pOutputItem->uPts;
            thiz->ldecManager.subItem[idx].uDuration = ntmpDuration;
            ldec_fullCopy_subItem(pOutputItem,&thiz->ldecManager.subItem[idx]);
        }
        else
        {
            ldec_fullCopy_subItem(pOutputItem,&thiz->ldecManager.subItem[idx]);
        }
        ret = EPDK_OK;
        if(nSuitsubReadIdx != nNewsubReadIdx)
        {
            WARNING("## fatal error! SuitReadIdx[%d], newReadIdx[%d]\n", nSuitsubReadIdx, nNewsubReadIdx);
        }
    }
    else
    {
        ret = EPDK_FAIL;
    }

    //����frame queue
    if(0 == nNewsubReadIdxFlag)    //û��ȷ��,��ζ��ȫ��ɾ��
    {
        if(nFindFlag)
        {
            WARNING("fatal error! find LrcItem has wrong!\n");
        }
        //nNewsubReadIdx = thiz->ldecManager.subWriteIdx;
        while(thiz->ldecManager.validSize)
        {
            if(thiz->ldecManager.validSize <= 0)
            {
                WARNING("fatal error! validSize[%d], check code!\n", thiz->ldecManager.validSize);
            }
            idx = thiz->ldecManager.subReadIdx;
            idx = thiz->ldecManager.subIdxBuf[idx];
            thiz->ldecManager.subIsFull[idx] = 0;
            if(thiz->ldecManager.subReadIdx + 1 == LDEC_MAX_TXT_FRM_NUM)
            {
                thiz->ldecManager.subReadIdx = 0;
            }
            else
            {
                thiz->ldecManager.subReadIdx++;
            }
            thiz->ldecManager.validSize--;
        }
    }
    else
    {
        if(0 == thiz->ldecManager.validSize)
        {
            WARNING("fatal error! ldecManager validSize == 0, check code!\n");
        }
        
        while(thiz->ldecManager.subReadIdx != nNewsubReadIdx)
        {
            if(thiz->ldecManager.validSize <= 0)
            {
                WARNING("fatal error! validSize[%d], check code!\n", thiz->ldecManager.validSize);
            }
            idx = thiz->ldecManager.subReadIdx;
            idx = thiz->ldecManager.subIdxBuf[idx];
            thiz->ldecManager.subIsFull[idx] = 0;
            if(thiz->ldecManager.subReadIdx + 1 == LDEC_MAX_TXT_FRM_NUM)
            {
                thiz->ldecManager.subReadIdx = 0;
            }
            else
            {
                thiz->ldecManager.subReadIdx++;
            }
            thiz->ldecManager.validSize--;
        }    
    }
    _exit0:
    //osal_switch_enable();
    thiz->FrmMgrLock.unlock(&thiz->FrmMgrLock);

    if(ret == EPDK_OK)
    {
        //for(i=0; i<ISDBDEC_MAX_LINE_NUM; i++)
        if(pOutputItem->subNum >= ISDBDEC_MAX_LINE_NUM)
        {
            WARNING("fatal error! pOutputItem->subNum[%d] >= ISDBDEC_MAX_LINE_NUM[%d]\n", pOutputItem->subNum, ISDBDEC_MAX_LINE_NUM);
        }
        for(i=0; i<pOutputItem->subNum; i++)
        {
            if((pOutputItem->subData[i] != NULL) && (pOutputItem->subDataLen[i] < ISDBDEC_MAX_FRM_SIZE))
                *(pOutputItem->subData[i]+pOutputItem->subDataLen[i]) = 0; 
        }  
        if(pOutputItem->pSubtitle != NULL)
            pOutputItem->pSubtitle[pOutputItem->uSubSize] = 0;
    }
    return ret;
}

/*******************************************************************************
Function name: Impl_ItxtDec_Open
Description: 
    1. ��lbsbuf��framequeue�����ڴ�
Parameters: 
    
Return: 
    
Time: 2011/9/26
*******************************************************************************/
__s32 Impl_IsdbDec_Open(IsdbDec *thiz)
{
    __s32   i;
    __s32   j;
    //step-2 create subdev  handle
    if(thiz->nLbsBufSize <= 0 || thiz->nLdecFrameSize <= 0)
    {
        WARNING("fatal error! check nLbsBufSize[%x], [%x]\n", thiz->nLbsBufSize, thiz->nLdecFrameSize);
    }
    thiz->ldecLbsBuf.ldecBuf = (__u8 *)PHY_MALLOC(sizeof(__u8)*thiz->nLbsBufSize, 1024);
    if(thiz->ldecLbsBuf.ldecBuf == NULL)
    {
        WARNING("malloc lbsbuf[%d] fail\n", thiz->nLbsBufSize);
        goto _err_mopen;
    }
    thiz->ldecLbsBuf.ldecBufEnd = thiz->ldecLbsBuf.ldecBuf + thiz->nLbsBufSize - 1;

    for(i=0; i<LDEC_MAX_TXT_FRM_NUM; i++)
    {
        thiz->ldecManager.subItem[i].pSubtitle = (__u8 *)PHY_MALLOC(sizeof(__u8)*thiz->nLdecFrameSize, 1024);
        if(thiz->ldecManager.subItem[i].pSubtitle == NULL)
            goto _err_mopen;
        //thiz->ldecManager.subItem[i].pPtsOffset = NULL;
    }

    for(i=0; i<LDEC_MAX_TXT_FRM_NUM; i++)
    {
        for(j=0; j<ISDBDEC_MAX_LINE_NUM; j++)
        {
            thiz->ldecManager.subItem[i].subData[j]= (__u8 *)malloc(ISDBDEC_MAX_FRM_SIZE);
            if(thiz->ldecManager.subItem[i].subData[j]== NULL)
                goto _err_mopen;
        }
        //thiz->ldecManager.subItem[i].pPtsOffset = NULL;
    }
    //����������
//    for(i=1; i < LDEC_MAX_TXT_FRM_NUM; i++)
//    {        
//        thiz->ldecManager.subItem[i-1].nextItem = &thiz->ldecManager.subItem[i];
//        thiz->ldecManager.subItem[i-1].index = i-1;
//    }
//    i = LDEC_MAX_TXT_FRM_NUM;
//    thiz->ldecManager.subItem[i-1].nextItem = &thiz->ldecManager.subItem[0];
//    thiz->ldecManager.subItem[i-1].index = i-1;
            
    thiz->anciDataBuf = (__u8 *)PHY_MALLOC(sizeof(__u8)*thiz->nanciDataBufSize, 1024);
    if(thiz->anciDataBuf == NULL)
    {
        WARNING("malloc anciDataBuf[%d] fail\n", thiz->nanciDataBufSize);
        goto _err_mopen;
    }
    ldec_isdb_init_info(thiz);
    return LDEC_ERR_NONE;
    
_err_mopen:

    for(i=0; i<LDEC_MAX_TXT_FRM_NUM; i++)
    {
        for(j=0; j<ISDBDEC_MAX_LINE_NUM; j++)
        {
            if (thiz->ldecManager.subItem[i].subData[j] != NULL )
            {
                free(thiz->ldecManager.subItem[i].subData[j]);
                thiz->ldecManager.subItem[i].subData[j] = NULL;
            }
        }
        //thiz->ldecManager.subItem[i].pPtsOffset = NULL;
    }
    if(thiz->anciDataBuf)
    {
        PHY_FREE(thiz->anciDataBuf);
        thiz->anciDataBuf = NULL;
    }
    if(thiz->ldecLbsBuf.ldecBuf!= NULL)
    {
        PHY_FREE(thiz->ldecLbsBuf.ldecBuf);
        thiz->ldecLbsBuf.ldecBuf = (__u8 *)0;
    }
    for(i=0; i<LDEC_MAX_TXT_FRM_NUM; i++)
    {
        if(thiz->ldecManager.subItem[i].pSubtitle !=NULL)
        {
            PHY_FREE(thiz->ldecManager.subItem[i].pSubtitle);
            thiz->ldecManager.subItem[i].pSubtitle = (__u8 *)0;
        }
    }
    return LDEC_ERR_DEC_SUBTITLE;
}

__s32 Impl_IsdbDec_Close(IsdbDec *thiz)
{
    __s32   i, j;
    if(thiz->anciDataBuf)
    {
        PHY_FREE(thiz->anciDataBuf);
        thiz->anciDataBuf = NULL;
    }
    if(thiz->ldecLbsBuf.ldecBuf!= NULL)
    {
        PHY_FREE(thiz->ldecLbsBuf.ldecBuf);
        thiz->ldecLbsBuf.ldecBuf = (__u8 *)0;
    }
    for(i=0; i<LDEC_MAX_TXT_FRM_NUM; i++)
    {
        if(thiz->ldecManager.subItem[i].pSubtitle !=NULL)
        {
            PHY_FREE(thiz->ldecManager.subItem[i].pSubtitle);
            thiz->ldecManager.subItem[i].pSubtitle = (__u8 *)0;
        }
    }
    for(i=0; i<LDEC_MAX_TXT_FRM_NUM; i++)
    {
        for(j=0; j<ISDBDEC_MAX_LINE_NUM; j++)
        {
            if (thiz->ldecManager.subItem[i].subData[j] != NULL )
            {
                free(thiz->ldecManager.subItem[i].subData[j]);
                thiz->ldecManager.subItem[i].subData[j] = NULL;
            }
        }
        //thiz->ldecManager.subItem[i].pPtsOffset = NULL;
    }
    return LDEC_ERR_NONE;
}
/*******************************************************************************
Function name: resetCCedarSubtitleFrame
Description: 
    ����CCedarSubtitleFrame�����ݽṹ.������д�Լ���Ҫ���õ�.�Ժ������Ҫ������.
Parameters: 
    
Return: 
    
Time: 2012/7/26
*******************************************************************************/
static __s32 resetCCedarSubtitleFrame(CCedarSubtitleFrame *pFrame)
{
    __s32   i;
    pFrame->isFull = 0;
    pFrame->subNum = 0;
    pFrame->position_flags = 0;
    pFrame->uPts = 0;
    
    for(i=0;i<ISDBDEC_MAX_LINE_NUM;i++)
    {
        pFrame->subDataLen[i] = 0;
    }

    return LDEC_ERR_NONE;
}

/*******************************************************************************
Function name: Base_ItxtDec_Decode
Description: 
    1.ע��thiz->nSyncStatFlag.�����ڽ���ǰ����buffer.\
    2. ע��ú����ǻ���ʵ�ֵģ����itext, issa����������Ļ����,�����ֽ�������
    ��lbs chunk��Ҫ����:chunk�ı߽�ͽ��������ϵ����ݿ����ϸ��һһ��Ӧ��ϵ.�������ݿ�
    ������һ����Ļ֡,����itext,issa;Ҳ�п�����һ��������,��tspsr��pgs��Ļ��������Ļ�������Եõ�
    һ����Ļ.
    3.isub������,��Ϊblock����Ļ֡û���ϸ�Ķ�Ӧ��ϵ,�����Լ���һ��lbs�������,����isub���ǻ���decode(),
    ��ȫ�Լ�ʵ��.
Parameters: 
    
Return: 
    
Time: 2011/9/27
*******************************************************************************/
__s32 Base_IsdbDec_Decode(IsdbDec *thiz)
{
    __s32 ret = 0;
    __u8  reconIdx = 0; //ָʾsubItem[]�е�һ��Ԫ��,��һ����Ԫ��.
    __u8  writeIdx = 0;
    __u8  readIdx= 0;   //lbsbuf�����ݿ����������
    __u32 usedBytes = 0;    //����ǰ�ǽ���ǰ��lbs�ĳ���,��������õ���lbs�ĳ���
    __u8  *curDataPtr = NULL;
    __s8  lbsChgFlg;
    //__maple_isdb_item_t *curSuItem;
    CCedarSubtitleFrame *curSuItem;
    __u8 *privateData;

    if(thiz->nSyncStatFlag) //socket_task()�Ѿ����Ա�֤,��sync�ڼ��������requestwrite(), updatepointer()��.
    {
        thiz->LbsBufLock.lock(&thiz->LbsBufLock);
        thiz->FrmMgrLock.lock(&thiz->FrmMgrLock);
        ldec_isdb_init_info(thiz);
        thiz->LbsBufLock.unlock(&thiz->LbsBufLock);
        thiz->FrmMgrLock.unlock(&thiz->FrmMgrLock);
        thiz->nSyncStatFlag = 0;
    }
    
    /*step 0 select frame manager*/
    if(thiz->ldecLbsBuf.validSize == 0)
        return LDEC_ERR_LBS_UNDERFLOW;

    while(reconIdx < LDEC_MAX_TXT_FRM_NUM)
    {
        if(thiz->ldecManager.subIsFull[reconIdx] == 0)
            break;
        reconIdx++;
    }
    if(reconIdx == LDEC_MAX_TXT_FRM_NUM)
        return LDEC_ERR_NO_FRAMEBUFFER;
    else
        thiz->ldecManager.reconBufIdx = reconIdx;

    readIdx = thiz->ldecLbsBuf.ldecReadIdx;
    curSuItem = &(thiz->ldecManager.subItem[reconIdx]);
    resetCCedarSubtitleFrame(curSuItem);
    if(0 == thiz->ldecLbsBuf.ldecBufCrl[readIdx].hasPtsFlag)
    {
        WARNING("fatal error! lbsbufctrl hasPtsFlag = 0, ptsVal\n");
    }
    curSuItem->uPts      = thiz->ldecLbsBuf.ldecBufCrl[readIdx].ptsVal;
    curSuItem->uDuration = thiz->ldecLbsBuf.ldecBufCrl[readIdx].duration;
    curSuItem->uDataType = thiz->ldecLbsBuf.ldecBufCrl[readIdx].dataType;
    curSuItem->uSubSize  = thiz->ldecLbsBuf.ldecBufCrl[readIdx].dataLen;    //uSubSize����Ӧ�ñ�ʾ��������Ļ���ݵĳ���,�����ǽ���ǰ����Ļ���ݵĳ���
    //curSuItem->uPtsSize  = 0;
    lbsChgFlg = thiz->ldecLbsBuf.ldecBufCrl[readIdx].lbsChgFlg;
    //WARNING("ldec_itxt BaseDecode: rd_idx[%d], pts[%d], duration[%d], datalen[%d]\n", readIdx, curSuItem->uPts, curSuItem->uDuration, curSuItem->uSubSize);
    //curDataPtr = ldec_judge_subdata_cover_bufEnd((void *) 0);
    privateData = thiz->ldecLbsBuf.ldecBufCrl[readIdx].privateData;
    curDataPtr = ldec_process_chunk_cover_bufEnd(&thiz->ldecLbsBuf, &thiz->anciDataBuf, &thiz->nanciDataBufSize);
    
    if(curDataPtr == NULL)
    {  
        thiz->LbsBufLock.lock(&thiz->LbsBufLock);
        ldec_update_read_pointer(&thiz->ldecLbsBuf, thiz->ldecLbsBuf.validSize);
        thiz->LbsBufLock.unlock(&thiz->LbsBufLock);
        return  LDEC_ERR_LFMT_ERR;
    }
    
    //ldec_itxt->decFlag = 1;
    usedBytes = thiz->ldecLbsBuf.ldecBufCrl[readIdx].dataLen;
    ret = thiz->SubtitleDecode(thiz, curSuItem, curDataPtr, &usedBytes, lbsChgFlg, privateData);
    
//    if(ldec_itxt->subtitle_type == LYRIC_ITXT)
//    {
//        ret = ldec_itxt_decode(curSuItem, curDataPtr, &usedBytes);
//    }
//    else if(ldec_itxt->subtitle_type == LYRIC_ISSA)
//    {
//        ret = ldec_issa_decode(curSuItem, curDataPtr, &usedBytes,lbsChgFlg);
//    }
//    else if(ldec_itxt->subtitle_type == LYRIC_ISUB)
//    {
//        ret = ldec_isub_decode(ldec_itxt, curSuItem,curDataPtr, &usedBytes);
//    }
//    else
//    {
//        usedBytes = ldec_itxt->ldecLbsBuf.ldecBufCrl[readIdx].dataLen;
//        ret = LDEC_ERR_LFMT_ERR;
//    }
    //�޸�lbsbuf����ر���,����
    thiz->LbsBufLock.lock(&thiz->LbsBufLock);
    ldec_update_read_pointer(&thiz->ldecLbsBuf, usedBytes);
    thiz->LbsBufLock.unlock(&thiz->LbsBufLock);
    if(ret == LDEC_ERR_NONE || ret == LDEC_LBS_EMPTY)
    {
        //�޸�framequeue����ر���,����
        thiz->FrmMgrLock.lock(&thiz->FrmMgrLock);
        thiz->ldecManager.validSize++;
        thiz->ldecManager.subIsFull[reconIdx] = 1;
        writeIdx = thiz->ldecManager.subWriteIdx;
        thiz->ldecManager.subIdxBuf[writeIdx] = reconIdx;
        if(thiz->ldecManager.subWriteIdx + 1 == LDEC_MAX_TXT_FRM_NUM)
        {
            thiz->ldecManager.subWriteIdx = 0;
        }
        else
        {
            thiz->ldecManager.subWriteIdx ++;
        }
        thiz->FrmMgrLock.unlock(&thiz->FrmMgrLock);
    }
    //ldec_itxt->decFlag = 0;

    return ret;
}

__s32 Impl_IsdbDec_CheckSupport(IsdbDec *thiz, __s32 nFileFmt)   //ret = EPDK_TRUE, EPDK_FALSE, nFileFmt = enum __CEDARLIB_LYRIC_FILE_FMT
{
    if(LYRIC_ISDB== nFileFmt)
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
    LDEC_ERR_IGNORE: ��Ļ���caption statement.�������.
    LDEC_ERR_DEC_SUBTITLE:��������.
    LDEC_ERR_NONE: �������,������һ�������ݵ���Ļ��.
    LDEC_LBS_EMPTY: �������.������һ������Ļ��.
Time: 2011/9/28
*******************************************************************************/
__s32 Impl_IsdbDec_SubtitleDecode(IsdbDec *thiz, CCedarSubtitleFrame *tmpItemInf, __u8 *curDataPtr, __u32 *usedBytes, __s8 lbsChgFlg, __u8 *privateData)   //��Ļ����
{
    if(*usedBytes != tmpItemInf->uSubSize)  //����*usedBytesΪ׼,�����ж�����Ϊ������uSubSize�ȸ���ֵ�ˣ����ɴ�����ĳ���
    {
        WARNING("fatal error! check code! [%d][%d]\n", *usedBytes, tmpItemInf->uSubSize);
    }
    return ldec_isdb_decode(thiz, tmpItemInf, curDataPtr, usedBytes, privateData);
}

void Impl_IsdbDec_Destruct(IsdbDec *thiz)
{
    IsdbDec *pDec = (IsdbDec*)thiz;
    deleteIsdbDec(pDec);
}

__s32 IsdbDec_Init(IsdbDec *thiz)
{
    __s32   tmpret;
    memset(thiz, 0, sizeof(IsdbDec));
    thiz->subtitle_type = LYRIC_ISDB;
    //thiz->nanciDataBufSize = LDEC_MAX_TXT_ANCI_SIZE;
    tmpret = CedarLock_Init(&thiz->LbsBufLock);
    if(tmpret != EPDK_OK)
    {
        WARNING("cedarlock init fail\n");
        goto _err1_mopen;
    }
    tmpret = CedarLock_Init(&thiz->FrmMgrLock);
    if(tmpret != EPDK_OK)
    {
        WARNING("cedarlock init fail\n");
        goto _err2_mopen;
    }
    
    thiz->IsdbDecIoCtrl = Impl_IsdbDec_IoCtrl;
    thiz->RequestWrite  = Impl_IsdbDec_RequestWrite;
    thiz->UpdatePointer = Impl_IsdbDec_UpdatePointer;
    thiz->GetLrcItem   = Impl_IsdbDec_GetIsdbItem;
    thiz->open          = Impl_IsdbDec_Open;
    thiz->close         = Impl_IsdbDec_Close;
    thiz->CommonDecode  = Base_IsdbDec_Decode;
    //return EPDK_OK;


    //���ǻ���ĳЩ���Ա
    strcpy((char*)thiz->strDecName, "Isdbdec");
    thiz->nLbsBufSize = ISDBDEC_MAX_LBSBUF_SIZE;
    thiz->nLdecFrameSize = ISDBDEC_MAX_FRM_SIZE;
    thiz->nanciDataBufSize = ISDBDEC_MAX_ANCI_SIZE;
    thiz->CheckSupport = Impl_IsdbDec_CheckSupport;
    thiz->SubtitleDecode = Impl_IsdbDec_SubtitleDecode;
    thiz->Destruct = Impl_IsdbDec_Destruct;
    
    //��ʼ���Լ��ĳ�Ա
    thiz->nOneSegFlag = 0;
    thiz->nCharEncodeType = LYRIC_SUB_ENCODE_ISO_8859_15;

    return EPDK_OK;
    
_err2_mopen:
    CedarLock_Exit(&thiz->LbsBufLock);
_err1_mopen:
//_err_mopen:
    return EPDK_FAIL;
}
__s32 IsdbDec_Exit(IsdbDec *thiz)
{
    CedarLock_Exit(&thiz->LbsBufLock);
    CedarLock_Exit(&thiz->FrmMgrLock);
    return EPDK_OK;
}

IsdbDec *newIsdbDec()
{
    __s32   tmpret;
    IsdbDec *pIsdbDec = (IsdbDec*)malloc(sizeof(IsdbDec));
    if(NULL == pIsdbDec)
    {
        WARNING("malloc IsdbDec fail\n");
        return NULL;
    }
    tmpret = IsdbDec_Init(pIsdbDec);
    if(tmpret != EPDK_OK)
    {
        WARNING("IsdbDec init fail\n");
        goto _err0;
    }
    return pIsdbDec;
_err0:
    free(pIsdbDec);
    return NULL;
}
void deleteIsdbDec(IsdbDec *thiz)
{
    IsdbDec_Exit(thiz);
    free(thiz);
}

