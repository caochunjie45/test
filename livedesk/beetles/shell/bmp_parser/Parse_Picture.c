/*
*************************************************************************************
*                         			eGon
*					        Application Of eGon2.0
*
*				(c) Copyright 2006-2010, All winners Co,Ld.
*							All	Rights Reserved
*
* File Name 	: Parse_Picture.c
*
* Author 		: javen
*
* Description 	: ͼƬ����
*
* History 		:
*      <author>    		<time>       	<version >    		<desc>
*       javen     	   2010-09-10          1.0            create this file
*
*************************************************************************************
*/

#include  "..\\Esh_shell_private.h"
#include  "Parse_Picture.h"
#include  "bmp.h"

#define spcl_size_align( x, y )         ( ( (x) + (y) - 1 ) & ~( (y) - 1 ) )
#define abs(x) (x) >= 0 ? (x):-(x)
/*
*******************************************************************************
*                     Parse_Pic_BMP_ByBuffer
*
* Description:
*    ����������ڴ��е�ͼƬ
*
* Parameters:
*    Pic_Buffer     :  input.  ���ͼƬ�����
*    Pic_BufferSize :  input.  ��������С
*    PictureInfo    :  output. ͼƬ���������Ϣ
*
* Return value:
*    void
*
* note:
*    void
*
*******************************************************************************
*/
__s32 Parse_Pic_BMP_ByBuffer(void *Pic_Buffer, __u32 Pic_BufferSize, Picture_t *PictureInfo)
{
	bmp_info_head_t *info_head_p = NULL;

    info_head_p = (bmp_info_head_t *)((__u32)Pic_Buffer + sizeof(bmp_file_head_t));

    PictureInfo->BitCount = info_head_p->biBitCount;
	PictureInfo->Width    = info_head_p->biWidth;
	PictureInfo->Height	  = abs(info_head_p->biHeight);
	PictureInfo->RowSize  = spcl_size_align( info_head_p->biWidth * ( info_head_p->biBitCount >> 3 ), 4 );

	PictureInfo->Buffer     = (void *)((__u32)Pic_Buffer + sizeof(bmp_info_head_t) + sizeof(bmp_file_head_t));
	PictureInfo->BufferSize = Pic_BufferSize - (sizeof(bmp_info_head_t) + sizeof(bmp_file_head_t));

	return 0;
}

/*
*******************************************************************************
*                     Parse_Pic_BMP
*
* Description:
*    ��·����������ͼƬ, ���Ұѽ��������ͼƬ������ָ���ĵ�ַ��
* ���ָ���ĵ�ַΪNULL, ����Դ�����κε�ַ��
*
* Parameters:
*    Path           :  input.  ͼƬ·��
*    PictureInfo    :  output. ͼƬ���������Ϣ
*    Addr			:  input.  ��Ž������ͼƬ, 
*
* Return value:
*    void
*
* note:
*    void
*
*******************************************************************************
*/
__s32 Parse_Pic_BMP_ByPath(char *Path, Picture_t *PictureInfo, __u32 Addr)
{
    HBMP_i_t hbmp_buf;
    HBMP  hbmp = NULL;
    
    eLIBs_memset(&hbmp_buf, 0, sizeof(HBMP_i_t));
    hbmp = open_bmp(Path, &hbmp_buf);
    if(hbmp == NULL){
        __wrn("ERR: open_bmp failed\n");
        return -1;
    }

    PictureInfo->BitCount = get_bitcount(hbmp);
	PictureInfo->Width    = get_width(hbmp);
	PictureInfo->Height	  = get_height(hbmp);
	PictureInfo->RowSize  = get_rowsize(hbmp);

	PictureInfo->BufferSize = PictureInfo->RowSize * PictureInfo->Height;
	if(Addr){
		PictureInfo->Buffer = (void *)Addr;
	}else{
		PictureInfo->Buffer = (void *)esMEMS_Balloc(PictureInfo->BufferSize);
	}
	if(PictureInfo->Buffer == NULL){
		__wrn("ERR: wboot_malloc failed\n");
        goto error;
	}

	eLIBs_memset(PictureInfo->Buffer, 0, PictureInfo->BufferSize);

	get_matrix(hbmp, PictureInfo->Buffer);

	close_bmp(hbmp);
	hbmp = NULL;

    return 0;

error:
	close_bmp(hbmp);
	
	return -1;
}

