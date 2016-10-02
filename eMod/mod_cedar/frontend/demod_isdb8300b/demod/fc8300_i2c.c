/*****************************************************************************
	Copyright(c) 2013 FCI Inc. All Rights Reserved

	File name : fc8300_i2c.c

	Description : source of I2C interface

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
#include "fci_types.h"
#include "fc8300_regs.h"
#include "fci_oal.h"
#include "fc8300_spi.h"
#include "mod_cedar_i.h"
#include "drv_twi.h"


static ES_FILE* ftwi = NULL;


static __s32 i2c_bulkread(HANDLE handle, __u8 chip, __u16 addr, __u8 *data, __u16 length)
{
	__twi_dev_para_ex_t    stwi;
	unsigned char iic_addr[2];
	int ret = 0;

	{
	    iic_addr[0] = addr & 0xff;
		iic_addr[1] = (addr >> 8) & 0xff;
	}
	   
	
	if(ftwi)
	{
		stwi.slave_addr      = chip;
		stwi.slave_addr_flag  =  0;
		stwi.byte_addr = iic_addr;
		stwi.byte_addr_width = 2;
		stwi.byte_count = length;
		stwi.data       = data;

		ret = eLIBs_fioctrl(ftwi, TWI_READ_SPEC_RS, 0, (void *)&stwi);

		if(0 == ret)
		{
			__msg("read IIC successful \n");
		}
		else
		{
			eDbug("read IIC failed \n");
		}
				
	}
	else
	{
		eDbug("i2c read failed ftwi is null\n");
	}

	return 0;
}

static __s32 i2c_bulkwrite(HANDLE handle, __u8 chip, __u16 addr, __u8 *data, __u16 length)
{
	__twi_dev_para_ex_t    stwi;
	unsigned char iic_addr[2];
	int ret = 0;
	
	{
	    iic_addr[0] = addr & 0xff;
		iic_addr[1] = (addr >> 8) & 0xff;
	}
	
	if(ftwi)
	{
		stwi.slave_addr      = chip;
		stwi.slave_addr_flag  =  0;
		stwi.byte_addr = iic_addr;
		stwi.byte_addr_width = 2;
		stwi.byte_count = length;
		stwi.data       = data;

		ret = eLIBs_fioctrl(ftwi, TWI_WRITE_SPEC_RS, 0, (void *)&stwi);

		if(0 == ret)
		{
			__msg("write IIC successful \n");
		}
		else
		{
			eDbug("write IIC failed \n");
		}		
	}
	else
	{
		eDbug("i2c write failed ftwi is null\n");
	}

	return 0;
}

__s32 fc8300_i2c_init(HANDLE handle, __u16 param1, __u16 param2)
{
	int ret;

	OAL_CREATE_SEMAPHORE();

#ifdef BBM_I2C_SPI
	fc8300_spi_init(handle, 0, 0);
#else
	/* ts_initialize(); */

	ftwi = eLIBs_fopen("b:\\BUS\\TWI2", "r");
	
    if(ftwi == NULL)
    {
	 	WARNING("open twi1 failed\n");
        return -1;
    }

	ret = eLIBs_fioctrl(ftwi, TWI_SET_SCL_CLOCK, 0, (void *)(100000));
    if(ret)
    {
		eDbug("set i2c clock[100K] failed\n");
    }
	eDbug("&&&&&&&&&&&&&&&&&&&&&&&&& TWI2 open sucess &&&&&&&&&&&&&&&&&\n");
	
	return 0;

#endif

	return BBM_OK;
}

__s32 fc8300_i2c_byteread(HANDLE handle, DEVICEID devid, __u16 addr, __u8 *data)
{
	__s32 res;

	OAL_OBTAIN_SEMAPHORE();
	res = i2c_bulkread(handle, (__u8) (devid >> 8) & 0xff, addr, data, 1);
	OAL_RELEASE_SEMAPHORE();

	return res;
}

__s32 fc8300_i2c_wordread(HANDLE handle, DEVICEID devid, __u16 addr, __u16 *data)
{
	__s32 res;

	OAL_OBTAIN_SEMAPHORE();
	res = i2c_bulkread(handle, (__u8) (devid >> 8) & 0xff,
			addr, (__u8 *) data, 2);
	OAL_RELEASE_SEMAPHORE();

	return res;
}

__s32 fc8300_i2c_longread(HANDLE handle, DEVICEID devid, __u16 addr, __u32 *data)
{
	__s32 res;

	OAL_OBTAIN_SEMAPHORE();
	res = i2c_bulkread(handle, (__u8) (devid >> 8) & 0xff,
			addr, (__u8 *) data, 4);
	OAL_RELEASE_SEMAPHORE();

	return res;
}

__s32 fc8300_i2c_bulkread(HANDLE handle, DEVICEID devid,
		__u16 addr, __u8 *data, __u16 length)
{
	__s32 res;

	OAL_OBTAIN_SEMAPHORE();
	res = i2c_bulkread(handle, (__u8) (devid >> 8) & 0xff,
			addr, data, length);
	OAL_RELEASE_SEMAPHORE();

	return res;
}

__s32 fc8300_i2c_bytewrite(HANDLE handle, DEVICEID devid, __u16 addr, __u8 data)
{
	__s32 res;

	OAL_OBTAIN_SEMAPHORE();
	res = i2c_bulkwrite(handle, (__u8) (devid >> 8) & 0xff,
			addr, (__u8 *)&data, 1);
	OAL_RELEASE_SEMAPHORE();

	return res;
}

__s32 fc8300_i2c_wordwrite(HANDLE handle, DEVICEID devid, __u16 addr, __u16 data)
{
	__s32 res;

	OAL_OBTAIN_SEMAPHORE();
	res = i2c_bulkwrite(handle, (__u8) (devid >> 8) & 0xff,
			addr, (__u8 *)&data, 2);
	OAL_RELEASE_SEMAPHORE();

	return res;
}

__s32 fc8300_i2c_longwrite(HANDLE handle, DEVICEID devid, __u16 addr, __u32 data)
{
	__s32 res;

	OAL_OBTAIN_SEMAPHORE();
	res = i2c_bulkwrite(handle, (__u8) (devid >> 8) & 0xff,
			addr, (__u8 *)&data, 4);
	OAL_RELEASE_SEMAPHORE();

	return res;
}

__s32 fc8300_i2c_bulkwrite(HANDLE handle, DEVICEID devid,
		__u16 addr, __u8 *data, __u16 length)
{
	__s32 res;

	OAL_OBTAIN_SEMAPHORE();
	res = i2c_bulkwrite(handle, (__u8) (devid >> 8) & 0xff,
			addr, data, length);
	OAL_RELEASE_SEMAPHORE();

	return res;
}

__s32 fc8300_i2c_dataread(HANDLE handle, DEVICEID devid,
		__u16 addr, __u8 *data, __u32 length)
{
	__s32 res;

#ifdef BBM_I2C_SPI
	res = fc8300_spi_dataread(handle, devid,
		addr, data, length);
#else
	OAL_OBTAIN_SEMAPHORE();
	res = i2c_bulkread(handle, (__u8) (devid >> 8) & 0xff,
			addr, data, length);
	OAL_RELEASE_SEMAPHORE();
#endif

	return res;
}

__s32 fc8300_i2c_deinit(HANDLE handle)
{
#ifdef BBM_I2C_SPI
	fc8300_spi_deinit(handle);
#else
	/* ts_receiver_disable(); */
	if(ftwi)
    {
        eLIBs_fclose(ftwi);
        ftwi = NULL;
    }
    
    return 0;

#endif

	OAL_DELETE_SEMAPHORE();

	return BBM_OK;
}

