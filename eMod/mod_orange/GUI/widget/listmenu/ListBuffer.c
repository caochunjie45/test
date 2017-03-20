
#ifndef __LIST__BUFFER__C____
#define __LIST__BUFFER__C____

#include "config.h"
#include "ListBuffer.h"
#include "sysmemory.h"

//------------------------------------------------------------------------------------------------------------
//����ListBuffer������ռ�	 
//------------------------------------------------------------------------------------------------------------
__hdle ListBuffer_Create(void)
{
    LIST_BUFFER_MAN_t  *pList_Buffer;

    pList_Buffer = (LIST_BUFFER_MAN_t *)MEM_MALLOC(sizeof(LIST_BUFFER_MAN_t));
    if(!pList_Buffer)
    {
       return NULL;
    }    

    return (__hdle)pList_Buffer;
    
}

//------------------------------------------------------------------------------------------------------------
//����ListBuffer�����տռ�	 
//------------------------------------------------------------------------------------------------------------
__u32 ListBuffer_Destroy(__hdle listbuffer)
{
    LIST_BUFFER_MAN_t  *pList_Buffer;

    pList_Buffer = (LIST_BUFFER_MAN_t *)listbuffer;
    if(listbuffer)
    {
      MEM_FREE(pList_Buffer);
    }
    else
    {
	   return 0;
    }

    return 0;
}

//------------------------------------------------------------------------------------------------------------
// THE END ! 
//------------------------------------------------------------------------------------------------------------



#endif //__LIST__BUFFER__C____


