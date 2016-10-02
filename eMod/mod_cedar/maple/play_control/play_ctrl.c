/*
*********************************************************************************************************
*                                                    eMOD
*                                   the Easy Portable/Player Operation System
*                                           mod_cedarcore sub-system
*
*                                (c) Copyright 2009-20010, Clif.Chen China
*                                               All Rights Reserved
*
* File   : dvb_ctrl.c
* Version: V1.0
* By     : clif.c
* update : 
*********************************************************************************************************
*/
#include "play_ctrl_s.h"
#include "play_ctrl.h"
#include "psr_2_adec.h"
#include "psr_2_vdec.h"
#include "psr_2_ldec.h"


#define PTS_DELTA_AUDIO	    0x21   //video pts回头时，videoclock不回头,所以在送到vdec/vdrv之前要累加.累加间隔就是0x21(33)毫秒.
#define PTS_DELTA_VIDEO	    0x21
#define PTS_DELTA_SUBTITLE  0x3e8  //字幕是根据video clock(如果没有,就是audioclock)来的.

#define PTS_ERROR_SCOPE_VIDEO	    4000    //单位:ms. 判断pts回头时,允许有误差,即后来的pts比先前的小.这里定义偏差的范围为4000ms
#define PTS_ERROR_SCOPE_AUDIO	    4000     
#define PTS_ERROR_SCOPE_SUBTITLE	1000


__dvbc_t *dvbc = NULL;

#define EPDK_CHIP_REG  (0xf1c00028)

static int HWChipSDKIsMatched(void)
{
    __u8 ChipID = 0;
    
    ChipID = (*((volatile __u8 *)(EPDK_CHIP_REG)));
    
    //read hardware chip id
    if ((ChipID == 0x01))
    {
        //chip and sdk not matched
        return EPDK_OK;
    }
    
    //chip and sdk matched
    WARNING("chipid[%x] is error! \n", ChipID);
    return EPDK_FAIL;
}
#if DRAW_RADIO_BACKGROUND
static void draw_background(void)
{
    ES_FILE*                fpPicture;
    __u32                   size;
    __disp_layer_para_t     tmpLayerAttr;
    FB                      tmpFrmBuf;
    
    
    if((dvbc != NULL) && (dvbc->setting_inf.vid_layer_hdl != NULL))
    {
        dvbc->disFile = fopen("b:\\DISP\\DISPLAY", "r+");
        if(dvbc->disFile == NULL)
        {
            WARNING("open display driver fail.\n");
            return;
        }
            
        dvbc->disLayer = dvbc->setting_inf.vid_layer_hdl;
        
        fpPicture = fopen("d:\\radio_bg.bmp", "rb");
        if(fpPicture == NULL)
        {
            WARNING("open graphic file fail.\n");
            return;
        }
            
        size = 800*480*4;
        
        dvbc->radio_bg = esMEMS_Palloc(size /1024, 0);
        if(dvbc->radio_bg == NULL)
        {
            WARNING("palloc fail.\n");
            fclose(fpPicture);
            return;
        }
        
        eLIBs_fread(dvbc->radio_bg, 1, 54, fpPicture);
        eLIBs_fread(dvbc->radio_bg, 1, 800*480*4, fpPicture);
        
        fclose(fpPicture);
        
        //* set video layer attribute
        tmpFrmBuf.fmt.type              = FB_TYPE_RGB;
        tmpFrmBuf.fmt.fmt.rgb.pixelfmt  = PIXEL_COLOR_ARGB8888;
        tmpFrmBuf.fmt.fmt.rgb.br_swap   = 0;
        tmpFrmBuf.fmt.fmt.rgb.pixseq    = 0;
        tmpFrmBuf.fmt.fmt.rgb.palette.addr    = 0;
        tmpFrmBuf.fmt.fmt.rgb.palette.size    = 0;
        
        
        tmpFrmBuf.addr[0]           = dvbc->radio_bg;
        tmpFrmBuf.addr[1]           = 0;
        tmpFrmBuf.size.width        = 800;
        tmpFrmBuf.size.height       = 480;

        tmpLayerAttr.mode           = DISP_LAYER_WORK_MODE_NORMAL;
        tmpLayerAttr.ck_mode        = 0;
        tmpLayerAttr.alpha_en       = 0;
        tmpLayerAttr.alpha_val      = 0;
        tmpLayerAttr.pipe           = 0;
        tmpLayerAttr.prio           = 0xff;
        //screen window information
        tmpLayerAttr.scn_win.x      = 0;
        tmpLayerAttr.scn_win.y      = 0;
        tmpLayerAttr.scn_win.width  = 800;
        tmpLayerAttr.scn_win.height = 480;
        //frame buffer pst and size information
        tmpLayerAttr.src_win.x      = 0;
        tmpLayerAttr.src_win.y      = 0;
        tmpLayerAttr.src_win.width  = 800;
        tmpLayerAttr.src_win.height = 480;
        //set channel
        tmpLayerAttr.channel        = DISP_LAYER_OUTPUT_CHN_DE_CH1;
        tmpLayerAttr.fb             = &tmpFrmBuf;
        
        fioctrl(dvbc->disFile, DISP_CMD_LAYER_SET_PARA, dvbc->disLayer, &tmpLayerAttr);
        
        //* open graphic layer
        fioctrl(dvbc->disFile, DISP_CMD_LAYER_OPEN, dvbc->disLayer, (void *)0);
    }
}

static void clear_background(void)
{
    __u32 size;
    if((dvbc->disFile != NULL) && (dvbc->disLayer != 0))
    {
        WARNING("close layer.\n");
        fioctrl(dvbc->disFile, DISP_CMD_LAYER_CLOSE, dvbc->disLayer, (void *)0);
        dvbc->disLayer = 0;
        WARNING("close display file.\n");
        fclose(dvbc->disFile);
        dvbc->disFile = NULL;
        if(dvbc->radio_bg)
        {
            size = 800*480*4;
            esMEMS_Pfree(dvbc->radio_bg, size/1024);
            dvbc->radio_bg = NULL;
        }
    }
}
#endif

static void dvbc_maintsk(void *p_arg);

static void stop_plugins(__dvbc_t *dvbc);
static void close_plugins(__dvbc_t *dvbc);
static __s32 QuickStop_plugins(__dvbc_t *dvbc);

static __s32 request_mem(__dvbc_t *dvbc, __s32 md_type, __md_buf_t* md_uf);
static __s32 update_data(__dvbc_t *dvbc, __s32 md_type, __md_data_info_t* info);

#if (PLAY_VIDEO_STREAM == 1)
static __s32 reset_video(__dvbc_t *dvbc);
#endif

static __s32 pass_ad_init_info(__dvbc_t *dvbc)
{
    __krnl_sktfrm_t *skt;	
	do
	{
    	skt = esKRNL_SktPend(dvbc->pABSSktout, KRNL_SKT_USR_IN, 20);
    	if(skt == NULL)
    	{
        	WARNING("pass_ad_init_info pend socket frame timeout.\n");
        	continue;
    	}
	}while(skt == NULL);
	
    *((__u32*)skt->msg) = (__u32)&dvbc->adFmt;
    skt->msglen         = sizeof(__ad_codec_fmt_t);
    skt->datatype       = CEDAR_DATATYPE_NONE;
    skt->msgtype        = PSR2ADEC_MSG_TYPE_ABSDECINF;
    skt->datalen        = 0;
    esKRNL_SktPost(dvbc->pABSSktout, KRNL_SKT_USR_IN, skt);

    return EPDK_OK;
}


static __s32 pass_vd_init_info(__dvbc_t *dvbc)
{
    __krnl_sktfrm_t *skt;
	do
	{
    	skt = esKRNL_SktPend(dvbc->pVBSSktout, KRNL_SKT_USR_IN, 20);
    	if(skt == NULL)
    	{
        	WARNING("pass_vd_init_info pend socket frame timeout.\n");
			continue;
    	}
	}while(skt == NULL);
	
    *((__u32*)skt->msg) = (__u32)&dvbc->vdFmt;
    skt->msglen         = sizeof(__vd_codec_fmt_t);
    skt->datatype       = CEDAR_DATATYPE_NONE;
    skt->msgtype        = PSR2VDEC_MSG_TYPE_VBSINF;
    skt->datalen        = 0;
    esKRNL_SktPost(dvbc->pVBSSktout, KRNL_SKT_USR_IN, skt);

    return EPDK_OK;
}


#if PLAY_SUBTITLE_STREAM
static __s32 pass_subt_init_info(__dvbc_t *dvbc)
{
    __krnl_sktfrm_t *skt;
   
	do
	{
    	skt = esKRNL_SktPend(dvbc->pLBSSktout, KRNL_SKT_USR_IN, 20);
			
    	if(skt == NULL)
    	{
        	WARNING("pass_subt_init_info pend socket frame timeout.\n");
        	continue;
    	}
	}while(skt == NULL);
	
    *((__u32*)skt->msg) = (__u32)&dvbc->subtFmt;
    skt->msglen         = sizeof(__subtitle_codec_format_t);
    skt->datatype       = CEDAR_DATATYPE_NONE;
    skt->msgtype        = PSR2LDEC_MSG_TYPE_LBSINF;
    skt->datalen        = 0;
    esKRNL_SktPost(dvbc->pLBSSktout, KRNL_SKT_USR_IN, skt);

    return EPDK_OK;
}
#endif

/*******************************************************************************
Function name: CB_CedarMsgQPost
Description: 
    1.返回值和MsgQueue->QPost()一样.
    2.给其他模块使用,因为epos的模块有自己的地址空间,没有办法传函数指针给其他模块使用,只能如此.
Parameters: 
    
Return: 
    
Time: 2011/6/20
*******************************************************************************/
__s32 CB_CedarMsgQPost(void *arg)    //arg的实际类型是MsgQueue的使用者自己定义的, 长度就是MsgQueue->nBufSize, 这里是CedarMsg类型
{
    //MESSAGE("call-back function, set audio play time!\n");
    return dvbc->CedarCmdQ->QPost(dvbc->CedarCmdQ, arg);
    //MESSAGE("call-back function, set audio play time successed!\n");
}

/*
*********************************************************************************************************
*                                     MInit
*
* Description: dvb control module init.
* Arguments  : none
*
* Returns    : module init result, always return EPDK_OK;
*********************************************************************************************************
*/
__s32 MInit(void)
{
    MESSAGE("Init dvb control module!\n");

    //* init the cedar handle to null
    dvbc = NULL;
    return EPDK_OK;
}


/*
*********************************************************************************************************
*                                     MExit
*
* Description: dvb control module exit.
*
* Arguments  : none
*
* Returns    : module exit result;
*              EPDK_OK     -   module exit successe,
*              EPDK_FAIL   -   module exit failed;
*********************************************************************************************************
*/
__s32 MExit(void)
{
    MESSAGE("Exit dvb control module!\n");

    if(dvbc)
    {
        //* module handle has not been closed
        WARNING("dvb control module has not been closed, now close it.\n");
        MClose((__mp *)dvbc);
        dvbc = 0;
    }

    MESSAGE("Exit dvb control module successe!\n");
    return EPDK_OK;
}


/*
*********************************************************************************************************
*                                     MOpen
*
* Description: module open function
*
* Arguments  : mid      cedarcore module id;
*              	 mod      the mode to open cedarcore mdoule;
*
* Returns    : cedar module handle;
*               !0  -   open cedar module successed;
*                0  -   open cedar module failed!
*********************************************************************************************************
*/
__mp *MOpen(__u32 mid, __u32 mod)
{
    __u8    err;
    __s32   i;

    MESSAGE("Open dvb control module!\n");
    CEDAR_HEAPINIT();
    CEDAR_PHEAPINIT();

    //* check if dvb control module has been opened by someone, it can only be opened one time
    if(dvbc)
    {
        WARNING("DVBC MOpen: dvbc has been opened by someone already!\n");
        return NULL;
    }
		
//		if(HWChipSDKIsMatched()!= EPDK_OK)
//		{
//				WARNING("The chipId is error\n");
//				return NULL;
//		}	
    dvbc = (__dvbc_t * )MALLOC(sizeof(__dvbc_t));
    if(dvbc == NULL)
    {
        WARNING("DVBC MOpen: request memory for dvb control module failed!\n");
        goto _err0_mopen;
    }

    MEMSET(dvbc, 0, sizeof(__dvbc_t));
    dvbc->nReservedMemSize = 600 * 1024;
	dvbc->tvMode = 1;
	
    dvbc->nFileSwitchVplyMode = CEDAR_FILE_SWITCH_VPLY_MODE_SEAMLESS;  //默认seamless模式, CEDAR_FILE_SWITCH_VPLY_MODE_SEAMLESS, CEDAR_FILE_SWITCH_VPLY_MODE_GAP
	dvbc->nStopMode = CEDAR_STOP_MODE_KEEP_PLUGINS;
	dvbc->aud_raw_data_mode = 0;
    dvbc->pLockStreamPts = newCedarLock();
    if(NULL == dvbc->pLockStreamPts)
    {
        WARNING("new semaphore fail!\n");
    }

    //* create message for send command to main task
//    dvbc->CmdQ = esKRNL_QCreate(DVBC_CMDQ_SIZE);
//    if(dvbc->CmdQ == NULL)
//        goto _err1_mopen;

    dvbc->CedarCmdQ = newMsgQueue(DVBC_CMDQ_SIZE, sizeof(CedarMsg));
    if(dvbc->CedarCmdQ == NULL)
    {
        goto _err1_mopen;
    }

    dvbc->mid = mid;
    //dvbc->maintskprio = esKRNL_TCreate(dvbc_maintsk, 0, 0x1000, (dvbc->mid << 8) | DVBC_CMD_TASK_PRIO);
    dvbc->maintskprio = esKRNL_TCreate(dvbc_maintsk, 0, 0x1000, (dvbc->mid << 8) | KRNL_priolevel2);
    if(dvbc->maintskprio == 0)
    {
        WARNING("DVBC MOpen: create cedar core main task failed!\n");
        goto _err2_mopen;
    }

    //* initial audio setting information
    dvbc->setting_inf.volume 				= 0;
    dvbc->setting_inf.volume_flag 			= 1;
    dvbc->setting_inf.eq 					= CEDAR_AUD_EQ_TYPE_NORMAL;
	
    //initial user eq filter
    for(i=0; i<USR_EQ_BAND_CNT; i++)
    {
        dvbc->setting_inf.usr_eq_filter[i] 	= 0;
    }
    dvbc->setting_inf.eq_flag 				= 1;
    dvbc->setting_inf.channel 				= 2;
    dvbc->setting_inf.channel_flag 			= 1;
    dvbc->setting_inf.vpsspeed 				= 0;
    dvbc->setting_inf.vpsspeed_flag 		= 1;
    dvbc->setting_inf.ab_loopcnt 			= 3;
    dvbc->setting_inf.ab_loopcnt_flag 		= 1;
    dvbc->setting_inf.spectrum 				= 0;
    dvbc->setting_inf.spectrum_flag 		= 1;

    //* initial video setting information
    dvbc->setting_inf.vid_layer_hdl        = 0;
    dvbc->setting_inf.vid_layer_hdl_flag   = 0;
    dvbc->setting_inf.vid_show_mode        = VID_WINDOW_BESTSHOW;
    dvbc->setting_inf.vid_show_mode_flag   = 1;
    dvbc->setting_inf.vid_window_flag      = 0;
    dvbc->setting_inf.frpic_hold_time      = CEDAR_FRPIC_HOLDTIME_DEFAULT;
    dvbc->setting_inf.frpic_hold_time_flag = 1;

    //* initial audio and video shared setting information
    dvbc->setting_inf.frspeed 				= CEDAR_FRSPEED_DEFAULT;
    dvbc->setting_inf.frspeed_flag 			= 1;
    dvbc->setting_inf.vid_layer_alpha       = 255;
    dvbc->setting_inf.alpha_blending_on     = 0;

    //* switch on video picture show
    dvbc->pic_show_flag 					= 1;
    dvbc->status 							= DVB_STAT_STOP;

    dvbc->nMafEnable                        = 0;
	
#if ((PLAY_AUDIO_STREAM == 0) || (PLAY_VIDEO_STREAM == 0))
	dvbc->tmpBuf = (__u8*)MALLOC(128*1024);
	if(!dvbc->tmpBuf)
	{
		esKRNL_TDelReq((dvbc->mid << 8) | dvbc->maintskprio);
		goto _err2_mopen;
	}
#endif
#if (PLAY_SUBTITLE_STREAM == 0)
    dvbc->tmpSubtitleBuf = (__u8*)MALLOC(128*1024);
    if(!dvbc->tmpSubtitleBuf)
    {
        esKRNL_TDelReq((dvbc->mid << 8) | dvbc->maintskprio);
        goto _err2_mopen;
    }
#endif


    return (__mp *)dvbc;

_err2_mopen:
//    esKRNL_QDel(dvbc->CmdQ, 0, &err);
//    dvbc->CmdQ = 0;
    deleteMsgQueue(dvbc->CedarCmdQ, OS_DEL_NO_PEND, &err);
    dvbc->CedarCmdQ = NULL;

_err1_mopen:
    FREE(dvbc);
    dvbc = 0;

_err0_mopen:
    if(dvbc->pLockStreamPts)
    {
        deleteCedarLock(dvbc->pLockStreamPts);
        dvbc->pLockStreamPts = NULL;
    }

    CEDAR_HEAPEXIT();
    CEDAR_PHEAPEXIT();
    
    return NULL;
}



/*
*********************************************************************************************************
*                                     MClose
*
* Description: close dvb control module
*
* Arguments  : mp module handle
*
* Returns    : if success return EPDK_OK, esle return EPDK_FAIL
*********************************************************************************************************
*/
__s32 MClose(__mp *mp)
{
    __u8 i;
    __u8 err;
    CedarMsg    tmpCedarMsgInst;

    if(mp != (__mp*)dvbc)
        return EPDK_FAIL;

    if(!dvbc)
    {
        MESSAGE("DVBC MClose: module has been closed by someone already!\n");
        return EPDK_OK;
    }

    //* uninstall all plug-in in cedar core sub-system
    uninstall_plugins(dvbc);

    //* delete main task
    for(i=200; i>0; i--)
    {
        if(esKRNL_TDelReq((dvbc->mid << 8) | dvbc->maintskprio) == OS_TASK_NOT_EXIST)
            break;

        //* wakeup main task
        //esKRNL_QPost(dvbc->CmdQ, (void *)DVBC_MAINTASK_CMD_NOP);
        tmpCedarMsgInst.nMsgType = DVBC_MAINTASK_CMD_NOP;
        dvbc->CedarCmdQ->QPost(dvbc->CedarCmdQ, &tmpCedarMsgInst);
                
        esKRNL_TimeDly(1);
    }

    if(i == 0)
    {
        WARNING("DVBC MClose: main task cannot be deleted!\n");
        return EPDK_FAIL;
    }

    //* delete command queue
//    if(dvbc->CmdQ)
//    {
//        esKRNL_QDel(dvbc->CmdQ, 0, &err);
//        dvbc->CmdQ = 0;
//    }
	deleteMsgQueue(dvbc->CedarCmdQ, OS_DEL_NO_PEND, &err);
    dvbc->CedarCmdQ = NULL;

    if(dvbc->pLockStreamPts)
    {
        deleteCedarLock(dvbc->pLockStreamPts);
        dvbc->pLockStreamPts = NULL;
    }
#if ((PLAY_AUDIO_STREAM == 0) || (PLAY_VIDEO_STREAM == 0))
	FREE(dvbc->tmpBuf);
#endif

#if (PLAY_SUBTITLE_STREAM == 0)
    FREE(dvbc->tmpSubtitleBuf);
#endif

    FREE(dvbc);
    dvbc = (__dvbc_t *)0;

    CEDAR_HEAPEXIT();
    CEDAR_PHEAPEXIT();
    
    MESSAGE("DVBC MClose: module close complete!\n");

    return EPDK_OK;
}




/*
*********************************************************************************************************
*                                     MRead
*
* Description: do nothing
*
* Arguments  : none
*
* Returns    : n
*
*********************************************************************************************************
*/
__u32 MRead(void *pdata, __u32 size, __u32 n, __mp *mp)
{
    return 0;
}


/*
*********************************************************************************************************
*                                     MWrite
*
* Description: do nothing
*
* Arguments  : none
*
* Returns    : n
*
*********************************************************************************************************
*/
__u32 MWrite(const void *pdata, __u32 size, __u32 n, __mp *mp)
{
    return 0;
}



/*
*********************************************************************************************************
*                                     MIoctrl
*
* Description:  module control
*
* Arguments  :  mp      module handle;
*               cmd     the command to be sent to cedar core;
*               aux
*               pbuffer
*
* Returns    : depend on different commands.
*
*********************************************************************************************************
*/
__s32 MIoctrl(__mp *mp, __u32 cmd, __s32 aux, void *pbuffer)
{
    __s32   result;
    CedarMsg    tmpCedarMsgInst;
    switch(cmd)
    {
        case DVB_CMD_SET_MAPLE_INI:
        {
            dvbc->pMapleINI = (CMapleINI*)pbuffer;
            return EPDK_OK;
        }
		
	    case DVB_CMD_SET_SPLY_DRAW_CB:
        {
            dvbc->CB_ShowDraw= (__pCBK_t)pbuffer;  
            return EPDK_OK;
        }
        case DVB_CMD_SET_FEEDBACK_MSG_QUEUE:
        {
            dvbc->pFeedBackMsgQ = (__krnl_event_t*)pbuffer;
            return EPDK_OK;
        }
        //* send play command, the command will be response when player current
        //* status is stop, pause, fast-forward or fast-backward, it will be ignored
        //* under others status
    case DVB_CMD_PLAY:  //在另一个线程执行播放操作,但是接口内同步等待.
        {
//#ifdef	DEBUG_TIME
#if	DEBUG_TIME
			esKRNL_TimeSet(0);  //这里是想统计从开始播放到收到第一个视频数据或音频数据的时间间隔.
#endif			
            if((dvbc->status == DVB_STAT_STOP) && (dvbc->maintskprio != 0))
            {
                dvbc->nOneSegFlag = aux;
                dvbc->status |= DVBC_TMP_STATUS_FLAG;
                //esKRNL_QPost(dvbc->CmdQ, (void *)DVBC_MAINTASK_CMD_PLAY);
                tmpCedarMsgInst.nMsgType = DVBC_MAINTASK_CMD_PLAY;
                dvbc->CedarCmdQ->QPost(dvbc->CedarCmdQ, &tmpCedarMsgInst);
								while(dvbc->status & DVBC_TMP_STATUS_FLAG)
								esKRNL_TimeDly(1);

				WARNING("play status = %d.\n", dvbc->status);
                return EPDK_OK;
            }
            else
            {
                return EPDK_FAIL;
            }
        }

        //* send stop command, the command will be response when current status is
        //* play, pause, fast-forward or fast-backward, it will be ignored under other status
    case DVB_CMD_STOP:
        {
			if((dvbc->status == DVB_STAT_PLAY) && (dvbc->maintskprio != 0))
			{
            	dvbc->status |= DVBC_TMP_STATUS_FLAG;
                //esKRNL_QPost(dvbc->CmdQ, (void *)DVBC_MAINTASK_CMD_STOP);
                tmpCedarMsgInst.nMsgType = DVBC_MAINTASK_CMD_STOP;
                dvbc->CedarCmdQ->QPost(dvbc->CedarCmdQ, &tmpCedarMsgInst);

                //接口内做同步
				while(dvbc->status & DVBC_TMP_STATUS_FLAG)
					esKRNL_TimeDly(1);

            	return EPDK_OK;
			}
			else
			{
				return EPDK_FAIL;
			}
        }

        //* get cedar current status
    case DVB_CMD_GET_STATUS:
        {
            while(dvbc->status & DVBC_TMP_STATUS_FLAG)
            {
                MESSAGE("Current status is a temp status, wait!\n");
                esKRNL_TimeDly(2);
            }

            return  dvbc->status;
        }

        //* set pcr to avsync module, code will be added later.
    case DVB_CMD_SET_PCR:
        {
		if(dvbc->status == DVB_STAT_PLAY)
		{
	                if(dvbc->avsync)
	                {
		         	esMODS_MIoctrl(dvbc->avsync, DRV_AVS_CMD_SET_PCR, 0, pbuffer);
	                }
	                else
	                {
	                	WARNING("fatal error! dvbc->avsync = NULL! check code!\n");
	                }
		}
		else
		{
			WARNING("Not Set PCR, dvbc->status[0x%x]!=DVB_STAT_PLAY, \n", dvbc->status);
		}
		return EPDK_OK;
        }
/*
	case DVB_CMD_SET_PCR_TO_DVB://因为有前期两次缓冲PCR值校验，所以要减回两次,按50ms一次计算
		{
			__u32 pcrpts = 0;
			if(dvbc->status == DVB_STAT_PLAY)
			{
				pcrpts = (__u32)pbuffer;
				
				 if(dvbc->pcrpts == pcrpts)
		        {
		            //如果和上次相同,就不要了
		            WARNING("impossible! pcrpts[%d] same! check!\n", pcrpts);		          
		            return EPDK_OK;
		        }
				dvbc->pcrpts = pcrpts;
				if(pcrpts !=0)
					WARNING("22pcrpts = %d\n",dvbc->pcrpts);
			}
            else
            {
                WARNING("fatal error! dvbc->status[0x%x]!=DVB_STAT_PLAY, \n", dvbc->status);
            }
            return EPDK_OK;
		}
*/
        //* get current play time, code will be added later.
    case DVB_CMD_GET_TIME:
        {
            __s64 time;
            time = esMODS_MIoctrl(dvbc->avsync, DRV_AVS_CMD_GET_VID_TIME, DRV_AVS_TIME_TOTAL, NULL);
            *((__s64*)pbuffer) = (time + dvbc->ptsBase);
            return EPDK_OK;
        }
        
        //* set call back function for close caption, code will be added later.
    case DVB_CMD_SET_CLOSE_CAPTION_CB:
        {
            return EPDK_OK;
        }

        //* set media information.
    case DVB_CMD_SET_MEDIA_INFO:
        {
            if(aux == MEDIA_TYPE_AUDIO)
            {
                __ad_codec_fmt_t* tmp = (__ad_codec_fmt_t*) pbuffer;
                MEMCPY(&dvbc->adFmt, tmp, sizeof(__ad_codec_fmt_t));
                dvbc->getAdFmt = 1;
				
#if (PLAY_AUDIO_STREAM == 0)
				dvbc->adFmt.codec_type = DVB_ABS_TYPE_NONE;
#endif
				if(dvbc->adFmt.codec_type != DVB_ABS_TYPE_NONE)
					dvbc->hasAudio = 1;
				
#if DEBUG_SAVE_AUDIO_STREAM
	dvbc->fpAudioStream = fopen("e:\\dvb_audio_stream.es", "wb");
	if(dvbc->fpAudioStream == NULL)
		WARNING("CANNOT CREATE FILE TO SAVE DVB AUDIO STREAM.\n");
#endif
    
                return EPDK_OK;
            }
            else if(aux == MEDIA_TYPE_VIDEO)
            {
                __vd_codec_fmt_t* tmp = (__vd_codec_fmt_t*) pbuffer;
                MEMCPY(&dvbc->vdFmt, tmp, sizeof(__vd_codec_fmt_t));
#if (MAPLE_TV_STANDAR == MAPLE_TV_CMMB)
				//dvbc->vdFmt.video_bs_src = 0;   //* (dvbc->tvStandar == PLAYER_STANDAR_CMMB) ? 0 : 2;
				dvbc->vdFmt.video_bs_src = CEDARLIB_FILE_FMT_UNKNOWN;
#else
				//dvbc->vdFmt.video_bs_src = 2;   //* (dvbc->tvStandar == PLAYER_STANDAR_CMMB) ? 0 : 2;
				dvbc->vdFmt.video_bs_src = CEDARLIB_FILE_FMT_TS;
#endif
                dvbc->getVdFmt = 1;

#if (PLAY_VIDEO_STREAM == 0)
				dvbc->vdFmt.codec_type = DVB_VBS_TYPE_NONE;
#endif
				if(dvbc->vdFmt.codec_type != DVB_VBS_TYPE_NONE)
					dvbc->hasVideo = 1;

                WARNING("dvbc->hasVideo[%d]\n", dvbc->hasVideo);
				
#if DEBUG_SAVE_VIDEO_STREAM
	dvbc->fpVideoStream= fopen("e:\\dvb_video_stream.es", "wb");
	if(dvbc->fpVideoStream == NULL)
		WARNING("CANNOT CREATE FILE TO SAVE DVB VIDEO STREAM.\n");
#endif
                return EPDK_OK;
            }
#if PLAY_SUBTITLE_STREAM
            else if(aux == MEDIA_TYPE_SUBTITLE)
            {
                __subt_codec_fmt_t* tmp = (__subt_codec_fmt_t*) pbuffer;
                MEMCPY(&dvbc->subtFmt, tmp, sizeof(__subt_codec_fmt_t));
                dvbc->getSubtFmt = 1;

				if(dvbc->subtFmt.codec_type != DVB_VBS_TYPE_NONE)
					dvbc->hasSubtitle = 1;
				
                return EPDK_OK;
            }
#endif
            else
            {
                WARNING("[%s]fatal error! DVB_CMD_SET_MEDIA_INFO, aux[0x%x]\n", __FUNCTION__, aux);
                return EPDK_FAIL;
            }
        }

        //* request media buffer, code will be added later.
    case DVB_CMD_REQUEST_BUFFER:
        {
            return request_mem(dvbc, aux, (__md_buf_t*)pbuffer);
        }

        //* update media data, code will be added later.
    case DVB_CMD_UPDATE_DATA:
        {
            //__u8    err;
            //互斥有必要时再打开
            dvbc->pLockStreamPts->lock(dvbc->pLockStreamPts);
            result = update_data(dvbc, aux, (__md_data_info_t*)pbuffer);
            dvbc->pLockStreamPts->unlock(dvbc->pLockStreamPts);
            return result;
        }

        //* set volume value, should we process the volume before start play???
    case DVB_CMD_SET_VOL:
        {
            return 0;
        }

        //* get current volume
    case DVB_CMD_GET_VOL:
        {
            return 0;
        }

        //* increase volume, one command just increase one step
    case DVB_CMD_VOLUP:
        {
            return 0;
        }

        //* decrease volume, one command just decrease one step
    case DVB_CMD_VOLDOWN:
        {
            return 0;
        }
    case DVB_CMD_SET_SWGAIN:
	{
		if(dvbc->ardr)
		{
			esMODS_MIoctrl(dvbc->ardr, CEDAR_ARDR_CMD_SET_SWGAIN, aux, 0);
		}
		dvbc->setting_inf.swgain = aux;
		return EPDK_OK;
    	}
	
    case DVB_CMD_GET_SWGAIN:
	{
		if(dvbc->ardr)
		{
			return esMODS_MIoctrl(dvbc->ardr, CEDAR_ARDR_CMD_GET_SWGAIN, 0, 0);
		}
		else
		{
			return dvbc->setting_inf.swgain;
		}
		
    	}
	
    case DVB_CMD_SET_VIDEO_LAYER:
        {
            if(pbuffer)
            {
                dvbc->setting_inf.vid_layer_hdl = (__s32)pbuffer;
                dvbc->setting_inf.vid_layer_hdl_flag = 1;

                return EPDK_OK;
            }

            return EPDK_FAIL;
        }

    case DVB_CMD_SET_VIDEO_WINDOW:
        {
            __rect_t *tmpVidWind = (__rect_t *)pbuffer;

            //* check if window parameter is valid
            if(!tmpVidWind || !tmpVidWind->width || !tmpVidWind->height)
            {
                WARNING("Video window parameter is invalid!\n");
                return EPDK_FAIL;
            }

            //* if display mode has not been set, set it to best show mode
            if(!dvbc->setting_inf.vid_show_mode_flag)
            {
                dvbc->setting_inf.vid_show_mode = VID_WINDOW_BESTSHOW;
                dvbc->setting_inf.vid_show_mode_flag = 1;
            }

            //* set video window information to video playback
            if(dvbc->vply)
            {
                //* set video window position and size information
                if(esMODS_MIoctrl(dvbc->vply, CEDAR_VPLY_CMD_SET_VIDWINDOW,  \
                    dvbc->setting_inf.vid_show_mode, tmpVidWind) != EPDK_OK)
                {
                    WARNING("Set video layer window information failed!\n");
                    return EPDK_FAIL;
                }
            }

            //* record the video window information
            dvbc->setting_inf.vid_window.x = tmpVidWind->x;
            dvbc->setting_inf.vid_window.y = tmpVidWind->y;
            dvbc->setting_inf.vid_window.width = tmpVidWind->width;
            dvbc->setting_inf.vid_window.height = tmpVidWind->height;
            dvbc->setting_inf.vid_window_flag = 1;

            return EPDK_OK;
        }

    case DVB_CMD_GET_VIDEO_WINDOW:
        {
            if(dvbc->setting_inf.vid_window_flag)
            {
                return (__s32)&dvbc->setting_inf.vid_window;
            }

            return 0;
        }

    case DVB_CMD_SET_VIDEO_SHOW_MODE:
        {
            //* check if parameter is valid
            if((aux <= VID_WINDOW_NOTCARE) || (aux >= VID_WINDOW_NUM_))
            {
                WARNING("video display mode parameter is invalid!\n");
                return dvbc->setting_inf.vid_show_mode;
            }

            //* set video window information to video playback
            if(dvbc->vply && dvbc->setting_inf.vid_window_flag)
            {
                if(VID_WINDOW_USERDEF == aux)
                {
                    esMODS_MIoctrl(dvbc->vply, CEDAR_VPLY_CMD_SET_CB_SETSRCSCNWINPARA, 0, (void *)pbuffer);
                }
                //* set video window position and size information
                if(esMODS_MIoctrl(dvbc->vply, CEDAR_VPLY_CMD_SET_VIDWINDOW, \
                    aux, (void *)&dvbc->setting_inf.vid_window) != EPDK_OK)
                {
                    WARNING("Set video layer window information failed! mode[%x]\n", aux);
                    return dvbc->setting_inf.vid_show_mode;
                }
            }

            //* record new parameter
            if(CEDAR_VID_WINDOW_USERDEF == aux)
            {
                dvbc->VplyCB_SetSrcScnWinPara = (__pCBK_t)pbuffer;
            }
            dvbc->setting_inf.vid_show_mode = aux;
            dvbc->setting_inf.vid_show_mode_flag = 1;
            return dvbc->setting_inf.vid_show_mode;
        }
        
    case DVB_CMD_SET_VIDEO_LYR_ALPHA:
        dvbc->setting_inf.vid_layer_alpha = (__u8)aux;
        return EPDK_OK;
        
    case DVB_CMD_SET_VIDEO_LYR_ALPHA_ON:
        dvbc->setting_inf.alpha_blending_on = 1;
        if(dvbc->vply)
        {
            esMODS_MIoctrl(dvbc->vply, CEDAR_VPLY_CMD_ENABLE_ALPHA_BLENDING, 0, NULL);
        }
        return EPDK_OK;
    case DVB_CMD_SET_VIDEO_LYR_ALPHA_OFF:
        if(dvbc->vply)
        {
            esMODS_MIoctrl(dvbc->vply, CEDAR_VPLY_CMD_DISABLE_ALPHA_BLENDING, 0, NULL);
        }
        dvbc->setting_inf.alpha_blending_on = 0;
        return EPDK_OK;

    case DVB_CMD_CONFIG_VIDEO_LYR_CK_ENABLE:
    {
        if(dvbc->vply)
        {
            esMODS_MIoctrl(dvbc->vply, CEDAR_VPLY_CMD_CONFIG_VIDEO_LYR_CK_ENABLE, aux, NULL);
        }
        dvbc->setting_inf.nCkEnableValue = aux;
        return EPDK_OK;
    }
    case DVB_CMD_ENABLE_DEINTERLACE:
    {
        dvbc->nMafEnable = aux;
        return EPDK_OK;
    }
    case DVB_CMD_GET_FRAMESIZE:
    {
        result = 0;
        if(dvbc->vdec)
        {
            result = esMODS_MIoctrl(dvbc->vdec, CEDAR_VDEC_CMD_GET_FRMSIZE, 0, 0);
        }
        return result;
    }
		case DVB_CMD_GET_VE_STATUS:
		{
				result = 0;
        if(dvbc->vdec)
        {
            result = esMODS_MIoctrl(dvbc->vdec, CEDAR_VDEC_CMD_GET_VE_STATUS, 0, 0);
        }
        return result;
				
		}
    case DVB_CMD_CHECK_PICTURE_OUT:
    {
        if(dvbc->vply)
        {
            result = esMODS_MIoctrl(dvbc->vply, CEDAR_VPLY_CMD_CHECK_PICTURE_OUT, 0, NULL);
        }
        else
        {
            result = EPDK_FALSE;
        }
        return result;
    }
		case DVB_CMD_SET_RESERVED_MEM_SIZE:
		{
			dvbc->nReservedMemSize = aux;
			return EPDK_OK;
    }
		case DVB_CMD_SET_TV_MODE:
		{
			dvbc->tvMode = aux;
			return EPDK_OK;
    }
		case DVB_CMD_SET_ROTATE:
		{
			dvbc->rotate_mode = aux;
			return EPDK_OK;
    }
    case DVB_CMD_SET_STOP_MODE:
    {
       dvbc->nStopMode = aux;
       return EPDK_OK;        
    }
		case DVB_CMD_SET_SWITCH_VPLY_MODE:
    {
      if(dvbc->nFileSwitchVplyMode != aux)
      {
        dvbc->nFileSwitchVplyMode = aux;
        if(dvbc->vply)
        {
            result = esMODS_MIoctrl(dvbc->vply, CEDAR_VPLY_CMD_SET_FILE_SWITCH_VPLY_MODE, aux, NULL);
        }
      }
     return result;                
        
    }
    default:
        break;
    }

    return EPDK_OK;
}



/*
*********************************************************************************************************
*                                     cedar_maintsk
*
* Description: main task of dvb control
*
* Arguments  : p_arg    cedar handler
*
* Returns    : void
*
*********************************************************************************************************
*/
static void dvbc_maintsk(void *p_arg)
{
    __u8            err;
    __s32           result;
    //void            *msg;
    CedarMsg        *pMsg = NULL;
    CedarMsg        CedarMsgInst;

    while(1)
    {
        while(1)
        {
            if(esKRNL_TDelReq(EXEC_prioself) == OS_TASK_DEL_REQ)
            {
                MESSAGE("Delete dvbc_maintsk!\n");
                goto _exit_cedar_maintsk;
            }


//            msg = esKRNL_QPend(dvbc->CmdQ, 100, &err);
//            if(msg)
//            {
//                MESSAGE("dvbc_maintsk get message:%d!\n", (__u32)msg);
//                break;
//            }
            pMsg = dvbc->CedarCmdQ->QPend(dvbc->CedarCmdQ, 100, &err, &CedarMsgInst);
            //if(msg)
            if(pMsg)
            {
                WARNING("Cedar main task get message:%x!\n", (__u32)pMsg->nMsgType);
                break;
            }
        }

        switch((__u32)pMsg->nMsgType)
        {
            // start play the media file
        case DVBC_MAINTASK_CMD_PLAY:
            {
                //only process _CEDAR_CMD_PLAY under stop status, or, the command will be ignored
                if((dvbc->status & ~DVBC_TMP_STATUS_FLAG) == DVB_STAT_STOP)
                {
                    dvbc->preVdPts[0] = dvbc->preVdPts[1] = 0;
                    dvbc->preAdPts[0] = dvbc->preAdPts[1] = 0;
                    //try to play the media file
                    if((dvbc->vdFmt.codec_type != DVB_VBS_TYPE_NONE) && (dvbc->vdFmt.codec_type != DVB_VBS_TYPE_UNKNOWN))
                    {
						//* create video bit stream socket
						dvbc->vbs_frm = NULL;
    					dvbc->pVBSSktout = esKRNL_SktCreate(CEDAR_PSR_VBS_QSIZE,
                                        					CEDAR_PSR_VBS_BUFSIZE | KRNL_SKT_BUF_PHY_UNSEQ,
                                        					sizeof(__psr_2_vdec_msg_t) | KRNL_SKT_BUF_PHY_UNSEQ );
    					if(!dvbc->pVBSSktout)
    					{
        					WARNING("dvbc_maintsk: create VBS socket failed!\n");
                        	dvbc->status = DVB_STAT_STOP;
                        	break;
    					}
                    }
					
					//* create audio bit stream socket
                    //try to play the media file
                    if((dvbc->adFmt.codec_type != DVB_ABS_TYPE_NONE) && (dvbc->adFmt.codec_type != DVB_ABS_TYPE_UNKNOWN))
                    {
						//* create video bit stream socket
						dvbc->abs_frm = NULL;
    					dvbc->pABSSktout = esKRNL_SktCreate(CEDAR_PSR_ABS_QSIZE,
                                        					CEDAR_PSR_ABS_BUFSIZE | KRNL_SKT_BUF_PHY_UNSEQ,
                                        					sizeof(__psr_2_adec_msg_t) | KRNL_SKT_BUF_PHY_UNSEQ );
    					if(!dvbc->pABSSktout)
    					{
        					WARNING("dvbc_maintsk: create ABS socket failed!\n");
                        	dvbc->status = DVB_STAT_STOP;
                        	break;
    					}
                    }

                    //* create subtitle bit stream socket
                    //try to play the media file
                    if((dvbc->subtFmt.codec_type != DVB_SBS_TYPE_NONE) && (dvbc->subtFmt.codec_type != DVB_SBS_TYPE_UNKNOWN))
                    {
						//* create subtitle bit stream socket
						dvbc->sbs_frm = NULL;
    					dvbc->pLBSSktout = esKRNL_SktCreate(CEDAR_PSR_LBS_QSIZE,
                                        		        	CEDAR_PSR_LBS_BUFSIZE | KRNL_SKT_BUF_PHY_UNSEQ,
                                        					sizeof(__psr_2_ldec_msg_t) | KRNL_SKT_BUF_PHY_UNSEQ );
    					if(!dvbc->pLBSSktout)
    					{
        					WARNING("dvbc_maintsk: create LBS socket failed!\n");
                        	dvbc->status = DVB_STAT_STOP;
                        	break;
    					}
                    }
					
					if((dvbc->adFmt.codec_type != DVB_ABS_TYPE_NONE) && (dvbc->adFmt.codec_type != DVB_ABS_TYPE_UNKNOWN))
                		pass_ad_init_info(dvbc);
				
					if((dvbc->vdFmt.codec_type != DVB_VBS_TYPE_NONE) && (dvbc->vdFmt.codec_type != DVB_VBS_TYPE_UNKNOWN))
                		pass_vd_init_info(dvbc);
                		
#if PLAY_SUBTITLE_STREAM
                    if((dvbc->subtFmt.codec_type != DVB_SBS_TYPE_NONE) && (dvbc->subtFmt.codec_type != DVB_SBS_TYPE_UNKNOWN))
                        pass_subt_init_info(dvbc);
#endif
	
                    result = play_media(dvbc);
                    if(result != EPDK_OK)
                    {
                        WARNING("dvbc_maintsk: try play media failed!\n");
                        dvbc->status = DVB_STAT_STOP;
                        break;
                    }
                    
#if DRAW_RADIO_BACKGROUND
                    WARNING("video codec_type = %d.\n", dvbc->vdFmt.codec_type);
                    if((dvbc->vdFmt.codec_type == DVB_VBS_TYPE_NONE) || (dvbc->vdFmt.codec_type == DVB_VBS_TYPE_UNKNOWN))
                    {
                        draw_background();
                    }
#endif
                    dvbc->status = DVB_STAT_PLAY;
                }
                break;
            }

            // stop play
        case DVBC_MAINTASK_CMD_APLYMSG_STOP:
        case DVBC_MAINTASK_CMD_VPLYMSG_STOP:
        {
            WARNING("aply or vply send MSG_STOP[%x]! cedar->status[%x], dvbc will send it to maple!\n", (__u32)pMsg->nMsgType, dvbc->status);
            //dvbc->status |= DVBC_TMP_STATUS_FLAG;
            if(dvbc->pFeedBackMsgQ)
            {
                esKRNL_QPost(dvbc->pFeedBackMsgQ, (void *)pMsg->nMsgType);
            }
            else
            {
                WARNING("fatal error! why dvbc->pFeedBackMsgQ = NULL ???\n");
            }
            break;
        }
        case DVBC_MAINTASK_CMD_STOP:
            {
				//__s32 fatal_error_flag = pMsg->fatal_error_flag;
//                if((__u32)pMsg->nMsgType != DVBC_MAINTASK_CMD_STOP)
//                {
//                    WARNING("aply or vply send MSG_STOP[%x]! cedar->status[%x]\n", (__u32)pMsg->nMsgType, dvbc->status);
//                    dvbc->status |= DVBC_TMP_STATUS_FLAG;
//                }
#if DRAW_RADIO_BACKGROUND
                if((dvbc->vdFmt.codec_type == DVB_VBS_TYPE_NONE) || (dvbc->vdFmt.codec_type == DVB_VBS_TYPE_UNKNOWN))
                {
                    clear_background();
                }
#endif
                if((dvbc->status & ~DVBC_TMP_STATUS_FLAG) != DVB_STAT_STOP)
                {
                	   FileStop_process(dvbc);
                   // uninstall_plugins(dvbc);
					dvbc->ptsBaseGet 	= 0;
					dvbc->ptsBase  		= 0;
					
					dvbc->hasVideo 		= 0;
                    dvbc->nGetVdFlag    = 0;
					//dvbc->vdPtsBase 	= 0;
                    dvbc->vdStreamStartPts = 0;
                    dvbc->vdStreamPts   = 0;
					dvbc->vdPts    		= 0;
					//dvbc->firstVdPts	= 0;
					
					dvbc->hasAudio 		= 0;
                    dvbc->nGetAdFlag    = 0;
					//dvbc->adPtsBase 	= 0;
					dvbc->adStreamStartPts = 0;
                    dvbc->adStreamPts   = 0;
					dvbc->adPts    		= 0;
					//dvbc->firstAdPts	= 0;
					
#if PLAY_SUBTITLE_STREAM
                    dvbc->hasSubtitle      = 0;
                    dvbc->nGetSubFlag    = 0;
                    //dvbc->subPtsBase   = 0;
                    dvbc->subStreamStartPts = 0;
                    dvbc->subStreamPts   = 0;
                    dvbc->subPts         = 0;
                    //dvbc->firstSubPts  = 0;

//					dvbc->hasSubtitle   = 0;
//					dvbc->subtPtsBase   = 0;
//					dvbc->subtPts       = 0;
//					dvbc->firstSubtPts  = 0;
#endif
                }
				
				if (dvbc->pLBSSktout)
				{
					esKRNL_SktDel(dvbc->pLBSSktout, OS_DEL_ALWAYS);
					dvbc->pLBSSktout = NULL;
				}
                
				if (dvbc->pABSSktout)
				{
					esKRNL_SktDel(dvbc->pABSSktout, OS_DEL_ALWAYS);
					dvbc->pABSSktout = NULL;
				}
				
				if (dvbc->pVBSSktout)
				{
					esKRNL_SktDel(dvbc->pVBSSktout, OS_DEL_ALWAYS);
					dvbc->pVBSSktout = NULL;
				}

                dvbc->status = DVB_STAT_STOP;
                break;
            }

        default:
            WARNING("dvbc_maintsk: unknown command %x.\n", (__u32)pMsg->nMsgType);
            break;
        }
    }

_exit_cedar_maintsk:

    INFORMATION("--------------------------- dvbc_maintsk exit!\n");

    esKRNL_TDel(EXEC_prioself);
}


/*
*********************************************************************************************************
*                                        STOP ALL PLUG-IN
*
* Description: Stop all plug-in.
*
* Arguments  : cedarCore - the module handle
*
* Returns    : none
*********************************************************************************************************
*/
static void stop_plugins(__dvbc_t *dvbc)
{
    if(dvbc)
    {
        MESSAGE("stop_plugins: stop all plug-in in cedar sub system!\n");
#if DEBUG_SAVE_VIDEO_STREAM
		if(dvbc->fpVideoStream)
		{
			fclose(dvbc->fpVideoStream);
			dvbc->fpVideoStream = NULL;
		}
#endif

#if DEBUG_SAVE_AUDIO_STREAM
		if(dvbc->fpAudioStream)
		{
			fclose(dvbc->fpAudioStream);
			dvbc->fpAudioStream = NULL;
		}
#endif

#if PLAY_SUBTITLE_STREAM
        if(dvbc->sply)
        {
            MESSAGE("stop subtitle playback...\n");
            esMODS_MIoctrl(dvbc->sply, CEDAR_SPLY_CMD_STOP, 0, 0);
        }
#endif

        if(dvbc->vply)
        {
            MESSAGE("stop video playback...\n");
            esMODS_MIoctrl(dvbc->vply, CEDAR_VPLY_CMD_STOP, 0, 0);
        }

        if(dvbc->aply)
        {
            MESSAGE("stop audio playback...\n");
            esMODS_MIoctrl(dvbc->aply, CEDAR_APLY_CMD_STOP, 0, 0);
        }
        //stop audio render plug-in
        if(dvbc->ardr)
        {
            MESSAGE("stop audio render...\n");
            //ERIC_WARNING("stop audio render...\n");
            esMODS_MIoctrl(dvbc->ardr, CEDAR_ARDR_CMD_STOP, 0, 0);
            //ERIC_WARNING("done!\n");
        }

        //* stop audio render plug-in
#if PLAY_SUBTITLE_STREAM
        if(dvbc->sdec)
        {
            MESSAGE("stop subtitle decoder...\n");
            esMODS_MIoctrl(dvbc->sdec, CEDAR_LDEC_CMD_STOP, 0, 0);
        }
#endif

        if(dvbc->vdec)
        {
            MESSAGE("stop video decoder...\n");
            esMODS_MIoctrl(dvbc->vdec, CEDAR_VDEC_CMD_STOP, 0, 0);
        }

        if(dvbc->adec)
        {
            MESSAGE("stop audio decoder...\n");
            esMODS_MIoctrl(dvbc->adec, CEDAR_ADEC_CMD_STOP, 0, 0);
        }

        //* stop av sync
        if(dvbc->avsync)
        {
            MESSAGE("stop av sync...\n");
            esMODS_MIoctrl(dvbc->avsync,  DRV_AVS_CMD_STOP,  0, 0);
        }
    }

    return;
}


/*
*********************************************************************************************************
*                                        CLOSE ALL PLUG-IN
*
* Description: Close all plug-in.
*
* Arguments  : cedarCore - the module handle
*
* Returns    : none
*********************************************************************************************************
*/
static void close_plugins(__dvbc_t *dvbc)
{
    __mp        *tmpHdle;
    if(dvbc)
    {
        MESSAGE("DVBC: close all plug-in in cedar sub system!\n");

        //* stop all plug in
        stop_plugins(dvbc);
        
#if PLAY_SUBTITLE_STREAM
        if(dvbc->sply)
        {
            MESSAGE("close subtitle playback...\n");
            esMODS_MClose(dvbc->sply);
            dvbc->sply = NULL;
        }
#endif

        if(dvbc->vply)
        {
            MESSAGE("close video playback...\n");
            //vply_MClose(dvbc->vply);
            esMODS_MClose(dvbc->vply);
            dvbc->vply = NULL;
        }

        if(dvbc->aply)
        {
            MESSAGE("close audio playback...\n");
            esMODS_MClose(dvbc->aply);
            dvbc->aply = NULL;
        }

        //close audio render plug-in
        if(dvbc->ardr)
        {
            MESSAGE("close audio render...\n");
            tmpHdle = dvbc->ardr;
            dvbc->ardr = (__mp*)0;
            esMODS_MClose(tmpHdle);
        }
#if PLAY_SUBTITLE_STREAM
        if(dvbc->sdec)
        {
            MESSAGE("close subtitle decoder...\n");
            esMODS_MClose(dvbc->sdec);
            dvbc->sdec = NULL;
        }
#endif

        if(dvbc->vdec)
        {
            MESSAGE("close video decoder...\n");
            esMODS_MClose(dvbc->vdec);
            dvbc->vdec = NULL;
        }

        if(dvbc->adec)
        {
            MESSAGE("close audio decoder...\n");
            esMODS_MClose(dvbc->adec);
            dvbc->adec = NULL;
        }

        if(dvbc->avsync)
        {
            MESSAGE("close av sync...\n");
            esMODS_MClose(dvbc->avsync);
            dvbc->avsync = NULL;
        }
    }

    return;
}


/*
*********************************************************************************************************
*                                        UNINSTALL ALL PLUG-IN
*
* Description: Uninstall all plug-in.
*
* Arguments  : cedarCore - the module handle
*
* Returns    : none
*********************************************************************************************************
*/
void uninstall_plugins(__dvbc_t *dvbc)
{
    //* stop and close all plug-in
    close_plugins(dvbc);

    //* unistall plug-in
    if(dvbc)
    {
        MESSAGE("uninstall_plugins: uninstall all plug-in in cedar sub system!\n");
        
#if PLAY_SUBTITLE_STREAM
        if(dvbc->sply_mid)
        {
            MESSAGE("uninstall subtitle playback...\n");
            esMODS_MUninstall(dvbc->sply_mid);
            dvbc->sply_mid = 0;
        }
#endif

        if(dvbc->vply_mid)
        {
            MESSAGE("uninstall video playback...\n");
            //vply_MExit();
            esMODS_MUninstall(dvbc->vply_mid);
            dvbc->vply_mid = 0;
        }

        if(dvbc->aply_mid)
        {
            MESSAGE("uninstall audio playback...\n");
            //aply_MExit();
            esMODS_MUninstall(dvbc->aply_mid);
            dvbc->aply_mid = 0;
        }

        //uninstall audio render plug-in
        if(dvbc->ardr_mid)
        {
            MESSAGE("uninstall audio render...\n");
            esMODS_MUninstall(dvbc->ardr_mid);
            dvbc->ardr_mid = 0;
        }
#if PLAY_SUBTITLE_STREAM
        if(dvbc->sdec_mid)
        {
            MESSAGE("uninstall subtitle decoder...\n");
            esMODS_MUninstall(dvbc->sdec_mid);
            dvbc->sdec_mid = 0;
        }
#endif

        if(dvbc->vdec_mid)
        {
            MESSAGE("uninstall video decoder...\n");
            esMODS_MUninstall(dvbc->vdec_mid);
            dvbc->vdec_mid = 0;
        }

        if(dvbc->adec_mid)
        {
            MESSAGE("uninstall audio decoder...\n");
            esMODS_MUninstall(dvbc->adec_mid);
            dvbc->adec_mid = 0;
        }

        if(dvbc->avsync_mid)
        {
            MESSAGE("uninstall av sync...\n");
            esMODS_MUninstall(dvbc->avsync_mid);
            dvbc->avsync_mid = 0;
        }
    }
    
    dvbc->getAdFmt   = 0;
    dvbc->getVdFmt   = 0;
    dvbc->getSubtFmt = 0;

    return;
}

/*******************************************************************************
Function name: QuickStopPlugins
Description: 
Parameters: 
    
Return: 
    
Time: 2011/9/30
*******************************************************************************/
__s32 QuickStop_plugins(__dvbc_t *dvbc)
{
   // __mp    *tmpHdle;
    __s32   tmpret;
    if(dvbc)
    {
        MESSAGE("dvbc:close all plug-in in cedar sub system!\n");
        stop_plugins(dvbc);
        
        //开始个插件IO_Close的过程
        //close sply, video and audio playback plug-in
#if PLAY_SUBTITLE_STREAM

        if(dvbc->sply)
        {
            MESSAGE("close subtitle playback...\n");
            esMODS_MClose(dvbc->sply);
            dvbc->sply = NULL;
        }
#endif
        if(dvbc->vply)
        {
            if(CEDAR_FILE_SWITCH_VPLY_MODE_GAP == dvbc->nFileSwitchVplyMode)
            {
                tmpret = esMODS_MIoctrl(dvbc->vply, CEDAR_VPLY_CMD_CLOSE, 0, NULL);
                if(tmpret != EPDK_OK)
                {
                    WARNING("fatal error! vply IO_Close() fail\n");
                }
            }
        }
        //aply没有IO_Close(),stop就可以了
        if(dvbc->aply)
        {
            tmpret = esMODS_MIoctrl(dvbc->aply, CEDAR_APLY_CMD_CLOSE, 0, NULL);
            if(tmpret != EPDK_OK)
            {
                WARNING("fatal error! aply IO_Close() fail\n");
            }
        }

        //close audio render plug-in
        //ardr没有IO_Close(),stop就可以了
        if(dvbc->ardr)
        {
            tmpret = esMODS_MIoctrl(dvbc->ardr, CEDAR_ARDR_CMD_CLOSE, 0, NULL);
            if(tmpret != EPDK_OK)
            {
                WARNING("fatal error! ardr IO_Close() fail\n");
            }
        }

        //close sub decoder, video decoder and audio decoder plug-in
       
#if PLAY_SUBTITLE_STREAM        
        if(dvbc->sdec)
        {
            if(EPDK_OK!=esMODS_MIoctrl(dvbc->sdec, CEDAR_LDEC_CMD_CLOSE, 0, NULL))    
            {
                WARNING("fatal error! ldec_itxt decoder IO_Close fail!\n");
            }
        }
#endif
        //vdec要注意,要调用IO_Close()
        if(dvbc->vdec)
        {
            tmpret = esMODS_MIoctrl(dvbc->vdec, CEDAR_VDEC_CMD_CLOSE, 0, NULL);
            if(tmpret != EPDK_OK)
            {
                WARNING("fatal error! vdec IO_Close() fail\n");
            }
        }
        //adec要调用IO_Close()
        if(dvbc->adec)
        {
            tmpret = esMODS_MIoctrl(dvbc->adec, CEDAR_ADEC_CMD_CLOSE, 0, NULL);
            if(tmpret != EPDK_OK)
            {
                WARNING("fatal error! adec IO_Close() fail\n");
            }
        }

        //close file parser
        //adec没有IO_Close().
      //  if(dvbc->psr)
      // {
      //      tmpret = esMODS_MIoctrl(dvbc->psr, CEDAR_PSR_CMD_CLOSE, 0, NULL);
      //      if(tmpret != EPDK_OK)
      //      {
      //          WARNING("fatal error! psr IO_Close fail\n");
      //      }

      //  }

        //close av sync
        //avsync没有IO_Close().
        if(dvbc->avsync)
        {
            tmpret = esMODS_MIoctrl(dvbc->avsync, DRV_AVS_CMD_CLOSE, 0, NULL);
            if(tmpret != EPDK_OK)
            {
                WARNING("fatal error! avsync IO_Close() fail\n");
            }
        }


        //uninstall subtitle, video and audio playback plug-in

#if PLAY_SUBTITLE_STREAM
        if(dvbc->sply_mid)
        {
            MESSAGE("uninstall subtitle playback...\n");
            esMODS_MUninstall(dvbc->sply_mid);
            dvbc->sply_mid = 0;
        }
#endif

    }

    return EPDK_OK;
    
}

/*******************************************************************************
Function name: FileStopProcess
Description: 
    two playmodes: keeping some plugins in the memery and uninstall all plugins.the front one will be quickly when you switch the program.
Parameters: 
    
Return: 
    
Time: 2011/9/29
*******************************************************************************/
__s32 FileStop_process(__dvbc_t * dvbc)
{
    __s32 nStopMode = dvbc->nStopMode;
    if(CEDAR_STOP_MODE_KEEP_PLUGINS == nStopMode)
    {
        QuickStop_plugins(dvbc);
       
    }
    else
    {
        uninstall_plugins(dvbc);
    }
    return EPDK_OK;
}

/*******************************************************************************
Function name: request_mem
Description: 
    1.负责向vdec, adec pend socket,socket一般携带空闲内存,空闲内存就是dmx所要的.
    2.socket一般不会马上post,等到update_data才会post回去.
    3.本函数会被dmx的多个channel的线程调用,必须做到可重入,必须相当小心.该互斥的地方就要互斥.
Parameters: 
    
Return: 
    
Time: 2011/12/21
*******************************************************************************/
static __s32 request_mem(__dvbc_t *dvbctrl, __s32 md_type, __md_buf_t* md_buf)
{
    __krnl_sktfrm_t *skt = NULL;

	if(dvbctrl->status != DVB_STAT_PLAY)
	{
    	md_buf->buf = NULL;
    	md_buf->bufSize = 0;
		return EPDK_FAIL;
	}

    if(md_type == MEDIA_TYPE_VIDEO)
    {
#if (PLAY_VIDEO_STREAM == 0)
    	md_buf->buf 	= dvbc->tmpBuf;
    	md_buf->bufSize = 128*1024;
		return EPDK_OK;
#else
        skt = dvbctrl->vbs_frm;
        
        if(skt)
        {
    		md_buf->buf 	= skt->data;
    		md_buf->bufSize = skt->datsize;
    		return EPDK_OK;
        }
        
        while(1)
        {
            skt = esKRNL_SktPend(dvbctrl->pVBSSktout, KRNL_SKT_USR_IN, 50);
            if(skt == NULL)
            {
                WARNING("pend video socket frame timeout.\n");
                md_buf->buf 	= NULL;
                md_buf->bufSize = 0;
                return EPDK_FAIL;
            }
            
            if(skt->msgtype != PSR2VDEC_MSG_TYPE_NONE)
            {
                WARNING("accept message (%x) from video decoder, discard it!\n", skt->msgtype);
                if(skt->msgtype == PSR2VDEC_MSG_TYPE_FATAL_ERR)
                {
                    WARNING("truely meet fatal error? check immediately!!!\n");
                    //* reset video
                    skt->msgtype 	= PSR2VDEC_MSG_TYPE_NONE;
                    skt->datatype 	= CEDAR_DATATYPE_NONE;
                    skt->datalen 	= 0;
                    esKRNL_SktPost(dvbctrl->pVBSSktout, KRNL_SKT_USR_IN, skt);
                    skt = NULL;
                    reset_video(dvbctrl);
                    continue;
                }
            }
            
            if(skt->datsize == 0)
            {
                WARNING("skt->datsize = 0 when request video bitstream memory, try again!\n");
                skt->msgtype 	= PSR2VDEC_MSG_TYPE_NONE;
                skt->datatype 	= CEDAR_DATATYPE_NONE;
                skt->datalen 	= 0;
                esKRNL_SktPost(dvbctrl->pVBSSktout, KRNL_SKT_USR_IN, skt);
                skt = NULL;
                continue;
            }
            
            break;
        }
        
        dvbctrl->vbs_frm = skt;
        md_buf->buf 	= skt->data;
        md_buf->bufSize = skt->datsize;
        return EPDK_OK;
#endif
    }
    else if (md_type == MEDIA_TYPE_AUDIO)
    {
#if (PLAY_AUDIO_STREAM == 0)
    	md_buf->buf 	= dvbc->tmpBuf;
    	md_buf->bufSize = 128*1024;
		return EPDK_OK;
#else
        skt = dvbctrl->abs_frm;
        
        if(skt)
        {
    		md_buf->buf 	= skt->data;
    		md_buf->bufSize = skt->datsize;
    		return EPDK_OK;
        }
        
        while(1)
        {
            skt = esKRNL_SktPend(dvbctrl->pABSSktout, KRNL_SKT_USR_IN, 50);
            if(skt == NULL)
            {
                WARNING("pend audio socket frame timeout.\n");
                md_buf->buf 	= NULL;
                md_buf->bufSize = 0;
                return EPDK_FAIL;
            }
            
            if(skt->msgtype != PSR2ADEC_MSG_TYPE_NONE)
            {
                //一般是PSR2ADEC_MSG_TYPE_FRMNUM
                //WARNING("accept message (%d) from audio decoder, discard it!\n", skt->msgtype);
            }
                
            if(skt->datsize == 0)
            {
                //send empty frame to audio decoder to get free buffer
                WARNING("skt->datsize = 0 when request audio bitstream memory, try again!\n");
                skt->msgtype = PSR2ADEC_MSG_TYPE_NONE;
                skt->datatype = CEDAR_DATATYPE_NONE;
                skt->datalen = 0;
                esKRNL_SktPost(dvbctrl->pABSSktout, KRNL_SKT_USR_IN, skt);
                continue;
            }
            
            break;
        }

        dvbctrl->abs_frm = skt;
        md_buf->buf 	= skt->data;
        md_buf->bufSize = skt->datsize;
        return EPDK_OK;
#endif
    }
    else if (md_type == MEDIA_TYPE_SUBTITLE)
    {
#if (PLAY_SUBTITLE_STREAM == 0)
        md_buf->buf 	= dvbc->tmpSubtitleBuf;
    	md_buf->bufSize = 128*1024;
		return EPDK_OK;
#else
        skt = dvbctrl->sbs_frm;

        if(skt)
        {
    		md_buf->buf 	= skt->data;
    		md_buf->bufSize = skt->datsize;
    		return EPDK_OK;
        }
        
        while(1)
        {
            skt = esKRNL_SktPend(dvbctrl->pLBSSktout, KRNL_SKT_USR_IN, 50);
            if(skt == NULL)
            {
                WARNING("pend subtitle socket frame timeout.\n");
                md_buf->buf 	= NULL;
                md_buf->bufSize = 0;
                return EPDK_FAIL;
            }
            
            if(skt->msgtype != PSR2LDEC_MSG_TYPE_NONE)
            {
                //一般是PSR2LDEC_MSG_TYPE_NONE
                WARNING("accept message (%d) from subtitle decoder, discard it!\n", skt->msgtype);
            }
                
            if(skt->datsize == 0)
            {
                //send empty frame to subtitle decoder to get free buffer
                WARNING("skt->datsize = 0 when request subtitle bitstream memory, try again!\n");
                skt->msgtype = PSR2LDEC_MSG_TYPE_NONE;
                skt->datatype = CEDAR_DATATYPE_NONE;
                skt->datalen = 0;
                esKRNL_SktPost(dvbctrl->pLBSSktout, KRNL_SKT_USR_IN, skt);
                continue;
            }
            
            break;
        }

        dvbctrl->sbs_frm = skt;
        md_buf->buf 	= skt->data;
        md_buf->bufSize = skt->datsize;
        return EPDK_OK;
#endif
    }
}

#if 0
#if 1
static __inline __s64 tranPts(__s64 pts, __s8 type)
{
	__s64 dstPts;
	__s64 timeDiff;

	if(type == 1)   //* audio
	{
		if(dvbc->adPtsBase == 0)
		{
			dvbc->adPtsBase = dvbc->ptsBase;
			dvbc->adPts 	= pts;
			dvbc->firstAdPts = pts - dvbc->adPtsBase;

			WARNING("===================================== first audio pts = %d.\n", (__u32)dvbc->firstAdPts);

			if(dvbc->vdFmt.codec_type != DVB_VBS_TYPE_NONE)
			{
				if(dvbc->firstVdPts != 0)
				{
					__u32 systime;

					systime = (__u32)((dvbc->firstAdPts < dvbc->firstVdPts) ? dvbc->firstAdPts : dvbc->firstVdPts);
					WARNING("============================= set system clock to %d.\n", systime - TIME_BEFORE_PLAY);
    				esMODS_MIoctrl(dvbc->avsync, 
						   			DRV_AVS_CMD_SET_VID_TIME, 
						   			DRV_AVS_TIME_TOTAL, 
						   			(void *)(systime - TIME_BEFORE_PLAY));
		
    				esMODS_MIoctrl(dvbc->avsync, DRV_AVS_CMD_START, AVS_VIDEO, 0);
				}
			}
			else
			{
				__u32 systime;

				systime = (__u32)(dvbc->firstAdPts);
				WARNING("============================= set system clock to %d.\n", systime - TIME_BEFORE_PLAY);
    			esMODS_MIoctrl(dvbc->avsync, 
						   		DRV_AVS_CMD_SET_VID_TIME, 
						   		DRV_AVS_TIME_TOTAL, 
						   		(void *)(systime - TIME_BEFORE_PLAY));
		
    			esMODS_MIoctrl(dvbc->avsync, DRV_AVS_CMD_START, AVS_VIDEO, 0);
			}
		}
		
		timeDiff = pts - dvbc->adPts;
//		WARNING("audio pts = %d, diff = %d.\n", (__u32)pts, timeDiff);
		if(timeDiff < -5000)
		{
			WARNING("audio pts loop back, pts = %d, pre_pts = %d.\n", (__u32)pts, (__u32)dvbc->adPts);
			if(dvbc->ptsBase == dvbc->adPtsBase)
			{
				//* adjust ptsBase to process audio pts loop back.
				dvbc->ptsBase 	= pts + dvbc->ptsBase - dvbc->adPts - PTS_DELTA_AUDIO;
				dvbc->adPtsBase = dvbc->ptsBase;
			}
			else
			{
				//* ptsBase has been adjusted by video pts loop back processing.
				dvbc->adPtsBase = dvbc->ptsBase;
			}
		}
		
		dstPts 		= pts - dvbc->adPtsBase;
		dvbc->adPts = pts;
	}
	else if(type == 0)  //* video
	{
		if(dvbc->vdPtsBase == 0)
		{
			dvbc->vdPtsBase = dvbc->ptsBase;
			dvbc->vdPts 	= pts;
			dvbc->firstVdPts = pts - dvbc->vdPtsBase;

			WARNING("===================================== first video pts = %d.\n", (__u32)dvbc->firstVdPts);

			if(dvbc->adFmt.codec_type != DVB_ABS_TYPE_NONE)
			{
				if(dvbc->firstAdPts != 0)
				{
					__u32 systime;

					systime = (__u32)((dvbc->firstAdPts < dvbc->firstVdPts) ? dvbc->firstAdPts : dvbc->firstVdPts);
					WARNING("============================= set system clock to %d.\n", systime - TIME_BEFORE_PLAY);
    				esMODS_MIoctrl(dvbc->avsync,
						   			DRV_AVS_CMD_SET_VID_TIME, 
						   			DRV_AVS_TIME_TOTAL, 
						   			(void *)(systime - TIME_BEFORE_PLAY));
		
    				esMODS_MIoctrl(dvbc->avsync, DRV_AVS_CMD_START, AVS_VIDEO, 0);
				}
			}
			else
			{
				__u32 systime;
				systime = (__u32)(dvbc->firstVdPts);
				WARNING("============================= set system clock to %d.\n", systime - TIME_BEFORE_PLAY);
    			esMODS_MIoctrl(dvbc->avsync,
						   		DRV_AVS_CMD_SET_VID_TIME, 
						   		DRV_AVS_TIME_TOTAL, 
						   		(void *)(systime - TIME_BEFORE_PLAY));
		
    			esMODS_MIoctrl(dvbc->avsync, DRV_AVS_CMD_START, AVS_VIDEO, 0);
			}

		}
		
		timeDiff = pts - dvbc->vdPts;
		if(timeDiff < -5000)
		{
			//* prc loop, adjust pts base
			WARNING("video pts loop back, pts = %d, pre_pts = %d.\n", (__u32)pts, (__u32)dvbc->vdPts);
			if(dvbc->ptsBase == dvbc->vdPtsBase)
			{
				//* adjust ptsBase to process video pts loop back.
				dvbc->ptsBase 	= pts + dvbc->ptsBase - dvbc->vdPts - PTS_DELTA_VIDEO;
				dvbc->vdPtsBase = dvbc->ptsBase;
			}
			else
			{
				//* ptsBase has been adjusted by audio pts loop back processing.
				dvbc->vdPtsBase = dvbc->ptsBase;
			}
		}
			
		dstPts 		= pts - dvbc->vdPtsBase;
		dvbc->vdPts = pts;
	}
	else    //* subtitle
	{
#if PLAY_SUBTITLE_STREAM
		if(dvbc->subtPtsBase == 0)
		{
			dvbc->subtPtsBase   = dvbc->ptsBase;
			dvbc->subtPts 	    = pts;
			dvbc->firstSubtPts  = pts - dvbc->subtPtsBase;

			WARNING("===================================== first subtitle pts = %d.\n", (__u32)dvbc->firstSubtPts);
		}
		
		timeDiff = pts - dvbc->subtPts;
		if(timeDiff < -5000)
		{
			//* prc loop, adjust pts base
			WARNING("subtitle pts loop back, pts = %x, pre_pts = %x.\n", (__u32)pts, (__u32)dvbc->subtPts);
			if(dvbc->ptsBase == dvbc->subtPtsBase)
			{
				//* adjust ptsBase to process video pts loop back.
				dvbc->ptsBase 	= pts + dvbc->ptsBase - dvbc->subtPts - PTS_DELTA_SUBTITLE;
				dvbc->subtPtsBase = dvbc->ptsBase;
			}
			else
			{
				//* ptsBase has been adjusted by audio pts loop back processing.
				dvbc->subtPtsBase = dvbc->ptsBase;
			}
		}
			
		dstPts 		  = pts - dvbc->subtPtsBase;
		dvbc->subtPts = pts;
#else
        dstPts = 0;
#endif
	}

	return dstPts;
}
#else
static __inline __s64 tranPts(__s64 pts, __s8 type)
{
    __s64 diff0;
    __s64 diff1;
    __s64 dstPts;
    
    dstPts = 0;
	if(type == 1)   //* audio
	{
	    if(dvbc->preAdPts[0] != 0 && dvbc->preAdPts[1] != 0)
	    {
	        diff0 = pts - dvbc->preAdPts[0];
	        diff1 = pts - dvbc->preAdPts[1];
	        if(diff0>-5000 && diff0<5000 && diff1>-5000 && diff1<5000)
	            dstPts = pts;
	    }
	    else
	        dstPts = pts;
	    
	    dvbc->preAdPts[0] = dvbc->preAdPts[1];
	    dvbc->preAdPts[1] = pts;
	}
	else if(type == 0)
	{
	    if(dvbc->preVdPts[0] != 0 && dvbc->preVdPts[1] != 0)
	    {
	        diff0 = pts - dvbc->preVdPts[0];
	        diff1 = pts - dvbc->preVdPts[1];
	        if(diff0>-5000 && diff0<5000 && diff1>-5000 && diff1<5000)
	            dstPts = pts;
	    }
	    else
	        dstPts = pts;
	            
	    dvbc->preVdPts[0] = dvbc->preVdPts[1];
	    dvbc->preVdPts[1] = pts;
	}
	else
	{
	    dstPts = pts;
	}
	
	return dstPts;
}
#endif
#endif
/*******************************************************************************
Function name: translatePts
Description: 
    1.被update_data()调用, update_data()要求多线程并发执行,可重入.这里尤其小心.
    2.分video, audio, subtitle3种。一个线程只能执行一种.
    3.要判断是否回头,并且指示出来.
    4.可能需要在这里改变的变量有:
        dvbctrl->StreamStartPts
        AccumulateStartPts

     video:
        vdStreamStartPts
     audio:
        adStreamStartPts
     subtitle:
        ...
    5.执行到本函数时,默认上述变量值已经确定。
        还有vdStreamPts等

        但是vdPts第一次是默认为0,不赋值的.
Parameters: 
    1.nStreamPts:原始的stream pts.
    2.type: MEDIA_TYPE_VIDEO等
    3.pnProcessReturnHeadFlag: 是否处理了回头的标记位,所谓处理,是指改变了dvbctrl的一些公共变量,如StreamStartPts, AccumulateStartPts.
    
Return: 
    是减去ptsBase,然后做过回头处理的pts.
    单位ms
Time: 2011/12/21
*******************************************************************************/
static __inline __s64 translatePts(__dvbc_t *dvbctrl, __s64 nStreamPts, __s32 type, __s32 *pnPtsReturnHeadFlag)
{
    //__u8    err;
    __s32   mainMdType = MEDIA_TYPE_VIDEO;    //为准的数据类型,是否回头以它为准.
    __s64   nStreamStartPts;    //一个stream中的起始的pts,取原始值.以video认为的为准,其次是audio
    __s64   nAccumulateStartPts;    //累积起始时间,每次回头以后，需要修改.该时间优先以video为准,并且需要减去ptsBase, 考虑回头.送到后面解码器的时间就以它为基础计算.
    __s64   nLocalPts = 0;  //这不是原始值,这是减去ptsBase并经过回头处理后的值,这个值会送到后面的vdec, adec去.
    *pnPtsReturnHeadFlag = 0;
    //1.确定有资格改变StreamStartPts和AccumulateStartPts的数据类型
    if(dvbctrl->hasVideo)
    {
        mainMdType = MEDIA_TYPE_VIDEO;
    }
    else if(dvbctrl->hasAudio)
    {
        mainMdType = MEDIA_TYPE_AUDIO;
    }
    else if(dvbctrl->hasSubtitle)
    {
        mainMdType = MEDIA_TYPE_SUBTITLE;
    }
    else
    {
        WARNING("[%s], fatal error! no video, no audio, subtitle!\n", __FUNCTION__);
    }
    //因为该函数允许并发,所以这里有必要先记录.下面的一些比较怪的代码都是如此目的
    nStreamStartPts = dvbctrl->StreamStartPts;
    nAccumulateStartPts = dvbctrl->AccumulateStartPts;
    if(MEDIA_TYPE_VIDEO == type)
    {
    	//eLIBs_printf("video:pts[%qd]\n",nStreamPts);
        //判断是否回头,如果回头,准备做大量处理,时间向前跳跃超过一定限度,也认为是PTS回头, PTS回头实质上就是更换时间轴.
        if(nStreamPts + PTS_ERROR_SCOPE_VIDEO < dvbctrl->vdStreamPts 
            || nStreamPts > dvbctrl->vdStreamPts + PTS_ERROR_SCOPE_VIDEO)  //回头了,改变共用变量AccumulateStartPts和StreamStartPts
        {
            WARNING("detect video pts return head! [%qd]->[%qd]\n", dvbctrl->vdStreamPts, nStreamPts);
            if(nStreamPts > dvbctrl->vdStreamPts + PTS_ERROR_SCOPE_VIDEO)
            {
                WARNING("video_pts jump forward [%qd]ms!\n", nStreamPts - dvbctrl->vdStreamPts);
            }
            dvbctrl->vdStreamStartPts = nStreamPts;
            *pnPtsReturnHeadFlag = 1;
            if(MEDIA_TYPE_VIDEO == mainMdType)
            {
                WARNING("video process return head!\n");
                //互斥有必要时再打开
                //dvbctrl->pSemStreamPts->SemPend(dvbctrl->pSemStreamPts, 0, &err);
                dvbctrl->AccumulateStartPts += (dvbctrl->vdStreamPts - dvbctrl->StreamStartPts  + PTS_DELTA_VIDEO);   //PTS_DELTA_VIDEO
                dvbctrl->StreamStartPts = nStreamPts;
                nStreamStartPts = dvbctrl->StreamStartPts;
                nAccumulateStartPts = dvbctrl->AccumulateStartPts;
                //dvbctrl->pSemStreamPts->SemPost(dvbctrl->pSemStreamPts);
            }
            else
            {
                WARNING("fatal error! mainMdType != MEDIA_TYPE_VIDEO\n");
            }
        }
        nLocalPts = nAccumulateStartPts + (nStreamPts - nStreamStartPts);
        dvbctrl->vdStreamPts = nStreamPts;
        if(dvbctrl->vdPts > nLocalPts)
        {
            if(dvbctrl->vdPts - nLocalPts >= 200)
            {
                //不做任何处理, 虽然有可能会出现.如果B帧的pts打得很准的话.
                WARNING("impossible? prev localvdPts[%qd],nLocalPts[%qd], [%qd]>=200ms\n", dvbctrl->vdPts, nLocalPts, dvbctrl->vdPts - nLocalPts);
            }
        }
        dvbctrl->vdPts = nLocalPts;
        //WARNING("localPts[%qd], StreamPts[%qd]\n", nLocalPts, nStreamPts);
    }
    else if(MEDIA_TYPE_AUDIO == type)
    {
    	//eLIBs_printf("audio:pts[%qd]\n",nStreamPts);
        if(nStreamPts + PTS_ERROR_SCOPE_AUDIO < dvbctrl->adStreamPts 
            || nStreamPts  > dvbctrl->adStreamPts + PTS_ERROR_SCOPE_AUDIO)
        {
            WARNING("detect audio pts return head! [%qd]->[%qd]\n", dvbctrl->adStreamPts, nStreamPts);
            if(nStreamPts  > dvbctrl->adStreamPts + PTS_ERROR_SCOPE_AUDIO)
            {
                WARNING("audio_pts jump forward [%qd]ms!\n", nStreamPts - dvbctrl->adStreamPts);
            }
            dvbctrl->adStreamStartPts = nStreamPts;
            *pnPtsReturnHeadFlag = 1;
            //如果mainMdType == MEDIA_TYPE_AUDIO,进行回头处理
            if(MEDIA_TYPE_AUDIO == mainMdType)
            {
                WARNING("audio process return head!\n");
                //互斥有必要时再打开
                //dvbctrl->pSemStreamPts->SemPend(dvbctrl->pSemStreamPts, 0, &err);
                dvbctrl->AccumulateStartPts += (dvbctrl->adStreamPts - dvbctrl->StreamStartPts  + PTS_DELTA_AUDIO);   //PTS_DELTA_AUDIO
                dvbctrl->StreamStartPts = nStreamPts;
                nStreamStartPts = dvbctrl->StreamStartPts;
                nAccumulateStartPts = dvbctrl->AccumulateStartPts;
                //dvbctrl->pSemStreamPts->SemPost(dvbctrl->pSemStreamPts);
            }
        }
        nLocalPts = nAccumulateStartPts + (nStreamPts - nStreamStartPts);
        dvbctrl->adStreamPts = nStreamPts;
        if(dvbctrl->adPts > nLocalPts)
        {
            //当前的规则:有video, 则audio无权修改StreamStartPts和AccumulateStartPts.所以在audio先回头时,较有可能出现当前的pts比上次的adPts小的情况.
            //这时不管,因为audio是可以随时校正audio clock的.错了也不怕.
            WARNING("find audio prev localadPts[%qd] > nLocalPts[%qd]=[%qd], returnflag[%d]!\n", dvbctrl->adPts, nLocalPts,dvbctrl->adPts-nLocalPts, *pnPtsReturnHeadFlag);
            //nLocalPts = dvbctrl->adPts;
            if(nLocalPts < 0)
            {
                nLocalPts = 0;
            }
        }
        dvbctrl->adPts = nLocalPts;
        //WARNING("AudiolocalPts[%qd], StreamPts[%qd]\n", nLocalPts, nStreamPts);
    }
    else if(MEDIA_TYPE_SUBTITLE == type)
    {
    		if((nStreamPts == 0)||(nStreamPts - dvbctrl->vdStreamPts >5000)||(dvbctrl->vdStreamPts-nStreamPts >5000))
    		{
    				nStreamPts = dvbctrl->vdStreamPts;
						WARNING("Be carefull!StreamPts[%qd] > StreamPts[%qd]?\n",nStreamPts,dvbctrl->vdStreamPts);
				}
        if(nStreamPts + PTS_ERROR_SCOPE_SUBTITLE < dvbctrl->subStreamPts)
        {
            WARNING("detect subtitle pts return head! [%qd]->[%qd]\n", dvbctrl->subStreamPts, nStreamPts);
            dvbctrl->subStreamStartPts = nStreamPts;
            dvbctrl->subStreamPts = nStreamPts;
            *pnPtsReturnHeadFlag = 1;
            //如果mainMdType == MEDIA_TYPE_AUDIO,进行回头处理
//            if(MEDIA_TYPE_AUDIO == mainMdType)
//            {
//                WARNING("subtitle process return head!\n");
                //互斥有必要时再打开
                //dvbctrl->pSemStreamPts->SemPend(dvbctrl->pSemStreamPts, 0, &err);
//                dvbctrl->AccumulateStartPts += (dvbctrl->subStreamPts - dvbctrl->StreamStartPts  + PTS_DELTA_AUDIO);   //PTS_DELTA_AUDIO
//                dvbctrl->StreamStartPts = nStreamPts;
//                nStreamStartPts = dvbctrl->StreamStartPts;
//                nAccumulateStartPts = dvbctrl->AccumulateStartPts;
                //dvbctrl->pSemStreamPts->SemPost(dvbctrl->pSemStreamPts);
//            }
            //考虑到如果字幕先回头,而视频没有回头,会导致第一条字幕因为时间戳过小,直接被skip掉而无法显示出来.
            //这里针对有视频的情况作一个特殊处理.针对码流[ISR]TMCC_003.TS.
            if(mainMdType == MEDIA_TYPE_VIDEO)
            {
                //先判断这时video有无回头,就比较video的当前原始vdStreamPts和字幕的nStreamPts的差值,大于60秒才认为是字幕先回头.
                if(dvbctrl->vdStreamPts > dvbctrl->subStreamPts + 60000)
                {
                    //如果字幕先回头,回头后的第一条字幕的起始pts临时用当前的vdStreamPts - nStreamStartPts来算.
                    WARNING("subStreamPts[%d] return! but vdStreamPts[%d] still not!\n", (__s32)dvbctrl->subStreamPts, (__s32)dvbctrl->vdStreamPts);
                    nLocalPts = nAccumulateStartPts + (dvbctrl->vdStreamPts - nStreamStartPts + PTS_DELTA_VIDEO);
                }
                else
                {
                    nLocalPts = nAccumulateStartPts + (nStreamPts - nStreamStartPts);
                }
            }
            else
            {
                nLocalPts = nAccumulateStartPts + (nStreamPts - nStreamStartPts);
            }
        }
        else
        {
            dvbctrl->subStreamPts = nStreamPts;
            nLocalPts = nAccumulateStartPts + (nStreamPts - nStreamStartPts);
        }
        
        if(dvbctrl->subPts > nLocalPts)
        {
            //当前的规则:有video, 则audio无权修改StreamStartPts和AccumulateStartPts.所以在audio先回头时,较有可能出现当前的pts比上次的adPts小的情况.
            //这时不管,因为audio是可以随时校正audio clock的.错了也不怕.
            WARNING("find subtitle prev localadPts[%qd] > nLocalPts[%qd], returnflag[%d]!\n", dvbctrl->subPts, nLocalPts, *pnPtsReturnHeadFlag);
            //nLocalPts = dvbctrl->adPts;
            if(nLocalPts < 0)
            {
                nLocalPts = 0;
            }
        }
        dvbctrl->subPts = nLocalPts;
        //WARNING("SubtitlelocalPts[%qd], StreamPts[%qd]\n", nLocalPts, nStreamPts);
    }
		#if 0
		else if(MEDIA_TYPE_SUBTITLE == type)
    {
        if(nStreamPts + PTS_ERROR_SCOPE_SUBTITLE < dvbctrl->subStreamPts)
        {
            WARNING("detect subtitle pts return head! [%qd]->[%qd]\n", dvbctrl->subStreamPts, nStreamPts);
            dvbctrl->subStreamStartPts = nStreamPts;
            dvbctrl->subStreamPts = nStreamPts;
            *pnPtsReturnHeadFlag = 1;
            //如果mainMdType == MEDIA_TYPE_AUDIO,进行回头处理
//            if(MEDIA_TYPE_AUDIO == mainMdType)
//            {
//                WARNING("subtitle process return head!\n");
                //互斥有必要时再打开
                //dvbctrl->pSemStreamPts->SemPend(dvbctrl->pSemStreamPts, 0, &err);
//                dvbctrl->AccumulateStartPts += (dvbctrl->subStreamPts - dvbctrl->StreamStartPts  + PTS_DELTA_AUDIO);   //PTS_DELTA_AUDIO
//                dvbctrl->StreamStartPts = nStreamPts;
//                nStreamStartPts = dvbctrl->StreamStartPts;
//                nAccumulateStartPts = dvbctrl->AccumulateStartPts;
                //dvbctrl->pSemStreamPts->SemPost(dvbctrl->pSemStreamPts);
//            }
            //考虑到如果字幕先回头,而视频没有回头,会导致第一条字幕因为时间戳过小,直接被skip掉而无法显示出来.
            //这里针对有视频的情况作一个特殊处理.针对码流[ISR]TMCC_003.TS.
            if(mainMdType == MEDIA_TYPE_VIDEO)
            {
                //先判断这时video有无回头,就比较video的当前原始vdStreamPts和字幕的nStreamPts的差值,大于60秒才认为是字幕先回头.
                if(dvbctrl->vdStreamPts > dvbctrl->subStreamPts + 60000)
                {
                    //如果字幕先回头,回头后的第一条字幕的起始pts临时用当前的vdStreamPts - nStreamStartPts来算.
                    WARNING("subStreamPts[%d] return! but vdStreamPts[%d] still not!\n", (__s32)dvbctrl->subStreamPts, (__s32)dvbctrl->vdStreamPts);
                    nLocalPts = nAccumulateStartPts + (dvbctrl->vdStreamPts - nStreamStartPts + PTS_DELTA_VIDEO);
                }
                else
                {
                    nLocalPts = nAccumulateStartPts + (nStreamPts - nStreamStartPts);
                }
            }
            else
            {
                nLocalPts = nAccumulateStartPts + (nStreamPts - nStreamStartPts);
            }
        }
        else
        {
            dvbctrl->subStreamPts = nStreamPts;
            nLocalPts = nAccumulateStartPts + (nStreamPts - nStreamStartPts);
        }
        
        if(dvbctrl->subPts > nLocalPts)
        {
            //当前的规则:有video, 则audio无权修改StreamStartPts和AccumulateStartPts.所以在audio先回头时,较有可能出现当前的pts比上次的adPts小的情况.
            //这时不管,因为audio是可以随时校正audio clock的.错了也不怕.
            WARNING("find subtitle prev localadPts[%qd] > nLocalPts[%qd], returnflag[%d]!\n", dvbctrl->subPts, nLocalPts, *pnPtsReturnHeadFlag);
            //nLocalPts = dvbctrl->adPts;
            if(nLocalPts < 0)
            {
                nLocalPts = 0;
            }
        }
        dvbctrl->subPts = nLocalPts;
        //WARNING("SubtitlelocalPts[%qd], StreamPts[%qd]\n", nLocalPts, nStreamPts);
    }
		#endif
    else
    {
        WARNING("[%s]:fatal error! check code!", __FUNCTION__);
    }
    return nLocalPts;
}


/*******************************************************************************
Function name: update_data
Description: 
    1.本函数会被dmx的多个channel的线程调用,必须做到可重入,必须相当小心.该互斥的地方就要互斥.
    2.在一些公共变量如__dvbc_t *dvbctrl的操作上,必须小心.特别是赋值,主要是streampts,回头等,要相当注意。
    现在是锁定只能有一个类型的数据可以改变pts相关的变量.如果以后否定了这种做法,就要重新考虑.
    3.PTS回头后,就在本函数里通知vply或者aply,现在的做法是有video,就只通知vply,audioframe的pts改变后,如果差距过大,aply会自己重设audio clock来修正;
        如果只有audio,那么aply会同时修改video clock和audio clock.video clock是作为stream clock来处理的.

    4.该函数会在被调用时做互斥.所以函数里面不用担心了,可以随便写.
Parameters: 
    
Return: 
    
Time: 2011/12/21
*******************************************************************************/
static __s32 update_data(__dvbc_t *dvbctrl, __s32 md_type, __md_data_info_t* info)
{
    __krnl_sktfrm_t     *skt = NULL;
    __s32   mainMdType = MEDIA_TYPE_VIDEO;    //为准的数据类型,是否回头以它为准.确定StreamStartPts, AccumulateStartPts也以它为准
    __s32   nPtsReturnHeadFlag = 0;
	//if(md_type ==MEDIA_TYPE_VIDEO)
	//WARNING("video DATALEN = %x\n",info->dataLen);
	//if(md_type ==MEDIA_TYPE_AUDIO)
	//WARNING("audio DATALEN = %x\n",info->dataLen);
	//if(md_type ==MEDIA_TYPE_SUBTITLE)
	//WARNING("suebtitl DATALEN = %x\n",info->dataLen);
	if(dvbctrl->status != DVB_STAT_PLAY)
		return EPDK_OK;
/**/
    //check pts
    if(info->ctrlBits & PTS_VALID_BIT)
    {
        switch(md_type)
        {
            case MEDIA_TYPE_VIDEO:
            {
                WARNING("postVidPts[%d]\n", (__s32)info->pts);
                break;
            }
           case MEDIA_TYPE_AUDIO:
           {
               WARNING("postAudPts[%d]\n", (__s32)info->pts);
               break;
           }
            case MEDIA_TYPE_SUBTITLE:
           {
                WARNING("postSubPts[%d]\n", (__s32)info->pts);
               break;
            }
            default:
           {
                WARNING("fatal error! mdtype[0x%x], pts[%d]\n", md_type, (__s32)info->pts);
               break;
           }
       }
    }
    
    if(dvbctrl->hasVideo)
    {
    	  mainMdType = MEDIA_TYPE_VIDEO;
    }
    else if(dvbctrl->hasAudio)
    {   		 
        mainMdType = MEDIA_TYPE_AUDIO;
    }
    else if(dvbctrl->hasSubtitle)
    {
        WARNING("fatal error! no video, no audio, check code!\n");
        mainMdType = MEDIA_TYPE_SUBTITLE;
        return EPDK_OK;
    }
    else
    {
        WARNING("fatal error! no video, no audio, no subtitle, check code!\n");
        mainMdType = MEDIA_TYPE_SUBTITLE;
        return EPDK_OK;
        
    }
	
    //第一次拿到数据,如果有video,一律以video为准;其次以audio为准,如有问题,以后再改.
    //有一个默认前提, 如果回头,audio, video, subtitle的pts应该在很短的时间间隔内相继回头.并且回头后pts相差不大
    //这里的目的是确定ptsBase.
	if(dvbctrl->ptsBaseGet == 0)
	{
        //加一个限制,有video,就以video为准.
        if(mainMdType == md_type)
        {
		    		if(info->ctrlBits & PTS_VALID_BIT)
		    		{
		                //公用的时间变量在这里统一赋值, ptsBase, StreamStartPts, AccumulateStartPts
		    			dvbctrl->ptsBase	= info->pts - SYSTEM_CLOCK_START_TIME;	//* 10000 is an offset of pts, it is used to avoid cases that media pts small than ptsBase
		    			dvbctrl->StreamStartPts = info->pts;
		                dvbctrl->AccumulateStartPts = dvbctrl->StreamStartPts - dvbctrl->ptsBase;
		                WARNING("decide ptsBase[%qd] done! md_type[0x%x]\n", dvbctrl->ptsBase, md_type);
		                dvbctrl->ptsBaseGet = 1;
		                //如果ts本身开始处就有回头,并且只有video或者audio回头,另一方不回头就会有问题.一般不会遇到吧.遇到再说.
		    		}
		    		else
		    		{
		    			MESSAGE("ptsBase not get yet, discard media data.\n");
		    			return EPDK_OK;
		    		}
        }
        else
        {
            WARNING("[%s], md_type[0x%x] != mainMdType[0x%x], ptsBase not get, discard!\n", __FUNCTION__, md_type, mainMdType);
            return EPDK_OK;
        }
	}
    //走到这里,ptsBase已经确定了.而且如果有video,就是video的第一个pts.以后不一定会确定ptsBase.
    //* use socket to update data to decoder, code will be added later.
    if(md_type == MEDIA_TYPE_VIDEO)
    {
#if PLAY_VIDEO_STREAM
    	__psr_2_vdec_msg_t  *vdec_msg;

        skt = dvbctrl->vbs_frm;
        if(skt == NULL)
        {
            WARNING("video socket is null when update data.\n");
            return EPDK_FAIL;
        }
        
        //第一次拿到视频数据,那么必须要有pts,如果有pts,就顺便确定vdStreamStartPts, StreamStartPts, AccumulateStartPts, vdStreamPts
        //vdPts专门在一个函数translatePts()里确定.
		if(dvbctrl->nGetVdFlag == 0)    
		{
			if((info->ctrlBits & PTS_VALID_BIT) == 0)
			{
				WARNING("vdPts is not get yet, discard video data.\n");
				return EPDK_OK;
			}
            WARNING("first vdStream pts[%qd]\n", info->pts);
			dvbctrl->vdStreamStartPts = dvbctrl->vdStreamPts = info->pts;
            dvbctrl->nGetVdFlag = 1;
            //算出音视频的时间差
            if(dvbctrl->nGetAdFlag)
            {
                dvbctrl->nAVPtsInterval = dvbctrl->vdStreamStartPts - dvbctrl->adStreamStartPts;
                WARNING("AV interval[%qd]! VidLastGet\n", dvbctrl->nAVPtsInterval);
                if(dvbctrl->vply)
                {
                    esMODS_MIoctrl(dvbctrl->vply, CEDAR_VPLY_CMD_SET_AV_INTERVAL, (__s32)dvbctrl->nAVPtsInterval, NULL);
                }
                else
                {
                    WARNING("fatal error! vply = NULL!\n");
                }
            }
#if	DEBUG_TIME
			dvbctrl->newTime = esKRNL_TimeGet();
			dvbctrl->time = dvbctrl->newTime;
			WARNING("********************* receive first video data at time %d ms.\n", dvbc->newTime*10);
#endif
		}
        
        skt->datalen  = info->dataLen;
        skt->msgtype  = PSR2VDEC_MSG_TYPE_NONE;
        skt->datatype = CEDAR_DATATYPE_MID;
        
        vdec_msg = (__psr_2_vdec_msg_t *)skt->msg;
        vdec_msg->vbsctlinf.CtrlBits    = info->ctrlBits;
        if(info->ctrlBits & FIRST_PART_BIT)
        {
		    if(info->ctrlBits & PTS_VALID_BIT)
		    {
                //WARNING("vd chunk pts[%qd]\n", info->pts);
//		        __s64 tmp;
//		        tmp = vdec_msg->vbsctlinf.uPTS;
#if 0
            	vdec_msg->vbsctlinf.uPTS        = tranPts(info->pts, 0);
            	if(vdec_msg->vbsctlinf.uPTS == 0)
            	{
            	    vdec_msg->vbsctlinf.CtrlBits &= ~PTS_VALID_BIT;
            	}
#else
            	//vdec_msg->vbsctlinf.uPTS        = info->pts;//tranPts(info->pts, 0);
            	vdec_msg->vbsctlinf.uPTS        = translatePts(dvbctrl, info->pts, MEDIA_TYPE_VIDEO, &nPtsReturnHeadFlag);
            	//WARNING("video pts = %d.\n", (__u32)info->pts);
            	if(vdec_msg->vbsctlinf.uPTS == 0)
            	{
            	    WARNING("fatal error! check code! set video pts invalid.\n");
                    //照现在的做法,这是不可能出现的.
            	    vdec_msg->vbsctlinf.CtrlBits &= ~PTS_VALID_BIT;
            	}
            	if(nPtsReturnHeadFlag)
            	{
                    WARNING("notify vply new clock change! new pts[%qd]\n", vdec_msg->vbsctlinf.uPTS);
                    if(dvbctrl->vply)
                    {
                        esMODS_MIoctrl(dvbctrl->vply, CEDAR_VPLY_CMD_SET_NEW_VIDEO_PTS, (__u32)vdec_msg->vbsctlinf.uPTS, NULL);
                    }
                    else
                    {
                        WARNING("fatal error! vply = NULL! can't tell vply to set new video pts to video clock!\n");
                    }
                    
            	}
//        	    if(dvbc->firstVdPts == 0)
//        	    {
//        	        if(dvbc->adPtsBase == 0)    //* system clock not start yet, need to start it.
//        	        {
//        	            esMODS_MIoctrl(dvbc->avsync, DRV_AVS_CMD_SET_VID_TIME, DRV_AVS_TIME_TOTAL, (void*)((__u32)info->pts - TIME_BEFORE_PLAY));
//		                esMODS_MIoctrl(dvbc->avsync, DRV_AVS_CMD_START, AVS_VIDEO, 0);
//		            }
//		            dvbc->firstVdPts = info->pts;
//		            dvbc->vdPtsBase  = info->pts;
//        	    }
#endif
//            	WARNING("trans pts (%x-%x) to (%x-%x), systime = %x.\n", (__u32)(tmp>>32), (__u32)(tmp & 0xffffffff), 
//            	        (__u32)(vdec_msg->vbsctlinf.uPTS>>32), (__u32)(vdec_msg->vbsctlinf.uPTS&0xffffffff),
//            	
//    			esMODS_MIoctrl(dvbc->avsync,
//						   		DRV_AVS_CMD_GET_VID_TIME, 
//						   		DRV_AVS_TIME_TOTAL, 
//						   		NULL));
		    }
		    else
		    {
		    	vdec_msg->vbsctlinf.uPTS		= -1;
		    	WARNING("video pts invalid.\n");
		    }
	    }
		
        vdec_msg->vbsctlinf.uFrmDecMode = MEDIA_STATUS_PLAY;

//		if(dvbctrl->vdFmt.codec_type == DVB_VBS_TYPE_H264)
//			vdec_msg->vbsctlinf.uPTS *= 1000;
        vdec_msg->vbsctlinf.uPTS *= 1000;   //vdec, adec都要求是微秒为单位. 而sdec要求是毫秒为单位.

        esKRNL_SktPost(dvbctrl->pVBSSktout, KRNL_SKT_USR_IN, skt);
        skt = NULL;

        dvbctrl->vbs_frm = NULL;
#endif

		
#if DEBUG_SAVE_VIDEO_STREAM
{
		__u8* data;
#if PLAY_VIDEO_STREAM
		data = skt->data;
#else
		data = dvbc->tmpBuf;
#endif

		if(dvbctrl->fpVideoStream)
			fwrite(data, 1, info->dataLen, dvbctrl->fpVideoStream);
}
#endif

        return EPDK_OK;
    }
    else if (md_type == MEDIA_TYPE_AUDIO)
    {    
#if PLAY_AUDIO_STREAM
    	__psr_2_adec_msg_t  *adec_msg;

//        WARNING("post audio data.\n");
        skt = dvbctrl->abs_frm;
        if(skt == NULL)
        {
            WARNING("audio socket is null when update data.\n");
            return EPDK_FAIL;
        }
		//第一次拿到音频数据,那么必须要有pts. 如果有pts,并且没有video, 就顺便确定adStreamStartPts, StreamStartPts, AccumulateStartPts, adStreamPts.
        //adPts专门在一个函数translatePts()里确定.
		if(dvbctrl->nGetAdFlag == 0)
		{
			if((info->ctrlBits & PTS_VALID_BIT) == 0)
			{
				WARNING("adPtsBase not get yet, discard audio data.\n");
				return EPDK_OK;
			}
            WARNING("first adStream pts[%qd]\n", info->pts);
			dvbctrl->adStreamStartPts = dvbctrl->adStreamPts = info->pts;
            dvbctrl->nGetAdFlag = 1;
            //算出音视频的时间差
            if(dvbctrl->nGetVdFlag)
            {
                dvbctrl->nAVPtsInterval = dvbctrl->vdStreamStartPts - dvbctrl->adStreamStartPts;
                WARNING("AV interval[%qd]! AudLastGet\n", dvbctrl->nAVPtsInterval);
                if(dvbctrl->vply)
                {
                    esMODS_MIoctrl(dvbctrl->vply, CEDAR_VPLY_CMD_SET_AV_INTERVAL, (__s32)dvbctrl->nAVPtsInterval, NULL);
                }
                else
                {
                    WARNING("fatal error! vply = NULL!\n");
                }
            }
#if	DEBUG_TIME
			dvbctrl->newTime = esKRNL_TimeGet();
			WARNING("********************* receive first audio data at time %d ms.\n", dvbc->newTime*10);
#endif
        
		}
		
        skt->datalen  = info->dataLen;
        skt->msgtype  = PSR2ADEC_MSG_TYPE_NONE;
        skt->datatype = CEDAR_DATATYPE_MID;
        
        adec_msg = (__psr_2_adec_msg_t *)skt->msg;
        adec_msg->absctlinf.CtrlBits    = info->ctrlBits;
		if(info->ctrlBits & PTS_VALID_BIT)
		{
          //  WARNING("ad chunk pts[%qd]\n", info->pts);
#if 0
        	adec_msg->absfrminf.uPts        = tranPts(info->pts, 1);
            if(adec_msg->absfrminf.uPts == 0)
            {
                adec_msg->absfrminf.CtrlBits &= ~PTS_VALID_BIT;
            }
#else
        	//adec_msg->absctlinf.uPts        = info->pts;//*tranPts(info->pts, 1);
        	adec_msg->absctlinf.uPts        = translatePts(dvbctrl, info->pts, MEDIA_TYPE_AUDIO, &nPtsReturnHeadFlag);
            if(adec_msg->absctlinf.uPts == 0)
            {
            	WARNING("fatal error! audio local pts can't == 0, set audio pts invalid.\n");
                adec_msg->absctlinf.CtrlBits &= ~PTS_VALID_BIT;
            }
            if(nPtsReturnHeadFlag)
        	{
                WARNING("notify aply new clock change! new pts[%qd]\n", adec_msg->absctlinf.uPts);
                if(dvbctrl->aply)
                {
                    esMODS_MIoctrl(dvbctrl->aply, CEDAR_APLY_CMD_SET_NEW_AUDIO_PTS, 0, (void*)&adec_msg->absctlinf.uPts);
                }
                else
                {
                    WARNING("fatal error! aply = NULL! can't tell aply to set new audio and video pts to av_clock!\n");
                }
        	}
//        	if(dvbc->firstAdPts == 0)
//        	{
//        	    if(dvbc->vdPtsBase == 0)    //* system clock not start yet, need to start it.
//        	    {
//        	        esMODS_MIoctrl(dvbc->avsync, DRV_AVS_CMD_SET_VID_TIME, DRV_AVS_TIME_TOTAL, (void*)((__u32)info->pts - TIME_BEFORE_PLAY));
//		            esMODS_MIoctrl(dvbc->avsync, DRV_AVS_CMD_START, AVS_VIDEO, 0);
//		        }
//		        dvbc->firstAdPts = info->pts;
//		        dvbc->adPtsBase  = info->pts;
//        	}
#endif
//			WARNING("audio pts = %d.\n", (__u32)adec_msg->absfrminf.uPts);
		}
		else
		{
			//WARNING("audio pts not valid.\n");
			adec_msg->absctlinf.uPts	= 0;
		}
        adec_msg->absctlinf.uPts *= 1000;
        esKRNL_SktPost(dvbctrl->pABSSktout, KRNL_SKT_USR_IN, skt);
        skt = NULL;
        dvbctrl->abs_frm = NULL;
#endif

#if DEBUG_SAVE_AUDIO_STREAM
{
		__u8* data;
#if PLAY_AUDIO_STREAM
		data = skt->data;
#else
		data = dvbc->tmpBuf;
#endif

		if(dvbctrl->fpAudioStream)
			fwrite(data, 1, info->dataLen, dvbctrl->fpAudioStream);
}
#endif

        return EPDK_OK;
    }
#if 0
		else if (md_type == MEDIA_TYPE_SUBTITLE)
    {    
#if PLAY_SUBTITLE_STREAM
    	__psr_2_ldec_msg_t  *sdec_msg;

        skt = dvbctrl->sbs_frm;
        if(skt == NULL)
        {
            WARNING("subtitle socket is null when update data.\n");
            return EPDK_FAIL;
        }
		if(dvbctrl->nGetSubFlag == 0)
		{
			WARNING("info->dataLen  =%x\n",info->dataLen);
			if((info->ctrlBits & PTS_VALID_BIT) == 0)//&&(info->dataLen ==0))
			{
				WARNING("subPtsBase not get yet, discard subtitle data.\n");
				return EPDK_OK;
			}
            WARNING("first subStream pts[%qd]\n", info->pts);
			dvbctrl->subStreamStartPts = dvbctrl->subStreamPts = info->pts;
            dvbctrl->nGetSubFlag = 1;
           
#if	DEBUG_TIME
			dvbctrl->newTime = esKRNL_TimeGet();
			WARNING("********************* receive first subtitle data at time %d ms.\n", dvbc->newTime*10);
#endif
        
		}
		
        skt->datalen  = info->dataLen;
        skt->msgtype  = PSR2LDEC_MSG_TYPE_NONE;
        skt->msgtype = PSR2LDEC_MSG_TYPE_LBSCHUNK_INF;
        skt->datatype = CEDAR_DATATYPE_MID;
        
        sdec_msg = (__psr_2_ldec_msg_t *)skt->msg;
        sdec_msg->lbs_dat_info.CtrlBits    = info->ctrlBits;
        memcpy((__u8 *)sdec_msg->lbs_dat_info.privateData, info->privateData, PES_PRIVATE_DATA_LEN);
		if(info->ctrlBits & PTS_VALID_BIT)
		{
            //WARNING("ad chunk pts[%qd]\n", info->pts);
#if 0
        	adec_msg->absfrminf.uPts        = tranPts(info->pts, 1);
            if(adec_msg->absfrminf.uPts == 0)
            {
                adec_msg->absfrminf.CtrlBits &= ~PTS_VALID_BIT;
            }
#else
        	sdec_msg->lbs_dat_info.uPTS= translatePts(dvbctrl, info->pts, MEDIA_TYPE_SUBTITLE, &nPtsReturnHeadFlag);
            if(sdec_msg->lbs_dat_info.uPTS == 0)
            {
            	WARNING("fatal error! subtitle local pts can't == 0, set subtitle pts invalid.\n");
                sdec_msg->lbs_dat_info.CtrlBits &= ~PTS_VALID_BIT;
            }
            if(nPtsReturnHeadFlag)
        	{
                WARNING("notify aply new clock change! new pts[%qd]\n", sdec_msg->lbs_dat_info.uPTS);
                if(dvbctrl->sply)
                {
                    esMODS_MIoctrl(dvbctrl->sply, CEDAR_SPLY_CMD_SET_NEW_VIDEO_PTS, 0, (void*)&sdec_msg->lbs_dat_info.uPTS);
                }
                else
                {
                    WARNING("fatal error! sply = NULL! can't tell sply to set new audio and video pts to av_clock!\n");
                }
        	}

#endif
		}
		else
		{
			//WARNING("subtitle pts not valid.\n");
			sdec_msg->lbs_dat_info.uPTS	= 0;
		}
        //sdec_msg->lbs_dat_info.uPTS *= 1000;   //sdec ms;
        skt->msgtype = PSR2LDEC_MSG_TYPE_LBSCHUNK_INF;
        //sdec_msg->lbs_dat_info.CtrlBits |= LDEC_DURATION_VALID_CTRL;
        //sdec_msg->lbs_dat_info.uDuration = 3000;
        
        esKRNL_SktPost(dvbctrl->pLBSSktout, KRNL_SKT_USR_IN, skt);
        skt = NULL;
        dvbctrl->sbs_frm = NULL;
#endif

#if DEBUG_SAVE_SUBTITLE_STREAM
{
		__u8* data;
#if PLAY_SUBTITLE_STREAM
		data = skt->data;
#else
		data = dvbc->tmpSubtitleBuf;
#endif

		if(dvbctrl->fpSubtitleStream)
			fwrite(data, 1, info->dataLen, dvbctrl->fpSubtitleStream);
}
#endif

        return EPDK_OK;
    }
#endif
    else if (md_type == MEDIA_TYPE_SUBTITLE)
    {    
#if PLAY_SUBTITLE_STREAM
    	__psr_2_ldec_msg_t  *sdec_msg;
			//__u32 tmpvidtime;
        skt = dvbctrl->sbs_frm;
        if(skt == NULL)
        {
            WARNING("subtitle socket is null when update data.\n");
            return EPDK_FAIL;
        }

		if(dvbctrl->nGetSubFlag == 0)
		{
			WARNING("info->dataLen  =%x\n",info->dataLen);
			if((info->ctrlBits & PTS_VALID_BIT) == 0)
			{
				WARNING("subPtsBase not get yet, discard subtitle data.\n");
				return EPDK_OK;
			}
            WARNING("first subStream pts[%qd]\n", info->pts);
			dvbctrl->subStreamStartPts = dvbctrl->subStreamPts = info->pts;
            dvbctrl->nGetSubFlag = 1;
           
#if	DEBUG_TIME
			dvbctrl->newTime = esKRNL_TimeGet();
			WARNING("********************* receive first subtitle data at time %d ms.\n", dvbc->newTime*10);
#endif
        
		}
		
        skt->datalen  = info->dataLen;
        skt->msgtype  = PSR2LDEC_MSG_TYPE_NONE;
        skt->msgtype = PSR2LDEC_MSG_TYPE_LBSCHUNK_INF;
        skt->datatype = CEDAR_DATATYPE_MID;
        
        sdec_msg = (__psr_2_ldec_msg_t *)skt->msg;
        sdec_msg->lbs_dat_info.CtrlBits    = info->ctrlBits;
        memcpy((__u8 *)sdec_msg->lbs_dat_info.privateData, info->privateData, PES_PRIVATE_DATA_LEN);
		if(info->ctrlBits & PTS_VALID_BIT)
		{
            //WARNING("ad chunk pts[%qd]\n", info->pts);
#if 0
        	adec_msg->absfrminf.uPts        = tranPts(info->pts, 1);
            if(adec_msg->absfrminf.uPts == 0)
            {
                adec_msg->absfrminf.CtrlBits &= ~PTS_VALID_BIT;
            }
#else
        	sdec_msg->lbs_dat_info.uPTS= translatePts(dvbctrl, info->pts, MEDIA_TYPE_SUBTITLE, &nPtsReturnHeadFlag);
            if(sdec_msg->lbs_dat_info.uPTS == 0)
            {
            	WARNING("fatal error! subtitle local pts can't == 0, set subtitle pts invalid.\n");
              //sdec_msg->lbs_dat_info.CtrlBits &= ~PTS_VALID_BIT;
            }
            if(nPtsReturnHeadFlag)
        	{
                WARNING("notify aply new clock change! new pts[%qd]\n", sdec_msg->lbs_dat_info.uPTS);
                if(dvbctrl->sply)
                {
                    esMODS_MIoctrl(dvbctrl->sply, CEDAR_SPLY_CMD_SET_NEW_VIDEO_PTS, 0, (void*)&sdec_msg->lbs_dat_info.uPTS);
                }
                else
                {
                    WARNING("fatal error! sply = NULL! can't tell sply to set new audio and video pts to av_clock!\n");
                }
        	}

#endif
		}
		else
		{
			//WARNING("subtitle pts not valid.\n");
			sdec_msg->lbs_dat_info.uPTS	= 0;
		}
        //sdec_msg->lbs_dat_info.uPTS *= 1000;   //sdec ms;
        skt->msgtype = PSR2LDEC_MSG_TYPE_LBSCHUNK_INF;
        //sdec_msg->lbs_dat_info.CtrlBits |= LDEC_DURATION_VALID_CTRL;
        //sdec_msg->lbs_dat_info.uDuration = 3000;
        
        esKRNL_SktPost(dvbctrl->pLBSSktout, KRNL_SKT_USR_IN, skt);
        skt = NULL;
        dvbctrl->sbs_frm = NULL;
#endif

#if DEBUG_SAVE_SUBTITLE_STREAM
{
		__u8* data;
#if PLAY_SUBTITLE_STREAM
		data = skt->data;
#else
		data = dvbc->tmpSubtitleBuf;
#endif

		if(dvbctrl->fpSubtitleStream)
			fwrite(data, 1, info->dataLen, dvbctrl->fpSubtitleStream);
}
#endif

        return EPDK_OK;
    }
}


#if (PLAY_VIDEO_STREAM == 1)
/*******************************************************************************
Function name: reset_video
Description: 
    1.没有验证过,只是这么写而已.实际上应该是不行的
Parameters: 
    
Return: 
    
Time: 2011/12/21
*******************************************************************************/
static __s32 reset_video(__dvbc_t *dvbc)
{
    __hdle          skt;
    __u8						nVdrvMafEnable;
    WARNING("xxxxxxxxxxxxxxxxxxxxxxxxx reseting video. really reset???? check!!!!\n");
    dvbc->status |= DVBC_TMP_STATUS_RESET_FALG;
    //esMODS_MIoctrl(dvbc->avsync, DRV_AVS_CMD_SET_VD_RESETING_FLAG, 1, NULL);
    
    if(dvbc->vply)
    {
        MESSAGE("stop video playback...\n");
        esMODS_MIoctrl(dvbc->vply, CEDAR_VPLY_CMD_STOP, 0, 0);
    }
    
    if(dvbc->vdec)
    {
        MESSAGE("stop video decoder...\n");
        esMODS_MIoctrl(dvbc->vdec, CEDAR_VDEC_CMD_STOP, 0, 0);
    }
        
    if(dvbc->vply)
    {
        MESSAGE("close video playback...\n");
        //vply_MClose(dvbc->vply);
        esMODS_MClose(dvbc->vply);
        dvbc->vply = NULL;
    }
    
    if(dvbc->vdec)
    {
        MESSAGE("close video decoder...\n");
        esMODS_MClose(dvbc->vdec);
        dvbc->vdec = NULL;
    }
    
    if(dvbc->vply_mid)
    {
        MESSAGE("uninstall video playback...\n");
        //vply_MExit();
        esMODS_MUninstall(dvbc->vply_mid);
        dvbc->vply_mid = 0;
    }
    
    if(dvbc->vdec_mid)
    {
        MESSAGE("uninstall video decoder...\n");
        esMODS_MUninstall(dvbc->vdec_mid);
        dvbc->vdec_mid = 0;
    }
    
    pass_vd_init_info(dvbc);
    //* install video decoder
    if (dvbc->vdec_mid == 0)
    {
//        if(dvbc->vdec_mid == DVB_VBS_TYPE_H264)
//            dvbc->vdec_mid = esMODS_MInstall("d:\\mod\\dvb\\vdec_264.plg", 0);
//        else
//            dvbc->vdec_mid = esMODS_MInstall("d:\\mod\\dvb\\vdec_mp2.plg", 0);
		dvbc->vdec_mid = esMODS_MInstall(CEDAR_PLUGINS_DIRECTORY"vdec_com.plg", 0);
        dvbc->vdec = NULL;
    }
    dvbc->vdec = esMODS_MOpen(dvbc->vdec_mid, 0);
    if(EPDK_OK!=esMODS_MIoctrl(dvbc->vdec, CEDAR_VDEC_CMD_UPDATE_VDRV, dvbc->vdFmt.codec_type, NULL))
    {
        WARNING("Cedar:UPDATE drv video decoder fail!\n");
//        oldmp = dvbctrl->adec;
//        dvbctrl->adec = (__mp*)0;
//        esMODS_MClose(oldmp);
//        goto _err_start_plg_fail;
    }
    if(EPDK_OK != esMODS_MIoctrl(dvbc->vdec, CEDAR_VDEC_CMD_OPEN, 0, NULL))
    {
        WARNING("cedar:vdec_com open fail\n");
//        oldmp = dvbctrl->vdec;
//        dvbctrl->vdec = (__mp *)0;
//        esMODS_MClose(oldmp);
//        goto _err_start_plg_fail;
    }
    esMODS_MIoctrl(dvbc->vdec, CEDAR_VDEC_CMD_SET_VBSSKTIN, 0, (void *)dvbc->pVBSSktout);
    esMODS_MIoctrl(dvbc->vdec, CEDAR_VDEC_CMD_SET_AVSHDL, 0, dvbc->avsync);
    esMODS_MIoctrl(dvbc->vdec, CEDAR_VDEC_CMD_SET_ROTATE, 0, NULL);
    esMODS_MIoctrl(dvbc->vdec, CEDAR_VDEC_CMD_ENABLE_VIDEO_AUTO_SCALE, 0, NULL);
    esMODS_MIoctrl(dvbc->vdec, CEDAR_VDEC_CMD_SET_RESERVED_MEM_SIZE, dvbc->nReservedMemSize, NULL);
		if(dvbc->nMafEnable > 1)
    {
        nVdrvMafEnable = 1;
    }
    else
    {
        nVdrvMafEnable = 0;
    }
    esMODS_MIoctrl(dvbc->vdec, CEDAR_VDEC_CMD_ENABLE_MAF, nVdrvMafEnable, NULL);
		
		esMODS_MIoctrl(dvbc->vdec, CEDAR_VDEC_CMD_SET_TV_MODE, dvbc->tvMode, NULL);
		esMODS_MIoctrl(dvbc->vdec, CEDAR_VDEC_CMD_SET_ROTATE, dvbc->rotate_mode, NULL);

    esMODS_MIoctrl(dvbc->vdec, CEDAR_VDEC_CMD_START, 0, 0);
    
    //dvbc->vply_mid = vply_MInit();
    dvbc->vply_mid = esMODS_MInstall(CEDAR_PLUGINS_DIRECTORY"vply.plg", 0);
    //dvbc->vply = vply_MOpen(dvbc->vply_mid, 0);
    dvbc->vply = esMODS_MOpen(dvbc->vply_mid, 0);
    if(EPDK_OK != esMODS_MIoctrl(dvbc->vply, CEDAR_VPLY_CMD_OPEN, 0, NULL))
    {
        WARNING("vply IO_Open fail\n");
        //goto _err_start_plg_fail;
        //return EPDK_FAIL;
    }

    //* connect video decoder to vply with pic skt
    skt = (__hdle)esMODS_MIoctrl(dvbc->vdec, CEDAR_VDEC_CMD_GET_PICSKTOUT, 0, 0);
    esMODS_MIoctrl(dvbc->vply, CEDAR_VPLY_CMD_SET_PICSKTIN, 0, (void *)skt);
    esMODS_MIoctrl(dvbc->vply, CEDAR_VPLY_CMD_SET_AVSHDL, 0, dvbc->avsync);
		esMODS_MIoctrl(dvbc->vply, CEDAR_VPLY_CMD_SET_ROTATE, dvbc->rotate_mode, NULL);

    if(esMODS_MIoctrl(dvbc->vply, CEDAR_VPLY_CMD_START, 0, 0) != EPDK_OK)
    {
        WARNING("play_media: start video playback failed!\n");
        //goto _err_start_plg_fail;
    }
    
    //esMODS_MIoctrl(dvbc->avsync, DRV_AVS_CMD_SET_VD_RESETING_FLAG, 0, NULL);
    
    dvbc->status &= ~DVBC_TMP_STATUS_RESET_FALG;
    WARNING("xxxxxxxxxxxxxxxxxxxxxxxxx reseting video end.\n");
    
    return 0;
}
#endif


