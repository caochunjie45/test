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
* File   : maple.c
* Version: V1.0
* By     : Eric_wang
* Date   : 2011-1-10
* Description:
    各模块调试驱动号:
    maple: 0x4a, plg
    play_control:0x5a, plg
    vdec:0x5c, plg
    adec:0x5d, plg
    ardr:0x5f, plg
    aply:0x59, plg
    avsync:0x50,
    vdrv:0x51,
    adrv:0x52
    dmx:0x53, plg
    tsc:0x54,
    demoddrv:0x55,
********************************************************************************
*/

#include "maple_i.h"

//#ifdef DVBT
//#define MAPLE_TV_STANDAR    MAPLE_TV_DVB_T
//#endif
//
//#ifdef ISDBT
//#define MAPLE_TV_STANDAR    MAPLE_TV_ISDB_T
//#endif
//
//#ifdef CMMB
//#define MAPLE_TV_STANDAR    MAPLE_TV_CMMB
//#endif

maple_t*    g_maple = NULL;
maple_ve_status_e *maple_ve_status = NULL;;

enum __CEDARLIB_LYRIC_ENCODE_TYPE convertSysCharset2CedarlibLyricEncodeType(__epdk_charset_enm_e charCodeType)
{
    enum __CEDARLIB_LYRIC_ENCODE_TYPE nCedarlibLyricEncodeType;
    switch(charCodeType)
    {
        case EPDK_CHARSET_ENM_UTF8:
        {
            nCedarlibLyricEncodeType = LYRIC_SUB_ENCODE_UTF8;
            break;
        }
        case EPDK_CHARSET_ENM_GB2312:
        {
            nCedarlibLyricEncodeType = LYRIC_SUB_ENCODE_GB2312;
            break;
        }
        case EPDK_CHARSET_ENM_ISO_8859_1:
        {
            nCedarlibLyricEncodeType = LYRIC_SUB_ENCODE_ISO_8859_1;
            break;
        }
        case EPDK_CHARSET_ENM_ISO_8859_15:
        {
            nCedarlibLyricEncodeType = LYRIC_SUB_ENCODE_ISO_8859_15;
            break;
        }
        case EPDK_CHARSET_ENM_JIS:
        {
            nCedarlibLyricEncodeType = LYRIC_SUB_ENCODE_ARIBJIS;
            break;
        }
        default:
        {
            eDbug("Unknown charCodeType[0x%x],convert to LYRIC_SUB_ENCODE_UTF8!\n", charCodeType);
            nCedarlibLyricEncodeType = LYRIC_SUB_ENCODE_UTF8;
            break;
        }
    }
    return nCedarlibLyricEncodeType;
}

static __s32 videoRequestBuffer(void* arg)
{
    //* request buffer from player.
    return esMODS_MIoctrl(g_maple->player, DVB_CMD_REQUEST_BUFFER, MEDIA_TYPE_VIDEO, (void*)arg);
}


static __s32 videoUpdateData(void* arg)
{
    //* update data to player.
    return esMODS_MIoctrl(g_maple->player, DVB_CMD_UPDATE_DATA, MEDIA_TYPE_VIDEO, (void*)arg);
}


static __s32 audioRequestBuffer(void* arg)
{
    //* request buffer from player.
    return esMODS_MIoctrl(g_maple->player, DVB_CMD_REQUEST_BUFFER, MEDIA_TYPE_AUDIO, (void*)arg);
}


static __s32 audioUpdateData(void* arg)
{
    //* update data to player.
    return esMODS_MIoctrl(g_maple->player, DVB_CMD_UPDATE_DATA, MEDIA_TYPE_AUDIO, (void*)arg);
}


#if (MAPLE_TV_STANDAR != MAPLE_TV_CMMB)
static __s32 subtitleRequestBuffer(void* arg)
{
    //* request buffer from player.
    return esMODS_MIoctrl(g_maple->player, DVB_CMD_REQUEST_BUFFER, MEDIA_TYPE_SUBTITLE, arg);
}

static __s32 subtitleUpdateData(void* arg)
{
    //* update data to player
    return esMODS_MIoctrl(g_maple->player, DVB_CMD_UPDATE_DATA, MEDIA_TYPE_SUBTITLE, arg);
}
#endif

//以下是录制service时用到的函数,现在的录制只是录制ts包,是不需要区分si_table, video,audio,subtitle的.这里在函数名上加以区分
//只是为了方便调试.
static __s32 RecordTspkt_WriteData(__md_buf_t *pBufInfo)
{
    __u8    err;
    __s32   ret = EPDK_FAIL;
    __s32   nWtNum = 0;
    g_maple->pSemWriteFS->SemPend(g_maple->pSemWriteFS, 0, &err);
    if(err != OS_NO_ERR)
    {
        eDbug("fatal error! RecordTsPkt: pend semaphore fail[%d]\n", err);
        if(g_maple->record_cb)
        {
            esKRNL_CallBack(g_maple->record_cb, (void*)MAPLE_REC_STAT_WRITE_FAIL);
        }
        return EPDK_FAIL;
    }
    if(g_maple->nDiskSpace <=(g_maple->nWtSize + MIN_FILE_SPACE))
    {
        eDbug("Already detect Disk will be full[%d][%d], return!\n", (__u32)(g_maple->nDiskSpace/(1024*1024)), (__u32)(g_maple->nWtSize/(1024*1024)));
        //应该已经发过消息了,直接返回
        ret = EPDK_FAIL;
        if(g_maple->record_cb)
        {
            esKRNL_CallBack(g_maple->record_cb, (void*)MAPLE_REC_STAT_DISK_FULL);
        }

        goto _exit_WriteData;
    }
    if(g_maple->pRecordFile)
    {
        nWtNum = fwrite(pBufInfo->buf, 1, pBufInfo->bufSize, g_maple->pRecordFile);
        if(nWtNum!=pBufInfo->bufSize)
        {
            eDbug("disk full?[%d][%d]\n", nWtNum, pBufInfo->bufSize);
            if(g_maple->record_cb)
            {
                esKRNL_CallBack(g_maple->record_cb, (void*)MAPLE_REC_STAT_WRITE_FAIL);
            }
            ret = EPDK_FAIL;
        }
        else
        {
            ret = EPDK_OK;
        }
        g_maple->nWtSize += nWtNum;
        if(g_maple->nDiskSpace <=(g_maple->nWtSize + MIN_FILE_SPACE))
        {
            eDbug("Disk will be full, stop record! [%d]MB,[%d]MB\n", (__u32)(g_maple->nDiskSpace/(1024*1024)), (__u32)(g_maple->nWtSize/(1024*1024)));
            esKRNL_QPost(g_maple->CmdQ, (void *)DVBC_MAINTASK_CMD_DISK_FULL);
            if(g_maple->record_cb)
            {
                esKRNL_CallBack(g_maple->record_cb, (void*)MAPLE_REC_STAT_DISK_FULL);
            }
        }
    }
    else
    {
        eDbug("fatal error! why g_maple->pRecordFile == NULL?\n");
        if(g_maple->record_cb)
        {
            esKRNL_CallBack(g_maple->record_cb, (void*)MAPLE_REC_STAT_WRITE_FAIL);
        }
        ret = EPDK_FAIL;
    }
_exit_WriteData:
    g_maple->pSemWriteFS->SemPost(g_maple->pSemWriteFS);
    return ret;
}

static __s32 RecordTspkt_SectionRequestBuffer(void* arg)
{
    //* request buffer from player.
    return EPDK_OK;
}
static __s32 RecordTspkt_SectionUpdateData(void* arg)
{
    __s32   ret;
    __md_buf_t *pBufInfo = (__md_buf_t*)arg;
    ret = RecordTspkt_WriteData(pBufInfo);
    if(ret != EPDK_OK)
    {
        eDbug("si section write data fail\n");
    }
    return ret;
}
static __s32 RecordTspkt_VideoRequestBuffer(void* arg)
{
    //* request buffer from player.
    return EPDK_OK;
}
static __s32 RecordTspkt_VideoUpdateData(void* arg)
{
    __s32   ret;
    __md_buf_t *pBufInfo = (__md_buf_t*)arg;
    ret = RecordTspkt_WriteData(pBufInfo);
    if(ret != EPDK_OK)
    {
        eDbug("video write data fail\n");
    }
    return ret;
}
static __s32 RecordTspkt_AudioRequestBuffer(void* arg)
{
    //* request buffer from player.
    return EPDK_OK;
}
static __s32 RecordTspkt_AudioUpdateData(void* arg)
{
    __s32   ret;
    __md_buf_t *pBufInfo = (__md_buf_t*)arg;
    ret = RecordTspkt_WriteData(pBufInfo);
    if(ret != EPDK_OK)
    {
        eDbug("audio write data fail\n");
    }
    return ret;
}
static __s32 RecordTspkt_SubtitleRequestBuffer(void* arg)
{
    //* request buffer from player.
    return EPDK_OK;
}
static __s32 RecordTspkt_SubtitleUpdateData(void* arg)
{
    __s32   ret;
    __md_buf_t *pBufInfo = (__md_buf_t*)arg;
    ret = RecordTspkt_WriteData(pBufInfo);
    if(ret != EPDK_OK)
    {
        eDbug("subtitle write data fail\n");
    }
    return ret;
}

/*******************************************************************************
Function name: PCRDetectCBFunc
Description: 
    1.parg暂定为__u32, 测试完后再改.单位ms
    2.在tsc的中断处理程序中调用的callback
    3.经过实测,北京标清.ts和EITV-GINGA_Velha_200k.ts都是50ms~60ms来一次pcr中断.并且是顺序增长,没有重复的.
    4.可以考虑每隔10次中断(大概是半秒钟左右)发一次semaphore,降低后面处理的次数，从而降低系统消耗.
	 5.ISDBT一般只有一个PCR PID而且是独立的，其他制式会出现多个PCR PID，并且可能在音视频的PID里面，
	 	这时打开一个就可以了,但是要考虑弱信号PCR出错情况发生，如果PCR数据偏差太大就不往AVS同步
	 	模块里面传送防止音视频同步出错，导致画面不能恢复的情况出现.

Parameters: 
    
Return: 
    
Time: 2011/1/13
*******************************************************************************/
static __s32 PCRDetectCBFunc(void* arg)
{
    __u32 tmpPcrPts = (__u32)arg;
	__u32 PcrPts = 0;
    OS_SEM_DATA tmpSemDat;
    //* update data to player
    //__wrn("[%d]\n", PcrPts);
//    if(g_maple->PcrPts == PcrPts)
//    {
//        eDbug("impossible! pcrpts[%d] same! check!\n", PcrPts);
//    }
//eDbug("1 g_maple->Adjustpcrcnt[%d]\n",g_maple->Adjustpcrcnt);
			if((5000<g_maple->PrePcrPts2 - g_maple->PrePcrPts1)&&(g_maple->PrePcrPts2 - g_maple->PrePcrPts1<-5000))
			{
				if(g_maple->PrePcrPts2 != 0)
				{
					g_maple->Adjustpcrcnt++;
				}
			}
//eDbug("2 g_maple->Adjustpcrcnt[%d]\n",g_maple->Adjustpcrcnt);

			g_maple->PrePcrPts2 = g_maple->PrePcrPts1;
			if((5000<g_maple->PrePcrPts1-tmpPcrPts)||(g_maple->PrePcrPts1-tmpPcrPts<-5000))
			{
			//正常信号下,PCR数值除了回头以外，每一次变动几乎都以300~500之间递增，现在弱信号下以10倍范围5000纠错。
	       // g_maple->pcrcnt++; 
					if(g_maple->PrePcrPts1 != 0)
					{
						g_maple->Adjustpcrcnt++;
					}
			}
		//	eDbug("3 g_maple->Adjustpcrcnt[%d]\n",g_maple->Adjustpcrcnt);

			g_maple->PrePcrPts1 = tmpPcrPts;
			//g_maple->Adjustpcrcnt大于2次都是PCR不按照规律,认为是弱信号
			if((g_maple->Adjustpcrcnt>2)||(g_maple->PrePcrPts2 == 0 )||(g_maple->PrePcrPts1 == 0))
			{
			
			//		eDbug("return:Adjustpcrcnt[%d][%d][%d][%d]\n",g_maple->Adjustpcrcnt,g_maple->PrePcrPts2,g_maple->PrePcrPts1,tmpPcrPts);
					g_maple->Adjustpcrcnt = 0;
					return EPDK_OK;
			}
	//		eDbug("start pcr:4 g_maple->Adjustpcrcnt[%d]\n",g_maple->Adjustpcrcnt);
			PcrPts = g_maple->PrePcrPts2;
		
    if(0 == g_maple->pcrcnt%10)    //10次中断发送一个semaphore吧,这样就是大约0.5s校准一次video clock,一般码流大约50ms来一次pcrpts.
    {
       // eDbug("pts[%d], cnt[%d], tm[%d]\n",g_maple->PcrPts, g_maple->pcrcnt, PcrPts);
        if(g_maple->PcrPts == PcrPts)
        {
            //如果和上次相同,就不发送了
            eDbug("impossible! pcrpts[%d] same! check!\n", PcrPts);
            g_maple->pcrcnt++;
            return EPDK_OK;
        }
        g_maple->pSemPcrDetect->SemQuery(g_maple->pSemPcrDetect, &tmpSemDat);
        if(0!=tmpSemDat.OSCnt)
        {
            //semaphore一般情况下一定来得及处理,如果50ms来一次pcrpts的话.
            eDbug("fatal error! semaphore is not 0, check code!\n");
            g_maple->pcrcnt++;
            return EPDK_OK;
        }
        g_maple->PcrPts = PcrPts;
        g_maple->pSemPcrDetect->SemPost(g_maple->pSemPcrDetect);
        g_maple->pcrcnt++;
        return EPDK_OK;
    }
    else
    {
        g_maple->pcrcnt++;
        return EPDK_OK;
    }
}
/*******************************************************************************
Function name: PCRDetectTask
Description: 
    1.默认一进来, PcrDetect就已经打开
Parameters: 
    
Return: 
    
Time: 2011/1/13
*******************************************************************************/
static void PCRDetectTask(void *p_arg)
{
    __u8            err;
    //__s32           result;

    while(1)
    {
        //1.pend Semaphore
        while(1)
        {
            if(esKRNL_TDelReq(EXEC_prioself) == OS_TASK_DEL_REQ)
            {
                eDbug("Delete PCRDetect Task!\n");
                goto _exit0;
            }
            g_maple->pSemPcrDetect->SemPend(g_maple->pSemPcrDetect, 50, &err);  //500ms吧
            if(err == OS_NO_ERR)
            {
                break;
            }
            else
            {
                continue;
            }
        }
        //2. set avsync to adjust video clock
        //eDbug("[%s] set pcrpts[%d] to avsync!\n", __FUNCTION__, g_maple->PcrPts);
        esMODS_MIoctrl(g_maple->player, DVB_CMD_SET_PCR, 0, (void*)g_maple->PcrPts);
    }

_exit0:
    INFORMATION("--------------------------- PCRDetect Task exit!\n");

    esKRNL_TDel(EXEC_prioself);
}


/*******************************************************************************
Function name: Maple_StartPCRDetect
Description: 
    1.务必注意pcr detect要放在相关video,audio,subtitle的pid的filter打开之后进行,因为
    pcrpts有可能附带在那些pid上传输,也有可能独立传输.
    如果是附带传输,务必先打开pidfilter,再配置相关寄存器.否则出错.

    目前看到的ts流都是独立传输pcrpts.

    2.调用本函数时,默认一定有pcrpid.
Parameters: 
    
Return: 
    
Time: 2011/1/14
*******************************************************************************/
__s32 Maple_StartPCRDetect(maple_t *pMaple, __u16 pcrPid)
{
    __u8            err;
    __s32           result;
    __pCBK_t        PCRDetectCallback;
    
    pMaple->pcrcnt = 0; //检测pcr中断来的次数
    if(pMaple->pSemPcrDetect)
    {
        eDbug("fatal error! pMaple->pSemPcrDetect[0x%x] != NULL, check code!\n", pMaple->pSemPcrDetect);
        deleteSemObj(pMaple->pSemPcrDetect, OS_DEL_ALWAYS, &err);
        pMaple->pSemPcrDetect = NULL;
    }
    pMaple->pSemPcrDetect = newSemObj(0);
    if(NULL == pMaple->pSemPcrDetect)
    {
        eDbug("fatal error! SemPcrDetect create fail!\n");
        goto _err0;
    }
    
    pMaple->pcrDetecttskprio = esKRNL_TCreate(PCRDetectTask, 0, 0x1000, (pMaple->mid << 8) | KRNL_priolevel2);
    if(0 == pMaple->pcrDetecttskprio)
    {
        eDbug("fatal error! Create PCRDetectTask() fail!\n");
        goto _err0;
    }
    
    PCRDetectCallback     = esKRNL_GetCallBack(PCRDetectCBFunc);
    result = esMODS_MIoctrl(pMaple->demux, 
                        DVB_DMX_CMD_OPEN_PCR_DETECT, 
                        pcrPid, 
                        (void*)PCRDetectCallback);
    if (result != EPDK_OK)
    {
        eDbug("open demux pcr filter fail.\n");
        //goto playServide_fail;
        goto _err0;
    }
    return EPDK_OK;

_err0:
    if(pMaple->pcrDetecttskprio)
    {
        // clear delay
        esKRNL_TimeDlyResume(pMaple->pcrDetecttskprio);
        while(esKRNL_TDelReq(pMaple->pcrDetecttskprio) != OS_TASK_NOT_EXIST)
        {
            //wait video decoder main task exit
            esKRNL_TimeDlyResume(pMaple->pcrDetecttskprio);
            esKRNL_TimeDly(1);
        }
        pMaple->pcrDetecttskprio = 0;
    }
    if(pMaple->pSemPcrDetect)
    {
        deleteSemObj(pMaple->pSemPcrDetect, OS_DEL_ALWAYS, &err);
        pMaple->pSemPcrDetect = NULL;
    }
    return EPDK_FAIL;
}

__s32 Maple_StopPCRDetect(maple_t *pMaple)
{
    __u8    err;
    // close pcrpid detect
    esMODS_MIoctrl(pMaple->demux, DVB_DMX_CMD_CLOSE_PCR_DETECT, 0, NULL);
    if(pMaple->pcrDetecttskprio)
    {
        // clear delay
        esKRNL_TimeDlyResume(pMaple->pcrDetecttskprio);
        while(esKRNL_TDelReq(pMaple->pcrDetecttskprio) != OS_TASK_NOT_EXIST)
        {
            //wait video decoder main task exit
            esKRNL_TimeDlyResume(pMaple->pcrDetecttskprio);
            esKRNL_TimeDly(1);
        }
        pMaple->pcrDetecttskprio = 0;
    }
    if(pMaple->pSemPcrDetect)
    {
        deleteSemObj(pMaple->pSemPcrDetect, OS_DEL_ALWAYS, &err);
        pMaple->pSemPcrDetect = NULL;
    }
    pMaple->pcrcnt = 0;
    pMaple->PcrPts = 0;
    return EPDK_OK;
}

/*******************************************************************************
Function name: RecordService
Description: 
    1.录制的内容有:
    pat
    pmt,

    video_pid, 
    audio_pid, 
    subtitle_pid

    2.会检查空闲空间还有多少, 太少就不录了.

    3.多音轨和多字幕以后再说.

    4.这里有个潜规则:播放必须先开始,录制的video,audio, subtitle pid必须和播放使用的pid一样,这由自己保证.如果不一样, dmx
    无法辨别出来, tsc会报错,导致通道打不开.
Parameters: 
    nProgId:要录制的service的内部id号。
Return: 
    
Time: 2012/6/21
*******************************************************************************/
static __s32 RecordService(__u32 nProgId)
{
    __s32   result;
    __u32   nCurPlayProgId;
    __s32   ncedar_fs_work_mode = CEDAR_FS_WORK_MODE_HERBCACHE;//CEDAR_FS_WORK_MODE_NO_CACHE;
    maple_serv_item_t*      pServItem = NULL;
    dvb_dmx_chan_param_t    dmxChanParam;
#if (MAPLE_TV_STANDAR == MAPLE_TV_CMMB)
            nCurPlayProgId = cmmb_get_cur_service_id();
#elif (MAPLE_TV_STANDAR == MAPLE_TV_DVB_T)
            nCurPlayProgId = dvb_get_cur_service_id();
#elif (MAPLE_TV_STANDAR == MAPLE_TV_DTMB)
			nCurPlayProgId = dtmb_get_cur_service_id();
#elif (MAPLE_TV_STANDAR == MAPLE_TV_ATSC)
			nCurPlayProgId = atsc_get_cur_service_id();
#elif (MAPLE_TV_STANDAR == MAPLE_TV_DVB_T2)
            nCurPlayProgId = dvbt2_get_cur_service_id();
#else
            nCurPlayProgId = isdb_get_cur_service_id();
#endif
    if(nCurPlayProgId != nProgId || g_maple->curServItem.channelId != nProgId)
    {
        eDbug("fatal error! cur play progId[%d][%d]!=recordProgId[%d], we record cur play service!\n", nCurPlayProgId, g_maple->curServItem.channelId, nProgId);
    }
    pServItem = &g_maple->curServItem;

    //检查剩余空间
    g_maple->nDiskSpace = GETVOLFSPACE(g_maple->record_media_file.file_path);
//    eDbug("disk left [%d]MB, change to 100MB\n", (__u32)(g_maple->nDiskSpace/(1024*1024)));
//    g_maple->nDiskSpace = 100 *1024*1024;
    if(g_maple->nDiskSpace <= MIN_FILE_SPACE)
    {
        if(g_maple->record_cb)
        {
            esKRNL_CallBack(g_maple->record_cb, (void*)MAPLE_REC_STAT_DISK_SPACE_NOT_ENOUGH);
        }
        eDbug("disk will be full! left [%d]MB, can't create file\n", (__u32)(g_maple->nDiskSpace/(1024*1024)));
        return EPDK_FAIL;
    }
    ncedar_fs_work_mode = CEDAR_fs_get_workmod();
    CEDAR_fs_set_workmod(CEDAR_FS_WORK_MODE_HERBCACHE);
    CEDAR_fs_SetCacheSize(MAPLE_RECORD_WTBUFFER_SIZE, CEDAR_FS_WORK_MODE_HERBCACHE);
    CEDAR_fs_set_mid(g_maple->mid);
    g_maple->pRecordFile = fopen(g_maple->record_media_file.file_path, "wb");
    if(NULL == g_maple->pRecordFile)
    {
        eDbug("fopen record file fail\n");
        CEDAR_fs_set_workmod(ncedar_fs_work_mode);  //还原
        if(g_maple->record_cb)
        {
            esKRNL_CallBack(g_maple->record_cb, (void*)MAPLE_REC_STAT_OPEN_FAIL);
        }
        return EPDK_FAIL;
    }
    g_maple->nWtSize = 0;   //初始化nWtSize.
    memset(&g_maple->curRecordServInfo, 0, sizeof(RecordServInfo));
    g_maple->nRecordPatVChanId = -1;
    g_maple->nRecordPmtVChanId = -1;
    g_maple->nRecordVideoVChanId = -1;
    g_maple->nRecordAudioVChanId = -1;
    g_maple->nRecordSubtitleVChanId = -1;
    // 1.PAT,PMT
    dmxChanParam.dmx_type           = DMX_TYPE_TS_DATA_SECTION;
    dmxChanParam.request_buffer_cb  = esKRNL_GetCallBack(RecordTspkt_SectionRequestBuffer);
    dmxChanParam.update_data_cb     = esKRNL_GetCallBack(RecordTspkt_SectionUpdateData);
    result = esMODS_MIoctrl(g_maple->demux, 
                            DVB_DMX_CMD_OPEN_FILTER, 
                            0, 
                            (void*)&dmxChanParam);
    if (result != 0)
    {
        eDbug("open pat section ts raw packet filter fail.\n");
        goto RecordServide_fail;
    }
    else
    {
        g_maple->nRecordPatVChanId = dmxChanParam.nVirtualChanId;
    }

    dmxChanParam.dmx_type           = DMX_TYPE_TS_DATA_SECTION;
    dmxChanParam.request_buffer_cb  = esKRNL_GetCallBack(RecordTspkt_SectionRequestBuffer);
    dmxChanParam.update_data_cb     = esKRNL_GetCallBack(RecordTspkt_SectionUpdateData);
    result = esMODS_MIoctrl(g_maple->demux, 
                            DVB_DMX_CMD_OPEN_FILTER, 
                            pServItem->pmtId, 
                            (void*)&dmxChanParam);
    if (result != 0)
    {
        eDbug("open pmt[0x%x] section ts raw packet filter fail.\n", pServItem->pmtId);
        goto RecordServide_fail;
    }
    else
    {
        g_maple->nRecordPmtVChanId = dmxChanParam.nVirtualChanId;
        g_maple->curRecordServInfo.PmtPid = pServItem->pmtId;
    }

    //2. video,audio,subtitle pid
    if(pServItem->vdStreamNum > 0)
    {
        dmxChanParam.dmx_type           = DMX_TYPE_TS_DATA_VIDEO;
        dmxChanParam.request_buffer_cb  = esKRNL_GetCallBack(RecordTspkt_VideoRequestBuffer);
        dmxChanParam.update_data_cb     = esKRNL_GetCallBack(RecordTspkt_VideoUpdateData);
        result = esMODS_MIoctrl(g_maple->demux, 
                                DVB_DMX_CMD_OPEN_FILTER, 
                                pServItem->vdInfo[0].pid, 
                                (void*)&dmxChanParam);
        if (result != 0)
        {
            eDbug("open demux video filter for tspacket fail.\n");
            goto RecordServide_fail;
        }
        else
        {
            g_maple->nRecordVideoVChanId = dmxChanParam.nVirtualChanId;
            g_maple->curRecordServInfo.VideoPid = pServItem->vdInfo[0].pid;
        }
    }
    if(pServItem->adStreamNum > 0)
    {
        eDbug("Record audioIdx[%d]\n", g_maple->audioIdx);
        dmxChanParam.dmx_type           = DMX_TYPE_TS_DATA_AUDIO;
        dmxChanParam.request_buffer_cb  = esKRNL_GetCallBack(RecordTspkt_AudioRequestBuffer);
        dmxChanParam.update_data_cb     = esKRNL_GetCallBack(RecordTspkt_AudioUpdateData);
        result = esMODS_MIoctrl(g_maple->demux, 
                                DVB_DMX_CMD_OPEN_FILTER, 
                                pServItem->adInfo[g_maple->audioIdx].pid, 
                                (void*)&dmxChanParam);
        if (result != 0)
        {
            eDbug("open demux audio filter for tspacket fail.\n");
            goto RecordServide_fail;
        }
        else
        {
            g_maple->nRecordAudioVChanId = dmxChanParam.nVirtualChanId;
            g_maple->curRecordServInfo.AudioPid = pServItem->adInfo[g_maple->audioIdx].pid;
        }
    }
    if(pServItem->subtitleStreamNum > 0)
    {
        dmxChanParam.dmx_type           = DMX_TYPE_TS_DATA_SUBTITLE;
        dmxChanParam.request_buffer_cb  = esKRNL_GetCallBack(RecordTspkt_SubtitleRequestBuffer);
        dmxChanParam.update_data_cb     = esKRNL_GetCallBack(RecordTspkt_SubtitleUpdateData);
        result = esMODS_MIoctrl(g_maple->demux, 
                                DVB_DMX_CMD_OPEN_FILTER, 
                                pServItem->subtInfo[0].subtPid,
                                (void*)&dmxChanParam);
        if (result != 0)
        {
            eDbug("open demux subtitle filter for tspacket fail.\n");
            goto RecordServide_fail;
        }
        else
        {
            g_maple->nRecordSubtitleVChanId = dmxChanParam.nVirtualChanId;
            g_maple->curRecordServInfo.SubtitlePid = pServItem->subtInfo[0].subtPid;
        }
    }
    CEDAR_fs_set_workmod(ncedar_fs_work_mode);  //还原
    return EPDK_OK;

RecordServide_fail:
    if(g_maple->record_cb)
    {
        esKRNL_CallBack(g_maple->record_cb, (void*)MAPLE_REC_STAT_OPEN_FAIL);
    }

    if(g_maple->nRecordPatVChanId != -1)
    {
        esMODS_MIoctrl(g_maple->demux, DVB_DMX_CMD_CLOSE_FILTER, 0, (void*)g_maple->nRecordPatVChanId);
    }
    if(g_maple->nRecordPmtVChanId != -1)
    {
        esMODS_MIoctrl(g_maple->demux, DVB_DMX_CMD_CLOSE_FILTER, pServItem->pmtId, (void*)g_maple->nRecordPmtVChanId);
    }
    if(g_maple->nRecordVideoVChanId != -1)
    {
        esMODS_MIoctrl(g_maple->demux, DVB_DMX_CMD_CLOSE_FILTER, pServItem->vdInfo[0].pid, (void*)g_maple->nRecordVideoVChanId);
    }
    if(g_maple->nRecordAudioVChanId != -1)
    {
        esMODS_MIoctrl(g_maple->demux, DVB_DMX_CMD_CLOSE_FILTER, pServItem->adInfo[g_maple->audioIdx].pid, (void*)g_maple->nRecordAudioVChanId);
    }
    if(g_maple->nRecordSubtitleVChanId)
    {
        esMODS_MIoctrl(g_maple->demux, DVB_DMX_CMD_CLOSE_FILTER, pServItem->subtInfo[0].subtPid, (void*)g_maple->nRecordSubtitleVChanId);
    }
    if(g_maple->pRecordFile)
    {
        fclose(g_maple->pRecordFile);
        g_maple->pRecordFile = NULL;
    }
    memset(&g_maple->curRecordServInfo, 0, sizeof(RecordServInfo));
    CEDAR_fs_set_workmod(ncedar_fs_work_mode);  //还原
    return EPDK_FAIL;
}

static __s32 stopRecord(void)
{
    //__s32   ret;
    RecordServInfo*  pServInfo;
    
    pServInfo = &g_maple->curRecordServInfo;
    esMODS_MIoctrl(g_maple->demux, DVB_DMX_CMD_CLOSE_FILTER, 0, (void*)g_maple->nRecordPatVChanId);
    if(pServInfo->PmtPid!=0)
    {
        esMODS_MIoctrl(g_maple->demux, DVB_DMX_CMD_CLOSE_FILTER, pServInfo->PmtPid, (void*)g_maple->nRecordPmtVChanId);
    }
    if(pServInfo->VideoPid!=0)
    {
        esMODS_MIoctrl(g_maple->demux, DVB_DMX_CMD_CLOSE_FILTER, pServInfo->VideoPid, (void*)g_maple->nRecordVideoVChanId);
    }
        
    if(pServInfo->AudioPid!=0)
    {
        esMODS_MIoctrl(g_maple->demux, DVB_DMX_CMD_CLOSE_FILTER, pServInfo->AudioPid, (void*)g_maple->nRecordAudioVChanId);
    }
        
    if(pServInfo->SubtitlePid!=0)
    {
        esMODS_MIoctrl(g_maple->demux, DVB_DMX_CMD_CLOSE_FILTER, pServInfo->SubtitlePid, (void*)g_maple->nRecordSubtitleVChanId);
    }
    fclose(g_maple->pRecordFile);
    g_maple->pRecordFile = NULL;
    memset(&g_maple->curRecordServInfo, 0, sizeof(RecordServInfo));
    return 0;
}

static __s32 selectAudstream(__u32 audioIdx)
	{
    __s32                   result;
    __s32                   tmpret;
    maple_serv_item_t*      item;
    tune_param_t            tp;
    __ad_codec_fmt_t        adFmt;
	
    item = &g_maple->curServItem;
    memset(&adFmt, 0, sizeof(adFmt));

    eDbug("playAudstream  channelId = %d,g_maple->audioIdx=%d,audioIdx = %d,adStreamNum=%d\n",item->channelId,g_maple->audioIdx,audioIdx,item->adStreamNum);


    esMODS_MIoctrl(g_maple->demux, DVB_DMX_CMD_CLOSE_FILTER, item->adInfo[g_maple->audioIdx].pid, (void*)g_maple->nAudioVChanId);

    g_maple->audioIdx = audioIdx;

//------------------------------------
    if(item->adStreamNum > 0)
    {
        if(audioIdx >= item->adStreamNum)
        {
            audioIdx = 0;
            g_maple->audioIdx = audioIdx;
        }

        if(item->adInfo[audioIdx].adCodeType == MAPLE_AUD_TYPE_MPEG)
            adFmt.codec_type = DVB_ABS_TYPE_MPEG12_LAYER3;
        else if(item->adInfo[audioIdx].adCodeType == MAPLE_AUD_TYPE_AC3)
            adFmt.codec_type = DVB_ABS_TYPE_AC3;
        else if(item->adInfo[audioIdx].adCodeType == MAPLE_AUD_TYPE_AAC)
            adFmt.codec_type = DVB_ABS_TYPE_MPEG_AAC_LC;
        else if(item->adInfo[audioIdx].adCodeType == MAPLE_AUD_TYPE_DTS)
            adFmt.codec_type = DVB_ABS_TYPE_DTS;
        else if(item->adInfo[audioIdx].adCodeType == MAPLE_AUD_TYPE_DRA)
            adFmt.codec_type = DVB_ABS_TYPE_DRA;
        else
            adFmt.codec_type = DVB_ABS_TYPE_NONE;
    }
    else
    {
        adFmt.codec_type = DVB_ABS_TYPE_NONE;
    }


    if( (adFmt.codec_type == DVB_ABS_TYPE_NONE))
    {
        eDbug("no audio stream of selected service");
        goto selectAudstream_fail;
    }

    esMODS_MIoctrl(g_maple->player, DVB_CMD_SET_MEDIA_INFO, MEDIA_TYPE_AUDIO, (void*)&adFmt);

    //* 4. open pes filter of demux
#if (MAPLE_TV_STANDAR == MAPLE_TV_CMMB)
    
#else
    {
        dvb_dmx_chan_param_t dmxChanParam;
        if(adFmt.codec_type != DVB_ABS_TYPE_NONE)
        {
            dmxChanParam.dmx_type           = DMX_TYPE_AUDIO;
            dmxChanParam.request_buffer_cb  = esKRNL_GetCallBack(audioRequestBuffer);
            dmxChanParam.update_data_cb     = esKRNL_GetCallBack(audioUpdateData);
            result = esMODS_MIoctrl(g_maple->demux,
                                    DVB_DMX_CMD_OPEN_FILTER,
                                    item->adInfo[audioIdx].pid,
                                    (void*)&dmxChanParam);
            if (result != 0)
            {
                eDbug("open audio pes filter fail.\n");
                goto selectAudstream_fail;
            }
            else
            {
                g_maple->nAudioVChanId = dmxChanParam.nVirtualChanId;
            }
        }

    }
#endif
#if 0
#if (MAPLE_TV_STANDAR == MAPLE_TV_CMMB)
    cmmb_set_current_service(channelId);
#elif (MAPLE_TV_STANDAR == MAPLE_TV_DVB_T)
    dvb_set_current_service(channelId);
#else
    isdb_set_current_service(channelId);
#endif
#endif

    return 0;

selectAudstream_fail:
    //* close video pes filter.
#if (MAPLE_TV_STANDAR == MAPLE_TV_CMMB)
#else
    //* close audio pes filter.
    if(adFmt.codec_type != DVB_ABS_TYPE_NONE)
        esMODS_MIoctrl(g_maple->demux, DVB_DMX_CMD_CLOSE_FILTER, item->adInfo[audioIdx].pid, (void*)g_maple->nAudioVChanId);
#endif

    return -1;
}
static __s32 UpdataServiceInfo(void)
{
    __s32                   result;
    __s32                   tmpret = 0;
    maple_serv_item_t*      item;

    item = &g_maple->curServItem;

    eDbug("CheckService  channelId = %d,audioIdx = %d,adStreamNum=%d\n",item->channelId,g_maple->audioIdx,item->adStreamNum);

    {
			  __u16 vdInfo_pid,adInfo_pid;
				__u8 adStreamNum_t,vdStreamNum_t;
				vdInfo_pid = item->vdInfo[0].pid;
				adInfo_pid = item->adInfo[item->adStreamNum-1].pid;
				adStreamNum_t = item->adStreamNum;	//test140219 
				vdStreamNum_t = item->vdStreamNum;	//test140219 
			  eDbug("::check servId[0x%x],vpid[0x%04x],apid[0x%04x],adnum[%d],tsId[%d],[%s] \n", item->servId, item->vdInfo[0].pid,item->adInfo[item->adStreamNum-1].pid, item->adStreamNum, item->tsId, item->servName);
#if(MAPLE_TV_STANDAR == MAPLE_TV_ISDB_T)
				//tmpret = isdb_update_service_info_by_serviceId(item);
#elif(MAPLE_TV_STANDAR == MAPLE_TV_DTMB)
				tmpret = dtmb_update_service_info_by_serviceId(item);
#endif
			  if(EPDK_OK != tmpret)
			  {
			      eDbug("check serviceId[0x%x] fail, return!\n", item->servId);
			      return EPDK_FAIL;
			  }
				if((EPDK_OK == tmpret)
					&&(((vdInfo_pid != item->vdInfo[0].pid)&&(item->vdInfo[0].pid!=0))
						||((adInfo_pid != item->adInfo[item->adStreamNum-1].pid)&&(item->adInfo[item->adStreamNum-1].pid!=0))
						||((adStreamNum_t != item->adStreamNum)&&(item->adStreamNum!=0))
						||((vdStreamNum_t != item->vdStreamNum)&&(item->vdStreamNum!=0))
					    ))
					{
				            eDbug("*******CheckService search serviceId save\n");
							return 1;
					}
				else
				 {
						return 0;
			   }
			        eDbug("retuner:check servId[0x%x], servType[0x%x],vpid[0x%04x],apid[0x%04x],adnum[%d],tsId[%d],[%s] \n", item->servId, item->servType,item->vdInfo[0].pid,item->adInfo[item->adStreamNum-1].pid, item->adStreamNum, item->tsId, item->servName);
    }
    return EPDK_OK;
}
static __s32 playService(__u32 channelId, __u32 audioIdx)
{
    __s32                   result;
    __s32                   tmpret;
    maple_serv_item_t*      item;
    tune_param_t            tp;
    __vd_codec_fmt_t        vdFmt;
    __ad_codec_fmt_t        adFmt;
    __subt_codec_fmt_t      subtFmt;


    
    item = &g_maple->curServItem;
    memset(&vdFmt, 0, sizeof(vdFmt));
    memset(&adFmt, 0, sizeof(adFmt));
    memset(&subtFmt, 0, sizeof(subtFmt));
    
#if (MAPLE_TV_STANDAR == MAPLE_TV_CMMB)
		cmmb_set_av_info(0);
    result = cmmb_get_service_item(item, channelId);
#elif (MAPLE_TV_STANDAR == MAPLE_TV_DVB_T)
    result = dvb_get_service_item(item, channelId);
#elif (MAPLE_TV_STANDAR == MAPLE_TV_DTMB)
    result = dtmb_get_service_item(item, channelId);
#elif (MAPLE_TV_STANDAR == MAPLE_TV_ATSC)
    result = atsc_get_service_item(item, channelId);
#elif (MAPLE_TV_STANDAR == MAPLE_TV_DVB_T2)
    result = dvbt2_get_service_item(item, channelId);
#else
    result = isdb_get_service_item(item, channelId);
#endif

    //eDbug("sizeof(maple_serv_item_t) = %d bytes\n", sizeof(maple_serv_item_t));
    if(result != 0)
        return -1;
    g_maple->audioIdx = audioIdx;
    g_maple->nVideoVChanId = -1;
    g_maple->nAudioVChanId = -1;
    g_maple->nSubtitleVChanId = -1;
    
    //* 1. tune to the frequency
    tp.frequency = item->demod_pm.frequency;
    tp.bandwidth = item->demod_pm.bandwidth;
#if(MAPLE_TV_STANDAR == MAPLE_TV_DVB_T2)
	tp.plpindex = item->demod_pm.plpindex;
	tp.plppid	= item->demod_pm.plppid;
	tp.t_t2_flag= item->demod_pm.t_t2_flag;
	eDbug("playService: frequency %d, bandwidth %d, plpindex %d, plppid %d\n", tp.frequency, tp.bandwidth, tp.plpindex, tp.plppid);
	result = esMODS_MIoctrl(g_maple->demod, DEMOD_TUNE_PLAY, 0, &tp);
#elif(MAPLE_TV_STANDAR == MAPLE_TV_ISDB_T)
	result = esMODS_MIoctrl(g_maple->demod, DEMOD_TUNE_PLAY, 0, &tp);
#else
    result = esMODS_MIoctrl(g_maple->demod, DEMOD_TUNE, 0, &tp);
#endif
    if(result != 0)
    {
        eDbug("==== tune fail, play service return.\n");
        goto playServide_fail;
    }
    //1.5 如果拿出来的service item连pmt_pid都没有,那么就临时再搜一遍.
    if(0 == item->pmtId)
    {
        eDbug("pmtpid ==0, servId[0x%x], servType[0x%x], need scan again!\n", item->servId, item->servType);
#if (MAPLE_TV_STANDAR == MAPLE_TV_ISDB_T)
        tmpret = isdb_search_service_by_serviceId(item);
        if(EPDK_OK != tmpret)
        {
            eDbug("search serviceId[0x%x] fail, return!\n", item->servId);
            return EPDK_FAIL;
        }
#elif (MAPLE_TV_STANDAR == MAPLE_TV_DVB_T)
        tmpret = dvb_search_service_by_serviceId(item);
        if(EPDK_OK != tmpret)
        {
            eDbug("search serviceId[0x%x] fail, return!\n", item->servId);
            return EPDK_FAIL;
        }
#elif (MAPLE_TV_STANDAR == MAPLE_TV_DTMB)
        tmpret = dtmb_search_service_by_serviceId(item);
        if(EPDK_OK != tmpret)
        {
            eDbug("search serviceId[0x%x] fail, return!\n", item->servId);
            return EPDK_FAIL;
        }
#elif (MAPLE_TV_STANDAR == MAPLE_TV_ATSC)
        tmpret = atsc_search_service_by_serviceId(item);
        if(EPDK_OK != tmpret)
        {
            eDbug("search serviceId[0x%x] fail, return!\n", item->servId);
            return EPDK_FAIL;
        }
#elif (MAPLE_TV_STANDAR == MAPLE_TV_DVB_T2)
        tmpret = dvbt2_search_service_by_serviceId(item);
        if(EPDK_OK != tmpret)
        {
            eDbug("search serviceId[0x%x] fail, return!\n", item->servId);
            return EPDK_FAIL;
        }
#endif
    }
		
#if(MAPLE_TV_STANDAR == MAPLE_TV_CMMB) //add by cky 弱信号下的处理
		if(((item->vdStreamNum <= 0)&&(item->adStreamNum <= 0))\
			||(item->vdInfo[0].vdCodeType == MAPLE_VID_TYPE_UNKNOWN)&&(item->vdStreamNum > 0)
			||(item->adInfo[0].adCodeType == MAPLE_AUD_TYPE_UNKNOWN)&&(item->adStreamNum > 0))
		{
				tmpret = cmmb_search_service_by_serviceId(item);
        if(EPDK_OK != tmpret)
        {
            eDbug("cmmb search serviceId[0x%x] fail, return!\n", item->servId);
            return EPDK_FAIL;
        }
		}
#endif

	eDbug("item->pmtId is 0x%x \n",item->pmtId);
	
    //* 2. set media information to player
    if(item->vdStreamNum > 0)
    {
        if(item->vdInfo[0].vdCodeType == MAPLE_VID_TYPE_MPEG2)
            vdFmt.codec_type = DVB_VBS_TYPE_MPEG2_ES;
        else if(item->vdInfo[0].vdCodeType == MAPLE_VID_TYPE_H264)
            vdFmt.codec_type = DVB_VBS_TYPE_H264;
        else
            vdFmt.codec_type = DVB_VBS_TYPE_NONE;
            
        vdFmt.frame_rate = item->vdInfo[0].frameRate;
        vdFmt.width      = item->vdInfo[0].width;
        vdFmt.height     = item->vdInfo[0].height;
        
        if(vdFmt.frame_rate != 0)
            vdFmt.mic_sec_per_frm = 1000*1000/vdFmt.frame_rate;
        else
        {
#if (MAPLE_TV_STANDAR == MAPLE_TV_CMMB)
            eDbug("xxxxxxxxxxxxxxx unknow frame rate, set to 25 fps.\n");
            vdFmt.frame_rate = 25000;
            vdFmt.mic_sec_per_frm = 1000*1000/vdFmt.frame_rate;
#endif
        }
    }
    else
    {
        vdFmt.codec_type = DVB_VBS_TYPE_NONE;
    }
    
    if(item->adStreamNum > 0)
    {
        if(audioIdx >= item->adStreamNum)
        {
            audioIdx = 0;
            g_maple->audioIdx = audioIdx;
        }
            
        if(item->adInfo[audioIdx].adCodeType == MAPLE_AUD_TYPE_MPEG)
            adFmt.codec_type = DVB_ABS_TYPE_MPEG12_LAYER3;
        else if(item->adInfo[audioIdx].adCodeType == MAPLE_AUD_TYPE_AC3)
            adFmt.codec_type = DVB_ABS_TYPE_AC3;
        else if(item->adInfo[audioIdx].adCodeType == MAPLE_AUD_TYPE_AAC)
            adFmt.codec_type = DVB_ABS_TYPE_MPEG_AAC_LC;
        else if(item->adInfo[audioIdx].adCodeType == MAPLE_AUD_TYPE_DTS)
            adFmt.codec_type = DVB_ABS_TYPE_DTS;
        else if(item->adInfo[audioIdx].adCodeType == MAPLE_AUD_TYPE_DRA)
            adFmt.codec_type = DVB_ABS_TYPE_DRA;
        else
            adFmt.codec_type = DVB_ABS_TYPE_NONE;
    }
    else
    {
        adFmt.codec_type = DVB_ABS_TYPE_NONE;
    }
    
    if(item->subtitleStreamNum > 0)
    {
        if(item->subtInfo[0].subtCodeType == MAPLE_SUBT_TYPE_DVBSUB)
            subtFmt.codec_type = DVB_SBS_TYPE_DVBSUBTITLE;
        else if(item->subtInfo[0].subtCodeType == MAPLE_SUBT_TYPE_ARIB)
        {
            subtFmt.codec_type = DVB_SBS_TYPE_ARIBSUBTITLE;
            subtFmt.data_encode_type = convertSysCharset2CedarlibLyricEncodeType(g_maple->charCodeType);
        }
        else
            subtFmt.codec_type = DVB_SBS_TYPE_NONE;

        subtFmt.subtitle_bs_src = CEDARLIB_FILE_FMT_TS;
    }
    else
    {
        subtFmt.codec_type = DVB_SBS_TYPE_NONE;
    }
    
    if((vdFmt.codec_type == DVB_VBS_TYPE_NONE) && (adFmt.codec_type == DVB_ABS_TYPE_NONE))
    {
        eDbug("no media information of selected service\n");
        goto playServide_fail;
    }
    
    result = esMODS_MIoctrl(g_maple->player, DVB_CMD_SET_MEDIA_INFO, MEDIA_TYPE_VIDEO, (void*)&vdFmt);
	if(result != EPDK_OK)
		eDbug("DVB_CMD_SET_MEDIA_INFO failed \n");
    esMODS_MIoctrl(g_maple->player, DVB_CMD_SET_MEDIA_INFO, MEDIA_TYPE_AUDIO, (void*)&adFmt);
    esMODS_MIoctrl(g_maple->player, DVB_CMD_SET_MEDIA_INFO, MEDIA_TYPE_SUBTITLE, (void*)&subtFmt);
    
    
    //* 3. start player
    //CMD_PLAY必须在Openfiler之前.因为dmx送数据的线程会设置avinterval给vply,不希望这时候vply还不存在.

	eDbug("------ start player --------\n");
    result = esMODS_MIoctrl(g_maple->player, DVB_CMD_PLAY, item->nOneSegFlag, NULL);
    if(result != EPDK_OK)
    {
        eDbug("start player fail.\n");
        goto playServide_fail;
    }

	__wrn("-------- open pes filter of demux ------\n");
    
    //* 4. open pes filter of demux
#if (MAPLE_TV_STANDAR == MAPLE_TV_CMMB)
    {	
        SmsDataInitiateParams_ST pParams ={
		CMMB_BASE_DIR,cmmb_data_svc_update_begin_cb,cmmb_data_svc_update_end_cb,NULL
		};
        cmmb_dmx_chan_param_t dmxChanParam;
		void * ppSvc;
		if(item->servType == MAPLE_SERV_TYPE_RADIO) //haspcr is used for judging data service
		    SmsDataSvcInitiate(item->servId,item->pmtId,&pParams,&ppSvc);

        dmxChanParam.type                = CMMB_DMX_TYPE_MEDIA;
        dmxChanParam.serviceId           = item->servId;
        dmxChanParam.subFrameId          = item->pmtId;  //* subFrameId is stored at pmtId field at cmmb mode.
        dmxChanParam.startTimeSlot       = item->tsId;   //* start time slot is stored at tsId field at cmmb mode.
        dmxChanParam.timeSlotCnt         = item->networkId;  //* time slot count is stored at networkId field at cmmb mode.
#if(CMMB_DEMOD_DRIVER == MAPLE_TV_CMMB_IF)    
				dmxChanParam.demodArg						 = item->demodArg;  //*for if 206 driver.
#elif(CMMB_DEMOD_DRIVER == MAPLE_TV_CMMB_TELE) 
				dmxChanParam.byteInterMode       = item->byteInterMode;
				dmxChanParam.ldpcRate						 = item->ldpcRate;
				dmxChanParam.modulateMode				 = item->modulateMode;
				dmxChanParam.muxfrmId						 = item->muxfrmId;
				dmxChanParam.rsRate							 = item->rsRate;
				dmxChanParam.scrambleMode				 =item->scrambleMode;
#endif
		dmxChanParam.vd_request_buffer   = esKRNL_GetCallBack(videoRequestBuffer);
        dmxChanParam.vd_update_data      = esKRNL_GetCallBack(videoUpdateData);
        dmxChanParam.ad_request_buffer   = esKRNL_GetCallBack(audioRequestBuffer);
        dmxChanParam.ad_update_data      = esKRNL_GetCallBack(audioUpdateData);
        dmxChanParam.filterCallback      = esKRNL_GetCallBack(cmmb_update_data_section);
        if(item->servType == MAPLE_SERV_TYPE_RADIO)
            dmxChanParam.filterCallbackParam = ppSvc;
        else
            dmxChanParam.filterCallbackParam = NULL;
        
        g_maple->cmmbDmxChan = esMODS_MIoctrl(g_maple->demux, CMMB_DMX_CMD_OPEN_CHAN, 0, &dmxChanParam);
        if(g_maple->cmmbDmxChan < 0)
            goto playServide_fail;
		
		
    }
#else
    {
    		
        dvb_dmx_chan_param_t dmxChanParam;
		eDbug("adFmt.codec_type = %d,vdFmt.codec_type =%d,subtFmt.codec_type = %d\n",adFmt.codec_type,vdFmt.codec_type,subtFmt.codec_type);
        if(adFmt.codec_type != DVB_ABS_TYPE_NONE)
        {
            dmxChanParam.dmx_type           = DMX_TYPE_AUDIO;
            dmxChanParam.request_buffer_cb  = esKRNL_GetCallBack(audioRequestBuffer);
            dmxChanParam.update_data_cb     = esKRNL_GetCallBack(audioUpdateData);
            result = esMODS_MIoctrl(g_maple->demux, 
                                    DVB_DMX_CMD_OPEN_FILTER, 
                                    item->adInfo[audioIdx].pid, 
                                    (void*)&dmxChanParam);
            if (result != 0)
            {
                eDbug("open audio pes filter fail.\n");
                goto playServide_fail;
            }
            else
            {
                g_maple->nAudioVChanId = dmxChanParam.nVirtualChanId;
            }
        }
        
        if(vdFmt.codec_type != DVB_VBS_TYPE_NONE)
        {
            dmxChanParam.dmx_type           = DMX_TYPE_VIDEO;
            dmxChanParam.request_buffer_cb  = esKRNL_GetCallBack(videoRequestBuffer);
            dmxChanParam.update_data_cb     = esKRNL_GetCallBack(videoUpdateData);
            result = esMODS_MIoctrl(g_maple->demux, 
                                    DVB_DMX_CMD_OPEN_FILTER, 
                                    item->vdInfo[0].pid, 
                                    (void*)&dmxChanParam);
            if (result != 0)
            {
                eDbug("open demux video pes filter fail.\n");
                goto playServide_fail;
            }
            else
            {
                g_maple->nVideoVChanId = dmxChanParam.nVirtualChanId;
            }
        }
        
        if(subtFmt.codec_type != DVB_SBS_TYPE_NONE)
        {
            dmxChanParam.dmx_type           = DMX_TYPE_SUBTITLE;
            dmxChanParam.request_buffer_cb  = esKRNL_GetCallBack(subtitleRequestBuffer);
            dmxChanParam.update_data_cb     = esKRNL_GetCallBack(subtitleUpdateData);
            result = esMODS_MIoctrl(g_maple->demux, 
                                    DVB_DMX_CMD_OPEN_FILTER, 
                                    item->subtInfo[0].subtPid, 
                                    (void*)&dmxChanParam);
            if (result != 0)
            {
                eDbug("open demux subtitle pes filter fail.\n");
                goto playServide_fail;
            }
            else
            {
                g_maple->nSubtitleVChanId = dmxChanParam.nVirtualChanId;
            }
        }

        if(item->hasPcr)
        {
            result = Maple_StartPCRDetect(g_maple, item->pcrPid);
            if (result != EPDK_OK)
            {
                eDbug("open demux pcr filter fail.\n");
                goto playServide_fail;
            }
        }
        else
        {
            eDbug("impossible? cur service has not pcr pid!\n");
        }
    }
#endif
    
#if (MAPLE_TV_STANDAR == MAPLE_TV_CMMB)
    cmmb_set_current_service(channelId);
#elif (MAPLE_TV_STANDAR == MAPLE_TV_DVB_T)
    dvb_set_current_service(channelId);
#elif (MAPLE_TV_STANDAR == MAPLE_TV_DTMB)
    dtmb_set_current_service(channelId);
#elif (MAPLE_TV_STANDAR == MAPLE_TV_ATSC)
    atsc_set_current_service(channelId);
#elif (MAPLE_TV_STANDAR == MAPLE_TV_DVB_T2)
    dvbt2_set_current_service(channelId);
#else
    isdb_set_current_service(channelId);
#endif
    return 0;
    
playServide_fail:
    //* close video pes filter.
#if (MAPLE_TV_STANDAR == MAPLE_TV_CMMB)
    if(g_maple->cmmbDmxChan >= 0)
    {

        eDbug("play service fail\n");
        esMODS_MIoctrl(g_maple->demux, CMMB_DMX_CMD_CLOSE_CHAN, g_maple->cmmbDmxChan, NULL);
        if(item->servType == MAPLE_SERV_TYPE_RADIO)
		    SmsDataSvcCleanup(item->servId,item->pmtId);
        g_maple->cmmbDmxChan = -1;
    }
#else
    //* close video pes filter.
    if(vdFmt.codec_type != DVB_VBS_TYPE_NONE)
        esMODS_MIoctrl(g_maple->demux, DVB_DMX_CMD_CLOSE_FILTER, item->vdInfo[0].pid, (void*)g_maple->nVideoVChanId);
        
    //* close audio pes filter.
    if(adFmt.codec_type != DVB_ABS_TYPE_NONE)
        esMODS_MIoctrl(g_maple->demux, DVB_DMX_CMD_CLOSE_FILTER, item->adInfo[audioIdx].pid, (void*)g_maple->nAudioVChanId);
        
    //* close subtitle pes filter.
    if(subtFmt.codec_type != DVB_SBS_TYPE_NONE)
        esMODS_MIoctrl(g_maple->demux, DVB_DMX_CMD_CLOSE_FILTER, item->subtInfo[0].subtPid, (void*)g_maple->nSubtitleVChanId);
#endif
                
    //* stop player
    esMODS_MIoctrl(g_maple->player, DVB_CMD_STOP, 0, NULL);
    
    return -1;
}


static __s32 stopPlay(void)
{
#if (MAPLE_TV_STANDAR == MAPLE_TV_CMMB)
    maple_serv_item_t*  item;    
    item = &g_maple->curServItem;

    if(g_maple->cmmbDmxChan >= 0)
    {
        eDbug("stop playing\n");
        esMODS_MIoctrl(g_maple->demux, CMMB_DMX_CMD_CLOSE_CHAN, g_maple->cmmbDmxChan, NULL);
        if(item->servType == MAPLE_SERV_TYPE_RADIO)
		    SmsDataSvcCleanup(item->servId,item->pmtId);
        g_maple->cmmbDmxChan = -1;
    }
#else
    __s32   ret;
    maple_serv_item_t*  item;
    
    item = &g_maple->curServItem;
    if(item->hasPcr)
    {
        ret = Maple_StopPCRDetect(g_maple);
        if(ret != EPDK_OK)
        {
            eDbug("stop PCRDetect fail, check code!\n");
        }
    }
    else
    {
        eDbug("impossible?[%s] cur service has not pcr pid!\n", __FUNCTION__);
    }
    if((item->vdStreamNum > 0) && (item->vdInfo[0].pid != 0))
        esMODS_MIoctrl(g_maple->demux, DVB_DMX_CMD_CLOSE_FILTER, item->vdInfo[0].pid, (void*)g_maple->nVideoVChanId);
        
    if((item->adStreamNum > 0) && (item->adInfo[g_maple->audioIdx].pid != 0))
        esMODS_MIoctrl(g_maple->demux, DVB_DMX_CMD_CLOSE_FILTER, item->adInfo[g_maple->audioIdx].pid, (void*)g_maple->nAudioVChanId);
        
    if((item->subtitleStreamNum > 0) && (item->subtInfo[0].subtPid != 0))
        esMODS_MIoctrl(g_maple->demux, DVB_DMX_CMD_CLOSE_FILTER, item->subtInfo[0].subtPid, (void*)g_maple->nSubtitleVChanId);
#endif
    
    esMODS_MIoctrl(g_maple->player, DVB_CMD_STOP, 0, NULL);

    memset(&g_maple->curServItem, 0, sizeof(maple_serv_item_t));
    
    return 0;
}

/*******************************************************************************
Function name: ConfigCedarINI
Description: 
    1.目前还没有malloc出来的成员在CedarINI里,所以不需要free函数.s
Parameters: 
    
Return: 
    
Time: 2011/8/2
*******************************************************************************/
__s32 ConfigMapleINI(CMapleINI   *pMapleINI)
{
    ScriptMapleINI(CEDAR_PLUGINS_DIRECTORY"maple.ini", pMapleINI);
    return EPDK_OK; //不管怎样,都不认为是失败.
}

static void maple_maintsk(void *p_arg)
{
    __u8            err;
    __s32           result;
    //__s32           tmpret;
    //__u32           uFreeMemSize;
    void            *msg;
    maple_t         *maple = (maple_t*)p_arg;
    __s32           nFeedbackStopFlag = 0;  //表示是否是下级模块发送上来要求stop的消息
    __s32           nDiskFullFlag = 0;
    //CedarMsg        *pMsg = NULL;
    //CedarMsg        CedarMsgInst;
    //__s32           nTmpStatus;
    INFORMATION("--------------------------- maple maintsk run!\n");

    while(1)
    {
        while(1)
        {
            //检查是否有其它任务请求删除cedar主进程
            if(esKRNL_TDelReq(EXEC_prioself) == OS_TASK_DEL_REQ)
            {
                eDbug("Delete maple main task!\n");
                goto _exit_maple_maintsk;
            }

            //从cedar命令队列获取命令
            msg = esKRNL_QPend(maple->CmdQ, 1000, &err);
             //pMsg = cedar->CedarCmdQ->QPend(cedar->CedarCmdQ, 1000, &err, &CedarMsgInst);
            if(msg)
            //if(pMsg)
            {
                eDbug("Cedar main task get message:0x%x!\n", (__u32)msg);
                break;
            }
        }

        switch((__u32)msg)
        {
            // stop play
            case DVBC_MAINTASK_CMD_APLYMSG_STOP:
            case DVBC_MAINTASK_CMD_VPLYMSG_STOP:
            case DVBC_MAINTASK_CMD_STOP:
            {
                if((__u32)msg == DVBC_MAINTASK_CMD_STOP)
                {
                    nFeedbackStopFlag = 0;
                }
                else
                {
                    nFeedbackStopFlag = 1;
                }
                eDbug("maple Status = [0x%x] when Stop! msg[0x%x], feedbackflag[%d]\n", maple->status, (__u32)msg, nFeedbackStopFlag);
                if(maple->status == MAPLE_STAT_STOP)
                {
                    eDbug("maple already stop!\n");
                    result = EPDK_OK;
                    goto _exit_stop_play;
                }
                if(stopPlay() == 0)
                {
                    maple->status = MAPLE_STAT_STOP;
                    result = EPDK_OK;
                }
                else
                {
                    eDbug("stop Play fail!\n");
                    result = EPDK_FAIL;
                }
            _exit_stop_play:
                if(0 == nFeedbackStopFlag)  //如果不是下级模块的stop消息,那么就要发送semaphore,因为maple的stop接口是同步的
                {
                    OS_SEM_DATA tmpSemDat;
                    maple->pSemStopPlayDone->SemQuery(maple->pSemStopPlayDone, &tmpSemDat);
                    if(tmpSemDat.OSCnt == 0)
                    {
                        maple->pSemStopPlayDone->SemPost(maple->pSemStopPlayDone);
                    }
                    else
                    {
                        eDbug("fatal error! SemStopPlayDone cnt[%d] > 0!\n", tmpSemDat.OSCnt);
                    }
                }
                break;
            }
            case DVBC_MAINTASK_CMD_STOP_RECORD:
            case DVBC_MAINTASK_CMD_DISK_FULL:
            {
                if((__u32)msg == DVBC_MAINTASK_CMD_DISK_FULL)
                {
                    nDiskFullFlag = 1;
                    eDbug("maple_maintask() Detect disk full!\n");
                }
                else
                {
                    nDiskFullFlag = 0;
                }
                eDbug("maple recordStatus = [0x%x] when Stop! msg[0x%x], diskfullflag[%d]\n", maple->nRecordStatus, (__u32)msg, nDiskFullFlag);
                if(maple->nRecordStatus == MAPLE_STAT_STOP)
                {
                    eDbug("maple already stopRecord!\n");
                    result = EPDK_OK;
                    goto _exit_stop_record;
                }
                if(stopRecord() == 0)
                {
                    maple->nRecordStatus = MAPLE_STAT_STOP;
                    result = EPDK_OK;
                }
                else
                {
                    eDbug("stop Record fail!\n");
                    result = EPDK_FAIL;
                }
            _exit_stop_record:
                if(0 == nDiskFullFlag)  //如果不是发送的stop消息,那么就要发送semaphore,因为maple的stopRecord接口是同步的
                {
                    OS_SEM_DATA tmpSemDat;
                    maple->pSemStopRecordDone->SemQuery(maple->pSemStopRecordDone, &tmpSemDat);
                    if(tmpSemDat.OSCnt == 0)
                    {
                        maple->pSemStopRecordDone->SemPost(maple->pSemStopRecordDone);
                    }
                    else
                    {
                        eDbug("fatal error! SemStopRecordDone cnt[%d] > 0!\n", tmpSemDat.OSCnt);
                    }
                }
                else
                {
					if(maple->record_cb)
					{
						esKRNL_CallBack(maple->record_cb, (void *)MAPLE_REC_STAT_DISK_FULL);
					}
                }
                break;
            }
            default:
            {
                eDbug("unknown msg[0x%x]\n", (__u32)msg);
                break;
            }
        }
    }

_exit_maple_maintsk:

    eDbug("--------------------------- maple maintsk exit!\n");

    esKRNL_TDel(EXEC_prioself);
}

__s32 MInit(void)
{
    g_maple = NULL;
    return EPDK_OK;
}


__s32 MExit(void)
{
    if(g_maple)
    {
        eDbug("maple has not been close yet, try to close it.\n");
        if(MClose((__mp*)g_maple) == EPDK_OK)
        {
            eDbug("close maple ok.\n");
            return EPDK_OK;
        }
        else
        {
            eDbug("close maple fail.\n");
            return EPDK_FAIL;
        }
    }
    else
        return EPDK_OK;
}


__mp* MOpen(__u32 mid, __u32 mode)
{
    __u32       demux_mid;
    __mp*       demux;
    __u32       player_mid;
    __mp*       player;
    __u32       demod_mid;
    __mp*       demod;
    maple_t*    maple;
    __u8        err;

    CEDAR_HEAPINIT();
    CEDAR_PHEAPINIT();

    demux       = NULL;
    demod       = NULL;
    player      = NULL;
    demux_mid   = 0;
    demod_mid   = 0;
    player_mid  = 0;
    maple       = NULL;

    maple = (maple_t*)malloc(sizeof(maple_t));
    if(maple == NULL)
    {
        eDbug("malloc memory for maple fail.\n");
        goto maple_open_fail;
    }
    memset(maple, 0, sizeof(maple_t));

	maple_ve_status = (maple_ve_status_e*)malloc(sizeof(maple_ve_status_e));
	if (maple_ve_status == NULL)
	{
        if(maple)
        {
            free(maple);
            goto maple_open_fail;;
        }
	}
	memset(maple_ve_status, 0, sizeof(maple_ve_status_e));
	
    ConfigMapleINI(&maple->MapleINI);
    maple->mid          = mid;
    maple->cmmbDmxChan  = -1;
    maple->nRecordProgId = (__u32)-1;

#if (MAPLE_TV_STANDAR == MAPLE_TV_CMMB)
    demux_mid = esMODS_MInstall(CMMB_DEMUX_PLUGIN_FILE_PATH, 0);
#else
    demux_mid = esMODS_MInstall(TS_DEMUX_PLUGIN_FILE_PATH, 0);
#endif

    if(demux_mid == 0)
    {
        eDbug("install demux module fail.\n");
        goto maple_open_fail;
    }

    demux = esMODS_MOpen(demux_mid, 0);
    if(demux == NULL)
    {
        eDbug("open demux module fail.\n");
        goto maple_open_fail;
    }

#if (MAPLE_TV_STANDAR != MAPLE_TV_CMMB)
    esMODS_MIoctrl(demux, DVB_DMX_CMD_SET_FECONFIG, 0, &maple->MapleINI.FEConfig);
    if(EPDK_OK!=esMODS_MIoctrl(demux, DVB_DMX_CMD_OPEN, 0, NULL))
    {
        eDbug("dmx IOOpen fail!\n");
        goto maple_open_fail;
    }
#endif    
    demod_mid = esMODS_MInstall(DEMOD_DRV_FILE_PATH, 0);
    if(demod_mid == 0)
    {
        eDbug("install [%s]demod driver fail.\n",DEMOD_DRV_FILE_PATH);
        goto maple_open_fail;
    }

    demod = esMODS_MOpen(demod_mid, 0);

    if(demod == NULL)
    {
        eDbug("open demod driver[%s] fail.\n",DEMOD_DRV_FILE_PATH);
        goto maple_open_fail;
    }
#if (MAPLE_TV_STANDAR == MAPLE_TV_ISDB_T)// ISDBT-1 SEG spi传输TS流
	//设置DEMOD驱动句柄给DEMUX模块，再传递给TSC模块，TSC拿到句柄对DEMOD模块进行操作。
	esMODS_MIoctrl(demux, DVB_DMX_CMD_SET_DEMODDRV, 0, demod);
#endif
//    esMODS_MIoctrl(demod, DEMOD_SET_FECONFIG, 0, &maple->MapleINI.FEConfig);
//    if(EPDK_OK!=esMODS_MIoctrl(demod, DEMOD_OPEN, 0, NULL))
//    {
//        eDbug("io_open demod driver fail.\n");
//        goto maple_open_fail;
//    }
    
    player_mid = esMODS_MInstall(PLAYER_PLUGIN_FILE_PATH, 0);
    if(player_mid == 0)
    {
        eDbug("install player module fail.\n");
        goto maple_open_fail;
    }
    
    player = esMODS_MOpen(player_mid, 0);
        
    if(player == NULL)
    {
        eDbug("open player module fail.\n");
        goto maple_open_fail;
    }
    esMODS_MIoctrl(player, DVB_CMD_SET_MAPLE_INI, 0, &maple->MapleINI);
    
    maple->demux_mid        = demux_mid;
    maple->demod_mid        = demod_mid;
    maple->player_mid       = player_mid;
    maple->demux            = demux;
    maple->demod            = demod;
    maple->player           = player;
    
#if (MAPLE_TV_STANDAR == MAPLE_TV_CMMB)
    {
        __mp* tmpDrvHdl;
        if(esMODS_MIoctrl(maple->demod, DEMOD_GET_CMMB_DRIVER_HDL, 0, &tmpDrvHdl) == EPDK_OK)
        {
            esMODS_MIoctrl(maple->demux, CMMB_DMX_CMD_SET_DRV_HDL, 0, tmpDrvHdl);
        }
        else
        {
            eDbug("get cmmb demod driver handle fail.\n");
            goto maple_open_fail;
        }
    }
#endif
    
#if (MAPLE_TV_STANDAR == MAPLE_TV_CMMB)
    if(cmmb_open() != 0)
    {
        eDbug("open cmmb library fail.\n");
        goto maple_open_fail;
    }
#elif (MAPLE_TV_STANDAR == MAPLE_TV_DVB_T)
    if(dvb_open() != 0)
    {
        eDbug("open si librasy fail.\n");
        goto maple_open_fail;
    }
#elif (MAPLE_TV_STANDAR == MAPLE_TV_DTMB)
    if(dtmb_open() != 0)
    {
        eDbug("open si librasy fail.\n");
        goto maple_open_fail;
    }
#elif (MAPLE_TV_STANDAR == MAPLE_TV_ATSC)
    if(atsc_open() != 0)
    {
        eDbug("open si librasy fail.\n");
        goto maple_open_fail;
    }
#elif (MAPLE_TV_STANDAR == MAPLE_TV_DVB_T2)
    if(dvbt2_open() != 0)
    {
        eDbug("open si librasy fail.\n");
        goto maple_open_fail;
    }
#else
    if(isdb_open() != 0)
    {
        eDbug("open isdb si librasy fail.\n");
        goto maple_open_fail;
    }
#endif

    maple->charCodeType = EPDK_CHARSET_ENM_ISO_8859_15;
    maple->status = MAPLE_STAT_STOP;
    maple->nRecordStatus = MAPLE_STAT_STOP;
    maple->pSemStopPlayDone = newSemObj(0);
    maple->pSemStopRecordDone = newSemObj(0);
    maple->pSemWriteFS = newSemObj(1);
    if(NULL == maple->pSemStopPlayDone || NULL == maple->pSemStopRecordDone || NULL == maple->pSemWriteFS)
    {
        eDbug("new semaphore StopPlayDone or stoprecord fail!\n");
        goto maple_open_fail;
    }
    maple->CmdQ = esKRNL_QCreate(MAPLE_CMDQ_SIZE);
    if(maple->CmdQ == (__krnl_event_t*)0)
    {
        eDbug("new MSGQ fail!\n");
        goto maple_open_fail;
    }
    esMODS_MIoctrl(maple->player, DVB_CMD_SET_FEEDBACK_MSG_QUEUE, 0, maple->CmdQ);
    maple->maintskprio = esKRNL_TCreate(maple_maintsk,
                                            (void *)maple,
                                            0x1000,
                                            (maple->mid<< 8) | KRNL_priolevel2);

    if(maple->maintskprio == 0)
    {
        eDbug("Create cedar main task failed!\n");
        goto maple_open_fail;
    }
    
    g_maple = maple;
    return (__mp*)g_maple;
    
maple_open_fail:
    if(demux)
    {
        esMODS_MClose(demux);
        demux = NULL;
    }
    
    if(player)
    {
        esMODS_MClose(player);
        player = NULL;
    }
    
    if(demod)
    {
        esMODS_MClose(demod);
        demod = NULL;
    }
    
    if(demux_mid)
    {
        esMODS_MUninstall(demux_mid);
        demux_mid = 0;
    }
    
    if(player_mid)
    {
        esMODS_MUninstall(player_mid);
        player_mid = 0;
    }
    
    if(demod_mid)
    {
        esMODS_MUninstall(demod_mid);
        demod_mid = 0;
    }
    if(maple->pSemStopPlayDone)
    {
        deleteSemObj(maple->pSemStopPlayDone, OS_DEL_ALWAYS, &err);
        maple->pSemStopPlayDone = NULL;
    }
    if(maple->pSemStopRecordDone)
    {
        deleteSemObj(maple->pSemStopRecordDone, OS_DEL_ALWAYS, &err);
        maple->pSemStopRecordDone = NULL;
    }
    if(maple->pSemWriteFS)
    {
        deleteSemObj(maple->pSemWriteFS, OS_DEL_ALWAYS, &err);
        maple->pSemWriteFS = NULL;
    }
    if(maple->CmdQ)
    {
        esKRNL_QDel(maple->CmdQ, OS_DEL_NO_PEND, &err);
        maple->CmdQ = NULL;
    }
	if(maple_ve_status)
    {
        free(maple_ve_status);
        maple_ve_status = NULL;
    }
    if(maple)
    {
        free(maple);
    }
    CEDAR_HEAPEXIT();
    CEDAR_PHEAPEXIT();
    return NULL;
}


__s32 MClose(__mp* mp)
{
    __u8    err;
    __s32   ret;
    maple_t* maple;
    __s32   i;
    maple = (maple_t*)mp;
    
    if(maple == NULL)
    {
        eDbug("NULL pointer passed to MClose of maple.\n");
        return EPDK_FAIL;
    }
    
    if(maple != g_maple)
    {
        eDbug("invalid pointer passed to MClose of maple.\n");
        return EPDK_FAIL;
    }
    
#if (MAPLE_TV_STANDAR == MAPLE_TV_CMMB)
    cmmb_close();
#elif (MAPLE_TV_STANDAR == MAPLE_TV_DVB_T)
    dvb_close();
#elif (MAPLE_TV_STANDAR == MAPLE_TV_DTMB)
    dtmb_close();
#elif (MAPLE_TV_STANDAR == MAPLE_TV_ATSC)
    atsc_close();
#elif (MAPLE_TV_STANDAR == MAPLE_TV_DVB_T2)
    dvbt2_close();
#else
    isdb_close();
#endif

    if(maple->demux)
    {
        esMODS_MClose(maple->demux);
        maple->demux = NULL;
    }
    
    if(maple->player)
    {
        esMODS_MClose(maple->player);
        maple->player = NULL;
    }

    if(maple->demod)
    {
        esMODS_MClose(maple->demod);
        maple->demod = NULL;
    }

    if(maple->demux_mid)
    {
        esMODS_MUninstall(maple->demux_mid);
        maple->demux_mid = 0;
    }

    if(maple->player_mid)
    {
        esMODS_MUninstall(maple->player_mid);
        maple->player_mid = 0;
    }

    if(maple->demod_mid)
    {
        esMODS_MUninstall(maple->demod_mid);
        maple->demod_mid = 0;
    }
    //pctpts检测的相关变量的检查
    if(maple->pcrDetecttskprio)
    {
        eDbug("why pcrDetecttask[%d] is exist? check code!\n", maple->pcrDetecttskprio);
        // clear delay
        esKRNL_TimeDlyResume(maple->pcrDetecttskprio);
        while(esKRNL_TDelReq(maple->pcrDetecttskprio) != OS_TASK_NOT_EXIST)
        {
            //wait video decoder main task exit
            esKRNL_TimeDlyResume(maple->pcrDetecttskprio);
            esKRNL_TimeDly(1);
        }
        maple->pcrDetecttskprio = 0;
    }
    if(maple->pSemPcrDetect)
    {
        eDbug("why pSemPcrDetect[0x%x] is exist? check code!\n", maple->pSemPcrDetect);
        deleteSemObj(maple->pSemPcrDetect, OS_DEL_ALWAYS, &err);
        maple->pSemPcrDetect = NULL;
    }

    //delete cedar main task
    for(i=0; i<200; i++)
    {
        if(esKRNL_TDelReq(maple->maintskprio) == OS_TASK_NOT_EXIST)
            break;

        //wakeup main task
        esKRNL_QPost(maple->CmdQ, (void *)DVBC_MAINTASK_CMD_NOP);
        //tmpCedarMsgInst.nMsgType = _CEDAR_CMD_NOP;
        //cedar->CedarCmdQ->QPost(cedar->CedarCmdQ, &tmpCedarMsgInst);
        esKRNL_TimeDly(1);
    }
    if(i == 200)
    {
        eDbug("maple:mclose:main tsk cannot be deleted!\n");
        //return EPDK_FAIL;
        ret = EPDK_FAIL;
    }

    if(maple->pSemStopPlayDone)
    {
        deleteSemObj(maple->pSemStopPlayDone, OS_DEL_ALWAYS, &err);
        maple->pSemStopPlayDone = NULL;
    }
    if(maple->pSemStopRecordDone)
    {
        deleteSemObj(maple->pSemStopRecordDone, OS_DEL_ALWAYS, &err);
        maple->pSemStopRecordDone = NULL;
    }
    if(maple->pSemWriteFS)
    {
        deleteSemObj(maple->pSemWriteFS, OS_DEL_ALWAYS, &err);
        maple->pSemWriteFS = NULL;
    }
    if(maple->CmdQ)
    {
        esKRNL_QDel(maple->CmdQ, OS_DEL_NO_PEND, &err);
        maple->CmdQ = NULL;
    }
	  if(maple_ve_status)
    {
        free(maple_ve_status);
        maple_ve_status = NULL;
    }
    if(g_maple)
    {
        free(g_maple);
        g_maple = NULL;
    }
    CEDAR_HEAPEXIT();
    CEDAR_PHEAPEXIT();
    return EPDK_OK;
}


__u32 MRead(void* pdata, __u32 size, __u32 pid, __mp* mp)
{
    return 0;
}


__u32 MWrite(const void* pdata, __u32 size, __u32 pid, __mp* mp)
{
    return 0;
}


__s32 MIoctrl(__mp* mp, __u32 cmd, __s32 aux, void* pbuffer)
{
    __s32               result=EPDK_OK;
    maple_t*            maple;
    maple_demod_info_t  demodParam;
    __u32               showMode;
    __u8                err;
    maple = (maple_t*)mp;
    if(maple == NULL)
    {
        eDbug("NULL pointer passed to MIoctrl of maple.\n");
        return EPDK_FAIL;
    }
    
    if(maple != g_maple)
    {
        eDbug("invalid pointer passed to MIoctrl of maple.\n");
        return EPDK_FAIL;
    }
    
    switch(cmd)
    {
        case MAPLE_CMD_SET_SPLY_DRAW_CB:
        {
          if(maple->player)
          {
              esMODS_MIoctrl(maple->player, DVB_CMD_SET_SPLY_DRAW_CB, 0, pbuffer);      
          }
          else
          {
              eDbug("Be careful: maple->player == NULL!\n");
          }   
		  result = EPDK_OK;
		  break;         
        }
            
        case MAPLE_CMD_TUNE:
        {
            //* aux = bandwidth_in_10KHz<<22 | frequency_in_KHz, 
            //* pbuffer = pointer to maple_serv_list_t;
            __u32  tmp = (__u32)aux;
            demodParam.bandwidth   = ((__u32)tmp >> 22) * 10;  //* in KHz
            demodParam.frequency   = tmp & 0x3fffff;    //* in KHz
            result = esMODS_MIoctrl(maple->demod, DEMOD_TUNE, 0, (void*)&demodParam);
            break;
        }
        
        case MAPLE_CMD_GET_SS:
        {
            result = esMODS_MIoctrl(maple->demod, DEMOD_GET_SIGNAL_STATUS, 0, pbuffer);
            break;
        }
        case MAPLE_CMD_GET_LOCK_STATUS:
        {
            result = esMODS_MIoctrl(maple->demod, DEMOD_GET_LOCK_STATUS, 0, NULL);
            break;
        }
        
        case MAPLE_CMD_SL_OPEN:             //* open file to read service list
        {
#if (MAPLE_TV_STANDAR == MAPLE_TV_CMMB)
            if(cmmb_open_list((__u8*)pbuffer) == 0)   //* pbuffer gives the file path.
                result = EPDK_OK;
            else
                result = EPDK_FAIL;
#elif (MAPLE_TV_STANDAR == MAPLE_TV_DVB_T)
            if(dvb_open_list((__u8*)pbuffer) == 0)   //* pbuffer gives the file path.
                result = EPDK_OK;
            else
                result = EPDK_FAIL;
#elif (MAPLE_TV_STANDAR == MAPLE_TV_DTMB)
            if(dtmb_open_list((__u8*)pbuffer) == 0)   //* pbuffer gives the file path.
                result = EPDK_OK;
            else
                result = EPDK_FAIL;
#elif (MAPLE_TV_STANDAR == MAPLE_TV_ATSC)
            if(atsc_open_list((__u8*)pbuffer) == 0)   //* pbuffer gives the file path.
                result = EPDK_OK;
            else
                result = EPDK_FAIL;
#elif (MAPLE_TV_STANDAR == MAPLE_TV_DVB_T2)
            if(dvbt2_open_list((__u8*)pbuffer) == 0)   //* pbuffer gives the file path.
                result = EPDK_OK;
            else
                result = EPDK_FAIL;
#else
            if(isdb_open_list((__u8*)pbuffer) == 0)
                result = EPDK_OK;
            else
                result = EPDK_FAIL;
#endif
            break;
        }
        
        case MAPLE_CMD_SL_SAVE:             //* save the current service list to file
        {
#if (MAPLE_TV_STANDAR == MAPLE_TV_CMMB)
            if(cmmb_save_list((__u8*)pbuffer) == 0)
                result = EPDK_OK;
            else
                result = EPDK_FAIL;
#elif (MAPLE_TV_STANDAR == MAPLE_TV_DVB_T)
            if(dvb_save_list((__u8*)pbuffer) == 0)
                result = EPDK_OK;
            else
                result = EPDK_FAIL;
#elif (MAPLE_TV_STANDAR == MAPLE_TV_DTMB)
            if(dtmb_save_list((__u8*)pbuffer) == 0)  
                result = EPDK_OK;
            else
                result = EPDK_FAIL;
#elif (MAPLE_TV_STANDAR == MAPLE_TV_ATSC)
            if(atsc_save_list((__u8*)pbuffer) == 0)   
                result = EPDK_OK;
            else
                result = EPDK_FAIL;
#elif (MAPLE_TV_STANDAR == MAPLE_TV_DVB_T2)
            if(dvbt2_save_list((__u8*)pbuffer) == 0)
                result = EPDK_OK;
            else
                result = EPDK_FAIL;
#else
            if(isdb_save_list((__u8*)pbuffer) == 0)
                result = EPDK_OK;
            else
                result = EPDK_FAIL;
#endif
            break;
        }
        
        case MAPLE_CMD_SL_GET:              //* get service list of the si/cmmb library
        {
            //* aux indicate the service type to copy.
            //* pbuffer gives a memory space to save service list.
#if (MAPLE_TV_STANDAR == MAPLE_TV_CMMB)
            if(cmmb_get_list((maple_serv_list_t*)pbuffer, aux) == 0)
                result = EPDK_OK;
            else
                result = EPDK_FAIL;
#elif (MAPLE_TV_STANDAR == MAPLE_TV_DVB_T)
            if(dvb_get_list((maple_serv_list_t*)pbuffer, aux) == 0)
                result = EPDK_OK;
            else
                result = EPDK_FAIL;
#elif (MAPLE_TV_STANDAR == MAPLE_TV_DTMB)
            if(dtmb_get_list((maple_serv_list_t*)pbuffer, aux) == 0)
                result = EPDK_OK;
            else
                result = EPDK_FAIL;
#elif (MAPLE_TV_STANDAR == MAPLE_TV_ATSC)
            if(atsc_get_list((maple_serv_list_t*)pbuffer, aux) == 0)
                result = EPDK_OK;
            else
                result = EPDK_FAIL;
#elif (MAPLE_TV_STANDAR == MAPLE_TV_DVB_T2)
            if(dvbt2_get_list((maple_serv_list_t*)pbuffer, aux) == 0)
                result = EPDK_OK;
            else
                result = EPDK_FAIL;
#else
            if(isdb_get_list((maple_serv_list_t*)pbuffer, aux) == 0)
                result = EPDK_OK;
            else
                result = EPDK_FAIL;
#endif
            break;
        }
        
        case MAPLE_CMD_SL_ADD:              //* add service list to the si library
        {
#if (MAPLE_TV_STANDAR == MAPLE_TV_CMMB)
            if(cmmb_add_list((maple_serv_list_t*)pbuffer) == 0)
                result = EPDK_OK;
            else
                result = EPDK_FAIL;
#elif (MAPLE_TV_STANDAR == MAPLE_TV_DVB_T)
            if(dvb_add_list((maple_serv_list_t*)pbuffer) == 0)
                result = EPDK_OK;
            else
                result = EPDK_FAIL;
#elif (MAPLE_TV_STANDAR == MAPLE_TV_DTMB)
            if(dtmb_add_list((maple_serv_list_t*)pbuffer) == 0)
                result = EPDK_OK;
            else
                result = EPDK_FAIL;
#elif (MAPLE_TV_STANDAR == MAPLE_TV_ATSC)
            if(atsc_add_list((maple_serv_list_t*)pbuffer) == 0)
                result = EPDK_OK;
            else
                result = EPDK_FAIL;
#elif (MAPLE_TV_STANDAR == MAPLE_TV_DVB_T2)
            if(dvbt2_add_list((maple_serv_list_t*)pbuffer) == 0)
                result = EPDK_OK;
            else
                result = EPDK_FAIL;
#else
            if(isdb_add_list((maple_serv_list_t*)pbuffer) == 0)
            {
                result = EPDK_OK;
                //isdb_sort_list(SORTMODE_REMOTE_CONTROL_KEY_ID);
            }
            else
                result = EPDK_FAIL;
#endif
            break;  
        }
        
        case MAPLE_CMD_SL_DELETE:           //* delete the service list of the si library
        {
#if (MAPLE_TV_STANDAR == MAPLE_TV_CMMB)
            if(cmmb_delete_list() == 0)
                result = EPDK_OK;
            else
                result = EPDK_FAIL;
#elif (MAPLE_TV_STANDAR == MAPLE_TV_DVB_T)
            if(dvb_delete_list() == 0)
                result = EPDK_OK;
            else
                result = EPDK_FAIL;
#elif (MAPLE_TV_STANDAR == MAPLE_TV_DTMB)
            if(dtmb_delete_list() == 0)
                result = EPDK_OK;
            else
                result = EPDK_FAIL;
#elif (MAPLE_TV_STANDAR == MAPLE_TV_ATSC)
            if(atsc_delete_list() == 0)
                result = EPDK_OK;
            else
                result = EPDK_FAIL;
#elif (MAPLE_TV_STANDAR == MAPLE_TV_DVB_T2)
            if(dvbt2_delete_list() == 0)
                result = EPDK_OK;
            else
                result = EPDK_FAIL;
#else
            if(isdb_delete_list() == 0)
                result = EPDK_OK;
            else
                result = EPDK_FAIL;
#endif
            break;
        }
        
        case MAPLE_CMD_SERV_ITEM_ADD:               //* aux = , pbuffer = pointer to service item;
        {
#if (MAPLE_TV_STANDAR == MAPLE_TV_CMMB)
            cmmb_add_serv_item((maple_serv_item_t*)pbuffer, 1);
#elif (MAPLE_TV_STANDAR == MAPLE_TV_DVB_T)
            dvb_add_serv_item((maple_serv_item_t*)pbuffer, 1);
#elif (MAPLE_TV_STANDAR == MAPLE_TV_DTMB)
            dtmb_add_serv_item((maple_serv_item_t*)pbuffer, 1);
#elif (MAPLE_TV_STANDAR == MAPLE_TV_ATSC)
            atsc_add_serv_item((maple_serv_item_t*)pbuffer, 1);
#elif (MAPLE_TV_STANDAR == MAPLE_TV_DVB_T2)
            dvbt2_add_serv_item((maple_serv_item_t*)pbuffer, 1);
#else
            isdb_add_serv_item((maple_serv_item_t*)pbuffer, 1);
#endif
            result = EPDK_OK;
            break;
        }
        
        case MAPLE_CMD_SERV_ITEM_DELETE:            //* aux = item channel id, pbuffer = ;
        {
#if (MAPLE_TV_STANDAR == MAPLE_TV_CMMB)
            cmmb_delete_serv_item(aux);
#elif (MAPLE_TV_STANDAR == MAPLE_TV_DVB_T)
            dvb_delete_serv_item(aux);
#elif (MAPLE_TV_STANDAR == MAPLE_TV_DTMB)
            dtmb_delete_serv_item(aux);
#elif (MAPLE_TV_STANDAR == MAPLE_TV_ATSC)
            atsc_delete_serv_item(aux);
#elif (MAPLE_TV_STANDAR == MAPLE_TV_DVB_T2)
            dvbt2_delete_serv_item(aux);
#else
            isdb_delete_serv_item(aux);
#endif
            result = EPDK_OK;
            break;
        }
        
        case MAPLE_CMD_SERV_ITEM_SET_FAVOURATE:     //* aux = item channel id, pbuffer = 0 or 1 to clear or set favourate flag;
        {
#if (MAPLE_TV_STANDAR == MAPLE_TV_CMMB)
            cmmb_set_service_item_favourate_flag(aux, (__u32)pbuffer);
#elif (MAPLE_TV_STANDAR == MAPLE_TV_DVB_T)
            dvb_set_service_item_favourate_flag(aux, (__u32)pbuffer);
#elif (MAPLE_TV_STANDAR == MAPLE_TV_DTMB)
            dtmb_set_service_item_favourate_flag(aux, (__u32)pbuffer);
#elif (MAPLE_TV_STANDAR == MAPLE_TV_ATSC)
            atsc_set_service_item_favourate_flag(aux, (__u32)pbuffer);
#elif (MAPLE_TV_STANDAR == MAPLE_TV_DVB_T2)
            dvbt2_set_service_item_favourate_flag(aux, (__u32)pbuffer);
#else
            isdb_set_service_item_favourate_flag(aux, (__u32)pbuffer);
#endif
            result = EPDK_OK;
            break;
        }
        
        case MAPLE_CMD_SERV_ITEM_SET_LOCK:
        {
#if (MAPLE_TV_STANDAR == MAPLE_TV_CMMB)
            cmmb_set_service_item_lock(aux, (__s32)pbuffer);
#elif (MAPLE_TV_STANDAR == MAPLE_TV_DVB_T)
            dvb_set_service_item_lock(aux, (__s32)pbuffer);
#elif (MAPLE_TV_STANDAR == MAPLE_TV_DTMB)
            dtmb_set_service_item_lock(aux, (__s32)pbuffer);
#elif (MAPLE_TV_STANDAR == MAPLE_TV_ATSC)
            atsc_set_service_item_lock(aux, (__s32)pbuffer);
#elif (MAPLE_TV_STANDAR == MAPLE_TV_DVB_T2)
            dvbt2_set_service_item_lock(aux, (__s32)pbuffer);
#else
            isdb_set_service_item_lock(aux, (__s32)pbuffer);
#endif
            result = EPDK_OK;
            break;
        }
        
        case MAPLE_CMD_SL_GET_SERV_NUM:
        {
#if (MAPLE_TV_STANDAR == MAPLE_TV_CMMB)
            return cmmb_get_service_num();
#elif (MAPLE_TV_STANDAR == MAPLE_TV_DVB_T)
            return dvb_get_service_num();
#elif (MAPLE_TV_STANDAR == MAPLE_TV_DTMB)
            return dtmb_get_service_num();
#elif (MAPLE_TV_STANDAR == MAPLE_TV_ATSC)
            return atsc_get_service_num();
#elif (MAPLE_TV_STANDAR == MAPLE_TV_DVB_T2)
            return dvbt2_get_service_num();
#else
            return isdb_get_service_num();
#endif
        }
        
        case MAPLE_CMD_SL_GET_CUR_SERV_CHAN_ID:
        {
#if (MAPLE_TV_STANDAR == MAPLE_TV_CMMB)
            return cmmb_get_cur_service_id();
#elif (MAPLE_TV_STANDAR == MAPLE_TV_DVB_T)
            return dvb_get_cur_service_id();
#elif (MAPLE_TV_STANDAR == MAPLE_TV_DTMB)
            return dtmb_get_cur_service_id();
#elif (MAPLE_TV_STANDAR == MAPLE_TV_ATSC)
            return atsc_get_cur_service_id();
#elif (MAPLE_TV_STANDAR == MAPLE_TV_DVB_T2)
            return dvbt2_get_cur_service_id();
#else
            return isdb_get_cur_service_id();
#endif
        }
        case MAPLE_CMD_SORT_SERV_LIST:
        {
            result = EPDK_OK;
#if (MAPLE_TV_STANDAR == MAPLE_TV_CMMB)
#elif (MAPLE_TV_STANDAR == MAPLE_TV_DVB_T)
						result = dvb_sort_list(aux);
#elif (MAPLE_TV_STANDAR == MAPLE_TV_DTMB)
						result = dtmb_sort_list(aux);
#elif (MAPLE_TV_STANDAR == MAPLE_TV_ATSC)
						result = atsc_sort_list(aux);
#elif (MAPLE_TV_STANDAR == MAPLE_TV_DVB_T2)
						result = dvbt2_sort_list(aux);
#else
            result = isdb_sort_list(aux);
#endif
            break;
        }
        case MAPLE_CMD_CB_SET:              //* set a callback function
        {
            if(aux == MAPLE_CB_SRCH_SL_EVENT)
            {
                maple->sl_event_cb = (__pCBK_t)pbuffer;
                result = EPDK_OK;
            }
            else if(aux == MAPLE_CB_SRCH_SL_FINISH)
            {
                maple->sl_finish_cb = (__pCBK_t)pbuffer;
                result = EPDK_OK;
            }
            else if(aux == MAPLE_CB_SRCH_SEPG_FINISH)
            {
                maple->sepg_finish_cb = (__pCBK_t)pbuffer;
                result = EPDK_OK;
            }
            else if(aux == MAPLE_CB_SRCH_SCHDL_EVENT)
            {
                maple->schdl_event_cb = (__pCBK_t)pbuffer;
                result = EPDK_OK;
            }
            else if(aux == MAPLE_CB_SRCH_SCHDL_FINISH)
            {
                maple->schdl_finish_cb = (__pCBK_t)pbuffer;
                result = EPDK_OK;
            }
            else if(aux == MAPLE_CB_SIGNAL_LOCK_EVENT)
            {
                maple->lock_cb = (__pCBK_t)pbuffer;
                if(maple->lock_cb == NULL)
                {
                    esMODS_MIoctrl(maple->demod, DEMOD_UNLOCK, 0, NULL);
                }
                else
                {
                    esMODS_MIoctrl(maple->demod, DEMOD_SET_LOCK_CB, (__u32)maple->lock_event_cb_param, (void*)maple->lock_cb);
                    esMODS_MIoctrl(maple->demod, DEMOD_LOCK, 0, NULL);
                }
                
                result = EPDK_OK;
            }
#if (MAPLE_TV_STANDAR == MAPLE_TV_CMMB)     
            else if(aux == MAPLE_CB_CMMB_DATASVC_BEGIN)
            {
                maple->cmmb_data_svc_update_begin_cb = (__pCBK_t)pbuffer;
                result = EPDK_OK;
            }
            else if(aux == MAPLE_CB_CMMB_DATASVC_END)
            {
                maple->cmmb_data_svc_update_begin_cb = (__pCBK_t)pbuffer;
                result = EPDK_OK;
            }
#endif
            else
            {
                result = EPDK_FAIL;
            }
            
            break;
        }
        
        case MAPLE_CMD_CB_SET_PARAM:            //* set parameter to be passed when callback
        {
            if(aux == MAPLE_CB_SRCH_SL_EVENT)
            {
                maple->sl_event_cb_param = (maple_sl_cb_param_t*)pbuffer;
                result = EPDK_OK;
            }
            else if(aux == MAPLE_CB_SRCH_SL_FINISH)
            {
                maple->sl_finish_cb_param = (maple_sl_cb_param_t*)pbuffer;
                result = EPDK_OK;
            }
            else if(aux == MAPLE_CB_SRCH_SEPG_FINISH)
            {
                maple->sepg_cb_param = (maple_sepg_cb_param_t*)pbuffer;
                result = EPDK_OK;
            }
            else if(aux == MAPLE_CB_SRCH_SCHDL_EVENT)
            {
                maple->schdl_event_cb_param = (maple_schdl_cb_param_t*)pbuffer;
                result = EPDK_OK;
            }
            else if(aux == MAPLE_CB_SRCH_SCHDL_FINISH)
            {
                maple->schdl_finish_cb_param = (maple_schdl_cb_param_t*)pbuffer;
                result = EPDK_OK;
            }
            else if(aux == MAPLE_CB_SIGNAL_LOCK_EVENT)
            {
                maple->lock_event_cb_param = (maple_lock_param_t*)pbuffer;
                result = EPDK_OK;
            }
#if (MAPLE_TV_STANDAR == MAPLE_TV_CMMB)     
            else if(aux == MAPLE_CB_CMMB_DATASVC_BEGIN)
            {
                maple->cmmb_datasvc_updatebegin_cb_param = (maple_cmmb_datasvc_cb_param_t *)pbuffer;
                result = EPDK_OK;
            }
            else if(aux == MAPLE_CB_CMMB_DATASVC_END)
            {
                maple->cmmb_datasvc_updateend_cb_param = (maple_cmmb_datasvc_cb_param_t *)pbuffer;
                result = EPDK_OK;
            }
#endif
            else
            {
                result = EPDK_FAIL;
            }
            break;
        }
        
        case MAPLE_CMD_SET_DEFAULT_CHAR_CODING_TYPE:
        {
            //aux = EPDK_CHARSET_ENM_ISO_8859_15;
            maple->charCodeType = aux;
#if (MAPLE_TV_STANDAR == MAPLE_TV_CMMB)
            cmmb_set_default_char_code_type((__epdk_charset_enm_e)aux);
#elif (MAPLE_TV_STANDAR == MAPLE_TV_DVB_T)
            dvb_set_default_char_code_type((__epdk_charset_enm_e)aux);
#elif (MAPLE_TV_STANDAR == MAPLE_TV_DTMB)
            dtmb_set_default_char_code_type((__epdk_charset_enm_e)aux);
#elif (MAPLE_TV_STANDAR == MAPLE_TV_ATSC)
            atsc_set_default_char_code_type((__epdk_charset_enm_e)aux);
#elif (MAPLE_TV_STANDAR == MAPLE_TV_DVB_T2)
            dvbt2_set_default_char_code_type((__epdk_charset_enm_e)aux);
#else
            isdb_set_default_char_code_type((__epdk_charset_enm_e)aux);
#endif
            result = EPDK_OK;
            break;
        }
        
        case MAPLE_CMD_SRCH_SERV:               //* start a task to search service
        {
            //* aux = bandwidth_in_10KHz<<22 | frequency_in_KHz, 
            //* pbuffer = pointer to maple_serv_list_t;
            __u32  tmp = (__u32)aux;
            demodParam.bandwidth   = (tmp >> 22) * 10;  //* in KHz
            demodParam.frequency   = tmp & 0x3fffff;    //* in KHz
            
#if (MAPLE_TV_STANDAR == MAPLE_TV_CMMB)
            if(cmmb_search_service(&demodParam, (maple_serv_list_t*)pbuffer) == 0)
                result = EPDK_OK;
            else
                result = EPDK_FAIL;
#elif (MAPLE_TV_STANDAR == MAPLE_TV_DVB_T)
            if(dvb_search_service(&demodParam, (maple_serv_list_t*)pbuffer) == 0)
                result = EPDK_OK;
            else
                result = EPDK_FAIL;
#elif (MAPLE_TV_STANDAR == MAPLE_TV_DTMB)
            if(dtmb_search_service(&demodParam, (maple_serv_list_t*)pbuffer) == 0)
                result = EPDK_OK;
            else
                result = EPDK_FAIL;
#elif (MAPLE_TV_STANDAR == MAPLE_TV_ATSC)
            if(atsc_search_service(&demodParam, (maple_serv_list_t*)pbuffer) == 0)
                result = EPDK_OK;
            else
                result = EPDK_FAIL;
#elif (MAPLE_TV_STANDAR == MAPLE_TV_DVB_T2)
            if(dvbt2_search_service(&demodParam, (maple_serv_list_t*)pbuffer) == 0)
                result = EPDK_OK;
            else
                result = EPDK_FAIL;
#else
            if(isdb_search_service(&demodParam, (maple_serv_list_t*)pbuffer) == 0)
                result = EPDK_OK;
            else
                result = EPDK_FAIL;
#endif
            break;
        }
        
        case MAPLE_CMD_SRCH_SERV_STOP:          //* break service searching
        {
#if (MAPLE_TV_STANDAR == MAPLE_TV_CMMB)
                if(cmmb_stop_service_search() == 0)
                    result = EPDK_OK;
                else
                    result = EPDK_FAIL;
#elif (MAPLE_TV_STANDAR == MAPLE_TV_DVB_T)
                if(dvb_stop_service_search() == 0)
                    result = EPDK_OK;
                else
                    result = EPDK_FAIL;
#elif (MAPLE_TV_STANDAR == MAPLE_TV_DTMB)
                if(dtmb_stop_service_search() == 0)
                    result = EPDK_OK;
                else
                    result = EPDK_FAIL;
#elif (MAPLE_TV_STANDAR == MAPLE_TV_ATSC)
                if(atsc_stop_service_search() == 0)
                    result = EPDK_OK;
                else
                    result = EPDK_FAIL;
#elif (MAPLE_TV_STANDAR == MAPLE_TV_DVB_T2)
                if(dvbt2_stop_service_search() == 0)
                    result = EPDK_OK;
                else
                    result = EPDK_FAIL;
#else
                if(isdb_stop_service_search() == 0)
                    result = EPDK_OK;
                else
                    result = EPDK_FAIL;
#endif
            break;
        }
        
        #if 0
        case MAPLE_CMD_SRCH_SEPG:               //* start a task to search simple EPG
        {
            //* aux = service channel id, pbuffer = pointer to maple_sepg_t;
#if (MAPLE_TV_STANDAR == MAPLE_TV_CMMB)
            if(cmmb_search_sepg(aux, (maple_sepg_t*)pbuffer) == 0)
                result = EPDK_OK;
            else
                result = EPDK_FAIL;
#elif (MAPLE_TV_STANDAR == MAPLE_TV_DVB_T)
            if(dvb_search_sepg(aux, (maple_sepg_t*)pbuffer) == 0)
                result = EPDK_OK;
            else
                result = EPDK_FAIL;
#else
            if(isdb_search_sepg(aux, (maple_sepg_t*)pbuffer) == 0)
                result = EPDK_OK;
            else
                result = EPDK_FAIL;
#endif
            break;
        }
        
        case MAPLE_CMD_SRCH_SEPG_STOP:          //* break simple EPG searching
        {
#if (MAPLE_TV_STANDAR == MAPLE_TV_CMMB)
            if(cmmb_stop_sepg_search() == 0)
                result = EPDK_OK;
            else
                result = EPDK_FAIL;
#elif (MAPLE_TV_STANDAR == MAPLE_TV_DVB_T)
            if(dvb_stop_sepg_search() == 0)
                result = EPDK_OK;
            else
                result = EPDK_FAIL;
#else
            if(isdb_stop_sepg_search() == 0)
                result = EPDK_OK;
            else
                result = EPDK_FAIL;
#endif
            break;
        }
        
        case MAPLE_CMD_SRCH_SCHDL:              //* start a task to search EPG schedule
        {
            //* aux = service channel id, pbuffer = pointer to schedule
#if (MAPLE_TV_STANDAR == MAPLE_TV_CMMB)
            if(cmmb_search_schedule(aux, (maple_schdl_list_t*)pbuffer) == 0)
                result = EPDK_OK;
            else
                result = EPDK_FAIL;
#elif (MAPLE_TV_STANDAR == MAPLE_TV_DVB_T)
            if(dvb_search_schedule(aux, (maple_schdl_list_t*)pbuffer) == 0)
                result = EPDK_OK;
            else
                result = EPDK_FAIL;
#else
            if(isdb_search_schedule(aux, (maple_schdl_list_t*)pbuffer) == 0)
                result = EPDK_OK;
            else
                result = EPDK_FAIL;
#endif
                
            break;
        }
        
        case MAPLE_CMD_SRCH_SCHDL_STOP:         //* break schedule searching
        {
#if (MAPLE_TV_STANDAR == MAPLE_TV_CMMB)
            if(cmmb_stop_schedule_search() == 0)
                result = EPDK_OK;
            else
                result = EPDK_FAIL;
#elif (MAPLE_TV_STANDAR == MAPLE_TV_DVB_T)
            if(dvb_stop_schedule_search() == 0)
                result = EPDK_OK;
            else
                result = EPDK_FAIL;
#else
            if(isdb_stop_schedule_search() == 0)
                result = EPDK_OK;
            else
                result = EPDK_FAIL;
#endif
            break;
        }
  #endif
        
        case MAPLE_CMD_EPG_MONITOR_START:      //* start epg monitor
        {
#if (MAPLE_TV_STANDAR == MAPLE_TV_CMMB)
            if(cmmb_open_epg_monitor() == 0)
                result = EPDK_OK;
            else
                result = EPDK_FAIL;
#elif (MAPLE_TV_STANDAR == MAPLE_TV_DVB_T)
            if(dvb_open_epg_monitor() == 0)
                result = EPDK_OK;
            else
                result = EPDK_FAIL;
#elif (MAPLE_TV_STANDAR == MAPLE_TV_DTMB)
            if(dtmb_open_epg_monitor() == 0)
                result = EPDK_OK;
            else
                result = EPDK_FAIL;
#elif (MAPLE_TV_STANDAR == MAPLE_TV_ATSC)
            if(atsc_open_epg_monitor() == 0)
                result = EPDK_OK;
            else
                result = EPDK_FAIL;
#elif (MAPLE_TV_STANDAR == MAPLE_TV_DVB_T2)
            if(dvbt2_open_epg_monitor() == 0)
                result = EPDK_OK;
            else
                result = EPDK_FAIL;
#else
            if(isdb_open_epg_monitor() == 0)
                result = EPDK_OK;
            else
                result = EPDK_FAIL;
#endif
            break;
        }
        
        case MAPLE_CMD_EPG_MONITOR_STOP:       //* stop epg monitor
        {
#if (MAPLE_TV_STANDAR == MAPLE_TV_CMMB)
            if(cmmb_close_epg_monitor() == 0)
                result = EPDK_OK;
            else
                result = EPDK_FAIL;
#elif (MAPLE_TV_STANDAR == MAPLE_TV_DVB_T)
            if(dvb_close_epg_monitor() == 0)
                result = EPDK_OK;
            else
                result = EPDK_FAIL;
#elif (MAPLE_TV_STANDAR == MAPLE_TV_DTMB)
            if(dtmb_close_epg_monitor() == 0)
                result = EPDK_OK;
            else
                result = EPDK_FAIL;
#elif (MAPLE_TV_STANDAR == MAPLE_TV_ATSC)
            if(atsc_close_epg_monitor() == 0)
                result = EPDK_OK;
            else
                result = EPDK_FAIL;
#elif (MAPLE_TV_STANDAR == MAPLE_TV_DVB_T2)
            if(dvbt2_close_epg_monitor() == 0)
                result = EPDK_OK;
            else
                result = EPDK_FAIL;
#else
            if(isdb_close_epg_monitor() == 0)
                result = EPDK_OK;
            else
                result = EPDK_FAIL;
#endif
            break;
        }
        
        case MAPLE_CMD_GET_SEPG:               //* get simple epg of specified service
        {
#if (MAPLE_TV_STANDAR == MAPLE_TV_CMMB)
            if(cmmb_get_sepg(aux, (maple_sepg_t*)pbuffer) == 0)
                result = EPDK_OK;
            else
                result = EPDK_FAIL;
#elif (MAPLE_TV_STANDAR == MAPLE_TV_DVB_T)
            if(dvb_get_sepg(aux, (maple_sepg_t*)pbuffer) == 0)
                result = EPDK_OK;
            else
                result = EPDK_FAIL;
#elif (MAPLE_TV_STANDAR == MAPLE_TV_DTMB)
            if(dtmb_get_sepg(aux, (maple_sepg_t*)pbuffer) == 0)
                result = EPDK_OK;
            else
                result = EPDK_FAIL;
#elif (MAPLE_TV_STANDAR == MAPLE_TV_ATSC)
            if(atsc_get_sepg(aux, (maple_sepg_t*)pbuffer) == 0)
                result = EPDK_OK;
            else
                result = EPDK_FAIL;
#elif (MAPLE_TV_STANDAR == MAPLE_TV_DVB_T2)
            if(dvbt2_get_sepg(aux, (maple_sepg_t*)pbuffer) == 0)
                result = EPDK_OK;
            else
                result = EPDK_FAIL;
#else
            if(isdb_get_sepg(aux, (maple_sepg_t*)pbuffer) == 0)
                result = EPDK_OK;
            else
                result = EPDK_FAIL;
#endif
            break;
        }
        
        case MAPLE_CMD_GET_SCHEDULE:           //* get simple schedule of specified service
        {
#if (MAPLE_TV_STANDAR == MAPLE_TV_CMMB)
            if(cmmb_get_schedule(aux, (maple_schdl_list_t*)pbuffer) == 0)
                result = EPDK_OK;
            else
                result = EPDK_FAIL;
#elif (MAPLE_TV_STANDAR == MAPLE_TV_DVB_T)
            if(dvb_get_schedule(aux, (maple_schdl_list_t*)pbuffer) == 0)
                result = EPDK_OK;
            else
                result = EPDK_FAIL;
#elif (MAPLE_TV_STANDAR == MAPLE_TV_DTMB)
            if(dtmb_get_schedule(aux, (maple_schdl_list_t*)pbuffer) == 0)
                result = EPDK_OK;
            else
                result = EPDK_FAIL;
#elif (MAPLE_TV_STANDAR == MAPLE_TV_ATSC)
            if(atsc_get_schedule(aux, (maple_schdl_list_t*)pbuffer) == 0)
                result = EPDK_OK;
            else
                result = EPDK_FAIL;						
#elif (MAPLE_TV_STANDAR == MAPLE_TV_DVB_T2)
            if(dvbt2_get_schedule(aux, (maple_schdl_list_t*)pbuffer) == 0)
                result = EPDK_OK;
            else
                result = EPDK_FAIL;
#else
            if(isdb_get_schedule(aux, (maple_schdl_list_t*)pbuffer) == 0)
                result = EPDK_OK;
            else
                result = EPDK_FAIL;
#endif
            break;
        }
        
        case MAPLE_CMD_TIME_MONITOR_START:      //* start tv time monitor
        {
#if (MAPLE_TV_STANDAR == MAPLE_TV_CMMB)
            if(cmmb_open_time_monitor() == 0)
                result = EPDK_OK;
            else
                result = EPDK_FAIL;
#elif (MAPLE_TV_STANDAR == MAPLE_TV_DVB_T)
            if(dvb_open_time_monitor() == 0)
                result = EPDK_OK;
            else
                result = EPDK_FAIL;
#elif (MAPLE_TV_STANDAR == MAPLE_TV_DTMB)
            if(dtmb_open_time_monitor() == 0)
                result = EPDK_OK;
            else
                result = EPDK_FAIL;
#elif (MAPLE_TV_STANDAR == MAPLE_TV_ATSC)
            if(atsc_open_time_monitor() == 0)
                result = EPDK_OK;
            else
                result = EPDK_FAIL;
#elif (MAPLE_TV_STANDAR == MAPLE_TV_DVB_T2)
            if(dvbt2_open_time_monitor() == 0)
                result = EPDK_OK;
            else
                result = EPDK_FAIL;
#else
            if(isdb_open_time_monitor() == 0)
                result = EPDK_OK;
            else
                result = EPDK_FAIL;
#endif
            break;
        }
        
        case MAPLE_CMD_TIME_MONITOR_STOP:       //* stop tv time monitor
        {
#if (MAPLE_TV_STANDAR == MAPLE_TV_CMMB)
            if(cmmb_close_time_monitor() == 0)
                result = EPDK_OK;
            else
                result = EPDK_FAIL;
#elif (MAPLE_TV_STANDAR == MAPLE_TV_DVB_T)
            if(dvb_close_time_monitor() == 0)
                result = EPDK_OK;
            else
                result = EPDK_FAIL;
#elif (MAPLE_TV_STANDAR == MAPLE_TV_DTMB)
            if(dtmb_close_time_monitor() == 0)
                result = EPDK_OK;
            else
                result = EPDK_FAIL;
#elif (MAPLE_TV_STANDAR == MAPLE_TV_ATSC)
            if(atsc_close_time_monitor() == 0)
                result = EPDK_OK;
            else
                result = EPDK_FAIL;
#elif (MAPLE_TV_STANDAR == MAPLE_TV_DVB_T2)
            if(dvbt2_close_time_monitor() == 0)
                result = EPDK_OK;
            else
                result = EPDK_FAIL;
#else
            if(isdb_close_time_monitor() == 0)
                result = EPDK_OK;
            else
                result = EPDK_FAIL;
#endif
            break;
        }
        
        case MAPLE_CMD_TIME_GET:                //* get the current stream time
        {
            //* pbuffer = pointer to maple_time_t;
#if (MAPLE_TV_STANDAR == MAPLE_TV_CMMB)
            if(cmmb_get_time((maple_time_t*)pbuffer) == 0)
                result = EPDK_OK;
            else
                result = EPDK_FAIL;
#elif (MAPLE_TV_STANDAR == MAPLE_TV_DVB_T)
            if(dvb_get_time((maple_time_t*)pbuffer) == 0)
                result = EPDK_OK;
            else
                result = EPDK_FAIL;
#elif (MAPLE_TV_STANDAR == MAPLE_TV_DTMB)
            if(dtmb_get_time((maple_time_t*)pbuffer) == 0)
                result = EPDK_OK;
            else
                result = EPDK_FAIL;
#elif (MAPLE_TV_STANDAR == MAPLE_TV_ATSC)
            if(atsc_get_time((maple_time_t*)pbuffer) == 0)
                result = EPDK_OK;
            else
                result = EPDK_FAIL;
#elif (MAPLE_TV_STANDAR == MAPLE_TV_DVB_T2)
            if(dvbt2_get_time((maple_time_t*)pbuffer) == 0)
                result = EPDK_OK;
            else
                result = EPDK_FAIL;
#else
            if(isdb_get_time((maple_time_t*)pbuffer) == 0)
                result = EPDK_OK;
            else
                result = EPDK_FAIL;
#endif
            break;
        }
        case MAPLE_CMD_GET_STATUS:
        {
            result = maple->status;
            break;
        }
		 
        case MAPLE_CMD_PLAY:
        {
						
            if(maple->status == MAPLE_STAT_PLAY)
            {
                eDbug("maple->status already MAPLE_STAT_PLAY\n");
                result = EPDK_OK;
            }
            else if(maple->status == MAPLE_STAT_STOP)
            {
                if(playService(aux, (__u32)pbuffer) == 0)
                {
                    maple->status = MAPLE_STAT_PLAY;
                    result = EPDK_OK;
                }
                else
                {
                    result = EPDK_FAIL;
                }
            }
            else
            {
                eDbug("maple->status=[0x%x], don't playService(), check code!\n", maple->status);
                result = EPDK_OK;
            }
            break;
        }
        
        case MAPLE_CMD_STOP:
        {
            if(maple->status == MAPLE_STAT_STOP)    //如果maple的状态已经是stop了，就直接返回
            {
                eDbug("stop done!\n");
                result = EPDK_OK;
                break;
            }
            if(0 == maple->maintskprio)
            {
                eDbug("impossible! maple maintask is not exist!\n");
                result = EPDK_FAIL;
                break;
            }
            //通知maple maintask 去stop
            esKRNL_QPost(maple->CmdQ, (void *)DVBC_MAINTASK_CMD_STOP);

            //等待 maple maintask stop done
            while(1)
            {
                maple->pSemStopPlayDone->SemPend(maple->pSemStopPlayDone, 50, &err);    //500ms为一个单位
                if(err == OS_NO_ERR)
                {
                    break;
                }
                else
                {
                    eDbug("timeout? err=[0x%x]\n", err);
                }
            }
            if(maple->status == MAPLE_STAT_STOP)    //如果maple的状态已经是stop了，就直接返回
            {
                result = EPDK_OK;
            }
            else
            {
                eDbug("why after stop, maple status = [0x%x]\n", maple->status);
                result = EPDK_FAIL;
            }
            break;
//            if(stopPlay() == 0)
//            {
//                result = EPDK_OK;
//            }
//            else
//                result = EPDK_FAIL;
//            break;
        }
        
        case MAPLE_CMD_SET_PREVIEW_MODE:
        {
            result = EPDK_FAIL;
            break;
        }

	 case MAPLE_CMD_SET_SWGAIN:
        {
            if(esMODS_MIoctrl(maple->player, DVB_CMD_SET_SWGAIN, aux, 0) == EPDK_OK)
                result = EPDK_OK;
            else
                result = EPDK_FAIL;
                
            break;
        }
		
	 case MAPLE_CMD_GET_SWGAIN:
        {
            return esMODS_MIoctrl(maple->player, DVB_CMD_GET_SWGAIN, 0, 0);
        }
		
        case MAPLE_CMD_SET_VID_LAYER:
        {
            if(esMODS_MIoctrl(maple->player, DVB_CMD_SET_VIDEO_LAYER, 0, pbuffer) == EPDK_OK)
                result = EPDK_OK;
            else
                result = EPDK_FAIL;
                
            break;
        }
        
        case MAPLE_CMD_SET_VID_WINDOW:
        {
            if(esMODS_MIoctrl(maple->player, DVB_CMD_SET_VIDEO_WINDOW, 0, pbuffer) == EPDK_OK)
                result = EPDK_OK;
            else
                result = EPDK_FAIL;
                
            break;
        }
        
        case MAPLE_CMD_GET_VID_WINDOW:
        {
            result = esMODS_MIoctrl(maple->player, DVB_CMD_GET_VIDEO_WINDOW, aux, pbuffer);
            break;
        }
        
        case MAPLE_CMD_SET_VID_SHOW_MODE:
        {
            if(aux == MAPLE_VID_WINDOW_NOTCARE)
                showMode = VID_WINDOW_NOTCARE;
            else if(aux == MAPLE_VID_WINDOW_ORIGINAL)
                showMode = VID_WINDOW_ORIGINAL;
            else if(aux == MAPLE_VID_WINDOW_BESTSHOW)
                showMode = VID_WINDOW_BESTSHOW;
            else if(aux == MAPLE_VID_WINDOW_FULLSCN)
                showMode = VID_WINDOW_FULLSCN;
            else if(aux == MAPLE_VID_WINDOW_4R3MODE)
                showMode = VID_WINDOW_4R3MODE;
            else if(aux == MAPLE_VID_WINDOW_16R9MODE)
                showMode = VID_WINDOW_16R9MODE;
            else if(aux == MAPLE_VID_WINDOW_CUTEDGE)
                showMode = VID_WINDOW_CUTEDGE;
            else if(aux == MAPLE_VID_WINDOW_USERDEF)
                showMode = VID_WINDOW_USERDEF;
            else
                return EPDK_FAIL;
                
            result = esMODS_MIoctrl(maple->player, DVB_CMD_SET_VIDEO_SHOW_MODE, showMode, pbuffer);
            break;
        }
        
        case MAPLE_CMD_GET_VID_SHOW_MODE:
        {
            result = VID_WINDOW_NOTCARE;
            break;
        }
        
        case MAPLE_CMD_SET_VIDEO_LYR_ALPHA_VALUE:
        {
            esMODS_MIoctrl(maple->player, DVB_CMD_SET_VIDEO_LYR_ALPHA, aux, NULL);
            result = EPDK_OK;
            break;
        }
        
        case MAPLE_CMD_SET_VIDEO_LYR_ALPHA_ON:
        {
            esMODS_MIoctrl(maple->player, DVB_CMD_SET_VIDEO_LYR_ALPHA_ON, 0, NULL);
            result = EPDK_OK;
            break;
        }
        
        case MAPLE_CMD_SET_VIDEO_LYR_ALPHA_OFF:
        {
            esMODS_MIoctrl(maple->player, DVB_CMD_SET_VIDEO_LYR_ALPHA_OFF, 0, NULL);
            result = EPDK_OK;
            break;
        }
        case MAPLE_CMD_CONFIG_VIDEO_LYR_CK_ENABLE:
        {
            esMODS_MIoctrl(maple->player, DVB_CMD_CONFIG_VIDEO_LYR_CK_ENABLE, aux, NULL);
            result = EPDK_OK;
            break;
        }
        case MAPLE_CMD_ENABLE_DEINTERLACE:
        {
            esMODS_MIoctrl(maple->player, DVB_CMD_ENABLE_DEINTERLACE, aux, NULL);
            result = EPDK_OK;
            break;
        }
        case MAPLE_CMD_GET_FRAMESIZE:
        {
            result = esMODS_MIoctrl(maple->player, DVB_CMD_GET_FRAMESIZE, 0, NULL);
            break;
        }
        case MAPLE_CMD_CA_GET_SN:
        {
#if(MAPLE_TV_STANDAR == MAPLE_TV_ISDB_T) 
            __s32   tmpret;
            __s32 ret = esMODS_MIoctrl(maple->demod, DEMOD_GET_CA_SN, aux, pbuffer);
            if(0 == ret)
            {
                result = EPDK_OK;
            }
            else if(1 == ret)
            {
                __s32		volume;
                FILE		*p_dac;
                eDbug("maple need reset demod!\n");
                //forbit audio
		        		p_dac = eLIBs_fopen("b:\\AUDIO\\PLAY", "r+");   //fopen("b:\\AUDIO\\PLAY0", "r+");
		        		if(!p_dac)
		        		{
		        			__err("open dac device fail!\n");
		        			result = EPDK_FAIL;
		                    goto __GET_SN_exit;
		        		}
                volume = eLIBs_fioctrl( p_dac, AUDIO_DEV_CMD_GET_VOLUME, 0, 0); 
        				eLIBs_fioctrl(p_dac, AUDIO_DEV_CMD_SET_VOLUME, 0, 0);	//静音	
                tmpret = esMODS_MIoctrl(maple->demod, DEMOD_RESET_BCAS, 0, NULL);
                if(tmpret!=EPDK_OK)
                {
                    eDbug("why rest BCAS fail?\n");
                }
                //restore audio
                eLIBs_fioctrl(p_dac, AUDIO_DEV_CMD_SET_VOLUME, volume, 0);	//restore
                eLIBs_fclose(p_dac);
                result = EPDK_OK;
            }
            else
            {
                result = EPDK_FAIL;
            }
        __GET_SN_exit:
            break;
#elif(MAPLE_TV_STANDAR == MAPLE_TV_CMMB)
						esMODS_MIoctrl(maple->demod, DEMOD_GET_CA_SN, aux, pbuffer);
            result = EPDK_OK;
            break;
#elif(MAPLE_TV_STANDAR == MAPLE_TV_DTMB)
						eDbug("dtmb get ca num fail!\n");
						break;

#elif(MAPLE_TV_STANDAR == MAPLE_TV_ATSC)
					eDbug("atsc get ca num fail!\n");
						break;

#elif(MAPLE_TV_STANDAR == MAPLE_TV_DVB_T2)
					eDbug("dvbt2 get ca num fail!\n");
						break;

#else//dvbt
						eDbug("dvbt get ca num fail!\n");
						break;
#endif
        }
				case MAPLE_CMD_GET_VE_STATUS:
				{
					// maple_ve_status_e maple_ve_status;
					result = NULL;                  
					if(maple->player)
					{
					    result = esMODS_MIoctrl(maple->player, DVB_CMD_GET_VE_STATUS, 0, NULL);
					}
                    
					if(NULL == result)
					{
					    eLIBs_memset(maple_ve_status, 0, sizeof(maple_ve_status_e));
					}
					else
					{
					 	*maple_ve_status = *(maple_ve_status_e*)result;
					}
					
    				 eDbug("ve_status:ve_errcnt =%d,ve_succnt =%d,ve_errflg =%d\n",
    				 		maple_ve_status->ve_errcnt,maple_ve_status->ve_succnt,maple_ve_status->ve_errflg);
					 
           			return (__s32)maple_ve_status;
				}
				case MAPLE_CMD_SET_RESERVED_MEM_SIZE:
				{
					 result = esMODS_MIoctrl(maple->player, DVB_CMD_SET_RESERVED_MEM_SIZE, aux, NULL);
            break;
				}
				case MAPLE_CMD_SET_TV_MODE:
				{
					 result = esMODS_MIoctrl(maple->player, DVB_CMD_SET_TV_MODE, aux, NULL);
            break;
				}
				case MAPLE_CMD_SET_ROTATE:
				{
					 result = esMODS_MIoctrl(maple->player, DVB_CMD_SET_ROTATE, aux, NULL);
            break;
				}
        case MAPLE_CMD_CHECK_PICTURE_OUT:
        {
            result = esMODS_MIoctrl(maple->player, DVB_CMD_CHECK_PICTURE_OUT, 0, NULL);
            break;
        }
        case MAPLE_CMD_GET_SUBTITLE_CNT:
        {
            __u32   channelId = aux;
            maple_serv_item_t   servItem;
            memset(&servItem, 0, sizeof(maple_serv_item_t));
            result = -1;
#if (MAPLE_TV_STANDAR == MAPLE_TV_CMMB)
            result = cmmb_get_service_item(&servItem, channelId);
#elif (MAPLE_TV_STANDAR == MAPLE_TV_DVB_T)
            result = dvb_get_service_item(&servItem, channelId);
#elif (MAPLE_TV_STANDAR == MAPLE_TV_DTMB)
            result = dtmb_get_service_item(&servItem, channelId);
#elif (MAPLE_TV_STANDAR == MAPLE_TV_ATSC)
            result = atsc_get_service_item(&servItem, channelId);
#elif (MAPLE_TV_STANDAR == MAPLE_TV_DVB_T2)
            result = dvbt2_get_service_item(&servItem, channelId);
#else
            result = isdb_get_service_item(&servItem, channelId);
#endif
            if(result != 0)
            {
                result = -1;
            }
            else
            {
                result = servItem.subtitleStreamNum;
            }
            break;
        }
        case MAPLE_CMD_SET_RECORD_MEDIAFILE:
        {
            MEM_CPY((void *)&maple->record_media_file, pbuffer, sizeof(CedarHerbMediaFileInf));
            result = EPDK_OK;
            break;
        }
        case MAPLE_CMD_RECORD:
        {
            if(maple->nRecordStatus == MAPLE_STAT_RECORD)
            {
                if(maple->nRecordProgId == aux)
                {
                    eDbug("maple->nRecordStatus already MAPLE_STAT_RECORD, record ProgId[%d], return!\n", maple->nRecordProgId);
                    result = EPDK_OK;
                }
                else
                {
                    eDbug("maple record progId[%d] != [%d], error happen, check code!\n", maple->nRecordProgId, aux);
                    result = EPDK_FAIL;
                }
            }
            else if(maple->nRecordStatus == MAPLE_STAT_STOP)
            {
                if(RecordService(aux) == 0)
                {
                    maple->nRecordStatus = MAPLE_STAT_RECORD;
                    result = EPDK_OK;
                }
                else
                {
                    result = EPDK_FAIL;
                }
            }
            else
            {
                eDbug("maple->nRecordStatus=[0x%x], don't recordService(), check code!\n", maple->nRecordStatus);
                result = EPDK_FAIL;
            }
            break;
        }
        case MAPLE_CMD_STOP_RECORD:
        {
            if(maple->nRecordStatus == MAPLE_STAT_STOP)    //如果maple的状态已经是stop了，就直接返回
            {
                eDbug("stop record done!\n");
                result = EPDK_OK;
                break;
            }
            if(0 == maple->maintskprio)
            {
                eDbug("impossible! maple maintask is not exist!\n");
                result = EPDK_FAIL;
                break;
            }
            //通知maple maintask 去stop
            esKRNL_QPost(maple->CmdQ, (void *)DVBC_MAINTASK_CMD_STOP_RECORD);

            //等待 maple maintask stop done
            while(1)
            {
                maple->pSemStopRecordDone->SemPend(maple->pSemStopRecordDone, 50, &err);    //500ms为一个单位
                if(err == OS_NO_ERR)
                {
                    break;
                }
                else
                {
                    eDbug("timeout? err=[0x%x]\n", err);
                }
            }
            if(maple->nRecordStatus == MAPLE_STAT_STOP)    //如果maple的状态已经是stop了，就直接返回
            {
                result = EPDK_OK;
            }
            else
            {
                eDbug("why after stop, maple nRecordStatus = [0x%x]\n", maple->nRecordStatus);
                result = EPDK_FAIL;
            }
            break;
        }
        case MAPLE_CMD_GET_RECORD_STATUS:
        {
            result = maple->nRecordStatus;
            break;
        }
		case MAPLE_CMD_SET_RECORD_CB:
		{
			maple->record_cb = (__pCBK_t)pbuffer;
            eDbug("....record_disk_full_cb:%p....\n",pbuffer);
			result = EPDK_OK;
			break;
		}
        case MAPLE_CMD_GET_SERVICE_INFO:
        {
#if (MAPLE_TV_STANDAR == MAPLE_TV_CMMB)
            result = cmmb_get_av_info();
#endif
            break;
        }    
				case MAPLE_CMD_GET_SERVICE_STATUS:
				{
#if(MAPLE_TV_STANDAR == MAPLE_TV_CMMB)

						 return esMODS_MIoctrl(maple->demux, CMMB_DMX_CMD_GET_SERVICE_STATUS, 0,0);
#endif
				}

				case MAPLE_CMD_UPDATE_SERVICE_INFO: 
				{
					
					if(maple->status != MAPLE_STAT_PLAY)
          {
              eDbug("MAPLE_CMD_CHECK_SERVICE_CHANGE no MAPLE_STAT_PLAY\n");
              result = EPDK_FAIL;
          }
	       	else
	       	{

						result = UpdataServiceInfo();//返回1表示有变化，返回0表示无变化
			      eDbug("MAPLE_CMD_CHECK_SERVICE_CHANGE %d\n", result);
			      break;         
				  }						
					
				}	

				case MAPLE_CMD_SEL_AUDSTREAM:
				{
					   if(maple->status != MAPLE_STAT_PLAY)
            {
                eDbug("maple->status no MAPLE_STAT_PLAY\n");
                result = EPDK_FAIL;
            }
	       		else
	       		{
							result = selectAudstream(aux);
							if(!result)
							{
           			 eDbug("select Audstream [%d] fail!\n",result);
							}
	       		}
					break;
				}
				case MAPLE_CMD_GET_AUDSTREAM:
				{					
					return g_maple->audioIdx;
				}
				 case MAPLE_CMD_SET_SWITCH_VPLY_MODE:
        {
            result = esMODS_MIoctrl(maple->player, DVB_CMD_SET_SWITCH_VPLY_MODE, aux, NULL);
						break;
				 }
				 case MAPLE_CMD_SET_STOP_MODE:
        {
            result = esMODS_MIoctrl(maple->player, DVB_CMD_SET_STOP_MODE, aux, NULL);
						break;
				 }
        default:
        {
            eDbug("unknown maple command.\n");
            result = EPDK_FAIL;
            break;
        }
    }
    
    return result;
}

