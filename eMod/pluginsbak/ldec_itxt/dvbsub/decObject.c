

#include "subdec.h"
//*************************************************************************************************//
//*************************************************************************************************//

static __u8 TWO_TO_FOUR_MAP_TABLE[4]    = { 0x00, 0x07, 0x08, 0x0F};
static __u8 TWO_TO_EIGHT_MAP_TABLE[4]   = { 0x00, 0x77, 0x88, 0xFF};
static __u8 FOUR_TO_EIGHT_MAP_TABLE[16] = { 0x00, 0x11, 0x22, 0x33,
                                            0x44, 0x55, 0x66, 0x77,
                                            0x88, 0x99, 0xAA, 0xBB,
                                            0xCC, 0xDD, 0xEE, 0xFF};
//********************************************************************************//
//********************************************************************************//
__u8 subdec_get_next_some_bits(__u8 *dataBuf, __u32 *hasUsedBits, __u8 someBits)
{
	__u32 pos =0;
	__u8  remainBits = 0;
	__u8  getVal = 0;
	__u8  nextVal = 0;
	__u32 usedBits = 0;

	usedBits = *hasUsedBits;
	pos = usedBits/8;
	remainBits = (pos+1)*8 - usedBits;

	if(remainBits >= someBits)
	{
		getVal = dataBuf[pos] >> (remainBits - someBits);
		getVal &= ((1<<someBits) - 1);
	}
	else
	{
		getVal = dataBuf[pos] & ((1<<remainBits) - 1);
		getVal <<= (someBits - remainBits);
		nextVal = dataBuf[pos+1] >> (8+remainBits-someBits);
		getVal |= nextVal;
	}

	(*hasUsedBits) += someBits;
	return getVal;
}

static __u16 pixelLen;
//********************************************************************************//
//********************************************************************************//
__u32 subdec_2bit_pixel_code_string(__u8 *newDataBuf,__u8 *oldDataBuf, __u16 *realSubWidth)
{
	__u8 nextBits = 0;
	__u8 codeVal = 0;
	__u16 codeLen = 0;
	__u32 usedBits = 0;
	__u32 pixelNum = 0;
	__u8 *lastDataBuf = NULL;

	pixelLen = 0;
	lastDataBuf = newDataBuf;

	while(1)
	{
		nextBits = subdec_get_next_some_bits(oldDataBuf, &usedBits, 2);

		if(nextBits != 0)	//2 bit pixel code
		{
			newDataBuf[0] = nextBits;
			newDataBuf++;
		}
		else
		{	//switch 1
			nextBits = subdec_get_next_some_bits(oldDataBuf, &usedBits, 1);
			if(nextBits ==1)
			{
				codeLen = subdec_get_next_some_bits(oldDataBuf, &usedBits, 3) + 3;
				codeVal = subdec_get_next_some_bits(oldDataBuf, &usedBits, 2);
				memset(newDataBuf,codeVal,codeLen);
				newDataBuf += codeLen;
			}
			else
			{	//switch 2
				nextBits = subdec_get_next_some_bits(oldDataBuf, &usedBits, 1);
				if(nextBits == 1)
				{
					newDataBuf[0] = 0;
					newDataBuf++;
				}
				else
				{	//switch 3
					nextBits = subdec_get_next_some_bits(oldDataBuf, &usedBits, 2);
					if(nextBits == 0)
					{
						break;	// end of 2-bit/pixel_code string
					}
					else if(nextBits == 1)
					{
						memset(newDataBuf, 0, 2);
						newDataBuf += 2;
					}
					else if(nextBits == 2)
					{
						codeLen = subdec_get_next_some_bits(oldDataBuf, &usedBits, 4) + 12;
						codeVal = subdec_get_next_some_bits(oldDataBuf, &usedBits, 2);
						memset(newDataBuf, codeVal, codeLen);
						newDataBuf += codeLen;
					}
					else if(nextBits == 3)
					{
						codeLen = subdec_get_next_some_bits(oldDataBuf, &usedBits, 8) + 29;
						codeVal = subdec_get_next_some_bits(oldDataBuf, &usedBits, 2);
						memset(newDataBuf, codeVal, codeLen);
						newDataBuf += codeLen;
					}
				}
			}
		}
	}
	
	if(*realSubWidth < (newDataBuf - lastDataBuf))
		*realSubWidth = (newDataBuf - lastDataBuf);

	pixelLen = (newDataBuf - lastDataBuf);
	pixelNum = usedBits>>3;
	if((pixelNum<<3) != usedBits)
		pixelNum += 1;
	return pixelNum;
}

//*******************************************************************************//
//********************************************************************************//

__u32 subdec_4bit_pixel_code_string(__u8 *newDataBuf,__u8 *oldDataBuf, __u16 *realSubWidth)
{
	__u8 nextBits = 0;
	__u8 codeVal = 0;
	__u16 codeLen = 0;
	__u32 usedBits = 0;
	__u32 pixelNum = 0;
	__u8 *lastDataBuf = NULL;

	pixelLen = 0;
	lastDataBuf = newDataBuf;

	while(1)
	{
		nextBits = subdec_get_next_some_bits(oldDataBuf, &usedBits, 4);

		if(nextBits != 0)	//2 bit pixel code
		{
			newDataBuf[0] = nextBits;
			newDataBuf ++;
		}
		else
		{	//switch 1
			nextBits = subdec_get_next_some_bits(oldDataBuf, &usedBits, 1);
			if(nextBits == 0)
			{
				nextBits = subdec_get_next_some_bits(oldDataBuf, &usedBits, 3);
				if(nextBits != 0)
				{
					codeLen = nextBits + 2;
					codeVal = 0;
					memset(newDataBuf, codeVal, codeLen);
					newDataBuf += codeLen;
				}
				else	// end of string signal
				{
					break;
				}
			}
			else
			{	//switch 2
				nextBits = subdec_get_next_some_bits(oldDataBuf, &usedBits, 1);
				if(nextBits == 0)
				{
					codeLen = subdec_get_next_some_bits(oldDataBuf, &usedBits, 2)+4;
					codeVal = subdec_get_next_some_bits(oldDataBuf, &usedBits, 4);  
					memset(newDataBuf, codeVal, codeLen);
					newDataBuf += codeLen;
				}
				else
				{	// switch 3
					nextBits = subdec_get_next_some_bits(oldDataBuf, &usedBits, 2);
					if(nextBits == 0)
					{
						newDataBuf[0] = 0;
						newDataBuf ++;
					}
					else if(nextBits == 1)
					{
						memset(newDataBuf, 0, 2);
						newDataBuf += 2;
					}
					else if(nextBits == 2)
					{
						codeLen = subdec_get_next_some_bits(oldDataBuf, &usedBits, 4)+9;
						codeVal = subdec_get_next_some_bits(oldDataBuf, &usedBits, 4);  
						memset(newDataBuf, codeVal, codeLen);
						newDataBuf += codeLen;
					}
					else if(nextBits == 3)
					{
						codeLen = subdec_get_next_some_bits(oldDataBuf, &usedBits, 8)+25;
						codeVal = subdec_get_next_some_bits(oldDataBuf, &usedBits, 4);
						memset(newDataBuf, codeVal, codeLen);
						newDataBuf += codeLen;
					}
				}
			}
		}
	}

	if(*realSubWidth < (newDataBuf - lastDataBuf))
		*realSubWidth = (newDataBuf - lastDataBuf);
	pixelLen = (newDataBuf - lastDataBuf);

	pixelNum = usedBits>>3;
	if((pixelNum<<3) != usedBits)
		pixelNum += 1;
	return pixelNum;
}


//**************************************************************************************//
//*************************************************************************************//

__u32 subdec_8bit_pixel_code_string(__u8 *newDataBuf,__u8 *oldDataBuf, __u16 *realSubWidth)
{
	__u8 nextBits = 0;
	__u8 codeVal = 0;
	__u16 codeLen = 0;
	__u32 pixelNum = 0;
	__u32 usedBits = 0;
	__u8 *lastDataBuf = NULL;

	pixelLen = 0;
	lastDataBuf = newDataBuf;

	while(1)
	{
		nextBits = subdec_get_next_some_bits(oldDataBuf, &usedBits, 8);
		if(nextBits != 0)
		{
			newDataBuf[0] = nextBits;
			newDataBuf ++;
		}
		else
		{	// switch 1
			nextBits = subdec_get_next_some_bits(oldDataBuf, &usedBits, 1);
			if(nextBits == 0)
			{
				nextBits = subdec_get_next_some_bits(oldDataBuf, &usedBits, 7);
				if(nextBits != 0)
				{
					codeLen = nextBits;
					codeVal = 0;
					memset(newDataBuf, codeVal, codeLen);
					newDataBuf += codeLen;
				}
				else	// end of string signal
				{
					break;
				}
			}
			else
			{
				codeLen = subdec_get_next_some_bits(oldDataBuf, &usedBits, 7);
				codeVal = subdec_get_next_some_bits(oldDataBuf, &usedBits, 8);
				memset(newDataBuf, codeVal, codeLen);
				newDataBuf += codeLen;
			}
		}
	}
	if(*realSubWidth < (newDataBuf - lastDataBuf))
		*realSubWidth = (newDataBuf - lastDataBuf);

	pixelLen = (newDataBuf - lastDataBuf);
	pixelNum = usedBits>>3;
	if((pixelNum<<3) != usedBits)
		pixelNum += 1;
	return pixelNum;
}
//*************************************************************************************//
__u32 subdec_2bit_to_4bit_table(__u8 *newDataBuf, __u8 *oldDataBuf, __u16 *realSubWidth)
{
	__u8 nextBits = 0;
	__u32 pixelNum = 0;
	__u32 usedBits = 0;
	__u8 *lastDataBuf = NULL;

	lastDataBuf = newDataBuf;
	while(usedBits < 16)
	{
		nextBits = subdec_get_next_some_bits(oldDataBuf, &usedBits, 2);
		newDataBuf[0] = TWO_TO_FOUR_MAP_TABLE[nextBits];
		newDataBuf ++;
	}
	pixelNum = 2;
	if(*realSubWidth < (newDataBuf - lastDataBuf))
		*realSubWidth = (newDataBuf - lastDataBuf);
	return pixelNum;
}

//************************************************************************************//
__u32 subdec_2bit_to_8bit_table(__u8 *newDataBuf, __u8 *oldDataBuf,__u16 *realSubWidth)
{
	__u8 nextBits = 0;
	__u32 pixelNum = 0;
	__u32 usedBits = 0;
	__u8 *lastDataBuf = NULL;

	lastDataBuf = newDataBuf;
	while(usedBits < 32)
	{
		nextBits = subdec_get_next_some_bits(oldDataBuf, &usedBits, 2);
		newDataBuf[0] = TWO_TO_EIGHT_MAP_TABLE[nextBits];
		newDataBuf ++;
	}
	pixelNum = 4;
	if(*realSubWidth < (newDataBuf - lastDataBuf))
		*realSubWidth = (newDataBuf - lastDataBuf);
	return pixelNum;
}
//*************************************************************************************//
__u32 subdec_4bit_to_8bit_table(__u8 *newDataBuf, __u8 *oldDataBuf,__u16 *realSubWidth)
{
	__u8 nextBits = 0;
	__u32 pixelNum = 0;
	__u32 usedBits = 0;
	__u8 *lastDataBuf = NULL;

	lastDataBuf = newDataBuf;
	while(usedBits < 128)
	{
		nextBits = subdec_get_next_some_bits(oldDataBuf, &usedBits, 4);
		newDataBuf[0] = FOUR_TO_EIGHT_MAP_TABLE[nextBits];
		newDataBuf ++;
	}
	pixelNum = 16;
	if(*realSubWidth < (newDataBuf - lastDataBuf))
		*realSubWidth = (newDataBuf - lastDataBuf);
	return pixelNum;
}

//************************************************************************************//
//************************************************************************************//

void subdec_pixel_data_sub_parse(__u8 *newDataBuf,__u8 *oldDataBuf, __u16 dataLen, __u16 widthLen, __u16 heightLen, __u16 *realSubWidth)
{
	__u8 dataType = 0;
	__u8 LastType = 0;
	__u16 objectLineNum = 0;
	__u32 pixelNum = 0;
	__u8 *curDataPtr;
	__u16 i,line,height,objectLineWidth[480];

	memset(objectLineWidth, 0, 480);
	
	while((dataLen > 0) && (objectLineNum < heightLen))
	{
		curDataPtr = newDataBuf + objectLineNum*widthLen;
		dataType = oldDataBuf[0];
		oldDataBuf += 1;
		dataLen -= 1;
		switch(dataType)
		{
			case 0x10:       //2-bit/pixel code string
				pixelNum = subdec_2bit_pixel_code_string(curDataPtr,oldDataBuf, realSubWidth);
				if (objectLineNum < 480)
					objectLineWidth[objectLineNum] = pixelLen;
				oldDataBuf += pixelNum;
				objectLineNum += 2;
				LastType = 0x10;
				break;
			case 0x11:       //4-bit/pixel code string
				pixelNum = subdec_4bit_pixel_code_string(curDataPtr,oldDataBuf, realSubWidth);
				if (objectLineNum < 480)
					objectLineWidth[objectLineNum] = pixelLen;
				oldDataBuf += pixelNum;
				objectLineNum += 2;
				LastType = 0x11;
				break;
			case 0x12:       //8-bit/pixel code string
				pixelNum = subdec_8bit_pixel_code_string(curDataPtr,oldDataBuf,realSubWidth);
				if (objectLineNum < 480)
					objectLineWidth[objectLineNum] = pixelLen;
				oldDataBuf += pixelNum;
				objectLineNum += 2;
				LastType = 0x12;
				break;
			case 0x20:       //2-to 4-bit map table data
				pixelNum = subdec_2bit_to_4bit_table(newDataBuf,oldDataBuf, realSubWidth);
				oldDataBuf += pixelNum;
				objectLineNum += 2;
				break; 
			case 0x21:       //2-to 8-bit map table data
				pixelNum = subdec_2bit_to_8bit_table(curDataPtr,oldDataBuf,realSubWidth);
				oldDataBuf += pixelNum;
				objectLineNum += 2;
				break;
			case 0x22:       //4-to 8-bit map table data
				pixelNum = subdec_4bit_to_8bit_table(curDataPtr,oldDataBuf,realSubWidth);
				oldDataBuf += pixelNum;
				objectLineNum += 2;
				break;
			case 0xf0:       //end of object line code
				break;
			default:
				break;
		}
		dataLen -= pixelNum;
	}
	if (*realSubWidth)
	{
		if (LastType >= 0x10 && LastType <= 0x12)
		{
			__u8 *dstDataPtr, *srcDataPtr;
			
			if (objectLineWidth[0] == *realSubWidth)
				return;	
			
			height = heightLen;
			if (height > 480)
				height = 480;
			line = 0;
			for (i=0; i<height; i++)
			{
				if (objectLineWidth[i] == *realSubWidth)
				{
					line = i;
					break;
				}
			}
			if (i == height)//所有行像素相等
				return;
//-----------------------------------------------------------------------------
		#if 0//20130411
			for (i=0; i<height; i++)
			{
				if (objectLineWidth[i] > 0 && objectLineWidth[i] < *realSubWidth)
				{
					dstDataPtr = newDataBuf + objectLineWidth[i] + i*widthLen;
					srcDataPtr = newDataBuf + objectLineWidth[i] + line*widthLen;
					memcpy(dstDataPtr, srcDataPtr, *realSubWidth - objectLineWidth[i]);
				}
			}
		#else//补齐第一行白边
			dstDataPtr = newDataBuf + objectLineWidth[0];
			srcDataPtr = newDataBuf + objectLineWidth[0] + line*widthLen;
			memcpy(dstDataPtr, srcDataPtr, *realSubWidth - objectLineWidth[0]);
		#endif
//-----------------------------------------------------------------------------
		}
	}
}

