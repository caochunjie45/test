/**
 * CopyRight: all rights reserved by rockchips fuzhou inc. 
 * 10.30.2010
 */



//#include <linux/module.h>
//#include <linux/device.h>
//#include <linux/kernel.h>
//#include <mach/hardware.h>
//#include <mach/iomux.h>
//#include <mach/gpio.h>
//#include <mach/irqs.h>

//#include <TSTV.h>
//#include <stdio.h>
#include "MB86A2X_app.h"
#include "MB86A22_API_version.h"
#include "MB86A22_API_defs.h"
        
#include "tuner/Tuner.h"
#include "MB86A22_API_structs.h"
#include "MB86A22_RDA_default_setting.h"
#include "MB86A22_user_define.h"
#include "MB86A22_API.h"
     
#include "MB86A22_BER_CN_Calculation.h"  

#include "tuner/rdatv_bb.h"

#include "demod/Fujitsu_printf.h"


/********************************************************************************************/
//
//
//    DEBUG Options
//
//
/********************************************************************************************/
#if 0
#define DBG(x...)	WARNING(x...) //printk(KERN_ERR x)
#else
#define DBG(x...)
#endif

MB86A2x_PARAM  m_MB86A2xParam;
MB86A2x_LOCKNG_INFO   m_MB86A2xLockinginfo ;
MB86A2x_CH_SCH_INFO   m_MB86A2ch_sch_info ;
MB86A2x_SIG_QUAL_INFO sign_qual_info;
MB86A2x_AGC_INFO      agc_info;
int MB86A2count=0;

#define PARAMETER_USE_FREQ

/********************************************************************************************/
//
//
//    Source Code
//
//
/********************************************************************************************/
char MB86A2x_init(void)
{
	MB86A2x_VERSION_INFO  version_info ;	/* prepare pointer(1/5) */
	//MB86A2x_PARAM         myfe ;			/* prepare pointer(2/5) */
	//MB86A2x_TMCC_INFO     tmcc_info ;		/* prepare pointer(4/5) */

	MB86A2x_ERR_MSG       ERR_MSG = MB86A2x_OK;

	//UINT32 loop, cnt ;

	if(MB86A2count==0)
	{

		//Enter device address to the pointer.
		ERR_MSG = MB86A2x_Init_Adr( &m_MB86A2xParam ) ;
		DBG("MB86A2x_Init_Adr %d\n",ERR_MSG);

		//Check Version information.
		ERR_MSG |= MB86A2x_Get_Version( &m_MB86A2xParam.nim_param, &version_info ) ;
		DBG("MB86A2x_Get_Version %d\n",ERR_MSG);
		#ifdef FUJITSU_MSG_PRINT
		FUJITSU_printf_version( &version_info ) ;
		#endif

		//Enter initialization parameters after power on to the pointer.
		ERR_MSG |= MB86A2x_Init_Param( &m_MB86A2xParam ) ;
		DBG("MB86A2x_Init_Param %d\n",ERR_MSG);

		//Initialize channel search result.
		ERR_MSG |= MB86A2x_Init_CH_SCH_Result( &m_MB86A2ch_sch_info ) ;
		//DBG("MB86A2x_Init_CH_SCH_Result %d\n",ERR_MSG);

		//If you want to change bandwidth or standard, you have to change here.
		m_MB86A2xParam.pll_param.BANDSEL = MB86A2x_BANDSEL_ISDBT_6MHz ;

		//Write initialization parameters for channel search.
		//ERR_MSG = MB86A2x_Set_Init( &m_MB86A2xParam, 1, &version_info ) ;
		//ERR_MSG = MB86A2x_Set_Init_CH_SCH( &m_MB86A2xParam, &m_MB86A2ch_sch_info, 1, &version_info ) ;
		ERR_MSG |= MB86A2x_Set_Init( &m_MB86A2xParam, 1, &version_info ) ;
		DBG("MB86A2x_Set_Init_CH_SCH %d\n",ERR_MSG);
		
		//Set reception layer here.
		m_MB86A2xParam.layer_param.RECEP_LAYER = MB86A2x_RECEP_HIGHEST ;

		MB86A2count++;
	}

    return ERR_MSG; //success
}

void MB86A2x_deinit(void)
{
    /* deinit */
	MB86A2count=0;
    return;
}

#if 0
unsigned char MB86A2x_tune(unsigned int frequency_KHz, unsigned int bandwidth_KHz)
{

	MB86A2x_ERR_MSG       ERR_MSG  = MB86A2x_OK;
	int loop = 1 ;
	int cnt = 0 ;
	#if 0
	//Set BER monitor timer parameters and MER monitor parameter.
	m_MB86A2xParam.mon_tim_param.VBER_A_NUM = 0x000fff ;
	m_MB86A2xParam.mon_tim_param.VBER_B_NUM = 0x00ffff ;
	m_MB86A2xParam.mon_tim_param.VBER_C_NUM = 0x00ffff ;
	m_MB86A2xParam.mon_tim_param.SBER_A_NUM = 0x0003ff ;
	m_MB86A2xParam.mon_tim_param.SBER_B_NUM = 0x003fff ;
	m_MB86A2xParam.mon_tim_param.SBER_C_NUM = 0x003fff ;
	m_MB86A2xParam.mon_tim_param.PBER_A_NUM = 0x0003ff ;
	m_MB86A2xParam.mon_tim_param.PBER_B_NUM = 0x003fff ;
	m_MB86A2xParam.mon_tim_param.PBER_C_NUM = 0x003fff ;
	m_MB86A2xParam.mon_tim_param.CN_SYMBOL  = 0x04 ;
	m_MB86A2xParam.mon_tim_param.MER_SYMBOL = 0x04 ;
	//You can also set these values by chaning the values in "MB86A21_***_default_setting.h" instead of above codes.

	//Write the timer parameters.
	ERR_MSG = MB86A2x_Set_Monitor_Timer( &m_MB86A2xParam ) ;

	//Initialize the parameters of LOCKING_INFO.
	ERR_MSG = MB86A2x_Init_LOCKNG_INFO( &m_MB86A2xLockinginfo ) ;

	//Initialize the parameters of SIG_QUAL_INFO.
	ERR_MSG = MB86A2x_Init_SIG_QUAL_INFO( &sign_qual_info ) ;
	#endif
	//Set Channel value. (13-62 for Japan, 7~69 for Brazil)
	//printf("freq: %d", frequency_KHz);
	#ifdef PARAMETER_USE_FREQ
		m_MB86A2xParam.nim_param.tuner_param.freq_kHz = frequency_KHz;
	#else
		if(frequency_KHz<473000)		
			m_MB86A2xParam.nim_param.tuner_param.CH = (frequency_KHz-177000)/6000+7;
		else
			m_MB86A2xParam.nim_param.tuner_param.CH = (frequency_KHz-473000)/6000+14;
	#endif
	//printf(" ch: %d", m_MB86A2xParam.nim_param.tuner_param.CH);
	//Set reception layer here.
	//myfe->layer_param.RECEP_LAYER = recep_layer ;
	#if 0
	//Set mode and guard value for all mods and all guards
	ERR_MSG = MB86A2x_Set_ModeGuard_all( &m_MB86A2xParam ) ;

	//Set timing recovery to default value
	ERR_MSG = MB86A2x_Set_STR_OFS( &(m_MB86A2xParam.nim_param), &(m_MB86A2xParam.pll_param), &(m_MB86A2xParam.recov_param) ) ;

	#endif
	ERR_MSG |= MB86A2x_Set_Channel( &m_MB86A2xParam ) ;
	#if 0
	//Write the data to reset monitor timers.
	ERR_MSG = MB86A2x_Reset_Monitor_Timer( &m_MB86A2xParam ) ;
	#endif
	#if 1
	//Write data to start channel search.
	ERR_MSG |= MB86A2x_Start_CH_SCH( &m_MB86A2xParam.nim_param ) ;
	if( ERR_MSG>0 ) {
		DBG( "I2C error---\n" ) ;
		return 1;
	}
	m_MB86A2ch_sch_info.CH_SCH_status = 0 ;

	while( m_MB86A2ch_sch_info.CH_SCH_status==0 ) {	//check whether the channel seach of first stage has finished or not.
		MB86A2x_wait( m_MB86A2xParam.ch_sch_param.WAIT_CH_SCH_STAT ) ;
		//Read the register value to know whether the channel seach of first stage has finished or not.
		ERR_MSG = MB86A2x_Get_CH_SCH_Status( &m_MB86A2xParam.nim_param, &m_MB86A2ch_sch_info ) ;

		if( ERR_MSG>0 ) {
			DBG( "I2C error-ffff\n" ) ;
			return 1;
		}
	}

	//Write the data to stop the channel search of first stage.
	ERR_MSG = MB86A2x_Reset_IRQ_CH_SCH_END( &m_MB86A2xParam.nim_param ) ;
	
	//Read the regsiter value to check whether OFDM signal was detected or not.
	ERR_MSG |= MB86A2x_Get_CH_SCH_Result( &m_MB86A2xParam.nim_param, &m_MB86A2ch_sch_info ) ;

	if( m_MB86A2ch_sch_info.CH_SCH_result==1 ) {	//Check whether OFDM signal was detected or not in the channel search of first stage.
		while( loop==1 ) {
			MB86A2x_wait( m_MB86A2xParam.ch_sch_param.WAIT_CH_SCH_SEQ ) ;
			//Read the ragister value to check the state value of the sequencer.
			ERR_MSG = MB86A2x_Get_SEQ_value( &m_MB86A2xParam.nim_param, &m_MB86A2xLockinginfo ) ;
			cnt++ ;
			if( cnt>m_MB86A2xParam.ch_sch_param.SCH_LOOP_MAX ) loop = 0 ;
		}

		if( m_MB86A2xLockinginfo.STATE>=8 ) {
			DBG( "ch=%02d Found\n", m_MB86A2xParam.nim_param.tuner_param.CH ) ;
			return 0;
		 }
		 else {
				#ifdef FUJITSU_MSG_PRINT
				FUJITSU_printf_Channel_None( m_MB86A2xParam.nim_param.tuner_param.CH  ) ;
				#endif
				return 1;
		 }
	}
	else {
		m_MB86A2xLockinginfo.STATE=0;
		#ifdef FUJITSU_MSG_PRINT
		FUJITSU_printf_Channel_None( m_MB86A2xParam.nim_param.tuner_param.CH  ) ;
		#endif
		return 1;
	}
	#endif
	
	//return 1;
		
}
#else
unsigned char MB86A2x_tune(unsigned int frequency_KHz, unsigned int bandwidth_KHz)
{

	MB86A2x_ERR_MSG       ERR_MSG = MB86A2x_OK;
	int loop = 1 ;
	int cnt = 0 ;

	//Set BER monitor timer parameters and MER monitor parameter.
	m_MB86A2xParam.mon_tim_param.VBER_A_NUM = 0x000fff ;
	m_MB86A2xParam.mon_tim_param.VBER_B_NUM = 0x00ffff ;
	m_MB86A2xParam.mon_tim_param.VBER_C_NUM = 0x00ffff ;
	m_MB86A2xParam.mon_tim_param.SBER_A_NUM = 0x0003ff ;
	m_MB86A2xParam.mon_tim_param.SBER_B_NUM = 0x003fff ;
	m_MB86A2xParam.mon_tim_param.SBER_C_NUM = 0x003fff ;
	m_MB86A2xParam.mon_tim_param.PBER_A_NUM = 0x0003ff ;
	m_MB86A2xParam.mon_tim_param.PBER_B_NUM = 0x003fff ;
	m_MB86A2xParam.mon_tim_param.PBER_C_NUM = 0x003fff ;
	m_MB86A2xParam.mon_tim_param.CN_SYMBOL  = 0x04 ;
	m_MB86A2xParam.mon_tim_param.MER_SYMBOL = 0x04 ;
	//You can also set these values by chaning the values in "MB86A21_***_default_setting.h" instead of above codes.

	//Write the timer parameters.
	ERR_MSG = MB86A2x_Set_Monitor_Timer( &m_MB86A2xParam ) ;

	//Initialize the parameters of LOCKING_INFO.
	ERR_MSG |= MB86A2x_Init_LOCKNG_INFO( &m_MB86A2xLockinginfo ) ;

	//Initialize the parameters of SIG_QUAL_INFO.
	ERR_MSG |= MB86A2x_Init_SIG_QUAL_INFO( &sign_qual_info ) ;

	//Set Channel value. (13-62 for Japan, 7~69 for Brazil)
	DBG("freq: %d", frequency_KHz);
	#ifdef PARAMETER_USE_FREQ
		m_MB86A2xParam.nim_param.tuner_param.freq_kHz = frequency_KHz;
	#else
		if(frequency_KHz<473000)		
			m_MB86A2xParam.nim_param.tuner_param.CH = (frequency_KHz-177000)/6000+7;
		else
			m_MB86A2xParam.nim_param.tuner_param.CH = (frequency_KHz-473000)/6000+14;
	#endif
	DBG(" ch: %d", m_MB86A2xParam.nim_param.tuner_param.CH);
	//Set reception layer here.
	//myfe->layer_param.RECEP_LAYER = recep_layer ;

	//Set mode and guard value for all mods and all guards
	ERR_MSG |= MB86A2x_Set_ModeGuard_all( &m_MB86A2xParam ) ;

	//Set timing recovery to default value
	ERR_MSG |= MB86A2x_Set_STR_OFS( &(m_MB86A2xParam.nim_param), &(m_MB86A2xParam.pll_param), &(m_MB86A2xParam.recov_param) ) ;


	ERR_MSG |= MB86A2x_Set_Channel( &m_MB86A2xParam ) ;

	//Write the data to reset monitor timers.
	ERR_MSG |= MB86A2x_Reset_Monitor_Timer( &m_MB86A2xParam ) ;

	#if 0
	//Write data to start channel search.
	ERR_MSG = MB86A2x_Start_CH_SCH( &m_MB86A2xParam.nim_param ) ;
	if( ERR_MSG>0 ) {
		DBG( "I2C error---\n" ) ;
		return 1;
	}
	m_MB86A2ch_sch_info.CH_SCH_status = 0 ;

	while( m_MB86A2ch_sch_info.CH_SCH_status==0 ) {	//check whether the channel seach of first stage has finished or not.
		MB86A2x_wait( m_MB86A2xParam.ch_sch_param.WAIT_CH_SCH_STAT ) ;
		//Read the register value to know whether the channel seach of first stage has finished or not.
		ERR_MSG = MB86A2x_Get_CH_SCH_Status( &m_MB86A2xParam.nim_param, &m_MB86A2ch_sch_info ) ;

		if( ERR_MSG>0 ) {
			DBG( "I2C error-ffff\n" ) ;
			return 1;
		}
	}

	//Write the data to stop the channel search of first stage.
	ERR_MSG = MB86A2x_Reset_IRQ_CH_SCH_END( &m_MB86A2xParam.nim_param ) ;
	
	//Read the regsiter value to check whether OFDM signal was detected or not.
	ERR_MSG = MB86A2x_Get_CH_SCH_Result( &m_MB86A2xParam.nim_param, &m_MB86A2ch_sch_info ) ;

	if( m_MB86A2ch_sch_info.CH_SCH_result==1 ) {	//Check whether OFDM signal was detected or not in the channel search of first stage.
		while( loop==1 ) {
			MB86A2x_wait( m_MB86A2xParam.ch_sch_param.WAIT_CH_SCH_SEQ ) ;
			//Read the ragister value to check the state value of the sequencer.
			ERR_MSG = MB86A2x_Get_SEQ_value( &m_MB86A2xParam.nim_param, &m_MB86A2xLockinginfo ) ;
			cnt++ ;
			if( cnt>m_MB86A2xParam.ch_sch_param.SCH_LOOP_MAX ) loop = 0 ;
		}

		if( m_MB86A2xLockinginfo.STATE>=8 ) {
			DBG( "ch=%02d Found\n", m_MB86A2xParam.nim_param.tuner_param.CH ) ;
			return 0;
		 }
		 else {
				FUJITSU_printf_Channel_None( m_MB86A2xParam.nim_param.tuner_param.CH  ) ;
				return 1;
		 }
	}
	else {
		m_MB86A2xLockinginfo.STATE=0;
		FUJITSU_printf_Channel_None( m_MB86A2xParam.nim_param.tuner_param.CH  ) ;
		return 1;
	}
	#endif
	
	return ERR_MSG;
		
}
#endif

char MB86A2x_lockstatus(void)
{	
	if(m_MB86A2xLockinginfo.STATE>=8)
	{
		DBG("MB86A2x_lockstatus is 1\n");
		return 1;		
	}
    return 0;
}

//! PID filter control
/**
 * Enables or disabled the internal PID filter of a demod. If it is disabled,
 * the complete MPEG2 Transport is put out otherwise only selected PIDs are
 * passed through (see demod_pid_filter).
 *
 * \param onoff 0 = disable PID filter (default), 1 = enable
 */
int  MB86A2x_pid_filter_ctrl(UINT8 onoff)
{
    return 1;
}

//! PID filter table
/**
 * Enable a PID to be filtered. Disable the consideration of a position of the internal table.
 *
 * \param id          index of the internal table
 * \param pid        PID to be filtered
 * \param onoff     activate de-activate the given table index
 */
int MB86A2x_pid_filter(UINT8 id, UINT16 pid, UINT8 onoff)
{
    return 1;
}

void MB86A2x_signal_strength_quality(UINT8* signal_quality, UINT8* signal_strength)
{
	//*signal_strength =100; //MB86A2x_Get_Signal_Strength();
	//*signal_quality =100; //MB86A2x_Get_Signal_Quality();
	//int loop=1,cnt=0;
	MB86A2x_ERR_MSG ERR_MSG;
	
    ERR_MSG = MB86A2x_Get_SEQ_value( &m_MB86A2xParam.nim_param, &m_MB86A2xLockinginfo );
    if(ERR_MSG != MB86A2x_OK)
    {
        DBG( "MB86A2x_Get_SEQ_value ERR\n" ) ;
        return;
    }
    else
    {
    	//printk( "MB86A2x_Get_SEQ_value state:%d\n", m_MB86A2xLockinginfo.STATE) ;
        if(m_MB86A2xLockinginfo.STATE < 8)
        {
            *signal_quality = 0;
            return;
        }
     }

    ERR_MSG = MB86A2x_Get_CN_Flag( &m_MB86A2xParam.nim_param, &sign_qual_info);
    if( ERR_MSG != MB86A2x_OK )
    {
        DBG( "MB86A2x_Get_CN_Flag ERR\n" ) ;
        return;
    }
    else
    {
        if( sign_qual_info.CN_update == 1 )
        {
            //Check whether whether C/N measurement is finished or not by checking the value of CN_update.
            //Read register values to know C/N value and calculate estimated C/N.
            ERR_MSG = MB86A2x_Get_CN_Value( &m_MB86A2xParam.nim_param, &sign_qual_info);
            if( ERR_MSG != MB86A2x_OK )
            {

                DBG( "MB86A2x_Get_CN_Value ERR\n" ) ;
                return;
            }

            //Reset C/N counter
            ERR_MSG = MB86A2x_Reset_CN_Counter( &m_MB86A2xParam.nim_param, &m_MB86A2xParam.mon_tim_param );
            if( ERR_MSG != MB86A2x_OK )
            {
                DBG( "MB86A2x_Reset_CN_Counter ERR\n" ) ;
                return;
            }
        }
        //printk("MB86A2x_Get_Signal_Quality: %d\n", sign_qual_info.CN_MON);
        *signal_quality = (UINT8)(sign_qual_info.CN_MON);
    }

///strong
/*
    ERR_MSG = MB86A2x_Get_SEQ_value( &m_MB86A2xParam.nim_param, &m_MB86A2xLockinginfo );
    if(ERR_MSG != MB86A2x_OK)
    {
        printk( "MB86A2x_Get_SEQ_value ERR\n" ) ;
        return ;
    }
    else
    {
        if(m_MB86A2xLockinginfo.STATE < 8)
        {
            *signal_strength = 0;
            return;
        }
     }

    ERR_MSG = MB86A2x_Get_IF_level( &m_MB86A2xParam.nim_param, &agc_info);
    if( ERR_MSG != MB86A2x_OK )
    {
        printk( "MB86A2x_Get_IF_level ERR\n" ) ;
        return;
    }
		printk("MB86A2x_Get_Signal_Strength: %d\n", agc_info.IF_INPUT_LEVEL);
    *signal_strength = agc_info.IF_INPUT_LEVEL * 256;
  */
  	RDATVSingalStrength(signal_strength);
	//printk("MB86A2x_Get_Signal_Strength: %d\n", *signal_strength);
    return;	
}

//! i2c chip address
/**
 * MTV chip ID is two as 0x86 and 0xDO corresponding with TMODEx pin configuration
 * TMODE(TMOD2 = 0, TMODE1 = 0, TMODE0 = 0), i2c address: 0x86
 * TMODE(TMOD2 = 0, TMODE1 = 1, TMODE0 = 1), i2c address: 0xd0
 */
int MB86A2x_i2c_addr(void)
{
    return MB86A22_I2C_ADDR;
}

//! i2c speed
/**
 * maximum 400khz in fast-mode
 * 100 khz in standard-mode
 */
unsigned short MB86A2x_i2c_speed(void)
{
    return 100;
}

char MB86A2x_null(void)
{
    return 1;
}

#if 0
/*tstv func*/
struct TSTV_Module_t gTSTVModule = {
    "MB86A2x_CONTROL",
    MB86A2x_init,
    MB86A2x_deinit,
    MB86A2x_tune,
    MB86A2x_lockstatus,
    MB86A2x_pid_filter,
    MB86A2x_pid_filter_ctrl,
    MB86A2x_signal_strength_quality,
    MB86A2x_i2c_addr,
    MB86A2x_i2c_speed,
    MB86A2x_init 
};
#endif

/********************************************************************************************/
//
//
//    OVER
//
//
/********************************************************************************************/
