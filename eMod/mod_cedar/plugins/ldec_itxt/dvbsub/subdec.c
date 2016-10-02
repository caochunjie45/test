
#if PLAY_DVBT_DVBSUBTITLE

#include "subdec.h"
#include "ldec_idvb_i.h"


static void ldec_dvbsub_init_info(__ldec_idvbsub_t *ldec_itxt)
{
    __u8    i;
    __ldec_info_t *ldecInfo = (__ldec_info_t *)ldec_itxt->privInfo;

    memset(ldec_itxt->ldecDisplay.dispSubIdx, 0,LDEC_MAX_LBS_SUB_NUM);
    memset(ldec_itxt->ldecDisplay.dispSubNs, 0,LDEC_MAX_LBS_SUB_NUM);

    ldec_itxt->ldecDisplay.readDispIdx = 0;
    ldec_itxt->ldecDisplay.writeDispIdx = 0;
	
    ldec_itxt->ldecLbsBuf.readAddr = ldec_itxt->ldecLbsBuf.ldecBuf;
    ldec_itxt->ldecLbsBuf.writeAddr = ldec_itxt->ldecLbsBuf.ldecBuf;
    ldec_itxt->ldecLbsBuf.ldecIdxSize = 0;
    ldec_itxt->ldecLbsBuf.ldecWriteIdx = 0;
    ldec_itxt->ldecLbsBuf.ldecReadIdx  = 0;
    ldec_itxt->ldecLbsBuf.validSize   = 0;
	
    ldec_itxt->ldecManager.validSize = 0;
    ldec_itxt->ldecManager.startBufIdx = 0;
    ldec_itxt->ldecManager.reconBufIdx = 0;
    ldec_itxt->ldecManager.subReadIdx = 0;
    ldec_itxt->ldecManager.subWriteIdx = 0;
	
    for(i = 0; i<LDEC_MAX_LBS_SUB_NUM; i++)
    {
        ldec_itxt->ldecManager.subItem[i].isFull = 0;
    }
}

__s32 Impl_DvbsubDec_IoCtrl(__ldec_idvbsub_t *thiz, __s32 cmd, __s32 aux, void *pbuffer)  //cmd = ItxtDecCmd
{
    __s32   ret = EPDK_FAIL;
    
    //WARNING("IoCtrl cmd(%d)\n",cmd);
    switch(cmd)
    {
        case DVBSUBDEC_CMD_SET_SUBTITLE_SOURCE:
        {
            thiz->subtitle_bs_src = aux;
            ret = EPDK_OK;
            break;
        }
        case DVBSUBDEC_CMD_NOTIFY_SYNCSTAT:
        {
            if(thiz->nSyncStatFlag)
            {
                WARNING("fatal error! thiz->nSyncStatFlag[%d]\n", thiz->nSyncStatFlag);
            }
            thiz->nSyncStatFlag = 1;
            ret = EPDK_OK;
            break;
        }
        case DVBSUBDEC_CMD_CHECK_SYNCSTAT:
        {
            ret = thiz->nSyncStatFlag;
            break;
        }
  
        case DVBSUBDEC_CMD_SET_CHAR_CODING_TYPE:
        {
            thiz->nCharEncodeType = (enum __CEDARLIB_LYRIC_ENCODE_TYPE)aux;
            ret = EPDK_OK;
            break;
        }

	case DVBSUBDEC_CMD_CHECK_DSP_TIME:
        {
            if(DvbsubDec_RequirePts(thiz, (__u32*)pbuffer) == 0)
                return EPDK_OK;
            else
                return EPDK_FAIL;
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
Time: 2013/1/24
*******************************************************************************/
__s32 Impl_DvbsubDec_RequestWrite(__ldec_idvbsub_t *thiz, __u32 uSizeRequested, __u8 **ppData0, __u32 *puSizeGot0, __u8 **ppData1, __u32 *puSizeGot1)
{
    __s32   ret;
    __u32 remainBufSize;
    __u32   nSize0 = 0, nSize1 = 0;
    
    __u16   ldecIdxSize; //lbs buffer的管理块的已经使用的个数
    __u32   validSize; //LBS buffer的有效字节数, 在这个函数执行期间,validsize只可能变大,所以无所谓
    __u8    *writeAddr;    //当前的写地址, 在此期间, writeAddr一直不会变

	//WARNING("RequestWrite(0x%x)\n",uSizeRequested);
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
	thiz->FrmMgrLock.lock(&thiz->FrmMgrLock);
	ldec_dvbsub_init_info(thiz);
	thiz->FrmMgrLock.unlock(&thiz->FrmMgrLock);
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
        *ppData1 = thiz->ldecLbsBuf.ldecBuf;	// 注:若空间不够,多余从头开始存
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
    
Time: 2013/1/24
*******************************************************************************/
__s32 Impl_DvbsubDec_UpdatePointer(__ldec_idvbsub_t *thiz, __lbs_datctlblk_t * pdatctl,__u32 uDataSize)
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
	//WARNING("UpdatePointer(0x%x, 0x%x)[%d, %d, %d, %d, %d, %d]\n",remainBufSize, uDataSize, pdatctl->CtrlBits, pdatctl->uPTS, pdatctl->uPTS2, pdatctl->uDuration, pdatctl->uDatatype, pdatctl->SubtitleType);
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
        //thiz->ldecLbsBuf.ldecBufCrl[writeIdx].coverBufEnd = 0; //一笔数据多次传送,有可能回头.
    }

    if(pdatctl->CtrlBits & LDEC_LAST_PART_CTRL)
    {
        //WARNING("update chunk![%d][%d][%d]\n", thiz->ldecLbsBuf.ldecWriteIdx, thiz->ldecLbsBuf.ldecBufCrl[writeIdx].dataLen, uDataSize);
        thiz->ldecLbsBuf.validSize += thiz->ldecLbsBuf.ldecBufCrl[writeIdx].dataLen;  //validSize必须和ldecWriteIdx一起生效,不允许断续增加, 否则解码会出错.
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
Time: 2013/1/24
*******************************************************************************/
__s32 Impl_DvbsubDec_GetDvbItem(__ldec_idvbsub_t *thiz, __u32 nPts, CCedarSubtitleFrame *pOutputItem, __s32 nOutputSubtitleSize)
{
	__s32 ret = EPDK_FAIL;
	__u32 startSubIdx = 0;
	__u32   timeSmall = 0;  //一条字幕项的起始时间
	__s32   timeLarge = 0;  //一条字幕项的结束时间
	__Ldec_manager_t  *subdecManager;
	__subtitle_item_t *psrcItem;

	thiz->FrmMgrLock.lock(&thiz->FrmMgrLock);
	subdecManager = &(thiz->ldecManager);

	pOutputItem->isFull = 0;
	pOutputItem->subNum = 0;
	
    if (subdecManager->validSize > 0)
    {
	startSubIdx = thiz->ldecDisplay.dispSubIdx[thiz->ldecDisplay.readDispIdx];
	if (subdecManager->subItem[startSubIdx].isFull)
	{
		__u32 i, count;

		psrcItem = &(thiz->ldecManager.subItem[startSubIdx]);
		count = thiz->ldecDisplay.dispSubNs[thiz->ldecDisplay.readDispIdx];

		pOutputItem->isFull = 1;
		subdecManager->validSize -= count;
		pOutputItem->uPts = psrcItem->ptsVal;
		pOutputItem->uDuration = psrcItem->timeDuration;
		pOutputItem->uDataType = CEDAR_SUB_ENCODE_BITMAP;

		for (i=0; i<count; i++)
		{
			psrcItem = &(thiz->ldecManager.subItem[startSubIdx]);
			timeSmall = subdecManager->subItem[startSubIdx].ptsVal;
			timeLarge = subdecManager->subItem[startSubIdx].ptsVal + subdecManager->subItem[startSubIdx].timeDuration;
			//if(nPts >= timeSmall && nPts <= timeLarge)
			{
				// 拷贝Subtitle数据
				ret = EPDK_OK;
				pOutputItem->subImageRect[pOutputItem->subNum].x = psrcItem->startX;
				pOutputItem->subImageRect[pOutputItem->subNum].y = psrcItem->startY;
				pOutputItem->subImageRect[pOutputItem->subNum].width = psrcItem->picWidth;//psrcItem->subWidth;
				pOutputItem->subImageRect[pOutputItem->subNum].height = psrcItem->picHeight;//psrcItem->subHeight;
				//-----------------------------------------------------------------------------------
				pOutputItem->subData[pOutputItem->subNum] = psrcItem->pSubtitle;// Subtitle data
				//-----------------------------------------------------------------------------------
				pOutputItem->subNum += 1;
				psrcItem->isFull = 0;// dvbSub_display_release
				startSubIdx ++;
				startSubIdx %= LDEC_MAX_LBS_SUB_NUM;
			}
		}
		thiz->ldecDisplay.readDispIdx ++;
		if(thiz->ldecDisplay.readDispIdx == LDEC_MAX_LBS_SUB_NUM)
			thiz->ldecDisplay.readDispIdx = 0;
	}
    _exit0:
    if (pOutputItem->subNum)
    	pOutputItem->isFull = 1;
    }
    //osal_switch_enable();
    thiz->FrmMgrLock.unlock(&thiz->FrmMgrLock);
    return ret;
}

/*******************************************************************************
Function name: Impl_ItxtDec_Open
Description: 
    1. 给lbsbuf和framequeue分配内存
Parameters: 
    
Return: 
    
Time: 2013/1/23
*******************************************************************************/
__s32 Impl_DvbsubDec_Open(__ldec_idvbsub_t *thiz)
{
    __s32   i;
    __ldec_info_t *ldecInfo =NULL;

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
    memset(thiz->ldecLbsBuf.ldecBuf, 0, thiz->nLbsBufSize);
    thiz->ldecLbsBuf.ldecBufEnd = thiz->ldecLbsBuf.ldecBuf + thiz->nLbsBufSize - 1;
	
    for(i=0; i<LDEC_MAX_LBS_SUB_NUM; i++)                                                               //* LDEC_MAX_LBS_SUB_NUM = 32;
    {
        thiz->ldecManager.subItem[i].pSubtitle = (__u8 *)PHY_MALLOC(sizeof(__u8)*thiz->nLdecFrameSize, 1024);  //* LDEC_MAX_LBS_SUB_SIZE = 64*1024
        if(thiz->ldecManager.subItem[i].pSubtitle == NULL)
	    goto _err_mopen;
    }
    thiz->ldecManager.reconBufIdx = -1;
    thiz->ldecManager.startBufIdx = 0;
	
    thiz->privInfo = NULL;
    ldecInfo = (__ldec_info_t *)malloc(sizeof(__ldec_info_t));
    if(ldecInfo == NULL)
    {
	goto _err_mopen;
    }
    thiz->privInfo = (void *)ldecInfo;
    
    ldecInfo->anciDataBuf = (__u8 *)PHY_MALLOC(sizeof(__u8)*thiz->nanciDataBufSize, 1024);	//* LDEC_MAX_LBS_ANCI_SIZE = 6*1024
    if(ldecInfo->anciDataBuf == NULL)
    {
    	WARNING("malloc anciDataBuf[%d] fail\n", thiz->nanciDataBufSize);
	goto _err_mopen;
    }
    memset(ldecInfo->anciDataBuf, 0, thiz->nanciDataBufSize);
    ldecInfo->subdecPage = (__ldec_page_t *)malloc(sizeof(__ldec_page_t));
    if(ldecInfo->subdecPage == NULL)
    {
	goto _err_mopen;
    }
    ldecInfo->subdecRegion = (__ldec_region_t *)malloc(LDEC_REGION_NUM*sizeof(__ldec_region_t));
    if(ldecInfo->subdecRegion == NULL)
    {
	goto _err_mopen;
    }
    ldecInfo->subdecClut = (__ldec_clut_t *)malloc(LDEC_CLUT_NUM * sizeof(__ldec_clut_t));	//* LDEC_CLUT_NUM = 5
    if(ldecInfo->subdecClut == NULL)
    {
	goto _err_mopen;
    }
    ldecInfo->subdecObject = (__ldec_object_t *)malloc(LDEC_OBJECT_NUM *sizeof(__ldec_object_t));       //* LDEC_OBJECT_NUM = 5;
    if(ldecInfo->subdecObject  == NULL)
    {
	goto _err_mopen;
    }
	
    ldecInfo->nextStep = LDEC_FIND_STREAM_ID;
    ldec_dvbsub_init_info(thiz);
    return LDEC_ERR_NONE;
    
_err_mopen:
    Impl_DvbsubDec_Close(thiz);
    return LDEC_ERR_DEC_SUBTITLE;
}

__s32 Impl_DvbsubDec_Close(__ldec_idvbsub_t *thiz)
{
    __s32   i;
    __ldec_info_t *ldecInfo = NULL;

    if(thiz->ldecLbsBuf.ldecBuf != NULL)
    {
        PHY_FREE(thiz->ldecLbsBuf.ldecBuf);
        thiz->ldecLbsBuf.ldecBuf = NULL;
    }
    for(i=0; i<LDEC_MAX_LBS_SUB_NUM; i++)
    {
        if(thiz->ldecManager.subItem[i].pSubtitle != NULL)
        {
            PHY_FREE(thiz->ldecManager.subItem[i].pSubtitle);
            thiz->ldecManager.subItem[i].pSubtitle = NULL;
        }
    }
    if(thiz->privInfo != NULL)
    {
        ldecInfo = (__ldec_info_t *)thiz->privInfo;
        if(ldecInfo->anciDataBuf != NULL)
        {
            PHY_FREE(ldecInfo->anciDataBuf); 
            ldecInfo->anciDataBuf = NULL;
        }
        if(ldecInfo->subdecPage != NULL)
        {
            free(ldecInfo->subdecPage);
            ldecInfo->subdecPage = NULL;
        }
        if(ldecInfo->subdecRegion != NULL)
        {
            free(ldecInfo->subdecRegion);
            ldecInfo->subdecRegion = NULL;
        }
        if(ldecInfo->subdecClut != NULL)
        {
            free(ldecInfo->subdecClut);
            ldecInfo->subdecClut = NULL;
        }
        if(ldecInfo->subdecObject != NULL)
        {
            free(ldecInfo->subdecObject);
            ldecInfo->subdecObject = NULL;
        }
    }
    return LDEC_ERR_NONE;
}

__s32 DvbsubDec_RequirePts(__ldec_idvbsub_t* thiz, __u32* pPts)
{   
	__Ldec_manager_t  *subdecManager;
	__u32 readDispIdx;
	__u32 startSubIdx;
	
    *pPts = 0;
	subdecManager = &(thiz->ldecManager);
	
    
    if(subdecManager->validSize == 0)
    {
    	//WARNING("%s(1)\n",__func__);
        return EPDK_FAIL;
    }
        
    thiz->FrmMgrLock.lock(&thiz->FrmMgrLock);
    readDispIdx = thiz->ldecDisplay.readDispIdx;
    startSubIdx = thiz->ldecDisplay.dispSubIdx[readDispIdx];
    if(!subdecManager->subItem[startSubIdx].isFull)
    {
    	//WARNING("%s(2)\n",__func__);
    	thiz->FrmMgrLock.unlock(&thiz->FrmMgrLock);
        return EPDK_FAIL;
    }
    else
     { 
     	//WARNING("%s(3):%d\n",__func__, subdecManager->subItem[startSubIdx].ptsVal);  
        *pPts = subdecManager->subItem[startSubIdx].ptsVal;
         return EPDK_OK;
     }
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
    
Time: 2013/1/23
*******************************************************************************/
__s32 Impl_DvbsubDec_Decode(__ldec_idvbsub_t *thiz)
{
    __ldec_lbs_buf_t   *subdecBuf;
	__ldec_info_t  *subdecInfo;
	__u8 findPesEndFlag = 0;
	__s8 result = LDEC_ERR_NONE;

    subdecBuf = &(thiz->ldecLbsBuf);
    subdecInfo = (__ldec_info_t *)thiz->privInfo;

    if(thiz->nSyncStatFlag) //socket_task()已经可以保证,在sync期间绝不调用requestwrite(), updatepointer()等.
    {
        thiz->LbsBufLock.lock(&thiz->LbsBufLock);
        thiz->FrmMgrLock.lock(&thiz->FrmMgrLock);
        ldec_dvbsub_init_info(thiz);
        thiz->FrmMgrLock.unlock(&thiz->FrmMgrLock);
        thiz->LbsBufLock.unlock(&thiz->LbsBufLock);
        thiz->nSyncStatFlag = 0;
        subdecInfo->nextStep = LDEC_FIND_STREAM_ID;
    }
	//********************************************************************//
	//******************* step_1 find subtitle stream id ******************//
        if(subdecInfo->nextStep == LDEC_FIND_STREAM_ID)       // find stream id 
		{   
			if(subdecBuf->validSize < LDEC_DATA_THRESHOLD)
				return LDEC_ERR_LBS_UNDERFLOW;
			if( subdec_find_startcode(subdecBuf, 0x20, 0x00, 1) < 0)
				return LDEC_ERR_LBS_UNDERFLOW;
			subdecInfo->nextStep = LDEC_PARSE_SEGMENT;
			subdecInfo->findStartCode = 1;
		}
		
		//**********************************************************************//
		//******************step_2 parse subtitle segment***********************//
		if(subdecInfo->nextStep == LDEC_PARSE_SEGMENT)
		{   
			while(1)
			{   
			    if(subdecInfo->findStartCode == 1)
			    {
					if(subdec_find_startcode(subdecBuf, 0x0F, 0, 0) < 0)
			    		return LDEC_ERR_LBS_UNDERFLOW;
			    }
			   
				result = dvbsub_parse_segment(thiz, &findPesEndFlag);
                		if(findPesEndFlag == 1)
				{  
					subdecInfo->nextStep = LDEC_FIND_STREAM_ID;
					break;
				}
				if(result != LDEC_ERR_NONE)
				{  
				   subdecInfo->findStartCode = 0;
				   if (result == LDEC_ERR_DEC_SUBTITLE)
				   {
				        thiz->LbsBufLock.lock(&thiz->LbsBufLock);
				        thiz->FrmMgrLock.lock(&thiz->FrmMgrLock);
				        ldec_dvbsub_init_info(thiz);
				        thiz->FrmMgrLock.unlock(&thiz->FrmMgrLock);
				        thiz->LbsBufLock.unlock(&thiz->LbsBufLock);
				   	subdecInfo->nextStep = LDEC_FIND_STREAM_ID;
				   	result = LDEC_ERR_NONE;
				   }
				   break;
			    }
			}
		}
	return result;
}

__s32 Impl_DvbsubDec_CheckSupport(__ldec_idvbsub_t *thiz, __s32 nFileFmt)   //ret = EPDK_TRUE, EPDK_FALSE, nFileFmt = enum __CEDARLIB_LYRIC_FILE_FMT
{
    if(LYRIC_DVBSUB== nFileFmt)
    {
        return EPDK_TRUE;
    }
    else
    {
        return EPDK_FALSE;
    }
}

void Impl_DvbsubDec_Destruct(__ldec_idvbsub_t *thiz)
{
    __ldec_idvbsub_t *pDec = (__ldec_idvbsub_t*)thiz;
    deleteDvbsubDec(pDec);
}

__s32 DvbsubDec_Init(__ldec_idvbsub_t *thiz)
{
    __s32   tmpret;
    memset(thiz, 0, sizeof(__ldec_idvbsub_t));
    thiz->subtitle_type = LYRIC_DVBSUB;
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
    
    thiz->DvbsubDecIoCtrl = Impl_DvbsubDec_IoCtrl;
    thiz->RequestWrite  = Impl_DvbsubDec_RequestWrite;
    thiz->UpdatePointer = Impl_DvbsubDec_UpdatePointer;
    thiz->GetLrcItem   = Impl_DvbsubDec_GetDvbItem;
    thiz->open          = Impl_DvbsubDec_Open;
    thiz->close         = Impl_DvbsubDec_Close;
    thiz->CommonDecode  = Impl_DvbsubDec_Decode;
    //return EPDK_OK;

    //覆盖基类某些虚成员
    strcpy((char*)thiz->strDecName, "Dvbdec");
    thiz->nLbsBufSize = LDEC_MAX_LBS_BUF_SIZE;
    thiz->nLdecFrameSize = LDEC_MAX_LBS_SUB_SIZE;//;LDEC_MAX_LBS_SUB_SIZE;//LDEC_MAX_FRM_SIZE
    thiz->nanciDataBufSize = LDEC_MAX_LBS_ANCI_SIZE;
		
    thiz->CheckSupport = Impl_DvbsubDec_CheckSupport;
    thiz->SubtitleDecode = NULL;
    thiz->Destruct = Impl_DvbsubDec_Destruct;
    
    //初始化自己的成员
    thiz->nCharEncodeType = LYRIC_SUB_ENCODE_ISO_8859_15;

    return EPDK_OK;
    
_err2_mopen:
    CedarLock_Exit(&thiz->LbsBufLock);
_err1_mopen:
//_err_mopen:
    return EPDK_FAIL;
}
__s32 DvbsubDec_Exit(__ldec_idvbsub_t *thiz)
{
    CedarLock_Exit(&thiz->LbsBufLock);
    CedarLock_Exit(&thiz->FrmMgrLock);
    return EPDK_OK;
}

__ldec_idvbsub_t *newDvbsubDec()
{
    __s32   tmpret;
    __ldec_idvbsub_t *pDvbDec = (__ldec_idvbsub_t*)malloc(sizeof(__ldec_idvbsub_t));
    if(NULL == pDvbDec)
    {
        WARNING("malloc __ldec_idvbsub_t fail\n");
        return NULL;
    }
    tmpret = DvbsubDec_Init(pDvbDec);
    if(tmpret != EPDK_OK)
    {
        WARNING("__ldec_idvbsub_t init fail\n");
        goto _err0;
    }
    return pDvbDec;
_err0:
    free(pDvbDec);
    return NULL;
}
void deleteDvbsubDec(__ldec_idvbsub_t *thiz)
{
    DvbsubDec_Exit(thiz);
    free(thiz);
}

#endif
