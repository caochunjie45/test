/*
********************************************************************************
*                                    eMOD
*                   the Easy Portable/Player Develop Kits
*                               mod_mmp sub-system
*                          ldec_itxt decoder plug-in module
*
*          (c) Copyright 2009-2010, Allwinner Microelectronic Co., Ltd.
*                              All Rights Reserved
*
* File   : ldec_itxt.c
* Version: V1.0
* By     : Eric_wang
* Date   : 2009-6-24
********************************************************************************
*/

#include "ldec_itxt_s.h"
#include "ldec_itxt.h"
//#include "ldec_issa_i.h"



static LdecItxt            *ldec_itxt = NULL;

//extern  void ldec_itxt_init_info(void *p_arg);
//static void ldec_exit(void *p_arg);

//extern __s32 ldec_getItem_acordTime(__u32 getItemType, __s32 time);
//extern __s32 ldec_creat_mainTask(void *p_arg);
//extern __s32 ldec_stop_mainTask(void *p_arg);
//extern __s32 ldec_subtitle_decode(void *p_arg);
//extern __s32 ldec_isub_decode(__ldec_itxt_t *pdec, __cedar_subtitle_item_t *curSuItem,__u8 *curDataPtr, __u32 *usedBytes);
//extern __s32 ldec_issa_decode(__cedar_subtitle_item_t *psubitem, __u8 *pLbs, __u32 *usedBytes,__s8 lbsChgFlg);
//extern void  ldec_update_read_pointer(__ldec_lbs_buf_t *ldecLbsBuf, __u32 uread_byte);

void wakeupDecodeTask()
{
    OS_SEM_DATA     tmpSemDat;
    if(NULL == ldec_itxt->LbsInputSem || NULL == ldec_itxt->FrameBufSem)
    {
        WARNING("fatal error! lbsinputsem == NULL!\n");
    }
    esKRNL_SemQuery(ldec_itxt->LbsInputSem, &tmpSemDat);
    if(tmpSemDat.OSCnt == 0)
    {
        esKRNL_SemPost(ldec_itxt->LbsInputSem);
    }
    esKRNL_SemQuery(ldec_itxt->FrameBufSem, &tmpSemDat);
    if(tmpSemDat.OSCnt == 0)
    {
        esKRNL_SemPost(ldec_itxt->FrameBufSem);
    }
}

/*******************************************************************************
Function name: ldec_subtitle_decode
Description: 
    1.这时, pLyricDecoder已经创建,可以直接使用
Parameters: 
    
Return: 
    
Time: 2011/9/25
*******************************************************************************/
__s32 ldec_subtitle_decode(void *p_arg)
{
    __s32 ret = 0;
    __s32 nSyncStatFlag;
    CedarIsubtitleDec *pLyricDecoder = ldec_itxt->pCedarIsubtitleDecoder;
    //先判断是否重新reset了,一般流程转换快结束时,会收到PSR2LDEC_MSG_TYPE_SYNCSTAT,表示psr要ldec重新reset所有buffer,重新解码
    if(ldec_itxt->nResetLdecBufFlag)
    {
        WARNING("subtitledecode meet nResetLdecBufFlag[%d]\n", ldec_itxt->nResetLdecBufFlag);
        pLyricDecoder->CedarISubtitleDecIoCtrl(pLyricDecoder, CEDARISUBTITLEDEC_CMD_NOTIFY_SYNCSTAT, 0, NULL);
    }
    ret = pLyricDecoder->decode(pLyricDecoder);
    nSyncStatFlag = pLyricDecoder->CedarISubtitleDecIoCtrl(pLyricDecoder, CEDARISUBTITLEDEC_CMD_CHECK_SYNCSTAT, 0, NULL);
    if(0 == nSyncStatFlag)
    {
        ldec_itxt->nResetLdecBufFlag = 0;
    }
    else
    {
        WARNING("fatal error! why nSyncStatFlag[%d]\n", nSyncStatFlag);
    }
    return ret;
}

/*******************************************************************************
Function name: ldec_update_buf_pointer
Description: 
    这个地方要注意:
    现在,utf8, ssa, sub都没有标开始结束边界,但是pgs是需要边界标识的.
    utf8等就默认一笔数据就含有开始结束边界.
Parameters: 
    
Return: 
    0:没有足够的lbsbuf了.
    1:正常
Time: 2011/9/24
*******************************************************************************/
__s32 ldec_update_buf_pointer(__krnl_sktfrm_t     *frm)
{
    __s32   tmpret;
    CedarIsubtitleDec *pIsubtitleDec = ldec_itxt->pCedarIsubtitleDecoder;
    __lbs_datctlblk_t       *pdatctl = NULL;
    __psr_2_ldec_msg_t      *plbsmsg = NULL;
    //__u8                    writeIdx = 0;
    //__u32                   remainBufSize;
    __u8    *pLbsData;
    __s32   nLbsValidLen;
    __u8    *pData0 = NULL;
    __u32   uSizeGot0 = 0;
    __u8    *pData1 = NULL;
    __u32   uSizeGot1 = 0;
    if(frm->datalen > frm->datsize)
    {
        WARNING("fatal error! scket size is smaller than the data len.\n");
        frm->datalen = frm->datsize;
    }
    if(PSR2LDEC_MSG_TYPE_LBSCHUNK_INF == frm->msgtype)
    {
        plbsmsg = (__psr_2_ldec_msg_t*)frm->msg;
        pdatctl = &plbsmsg->lbs_dat_info;
    }
    else
    {
        WARNING("fatal error! lbsdatainfo = NULL\n");
        pdatctl = NULL;
    }
    pLbsData = (__u8*)frm->data;
    nLbsValidLen = frm->datalen;
    //现在有专门的buffer和psr传递数据,所以做一下出错判断
    if(pLbsData!=ldec_itxt->copyDataBuf)
    {
        WARNING("fatal error! [%x][%x]\n", pLbsData, ldec_itxt->copyDataBuf);
    }
    //if(0 == pdatctl->uDuration)
//    if(1)
//    {
//        WARNING("CtrlBits[0x%x], uPTS[%d], uPTS2[%d], uDuration[%d], uDatatype[%d], SubtitleType[%d], LbsChgFlg[%d], datalen[%d]\n", 
//            (__s32)pdatctl->CtrlBits, (__s32)pdatctl->uPTS, (__s32)pdatctl->uPTS2, (__s32)pdatctl->uDuration, 
//            pdatctl->uDatatype, pdatctl->SubtitleType, pdatctl->LbsChgFlg, nLbsValidLen);
//        WARNING("[%s]\n", pLbsData);
//    }
    //开始调用ItxtDec的接口完成数据输入
    tmpret = pIsubtitleDec->RequestWrite(pIsubtitleDec, nLbsValidLen, &pData0, &uSizeGot0, &pData1, &uSizeGot1);
    //remainBufSize = LDEC_MAX_TXT_BUF_SIZE - ldec_itxt->ldecLbsBuf.validSize;
    if(tmpret != EPDK_OK)    //lbsbuf不够,直接返回
    {
        return 0;
    }
    if(uSizeGot0)
    {
        memcpy(pData0, pLbsData, uSizeGot0);
    }
    if(uSizeGot1)
    {
        memcpy(pData1, pLbsData+uSizeGot0, uSizeGot1);
    }
    tmpret = pIsubtitleDec->UpdatePointer(pIsubtitleDec, pdatctl, nLbsValidLen);
    if(tmpret != EPDK_OK)
    {
        WARNING("fatal error! update pointer fail, [%x][%x]\n", tmpret, nLbsValidLen);
    }
    return 1;
}

/*******************************************************************************
Function name: ResetLdecItxtBuffer
Description: 
    1.把lbsbuffer和frame queue全部清0.
    2. 方法为:通知ldec_decode_task().然后等待其清0.
Parameters: 
    
Return: 
    
Time: 2011/9/25
*******************************************************************************/
__s32 ResetLdecItxtBuffer()
{
    if(0 != ldec_itxt->nResetLdecBufFlag)
    {
        WARNING("fatal error! nInitLdecBufFlag[%x]! check code!\n", ldec_itxt->nResetLdecBufFlag);
    }
    ldec_itxt->nResetLdecBufFlag = 1;
    //唤醒decode线程
    wakeupDecodeTask();
    while(1)
    {
        if(0 == ldec_itxt->nResetLdecBufFlag)
        {
            WARNING("ResetLdecBuf OK!\n");
            break;
        }
        else
        {
            WARNING("ResetLdecBuf Wait 50ms!\n");
            esKRNL_TimeDly(5);
            continue;
        }
    }
    return EPDK_OK;
}

void ldec_socket_maintsk(void *p_arg)
{
    __krnl_sktfrm_t         *frm = NULL;
    __psr_2_ldec_msg_t      *pmsg;
    __s8                    ret = 0;
    __subtitle_codec_format_t   *ptFormat = NULL;

    /*step 0: check stop_flag, request of deleting main task, and pend-post socket*/
    while(1)
    {
        if(ldec_itxt->stop_flag)
            goto _ldec_socket_maintsk_exit0;

        if(esKRNL_TDelReq(EXEC_prioself) == OS_TASK_DEL_REQ)
        {
            MESSAGE("delete lyric decoder main task!\n");
            goto _ldec_socket_maintsk_exit0;
        }

        frm = esKRNL_SktPend(ldec_itxt->pLSktin, KRNL_SKT_USR_OUT, 50);
        if(frm)
        {
            if(frm->msgtype == PSR2LDEC_MSG_TYPE_LBSINF)    //一般msgtype = PSR2LDEC_MSG_TYPE_LBSINF时,msg = __psr_2_ldec_msg_t,  dataype = CEDAR_DATATYPE_NONE, datalen = 0, 
            {
                //现在,msg就是__subtitle_codec_format_t**, mkv, mpg, ts都是这样
                WARNING("ldec socket maintsk pend PSR2LDEC_MSG_TYPE_LBSINF!\n");
                ptFormat = *(__subtitle_codec_format_t**)frm->msg;
                ldec_itxt->SubtitleCodecInfo.codec_type = ptFormat->codec_type;
                ldec_itxt->SubtitleCodecInfo.data_encode_type = ptFormat->data_encode_type;
                ldec_itxt->SubtitleCodecInfo.subtitle_bs_src = ptFormat->subtitle_bs_src;
                WARNING("## ptFormat->codec_type = [%d]; ptFormat->data_encode_type = [%d]; ptFormat->subtitle_bs_src = [%d]\n", 
                    ptFormat->codec_type, ptFormat->data_encode_type, ptFormat->subtitle_bs_src);
                ldec_itxt->pCedarIsubtitleDecoder->CedarISubtitleDecIoCtrl(ldec_itxt->pCedarIsubtitleDecoder, CEDARISUBTITLEDEC_CMD_SET_SUBTITLE_SOURCE, 
                    ldec_itxt->SubtitleCodecInfo.subtitle_bs_src, NULL);
                ldec_itxt->pCedarIsubtitleDecoder->CedarISubtitleDecIoCtrl(ldec_itxt->pCedarIsubtitleDecoder, CEDARISUBTITLEDEC_CMD_SET_CHAR_CODING_TYPE,
                    ldec_itxt->SubtitleCodecInfo.data_encode_type, NULL);
                break;
            }
            else
            {
                WARNING("ldecItxt Start: Not LbsInf, msgtype[%x], datatype[%x], post and wait!\n", frm->msgtype, frm->datatype);
                frm->data = 0;
                frm->datalen = 0;
                frm->datatype = CEDAR_DATATYPE_NONE;
                frm->datsize = 0;
                frm->msgtype = LDEC2LRDR_MSG_TYPE_NONE;
                esKRNL_SktPost(ldec_itxt->pLSktin, KRNL_SKT_USR_OUT, frm);
                frm = NULL;
            }
        }
    }

    do
    {
        if(frm->msgtype==PSR2LDEC_MSG_TYPE_SYNCSTAT)    //流程转换的同步.这时是已经转换完了,最后的握手阶段.
        {
//            while(1)
//            {
//                if(ldec_itxt->decFlag == 0)
//                    break;
//                esKRNL_TimeDly(1);
//            }
            //osal_switch_disable();
            ResetLdecItxtBuffer();
            //ldec_itxt_init_info((void*) 0);
            //osal_switch_enable();
            
            frm->msgtype = PSR2LDEC_MSG_TYPE_SYNCSTAT;
            esKRNL_SktPost(ldec_itxt->pLSktin, KRNL_SKT_USR_OUT, frm);
            frm = NULL;
            goto _ldec_socket_check;
        }
        else if(frm->msgtype==PSR2LDEC_MSG_TYPE_LBSCHUNK_INF)        //step 1.1 process msgType, 一般每笔数据的msgtype都是它,换字幕不在这里处理了.
        {
            pmsg = frm->msg;
            if(pmsg->lbs_dat_info.LbsChgFlg)
            {
//                while(1)
//                {
//                    if(ldec_itxt->decFlag == 0)
//                        break;
//                    esKRNL_TimeDly(1);
//                }
//                osal_switch_disable();
//                ldec_itxt_init_info((void*) 0);
//                osal_switch_enable();
                //pmsg->lbs_dat_info.LbsChgFlg = 0;
                WARNING("psr notify lbs change! curtype[%x]\n", ldec_itxt->subtitle_type);
            }
            if(pmsg->lbs_dat_info.CtrlBits&LDEC_SUBTYPE_VALID_CTRL)
            {
                //ldec_itxt->subtitle_type = pmsg->lbs_dat_info.SubtitleType;
                if(pmsg->lbs_dat_info.SubtitleType!=ldec_itxt->subtitle_type)
                {
                    WARNING("fatal error! subtitle type [%x]==[%x]\n", pmsg->lbs_dat_info.SubtitleType, ldec_itxt->subtitle_type);
                }
            }
//            else
//                ldec_itxt->subtitle_type = CEDAR_LRC_TYPE_ITXT;
        }

        if((frm->datatype!=CEDAR_DATATYPE_NONE) && (frm->datalen>0))
        {
            do
            {
                ret = ldec_update_buf_pointer(frm);
                if(!ret)    //如果没有足够的lbsbuf,等待.因为字幕解码实时性要求不高,所以简单的直接delay就可以了,不必设计一个等lbs释放的semaphore.
                {
                    if(ldec_itxt->stop_flag)
                        goto _ldec_socket_maintsk_exit0;
                    esKRNL_TimeDly(5);
                }
                else
                {
                    //wake up decode task, post lbs input的sema
                    OS_SEM_DATA     tmpSemDat;
                    esKRNL_SemQuery(ldec_itxt->LbsInputSem, &tmpSemDat);
                    if(tmpSemDat.OSCnt == 0)
                    {
                        esKRNL_SemPost(ldec_itxt->LbsInputSem);
                    }
                }
            }while(ret == 0);
        }

        //update data之后, 重新把copyDataBuf发送给psr. request data
        memset(ldec_itxt->copyDataBuf, 0, LDEC_MAX_TXT_COPY_SIZE);
        
        frm->data = ldec_itxt->copyDataBuf;
        frm->datsize = LDEC_MAX_TXT_COPY_SIZE;
        frm->datatype = CEDAR_DATATYPE_NONE;
        frm->datalen = 0;
        frm->msgtype = PSR2LDEC_MSG_TYPE_NONE;
        frm->msglen = 0;
        frm->msgsize = sizeof(__psr_2_ldec_msg_t);
        esKRNL_SktPost(ldec_itxt->pLSktin, KRNL_SKT_USR_OUT, frm);
        frm = NULL;

        _ldec_socket_check:
        while(1)
        {
            if(ldec_itxt->stop_flag)
                goto _ldec_socket_maintsk_exit0;

            if(esKRNL_TDelReq(EXEC_prioself) == OS_TASK_DEL_REQ)    //check if any request to delete main task
            {
                MESSAGE("delete lyric decoder main task!\n");
                goto _ldec_socket_maintsk_exit0;
            }
            frm = esKRNL_SktPend(ldec_itxt->pLSktin, KRNL_SKT_USR_OUT, 50);
            if(frm)
            {
                break;
            }
        }
    }while(1);

    _ldec_socket_maintsk_exit0:
    INFORMATION("--------------------------- ldec_itxt_maintsk end!\n");
    esKRNL_TDel(EXEC_prioself);
}

void ldec_subtitle_maintsk(void *p_arg)
{
    __s32   result;
    __u8    err;

    while(1)
    {
        if(ldec_itxt->stop_flag)
            goto _ldec_subtitle_maintsk_exit0;

        if(esKRNL_TDelReq(EXEC_prioself) == OS_TASK_DEL_REQ)
        {
            MESSAGE("delete lyric decoder main task!\n");
            goto _ldec_subtitle_maintsk_exit0;
        }

        result = ldec_subtitle_decode((void *)0);
        if(result == LDEC_ERROR_OK)
        {
            //WARNING("ldec subtitle successed!\n");
        }
        else if(result == LDEC_ERROR_FAIL)
        {
            WARNING("subtitle decoder result = LDEC_ERR_DEC_SUBTITLE, check!\n");
        }
        else if(result == LDEC_ERROR_LBS_UNDERFLOW)
        {
            //pending a semaphore to wait file parser input lbs data
            esKRNL_SemPend(ldec_itxt->LbsInputSem, 50, &err);
        }
        else if(result == LDEC_ERROR_NO_FRAMEBUFFER)
        {
            //pending a semaphore to wait file parser input lbs data
            esKRNL_SemPend(ldec_itxt->FrameBufSem, 50, &err);
        }
        else
        {
            WARNING("fatal error! subtitle decode result[%x]\n", result);
        }
    }

    _ldec_subtitle_maintsk_exit0:
    INFORMATION("--------------------------- ldec_itxt_maintsk end!\n");
    esKRNL_TDel(EXEC_prioself);
}


static __s32 ldec_creat_mainTask(void *p_arg)
{
    if(ldec_itxt->pLSktin == 0)
    {
        WARNING("socket for lyric input has not been set!\n");
        return EPDK_FAIL;
    }
    else
    {
        ldec_itxt->stop_flag = 0;
        ldec_itxt->socket_tsk_prio = esKRNL_TCreate(ldec_socket_maintsk, (void *)0, 0x2000,
                                                (EPOS_rsvid << 8) | LDEC_RECTASK_PRIO); //LDEC_RECTASK_PRIO, LPLY_MAINTASK_PRIO
                                                //(ldec_itxt->mid<< 8) | KRNL_priolevel3);
        if(!ldec_itxt->socket_tsk_prio)
        {
            __wrn("create ldec receive task failed!\n");
            return EPDK_FAIL;
        }
        ldec_itxt->dec_tsk_prio = esKRNL_TCreate(ldec_subtitle_maintsk, (void *)0, 0x2000,
                                                //(EPOS_rsvid << 8) | LDEC_MAINTASK_PRIO);    //可能会提高到KRNL_priolevel3,视pgs解码情况而定, LDEC_MAINTASK_PRIO
                                                (ldec_itxt->mid<< 8) | KRNL_priolevel3);
        if(!ldec_itxt->dec_tsk_prio)
        {
            __wrn("create ldec decode task failed!\n");
            return EPDK_FAIL;
        }
        return EPDK_OK;
    }
}

/*****************************************************************************************/
static __s32 ldec_stop_mainTask(void *p_arg)
{
    MESSAGE("Stop lyric decoder!\n");
    if(ldec_itxt->stop_flag == 0)
    {
        ldec_itxt->stop_flag = 1;

        if(ldec_itxt->socket_tsk_prio)
        {
            esKRNL_TimeDlyResume(ldec_itxt->socket_tsk_prio);
            while(esKRNL_TDelReq(ldec_itxt->socket_tsk_prio) != OS_TASK_NOT_EXIST)
            {
                esKRNL_TimeDlyResume(ldec_itxt->socket_tsk_prio);
                esKRNL_TimeDly(1);
            }
            ldec_itxt->socket_tsk_prio = 0;
        }
        if(ldec_itxt->dec_tsk_prio)
        {
            wakeupDecodeTask();
            esKRNL_TimeDlyResume(ldec_itxt->dec_tsk_prio);
            while(esKRNL_TDelReq(ldec_itxt->dec_tsk_prio) != OS_TASK_NOT_EXIST)
            {
                esKRNL_TimeDlyResume(ldec_itxt->dec_tsk_prio);
                esKRNL_TimeDly(1);
            }
            ldec_itxt->dec_tsk_prio = 0;
        }
    }
    ldec_itxt->pLSktin = 0;
    return EPDK_OK;
}

/*******************************************************************************
Function name: ldec_getItem_acordTime和ldec_getSubtitleFrame_acordTime
Description: 
    1.原则是: 如果有多条字幕的时间范围符合要求,取最后一条送出,其余的忽略掉.
    2. 默认的帧队列的规律是:
        按时间顺序递增.
    3.有隐患.GetLrcItem拿到的指针指向的__cedar_subtitle_item_t,有可能会被改写.所以要迅速copy到
        outputItem中.一般不会有事.所以不做互斥处理.
        pLyricDecoder还有可能被destroy.也没做互斥.

    4. ldec_getSubtitleFrame_acordTime是第二种要帧接口.给subtitle_plybk用的.
Parameters: 
    
Return: 
    字幕项__cedar_subtitle_item_t的指针.转为__s32
Time: 2011/6/14
*******************************************************************************/
static __s32 ldec_getItem_acordTime(__u32 getItemType, __s32 time)
{
    //__s32   tmpret;
    //__u8    idx = 0;
    //__s32   timeSmall = 0;
    //__s32   timeLarge = 0;
    //__s32   ret = EPDK_FAIL;
    //__s32   nFindFlag = 0;  //标识是否找到合适的字幕了,如果多条字幕符合要求，取最后一条字幕。
    //__cedar_subtitle_item_t *pItem = NULL;
    OS_SEM_DATA     tmpSemDat;
    __cedar_subtitle_item_t *pOutputItem = NULL;
    __s32   nRedecodeFlag = 0;
    if(getItemType != CEDAR_GET_SUB_INF_ITEM)
    {
        //ret = EPDK_FAIL;
        return (__s32)NULL;
    }
    if(NULL == ldec_itxt->pCedarIsubtitleDecoder)
    {
        WARNING("error! pCedarIsubtitleDecoder = NULL\n");
        return (__s32)NULL;
    }
    if(ldec_itxt->nRequestFrameMode != ReqSubtitleFrameMode_AppReq)
    {
        WARNING("fatal error!, reqSubtitleFrameMode[0x%x] wrong, check code!\n", ldec_itxt->nRequestFrameMode);
        return (__s32)NULL;
    }
    pOutputItem = ldec_itxt->pCedarIsubtitleDecoder->GetLrcItem(ldec_itxt->pCedarIsubtitleDecoder, time, &nRedecodeFlag);
    if(nRedecodeFlag)
    {
        //wake up decode task
        esKRNL_SemQuery(ldec_itxt->FrameBufSem, &tmpSemDat);
        if(tmpSemDat.OSCnt == 0)
        {
            esKRNL_SemPost(ldec_itxt->FrameBufSem);
        }
    }
    return (__s32)pOutputItem;
}
static __s32 ldec_getSubtitleFrame_acordTime(__u32 getItemType, __s32 time)
{
    //__s32   tmpret;
    //__u8    idx = 0;
    //__s32   timeSmall = 0;
    //__s32   timeLarge = 0;
    //__s32   ret = EPDK_FAIL;
    //__s32   nFindFlag = 0;  //标识是否找到合适的字幕了,如果多条字幕符合要求，取最后一条字幕。
    //__cedar_subtitle_item_t *pItem = NULL;
    OS_SEM_DATA     tmpSemDat;
    CCedarSubtitleFrame *pOutputSubtitleFrame = NULL;
    __s32   nRedecodeFlag = 0;
    if(getItemType != CEDAR_GET_SUB_INF_ITEM)
    {
        //ret = EPDK_FAIL;
        return (__s32)NULL;
    }
    if(NULL == ldec_itxt->pCedarIsubtitleDecoder)
    {
        WARNING("error! pCedarIsubtitleDecoder = NULL\n");
        return (__s32)NULL;
    }
    if(ldec_itxt->nRequestFrameMode != ReqSubtitleFrameMode_SubtitleplybkReq)
    {
        WARNING("fatal error!, reqSubtitleFrameMode[0x%x] wrong, check code!\n", ldec_itxt->nRequestFrameMode);
        return (__s32)NULL;
    }
    pOutputSubtitleFrame = ldec_itxt->pCedarIsubtitleDecoder->GetSubtitleFrame(ldec_itxt->pCedarIsubtitleDecoder, time, &nRedecodeFlag);
    if(nRedecodeFlag)
    {
        //wake up decode task
        esKRNL_SemQuery(ldec_itxt->FrameBufSem, &tmpSemDat);
        if(tmpSemDat.OSCnt == 0)
        {
            esKRNL_SemPost(ldec_itxt->FrameBufSem);
        }
    }
    return (__s32)pOutputSubtitleFrame;
}



/*******************************************************************************
Function name: SetInitValue
Description: 
    1.在Init时，一些成员变量的初始值的设置,一些在初始化时已经确定的值不会在这个函数里赋值.
    在IO_Close()时也会调用
Parameters: 
    
Return: 
    
Time: 2011/10/7
*******************************************************************************/
__s32 SetLdecItxtInitValue(LdecItxt *thiz)
{
    thiz->stop_flag = 0;
    thiz->pLSktin = NULL;
    thiz->subtitle_type = CEDAR_LRC_TYPE_NONE;
    thiz->frm_rate = 0;
    thiz->width = 0;
    thiz->height = 0;
    thiz->screenWidth = 0;
    thiz->screenHeight = 0;
    thiz->nResetLdecBufFlag = 0;
    thiz->hAvs = NULL;
    thiz->nOneSegFlag = 0;
    thiz->IOOpenFlag = 0;
    thiz->nRequestFrameMode = ReqSubtitleFrameMode_AppReq;
    if(thiz->copyDataBuf)
    {
        WARNING("fatal error! copyDataBuf!=NULL\n");
    }
    if(thiz->LbsInputSem)
    {
        WARNING("fatal error! LbsInputSem!=NULL\n");
    }
    if(thiz->FrameBufSem)
    {
        WARNING("fatal error! FrameBufSem!=NULL\n");
    }
    if(thiz->pCedarIsubtitleDecoder)
    {
        WARNING("fatal error! pCedarIsubtitleDecoder!=NULL\n");
    }
    memset(&thiz->SubtitleCodecInfo, 0, sizeof(__subtitle_codec_format_t));
    return EPDK_OK;
}

__s32 Impl_LdecItxt_Open(LdecItxt *ldec_itxt)
{
    __u8    err;
    if(ldec_itxt->IOOpenFlag)
    {
        WARNING("ldec_itxt:IOOpenFlag[%d], already open, return!\n", ldec_itxt->IOOpenFlag);
        return EPDK_OK;
    }
    ldec_itxt->copyDataBuf = (__u8 *)PHY_MALLOC(sizeof(__u8)*LDEC_MAX_TXT_COPY_SIZE, 1024);
    if(ldec_itxt->copyDataBuf == NULL)
    {
        goto _err_mopen;
    }
    //create semaphore for wakeup lyric decoder
    ldec_itxt->LbsInputSem = esKRNL_SemCreate(0);
    if(!ldec_itxt->LbsInputSem)
    {
        WARNING("Create semaphore failed when open lyric decoder plug-in!\n");
        goto _err_mopen;
    }

    //create semaphore for wakeup lyric decoder
    ldec_itxt->FrameBufSem = esKRNL_SemCreate(0);
    if(!ldec_itxt->FrameBufSem)
    {
        WARNING("Create semaphore failed when open lyric decoder plug-in!\n");
        goto _err_mopen;
    }
    ldec_itxt->IOOpenFlag = 1;
    return EPDK_OK;

_err_mopen:
    if(ldec_itxt->LbsInputSem)
    {
        esKRNL_SemDel(ldec_itxt->LbsInputSem, OS_DEL_ALWAYS, &err);
        ldec_itxt->LbsInputSem = 0;
    }

    if(ldec_itxt->FrameBufSem)
    {
        esKRNL_SemDel(ldec_itxt->FrameBufSem, OS_DEL_ALWAYS, &err);
        ldec_itxt->FrameBufSem = 0;
    }
    if(ldec_itxt->copyDataBuf)
    {
        PHY_FREE(ldec_itxt->copyDataBuf);
        ldec_itxt->copyDataBuf = NULL;
    }
    return EPDK_FAIL;
}
__s32 Impl_LdecItxt_Close(LdecItxt *thiz)
{
    __u8    err;
    if(0 == thiz->IOOpenFlag)
    {
        WARNING("IOOpenFlag[%d], already close, return!\n", thiz->IOOpenFlag);
        return EPDK_OK;
    }
    if(thiz->pCedarIsubtitleDecoder)
    {
        WARNING("why pCedarIsubtitleDecoder is not delete?\n");
        CedarIsubtitleDec_Destroy(thiz->pCedarIsubtitleDecoder);
        thiz->pCedarIsubtitleDecoder = NULL;
    }
    if(thiz->LbsInputSem)
    {
        esKRNL_SemDel(thiz->LbsInputSem, OS_DEL_ALWAYS, &err);
        thiz->LbsInputSem = 0;
    }

    if(thiz->FrameBufSem)
    {
        esKRNL_SemDel(thiz->FrameBufSem, OS_DEL_ALWAYS, &err);
        thiz->FrameBufSem = 0;
    }
    if(thiz->copyDataBuf)
    {
        PHY_FREE(thiz->copyDataBuf);
        thiz->copyDataBuf = NULL;
    }
    SetLdecItxtInitValue(thiz);
    thiz->IOOpenFlag = 0;
    return EPDK_OK;
}

__s32 LdecItxt_Init(LdecItxt *ldec_itxt, __u32 mid)
{
    memset(ldec_itxt, 0, sizeof(LdecItxt));
    //create socket for transfer lyric item
    ldec_itxt->pLSktout = NULL;
    ldec_itxt->frm_rate = 0;
    ldec_itxt->width = 0;
    ldec_itxt->height = 0;
    //ldec_itxt->outputItem.uPts = 0;
    //ldec_itxt->outputItem.uDuration = 0;
    ldec_itxt->mid = mid;

    ldec_itxt->open = Impl_LdecItxt_Open;
    ldec_itxt->close = Impl_LdecItxt_Close;
    SetLdecItxtInitValue(ldec_itxt);
    return EPDK_OK;
}

__s32 LdecItxt_Exit(LdecItxt *thiz)
{
    return EPDK_OK;
}

LdecItxt* newLdecItxt(__u32 mid)
{
    __s32   tmpret;
    LdecItxt *pLdecItxt;
    //create lyric decoder handle
    pLdecItxt = (LdecItxt *)malloc(sizeof(LdecItxt));
    if(!pLdecItxt)
    {
        WARNING("Create lyric decoder handle failed!\n");
        goto _err0_mopen;
    }
    //初始化
    tmpret = LdecItxt_Init(pLdecItxt, mid);
    if(tmpret!=EPDK_OK)
    {
        goto _err1_mopen;
    }
    return pLdecItxt;

_err1_mopen:
    free(pLdecItxt);
    pLdecItxt = NULL;
_err0_mopen:
    return NULL;
}
void deleteLdecItxt(LdecItxt* thiz)
{
    //exit
    LdecItxt_Exit(thiz);
    free(thiz);
}

/*
*********************************************************************************************************
*                               LYRIC DECODER MODULE INIT
*
*Description: lyric decoder module init.
*
*Arguments  : None.
*
*Return     : Always return successful.
*********************************************************************************************************
*/
__s32 MInit(void)
{
    MESSAGE("==ldec_itxt==MInit!\n");
    ldec_itxt = (LdecItxt *)0;
    return EPDK_OK;
}

/*
*********************************************************************************************************
*                               LYRIC DECODER MODULE EXIT
*
*Description: lyric decoder module exit.
*
*Arguments  : None.
*
*Return     : Always return successful.
*********************************************************************************************************
*/
__s32 MExit(void)
{
    MESSAGE("==ldec_itxt==MExit!\n");
    return EPDK_OK;
}


/*
*********************************************************************************************************
*                               LYRIC DECODER MODULE OPEN
*
*Description: lyric decoder module open.
*
*Arguments  : mid       ID of lyric decoder module;
*             mode      the type of open.
*
*Return     : the handle of the lyric decoder.
*                   != 0    open lyric decoder module successful;
*                   == 0    open lyric decoder module fialed.
*********************************************************************************************************
*/
__mp *MOpen(__u32 mid, __u32 mode)
{
    //__u8    i = 0;
    //__s32   tmpret;
    //check if lyric decoder has been opened already, it can only be opened one time
    if(ldec_itxt != (LdecItxt*)0)
    {
        WARNING("Lyric decoder has been opened by some one already!\n");
        return 0;
    }
    //initiate medar heap and physic heap
    CEDAR_HEAPINIT();
    CEDAR_PHEAPINIT();

    ldec_itxt = newLdecItxt(mid);
    if(!ldec_itxt)
    {
        WARNING("Ldec Itxt malloc fail!\n");
        goto _err_mopen;
    }
    return (__mp*)ldec_itxt;
    
_err_mopen:
    //ldec_exit((void*)0);
    CEDAR_HEAPEXIT();
    CEDAR_PHEAPEXIT();
    
    return (__mp *)0;
}

/*
*********************************************************************************************************
*                               LYRIC DECODER MODULE CLOSE
*
*Description: lyric decoder module close.
*
*Arguments  : mp    the handle of lyric decoder module;
*
*Return     : Always return successful.
*********************************************************************************************************
*/
__s32 MClose(__mp *mp)
{
    //0. check if the lyric decoder has been closed already
    if(ldec_itxt == NULL)
    {
        WARNING("lyric decoder has been closed already!\n");
        return EPDK_FAIL;
    }
    ldec_itxt->close(ldec_itxt);
    deleteLdecItxt(ldec_itxt);
    ldec_itxt = NULL;
    CEDAR_HEAPEXIT();
    CEDAR_PHEAPEXIT();

    return EPDK_OK;
}

/*
*********************************************************************************************************
*                               LYRIC DECODER MODULE READ
*
*Description: lyric deocder module read, do nothing.
*
*Arguments  :
*
*Return     :
*********************************************************************************************************
*/
__u32 MRead(void *pdata, __u32 size, __u32 n, __mp *mp)
{
    return n;
}


/*
*********************************************************************************************************
*                               LYRIC DECODER MODULE WRITE
*
*Description: lyric decoder module write, do nothing.
*
*Arguments  :
*
*Return     :
*********************************************************************************************************
*/
__u32 MWrite(const void *pdata, __u32 size, __u32 n, __mp *mp)
{
    return n;
}


/*
*********************************************************************************************************
*                               LYRIC DECODER MODULE IO CONTROL
*
*Description: lyric decoder module IO control, process command from other module.
*
*Arguments  : mp        lyric decoder handle;
*             cmd       the command need send to lyric decoder;
*             aux       user defined;
*             pbuffer   user defined.
*
*Return     :
*********************************************************************************************************
*/
__s32 MIoctrl(__mp *mp, __u32 cmd, __s32 aux, void *pbuffer)
{
    __s32 ret = EPDK_OK;

    switch(cmd)
    {
        case CEDAR_LDEC_CMD_CHKSPT:
        {
            ret = (aux==CEDAR_LRC_TYPE_ITXT) || (aux==CEDAR_LRC_TYPE_ISSA) ||(aux==CEDAR_LRC_TYPE_ISUB) || (aux==CEDAR_LRC_TYPE_PGS) || (aux==CEDAR_LRC_TYPE_ISDB)||(aux==CEDAR_LRC_TYPE_DVBSUB);
            if(ldec_itxt->pCedarIsubtitleDecoder)
            {
                WARNING("Be careful! pLyricDecoder[%x]!= NULL\n", ldec_itxt->pCedarIsubtitleDecoder);
                ret = ldec_itxt->pCedarIsubtitleDecoder->CheckSupport(ldec_itxt->pCedarIsubtitleDecoder, aux);
            }
            break;
        }
        case CEDAR_LDEC_CMD_SET_LBSTYPE:
        {
            ldec_itxt->subtitle_type = aux;
            if(ldec_itxt->pCedarIsubtitleDecoder)
            {
                WARNING("fatal error! pLyricDecoder should be null!\n");
            }
            ret = EPDK_OK;
            break;
        }
        case CEDAR_LDEC_CMD_SET_LBSSKTIN:
            ldec_itxt->pLSktin = (__hdle)pbuffer;
            ret = EPDK_OK;
            break;
        case CEDAR_LDEC_CMD_SET_ReqSubtitleFrameMode:
        {
            ldec_itxt->nRequestFrameMode = (ReqSubtitleFrameMode)aux;
            ret = EPDK_OK;
            break;
        }
        case CEDAR_LDEC_CMD_GET_LRCITEM:
            ret = ldec_getItem_acordTime((__u32)pbuffer, aux);
            break;
        case CEDAR_LDEC_CMD_GET_SUBTITLEFRAME:
        {
            ret = ldec_getSubtitleFrame_acordTime((__u32)pbuffer, aux);
            break;
        }
        case CEDAR_LDEC_CMD_OPEN:
        {
            ret = ldec_itxt->open(ldec_itxt);
            break;
        }
        case CEDAR_LDEC_CMD_CLOSE:
        {
            ret = ldec_itxt->close(ldec_itxt);
            break;
        }
        case CEDAR_LDEC_CMD_START:
        {
            __s32   tmpret;
            if(ldec_itxt->pCedarIsubtitleDecoder)
            {
                WARNING("fatal error! pCedarIsubtitleDecoder[%x]!=NULL\n", ldec_itxt->pCedarIsubtitleDecoder);
                ret = EPDK_FAIL;
                goto _start_err0;
            }
            WARNING("## ldec_itxt->subtitle_type = [%d]\n", ldec_itxt->subtitle_type);
            ldec_itxt->pCedarIsubtitleDecoder = CedarIsubtitleDec_Create(ldec_itxt->subtitle_type);
            if(NULL == ldec_itxt->pCedarIsubtitleDecoder)
            {
                WARNING("create pCedarIsubtitleDecoder fail!\n");
                ret = EPDK_FAIL;
                goto _start_err0;
            }
            ldec_itxt->pCedarIsubtitleDecoder->CedarISubtitleDecIoCtrl(ldec_itxt->pCedarIsubtitleDecoder, CEDARISUBTITLEDEC_CMD_SET_FRMRATE, 
                    ldec_itxt->frm_rate, NULL);
            ldec_itxt->pCedarIsubtitleDecoder->CedarISubtitleDecIoCtrl(ldec_itxt->pCedarIsubtitleDecoder, CEDARISUBTITLEDEC_CMD_SET_FRMSIZE, 
                    ldec_itxt->width<<16 | ldec_itxt->height, NULL);
            ldec_itxt->pCedarIsubtitleDecoder->CedarISubtitleDecIoCtrl(ldec_itxt->pCedarIsubtitleDecoder, CEDARISUBTITLEDEC_CMD_SET_SCREEN_SIZE, 
                    ldec_itxt->screenWidth<<16|ldec_itxt->screenHeight, NULL);
            ldec_itxt->pCedarIsubtitleDecoder->CedarISubtitleDecIoCtrl(ldec_itxt->pCedarIsubtitleDecoder, CEDARISUBTITLEDEC_CMD_SET_ONESEGFLAG, 
                    ldec_itxt->nOneSegFlag, NULL);
            tmpret = ldec_itxt->pCedarIsubtitleDecoder->open(ldec_itxt->pCedarIsubtitleDecoder);
            if(tmpret != LDEC_ERR_NONE)
            {
                WARNING("pCedarIsubtitleDecoder open fail![%x]\n", tmpret);
                ret = EPDK_FAIL;
                goto _start_err1;
            }
            ret = ldec_creat_mainTask((void*)0);
            if(ret != EPDK_OK)
            {
                ret = EPDK_FAIL;
                goto _start_err2;
            }
            ret = EPDK_OK;
            break;
            
        _start_err2:
            ldec_itxt->pCedarIsubtitleDecoder->close(ldec_itxt->pCedarIsubtitleDecoder);
        _start_err1:
            CedarIsubtitleDec_Destroy(ldec_itxt->pCedarIsubtitleDecoder);
            ldec_itxt->pCedarIsubtitleDecoder = NULL;
        _start_err0:
            ret = EPDK_FAIL;
            break;
        }
        case CEDAR_LDEC_CMD_STOP:
        {
            __s32   tmpret;
            CedarIsubtitleDec *pIsubtitleDec = ldec_itxt->pCedarIsubtitleDecoder;
            ret = ldec_stop_mainTask((void*)0);
            if(ldec_itxt->pCedarIsubtitleDecoder)
            {
                tmpret = ldec_itxt->pCedarIsubtitleDecoder->close(ldec_itxt->pCedarIsubtitleDecoder);
                if(tmpret != LDEC_ERR_NONE)
                {
                    WARNING("pCedarIsubtitleDecoder close fail![%x]\n", tmpret);
                }
                ldec_itxt->pCedarIsubtitleDecoder = NULL;    //因为要防止CEDAR_LDEC_CMD_GET_LRCITEM引用空指针.所以先迅速置null.
                CedarIsubtitleDec_Destroy(pIsubtitleDec);
            }
            else
            {
                WARNING("fatal error when LDECITXT stop, ldec_itxt->pLyricDecoder = null\n");
            }
            
            break;
        }
        case CEDAR_LDEC_CMD_SET_VIDFRMRATE:
        {
            ldec_itxt->frm_rate = aux;
            if(ldec_itxt->pCedarIsubtitleDecoder)
            {
                WARNING("fatal error! pCedarIsubtitleDecoder should be null!\n");
                ldec_itxt->pCedarIsubtitleDecoder->CedarISubtitleDecIoCtrl(ldec_itxt->pCedarIsubtitleDecoder, CEDARISUBTITLEDEC_CMD_SET_FRMRATE, aux, NULL);
            }
            ret = EPDK_OK;
            break;
        }
        case CEDAR_LDEC_CMD_SET_VIDFRMSIZE:
            ldec_itxt->width = (aux>>16);
            ldec_itxt->height = (aux&0xffff);
            if(ldec_itxt->pCedarIsubtitleDecoder)
            {
                WARNING("fatal error! pCedarIsubtitleDecoder should be null!\n");
                ldec_itxt->pCedarIsubtitleDecoder->CedarISubtitleDecIoCtrl(ldec_itxt->pCedarIsubtitleDecoder, CEDARISUBTITLEDEC_CMD_SET_FRMSIZE, aux, NULL);
            }
            return EPDK_OK;

        case CEDAR_LDEC_CMD_SET_SCREEN_SIZE:
        {
            ldec_itxt->screenWidth  = aux>>16;
            ldec_itxt->screenHeight = aux & 0xffff;
            if(ldec_itxt->pCedarIsubtitleDecoder)
            {
                WARNING("fatal error! pCedarIsubtitleDecoder should be null!\n");
                ldec_itxt->pCedarIsubtitleDecoder->CedarISubtitleDecIoCtrl(ldec_itxt->pCedarIsubtitleDecoder, CEDARISUBTITLEDEC_CMD_SET_SCREEN_SIZE, aux, NULL);
            }
            return EPDK_OK;
        }
        case CEDAR_LDEC_CMD_SET_ONESEGFLAG:
        {
            ldec_itxt->nOneSegFlag = aux;
            if(ldec_itxt->pCedarIsubtitleDecoder)
            {
                ldec_itxt->pCedarIsubtitleDecoder->CedarISubtitleDecIoCtrl(ldec_itxt->pCedarIsubtitleDecoder, CEDARISUBTITLEDEC_CMD_SET_ONESEGFLAG, ldec_itxt->nOneSegFlag, NULL);
            }
            return EPDK_OK;
        }
        default:
            ret = EPDK_FAIL;
            break;
    }
    return ret;
}

#if 0 //20110614封存
static void ldec_itxt_init_info(void * p_arg)
{
    __u8    i = 0;

    memset(ldec_itxt->anciDataBuf, 0, LDEC_MAX_TXT_ANCI_SIZE);
    memset(ldec_itxt->ldecLbsBuf.ldecBuf, 0, LDEC_MAX_TXT_BUF_SIZE);
   
    ldec_itxt->outputItem.uPts = 0;
    ldec_itxt->outputItem.uDuration = 0;
    memset(ldec_itxt->outputItem.pSubtitle, 0, LDEC_MAX_TXT_FRM_SIZE);
    
    ldec_itxt->ldecLbsBuf.readAddr = ldec_itxt->ldecLbsBuf.ldecBuf;
    ldec_itxt->ldecLbsBuf.writeAddr = ldec_itxt->ldecLbsBuf.ldecBuf;
    ldec_itxt->ldecLbsBuf.ldecIdxSize = 0;
    ldec_itxt->ldecLbsBuf.ldecWriteIdx = 0;
    ldec_itxt->ldecLbsBuf.ldecReadIdx  = 0;
    ldec_itxt->ldecLbsBuf.validSize   = 0;
    ldec_itxt->ldecLbsBuf.ldecIdxSize = 0;

    ldec_itxt->ldecManager.validSize = 0;
    ldec_itxt->ldecManager.reconBufIdx = 0;
    ldec_itxt->ldecManager.subReadIdx = 0;
    ldec_itxt->ldecManager.subWriteIdx = 0;

    ldec_itxt->decFlag = 0;
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

static __s32 ldec_getItem_acordTime0(__u32 getItemType, __s32 time)
{
    __u8    idx = 0;
    __s32   timeSmall = 0;
    __s32   timeLarge = 0;
    __s32   ret = EPDK_FAIL;
    __s32   nFindFlag = 0;  //标识是否找到合适的字幕了,如果多条字幕符合要求，取最后一条字幕。

    if(getItemType != CEDAR_GET_SUB_INF_ITEM)
    {
        ret = EPDK_FAIL;
    }
    else
    {
        osal_switch_disable();
        while(ldec_itxt->ldecManager.validSize > 0)
        {
            idx = ldec_itxt->ldecManager.subReadIdx;
            idx = ldec_itxt->ldecManager.subIdxBuf[idx];
            timeSmall = ldec_itxt->ldecManager.subItem[idx].uPts;
            timeLarge = ldec_itxt->ldecManager.subItem[idx].uPts + ldec_itxt->ldecManager.subItem[idx].uDuration;
            if(time < timeSmall)
            {
                if(nFindFlag)
                {
                    if(ldec_itxt->ldecManager.subReadIdx == 0)
                    {
                       ldec_itxt->ldecManager.subReadIdx = LDEC_MAX_TXT_FRM_NUM;
                    }
                    ldec_itxt->ldecManager.validSize++;
                    ldec_itxt->ldecManager.subReadIdx--;
                    idx = ldec_itxt->ldecManager.subReadIdx;
                    idx = ldec_itxt->ldecManager.subIdxBuf[idx];
                    ldec_itxt->ldecManager.subIsFull[idx] = 1;
                    ret = EPDK_OK;
                    break;
                }
                else
                {
                    ret = EPDK_FAIL;
                    break;
                }
            }
            else if((time>=timeSmall)&&(time<timeLarge))
            {
                //ldec_fullCopy_subItem(&ldec_itxt->outputItem,&ldec_itxt->ldecManager.subItem[idx]);
                if(nFindFlag)
                {
                    WARNING("find item again[%d][%d][%d], continue\n", time, timeSmall, timeLarge);
                }
                ret = EPDK_OK;
                nFindFlag = 1;
                if(1 == ldec_itxt->ldecManager.validSize)   //最后一条有效字幕了,就不继续找下去.
                {
                    break;
                }
                //break;
            }
            else
            {
                if(nFindFlag)
                {
                    if(ldec_itxt->ldecManager.subReadIdx == 0)
                    {
                       ldec_itxt->ldecManager.subReadIdx = LDEC_MAX_TXT_FRM_NUM;
                    }
                    ldec_itxt->ldecManager.validSize++;
                    ldec_itxt->ldecManager.subReadIdx--;
                    idx = ldec_itxt->ldecManager.subReadIdx;
                    idx = ldec_itxt->ldecManager.subIdxBuf[idx];
                    ldec_itxt->ldecManager.subIsFull[idx] = 1;
                    ret = EPDK_OK;
                    break;
                }
            }

            //osal_switch_disable();
            ldec_itxt->ldecManager.subIsFull[idx] = 0;
            ldec_itxt->ldecManager.validSize --;
            ldec_itxt->ldecManager.subReadIdx ++;
            if(ldec_itxt->ldecManager.subReadIdx == LDEC_MAX_TXT_FRM_NUM)
               ldec_itxt->ldecManager.subReadIdx = 0;
            //osal_switch_enable();
        }
        if(nFindFlag)
        {
            ldec_fullCopy_subItem(&ldec_itxt->outputItem,&ldec_itxt->ldecManager.subItem[idx]);
            ret = EPDK_OK;
        }
        osal_switch_enable();
    }

    if(ret == EPDK_OK)
    {
        OS_SEM_DATA     tmpSemDat;

        if(ldec_itxt->outputItem.pSubtitle != NULL)
            ldec_itxt->outputItem.pSubtitle[ldec_itxt->outputItem.uSubSize] = 0;

        //wake up decode task
        esKRNL_SemQuery(ldec_itxt->FrameBufSem, &tmpSemDat);
        if(tmpSemDat.OSCnt == 0)
        {
            esKRNL_SemPost(ldec_itxt->FrameBufSem);
        }

        return (__s32)(&ldec_itxt->outputItem);
    }
    else
    {
        //WARNING("cannot find the right subtitle\n");
        return (__s32)NULL;
    }
}

static __s32 ldec_getItem_acordTime0(__u32 getItemType, __s32 time)
{
    __u8    idx = 0;
    __s32   timeSmall = 0;
    __s32   timeLarge = 0;
    __s32   ret = EPDK_FAIL;

    if(getItemType != CEDAR_GET_SUB_INF_ITEM)
    {
        ret = EPDK_FAIL;
    }
    else
    {
        while(ldec_itxt->ldecManager.validSize > 0)
        {
            idx = ldec_itxt->ldecManager.subReadIdx;
            idx = ldec_itxt->ldecManager.subIdxBuf[idx];
            timeSmall = ldec_itxt->ldecManager.subItem[idx].uPts;
            timeLarge = ldec_itxt->ldecManager.subItem[idx].uPts + ldec_itxt->ldecManager.subItem[idx].uDuration;
            if(time < timeSmall)
            {
                ret = EPDK_FAIL;
                break;
            }
            else if((time>=timeSmall)&&(time<=timeLarge))
            {
                ldec_fullCopy_subItem(&ldec_itxt->outputItem,&ldec_itxt->ldecManager.subItem[idx]);
                ret = EPDK_OK;
                break;
            }

            osal_switch_disable();
            ldec_itxt->ldecManager.subIsFull[idx] = 0;
            ldec_itxt->ldecManager.validSize --;
            ldec_itxt->ldecManager.subReadIdx ++;
            if(ldec_itxt->ldecManager.subReadIdx == LDEC_MAX_TXT_FRM_NUM)
               ldec_itxt->ldecManager.subReadIdx = 0;
            osal_switch_enable();
        }
    }

    if(ret == EPDK_OK)
    {
        OS_SEM_DATA     tmpSemDat;

        if(ldec_itxt->outputItem.pSubtitle != NULL)
            ldec_itxt->outputItem.pSubtitle[ldec_itxt->outputItem.uSubSize] = 0;

        //wake up decode task
        esKRNL_SemQuery(ldec_itxt->FrameBufSem, &tmpSemDat);
        if(tmpSemDat.OSCnt == 0)
        {
            esKRNL_SemPost(ldec_itxt->FrameBufSem);
        }

        return (__s32)(&ldec_itxt->outputItem);
    }
    else
    {
        //WARNING("cannot find the right subtitle\n");
        return (__s32)NULL;
    }
}

/*******************************************************************************
*******************************************************************************/
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

__s32 ldec_subtitle_decode0(void *p_arg)
{
    __s32 ret = 0;
    __u8  reconIdx = 0;
    __u8  writeIdx = 0;
    __u8  readIdx= 0;
    __u32 usedBytes = 0;
    __u8  *curDataPtr = NULL;
    __s8  lbsChgFlg;
    __cedar_subtitle_item_t *curSuItem;

    /*step 0 select frame manager*/
    if(ldec_itxt->ldecLbsBuf.validSize == 0)
        return LDEC_ERR_LBS_UNDERFLOW;

    while(reconIdx < LDEC_MAX_TXT_FRM_NUM)
    {
        if(ldec_itxt->ldecManager.subIsFull[reconIdx] == 0)
            break;
        reconIdx++;
    }
    if(reconIdx == LDEC_MAX_TXT_FRM_NUM)
        return LDEC_ERR_NO_FRAMEBUFFER;
    else
        ldec_itxt->ldecManager.reconBufIdx = reconIdx;

    readIdx = ldec_itxt->ldecLbsBuf.ldecReadIdx;
    curSuItem = &(ldec_itxt->ldecManager.subItem[reconIdx]);

    curSuItem->uPts      = ldec_itxt->ldecLbsBuf.ldecBufCrl[readIdx].ptsVal;
    curSuItem->uDuration = ldec_itxt->ldecLbsBuf.ldecBufCrl[readIdx].duration;
    curSuItem->uDataType = ldec_itxt->ldecLbsBuf.ldecBufCrl[readIdx].dataType;
    curSuItem->uSubSize  = ldec_itxt->ldecLbsBuf.ldecBufCrl[readIdx].dataLen;
    curSuItem->uPtsSize  = 0;
    lbsChgFlg = ldec_itxt->ldecLbsBuf.ldecBufCrl[readIdx].lbsChgFlg;
    curDataPtr = ldec_judge_subdata_cover_bufEnd((void *) 0);
    if(curDataPtr == NULL)
    {  
       ldec_update_read_pointer(&ldec_itxt->ldecLbsBuf, ldec_itxt->ldecLbsBuf.validSize);
       return  LDEC_ERR_LFMT_ERR;
    }
    
    ldec_itxt->decFlag = 1;
    
    if(ldec_itxt->subtitle_type == LYRIC_ITXT)
    {
        ret = ldec_itxt_decode(curSuItem, curDataPtr, &usedBytes);
    }
    else if(ldec_itxt->subtitle_type == LYRIC_ISSA)
    {
        ret = ldec_issa_decode(curSuItem, curDataPtr, &usedBytes,lbsChgFlg);
    }
    else if(ldec_itxt->subtitle_type == LYRIC_ISUB)
    {
        ret = ldec_isub_decode(ldec_itxt, curSuItem,curDataPtr, &usedBytes);
    }
    else
    {
        usedBytes = ldec_itxt->ldecLbsBuf.ldecBufCrl[readIdx].dataLen;
        ret = LDEC_ERR_LFMT_ERR;
    }

    ldec_update_read_pointer(&ldec_itxt->ldecLbsBuf, usedBytes);
    if(ret == LDEC_ERR_NONE)
    {
        ldec_itxt->ldecManager.validSize++;
        ldec_itxt->ldecManager.subIsFull[reconIdx] = 1;
        writeIdx = ldec_itxt->ldecManager.subWriteIdx;
        ldec_itxt->ldecManager.subIdxBuf[writeIdx] = reconIdx;
        ldec_itxt->ldecManager.subWriteIdx ++;
        if(ldec_itxt->ldecManager.subWriteIdx == LDEC_MAX_TXT_FRM_NUM)
        {
            ldec_itxt->ldecManager.subWriteIdx = 0;
        }
    }
    ldec_itxt->decFlag = 0;

    return ret;
}

static void ldec_exit(void *p_arg)
{
    __s32   i = 0;
    __u8    err;

    if(ldec_itxt->LbsInputSem)
    {
        esKRNL_SemDel(ldec_itxt->LbsInputSem, OS_DEL_ALWAYS, &err);
        ldec_itxt->LbsInputSem = 0;
    }

    if(ldec_itxt->FrameBufSem)
    {
        esKRNL_SemDel(ldec_itxt->FrameBufSem, OS_DEL_ALWAYS, &err);
        ldec_itxt->FrameBufSem = 0;
    }

    if(ldec_itxt->ldecLbsBuf.ldecBuf!= NULL)
    {
        CEDAR_FREE(ldec_itxt->ldecLbsBuf.ldecBuf);
        ldec_itxt->ldecLbsBuf.ldecBuf = (__u8 *)0;
    }
    if(ldec_itxt->anciDataBuf != NULL)
    {
        CEDAR_FREE(ldec_itxt->anciDataBuf);
        ldec_itxt->anciDataBuf = (__u8 *)0;
    }
    if(ldec_itxt->copyDataBuf!= NULL)
    {
        CEDAR_FREE(ldec_itxt->copyDataBuf);
        ldec_itxt->copyDataBuf = (__u8 *)0;
    }
    if(ldec_itxt->outputItem.pSubtitle != NULL)
    {
        CEDAR_FREE(ldec_itxt->outputItem.pSubtitle);
        ldec_itxt->outputItem.pSubtitle = (__u8 *)0;
    }

    for(i=0; i<LDEC_MAX_TXT_FRM_NUM; i++)
    {
        if(ldec_itxt->ldecManager.subItem[i].pSubtitle !=NULL)
        {
            CEDAR_FREE(ldec_itxt->ldecManager.subItem[i].pSubtitle);
            ldec_itxt->ldecManager.subItem[i].pSubtitle = (__u8 *)0;
        }
    }

    if(ldec_itxt != NULL)
    {
        CEDAR_FREE(ldec_itxt);
        ldec_itxt = (__ldec_itxt_t *)0;
    }

    //4. deinit cedar heap and physic heap
    CEDAR_HEAPEXIT();
    CEDAR_PHEAPEXIT();
}

#endif

