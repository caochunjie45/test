/*
********************************************************************************
*                                    eMOD
*                   the Easy Portable/Player Develop Kits
*                               mod_cedar sub-system
*                          (module name, e.g.mpeg4 decoder plug-in) module
*
*          (c) Copyright 2010-2012, Allwinner Microelectronic Co., Ltd.
*                              All Rights Reserved
*
* File   : maple_common.h
* Version: V1.0
* By     : Eric_wang
* Date   : 2011-2-13
* Description:
    主控模块群会根据需要划分为若干模块,这些模块的共用部分就放在本头文件中
********************************************************************************
*/

#ifndef _MAPLE_COMMON_H
#define _MAPLE_COMMON_H

//#include "ePDK.h"
#include "mod_cedar_i.h"
#include "maple_config.h"
//#include "mod_maple.h"
#include "demod_interface.h"
#include "tsc_interface.h"
#include "tsDemux_interface.h"
#include "player_interface.h"
//#include "util.h"

#include "cmmbDrv_interface.h"
#include "cmmbDemux_interface.h"

#include "util.h"

//#undef DECLS_BEGIN
//#undef DECLS_END
//
//#ifdef __cplusplus
//#define DECLS_BEGIN extern "C" {
//#define DECLS_END   }
//#else
//#define DECLS_BEGIN
//#define DECLS_END
//#endif/*__cplusplus*/

//#define CONFIG_USE_ARM_MEMORY_FUNCTIONS         (0)

//***********************************************************************//
//*********** define module install address for each module *************//
//***********************************************************************//
#define MAPLE_INSTALL_ADDR                      EMOD_TYPE_DRV_MM_BASE       //* 0x50

#define PLAYER_AVSYNC_DRV_INSTALL_TYPE          (MAPLE_INSTALL_ADDR + 0)
#define PLAYER_VDEC_DRV_INSTALL_TYPE            (MAPLE_INSTALL_ADDR + 1)
#define PLAYER_ADEC_DRV_INSTALL_TYPE            (MAPLE_INSTALL_ADDR + 2)

#define TSC_DRV_INSTALL_TYPE                    (MAPLE_INSTALL_ADDR + 4)
#define CMMB_DRV_INSTALL_TYPE                   (MAPLE_INSTALL_ADDR + 4)
#define DEMOD_DRV_INSTALL_TYPE                  (MAPLE_INSTALL_ADDR + 5)
#define EMOD_TYPE_DRV_DEMOD						(MAPLE_INSTALL_ADDR + 6)


//***********************************************************************//
//**************************** redefine types ***************************//
//***********************************************************************//
//typedef signed char                 __s8;
//typedef signed short                __s16;
//typedef signed int                  __s32;
//typedef long long int               __s64;
//typedef unsigned char               __u8;
//typedef unsigned short              __u16;
//typedef unsigned int                __u32;
//typedef unsigned long long int      __u64;



//==============================================================================
//                      FILE SYSTEM SYSTEM-CALL
//==============================================================================
//typedef struct __ESFILE                 FILE;
//#define fopen(name, mode)               eLIBs_fopen((name),(mode))
//#define fclose(fptr)                    eLIBs_fclose((fptr))
//#define fread(buf,unit,size,fptr)       eLIBs_fread((buf),(unit),(size),(fptr))
//#define fwrite(buf,unit,size,fptr)      eLIBs_fwrite((buf), (unit), (size), (fptr))
//#define fseek(fptr,offset,origin)       eLIBs_fllseek((fptr),(offset),(origin))
//#define ftell(fptr)                     eLIBs_flltell((fptr))
//#define fioctrl(fp, cmd, aud, pbuf)     eLIBs_fioctrl((fp), (cmd), (aud), (pbuf))

#define fmkdir(dir)                     eLIBs_mkdir(dir);

//==============================================================================
//                      PRINT INFORMATION SYSTEM-CALL
//==============================================================================
//#define MSG                 __msg
//#define MESSAGE             __msg
//#define WRN                 __wrn
//#define WARNING             __wrn
//#define ERR                 __err
//#define ERROR               __err
//#define LOG                 __log
//#define INF                 __inf
//#define INFORMATION         __inf
//#define printf              eLIBs_printf
//#define DUMP_MEM(x, y, z)   eLIBs_dumpmem(x, y, z)



//==============================================================================
//                      MEMORY OPERATION SYSTEM-CALL
//==============================================================================
//#define MALLOC(x)                       esMEMS_Malloc(0, x)
//#define malloc(x)                       esMEMS_Malloc(0, x)
//#define FREE(x)                         esMEMS_Mfree(0, x)
//#define free(x)                         esMEMS_Mfree(0, x)
//#define palloc(x, y)                    esMEMS_Palloc(x, y)
//#define pfree(x, y)                     esMEMS_Pfree(x, y)
//#define realloc(f, x)                   esMEMS_Realloc(0, f, x)
//#define REALLOC(f, x)                   esMEMS_Realloc(0, f, x)
//
//#define BALLOC(x)                       esMEMS_Balloc(x)
//#define BFREE(x, y)                     esMEMS_Bfree(x, y)

//#if CONFIG_USE_ARM_MEMORY_FUNCTIONS
//
//#define memset(buf, val, size)          eLIBs_memset(buf, val, size)
//#define MEMSET                          memset
//#define memcpy(dst, src, size)          eLIBs_memcpy(dst, src, size)
//#define MEMCPY                          memcpy
//#define memcmp(src1, src2, size)        eLIBs_memcmp(src1, src2, size)
//#define MEMCMP                          memcmp
//  
//#else
//
//#define memset(buf, val, size)          eLIBs_memset(buf, val, size)
//#define MEMSET                          memset
//#define memcpy(dst, src, size)          eLIBs_memcpy(dst, src, size)
//#define MEMCPY                          memcpy
//#define memcmp(src1, src2, size)        eLIBs_memcmp(src1, src2, size)
//#define MEMCMP                          memcmp  
//
//#endif                   

//#define MEM_SET         MEMSET
//#define MEM_CPY         MEMCPY

//#define TOUPPER(n)                      eLIBs_toupper(n)
//#define toupper(n)                      eLIBs_toupper(n)
//#define STR_NCMP(dst, src, size)        eLIBs_strncmp(dst, src, size)
//#define strncmp(dst, src, size)         eLIBs_strncmp(dst, src, size)
//#define STR_CMP(s1, s2)                 eLIBs_strcmp(s1, s2)
//#define strcmp(s1, s2)                  eLIBs_strcmp(s1, s2)
//#define STR_LEN(s)                      eLIBs_strlen(s)
//#define strlen(s)                       eLIBs_strlen(s)
//#define STR_NCPY(s1, s2, n)             eLIBs_strncpy(s1, s2, n)
//#define strncpy(s1, s2, n)              eLIBs_strncpy(s1, s2, n)
//#define STR_CPY(s1, s2)                 eLIBs_strcpy(s1, s2)
//#define strcpy(s1, s2)                  eLIBs_strcpy(s1, s2)
//#define STR_CHR(s, ch)                  eLIBs_strchr(s,ch)
//#define strchr(s, ch)                   eLIBs_strchr(s,ch)
//#define STR_STR(s1, s2)                 eLIBs_strstr(s1, s2)
//#define strstr(s1, s2)                  eLIBs_strstr(s1, s2)
//#define STR_CAT(s1, s2)                 eLIBs_strcat(s1, s2)
//#define strcat(s1, s2)                  eLIBs_strcat(s1, s2)
//
//#define stricmp(s1, s2)                 eLIBs_stricmp(s1, s2)
//#define strnicmp(dst, srt, size)        eLIBs_strnicmp(dst, srt, size)
//#define strrchr(s, ch)                  eLIBs_strchrlast(s,ch)
//#define sprintf                         eLIBs_sprintf


//#define return_if_fail(p) if(!(p)) \
//	{WARNING("Warning: "#p" failed.\n" \
//		); return;}
//#define return_val_if_fail(p, ret) if(!(p)) \
//	{WARNING("Warning: "#p" failed.\n" \
//	); return (ret);}

//==============================================================================
//                      TASK SCHEDULE SYSTEM-CALL
//==============================================================================
//#define DISABLE_OS_TASK_SWITCH()        esKRNL_SchedLock()
//#define ENABLE_OS_TASK_SWITCH()         esKRNL_SchedUnlock()
//#define VirAddr2PhyAddr(x)              esMEMS_VA2PA((void *)(x))

//#define DisableSwitch()                 esKRNL_SchedLock()
//#define EnableSwitch()                  esKRNL_SchedUnlock()

//#define DisableInterrupt()              ENTER_CRITICAL(cpu_sr)
//#define EnableInterrupt()               EXIT_CRITICAL(cpu_sr)


#endif

