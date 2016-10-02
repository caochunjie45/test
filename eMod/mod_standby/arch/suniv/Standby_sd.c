#include "standby_sd.h"
/*
#define SD_GCTL 0x00;
#define SD_CKCR 0x04;
#define SD_TMOR 0x08;
...
#define SD_IMKR 0x30;
#define SD_MISR 0x34;
#define SD_RISR 0x38;
#define SD_STAR 0x3c;
*/

#if(ALLOW_SD_WAKEUP_STANDBY)




/*
****************************************************************************************************
*                                       
*              
* Description: 
*              
* Arguments  : 
*              
* Return     : 
*              
* Notes      : 
*              
****************************************************************************************************
*/
__s32 standby_sd_init(void)
{
    return EPDK_OK;
}

/*
****************************************************************************************************
*                                       
*              
* Description: 
*              
* Arguments  : 
*              
* Return     : 
*              
* Notes      : 
*              
****************************************************************************************************
*/
__s32 standby_sd_exit(void)
{
    return EPDK_OK;
}


/*
****************************************************************************************************
*                                       
*              
* Description: check  state.
*              
* Arguments  : 
*              
* Return     : 
*              
* Notes      : 
*              
****************************************************************************************************
*/
__s32 standby_query_sd_event(void)
{
	__u32 sdc_dect		= 0;
	__s32 ret;
	/*使用id/vbus 模式，只检测sdmmc0*/
	__u32 *pg_data;
	__u32 data;

	pg_data =(__u32 *) 0xF1C208E8;	
	data = *pg_data;
	sdc_dect = (data &(1 << 0x0));

	if(pStandbyPar->enter_standby_status.sdmmc0_status == 0)
	{	
		if(sdc_dect == 1)
		{
			ret = EPDK_TRUE;
		}
		else
		{
			ret = EPDK_FALSE;
		}
	}
	else
	{
		if(sdc_dect == 1)
		{
			ret = EPDK_FALSE;
		}
		else
		{
			ret = EPDK_TRUE;
		}
	}	
	return ret;
}

#endif
