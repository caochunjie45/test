/*
***************************************************************************
*
*  Functions to calculate Tuner Parameter
*
*  Copyright (c) 2009 Fujitsu Semiconductor 
*  All rights reserved. 
*
*  Created 03/24/2009
*
***************************************************************************
*/
#ifndef _TUNER_H_
#define _TUNER_H_

#include "../demod/MB86A22_API_defs.h"
#include "Rdatv_bb_defs.h"
#include "../DataType.h"
/*
***************************************************************************
*	Tuner Configuration
***************************************************************************
*/

typedef struct 
{
	UINT32           Xtal_freq_kHz ;
	UINT32           IF_freq_kHz ;
	MB86A2x_CH_UNIT  CH_UNIT ;
	UINT32           CH ;
	UINT32           freq_kHz ;
	RDA5880_PARAM    rf_param ;
} RDA5880_TunerConfigS;


/*
***************************************************************************
*	Tuner Error Message
***************************************************************************
*/

typedef enum {
	TUNER_NG       = 0,
	TUNER_OK       = 1,
} TUNER_MSG ;


typedef enum {
	Tuner_Reset_NOTDONE       = 0,
	Tuner_Reset_DONE       = 1,
} TUNER_Reset ;

/*
***************************************************************************
*  Functions to calculate Tuner Parameter
***************************************************************************
*/

TUNER_MSG RDA_TUNER_INIT(RDA5880_TunerConfigS *mytuner) ;
TUNER_MSG RDA_TUNER_SET_FREQ( RDA5880_TunerConfigS *mytuner, MB86A2x_BANDSEL BANDSEL ) ;
#ifndef PARAMETER_USE_FREQ
TUNER_MSG CH_to_freq( RDA5880_TunerConfigS *mytuner ) ;
TUNER_MSG freq_to_CH( RDA5880_TunerConfigS *mytuner );
#endif

#endif
