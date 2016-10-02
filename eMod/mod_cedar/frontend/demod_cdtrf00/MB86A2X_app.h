#ifndef _MB86A2X_APP_H_
#define _MB86A2X_APP_H_

#include "tuner/rdatv_bb_defs.h"
#include "typedef.h"

char MB86A2x_init(void);
void MB86A2x_deinit(void);
unsigned char MB86A2x_tune(unsigned int frequency_KHz, unsigned int bandwidth_KHz);
char MB86A2x_lockstatus(void);
void MB86A2x_signal_strength_quality(UINT8* signal_quality, UINT8* signal_strength);


#endif
