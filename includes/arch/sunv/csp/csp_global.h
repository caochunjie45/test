/*
*********************************************************************************************************
*                                                    MELIS
*                                    the Easy Portable/Player Develop Kits
*                                             SOC Global Information
*
*                                    (c) Copyright 2011-2014, Sunny China
*                                             All Rights Reserved
*
* File    : csp_global.h
* By      : Sunny
* Version : v1.0
* Date    : 2011-11-23
* Descript: 1623 soc global informations.
* Update  : date                auther      ver     notes
*           2011-11-23 13:52:38 Sunny       1.0     Create this file.
*********************************************************************************************************
*/

#ifndef	__CSP_GLOBAL_H__
#define	__CSP_GLOBAL_H__


//------------------------------------------------------------------------------
// module types
//------------------------------------------------------------------------------
#define	GLOBAL_MODULE_TYPE_NULL		0x00
#define	GLOBAL_MODULE_TYPE_IO		0x01
#define	GLOBAL_MODULE_TYPE_MEMORY	0x02


//------------------------------------------------------------------------------
// module item information
//------------------------------------------------------------------------------
typedef struct __MODULE_INFO_ITEM
{
	__u32 PhyBaseAddr;	//module`s io memory base address
	__u32 PhyIOAize;	//io memory space
	__u32 ModType;		//such as CSP_GLOBAL_MOD_TYPE_NULL
} __module_info_item_t;


//------------------------------------------------------------------------------
// soc modules information
//------------------------------------------------------------------------------
typedef	struct __SOC_MODULE_INFO
{
	__u32                 Number;       //the number of total modules
	__module_info_item_t *pModules;     //the table for store modules items,
	                                    //the table size depend on "Number".
} __soc_module_info_t;


//------------------------------------------------------------------------------
// golbal interface
//------------------------------------------------------------------------------
__soc_module_info_t *GLOBAL_GetSocModuleInfo(void);

#endif	//__CSP_GLOBAL_H__

