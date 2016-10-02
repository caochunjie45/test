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
* File   : dumpData.c
* Version: V1.0
* By     : Eric_wang
* Date   : 2012-3-31
* Description:
********************************************************************************
*/
#include "test_tsc_s.h"

#include "dumpDataV3.h"


/*******************************************************************************
Function name: WriteToFile
Description: 
    1.该函数,只写整数倍的WT_DATA_BLOCK_SIZE.最后再把剩下的写入.
    2.已在调用该函数前保证,要读的数据段不会回头.所以如果回头，就是fatal error!
    3.只负责读数据，写入文件，并操作nCurOffset,但不会管nDiskFullFlag的值的改变。
Parameters: 
    
Return: 
    写入的字节数
Time: 2012/4/27
*******************************************************************************/
__s32 WriteToFile(CDumpDataV3 *pDumpData, __s8 *pMem, __s32 nSize, FILE *pFile)
{
    __s32 i = 0;
    __s32 nBlockCnt = nSize/WT_DATA_BLOCK_SIZE;
    __s32 nLeftSize = nSize%WT_DATA_BLOCK_SIZE;
    __s32 nWtBytes = 0;
    __s8 *pWt = pMem;
    for(i = 0; i < nBlockCnt; i++)
    {
        nWtBytes = fwrite(pWt, 1, WT_DATA_BLOCK_SIZE, pFile);
        pWt += nWtBytes;
        if((pDumpData->nCurOffset + nWtBytes) > pDumpData->nDumpBufSize)
        {
            eLIBs_printf("fatal error! out of memory! check code! [%d][%d][%d]\n", pDumpData->nCurOffset, nWtBytes, pDumpData->nDumpBufSize);
            pDumpData->nCurOffset = 0;
        }
        else if((pDumpData->nCurOffset + nWtBytes) == pDumpData->nDumpBufSize)
        {
            pDumpData->nCurOffset = 0;
        }
        else
        {
            pDumpData->nCurOffset += nWtBytes;
        }
        if(nWtBytes < WT_DATA_BLOCK_SIZE)
        {
            eLIBs_printf("fatal error! disk is full! write [%d]bytes total!\n", i*WT_DATA_BLOCK_SIZE + nWtBytes);
            break;
        }
    }
    if(i < nBlockCnt)   //文件系统满了
    {
        pDumpData->nWtFSysBytes += (i*WT_DATA_BLOCK_SIZE + nWtBytes);
        return (i*WT_DATA_BLOCK_SIZE + nWtBytes);
    }
    nWtBytes = 0;
    if(nLeftSize)
    {
        nWtBytes = fwrite(pWt, 1, nLeftSize, pFile);
        if(nWtBytes < nLeftSize)
        {
            eLIBs_printf("fatal error! disk is full! write [%d]bytes total!\n", i*WT_DATA_BLOCK_SIZE + nWtBytes);
        }
        if((pDumpData->nCurOffset + nWtBytes) > pDumpData->nDumpBufSize)
        {
            eLIBs_printf("fatal error! out of memory! check code! [%d][%d][%d]\n", pDumpData->nCurOffset, nWtBytes, pDumpData->nDumpBufSize);
            pDumpData->nCurOffset = 0;
        }
        else if((pDumpData->nCurOffset + nWtBytes) == pDumpData->nDumpBufSize)
        {
            pDumpData->nCurOffset = 0;
        }
        else
        {
            pDumpData->nCurOffset += nWtBytes;
        }
    }
    pDumpData->nWtFSysBytes += (i*WT_DATA_BLOCK_SIZE + nWtBytes);
    return i*WT_DATA_BLOCK_SIZE + nWtBytes;
}

/*******************************************************************************
Function name: DumpDataWriteFSysTask
Description: 
    1.nRd == nWt,认为是没有数据.
Parameters: 
    
Return: 
    
Time: 2012/4/27
*******************************************************************************/
static void DumpDataWriteFSysTask(void *p_arg)
{
    CDumpDataV3 *pDumpData = (CDumpDataV3*)p_arg;
    __s32   nRd; //都是相对于DumpBuf而言.
    __s32   nWt;
    __s32   nValidLen1;
    __s32   nValidLen2;
    __s32   nWantedWtLen;
    __s32   nWtBytes;
    while(1)
    {
        if(esKRNL_TDelReq(EXEC_prioself) == OS_TASK_DEL_REQ)
        {
            MESSAGE("delete DumpDataWriteFSys Task!\n");
            goto _WriteFSysTaskExit;
        }
        if(pDumpData->nDiskFullFlag)
        {
            WARNING("Disk is Full! DiskFullFlag[%d], FullFlag[%d]\n", pDumpData->nDiskFullFlag, pDumpData->nFullFlag);
            esKRNL_TimeDly(20); //delay 200ms
            continue;
        }
        if(pDumpData->nFullFlag)
        {
            nRd = pDumpData->nCurOffset;
            nWt = pDumpData->nWtPos;
            if(nWt == nRd)
            {
                //WARNING("memory is Full! write to file done! don't write again!\n");
            }
            else if(nWt > nRd)  //没有回头
            {
                WARNING("memory is Full! write to file done! don't write again!\n");
                nWtBytes = WriteToFile(pDumpData, pDumpData->pDumpBuf + nRd, nWt - nRd, pDumpData->pFile);
                if(nWtBytes < nWt - nRd) //文件系统写满
                {
                    pDumpData->nDiskFullFlag = 1;
                }
            }
            else    //回头
            {
                WARNING("memory is Full! write to file done! don't write again!\n");
                nValidLen1 = pDumpData->nDumpBufSize - nRd;
                nValidLen2 = nWt;
                //先把nValidLen1写完再说
                nWtBytes = WriteToFile(pDumpData, pDumpData->pDumpBuf + nRd, nValidLen1, pDumpData->pFile);
                if(nWtBytes < nValidLen1) //文件系统写满
                {
                    pDumpData->nDiskFullFlag = 1;
                    continue;
                }
                //再写nValidLen2.
                if(nValidLen2 > 0)
                {
                    nWtBytes = WriteToFile(pDumpData, pDumpData->pDumpBuf, nValidLen2, pDumpData->pFile);
                    if(nWtBytes < nValidLen2) //文件系统写满
                    {
                        pDumpData->nDiskFullFlag = 1;
                    }
                }
            }
            esKRNL_TimeDly(20); //delay 200ms
            continue;
        }
        nRd = pDumpData->nCurOffset;
        nWt = pDumpData->nWtPos;
        if(nWt == nRd)
        {
            esKRNL_TimeDly(20); //delay 200ms再看.
            continue;
        }
        else if(nWt > nRd)   //没有回头
        {
            nValidLen1 = nWt - nRd;
            if(nValidLen1 >= WT_DATA_BLOCK_SIZE)    //只写整数倍的WT_DATA_BLOCK_SIZE
            {
                nWantedWtLen = nValidLen1 - nValidLen1%WT_DATA_BLOCK_SIZE;
                nWtBytes = WriteToFile(pDumpData, pDumpData->pDumpBuf + nRd, nWantedWtLen, pDumpData->pFile);
                if(nWtBytes < nWantedWtLen) //文件系统写满
                {
                    pDumpData->nDiskFullFlag = 1;
                }
            }
            else    //不够数据,不读,继续等
            {
                esKRNL_TimeDly(20); //delay 200ms再看.
            }
            continue;
        }
        else    //回头了
        {
            nValidLen1 = pDumpData->nDumpBufSize - nRd;
            nValidLen2 = nWt;
            //把nValidLen1写完就可以了.
            nWtBytes = WriteToFile(pDumpData, pDumpData->pDumpBuf + nRd, nValidLen1, pDumpData->pFile);
            if(nWtBytes < nValidLen1) //文件系统写满
            {
                pDumpData->nDiskFullFlag = 1;
            }
            continue;
        }
    }

_WriteFSysTaskExit:
    if(0 == pDumpData->nDiskFullFlag)
    {
        nRd = pDumpData->nCurOffset;
        nWt = pDumpData->nWtPos;
        if(nWt == nRd)
        {
        }
        else if(nWt > nRd)  //没有回头
        {
            nWtBytes = WriteToFile(pDumpData, pDumpData->pDumpBuf + nRd, nWt - nRd, pDumpData->pFile);
            if(nWtBytes < nWt - nRd) //文件系统写满
            {
                pDumpData->nDiskFullFlag = 1;
            }
        }
        else    //回头
        {
            nValidLen1 = pDumpData->nDumpBufSize - nRd;
            nValidLen2 = nWt;
            //先把nValidLen1写完再说
            nWtBytes = WriteToFile(pDumpData, pDumpData->pDumpBuf + nRd, nValidLen1, pDumpData->pFile);
            if(nWtBytes < nValidLen1) //文件系统写满
            {
                pDumpData->nDiskFullFlag = 1;
            }
            //再写nValidLen2.
            if(0==pDumpData->nDiskFullFlag && nValidLen2 > 0)
            {
                nWtBytes = WriteToFile(pDumpData, pDumpData->pDumpBuf, nValidLen2, pDumpData->pFile);
                if(nWtBytes < nValidLen2) //文件系统写满
                {
                    pDumpData->nDiskFullFlag = 1;
                }
            }
        }
    }
    WARNING("--------------------------- DumpDataWriteFSys Task end, Write to FSys [%d]bytes!---------------------------\n", pDumpData->nWtFSysBytes);
    esKRNL_TDel(EXEC_prioself);
    return;
}
/*******************************************************************************
Function name: Impl_CDumpDataV3_WriteData
Description: 
    1. 无锁结构,必须最多装载nDumpBufSize-1才能实现.
Parameters: 
    
Return: 
    
Time: 2012/4/27
*******************************************************************************/
__s32 Impl_CDumpDataV3_WriteData(CDumpDataV3 *thiz, __u8* pData, __s32 nDataSize)
{
    __s32   nRd = thiz->nCurOffset;
    __s32   nWt = thiz->nWtPos;
    __s32   nFreeSizeTotal;
    __s32   nFreeSize1;
    __s32   nFreeSize2;
    __s32   nWtSize1;
    __s32   nWtSize2;
    if(thiz->nFullFlag)
    {
        WARNING("memory Already full!\n");
        return EPDK_FAIL;
    }
    if(nRd == nWt)
    {
        if(nRd > 0)
        {
            nFreeSize1 = thiz->nDumpBufSize - nWt;
            nFreeSize2 = nRd - 1;
        }
        else
        {
            nFreeSize1 = thiz->nDumpBufSize - 1;
            nFreeSize2 = 0;
        }
    }
    else if(nWt > nRd)  //已写入DumpBuffer的数据没有回头
    {
        if(nRd > 0)
        {
            nFreeSize1 = thiz->nDumpBufSize - nWt;
            nFreeSize2 = nRd - 1;
        }
        else
        {
            nFreeSize1 = thiz->nDumpBufSize - nWt - 1;
            nFreeSize2 = 0;
        }
    }
    else    //已写入DumpBuffer的数据回头了,只要nWt回头就算回头,不考虑是否真写了
    {
        nFreeSize1 = nRd - nWt -  1;
        nFreeSize2 = 0;
    }
    nFreeSizeTotal = nFreeSize1 + nFreeSize2;

    if(nFreeSizeTotal < nDataSize)
    {
        WARNING("DumpBuffer will be full! don't write current data, return! [%d][%d],[%d]\n", nWt, nRd, thiz->nDumpBufSize);
        thiz->nFullFlag = 1;
        return EPDK_FAIL;
    }
    if(nFreeSize1 >= nDataSize)
    {
        nWtSize1 = nDataSize;
        nWtSize2 = 0;
    }
    else
    {
        nWtSize1 = nFreeSize1;
        nWtSize2 = nDataSize - nFreeSize1;
    }

    if(nWtSize1)
    {
        memcpy(thiz->pDumpBuf + nWt, pData, nWtSize1);
    }
    if(nWtSize2)
    {
        memcpy(thiz->pDumpBuf, pData+nWtSize1, nWtSize2);
    }

    if(thiz->nWtPos + nDataSize >= thiz->nDumpBufSize)
    {
        thiz->nWtPos = thiz->nWtPos + nDataSize - thiz->nDumpBufSize;
    }
    else
    {
        thiz->nWtPos += nDataSize;
    }
    return EPDK_OK;
}
__s32 Impl_CDumpDataV3_Reinit(CDumpDataV3 *thiz)
{
    thiz->nCurOffset = 0;
    thiz->nFullFlag = 0;
    thiz->nDiskFullFlag = 0;
    thiz->nWtFSysBytes = 0;
    memset(thiz->pDumpBuf, 0, thiz->nDumpBufSize);
    return EPDK_OK;
}


__s32 CDumpDataV3_Init(CDumpDataV3 *thiz, __s8 *pDumpBuf, __s32 nDumpBufSize, FILE *pFile)
{
    memset(thiz, 0, sizeof(CDumpDataV3));
    thiz->nDumpBufSize = nDumpBufSize;
    thiz->Reinit = Impl_CDumpDataV3_Reinit;
    thiz->WriteData = Impl_CDumpDataV3_WriteData;

    thiz->pDumpBuf = pDumpBuf;
    if(NULL == thiz->pDumpBuf)
    {
        WARNING("malloc dumpbuf[%d]bytes fail\n", nDumpBufSize);
        goto _err0;
    }
    memset(thiz->pDumpBuf, 0, nDumpBufSize);

    thiz->pFile = pFile;

    thiz->nWriteTaskPrio = esKRNL_TCreate(DumpDataWriteFSysTask,
                                                      (void *)thiz,
                                                      0x400,
                                                      KRNL_priolevel3);
    if(!thiz->nWriteTaskPrio)
    {
        WARNING("Create nWriteTaskPrio fail! quit!\n");
        goto _err0;
    }
    return EPDK_OK;
    
_err0:
    return EPDK_FAIL;

}
__s32 CDumpDataV3_Exit(CDumpDataV3 *thiz)
{
//    if(thiz->pDumpBuf)
//    {
//        free(thiz->pDumpBuf);
//        thiz->pDumpBuf = NULL;
//    }
    if(thiz->nWriteTaskPrio)
    {
        //wake up audio decoder main task, and wait it exit
        esKRNL_TimeDlyResume(thiz->nWriteTaskPrio);
        while(esKRNL_TDelReq(thiz->nWriteTaskPrio) != OS_TASK_NOT_EXIST)
        {
            esKRNL_TimeDlyResume(thiz->nWriteTaskPrio);
            esKRNL_TimeDly(1);
        }
        thiz->nWriteTaskPrio = 0;
    }
    return EPDK_OK;
}
CDumpDataV3* newCDumpDataV3(__s8 *pDumpBuf, __s32 nDumpBufSize, FILE *pFile)
{
    __s32   ret;
    CDumpDataV3 *pDumpData = malloc(sizeof(CDumpDataV3));
    if(NULL == pDumpData)
    {
        WARNING("malloc CDumpDataV3 fail!\n");
        return NULL;
    }
    ret = CDumpDataV3_Init(pDumpData, pDumpBuf, nDumpBufSize, pFile);
    if(EPDK_OK != ret)
    {
        goto _err0;
    }
    return pDumpData;
    
_err0:
    free(pDumpData);
    return NULL;
}

void deleteCDumpDataV3(CDumpDataV3 *thiz)
{
    __s32   ret;
    CDumpDataV3 *pDumpData = (CDumpDataV3*)thiz;
    ret = CDumpDataV3_Exit(pDumpData);
    if(EPDK_OK!=ret)
    {
    }
    free(pDumpData);
}

