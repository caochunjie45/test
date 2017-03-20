
#include "pgs_s.h"
#include "pgs_decoder.h"
//#include "pgs.h"
//#include "fbm.h"

void pgs_lbs_reset(sub_decoder_t *pPgsDec)
{
    memset(pPgsDec->anciDataBuf, 0, pPgsDec->nanciDataBufSize);
    memset(pPgsDec->ldecLbsBuf.ldecBuf, 0, pPgsDec->nLbsBufSize);
    pPgsDec->ldecLbsBuf.readAddr = pPgsDec->ldecLbsBuf.ldecBuf;
    pPgsDec->ldecLbsBuf.writeAddr = pPgsDec->ldecLbsBuf.ldecBuf;
    pPgsDec->ldecLbsBuf.ldecIdxSize = 0;
    pPgsDec->ldecLbsBuf.ldecWriteIdx = 0;
    pPgsDec->ldecLbsBuf.ldecReadIdx  = 0;
    pPgsDec->ldecLbsBuf.validSize   = 0;
    //pPgsDec->useAnciBuf = 0;
}
/*******************************************************************************
Function name: PgsDecodeSegment
Description: 
    ��lbsbufȡһ��chunk,һ�����һ��segment����.
Parameters: 
    
Return: 
    PGS_ERR_LBS_UNDERFLOW
    PGS_ERR_NO_FRAMEBUFFER
    PGS_ERR_LFMT_ERR
    PGS_ERR_NONE:����ɹ�,����δ���һ֡
    PGS_ERR_DEC_ONE_FRAME:����ɹ�,���һ֡.
Time: 2011/10/19
*******************************************************************************/
static __s32 PgsDecodeSegment(sub_decoder_t *pPgsDec)
{
    __s32 ret = 0;
    __s32 tmpret;
    //__u8  reconIdx = 0; //ָʾsubItem[]�е�һ��Ԫ��,��һ����Ԫ��.
    //__u8  writeIdx = 0;
    __u16  readIdx= 0;   //lbsbuf�����ݿ����������
    //__u32 usedBytes = 0;    //����ǰ�ǽ���ǰ��lbs�ĳ���,��������õ���lbs�ĳ���
    __u8  *curDataPtr = NULL;
    //__s8  lbsChgFlg;
    //__cedar_subtitle_item_t *curSuItem;
    frm_info_t*		fb = NULL;
	fbm_t*			fbm;
    __u8*			data;   //��Ҫ�ĳ�һ��chunk����ʼ��ַ,���ܻ�ͷ
	__u32			dataLen;    //��Ҫ�ĳ�һ��chunk�ĳ���
    fbm = pPgsDec->fbm;
    
    /*step 0 select frame manager*/
    //lbs�ж�
    if(pPgsDec->ldecLbsBuf.validSize == 0 || 0 == pPgsDec->ldecLbsBuf.ldecIdxSize)
    {
        //WARNING("pbs lbs underflow![%d][%d]\n", pPgsDec->ldecLbsBuf.validSize, pPgsDec->ldecLbsBuf.ldecIdxSize);
        return PGS_ERR_LBS_UNDERFLOW;    
    }	

    //frame�жϲ�׼����
	pPgsDec->FrmMgrLock.lock(&pPgsDec->FrmMgrLock);
    tmpret = fbm_decoder_request_frm(&fb, fbm);
    pPgsDec->FrmMgrLock.unlock(&pPgsDec->FrmMgrLock);
    
    if(tmpret != 0)
    {
        //waitFrameBuffer();
        WARNING("pgs frame is full!\n");
        return PGS_ERR_NO_FRAMEBUFFER;
    }

    //lbsbuf������,frame�Ѿ�׼����. ��ʼ׼��lbsbuf
    readIdx = pPgsDec->ldecLbsBuf.ldecReadIdx;
    if(0 == pPgsDec->ldecLbsBuf.ldecBufCrl[readIdx].hasPtsFlag)
    {
        //WARNING("fatal error! lbsbufctrl hasPtsFlag = 0, [%d], ptsVal[%d]\n", readIdx, pPgsDec->ldecLbsBuf.ldecBufCrl[readIdx].ptsVal);
        pPgsDec->ldecLbsBuf.ldecBufCrl[readIdx].ptsVal = 0;
    }
    curDataPtr = ldec_process_chunk_cover_bufEnd(&pPgsDec->ldecLbsBuf, &pPgsDec->anciDataBuf, &pPgsDec->nanciDataBufSize);
    
    if(curDataPtr == NULL)
    {  
        WARNING("meet impossible case! clear lbsbuf!\n");
        pPgsDec->LbsBufLock.lock(&pPgsDec->LbsBufLock);
        ldec_update_read_pointer(&pPgsDec->ldecLbsBuf, pPgsDec->ldecLbsBuf.validSize);  //������fatal error�Ĵ���,lbsȫ�����
        pPgsDec->LbsBufLock.unlock(&pPgsDec->LbsBufLock);
        return PGS_ERR_LFMT_ERR;
    }

    //����
    data    = curDataPtr;    //��Ҫ�ĳ�һ��chunk����ʼ��ַ,���ܻ�ͷ
    dataLen = pPgsDec->ldecLbsBuf.ldecBufCrl[readIdx].dataLen;   //��Ҫ�ĳ�һ��chunk�ĳ���
	ret = pgs_dec_decode(pPgsDec->pgsDecCtx, data, dataLen, fb, (__u32)pPgsDec->ldecLbsBuf.ldecBufCrl[readIdx].ptsVal);   //Ӧ����һ�ν�һ��segment, p->pts�Ǹ�chunk��pts
	//����framequeue
	if (ret > 0)    //��֡��ʵ֡�Ѿ������
	{
		if(ret == 2)                    //* decoder decode a display end segment,
		{
//		    //* insert a empty frame here.
            frm_info_t* tmp = NULL;
		    //frm_info_t* tmp         = &p->emptyFrame;
		    while(1)
		    {
                pPgsDec->FrmMgrLock.lock(&pPgsDec->FrmMgrLock);
                tmpret = fbm_decoder_request_empty_frm(&tmp, fbm);
                pPgsDec->FrmMgrLock.unlock(&pPgsDec->FrmMgrLock);
                if(tmpret == -1)
                {
                    WARNING("[%s],fatal error! PGS request empty frame fail\n", __FUNCTION__);
                    esKRNL_TimeDly(10);
                }
                else
                {
                    break;
                }
		    }
//		    p->emptyFrame.uPTS      = fb->uPTS;
//		    p->emptyFrame.dataLen   = 0;
//		    p->emptyFrame.usedByDsp = 0;
//		    p->emptyFrame.inQueue   = 0;
            //WARNING("decode one empty pgsframe!pts[%d]\n", tmp->uPTS);
            tmp->uPTS      = fb->uPTS;
            tmp->dataLen   = 0;
            pPgsDec->FrmMgrLock.lock(&pPgsDec->FrmMgrLock);
		    fbm_decoder_return_valid_frm(&tmp, fbm);
		    fbm_decoder_return_invalid_frm(&fb, fbm);
            pPgsDec->FrmMgrLock.unlock(&pPgsDec->FrmMgrLock);
            
		}
		else
		{
            //WARNING("decode one pgsframe!pts[%d]\n", fb->uPTS);
		    fb->dataLen = PGS_FRAME_SIZE;
            pPgsDec->FrmMgrLock.lock(&pPgsDec->FrmMgrLock);
		    fbm_decoder_return_valid_frm(&fb, fbm);
            pPgsDec->FrmMgrLock.unlock(&pPgsDec->FrmMgrLock);
            
		}
	}
	else
	{
        pPgsDec->FrmMgrLock.lock(&pPgsDec->FrmMgrLock);
		fbm_decoder_return_invalid_frm(&fb, fbm);
        pPgsDec->FrmMgrLock.unlock(&pPgsDec->FrmMgrLock);
	}

    //����lbsbuf
    pPgsDec->LbsBufLock.lock(&pPgsDec->LbsBufLock);
    ldec_update_read_pointer(&pPgsDec->ldecLbsBuf, dataLen);
    pPgsDec->LbsBufLock.unlock(&pPgsDec->LbsBufLock);
    if(ret > 0)
    {
        return PGS_ERR_DEC_ONE_FRAME;
    }
    else
    {
        return PGS_ERR_NONE;
    }
}

__s32 Impl_PgsDec_Open(PgsDecInterface *thiz)
{
    //__u8            err;
    sub_decoder_t*  p = (sub_decoder_t*)thiz;
    // ldecLbsBuf��ancibuf
    if(p->nLbsBufSize <= 0 || p->nanciDataBufSize<=0)
    {
        WARNING("fatal error! check nLbsBufSize[%d],[%d]\n", p->nLbsBufSize, p->nanciDataBufSize);
    }
    p->ldecLbsBuf.ldecBuf = (__u8 *)PHY_MALLOC(sizeof(__u8)*p->nLbsBufSize, 1024);
    if(p->ldecLbsBuf.ldecBuf == NULL)
    {
        WARNING("malloc lbsbuf[%d] fail\n", p->nLbsBufSize);
        goto _err0;
    }
    p->ldecLbsBuf.ldecBufEnd = p->ldecLbsBuf.ldecBuf + p->nLbsBufSize - 1;
    p->anciDataBuf = (__u8 *)PHY_MALLOC(sizeof(__u8)*p->nanciDataBufSize, 1024);
    if(p->anciDataBuf == NULL)
    {
        WARNING("malloc anciDataBuf[%d] fail\n", p->nanciDataBufSize);
        goto _err1;
    }
    pgs_lbs_reset(p);

    // framebuf��decCtx
    p->fbm = fbm_create(PGS_FRAME_SIZE, PGS_FRAME_BUFFER_NUM);
    if(p->fbm == NULL)
    {
        MESSAGE("create frame buffer manager fail.\n");
        goto _exit_subdec_open;
    }

    p->pgsDecCtx = pgs_dec_create();
    if (p->pgsDecCtx == NULL)
    {
        MESSAGE("create subtitle decoder fail.\n");
        goto _exit_subdec_open;
    }

    return PGS_ERR_NONE;

_exit_subdec_open:
    //* close decoder
    if (p->pgsDecCtx)
    {
        pgs_dec_destroy(&p->pgsDecCtx);
        p->pgsDecCtx = NULL;
    }
    
    //* release frame buffer
    if(p->fbm)
    {
        fbm_destroy(&p->fbm);
        p->fbm = NULL;
    }

_err1:
    if(p->ldecLbsBuf.ldecBuf)
    {
        PHY_FREE(p->ldecLbsBuf.ldecBuf);
        p->ldecLbsBuf.ldecBuf = NULL;
    }
    if(p->anciDataBuf)
    {
        PHY_FREE(p->anciDataBuf);
        p->anciDataBuf = NULL;
    }

_err0:
    return PGS_ERR_FAIL;
}

__s32 Impl_PgsDec_Close(PgsDecInterface *thiz)
{
//     __u8            err;
    sub_decoder_t*  p = (sub_decoder_t*)thiz;

    //* close decoder
    //frame��decCtx
    if (p->pgsDecCtx)
    {
        pgs_dec_destroy(&p->pgsDecCtx);
        p->pgsDecCtx = NULL;
    }
    
    //* release frame buffer
    if(p->fbm)
    {
        fbm_destroy(&p->fbm);
        p->fbm = NULL;
    }
    
    // ldecLbsBuf��ancibuf
    if(p->anciDataBuf)
    {
        PHY_FREE(p->anciDataBuf);
        p->anciDataBuf = NULL;
    }
    if(p->ldecLbsBuf.ldecBuf!= NULL)
    {
        PHY_FREE(p->ldecLbsBuf.ldecBuf);
        p->ldecLbsBuf.ldecBuf = NULL;
    }

    return PGS_ERR_NONE;
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
__s32 Impl_PgsDec_RequestWrite(PgsDecInterface *thiz, __u32 uSizeRequested, __u8 **ppData0, __u32 *puSizeGot0, __u8 **ppData1, __u32 *puSizeGot1)
{
    __s32   ret;
    __u32 remainBufSize;
    __u32   nSize0 = 0, nSize1 = 0;
    
    __u16   ldecIdxSize; //lbs buffer�Ĺ������Ѿ�ʹ�õĸ���
    __u32   validSize; //LBS buffer����Ч�ֽ���, ���������ִ���ڼ�,validsizeֻ���ܱ��,��������ν
    __u8    *writeAddr;    //��ǰ��д��ַ, �ڴ��ڼ�, writeAddrһֱ�����
    sub_decoder_t *pPgsDec = (sub_decoder_t*)thiz;

    //�������,��Ϊ������ֻһ��.ֻ��ͨ����������decode()����.
    pPgsDec->LbsBufLock.lock(&pPgsDec->LbsBufLock);
    ldecIdxSize = pPgsDec->ldecLbsBuf.ldecIdxSize; //lbs buffer�Ĺ������Ѿ�ʹ�õĸ���
    validSize = pPgsDec->ldecLbsBuf.validSize; //LBS buffer����Ч�ֽ���, ���������ִ���ڼ�,validsizeֻ���ܱ��,��������ν
    writeAddr = pPgsDec->ldecLbsBuf.writeAddr;    //��ǰ��д��ַ, �ڴ��ڼ�, writeAddrһֱ�����
    remainBufSize = pPgsDec->nLbsBufSize - validSize;
    if(remainBufSize < uSizeRequested)
    {
        WARNING("lbs buf underflow! [%d][%d]\n", remainBufSize, uSizeRequested);
        ret = PGS_ERR_FAIL;
        goto _exit0;
    }
    if(ldecIdxSize >= LDEC_MAX_TXT_DATA_NUM)
    {
        WARNING("[%d] >= LDEC_MAX_TXT_DATA_NUM[%d]\n", ldecIdxSize, LDEC_MAX_TXT_DATA_NUM);
        ret = PGS_ERR_FAIL;
        goto _exit0;
    }
    //��ʼ��д����Ĳ���
    nSize0 = pPgsDec->ldecLbsBuf.ldecBufEnd + 1 - writeAddr;
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
        *ppData1 = pPgsDec->ldecLbsBuf.ldecBuf;
        *puSizeGot1 = nSize1;
    }
    ret = PGS_ERR_NONE;
    goto _exit0;

_exit0:
    pPgsDec->LbsBufLock.unlock(&pPgsDec->LbsBufLock);
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
__s32 Impl_PgsDec_UpdatePointer(PgsDecInterface *thiz, __lbs_datctlblk_t * pdatctl,__u32 uDataSize)
{
    __u16 writeIdx; //�ڴ��ڼ䲻���
    __s32 remainBufSize;
    sub_decoder_t *pPgsDec = (sub_decoder_t*)thiz;
    //���Ȱ�LDEC_FIRST_PART_CTRL��������, �����Դ��mkv����vob, �Զ�����,Ŀǰ������,��Ϊmkv, mpg psrû��first ctrl bit
//    if(CEDARLIB_FILE_FMT_MKV == thiz->subtitle_bs_src || CEDARLIB_FILE_FMT_VOB == thiz->subtitle_bs_src)
//    {
//        pdatctl->CtrlBits |= LDEC_FIRST_PART_CTRL;
//        pdatctl->CtrlBits |= LDEC_LAST_PART_CTRL;
//    }
    //�����������
    pPgsDec->LbsBufLock.lock(&pPgsDec->LbsBufLock);
    if(pPgsDec->ldecLbsBuf.ldecIdxSize >= LDEC_MAX_TXT_DATA_NUM)
    {
        WARNING("fatal error! ldecIdxSize[%d]>=LDEC_MAX_TXT_DATA_NUM[%d] \n", pPgsDec->ldecLbsBuf.ldecIdxSize, LDEC_MAX_TXT_DATA_NUM);
    }
    writeIdx = pPgsDec->ldecLbsBuf.ldecWriteIdx;

    if(pdatctl->CtrlBits & LDEC_FIRST_PART_CTRL)
    {
        memset(&pPgsDec->ldecLbsBuf.ldecBufCrl[writeIdx],0x00, sizeof( __ldec_buffer_ctrl_t));
        pPgsDec->ldecLbsBuf.ldecBufCrl[writeIdx].startAddr = pPgsDec->ldecLbsBuf.writeAddr;
        pPgsDec->ldecLbsBuf.ldecBufCrl[writeIdx].lbsChgFlg = pdatctl->LbsChgFlg;
    }
    else
    {
        WARNING("not meet FIRST_PART_CTRL.uDataSize[%d]\n", uDataSize);
    }
    if(pdatctl->CtrlBits & LDEC_PTS_VALID_CTRL)
    {
        pPgsDec->ldecLbsBuf.ldecBufCrl[writeIdx].hasPtsFlag = 1;
        pPgsDec->ldecLbsBuf.ldecBufCrl[writeIdx].ptsVal = pdatctl->uPTS;
    }
    else
    {
        WARNING("one pgs segment has no pts?\n");
    }
    if(pdatctl->CtrlBits & LDEC_DURATION_VALID_CTRL)
    {
        pPgsDec->ldecLbsBuf.ldecBufCrl[writeIdx].duration = pdatctl->uDuration;
    }

    if(pdatctl->CtrlBits & LDEC_DATATYPE_VALID_CTRL)
    {
        pPgsDec->ldecLbsBuf.ldecBufCrl[writeIdx].dataType = pdatctl->uDatatype;
    }
    
    //pPgsDec->ldecLbsBuf.validSize += uDataSize;
    pPgsDec->ldecLbsBuf.ldecBufCrl[writeIdx].dataLen += uDataSize;
    if((pPgsDec->ldecLbsBuf.writeAddr+uDataSize) > pPgsDec->ldecLbsBuf.ldecBufEnd)
    {
        remainBufSize = pPgsDec->ldecLbsBuf.ldecBufEnd - pPgsDec->ldecLbsBuf.writeAddr+1;
        pPgsDec->ldecLbsBuf.writeAddr = pPgsDec->ldecLbsBuf.ldecBuf + uDataSize - remainBufSize;
        pPgsDec->ldecLbsBuf.ldecBufCrl[writeIdx].coverBufEnd = 1;
    }
    else
    {
        if(pPgsDec->ldecLbsBuf.writeAddr + uDataSize == pPgsDec->ldecLbsBuf.ldecBufEnd + 1)
        {
            pPgsDec->ldecLbsBuf.writeAddr = pPgsDec->ldecLbsBuf.ldecBuf;
        }
        else
        {
            pPgsDec->ldecLbsBuf.writeAddr += uDataSize;
        }
        //pPgsDec->ldecLbsBuf.ldecBufCrl[writeIdx].coverBufEnd = 0; //һ�����ݶ�δ���,�п��ܻ�ͷ.
    }
    if(pdatctl->CtrlBits & LDEC_LAST_PART_CTRL)
    {
        //WARNING("update chunk![%d][%d][%d]\n", pPgsDec->ldecLbsBuf.ldecWriteIdx, pPgsDec->ldecLbsBuf.ldecBufCrl[writeIdx].dataLen, uDataSize);
        pPgsDec->ldecLbsBuf.validSize += pPgsDec->ldecLbsBuf.ldecBufCrl[writeIdx].dataLen;  //validSize�����ldecWriteIdxһ����Ч,�������������, �����������.
        if(pPgsDec->ldecLbsBuf.ldecWriteIdx + 1 == LDEC_MAX_TXT_DATA_NUM)
        {
            pPgsDec->ldecLbsBuf.ldecWriteIdx = 0;
        }
        else
        {
            pPgsDec->ldecLbsBuf.ldecWriteIdx++;
        }
        pPgsDec->ldecLbsBuf.ldecIdxSize ++;
        
    }
    else
    {
        WARNING("not meet LAST_PART_CTRL.uDataSize[%d]\n", uDataSize);
    }
    
    //WARNING("update one pgs chunk!pts[%d]\n", (__u32)pdatctl->uPTS);
    pPgsDec->LbsBufLock.unlock(&pPgsDec->LbsBufLock);

    return EPDK_OK;
}

__s32 Impl_PgsDec_RequestFrame(PgsDecInterface *thiz, subtitle_frame_t *pItem)
{
    __s32               result;
    frm_info_t*         frm_info = NULL;
    sub_decoder_t       *p = (sub_decoder_t*)thiz;
    
    p->FrmMgrLock.lock(&p->FrmMgrLock);
    result = fbm_dsp_request_frm(&frm_info, p->fbm);
    p->FrmMgrLock.unlock(&p->FrmMgrLock);
    if(result != 0)
    {
    	return PGS_ERR_FAIL;
    }
    
    //get the frame buffer information
    if(frm_info->dataLen == 0)  //��֡
    {
        //* end notify
        pItem->index		= fbm_empty_ptr_to_index(frm_info, p->fbm);
        pItem->isFull		= 0;
        pItem->subWidth		= 0;
        pItem->subHeight	= 0;
        pItem->startX		= 0;
        pItem->startY		= 0;
        pItem->videoWidth	= 0;
        pItem->videoHeight	= 0;
        pItem->ptsVal		= frm_info->uPTS;
        pItem->timeDuration = 0;
        pItem->pSubtitle	= NULL;
        pItem->nextItem		= NULL;
    }
    else
    {
        pItem->index		= fbm_ptr_to_index(frm_info, p->fbm);
        pItem->isFull		= 1;
        pItem->subWidth		= frm_info->width;
        pItem->subHeight	= frm_info->height;
        pItem->startX		= frm_info->x;
        pItem->startY		= frm_info->y;
        pItem->videoWidth	= frm_info->videoWidth;
        pItem->videoHeight	= frm_info->videoHeight;
        pItem->ptsVal		= frm_info->uPTS;
        pItem->timeDuration = frm_info->endDispTime - frm_info->startDispTime;
        pItem->pSubtitle	= frm_info->data;
        pItem->nextItem		= NULL;
    }
    
    return PGS_ERR_NONE;
}

__s32 Impl_PgsDec_ReleaseFrame(PgsDecInterface *thiz, subtitle_frame_t *pItem)
{
    frm_info_t*         frm_info;
    sub_decoder_t       *p = (sub_decoder_t*)thiz;
    if(pItem->pSubtitle)
    {
        frm_info = fbm_index_to_ptr(pItem->index, p->fbm);
        if(frm_info != NULL)
        {
            frm_info->data = pItem->pSubtitle;
            p->FrmMgrLock.lock(&p->FrmMgrLock);    
            fbm_dsp_return_frm(frm_info, p->fbm);
            p->FrmMgrLock.unlock(&p->FrmMgrLock);
            //releaseFrameBuffer();
            return PGS_ERR_NONE;
        }
        else
        {
            WARNING("fatal error! release frame is not find!\n");
            return PGS_ERR_FAIL;
        }
    }
    else    //��֡
    {
        frm_info = fbm_empty_index_to_ptr(pItem->index, p->fbm);
        if(frm_info != NULL)
        {
            frm_info->data = pItem->pSubtitle;
            p->FrmMgrLock.lock(&p->FrmMgrLock);  
            fbm_dsp_return_frm(frm_info, p->fbm);
            p->FrmMgrLock.unlock(&p->FrmMgrLock);
            return PGS_ERR_NONE;
        }
        else
        {
            WARNING("fatal error! release empty frame is not find!\n");
            return PGS_ERR_FAIL;
        }
    }
}

/*******************************************************************************
Function name: Impl_PgsDec_Decode
Description: 
    1.���ñ�����������һ֡������������������ŷ���.
Parameters: 
    
Return: 
    PGS_ERR_LBS_UNDERFLOW
    PGS_ERR_NO_FRAMEBUFFER
    PGS_ERR_LFMT_ERR:
    PGS_ERR_NONE:����һ֡.
    PGS_ERR_FAIL;
Time: 2011/10/19
*******************************************************************************/
__s32 Impl_PgsDec_Decode(PgsDecInterface *thiz)
{
    __s32 ret = 0;
    __s32 tmpret;
    //__u8  reconIdx = 0; //ָʾsubItem[]�е�һ��Ԫ��,��һ����Ԫ��.
    //__u8  writeIdx = 0;
    //__u8  readIdx= 0;   //lbsbuf�����ݿ����������
    //__u32 usedBytes = 0;    //����ǰ�ǽ���ǰ��lbs�ĳ���,��������õ���lbs�ĳ���
    //__u8  *curDataPtr = NULL;
    //__s8  lbsChgFlg;
    //__cedar_subtitle_item_t *curSuItem;
    sub_decoder_t   *pPgsDec = (sub_decoder_t*)thiz;

    if(pPgsDec->nSyncStatFlag) //socket_task()�Ѿ����Ա�֤,��sync�ڼ��������requestwrite(), updatepointer()��.
    {
        pPgsDec->LbsBufLock.lock(&pPgsDec->LbsBufLock);
        pPgsDec->FrmMgrLock.lock(&pPgsDec->FrmMgrLock);
        pgs_lbs_reset(pPgsDec);
        pgs_dec_reset(pPgsDec->pgsDecCtx);
        fbm_reset(pPgsDec->fbm, 1);
        pPgsDec->LbsBufLock.unlock(&pPgsDec->LbsBufLock);
        pPgsDec->FrmMgrLock.unlock(&pPgsDec->FrmMgrLock);
        pPgsDec->nSyncStatFlag = 0;
    }

    while(1)
    {
        tmpret = PgsDecodeSegment(pPgsDec);
        switch(tmpret)
        {
            case PGS_ERR_LBS_UNDERFLOW:
            case PGS_ERR_NO_FRAMEBUFFER:
            case PGS_ERR_LFMT_ERR:
            {
                ret = tmpret;
                break;
            }
            case PGS_ERR_DEC_ONE_FRAME:
            {
                ret = PGS_ERR_NONE;
                break;
            }
            case PGS_ERR_NONE:
            {
                continue;
            }
            default:
            {
                WARNING("fatal error! ret[%x]\n", tmpret);
                ret = PGS_ERR_FAIL;
                break;
            }
        }
        break;
    }
    return ret;
}

__s32 Impl_PgsDec_IoCtrl(PgsDecInterface *thiz, __s32 cmd, __s32 aux, void *pbuffer)
{
    __s32   ret;
    sub_decoder_t *pPgsDec = (sub_decoder_t*)thiz;
    switch(cmd)
    {
        case PGS_CMD_SET_SUBTITLE_SOURCE:
        {
            pPgsDec->base.subtitle_bs_src = aux;
            ret = PGS_ERR_NONE;
            break;
        }
        case PGS_CMD_GET_NEXT_PTS:
        {
            if(fbm_dsp_get_show_time((__u32*)pbuffer, pPgsDec->fbm) == 0)
            {
                ret = PGS_ERR_NONE;
            }
            else
            {
                ret = PGS_ERR_FAIL;
            }
            break;
        }
        case PGS_CMD_NOTIFY_SYNCSTAT:
        {
            pPgsDec->nSyncStatFlag = 1;
            ret = PGS_ERR_NONE;
            break;
        }
        case PGS_CMD_CHECK_SYNCSTAT:
        {
            ret = pPgsDec->nSyncStatFlag;
            break;
        }
        default:
        {
            WARNING("fatal error! unkown cmd[%x]\n", cmd);
            ret = PGS_ERR_FAIL;
            break;
        }
    }
    return ret;
}

__s32 PgsDecInterface_Init(PgsDecInterface *thiz)
{
    memset(thiz, 0, sizeof(PgsDecInterface));
    thiz->subtitle_type = LYRIC_PGS;
    strcpy((char*)thiz->strDecName, "Pgsdec");
    return PGS_ERR_NONE;
}

__s32 PgsDecInterface_Exit(PgsDecInterface *thiz)
{
    return PGS_ERR_NONE;
}
__s32 PgsDec_Init(sub_decoder_t *thiz)
{
    __s32   ret;
    __s32   tmpret;
    memset(thiz, 0, sizeof(sub_decoder_t));
    //�ȳ�ʼ������
    ret = PgsDecInterface_Init(&thiz->base);
    if(PGS_ERR_NONE != ret)
    {
        WARNING("PgsDecInterface Init fail\n");
        goto _err0;
    }
    //�����麯��
    thiz->base.open = Impl_PgsDec_Open;
    thiz->base.close = Impl_PgsDec_Close;
    thiz->base.RequestWrite = Impl_PgsDec_RequestWrite;
    thiz->base.UpdatePointer = Impl_PgsDec_UpdatePointer;
    thiz->base.RequestFrame = Impl_PgsDec_RequestFrame;
    thiz->base.ReleaseFrame = Impl_PgsDec_ReleaseFrame;
    thiz->base.decode = Impl_PgsDec_Decode;
    thiz->base.PgsIoCtrl = Impl_PgsDec_IoCtrl;
    
    //��ʼ���������
    tmpret = CedarLock_Init(&thiz->LbsBufLock);
    if(tmpret != EPDK_OK)
    {
        WARNING("cedarlock init fail\n");
        goto _err1;
    }
    tmpret = CedarLock_Init(&thiz->FrmMgrLock);
    if(tmpret != EPDK_OK)
    {
        WARNING("cedarlock init fail\n");
        goto _err2;
    }
    thiz->nLbsBufSize = PGS_VBV_BUFFER_SIZE;
    thiz->nanciDataBufSize = PGS_VBV_BUFFFR_ANCI_SIZE;
    return PGS_ERR_NONE;

_err2:
    CedarLock_Exit(&thiz->LbsBufLock);
_err1:
    PgsDecInterface_Exit(&thiz->base);
_err0:
    return PGS_ERR_FAIL;
}
__s32 PgsDec_Exit(sub_decoder_t *thiz)
{
    //��Ϊ�Ḳ�ǻ���Ԫ�ص�Ե��. һ�����ͷ��Լ���,�ٴ�������.
    CedarLock_Exit(&thiz->LbsBufLock);
    CedarLock_Exit(&thiz->FrmMgrLock);
    PgsDecInterface_Exit(&thiz->base);
    return LDEC_ERROR_OK;
}

PgsDecInterface* newPgsDecInterface()
{
    __s32   tmpret;
    sub_decoder_t *pPgsDec = (sub_decoder_t*)malloc(sizeof(sub_decoder_t));
    if(NULL == pPgsDec)
    {
        WARNING("malloc pPgsDec fail\n");
        return NULL;
    }
    tmpret = PgsDec_Init(pPgsDec);
    if(tmpret != PGS_ERR_NONE)
    {
        WARNING("pPgsDec init fail\n");
        goto _err0;
    }
    return &pPgsDec->base;
    
_err0:
    free(pPgsDec);
    return NULL;
}
void deletePgsDecInterface(PgsDecInterface *thiz)
{
    sub_decoder_t *pPgsDec = (sub_decoder_t*)thiz;
    PgsDec_Exit(pPgsDec);
    free(pPgsDec);
}

#if 0   //20111019������
sub_decoder_t* subDecCtrl = NULL;

void  releaseFrameBuffer(void);          //* display task release frame buffer.
__s32 waitFrameBuffer(void);             //* decode task wait for frame buffer.
/*******************************************************************************
Function name: SubtitleDecDecode
Description: 
    //1.��lbsbuf, ��Ϊ���ȷ���lbs underflow
    //2.��framebuf


    //3.whileѭ����ʼ����
    //3.1 ��lbsbuf
    //3.2 decode
Parameters: 
    
Return: 
    
Time: 2011/10/19
*******************************************************************************/
static void SubtitleDecDecode(sub_decoder_t* p)
{
	__s32			ret;
	__u8*			data;
	__u32			dataLen;
	frm_info_t*		fb;
	fbm_t*			fbm;

	fbm = p->fbm;
	
	do
	{
	    ret = fbm_decoder_request_frm(&fb, fbm);
	    
	    if(ret != 0)
		    waitFrameBuffer();
		
		break;
		
	}while(ret != 0);

    data    = p->vbvBuf;    //��Ҫ�ĳ�һ��chunk����ʼ��ַ,���ܻ�ͷ
    dataLen = p->vbvDataSize;   //��Ҫ�ĳ�һ��chunk�ĳ���

	ret = pgs_dec_decode(p->pgsDecCtx, data, dataLen, fb, (__u32)p->pts);   //Ӧ����һ�ν�һ��segment, p->pts�Ǹ�chunk��pts

	if (ret > 0)
	{
		if(ret == 2)                    //* decoder decode a display end segment,
		{
//		    //* insert a empty frame here.
            frm_info_t* tmp = NULL;
		    //frm_info_t* tmp         = &p->emptyFrame;
		    while(-1 == fbm_decoder_request_empty_frm(&tmp, fbm))
		    {
                WARNING("[%s],fatal error! PGS request empty frame fail\n", __FUNCTION__);
                esKRNL_TimeDly(10);
		    }
//		    p->emptyFrame.uPTS      = fb->uPTS;
//		    p->emptyFrame.dataLen   = 0;
//		    p->emptyFrame.usedByDsp = 0;
//		    p->emptyFrame.inQueue   = 0;
            tmp->uPTS      = fb->uPTS;
            tmp->dataLen   = 0;
		    fbm_decoder_return_valid_frm(&tmp, fbm);
		    fbm_decoder_return_invalid_frm(&fb, fbm);
		}
		else
		{
		    fb->dataLen = PGS_FRAME_SIZE;
		    fbm_decoder_return_valid_frm(&fb, fbm);
		}
	}
	else
	{
		fbm_decoder_return_invalid_frm(&fb, fbm);
	}
	
    return;
}

void SubtitleDecInit(void)
{
    subDecCtrl = NULL;
}

void SubtitleDecExit(void)
{
    return;
}

__mp* SubtitleDecOpen(__u32 mid, __u32 mode)
{
    __u8            err;
    sub_decoder_t*  p;

    p = (sub_decoder_t*)malloc(sizeof(sub_decoder_t));
    if (!p)
    {
        MESSAGE("malloc buffer for sub_decoder_t fail.\n");
        goto _exit_subdec_open;
    }

    memset(p, 0, sizeof(sub_decoder_t));
    p->mid          = mid;

    p->frameBufSem  = esKRNL_SemCreate(0);
    if (p->frameBufSem == NULL)
    {
        MESSAGE("create semaphores fail.\n");
        goto _exit_subdec_open;
    }
    
    p->fbm = fbm_create(PGS_FRAME_SIZE, PGS_FRAME_BUFFER_NUM);
    if(p->fbm == NULL)
    {
        MESSAGE("create frame buffer manager fail.\n");
        goto _exit_subdec_open;
    }
    
    p->vbvBuf = (__u8*)malloc(PGS_VBV_BUFFER_SIZE);
    if(p->vbvBuf == NULL)
    {
        MESSAGE("create vbv buffer fail.\n");
        goto _exit_subdec_open;
    }
    p->vbvBufSize = PGS_VBV_BUFFER_SIZE;
    p->vbvBufEnd  = p->vbvBuf + p->vbvBufSize;

    p->pgsDecCtx = pgs_dec_create();
    if (p->pgsDecCtx == NULL)
    {
        MESSAGE("create subtitle decoder fail.\n");
        goto _exit_subdec_open;
    }

    subDecCtrl = p;
    return (__mp*)subDecCtrl;

_exit_subdec_open:
    if (p)
    {
        //* close decoder
        if (p->pgsDecCtx)
        {
            pgs_dec_destroy(&p->pgsDecCtx);
        }
        
        //* release frame buffer
        if(p->fbm)
        {
            fbm_destroy(&p->fbm);
        }
        
        //* release vbv buffer
        if(p->vbvBuf)
        {
            free(p->vbvBuf);
            p->vbvBuf     = NULL;
            p->vbvBufEnd  = NULL;
            p->vbvBufSize = 0;
        }

        //* delete semaphores
        if (p->frameBufSem)
        {
            esKRNL_SemDel(p->frameBufSem, OS_DEL_ALWAYS, &err);
            p->frameBufSem = NULL;
        }

        free(p);
    }

    return NULL;
}


__s32 SubtitleDecClose(__mp* mp)
{
    __u8            err;
    sub_decoder_t*  p;

    p = (sub_decoder_t*)mp;
    if (p)
    {
        //* close decoder
        if (p->pgsDecCtx)
        {
            pgs_dec_destroy(&p->pgsDecCtx);
        }
        
        //* release frame buffer
        if(p->fbm)
        {
            fbm_destroy(&p->fbm);
        }
        
        //* release vbv buffer
        if(p->vbvBuf)
        {
            free(p->vbvBuf);
            p->vbvBuf     = NULL;
            p->vbvBufEnd  = NULL;
            p->vbvBufSize = 0;
        }

        //* delete semaphores
        if (p->frameBufSem)
        {
            esKRNL_SemDel(p->frameBufSem, OS_DEL_ALWAYS, &err);
            p->frameBufSem = NULL;
        }

        free(p);

        subDecCtrl = NULL;
        return 0;
    }
    else
    {
        MESSAGE("error subtitle decoder handle.\n");
        return -1;
    }
}

__u32 SubtitleDecRead(void* pdata, __u32 size, __u32 pid, __mp* mp)
{
    return 0;
}

__u32 SubtitleDecWrite(const void* pdata, __u32 size, __u32 pid, __mp* mp)
{
    return 0;
}

__s32 SubtitleDecIOCtrl(__mp* mp, __u32 cmd, __s32 aux, void* pbuffer)
{
    __s32 result;
    sub_decoder_t* p;

    p = (sub_decoder_t*)mp;
    if (p == NULL)
        return EPDK_FAIL;

    switch (cmd)
    {
    case SUBTITLE_CMD_START:
        {
            return EPDK_OK;
        }
        
    case SUBTITLE_CMD_STOP:
        {
            return EPDK_OK;
        }
        
    case SUBTITLE_CMD_REQUEST_BUFFER:
        {
            __md_buf_t* mdBuf;

            mdBuf = (__md_buf_t*)pbuffer;
            mdBuf->bufSize = 0;
            
            mdBuf->buf = p->vbvBuf;
            mdBuf->bufSize = p->vbvBufSize;
            return EPDK_OK;
        }
        
    case SUBTITLE_CMD_UPDATE_DATA:
        {
            __md_data_info_t*   mdInfo;
            __u32               first;
            __u32               last;
            
            WARNING("update data.\n");

            mdInfo = (__md_data_info_t*)pbuffer;
            if (mdInfo->ctrlBits & FIRST_PART_BIT)
                first = 1;
            else
                first = 0;

            if (mdInfo->ctrlBits & LAST_PART_BIT)
                last = 1;
            else
                last = 0;

            if (mdInfo->ctrlBits & PTS_VALID_BIT)
                p->pts = mdInfo->pts;
            else
                p->pts = 0;
                
            p->vbvDataSize = mdInfo->dataLen;
                
            if(first == 0 || last == 0)
            {
                WARNING("update data fail.\n");
                return EPDK_OK;
            }
                
            SubtitleDecDecode(p);
            WARNING("update data ok.\n");
            
            return EPDK_OK;
        }
        
    case SUBTITLE_CMD_REQUEST_FRAME:
        {
            frm_info_t*         frm_info;
            subtitle_frame_t*   pItem;
            
            pItem = (subtitle_frame_t*)pbuffer;
            
	        result = fbm_dsp_request_frm(&frm_info, p->fbm);
	        if(result != 0)
	        {
	        	return EPDK_FAIL;
	        }
            
            //get the frame buffer information
            if(frm_info->dataLen == 0)  //��֡
            {
                //* end notify
	            pItem->index		= fbm_empty_ptr_to_index(frm_info, p->fbm);
	            pItem->isFull		= 0;
	            pItem->subWidth		= 0;
	            pItem->subHeight	= 0;
	            pItem->startX		= 0;
	            pItem->startY		= 0;
	            pItem->videoWidth	= 0;
	            pItem->videoHeight	= 0;
	            pItem->ptsVal		= frm_info->uPTS;
	            pItem->timeDuration = 0;
	            pItem->pSubtitle	= NULL;
	            pItem->nextItem		= NULL;
            }
            else
            {
	            pItem->index		= fbm_ptr_to_index(frm_info, p->fbm);
	            pItem->isFull		= 1;
	            pItem->subWidth		= frm_info->width;
	            pItem->subHeight	= frm_info->height;
	            pItem->startX		= frm_info->x;
	            pItem->startY		= frm_info->y;
	            pItem->videoWidth	= frm_info->videoWidth;
	            pItem->videoHeight	= frm_info->videoHeight;
	            pItem->ptsVal		= frm_info->uPTS;
	            pItem->timeDuration = frm_info->endDispTime - frm_info->startDispTime;
	            pItem->pSubtitle	= frm_info->data;
	            pItem->nextItem		= NULL;
            }
	        
	        return EPDK_OK;
        }
        
    case SUBTITLE_CMD_RELEASE_FRAME:
        {
            frm_info_t*         frm_info;
            subtitle_frame_t*   pItem;
            
            pItem = (subtitle_frame_t*)pbuffer;
	        if(pItem->pSubtitle)
	        {
    	        frm_info = fbm_index_to_ptr(pItem->index, p->fbm);
    	        if(frm_info != NULL)
    	        {
    	            frm_info->data = pItem->pSubtitle;
    	        
    	            fbm_dsp_return_frm(frm_info, p->fbm);
                    releaseFrameBuffer();
                
                    return EPDK_OK;
                }
                else
                {
                    WARNING("fatal error! release frame is not find!\n");
                    return EPDK_FAIL;
                }
	        }
            else    //��֡
            {
                frm_info = fbm_empty_index_to_ptr(pItem->index, p->fbm);
    	        if(frm_info != NULL)
    	        {
    	            frm_info->data = pItem->pSubtitle;
    	        
    	            fbm_dsp_return_frm(frm_info, p->fbm);
                    return EPDK_OK;
                }
                else
                {
                    WARNING("fatal error! release empty frame is not find!\n");
                    return EPDK_FAIL;
                }
            }
        }
                       
    case SUBTITLE_CMD_GET_NEXT_PTS:
        {
            if(fbm_dsp_get_show_time((__u32*)pbuffer, p->fbm) == 0)
                return EPDK_OK;
            else
                return EPDK_FAIL;
        }
                                    
    case SUBTITLE_CMD_RESET:
        {
            pgs_dec_reset(p->pgsDecCtx);
            fbm_reset(p->fbm, 1);
            releaseFrameBuffer();
            return EPDK_OK;
        }

    default:
        {
            return EPDK_FAIL;
        }
    }
}

void releaseFrameBuffer(void)
{
    OS_SEM_DATA semInfo;
    esKRNL_SemQuery(subDecCtrl->frameBufSem, &semInfo);
    if (semInfo.OSCnt == 0)
    {
        esKRNL_SemPost(subDecCtrl->frameBufSem);
    }
}

__s32 waitFrameBuffer(void)
{
    __u8 err;
    esKRNL_SemPend(subDecCtrl->frameBufSem, 500, &err);
    if (err == 0)
        return 0;
    else
        return -1;
}

#endif

