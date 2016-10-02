
#ifndef DEMOD_DRV_H
#define DEMOD_DRV_H

#include "mod_cedar_i.h"

#include "demod_interface.h"
#include "os_adapter.h"
#include "demod/Atbm781x.h"

typedef struct DEMOD_DEV
{
    __u32               mid;
    demod_status_t      status;
    __u8                lockTaskPrio;
    __u32               frequency;
    __u32               bandwidth;
    
    __pCBK_t            lockCb;
    lock_cb_param_t*    lockCbParam;

	__u8				plpindex;
	__u8				plppid;
	ATBM781x_DVBT2_PARAMS pDvbT2Params;
	__s32		t_or_t2;  //2  dvb-t  3   dvb-t2
	ATBM781x_DVBT_PARAMS	pDvbTParams;
}demod_dev_t;

#endif
