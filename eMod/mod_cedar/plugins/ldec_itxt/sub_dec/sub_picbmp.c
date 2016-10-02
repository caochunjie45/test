

#include "subcore_s.h"


__s8 convert_data_to_bmp(__cedar_subtitle_item_t *curSuItem, __u8 *dataBuf)
{   
    __u8 val;
	__u16 i, j;
	__u16 picSizex, picSizey;
	__u32 picSize;
    __u16 fstXpos, fstYpos;
    __u16 endXpos, endYpos;
    __u8  contrast[4] = {0};

    contrast[0] = curSuItem->contrast.backpixelContrast;
    contrast[1] = curSuItem->contrast.patpixelContrast;
    contrast[2] = curSuItem->contrast.pixel1Contrast;
    contrast[3] =  curSuItem->contrast.pixel2Contrast;
 
	fstXpos = curSuItem->nPicWidth;
	fstYpos = curSuItem->nPicHeight;
	endXpos = 0;
    endYpos = 0;
    
	for(i=0; i<curSuItem->nPicHeight; i++)
	{
        for(j=1; j<curSuItem->nPicWidth-1; j++)
		{    
            val = dataBuf[i*curSuItem->nPicWidth+j];
			 if((val!= 0) &&(contrast[val]!= 0))
			{   
				if(j < fstXpos)
                    fstXpos = j;
				if(j > endXpos)
				    endXpos = j;
                if(i < fstYpos)
                    fstYpos = i;
				if(i > endYpos)
                    endYpos = i;
			}
		}
	}
    
	if((fstXpos >= endXpos) || (fstYpos >= endYpos))
	{
		return -1;
	}
	picSizex = endXpos - fstXpos + 1; 
	picSizey = endYpos - fstYpos + 1; 
    
	picSize = picSizex * picSizey;

    if(picSize > LDEC_MAX_TXT_FRM_SIZE)
    {
      //PHY_FREE(curSuItem->pSubtitle);
      //curSuItem->pSubtitle = PHY_MALLOC(160*1024, 1024);
      //return -1;
      picSizex = LDEC_MAX_TXT_FRM_SIZE / picSizey;
    }

    if((picSizex <= 10) ||(picSizey <=10))
      return -1;

	for(j = 0; j<picSizey; j++)
	{   
		memcpy(curSuItem->pSubtitle+j*picSizex, dataBuf+(fstYpos+j)*curSuItem->nPicWidth+fstXpos, picSizex);
	}

	curSuItem->nPicWidth= picSizex;
    curSuItem->nPicHeight = picSizey;
    curSuItem->uSubSize = picSize;
	return 0;
}

