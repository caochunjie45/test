/*****************************************************************************
 Copyright(c) 2012 FCI Inc. All Rights Reserved

 File name : fc8300_api.h

 Description :

*******************************************************************************/

#ifndef __FC8300_API_H__
#define __FC8300_API_H__


#include "typedef.h"
#include "mod_cedar_i.h"



#ifdef __cplusplus
extern "C" {
#endif

extern int mtv_init(int mode);
extern int mtv_deinit(void);
extern int mtv_set_channel(__s32 freq, __u8 subch_number);
extern int mtv_lock_check(void);
extern void mtv_get_ews_flag(__u8 *ews);
extern void mtv_signal_measure(__u8 *lock, double *CN
	, double *BER_A, double *PER_A, double *BER_B, double *PER_B
	, double *BER_C, double *PER_C, __s32 *i32RSSI);
extern void mtv_monitor_mfd(void);

#ifdef __cplusplus
}
#endif

#endif 		/* __FC8300_API_H__ */
