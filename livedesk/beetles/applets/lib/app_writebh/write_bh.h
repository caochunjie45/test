#ifndef __WRITE_BH_H__
#define __WRITE_BH_H__

#include "beetles_app.h"


typedef enum __SPINOR_STORE_STRUCT
{
    SPINOR_TOP     = 0,
    SPINOR_BOTTOM  = 1,
    SPINOR_UNIFORM = 2
} __spinor_store_struct_t;


typedef struct __SPINOR_CMD_SET
{
    __u8 WREN;      /* 读写enable */
    __u8 WRDI;      /* 读写disable */
    __u8 RDSR;      /* Read status register */
    __u8 WRSR;      /* Write status register */
    __u8 READ;      /* Read */
    __u8 FAST_READ; /* Fast read */
    __u8 PP;        /* 页写功能, page program */
    __u8 SE;        /* Sector erase */
    __u8 BE;        /* Block Erase */
} __spinor_cmd_set_t;

typedef struct __SPINOR_INFO
{
    char                    factory[8];   /* 厂家名称 */
    char                    pattern[24];  /* spinor型号 */
    __u32                   interface;    /* spinor接口 */
    __u32                   RDID_ID;      /* 对应RDID的mid, did */
    __u32                   REMS_ID;      /* 对应REMS的mid */
    __spinor_store_struct_t store_struct; /* 存储架构，分为 0：top, 1:bottom, 2:Uniform */
    __u32                   page_size;    /* 页面大小，包括256bytes， 264bytes */
    __u32                   capaticy;     /* 总的容量大小，单位Mbit */
	__u32                   freq_read;    /* Clock frequency for READ instruction */
	__u32                   freq;         /* Clock frequency for all instruction, except READ */
	__s32                   cmd_index;    /* 命令集索引号 */
} __spinor_info_t;



__s32 spinor_wrsr(__u8 value1,__u8 value2);
__s32 DISK_Clear(void);
__s32 spinor_bulk_eraser(void);
//__s32 spinor_init(void);
//__s32 spinor_exit(void);
#endif