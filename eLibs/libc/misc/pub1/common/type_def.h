/*
************************************************************************************************************************
*                                                         eGON
*                                         the Embedded GO-ON Bootloader System
*
*                             Copyright(C), 2006-2008, SoftWinners Microelectronic Co., Ltd.
*											       All Rights Reserved
*
* File Name : type_def.h
*
* Author : Gary.Wang
*
* Version : 1.1.0
*
* Date : 2007.11.26
*
* Description : This file defines some basic data types and general-purpose macros.
*
* Others : None at present.
*
*
* History :
*
*  <Author>        <time>       <version>      <description>
*
* Gary.Wang       2007.11.26      1.1.0        build the file
*
************************************************************************************************************************
*/

#ifndef  __type_def_h
#define  __type_def_h




#if 1

typedef signed char			  s8;
typedef signed char			  __s8;
typedef signed char		      int8;
                              
typedef unsigned char		  u8;
typedef unsigned char		  __u8;
typedef unsigned char		  uint8;
                              
                              
typedef short int			  s16;
typedef short int			  __s16;
typedef short int			  int16;
                              
typedef unsigned short		  u16;
typedef unsigned short		  __u16;
typedef unsigned short		  uint16;
                              
typedef int					  s32;
typedef int					  __s32;
typedef int					  int32;
                              
typedef unsigned int		  u32;
typedef unsigned int		  __u32;
typedef unsigned int		  uint32;
                              
/*                            
typedef unsigned long long	  u64;
typedef unsigned long long	  __u64;
typedef unsigned long long	  uint64;
                              
typedef signed long long	  s64;
typedef signed long long	  __s64;
typedef signed long long	  int64;
*/

typedef int32                 __Bool;
#define __TRUE                1
#define __FALSE               0

#else  // #if 1


//------------------------------------------------------------------------------------------------------------
//!�з�������
//------------------------------------------------------------------------------------------------------------
#ifndef s8
#define s8					char
#endif //s8


#ifndef s16
#define s16					short int
#endif //s16

#ifndef s32
#define s32					int
#endif //s32


#ifndef __s8
#define __s8				char
#endif //__s8


#ifndef __s16
#define __s16				short int
#endif //__s16


#ifndef __s32
#define __s32				int
#endif //__s32

#ifndef int8
#define int8				char
#endif //int8

#ifndef int16
#define int16				short int
#endif //int16


#ifndef int32
#define int32				int
#endif //int32

//------------------------------------------------------------------------------------------------------------
//!�޷�������
//------------------------------------------------------------------------------------------------------------

#ifndef u8
#define u8					unsigned char
#endif //u8


#ifndef u16
#define u16					unsigned short 
#endif //u16

#ifndef u32
#define u32					unsigned int
#endif //u32


#ifndef __u8
#define __u8				unsigned char
#endif //__u8


#ifndef __u16
#define __u16				unsigned short 
#endif //__u16


#ifndef __u32
#define __u32				unsigned int
#endif //__u32



#ifndef uint8
#define uint8				unsigned char
#endif //uint8

#ifndef uint16
#define uint16				unsigned short int
#endif //uint16


#ifndef uint32
#define uint32				unsigned int
#endif //uint32


#endif  // #if 1



#ifndef SZ_1K
#define SZ_512       0x00000200U
#define SZ_1K        0x00000400U
#define SZ_2K        0x00000800U
#define SZ_4K        0x00001000U
#define SZ_8K        0x00002000U
#define SZ_16K       0x00004000U
#define SZ_32K       0x00008000U
#define SZ_64K       0x00010000U
#define SZ_128K      0x00020000U
#define SZ_256K      0x00040000U
#define SZ_512K      0x00080000U
#define SZ_1M        0x00100000U
#define SZ_2M        0x00200000U
#define SZ_4M        0x00400000U
#define SZ_8M        0x00800000U
#define SZ_16M       0x01000000U
#define SZ_32M       0x02000000U
#define SZ_64M       0x04000000U
#define SZ_128M      0x08000000U
#define SZ_256M      0x10000000U
#define SZ_512M      0x20000000U
#define SZ_1G        0x40000000U
#define SZ_2G        0x80000000U
#define SZ_4G        0x0100000000ULL
#define SZ_8G        0x0200000000ULL
#define SZ_16G       0x0400000000ULL
#define SZ_32G       0x0800000000ULL
#define SZ_64G       0x1000000000ULL
#endif  // ifndef SZ_1K




#undef  NULL
#define NULL                  0


#undef  RSVD_VALUE
#define RSVD_VALUE            0


#undef  FS_DIR_NAME_MAX_SIZE
#define FS_DIR_NAME_MAX_SIZE            260

#undef  FS_FILE_NAME_MAX_SIZE
#define FS_FILE_NAME_MAX_SIZE           511

#undef  FS_FULL_PATH_MAX_SIZE
#define FS_FULL_PATH_MAX_SIZE           ( 256 + FS_FILE_NAME_MAX_SIZE + 1 )


#undef  COMPILE_TIME_ASSERT
#define COMPILE_TIME_ASSERT(expr)       typedef char assert_type[(expr) ? 1 : -1]


#undef  set_bit
#define set_bit( x, y )      			( (x) |=  ( 1U << (y) ) )

#undef  clear_bit
#define clear_bit( x, y )    			( (x) &= ~( 1U << (y) ) )

#undef  reverse_bit
#define reverse_bit( x, y )  			( (x) ^=  ( 1U << (y) ) )

#undef  test_bit
#define test_bit( x, y )     			( (x)  &  ( 1U << (y) ) )


#undef  min
#define min( x, y )          			( (x) < (y) ? (x) : (y) )
                			
#undef  max                 			
#define max( x, y )          			( (x) > (y) ? (x) : (y) )

	
	
#undef  spcl_size_align
#define spcl_size_align( x, y )         ( ( (x) + (y) - 1 ) & ~( (y) - 1 ) )


#undef  _4_char_to_int32
#define _4_char_to_int32(a,b,c,d)       ( (a)<<24 | (b)<<16 | (c)<<8 | (d) )


#undef  MEMBER_OFFSET
#define MEMBER_OFFSET( type, mem )      ( (__u32) (&( ( (type *)0 )->mem ) ) )


#define ABS( x )                        ( (x) >0 ? (x) : -(x) )
	

#define WREG( x )                       (*(volatile unsigned int *)(x))



#endif     //  ifndef __type_def_h


/* end of type_def.h  */
