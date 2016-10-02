
#ifndef EIT_PARSER_H
#define EIT_PARSER_H

#include "isdb_interface.h"
#include "isdb_eit_pool.h"

#define	EVENT_NAME_LENGTH_LIMIT		256  //event name的最大长度,不包括\0


#define SHORT_EVENT_DESC  		0x4D
#define	EXTENTED_EVENT_DESC 	0x4E
#define COMPONENT_DESC			0x50
#define CONTENT_DESC			0x54
#define PARENTAL_RATING_DESC	0x55
#define TELE_TEXT_DESC			0x57
#define CA_DESC					0x53

#define PRESENT_EVENT 			0x0
#define FOLLOWING_EVENT			0x1
#define SCHEDULE_EVENT 			0x2

#ifndef memcmp
#define memcmp(src1, src2, size)        eLIBs_memcmp(src1, src2, size)
#endif

//* #define PARSING_EXTRA_DESC_EN

typedef struct EIT_DATE_TIME_TYPE
{
	__u32 mjd;
	__u16 year;
	__u8  month;
	__u8  day;
	__u8  weekday;
	__u8  hour;
	__u8  min;
	__u8  sec;
}eit_date_time_t;


/* content */
typedef struct EIT_CONTENT_DESC_TYPE eit_content_desc_t;
typedef struct EIT_CONTENT_DESC_TYPE
{
	__u8				contentNibble1;
	__u8				contentNibble2;
	__u8				userNibble1;
	__u8				userNibble2;
	eit_content_desc_t*	next;
}eit_content_desc_t;


/* parent rating descriptor */
typedef struct EIT_PR_DESC_TYPE	eit_pr_desc_t;
typedef struct EIT_PR_DESC_TYPE 
{
	__u32			countryCode;
	__u32			rating;
	eit_pr_desc_t*	next;
}eit_pr_desc_t;


/* short event descriptor */
typedef struct EIT_SHORT_DESC_TYPE	eit_short_desc_t;
typedef struct EIT_SHORT_DESC_TYPE
{
	__u8				langCode[3];
	__u8				textDescFlag;   // 1 -- have text desc 0 -- not 
	__u16               eventNameLen;   //老版本包括一个字节的\0的长度. 新版本就是字符串的长度不包括\0,而且并不是eventName指向的内存空间的总长,但是保证加上\0也一定<=eventNameSize
	__u16               textDescLen;    //同上
	__u8*				eventName;
	__u8*				textDesc;
	eit_short_desc_t*	next;
}eit_short_desc_t;


typedef struct EIT_EXTENTED_DESC_TYPE	eit_ext_desc_t;
typedef struct EIT_EXTENTED_DESC_TYPE
{
	__u8			langCode[3];
	__u8			descNumber;
	__u8			lastDescNumber;  // for extend desc 
	__u16			descNumberMask;
	__u8*			itemDesc;
	__u8*			itemText;
	__u8*			textChar;
	eit_ext_desc_t*	next;
}eit_ext_desc_t;


typedef struct EIT_COMPONENT_DESC_TYPE	eit_component_desc_t;
typedef struct EIT_COMPONENT_DESC_TYPE
{
	__u8					streamType;
	__u8					conponentType;
	__u32					languageCode;
	eit_component_desc_t*	next;
}eit_component_desc_t;


/*ca identity*/
typedef struct EIT_CA_DESC_TYPE	eit_ca_desc_t;
typedef struct EIT_CA_DESC_TYPE
{
	__u16			caSystemId;
	eit_ca_desc_t*	next;
}eit_ca_desc_t;


/*telephone*/
typedef struct EIT_TELEPHONE_DESC_TYPE eit_tele_desc_t;
typedef struct EIT_TELEPHONE_DESC_TYPE
{
	__u8*				contryPrefixChar;
	__u8*				operatorCodeChar;
	__u8*				nationalCodeChar;
	__u8*				coreNumberChar;
	eit_tele_desc_t*	next;	
}eit_tele_desc_t;


typedef struct EIT_EVENT_INFO_TYPE	eit_event_info_t;
typedef struct EIT_EVENT_INFO_TYPE
{
	__u16					eventId;
	__u8					runningStatus;	//* bit 7 : 1- following 0-current ; bit (2~0) :defined as dvb si; 修正: bit 6:1-current,因为oneseg的service的tableid = 0x4e,会同时携带pf和schedule信息,所以要标识.
	__u8					reserved;

	eit_date_time_t			startTime;
	eit_date_time_t			endTime;
	__u32					duration;
	
	eit_content_desc_t*		eitContentDesc;
	eit_pr_desc_t*			eitPrdesc;		//* parent rating descriptor
	eit_short_desc_t*		eitShtDesc;
	eit_ext_desc_t*			eitExtDesc;
	
#ifdef PARSING_EXTRA_DESC_EN
	eit_component_desc_t*	eitComponentDesc;
	eit_ca_desc_t*			eitCaDesc;
	eit_tele_desc_t*		eitTeleDesc;
#endif

	eit_event_info_t*		last;
	eit_event_info_t*		next;
	
}eit_event_info_t;


typedef struct EIT_SERVICE_INFO_TYPE eit_service_info_t;
typedef struct EIT_SERVICE_INFO_TYPE
{
	__u16				serviceId;
	__u8				pfCurSectionMask;
	__u8				pfVersionNumber;

	eit_event_info_t*	pfEventHeader;  //isdb的oneseg的service的epg是用pid = 0x26/0x27的包传输的,tableid = 0x4e, 而且section2会携带schedule信息.这些schedule信息也放在这里.

	__u16				schdlCurSectionNum[EIT_SCHEDULE_TABLE_ID_NUM];
	__u8				schdlVersionNumber[EIT_SCHEDULE_TABLE_ID_NUM];
	__u8				schdlSectionMask[EIT_SCHEDULE_TABLE_ID_NUM][32];

	eit_event_info_t*	schdlEventHeader[EIT_SCHEDULE_TABLE_ID_NUM];    //暂时不区分extendedSchedule
	eit_service_info_t* next;

}eit_service_info_t;    //解析后的结果


typedef struct EIT_TP_TYPE	eit_tp_t;
typedef struct EIT_TP_TYPE
{
	__u32				origNetworkId;	//* sometime it will be tpId.
	__u16				tsId;
	eit_service_info_t* eitServiceInfoHeader;
	eit_tp_t*			next;
}eit_tp_t;


__s32 eit_parser_clean(void);

#if 0
__s32 eit_parser_parse_section(__u32 tpId, __u8* section, __u32 length);
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
							 __u32 eventInfoLen);

__s32 eit_parser_get_schedule(__u32 tpId, __u16 tsId, __u16 origNetworkId, __u16 serviceId, maple_schdl_list_t* schdlList, __u32 maxEventNum);

__s32 eit_parser_get_pf(__u32 tpId, __u16 tsId, __u16 origNetworkId, __u16 serviceId, maple_sepg_t* pf);

#endif

