/*
*********************************************************************************************************
*                                                    ePOS
*                                   the Easy Portable/Player Operation System
*                                               mems sub-system
*
*                          (c) Copyright 1992-2006, Jean J. Labrosse, Weston, FL
*                                             All Rights Reserved
*
* File    : mems.h
* By      : Steven.ZGJ
* Version : V1.00
*********************************************************************************************************
*/
#ifndef _MEMS_H_
#define _MEMS_H_

#include "typedef.h"
#include "cfgs.h"

#ifdef EPDK_USED_BY_KRNL
#define __swi(x)
#endif
//**********************************************************************************************************
//* define level 0( system level)
//************************************************
// ��������

#define MEMS_normaldomain   0
#define MEMS_domain_user    0x0f
#define MEMS_domain_system  0x0f

// �����ڴ�͵�ַ�Ķ��뷽ʽ,1k/2k/4k/8k/16k/32k...
#define MEMS_PALLOC_MODE_BND_NONE   0x00
#define MEMS_PALLOC_MODE_BND_2K     0x01
#define MEMS_PALLOC_MODE_BND_4K     0x02
#define MEMS_PALLOC_MODE_BND_8K     0x03
#define MEMS_PALLOC_MODE_BND_16K    0x04
#define MEMS_PALLOC_MODE_BND_32K    0x05
#define MEMS_PALLOC_MODE_BND_MSK    0x0f

// ��������ڴ��ı߽����ԣ�Ŀ���ֻ���ڶ���
//���ڴ���ڷ���,2M/4M/8M/16M/32M/64M...
#define MEMS_PALLOC_MODE_BNK_NONE   0x00
#define MEMS_PALLOC_MODE_BNK_2M     0x10
#define MEMS_PALLOC_MODE_BNK_4M     0x20
#define MEMS_PALLOC_MODE_BNK_8M     0x30
#define MEMS_PALLOC_MODE_BNK_16M    0x40
#define MEMS_PALLOC_MODE_BNK_32M    0x50
#define MEMS_PALLOC_MODE_BNK_64M    0x60
#define MEMS_PALLOC_MODE_BNK_128M   0x70
#define MEMS_PALLOC_MODE_BNK_256M   0x80
#define MEMS_PALLOC_MODE_BNK_512M   0x90
#define MEMS_PALLOC_MODE_BNK_1G     0xa0
#define MEMS_PALLOC_MODE_BNK_2G     0xb0
#define MEMS_PALLOC_MODE_BNK_4G     0xc0
#define MEMS_PALLOC_MODE_BNK_8G     0xd0
#define MEMS_PALLOC_MODE_BNK_16G    0xe0
#define MEMS_PALLOC_MODE_BNK_32G    0xf0
#define MEMS_PALLOC_MODE_BNK_MSK    0xf0

// ��������ڴ���λ�����ԣ�Ŀ�������ڸõ�ַ��
// ��Χ�ڽ��з���,2M/4M/8M/16M/32M/64M...
#define MEMS_PALLOC_MODE_AREA_NONE  0x0000
#define MEMS_PALLOC_MODE_AREA_2M    0x0100
#define MEMS_PALLOC_MODE_AREA_4M    0x0200
#define MEMS_PALLOC_MODE_AREA_8M    0x0300
#define MEMS_PALLOC_MODE_AREA_16M   0x0400
#define MEMS_PALLOC_MODE_AREA_32M   0x0500
#define MEMS_PALLOC_MODE_AREA_64M   0x0500
#define MEMS_PALLOC_MODE_AREA_128M  0x0600
#define MEMS_PALLOC_MODE_AREA_256M  0x0700
#define MEMS_PALLOC_MODE_AREA_MSK   0x0f00


// ����DRAM��size����DRAM����ʵ��ɨ��õ�
extern __epos_para_t KRNLPara;
#define DRAM_SIZE       (KRNLPara.DramPara.dram_size<<20)


// ϵͳ��λ��SDRAM�±߽�����,ϵͳջλ��SDRAM���߽�(0xc0000000-0xc1ffffff)����
// ϵͳ�Ѻ�ջ����Ҫͨ���ڴ����ģ�鴴�����
// ϵͳ�Ѻ�ջ��С���Զ�̬�仯
#define SYSHEAP_INIT_SIZE   (1*1024*1024)
#define SYSHEAP_MAX_SIZE    (4*1024*1024)
#define esSysheap           ((__mems_heap_t *)(DRAM_vBASE + DRAM_SIZE))
#define esVmheap            ((__u32)esSysheap + SYSHEAP_MAX_SIZE)
#define esSysstack          DRAM_vBASE

// ϵͳTLB�����ַ
#define esTLBp          ((__u32 *)DRAM_pBASE)
// ϵͳTLB�����ַ
#define esTLBv          ((__u32 *)DRAM_vBASE)

#define MEMS_maxfreelist    6
#define MEMS_nulllistmask   31
#define MEMS_errlist        ((__mems_mblk_t *)~31)

//************************************************
// �������Ͷ���
typedef struct __MEMS_MBLK
{
    struct __MEMS_MBLK  *next;      //next free block
    __u32                len;       //current block size(bytes)/��balloc����λ�ý��������freeblock��
} __mems_mblk_t;

typedef struct __MEMS_PAGE
{
    __u8    byte[1024];
}__mems_page_t;

typedef struct __MEMS_PGPOOL
{
    //��ǰ�����ĵط��������������ָ������ռ�ʱ����ʼ����ҳ��ÿ�η��䶼�Ǵ����λ�ÿ�ʼ�����ҵ���β���ٴ�ͷ��ʼ��
    __u32       startindex;     //start find index in the bitmap table
    __u32       tblsize;        //table size

    __u32       npage;          //�˳���һ���ж���ҳ
    __u32       nfree;          //free pages

    __s32       page_idx[8];    //1M/2M/4M/...����ҳ����ϵͳҳ���е�������, �����ֵΪ-1,��ʾҳ����û�д�ҳ

    __u32       *bmptbl_v;      //ֻ��bitmaptable�ĵ�ַ,������bitΪ1����ʾ����
    __mems_page_t       *pool_v;//ָʾҳ�ص������ַ
       __mems_page_t    *pool_p;//ҳ�ص������ַ
}__mems_pgpool_t;

typedef struct __MEMS_HEAP
{
    __mems_mblk_t   *free;      //free blocks link
    __u32           size;       //heap size
//    __u8            heap[size];
} __mems_heap_t;


typedef struct __MEMS_BLKGRP
{
    struct __MEMS_BLKGRP    *next;      //ָ����һ��group�ĵ�ַ, ���ڱ�̷��㣬�˵�һ������ڿ�ʼ��λ��
    __u16                   nfree;      //��ǰgroup��free��block��
    __u16                   nlistfree;  //��grp�ܹ����ж��ٸ�freelistû�йҽ�1page����
    __mems_mblk_t           *freelist[MEMS_maxfreelist];    //ǰ27λΪ��ַ�� ��5λΪfree counter
} __mems_blkgrp_t;          //32byteһ���� һ��group����Թ������block��С���ܺ�Ϊ6Kbytes�������󳬹���
                            //��Ҫ����������µ�group����ͬ��Сblock��group������Ϊһ��group list
                            //�Ժ���������Ҫ��Ҫ��������ݽṹ��Ҳֻ����64/128/256/512/...
#define VHEAP_BMP_SIZE  (16)
typedef struct __VIRTUAL_HEAP
{
    __mems_page_t   *head;      //����ѵ���ʼ��ַ

    __u32           npage;      //�������ҳ��������ѵ�һ��ҳռ1M�ռ�
    __u32           nfree;      //����ѵĿ���ҳ��

    __u32           bmptbl[VHEAP_BMP_SIZE];     //�����ҳ���״̬ͼ����������Ϊ32*16M=512Mbyte

} __virtual_heap_t;


typedef struct __MEMS_INFO
{
    __u32   pagepool_npage;
    __u32   pagepool_nfree;
    __u32   sysheap_size;
    __u32   sysheap_free;
} __mems_info_t;

//************************************************
// ��������
/* GLOBAL VARIABLES */

//************************************************
// ��������
/* system call table */
typedef struct
{
     __pSWI_t esMEMS_Palloc             ;
     __pSWI_t esMEMS_Pfree              ;
     __pSWI_t esMEMS_VA2PA              ;
     __pSWI_t esMEMS_VMCreate           ;
     __pSWI_t esMEMS_VMDelete           ;
     __pSWI_t esMEMS_HeapCreate         ;
     __pSWI_t esMEMS_HeapDel            ;
     __pSWI_t esMEMS_Malloc             ;
     __pSWI_t esMEMS_Mfree              ;
     __pSWI_t esMEMS_Realloc            ;
     __pSWI_t esMEMS_Calloc             ;
     __pSWI_t esMEMS_Balloc             ;
     __pSWI_t esMEMS_Bfree              ;
     __pSWI_t esMEMS_Info               ;

     __pSWI_t esMEMS_CleanDCache        ;
     __pSWI_t esMEMS_CleanFlushDCache   ;
     __pSWI_t esMEMS_CleanFlushCache    ;
     __pSWI_t esMEMS_FlushDCache        ;
     __pSWI_t esMEMS_FlushICache        ;
     __pSWI_t esMEMS_FlushCache         ;
     __pSWI_t esMEMS_CleanDCacheRegion  ;
     __pSWI_t esMEMS_CleanFlushDCacheRegion ;
     __pSWI_t esMEMS_CleanFlushCacheRegion  ;
     __pSWI_t esMEMS_FlushDCacheRegion  ;
     __pSWI_t esMEMS_FlushICacheRegion  ;
     __pSWI_t esMEMS_FlushCacheRegion   ;

     __pSWI_t esMEMS_VMalloc            ;
     __pSWI_t esMEMS_VMfree             ;
     __pSWI_t esMEMS_FreeMemSize        ;
     __pSWI_t esMEMS_TotalMemSize       ;

     __pSWI_t esMEMS_LockICache         ;
     __pSWI_t esMEMS_LockDCache         ;
     __pSWI_t esMEMS_UnlockICache       ;
     __pSWI_t esMEMS_UnlockDCache       ;
     __pSWI_t esMEMS_GetIoVAByPA        ;
     __pSWI_t esMEMS_PhyAddrConti       ;

} SWIHandler_MEMS_t;

#ifndef SIM_PC_WIN
	#define SYSCALL_MEMS(x,y) __swi(SWINO(SWINO_MEMS, SWIHandler_MEMS_t, y))x y
#else
    #define SYSCALL_MEMS(x,y) x y
#endif

/* system pages management */
SYSCALL_MEMS(void*, esMEMS_Palloc   )(__u32 /*npage*/, __u32 /*mode*/);
SYSCALL_MEMS(void,  esMEMS_Pfree    )(void */*pblk*/, __u32 /*npage*/);
SYSCALL_MEMS(void*, esMEMS_VA2PA    )(void */*addr_v*/);
SYSCALL_MEMS(__s32, esMEMS_VMCreate )(__mems_page_t */*pblk*/, __u32 /*size*/, __u8 /*domain*/);
SYSCALL_MEMS(void,  esMEMS_VMDelete )(__mems_page_t */*pblk*/, __u32 /*npage*/);
/* system malloc/free management */
SYSCALL_MEMS(__s32, esMEMS_HeapCreate)(void * /*heapaddr*/, __u32 /*initnpage*/);
SYSCALL_MEMS(void,  esMEMS_HeapDel  )(__mems_heap_t * /*heap*/);
SYSCALL_MEMS(void*, esMEMS_Malloc   )(__mems_heap_t *heap, __u32 size);
SYSCALL_MEMS(void,  esMEMS_Mfree    )(__mems_heap_t *heap, void *f);
SYSCALL_MEMS(void*, esMEMS_Realloc  )(__mems_heap_t *heap, void *f, __u32 size);
SYSCALL_MEMS(void*, esMEMS_Calloc   )(__mems_heap_t *heap, __u32 n, __u32 m);
/* system bmalloc/bfree management */
SYSCALL_MEMS(void*, esMEMS_Balloc   )(__u32 size);
SYSCALL_MEMS(void,  esMEMS_Bfree    )(void *mblk, __u32 size);
SYSCALL_MEMS(__s32, esMEMS_Info     )(void);
/*cache operations*/
SYSCALL_MEMS(void,  esMEMS_CleanDCache)(void);
SYSCALL_MEMS(void,  esMEMS_CleanFlushDCache)(void);
SYSCALL_MEMS(void,  esMEMS_CleanFlushCache )(void);
SYSCALL_MEMS(void,  esMEMS_FlushDCache)(void);
SYSCALL_MEMS(void,  esMEMS_FlushICache)(void);
SYSCALL_MEMS(void,  esMEMS_FlushCache )(void);
SYSCALL_MEMS(void,  esMEMS_CleanDCacheRegion)(void *adr, __u32 bytes);
SYSCALL_MEMS(void,  esMEMS_CleanFlushDCacheRegion)(void *adr, __u32 bytes);
SYSCALL_MEMS(void,  esMEMS_CleanFlushCacheRegion )(void *adr, __u32 bytes);
SYSCALL_MEMS(void,  esMEMS_FlushDCacheRegion)(void *adr, __u32 bytes);
SYSCALL_MEMS(void,  esMEMS_FlushICacheRegion)(void *adr, __u32 bytes);
SYSCALL_MEMS(void,  esMEMS_FlushCacheRegion )(void *adr, __u32 bytes);

SYSCALL_MEMS(void*, esMEMS_VMalloc    )(__u32 size);
SYSCALL_MEMS(void,  esMEMS_VMfree     )(void *ptr);
SYSCALL_MEMS(__u32, esMEMS_FreeMemSize)(void);
SYSCALL_MEMS(__u32, esMEMS_TotalMemSize)(void);
SYSCALL_MEMS(__s32, esMEMS_PhyAddrConti)(void *mem, __u32 size);
SYSCALL_MEMS(__u32, esMEMS_GetIoVAByPA )(__u32 phyaddr, __u32 size);

SYSCALL_MEMS(__s32, esMEMS_LockICache  )(void *addr, __u32 size);
SYSCALL_MEMS(__s32, esMEMS_UnlockICache)(void *addr);
SYSCALL_MEMS(__s32, esMEMS_LockDCache  )(void *addr, __u32 size);
SYSCALL_MEMS(__s32, esMEMS_UnlockDCache)(void *addr);


void esMEMS_MMUFlushTLB(void);

//**********************************************************************************************************

//**********************************************************************************************************
//* define level 1( system level)
//************************************************
// ��������

//************************************************
// �������Ͷ���

//************************************************
// ��������
extern SWIHandler_MEMS_t SWIHandler_MEMS;
//************************************************
// ��������
__s32 MEMS_Init  (void);
__s32 MEMS_Exit  (void);
__s32 MEMS_PagePoolInfo(void);
void  esMEMS_set_fcse(__u8 id);
__u8  esMEMS_get_fcse(void);


//**********************************************************************************************************

#endif  /* _KRNL_H_ */

