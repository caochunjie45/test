
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
    �������Ĭ�Ϲ�����:һ��block�����ܺ�������������frame.�����һ��frameβ��,һ������frame,һ��frameͷ��.

    ��ʵ����,��������Դ����������,����һ��block��������������frame,��frame��һ���ֶ���.
    
    ��mpgpsr����,����Ļʱ,һ��block��ຬ��һ��������frame������һ��frame��һ����,������������ݷ���2��frame
    �����.

********************************************************************************
*/
extern void adjust_read_pointer_case1(__ldec_lbs_buf_t *ldecLbsBuf, __u32 uread_byte);
extern void adjust_read_pointer_case2(__ldec_lbs_buf_t *ldecLbsBuf, __u32 uread_byte);
extern void adjust_read_pointer_case3(__ldec_lbs_buf_t *ldecLbsBuf, __u32 uread_byte);

/*******************************************************************************
Function name: ldec_update_read_pointer
Description: 
    1.�����������ʱ���涨���Ϊ:ldecLbsBuf->readAddrһ����ldecLbsBuf->ldecReadIdxָ���
    ctrlblk�ķ�Χ��.�������, fatal error,���Ҳ����ж�.

    3.�����޸ĵ���__ldec_lbs_buf_t��ldecReadIdx, ldecIdxSize, readAddr;
    �Զ�ָ��Ϊ׼�ж��Ƿ��ͷ.ֻҪ��ָ���ͷ��,����Ϊ�ǻ�ͷ.

     4.�п��ܳ����������:validSize >0,����һ��chunk��û��װ��, ����ldecIdxSize����Ӧ�ü�1֮���Ϊ0.
    Ҳ����˵:validSize > 0, ��ldecIdxSize == 0.�ǿ��ܳ��ֵ�.�������˴���,chunk��װ��,validSize��������
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
    1. �޸�__ldec_lbs_buf_t��ldecReadIdx��ldecIdxSize.Ĭ�Ϲ���Ϊ:˳���ȡ
    __ldec_buffer_ctrl_t ldecBufCrl[].
    2.Ŀ��ΪldecReadIdx���IdxSize��ctrl��.ָ���µĽ�Ҫ��ȡ��ctrl��.
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
    1.������Ҫ��ȡ��ctrl�����һ��ctrl���idx.
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
    1.�����Ϊ:block�����ͷ��,��ldeclbsbuf->readaddr���ڻ�ͷ֮���buf����ʼ�Ĳ���.
    2. ��ʱ,ldecLbsBuf->validSize���Ѿ���ȥ���õ����ֽ�����ֵ

    3.ͨ���������,�޸���__ldec_lbs_buf_t��ldecReadIdx, ldecIdxSize, readAddr;
    ����ʱ,ldecReadIdx���ǵ�ǰ��Ҫ����chunk��idx.

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
    1.�����Ϊ:block��������û�л�ͷ,ldeclbsbuf->readaddr�ǻ�ͷ֮ǰ��,������ĳ���
    ��ͷ��.���볤�ȵ����һ���ֽں�buf�����һ���ֽ��غ�,��Ϊ��ָ���ͷָ��buf�ĵ�һ���ֽ���,
    ����Ҳ��Ϊ�ǻ�ͷ
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
    1.�����Ϊ:block��������û�л�ͷ,��ldeclbsbuf->readaddr�ǻ�ͷ֮ǰ,��������frame����
    ��ͷ֮ǰ.�ؼ��Ƕ�ָ�벻��ͷ.
    
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

//********************************�ж������Ƿ��߽�**********************************************//

/*******************************************************************************
Function name: ldec_judge_subdata_cover_bufEnd
Description: 
    ��Ϊ��Ļ���ݿ鲻һ���ܸ���������Ŀ,����������ж�һ��sub��Ļ��ռ�˼������ݿ�.����һ��.
    mkvpsrĿǰ�����ݶ���һ�����ݿ����һ������,����������Ļ��.
    tspsr��pgs,һ�����ݿ����һ������,��һ������Ļ.���������һ����Ļ,��pgs�������������Ϊ��λ,����������ó�һ����Ļ.
    mpgpsrһ�����ݿ鲻һ����һ������Ļ��.�����Ը���pts�жϳ�����Ļ������ƴ����.���Զ���mpgpsr����, pts�ǳ���Ҫ,������.


    itextҪ��һ�����ݿ������һ����������Ļ��.
    issaͬ��.
    isub��Ҫ��,�����ݿ��Ҫ��ܿ���,�����Լ����ݽ�����֪��һ֡�Ƿ����. lbsctrlblock����û��̫��Ĳο�����. ֻ��ptsҪ�õ�.
    ֻ��ϣ��pts������׼ȷ.������pts֮����þ���һ����Ļ
    pgs decodeҪ��һ�����ݿ������һ����������,��������������Խ����һ��pgs��Ļ.

    
Parameters: 
    
Return: 
    
Time: 2011/9/27
*******************************************************************************/
 //__u8 *ldec_judge_subdata_cover_bufEnd(ItxtDec *ldec_itxt)
/*******************************************************************************
Function name: ldec_process_chunk_cover_bufEnd
Description: 
    1.���ڻ����decode()����,ֻ������ǰblock��ͷ�����,�����ͷ,�Ѹ�block
    ������ancibuf��.����ancibuf��ָ�롣�����������lbsbuf��ָ��
Parameters: 
    **ppanciDataBuf : ���ancibuf����,�����·���.������ָ���ָ��.
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
    if(pLbsChunk->startAddr + pLbsChunk->dataLen <= pLbsBufManager->ldecBufEnd + 1)   //����ͷ
    {
        if(pLbsChunk->startAddr + pLbsChunk->dataLen == pLbsBufManager->ldecBufEnd + 1)
        {
            WARNING("LbsBlock end == ldecbufend! be careful!\n");
        }
        return pLbsChunk->startAddr;
    }
    else    //��ͷ
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
        //������block����ʱ��ancibuf
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
    //����Ϊpgs��Ļdebug��,debug��ɱ���ɾ��
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

