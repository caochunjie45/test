// #include <stdio.h>
//#include "..\platform.h" //#include <stdio.h>
//#include "..\pfdriver.h"
#include "rdatv_bb_defs.h"
#include "../Os_adapter.h"

//extern int iic_write(int devAddr, unsigned char* regAddr, int addrSize, unsigned char* txBuf, int txCnt);
//extern int iic_read(int devAddr, unsigned char* regAddr, int addrSize, unsigned char* rxBuf, int rxCnt);
//extern void msleep(int ms);

RDA5880_ERR_MSG HAL_TUNER_Write( UINT8 dev_addr, UINT8 *reg, UINT8 reg_num, UINT8 *data, UINT8 num ) {
	// You should implement I2C write protocol that complies with MB86A22
	// and RDA5880HT format.

	
	// +------+-+--------+-+-+----------+-+----------+-+-+
	// |MASTER|S|OFDM_adr|W| |0xFE      | |0x00e     | |P|
	// +------+-+--------+-+-+----------+-+----------+-+-+
	// |SLAVE |            |A|          |A|          |A| |
	// +------+------------+-+----------+-+----------+-+-+
	// +------+-+--------+-+-+----------+-+----------+-+----------+-+-+
	// |MASTER|S|dev_addr|W| |*(data+0) | |*(data+1) | |*(data+2) | |P|
	// +------+-+--------+-+-+----------+-+----------+-+----------+-+-+
	// |SLAVE |            |A|          |A|          |A|          |A| |
	// +------+------------+-+----------+-+----------+-+----------+-+-+
	

  RDA5880_ERR_MSG status = RDA5880_OK;
  UINT8 i;
  UINT8 though_addr = 0xfe;
  UINT8 though_flag = 0x00;
    for(i=0;i<10;i++)
   {
        // ATBM781x need turn on through mode, then read or write Tuner
        status = (RDA5880_ERR_MSG)iic_write(MB86A22_I2C_ADDR>>1, &though_addr, 1, &though_flag,  1);
       //MxLWare603_OEM_Sleep(1);
        if(status != 0)
        {
            continue;
        }
        
        // write tuner data
        status =  (RDA5880_ERR_MSG)iic_write(RDA5880H_TUNER_I2C_ADDR>>1, reg, reg_num, data,  num);
		if(status == 0)
		{
			return RDA5880_OK;
		}
   	}
	return RDA5880_ERR ;
}


RDA5880_ERR_MSG HAL_TUNER_ReadbyAddr( UINT8 dev_addr, UINT8 reg, UINT8 *data, UINT8 num ) {
	// You should implement I2C write/read protocol that complies with MB86A22
	// and RDA5880HT format.

	
	// +------+-+--------+-+-+----------+-+-----------+-+-+
	// |MASTER|S|OFDM_adr|W| |0xFE      | |0x00e      | |P|
	// +------+-+--------+-+-+----------+-+-----------+-+-+
	// |SLAVE |            |A|          |A|           |A| |
	// +------+------------+-+----------+-+-----------+-+-+
	// +------+-+--------+-+-+----------+-+-+-------+-+-+----------+-+----------+-+-+
	// |MASTER|S|dev_addr|W| |reg       | |S|dev_adr|R| |          |A|          |N|P|
	// +------+-+--------+-+-+----------+-+-+-------+-+-+----------+-+----------+-+-+
	// |SLAVE |            |A|          |A|           |A|*(data+0) | |*(data+1) | | |
	// +------+------------+-+----------+-+-----------+-+----------+-+----------+-+-+
	//
	
  RDA5880_ERR_MSG status = RDA5880_OK;
  UINT8 i;
  UINT8 though_addr = 0xfe;
  UINT8 though_flag = 0x00;
    for(i=0;i<10;i++)
   {
   		
        // ATBM781x need turn on through mode, then read or write Tuner
        status = (RDA5880_ERR_MSG)iic_write(MB86A22_I2C_ADDR>>1, &though_addr, 1, &though_flag,  1);
		
        // write tuner data
        status =  (RDA5880_ERR_MSG)iic_read(dev_addr>>1, &reg, 1, data,  num);
		if(status == 0)
		{
			return RDA5880_OK;
		}
   	}
	return RDA5880_ERR ;
}


void SAL_PLT_DelayMS( UINT32 deley_t ) {
	//You should implement delay function[ms].
    msleep(deley_t);
	//Gyi2c_wait( deley_t ) ;
}
