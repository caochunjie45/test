
#include "subcore_s.h"

/*******************************************************************************
Function name: idxsub_decode_func
Description: 
    
Parameters: 
    picBufptr:��Ž�������frame����ʼ��ַ��
    bufsize: ��Ž�������frame�ĳ��ȡ�
    used_buf_len:��frame������ʹ�õĳ��ȡ�

    usedBytes:����ʱû���κκ���.��ȥʱ��ʾ�õ���lbs���ֽ���
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
    1.Isub�����ж������Ƿ��߽�
    2.�������lbsbuf�����ݽṹ�Ѽ�������
    3.ִ�е�����,lbsbuf��һ��������,�������汣֤
Parameters: 
    
Return: 
    
Time: 2011/10/9
*******************************************************************************/
 __u8 *ldec_judge_subdata_cover_bufEnd(ItxtDec *ldec_itxt)
{ 
  __u8  curReadIdx  = 0;
  __u8  nextReadIdx = 0;    //nextReadIdx���ָ�������һ����Ļ֡����ʼ��chunk.
  __u8  rcdReadIdx  = 1;    //�Ѱ�����chunk ctrl������
  __u32 remainData = 0; //��ͷǰ����Ļ֡�ĳ���
  __u32 remainAnciBufSize;  //װ���ͷǰ����Ļ֡����֮��,ancibuf��ʣ�µ��ֽ���
  __u32 remainLbsData;  //��ͷ�����Ļ֡���ֵĳ���
  
  curReadIdx = ldec_itxt->ldecLbsBuf.ldecReadIdx;
  nextReadIdx = curReadIdx;
  
  while(rcdReadIdx < ldec_itxt->ldecLbsBuf.ldecIdxSize)
  {
    nextReadIdx = nextReadIdx + 1;
    if(nextReadIdx == LDEC_MAX_TXT_DATA_NUM)
        nextReadIdx = 0;
    if(nextReadIdx == curReadIdx)
        break;
    else if(ldec_itxt->ldecLbsBuf.ldecBufCrl[nextReadIdx].hasPtsFlag == 1)  //����Isub��˵,����ζ��������chunk��һ���µ���Ļ֡�Ŀ�ʼ
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
      remainLbsData     = ldec_itxt->ldecLbsBuf.validSize-remainData;   //ȫ���ȥ
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
    1.ע��thiz->nSyncStatFlag.�����ڽ���ǰ����buffer.\
    2. ע��ú����ǻ���ʵ�ֵģ����itext, issa����������Ļ����,�����ֽ�������
    ��lbs chunk��Ҫ����:chunk�ı߽�ͽ��������ϵ����ݿ����ϸ��һһ��Ӧ��ϵ.�������ݿ�
    ������һ����Ļ֡,����itext,issa;Ҳ�п�����һ��������,��tspsr��pgs��Ļ��������Ļ�������Եõ�
    һ����Ļ.
    3.isub������,��Ϊblock����Ļ֡û���ϸ�Ķ�Ӧ��ϵ,�����Լ���һ��lbs�������,����isub���ǻ���decode(),
    ��ȫ�Լ�ʵ��.

    4.����������Isub��ȫ�Լ�ʵ�ֵ�CommonDecode().

    5.Isub���������������Ϊchunk����Ļ֡û���ϸ��Ӧ��ϵ,������Ϊvob�ļ���������ɵ�.mkv�����ϸ��Ӧ������Ļ֡��.
Parameters: 
    
Return: 
    
Time: 2011/9/27
*******************************************************************************/
__s32 IsubDec_Decode(ItxtDec *thiz)
{
    __s32 ret = 0;
    __u8  reconIdx = 0; //ָʾsubItem[]�е�һ��Ԫ��,��һ����Ԫ��.
    __u8  writeIdx = 0;
    __u8  readIdx= 0;   //lbsbuf�����ݿ����������
    __u32 usedBytes = 0;
    __u8  *curDataPtr = NULL;
    //__s8  lbsChgFlg;
    __cedar_subtitle_item_t *curSuItem;

    if(thiz->nSyncStatFlag) //socket_task()�Ѿ����Ա�֤,��sync�ڼ��������requestwrite(), updatepointer()��.
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
    curSuItem->uSubSize  = thiz->ldecLbsBuf.ldecBufCrl[readIdx].dataLen;    //uSubSize����Ӧ�ñ�ʾ��������Ļ���ݵĳ���,�����ǽ���ǰ����Ļ���ݵĳ���
    curSuItem->uPtsSize  = 0;
    //lbsChgFlg = thiz->ldecLbsBuf.ldecBufCrl[readIdx].lbsChgFlg;

    //����֮����Ҫ����,����ΪIsub���жϻ����chunk
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
    //�ȳ�ʼ������
    tmpret = ItxtDec_Init(&thiz->base, LYRIC_ISUB);
    if(tmpret != EPDK_OK)
    {
        WARNING("init ItxtDec fail\n");
        goto _err0;
    }
    //���ǻ���ĳЩ���Ա
    strcpy((char*)thiz->base.strDecName, "Isubdec");
    thiz->base.nLbsBufSize = LDEC_MAX_TXT_BUF_SIZE;
    thiz->base.nLdecFrameSize = LDEC_MAX_TXT_FRM_SIZE;
    thiz->base.nanciDataBufSize = LDEC_MAX_TXT_ANCI_SIZE;
    thiz->base.CheckSupport = IsubDec_CheckSupport;
    thiz->base.CommonDecode = IsubDec_Decode;
    thiz->base.SubtitleDecode = NULL;   //Isub����Ҫʵ��SubtitleDecode��.��Ϊ���Ѿ���CommonDecode()��ȫ����
    thiz->base.Destruct = IsubDec_ItxtDec_Destruct;
    //��ʼ���Լ��ĳ�Ա

    return EPDK_OK;

_err0:
    return EPDK_FAIL;
}
__s32 IsubDec_Exit(IsubDec *thiz)
{
    //��Ϊ�Ḳ�ǻ���Ԫ�ص�Ե��. һ�����ͷ��Լ���,�ٴ�������.
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

