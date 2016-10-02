/*
***************************************************************************
*
*  Functions to calculate Tuner Parameter
*
*  Copyright (c) 2009 Fujitsu Semiconductor 
*  All rights reserved. 
*
*  Created 02/02/2009
*
***************************************************************************
*/
#include "Tuner.h"
#include "rdatv_bb.h"

TUNER_MSG RDA_TUNER_INIT(RDA5880_TunerConfigS *mytuner)
{
	RDA5880_ERR_MSG msg ;

	msg = devTunerInit(&mytuner->rf_param, mytuner->Xtal_freq_kHz) ;
	if( msg==RDA5880_ERR ) return TUNER_NG ;

	return TUNER_OK ;
}


TUNER_MSG RDA_TUNER_SET_FREQ( RDA5880_TunerConfigS *mytuner, MB86A2x_BANDSEL BANDSEL )
{
	UINT8 bw ;

	RDA5880_ERR_MSG msg ;

	if( BANDSEL==MB86A2x_BANDSEL_ISDBT_6MHz ) {
		bw = BW_MODE_6MHZ ;
	}
	else if( BANDSEL==MB86A2x_BANDSEL_ISDBT_7MHz ) {
		bw = BW_MODE_7MHZ ;
	}
	else if( BANDSEL==MB86A2x_BANDSEL_ISDBT_8MHz ) {
		bw = BW_MODE_8MHZ ;
	}
	else if( BANDSEL==MB86A2x_BANDSEL_DVBT_6MHz ) {
		bw = BW_MODE_6MHZ ;
	}
	else if( BANDSEL==MB86A2x_BANDSEL_DVBT_7MHz ) {
		bw = BW_MODE_7MHZ ;
	}
	else if( BANDSEL==MB86A2x_BANDSEL_DVBT_8MHz ) {
		bw = BW_MODE_8MHZ ;
	}
	#ifdef PARAMETER_USE_FREQ
	;
   	#else
	if( mytuner->CH_UNIT<2 ) CH_to_freq( mytuner ) ;
	
	#endif
	msg = devDigitalTuner_SetFreq( mytuner->freq_kHz, bw, &mytuner->rf_param ) ;
	if( msg==RDA5880_ERR ) return TUNER_NG ;

	return TUNER_OK ;
}

#ifndef PARAMETER_USE_FREQ
TUNER_MSG CH_to_freq( RDA5880_TunerConfigS *mytuner )
{

	if( mytuner->CH_UNIT==MB86A2x_CH_UNIT_JP ) {
		if(      mytuner->CH== 1 ) mytuner->freq_kHz = 93143 ;
		else if( mytuner->CH== 2 ) mytuner->freq_kHz = 99143 ;
		else if( mytuner->CH== 3 ) mytuner->freq_kHz = 105143 ;
		else if( mytuner->CH== 4 ) mytuner->freq_kHz = 173143 ;
		else if( mytuner->CH== 5 ) mytuner->freq_kHz = 179143 ;
		else if( mytuner->CH== 6 ) mytuner->freq_kHz = 185143 ;
		else if( mytuner->CH== 7 ) mytuner->freq_kHz = 191143 ;
		else if( mytuner->CH== 8 ) mytuner->freq_kHz = 195143 ;
		else if( mytuner->CH== 9 ) mytuner->freq_kHz = 201143 ;
		else if( mytuner->CH==10 ) mytuner->freq_kHz = 207143 ;
		else if( mytuner->CH==11 ) mytuner->freq_kHz = 213143 ;
		else if( mytuner->CH==12 ) mytuner->freq_kHz = 219143 ;
		else if( mytuner->CH==13 ) mytuner->freq_kHz = 473143 ;
		else                       mytuner->freq_kHz = 6000*((mytuner->CH)-13)+ 473143 ;
	}
	else {
		if( mytuner->CH<=13 ) mytuner->freq_kHz = 177143 + 6000*((mytuner->CH)-7) ;
		else                  mytuner->freq_kHz = 473143 + 6000*((mytuner->CH)-14) ;
	}

	return TUNER_OK ;
}

TUNER_MSG freq_to_CH( RDA5880_TunerConfigS *mytuner )
{

	if( mytuner->CH_UNIT==MB86A2x_CH_UNIT_JP ) {
		if(mytuner->freq_kHz == 93143)       mytuner->CH = 1;
		else if(mytuner->freq_kHz == 99143)  mytuner->CH = 2 ;
		else if(mytuner->freq_kHz == 105143) mytuner->CH = 3;
		else if(mytuner->freq_kHz == 173143) mytuner->CH = 4;
		else if(mytuner->freq_kHz == 179143) mytuner->CH = 5;
		else if(mytuner->freq_kHz == 185143) mytuner->CH = 6;
		else if(mytuner->freq_kHz == 191143) mytuner->CH = 7;
		else if(mytuner->freq_kHz == 195143) mytuner->CH = 8;
		else if(mytuner->freq_kHz == 201143) mytuner->CH = 9;
		else if(mytuner->freq_kHz == 207143) mytuner->CH = 10;
		else if(mytuner->freq_kHz == 213143) mytuner->CH = 11;
		else if(mytuner->freq_kHz == 219143) mytuner->CH = 12;
		else if(mytuner->freq_kHz == 473143) mytuner->CH = 13;
		else                                 mytuner->CH = (mytuner->freq_kHz -473143)/6000 + 13;
	}
	else {
        if(mytuner->freq_kHz <=213143) mytuner->CH = (mytuner->freq_kHz - 177143)/6000 + 7;
        else mytuner->CH = (mytuner->freq_kHz - 177143)/6000 + 14;
	}

	return TUNER_OK ;
}
#endif

