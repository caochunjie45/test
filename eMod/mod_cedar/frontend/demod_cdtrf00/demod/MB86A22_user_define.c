/*
***************************************************************************
*
*  Front-end device driver for Fujitsu MB86A22 ISDB-T frontend.
*
*  Copyright (c) 2010 Fujitsu Semiconductor 
*  All rights reserved. 
*
*  Created 12/14/2010
*
***************************************************************************
*/

//#include "..\platform.h" //#include "stdio.h"
//#include "stdlib.h"
//#include "..\pfdriver.h"
#include "../DataType.h"
#include "MB86A22_API_defs.h"
#include "../tuner/rdatv_bb_defs.h"
#include "MB86A22_user_define.h"
#include "Os_adapter.h"

/*
***************************************************************************
*	MB86A22 Timer function
*	Unit = ms
***************************************************************************
*/
void MB86A2x_wait( SINT32 wait_time ) {
	//You should implement delay function[ms].
    msleep(wait_time);
	//Gyi2c_wait( wait_time ) ;
}

/*
***************************************************************************
*	MB86A21 I2C Write Access
*	Return value  OK:1, NG;0
***************************************************************************
*/
UINT32 MB86A2x_write( UINT8 OFDM_adr, UINT8 address , UINT8 value ) {
	// You should implement I2C write protocol that complies with MB86A22 I2C
	// format.

	// 8 bit Register Write Protocol:
	// +------+-+--------+-+-+---------+-+--------+-+-+
	// |MASTER|S|OFDM_adr|W| |address  | |value   | |P|
	// +------+-+--------+-+-+---------+-+--------+-+-+
	// |SLAVE |            |A|         |A|        |A| |
	// +------+------------+-+---------+-+--------+-+-+
	// OFDM_adr (MB86A22 device address, 0x20 or 0x22 or 0x24 or 0x26)
	// S (Start condition), A (Ack), N(NACK), 
	// P(Stop condition)

	/*
	SINT32 i2cok ;

	i2cok =  I2Cwrite( OFDM_adr, address, value ) ;

	if( i2cok==1 ) return I2C_OK ;
	else           return I2C_NG ;
	*/
   RDA5880_ERR_MSG status = (RDA5880_ERR_MSG)I2C_OK;
  UINT8 i;
    for(i=0;i<10;i++)
   {
        // ATBM781x need turn on through mode, then read or write Tuner
        status = (RDA5880_ERR_MSG)iic_write(OFDM_adr>>1, &address, 1, &value,  1);
       //MxLWare603_OEM_Sleep(1);
        if(status != 0)
        {
            continue;
        }
        else
        {
            return I2C_OK;
        }
			
    }

	return I2C_NG;
}

/*
***************************************************************************
*	MB86A22 I2C Read Access
*	Return value  OK:1, NG;0
***************************************************************************
*/
UINT32 MB86A2x_read( UINT8 OFDM_adr, UINT8 address , UINT8 *value ) {
	// You should implement I2C read protocol that complies with MB86A22 I2C
	// format.

	// 8 bit Register Read Protocol:
	// +------+-+--------+-+-+----------+-+-+
	// |MASTER|S|OFDM_adr|W| |address   | |P|
	// +------+-+--------+-+-+----------+-+-+
	// |SLAVE |            |A|          |A| |
	// +------+-+----------+-+----------+-+-+
	// +------+-+--------+-+-+----------+-+-+
	// |MASTER|S|OFDM_adr|R| |          |N|P|
	// +------+-+--------+-+-+----------+-+-+
	// |SLAVE |            |A|*value    | | |
	// +------+------------+-+----------+-+-+
	// OFDM_adr (MB86A22 device address, 0x20 or 0x22 or 0x24 or 0x26)
	// S(Start condition), A(Ack), N(NACK), 
	// P(Stop condition)

	/*
	SINT32 i2cok ;

	i2cok =  I2Cread( OFDM_adr, address, value ) ;

	if( i2cok==1 ) return I2C_OK ;
	else           return I2C_NG ;
	*/
	RDA5880_ERR_MSG status = (RDA5880_ERR_MSG)I2C_OK;
    UINT8 i;
    for(i=0;i<10;i++)
   {
   	#if 0
        // ATBM781x need turn on through mode, then read or write Tuner
        status = iic_write(OFDM_adr, &address, 1, NULL,  0);
       //MxLWare603_OEM_Sleep(1);
        if(status != 0)
        {
            continue;
        }
	#endif
        status = (RDA5880_ERR_MSG)iic_read(OFDM_adr>>1, &address, 1, value,  1);
        if(status == 0)
        {
           return I2C_OK;
        }
    }
	return I2C_NG ;
}

