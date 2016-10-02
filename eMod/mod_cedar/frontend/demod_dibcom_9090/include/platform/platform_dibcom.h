#ifndef DIBTYPES_H
#define DIBTYPES_H

//#include <stdlib.h>
//#include <stdio.h>
//#include <string.h>

// For example: This is embed in ZORAN system.
// Please remove the define(__ZORAN_SYSTEM__), unless you are using the ZORAN system.
//#define __ZORAN_SYSTEM__

/////////////////////////////////////
// include system files
#ifdef __ZORAN_SYSTEM__
#include <math.h>
#include "Z_os_api.h"
#include "globals.h"
#include "timer.h"

extern void FreeMemory( void *ptr );
extern void* GetMemory(size_t size);

#define MemAlloc	                    GetMemory
#define	MemFree( chunk, size )	        FreeMemory(chunk)
#define DibZeroMemory(buf, len)         memset(buf, 0, len)

#define DibMSleep(v)                    Timer_wait(v * 1000) // OS_Sleep(v)
#define DibUSleep(v)                    Timer_wait(v) // OS_Sleep(v/1000)

#define systime                         TMT_Retrieve_Clock

#else

// Please write you system files here
// #include ...

#include "ePDK.h"

// Please write you system files here

#define MemAlloc                           eLIBs_malloc
#define	MemFree( chunk, size )   eLIBs_free(chunk)

#define DibZeroMemory(buf, len)    eLIBs_memset( (char *)buf, 0, len)

#define DibMSleep(v)    msleep(v)
#define DibUSleep(v)    usleep(v)

// Please write you system files here
// System ticks
#define systime   esKRNL_TimeGet

#if 0
#define printf    eLIBs_printf
#define demod_9090_printf   eLIBs_printf
#else
#define printf    
#define demod_9090_printf   
#endif

#endif



////////////////////////////////////
// define the the length of data types
typedef double          REAL;
typedef unsigned char   uint8_t;
typedef char            int8_t;
typedef unsigned int    uint32_t;
typedef int             int32_t;
typedef unsigned short  uint16_t;
typedef short           int16_t;

//
typedef int             DIB_LOCK;

// If not use DiB0190, please remove the difine
//#define _USE_DIBCOM0190_PMU_

//Just for debug
#define DIBCOM_DEBUG_INFO

#define DibInitLock(lock)
#define DibFreeLock(lock)
#define DibAcquireLock(lock)
#define DibReleaseLock(lock)
#define DibAllocateLock(lock)
#define DibDeAllocateLock(lock)

#ifdef DIBCOM_TESTING
struct dibI2CAccess;
extern void debug_i2c_write(struct dibI2CAccess *msg);
#endif

#endif
