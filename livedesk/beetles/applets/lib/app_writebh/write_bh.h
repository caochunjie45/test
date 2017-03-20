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
    __u8 WREN;      /* ��дenable */
    __u8 WRDI;      /* ��дdisable */
    __u8 RDSR;      /* Read status register */
    __u8 WRSR;      /* Write status register */
    __u8 READ;      /* Read */
    __u8 FAST_READ; /* Fast read */
    __u8 PP;        /* ҳд����, page program */
    __u8 SE;        /* Sector erase */
    __u8 BE;        /* Block Erase */
} __spinor_cmd_set_t;

typedef struct __SPINOR_INFO
{
    char                    factory[8];   /* �������� */
    char                    pattern[24];  /* spinor�ͺ� */
    __u32                   interface;    /* spinor�ӿ� */
    __u32                   RDID_ID;      /* ��ӦRDID��mid, did */
    __u32                   REMS_ID;      /* ��ӦREMS��mid */
    __spinor_store_struct_t store_struct; /* �洢�ܹ�����Ϊ 0��top, 1:bottom, 2:Uniform */
    __u32                   page_size;    /* ҳ���С������256bytes�� 264bytes */
    __u32                   capaticy;     /* �ܵ�������С����λMbit */
	__u32                   freq_read;    /* Clock frequency for READ instruction */
	__u32                   freq;         /* Clock frequency for all instruction, except READ */
	__s32                   cmd_index;    /* ��������� */
} __spinor_info_t;



__s32 spinor_wrsr(__u8 value1,__u8 value2);
__s32 DISK_Clear(void);
__s32 spinor_bulk_eraser(void);
//__s32 spinor_init(void);
//__s32 spinor_exit(void);
#endif