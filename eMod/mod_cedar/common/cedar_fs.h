/*
*********************************************************************************************************
*                                                    eMOD
*                                   the Easy Portable/Player Operation System
*                                              mod_mmp sub-system
*
*                                    (c) Copyright 2008-2009, Kevin.Z China
*                                              All Rights Reserved
*
* File   : cedar_fs.h
* Version: V1.0
* By     : kevin.z
* Date   : 2009-2-2 8:37
    cedar体系的文件操作的API.
    cedar内部所有模块都是用这个API操作文件，不会直接调用系统的fopen等函数。
    
    强制规定:
    如果想使用cedar_fs.h提供的函数，必须在模块打开时先调用
    CEDAR_InitHeap();
    CEDAR_InitPhyHeap();
    初始化cedar_common的内存分配链.因为cedar_fs的函数默认使用cedar_common提供的
    内存操作函数.
*********************************************************************************************************
*/
#ifndef _CEDAR_FS_H_
#define _CEDAR_FS_H_

//#include "cedar_malloc.h"
//#include "mod_cedar_i.h"

/* cedar fs read mode */
//#define CEDAR_FS_WORK_MODE_BYTEALIGN            0   // read align by byte
//#define CEDAR_FS_WORK_MODE_SECTALIGN            1   // read align by sector
//#define CEDAR_FS_WORK_MODE_USER_DEFINED         2   // user defined read,目前默认no cache
//#define CEDAR_FS_WORK_MODE_NO_CACHE             3   // use fread of fs，no cache
typedef enum tag_CEDAR_FS_WORK_MODE
{
    CEDAR_FS_WORK_MODE_BYTEALIGN    = 0,   // read align by byte
    CEDAR_FS_WORK_MODE_SECTALIGN    = 1,   // read align by sector
    CEDAR_FS_WORK_MODE_USER_DEFINED = 2,   // user defined read,用户自己处理如果读取文件.目前默认no cache,解决不是真正存在于硬盘上的文件的类型，比如从内存读取等，不适用于文件操作的文件。对于这类文件,不要用fopen()等函数
    CEDAR_FS_WORK_MODE_NO_CACHE     = 3,   // use fread of fs，no cache
    CEDAR_FS_WORK_MODE_HERBCACHE  = 4,    //内部起线程,主要用于录制写.
}CedarFSWorkMode;

//extern __s32    cedar_fs_work_mode;    // cedar fs work mode
//extern __s32    CEDAR_FS_CACHE_SIZE;
//extern __s32    CEDAR_FS_CACHE_BITS;

extern FILE     *CEDAR_fopen(const char *path, const char *mode);
extern int      CEDAR_fclose(FILE *fp);
extern int      CEDAR_fread(void *buf, int size, int count, FILE *fp);
extern int      CEDAR_fwrite(void *buf, int size, int count, FILE *fp);
extern int      CEDAR_fseek(FILE *fp, __s64 offset, int origin);
extern __s64    CEDAR_ftell(FILE *fp);
extern int      CEDAR_FIOCTRL(FILE *fp, __s32 Cmd, __s32 Aux, void *pBuffer);
extern __u32    CEDAR_fsize(FILE *fp);      //计算filesize的函数

//以下函数配置cedarfs的属性，必须在newCedarFs之前(即调用CEDAR_fopen之前)全部配置好。创建CedarFS之后，再调用这3个函数就无用了
extern __s32    CEDAR_fs_set_workmod(__s32 mode);
extern __s32    CEDAR_fs_get_workmod(void);
extern int      CEDAR_fs_SetCacheSize(int cache_size,int mode);  //适用于mode = CEDAR_FS_WORK_MODE_SECTALIGN等有cache的模式
extern void     CEDAR_fs_set_mid(__u32 mid); //因为fs buffer mode要开线程， epdk系统下，开线程得给一个module id.
//如果是USRDEF模式，再配置__cedar_usr_file_op_t
extern void     CEDAR_ufinit(void *fop);    //仅用于CEDAR_FS_WORK_MODE_USER_DEFINED这种模式，设置好必要的数据结构fop = __cedar_usr_file_op_t g_cedar_usr_fop;在打开文件时使用

#endif //_CEDAR_FS_H_

