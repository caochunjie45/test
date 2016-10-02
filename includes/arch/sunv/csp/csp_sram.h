/*
****************************************************************************************************
*                                               MELIS
*                               the Easy Portable/Player Develop Kits
*                                           SRAM Module
*
*                           (c) Copyright 2011-2014, All winners Co,Ld.
*                                       All Rights Reserved
*
* File    : csp_sram.h
* By      : james.deng
* Version : 1.0.0
* Date    : 2011-11-25
* Descript: the header of sram module.
* Update  : <date>          <author>            <version>           <notes>
*           2011-11-25      james.deng          1.0.0               build the file.
****************************************************************************************************
*/

#ifndef	__CSP_SRAM_H__
#define	__CSP_SRAM_H__


// SRAM ZONE ID
typedef	enum __CSP_SRAM_ZONE
{
	CSP_SRAM_ZONE_A1	     ,
	CSP_SRAM_ZONE_A2	     ,
	CSP_SRAM_ZONE_A3	     ,
	CSP_SRAM_ZONE_A4	     ,

	CSP_SRAM_ZONE_B1	     ,
                             
	CSP_SRAM_ZONE_C1	     ,
	CSP_SRAM_ZONE_C2	     ,
	CSP_SRAM_ZONE_C3	     ,
	                          
	CSP_SRAM_ZONE_D1	     ,
                             
	CSP_SRAM_ZONE_NFC	     ,
    
	CSP_SRAM_ZONE_MAX_NR
} __csp_sram_zone_e;


// Module
typedef enum __CSP_SRAM_MODULE
{
	CSP_SRAM_MODULE_CPU_DMA	        ,
	CSP_SRAM_MODULE_VE		        ,
	CSP_SRAM_MODULE_ISP	            ,
	CSP_SRAM_MODULE_AE		        ,
	CSP_SRAM_MODULE_CE		        ,
	CSP_SRAM_MODULE_ACE		        ,
	CSP_SRAM_MODULE_EMAC			,
	CSP_SRAM_MODULE_USB0			,

	CSP_SRAM_MODULE_MAX_NR
} __csp_sram_module_e;


typedef struct __SRAM_ZONE_INFO
{
	__csp_sram_zone_e ZoneID;
	__u32             ZoneSize;
} __sram_zone_info_t;


/*
****************************************************************************************************
*                                       CSP_SRAM_Init
*              
* Description: Init sram controller.
*              
* Arguments  : 
*              
* Return     : EPDK_OK
*              
* Notes      : 
*              
****************************************************************************************************
*/
__s32 CSP_SRAM_Init(void);

/*
****************************************************************************************************
*                                       CSP_SRAM_Exit
*              
* Description: Exit sram controller.
*              
* Arguments  : 
*              
* Return     : EPDK_OK
*              
* Notes      : 
*              
****************************************************************************************************
*/
__s32 CSP_SRAM_Exit(void);

/*
****************************************************************************************************
*                                       CSP_SRAM_GetZoneNumber
*              
* Description: Get the number of sram.
*              
* Arguments  : 
*              
* Return     : __s32        : the number of sram.
*              
* Notes      : 
*              
****************************************************************************************************
*/
__s32 CSP_SRAM_GetZoneNumber(void);

/*
****************************************************************************************************
*                                       CSP_SRAM_GetZoneInfo
*              
* Description: Get the information of sram.
*              
* Arguments  : nZoneIndex   : the index of sram zone, start with 0.
*              pZoneInfo    : the information of sram, allocate by caller.
*              
* Return     : EPDK_OK
*              EPDK_FAIL
*              
* Notes      : 
*              
****************************************************************************************************
*/
__s32 CSP_SRAM_GetZoneInfo(__u32 nZoneIndex, __sram_zone_info_t *pZoneInfo);

/*
****************************************************************************************************
*                                       CSP_SRAM_SwitchMapping
*              
* Description: Switch mapping of sram.
*              
* Arguments  : ZoneID       : the sram zone id.
*              MapModule    : the module to be mapping.
*              
* Return     : EPDK_OK
*              EPDK_FAIL
*              
* Notes      : 
*              
****************************************************************************************************
*/
__s32 CSP_SRAM_SwitchMapping(__csp_sram_zone_e ZoneID, 
                             __csp_sram_module_e MapModule);

#endif	//__CSP_SRAM_H__

