#ifndef __MEM_H_
#define __MEM_H_

#include "beetles_app.h"



#define SIZE 256



//��̬�ڴ���������ṹ��
typedef struct
{
    void* pointer;      //���뵽���ڴ��ַ
    int size;           //�ڴ���С
    const char* file;   //�ļ���
    int line;           //�ļ��к�
    
}MItem;





void *mallocEx(__size_t size, const char* file, const int line);

void freeEx(void *ptr,__size_t size);

void Leak_Info(void);



#define In_Malloc(n)  mallocEx(n, __FILE__, __LINE__)

#define In_Free(ptr,size)    freeEx(ptr,size)





#endif


