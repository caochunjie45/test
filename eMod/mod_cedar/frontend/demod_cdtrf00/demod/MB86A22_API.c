/****************************************************************************
*
*  Front-end device driver for Fujitsu MB86A22 ISDB-T frontend.
*
*  Copyright (c) 2010 Fujitsu Semiconductor 
*  All rights reserved. 
*
*  Created 12/14/2010
*
****************************************************************************/
//#include <stdio.h>
#include "MB86A22_API_version.h"
#include "MB86A22_API_defs.h"
        
#include "../tuner/Tuner.h"
#include "MB86A22_API_structs.h"
#include "MB86A22_RDA_default_setting.h"
#include "MB86A22_user_define.h"
#include "MB86A22_API.h"
#include "Fujitsu_printf.h"
     
#include "MB86A22_BER_CN_Calculation.h"

#if 0
MB86A2x_ERR_MSG MB86A2x_Calc_MASTER_CLK( MB86A2x_PLL_PARAM *pll_param, RDA5880_TunerConfigS *tuner_param );
MB86A2x_ERR_MSG MB86A2x_Set_STR_OFS( MB86A2x_NIM_PARAM *nim_param, MB86A2x_PLL_PARAM *pll_param, MB86A2x_RECOV_PARAM *recov_param );
MB86A2x_ERR_MSG MB86A2x_Set_CRR_OFS( MB86A2x_NIM_PARAM *nim_param, MB86A2x_PLL_PARAM *pll_param, MB86A2x_RECOV_PARAM *recov_param );
MB86A2x_ERR_MSG MB86A2x_Set_WIDE_FREQ_DET( MB86A2x_NIM_PARAM *nim_param, MB86A2x_PLL_PARAM *pll_param );
MB86A2x_ERR_MSG MB86A2x_Set_AGC(  MB86A2x_NIM_PARAM *nim_param, MB86A2x_AGC_PARAM * agc_param );
MB86A2x_ERR_MSG MB86A2x_Set_IFAGC( MB86A2x_NIM_PARAM *nim_param, MB86A2x_IFAGC_PARAM *ifagc_param );
MB86A2x_ERR_MSG MB86A2x_Set_Opt_Param( MB86A2x_NIM_PARAM *nim_param, MB86A2x_TU_DEP_PARAM *tun_dep_param, MB86A2x_VERSION_INFO *version_info );
MB86A2x_ERR_MSG MB86A2x_Set_TS_Output( MB86A2x_NIM_PARAM *nim_param, MB86A2x_TSOUT_PARAM *tsout_param, int RS_EN );
UINT32 MB86A2x_calc_symbol_length_us( MB86A2x_BANDSEL BANDSEL, UINT8 MODE, UINT8 GUARD );
UINT32 MB86A2x_calc_carrier_num( MB86A2x_BANDSEL BANDSEL, UINT8 MODE );
UINT32 MB86A2x_calc_symbol_bit( UINT8 modulation );
UINT32 MB86A2x_calc_data_ratio_1000( UINT8 code_rate );
UINT32 MB86A2x_calc_TSP_num( UINT8 mode, UINT8 modulation, UINT8 code_rate );
MB86A2x_ERR_MSG MB86A2x_Set_RF( MB86A2x_NIM_PARAM *nim_param, MB86A2x_PLL_PARAM *pll_param );
MB86A2x_ERR_MSG MB86A2x_Set_IQ( MB86A2x_NIM_PARAM *nim_param );
MB86A2x_ERR_MSG MB86A2x_Set_RECEP_LAYER( MB86A2x_NIM_PARAM *nim_param, MB86A2x_LAYER_PARAM *recep_layer );
MB86A2x_ERR_MSG MB86A2x_Set_VBER_Timer( MB86A2x_NIM_PARAM *nim_param, MB86A2x_MON_TIM_PARAM *mon_tim_param );
MB86A2x_ERR_MSG MB86A2x_Set_SBER_Timer( MB86A2x_NIM_PARAM *nim_param, MB86A2x_MON_TIM_PARAM *mon_tim_param );
MB86A2x_ERR_MSG MB86A2x_Set_PBER_Timer( MB86A2x_NIM_PARAM *nim_param, MB86A2x_MON_TIM_PARAM *mon_tim_param );
MB86A2x_ERR_MSG MB86A2x_Set_CN_Timer( MB86A2x_NIM_PARAM *nim_param, MB86A2x_MON_TIM_PARAM *mon_tim_param );
MB86A2x_ERR_MSG MB86A2x_Set_MER_Timer( MB86A2x_NIM_PARAM *nim_param, MB86A2x_MON_TIM_PARAM *mon_tim_param );
MB86A2x_ERR_MSG MB86A2x_Get_TMCC_Sys_Emerg( MB86A2x_NIM_PARAM *nim_param, MB86A2x_TMCC_INFO *tmcc_info );
MB86A2x_ERR_MSG MB86A2x_Get_TMCC_Part_Recep( MB86A2x_NIM_PARAM *nim_param, MB86A2x_TMCC_INFO *tmcc_info );
MB86A2x_ERR_MSG MB86A2x_Get_TMCC_Mod_Lay_A( MB86A2x_NIM_PARAM *nim_param, MB86A2x_TMCC_INFO *tmcc_info );
MB86A2x_ERR_MSG MB86A2x_Get_TMCC_Mod_Lay_B( MB86A2x_NIM_PARAM *nim_param, MB86A2x_TMCC_INFO *tmcc_info );
MB86A2x_ERR_MSG MB86A2x_Get_TMCC_Mod_Lay_C( MB86A2x_NIM_PARAM *nim_param, MB86A2x_TMCC_INFO *tmcc_info );
MB86A2x_ERR_MSG MB86A2x_Get_TMCC_Rate_Lay_A( MB86A2x_NIM_PARAM *nim_param, MB86A2x_TMCC_INFO *tmcc_info );
MB86A2x_ERR_MSG MB86A2x_Get_TMCC_Rate_Lay_B( MB86A2x_NIM_PARAM *nim_param, MB86A2x_TMCC_INFO *tmcc_info );
MB86A2x_ERR_MSG MB86A2x_Get_TMCC_Rate_Lay_C( MB86A2x_NIM_PARAM *nim_param, MB86A2x_TMCC_INFO *tmcc_info );
MB86A2x_ERR_MSG MB86A2x_Get_TMCC_IL_Lay_A( MB86A2x_NIM_PARAM *nim_param, MB86A2x_TMCC_INFO *tmcc_info );
MB86A2x_ERR_MSG MB86A2x_Get_TMCC_IL_Lay_B( MB86A2x_NIM_PARAM *nim_param, MB86A2x_TMCC_INFO *tmcc_info );
MB86A2x_ERR_MSG MB86A2x_Get_TMCC_IL_Lay_C( MB86A2x_NIM_PARAM *nim_param, MB86A2x_TMCC_INFO *tmcc_info );
MB86A2x_ERR_MSG MB86A2x_Get_TMCC_Num_Lay_A( MB86A2x_NIM_PARAM *nim_param, MB86A2x_TMCC_INFO *tmcc_info );
MB86A2x_ERR_MSG MB86A2x_Get_TMCC_Num_Lay_B( MB86A2x_NIM_PARAM *nim_param, MB86A2x_TMCC_INFO *tmcc_info );
MB86A2x_ERR_MSG MB86A2x_Get_TMCC_Num_Lay_C( MB86A2x_NIM_PARAM *nim_param, MB86A2x_TMCC_INFO *tmcc_info );
MB86A2x_ERR_MSG MB86A2x_Reset_VBER_ALL_Counter( MB86A2x_NIM_PARAM *nim_param );
MB86A2x_ERR_MSG MB86A2x_Reset_SBER_ALL_Counter( MB86A2x_NIM_PARAM *nim_param );
MB86A2x_ERR_MSG MB86A2x_Reset_PBER_ALL_Counter( MB86A2x_NIM_PARAM *nim_param );
MB86A2x_ERR_MSG MB86A2x_Reset_CN_Counter( MB86A2x_NIM_PARAM *nim_param, MB86A2x_MON_TIM_PARAM *mon_tim_param );
MB86A2x_ERR_MSG MB86A2x_Reset_MER_Counter( MB86A2x_NIM_PARAM *nim_param );	
MB86A2x_ERR_MSG MB86A2x_Set_Power_Mode( MB86A2x_NIM_PARAM *nim_param, MB86A2x_AGC_PARAM *agc_param );
MB86A2x_ERR_MSG MB86A2x_Set_Low_Power( MB86A2x_NIM_PARAM *nim_param );
MB86A2x_ERR_MSG MB86A2x_Set_Normal_Power( MB86A2x_NIM_PARAM *nim_param, MB86A2x_AGC_PARAM *agc_param );
MB86A2x_ERR_MSG MB86A2x_Set_IRQ_All_Off( MB86A2x_NIM_PARAM *nim_param );
MB86A2x_ERR_MSG MB86A2x_Set_IRQ_TMCC_EMG_INV( MB86A2x_NIM_PARAM *nim_param, UINT8 inv );
MB86A2x_ERR_MSG MB86A2x_Set_IRQ_CH_SCH_END_Off( MB86A2x_NIM_PARAM *nim_param, UINT8 disable );
MB86A2x_ERR_MSG MB86A2x_Get_IRQ_CH_SCH_END( MB86A2x_NIM_PARAM *nim_param, UINT8 *irq_ch_sch_end );
#endif
	
MB86A2x_ERR_MSG MB86A2x_Init_Adr( MB86A2x_PARAM *param )
{
	if(      MB86A22_I2C_ADDR==0x20 ) param->nim_param.MB86A2x_Adr = MB86A2x_DEV_ADR_20 ;
	else if( MB86A22_I2C_ADDR==0x22 ) param->nim_param.MB86A2x_Adr = MB86A2x_DEV_ADR_22 ;
	else if( MB86A22_I2C_ADDR==0x24 ) param->nim_param.MB86A2x_Adr = MB86A2x_DEV_ADR_24 ;
	else                              param->nim_param.MB86A2x_Adr = MB86A2x_DEV_ADR_26 ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Get_Version( MB86A2x_NIM_PARAM *nim_param, MB86A2x_VERSION_INFO *version_info )
{
	UINT32 Status ;
	UINT8  read_val ;

	version_info->DRV_VERSION = MB86A2x_DRIVER_VERSION ;

	Status = MB86A2x_read( (UINT8)(nim_param->MB86A2x_Adr), 0x00, &read_val ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	version_info->LSI_VERSION = read_val ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Get_DRV_Version( MB86A2x_VERSION_INFO *version_info )
{

	version_info->DRV_VERSION = MB86A2x_DRIVER_VERSION ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Get_LSI_Version( MB86A2x_NIM_PARAM *nim_param, MB86A2x_VERSION_INFO *version_info )
{
	UINT32 Status ;
	UINT8  read_val ;

	Status = MB86A2x_read( (UINT8)(nim_param->MB86A2x_Adr), 0x00, &read_val ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	version_info->LSI_VERSION = read_val ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Init_Param( MB86A2x_PARAM *param )
{
	param->nim_param.tuner_param.Xtal_freq_kHz  = RF_Xtal_freq_default ;
	param->nim_param.tuner_param.IF_freq_kHz    = RDATV_IF_FREQ ;	//defined in "rda5880h_defs.h"
	param->nim_param.tuner_param.CH_UNIT        = MB86A2x_CH_UNIT_DEFAULT ;
	param->nim_param.tuner_param.CH             = MB86A2x_CH_DEFAULT ;

	//MB86A22 Setting
	param->nim_param.IQ_pol                  = IQ_pol_DEFAULT ;
	param->nim_param.POWER_DOWN              = POWER_DOWN_DEFAULT ;
	param->nim_param.RF_CONT_EN              = RF_CONT_EN_DEFAULT ;

	param->pll_param.INPUT_CLK_FREQ_kHz      = INPUT_CLK_FREQ_kHz_DEFAULT ;
	param->pll_param.REFSEL                  = REFSEL_DEFAULT ;
	param->pll_param.PLLPASS                 = PLLPASS_DEFAULT ;
	param->pll_param.BANDSEL                 = BANDSEL_DEFAULT ;
	param->pll_param.FREQSEL                 = FREQSEL_DEFAULT ;

	param->layer_param.RECEP_LAYER           = RECEP_LAYER_DEFAULT ;

	param->agc_param.MAIF                    = MAIF_DEFAULT ;
	param->agc_param.IFAGC_POL               = IFAGC_POL_DEFAULT ;
	param->agc_param.RFAGC_ON                = RFAGC_ON_DEFAULT ;

	param->ifagc_param.VIFREF                = VIFREF_DEFAULT ;
	param->ifagc_param.AGAIN                 = AGAIN_DEFAULT ;
	param->ifagc_param.DTS                   = DTS_DEFAULT ;
	param->ifagc_param.IFA                   = IFA_DEFAULT ;
	param->ifagc_param.IFB                   = IFB_DEFAULT ;
	param->ifagc_param.IFAGCO                = IFAGCO_DEFAULT ;
	param->ifagc_param.MAXIFAGC              = MAXIFAGC_DEFAULT ;
	param->ifagc_param.IFSAMPLE              = IFSAMPLE_DEFAULT ;
	param->ifagc_param.OUTSAMPLE             = OUTSAMPLE_DEFAULT ;
	param->ifagc_param.VMAIF                 = VMAIF_DEFAULT ;

	param->rfagc_param.VMARF                 = VMARF_DEFAULT ;

	param->tun_dep_param.S1CNT               = S1CNT_DEFAULT ;

	param->tsout_param.TS_OUT_SEL            = TS_OUT_SEL_DEFAULT ;
	param->tsout_param.TS_LAYER              = TS_LAYER_DEFAULT ;
	param->tsout_param.TS_CLK                = TS_CLK_DEFALT ;
	param->tsout_param.TS_ERR_MASK           = TS_ERR_MASK_DEFAULT ;
	param->tsout_param.TS_PARITY             = TS_PARITY_DEFAULT ;
	param->tsout_param.TS_SYNC               = TS_SYNC_DEFAULT ;

	param->mon_tim_param.VBER_A_NUM          = VBER_A_NUM_DEFAULT ;
	param->mon_tim_param.VBER_B_NUM          = VBER_B_NUM_DEFAULT ;
	param->mon_tim_param.VBER_C_NUM          = VBER_C_NUM_DEFAULT ;
	param->mon_tim_param.SBER_A_NUM          = SBER_A_NUM_DEFAULT ;
	param->mon_tim_param.SBER_B_NUM          = SBER_B_NUM_DEFAULT ;
	param->mon_tim_param.SBER_C_NUM          = SBER_C_NUM_DEFAULT ;
	param->mon_tim_param.PBER_A_NUM          = PBER_A_NUM_DEFAULT ;
	param->mon_tim_param.PBER_B_NUM          = PBER_B_NUM_DEFAULT ;
	param->mon_tim_param.PBER_C_NUM          = PBER_C_NUM_DEFAULT ;
	param->mon_tim_param.CN_SYMBOL           = CN_SYMBOL_DEFAULT ;
	param->mon_tim_param.MER_SYMBOL          = MER_SYMBOL_DEFAULT ;

	param->ch_sch_param.START_CH             = START_CH_DEFAULT ;
	param->ch_sch_param.STOP_CH              = STOP_CH_DEFAULT ;
	param->ch_sch_param.WAIT_CH_SCH_STAT     = WAIT_CH_SCH_STAT_DEFAULT ;
	param->ch_sch_param.WAIT_CH_SCH_SEQ      = WAIT_CH_SCH_SEQ_DEFAULT ;
	param->ch_sch_param.SCH_LOOP_MAX         = SCH_LOOP_MAX_DEFAULT ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Set_Init( MB86A2x_PARAM *param, int RS_EN, MB86A2x_VERSION_INFO *version_info )
{
	UINT32 Status ;
	MB86A2x_ERR_MSG ERR_MSG = I2C_OK;

	Status = MB86A2x_write( (UINT8)(param->nim_param.MB86A2x_Adr), 0x70, 0x00 ) ;	//execute I2C Reg reset
	Status |= MB86A2x_write( (UINT8)(param->nim_param.MB86A2x_Adr), 0x70, 0xff ) ;	//release I2C Reg reset
	Status |= MB86A2x_write( (UINT8)(param->nim_param.MB86A2x_Adr), 0x08, 0x01 ) ;	//sequencer stop
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	ERR_MSG = MB86A2x_Set_Power_Mode( &param->nim_param, &param->agc_param ) ;	//Power Down Setting
	if( ERR_MSG==MB86A2x_ERR_INIT ) return MB86A2x_ERR_INIT ;

	Status = MB86A2x_write( (UINT8)(param->nim_param.MB86A2x_Adr), 0x06, 0xff ) ;	//Set all mode detect(1/2)
	Status |= MB86A2x_write( (UINT8)(param->nim_param.MB86A2x_Adr), 0x1f, 0x0f ) ;	//Set all mode detect(1/2)


	if (param->pll_param.BANDSEL == MB86A2x_BANDSEL_ISDBT_6MHz || param->pll_param.BANDSEL == MB86A2x_BANDSEL_ISDBT_7MHz || param->pll_param.BANDSEL == MB86A2x_BANDSEL_ISDBT_8MHz){  // ISDB-T 6,7,8MHz
		if (param->pll_param.BANDSEL == MB86A2x_BANDSEL_ISDBT_6MHz){  // ISDB-T 6MHz
			Status |= MB86A2x_write( (UINT8)(param->nim_param.MB86A2x_Adr), 0x71, (UINT8)(0x00|((param->pll_param.FREQSEL)<<3)) ) ;	
		}
		else if (param->pll_param.BANDSEL == MB86A2x_BANDSEL_ISDBT_7MHz){  // ISDB-T 7MHz
			Status |= MB86A2x_write( (UINT8)(param->nim_param.MB86A2x_Adr), 0x71, 0x20 ) ;	
		}
		else if (param->pll_param.BANDSEL == MB86A2x_BANDSEL_ISDBT_8MHz){  // ISDB-T 8MHz
			Status |= MB86A2x_write( (UINT8)(param->nim_param.MB86A2x_Adr), 0x71, 0x40 ) ;	
		}
	}
	else if (param->pll_param.BANDSEL == MB86A2x_BANDSEL_DVBT_6MHz || param->pll_param.BANDSEL == MB86A2x_BANDSEL_DVBT_7MHz || param->pll_param.BANDSEL == MB86A2x_BANDSEL_DVBT_8MHz ){ // DVB-T 6MHz, DVB-T 7MHz, DVB-T 8MHz
		if (param->pll_param.BANDSEL == MB86A2x_BANDSEL_DVBT_6MHz){  // DVB-T 6MHz
			Status |= MB86A2x_write( (UINT8)(param->nim_param.MB86A2x_Adr), 0x71, 0x61 ) ;	
		}
		else if (param->pll_param.BANDSEL == MB86A2x_BANDSEL_DVBT_7MHz){  // DVB-T 7MHz
			Status |= MB86A2x_write( (UINT8)(param->nim_param.MB86A2x_Adr), 0x71, 0x01 ) ;	
		}
		else if (param->pll_param.BANDSEL == MB86A2x_BANDSEL_DVBT_8MHz){  // DVB-T 8MHz
			Status |= MB86A2x_write( (UINT8)(param->nim_param.MB86A2x_Adr), 0x71, 0x21 ) ;	
		}
	}

	Status |= MB86A2x_write( (UINT8)(param->nim_param.MB86A2x_Adr), 0x28, 0x29 ) ;   // Manual Setting for Clock Recovery Offset
	Status |= MB86A2x_write( (UINT8)(param->nim_param.MB86A2x_Adr), 0x2b, 0x25 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	Status = MB86A2x_write( (UINT8)(param->nim_param.MB86A2x_Adr), 0x28, 0x1f ) ;   // Manual Setting for Wide Carrier Offset
	Status |= MB86A2x_write( (UINT8)(param->nim_param.MB86A2x_Adr), 0x2b, 0x04 ) ;

	if( param->nim_param.tuner_param.IF_freq_kHz==MB86A2x_IF_FREQ_00_000kHz ) {
		Status |= MB86A2x_write( (UINT8)(param->nim_param.MB86A2x_Adr), 0x1c, 0x02 ) ;	//Set IF input mode=IQ
	}
	else if( param->nim_param.tuner_param.IF_freq_kHz==MB86A2x_IF_FREQ_03_300kHz ) {
		Status |= MB86A2x_write( (UINT8)(param->nim_param.MB86A2x_Adr), 0x1c, 0x01 ) ;	//Set IF input mode=IF
	}
	else if( param->nim_param.tuner_param.IF_freq_kHz==MB86A2x_IF_FREQ_03_650kHz ) {
		Status |= MB86A2x_write( (UINT8)(param->nim_param.MB86A2x_Adr), 0x1c, 0x01 ) ;	//Set IF input mode=IF
	}
	else if( param->nim_param.tuner_param.IF_freq_kHz==MB86A2x_IF_FREQ_04_000kHz ) {
		Status |= MB86A2x_write( (UINT8)(param->nim_param.MB86A2x_Adr), 0x1c, 0x01 ) ;	//Set IF input mode=IF
	}
	else if( param->nim_param.tuner_param.IF_freq_kHz==MB86A2x_IF_FREQ_04_500kHz ) {
		Status |= MB86A2x_write( (UINT8)(param->nim_param.MB86A2x_Adr), 0x1c, 0x01 ) ;	//Set IF input mode=IF
	}
	else if( param->nim_param.tuner_param.IF_freq_kHz==MB86A2x_IF_FREQ_05_000kHz ) {
		Status |= MB86A2x_write( (UINT8)(param->nim_param.MB86A2x_Adr), 0x1c, 0x01 ) ;	//Set IF input mode=IF
	}
	else if( param->nim_param.tuner_param.IF_freq_kHz==MB86A2x_IF_FREQ_57_000kHz ) {
		Status |= MB86A2x_write( (UINT8)(param->nim_param.MB86A2x_Adr), 0x1c, 0x01 ) ;	//Set IF input mode=IF
	}
	else if( param->nim_param.tuner_param.IF_freq_kHz==MB86A2x_IF_FREQ_44_000kHz ) {
		Status |= MB86A2x_write( (UINT8)(param->nim_param.MB86A2x_Adr), 0x1c, 0x11 ) ;	//Set IF input mode=IF
	}
	else if( param->nim_param.tuner_param.IF_freq_kHz==MB86A2x_IF_FREQ_36_000kHz ) {
		Status |= MB86A2x_write( (UINT8)(param->nim_param.MB86A2x_Adr), 0x1c, 0x21 ) ;	//Set IF input mode=IF
	}
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	
	ERR_MSG = MB86A2x_Calc_MASTER_CLK( &param->pll_param, &param->nim_param.tuner_param ) ;
	if( ERR_MSG==MB86A2x_ERR_INIT ) return MB86A2x_ERR_INIT ;

	ERR_MSG = MB86A2x_Set_STR_OFS( &param->nim_param, &param->pll_param, &param->recov_param ) ;
	if( ERR_MSG==MB86A2x_ERR_INIT ) return MB86A2x_ERR_INIT ;

	ERR_MSG = MB86A2x_Set_CRR_OFS( &param->nim_param, &param->pll_param, &param->recov_param ) ;
	if( ERR_MSG==MB86A2x_ERR_INIT ) return MB86A2x_ERR_INIT ;

	ERR_MSG = MB86A2x_Set_WIDE_FREQ_DET( &param->nim_param, &param->pll_param ) ;
	if( ERR_MSG==MB86A2x_ERR_INIT ) return MB86A2x_ERR_INIT ;

	ERR_MSG = MB86A2x_Set_AGC( &param->nim_param, &param->agc_param ) ;
	if( ERR_MSG==MB86A2x_ERR_INIT ) return MB86A2x_ERR_INIT ;

	ERR_MSG = MB86A2x_Set_IFAGC( &param->nim_param, &param->ifagc_param ) ;
	if( ERR_MSG==MB86A2x_ERR_INIT ) return MB86A2x_ERR_INIT ;

	ERR_MSG = MB86A2x_Set_TS_Output( &param->nim_param, &param->tsout_param, RS_EN ) ;
	if( ERR_MSG==MB86A2x_ERR_INIT ) return MB86A2x_ERR_INIT ;

	ERR_MSG = MB86A2x_Set_Opt_Param( &param->nim_param, &param->tun_dep_param, version_info ) ;
	if( ERR_MSG==MB86A2x_ERR_INIT ) return MB86A2x_ERR_INIT ;

	Status = MB86A2x_write( (UINT8)(param->nim_param.MB86A2x_Adr), 0x52, 0x01 ) ;	//Pre Viterbi BER Switch On
	Status |= MB86A2x_write( (UINT8)(param->nim_param.MB86A2x_Adr), 0x53, 0x00 ) ;
	Status |= MB86A2x_write( (UINT8)(param->nim_param.MB86A2x_Adr), 0x53, 0x07 ) ;

	Status |= MB86A2x_write( (UINT8)(param->nim_param.MB86A2x_Adr), 0x5e, 0x07 ) ;	//Post Viterbi BER Switch On
	Status |= MB86A2x_write( (UINT8)(param->nim_param.MB86A2x_Adr), 0x5f, 0x00 ) ;
	Status |= MB86A2x_write( (UINT8)(param->nim_param.MB86A2x_Adr), 0x5f, 0x07 ) ;

	Status |= MB86A2x_write( (UINT8)(param->nim_param.MB86A2x_Adr), 0x48, 0x04 ) ;	//set C/N Monitor mode to manual(modified from version 4)

	Status |= MB86A2x_write( (UINT8)(param->nim_param.MB86A2x_Adr), 0x50, 0x50 ) ;	//set MER Monitor mode to manual(1/2)
	Status |= MB86A2x_write( (UINT8)(param->nim_param.MB86A2x_Adr), 0x51, 0x02 ) ;	//set MER Monitor mode to manual(2/2)

	Status |= MB86A2x_write( (UINT8)(param->nim_param.MB86A2x_Adr), 0x50, 0xb0 ) ;	//Post RS BER Switch On(1/2)
	Status |= MB86A2x_write( (UINT8)(param->nim_param.MB86A2x_Adr), 0x51, 0x07 ) ;	//Post RS BER Switch On(1/2)
	Status |= MB86A2x_write( (UINT8)(param->nim_param.MB86A2x_Adr), 0x50, 0xb1 ) ;
	Status |= MB86A2x_write( (UINT8)(param->nim_param.MB86A2x_Adr), 0x51, 0x07 ) ;
	Status |= MB86A2x_write( (UINT8)(param->nim_param.MB86A2x_Adr), 0x51, 0x00 ) ;

	//added from ver5
	Status |= MB86A2x_write( (UINT8)(param->nim_param.MB86A2x_Adr), 0x6d, 0xfb ) ;	//Accept Layer-A > Layer-B
	Status |= MB86A2x_write( (UINT8)(param->nim_param.MB86A2x_Adr), 0x6e, 0xbf ) ;	//Accept Layer-A > Layer-B
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	ERR_MSG = MB86A2x_Set_IRQ_All_Off( &param->nim_param ) ;
	if( ERR_MSG==MB86A2x_ERR_INIT ) return MB86A2x_ERR_INIT ;

	Status = RDA_TUNER_INIT(&param->nim_param.tuner_param) ;
	if( Status==TUNER_NG ) return MB86A2x_ERR_RFTUNE ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Set_ModeGuard_fixed( MB86A2x_PARAM *param, MB86A2x_CH_SCH_INFO *ch_sch_info, UINT8 ch_num )
{
	UINT32 Status ;
	UINT32 MG ;

	if( ch_sch_info->MODE[ch_num]==0 ) {	//MODE1
		if(      ch_sch_info->GUARD[ch_num]==0 ) MG = 0x001 ;	//MODE1, GI=1/4
		else if( ch_sch_info->GUARD[ch_num]==1 ) MG = 0x002 ;	//MODE1, GI=1/8
		else if( ch_sch_info->GUARD[ch_num]==2 ) MG = 0x004 ;	//MODE1, GI=1/16
		else if( ch_sch_info->GUARD[ch_num]==3 ) MG = 0x008 ;	//MODE1, GI=1/32
	}
	else if( ch_sch_info->MODE[ch_num]==1 ) {	//MODE2
		if(      ch_sch_info->GUARD[ch_num]==0 ) MG = 0x010 ;	//MODE2, GI=1/4
		else if( ch_sch_info->GUARD[ch_num]==1 ) MG = 0x020 ;	//MODE2, GI=1/8
		else if( ch_sch_info->GUARD[ch_num]==2 ) MG = 0x040 ;	//MODE2, GI=1/16
		else if( ch_sch_info->GUARD[ch_num]==3 ) MG = 0x080 ;	//MODE2, GI=1/32
	}
	else if( ch_sch_info->MODE[ch_num]==2 ) {	//MODE3
		if(      ch_sch_info->GUARD[ch_num]==0 ) MG = 0x100 ;	//MODE3, GI=1/4
		else if( ch_sch_info->GUARD[ch_num]==1 ) MG = 0x200 ;	//MODE3, GI=1/8
		else if( ch_sch_info->GUARD[ch_num]==2 ) MG = 0x400 ;	//MODE3, GI=1/16
		else if( ch_sch_info->GUARD[ch_num]==3 ) MG = 0x800 ;	//MODE3, GI=1/32
	}

	Status = MB86A2x_write( (UINT8)(param->nim_param.MB86A2x_Adr), 0x06, (UINT8)((MG&0xff0)>>4) ) ;	//Set all mode detect(1/2)
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(param->nim_param.MB86A2x_Adr), 0x1f, (UINT8)(MG&0x00f) ) ;	//Set all mode detect(1/2)
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	return MB86A2x_OK ;
}

MB86A2x_ERR_MSG MB86A2x_Set_ModeGuard_all( MB86A2x_PARAM *param )
{
	UINT32 Status ;

	Status = MB86A2x_write( (UINT8)(param->nim_param.MB86A2x_Adr), 0x06, 0xff ) ;	//Set all mode detect(1/2)
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(param->nim_param.MB86A2x_Adr), 0x1f, 0xf ) ;	//Set all mode detect(1/2)
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Calc_MASTER_CLK( MB86A2x_PLL_PARAM *pll_param, RDA5880_TunerConfigS *tuner_param )
{
	UINT32 temp;
	if( pll_param->PLLPASS==1 ) {
		pll_param->MASTER_CLK_FREQ_kHz = pll_param->INPUT_CLK_FREQ_kHz ;
	}
	else {
		if( pll_param->REFSEL==MB86A2x_REFSEL_32MHz || pll_param->REFSEL==MB86A2x_REFSEL_26MHz )
			temp = pll_param->INPUT_CLK_FREQ_kHz*10 / 8;
		else if( pll_param->REFSEL==MB86A2x_REFSEL_16MHz )
			temp = pll_param->INPUT_CLK_FREQ_kHz*10 / 4;
		else
			temp = pll_param->INPUT_CLK_FREQ_kHz*10;

		if( pll_param->REFSEL==MB86A2x_REFSEL_04MHz
			|| pll_param->REFSEL==MB86A2x_REFSEL_16MHz
			|| pll_param->REFSEL==MB86A2x_REFSEL_32MHz )
			temp = temp * 114;
		else
			temp = temp * 141;

		if (tuner_param->IF_freq_kHz == MB86A2x_IF_FREQ_36_000kHz){
			temp = temp / 100;
		}
		else{
			if( pll_param->BANDSEL==MB86A2x_BANDSEL_ISDBT_6MHz ){
				if( pll_param->FREQSEL==MB86A2x_FREQSEL_0 )
					temp = temp / 140;
				else if( pll_param->FREQSEL==MB86A2x_FREQSEL_1 )
					temp = temp / 120;
				else if( pll_param->FREQSEL==MB86A2x_FREQSEL_2 )
					temp = temp / 110;
			}
			else if( pll_param->BANDSEL==MB86A2x_BANDSEL_ISDBT_7MHz )
				temp = temp / 120;
			else if( pll_param->BANDSEL==MB86A2x_BANDSEL_ISDBT_8MHz )
				temp = temp / 100;
			else if( pll_param->BANDSEL==MB86A2x_BANDSEL_DVBT_6MHz )
				temp = temp / 160;
			else if( pll_param->BANDSEL==MB86A2x_BANDSEL_DVBT_7MHz )
				temp = temp / 140;
			else if( pll_param->BANDSEL==MB86A2x_BANDSEL_DVBT_8MHz )
				temp = temp / 120;		
		}
		pll_param->MASTER_CLK_FREQ_kHz = temp;
	}

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Set_STR_OFS( MB86A2x_NIM_PARAM *nim_param, MB86A2x_PLL_PARAM *pll_param, MB86A2x_RECOV_PARAM *recov_param )
{
	UINT32 Status ;

	if (pll_param->BANDSEL == MB86A2x_BANDSEL_ISDBT_6MHz)
		recov_param->STR_OFS = ( pll_param->MASTER_CLK_FREQ_kHz*63*1024 - 2097152000 ) / 125 ;
	else if (pll_param->BANDSEL == MB86A2x_BANDSEL_ISDBT_7MHz)
		recov_param->STR_OFS = ( pll_param->MASTER_CLK_FREQ_kHz*54*1024 - 2097152000 ) / 125 ;
	else if (pll_param->BANDSEL == MB86A2x_BANDSEL_ISDBT_8MHz)
		recov_param->STR_OFS = ( pll_param->MASTER_CLK_FREQ_kHz*189*256 - 2097152000 ) / 125 ;
	else if (pll_param->BANDSEL == MB86A2x_BANDSEL_DVBT_6MHz)
		recov_param->STR_OFS =   pll_param->MASTER_CLK_FREQ_kHz*32768/125*7/3 - 16777216 ;
	else if (pll_param->BANDSEL == MB86A2x_BANDSEL_DVBT_7MHz)
		recov_param->STR_OFS =   pll_param->MASTER_CLK_FREQ_kHz*32768/125*2 - 16777216 ;
	else if (pll_param->BANDSEL == MB86A2x_BANDSEL_DVBT_8MHz)
		recov_param->STR_OFS =   pll_param->MASTER_CLK_FREQ_kHz*28672/125*2 - 16777216 ;

	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x28, 0x2a ) ;	//Timing recoevery setting(1/4)
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x29, (UINT8)((recov_param->STR_OFS)>>16) ) ;	//Timing recoevery setting(2/4)
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x2a, (UINT8)(((recov_param->STR_OFS)&0x00ff00)>>8) ) ;	//Timing recoevery setting(3/4)
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x2b, (UINT8)((recov_param->STR_OFS)&0x0000ff) ) ;	//Timing recoevery setting(4/4)
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Set_STR_OFS2( MB86A2x_NIM_PARAM *nim_param, MB86A2x_CH_SCH_INFO *ch_sch_info, UINT8 ch_num, MB86A2x_RECOV_PARAM *recov_param )
{
	UINT32 Status = I2C_OK;

	recov_param->STR_OFS = ch_sch_info->STR[ch_num] ;

	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x28, 0x2a ) ;	//Timing recoevery setting(1/4)
	Status |= MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x29, (UINT8)((recov_param->STR_OFS)>>16) ) ;	//Timing recoevery setting(2/4)
	Status |= MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x2a, (UINT8)(((recov_param->STR_OFS)&0x00ff00)>>8) ) ;	//Timing recoevery setting(3/4)
	Status |= MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x2b, (UINT8)((recov_param->STR_OFS)&0x0000ff) ) ;	//Timing recoevery setting(4/4)
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Set_CRR_OFS( MB86A2x_NIM_PARAM *nim_param, MB86A2x_PLL_PARAM *pll_param, MB86A2x_RECOV_PARAM *recov_param )
{
	UINT32 Status = I2C_OK;

	if( nim_param->tuner_param.IF_freq_kHz==MB86A2x_IF_FREQ_57_000kHz ) {
		recov_param->CRR_OFS = (pll_param->MASTER_CLK_FREQ_kHz*2 - MB86A2x_IF_FREQ_57_000kHz)*16384/pll_param->MASTER_CLK_FREQ_kHz*1024 ;
	}
	else if( nim_param->tuner_param.IF_freq_kHz==MB86A2x_IF_FREQ_44_000kHz ) {
		recov_param->CRR_OFS = (MB86A2x_IF_FREQ_44_000kHz - pll_param->MASTER_CLK_FREQ_kHz)*16384/pll_param->MASTER_CLK_FREQ_kHz*1024 ;
	}
	else if( nim_param->tuner_param.IF_freq_kHz==MB86A2x_IF_FREQ_36_000kHz ) {
		recov_param->CRR_OFS = (pll_param->MASTER_CLK_FREQ_kHz - MB86A2x_IF_FREQ_36_000kHz)*16384/pll_param->MASTER_CLK_FREQ_kHz*1024 ;
	}
	else if( nim_param->tuner_param.IF_freq_kHz==MB86A2x_IF_FREQ_05_000kHz ) {
		recov_param->CRR_OFS = MB86A2x_IF_FREQ_05_000kHz*16384/pll_param->MASTER_CLK_FREQ_kHz*1024 ;
	}
	else if( nim_param->tuner_param.IF_freq_kHz==MB86A2x_IF_FREQ_04_500kHz ) {
		recov_param->CRR_OFS = MB86A2x_IF_FREQ_04_500kHz*16384/pll_param->MASTER_CLK_FREQ_kHz*1024 ;
	}
	else if( nim_param->tuner_param.IF_freq_kHz==MB86A2x_IF_FREQ_04_000kHz ) {
		recov_param->CRR_OFS = MB86A2x_IF_FREQ_04_000kHz*16384/pll_param->MASTER_CLK_FREQ_kHz*1024 ;
	}
	else if( nim_param->tuner_param.IF_freq_kHz==MB86A2x_IF_FREQ_03_650kHz ) {
		recov_param->CRR_OFS = MB86A2x_IF_FREQ_03_650kHz*16384/pll_param->MASTER_CLK_FREQ_kHz*1024 ;
	}
	else if( nim_param->tuner_param.IF_freq_kHz==MB86A2x_IF_FREQ_03_300kHz ) {
		recov_param->CRR_OFS = MB86A2x_IF_FREQ_03_300kHz*16384/pll_param->MASTER_CLK_FREQ_kHz*1024 ;
	}
	else if( nim_param->tuner_param.IF_freq_kHz==MB86A2x_IF_FREQ_00_000kHz ) {
		recov_param->CRR_OFS = 0 ;
	}

	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x28, 0x20 ) ;	//Carrier recoevery setting(1/4)
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x29, (UINT8)((recov_param->CRR_OFS)>>16) ) ;	//Carrier recoevery setting(2/4)
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x2a, (UINT8)(((recov_param->CRR_OFS)&0x00ff00)>>8) ) ;	//Carrier recoevery setting(3/4)
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x2b, (UINT8)((recov_param->CRR_OFS)&0x0000ff) ) ;	//Carrier recoevery setting(4/4)
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Set_WIDE_FREQ_DET( MB86A2x_NIM_PARAM *nim_param, MB86A2x_PLL_PARAM *pll_param )
{
	UINT32 Status ;
	UINT32 temp_value ;

	if ( pll_param->BANDSEL == MB86A2x_BANDSEL_ISDBT_6MHz)
	temp_value = ((8192000*64+pll_param->MASTER_CLK_FREQ_kHz/2)/(pll_param->MASTER_CLK_FREQ_kHz)*32+31)/63 ;
	else if ( pll_param->BANDSEL == MB86A2x_BANDSEL_ISDBT_7MHz)
	temp_value = (((8192000*64+pll_param->MASTER_CLK_FREQ_kHz/2)/(pll_param->MASTER_CLK_FREQ_kHz)*32*7+31)/63+3)/6 ;
	else if ( pll_param->BANDSEL == MB86A2x_BANDSEL_ISDBT_8MHz)
	temp_value = (((8192000*64+pll_param->MASTER_CLK_FREQ_kHz/2)/(pll_param->MASTER_CLK_FREQ_kHz)*32*8+31)/63+3)/6 ;
	else if ( pll_param->BANDSEL == MB86A2x_BANDSEL_DVBT_6MHz)
	temp_value = ((8192000*48+pll_param->MASTER_CLK_FREQ_kHz/2)/(pll_param->MASTER_CLK_FREQ_kHz)*4+3)/7 ;
	else if ( pll_param->BANDSEL == MB86A2x_BANDSEL_DVBT_7MHz)
	temp_value = (((8192000*48+pll_param->MASTER_CLK_FREQ_kHz/2)/(pll_param->MASTER_CLK_FREQ_kHz)*4*7+3)/7+3)/6 ;
	else if ( pll_param->BANDSEL == MB86A2x_BANDSEL_DVBT_8MHz)
	temp_value = (((8192000*48+pll_param->MASTER_CLK_FREQ_kHz/2)/(pll_param->MASTER_CLK_FREQ_kHz)*4*8+3)/7+3)/6 ;

	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x28, 0x22 ) ;	//Wide freq detector data conversion setting(1/4)
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x29, (UINT8)((temp_value)>>16) ) ;	//Wide freq detector data conversion setting(2/4)
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x2a, (UINT8)(((temp_value)&0x00ff00)>>8) ) ;	//Wide freq detector data conversion setting(3/4)
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x2b, (UINT8)((temp_value)&0x0000ff) ) ;	//Wide freq detector data conversion setting(4/4)
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Set_AGC(  MB86A2x_NIM_PARAM *nim_param, MB86A2x_AGC_PARAM * agc_param )
{
	UINT32 Status ;
	UINT8 value ;

	value = 0x00 | (UINT8)((agc_param->MAIF)<<4) | (UINT8)((agc_param->IFAGC_POL)<<3) ;

	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x01, value ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x04, 0x19 ) ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x05, 0x00 ) ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Set_IFAGC( MB86A2x_NIM_PARAM *nim_param, MB86A2x_IFAGC_PARAM *ifagc_param )
{
	UINT32 Status = I2C_OK;

	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x04, 0x08 ) ;	//DTS setting(1/4)
	Status |= MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x05, (UINT8)((ifagc_param->DTS)&0x00ff) ) ;	//DTS setting(2/4)
	//Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x04, 0x1b ) ;	//DTS setting(3/4)
	//if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	//Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x05, (UINT8)((ifagc_param->DTS)>>8) ) ;	//DTS setting(4/4)
	//if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	Status |= MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x04, 0x0e ) ;	//VIFREF setting(1/4)
	Status |= MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x05, (UINT8)((ifagc_param->VIFREF)>>8) ) ;	//VIFREF setting(2/4)
	Status |= MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x04, 0x0f ) ;	//VIFREF setting(3/4)
	Status |= MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x05, (UINT8)((ifagc_param->VIFREF)&0x00ff) ) ;	//VIFREF setting(4/4)

	Status |= MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x04, 0x0b ) ;	//AGAIN setting(1/2)
	Status |= MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x05, (UINT8)(ifagc_param->AGAIN) ) ;	//AGAIN setting(2/2)

	Status |= MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x04, 0x00 ) ;	//IFA setting(1/4)
	Status |= MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x05, (UINT8)((ifagc_param->IFA)>>8) ) ;	//IFA setting(2/4)
	Status |= MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x04, 0x01 ) ;	//IFA setting(3/4)
	Status |= MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x05, (UINT8)((ifagc_param->IFA)&0x00ff) ) ;	//IFA setting(4/4)

	Status |= MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x04, 0x02 ) ;	//IFB setting(1/4)
	Status |= MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x05, (UINT8)((ifagc_param->IFB)>>8) ) ;	//IFB setting(2/4)
	Status |= MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x04, 0x03 ) ;	//IFB setting(3/4)
	Status |= MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x05, (UINT8)((ifagc_param->IFB)&0x00ff) ) ;	//IFB setting(4/4)

	Status |= MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x04, 0x09 ) ;	//IFAGCO setting(1/2)
	Status |= MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x05, (UINT8)(ifagc_param->IFAGCO) ) ;	//IFAGCO setting(2/2)

	Status |= MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x04, 0x0a ) ;	//MAXIFAGC setting(1/2)
	Status |= MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x05, (UINT8)(ifagc_param->MAXIFAGC) ) ;	//MAXIFAGC setting(2/2)

	Status |= MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x04, 0x29 ) ;	//IFSAMPLE setting(1/2)
	Status |= MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x05, (UINT8)(ifagc_param->IFSAMPLE) ) ;	//IFSAMPLE setting(2/2)

	Status |= MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x04, 0x32 ) ;	//OUTSAMPLE setting(1/2)
	Status |= MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x05, (UINT8)(ifagc_param->OUTSAMPLE) ) ;	//OUTSAMPLE setting(2/2)

	Status |= MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x04, 0x27 ) ;	//VMAIF setting(1/2)
	Status |= MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x05, (UINT8)(ifagc_param->VMAIF) ) ;	//VMAIF setting(2/2)
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Set_RFAGC( MB86A2x_NIM_PARAM *nim_param, MB86A2x_RFAGC_PARAM *rfagc_param )
{
	UINT32 Status ;

	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x04, 0x1e ) ;	//VMARF setting(1/2)
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x05, (UINT8)(rfagc_param->VMARF) ) ;	//VMARF setting(2/2)
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Set_Opt_Param( MB86A2x_NIM_PARAM *nim_param, MB86A2x_TU_DEP_PARAM *tun_dep_param, MB86A2x_VERSION_INFO *version_info )
{
	UINT32 Status ;
	
	if( version_info->LSI_VERSION==0x43 ) {	//MB86A22(ES2)
		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x28, 0x19 ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x29, 0x00 ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x2a, 0x00 ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x2b, 0xa2 ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x3b, 0xd1 ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x3c, 0xa1 ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x50, 0x38 ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x51, 0xa0 ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x28, 0xee ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x29, 0xc0 ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x2a, 0x04 ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x2b, 0x00 ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x1a, 0x09 ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x1b, 0x43 ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x28, 0x4f ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x29, 0x00 ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x2a, 0x00 ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x2b, 0xff ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x20, 0xab ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x21, 0x89 ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x22, 0x37 ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x28, 0x59 ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x29, 0x00 ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x2a, 0x00 ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x2b, 0x0f ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x28, 0x64 ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x29, 0x00 ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x2a, 0x00 ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x2b, 0x80 ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x23, 0x81 ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x24, 0x91 ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x25, 0x93 ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x26, 0x92 ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x27, 0x94 ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x28, 0x68 ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x29, 0x61 ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x2a, 0x22 ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x2b, 0x22 ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x28, 0x3e ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x29, 0xb1 ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x2a, 0x82 ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x2b, 0x42 ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x28, 0x3d ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x29, 0x00 ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x2a, 0x06 ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x2b, 0xc4 ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x28, 0x54 ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x29, 0x00 ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x2a, 0x00 ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x2b, 0x40 ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x28, 0x52 ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x29, 0x00 ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x2a, 0x03 ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x2b, 0x00 ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

		//2011.03.30 Takagi
		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x3b, 0x6c ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x3c, 0x72 ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x28, 0x6a ) ;	
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x29, 0x00 ) ;	
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x2a, (UINT8)((0x00ff00&(tun_dep_param->S1CNT))>>8) ) ;	
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x2b, (UINT8)( 0x0000ff&(tun_dep_param->S1CNT)) ) ;	

	}
	else {	//MB86A22 ES3(No.12)
		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x28, 0x52 ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x29, 0x00 ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x2a, 0x07 ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x2b, 0x04 ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x28, 0x32 ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x29, 0x81 ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x2a, 0x81 ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x2b, 0x70 ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x28, 0x68 ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x29, 0x61 ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x2a, 0x22 ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x2b, 0x22 ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x20, 0x7c ) ;

		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x28, 0x3c ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x29, 0x00 ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x2a, 0x02 ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x2b, 0x11 ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x28, 0x75 ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x29, 0x00 ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x2a, 0x00 ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x2b, 0xff ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x28, 0x64 ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x29, 0x00 ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x2a, 0x00 ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x2b, 0x80 ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x23, 0x50 ) ;
		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x24, 0x30 ) ;
		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x25, 0x00 ) ;
		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x26, 0x10 ) ;
		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x27, 0x00 ) ;

		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x3b, 0x2f ) ;
		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x3c, 0x60 ) ;

		//2011.03.30 Takagi
		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x3b, 0x6c ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x3c, 0x72 ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x28, 0x6a ) ;	
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x29, 0x00 ) ;	
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x2a, (UINT8)((0x00ff00&(tun_dep_param->S1CNT))>>8) ) ;	
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x2b, (UINT8)( 0x0000ff&(tun_dep_param->S1CNT)) ) ;
		
		//2011.11.18 Takagi
		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x3b, 0x5d ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x3c, 0x00 ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x3b, 0xe9 ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x3c, 0x80 ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x3b, 0xea ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x3c, 0x11 ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x40, 0x38 ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x41, 0xc9 ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x40, 0x6b ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x41, 0x03 ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

		/*
		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x3b, 0x4c ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x3c, 0x57 ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
		*/
		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x3b, 0x4c ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x3c, 0x74 ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	}

	return MB86A2x_OK ;

}


MB86A2x_ERR_MSG MB86A2x_Set_TS_Output( MB86A2x_NIM_PARAM *nim_param, MB86A2x_TSOUT_PARAM *tsout_param, int RS_EN )
{
	UINT32 Status ;
	UINT8 temp ;

	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x50, 0xd5 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	//Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x51, (UINT8)(0x00|(tsout_param->TS_ERR_MASK<<1)|(tsout_param->TS_OUT_SEL)|0x20) ) ;//LXD TSÊä³öLSB
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x51, (UINT8)(0x00|(tsout_param->TS_ERR_MASK<<1)|(tsout_param->TS_OUT_SEL)) ) ;//LXD TSÊä³öLSB
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	if( tsout_param->TS_OUT_SEL==MB86A2x_TS_OUT_PARALLEL ) {
		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x50, 0xd6 ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
		temp = (UINT8)(0x00|(tsout_param->TS_SYNC<<4)|(tsout_param->TS_PARITY<<3)|(tsout_param->TS_LAYER)) ;
		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x51, temp ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x50, 0xd2 ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
		if( tsout_param->TS_CLK==MB86A2x_TS_CLK_NEG ) temp = 0x07 ;
		else                                          temp = 0x03 ;
		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x51, temp ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	}
	else {
		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x50, 0xd7 ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
		//temp = (UINT8)(0x20|(tsout_param->TS_CLK<<7)|(tsout_param->TS_SYNC<<4)|(tsout_param->TS_PARITY<<3)|(tsout_param->TS_LAYER)) ;
		temp = (UINT8)(0x00|(tsout_param->TS_CLK<<7)|(RS_EN<<5)|
			(tsout_param->TS_SYNC<<4)|(tsout_param->TS_PARITY<<3)|(tsout_param->TS_LAYER)) ;
		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x51, temp ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	}

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Set_Channel( MB86A2x_PARAM *param )
{
	UINT32 Status ;
	MB86A2x_ERR_MSG ERR_MSG ;

	Status = MB86A2x_write( (UINT8)(param->nim_param.MB86A2x_Adr), 0x08, 0x01 ) ;	//sequencer stop
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	if( param->nim_param.RF_CONT_EN==1 ) {
		ERR_MSG = MB86A2x_Set_RF( &param->nim_param, &param->pll_param ) ;
		if( ERR_MSG==MB86A2x_ERR_RFTUNE ) return MB86A2x_ERR_RFTUNE ;
	}

	ERR_MSG = MB86A2x_Set_IQ( &param->nim_param ) ;
	if( ERR_MSG==MB86A2x_ERR_INIT ) return MB86A2x_ERR_INIT ;

	ERR_MSG = MB86A2x_Set_RECEP_LAYER( &param->nim_param, &param->layer_param ) ;
	if( ERR_MSG==MB86A2x_ERR_INIT ) return MB86A2x_ERR_INIT ;

	Status = MB86A2x_write( (UINT8)(param->nim_param.MB86A2x_Adr), 0x08, 0x00 ) ;	//sequencer re-start
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Set_Channel_1( MB86A2x_PARAM *param )
{
	UINT32 Status ;

	Status = MB86A2x_write( (UINT8)(param->nim_param.MB86A2x_Adr), 0x08, 0x01 ) ;	//sequencer stop
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Set_Channel_2( MB86A2x_PARAM *param )
{
	UINT32 Status ;
	MB86A2x_ERR_MSG ERR_MSG ;

	ERR_MSG = MB86A2x_Set_IQ( &param->nim_param ) ;
	if( ERR_MSG==MB86A2x_ERR_INIT ) return MB86A2x_ERR_INIT ;

	ERR_MSG = MB86A2x_Set_RECEP_LAYER( &param->nim_param, &param->layer_param ) ;
	if( ERR_MSG==MB86A2x_ERR_INIT ) return MB86A2x_ERR_INIT ;

	Status = MB86A2x_write( (UINT8)(param->nim_param.MB86A2x_Adr), 0x70, 0xf0 ) ;	//execute logic reset
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(param->nim_param.MB86A2x_Adr), 0x70, 0xff ) ;	//release logic reset
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	Status = MB86A2x_write( (UINT8)(param->nim_param.MB86A2x_Adr), 0x08, 0x00 ) ;	//sequencer re-start
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Set_Monitor_Timer( MB86A2x_PARAM *param )
{
	MB86A2x_ERR_MSG ERR_MSG ;

	ERR_MSG = MB86A2x_Set_VBER_Timer( &param->nim_param, &param->mon_tim_param ) ;
	if( ERR_MSG==MB86A2x_ERR_INIT ) return MB86A2x_ERR_INIT ;

	ERR_MSG = MB86A2x_Set_SBER_Timer( &param->nim_param, &param->mon_tim_param ) ;
	if( ERR_MSG==MB86A2x_ERR_INIT ) return MB86A2x_ERR_INIT ;

	ERR_MSG = MB86A2x_Set_PBER_Timer( &param->nim_param, &param->mon_tim_param ) ;
	if( ERR_MSG==MB86A2x_ERR_INIT ) return MB86A2x_ERR_INIT ;

	ERR_MSG = MB86A2x_Set_CN_Timer( &param->nim_param, &param->mon_tim_param ) ;
	if( ERR_MSG==MB86A2x_ERR_INIT ) return MB86A2x_ERR_INIT ;

	ERR_MSG = MB86A2x_Set_MER_Timer( &param->nim_param, &param->mon_tim_param ) ;
	if( ERR_MSG==MB86A2x_ERR_INIT ) return MB86A2x_ERR_INIT ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Reset_Monitor_Timer( MB86A2x_PARAM *param )
{
	MB86A2x_ERR_MSG ERR_MSG ;

	ERR_MSG = MB86A2x_Reset_VBER_ALL_Counter( &param->nim_param ) ;
	if( ERR_MSG==MB86A2x_ERR_INIT ) return MB86A2x_ERR_INIT ;

	ERR_MSG = MB86A2x_Reset_SBER_ALL_Counter( &param->nim_param ) ;
	if( ERR_MSG==MB86A2x_ERR_INIT ) return MB86A2x_ERR_INIT ;

	ERR_MSG = MB86A2x_Reset_PBER_ALL_Counter( &param->nim_param ) ;
	if( ERR_MSG==MB86A2x_ERR_INIT ) return MB86A2x_ERR_INIT ;

	ERR_MSG = MB86A2x_Reset_CN_Counter( &param->nim_param, &param->mon_tim_param ) ;
	if( ERR_MSG==MB86A2x_ERR_INIT ) return MB86A2x_ERR_INIT ;

	ERR_MSG = MB86A2x_Reset_MER_Counter( &param->nim_param ) ;
	if( ERR_MSG==MB86A2x_ERR_INIT ) return MB86A2x_ERR_INIT ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Calc_VBER_NUM( MB86A2x_BANDSEL BANDSEL, MB86A2x_CH_SCH_INFO *ch_sch_info, UINT32 VBER_T_A, UINT32 VBER_T_B, UINT32 VBER_T_C,
										UINT8 ch_num, MB86A2x_MON_TIM_PARAM *mon_tim_param )
{
	UINT32 sym_length ;
	UINT32 carrier_num ;
	UINT32 symbol_bit ;
	UINT32 R ;
	UINT32 temp ;

	sym_length = MB86A2x_calc_symbol_length_us( BANDSEL, ch_sch_info->MODE[ch_num], ch_sch_info->GUARD[ch_num] ) ;
	carrier_num = MB86A2x_calc_carrier_num( BANDSEL, ch_sch_info->MODE[ch_num] ) ;

	symbol_bit = MB86A2x_calc_symbol_bit( ch_sch_info->MOD_A[ch_num] ) ;
	R = MB86A2x_calc_data_ratio_1000( ch_sch_info->RATE_A[ch_num] ) ;
	if (BANDSEL == MB86A2x_BANDSEL_ISDBT_6MHz || BANDSEL == MB86A2x_BANDSEL_ISDBT_7MHz || BANDSEL == MB86A2x_BANDSEL_ISDBT_8MHz)
		temp = carrier_num*symbol_bit*R*(ch_sch_info->NUM_A[ch_num])/sym_length*VBER_T_A/26 ;
	else if (BANDSEL == MB86A2x_BANDSEL_DVBT_6MHz || BANDSEL == MB86A2x_BANDSEL_DVBT_7MHz || BANDSEL == MB86A2x_BANDSEL_DVBT_8MHz)
		temp = VBER_T_A*carrier_num*symbol_bit*R*13/sym_length/26 ;

	if( temp>16777215 ) temp = 16777215 ;
	if( temp==0 )       temp = 1 ;
	mon_tim_param->VBER_A_NUM = temp ;

	if( (1<=ch_sch_info->NUM_B[ch_num])&&(ch_sch_info->NUM_B[ch_num]<=12) ) {	//Layer-B exsits
		symbol_bit = MB86A2x_calc_symbol_bit( ch_sch_info->MOD_B[ch_num] ) ;
		R = MB86A2x_calc_data_ratio_1000( ch_sch_info->RATE_B[ch_num] ) ;
		if (BANDSEL == MB86A2x_BANDSEL_ISDBT_6MHz || BANDSEL == MB86A2x_BANDSEL_ISDBT_7MHz || BANDSEL == MB86A2x_BANDSEL_ISDBT_8MHz)
			temp = carrier_num*symbol_bit*R*(ch_sch_info->NUM_B[ch_num])/sym_length*VBER_T_B/26 ;
		else if (BANDSEL == MB86A2x_BANDSEL_DVBT_6MHz || BANDSEL == MB86A2x_BANDSEL_DVBT_7MHz || BANDSEL == MB86A2x_BANDSEL_DVBT_8MHz)
			temp = VBER_T_B*carrier_num*symbol_bit*R*13/sym_length/26 ;
		if( temp>16777215 ) temp = 16777215 ;
		if( temp==0 )       temp = 1 ;
		mon_tim_param->VBER_B_NUM = temp ;
	}
	else {
		mon_tim_param->VBER_B_NUM = 0x7fffff ;
	}

	if( (1<=ch_sch_info->NUM_C[ch_num])&&(ch_sch_info->NUM_C[ch_num]<=11) ) {	//Layer-C exsits
		symbol_bit = MB86A2x_calc_symbol_bit( ch_sch_info->MOD_C[ch_num] ) ;
		R = MB86A2x_calc_data_ratio_1000( ch_sch_info->RATE_C[ch_num] ) ;
		if (BANDSEL == MB86A2x_BANDSEL_ISDBT_6MHz || BANDSEL == MB86A2x_BANDSEL_ISDBT_7MHz || BANDSEL == MB86A2x_BANDSEL_ISDBT_8MHz)
			temp = carrier_num*symbol_bit*R*(ch_sch_info->NUM_C[ch_num])/sym_length*VBER_T_C/26 ;
		else if (BANDSEL == MB86A2x_BANDSEL_DVBT_6MHz || BANDSEL == MB86A2x_BANDSEL_DVBT_7MHz || BANDSEL == MB86A2x_BANDSEL_DVBT_8MHz)
			temp = VBER_T_C*carrier_num*symbol_bit*R*13/sym_length/26 ;
		if( temp>16777215 ) temp = 16777215 ;
		if( temp==0 )       temp = 1 ;
		mon_tim_param->VBER_C_NUM = temp ;
	}
	else {
		mon_tim_param->VBER_C_NUM = 0x7fffff ;
	}

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Calc_SBER_NUM( MB86A2x_BANDSEL BANDSEL, MB86A2x_CH_SCH_INFO *ch_sch_info, UINT32 SBER_T_A, UINT32 SBER_T_B, UINT32 SBER_T_C,
										UINT8 ch_num, MB86A2x_MON_TIM_PARAM *mon_tim_param )
{
	UINT32 sym_length ;
	UINT32 TSP_num ;
	UINT32 temp ;

	sym_length = MB86A2x_calc_symbol_length_us( BANDSEL, ch_sch_info->MODE[ch_num], ch_sch_info->GUARD[ch_num] ) ;

	TSP_num = MB86A2x_calc_TSP_num( ch_sch_info->MODE[ch_num], ch_sch_info->MOD_A[ch_num], ch_sch_info->RATE_A[ch_num] ) ;
	if (BANDSEL == MB86A2x_BANDSEL_ISDBT_6MHz || BANDSEL == MB86A2x_BANDSEL_ISDBT_7MHz || BANDSEL == MB86A2x_BANDSEL_ISDBT_8MHz)
		temp = SBER_T_A*TSP_num*100/sym_length*(ch_sch_info->NUM_A[ch_num])*10/2652 ;
	else if (BANDSEL == MB86A2x_BANDSEL_DVBT_6MHz || BANDSEL == MB86A2x_BANDSEL_DVBT_7MHz || BANDSEL == MB86A2x_BANDSEL_DVBT_8MHz)
		temp = SBER_T_A*TSP_num*13/sym_length/2652*1000 ;
	if( temp>65535 ) temp = 65535 ;
	if( temp<0 )     temp = 1 ;
	mon_tim_param->SBER_A_NUM = temp ;

	if( (1<=ch_sch_info->NUM_B[ch_num])&&(ch_sch_info->NUM_B[ch_num]<=12) ) {	//Layer-B exsits
		TSP_num = MB86A2x_calc_TSP_num( ch_sch_info->MODE[ch_num], ch_sch_info->MOD_B[ch_num], ch_sch_info->RATE_B[ch_num] ) ;
		if (BANDSEL == MB86A2x_BANDSEL_ISDBT_6MHz || BANDSEL == MB86A2x_BANDSEL_ISDBT_7MHz || BANDSEL == MB86A2x_BANDSEL_ISDBT_8MHz)
			temp = SBER_T_B*TSP_num*100/sym_length*(ch_sch_info->NUM_B[ch_num])*10/2652 ;
		else if (BANDSEL == MB86A2x_BANDSEL_DVBT_6MHz || BANDSEL == MB86A2x_BANDSEL_DVBT_7MHz || BANDSEL == MB86A2x_BANDSEL_DVBT_8MHz)
			temp = SBER_T_B*TSP_num*13/sym_length/2652*1000 ;
		if( temp>65535 ) temp = 65535 ;
		if( temp==0 )    temp = 1 ;
		mon_tim_param->SBER_B_NUM = temp ;
	}
	else {
		mon_tim_param->SBER_B_NUM = 0x7fff ;
	}

	if( (1<=ch_sch_info->NUM_C[ch_num])&&(ch_sch_info->NUM_C[ch_num]<=11) ) {	//Layer-C exsits
		TSP_num = MB86A2x_calc_TSP_num( ch_sch_info->MODE[ch_num], ch_sch_info->MOD_C[ch_num], ch_sch_info->RATE_C[ch_num] ) ;
		if (BANDSEL == MB86A2x_BANDSEL_ISDBT_6MHz || BANDSEL == MB86A2x_BANDSEL_ISDBT_7MHz || BANDSEL == MB86A2x_BANDSEL_ISDBT_8MHz)
			temp = SBER_T_C*TSP_num*100/sym_length*(ch_sch_info->NUM_C[ch_num])*10/2652 ;
		else if (BANDSEL == MB86A2x_BANDSEL_DVBT_6MHz || BANDSEL == MB86A2x_BANDSEL_DVBT_7MHz || BANDSEL == MB86A2x_BANDSEL_DVBT_8MHz)
			temp = SBER_T_C*TSP_num*13/sym_length/2652*1000 ;
		if( temp>65535 ) temp = 65535 ;
		if( temp==0 )    temp = 1 ;
		mon_tim_param->SBER_C_NUM = temp ;
	}
	else {
		mon_tim_param->SBER_C_NUM = 0x7fff ;
	}

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Calc_PBER_NUM( MB86A2x_BANDSEL BANDSEL, MB86A2x_CH_SCH_INFO *ch_sch_info, UINT32 PBER_T_A, UINT32 PBER_T_B, UINT32 PBER_T_C,
										UINT8 ch_num, MB86A2x_MON_TIM_PARAM *mon_tim_param )
{
	UINT32 sym_length ;
	UINT32 TSP_num ;
	UINT32 temp ;

	sym_length = MB86A2x_calc_symbol_length_us( BANDSEL, ch_sch_info->MODE[ch_num], ch_sch_info->GUARD[ch_num] ) ;

	TSP_num = MB86A2x_calc_TSP_num( ch_sch_info->MODE[ch_num], ch_sch_info->MOD_A[ch_num], ch_sch_info->RATE_A[ch_num] ) ;
	if (BANDSEL == MB86A2x_BANDSEL_ISDBT_6MHz || BANDSEL == MB86A2x_BANDSEL_ISDBT_7MHz || BANDSEL == MB86A2x_BANDSEL_ISDBT_8MHz)
		temp = PBER_T_A*TSP_num*100/sym_length*(ch_sch_info->NUM_A[ch_num])*10/2652 ;
	else if (BANDSEL == MB86A2x_BANDSEL_DVBT_6MHz || BANDSEL == MB86A2x_BANDSEL_DVBT_7MHz || BANDSEL == MB86A2x_BANDSEL_DVBT_8MHz)
		temp = PBER_T_A*TSP_num*13/sym_length/2652*1000 ;
	if( temp>655350 ) temp = 65535 ;
	if( temp==0 )     temp = 1 ;
	mon_tim_param->PBER_A_NUM = temp ;

	if( (1<=ch_sch_info->NUM_B[ch_num])&&(ch_sch_info->NUM_B[ch_num]<=12) ) {	//Layer-B exsits
		TSP_num = MB86A2x_calc_TSP_num( ch_sch_info->MODE[ch_num], ch_sch_info->MOD_B[ch_num], ch_sch_info->RATE_B[ch_num] ) ;
		if (BANDSEL == MB86A2x_BANDSEL_ISDBT_6MHz || BANDSEL == MB86A2x_BANDSEL_ISDBT_7MHz || BANDSEL == MB86A2x_BANDSEL_ISDBT_8MHz)
			temp = PBER_T_B*TSP_num*100/sym_length*(ch_sch_info->NUM_B[ch_num])*10/2652 ;
		else if (BANDSEL == MB86A2x_BANDSEL_DVBT_6MHz || BANDSEL == MB86A2x_BANDSEL_DVBT_7MHz || BANDSEL == MB86A2x_BANDSEL_DVBT_8MHz)
			temp = PBER_T_B*TSP_num*13/sym_length/2652*1000 ;
		if( temp>655350 ) temp = 65535 ;
		if( temp==0 )     temp = 1 ;
		mon_tim_param->PBER_B_NUM = temp ;
	}
	else {
		mon_tim_param->PBER_B_NUM = 0x7fff ;
	}

	if( (1<=ch_sch_info->NUM_C[ch_num])&&(ch_sch_info->NUM_C[ch_num]<=11) ) {	//Layer-C exsits
		TSP_num = MB86A2x_calc_TSP_num( ch_sch_info->MODE[ch_num], ch_sch_info->MOD_C[ch_num], ch_sch_info->RATE_C[ch_num] ) ;
		if (BANDSEL == MB86A2x_BANDSEL_ISDBT_6MHz || BANDSEL == MB86A2x_BANDSEL_ISDBT_7MHz || BANDSEL == MB86A2x_BANDSEL_ISDBT_8MHz)
			temp = PBER_T_C*TSP_num*100/sym_length*(ch_sch_info->NUM_C[ch_num])*10/2652 ;
		else if (BANDSEL == MB86A2x_BANDSEL_DVBT_6MHz || BANDSEL == MB86A2x_BANDSEL_DVBT_7MHz || BANDSEL == MB86A2x_BANDSEL_DVBT_8MHz)
			temp = PBER_T_C*TSP_num*13/sym_length/2652*1000 ;
		if( temp>655350 ) temp = 65535 ;
		if( temp==0 )     temp = 1 ;
		mon_tim_param->PBER_C_NUM = temp ;
	}
	else {
		mon_tim_param->PBER_C_NUM = 0x7fff ;
	}

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Calc_MER_SYMBOL( MB86A2x_BANDSEL BANDSEL, MB86A2x_CH_SCH_INFO *ch_sch_info, UINT32 MER_T, UINT8 ch_num, MB86A2x_MON_TIM_PARAM *mon_tim_param )
{
	UINT32 sym_length ;
	UINT32  MER_T_temp ;
	UINT8  mer_symbol_temp ;
	UINT8  loop ;

	sym_length = MB86A2x_calc_symbol_length_us( BANDSEL, ch_sch_info->MODE[ch_num], ch_sch_info->GUARD[ch_num] ) ;

	loop = 1 ;
	mer_symbol_temp = 0 ;
	while( loop==1 ) {
		MER_T_temp = (UINT32)( sym_length * (1<<mer_symbol_temp) ) ;	//calculation is different from C/N
		if( MER_T_temp>=MER_T ) loop = 0 ;
		else mer_symbol_temp++ ;
		if( mer_symbol_temp>7 ) loop = 0 ;
	}
	if(      mer_symbol_temp==0 ) mon_tim_param->MER_SYMBOL = mer_symbol_temp ;
	else if( mer_symbol_temp>7  ) mon_tim_param->MER_SYMBOL = 7 ;
	else if( MER_T_temp==MER_T  ) mon_tim_param->MER_SYMBOL = mer_symbol_temp ;
	else if( MER_T_temp>MER_T   ) mon_tim_param->MER_SYMBOL = mer_symbol_temp - 1 ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Calc_CN_SYMBOL( MB86A2x_BANDSEL BANDSEL, MB86A2x_CH_SCH_INFO *ch_sch_info, UINT32 CN_T, UINT8 ch_num, MB86A2x_MON_TIM_PARAM *mon_tim_param )
{
	UINT32 sym_length ;
	UINT32  CN_T_temp ;
	UINT8  cn_symbol_temp ;
	UINT8  loop ;

	sym_length = MB86A2x_calc_symbol_length_us( BANDSEL, ch_sch_info->MODE[ch_num], ch_sch_info->GUARD[ch_num] ) ;

	loop = 1 ;
	cn_symbol_temp = 0 ;
	while( loop==1 ) {
		CN_T_temp = sym_length * (1<<(cn_symbol_temp+1)) ;	//calculation is different from MER
		if( CN_T_temp>=CN_T*1000 ) loop = 0 ;
		else cn_symbol_temp++ ;
		if( cn_symbol_temp>15 ) loop = 0 ;
	}
	if(      cn_symbol_temp==0 )    mon_tim_param->CN_SYMBOL = cn_symbol_temp ;
	else if( cn_symbol_temp>15 )    mon_tim_param->CN_SYMBOL = 15 ;
	else if( CN_T_temp==CN_T*1000 ) mon_tim_param->CN_SYMBOL = cn_symbol_temp ;
	else if( CN_T_temp>CN_T*1000 )  mon_tim_param->CN_SYMBOL = cn_symbol_temp - 1 ;

	return MB86A2x_OK ;
}


UINT32 MB86A2x_Calc_MER_Period( MB86A2x_BANDSEL BANDSEL, MB86A2x_CH_SCH_INFO *ch_sch_info, UINT8 ch_num, MB86A2x_MON_TIM_PARAM *mon_tim_param )
{
	UINT32 sym_length ;
	UINT32  MER_T ;

	sym_length = MB86A2x_calc_symbol_length_us( BANDSEL, ch_sch_info->MODE[ch_num], ch_sch_info->GUARD[ch_num] ) ;

	MER_T = sym_length * (1<<mon_tim_param->MER_SYMBOL) / 1000 ;	//calculation is different from C/N

	return MER_T ;
}


UINT32 MB86A2x_Calc_CN_Period( MB86A2x_BANDSEL BANDSEL, MB86A2x_CH_SCH_INFO *ch_sch_info, UINT8 ch_num, MB86A2x_MON_TIM_PARAM *mon_tim_param )
{
	UINT32 sym_length ;
	UINT32  CN_T ;

	sym_length = MB86A2x_calc_symbol_length_us( BANDSEL, ch_sch_info->MODE[ch_num], ch_sch_info->GUARD[ch_num] ) ;

	CN_T = sym_length * (1<<(mon_tim_param->CN_SYMBOL+1)) / 1000 ;

	return CN_T ;
}


UINT32 MB86A2x_calc_symbol_length_us( MB86A2x_BANDSEL BANDSEL, UINT8 MODE, UINT8 GUARD )
{
	UINT32 sym_length_e, sym_length ;
	UINT32 guard_r ;

	if (BANDSEL == MB86A2x_BANDSEL_ISDBT_6MHz){
		if(      MODE==0 ) sym_length_e = 252 ;
		else if( MODE==1 ) sym_length_e = 504 ;
		else if( MODE==2 ) sym_length_e = 1008 ;
	}
	else if (BANDSEL == MB86A2x_BANDSEL_ISDBT_7MHz){
		if(      MODE==0 ) sym_length_e = 216 ;
		else if( MODE==1 ) sym_length_e = 432 ;
		else if( MODE==2 ) sym_length_e = 864 ;
	}
	else if (BANDSEL == MB86A2x_BANDSEL_ISDBT_8MHz){
		if(      MODE==0 ) sym_length_e = 189 ;
		else if( MODE==1 ) sym_length_e = 378 ;
		else if( MODE==2 ) sym_length_e = 756 ;
	}
	else if (BANDSEL == MB86A2x_BANDSEL_DVBT_6MHz){
		if(      MODE==0 ) sym_length_e = 299 ;
		else if( MODE==1 ) sym_length_e = 597 ;
		else if( MODE==2 ) sym_length_e = 1195 ;
	}
	else if (BANDSEL == MB86A2x_BANDSEL_DVBT_7MHz){
		if(      MODE==0 ) sym_length_e = 256 ;
		else if( MODE==1 ) sym_length_e = 512 ;
		else if( MODE==2 ) sym_length_e = 1024 ;
	}
	else if (BANDSEL == MB86A2x_BANDSEL_DVBT_8MHz){
		if(      MODE==0 ) sym_length_e = 224 ;
		else if( MODE==1 ) sym_length_e = 448 ;
		else if( MODE==2 ) sym_length_e = 896 ;
	}
	

	if(      GUARD==0 ) guard_r = 1000+1000/4 ;
	else if( GUARD==1 ) guard_r = 1000+1000/8 ;
	else if( GUARD==2 ) guard_r = 1000+1000/16 ;
	else if( GUARD==3 ) guard_r = 1000+1000/32 ;	

	sym_length = sym_length_e * guard_r / 1000 ;

	return sym_length ;
}


UINT32 MB86A2x_calc_carrier_num( MB86A2x_BANDSEL BANDSEL, UINT8 MODE )
{
	UINT32 carrier_num ;

	if (BANDSEL == MB86A2x_BANDSEL_ISDBT_6MHz || BANDSEL == MB86A2x_BANDSEL_ISDBT_7MHz || BANDSEL == MB86A2x_BANDSEL_ISDBT_8MHz){
		if(      MODE==0 ) carrier_num = 1248 ;
		else if( MODE==1 ) carrier_num = 2496 ;
		else if( MODE==2 ) carrier_num = 4992 ;
	}
	else if (BANDSEL == MB86A2x_BANDSEL_DVBT_6MHz || BANDSEL == MB86A2x_BANDSEL_DVBT_7MHz || BANDSEL == MB86A2x_BANDSEL_DVBT_8MHz){
		if(      MODE==0 ) carrier_num = 1512 ;
		else if( MODE==1 ) carrier_num = 3024 ;
		else if( MODE==2 ) carrier_num = 6048 ;
	}

	return carrier_num ;
}


UINT32 MB86A2x_calc_symbol_bit( UINT8 modulation )
{
	UINT32 symbol_bit ;

	if(      modulation==0 ) symbol_bit = 2 ;
	else if( modulation==1 ) symbol_bit = 2 ;
	else if( modulation==2 ) symbol_bit = 4 ;
	else if( modulation==3 ) symbol_bit = 6 ;

	return symbol_bit ;
}


UINT32 MB86A2x_calc_data_ratio_1000( UINT8 code_rate )
{
	UINT32 data_ratio ;

	if(      code_rate==0 ) data_ratio = 1000 ;
	else if( code_rate==1 ) data_ratio = 2000/3 ;
	else if( code_rate==2 ) data_ratio = 1000 ;
	else if( code_rate==3 ) data_ratio = 1000 ;
	else if( code_rate==4 ) data_ratio =  750 ;

	return data_ratio ;
}


UINT32 MB86A2x_calc_TSP_num( UINT8 mode, UINT8 modulation, UINT8 code_rate )
{
	UINT32 temp1, temp2 ;
	UINT32 TSP_num ;

	if( (modulation==0)||(modulation==1) ) {
		if(      code_rate==0 ) temp1 = 156 ;
		else if( code_rate==1 ) temp1 = 208 ;
		else if( code_rate==2 ) temp1 = 234 ;
		else if( code_rate==3 ) temp1 = 260 ;
		else if( code_rate==4 ) temp1 = 273 ;
	}
	else if( modulation==2 ) {
		if(      code_rate==0 ) temp1 = 312 ;
		else if( code_rate==1 ) temp1 = 416 ;
		else if( code_rate==2 ) temp1 = 468 ;
		else if( code_rate==3 ) temp1 = 520 ;
		else if( code_rate==4 ) temp1 = 546 ;
	}
	else if( modulation==3 ) {
		if(      code_rate==0 ) temp1 = 468 ;
		else if( code_rate==1 ) temp1 = 624 ;
		else if( code_rate==2 ) temp1 = 702 ;
		else if( code_rate==3 ) temp1 = 780 ;
		else if( code_rate==4 ) temp1 = 819 ;
	}

	if(      mode==0 ) temp2 = 1 ;
	else if( mode==1 ) temp2 = 2 ;
	else if( mode==2 ) temp2 = 4 ;

	TSP_num = temp1*temp2 ;

	return TSP_num ;
}


MB86A2x_ERR_MSG MB86A2x_Set_RF( MB86A2x_NIM_PARAM *nim_param, MB86A2x_PLL_PARAM *pll_param )
{
	UINT32 Status ;

	Status = RDA_TUNER_SET_FREQ( &nim_param->tuner_param, pll_param->BANDSEL ) ;	//tuner frequency setting
	if( Status==TUNER_NG ) return MB86A2x_ERR_RFTUNE ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Set_IQ( MB86A2x_NIM_PARAM *nim_param )
{
	UINT32 Status ;

	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x09, (UINT8)(0x3a|((nim_param->IQ_pol)<<2)) ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Init_LOCKNG_INFO( MB86A2x_LOCKNG_INFO *locking_info )
{
	locking_info->S8_REACH = 0 ;
	locking_info->S8_REACH_his = 0 ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Init_SIG_QUAL_INFO( MB86A2x_SIG_QUAL_INFO *sig_qual_info )
{

	sig_qual_info->V_ERR_A = 16777215 ;
	sig_qual_info->V_ERR_B = 16777215 ;
	sig_qual_info->V_ERR_C = 16777215 ;
	sig_qual_info->S_ERR_A = 16777215 ;
	sig_qual_info->S_ERR_B = 16777215 ;
	sig_qual_info->S_ERR_C = 16777215 ;
	sig_qual_info->P_ERR_A = 16777215 ;
	sig_qual_info->P_ERR_B = 16777215 ;
	sig_qual_info->P_ERR_C = 16777215 ;
	sig_qual_info->CN_MON = 16777215 ;
	sig_qual_info->MER_MON_A =16777215 ;
	sig_qual_info->MER_MON_B = 16777215 ;
	sig_qual_info->MER_MON_C = 16777215 ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Store_Channel_Info( UINT32 ch, MB86A2x_TMCC_INFO *tmcc_info, MB86A2x_CH_SCH_INFO *ch_sch_info )
{
	ch_sch_info->CH_num[69] = ch ;
	
	ch_sch_info->MODE[69] = tmcc_info->MODE_current ;
	ch_sch_info->GUARD[69] = tmcc_info->GUARD_current ;

	ch_sch_info->MOD_A[69] = tmcc_info->MOD_A_current ;
	ch_sch_info->MOD_B[69] = tmcc_info->MOD_B_current ;
	ch_sch_info->MOD_C[69] = tmcc_info->MOD_C_current ;

	ch_sch_info->RATE_A[69] = tmcc_info->RATE_A_current ;
	ch_sch_info->RATE_B[69] = tmcc_info->RATE_B_current ;
	ch_sch_info->RATE_C[69] = tmcc_info->RATE_C_current ;

	ch_sch_info->IL_A[69] = tmcc_info->IL_A_current ;
	ch_sch_info->IL_B[69] = tmcc_info->IL_B_current ;
	ch_sch_info->IL_C[69] = tmcc_info->IL_C_current ;

	ch_sch_info->NUM_A[69] = tmcc_info->NUM_A_current ;
	ch_sch_info->NUM_B[69] = tmcc_info->NUM_B_current ;
	ch_sch_info->NUM_C[69] = tmcc_info->NUM_C_current ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Load_CH_SCH_Result( MB86A2x_CH_SCH_INFO *ch_sch_info )
{
	/*
	UINT8 i ;
	FILE *f1 ;
	f1 = fopen( "Channel_Search_Result.txt", "rt" ) ;

	fscanf( f1, "Detected_ch_num=%d\n", &(ch_sch_info->Detected_ch_num) ) ;
	for( i=0 ; i<ch_sch_info->Detected_ch_num ; i++ ) {
		fscanf( f1, "CH_num=%d,", &(ch_sch_info->CH_num[i]) ) ;
		fscanf( f1, "MODE=%d,", &(ch_sch_info->MODE[i]) ) ;
		fscanf( f1, "GUARD=%d,", &(ch_sch_info->GUARD[i]) ) ;

		fscanf( f1, "MOD_A=%d,", &(ch_sch_info->MOD_A[i]) ) ;
		fscanf( f1, "RATE_A=%d,", &(ch_sch_info->RATE_A[i]) ) ;
		fscanf( f1, "IL_A=%d,", &(ch_sch_info->IL_A[i]) ) ;
		fscanf( f1, "NUM_A=%d,", &(ch_sch_info->NUM_A[i]) ) ;

		fscanf( f1, "MOD_B=%d,", &(ch_sch_info->MOD_B[i]) ) ;
		fscanf( f1, "RATE_B=%d,", &(ch_sch_info->RATE_B[i]) ) ;
		fscanf( f1, "IL_B=%d,", &(ch_sch_info->IL_B[i]) ) ;
		fscanf( f1, "NUM_B=%d,", &(ch_sch_info->NUM_B[i]) ) ;

		fscanf( f1, "MOD_C=%d,", &(ch_sch_info->MOD_C[i]) ) ;
		fscanf( f1, "RATE_C=%d,", &(ch_sch_info->RATE_C[i]) ) ;
		fscanf( f1, "IL_C=%d,", &(ch_sch_info->IL_C[i]) ) ;
		fscanf( f1, "NUM_C=%d,", &(ch_sch_info->NUM_C[i]) ) ;

		fscanf( f1, "STR=%d\n", &(ch_sch_info->STR[i]) ) ;
	}
	fclose( f1 ) ;
*/
	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Set_RECEP_LAYER( MB86A2x_NIM_PARAM *nim_param, MB86A2x_LAYER_PARAM *recep_layer )
{
	UINT32 Status ;

	if( recep_layer->RECEP_LAYER==MB86A2x_RECEP_LAYER_A ) {
		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x39, 0x00 ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	}
	else if( recep_layer->RECEP_LAYER==MB86A2x_RECEP_LAYER_B ) {
		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x39, 0x01 ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	}
	else {
		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x39, 0x02 ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	}

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Set_VBER_Timer( MB86A2x_NIM_PARAM *nim_param, MB86A2x_MON_TIM_PARAM *mon_tim_param )
{
	UINT32 Status ;

	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x50, 0xa7 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x51, (UINT8)(((mon_tim_param->VBER_A_NUM)&0x0ff0000)>>16) ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x50, 0xa8 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x51, (UINT8)(((mon_tim_param->VBER_A_NUM)&0x000ff00)>>8) ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x50, 0xa9 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x51, (UINT8)(( mon_tim_param->VBER_A_NUM)&0x00000ff) ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x50, 0xaa ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x51, (UINT8)(((mon_tim_param->VBER_B_NUM)&0x0ff0000)>>16) ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x50, 0xab ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x51, (UINT8)(((mon_tim_param->VBER_B_NUM)&0x000ff00)>>8) ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x50, 0xac ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x51, (UINT8)(( mon_tim_param->VBER_B_NUM)&0x00000ff) ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x50, 0xad ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x51, (UINT8)(((mon_tim_param->VBER_C_NUM)&0x0ff0000)>>16) ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x50, 0xae ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x51, (UINT8)(((mon_tim_param->VBER_C_NUM)&0x000ff00)>>8) ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x50, 0xaf ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x51, (UINT8)(( mon_tim_param->VBER_C_NUM)&0x00000ff) ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Set_SBER_Timer( MB86A2x_NIM_PARAM *nim_param, MB86A2x_MON_TIM_PARAM *mon_tim_param )
{
	UINT32 Status ;

	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x50, 0xdc ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x51, (UINT8)(((mon_tim_param->SBER_A_NUM)&0x000ff00)>>8) ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x50, 0xdd ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x51, (UINT8)(( mon_tim_param->SBER_A_NUM)&0x00000ff) ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x50, 0xde ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x51, (UINT8)(((mon_tim_param->SBER_B_NUM)&0x000ff00)>>8) ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x50, 0xdf ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x51, (UINT8)(( mon_tim_param->SBER_B_NUM)&0x00000ff) ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x50, 0xe0 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x51, (UINT8)(((mon_tim_param->SBER_C_NUM)&0x000ff00)>>8) ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x50, 0xe1 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x51, (UINT8)((mon_tim_param->SBER_C_NUM)&0x00000ff) ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Set_PBER_Timer( MB86A2x_NIM_PARAM *nim_param, MB86A2x_MON_TIM_PARAM *mon_tim_param )
{
	UINT32 Status ;

	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x50, 0xb2 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x51, (UINT8)(((mon_tim_param->PBER_A_NUM)&0x000ff00)>>8) ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x50, 0xb3 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x51, (UINT8)((mon_tim_param->PBER_A_NUM)&0x00000ff) ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x50, 0xb4 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x51, (UINT8)(((mon_tim_param->PBER_B_NUM)&0x000ff00)>>8) ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x50, 0xb5 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x51, (UINT8)((mon_tim_param->PBER_B_NUM)&0x00000ff) ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x50, 0xb6 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x51, (UINT8)(((mon_tim_param->PBER_C_NUM)&0x000ff00)>>8) ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x50, 0xb7 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x51, (UINT8)((mon_tim_param->PBER_C_NUM)&0x00000ff) ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Set_CN_Timer( MB86A2x_NIM_PARAM *nim_param, MB86A2x_MON_TIM_PARAM *mon_tim_param )
{
	UINT32 Status ;
	UINT8  temp ;

	Status = MB86A2x_read( (UINT8)(nim_param->MB86A2x_Adr), 0x45, &temp ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	
	temp = (temp&0xf0) | (UINT8)(mon_tim_param->CN_SYMBOL) ;

	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x45, temp ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Set_MER_Timer( MB86A2x_NIM_PARAM *nim_param, MB86A2x_MON_TIM_PARAM *mon_tim_param )
{
	UINT32 Status ;

	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x50, 0x51 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x51, (UINT8)(mon_tim_param->MER_SYMBOL) ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Get_SEQ_value( MB86A2x_NIM_PARAM *nim_param, MB86A2x_LOCKNG_INFO *locking_info )
{
	UINT8  read_val ;
	UINT32 Status ;

	Status = MB86A2x_read( (UINT8)(nim_param->MB86A2x_Adr), 0x0a, &read_val ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	locking_info->STATE = read_val ;

	if( locking_info->STATE>=8 ) {
		if( locking_info->S8_REACH_his==0 ) {
			locking_info->S8_REACH = 1 ;
			locking_info->S8_REACH_his = 1 ;
		}
		else {
			locking_info->S8_REACH = 0 ;
		}
	}
	else {
		locking_info->S8_REACH = 0 ;
		locking_info->S8_REACH_his = 0 ;
	}

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Get_IF_level( MB86A2x_NIM_PARAM *nim_param, MB86A2x_AGC_INFO *agc_info )
{
	UINT8  read_val1, read_val2 ;
	UINT32 Status ;

	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x04, 0x41 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_read( (UINT8)(nim_param->MB86A2x_Adr), 0x05, &read_val1 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x04, 0x42 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_read( (UINT8)(nim_param->MB86A2x_Adr), 0x05, &read_val2 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	agc_info->IF_INPUT_LEVEL = (read_val1<<8) + read_val2 ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Get_IFAGCDAC( MB86A2x_NIM_PARAM *nim_param, MB86A2x_AGC_INFO *agc_info )
{
	UINT8  read_val ;
	UINT32 Status ;

	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x04, 0x3a ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_read( (UINT8)(nim_param->MB86A2x_Adr), 0x05, &read_val ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	agc_info->IFAGCDAC = read_val ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Get_RFAGCDAC( MB86A2x_NIM_PARAM *nim_param, MB86A2x_AGC_INFO *agc_info )
{
	UINT8  read_val ;
	UINT32 Status ;

	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x04, 0x3b ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_read( (UINT8)(nim_param->MB86A2x_Adr), 0x05, &read_val ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	agc_info->RFAGCDAC = read_val ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Get_STR_ppm_Error( MB86A2x_PARAM *param, MB86A2x_FREQ_ERR_INFO *freq_err_info )
{
	UINT8  read_val1, read_val2, read_val3 ;
	UINT32 value ;
	SINT32 temp_value ;
	UINT32 Status ;

	Status = MB86A2x_write( (UINT8)(param->nim_param.MB86A2x_Adr), 0x28, 0x2b ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	Status = MB86A2x_read( (UINT8)(param->nim_param.MB86A2x_Adr), 0x29, &read_val1 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_read( (UINT8)(param->nim_param.MB86A2x_Adr), 0x2a, &read_val2 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_read( (UINT8)(param->nim_param.MB86A2x_Adr), 0x2b, &read_val3 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	value = (read_val1<<16) + (read_val2<<8) + read_val3 ;

	temp_value = (SINT32)(value) - (SINT32)(param->recov_param.STR_OFS) ;
	if(     temp_value> 1678 ) temp_value =  1678 ;
	else if(temp_value<-1678 ) temp_value = -1678 ;

	freq_err_info->STR_ppm_error = temp_value*15625/262144 ;

	freq_err_info->STR_freq_error_Hz = (SINT32)(param->pll_param.INPUT_CLK_FREQ_kHz)*(freq_err_info->STR_ppm_error)/1000 ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Get_STR_Register_Value( MB86A2x_NIM_PARAM *nim_param, MB86A2x_TMCC_INFO *tmcc_info )
{
	UINT8  read_val1, read_val2, read_val3 ;
	UINT32 value ;
	UINT32 Status ;

	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x28, 0x2b ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	Status = MB86A2x_read( (UINT8)(nim_param->MB86A2x_Adr), 0x29, &read_val1 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_read( (UINT8)(nim_param->MB86A2x_Adr), 0x2a, &read_val2 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_read( (UINT8)(nim_param->MB86A2x_Adr), 0x2b, &read_val3 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	value = (read_val1<<16) + (read_val2<<8) + read_val3 ;

	tmcc_info->STR_current = value ;
	
	return MB86A2x_OK ;
}

MB86A2x_ERR_MSG MB86A2x_Get_CRR_Freq_Error( MB86A2x_PARAM *param, MB86A2x_FREQ_ERR_INFO *freq_err_info )
{
	UINT8  read_val1, read_val2, read_val3 ;
	SINT32 value ;
	SINT32 temp_value ;
	UINT32 Status ;

	Status = MB86A2x_write( (UINT8)(param->nim_param.MB86A2x_Adr), 0x28, 0x21 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	Status = MB86A2x_read( (UINT8)(param->nim_param.MB86A2x_Adr), 0x29, &read_val1 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_read( (UINT8)(param->nim_param.MB86A2x_Adr), 0x2a, &read_val2 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_read( (UINT8)(param->nim_param.MB86A2x_Adr), 0x2b, &read_val3 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	value = (read_val1<<16) + (read_val2<<8) + read_val3 ;
	if( value>=8388608 ) value = value - 16777216 ;

	temp_value = value - (SINT32)(param->recov_param.CRR_OFS) ;
	if(      temp_value>5600000 )  temp_value =  5600000 ;
	else if( temp_value<-5600000 ) temp_value = -5600000 ;

	freq_err_info->CRR_freq_error_kHz = (temp_value/32*(SINT32)(param->pll_param.MASTER_CLK_FREQ_kHz)) / 524288 ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Get_Mode_Guard( MB86A2x_NIM_PARAM *nim_param, MB86A2x_TMCC_INFO *tmcc_info )
{
	UINT8  read_val ;
	UINT32 Status ;

	Status = MB86A2x_read( (UINT8)(nim_param->MB86A2x_Adr), 0x07, &read_val ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	tmcc_info->MODE_current = (0x0c&read_val)>>2 ;
	tmcc_info->GUARD_current = 0x03&read_val ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Get_TMCC_Info( MB86A2x_NIM_PARAM *nim_param, MB86A2x_TMCC_INFO *tmcc_info )
{
	MB86A2x_ERR_MSG ERR_MSG ;

	ERR_MSG = MB86A2x_Get_TMCC_Sys_Emerg( nim_param, tmcc_info ) ;
	if( ERR_MSG==MB86A2x_ERR_INIT ) return MB86A2x_ERR_INIT ;

	ERR_MSG = MB86A2x_Get_TMCC_Part_Recep( nim_param, tmcc_info ) ;
	if( ERR_MSG==MB86A2x_ERR_INIT ) return MB86A2x_ERR_INIT ;

	ERR_MSG = MB86A2x_Get_TMCC_Mod_Lay_A( nim_param, tmcc_info ) ;
	if( ERR_MSG==MB86A2x_ERR_INIT ) return MB86A2x_ERR_INIT ;

	ERR_MSG = MB86A2x_Get_TMCC_Mod_Lay_B( nim_param, tmcc_info ) ;
	if( ERR_MSG==MB86A2x_ERR_INIT ) return MB86A2x_ERR_INIT ;

	ERR_MSG = MB86A2x_Get_TMCC_Mod_Lay_C( nim_param, tmcc_info ) ;
	if( ERR_MSG==MB86A2x_ERR_INIT ) return MB86A2x_ERR_INIT ;

	ERR_MSG = MB86A2x_Get_TMCC_Rate_Lay_A( nim_param, tmcc_info ) ;
	if( ERR_MSG==MB86A2x_ERR_INIT ) return MB86A2x_ERR_INIT ;

	ERR_MSG = MB86A2x_Get_TMCC_Rate_Lay_B( nim_param, tmcc_info ) ;
	if( ERR_MSG==MB86A2x_ERR_INIT ) return MB86A2x_ERR_INIT ;

	ERR_MSG = MB86A2x_Get_TMCC_Rate_Lay_C( nim_param, tmcc_info ) ;
	if( ERR_MSG==MB86A2x_ERR_INIT ) return MB86A2x_ERR_INIT ;

	ERR_MSG = MB86A2x_Get_TMCC_IL_Lay_A( nim_param, tmcc_info ) ;
	if( ERR_MSG==MB86A2x_ERR_INIT ) return MB86A2x_ERR_INIT ;

	ERR_MSG = MB86A2x_Get_TMCC_IL_Lay_B( nim_param, tmcc_info ) ;
	if( ERR_MSG==MB86A2x_ERR_INIT ) return MB86A2x_ERR_INIT ;

	ERR_MSG = MB86A2x_Get_TMCC_IL_Lay_C( nim_param, tmcc_info ) ;
	if( ERR_MSG==MB86A2x_ERR_INIT ) return MB86A2x_ERR_INIT ;

	ERR_MSG = MB86A2x_Get_TMCC_Num_Lay_A( nim_param, tmcc_info ) ;
	if( ERR_MSG==MB86A2x_ERR_INIT ) return MB86A2x_ERR_INIT ;

	ERR_MSG = MB86A2x_Get_TMCC_Num_Lay_B( nim_param, tmcc_info ) ;
	if( ERR_MSG==MB86A2x_ERR_INIT ) return MB86A2x_ERR_INIT ;

	ERR_MSG = MB86A2x_Get_TMCC_Num_Lay_C( nim_param, tmcc_info ) ;
	if( ERR_MSG==MB86A2x_ERR_INIT ) return MB86A2x_ERR_INIT ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Get_TMCC_Sys_Emerg( MB86A2x_NIM_PARAM *nim_param, MB86A2x_TMCC_INFO *tmcc_info )
{
	UINT8  read_val ;
	UINT32 Status ;

	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x6d, 0x84 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_read( (UINT8)(nim_param->MB86A2x_Adr), 0x6e, &read_val ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	tmcc_info->SYSTEM_current = (read_val&0x60)>>5 ;
	tmcc_info->EMERG_current  =  read_val&0x01 ;
	tmcc_info->CNTDN_current = (read_val&0x1e)>>1 ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Get_TMCC_Part_Recep( MB86A2x_NIM_PARAM *nim_param, MB86A2x_TMCC_INFO *tmcc_info )
{
	UINT8  read_val ;
	UINT32 Status ;

	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x6d, 0x85 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_read( (UINT8)(nim_param->MB86A2x_Adr), 0x6e, &read_val ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	tmcc_info->PARTIAL_current = (read_val&0x10)>>4 ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Get_TMCC_Mod_Lay_A( MB86A2x_NIM_PARAM *nim_param, MB86A2x_TMCC_INFO *tmcc_info )
{
	UINT8  read_val ;
	UINT32 Status ;

	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x6d, 0x86 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_read( (UINT8)(nim_param->MB86A2x_Adr), 0x6e, &read_val ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	tmcc_info->MOD_A_current = (read_val&0x70)>>4 ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Get_TMCC_Mod_Lay_B( MB86A2x_NIM_PARAM *nim_param, MB86A2x_TMCC_INFO *tmcc_info )
{
	UINT8  read_val ;
	UINT32 Status ;

	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x6d, 0x8a ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_read( (UINT8)(nim_param->MB86A2x_Adr), 0x6e, &read_val ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	tmcc_info->MOD_B_current = (read_val&0x70)>>4 ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Get_TMCC_Mod_Lay_C( MB86A2x_NIM_PARAM *nim_param, MB86A2x_TMCC_INFO *tmcc_info )
{
	UINT8  read_val ;
	UINT32 Status ;

	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x6d, 0x8e ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_read( (UINT8)(nim_param->MB86A2x_Adr), 0x6e, &read_val ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	tmcc_info->MOD_C_current = (read_val&0x70)>>4 ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Get_TMCC_Rate_Lay_A( MB86A2x_NIM_PARAM *nim_param, MB86A2x_TMCC_INFO *tmcc_info )
{
	UINT8  read_val ;
	UINT32 Status ;

	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x6d, 0x87 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_read( (UINT8)(nim_param->MB86A2x_Adr), 0x6e, &read_val ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	tmcc_info->RATE_A_current = (read_val&0x70)>>4 ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Get_TMCC_Rate_Lay_B( MB86A2x_NIM_PARAM *nim_param, MB86A2x_TMCC_INFO *tmcc_info )
{
	UINT8  read_val ;
	UINT32 Status ;

	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x6d, 0x8b ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_read( (UINT8)(nim_param->MB86A2x_Adr), 0x6e, &read_val ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	tmcc_info->RATE_B_current = (read_val&0x70)>>4 ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Get_TMCC_Rate_Lay_C( MB86A2x_NIM_PARAM *nim_param, MB86A2x_TMCC_INFO *tmcc_info )
{
	UINT8  read_val ;
	UINT32 Status ;

	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x6d, 0x8f ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_read( (UINT8)(nim_param->MB86A2x_Adr), 0x6e, &read_val ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	tmcc_info->RATE_C_current = (read_val&0x70)>>4 ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Get_TMCC_IL_Lay_A( MB86A2x_NIM_PARAM *nim_param, MB86A2x_TMCC_INFO *tmcc_info )
{
	UINT8  read_val ;
	UINT32 Status ;

	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x6d, 0x88 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_read( (UINT8)(nim_param->MB86A2x_Adr), 0x6e, &read_val ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	tmcc_info->IL_A_current = (read_val&0x70)>>4 ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Get_TMCC_IL_Lay_B( MB86A2x_NIM_PARAM *nim_param, MB86A2x_TMCC_INFO *tmcc_info )
{
	UINT8  read_val ;
	UINT32 Status ;

	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x6d, 0x8c ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_read( (UINT8)(nim_param->MB86A2x_Adr), 0x6e, &read_val ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	tmcc_info->IL_B_current = (read_val&0x70)>>4 ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Get_TMCC_IL_Lay_C( MB86A2x_NIM_PARAM *nim_param, MB86A2x_TMCC_INFO *tmcc_info )
{
	UINT8  read_val ;
	UINT32 Status ;

	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x6d, 0x90 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_read( (UINT8)(nim_param->MB86A2x_Adr), 0x6e, &read_val ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	tmcc_info->IL_C_current = (read_val&0x70)>>4 ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Get_TMCC_Num_Lay_A( MB86A2x_NIM_PARAM *nim_param, MB86A2x_TMCC_INFO *tmcc_info )
{
	UINT8  read_val ;
	UINT32 Status ;

	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x6d, 0x89 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_read( (UINT8)(nim_param->MB86A2x_Adr), 0x6e, &read_val ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	tmcc_info->NUM_A_current = (read_val&0xf0)>>4 ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Get_TMCC_Num_Lay_B( MB86A2x_NIM_PARAM *nim_param, MB86A2x_TMCC_INFO *tmcc_info )
{
	UINT8  read_val ;
	UINT32 Status ;

	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x6d, 0x8d ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_read( (UINT8)(nim_param->MB86A2x_Adr), 0x6e, &read_val ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	tmcc_info->NUM_B_current = (read_val&0xf0)>>4 ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Get_TMCC_Num_Lay_C( MB86A2x_NIM_PARAM *nim_param, MB86A2x_TMCC_INFO *tmcc_info )
{
	UINT8  read_val ;
	UINT32 Status ;

	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x6d, 0x91 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_read( (UINT8)(nim_param->MB86A2x_Adr), 0x6e, &read_val ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	tmcc_info->NUM_C_current = (read_val&0xf0)>>4 ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Reset_VBER_A_Counter( MB86A2x_NIM_PARAM *nim_param )
{
	UINT32 Status ;

	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x53, 0x06 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x53, 0x07 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Reset_VBER_B_Counter( MB86A2x_NIM_PARAM *nim_param )
{
	UINT32 Status ;

	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x53, 0x05 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x53, 0x07 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Reset_VBER_C_Counter( MB86A2x_NIM_PARAM *nim_param )
{
	UINT32 Status ;

	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x53, 0x03 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x53, 0x07 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Reset_VBER_ALL_Counter( MB86A2x_NIM_PARAM *nim_param )
{
	UINT32 Status ;

	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x53, 0x00 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x53, 0x07 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Get_VBER_Flag( MB86A2x_NIM_PARAM *nim_param, MB86A2x_SIG_QUAL_INFO *sig_qual_info )
{
	UINT8  read_val ;
	UINT32 Status ;

	Status = MB86A2x_read( (UINT8)(nim_param->MB86A2x_Adr), 0x54, &read_val ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	sig_qual_info->VBER_A_update =  read_val&0x01 ;
	sig_qual_info->VBER_B_update = (read_val&0x02)>>1 ;
	sig_qual_info->VBER_C_update = (read_val&0x04)>>2 ;

	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Get_VBER_A( MB86A2x_NIM_PARAM *nim_param, MB86A2x_MON_TIM_PARAM *mon_tim_param, MB86A2x_SIG_QUAL_INFO *sig_qual_info )
{
	UINT8  read_val1, read_val2, read_val3 ;
	UINT32 Status ;

	Status = MB86A2x_read( (UINT8)(nim_param->MB86A2x_Adr), 0x55, &read_val1 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_read( (UINT8)(nim_param->MB86A2x_Adr), 0x56, &read_val2 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_read( (UINT8)(nim_param->MB86A2x_Adr), 0x57, &read_val3 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	sig_qual_info->V_ERR_A = (read_val1<<16) + (read_val2<<8) + read_val3 ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Get_VBER_B( MB86A2x_NIM_PARAM *nim_param, MB86A2x_MON_TIM_PARAM *mon_tim_param, MB86A2x_SIG_QUAL_INFO *sig_qual_info )
{
	UINT8  read_val1, read_val2, read_val3 ;
	UINT32 Status ;

	Status = MB86A2x_read( (UINT8)(nim_param->MB86A2x_Adr), 0x58, &read_val1 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_read( (UINT8)(nim_param->MB86A2x_Adr), 0x59, &read_val2 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_read( (UINT8)(nim_param->MB86A2x_Adr), 0x5a, &read_val3 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	sig_qual_info->V_ERR_B = (read_val1<<16) + (read_val2<<8) + read_val3 ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Get_VBER_C( MB86A2x_NIM_PARAM *nim_param, MB86A2x_MON_TIM_PARAM *mon_tim_param, MB86A2x_SIG_QUAL_INFO *sig_qual_info )
{
	UINT8  read_val1, read_val2, read_val3 ;
	UINT32 Status ;

	Status = MB86A2x_read( (UINT8)(nim_param->MB86A2x_Adr), 0x5b, &read_val1 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_read( (UINT8)(nim_param->MB86A2x_Adr), 0x5c, &read_val2 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_read( (UINT8)(nim_param->MB86A2x_Adr), 0x5d, &read_val3 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	sig_qual_info->V_ERR_C = (read_val1<<16) + (read_val2<<8) + read_val3 ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Reset_SBER_A_Counter( MB86A2x_NIM_PARAM *nim_param )
{
	UINT32 Status ;

	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x5f, 0x06 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x5f, 0x07 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Reset_SBER_B_Counter( MB86A2x_NIM_PARAM *nim_param )
{
	UINT32 Status ;

	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x5f, 0x05 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x5f, 0x07 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Reset_SBER_C_Counter( MB86A2x_NIM_PARAM *nim_param )
{
	UINT32 Status ;

	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x5f, 0x03 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x5f, 0x07 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Reset_SBER_ALL_Counter( MB86A2x_NIM_PARAM *nim_param )
{
	UINT32 Status ;

	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x5f, 0x00 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x5f, 0x07 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Get_SBER_Flag( MB86A2x_NIM_PARAM *nim_param, MB86A2x_SIG_QUAL_INFO *sig_qual_info )
{
	UINT8  read_val ;
	UINT32 Status ;

	Status = MB86A2x_read( (UINT8)(nim_param->MB86A2x_Adr), 0x60, &read_val ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	sig_qual_info->SBER_A_update =  read_val&0x01 ;
	sig_qual_info->SBER_B_update = (read_val&0x02)>>1 ;
	sig_qual_info->SBER_C_update = (read_val&0x04)>>2 ;

	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Get_SBER_A( MB86A2x_NIM_PARAM *nim_param, MB86A2x_MON_TIM_PARAM *mon_tim_param, MB86A2x_SIG_QUAL_INFO *sig_qual_info )
{
	UINT8  read_val1, read_val2, read_val3 ;
	UINT32 Status ;

	Status = MB86A2x_read( (UINT8)(nim_param->MB86A2x_Adr), 0x64, &read_val1 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_read( (UINT8)(nim_param->MB86A2x_Adr), 0x65, &read_val2 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_read( (UINT8)(nim_param->MB86A2x_Adr), 0x66, &read_val3 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	sig_qual_info->S_ERR_A = (read_val1<<16) + (read_val2<<8) + read_val3 ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Get_SBER_B( MB86A2x_NIM_PARAM *nim_param, MB86A2x_MON_TIM_PARAM *mon_tim_param, MB86A2x_SIG_QUAL_INFO *sig_qual_info )
{
	UINT8  read_val1, read_val2, read_val3 ;
	UINT32 Status ;

	Status = MB86A2x_read( (UINT8)(nim_param->MB86A2x_Adr), 0x67, &read_val1 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_read( (UINT8)(nim_param->MB86A2x_Adr), 0x68, &read_val2 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_read( (UINT8)(nim_param->MB86A2x_Adr), 0x69, &read_val3 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	sig_qual_info->S_ERR_B = (read_val1<<16) + (read_val2<<8) + read_val3 ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Get_SBER_C( MB86A2x_NIM_PARAM *nim_param, MB86A2x_MON_TIM_PARAM *mon_tim_param, MB86A2x_SIG_QUAL_INFO *sig_qual_info )
{
	UINT8  read_val1, read_val2, read_val3 ;
	UINT32 Status ;

	Status = MB86A2x_read( (UINT8)(nim_param->MB86A2x_Adr), 0x6a, &read_val1 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_read( (UINT8)(nim_param->MB86A2x_Adr), 0x6b, &read_val2 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_read( (UINT8)(nim_param->MB86A2x_Adr), 0x6c, &read_val3 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	sig_qual_info->S_ERR_C = (read_val1<<16) + (read_val2<<8) + read_val3 ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Reset_PBER_A_Counter( MB86A2x_NIM_PARAM *nim_param )
{
	UINT32 Status ;

	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x50, 0xb1 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x51, 0x01 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x51, 0x00 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Reset_PBER_B_Counter( MB86A2x_NIM_PARAM *nim_param )
{
	UINT32 Status ;

	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x50, 0xb1 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x51, 0x02 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x51, 0x00 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Reset_PBER_C_Counter( MB86A2x_NIM_PARAM *nim_param )
{
	UINT32 Status ;

	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x50, 0xb1 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x51, 0x04 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x51, 0x00 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Reset_PBER_ALL_Counter( MB86A2x_NIM_PARAM *nim_param )
{
	UINT32 Status ;

	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x50, 0xb1 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x51, 0x07 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x51, 0x00 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Get_PBER_Flag( MB86A2x_NIM_PARAM *nim_param, MB86A2x_SIG_QUAL_INFO *sig_qual_info )
{
	UINT8  read_val ;
	UINT32 Status ;

	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x50, 0xb8 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_read( (UINT8)(nim_param->MB86A2x_Adr), 0x51, &read_val ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	sig_qual_info->PBER_A_update =  read_val&0x01 ;
	sig_qual_info->PBER_B_update = (read_val&0x02)>>1 ;
	sig_qual_info->PBER_C_update = (read_val&0x04)>>2 ;

	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Get_PBER_A( MB86A2x_NIM_PARAM *nim_param, MB86A2x_MON_TIM_PARAM *mon_tim_param, MB86A2x_SIG_QUAL_INFO *sig_qual_info )
{
	UINT8  read_val1, read_val2 ;
	UINT32 Status ;

	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x50, 0xb9 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_read( (UINT8)(nim_param->MB86A2x_Adr), 0x51, &read_val1 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x50, 0xba ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_read( (UINT8)(nim_param->MB86A2x_Adr), 0x51, &read_val2 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	sig_qual_info->P_ERR_A = (read_val1<<8) + read_val2 ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Get_PBER_B( MB86A2x_NIM_PARAM *nim_param, MB86A2x_MON_TIM_PARAM *mon_tim_param, MB86A2x_SIG_QUAL_INFO *sig_qual_info )
{
	UINT8  read_val1, read_val2 ;
	UINT32 Status ;

	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x50, 0xbb ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_read( (UINT8)(nim_param->MB86A2x_Adr), 0x51, &read_val1 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x50, 0xbc ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_read( (UINT8)(nim_param->MB86A2x_Adr), 0x51, &read_val2 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	sig_qual_info->P_ERR_B = (read_val1<<8) + read_val2 ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Get_PBER_C( MB86A2x_NIM_PARAM *nim_param, MB86A2x_MON_TIM_PARAM *mon_tim_param, MB86A2x_SIG_QUAL_INFO *sig_qual_info )
{
	UINT8  read_val1, read_val2 ;
	UINT32 Status ;

	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x50, 0xbd ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_read( (UINT8)(nim_param->MB86A2x_Adr), 0x51, &read_val1 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x50, 0xbe ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_read( (UINT8)(nim_param->MB86A2x_Adr), 0x51, &read_val2 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	sig_qual_info->P_ERR_C = (read_val1<<8) + read_val2 ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Reset_CN_Counter( MB86A2x_NIM_PARAM *nim_param, MB86A2x_MON_TIM_PARAM *mon_tim_param )
{
	UINT32 Status ;

	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x45, (UINT8)(0x10|mon_tim_param->CN_SYMBOL) ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x45, (UINT8)(0x00|mon_tim_param->CN_SYMBOL) ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Get_CN_Flag( MB86A2x_NIM_PARAM *nim_param, MB86A2x_SIG_QUAL_INFO *sig_qual_info )
{
	UINT8  read_val ;
	UINT32 Status ;

	Status = MB86A2x_read( (UINT8)(nim_param->MB86A2x_Adr), 0x45, &read_val ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	sig_qual_info->CN_update = (read_val & 0x40)>>6 ; ;

	return MB86A2x_OK ;

}


MB86A2x_ERR_MSG MB86A2x_Get_CN_Value( MB86A2x_NIM_PARAM *nim_param, MB86A2x_SIG_QUAL_INFO *sig_qual_info )
{
	UINT8  read_val1, read_val2 ;
	UINT32 Status ;

	Status = MB86A2x_read( (UINT8)(nim_param->MB86A2x_Adr), 0x46, &read_val1 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_read( (UINT8)(nim_param->MB86A2x_Adr), 0x47, &read_val2 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	sig_qual_info->CN_MON = (read_val1<<8) + read_val2 ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Reset_MER_Counter( MB86A2x_NIM_PARAM *nim_param )
{
	UINT32 Status ;

	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x50, 0x50 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x51, 0x03 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x51, 0x02 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Get_MER_Flag( MB86A2x_NIM_PARAM *nim_param, MB86A2x_SIG_QUAL_INFO *sig_qual_info )
{
	UINT8  read_val ;
	UINT32 Status ;

	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x50, 0x5b ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_read( (UINT8)(nim_param->MB86A2x_Adr), 0x51, &read_val ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	sig_qual_info->MER_update = read_val & 0x01 ;

	return MB86A2x_OK ;

}


MB86A2x_ERR_MSG MB86A2x_Get_MER_A_Value( MB86A2x_NIM_PARAM *nim_param, MB86A2x_TMCC_INFO *tmcc_info, MB86A2x_SIG_QUAL_INFO *sig_qual_info )
{
	UINT8  read_val1, read_val2 ;
	UINT32 Status ;

	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x50, 0x52 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_read( (UINT8)(nim_param->MB86A2x_Adr), 0x51, &read_val1 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x50, 0x53 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_read( (UINT8)(nim_param->MB86A2x_Adr), 0x51, &read_val2 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	sig_qual_info->MER_MON_A = (read_val1<<8) + read_val2 ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Get_MER_B_Value( MB86A2x_NIM_PARAM *nim_param, MB86A2x_TMCC_INFO *tmcc_info, MB86A2x_SIG_QUAL_INFO *sig_qual_info )
{
	UINT8  read_val1, read_val2 ;
	UINT32 Status ;

	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x50, 0x55 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_read( (UINT8)(nim_param->MB86A2x_Adr), 0x51, &read_val1 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x50, 0x56 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_read( (UINT8)(nim_param->MB86A2x_Adr), 0x51, &read_val2 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	sig_qual_info->MER_MON_B = (read_val1<<8) + read_val2 ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Get_MER_C_Value( MB86A2x_NIM_PARAM *nim_param, MB86A2x_TMCC_INFO *tmcc_info, MB86A2x_SIG_QUAL_INFO *sig_qual_info )
{
	UINT8  read_val1, read_val2 ;
	UINT32 Status ;

	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x50, 0x58 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_read( (UINT8)(nim_param->MB86A2x_Adr), 0x51, &read_val1 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x50, 0x59 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_read( (UINT8)(nim_param->MB86A2x_Adr), 0x51, &read_val2 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	sig_qual_info->MER_MON_C = (read_val1<<8) + read_val2 ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Set_Init_CH_SCH( MB86A2x_PARAM *param, MB86A2x_CH_SCH_INFO *ch_sch_info, int RS_EN, MB86A2x_VERSION_INFO *version_info )
{
	MB86A2x_ERR_MSG ERR_MSG ;

	ERR_MSG = MB86A2x_Set_Init( param, RS_EN, version_info ) ;
	if( ERR_MSG==MB86A2x_ERR_INIT ) return MB86A2x_ERR_INIT ;

	ERR_MSG = MB86A2x_Set_Monitor_Timer( param ) ;
	if( ERR_MSG==MB86A2x_ERR_INIT ) return MB86A2x_ERR_INIT ;

	ERR_MSG = MB86A2x_Reset_Monitor_Timer( param ) ;
	if( ERR_MSG==MB86A2x_ERR_INIT ) return MB86A2x_ERR_INIT ;

	ERR_MSG = MB86A2x_Set_IRQ_All_Off( &param->nim_param ) ;
	if( ERR_MSG==MB86A2x_ERR_INIT ) return MB86A2x_ERR_INIT ;

	ERR_MSG = MB86A2x_Set_IRQ_CH_SCH_END_Off( &param->nim_param, 0 ) ;
	if( ERR_MSG==MB86A2x_ERR_INIT ) return MB86A2x_ERR_INIT ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Init_CH_SCH_Result( MB86A2x_CH_SCH_INFO *ch_sch_info )
{
	UINT32 i ;

	ch_sch_info->Detected_ch_num = 0 ;

	for( i=0 ; i<70 ; i++ ) {
		ch_sch_info->CH_num[i] = 0 ;
		ch_sch_info->MODE[i] = 7 ;
		ch_sch_info->GUARD[i] = 7 ;
		ch_sch_info->MOD_A[i] = 7 ;
		ch_sch_info->MOD_B[i] = 7 ;
		ch_sch_info->MOD_C[i] = 7 ;
		ch_sch_info->RATE_A[i] = 7 ;
		ch_sch_info->RATE_B[i] = 7 ;
		ch_sch_info->RATE_C[i] = 7 ;
		ch_sch_info->IL_A[i] = 32 ;
		ch_sch_info->IL_B[i] = 32 ;
		ch_sch_info->IL_C[i] = 32 ;
		ch_sch_info->NUM_A[i] = 15 ;
		ch_sch_info->NUM_B[i] = 15 ;
		ch_sch_info->NUM_C[i] = 15 ;
		ch_sch_info->STR[i] = 0 ;
	}

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Start_CH_SCH( MB86A2x_NIM_PARAM *nim_param )
{
	UINT32 Status ;

	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0xe7, 0x01 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0xe6, 0x20 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0xe6, 0x21 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Get_CH_SCH_Status( MB86A2x_NIM_PARAM *nim_param, MB86A2x_CH_SCH_INFO *ch_sch_info )
{
	MB86A2x_ERR_MSG ERR_MSG ;
	UINT8  chend ;

	ERR_MSG = MB86A2x_Get_IRQ_CH_SCH_END( nim_param, &chend ) ;
	if( ERR_MSG==MB86A2x_ERR_INIT ) return MB86A2x_ERR_INIT ;

	ch_sch_info->CH_SCH_status = chend ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Get_CH_SCH_Result( MB86A2x_NIM_PARAM *nim_param, MB86A2x_CH_SCH_INFO *ch_sch_info )
{
	UINT32 Status ;
	UINT8  read_val ;

	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0xe8, 0x00 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_read( (UINT8)(nim_param->MB86A2x_Adr), 0xe9, &read_val ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	ch_sch_info->CH_SCH_result = read_val &0x01 ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Store_CH_SCH_Result( UINT32 ch, MB86A2x_TMCC_INFO *tmcc_info, MB86A2x_CH_SCH_INFO *ch_sch_info )
{
	ch_sch_info->CH_num[ch_sch_info->Detected_ch_num] = ch ;
	
	ch_sch_info->MODE[ch_sch_info->Detected_ch_num] = tmcc_info->MODE_current ;
	ch_sch_info->GUARD[ch_sch_info->Detected_ch_num] = tmcc_info->GUARD_current ;

	ch_sch_info->MOD_A[ch_sch_info->Detected_ch_num] = tmcc_info->MOD_A_current ;
	ch_sch_info->MOD_B[ch_sch_info->Detected_ch_num] = tmcc_info->MOD_B_current ;
	ch_sch_info->MOD_C[ch_sch_info->Detected_ch_num] = tmcc_info->MOD_C_current ;

	ch_sch_info->RATE_A[ch_sch_info->Detected_ch_num] = tmcc_info->RATE_A_current ;
	ch_sch_info->RATE_B[ch_sch_info->Detected_ch_num] = tmcc_info->RATE_B_current ;
	ch_sch_info->RATE_C[ch_sch_info->Detected_ch_num] = tmcc_info->RATE_C_current ;

	ch_sch_info->IL_A[ch_sch_info->Detected_ch_num] = tmcc_info->IL_A_current ;
	ch_sch_info->IL_B[ch_sch_info->Detected_ch_num] = tmcc_info->IL_B_current ;
	ch_sch_info->IL_C[ch_sch_info->Detected_ch_num] = tmcc_info->IL_C_current ;

	ch_sch_info->NUM_A[ch_sch_info->Detected_ch_num] = tmcc_info->NUM_A_current ;
	ch_sch_info->NUM_B[ch_sch_info->Detected_ch_num] = tmcc_info->NUM_B_current ;
	ch_sch_info->NUM_C[ch_sch_info->Detected_ch_num] = tmcc_info->NUM_C_current ;

	ch_sch_info->STR[ch_sch_info->Detected_ch_num] = tmcc_info->STR_current ;

	ch_sch_info->Detected_ch_num++ ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Save_CH_SCH_Result( MB86A2x_CH_SCH_INFO *ch_sch_info )
{
	/*
	UINT8 i ;
	FILE *f1 ;
	f1 = fopen( "Channel_Search_Result.txt", "wt" ) ;

	fprintf( f1, "Detected_ch_num=%d\n", ch_sch_info->Detected_ch_num ) ;
	for( i=0 ; i<ch_sch_info->Detected_ch_num ; i++ ) {
		fprintf( f1, "CH_num=%d,", ch_sch_info->CH_num[i] ) ;
		fprintf( f1, "MODE=%d,", ch_sch_info->MODE[i] ) ;
		fprintf( f1, "GUARD=%d,", ch_sch_info->GUARD[i] ) ;

		fprintf( f1, "MOD_A=%d,", ch_sch_info->MOD_A[i] ) ;
		fprintf( f1, "RATE_A=%d,", ch_sch_info->RATE_A[i] ) ;
		fprintf( f1, "IL_A=%d,", ch_sch_info->IL_A[i] ) ;
		fprintf( f1, "NUM_A=%d,", ch_sch_info->NUM_A[i] ) ;

		fprintf( f1, "MOD_B=%d,", ch_sch_info->MOD_B[i] ) ;
		fprintf( f1, "RATE_B=%d,", ch_sch_info->RATE_B[i] ) ;
		fprintf( f1, "IL_B=%d,", ch_sch_info->IL_B[i] ) ;
		fprintf( f1, "NUM_B=%d,", ch_sch_info->NUM_B[i] ) ;

		fprintf( f1, "MOD_C=%d,", ch_sch_info->MOD_C[i] ) ;
		fprintf( f1, "RATE_C=%d,", ch_sch_info->RATE_C[i] ) ;
		fprintf( f1, "IL_C=%d,", ch_sch_info->IL_C[i] ) ;
		fprintf( f1, "NUM_C=%d,", ch_sch_info->NUM_C[i] ) ;

		fprintf( f1, "STR=%d", ch_sch_info->STR[i] ) ;

		fprintf( f1, "\n" ) ;
	}
	fclose( f1 ) ;
*/
int i;
	for( i=0 ; i<ch_sch_info->Detected_ch_num ; i++ ) {
		DBG("CH_num=%d,", ch_sch_info->CH_num[i] ) ;
		DBG("MODE=%d,", ch_sch_info->MODE[i] ) ;
		DBG("GUARD=%d,", ch_sch_info->GUARD[i] ) ;

		DBG("MOD_A=%d,", ch_sch_info->MOD_A[i] ) ;
		DBG("RATE_A=%d,", ch_sch_info->RATE_A[i] ) ;
		DBG("IL_A=%d,", ch_sch_info->IL_A[i] ) ;
		DBG("NUM_A=%d,", ch_sch_info->NUM_A[i] ) ;

		DBG("MOD_B=%d,", ch_sch_info->MOD_B[i] ) ;
		DBG("RATE_B=%d,", ch_sch_info->RATE_B[i] ) ;
		DBG("IL_B=%d,", ch_sch_info->IL_B[i] ) ;
		DBG("NUM_B=%d,", ch_sch_info->NUM_B[i] ) ;

		DBG("MOD_C=%d,", ch_sch_info->MOD_C[i] ) ;
		DBG("RATE_C=%d,", ch_sch_info->RATE_C[i] ) ;
		DBG("IL_C=%d,", ch_sch_info->IL_C[i] ) ;
		DBG("NUM_C=%d,", ch_sch_info->NUM_C[i] ) ;

		DBG("STR=%d", ch_sch_info->STR[i] ) ;
	}
	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Set_Power_Mode( MB86A2x_NIM_PARAM *nim_param, MB86A2x_AGC_PARAM *agc_param ) {

	MB86A2x_ERR_MSG ERR_MSG ;

	if( nim_param->POWER_DOWN==0 ) {
		ERR_MSG = MB86A2x_Set_Normal_Power( nim_param, agc_param ) ;	//Power Down Setting
		if( ERR_MSG==MB86A2x_ERR_INIT ) return MB86A2x_ERR_INIT ;
	}
	else {
		MB86A2x_Set_Low_Power( nim_param ) ;
	}

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Set_Low_Power( MB86A2x_NIM_PARAM *nim_param )
{
	UINT32 Status ;

	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0xd0, 0x33 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x08, 0x01 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x73, 0x01 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Set_Normal_Power( MB86A2x_NIM_PARAM *nim_param, MB86A2x_AGC_PARAM *agc_param )
{
	UINT32 Status ;
	UINT8  write_val ;
	UINT8  RFAGCDAC_OFF ;
	UINT8  ADCQ_OFF ;

	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x73, 0x00 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x08, 0x00 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
    
	if( nim_param->tuner_param.IF_freq_kHz==MB86A2x_IF_FREQ_00_000kHz ) {
		ADCQ_OFF = 0 ;
	}
	else {
		ADCQ_OFF = 1 ;
	}
	if( agc_param->RFAGC_ON==1 ) {
		RFAGCDAC_OFF = 0 ;
	}
	else {
		RFAGCDAC_OFF = 1 ;
	}
	write_val = 0x00 | (RFAGCDAC_OFF<<5) | (ADCQ_OFF<<2);

	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0xd0, write_val ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Set_GPIO_Mode( MB86A2x_NIM_PARAM *nim_param, UINT8 Port_No, UINT8 Port_mode )
{
	UINT32 Status ;
	UINT8  read_val ;
	UINT8  write_val ;

	Status = MB86A2x_read( (UINT8)(nim_param->MB86A2x_Adr), 0xec, &read_val ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	if( Port_No==0 ) {
		write_val = read_val & 0xfe | Port_mode ;
	}
	else if( Port_No==1 ) {
		write_val = read_val & 0xfd | (Port_mode<<1) ;
	}
	else if( Port_No==2 ) {
		write_val = read_val & 0xfb | (Port_mode<<2) ;
	}
	else {
		write_val = read_val & 0xf7 | (Port_mode<<3) ;
	}

	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0xec, write_val ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Get_GPIO_Mode( MB86A2x_NIM_PARAM *nim_param, UINT8 Port_No, UINT8 *Port_mode )
{
	UINT32 Status ;
	UINT8  read_val ;

	Status = MB86A2x_read( (UINT8)(nim_param->MB86A2x_Adr), 0xec, &read_val ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	if( Port_No==0 ) {
		*Port_mode = read_val&0x01 ;
	}
	else if( Port_No==1 ) {
		*Port_mode = (read_val&0x02)>>1 ;
	}
	else if( Port_No==2 ) {
		*Port_mode = (read_val&0x04)>>2 ;
	}
	else {
		*Port_mode = (read_val&0x08)>>3 ;
	}

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Set_GPIO_Value( MB86A2x_NIM_PARAM *nim_param, UINT8 Port_No, UINT8 Port_val )
{
	UINT32 Status ;
	UINT8  read_val ;
	UINT8  write_val ;

	Status = MB86A2x_read( (UINT8)(nim_param->MB86A2x_Adr), 0xeb, &read_val ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	if( Port_No==0 ) {
		write_val = read_val & 0x0e | Port_val ;
	}
	else if( Port_No==1 ) {
		write_val = read_val & 0x0d | (Port_val<<1) ;
	}
	else if( Port_No==2 ) {
		write_val = read_val & 0x0b | (Port_val<<2) ;
	}
	else {
		write_val = read_val & 0x07 | (Port_val<<3) ;
	}

	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0xeb, write_val ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Get_GPIO_Value( MB86A2x_NIM_PARAM *nim_param, UINT8 Port_No, UINT8 *Port_val )
{
	UINT32 Status ;
	UINT8  read_val ;

	Status = MB86A2x_read( (UINT8)(nim_param->MB86A2x_Adr), 0xeb, &read_val ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	if( Port_No==0 ) {
		*Port_val = read_val&0x01 ;
	}
	else if( Port_No==1 ) {
		*Port_val = (read_val&0x02)>>1 ;
	}
	else if( Port_No==2 ) {
		*Port_val = (read_val&0x04)>>2 ;
	}
	else {
		*Port_val = (read_val&0x08)>>3 ;
	}

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Set_IRQ_All_Off( MB86A2x_NIM_PARAM *nim_param )
{
	UINT32 Status ;
	UINT8 temp ;

	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x6d, 0x35 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x6e, 0x07 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x50, 0xda ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x51, 0xff ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x50, 0xdb ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x51, 0x07 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0xee, 0x03 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	Status = MB86A2x_read( (UINT8)(nim_param->MB86A2x_Adr), 0xec, &temp ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	temp = 0xf0 | temp ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0xec, temp ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	
	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Set_IRQ_TMCC_EMG_Off( MB86A2x_NIM_PARAM *nim_param, UINT8 disable )
{
	UINT32 Status ;
	UINT8 temp ;
	MB86A2x_ERR_MSG ERR_MSG ;

	ERR_MSG = MB86A2x_Set_IRQ_TMCC_EMG_INV( nim_param, 0 ) ;
	if( ERR_MSG==MB86A2x_ERR_INIT ) return MB86A2x_ERR_INIT ;

	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x6d, 0x35 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_read( (UINT8)(nim_param->MB86A2x_Adr), 0x6e, &temp ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	temp = (temp&0x03)|(disable<<2) ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x6d, 0x35 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x6e, temp ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	
	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Reset_IRQ_TMCC_EMG( MB86A2x_NIM_PARAM *nim_param )
{
	UINT32 Status ;
	UINT8 temp1, temp2 ;

	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x6d, 0x36 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_read( (UINT8)(nim_param->MB86A2x_Adr), 0x6e, &temp1 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	temp2 = temp1 | 0x04 ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x6d, 0x36 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x6e, temp2 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	temp2 = temp1 & 0x03 ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x6d, 0x36 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x6e, temp2 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Set_IRQ_TMCC_EMG_INV( MB86A2x_NIM_PARAM *nim_param, UINT8 inv )
{
	UINT32 Status ;

	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x6d, 0x38 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x6e, (UINT8)((inv<<4)|0x01) ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Get_IRQ_TMCC_EMG( MB86A2x_NIM_PARAM *nim_param, UINT8 *irq_tmcc_emg )
{
	UINT32 Status ;
	UINT8 temp ;

	Status = MB86A2x_read( (UINT8)(nim_param->MB86A2x_Adr), 0x63, &temp ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	*irq_tmcc_emg = (temp&0x04)>>2 ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Set_IRQ_TMCC_CNTDN_Off( MB86A2x_NIM_PARAM *nim_param, UINT8 disable )
{
	UINT32 Status ;
	UINT8 temp ;

	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x6d, 0x35 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_read( (UINT8)(nim_param->MB86A2x_Adr), 0x6e, &temp ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	temp = (temp&0x05)|(disable<<1) ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x6d, 0x35 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x6e, temp ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	
	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Reset_IRQ_TMCC_CNTDN( MB86A2x_NIM_PARAM *nim_param )
{
	UINT32 Status ;
	UINT8 temp1, temp2 ;

	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x6d, 0x36 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_read( (UINT8)(nim_param->MB86A2x_Adr), 0x6e, &temp1 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	temp2 = temp1 | 0x02 ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x6d, 0x36 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x6e, temp2 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	temp2 = temp1 & 0x05 ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x6d, 0x36 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x6e, temp2 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Get_IRQ_TMCC_CNTDN( MB86A2x_NIM_PARAM *nim_param, UINT8 *irq_tmcc_cntdn )
{
	UINT32 Status ;
	UINT8 temp ;

	Status = MB86A2x_read( (UINT8)(nim_param->MB86A2x_Adr), 0x63, &temp ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	*irq_tmcc_cntdn = (temp&0x02)>>1 ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Set_IRQ_SBER_A_FIN_Off( MB86A2x_NIM_PARAM *nim_param, UINT8 disable )
{
	UINT32 Status ;
	UINT8 temp ;

	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x50, 0xda ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_read( (UINT8)(nim_param->MB86A2x_Adr), 0x51, &temp ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	temp = (temp&0xfe)|disable ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x50, 0xda ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x51, temp ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	
	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Set_IRQ_SBER_B_FIN_Off( MB86A2x_NIM_PARAM *nim_param, UINT8 disable )
{
	UINT32 Status ;
	UINT8 temp ;

	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x50, 0xda ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_read( (UINT8)(nim_param->MB86A2x_Adr), 0x51, &temp ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	temp = (temp&0xfd)|(disable<<1) ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x50, 0xda ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x51, temp ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	
	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Set_IRQ_SBER_C_FIN_Off( MB86A2x_NIM_PARAM *nim_param, UINT8 disable )
{
	UINT32 Status ;
	UINT8 temp ;

	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x50, 0xda ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_read( (UINT8)(nim_param->MB86A2x_Adr), 0x51, &temp ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	temp = (temp&0xfb)|(disable<<2) ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x50, 0xda ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x51, temp ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	
	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Set_IRQ_SBER_A_TH_Off( MB86A2x_NIM_PARAM *nim_param, UINT8 disable )
{
	UINT32 Status ;
	UINT8 temp ;

	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x50, 0xda ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_read( (UINT8)(nim_param->MB86A2x_Adr), 0x51, &temp ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	temp = (temp&0xf7)|(disable<<3) ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x50, 0xda ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x51, temp ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	
	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Set_THRES_SBER_A( MB86A2x_NIM_PARAM *nim_param, UINT32 thres_a )
{
	UINT32 Status ;

	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x50, 0xe2 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x51, (UINT8)((thres_a&0x0ff0000)>>16) ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x50, 0xe3 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x51, (UINT8)((thres_a&0x000ff00)>>8) ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x50, 0xe4 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x51, (UINT8)(thres_a&0x00000ff) ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Get_IRQ_SBER_THRES_A( MB86A2x_NIM_PARAM *nim_param, UINT8 *irq_sber_thres_a )
{
	UINT32 Status ;
	UINT8 temp ;

	Status = MB86A2x_read( (UINT8)(nim_param->MB86A2x_Adr), 0x6f, &temp ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	*irq_sber_thres_a = temp&0x01 ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Set_IRQ_SBER_B_TH_Off( MB86A2x_NIM_PARAM *nim_param, UINT8 disable )
{
	UINT32 Status ;
	UINT8 temp ;

	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x50, 0xda ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_read( (UINT8)(nim_param->MB86A2x_Adr), 0x51, &temp ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	temp = (temp&0xef)|(disable<<4) ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x50, 0xda ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x51, temp ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	
	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Set_THRES_SBER_B( MB86A2x_NIM_PARAM *nim_param, UINT32 thres_b )
{
	UINT32 Status ;

	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x50, 0xe5 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x51, (UINT8)((thres_b&0x0ff0000)>>16) ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x50, 0xe6 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x51, (UINT8)((thres_b&0x000ff00)>>8) ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x50, 0xe7 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x51, (UINT8)(thres_b&0x00000ff) ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Get_IRQ_SBER_THRES_B( MB86A2x_NIM_PARAM *nim_param, UINT8 *irq_sber_thres_b )
{
	UINT32 Status ;
	UINT8 temp ;

	Status = MB86A2x_read( (UINT8)(nim_param->MB86A2x_Adr), 0x6f, &temp ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	*irq_sber_thres_b = (temp&0x02)>>1 ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Set_IRQ_SBER_C_TH_Off( MB86A2x_NIM_PARAM *nim_param, UINT8 disable )
{
	UINT32 Status ;
	UINT8 temp ;

	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x50, 0xda ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_read( (UINT8)(nim_param->MB86A2x_Adr), 0x51, &temp ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	temp = (temp&0xdf)|(disable<<5) ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x50, 0xda ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x51, temp ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	
	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Get_IRQ_SBER_THRES_C( MB86A2x_NIM_PARAM *nim_param, UINT8 *irq_sber_thres_c )
{
	UINT32 Status ;
	UINT8 temp ;

	Status = MB86A2x_read( (UINT8)(nim_param->MB86A2x_Adr), 0x6f, &temp ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	*irq_sber_thres_c = (temp&0x04)>>2 ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Set_IRQ_PAC_ERR_A_Off( MB86A2x_NIM_PARAM *nim_param, UINT8 disable )
{
	UINT32 Status ;
	UINT8 temp ;

	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x50, 0xdb ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_read( (UINT8)(nim_param->MB86A2x_Adr), 0x51, &temp ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	temp = (temp&0xfe)|disable ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x50, 0xdb ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x51, temp ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	
	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Set_THRES_SBER_C( MB86A2x_NIM_PARAM *nim_param, UINT32 thres_c )
{
	UINT32 Status ;

	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x50, 0xe8 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x51, (UINT8)((thres_c&0x0ff0000)>>16) ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x50, 0xe9 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x51, (UINT8)((thres_c&0x000ff00)>>8) ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x50, 0xea ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x51, (UINT8)(thres_c&0x00000ff) ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Reset_IRQ_PAC_ERR_A( MB86A2x_NIM_PARAM *nim_param )
{
	UINT32 Status ;
	UINT8 temp1, temp2 ;

	Status = MB86A2x_read( (UINT8)(nim_param->MB86A2x_Adr), 0x62, &temp1 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	temp2 = temp1 | 0x10 ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x62, temp2 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	temp2 = temp1 & 0xe0 ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x62, temp2 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Get_IRQ_PAC_ERR_A( MB86A2x_NIM_PARAM *nim_param, UINT8 *irq_tserr_a )
{
	UINT32 Status ;
	UINT8 temp ;

	Status = MB86A2x_read( (UINT8)(nim_param->MB86A2x_Adr), 0x63, &temp ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	*irq_tserr_a = (temp&0x10)>>4 ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Set_IRQ_PAC_ERR_B_Off( MB86A2x_NIM_PARAM *nim_param, UINT8 disable )
{
	UINT32 Status ;
	UINT8 temp ;

	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x50, 0xdb ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_read( (UINT8)(nim_param->MB86A2x_Adr), 0x51, &temp ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	temp = (temp&0xfd)|(disable<<1) ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x50, 0xdb ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x51, temp ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	
	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Reset_IRQ_PAC_ERR_B( MB86A2x_NIM_PARAM *nim_param )
{
	UINT32 Status ;
	UINT8 temp1, temp2 ;

	Status = MB86A2x_read( (UINT8)(nim_param->MB86A2x_Adr), 0x62, &temp1 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	temp2 = temp1 | 0x20 ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x62, temp2 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	temp2 = temp1 & 0xd0 ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x62, temp2 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Get_IRQ_PAC_ERR_B( MB86A2x_NIM_PARAM *nim_param, UINT8 *irq_tserr_b )
{
	UINT32 Status ;
	UINT8 temp ;

	Status = MB86A2x_read( (UINT8)(nim_param->MB86A2x_Adr), 0x63, &temp ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	*irq_tserr_b = (temp&0x20)>>5 ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Set_IRQ_PAC_ERR_C_Off( MB86A2x_NIM_PARAM *nim_param, UINT8 disable )
{
	UINT32 Status ;
	UINT8 temp ;

	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x50, 0xdb ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_read( (UINT8)(nim_param->MB86A2x_Adr), 0x51, &temp ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	temp = (temp&0xfb)|(disable<<2) ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x50, 0xdb ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x51, temp ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	
	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Reset_IRQ_PAC_ERR_C( MB86A2x_NIM_PARAM *nim_param )
{
	UINT32 Status ;
	UINT8 temp1, temp2 ;

	Status = MB86A2x_read( (UINT8)(nim_param->MB86A2x_Adr), 0x62, &temp1 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	temp2 = temp1 | 0x40 ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x62, temp2 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	temp2 = temp1 & 0xb0 ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x62, temp2 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Get_IRQ_PAC_ERR_C( MB86A2x_NIM_PARAM *nim_param, UINT8 *irq_tserr_c )
{
	UINT32 Status ;
	UINT8 temp ;

	Status = MB86A2x_read( (UINT8)(nim_param->MB86A2x_Adr), 0x63, &temp ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	*irq_tserr_c = (temp&0x40)>>6 ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Set_IRQ_GPIO_CNG_Off( MB86A2x_NIM_PARAM *nim_param, UINT8 disable )
{
	UINT32 Status ;
	UINT8 temp ;

	Status = MB86A2x_read( (UINT8)(nim_param->MB86A2x_Adr), 0xee, &temp ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	temp = (temp&0xfd)|(disable<<1) ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0xee, temp ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	
	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Set_IRQ_GPIO3_CNG_Off( MB86A2x_NIM_PARAM *nim_param, UINT8 disable )
{
	UINT32 Status ;
	UINT8 temp ;

	Status = MB86A2x_read( (UINT8)(nim_param->MB86A2x_Adr), 0xec, &temp ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	temp = (temp&0x7f)|(disable<<7) ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0xec, temp ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	
	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Set_IRQ_GPIO2_CNG_Off( MB86A2x_NIM_PARAM *nim_param, UINT8 disable )
{
	UINT32 Status ;
	UINT8 temp ;

	Status = MB86A2x_read( (UINT8)(nim_param->MB86A2x_Adr), 0xec, &temp ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	temp = (temp&0xbf)|(disable<<6) ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0xec, temp ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	
	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Set_IRQ_GPIO1_CNG_Off( MB86A2x_NIM_PARAM *nim_param, UINT8 disable )
{
	UINT32 Status ;
	UINT8 temp ;

	Status = MB86A2x_read( (UINT8)(nim_param->MB86A2x_Adr), 0xec, &temp ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	temp = (temp&0xdf)|(disable<<5) ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0xec, temp ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	
	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Set_IRQ_GPIO0_CNG_Off( MB86A2x_NIM_PARAM *nim_param, UINT8 disable )
{
	UINT32 Status ;
	UINT8 temp ;

	Status = MB86A2x_read( (UINT8)(nim_param->MB86A2x_Adr), 0xec, &temp ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	temp = (temp&0xef)|(disable<<4) ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0xec, temp ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	
	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Reset_IRQ_GPIO_CNG( MB86A2x_NIM_PARAM *nim_param )
{
	UINT32 Status ;
	UINT8 temp1, temp2 ;

	Status = MB86A2x_read( (UINT8)(nim_param->MB86A2x_Adr), 0xee, &temp1 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	temp2 = temp1 | 0x20 ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0xee, temp2 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	temp2 = temp1 & 0xdf ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0xee, temp2 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Get_IRQ_GPIO_CNG( MB86A2x_NIM_PARAM *nim_param, UINT8 *irq_gpio_cng )
{
	UINT32 Status ;
	UINT8 temp ;

	Status = MB86A2x_read( (UINT8)(nim_param->MB86A2x_Adr), 0xed, &temp ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	*irq_gpio_cng = (temp&0x02)>>1 ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Set_IRQ_CH_SCH_END_Off( MB86A2x_NIM_PARAM *nim_param, UINT8 disable )
{
	UINT32 Status ;
	UINT8 temp ;

	Status = MB86A2x_read( (UINT8)(nim_param->MB86A2x_Adr), 0xee, &temp ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	temp = (temp&0xfe)|disable ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0xee, temp ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	
	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Reset_IRQ_CH_SCH_END( MB86A2x_NIM_PARAM *nim_param )
{
	UINT32 Status ;
	UINT8 temp1, temp2 ;

	Status = MB86A2x_read( (UINT8)(nim_param->MB86A2x_Adr), 0xee, &temp1 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	temp2 = temp1 | 0x10 ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0xee, temp2 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	temp2 = temp1 & 0xef ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0xee, temp2 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	//stop ch search
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0xe6, 0x20 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Get_IRQ_CH_SCH_END( MB86A2x_NIM_PARAM *nim_param, UINT8 *irq_ch_sch_end )
{
	UINT32 Status ;
	UINT8 temp ;

	Status = MB86A2x_read( (UINT8)(nim_param->MB86A2x_Adr), 0xed, &temp ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	*irq_ch_sch_end = temp&0x01 ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Set_RS_SW( MB86A2x_NIM_PARAM *nim_param, UINT8 RS_SW )
{
	UINT32 Status ;

	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x50, 0xd1 ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x51, (UINT8)(0x20|(RS_SW<<1)) ) ;
	if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;

	return MB86A2x_OK ;
}


MB86A2x_ERR_MSG MB86A2x_Set_RECEP_BW( MB86A2x_NIM_PARAM *nim_param, UINT8 BW )
{
	UINT32 Status ;

	if( BW==0 ) {	//input signal BW = 13seg
		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x71, 0x00 ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	}
	else {			//input signal BW = 1seg
		Status = MB86A2x_write( (UINT8)(nim_param->MB86A2x_Adr), 0x71, 0x02 ) ;
		if( Status==I2C_NG ) return MB86A2x_ERR_INIT ;
	}

	return MB86A2x_OK ;
}
