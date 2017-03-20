
#include "isdb_context.h"
#include "isdb_eit_pool.h"
#include "isdb_eit_parser.h"

//STD-B10v4_6-E2_Service Information for Digital Broadcasting System, P83
#define WAIT_SDT_TIMEOUT    (5000)  //�ȴ�sdt��ʱ����ֵ,��λms, �涨Ϊ2s.��Ϊ3s������
#define WAIT_NIT_TIMEOUT    (5000)  //�ȴ�sdt��ʱ����ֵ,��λms, һ����10s,���ﶨΪ5s.
#define WAIT_PAT_TIMEOUT    (1500)  //�ȴ�pat��ʱ����ֵ,��λms, �涨Ϊ100ms,���ﶨΪ1s
#define WAIT_PMT_TIMEOUT    (2000)  //�ȴ�pmt��ʱ����ֵ,��λms, �涨Ϊ100ms,���ﶨΪ200ms.



#define DEFAULT_SERVICE_NAME    "NoName"
#define DEFAULT_REMOTE_CONTROL_KEY_ID    (0xff)

maple_serv_list_t   *servList;

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

static __u32 isdb_bcd_to_int(__u32 val)
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
__s32 parse_partial_reception_descriptor_NIT(NITTSInfo* pNITTSInfo, __u32 despTag, __s32 despLen, __u8 *content)
{
    __s32   i;
    pNITTSInfo->nOneSegServNum = despLen / 2;
    if(pNITTSInfo->nOneSegServNum > ONESEG_MAX_SERVICE_NUM)
    {
        eDbug("impossbile? oneseg sevice num = [%d]?\n", pNITTSInfo->nOneSegServNum);
        pNITTSInfo->nOneSegServNum = ONESEG_MAX_SERVICE_NUM;
    }
    for(i=0; i<pNITTSInfo->nOneSegServNum; i++)
    {
        pNITTSInfo->OneSegServIdArray[i] = SI_MERGE_U16(content);
        content+=2;
    }
    return EPDK_OK;
}

/*******************************************************************************
Function name: isdb_parse_pmt_descriptor
Description:
    1.��ʱstreamtype��streampid���Ѿ���ֵ��,����streamtype��Ҫ�������������޸�.
    2.һ�δ���һ��������
    3.��Ҫ�����һ���õ�private stream������.
Parameters:

Return:

Time: 2012/6/18
*******************************************************************************/
static __u32 isdb_parse_pmt_descriptor(PMTStreamInfo *pPMTStreamInfo, __u8 *dataPtr, __s32 streamType, __s32 esInfoLen)
{
    __u8 despTag        =   0;
    __u8 despLen        =   0;
    __u16   data_component_id = 0;

    if(esInfoLen > 0)
    {
        despTag        =   dataPtr[0];
        despLen        =   dataPtr[1] + 2;
    }

    if(streamType == STREAM_TYPE_PRIVATE_DATA)
    {
        if((despTag==AC3_DESCRIPTOR_TAG) || (despTag==ENHANCED_AC3_DESCRIPTOR_TAG))
            streamType  =   STREAM_TYPE_AUDIO_AC3;          /*assume DVB AC-3 Audio*/
        else if(despTag == DTS_DESCRIPTOR_TAG)
            streamType  =   STREAM_TYPE_AUDIO_DTS;          /* DVB DTS audio */
        else if(despTag == SUBTITLING_DESCRIPTOR_TAG)
            streamType  =   STREAM_TYPE_SUBTITLE_DVB;
        else if(despTag == 0x52)
        {
            eDbug("## Be careful: STREAM_IDENTIFIER_DESCRIPTOR_TAG!\n");
            if (0xfd == (__u8)*(dataPtr + despLen))
            {
                eDbug("## Be careful: STREAM_TYPE_SUBTITLE_ARIB!\n");
                streamType = STREAM_TYPE_SUBTITLE_ARIB;
            }
        }
        else if(DATA_COMPONENT_DESCRIPTOR_TAG == despTag)
        {
            data_component_id = SI_MERGE_U16(&dataPtr[2]);

            if(0x08 == data_component_id || 0x12 == data_component_id)    //isdb si spec:Page206, ˵����ARIB-Subtitle
            {
                if (streamType == STREAM_TYPE_PRIVATE_DATA)
                {
                    eDbug("detect isdb subtitle!data_component_id[0x%x]\n", data_component_id);
                    streamType = STREAM_TYPE_SUBTITLE_ARIB;
                }
            }
        }
        else if(despTag == REGISTRATION_DESCRIPTOR_TAG)
        {
            if((dataPtr[2]=='D')&&(dataPtr[3]=='T')&&(dataPtr[4]=='S'))
                streamType  =   STREAM_TYPE_AUDIO_DTS;
            else if((dataPtr[2]=='d')&&(dataPtr[3]=='t')&&(dataPtr[4]=='s'))
                streamType  =   STREAM_TYPE_AUDIO_DTS;
            else if((dataPtr[2]=='V')&&(dataPtr[3]=='C')&&(dataPtr[4]=='-')&&(dataPtr[5]=='1'))
                streamType = STREAM_TYPE_VIDEO_VC1;
        }
        pPMTStreamInfo->nStreamType = streamType;
    }

    switch(despTag)
    {
        case ISO_639_LANGUAGE_DESCRIPTOR_TAG:
            safeStringCopy(pPMTStreamInfo->StreamName, MAX_STREAMNAME_SIZE, dataPtr + 2, 3);
            break;
        case SUBTITLING_DESCRIPTOR_TAG:
            safeStringCopy(pPMTStreamInfo->StreamName, MAX_STREAMNAME_SIZE, dataPtr + 2, 3);
            break;
        default:
        {
            eDbug("pmt ignore descriptor_tag[0x%x]\n", despTag);
            break;
        }
    }
    return despLen;
}

static void isdb_parse_sdt_descriptor(SDTServInfo* pSDTServInfo, __u8 *sectionPtr)
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
            sectionPtr += 4+servProviderLen;
            if(sectionPtr[0] > 0)
                pSDTServInfo->nServNameLen = isdb_convert_str_to_utf8(sectionPtr+1, sectionPtr[0], pSDTServInfo->servName, MAX_SERVNAME_SIZE, 0);
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
Function name: isdb_parse_nit_descriptor_loop2
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
static __s32 isdb_parse_nit_descriptor_loop2(NITTSInfo* pNITTSInfo, __u8 *sectionPtr)
{
    __u32   despTag = 0;
    __s32   despLen = 0;
    __s32   ret = EPDK_FAIL;
    despTag = sectionPtr[0];
    despLen = sectionPtr[1];
    switch(despTag)
    {
        case TS_INFORMATION_DESCRIPTOR_TAG:              //TS��Ϣ������,isdb si����
        {
            pNITTSInfo->nRemoteControlKeyId = sectionPtr[2];
            ret = EPDK_OK;
            break;
        }
        case PARTIAL_RECEPTION_DESCRIPTOR_TAG:
        {
            ret = parse_partial_reception_descriptor_NIT(pNITTSInfo, despTag, despLen, sectionPtr+2);
            break;
        }
        default:
        {
            ret = 1;
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
        case STREAM_TYPE_SUBTITLE_ARIB:
            return EPDK_TRUE;
        default:
            return EPDK_FALSE;
    }
}


static __s32 isdb_parse_pmt_section(PMTInfo *pPmtInfoForSearch, __u8 *section, __u32* sectionNum)
{
    __s32 sectionLen                = 0;
    __s32 esInfoLen                 = 0;
    __u32 pgmInfoLen                = 0;
    //__u32 streamPid                   = 0;
    __s32 streamType                = 0;
    __u32 despLen                   = 0;
    //__s32 nStreamIdx                = 0;
    __u8  *orgDataPtr               = NULL;
    PMTStreamInfo   *pPMTStreamInfo = NULL;
    //maple_serv_item_t* curServItem  = NULL;

    orgDataPtr                      = section;


    if(section[6] != (*sectionNum))     //step-1 check section number
    {
        eDbug("not we need pmt section num[%d][%d]\n", section[6], *sectionNum);
        return 0;
    }
    else if(section[0] != 0x02)     //0x02Ϊ��ǰts��pmt table id
    {
        return 0;
    }

    if((section[5] & 0x1) == 0)        // ��ǰpmt������
    {
        eDbug("cur pmt section nextIndecator == 0, return!\n");
        return 0;
    }

    pPmtInfoForSearch->nServId = (section[3]<<8) | section[4];
    pPmtInfoForSearch->pcrPid = ((section[8]&0x1f)<<8) | section[9];
	eDbug("pPmtInfoForSearch->nServId = %x,pPmtInfoForSearch->pcrPid is 0x%x \n",pPmtInfoForSearch->nServId,pPmtInfoForSearch->pcrPid);

    sectionLen  =   ((section[1]&0x0f)<<8)  | section[2];
    pgmInfoLen  =   ((section[10]&0x0f)<<8) | section[11];
    section     +=  (12 + pgmInfoLen);
    sectionLen  -=  (13 + pgmInfoLen);  //second loop length

    pPmtInfoForSearch->nStreamNum = 0;
    while(sectionLen > 0)
    {
        if(pPmtInfoForSearch->nStreamNum >= MAX_STREAM_NUM_IN_SERVICE)
        {
            eDbug("pmt stream array is full!break![%d]\n", pPmtInfoForSearch->nStreamNum);
            break;
        }
        pPMTStreamInfo = &pPmtInfoForSearch->streamInfoArray[pPmtInfoForSearch->nStreamNum];
        pPMTStreamInfo->nStreamType = streamType  = section[0];
        pPMTStreamInfo->nStreamPid = ((section[1]&0x1f)<<8) | section[2];
        esInfoLen   = ((section[3]&0x0f)<<8) | section[4];
        sectionLen -= (5+esInfoLen);
        if(sectionLen < 0)
        {
            eDbug("fatal error! PMT section loop parse error! check pmt section or code![%d][%d]\n", sectionLen, esInfoLen);
            break;
        }
        section    += 5;

        do
        {
            //* despLen: current descriptor length
            despLen      = isdb_parse_pmt_descriptor(pPMTStreamInfo, section, streamType, esInfoLen);
            section     += despLen;
            esInfoLen   -= despLen;
        }
        while(esInfoLen > 0);
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
Function name: isdb_parse_pat_section
Description:

Parameters:

Return:
    1:pat��section����������
    0:patû�н�����
Time: 2011/1/4
*******************************************************************************/
static __s32 isdb_parse_pat_section(PATInfo *pPatInfoForSearch, __u8 *section, __u32* sectionNum)
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
        for(i=0; i<prgmNum; i++)    //* ����PAT��ÿһ�� program_map_pid
        {
            if((curDataPtr[0]<<8|curDataPtr[1]) != 0)
            {
                if(pPatInfoForSearch->nServNum >= TS_MAX_SERVICE_NUM)
                {
                    eDbug("pat_serv_info array is full\n");
                    break;
                }
                pPatInfoForSearch->servInfoArray[pPatInfoForSearch->nServNum].nServId = curDataPtr[0]<<8|curDataPtr[1];
                pPatInfoForSearch->servInfoArray[pPatInfoForSearch->nServNum].pmtpid = (curDataPtr[2]&0x1f)<<8| curDataPtr[3];
                pPatInfoForSearch->nServNum++;
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
Function name: isdb_parse_sdt_section
Description:
    û�в�versionnum.
Parameters:
    sectionNum:����ʱ��ʾ��ǰ�������sdt���sectionnum,�������ֵ,��ʾ��һ���������sdt���sectionnum
Return:
    0:û��parse�ɹ�,������sdt��sectionû��parse��.
    1:sdt���ȫ����section��parse�����.
Time: 2012/6/17
*******************************************************************************/
static __s32 isdb_parse_sdt_section(SDTInfo *pSDTInfoForSearch, __u8 *section, __u32 *sectionNum)
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
        eDbug("cur sdt section nextIndecator == 0, return!\n");
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
            eDbug("malloc SDTServInfo fail!\n");
            return 1;   //�͵�������
        }
        pSDTServInfo->nServId = SI_MERGE_U16(&section[0]);
        pSDTServInfo->nFreeCAMode = (section[3]&0x10)>>4;

        despLen  = ((section[3]&0x0f)<<8) | section[4];
        sectionLen -= (5+despLen);
        if(sectionLen < 0)
        {
            eDbug("fatal error! SDT section loop parse error! check sdt section or code![%d][%d]\n", sectionLen, despLen);
            deleteSDTServInfo(pSDTServInfo);
            break;
        }
        //��ʼdescriptor loop����
        section += 5;
        while(despLen > 0)
        {
            isdb_parse_sdt_descriptor(pSDTServInfo, section);
            despLen -= (2+section[1]);
            section += (2+section[1]);
            if(despLen < 0)
            {
                eDbug("fatal error! SDT descriptor loop parse error! check sdt section or code!\n");
            }
        }

        tmpret = pSDTInfoForSearch->pList_SDTServInfo->AppendElem(pSDTInfoForSearch->pList_SDTServInfo, (void*)pSDTServInfo);
        if(tmpret!=EPDK_OK)
        {
            deleteSDTServInfo(pSDTServInfo);
            eDbug("AppendElem SDTServInfo fail!\n");
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
Function name: isdb_parse_nit_section_for_search
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
static __s32 isdb_parse_nit_section_for_search(NITInfo     *pNitInfoForSearch, __u8 *section, __u32 *sectionNum)
{
    __u32               nextIndicator   = 0;
    //__s32               sectionLen      = 0;
    //__s32               despLen         = 0;
    //__u16               serviceId       = 0;
    //__u16               tsId            = 0;
    __u16               network_descriptors_length;
    __u16               transport_stream_loop_length;
    __u16               transport_descriptors_length;
    __s32               nLeftLen;
    __s32               nLeftDescLoopLen;
    //__u16               orgNetworkId    = 0;
    //__s32               i               = 0;
    __u8                *orgDataPtr     = NULL;
    //maple_serv_item_t   *curServItem    = NULL;

    NITTSInfo           *pNITTSInfo = NULL;
    DList               *pNITTSInfoList; //������NITTSInfo
    __s32               tmpret;

    if(section[6] != (*sectionNum))     //step-1 check section number
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

    //sectionLen = (((section[1]&0x0f)<<8) | section[2]) - 12;
    section += 10;
    section += network_descriptors_length;  //����̨�龰��,���ڲ���network descriptors.
    transport_stream_loop_length = (section[0]&0x0f)<<8 | section[1];
    section += 2;
    nLeftLen = transport_stream_loop_length;
    pNITTSInfoList = pNitInfoForSearch->pList_NITTSInfo;
    while(nLeftLen > 0)
    {
        pNITTSInfo = newNITTSInfo();
        if(NULL == pNITTSInfo)
        {
            eDbug("malloc NITTSInfo fail!\n");
            return 1;   //�͵�������
        }
        pNITTSInfo->ts_id = SI_MERGE_U16(&section[0]);
        pNITTSInfo->orig_ts_id = SI_MERGE_U16(&section[2]);
        pNITTSInfo->nRemoteControlKeyId = DEFAULT_REMOTE_CONTROL_KEY_ID;
        nLeftDescLoopLen = transport_descriptors_length = section[4]&0x0f | section[5];

        nLeftLen -= (6+transport_descriptors_length);
        if(nLeftLen < 0)
        {
            eDbug("fatal error! nLeftLen[%d] wrong!\n");
            deleteNITTSInfo(pNITTSInfo);
            goto _exit_nit_section_parse;
        }

        section += 6;
        while(nLeftDescLoopLen > 0)
        {
            isdb_parse_nit_descriptor_loop2(pNITTSInfo, section);
            nLeftDescLoopLen -= (2+section[1]);
            section += (2+section[1]);
        }

        tmpret = pNITTSInfoList->AppendElem(pNITTSInfoList, (void*)pNITTSInfo);
        if(tmpret!=EPDK_OK)
        {
            deleteNITTSInfo(pNITTSInfo);
            eDbug("AppendElem NitTsInfo fail!\n");
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

static __s32 isdb_parse_tdt_section(__u8 *section, maple_time_t* pTdt, __s32 sectionLen)
{
    __u32           startTime;
    __u32           year;
    __u32           month;
    __u32           day;
    __u32           tmp;
    maple_weekday_t weekDay;

    startTime = 0;

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
    pTdt->hour      = isdb_bcd_to_int(startTime >>16);
    pTdt->min       = isdb_bcd_to_int((startTime & 0x00ff00)>>8);
    pTdt->sec       = isdb_bcd_to_int(startTime & 0x0000ff);

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

    return 1;
}


//******************************************************************************//
//******************************************************************************//
//*****************************************************************************//


__s32 isdb_pat_request_buffer(void *p_arg)
{
    __md_buf_t* pbuffer = NULL;

    pbuffer = ( __md_buf_t*) p_arg;

    if(g_isdbCtx->endPatSection == 1)      // has parsed pat section success
    {
        pbuffer->buf     =   NULL;
        pbuffer->bufSize =   0;
    }
    else  if((g_isdbCtx->isdbSectionBuf[PAT_BUF_IDX]==NULL)||(g_isdbCtx->isBufFull[PAT_BUF_IDX]==1))
    {
        pbuffer->buf     =   NULL;
        pbuffer->bufSize =   0;
        __wrn("the pat section is used,.\n");
    }
    else
    {
        pbuffer->buf     =   g_isdbCtx->isdbSectionBuf[PAT_BUF_IDX];
        pbuffer->bufSize =   ISDB_SECTION_BUF_SIZE;
    }

    return 0;
}

__s32 isdb_pmt_request_buffer(void *p_arg)
{
    __md_buf_t* pbuffer = NULL;

    pbuffer = ( __md_buf_t*) p_arg;

    if(g_isdbCtx->endPmtSection == 1)      // has parsed pat section success
    {
        pbuffer->buf     =   NULL;
        pbuffer->bufSize =   0;
    }
    else if((g_isdbCtx->isdbSectionBuf[PMT_BUF_IDX]==NULL)||(g_isdbCtx->isBufFull[PMT_BUF_IDX]==1))
    {
        pbuffer->buf     =   NULL;
        pbuffer->bufSize =   0;
        __wrn("the pmt section is used,.\n");
    }
    else
    {
        pbuffer->buf     =   g_isdbCtx->isdbSectionBuf[PMT_BUF_IDX];
        pbuffer->bufSize =   ISDB_SECTION_BUF_SIZE;
    }

    
    return 0;
}

__s32 isdb_sdt_request_buffer(void *p_arg)
{
    __md_buf_t* pbuffer = NULL;

    pbuffer = ( __md_buf_t*) p_arg;

    eDbug("pbuffer->bufSize is %d \n",pbuffer->bufSize);

    if(g_isdbCtx->endSdtSection == 1)      // has parsed pat section success
    {
        pbuffer->buf     =   NULL;
        pbuffer->bufSize =   0;
    }
    else if((g_isdbCtx->isdbSectionBuf[SDT_BUF_IDX]==NULL)||(g_isdbCtx->isBufFull[SDT_BUF_IDX]==1))
    {
        pbuffer->buf     =   NULL;
        pbuffer->bufSize =   0;
        eDbug("the sdt section is used,.\n");
    }
    else
    {
        pbuffer->buf     =   g_isdbCtx->isdbSectionBuf[SDT_BUF_IDX];
        pbuffer->bufSize =   ISDB_SECTION_BUF_SIZE;
    }

    
    return 0;
}

__s32 isdb_nit_request_buffer(void *p_arg)
{
    __md_buf_t* pbuffer = NULL;

    pbuffer = (__md_buf_t*)p_arg;

    if(g_isdbCtx->endNitSection == 1)      // has parsed nit section success
    {
        pbuffer->buf     =   NULL;
        pbuffer->bufSize =   0;
    }
    else if((g_isdbCtx->isdbSectionBuf[NIT_BUF_IDX]==NULL)||(g_isdbCtx->isBufFull[NIT_BUF_IDX]==1))
    {
        pbuffer->buf     =   NULL;
        pbuffer->bufSize =   0;
        __wrn("the nit section is used. check!\n");
    }
    else
    {
        pbuffer->buf     =   g_isdbCtx->isdbSectionBuf[NIT_BUF_IDX];
        pbuffer->bufSize =   ISDB_SECTION_BUF_SIZE;
    }
    return 0;
}

__s32 isdb_tdt_request_buffer(void *p_arg)
{
    __md_buf_t* pbuffer = NULL;

    pbuffer = ( __md_buf_t*) p_arg;

    if((g_isdbCtx->isdbSectionBuf[TDT_BUF_IDX]==NULL)||(g_isdbCtx->isBufFull[TDT_BUF_IDX]==1))
    {
        pbuffer->buf     =   NULL;
        pbuffer->bufSize =   0;
        __wrn("the tdt section is used,.\n");
    }
    else
    {
        pbuffer->buf     =   g_isdbCtx->isdbSectionBuf[TDT_BUF_IDX];
        pbuffer->bufSize =   ISDB_SECTION_BUF_SIZE;
    }
    return 0;
}

__s32 isdb_heit_request_buffer(void *p_arg)
{
    __md_buf_t* pbuffer = NULL;

    pbuffer = ( __md_buf_t*) p_arg;

    if((g_isdbCtx->isdbSectionBuf[HEIT_BUF_IDX]==NULL)||(g_isdbCtx->isBufFull[HEIT_BUF_IDX]==1))
    {
        pbuffer->buf     =   NULL;
        pbuffer->bufSize =   0;
        __wrn("the heit section is used,.\n");
    }
    else
    {
        pbuffer->buf     =   g_isdbCtx->isdbSectionBuf[HEIT_BUF_IDX];
        pbuffer->bufSize =   ISDB_SECTION_BUF_SIZE;
    }

    return 0;
}

__s32 isdb_meit_request_buffer(void *p_arg)
{
    __md_buf_t* pbuffer = NULL;

    pbuffer = ( __md_buf_t*) p_arg;

    if((g_isdbCtx->isdbSectionBuf[MEIT_BUF_IDX]==NULL)||(g_isdbCtx->isBufFull[MEIT_BUF_IDX]==1))
    {
        pbuffer->buf     =   NULL;
        pbuffer->bufSize =   0;
        __wrn("the meit section is used,.\n");
    }
    else
    {
        pbuffer->buf     =   g_isdbCtx->isdbSectionBuf[MEIT_BUF_IDX];
        pbuffer->bufSize =   ISDB_SECTION_BUF_SIZE;
    }

    return 0;
}

__s32 isdb_leit_request_buffer(void *p_arg)
{
    __md_buf_t* pbuffer = NULL;

    pbuffer = ( __md_buf_t*) p_arg;

    if((g_isdbCtx->isdbSectionBuf[LEIT_BUF_IDX]==NULL)||(g_isdbCtx->isBufFull[LEIT_BUF_IDX]==1))
    {
        pbuffer->buf     =   NULL;
        pbuffer->bufSize =   0;
        __wrn("the leit section is used,.\n");
    }
    else
    {
        pbuffer->buf     =   g_isdbCtx->isdbSectionBuf[LEIT_BUF_IDX];
        pbuffer->bufSize =   ISDB_SECTION_BUF_SIZE;
    }

    return 0;
}

__s32 isdb_pat_update_data(void *p_arg)             // ��ȡ��section�����ݳ���
{
    __u32 *dataLen   = NULL;
    __u32 patDataLen = 0;

    dataLen = (__u32 *)p_arg;
    patDataLen = *dataLen;

    g_isdbCtx->isBufFull[PAT_BUF_IDX] = 1;

    if(patDataLen > 0)
    {
        if (Crc32Compute(0xffffffff, g_isdbCtx->isdbSectionBuf[PAT_BUF_IDX], patDataLen))
        {
            g_isdbCtx->isBufFull[PAT_BUF_IDX]  = 0;
            return -1;
        }
    }

    g_isdbCtx->endPatSection = isdb_parse_pat_section(g_isdbCtx->pPatInfoForSearch, g_isdbCtx->isdbSectionBuf[PAT_BUF_IDX], &g_isdbCtx->curPatSecNum);
    g_isdbCtx->isBufFull[PAT_BUF_IDX]  = 0;

    return 0;
}

__s32 isdb_pmt_update_data(void *p_arg)             // ��ȡ��section�����ݳ���
{

    __u32 pmtDataLen = 0;
    __u32 *dataLen = NULL;

    dataLen = (__u32 *)p_arg;
    pmtDataLen = *dataLen;

    g_isdbCtx->isBufFull[PMT_BUF_IDX] = 1;


    if(pmtDataLen > 0)
    {
        if (Crc32Compute(0xffffffff, g_isdbCtx->isdbSectionBuf[PMT_BUF_IDX], pmtDataLen))
        {
            g_isdbCtx->isBufFull[PMT_BUF_IDX]  = 0;
            return -1;
        }
    }

    g_isdbCtx->endPmtSection = isdb_parse_pmt_section(g_isdbCtx->pPmtInfoForSearch, g_isdbCtx->isdbSectionBuf[PMT_BUF_IDX], &g_isdbCtx->curPmtSecNum);
    g_isdbCtx->isBufFull[PMT_BUF_IDX] = 0;

    return 0;
}

__s32 isdb_sdt_update_data(void *p_arg)             // ��ȡ��section�����ݳ���,����size.
{
    __u32 sdtDataLen = 0;
    __u32 *dataLen = NULL;

    dataLen = (__u32 *)p_arg;
    sdtDataLen = *dataLen;

    g_isdbCtx->isBufFull[SDT_BUF_IDX] = 1;

	eDbug("sdtDataLen is %d \n",sdtDataLen);
	
    if(sdtDataLen > 0)
    {
        if (Crc32Compute(0xffffffff, g_isdbCtx->isdbSectionBuf[SDT_BUF_IDX], sdtDataLen))
        {
            g_isdbCtx->isBufFull[SDT_BUF_IDX]  = 0;
            return -1;
        }
    }

    g_isdbCtx->endSdtSection = isdb_parse_sdt_section(g_isdbCtx->pSdtInfoForSearch, g_isdbCtx->isdbSectionBuf[SDT_BUF_IDX], &g_isdbCtx->curSdtSecNum);
    g_isdbCtx->isBufFull[SDT_BUF_IDX]  = 0;

    return 0;
}

__s32 isdb_nit_update_data(void *p_arg)             // ��ȡ��section�����ݳ���, total size
{
    __u32 nitDataLen = 0;
    __u32 *dataLen = NULL;

    dataLen = (__u32 *)p_arg;
    nitDataLen = *dataLen;

    g_isdbCtx->isBufFull[NIT_BUF_IDX] = 1;

    if(nitDataLen > 0)
    {
        if (Crc32Compute(0xffffffff, g_isdbCtx->isdbSectionBuf[NIT_BUF_IDX], nitDataLen))
        {
            g_isdbCtx->isBufFull[NIT_BUF_IDX]  = 0;
            return -1;
        }
    }

    g_isdbCtx->endNitSection = isdb_parse_nit_section_for_search(g_isdbCtx->pNitInfoForSearch, g_isdbCtx->isdbSectionBuf[NIT_BUF_IDX], &g_isdbCtx->curNitSecNum);
    g_isdbCtx->isBufFull[NIT_BUF_IDX]  = 0;

    return 0;
}

__s32 isdb_tdt_update_data(void *p_arg)             // ��ȡ��section�����ݳ���
{
    __u32 tdtDataLen = 0;
    __u32 *dataLen = NULL;

    dataLen = (__u32 *)p_arg;
    tdtDataLen = *dataLen;

    g_isdbCtx->isBufFull[TDT_BUF_IDX] = 1;

    if(tdtDataLen > 0)
    {
        if (Crc32Compute(0xffffffff, g_isdbCtx->isdbSectionBuf[TDT_BUF_IDX], tdtDataLen))
        {
            eDbug("crc check error.\n");
            g_isdbCtx->isBufFull[TDT_BUF_IDX]  = 0;
            return -1;
        }
    }
    isdb_parse_tdt_section(g_isdbCtx->isdbSectionBuf[TDT_BUF_IDX], &g_isdbCtx->curStreamTime, tdtDataLen);
    g_isdbCtx->isBufFull[TDT_BUF_IDX] = 0;

    return 0;
}



__s32 isdb_heit_update_data(void *p_arg)             // ��ȡ��section�����ݳ���
{
    __u8  i           = 0;
    __u32 heitDataLen = 0;
    __u32 *dataLen = NULL;

    dataLen = (__u32 *)p_arg;
    heitDataLen = *dataLen;

    g_isdbCtx->isBufFull[HEIT_BUF_IDX] = 1;

    if(heitDataLen > 0)
    {
        if (Crc32Compute(0xffffffff, g_isdbCtx->isdbSectionBuf[HEIT_BUF_IDX], heitDataLen))
        {
            eDbug("crc check error.\n");
            g_isdbCtx->isBufFull[HEIT_BUF_IDX]  = 0;
            return -1;
        }
    }

    while(1)
    {
        if(isdb_get_mutex(g_isdbCtx->epgMutex) == 0)
            break;
        esKRNL_TimeDly(1);
        i++;
        if(i == 100)
        {
            g_isdbCtx->isBufFull[HEIT_BUF_IDX]  = 0;
            return -1;
        }
    }
    eit_pool_append_section(0, g_isdbCtx->isdbSectionBuf[HEIT_BUF_IDX], heitDataLen, eit_type_heit);
    g_isdbCtx->isBufFull[HEIT_BUF_IDX]  = 0;
    isdb_release_mutex(g_isdbCtx->epgMutex);
    return 0;
}

__s32 isdb_meit_update_data(void     *p_arg)             // ��ȡ��section�����ݳ���
{
    __u8  i = 0;
    __u32 meitDataLen = 0;
    __u32 *dataLen = NULL;

    dataLen = (__u32 *)p_arg;
    meitDataLen = *dataLen;

    g_isdbCtx->isBufFull[MEIT_BUF_IDX] = 1;

    if(meitDataLen > 0)
    {
        if (Crc32Compute(0xffffffff, g_isdbCtx->isdbSectionBuf[MEIT_BUF_IDX], meitDataLen))
        {
            eDbug("crc check error.\n");
            g_isdbCtx->isBufFull[MEIT_BUF_IDX]  = 0;
            return -1;
        }
    }

    while(1)
    {
        if(isdb_get_mutex(g_isdbCtx->epgMutex) == 0)
            break;
        esKRNL_TimeDly(1);
        i++;
        if(i == 100)
        {
            g_isdbCtx->isBufFull[MEIT_BUF_IDX]  = 0;
            return -1;
        }
    }
    eit_pool_append_section(0, g_isdbCtx->isdbSectionBuf[MEIT_BUF_IDX], meitDataLen, eit_type_meit);
    g_isdbCtx->isBufFull[MEIT_BUF_IDX]  = 0;
    isdb_release_mutex(g_isdbCtx->epgMutex);
    return 0;
}

__s32 isdb_leit_update_data(void *p_arg)             // ��ȡ��section�����ݳ���
{
    __u8  i           = 0;
    __u32 leitDataLen = 0;
    __u32 *dataLen = NULL;

    dataLen = (__u32 *)p_arg;
    leitDataLen = *dataLen;

    g_isdbCtx->isBufFull[LEIT_BUF_IDX] = 1;

    if(leitDataLen > 0)
    {
        if (Crc32Compute(0xffffffff, g_isdbCtx->isdbSectionBuf[LEIT_BUF_IDX], leitDataLen))
        {
            eDbug("crc check error.\n");
            g_isdbCtx->isBufFull[LEIT_BUF_IDX]  = 0;
            return -1;
        }
    }

    while(1)
    {
        if(isdb_get_mutex(g_isdbCtx->epgMutex) == 0)
            break;
        esKRNL_TimeDly(1);
        i++;
        if(i == 100)
        {
            g_isdbCtx->isBufFull[LEIT_BUF_IDX]  = 0;
            return -1;
        }
    }
    eit_pool_append_section(0, g_isdbCtx->isdbSectionBuf[LEIT_BUF_IDX], leitDataLen, eit_type_leit);
    g_isdbCtx->isBufFull[LEIT_BUF_IDX]  = 0;
    isdb_release_mutex(g_isdbCtx->epgMutex);
    return 0;
}


/*******************************************************************************
Function name: isdb_set_service_type
Description:
    1.����service����Ƶ������Ƶ������������serviceType.
Parameters:

Return:

Time: 2012/6/17
*******************************************************************************/
void isdb_set_service_type(void* p_arg)
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
        else
        {
            // add the new type later

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

    eDbug("pServItem->pmtId is 0x%x \n",pServItem->pmtId);
    
    if(pServItem->pmtId >= PMT_PID_FOR_ONESEG && pServItem->pmtId <= PMT_PID_FOR_ONESEG + 7)
    {
        pServItem->nOneSegFlag = 1;
    }
    else
    {
        pServItem->nOneSegFlag = 0;
    }
    pServItem->vdStreamNum = 0;
    pServItem->adStreamNum = 0;
    pServItem->subtitleStreamNum = 0;

    eDbug("pPmtInfoForSearch->nStreamNum is %d \n",pPmtInfoForSearch->nStreamNum);
    
    for(i=0; i<pPmtInfoForSearch->nStreamNum; i++)
    {
        switch(pPmtInfoForSearch->streamInfoArray[i].nStreamType)
        {
            case STREAM_TYPE_AUDIO_MPEG1:
            case STREAM_TYPE_AUDIO_MPEG2:
            {
                nStreamFlag = 1;
                nCodecType = MAPLE_AUD_TYPE_MPEG;
                break;
            }
            case STREAM_TYPE_AUDIO_AAC:
            case STREAM_TYPE_AUDIO_MPEG4:
            {
                nStreamFlag = 1;
                nCodecType = MAPLE_AUD_TYPE_AAC;
                break;
            }
            case STREAM_TYPE_AUDIO_AC3:
            case STREAM_TYPE_AUDIO_AC3_:
            case STREAM_TYPE_AUDIO_EAC3:
            case STREAM_TYPE_AUDIO_AC3_TRUEHD:
            {
                nStreamFlag = 1;
                nCodecType = MAPLE_AUD_TYPE_AC3;
                break;
            }
            case STREAM_TYPE_AUDIO_DTS:
            case STREAM_TYPE_AUDIO_DTS_:
            case STREAM_TYPE_AUDIO_HDMV_DTS:
            case STREAM_TYPE_AUDIO_DTS_HRA:
            case STREAM_TYPE_AUDIO_DTS_MA:
            {
                nStreamFlag = 1;
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
            case STREAM_TYPE_SUBTITLE_ARIB:
            {
                nStreamFlag = 2;
                nCodecType = MAPLE_SUBT_TYPE_ARIB;
                break;
            }
            default:
            {
                eDbug("unknown streamtype[0x%x]\n", pPmtInfoForSearch->streamInfoArray[i].nStreamType);
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
                    eDbug("vdStreamNum[%d] is full,ignore!\n", pServItem->vdStreamNum);
                    break;
                }
                pServItem->vdInfo[pServItem->vdStreamNum].pid = pPmtInfoForSearch->streamInfoArray[i].nStreamPid;
                pServItem->vdInfo[pServItem->vdStreamNum].vdCodeType = (maple_vd_codec_e)nCodecType;

                eDbug("pServItem->vdInfo[%d].pid is 0x%x \n",pServItem->vdStreamNum,pServItem->vdInfo[pServItem->vdStreamNum].pid);
                eDbug("pServItem->vdInfo[%d].vdCodeType is %d \n",pServItem->vdStreamNum,pServItem->vdInfo[pServItem->vdStreamNum].vdCodeType);
				eDbug("pServItem->vdStreamNum is %d \n",pServItem->vdStreamNum);
                pServItem->vdStreamNum++;

                break;
            }
            case 1: //audio
            {
                if(pServItem->adStreamNum >= MAPLE_MAX_AD_STREAM_NUM)
                {
                    eDbug("adStreamNum[%d] is full,ignore!\n", pServItem->adStreamNum);
                    break;
                }
                pServItem->adInfo[pServItem->adStreamNum].pid = pPmtInfoForSearch->streamInfoArray[i].nStreamPid;
                pServItem->adInfo[pServItem->adStreamNum].adCodeType = (maple_ad_codec_e)nCodecType;
                if(pPmtInfoForSearch->streamInfoArray[i].nStreamNameLen)
                {
                    safeStringCopy(pServItem->adInfo[pServItem->adStreamNum].adStreamName, MAPLE_MAX_NAME_LEN,
                                   pPmtInfoForSearch->streamInfoArray[i].StreamName, pPmtInfoForSearch->streamInfoArray[i].nStreamNameLen);
                }
                
                eDbug("pServItem->adInfo[%d].pid is 0x%x \n",pServItem->adStreamNum,pServItem->adInfo[pServItem->vdStreamNum].pid);
                eDbug("pServItem->adInfo[%d].adCodeType is %d \n",pServItem->adStreamNum,pServItem->adInfo[pServItem->vdStreamNum].adCodeType);
				eDbug("pServItem->adStreamNum is %d \n",pServItem->adStreamNum);
				eDbug("pServItem->adInfo[%d].adStreamName is %s \n",pServItem->adStreamNum,pServItem->adInfo[pServItem->adStreamNum].adStreamName);

                pServItem->adStreamNum++;

                
                break;
            }
            case 2: //subtitle
            {
                if(pServItem->subtitleStreamNum >= MAPLE_MAX_ST_STREAM_NUM)
                {
                    eDbug("subtitleStreamNum[%d] is full,ignore!\n", pServItem->subtitleStreamNum);
                    break;
                }
                pServItem->subtInfo[pServItem->subtitleStreamNum].subtPid = pPmtInfoForSearch->streamInfoArray[i].nStreamPid;
                pServItem->subtInfo[pServItem->subtitleStreamNum].subtCodeType = (maple_subt_codec_e)nCodecType;
                if(pPmtInfoForSearch->streamInfoArray[i].nStreamNameLen)
                {
                    safeStringCopy(pServItem->subtInfo[pServItem->subtitleStreamNum].subtName, MAPLE_MAX_NAME_LEN,
                                   pPmtInfoForSearch->streamInfoArray[i].StreamName, pPmtInfoForSearch->streamInfoArray[i].nStreamNameLen);
                }
                eDbug("pServItem->subtInfo[%d].subtPid is 0x%x \n",pServItem->subtitleStreamNum,pServItem->subtInfo[pServItem->subtitleStreamNum].subtPid);
                eDbug("pServItem->subtInfo[%d].subtCodeType is %d \n",pServItem->subtitleStreamNum,pServItem->subtInfo[pServItem->subtitleStreamNum].subtCodeType);
				eDbug("pServItem->subtitleStreamNum is %d \n",pServItem->subtitleStreamNum);
				eDbug("pServItem->subtInfo[%d].subtName is %s \n",pServItem->subtitleStreamNum,pServItem->subtInfo[pServItem->subtitleStreamNum].subtName);
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

        eDbug("when pmt parse, service type still unknow, Judge_Month service type from es streams numbers, servtype[%d]\n", pServItem->servType);
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
    maple_serv_item_t   *pServItem = NULL;
    //����service.
    for(i=0; i<pServList->servNum; i++)
    {
        if(pServList->pServItems[i].servId == pPmtInfoForSearch->nServId)
        {
            pServItem = &pServList->pServItems[i];
            break;
        }
    }
    if(NULL == pServItem)
    {
        eDbug("not find serviceid[0x%x]\n", pPmtInfoForSearch->nServId);
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
    2.pat��: pmt_pid, oneseg/fullseg��Ϣ,tsId
    3.pmt�����Ϣ,��Ҫ��дvideo/audio/subtitle pid, pcrPid, streamtype, streamname, pmtpid��oneseg/fullsegҲ��������,��Ϊ���治һ�����.���service type��û��,��ôҲ������ȷ��.˳��Ƚ�һ��.
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
        eDbug("servList is full!\n");
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

/*******************************************************************************
Function name: isdb_scan_task
Description:
    1.��Ե�Ƶ�������ĺ���
    2.servListָ��Ŀռ��ǵ����߷����,����ֻ�������������service��д��ȥ,���ǲ������maple�Լ�
    ��service�б�.����service�б�������Ľӿ�.
Parameters:

Return:

Time: 2012/3/15
*******************************************************************************/
void isdb_scan_task(void* arg)
{
    __u32               i = 0;
    __u32               j = 0;
    //__u32               maxDelayNum = 100;
    maple_demod_info_t  demodParam;
    __s32               nWaitSectionTime = 0;
    //maple_serv_item_t   *curServItem    = NULL;
    __u16               nRemoteControlKeyId;
    __u16               tsId = 0;
    __s32               nTmp;
    __s32               nIdx;
    NITTSInfo           *pNITTSInfo = NULL;
    __u32               servIndex  = 0;
    __s32               nServiceNum = 0;
    __s32               tmpret;

    __s32               nSdtVChanId = -1;
    __s32               nPatVChanId = -1;
    __s32               nPmtVChanId = -1;
    __s32               nNitVChanId = -1;

    servList = (maple_serv_list_t *)arg;

    //��ʼ��servList.
    servList->servNum       = 0;
    servList->curServIdx    = 0;
    servList->nServItemsCount = MAPLE_MAX_SERV_NUM;
    memset(servList->pServItems, 0, sizeof(maple_serv_item_t)*MAPLE_MAX_SERV_NUM);

    //* step 1, tune.
    demodParam.frequency = g_isdbCtx->frequency;
    demodParam.bandwidth = g_isdbCtx->bandwidth;

    if(isdb_tune(&demodParam) != 0)
    {
        goto exit_isdb_search_service_task;
    }

    isdb_get_tuner_param(&demodParam);

	eDbug("demodParam.bandwidth is %d \n",demodParam.bandwidth);


    //* 1. search sdt. ͨ��g_isdbCtx->endSdtSection�ж�sdt�Ƿ�ɹ��õ�������.
    g_isdbCtx->curSdtSecNum   = 0;
    g_isdbCtx->endSdtSection  = 0;
    g_isdbCtx->isBufFull[SDT_BUF_IDX] = 0;
    if(g_isdbCtx->pSdtInfoForSearch)
    {
        eDbug("fatal error! why g_isdbCtx->pSdtInfoForSearch[0x%x]!=NULL?\n", g_isdbCtx->pSdtInfoForSearch);
        deleteSDTInfo(g_isdbCtx->pSdtInfoForSearch);
        g_isdbCtx->pSdtInfoForSearch = NULL;
    }
    g_isdbCtx->pSdtInfoForSearch = newSDTInfo();
    if(NULL == g_isdbCtx->pSdtInfoForSearch)
    {
        eDbug("malloc PatInfo fail,skip nit search!\n");
        goto exit_isdb_search_service_task;
    }
    isdb_open_section_filter(SDT_BAT_PID,0,&nSdtVChanId);
    nWaitSectionTime = 0;
    while(g_isdbCtx->endSdtSection == 0)
    {
        if(isdb_check_task_delete_request(EXEC_prioself)==OS_TASK_DEL_REQ)        // time is overflow
        {
			eDbug("isdb_check_task_delete_request \n");
           //isdb_set_service_type((void*)0);
            isdb_close_section_filter(SDT_BAT_PID, nSdtVChanId);
            servList->servNum = 0;  //��Ϊ��;�˳�,���Ը�Ƶ���service������¼.
            goto exit_isdb_search_service_task;
        }
        esKRNL_TimeDly(10);
        nWaitSectionTime++;
        if(nWaitSectionTime * 100 > WAIT_SDT_TIMEOUT)
        {
            eDbug("wait sdt timeout! no sdt!\n");
            break;
        }
    }
    isdb_close_section_filter(SDT_BAT_PID, nSdtVChanId);
    //1.1 ����sdt��������д�������ͽ�����service�б�,���û��sdt�������.
    //��Ҫ��дserviceid,servicetype, servicename, isCAFree, tsId, orgNetworkId(��Ӧmaple_serv_item_t->networkId)
    if(g_isdbCtx->endSdtSection)    //���sdtû��ȫ������,����Ϊsdt��Ч.
    {
        nServiceNum = g_isdbCtx->pSdtInfoForSearch->pList_SDTServInfo->GetLength(g_isdbCtx->pSdtInfoForSearch->pList_SDTServInfo);

		eDbug("nServiceNum is %d \n",nServiceNum);
        if(nServiceNum > servList->nServItemsCount)
        {
            eDbug("service num[%d] > nServItemsCount[%d]\n", nServiceNum, servList->nServItemsCount);
        }
        //��һ��foreach()����,�����д����.˳�����.ĿǰֻҪtv, radio. teletext��ʱ��Ҫ
        tmpret = g_isdbCtx->pSdtInfoForSearch->pList_SDTServInfo->ForeachElem(g_isdbCtx->pSdtInfoForSearch->pList_SDTServInfo, DListDataVisitFunc_SDTAddService, servList);
        if(tmpret != DLIST_RET_OK)
        {
            eDbug("fatal error! foreach ret[%d], why != OK?\n", tmpret);
        }
        //��tsId, orgNetworkId��ֵ
        for(i=0; i<servList->servNum; i++)
        {
            servList->pServItems[i].tsId = g_isdbCtx->pSdtInfoForSearch->tsId;
            servList->pServItems[i].networkId = g_isdbCtx->pSdtInfoForSearch->orgNetworkId;

            eDbug("servList->pServItems[%d].tsId is 0x%x \n",i,servList->pServItems[i].tsId);
            eDbug("servList->pServItems[%d].networkId is 0x%x \n",i,servList->pServItems[i].networkId);
            eDbug("servList->pServItems[%d].servId is 0x%x \n",i,servList->pServItems[i].servId);
            
        }
    }
    if(g_isdbCtx->pSdtInfoForSearch)
    {
        deleteSDTInfo(g_isdbCtx->pSdtInfoForSearch);
        g_isdbCtx->pSdtInfoForSearch = NULL;
    }

    //2.����pat��,�����oneseg��Ƶ��,ֱ������
    g_isdbCtx->curPatSecNum      = 0;
    g_isdbCtx->endPatSection     = 0;
    g_isdbCtx->isBufFull[PAT_BUF_IDX] = 0;

    eDbug("demodParam.bandwidth is %d \n",demodParam.bandwidth);
    
    if(DEMOD_BANDWIDTH_1_SEG != demodParam.bandwidth)
    {
        //* search pat.
        if(g_isdbCtx->pPatInfoForSearch)
        {
            eDbug("fatal error! why g_isdbCtx->pPatInfoForSearch[0x%x]!=NULL?\n", g_isdbCtx->pPatInfoForSearch);
            deletePATInfo(g_isdbCtx->pPatInfoForSearch);
            g_isdbCtx->pPatInfoForSearch = NULL;
        }
        g_isdbCtx->pPatInfoForSearch = newPATInfo();
        if(NULL == g_isdbCtx->pPatInfoForSearch)
        {
            eDbug("malloc PatInfo fail,skip nit search!\n");
            servList->servNum = 0;
            goto exit_isdb_search_service_task;
        }
        isdb_open_section_filter(PAT_PID,0, &nPatVChanId);
        nWaitSectionTime = 0;
        while(g_isdbCtx->endPatSection == 0)
        {
            if(isdb_check_task_delete_request(EXEC_prioself) == OS_TASK_DEL_REQ)        // time is overflow
            {
                isdb_close_section_filter(PAT_PID, nPatVChanId);
                servList->servNum = 0;  //��;�˳�,��Ƶ���service������¼.
                goto exit_isdb_search_service_task;
            }
            esKRNL_TimeDly(1);
            nWaitSectionTime++;
            if(nWaitSectionTime*10 > WAIT_PAT_TIMEOUT)    //��ʱ
            {
                eDbug("search pat timeout!\n");
                break;
            }
        }
        isdb_close_section_filter(PAT_PID, nPatVChanId);
        if(g_isdbCtx->endPatSection)    //pat����Ѱ��ɾ���д.
        {
            //2.1 ��ʼ��дmaple��service�б�, ��Ҫ��дpmt_pid, oneseg/fullseg��Ϣ,tsId, ���sdt��ɹ�����,����sdt��Ϊ׼.
            //������pat��Ϊ׼.�����sdt��,tsId���sdt��Ƚ�,����sdt��Ϊ׼.

            eDbug("g_isdbCtx->pPatInfoForSearch->nServNum is %d \n",g_isdbCtx->pPatInfoForSearch->nServNum);
			eDbug("servList->servNum is %d \n",servList->servNum);
			
			if((servList->servNum > 0)/*&&(servList->servNum == g_isdbCtx->pPatInfoForSearch->nServNum)*/)   //˵��sdt���Ѿ���д��servList.pat����У������
            {
            	static __u32 valid_servNum = 0;
            	
                for(i=0; i<servList->servNum; i++)
                {
                    //Ѱ��servId
                    for(j=0; j<g_isdbCtx->pPatInfoForSearch->nServNum; j++)
                    {
                        if(servList->pServItems[i].servId == g_isdbCtx->pPatInfoForSearch->servInfoArray[j].nServId)
                        {
							servList->pServItems[valid_servNum].servId = g_isdbCtx->pPatInfoForSearch->servInfoArray[j].nServId;
							servList->pServItems[valid_servNum].pmtId = g_isdbCtx->pPatInfoForSearch->servInfoArray[j].pmtpid;
                            if(servList->pServItems[valid_servNum].pmtId >= PMT_PID_FOR_ONESEG && servList->pServItems[valid_servNum].pmtId <= PMT_PID_FOR_ONESEG + 7)
                            {
                                servList->pServItems[valid_servNum].nOneSegFlag = 1;
                            }
                            else
                            {
                                servList->pServItems[valid_servNum].nOneSegFlag = 0;
                            }
                            if(servList->pServItems[i].tsId != g_isdbCtx->pPatInfoForSearch->tsId)
                            {
                                eDbug("fatal error! sdt, pat is not same! tsId[0x%x][0x%x]\n", servList->pServItems[i].tsId, g_isdbCtx->pPatInfoForSearch->tsId);
                            }

		                    eDbug("valid_servNum is %d \n",valid_servNum);							
		                    eDbug("servList->pServItems[%d].servId is 0x%x \n",valid_servNum,servList->pServItems[valid_servNum].servId);
                            eDbug("servList->pServItems[%d].pmtId is 0x%x \n",valid_servNum,servList->pServItems[valid_servNum].pmtId);
                            eDbug("servList->pServItems[%d].tsId is 0x%x \n",valid_servNum,servList->pServItems[valid_servNum].tsId);
                            eDbug("servList->pServItems[%d].nOneSegFlag is %d \n",valid_servNum,servList->pServItems[valid_servNum].nOneSegFlag);

							valid_servNum ++;
                            
                            break;
                        }
                    }
                }
				servList->servNum = valid_servNum;
				valid_servNum = 0;
                
            }
            else    //û��sdt��,����sdt���������? ����patΪ׼
            {
        		//if(servList->servNum != g_isdbCtx->pPatInfoForSearch->nServNum)
				{
					servList->servNum = 0;
				}
                for(j=0; j<g_isdbCtx->pPatInfoForSearch->nServNum; j++)
                {
                    servList->pServItems[servList->servNum].servId = g_isdbCtx->pPatInfoForSearch->servInfoArray[j].nServId;
                    servList->pServItems[servList->servNum].servType = MAPLE_SERV_TYPE_UNKNOWN;
                    safeStringCopy(servList->pServItems[servList->servNum].servName, MAPLE_MAX_NAME_LEN, DEFAULT_SERVICE_NAME, strlen(DEFAULT_SERVICE_NAME));
                    servList->pServItems[servList->servNum].isCAFree = 1;
                    servList->pServItems[servList->servNum].tsId = g_isdbCtx->pPatInfoForSearch->tsId;
                    servList->pServItems[servList->servNum].pmtId = g_isdbCtx->pPatInfoForSearch->servInfoArray[j].pmtpid;
                    if(servList->pServItems[servList->servNum].pmtId >= PMT_PID_FOR_ONESEG && servList->pServItems[servList->servNum].pmtId <= PMT_PID_FOR_ONESEG + 7)
                    {
                        servList->pServItems[servList->servNum].nOneSegFlag = 1;
                    }
                    else
                    {
                        servList->pServItems[servList->servNum].nOneSegFlag = 0;
                    }

                    eDbug("servList->pServItems[%d].servId is 0x%x \n",j,servList->pServItems[servList->servNum].servId);
                    eDbug("servList->pServItems[%d].pmtId is 0x%x \n",j,servList->pServItems[servList->servNum].pmtId);
                    eDbug("servList->pServItems[%d].tsId is 0x%x \n",j,servList->pServItems[servList->servNum].tsId);
                    eDbug("servList->pServItems[%d].nOneSegFlag is %d \n",j,servList->pServItems[servList->servNum].nOneSegFlag);
                    eDbug("servList->pServItems[%d].servName is %s \n",j,servList->pServItems[servList->servNum].servName);

                    servList->servNum++;
                    if(servList->servNum >= servList->nServItemsCount)
                    {
                        eDbug("service is too much! [%d][%d][%d]\n", servList->servNum, servList->nServItemsCount, g_isdbCtx->pPatInfoForSearch->nServNum);
                        break;
                    }
                }
            }
        }
        if(g_isdbCtx->pPatInfoForSearch)
        {
            deletePATInfo(g_isdbCtx->pPatInfoForSearch);
            g_isdbCtx->pPatInfoForSearch = NULL;
        }
    }

    //3. ��ʼpmt�������
    //pmt�����Ϣ,��Ҫ��дvideo/audio/subtitle pid, pcrPid, streamtype, streamname, oneseg/fullsegҲ����һ��.���service type��û��,��ôҲ������ȷ��.˳��Ƚ�һ��.
    if(g_isdbCtx->pPmtInfoForSearch)
    {
        eDbug("fatal error! why g_isdbCtx->pPmtInfoForSearch[0x%x]!=NULL?\n", g_isdbCtx->pPmtInfoForSearch);
        deletePMTInfo(g_isdbCtx->pPmtInfoForSearch);
        g_isdbCtx->pPmtInfoForSearch = NULL;
    }
    if(g_isdbCtx->endPatSection) //�ѵ���pat��,��ʾservice�б��Ѿ�ȷ����service��������pmt_pid.��ô����service�б����Ŀ����pmt,pmt_pidΪ0��ʾû���ҵ�pmt_pid.��ô����������.
    {
        if(0 == servList->servNum)
        {
            eDbug("fatal error! sdt pat said no service??check!!\n");
        }
        //* search all pmt.
        servIndex = 0;
        while(servIndex < servList->servNum)
        {
            nWaitSectionTime = 0;
            g_isdbCtx->curPmtSecNum = 0;
            g_isdbCtx->isBufFull[PMT_BUF_IDX] = 0;
            g_isdbCtx->endPmtSection = 0;
            g_isdbCtx->pPmtInfoForSearch = newPMTInfo();
            if(NULL == g_isdbCtx->pPmtInfoForSearch)
            {
                eDbug("malloc PmtInfo fail,skip pmt search!\n");
                goto exit_isdb_search_service_task;
            }
            if(0 == servList->pServItems[servIndex].pmtId)
            {
                eDbug("why pat not contain this serviceid[0x%x]?\n", servList->pServItems[servIndex].servId);
                servIndex++;
                continue;
            }
            isdb_open_section_filter(servList->pServItems[servIndex].pmtId,servList->pServItems[servIndex].servId,&nPmtVChanId);
            while(g_isdbCtx->endPmtSection == 0)
            {
                if(isdb_check_task_delete_request(EXEC_prioself) == OS_TASK_DEL_REQ)
                {
                    isdb_close_section_filter(servList->pServItems[servIndex].pmtId, nPmtVChanId);
                    servList->servNum = 0;
                    goto exit_isdb_search_service_task;
                }
                esKRNL_TimeDly(10);
                nWaitSectionTime++;
                if(nWaitSectionTime*100 > WAIT_PMT_TIMEOUT)
                {
                    eDbug("pmtpid[0x%x] overtime!\n", servList->pServItems[servIndex].pmtId);
                    break;
                }
            }
            isdb_close_section_filter(servList->pServItems[servIndex].pmtId, nPmtVChanId);
            //����pmt������һ�ž���дһ��.����ط�Ҫ���.
            if(g_isdbCtx->endPmtSection)    //pmt�ѵ��˾���д
            {
                PmtSupplementServiceInfo(servList, g_isdbCtx->pPmtInfoForSearch, servList->pServItems[servIndex].pmtId);
            }
            if(g_isdbCtx->pPmtInfoForSearch)
            {
                deletePMTInfo(g_isdbCtx->pPmtInfoForSearch);
                g_isdbCtx->pPmtInfoForSearch = NULL;
            }
            servIndex++;
        }
        
        if(g_isdbCtx->pPmtInfoForSearch)
        {
            deletePMTInfo(g_isdbCtx->pPmtInfoForSearch);
            g_isdbCtx->pPmtInfoForSearch = NULL;
        }
    
    }
    else    //���û���ѵ�pat��,��ôֱ����pmt.��0x1fc8��ʼ,ע������ж��service��onesegƵ�ε����.û��pat��,Ҳ��������sdt���.�����ȿ�sdt���������˵.
    {
        //����Ѿ���service����Ϣ��,��ô����ÿ�ѵ�һ��pmt�ͺ�service�б�ȶ�,û�оͷ���.���û��service�б�Ϊ��.��ô���ѵ���pmtΪ׼
        __u32   pmt_pid;
        __s32   nServiceListValid;  //serivce�б��Ƿ���Ч�ı��λ,�����Ч,��ô�ѵ���pmt��service�б�Ϊ׼.�����Ч,��ô���ѵ���pmtΪ׼.
        if(servList->servNum > 0)
        {
            nServiceListValid = 1;
        }
        else
        {
            nServiceListValid = 0;
        }

        //* isdb-t 1 segment received, perhaps there is no pat, pmt pid = 0x1fc8.
        //* search pmt for 1-seg.
        pmt_pid = PMT_PID_FOR_ONESEG;
        for(i=0; i<8; i++, pmt_pid++)   //* from 0x1fc8 to 0x1fcf.
        {
            nWaitSectionTime = 0;
            g_isdbCtx->curPmtSecNum = 0;
            g_isdbCtx->isBufFull[PMT_BUF_IDX] = 0;
            g_isdbCtx->endPmtSection = 0;
            g_isdbCtx->pPmtInfoForSearch = newPMTInfo();
            if(NULL == g_isdbCtx->pPmtInfoForSearch)
            {
                eDbug("malloc PmtInfo fail,skip pmt search!\n");
                goto exit_isdb_search_service_task;
            }
            isdb_open_section_filter(pmt_pid,0, &nPmtVChanId);
            while(g_isdbCtx->endPmtSection == 0)
            {
                if(isdb_check_task_delete_request(EXEC_prioself) == OS_TASK_DEL_REQ)
                {
                    isdb_close_section_filter(pmt_pid, nPmtVChanId);
                    servList->servNum = 0;
                    goto exit_isdb_search_service_task;
                }
                esKRNL_TimeDly(10);
                nWaitSectionTime++;
                if(nWaitSectionTime*100 > WAIT_PMT_TIMEOUT)
                {
                    eDbug("pmtpid[0x%x] overtime!\n", pmt_pid);
                    break;
                }
            }
            isdb_close_section_filter(pmt_pid, nPmtVChanId);
            if(g_isdbCtx->endPmtSection)   //pmt��Ч,��ô����
            {
                if(nServiceListValid)   //service�б��Ѿ�ȷ����,��ô�ȶ�,Ȼ����д
                {
                    PmtSupplementServiceInfo(servList, g_isdbCtx->pPmtInfoForSearch, pmt_pid);
                }
                else    //û��service�б�,�ų�sdt���pat�����ݴ�������,һ����sdt���pat��û���ѵ���ɵ�.
                {
                    //��PMTInfo����Ϣ��д��ȥ,�����Ļ�,sdt��,pat��Ҫ�������,����ҲҪ����.
                    PmtAddNewServiceInfo(servList, g_isdbCtx->pPmtInfoForSearch, pmt_pid); //��PmtSupplementServiceInfo()��֮ͬ������������ȫ������һ��service,��PmtSupplementServiceInfo()�����ҵ��ٲ�����д.
                }
            }
            else
            {
                eDbug("not find pmt_pid[0x%x]\n", pmt_pid);
            }

            if(g_isdbCtx->pPmtInfoForSearch)
            {
                deletePMTInfo(g_isdbCtx->pPmtInfoForSearch);
                g_isdbCtx->pPmtInfoForSearch = NULL;
            }
        }
    }

    if(servList->servNum == 0)
    {
        eDbug("this freq[%d][%d] has no service?\n", demodParam.frequency, demodParam.bandwidth);
        goto exit_isdb_search_service_task;
    }
    //������һ��curServItem->demod_pm.
    for(i=0; i<servList->servNum; i++)
    {
        servList->pServItems[i].demod_pm.frequency = demodParam.frequency;
        servList->pServItems[i].demod_pm.bandwidth = demodParam.bandwidth;
    }

    //4. nit
    // search nit.��ǰ��NIT��Ŀ����Ϊ��TS information descriptor�е�remote control id. ����onsseg����Ϣ,��Partial reception descriptor��.
    //��Ҫ��дremote_control_id. ������в���δ����oneseg/fullseg��ǵ�service,��ô����nit������дoneseg/fullseg���.
    //��service��Ϣ��pmt_pid,Ϊ0��ʾδ����. һ����û���ѵ�pat���pmt����ɵ�.
    if(g_isdbCtx->pNitInfoForSearch)
    {
        eDbug("fatal error! why g_isdbCtx->pNitInfoForSearch[0x%x]!=NULL?\n", g_isdbCtx->pNitInfoForSearch);
        deleteNITInfo(g_isdbCtx->pNitInfoForSearch);
        g_isdbCtx->pNitInfoForSearch = NULL;
    }
    g_isdbCtx->curNitSecNum   = 0;
    g_isdbCtx->endNitSection  = 0;
    g_isdbCtx->isBufFull[NIT_BUF_IDX] = 0;

    //���ȿ���tsId��û��
    nRemoteControlKeyId = DEFAULT_REMOTE_CONTROL_KEY_ID;
    if(g_isdbCtx->endPatSection || g_isdbCtx->endSdtSection)
    {
        g_isdbCtx->pNitInfoForSearch = newNITInfo();
        if(NULL == g_isdbCtx->pNitInfoForSearch)
        {
            eDbug("malloc NitInfo fail,quit!\n");
            servList->servNum = 0;
            goto exit_isdb_search_service_task;
        }
        isdb_open_section_filter(NIT_PID,0, &nNitVChanId);
        nWaitSectionTime = 0;
        while(g_isdbCtx->endNitSection == 0)
        {
            if(isdb_check_task_delete_request(EXEC_prioself)==OS_TASK_DEL_REQ)        // time is overflow
            {
                isdb_close_section_filter(NIT_PID, nNitVChanId);
                servList->servNum = 0;
                goto exit_isdb_search_service_task;
            }
            esKRNL_TimeDly(1);
            nWaitSectionTime++;
            if(nWaitSectionTime * 10 > WAIT_NIT_TIMEOUT)
            {
                eDbug("wait nit timeout [%d]ms! no nit!\n", WAIT_NIT_TIMEOUT);
                break;
            }
        }
        isdb_close_section_filter(NIT_PID, nNitVChanId);

        //4.1��ʼ������дservice�б�,oneseg/fullseg�ı��,��ȷ��remotecontrolkeyid
        if(g_isdbCtx->endNitSection)
        {
            tsId = servList->pServItems[0].tsId;
            //����tsIdȥ��nit�ж�Ӧ��ts��Ϣ
            nTmp = g_isdbCtx->pNitInfoForSearch->pList_NITTSInfo->GetLength(g_isdbCtx->pNitInfoForSearch->pList_NITTSInfo);
            if(nTmp <= 0)
            {
                eDbug("nit has not ts info!\n");
                goto _nit_scan_exit;
            }
            nIdx = g_isdbCtx->pNitInfoForSearch->pList_NITTSInfo->FindElem(g_isdbCtx->pNitInfoForSearch->pList_NITTSInfo, DListDataCompareFunc_FindNITTSInfoByTsId, (void*)tsId);
            if(nIdx < 0 || nIdx >= nTmp)
            {
                eDbug("Impossible! not find tsinfo by tsId[0x%x], nIdx[%d], nSize[%d], check code!\n", tsId,nIdx, nTmp);
                goto _nit_scan_exit;
            }
            if(EPDK_OK != g_isdbCtx->pNitInfoForSearch->pList_NITTSInfo->GetElemByIndex(g_isdbCtx->pNitInfoForSearch->pList_NITTSInfo, nIdx, (void**)&pNITTSInfo))
            {
                eDbug("Impossible! Can't find elem [%d], check code!\n", nIdx);
                goto _nit_scan_exit;
            }
            nRemoteControlKeyId = pNITTSInfo->nRemoteControlKeyId;
            //������дoneseg/fullseg���,���pat��pmt����û���ѵ��Ļ�,����Ҫ��.
            for(i=0; i<servList->servNum; i++)
            {
                if(0 == servList->pServItems[i].pmtId)
                {
                    eDbug("servId[0x%x] pmtpid is not sure!\n", servList->pServItems[i].servId);
                    for(j=0; j<pNITTSInfo->nOneSegServNum; j++)
                    {
                        if(servList->pServItems[i].servId == pNITTSInfo->OneSegServIdArray[j])
                        {
                            servList->pServItems[i].nOneSegFlag = 1;
                            break;
                        }
                    }
                }
            }
        }
        if(g_isdbCtx->pNitInfoForSearch)
        {
            deleteNITInfo(g_isdbCtx->pNitInfoForSearch);
            g_isdbCtx->pNitInfoForSearch = NULL;
        }
    }
    else
    {
        eDbug("no pat,sdt, we don't know tsId, so search nit is not need!\n");
    }
_nit_scan_exit:
    //ͳһ��ֵremote_control_key_id
    for(i=0; i<servList->servNum; i++)
    {
        servList->pServItems[i].nRemoteControlKeyId = nRemoteControlKeyId;
    }

    //5.֪ͨapp, �����ʾ.
    for(i=0; i<servList->servNum; i++)
    {
        isdb_search_one_service_cb(&servList->pServItems[i]);
    }

exit_isdb_search_service_task:
    if(g_isdbCtx->pPmtInfoForSearch)
    {
        deletePMTInfo(g_isdbCtx->pPmtInfoForSearch);
        g_isdbCtx->pPmtInfoForSearch = NULL;
    }
    if(g_isdbCtx->pPatInfoForSearch)
    {
        deletePATInfo(g_isdbCtx->pPatInfoForSearch);
        g_isdbCtx->pPatInfoForSearch = NULL;
    }
    if(g_isdbCtx->pSdtInfoForSearch)
    {
        deleteSDTInfo(g_isdbCtx->pSdtInfoForSearch);
        g_isdbCtx->pSdtInfoForSearch = NULL;
    }
    if(g_isdbCtx->pNitInfoForSearch)
    {
        deleteNITInfo(g_isdbCtx->pNitInfoForSearch);
        g_isdbCtx->pNitInfoForSearch = NULL;
    }
//    eDbug("freq[%d] search done! wait 20s!\n", demodParam.frequency);
//    esKRNL_TimeDly(2000);
    isdb_search_service_finish_cb();
    g_isdbCtx->slSearchStopFlag = 1;
    isdb_delete_task(EXEC_prioself);
}

/*******************************************************************************
Function name: isdb_scan_by_serviceId
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
__s32 isdb_scan_by_serviceId(maple_serv_item_t* pServItem)
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

    if(isdb_tune(&demodParam) != 0)
    {
        ret = EPDK_FAIL;
        goto exit_isdb_search_service_by_serviceId;
    }
    //isdb_get_tuner_param(&demodParam);
    if(pServItem->pmtId)
    {
        eDbug("why need search servId[0x%x], pmtpid[0x%x]?\n", pServItem->servId, pServItem->pmtId);
    }

    //2.����pat��,�����oneseg��Ƶ��,ֱ������
    g_isdbCtx->curPatSecNum      = 0;
    g_isdbCtx->endPatSection     = 0;
    g_isdbCtx->isBufFull[PAT_BUF_IDX] = 0;
    if(DEMOD_BANDWIDTH_1_SEG != demodParam.bandwidth)
    {
        //* search pat.
        if(g_isdbCtx->pPatInfoForSearch)
        {
            eDbug("fatal error! why g_isdbCtx->pPatInfoForSearch[0x%x]!=NULL?\n", g_isdbCtx->pPatInfoForSearch);
            deletePATInfo(g_isdbCtx->pPatInfoForSearch);
            g_isdbCtx->pPatInfoForSearch = NULL;
        }
        g_isdbCtx->pPatInfoForSearch = newPATInfo();
        if(NULL == g_isdbCtx->pPatInfoForSearch)
        {
            eDbug("malloc PatInfo fail,skip nit search!\n");
            ret = EPDK_FAIL;
            goto exit_isdb_search_service_by_serviceId;
        }
        isdb_open_section_filter(PAT_PID,0, &nPatVChanId);
        nWaitSectionTime = 0;
        while(g_isdbCtx->endPatSection == 0)
        {
            if(isdb_check_task_delete_request(EXEC_prioself) == OS_TASK_DEL_REQ)        // time is overflow
            {
                isdb_close_section_filter(PAT_PID, nPatVChanId);
                ret = EPDK_FAIL;
                goto exit_isdb_search_service_by_serviceId;
            }
            esKRNL_TimeDly(1);
            nWaitSectionTime++;
            if(nWaitSectionTime*10 > WAIT_PAT_TIMEOUT)    //��ʱ
            {
                eDbug("search pat timeout!\n");
                break;
            }
        }
        isdb_close_section_filter(PAT_PID, nPatVChanId);
        if(g_isdbCtx->endPatSection)    //pat����Ѱ��ɾ���д.����ֻ��Ҫ������service.
        {
            //2.1 ��ʼ��дmaple��service�б�, ��Ҫ��дpmt_pid, oneseg/fullseg��Ϣ,tsId, ���sdt��ɹ�����,����sdt��Ϊ׼.
            //������pat��Ϊ׼.�����sdt��,tsId���sdt��Ƚ�,����sdt��Ϊ׼.
            //Ѱ��servId
            for(j=0; j<g_isdbCtx->pPatInfoForSearch->nServNum; j++)
            {
                if(pServItem->servId == g_isdbCtx->pPatInfoForSearch->servInfoArray[j].nServId)
                {
                    pServItem->pmtId = g_isdbCtx->pPatInfoForSearch->servInfoArray[j].pmtpid;
                    if(pServItem->pmtId >= PMT_PID_FOR_ONESEG && pServItem->pmtId <= PMT_PID_FOR_ONESEG + 7)
                    {
                        pServItem->nOneSegFlag = 1;
                    }
                    else
                    {
                        pServItem->nOneSegFlag = 0;
                    }
                    if(pServItem->tsId != g_isdbCtx->pPatInfoForSearch->tsId)
                    {
                        eDbug("why tsId is not same as pat tsId[0x%x][0x%x]\n", pServItem->tsId, g_isdbCtx->pPatInfoForSearch->tsId);
                        pServItem->tsId = g_isdbCtx->pPatInfoForSearch->tsId;
                    }
                    break;
                }
            }
            if(j == g_isdbCtx->pPatInfoForSearch->nServNum)
            {
                eDbug("this serviceId[0x%x] is not in pat!\n", pServItem->servId);
                ret = EPDK_FAIL;
                goto exit_isdb_search_service_by_serviceId;
            }
        }
        else    //���û���ѵ�pat��,�����˳�
        {
            ret = EPDK_FAIL;
            goto exit_isdb_search_service_by_serviceId;
        }
        if(g_isdbCtx->pPatInfoForSearch)
        {
            deletePATInfo(g_isdbCtx->pPatInfoForSearch);
            g_isdbCtx->pPatInfoForSearch = NULL;
        }
    }

    //3. ��ʼpmt�������
    //pmt�����Ϣ,��Ҫ��дvideo/audio/subtitle pid, pcrPid, streamtype, streamname, oneseg/fullsegҲ����һ��.���service type��û��,��ôҲ������ȷ��.˳��Ƚ�һ��.
    if(g_isdbCtx->pPmtInfoForSearch)
    {
        eDbug("fatal error! why g_isdbCtx->pPmtInfoForSearch[0x%x]!=NULL?\n", g_isdbCtx->pPmtInfoForSearch);
        deletePMTInfo(g_isdbCtx->pPmtInfoForSearch);
        g_isdbCtx->pPmtInfoForSearch = NULL;
    }
    if(pServItem->pmtId) //ȷ����pmt_pid.�Ϳ�ʼ��pmt
    {
        //* search all pmt.
        nWaitSectionTime = 0;
        g_isdbCtx->curPmtSecNum = 0;
        g_isdbCtx->isBufFull[PMT_BUF_IDX] = 0;
        g_isdbCtx->endPmtSection = 0;
        g_isdbCtx->pPmtInfoForSearch = newPMTInfo();
        if(NULL == g_isdbCtx->pPmtInfoForSearch)
        {
            eDbug("malloc PmtInfo fail,skip pmt search!\n");
            ret = EPDK_FAIL;
            goto exit_isdb_search_service_by_serviceId;
        }
        isdb_open_section_filter(pServItem->pmtId,0, &nPmtVChanId);
        while(g_isdbCtx->endPmtSection == 0)
        {
            if(isdb_check_task_delete_request(EXEC_prioself) == OS_TASK_DEL_REQ)
            {
                isdb_close_section_filter(pServItem->pmtId, nPmtVChanId);
                ret = EPDK_FAIL;
                goto exit_isdb_search_service_by_serviceId;
            }
            esKRNL_TimeDly(10);
            nWaitSectionTime++;
            if(nWaitSectionTime*100 > WAIT_PMT_TIMEOUT)
            {
                eDbug("pmtpid[0x%x] overtime!\n", pServItem->pmtId);
                break;
            }
        }
        isdb_close_section_filter(pServItem->pmtId, nPmtVChanId);
        //����pmt������һ�ž���дһ��.����ط�Ҫ���.
        if(g_isdbCtx->endPmtSection)    //pmt�ѵ��˾���д
        {
            PmtFillServItem(pServItem, g_isdbCtx->pPmtInfoForSearch, pServItem->pmtId);
        }
        if(g_isdbCtx->pPmtInfoForSearch)
        {
            deletePMTInfo(g_isdbCtx->pPmtInfoForSearch);
            g_isdbCtx->pPmtInfoForSearch = NULL;
        }
        ret = EPDK_OK;
    }
    else
    {
        eDbug("fatal error! check code!\n");
        ret = EPDK_FAIL;
    }

exit_isdb_search_service_by_serviceId:
    if(g_isdbCtx->pPmtInfoForSearch)
    {
        deletePMTInfo(g_isdbCtx->pPmtInfoForSearch);
        g_isdbCtx->pPmtInfoForSearch = NULL;
    }
    if(g_isdbCtx->pPatInfoForSearch)
    {
        deletePATInfo(g_isdbCtx->pPatInfoForSearch);
        g_isdbCtx->pPatInfoForSearch = NULL;
    }
//    eDbug("freq[%d] search done! wait 20s!\n", demodParam.frequency);
//    esKRNL_TimeDly(2000);
    return ret;
}

void isdb_time_monitor_task(void* arg)
{
    __s32   nTdtVChanId = -1;
    isdb_open_section_filter(TDT_TOT_PID,0, &nTdtVChanId);

    while(1)
    {
        if(isdb_check_task_delete_request(EXEC_prioself)==OS_TASK_DEL_REQ)        // time is overflow
            break;
        esKRNL_TimeDly(10);
    }

    isdb_close_section_filter(TDT_TOT_PID, nTdtVChanId);
    g_isdbCtx->timeMonitorStopFlag = 1;
    isdb_delete_task(EXEC_prioself);
}

void isdb_epg_monitor_task(void* arg)
{
    __s32   nHEitVChanId = -1;
    __s32   nMEitVChanId = -1;
    __s32   nLEitVChanId = -1;
    //* open EIT Section filter.
    isdb_open_section_filter(HEIT_PID,0, &nHEitVChanId);
    isdb_open_section_filter(MEIT_PID,0, &nMEitVChanId);
    isdb_open_section_filter(LEIT_PID,0, &nLEitVChanId);

    while (1)
    {
        if(isdb_check_task_delete_request(EXEC_prioself) == OS_TASK_DEL_REQ)        // time is overflow
            break;
        esKRNL_TimeDly(10);
    }

    //* close pat section filter.
    isdb_close_section_filter(HEIT_PID, nHEitVChanId);
    isdb_close_section_filter(MEIT_PID, nMEitVChanId);
    isdb_close_section_filter(LEIT_PID, nLEitVChanId);
    g_isdbCtx->epgMonitorStopFlag = 1;
    isdb_delete_task(EXEC_prioself);
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
        eDbug("pat info mallc fail\n");
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
        eDbug("pmt info mallc fail\n");
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
        eDbug("SDTServInfo mallc fail\n");
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
        eDbug("Service List is full!");
        return DLIST_RET_STOP;
    }
    if(0x01 == pSDTServInfo->nServType || 0x02 == pSDTServInfo->nServType || 0xC0 == pSDTServInfo->nServType || 0x00 == pSDTServInfo->nServType || 0x16 == pSDTServInfo->nServType) //tv,radio
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
            case 0x16:            
                pServList->pServItems[pServList->servNum].servType = MAPLE_SERV_TYPE_TV;
                break;
            case 0x02:
                pServList->pServItems[pServList->servNum].servType = MAPLE_SERV_TYPE_RADIO;
                break;
            case 0xC0:
            case 0x00:
                eDbug("service type == [0x%x]\n", pSDTServInfo->nServType);
                pServList->pServItems[pServList->servNum].servType = MAPLE_SERV_TYPE_UNKNOWN;
                break;
            default:
                pServList->pServItems[pServList->servNum].servType = MAPLE_SERV_TYPE_UNKNOWN;
                break;
        }
        pServList->servNum++;
    }
    else
    {
        eDbug("ignore servicetype[0x%x]\n", pSDTServInfo->nServType);
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
        eDbug("SDTServInfo Dlist create fail\n");
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
        eDbug("sdt info mallc fail\n");
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
    if(NULL == pNITTSInfo)
    {
        eDbug("NITTSInfo mallc fail\n");
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
    deleteNITTSInfo(pNITTSInfo);
}

__s32 NITInfo_Init(NITInfo *thiz)
{
    memset(thiz, 0, sizeof(NITInfo));
    thiz->pList_NITTSInfo = newDList(DListDataDestroyFunc_NITTSInfo, NULL);
    if(NULL == thiz->pList_NITTSInfo)
    {
        eDbug("NITTSInfo Dlist create fail\n");
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
        eDbug("nitinfo mallc fail\n");
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


#if 0   //�����Ĵ���
static void isdb_parse_sdt_descriptor0(maple_serv_item_t* curServPtem, __u8 *sectionPtr)
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
            memset(curServPtem->servName, 0, MAPLE_MAX_NAME_LEN);
            if(sectionPtr[2] == 0x01)
                curServPtem->servType = MAPLE_SERV_TYPE_TV;
            else if(sectionPtr[2] == 0x02)
                curServPtem->servType = MAPLE_SERV_TYPE_RADIO;
            else if(sectionPtr[2] == 0x03)
                curServPtem->servType = MAPLE_SERV_TYPE_TELETEXT;
            else
                curServPtem->servType = MAPLE_SERV_TYPE_UNKNOWN;

            servProviderLen = sectionPtr[3];
            sectionPtr += 4+servProviderLen;
            if(sectionPtr[0] > 0)
                isdb_convert_str_to_utf8(sectionPtr+1, sectionPtr[0], curServPtem->servName, MAPLE_MAX_NAME_LEN, 0);
            break;
        case TELEPHONE_DESCRIPTOR_TAG:                  //�绰������
            break;
        case TIME_SHIFTED_SERVICE_DESCRIPTOR_TAG:       //ʱ��ƽ��ҵ��������
            break;
        default:
            break;
    }
}

static __s32 isdb_parse_pmt_section0(maple_serv_list_t *servList, __u8 *section, __u32 servIndex)
{
    __s32 sectionLen                = 0;
    __s32 esInfoLen                 = 0;
    __u32 pgmInfoLen                = 0;
    __u32 streamPid                 = 0;
    __s32 streamType                = 0;
    __u32 despLen                   = 0;
    maple_serv_item_t* curServItem  = NULL;

    curServItem                     = &(servList->pServItems[servIndex]);
    curServItem->hasPcr             = 0;
    curServItem->adStreamNum        = 0;
    curServItem->subtitleStreamNum  = 0;

    if((section[5] & 0x1) == 0)        // ��ǰpmt ������
        return 0;

    curServItem->servId = (section[3]<<8) | section[4];
    curServItem->pcrPid = ((section[8]&0x1f)<<8) | section[9];
    if(servList->pServItems[servIndex].pcrPid != 0x1fff)
        curServItem->hasPcr = 1;
    else
        curServItem->hasPcr = 0;

    sectionLen  =   ((section[1]&0x0f)<<8)  | section[2];
    pgmInfoLen  =   ((section[10]&0x0f)<<8) | section[11];
    section     +=  (12 + pgmInfoLen);
    sectionLen  -=  (13 + pgmInfoLen);

    while(sectionLen > 0)
    {
        streamType  = section[0];
        streamPid   = ((section[1]&0x1f)<<8) | section[2];
        esInfoLen   = ((section[3]&0x0f)<<8) | section[4];
        sectionLen -= (5+esInfoLen);
        section    += 5;

        do
        {
            //* despLen: current descriptor length
            despLen      = isdb_parse_pmt_descriptor(curServItem, section, streamType, streamPid,esInfoLen);
            section     += despLen;
            esInfoLen   -= despLen;
        }
        while(esInfoLen > 0);

        if(curServItem->subtInfo[curServItem->subtitleStreamNum].subtPid == streamPid)
            curServItem->subtitleStreamNum ++;
        else if(curServItem->adInfo[curServItem->adStreamNum].pid == streamPid)
            curServItem->adStreamNum ++;
        else if(curServItem->vdInfo[curServItem->vdStreamNum].pid == streamPid)
        {
            curServItem->vdInfo[curServItem->vdStreamNum].frameRate = 0;
            curServItem->vdStreamNum ++;
        }
    }
    if(curServItem->pmtId >= PMT_PID_FOR_ONESEG && curServItem->pmtId <= PMT_PID_FOR_ONESEG + 7)
    {
        curServItem->nOneSegFlag = 1;
        eDbug("detect OneSeg service, pmtpid[0x%x]\n", curServItem->pmtId);
    }
    else
    {
        curServItem->nOneSegFlag = 0;
    }
    return 1;
}

static __s32 isdb_parse_pat_section0(maple_serv_list_t *servList, PATInfo *pPatInfoForSearch, __u8 *section, __u32* sectionNum)
{
    __u32 sectionLen = 0;
    __u32 prgmNum    = 0;
    __u32 servIndex  = 0;
    __u32 i          = 0;
    __u8 *curDataPtr = NULL;
    __u32 tsId;

    if(section[6] != (*sectionNum))     //step-1 check section number
    {
        return 0;
    }
    else                                 //step-3 parse pat information
    {
        pPatInfoForSearch->tsId = tsId       = (section[3]<<8)|section[4];
        curDataPtr = section;
        sectionLen = ((section[1] & 0x0f)<<8) | section[2];
        prgmNum    = (sectionLen-9)>>2;
        curDataPtr = section + 8;
        for(i=0; i<prgmNum; i++)    //* ����PAT��ÿһ�� program_map_pid
        {
            if((curDataPtr[0]<<8|curDataPtr[1]) != 0)
            {
                servIndex = servList->servNum;
                servList->servNum++;
                memset(&servList->pServItems[servIndex], 0, sizeof(maple_serv_item_t));
                servList->pServItems[servIndex].tsId               = tsId;
                servList->pServItems[servIndex].servId             = curDataPtr[0]<<8|curDataPtr[1];
                servList->pServItems[servIndex].pmtId              = (curDataPtr[2]&0x1f)<<8| curDataPtr[3];
                servList->pServItems[servIndex].demod_pm.frequency = g_isdbCtx->frequency;
                servList->pServItems[servIndex].demod_pm.bandwidth = g_isdbCtx->bandwidth;
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

static __s32 isdb_parse_sdt_section0(maple_serv_list_t *servList, SDTInfo *pSDTInfoForSearch, __u8 *section, __u32 *sectionNum)
{
    __u32               nextIndicator   = 0;
    __s32               sectionLen      = 0;
    __s32               despLen         = 0;
    __u16               serviceId       = 0;
    __u16               tsId            = 0;
    __u16               orgNetworkId    = 0;
    __s32               i               = 0;
    __u8                *orgDataPtr     = NULL;
    maple_serv_item_t   *curServItem    = NULL;

    if(section[6] != (*sectionNum))     //step-1 check section number
    {
        return 0;
    }
    else if(section[0] != 0x42)     //0x42Ϊ��ǰts��service_descriptor_section
    {
        return 0;
    }
    else
    {
        orgDataPtr = section;
        nextIndicator = section[5] & 0x01;
        if(nextIndicator == 0)
            return 0;

        pSDTInfoForSearch->tsId = tsId         = section[3]<<8 | section[4];
        orgNetworkId = section[8]<<8 | section[9];

        sectionLen = (((section[1]&0x0f)<<8) | section[2]) - 12;
        section += 11;

        while(sectionLen > 0)
        {
            serviceId = (section[0]<<8) | section[1];
            i = 0;
            while(i < servList->servNum)
            {
                if((serviceId == servList->pServItems[i].servId) &&
                   (servList->pServItems[i].vdStreamNum > 0 ||      //* only parse services which have get its pmt.
                    servList->pServItems[i].adStreamNum > 0 ||      //* if pmt was parsed, the stream num should bigger than 0.
                    servList->pServItems[i].subtitleStreamNum > 0))
                {
                    servList->pServItems[i].tsId      = tsId;
                    servList->pServItems[i].networkId = orgNetworkId;
                    break;
                }
                i++;
            }

            despLen  = ((section[3]&0x0f)<<8) | section[4];
            sectionLen -= (5+despLen);
            if(sectionLen < 0)
                break;

            if(i == servList->servNum)
            {
                section += (5+despLen);
            }
            else
            {
                curServItem = &(servList->pServItems[i]);
                curServItem->isCAFree = 1-((section[3]&0x10)>>4);
                section += 5;
                while(despLen > 0)
                {
                    isdb_parse_sdt_descriptor(curServItem, section);
                    despLen -= (2+section[1]);
                    section += (2+section[1]);
                }

                //* sometimes we can not parse the service type correctly,
                //* here we jude service type with
                if(curServItem->servType == MAPLE_SERV_TYPE_UNKNOWN)
                {
                    eDbug("parse sdt section, service type unknow, Judge_Month service type from es streams numbers.");
                    if(curServItem->vdStreamNum > 0)
                        curServItem->servType = MAPLE_SERV_TYPE_TV;
                    else if(curServItem->adStreamNum > 0)
                        curServItem->servType = MAPLE_SERV_TYPE_RADIO;
                    else if(curServItem->subtitleStreamNum > 0)
                        curServItem->servType = MAPLE_SERV_TYPE_TELETEXT;
                }

                isdb_get_tuner_param(&curServItem->demod_pm);
                isdb_search_one_service_cb(curServItem);
            }
        }           // end while(sectionLen > 0)

        if((*sectionNum) != orgDataPtr[7])     // step-4 check the last section
        {
            (*sectionNum) ++;
            return 0;
        }
        return 1;
    }
}

void isdb_scan_task0(void* arg)
{
    __u32               i = 0;
    __u32               maxDelayNum = 100;
    maple_demod_info_t  demodParam;
    __s32               nWaitSectionTime = 0;
    maple_serv_item_t   *curServItem    = NULL;
    __u16               nRemoteControlKeyId;
    __u16               tsId = 0;
    __s32               nTmp;
    __s32               nIdx;
    NITTSInfo           *pNITTSInfo = NULL;
    __u32               servIndex  = 0;

    __u32     temp = 0;

    servList = (maple_serv_list_t *)arg;

    servList->servNum       = 0;

    memset(servList->pServItems, 0, sizeof(maple_serv_item_t)*MAPLE_MAX_SERV_NUM);

    //* step 1, tune.
    demodParam.frequency = g_isdbCtx->frequency;
    demodParam.bandwidth = g_isdbCtx->bandwidth;

    if(isdb_tune(&demodParam) != 0)
    {
        goto exit_isdb_search_service_task;
    }

    isdb_get_tuner_param(&demodParam);

    // step-2 parse pat section
    if(demodParam.bandwidth == DEMOD_BANDWIDTH_1_SEG)
    {
        __u32 pmt_pid;
        __u32 pmt_found;
        __u32 count;
        __u32 force_exit;

        //* isdb-t 1 segment received, perhaps there is no pat, pmt pid = 0x1fc8.
        //* search pmt for 1-seg.
        servList->curServIdx         = 0;
        g_isdbCtx->endPmtSection     = 0;
        g_isdbCtx->isBufFull[PMT_BUF_IDX] = 0;

        pmt_pid = PMT_PID_FOR_ONESEG;
        pmt_found = 0;
        force_exit = 0;
        servIndex = 0;
        for(i=0; i<8; i++, pmt_pid++)   //* from 0x1fc8 to 0x1fcf.
        {
            servList->pServItems[servIndex].pmtId = pmt_pid;
            isdb_open_section_filter(pmt_pid);

            for(count=0; count<300; count++)    //* maximum wait pmt for 3 seconds.
            {
                if(g_isdbCtx->endPmtSection != 0)
                {
                    servIndex++;
                    pmt_found = 1;
                    break;
                }

                if(isdb_check_task_delete_request(EXEC_prioself) == OS_TASK_DEL_REQ)        // time is overflow
                {
                    force_exit = 1;
                    break;
                }

                esKRNL_TimeDly(1);
            }

            isdb_close_section_filter(pmt_pid);
            if(force_exit == 1)
            {
                servList->servNum = 0;
                goto exit_isdb_search_service_task;
            }

            if(pmt_found == 1)
            {
                servList->servNum = 1;
                break;
            }
        }

        if(servList->servNum == 0)
            goto exit_isdb_search_service_task;

    }
    else
    {
        //* isdb-t three segment or full segment received, pat exits.
        //* search pat.
        g_isdbCtx->curPatSecNum      = 0;
        g_isdbCtx->endPatSection     = 0;
        g_isdbCtx->isBufFull[PAT_BUF_IDX] = 0;
        if(g_isdbCtx->pPatInfoForSearch)
        {
            eDbug("fatal error! why g_isdbCtx->pPatInfoForSearch[0x%x]!=NULL?\n", g_isdbCtx->pPatInfoForSearch);
            deletePATInfo(g_isdbCtx->pPatInfoForSearch);
            g_isdbCtx->pPatInfoForSearch = NULL;
        }
        g_isdbCtx->pPatInfoForSearch = newPATInfo();
        if(NULL == g_isdbCtx->pPatInfoForSearch)
        {
            eDbug("malloc PatInfo fail,skip nit search!\n");
            goto exit_isdb_search_service_task;
        }
        isdb_open_section_filter(PAT_PID);
        i = 0;
        while(g_isdbCtx->endPatSection == 0)
        {
            if(isdb_check_task_delete_request(EXEC_prioself) == OS_TASK_DEL_REQ)        // time is overflow
            {
                isdb_close_section_filter(PAT_PID);
                servList->servNum = 0;
                goto exit_isdb_search_service_task;
            }
            esKRNL_TimeDly(1);
            i++;
            if(i*10 > WAIT_PAT_TIMEOUT)    //��ʱ
            {
                eDbug(" WAIT_PAT_TIMEOUT***\n");

                deletePATInfo(g_isdbCtx->pPatInfoForSearch);
                g_isdbCtx->pPatInfoForSearch = NULL;
                break;
            }
        }
        isdb_close_section_filter(PAT_PID);

        if(servList->servNum == 0)  //һ��serviceû�ѵ�,��ô���ٵ���one segment����.����һ��.
        {
            __u32 pmt_pid;
            __u32 pmt_found;
            __u32 count;
            __u32 force_exit;

            demodParam.bandwidth = DEMOD_BANDWIDTH_1_SEG;
//           isdb_tune(&demodParam);    /*Bridge@Apr14--2012*/

            servList->curServIdx         = 0;

            g_isdbCtx->endPmtSection = 0;
            g_isdbCtx->isBufFull[PMT_BUF_IDX] = 0;

            pmt_pid = PMT_PID_FOR_ONESEG;
            pmt_found = 0;
            force_exit = 0;
            servIndex = 0;
#if 0
            for(i=0; i<8; i++, pmt_pid++)  //* from 0x1fc8 to 0x1fcf.,����oneseg,spec�涨һ��Ƶ��ֻ��һ��service,�����ѵ�һ���Ϳ����˳���.
            {
                servList->pServItems[servIndex].pmtId = pmt_pid;
                isdb_open_section_filter(pmt_pid);

                for(count=0; count<300; count++)    //* maximum wait pmt for 3 seconds.
                {
                    if(g_isdbCtx->endPmtSection != 0)
                    {
                        servIndex++;
                        pmt_found = 1;

                        break;
                    }

                    if(isdb_check_task_delete_request(EXEC_prioself) == OS_TASK_DEL_REQ)        // time is overflow
                    {
                        force_exit = 1;
                        break;
                    }

                    esKRNL_TimeDly(1);
                }

                isdb_close_section_filter(pmt_pid);
                if(force_exit == 1)
                {
                    servList->servNum = 0;
                    goto exit_isdb_search_service_task;
                }

                if(pmt_found == 1)
                {
                    servList->servNum = 1;
                    break;
                }
            }
#else
            /*Bridge@Jun06--2012*/
            for(i=0; i<2; i++, pmt_pid++)  /*0x1fc8, 0x1fc9*/
            {
                g_isdbCtx->isBufFull[PMT_BUF_IDX] = 0;
                g_isdbCtx->endPmtSection = 0;
                temp = 0;

                servList->pServItems[servList->curServIdx].pmtId = pmt_pid;

                isdb_open_section_filter(servList->pServItems[servList->curServIdx].pmtId);
                while(g_isdbCtx->endPmtSection == 0)
                {
                    if(isdb_check_task_delete_request(EXEC_prioself) == OS_TASK_DEL_REQ)
                    {
                        isdb_close_section_filter(servList->pServItems[servList->curServIdx].pmtId);
                        servList->servNum = 0;
                        goto exit_isdb_search_service_task;
                    }

                    esKRNL_TimeDly(1);
                    temp++;
                    if(temp*15 > WAIT_PMT_TIMEOUT)
                    {
                        eDbug(" only oneseg WAIT_PMT_TIMEOUT***\n");

                        isdb_close_section_filter(servList->pServItems[servList->curServIdx].pmtId);

                        break;
                    }
                }

                if(temp*15 <= WAIT_PMT_TIMEOUT)
                {
                    servList->curServIdx++;

                    servList->servNum++;
                }

            }

            if(servList->curServIdx >0)
                servList->curServIdx--;
#endif

            if(servList->servNum == 0)
                goto exit_isdb_search_service_task;
        }
        else
        {
            //* search all pmt.
            servList->curServIdx = 0;
            while(servList->curServIdx < servList->servNum)
            {
                i = 0;
                g_isdbCtx->isBufFull[PMT_BUF_IDX] = 0;
                g_isdbCtx->endPmtSection = 0;

                isdb_open_section_filter(servList->pServItems[servList->curServIdx].pmtId);
                while(g_isdbCtx->endPmtSection == 0)
                {
                    if(isdb_check_task_delete_request(EXEC_prioself) == OS_TASK_DEL_REQ)
                    {
                        isdb_close_section_filter(servList->pServItems[servList->curServIdx].pmtId);
                        servList->servNum = 0;
                        goto exit_isdb_search_service_task;
                    }
                    esKRNL_TimeDly(1);
                    i++;
                    if(i*10 > WAIT_PMT_TIMEOUT)
                    {
                        eDbug(" WAIT_PMT_TIMEOUT***\n");

                        break;
                    }
                }

                isdb_close_section_filter(servList->pServItems[servList->curServIdx].pmtId);
                servList->curServIdx++;
            }
        }
    }

    //* search sdt.
    g_isdbCtx->curSdtSecNum   = 0;
    g_isdbCtx->endSdtSection  = 0;
    g_isdbCtx->isBufFull[SDT_BUF_IDX] = 0;
    if(g_isdbCtx->pSdtInfoForSearch)
    {
        eDbug("fatal error! why g_isdbCtx->pSdtInfoForSearch[0x%x]!=NULL?\n", g_isdbCtx->pSdtInfoForSearch);
        deleteSDTInfo(g_isdbCtx->pSdtInfoForSearch);
        g_isdbCtx->pSdtInfoForSearch = NULL;
    }
    g_isdbCtx->pSdtInfoForSearch = newSDTInfo();
    if(NULL == g_isdbCtx->pSdtInfoForSearch)
    {
        eDbug("malloc PatInfo fail,skip nit search!\n");
        goto exit_isdb_search_service_task;
    }
    isdb_open_section_filter(SDT_BAT_PID);
    nWaitSectionTime = 0;
    while(g_isdbCtx->endSdtSection == 0)
    {
        if(isdb_check_task_delete_request(EXEC_prioself)==OS_TASK_DEL_REQ)        // time is overflow
        {
            isdb_set_service_type((void*)0);
            isdb_close_section_filter(SDT_BAT_PID);
            goto exit_isdb_search_service_task;
        }
        esKRNL_TimeDly(1);
        nWaitSectionTime++;
        if(nWaitSectionTime * 10 > WAIT_SDT_TIMEOUT)
        {
            eDbug("wait sdt timeout! no sdt!\n");
            deleteSDTInfo(g_isdbCtx->pSdtInfoForSearch);
            g_isdbCtx->pSdtInfoForSearch = NULL;
            break;
        }
    }
    isdb_close_section_filter(SDT_BAT_PID);

    //* search nit.��ǰ��NIT��Ŀ����Ϊ��TS information descriptor�е�remote control id.
    g_isdbCtx->curNitSecNum   = 0;
    g_isdbCtx->endNitSection  = 0;
    g_isdbCtx->isBufFull[NIT_BUF_IDX] = 0;
    if(g_isdbCtx->pNitInfoForSearch)
    {
        eDbug("fatal error! why g_isdbCtx->pNitInfoForSearch[0x%x]!=NULL?\n", g_isdbCtx->pNitInfoForSearch);
        deleteNITInfo(g_isdbCtx->pNitInfoForSearch);
        g_isdbCtx->pNitInfoForSearch = NULL;
    }
    g_isdbCtx->pNitInfoForSearch = newNITInfo();
    if(NULL == g_isdbCtx->pNitInfoForSearch)
    {
        eDbug("malloc NitInfo fail,skip nit search!\n");
        goto _skip_nit_search;
    }
    isdb_open_section_filter(NIT_PID);
    nWaitSectionTime = 0;
    while(g_isdbCtx->endNitSection == 0)
    {
        if(isdb_check_task_delete_request(EXEC_prioself)==OS_TASK_DEL_REQ)        // time is overflow
        {
            isdb_close_section_filter(NIT_PID);
            goto exit_isdb_search_service_task;
        }
        esKRNL_TimeDly(1);
        nWaitSectionTime++;
        if(nWaitSectionTime * 10 > WAIT_NIT_TIMEOUT)
        {
            eDbug("wait nit timeout [%d]ms! no nit!\n", WAIT_NIT_TIMEOUT);
            deleteNITInfo(g_isdbCtx->pNitInfoForSearch);
            g_isdbCtx->pNitInfoForSearch = NULL;
            break;
        }
    }
    isdb_close_section_filter(NIT_PID);

_skip_nit_search:
    //���û���ҵ�nit,������������service��remote control_key_idΪ0xff
    if(0 == g_isdbCtx->endNitSection)
    {
        for(i = 0; i < servList->servNum; i++)
        {
            curServItem = &(servList->pServItems[i]);
            curServItem->nRemoteControlKeyId = DEFAULT_REMOTE_CONTROL_KEY_ID;
        }
    }

    //���û���ҵ�sdt,������������service��service_type��service_name
    if(0 == g_isdbCtx->endSdtSection)
    {
        //* sometimes we can not parse the service type correctly,
        //* here we jude service type with
        for(i = 0; i < servList->servNum; i++)
        {
            curServItem = &(servList->pServItems[i]);
            if(curServItem->vdStreamNum > 0)
                curServItem->servType = MAPLE_SERV_TYPE_TV;
            else if(curServItem->adStreamNum > 0)
                curServItem->servType = MAPLE_SERV_TYPE_RADIO;
            else if(curServItem->subtitleStreamNum > 0)
                curServItem->servType = MAPLE_SERV_TYPE_TELETEXT;
            else
            {
                eDbug("fatal error! no sdt, is this a service???\n");
            }
            safeStringCopy(curServItem->servName, MAPLE_MAX_NAME_LEN, DEFAULT_SERVICE_NAME, strlen(DEFAULT_SERVICE_NAME));
            isdb_get_tuner_param(&curServItem->demod_pm);
            isdb_search_one_service_cb(curServItem);
        }
    }

    //ͳһ��ֵremote_control_key_id
    nRemoteControlKeyId = DEFAULT_REMOTE_CONTROL_KEY_ID;
    if(g_isdbCtx->pNitInfoForSearch)
    {
        if(g_isdbCtx->pPatInfoForSearch)
        {
            tsId = g_isdbCtx->pPatInfoForSearch->tsId;
        }
        else if(g_isdbCtx->pSdtInfoForSearch)
        {
            tsId = g_isdbCtx->pSdtInfoForSearch->tsId;
        }
        else
        {
            eDbug("no pat and sdt, can't decide cur_tsId!\n");
            goto _skip_get_remote_control_key_id;
        }

        //����tsIdȥ��nit�ж�Ӧ��ts��Ϣ
        nTmp = g_isdbCtx->pNitInfoForSearch->pList_NITTSInfo->GetLength(g_isdbCtx->pNitInfoForSearch->pList_NITTSInfo);
        if(nTmp <= 0)
        {
            eDbug("nit has not ts info!\n");
            goto _skip_get_remote_control_key_id;
        }
        nIdx = g_isdbCtx->pNitInfoForSearch->pList_NITTSInfo->FindElem(g_isdbCtx->pNitInfoForSearch->pList_NITTSInfo, DListDataCompareFunc_FindNITTSInfoByTsId, (void*)tsId);
        if(nIdx < 0 || nIdx >= nTmp)
        {
            eDbug("Impossible! not find tsinfo by tsId[0x%x], nIdx[%d], nSize[%d], check code!\n", tsId,nIdx, nTmp);
            goto _skip_get_remote_control_key_id;
        }
        if(EPDK_OK != g_isdbCtx->pNitInfoForSearch->pList_NITTSInfo->GetElemByIndex(g_isdbCtx->pNitInfoForSearch->pList_NITTSInfo, nIdx, (void**)&pNITTSInfo))
        {
            eDbug("Impossible! Can't find elem [%d], check code!\n", nIdx);
            goto _skip_get_remote_control_key_id;
        }
        nRemoteControlKeyId = pNITTSInfo->nRemoteControlKeyId;
    }
_skip_get_remote_control_key_id:
    for(i = 0; i < servList->servNum; i++)
    {
        curServItem = &(servList->pServItems[i]);
        curServItem->nRemoteControlKeyId = nRemoteControlKeyId;
    }

exit_isdb_search_service_task:
    if(g_isdbCtx->pPatInfoForSearch)
    {
        deletePATInfo(g_isdbCtx->pPatInfoForSearch);
        g_isdbCtx->pPatInfoForSearch = NULL;
    }
    if(g_isdbCtx->pSdtInfoForSearch)
    {
        deleteSDTInfo(g_isdbCtx->pSdtInfoForSearch);
        g_isdbCtx->pSdtInfoForSearch = NULL;
    }
    if(g_isdbCtx->pNitInfoForSearch)
    {
        deleteNITInfo(g_isdbCtx->pNitInfoForSearch);
        g_isdbCtx->pNitInfoForSearch = NULL;
    }
//    eDbug("freq[%d] search done! wait 20s!\n", demodParam.frequency);
//    esKRNL_TimeDly(2000);
    isdb_search_service_finish_cb();
    g_isdbCtx->slSearchStopFlag = 1;
    isdb_delete_task(EXEC_prioself);
}
#endif

