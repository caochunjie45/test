/*
*********************************************************************************************************
*                                                    MELIS
*                                    the Easy Portable/Player Develop Kits
*                                           power key board driver
*
*                                    (c) Copyright 2010-2012, Sunny China
*                                             All Rights Reserved
*
* File    : powerkey_dev_i.h
* By      : Sunny
* Version : v1.0
* Date    : 2011-1-7 8:58:35
* Descript: powerkey input device internal header file.
* Update  : date                auther      ver     notes
*           2011-1-7 8:58:39    Sunny       1.0     powerkey driver intrenal header file.
*********************************************************************************************************
*/

#ifndef     __POWERKEY_DEV_I_H__
#define     __POWERKEY_DEV_I_H__

#include    "epdk.h"

#define BITS_PER_BYTE		8
#define DIV_ROUND_UP(n,d)   (((n) + (d) - 1) / (d))
#define BITS_PER_LONG       (sizeof(long) * BITS_PER_BYTE)
#define BIT(nr)			    (1UL << (nr))
#define BIT_MASK(nr)		(1UL << ((nr) % BITS_PER_LONG))
#define BIT_WORD(nr)		((nr) / BITS_PER_LONG)
#define BITS_TO_LONGS(nr)	DIV_ROUND_UP(nr, BITS_PER_LONG)

/**
 * __set_bit - Set a bit in memory
 * @nr: the bit to set
 * @addr: the address to start counting from
 *
 * Unlike set_bit(), this function is non-atomic and may be reordered.
 * If it's called on the same region of memory simultaneously, the effect
 * may be that only one operation succeeds.
 */
static __inline void __set_bit(int nr, volatile unsigned long *addr)
{
	unsigned long mask = BIT_MASK(nr);
	unsigned long *p = ((unsigned long *)addr) + BIT_WORD(nr);

	*p  |= mask;
}

#endif      /* __POWERKEY_DEV_I_H__ */
