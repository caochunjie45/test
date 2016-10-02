
#include "isdb_eit_parser.h"

//* the entry pointer of pf/sch eit info
eit_tp_t* gEitTpHeader = NULL;
extern __s32 safeStringCopy(__u8* dst, __u32 dstSize, __u8* src, __u32 srcLen);

static __s32 eit_parser_clean_sht_desc_list(eit_event_info_t* eitEventInfo);
static __s32 eit_parser_clean_ext_desc_list(eit_event_info_t *eitEventInfo);
static __s32 eit_parser_clean_desc_list(eit_event_info_t* eitEventInfo);
static __s32 eit_parser_clean_event_list(eit_event_info_t *header);
//static __s32 eit_clean_all_event(eit_tp_t* tpHeader);
static __s32 eit_parser_clean_all_service(eit_tp_t* tpHeader);
static __s32 eit_parser_clean_all(eit_tp_t* tpHeader);

static eit_tp_t* eit_parser_get_tp(eit_tp_t* header, __u32 origNetworkId, __u16 tsId);
static eit_tp_t* eit_parser_add_new_tp(eit_tp_t** tpHeader, __u16 tsId, __u32 origNetworkId);
static eit_service_info_t* eit_parser_get_service(eit_tp_t* tpt, __u16 serviceId);
static eit_service_info_t* eit_parser_add_new_service(eit_service_info_t** serviceHeader, __u16 serviceId);
static eit_event_info_t* eit_parser_add_new_event(eit_event_info_t** eventHeader, eit_date_time_t* dt, __s32 event_type);
static __s32 eit_parser_get_event_info(eit_event_info_t** eitEventInfoHeader, __u8* eventBuf, __s32 eventLen, __s32 eventType);

static __s32 eit_parser_get_sht_desc(eit_short_desc_t** eitShrtDesc, __u8* descBuf);
static __s32 eit_parser_get_content_desc(eit_content_desc_t** eitContentDesc, __u8* descBuf);
static __s32 eit_parser_get_pr_desc(eit_pr_desc_t** eitPrDesc, __u8* descBuf);

#ifdef PARSING_EXTRA_DESC_EN
__s32 eit_parser_get_component_desc(eit_component_desc_t** eitComponentDesc, __u8* descBuf);
static __s32 eit_parser_get_ca_desc(eit_ca_desc_t** eitCaDesc, __u8* descBuf);
static __s32 eit_parser_get_tele_desc(eit_tele_desc_t** eitTeleDesc, __u8* descBuf);
#endif

static void bcd_to_hms(__u8* time, __u8* hour, __u8* min, __u8* sec);
static void mjd_to_ymd(__u32 mjd, __u16* year, __u8* month, __u8* day, __u8* weekday);
static __s32 eit_compare_time(eit_date_time_t *d1, eit_date_time_t *d2);
static void eit_parser_get_start_end_time(__u16 mjdNum, __u8* time, eit_date_time_t* startTime, __u32* duration, eit_date_time_t* endTime);


//******************************** SEPERATOR ********************************//
//************************* local function prototype ************************//
//***************************************************************************//
static __s32 eit_parser_clean_sht_desc_list(eit_event_info_t* eitEventInfo)
{
    eit_short_desc_t *dp;
    eit_short_desc_t *pt;

    if (eitEventInfo == NULL)
        return -1;

    pt = NULL;
    dp = eitEventInfo->eitShtDesc;

    while(dp != NULL)
    {
        pt = dp->next;
        if (dp->eventName != NULL)
            free(dp->eventName);

        if (dp->textDesc != NULL)
            free(dp->textDesc);

        free(dp);
        dp = pt;
    }

    eitEventInfo->eitShtDesc = NULL;

    return 0;
}


static __s32 eit_parser_clean_ext_desc_list(eit_event_info_t* eitEventInfo)
{
    eit_ext_desc_t *dp;
    eit_ext_desc_t *pt;

    if (NULL == eitEventInfo)
        return -1;

    pt = NULL;
    dp = eitEventInfo->eitExtDesc;

    while (dp != NULL)
    {
        pt = dp->next;
        if (dp->textChar != NULL)
            free(dp->textChar);

        free(dp);

        dp = pt;
    }

    eitEventInfo->eitExtDesc = NULL;

    return 0;
}


static __s32 eit_parser_clean_desc_list(eit_event_info_t* eitEventInfo)
{
    void* pt;

    if (eitEventInfo == NULL)
        return 0;

    while (eitEventInfo->eitPrdesc != NULL)
    {
        pt = (eit_pr_desc_t*)eitEventInfo->eitPrdesc->next;
        free(eitEventInfo->eitPrdesc);
        eitEventInfo->eitPrdesc=pt;
    }

    while (eitEventInfo->eitContentDesc != NULL)
    {
        pt = (eit_content_desc_t*)eitEventInfo->eitContentDesc->next;
        free(eitEventInfo->eitContentDesc);
        eitEventInfo->eitContentDesc = pt;
    }


#ifdef PARSING_EXTRA_DESC_EN
    while (eitEventInfo->eitTeleDesc != NULL)
    {
        pt = (eit_tele_desc_t*)eitEventInfo->eitTeleDesc->next;
        free(eitEventInfo->eitTeleDesc);
        eitEventInfo->eitTeleDesc = pt;
    }

    while (eitEventInfo->eitCaDesc != NULL)
    {
        pt = (eit_ca_desc_t*)eitEventInfo->eitCaDesc->next;
        free(eitEventInfo->eitCaDesc);
        eitEventInfo->eitCaDesc = pt;
    }
    while (eitEventInfo->eitComponentDesc != NULL)
    {
        pt = (eit_component_desc_t*)eitEventInfo->eitComponentDesc->next;
        free(eitEventInfo->eitComponentDesc);
        eitEventInfo->eitComponentDesc = pt;
    }

#endif

    eit_parser_clean_sht_desc_list(eitEventInfo);

    eit_parser_clean_ext_desc_list(eitEventInfo);

    return 0;
}


static __s32 eit_parser_clean_event_list(eit_event_info_t* header)
{

    eit_event_info_t* pt;

    pt = NULL;

    while(header != NULL)
    {
        eit_parser_clean_desc_list(header);
        pt = header->next;
        free(header);
        header = pt;
    }

    return 0;
}


/*******************************************************************************
Function name: eit_parser_clean_all_event
Description:
    1.清除解析后的结果的数据结构.
Parameters:

Return:

Time: 2011/2/22
*******************************************************************************/
static __s32 eit_parser_clean_all_event(eit_tp_t* tpHeader)
{
    eit_tp_t*           tp;
    eit_service_info_t* sp;
    eit_event_info_t*   ep;
    __s32               i;

    tp = tpHeader;

    while(NULL != tp)
    {
        sp = tp->eitServiceInfoHeader;

        while(NULL != sp)
        {
            ep = sp->pfEventHeader;
            eit_parser_clean_event_list(ep);
            sp->pfEventHeader    = NULL;
            sp->pfCurSectionMask = 0;
            sp->pfVersionNumber  = 0xff;

            for (i=0; i<EIT_SCHEDULE_TABLE_ID_NUM; i++)
            {
                ep = sp->schdlEventHeader[i];
                eit_parser_clean_event_list(ep);
                sp->schdlEventHeader[i]   = NULL;
                sp->schdlCurSectionNum[i] = 0xffff;
                sp->schdlVersionNumber[i] = 0xff;
            }

            memset(sp->schdlSectionMask, 0, EIT_SCHEDULE_TABLE_ID_NUM*32);

            sp = sp->next;
        }

        tp = tp->next;
    }

    return 0;
}


static __s32 eit_parser_clean_all_service(eit_tp_t* tpHeader)
{
    eit_tp_t*           tp;
    eit_service_info_t* sp;
    eit_service_info_t* sp1;

    tp  = tpHeader;
    sp  = NULL;
    sp1 = NULL;

    eit_parser_clean_all_event(tpHeader);

    while(NULL != tp)
    {
        sp = tp->eitServiceInfoHeader;

        while (NULL != sp)
        {
            sp1 = sp->next;
            free(sp);
            sp = sp1;
        }

        tp->eitServiceInfoHeader = NULL;
        tp = tp->next;
    }

    return 0;
}


static __s32 eit_parser_clean_all(eit_tp_t* tpHeader)
{
    eit_tp_t* tp;
    eit_tp_t* tp1;

    eit_parser_clean_all_service(tpHeader);
    tp = tpHeader;

    while(NULL != tp)
    {
        tp1 = tp->next;
        free(tp);
        tp = tp1;
    }

    return 0;
}


static eit_tp_t* eit_parser_get_tp(eit_tp_t* header, __u32 origNetworkId, __u16 tsId)
{
    eit_tp_t* eitTpPt = header;

    if (eitTpPt == NULL)
        return NULL;

    while (eitTpPt != NULL)
    {
#if 0
        if(eitTpPt->tsId == tsId && eitTpPt->origNetworkId == origNetworkId)
#else
        if(eitTpPt->tsId == tsId)
#endif
            break;

        eitTpPt = eitTpPt->next;
    }

    return eitTpPt;
}


static eit_tp_t* eit_parser_add_new_tp(eit_tp_t** tpHeader, __u16 tsId, __u32 origNetworkId)
{
    eit_tp_t* tpt;

    tpt = (eit_tp_t*)malloc(sizeof(eit_tp_t));
    if (tpt == NULL)
        return NULL;

    memset(tpt, 0, sizeof(eit_tp_t));

    tpt->tsId           = tsId;
    tpt->origNetworkId  = origNetworkId;

    tpt->next = *tpHeader;
    *tpHeader = tpt;

    return tpt;
}


static eit_service_info_t* eit_parser_get_service(eit_tp_t* tpt, __u16 serviceId)
{
    eit_service_info_t* eitServiceInfoPt;
    if(tpt == NULL)
        return NULL;

    eitServiceInfoPt = tpt->eitServiceInfoHeader;

    while(eitServiceInfoPt != NULL)
    {
        if(eitServiceInfoPt->serviceId == serviceId)
            break;

        eitServiceInfoPt = eitServiceInfoPt->next;
    }

    return eitServiceInfoPt;
}


static eit_service_info_t* eit_parser_add_new_service(eit_service_info_t** serviceHeader, __u16 serviceId)
{
    eit_service_info_t* spt;
    int i;

    spt = (eit_service_info_t*)malloc(sizeof(eit_service_info_t));
    if(NULL == spt)
        return spt;

    memset(spt, 0, sizeof(eit_service_info_t));

    spt->serviceId = serviceId;
    spt->pfVersionNumber  = 0xff;

    for (i=0; i<EIT_SCHEDULE_TABLE_ID_NUM; i++)
    {
        spt->schdlVersionNumber[i] = 0xff;
        spt->schdlCurSectionNum[i] = 0xffff;
    }

    spt->next = *serviceHeader;
    *serviceHeader = spt;

    return spt;
}


/* Convert BCD code to hour, min, sec */
static void bcd_to_hms(__u8* time, __u8* hour, __u8* min, __u8* sec)
{
    *hour = (time[0] >> 4) * 10 + (time[0] & 0x0f);
    *min  = (time[1] >> 4) * 10 + (time[1] & 0x0f);
    *sec  = (time[2] >> 4) * 10 + (time[2] & 0x0f);
}

/*
*Convert MJD code to year, month, day, week_day.  year >= 1900
*/
static void mjd_to_ymd(__u32 mjd, __u16* year, __u8* month, __u8* day, __u8* weekday)
{
    static __u32  preMjd = 0xffffffff;
    static __u16  preYear;
    static __u8   preMonth;
    static __u8   preDay;
    static __u8   preWeekday;

    __u32 Y, M, D, K;

    if (mjd == preMjd)
    {
        *year    = preYear;
        *month   = preMonth;
        *day     = preDay;
        *weekday = preWeekday;
    }
    else
    {
        __u32 YY;

        Y  = (20*mjd - 301564)/7305;
        YY = (Y*365 + Y/4);
        M  = 1000*(10*mjd - 149561 - 10*YY)/306001;
        D  = mjd - 14956 - YY - M*306001/10000;

        if (M==14 || M==15)
            K = 1;
        else
            K = 0;

        *year  = (__u16)(Y + K + 1900);
        *month = M - 1 - K*12;
        if (*month > 12)
            *month = 1;

        *day = D;
        if (*day > 31)
            *day = 1;

        *weekday = ((mjd + 2) % 7) + 1;

        preMjd     = mjd;
        preYear    = *year;
        preMonth   = *month;
        preDay     = *day;
        preWeekday = *weekday;
    }
}


/*if d2 > d1 , return 1,
if d2== d1, return 0,
if d2 < d1, return -1*/
static __s32 eit_compare_time(eit_date_time_t *d1, eit_date_time_t *d2)
{
    if (d2->year - d1->year != 0)
        return d2->year - d1->year;
    else if (d2->month - d1->month != 0)
        return d2->month - d1->month;
    else if (d2->day - d1->day != 0)
        return d2->day - d1->day;
    else if (d2->hour - d1->hour !=0)
        return d2->hour - d1->hour;
    else
        return d2->min - d1->min;
}


/*
* mjd_num: IN
* time: IN
* start_time: OUT, = mjd_num + time
* duration: IN
* end_time: OUT, = start_time + duration
*/
static void eit_parser_get_start_end_time(__u16 mjdNum, __u8* time, eit_date_time_t* startTime, __u32* duration, eit_date_time_t* endTime)
{
    __u8 hour, min, sec;
    __u8 durHour, durMin, durSec;
    __u8 carry;
    __u8 durationTime[3] = {0};
    carry = 0;

    bcd_to_hms(time, &hour, &min, &sec);
    startTime->mjd = mjdNum;
    startTime->hour = hour;
    startTime->min  = min;
    startTime->sec  = 0;
    mjd_to_ymd(mjdNum, &startTime->year, &startTime->month, &startTime->day, &startTime->weekday);

    durationTime[0] = ((*duration)>>16) & 0x00ff;
    durationTime[1] = ((*duration)>>8) & 0x00ff;
    durationTime[2] = ((*duration)>>0) & 0x00ff;
    // bcd_to_hms((__u8*)duration, &durHour, &durMin, &durSec);
    bcd_to_hms(durationTime, &durHour, &durMin, &durSec);

    durSec += sec;
    if (durSec >= 60)
        carry = 1;
    else
        carry = 0;
    durSec %= 60;

    durMin += (carry + min);
    if(durMin >= 60)
        carry = 1;
    else
        carry = 0;
    durMin %= 60;

    durHour += (carry+hour);
    if (durHour >= 24)
        carry = 1;
    else
        carry = 0;
    durHour %= 24;

    endTime->hour = durHour;
    endTime->min  = durMin;
    endTime->sec  = 0;
    endTime->mjd  = mjdNum + carry;
    mjd_to_ymd(endTime->mjd, &endTime->year, &endTime->month, &endTime->day, &endTime->weekday);
}


static eit_event_info_t* eit_parser_add_new_event(eit_event_info_t** eventHeader, eit_date_time_t* dt, __s32 event_type)
{
    __s32             ret;
    eit_event_info_t* tmp;
    eit_event_info_t* ept;
    eit_event_info_t* prev;

    tmp  = *eventHeader;

    ept = (eit_event_info_t*)malloc(sizeof(eit_event_info_t));
    if (NULL == ept)
        return NULL;

    memset(ept, 0, sizeof(eit_event_info_t));

    if(tmp==NULL)   //event_header is NULL
    {
        *eventHeader = ept;
        return ept;
    }

    while(tmp != NULL)                           // 对event 按时间进行排序
    {
        prev = tmp;
        ret  = eit_compare_time(&tmp->startTime, dt);
        if (ret == 0)
        {
            free(ept);
            return tmp;
        }
        else if (ret < 0)
            break;

        tmp = tmp->next;
    }

    if(tmp == NULL) //insert at the end.
    {
        prev->next = ept;
        ept->last  = prev;
    }
    else
    {
        if (tmp == *eventHeader) // insert in the header
        {
            *eventHeader = ept;
            ept->next    = tmp;
            ept->last    = NULL;
            tmp->last    = ept;
        }
        else    // insert between two node;
        {
            ept->next       = tmp;
            ept->last       = tmp->last;
            ept->last->next = ept;
            tmp->last       = ept;
        }
    }

    return ept;
}


//* parse short descriptor
static __s32 eit_parser_get_sht_desc(eit_short_desc_t** eitShrtDesc, __u8* descBuf)
{
    __s32               eventNameLen;
    __s32               eventTextLen;
    __s32               eventNameSize;
    __s32               eventTextSize;
    __s32               eventNameLenLimit;  //eventname的长度，不包括\0
    eit_short_desc_t*   descPt;
    eit_short_desc_t*   tmp;
    __u8*               p;
//  __u32               refCode;
    __u8                langCode[3];

    descPt  = *eitShrtDesc;
    tmp     = NULL;
    p       = descBuf;
//  refCode = 0;

    langCode[0] = p[2];
    langCode[1] = p[3];
    langCode[2] = p[4];

    //* get tail, check if lang_code already exists.
    while (descPt != NULL)
    {
        //* this lang_code's descriptor already exists.
        if(memcmp(descPt->langCode, langCode, 3) == 0)
            return -1;

        tmp = descPt;
        descPt = descPt->next;
    }

    descPt = (eit_short_desc_t*)malloc(sizeof(eit_short_desc_t));
    if (descPt == NULL)
    {
        __msg("malloc memory for short descriptor fail.\n");
        return -1;
    }

    memset(descPt, 0, sizeof(eit_short_desc_t));

    p += 2;
    descPt->langCode[0] = *p++;
    descPt->langCode[1] = *p++;
    descPt->langCode[2] = *p++;
    //* refCode = get_iso639_refcode(descPt->langCode);
    //* subset = get_latin_national_subset(desc_pt->lang_code);

    eventNameLen        = *p++;
    if (eventNameLen > EVENT_NAME_LENGTH_LIMIT)
        eventNameLenLimit = EVENT_NAME_LENGTH_LIMIT;
    else
        eventNameLenLimit = eventNameLen;

    //把eit_pool中的content(也就是eit section)解析出来放到eit_short_desc_t中去.
    if (eventNameLenLimit > 0)
    {
        eventNameSize = 4 * eventNameLenLimit + 10;
        //descPt->eventName = (__u8*)malloc(eventNameLenLimit + 1);
        descPt->eventName = (__u8*)malloc(eventNameSize);
        if (NULL == descPt->eventName)
        {
            free(descPt);
            __wrn("malloc memory for short descriptor fail.\n");
            return -1;
        }
        //memcpy(descPt->eventName, p, eventNameLenLimit);
        //descPt->eventName[eventNameLenLimit] = 0;
        //descPt->eventNameLen = eventNameLenLimit + 1;
        descPt->eventNameLen = isdb_convert_str_to_utf8(p, eventNameLenLimit, descPt->eventName, eventNameSize-1, descPt->langCode);
        descPt->eventName[descPt->eventNameLen] = 0;
        if (descPt->eventName[0] == 0)
        {
            __wrn("why event name[0] = 0?\n");
            free(descPt->eventName);
            free(descPt);
            return -1;
        }
        //__wrn(" #name: %s\n", descPt->eventName);
    }
    //将新的descPt加入链表
    if (tmp != NULL)
        tmp->next = descPt;
    else
        *eitShrtDesc = descPt;

    p += eventNameLen;
    eventTextLen = *p++;
    if (eventTextLen > 0)
    {
        descPt->textDescFlag = 1;
        eventTextSize = 4 * eventTextLen + 10;
        //descPt->textDesc     = (__u8*)malloc(eventTextLen + 1);
        descPt->textDesc     = (__u8*)malloc(eventTextSize);
        if (descPt->textDesc == NULL)
        {
            __wrn("malloc memory for short descriptor fail.\n");
            return -1;
        }

//      memcpy(descPt->textDesc, p, eventTextLen);
//      descPt->textDesc[eventTextLen] = 0;
//      descPt->textDescLen = eventTextLen + 1;
        descPt->textDescLen = isdb_convert_str_to_utf8(p, eventTextLen, descPt->textDesc, eventTextSize-1, descPt->langCode);
        descPt->textDesc[descPt->textDescLen] = 0;
        //__wrn(" #text: %s\n", descPt->textDesc);
        //__wrn(" ==========================\n\n");
    }

    return 0;
}

//get extend descriptor
static __s32 eit_parser_get_ext_desc(eit_ext_desc_t** eitExtDesc, __u8* descBuf)
{
//  extern UINT8 ComUniStrLen(const UINT16* string);
    eit_ext_desc_t* descPt;
    eit_ext_desc_t* tmp;
//  __u32           refcode;
    __s32           descTag;
//  __s32           descLen;
    __s32           itemLen;
    __s32           textLen;
    __u8*           p;
    __u8            langCode[3];
    __u8            descNumber;
    __u8            lastDescNumber;
    eit_ext_desc_t* tmp1;
    eit_ext_desc_t* tmp2;
    eit_ext_desc_t* prev1;
    eit_ext_desc_t* prev2;

    descPt          = *eitExtDesc;
    tmp             = NULL;
    textLen         = 0;
    p               = descBuf;
    descNumber      = 0;
    lastDescNumber  = 0;
    tmp1            = NULL;
    tmp2            = NULL;
    prev1           = NULL;
    prev2           = NULL;

    descTag = p[0];
//  descLen = p[1];

    if (descTag == EXTENTED_EVENT_DESC)
    {
        descNumber      = (p[2]&0xf0)>>4;
        lastDescNumber  = p[2]&0x0f;
        langCode[0]     = p[3];
        langCode[1]     = p[4];
        langCode[2]     = p[5];
        //* refcode = get_iso639_refcode(langCode);

        itemLen = p[6];
        textLen = p[itemLen + 7];

        p = descBuf + itemLen + 8;
    }
    else if(descTag == SHORT_EVENT_DESC)
    {
        itemLen = p[5];
        textLen = p[6+itemLen];

        p = descBuf + itemLen + 7;
    }

    if (textLen > 0)
    {
        while (descPt != NULL)
        {
            if (descTag == EXTENTED_EVENT_DESC)
            {
                if (memcmp(descPt->langCode, langCode, 3) == 0 && descPt->descNumber == descNumber)
                {
                    return -1;
                }
            }

            tmp = descPt;
            descPt = descPt->next;
        }

        descPt = (eit_ext_desc_t*)malloc(sizeof(eit_ext_desc_t));
        if (descPt == NULL)
        {
            __msg("malloc memory for eit extented descriptor fail.\n");
            return -1;
        }

        memset(descPt, 0, sizeof(eit_ext_desc_t));

        if (descTag == EXTENTED_EVENT_DESC)
        {
            descPt->langCode[0]     = langCode[0];
            descPt->langCode[1]     = langCode[1];
            descPt->langCode[2]     = langCode[2];
            descPt->descNumber      = descNumber;
            descPt->lastDescNumber  = lastDescNumber;
            descPt->descNumberMask |= (1<<descNumber);
        }

        descPt->textChar = (__u8*)malloc(textLen + 1);
        if (descPt->textChar == NULL)
        {
            __msg("malloc memory for eit extented descriptor fail.\n");
            free(descPt);
            return -1;
        }

        memcpy(descPt->textChar, p, textLen);
        descPt->textChar[textLen] = 0;
        if (descPt->textChar[0] == 0)
        {
            free(descPt->textChar);
            free(descPt);
            return -1;
        }

        if (descTag == EXTENTED_EVENT_DESC)
        {
            if (tmp != NULL)
            {
                tmp1 = *eitExtDesc;
                while (tmp1 != NULL)
                {
                    if (memcmp(descPt->langCode, tmp1->langCode, 3) == 0)
                    {
                        tmp2 = tmp1;
                        while(tmp2 != NULL && memcmp(descPt->langCode, tmp2->langCode, 3) == 0)
                        {
                            if (descPt->descNumber < tmp2->descNumber)
                                break;

                            prev2 = tmp2;
                            tmp2  = tmp2->next;
                        }

                        if (tmp2 == NULL)
                            prev2->next = descPt;
                        else
                        {
                            if(prev2 == NULL)
                            {
                                if(prev1 != NULL)
                                    prev1->next = descPt;
                                else
                                    *eitExtDesc = descPt;
                            }
                            else
                            {
                                prev2->next = descPt;
                            }

                            descPt->next = tmp2;
                        }

                        break;
                    }

                    prev1   = tmp1;
                    tmp1    = tmp1->next;
                }

                if(tmp1 == NULL)
                {
                    prev1->next = descPt;
                }
            }
            else
                *eitExtDesc = descPt;
        }
        else if (descTag == SHORT_EVENT_DESC) //always put the short desc text in the head of  desc list.
        {
            descPt->next  = *eitExtDesc;
            *eitExtDesc   = descPt;
        }
    }
    else
        __msg("text_len = %d in eit_parser_get_ext_desc.\n", textLen);

    return 0;
}


static __s32 eit_parser_get_content_desc(eit_content_desc_t** eitContentDesc, __u8* descBuf)
{
    __u8* p;
    eit_content_desc_t* descPt;

    p      = descBuf + 2;
    descPt = *eitContentDesc;

    if (descPt == NULL)
    {
        descPt = (eit_content_desc_t*) malloc(sizeof(eit_content_desc_t));

        if (descPt == NULL)
        {
            __msg("alloc memory for content descriptor fail.\n");
            return -1;
        }

        memset(descPt, 0, sizeof(eit_content_desc_t));
    }

    if (*eitContentDesc == NULL)
        *eitContentDesc = descPt;

    descPt->contentNibble1 = ((*p)>>4) & 0xf;
    descPt->contentNibble2 = (*p) & 0xf;
    descPt->userNibble1    = (*(p+1) >>4) & 0xf;
    descPt->userNibble2    = (*(p+1)) & 0xf;

    return 0;
}


static __s32 eit_parser_get_pr_desc(eit_pr_desc_t** eitPrDesc, __u8* descBuf)
{
    __u8*           p;
    eit_pr_desc_t*  descPt;

    p      = descBuf + 2;
    descPt = *eitPrDesc;

    if (descPt == NULL)
    {
        descPt = (eit_pr_desc_t*)malloc(sizeof(eit_pr_desc_t));
        if (descPt == NULL)
        {
            __msg("alloc memory for parent rating descriptor fail.\n");
            return -1;
        }

        memset(descPt, 0, sizeof(eit_pr_desc_t));
    }

    if (*eitPrDesc == NULL)
        *eitPrDesc = descPt;

    descPt->countryCode = *p<<16 | *(p+1)<<8 | *(p+2);
    descPt->rating      = *(p+3);

    if (descPt->rating >= 0x10)
        __msg("!!! warning -- rating: %x defined by the broadcaster !!!\n", descPt->rating);

    return 0;
}


#ifdef PARSING_EXTRA_DESC_EN

__s32 eit_parser_get_component_desc(eit_component_desc_t** eitComponentDesc, __u8* descBuf)
{
    eit_component_desc_t* descPt;
    eit_component_desc_t* tmp;
    __u8*                 p;

    descPt = *eitComponentDesc;
    tmp    = NULL;
    p      = descBuf;

    while (descPt != NULL)
    {
        tmp    = descPt;
        descPt = descPt->next;
    }

    descPt = (eit_component_desc_t*) malloc(sizeof(eit_component_desc_t));
    if (descPt == NULL)
    {
        __msg("alloc memory for component descriptor.\n");
        return -1;
    }

    memset(descPt, 0, sizeof(eit_component_desc_t));

    descPt->next = NULL;
    if (tmp != NULL)
        tmp->next = descPt;
    else
        *eitComponentDesc = descPt;

    descPt->conponentType = p[3];
    descPt->streamType    = p[2] & 0xf;
    descPt->languageCode  = p[5]<<16 | p[6]<<8 | p[7];

    return 0;
}


static __s32 eit_parser_get_ca_desc(eit_ca_desc_t** eitCaDesc, __u8* descBuf)
{
    __s32           descLen;
    __u8*           p;
    eit_ca_desc_t*  descPt;
    eit_ca_desc_t*  tmp;
    __s32           i;

    descLen = descBuf[1];
    p       = descBuf + 2;
    descPt  = *eitCaDesc;
    tmp     = NULL;
    i       = 2;

    while (i<descLen)
    {
        if (NULL == descPt)
        {
            descPt = (eit_ca_desc_t*)malloc(sizeof(eit_ca_desc_t));

            if (descPt == NULL)
            {
                __msg("alloc memory for ca descriptor fail.\n");
                return -1;
            }

            descPt->next = NULL;
        }

        if (tmp != NULL)
            tmp->next = descPt;
        else
            *eitCaDesc = descPt;

        descPt->caSystemId = p[0]<<8 | p[1];

        p += 2;
        i += 2;

        tmp    = descPt;
        descPt = descPt->next;
    }

    return 0;
}

static __s32 eit_parser_get_tele_desc(eit_tele_desc_t** eitTeleDesc, __u8* descBuf)
{
    return 0;
}

#endif  //* #ifdef PARSING_EXTRA_DESC_EN


static __s32 eit_parser_get_event_info(eit_event_info_t** eitEventInfoHeader, __u8* eventBuf, __s32 eventLen, __s32 eventType)
{
    __u16               mjdNum;
    __u8                time[4];
    __u32               duration;
    __u16               eventLoopStart;
    __u16               descLoopStart;
    __u16               descLoopLen;
    __u16               descLen;
    __u8                descTag;
    eit_event_info_t*   eitEventInfoPt;
    __u8*               p;
    eit_date_time_t     startTime;
    eit_date_time_t     endTime;
    __u16               eventId;
    __u16               runningStatus;
    __u32               parseFlag;

    parseFlag       = 1;
    mjdNum          = 0;
    duration        = 0;
    eventLoopStart  = 0;
    descLoopStart   = 0;
    eitEventInfoPt  = NULL;
    p               = eventBuf;

    do
    {
        eventId = p[0]<<8 | p[1];
        p += 2;

        mjdNum = p[0]<<8 | p[1];
        p += 2;

        time[0] = *p++;
        time[1] = *p++;
        time[2] = *p++;

        duration = p[0]<<16 | p[1]<<8 | p[2];
        p += 3;

        runningStatus = *p>>5;
        descLoopLen   = (p[0]&0xf)<<8 | p[1];

        p += 2;

        eit_parser_get_start_end_time(mjdNum, time, &startTime, &duration, &endTime);

#if 0
        if(fast_parse.fast_parse_flag)
        {
            if (eit_compare_time(&startTime, &fast_parse.start) >= 0 && eit_compare_time(&endTime, &fast_parse.start) < 0)
                parseFlag =1;
            else if (eit_compare_time(&startTime, &fast_parse.end) >=0 && eit_compare_time(&endTime,&fast_parse.end) <= 0)
                parseFlag =1;
            else if (eit_compare_time(&startTime, &fast_parse.start) <= 0 && eit_compare_time(&endTime, &fast_parse.end) >= 0)
                parseFlag =1;
            else
                parseFlag = 0;
        }
#endif
        eventLoopStart += 12;
        descLoopStart   = 0;

        if (eventLoopStart + descLoopLen > eventLen)
            return -1;

        if(parseFlag)
        {
            eitEventInfoPt = eit_parser_add_new_event(eitEventInfoHeader, &startTime, eventType);
            if (eitEventInfoPt==NULL)
            {
                __wrn("alloc fail when parsing event info.\n");
                return -1;
            }

            eitEventInfoPt->duration  = duration;
            eitEventInfoPt->eventId   = eventId;
            eitEventInfoPt->startTime = startTime;
            eitEventInfoPt->endTime   = endTime;

            if (PRESENT_EVENT == eventType)
                eitEventInfoPt->runningStatus = (runningStatus & 0x0f) | 0x40;
            else if(FOLLOWING_EVENT == eventType)
                eitEventInfoPt->runningStatus = runningStatus | 0x80;
            else
                eitEventInfoPt->runningStatus = runningStatus;
        }

        descLoopStart = 0;
        while (descLoopStart < descLoopLen)
        {
            descTag = *p;
            descLen = *(p+1);

            if (parseFlag)
            {
                switch (descTag)
                {
                    case SHORT_EVENT_DESC:
                    {
                        if (eit_parser_get_sht_desc(&eitEventInfoPt->eitShtDesc, p) != 0)
                        {
                            /*return !SUCCESS*/;    //continue
                        }
                        break;
                    }
                    case CONTENT_DESC:
                    {
                        eit_parser_get_content_desc(&eitEventInfoPt->eitContentDesc, p);
                        break;
                    }
                    case PARENTAL_RATING_DESC:
                    {
                        eit_parser_get_pr_desc(&eitEventInfoPt->eitPrdesc, p);
                        break;
                    }
                    case EXTENTED_EVENT_DESC:
                    {
                        eit_parser_get_ext_desc(&eitEventInfoPt->eitExtDesc, p);
                        break;
                    }
#ifdef PARSING_EXTRA_DESC_EN
                    case CA_DESC:
                    {
                        if(eit_parser_get_ca_desc(&eitEventInfoPt->eitCaDesc, p) != 0)
                            return -1;
                        break;
                    }

                    case TELE_TEXT_DESC:
                    {
                        if (eit_parser_get_tele_desc(&eitEventInfoPt->eitTeleDesc, p) != 0)
                            return -1;
                        break;
                    }

                    case COMPONENT_DESC:
                    {
                        if (eit_parser_get_component_desc(&eitEventInfoPt->eitComponentDesc, p) != 0)
                            return -1;
                        break;
                    }
#endif

                    default:
                        break;
                }
            }

            p += (descLen + 2);
            descLoopStart += (descLen + 2);
        }

        eventLoopStart += descLoopLen;

    }
    while(eventLoopStart<eventLen);

    return 0;
}


/*******************************************************************************
Function name: eit_parser_clean
Description:
    1.清除解析后的结果的数据结构
Parameters:

Return:

Time: 2011/2/22
*******************************************************************************/
__s32 eit_parser_clean(void)
{
    eit_parser_clean_all(gEitTpHeader);
    gEitTpHeader = NULL;
    return 0;
}

#if 0
__s32 eit_parser_parse_section(__u32 tpId, __u8* section, __u32 length)
{
    return 0;
}
#endif

__s32 eit_parser_parse_event(__u32 tpId,
                             __u16 tsId,
                             __u16 origNetworkId,
                             __u16 serviceId,
                             __u8  tableId,
                             __u8  versionNum,
                             __u8  curNextIdct,
                             __u8  sectionNum,
                             __u8* eventBuf,
                             __u32 eventInfoLen)
{
    __s32               i;
    __s32               j;
    eit_service_info_t* eitServiceInfoPt;
    eit_tp_t*           eitTpPt;
    __u8                segment;

    eitServiceInfoPt = NULL;
    eitTpPt          = NULL;

#if 0
    //* for dvb-s(shenzhou, 2tuner dvr), some services have the same [orig_network_id, ts_id, service_id],
    //* so we have to use tp_id instead of orig_network_id!
    eitTpPt = eit_parser_get_tp(gEitTpHeader, tpId, tsId);
    if (eitTpPt == NULL)
        eitTpPt = eit_parser_add_new_tp(&gEitTpHeader, tsId, tpId);
#else
    eitTpPt = eit_parser_get_tp(gEitTpHeader, origNetworkId, tsId);
    if (eitTpPt == NULL)
        eitTpPt = eit_parser_add_new_tp(&gEitTpHeader, tsId, origNetworkId);
#endif

    if (eitTpPt == NULL)
    {
        __msg("new tp allocate failed. Not enough memory.\n");
        return -1;
    }

    eitServiceInfoPt = eit_parser_get_service(eitTpPt, serviceId);
    if (eitServiceInfoPt == NULL)
        eitServiceInfoPt = eit_parser_add_new_service(&eitTpPt->eitServiceInfoHeader, serviceId);

    if (eitServiceInfoPt == NULL)
    {
        __msg("new service allocate failed! Not enough memory!\n");
        return -1;
    }

    if (IS_PF(tableId))
    {
        /*the eit content of this service has changed*/
        if (eitServiceInfoPt->pfVersionNumber != versionNum && eitServiceInfoPt->pfVersionNumber != 0xff && curNextIdct == 1)
        {
            __msg("last version: %d, current version: %d\n",eitServiceInfoPt->pfVersionNumber, versionNum);
            eit_parser_clean_event_list(eitServiceInfoPt->pfEventHeader);
            eitServiceInfoPt->pfEventHeader    = NULL;
            eitServiceInfoPt->pfVersionNumber  = 0xff;
            eitServiceInfoPt->pfCurSectionMask = 0;
        }
        if (eventInfoLen == 0 || ((1<<sectionNum) & eitServiceInfoPt->pfCurSectionMask) != 0) //section already exist
        {
            __msg("section %d already exist!\n", sectionNum);
            return -1;
        }

        if (eit_parser_get_event_info(&eitServiceInfoPt->pfEventHeader, eventBuf, eventInfoLen, sectionNum) != 0)
        {
            __msg("eit_get_event_info error!\n");
            return -1;
        }

        //* update
        eitServiceInfoPt->pfCurSectionMask |= (1<<sectionNum);
        eitServiceInfoPt->pfVersionNumber   = versionNum;
    }
    else
    {
        i = tableId & 0x0F;

        /*the eit content of this service has changed*/
        if (eitServiceInfoPt->schdlVersionNumber[i] != versionNum && curNextIdct == 1)
        {
            __msg("table %x - last version: %d, current version: %d\n", tableId, eitServiceInfoPt->schdlVersionNumber[i], versionNum);
            eit_parser_clean_event_list(eitServiceInfoPt->schdlEventHeader[i]);
            eitServiceInfoPt->schdlEventHeader[i]   = NULL;
            eitServiceInfoPt->schdlVersionNumber[i] = 0xff;
            for (j=0; j<32; j++)
                eitServiceInfoPt->schdlSectionMask[i][j] = 0;
        }

        segment = sectionNum >> 3;

        if ((eitServiceInfoPt->schdlSectionMask[i][segment] & (1<<(sectionNum & 0x7))) != 0)
        {
            __msg("section %d already exist!\n", sectionNum);
            return -1;
        }

#if 0
        //always parse to sch_event_header[0]
        if (eit_parser_get_event_info(&eitServiceInfoPt->schdlEventHeader[0], eventBuf, eventInfoLen, SCHEDULE_EVENT) != 0)
#else
        if (eit_parser_get_event_info(&eitServiceInfoPt->schdlEventHeader[i], eventBuf, eventInfoLen, SCHEDULE_EVENT) != 0)
#endif
        {
            __msg("eit_get_event_info error!\n");
            return -1;
        }

        //* update
        eitServiceInfoPt->schdlSectionMask[i][segment] |= (1 << (sectionNum & 0x7));
        eitServiceInfoPt->schdlVersionNumber[i] = versionNum;
    }


    return 0;
}

__s32 eit_parser_get_schedule(__u32 tpId, __u16 tsId, __u16 origNetworkId, __u16 serviceId, maple_schdl_list_t* schdlList, __u32 maxEventNum)
{
    eit_service_info_t*  eitServiceInfoPt;
    eit_tp_t*            eitTpPt;
    eit_event_info_t*    pSchdl;
    maple_schdl_event_t* pEvent;
    eit_short_desc_t*    shtDesc;
    __u32                i;
    __u32                j;
    //__u32               tmpLen;

    eitServiceInfoPt = NULL;
    eitTpPt          = NULL;

    schdlList->eventNum = 0;

#if 0
    //* for dvb-s(shenzhou, 2tuner dvr), some services have the same [orig_network_id, ts_id, service_id],
    //* so we have to use tp_id instead of orig_network_id!
    eitTpPt = eit_parser_get_tp(gEitTpHeader, tpId, tsId);
    if (eitTpPt == NULL)
    {
        __msg("can not find tp of tp id %d.\n", tpId);
        return -1;
    }
#else
    eitTpPt = eit_parser_get_tp(gEitTpHeader, origNetworkId, tsId);
    if (eitTpPt == NULL)
    {
        __msg("can not find tp of original network id %d.\n", origNetworkId);
        return -1;
    }
#endif


    eitServiceInfoPt = eit_parser_get_service(eitTpPt, serviceId);
    if (eitServiceInfoPt == NULL)
    {
        __msg("can not find service of service id %d.\n", serviceId);
        return -1;
    }
    //这里不区分extendedSchedule,因为都在这个函数之前,会选择解析basic schedule还是extended schedule,解析完后统一放在eitServiceInfoPt->schdlEventHeader中.
    //isdb的spec规定0x50~0x57是basic, 0x58~0x5f是extended.所以不冲突.
    for (i=0, j=0; i<EIT_SCHEDULE_TABLE_ID_NUM; i++)
    {
        pSchdl = eitServiceInfoPt->schdlEventHeader[i];
        while(pSchdl != NULL)
        {
            shtDesc = pSchdl->eitShtDesc;
            while(shtDesc != NULL)
            {
                pEvent = &schdlList->pSchdlEvents[j];

//              isdb_convert_str_to_utf8(shtDesc->eventName,
//                                       shtDesc->eventNameLen,
//                                       pEvent->name,
//                                       MAPLE_MAX_NAME_LEN,
//                                       shtDesc->langCode);
                safeStringCopy(pEvent->name, MAPLE_MAX_NAME_LEN, shtDesc->eventName, shtDesc->eventNameLen);
                if(shtDesc->textDescFlag)
                {
                    pEvent->eventContent = shtDesc->textDesc;
                    pEvent->eventContentLen = shtDesc->textDescLen;
                }
                else
                {
                    pEvent->eventContent = NULL;
                    pEvent->eventContentLen = 0;
                }

                pEvent->startTime.MJD       = pSchdl->startTime.mjd;
                pEvent->startTime.year      = pSchdl->startTime.year;
                pEvent->startTime.month     = pSchdl->startTime.month;
                pEvent->startTime.day       = pSchdl->startTime.day;
                pEvent->startTime.hour      = pSchdl->startTime.hour;
                pEvent->startTime.min       = pSchdl->startTime.min;
                pEvent->startTime.sec       = pSchdl->startTime.sec;
                pEvent->startTime.weekDay   = pSchdl->startTime.weekday;
                pEvent->endTime.MJD         = pSchdl->endTime.mjd;
                pEvent->endTime.year        = pSchdl->endTime.year;
                pEvent->endTime.month       = pSchdl->endTime.month;
                pEvent->endTime.day         = pSchdl->endTime.day;
                pEvent->endTime.hour        = pSchdl->endTime.hour;
                pEvent->endTime.min         = pSchdl->endTime.min;
                pEvent->endTime.sec         = pSchdl->endTime.sec;
                pEvent->endTime.weekDay     = pSchdl->endTime.weekday;

                shtDesc = shtDesc->next;
                j++;
                if (j >= maxEventNum)
                {
                    __wrn("serviceId[0x%x], get schedule max event num[%d], table_i =[%d], break!\n", serviceId, maxEventNum, i);
                    //break;
                    goto __skip_get_schedule;
                }
            }
            pSchdl = pSchdl->next;
            if (j >= maxEventNum)
                break;
        }
    }
__skip_get_schedule:

    if(j == 0)  //* MEit and LEit store schedule at pfEventHeader.
    {
        pSchdl = eitServiceInfoPt->pfEventHeader;
        while(pSchdl != NULL)
        {
            shtDesc = pSchdl->eitShtDesc;
            while(shtDesc != NULL)
            {
                pEvent = &schdlList->pSchdlEvents[j];

//              isdb_convert_str_to_utf8(shtDesc->eventName,
//                                       shtDesc->eventNameLen,
//                                       pEvent->name,
//                                       MAPLE_MAX_NAME_LEN,
//                                       shtDesc->langCode);
                safeStringCopy(pEvent->name, MAPLE_MAX_NAME_LEN, shtDesc->eventName, shtDesc->eventNameLen);
                if(shtDesc->textDescFlag)
                {
                    pEvent->eventContent = shtDesc->textDesc;
                    pEvent->eventContentLen = shtDesc->textDescLen;
                }
                else
                {
                    pEvent->eventContent = NULL;
                    pEvent->eventContentLen = 0;
                }

                pEvent->startTime.MJD       = pSchdl->startTime.mjd;
                pEvent->startTime.year      = pSchdl->startTime.year;
                pEvent->startTime.month     = pSchdl->startTime.month;
                pEvent->startTime.day       = pSchdl->startTime.day;
                pEvent->startTime.hour      = pSchdl->startTime.hour;
                pEvent->startTime.min       = pSchdl->startTime.min;
                pEvent->startTime.sec       = pSchdl->startTime.sec;
                pEvent->startTime.weekDay   = pSchdl->startTime.weekday;
                pEvent->endTime.MJD         = pSchdl->endTime.mjd;
                pEvent->endTime.year        = pSchdl->endTime.year;
                pEvent->endTime.month       = pSchdl->endTime.month;
                pEvent->endTime.day         = pSchdl->endTime.day;
                pEvent->endTime.hour        = pSchdl->endTime.hour;
                pEvent->endTime.min         = pSchdl->endTime.min;
                pEvent->endTime.sec         = pSchdl->endTime.sec;
                pEvent->endTime.weekDay     = pSchdl->endTime.weekday;

                shtDesc = shtDesc->next;

                j++;

                if (j >= maxEventNum)
                    break;
            }

            pSchdl = pSchdl->next;

            if (j >= maxEventNum)
                break;
        }
    }

    schdlList->eventNum = j;

    return 0;
}

__s32 eit_parser_get_pf(__u32 tpId, __u16 tsId, __u16 origNetworkId, __u16 serviceId, maple_sepg_t* pf)
{
    eit_service_info_t* eitServiceInfoPt;
    eit_tp_t*           eitTpPt;
    eit_event_info_t*   pfEvent;
    eit_short_desc_t*   eitShtDesc;
    eit_pr_desc_t*      eitPrDesc;

    eitServiceInfoPt = NULL;
    eitTpPt          = NULL;

    pf->hasCurEventInfo  = 0;
    pf->hasNextEventInfo = 0;
    pf->nParentalRating = 0;
    pf->nFollowingParentalRating = 0;

#if 0
    //* for dvb-s(shenzhou, 2tuner dvr), some services have the same [orig_network_id, ts_id, service_id],
    //* so we have to use tp_id instead of orig_network_id!
    eitTpPt = eit_parser_get_tp(gEitTpHeader, tpId, tsId);
    if (eitTpPt == NULL)
    {
        __msg("can not find tp of tp id %d.\n", tpId);
        return -1;
    }
#else
    eitTpPt = eit_parser_get_tp(gEitTpHeader, origNetworkId, tsId);
    if (eitTpPt == NULL)
    {
        __msg("can not find tp of original network id %d.\n", origNetworkId);
        return -1;
    }
#endif

    eitServiceInfoPt = eit_parser_get_service(eitTpPt, serviceId);
    if (eitServiceInfoPt == NULL)
    {
        __msg("can not find service of service id %d.\n", serviceId);
        return -1;
    }

    pfEvent = eitServiceInfoPt->pfEventHeader;
    while(pfEvent != NULL)
    {
        //先拿short event descriptor
        eitShtDesc = pfEvent->eitShtDesc;
        if (eitShtDesc != NULL)
        {
            if (pfEvent->runningStatus & 0x80)
            {
//              isdb_convert_str_to_utf8(eitShtDesc->eventName,
//                                      eitShtDesc->eventNameLen,
//                                      pf->nextEventName,
//                                      MAPLE_MAX_NAME_LEN,
//                                      pfEvent->eitShtDesc->langCode);

                safeStringCopy(pf->nextEventName, MAPLE_MAX_NAME_LEN, eitShtDesc->eventName, eitShtDesc->eventNameLen);
                if(eitShtDesc->textDescFlag)
                {
                    pf->nextEventContent = eitShtDesc->textDesc;
                    pf->nextEventContentLen = eitShtDesc->textDescLen;
                }
                else
                {
                    pf->nextEventContent = NULL;
                    pf->nextEventContentLen = 0;
                }
                pf->nextEventstartTime.MJD      = pfEvent->startTime.mjd;
                pf->nextEventstartTime.year     = pfEvent->startTime.year;
                pf->nextEventstartTime.month    = pfEvent->startTime.month;
                pf->nextEventstartTime.day      = pfEvent->startTime.day;
                pf->nextEventstartTime.hour     = pfEvent->startTime.hour;
                pf->nextEventstartTime.min      = pfEvent->startTime.min;
                pf->nextEventstartTime.sec      = pfEvent->startTime.sec;
                pf->nextEventstartTime.weekDay  = pfEvent->startTime.weekday;
                pf->nextEventEndTime.MJD        = pfEvent->endTime.mjd;
                pf->nextEventEndTime.year       = pfEvent->endTime.year;
                pf->nextEventEndTime.month      = pfEvent->endTime.month;
                pf->nextEventEndTime.day        = pfEvent->endTime.day;
                pf->nextEventEndTime.hour       = pfEvent->endTime.hour;
                pf->nextEventEndTime.min        = pfEvent->endTime.min;
                pf->nextEventEndTime.sec        = pfEvent->endTime.sec;
                pf->nextEventEndTime.weekDay    = pfEvent->endTime.weekday;
                pf->hasNextEventInfo            = 1;
            }
            else if(pfEvent->runningStatus & 0x40)
            {
//              isdb_convert_str_to_utf8(eitShtDesc->eventName,
//                                      eitShtDesc->eventNameLen,
//                                      pf->curEventName,
//                                      MAPLE_MAX_NAME_LEN,
//                                      pfEvent->eitShtDesc->langCode);
                safeStringCopy(pf->curEventName, MAPLE_MAX_NAME_LEN, eitShtDesc->eventName, eitShtDesc->eventNameLen);
                if(eitShtDesc->textDescFlag)
                {
                    pf->curEventContent = eitShtDesc->textDesc;
                    pf->curEventContentLen = eitShtDesc->textDescLen;
                }
                else
                {
                    pf->curEventContent = NULL;
                    pf->curEventContentLen = 0;
                }

                pf->curEventstartTime.MJD       = pfEvent->startTime.mjd;
                pf->curEventstartTime.year      = pfEvent->startTime.year;
                pf->curEventstartTime.month     = pfEvent->startTime.month;
                pf->curEventstartTime.day       = pfEvent->startTime.day;
                pf->curEventstartTime.hour      = pfEvent->startTime.hour;
                pf->curEventstartTime.min       = pfEvent->startTime.min;
                pf->curEventstartTime.sec       = pfEvent->startTime.sec;
                pf->curEventstartTime.weekDay   = pfEvent->startTime.weekday;
                pf->curEventEndTime.MJD         = pfEvent->endTime.mjd;
                pf->curEventEndTime.year        = pfEvent->endTime.year;
                pf->curEventEndTime.month       = pfEvent->endTime.month;
                pf->curEventEndTime.day         = pfEvent->endTime.day;
                pf->curEventEndTime.hour        = pfEvent->endTime.hour;
                pf->curEventEndTime.min         = pfEvent->endTime.min;
                pf->curEventEndTime.sec         = pfEvent->endTime.sec;
                pf->curEventEndTime.weekDay     = pfEvent->endTime.weekday;
                pf->hasCurEventInfo             = 1;
            }
        }
        //再拿parental rating
        eitPrDesc = pfEvent->eitPrdesc;
        if(pfEvent->runningStatus & 0x40)   //表示是present event
        {
            if(eitPrDesc!=NULL)
            {
                pf->nParentalRating = eitPrDesc->rating;
            }
            else
            {
                pf->nParentalRating = 0;
            }
        }
        else if(pfEvent->runningStatus & 0x80)  //表示是following event
        {
            if(eitPrDesc!=NULL)
            {
                pf->nFollowingParentalRating = eitPrDesc->rating;
            }
            else
            {
                pf->nFollowingParentalRating = 0;
            }
        }
        pfEvent = pfEvent->next;
    }

    return 0;

}

