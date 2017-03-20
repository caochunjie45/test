

#include "mem.h"

#define SIZE 256


static MItem g_record[SIZE]; //��¼ÿ����̬�ڴ�����Ĳ���



void* mallocEx(__size_t size, const char* file, const int line)
{
    int i = 0;
    
    void* ptr = esMEMS_Balloc(size);//��̬�ڴ�����

	eLIBs_memset(ptr,0,size);
    
    if(ptr != NULL)
    {
        //����ɹ������¼����
        
        //����ȫ�����飬��¼�˴β���
        for(i = 0; i< SIZE; i++)
        {
            //����λ��
            if(g_record[i].pointer == NULL)
            {
                g_record[i].pointer = ptr;
                g_record[i].size    = size;
                g_record[i].file    = file;
                g_record[i].line    = line;

			#if 0
             	eDbug("malloc memery -> Address:%p, size:%d, Location:%s:%d \n",
                      g_record[i].pointer,
                      g_record[i].size,
                      g_record[i].file,
                      g_record[i].line);
			#endif
			
                break;
            }
        }
    }
    
    return ptr;
}



void freeEx(void *ptr,__size_t size)
{
    if(ptr != NULL)
    {
        int i = 0;

        //����ȫ�����飬�ͷ��ڴ�ռ䣬�����������¼
        for(i = 0; i< SIZE; i++)
        {
            if(g_record[i].pointer == ptr)
            {

                esMEMS_Bfree(ptr,g_record[i].size);

			#if 0
             	eDbug("free memery -> Address:%p, size:%d, Location:%s:%d \n",
                      g_record[i].pointer,
                      g_record[i].size,
                      g_record[i].file,
                      g_record[i].line);
			#endif
			
                g_record[i].pointer = NULL;
                g_record[i].size = 0;
                g_record[i].file = NULL;
                g_record[i].line = 0;
                
                break;
            }
        }
    }
}


void Leak_Info(void)
{
    int i = 0;

    //����ȫ�����飬��ӡδ�ͷŵĿռ�������¼
    for(i = 0; i< SIZE; i++)
    {
        //����λ��
        if(g_record[i].pointer != NULL)
        {
             eDbug("have not free memery -> Address:%p, size:%d, Location:%s:%d \n",
                      g_record[i].pointer,
                      g_record[i].size,
                      g_record[i].file,
                      g_record[i].line);
         }
     }

     eDbug("memery check over \n");
}


