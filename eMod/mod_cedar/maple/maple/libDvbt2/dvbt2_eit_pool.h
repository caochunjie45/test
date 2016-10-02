
#ifndef EIT_POOL_H
#define EIT_POOL_H

#include "dvbt2_interface.h"

//* for dvb-c and dvb-t, define this micro, for dvb-s, undefine it.
#define	EIT_SAME_NETWORK

#define EIT_POOL_BUFFER_SIZE	            0x80000
#define EIT_MAX_SERVICE_NUM					96
#define EIT_POOL_MAX_ACTIVE_SERVICE_NUM     40//10
#define EIT_SCHEDULE_TABLE_ID_NUM           2

#define SI_INVALID_ID						0xFFFF
#define SI_EIT_INVALID_VERSION_NUMBER		0xFF

#if 0
#define IS_SCHDL(tid)		(((tid&0xf0) == 0x50)||((tid&0xf0) == 0x60))
#define IS_PF(tid)			((tid == 0x4e)||(tid == 0x4f))
#else
#define IS_SCHDL(tid)		((tid&0xf0) == 0x50)
#define IS_PF(tid)			(tid == 0x4e)
#endif


typedef enum
{
	epg_type_none = 0,
	epg_type_pf   = 0x01,
	epg_type_sch  = 0x02,
	epg_type_both = 0x03,	//epg_type_pf | epg_type_sch
}epg_type_e;


typedef struct ACTIVE_SERVICE_TYPE
{
    __u32 tpId;
    __u16 origNetworkId;
    __u16 tsId;
    __u16 serviceId;
}active_service_t;

__s32 eit_pool_init(__u32 bufSize);

__s32 eit_pool_release(void);

void  eit_pool_reset(void);

void  eit_pool_set_active_services(active_service_t* service, __u32 count);

__s32 eit_pool_is_service_active(__u32 tpId, __u16 orgNetworkId, __u16 tsId, __u16 serviceId);

__s32 eit_pool_append_section(__u32 tpId, __u8* section, __s32 sectionLen);

__s32 eit_pool_parse_content(__u32 tpId, __u16 origNetworkId, __u16 tsId, __u16 serviceId, epg_type_e type);

#if 0
__s32 eit_pool_save_to_file(char* filePath);

__s32 eit_pool_load_from_file(char* filePath);

__s32 eit_pool_parse_content_from_file(char* filePath, __u32 tpId, __u16 origNetworkId, __u16 tsId, 
									   __u16 serviceId, epg_type_e type, __u32 startDate, __u32 endDate);
#endif

#endif

