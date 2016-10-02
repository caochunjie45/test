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
#include "ldec_itxt_s.h"
#include "itxt_decode.h"

/*******************************************************************************
Function name: ldec_itxt_init_info
Description: 
    1.LBSBUF, FrameQueue, 还有一些buf和管理buf的数据结构的初始化
Parameters: 
    
Return: 
    
Time: 2011/9/26
*******************************************************************************/
void ldec_itxt_init_info(ItxtDec *ldec_itxt)
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
 //   ldec_itxt->ldecLbsBuf.ldecIdxSize = 0;//by add dvbsub

    ldec_itxt->ldecManager.validSize = 0;
    ldec_itxt->ldecManager.reconBufIdx = 0;
    ldec_itxt->ldecManager.subReadIdx = 0;
    ldec_itxt->ldecManager.subWriteIdx = 0;

    //ldec_itxt->decFlag = 0;
    ldec_itxt->useAnciBuf = 0;

    for(i = 0; i<LDEC_MAX_TXT_FRM_NUM; i++)
    {
        ldec_itxt->ldecManager.subIsFull[i] = 0;
    }
    for(i=0; i<4; i++)
    {
        ldec_itxt->lastColorCode[i] = 0;
        ldec_itxt->lastContrast[i] = 0;
    }
}

void ldec_fullCopy_subItem(__cedar_subtitle_item_t *pdes, __cedar_subtitle_item_t *psrc)
{
    __s16   *pTmpPtsOffset = pdes->pPtsOffset;
    __u8    *pTmpSubtitle = pdes->pSubtitle;
    memcpy(pdes, psrc, sizeof(__cedar_subtitle_item_t));
    pdes->pPtsOffset = pTmpPtsOffset;
    pdes->pSubtitle = pTmpSubtitle;
    if(psrc->pPtsOffset !=NULL && pdes->pPtsOffset!=NULL)
    {
        memcpy(pdes->pPtsOffset, psrc->pPtsOffset, psrc->uPtsSize*sizeof(__s16));
    }
    if(psrc->pSubtitle!=NULL && pdes->pSubtitle!=NULL)
    {
        if(psrc->uSubSize < 0 || psrc->uSubSize > LDEC_MAX_TXT_FRM_SIZE)
        {
            WARNING("fatal error! psrc->uSubSize[%x]\n", psrc->uSubSize);
        }
        memcpy(pdes->pSubtitle, psrc->pSubtitle, psrc->uSubSize);
    }
}

__s32 Impl_ItxtDec_IoCtrl(ItxtDec *thiz, __s32 cmd, __s32 aux, void *pbuffer)  //cmd = ItxtDecCmd
{
    __s32   ret = EPDK_FAIL;
    
    switch(cmd)
    {
        case ITXTDEC_CMD_SET_FRMRATE:
        {
            thiz->frm_rate = aux;
            ret = EPDK_OK;
            break;
        }
        case ITXTDEC_CMD_SET_FRMSIZE:
        {
            thiz->width = (aux>>16);
            thiz->height = (aux&0xffff);
            ret = EPDK_OK;
            break;
        }
        case ITXTDEC_CMD_SET_SCREEN_SIZE:
        {
            thiz->screenWidth  = aux>>16;
            thiz->screenHeight = aux & 0xffff;
            ret = EPDK_OK;
            break;
        }
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
    1. bufsize不够,返回fail; buf管理的块数量不够也返回fail
    2. requestwrite争取不加锁,提高效率
Parameters: 
    
Return: 
    EPDK_FAIL;
    EPDK_OK
Time: 2011/9/26
*******************************************************************************/
__s32 Impl_ItxtDec_RequestWrite(ItxtDec *thiz, __u32 uSizeRequested, __u8 **ppData0, __u32 *puSizeGot0, __u8 **ppData1, __u32 *puSizeGot1)
{
    __s32   ret;
    __u32 remainBufSize;
    __u32   nSize0 = 0, nSize1 = 0;
    
    __u16   ldecIdxSize; //lbs buffer的管理块的已经使用的个数
    __u32   validSize; //LBS buffer的有效字节数, 在这个函数执行期间,validsize只可能变大,所以无所谓
    __u8    *writeAddr;    //当前的写地址, 在此期间, writeAddr一直不会变

    //必须加锁,因为变量不只一个.只能通过加锁来和decode()互斥.
    thiz->LbsBufLock.lock(&thiz->LbsBufLock);
    ldecIdxSize = thiz->ldecLbsBuf.ldecIdxSize; //lbs buffer的管理块的已经使用的个数
    validSize = thiz->ldecLbsBuf.validSize; //LBS buffer的有效字节数, 在这个函数执行期间,validsize只可能变大,所以无所谓
    writeAddr = thiz->ldecLbsBuf.writeAddr;    //当前的写地址, 在此期间, writeAddr一直不会变
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
    //开始填写送入的参数
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
    1.更新LbsBuf
    2. 现在mkv，mpg psr都没有置lastPartctrl, 只有tspsr置了.
        而mkv,mpg负责的subtitle type是 itxt, issa, isub,所以这3种格式在本函数自动加上
        firstctrlbit和lastctrlbit.
        希望以后都能统一加上firstctrlbit和lastctrlbit.
        isub解码器其实是不关心firstctrlbit等的

        如果以后mkv要加pgs字幕支持,就必须修改mkv_psr，加上firstctrlbit和lastctrlbit

    3. 需要加锁

    4. chunk信息在这里赋给lbsbuf manager.
Parameters: 
    uDataSize:已写入buffer的数据长度
Return: 
    
Time: 2011/9/26
*******************************************************************************/
__s32 Impl_ItxtDec_UpdatePointer(ItxtDec *thiz, __lbs_datctlblk_t * pdatctl,__u32 uDataSize)
{
    __u16 writeIdx; //在此期间不会变
    __s32 remainBufSize;
    //首先把LDEC_FIRST_PART_CTRL等置起来, 如果来源是mkv或者vob, 自动加上,目前是这样,因为mkv, mpg psr没加first ctrl bit
//    if(CEDARLIB_FILE_FMT_MKV == thiz->subtitle_bs_src || CEDARLIB_FILE_FMT_VOB == thiz->subtitle_bs_src)
//    {
//        pdatctl->CtrlBits |= LDEC_FIRST_PART_CTRL;
//        pdatctl->CtrlBits |= LDEC_LAST_PART_CTRL;
//    }
    //就在这里加锁
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

    if(pdatctl->CtrlBits & LDEC_DATATYPE_VALID_CTRL)
    {
        thiz->ldecLbsBuf.ldecBufCrl[writeIdx].dataType = pdatctl->uDatatype;
    }
    //private data,ISDB规格的ts流,isdb字幕的一些附件数据会放在pes包的PES_private_data中,但是issa, itext, isub应该暂时都不会用到
    //memcpy(thiz->ldecLbsBuf.ldecBufCrl[writeIdx].privateData, pdatctl->privateData, LDEC_BUF_PRIVATE_DATA_LEN);
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
Function name: Impl_ItxtDec_GetLrcItem
Description: 
    1.原则是: 如果有多条字幕的时间范围符合要求,取最后一条送出,其余的忽略掉.
    2. 默认的帧队列的规律是:
        按时间顺序递增.
    3.有隐患.
        pLyricDecoder还有可能被destroy.也没做互斥.
    4.取完一条字幕后,该字幕没有立刻丢弃,因为还在要求的pts的范围内. 即使pOutputItem做深拷贝,也不丢弃.
    5.对互斥的原则是:读数据不互斥,改写数据时互斥.如果读的数据的值改变,不影响程序正确的前提下.
        这里,因为没有什么等待操作,全部互斥,不会影响性能.
Parameters: 
    pOutputItem:调用者输入的item,深拷贝.
Return: 
    EPDK_OK;
Time: 2011/9/26
*******************************************************************************/
__s32 Impl_ItxtDec_GetLrcItem(ItxtDec *thiz, __u32 nPts, __cedar_subtitle_item_t *pOutputItem, __s32 nOutputSubtitleSize)
{
    __u8    idx = 0;    //一般针对subItem[]
    __s32   timeSmall = 0;  //一条字幕项的起始时间
    __s32   timeLarge = 0;  //一条字幕项的结束时间
    __s32   ret = EPDK_FAIL;
    __s32   nFindFlag = 0;  //标识是否找到合适的字幕了,如果多条字幕符合要求，取最后一条字幕。
    __s32   nNewsubReadIdxFlag = 0; //表示已经确定了新的开始的字幕项的idx
    __s32   validSize;
    __s32   subReadIdx;     //针对__u8   subIdxBuf[].
    __s32   nNewsubReadIdx = 0;   //通过while()循环查找后,新的有效的subReadIdx.在它之前的idx全部删除.均针对__u8   subIdxBuf[].
    __s32   nSuitsubReadIdx = 0;   //找到的合适的字幕项的idx, 针对__u8   subIdxBuf[].
    //osal_switch_disable();
    thiz->FrmMgrLock.lock(&thiz->FrmMgrLock);
    validSize = thiz->ldecManager.validSize;
    subReadIdx = thiz->ldecManager.subReadIdx;
    if(validSize > 0)
    {
        idx = thiz->ldecManager.subIdxBuf[subReadIdx];
        timeSmall = thiz->ldecManager.subItem[idx].uPts;
        if(nPts < timeSmall)
        {
            //WARNING("No SuitItem! subReadIdx[%d], idx[%d], validSize[%d], pts[%d] > desPts[%d]\n", subReadIdx, idx, thiz->ldecManager.validSize, timeSmall, nPts);
            //时间未到,不用再查了,也不删除
            ret = EPDK_FAIL;
            goto _exit0;
        }
    }
    else
    {
        //WARNING("subitem validSize == 0!\n");
        ret = EPDK_FAIL;
        goto _exit0;
    }
    //通过while()循环,确定哪些字幕条目要丢弃,确定是否有合适的字幕项, 找最后一条适合的字幕项.
    while(validSize > 0)
    {
        idx = thiz->ldecManager.subIdxBuf[subReadIdx];
        timeSmall = thiz->ldecManager.subItem[idx].uPts;
        timeLarge = thiz->ldecManager.subItem[idx].uPts + thiz->ldecManager.subItem[idx].uDuration;
        if(nPts < timeSmall)
        {
            if(0 == nNewsubReadIdxFlag)
            {
                nNewsubReadIdx = subReadIdx;
                nNewsubReadIdxFlag = 1;
            }
        }
        else if(nPts >= timeSmall && nPts < timeLarge)
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
            //WARNING("find subitem[%d][%d][%d], [%d][%d], [%d]\n", subReadIdx, idx, thiz->ldecManager.validSize, timeSmall, timeLarge, nPts);
        }
        else
        {
            
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
        ldec_fullCopy_subItem(pOutputItem,&thiz->ldecManager.subItem[idx]);
        ret = EPDK_OK;
        if(nSuitsubReadIdx != nNewsubReadIdx)
        {
            WARNING("fatal error! SuitReadIdx[%d], newReadIdx[%d]\n", nSuitsubReadIdx, nNewsubReadIdx);
        }
    }
    else
    {
        ret = EPDK_FAIL;
    }

    //更新frame queue
    if(0 == nNewsubReadIdxFlag)    //没有确定,意味着全部删除
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
        if(pOutputItem->pSubtitle != NULL)
            pOutputItem->pSubtitle[pOutputItem->uSubSize] = 0;
    }
    return ret;
}
/*******************************************************************************
Function name: Impl_ItxtDec_Open
Description: 
    1. 给lbsbuf和framequeue分配内存
Parameters: 
    
Return: 
    
Time: 2011/9/26
*******************************************************************************/
__s32 Impl_ItxtDec_Open(ItxtDec *thiz)
{
    __s32   i;
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
        thiz->ldecManager.subItem[i].pPtsOffset = NULL;
    }
    thiz->anciDataBuf = (__u8 *)PHY_MALLOC(sizeof(__u8)*thiz->nanciDataBufSize, 1024);
    if(thiz->anciDataBuf == NULL)
    {
        WARNING("malloc anciDataBuf[%d] fail\n", thiz->nanciDataBufSize);
        goto _err_mopen;
    }
    ldec_itxt_init_info(thiz);
    return LDEC_ERR_NONE;
    
_err_mopen:
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

__s32 Impl_ItxtDec_Close(ItxtDec *thiz)
{
    __s32   i;
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
    return LDEC_ERR_NONE;
}

/*******************************************************************************
Function name: Base_ItxtDec_Decode
Description: 
    1.注意thiz->nSyncStatFlag.可能在解码前先清buffer.\
    2. 注意该函数是基类实现的，针对itext, issa两种内置字幕类型,这两种解码类型
    对lbs chunk的要求是:chunk的边界和解码意义上的数据块有严格的一一对应关系.解码数据块
    可能是一个字幕帧,例如itext,issa;也有可能是一个数据项,如tspsr的pgs字幕，若干字幕项解完可以得到
    一个字幕.
    3.isub解码器,认为block和字幕帧没有严格的对应关系,它有自己的一套lbs管理机制,所以isub覆盖基类decode(),
    完全自己实现.
Parameters: 
    
Return: 
    
Time: 2011/9/27
*******************************************************************************/
__s32 Base_ItxtDec_Decode(ItxtDec *thiz)
{
    __s32 ret = 0;
    __u8  reconIdx = 0; //指示subItem[]中的一个元素,找一个空元素.
    __u8  writeIdx = 0;
    __u8  readIdx= 0;   //lbsbuf的数据块读到哪里了
    __u32 usedBytes = 0;    //解码前是解码前的lbs的长度,解码后是用掉的lbs的长度
    __u8  *curDataPtr = NULL;
    __s8  lbsChgFlg;
    __cedar_subtitle_item_t *curSuItem;

    if(thiz->nSyncStatFlag) //socket_task()已经可以保证,在sync期间绝不调用requestwrite(), updatepointer()等.
    {
        thiz->LbsBufLock.lock(&thiz->LbsBufLock);
        thiz->FrmMgrLock.lock(&thiz->FrmMgrLock);
        ldec_itxt_init_info(thiz);
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

    if(0 == thiz->ldecLbsBuf.ldecBufCrl[readIdx].hasPtsFlag)
    {
        WARNING("fatal error! lbsbufctrl hasPtsFlag = 0, ptsVal\n");
    }
    curSuItem->uPts      = thiz->ldecLbsBuf.ldecBufCrl[readIdx].ptsVal;
    curSuItem->uDuration = thiz->ldecLbsBuf.ldecBufCrl[readIdx].duration;
    curSuItem->uDataType = thiz->ldecLbsBuf.ldecBufCrl[readIdx].dataType;
    curSuItem->uSubSize  = thiz->ldecLbsBuf.ldecBufCrl[readIdx].dataLen;    //uSubSize最终应该表示解码后的字幕数据的长度,现在是解码前的字幕数据的长度
    curSuItem->uPtsSize  = 0;
    lbsChgFlg = thiz->ldecLbsBuf.ldecBufCrl[readIdx].lbsChgFlg;
    //WARNING("ldec_itxt BaseDecode: rd_idx[%d], pts[%d], duration[%d], datalen[%d]\n", readIdx, curSuItem->uPts, curSuItem->uDuration, curSuItem->uSubSize);
    //curDataPtr = ldec_judge_subdata_cover_bufEnd((void *) 0);
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
    ret = thiz->SubtitleDecode(thiz, curSuItem, curDataPtr, &usedBytes, lbsChgFlg);
    
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
    //修改lbsbuf的相关变量,加锁
    thiz->LbsBufLock.lock(&thiz->LbsBufLock);
    ldec_update_read_pointer(&thiz->ldecLbsBuf, usedBytes);
    thiz->LbsBufLock.unlock(&thiz->LbsBufLock);
    if(ret == LDEC_ERR_NONE)
    {
        //修改framequeue的相关变量,加锁
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

void Base_ItxtDec_Destruct(ItxtDec *thiz)
{
    WARNING("fatal error! ItxtDec Base don't destuct!\n");
}

__s32 ItxtDec_Init(ItxtDec *thiz, __s32 nFileFmt)
{
    __s32   tmpret;
    memset(thiz, 0, sizeof(ItxtDec));
    thiz->subtitle_type = nFileFmt;
    thiz->nanciDataBufSize = LDEC_MAX_TXT_ANCI_SIZE;
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
    thiz->ItxtDecIoCtrl = Impl_ItxtDec_IoCtrl;
    thiz->RequestWrite  = Impl_ItxtDec_RequestWrite;
    thiz->UpdatePointer = Impl_ItxtDec_UpdatePointer;
    thiz->GetLrcItem    = Impl_ItxtDec_GetLrcItem;
    thiz->open          = Impl_ItxtDec_Open;
    thiz->close         = Impl_ItxtDec_Close;
    thiz->CommonDecode  = Base_ItxtDec_Decode;
    thiz->Destruct      = Base_ItxtDec_Destruct;
    return EPDK_OK;
    
_err2_mopen:
    CedarLock_Exit(&thiz->LbsBufLock);
_err1_mopen:
//_err_mopen:
    return EPDK_FAIL;
}
__s32 ItxtDec_Exit(ItxtDec *thiz)
{
    CedarLock_Exit(&thiz->LbsBufLock);
    CedarLock_Exit(&thiz->FrmMgrLock);
    return EPDK_OK;
}

ItxtDec* ItxtDec_Create(__s32 nFileFmt)   //enum __CEDARLIB_LYRIC_FILE_FMT
{
    ItxtDec *pItxtDec = NULL;
    switch(nFileFmt)
    {
        case LYRIC_ITXT:
        {
            pItxtDec = (ItxtDec*)newItextDec();
            break;
        }
        case LYRIC_ISSA:
        {
            pItxtDec = (ItxtDec*)newIssaDec();
            break;
        }
        case LYRIC_ISUB:
        {
            pItxtDec = (ItxtDec*)newIsubDec();
            break;
        }
        default:
        {
            WARNING("fatal error! embeded subtitle type[%x] is not support!\n", nFileFmt);
            pItxtDec = NULL;
            break;
        }
    }
    return pItxtDec;
}

void ItxtDec_Destroy(ItxtDec *thiz)
{
    thiz->Destruct(thiz);
}

