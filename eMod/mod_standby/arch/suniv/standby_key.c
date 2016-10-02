/*
*********************************************************************************************************
*                                                    eMOD
*                                   the Easy Portable/Player Operation System
*                                            power manager sub-system
*
*                                     (c) Copyright 2008-2009, kevin.z China
*                                              All Rights Reserved
*
* File   : power.c
* Version: V1.0
* By     : kevin.z
* Date   : 2009-6-16 17:31
*********************************************************************************************************
*/
#include "standby_key.h"


#if(ALLOW_TPADCKEY_WAKEUP_STANDBY)



/*
*********************************************************************************************************
*                                     INIT KEY FOR STANDBY
*
* Description: init key for standby.
*
* Arguments  : none
*
* Returns    : EPDK_OK;
*********************************************************************************************************
*/
__s32 standby_key_init(void)
{
	

	__u32		 reg_value;

/*
	
	// 初始化REG_CTRL
		TpCtlRegBak1 = STANDBY_RREG(0xf1c25004);
		reg_value = reg_value & ~(0x01 << 4);		// set tp disable
		//reg_value = reg_value | (0x01 << 1);		// set ADC enable
		reg_value = reg_value | (0x01 << 3);		// set ADC select
		reg_value = reg_value | ~(0x00 << 0);		 // set 4 channel select robin round
	//	reg_value = reg_value | (0x0f << 13);		// set CLK_IN/32
	//	reg_value = reg_value | (0x01 << 17);		// HOSC(24MHz)
		reg_value = reg_value & ~(0x01 << 5);		// 
	
		STANDBY_WREG(0xf1c25004, reg_value);
		
		// 初始化REG_INT
		TPIntCtlBak = STANDBY_RREG(0xf1c25010);
		reg_value |= TPADCKEY_IRQ_EN;  // disable int
		STANDBY_WREG(0xf1c2501, reg_value);
	
		// 初始化REG_FIFOC
		reg_value = STANDBY_RREG(0xf1c25010);
		reg_value = reg_value | (0x01 << 0);		// FIRST Touch IRQ DISABLE
		reg_value = reg_value | (0x01 << 1);		// LAST Touch IRQ DISABLE
		//reg_value = reg_value | (0x01 << 7);		// DRQ  enable
		reg_value = reg_value | (0x01 << 13);		// TP  xy change  enable
		reg_value = reg_value | (0x01 << 16);		// FIFO DATA IRQ ENABLE
		reg_value = reg_value | (0x01 << 17);		// FIFO OVER RUN IRQ ENABLE
		STANDBY_WREG(0xf1c25010, reg_value);
*/
	return EPDK_OK;
	

}


/*
*********************************************************************************************************
*                                     EXIT KEY FOR STANDBY
*
* Description: exit key for standby.
*
* Arguments  : none
*
* Returns    : EPDK_OK;
*********************************************************************************************************
*/
__s32 standby_key_exit(void)
{


	
    return EPDK_OK;
}

#endif  //ALLOW_LRADCKEY_WAKEUP_STANDBY

