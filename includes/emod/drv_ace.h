/*
*********************************************************************************************************
*                                                    MELIS
*                                    the Easy Portable/Player Develop Kits
*                                               ACE Manager Module
*
*                                    (c) Copyright 2011-2012, young.gq.yang China
*                                             All Rights Reserved
*
* File    : drv_ace.h
* By      : young.gq.yang
* Version : v1.0
* Date    : 2011-3-8 9:53
* Descript: define the interface for ACE manager;
* Update  : date                auther      ver     notes
*
*********************************************************************************************************
*/
#ifndef __ACE_HAL_H__
#define	__ACE_HAL_H__

#include "typedef.h"
#include "cfgs.h"

// operation result
// EPDK_OK  - ACE module operation successed;
// EPDK_FAIL- ACE module operation failed;


// defule type of modules in ACE
typedef enum __ACE_MODULE_TYPE
{
    ACE_MODULE_NONE = 0,
    ACE_MODULE_CE,
    ACE_MODULE_AE,
    ACE_MODULE_PNG,
    ACE_MODULE_TSCC,
    ACE_MODULE_

}__ace_module_type_e;


typedef enum __ACE_REQUEST_MODE
{
    ACE_REQUEST_MODE_WAIT = 0,  //request hw with waiting mode
    ACE_REQUEST_MODE_NOWAIT,    //request hw with no-wait mode
    ACE_REQUEST_MODE_

} __ace_request_mode_e;

#ifdef USED_BY_DRVACE	//used by drvace module itsef


/*
*********************************************************************************************************
*                                   ACE INIT
*
* Description: initialise ACE moudle, create the manager for resource management.
*
* Arguments  : none
*
* Returns    : result;
*               EPDK_OK   - init ACE successed;
*               EPDK_FAIL - init ACE failed;
*
* Note       : This funciton just create manager, without hardware operation;
*********************************************************************************************************
*/
__s32 ACE_Init(void);


/*
*********************************************************************************************************
*                                   ACE EXIT
*
* Description: exit ACE module, destroy resource manager;
*
* Arguments  : none
*
* Returns    : result;
*               EPDK_OK   - exit ace module successed;
*               EPDK_FAIL - exit ace module failed;
*
* Note       :
*********************************************************************************************************
*/
__s32 ACE_Exit(void);


/*
*********************************************************************************************************
*                                   REQUEST HARDWARE RESOURCE
*
* Description: require hardware resource.
*
* Arguments  : module   the hardware module which need be requested;
*              mode     mode of hardware module requested;
                            ACE_REQUEST_MODE_WAIT   - request hw resource with waiting mode;
                            ACE_REQUEST_MODE_NOWAIT - request hw resource with no-wait mode;
*              timeout  limitation of time out, just used under ACE_REQUEST_MODE_WAIT mode;
*
* Returns    : handle of hardware resource, NULL means request hw-resource failed;
*
* Note       :
*********************************************************************************************************
*/
__hdle ACE_HwReq(__ace_module_type_e module, __ace_request_mode_e mode, __u32 timeout);


/*
*********************************************************************************************************
*                                   RELEASE HARDWARE RESOURCE
*
* Description: release hardware resource;
*
* Arguments  : moudle   the module need be released;
*
* Returns    : result;
*                   EPDK_OK,    release hardware resource successed;
*                   EPDK_FAIL,  release hardware resource failed;
*
* Note       :
*********************************************************************************************************
*/
__s32 ACE_HwRel(__hdle hHWRes);

/*
*********************************************************************************************************
*                                       GET ACE MODULE CLOCK
*
* Description: This function Get ACE module clock;
*
* Arguments  :
*
* Returns    : nFreq    ce module clk freqrence value;
*********************************************************************************************************
*/
__u32 ACE_GetClk(void);

#else
/*
*********************************************************************************************************
*                                       FOR EXPORTS SYMBOL TABLE
*
* Description:
*
* Arguments  :
*
* Returns    :
*
* Note       :
*********************************************************************************************************
*/
typedef struct DRVACE_API_ENTRY
{
    __s32 (*ptr_ACE_Init)(void);
    __s32 (*ptr_ACE_Exit)(void);
    __hdle(*ptr_ACE_HwReq)(__ace_module_type_e module, __ace_request_mode_e mode, __u32 timeout);
    __s32 (*ptr_ACE_HwRel)(__hdle hHWRes);
    __u32 (*ptr_ACE_GetClk)(void);
} drvace_api_entry_t;


#define DRVACE_ENTRY_TBL   (0xe0900000)   //this address is depended on the link address of mod_drvace
#define DRVACE_FUNC_ENTRY  ((drvace_api_entry_t *)DRVACE_ENTRY_TBL)
#define ACE_Init  	(DRVACE_FUNC_ENTRY->ptr_ACE_Init	)
#define ACE_Exit  	(DRVACE_FUNC_ENTRY->ptr_ACE_Exit	)
#define ACE_HwReq 	(DRVACE_FUNC_ENTRY->ptr_ACE_HwReq	)
#define ACE_HwRel 	(DRVACE_FUNC_ENTRY->ptr_ACE_HwRel	)
#define ACE_GetClk	(DRVACE_FUNC_ENTRY->ptr_ACE_GetClk	)

#endif /*USED_BY_DRVACE*/


#endif	/* __ACE_HAL_H__ */

