/****************************************************************************
*
*  Sample code of MB86A21 to calculate BER and C/N 
*
*  Copyright (c) 2012 Fujitsu Microelectronics 
*  All rights reserved. 
*
*  Created 06/15/2012
*
****************************************************************************/
//Note that this code includes REAL32 calculation
#ifndef _MB86A22_BER_CH_CALCULATION_H_
#define _MB86A22_BER_CH_CALCULATION_H_

#include "MB86A22_API_structs.h"

#define REAL32 double


REAL32 MB86A2x_Get_VBER_RATE_A( MB86A2x_MON_TIM_PARAM *mon_tim_param, MB86A2x_SIG_QUAL_INFO *sig_qual_info ) ;
REAL32 MB86A2x_Get_VBER_RATE_B( MB86A2x_MON_TIM_PARAM *mon_tim_param, MB86A2x_SIG_QUAL_INFO *sig_qual_info ) ;
REAL32 MB86A2x_Get_VBER_RATE_C( MB86A2x_MON_TIM_PARAM *mon_tim_param, MB86A2x_SIG_QUAL_INFO *sig_qual_info ) ;
REAL32 MB86A2x_Get_SBER_RATE_A( MB86A2x_MON_TIM_PARAM *mon_tim_param, MB86A2x_SIG_QUAL_INFO *sig_qual_info ) ;
REAL32 MB86A2x_Get_SBER_RATE_B( MB86A2x_MON_TIM_PARAM *mon_tim_param, MB86A2x_SIG_QUAL_INFO *sig_qual_info ) ;
REAL32 MB86A2x_Get_SBER_RATE_C( MB86A2x_MON_TIM_PARAM *mon_tim_param, MB86A2x_SIG_QUAL_INFO *sig_qual_info ) ;
REAL32 MB86A2x_Get_PBER_RATE_A( MB86A2x_MON_TIM_PARAM *mon_tim_param, MB86A2x_SIG_QUAL_INFO *sig_qual_info ) ;
REAL32 MB86A2x_Get_PBER_RATE_B( MB86A2x_MON_TIM_PARAM *mon_tim_param, MB86A2x_SIG_QUAL_INFO *sig_qual_info ) ;
REAL32 MB86A2x_Get_PBER_RATE_C( MB86A2x_MON_TIM_PARAM *mon_tim_param, MB86A2x_SIG_QUAL_INFO *sig_qual_info ) ;
REAL32 MB86A2x_Get_CN_Value_dB( MB86A2x_SIG_QUAL_INFO *sig_qual_info ) ;
UINT32 MB86A2x_CNval_to_CN10( UINT32 cn_read_value ) ;
REAL32 MB86A2x_Get_MER_A_Value_dB( MB86A2x_TMCC_INFO *tmcc_info, MB86A2x_SIG_QUAL_INFO *sig_qual_info ) ;
REAL32 MB86A2x_Get_MER_B_Value_dB( MB86A2x_TMCC_INFO *tmcc_info, MB86A2x_SIG_QUAL_INFO *sig_qual_info ) ;
REAL32 MB86A2x_Get_MER_C_Value_dB( MB86A2x_TMCC_INFO *tmcc_info, MB86A2x_SIG_QUAL_INFO *sig_qual_info ) ;
UINT32 MB86A2x_MERval_to_CN10_DQPSK( UINT32 mer_read_vale ) ;
UINT32 MB86A2x_MERval_to_CN10_QPSK( UINT32 mer_read_vale ) ;
UINT32 MB86A2x_MERval_to_CN10_16QAM( UINT32 mer_read_vale ) ;
UINT32 MB86A2x_MERval_to_CN10_64QAM( UINT32 mer_read_vale ) ;
#endif

