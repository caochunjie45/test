/*
*******************************************************************************************************************
*                                                         	Listmenu  Framework
*                                         			the display dirver support module
*
*                             				 Copyright(C), 2006-2008, SoftWinners Microelectronic Co., Ltd.
*											               All Rights Reserved
*
*File Name�� 		txtbuffer.h
*
*Author�� 		    William Wang
*
*Version :   		1.1.0
*
*Date:  			2008-12-18
*
*Description : 	    txtbox framebuffer definition 
*
*Others : 		    None at present.
*
* History :
*
* <Author>          <time>          <version>     <description>
*
* William Wang     2008-12-18         1.1.0          Create File
*
*******************************************************************************************************************
*/
#ifndef __TXTBUFFER_H__
#define __TXTBUFFER_H__

#include "txtbox_e.h"

//------------------------------------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------------------------------------
typedef struct tag_TXTBUFFER_MAN
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
}txtbuffer_man_t;

txtbuffer_man_t*    TxtBuffer_Create(void);
__u32               TxtBuffer_Destroy(txtbuffer_man_t* txtbuffer);

//------------------------------------------------------------------------------------------------------------
// THE END ! 
//------------------------------------------------------------------------------------------------------------




#endif
