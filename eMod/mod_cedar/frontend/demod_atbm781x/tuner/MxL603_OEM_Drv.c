/*****************************************************************************************
 *
 * FILE NAME          : MxL603_OEM_Drv.c
 * 
 * AUTHOR             : Mahendra Kondur
 *
 * DATE CREATED       : 12/23/2011  
 *
 * DESCRIPTION        : This file contains I2C driver and Sleep functins that 
 *                      OEM should implement for MxL603 APIs
 *                             
 *****************************************************************************************
 *                Copyright (c) 2011, MaxLinear, Inc.
 ****************************************************************************************/

#include "drv_mxl603_i.h"

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare603_OEM_WriteRegister
--| 
--| AUTHOR        : Brenndon Lee
--|
--| DATE CREATED  : 7/30/2009
--|
--| DESCRIPTION   : This function does I2C write operation.
--|
--| RETURN VALUE  : True or False
--|
--|-------------------------------------------------------------------------------------*/

MXL_STATUS MxLWare603_OEM_WriteRegister(UINT8 devId, UINT8 RegAddr, UINT8 RegData)
{
  // OEM should implement I2C write protocol that complies with MxL603 I2C
  // format.

  // 8 bit Register Write Protocol:
  // +------+-+-----+-+-+----------+-+----------+-+-+
  // |MASTER|S|SADDR|W| |RegAddr   | |RegData(L)| |P|
  // +------+-+-----+-+-+----------+-+----------+-+-+
  // |SLAVE |         |A|          |A|          |A| |
  // +------+---------+-+----------+-+----------+-+-+
  // Legends: SADDR (I2c slave address), S (Start condition), A (Ack), N(NACK), 
  // P(Stop condition)

  MXL_STATUS status = MXL_FALSE;
  

/* If OEM data is implemented, customer needs to use OEM data structure related operation 
   Following code should be used as a reference. 
   For more information refer to sections 2.5 & 2.6 of MxL603_mxLWare_API_UserGuide document.

  UINT8 i2cSlaveAddr;
  UINT8 i2c_bus;
  user_data_t * user_data = (user_data_t *) MxL603_OEM_DataPtr[devId];
 
  if (user_data)
  {
    i2cSlaveAddr = user_data->i2c_address;           // get device i2c address
    i2c_bus = user_data->i2c_bus;                   // get device i2c bus  
  
    sem_up(user_data->sem);                         // up semaphore if needed

    // I2C Write operation 
    status = USER_I2C_WRITE_FUNCTION(i2cSlaveAddr, i2c_bus, RegAddr, RegData);
    
    sem_down(user_data->sem);                       // down semaphore
    user_data->i2c_cnt++;                           // user statistics
  }

*/

  /* If OEM data is not required, customer should treat devId as I2C slave Address */

  //devId = devId;
  //RegAddr = RegAddr;
  //RegData = RegData;
#if 0
	__u8    	data;
	__u8    	byte_addr;
	__u16    	slave_addr;
	__s32	ret = -1;

	__twi_dev_para_ex_t  piic;  
	eLIBs_memset(&piic, 0, sizeof(__twi_dev_para_ex_t));

	slave_addr = 0x60;//devId;
	byte_addr = RegAddr;
	data = RegData;
	
	piic.slave_addr_flag = TWI_SLAVE_ADDRESS_07BIT;
	piic.slave_addr      = 0x60;//slave_addr;//ui8I2CSlaveAddr>>1;
	
	piic.byte_addr  = &byte_addr;
	piic.byte_addr_width = 1;
	
	piic.data       = &data;
	piic.byte_count = 1;


	ret = eLIBs_fioctrl(fiic, TWI_WRITE_SPEC_RS,0 , (void *)&piic);

	WARNING("------write ret= %d\n",ret);
	
	if(0 == ret)
		status  = MXL_TRUE;
	else
		status =  MXL_FALSE;
  
  return status;

  #else
	__s32	u8tmp,ret = -1;

	for(u8tmp = 0; u8tmp < 3; u8tmp++)
	{
		ret = iic_write(0x60,&RegAddr, 1, &RegData, 1);
		if(0 == ret)
		{
			status = MXL_TRUE;
			break;
		}
		else
		{
			status = MXL_FALSE;
			esKRNL_TimeDly(1);
		}
	}

	return status;
	
  #endif
}

/*------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare603_OEM_ReadRegister
--| 
--| AUTHOR        : Brenndon Lee
--|
--| DATE CREATED  : 7/30/2009
--|
--| DESCRIPTION   : This function does I2C read operation.
--|
--| RETURN VALUE  : True or False
--|
--|---------------------------------------------------------------------------*/

MXL_STATUS MxLWare603_OEM_ReadRegister(UINT8 devId, UINT8 RegAddr, UINT8 *DataPtr)
{
  // OEM should implement I2C read protocol that complies with MxL603 I2C
  // format.

  // 8 bit Register Read Protocol:
  // +------+-+-----+-+-+----+-+----------+-+-+
  // |MASTER|S|SADDR|W| |0xFB| |RegAddr   | |P|
  // +------+-+-----+-+-+----+-+----------+-+-+
  // |SLAVE |         |A|    |A|          |A| |
  // +------+-+-----+-+-+----+-+----------+-+-+
  // +------+-+-----+-+-+-----+--+-+
  // |MASTER|S|SADDR|R| |     |MN|P|
  // +------+-+-----+-+-+-----+--+-+
  // |SLAVE |         |A|Data |  | |
  // +------+---------+-+-----+--+-+
  // Legends: SADDR(I2c slave address), S(Start condition), MA(Master Ack), MN(Master NACK), 
  // P(Stop condition)

  MXL_STATUS status = MXL_FALSE;

/* If OEM data is implemented, customer needs to use OEM data structure related operation 
   Following code should be used as a reference. 
   For more information refer to sections 2.5 & 2.6 of MxL603_mxLWare_API_UserGuide document.

  UINT8 i2cSlaveAddr;
  UINT8 i2c_bus;
  user_data_t * user_data = (user_data_t *) MxL603_OEM_DataPtr[devId];
 
  if (user_data)
  {
    i2cSlaveAddr = user_data->i2c_address;           // get device i2c address
    i2c_bus = user_data->i2c_bus;                   // get device i2c bus  
  
    sem_up(user_data->sem);                         // up semaphore if needed

    // I2C Write operation 
    status = USER_I2C_READ_FUNCTION(i2cSlaveAddr, i2c_bus, RegAddr, DataPtr);
    
    sem_down(user_data->sem);                       // down semaphore
    user_data->i2c_cnt++;                           // user statistics
  }

*/

  /* If OEM data is not required, customer should treat devId as I2C slave Address */

  //devId = devId;
  //RegAddr = RegAddr;
  //*DataPtr = *DataPtr;
#if 0
	__u8    	data[2];
	__u8    dataread = 0;
	__u8    	byte_addr;
	__u16    	slave_addr;
  	__s32	ret = -1;
	__twi_dev_para_ex_t  piic;

	slave_addr = 0x60;//devId;
	byte_addr = RegAddr;
	data[0] = 0xfb;
	data[1] =byte_addr;

	//esKRNL_TimeDly(1);
	eLIBs_memset(&piic, 0, sizeof(__twi_dev_para_ex_t));

	piic.slave_addr_flag = TWI_SLAVE_ADDRESS_07BIT;
	piic.slave_addr      = 0x60;//slave_addr;////ui8I2CSlaveAddr>>1;
	
	piic.byte_addr       	   = &data[0];
	piic.byte_addr_width = 1;

	piic.data            = &data[1];
	piic.byte_count      = 1;
	ret = eLIBs_fioctrl(fiic, TWI_WRITE_SPEC_RS, 0, (void *)&piic);    

	WARNING("-------read write ret= %d\n",ret);
	esKRNL_TimeDly(1);

	eLIBs_memset(&piic, 0, sizeof(__twi_dev_para_ex_t));

	piic.slave_addr_flag = TWI_SLAVE_ADDRESS_07BIT;
	piic.slave_addr      = 0x60;//slave_addr;////ui8I2CSlaveAddr>>1;
	
	piic.byte_addr       = 0x00;//&byte_addr;////4-7
	piic.byte_addr_width = 0;

	piic.data            = &dataread;
	piic.byte_count      = 1;
	ret = eLIBs_fioctrl(fiic, TWI_READ_EX_NO_RS, 0, (void *)&piic);    

	*DataPtr = dataread;
	WARNING("-------read read ret= %d,dataread = %d\n",ret,dataread);

	if(0 == ret)
		status  = MXL_TRUE;
	else
		status  = MXL_FALSE;

  return status;
#else

	__u8    	data[2];
 	__s32	u8tmp,ret = -1;

	data[0] = 0xfb;
	data[1] = RegAddr;

	for(u8tmp = 0; u8tmp < 3; u8tmp++)
	{
		ret = iic_write(0x60,&data[0], 1, &data[1], 1);
		if(0 == ret)
		{
			status = MXL_TRUE;
			break;
		}
		else
		{
			status = MXL_FALSE;
			esKRNL_TimeDly(1);
		}
	}
	//////
	if(MXL_FALSE == status)
	{
		  return status;
	}
	////////
	esKRNL_TimeDly(1);
	data[0] = 0x00;
	data[1] = 0x00;

	for(u8tmp = 0; u8tmp < 3; u8tmp++)
	{
		ret =iic_read(0x60, &data[0], 0, DataPtr, 1);
		if(0 == ret)
		{
			status = MXL_TRUE;
			break;
		}
		else
		{
			status = MXL_FALSE;
			esKRNL_TimeDly(1);
		}
	}

	return status;
	
 #endif
}

/*------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare603_OEM_Sleep
--| 
--| AUTHOR        : Dong Liu
--|
--| DATE CREATED  : 01/10/2010
--|
--| DESCRIPTION   : This function complete sleep operation. WaitTime is in ms unit
--|
--| RETURN VALUE  : None
--|
--|-------------------------------------------------------------------------------------*/

void MxLWare603_OEM_Sleep(UINT16 DelayTimeInMs)
{
  // OEM should implement sleep operation 
  //DelayTimeInMs = DelayTimeInMs;
  esKRNL_TimeDly(DelayTimeInMs);
  
}

