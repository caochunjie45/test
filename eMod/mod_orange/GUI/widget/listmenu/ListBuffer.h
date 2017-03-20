
#ifndef __LIST__BUFFER__H____
#define __LIST__BUFFER__H____


#include "config.h"

//------------------------------------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------------------------------------
typedef struct tag_LIST_BUFFER_MAN
{
    __u8                    mode;
    __u32                   size;
    __rectsz_t			    rectsz;
    __fb_format_t           layer_format;
    void *                  buffer;
    void *                  ebuffer;    /*�����framebuffer��ַ*/
    __u32                   esize;      /*�����framebuffer��С*/
    __u32                   vsize;      /*��Ч��framebuffer��С*/    
    __rect_t                vrect;              /*valid rect*/
}LIST_BUFFER_MAN_t;

__hdle  ListBuffer_Create(void);
__u32   ListBuffer_Destroy(__hdle listbuffer);

//------------------------------------------------------------------------------------------------------------
// THE END ! 
//------------------------------------------------------------------------------------------------------------



#endif //__LIST__BUFFER__H____


