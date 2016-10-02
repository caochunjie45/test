
#include "dvbt2_eit_pool.h"
#include "dvbt2_eit_parser.h"

//******************************** SEPERATOR ********************************//
//******************************* type define *******************************//
//***************************************************************************//

#define EIT_POOL_EPG_GARBAGE_LEN_THRESHOLD		20

typedef struct EVENT_HEAD_TYPE
{
	__u8  tableId:7;
	__u8  versionNumber:5;
	//*   __u8 curNextIdct:1;
	__u16 eventInfoLen:12;
	__u8  sectionNumber;
}event_head_t;


typedef struct CONTENT_TYPE content_t;
typedef struct CONTENT_TYPE
{
    content_t* next;
    __u32      len;
    __u8       addr[1];
}content_t;


typedef struct SERVICE_TYPE
{
    __u32       tpId;
    __u16       origNetworkId;
    __u16       tsId;
    __u16       serviceId;
    __u8        pfVersionNumber;
    __u8        pfSectionNumMask;
    __u8        schdlVersionNumber[EIT_SCHEDULE_TABLE_ID_NUM];
    __u8        schdlSectionNumMask[EIT_SCHEDULE_TABLE_ID_NUM][32];
    
	__u16		age;

    content_t*  pfList;
    content_t*  schdlList;
}service_t;


typedef struct EIT_POOL_TYPE
{
    __u8*            buf;
    __u32            bufLen;
    
    active_service_t activeService[EIT_POOL_MAX_ACTIVE_SERVICE_NUM];
    __u32            activeServiceCount;
    
    service_t*       service;
    service_t*       curService;
    
    content_t*       freeList;
    
}eit_pool_t;


static eit_pool_t gEitPool;

//******************************** SEPERATOR ********************************//
//************************* local function prototype ************************//
//***************************************************************************//
static content_t* eit_pool_alloc_content(content_t **head, __u32 len);
static void eit_pool_add_content_list(content_t **head, content_t *node);
static __s32 eit_pool_defrag_content(content_t **head, content_t *node);
static void eit_pool_free_content_list(content_t **head, content_t *list);

static service_t* eit_pool_find_service(service_t* service, 
										__u32 serviceNum, 
										__u32 tpId, 
										__u16 orgNetworkId, 
										__u16 tsId, 
										__u16 serviceId,
										__s32 isAllocFree /* if not find, alloc a new node*/);
static void eit_pool_reset_service (service_t* service);
static service_t* eit_pool_force_alloc_service(service_t *service, __s32 serviceNum);
static __s32 eit_pool_force_free_a_service_except_this(service_t* service);
static void eit_pool_update_service(service_t* service,
									__u32 tpId,
									__u16 origNetworkId,
									__u16 tsId,
									__u16 serviceId,
									__u8  tableId,
									__u8  versionNumber,
									__u8  sectionNumber);
static void eit_pool_reset_service_by_tid(service_t* service, __u16 table_id);

static __s32 eit_pool_is_version_update(service_t *service, __u8 tableId, __u8 versionNumber);
static __s32 eit_pool_is_section_found(service_t *service, __u8 tableId, __u8 sectionNumber);
static __inline __s32 eit_pool_get_section_info(__u8*  section, 
												__u16  dataLen, 
												__u8*  tableId,
												__u16* tsId, 
												__u16* origNetworkId,
												__u16* serviceId, 
												__u8*  versionNumber, 
												__u8*  curNextInd, 
												__u8*  sectionNumber,
												__u8*  lastSectionNumber, 
												__u16* infoOffset, 
												__u16* infoLength);

//******************************** SEPERATOR ********************************//
//***************************** function define *****************************//
//***************************************************************************//

/*
 * Alloc a content, it includes both content_struct and section_data area.
 */
static content_t* eit_pool_alloc_content(content_t **head, __u32 len)
{
	content_t* pre;
	content_t* newNode;
	content_t* preNewNode;
	content_t* node;
	__u32      lenLimit1;
	__u32      lenLimit2;
	__u32      size;

	pre		= NULL;
	newNode = NULL;
	preNewNode = NULL;
	node		= *head;

	lenLimit1	= len + EIT_POOL_EPG_GARBAGE_LEN_THRESHOLD;
	lenLimit2   = len + 2*EIT_POOL_EPG_GARBAGE_LEN_THRESHOLD;
	size        = ((len + sizeof(content_t) + 3)) & 0xfffffffc;

	while (node != NULL)
	{
		//* check if len
		if(node->len >= len)
		{
			if (node->len <= lenLimit1)
			{
				//* delete the node from the list
				if (pre == NULL)
					*head = node->next;
				else
					pre->next = node->next;

				node->next = NULL;

				return node;
			}
			else if (newNode == NULL || node->len < newNode->len)	//* get the shortest one
			{
				newNode		= node;
				preNewNode	= pre;
			}
		}

		pre  = node;
		node = node->next;
	}

	if (newNode != NULL)
	{
		//* newNode >= len + 2*threshold, divide it
		if (newNode->len >= lenLimit2)
		{
			node = newNode;

			node->len   -= size;
			newNode		 = (content_t*)(node->addr + node->len);
			newNode->len = size - sizeof(content_t);

			return newNode;
		}
		else	//* len + threshold < newNode->len < len + 2*threshold
		{
			//* delete the node from the list.
			if (preNewNode == NULL)
				*head = newNode->next;
			else
				preNewNode->next = newNode->next;

			newNode->next = NULL;

			return newNode;
		}
	}
	
	__msg("allocate content node fail!\n");
	return NULL;
}


/*
 * add the content node to the list
 */
static void eit_pool_add_content_list(content_t **head, content_t *node)
{
	node->next = (*head);
	*head = node;
}


/*
 * defrag a content node with the free list
 */
static __s32 eit_pool_defrag_content(content_t **head, content_t *node)
{
	content_t* list;
	content_t* pre;
	__u32      addr;

	list = *head;
	pre  = NULL;
	
	if (list == NULL || node == NULL)
		return -1;
	
	addr = (__u32)(node->addr + node->len);
	while (list != NULL)
	{
		if ((__u32)(list->addr + list->len) == (__u32)node)
		{
			// list <-- node
			list->len += (sizeof(content_t) + node->len);
			return 0;
		}
		else if (addr == (__u32)list)
		{
			node->len += (sizeof(content_t) + list->len);
			node->next = list->next;
			if (pre == NULL)
				*head = node;
			else
				pre->next = node;
			
			return 0;
		}
		
		pre  = list;
		list = list->next;
	}
	
	__msg("defrag failed, node: %x\n", node);
	return -1;
}


/*
 * defrag the free list
 */
static void eit_pool_free_content_list(content_t **head, content_t *list)
{
	content_t *node;
	content_t *next;

	node = list;
	
	while (node != NULL)
	{
		next = node->next;
		node->next = NULL;
		
		if (eit_pool_defrag_content(head, node) != 0)
			eit_pool_add_content_list(head, node);

		node = next;
	}
}


/*
 * Find the service node whose id is serviceId in the list.
 * If not found and isAllocFree == 1, then alloc a new node for the service.
 */
static service_t* eit_pool_find_service(service_t* service, 
										__u32 serviceNum, 
										__u32 tpId, 
										__u16 orgNetworkId, 
										__u16 tsId, 
										__u16 serviceId,
										__s32 isAllocFree /* if not find, alloc a new node*/)
{
	__u32 i;
	__s32 free = -1;
	
	if(service == NULL)
		return NULL;
	
	for (i=0; i<serviceNum; i++)
	{
		if(service[i].serviceId == serviceId
			&& service[i].tsId == tsId
#ifndef EIT_SAME_NETWORK
			// && service[i].origNetworkId == orgNetworkId
			&& service[i].tpId == tpId
#endif
			)
		{
			return &service[i];
		}
		else if (isAllocFree == 1
			&& free == -1
			&& service[i].serviceId == SI_INVALID_ID
			&& service[i].tsId == SI_INVALID_ID
#ifndef EIT_SAME_NETWORK
			&& service[i].origNetworkId == SI_INVALID_ID
			&& service[i].tpId == SI_INVALID_ID
#endif
			)
		{
			free = i;
		}
	}
	
	if(isAllocFree == 1 && free != -1)
	{
	    __msg("alloc service %d, tpId = %d, networkId = %d, tsId = %d, serviceId = %d.\n", 
	                free, tpId, orgNetworkId, tsId, serviceId);
		return &service[free];
	}
	else
		return NULL;
}

/*
 * free a service
 */
static void eit_pool_free_service(service_t *service)
{
	if (service == NULL)
		return;

	//* free its pf_list and sch_list
	if (service->pfList != NULL)
		eit_pool_free_content_list(&gEitPool.freeList, service->pfList);
	if (service->schdlList != NULL)
		eit_pool_free_content_list(&gEitPool.freeList, service->schdlList);

	//* reset service
	eit_pool_reset_service(service);
}

/*
 * force to alloc a non-free service node from the service list(non-active first, then active one)
 */
static service_t* eit_pool_force_alloc_service(service_t *service, __s32 serviceNum)
{
	__s32	   i;
	service_t* oldActive;
	service_t* oldNonActive;
	
	oldActive    = NULL;
	oldNonActive = NULL;

	for (i=0; i<serviceNum; i++)
	{
#ifndef EIT_SAME_NETWORK
		if(eit_pool_is_service_active(service[i].tpId, service[i].origNetworkId, service[i].tsId, service[i].serviceId))
#else
		if(eit_pool_is_service_active(0, 0, service[i].tsId, service[i].serviceId))
#endif
		{
			if (oldActive == NULL || service[i].age > oldActive->age)
				oldActive = &service[i];
		}
		else
		{
			if(oldNonActive == NULL || service[i].age > oldNonActive->age)
				oldNonActive = &service[i];
		}
	}

	if (oldNonActive != NULL)
	{
		eit_pool_free_service(oldNonActive);
		return oldNonActive;
	}
	else if (oldActive != NULL)
	{
		eit_pool_free_service(oldActive);
		return oldActive;
	}
	else
	{
		__msg("error: no service can be forced to be alloced!!!\n");
		__msg("-- it should not come here! --\n");
		return NULL;
	}
}

/*
 * force to free a service (!= service, non-active first, then active one)
 */
static __s32 eit_pool_force_free_a_service_except_this(service_t* service)
{
	__s32		i;
	service_t*	oldActive;
	service_t*  oldNonActive;

	oldActive	 = NULL;
	oldNonActive = NULL;

	for (i=0; i<EIT_MAX_SERVICE_NUM; i++)
	{
		if (gEitPool.service[i].serviceId != SI_INVALID_ID && service != &gEitPool.service[i])
		{
#ifndef EIT_SAME_NETWORK
			if(eit_pool_is_service_active(gEitPool.service[i].tpId, gEitPool.service[i].origNetworkId, gEitPool.service[i].tsId, gEitPool.service[i].serviceId))
#else
			if(eit_pool_is_service_active(0, 0, gEitPool.service[i].tsId, gEitPool.service[i].serviceId))
#endif
			{
				if (oldActive == NULL || gEitPool.service[i].age > oldActive->age)
					oldActive = &gEitPool.service[i];
			}
			else
			{
				if (oldNonActive == NULL || gEitPool.service[i].age > oldNonActive->age)
					oldNonActive = &gEitPool.service[i];
			}
		}
	}

	if (oldNonActive != NULL)
	{
		__msg("free non-active service %x\n", oldNonActive);
		eit_pool_free_service(oldNonActive);
		return 0;
	}
	else if (oldActive != NULL)
	{
		__msg("free active service %x\n",oldActive);
		eit_pool_free_service(oldActive);
		
		return 0;
	}
	else
	{
		__msg("free service failed!!!\n");
		return -1;
	}
}

/*
 * reset the service's all pf and sch info
 */
static void eit_pool_reset_service (service_t* service)
{
	if (service == NULL)
		return;

#ifndef EIT_SAME_NETWORK
	service->tpId			  = SI_INVALID_ID;
	service->origNetworkId	  = SI_INVALID_ID;
#endif
	service->tsId			  = SI_INVALID_ID;
	service->serviceId		  = SI_INVALID_ID;
	service->pfVersionNumber  = SI_EIT_INVALID_VERSION_NUMBER;
	service->pfSectionNumMask = 0;
	service->age			  = 0xffff;
	service->pfList			  = NULL;
	service->schdlList		  = NULL;
	memset(service->schdlVersionNumber, SI_EIT_INVALID_VERSION_NUMBER, EIT_SCHEDULE_TABLE_ID_NUM);
	memset(service->schdlSectionNumMask, 0, EIT_SCHEDULE_TABLE_ID_NUM*32);
}

/*
* reset the service's pf or sch info by table_id
*/
static void eit_pool_reset_service_by_tid(service_t* service, __u16 table_id)
{
	if (service == NULL)
		return;
	
	if (IS_PF(table_id))
	{
		service->pfVersionNumber  = SI_EIT_INVALID_VERSION_NUMBER;
		service->pfSectionNumMask = 0;
		service->pfList			  = NULL;
	}
	else if (IS_SCHDL(table_id))
	{
		memset(service->schdlVersionNumber, SI_EIT_INVALID_VERSION_NUMBER, EIT_SCHEDULE_TABLE_ID_NUM);
		memset(service->schdlSectionNumMask, 0, EIT_SCHEDULE_TABLE_ID_NUM*32);
		service->schdlList = NULL;
	}
}

/*
 * update the service's info such as service_id, version_number and section_number
 */
static void eit_pool_update_service(service_t* service,
									__u32 tpId,
									__u16 origNetworkId,
									__u16 tsId,
									__u16 serviceId,
									__u8  tableId,
									__u8  versionNumber,
									__u8  sectionNumber)
{
	__u8 seg;
	__u8 bit;

	if(service == NULL)
		return;

#ifndef EIT_SAME_NETWORK
	service->tpId = tpId;
	service->origNetworkId = origNetworkId;
#endif

	service->tsId = tsId;
	service->serviceId = serviceId;

	seg = sectionNumber>>3;
	bit = sectionNumber & 0x7;

	if(IS_PF(tableId))
	{
		service->pfVersionNumber   = versionNumber;
		service->pfSectionNumMask |= (1<<bit);
	}
	else if(IS_SCHDL(tableId))
	{
		if((tableId & 0xf) >= EIT_SCHEDULE_TABLE_ID_NUM)
		{
			__err("table id exceed EIT_SCHEDULE_TABLE_ID_NUM.\n");
			return;
		}
		service->schdlVersionNumber[tableId & 0xf]		  = versionNumber;
		service->schdlSectionNumMask[tableId & 0xf][seg] |= (1<<bit);
	}
}


/*
 * check if the service need update
 */
static __s32 eit_pool_is_version_update(service_t *service, __u8 tableId, __u8 versionNumber)
{
	if (service == NULL)
		return 0;
	
	//*check version number
	if (IS_PF(tableId))
	{
		if (versionNumber != service->pfVersionNumber && service->pfVersionNumber != SI_EIT_INVALID_VERSION_NUMBER)
		{
			__msg("pf -- pre version: %d, new version: %d\n",service->pfVersionNumber, versionNumber);
			return 1;
		}
	}
	else if (IS_SCHDL(tableId))
	{
		if (versionNumber != service->schdlVersionNumber[tableId&0x0F]
			&& service->schdlVersionNumber[tableId&0xf] != SI_EIT_INVALID_VERSION_NUMBER)
		{
			__msg("sch -- pre version: %d, new version: %d\n", service->schdlVersionNumber[tableId&0xf], versionNumber);
			return 1;
		}
	}
	
	return 0;
}


/*
* check if the service already has the section(section_number)
 */
static __s32 eit_pool_is_section_found(service_t *service, __u8 tableId, __u8 sectionNumber)
{
	__u8 seg;
	__u8 bit;
	
	seg = sectionNumber >> 3;
	bit = sectionNumber & 0x7;
	
	if (IS_PF(tableId))
	{
		return (service->pfSectionNumMask & (1<<bit)) == 0 ? 0 : 1;
	}
	else if (IS_SCHDL(tableId))	//SCH
	{
		return (service->schdlSectionNumMask[tableId&0xf][seg] & (1<<bit)) == 0 ? 0 : 1;
	}
	else
		return 1;
}

static __inline __s32 eit_pool_get_section_info(__u8*  section, 
												__u16  dataLen, 
												__u8*  tableId,
												__u16* tsId, 
												__u16* origNetworkId,
												__u16* serviceId, 
												__u8*  versionNumber, 
												__u8*  curNextInd, 
												__u8*  sectionNumber,
												__u8*  lastSectionNumber, 
												__u16* infoOffset, 
												__u16* infoLength)
{
	__u16 sectionLen;

	sectionLen = 0;

	sectionLen = (section[1] & 0x0f)<<8 | section[2];

	*tableId			= section[0];
	*serviceId			= section[3]<<8 | section[4];
	*tsId				= section[8]<<8 | section[9];
	*origNetworkId		= section[10]<<8 | section[11];

	*curNextInd			= section[5] & 0x01;
	*versionNumber		= (section[5] >> 1) & 0x1f;
	*sectionNumber		= section[6];
	*lastSectionNumber	= section[7];

	*infoOffset			= 14;
	*infoLength			= sectionLen - (11 + 4);	//* not include the crc.

	if(sectionLen > (dataLen - 3))
	{
		*infoLength = dataLen - 14;
		return -1;
	}

	return 0;

}


__s32 eit_pool_init(__u32 bufSize)
{
    __u8* eitBuf;
    __u32 pos;
    __u32 i;

    eitBuf = (__u8*)malloc(bufSize);
    if(eitBuf == NULL)
    {
        __msg("alloc buffer for eit pool fail, buffer size = %d.\n", bufSize);

		gEitPool.buf				= NULL;
		gEitPool.bufLen				= 0;
		gEitPool.activeServiceCount = 0;
		gEitPool.service			= NULL;
		gEitPool.curService			= NULL;
		gEitPool.freeList			= NULL;

		return -1;
    }
	else
	{
		gEitPool.buf				= eitBuf;
		gEitPool.bufLen				= bufSize;
		gEitPool.activeServiceCount	= 0;
		gEitPool.service			= (service_t*)gEitPool.buf;
		gEitPool.curService			= NULL;

		for(i=0; i<EIT_MAX_SERVICE_NUM; i++)
			eit_pool_reset_service(&gEitPool.service[i]);

		pos						= EIT_MAX_SERVICE_NUM*sizeof(service_t);
		gEitPool.freeList		= (content_t*)(gEitPool.buf + pos);
		gEitPool.freeList->len	= gEitPool.bufLen - pos - sizeof(content_t);
		gEitPool.freeList->next = NULL;

		return 0;
	}
}


__s32 eit_pool_release(void)
{
	if(gEitPool.buf != NULL)
	{
		free(gEitPool.buf);

		gEitPool.buf				= NULL;
		gEitPool.bufLen				= 0;
		gEitPool.activeServiceCount = 0;
		gEitPool.service			= NULL;
		gEitPool.curService			= NULL;
		gEitPool.freeList			= NULL;
	}

	return 0;
}


void eit_pool_reset(void)
{
	__u32 pos;
	__s32 i;

	if(gEitPool.buf != NULL)
	{
		gEitPool.curService = NULL;

		for (i=0; i<EIT_MAX_SERVICE_NUM; i++)
			eit_pool_reset_service(&gEitPool.service[i]);

		pos = EIT_MAX_SERVICE_NUM * sizeof(service_t);

		gEitPool.freeList		= (content_t*)(gEitPool.buf + pos);
		gEitPool.freeList->len	= gEitPool.bufLen - pos - sizeof(content_t);
		gEitPool.freeList->next = NULL;
	}
}


void  eit_pool_set_active_services(active_service_t* service, __u32 count)
{
	if (count > EIT_POOL_MAX_ACTIVE_SERVICE_NUM)
	{
		__wrn("set active services: count %d > max_count %d\n", count, EIT_POOL_MAX_ACTIVE_SERVICE_NUM);
		count = EIT_POOL_MAX_ACTIVE_SERVICE_NUM;
	}

	if (service == NULL)
		gEitPool.activeServiceCount = 0;
	else
	{
		memcpy(gEitPool.activeService, service, count*sizeof(active_service_t));
		gEitPool.activeServiceCount = count;
	}
}


__s32 eit_pool_is_service_active(__u32 tpId, __u16 orgNetworkId, __u16 tsId, __u16 serviceId)
{
	__u32 i;
	for (i=0; i<gEitPool.activeServiceCount; i++)
	{
		if(serviceId == gEitPool.activeService[i].serviceId
			&& tsId == gEitPool.activeService[i].tsId
#ifndef EIT_SAME_NETWORK
			// && orgNetworkId == gEitPool.activeService[i].orgNetworkId
			&& tpId == gEitPool.activeService[i].tpId
#endif
			)
			return 1;
	}

	return 0;
}


__s32 eit_pool_append_section(__u32 tpId, __u8* section, __s32 sectionLen)
{
	__s32		 i;
	__u8		 tableId;
	__u16		 serviceId;
	__u16		 tsId;
	__u16		 origNetworkId;
	__u8		 versionNumber;
	__u8		 curNextIdct;
	__u8		 lastSectionNumber;
	__u8		 sectionNumber;
	__u16		 eventInfoOffset;
	__u16		 eventInfoLen;
	service_t*	 service;
	content_t*	 content;
	event_head_t eventHead;

	service = NULL;
	content = NULL;

	if (section == NULL || sectionLen <= 0)
		return -1;
		
    if(!(IS_SCHDL(section[0]) || IS_PF(section[0])))
        return -1;

	if (IS_SCHDL(section[0]) && (section[0] & 0xf) >= EIT_SCHEDULE_TABLE_ID_NUM)
	{
		__msg("sch table id flow!");
		return -1;
	}

	if (eit_pool_get_section_info(section, (__u16)sectionLen, &tableId, &tsId, &origNetworkId, &serviceId, &versionNumber, 
									&curNextIdct, &sectionNumber, &lastSectionNumber, &eventInfoOffset, &eventInfoLen) == 0)
	{
		__msg("table_id: 0x%X, ts_id: 0x%X, service_id: 0x%X, section_number: %d, event_info_len: %d\n",
				tableId, tsId, serviceId, sectionNumber, eventInfoLen);
		
		if (eventInfoLen > 0 && curNextIdct == 1)
		{
			//* 1. check if service exists.
			if(gEitPool.curService != NULL 
				&& gEitPool.curService->serviceId == serviceId 
				&& gEitPool.curService->tsId == tsId
#ifndef EIT_SAME_NETWORK
				// gEitPool.curService->origNetworkId == origNetworkId
				&& gEitPool.curService->tpId == tpId
#endif
				)
			{
				__msg("current service hit!\n");
				service = gEitPool.curService;
			}
			else
			{
				__msg("current service not hit!\n");
				
				service = eit_pool_find_service(gEitPool.service, EIT_MAX_SERVICE_NUM, tpId, origNetworkId, tsId, serviceId, 1);
				if (service == NULL)
				{
					if (eit_pool_is_service_active(tpId, origNetworkId, tsId, serviceId))
					{
						service  = eit_pool_force_alloc_service(gEitPool.service, EIT_MAX_SERVICE_NUM);
						if (service == NULL)
						{
							__err("eit_pool_force_alloc_service fail! It shouldn't run to here.\n");
							return -1;
						}
						else
						{
							__wrn("non-active service %d, ignored!\n", serviceId);
							return -1;
						}
					}
					else
					{
						__err("can not find service and alloc service, it shouldn't run to here.\n");
						return -1;
					}
				}
				
				gEitPool.curService = service;
				
				//* age++
				for (i=0; i<EIT_MAX_SERVICE_NUM; i++)
				{
					if (gEitPool.service[i].age < gEitPool.curService->age)
						gEitPool.service[i].age++;
				}
				gEitPool.curService->age = 0;
			}
			
			//* 2. check if version update.
			if(eit_pool_is_version_update(service, tableId, versionNumber))
			{
				__msg("version update for service %x.\n", serviceId);
				
				if(IS_PF(tableId))
					eit_pool_free_content_list(&gEitPool.freeList, service->pfList);
				else if(IS_SCHDL(tableId))
					eit_pool_free_content_list(&gEitPool.freeList, service->schdlList);
				
				//* reset pf or schedule
				eit_pool_reset_service_by_tid(service, tableId);
			}
			else
			{
				//* 3. check if this section exist.
				if(eit_pool_is_section_found(service, tableId, sectionNumber))
				{
				    __msg("section found.\n");
					return -1;
				}
			}
			
			//* 4. alloc content node to store this section of data.
			do 
			{
				content = eit_pool_alloc_content(&gEitPool.freeList, eventInfoLen + sizeof(eventHead));
				
				if (content == NULL)
				{
					__msg("alloc content failed! so do free some service!\n");
					
					if (eit_pool_is_service_active(tpId, origNetworkId, tsId, serviceId))
					{
						if (eit_pool_force_free_a_service_except_this(service) == -1)
						{
							__wrn("force free service fail, reset eit pool.\n");
							eit_pool_reset();
							return -1;
						}
					}
					else
					{
						__wrn("can't alloc content for this non-active service %d, ignore this section!\n", serviceId);
						return -1;
					}
				}
			} while (content == NULL);
			
			if (IS_PF(tableId))
				eit_pool_add_content_list(&service->pfList, content);
			else if(IS_SCHDL(tableId))
				eit_pool_add_content_list(&service->schdlList, content);
			
			//* copy
			eventHead.tableId = tableId;
			eventHead.versionNumber = versionNumber;
			eventHead.eventInfoLen  = eventInfoLen;
			eventHead.sectionNumber = sectionNumber;
			memcpy(content->addr, &eventHead, sizeof(eventHead));
			memcpy(content->addr + sizeof(eventHead), section + eventInfoOffset, eventInfoLen);
			
			eit_pool_update_service(service, tpId, origNetworkId, tsId, serviceId, tableId, versionNumber, sectionNumber);
			
			return 0;
		}
		else
		{
			__msg("event_info_len: %d, cur_next_idct: %d!\n", eventInfoLen, curNextIdct);
			__msg("service: %x, table: %x, section: %d\n", serviceId, tableId, sectionNumber);
		}
	}
	else
		__wrn("eit_pool_get_section_info fail.\n");

	return -1;
}


__s32 eit_pool_parse_content(__u32 tpId, __u16 origNetworkId, __u16 tsId, __u16 serviceId, epg_type_e type)
{
	service_t*		service;
	content_t*		content;
	event_head_t*	eventHead;

	service = eit_pool_find_service(gEitPool.service, EIT_MAX_SERVICE_NUM, tpId, origNetworkId, tsId, serviceId, 0);
	if (service != NULL)
	{
		if (type & epg_type_pf)
		{
			content = service->pfList;
			while (content != NULL)
			{
				eventHead = (event_head_t*)content->addr;
				eit_parser_parse_event(tpId, tsId, origNetworkId, serviceId, eventHead->tableId, 
										eventHead->versionNumber, 1 /* current next indicator*/, eventHead->sectionNumber,
										content->addr + sizeof(event_head_t), eventHead->eventInfoLen);
				content = content->next;
			}
		}

		if (type & epg_type_sch)
		{
			content = service->schdlList;
			while (content != NULL)
			{
				eventHead = (event_head_t*)content->addr;
				eit_parser_parse_event(tpId, tsId, origNetworkId, serviceId, eventHead->tableId, 
										eventHead->versionNumber, 1 /* current next indicator*/, eventHead->sectionNumber,
										content->addr + sizeof(event_head_t), eventHead->eventInfoLen);
				content = content->next;
			}
		}
	}
	else
	{
		__msg("can not find service in eit pool.\n");
	}

	return 0;
}


#if 0
__s32 eit_pool_save_to_file(char* filePath)
{
    return 0;
}


__s32 eit_pool_load_from_file(char* filePath)
{
    return 0;
}


__s32 eit_pool_parse_content_from_file(char* filePath, __u32 tpId, __u16 origNetworkId, __u16 tsId, 
									   __u16 serviceId, epg_type_e type, __u32 startDate, __u32 endDate)
{
	return 0;
}
#endif
