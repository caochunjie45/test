/*
**********************************************************************************
*
*  Front-end device driver for Fujitsu MB86A22 ISDB-T frontend.

* verison 01
*    Internal temp. version
* version 02
*    Initial Release
* version 03
*    Tuner.h
*       Following variable is added to struct "MxL131RF_TunerConfigS" for better understanding.
*           UINT8  flag
*    Tuner.c
*       Following functions are changed for better understanding.
*           TUNER_MSG MxL_TUNER_INIT( UINT8 OFDM_adr, MxL131RF_TunerConfigS *mytuner, MB86A2x_BANDSEL BANDSEL )
*    MB86A21_22_API_defs.h
*       Following enum values are changed to keep the compativirity to A21's software.
*          Enum MB86A2x_RECEP_LAYER was changed to keep the compatibility to A21's software.
*    MB86A22_API.c
*       Following functions are changed for better understanding.
*          MB86A2x_ERR_MSG MB86A2x_Init_Param( MB86A2x_PARAM *param )
*       Following functions are changed to correnct the mistake of return value.
*          MB86A2x_ERR_MSG MB86A2x_Set_RF( MB86A2x_NIM_PARAM *nim_param, MB86A2x_BANDSEL BANDSEL )
*       Following functions are changed to keep the compatibility to A21's software.
*          MB86A2x_ERR_MSG MB86A2x_Set_RECEP_LAYER( MB86A2x_NIM_PARAM *nim_param, MB86A2x_LAYER_PARAM *recep_layer )
*       Following functions are changed to imrpve the performance.
*          MB86A2x_ERR_MSG MB86A2x_Set_Channel( MB86A2x_PARAM *param )
*          MB86A2x_ERR_MSG MB86A2x_Set_Opt_Param( MB86A2x_NIM_PARAM *nim_param, MB86A2x_TU_DEP_PARAM *tun_dep_param )
*          MB86A2x_ERR_MSG MB86A2x_Set_Init( MB86A2x_PARAM *param, int RS_EN )
*    MB86A22_API.h
*       Following functions are changed to imrpve the performance.
*          MB86A2x_ERR_MSG MB86A2x_Set_Opt_Param( MB86A2x_NIM_PARAM *nim_param, MB86A2x_TU_DEP_PARAM *tun_dep_param )
* version 04
*    MB86A22_API.c
*       Following functions are changed to support both ES2 and ES3.
*          MB86A2x_ERR_MSG MB86A2x_Set_Init( MB86A2x_PARAM *param, int RS_EN, MB86A2x_VERSION_INFO *version_info )
*          MB86A2x_ERR_MSG MB86A2x_Set_Opt_Param( MB86A2x_NIM_PARAM *nim_param, MB86A2x_TU_DEP_PARAM *tun_dep_param, MB86A2x_VERSION_INFO *version_info )
*          MB86A2x_ERR_MSG MB86A2x_Set_Init_CH_SCH( MB86A2x_PARAM *param, MB86A2x_CH_SCH_INFO *ch_sch_info, int RS_EN, MB86A2x_VERSION_INFO *version_info )
*    MB86A22_API.h
*       Following functions are changed to support both ES2 and ES3.
*          MB86A2x_ERR_MSG MB86A2x_Set_Init( MB86A2x_PARAM *param, int RS_EN, MB86A2x_VERSION_INFO *version_info )
*          MB86A2x_ERR_MSG MB86A2x_Set_Opt_Param( MB86A2x_NIM_PARAM *nim_param, MB86A2x_TU_DEP_PARAM *tun_dep_param, MB86A2x_VERSION_INFO *version_info )
*          MB86A2x_ERR_MSG MB86A2x_Set_Init_CH_SCH( MB86A2x_PARAM *param, MB86A2x_CH_SCH_INFO *ch_sch_info, int RS_EN, MB86A2x_VERSION_INFO *version_info )
*
* version 05
*    MB86A22_API.c
*       Following functions are changed to support Doppler performance
*          MB86A2x_ERR_MSG MB86A2x_Set_Opt_Param( MB86A2x_NIM_PARAM *nim_param, MB86A2x_TU_DEP_PARAM *tun_dep_param, MB86A2x_VERSION_INFO *version_info )
*
* version 06
*    MB86A22_API.c
*       Following functions are changed to enable the parameterization of the delay time of channel search
*          MB86A2x_ERR_MSG MB86A2x_Set_Opt_Param( MB86A2x_NIM_PARAM *nim_param, MB86A2x_TU_DEP_PARAM *tun_dep_param, MB86A2x_VERSION_INFO *version_info )
*    Tuner.c
*       Following functions are modified to enable/disable Loopthrough
*          MxL_TUNER_INIT( UINT8 OFDM_adr, MxL131RF_TunerConfigS *mytuner, MB86A2x_BANDSEL BANDSEL )
*
* version 07
*    MB86A22_API.c
*       Following functions are changed to improved the echo performance close to guard interval.
*          MB86A2x_ERR_MSG MB86A2x_Set_Opt_Param( MB86A2x_NIM_PARAM *nim_param, MB86A2x_TU_DEP_PARAM *tun_dep_param, MB86A2x_VERSION_INFO *version_info )
*
* version 08
*    Internal version
*
* version 09
*    MB86A22_MxL_default_setting.h
*       Following parameters are changed to improved the channel search stability.
*          DTS_DEFAULT  31 to 16
*          IFA_DEFAULT  53 to 1656
*
* version 10
*       Following functions are changed to improve 0dB echo performance
*          MB86A2x_ERR_MSG MB86A2x_Set_Opt_Param( MB86A2x_NIM_PARAM *nim_param, MB86A2x_TU_DEP_PARAM *tun_dep_param, MB86A2x_VERSION_INFO *version_info )
*
* version 11
*  This version is made to remove double calculation.
*     MB86A22_API.c
*        -Following function were changed to remove double calculation
*            MB86A2x_ERR_MSG MB86A2x_Set_STR_OFS( MB86A2x_NIM_PARAM *nim_param, MB86A2x_PLL_PARAM *pll_param, MB86A2x_RECOV_PARAM *recov_param )
*            MB86A2x_ERR_MSG MB86A2x_Set_CRR_OFS( MB86A2x_NIM_PARAM *nim_param, MB86A2x_PLL_PARAM *pll_param, MB86A2x_RECOV_PARAM *recov_param )
*            MB86A2x_ERR_MSG MB86A2x_Set_WIDE_FREQ_DET( MB86A2x_NIM_PARAM *nim_param, MB86A2x_PLL_PARAM *pll_param )
*            MB86A2x_ERR_MSG MB86A2x_Calc_VBER_NUM( MB86A2x_BANDSEL BANDSEL, MB86A2x_CH_SCH_INFO *ch_sch_info, UINT32 VBER_T_A, UINT32 VBER_T_B, UINT32 VBER_T_C,
*                                                   UINT8 ch_num, MB86A2x_MON_TIM_PARAM *mon_tim_param )
*            MB86A2x_ERR_MSG MB86A2x_Calc_SBER_NUM( MB86A2x_BANDSEL BANDSEL, MB86A2x_CH_SCH_INFO *ch_sch_info, UINT32 SBER_T_A, UINT32 SBER_T_B, UINT32 SBER_T_C,
*                                                   UINT8 ch_num, MB86A2x_MON_TIM_PARAM *mon_tim_param )
*            MB86A2x_ERR_MSG MB86A2x_Calc_PBER_NUM( MB86A2x_BANDSEL BANDSEL, MB86A2x_CH_SCH_INFO *ch_sch_info, UINT32 PBER_T_A, UINT32 PBER_T_B, UINT32 PBER_T_C,
*                                                   UINT8 ch_num, MB86A2x_MON_TIM_PARAM *mon_tim_param )
*            MB86A2x_ERR_MSG MB86A2x_Calc_MER_SYMBOL( MB86A2x_BANDSEL BANDSEL, MB86A2x_CH_SCH_INFO *ch_sch_info, UINT32 MER_T, UINT8 ch_num, MB86A2x_MON_TIM_PARAM *mon_tim_param )
*            MB86A2x_ERR_MSG MB86A2x_Calc_CN_SYMBOL( MB86A2x_BANDSEL BANDSEL, MB86A2x_CH_SCH_INFO *ch_sch_info, UINT32 CN_T, UINT8 ch_num, MB86A2x_MON_TIM_PARAM *mon_tim_param ) ;
*            UINT32 MB86A2x_Calc_MER_Period( MB86A2x_BANDSEL BANDSEL, MB86A2x_CH_SCH_INFO *ch_sch_info, UINT8 ch_num, MB86A2x_MON_TIM_PARAM *mon_tim_param ) ;
*            UINT32 MB86A2x_Calc_CN_Period( MB86A2x_BANDSEL BANDSEL, MB86A2x_CH_SCH_INFO *ch_sch_info, UINT8 ch_num, MB86A2x_MON_TIM_PARAM *mon_tim_param ) ;
*            MB86A2x_ERR_MSG MB86A2x_Get_CRR_Freq_Error( MB86A2x_PARAM *param, MB86A2x_FREQ_ERR_INFO *freq_err_info ) ;
*            MB86A2x_ERR_MSG MB86A2x_Get_VBER_A( MB86A2x_NIM_PARAM *nim_param, MB86A2x_MON_TIM_PARAM *mon_tim_param, MB86A2x_SIG_QUAL_INFO *sig_qual_info ) ;
*            MB86A2x_ERR_MSG MB86A2x_Get_VBER_B( MB86A2x_NIM_PARAM *nim_param, MB86A2x_MON_TIM_PARAM *mon_tim_param, MB86A2x_SIG_QUAL_INFO *sig_qual_info ) ;
*            MB86A2x_ERR_MSG MB86A2x_Get_VBER_C( MB86A2x_NIM_PARAM *nim_param, MB86A2x_MON_TIM_PARAM *mon_tim_param, MB86A2x_SIG_QUAL_INFO *sig_qual_info ) ;
*            MB86A2x_ERR_MSG MB86A2x_Get_SBER_A( MB86A2x_NIM_PARAM *nim_param, MB86A2x_MON_TIM_PARAM *mon_tim_param, MB86A2x_SIG_QUAL_INFO *sig_qual_info ) ;
*            MB86A2x_ERR_MSG MB86A2x_Get_SBER_B( MB86A2x_NIM_PARAM *nim_param, MB86A2x_MON_TIM_PARAM *mon_tim_param, MB86A2x_SIG_QUAL_INFO *sig_qual_info ) ;
*            MB86A2x_ERR_MSG MB86A2x_Get_SBER_C( MB86A2x_NIM_PARAM *nim_param, MB86A2x_MON_TIM_PARAM *mon_tim_param, MB86A2x_SIG_QUAL_INFO *sig_qual_info ) ;
*            MB86A2x_ERR_MSG MB86A2x_Get_PBER_A( MB86A2x_NIM_PARAM *nim_param, MB86A2x_MON_TIM_PARAM *mon_tim_param, MB86A2x_SIG_QUAL_INFO *sig_qual_info ) ;
*            MB86A2x_ERR_MSG MB86A2x_Get_PBER_B( MB86A2x_NIM_PARAM *nim_param, MB86A2x_MON_TIM_PARAM *mon_tim_param, MB86A2x_SIG_QUAL_INFO *sig_qual_info ) ;
*            MB86A2x_ERR_MSG MB86A2x_Get_PBER_C( MB86A2x_NIM_PARAM *nim_param, MB86A2x_MON_TIM_PARAM *mon_tim_param, MB86A2x_SIG_QUAL_INFO *sig_qual_info ) ;
*            MB86A2x_ERR_MSG MB86A2x_Init_SIG_QUAL_INFO( MB86A2x_SIG_QUAL_INFO *sig_qual_info ) ;
*            MB86A2x_ERR_MSG MB86A2x_Get_CN_Value( MB86A2x_NIM_PARAM *nim_param, MB86A2x_SIG_QUAL_INFO *sig_qual_info ) ;
*            MB86A2x_ERR_MSG MB86A2x_Get_MER_A_Value( MB86A2x_NIM_PARAM *nim_param, MB86A2x_TMCC_INFO *tmcc_info, MB86A2x_SIG_QUAL_INFO *sig_qual_info ) ;
*            MB86A2x_ERR_MSG MB86A2x_Get_MER_B_Value( MB86A2x_NIM_PARAM *nim_param, MB86A2x_TMCC_INFO *tmcc_info, MB86A2x_SIG_QUAL_INFO *sig_qual_info ) ;
*            MB86A2x_ERR_MSG MB86A2x_Get_MER_C_Value( MB86A2x_NIM_PARAM *nim_param, MB86A2x_TMCC_INFO *tmcc_info, MB86A2x_SIG_QUAL_INFO *sig_qual_info ) ;
*        -Following funtions were removed
*            REAL32 MB86A2x_calc_symbol_length( MB86A2x_BANDSEL BANDSEL, UINT8 MODE, UINT8 GUARD )
*            REAL32 MB86A2x_calc_carrier_num( MB86A2x_BANDSEL BANDSEL, UINT8 MODE ) ;
*            REAL32 MB86A2x_calc_symbol_bit( UINT8 modulation ) ;
*            REAL32 MB86A2x_calc_data_ratio( UINT8 code_rate ) ;
*            REAL32 MB86A2x_calc_TSP_num( UINT8 mode, UINT8 modulation, UINT8 code_rate ) ;
*            MB86A2x_ERR_MSG MB86A2x_Get_STR_Freq_Error( MB86A2x_PARAM *param, MB86A2x_FREQ_ERR_INFO *freq_err_info ) ;
*        -Following funtions were added
*            UINT32 MB86A2x_calc_symbol_length_us( MB86A2x_BANDSEL BANDSEL, UINT8 MODE, UINT8 GUARD )
*            UINT32 MB86A2x_calc_carrier_num( MB86A2x_BANDSEL BANDSEL, UINT8 MODE ) ;
*            UINT32 MB86A2x_calc_symbol_bit( UINT8 modulation ) ;
*            UINT32 MB86A2x_calc_data_ratio_1000( UINT8 code_rate ) ;
*            UINT32 MB86A2x_calc_TSP_num( UINT8 mode, UINT8 modulation, UINT8 code_rate ) ;
*            MB86A2x_ERR_MSG MB86A2x_Get_STR_ppm_Error( MB86A2x_PARAM *param, MB86A2x_FREQ_ERR_INFO *freq_err_info )
*        -Following functions moved to MB86A22_BER_CN_Calculation.c
*            UINT32 MB86A2x_CNval_to_CN10( UINT32 cn_read_value )
*            UINT32 MB86A2x_MERval_to_CN10_DQPSK( UINT32 mer_read_vale )
*            UINT32 MB86A2x_MERval_to_CN10_QPSK( UINT32 mer_read_vale )
*            UINT32 MB86A2x_MERval_to_CN10_16QAM( UINT32 mer_read_vale )
*            UINT32 MB86A2x_MERval_to_CN10_64QAM( UINT32 mer_read_vale )
*     MB86A22_API.h
*        -Following funtions were removed
*            REAL32 MB86A2x_calc_symbol_length( UINT8 MODE, UINT8 GUARD )
*            REAL32 MB86A2x_calc_carrier_num( UINT8 MODE )
*            REAL32 MB86A2x_calc_symbol_bit( UINT8 modulation )
*            REAL32 MB86A2x_calc_data_ratio( UINT8 code_rate )
*            REAL32 MB86A2x_calc_TSP_num( UINT8 mode, UINT8 modulation, UINT8 code_rate )
*            MB86A2x_ERR_MSG MB86A2x_Get_STR_Freq_Error( MB86A2x_PARAM *param, MB86A2x_FREQ_ERR_INFO *freq_err_info )
*        -Following funtions were added
*            UINT32 MB86A2x_calc_symbol_length_us( UINT8 MODE, UINT8 GUARD )
*            UINT32 MB86A2x_calc_carrier_num( UINT8 MODE )
*            UINT32 MB86A2x_calc_symbol_bit( UINT8 modulation )
*            UINT32 MB86A2x_calc_data_ratio_1000( UINT8 code_rate )
*            UINT32 MB86A2x_calc_TSP_num( UINT8 mode, UINT8 modulation, UINT8 code_rate )
*            MB86A2x_ERR_MSG MB86A2x_Get_STR_ppm_Error( MB86A2x_PARAM *param, MB86A2x_FREQ_ERR_INFO *freq_err_info )
*        -Following functions moved to MB86A21_BER_CN_Calculation.c
*            UINT32 MB86A2x_CNval_to_CN10( UINT32 cn_read_value )
*            UINT32 MB86A2x_MERval_to_CN10_DQPSK( UINT32 mer_read_vale )
*            UINT32 MB86A2x_MERval_to_CN10_QPSK( UINT32 mer_read_vale )
*            UINT32 MB86A2x_MERval_to_CN10_16QAM( UINT32 mer_read_vale )
*            UINT32 MB86A2x_MERval_to_CN10_64QAM( UINT32 mer_read_vale )
*    MB86A22_API_defs.h
*        -Below definition is moved to "MB86A21_BER_CN_Calculation.h".
*            typedef double         REAL32 ;
*    MB86A22_API_structs.h
*        -Below member is added to struct MB86A2x_FREQ_ERR_INFO
*            SINT32 STR_ppm_error
*        -Below members are removed from struct MB86A2x_SIG_QUAL_INFO
*            REAL32 VBER_A_current, REAL32 VBER_B_current, REAL32 VBER_C_current
*            REAL32 SBER_A_current, REAL32 SBER_B_current, REAL32 SBER_C_current
*            REAL32 PBER_A_current, REAL32 PBER_B_current, REAL32 PBER_C_current
*            REAL32 CN_current
*            REAL32 MER_A_current, REAL32 MER_B_current, REAL32 MER_C_current
*        -Below members are added to struct MB86A2x_SIG_QUAL_INFO
*            UINT32 V_ERR_A, V_ERR_B, V_ERR_C
*            UINT32 S_ERR_A, S_ERR_B, S_ERR_C
*            UINT32 P_ERR_A, P_ERR_B, P_ERR_C
*            UINT32 CN_MON
*            UINT32 MER_MON_A, MER_MON_B, MER_MON_C
*    fujitsu_printf.c
*        -Below functions are changed
*             void FUJITSU_printf_STR_Freq_Error( MB86A2x_FREQ_ERR_INFO *freq_err_info )
*             void FUJITSU_printf_CN( MB86A2x_SIG_QUAL_INFO *sig_qual_info )
*             void FUJITSU_printf_BER_Lay_A( MB86A2x_SIG_QUAL_INFO *sig_qual_info, MB86A2x_TMCC_INFO *tmcc_info )
*             void FUJITSU_printf_MER( MB86A2x_SIG_QUAL_INFO *sig_qual_info )
*    MB86A22_API_package.c
*        -MB86A2x_ERR_MSG MB86A2x_Monitor1( MB86A2x_PARAM myfe, MB86A2x_LOCKNG_INFO *locking_info )
*             Called functions are changed from MB86A2x_Get_STR_Freq_Error to MB86A2x_Get_STR_ppm_Error
*        -MB86A2x_ERR_MSG MB86A2x_Monitor3( MB86A2x_PARAM myfe, MB86A2x_TMCC_INFO *tmcc_info, MB86A2x_SIG_QUAL_INFO *sign_qual_info )
*             Called functions to print are changed
*    MB86A22_BER_CN_Calculation.c (Newly added file)
*        -Below functions are added.
*             REAL32 MB86A2x_Get_VBER_RATE_A( MB86A2x_MON_TIM_PARAM *mon_tim_param, MB86A2x_SIG_QUAL_INFO *sig_qual_info )
*             REAL32 MB86A2x_Get_VBER_RATE_B( MB86A2x_MON_TIM_PARAM *mon_tim_param, MB86A2x_SIG_QUAL_INFO *sig_qual_info )
*             REAL32 MB86A2x_Get_VBER_RATE_C( MB86A2x_MON_TIM_PARAM *mon_tim_param, MB86A2x_SIG_QUAL_INFO *sig_qual_info )
*             REAL32 MB86A2x_Get_SBER_RATE_A( MB86A2x_MON_TIM_PARAM *mon_tim_param, MB86A2x_SIG_QUAL_INFO *sig_qual_info )
*             REAL32 MB86A2x_Get_SBER_RATE_B( MB86A2x_MON_TIM_PARAM *mon_tim_param, MB86A2x_SIG_QUAL_INFO *sig_qual_info )
*             REAL32 MB86A2x_Get_SBER_RATE_C( MB86A2x_MON_TIM_PARAM *mon_tim_param, MB86A2x_SIG_QUAL_INFO *sig_qual_info )
*             REAL32 MB86A2x_Get_PBER_RATE_A( MB86A2x_MON_TIM_PARAM *mon_tim_param, MB86A2x_SIG_QUAL_INFO *sig_qual_info )
*             REAL32 MB86A2x_Get_PBER_RATE_B( MB86A2x_MON_TIM_PARAM *mon_tim_param, MB86A2x_SIG_QUAL_INFO *sig_qual_info )
*             REAL32 MB86A2x_Get_PBER_RATE_C( MB86A2x_MON_TIM_PARAM *mon_tim_param, MB86A2x_SIG_QUAL_INFO *sig_qual_info )
*             REAL32 MB86A2x_Get_CN_Value_dB( MB86A2x_SIG_QUAL_INFO *sig_qual_info )
*             REAL32 MB86A2x_Get_MER_A_Value_dB( MB86A2x_TMCC_INFO *tmcc_info, MB86A2x_SIG_QUAL_INFO *sig_qual_info )
*             REAL32 MB86A2x_Get_MER_B_Value_dB( MB86A2x_TMCC_INFO *tmcc_info, MB86A2x_SIG_QUAL_INFO *sig_qual_info )
*             REAL32 MB86A2x_Get_MER_C_Value_dB( MB86A2x_TMCC_INFO *tmcc_info, MB86A2x_SIG_QUAL_INFO *sig_qual_info )
*    MB86A22_BER_CN_Calculation.h (Newly added file)
*        -Below functions are added.
*             REAL32 MB86A2x_Get_VBER_RATE_A( MB86A2x_MON_TIM_PARAM *mon_tim_param, MB86A2x_SIG_QUAL_INFO *sig_qual_info )
*             REAL32 MB86A2x_Get_VBER_RATE_B( MB86A2x_MON_TIM_PARAM *mon_tim_param, MB86A2x_SIG_QUAL_INFO *sig_qual_info )
*             REAL32 MB86A2x_Get_VBER_RATE_C( MB86A2x_MON_TIM_PARAM *mon_tim_param, MB86A2x_SIG_QUAL_INFO *sig_qual_info )
*             REAL32 MB86A2x_Get_SBER_RATE_A( MB86A2x_MON_TIM_PARAM *mon_tim_param, MB86A2x_SIG_QUAL_INFO *sig_qual_info )
*             REAL32 MB86A2x_Get_SBER_RATE_B( MB86A2x_MON_TIM_PARAM *mon_tim_param, MB86A2x_SIG_QUAL_INFO *sig_qual_info )
*             REAL32 MB86A2x_Get_SBER_RATE_C( MB86A2x_MON_TIM_PARAM *mon_tim_param, MB86A2x_SIG_QUAL_INFO *sig_qual_info )
*             REAL32 MB86A2x_Get_PBER_RATE_A( MB86A2x_MON_TIM_PARAM *mon_tim_param, MB86A2x_SIG_QUAL_INFO *sig_qual_info )
*             REAL32 MB86A2x_Get_PBER_RATE_B( MB86A2x_MON_TIM_PARAM *mon_tim_param, MB86A2x_SIG_QUAL_INFO *sig_qual_info )
*             REAL32 MB86A2x_Get_PBER_RATE_C( MB86A2x_MON_TIM_PARAM *mon_tim_param, MB86A2x_SIG_QUAL_INFO *sig_qual_info )
*             REAL32 MB86A2x_Get_CN_Value_dB( MB86A2x_SIG_QUAL_INFO *sig_qual_info )
*             REAL32 MB86A2x_Get_MER_A_Value_dB( MB86A2x_TMCC_INFO *tmcc_info, MB86A2x_SIG_QUAL_INFO *sig_qual_info )
*             REAL32 MB86A2x_Get_MER_B_Value_dB( MB86A2x_TMCC_INFO *tmcc_info, MB86A2x_SIG_QUAL_INFO *sig_qual_info )
*             REAL32 MB86A2x_Get_MER_C_Value_dB( MB86A2x_TMCC_INFO *tmcc_info, MB86A2x_SIG_QUAL_INFO *sig_qual_info )
*
* version 12
*    MB86A22_BER_CN_Calculation.c
*       Return value of following functions are corrected.
*          REAL32 MB86A2x_Get_CN_Value_dB( MB86A2x_SIG_QUAL_INFO *sig_qual_info )
*    rda5880h.h
*       This file is renamed as "rdatv_bb.h".
*    rda5880h.c
*       This file is renamed as "rdatv_bb.c".
*    rda5880h_defs.h
*       This file is renamed as "rdatv_bb_defs.h".
*    rda5880h_user_define.h
*       This file is renamed as "rdatv_bb_user_define.h".
*    rda5880h_user_define.c
*       This file is renamed as "rdatv_bb_user_define.c".
*
* version 13
*    rdatv_bb.h
*       This file is updated by RDA.
*    rdatv_bb.c
*       This file is updated by RDA.
*    rdatv_bb_defs.h 
*       This file is updated by RDA.
*    Tuner.h
*       Below parameter is added to struct RDA5880_TunerConfigS.
*         	UINT32           Xtal_freq_kHz ;
*    Tuner.c
*       Below function is changed.
*           TUNER_MSG RDA_TUNER_INIT(RDA5880_TunerConfigS *mytuner)
*    MB86A22_RDA_default_setting.h
*       Below definitions are added.
*           #define RF_Xtal_freq_default          27000
*    MB86A22_API.c
*       Below functions are corrected.
*          MB86A2x_ERR_MSG MB86A2x_Calc_MER_SYMBOL( MB86A2x_BANDSEL BANDSEL, MB86A2x_CH_SCH_INFO *ch_sch_info, UINT32 MER_T, UINT8 ch_num, MB86A2x_MON_TIM_PARAM *mon_tim_param )
*    MB86A22_BER_CN_Calculation.c
*       Below functions are corrected.
*          REAL32 MB86A2x_Get_SBER_RATE_A( MB86A2x_MON_TIM_PARAM *mon_tim_param, MB86A2x_SIG_QUAL_INFO *sig_qual_info )
*          REAL32 MB86A2x_Get_SBER_RATE_B( MB86A2x_MON_TIM_PARAM *mon_tim_param, MB86A2x_SIG_QUAL_INFO *sig_qual_info )
*          REAL32 MB86A2x_Get_SBER_RATE_C( MB86A2x_MON_TIM_PARAM *mon_tim_param, MB86A2x_SIG_QUAL_INFO *sig_qual_info )
*       
***********************************************************************************
*/
#ifndef _MB86A2_API_VERSION_H_
#define _MB86A2_API_VERSION_H_
#define MB86A2x_DRIVER_VERSION    0x0f
#endif

