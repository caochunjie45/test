

#include "subcore_s.h"


__u8 *run_len_decode_process(__cedar_subtitle_item_t *curSuItem,  __ldec_sp_dcqt_t *spSqtInfo, __u8 *curPicptr, __u8 *dataBuf,
							  __u16 maxSizey, __u32 run, __u8 len, __u8 *contrast, __u8 fieldType, __u32 *sBitLeft)

{   
    __s32 minusLineNs = 0;
    __u32 ptrOffset   = 0;
	__bool flag1 = FALSE, flag2 = FALSE;

	minusLineNs = spSqtInfo->maxDecLine[fieldType] - spSqtInfo->minDecLine[fieldType];
	
	flag1 = (spSqtInfo->nonZreoValFlag == TRUE) && (minusLineNs == maxSizey);
	flag2 = (spSqtInfo->nonZreoValFlag == FALSE) && (spSqtInfo->maxDecLine[fieldType] == (curSuItem->nPicHeight/2 -1));

	if(flag1 || flag2)
	{   
		return NULL;
	}
	else
	{
		if(spSqtInfo->nonZreoValFlag == FALSE)           // 未发现不全为0 的一行
		{   
            if((contrast[len]==0)||(len==0))
            {   
                while(run > 0)
                {
                    if((spSqtInfo->hasDecnum + run) < curSuItem->nPicWidth)
                   {
                       spSqtInfo->hasDecnum  += run;
                       break;
                   }
                  else
                  {    
                       *sBitLeft = 0;
                       run -= (curSuItem->nPicWidth - spSqtInfo->hasDecnum);
                       spSqtInfo->hasDecnum = 0;
                       spSqtInfo->maxDecLine[fieldType] += 1;
                   }
                }
            }      
			else            // len != 0
			{
				if(fieldType == BOTTOM_FIELD_FLAG)
				{
                    if(spSqtInfo->maxDecLine[BOTTOM_FIELD_FLAG] < (spSqtInfo->minDecLine[TOP_FIELD_FLAG] -1))
					{
						spSqtInfo->maxDecLine[fieldType] ++;
						return curPicptr;
					}
					else if(spSqtInfo->maxDecLine[BOTTOM_FIELD_FLAG] == (spSqtInfo->minDecLine[TOP_FIELD_FLAG] - 1))
					{
						curPicptr -= 2*curSuItem->nPicWidth;
						spSqtInfo->decBufAddr = curPicptr - curSuItem->nPicWidth;
					} 
				}
               
				spSqtInfo->minDecLine[fieldType] = spSqtInfo->maxDecLine[fieldType];
				spSqtInfo->nonZreoValFlag = TRUE;
                spSqtInfo->nonEnoughData = TRUE;
                if(spSqtInfo->hasDecnum >= curSuItem->nPicWidth)
                    spSqtInfo->hasDecnum -= curSuItem->nPicWidth;
				memset(curPicptr, spSqtInfo->lastVal, spSqtInfo->hasDecnum);	
                curPicptr += spSqtInfo->hasDecnum;
     
			}
            spSqtInfo->lastVal = len;
		} 
	    
		if(spSqtInfo->nonZreoValFlag == TRUE)
        {   
            if((spSqtInfo->hasDecnum +run) >= 2*curSuItem->nPicWidth)
            {
                curPicptr = NULL;
                return curPicptr;
            }
            while(run > 0)
            {  
               if((spSqtInfo->hasDecnum +run) < curSuItem->nPicWidth)
               {
                 memset(curPicptr, len, run);
                 spSqtInfo->hasDecnum += run;
                 curPicptr += run;
                 run  = 0;
              }
              else
              {  
                *sBitLeft = 0;
                 memset(curPicptr, len, curSuItem->nPicWidth - spSqtInfo->hasDecnum);
                 run -= (curSuItem->nPicWidth - spSqtInfo->hasDecnum);
                 spSqtInfo->hasDecnum = 0;
                 spSqtInfo->maxDecLine[fieldType] ++;
                 if(spSqtInfo->maxDecLine[fieldType] < (curSuItem->nPicHeight/2))
				 {	 
                     ptrOffset = (fieldType +2*(spSqtInfo->maxDecLine[fieldType]-spSqtInfo->minDecLine[fieldType]))*curSuItem->nPicWidth;
                     curPicptr = spSqtInfo->decBufAddr + ptrOffset + spSqtInfo->hasDecnum;
                 }
                 else
                 {
                    curPicptr = NULL;
                    break;
                 }
              }
            }
	       }
	    }
	return curPicptr;
}


__u32 get_bits (__u8 bits, __u8* buf, __u32* offset, __u32* sData,__u32* sBitLeft)
{
	__u32 ret = 0;

    *offset = 0;

	while(bits)
    {
		if (bits > (*sBitLeft))
        {
			ret |= (*sData) << (bits - (*sBitLeft));
			bits -= (*sBitLeft);
			(*sData) = *buf;
			(*sBitLeft) = 8;
            buf++;
            (*offset) += 1;
		} 
        else 
       {
			(*sBitLeft) -= bits;
			ret |= (*sData) >> (*sBitLeft);
			(*sData) &= (1 << (*sBitLeft)) - 1;
			bits = 0;
		}
	} 
	return ret;	
}


__s8 run_length_decode(__cedar_subtitle_item_t *curSuItem, __u8 *start_addr, __ldec_sp_dcqt_t *spSqtInfo, __u8 *dataBuf, __u8 fieldType)
{   
	__u8 *curPtr = NULL;
	__u8 *curPicPtr = NULL;
	__u32 dataRun = 0;
    __u32 staticData = 0;
    __u32 staticBitLeft = 0;
    __u32 vlc = 0;
    __u32 offset = 0;
	__u8  dataLen = 0;
	__u16 maxSizey = 0;    
	__u16 maxDecLine = 0;
	__u16 minDecLine = 0;
	__u8  colorContrast[4] = {0};
    
	spSqtInfo->nonZreoValFlag = FALSE;
	spSqtInfo->nonEnoughData = FALSE;
	spSqtInfo->hasDecnum = 0;
	spSqtInfo->maxDecLine[fieldType] = 0;
	spSqtInfo->minDecLine[fieldType] = 0;
    spSqtInfo->lastVal = 0;
    
	colorContrast[0] = curSuItem->contrast.backpixelContrast;
	colorContrast[1] = curSuItem->contrast.patpixelContrast;
	colorContrast[2] = curSuItem->contrast.pixel1Contrast;
	colorContrast[3] = curSuItem->contrast.pixel2Contrast;

	maxSizey = LDEC_MAX_TXT_DATA_SIZE / (curSuItem->nPicWidth* 2) - 2;

	if(fieldType == TOP_FIELD_FLAG)
	{   
		curPtr = start_addr + spSqtInfo->fstTop;
		curPicPtr = dataBuf + curSuItem->nPicWidth;
		spSqtInfo->decBufAddr = curPicPtr;
	}
	else if(fieldType == BOTTOM_FIELD_FLAG)
	{   
		curPtr = start_addr + spSqtInfo->fstBottom;
		curPicPtr = dataBuf + 2*curSuItem->nPicWidth;
	}
    
    staticData = 0;
    staticBitLeft = 0;
    
    while(spSqtInfo->maxDecLine[fieldType]< (curSuItem->nPicHeight/2))
    {   
        vlc = get_bits (4, curPtr, &offset, &staticData,&staticBitLeft);
        curPtr += offset;
        
		if(vlc < 0x0004) 
		{
		    vlc = (vlc << 4) | get_bits (4, curPtr, &offset,&staticData,&staticBitLeft);
            curPtr += offset;
			if(vlc < 0x0010) 
			{
			   vlc = (vlc << 4) | get_bits (4, curPtr, &offset,&staticData,&staticBitLeft);
               curPtr += offset;
				if(vlc < 0x0040) 
			    {
                   vlc = (vlc << 4) | get_bits (4, curPtr, &offset,&staticData,&staticBitLeft);
                   curPtr += offset;
				}       
			}
		}

        if(vlc == 0) 
        {
		    dataRun = curSuItem->nPicWidth - spSqtInfo->hasDecnum;
			dataLen = 0;
		}
		else 
        {
			dataRun = vlc>>2;
			if(dataRun == 0)
				dataRun = curSuItem->nPicWidth - spSqtInfo->hasDecnum;
			dataLen = vlc & 0x03;
		}
        
        curPicPtr = run_len_decode_process(curSuItem, spSqtInfo, curPicPtr, dataBuf, maxSizey, dataRun, dataLen, colorContrast,fieldType, &staticBitLeft);
		if(curPicPtr == NULL)
		{
			break;
		}
    }
    
    if(spSqtInfo->nonEnoughData == FALSE)
        return -1;
	if(fieldType == BOTTOM_FIELD_FLAG)
	{   
		if(spSqtInfo->maxDecLine[TOP_FIELD_FLAG] >= spSqtInfo->maxDecLine[BOTTOM_FIELD_FLAG])
			maxDecLine = spSqtInfo->maxDecLine[TOP_FIELD_FLAG];
		else
			maxDecLine = spSqtInfo->maxDecLine[BOTTOM_FIELD_FLAG];
        
		if(spSqtInfo->minDecLine[TOP_FIELD_FLAG] <= spSqtInfo->minDecLine[BOTTOM_FIELD_FLAG])
			minDecLine = spSqtInfo->minDecLine[TOP_FIELD_FLAG];
		else
            minDecLine = spSqtInfo->minDecLine[BOTTOM_FIELD_FLAG];
		
		curSuItem->nPicHeight = 2 * (maxDecLine - minDecLine);
	}
	return 0;
}


