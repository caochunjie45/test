/*
**************************************************************************************************************
*											         ePDK
*						            the Easy Portable/Player Develop Kits
*									           desktop system 
*
*						        	 (c) Copyright 2007-2010, ANDY, China
*											 All Rights Reserved
*
* File    	: mode_desktop.c
* By      	: Andy.zhang
* Func		: system lib
* Version	: v1.0
* ============================================================================================================
* 2009-8-25 8:53:32  andy.zhang  create this file, implements the fundemental interface;
**************************************************************************************************************
*/

#include "mod_desktop_i.h"

__desktop_t desktp_data;

/*
****************************************************************************************************
*
*             DESKTOP_MInit
*
*  Description:
*       DESKTOP_MInit
*
*  Parameters:
*  
*  Return value:
*       EPDK_OK
*       EPDK_FAIL
****************************************************************************************************
*/    
__s32 DESKTOP_MInit(void)
{		
	return EPDK_OK;
}
/*
****************************************************************************************************
*
*             DESKTOP_MExit
*
*  Description:
*       DESKTOP_MExit
*
*  Parameters:
*  
*  Return value:
*       EPDK_OK
*       EPDK_FAIL
****************************************************************************************************
*/
__s32 DESKTOP_MExit(void)
{
    return EPDK_OK;
}
/*
****************************************************************************************************
*
*             DESKTOP_MOpen
*
*  Description:
*       DESKTOP_MOpen
*
*  Parameters:
*  
*  Return value:
*       EPDK_OK
*       EPDK_FAIL
****************************************************************************************************
*/
__mp *DESKTOP_MOpen(__u32 mid, __u32 mod)
{      
    __msg("----------------------DESKTOP_MOpen --------------------------\n");
 
    desktp_data.mid  = mid;	
    
    desktp_data.init_id = esMODS_MInstall(BEETLES_APP_ROOT"apps\\init.axf", 0);
    desktp_data.init_mp = esMODS_MOpen(desktp_data.init_id, 0);
    //esEXEC_Run(BEETLES_APP_ROOT"apps\\init.axf",0,0,0);

    __inf("0\n");
    return (__mp *)&desktp_data;
}
/*
****************************************************************************************************
*
*             DESKTOP_MClose
*
*  Description:
*       DESKTOP_MClose
*
*  Parameters:
*  
*  Return value:
*       EPDK_OK
*       EPDK_FAIL
****************************************************************************************************
*/
__s32 DESKTOP_MClose(__mp *mp)
{
    __here__;
	esMODS_MClose(desktp_data.init_mp);
    __here__;
	esMODS_MUninstall(desktp_data.init_id);		
	__here__;
	desktp_data.init_mp = NULL;
	desktp_data.init_id = 0;	
	
    return EPDK_OK;
}
/*
****************************************************************************************************
*
*             DESKTOP_MRead
*
*  Description:
*       DESKTOP_MRead
*
*  Parameters:
*  
*  Return value:
*       EPDK_OK
*       EPDK_FAIL
****************************************************************************************************
*/
__u32 DESKTOP_MRead(void *pdata, __u32 size, __u32 n, __mp *mp)
{
    return size * n;
}
/*
****************************************************************************************************
*
*             DESKTOP_MWrite
*
*  Description:
*       DESKTOP_MWrite
*
*  Parameters:
*  
*  Return value:
*       EPDK_OK
*       EPDK_FAIL
****************************************************************************************************
*/
__u32 DESKTOP_MWrite(const void *pdata, __u32 size, __u32 n, __mp *mp)
{   
    return size * n;
}
/*
****************************************************************************************************
*
*             DESKTOP_MIoctrl
*
*  Description:
*       DESKTOP_MIoctrl
*
*  Parameters:
*  
*  Return value:
*       EPDK_OK
*       EPDK_FAIL
****************************************************************************************************
*/
__s32 DESKTOP_MIoctrl(__mp *mp, __u32 cmd, __s32 aux, void *pbuffer)
{   
	return EPDK_OK;
}    






