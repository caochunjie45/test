//#include <stdio.h>
//#include <stdlib.h>
#include "Fujitsu_printf.h"
#include "MB86A22_API_structs.h"
#include "MB86A22_BER_CN_Calculation.h"

#ifdef FUJITSU_MSG_PRINT
void FUJITSU_printf_version( MB86A2x_VERSION_INFO *version_info )
{
	DBG( "software_version=%d\n", version_info->DRV_VERSION ) ;
	DBG( "LSI version=%02x\n", version_info->LSI_VERSION ) ;
}


void FUJITSU_printf_IF_input_level( MB86A2x_AGC_INFO *agc_info )
{
	DBG( "\nIF_input_level=%d ", agc_info->IF_INPUT_LEVEL ) ;
}


void FUJITSU_printf_IFAGCDAC( MB86A2x_AGC_INFO *agc_info )
{
	DBG( "DACOUT=%d ", agc_info->IFAGCDAC ) ;
}


void FUJITSU_printf_SEQUENCE_value( MB86A2x_LOCKNG_INFO *locking_info )
{
	DBG( "STATE=%d \n", locking_info->STATE ) ;
}


void FUJITSU_printf_STR_Freq_Error( MB86A2x_FREQ_ERR_INFO *freq_err_info )
{
	DBG( "   STR_freq_err[Hz]=%d ", freq_err_info->STR_freq_error_Hz ) ;
}


void FUJITSU_printf_CRR_Freq_Error( MB86A2x_FREQ_ERR_INFO *freq_err_info )
{
	DBG( "CRR_freq_err[kHz]=%d\n", freq_err_info->CRR_freq_error_kHz ) ;
}


void FUJITSU_printf_CN( MB86A2x_SIG_QUAL_INFO *sig_qual_info )
{
	if( sig_qual_info->CN_MON==16777215 ) DBG( "   C/N--.-[dB]\n" ) ;
	else                                  
		DBG( "   C/N=%4.1lf[dB]\n", MB86A2x_Get_CN_Value_dB( sig_qual_info ) ) ;
}


void FUJITSU_printf_BER_Lay_A( MB86A2x_MON_TIM_PARAM *mon_tim_param, MB86A2x_SIG_QUAL_INFO *sig_qual_info, MB86A2x_TMCC_INFO *tmcc_info )
{
	if( tmcc_info->NUM_A_current<=13 ) {
		if( sig_qual_info->V_ERR_A==16777215 ) {
			DBG( "   VBER_A=-.---e+000 " ) ;
		}
		else {
			DBG( "   VBER_A=%5.3le ", MB86A2x_Get_VBER_RATE_A( mon_tim_param, sig_qual_info ) ) ;
		}
		if( sig_qual_info->S_ERR_A==16777215 ) {
			DBG( "SBER_A=-.---e+000 " ) ;
		}
		else {
			DBG( "SBER_A=%5.3le ", MB86A2x_Get_SBER_RATE_A( mon_tim_param, sig_qual_info ) ) ;
		}
		if( sig_qual_info->P_ERR_A==16777215 ) {
			DBG( "PBER_A=-.---e+000\n" ) ;
		}
		else {
			DBG( "PBER_A=%5.3le\n", MB86A2x_Get_PBER_RATE_A( mon_tim_param, sig_qual_info ) ) ;
		}
	}
	else {
		DBG( "   VBER_A=-.---e+000 SBER_A=-.---e+000 PBER_A=-.---e+000\n" ) ;
	}
}


void FUJITSU_printf_BER_Lay_B( MB86A2x_MON_TIM_PARAM *mon_tim_param, MB86A2x_SIG_QUAL_INFO *sig_qual_info, MB86A2x_TMCC_INFO *tmcc_info )
{
	if( tmcc_info->NUM_B_current<=13 ) {
		if( sig_qual_info->V_ERR_B==16777215 ) {
			DBG( "   VBER_B=-.---e+000 " ) ;
		}
		else {
			DBG( "   VBER_B=%5.3le ", MB86A2x_Get_VBER_RATE_B( mon_tim_param, sig_qual_info ) ) ;
		}
		if( sig_qual_info->S_ERR_B==16777215 ) {
			DBG( "SBER_B=-.---e+000 " ) ;
		}
		else {
			DBG( "SBER_B=%5.3le ", MB86A2x_Get_SBER_RATE_B( mon_tim_param, sig_qual_info ) ) ;
		}
		if( sig_qual_info->P_ERR_B==16777215 ) {
			DBG( "PBER_B=-.---e+000\n" ) ;
		}
		else {
			DBG( "PBER_B=%5.3le\n", MB86A2x_Get_PBER_RATE_B( mon_tim_param, sig_qual_info ) ) ;
		}
	}
	else {
		DBG( "   VBER_B=-.---e+000 SBER_B=-.---e+000 PBER_B=-.---e+000\n" ) ;
	}
}


void FUJITSU_printf_BER_Lay_C( MB86A2x_MON_TIM_PARAM *mon_tim_param, MB86A2x_SIG_QUAL_INFO *sig_qual_info, MB86A2x_TMCC_INFO *tmcc_info )
{
	if( tmcc_info->NUM_C_current<=13 ) {
		if( sig_qual_info->V_ERR_C==16777215 ) {
			DBG( "   VBER_C=-.---e+000 " ) ;
		}
		else {
			DBG( "   VBER_C=%5.3le ", MB86A2x_Get_VBER_RATE_C( mon_tim_param, sig_qual_info ) ) ;
		}
		if( sig_qual_info->S_ERR_C==16777215 ) {
			DBG( "SBER_C=-.---e+000 " ) ;
		}
		else {
			DBG( "SBER_C=%5.3le ", MB86A2x_Get_SBER_RATE_C( mon_tim_param, sig_qual_info ) ) ;
		}
		if( sig_qual_info->P_ERR_C==16777215 ) {
			DBG( "PBER_C=-.---e+000\n" ) ;
		}
		else {
			DBG( "PBER_C=%5.3le\n", MB86A2x_Get_PBER_RATE_C( mon_tim_param, sig_qual_info ) ) ;
		}
	}
	else {
		DBG( "   VBER_C=-.---e+000 SBER_C=-.---e+000 PBER_C=-.---e+000\n" ) ;
	}
}


void FUJITSU_printf_MER( MB86A2x_SIG_QUAL_INFO *sig_qual_info, MB86A2x_TMCC_INFO *tmcc_info )
{
	if( tmcc_info->NUM_A_current<=13 ) {
		if( sig_qual_info->MER_MON_A==16777215 ) DBG( "   MER_A=--.-[dB] " ) ;
		else                                     DBG( "   MER_A=%4.1lf[dB] ", MB86A2x_Get_MER_A_Value_dB( tmcc_info, sig_qual_info ) ) ;
	}
	else {
		DBG( "   MER_A=--.-[dB] " ) ;
	}
	if( tmcc_info->NUM_B_current<=13 ) {
		if( sig_qual_info->MER_MON_B==16777215 ) DBG( "MER_B=--.-[dB] " ) ;
		else                                     DBG( "MER_B=%4.1lf[dB] ", MB86A2x_Get_MER_B_Value_dB( tmcc_info, sig_qual_info ) ) ;
	}
	else {
		DBG( "MER_B=--.-[dB] " ) ;
	}
	if( tmcc_info->NUM_C_current<=13 ) {
		if( sig_qual_info->MER_MON_C==16777215 ) DBG( "MER_C=--.-[dB] " ) ;
		else                                     DBG( "MER_C=%4.1lf[dB] ", MB86A2x_Get_MER_C_Value_dB( tmcc_info, sig_qual_info ) ) ;
	}
	else {
		DBG( "MER_C=--.-[dB] " ) ;
	}
	DBG( "\n" ) ;
}


void FUJITSU_printf_Mode_Guard( MB86A2x_TMCC_INFO *tmcc_info )
{
	if(      tmcc_info->MODE_current==1 ) DBG( "   MODE=2 " ) ;
	else if( tmcc_info->MODE_current==2 ) DBG( "   MODE=3 " ) ;
	else                                  DBG( "   MODE=? " ) ;

	if(      tmcc_info->GUARD_current==0 ) DBG( "GUARD=1/4\n" ) ;
	else if( tmcc_info->GUARD_current==1 ) DBG( "GUARD=1/8\n" ) ;
	else if( tmcc_info->GUARD_current==2 ) DBG( "GUARD=1/16\n" ) ;
	else if( tmcc_info->GUARD_current==3 ) DBG( "GUARD=1/32\n" ) ;
	else                                   DBG( "GUARD=-/--\n" ) ;
}


void FUJITSU_printf_TMCC( MB86A2x_TMCC_INFO *tmcc_info )
{
	if( tmcc_info->NUM_A_current<=13 ) {
		if(      tmcc_info->MOD_A_current==0 ) DBG( "   LAYER_A=DQPSK" ) ;
		else if( tmcc_info->MOD_A_current==1 ) DBG( "   LAYER_A=QPSK " ) ;
		else if( tmcc_info->MOD_A_current==2 ) DBG( "   LAYER_A=16QAM" ) ;
		else if( tmcc_info->MOD_A_current==3 ) DBG( "   LAYER_A=64QAM" ) ;
		else                                   DBG( "   LAYER_A=-----" ) ;

		if(      tmcc_info->RATE_A_current==0 ) DBG( "(1/2) " ) ;
		else if( tmcc_info->RATE_A_current==1 ) DBG( "(2/3) " ) ;
		else if( tmcc_info->RATE_A_current==2 ) DBG( "(3/4) " ) ;
		else if( tmcc_info->RATE_A_current==3 ) DBG( "(5/6) " ) ;
		else if( tmcc_info->RATE_A_current==4 ) DBG( "(7/8) " ) ;
		else                                    DBG( "(-/-) " ) ;

		if(      (tmcc_info->MODE_current==0)&&(tmcc_info->IL_A_current==0) ) DBG( "IL= 0(M1) " ) ;
		else if( (tmcc_info->MODE_current==0)&&(tmcc_info->IL_A_current==1) ) DBG( "IL= 4(M1) " ) ;
		else if( (tmcc_info->MODE_current==0)&&(tmcc_info->IL_A_current==2) ) DBG( "IL= 8(M1) " ) ;
		else if( (tmcc_info->MODE_current==0)&&(tmcc_info->IL_A_current==3) ) DBG( "IL=16(M1) " ) ;
		else if( (tmcc_info->MODE_current==0)&&(tmcc_info->IL_A_current==4) ) DBG( "IL=32(M1) " ) ;
		else if( (tmcc_info->MODE_current==1)&&(tmcc_info->IL_A_current==0) ) DBG( "IL= 0(M2) " ) ;
		else if( (tmcc_info->MODE_current==1)&&(tmcc_info->IL_A_current==1) ) DBG( "IL= 2(M2) " ) ;
		else if( (tmcc_info->MODE_current==1)&&(tmcc_info->IL_A_current==2) ) DBG( "IL= 4(M2) " ) ;
		else if( (tmcc_info->MODE_current==1)&&(tmcc_info->IL_A_current==3) ) DBG( "IL= 8(M2) " ) ;
		else if( (tmcc_info->MODE_current==1)&&(tmcc_info->IL_A_current==4) ) DBG( "IL=16(M2) " ) ;
		else if( (tmcc_info->MODE_current==2)&&(tmcc_info->IL_A_current==0) ) DBG( "IL= 0(M3) " ) ;
		else if( (tmcc_info->MODE_current==2)&&(tmcc_info->IL_A_current==1) ) DBG( "IL= 1(M3) " ) ;
		else if( (tmcc_info->MODE_current==2)&&(tmcc_info->IL_A_current==2) ) DBG( "IL= 2(M3) " ) ;
		else if( (tmcc_info->MODE_current==2)&&(tmcc_info->IL_A_current==3) ) DBG( "IL= 4(M3) " ) ;
		else if( (tmcc_info->MODE_current==2)&&(tmcc_info->IL_A_current==4) ) DBG( "IL= 8(M3) " ) ;
		else                                                                  DBG( "IL=------ " ) ;

		if( 1<=tmcc_info->NUM_A_current ) {
			DBG( "NUM=%02d\n", tmcc_info->NUM_A_current ) ;
		}
		else {
			DBG( "\n" ) ;
		}

	}

	if( (1<=tmcc_info->NUM_B_current)&&(tmcc_info->NUM_B_current<=13) ) {
		if(      tmcc_info->MOD_B_current==0 ) DBG( "   LAYER_B=DQPSK" ) ;
		else if( tmcc_info->MOD_B_current==1 ) DBG( "   LAYER_B=QPSK " ) ;
		else if( tmcc_info->MOD_B_current==2 ) DBG( "   LAYER_B=16QAM" ) ;
		else if( tmcc_info->MOD_B_current==3 ) DBG( "   LAYER_B=64QAM" ) ;
		else                                   DBG( "   LAYER_B=-----" ) ;

		if(      tmcc_info->RATE_B_current==0 ) DBG( "(1/2) " ) ;
		else if( tmcc_info->RATE_B_current==1 ) DBG( "(2/3) " ) ;
		else if( tmcc_info->RATE_B_current==2 ) DBG( "(3/4) " ) ;
		else if( tmcc_info->RATE_B_current==3 ) DBG( "(5/6) " ) ;
		else if( tmcc_info->RATE_B_current==4 ) DBG( "(7/8) " ) ;
		else                                    DBG( "(-/-) " ) ;

		if(      (tmcc_info->MODE_current==0)&&(tmcc_info->IL_B_current==0) ) DBG( "IL= 0(M1) " ) ;
		else if( (tmcc_info->MODE_current==0)&&(tmcc_info->IL_B_current==1) ) DBG( "IL= 4(M1) " ) ;
		else if( (tmcc_info->MODE_current==0)&&(tmcc_info->IL_B_current==2) ) DBG( "IL= 8(M1) " ) ;
		else if( (tmcc_info->MODE_current==0)&&(tmcc_info->IL_B_current==3) ) DBG( "IL=16(M1) " ) ;
		else if( (tmcc_info->MODE_current==0)&&(tmcc_info->IL_B_current==4) ) DBG( "IL=32(M1) " ) ;
		else if( (tmcc_info->MODE_current==1)&&(tmcc_info->IL_B_current==0) ) DBG( "IL= 0(M2) " ) ;
		else if( (tmcc_info->MODE_current==1)&&(tmcc_info->IL_B_current==1) ) DBG( "IL= 2(M2) " ) ;
		else if( (tmcc_info->MODE_current==1)&&(tmcc_info->IL_B_current==2) ) DBG( "IL= 4(M2) " ) ;
		else if( (tmcc_info->MODE_current==1)&&(tmcc_info->IL_B_current==3) ) DBG( "IL= 8(M2) " ) ;
		else if( (tmcc_info->MODE_current==1)&&(tmcc_info->IL_B_current==4) ) DBG( "IL=16(M2) " ) ;
		else if( (tmcc_info->MODE_current==2)&&(tmcc_info->IL_B_current==0) ) DBG( "IL= 0(M3) " ) ;
		else if( (tmcc_info->MODE_current==2)&&(tmcc_info->IL_B_current==1) ) DBG( "IL= 1(M3) " ) ;
		else if( (tmcc_info->MODE_current==2)&&(tmcc_info->IL_B_current==2) ) DBG( "IL= 2(M3) " ) ;
		else if( (tmcc_info->MODE_current==2)&&(tmcc_info->IL_B_current==3) ) DBG( "IL= 4(M3) " ) ;
		else if( (tmcc_info->MODE_current==2)&&(tmcc_info->IL_B_current==4) ) DBG( "IL= 8(M3) " ) ;
		else                                                                  DBG( "IL=------ " ) ;

		if( tmcc_info->NUM_B_current>13 ) DBG( "NUM=--\n" ) ;
		else                              DBG( "NUM=%02d\n", tmcc_info->NUM_B_current ) ;
	}

	if( (1<=tmcc_info->NUM_C_current)&&(tmcc_info->NUM_C_current<=13) ) {
		if(      tmcc_info->MOD_C_current==0 ) DBG( "   LAYER_C=DQPSK" ) ;
		else if( tmcc_info->MOD_C_current==1 ) DBG( "   LAYER_C=QPSK " ) ;
		else if( tmcc_info->MOD_C_current==2 ) DBG( "   LAYER_C=16QAM" ) ;
		else if( tmcc_info->MOD_C_current==3 ) DBG( "   LAYER_C=64QAM" ) ;
		else                                   DBG( "   LAYER_C=-----" ) ;

		if(      tmcc_info->RATE_C_current==0 ) DBG( "(1/2) " ) ;
		else if( tmcc_info->RATE_C_current==1 ) DBG( "(2/3) " ) ;
		else if( tmcc_info->RATE_C_current==2 ) DBG( "(3/4) " ) ;
		else if( tmcc_info->RATE_C_current==3 ) DBG( "(5/6) " ) ;
		else if( tmcc_info->RATE_C_current==4 ) DBG( "(7/8) " ) ;
		else                                    DBG( "(-/-) " ) ;

		if(      (tmcc_info->MODE_current==0)&&(tmcc_info->IL_C_current==0) ) DBG( "IL= 0(M1) " ) ;
		else if( (tmcc_info->MODE_current==0)&&(tmcc_info->IL_C_current==1) ) DBG( "IL= 4(M1) " ) ;
		else if( (tmcc_info->MODE_current==0)&&(tmcc_info->IL_C_current==2) ) DBG( "IL= 8(M1) " ) ;
		else if( (tmcc_info->MODE_current==0)&&(tmcc_info->IL_C_current==3) ) DBG( "IL=16(M1) " ) ;
		else if( (tmcc_info->MODE_current==0)&&(tmcc_info->IL_C_current==4) ) DBG( "IL=32(M1) " ) ;
		else if( (tmcc_info->MODE_current==1)&&(tmcc_info->IL_C_current==0) ) DBG( "IL= 0(M2) " ) ;
		else if( (tmcc_info->MODE_current==1)&&(tmcc_info->IL_C_current==1) ) DBG( "IL= 2(M2) " ) ;
		else if( (tmcc_info->MODE_current==1)&&(tmcc_info->IL_C_current==2) ) DBG( "IL= 4(M2) " ) ;
		else if( (tmcc_info->MODE_current==1)&&(tmcc_info->IL_C_current==3) ) DBG( "IL= 8(M2) " ) ;
		else if( (tmcc_info->MODE_current==1)&&(tmcc_info->IL_C_current==4) ) DBG( "IL=16(M2) " ) ;
		else if( (tmcc_info->MODE_current==2)&&(tmcc_info->IL_C_current==0) ) DBG( "IL= 0(M3) " ) ;
		else if( (tmcc_info->MODE_current==2)&&(tmcc_info->IL_C_current==1) ) DBG( "IL= 1(M3) " ) ;
		else if( (tmcc_info->MODE_current==2)&&(tmcc_info->IL_C_current==2) ) DBG( "IL= 2(M3) " ) ;
		else if( (tmcc_info->MODE_current==2)&&(tmcc_info->IL_C_current==3) ) DBG( "IL= 4(M3) " ) ;
		else if( (tmcc_info->MODE_current==2)&&(tmcc_info->IL_C_current==4) ) DBG( "IL= 8(M3) " ) ;
		else                                                                  DBG( "IL=------ " ) ;

		if( tmcc_info->NUM_C_current>13 ) DBG( "NUM=--\n" ) ;
		else                              DBG( "NUM=%02d\n", tmcc_info->NUM_C_current ) ;
	}
}


void FUJITSU_printf_Channel_None( int ch )
{
	DBG( "ch=%02d None\n", ch ) ;
}


void FUJITSU_fprintf_TMCC( MB86A2x_CH_SCH_INFO *ch_sch_info )
{
/*
	FILE *f1 ;
	UINT8 ch ;

	f1 = fopen( "Channel_Search_Result.txt", "wt" ) ;
	for( ch=0 ; ch<ch_sch_info->Detected_ch_num ; ch++ ) {

		DBG( f1, "No.=%02d CH_num=%02d", ch, ch_sch_info->CH_num[ch] ) ;

		if(      ch_sch_info->MODE[ch]==1 ) DBG( f1, "   MODE=2 " ) ;
		else if( ch_sch_info->MODE[ch]==2 ) DBG( f1, "   MODE=3 " ) ;
		else                                DBG( f1, "   MODE=? " ) ;

		if(      ch_sch_info->GUARD[ch]==0 ) DBG( f1, "GUARD=1/4\n" ) ;
		else if( ch_sch_info->GUARD[ch]==1 ) DBG( f1, "GUARD=1/8\n" ) ;
		else if( ch_sch_info->GUARD[ch]==2 ) DBG( f1, "GUARD=1/16\n" ) ;
		else                                 DBG( f1, "GUARD=-/-\n" ) ;

		if(      ch_sch_info->MOD_A[ch]==0 ) DBG( f1, "    LAYER_A=DQPSK" ) ;
		else if( ch_sch_info->MOD_A[ch]==1 ) DBG( f1, "    LAYER_A=QPSK " ) ;
		else if( ch_sch_info->MOD_A[ch]==2 ) DBG( f1, "    LAYER_A=16QAM" ) ;
		else if( ch_sch_info->MOD_A[ch]==3 ) DBG( f1, "    LAYER_A=64QAM" ) ;
		else                                 DBG( f1, "    LAYER_A=-----" ) ;

		if(      ch_sch_info->RATE_A[ch]==0 ) DBG( f1, "(1/2) " ) ;
		else if( ch_sch_info->RATE_A[ch]==1 ) DBG( f1, "(2/3) " ) ;
		else if( ch_sch_info->RATE_A[ch]==2 ) DBG( f1, "(3/4) " ) ;
		else if( ch_sch_info->RATE_A[ch]==3 ) DBG( f1, "(5/6) " ) ;
		else if( ch_sch_info->RATE_A[ch]==4 ) DBG( f1, "(7/8) " ) ;
		else                                  DBG( f1, "(-/-) " ) ;

		if(      (ch_sch_info->MODE[ch]==0)&&(ch_sch_info->IL_A[ch]==0) ) DBG( f1, "IL= 0(M1) " ) ;
		else if( (ch_sch_info->MODE[ch]==0)&&(ch_sch_info->IL_A[ch]==1) ) DBG( f1, "IL= 4(M1) " ) ;
		else if( (ch_sch_info->MODE[ch]==0)&&(ch_sch_info->IL_A[ch]==2) ) DBG( f1, "IL= 8(M1) " ) ;
		else if( (ch_sch_info->MODE[ch]==0)&&(ch_sch_info->IL_A[ch]==3) ) DBG( f1, "IL=16(M1) " ) ;
		else if( (ch_sch_info->MODE[ch]==0)&&(ch_sch_info->IL_A[ch]==4) ) DBG( f1, "IL=32(M1) " ) ;
		else if( (ch_sch_info->MODE[ch]==1)&&(ch_sch_info->IL_A[ch]==0) ) DBG( f1, "IL= 0(M2) " ) ;
		else if( (ch_sch_info->MODE[ch]==1)&&(ch_sch_info->IL_A[ch]==1) ) DBG( f1, "IL= 2(M2) " ) ;
		else if( (ch_sch_info->MODE[ch]==1)&&(ch_sch_info->IL_A[ch]==2) ) DBG( f1, "IL= 4(M2) " ) ;
		else if( (ch_sch_info->MODE[ch]==1)&&(ch_sch_info->IL_A[ch]==3) ) DBG( f1, "IL= 8(M2) " ) ;
		else if( (ch_sch_info->MODE[ch]==1)&&(ch_sch_info->IL_A[ch]==4) ) DBG( f1, "IL=16(M2) " ) ;
		else if( (ch_sch_info->MODE[ch]==2)&&(ch_sch_info->IL_A[ch]==0) ) DBG( f1, "IL= 0(M3) " ) ;
		else if( (ch_sch_info->MODE[ch]==2)&&(ch_sch_info->IL_A[ch]==1) ) DBG( f1, "IL= 1(M3) " ) ;
		else if( (ch_sch_info->MODE[ch]==2)&&(ch_sch_info->IL_A[ch]==2) ) DBG( f1, "IL= 2(M3) " ) ;
		else if( (ch_sch_info->MODE[ch]==2)&&(ch_sch_info->IL_A[ch]==3) ) DBG( f1, "IL= 4(M3) " ) ;
		else if( (ch_sch_info->MODE[ch]==2)&&(ch_sch_info->IL_A[ch]==4) ) DBG( f1, "IL= 8(M3) " ) ;
		else                                                              DBG( f1, "IL=------ " ) ;

		if( ch_sch_info->NUM_A[ch]>13 ) DBG( f1, "NUM=--\n" ) ;
		else                            DBG( f1, "NUM=%02d\n", ch_sch_info->NUM_A[ch] ) ;

		if(      ch_sch_info->MOD_B[ch]==0 ) DBG( f1, "    LAYER_B=DQPSK" ) ;
		else if( ch_sch_info->MOD_B[ch]==1 ) DBG( f1, "    LAYER_B=QPSK " ) ;
		else if( ch_sch_info->MOD_B[ch]==2 ) DBG( f1, "    LAYER_B=16QAM" ) ;
		else if( ch_sch_info->MOD_B[ch]==3 ) DBG( f1, "    LAYER_B=64QAM" ) ;
		else                                 DBG( f1, "    LAYER_B=-----" ) ;

		if(      ch_sch_info->RATE_B[ch]==0 ) DBG( f1, "(1/2) " ) ;
		else if( ch_sch_info->RATE_B[ch]==1 ) DBG( f1, "(2/3) " ) ;
		else if( ch_sch_info->RATE_B[ch]==2 ) DBG( f1, "(3/4) " ) ;
		else if( ch_sch_info->RATE_B[ch]==3 ) DBG( f1, "(5/6) " ) ;
		else if( ch_sch_info->RATE_B[ch]==4 ) DBG( f1, "(7/8) " ) ;
		else                                  DBG( f1, "(-/-) " ) ;

		if(      (ch_sch_info->MODE[ch]==0)&&(ch_sch_info->IL_B[ch]==0) ) DBG( f1, "IL= 0(M1) " ) ;
		else if( (ch_sch_info->MODE[ch]==0)&&(ch_sch_info->IL_B[ch]==1) ) DBG( f1, "IL= 4(M1) " ) ;
		else if( (ch_sch_info->MODE[ch]==0)&&(ch_sch_info->IL_B[ch]==2) ) DBG( f1, "IL= 8(M1) " ) ;
		else if( (ch_sch_info->MODE[ch]==0)&&(ch_sch_info->IL_B[ch]==3) ) DBG( f1, "IL=16(M1) " ) ;
		else if( (ch_sch_info->MODE[ch]==0)&&(ch_sch_info->IL_B[ch]==4) ) DBG( f1, "IL=32(M1) " ) ;
		else if( (ch_sch_info->MODE[ch]==1)&&(ch_sch_info->IL_B[ch]==0) ) DBG( f1, "IL= 0(M2) " ) ;
		else if( (ch_sch_info->MODE[ch]==1)&&(ch_sch_info->IL_B[ch]==1) ) DBG( f1, "IL= 2(M2) " ) ;
		else if( (ch_sch_info->MODE[ch]==1)&&(ch_sch_info->IL_B[ch]==2) ) DBG( f1, "IL= 4(M2) " ) ;
		else if( (ch_sch_info->MODE[ch]==1)&&(ch_sch_info->IL_B[ch]==3) ) DBG( f1, "IL= 8(M2) " ) ;
		else if( (ch_sch_info->MODE[ch]==1)&&(ch_sch_info->IL_B[ch]==4) ) DBG( f1, "IL=16(M2) " ) ;
		else if( (ch_sch_info->MODE[ch]==2)&&(ch_sch_info->IL_B[ch]==0) ) DBG( f1, "IL= 0(M3) " ) ;
		else if( (ch_sch_info->MODE[ch]==2)&&(ch_sch_info->IL_B[ch]==1) ) DBG( f1, "IL= 1(M3) " ) ;
		else if( (ch_sch_info->MODE[ch]==2)&&(ch_sch_info->IL_B[ch]==2) ) DBG( f1, "IL= 2(M3) " ) ;
		else if( (ch_sch_info->MODE[ch]==2)&&(ch_sch_info->IL_B[ch]==3) ) DBG( f1, "IL= 4(M3) " ) ;
		else if( (ch_sch_info->MODE[ch]==2)&&(ch_sch_info->IL_B[ch]==4) ) DBG( f1, "IL= 8(M3) " ) ;
		else                                                              DBG( f1, "IL=------ " ) ;

		if( ch_sch_info->NUM_B[ch]>13 ) DBG( f1, "NUM=--\n" ) ;
		else                            DBG( f1, "NUM=%02d\n", ch_sch_info->NUM_B[ch] ) ;

		if(      ch_sch_info->MOD_C[ch]==0 ) DBG( f1, "    LAYER_C=DQPSK" ) ;
		else if( ch_sch_info->MOD_C[ch]==1 ) DBG( f1, "    LAYER_C=QPSK " ) ;
		else if( ch_sch_info->MOD_C[ch]==2 ) DBG( f1, "    LAYER_C=16QAM" ) ;
		else if( ch_sch_info->MOD_C[ch]==3 ) DBG( f1, "    LAYER_C=64QAM" ) ;
		else                                 DBG( f1, "    LAYER_C=-----" ) ;

		if(      ch_sch_info->RATE_C[ch]==0 ) DBG( f1, "(1/2) " ) ;
		else if( ch_sch_info->RATE_C[ch]==1 ) DBG( f1, "(2/3) " ) ;
		else if( ch_sch_info->RATE_C[ch]==2 ) DBG( f1, "(3/4) " ) ;
		else if( ch_sch_info->RATE_C[ch]==3 ) DBG( f1, "(5/6) " ) ;
		else if( ch_sch_info->RATE_C[ch]==4 ) DBG( f1, "(7/8) " ) ;
		else                                  DBG( f1, "(-/-) " ) ;

		if(      (ch_sch_info->MODE[ch]==0)&&(ch_sch_info->IL_C[ch]==0) ) DBG( f1, "IL= 0(M1) " ) ;
		else if( (ch_sch_info->MODE[ch]==0)&&(ch_sch_info->IL_C[ch]==1) ) DBG( f1, "IL= 4(M1) " ) ;
		else if( (ch_sch_info->MODE[ch]==0)&&(ch_sch_info->IL_C[ch]==2) ) DBG( f1, "IL= 8(M1) " ) ;
		else if( (ch_sch_info->MODE[ch]==0)&&(ch_sch_info->IL_C[ch]==3) ) DBG( f1, "IL=16(M1) " ) ;
		else if( (ch_sch_info->MODE[ch]==0)&&(ch_sch_info->IL_C[ch]==4) ) DBG( f1, "IL=32(M1) " ) ;
		else if( (ch_sch_info->MODE[ch]==1)&&(ch_sch_info->IL_C[ch]==0) ) DBG( f1, "IL= 0(M2) " ) ;
		else if( (ch_sch_info->MODE[ch]==1)&&(ch_sch_info->IL_C[ch]==1) ) DBG( f1, "IL= 2(M2) " ) ;
		else if( (ch_sch_info->MODE[ch]==1)&&(ch_sch_info->IL_C[ch]==2) ) DBG( f1, "IL= 4(M2) " ) ;
		else if( (ch_sch_info->MODE[ch]==1)&&(ch_sch_info->IL_C[ch]==3) ) DBG( f1, "IL= 8(M2) " ) ;
		else if( (ch_sch_info->MODE[ch]==1)&&(ch_sch_info->IL_C[ch]==4) ) DBG( f1, "IL=16(M2) " ) ;
		else if( (ch_sch_info->MODE[ch]==2)&&(ch_sch_info->IL_C[ch]==0) ) DBG( f1, "IL= 0(M3) " ) ;
		else if( (ch_sch_info->MODE[ch]==2)&&(ch_sch_info->IL_C[ch]==1) ) DBG( f1, "IL= 1(M3) " ) ;
		else if( (ch_sch_info->MODE[ch]==2)&&(ch_sch_info->IL_C[ch]==2) ) DBG( f1, "IL= 2(M3) " ) ;
		else if( (ch_sch_info->MODE[ch]==2)&&(ch_sch_info->IL_C[ch]==3) ) DBG( f1, "IL= 4(M3) " ) ;
		else if( (ch_sch_info->MODE[ch]==2)&&(ch_sch_info->IL_C[ch]==4) ) DBG( f1, "IL= 8(M3) " ) ;
		else                                                              DBG( f1, "IL=------ " ) ;

		if( ch_sch_info->NUM_C[ch]>13 ) DBG( f1, "NUM=--\n" ) ;
		else                            DBG( f1, "NUM=%02d\n", ch_sch_info->NUM_C[ch] ) ;

		DBG( f1, "\n" ) ;

	}
	fclose( f1 ) ;
*/
}
#endif

