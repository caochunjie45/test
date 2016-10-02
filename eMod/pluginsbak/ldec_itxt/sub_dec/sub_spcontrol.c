
#include "subcore_s.h"

//__s32 process_sp_dcsqt(__ldec_itxt_t *pdec, __cedar_subtitle_item_t *curSuItem, __u8 *start_addr, __ldec_sp_dcqt_t *spSqtInfo)        
__s32 process_sp_dcsqt(ItxtDec *pdec, __cedar_subtitle_item_t *curSuItem, __u8 *start_addr, __ldec_sp_dcqt_t *spSqtInfo)
{   
	__u8 *spaddr = NULL;
    __u8  *endAddr = NULL;
	__u16 fstTmval = 0;
	__u16 fstSqsa  = 0;
	__u16 lastTmval = 0;
	__u16 lastSqsa  = 0;
	__u16 startx = 0;
	__u16 starty = 0;
	__u16 endx = 0;
	__u16 endy = 0;
    __u16 frameNum = 0;
	__bool findLastdcsq = FALSE;
	__bool lastDcsqStop = FALSE;
    __bool findColorcode = FALSE;
    __bool findContrast = FALSE;

    spaddr = start_addr + spSqtInfo->spStartaddr;
    endAddr = start_addr + spSqtInfo->spuSize;
    
	fstTmval = (spaddr[0] << 8) | spaddr[1];
    fstSqsa = (spaddr[2] << 8) | spaddr[3];
    spaddr += 4;

    curSuItem->contrast.backpixelContrast = 0;
    curSuItem->contrast.patpixelContrast = 0;
    curSuItem->contrast.pixel1Contrast = 0;
    curSuItem->contrast.pixel2Contrast = 0;
                   
	while(lastDcsqStop == FALSE)
	{   
		while(spaddr <= endAddr)
		{   
			if(spaddr[0] == 0x03)
			{   
                findColorcode = TRUE;
				curSuItem->colorCode.pixel2ColorCode = spaddr[1] >> 4;
				curSuItem->colorCode.pixel1ColorCode = spaddr[1] & 0x0f;
				curSuItem->colorCode.patpixelColorCode = spaddr[2] >> 4;
				curSuItem->colorCode.backpixelColorCode = spaddr[2] & 0x0f;
				spaddr += 2;
			}
			else if(spaddr[0] == 0x04)
			{   
			    if((spaddr[1]||spaddr[2])!=0)
			    {  
                   findContrast = TRUE;
			       curSuItem->contrast.pixel2Contrast = spaddr[1] >> 4;
				   curSuItem->contrast.pixel1Contrast = spaddr[1] & 0x0f;
				   curSuItem->contrast.patpixelContrast = spaddr[2] >> 4;
				   curSuItem->contrast.backpixelContrast = spaddr[2] & 0x0f;
			    }
				spaddr += 2;
			}
			else if(spaddr[0] == 0x05)
			{
				startx = ((spaddr[1]&0x3f)<<4) | (spaddr[2]>>4);
				endx = ((spaddr[2]&0x0f)<<8) | spaddr[3];
				starty = ((spaddr[4]&0x3f)<<4) | (spaddr[5]>>4);
				endy = ((spaddr[5]&0x03)<<8) | spaddr[6];
                
                if((endx <= startx) || (endy <= starty))
                    return -1;    
				curSuItem->nPicWidth= (endx - startx + 1);
				curSuItem->nPicHeight = (endy - starty + 1);
           
				spaddr += 6;
			}
			else if(spaddr[0] == 0x06)
			{   
				spSqtInfo->fstTop = (spaddr[1]<<8) | spaddr[2];
				spSqtInfo->fstBottom = (spaddr[3]<<8) | spaddr[4];
				spaddr += 4;
			}
			else if(spaddr[0] == 0xff)
			{   
				if(findLastdcsq == TRUE)
			      lastDcsqStop = TRUE;
				spaddr ++;
				break;
			}
			spaddr ++;
		}
        if(findLastdcsq == FALSE)
		{
			spaddr = start_addr + fstSqsa;
			lastTmval = (spaddr[0] << 8) | spaddr[1];
			lastSqsa = (spaddr[2] << 8) | spaddr[3];
			spaddr += 4;
			if(lastSqsa == fstSqsa)
				findLastdcsq = TRUE;
			else
                fstSqsa = lastSqsa;
		}
	}

    if(findColorcode == FALSE)
    {
        curSuItem->colorCode.pixel2ColorCode = pdec->lastColorCode[3];
		curSuItem->colorCode.pixel1ColorCode = pdec->lastColorCode[2];
		curSuItem->colorCode.patpixelColorCode = pdec->lastColorCode[1];
	    curSuItem->colorCode.backpixelColorCode = pdec->lastColorCode[0];
    }
    else
    {
        pdec->lastColorCode[3] = curSuItem->colorCode.pixel2ColorCode;
		pdec->lastColorCode[2] = curSuItem->colorCode.pixel1ColorCode;
		pdec->lastColorCode[1] = curSuItem->colorCode.patpixelColorCode;
	    pdec->lastColorCode[0] = curSuItem->colorCode.backpixelColorCode;
    }
     if(findContrast == FALSE)
    {
        curSuItem->contrast.pixel2Contrast  = pdec->lastContrast[3];
		curSuItem->contrast.pixel1Contrast= pdec->lastContrast[2];
		curSuItem->contrast.patpixelContrast= pdec->lastContrast[1];
	    curSuItem->contrast.backpixelContrast= pdec->lastContrast[0];
    }
    else
    {
        pdec->lastContrast[3] = curSuItem->contrast.pixel2Contrast;
		pdec->lastContrast[2] = curSuItem->contrast.pixel1Contrast;
		pdec->lastContrast[1] = curSuItem->contrast.patpixelContrast;
	    pdec->lastContrast[0] = curSuItem->contrast.backpixelContrast;
    }
    
    if((curSuItem->contrast.pixel2Contrast ==0) &&(curSuItem->contrast.pixel1Contrast==0)
        &&(curSuItem->contrast.patpixelContrast==0) &&(curSuItem->contrast.backpixelContrast==0))
    {
        return -1;
    }
    if(spaddr >= (endAddr+1))
    {
        return -1;
    }
        
   	if(lastTmval <=  fstTmval)
   	{
        curSuItem->uDuration =  2000;  
        WARNING("The duration of the sub pic is 2s.\n");
    }
    else
    {
       curSuItem->uDuration = (lastTmval - fstTmval);
	   if(pdec->tvType == TV_SYSTEM_NTSC)
	      frameNum = (curSuItem->uDuration * 1024)/3003 + 1;
	   else if(pdec->tvType == TV_SYSTEM_PAL)
	      frameNum = (curSuItem->uDuration * 64)/225 + 1;
	   curSuItem->uDuration =  (1000 *1000 /pdec->frm_rate) * frameNum;
    } 
	return 0;
}

