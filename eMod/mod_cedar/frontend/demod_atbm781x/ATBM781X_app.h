#ifndef _ATBM781X_APP_H_
#define _ATBM781X_APP_H_

#include "typedef.h"
#include "tuner/drv_mxl603_i.h"
#include "demod/drv_atbm_i.h"


__s32 ATBM781X_init(void);
void ATBM781X_deinit(void);
void ATBM781X_signal_strength_quality(UINT8* signal_quality, UINT8* signal_strength, __s32 t_t2_flag);
__s32 ATBM781X_tune(unsigned int frequency_KHz, unsigned int bandwidth_KHz);
__s32 ATBM781X_lockstatus(void);
__s32 ATBM781X_czp_lockstatus(__s32	t_t2_flag);



#endif
