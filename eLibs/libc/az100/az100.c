/*
*********************************************************************************************************
*                                                    MELIS
*                                    the Easy Portable/Player Develop Kits
*                                          az100 compress libary
*
*                                    (c) Copyright 2011-2014, Sunny China
*                                             All Rights Reserved
*
* File    : az100.c
* By      : Sunny
* Version : v1.0
* Date    : 2011-4-11
* Descript: az100 compress libary.
* Update  : date                auther      ver     notes
*           2011-4-11 14:53:10  Sunny       1.0     Create this file.
*********************************************************************************************************
*/

#include  "az100_i.h"

/*
*********************************************************************************************************
*                                    CHECK DATA BUFFER IS COMPRESSED OR NOT
*
* Description: check a input buffer is use compressed by az100 or not.
*
* Arguments  : pBuffer  :  the pointer of input buffer,
*              Size     :  the size of input buffer.
*
* Returns    : return EPDK_TRUE if compressed by az100, return EPDK_FALSE if not.
*
* Note       :
*********************************************************************************************************
*/
__bool AZ100_IsCompress(void *pBuffer, __u32 Size)
{
    __u8  *pHeader;
    
    if (pBuffer == NULL || Size < AZ100_HEADER_LEN)
    {
        __wrn("invalid input argument for uncompress check\n");
        return EPDK_FALSE;
    }
    
    pHeader = (__u8 *)pBuffer;
    
    //check az100 magic valid or not
    if (eLIBs_strncmp((const char *)pHeader, AZ100_MAGIC, 4) == 0)
    {
        //valid az100 compressed data
        return EPDK_TRUE;
    }
    return EPDK_FALSE;
}

/*
*********************************************************************************************************
*                                       GET UNCOMPRESS SIZE
*
* Description: get the uncompress size of a az100 compressed buffer.
*
* Arguments  : pBuffer  :  the pointer of input buffer,
*              Size     :  the size of input buffer.
*
* Returns    : the size of uncompress buffer, 0 if failed.
*
* Note       :
*********************************************************************************************************
*/
__u32 AZ100_GetUncompressSize(void *pBuffer, __u32 Size)
{
    __u32  i;
    __u8  *pHeader;
    __u32  UncompressSize;
    
    if (pBuffer == NULL || Size < AZ100_HEADER_LEN)
    {
        __wrn("invalid input argument for uncompress size get\n");
        return 0;
    }
    
    //check az100 magic valid or not
    if (eLIBs_strncmp((const char *)pBuffer, AZ100_MAGIC, 4) != 0)
    {
        //valid az100 compressed data
        __wrn("invalid az100 compressed data for uncompress size get\n");
        return 0;
    }
    
    pHeader = (__u8 *)pBuffer;
    
    //should skip uncompress size : 4byte
    //by sunny at 2011-4-11 15:11:13.
    pHeader += AZ100_MAGIC_LEN;
    
    //get uncompress data size
    UncompressSize = 0;
    for (i = 0; i < 4; i++)
    {
        UncompressSize += ((__u32)pHeader[i] << (i * 8));
    }
    
    return UncompressSize;
}

/*
*********************************************************************************************************
*                                     UNCOMPRESS DATA BUFFER
*
* Description: uncompress a az100 compressed buffer.
*
* Arguments  : pSource      :  the pointer of source buffer,
*              SourceSize   :  the size of source buffer.
*              pDest        :  the pointer of output buffer,
*              DestSize     :  the size of output buffer.
*
* Returns    : EPDK_OK if succeeded, EPDK_FAIL if failed.
*
* Note       :
*********************************************************************************************************
*/
__s32 AZ100_Uncompress(void *pSource, __u32 SourceSize, void *pDest, __u32 DestSize)
{
    __u8  *pHeader;
    
    if (pSource == NULL || SourceSize < AZ100_HEADER_LEN || pDest == NULL)
    {
        __wrn("invalid input argument for data uncompress\n");
        return 0;
    }
    pHeader = (__u8 *)pSource;
    
    //get uncompress data size
    //should skip uncompress size and magic header,
    //by sunny at 2011-4-11 15:20:10.
    return AZ100_DataUncompress((__u8 *)pDest, DestSize, pHeader + 8, SourceSize);
}
