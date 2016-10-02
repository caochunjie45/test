
#ifndef OS_ADAPTER_H
#define OS_ADAPTER_H

#include "demodDrv.h"
#include "drv_twi.h"

//#include "platform_dibcom.h"

#define USE_BOARD_6007      (0)

#if 0
typedef __s8   INT8;
typedef __s16  INT16;
typedef __s32  INT32;
typedef __u8   UINT8;
typedef __u16  UINT16;
typedef __u32  UINT32;
#endif

#if 1
#define demod_ATBM781X_printf   __msg//WARNING//eLIBs_printf
#else
//#define printf    
#define demod_MB86A2x_printf(...)   
#endif

int iic_init(void);

int iic_exit(void);

int iic_write(int devAddr, unsigned char* regAddr, int addrSize, unsigned char* txBuf, int txCnt);

int iic_read(int devAddr, unsigned char* regAddr, int addrSize, unsigned char* rxBuf, int rxCnt);

void msleep(int ms);

void usleep(int us);


#endif

