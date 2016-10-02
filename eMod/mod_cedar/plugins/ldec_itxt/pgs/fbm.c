
#include "pgs_s.h"
#include "fbm.h"

static void display_enqueue(frm_info_t* pFrm, dsp_queue_t* dspQ)
{
	if(dspQ->frmNum < dspQ->maxFrmNum)
	{
    	dspQ->frmId[dspQ->writeIdx] = pFrm;

    	dspQ->writeIdx++;
    	if (dspQ->writeIdx >= dspQ->maxFrmNum)
        	dspQ->writeIdx = 0;

    	dspQ->frmNum++;
	}
    else
    {
        WARNING("fatal error! display inqueue fail![%d][%d]\n", dspQ->frmNum, dspQ->maxFrmNum);
    }
}

static frm_info_t* display_dequeue(dsp_queue_t* dspQ)
{
	frm_info_t* tmp;

	if(dspQ->frmNum > 0)
	{
		tmp = dspQ->frmId[dspQ->readIdx];
		dspQ->frmId[dspQ->readIdx] = NULL;

		dspQ->readIdx++;
		if(dspQ->readIdx >= dspQ->maxFrmNum)
			dspQ->readIdx = 0;

		dspQ->frmNum--;
		return tmp;
	}
	else
	{
        WARNING("fatal error! display dequeue fail, [%d]\n", dspQ->frmNum);
		return NULL;
	}
}

/******************************************************************************************************
*                       FBM_Create
*
*Description: create a frame buffer manager;
*
*Arguments  : width         picture width;
*             height        picture height;
*             widthStep     byte count of one line data;
*             maxFrmNum     the max frame count in the frame buffer manager;实帧
*
*Return     : pointer to frame buffer manager;
*             != 0    create success;
*             == 0    create failed, if momory is not enough, then VBV_Create() will fail.
******************************************************************************************************/
fbm_t* fbm_create(__u32 frmBufSize, __u32 maxFrmNum)
{
	__s32	i;
	fbm_t*  fbm;

	if(maxFrmNum > FRAME_BUFFER_NUM/2)      //maxFrmNum是指实帧的数量
	{
        WARNING("exceed frame buffer number [%d]\n", maxFrmNum);
		return NULL;
	}

	fbm = (fbm_t*) malloc(sizeof(fbm_t));
	if(!fbm)
		return NULL;
	memset(fbm, 0, sizeof(fbm_t));

    fbm->maxFrmNum = maxFrmNum;
    fbm->frame_buffer_size = frmBufSize;    //装载实帧的buffer的长度

    for (i=0; i<(__s32)maxFrmNum; i++)
    {
		fbm->frmInfo[i].dataLen = frmBufSize;
        //fbm->frmInfo[i].data	= (__u8*)palloc(frmBufSize>>10, 0);
        fbm->frmInfo[i].data	= (__u8*)PHY_MALLOC(frmBufSize, 1024);
        if(!fbm->frmInfo[i].data)
        {
            fbm_destroy((fbm_t**)&fbm);
            return NULL;
        }
    }

    fbm->dsp_queue.maxFrmNum = fbm->maxFrmNum*2;  //乘以2是因为一个实帧配一个空帧,所以显示帧队列的个数是实帧个数的2倍

    return (fbm_t*)fbm;
}


void fbm_reset(fbm_t* fbm, __u8 bReserveDspFrm)
{
    __u32			i;
    dsp_queue_t*	dspQ;

    dspQ = &fbm->dsp_queue;

	DisableSwitch();
    dspQ->readIdx = dspQ->writeIdx = dspQ->frmNum = 0;

    for(i=0; i<fbm->maxFrmNum; i++)
    {
        fbm->frmInfo[i].usedByDecoder = 0;
        fbm->frmInfo[i].inQueue = 0;
        if(bReserveDspFrm == 0)
        {
            fbm->frmInfo[i].usedByDsp = 0;
        }
        fbm->emptyFrameArray[i].usedByDecoder = 0;
        fbm->emptyFrameArray[i].inQueue = 0;
        if(bReserveDspFrm == 0)
        {
            fbm->emptyFrameArray[i].usedByDsp = 0;
        }        
    }
	EnableSwitch();
}

/******************************************************************************************************
*                       FBM_Destroy
*
*Description: destroy a frame buffer manager and release memory;
*
*Arguments  : ppfbm         pionter to a frame buffer manager pointer;
*
*Return     : destroy result, always return FBM_RESULT_OK.
******************************************************************************************************/

__s32 fbm_destroy(fbm_t **ppfbm)
{
	fbm_t*	fbm;
	__s32   i;

	if(!ppfbm)
		return -1;

	fbm = (fbm_t*)(*ppfbm);
	if (!fbm)
		return -1;

    for(i=0; i<(__s32)fbm->maxFrmNum; i++)
    {
        if(fbm->frmInfo[i].data)
        {
            //pfree(fbm->frmInfo[i].data, fbm->frame_buffer_size>>10);
            PHY_FREE(fbm->frmInfo[i].data);
            fbm->frmInfo[i].data = NULL;
        }
    }

    free(fbm);
    *ppfbm = NULL;
    return 0;
}


/******************************************************************************************************
*                       FBM_Decoder_Request_Frame
*
*Description: Decoder request one frame buffer from frame buffer manager;
*
*Arguments  : ppframe       output param, return the address of a __FRM_INFO struct;
*             fbm           pointer to a frame buffer manager;
*
*Return     : request result;
*             == FBM_RESULT_ERR_POINTER   ppvbv or fbm is invalid;
*             == FBM_RESULT_OK            request successful.
*             == FBM_RESULT_NO_FREE_FRAME no free frame buffer
*             == FBM_RESULT_FAILED        failed, maybe the fbm has no frame buffers.
******************************************************************************************************/
__s32 fbm_decoder_request_frm(frm_info_t **ppFrm, fbm_t* fbm)
{
	__u32   i;
	__s32   retVal;

	if(!ppFrm || !fbm)
		return -1;

	DisableSwitch();
	for(i=0; i<fbm->maxFrmNum; i++)
	{
		if( fbm->frmInfo[i].usedByDecoder == 0 &&
			fbm->frmInfo[i].usedByDsp == 0 &&
			fbm->frmInfo[i].inQueue == 0)
		{
			break;
		}
	}

	if(i == fbm->maxFrmNum)
		retVal = -1;
	else
	{
		fbm->frmInfo[i].usedByDecoder = 1;
    	*ppFrm = &fbm->frmInfo[i];
    	retVal = 0;
	}

	EnableSwitch();

	return retVal;
}

/*******************************************************************************
Function name: fbm_decoder_request_empty_frm
Description: 
    1.发现pgs的一个实帧结束,准备插空帧.所以先申请空帧
Parameters: 
    
Return: 
    -1表示没有申请到.
Time: 2011/10/15
*******************************************************************************/
__s32 fbm_decoder_request_empty_frm(frm_info_t **ppFrm, fbm_t* fbm)
{
	__u32   i;
	__s32   retVal;

	if(!ppFrm || !fbm)
		return -1;

	DisableSwitch();
	for(i=0; i<fbm->maxFrmNum; i++)
	{
		if( fbm->emptyFrameArray[i].usedByDecoder == 0 &&
			fbm->emptyFrameArray[i].usedByDsp == 0 &&
			fbm->emptyFrameArray[i].inQueue == 0)
		{
			break;
		}
	}

	if(i == fbm->maxFrmNum)
		retVal = -1;
	else
	{
		fbm->emptyFrameArray[i].usedByDecoder = 1;
    	*ppFrm = &fbm->emptyFrameArray[i];
    	retVal = 0;
	}

	EnableSwitch();

	return retVal;
}

/********************************************************************************************************
*                       FBM_Decoder_Return_Invalid_Frame
*
*Description: Decoder return a frame buffer, and do not add it to the display fifo;
*
*Arguments  : ppframe       pionter to address of a __FRM_INFO struct you want to return, after
*                           successully returned, *ppframe will be set to zero;
*             fbm           pointer to a frame buffer manager;
*
*Return     : request result;
*             == FBM_RESULT_ERR_POINTER   ppvbv or fbm is invalid;
*             == FBM_RESULT_OK            return successful;
*             == FBM_RESULT_FAILED        failed, this buffer is not belong to the frame buffer manager.
********************************************************************************************************/
__s32 fbm_decoder_return_invalid_frm(frm_info_t **ppFrm, fbm_t* fbm)
{
	frm_info_t* frm;

	if(!ppFrm)
		return -1;

	frm = *ppFrm;
	if(!frm)
		return -1;

	DisableSwitch();
	frm->usedByDecoder = 0;
	*ppFrm = NULL;
	EnableSwitch();

	return 0;
}


/********************************************************************************************************
*                       FBM_Decoder_Return_Valid_Frame
*
*Description: Decoder return a frame buffer, and add it to the display fifo;
*
*Arguments  : ppframe       pionter to address of a __FRM_INFO struct you want to return, after
*                           successully returned, *ppframe will be set to zero;
*             fbm           pointer to a frame buffer manager;
*
*Return     : request result;
*             == FBM_RESULT_ERR_POINTER    ppvbv or fbm is invalid;
*             == FBM_RESULT_OK             return successful;
*             == FBM_RESULT_FAILED         failed, this buffer is not belong to the frame buffer manager;
*             == FBM_RESULT_DSP_QUEUE_FULL the display frame fifo is full.
********************************************************************************************************/
__s32 fbm_decoder_return_valid_frm(frm_info_t** ppFrm, fbm_t* fbm)
{
	frm_info_t* frm;

	if(!ppFrm || !fbm)
		return -1;

	frm = *ppFrm;
	if(!frm)
		return -1;

	DisableSwitch();
	frm->usedByDecoder = 0;
	if(frm->usedByDsp == 0 && frm->inQueue == 0)
	{
		display_enqueue(frm, &fbm->dsp_queue);
		frm->inQueue = 1;
	}
	*ppFrm = NULL;
	EnableSwitch();

	return 0;
}


/********************************************************************************************************
*                       FBM_Decoder_Share_Frame
*
*Description: add a frame to the display fifo, and decoder will still use it as refference frames;
*
*Arguments  : pframe        pionter to a __FRM_INFO struct you want to share with frame buffer manager;
*             fbm           pointer to a frame buffer manager;
*
*Return     : request result;
*             == FBM_RESULT_ERR_POINTER    pvbv or fbm is invalid;
*             == FBM_RESULT_OK             return successful;
*             == FBM_RESULT_FAILED         failed, this buffer is not belong to the frame buffer manager;
*             == FBM_RESULT_DSP_QUEUE_FULL the display frame fifo is full.
********************************************************************************************************/
__s32 fbm_decoder_share_frm(frm_info_t* pFrm, fbm_t* fbm)
{
    if(!fbm || !pFrm)
    	return -1;

	DisableSwitch();
    display_enqueue(pFrm, &fbm->dsp_queue);
    pFrm->inQueue = 1;
	EnableSwitch();

    return 0;
}


/******************************************************************************************************
*                       FBM_Dsp_Request_Frame
*
*Description: diplayer request one frame buffer from frame buffer manager's display fifo;
*
*Arguments  : ppframe       output param, return the address of a __FRM_INFO struct;
*             fbm           pointer to a frame buffer manager;
*             bflag         first frame flag;
*
*Return     : request result;
*             == FBM_RESULT_ERR_POINTER     ppvbv or fbm is invalid;
*             == FBM_RESULT_OK              request successful;
*             == FBM_RESULT_DSP_QUEUE_EMPTY no frame to show, the display fifo is empty;
*             == FBM_RESULT_FAILED          failed, display fifo error.
******************************************************************************************************/

__s32 fbm_dsp_request_frm(frm_info_t** ppFrm, fbm_t* fbm)
{
	frm_info_t*  pFrm;
	dsp_queue_t* dspQ;

	if(!ppFrm || !fbm)
		return -1;

	dspQ = &fbm->dsp_queue;

	DisableSwitch();
	pFrm = display_dequeue(dspQ);
	if(pFrm)
	{
		pFrm->inQueue   = 0;
		pFrm->usedByDsp = 1;
	}
	EnableSwitch();

	*ppFrm = pFrm;

	return (pFrm == NULL) ? -1 : 0;
}


/******************************************************************************************************
*                       FBM_Dsp_Return_Frame
*
*Description: diplayer return one frame buffer to frame buffer manager;
*
*Arguments  : ppframe       pionter to address of a __FRM_INFO struct you want to return, after
*                           successully returned, *ppframe will be set to zero;
*             fbm           pointer to a frame buffer manager;
*
*Return     : request result;
*             == FBM_RESULT_ERR_POINTER     ppvbv or fbm is invalid;
*             == FBM_RESULT_OK              request successful.
******************************************************************************************************/
__s32 fbm_dsp_return_frm(frm_info_t* pFrm, fbm_t* fbm)
{
	if(!pFrm || !fbm)
		return -1;

	pFrm->usedByDsp = 0;

	return 0;
}


__s32 fbm_dsp_get_show_time(__u32 *pts, fbm_t* fbm)
{
	dsp_queue_t* dspQ;

	dspQ = &fbm->dsp_queue;

	if(dspQ->frmNum > 0)
	{
	    *pts = dspQ->frmId[dspQ->readIdx]->uPTS;
	    return 0;
	}
	else
	{
	    *pts = 0;
	    return -1;
	}

}

frm_info_t* fbm_index_to_ptr(__u8 idx, fbm_t* fbm)  //实帧的idx
{
    return &fbm->frmInfo[idx];
}

__u8 fbm_ptr_to_index(frm_info_t* ptr, fbm_t* fbm)  //实帧的ptr
{
    __u32 i;
    for(i=0; i<fbm->maxFrmNum; i++)
    {
        if(ptr == &fbm->frmInfo[i])
            break;
    }

    return i;
}

frm_info_t* fbm_empty_index_to_ptr(__u8 idx, fbm_t* fbm)  //空帧的idx
{
    return &fbm->emptyFrameArray[idx];
}

__u8 fbm_empty_ptr_to_index(frm_info_t* ptr, fbm_t* fbm)  //空帧的ptr
{
    __u32 i;
    for(i=0; i<fbm->maxFrmNum; i++)
    {
        if(ptr == &fbm->emptyFrameArray[i])
            break;
    }

    return i;
}

