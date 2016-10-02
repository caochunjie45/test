#ifndef _STANDBY_SD_H_
#define _STANDBY_SD_H_

#include "ePDK.h"
#include "standby_cfg.h"
#include "standby_reg.h"

typedef struct STANDBY_SDMMC_CTRL
{
    __hdle hPin;
    __s32  bConnect;
    __s32  nDebounce;
    __s32  nState;
}standby_sdmmc_ctrl_t;


extern __s32  standby_sd_init(void);
extern __s32  standby_sd_exit(void);
extern __s32  standby_query_sd_event(void);

#endif
