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
#ifndef _MB86A22_API_H_
#define _MB86A22_API_H_

#include "MB86A22_API_structs.h"



MB86A2x_ERR_MSG MB86A2x_Init_Adr( MB86A2x_PARAM *param ) ;
MB86A2x_ERR_MSG MB86A2x_Get_Version( MB86A2x_NIM_PARAM *nim_param, MB86A2x_VERSION_INFO *version_info ) ;
MB86A2x_ERR_MSG MB86A2x_Get_DRV_Version( MB86A2x_VERSION_INFO *version_info ) ;
MB86A2x_ERR_MSG MB86A2x_Get_LSI_Version( MB86A2x_NIM_PARAM *nim_param, MB86A2x_VERSION_INFO *version_info ) ;
MB86A2x_ERR_MSG MB86A2x_Init_Param( MB86A2x_PARAM *param ) ;
MB86A2x_ERR_MSG MB86A2x_Set_Init( MB86A2x_PARAM *param, int RS_EN, MB86A2x_VERSION_INFO *version_info ) ;
MB86A2x_ERR_MSG MB86A2x_Set_ModeGuard_fixed( MB86A2x_PARAM *param, MB86A2x_CH_SCH_INFO *ch_sch_info, UINT8 ch_num ) ;
MB86A2x_ERR_MSG MB86A2x_Set_ModeGuard_all( MB86A2x_PARAM *param ) ;
MB86A2x_ERR_MSG MB86A2x_Calc_MASTER_CLK( MB86A2x_PLL_PARAM *pll_param,  RDA5880_TunerConfigS *tuner_param) ;
MB86A2x_ERR_MSG MB86A2x_Set_STR_OFS( MB86A2x_NIM_PARAM *nim_param, MB86A2x_PLL_PARAM *pll_param, MB86A2x_RECOV_PARAM *recov_param ) ;
MB86A2x_ERR_MSG MB86A2x_Set_STR_OFS2( MB86A2x_NIM_PARAM *nim_param, MB86A2x_CH_SCH_INFO *ch_sch_info, UINT8 ch_num, MB86A2x_RECOV_PARAM *recov_param ) ;
MB86A2x_ERR_MSG MB86A2x_Set_CRR_OFS( MB86A2x_NIM_PARAM *nim_param, MB86A2x_PLL_PARAM *pll_param, MB86A2x_RECOV_PARAM *recov_param ) ;
MB86A2x_ERR_MSG MB86A2x_Set_WIDE_FREQ_DET( MB86A2x_NIM_PARAM *nim_param, MB86A2x_PLL_PARAM *pll_param ) ;
MB86A2x_ERR_MSG MB86A2x_Set_AGC( MB86A2x_NIM_PARAM *nim_param, MB86A2x_AGC_PARAM * agc_param ) ;
MB86A2x_ERR_MSG MB86A2x_Set_IFAGC( MB86A2x_NIM_PARAM *nim_param, MB86A2x_IFAGC_PARAM *ifagc_param ) ;
MB86A2x_ERR_MSG MB86A2x_Set_RFAGC( MB86A2x_NIM_PARAM *nim_param, MB86A2x_RFAGC_PARAM *rfagc_param ) ;
MB86A2x_ERR_MSG MB86A2x_Set_Opt_Param( MB86A2x_NIM_PARAM *nim_param, MB86A2x_TU_DEP_PARAM *tun_dep_param, MB86A2x_VERSION_INFO *version_info ) ;
MB86A2x_ERR_MSG MB86A2x_Set_TS_Output( MB86A2x_NIM_PARAM *nim_param, MB86A2x_TSOUT_PARAM *tsout_param, int RS_EN ) ;
MB86A2x_ERR_MSG MB86A2x_Set_Channel( MB86A2x_PARAM *param ) ;
MB86A2x_ERR_MSG MB86A2x_Set_Channel_1( MB86A2x_PARAM *param ) ;
MB86A2x_ERR_MSG MB86A2x_Set_Channel_2( MB86A2x_PARAM *param ) ;
MB86A2x_ERR_MSG MB86A2x_Set_Monitor_Timer( MB86A2x_PARAM *param ) ;
MB86A2x_ERR_MSG MB86A2x_Reset_Monitor_Timer( MB86A2x_PARAM *param ) ;
MB86A2x_ERR_MSG MB86A2x_Calc_VBER_NUM( MB86A2x_BANDSEL BANDSEL, MB86A2x_CH_SCH_INFO *ch_sch_info, UINT32 VBER_T_A, UINT32 VBER_T_B, UINT32 VBER_T_C,
										UINT8 ch_num, MB86A2x_MON_TIM_PARAM *mon_tim_param ) ;
MB86A2x_ERR_MSG MB86A2x_Calc_SBER_NUM( MB86A2x_BANDSEL BANDSEL, MB86A2x_CH_SCH_INFO *ch_sch_info, UINT32 SBER_T_A, UINT32 SBER_T_B, UINT32 SBER_T_C,
										UINT8 ch_num, MB86A2x_MON_TIM_PARAM *mon_tim_param ) ;
MB86A2x_ERR_MSG MB86A2x_Calc_PBER_NUM( MB86A2x_BANDSEL BANDSEL, MB86A2x_CH_SCH_INFO *ch_sch_info, UINT32 PBER_T_A, UINT32 PBER_T_B, UINT32 PBER_T_C,
										UINT8 ch_num, MB86A2x_MON_TIM_PARAM *mon_tim_param ) ;
MB86A2x_ERR_MSG MB86A2x_Calc_MER_SYMBOL( MB86A2x_BANDSEL BANDSEL, MB86A2x_CH_SCH_INFO *ch_sch_info, UINT32 MER_T, UINT8 ch_num, MB86A2x_MON_TIM_PARAM *mon_tim_param ) ;
MB86A2x_ERR_MSG MB86A2x_Calc_CN_SYMBOL( MB86A2x_BANDSEL BANDSEL, MB86A2x_CH_SCH_INFO *ch_sch_info, UINT32 CN_T, UINT8 ch_num, MB86A2x_MON_TIM_PARAM *mon_tim_param ) ;
UINT32 MB86A2x_Calc_MER_Period( MB86A2x_BANDSEL BANDSEL, MB86A2x_CH_SCH_INFO *ch_sch_info, UINT8 ch_num, MB86A2x_MON_TIM_PARAM *mon_tim_param ) ;
UINT32 MB86A2x_Calc_CN_Period( MB86A2x_BANDSEL BANDSEL, MB86A2x_CH_SCH_INFO *ch_sch_info, UINT8 ch_num, MB86A2x_MON_TIM_PARAM *mon_tim_param ) ;
UINT32 MB86A2x_calc_symbol_length_us( MB86A2x_BANDSEL BANDSEL, UINT8 MODE, UINT8 GUARD ) ;
UINT32 MB86A2x_calc_carrier_num( MB86A2x_BANDSEL BANDSEL, UINT8 MODE ) ;
UINT32 MB86A2x_calc_symbol_bit( UINT8 modulation ) ;
UINT32 MB86A2x_calc_data_ratio_1000( UINT8 code_rate ) ;
UINT32 MB86A2x_calc_TSP_num( UINT8 mode, UINT8 modulation, UINT8 code_rate ) ;
MB86A2x_ERR_MSG MB86A2x_Set_RF( MB86A2x_NIM_PARAM *nim_param, MB86A2x_PLL_PARAM *pll_param ) ;
MB86A2x_ERR_MSG MB86A2x_Set_IQ( MB86A2x_NIM_PARAM *nim_param ) ;
MB86A2x_ERR_MSG MB86A2x_Init_LOCKNG_INFO( MB86A2x_LOCKNG_INFO *locking_info ) ;
MB86A2x_ERR_MSG MB86A2x_Init_SIG_QUAL_INFO( MB86A2x_SIG_QUAL_INFO *sig_qual_info ) ;
MB86A2x_ERR_MSG MB86A2x_Store_Channel_Info( UINT32 ch, MB86A2x_TMCC_INFO *tmcc_info, MB86A2x_CH_SCH_INFO *ch_sch_info ) ;
MB86A2x_ERR_MSG MB86A2x_Load_CH_SCH_Result( MB86A2x_CH_SCH_INFO *ch_sch_info ) ;
MB86A2x_ERR_MSG MB86A2x_Set_RECEP_LAYER( MB86A2x_NIM_PARAM *nim_param, MB86A2x_LAYER_PARAM *recep_layer ) ;
MB86A2x_ERR_MSG MB86A2x_Set_VBER_Timer( MB86A2x_NIM_PARAM *nim_param, MB86A2x_MON_TIM_PARAM *mon_tim_param ) ;
MB86A2x_ERR_MSG MB86A2x_Set_SBER_Timer( MB86A2x_NIM_PARAM *nim_param, MB86A2x_MON_TIM_PARAM *mon_tim_param ) ;
MB86A2x_ERR_MSG MB86A2x_Set_PBER_Timer( MB86A2x_NIM_PARAM *nim_param, MB86A2x_MON_TIM_PARAM *mon_tim_param ) ;
MB86A2x_ERR_MSG MB86A2x_Set_CN_Timer( MB86A2x_NIM_PARAM *nim_param, MB86A2x_MON_TIM_PARAM *mon_tim_param ) ;
MB86A2x_ERR_MSG MB86A2x_Set_MER_Timer( MB86A2x_NIM_PARAM *nim_param, MB86A2x_MON_TIM_PARAM *mon_tim_param ) ;
MB86A2x_ERR_MSG MB86A2x_Get_SEQ_value( MB86A2x_NIM_PARAM *nim_param, MB86A2x_LOCKNG_INFO *locking_info ) ;
MB86A2x_ERR_MSG MB86A2x_Get_IF_level( MB86A2x_NIM_PARAM *nim_param, MB86A2x_AGC_INFO *agc_info ) ;
MB86A2x_ERR_MSG MB86A2x_Get_IFAGCDAC( MB86A2x_NIM_PARAM *nim_param, MB86A2x_AGC_INFO *agc_info ) ;
MB86A2x_ERR_MSG MB86A2x_Get_RFAGCDAC( MB86A2x_NIM_PARAM *nim_param, MB86A2x_AGC_INFO *agc_info ) ;
MB86A2x_ERR_MSG MB86A2x_Get_STR_ppm_Error( MB86A2x_PARAM *param, MB86A2x_FREQ_ERR_INFO *freq_err_info ) ;
MB86A2x_ERR_MSG MB86A2x_Get_STR_Register_Value( MB86A2x_NIM_PARAM *nim_param, MB86A2x_TMCC_INFO *tmcc_info ) ;
MB86A2x_ERR_MSG MB86A2x_Get_CRR_Freq_Error( MB86A2x_PARAM *param, MB86A2x_FREQ_ERR_INFO *freq_err_info ) ;
MB86A2x_ERR_MSG MB86A2x_Get_Mode_Guard( MB86A2x_NIM_PARAM *nim_param, MB86A2x_TMCC_INFO *tmcc_info ) ;
MB86A2x_ERR_MSG MB86A2x_Get_TMCC_Info( MB86A2x_NIM_PARAM *nim_param, MB86A2x_TMCC_INFO *tmcc_info ) ;
MB86A2x_ERR_MSG MB86A2x_Get_TMCC_Sys_Emerg( MB86A2x_NIM_PARAM *nim_param, MB86A2x_TMCC_INFO *tmcc_info ) ;
MB86A2x_ERR_MSG MB86A2x_Get_TMCC_Part_Recep( MB86A2x_NIM_PARAM *nim_param, MB86A2x_TMCC_INFO *tmcc_info ) ;
MB86A2x_ERR_MSG MB86A2x_Get_TMCC_Mod_Lay_A( MB86A2x_NIM_PARAM *nim_param, MB86A2x_TMCC_INFO *tmcc_info ) ;
MB86A2x_ERR_MSG MB86A2x_Get_TMCC_Mod_Lay_B( MB86A2x_NIM_PARAM *nim_param, MB86A2x_TMCC_INFO *tmcc_info ) ;
MB86A2x_ERR_MSG MB86A2x_Get_TMCC_Mod_Lay_C( MB86A2x_NIM_PARAM *nim_param, MB86A2x_TMCC_INFO *tmcc_info ) ;
MB86A2x_ERR_MSG MB86A2x_Get_TMCC_Rate_Lay_A( MB86A2x_NIM_PARAM *nim_param, MB86A2x_TMCC_INFO *tmcc_info ) ;
MB86A2x_ERR_MSG MB86A2x_Get_TMCC_Rate_Lay_B( MB86A2x_NIM_PARAM *nim_param, MB86A2x_TMCC_INFO *tmcc_info ) ;
MB86A2x_ERR_MSG MB86A2x_Get_TMCC_Rate_Lay_C( MB86A2x_NIM_PARAM *nim_param, MB86A2x_TMCC_INFO *tmcc_info ) ;
MB86A2x_ERR_MSG MB86A2x_Get_TMCC_IL_Lay_A( MB86A2x_NIM_PARAM *nim_param, MB86A2x_TMCC_INFO *tmcc_info ) ;
MB86A2x_ERR_MSG MB86A2x_Get_TMCC_IL_Lay_B( MB86A2x_NIM_PARAM *nim_param, MB86A2x_TMCC_INFO *tmcc_info ) ;
MB86A2x_ERR_MSG MB86A2x_Get_TMCC_IL_Lay_C( MB86A2x_NIM_PARAM *nim_param, MB86A2x_TMCC_INFO *tmcc_info ) ;
MB86A2x_ERR_MSG MB86A2x_Get_TMCC_Num_Lay_A( MB86A2x_NIM_PARAM *nim_param, MB86A2x_TMCC_INFO *tmcc_info ) ;
MB86A2x_ERR_MSG MB86A2x_Get_TMCC_Num_Lay_B( MB86A2x_NIM_PARAM *nim_param, MB86A2x_TMCC_INFO *tmcc_info ) ;
MB86A2x_ERR_MSG MB86A2x_Get_TMCC_Num_Lay_C( MB86A2x_NIM_PARAM *nim_param, MB86A2x_TMCC_INFO *tmcc_info ) ;
MB86A2x_ERR_MSG MB86A2x_Reset_VBER_A_Counter( MB86A2x_NIM_PARAM *nim_param ) ;
MB86A2x_ERR_MSG MB86A2x_Reset_VBER_B_Counter( MB86A2x_NIM_PARAM *nim_param ) ;
MB86A2x_ERR_MSG MB86A2x_Reset_VBER_C_Counter( MB86A2x_NIM_PARAM *nim_param ) ;
MB86A2x_ERR_MSG MB86A2x_Reset_VBER_ALL_Counter( MB86A2x_NIM_PARAM *nim_param ) ;
MB86A2x_ERR_MSG MB86A2x_Get_VBER_Flag( MB86A2x_NIM_PARAM *nim_param, MB86A2x_SIG_QUAL_INFO *sig_qual_info ) ;
MB86A2x_ERR_MSG MB86A2x_Get_VBER_A( MB86A2x_NIM_PARAM *nim_param, MB86A2x_MON_TIM_PARAM *mon_tim_param, MB86A2x_SIG_QUAL_INFO *sig_qual_info ) ;
MB86A2x_ERR_MSG MB86A2x_Get_VBER_B( MB86A2x_NIM_PARAM *nim_param, MB86A2x_MON_TIM_PARAM *mon_tim_param, MB86A2x_SIG_QUAL_INFO *sig_qual_info ) ;
MB86A2x_ERR_MSG MB86A2x_Get_VBER_C( MB86A2x_NIM_PARAM *nim_param, MB86A2x_MON_TIM_PARAM *mon_tim_param, MB86A2x_SIG_QUAL_INFO *sig_qual_info ) ;
MB86A2x_ERR_MSG MB86A2x_Reset_SBER_A_Counter( MB86A2x_NIM_PARAM *nim_param ) ;
MB86A2x_ERR_MSG MB86A2x_Reset_SBER_B_Counter( MB86A2x_NIM_PARAM *nim_param ) ;
MB86A2x_ERR_MSG MB86A2x_Reset_SBER_C_Counter( MB86A2x_NIM_PARAM *nim_param ) ;
MB86A2x_ERR_MSG MB86A2x_Reset_SBER_ALL_Counter( MB86A2x_NIM_PARAM *nim_param ) ;
MB86A2x_ERR_MSG MB86A2x_Get_SBER_Flag( MB86A2x_NIM_PARAM *nim_param, MB86A2x_SIG_QUAL_INFO *sig_qual_info ) ;
MB86A2x_ERR_MSG MB86A2x_Get_SBER_A( MB86A2x_NIM_PARAM *nim_param, MB86A2x_MON_TIM_PARAM *mon_tim_param, MB86A2x_SIG_QUAL_INFO *sig_qual_info ) ;
MB86A2x_ERR_MSG MB86A2x_Get_SBER_B( MB86A2x_NIM_PARAM *nim_param, MB86A2x_MON_TIM_PARAM *mon_tim_param, MB86A2x_SIG_QUAL_INFO *sig_qual_info ) ;
MB86A2x_ERR_MSG MB86A2x_Get_SBER_C( MB86A2x_NIM_PARAM *nim_param, MB86A2x_MON_TIM_PARAM *mon_tim_param, MB86A2x_SIG_QUAL_INFO *sig_qual_info ) ;
MB86A2x_ERR_MSG MB86A2x_Reset_PBER_A_Counter( MB86A2x_NIM_PARAM *nim_param ) ;
MB86A2x_ERR_MSG MB86A2x_Reset_PBER_B_Counter( MB86A2x_NIM_PARAM *nim_param ) ;
MB86A2x_ERR_MSG MB86A2x_Reset_PBER_C_Counter( MB86A2x_NIM_PARAM *nim_param ) ;
MB86A2x_ERR_MSG MB86A2x_Reset_PBER_ALL_Counter( MB86A2x_NIM_PARAM *nim_param ) ;
MB86A2x_ERR_MSG MB86A2x_Get_PBER_Flag( MB86A2x_NIM_PARAM *nim_param, MB86A2x_SIG_QUAL_INFO *sig_qual_info ) ;
MB86A2x_ERR_MSG MB86A2x_Get_PBER_A( MB86A2x_NIM_PARAM *nim_param, MB86A2x_MON_TIM_PARAM *mon_tim_param, MB86A2x_SIG_QUAL_INFO *sig_qual_info ) ;
MB86A2x_ERR_MSG MB86A2x_Get_PBER_B( MB86A2x_NIM_PARAM *nim_param, MB86A2x_MON_TIM_PARAM *mon_tim_param, MB86A2x_SIG_QUAL_INFO *sig_qual_info ) ;
MB86A2x_ERR_MSG MB86A2x_Get_PBER_C( MB86A2x_NIM_PARAM *nim_param, MB86A2x_MON_TIM_PARAM *mon_tim_param, MB86A2x_SIG_QUAL_INFO *sig_qual_info ) ;
MB86A2x_ERR_MSG MB86A2x_Reset_CN_Counter( MB86A2x_NIM_PARAM *nim_param, MB86A2x_MON_TIM_PARAM *mon_tim_param ) ;
MB86A2x_ERR_MSG MB86A2x_Get_CN_Flag( MB86A2x_NIM_PARAM *nim_param, MB86A2x_SIG_QUAL_INFO *sig_qual_info ) ;
MB86A2x_ERR_MSG MB86A2x_Get_CN_Value( MB86A2x_NIM_PARAM *nim_param, MB86A2x_SIG_QUAL_INFO *sig_qual_info ) ;
MB86A2x_ERR_MSG MB86A2x_Reset_MER_Counter( MB86A2x_NIM_PARAM *nim_param ) ;
MB86A2x_ERR_MSG MB86A2x_Get_MER_Flag( MB86A2x_NIM_PARAM *nim_param, MB86A2x_SIG_QUAL_INFO *sig_qual_info ) ;
MB86A2x_ERR_MSG MB86A2x_Get_MER_A_Value( MB86A2x_NIM_PARAM *nim_param, MB86A2x_TMCC_INFO *tmcc_info, MB86A2x_SIG_QUAL_INFO *sig_qual_info ) ;
MB86A2x_ERR_MSG MB86A2x_Get_MER_B_Value( MB86A2x_NIM_PARAM *nim_param, MB86A2x_TMCC_INFO *tmcc_info, MB86A2x_SIG_QUAL_INFO *sig_qual_info ) ;
MB86A2x_ERR_MSG MB86A2x_Get_MER_C_Value( MB86A2x_NIM_PARAM *nim_param, MB86A2x_TMCC_INFO *tmcc_info, MB86A2x_SIG_QUAL_INFO *sig_qual_info ) ;
MB86A2x_ERR_MSG MB86A2x_Init_CH_SCH_Result( MB86A2x_CH_SCH_INFO *ch_sch_info ) ;
MB86A2x_ERR_MSG MB86A2x_Set_Init_CH_SCH( MB86A2x_PARAM *param, MB86A2x_CH_SCH_INFO *ch_sch_info, int RS_EN, MB86A2x_VERSION_INFO *version_info ) ;
MB86A2x_ERR_MSG MB86A2x_Start_CH_SCH( MB86A2x_NIM_PARAM *nim_param ) ;
MB86A2x_ERR_MSG MB86A2x_Get_CH_SCH_Status( MB86A2x_NIM_PARAM *nim_param, MB86A2x_CH_SCH_INFO *ch_sch_info ) ;
MB86A2x_ERR_MSG MB86A2x_Get_CH_SCH_Result( MB86A2x_NIM_PARAM *nim_param, MB86A2x_CH_SCH_INFO *ch_sch_info ) ;
MB86A2x_ERR_MSG MB86A2x_Store_CH_SCH_Result( UINT32 ch, MB86A2x_TMCC_INFO *tmcc_info, MB86A2x_CH_SCH_INFO *ch_sch_info ) ;
MB86A2x_ERR_MSG MB86A2x_Save_CH_SCH_Result( MB86A2x_CH_SCH_INFO *ch_sch_info ) ;
MB86A2x_ERR_MSG MB86A2x_Set_Power_Mode( MB86A2x_NIM_PARAM *nim_param, MB86A2x_AGC_PARAM *agc_param ) ;
MB86A2x_ERR_MSG MB86A2x_Set_Low_Power( MB86A2x_NIM_PARAM *nim_param ) ;
MB86A2x_ERR_MSG MB86A2x_Set_Normal_Power( MB86A2x_NIM_PARAM *nim_param, MB86A2x_AGC_PARAM *agc_param ) ;
MB86A2x_ERR_MSG MB86A2x_Set_GPIO_Mode( MB86A2x_NIM_PARAM *nim_param, UINT8 Port_No, UINT8 Port_mode ) ;
MB86A2x_ERR_MSG MB86A2x_Get_GPIO_Mode( MB86A2x_NIM_PARAM *nim_param, UINT8 Port_No, UINT8 *Port_mode ) ;
MB86A2x_ERR_MSG MB86A2x_Set_GPIO_Value( MB86A2x_NIM_PARAM *nim_param, UINT8 Port_No, UINT8 Port_val ) ;
MB86A2x_ERR_MSG MB86A2x_Get_GPIO_Value( MB86A2x_NIM_PARAM *nim_param, UINT8 Port_No, UINT8 *Port_val ) ;
MB86A2x_ERR_MSG MB86A2x_Set_IRQ_All_Off( MB86A2x_NIM_PARAM *nim_param ) ;
MB86A2x_ERR_MSG MB86A2x_Set_IRQ_TMCC_EMG_Off( MB86A2x_NIM_PARAM *nim_param, UINT8 disable ) ;
MB86A2x_ERR_MSG MB86A2x_Reset_IRQ_TMCC_EMG( MB86A2x_NIM_PARAM *nim_param ) ;
MB86A2x_ERR_MSG MB86A2x_Set_IRQ_TMCC_EMG_INV( MB86A2x_NIM_PARAM *nim_param, UINT8 inv ) ;
MB86A2x_ERR_MSG MB86A2x_Get_IRQ_TMCC_EMG( MB86A2x_NIM_PARAM *nim_param, UINT8 *irq_tmcc_emg ) ;
MB86A2x_ERR_MSG MB86A2x_Set_IRQ_TMCC_CNTDN_Off( MB86A2x_NIM_PARAM *nim_param, UINT8 disable ) ;
MB86A2x_ERR_MSG MB86A2x_Reset_IRQ_TMCC_CNTDN( MB86A2x_NIM_PARAM *nim_param ) ;
MB86A2x_ERR_MSG MB86A2x_Get_IRQ_TMCC_CNTDN( MB86A2x_NIM_PARAM *nim_param, UINT8 *irq_tmcc_cntdn ) ;
MB86A2x_ERR_MSG MB86A2x_Set_IRQ_SBER_A_FIN_Off( MB86A2x_NIM_PARAM *nim_param, UINT8 disable ) ;
MB86A2x_ERR_MSG MB86A2x_Set_IRQ_SBER_B_FIN_Off( MB86A2x_NIM_PARAM *nim_param, UINT8 disable ) ;
MB86A2x_ERR_MSG MB86A2x_Set_IRQ_SBER_C_FIN_Off( MB86A2x_NIM_PARAM *nim_param, UINT8 disable ) ;
MB86A2x_ERR_MSG MB86A2x_Set_IRQ_SBER_A_TH_Off( MB86A2x_NIM_PARAM *nim_param, UINT8 disable ) ;
MB86A2x_ERR_MSG MB86A2x_Set_THRES_SBER_A( MB86A2x_NIM_PARAM *nim_param, UINT32 thres_a ) ;
MB86A2x_ERR_MSG MB86A2x_Get_IRQ_SBER_THRES_A( MB86A2x_NIM_PARAM *nim_param, UINT8 *irq_sber_thres_a ) ;
MB86A2x_ERR_MSG MB86A2x_Set_IRQ_SBER_B_TH_Off( MB86A2x_NIM_PARAM *nim_param, UINT8 disable ) ;
MB86A2x_ERR_MSG MB86A2x_Set_THRES_SBER_B( MB86A2x_NIM_PARAM *nim_param, UINT32 thres_b ) ;
MB86A2x_ERR_MSG MB86A2x_Get_IRQ_SBER_THRES_B( MB86A2x_NIM_PARAM *nim_param, UINT8 *irq_sber_thres_b ) ;
MB86A2x_ERR_MSG MB86A2x_Set_IRQ_SBER_C_TH_Off( MB86A2x_NIM_PARAM *nim_param, UINT8 disable ) ;
MB86A2x_ERR_MSG MB86A2x_Get_IRQ_SBER_THRES_C( MB86A2x_NIM_PARAM *nim_param, UINT8 *irq_sber_thres_c ) ;
MB86A2x_ERR_MSG MB86A2x_Set_IRQ_PAC_ERR_A_Off( MB86A2x_NIM_PARAM *nim_param, UINT8 disable ) ;
MB86A2x_ERR_MSG MB86A2x_Set_THRES_SBER_C( MB86A2x_NIM_PARAM *nim_param, UINT32 thres_c ) ;
MB86A2x_ERR_MSG MB86A2x_Reset_IRQ_PAC_ERR_A( MB86A2x_NIM_PARAM *nim_param ) ;
MB86A2x_ERR_MSG MB86A2x_Get_IRQ_PAC_ERR_A( MB86A2x_NIM_PARAM *nim_param, UINT8 *irq_tserr_a ) ;
MB86A2x_ERR_MSG MB86A2x_Set_IRQ_PAC_ERR_B_Off( MB86A2x_NIM_PARAM *nim_param, UINT8 disable ) ;
MB86A2x_ERR_MSG MB86A2x_Reset_IRQ_PAC_ERR_B( MB86A2x_NIM_PARAM *nim_param ) ;
MB86A2x_ERR_MSG MB86A2x_Get_IRQ_PAC_ERR_B( MB86A2x_NIM_PARAM *nim_param, UINT8 *irq_tserr_b ) ;
MB86A2x_ERR_MSG MB86A2x_Set_IRQ_PAC_ERR_C_Off( MB86A2x_NIM_PARAM *nim_param, UINT8 disable ) ;
MB86A2x_ERR_MSG MB86A2x_Reset_IRQ_PAC_ERR_C( MB86A2x_NIM_PARAM *nim_param ) ;
MB86A2x_ERR_MSG MB86A2x_Get_IRQ_PAC_ERR_C( MB86A2x_NIM_PARAM *nim_param, UINT8 *irq_tserr_c ) ;
MB86A2x_ERR_MSG MB86A2x_Set_IRQ_GPIO_CNG_Off( MB86A2x_NIM_PARAM *nim_param, UINT8 disable ) ;
MB86A2x_ERR_MSG MB86A2x_Set_IRQ_GPIO3_CNG_Off( MB86A2x_NIM_PARAM *nim_param, UINT8 disable ) ;
MB86A2x_ERR_MSG MB86A2x_Set_IRQ_GPIO2_CNG_Off( MB86A2x_NIM_PARAM *nim_param, UINT8 disable ) ;
MB86A2x_ERR_MSG MB86A2x_Set_IRQ_GPIO1_CNG_Off( MB86A2x_NIM_PARAM *nim_param, UINT8 disable ) ;
MB86A2x_ERR_MSG MB86A2x_Set_IRQ_GPIO0_CNG_Off( MB86A2x_NIM_PARAM *nim_param, UINT8 disable ) ;
MB86A2x_ERR_MSG MB86A2x_Reset_IRQ_GPIO_CNG( MB86A2x_NIM_PARAM *nim_param ) ;
MB86A2x_ERR_MSG MB86A2x_Get_IRQ_GPIO_CNG( MB86A2x_NIM_PARAM *nim_param, UINT8 *irq_gpio_cng ) ;
MB86A2x_ERR_MSG MB86A2x_Set_IRQ_CH_SCH_END_Off( MB86A2x_NIM_PARAM *nim_param, UINT8 disable ) ;
MB86A2x_ERR_MSG MB86A2x_Reset_IRQ_CH_SCH_END( MB86A2x_NIM_PARAM *nim_param ) ;
MB86A2x_ERR_MSG MB86A2x_Get_IRQ_CH_SCH_END( MB86A2x_NIM_PARAM *nim_param, UINT8 *irq_ch_sch_end ) ;
MB86A2x_ERR_MSG MB86A2x_Set_RS_SW( MB86A2x_NIM_PARAM *nim_param, UINT8 RS_SW ) ;
MB86A2x_ERR_MSG MB86A2x_Set_RECEP_BW( MB86A2x_NIM_PARAM *nim_param, UINT8 BW ) ;
#endif

