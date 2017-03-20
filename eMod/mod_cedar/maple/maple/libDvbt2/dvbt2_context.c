
#include "dvbt2_context.h"
#include "dvbt2_eit_parser.h"
#include "dvbt2_eit_pool.h"

//STD-B10v4_6-E2_Service Information for Digital Broadcasting System, P83
#define WAIT_SDT_TIMEOUT    (5000)  //�ȴ�sdt��ʱ����ֵ,��λms, �涨Ϊ2s.��Ϊ3s������
#define WAIT_NIT_TIMEOUT    (10000)  //�ȴ�sdt��ʱ����ֵ,��λms, һ����10s,���ﶨΪ5s.
#define WAIT_PAT_TIMEOUT    (1000)  //�ȴ�pat��ʱ����ֵ,��λms, �涨Ϊ100ms,���ﶨΪ1s
#define WAIT_PMT_TIMEOUT    (1000)  //�ȴ�pmt��ʱ����ֵ,��λms, �涨Ϊ100ms,���ﶨΪ200ms.
#define CAI_MODIFY_20140528			1
#define CAI_MODIFY_FOR_MULTIPLP			1


#define DEFAULT_SERVICE_NAME    "NoName"
#define DEFAULT_REMOTE_CONTROL_KEY_ID    (0xff)
#define START_REMOTE_CONTROL_KEY_ID		(0x15E)
maple_serv_list_t   *servList;
//maple_serv_list_t   *tmpservList;
extern __s32 safeStringCopy(__u8* dst, __u32 dstSize, __u8* src, __u32 srcLen);
extern int DListDataCompareFunc_FindNITTSInfoByTsId(void *ctx, void *data);
//typedef DListRet (*DListDataVisitFunc)(void* ctx, void* data);      //DList foreach elemʱ��ʱ����, ctx����ʱ�����������
extern DListRet DListDataVisitFunc_SDTAddService(void* ctx, void* data);   //ctx = maple_serv_list_t   *servList; data = SDTServInfo*

/*******************************************************************************
Function name: safeStringCopy
Description: 
    
Parameters: 
    srcLen:�ַ������ȣ�������\0
    dstSize:Ŀ��buffer�ܳ�,����\0,������2���ֽ�.
Return: 
    
Time: 2011/2/23
*******************************************************************************/
__s32 safeStringCopy(__u8* dst, __u32 dstSize, __u8* src, __u32 srcLen)
{
    __u32   tmpLen;
    if(dstSize <= 2)
    {
        return 0;
    }
    if(dstSize < srcLen + 2)
    {
        tmpLen = dstSize - 2;
    }
    else
    {
        tmpLen = srcLen;
    }
    memcpy(dst, src, tmpLen);
    dst[tmpLen] = 0;
    dst[tmpLen+1] = 0;
    return 0;
}
static __u32 dvbt2_bcd_to_int(__u32 val)
{
    __u32 highBits  = 0;
    __u32 lowBits   = 0;
    __u32 result    = 0;
    
    highBits    = val>>4;
    lowBits     = val & 0x0f;
    result      = highBits*10 + lowBits;
    return result;
}
/*******************************************************************************
Function name: parse_partial_reception_descriptor
Description: 
    parse NIT���Partial reception descriptor.
Parameters: 
    content: ָ��despLen֮����ֽ�,����3���ֽ�
Return: 
    
Time: 2012/6/19
*******************************************************************************/
/*
__s32 parse_partial_reception_descriptor_NIT(NITTSInfo* pNITTSInfo, __u32 despTag, __s32 despLen, __u8 *content)
{
    __s32   i;
    pNITTSInfo->nOneSegServNum = despLen / 2;
    if(pNITTSInfo->nOneSegServNum > ONESEG_MAX_SERVICE_NUM)
    {
        WARNING("impossbile? oneseg sevice num = [%d]?\n", pNITTSInfo->nOneSegServNum);
        pNITTSInfo->nOneSegServNum = ONESEG_MAX_SERVICE_NUM;
    }
    for(i=0;i<pNITTSInfo->nOneSegServNum;i++)
    {
        pNITTSInfo->OneSegServIdArray[i] = SI_MERGE_U16(content);
        content+=2;
    }
    return EPDK_OK;
}
*/
#if(ckydebug == 1)
__s32 dvbt2_parse_linkage_descriptor_SSU(NITTSInfo* pNITTSInfo, __u32 despTag, __s32 despLen, __u8 *content)
{
	__u8 u8Offset = 0;
	
	WARNING("despTag[%x],despLen[%d]\n",despTag,despLen);
	pNITTSInfo->u16TSId = SI_MERGE_U16(content);
    pNITTSInfo->u16ONId = SI_MERGE_U16(content+2);
    pNITTSInfo->u16ServiceId = SI_MERGE_U16(content+4);
    pNITTSInfo->u8LinkageType = SI_MERGE_U16[content+6];
	WARNING("pNITTSInfo->u8LinkageType[%x]\n",pNITTSInfo->u8LinkageType)
    if ( pNITTSInfo->u8LinkageType == 0x08 )//wei check
    {
        pNITTSInfo->u8HandOverType = content[7]&0x0f;
        pNITTSInfo->u8OriginType = content[7]&0x80;//?content[7]&0x01;
        
        if ( ( pNITTSInfo->u8HandOverType == 0x01 ) || ( pNITTSInfo->u8HandOverType == 0x02 ) || ( pNITTSInfo->u8HandOverType == 0x03 ) )
        {
            pNITTSInfo->u16NetworkId = SI_MERGE_U16(content+8);
            u8Offset += 2;
        }
        if ( pNITTSInfo->u8OriginType == 0x00 )
        {
            pNITTSInfo->u16InitServiceId = SI_MERGE_U16(content+8+u8Offset);
            u8Offset += 2;
        }
        pNITTSInfo->u8PrivDataLeng = despLen - 8 - u8Offset;
        pNITTSInfo->pu8PrivData = content[8+u8Offset];
    }
    else//0x09 or 0x0a
    {
        pNITTSInfo->u8PrivDataLeng = despLen - 7;
        pNITTSInfo->pu8PrivData = content[7];
		WARNING("pNITTSInfo->u8PrivDataLeng[%d],pNITTSInfo->pu8PrivData[%x]\n",pNITTSInfo->u8PrivDataLeng,pNITTSInfo->pu8PrivData);
    }	
	return despLen;
}
#endif
/*******************************************************************************
Function name: dvbt2_parse_logical_channel_descriptor_NIT
Description: 
    parse NIT���logical channel descriptor.��ͬ�������ֵ��ӱ�׼������ͬ��
    ŷ�޹���������������ֵ:һ��Ϊ0x83��һ��0x86 
Parameters: 
 
Return: 
    
Time: 2012/6/19
*******************************************************************************/

__s32 dvbt2_parse_logical_channel_descriptor_NIT(NITTSInfo* pNITTSInfo, __u32 despTag, __s32 despLen, __u8 *content) 
{
	__s32   i;
	pNITTSInfo->nServNum = despLen / 4;
	
	WARNING("pNITTSInfo->nServNum [%d]",pNITTSInfo->nServNum);
	WARNING("NIT:%x %x %x %x %x %x %x\n",content[0],content[1],content[2],content[3],content[4],content[5],content[6]);
	if(pNITTSInfo->nServNum > TS_MAX_SERVICE_NUM)
    {
        WARNING("impossbile? dvbt2 sevice num = [%d]?\n", pNITTSInfo->nServNum);
        pNITTSInfo->nServNum = TS_MAX_SERVICE_NUM;
    }
	 for(i = 0;i < pNITTSInfo->nServNum;i++)
	 {
			pNITTSInfo->ServIdArray[i] = SI_MERGE_U16(content);
			//pNITTSInfo->nVisible_Service_Flag = (SI_MERGE_U16(content+2)&0x8000)>>16;
			pNITTSInfo->nRemoteControlKeyId[i] = SI_MERGE_U16(content+2)&0xff;
			WARNING("ServIdArray[%d] = 0x%x,nRemoteControlKeyId[%d] = 0x%x\n",i,pNITTSInfo->ServIdArray[i],i,pNITTSInfo->nRemoteControlKeyId[i]);
			content+=4;

	 }
    return EPDK_OK;
}

/*******************************************************************************
Function name: dvbt2_parse_pmt_descriptor
Description: 
    1.��ʱstreamtype��streampid���Ѿ���ֵ��,����streamtype��Ҫ�������������޸�.
    2.һ�δ���һ��������
    3.��Ҫ�����һ���õ�private stream������.
Parameters: 
    
Return: 
    
Time: 2012/6/18
*******************************************************************************/
static __u32 dvbt2_parse_pmt_descriptor(PMTStreamInfo *pPMTStreamInfo, __u8 *dataPtr, __s32 streamType, __s32 esInfoLen)
{
	__u8 despTag		=	0;
	__u8 despLen		=	0;
    __u16   data_component_id = 0;

    if(esInfoLen > 0)
    {
         despTag		=	dataPtr[0];
	     despLen		=	dataPtr[1] + 2;
    }
   
	if(streamType == STREAM_TYPE_PRIVATE_DATA)
	{
		if((despTag==AC3_DESCRIPTOR_TAG) || (despTag==ENHANCED_AC3_DESCRIPTOR_TAG))
			streamType	=	STREAM_TYPE_AUDIO_AC3;			//assume DVB AC-3 Audio
		else if(despTag == DTS_DESCRIPTOR_TAG)
			streamType	=	STREAM_TYPE_AUDIO_DTS;			// DVB DTS audio 
		else if(despTag == SUBTITLING_DESCRIPTOR_TAG)
			streamType	=	STREAM_TYPE_SUBTITLE_DVB;
		else if(despTag == TELETEXT_DESCRIPTOR_TAG)
			streamType	=	STREAM_TYPE_SUBTITLE_TELETEXT;
//   else if(despTag == STREAM_IDENTIFIER_DESCRIPTOR_TAG)
//        {
//            WARNING("## Be careful: STREAM_IDENTIFIER_DESCRIPTOR_TAG!\n");
//            if (0xfd == (__u8)*(dataPtr + despLen))
//            {
//                WARNING("## Be careful: STREAM_TYPE_SUBTITLE_ARIB!\n");
//                streamType = STREAM_TYPE_SUBTITLE_ARIB;
//            }
//        }
//    else if(DATA_COMPONENT_DESCRIPTOR_TAG == despTag)
//     {
//         data_component_id = SI_MERGE_U16(&dataPtr[2]);
//              
//          if(0x08 == data_component_id || 0x12 == data_component_id)    //dvbt2 si spec:Page206, ˵����ARIB-Subtitle
//         {
//             if (streamType == STREAM_TYPE_PRIVATE_DATA)
//              {
//                 WARNING("detect dvbt2 subtitle!data_component_id[0x%x]\n", data_component_id);
//                 streamType = STREAM_TYPE_SUBTITLE_ARIB;
//            }
//        }
//    }
		else if(despTag == REGISTRATION_DESCRIPTOR_TAG)
		{
			if((dataPtr[2]=='D')&&(dataPtr[3]=='T')&&(dataPtr[4]=='S'))
				streamType	=	STREAM_TYPE_AUDIO_DTS;
			else if((dataPtr[2]=='d')&&(dataPtr[3]=='t')&&(dataPtr[4]=='s'))
				streamType	=	STREAM_TYPE_AUDIO_DTS;
			else if((dataPtr[2]=='V')&&(dataPtr[3]=='C')&&(dataPtr[4]=='-')&&(dataPtr[5]=='1'))
				streamType = STREAM_TYPE_VIDEO_VC1;
		}
        if(despTag != STREAM_IDENTIFIER_DESCRIPTOR_TAG)//yefl test 20150930
        {
       		 pPMTStreamInfo->nStreamType = streamType;
        }
	}
    
	switch(despTag)
	{
        case ISO_639_LANGUAGE_DESCRIPTOR_TAG:
            safeStringCopy(pPMTStreamInfo->StreamName, MAX_STREAMNAME_SIZE, dataPtr + 2, 3);
            pPMTStreamInfo->nStreamNameLen = despLen;
            break;
        case SUBTITLING_DESCRIPTOR_TAG:
            safeStringCopy(pPMTStreamInfo->StreamName, MAX_STREAMNAME_SIZE, dataPtr + 2, 3);
            pPMTStreamInfo->nStreamNameLen = despLen;
            break;
		// case TELETEXT_DESCRIPTOR_TAG:
      //      safeStringCopy(pPMTStreamInfo->StreamName, MAX_STREAMNAME_SIZE, dataPtr + 2, 3);
      //      break;
        default:
        {
            WARNING("pmt ignore descriptor_tag[0x%x]\n", despTag);
            break;
        }
	}
	return despLen;
}

/*******************************************************************************
Function name: dvbt2_parse_pmt_descriptor
Description: 
    1.��ʱstreamtype��streampid���Ѿ���ֵ��,����streamtype��Ҫ�������������޸�.
    2.һ�δ���һ��������
    3.��Ҫ�����һ���õ�private stream������.
Parameters: 
    
Return: 
    
Time: 2012/10/18
*******************************************************************************/
/*
static __u32 dvbt2_parse_pmt_descriptor(maple_serv_item_t *curServItem, __u8 *dataPtr, __s32 streamType, __u32 streamPid, __s32 esInfoLen)
{
	__u8 despTag		=	0;
	__u8 despLen		=	0;
	__u8 hasAudStream	=	0;
	__u8 hasVidStream	=	0;
	__u8 hasSubStream	=	0;

    if(esInfoLen > 0)
    {
         despTag		=	dataPtr[0];
	     despLen		=	dataPtr[1] + 2;
    }
   
	if(streamType == STREAM_TYPE_PRIVATE_DATA)
	{
		if((despTag==0x6A) || (despTag==0x7A))
			streamType	=	STREAM_TYPE_AUDIO_AC3;			//assume DVB AC-3 Audio/
		else if(despTag == 0x7B)
			streamType	=	STREAM_TYPE_AUDIO_DTS;			/// DVB DTS audio //
		else if(despTag == 0x59)
			streamType	=	STREAM_TYPE_SUBTITLE_DVB;	
		else if(despTag == 0x05)
		{
			if((dataPtr[2]=='D')&&(dataPtr[3]=='T')&&(dataPtr[4]=='S'))
				streamType	=	STREAM_TYPE_AUDIO_DTS;
			else if((dataPtr[2]=='d')&&(dataPtr[3]=='t')&&(dataPtr[4]=='s'))
				streamType	=	STREAM_TYPE_AUDIO_DTS;
			else if((dataPtr[2]=='V')&&(dataPtr[3]=='C')&&(dataPtr[4]=='-')&&(dataPtr[5]=='1'))
				streamType = STREAM_TYPE_VIDEO_VC1;
		}
	}
 
	if((streamType==STREAM_TYPE_AUDIO_MPEG1) || (streamType==STREAM_TYPE_AUDIO_MPEG2))
	{
		streamType		=	MAPLE_AUD_TYPE_MPEG;
		hasAudStream	=	1;
	}
	else if((streamType==STREAM_TYPE_AUDIO_AAC) || (streamType==STREAM_TYPE_AUDIO_MPEG4))
	{
		streamType		=	MAPLE_AUD_TYPE_AAC;
		hasAudStream	=	1;
	}
	else if((streamType==STREAM_TYPE_AUDIO_AC3) || (streamType==STREAM_TYPE_AUDIO_AC3_) ||
		   (streamType==STREAM_TYPE_AUDIO_EAC3) || (streamType==STREAM_TYPE_AUDIO_AC3_TRUEHD))
	{
		streamType		=	MAPLE_AUD_TYPE_AC3;
		hasAudStream	=	1;
	}
	else if((streamType==STREAM_TYPE_AUDIO_DTS) || (streamType==STREAM_TYPE_AUDIO_DTS_) ||
		   (streamType==STREAM_TYPE_AUDIO_HDMV_DTS) || (streamType==STREAM_TYPE_AUDIO_DTS_HRA)
		   || (streamType==STREAM_TYPE_AUDIO_DTS_MA))
	{
		streamType		=	MAPLE_AUD_TYPE_DTS;
		hasAudStream	=	1;
	}
	else if((streamType==STREAM_TYPE_VIDEO_MPEG1) || (streamType==STREAM_TYPE_VIDEO_MPEG2))
	{
		streamType		=	MAPLE_VID_TYPE_MPEG2;
		hasVidStream	=	1;
	}
	else if(streamType == STREAM_TYPE_VIDEO_H264)
	{
		streamType		=	MAPLE_VID_TYPE_H264;
		hasVidStream	=	1;
	}
	else if((streamType==STREAM_TYPE_VIDEO_VC1) || (streamType==STREAM_TYPE_VIDEO_MPEG4))
	{
		streamType		=	MAPLE_VID_TYPE_UNKNOWN;
		hasVidStream	=	1;
	}
	else if(streamType == STREAM_TYPE_SUBTITLE_DVB)
	{
		streamType		=	MAPLE_SUBT_TYPE_DVBSUB;
		hasSubStream	=	1;
	}

	
	if(hasAudStream == 1)
	{
		curServItem->adInfo[curServItem->adStreamNum].pid			=	streamPid;
		curServItem->adInfo[curServItem->adStreamNum].adCodeType	=	(maple_ad_codec_e)streamType;
		memset(curServItem->adInfo[curServItem->adStreamNum].adStreamName, 0, MAPLE_MAX_NAME_LEN);
		if(despTag == ISO_639_LANGUAGE_DESCRIPTOR_TAG)
		{
			memcpy(curServItem->adInfo[curServItem->adStreamNum].adStreamName, dataPtr+2,3);
		}
	}
	else if(hasVidStream == 1)
	{
		curServItem->vdInfo[curServItem->vdStreamNum].pid			=	streamPid;
		curServItem->vdInfo[curServItem->vdStreamNum].vdCodeType	=	(maple_vd_codec_e)streamType;
	}
	else if(hasSubStream == 1)
	{
		if(despTag	==	SUBTITLING_DESCRIPTOR_TAG)
		{
			curServItem->subtInfo[curServItem->subtitleStreamNum].subtPid		=	streamPid;
			curServItem->subtInfo[curServItem->subtitleStreamNum].subtCodeType	=	(maple_subt_codec_e)streamType;
			memset(curServItem->subtInfo[curServItem->subtitleStreamNum].subtName, 0, MAPLE_MAX_NAME_LEN);
			memcpy(curServItem->subtInfo[curServItem->subtitleStreamNum].subtName, dataPtr+2,3);
		}
	}
	return despLen;
}
*/
static void dvbt2_parse_sdt_descriptor(SDTServInfo* pSDTServInfo, __u8 *sectionPtr)//(maple_serv_item_t* curServPtem, __u8 *sectionPtr)
{  
    __u32 despTag = 0;
    __u32 servProviderLen = 0;
    
    despTag = sectionPtr[0];
    
    switch(despTag)
    {
    case BOUQUET_NAME_DESCRIPTOR_TAG:              //ҵ������������
        break;
    case CA_IDENTIFIER_DESCRIPTOR_TAG:             //CA ��ʶ��������
        break;
    case COUNTRY_AVAILABILITY_DESCRIPTOR_TAG:      //���һ����������
        break;
    case DATA_BROADCAST_MDESCRIPTOR_TAG:           //���ݹ㲥������
        break;
    case LINKAGE_DESCRIPTOR_TAG:                   //����������
        break;
    case MOSAIC_DESCRIPTOR_TAG:                    //��Ƕ������
        break;
    case NVOD_REFERENCE_DESCRIPTOR_TAG:            //NVOD�ο�������
        break;
    case SERVICE_DESCRIPTOR_TAG:                   //ҵ��������
		memset(pSDTServInfo->servName, 0, MAX_SERVNAME_SIZE);
		pSDTServInfo->nServNameLen = 0;
       pSDTServInfo->nServType = sectionPtr[2];
		servProviderLen = sectionPtr[3];
        sectionPtr += (4+servProviderLen);
        if(sectionPtr[0] > 0)
           pSDTServInfo->nServNameLen =  dvbt2_convert_str_to_utf8(sectionPtr+1, sectionPtr[0], pSDTServInfo->servName, MAPLE_MAX_NAME_LEN, 0);
        break;
    case TELEPHONE_DESCRIPTOR_TAG:                  //�绰������
        break;
    case TIME_SHIFTED_SERVICE_DESCRIPTOR_TAG:       //ʱ��ƽ��ҵ��������
        break;
    default:
        break;
    }
}

/*******************************************************************************
Function name: dvbt2_parse_nit_descriptor_loop2
Description: 
    1.����nit section�ڶ���desc loop���������ĺ���.
    2.һ�ν���һ��������.
    3.��ǰ����Щ��������Ϊsearch�����,���Բ�������ĺ�ȫ.��Ҫʲô�ͽ���ʲô.
Parameters: 
    
Return: 
    EPDK_OK;
    1:ignore
Time: 2012/2/29
*******************************************************************************/
static __s32 dvbt2_parse_nit_descriptor_loop2(NITTSInfo* pNITTSInfo, __u8 *sectionPtr)
{
    __u32   despTag = 0;
    __s32   despLen = 0;
    __s32   ret = EPDK_FAIL;
    despTag = sectionPtr[0];
    despLen = sectionPtr[1];
	WARNING("loop2 despTag = 0x%x,despLen = 0x%x\n",despTag,despLen);
    switch(despTag)
    {
        case LOGICAL_CHANNEL_DESCRIPTOR_TAG:
		 {
		 		ret = dvbt2_parse_logical_channel_descriptor_NIT(pNITTSInfo, despTag, despLen, sectionPtr+2);
				break;
		 }
					
        case LOGICAL_CHANNEL_DESCRIPTOR_V2_TAG:
        {
				
				WARNING("LOGICAL_CHANNEL_DESCRIPTOR_V2_TAG = 0x%x",despTag);
           // ret = dvbt2_parse_logical_channel_V2_descriptor_NIT(pNITTSInfo, despTag, despLen, sectionPtr+2);
             break;
        }
#if(ckydebug == 1)
		case LINKAGE_DESCRIPTOR_TAG:
		{
			WARNING("LINKAGE_DESCRIPTOR_TAG:despTag = 0x%x,despLen = 0x%x\n",despTag,despLen);
			ret = dvbt2_parse_linkage_descriptor_SSU(pNITTSInfo, despTag, despLen, sectionPtr+2);
			break;
		}
#endif
        default:
        {
			 WARNING("default cky:despTag = 0x%x,despLen = 0x%x\n",despTag,despLen);
			   ret = despLen;
            break;
        }
    }
    return ret;
}

/*******************************************************************************
Function name: decideValidStream
Description: 
    1.�ж��Ƿ�����Ҫ��stream
Parameters: 
    nStreamType;    //STREAM_TYPE_VIDEO_MPEG1��
Return: 
    EPDK_TRUE:��,
    
Time: 2012/6/18
*******************************************************************************/
__s32 decideValidStream(__u16 nStreamType)
{
    switch(nStreamType)
    {
        case STREAM_TYPE_VIDEO_MPEG1:
        case STREAM_TYPE_VIDEO_MPEG2:
        case STREAM_TYPE_AUDIO_MPEG1:
        case STREAM_TYPE_AUDIO_MPEG2:
        case STREAM_TYPE_AUDIO_AAC:
        case STREAM_TYPE_VIDEO_MPEG4:
        case STREAM_TYPE_AUDIO_MPEG4:
        case STREAM_TYPE_VIDEO_H264:              
        case STREAM_TYPE_VIDEO_VC1:
        case STREAM_TYPE_PCM_BLURAY:
        case STREAM_TYPE_AUDIO_AC3:
        case STREAM_TYPE_AUDIO_HDMV_DTS:
        case STREAM_TYPE_AUDIO_AC3_TRUEHD:
        case STREAM_TYPE_AUDIO_EAC3:
        case STREAM_TYPE_AUDIO_DTS_HRA:
        case STREAM_TYPE_AUDIO_DTS_MA:
        case STREAM_TYPE_AUDIO_DTS:
        case STREAM_TYPE_AUDIO_AC3_:
        case STREAM_TYPE_AUDIO_DTS_:
        case STREAM_TYPE_AUDIO_HDMV_PGS_SUBTITLE:
        case STREAM_TYPE_SUBTITLE_DVB:
		 case STREAM_TYPE_SUBTITLE_TELETEXT:
        case STREAM_TYPE_SUBTITLE_ARIB:
            return EPDK_TRUE;
        default:
            return EPDK_FALSE;
    }
}


static __s32 dvbt2_parse_pmt_section(PMTInfo *pPmtInfoForSearch, __u8 *section, __u32* sectionNum)
{
	__s32 sectionLen	            = 0;
	__s32 esInfoLen		            = 0;
    __u32 pgmInfoLen	            = 0;
    //__u32 streamPid		            = 0;
    __s32 streamType	            = 0;
	__u32 despLen                   = 0;
    //__s32 nStreamIdx                = 0;
    __u8  *orgDataPtr               = NULL;
    PMTStreamInfo   *pPMTStreamInfo = NULL;
  //  maple_serv_item_t* curServItem  = NULL;

    orgDataPtr                      = section;


    if(section[6] != (*sectionNum))     //step-1 check section number
    {
        WARNING("not we need pmt section num[%d][%d]\n", section[6], *sectionNum);
        return 0;
    }
    else if(section[0] != 0x02)     //0x02Ϊ��ǰts��pmt table id
    {
        return 0;
    }

	if((section[5] & 0x1) == 0)        // ��ǰpmt������
	{
        WARNING("cur pmt section nextIndecator == 0, return!\n");
        return 0;
	}

	pPmtInfoForSearch->nServId = (section[3]<<8) | section[4];
	pPmtInfoForSearch->pcrPid = ((section[8]&0x1f)<<8) | section[9];

	sectionLen	=	((section[1]&0x0f)<<8)  | section[2];
    pgmInfoLen	=	((section[10]&0x0f)<<8) | section[11];
    section		+=	(12 + pgmInfoLen);
    sectionLen	-=	(13 + pgmInfoLen);  //second loop length

    pPmtInfoForSearch->nStreamNum = 0;
	while(sectionLen > 0)
	{
        if(pPmtInfoForSearch->nStreamNum >= MAX_STREAM_NUM_IN_SERVICE)
        {
            WARNING("pmt stream array is full!break![%d]\n", pPmtInfoForSearch->nStreamNum);
            break;
        }
        pPMTStreamInfo = &pPmtInfoForSearch->streamInfoArray[pPmtInfoForSearch->nStreamNum];
        pPMTStreamInfo->nStreamType = streamType  = section[0];
		pPMTStreamInfo->nStreamPid = ((section[1]&0x1f)<<8) | section[2];
		esInfoLen   = ((section[3]&0x0f)<<8) | section[4];
		sectionLen -= (5+esInfoLen);
        if(sectionLen < 0)
        {
            WARNING("fatal error! PMT section loop parse error! check pmt section or code![%d][%d]\n", sectionLen, esInfoLen);
            break;
        }
        section	   += 5;

        do
		{  
		    //* despLen: current descriptor length
		    despLen	     = dvbt2_parse_pmt_descriptor(pPMTStreamInfo, section, streamType, esInfoLen);
			section	    += despLen;
			esInfoLen   -= despLen;
		}while(esInfoLen > 0);
	      //�ж����stream�Ƿ�Ҫ����������,����ֻ��video/audio/subtitle.
        if(EPDK_TRUE == decideValidStream(pPMTStreamInfo->nStreamType))
        {
            pPmtInfoForSearch->nStreamNum++;
        }
        else
        {
            memset(pPMTStreamInfo, 0, sizeof(PMTStreamInfo));   //���
        }
        
	}
    
    if((*sectionNum) != orgDataPtr[7]) // step-4 check the last section
    {
        (*sectionNum)++;
        return 0;
    }
    return 1;
}

/*******************************************************************************
Function name: dvbt2_parse_pat_section
Description: 
    
Parameters: 
    
Return: 
    1:pat��section����������
    0:patû�н�����
Time: 2011/1/4
*******************************************************************************/
static __s32 dvbt2_parse_pat_section(PATInfo *pPatInfoForSearch, __u8 *section, __u32* sectionNum)
{
    __u32 sectionLen = 0;
    __u32 prgmNum    = 0;
    __u32 i          = 0;
    __u8 *curDataPtr = NULL;
   
    if(section[6] != (*sectionNum))     //step-1 check section number
    {
        return 0;
    }
    else                                 //step-3 parse pat information
    { 
        pPatInfoForSearch->tsId = (section[3]<<8)|section[4];
        curDataPtr = section;
        sectionLen = ((section[1] & 0x0f)<<8) | section[2];
        prgmNum    = (sectionLen-9)>>2;
        curDataPtr = section + 8;
        for(i=0;i<prgmNum; i++)     //* ����PAT��ÿһ�� program_map_pid
        {
	        if((curDataPtr[0]<<8|curDataPtr[1]) != 0)
            {
                if(pPatInfoForSearch->nServNum >= TS_MAX_SERVICE_NUM)
                {
                    WARNING("pat_serv_info array is full\n");
                    break;
                }
				pPatInfoForSearch->nServNum++;
                pPatInfoForSearch->servInfoArray[pPatInfoForSearch->nServNum].nServId = curDataPtr[0]<<8|curDataPtr[1];
                pPatInfoForSearch->servInfoArray[pPatInfoForSearch->nServNum].pmtpid = (curDataPtr[2]&0x1f)<<8| curDataPtr[3];
            }
            curDataPtr += 4;
        }
    }
    
    if((*sectionNum) != section[7])     // step-4 check the last section
    {
        (*sectionNum) ++;
        return 0;
    }
    else
    {
        return 1;
    } 
}

/*******************************************************************************
Function name: dvbt2_parse_sdt_section
Description: 
    û�в�versionnum.
Parameters: 
    sectionNum:����ʱ��ʾ��ǰ�������sdt���sectionnum,�������ֵ,��ʾ��һ���������sdt���sectionnum
Return: 
    0:û��parse�ɹ�,������sdt��sectionû��parse��.
    1:sdt���ȫ����section��parse�����.
Time: 2012/6/17
*******************************************************************************/
static __s32 dvbt2_parse_sdt_section(SDTInfo *pSDTInfoForSearch, __u8 *section, __u32 *sectionNum)
{
    __u32               nextIndicator   = 0;
    __s32               sectionLen      = 0;
    __s32               despLen         = 0;
    //__u16               serviceId       = 0;
    __u8                *orgDataPtr     = NULL;
    SDTServInfo         *pSDTServInfo   = NULL;
    __s32   tmpret;
    if(section[6] != (*sectionNum))     //step-1 check section number
    {
        return 0;
    }
    else if(section[0] != 0x42)     //0x42Ϊ��ǰts��service_description_table_id
    {
        return 0;
    }

    orgDataPtr = section;
    nextIndicator = section[5] & 0x01;
    if(nextIndicator == 0)
    {
        WARNING("cur sdt section nextIndecator == 0, return!\n");
        return 0;
    }
    
    pSDTInfoForSearch->tsId = section[3]<<8 | section[4];
    pSDTInfoForSearch->orgNetworkId = section[8]<<8 | section[9];
        
    sectionLen = (((section[1]&0x0f)<<8) | section[2]) - 12;    //ȥ��ͷ��,ȥ��CRCУ��֮��ĳ���.
    section += 11;
    while(sectionLen > 0)
    {
        pSDTServInfo = newSDTServInfo();
        if(NULL == pSDTServInfo)
        {
            WARNING("malloc SDTServInfo fail!\n");
            return 1;   //�͵�������
        }
        pSDTServInfo->nServId = SI_MERGE_U16(&section[0]);
        pSDTServInfo->nFreeCAMode = (section[3]&0x10)>>4;
        
		despLen  = ((section[3]&0x0f)<<8) | section[4];
		sectionLen -= (5+despLen);
        if(sectionLen < 0)
        {
            WARNING("fatal error! SDT section loop parse error! check sdt section or code![%d][%d]\n", sectionLen, despLen);
            deleteSDTServInfo(pSDTServInfo);
            break;
        }
        //��ʼdescriptor loop����
		section += 5;
		while(despLen > 0)
		{
			dvbt2_parse_sdt_descriptor(pSDTServInfo, section);
			despLen -= (2+section[1]);
			section += (2+section[1]);
            if(despLen < 0)
            {
                WARNING("fatal error! SDT descriptor loop parse error! check sdt section or code!\n");
            }
		}
        tmpret = pSDTInfoForSearch->pList_SDTServInfo->AppendElem(pSDTInfoForSearch->pList_SDTServInfo, (void*)pSDTServInfo);
        if(tmpret!=EPDK_OK)
        {
            deleteSDTServInfo(pSDTServInfo);
            WARNING("AppendElem SDTServInfo fail!\n");
            return 1;
        }
    } // end while(sectionLen > 0)

    if((*sectionNum) != orgDataPtr[7])     // step-4 check the last section
    {
        (*sectionNum) ++;
        return 0;
    }
    return 1;
}

/*******************************************************************************
Function name: dvbt2_parse_nit_section_for_search
Description: 
    ������̨��nit section�Ľ�������.ֻ��actual nit, tableid = 0x40
Parameters: 
    *sectionNum:��ǰ��Ҫ��section��number. �������,���������һ��section,��ֵ����һ��section num.
Return: 
    ����NIT���Ƿ�������.
    1:����
    0:��section�������, ����sectionû����.
Time: 2012/2/29
*******************************************************************************/
static __s32 dvbt2_parse_nit_section_for_search(NITInfo     *pNitInfoForSearch, __u8 *section, __u32 *sectionNum)
{
    __u32               nextIndicator   = 0;
    //__s32               sectionLen      = 0;
    //__s32               despLen         = 0;
    //__u16               serviceId       = 0;
    //__u16               tsId            = 0;
    __u16               network_descriptors_length;
    __u16               transport_stream_loop_length;
    __u16               transport_descriptors_length;
	__s32               nNetDesLen;
    __s32               nLeftNetDesLen;
    __s32               nLeftLen;
    __s32               nLeftDescLoopLen;
    //__u16               orgNetworkId    = 0;
    __u8                i               = 0;
    __u8                *orgDataPtr     = NULL;
    //maple_serv_item_t   *curServItem    = NULL;

    NITTSInfo           *pNITTSInfo = NULL;
    DList               *pNITTSInfoList; //������NITTSInfo
    __s32               tmpret;
    
    if(section[6] != (*sectionNum))     //step-1 check last_section_number
    {
        return 0;
    }
    else if(section[0] != 0x40)     //0x40 = actual nit table id
    {
        return 0;
    }
    orgDataPtr = section;
    nextIndicator = section[5] & 0x01;
    if(nextIndicator == 0)
        return 0;

    pNitInfoForSearch->networkId = SI_MERGE_U16(&section[3]);
    network_descriptors_length = (section[8]&0x0f)<<8 | section[9];
    WARNING("network_descriptors_length[%x]\n",network_descriptors_length);
    //sectionLen = (((section[1]&0x0f)<<8) | section[2]) - 12;
    section += 10;
#if(ckydebug == 1)	
	nNetDesLen = network_descriptors_length;
	while(nNetDesLen > 0)//network descriptors.
	{
		WARNING("2 init NITTSInfo\n");
		// pNITTSInfo = newNITTSInfo();
        //if(NULL == pNITTSInfo)
        {
            WARNING("malloc NITTSInfo fail!\n");
        //   return 1;   //�͵�������
        }		
		nLeftNetDesLen =  dvbt2_parse_nit_descriptor_loop2(pNITTSInfo, section);
		WARNING("1 nLeftNetDesLen[%d],nNetDesLen[%d]\n",nLeftNetDesLen,nNetDesLen);
		nNetDesLen = nNetDesLen - (nLeftNetDesLen+2);
		section += (2+nLeftNetDesLen);
		WARNING("2 nLeftNetDesLen[%d],nNetDesLen[%d]\n",nLeftNetDesLen,nNetDesLen);
	}
#endif	
    section += network_descriptors_length;  //����̨�龰��,���ڲ���network descriptors.
    
    transport_stream_loop_length = (section[0]&0x0f)<<8 | section[1];
    section += 2;
    nLeftLen = transport_stream_loop_length;
    pNITTSInfoList = pNitInfoForSearch->pList_NITTSInfo;
    while(nLeftLen > 0)
    {
    	//if(NULL == pNITTSInfo)
    	//{
      		pNITTSInfo = newNITTSInfo();
       		if(NULL == pNITTSInfo)
       		{
           		WARNING("malloc NITTSInfo fail!\n");
           		return 1;   //�͵�������
        	}
    	//}
        pNITTSInfo->ts_id = SI_MERGE_U16(&section[0]);
        pNITTSInfo->orig_ts_id = SI_MERGE_U16(&section[2]);
        //pNITTSInfo->nRemoteControlKeyId = DEFAULT_REMOTE_CONTROL_KEY_ID;
		for(i = 0;i<servList->servNum;i++)///����Ҫ�޸�
		{
	    	pNITTSInfo->nRemoteControlKeyId[i] = DEFAULT_REMOTE_CONTROL_KEY_ID;
		}
        nLeftDescLoopLen = transport_descriptors_length = section[4]&0x0f | section[5];

        nLeftLen -= (6+transport_descriptors_length);
        if(nLeftLen < 0)
        {
            WARNING("fatal error! nLeftLen[%d] wrong!\n");
			WARNING("3 NITTSInfo delete\n");
            deleteNITTSInfo(pNITTSInfo);
            goto _exit_nit_section_parse;
        }
        
        section += 6;
		WARNING("ts_des_len[%d]\n",transport_descriptors_length);
        while(nLeftDescLoopLen > 0)
        {
            dvbt2_parse_nit_descriptor_loop2(pNITTSInfo, section);
            nLeftDescLoopLen -= (2+section[1]);
            section += (2+section[1]);
        }
        
        tmpret = pNITTSInfoList->AppendElem(pNITTSInfoList, (void*)pNITTSInfo);
        if(tmpret!=EPDK_OK)
        {
        	WARNING("4 NITTSInfo delete\n");
            deleteNITTSInfo(pNITTSInfo);
            WARNING("AppendElem NitTsInfo fail!\n");
            return 1;
        }
    }
    
_exit_nit_section_parse:
    if((*sectionNum) != orgDataPtr[7])     // step-4 check the last section
    {
        (*sectionNum) ++;
        return 0;
    }
    return 1;
}

static __s32 dvbt2_parse_tdt_section(__u8 *section, maple_time_t* pTdt, __s32 sectionLen)
{   
    __u32           startTime = 0;
    __u32           year = 0;
    __u32           month = 0;
    __u32           day = 0;
    __u32           tmp = 0;
    maple_weekday_t weekDay = Sunday;
    __u16           u16TotalParseLength = 0;
    __u16           u16DescriptorLength = 0;
    __u16           u16TotalDescriptorLength = 0;
    __u8            u8Tag = 0;
    __u8*           pu8Descriptor = NULL;
    
	if((section[0] != 0x70) && (section[0] != 0x73))           //0x70 TDT 0x73:TOT
        return 0;
	else if(section[0] == 0x73)
	{
	    if(Crc32Compute(0xffffffff, section, sectionLen) != 0)                     // data error
	        return 0;     
	}  
   
    pTdt->MJD       = section[3]<<8 | section[4];
    MJD2YearMonthDate(section[3]<<8 | section[4], &year, &month, &day);
	GetWeekDay(year, month, day, &tmp);
	weekDay = (maple_weekday_t)tmp;
        
    pTdt->year      = (__u16)year;
    pTdt->month     = (__u8)month;
    pTdt->day       = (__u8)day;
    pTdt->weekDay   = weekDay;
	startTime       = section[5]<<16 | section[6]<<8 | section[7];
	pTdt->hour      = dvbt2_bcd_to_int(startTime >>16);
	pTdt->min       = dvbt2_bcd_to_int((startTime & 0x00ff00)>>8);
	pTdt->sec       = dvbt2_bcd_to_int(startTime & 0x0000ff);
	
	if(pTdt->sec >= 60)
	{ 
		pTdt->sec -= 60;
	    pTdt->min ++;
	}
	
	if(pTdt->min >= 60)
	{ 
		pTdt->min -= 60;
		pTdt->hour ++;
	}
	
	if(pTdt->hour >= 24)
	    pTdt->hour -= 24;
	 
    // Reset local time offset descriptor info
    memset( &pTdt->stTotLto, 0, sizeof(pTdt->stTotLto));

    u16TotalDescriptorLength = section[8] & 0x0FFF;
    pu8Descriptor = &section[10];

    __wrn("DescriptorLength:%d, pu8Descriptor:%p\n",u16TotalDescriptorLength,pu8Descriptor);
    if((section[0] == 0x73) && (u16TotalDescriptorLength != 0) && (NULL != pu8Descriptor) )
    {
        while ( u16TotalParseLength < u16TotalDescriptorLength )
        {
            u8Tag = pu8Descriptor[0];   //descripter Tag
            __wrn("u8Tag:%#x\n",u8Tag);
            if (u8Tag == 0x58)  //0x58:local_time_offset_descriptor
            {
                __u8   u8Length = 0;
                __u8  *pu8Data = NULL;
                
                u8Length = 0;
                pu8Data = &pu8Descriptor[2];
                while ( u8Length < pu8Descriptor[1] )
                {
                    // Store max 20 pTdt->stTotLto information. (LG spec.)
                    if (pTdt->stTotLto.u8NumofRegion < MAX_LTO_REGION_NUM )
                    {
                        memcpy(pTdt->stTotLto.aLTOInfo[pTdt->stTotLto.u8NumofRegion].eCountry, pu8Data, 3);
                        __wrn("eCountryIdx %s\n",pTdt->stTotLto.aLTOInfo[pTdt->stTotLto.u8NumofRegion].eCountry);
                        pTdt->stTotLto.aLTOInfo[pTdt->stTotLto.u8NumofRegion].u8RegionId = (pu8Data[3] >> 2) & 0x3F;
                        __wrn("region index %bu\n",pTdt->stTotLto.aLTOInfo[pTdt->stTotLto.u8NumofRegion].u8RegionId);
                        pTdt->stTotLto.aLTOInfo[pTdt->stTotLto.u8NumofRegion].u8LTOPolarity = pu8Data[3] & 0x01;
                        pTdt->stTotLto.aLTOInfo[pTdt->stTotLto.u8NumofRegion].u16LTO = (pu8Data[4] << 8) | pu8Data[5];
                        __wrn("pTdt->stTotLto %#lx\n",pTdt->stTotLto.aLTOInfo[pTdt->stTotLto.u8NumofRegion].u16LTO);
                        memcpy( pTdt->stTotLto.aLTOInfo[pTdt->stTotLto.u8NumofRegion].au8TimeOfChange, &pu8Data[6], 5 );
                        pTdt->stTotLto.aLTOInfo[pTdt->stTotLto.u8NumofRegion].u16NextTimeOffset = (pu8Data[11] << 8) | pu8Data[12];
                        __wrn("pTdt->TimeOffse %#x\n",pTdt->stTotLto.aLTOInfo[pTdt->stTotLto.u8NumofRegion].u16NextTimeOffset);
                        pTdt->stTotLto.u8NumofRegion++;
                    }
                    else
                    {
                        break;
                    }
                    u8Length += 13;
                    pu8Data += 13;
                }

            }
            else
            {
                esKRNL_TimeDly(2);
            }
            /* move to next descriptor */
            u16DescriptorLength = pu8Descriptor[1] + 2;
            u16TotalParseLength += u16DescriptorLength;
            pu8Descriptor += u16DescriptorLength;
        }
    }   
	return 1;
}

__s32 dvbt2_pat_request_buffer(void *p_arg)
{  
   __md_buf_t* pbuffer = NULL;

   pbuffer = ( __md_buf_t*) p_arg;

   if(g_dvbt2Ctx->endPatSection == 1)      // has parsed pat section success
   {
       pbuffer->buf     =   NULL;
       pbuffer->bufSize =   0;
   }
  else  if((g_dvbt2Ctx->dvbt2SectionBuf[PAT_BUF_IDX]==NULL)||(g_dvbt2Ctx->isBufFull[PAT_BUF_IDX]==1))
   {
       pbuffer->buf     =   NULL;
       pbuffer->bufSize =   0;
       eLIBs_printf("the pat section is used,.\n");
   }
   else
   {
       pbuffer->buf     =   g_dvbt2Ctx->dvbt2SectionBuf[PAT_BUF_IDX];
       pbuffer->bufSize =   DVBT2_SECTION_BUF_SIZE;
   }
   
   return 0;
}

__s32 dvbt2_pmt_request_buffer(void *p_arg)
{  
   __md_buf_t* pbuffer = NULL;

   pbuffer = ( __md_buf_t*) p_arg;

   if(g_dvbt2Ctx->endPmtSection == 1)      // has parsed pat section success
   {
       pbuffer->buf     =   NULL;
       pbuffer->bufSize =   0;
   }
  else if((g_dvbt2Ctx->dvbt2SectionBuf[PMT_BUF_IDX]==NULL)||(g_dvbt2Ctx->isBufFull[PMT_BUF_IDX]==1))
   {
       pbuffer->buf     =   NULL;
       pbuffer->bufSize =   0;
       eLIBs_printf("the pmt section is used,.\n");
   }
   else
   {
       pbuffer->buf     =   g_dvbt2Ctx->dvbt2SectionBuf[PMT_BUF_IDX];
       pbuffer->bufSize =   DVBT2_SECTION_BUF_SIZE;
   }
   return 0;
}

__s32 dvbt2_sdt_request_buffer(void *p_arg)
{  
   __md_buf_t* pbuffer = NULL;

   pbuffer = ( __md_buf_t*) p_arg;

   if(g_dvbt2Ctx->endSdtSection == 1)      // has parsed pat section success
   {
       pbuffer->buf     =   NULL;
       pbuffer->bufSize =   0;
   }
  else if((g_dvbt2Ctx->dvbt2SectionBuf[SDT_BUF_IDX]==NULL)||(g_dvbt2Ctx->isBufFull[SDT_BUF_IDX]==1))
   {
       pbuffer->buf     =   NULL;
       pbuffer->bufSize =   0;
       eLIBs_printf("the sdt section is used,.\n");
   }
   else
   {
       pbuffer->buf     =   g_dvbt2Ctx->dvbt2SectionBuf[SDT_BUF_IDX];
       pbuffer->bufSize =   DVBT2_SECTION_BUF_SIZE;
   }
   return 0;
}

__s32 dvbt2_nit_request_buffer(void *p_arg)
{  
    __md_buf_t* pbuffer = NULL;

    pbuffer = (__md_buf_t*)p_arg;

    if(g_dvbt2Ctx->endNitSection == 1)      // has parsed nit section success
    {
        pbuffer->buf     =   NULL;
        pbuffer->bufSize =   0;
    }
    else if((g_dvbt2Ctx->dvbt2SectionBuf[NIT_BUF_IDX]==NULL)||(g_dvbt2Ctx->isBufFull[NIT_BUF_IDX]==1))
    {
        pbuffer->buf     =   NULL;
        pbuffer->bufSize =   0;
        eLIBs_printf("the nit section is used. check!\n");
    }
    else
    {
        pbuffer->buf     =   g_dvbt2Ctx->dvbt2SectionBuf[NIT_BUF_IDX];
        pbuffer->bufSize =   DVBT2_SECTION_BUF_SIZE;
    }
    return 0;
}

__s32 dvbt2_tdt_request_buffer(void *p_arg)
{  
   __md_buf_t* pbuffer = NULL;

   pbuffer = ( __md_buf_t*) p_arg;

   if((g_dvbt2Ctx->dvbt2SectionBuf[TDT_BUF_IDX]==NULL)||(g_dvbt2Ctx->isBufFull[TDT_BUF_IDX]==1))
   {
       pbuffer->buf     =   NULL;
       pbuffer->bufSize =   0;
       eLIBs_printf("the tdt section is used,.\n");
   }
   else
   {
       pbuffer->buf     =   g_dvbt2Ctx->dvbt2SectionBuf[TDT_BUF_IDX];
       pbuffer->bufSize =   DVBT2_SECTION_BUF_SIZE;
   }
   return 0;
}

__s32 dvbt2_eit_request_buffer(void *p_arg)
{  
   __md_buf_t* pbuffer = NULL;

   pbuffer = ( __md_buf_t*) p_arg;

   if((g_dvbt2Ctx->dvbt2SectionBuf[EIT_BUF_IDX]==NULL)||(g_dvbt2Ctx->isBufFull[EIT_BUF_IDX]==1))
   {
       pbuffer->buf     =   NULL;
       pbuffer->bufSize =   0;
       eLIBs_printf("the heit section is used,.\n");
   }
   else
   {
       pbuffer->buf     =   g_dvbt2Ctx->dvbt2SectionBuf[EIT_BUF_IDX];
       pbuffer->bufSize =   DVBT2_SECTION_BUF_SIZE;
   }

   return 0;
}

__s32 dvbt2_pat_update_data(void *p_arg)             // ��ȡ��section�����ݳ���
{ 
   __u32 *dataLen   = NULL;
   __u32 patDataLen = 0;

  dataLen = (__u32 *)p_arg;
  patDataLen = *dataLen;
  
  g_dvbt2Ctx->isBufFull[PAT_BUF_IDX] = 1;
  
  if(patDataLen > 0)
  {
    if (Crc32Compute(0xffffffff, g_dvbt2Ctx->dvbt2SectionBuf[PAT_BUF_IDX], patDataLen))
    {
	   WARNING("crc check error.\n");
       g_dvbt2Ctx->isBufFull[PAT_BUF_IDX]  = 0;
	   return -1;
    }
  }

  g_dvbt2Ctx->endPatSection = dvbt2_parse_pat_section(g_dvbt2Ctx->pPatInfoForSearch, g_dvbt2Ctx->dvbt2SectionBuf[PAT_BUF_IDX], &g_dvbt2Ctx->curPatSecNum);
  g_dvbt2Ctx->isBufFull[PAT_BUF_IDX]  = 0;

  return 0;
}

__s32 dvbt2_pmt_update_data(void *p_arg)             // ��ȡ��section�����ݳ���
{ 
   
   __u32 pmtDataLen = 0;
   __u32 *dataLen = NULL;

  dataLen = (__u32 *)p_arg;
  pmtDataLen = *dataLen;
  
  g_dvbt2Ctx->isBufFull[PMT_BUF_IDX] = 1;
  
  if(pmtDataLen > 0)
  {
    if (Crc32Compute(0xffffffff, g_dvbt2Ctx->dvbt2SectionBuf[PMT_BUF_IDX], pmtDataLen))
    {
	   WARNING("crc check error.\n");
       g_dvbt2Ctx->isBufFull[PMT_BUF_IDX]  = 0;
	   return -1;
    }
  }

  g_dvbt2Ctx->endPmtSection = dvbt2_parse_pmt_section(g_dvbt2Ctx->pPmtInfoForSearch, g_dvbt2Ctx->dvbt2SectionBuf[PMT_BUF_IDX], &g_dvbt2Ctx->curPmtSecNum);
  g_dvbt2Ctx->isBufFull[PMT_BUF_IDX]  = 0;
  
  return 0;
}

__s32 dvbt2_sdt_update_data(void *p_arg)             // ��ȡ��section�����ݳ���
{ 
   __u32 sdtDataLen = 0;
   __u32 *dataLen = NULL;

  dataLen = (__u32 *)p_arg;
  sdtDataLen = *dataLen;
  
  g_dvbt2Ctx->isBufFull[SDT_BUF_IDX] = 1;
  
  if(sdtDataLen > 0)
  {
    if (Crc32Compute(0xffffffff, g_dvbt2Ctx->dvbt2SectionBuf[SDT_BUF_IDX], sdtDataLen))
    {
	   WARNING("crc check error.\n");
       g_dvbt2Ctx->isBufFull[SDT_BUF_IDX]  = 0;
	   return -1;
    }
  }

  g_dvbt2Ctx->endSdtSection = dvbt2_parse_sdt_section(g_dvbt2Ctx->pSdtInfoForSearch, g_dvbt2Ctx->dvbt2SectionBuf[SDT_BUF_IDX], &g_dvbt2Ctx->curSdtSecNum);
  g_dvbt2Ctx->isBufFull[SDT_BUF_IDX]  = 0;
  
  return 0;
}

__s32 dvbt2_nit_update_data(void *p_arg)             // ��ȡ��section�����ݳ���, total size
{ 
   __u32 nitDataLen = 0;
   __u32 *dataLen = NULL;

  dataLen = (__u32 *)p_arg;
  nitDataLen = *dataLen;
  
  g_dvbt2Ctx->isBufFull[NIT_BUF_IDX] = 1;
  
  if(nitDataLen > 0)
  {
    if (Crc32Compute(0xffffffff, g_dvbt2Ctx->dvbt2SectionBuf[NIT_BUF_IDX], nitDataLen))
    {
	   WARNING("NIT crc check error.\n");
       g_dvbt2Ctx->isBufFull[NIT_BUF_IDX]  = 0;
	   return -1;
    }
  }

  g_dvbt2Ctx->endNitSection = dvbt2_parse_nit_section_for_search(g_dvbt2Ctx->pNitInfoForSearch, g_dvbt2Ctx->dvbt2SectionBuf[NIT_BUF_IDX], &g_dvbt2Ctx->curNitSecNum);
  g_dvbt2Ctx->isBufFull[NIT_BUF_IDX]  = 0;
  
  return 0;
}


__s32 dvbt2_tdt_update_data(void *p_arg)             // ��ȡ��section�����ݳ���
{ 
   __u32 tdtDataLen = 0;
   __u32 *dataLen = NULL;

  dataLen = (__u32 *)p_arg;
  tdtDataLen = *dataLen;
  
  g_dvbt2Ctx->isBufFull[TDT_BUF_IDX] = 1;
  
  if(tdtDataLen > 0)
  {
    //if (Crc32Compute(0xffffffff, g_dvbt2Ctx->dvbt2SectionBuf[TDT_BUF_IDX], tdtDataLen))
    //{
	//   WARNING("crc check error.\n");
    //   g_dvbt2Ctx->isBufFull[TDT_BUF_IDX]  = 0;
	//   return -1;
    //}
  }
  dvbt2_parse_tdt_section(g_dvbt2Ctx->dvbt2SectionBuf[TDT_BUF_IDX], &g_dvbt2Ctx->curStreamTime, tdtDataLen);
  g_dvbt2Ctx->isBufFull[TDT_BUF_IDX] = 0;
  
  return 0;
}

__s32 dvbt2_eit_update_data(void *p_arg)             // ��ȡ��section�����ݳ���
{ 
   __u8  i           = 0;
   __u32 eitDataLen = 0;
   __u32 *dataLen = NULL;

  dataLen = (__u32 *)p_arg;
  eitDataLen = *dataLen;
  
  g_dvbt2Ctx->isBufFull[EIT_BUF_IDX] = 1;
  
  if(eitDataLen > 0)
  {
    if (Crc32Compute(0xffffffff, g_dvbt2Ctx->dvbt2SectionBuf[EIT_BUF_IDX], eitDataLen))
    {
	   WARNING("eit crc check error.\n");
       g_dvbt2Ctx->isBufFull[EIT_BUF_IDX]  = 0;
	   return -1;
    }
  }

  while(1)
  {
     if(dvbt2_get_mutex(g_dvbt2Ctx->epgMutex) == 0)
        break;
     esKRNL_TimeDly(1);
     i++;
     if(i == 100)
     {
         g_dvbt2Ctx->isBufFull[EIT_BUF_IDX]  = 0;
	     return -1;
     }
  }
  eit_pool_append_section(0, g_dvbt2Ctx->dvbt2SectionBuf[EIT_BUF_IDX], eitDataLen);
  g_dvbt2Ctx->isBufFull[EIT_BUF_IDX]  = 0;
  dvbt2_release_mutex(g_dvbt2Ctx->epgMutex);
  return 0;
}


void dvbt2_set_service_type(void* p_arg)
{
    __u8 i = 0;
    for(i=0; i<servList->servNum; i++)
    {
        if(servList->pServItems[i].vdStreamNum > 0)
        {
            servList->pServItems[i].servType = MAPLE_SERV_TYPE_TV;
        }
        else if(servList->pServItems[i].adStreamNum > 0)
        {
            servList->pServItems[i].servType = MAPLE_SERV_TYPE_RADIO;
        }
        else if(servList->pServItems[i].subtitleStreamNum > 0)
        {
            servList->pServItems[i].servType = MAPLE_SERV_TYPE_TELETEXT;      
        }
    }
}


/*******************************************************************************
Function name: PmtFillServItem
Description: 
    1. servIdĬ���Ѿ������.
Parameters: 
    
Return: 
    
Time: 2012/6/19
*******************************************************************************/
__s32 PmtFillServItem(maple_serv_item_t *pServItem, PMTInfo *pPmtInfoForSearch, __u16 nPmtPid)
{
    __s32   i;
    __s32   nStreamFlag = 3;    //0:video, 1:audio; 2:subtitle, 3:unknown
    __s32   nCodecType; //MAPLE_VID_TYPE_H264��

    if(pServItem->servId != pPmtInfoForSearch->nServId)
    {
        pServItem->servId = pPmtInfoForSearch->nServId;
    }
    
    pServItem->pcrPid = pPmtInfoForSearch->pcrPid;
    if(pServItem->pcrPid != 0x1fff)
		pServItem->hasPcr = 1;
	else
		pServItem->hasPcr = 0;

    pServItem->pmtId = nPmtPid;
		WARNING("pServItem->pmtId = 0x%x\n",pServItem->pmtId);
   // if(pServItem->pmtId >= PMT_PID_FOR_ONESEG && pServItem->pmtId <= PMT_PID_FOR_ONESEG + 7)
    //{
   //     pServItem->nOneSegFlag = 1;
   // }
   // else
  //  {
  //      pServItem->nOneSegFlag = 0;
  //  }
    pServItem->vdStreamNum = 0;
    pServItem->adStreamNum = 0;
    pServItem->subtitleStreamNum = 0;
    for(i=0;i<pPmtInfoForSearch->nStreamNum;i++)
    {
        switch(pPmtInfoForSearch->streamInfoArray[i].nStreamType)
        {
            case STREAM_TYPE_AUDIO_MPEG1:
            case STREAM_TYPE_AUDIO_MPEG2:
            {
        	nStreamFlag	= 1;
                nCodecType = MAPLE_AUD_TYPE_MPEG;
                break;
            }
            case STREAM_TYPE_AUDIO_AAC:
            case STREAM_TYPE_AUDIO_MPEG4:
            {
                nStreamFlag	= 1;
                nCodecType = MAPLE_AUD_TYPE_AAC;
                break;
            }
            case STREAM_TYPE_AUDIO_AC3:
            case STREAM_TYPE_AUDIO_AC3_:
            case STREAM_TYPE_AUDIO_EAC3:
            case STREAM_TYPE_AUDIO_AC3_TRUEHD:
            {
                nStreamFlag	= 1;
                nCodecType = MAPLE_AUD_TYPE_AC3;
                break;
            }
            case STREAM_TYPE_AUDIO_DTS:
            case STREAM_TYPE_AUDIO_DTS_:
            case STREAM_TYPE_AUDIO_HDMV_DTS:
            case STREAM_TYPE_AUDIO_DTS_HRA:
            case STREAM_TYPE_AUDIO_DTS_MA:
            {
                nStreamFlag	= 1;
                nCodecType = MAPLE_AUD_TYPE_DTS;
                break;
            }
            case STREAM_TYPE_VIDEO_MPEG1:
            case STREAM_TYPE_VIDEO_MPEG2:
            {
                nStreamFlag = 0;
                nCodecType = MAPLE_VID_TYPE_MPEG2;
                break;
            }
            case STREAM_TYPE_VIDEO_H264:
            {
                nStreamFlag = 0;
                nCodecType = MAPLE_VID_TYPE_H264;
                break;
            }
            case STREAM_TYPE_VIDEO_VC1:
            case STREAM_TYPE_VIDEO_MPEG4:
            {
                nStreamFlag = 3;
                nCodecType = MAPLE_VID_TYPE_UNKNOWN;
                break;
            }
            case STREAM_TYPE_SUBTITLE_DVB:
            {
                nStreamFlag = 2;
                nCodecType = MAPLE_SUBT_TYPE_DVBSUB;
                break;
            }
			   case STREAM_TYPE_SUBTITLE_TELETEXT:
            {
                nStreamFlag = 2;
                nCodecType = MAPLE_SUBT_TYPE_TTXT;
                break;
            }
            case STREAM_TYPE_SUBTITLE_ARIB:
            {
                nStreamFlag = 2;
                nCodecType = MAPLE_SUBT_TYPE_ARIB;
                break;
            }
            default:
            {
                WARNING("unknown streamtype[0x%x]\n", pPmtInfoForSearch->streamInfoArray[i].nStreamType);
                nStreamFlag = 3;
                nCodecType = MAPLE_VID_TYPE_UNKNOWN;
                break;
            }
        }
        
        switch(nStreamFlag)
        {
            case 0: //video
            {
                if(pServItem->vdStreamNum >= MAPLE_MAX_VD_STREAM_NUM)
                {
                    WARNING("vdStreamNum[%d] is full,ignore!\n", pServItem->vdStreamNum);
                    break;
                }
                pServItem->vdInfo[pServItem->vdStreamNum].pid = pPmtInfoForSearch->streamInfoArray[i].nStreamPid;
                pServItem->vdInfo[pServItem->vdStreamNum].vdCodeType = (maple_vd_codec_e)nCodecType;
                pServItem->vdStreamNum++;
                break;
            }
            case 1: //audio
            {
                if(pServItem->adStreamNum >= MAPLE_MAX_AD_STREAM_NUM)
                {
                    WARNING("adStreamNum[%d] is full,ignore!\n", pServItem->adStreamNum);
                    break;
                }
                pServItem->adInfo[pServItem->adStreamNum].pid = pPmtInfoForSearch->streamInfoArray[i].nStreamPid;
                pServItem->adInfo[pServItem->adStreamNum].adCodeType = (maple_ad_codec_e)nCodecType;
                if(pPmtInfoForSearch->streamInfoArray[i].nStreamNameLen)
                {
                    safeStringCopy(pServItem->adInfo[pServItem->adStreamNum].adStreamName, MAPLE_MAX_NAME_LEN, 
                        pPmtInfoForSearch->streamInfoArray[i].StreamName, pPmtInfoForSearch->streamInfoArray[i].nStreamNameLen);
                }
                pServItem->adStreamNum++;
                break;
            }
            case 2: //subtitle
            {
                if(pServItem->subtitleStreamNum >= MAPLE_MAX_ST_STREAM_NUM)
                {
                    WARNING("subtitleStreamNum[%d] is full,ignore!\n", pServItem->subtitleStreamNum);
                    break;
                }
                pServItem->subtInfo[pServItem->subtitleStreamNum].subtPid = pPmtInfoForSearch->streamInfoArray[i].nStreamPid;
                pServItem->subtInfo[pServItem->subtitleStreamNum].subtCodeType = (maple_subt_codec_e)nCodecType;
                if(pPmtInfoForSearch->streamInfoArray[i].nStreamNameLen)
                {
                    safeStringCopy(pServItem->subtInfo[pServItem->subtitleStreamNum].subtName, MAPLE_MAX_NAME_LEN, 
                        pPmtInfoForSearch->streamInfoArray[i].StreamName, pPmtInfoForSearch->streamInfoArray[i].nStreamNameLen);
                }
                pServItem->subtitleStreamNum++;
                break;
            }
            default:
                break;
        }
    }

    if(MAPLE_SERV_TYPE_UNKNOWN == pServItem->servType)  //�����û����,�����������
    {
        if(pServItem->vdStreamNum > 0)
            pServItem->servType = MAPLE_SERV_TYPE_TV;
        else if(pServItem->adStreamNum > 0)
            pServItem->servType = MAPLE_SERV_TYPE_RADIO;
        else if(pServItem->subtitleStreamNum > 0)
            pServItem->servType = MAPLE_SERV_TYPE_TELETEXT;

        WARNING("when pmt parse, service type still unknow, Judge_Month service type from es streams numbers, servtype[%d]\n", pServItem->servType);
    }
    return EPDK_OK;
}
/*******************************************************************************
Function name: PmtSupplementServiceInfo
Description: 
    1.��̨ʱ,���ݵ�ǰ�ѵ���pmt�����Ϣ,��servlist���ҵ���Ӧ�����д��Ϣ
    2.pmt�����Ϣ,��Ҫ��дvideo/audio/subtitle pid, pcrPid, streamtype, streamname, pmtpid��oneseg/fullsegҲ��������,��Ϊ���治һ�����.���service type��û��,��ôҲ������ȷ��.˳��Ƚ�һ��.
Parameters: 
    
Return: 
    EPDK_OK:�ɹ��ҵ�service,����д;
    EPDK_FAIL:
Time: 2012/6/18
*******************************************************************************/
__s32 PmtSupplementServiceInfo(maple_serv_list_t *pServList, PMTInfo *pPmtInfoForSearch, __u16 nPmtPid)
{
    __s32   i;
    maple_serv_item_t   *pServItem;
    //����service.
    WARNING("cky servNum=%d\n",pServList->servNum);
    for(i=0;i<pServList->servNum;i++)
    {
    		WARNING("cky servid =%d\n",pPmtInfoForSearch->nServId);
        if(pServList->pServItems[i].servId == pPmtInfoForSearch->nServId)
        {
            pServItem = &pServList->pServItems[i];
            break;
        }
    }
    if(NULL == pServItem)
    {
        WARNING("not find serviceid[0x%x]\n", pPmtInfoForSearch->nServId);
        return EPDK_FAIL;
    }
    PmtFillServItem(pServItem, pPmtInfoForSearch, nPmtPid);
    return EPDK_OK;
}

/*******************************************************************************
Function name: PmtAddNewServiceInfo
Description: 
    //��PmtSupplementServiceInfo()��֮ͬ������������ȫ������һ��service,��PmtSupplementServiceInfo()�����ҵ��ٲ�����д.

    1.sdt: serviceid,servicetype, servicename, isCAFree, tsId, orgNetworkId(��Ӧmaple_serv_item_t->networkId)
    2.pat��: pmt_pid,  tsId
    3.pmt�����Ϣ,��Ҫ��дvideo/audio/subtitle pid, pcrPid, streamtype, streamname, pmtpid Ҳ��������,��Ϊ���治һ�����.���service type��û��,��ôҲ������ȷ��.˳��Ƚ�һ��.
Parameters: 
    
Return: 
    EPDK_OK;
Time: 2012/6/19
*******************************************************************************/
__s32 PmtAddNewServiceInfo(maple_serv_list_t *pServList, PMTInfo *pPmtInfoForSearch, __u16 nPmtPid) 
{
    maple_serv_item_t   *pServItem = NULL;
    if(pServList->servNum >= pServList->nServItemsCount)
    {
        WARNING("servList is full!\n");
        return EPDK_FAIL;
    }
    pServItem = &pServList->pServItems[pServList->servNum];

    //�����Ȱ�pmt�����Ķ���������
    pServItem->servId = pPmtInfoForSearch->nServId;
    pServItem->servType = MAPLE_SERV_TYPE_UNKNOWN;
    safeStringCopy(pServItem->servName, MAPLE_MAX_NAME_LEN, DEFAULT_SERVICE_NAME, strlen(DEFAULT_SERVICE_NAME));
    pServItem->isCAFree = 1;
    PmtFillServItem(pServItem, pPmtInfoForSearch, nPmtPid);
    pServList->servNum++;
    return EPDK_OK;
}

/************************************************************
Function name: dvbt2_enble_remotecontrlkey
Description: 
    �Ƿ����Ƶ�����ڲ���remote_contrl_key,
    Ĭ��1:�򿪣�remotecontrlkeyID������256��̨��
    0:������Զ���Ŵ�350��ʼС��350��̨
Parameters: 
    
Return: 
    
Time: 2012/3/15
*************************************************************/
__s32 dvbt2_enble_remotecontrlkey(__u8 aux)
{
	maple_serv_list_t *servList;
	NITTSInfo           *pNITTSInfo ;
	__u8 i = 0;
	__u8 k = 0;
	WARNING("aux = %d\n",aux);
	if(aux)
	{
		WARNING("open video LCN :nRemoteControlKeyId\n");
		for(i=0;i<servList->servNum;i++)
	 	{	
   			WARNING("pNITTSInfo->ServIdArray[i] = 0x%x,pNITTSInfo->nRemoteControlKeyId[i]=0x%x\n",
						pNITTSInfo->ServIdArray[i],pNITTSInfo->nRemoteControlKeyId[i]);
     	 	if(servList->pServItems[i].servId==pNITTSInfo->ServIdArray[i])
     	 		{
     	 			
        			servList->pServItems[i].nRemoteControlKeyId =  pNITTSInfo->nRemoteControlKeyId[i];
     	 		}
			else
				{
					WARNING("TS con't find LCN,now auto LCN\n");
				 	servList->pServItems[i].nRemoteControlKeyId  = START_REMOTE_CONTROL_KEY_ID+k;
					k++;	 	
			   }
    	} 
	}
	else
	{
					WARNING("AUTO LCN :nRemoteControlKeyId\n");
				 	servList->pServItems[i].nRemoteControlKeyId  = START_REMOTE_CONTROL_KEY_ID+k;
					k++;	 	
	}
	return EPDK_OK;
}


/*******************************************************************************
Function name: dvbt2_scan_task
Description: 
    1.��Ե�Ƶ�������ĺ���
    2.servListָ��Ŀռ��ǵ����߷����,����ֻ�������������service��д��ȥ,���ǲ������maple�Լ�
    ��service�б�.����service�б�������Ľӿ�.
Parameters: 
    
Return: 
    
Time: 2012/3/15
*******************************************************************************/
 void dvbt2_scan_task(void* arg)
{   
	
    __u32 									i = 0;
	 __u32     								j = 0;
	 __u32										k = 0;
    //__u32 							maxDelayNum;
    maple_demod_info_t  	demodParam;
	__s32               nWaitSectionTime = 0;
	   //maple_serv_item_t   *curServItem    = NULL;
    __u16               nRemoteControlKeyId[TS_MAX_SERVICE_NUM];
	 __u16               tsId = 0;
    __s32               nTmp;
    __s32               nIdx;
    NITTSInfo           *pNITTSInfo = NULL;
    __u32               servIndex  = 0;
    __s32               nServiceNum = 0;//��n��PLP�Ľ�Ŀ�б�.
    __s32               tmpret;
	
    __s32               nSdtVChanId = -1;
    __s32               nPatVChanId = -1;
    __s32               nPmtVChanId = -1;
  	__s32               nNitVChanId = -1;
	__s32               old_servNum = 0;//�Ѿ��ѵ��Ľ�Ŀ����
	//maple_serv_list_t	*tmpservList = NULL; //��n��PLP�Ľ�Ŀ�б�.
		
    servList = (maple_serv_list_t *)arg;
       //��ʼ��servList.
    servList->servNum       = 0;
    servList->curServIdx    = 0;
    servList->nServItemsCount = MAPLE_MAX_SERV_NUM;
    memset(servList->pServItems, 0, sizeof(maple_serv_item_t)*MAPLE_MAX_SERV_NUM);
	//memset(tmpservList->pServItems, 0, sizeof(maple_serv_item_t)*MAPLE_MAX_SERV_NUM);
	
  	//* step 1, tune.
	demodParam.frequency = g_dvbt2Ctx->frequency;
	demodParam.bandwidth = g_dvbt2Ctx->bandwidth;
	demodParam.plpindex  = 0;
	demodParam.plppid	 = 0;
	demodParam.plpnumber = 0;
    if(dvbt2_tune(&demodParam) != 0)
        goto exit_dvbt2_search_service_task;
	 dvbt2_get_tuner_param(&demodParam);

	if(demodParam.t_t2_flag == 2)//dvbt
	{
		
	    g_dvbt2Ctx->curSdtSecNum  = 0;
		 g_dvbt2Ctx->endSdtSection = 0;
	    g_dvbt2Ctx->isBufFull[SDT_BUF_IDX] = 0;
		  if(g_dvbt2Ctx->pSdtInfoForSearch)
	    {
	        WARNING("fatal error! why g_dvbCtx->pSdtInfoForSearch[0x%x]!=NULL?\n", g_dvbt2Ctx->pSdtInfoForSearch);
	        deleteSDTInfo(g_dvbt2Ctx->pSdtInfoForSearch);
	        g_dvbt2Ctx->pSdtInfoForSearch = NULL;
	    }
	    g_dvbt2Ctx->pSdtInfoForSearch = newSDTInfo();
	    if(NULL == g_dvbt2Ctx->pSdtInfoForSearch)
	    {
	        WARNING("malloc PatInfo fail,skip nit search!\n");
	        goto exit_dvbt2_search_service_task;
	    }
			
		dvbt2_open_section_filter(SDT_BAT_PID,0,&nSdtVChanId);
	    nWaitSectionTime = 0;
		while(g_dvbt2Ctx->endSdtSection == 0)
		{
			if(dvbt2_check_task_delete_request(EXEC_prioself)==OS_TASK_DEL_REQ)        // time is overflow
			{
	            //dvb_set_service_type((void*)0);
				  dvbt2_close_section_filter(SDT_BAT_PID,nSdtVChanId);
					 servList->servNum = 0;  //��Ϊ��;�˳�,���Ը�Ƶ���service������¼.
					goto exit_dvbt2_search_service_task;
	        }
			   esKRNL_TimeDly(10);
	        nWaitSectionTime++;
	        if(nWaitSectionTime * 100 > WAIT_SDT_TIMEOUT)
	        {
	            WARNING("wait sdt timeout! no sdt!\n");
	            break;
	        }
		}
		dvbt2_close_section_filter(SDT_BAT_PID,nSdtVChanId);
		  //1.1 ����sdt��������д�������ͽ�����service�б�,���û��sdt�������.
	    //��Ҫ��дserviceid,servicetype, servicename, isCAFree, tsId, orgNetworkId(��Ӧmaple_serv_item_t->networkId)
	    if(g_dvbt2Ctx->endSdtSection)    //���sdtû��ȫ������,����Ϊsdt��Ч.
	    {
	        nServiceNum = g_dvbt2Ctx->pSdtInfoForSearch->pList_SDTServInfo->GetLength(g_dvbt2Ctx->pSdtInfoForSearch->pList_SDTServInfo);
	        if(nServiceNum > servList->nServItemsCount)
	        {
	            WARNING("service num[%d] > nServItemsCount[%d]\n", nServiceNum, servList->nServItemsCount);
	        }
	        //��һ��foreach()����,�����д����.˳�����.ĿǰֻҪtv, radio. teletext��ʱ��Ҫ
	        tmpret = g_dvbt2Ctx->pSdtInfoForSearch->pList_SDTServInfo->ForeachElem(g_dvbt2Ctx->pSdtInfoForSearch->pList_SDTServInfo, DListDataVisitFunc_SDTAddService, servList);
			if(tmpret != DLIST_RET_OK)
	        {
	            WARNING("fatal error! foreach ret[%d], why != OK?\n", tmpret);
	        }
	        //��tsId, orgNetworkId��ֵ
	        for(i=0;i<servList->servNum;i++)
	        {
	            servList->pServItems[i].tsId = g_dvbt2Ctx->pSdtInfoForSearch->tsId;
	            servList->pServItems[i].networkId = g_dvbt2Ctx->pSdtInfoForSearch->orgNetworkId;
	        }
	    }
	    if(g_dvbt2Ctx->pSdtInfoForSearch)
	    {
	        deleteSDTInfo(g_dvbt2Ctx->pSdtInfoForSearch);
	        g_dvbt2Ctx->pSdtInfoForSearch = NULL;
	    }
	    
		
		//* search pat.
		g_dvbt2Ctx->endPatSection = 0;
	    g_dvbt2Ctx->curPatSecNum  = 0;
	    g_dvbt2Ctx->isBufFull[PAT_BUF_IDX] = 0;
		 if(g_dvbt2Ctx->pPatInfoForSearch)
	    {
        WARNING("fatal error! why g_dvbt2Ctx->pPatInfoForSearch[0x%x]!=NULL?\n", g_dvbt2Ctx->pPatInfoForSearch);
	        deletePATInfo(g_dvbt2Ctx->pPatInfoForSearch);
	        g_dvbt2Ctx->pPatInfoForSearch = NULL;
	    }
	    g_dvbt2Ctx->pPatInfoForSearch = newPATInfo();
	    if(NULL == g_dvbt2Ctx->pPatInfoForSearch)
	    {
	        WARNING("malloc PatInfo fail,skip nit search!\n");
			  servList->servNum = 0;
	        goto exit_dvbt2_search_service_task;
	    }
		dvbt2_open_section_filter(PAT_PID,0,&nPatVChanId);
	    nWaitSectionTime = 0;
		while(g_dvbt2Ctx->endPatSection == 0)
		{   
	        if(dvbt2_check_task_delete_request(EXEC_prioself) == OS_TASK_DEL_REQ)        // time is overflow
			{
			    dvbt2_close_section_filter(PAT_PID,nPatVChanId);
		        servList->servNum = 0;
				goto exit_dvbt2_search_service_task;
			}
	          esKRNL_TimeDly(10);
	          nWaitSectionTime++;
	          if(nWaitSectionTime*10 > WAIT_PAT_TIMEOUT)    //��ʱ
	          {
	              WARNING("search pat timeout!\n");
	              break;
	          }
		}
		dvbt2_close_section_filter(PAT_PID,nPatVChanId);
		  if(g_dvbt2Ctx->endPatSection)    //pat����Ѱ��ɾ���д.
	        {
	            //2.1 ��ʼ��дmaple��service�б�, ��Ҫ��дpmt_pid, oneseg/fullseg��Ϣ,tsId, ���sdt��ɹ�����,����sdt��Ϊ׼.
	            //������pat��Ϊ׼.�����sdt��,tsId���sdt��Ƚ�,����sdt��Ϊ׼.
				//if((servList->servNum > 0)&&(servList->servNum == g_dvbt2Ctx->pPatInfoForSearch->nServNum))   //˵��sdt���Ѿ���д��servList.pat����У������
	            if((servList->servNum > 0) &&( nServiceNum == g_dvbt2Ctx->pPatInfoForSearch->nServNum))   //˵��sdt���Ѿ���д��servList.pat����У������  
				{
	                for(i=0;i<servList->servNum;i++)
	                {
	                    //Ѱ��servId
	                    for(j=1;j<=g_dvbt2Ctx->pPatInfoForSearch->nServNum;j++)//PAT��servNum ��1��ʼ
	                    {
	                        if(servList->pServItems[i].servId == g_dvbt2Ctx->pPatInfoForSearch->servInfoArray[j].nServId)
	                        {
	                            servList->pServItems[i].pmtId = g_dvbt2Ctx->pPatInfoForSearch->servInfoArray[j].pmtpid;
	                        	// if(servList->pServItems[i].pmtId >= PMT_PID_FOR_ONESEG && servList->pServItems[i].pmtId <= PMT_PID_FOR_ONESEG + 7)
	                            //{
	                            //    servList->pServItems[i].nOneSegFlag = 1;
	                          //  }
	                           // else
	                           // {
	                            //    servList->pServItems[i].nOneSegFlag = 0;
	                            //}
	                            if(servList->pServItems[i].tsId != g_dvbt2Ctx->pPatInfoForSearch->tsId)
	                            {
	                                WARNING("fatal error! sdt, pat is not same! tsId[0x%x][0x%x]\n", servList->pServItems[i].tsId, g_dvbt2Ctx->pPatInfoForSearch->tsId);
	                            }
	                            break;
	                        }
	                    }
	                }
	            }
	            else    //û��sdt��,����sdt���������? ����patΪ׼
	            {
				//if(servList->servNum != g_dvbt2Ctx->pPatInfoForSearch->nServNum)
					if(nServiceNum != g_dvbt2Ctx->pPatInfoForSearch->nServNum)								
					{
							servList->servNum = 0;
					}
					for(j=0;j<g_dvbt2Ctx->pPatInfoForSearch->nServNum;j++)
	                {
	                    servList->pServItems[servList->servNum].servId = g_dvbt2Ctx->pPatInfoForSearch->servInfoArray[j].nServId;
	                    servList->pServItems[servList->servNum].servType = MAPLE_SERV_TYPE_UNKNOWN;
	                    safeStringCopy(servList->pServItems[servList->servNum].servName, MAPLE_MAX_NAME_LEN, DEFAULT_SERVICE_NAME, strlen(DEFAULT_SERVICE_NAME));
	                    servList->pServItems[servList->servNum].isCAFree = 1;
	                    servList->pServItems[servList->servNum].tsId = g_dvbt2Ctx->pPatInfoForSearch->tsId;
	                    servList->pServItems[servList->servNum].pmtId = g_dvbt2Ctx->pPatInfoForSearch->servInfoArray[j].pmtpid;
						// if(servList->pServItems[servList->servNum].pmtId >= PMT_PID_FOR_ONESEG && servList->pServItems[servList->servNum].pmtId <= PMT_PID_FOR_ONESEG + 7)
	                    //{
	                     //   servList->pServItems[servList->servNum].nOneSegFlag = 1;
	                  //  }
	                    //else
	                   // {
	                     //   servList->pServItems[servList->servNum].nOneSegFlag = 0;
	                    //}
						servList->servNum++;
	                    if(servList->servNum >= servList->nServItemsCount)
	                    {
	                        WARNING("service is too much! [%d][%d][%d]\n", servList->servNum, servList->nServItemsCount, g_dvbt2Ctx->pPatInfoForSearch->nServNum);
	                        break;
	                    }
	                }
	            }
	        }
	        if(g_dvbt2Ctx->pPatInfoForSearch)
	        {
	            deletePATInfo(g_dvbt2Ctx->pPatInfoForSearch);
	            g_dvbt2Ctx->pPatInfoForSearch = NULL;
	        }
	    
		      //3. ��ʼpmt�������
		//* start search all pmt.
		  if(g_dvbt2Ctx->pPmtInfoForSearch)
	    {
	        WARNING("fatal error! why g_dvbCtx->pPmtInfoForSearch[0x%x]!=NULL?\n", g_dvbt2Ctx->pPmtInfoForSearch);
	        deletePMTInfo(g_dvbt2Ctx->pPmtInfoForSearch);
	        g_dvbt2Ctx->pPmtInfoForSearch = NULL;
	    }

		 if(g_dvbt2Ctx->endPatSection) //�ѵ���pat��,��ʾservice�б��Ѿ�ȷ����service��������pmt_pid.��ô����service�б����Ŀ����pmt,pmt_pidΪ0��ʾû���ҵ�pmt_pid.��ô����������.
	    {
	        if(0 == servList->servNum)
	        {
	            WARNING("fatal error! sdt pat said no service??check!!\n");
	        }
		
				servIndex = 0;
				WARNING("3 servList->servNum =%d\n",servList->servNum);
				while(servIndex < servList->servNum)
				{   
			        nWaitSectionTime = 0;
					 		g_dvbt2Ctx->curPmtSecNum = 0;
	            g_dvbt2Ctx->isBufFull[PMT_BUF_IDX] = 0;
			        g_dvbt2Ctx->endPmtSection = 0;
			        g_dvbt2Ctx->pPmtInfoForSearch = newPMTInfo();
					   if(NULL == g_dvbt2Ctx->pPmtInfoForSearch)
		            {
		                WARNING("malloc PmtInfo fail,skip pmt search!\n");
		                goto exit_dvbt2_search_service_task;
		            }
						 WARNING("PMT PID:%x,%x\n",servList->pServItems[0].pmtId,servList->pServItems[1].pmtId);
		            if(0 == servList->pServItems[servIndex].pmtId)
		            {
		                WARNING("why pat not contain this serviceid[0x%x]?\n", servList->pServItems[servIndex].servId);
										servIndex++;
						   			continue;
		            }
							dvbt2_open_section_filter(servList->pServItems[servIndex].pmtId,servList->pServItems[servIndex].servId,&nPmtVChanId);
			        while(g_dvbt2Ctx->endPmtSection == 0)
			        {
			          if(dvbt2_check_task_delete_request(EXEC_prioself) == OS_TASK_DEL_REQ)
					    	{  
					        dvbt2_close_section_filter(servList->pServItems[servIndex].pmtId,nPmtVChanId);
					        servList->servNum = 0;
					        goto exit_dvbt2_search_service_task;
					    	}
				        esKRNL_TimeDly(10);
								nWaitSectionTime++;
								if(nWaitSectionTime*100 > WAIT_PMT_TIMEOUT)
		            {
		                WARNING("pmtpid[0x%x] overtime!\n", servList->pServItems[servIndex].pmtId);
		                break;
		            }
				        
			        }
			        dvbt2_close_section_filter(servList->pServItems[servIndex].pmtId,nPmtVChanId);
						 //����pmt������һ�ž���дһ��.����ط�Ҫ���.
		            if(g_dvbt2Ctx->endPmtSection)    //pmt�ѵ��˾���д
		            {
		            		PmtSupplementServiceInfo(servList, g_dvbt2Ctx->pPmtInfoForSearch, servList->pServItems[servIndex].pmtId);
		            }
		            if(g_dvbt2Ctx->pPmtInfoForSearch)
		            {
		                deletePMTInfo(g_dvbt2Ctx->pPmtInfoForSearch);
		                g_dvbt2Ctx->pPmtInfoForSearch = NULL;			

		            }
						 servIndex ++;
						//servList->curServIdx++;		
				}
		 	} 
	    if(servList->servNum == 0)
	    {
	        WARNING("this freq[%d][%d] has no service?\n", demodParam.frequency, demodParam.bandwidth);
	        goto exit_dvbt2_search_service_task;
	    }
	    //������һ��curServItem->demod_pm.
	    for(i=0;i<servList->servNum;i++)
	    {
	        servList->pServItems[i].demod_pm.frequency = demodParam.frequency;
	        servList->pServItems[i].demod_pm.bandwidth = demodParam.bandwidth;
			servList->pServItems[i].demod_pm.t_t2_flag = demodParam.t_t2_flag;
			WARNING("dvbt2_scan_task: servList->pServItems[i].demod_pm.t_t2_flag = %d\n", demodParam.t_t2_flag);
	    }
	    
		///4. nit
	    // search nit.��ǰ��NIT��Ŀ����Ϊ��TS information descriptor�е�remote control id. 
	    //��Ҫ��дremote_control_id. 
	    //��service��Ϣ��pmt_pid,Ϊ0��ʾδ����. һ����û���ѵ�pat���pmt����ɵ�.
	    if(g_dvbt2Ctx->pNitInfoForSearch)
	    {
	        WARNING("fatal error! why g_dvbCtx->pNitInfoForSearch[0x%x]!=NULL?\n", g_dvbt2Ctx->pNitInfoForSearch);
	        deleteNITInfo(g_dvbt2Ctx->pNitInfoForSearch);
	        g_dvbt2Ctx->pNitInfoForSearch = NULL;
	    }
	    g_dvbt2Ctx->curNitSecNum   = 0;
	    g_dvbt2Ctx->endNitSection  = 0;
	    g_dvbt2Ctx->isBufFull[NIT_BUF_IDX] = 0;

		 //���ȿ���tsId��û��
	    if(g_dvbt2Ctx->endPatSection || g_dvbt2Ctx->endSdtSection)
	    {
	        g_dvbt2Ctx->pNitInfoForSearch = newNITInfo();
	        if(NULL == g_dvbt2Ctx->pNitInfoForSearch)
	        {
	            WARNING("malloc NitInfo fail,quit!\n");
	            servList->servNum = 0;
	            goto exit_dvbt2_search_service_task;
	        }
	        dvbt2_open_section_filter(NIT_PID, 0, &nNitVChanId);
	        nWaitSectionTime = 0;
	        while(g_dvbt2Ctx->endNitSection == 0)
	        {
	        	if(dvbt2_check_task_delete_request(EXEC_prioself)==OS_TASK_DEL_REQ)        // time is overflow
	        	{   
	        		dvbt2_close_section_filter(NIT_PID, nNitVChanId);
	                servList->servNum = 0;
	                goto exit_dvbt2_search_service_task;
	        	}
	            esKRNL_TimeDly(1);
	            nWaitSectionTime++;
	            if(nWaitSectionTime * 10 > WAIT_NIT_TIMEOUT)
	            {
	                WARNING("wait nit timeout [%d]ms! no nit!\n", WAIT_NIT_TIMEOUT);
	                break;
	            }
	        }
	        dvbt2_close_section_filter(NIT_PID, nNitVChanId);
			  //4.1��ʼ������дservice�б�,��ȷ��remotecontrolkeyid
	        if(g_dvbt2Ctx->endNitSection)
	        {
	        
	            tsId = servList->pServItems[0].tsId;
	            //����tsIdȥ��nit�ж�Ӧ��ts��Ϣ
	            nTmp = g_dvbt2Ctx->pNitInfoForSearch->pList_NITTSInfo->GetLength(g_dvbt2Ctx->pNitInfoForSearch->pList_NITTSInfo);
	            if(nTmp <= 0)
	            {
	                WARNING("nit has not ts info!\n");
	                goto dvb_nit_scan_exit;
	            }
	            nIdx = g_dvbt2Ctx->pNitInfoForSearch->pList_NITTSInfo->FindElem(g_dvbt2Ctx->pNitInfoForSearch->pList_NITTSInfo, DListDataCompareFunc_FindNITTSInfoByTsId, (void*)tsId);
	            if(nIdx < 0 || nIdx >= nTmp)
	            {
	                WARNING("Impossible! not find tsinfo by tsId[0x%x], nIdx[%d], nSize[%d], check code!\n", tsId,nIdx, nTmp);
	                goto dvb_nit_scan_exit;
	            }
	            if(EPDK_OK != g_dvbt2Ctx->pNitInfoForSearch->pList_NITTSInfo->GetElemByIndex(g_dvbt2Ctx->pNitInfoForSearch->pList_NITTSInfo, nIdx, (void**)&pNITTSInfo))
	            {
	                WARNING("Impossible! Can't find elem [%d], check code!\n", nIdx);
	                goto dvb_nit_scan_exit;
	            }
	           
	            //������д,���pat��pmt����û���ѵ��Ļ�,����Ҫ��.
	           for(i=0;i<servList->servNum;i++)
	            {
	                if(0 == servList->pServItems[i].pmtId)
	                {
	                    WARNING("servId[0x%x] pmtpid is not sure!\n", servList->pServItems[i].servId);
	                    for(j=0;j<pNITTSInfo->nServNum;j++)
	                    {
	                        if(servList->pServItems[i].servId == pNITTSInfo->ServIdArray[j])
	                        {
	                          // servList->pServItems[i].nOneSegFlag = 1;
	                            break;
	                        }
	                    }
	                }
	            }	
	        }
	        if(g_dvbt2Ctx->pNitInfoForSearch)
	        {
	            deleteNITInfo(g_dvbt2Ctx->pNitInfoForSearch);
	            g_dvbt2Ctx->pNitInfoForSearch = NULL;
	        }
	    }
	    else
	    {
	        WARNING("no pat,sdt, we don't know tsId, so search nit is not need!\n");
	    }
	    dvb_nit_scan_exit:
			
		//���ж��Ƿ�NIT������remote_control_key_id��������ͳһ��ֵNIT���е�remote_control_key_id�� ��Ҫ���TV����	
	    //��Щû��remote_control_key_id������Radio����������remote_control_key_id������Ϊ���⸳ֵ����350 =0x15E ��ʼ��
	    //���NIT���û��remote_control_key_id����ȫ���Զ���ֵ����350 =0x15E ��ʼ��
	  
		for(i=0;i<servList->servNum;i++)
		 {
		 	//Note: pNITTSInfoҪΪ�ǿ�ʱ���ܱȽ�,���������
	     	 	if ((pNITTSInfo != NULL) && (servList->pServItems[i].servId==pNITTSInfo->ServIdArray[i]))
	        		servList->pServItems[i].nRemoteControlKeyId =  pNITTSInfo->nRemoteControlKeyId[i];
				else
					{
					 	servList->pServItems[i].nRemoteControlKeyId  = START_REMOTE_CONTROL_KEY_ID+k;
						k++;	 	
				   }
	    } 
		
		
	    //5.֪ͨapp, �����ʾ.
	    for(i=0;i<servList->servNum;i++)
	    {
	        dvbt2_search_one_service_cb(&servList->pServItems[i]);
	    }
		  
	}
	else//dvbt2
	{
	__dvbt2_t2_set_plp:
		WARNING("demodParam.plpindex = %d, demodParam.plpnumber = %d\n", demodParam.plpindex, demodParam.plpnumber);
		if(demodParam.plpindex >= demodParam.plpnumber)
			goto exit_dvbt2_search_service_task;
		
		if(dvbt2_set_tuner_plpindex(&demodParam) != 0)
		{
			demodParam.plpindex++;
			WARNING("dvbt2_set_tuner_plp --fail->\n");
			goto __dvbt2_t2_set_plp;
		}
		//tmp_index = demodParam.plpindex;
		WARNING("dvbt2_set_tuner_plpindex --sucess----index = %d---->\n", demodParam.plpindex);
		dvbt2_get_plppid(&demodParam);
		WARNING("dvbt2_get_plppid = %d\n", demodParam.plppid);
		//* search sdt.
		
	    g_dvbt2Ctx->curSdtSecNum  = 0;
		g_dvbt2Ctx->endSdtSection = 0;
	    g_dvbt2Ctx->isBufFull[SDT_BUF_IDX] = 0;
		if(g_dvbt2Ctx->pSdtInfoForSearch)
	    {
	        WARNING("fatal error! why g_dvbt2Ctx->pSdtInfoForSearch[0x%x]!=NULL?\n", g_dvbt2Ctx->pSdtInfoForSearch);
	        deleteSDTInfo(g_dvbt2Ctx->pSdtInfoForSearch);
	        g_dvbt2Ctx->pSdtInfoForSearch = NULL;
	    }
	    g_dvbt2Ctx->pSdtInfoForSearch = newSDTInfo();
	    if(NULL == g_dvbt2Ctx->pSdtInfoForSearch)
	    {
	        WARNING("malloc PatInfo fail,skip nit search!\n");
	        goto exit_dvbt2_search_service_task;
	    }
			
		dvbt2_open_section_filter(SDT_BAT_PID,0,&nSdtVChanId);
	    nWaitSectionTime = 0;
		while(g_dvbt2Ctx->endSdtSection == 0)
		{
			if(dvbt2_check_task_delete_request(EXEC_prioself)==OS_TASK_DEL_REQ)        // time is overflow
			{
	            //dvbt2_set_service_type((void*)0);
				  dvbt2_close_section_filter(SDT_BAT_PID,nSdtVChanId);
					servList->servNum = 0;  //��Ϊ��;�˳�,���Ը�Ƶ���service������¼.
					//servList->servNum = old_servNum;//dvbt2 modify by cky	
					goto exit_dvbt2_search_service_task;
	        }
			esKRNL_TimeDly(10);
	        nWaitSectionTime++;
	        if(nWaitSectionTime * 100 > WAIT_SDT_TIMEOUT)
	        {
	            WARNING("wait sdt timeout! no sdt!\n");
	            break;
	        }
		}
		dvbt2_close_section_filter(SDT_BAT_PID,nSdtVChanId);
		  //1.1 ����sdt��������д�������ͽ�����service�б�,���û��sdt�������.
	    //��Ҫ��дserviceid,servicetype, servicename, isCAFree, tsId, orgNetworkId(��Ӧmaple_serv_item_t->networkId)
	    if(g_dvbt2Ctx->endSdtSection)    //���sdtû��ȫ������,����Ϊsdt��Ч.
	    {
	        nServiceNum = g_dvbt2Ctx->pSdtInfoForSearch->pList_SDTServInfo->GetLength(g_dvbt2Ctx->pSdtInfoForSearch->pList_SDTServInfo);
	        if(nServiceNum > servList->nServItemsCount)
	        {
	            WARNING("service num[%d] > nServItemsCount[%d]\n", nServiceNum, servList->nServItemsCount);
	        }
	        //��һ��foreach()����,�����д����.˳�����.ĿǰֻҪtv, radio. teletext��ʱ��Ҫ
	        tmpret = g_dvbt2Ctx->pSdtInfoForSearch->pList_SDTServInfo->ForeachElem(g_dvbt2Ctx->pSdtInfoForSearch->pList_SDTServInfo, DListDataVisitFunc_SDTAddService, servList);
			if(tmpret != DLIST_RET_OK)
	        {
	            WARNING("fatal error! foreach ret[%d], why != OK?\n", tmpret);
	        }
	        //��tsId, orgNetworkId��ֵ
	       // for(i=0;i<servList->servNum;i++)	//dvbt2 modify by cky			
			for(i = old_servNum;i<servList->servNum;i++)
	        {          
	            servList->pServItems[i].tsId = g_dvbt2Ctx->pSdtInfoForSearch->tsId;
	            servList->pServItems[i].networkId = g_dvbt2Ctx->pSdtInfoForSearch->orgNetworkId;
                WARNING("servList->pServItems[%d].tsId is %d \n",i,servList->pServItems[i].tsId);
                WARNING("g_dvbt2Ctx->pSdtInfoForSearch->tsId is %d \n",g_dvbt2Ctx->pSdtInfoForSearch->tsId);
	        }
	    }
	    if(g_dvbt2Ctx->pSdtInfoForSearch)
	    {
	        deleteSDTInfo(g_dvbt2Ctx->pSdtInfoForSearch);
	        g_dvbt2Ctx->pSdtInfoForSearch = NULL;
	    }
	    
		
		//* search pat.
		g_dvbt2Ctx->endPatSection = 0;
	    g_dvbt2Ctx->curPatSecNum  = 0;
	    g_dvbt2Ctx->isBufFull[PAT_BUF_IDX] = 0;
		if(g_dvbt2Ctx->pPatInfoForSearch)
	    {
	        WARNING("fatal error! why g_dvbt2Ctx->pPatInfoForSearch[0x%x]!=NULL?\n", g_dvbt2Ctx->pPatInfoForSearch);
	        deletePATInfo(g_dvbt2Ctx->pPatInfoForSearch);
	        g_dvbt2Ctx->pPatInfoForSearch = NULL;
	    }
	    g_dvbt2Ctx->pPatInfoForSearch = newPATInfo();
	    if(NULL == g_dvbt2Ctx->pPatInfoForSearch)
	    {
	        WARNING("malloc PatInfo fail,skip nit search!\n");
			  servList->servNum = 0;//dvbt2 cky modify
			  //servList->servNum = old_servNum;
	        goto exit_dvbt2_search_service_task;
	    }
		dvbt2_open_section_filter(PAT_PID,0,&nPatVChanId);
        WARNING("dvbt2_open_section_filter \n");
	    nWaitSectionTime = 0;
		while(g_dvbt2Ctx->endPatSection == 0)
		{   
	        if(dvbt2_check_task_delete_request(EXEC_prioself) == OS_TASK_DEL_REQ)        // time is overflow
			{
			    dvbt2_close_section_filter(PAT_PID,nPatVChanId);
		        servList->servNum = 0;//dvbt2 cky modify
				//servList->servNum = old_servNum;
				goto exit_dvbt2_search_service_task;
			}
	          esKRNL_TimeDly(10);
	          nWaitSectionTime++;
	          if(nWaitSectionTime*10 > WAIT_PAT_TIMEOUT)    //��ʱ
	          {
	              WARNING("search pat timeout!\n");
	              break;
	          }
		}
		dvbt2_close_section_filter(PAT_PID,nPatVChanId);
		  if(g_dvbt2Ctx->endPatSection)    //pat����Ѱ��ɾ���д.
	        {
	            //2.1 ��ʼ��дmaple��service�б�, ��Ҫ��дpmt_pid, oneseg/fullseg��Ϣ,tsId, ���sdt��ɹ�����,����sdt��Ϊ׼.
	            //������pat��Ϊ׼.�����sdt��,tsId���sdt��Ƚ�,����sdt��Ϊ׼.
	             
	            if((servList->servNum > 0) &&( nServiceNum == g_dvbt2Ctx->pPatInfoForSearch->nServNum))   //˵��sdt���Ѿ���д��servList.pat����У������  
				// if((servList->servNum > old_servNum) &&( nServiceNum == g_dvbt2Ctx->pPatInfoForSearch->nServNum))   //˵��sdt���Ѿ���д��servList.pat����У������  
				{
	               // for(i=0;i<servList->servNum;i++)//dvbt2 cky modify
					 for(i=old_servNum;i<servList->servNum;i++)
	                {
	                    //Ѱ��servId
	                    for(j=1;j<=g_dvbt2Ctx->pPatInfoForSearch->nServNum;j++)//PAT��servNum ��1��ʼ
	                    {
	                        if(servList->pServItems[i].servId == g_dvbt2Ctx->pPatInfoForSearch->servInfoArray[j].nServId)
	                        {
	                            servList->pServItems[i].pmtId = g_dvbt2Ctx->pPatInfoForSearch->servInfoArray[j].pmtpid;
	                        	// if(servList->pServItems[i].pmtId >= PMT_PID_FOR_ONESEG && servList->pServItems[i].pmtId <= PMT_PID_FOR_ONESEG + 7)
	                            //{
	                            //    servList->pServItems[i].nOneSegFlag = 1;
	                          //  }
	                           // else
	                           // {
	                            //    servList->pServItems[i].nOneSegFlag = 0;
	                            //}
	                            if(servList->pServItems[i].tsId != g_dvbt2Ctx->pPatInfoForSearch->tsId)
	                            {
	                                WARNING("fatal error! sdt, pat is not same! tsId[0x%x][0x%x]\n", servList->pServItems[i].tsId, g_dvbt2Ctx->pPatInfoForSearch->tsId);
	                            }
	                            break;
	                        }
	                    }
	                }
	            }
	            else    //û��sdt��,����sdt���������? ����patΪ׼
	            {
	            	WARNING("PAT :servNum =%d\n",g_dvbt2Ctx->pPatInfoForSearch->nServNum);
					//if(servList->servNum != g_dvbCtx->pPatInfoForSearch->nServNum)
					if(nServiceNum != g_dvbt2Ctx->pPatInfoForSearch->nServNum)
					{
							WARNING("servList->servNum = 0----\n");
							 servList->servNum = 0;//dvbt2 cky modify
							//servList->servNum = old_servNum;
					}
	               // for(j=0;j<g_dvbt2Ctx->pPatInfoForSearch->nServNum;j++)//dvbt2 cky modify
					for(j=old_servNum;j<g_dvbt2Ctx->pPatInfoForSearch->nServNum;j++)
	                {
	                    servList->pServItems[servList->servNum].servId = g_dvbt2Ctx->pPatInfoForSearch->servInfoArray[j].nServId;
	                    servList->pServItems[servList->servNum].servType = MAPLE_SERV_TYPE_UNKNOWN;
	                    safeStringCopy(servList->pServItems[servList->servNum].servName, MAPLE_MAX_NAME_LEN, DEFAULT_SERVICE_NAME, strlen(DEFAULT_SERVICE_NAME));
	                    servList->pServItems[servList->servNum].isCAFree = 1;
	                    servList->pServItems[servList->servNum].tsId = g_dvbt2Ctx->pPatInfoForSearch->tsId;
	                    servList->pServItems[servList->servNum].pmtId = g_dvbt2Ctx->pPatInfoForSearch->servInfoArray[j].pmtpid;
						// if(servList->pServItems[servList->servNum].pmtId >= PMT_PID_FOR_ONESEG && servList->pServItems[servList->servNum].pmtId <= PMT_PID_FOR_ONESEG + 7)
	                    //{
	                     //   servList->pServItems[servList->servNum].nOneSegFlag = 1;
	                  //  }
	                    //else
	                   // {
	                     //   servList->pServItems[servList->servNum].nOneSegFlag = 0;
	                    //}
						servList->servNum++;
	                    if(servList->servNum >= servList->nServItemsCount)
	                    {
	                        WARNING("service is too much! [%d][%d][%d]\n", servList->servNum, servList->nServItemsCount, g_dvbt2Ctx->pPatInfoForSearch->nServNum);
	                        break;
	                    }
	                }
	            }
	        }
	        if(g_dvbt2Ctx->pPatInfoForSearch)
	        {
	            deletePATInfo(g_dvbt2Ctx->pPatInfoForSearch);
	            g_dvbt2Ctx->pPatInfoForSearch = NULL;
	        }
	    
		      //3. ��ʼpmt�������
		//* start search all pmt.
		  if(g_dvbt2Ctx->pPmtInfoForSearch)
	    {
	        WARNING("fatal error! why g_dvbt2Ctx->pPmtInfoForSearch[0x%x]!=NULL?\n", g_dvbt2Ctx->pPmtInfoForSearch);
	        deletePMTInfo(g_dvbt2Ctx->pPmtInfoForSearch);
	        g_dvbt2Ctx->pPmtInfoForSearch = NULL;
	    }

		 if(g_dvbt2Ctx->endPatSection) //�ѵ���pat��,��ʾservice�б��Ѿ�ȷ����service��������pmt_pid.��ô����service�б����Ŀ����pmt,pmt_pidΪ0��ʾû���ҵ�pmt_pid.��ô����������.
	    {
	        if(0 == servList->servNum)
	        {
	            WARNING("fatal error! sdt pat said no service??check!!\n");
	        }
		
				servIndex = 0;//dvbt2 cky modify
				//servIndex = old_servNum;
				WARNING("3 servList->servNum =%d\n",servList->servNum);
				while(servIndex < servList->servNum)//dvbt2 cky modify
				//while(servIndex < (servList->servNum+old_servNum))
				{   
			        nWaitSectionTime = 0;
					g_dvbt2Ctx->curPmtSecNum = 0;
	                g_dvbt2Ctx->isBufFull[PMT_BUF_IDX] = 0;
			        g_dvbt2Ctx->endPmtSection = 0;
			        g_dvbt2Ctx->pPmtInfoForSearch = newPMTInfo();
					if(NULL == g_dvbt2Ctx->pPmtInfoForSearch)
		            {
		                WARNING("malloc PmtInfo fail,skip pmt search!\n");
		                goto exit_dvbt2_search_service_task;
		            }
						 WARNING("PMT PID:%x,%x\n",servList->pServItems[0].pmtId,servList->pServItems[1].pmtId);
		            if(0 == servList->pServItems[servIndex].pmtId)
		            {
		                WARNING("why pat not contain this serviceid[0x%x]?\n", servList->pServItems[servIndex].servId);
						servIndex++;
						continue;
		            }
					dvbt2_open_section_filter(servList->pServItems[servIndex].pmtId,servList->pServItems[servIndex].servId,&nPmtVChanId);
			        while(g_dvbt2Ctx->endPmtSection == 0)
			        {
		          		if(dvbt2_check_task_delete_request(EXEC_prioself) == OS_TASK_DEL_REQ)
				    	{  
				        	dvbt2_close_section_filter(servList->pServItems[servIndex].pmtId,nPmtVChanId);
			
							WARNING("go to __dvb_t2_set_plp--->\n");
							demodParam.plpindex++;
							//old_servNum = servList->servNum;//dvbt2 cky modify  //???
							old_servNum = servList->servNum + old_servNum;
					        goto __dvbt2_t2_set_plp;					
				    	}
			        	esKRNL_TimeDly(10);
						nWaitSectionTime++;
						if(nWaitSectionTime*100 > WAIT_PMT_TIMEOUT)
			            {
			                WARNING("pmtpid[0x%x] overtime!\n", servList->pServItems[servIndex].pmtId);
			                break;
			            }
				        
			        }
			        dvbt2_close_section_filter(servList->pServItems[servIndex].pmtId,nPmtVChanId);
						 //����pmt������һ�ž���дһ��.����ط�Ҫ���.
		            if(g_dvbt2Ctx->endPmtSection)    //pmt�ѵ��˾���д
		            {
		            	PmtSupplementServiceInfo(servList, g_dvbt2Ctx->pPmtInfoForSearch, servList->pServItems[servIndex].pmtId);
		            }
		            if(g_dvbt2Ctx->pPmtInfoForSearch)
		            {
		                deletePMTInfo(g_dvbt2Ctx->pPmtInfoForSearch);
		                g_dvbt2Ctx->pPmtInfoForSearch = NULL;			

		            }
						 servIndex ++;
						//servList->curServIdx++;		
				}
		 	} 
	    if(servList->servNum == 0)
	    {
	        WARNING("this freq[%d][%d] has no service?\n", demodParam.frequency, demodParam.bandwidth);
	        goto exit_dvbt2_search_service_task;
	    }
	    //������һ��curServItem->demod_pm.
	   // for(i=0;i<servList->servNum;i++) //dvbt2 cky modify
		for(i=old_servNum;i<servList->servNum;i++)
	    {
	        servList->pServItems[i].demod_pm.frequency = demodParam.frequency;
	        servList->pServItems[i].demod_pm.bandwidth = demodParam.bandwidth;
			WARNING("test0: demodParam.plpindex = %d, demod_pm.plppid = %d-----i = %d\n", demodParam.plpindex, servList->pServItems[i].demod_pm.plppid, i);
			//if(i >= old_servNum)//dvbt2 cky modify
			{
				servList->pServItems[i].demod_pm.plpindex  = demodParam.plpindex;
				servList->pServItems[i].demod_pm.plppid	   = demodParam.plppid;
				servList->pServItems[i].demod_pm.t_t2_flag = demodParam.t_t2_flag;
				WARNING("dvbt2_scan_task: demod_p,.t_t2_flag = %d\n", demodParam.t_t2_flag);
			}

	    }
	    
		///4. nit
	    // search nit.��ǰ��NIT��Ŀ����Ϊ��TS information descriptor�е�remote control id. 
	    //��Ҫ��дremote_control_id. 
	    //��service��Ϣ��pmt_pid,Ϊ0��ʾδ����. һ����û���ѵ�pat���pmt����ɵ�.
	    if(g_dvbt2Ctx->pNitInfoForSearch)
	    {
	        WARNING("fatal error! why g_dvbt2Ctx->pNitInfoForSearch[0x%x]!=NULL?\n", g_dvbt2Ctx->pNitInfoForSearch);
	        deleteNITInfo(g_dvbt2Ctx->pNitInfoForSearch);
	        g_dvbt2Ctx->pNitInfoForSearch = NULL;
	    }
	    g_dvbt2Ctx->curNitSecNum   = 0;
	    g_dvbt2Ctx->endNitSection  = 0;
	    g_dvbt2Ctx->isBufFull[NIT_BUF_IDX] = 0;

		 //���ȿ���tsId��û��
	    if(g_dvbt2Ctx->endPatSection || g_dvbt2Ctx->endSdtSection)
	    {
	        g_dvbt2Ctx->pNitInfoForSearch = newNITInfo();
	        if(NULL == g_dvbt2Ctx->pNitInfoForSearch)
	        {
	            WARNING("malloc NitInfo fail,quit!\n");
	            servList->servNum = 0;//dvbt2 cky modify
				//servList->servNum = old_servNum;
	            goto exit_dvbt2_search_service_task;
	        }
	        dvbt2_open_section_filter(NIT_PID, 0, &nNitVChanId);
	        nWaitSectionTime = 0;
	        while(g_dvbt2Ctx->endNitSection == 0)
	        {
	        	if(dvbt2_check_task_delete_request(EXEC_prioself)==OS_TASK_DEL_REQ)        // time is overflow
	        	{   
	        		dvbt2_close_section_filter(NIT_PID, nNitVChanId);
					servList->servNum = 0;//dvbt2 cky modify
					//servList->servNum = old_servNum;
	                goto exit_dvbt2_search_service_task;
	        	}
	            esKRNL_TimeDly(1);
	            nWaitSectionTime++;
	            if(nWaitSectionTime * 10 > WAIT_NIT_TIMEOUT)
	            {
	                WARNING("wait nit timeout [%d]ms! no nit!\n", WAIT_NIT_TIMEOUT);
	                break;
	            }
	        }

            dvbt2_close_section_filter(NIT_PID, nNitVChanId);

			  //4.1��ʼ������дservice�б�,��ȷ��remotecontrolkeyid
			WARNING("g_dvbt2Ctx->endNitSection is %d \n",g_dvbt2Ctx->endNitSection);
	        if(g_dvbt2Ctx->endNitSection)
	        {
	            WARNING("g_dvbt2Ctx->endNitSection \n");
	           // tsId = servList->pServItems[0].tsId;//dvbt2 cky modify
				tsId = servList->pServItems[old_servNum].tsId;
	            //����tsIdȥ��nit�ж�Ӧ��ts��Ϣ
	            nTmp = g_dvbt2Ctx->pNitInfoForSearch->pList_NITTSInfo->GetLength(g_dvbt2Ctx->pNitInfoForSearch->pList_NITTSInfo);
	            if(nTmp <= 0)
	            {
	                WARNING("nit has not ts info!\n");
	                goto _nit_scan_exit;
	            }
	            nIdx = g_dvbt2Ctx->pNitInfoForSearch->pList_NITTSInfo->FindElem(g_dvbt2Ctx->pNitInfoForSearch->pList_NITTSInfo, DListDataCompareFunc_FindNITTSInfoByTsId, (void*)tsId);
	            if(nIdx < 0 || nIdx >= nTmp)
	            {
	                WARNING("Impossible! not find tsinfo by tsId[0x%x], nIdx[%d], nSize[%d], check code!\n", tsId,nIdx, nTmp);
	                goto _nit_scan_exit;
	            }
	            if(EPDK_OK != g_dvbt2Ctx->pNitInfoForSearch->pList_NITTSInfo->GetElemByIndex(g_dvbt2Ctx->pNitInfoForSearch->pList_NITTSInfo, nIdx, (void**)&pNITTSInfo))
	            {
	                WARNING("Impossible! Can't find elem [%d], check code!\n", nIdx);
	                goto _nit_scan_exit;
	            }

	            //������д,���pat��pmt����û���ѵ��Ļ�,����Ҫ��.
	           //for(i=0;i<servList->servNum;i++)//dvbt2 cky modify
			   	for(i=old_servNum;i<servList->servNum;i++)
	            {
	               // if(0 == servList->pServItems[i].pmtId)
	                {
	                   // WARNING("servId[0x%x] pmtpid is not sure!\n", servList->pServItems[i].servId);
                        //WARNING("the address of pNITTSInfo is %p \n",pNITTSInfo);
                        servList->pServItems[i].nRemoteControlKeyId = 0XFFFF;
                       	servList->pServItems[i].nOneSegFlag = 0;//DVBT��ʽ����������ж��Ƿ���RemoteControlKeyId

                        for(j=0;j<pNITTSInfo->nServNum;j++)
	                    {
                            WARNING("servList->pServItems[%d].servId[%x]pmtid[%x],pNITTSInfo->ServIdArray[%d][%x] \n",i,servList->pServItems[i].servId,servList->pServItems[i].pmtId,j,pNITTSInfo->ServIdArray[j]);

                            if((servList->pServItems[i].servId == pNITTSInfo->ServIdArray[j])||(servList->pServItems[i].pmtId == pNITTSInfo->ServIdArray[j]))
	                        {
	                          servList->pServItems[i].nOneSegFlag = 1;//DVBT��ʽ����������ж��Ƿ���RemoteControlKeyId
	                           servList->pServItems[i].nRemoteControlKeyId =  pNITTSInfo->nRemoteControlKeyId[j];
								WARNING("RemoteControlKeyId d:[%x]s[%x] is ok\n",servList->pServItems[i].nRemoteControlKeyId, pNITTSInfo->nRemoteControlKeyId[j]);
								break;
	                        }							
	                    }
	                }
	            }	
	        }

	        if(g_dvbt2Ctx->pNitInfoForSearch)
	        {
	            deleteNITInfo(g_dvbt2Ctx->pNitInfoForSearch);
	            g_dvbt2Ctx->pNitInfoForSearch = NULL;
	        }
	    }
	    else
	    {
	        WARNING("no pat,sdt, we don't know tsId, so search nit is not need!\n");
	    }
	_nit_scan_exit:
    //���NIT���û��remote_control_key_id����ȫ���Զ���ֵ����350 =0x15E ��ʼ��
  		// for(i=0;i<servList->servNum;i++)//dvbt2 cky modify
		for(i=old_servNum;i<servList->servNum;i++)
		{
		 	//Note: pNITTSInfoҪΪ�ǿ�ʱ���ܱȽ�,���������
		 	if((servList->pServItems[i].nOneSegFlag == 0)&&(servList->pServItems[i].nRemoteControlKeyId == 0XFFFF)) 
			{
			 	servList->pServItems[i].nRemoteControlKeyId  = START_REMOTE_CONTROL_KEY_ID+k+old_servNum;
				k++;	 	
		    }
	    } 	

	    //5.֪ͨapp, �����ʾ.
	   // for(i=0;i<servList->servNum;i++)//dvbt2 cky modify
		for(i=old_servNum;i<servList->servNum;i++)
	    {
	        dvbt2_search_one_service_cb(&servList->pServItems[i]);
	    }
	    WARNING("test1: demodParam.plpindex = %d\n", demodParam.plpindex);
		if(demodParam.plpindex < demodParam.plpnumber)
		{
		
			demodParam.plpindex++;
			//old_servNum = servList->servNum;//dvbt2 cky modify
			old_servNum = old_servNum+servList->servNum;
			WARNING("test2:demodParam.plpindex = %d\n", demodParam.plpindex);
			goto __dvbt2_t2_set_plp;
		}
		//cky add 20151027
		servList->servNum = old_servNum;
	}
    
	
exit_dvbt2_search_service_task:
	 if(g_dvbt2Ctx->pPmtInfoForSearch)
    {
        deletePMTInfo(g_dvbt2Ctx->pPmtInfoForSearch);
        g_dvbt2Ctx->pPmtInfoForSearch = NULL;
    }
	  if(g_dvbt2Ctx->pPatInfoForSearch)
    {
        deletePATInfo(g_dvbt2Ctx->pPatInfoForSearch);
        g_dvbt2Ctx->pPatInfoForSearch = NULL;
    }
	
    if(g_dvbt2Ctx->pSdtInfoForSearch)
    {
        deleteSDTInfo(g_dvbt2Ctx->pSdtInfoForSearch);
        g_dvbt2Ctx->pSdtInfoForSearch = NULL;
    }
	 if(g_dvbt2Ctx->pNitInfoForSearch)
    {
        deleteNITInfo(g_dvbt2Ctx->pNitInfoForSearch);
        g_dvbt2Ctx->pNitInfoForSearch = NULL;
    }
    dvbt2_search_service_finish_cb();
    g_dvbt2Ctx->slSearchStopFlag = 1;
    dvbt2_delete_task(EXEC_prioself);

}

#if(ckydebug == 1)
 void dvbt2_scan_by_ssu(void* arg)
{   
	
    __u32 									i = 0;
	 __u32     								j = 0;
	 __u32										k = 0;
    //__u32 							maxDelayNum;
    maple_demod_info_t  	demodParam;
	__s32               nWaitSectionTime = 0;
	   //maple_serv_item_t   *curServItem    = NULL;
    __u16               nRemoteControlKeyId[TS_MAX_SERVICE_NUM];
	 __u16               tsId = 0;
    __s32               nTmp;
    __s32               nIdx;
    NITTSInfo           *pNITTSInfo = NULL;
    __u32               servIndex  = 0;
    __s32               nServiceNum = 0;
    __s32               tmpret;
	__s32				ret = EPDK_FAIL;
    __s32               nPatVChanId = -1;
    __s32               nPmtVChanId = -1;
  	__s32               nNitVChanId = -1;
	__s32               old_servNum = 0;

		
    servList = (maple_serv_list_t *)arg;
       //��ʼ��servList.
    servList->servNum       = 0;
    servList->curServIdx    = 0;
    servList->nServItemsCount = MAPLE_MAX_SERV_NUM;
    memset(servList->pServItems, 0, sizeof(maple_serv_item_t)*MAPLE_MAX_SERV_NUM);
	
  	//* step 1, tune.
	demodParam.frequency = g_dvbt2Ctx->frequency;
	demodParam.bandwidth = g_dvbt2Ctx->bandwidth;
	demodParam.plpindex  = 0;
	demodParam.plppid		 = 0;
	demodParam.plpnumber = 0;
//WARNING("dvbt2_tune--start-->>>\n");
    if(dvbt2_tune(&demodParam) != 0)
        goto exit_dvbt2_search_service_task;
//WARNING("dvbt2_tune---end->>>\n");        
	 dvbt2_get_tuner_param(&demodParam);

	if(demodParam.t_t2_flag == 2)
	{		
		//* search pat.
		 g_dvbt2Ctx->endPatSection = 0;
	    g_dvbt2Ctx->curPatSecNum  = 0;
	    g_dvbt2Ctx->isBufFull[PAT_BUF_IDX] = 0;
		 if(g_dvbt2Ctx->pPatInfoForSearch)
	    {
        WARNING("fatal error! why g_dvbt2Ctx->pPatInfoForSearch[0x%x]!=NULL?\n", g_dvbt2Ctx->pPatInfoForSearch);
	        deletePATInfo(g_dvbt2Ctx->pPatInfoForSearch);
	        g_dvbt2Ctx->pPatInfoForSearch = NULL;
	    }
	    g_dvbt2Ctx->pPatInfoForSearch = newPATInfo();
	    if(NULL == g_dvbt2Ctx->pPatInfoForSearch)
	    {
	        WARNING("malloc PatInfo fail,skip nit search!\n");
			  servList->servNum = 0;
	        goto exit_dvbt2_search_service_task;
	    }
		dvbt2_open_section_filter(PAT_PID,0,&nPatVChanId);
	    nWaitSectionTime = 0;
		while(g_dvbt2Ctx->endPatSection == 0)
		{   
	        if(dvbt2_check_task_delete_request(EXEC_prioself) == OS_TASK_DEL_REQ)        // time is overflow
			{
			    dvbt2_close_section_filter(PAT_PID,nPatVChanId);
		        servList->servNum = 0;
				goto exit_dvbt2_search_service_task;
			}
	          esKRNL_TimeDly(10);
	          nWaitSectionTime++;
	          if(nWaitSectionTime*10 > WAIT_PAT_TIMEOUT)    //��ʱ
	          {
	              WARNING("search pat timeout!\n");
	              break;
	          }
		}
		dvbt2_close_section_filter(PAT_PID,nPatVChanId);
		  if(g_dvbt2Ctx->endPatSection)    //pat����Ѱ��ɾ���д.
	        {
	            //2.1 ��ʼ��дmaple��service�б�, ��Ҫ��дpmt_pid, oneseg/fullseg��Ϣ,tsId, ���sdt��ɹ�����,����sdt��Ϊ׼.
	            //������pat��Ϊ׼.�����sdt��,tsId���sdt��Ƚ�,����sdt��Ϊ׼.
				//if((servList->servNum > 0)&&(servList->servNum == g_dvbt2Ctx->pPatInfoForSearch->nServNum))   //˵��sdt���Ѿ���д��servList.pat����У������
	            if((servList->servNum > 0) &&( nServiceNum == g_dvbt2Ctx->pPatInfoForSearch->nServNum))   //˵��sdt���Ѿ���д��servList.pat����У������  
				{
	                for(i=0;i<servList->servNum;i++)
	                {
	                    //Ѱ��servId
	                    for(j=1;j<=g_dvbt2Ctx->pPatInfoForSearch->nServNum;j++)//PAT��servNum ��1��ʼ
	                    {
	                        if(servList->pServItems[i].servId == g_dvbt2Ctx->pPatInfoForSearch->servInfoArray[j].nServId)
	                        {
	                            servList->pServItems[i].pmtId = g_dvbt2Ctx->pPatInfoForSearch->servInfoArray[j].pmtpid;
	                        	// if(servList->pServItems[i].pmtId >= PMT_PID_FOR_ONESEG && servList->pServItems[i].pmtId <= PMT_PID_FOR_ONESEG + 7)
	                            //{
	                            //    servList->pServItems[i].nOneSegFlag = 1;
	                          //  }
	                           // else
	                           // {
	                            //    servList->pServItems[i].nOneSegFlag = 0;
	                            //}
	                            if(servList->pServItems[i].tsId != g_dvbt2Ctx->pPatInfoForSearch->tsId)
	                            {
	                                WARNING("fatal error! sdt, pat is not same! tsId[0x%x][0x%x]\n", servList->pServItems[i].tsId, g_dvbt2Ctx->pPatInfoForSearch->tsId);
	                            }
	                            break;
	                        }
	                    }
	                }
	            }
	            else    //û��sdt��,����sdt���������? ����patΪ׼
	            {
				//if(servList->servNum != g_dvbt2Ctx->pPatInfoForSearch->nServNum)
					if(nServiceNum != g_dvbt2Ctx->pPatInfoForSearch->nServNum)								
					{
							servList->servNum = 0;
					}
					for(j=0;j<g_dvbt2Ctx->pPatInfoForSearch->nServNum;j++)
	                {
	                    servList->pServItems[servList->servNum].servId = g_dvbt2Ctx->pPatInfoForSearch->servInfoArray[j].nServId;
	                    servList->pServItems[servList->servNum].servType = MAPLE_SERV_TYPE_UNKNOWN;
	                    safeStringCopy(servList->pServItems[servList->servNum].servName, MAPLE_MAX_NAME_LEN, DEFAULT_SERVICE_NAME, strlen(DEFAULT_SERVICE_NAME));
	                    servList->pServItems[servList->servNum].isCAFree = 1;
	                    servList->pServItems[servList->servNum].tsId = g_dvbt2Ctx->pPatInfoForSearch->tsId;
	                    servList->pServItems[servList->servNum].pmtId = g_dvbt2Ctx->pPatInfoForSearch->servInfoArray[j].pmtpid;
											// if(servList->pServItems[servList->servNum].pmtId >= PMT_PID_FOR_ONESEG && servList->pServItems[servList->servNum].pmtId <= PMT_PID_FOR_ONESEG + 7)
	                    //{
	                     //   servList->pServItems[servList->servNum].nOneSegFlag = 1;
	                  //  }
	                    //else
	                   // {
	                     //   servList->pServItems[servList->servNum].nOneSegFlag = 0;
	                    //}
											servList->servNum++;
	                    if(servList->servNum >= servList->nServItemsCount)
	                    {
	                        WARNING("service is too much! [%d][%d][%d]\n", servList->servNum, servList->nServItemsCount, g_dvbt2Ctx->pPatInfoForSearch->nServNum);
	                        break;
	                    }
	                }
	            }
	        }
	        if(g_dvbt2Ctx->pPatInfoForSearch)
	        {
	            deletePATInfo(g_dvbt2Ctx->pPatInfoForSearch);
	            g_dvbt2Ctx->pPatInfoForSearch = NULL;
	        }
	    
		      //3. ��ʼpmt�������
		//* start search all pmt.
		  if(g_dvbt2Ctx->pPmtInfoForSearch)
	    {
	        WARNING("fatal error! why g_dvbCtx->pPmtInfoForSearch[0x%x]!=NULL?\n", g_dvbt2Ctx->pPmtInfoForSearch);
	        deletePMTInfo(g_dvbt2Ctx->pPmtInfoForSearch);
	        g_dvbt2Ctx->pPmtInfoForSearch = NULL;
	    }

		 if(g_dvbt2Ctx->endPatSection) //�ѵ���pat��,��ʾservice�б��Ѿ�ȷ����service��������pmt_pid.��ô����service�б����Ŀ����pmt,pmt_pidΪ0��ʾû���ҵ�pmt_pid.��ô����������.
	    {
	        if(0 == servList->servNum)
	        {
	            WARNING("fatal error! sdt pat said no service??check!!\n");
	        }
		
				servIndex = 0;
				WARNING("3 servList->servNum =%d\n",servList->servNum);
				while(servIndex < servList->servNum)
				{   
			        nWaitSectionTime = 0;
					 		g_dvbt2Ctx->curPmtSecNum = 0;
	            g_dvbt2Ctx->isBufFull[PMT_BUF_IDX] = 0;
			        g_dvbt2Ctx->endPmtSection = 0;
			        g_dvbt2Ctx->pPmtInfoForSearch = newPMTInfo();
					   if(NULL == g_dvbt2Ctx->pPmtInfoForSearch)
		            {
		                WARNING("malloc PmtInfo fail,skip pmt search!\n");
		                goto exit_dvbt2_search_service_task;
		            }
						 WARNING("PMT PID:%x,%x\n",servList->pServItems[0].pmtId,servList->pServItems[1].pmtId);
		            if(0 == servList->pServItems[servIndex].pmtId)
		            {
		                WARNING("why pat not contain this serviceid[0x%x]?\n", servList->pServItems[servIndex].servId);
										servIndex++;
						   			continue;
		            }
							dvbt2_open_section_filter(servList->pServItems[servIndex].pmtId,servList->pServItems[servIndex].servId,&nPmtVChanId);
			        while(g_dvbt2Ctx->endPmtSection == 0)
			        {
			          if(dvbt2_check_task_delete_request(EXEC_prioself) == OS_TASK_DEL_REQ)
					    	{  
					        dvbt2_close_section_filter(servList->pServItems[servIndex].pmtId,nPmtVChanId);
					        servList->servNum = 0;
					        goto exit_dvbt2_search_service_task;
					    	}
				        esKRNL_TimeDly(10);
								nWaitSectionTime++;
								if(nWaitSectionTime*100 > WAIT_PMT_TIMEOUT)
		            {
		                WARNING("pmtpid[0x%x] overtime!\n", servList->pServItems[servIndex].pmtId);
		                break;
		            }
				        
			        }
			        dvbt2_close_section_filter(servList->pServItems[servIndex].pmtId,nPmtVChanId);
						 //����pmt������һ�ž���дһ��.����ط�Ҫ���.
		            if(g_dvbt2Ctx->endPmtSection)    //pmt�ѵ��˾���д
		            {
		            		PmtSupplementServiceInfo(servList, g_dvbt2Ctx->pPmtInfoForSearch, servList->pServItems[servIndex].pmtId);
		            }
		            if(g_dvbt2Ctx->pPmtInfoForSearch)
		            {
		                deletePMTInfo(g_dvbt2Ctx->pPmtInfoForSearch);
		                g_dvbt2Ctx->pPmtInfoForSearch = NULL;			

		            }
						 servIndex ++;
						//servList->curServIdx++;		
				}
		 	} 
	    if(servList->servNum == 0)
	    {
	        WARNING("this freq[%d][%d] has no service?\n", demodParam.frequency, demodParam.bandwidth);
	        goto exit_dvbt2_search_service_task;
	    }
	    //������һ��curServItem->demod_pm.
	    for(i=0;i<servList->servNum;i++)
	    {
	        servList->pServItems[i].demod_pm.frequency = demodParam.frequency;
	        servList->pServItems[i].demod_pm.bandwidth = demodParam.bandwidth;
			servList->pServItems[i].demod_pm.t_t2_flag = demodParam.t_t2_flag;
			WARNING("dvbt2_scan_task: servList->pServItems[i].demod_pm.t_t2_flag = %d\n", demodParam.t_t2_flag);
	    }
	    
		///4. nit
	    // search nit.��ǰ��NIT��Ŀ����Ϊ��TS information descriptor�е�remote control id. 
	    //��Ҫ��дremote_control_id. 
	    //��service��Ϣ��pmt_pid,Ϊ0��ʾδ����. һ����û���ѵ�pat���pmt����ɵ�.
	    if(g_dvbt2Ctx->pNitInfoForSearch)
	    {
	        WARNING("fatal error! why g_dvbCtx->pNitInfoForSearch[0x%x]!=NULL?\n", g_dvbt2Ctx->pNitInfoForSearch);
	        deleteNITInfo(g_dvbt2Ctx->pNitInfoForSearch);
	        g_dvbt2Ctx->pNitInfoForSearch = NULL;
	    }
	    g_dvbt2Ctx->curNitSecNum   = 0;
	    g_dvbt2Ctx->endNitSection  = 0;
	    g_dvbt2Ctx->isBufFull[NIT_BUF_IDX] = 0;

		 //���ȿ���tsId��û��
	    if(g_dvbt2Ctx->endPatSection || g_dvbt2Ctx->endSdtSection)
	    {
	        g_dvbt2Ctx->pNitInfoForSearch = newNITInfo();
	        if(NULL == g_dvbt2Ctx->pNitInfoForSearch)
	        {
	            WARNING("malloc NitInfo fail,quit!\n");
	            servList->servNum = 0;
	            goto exit_dvbt2_search_service_task;
	        }
	        dvbt2_open_section_filter(NIT_PID, 0, &nNitVChanId);
	        nWaitSectionTime = 0;
	        while(g_dvbt2Ctx->endNitSection == 0)
	        {
	        	if(dvbt2_check_task_delete_request(EXEC_prioself)==OS_TASK_DEL_REQ)        // time is overflow
	        	{   
	        		dvbt2_close_section_filter(NIT_PID, nNitVChanId);
	                servList->servNum = 0;
	                goto exit_dvbt2_search_service_task;
	        	}
	            esKRNL_TimeDly(1);
	            nWaitSectionTime++;
	            if(nWaitSectionTime * 10 > WAIT_NIT_TIMEOUT)
	            {
	                WARNING("wait nit timeout [%d]ms! no nit!\n", WAIT_NIT_TIMEOUT);
	                break;
	            }
	        }
	        dvbt2_close_section_filter(NIT_PID, nNitVChanId);
			  //4.1��ʼ������дservice�б�,��ȷ��remotecontrolkeyid
	        if(g_dvbt2Ctx->endNitSection)
	        {
	        
	            tsId = servList->pServItems[0].tsId;
	            //����tsIdȥ��nit�ж�Ӧ��ts��Ϣ
	            nTmp = g_dvbt2Ctx->pNitInfoForSearch->pList_NITTSInfo->GetLength(g_dvbt2Ctx->pNitInfoForSearch->pList_NITTSInfo);
	            if(nTmp <= 0)
	            {
	                WARNING("nit has not ts info!\n");
	                goto dvb_nit_scan_exit;
	            }
	            nIdx = g_dvbt2Ctx->pNitInfoForSearch->pList_NITTSInfo->FindElem(g_dvbt2Ctx->pNitInfoForSearch->pList_NITTSInfo, DListDataCompareFunc_FindNITTSInfoByTsId, (void*)tsId);
	            if(nIdx < 0 || nIdx >= nTmp)
	            {
	                WARNING("Impossible! not find tsinfo by tsId[0x%x], nIdx[%d], nSize[%d], check code!\n", tsId,nIdx, nTmp);
	                goto dvb_nit_scan_exit;
	            }
	            if(EPDK_OK != g_dvbt2Ctx->pNitInfoForSearch->pList_NITTSInfo->GetElemByIndex(g_dvbt2Ctx->pNitInfoForSearch->pList_NITTSInfo, nIdx, (void**)&pNITTSInfo))
	            {
	                WARNING("Impossible! Can't find elem [%d], check code!\n", nIdx);
	                goto dvb_nit_scan_exit;
	            }
	           
	            //������д,���pat��pmt����û���ѵ��Ļ�,����Ҫ��.
	           for(i=0;i<servList->servNum;i++)
	            {
	                if(0 == servList->pServItems[i].pmtId)
	                {
	                    WARNING("servId[0x%x] pmtpid is not sure!\n", servList->pServItems[i].servId);
	                    for(j=0;j<pNITTSInfo->nServNum;j++)
	                    {
	                        if(servList->pServItems[i].servId == pNITTSInfo->ServIdArray[j])
	                        {
	                          // servList->pServItems[i].nOneSegFlag = 1;
	                            break;
	                        }
	                    }
	                }
	            }	
	        }
	        if(g_dvbt2Ctx->pNitInfoForSearch)
	        {
	            deleteNITInfo(g_dvbt2Ctx->pNitInfoForSearch);
	            g_dvbt2Ctx->pNitInfoForSearch = NULL;
	        }
	    }
	    else
	    {
	        WARNING("no pat,sdt, we don't know tsId, so search nit is not need!\n");
	    }
	    dvb_nit_scan_exit:
			
		//���ж��Ƿ�NIT������remote_control_key_id��������ͳһ��ֵNIT���е�remote_control_key_id�� ��Ҫ���TV����	
	    //��Щû��remote_control_key_id������Radio����������remote_control_key_id������Ϊ���⸳ֵ����350 =0x15E ��ʼ��
	    //���NIT���û��remote_control_key_id����ȫ���Զ���ֵ����350 =0x15E ��ʼ��
	  
		for(i=0;i<servList->servNum;i++)
		 {
		 	//Note: pNITTSInfoҪΪ�ǿ�ʱ���ܱȽ�,���������
     	 	if ((pNITTSInfo != NULL) && (servList->pServItems[i].servId==pNITTSInfo->ServIdArray[i]))
        		servList->pServItems[i].nRemoteControlKeyId =  pNITTSInfo->nRemoteControlKeyId[i];
			else
			{
				 	servList->pServItems[i].nRemoteControlKeyId  = START_REMOTE_CONTROL_KEY_ID+k;
					k++;	 	
			}
	    } 
		
		
	    //5.֪ͨapp, �����ʾ.
	    for(i=0;i<servList->servNum;i++)
	    {
	        dvbt2_search_one_service_cb(&servList->pServItems[i]);
	    }
		  
	}
	else
	{
	__dvbt2_t2_set_plp:
		WARNING("demodParam.plpindex = %d, demodParam.plpnumber = %d\n", demodParam.plpindex, demodParam.plpnumber);
		if(demodParam.plpindex >= demodParam.plpnumber)
			goto exit_dvbt2_search_service_task;
		
		if(dvbt2_set_tuner_plpindex(&demodParam) != 0)
		{
			demodParam.plpindex++;
			WARNING("dvbt2_set_tuner_plp --fail->\n");
			goto __dvbt2_t2_set_plp;
		}
		//tmp_index = demodParam.plpindex;
		WARNING("dvbt2_set_tuner_plpindex --sucess----index = %d---->\n", demodParam.plpindex);
		dvbt2_get_plppid(&demodParam);
		WARNING("dvbt2_get_plppid = %d\n", demodParam.plppid);
		//* search sdt.
		
	    g_dvbt2Ctx->curSdtSecNum  = 0;
		g_dvbt2Ctx->endSdtSection = 0;
	    g_dvbt2Ctx->isBufFull[SDT_BUF_IDX] = 0;
		if(g_dvbt2Ctx->pSdtInfoForSearch)
	    {
	        WARNING("fatal error! why g_dvbt2Ctx->pSdtInfoForSearch[0x%x]!=NULL?\n", g_dvbt2Ctx->pSdtInfoForSearch);
	        deleteSDTInfo(g_dvbt2Ctx->pSdtInfoForSearch);
	        g_dvbt2Ctx->pSdtInfoForSearch = NULL;
	    }
	    g_dvbt2Ctx->pSdtInfoForSearch = newSDTInfo();
	    if(NULL == g_dvbt2Ctx->pSdtInfoForSearch)
	    {
	        WARNING("malloc PatInfo fail,skip nit search!\n");
	        goto exit_dvbt2_search_service_task;
	    }
			
		dvbt2_open_section_filter(SDT_BAT_PID,0,&nSdtVChanId);
	    nWaitSectionTime = 0;
		while(g_dvbt2Ctx->endSdtSection == 0)
		{
			if(dvbt2_check_task_delete_request(EXEC_prioself)==OS_TASK_DEL_REQ)        // time is overflow
			{
	            //dvbt2_set_service_type((void*)0);
				  dvbt2_close_section_filter(SDT_BAT_PID,nSdtVChanId);
					 servList->servNum = 0;  //��Ϊ��;�˳�,���Ը�Ƶ���service������¼.
					goto exit_dvbt2_search_service_task;
	        }
			   esKRNL_TimeDly(10);
	        nWaitSectionTime++;
	        if(nWaitSectionTime * 100 > WAIT_SDT_TIMEOUT)
	        {
	            WARNING("wait sdt timeout! no sdt!\n");
	            break;
	        }
		}
		dvbt2_close_section_filter(SDT_BAT_PID,nSdtVChanId);
		  //1.1 ����sdt��������д�������ͽ�����service�б�,���û��sdt�������.
	    //��Ҫ��дserviceid,servicetype, servicename, isCAFree, tsId, orgNetworkId(��Ӧmaple_serv_item_t->networkId)
	    if(g_dvbt2Ctx->endSdtSection)    //���sdtû��ȫ������,����Ϊsdt��Ч.
	    {
	        nServiceNum = g_dvbt2Ctx->pSdtInfoForSearch->pList_SDTServInfo->GetLength(g_dvbt2Ctx->pSdtInfoForSearch->pList_SDTServInfo);
	        if(nServiceNum > servList->nServItemsCount)
	        {
	            WARNING("service num[%d] > nServItemsCount[%d]\n", nServiceNum, servList->nServItemsCount);
	        }
	        //��һ��foreach()����,�����д����.˳�����.ĿǰֻҪtv, radio. teletext��ʱ��Ҫ
	        tmpret = g_dvbt2Ctx->pSdtInfoForSearch->pList_SDTServInfo->ForeachElem(g_dvbt2Ctx->pSdtInfoForSearch->pList_SDTServInfo, DListDataVisitFunc_SDTAddService, servList);
			if(tmpret != DLIST_RET_OK)
	        {
	            WARNING("fatal error! foreach ret[%d], why != OK?\n", tmpret);
	        }
	        //��tsId, orgNetworkId��ֵ
	        for(i=0;i<servList->servNum;i++)
	        {          
	            servList->pServItems[i].tsId = g_dvbt2Ctx->pSdtInfoForSearch->tsId;
	            servList->pServItems[i].networkId = g_dvbt2Ctx->pSdtInfoForSearch->orgNetworkId;
                WARNING("servList->pServItems[%d].tsId is %d \n",i,servList->pServItems[i].tsId);
                WARNING("g_dvbt2Ctx->pSdtInfoForSearch->tsId is %d \n",g_dvbt2Ctx->pSdtInfoForSearch->tsId);
	        }
	    }
	    if(g_dvbt2Ctx->pSdtInfoForSearch)
	    {
	        deleteSDTInfo(g_dvbt2Ctx->pSdtInfoForSearch);
	        g_dvbt2Ctx->pSdtInfoForSearch = NULL;
	    }
	    
		
		//* search pat.
		g_dvbt2Ctx->endPatSection = 0;
	    g_dvbt2Ctx->curPatSecNum  = 0;
	    g_dvbt2Ctx->isBufFull[PAT_BUF_IDX] = 0;
		if(g_dvbt2Ctx->pPatInfoForSearch)
	    {
	        WARNING("fatal error! why g_dvbt2Ctx->pPatInfoForSearch[0x%x]!=NULL?\n", g_dvbt2Ctx->pPatInfoForSearch);
	        deletePATInfo(g_dvbt2Ctx->pPatInfoForSearch);
	        g_dvbt2Ctx->pPatInfoForSearch = NULL;
	    }
	    g_dvbt2Ctx->pPatInfoForSearch = newPATInfo();
	    if(NULL == g_dvbt2Ctx->pPatInfoForSearch)
	    {
	        WARNING("malloc PatInfo fail,skip nit search!\n");
			  servList->servNum = 0;
	        goto exit_dvbt2_search_service_task;
	    }
		dvbt2_open_section_filter(PAT_PID,0,&nPatVChanId);
        WARNING("dvbt2_open_section_filter \n");
	    nWaitSectionTime = 0;
		while(g_dvbt2Ctx->endPatSection == 0)
		{   
	        if(dvbt2_check_task_delete_request(EXEC_prioself) == OS_TASK_DEL_REQ)        // time is overflow
			{
			    dvbt2_close_section_filter(PAT_PID,nPatVChanId);
		        servList->servNum = 0;
				goto exit_dvbt2_search_service_task;
			}
	          esKRNL_TimeDly(10);
	          nWaitSectionTime++;
	          if(nWaitSectionTime*10 > WAIT_PAT_TIMEOUT)    //��ʱ
	          {
	              WARNING("search pat timeout!\n");
	              break;
	          }
		}
		dvbt2_close_section_filter(PAT_PID,nPatVChanId);
		  if(g_dvbt2Ctx->endPatSection)    //pat����Ѱ��ɾ���д.
	        {
	            //2.1 ��ʼ��дmaple��service�б�, ��Ҫ��дpmt_pid, oneseg/fullseg��Ϣ,tsId, ���sdt��ɹ�����,����sdt��Ϊ׼.
	            //������pat��Ϊ׼.�����sdt��,tsId���sdt��Ƚ�,����sdt��Ϊ׼.
	            //if((servList->servNum > 0) &&(servList->servNum == g_dvbCtx->pPatInfoForSearch->nServNum))   //˵��sdt���Ѿ���д��servList.pat����У������  
	            if((servList->servNum > 0) &&( nServiceNum == g_dvbt2Ctx->pPatInfoForSearch->nServNum))   //˵��sdt���Ѿ���д��servList.pat����У������  

				{
	                for(i=0;i<servList->servNum;i++)
	                {
	                    //Ѱ��servId
	                    for(j=1;j<=g_dvbt2Ctx->pPatInfoForSearch->nServNum;j++)//PAT��servNum ��1��ʼ
	                    {
	                        if(servList->pServItems[i].servId == g_dvbt2Ctx->pPatInfoForSearch->servInfoArray[j].nServId)
	                        {
	                            servList->pServItems[i].pmtId = g_dvbt2Ctx->pPatInfoForSearch->servInfoArray[j].pmtpid;
	                        	// if(servList->pServItems[i].pmtId >= PMT_PID_FOR_ONESEG && servList->pServItems[i].pmtId <= PMT_PID_FOR_ONESEG + 7)
	                            //{
	                            //    servList->pServItems[i].nOneSegFlag = 1;
	                          //  }
	                           // else
	                           // {
	                            //    servList->pServItems[i].nOneSegFlag = 0;
	                            //}
	                            if(servList->pServItems[i].tsId != g_dvbt2Ctx->pPatInfoForSearch->tsId)
	                            {
	                                WARNING("fatal error! sdt, pat is not same! tsId[0x%x][0x%x]\n", servList->pServItems[i].tsId, g_dvbt2Ctx->pPatInfoForSearch->tsId);
	                            }
	                            break;
	                        }
	                    }
	                }
	            }	      
                else    //���û���ѵ�pat��,�����˳�
		        {
		            ret = EPDK_FAIL;
		            goto exit_dvbt2_search_service_by_serviceId;
		        }	           
	        }
	        if(g_dvbt2Ctx->pPatInfoForSearch)
	        {
	            deletePATInfo(g_dvbt2Ctx->pPatInfoForSearch);
	            g_dvbt2Ctx->pPatInfoForSearch = NULL;
	        }
	    
		      //3. ��ʼpmt�������
		//* start search all pmt.
		  if(g_dvbt2Ctx->pPmtInfoForSearch)
	    {
	        WARNING("fatal error! why g_dvbt2Ctx->pPmtInfoForSearch[0x%x]!=NULL?\n", g_dvbt2Ctx->pPmtInfoForSearch);
	        deletePMTInfo(g_dvbt2Ctx->pPmtInfoForSearch);
	        g_dvbt2Ctx->pPmtInfoForSearch = NULL;
	    }

		 if(g_dvbt2Ctx->endPatSection) //�ѵ���pat��,��ʾservice�б��Ѿ�ȷ����service��������pmt_pid.��ô����service�б����Ŀ����pmt,pmt_pidΪ0��ʾû���ҵ�pmt_pid.��ô����������.
	    {
	        if(0 == servList->servNum)
	        {
	            WARNING("fatal error! sdt pat said no service??check!!\n");
	        }
		
				servIndex = 0;
				WARNING("3 servList->servNum =%d\n",servList->servNum);
				while(servIndex < servList->servNum)
				{   
			        nWaitSectionTime = 0;
					g_dvbt2Ctx->curPmtSecNum = 0;
	                g_dvbt2Ctx->isBufFull[PMT_BUF_IDX] = 0;
			        g_dvbt2Ctx->endPmtSection = 0;
			        g_dvbt2Ctx->pPmtInfoForSearch = newPMTInfo();
					   if(NULL == g_dvbt2Ctx->pPmtInfoForSearch)
		            {
		                WARNING("malloc PmtInfo fail,skip pmt search!\n");
		                goto exit_dvbt2_search_service_task;
		            }
						 WARNING("PMT PID:%x,%x\n",servList->pServItems[0].pmtId,servList->pServItems[1].pmtId);
		            if(0 == servList->pServItems[servIndex].pmtId)
		            {
		                WARNING("why pat not contain this serviceid[0x%x]?\n", servList->pServItems[servIndex].servId);
										servIndex++;
						   			continue;
		            }
							dvbt2_open_section_filter(servList->pServItems[servIndex].pmtId,servList->pServItems[servIndex].servId,&nPmtVChanId);
			        while(g_dvbt2Ctx->endPmtSection == 0)
			        {
			          		if(dvbt2_check_task_delete_request(EXEC_prioself) == OS_TASK_DEL_REQ)
					    	{  
						        dvbt2_close_section_filter(servList->pServItems[servIndex].pmtId,nPmtVChanId);
						
								WARNING("go to __dvb_t2_set_plp--->\n");
								demodParam.plpindex++;
								old_servNum = servList->servNum;
						        goto __dvbt2_t2_set_plp;					
					    	}
				        esKRNL_TimeDly(10);
						nWaitSectionTime++;
						if(nWaitSectionTime*100 > WAIT_PMT_TIMEOUT)
			            {
			                WARNING("pmtpid[0x%x] overtime!\n", servList->pServItems[servIndex].pmtId);
			                break;
			            }
				        
			        }
			        dvbt2_close_section_filter(servList->pServItems[servIndex].pmtId,nPmtVChanId);
						 //����pmt������һ�ž���дһ��.����ط�Ҫ���.
		            if(g_dvbt2Ctx->endPmtSection)    //pmt�ѵ��˾���д
		            {
		            		PmtSupplementServiceInfo(servList, g_dvbt2Ctx->pPmtInfoForSearch, servList->pServItems[servIndex].pmtId);
		            }
		            if(g_dvbt2Ctx->pPmtInfoForSearch)
		            {
		                deletePMTInfo(g_dvbt2Ctx->pPmtInfoForSearch);
		                g_dvbt2Ctx->pPmtInfoForSearch = NULL;			

		            }
						 servIndex ++;
						//servList->curServIdx++;		
				}
		 	} 
	    if(servList->servNum == 0)
	    {
	        WARNING("this freq[%d][%d] has no service?\n", demodParam.frequency, demodParam.bandwidth);
	        goto exit_dvbt2_search_service_task;
	    }
	    //������һ��curServItem->demod_pm.
	    for(i=0;i<servList->servNum;i++)
	    {
	        servList->pServItems[i].demod_pm.frequency = demodParam.frequency;
	        servList->pServItems[i].demod_pm.bandwidth = demodParam.bandwidth;
			WARNING("test0: demodParam.plpindex = %d, demod_pm.plppid = %d-----i = %d\n", demodParam.plpindex, servList->pServItems[i].demod_pm.plppid, i);
			if(i >= old_servNum)
			{
				servList->pServItems[i].demod_pm.plpindex  = demodParam.plpindex;
				servList->pServItems[i].demod_pm.plppid	   = demodParam.plppid;
				servList->pServItems[i].demod_pm.t_t2_flag = demodParam.t_t2_flag;
				WARNING("dvbt2_scan_task: demod_p,.t_t2_flag = %d\n", demodParam.t_t2_flag);
			}

	    }
	    
		///4. nit
	    // search nit.��ǰ��NIT��Ŀ����Ϊ��TS information descriptor�е�remote control id. 
	    //��Ҫ��дremote_control_id. 
	    //��service��Ϣ��pmt_pid,Ϊ0��ʾδ����. һ����û���ѵ�pat���pmt����ɵ�.
	    if(g_dvbt2Ctx->pNitInfoForSearch)
	    {
	        WARNING("fatal error! why g_dvbt2Ctx->pNitInfoForSearch[0x%x]!=NULL?\n", g_dvbt2Ctx->pNitInfoForSearch);
	        deleteNITInfo(g_dvbt2Ctx->pNitInfoForSearch);
	        g_dvbt2Ctx->pNitInfoForSearch = NULL;
	    }
	    g_dvbt2Ctx->curNitSecNum   = 0;
	    g_dvbt2Ctx->endNitSection  = 0;
	    g_dvbt2Ctx->isBufFull[NIT_BUF_IDX] = 0;

		 //���ȿ���tsId��û��
	    if(g_dvbt2Ctx->endPatSection || g_dvbt2Ctx->endSdtSection)
	    {
	        g_dvbt2Ctx->pNitInfoForSearch = newNITInfo();
	        if(NULL == g_dvbt2Ctx->pNitInfoForSearch)
	        {
	            WARNING("malloc NitInfo fail,quit!\n");
	            servList->servNum = 0;
	            goto exit_dvbt2_search_service_task;
	        }
	        dvbt2_open_section_filter(NIT_PID, 0, &nNitVChanId);
	        nWaitSectionTime = 0;
	        while(g_dvbt2Ctx->endNitSection == 0)
	        {
	        	if(dvbt2_check_task_delete_request(EXEC_prioself)==OS_TASK_DEL_REQ)        // time is overflow
	        	{   
	        		dvbt2_close_section_filter(NIT_PID, nNitVChanId);
	                servList->servNum = 0;
	                goto exit_dvbt2_search_service_task;
	        	}
	            esKRNL_TimeDly(1);
	            nWaitSectionTime++;
	            if(nWaitSectionTime * 10 > WAIT_NIT_TIMEOUT)
	            {
	                WARNING("wait nit timeout [%d]ms! no nit!\n", WAIT_NIT_TIMEOUT);
	                break;
	            }
	        }

            dvbt2_close_section_filter(NIT_PID, nNitVChanId);

			  //4.1��ʼ������дservice�б�,��ȷ��remotecontrolkeyid
			WARNING("g_dvbt2Ctx->endNitSection is %d \n",g_dvbt2Ctx->endNitSection);
	        if(g_dvbt2Ctx->endNitSection)
	        {
	            WARNING("g_dvbt2Ctx->endNitSection \n");
	            tsId = servList->pServItems[0].tsId;
	            //����tsIdȥ��nit�ж�Ӧ��ts��Ϣ
	            nTmp = g_dvbt2Ctx->pNitInfoForSearch->pList_NITTSInfo->GetLength(g_dvbt2Ctx->pNitInfoForSearch->pList_NITTSInfo);
	            if(nTmp <= 0)
	            {
	                WARNING("nit has not ts info!\n");
	                goto _nit_scan_exit;
	            }
	            nIdx = g_dvbt2Ctx->pNitInfoForSearch->pList_NITTSInfo->FindElem(g_dvbt2Ctx->pNitInfoForSearch->pList_NITTSInfo, DListDataCompareFunc_FindNITTSInfoByTsId, (void*)tsId);
	            if(nIdx < 0 || nIdx >= nTmp)
	            {
	                WARNING("Impossible! not find tsinfo by tsId[0x%x], nIdx[%d], nSize[%d], check code!\n", tsId,nIdx, nTmp);
	                goto _nit_scan_exit;
	            }
	            if(EPDK_OK != g_dvbt2Ctx->pNitInfoForSearch->pList_NITTSInfo->GetElemByIndex(g_dvbt2Ctx->pNitInfoForSearch->pList_NITTSInfo, nIdx, (void**)&pNITTSInfo))
	            {
	                WARNING("Impossible! Can't find elem [%d], check code!\n", nIdx);
	                goto _nit_scan_exit;
	            }

	            //������д,���pat��pmt����û���ѵ��Ļ�,����Ҫ��.
	           for(i=0;i<servList->servNum;i++)
	            {
	                if(0 == servList->pServItems[i].pmtId)
	                {
	                    WARNING("servId[0x%x] pmtpid is not sure!\n", servList->pServItems[i].servId);
                        WARNING("the address of pNITTSInfo is %p \n",pNITTSInfo);

                        for(j=0;j<pNITTSInfo->nServNum;j++)
	                    {
                            WARNING("the address of pNITTSInfo is %p \n",pNITTSInfo);

                            if(servList->pServItems[i].servId == pNITTSInfo->ServIdArray[j])
	                        {
	                          // servList->pServItems[i].nOneSegFlag = 1;
	                            break;
	                        }
	                    }
	                }
	            }	
	        }
            WARNING("at the line is %d \n",__LINE__);

	        if(g_dvbt2Ctx->pNitInfoForSearch)
	        {
                WARNING("at the line is %d \n",__LINE__);
                deleteNITInfo(g_dvbt2Ctx->pNitInfoForSearch);
                WARNING("at the line is %d \n",__LINE__);
	            g_dvbt2Ctx->pNitInfoForSearch = NULL;
	        }
	    }
	    else
	    {
	        WARNING("no pat,sdt, we don't know tsId, so search nit is not need!\n");
	    }
	    _nit_scan_exit:
			
		//���ж��Ƿ�NIT������remote_control_key_id��������ͳһ��ֵNIT���е�remote_control_key_id�� ��Ҫ���TV����	
	    //��Щû��remote_control_key_id������Radio����������remote_control_key_id������Ϊ���⸳ֵ����350 =0x15E ��ʼ��
	    //���NIT���û��remote_control_key_id����ȫ���Զ���ֵ����350 =0x15E ��ʼ��
        WARNING("servList->servNum[%d] \n",servList->servNum);	  
		for(i=0;i<servList->servNum;i++)
		{           
		 	//Note: pNITTSInfoҪΪ�ǿ�ʱ���ܱȽ�,���������
           WARNING("pNITTSInfo is [%x]nServNum[%x] i[%d]\n",pNITTSInfo,pNITTSInfo->nServNum,i);
		 	if(pNITTSInfo!=NULL)
		 	{
	            for(j=0;j<pNITTSInfo->nServNum;j++)
	            {

	                WARNING("servList->pServItems[%d].servId is %d \n",i,servList->pServItems[i].servId);
	                WARNING("pNITTSInfo->ServIdArray[j] is %d \n",pNITTSInfo->ServIdArray[j]);
		     	 	if ((pNITTSInfo != NULL) && (servList->pServItems[i].servId==pNITTSInfo->ServIdArray[j]))
		     	 	{
		     	 		WARNING("i[%d] \n",i);	
	                    servList->pServItems[i].nRemoteControlKeyId =  pNITTSInfo->nRemoteControlKeyId[j];
	                    break;
		     	 	}
					else
					{
							
	                    servList->pServItems[i].nRemoteControlKeyId  = START_REMOTE_CONTROL_KEY_ID+k;
					    k++;	
						WARNING("k[%d][%d] \n",k,pNITTSInfo->nServNum);
						if(k>pNITTSInfo->nServNum)
							break;
					}
	            }
		 	}
			else
			{
				WARNING("cky i[%d]pNITTSInfo[%x],servList->servNum[%d] \n",i,pNITTSInfo,servList->servNum);	
				 servList->pServItems[i].nRemoteControlKeyId  = START_REMOTE_CONTROL_KEY_ID+k;
					    k++;	
						WARNING("k[%d][%d] \n",k,servList->servNum);
						if(k>servList->servNum)
							break;
			}
	    } 
		
	    //5.֪ͨapp, �����ʾ.
	    for(i=0;i<servList->servNum;i++)
	    {
	        dvbt2_search_one_service_cb(&servList->pServItems[i]);
	    }
	    WARNING("test1: demodParam.plpindex = %d\n", demodParam.plpindex);
		if(demodParam.plpindex < demodParam.plpnumber)
		{
		
			demodParam.plpindex++;
			old_servNum = servList->servNum;
			WARNING("test2:demodParam.plpindex = %d\n", demodParam.plpindex);
			goto __dvbt2_t2_set_plp;
		}
	}
    WARNING("at the line is %d \n",__LINE__);

exit_dvbt2_search_service_task:

	if(g_dvbt2Ctx->pNitInfoForSearch)
    {
        deleteNITInfo(g_dvbt2Ctx->pNitInfoForSearch);
        g_dvbt2Ctx->pNitInfoForSearch = NULL;
    }
	 if(g_dvbt2Ctx->pPmtInfoForSearch)
    {
        deletePMTInfo(g_dvbt2Ctx->pPmtInfoForSearch);
        g_dvbt2Ctx->pPmtInfoForSearch = NULL;
    }
	  if(g_dvbt2Ctx->pPatInfoForSearch)
    {
        deletePATInfo(g_dvbt2Ctx->pPatInfoForSearch);
        g_dvbt2Ctx->pPatInfoForSearch = NULL;
    }	

    dvbt2_search_service_finish_cb();
    g_dvbt2Ctx->slSearchStopFlag = 1;
    dvbt2_delete_task(EXEC_prioself);
	return ret;
}

#endif
/*******************************************************************************
Function name: dvbt2_scan_by_serviceId
Description: 
    1.����serviceIdֻ��һ��service����ϸ��Ϣ,ֻ��pat���pmt��,��Ҫ��Ϊ�˵õ������ű����Ϣ,v/a/subt pid,pcrpid��.
    �����SI����.
    2.���Ը���maple si lib���ص����ݿ�(��service�б�)
    3.����ֻ���:patû�ѵ�,����service��û��pmt_pid�����.��������Ժ������ٸ�.
Parameters: 
    
Return: 
    EPDK_OK:�ɹ��ҵ�
    EPDK_FAIL:û���ҵ�.
Time: 2012/6/19
*******************************************************************************/
__s32 dvbt2_scan_by_serviceId(maple_serv_item_t* pServItem)
{
    //__u32               i = 0;
    __u32               j = 0;
    //__u32               maxDelayNum = 100;
    maple_demod_info_t  demodParam;
    __s32               nWaitSectionTime = 0;
    //maple_serv_item_t   *curServItem    = NULL;
    //__u16               nRemoteControlKeyId;
    //__u16               tsId = 0;
    //__s32               nTmp;
    //__s32               nIdx;
    //NITTSInfo           *pNITTSInfo = NULL;
    //__u32               servIndex  = 0;
    //__s32               nServiceNum = 0;
    //__s32               tmpret;
	__s32               ret = EPDK_FAIL;
    
    //__s32               nSdtVChanId = -1;
    __s32               nPatVChanId = -1;
    __s32               nPmtVChanId = -1;
    //__s32               nNitVChanId = -1;
	//* step 1, tune.
	demodParam.frequency = pServItem->demod_pm.frequency;
	demodParam.bandwidth = pServItem->demod_pm.bandwidth;
	
    if(dvbt2_tune(&demodParam) != 0)
    {   
        ret = EPDK_FAIL;
        goto exit_dvbt2_search_service_by_serviceId;
    }
	//dvbt2_get_tuner_param(&demodParam);
    if(pServItem->pmtId)
    {
        WARNING("why need search servId[0x%x], pmtpid[0x%x]?\n", pServItem->servId, pServItem->pmtId);
    }
    
    //2.����pat��,�����oneseg��Ƶ��,ֱ������
    g_dvbt2Ctx->curPatSecNum      = 0;
    g_dvbt2Ctx->endPatSection     = 0;
    g_dvbt2Ctx->isBufFull[PAT_BUF_IDX] = 0;
   // if(DEMOD_BANDWIDTH_1_SEG != demodParam.bandwidth)
    //{
        //* search pat.
        if(g_dvbt2Ctx->pPatInfoForSearch)
        {
            WARNING("fatal error! why g_dvbt2Ctx->pPatInfoForSearch[0x%x]!=NULL?\n", g_dvbt2Ctx->pPatInfoForSearch);
            deletePATInfo(g_dvbt2Ctx->pPatInfoForSearch);
            g_dvbt2Ctx->pPatInfoForSearch = NULL;
        }
        g_dvbt2Ctx->pPatInfoForSearch = newPATInfo();
        if(NULL == g_dvbt2Ctx->pPatInfoForSearch)
        {
            WARNING("malloc PatInfo fail,skip nit search!\n");
            ret = EPDK_FAIL;
            goto exit_dvbt2_search_service_by_serviceId;
        }
	    dvbt2_open_section_filter(PAT_PID, 0,&nPatVChanId);
        nWaitSectionTime = 0;
	    while(g_dvbt2Ctx->endPatSection == 0)
	    {	
	    	if(dvbt2_check_task_delete_request(EXEC_prioself) == OS_TASK_DEL_REQ)        // time is overflow
	    	{
	    	    dvbt2_close_section_filter(PAT_PID, nPatVChanId);
                ret = EPDK_FAIL;
	    		goto exit_dvbt2_search_service_by_serviceId;
	    	}
            esKRNL_TimeDly(1);
            nWaitSectionTime++;
            if(nWaitSectionTime*10 > WAIT_PAT_TIMEOUT)    //��ʱ
            {
                WARNING("search pat timeout!\n");
                break;
            }
	    }
	    dvbt2_close_section_filter(PAT_PID, nPatVChanId);
        if(g_dvbt2Ctx->endPatSection)    //pat����Ѱ��ɾ���д.����ֻ��Ҫ������service.
        {
            //2.1 ��ʼ��дmaple��service�б�, ��Ҫ��дpmt_pid, oneseg/fullseg��Ϣ,tsId, ���sdt��ɹ�����,����sdt��Ϊ׼.
            //������pat��Ϊ׼.�����sdt��,tsId���sdt��Ƚ�,����sdt��Ϊ׼.
            //Ѱ��servId
            for(j=0;j<g_dvbt2Ctx->pPatInfoForSearch->nServNum;j++)
            {
                if(pServItem->servId == g_dvbt2Ctx->pPatInfoForSearch->servInfoArray[j].nServId)
                {
                    pServItem->pmtId = g_dvbt2Ctx->pPatInfoForSearch->servInfoArray[j].pmtpid;
                 //   if(pServItem->pmtId >= PMT_PID_FOR_ONESEG && pServItem->pmtId <= PMT_PID_FOR_ONESEG + 7)
                  //  {
                  //      pServItem->nOneSegFlag = 1;
                 //   }
                 //   else
                  //  {
                 //       pServItem->nOneSegFlag = 0;
               //     }
                    if(pServItem->tsId != g_dvbt2Ctx->pPatInfoForSearch->tsId)
                    {
                        WARNING("why tsId is not same as pat tsId[0x%x][0x%x]\n", pServItem->tsId, g_dvbt2Ctx->pPatInfoForSearch->tsId);
                        pServItem->tsId = g_dvbt2Ctx->pPatInfoForSearch->tsId;
                    }
                    break;
                }
            }
            if(j == g_dvbt2Ctx->pPatInfoForSearch->nServNum)
            {
                WARNING("this serviceId[0x%x] is not in pat!\n", pServItem->servId);
                ret = EPDK_FAIL;
                goto exit_dvbt2_search_service_by_serviceId;
            }
        }
        else    //���û���ѵ�pat��,�����˳�
        {
            ret = EPDK_FAIL;
            goto exit_dvbt2_search_service_by_serviceId;
        }
        if(g_dvbt2Ctx->pPatInfoForSearch)
        {
            deletePATInfo(g_dvbt2Ctx->pPatInfoForSearch);
            g_dvbt2Ctx->pPatInfoForSearch = NULL;
        }
   // }
    
    //3. ��ʼpmt�������
    //pmt�����Ϣ,��Ҫ��дvideo/audio/subtitle pid, pcrPid, streamtype, streamname, oneseg/fullsegҲ����һ��.���service type��û��,��ôҲ������ȷ��.˳��Ƚ�һ��.
    if(g_dvbt2Ctx->pPmtInfoForSearch)
    {
        WARNING("fatal error! why g_dvbt2Ctx->pPmtInfoForSearch[0x%x]!=NULL?\n", g_dvbt2Ctx->pPmtInfoForSearch);
        deletePMTInfo(g_dvbt2Ctx->pPmtInfoForSearch);
        g_dvbt2Ctx->pPmtInfoForSearch = NULL;
    }
    if(pServItem->pmtId) //ȷ����pmt_pid.�Ϳ�ʼ��pmt
    {
        //* search all pmt.
        nWaitSectionTime = 0;
        g_dvbt2Ctx->curPmtSecNum = 0;
        g_dvbt2Ctx->isBufFull[PMT_BUF_IDX] = 0;
        g_dvbt2Ctx->endPmtSection = 0;
        g_dvbt2Ctx->pPmtInfoForSearch = newPMTInfo();
        if(NULL == g_dvbt2Ctx->pPmtInfoForSearch)
        {
            WARNING("malloc PmtInfo fail,skip pmt search!\n");
            ret = EPDK_FAIL;
            goto exit_dvbt2_search_service_by_serviceId;
        }
    	dvbt2_open_section_filter(pServItem->pmtId,0, &nPmtVChanId);
    	while(g_dvbt2Ctx->endPmtSection == 0)	
    	{
             if(dvbt2_check_task_delete_request(EXEC_prioself) == OS_TASK_DEL_REQ)
    	    {  
    	        dvbt2_close_section_filter(pServItem->pmtId, nPmtVChanId);
                ret = EPDK_FAIL;
    	        goto exit_dvbt2_search_service_by_serviceId;
    	    }
            esKRNL_TimeDly(10);
            nWaitSectionTime++;
            if(nWaitSectionTime*100 > WAIT_PMT_TIMEOUT)
            {
                WARNING("pmtpid[0x%x] overtime!\n", pServItem->pmtId);
                break;
            }
        }
        dvbt2_close_section_filter(pServItem->pmtId, nPmtVChanId);
        //����pmt������һ�ž���дһ��.����ط�Ҫ���.
        if(g_dvbt2Ctx->endPmtSection)    //pmt�ѵ��˾���д
        {
            PmtFillServItem(pServItem, g_dvbt2Ctx->pPmtInfoForSearch, pServItem->pmtId);
        }
        if(g_dvbt2Ctx->pPmtInfoForSearch)
        {
            deletePMTInfo(g_dvbt2Ctx->pPmtInfoForSearch);
            g_dvbt2Ctx->pPmtInfoForSearch = NULL;
        }
        ret = EPDK_OK;
    }
    else
    {
        WARNING("fatal error! check code!\n");
        ret = EPDK_FAIL;
    }
	
exit_dvbt2_search_service_by_serviceId:
    if(g_dvbt2Ctx->pPmtInfoForSearch)
    {
        deletePMTInfo(g_dvbt2Ctx->pPmtInfoForSearch);
        g_dvbt2Ctx->pPmtInfoForSearch = NULL;
    }
    if(g_dvbt2Ctx->pPatInfoForSearch)
    {
        deletePATInfo(g_dvbt2Ctx->pPatInfoForSearch);
        g_dvbt2Ctx->pPatInfoForSearch = NULL;
    }
//    WARNING("freq[%d] search done! wait 20s!\n", demodParam.frequency);
//    esKRNL_TimeDly(2000);
    return ret;
}


void dvbt2_time_monitor_task(void* arg)
{
	__s32   nTdtVChanId = -1;
    dvbt2_open_section_filter(TDT_TOT_PID,0,&nTdtVChanId);
    
	while(1)
	{   
		if(dvbt2_check_task_delete_request(EXEC_prioself) == OS_TASK_DEL_REQ)        // time is overflow
			break;
		esKRNL_TimeDly(10);	
	}
	
    dvbt2_close_section_filter(TDT_TOT_PID,nTdtVChanId);
    g_dvbt2Ctx->timeMonitorStopFlag = 1;
    dvbt2_delete_task(EXEC_prioself);
}


void dvbt2_epg_monitor_task(void* arg)
{
	__s32   nEitVChanId = -1;
   
    //* open EIT Section filter.
    g_dvbt2Ctx->isBufFull[EIT_BUF_IDX] = 0;
    dvbt2_open_section_filter(EIT_PID,0,&nEitVChanId);

    while (1)
    {
		if(dvbt2_check_task_delete_request(EXEC_prioself) == OS_TASK_DEL_REQ)        // time is overflow
			break;

        esKRNL_TimeDly(10);	
    }
    
    //* close pat section filter.
    dvbt2_close_section_filter(EIT_PID,nEitVChanId);
    g_dvbt2Ctx->epgMonitorStopFlag = 1;
    dvbt2_delete_task(EXEC_prioself);
}




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  SI Table Parse Info
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//PAT Info
__s32 PATInfo_Init(PATInfo *thiz)
{
    memset(thiz, 0, sizeof(PATInfo));
    return EPDK_OK;
}

__s32 PATInfo_Exit(PATInfo *thiz)
{
    return EPDK_OK;
}

PATInfo* newPATInfo()
{
    __s32   tmpret;
    PATInfo *pPATInfo = (PATInfo*)malloc(sizeof(PATInfo));
    if(NULL == pPATInfo)
    {
        WARNING("pat info mallc fail\n");
        return NULL;
    }
    //��ʼ����Ա����
    tmpret = PATInfo_Init(pPATInfo);
    if(tmpret != EPDK_OK)
    {
        goto _err0;
    }
    return pPATInfo;
    
_err0:
    free(pPATInfo);
    pPATInfo = NULL;
    return NULL;
}

void deletePATInfo(PATInfo* thiz)
{
    return_if_fail(thiz!=NULL);
    PATInfo_Exit(thiz);
    free(thiz);
}

__s32 PMTInfo_Init(PMTInfo *thiz)
{
    memset(thiz, 0, sizeof(PMTInfo));
    return EPDK_OK;
}
__s32 PMTInfo_Exit(PMTInfo *thiz)
{
    return EPDK_OK;
}
PMTInfo* newPMTInfo()
{
    __s32   tmpret;
    PMTInfo *pPMTInfo = (PMTInfo*)malloc(sizeof(PMTInfo));
    if(NULL == pPMTInfo)
    {
        WARNING("pmt info mallc fail\n");
        return NULL;
    }
    //��ʼ����Ա����
    tmpret = PMTInfo_Init(pPMTInfo);
    if(tmpret != EPDK_OK)
    {
        goto _err0;
    }
    return pPMTInfo;
    
_err0:
    free(pPMTInfo);
    pPMTInfo = NULL;
    return NULL;
}

void deletePMTInfo(PMTInfo* thiz)
{
    return_if_fail(thiz!=NULL);
    PMTInfo_Exit(thiz);
    free(thiz);
}

SDTServInfo* newSDTServInfo()
{
    SDTServInfo *pSDTServInfo = (SDTServInfo*)malloc(sizeof(SDTServInfo));
    if(NULL == pSDTServInfo)
    {
        WARNING("SDTServInfo mallc fail\n");
        return NULL;
    }
    //��ʼ����Ա����
    memset(pSDTServInfo, 0, sizeof(SDTServInfo));
    return pSDTServInfo;
}
void deleteSDTServInfo(SDTServInfo* thiz)
{
    free(thiz);
}

/*******************************************************************************
Function name: DListDataVisitFunc_SDTAddService
Description: 
    1.������̨ʱ,�����ѵ�sdt��,Ȼ����sdt�����Ϣ,��дservList. servList��ʼӦ��Ϊ��
    2. Ŀǰֻ����tv��radio,�����Ե�. nServType = 0Ҳ�����б�.��Ϊ��������Ϊû��service descriptor��ԭ��.
    3. serviceid,servicetype, servicename, isCAFree, tsId, orgNetworkId(��Ӧmaple_serv_item_t->networkId),
        ����, tsId, orgNetworkIdû�������︳ֵ.
    
Parameters: 
    
Return: 
    
Time: 2012/6/17
*******************************************************************************/
DListRet DListDataVisitFunc_SDTAddService(void* ctx, void* data)   //ctx = maple_serv_list_t   *servList; data = SDTServInfo*
{
    maple_serv_list_t   *pServList = (maple_serv_list_t*)ctx;
    SDTServInfo         *pSDTServInfo = (SDTServInfo*)data;
    if(pServList->servNum >= pServList->nServItemsCount)
    {
        WARNING("Service List is full!");
        return DLIST_RET_STOP;
    }
    if(0x01 == pSDTServInfo->nServType || 0x02 == pSDTServInfo->nServType || 0x03 == pSDTServInfo->nServType || 0x16 == pSDTServInfo->nServType|| 0x19 == pSDTServInfo->nServType) //tv,radio,teletext//0x16,0x19 taiguo
    {
        pServList->pServItems[pServList->servNum].servId = pSDTServInfo->nServId;
        if(pSDTServInfo->nServNameLen)
        {
            safeStringCopy(pServList->pServItems[pServList->servNum].servName, MAPLE_MAX_NAME_LEN, pSDTServInfo->servName, pSDTServInfo->nServNameLen);
        }
        else
        {
            safeStringCopy(pServList->pServItems[pServList->servNum].servName, MAPLE_MAX_NAME_LEN, DEFAULT_SERVICE_NAME, strlen(DEFAULT_SERVICE_NAME));
        }
        pServList->pServItems[pServList->servNum].isCAFree = 1 - pSDTServInfo->nFreeCAMode;
        switch(pSDTServInfo->nServType)
        {
            case 0x01:
								if(pServList->pServItems[pServList->servNum].vdStreamNum > 0)
			            pServList->pServItems[pServList->servNum].servType = MAPLE_SERV_TYPE_TV;
								else
									pServList->pServItems[pServList->servNum].servType = MAPLE_SERV_TYPE_UNKNOWN;
                break;
            case 0x02:
								if(pServList->pServItems[pServList->servNum].adStreamNum > 0)
			            pServList->pServItems[pServList->servNum].servType = MAPLE_SERV_TYPE_RADIO;
								else
									pServList->pServItems[pServList->servNum].servType = MAPLE_SERV_TYPE_UNKNOWN;
                break;
            case 0x03:
                if(pServList->pServItems[pServList->servNum].subtitleStreamNum > 0)
                	pServList->pServItems[pServList->servNum].servType = MAPLE_SERV_TYPE_TELETEXT;
								else
									pServList->pServItems[pServList->servNum].servType = MAPLE_SERV_TYPE_UNKNOWN;
	                break;
            case 0x16://taiguo
								if(pServList->pServItems[pServList->servNum].vdStreamNum > 0)
			            pServList->pServItems[pServList->servNum].servType = MAPLE_SERV_TYPE_TV;
								else
									pServList->pServItems[pServList->servNum].servType = MAPLE_SERV_TYPE_UNKNOWN;
                break;
            case 0x19://taiguo
								if(pServList->pServItems[pServList->servNum].vdStreamNum > 0)
			            pServList->pServItems[pServList->servNum].servType = MAPLE_SERV_TYPE_TV;
								else
									pServList->pServItems[pServList->servNum].servType = MAPLE_SERV_TYPE_UNKNOWN;
	                break;
            default:
                WARNING("parse sdt section, service type unknow, Judge_Month service type from es streams numbers.");
                break;
        }
        pServList->servNum++;
    }
    else
    {	
        WARNING("ignore servicetype[0x%x]\n", pSDTServInfo->nServType);
	 	pServList->pServItems[pServList->servNum].servType = MAPLE_SERV_TYPE_UNKNOWN;
	 }
    return DLIST_RET_OK;
}



/*******************************************************************************
Function name: DListDataDestroyFunc_NITTSInfo
Description: 
    1.free SDTServInfo
    //typedef void     (*DListDataDestroyFunc)(void* ctx, void* data);    //DList��ʼ��ʱ�ʹ���, ���й�������Ч, ctx�������ģ���ʼ��ʱ�ʹ���
Parameters: 
    
Return: 
    
Time: 2012/6/18
*******************************************************************************/
void DListDataDestroyFunc_SDTServInfo(void* ctx, void* data)
{
    SDTServInfo *pSDTServInfo = (SDTServInfo*)data;
    deleteSDTServInfo(pSDTServInfo);
}

//SDT Info
__s32 SDTInfo_Init(SDTInfo *thiz)
{
    memset(thiz, 0, sizeof(SDTInfo));
    thiz->pList_SDTServInfo = newDList(DListDataDestroyFunc_SDTServInfo, NULL);
    if(NULL == thiz->pList_SDTServInfo)
    {
        WARNING("SDTServInfo Dlist create fail\n");
        goto _err0;
    }
    return EPDK_OK;
    
_err0:
    return EPDK_FAIL;
}

__s32 SDTInfo_Exit(SDTInfo *thiz)
{
    if(thiz->pList_SDTServInfo)
    {
        deleteDList(thiz->pList_SDTServInfo);
        thiz->pList_SDTServInfo = NULL;
    }
    return EPDK_OK;
}

SDTInfo* newSDTInfo()
{
    __s32   tmpret;
    SDTInfo *pSDTInfo = (SDTInfo*)malloc(sizeof(SDTInfo));
    if(NULL == pSDTInfo)
    {
        WARNING("sdt info mallc fail\n");
        return NULL;
    }
    //��ʼ����Ա����
    tmpret = SDTInfo_Init(pSDTInfo);
    if(tmpret != EPDK_OK)
    {
        goto _err0;
    }
    return pSDTInfo;
    
_err0:
    free(pSDTInfo);
    pSDTInfo = NULL;
    return NULL;
}

void deleteSDTInfo(SDTInfo* thiz)
{
    return_if_fail(thiz!=NULL);
    SDTInfo_Exit(thiz);
    free(thiz);
}

//NIT Table Info
NITTSInfo* newNITTSInfo()
{
	
    NITTSInfo *pNITTSInfo = (NITTSInfo*)malloc(sizeof(NITTSInfo));
	WARNING("1 init NITTSInfo\n");
    if(NULL == pNITTSInfo)
    {
        WARNING("NITTSInfo mallc fail\n");
        return NULL;
    }
    //��ʼ����Ա����
    memset(pNITTSInfo, 0, sizeof(NITTSInfo));
    return pNITTSInfo;
}
void deleteNITTSInfo(NITTSInfo* thiz)
{
    free(thiz);
}


/*******************************************************************************
Function name: DListDataCompareFunc_FindNITTSInfoByTsId
Description: 
    ��ƥ��tsId��ts��Ϣ
    typedef int      (*DListDataCompareFunc)(void* ctx, void* data);    //DList find elemʱ����ʱ����, ctx����ʱ�����������
Parameters: 
    ctx: tsId;
    
Return: 
    0:ƥ��
    -1:��ƥ��
Time: 2012/3/1
*******************************************************************************/
int DListDataCompareFunc_FindNITTSInfoByTsId(void *ctx, void *data)
{
    __u16       tsId = (__u16)(__u32)ctx;
    NITTSInfo   *pNITTSInfo = (NITTSInfo*)data;
    if(pNITTSInfo->ts_id == tsId)
    {
        return 0;
    }
    else
    {
        return -1;
    }
}

/*******************************************************************************
Function name: DListDataDestroyFunc_NITTSInfo
Description: 
    1.free NITTSInfo
    //typedef void     (*DListDataDestroyFunc)(void* ctx, void* data);    //DList��ʼ��ʱ�ʹ���, ���й�������Ч, ctx�������ģ���ʼ��ʱ�ʹ���
Parameters: 
    
Return: 
    
Time: 2012/2/29
*******************************************************************************/
void DListDataDestroyFunc_NITTSInfo(void* ctx, void* data)
{
    NITTSInfo *pNITTSInfo = (NITTSInfo*)data;
	WARNING("2 NITTSInfo delete\n");
    deleteNITTSInfo(pNITTSInfo);
}

__s32 NITInfo_Init(NITInfo *thiz)
{
    memset(thiz, 0, sizeof(NITInfo));
	WARNING("1 NITTSInfo delete\n");
    thiz->pList_NITTSInfo = newDList(DListDataDestroyFunc_NITTSInfo, NULL);
    if(NULL == thiz->pList_NITTSInfo)
    {
        WARNING("NITTSInfo Dlist create fail\n");
        goto _err0;
    }
    return EPDK_OK;

_err0:
    return EPDK_FAIL;
}

__s32 NITInfo_Exit(NITInfo *thiz)
{
    if(thiz->pList_NITTSInfo)
    {
        deleteDList(thiz->pList_NITTSInfo);
        thiz->pList_NITTSInfo = NULL;
    }
    return EPDK_OK;
}

NITInfo* newNITInfo()
{
    __s32   tmpret;
    NITInfo *pNITInfo = (NITInfo*)malloc(sizeof(NITInfo));
    if(NULL == pNITInfo)
    {
        WARNING("nitinfo mallc fail\n");
        return NULL;
    }
    //��ʼ����Ա����
    tmpret = NITInfo_Init(pNITInfo);
    if(tmpret != EPDK_OK)
    {
        goto _err0;
    }
    return pNITInfo;
    
_err0:
    free(pNITInfo);
    pNITInfo = NULL;
    return NULL;
}

void deleteNITInfo(NITInfo* thiz)
{
    return_if_fail(thiz!=NULL);
    NITInfo_Exit(thiz);
    free(thiz);
}



