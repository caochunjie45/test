#include "standby_tp.h"

__s32 standby_tp_init()
{
        __u32 reg_val;

	reg_val = STANDBY_RREG(0xF1C20800 + 0xDC);
	reg_val |= (0x06 << 12);
	STANDBY_WREG((0xF1C20800 + 0xDC),reg_val);

	reg_val = STANDBY_RREG(0xF1C20800 + 0x210);
	reg_val |= (0x1 << 11);
	STANDBY_WREG((0xF1C20800 + 0x210),reg_val);

	
	return EPDK_OK;
}

__s32 standby_query_tp_event()
{
	__u32 reg_val;
	__u32 reg_ret;
	
	reg_val = 0;
	reg_ret = 0;
	
	reg_val = STANDBY_RREG(0xF1C20800 + 0x214);
	reg_ret = reg_val&(0x01 << 11);
	if(reg_ret)
	{		
		return EPDK_TRUE;
	}
	return EPDK_FAIL;



}
