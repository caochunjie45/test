
#include "isdb_eit_pool.h"
#include "isdb_eit_parser.h"

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
    __u32      len; //表示除去自己这个数据结构,剩余的可使用的buffer的长度, 本content指向的内容所占据的长度.
    __u8       addr[1];
}content_t;


typedef struct SERVICE_TYPE
{
    __u32       tpId;
    __u16       origNetworkId;
    __u16       tsId;
    __u16       serviceId;
	__u16		age;                //用于比较时间的,当要强制删除serivce时,最老的service被删除.
    
    content_t*  heitPfList;         //* HEit p/f, pid = 0x12, table id = 0x4e
    content_t*  heitSchdlList;      //* HEit schedule basic, pid = 0x12, table id = 0x50 ~ 0x57
    content_t*  heitExtendedList;   //* HEit schedule extended, pid = 0x12, table id = 0x58 ~ 0x5f
    content_t*  meitList;           //* MEit p/f and schedule, pid = 0x26, table id = 0x4e
    content_t*  leitList;           //* LEit p/f and schedule, pid = 0x27, table id = 0x4e
    
    __u8        pfVersionNumber;
    __u8        pfSectionNumMask;
    
    __u8        meitVersionNumber;
    __u8        leitVersionNumber;
    
    __u8        meitSectionNumMask[32];
    __u8        leitSectionNumMask[32];
    
    __u8        schdlVersionNumber[EIT_SCHEDULE_TABLE_ID_NUM];
//    __u8        extendedVersionNumber[EIT_SCHEDULE_TABLE_ID_NUM];
    
    __u8        schdlSectionNumMask[EIT_SCHEDULE_TABLE_ID_NUM][32];
//    __u8        extendedSectionNumMask[EIT_SCHEDULE_TABLE_ID_NUM][32];
    
}service_t; //解析前的存放eit section的数据结构.


typedef struct EIT_POOL_TYPE
{
    __u8*            buf;   //eit pool的buffer的起始地址,这片buffer给下面的成员service和freeList划分
    __u32            bufLen;    //buffer总长
    
    active_service_t activeService[EIT_POOL_MAX_ACTIVE_SERVICE_NUM];
    __u32            activeServiceCount;
    
    service_t*       service;       //现在占据buf的起始地址,参见eit_pool_init(),长度为EIT_MAX_SERVICE_NUM*sizeof(service_t);
    service_t*       curService;    //记录上次存储的eit表所属于的service.
    
    content_t*       freeList;      //占据buf的剩余部分.应该是被service_t的内部成员使用.
    
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
									__u8  sectionNumber,
									eit_type_e eit_type);
static void eit_pool_reset_service_by_tid(service_t* service, __u16 table_id, eit_type_e eit_type);

static __s32 eit_pool_is_version_update(service_t *service, __u8 tableId, __u8 versionNumber, eit_type_e eit_type);
static __s32 eit_pool_is_section_found(service_t *service, __u8 tableId, __u8 sectionNumber, eit_type_e eit_type);
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
/*******************************************************************************
Function name: eit_pool_alloc_content
Description: 
    1.分配一个content节点,能够容纳一个eit section的 完整的eventInfo,不解析.
Parameters: 
    len: eventInfoLen + sizeof(event_head_t);
Return: 
    
Time: 2011/2/21
*******************************************************************************/
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
	size        = ((len + sizeof(content_t) + 3)) & 0xfffffffc; //这里是保证4字节对齐.

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
			newNode->next = NULL;

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
/*******************************************************************************
Function name: eit_pool_find_service
Description: 
    找一个service.
    找不到,就根据isAllocFree来处理.
Parameters: 
    serivce:待查的service数组
    serviceNum:service数组总个数
    tpId等:查找的serivce的特征
    isAllocFree:1表示如果没找到就分一个.
Return: 
    找到的service指针.
Time: 2011/2/21
*******************************************************************************/
static service_t* eit_pool_find_service(service_t* service, 
										__u32 serviceNum, 
										__u32 tpId, 
										__u16 orgNetworkId, 
										__u16 tsId, 
										__u16 serviceId,
										__s32 isAllocFree /* if not find, alloc a new node*/)
{
	__u32 i;
	__s32 free = -1;    //找到的service数组中空闲的一个元素的下标号
	
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
	if (service->heitPfList != NULL)
		eit_pool_free_content_list(&gEitPool.freeList, service->heitPfList);
    
    //* free schedule basic list
	if (service->heitSchdlList != NULL)
		eit_pool_free_content_list(&gEitPool.freeList, service->heitSchdlList);
    
    //* free schedule extended list
	if (service->heitExtendedList != NULL)
		eit_pool_free_content_list(&gEitPool.freeList, service->heitExtendedList);
    
    //* free meit list
	if (service->meitList != NULL)
		eit_pool_free_content_list(&gEitPool.freeList, service->meitList);
    
    //* free leit list
	if (service->leitList != NULL)
		eit_pool_free_content_list(&gEitPool.freeList, service->leitList);

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

	for (i=0; i<serviceNum; i++)    //找一个最老的nonActive servcie，同时确定最老的Active service.实在不行就删除Active serivce
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
	service->tpId			        = SI_INVALID_ID;
	service->origNetworkId	        = SI_INVALID_ID;
#endif
	service->tsId			        = SI_INVALID_ID;
	service->serviceId		        = SI_INVALID_ID;
	service->age			        = 0xffff;
    
	service->heitPfList		        = NULL;
	service->heitSchdlList	        = NULL;
	service->heitExtendedList       = NULL;
	service->meitList               = NULL;
	service->leitList               = NULL;
    
	service->pfVersionNumber        = SI_EIT_INVALID_VERSION_NUMBER;
	service->pfSectionNumMask       = 0;
	service->meitVersionNumber      = SI_EIT_INVALID_VERSION_NUMBER;
	service->meitSectionNumMask[0]  = 0;
	service->meitSectionNumMask[1]  = 0;
	service->meitSectionNumMask[2]  = 0;
	service->meitSectionNumMask[3]  = 0;
	service->leitVersionNumber      = SI_EIT_INVALID_VERSION_NUMBER;
	service->leitSectionNumMask[0]  = 0;
	service->leitSectionNumMask[1]  = 0;
	service->leitSectionNumMask[2]  = 0;
	service->leitSectionNumMask[3]  = 0;
	memset(service->schdlVersionNumber, SI_EIT_INVALID_VERSION_NUMBER, EIT_SCHEDULE_TABLE_ID_NUM);
	memset(service->schdlSectionNumMask, 0, EIT_SCHEDULE_TABLE_ID_NUM*32);
	//memset(service->extendedVersionNumber, SI_EIT_INVALID_VERSION_NUMBER, EIT_SCHEDULE_TABLE_ID_NUM);
	//memset(service->extendedSectionNumMask, 0, EIT_SCHEDULE_TABLE_ID_NUM*32);
}

/*
* reset the service's pf or sch info by table_id
*/
static void eit_pool_reset_service_by_tid(service_t* service, __u16 table_id, eit_type_e eit_type)
{
	if (service == NULL)
		return;
    
	if(eit_type == eit_type_heit)
	{
	    if (IS_PF(table_id))
	    {
	    	service->pfVersionNumber  = SI_EIT_INVALID_VERSION_NUMBER;
	    	service->pfSectionNumMask = 0;
	    	service->heitPfList       = NULL;
	    }
	    else if (IS_SCHDL(table_id))
	    {
	    	memset(service->schdlVersionNumber, SI_EIT_INVALID_VERSION_NUMBER, EIT_SCHEDULE_TABLE_ID_NUM);
	    	memset(service->schdlSectionNumMask, 0, EIT_SCHEDULE_TABLE_ID_NUM*32);
	    	service->heitSchdlList = NULL;
	    	//memset(service->extendedVersionNumber, SI_EIT_INVALID_VERSION_NUMBER, EIT_SCHEDULE_TABLE_ID_NUM);
	    	//memset(service->extendedSectionNumMask, 0, EIT_SCHEDULE_TABLE_ID_NUM*32);
	    	service->heitExtendedList = NULL;
	    }
	}
	else if(eit_type == eit_type_meit)
	{
	    service->meitVersionNumber      = SI_EIT_INVALID_VERSION_NUMBER;
	    service->meitSectionNumMask[0]  = 0;
	    service->meitSectionNumMask[1]  = 0;
	    service->meitSectionNumMask[2]  = 0;
	    service->meitSectionNumMask[3]  = 0;
    	service->meitList               = NULL;
	}
	else
	{
	    service->leitVersionNumber      = SI_EIT_INVALID_VERSION_NUMBER;
	    service->leitSectionNumMask[0]  = 0;
	    service->leitSectionNumMask[1]  = 0;
	    service->leitSectionNumMask[2]  = 0;
	    service->leitSectionNumMask[3]  = 0;
    	service->leitList               = NULL;
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
									__u8  sectionNumber,
									eit_type_e eit_type)
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
	
	if(eit_type == eit_type_heit)
	{
        if(IS_PF(tableId))
        {
        	service->pfVersionNumber   = versionNumber;
        	service->pfSectionNumMask |= (1<<bit);
        }
        else if(IS_SCHDL(tableId))
        {
//            if(tableId < 0x58)  //* 0x50 ~ 0x57 is for schedule basic events.
//    	    {
                if((tableId & 0xf) >= EIT_SCHEDULE_TABLE_ID_NUM)
            	{
            		__wrn("table id[0x%x] exceed EIT_SCHEDULE_TABLE_ID_NUM.\n", tableId);
            		return;
            	}
            	service->schdlVersionNumber[tableId & 0xf]		  = versionNumber;
            	service->schdlSectionNumMask[tableId & 0xf][seg] |= (1<<bit);
//    		}
//    		else    //* 0x58 ~ 0x5f is for schedule extended events.
//    		{
//                if((tableId & 0x7) >= EIT_SCHEDULE_TABLE_ID_NUM)
//            	{
//            		__wrn("table id[0x%x] extend exceed EIT_SCHEDULE_TABLE_ID_NUM.\n", tableId);
//            		return;
//            	}
//            	service->extendedVersionNumber[tableId & 0x7]		  = versionNumber;
//            	service->extendedSectionNumMask[tableId & 0x7][seg] |= (1<<bit);
//    		}
        }
	}
	else if(eit_type == eit_type_meit)
	{
#if 1
        if(sectionNumber >= 4)  //* meit has 4 sections at most.
           __msg("meit has more than 4 sections, section number = %d.\n", sectionNumber);
#endif
	    if(tableId != 0x4e)
	    {
            __msg("meit table id != 0x4e.\n");
            return;
        }
        
        service->meitVersionNumber        = versionNumber;
        service->meitSectionNumMask[seg] |= (1<<bit);
	}
	else
	{
#if 1
        if(sectionNumber >= 3)  //* leit has 4 sections at most.
            __wrn("leit has more than 3 sections, section number = %d.\n", sectionNumber);
#endif
	    if(tableId != 0x4e)
	    {
            __msg("leit table id != 0x4e.\n");
            return;
        }
        
        service->leitVersionNumber        = versionNumber;
        service->leitSectionNumMask[seg] |= (1<<bit);
	}

    return;
}


/*
 * check if the service need update
 */
/*******************************************************************************
Function name: eit_pool_is_version_update
Description: 
    1.查看version number是否有变化.正常的做法是version number比之前的更大(回头算更大)才认为是有变化.
    在实际广播中,一般是递增.而TS流不停回放的话,就难说了.如果是ts流回放,一般情况下,也遇不到version number
    变化.
Parameters: 
    
Return: 
    1:有变化
    0:无变化.
Time: 2011/2/21
*******************************************************************************/
static __s32 eit_pool_is_version_update(service_t *service, __u8 tableId, __u8 versionNumber, eit_type_e eit_type)
{
	if (service == NULL)
		return 0;
	
	//*check version number
	if(eit_type == eit_type_heit)
	{
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
//    	    if(tableId < 0x58)  //* 0x50 ~ 0x57 is for schedule basic events.
//    	    {
    		    if (versionNumber != service->schdlVersionNumber[tableId&0x0F]
    			    && service->schdlVersionNumber[tableId&0xf] != SI_EIT_INVALID_VERSION_NUMBER)
    		    {
    			    __msg("sch basic -- pre version: %d, new version: %d\n", service->schdlVersionNumber[tableId&0xf], versionNumber);
    			    return 1;
    		    }
//    		}
//    		else    //* 0x58 ~ 0x5f is for schedule extended events.
//    		{
//    		    if (versionNumber != service->extendedVersionNumber[(tableId&0x0f) - 8]
//    			    && service->extendedVersionNumber[(tableId&0xf) - 8] != SI_EIT_INVALID_VERSION_NUMBER)
//    		    {
//    			    __msg("sch extended -- pre version: %d, new version: %d\n", service->extendedVersionNumber[(tableId&0xf) - 8], versionNumber);
//    			    return 1;
//    		    }
//    		}
    	}
    }
    else if (eit_type == eit_type_meit)
    {
        if(tableId == 0x4e) //* meit table id must be 0x4e.
        {
    		if (versionNumber != service->meitVersionNumber && service->meitVersionNumber != SI_EIT_INVALID_VERSION_NUMBER)
    		{
    			__msg("meit -- pre version: %d, new version: %d\n",service->meitVersionNumber, versionNumber);
    			return 1;
    		}
        }
    }
    else
    {
        if(tableId == 0x4e) //* meit table id must be 0x4e.
        {
    		if (versionNumber != service->leitVersionNumber && service->leitVersionNumber != SI_EIT_INVALID_VERSION_NUMBER)
    		{
    			__msg("leit -- pre version: %d, new version: %d\n",service->leitVersionNumber, versionNumber);
    			return 1;
    		}
        }
    }
	
	return 0;
}


/*
* check if the service already has the section(section_number)
 */
static __s32 eit_pool_is_section_found(service_t *service, __u8 tableId, __u8 sectionNumber, eit_type_e eit_type)
{
	__u8 seg;
	__u8 bit;
	
	seg = sectionNumber >> 3;
	bit = sectionNumber & 0x7;
	
	if(eit_type == eit_type_heit)
	{
    	if (IS_PF(tableId))
    	{
    		return (service->pfSectionNumMask & (1<<bit)) == 0 ? 0 : 1;
    	}
    	else if (IS_SCHDL(tableId))	//SCH
    	{
    		//return (service->schdlSectionNumMask[tableId&0xf][seg] & (1<<bit)) == 0 ? 0 : 1;
//            if(tableId < 0x58)  //* 0x50 ~ 0x57 is for schedule basic events.
//    	    {
                return (service->schdlSectionNumMask[tableId&0xf][seg] & (1<<bit)) == 0 ? 0 : 1;
//    		}
//    		else    //* 0x58 ~ 0x5f is for schedule extended events.
//    		{
//                return (service->extendedSectionNumMask[tableId&0x7][seg] & (1<<bit)) == 0 ? 0 : 1;
//    		}
    	}
    	else
    		return 1;
	}
	else if(eit_type == eit_type_meit)
	{
	    if(tableId == 0x4e)
	    {
    		return (service->meitSectionNumMask[seg] & (1<<bit)) == 0 ? 0 : 1;
	    }
	    else
	        return 1;
	}
	else
	{
	    if(tableId == 0x4e)
	    {
    		return (service->leitSectionNumMask[seg] & (1<<bit)) == 0 ? 0 : 1;
	    }
	    else
	        return 1;
	}
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

    
    //eitBuf = (__u8*)malloc(bufSize);
    eitBuf = (__u8*)BALLOC(bufSize);
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


/*******************************************************************************
Function name: eit_pool_release
Description: 
    1.清除解析前存放的EIT section.
Parameters: 
    
Return: 
    
Time: 2011/2/22
*******************************************************************************/
__s32 eit_pool_release(void)
{
	if(gEitPool.buf != NULL)
	{   
		//free(gEitPool.buf);
		BFREE(gEitPool.buf, gEitPool.bufLen);
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


__s32 eit_pool_append_section(__u32 tpId, __u8* section, __s32 sectionLen, eit_type_e eitType)
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
		
	if(eitType == eit_type_heit)
	{
        if(!(IS_SCHDL(section[0]) || IS_PF(section[0])))
            return -1;
    
        //* 0x50 ~ 0x50 + EIT_SCHEDULE_TABLE_ID_NUM for schedule basic
        //* 0x58 ~ 0x58 + EIT_SCHEDULE_TABLE_ID_NUM for schedule extended
    	if (IS_SCHDL(section[0]) && (section[0] & 0x7) >= ISDB_EIT_SCHEDULE_TABLEID_NUM)    //就在这里过滤掉不要的eit表,之后的代码就不再管了.
    	{
    		__msg("sch table id flow!\n");
    		return -1;
    	}
	}
	else
	{
	    if(section[0] != 0x4e)
	    {
	        __wrn("%s table id != 0x4e\n", eitType == eit_type_meit ? "meit" : "leit");
	        return -1;
	    }
	}
	

	if (eit_pool_get_section_info(section, (__u16)sectionLen, &tableId, &tsId, &origNetworkId, &serviceId, &versionNumber, 
									&curNextIdct, &sectionNumber, &lastSectionNumber, &eventInfoOffset, &eventInfoLen) == 0)
	{
//		__wrn("table_id: 0x%X, ts_id: 0x%X, service_id: 0x%X, section_number: %d, event_info_len: %d\n",
//				tableId, tsId, serviceId, sectionNumber, eventInfoLen);
		
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
					if (eit_pool_is_service_active(tpId, origNetworkId, tsId, serviceId))   //如果eit属于的service是active的,那么就必须释放其他的service的epg信息,空出空间来解析这个service的eit表
					{
						service  = eit_pool_force_alloc_service(gEitPool.service, EIT_MAX_SERVICE_NUM);
						if (service == NULL)
						{
							__msg("eit_pool_force_alloc_service fail! It shouldn't run to here.\n");
							return -1;
						}
						else
						{
//							__wrn("non-active service %d, ignored!\n", serviceId);
//							return -1;
                            __wrn("force alloc node for serviceId[%d]!\n", serviceId);

						}
					}
					else
					{
						__msg("can not find service and alloc service, it shouldn't run to here.\n");
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
			if(eit_pool_is_version_update(service, tableId, versionNumber, eitType))
			{
				__msg("version update for service %x.\n", serviceId);
				
				if(eitType == eit_type_heit)
				{
				    if(IS_PF(tableId))
				    	eit_pool_free_content_list(&gEitPool.freeList, service->heitPfList);
				    else if(IS_SCHDL(tableId))
				    {
				    	eit_pool_free_content_list(&gEitPool.freeList, service->heitSchdlList);
				    	eit_pool_free_content_list(&gEitPool.freeList, service->heitExtendedList);
				    }
				}
				else if(eitType == eit_type_meit)
				{
				    eit_pool_free_content_list(&gEitPool.freeList, service->meitList);
				}
				else
				{
				    eit_pool_free_content_list(&gEitPool.freeList, service->leitList);
				}
				
				//* reset pf or schedule or meit or leit list.
				eit_pool_reset_service_by_tid(service, tableId, eitType);
			}
			else
			{
				//* 3. check if this section exist.
				if(eit_pool_is_section_found(service, tableId, sectionNumber, eitType))
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
			
			if(eitType == eit_type_heit)
			{
			    if (IS_PF(tableId))
			    	eit_pool_add_content_list(&service->heitPfList, content);
			    else if(IS_SCHDL(tableId))
			    {
			        if((section[0] & 0xf) < ISDB_EIT_SCHEDULE_TABLEID_NUM)
			    	    eit_pool_add_content_list(&service->heitSchdlList, content);
			    	else
			    	    eit_pool_add_content_list(&service->heitExtendedList, content);
			    }
			}
			else if(eitType == eit_type_meit)
			{
			    eit_pool_add_content_list(&service->meitList, content);
			}
			else
			{
			    eit_pool_add_content_list(&service->leitList, content);
			}
			
			//* copy
			eventHead.tableId       = tableId;
			eventHead.versionNumber = versionNumber;
			eventHead.eventInfoLen  = eventInfoLen;
			eventHead.sectionNumber = sectionNumber;
			memcpy(content->addr, &eventHead, sizeof(eventHead));
			memcpy(content->addr + sizeof(eventHead), section + eventInfoOffset, eventInfoLen);
			
			eit_pool_update_service(service, tpId, origNetworkId, tsId, serviceId, tableId, versionNumber, sectionNumber, eitType);
			
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
		if (type == epg_type_pf)
		{
            if(service->heitPfList != NULL)
                content = service->heitPfList;
            else if(service->meitList != NULL)
                content = service->meitList;
            else
                content = service->leitList;
                
		    while (content != NULL)
		    {
		    	eventHead = (event_head_t*)content->addr;
		    	if(eventHead->sectionNumber == 0 /* present */ || eventHead->sectionNumber == 1 /* following */)
		    	    eit_parser_parse_event(tpId, tsId, origNetworkId, serviceId, eventHead->tableId, 
		    							    eventHead->versionNumber, 1 /* current next indicator*/, eventHead->sectionNumber,
		    							    content->addr + sizeof(event_head_t), eventHead->eventInfoLen);
		    	content = content->next;
		    }
		}
		else
		{
            if(service->heitSchdlList != NULL)
            {
                //* for heit, currently we only parse schedule basic information, ignore the schedule extended list.
                content = service->heitSchdlList;
                
			    while (content != NULL)
			    {
			    	eventHead = (event_head_t*)content->addr;
			    	eit_parser_parse_event(tpId, tsId, origNetworkId, serviceId, eventHead->tableId, 
			    							eventHead->versionNumber, 1 /* current next indicator*/, eventHead->sectionNumber,
			    							content->addr + sizeof(event_head_t), eventHead->eventInfoLen);
			    	content = content->next;
			    }
            }
            else if(service->meitList != NULL)
            {
                content = service->meitList;
                
			    while (content != NULL)
			    {
			    	eventHead = (event_head_t*)content->addr;
		    	    if(eventHead->sectionNumber != 0 && eventHead->sectionNumber != 1)  //* discard present/following
			    	    eit_parser_parse_event(tpId, tsId, origNetworkId, serviceId, eventHead->tableId, 
			    							    eventHead->versionNumber, 1 /* current next indicator*/, eventHead->sectionNumber,
			    							    content->addr + sizeof(event_head_t), eventHead->eventInfoLen);
			    	content = content->next;
			    }
            }
            else
            {
                content = service->leitList;
                
			    while (content != NULL)
			    {
			    	eventHead = (event_head_t*)content->addr;
		    	    if(eventHead->sectionNumber != 0 && eventHead->sectionNumber != 1)  //* discard present/following
			    	    eit_parser_parse_event(tpId, tsId, origNetworkId, serviceId, eventHead->tableId, 
			    							    eventHead->versionNumber, 1 /* current next indicator*/, eventHead->sectionNumber,
			    							    content->addr + sizeof(event_head_t), eventHead->eventInfoLen);
			    	content = content->next;
			    }
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
