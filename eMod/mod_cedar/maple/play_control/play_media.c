/*
*********************************************************************************************************
*                                                    eMOD
*                                   the Easy Portable/Player Operation System
*                                           mod_cedarcore sub-system
*
*                                (c) Copyright 2009-20010, Clif.Chen China
*                                               All Rights Reserved
*
* File   : play_media.c
* Version: V1.0
* By     : clif.c
* update : 
*********************************************************************************************************
*/
#include "play_ctrl_s.h"
#include "play_ctrl.h"
#include "psr_2_adec.h"
#include "psr_2_vdec.h"

__s32 StopVply(__dvbc_t *dvbctrl)
{
    if(dvbctrl->vply)
    {
        MESSAGE("stop video playback...\n");
        esMODS_MIoctrl(dvbctrl->vply, CEDAR_VPLY_CMD_STOP, 0, 0);
    }
    return EPDK_OK;
}
__s32 CloseVply(__dvbc_t *dvbctrl)
{
    __mp        *tmpHdle;
    StopVply(dvbctrl);
    if(dvbctrl->vply)
    {
        MESSAGE("close video playback...\n");
        tmpHdle = dvbctrl->vply;
        dvbctrl->vply = (__mp*)0;
        //vply_MClose(tmpHdle);
        esMODS_MClose(tmpHdle);
    }
    return EPDK_OK;
}
__s32 UninstallVply(__dvbc_t *dvbctrl)
{
    CloseVply(dvbctrl);
    if(dvbctrl->vply_mid)
    {
        MESSAGE("uninstall video playback...\n");
        //vply_MExit();
        esMODS_MUninstall(dvbctrl->vply_mid);
        dvbctrl->vply_mid = 0;
    }
    return EPDK_OK;
}

/*******************************************************************************
Function name: install_adec
Description: 
    
Parameters: 
    nAbsType = CEDAR_ABS_TYPE_AC3,
    praw_adec_flag :表示最后加载的插件是否是rawdata_adec类型的
Return: 
    EPDK_FAIL;表示要goto _err_start_plg_fail;
    1:表示要goto _ad_conti_cedar_maintsk;
    EPDK_OK:正常
Time: 2011/3/17
*******************************************************************************/
__s32 install_adec(__dvbc_t *pCedar, __s32 nAbsType, __s32 *praw_adec_flag)
{
    __s32   ret = EPDK_FAIL;
    if(NULL == praw_adec_flag)
    {
        WARNING("fatal error, praw_adec_flag = NULL\n");
        return EPDK_FAIL;
    }
    *praw_adec_flag = 0;
    if(pCedar->aud_raw_data_mode)
    {
        switch(nAbsType)
        {
            case CEDAR_ABS_TYPE_AC3:
            case CEDAR_ABS_TYPE_DTS:
            {
                MESSAGE("Install audio decoder(audio[%x] raw data mode)!\n", nAbsType);
                pCedar->adec_mid = esMODS_MInstall(CEDAR_PLUGINS_DIRECTORY"adec_com.plg", 0);
                *praw_adec_flag = 1;
                ret = EPDK_OK;
                break;
            }
            default:
            {
                WARNING("don't support raw data mode of this type[%d], prepare to goto normal play mode\n", nAbsType);
                //goto _err_start_plg_fail;
                //goto _ad_conti_cedar_maintsk;
                *praw_adec_flag = 0;
                //goto __aud_decode_mode;
                break;
            }
        }
    }
    if(0 == *praw_adec_flag)
    {
//__aud_decode_mode:
        switch(nAbsType)
        {
            case CEDAR_ABS_TYPE_NONE:
                MESSAGE("Cedar:audio bitstream none!\n");
                //goto _ad_conti_cedar_maintsk;
                ret = 1;
                break;

            case CEDAR_ABS_TYPE_UNKNOWN:
                MESSAGE("Cedar:audio bitstream not support!\n");
                //goto _ad_conti_cedar_maintsk;
                ret = 1;
                break;

            case CEDAR_ABS_TYPE_ADPCM:
            case CEDAR_ABS_TYPE_LPCM_V:
            case CEDAR_ABS_TYPE_LPCM_A:
            case CEDAR_ABS_TYPE_PCM:
            case CEDAR_ABS_TYPE_MPEG12_LAYER1:
            case CEDAR_ABS_TYPE_MPEG12_LAYER2:
            case CEDAR_ABS_TYPE_MPEG12_LAYER3:
            case CEDAR_ABS_TYPE_WMA_STANDARD:
            case CEDAR_ABS_TYPE_APE:
            case CEDAR_ABS_TYPE_FLAC:
            case CEDAR_ABS_TYPE_OGG:
            case CEDAR_ABS_TYPE_DTS:
            case CEDAR_ABS_TYPE_AC3:
            case CEDAR_ABS_TYPE_MPEG_AAC_LC:
            case CEDAR_ABS_TYPE_COOK:
            case CEDAR_ABS_TYPE_RAAC:
            case CEDAR_ABS_TYPE_SIPR:
            case CEDAR_ABS_TYPE_ATRC:
            case CEDAR_ABS_TYPE_AMR:
            case CEDAR_ABS_TYPE_RA:
            case CEDAR_ABS_TYPE_MID:
               // MESSAGE("Install audio decoder(%x)!\n", tmp);
                pCedar->adec_mid = esMODS_MInstall(CEDAR_PLUGINS_DIRECTORY"adec_com.plg", 0);
                ret = EPDK_OK;
                break;
            default:
                WARNING("audio bitstream not support\n");
                //goto _err_start_plg_fail;
                ret = EPDK_FAIL;
                break;
        }
    }
    return ret;
}

/*
*********************************************************************************************************
*                                        play_media
*
* Description: This function will start play a media, install psr, dec, render and plugins.
*
* Arguments  : dvbctrl  dvb control module handler
*
* Returns    : if succeed return EPDK_OK
*               else return EPDK_FAIL
*********************************************************************************************************
*/


__s32 play_media(__dvbc_t *dvbctrl)
{
    __hdle          skt;
    __s32           result;
    __s32           tmpret;
    __u32           tmp;
    __mp*           oldmp;
    __s32           raw_adec_flag = 0;  //表示adec插件是否是raw类型的,默认值为0，不是
    __s32           is_aud_raw_type = 0;        //0:表示当前的音频解码插件不是raw data类型的; 1:是raw data类型的。
    __pCBK_t        call_back = NULL;
    __s32           vply_work_mode = VPLY_WORK_MODE_TASK_ACTIVE;  //vply模块的工作方式. enum __VPLY_WORK_MODE, VPLY_WORK_MODE_ISR:老方式，中断ISR要帧; VPLY_WORK_MODE_TASK_ACTIVE:新方式，vplytask()要帧设帧
    __s32           nVdrvMafEnable;

    MESSAGE("********************************** play_media *************************************\n");

	//*********************************************************************************************//
	//**************************************** AVSync Driver ****************************************//
	//*********************************************************************************************//
	
    //* install av sync driver.
    if(dvbctrl->avsync_mid == 0)
    {
        dvbctrl->avsync_mid = esMODS_MInstall(CEDAR_PLUGINS_DIRECTORY"avs.drv", 0);
        if(dvbctrl->avsync_mid == 0)
        {
            WARNING("play_media: install av sync driver fail!\n");
            goto _err_start_plg_fail;
        }

        dvbctrl->avsync = NULL;
    }

    if(!dvbctrl->avsync)
    {
        dvbctrl->avsync = esMODS_MOpen(dvbctrl->avsync_mid, 0);
        if(dvbctrl->avsync == NULL)
        {
            WARNING("play_media: open av sync driver fail!\n");
            goto _err_start_plg_fail;
        }
		//MESSAGE("open avsync driver success.\n");
    }
    tmpret = esMODS_MIoctrl(dvbctrl->avsync, DRV_AVS_CMD_OPEN, 0, NULL);
    if(EPDK_OK != tmpret)
    {
        WARNING("Cedar:open av sync driver fail!\n");
        goto _err_start_plg_fail;
    }
    if(esMODS_MIoctrl(dvbctrl->avsync, DRV_AVS_CMD_MODULE_START, 0, NULL) != EPDK_OK)
    {
        WARNING("Cedar:playfile():avsync: media open fail!\n");
        goto _err_start_plg_fail;
    }
    MESSAGE("Open avsync successed!\n");
    //直接启动同步.
    esMODS_MIoctrl(dvbc->avsync, DRV_AVS_CMD_ENABLE_AVSYNC, AVS_ENABLE, 0);
    //因为准备把video clock作为模拟流时钟的clock,所以不管有没有video,都要打开video clock.
    esMODS_MIoctrl(dvbc->avsync, DRV_AVS_CMD_REGIST_CLOCK, AVS_VIDEO, 0);

	//*********************************************************************************************//
	//***************************************** Audio Plugin* ****************************************//
	//*********************************************************************************************//
    //* install audio plugin
    tmp = dvbctrl->adFmt.codec_type;
    if(dvbctrl->adec_mid)                 /* 判断是否有 audio decoder 插件        */
    {
        if(!dvbctrl->adec)
        {
            dvbctrl->adec = esMODS_MOpen(dvbctrl->adec_mid, 0);
        }
        if(dvbctrl->adec == (__mp *)0)
        {
            WARNING("Cedar:open audio decoder fail!\n");
            goto _err_start_plg_fail;
        }

        /* 判断是否支持当前文件格式, 判断是否匹配raw_data的设置。因为raw_data单独做了模块。*/
        if(!esMODS_MIoctrl(dvbctrl->adec, CEDAR_ADEC_CMD_CHKSPT, tmp, (void*)dvbctrl->aud_raw_data_mode))    //dtv没有rawdata模式,至少现在没有.
        {
            oldmp = dvbctrl->adec;
            dvbctrl->adec = 0;
            esMODS_MClose(oldmp);
            esMODS_MUninstall(dvbctrl->adec_mid);
            dvbctrl->adec_mid = 0;
        }
        else    //支持，就顺便再更新加载解码库
        {
            //esMODS_MIoctrl(pCedar->adec, CEDAR_ADEC_CMD_SET_ABSTYPE, tmp, 0);
            if(esMODS_MIoctrl(dvbctrl->adec, CEDAR_ADEC_CMD_UPDATE_ADRV, tmp, (void*)dvbctrl->aud_raw_data_mode) != EPDK_OK)
            {
                WARNING("Cedar:UPDATE drv audio decoder fail!\n");
                goto _err_start_plg_fail;
            }
        }
    }

    if(dvbctrl->adec_mid == 0)            /* 如果没有 audio decoder 插件或被卸载了*/
    {
        MESSAGE("Try to install audio decoder(atype:%d)!\n", tmp);
        //首先处理raw data mode
        tmpret = install_adec(dvbctrl, tmp, &raw_adec_flag);
        if(EPDK_FAIL == tmpret)
        {
            goto _err_start_plg_fail;
        }
        else if(1 == tmpret)
        {
            //goto _ad_conti_cedar_maintsk;
            goto _ad_conti_dvbc_maintsk;
        }
        else
        { //一般是EPDK_OK
        }
        if(dvbctrl->adec_mid == 0)
        {
            WARNING("Cedar:install audio decoder fail!\n");
            goto _err_start_plg_fail;
        }

        MESSAGE("Install audio decoder successed, mod_id:0x%x!\n", dvbctrl->adec_mid);
    }

    if(!dvbctrl->adec)
    {
        dvbctrl->adec = esMODS_MOpen(dvbctrl->adec_mid, 0);
        if(dvbctrl->adec == (__mp *)0)
        {
            WARNING("Cedar:open audio decoder fail!\n");
            goto _err_start_plg_fail;
        }
        
        if(EPDK_OK!=esMODS_MIoctrl(dvbctrl->adec, CEDAR_ADEC_CMD_UPDATE_ADRV, tmp, (void*)dvbctrl->aud_raw_data_mode))
        {
            WARNING("Cedar:UPDATE drv audio decoder fail!\n");
            oldmp = dvbctrl->adec;
            dvbctrl->adec = (__mp*)0;
            esMODS_MClose(oldmp);
            goto _err_start_plg_fail;
        }
    }
    if(EPDK_OK!=esMODS_MIoctrl(dvbctrl->adec, CEDAR_ADEC_CMD_OPEN, 0, NULL))
    {
        WARNING("cedar adec open fail\n");
        oldmp = dvbctrl->adec;
        dvbctrl->adec = (__mp*)0;
        esMODS_MClose(oldmp);
        goto _err_start_plg_fail;
    }
    //set cedarini to aply
    //esMODS_MIoctrl(dvbctrl->adec, CEDAR_ADEC_CMD_SET_CEDARINI, 0, &dvbctrl->CustomCedarINI);
    esMODS_MIoctrl(dvbctrl->adec, CEDAR_ADEC_CMD_SET_CEDARINI, 0, &dvbctrl->pMapleINI->CedarINIInstance);
    //检测音频解码插件是否是raw data type.
    if(EPDK_TRUE == esMODS_MIoctrl(dvbctrl->adec, CEDAR_ADEC_CMD_IS_RAWDATA, 0, NULL))
    {
        is_aud_raw_type = 1;
    }
    else
    {
        is_aud_raw_type = 0;
    }
    
    //* connect parser to adec with abs skt
    esMODS_MIoctrl(dvbctrl->adec, CEDAR_ADEC_CMD_SET_ABSSKTIN, 0, (void *)dvbctrl->pABSSktout);
    //* set avsync driver module handle to audio decoder
    esMODS_MIoctrl(dvbctrl->adec, CEDAR_ADEC_CMD_SET_AVSHDL, 0, dvbctrl->avsync);
    //* clear run with video flag, it will be set to true when install video decoder
    esMODS_MIoctrl(dvbctrl->adec, CEDAR_ADEC_CMD_SET_WITH_VIDEO, 1, 0); //对于dtv,即使只有音频,也当做视频psr处理,因为实际上是ts流,不允许adrv自己读文件.
    //set ff/rr speed to audio decoder
    esMODS_MIoctrl(dvbctrl->adec, CEDAR_ADEC_CMD_SET_FRSPEED, dvbctrl->setting_inf.frspeed, 0);

    //* start adec plugin
    MESSAGE("start audio decoder.\n");
    if(esMODS_MIoctrl(dvbctrl->adec, CEDAR_ADEC_CMD_START, 0, 0) != EPDK_OK)
    {
        WARNING("start audio fail.\n");
        goto _err_start_plg_fail;
    }

	//* set adec to avsync,如果需要用absbuffer的缓冲数据调节播放速度,就需要.暂时想通过pcrpts来搞定.
	//esMODS_MIoctrl(dvbctrl->avsync, DRV_AVS_CMD_SET_ADEC_HDLR, 0, (void*)dvbctrl->adec);

    esMODS_MIoctrl(dvbctrl->avsync, DRV_AVS_CMD_REGIST_CLOCK, AVS_AUDIO, 0);    //有audio数据,就要注册audio时间轴,表示存在

_ad_conti_dvbc_maintsk:
	//*********************************************************************************************//
	//***************************************** Video Plugin* ****************************************//
	//*********************************************************************************************//
    //* install video decoder
    //tmp = esMODS_MIoctrl(pCedar->psr, CEDAR_PSR_CMD_GET_VBSTYPE, 0, 0);
    //MESSAGE("Video bitstream type:%x, vdec_mid:%x!\n", tmp, pCedar->vdec_mid);
    tmp = dvbctrl->vdFmt.codec_type;

    if(dvbctrl->vdec_mid)                 /* 判断是否有 video decoder 插件        */
    {
        if(!dvbctrl->vdec)
        {
            dvbctrl->vdec = esMODS_MOpen(dvbctrl->vdec_mid, 0);
        }
        if(dvbctrl->vdec == (__mp *)0)
        {
            WARNING("Cedar:playfile():open video decoder fail!\n");
            goto _err_start_plg_fail;
        }

        /*判断是否支持当前文件格式*/
        if(!esMODS_MIoctrl(dvbctrl->vdec, CEDAR_VDEC_CMD_CHKSPT, tmp, 0))
        {
            oldmp = dvbctrl->vdec;
            dvbctrl->vdec = 0;
            esMODS_MClose(oldmp);
            esMODS_MUninstall(dvbctrl->vdec_mid);
            dvbctrl->vdec_mid = 0;
        }
        else    //如果支持，把当前格式设给解码器,因为一个vdrv可以解很多格式，所以要把该种格式设置一下
        {
            //esMODS_MIoctrl(pCedar->vdec, CEDAR_VDEC_SET_VBSTYPE, tmp, 0);
            if(esMODS_MIoctrl(dvbctrl->vdec, CEDAR_VDEC_CMD_UPDATE_VDRV, tmp, NULL) != EPDK_OK)
            {
                WARNING("Cedar:UPDATE drv video decoder fail!\n");
                goto _err_start_plg_fail;
            }
        }
    }

    if(dvbctrl->vdec_mid == 0)            /* 如果没有 video decoder 插件或被卸载了*/
    {
        MESSAGE("Try to install video decoder!\n");

        switch(tmp)
        {
            case CEDAR_VBS_TYPE_NONE:
                MESSAGE("Cedar:no video bitstream!\n");
                goto _vd_conti_dvbc_maintsk;

            case CEDAR_VBS_TYPE_UNKNOWN:
                MESSAGE("Cedar:video bitstream not support!\n");
                goto _vd_conti_dvbc_maintsk;

            case CEDAR_VBS_TYPE_XVID:
            case CEDAR_VBS_TYPE_DIVX3:
            case CEDAR_VBS_TYPE_DIVX4:
            case CEDAR_VBS_TYPE_DIVX5:
            case CEDAR_VBS_TYPE_RMG2:
            case CEDAR_VBS_TYPE_SORENSSON_H263:
            case CEDAR_VBS_TYPE_H263:
            case CEDAR_VBS_TYPE_WMV1:
            case CEDAR_VBS_TYPE_RMVB8:
            case CEDAR_VBS_TYPE_RMVB9:
            case CEDAR_VBS_TYPE_MPEG1_ES:
            case CEDAR_VBS_TYPE_MPEG2_ES:
            case CEDAR_VBS_TYPE_MJPEG:
            case CEDAR_VBS_TYPE_H264:
            case CEDAR_VBS_TYPE_VC1:
	        case CEDAR_VBS_TYPE_WMV2:
            case CEDAR_VBS_TYPE_VP6:
            case CEDAR_VBS_TYPE_AVS:
                MESSAGE("install vdec_com.plg!\n");
                dvbctrl->vdec_mid = esMODS_MInstall(CEDAR_PLUGINS_DIRECTORY"vdec_com.plg", 0);
                break;
            case CEDAR_VBS_TYPE_JPEG:
//                MESSAGE("install vdec_jpg.plg!\n");
//                pCedar->vdec_mid = esMODS_MInstall("CEDAR_PLUGINS_DIRECTORYvdec_jpg.plg", 0);
//            case CEDAR_VBS_TYPE_AVS:
//                MESSAGE("install vdec_avs.plg!\n");
//                pCedar->vdec_mid = esMODS_MInstall("CEDAR_PLUGINS_DIRECTORYvdec_avs.plg", 0);
            default:
                WARNING("Cedar:playfile():video bitstream[%x] not support\n", tmp);
                goto _err_start_plg_fail;
        }

        if(dvbctrl->vdec_mid == 0)
        {
            WARNING("Cedar:install video decoder fail!\n");
            goto _err_start_plg_fail;
        }
       MESSAGE("Install video decoder successed, mod_id:%x!\n", dvbctrl->vdec_mid);
    }

    if(!dvbctrl->vdec)
    {
        MESSAGE("Cedar:try to open video decoder!\n");
        dvbctrl->vdec = esMODS_MOpen(dvbctrl->vdec_mid, 0);
        if(dvbctrl->vdec == (__mp *)0)
        {
            WARNING("Cedar:open video decoder fail!\n");
            goto _err_start_plg_fail;
        }
        if(EPDK_OK!=esMODS_MIoctrl(dvbctrl->vdec, CEDAR_VDEC_CMD_UPDATE_VDRV, tmp, NULL))
        {
            WARNING("Cedar:UPDATE drv video decoder fail!\n");
            oldmp = dvbctrl->adec;
            dvbctrl->adec = (__mp*)0;
            esMODS_MClose(oldmp);
            goto _err_start_plg_fail;
        }
        
    }
    //打开vdec,现在分配资源不在esMODS_MOpen()中进行
    if(EPDK_OK != esMODS_MIoctrl(dvbctrl->vdec, CEDAR_VDEC_CMD_OPEN, 0, NULL))
    {
        WARNING("cedar:vdec_com open fail\n");
        oldmp = dvbctrl->vdec;
        dvbctrl->vdec = (__mp *)0;
        esMODS_MClose(oldmp);
        goto _err_start_plg_fail;
    }

    MESSAGE("Cedar:set video bitstream socket!\n");
    
    MESSAGE("play_media: set video bit stream socket!\n");
    esMODS_MIoctrl(dvbctrl->vdec, CEDAR_VDEC_CMD_SET_VBSSKTIN, 0, (void *)dvbctrl->pVBSSktout);
    esMODS_MIoctrl(dvbctrl->vdec, CEDAR_VDEC_CMD_SET_AVSHDL, 0, dvbctrl->avsync);
    esMODS_MIoctrl(dvbctrl->vdec, CEDAR_VDEC_CMD_SET_ROTATE, 0, NULL);
    esMODS_MIoctrl(dvbctrl->vdec, CEDAR_VDEC_CMD_ENABLE_VIDEO_AUTO_SCALE, 0, NULL);
    esMODS_MIoctrl(dvbctrl->vdec, CEDAR_VDEC_CMD_SET_RESERVED_MEM_SIZE, dvbctrl->nReservedMemSize, NULL);
    if(dvbctrl->nMafEnable > 1)
    {
        nVdrvMafEnable = 1;
    }
    else
    {
        nVdrvMafEnable = 0;
    }
    esMODS_MIoctrl(dvbctrl->vdec, CEDAR_VDEC_CMD_ENABLE_MAF, nVdrvMafEnable, NULL);
		esMODS_MIoctrl(dvbctrl->vdec, CEDAR_VDEC_CMD_SET_ROTATE, dvbctrl->rotate_mode, NULL);
		esMODS_MIoctrl(dvbctrl->vdec, CEDAR_VDEC_CMD_SET_TV_MODE, dvbctrl->tvMode, NULL);
    esMODS_MIoctrl(dvbctrl->adec, CEDAR_ADEC_CMD_SET_WITH_VIDEO, 1, 0);

    MESSAGE("play_media: start video decoder\n");
    if(esMODS_MIoctrl(dvbctrl->vdec, CEDAR_VDEC_CMD_START, 0, 0) != EPDK_OK)
    {
        WARNING("play_media: start video decoder fail.\n");
        goto _err_start_plg_fail;
    }
	
	//* set vdec to avsync
	//esMODS_MIoctrl(dvbctrl->avsync, DRV_AVS_CMD_SET_VDEC_HDLR, 0, (void*)dvbctrl->vdec);

_vd_conti_dvbc_maintsk:
#if PLAY_SUBTITLE_STREAM
	//*********************************************************************************************//
	//************************************* subtitle decoder* *************************************//
	//*********************************************************************************************//
	tmp = dvbctrl->subtFmt.codec_type;
	if(dvbctrl->sdec_mid)                 /* 判断是否有 audio decoder 插件        */
    {
        if(!dvbctrl->sdec)
        {
            dvbctrl->sdec = esMODS_MOpen(dvbctrl->sdec_mid, 0);
        }
        if(dvbctrl->sdec == (__mp *)0)
        {
            WARNING("Cedar:open lyric decoder fail!\n");
            goto _err_start_plg_fail;
        }

        /*判断是否支持当前文件格式*/
        if(!esMODS_MIoctrl(dvbctrl->sdec, CEDAR_LDEC_CMD_CHKSPT, tmp, 0))
        {
            oldmp = dvbctrl->sdec;
            dvbctrl->sdec = 0;
            esMODS_MClose(oldmp);
            esMODS_MUninstall(dvbctrl->sdec_mid);
            dvbctrl->sdec_mid = 0;
        }
    }
    //* install subtitle decoder
    if (dvbctrl->sdec_mid == 0)
    {
        switch(dvbctrl->subtFmt.codec_type)
        {
        case CEDAR_LRC_TYPE_DVBSUB:
              WARNING("play_media: no support DVB subtitle!\n");
             // goto _subt_conti_dvbc_maintsk;
             // dvbctrl->sdec_mid = esMODS_MInstall(CEDAR_PLUGINS_DIRECTORY"ldec_itx.plg", 0);
              
        case CEDAR_LRC_TYPE_ISDB:
            MESSAGE("play_media: install moudle sdec_itx.plg\n");
            // goto _subt_conti_dvbc_maintsk;
            dvbctrl->sdec_mid = esMODS_MInstall(CEDAR_PLUGINS_DIRECTORY"ldec_itx.plg", 0);
            break;

        default:
            MESSAGE("play_media: no subtitle bit stream!\n");
            goto _subt_conti_dvbc_maintsk;
        }

        if(dvbctrl->sdec_mid == 0)
        {
            WARNING("play_media: install subtitle decoder fail!\n");
            goto _err_start_plg_fail;
        }
		
        dvbctrl->sdec = NULL;
    }

    if(!dvbctrl->sdec)
    {
        dvbctrl->sdec = esMODS_MOpen(dvbctrl->sdec_mid, 0);
        if(dvbctrl->sdec == NULL)
        {
            WARNING("play_media: open subtitle decoder fail!\n");
            goto _err_start_plg_fail;
        }
		MESSAGE("open subtitle decoder success!\n");
    }
    if(EPDK_OK!=esMODS_MIoctrl(dvbctrl->sdec, CEDAR_LDEC_CMD_OPEN, 0, NULL))    
    {
        WARNING("Cedar:lyric decoder IO_Open fail!\n");
        goto _err_start_plg_fail;
    }
    //内置字幕模块还是通过socket传送数据.
    //* connect parser to sdec with lbs skt
    esMODS_MIoctrl(dvbctrl->sdec, CEDAR_LDEC_CMD_SET_LBSSKTIN, 0, (void *)dvbctrl->pLBSSktout);
    //* set avsync driver module handle to audio decoder
    esMODS_MIoctrl(dvbctrl->sdec, CEDAR_LDEC_CMD_SET_LBSTYPE, tmp, NULL);
    //set av-sync module handle to lyric decoder
    esMODS_MIoctrl(dvbctrl->sdec, CEDAR_LDEC_CMD_SET_AVSHDL, 0, dvbctrl->avsync);
    //设置字幕要帧模式.数字电视统一采用subtitle_plybk要帧模式.简化应用程序.
    esMODS_MIoctrl(dvbctrl->sdec, CEDAR_LDEC_CMD_SET_ReqSubtitleFrameMode, ReqSubtitleFrameMode_SubtitleplybkReq, NULL);
    //设置oneseg还是fullseg的标记
    esMODS_MIoctrl(dvbctrl->sdec, CEDAR_LDEC_CMD_SET_ONESEGFLAG, dvbctrl->nOneSegFlag, NULL);
    
    if(esMODS_MIoctrl(dvbctrl->sdec, CEDAR_LDEC_CMD_START, 0, 0) != EPDK_OK)
    {
        WARNING("play_media: start subtitle decoder fail.\n");
       // goto _err_start_plg_fail;  //只要video audio成功，字幕失败没关系，modify by yefl at 20151029 
    }

_subt_conti_dvbc_maintsk:
#endif
    /****************************************************************/
    /* 启动audio renderer的插件                                     */
    /****************************************************************/
    MESSAGE("Cedar:try to install audio renderer\n");

    if(dvbctrl->adec_mid == 0)  //表示没有audio,那么卸载ardr
    {
        //no audio bitstream, uninstall audio render, if it is exist
        if(dvbctrl->ardr)
        {
            oldmp = dvbctrl->ardr;
            dvbctrl->ardr = NULL;
            esMODS_MClose(oldmp);
        }
        if(dvbctrl->ardr_mid)
        {
            esMODS_MUninstall(dvbctrl->ardr_mid);
            dvbctrl->ardr = 0;
            dvbctrl->ardr_mid = 0;
        }
        goto _ar_conti_cedar_maintsk;
    }

    tmp = esMODS_MIoctrl(dvbctrl->adec, CEDAR_ADEC_CMD_GET_RDRTYPE, 0, 0);

    if(dvbctrl->ardr_mid)                 /* 判断是否有ardr 插件                  */
    {
        if(!dvbctrl->ardr)
        {
            dvbctrl->ardr = esMODS_MOpen(dvbctrl->ardr_mid, 0);
        }
        if(dvbctrl->ardr == (__mp *)0)
        {
            WARNING("Cedar:open audio renderer fail!\n");
            goto _err_start_plg_fail;
        }

        /*判断是否支持当前的后处理类型*/
        if(!esMODS_MIoctrl(dvbctrl->ardr, CEDAR_ARDR_CMD_CHKSPT, tmp, 0))
        {
            oldmp = dvbctrl->ardr;
            dvbctrl->ardr = 0;
            esMODS_MClose(oldmp);
            esMODS_MUninstall(dvbctrl->ardr_mid);
            dvbctrl->ardr_mid = 0;
        }
    }

    if(dvbctrl->ardr_mid == 0)
    {
        /* 如果没有ardr插件或被卸载了 */
        MESSAGE("Try to install audio renderer!\n");
        //WARNING("ARDR before install!\n");
        switch(tmp)
        {
            case CEDAR_ADEC_ARDR_TYPE_SW:
                dvbctrl->ardr_mid = esMODS_MInstall(CEDAR_PLUGINS_DIRECTORY"ardr_sw.plg", 0);
                break;

            case CEDAR_ADEC_ARDR_TYPE_HW:
                dvbctrl->ardr_mid = esMODS_MInstall(CEDAR_PLUGINS_DIRECTORY"ardr_hw.plg", 0);
                break;

            default:
                WARNING("Cedar:rdr not support\n");
                goto _err_start_plg_fail;
        }

        if(dvbctrl->ardr_mid == 0)
        {
            WARNING("Cedar:install audio renderer fail!\n");
            goto _err_start_plg_fail;
        }

        MESSAGE("Install audio renderer successed, mod_id:%x!\n", dvbctrl->ardr_mid);
    }

    if(!dvbctrl->ardr)
    {
        //WARNING("ARDR before Mopen!\n");
        dvbctrl->ardr = esMODS_MOpen(dvbctrl->ardr_mid, 0);
        if(dvbctrl->ardr == (__mp *)0)
        {
            WARNING("Cedar:open audio renderer fail!\n");
            goto _err_start_plg_fail;
        }
        //WARNING("ARDR Mopen done!\n");
        MESSAGE("Open audio renderer successed!\n");
    }
    if(EPDK_OK!=esMODS_MIoctrl(dvbctrl->ardr, CEDAR_ARDR_CMD_OPEN, 0, NULL))
    {
        WARNING("cedar:ardr open fail\n");
        oldmp = dvbctrl->ardr;
        dvbctrl->ardr = (__mp *)0;
        esMODS_MClose(oldmp);
        goto _err_start_plg_fail;
    }

    /*    connect adec to ardr with pcm skt    */
    skt = (__hdle)esMODS_MIoctrl(dvbctrl->adec, CEDAR_ADEC_CMD_GET_PCMSKTOUT, 0, 0);
    esMODS_MIoctrl(dvbctrl->ardr, CEDAR_ARDR_CMD_SET_PCMSKTIN, 0, (void *)skt);

    /* check if need set audio render some parameter */
    if(dvbctrl->aud_raw_data_mode)
    {
        esMODS_MIoctrl(dvbctrl->ardr, CEDAR_ARDR_CMD_SET_EQ, CEDAR_AUD_EQ_TYPE_NORMAL, dvbctrl->setting_inf.usr_eq_filter);
        esMODS_MIoctrl(dvbctrl->ardr, CEDAR_ARDR_CMD_SET_VPS, 0, 0);
        esMODS_MIoctrl(dvbctrl->ardr, CEDAR_ARDR_CMD_SET_SPECTRUM, 0, 0);
        esMODS_MIoctrl(dvbctrl->adec, CEDAR_ADEC_CMD_SET_AB_LOOPCNT, 0, 0);
    }
    else
    {
        if(!dvbctrl->vdec)
        {
            esMODS_MIoctrl(dvbctrl->ardr, CEDAR_ARDR_CMD_SET_EQ, dvbctrl->setting_inf.eq, dvbctrl->setting_inf.usr_eq_filter);
            esMODS_MIoctrl(dvbctrl->ardr, CEDAR_ARDR_CMD_SET_VPS, dvbctrl->setting_inf.vpsspeed*10, 0);
            esMODS_MIoctrl(dvbctrl->ardr, CEDAR_ARDR_CMD_SET_SPECTRUM, dvbctrl->setting_inf.spectrum, 0);
            esMODS_MIoctrl(dvbctrl->adec, CEDAR_ADEC_CMD_SET_AB_LOOPCNT, dvbctrl->setting_inf.ab_loopcnt, 0);
        }
        else
        {
            //esMODS_MIoctrl(pCedar->ardr, CEDAR_ARDR_CMD_SET_EQ, CEDAR_AUD_EQ_TYPE_NORMAL, pCedar->setting_inf.usr_eq_filter);
            esMODS_MIoctrl(dvbctrl->ardr, CEDAR_ARDR_CMD_SET_EQ, dvbctrl->setting_inf.eq, dvbctrl->setting_inf.usr_eq_filter);
            esMODS_MIoctrl(dvbctrl->ardr, CEDAR_ARDR_CMD_SET_VPS, dvbctrl->setting_inf.vpsspeed*10, 0);
            esMODS_MIoctrl(dvbctrl->ardr, CEDAR_ARDR_CMD_SET_SPECTRUM, 0, 0);
            esMODS_MIoctrl(dvbctrl->adec, CEDAR_ADEC_CMD_SET_AB_LOOPCNT, 0, 0);
        }
    }

    //set audio decode driver handle to audio render
    result = esMODS_MIoctrl(dvbctrl->adec, CEDAR_ADEC_CMD_GET_ADRVHDL, 0, 0);
    if(esMODS_MIoctrl(dvbctrl->ardr, CEDAR_ARDR_CMD_SET_ADRVHDL, 0, (void *)result) != EPDK_OK)
    {
        WARNING("set aduio decode driver handle to audio render failed!\n");
        goto _err_start_plg_fail;
    }

    //设置ardr输出声音的软件增益
    esMODS_MIoctrl(dvbctrl->ardr, CEDAR_ARDR_CMD_SET_SWGAIN, dvbc->setting_inf.swgain, 0);
    /* set av-sync driver handle to audio render    */
    esMODS_MIoctrl(dvbctrl->ardr, CEDAR_ARDR_CMD_SET_AVSHDL, 0, dvbctrl->avsync);
    //同时也把ardr设置给avsync,avsync需要调用它去变速.
    esMODS_MIoctrl(dvbctrl->avsync, DRV_AVS_CMD_SET_ARDRHDL, 0, dvbctrl->ardr);
#if((MAPLE_TV_STANDAR == MAPLE_TV_ISDB_T)||(MAPLE_TV_STANDAR == MAPLE_TV_DVB_T)\
||(MAPLE_TV_STANDAR == MAPLE_TV_DTMB)||(MAPLE_TV_STANDAR == MAPLE_TV_ATSC)\
||(MAPLE_TV_STANDAR == MAPLE_TV_DVB_T2))
    esMODS_MIoctrl(dvbctrl->avsync, DRV_AVS_CMD_SET_SYNCMODE, DRV_AVS_SYNCMODE_A2V, NULL);
#elif(MAPLE_TV_STANDAR == MAPLE_TV_CMMB)
		esMODS_MIoctrl(dvbctrl->avsync, DRV_AVS_CMD_SET_SYNCMODE, DRV_AVS_SYNCMODE_V2A, NULL);
#endif

    /*    start ardr                            */
    //WARNING("ARDR before start!\n");
    if(esMODS_MIoctrl(dvbctrl->ardr, CEDAR_ARDR_CMD_START, 0, 0) != EPDK_OK)
    {
        goto _err_start_plg_fail;
    }
    //WARNING("ARDR start done!\n");
_ar_conti_cedar_maintsk:
	//*********************************************************************************************//
	//**************************************** Audio Playback ***************************************//
	//********************************************************************** **********************//
    //* start audio play back.
    if(!dvbctrl->ardr_mid)
    {
        MESSAGE("no audio renderer!\n");
        //no audio bitstream, uninstall audio plybk, if it is exist
        if(dvbctrl->aply)
        {
            oldmp = dvbctrl->aply;
            dvbctrl->aply = NULL;
            esMODS_MClose(oldmp);
        }
        if(dvbctrl->aply_mid)
        {
            esMODS_MUninstall(dvbctrl->aply_mid);
            dvbctrl->aply = NULL;
            dvbctrl->aply_mid = 0;
        }
        goto _apb_conti_dvbc_maintsk;
    }

    if(!dvbctrl->aply_mid)
    {
        MESSAGE("Try to install audio playback!\n");

        //pCedar->aply_mid = aply_MInit();
        dvbctrl->aply_mid = esMODS_MInstall(CEDAR_PLUGINS_DIRECTORY"aply.plg", 0);
        if(dvbctrl->aply_mid == 0)
        {
            WARNING("Cedar:install aply fail!\n");
            goto _err_start_plg_fail;
        }

        MESSAGE("Install audio playback successed, mod_id:%x!\n", dvbctrl->aply_mid);
    }

    if(!dvbctrl->aply)
    {
        //pCedar->aply = aply_MOpen(pCedar->aply_mid, 0);
        dvbctrl->aply = esMODS_MOpen(dvbctrl->aply_mid, 0);
        if(!dvbctrl->aply)
        {
            WARNING("Open audio playback failed!\n");
            goto _err_start_plg_fail;
        }

        MESSAGE("open audio playback successed!\n");
    }
    if(EPDK_OK!=esMODS_MIoctrl(dvbctrl->aply, CEDAR_APLY_CMD_OPEN, 0, NULL))
    {
        WARNING("cedar aply open fail\n");
        oldmp = dvbctrl->aply;
        dvbctrl->aply = (__mp*)0;
        esMODS_MClose(oldmp);
        goto _err_start_plg_fail;
    }
    //set cedarini to aply
    //esMODS_MIoctrl(dvbctrl->aply, CEDAR_APLY_CMD_SET_CEDARINI, 0, &dvbctrl->CustomCedarINI);
    esMODS_MIoctrl(dvbctrl->aply, CEDAR_APLY_CMD_SET_CEDARINI, 0, &dvbctrl->pMapleINI->CedarINIInstance);
    //把cedar自己的句柄设给aply
    esMODS_MIoctrl(dvbctrl->aply, CEDAR_APLY_CMD_SET_CEDARHDL, 0, (void*)dvbctrl);

    //* connect adec to ardr with pcm skt
    skt = (__hdle)esMODS_MIoctrl(dvbctrl->ardr, CEDAR_ARDR_CMD_GET_PCMSKTOUT, 0, 0);
    esMODS_MIoctrl(dvbctrl->aply, CEDAR_APLY_CMD_SET_PCMSKTIN, 0, (void *)skt);
    //* av-sync module handle to audio play back
    esMODS_MIoctrl(dvbctrl->aply, CEDAR_APLY_CMD_SET_AVSHDL, 0, (void *)dvbctrl->avsync);
	esMODS_MIoctrl(dvbctrl->aply, CEDAR_APLY_CMD_SET_RAW_DATA_MODE, is_aud_raw_type, NULL);
    //设置cedar的CmdQ,作为aply的反馈消息通道，目前仅音频文件的jump方式的AB播放会用到它,为消息通道做callback函数
    //esMODS_MIoctrl(pCedar->aply, CEDAR_APLY_CMD_SET_MSG_QUEUE, 0, pCedar->CedarCmdQ);
    call_back = esKRNL_GetCallBack((__pCBK_t)CB_CedarMsgQPost);
    esMODS_MIoctrl(dvbctrl->aply, CEDAR_APLY_CMD_SET_CB_MSGQPOST, 0, (void *)call_back);
    if(dvbctrl->hasVideo)   //有video,aply就不能自己启动video clock了.
    {
        esMODS_MIoctrl(dvbctrl->aply, CEDAR_APLY_CMD_NEED_VIDEOCLOCK_START, 0, NULL);
    }
    else
    {
        esMODS_MIoctrl(dvbctrl->aply, CEDAR_APLY_CMD_NEED_VIDEOCLOCK_START, 1, NULL);
    }
    esMODS_MIoctrl(dvbctrl->aply, CEDAR_APLY_CMD_SET_DTV_DELAY_TIME, DTV_DELAY_TIME, NULL);
    
    MESSAGE("play_media: open audio playback success!\n");

_apb_conti_dvbc_maintsk:
	//*********************************************************************************************//
	//**************************************** Video Playback ***************************************//
	//********************************************************************** **********************//

    //* start video play back.
    if(!dvbctrl->vdec_mid)
    {
        MESSAGE("no video bitstream!\n");
        //
        //增加vply的句柄的判断,如果vply不为0,还是要close并卸载的,因为这个文件如果没有视频数据，vply就应该卸载了.
        UninstallVply(dvbctrl);
        goto _vpb_conti_dvbc_maintsk;
    }

    if(!dvbctrl->vply_mid)
    {
        MESSAGE("Try to install video playback!\n");

        //dvbctrl->vply_mid = vply_MInit();
        dvbctrl->vply_mid = esMODS_MInstall(CEDAR_PLUGINS_DIRECTORY"vply.plg", 0);

        MESSAGE("Install video playback successed, mod_id:%x!\n", dvbctrl->vply_mid);
    }

    if(!dvbctrl->vply)
    {
        //在打开vply模块前，先测试de驱动
        vply_work_mode = VPLY_WORK_MODE_TASK_ACTIVE;
        //dvbctrl->vply = vply_MOpen(dvbctrl->vply_mid, 0);
        dvbctrl->vply = esMODS_MOpen(dvbctrl->vply_mid, 0);
        if(!dvbctrl->vply)
        {
            WARNING("Open video playback failed!\n");
            goto _err_start_plg_fail;
            //return EPDK_FAIL;
        }

        MESSAGE("open video playback successed!\n");
    }
    tmpret = esMODS_MIoctrl(dvbctrl->vply, CEDAR_VPLY_CMD_OPEN, 0, NULL);
    if(tmpret!=EPDK_OK)
    {
        WARNING("vply IO_Open fail\n");
        goto _err_start_plg_fail;
        //return EPDK_FAIL;
    }
    /* 设置vply工作模式*/
    esMODS_MIoctrl(dvbctrl->vply, CEDAR_VPLY_CMD_SET_WORK_MODE, vply_work_mode, NULL);
    //设置文件切换是否是无缝切换模式
    esMODS_MIoctrl(dvbctrl->vply, CEDAR_VPLY_CMD_SET_FILE_SWITCH_VPLY_MODE, dvbctrl->nFileSwitchVplyMode, NULL);
    //* connect video decoder to vply with pic skt
    skt = (__hdle)esMODS_MIoctrl(dvbctrl->vdec, CEDAR_VDEC_CMD_GET_PICSKTOUT, 0, 0);
    esMODS_MIoctrl(dvbctrl->vply, CEDAR_VPLY_CMD_SET_PICSKTIN, 0, (void *)skt);
    esMODS_MIoctrl(dvbctrl->vply, CEDAR_VPLY_CMD_SET_AVSHDL, 0, dvbctrl->avsync);
    //设置cedar的CmdQ,作为vply的反馈消息通道，目前仅视频文件的AB播放会用到它
    //esMODS_MIoctrl(dvbctrl->vply, CEDAR_VPLY_CMD_SET_MSG_QUEUE, 0, dvbctrl->CedarCmdQ);
    call_back = esKRNL_GetCallBack((__pCBK_t)CB_CedarMsgQPost);
    esMODS_MIoctrl(dvbctrl->vply, CEDAR_VPLY_CMD_SET_CB_MSGQPOST, 0, (void *)call_back);
		    //设置旋转角度模式
    esMODS_MIoctrl(dvbctrl->vply, CEDAR_VPLY_CMD_SET_ROTATE, dvbctrl->rotate_mode, NULL);
 	
    //设置缓冲时间.
    esMODS_MIoctrl(dvbctrl->vply, CEDAR_VPLY_CMD_SET_DTV_DELAY_TIME, DTV_DELAY_TIME, 0);
    WARNING("play_media: open video playback success!\n");

_vpb_conti_dvbc_maintsk:
#if PLAY_SUBTITLE_STREAM
	//*********************************************************************************************//
	//*********************************** subtitle Playback ***************************************//
	//********************************************************************** **********************//
    //* start subtitle play back.
    if(!dvbctrl->sdec_mid)
    {
        MESSAGE("play_media: no subtitle bit stream!\n");
        goto _start_play_back;
    }

    if(!dvbctrl->sply_mid)
    {        
        MESSAGE("Try to install subtitle playback!\n");
        dvbctrl->sply_mid = esMODS_MInstall(CEDAR_PLUGINS_DIRECTORY"sply.plg", 0);
    }
    dvbctrl->sply = esMODS_MOpen(dvbctrl->sply_mid, 0);
    if(!dvbctrl->sply)
    {
        WARNING("play_media: open subtitle playback failed!\n");
        goto _err_start_plg_fail;
    }
    
    tmpret = esMODS_MIoctrl(dvbctrl->sply, CEDAR_SPLY_CMD_OPEN, 0, NULL);
    if(tmpret!=EPDK_OK)
    {
        WARNING("sply IO_Open fail\n");
        goto _err_start_plg_fail;
    }
    //set subtitle moudle callback function;
    esMODS_MIoctrl(dvbctrl->sply, CEDAR_SPLY_CMD_SET_CB_SHOWDRAW, 0, (void *)dvbctrl->CB_ShowDraw);    

    //* connect subtitle decoder to sply moudle;
    esMODS_MIoctrl(dvbctrl->sply, CEDAR_SPLY_CMD_SET_LDECHDL, 0, (void *)dvbctrl->sdec);
    esMODS_MIoctrl(dvbctrl->sply, CEDAR_SPLY_CMD_SET_AVSHDL, 0, (void *)dvbctrl->avsync);

_start_play_back:
#endif
	//*********************************************************************************************//
	//************************************** Start Video Playback *************************************//
	//********************************************************************** **********************//
    //* start video playback
    if(dvbctrl->vply)
    {
        //* set video layer handle
        if(esMODS_MIoctrl(dvbctrl->vply, CEDAR_VPLY_CMD_SET_VIDLAYERHDL, 0, (void *)dvbctrl->setting_inf.vid_layer_hdl) != EPDK_OK)
        {
            WARNING("play_media: set video layer handle failed!\n");
            goto _err_start_plg_fail;
        }
        // set video layer color key enable
        if(esMODS_MIoctrl(dvbctrl->vply, CEDAR_VPLY_CMD_CONFIG_VIDEO_LYR_CK_ENABLE, dvbc->setting_inf.nCkEnableValue, NULL) != EPDK_OK)
        {
            WARNING("play_media: set video layer ck_enable failed!\n");
            goto _err_start_plg_fail;
        }
        if(esMODS_MIoctrl(dvbctrl->vply, CEDAR_VPLY_CMD_SET_DEINTERLACE_LEVEL, dvbctrl->nMafEnable, NULL) != EPDK_OK)
        {
            WARNING("play_media: set deinterlace_level failed!\n");
            goto _err_start_plg_fail;
        }
        
        //* set video window attribute
        if(dvbctrl->setting_inf.vid_window_flag)
        {
            esMODS_MIoctrl(dvbctrl->vply, CEDAR_VPLY_CMD_SET_CB_SETSRCSCNWINPARA, 0, (void *)dvbctrl->VplyCB_SetSrcScnWinPara);
            if(esMODS_MIoctrl(dvbctrl->vply, CEDAR_VPLY_CMD_SET_VIDWINDOW, \
                dvbctrl->setting_inf.vid_show_mode, (void *)&dvbctrl->setting_inf.vid_window) != EPDK_OK)
            {
                WARNING("play_media: set video window failed!\n");
                goto _err_start_plg_fail;
            }
        }
        else
        {
            WARNING("play_media: video window has not been set!!\n");
            goto _err_start_plg_fail;
        }
        MESSAGE("play_media: start video playback!\n");
        esMODS_MIoctrl(dvbctrl->vply, CEDAR_VPLY_CMD_SWITCH_PIC_SHOW, dvbctrl->pic_show_flag, 0);
        if(esMODS_MIoctrl(dvbctrl->vply, CEDAR_VPLY_CMD_START, 0, 0) != EPDK_OK)
        {
            WARNING("play_media: start video playback failed!\n");
            goto _err_start_plg_fail;
        }
    }

	//*********************************************************************************************//
	//************************************** Start Audio Playback *************************************//
	//********************************************************************** **********************//
    //* start audio playback
    if(dvbctrl->aply)
    {
        MESSAGE("play_media: start audio playback!\n");
        if(esMODS_MIoctrl(dvbctrl->aply, CEDAR_APLY_CMD_START, 0, 0) != EPDK_OK)
        {
            WARNING("play_media: start audio playback failed!\n");
            goto _err_start_plg_fail;
        }
    }
    
    
	//*********************************************************************************************//
	//************************************** Start Subtitle Playback *************************************//
	//********************************************************************** **********************//
    //* start audio playback
#if PLAY_SUBTITLE_STREAM
    if(dvbctrl->sply)
    {
        MESSAGE("play_media: start subtitle playback!\n");
        if(esMODS_MIoctrl(dvbctrl->sply, CEDAR_SPLY_CMD_START, 0, 0) != EPDK_OK)
        {
            WARNING("play_media: start subtitle playback failed!\n");
            goto _err_start_plg_fail;
        }
    }
#endif
    
    
    //* check if either audio or video is exist
    if(!(dvbctrl->adec_mid || dvbctrl->vdec_mid))
    {
        WARNING("play_media: neither audio bit stream nor video bit stream is supported!\n");
        goto _err_start_plg_fail;
    }

    //* start play media file success.
    return EPDK_OK;

    //* process error
_err_start_plg_fail:
    result = EPDK_FAIL;
  //  uninstall_plugins(dvbctrl);//seamless
    FileStop_process(dvbctrl);
    return result;
}
