
#include "subcore_s.h"

/*******************************************************************************
Function name: idxsub_decode_func
Description: 
    
Parameters: 
    picBufptr:存放解码结果的frame的起始地址。
    bufsize: 存放解码结果的frame的长度。
    used_buf_len:在frame中真正使用的长度。

    usedBytes:进来时没有任何含义.出去时表示用掉的lbs的字节数
Return: 
    
Time: 2009/7/4
*******************************************************************************/ 

//__s32 ldec_isub_decode(__ldec_itxt_t *pdec, __cedar_subtitle_item_t *curSuItem,__u8 *curDataPtr, __u32 *usedBytes)
__s32 ldec_isub_decode(ItxtDec *pdec, __cedar_subtitle_item_t *curSuItem,__u8 *curDataPtr, __u32 *usedBytes)
{
   __ldec_sp_dcqt_t      spSqt;
   __u8                 *dataBuf;
   __s32                 ret;
   __u32                 maxDataSize1 = 0;
   __u32                 maxDataSize2 = 0;
   __u32                 compDataSize = 0;
   
   *usedBytes = 0;
    spSqt.spuSize = (curDataPtr[0] << 8) | curDataPtr[1];
    spSqt.spStartaddr = (curDataPtr[2] << 8) | curDataPtr[3];

    if(pdec->useAnciBuf == 1)
    {
        if(spSqt.spuSize > LDEC_MAX_TXT_ANCI_SIZE)
        {    
            *usedBytes =  spSqt.spuSize;
            return LDEC_ERR_DEC_SUBTITLE;
        }
    }
    else
    {   
        maxDataSize1 = pdec->ldecLbsBuf.validSize;
        maxDataSize2 = pdec->ldecLbsBuf.ldecBufEnd - pdec->ldecLbsBuf.readAddr + 1;
        compDataSize  = (maxDataSize1 <= maxDataSize2)? maxDataSize1 : maxDataSize2;
        if(compDataSize < spSqt.spuSize)
            return LDEC_ERR_LBS_UNDERFLOW;
    }
    
    *usedBytes =  spSqt.spuSize;
  
    ret = process_sp_dcsqt(pdec, curSuItem, curDataPtr, &spSqt);         // control information
	if(ret < 0)
		return LDEC_ERR_DEC_SUBTITLE;
    
    dataBuf = (__u8 *)CEDAR_MALLOC(sizeof(__u8) * LDEC_MAX_TXT_DATA_SIZE);
	memset(dataBuf, 0, LDEC_MAX_TXT_DATA_SIZE);
    
    ret = run_length_decode(curSuItem, curDataPtr, &spSqt, dataBuf, TOP_FIELD_FLAG);        // top filed
	
	if(ret < 0)
	{   
		CEDAR_FREE(dataBuf);
		return LDEC_ERR_DEC_SUBTITLE;
	}
    ret = run_length_decode(curSuItem, curDataPtr, &spSqt, dataBuf, BOTTOM_FIELD_FLAG);     // bottom filed  
	if(ret < 0)
	{   
		CEDAR_FREE(dataBuf);
		return LDEC_ERR_DEC_SUBTITLE;
	}

    ret = convert_data_to_bmp(curSuItem, dataBuf);
	CEDAR_FREE(dataBuf);
    if(ret < 0)
    {
       return LDEC_ERR_DEC_SUBTITLE;
    }

	return LDEC_ERR_NONE;
	
}

/*******************************************************************************
Function name: ldec_judge_subdata_cover_bufEnd
Description: 
    1.Isub用于判断数据是否跨边界
    2.在外面对lbsbuf的数据结构已加锁保护
    3.执行到这里,lbsbuf里一定有数据,这在外面保证
Parameters: 
    
Return: 
    
Time: 2011/10/9
*******************************************************************************/
 __u8 *ldec_judge_subdata_cover_bufEnd(ItxtDec *ldec_itxt)
{ 
  __u8  curReadIdx  = 0;
  __u8  nextReadIdx = 0;    //nextReadIdx最后指向的是下一个字幕帧的起始的chunk.
  __u8  rcdReadIdx  = 1;    //已包括的chunk ctrl的数量
  __u32 remainData = 0; //回头前的字幕帧的长度
  __u32 remainAnciBufSize;  //装完回头前的字幕帧部分之后,ancibuf还剩下的字节数
  __u32 remainLbsData;  //回头后的字幕帧部分的长度
  
  curReadIdx = ldec_itxt->ldecLbsBuf.ldecReadIdx;
  nextReadIdx = curReadIdx;
  
  while(rcdReadIdx < ldec_itxt->ldecLbsBuf.ldecIdxSize)
  {
    nextReadIdx = nextReadIdx + 1;
    if(nextReadIdx == LDEC_MAX_TXT_DATA_NUM)
        nextReadIdx = 0;
    if(nextReadIdx == curReadIdx)
        break;
    else if(ldec_itxt->ldecLbsBuf.ldecBufCrl[nextReadIdx].hasPtsFlag == 1)  //对于Isub来说,这意味着遇到的chunk是一个新的字幕帧的开始
        break;
    rcdReadIdx ++;
  }

  //if(rcdReadIdx == ldec_itxt->ldecLbsBuf.ldecIdxSize)
  if(nextReadIdx == curReadIdx)
  {
        if(ldec_itxt->ldecLbsBuf.validSize <= ldec_itxt->ldecLbsBuf.ldecBufEnd - ldec_itxt->ldecLbsBuf.readAddr + 1)
        {
            ldec_itxt->useAnciBuf = 0;
            return ldec_itxt->ldecLbsBuf.readAddr;
        }
        else
        {
            WARNING("must use anciBuf!\n");
            WARNING("Be Careful! [%d][%d][%d], [0x%x][0x%x][%d], [%d]\n", curReadIdx, nextReadIdx, ldec_itxt->ldecLbsBuf.ldecIdxSize,
            ldec_itxt->ldecLbsBuf.readAddr, ldec_itxt->ldecLbsBuf.ldecBuf, ldec_itxt->ldecLbsBuf.validSize, LDEC_MAX_TXT_DATA_NUM);
            goto _use_anci_buf;
        }        
  }
  else if(ldec_itxt->ldecLbsBuf.ldecBufCrl[curReadIdx].startAddr == ldec_itxt->ldecLbsBuf.ldecBufCrl[nextReadIdx].startAddr)
  {
    ldec_itxt->useAnciBuf = 0;
    WARNING("the sub data is error.\n");
    return NULL;
  }
 else if(ldec_itxt->ldecLbsBuf.ldecBufCrl[curReadIdx].startAddr < ldec_itxt->ldecLbsBuf.ldecBufCrl[nextReadIdx].startAddr)
  {
      ldec_itxt->useAnciBuf = 0;
      return ldec_itxt->ldecLbsBuf.readAddr;
  }
  else
  {   
_use_anci_buf:
      WARNING("use the anci data buf.\n");
      
      ldec_itxt->useAnciBuf = 1;
      remainData = ldec_itxt->ldecLbsBuf.ldecBufEnd - ldec_itxt->ldecLbsBuf.readAddr + 1;
      memcpy(ldec_itxt->anciDataBuf, ldec_itxt->ldecLbsBuf.readAddr, remainData);
      remainAnciBufSize = LDEC_MAX_TXT_ANCI_SIZE - remainData;
      remainLbsData     = ldec_itxt->ldecLbsBuf.validSize-remainData;   //全算进去
      //remainLbsData     = (__u32)(ldec_itxt->ldecLbsBuf.ldecBufCrl[nextReadIdx].startAddr-ldec_itxt->ldecLbsBuf.ldecBuf);
      if(remainAnciBufSize > remainLbsData)
          remainAnciBufSize = remainLbsData;

      memcpy(ldec_itxt->anciDataBuf+remainData,ldec_itxt->ldecLbsBuf.ldecBuf, remainAnciBufSize);
      return ldec_itxt->anciDataBuf;
  }
  
}

__s32 IsubDec_CheckSupport(ItxtDec *thiz, __s32 nFileFmt)   //ret = EPDK_TRUE, EPDK_FALSE, nFileFmt = enum __CEDARLIB_LYRIC_FILE_FMT
{
    if(LYRIC_ISUB == nFileFmt)
    {
        return EPDK_TRUE;
    }
    else
    {
        return EPDK_FALSE;
    }
}

/*******************************************************************************
Function name: Isub_ItxtDec_Decode
Description: 
    1.注意thiz->nSyncStatFlag.可能在解码前先清buffer.\
    2. 注意该函数是基类实现的，针对itext, issa两种内置字幕类型,这两种解码类型
    对lbs chunk的要求是:chunk的边界和解码意义上的数据块有严格的一一对应关系.解码数据块
    可能是一个字幕帧,例如itext,issa;也有可能是一个数据项,如tspsr的pgs字幕，若干字幕项解完可以得到
    一个字幕.
    3.isub解码器,认为block和字幕帧没有严格的对应关系,它有自己的一套lbs管理机制,所以isub覆盖基类decode(),
    完全自己实现.

    4.本函数就是Isub完全自己实现的CommonDecode().

    5.Isub的问题就在于它认为chunk和字幕帧没有严格对应关系,这是因为vob文件的特性造成的.mkv还是严格对应的送字幕帧的.
Parameters: 
    
Return: 
    
Time: 2011/9/27
*******************************************************************************/
__s32 IsubDec_Decode(ItxtDec *thiz)
{
    __s32 ret = 0;
    __u8  reconIdx = 0; //指示subItem[]中的一个元素,找一个空元素.
    __u8  writeIdx = 0;
    __u8  readIdx= 0;   //lbsbuf的数据块读到哪里了
    __u32 usedBytes = 0;
    __u8  *curDataPtr = NULL;
    //__s8  lbsChgFlg;
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
    if(0 == thiz->ldecLbsBuf.ldecIdxSize)
    {
        WARNING("fatal error! validSize[%d], IdxSize[%d]\n", thiz->ldecLbsBuf.validSize, thiz->ldecLbsBuf.ldecIdxSize);
    }

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
        WARNING("be careful! lbsbufctrl hasPtsFlag = 0, ptsVal\n");
    }
    curSuItem->uPts      = thiz->ldecLbsBuf.ldecBufCrl[readIdx].ptsVal;
    curSuItem->uDuration = thiz->ldecLbsBuf.ldecBufCrl[readIdx].duration;
    curSuItem->uDataType = thiz->ldecLbsBuf.ldecBufCrl[readIdx].dataType;
    curSuItem->uSubSize  = thiz->ldecLbsBuf.ldecBufCrl[readIdx].dataLen;    //uSubSize最终应该表示解码后的字幕数据的长度,现在是解码前的字幕数据的长度
    curSuItem->uPtsSize  = 0;
    //lbsChgFlg = thiz->ldecLbsBuf.ldecBufCrl[readIdx].lbsChgFlg;

    //这里之所以要加锁,是因为Isub的判断会跨多个chunk
    thiz->LbsBufLock.lock(&thiz->LbsBufLock);
    curDataPtr = ldec_judge_subdata_cover_bufEnd(thiz);
    thiz->LbsBufLock.unlock(&thiz->LbsBufLock);
    
    if(curDataPtr == NULL)
    {  
        thiz->LbsBufLock.lock(&thiz->LbsBufLock);
       ldec_update_read_pointer(&thiz->ldecLbsBuf, thiz->ldecLbsBuf.validSize);
       thiz->LbsBufLock.unlock(&thiz->LbsBufLock);
       return  LDEC_ERR_LFMT_ERR;
    }
    
    //ldec_itxt->decFlag = 1;
    ret = ldec_isub_decode(thiz, curSuItem,curDataPtr, &usedBytes);

    thiz->LbsBufLock.lock(&thiz->LbsBufLock);
    ldec_update_read_pointer(&thiz->ldecLbsBuf, usedBytes);
    thiz->LbsBufLock.unlock(&thiz->LbsBufLock);
    if(ret == LDEC_ERR_NONE)
    {
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
void IsubDec_ItxtDec_Destruct(ItxtDec *thiz)
{
    IsubDec *pDec = (IsubDec*)thiz;
    deleteIsubDec(pDec);
}

__s32 IsubDec_Init(IsubDec *thiz)
{
    __s32   tmpret;
    //先初始化基类
    tmpret = ItxtDec_Init(&thiz->base, LYRIC_ISUB);
    if(tmpret != EPDK_OK)
    {
        WARNING("init ItxtDec fail\n");
        goto _err0;
    }
    //覆盖基类某些虚成员
    strcpy((char*)thiz->base.strDecName, "Isubdec");
    thiz->base.nLbsBufSize = LDEC_MAX_TXT_BUF_SIZE;
    thiz->base.nLdecFrameSize = LDEC_MAX_TXT_FRM_SIZE;
    thiz->base.nanciDataBufSize = LDEC_MAX_TXT_ANCI_SIZE;
    thiz->base.CheckSupport = IsubDec_CheckSupport;
    thiz->base.CommonDecode = IsubDec_Decode;
    thiz->base.SubtitleDecode = NULL;   //Isub不需要实现SubtitleDecode了.因为它已经把CommonDecode()完全覆盖
    thiz->base.Destruct = IsubDec_ItxtDec_Destruct;
    //初始化自己的成员

    return EPDK_OK;

_err0:
    return EPDK_FAIL;
}
__s32 IsubDec_Exit(IsubDec *thiz)
{
    //因为会覆盖基类元素的缘故. 一般先释放自己的,再处理基类的.
    ItxtDec_Exit(&thiz->base);
    return EPDK_OK;
}

IsubDec *newIsubDec()
{
    __s32   tmpret;
    IsubDec *pIsubDec = (IsubDec*)malloc(sizeof(IsubDec));
    if(NULL == pIsubDec)
    {
        WARNING("malloc IsubDec fail\n");
        return NULL;
    }
    tmpret = IsubDec_Init(pIsubDec);
    if(tmpret != EPDK_OK)
    {
        WARNING("IsubDec init fail\n");
        goto _err0;
    }
    return pIsubDec;
_err0:
    free(pIsubDec);
    return NULL;
}

void deleteIsubDec(IsubDec *thiz)
{
    IsubDec_Exit(thiz);
    free(thiz);
}

