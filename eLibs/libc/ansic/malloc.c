/*
*********************************************************************************************************
*                                                    eMOD
*                                   the Easy Portable/Player Operation System
*                                               mod_mmp sub-system
*                                               red malloc module
*
*                                    (c) Copyright 2008-2009, kevin.z China
*                                              All Rights Reserved
*
* File   : red_malloc.c
* Version: V1.0
* By     : kevin
* Date   : 2008-11-20 16:08
* Note   : this module is for algorithm lib, because they need request memory resource in there,
*          module, but when the task is killed by otherone, not themselves, the memory resource
*          will be lost. so, we need record the memory request, and when kill the task, we can
*          check which memory was not release, we can release it forcibly.
*********************************************************************************************************
*/
#include "elibs_i.h"
#include "malloc.h"

#define RECORD_MOD_HEAP_SIZE        (0)

typedef struct __RED_BUF_NODE
{
    __u32       addr;       //the address of the buffer malloc from system heap
    __u32       size;       //the size of the memory block

    struct __RED_BUF_NODE  *prev;     //prev pointer, point to front node
    struct __RED_BUF_NODE  *next;     //next pointer, point to next node

} __red_buf_node_t;

static __red_buf_node_t      RedHeapHead={0};       //heap header
static __red_buf_node_t      RedPhyHeapHead={0};    //physic heap header
static __s32 bheap_init = 0;
static __s32 bphy_heap_init = 0;


#if(RECORD_MOD_HEAP_SIZE)
static __s32   maxHeapSize = 0;
static __s32   curCurHeapSize = 0;
#endif

#define bGarbage 0xA3
//#define RED_DEBUG

/*
*********************************************************************************************************
*                           INIT RED PHYSIC HEAP
*
* Description: init red heap;
*
* Arguments  : none;
*
* Returns    : EPDK_OK/FAIL
*********************************************************************************************************
*/
__s32 RED_InitHeap(void)
{
    if(bheap_init)
    {
        return EPDK_OK;
    }
    
    RedHeapHead.addr = 0;
    RedHeapHead.size = 0;
    RedHeapHead.prev = &RedHeapHead;    //prev pointer point to the last node
    RedHeapHead.next = 0;

#if(RECORD_MOD_HEAP_SIZE)
    maxHeapSize = 0;
    curCurHeapSize = 0;
#endif

    bheap_init = 1;

    return EPDK_OK;
}

/*
*********************************************************************************************************
*                           INIT RED PHYSIC HEAP
*
* Description: init red physic heap;
*
* Arguments  : none;
*
* Returns    : EPDK_OK/FAIL
*********************************************************************************************************
*/
__s32 RED_InitPhyHeap(void)
{
    if(bphy_heap_init)
    {
        return EPDK_OK;
    }
    
    RedPhyHeapHead.addr = 0;
    RedPhyHeapHead.size = 0;
    RedPhyHeapHead.prev = &RedPhyHeapHead; //prev pointer point to the last node
    RedPhyHeapHead.next = 0;

    bphy_heap_init = 1;

    return EPDK_OK;
}

/*
*********************************************************************************************************
*                           EXIT RED PHYSIC HEAP
*
* Description: exit red heap;
*
* Arguments  : none;
*
* Returns    : EPDK_OK/FAIL
*********************************************************************************************************
*/
__s32 RED_ExitHeap(void)
{
    __red_buf_node_t      *tmpBufNode = RedHeapHead.next;
    __red_buf_node_t      *tmpFreeNode;

    if(0 == bheap_init)
    {
        return EPDK_FAIL;
    }

    while(tmpBufNode)
    {
        #if 0
        {
            __u32       fcseid;
	        __asm
	        {
	        	mrc p15, 0, fcseid, c13, c0, 0
            };
            __wrn("Some memory(addr:%x, size:%x) request by malloc has not been free, (mod:%x, pid:%d)!\n",
                tmpBufNode->addr, tmpBufNode->size, &RED_ExitPhyHeap, (fcseid>>25)&0xff);
        }
        #else
        __wrn("Some memory(addr:%x, size:%x) request by malloc has not been free!\n",
            tmpBufNode->addr, tmpBufNode->size);
        #endif

        //free the memory
        esMEMS_Mfree(0, (void *)tmpBufNode->addr);

        //process the pointer
        tmpFreeNode = tmpBufNode;
        tmpBufNode = tmpBufNode->next;
        RedHeapHead.next = tmpBufNode;

        //free the node
        esMEMS_Mfree(0, (void *)tmpFreeNode);
    }

    bheap_init = 0;

    return EPDK_OK;
}


/*
*********************************************************************************************************
*                           EXIT RED PHYSIC HEAP
*
* Description: exit red physic heap;
*
* Arguments  : none;
*
* Returns    : EPDK_OK/FAIL
*********************************************************************************************************
*/
__s32 RED_ExitPhyHeap(void)
{
    __red_buf_node_t      *tmpBufNode = RedPhyHeapHead.next;
    __red_buf_node_t      *tmpFreeNode;

    if(0 == bphy_heap_init)
    {
        return EPDK_FAIL;
    }

    while(tmpBufNode)
    {
        #if 0
        {
            __u32       fcseid;
	        __asm
	        {
	        	mrc p15, 0, fcseid, c13, c0, 0
            };
            __wrn("Some memory(addr:%x, size:%x) request by palloc has not been free, (mod:%x, pid:%d)!\n",
                tmpBufNode->addr, tmpBufNode->size, &RED_ExitPhyHeap, (fcseid>>25)&0xff);
        }
        #else
        __wrn("Some memory(addr:%x, size:%x) request by palloc has not been free!\n",
            tmpBufNode->addr, tmpBufNode->size);
        #endif

        //free the memory
        esMEMS_Pfree((void *)tmpBufNode->addr, tmpBufNode->size);

        //process the pointer
        tmpFreeNode = tmpBufNode;
        tmpBufNode = tmpBufNode->next;
        RedPhyHeapHead.next = tmpBufNode;

        //free the node
        esMEMS_Mfree(0, (void *)tmpFreeNode);
    }

#if(RECORD_MOD_HEAP_SIZE)
    __wrn("======================================================\n");
    __wrn("-------------------MAX HEAP SIZE:%x ------------------\n", maxHeapSize);
    __wrn("======================================================\n");
#endif

    bphy_heap_init = 0;

    return EPDK_OK;
}

__s32 RED_DumpPhyHeap(void)
{
    __red_buf_node_t      *tmpBufNode = RedPhyHeapHead.next;

    esKRNL_SchedLock();
    
    while(tmpBufNode)
    {
        #if 0
        {
            __u32       fcseid;
	        __asm
	        {
	        	mrc p15, 0, fcseid, c13, c0, 0
            };
            __wrn("Some memory(addr:%x, size:%x) request by palloc has not been free, (mod:%x, pid:%d)!\n",
                tmpBufNode->addr, tmpBufNode->size, &RED_ExitPhyHeap, (fcseid>>25)&0xff);
        }
        #else
        __wrn("Some memory(addr:%x, size:%x) request by palloc has not been free!\n",
            tmpBufNode->addr, tmpBufNode->size);
        #endif

        //free the memory
        //esMEMS_Pfree((void *)tmpBufNode->addr, tmpBufNode->size);

        //process the pointer
       
        //tmpFreeNode = tmpBufNode;
        tmpBufNode = tmpBufNode->next;
        //RedPhyHeapHead.next = tmpBufNode;

        //free the node
        //esMEMS_Mfree(0, (void *)tmpFreeNode);
    }

#if(RECORD_MOD_HEAP_SIZE)
    __wrn("======================================================\n");
    __wrn("-------------------MAX HEAP SIZE:%x ------------------\n", maxHeapSize);
    __wrn("======================================================\n");
#endif

    esKRNL_SchedUnlock();

    return EPDK_OK;
}



/*
*********************************************************************************************************
*                       REQUEST MEMORY FROM RED HEAP
*
* Description: Request memory from red heap;
*
* Arguments  : size     the size of memory request;
*
* Returns    : buffer pointer
*                   != 0, request memory successed, return buffer address;
*                    = 0, request memroy failed, return NULL;
*********************************************************************************************************
*/
void *RED_malloc(__s32 size)
{
    __red_buf_node_t      *tmpBufNode;
    __red_buf_node_t      *tmpRequestNode;

    esKRNL_SchedLock();
    if(0 == bheap_init)
    {
        RED_InitHeap();
    }
    esKRNL_SchedUnlock();

    //create a buffer node to record the buffer request
    tmpRequestNode = (__red_buf_node_t *)esMEMS_Malloc(0, sizeof(__red_buf_node_t));
    if(!tmpRequestNode)
    {
        __wrn("Request memory for buffer node failed!\n");
        return 0;
    }
    //clear the node
    tmpRequestNode->addr = 0;
    tmpRequestNode->size = 0;
    tmpRequestNode->prev = 0;
    tmpRequestNode->next = 0;

    //request memory for user
    tmpRequestNode->addr = (__u32)esMEMS_Malloc(0, size);
    if(!tmpRequestNode->addr)
    {
        __wrn("Request memory(size:%x) for user failed!\n", size);
        //release buffer node
        esMEMS_Mfree(0, (void *)tmpRequestNode);
        return 0;
    }

#ifdef RED_DEBUG
    memset(tmpRequestNode->addr, bGarbage, size);
#endif
    tmpRequestNode->size = size;

    //add buffer node to node list, modify global variable, need protect
    esKRNL_SchedLock();
    tmpBufNode = RedHeapHead.prev;
    tmpRequestNode->prev = tmpBufNode;
    tmpBufNode->next = tmpRequestNode;
    RedHeapHead.prev = tmpRequestNode;
    esKRNL_SchedUnlock();

#if(RECORD_MOD_HEAP_SIZE)
    esKRNL_SchedLock();
    curCurHeapSize += size;
    if(maxHeapSize < curCurHeapSize)
    {
        maxHeapSize = curCurHeapSize;
    }
    esKRNL_SchedUnlock();
#endif

    return (void *)tmpRequestNode->addr;
}


/*
*********************************************************************************************************
*                       esMEMS_free MEMORY TO RED HEAP
*
* Description: free memory to red heap, the memory should be requesed from red heap;
*
* Arguments  : pbuf     buffer address which need be free;
*
* Returns    : result
*                   = EPDK_OK, free memory to red heap successed;
*                   = EPDK_FAIL, free memory to red heap failed;
*********************************************************************************************************
*/
__s32 RED_free(void *pbuf)
{
    __red_buf_node_t      *tmpReleaseNode, *tmpBufNode;

    esKRNL_SchedLock();

    tmpReleaseNode = RedHeapHead.next;

    //look for the buffer node by the buffer address
    while(tmpReleaseNode)
    {
        //find the buffer node, free the memory and the buffer node
        if(tmpReleaseNode->addr == (__u32)pbuf)
        {
            //delete the node from the buffer node list
            tmpBufNode = tmpReleaseNode->prev;
            tmpBufNode->next = tmpReleaseNode->next;
            tmpBufNode = tmpReleaseNode->next;
            if(tmpBufNode)
            {
                tmpBufNode->prev = tmpReleaseNode->prev;
            }
            else
            {
                //reach the list tail, modify header pointer
                RedHeapHead.prev = tmpReleaseNode->prev;
            }

#if(RECORD_MOD_HEAP_SIZE)
            curCurHeapSize -= tmpReleaseNode->size;
#endif
           //free memory requested by user
            esMEMS_Mfree(0, (void *)tmpReleaseNode->addr);
            //free buffer node
            esMEMS_Mfree(0, (void *)tmpReleaseNode);

            esKRNL_SchedUnlock();
            return EPDK_OK;
        }

        tmpReleaseNode = tmpReleaseNode->next;
    }

    esKRNL_SchedUnlock();

    eLIBs_printf("Free memory(addr:%x) failed!\n", pbuf);
    return EPDK_FAIL;
}



/*
*********************************************************************************************************
*                       REQUEST MEMORY FROM RED PHYSIC HEAP
*
* Description: Request memory from red physic heap;
*
* Arguments  : size     the size of memory request;
*              align    align size for physic malloc
*
* Returns    : buffer pointer
*                   != 0, request memory successed, return buffer address;
*                    = 0, request memroy failed, return NULL;
*********************************************************************************************************
*/
void *RED_phymalloc(__s32 size, __s32 align)
{
    __s32                   tmpPageSize;
    __red_buf_node_t      *tmpBufNode;
    __red_buf_node_t      *tmpRequestNode;

    esKRNL_SchedLock();
    if(0 == bphy_heap_init)
    {
        RED_InitPhyHeap();
    }
    esKRNL_SchedUnlock();

    //create a buffer node to record the buffer request
    tmpRequestNode = (__red_buf_node_t *)esMEMS_Malloc(0, sizeof(__red_buf_node_t));
    if(!tmpRequestNode)
    {
        __wrn("Request memory for buffer node failed!\n");
        return 0;
    }
    //clear the node
    tmpRequestNode->addr = 0;
    tmpRequestNode->size = 0;
    tmpRequestNode->prev = 0;
    tmpRequestNode->next = 0;
   
    tmpRequestNode->addr = (__u32)esMEMS_Balloc(size);
        
    if(!tmpRequestNode->addr)
    {
        __wrn("Request memory(size:%x) by palloc for user failed!\n", size);
        //release buffer node
        esMEMS_Mfree(0, (void *)tmpRequestNode);
        return 0;
    }
#ifdef RED_DEBUG
    memset(tmpRequestNode->addr, bGarbage, size);
#endif
    tmpRequestNode->size = size;

    //add buffer node to node list
    esKRNL_SchedLock();
    tmpBufNode = RedPhyHeapHead.prev;
    tmpRequestNode->prev = tmpBufNode;
    tmpBufNode->next = tmpRequestNode;
    RedPhyHeapHead.prev = tmpRequestNode;
    esKRNL_SchedUnlock();

    __inf(" addr=0x%xb", tmpRequestNode->addr);

    return (void *)tmpRequestNode->addr;
}


/*
*********************************************************************************************************
*                       esMEMS_free MEMORY TO RED PHYSIC HEAP
*
* Description: free memory to red physic heap, the memory should be requesed from red physic heap;
*
* Arguments  : pbuf     buffer address which need be free;
*
* Returns    : result
*                   = EPDK_OK, free memory to red heap successed;
*                   = EPDK_FAIL, free memory to red heap failed;
*********************************************************************************************************
*/
__s32 RED_phyfree(void *pbuf)
{
    __red_buf_node_t      *tmpReleaseNode, *tmpBufNode;

    esKRNL_SchedLock();
    tmpReleaseNode = RedPhyHeapHead.next;

    //look for the buffer node by the buffer address
    while(tmpReleaseNode)
    {
        //find the buffer node, free the memory and the buffer node
        if(tmpReleaseNode->addr == (__u32)pbuf)
        {
            //delete the node from the buffer node list
            tmpBufNode = tmpReleaseNode->prev;
            tmpBufNode->next = tmpReleaseNode->next;
            tmpBufNode = tmpReleaseNode->next;
            if(tmpBufNode)
            {
                tmpBufNode->prev = tmpReleaseNode->prev;
            }
            else
            {
                //reach the list tail, modify header pointer
                RedPhyHeapHead.prev = tmpReleaseNode->prev;
            }

            //free memory requested by user
            esMEMS_Bfree((void *)tmpReleaseNode->addr, tmpReleaseNode->size);

            //free buffer node
            esMEMS_Mfree(0, (void *)tmpReleaseNode);
            esKRNL_SchedUnlock();
            return EPDK_OK;
        }

        tmpReleaseNode = tmpReleaseNode->next;
    }
    esKRNL_SchedUnlock();

    eLIBs_printf("Free memory(addr:%x) failed!\n", pbuf);
    return EPDK_FAIL;
}

void *RED_phyrealloc(void *f, __s32 size, __s32 align)
{
    __s32 src_size;
    void* p;
    __red_buf_node_t      *tmpBufNode;

    esKRNL_SchedLock();
    if(0 == bphy_heap_init)
    {
        RED_InitPhyHeap();
    }
    esKRNL_SchedUnlock();
      
    if(NULL == f)
    {
        return RED_phymalloc(size, align);
    }

    p = NULL;
    src_size = -1;
    esKRNL_SchedLock();
    tmpBufNode = RedPhyHeapHead.next;
    while(tmpBufNode)
    {       
        if((__u32)f == tmpBufNode->addr)
        {
            src_size = tmpBufNode->size;
            break;
        }
        
        tmpBufNode = tmpBufNode->next;        
    }
    if(-1 == src_size)
    {
        __wrn("realloc mem fail...\n");
        esKRNL_SchedUnlock();
        return NULL;
    }

    p = RED_phymalloc(size, align);
    if(NULL == p)
    {
        esKRNL_SchedUnlock();
        return NULL;
    }

    SLIB_memcpy(p, f, size<src_size? size:src_size);       

    RED_phyfree(f);

    esKRNL_SchedUnlock();

    return p;
}

void *RED_phycalloc(__s32 size, __s32 align)
{
    void* p;

    p = RED_phymalloc(size, align);
    if(NULL == p)
    {
        return NULL;
    }

    SLIB_memset(p, 0, size);
    
    return p;
}

