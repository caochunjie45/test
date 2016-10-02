#ifndef __MEM_H_
#define __MEM_H_

#include "beetles_app.h"



#define SIZE 256



//动态内存申请参数结构体
typedef struct
{
    void* pointer;      //申请到的内存地址
    int size;           //内存块大小
    const char* file;   //文件名
    int line;           //文件行号
    
}MItem;





void *mallocEx(__size_t size, const char* file, const int line);

void freeEx(void *ptr,__size_t size);

void Leak_Info(void);



#define In_Malloc(n)  mallocEx(n, __FILE__, __LINE__)

#define In_Free(ptr,size)    freeEx(ptr,size)





#endif


