/*
*********************************************************************************************************
*                                                    MELIS
*                                    the Easy Portable/Player Develop Kits
*                                                Cache Operations
*
*                                    (c) Copyright 2011-2014, Sunny China
*                                             All Rights Reserved
*
* File    : cache.c
* By      : Sunny
* Version : v1.0
* Date    : 2011-12-1
* Descript: melis cache operations.
* Update  : date                auther      ver     notes
*           2011-12-1 15:03:08  Sunny       1.0     Create this file.
*********************************************************************************************************
*/

#include "elibs.h"

void eLIBs_CleanDCache(void)
{
    esMEMS_CleanDCache();
}

void eLIBs_CleanFlushDCache(void)
{
    esMEMS_CleanFlushDCache();
}

void eLIBs_CleanFlushCache(void)
{
    esMEMS_CleanFlushCache();
}

void eLIBs_FlushDCache(void)
{
    esMEMS_FlushDCache();
}

void eLIBs_FlushICache(void)
{
    esMEMS_FlushICache();
}

void eLIBs_FlushCache(void)
{
    esMEMS_FlushCache();
}

void eLIBs_CleanDCacheRegion(void *adr, __u32 bytes)
{
    esMEMS_CleanDCacheRegion(adr, bytes);
}

void eLIBs_CleanFlushDCacheRegion(void *adr, __u32 bytes)
{
    esMEMS_CleanFlushDCacheRegion(adr, bytes);
}

void eLIBs_CleanFlushCacheRegion(void *adr, __u32 bytes)
{
    esMEMS_CleanFlushCacheRegion(adr, bytes);
}

void eLIBs_FlushDCacheRegion(void *adr, __u32 bytes)
{
    esMEMS_FlushDCacheRegion(adr, bytes);
}
void eLIBs_FlushICacheRegion(void *adr, __u32 bytes)
{
    esMEMS_FlushICacheRegion(adr, bytes);
}
void eLIBs_FlushCacheRegion(void *adr, __u32 bytes)
{
    esMEMS_FlushCacheRegion(adr, bytes);
}
