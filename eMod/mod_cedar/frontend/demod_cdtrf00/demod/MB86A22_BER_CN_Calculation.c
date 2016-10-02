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
//#include <stdio.h>
#include "MB86A22_BER_CN_Calculation.h"
#include "MB86A22_API.h"

REAL32 MB86A2x_Get_VBER_RATE_A( MB86A2x_MON_TIM_PARAM *mon_tim_param, MB86A2x_SIG_QUAL_INFO *sig_qual_info )
{
	double BER ;

	//BER = (REAL32)(sig_qual_info->V_ERR_A)/(REAL32)(mon_tim_param->VBER_A_NUM) ;

	return BER ;
}


REAL32 MB86A2x_Get_VBER_RATE_B( MB86A2x_MON_TIM_PARAM *mon_tim_param, MB86A2x_SIG_QUAL_INFO *sig_qual_info )
{
	double BER ;

	//BER = (REAL32)(sig_qual_info->V_ERR_B)/(REAL32)(mon_tim_param->VBER_B_NUM) ;

	return BER ;
}


REAL32 MB86A2x_Get_VBER_RATE_C( MB86A2x_MON_TIM_PARAM *mon_tim_param, MB86A2x_SIG_QUAL_INFO *sig_qual_info )
{
	double BER ;

	//BER = (REAL32)(sig_qual_info->V_ERR_C)/(REAL32)(mon_tim_param->VBER_C_NUM) ;

	return BER ;
}


REAL32 MB86A2x_Get_SBER_RATE_A( MB86A2x_MON_TIM_PARAM *mon_tim_param, MB86A2x_SIG_QUAL_INFO *sig_qual_info )
{
	double BER ;

	//BER = (REAL32)(sig_qual_info->S_ERR_A)/((REAL32)(mon_tim_param->SBER_A_NUM)*204.0*8.0) ;

	return BER ;
}


REAL32 MB86A2x_Get_SBER_RATE_B( MB86A2x_MON_TIM_PARAM *mon_tim_param, MB86A2x_SIG_QUAL_INFO *sig_qual_info )
{
	double BER ;

	//BER = (REAL32)(sig_qual_info->S_ERR_B)/((REAL32)(mon_tim_param->SBER_B_NUM)*204.0*8.0) ;

	return BER ;
}


REAL32 MB86A2x_Get_SBER_RATE_C( MB86A2x_MON_TIM_PARAM *mon_tim_param, MB86A2x_SIG_QUAL_INFO *sig_qual_info )
{
	double BER ;

	//BER = (REAL32)(sig_qual_info->S_ERR_C)/((REAL32)(mon_tim_param->SBER_C_NUM)*204.0*8.0) ;

	return BER ;
}


REAL32 MB86A2x_Get_PBER_RATE_A( MB86A2x_MON_TIM_PARAM *mon_tim_param, MB86A2x_SIG_QUAL_INFO *sig_qual_info )
{
	double BER ;

	//BER = (REAL32)(sig_qual_info->P_ERR_A)/(REAL32)(mon_tim_param->PBER_A_NUM) ;

	return BER ;
}


REAL32 MB86A2x_Get_PBER_RATE_B( MB86A2x_MON_TIM_PARAM *mon_tim_param, MB86A2x_SIG_QUAL_INFO *sig_qual_info )
{
	double BER ;

	//BER = (REAL32)(sig_qual_info->P_ERR_B)/(REAL32)(mon_tim_param->PBER_B_NUM) ;

	return BER ;
}


REAL32 MB86A2x_Get_PBER_RATE_C( MB86A2x_MON_TIM_PARAM *mon_tim_param, MB86A2x_SIG_QUAL_INFO *sig_qual_info )
{
	double BER ;

	//BER = (REAL32)(sig_qual_info->P_ERR_C)/(REAL32)(mon_tim_param->PBER_C_NUM) ;

	return BER ;
}


REAL32 MB86A2x_Get_CN_Value_dB( MB86A2x_SIG_QUAL_INFO *sig_qual_info )
{
	REAL32 cn_value_db ;

	//cn_value_db = (REAL32)(MB86A2x_CNval_to_CN10(sig_qual_info->CN_MON))/10.0;

	return cn_value_db ;
}


UINT32 MB86A2x_CNval_to_CN10( UINT32 cn_read_value )
{
	UINT32 ret ;
	SINT32 A, B, C ;

	if(      cn_read_value>=18187 ) { A= 10 ; B=1461 ; C=18187 ; }
	else if( cn_read_value>=16534 ) { A= 20 ; B=1653 ; C=16534 ; }
	else if( cn_read_value>=14823 ) { A= 30 ; B=1711 ; C=14823 ; }
	else if( cn_read_value>=13161 ) { A= 40 ; B=1662 ; C=13161 ; }
	else if( cn_read_value>=11622 ) { A= 50 ; B=1539 ; C=11622 ; }
	else if( cn_read_value>=10279 ) { A= 60 ; B=1343 ; C=10279 ; }
	else if( cn_read_value>= 9089 ) { A= 70 ; B=1190 ; C= 9089 ; }
	else if( cn_read_value>= 8042 ) { A= 80 ; B=1047 ; C= 8042 ; }
	else if( cn_read_value>= 7137 ) { A= 90 ; B= 905 ; C= 7137 ; }
	else if( cn_read_value>= 6342 ) { A=100 ; B= 795 ; C= 6342 ; }
	else if( cn_read_value>= 5641 ) { A=110 ; B= 701 ; C= 5641 ; }
	else if( cn_read_value>= 5030 ) { A=120 ; B= 611 ; C= 5030 ; }
	else if( cn_read_value>= 4474 ) { A=130 ; B= 556 ; C= 4474 ; }
	else if( cn_read_value>= 3988 ) { A=140 ; B= 486 ; C= 3988 ; }
	else if( cn_read_value>= 3556 ) { A=150 ; B= 432 ; C= 3556 ; }
	else if( cn_read_value>= 3180 ) { A=160 ; B= 376 ; C= 3180 ; }
	else if( cn_read_value>= 2841 ) { A=170 ; B= 339 ; C= 2841 ; }
	else if( cn_read_value>= 2541 ) { A=180 ; B= 300 ; C= 2541 ; }
	else if( cn_read_value>= 2276 ) { A=190 ; B= 265 ; C= 2276 ; }
	else if( cn_read_value>= 2038 ) { A=200 ; B= 238 ; C= 2038 ; }
	else if( cn_read_value>= 1800 ) { A=210 ; B= 238 ; C= 1800 ; }
	else if( cn_read_value>= 1625 ) { A=220 ; B= 175 ; C= 1625 ; }
	else if( cn_read_value>= 1462 ) { A=230 ; B= 163 ; C= 1462 ; }
	else if( cn_read_value>= 1324 ) { A=240 ; B= 138 ; C= 1324 ; }
	else if( cn_read_value>= 1175 ) { A=250 ; B= 149 ; C= 1175 ; }
	else if( cn_read_value>= 1063 ) { A=260 ; B= 112 ; C= 1063 ; }
	else if( cn_read_value>=  980 ) { A=270 ; B=  83 ; C=  980 ; }
	else if( cn_read_value>=  907 ) { A=280 ; B=  73 ; C=  907 ; }
	else if( cn_read_value>=  840 ) { A=290 ; B=  67 ; C=  840 ; }
	else if( cn_read_value>=  788 ) { A=300 ; B=  52 ; C=  788 ; }

	if(      cn_read_value>=19648 ) ret = 0 ;
	else if( cn_read_value<   788 ) ret = 300 ;
	else                            ret = (UINT32)( A-(10*((SINT32)(cn_read_value)-C)-B/2)/B) ;

	return ret ;
}


REAL32 MB86A2x_Get_MER_A_Value_dB( MB86A2x_TMCC_INFO *tmcc_info, MB86A2x_SIG_QUAL_INFO *sig_qual_info )
{
	REAL32 mer_value_db ;
/*
	if(      tmcc_info->MOD_A_current==0 ) mer_value_db = (REAL32)(MB86A2x_MERval_to_CN10_DQPSK( sig_qual_info->MER_MON_A ))/10.0 ;
	else if( tmcc_info->MOD_A_current==1 ) mer_value_db = (REAL32)(MB86A2x_MERval_to_CN10_QPSK(  sig_qual_info->MER_MON_A ))/10.0 ;
	else if( tmcc_info->MOD_A_current==2 ) mer_value_db = (REAL32)(MB86A2x_MERval_to_CN10_16QAM( sig_qual_info->MER_MON_A ))/10.0 ;
	else if( tmcc_info->MOD_A_current==3 ) mer_value_db = (REAL32)(MB86A2x_MERval_to_CN10_64QAM( sig_qual_info->MER_MON_A ))/10.0 ;
*/
	return mer_value_db ;
}


REAL32 MB86A2x_Get_MER_B_Value_dB( MB86A2x_TMCC_INFO *tmcc_info, MB86A2x_SIG_QUAL_INFO *sig_qual_info )
{
	REAL32 mer_value_db ;

	//if(      tmcc_info->MOD_B_current==0 ) mer_value_db = (REAL32)(MB86A2x_MERval_to_CN10_DQPSK( sig_qual_info->MER_MON_B ))/10.0 ;
	//else if( tmcc_info->MOD_B_current==1 ) mer_value_db = (REAL32)(MB86A2x_MERval_to_CN10_QPSK(  sig_qual_info->MER_MON_B ))/10.0 ;
	//else if( tmcc_info->MOD_B_current==2 ) mer_value_db = (REAL32)(MB86A2x_MERval_to_CN10_16QAM( sig_qual_info->MER_MON_B ))/10.0 ;
	//else if( tmcc_info->MOD_B_current==3 ) mer_value_db = (REAL32)(MB86A2x_MERval_to_CN10_64QAM( sig_qual_info->MER_MON_B ))/10.0 ;

	return mer_value_db ;
}


REAL32 MB86A2x_Get_MER_C_Value_dB( MB86A2x_TMCC_INFO *tmcc_info, MB86A2x_SIG_QUAL_INFO *sig_qual_info )
{
	REAL32 mer_value_db ;

	//if(      tmcc_info->MOD_C_current==0 ) mer_value_db = (REAL32)(MB86A2x_MERval_to_CN10_DQPSK( sig_qual_info->MER_MON_C ))/10.0 ;
	//else if( tmcc_info->MOD_C_current==1 ) mer_value_db = (REAL32)(MB86A2x_MERval_to_CN10_QPSK(  sig_qual_info->MER_MON_C ))/10.0 ;
	//else if( tmcc_info->MOD_C_current==2 ) mer_value_db = (REAL32)(MB86A2x_MERval_to_CN10_16QAM( sig_qual_info->MER_MON_C ))/10.0 ;
	//else if( tmcc_info->MOD_C_current==3 ) mer_value_db = (REAL32)(MB86A2x_MERval_to_CN10_64QAM( sig_qual_info->MER_MON_C ))/10.0 ;

	return mer_value_db ;
}


UINT32 MB86A2x_MERval_to_CN10_DQPSK( UINT32 mer_read_value )
{
	SINT32 ret ;
	SINT32 A, B, C ;

	if(      mer_read_value>=16293 ) { A= 10 ; B=  50 ; C=16293 ; }
	else if( mer_read_value>=16177 ) { A= 20 ; B= 116 ; C=16177 ; }
	else if( mer_read_value>=15994 ) { A= 30 ; B= 183 ; C=15994 ; }
	else if( mer_read_value>=15650 ) { A= 40 ; B= 344 ; C=15650 ; }
	else if( mer_read_value>=15091 ) { A= 50 ; B= 559 ; C=15091 ; }
	else if( mer_read_value>=14265 ) { A= 60 ; B= 826 ; C=14265 ; }
	else if( mer_read_value>=13142 ) { A= 70 ; B=1123 ; C=13142 ; }
	else if( mer_read_value>=11760 ) { A= 80 ; B=1382 ; C=11760 ; }
	else if( mer_read_value>=10202 ) { A= 90 ; B=1558 ; C=10202 ; }
	else if( mer_read_value>= 8607 ) { A=100 ; B=1595 ; C= 8607 ; }
	else if( mer_read_value>= 7090 ) { A=110 ; B=1517 ; C= 7090 ; }
	else if( mer_read_value>= 5744 ) { A=120 ; B=1346 ; C= 5744 ; }
	else if( mer_read_value>= 4608 ) { A=130 ; B=1136 ; C= 4608 ; }
	else if( mer_read_value>= 3685 ) { A=140 ; B= 923 ; C= 3685 ; }
	else if( mer_read_value>= 2950 ) { A=150 ; B= 735 ; C= 2950 ; }
	else if( mer_read_value>= 2365 ) { A=160 ; B= 585 ; C= 2365 ; }
	else if( mer_read_value>= 1903 ) { A=170 ; B= 462 ; C= 1903 ; }
	else if( mer_read_value>= 1538 ) { A=180 ; B= 365 ; C= 1538 ; }
	else if( mer_read_value>= 1262 ) { A=190 ; B= 276 ; C= 1262 ; }
	else if( mer_read_value>= 1028 ) { A=200 ; B= 234 ; C= 1028 ; }
	else if( mer_read_value>=  822 ) { A=210 ; B= 206 ; C=  822 ; }
	else if( mer_read_value>=  683 ) { A=220 ; B= 139 ; C=  683 ; }
	else if( mer_read_value>=  580 ) { A=230 ; B= 103 ; C=  580 ; }
	else if( mer_read_value>=  487 ) { A=240 ; B=  93 ; C=  487 ; }
	else if( mer_read_value>=  401 ) { A=250 ; B=  86 ; C=  401 ; }
	else if( mer_read_value>=  348 ) { A=260 ; B=  53 ; C=  348 ; }
	else if( mer_read_value>=  312 ) { A=270 ; B=  36 ; C=  312 ; }
	else if( mer_read_value>=  279 ) { A=280 ; B=  33 ; C=  279 ; }
	else if( mer_read_value>=  254 ) { A=290 ; B=  25 ; C=  254 ; }
	else if( mer_read_value>=  235 ) { A=300 ; B=  19 ; C=  235 ; }

	if(      mer_read_value>=16343 ) ret = 0 ;
	else if( mer_read_value<   235 ) ret = 300 ;
	else                    ret = (UINT32)( A-(10*((SINT32)(mer_read_value)-C)-B/2)/B) ;
	return ret ;
}


UINT32 MB86A2x_MERval_to_CN10_QPSK( UINT32 mer_read_value )
{
	SINT32 ret ;
	SINT32 A, B, C ;

	if(      mer_read_value>=10483 ) { A= 10 ; B= 588 ; C=10483 ; }
	else if( mer_read_value>= 9910 ) { A= 20 ; B= 573 ; C= 9910 ; }
	else if( mer_read_value>= 9343 ) { A= 30 ; B= 567 ; C= 9343 ; }
	else if( mer_read_value>= 8333 ) { A= 40 ; B=1010 ; C= 8333 ; }
	else if( mer_read_value>= 7446 ) { A= 50 ; B= 887 ; C= 7446 ; }
	else if( mer_read_value>= 6510 ) { A= 60 ; B= 936 ; C= 6510 ; }
	else if( mer_read_value>= 5555 ) { A= 70 ; B= 955 ; C= 5555 ; }
	else if( mer_read_value>= 4647 ) { A= 80 ; B= 908 ; C= 4647 ; }
	else if( mer_read_value>= 3814 ) { A= 90 ; B= 833 ; C= 3814 ; }
	else if( mer_read_value>= 3087 ) { A=100 ; B= 727 ; C= 3087 ; }
	else if( mer_read_value>= 2474 ) { A=110 ; B= 613 ; C= 2474 ; }
	else if( mer_read_value>= 1975 ) { A=120 ; B= 499 ; C= 1975 ; }
	else if( mer_read_value>= 1574 ) { A=130 ; B= 401 ; C= 1574 ; }
	else if( mer_read_value>= 1253 ) { A=140 ; B= 321 ; C= 1253 ; }
	else if( mer_read_value>=  998 ) { A=150 ; B= 255 ; C=  998 ; }
	else if( mer_read_value>=  797 ) { A=160 ; B= 201 ; C=  797 ; }
	else if( mer_read_value>=  637 ) { A=170 ; B= 160 ; C=  637 ; }
	else if( mer_read_value>=  509 ) { A=180 ; B= 128 ; C=  509 ; }
	else if( mer_read_value>=  411 ) { A=190 ; B=  98 ; C=  411 ; }
	else if( mer_read_value>=  326 ) { A=200 ; B=  85 ; C=  326 ; }
	else if( mer_read_value>=  254 ) { A=210 ; B=  72 ; C=  254 ; }
	else if( mer_read_value>=  205 ) { A=220 ; B=  49 ; C=  205 ; }
	else if( mer_read_value>=  167 ) { A=230 ; B=  38 ; C=  167 ; }
	else if( mer_read_value>=  135 ) { A=240 ; B=  32 ; C=  135 ; }
	else if( mer_read_value>=  106 ) { A=250 ; B=  29 ; C=  106 ; }
	else if( mer_read_value>=   86 ) { A=260 ; B=  20 ; C=   86 ; }
	else if( mer_read_value>=   72 ) { A=270 ; B=  14 ; C=   72 ; }
	else if( mer_read_value>=   61 ) { A=280 ; B=  11 ; C=   61 ; }
	else if( mer_read_value>=   52 ) { A=290 ; B=   9 ; C=   52 ; }
	else if( mer_read_value>=   45 ) { A=300 ; B=   7 ; C=   45 ; }

	if(      mer_read_value>=11071 ) ret = 0 ;
	else if( mer_read_value<    45 ) ret = 300 ;
	else                    ret = (UINT32)( A-(10*((SINT32)(mer_read_value)-C)-B/2)/B) ;
	return ret ;
}


UINT32 MB86A2x_MERval_to_CN10_16QAM( UINT32 mer_read_value )
{
	SINT32 ret ;
	SINT32 A, B, C ;

	if(      mer_read_value>=20387 ) { A= 10 ; B= 374 ; C=20387 ; }
	else if( mer_read_value>=19995 ) { A= 20 ; B= 392 ; C=19995 ; }
	else if( mer_read_value>=19527 ) { A= 30 ; B= 468 ; C=19527 ; }
	else if( mer_read_value>=19045 ) { A= 40 ; B= 482 ; C=19045 ; }
	else if( mer_read_value>=18500 ) { A= 50 ; B= 545 ; C=18500 ; }
	else if( mer_read_value>=17985 ) { A= 60 ; B= 515 ; C=17985 ; }
	else if( mer_read_value>=17417 ) { A= 70 ; B= 568 ; C=17417 ; }
	else if( mer_read_value>=16798 ) { A= 80 ; B= 619 ; C=16798 ; }
	else if( mer_read_value>=15988 ) { A= 90 ; B= 810 ; C=15988 ; }
	else if( mer_read_value>=14985 ) { A=100 ; B=1003 ; C=14985 ; }
	else if( mer_read_value>=13754 ) { A=110 ; B=1231 ; C=13754 ; }
	else if( mer_read_value>=12328 ) { A=120 ; B=1426 ; C=12328 ; }
	else if( mer_read_value>=10769 ) { A=130 ; B=1559 ; C=10769 ; }
	else if( mer_read_value>= 9168 ) { A=140 ; B=1601 ; C= 9168 ; }
	else if( mer_read_value>= 7637 ) { A=150 ; B=1531 ; C= 7637 ; }
	else if( mer_read_value>= 6225 ) { A=160 ; B=1382 ; C= 6255 ; }
	else if( mer_read_value>= 5070 ) { A=170 ; B=1185 ; C= 5070 ; }
	else if( mer_read_value>= 4078 ) { A=180 ; B= 992 ; C= 4078 ; }
	else if( mer_read_value>= 3280 ) { A=190 ; B= 798 ; C= 3280 ; }
	else if( mer_read_value>= 2626 ) { A=200 ; B= 654 ; C= 2626 ; }
	else if( mer_read_value>= 2043 ) { A=210 ; B= 583 ; C= 2043 ; }
	else if( mer_read_value>= 1659 ) { A=220 ; B= 384 ; C= 1659 ; }
	else if( mer_read_value>= 1348 ) { A=230 ; B= 311 ; C= 1348 ; }
	else if( mer_read_value>= 1094 ) { A=240 ; B= 254 ; C= 1094 ; }
	else if( mer_read_value>=  865 ) { A=250 ; B= 229 ; C=  865 ; }
	else if( mer_read_value>=  702 ) { A=260 ; B= 163 ; C=  702 ; }
	else if( mer_read_value>=  590 ) { A=270 ; B= 112 ; C=  590 ; }
	else if( mer_read_value>=  502 ) { A=280 ; B=  88 ; C=  502 ; }
	else if( mer_read_value>=  430 ) { A=290 ; B=  72 ; C=  430 ; }
	else if( mer_read_value>=  374 ) { A=300 ; B=  56 ; C=  374 ; }

	if(      mer_read_value>=20761 ) ret = 0 ;
	else if( mer_read_value<   374 ) ret = 300 ;
	else                    ret = (UINT32)( A-(10*((SINT32)(mer_read_value)-C)-B/2)/B) ;
	return ret ;
}


UINT32 MB86A2x_MERval_to_CN10_64QAM( UINT32 mer_read_value )
{
	SINT32 ret ;
	SINT32 A, B, C ;

	if(      mer_read_value>=15314 ) { A= 10 ; B=   9 ; C=15314 ; }
	else if( mer_read_value>=15245 ) { A= 20 ; B=  69 ; C=15245 ; }
	else if( mer_read_value>=15214 ) { A= 30 ; B=  31 ; C=15214 ; }
	else if( mer_read_value>=15166 ) { A= 40 ; B=  48 ; C=15166 ; }
	else if( mer_read_value>=15128 ) { A= 50 ; B=  38 ; C=15128 ; }
	else if( mer_read_value>=15074 ) { A= 60 ; B=  54 ; C=15074 ; }
	else if( mer_read_value>=15045 ) { A= 70 ; B=  29 ; C=15045 ; }
	else if( mer_read_value>=14996 ) { A= 80 ; B=  49 ; C=14996 ; }
	else if( mer_read_value>=14958 ) { A= 90 ; B=  38 ; C=14958 ; }
	else if( mer_read_value>=14915 ) { A=100 ; B=  43 ; C=14915 ; }
	else if( mer_read_value>=14871 ) { A=110 ; B=  44 ; C=14871 ; }
	else if( mer_read_value>=14812 ) { A=120 ; B=  59 ; C=14812 ; }
	else if( mer_read_value>=14715 ) { A=130 ; B=  97 ; C=14715 ; }
	else if( mer_read_value>=14535 ) { A=140 ; B= 180 ; C=14535 ; }
	else if( mer_read_value>=14209 ) { A=150 ; B= 326 ; C=14209 ; }
	else if( mer_read_value>=13666 ) { A=160 ; B= 543 ; C=13666 ; }
	else if( mer_read_value>=12875 ) { A=170 ; B= 791 ; C=12875 ; }
	else if( mer_read_value>=11840 ) { A=180 ; B=1035 ; C=11840 ; }
	else if( mer_read_value>=10607 ) { A=190 ; B=1233 ; C=10607 ; }
	else if( mer_read_value>= 9229 ) { A=200 ; B=1378 ; C= 9229 ; }
	else if( mer_read_value>= 7669 ) { A=210 ; B=1560 ; C= 7669 ; }
	else if( mer_read_value>= 6444 ) { A=220 ; B=1225 ; C= 6444 ; }
	else if( mer_read_value>= 5339 ) { A=230 ; B=1105 ; C= 5339 ; }
	else if( mer_read_value>= 4378 ) { A=240 ; B= 961 ; C= 4378 ; }
	else if( mer_read_value>= 3480 ) { A=250 ; B= 898 ; C= 3480 ; }
	else if( mer_read_value>= 2826 ) { A=260 ; B= 654 ; C= 2826 ; }
	else if( mer_read_value>= 2391 ) { A=270 ; B= 435 ; C= 2391 ; }
	else if( mer_read_value>= 2027 ) { A=280 ; B= 364 ; C= 2027 ; }
	else if( mer_read_value>= 1751 ) { A=290 ; B= 276 ; C= 1751 ; }
	else if( mer_read_value>= 1519 ) { A=300 ; B= 232 ; C= 1519 ; }

	if(      mer_read_value>=15323 ) ret = 0 ;
	else if( mer_read_value<  1519 ) ret = 300 ;
	else                    ret = (UINT32)( A-(10*((SINT32)(mer_read_value)-C)-B/2)/B) ;
	return ret ;
}
