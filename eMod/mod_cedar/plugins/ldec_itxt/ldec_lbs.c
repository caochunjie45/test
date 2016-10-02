
#include "ldec_itxt_s.h"
#include "ldec_lbs.h"

/*
********************************************************************************
*                                    eMOD
*                   the Easy Portable/Player Develop Kits
*                               mod_mmp sub-system
*                          ldec_itxt decoder plug-in module
*
*          (c) Copyright 2009-2010, Allwinner Microelectronic Co., Ltd.
*                              All Rights Reserved
*
* File   : ldec_itxt.c
* Version: V1.0
* By     : Eric_wang
* Date   : 2009-6-24
Description:
    本代码的默认规则是:一个block不可能含有两个完整的frame.最多是一个frame尾部,一个完整frame,一个frame头部.

    但实际上,本代码可以处理任意情况,允许一个block包含任意数量的frame,或frame的一部分都行.
    
    以mpgpsr而言,送字幕时,一个block最多含有一个完整的frame，或者一个frame的一部分,绝不会出现数据分属2个frame
    的情况.

********************************************************************************
*/
extern void adjust_read_pointer_case1(__ldec_lbs_buf_t *ldecLbsBuf, __u32 uread_byte);
extern void adjust_read_pointer_case2(__ldec_lbs_buf_t *ldecLbsBuf, __u32 uread_byte);
extern void adjust_read_pointer_case3(__ldec_lbs_buf_t *ldecLbsBuf, __u32 uread_byte);

/*******************************************************************************
Function name: ldec_update_read_pointer
Description: 
    1.调用这个函数时，规定情况为:ldecLbsBuf->readAddr一定在ldecLbsBuf->ldecReadIdx指向的
    ctrlblk的范围内.如果不是, fatal error,而且不做判断.

    3.这里修改的是__ldec_lbs_buf_t的ldecReadIdx, ldecIdxSize, readAddr;
    以读指针为准判定是否回头.只要读指针回头了,就认为是回头.

     4.有可能出现这种情况:validSize >0,但是一个chunk还没有装完, 所以ldecIdxSize还是应该减1之后成为0.
    也就是说:validSize > 0, 而ldecIdxSize == 0.是可能出现的.现在做了处理,chunk不装完,validSize绝不增加
Parameters: 
    
Return: 
    
Time: 2011/9/27
*******************************************************************************/
void ldec_update_read_pointer(__ldec_lbs_buf_t *ldecLbsBuf, __u32 uread_byte)
{
	if(uread_byte > 0)
	{      
		//osal_switch_disable();
        if(ldecLbsBuf->validSize >= uread_byte)
        {
            ldecLbsBuf->validSize -= uread_byte;
        }
        else
        {
            WARNING("fatal error! [%d]<[%d]\n", ldecLbsBuf->validSize, uread_byte);
            ldecLbsBuf->validSize = 0;
        }
		
		if(ldecLbsBuf->readAddr < ldecLbsBuf->ldecBufCrl[ldecLbsBuf->ldecReadIdx].startAddr)
		{
			//current packet covers buffer end and read pointer has already jumps to the beginning of buffer
			adjust_read_pointer_case1(ldecLbsBuf, uread_byte);
			
		}
		else if((ldecLbsBuf->readAddr + uread_byte) > ldecLbsBuf->ldecBufEnd)
		{
			adjust_read_pointer_case2(ldecLbsBuf, uread_byte);  		//read data covers loopback
		}
		else
		{
			adjust_read_pointer_case3(ldecLbsBuf, uread_byte);       //read data not covers loop back points
		}
	   //osal_switch_enable();	  
	}
}

/*******************************************************************************
Function name: adjust_pic_readIdx
Description: 
    1. 修改__ldec_lbs_buf_t的ldecReadIdx和ldecIdxSize.默认规则为:顺序读取
    __ldec_buffer_ctrl_t ldecBufCrl[].
    2.目的为ldecReadIdx跨过IdxSize个ctrl块.指向新的将要读取的ctrl块.
Parameters: 
    
Return: 
    
Time: 2011/9/27
*******************************************************************************/
void adjust_pic_readIdx(__ldec_lbs_buf_t *ldecLbsBuf, __u16 IdxSize)
{
	ldecLbsBuf->ldecReadIdx += IdxSize;
	ldecLbsBuf->ldecIdxSize -= IdxSize;
	if(ldecLbsBuf->ldecReadIdx >= LDEC_MAX_TXT_DATA_NUM)
		ldecLbsBuf->ldecReadIdx -= LDEC_MAX_TXT_DATA_NUM;
}
/*******************************************************************************
Function name: adjust_pic_nextIdx
Description: 
    1.给出将要读取的ctrl块的下一个ctrl块的idx.
Parameters: 
    
Return: 
    
Time: 2011/9/27
*******************************************************************************/
void adjust_pic_nextIdx(__ldec_lbs_buf_t *ldecLbsBuf, __u16 *IdxValue)
{
	*IdxValue = ldecLbsBuf->ldecReadIdx+1;
	if(*IdxValue >= LDEC_MAX_TXT_DATA_NUM)
		*IdxValue -= LDEC_MAX_TXT_DATA_NUM;
}

/*******************************************************************************
Function name: adjust_read_pointer_case1
Description: 
    1.该情况为:block本身回头了,而ldeclbsbuf->readaddr是在回头之后的buf的起始的部分.
    2. 此时,ldecLbsBuf->validSize是已经减去了用掉的字节数的值

    3.通过这个函数,修改了__ldec_lbs_buf_t的ldecReadIdx, ldecIdxSize, readAddr;
    返回时,ldecReadIdx就是当前将要读的chunk的idx.

Parameters: 
    
Return: 
    
Time: 2011/9/28
*******************************************************************************/
void adjust_read_pointer_case1(__ldec_lbs_buf_t *ldecLbsBuf, __u32 uread_byte)
{   
	__u16 nextIdx;
    
    ldecLbsBuf->readAddr += uread_byte;
    
	if(!ldecLbsBuf->validSize)
	{
		adjust_pic_readIdx(ldecLbsBuf, ldecLbsBuf->ldecIdxSize);
	}
	else
	{
	   adjust_pic_nextIdx(ldecLbsBuf, &nextIdx);
//        if(1 == ldecLbsBuf->ldecIdxSize)
//        {
//            __u8 *nextChar = ldecLbsBuf->ldecBufCrl[ldecLbsBuf->ldecReadIdx].startAddr + ldecLbsBuf->ldecBufCrl[ldecLbsBuf->ldecReadIdx].dataLen;
//            if(nextChar > ldecLbsBuf->ldecBufEnd)
//            {
//                nextChar = ldecLbsBuf->ldecBuf + (nextChar - ldecLbsBuf->ldecBufEnd - 1);
//            }
//            else
//            {
//                WARNING("case1 adjust rdpointer fatal error! [%x]\n", nextChar);
//            }
//            if(ldecLbsBuf->readAddr == nextChar)
//            {
//                WARNING("last lbs_idx[%d] is used, left [%d]bytes\n", ldecLbsBuf->ldecReadIdx, ldecLbsBuf->validSize);
//                adjust_pic_readIdx(ldecLbsBuf, 1);
//            }
//            else if(ldecLbsBuf->readAddr > nextChar)
//            {
//                WARNING("adjust rdpointer: fatal error! [%x][%x]\n", ldecLbsBuf->readAddr, nextChar);
//            }
//            else
//            {
//                WARNING("normal case, last lbs_idx[%d] is still exist!\n", ldecLbsBuf->ldecReadIdx);
//            }
//        }
	   if(( ldecLbsBuf->ldecIdxSize>1) &&
           (ldecLbsBuf->readAddr >=ldecLbsBuf->ldecBufCrl[nextIdx].startAddr))
	   {
		   adjust_pic_readIdx(ldecLbsBuf, 1);
           
		   while(ldecLbsBuf->ldecBufCrl[ldecLbsBuf->ldecReadIdx].startAddr
                    < ldecLbsBuf->readAddr)
		   {
               if(ldecLbsBuf->ldecIdxSize<= 1)
					break;
			   else
			   {
				   adjust_pic_nextIdx(ldecLbsBuf, &nextIdx);
				   if(ldecLbsBuf->ldecBufCrl[nextIdx].startAddr
                        <= ldecLbsBuf->readAddr)
				   {
                       adjust_pic_readIdx(ldecLbsBuf, 1);
				   }
				   else
				   {
					   break;
				   }
			   }
		   }     // end while()	
	   }         // end if()
	}            // ebd else()
}

/*******************************************************************************
Function name: adjust_read_pointer_case2
Description: 
    1.该情况为:block本身不管有没有回头,ldeclbsbuf->readaddr是回头之前的,而解码的长度
    回头了.解码长度的最后一个字节和buf的最后一个字节重合,因为读指针回头指向buf的第一个字节了,
    所以也认为是回头
Parameters: 
    
Return: 
    
Time: 2011/9/28
*******************************************************************************/
void adjust_read_pointer_case2(__ldec_lbs_buf_t *ldecLbsBuf, __u32 uread_byte)
{   
	__u16 nextIdx;
    __u32 nLbsBufSize = ldecLbsBuf->ldecBufEnd + 1 - ldecLbsBuf->ldecBuf;

	//ldecLbsBuf->readAddr -=   (LDEC_MAX_TXT_BUF_SIZE -uread_byte);
	ldecLbsBuf->readAddr -= (nLbsBufSize-uread_byte);
    
	if(!ldecLbsBuf->validSize)
	{   
		adjust_pic_readIdx(ldecLbsBuf, ldecLbsBuf->ldecIdxSize);
	}
	else
	{   
		adjust_pic_nextIdx(ldecLbsBuf, &nextIdx);
//        if(1 == ldecLbsBuf->ldecIdxSize)
//        {
//            __u8 *nextChar = ldecLbsBuf->ldecBufCrl[ldecLbsBuf->ldecReadIdx].startAddr + ldecLbsBuf->ldecBufCrl[ldecLbsBuf->ldecReadIdx].dataLen;
//            if(nextChar > ldecLbsBuf->ldecBufEnd)
//            {
//                nextChar = ldecLbsBuf->ldecBuf + (nextChar - ldecLbsBuf->ldecBufEnd - 1);
//            }
//            else
//            {
//                WARNING("case2:adjust rdpointer fatal error! [%x]\n", nextChar);
//            }
//            if(ldecLbsBuf->readAddr == nextChar)
//            {
//                WARNING("last lbs_idx[%d] is used, left [%d]bytes\n", ldecLbsBuf->ldecReadIdx, ldecLbsBuf->validSize);
//                adjust_pic_readIdx(ldecLbsBuf, 1);
//            }
//            else if(ldecLbsBuf->readAddr > nextChar)
//            {
//                WARNING("adjust rdpointer: fatal error! [%x][%x]\n", ldecLbsBuf->readAddr, nextChar);
//            }
//            else
//            {
//                WARNING("normal case, last lbs_idx[%d] is still exist!\n", ldecLbsBuf->ldecReadIdx);
//            }
//        }

		while((ldecLbsBuf->ldecIdxSize>1) 
            && (ldecLbsBuf->ldecBufCrl[ldecLbsBuf->ldecReadIdx].startAddr
                < ldecLbsBuf->ldecBufCrl[nextIdx].startAddr))
		{
			adjust_pic_readIdx(ldecLbsBuf, 1);
            adjust_pic_nextIdx(ldecLbsBuf, &nextIdx);
		} 

         if(ldecLbsBuf->ldecIdxSize > 1)
		 {
			 adjust_pic_nextIdx(ldecLbsBuf, &nextIdx);
			 
			 if(ldecLbsBuf->ldecBufCrl[nextIdx].startAddr 
                <= ldecLbsBuf->readAddr)
			 {
				 adjust_pic_readIdx(ldecLbsBuf, 1);
				 
				 while(ldecLbsBuf->ldecBufCrl[ldecLbsBuf->ldecReadIdx].startAddr < ldecLbsBuf->readAddr)
				 {
					 if(ldecLbsBuf->ldecIdxSize <= 1)
						 break;
					 
					 adjust_pic_nextIdx(ldecLbsBuf, &nextIdx);

					 if(ldecLbsBuf->ldecBufCrl[nextIdx].startAddr 
                            <= ldecLbsBuf->readAddr)
						 adjust_pic_readIdx(ldecLbsBuf, 1);
					 else
						break;
				 }

			 }
		 }
	}
}

/*******************************************************************************
Function name: adjust_read_pointer_case3
Description: 
    1.该情况为:block本身不管有没有回头,而ldeclbsbuf->readaddr是回头之前,而且整个frame都在
    回头之前.关键是读指针不回头.
    
Parameters: 
    
Return: 
    
Time: 2011/9/28
*******************************************************************************/
void adjust_read_pointer_case3(__ldec_lbs_buf_t *ldecLbsBuf, __u32 uread_byte)
{   
	__u16 nextIdx, curIdx;
	__bool flag1, flag2;

    ldecLbsBuf->readAddr = ldecLbsBuf->readAddr + uread_byte;

	if(!ldecLbsBuf->validSize)
	{
		adjust_pic_readIdx(ldecLbsBuf, ldecLbsBuf->ldecIdxSize);
	}
	else
	{
//        if(1 == ldecLbsBuf->ldecIdxSize)
//        {
//            __u8 *nextChar = ldecLbsBuf->ldecBufCrl[ldecLbsBuf->ldecReadIdx].startAddr + ldecLbsBuf->ldecBufCrl[ldecLbsBuf->ldecReadIdx].dataLen;
//            if(nextChar > ldecLbsBuf->ldecBufEnd)
//            {
//                nextChar = ldecLbsBuf->ldecBuf + (nextChar - ldecLbsBuf->ldecBufEnd - 1);
//            }
//            if(ldecLbsBuf->readAddr == nextChar)
//            {
//                WARNING("last lbs_idx[%d] is used, left [%d]bytes\n", ldecLbsBuf->ldecReadIdx, ldecLbsBuf->validSize);
//                adjust_pic_readIdx(ldecLbsBuf, 1);
//            }
//            else if(ldecLbsBuf->readAddr > nextChar)
//            {
//                WARNING("case3 be careful! [%x][%x]\n", ldecLbsBuf->readAddr, nextChar);
//            }
//            else
//            {
//                WARNING("normal case, last lbs_idx[%d] is still exist!\n", ldecLbsBuf->ldecReadIdx);
//            }
//        }
        if(ldecLbsBuf->ldecIdxSize > 1)
        {
    		while(ldecLbsBuf->ldecBufCrl[ldecLbsBuf->ldecReadIdx].startAddr < ldecLbsBuf->readAddr)
    		{
    			if(ldecLbsBuf->ldecIdxSize <= 1)
    				break;
    			else
    			{
    				adjust_pic_nextIdx(ldecLbsBuf, &nextIdx);
    				curIdx = ldecLbsBuf->ldecReadIdx;
    				flag1 = (ldecLbsBuf->ldecBufCrl[nextIdx].startAddr 
                                <= ldecLbsBuf->readAddr);
    				flag2 = (ldecLbsBuf->ldecBufCrl[nextIdx].startAddr
                                >ldecLbsBuf->ldecBufCrl[curIdx].startAddr);
    				
    				if(flag1 && flag2)
    				{
    					adjust_pic_readIdx(ldecLbsBuf, 1);
    				}
    				else
    				{
    					break;
    				}
    			}
    		}
        }
	}
}

//********************************判断数据是否跨边界**********************************************//

/*******************************************************************************
Function name: ldec_judge_subdata_cover_bufEnd
Description: 
    因为字幕数据块不一定能覆盖整个条目,所以这里会判断一个sub字幕项占了几个数据块.至少一个.
    mkvpsr目前送数据都是一个数据块就是一个整项,而且是整字幕项.
    tspsr封pgs,一个数据块就是一个整项,不一定是字幕.若干项组成一个字幕,但pgs解码就是以整项为单位,解完若干项得出一条字幕.
    mpgpsr一个数据块不一定是一个整字幕项.但可以根据pts判断出整字幕项重新拼起来.所以对于mpgpsr而言, pts非常重要,必须送.


    itext要求一个数据块必须是一个完整的字幕项.
    issa同上.
    isub不要求,对数据块的要求很宽松,它会自己根据解码结果知道一帧是否解完. lbsctrlblock对它没有太大的参考意义. 只是pts要拿到.
    只是希望pts尽可能准确.而且两pts之间最好就是一条字幕
    pgs decode要求一个数据块必须是一个整数据项,而若干数据项可以解码出一条pgs字幕.

    
Parameters: 
    
Return: 
    
Time: 2011/9/27
*******************************************************************************/
 //__u8 *ldec_judge_subdata_cover_bufEnd(ItxtDec *ldec_itxt)
/*******************************************************************************
Function name: ldec_process_chunk_cover_bufEnd
Description: 
    1.用于基类的decode()函数,只负责处理当前block回头的情况,如果回头,把该block
    拷贝到ancibuf中.返回ancibuf的指针。正常情况返回lbsbuf的指针
Parameters: 
    **ppanciDataBuf : 如果ancibuf不够,会重新分配.所以送指针的指针.
Return: 
    
Time: 2011/9/28
*******************************************************************************/
__u8 *ldec_process_chunk_cover_bufEnd(__ldec_lbs_buf_t *pLbsBufManager, __u8 **ppanciDataBuf, __u32 *pnanciDataBufSize)
{    
    __u8  curReadIdx  = 0;
    __u32 nLen1, nLen2;
    __ldec_buffer_ctrl_t  *pLbsChunk = NULL;
    __u8 *anciDataBuf = *ppanciDataBuf;
    __u32 nanciDataBufSize = *pnanciDataBufSize;

    curReadIdx = pLbsBufManager->ldecReadIdx;
    pLbsChunk = &pLbsBufManager->ldecBufCrl[curReadIdx];
    if(pLbsChunk->startAddr + pLbsChunk->dataLen <= pLbsBufManager->ldecBufEnd + 1)   //不回头
    {
        if(pLbsChunk->startAddr + pLbsChunk->dataLen == pLbsBufManager->ldecBufEnd + 1)
        {
            WARNING("LbsBlock end == ldecbufend! be careful!\n");
        }
        return pLbsChunk->startAddr;
    }
    else    //回头
    {
        if(nanciDataBufSize < pLbsChunk->dataLen)
        {
            WARNING("fatal error! ancibuf size is small, [%d][%d]\n", nanciDataBufSize, pLbsChunk->dataLen);
            PHY_FREE(anciDataBuf);
            anciDataBuf = PHY_MALLOC(pLbsChunk->dataLen, 1024);
            if(NULL == anciDataBuf)
            {
                WARNING("fatal error! malloc [%d]bytes fail\n", pLbsChunk->dataLen);
                nanciDataBufSize = 0;
                *ppanciDataBuf = NULL;
                //*pnanciDataBufSize = 0;
                return NULL;
            }
            else
            {
                nanciDataBufSize = pLbsChunk->dataLen;
            }
        }
        //拷贝该block到临时的ancibuf
        nLen1 = pLbsBufManager->ldecBufEnd + 1 - pLbsChunk->startAddr;
        nLen2 = pLbsChunk->dataLen - nLen1;
        memcpy(anciDataBuf, pLbsChunk->startAddr, nLen1);
        memcpy(anciDataBuf + nLen1, pLbsBufManager->ldecBuf, nLen2);
        *ppanciDataBuf = anciDataBuf;
        *pnanciDataBufSize = nanciDataBufSize;
        return anciDataBuf;
    }
}

#if 0
void ldec_update_read_pointer1(__ldec_lbs_buf_t *ldecLbsBuf, __u32 uread_byte)
{
    //纯粹为pgs字幕debug用,debug完成必须删除
    if(ldecLbsBuf->readAddr != ldecLbsBuf->ldecBufCrl[ldecLbsBuf->ldecReadIdx].startAddr)
    {
        WARNING("fatal error![%d][0x%x][0x%x]\n", ldecLbsBuf->ldecReadIdx, ldecLbsBuf->readAddr, ldecLbsBuf->ldecBufCrl[ldecLbsBuf->ldecReadIdx].startAddr);
    }
    if(uread_byte == 0)
    {
        WARNING("updatepointer usedlen[%d]\n", uread_byte);
    }
    if(0 == ldecLbsBuf->ldecIdxSize)
    {
        WARNING("fatal error! adjust rdpointer: idxSize[%d]\n", ldecLbsBuf->ldecIdxSize);
    }
	if(uread_byte > 0)
	{      
		//osal_switch_disable();
        if(ldecLbsBuf->validSize >= uread_byte)
        {
            ldecLbsBuf->validSize -= uread_byte;
        }
        else
        {
            WARNING("update lbspointer:fatal error! [%d]<[%d]\n", ldecLbsBuf->validSize, uread_byte);
            ldecLbsBuf->validSize = 0;
        }
		
		if(ldecLbsBuf->readAddr < ldecLbsBuf->ldecBufCrl[ldecLbsBuf->ldecReadIdx].startAddr)
		{
            WARNING("pgs fatal error! [%d][0x%x][0x%x]\n", ldecLbsBuf->ldecReadIdx, ldecLbsBuf->readAddr, ldecLbsBuf->ldecBufCrl[ldecLbsBuf->ldecReadIdx].startAddr);
			//current packet covers buffer end and read pointer has already jumps to the beginning of buffer
			adjust_read_pointer_case1(ldecLbsBuf, uread_byte);
			
		}
		else if((ldecLbsBuf->readAddr + uread_byte) > ldecLbsBuf->ldecBufEnd)
		{
			adjust_read_pointer_case2(ldecLbsBuf, uread_byte);  		//read data covers loopback
		}
		else
		{
			adjust_read_pointer_case3(ldecLbsBuf, uread_byte);       //read data not covers loop back points
		}
	   //osal_switch_enable();	  
	    if(0 == ldecLbsBuf->validSize || 0 == ldecLbsBuf->ldecIdxSize)
	    {
            WARNING("update lbspointer: [%d][%d]\n", ldecLbsBuf->validSize, ldecLbsBuf->ldecIdxSize);
	    }
        if(ldecLbsBuf->ldecIdxSize)
        {
            if(ldecLbsBuf->readAddr != ldecLbsBuf->ldecBufCrl[ldecLbsBuf->ldecReadIdx].startAddr)
            {
                WARNING("after update lbspointer: fatal error![%d][0x%x][0x%x]\n", ldecLbsBuf->ldecReadIdx, ldecLbsBuf->readAddr, ldecLbsBuf->ldecBufCrl[ldecLbsBuf->ldecReadIdx].startAddr);
            }
        }
	}
}
#endif

