/*****************************************************************************
	Copyright(c) 2013 FCI Inc. All Rights Reserved

	File name : fc8300_ppi.h

	Description : header of EBI2/LCD interface

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
#ifndef __FC8300_PPI_H__
#define __FC8300_PPI_H__

#ifdef __cplusplus
extern "C" {
#endif

extern __s32 fc8300_ppi_init(HANDLE handle, __u16 param1, __u16 param2);
extern __s32 fc8300_ppi_byteread(HANDLE handle, DEVICEID devid,
		__u16 addr, __u8 *data);
extern __s32 fc8300_ppi_wordread(HANDLE handle, DEVICEID devid,
		__u16 addr, __u16 *data);
extern __s32 fc8300_ppi_longread(HANDLE handle, DEVICEID devid,
		__u16 addr, __u32 *data);
extern __s32 fc8300_ppi_bulkread(HANDLE handle, DEVICEID devid,
		__u16 addr, __u8 *data, __u16 length);
extern __s32 fc8300_ppi_bytewrite(HANDLE handle, DEVICEID devid,
		__u16 addr, __u8 data);
extern __s32 fc8300_ppi_wordwrite(HANDLE handle, DEVICEID devid,
		__u16 addr, __u16 data);
extern __s32 fc8300_ppi_longwrite(HANDLE handle, DEVICEID devid,
		__u16 addr, __u32 data);
extern __s32 fc8300_ppi_bulkwrite(HANDLE handle, DEVICEID devid,
		__u16 addr, __u8 *data, __u16 length);
extern __s32 fc8300_ppi_dataread(HANDLE handle, DEVICEID devid,
		__u16 addr, __u8 *data, __u32 length);
extern __s32 fc8300_ppi_deinit(HANDLE handle);

#ifdef __cplusplus
}
#endif

#endif /* __FC8300_PPI_H__ */
