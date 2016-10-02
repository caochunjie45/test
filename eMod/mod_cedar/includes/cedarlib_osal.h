/*
********************************************************************************
*                                    eMOD
*                   the Easy Portable/Player Develop Kits
*                               mod_duckweed sub-system
*                          (module name, e.g.mpeg4 decoder plug-in) module
*
*          (c) Copyright 2010-2012, Allwinner Microelectronic Co., Ltd.
*                              All Rights Reserved
*
* File   : cedarlib_osal.h
* Version: V1.0
* By     : Eric_wang
* Date   : 2010-10-26
* Description:
    cedarlib所需内存操作、文件操作、字符操作、系统底层操作、打印操作等，由file_depack lib的
    使用者来实现.
        为简便考虑，这个文件包含了filedepacklib, vdeclib, adeclib等各种库
    所需的基础函数的定义。
    [cedarlib]
********************************************************************************
*/
#ifndef _CEDARLIB_OSAL_H_
#define _CEDARLIB_OSAL_H_

//==============================================================================
//                              MEMORY MANAGEMENT MODULE
//==============================================================================
extern void*    cedarlib_malloc(int size);
extern int      cedarlib_free(void *pbuf);
extern void*    cedarlib_phymalloc(int size, int align);   //phymalloc()要求特殊处理，和cedarlib_InitPhyHeap()配合，记录malloc的块数，在cedarlib_phyfree()时不需填写块数
extern int      cedarlib_phyfree(void *pbuf);
extern void     cedarlib_memset(void *buf, unsigned char val, unsigned int size);
extern void*    cedarlib_memcpy(void *dst, const void *src, unsigned int size);

//==============================================================================
//                              FILE SYSTEM ADAPT MODULE
//特别说明:FILE这个数据类型，各个系统定义不一样。cedarlib内部只使用其指针，所以
//会在cedarlib内部定义一下FILE的类型, 目的仅仅是让编译通过。
//==============================================================================
#define CEDARLIB_SEEK_SET 0 /* SEEK_SET, start of stream (see fseek) */
#define CEDARLIB_SEEK_CUR 1 /* SEEK_CUR, current position in stream (see fseek) */
#define CEDARLIB_SEEK_END 2 /* SEEK_END, end of stream (see fseek) */
extern FILE*        cedarlib_fopen(const char * filename, const char * mode);
extern int          cedarlib_fclose(FILE *fp);
extern int          cedarlib_fread(void *buf, int size, int count, FILE *fp);
extern int          cedarlib_fwrite(const void *buf, int size, int count, FILE *fp);
extern int          cedarlib_fseek(FILE *fp, long long offset, int origin);
extern long long    cedarlib_ftell(FILE *fp);

//==============================================================================
//                              string operation MODULE
//==============================================================================
extern int              cedarlib_toupper(int c);
extern unsigned short   cedarlib_strncmp(const char *dst, const char *src, unsigned int size);
extern unsigned short   cedarlib_strcmp(const char *s1, const char *s2);
extern unsigned int     cedarlib_strlen(const char *s);
extern char*            cedarlib_strncpy(char *s1, const char *s2, unsigned int n);
extern char*            cedarlib_strcpy(char *s1, const char *s2);
extern char*            cedarlib_strchr(char *s, char ch); 
extern char*            cedarlib_strstr(char *s1, char *s2);
extern char*            cedarlib_strcat(char *s1, const char *s2);

//==============================================================================
//                              SYSTEM OPERATION
//==============================================================================
extern unsigned int cedarlib_disable_interupt(void);
extern void         cedarlib_enable_interupt(unsigned int cpu_sr);
extern void         cedarlib_disable_task_switch(void);
extern void         cedarlib_enable_task_switch(void);
extern void*        cedarlib_VirAddr2PhyAddr(void* addr_v);      //虚地址转物理地址
extern void         cedarlib_timedelay(int nDelay);   //单位为ms
extern void         cedarlib_reset(void);        //SYSCALL_KSRV(void , esKSRV_Reset       )(void);

//==============================================================================
//                              打印操作和dump内存等
//==============================================================================
extern int          cedarlib_printf(const char * format, ...);
extern void         cedarlib_dumpmem(FILE *pdes, const char *pmem, int len);

//==============================================================================
//                              其他特殊的操作等
//==============================================================================
#endif  /* _CEDARLIB_OSAL_H_ */

