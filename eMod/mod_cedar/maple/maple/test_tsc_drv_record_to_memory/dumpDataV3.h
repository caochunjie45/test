/*
********************************************************************************
*                                    eMOD
*                   the Easy Portable/Player Develop Kits
*                               mod_cedar sub-system
*                          (module name, e.g.mpeg4 decoder plug-in) module
*
*          (c) Copyright 2010-2012, Allwinner Microelectronic Co., Ltd.
*                              All Rights Reserved
*
* File   : dumpData.h
* Version: V3.0
* By     : Eric_wang
* Date   : 2012-4-27
* Description:
    1.一个dump数据的测试类.第二个版本.这个版本的特点是写buffer在外面分配好，传进来.
    但CDumpData类的数据结构是一样的.

    2.实际的DumpBuf利用率为nDumpBufSize-1.这是要保证无锁结构所需付出的代价.
********************************************************************************
*/
#ifndef _DUMPDATAV3_H_
#define _DUMPDATAV3_H_

//#define WT_DATA_BLOCK_SIZE (2*1024*1024)
#define WT_DATA_BLOCK_SIZE (128*1024)


typedef struct tag_CDumpDataV3 CDumpDataV3;
typedef __s32 (*CDumpDataV3_WriteData)(CDumpDataV3 *thiz, __u8* pData, __s32 nDataSize);    //ret = EPDK_OK, EPDK_FAIL
typedef __s32 (*CDumpDataV3_Reinit)(CDumpDataV3 *thiz);    //ret = EPDK_OK, EPDK_FAIL
typedef struct tag_CDumpDataV3
{
    __s8    *pDumpBuf;
    __s32   nCurOffset; //读Dumpbuf已经读到的位置,这个位置将要被读.读出来的数据可能再给写到sd卡中.
    __s32   nWtPos;     //指向写DumpBuf已经写到的位置.这个位置将要被写.
    __s32   nDumpBufSize;
    __s32   nFullFlag;  //当写不下时,标记位置1.以后就不再写.以保证已有数据的完整. 另一个线程写, DumpDataWriteFSysTask()读

    FILE    *pFile;     //文件指针,传入时已经打开,给CDumpDataV3去写.
    __s32   nDiskFullFlag;  //当文件系统写满写不下时,标记位置1.以后就不再写., DumpDataWriteFSysTask()可以写,另一个线程只能读.
    __s32   nWtFSysBytes;   //记录写入文件的字节数.
    

    __u8    nWriteTaskPrio;      // 写文件系统的线程,优先级暂定为level3.

    CDumpDataV3_Reinit    Reinit;
    CDumpDataV3_WriteData WriteData;
}CDumpDataV3;
extern __s32 CDumpDataV3_Init(CDumpDataV3 *thiz, __s8 *pDumpBuf, __s32 nDumpBufSize, FILE *pFile);
extern __s32 CDumpDataV3_Exit(CDumpDataV3 *thiz);
extern CDumpDataV3* newCDumpDataV3(__s8 *pDumpBuf, __s32 nDumpBufSize, FILE *pFile);
extern void deleteCDumpDataV3(CDumpDataV3 *thiz);

#endif  /* _DUMPDATAV3_H_ */

