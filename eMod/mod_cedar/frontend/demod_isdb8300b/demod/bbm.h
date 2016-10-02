/*****************************************************************************
	Copyright(c) 2013 FCI Inc. All Rights Reserved

	File name : bbm.h

	Description : API header of ISDB-T baseband module

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA

	History :
	----------------------------------------------------------------------
*******************************************************************************/
#ifndef __BBM_H__
#define __BBM_H__

#include "fci_types.h"

#define DRIVER_VER "S1.21.0"

#ifdef __cplusplus
extern "C" {
#endif

extern __s32 bbm_com_reset(HANDLE handle, DEVICEID devid);
extern __s32 bbm_com_probe(HANDLE handle, DEVICEID devid);
extern __s32 bbm_com_init(HANDLE handle, DEVICEID devid);
extern __s32 bbm_com_deinit(HANDLE handle, DEVICEID devid);
extern __s32 bbm_com_read(HANDLE handle, DEVICEID devid, __u16 addr, __u8 *data);
extern __s32 bbm_com_byte_read(HANDLE handle, DEVICEID devid, __u16 addr, __u8 *data);
extern __s32 bbm_com_word_read(HANDLE handle
	, DEVICEID devid, __u16 addr, __u16 *data);
extern __s32 bbm_com_long_read(HANDLE handle
	, DEVICEID devid, __u16 addr, __u32 *data);
extern __s32 bbm_com_bulk_read(HANDLE handle, DEVICEID devid, __u16 addr, __u8 *data,
							__u16 size);
extern __s32 bbm_com_data(HANDLE handle
	, DEVICEID devid, __u16 addr, __u8 *data, __u32 size);
extern __s32 bbm_com_write(HANDLE handle, DEVICEID devid, __u16 addr, __u8 data);
extern __s32 bbm_com_byte_write(HANDLE handle, DEVICEID devid, __u16 addr, __u8 data);
extern __s32 bbm_com_word_write(HANDLE handle
	, DEVICEID devid, __u16 addr, __u16 data);
extern __s32 bbm_com_long_write(HANDLE handle
	, DEVICEID devid, __u16 addr, __u32 data);
extern __s32 bbm_com_bulk_write(HANDLE handle, DEVICEID devid,
		__u16 addr, __u8 *data, __u16 size);
extern __s32 bbm_com_i2c_init(HANDLE handle, __u32 type);
extern __s32 bbm_com_i2c_deinit(HANDLE handle);
extern __s32 bbm_com_tuner_select(HANDLE handle, DEVICEID devid,
		__u32 product, __u32 brodcast);
extern __s32 bbm_com_tuner_deselect(HANDLE handle, DEVICEID devid);
extern __s32 bbm_com_tuner_read(HANDLE handle, DEVICEID devid,
		__u8 addr, __u8 alen, __u8 *buffer, __u8 len);
extern __s32 bbm_com_tuner_write(HANDLE handle, DEVICEID devid,
		__u8 addr, __u8 alen, __u8 *buffer, __u8 len);
extern __s32 bbm_com_tuner_set_freq(HANDLE handle
	, DEVICEID devid, __u32 freq, __u8 subch);
extern __s32 bbm_com_tuner_get_rssi(HANDLE handle, DEVICEID devid, __s32 *rssi);
extern __s32 bbm_com_scan_status(HANDLE handle, DEVICEID devid);
extern __s32 bbm_com_hostif_select(HANDLE handle, __u8 hostif);
extern __s32 bbm_com_hostif_deselect(HANDLE handle);
extern __s32 bbm_com_ts_callback_register(unsigned long userdata,
	__s32 (*callback)(unsigned long userdata, __u8 bufid, __u8 *data, __s32 length));
extern __s32 bbm_com_ts_callback_deregister(void);
extern __s32 bbm_com_ac_callback_register(unsigned long userdata,
	__s32 (*callback)(unsigned long userdata, __u8 bufid, __u8 *data, __s32 length));
extern __s32 bbm_com_ac_callback_deregister(void);
extern void bbm_com_isr(HANDLE handle);

#ifdef __cplusplus
}
#endif

#endif /* __BBM_H__ */
