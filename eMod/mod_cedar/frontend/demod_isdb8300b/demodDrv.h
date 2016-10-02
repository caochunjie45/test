
#ifndef DEMOD_DRV_H
#define DEMOD_DRV_H

#include "mod_cedar_i.h"

#include "demod_interface.h"

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


}demod_dev_t;

#endif
