/*
*********************************************************************************************************
*											        ePDK
*						            the Easy Portable/Player Develop Kits
*									           ir key driver
*
*						        (c) Copyright 2006-2007, Steven.ZGJ China
*											All	Rights Reserved
*
* File    : DEV_KEY.c
* By      : Steven.ZGJ
* Version : V1.00
*********************************************************************************************************
*/

#include "drv_fm_i.h"
//#include "rda5820.h"
//#include "tea5767.h"
//#include "rda5807p.h"
#include "RDA_FM5807M_drv.h"

extern    __krnl_event_t  *fm_lock;
extern    __fm_drv_t       fm_drv;
ES_FILE   *fiic;
/*
**********************************************************************************************************************
*                                               FUNCTION
*
* Description:
*
* Arguments  :
*
* Returns    :
*
* Notes      :
*
**********************************************************************************************************************
*/
static void FM_OpLock(void)
{
    __u8  err;

    esKRNL_SemPend(fm_lock, 0, &err);
}
/*
**********************************************************************************************************************
*                                               FUNCTION
*
* Description:
*
* Arguments  :
*
* Returns    :
*
* Notes      :
*
**********************************************************************************************************************
*/
static void FM_OpUnlock(void)
{
    esKRNL_SemPost(fm_lock);
}
/*
****************************************************************************************************
*
*             DEV_FM_Open
*
*  Description:
*       DEV_FM_Open
*
*  Parameters:
*
*  Return value:
*       EPDK_OK
*       EPDK_FAIL
****************************************************************************************************
*/
__hdle DEV_FM_Open(void * open_arg, __u32 mode)
{
    if(!fm_drv.fm_dev.iic_file)
    {
        return (__hdle)0;
    }
    else
    {
        fiic = fm_drv.fm_dev.iic_file;

        //eLIBs_fioctrl(fm_drv.fm_dev.power_file, DRV_POWER_CMD_VSTS_SET, POWER_VOL_LDO5, (void *)POWER_VOL_ON);
        //esKRNL_TimeDly(5);
        //fm_mute(DRV_FM_VOICE_OFF);

        return (__hdle)&(fm_drv.fm_dev);
    }
}
/*
****************************************************************************************************
*
*             DEV_FM_Close
*
*  Description:
*       DEV_FM_Close
*
*  Parameters:
*
*  Return value:
*       EPDK_OK
*       EPDK_FAIL
****************************************************************************************************
*/
__s32 DEV_FM_Close(__hdle hfm)
{
    //eLIBs_fioctrl(fm_drv.fm_dev.power_file, DRV_POWER_CMD_VSTS_SET, POWER_VOL_LDO5, (void *)POWER_VOL_OFF);
    //fm_mute(DRV_FM_VOICE_OFF);
    fiic = NULL;

	return EPDK_OK;
}
/*
****************************************************************************************************
*
*             DEV_FM_Read
*
*  Description:
*       DEV_FM_Read
*
*  Parameters:
*
*  Return value:
*       size*n
*
****************************************************************************************************
*/
__u32 DEV_FM_Read(void *pdata, __u32 size, __u32 n, __hdle hPower)
{
    return 0;
}
/*
****************************************************************************************************
*
*             DEV_FM_Write
*
*  Description:
*       DEV_FM_Write
*
*  Parameters:
*
*  Return value:
*       size*n
*
****************************************************************************************************
*/
__u32 DEV_FM_Write(const void *pdata, __u32 size, __u32 n, __hdle hPower)
{
    return size * n;
}
/*
****************************************************************************************************
*
*             DEV_FM_Ioctrl
*
*  Description:
*       DEV_FM_Ioctrl
*
*  Parameters:
*
*  Return value:
*       EPDK_OK
*       EPDK_FAIL
****************************************************************************************************
*/
//////////////
#define SET_REG_BIT(val, pos)               ((val) |= 1U<<(pos))
#define CLEAR_REG_BIT(val, pos)             ((val) &= ~(1U<<(pos)))
#define DAC_ANA_RCH_ENABLE_BIT			31U			//internal dac right channel enable bit;0:disable,1:enable
#define DAC_ANA_LCH_ENABLE_BIT			30U			//internal dac left channel enable bit;0:disable,1:enable
#define DAC_ANA_MIX_ENABLE_BIT			29U			//analog output mixer enable
#define DAC_ANA_LINEIN_GAIN_BIT			26U			//line_in gain stage to output mixer gain control,change in A10
#define DAC_ANA_FM_GAIN_POS				23U			//FM input to output mixer gain control,bit23~bit25
#define DAC_ANA_FM_GAIN_LEN				3U			
#define DAC_ANA_MIC_GAIN_POS			20U			//MIC1/2 input to output mixer gain control,bit20~bit22
#define DAC_ANA_MIC_GAIN_LEN			3U
#define DAC_ANA_LLIN_TO_LMIX_BIT		19U			//left line_in gain stage to left output mixer mute;0:mute,1:not mute
#define DAC_ANA_RLIN_TO_RMIX_BIT		18U			//right line_in gain stage to right output mixer mute;0:mute,1:not mute
#define DAC_ANA_LFM_TO_LMIX_BIT			17U			//left fm gain stage to left output mixer mute;0:mute,1:not mute
#define DAC_ANA_RFM_TO_RMIX_BIT			16U			//right fm gain stage to right output mixer mute;0:mute,1:not mute
#define DAC_ANA_LDAC_TO_LMIX_BIT		15U			//left dac to left output mixer mute;0:mute,1:not mute
#define DAC_ANA_RDAC_TO_RMIX_BIT		14U			//right dac to right output mixer mute;0:mute,1:not mute
#define DAC_ANA_LDAC_TO_RMIX_BIT		13U			//left dac to right output mixer mute;0:mute,1:not mute
#define DAC_ANA_LMIC1_TO_LMIX_BIT		12U			//left MIC1 gain stage to left output mixer mute;0:mute,1:not mute
#define DAC_ANA_RMIC1_TO_RMIX_BIT		11U			//right MIC1 gain stage to right output mixer mute;0:mute,1:not mute
#define DAC_ANA_LMIC2_TO_LMIX_BIT		10U			//left MIC2 gain stage to left output mixer mute;0:mute,1:not mute
#define DAC_ANA_RMIC2_TO_RMIX_BIT		9U			//right MIC2 gain stage to right output mixer mute;0:mute,1:not mute
#define DAC_ANA_DAC_TO_PA_MUTE_BIT		8U			//dac to pa mute;0:mute 1:not mute
#define DAC_ANA_MIX_TO_PA_MUTE_BIT		7U			//output mixer to pa mute;0:mute 1:not mute
#define DAC_ANA_PA_MUTE_BIT				6U			//all input source to pa mute;0:mute 1:not mute
#define DAC_ANA_PA_VOL_POS				0U			//pa volume control,64 level,mute when 0;bit0~bit5
#define DAC_ANA_PA_VOL_LEN				6U


#define MIC_DAC_CTL  (*(volatile unsigned int*)0xf1c22c10)
#define MIC_ADC_CTL  (*(volatile unsigned int*)(0xf1c22c00 + 0x28))


//mic 直通功能开，mic  采样信号(可能经过微小放大)直接通过功放输出
//进音乐和视频加载混响会有问题，所以提供
//进音乐和视频时必须先调用dsk_com_mic_close,再调用此函数
static void dsk_audio_direct_output_onoff(__u8 flag)
{
	__u32 val;

	if(0 == flag)
	{
		val = MIC_DAC_CTL | (0x200019C0);
		MIC_DAC_CTL = val;

	    val = MIC_ADC_CTL & ~(1U<<4);
		MIC_ADC_CTL = val;		
	}
	else
	{
		val = MIC_DAC_CTL | (0x200019C0);
		MIC_DAC_CTL = val;
		
		val = MIC_ADC_CTL | (1U<<4);
		MIC_ADC_CTL = val;
	}
}


/////////////////

__s32 DEV_FM_Ioctrl(__hdle hPower, __u32 cmd, __s32 aux, void *pbuffer)
{
    __s32                ret;

	ret = EPDK_FAIL;

	if(!fiic)
	{
	    return ret;
	}

    FM_OpLock();

    switch(cmd)
    {
        case DRV_FM_CMD_INIT:
        {
            ES_FILE *paudio;
            fm_init();
            fm_drv.fm_dev.status = FM_DRV_STATUS_RECEIVE;
#if 0
            paudio = eLIBs_fopen("b:\\AUDIO\\FM", "r+");
        	if(!paudio)
        	{
        		__wrn("Open Audio Dev File Failed\n");
        		return EPDK_FAIL;
        	}
            eLIBs_fioctrl(paudio, AUDIO_DEV_CMD_START, 0, 0);
            eLIBs_fclose(paudio);
////////////////
	{
		SET_REG_BIT(READ_REG32(0xf1c22c10), DAC_ANA_RFM_TO_RMIX_BIT);
		SET_REG_BIT(READ_REG32(0xf1c22c10), DAC_ANA_LFM_TO_LMIX_BIT);

		*(volatile unsigned long*)(0xf1c22c10) |= (0x0<<23); ////linein 增益 0:-4.5db  1:-3db 2:-1.5db  3:0db 4:1.5db 5:3db 6:4.5db 7:6db
		__msg("*(volatile unsigned long*)(0xf1c22c10 )=0x%x\n", *(volatile unsigned long*)(0xf1c22c10));
	}
///////////////
#else
        	{
                   //__wrn("\n              LINE IN START succeed!\n");
        	    
                paudio = eLIBs_fopen("b:\\AUDIO\\FM", "r+");
            	if(!paudio)
            	{
            		__wrn("Open Audio Dev File Failed\n");
            		return EPDK_FAIL;
            	}
                eLIBs_fioctrl(paudio, AUDIO_DEV_CMD_START, 0, 0);
                eLIBs_fclose(paudio);

        	    SET_REG_BIT(READ_REG32(0xf1c22c10), DAC_ANA_RFM_TO_RMIX_BIT);
        	    SET_REG_BIT(READ_REG32(0xf1c22c10), DAC_ANA_LFM_TO_LMIX_BIT);
        	    
        	    dsk_audio_direct_output_onoff(1);

                //*(volatile unsigned long*)(0xf1c22c10) |= (0x0<<23); ////linein 增益 0:-4.5db  1:-3db 2:-1.5db  3:0db 4:1.5db 5:3db 6:4.5db 7:6db
                //__wrn("*(volatile unsigned long*)(0xf1c22c10 )=0x%x\n", *(volatile unsigned long*)(0xf1c22c10));
            }
#endif

            ret = fm_drv.fm_dev.status;

            break;
        }
        case DRV_FM_CMD_EXIT:
        {
#if 0			
            ES_FILE  *paudio;

            paudio = eLIBs_fopen("b:\\AUDIO\\FM", "r+");
        	if(paudio)
        	{
        	    eLIBs_fioctrl(paudio, AUDIO_DEV_CMD_STOP, 0, 0);
                eLIBs_fclose(paudio);
            }
            fm_exit();
	//////////////////////////
	{
	    CLEAR_REG_BIT(READ_REG32(0xf1c22c10), DAC_ANA_RFM_TO_RMIX_BIT);
	    CLEAR_REG_BIT(READ_REG32(0xf1c22c10), DAC_ANA_LFM_TO_LMIX_BIT);

	    //change to default
	    *(volatile unsigned long*)(0xf1c22c10) |= (0x1<<23);
	    __msg("*(volatile unsigned long*)(0xf1c22c10 )=0x%x\n", *(volatile unsigned long*)(0xf1c22c10 ));
	}
	////////////////////////
#else
	        fm_exit();
    		{
                ES_FILE  *paudio;

                paudio = eLIBs_fopen("b:\\AUDIO\\FM", "r+");
            	if(paudio)
            	{
            	    eLIBs_fioctrl(paudio, AUDIO_DEV_CMD_STOP, 0, 0);
                    eLIBs_fclose(paudio);
                }
             }
        	{
        	    CLEAR_REG_BIT(READ_REG32(0xf1c22c10), DAC_ANA_RFM_TO_RMIX_BIT);
        	    CLEAR_REG_BIT(READ_REG32(0xf1c22c10), DAC_ANA_LFM_TO_LMIX_BIT);

        		dsk_audio_direct_output_onoff(0);
                //change to default
                //*(volatile unsigned long*)(0xf1c22c10) |= (0x1<<23);
                //__wrn("*(volatile unsigned long*)(0xf1c22c10 )=0x%x\n", *(volatile unsigned long*)(0xf1c22c10 ));
            }
#endif

            ret = EPDK_OK;
            break;
        }
        case DRV_FM_CMD_RECVE_INIT:
        {
            break;
       }
        case DRV_FM_CMD_RECVE_EXIT:
        {

            break;
        }
        case DRV_FM_CMD_RECVE_PLAY:
        {
            fm_play((__s32)pbuffer);

            ret = EPDK_OK;
            break;
        }
        case DRV_FM_CMD_RECVE_SEARCH:
        {
            if((aux & 0xff) == DRV_FM_AUTO_SEARCH)
            {
            	__wrn(" ret = %d,  aux & 0xff00 = 0x%x\n",ret,aux & 0xff00);
                ret = fm_auto_search((__s32)pbuffer, aux & 0xff00);
            }
            else if((aux & 0xff) == DRV_FM_MANUAL_SEARCH)
            {
            	__wrn(" ret = %d,  aux & 0xff00 = 0x%x\n",ret,aux & 0xff00);
                ret = fm_manual_search((__s32)pbuffer, aux & 0xff00);
            }

            break;
        }
        case DRV_FM_CMD_AREA_SET:
        {
            ret = fm_area_choose(aux, pbuffer);

            break;
        }
        case DRV_FM_CMD_STEREO_SET:
        {
            ret = fm_stereo_choose(aux);

            break;
        }

        case DRV_FM_CMD_MUTE:
        {
            ret = fm_mute(aux);

            break;
        }

        case DRV_FM_CMD_SLEVEL_SET:
        {
            ret = fm_signal_level(aux);

            break;
        }

        case DRV_FM_CMD_SEND_INIT:
        {
            ret = fm_send_on();
            if(ret == EPDK_OK)
            {
                fm_drv.fm_dev.status = FM_DRV_STATUS_SEND;
            }
            break;
        }
        case DRV_FM_CMD_SEND_EXIT:
        {
            ret = fm_send_off();

            break;
        }
        case DRV_FM_CMD_SEND_START:
        {
            ret = fm_send((__s32)pbuffer);

            break;
        }
        case DRV_FM_CMD_SEND_PAGAIN_SET:
        {
            ret = fm_pa_gain(aux);

            break;
        }

        case DRV_FM_CMD_QUERY_STATUS:
        {
            //ret = fm_drv.fm_dev.status;
            fm_drv.fm_dev.status = fm_get_status();
            ret = fm_drv.fm_dev.status;

            break;
        }
/*
        case DRV_FM_CMD_DE_EMPHASIS_SET:
        {
            break;
        }
*/
    	default:
		{
			__wrn("Unkonwn Command...\n");

			break;
		}
	}

	FM_OpUnlock();

	return ret;
}

__dev_devop_t fm_dev_ops =
{
    DEV_FM_Open,
    DEV_FM_Close,
    DEV_FM_Read,
    DEV_FM_Write,
    DEV_FM_Ioctrl
};
