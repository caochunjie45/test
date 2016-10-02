#ifndef _FUJITSU_PRINTF_H_
#define _FUJITSU_PRINTF_H_

#include "MB86A22_API_structs.h"
#include "MB86A22_BER_CN_Calculation.h"
#include "demodDrv.h" 

#if 1
#define DBG	WARNING //printk(KERN_ERR x)
#else
#define DBG(x...)
#endif
#ifdef FUJITSU_MSG_PRINT
void FUJITSU_printf_version( MB86A2x_VERSION_INFO *version_info );
void FUJITSU_printf_IF_input_level( MB86A2x_AGC_INFO *agc_info );
void FUJITSU_printf_IFAGCDAC( MB86A2x_AGC_INFO *agc_info );
void FUJITSU_printf_SEQUENCE_value( MB86A2x_LOCKNG_INFO *locking_info );
void FUJITSU_printf_STR_Freq_Error( MB86A2x_FREQ_ERR_INFO *freq_err_info );
void FUJITSU_printf_CRR_Freq_Error( MB86A2x_FREQ_ERR_INFO *freq_err_info );
void FUJITSU_printf_CN( MB86A2x_SIG_QUAL_INFO *sig_qual_info );
void FUJITSU_printf_BER_Lay_A( MB86A2x_MON_TIM_PARAM *mon_tim_param, MB86A2x_SIG_QUAL_INFO *sig_qual_info, MB86A2x_TMCC_INFO *tmcc_info );
void FUJITSU_printf_BER_Lay_B( MB86A2x_MON_TIM_PARAM *mon_tim_param, MB86A2x_SIG_QUAL_INFO *sig_qual_info, MB86A2x_TMCC_INFO *tmcc_info );
void FUJITSU_printf_BER_Lay_C( MB86A2x_MON_TIM_PARAM *mon_tim_param, MB86A2x_SIG_QUAL_INFO *sig_qual_info, MB86A2x_TMCC_INFO *tmcc_info );
void FUJITSU_printf_MER( MB86A2x_SIG_QUAL_INFO *sig_qual_info, MB86A2x_TMCC_INFO *tmcc_info );
void FUJITSU_printf_Mode_Guard( MB86A2x_TMCC_INFO *tmcc_info );
void FUJITSU_printf_TMCC( MB86A2x_TMCC_INFO *tmcc_info );
void FUJITSU_printf_Channel_None( int ch );
void FUJITSU_fprintf_TMCC( MB86A2x_CH_SCH_INFO *ch_sch_info );
#endif

#endif

