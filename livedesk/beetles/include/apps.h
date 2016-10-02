/*
**********************************************************************************************************************
*                                                    ePDK
*                                    the Easy Portable/Player Develop Kits
*                                              eMOD Sub-System
*
*                                   (c) Copyright 2007-2009, Steven.ZGJ.China
*                                             All Rights Reserved
*
* Moudle  : lion
* File    : apps.h
*
* By      : andy.zhang
* Version : v1.0
* Date    : 2008-10-4 11:28:56
**********************************************************************************************************************
*/
#ifndef __APPS_H__
#define __APPS_H__

#define BEETLES_APP_ROOT   "d:\\"//"f:\\rootfs\\"//程序和资源的所在盘符.如果非调试，则定义为"d:\\"
#define BEETLES_RTC_EXIST	0
#define EXTERN_KARAOKE_EXIST 0 //是否需要卡拉OK ,add by hot.lee


#define SUPPORT_DIGIT_KEY

#define ANTENNA_MODE_SELECT    0// 选择天线的方式 ；拉杆天线，外接天线


#define BEETLES_POWEROFF_CHEAK          0
#define BEETLES_UPDATE_ENABLE	        1//update.mod del
#define BEETLES_LINEIN_DETECT           0//example never use
#define BEETLES_IOCTRL_CHECK            0//linein initserver
#define BEETLES_IOECHO_CHECK            1//echo initserver
#define BEETLES_KEYTONE_ENABLE          0
#define BEETLES_SYSINFO_ENABLE   	    1//1-enable,0-disable
#define BEETLES_FM_SEARCH_PREVIEW   	1//fm
#define BEETLES_DEVICE_PLUGIN_NOPLAY   	1//video no play


#define BEETLES_KEY_REDEF       0////c700 dtmb

#define BEETLES_SPEAK_ON   	    1////1-c700 dtmb
#define BEETLES_SPEAK_OFF       0////0

#define AB_D_UPEN   			0 ////a-1....
#define AB_D_PORT   			3 ////a-1....
#define AB_D_PORTNUM   		    7 ////1-1...

#define RF_SW_PORT              2
#define RF_SW_PORTNUM           8

#define ANTENN_POWER_PORT       2
#define ANTENN_POWER_PORTNUM    7

#define LINEIN_PORT   			3
#define LINEIN_PORTNUM   		5

#define ECHO_PORT   			3
#define ECHO_PORTNUM   		    6

#define BEETLES_ECHO_4305   	0
#define BEETLES_FM_HEADBAR   	0

#define BEETLES_TVOUT_ENABLE   	1//1-enable,0-disable

#define BEETLES_MICIN_DISABLE   	0//1-micin disable,0-micin enable

#define BETTLES_BRIGHT_ON_TO_MAX		1  //亮度调到最大值使能


#define CAI_ADD_WRITE_DETECT	1//是否添加写保护



#define BEETLES_BLACK_COLOR	0x99000000
#define BEETLES_GRAY_COLOR	0x99A0A0A0

typedef void* H_KAK;
typedef void* H_KSC;

#define BEETLES_BRIGHT_SETTING_CHANGE_FLAG 0

#define BEETLES_FONT_SIZE_INT 24
#define BEETLES_FONT_SIZE_STR "24"

#define AW_SDK_PMU    1 

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


/////////////////

#include "epdk.h"
#include "mod_orange.h"
#include "mod_orange\orange_api_entry.h"
//#include "mod_flash.h"
#include "applets/app_load_para.h"
#include "applets/tv_visible_area.h"

#include "elibs\lib_ex.h"

#include "mod_desktop\desktop.h"

#include "init\init.h"

#include "libapps\libapps_i.h"

#include "res\lang.h"
#include "res\theme.h"

#endif

