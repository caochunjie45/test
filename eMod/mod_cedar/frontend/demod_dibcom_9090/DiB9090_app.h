#ifndef DIB7000_XXXXXXXX
#define DIB7000_XXXXXXXX

#include <adapter/tuner.h>
#include <adapter/demod.h>

#include "typedef.h"

extern struct dibI2CAdapter * open_dibcom_i2c();

char dib_init();
unsigned char dib_tune(unsigned int frequency,char bandwidth);
char dib_lockstatus();
void dib_signal_strenth_quality(unsigned char *signal_quality, unsigned char *signal_strength);

#endif

