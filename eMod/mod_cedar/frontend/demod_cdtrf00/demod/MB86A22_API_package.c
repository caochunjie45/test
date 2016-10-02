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
#include "MB86A22_API_defs.h"
#include "MB86A22_API_structs.h"
#include "MB86A22_API.h"
#include "Fujitsu_printf.h"

MB86A2x_ERR_MSG MB86A2x_Monitor1( MB86A2x_PARAM myfe, MB86A2x_LOCKNG_INFO *locking_info ) {

	MB86A2x_ERR_MSG       ERR_MSG ;

	MB86A2x_AGC_INFO      agc_info ;		/* prepare pointer(4/8) */
	MB86A2x_FREQ_ERR_INFO freq_err_info ;	/* prepare pointer(5/8) */

	//Read the register value to check IF input level.
	ERR_MSG = MB86A2x_Get_IF_level( &myfe.nim_param, &agc_info ) ;
	#ifdef FUJITSU_MSG_PRINT
	FUJITSU_printf_IF_input_level( &agc_info ) ;
	#endif

	//Read the register value to check IFAGCDAC output level.
	ERR_MSG = MB86A2x_Get_IFAGCDAC( &myfe.nim_param, &agc_info ) ;
	#ifdef FUJITSU_MSG_PRINT
	FUJITSU_printf_IFAGCDAC( &agc_info ) ;
	#endif
        // ylzhu add rfagc here  & rda function
        
	//Read register values to check state value.
	ERR_MSG = MB86A2x_Get_SEQ_value( &myfe.nim_param, locking_info ) ;
	#ifdef FUJITSU_MSG_PRINT
	FUJITSU_printf_SEQUENCE_value( locking_info ) ;
	#endif

	//Read register values to check frequency error detected by timing recovery.
	ERR_MSG = MB86A2x_Get_STR_ppm_Error( &myfe, &freq_err_info ) ;
	#ifdef FUJITSU_MSG_PRINT
	FUJITSU_printf_STR_Freq_Error( &freq_err_info ) ;
	#endif

	//Read register values to check frequency error detected by carrier recovery.
	ERR_MSG = MB86A2x_Get_CRR_Freq_Error( &myfe, &freq_err_info ) ;
	#ifdef FUJITSU_MSG_PRINT
	FUJITSU_printf_CRR_Freq_Error( &freq_err_info ) ;
	#endif

	return ERR_MSG ;
}


MB86A2x_ERR_MSG MB86A2x_Monitor2( MB86A2x_PARAM myfe, MB86A2x_SIG_QUAL_INFO *sign_qual_info ) {

	MB86A2x_ERR_MSG       ERR_MSG ;

	//Read register values to check whether C/N measurement is finished or not and store the result to CN_update.
	ERR_MSG = MB86A2x_Get_CN_Flag( &myfe.nim_param, sign_qual_info ) ;
	if( sign_qual_info->CN_update==1 ) {	//Check whether whether C/N measurement is finished or not by checking the value of CN_update.
		//Read register values to know C/N value and calculate estimated C/N.
		ERR_MSG = MB86A2x_Get_CN_Value( &myfe.nim_param, sign_qual_info ) ;
		//Reset C/N counter.
		ERR_MSG = MB86A2x_Reset_CN_Counter( &myfe.nim_param, &myfe.mon_tim_param ) ;
	}

	return ERR_MSG ;
}

MB86A2x_ERR_MSG MB86A2x_Monitor3( MB86A2x_PARAM myfe, MB86A2x_TMCC_INFO *tmcc_info, MB86A2x_SIG_QUAL_INFO *sign_qual_info ) {

	MB86A2x_ERR_MSG       ERR_MSG ;
	if (myfe.pll_param.BANDSEL == MB86A2x_BANDSEL_ISDBT_6MHz || myfe.pll_param.BANDSEL == MB86A2x_BANDSEL_ISDBT_7MHz || myfe.pll_param.BANDSEL == MB86A2x_BANDSEL_ISDBT_8MHz){
		if( (1<=tmcc_info->NUM_A_current)&&(tmcc_info->NUM_A_current<=13) ) {	//Check whether the segment number of layer-A is within the range of 1~13.
			//Read register values to check whether pre-Viterbi BER of layer-A is finished or not and store the result to VBER_A_update.
			ERR_MSG = MB86A2x_Get_VBER_Flag( &myfe.nim_param, sign_qual_info ) ;
			if( sign_qual_info->VBER_A_update==1 ) {	//Check whether pre-Viterbi BER of layer-A is finished or not by checking the value of VBER_A_update.
				//Read register values to know pre-Viterbi BER of layer-A and calculate pre-Viterbi BER.
				ERR_MSG = MB86A2x_Get_VBER_A( &myfe.nim_param, &myfe.mon_tim_param, sign_qual_info ) ;
				//Reset BER counter for pre-Viterbi of layer-A.
				ERR_MSG = MB86A2x_Reset_VBER_A_Counter( &myfe.nim_param ) ;
			}
			//Read register values to check whether post-Viterbi BER of layer-A is finished or not and store the result to SBER_A_update.
			ERR_MSG = MB86A2x_Get_SBER_Flag( &myfe.nim_param, sign_qual_info ) ;
			if( sign_qual_info->SBER_A_update==1 ) {	//Check whether post-Viterbi BER of layer-A is finished or not by checking the value of SBER_A_update.
				//Read register values to know post-Viterbi BER of layer-A and calculate post-Viterbi BER.
				ERR_MSG = MB86A2x_Get_SBER_A( &myfe.nim_param, &myfe.mon_tim_param, sign_qual_info ) ;
				//Reset BER counter for post-Viterbi of layer-A.
				ERR_MSG = MB86A2x_Reset_SBER_A_Counter( &myfe.nim_param ) ;
			}
			//Read register values to check whether post-RS BER of layer-A is finished or not and store the result to PBER_A_update.
			ERR_MSG = MB86A2x_Get_PBER_Flag( &myfe.nim_param, sign_qual_info ) ;
			if( sign_qual_info->PBER_A_update==1 ) {	//Check whether post-RS BER of layer-A is finished or not by checking the value of PBER_A_update.
				//Read register values to know post-RS BER of layer-A and calculate post-RS BER.
				ERR_MSG = MB86A2x_Get_PBER_A( &myfe.nim_param, &myfe.mon_tim_param, sign_qual_info ) ;
				//Reset BER counter for post-RS of layer-A.
				ERR_MSG = MB86A2x_Reset_PBER_A_Counter( &myfe.nim_param ) ;
			}
		}
		#ifdef FUJITSU_MSG_PRINT
		FUJITSU_printf_BER_Lay_A( &myfe.mon_tim_param, sign_qual_info, tmcc_info ) ;
		#endif
		if( (1<=tmcc_info->NUM_B_current)&&(tmcc_info->NUM_B_current<=12) ) {	//Check whether the segment number of layer-B is within the range of 1~12.
			//Read register values to check whether pre-Viterbi BER of layer-B is finished or not and store the result to VBER_update.
			ERR_MSG = MB86A2x_Get_VBER_Flag( &myfe.nim_param, sign_qual_info ) ;
			if( sign_qual_info->VBER_B_update==1 ) {	//Check whether pre-Viterbi BER of layer-B is finished or not by checking the value of VBER_B_update.
				//Read register values to know pre-Viterbi BER of layer-B and calculate pre-Viterbi BER.
				ERR_MSG = MB86A2x_Get_VBER_B( &myfe.nim_param, &myfe.mon_tim_param, sign_qual_info ) ;
				//Reset BER counter for pre-Viterbi of layer-B.
				ERR_MSG = MB86A2x_Reset_VBER_B_Counter( &myfe.nim_param ) ;
			}
			//Read register values to check whether post-Viterbi BER of layer-B is finished or not and store the result to SBER_B_update.
			ERR_MSG = MB86A2x_Get_SBER_Flag( &myfe.nim_param, sign_qual_info ) ;
			if( sign_qual_info->SBER_B_update==1 ) {	//Check whether post-Viterbi BER of layer-B is finished or not by checking the value of SBER_B_update.
				//Read register values to know post-Viterbi BER of layer-B and calculate post-Viterbi BER.
				ERR_MSG = MB86A2x_Get_SBER_B( &myfe.nim_param, &myfe.mon_tim_param, sign_qual_info ) ;
				//Reset BER counter for post-Viterbi of layer-B.
				ERR_MSG = MB86A2x_Reset_SBER_B_Counter( &myfe.nim_param ) ;
			}
			//Read register values to check whether post-RS BER of layer-B is finished or not and store 
			ERR_MSG = MB86A2x_Get_PBER_Flag( &myfe.nim_param, sign_qual_info ) ;
			if( sign_qual_info->PBER_B_update==1 ) {	//Check whether post-RS BER of layer-B is finished or not by checking the value of PBER_B_update.
				//Read register values to know post-RS BER of layer-B and calculate post-RS BER.
				ERR_MSG = MB86A2x_Get_PBER_B( &myfe.nim_param, &myfe.mon_tim_param, sign_qual_info ) ;
				//Reset BER counter for post-RS of layer-B.
				ERR_MSG = MB86A2x_Reset_PBER_B_Counter( &myfe.nim_param ) ;
			}
		}
		#ifdef FUJITSU_MSG_PRINT
		FUJITSU_printf_BER_Lay_B( &myfe.mon_tim_param, sign_qual_info, tmcc_info ) ;
		#endif

		if( (1<=tmcc_info->NUM_C_current)&&(tmcc_info->NUM_C_current<=11) ) {	//Check whether the segment number of layer-C is within the range of 1~11.
			//Read register values to check whether pre-Viterbi BER of layer-C is finished or not and store the result to VBER_update.
			ERR_MSG = MB86A2x_Get_VBER_Flag( &myfe.nim_param, sign_qual_info ) ;
			if( sign_qual_info->VBER_C_update==1 ) {	//Check whether pre-Viterbi BER of layer-C is finished or not by checking the value of VBER_C_update.
				//Read register values to know pre-Viterbi BER of layer-C and calculate pre-Viterbi BER.
				ERR_MSG = MB86A2x_Get_VBER_C( &myfe.nim_param, &myfe.mon_tim_param, sign_qual_info ) ;
				//Reset BER counter for pre-Viterbi of layer-C.
				ERR_MSG = MB86A2x_Reset_VBER_C_Counter( &myfe.nim_param ) ;
			}
			//Read register values to check whether post-Viterbi BER of layer-C is finished or not and store the result to SBER_C_update.
			ERR_MSG = MB86A2x_Get_SBER_Flag( &myfe.nim_param, sign_qual_info ) ;
			if( sign_qual_info->SBER_C_update==1 ) {	//Check whether post-Viterbi BER of layer-C is finished or not by checking the value of SBER_C_update.
				//Read register values to know post-Viterbi BER of layer-C and calculate post-Viterbi BER.
				ERR_MSG = MB86A2x_Get_SBER_C( &myfe.nim_param, &myfe.mon_tim_param, sign_qual_info ) ;
				//Reset BER counter for post-Viterbi of Layer-C.
				ERR_MSG = MB86A2x_Reset_SBER_C_Counter( &myfe.nim_param ) ;
			}
			//Read register values to check whether post-RS BER of layer-C is finished or not and store the result to PBER_C_update.
			ERR_MSG = MB86A2x_Get_PBER_Flag( &myfe.nim_param, sign_qual_info ) ;
			if( sign_qual_info->PBER_C_update==1 ) {	//Check whether post-RS BER of layer-C is finished or not by checking the value of PBER_C_update.
				//Read register values to know post-RS BER of layer-C and calculate post-RS BER.
				ERR_MSG = MB86A2x_Get_PBER_C( &myfe.nim_param, &myfe.mon_tim_param, sign_qual_info ) ;
				//Reset BER counter for post-RS of layer-C.
				ERR_MSG = MB86A2x_Reset_PBER_C_Counter( &myfe.nim_param ) ;
			}
		}
		#ifdef FUJITSU_MSG_PRINT
		FUJITSU_printf_BER_Lay_C( &myfe.mon_tim_param, sign_qual_info, tmcc_info ) ;
		#endif
	}
	else if (myfe.pll_param.BANDSEL == MB86A2x_BANDSEL_DVBT_6MHz || myfe.pll_param.BANDSEL == MB86A2x_BANDSEL_DVBT_7MHz || myfe.pll_param.BANDSEL == MB86A2x_BANDSEL_DVBT_8MHz){
			//Read register values to check whether pre-Viterbi BER of layer-A is finished or not and store the result to VBER_A_update.
			ERR_MSG = MB86A2x_Get_VBER_Flag( &myfe.nim_param, sign_qual_info ) ;
			if( sign_qual_info->VBER_A_update==1 ) {	//Check whether pre-Viterbi BER of layer-A is finished or not by checking the value of VBER_A_update.
				//Read register values to know pre-Viterbi BER of layer-A and calculate pre-Viterbi BER.
				ERR_MSG = MB86A2x_Get_VBER_A( &myfe.nim_param, &myfe.mon_tim_param, sign_qual_info ) ;
				//Reset BER counter for pre-Viterbi of layer-A.
				ERR_MSG = MB86A2x_Reset_VBER_A_Counter( &myfe.nim_param ) ;
			}
			//Read register values to check whether post-Viterbi BER of layer-A is finished or not and store the result to SBER_A_update.
			ERR_MSG = MB86A2x_Get_SBER_Flag( &myfe.nim_param, sign_qual_info ) ;
			if( sign_qual_info->SBER_A_update==1 ) {	//Check whether post-Viterbi BER of layer-A is finished or not by checking the value of SBER_A_update.
				//Read register values to know post-Viterbi BER of layer-A and calculate post-Viterbi BER.
				ERR_MSG = MB86A2x_Get_SBER_A( &myfe.nim_param, &myfe.mon_tim_param, sign_qual_info ) ;
				//Reset BER counter for post-Viterbi of layer-A.
				ERR_MSG = MB86A2x_Reset_SBER_A_Counter( &myfe.nim_param ) ;
			}
			//Read register values to check whether post-RS BER of layer-A is finished or not and store the result to PBER_A_update.
			ERR_MSG = MB86A2x_Get_PBER_Flag( &myfe.nim_param, sign_qual_info ) ;
			if( sign_qual_info->PBER_A_update==1 ) {	//Check whether post-RS BER of layer-A is finished or not by checking the value of PBER_A_update.
				//Read register values to know post-RS BER of layer-A and calculate post-RS BER.
				ERR_MSG = MB86A2x_Get_PBER_A( &myfe.nim_param, &myfe.mon_tim_param, sign_qual_info ) ;
				//Reset BER counter for post-RS of layer-A.
				ERR_MSG = MB86A2x_Reset_PBER_A_Counter( &myfe.nim_param ) ;
			}
		#ifdef FUJITSU_MSG_PRINT
		FUJITSU_printf_BER_Lay_A( &myfe.mon_tim_param, sign_qual_info, tmcc_info ) ;
		#endif
	}
	//Read register values to check whether MER measurement is finished or not and store the result to MER_update.
	ERR_MSG = MB86A2x_Get_MER_Flag( &myfe.nim_param, sign_qual_info ) ;
	if( sign_qual_info->MER_update==1 ) {	//Check whether whether MER measurement is finished or not by checking the value of MER_update.
		//Read register values to know MER value and calculate estimated C/N.
		ERR_MSG = MB86A2x_Get_MER_A_Value( &myfe.nim_param, tmcc_info, sign_qual_info ) ;
		ERR_MSG = MB86A2x_Get_MER_B_Value( &myfe.nim_param, tmcc_info, sign_qual_info ) ;
		ERR_MSG = MB86A2x_Get_MER_C_Value( &myfe.nim_param, tmcc_info, sign_qual_info ) ;
		//Reset MER counter.
		ERR_MSG = MB86A2x_Reset_MER_Counter( &myfe.nim_param ) ;
	}
	#ifdef FUJITSU_MSG_PRINT
	FUJITSU_printf_MER( sign_qual_info, tmcc_info ) ;
	#endif

	return ERR_MSG ;
}


MB86A2x_ERR_MSG Reception_before_Channel_Search_Fixed_Timer_Periods_1( 
				MB86A2x_PARAM *myfe, MB86A2x_LOCKNG_INFO *locking_info,
				MB86A2x_SIG_QUAL_INFO *sign_qual_info,
				UINT32 ch, MB86A2x_RECEP_LAYER recep_layer )
{

	MB86A2x_ERR_MSG       ERR_MSG ;

	//Initialize the parameters of LOCKING_INFO.
	ERR_MSG = MB86A2x_Init_LOCKNG_INFO( locking_info ) ;

	//Initialize the parameters of SIG_QUAL_INFO.
	ERR_MSG = MB86A2x_Init_SIG_QUAL_INFO( sign_qual_info ) ;

	//Set Channel value. (13-62 for Japan, 7~69 for Brazil)
	myfe->nim_param.tuner_param.CH = ch ;

	//Set reception layer here.
	myfe->layer_param.RECEP_LAYER = recep_layer ;

	//Set mode and guard value for all mods and all guards
	ERR_MSG = MB86A2x_Set_ModeGuard_all( myfe ) ;

	//Set timing recovery to default value
	ERR_MSG = MB86A2x_Set_STR_OFS( &(myfe->nim_param), &(myfe->pll_param), &(myfe->recov_param) ) ;

	//Write channel setting parameters.
	ERR_MSG = MB86A2x_Set_Channel( myfe ) ;

	//Write the data to reset monitor timers.
	ERR_MSG = MB86A2x_Reset_Monitor_Timer( myfe ) ;

	return ERR_MSG ;
}


MB86A2x_ERR_MSG Reception_before_Channel_Search_Fixed_Timer_Periods_2( 
				MB86A2x_PARAM *myfe, MB86A2x_LOCKNG_INFO *locking_info, MB86A2x_SIG_QUAL_INFO *sign_qual_info,
				MB86A2x_TMCC_INFO *tmcc_info, MB86A2x_CH_SCH_INFO *ch_sch_info/*, MB86A2x_RECEP_LAYER recep_layer*/ )
{

	MB86A2x_ERR_MSG       ERR_MSG ;

	UINT8 ch_number ;		//Channel number of detected channel(0,1,2, ... ,N-1)
							//N:The number of detected channel 

	UINT32 VBER_T_A, VBER_T_B, VBER_T_C ;	/* Timer periods for pre-Viterbi */
	UINT32 SBER_T_A, SBER_T_B, SBER_T_C ;	/* Timer periods for post-Viterbi */
	UINT32 PBER_T_A, PBER_T_B, PBER_T_C ;	/* Timer periods for post-RS */
	UINT32 MER_T ;							/* Timer period for MER */
	UINT32 CN_T ;							/* Timer period for C/N */

	//Enter timer period for pre-Viterbi BER timer, post-Viterbi BER timer timer, post-RS BER timer, MER timer, C/N timer.
	VBER_T_A = 200 ;
	VBER_T_B = 250 ;
	VBER_T_C = 250 ;
	SBER_T_A = 250 ;
	SBER_T_B = 250 ;
	SBER_T_C = 250 ;
	PBER_T_A = 250 ;
	PBER_T_B = 250 ;
	PBER_T_C = 250 ;
	MER_T = 50 ;
	CN_T = 100 ;

	//Set the channel number to store channel information to 69.
	ch_number = 69 ;	//This value should be 69 in spite of reception channel.

	ERR_MSG = MB86A2x_Monitor1( *myfe, locking_info ) ;

	//Read register values to check the mode, GI.
	ERR_MSG = MB86A2x_Get_Mode_Guard( &(myfe->nim_param), tmcc_info ) ;
	#ifdef FUJITSU_MSG_PRINT
	FUJITSU_printf_Mode_Guard( tmcc_info ) ;
	#endif

	if( locking_info->STATE>=7 ) {
		MB86A2x_Monitor2( *myfe, sign_qual_info ) ;
	}
	#ifdef FUJITSU_MSG_PRINT
	FUJITSU_printf_CN( sign_qual_info ) ;
	#endif

	if( locking_info->STATE>=8 ) {	//Check whether the state value has reached to 8 or not.
		//Read register values to check the TMCC information.
		ERR_MSG = MB86A2x_Get_TMCC_Info( &(myfe->nim_param), tmcc_info ) ;
		#ifdef FUJITSU_MSG_PRINT
		FUJITSU_printf_TMCC( tmcc_info ) ;
		#endif
	}
	if( locking_info->S8_REACH==1 ) {
		//Store TMCC information (channel information) to pointer.
		ERR_MSG = MB86A2x_Store_Channel_Info( myfe->nim_param.tuner_param.CH, tmcc_info, ch_sch_info ) ;

		//Define reception layer
		/*
		if(recep_layer==MB86A2x_RECEP_HIGHEST ) {
			ERR_MSG = MB86A2x_Init_RECEP_SEG_Highest( ch_sch_info, &(myfe->seg_param), ch_number ) ;
		}
		else if( recep_layer==MB86A2x_RECEP_LAYER_A ) {
			ERR_MSG = MB86A2x_Init_RECEP_SEG_Lay_A( ch_sch_info, &(myfe->seg_param), ch_number ) ;
		}
		else if( recep_layer==MB86A2x_RECEP_LAYER_B ) {
			ERR_MSG = MB86A2x_Init_RECEP_SEG_Lay_B( ch_sch_info, &(myfe->seg_param), ch_number ) ;
		}
		else if( recep_layer==MB86A2x_RECEP_LAYER_C ) {
			ERR_MSG = MB86A2x_Init_RECEP_SEG_Lay_C( ch_sch_info, &(myfe->seg_param), ch_number ) ;
		}

		//Write reception segment parameters.
		ERR_MSG = MB86A2x_Set_RECEP_SEG( &(myfe->nim_param), &(myfe->seg_param) ) ;
		*/

		//Calculate registers from channel information and pre-Viterbi BER timer period.
		ERR_MSG = MB86A2x_Calc_VBER_NUM( myfe->pll_param.BANDSEL, ch_sch_info, VBER_T_A, VBER_T_B, VBER_T_C, ch_number, &(myfe->mon_tim_param) ) ;
		//Calculate registers from channel information and post-Viterbi BER timer period.
		ERR_MSG = MB86A2x_Calc_SBER_NUM( myfe->pll_param.BANDSEL, ch_sch_info, SBER_T_A, SBER_T_B, SBER_T_C, ch_number, &(myfe->mon_tim_param) ) ;
		//Calculate registers from channel information and post-RS BER timer period.
		ERR_MSG = MB86A2x_Calc_PBER_NUM( myfe->pll_param.BANDSEL, ch_sch_info, PBER_T_A, PBER_T_B, PBER_T_C, ch_number, &(myfe->mon_tim_param) ) ;
		//Calculate registers from channel information and MER timer period.
		ERR_MSG = MB86A2x_Calc_MER_SYMBOL( myfe->pll_param.BANDSEL, ch_sch_info, MER_T, ch_number, &(myfe->mon_tim_param) ) ;
		//Calculate registers from channel information and C/N timer period.
		ERR_MSG = MB86A2x_Calc_CN_SYMBOL( myfe->pll_param.BANDSEL, ch_sch_info, CN_T, ch_number, &(myfe->mon_tim_param) ) ;
		//Write the timer parameters.
		ERR_MSG = MB86A2x_Set_Monitor_Timer( myfe ) ;
		//Write the data to reset monitor timers.
		ERR_MSG = MB86A2x_Reset_Monitor_Timer( myfe ) ;
	}
	if( locking_info->STATE>=9 ) {	//Check whether the state value has reached to 9 or not.
		ERR_MSG = MB86A2x_Monitor3( *myfe, tmcc_info, sign_qual_info ) ;
	}

	return ERR_MSG ;
}


MB86A2x_ERR_MSG Reception_before_Channel_Search_Fixed_Timer_Register_Values_1( 
				MB86A2x_PARAM *myfe, MB86A2x_LOCKNG_INFO *locking_info,
				MB86A2x_SIG_QUAL_INFO *sign_qual_info,
				UINT32 ch, MB86A2x_RECEP_LAYER recep_layer )
{

	MB86A2x_ERR_MSG       ERR_MSG ;

	//Set BER monitor timer parameters and MER monitor parameter.
	myfe->mon_tim_param.VBER_A_NUM = 0x0000ff ;
	myfe->mon_tim_param.VBER_B_NUM = 0x00ffff ;
	myfe->mon_tim_param.VBER_C_NUM = 0x00ffff ;
	myfe->mon_tim_param.SBER_A_NUM = 0x0000ff ;
	myfe->mon_tim_param.SBER_B_NUM = 0x003fff ;
	myfe->mon_tim_param.SBER_C_NUM = 0x003fff ;
	myfe->mon_tim_param.PBER_A_NUM = 0x0000ff ;
	myfe->mon_tim_param.PBER_B_NUM = 0x003fff ;
	myfe->mon_tim_param.PBER_C_NUM = 0x003fff ;
	myfe->mon_tim_param.CN_SYMBOL  = 0x04 ;
	myfe->mon_tim_param.MER_SYMBOL = 0x04 ;
	//You can also set these values by chaning the values in "MB86A21_***_default_setting.h" instead of above codes.

	//Write the timer parameters.
	ERR_MSG = MB86A2x_Set_Monitor_Timer( myfe ) ;

	//Initialize the parameters of LOCKING_INFO.
	ERR_MSG = MB86A2x_Init_LOCKNG_INFO( locking_info ) ;

	//Initialize the parameters of SIG_QUAL_INFO.
	ERR_MSG = MB86A2x_Init_SIG_QUAL_INFO( sign_qual_info ) ;

	//Set Channel value. (13-62 for Japan, 7~69 for Brazil)
	myfe->nim_param.tuner_param.CH = ch ;

	//Set reception layer here.
	myfe->layer_param.RECEP_LAYER = recep_layer ;

	//Set mode and guard value for all mods and all guards
	ERR_MSG = MB86A2x_Set_ModeGuard_all( myfe ) ;

	//Set timing recovery to default value
	ERR_MSG = MB86A2x_Set_STR_OFS( &(myfe->nim_param), &(myfe->pll_param), &(myfe->recov_param) ) ;

	//Write channel setting parameters.
	ERR_MSG = MB86A2x_Set_Channel( myfe ) ;

	//Write the data to reset monitor timers.
	ERR_MSG = MB86A2x_Reset_Monitor_Timer( myfe ) ;

	return ERR_MSG ;
}


MB86A2x_ERR_MSG Reception_before_Channel_Search_Fixed_Timer_Register_Values_2( 
				MB86A2x_PARAM *myfe, MB86A2x_LOCKNG_INFO *locking_info, MB86A2x_SIG_QUAL_INFO *sign_qual_info,
				MB86A2x_TMCC_INFO *tmcc_info, MB86A2x_CH_SCH_INFO *ch_sch_info/*, MB86A2x_RECEP_LAYER recep_layer*/ )
{

	MB86A2x_ERR_MSG       ERR_MSG ;

	// UINT8 ch_number ;		//Channel number of detected channel(0,1,2, ... ,N-1)
							//N:The number of detected channel 

	//Set the channel number to store channel information to 69.
	// ch_number = 69 ;	//This value should be 69 in spite of reception channel.

	ERR_MSG = MB86A2x_Monitor1( *myfe, locking_info ) ;

	//Read register values to check the mode, GI.
	ERR_MSG = MB86A2x_Get_Mode_Guard( &(myfe->nim_param), tmcc_info ) ;
	#ifdef FUJITSU_MSG_PRINT
	FUJITSU_printf_Mode_Guard( tmcc_info ) ;
	#endif

	if( locking_info->STATE>=7 ) {
		MB86A2x_Monitor2( *myfe, sign_qual_info ) ;
	}
	#ifdef FUJITSU_MSG_PRINT
	FUJITSU_printf_CN( sign_qual_info ) ;
	#endif

	if( locking_info->STATE>=8 ) {	//Check whether the state value has reached to 8 or not.
		//Read register values to check the TMCC information.
		ERR_MSG = MB86A2x_Get_TMCC_Info( &(myfe->nim_param), tmcc_info ) ;
		#ifdef FUJITSU_MSG_PRINT
		FUJITSU_printf_TMCC( tmcc_info ) ;
		#endif
	}
	if( locking_info->S8_REACH==1 ) {
		//Store TMCC information (channel information) to pointer.
		ERR_MSG = MB86A2x_Store_Channel_Info( myfe->nim_param.tuner_param.CH, tmcc_info, ch_sch_info ) ;

		//Define reception layer
		/*
		if(recep_layer==MB86A2x_RECEP_HIGHEST ) {
			ERR_MSG = MB86A2x_Init_RECEP_SEG_Highest( ch_sch_info, &(myfe->seg_param), ch_number ) ;
		}
		else if( recep_layer==MB86A2x_RECEP_LAYER_A ) {
			ERR_MSG = MB86A2x_Init_RECEP_SEG_Lay_A( ch_sch_info, &(myfe->seg_param), ch_number ) ;
		}
		else if( recep_layer==MB86A2x_RECEP_LAYER_B ) {
			ERR_MSG = MB86A2x_Init_RECEP_SEG_Lay_B( ch_sch_info, &(myfe->seg_param), ch_number ) ;
		}
		else if( recep_layer==MB86A2x_RECEP_LAYER_C ) {
			ERR_MSG = MB86A2x_Init_RECEP_SEG_Lay_C( ch_sch_info, &(myfe->seg_param), ch_number ) ;
		}
		*/
		//Write reception segment parameters.
		//ERR_MSG = MB86A2x_Set_RECEP_SEG( &(myfe->nim_param), &(myfe->seg_param) ) ;
	}
    //#endif
	if( locking_info->STATE>=9 ) {	//Check whether the state value has reached to 9 or not.
		ERR_MSG = MB86A2x_Monitor3( *myfe, tmcc_info, sign_qual_info ) ;
	}

	return ERR_MSG ;
}


MB86A2x_ERR_MSG Reception_after_Channel_Search_Fixed_Timer_Periods_1( 
				MB86A2x_PARAM *myfe, MB86A2x_LOCKNG_INFO *locking_info,
				MB86A2x_SIG_QUAL_INFO *sign_qual_info, MB86A2x_CH_SCH_INFO *ch_sch_info, 
				UINT8 ch_number, MB86A2x_RECEP_LAYER recep_layer )
{

	MB86A2x_ERR_MSG       ERR_MSG ;

	UINT32 VBER_T_A, VBER_T_B, VBER_T_C ;	/* Timer periods for pre-Viterbi */
	UINT32 SBER_T_A, SBER_T_B, SBER_T_C ;	/* Timer periods for post-Viterbi */
	UINT32 PBER_T_A, PBER_T_B, PBER_T_C ;	/* Timer periods for post-RS */
	UINT32 MER_T ;							/* Timer period for MER */
	UINT32 CN_T ;							/* Timer period for C/N */

	//Enter timer period for pre-Viterbi BER timer, post-Viterbi BER timer timer, post-RS BER timer, MER timer, C/N timer.
	VBER_T_A = 200 ;
	VBER_T_B = 250 ;
	VBER_T_C = 250 ;
	SBER_T_A = 250 ;
	SBER_T_B = 250 ;
	SBER_T_C = 250 ;
	PBER_T_A = 250 ;
	PBER_T_B = 250 ;
	PBER_T_C = 250 ;
	MER_T = 50 ;
	CN_T = 100 ;

	//Initialize the parameters of LOCKING_INFO.
	ERR_MSG = MB86A2x_Init_LOCKNG_INFO( locking_info ) ;

	//Initialize the parameters of SIG_QUAL_INFO.
	ERR_MSG = MB86A2x_Init_SIG_QUAL_INFO( sign_qual_info ) ;

	//Lcok to the channel that was detected by channel search at first
	myfe->nim_param.tuner_param.CH = ch_sch_info->CH_num[ch_number] ;

	//Set reception layer here.
	myfe->layer_param.RECEP_LAYER = recep_layer ;

	//Set mode and guard value to reception channel
	ERR_MSG = MB86A2x_Set_ModeGuard_fixed( myfe, ch_sch_info, ch_number ) ;

	//Set timing recovery to accurate value
	ERR_MSG = MB86A2x_Set_STR_OFS2( &(myfe->nim_param), ch_sch_info, ch_number, &(myfe->recov_param) ) ;

	//Define reception layer
	/*
	if(recep_layer==MB86A2x_RECEP_HIGHEST ) {
		ERR_MSG = MB86A2x_Init_RECEP_SEG_Highest( ch_sch_info, &(myfe->seg_param), ch_number ) ;
	}
	else if( recep_layer==MB86A2x_RECEP_LAYER_A ) {
		ERR_MSG = MB86A2x_Init_RECEP_SEG_Lay_A( ch_sch_info, &(myfe->seg_param), ch_number ) ;
	}
	else if( recep_layer==MB86A2x_RECEP_LAYER_B ) {
		ERR_MSG = MB86A2x_Init_RECEP_SEG_Lay_B( ch_sch_info, &(myfe->seg_param), ch_number ) ;
	}
	else if( recep_layer==MB86A2x_RECEP_LAYER_C ) {
		ERR_MSG = MB86A2x_Init_RECEP_SEG_Lay_C( ch_sch_info, &(myfe->seg_param), ch_number ) ;
	}
	*/

	//Set RECEP_LAYER value to MB86A2x_RECEP_LAYER_A.
	myfe->layer_param.RECEP_LAYER = MB86A2x_RECEP_LAYER_A ;

	//Write channel setting parameters.
	ERR_MSG = MB86A2x_Set_Channel( myfe ) ;

	//Calculate registers from channel information and pre-Viterbi BER timer period.
	ERR_MSG = MB86A2x_Calc_VBER_NUM( myfe->pll_param.BANDSEL, ch_sch_info, VBER_T_A, VBER_T_B, VBER_T_C, ch_number, &(myfe->mon_tim_param) ) ;
	//Calculate registers from channel information and post-Viterbi BER timer period.
	ERR_MSG = MB86A2x_Calc_SBER_NUM( myfe->pll_param.BANDSEL, ch_sch_info, SBER_T_A, SBER_T_B, SBER_T_C, ch_number, &(myfe->mon_tim_param) ) ;
	//Calculate registers from channel information and post-RS BER timer period.
	ERR_MSG = MB86A2x_Calc_PBER_NUM( myfe->pll_param.BANDSEL, ch_sch_info, PBER_T_A, PBER_T_B, PBER_T_C, ch_number, &(myfe->mon_tim_param) ) ;
	//Calculate registers from channel information and MER timer period.
	ERR_MSG = MB86A2x_Calc_MER_SYMBOL( myfe->pll_param.BANDSEL, ch_sch_info, MER_T, ch_number, &(myfe->mon_tim_param) ) ;
	//Calculate registers from channel information and C/N timer period.
	ERR_MSG = MB86A2x_Calc_CN_SYMBOL( myfe->pll_param.BANDSEL, ch_sch_info, CN_T, ch_number, &(myfe->mon_tim_param) ) ;
	//Write the timer parameters.
	ERR_MSG = MB86A2x_Set_Monitor_Timer( myfe ) ;

	//Write the data to reset monitor timers.
	ERR_MSG = MB86A2x_Reset_Monitor_Timer( myfe ) ;

	return ERR_MSG ;
}


MB86A2x_ERR_MSG Reception_after_Channel_Search_Fixed_Timer_Periods_2( 
				MB86A2x_PARAM *myfe, MB86A2x_LOCKNG_INFO *locking_info, MB86A2x_SIG_QUAL_INFO *sign_qual_info,
				MB86A2x_TMCC_INFO *tmcc_info )
{

	MB86A2x_ERR_MSG       ERR_MSG ;

	ERR_MSG = MB86A2x_Monitor1( *myfe, locking_info ) ;

	//Read register values to check the mode, GI.
	ERR_MSG = MB86A2x_Get_Mode_Guard( &(myfe->nim_param), tmcc_info ) ;
	#ifdef FUJITSU_MSG_PRINT
	FUJITSU_printf_Mode_Guard( tmcc_info ) ;
	#endif

	if( locking_info->STATE>=7 ) {
		MB86A2x_Monitor2( *myfe, sign_qual_info ) ;
	}
	#ifdef FUJITSU_MSG_PRINT
	FUJITSU_printf_CN( sign_qual_info ) ;
	#endif

	if( locking_info->STATE>=8 ) {	//Check whether the state value has reached to 8 or not.
		//Read register values to check the TMCC information.
		ERR_MSG = MB86A2x_Get_TMCC_Info( &(myfe->nim_param), tmcc_info ) ;
		#ifdef FUJITSU_MSG_PRINT
		FUJITSU_printf_TMCC( tmcc_info ) ;
		#endif
	}
	if( locking_info->STATE>=9 ) {	//Check whether the state value has reached to 9 or not.
		ERR_MSG = MB86A2x_Monitor3( *myfe, tmcc_info, sign_qual_info ) ;
	}

	return ERR_MSG ;
}


MB86A2x_ERR_MSG Reception_after_Channel_Search_Fixed_Timer_Register_Values_1( 
				MB86A2x_PARAM *myfe, MB86A2x_LOCKNG_INFO *locking_info,
				MB86A2x_SIG_QUAL_INFO *sign_qual_info, MB86A2x_CH_SCH_INFO *ch_sch_info, 
				UINT8 ch_number, MB86A2x_RECEP_LAYER recep_layer )
{

	MB86A2x_ERR_MSG       ERR_MSG ;

	//Set BER monitor timer parameters and MER monitor parameter.
	myfe->mon_tim_param.VBER_A_NUM = 0x0000ff ;
	myfe->mon_tim_param.VBER_B_NUM = 0x00ffff ;
	myfe->mon_tim_param.VBER_C_NUM = 0x00ffff ;
	myfe->mon_tim_param.SBER_A_NUM = 0x0000ff ;
	myfe->mon_tim_param.SBER_B_NUM = 0x003fff ;
	myfe->mon_tim_param.SBER_C_NUM = 0x003fff ;
	myfe->mon_tim_param.PBER_A_NUM = 0x0000ff ;
	myfe->mon_tim_param.PBER_B_NUM = 0x003fff ;
	myfe->mon_tim_param.PBER_C_NUM = 0x003fff ;
	myfe->mon_tim_param.CN_SYMBOL  = 0x04 ;
	myfe->mon_tim_param.MER_SYMBOL = 0x04 ;
	//You can also set these values by chaning the values in "MB86A21_***_default_setting.h" instead of above codes.

	//Initialize the parameters of LOCKING_INFO.
	ERR_MSG = MB86A2x_Init_LOCKNG_INFO( locking_info ) ;

	//Initialize the parameters of SIG_QUAL_INFO.
	ERR_MSG = MB86A2x_Init_SIG_QUAL_INFO( sign_qual_info ) ;

	//Lcok to the channel that was detected by channel search at first
	myfe->nim_param.tuner_param.CH = ch_sch_info->CH_num[ch_number] ;

	//Set reception layer here.
	myfe->layer_param.RECEP_LAYER = recep_layer ;

	//Set mode and guard value to reception channel
	ERR_MSG = MB86A2x_Set_ModeGuard_fixed( myfe, ch_sch_info, ch_number ) ;

	//Set timing recovery to accurate value
	ERR_MSG = MB86A2x_Set_STR_OFS2( &(myfe->nim_param), ch_sch_info, ch_number, &(myfe->recov_param) ) ;

	//Write the timer parameters.
	ERR_MSG = MB86A2x_Set_Monitor_Timer( myfe ) ;

	//Write channel setting parameters.
	ERR_MSG = MB86A2x_Set_Channel( myfe ) ;

	//Write the data to reset monitor timers.
	ERR_MSG = MB86A2x_Reset_Monitor_Timer( myfe ) ;

	return ERR_MSG ;
}


MB86A2x_ERR_MSG Reception_after_Channel_Search_Fixed_Timer_Register_Values_2( 
				MB86A2x_PARAM *myfe, MB86A2x_LOCKNG_INFO *locking_info, MB86A2x_SIG_QUAL_INFO *sign_qual_info,
				MB86A2x_TMCC_INFO *tmcc_info )
{

	MB86A2x_ERR_MSG       ERR_MSG ;

	ERR_MSG = MB86A2x_Monitor1( *myfe, locking_info ) ;

	//Read register values to check the mode, GI.
	ERR_MSG = MB86A2x_Get_Mode_Guard( &(myfe->nim_param), tmcc_info ) ;
	#ifdef FUJITSU_MSG_PRINT
	FUJITSU_printf_Mode_Guard( tmcc_info ) ;
	#endif

	if( locking_info->STATE>=7 ) {
		MB86A2x_Monitor2( *myfe, sign_qual_info ) ;
	}
	#ifdef FUJITSU_MSG_PRINT
	FUJITSU_printf_CN( sign_qual_info ) ;
	#endif

	if( locking_info->STATE>=8 ) {	//Check whether the state value has reached to 8 or not.
		//Read register values to check the TMCC information.
		ERR_MSG = MB86A2x_Get_TMCC_Info( &(myfe->nim_param), tmcc_info ) ;
		#ifdef FUJITSU_MSG_PRINT
		FUJITSU_printf_TMCC( tmcc_info ) ;
		#endif
	}
	if( locking_info->STATE>=9 ) {	//Check whether the state value has reached to 9 or not.
		ERR_MSG = MB86A2x_Monitor3( *myfe, tmcc_info, sign_qual_info ) ;
	}

	return ERR_MSG ;
}

