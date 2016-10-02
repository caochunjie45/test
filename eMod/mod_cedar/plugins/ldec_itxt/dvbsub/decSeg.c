#if PLAY_DVBT_DVBSUBTITLE

#include "subdec.h"


extern void show_picture(__u8 *picBuf, __u16 picSizex,__u16 picSizey, __u32 *palette);


//*************************************************************************************************//
//******************func1 subdec_find_startcode() ************************************************//

static __u16 subSize_2power[11] = {1,2,4,8,16,32,64,128,256,512,1024};

static __u32  dvbsub_default_TRGB[256] = {
  
	0x00000000, 0xff000000, 0xff0000a9, 0xff00017d, 0xff00aa00, 0xff007f00, 0xff01aba8, 0xff00807e,
    0xffa90000, 0xff7d0000, 0xffa901a9, 0xff7e017e, 0xffa8aa01, 0xff7e7f00, 0xffaaaaaa, 0xff7f7f7f,
	0x3f000000, 0xff000000, 0x3f0000a9, 0xff000053, 0x3f00aa00, 0xff005501, 0x3f01aba8, 0xff005452, 
	0x3fa90000, 0xff530000, 0x3fa901a9, 0xff530051, 0x3fa8aa01, 0xff525501, 0x3faaaaaa, 0xff545454,
	0x3f0001fe, 0xff00013d, 0xff0001fe, 0xff0000bd, 0xff01aa54, 0xff007f3f, 0xff00aafc, 0xff007fbd,
	0xffa90153, 0xff7e013e, 0xffa801fc, 0xff7c01be, 0xffa9aa55, 0xff7e7f40, 0xffaaaafd, 0xff7e7fbe,
	0x3f000053, 0xff000029, 0x3f0001fe, 0xff00017d, 0x3f01aa54, 0xff00552b, 0x3f00aafc, 0xff00547d, 
    0x3fa90153, 0xff520027, 0x3fa801fc, 0xff52017d, 0x3fa9aa55, 0xff53542b, 0x3faaaafd, 0xff53547d,
	0x3f00fe00, 0xff003e00, 0xff0055a9, 0xff003f7d, 0xff00fd00, 0xff01bf00, 0xff00fea8, 0xff00bf7e,
    0xffa85500, 0xff7d3f00, 0xffa855a8, 0xff7d407e, 0xffa9fe00, 0xff7fbf01, 0xffa9ffab, 0xff7fbf7e,
    0x3f005501, 0xff002a00, 0x3f0055a9, 0xff002b53, 0x3f00fd00, 0xff007f00, 0x3f00fea8, 0xff017f54,
	0x3fa85500, 0xff522a00, 0x3fa855a8, 0xff522b52, 0x3fa9fe00, 0xff537f00, 0x3fa9ffab, 0xff557f54,
    0x3f00fffe, 0xff00403d, 0xff0054fd, 0xff003fbd, 0xff00fe53, 0xff00bf40, 0xff01fffc, 0xff00bebd,
    0xffa95454, 0xff7e4040, 0xffa955fd, 0xff7d3fbd, 0xffa9ff54, 0xff7fc03f, 0xffaafefc, 0xff7fbfbc,
    0x3f005452, 0xff002a29, 0x3f0054fd, 0xff012b7d, 0x3f00fe53, 0xff008029, 0x3f01fffc, 0xff00807e,
	0x3fa95454, 0xff532a29, 0x3fa955fd, 0xff532a7e, 0x3fa9ff54, 0xff548029, 0x3faafefc, 0xff54807e,
    0x3ffe0100, 0xff3d0000, 0xff5300a8, 0xff3e017d, 0xff53aa00, 0xff3f7f00, 0xff55aba8, 0xff3f7f7c,
    0xfffd0000, 0xffbe0100, 0xfffd01a9, 0xffbe017d, 0xfffcab00, 0xffbd7f00, 0xfffdaaaa, 0xffbe8080,
    0x3f530000, 0xff290000, 0x3f5300a8, 0xff290152, 0x3f53aa00, 0xff295400, 0x3f55aba8, 0xff2a5453,
    0x3ffd0000, 0xff7d0000, 0x3ffd01a9, 0xff7d0152, 0x3ffcab00, 0xff7e5400, 0x3ffdaaaa, 0xff7d5453,
    0x3ffd02fd, 0xff3e023d, 0xff5301fc, 0xff3d02bc, 0xff54aa54, 0xff3f7f3e, 0xff54aafc, 0xff3f80bd,
    0xfffd0055, 0xffbe0040, 0xfffd02fd, 0xffbc01bd, 0xfffdaa53, 0xffbd7f3f, 0xfffdaafb, 0xffbd80be,
    0x3f530051, 0xff280128, 0x3f5301fc, 0xff29017e, 0x3f54aa54, 0xff2a542a, 0x3f54aafc, 0xff2a557d,
    0x3ffd0055, 0xff7e002a, 0x3ffd02fd, 0xff7e017e, 0x3ffdaa53, 0xff7d552a, 0x3ffdaafb, 0xff7c557d,
    0x3ffdff00, 0xff3e3f00, 0xff5355a9, 0xff3e3f7e, 0xff54fe00, 0xff3ebf00, 0xff53fea9, 0xff3fbf7f,
	0xfffc5500, 0xffbe4000, 0xfffb55a8, 0xffbd407d, 0xfffcfe00, 0xffbec000, 0xfffcffab, 0xffbebf80,
    0x3f525501, 0xff292a01, 0x3f5355a9, 0xff292a53, 0x3f54fe00, 0xff2a7f00, 0x3f53fea9, 0xff2a7f53,
    0x3ffc5500, 0xff7d2b00, 0x3ffb55a8, 0xff7e2b52, 0x3ffcfe00, 0xff7e7f00, 0x3ffcffab, 0xff7e8054,
    0x3fffffff, 0xff3f3f3f, 0xff5454fd, 0xff3e3fbe, 0xff53fe53, 0xff3fbf3f, 0xff55fffc, 0xff3fbfbd,
    0xfffd5454, 0xffbe403f, 0xfffd55fd, 0xffbd40bd, 0xfffcfe55, 0xffbebf40, 0xfffefefe, 0xffbfbfbf,
    0x3f545454, 0xff2a2a2a, 0x3f5454fd, 0xff2a2a7d, 0x3f53fe53, 0xff297f29, 0x3f55fffc, 0xff2a807d,
    0x3ffd5454, 0xff7d2a2a, 0x3ffd55fd, 0xff7e2b7e, 0x3ffcfe55, 0xff7d802a, 0x3ffefefe, 0xff7f7f7f
};



void subdec_clear_used_flag(__ldec_idvbsub_t *ldec_idvb)
{
	__u16 i = 0;
	__u16 j = 0;

	__ldec_info_t *subdecInfo = NULL;
	subdecInfo = (__ldec_info_t *)ldec_idvb->privInfo;

	for(i=0; i<LDEC_REGION_NUM; i++)
		subdecInfo->subdecPage->pageRegion[i].hasUsedFlag = 0;

	for(i=0; i<LDEC_CLUT_NUM; i++)
	{
		subdecInfo->subdecClut[i].hasUsedFlag= 0;
		for(j=0; j<LDEC_CLUTENTRY_NUM; j++)
			subdecInfo->subdecClut[i].clutEntry[j].hasUsedFlag = 0;
	}
	for(i=0; i<LDEC_REGION_NUM; i++)
	{
		subdecInfo->subdecRegion[i].hasUsedFlag = 0;
		for(j=0; j<LDEC_OBJECT_NUM; j++)
			subdecInfo->subdecRegion[i].regionObject[j].hasUsedFlag = 0;
	}
}


__s8 subdec_find_startcode(__ldec_lbs_buf_t *subdecBuf, __u8 fstVal, __u8 secVal, __u8 checkSecFlag)
{
	__u32  utmp_used = 0;
	__u8   *utmp_read = 0;
	
	utmp_read = subdecBuf->readAddr;
	
	while(utmp_used <= subdecBuf->validSize)
	{
		if(utmp_read[0] == fstVal)
		{
			if(checkSecFlag == 0)
			{
				utmp_used  += 1;
				ldec_update_read_pointer(subdecBuf, utmp_used);
				return 0;
			}
			else
			{
				if(utmp_read == subdecBuf->ldecBufEnd)
					utmp_read = subdecBuf->ldecBuf;
				else
					utmp_read ++;
				if(utmp_read[0] == secVal)
				{
					utmp_used  += 2;;
					ldec_update_read_pointer(subdecBuf, utmp_used);
					return 0;
				}
			}
		}

		utmp_read ++;
		utmp_used ++;
		if(utmp_read == subdecBuf->ldecBufEnd)
			utmp_read = subdecBuf->ldecBuf;
	}

	ldec_update_read_pointer(subdecBuf, utmp_used);
	return -1;
}

//*************************************************************************************************//
//******************func2 subdec_check_buffer_boundary() *****************************************//

__u8 *subdec_check_buffer_boundary(__ldec_idvbsub_t *ldec_idvb, __u32 dataLen)
{
	__ldec_lbs_buf_t  *subdecBuf;
	__ldec_info_t    *subdecInfo;
	__u32 remainData = 0;

	subdecBuf = &(ldec_idvb->ldecLbsBuf);
	subdecInfo   = (__ldec_info_t  *)ldec_idvb->privInfo;

	remainData = subdecBuf->ldecBufEnd - subdecBuf->readAddr+1;
	if(remainData >= dataLen)
		return subdecBuf->readAddr;
	else
	{
		memcpy(subdecInfo->anciDataBuf,subdecBuf->readAddr, remainData);
		memcpy(subdecInfo->anciDataBuf+remainData,subdecBuf->ldecBuf, dataLen-remainData);
		return subdecInfo->anciDataBuf;
	}
}


//*************************************************************************************************//
//******************func3 subdec_check_index() ****************************************************//

__s16 subdec_check_index(__ldec_info_t *subdecInfo, __u8 indexType, __s16 refIndex)
{
	__s16 i = 0;

	switch(indexType)
	{
		case LDEC_PAGE_REGION_INDEX:
			while(i < LDEC_REGION_NUM)
			{
				if(subdecInfo->subdecPage->pageRegion[i].hasUsedFlag == 0)
					break;
				i++;
			}
			if(i==LDEC_REGION_NUM)
			{
				WARNING("here0: Cannot find a suitable index\n");
				i = 0;    //强制性的将index置为0
			}
			break;
		case LDEC_REGION_OBJECT_INDEX:
			while(i < LDEC_OBJECT_NUM)
			{
				if(subdecInfo->subdecRegion[refIndex].regionObject[i].hasUsedFlag == 0)
					break;
				i++;
			}
			if(i==LDEC_OBJECT_NUM)
			{
				WARNING("here1: Cannot find a suitable index\n");
				i = 0;    //强制性的将index置为0
			}	
			break;
		case LDEC_CLUT_INDEX:
			while(i < LDEC_CLUT_NUM)
			{
				if(subdecInfo->subdecClut[i].hasUsedFlag == 0)
					break;
				i++;
			}
			if(i==LDEC_CLUT_NUM)
			{
				WARNING("here4: Cannot find a suitable index\n");
				i = 0;    //强制性的将index置为0
			}
			break;
		case LDEC_REGION_INDEX:
			while(i < LDEC_REGION_NUM)
			{
				if(subdecInfo->subdecRegion[i].hasUsedFlag == 0)
					break;
				i++;
			}
			if(i==LDEC_REGION_NUM)
			{
				WARNING("here5: Cannot find a suitable index\n");
				i = 0;    //强制性的将index置为0
			}
			break;
		default:
			break;
	}
	return i;
}

//*************************************************************************************************//
//******************func4 dvbsub_page_comp_seg_parse() *********************************************//

__s8 dvbsub_page_comp_seg_parse(__ldec_idvbsub_t *ldec_idvb, __u32 segmentLen)
{
	__ldec_lbs_buf_t  *subdecBuf;
	__ldec_info_t    *subdecInfo;
	__ldec_page_t    *pSubPage;
	__u8             *curDataPtr;
	__u32            oldSegLen = 0;
	__s16            regionIndex = 0;
	__s16            pageIndex = 0;
	__u16            i;

	subdecBuf = &(ldec_idvb->ldecLbsBuf);
	subdecInfo   = (__ldec_info_t  *)ldec_idvb->privInfo;
	pSubPage   = subdecInfo->subdecPage;
	oldSegLen = segmentLen;
	pSubPage->newPageFlag = 0;
	ldec_update_read_pointer(subdecBuf,5);

	//detect page value
	curDataPtr = subdec_check_buffer_boundary(ldec_idvb, segmentLen);
	pSubPage->pageTimeOut = curDataPtr[0]*1000;
	pSubPage->pageVersion = (curDataPtr[1] >> 4);
	pSubPage->pageState   = (__ldec_page_status_t)((curDataPtr[1]&0x0c)>>2);

	if((pSubPage->pageState == LDEC_PAGE_ACQUISITON_POINT) 
			|| (pSubPage->pageState == LDEC_PAGE_MODE_CHANGE))
	{
		subdec_clear_used_flag(ldec_idvb);
		subdecInfo->lastClutIdx = -1;
	}
/*	if(pSubPage->pageState != LDEC_PAGE_NORMAL_STATUS)      //需要修改
	{  
	    WARNING("pSubPage->pageState is not equal to LDEC_PAGE_NORMAL_STATUS\n");
        ldec_update_read_pointer(subdecBuf,segmentLen);
		pSubPage->newPageFlag = 0;
        return 0;
	}*/

	pSubPage->pageRegionNum = (segmentLen-2)/6;
	curDataPtr+= 2;
	segmentLen -= 2;

	//-----------------------------------------------------
	// PES Packet data error, For Protect!
	//-----------------------------------------------------
	if (pSubPage->pageRegionNum > LDEC_REGION_NUM)
	{
		pSubPage->pageRegionNum = 0;
		return -1;
	}

	for(i=0; i < pSubPage->pageRegionNum; i++)
	{
		regionIndex = subdec_check_index(subdecInfo, LDEC_PAGE_REGION_INDEX, pageIndex);
		pSubPage->pageRegion[regionIndex].regionId = curDataPtr[0];
		pSubPage->pageRegion[regionIndex].regionHorAddr = (curDataPtr[2]<<8)|curDataPtr[3];
		pSubPage->pageRegion[regionIndex].regionVerAddr = (curDataPtr[4]<<8)|curDataPtr[5];	 
		if(pSubPage->pageRegion[regionIndex].regionHorAddr >= 720)
			break;
		else if(pSubPage->pageRegion[regionIndex].regionVerAddr >= 576)
			break;
		curDataPtr += 6;
		segmentLen -= 6;
		pSubPage->pageRegion[regionIndex].hasUsedFlag = 1;
		pSubPage->newPageFlag = 1;
	}
	ldec_update_read_pointer(subdecBuf,oldSegLen);
	return 	pSubPage->newPageFlag;
}

//*************************************************************************************************//
//******************func5 dvbsub_region_comp_seg_parse() *********************************************//

__s8 dvbsub_region_comp_seg_parse(__ldec_idvbsub_t *ldec_idvb, __u32 segmentLen)
{
	__ldec_lbs_buf_t  *subdecBuf;
	__ldec_info_t    *subdecInfo;
	__ldec_region_t   *pSubRegion;
	__u8             *curDataPtr;
	__s16            objectIndex = 0;
	__s16            regionIndex = 0;
	__u32            orgSegLen = 0;
	__u16            i, regions_length;

	subdecBuf = &(ldec_idvb->ldecLbsBuf);
	subdecInfo   = (__ldec_info_t  *)ldec_idvb->privInfo;
	pSubRegion   = subdecInfo->subdecRegion;
	orgSegLen = segmentLen;

	ldec_update_read_pointer(subdecBuf,5);

	if(segmentLen <= 10)	// there is no object info
	{
		ldec_update_read_pointer(subdecBuf,orgSegLen);
		return 0;
	}

	curDataPtr = subdec_check_buffer_boundary(ldec_idvb, segmentLen);
	regionIndex = subdec_check_index(subdecInfo, LDEC_REGION_INDEX, 0);

	pSubRegion[regionIndex].regionId = curDataPtr[0];
	pSubRegion[regionIndex].regionVersion = curDataPtr[1]>>4;
	pSubRegion[regionIndex].regionWidth = (curDataPtr[2]<<8)|curDataPtr[3];
	pSubRegion[regionIndex].regionHeight = (curDataPtr[4]<<8)|curDataPtr[5];
	pSubRegion[regionIndex].regionLevel  = curDataPtr[6]>>5;
	pSubRegion[regionIndex].regionDepth  = curDataPtr[6]>>2;
	pSubRegion[regionIndex].regionClutId = curDataPtr[7];
	if ((pSubRegion[regionIndex].regionWidth == 0) || (pSubRegion[regionIndex].regionHeight == 0))
	{
		return -1;
	}

	curDataPtr += 10;
	segmentLen -= 10;

	pSubRegion[regionIndex].hasUsedFlag = 1;

	if((pSubRegion[regionIndex].regionObject[objectIndex].objectType == 0x01)||
		(pSubRegion[regionIndex].regionObject[objectIndex].objectType == 0x02))
	{
		regions_length = segmentLen/8;
	}
	else {
		regions_length = segmentLen/6;
	}
	//-----------------------------------------------------
	// PES Packet data error, For Protect!
	//-----------------------------------------------------
	if (regions_length > LDEC_OBJECT_NUM)
	{
		regions_length = 0;
		return -1;
	}
	
	for (i=0; i < regions_length; i++)	
	{
		objectIndex = subdec_check_index(subdecInfo, LDEC_REGION_OBJECT_INDEX,regionIndex);
		if(objectIndex >= 0)
		{
			pSubRegion[regionIndex].regionObject[objectIndex].objectId = (curDataPtr[0]<<8) | curDataPtr[1];
			pSubRegion[regionIndex].regionObject[objectIndex].objectType = curDataPtr[2]>>6;
			pSubRegion[regionIndex].regionObject[objectIndex].objectHorPos = ((curDataPtr[2]&0x0f)<<8) | curDataPtr[3];
			pSubRegion[regionIndex].regionObject[objectIndex].objectVerPos = ((curDataPtr[4]&0x0f)<<8) | curDataPtr[5];
			curDataPtr += 6;
			segmentLen -= 6;
			if((pSubRegion[regionIndex].regionObject[objectIndex].objectType == 0x01)||
				(pSubRegion[regionIndex].regionObject[objectIndex].objectType == 0x02))
			{
				curDataPtr += 2;
				segmentLen -= 2;
			}
			pSubRegion[regionIndex].regionObject[objectIndex].hasUsedFlag = 1;
		}
	}
	ldec_update_read_pointer(subdecBuf,orgSegLen);
	return 0;
}

//*************************************************************************************************//
//******************func6 dvbsub_clut_def_seg_parse() *********************************************//

__s8 dvbsub_clut_def_seg_parse(__ldec_idvbsub_t *ldec_idvb, __u32 segmentLen)
{
	__ldec_lbs_buf_t  *subdecBuf;
	__ldec_info_t    *subdecInfo;
	__ldec_clut_t   *pSubClut;
	__u8             *curDataPtr;
	__u8             fullRangeFlag;
	__s16            entryIndex = 0;
	__s16            clutIndex = 0;
	__u32            orgSegLen = 0;
	__u16            i=0, cluts_length=0;

	subdecBuf = &(ldec_idvb->ldecLbsBuf);
	subdecInfo   = (__ldec_info_t  *)ldec_idvb->privInfo;
	pSubClut   = subdecInfo->subdecClut;
	
	ldec_update_read_pointer(subdecBuf,5);
	curDataPtr = subdec_check_buffer_boundary(ldec_idvb, segmentLen);
	clutIndex = subdec_check_index(subdecInfo, LDEC_CLUT_INDEX, 0);
	if (segmentLen >= 4)
	{
		if (curDataPtr[3] & 0x01)
			i = 8;
		else
			i = 6;
	}
	if (segmentLen >= i)
	{
		pSubClut[clutIndex].clutId = curDataPtr[0];
		pSubClut[clutIndex].clutVerion = curDataPtr[1]>>4;
		orgSegLen = segmentLen;
		segmentLen -= 2;
		curDataPtr += 2;
		pSubClut[clutIndex].hasUsedFlag = 1;
		i = 0;
		while(i+2 < segmentLen)
		{
			cluts_length++;
			if (curDataPtr[i+1] & 0x01)
				i += 6;
			else
				i += 4;
			if (i > segmentLen)
			{
				cluts_length--;
				break;
			}
		}
	}
	//-----------------------------------------------------
	// PES Packet data error, For Protect!
	//-----------------------------------------------------
	if (cluts_length > LDEC_CLUTENTRY_NUM)
	{
		cluts_length = 0;
		return -1;
	}

	for (i=0; i < cluts_length; i++)
	{
		entryIndex = curDataPtr[0];
		pSubClut[clutIndex].clutEntry[entryIndex].entryId = curDataPtr[0];
		pSubClut[clutIndex].clutEntry[entryIndex].twoEntryFlag = (curDataPtr[1]>>7)&0x01;
		pSubClut[clutIndex].clutEntry[entryIndex].fourEntryFlag = (curDataPtr[1]>>6)&0x01;
		pSubClut[clutIndex].clutEntry[entryIndex].eightEntryFlag = (curDataPtr[1]>>5)&0x01;
		fullRangeFlag = curDataPtr[1] & 0x01;
		curDataPtr += 2;
		segmentLen -= 2;

		if(fullRangeFlag == 1)
		{
			pSubClut[clutIndex].clutEntry[entryIndex].yValue  = curDataPtr[0];
			pSubClut[clutIndex].clutEntry[entryIndex].crValue = curDataPtr[1];
			pSubClut[clutIndex].clutEntry[entryIndex].cbValue = curDataPtr[2];
			pSubClut[clutIndex].clutEntry[entryIndex].tValue  = curDataPtr[3];
			curDataPtr += 4;
			segmentLen -= 4;
		}
		else
		{
			pSubClut[clutIndex].clutEntry[entryIndex].yValue  = curDataPtr[0] >> 2;
			pSubClut[clutIndex].clutEntry[entryIndex].crValue = ((curDataPtr[0]&0x3)<<2)|(curDataPtr[1]>>6);
			pSubClut[clutIndex].clutEntry[entryIndex].cbValue = (curDataPtr[1]>>2)&0x0f;
			pSubClut[clutIndex].clutEntry[entryIndex].tValue  = curDataPtr[1] & 0x3;
			curDataPtr += 2;
			segmentLen -= 2;
		}
		pSubClut[clutIndex].clutEntry[entryIndex].hasUsedFlag = 1;
	}
	ldec_update_read_pointer(subdecBuf,orgSegLen);
	return 0;
}

//*************************************************************************************************//
//******************func7 dvbsub_object_def_seg_parse() *********************************************//

__s8 subdec_calculate_object_info(__ldec_idvbsub_t *ldec_idvb)
{
	__u8   regionIndex = 0;
	__u8   regionObjectIndex = 0;
	__u8   pageRegionIndex = 0;
	__u8   objectClutIndex = 0;
	__u8   regionId = 0;
	__u8 findObjectIndexFlag = 0;
	__u8 findRegionIndexFlag = 0;
	__ldec_lbs_buf_t  *subdecBuf = NULL;
	__ldec_info_t    *subdecInfo = NULL;
	__ldec_object_t   *subObject = NULL;
	__ldec_region_t *curRegion = NULL;

	subdecBuf = &(ldec_idvb->ldecLbsBuf);
	subdecInfo   = (__ldec_info_t  *)ldec_idvb->privInfo;
	subObject   = subdecInfo->subdecObject;

	//检测object的pts
	if(subdecBuf->ldecBufCrl[subdecBuf->ldecReadIdx].hasPtsFlag)
	{
		subObject->ptsValid = 1;
		subObject->ptsVal = subdecBuf->ldecBufCrl[subdecBuf->ldecReadIdx].ptsVal;
//		WARNING("object pts=%x.\n",(__u32) subObject->ptsVal);
	}
	else
	{
		subObject->ptsValid = 0;
		subObject->ptsVal = 0;
//		WARNING("The object pts is 0.\n");
	}

	while(regionIndex < LDEC_REGION_NUM)
	{
		curRegion = &(subdecInfo->subdecRegion[regionIndex]);
		if(curRegion->hasUsedFlag == 1)
		{
			while(regionObjectIndex < LDEC_OBJECT_NUM)
			{
				if(curRegion->regionObject[regionObjectIndex].objectId == subObject->objectId)
				{
					findObjectIndexFlag = 1;
					regionId = subdecInfo->subdecRegion[regionIndex].regionId;
					break;
				}
				regionObjectIndex++;
			}
		}

		if(findObjectIndexFlag == 1)
			break;
		else
		{
			regionIndex ++;
			regionObjectIndex = 0;
		}
	}

	if(findObjectIndexFlag == 0)
		return -1;
	else
	{
		while(pageRegionIndex < LDEC_REGION_NUM)
		{
			if(subdecInfo->subdecPage->pageRegion[pageRegionIndex].regionId == regionId)
			{
				findRegionIndexFlag = 1;
				break;
			}
			pageRegionIndex ++;
		}
	}

	if(findRegionIndexFlag == 0)
		return -1;

	while(objectClutIndex < LDEC_CLUT_NUM)
	{
		if(subdecInfo->subdecClut[objectClutIndex].hasUsedFlag)
		{
			if(subdecInfo->subdecClut[objectClutIndex].clutId == subdecInfo->subdecRegion[regionIndex].regionClutId)
				break;
		}
		objectClutIndex ++;
	}

	if(objectClutIndex == LDEC_CLUT_NUM)
		subdecInfo->subdecObject->clutIdx = -1;
	else
		subdecInfo->subdecObject->clutIdx = objectClutIndex;
	subdecInfo->subdecObject->pageRegionIdx   = pageRegionIndex;
	subdecInfo->subdecObject->regionIdx       = regionIndex;
	subdecInfo->subdecObject->regionObjectIdx = regionObjectIndex;

	subdecInfo->subdecObject->objectWidth = subdecInfo->subdecRegion[regionIndex].regionWidth - subdecInfo->subdecRegion[regionIndex].regionObject[regionObjectIndex].objectHorPos;
	subdecInfo->subdecObject->objectHeight = subdecInfo->subdecRegion[regionIndex].regionHeight - subdecInfo->subdecRegion[regionIndex].regionObject[regionObjectIndex].objectVerPos;

	subdecInfo->subdecObject->objectStartX    = subdecInfo->subdecPage->pageRegion[pageRegionIndex].regionHorAddr +subdecInfo->subdecRegion[regionIndex].regionObject[regionObjectIndex].objectHorPos;
	subdecInfo->subdecObject->objectStartY    = subdecInfo->subdecPage->pageRegion[pageRegionIndex].regionVerAddr + subdecInfo->subdecRegion[regionIndex].regionObject[regionObjectIndex].objectVerPos;
	return 0;
}

__s8 dvbsub_object_def_seg_parse(__ldec_idvbsub_t *ldec_idvb, __u32 segmentLen)
{
	__ldec_lbs_buf_t  *subdecBuf;
	__ldec_info_t    *subdecInfo;
	__ldec_object_t   *subObject;
	__Ldec_manager_t *subdecManager;
	__u8             *curDataPtr;
	__u16            i = 0;
	__u16            widthLen = 0;
	__u16            heightLen = 0;
	__u16            topBlockLen = 0;
	__u16            bottomBlockLen = 0;
	__u8             *decDataPtr = NULL;
	__u32            orgSegLen = 0;
	
	subdecManager = &(ldec_idvb->ldecManager);
	subdecBuf = &(ldec_idvb->ldecLbsBuf);
	subdecInfo   = (__ldec_info_t  *)ldec_idvb->privInfo;
	subObject   = subdecInfo->subdecObject;
	orgSegLen = segmentLen;

	if(subdecManager->subItem[subdecManager->reconBufIdx].isFull == 1)
	{
		ldec_update_read_pointer(subdecBuf,orgSegLen);
		return -1;
	}

	ldec_update_read_pointer(subdecBuf,5);
	curDataPtr = subdec_check_buffer_boundary(ldec_idvb, segmentLen);

	subObject->objectId = (curDataPtr[0]<<8) | curDataPtr[1];
	subObject->objectVersion = curDataPtr[2] >> 4;
	subObject->objectCodeType = (curDataPtr[2] & 0x0f) >> 2;
	subObject->nonModifyFlag = (curDataPtr[2] & 0x03) >> 1;

	subdec_calculate_object_info(ldec_idvb);
	memset(ldec_idvb->subIdxBuf, 0, LDEC_MAX_LBS_SUB_SIZE);

	if(subObject->objectCodeType == LDEC_CODE_STRING)  // coded as a string of characters
	{
		memcpy(ldec_idvb->subIdxBuf, curDataPtr+4,2*curDataPtr[3]);	//case1: process
		subObject->subLen = 2*curDataPtr[3];
	}
	else if(subObject->objectCodeType == LDEC_CODE_PIXEL) //coding of pixels    //case2: process
	{
		topBlockLen = (curDataPtr[3]<<8)|curDataPtr[4];
		bottomBlockLen = (curDataPtr[5]<<8)|curDataPtr[6];
		curDataPtr += 7;
		segmentLen -= 7;
		decDataPtr = ldec_idvb->subIdxBuf;
		memset(decDataPtr, 0, sizeof(__u8) *LDEC_MAX_PALTTE_NUM);
		widthLen  = subdecInfo->subdecObject->objectWidth;
		heightLen = subdecInfo->subdecObject->objectHeight;
		subdecInfo->subdecObject->realSubWidth = 0;
		//-----------------------------------------------------
		// Check Object data length, For Protect!
		//-----------------------------------------------------
		i = topBlockLen;
		if(bottomBlockLen != 0)
			i += bottomBlockLen;
		else
			i += topBlockLen;
		if(i > segmentLen)
		{
			return -2;
		}
		
		subdec_pixel_data_sub_parse(decDataPtr,curDataPtr, topBlockLen, widthLen, heightLen, &subdecInfo->subdecObject->realSubWidth);
		if(bottomBlockLen != 0)
		{
			curDataPtr += topBlockLen;
			decDataPtr += widthLen;
			subdec_pixel_data_sub_parse(decDataPtr,curDataPtr, bottomBlockLen, widthLen, heightLen, &subdecInfo->subdecObject->realSubWidth);
		}
		else	//The top field datas are same with the bottom field datas.
		{
			for(i=0; i<heightLen; i+=2)
			{
				memcpy(decDataPtr+widthLen, decDataPtr, widthLen);
				decDataPtr += 2*widthLen;
			}
		}
	}

	ldec_update_read_pointer(subdecBuf,orgSegLen);
	return 0;
 }

//*************************************************************************************************//
//******************func8 subdec_select_frame_index() *********************************************//

__s8 subdec_select_frame_index(__ldec_idvbsub_t *ldec_idvb)
{
	__u8 i = 0;
	__Ldec_manager_t *subdecManager;
	
	subdecManager = &(ldec_idvb->ldecManager);
 

	i = subdecManager->reconBufIdx;

	while(i < LDEC_MAX_LBS_SUB_NUM)
	{
		if(subdecManager->subItem[i].isFull == 0)
			break;
		i++;
	}
	if(i == LDEC_MAX_LBS_SUB_NUM)
	{
		for(i =0; i<LDEC_MAX_LBS_SUB_NUM; i++)
		{
			if(subdecManager->subItem[i].isFull == 0)
				break;
			i++;
		}
		if(i==LDEC_MAX_LBS_SUB_NUM)
			return -1;
	}
	return i;
}

//*************************************************************************************************//
//******************func9 dvbsub_put_sub_output() *********************************************//

__u32 subdec_change_yuv_to_rgb(__u8 aVal, __u8 yuv_y,__u8 yuv_u, __u8 yuv_v)
{
	__s32 u, v, rdif, invgdif, bdif, r, g, b;
	__u32 argbVal = 0;

	u = yuv_u - 128;
	v = yuv_v - 128;
	
	rdif = v + ((v * 103) >> 8);
	invgdif = ((u * 88) >> 8) +((v * 183) >> 8);
	bdif = u +( (u*198) >> 8);
	
	r = yuv_y + rdif;
	g = yuv_y - invgdif;
	b = yuv_y + bdif;
	
	r = (r > 255)? 255 : r;
	r = (r < 0)?     0 : r;
	g = (g > 255)? 255 : g;
	g = (g < 0) ?    0 : g;
	b = (b > 255)? 255 : b;
	b = (b < 0)?     0 : b;
	
	argbVal = (((__u8)aVal)<<24)|(((__u8)r)<<16)|(((__u8)g)<<8)|((__u8)b);
	return argbVal;
}

__s8 subdec_process_object_data(__ldec_idvbsub_t *ldec_idvb)
{
	__u32 i = 0;
	__u32 j = 0;
	__u16 maxEntryNum = 0;
	__u8  val = 0;
	__s8  clutIndex;
	__u8  frameIdx;
	__u8  *orgDataPtr;
	__u32  *curDataPtr;
	__u32  subDataSize;
	__u8  yVal, uVal, vVal, aVal;
	
	__ldec_info_t     *subdecInfo;
	__ldec_object_t   *subdecObject;
	__Ldec_manager_t  *subdecManager;

	subdecManager = &(ldec_idvb->ldecManager);
	subdecInfo  = (__ldec_info_t *)ldec_idvb->privInfo;
	subdecObject = subdecInfo->subdecObject;
	frameIdx      = subdecManager->reconBufIdx;
	clutIndex     = subdecObject->clutIdx;
	
	subdecManager->subItem[frameIdx].ptsVal = subdecObject->ptsVal;
	subdecManager->subItem[frameIdx].uDataType = subdecObject->objectCodeType;
	if(subdecObject->objectCodeType == LDEC_CODE_STRING)  // coded as a string of characters
	{
//--------------------------------------------------------------
		return -1;// Now,No support TEXT subtitle 
//--------------------------------------------------------------
		orgDataPtr = ldec_idvb->subIdxBuf;
		curDataPtr = (__u32 *)subdecManager->subItem[frameIdx].pSubtitle;
		subdecManager->subItem[frameIdx].subDataLen = subdecObject->subLen;
		memcpy(curDataPtr, orgDataPtr, subdecObject->subLen);
		subdecManager->subItem[frameIdx].startX = subdecObject->objectStartX;
		subdecManager->subItem[frameIdx].startY = subdecObject->objectStartY;
		subdecManager->subItem[frameIdx].subWidth = subdecObject->objectWidth;
		subdecManager->subItem[frameIdx].subHeight = subdecObject->objectHeight;
		return 1;
	}
	
	//******************将调色板转成rgb格式************************//
	if((clutIndex>=0) && clutIndex != (__s8)subdecInfo->lastClutIdx)
	{
		if(subdecInfo->subdecClut[clutIndex].clutEntry[i].twoEntryFlag)
			maxEntryNum = 4;
		else if(subdecInfo->subdecClut[clutIndex].clutEntry[i].fourEntryFlag)
			maxEntryNum = 16;
		else if(subdecInfo->subdecClut[clutIndex].clutEntry[i].eightEntryFlag)
			maxEntryNum = 256;
		else
		{
			maxEntryNum = 0;
			WARNING("may be use the default paltte.\n");
		}
		
		for(i=0; i<maxEntryNum; i++)
		{
			if (subdecInfo->subdecClut[clutIndex].clutEntry[i].hasUsedFlag == 1)
			{
				yVal = subdecInfo->subdecClut[clutIndex].clutEntry[i].yValue;
				uVal = subdecInfo->subdecClut[clutIndex].clutEntry[i].cbValue;
				vVal = subdecInfo->subdecClut[clutIndex].clutEntry[i].crValue;
				aVal = subdecInfo->subdecClut[clutIndex].clutEntry[i].tValue;
				
				if(yVal == 0)
					subdecInfo->palette[i] = 0x00ffffff;
				else
				{
					subdecInfo->palette[i] = subdec_change_yuv_to_rgb(aVal, yVal,uVal,vVal);
					//subdecInfo->palette[i] |= (dvbsub_default_TRGB[i] & 0xff000000);
				}
			}
			else
			{
				__s32 r, g, b,aVal;
				__u32 argbVal = 0;
				
				if (i == 0) {
			            r = 0;
			            g = 0;
			            b = 0;
			            aVal = 255;
				}
				else if (maxEntryNum == 4)
				{
					if (i == 1){
				            r = 255;
				            g = 255;
				            b = 255;
					}
					else if (i == 2){
				            r = 0;
				            g = 0;
				            b = 0;
					}
					else {
				            r = 127;
				            g = 127;
				            b = 127;
					}
				        aVal = 0;
				}
				else if (maxEntryNum == 16)
				{
					if (i < 8) {
				            r = (i & 1) ? 255 : 0;
				            g = (i & 2) ? 255 : 0;
				            b = (i & 4) ? 255 : 0;
					} else {
				            r = (i & 1) ? 127 : 0;
				            g = (i & 2) ? 127 : 0;
				            b = (i & 4) ? 127 : 0;
					}
				        aVal = 0;
				}
				else
				{
					if (i < 8) {
				            r = (i & 1) ? 255 : 0;
				            g = (i & 2) ? 255 : 0;
				            b = (i & 4) ? 255 : 0;
				            aVal = 191;
					} else {
					    switch (i & 0x88)
					    {
						case 0x00:
						    r = ((i & 1) ? 85 : 0) + ((i & 0x10) ? 170 : 0);
						    g = ((i & 2) ? 85 : 0) + ((i & 0x20) ? 170 : 0);
						    b = ((i & 4) ? 85 : 0) + ((i & 0x40) ? 170 : 0);
						    aVal = 0;
						    break;
						case 0x08:
						    r = ((i & 1) ? 85 : 0) + ((i & 0x10) ? 170 : 0);
						    g = ((i & 2) ? 85 : 0) + ((i & 0x20) ? 170 : 0);
						    b = ((i & 4) ? 85 : 0) + ((i & 0x40) ? 170 : 0);
						    aVal = 127;
						    break;
						case 0x80:
						    r = 127 + ((i & 1) ? 43 : 0) + ((i & 0x10) ? 85 : 0);
						    g = 127 + ((i & 2) ? 43 : 0) + ((i & 0x20) ? 85 : 0);
						    b = 127 + ((i & 4) ? 43 : 0) + ((i & 0x40) ? 85 : 0);
						    aVal = 0;
						    break;
						case 0x88:
						    r = ((i & 1) ? 43 : 0) + ((i & 0x10) ? 85 : 0);
						    g = ((i & 2) ? 43 : 0) + ((i & 0x20) ? 85 : 0);
						    b = ((i & 4) ? 43 : 0) + ((i & 0x40) ? 85 : 0);
						    aVal = 0;
						    break;
					    }
					}
				}
				argbVal = (aVal<<24)|(r<<16)|(g<<8)|b;
				subdecInfo->palette[i] = argbVal;
			}
		}
		subdecInfo->lastClutIdx = clutIndex;
	}
	subdecManager->subItem[frameIdx].subWidth = subdecObject->realSubWidth;
	subdecManager->subItem[frameIdx].subHeight = subdecObject->objectHeight;
//-----------------------------------------------------------------------------
#if 0
	i = 10;
	while(i > 0)
	{
		if((subdecObject->realSubWidth>subSize_2power[i-1]) && 
			(subdecObject->realSubWidth<=subSize_2power[i]))
			break;
		i--;
	}
	if(i == 0)
	{
		WARNING("The object width is too small.\n");
		return -1;
	}
	subdecManager->subItem[frameIdx].picWidth = subSize_2power[i];

	i = 1;
	while(i <= 10)
	{
		if((subdecObject->objectHeight>subSize_2power[i-1]) && 
			(subdecObject->objectHeight<=subSize_2power[i]))
			break;
		i++;
	}
	if(i == 11)
	{
		WARNING("The object Height is too large.\n");
		return -1;
	}
	subdecManager->subItem[frameIdx].picHeight = subSize_2power[i];
//-----------------------------------------------------------------------------
#else
	subdecManager->subItem[frameIdx].picWidth = subdecObject->realSubWidth;
	subdecManager->subItem[frameIdx].picHeight = subdecObject->objectHeight;
#endif
//-----------------------------------------------------------------------------
	if ((subdecManager->subItem[frameIdx].picWidth > 720) || (subdecManager->subItem[frameIdx].picHeight > 480))
	{
		WARNING("The sub size is lager than the screen size.\n");
		return -1;
	}
	subDataSize = subdecManager->subItem[frameIdx].picHeight * subdecManager->subItem[frameIdx].picWidth;
	if((sizeof(__u32)*subDataSize) > LDEC_MAX_LBS_SUB_SIZE)
	{
		WARNING("The sub size is lager than the sub buffer.\n");
		return -1;
	}

	orgDataPtr = ldec_idvb->subIdxBuf;
	curDataPtr = (__u32 *)subdecManager->subItem[frameIdx].pSubtitle;
	for(i=0; i<subDataSize; i++)
	{
		curDataPtr[i] = 0x00ffffff;
	}
	curDataPtr = (__u32 *)subdecManager->subItem[frameIdx].pSubtitle;
	subdecManager->subItem[frameIdx].startX = subdecObject->objectStartX;
	subdecManager->subItem[frameIdx].startY = subdecObject->objectStartY;

	for(i=1; i<subdecObject->objectHeight; i++)
	{
		if (subdecObject->realSubWidth > 4)
		{
			orgDataPtr[i*subdecObject->objectWidth+0] = orgDataPtr[0];
			orgDataPtr[i*subdecObject->objectWidth+1] = orgDataPtr[1];
			orgDataPtr[i*subdecObject->objectWidth+2] = orgDataPtr[2];
			orgDataPtr[i*subdecObject->objectWidth+3] = orgDataPtr[3];
			orgDataPtr[i*subdecObject->objectWidth+4] = orgDataPtr[4];
		}
	}

	if(clutIndex == -1)
	{
		for(i=0; i<subdecObject->objectHeight; i++)
		{
			for(j=0; j<subdecObject->realSubWidth; j++)
			{
				val = orgDataPtr[i*subdecObject->objectWidth+j];
				curDataPtr[i*subdecManager->subItem[frameIdx].picWidth+j] = dvbsub_default_TRGB[val]; 
			}
		}
	}
	else
	{
		for(i=0; i<subdecObject->objectHeight; i++)
		{
			for(j=0; j<subdecObject->realSubWidth; j++)
			{
				val = orgDataPtr[i*subdecObject->objectWidth+j];
				curDataPtr[i*subdecManager->subItem[frameIdx].picWidth+j] = subdecInfo->palette[val]; 
			}
		}
	}

	return 1;
}


void dvbsub_put_sub_output(__ldec_idvbsub_t *ldec_idvb)
{
	__Ldec_manager_t  *subdecManager;
	__u8              frameIdx =0;

	subdecManager = &(ldec_idvb->ldecManager);
	frameIdx      = subdecManager->reconBufIdx;
	
	if(subdec_process_object_data(ldec_idvb) > 0)
	{
		subdecManager->subIdxBuf[subdecManager->subWriteIdx] = frameIdx;
		subdecManager->subWriteIdx ++;
		if(subdecManager->subWriteIdx == LDEC_MAX_LBS_SUB_NUM)
			subdecManager->subWriteIdx = 0;
		subdecManager->reconBufIdx ++;
		if(subdecManager->reconBufIdx == LDEC_MAX_LBS_SUB_NUM)
			subdecManager->reconBufIdx = 0;
	}
}

__u8 dvbsub_construct_sub_chain(__ldec_idvbsub_t *ldec_idvb)
{
	__u8              i = 0;
	__u32             uDuration;
	__Ldec_manager_t  *subdecManager;
	__ldec_info_t    *subdecInfo;

	subdecInfo   = (__ldec_info_t  *)ldec_idvb->privInfo;

	subdecManager = &(ldec_idvb->ldecManager);

	uDuration = subdecInfo->subdecPage->pageTimeOut;
	if (uDuration == 0)
	{
		uDuration = DVBSUB_FRAME_MAX_DURATION;
	}

	if(subdecInfo->subdecPage->newPageFlag == 0)
	{
		return 0;
	}
	if(subdecManager->startBufIdx == subdecManager->reconBufIdx)
	{
		WARNING("Cannot dec sub picture correct.\n");
		return 0;
	}
	else if(subdecManager->startBufIdx < subdecManager->reconBufIdx)
	{
		WARNING("here1:startSubIdx=%d, writeDispIdx=%d\n",subdecManager->startBufIdx,ldec_idvb->ldecDisplay.writeDispIdx);

		for(i=subdecManager->startBufIdx; i<(subdecManager->reconBufIdx-1); i++)
		{
			subdecManager->subItem[i].isFull = 1;
			subdecManager->subItem[i].timeDuration = uDuration;
			subdecManager->subItem[i].nextItem = &subdecManager->subItem[i+1];
		}
		subdecManager->subItem[i].isFull = 1;
		subdecManager->subItem[i].nextItem = NULL;
		subdecManager->subItem[i].timeDuration = uDuration;
		subdecManager->validSize += (subdecManager->reconBufIdx - subdecManager->startBufIdx);
		ldec_idvb->ldecDisplay.dispSubNs[ldec_idvb->ldecDisplay.writeDispIdx] = (subdecManager->reconBufIdx - subdecManager->startBufIdx);
	}
	else
	{
		WARNING("here2:startSubIdx=%d, writeDispIdx=%d\n",subdecManager->startBufIdx,ldec_idvb->ldecDisplay.writeDispIdx);
		for(i=subdecManager->startBufIdx; i<(LDEC_MAX_LBS_SUB_NUM-1); i++)
		{
			subdecManager->subItem[i].isFull = 1;
			subdecManager->subItem[i].timeDuration = uDuration;
			subdecManager->subItem[i].nextItem = &subdecManager->subItem[i+1];
		}
		subdecManager->subItem[i].isFull = 1;
		subdecManager->subItem[i].timeDuration = uDuration;
		if(subdecManager->reconBufIdx == 0)
			subdecManager->subItem[i].nextItem = NULL;
		else
			subdecManager->subItem[i].nextItem = &subdecManager->subItem[0];
		subdecManager->validSize += (LDEC_MAX_LBS_SUB_NUM - subdecManager->startBufIdx);
		ldec_idvb->ldecDisplay.dispSubNs[ldec_idvb->ldecDisplay.writeDispIdx] = (LDEC_MAX_LBS_SUB_NUM - subdecManager->startBufIdx);

		if(subdecManager->reconBufIdx > 0)
		{
			for(i=0; i<(subdecManager->reconBufIdx-1); i++)
			{
				subdecManager->subItem[i].isFull = 1;
				subdecManager->subItem[i].timeDuration = uDuration;
				subdecManager->subItem[i].nextItem = &subdecManager->subItem[i+1];
			}
			subdecManager->subItem[i].isFull = 1;
			subdecManager->subItem[i].timeDuration = uDuration;
			subdecManager->subItem[i].nextItem = NULL;
			subdecManager->validSize += (subdecManager->reconBufIdx + 1);
			ldec_idvb->ldecDisplay.dispSubNs[ldec_idvb->ldecDisplay.writeDispIdx]  += (subdecManager->reconBufIdx + 1);
		}
	}

	ldec_idvb->ldecDisplay.writeDispIdx ++;
	if(ldec_idvb->ldecDisplay.writeDispIdx >= LDEC_MAX_LBS_SUB_NUM)
		ldec_idvb->ldecDisplay.writeDispIdx = 0;
	subdecInfo->subdecPage->newPageFlag = 0;
	return 1;
}
 
//*************************************************************************************//
//*********************************main func******************************************//
__s8 dvbsub_parse_segment(__ldec_idvbsub_t *ldec_idvb, __u8 *findEndFlag)
{
	__s8  result = 0;
	__u8  segmentType = 0;
	__u16 segmentLen = 0;
	__s32 frmIdx    = 0;
	__u8  *curDataPtr = NULL;
	__ldec_lbs_buf_t  *subdecBuf;
	__ldec_info_t    *subdecInfo;
	__Ldec_manager_t  *subdecManager;
	

	subdecBuf = &(ldec_idvb->ldecLbsBuf);
	subdecInfo   = (__ldec_info_t  *)ldec_idvb->privInfo;
	subdecManager = &(ldec_idvb->ldecManager);

	*findEndFlag   = 0;
	curDataPtr = subdec_check_buffer_boundary(ldec_idvb, 5);
	segmentType = curDataPtr[0];
	segmentLen = (curDataPtr[3]<<8) | curDataPtr[4];
	if(subdecBuf->validSize < segmentLen)
		return LDEC_ERR_LBS_UNDERFLOW;

	if(segmentType>=0x10 && segmentType<=0x13)
	{
		if((segmentType != LDEC_PAGE_COMPOSITION_SEGMENT)
			&& (subdecInfo->subdecPage->newPageFlag==0))
		{
			ldec_update_read_pointer(subdecBuf,segmentLen+5);
			return LDEC_ERR_NONE;
		}
	}
	switch(segmentType)
	{
		case LDEC_PAGE_COMPOSITION_SEGMENT:
			frmIdx = subdec_select_frame_index(ldec_idvb);	//cannot find free_frame_idx
			if(frmIdx < 0)
				return LDEC_ERR_NO_FRAMEBUFFER;
			result = dvbsub_page_comp_seg_parse(ldec_idvb, segmentLen);
			if(result==1)
			{
				subdecManager->startBufIdx  = frmIdx;
				subdecManager->reconBufIdx = frmIdx;
				ldec_idvb->ldecDisplay.dispSubIdx[ldec_idvb->ldecDisplay.writeDispIdx] = frmIdx;
			}
			if (result==-1)
			{
				return LDEC_ERR_DEC_SUBTITLE;
			}
			break;
		case LDEC_REGION_COMPOSITION_SEGMENT:
			result = dvbsub_region_comp_seg_parse(ldec_idvb, segmentLen);
			if(result < 0)
			{
				return LDEC_ERR_DEC_SUBTITLE;
			}
			break;
		case LDEC_CLUT_DEFINITION_SEGMENT:
			result = dvbsub_clut_def_seg_parse(ldec_idvb, segmentLen);
			if(result < 0)
			{
				return LDEC_ERR_DEC_SUBTITLE;
			}
			break;
		case LDEC_OBJECT_DATA_SEGMENT:
			result = dvbsub_object_def_seg_parse(ldec_idvb, segmentLen);
			if(result < 0)
			{
				if (result == -2)
					return LDEC_ERR_DEC_SUBTITLE;
				return LDEC_ERROR_NO_FRAMEBUFFER;
			}
			dvbsub_put_sub_output(ldec_idvb);
			break;
		case LDEC_END_DISPLAY_SEGMENT:
			ldec_update_read_pointer(subdecBuf,6);
			dvbsub_construct_sub_chain(ldec_idvb); 
			*findEndFlag = 1;
			break;
		case LDEC_END_PES_DATA_MARKER:
			ldec_update_read_pointer(subdecBuf,1);
			*findEndFlag = 1;
			break;
		default:
			return LDEC_ERR_DEC_SUBTITLE;

	}
	return LDEC_ERR_NONE;
}

#endif
