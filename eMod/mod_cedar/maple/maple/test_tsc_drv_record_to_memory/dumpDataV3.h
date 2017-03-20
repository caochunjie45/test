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
    1.һ��dump���ݵĲ�����.�ڶ����汾.����汾���ص���дbuffer���������ã�������.
    ��CDumpData������ݽṹ��һ����.

    2.ʵ�ʵ�DumpBuf������ΪnDumpBufSize-1.����Ҫ��֤�����ṹ���踶���Ĵ���.
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
    __s32   nCurOffset; //��Dumpbuf�Ѿ�������λ��,���λ�ý�Ҫ����.�����������ݿ����ٸ�д��sd����.
    __s32   nWtPos;     //ָ��дDumpBuf�Ѿ�д����λ��.���λ�ý�Ҫ��д.
    __s32   nDumpBufSize;
    __s32   nFullFlag;  //��д����ʱ,���λ��1.�Ժ�Ͳ���д.�Ա�֤�������ݵ�����. ��һ���߳�д, DumpDataWriteFSysTask()��

    FILE    *pFile;     //�ļ�ָ��,����ʱ�Ѿ���,��CDumpDataV3ȥд.
    __s32   nDiskFullFlag;  //���ļ�ϵͳд��д����ʱ,���λ��1.�Ժ�Ͳ���д., DumpDataWriteFSysTask()����д,��һ���߳�ֻ�ܶ�.
    __s32   nWtFSysBytes;   //��¼д���ļ����ֽ���.
    

    __u8    nWriteTaskPrio;      // д�ļ�ϵͳ���߳�,���ȼ��ݶ�Ϊlevel3.

    CDumpDataV3_Reinit    Reinit;
    CDumpDataV3_WriteData WriteData;
}CDumpDataV3;
extern __s32 CDumpDataV3_Init(CDumpDataV3 *thiz, __s8 *pDumpBuf, __s32 nDumpBufSize, FILE *pFile);
extern __s32 CDumpDataV3_Exit(CDumpDataV3 *thiz);
extern CDumpDataV3* newCDumpDataV3(__s8 *pDumpBuf, __s32 nDumpBufSize, FILE *pFile);
extern void deleteCDumpDataV3(CDumpDataV3 *thiz);

#endif  /* _DUMPDATAV3_H_ */

