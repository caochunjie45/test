#ifndef _RS00_APP_H_
#define _RS00_APP_H_
#include "DataType.h"

char RS00_init(void);
void RS00_deinit(void);
unsigned char RS00_tune(unsigned int frequency_KHz, unsigned int bandwidth_KHz);
char RS00_lockstatus(void);
void RS00_signal_strength_quality(UINT8* signal_quality, UINT8* signal_strength);


#endif

