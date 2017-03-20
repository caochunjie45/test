/*
************************************************************************************************************************
*                                                       SFTE 2.0
*
*                             Copyright(C), 2006-2009, SoftWinners Microelectronic Co., Ltd.
*											       All Rights Reserved
*
* File Name   : sft_decode.c
*
* Author      : Bayden.Chen
*
* Version     : 1.1.0
*
* Date        : 2009.11.23
*
* Description :
* 
* Others      : None at present.
*
*
* History     :
*
*  <Author>        <time>       <version>      <description>
*
* Gary.Wang      2009.11.23       1.1.0        build the file
*
************************************************************************************************************************
*/
#ifndef __sft_decode_c
#define __sft_decode_c

#include "epdk.h"

#pragma pack(1)

#define SFT_GET_REAL_SIZE(n)	(n&(~(((__u32)0x01)<<31)))
#define SFT_GET_COMPRESS_FLAG(n)	((n>>31)&0x01)
#define SFT_MIN(a, b)	(a) < (b) ? (a):(b)
#define SFT_ABS(a)	(a) > 0 ? (a):(-a)
#define g_fopen eLIBs_fopen
#define g_fclose eLIBs_fclose
#define g_fread eLIBs_fread
#define g_fseek eLIBs_fseek
#define g_ftell eLIBs_ftell
#define g_strncmp eLIBs_strncmp
#define g_memcpy eLIBs_memcpy
#define g_memset eLIBs_memset
#define g_memcmp eLIBs_memcmp
#define g_strcpy eLIBs_strcpy

typedef struct
{
	char head[8];
	__s32 block_size_start_offset;
	__s32 data_start_offset;
	__s32 file_size;//�ļ���ѹ�����ܴ�С		
	__s32 block_size;
	__s32 block_cnt;
	__s32 max_compress_block_size;
}sft_c_header_t;

typedef struct  
{
	ES_FILE* fp;
	__s32 type;//0: normal file 1: compress file
	__s32 offset;//ѹ���ļ��ĵ�ǰ�ļ�ָ��λ�ã�������ļ���ʼλ��
	char* pmax_compress_block_buf;
	char* puncompress_block_buf;
	sft_c_header_t sft_c_header;	
}SFT_FILE;

ES_FILE* sft_fopen(const char* file, const char* mode)
{
	sft_c_header_t sft_header;
	ES_FILE* fp;
	SFT_FILE* sft_fp;
	__s32 read_len;

	fp = NULL;
	sft_fp = NULL;

	fp = g_fopen(file, mode);
	if(!fp)
	{
		goto err;
	}

	read_len = g_fread(&sft_header, 1, sizeof(sft_header), fp);
	if(sizeof(sft_header) != read_len)
	{
		goto err;
	}	
	
	sft_fp = (SFT_FILE*)esMEMS_Malloc(0, sizeof(SFT_FILE));
	if (!sft_fp)
	{
		goto err;
	}
	g_memset(sft_fp, 0, sizeof(SFT_FILE));

	if (0 == g_strncmp("COMPV1.0", sft_header.head, 8))
	{
		g_memcpy(&sft_fp->sft_c_header, &sft_header, sizeof(sft_c_header_t));
		sft_fp->type = 1;
		sft_fp->pmax_compress_block_buf = (char*)esMEMS_Balloc(sft_fp->sft_c_header.max_compress_block_size);
		if(!sft_fp->pmax_compress_block_buf)
		{
			__msg("mem not enough...\n");
			goto err;
		}

		sft_fp->puncompress_block_buf = (char*)esMEMS_Balloc(sft_fp->sft_c_header.block_size);
		if(!sft_fp->puncompress_block_buf)
		{
			__msg("mem not enough...\n");
			goto err;
		}
	}
	else
	{
		sft_fp->type = 0;
	}
	
	sft_fp->fp = fp;		

	g_fseek(fp, 0, SEEK_SET);

	return (ES_FILE*)sft_fp;

err:;
	if(fp)
	{
		g_fclose(fp);
		fp = NULL;
	}
	if (sft_fp)
	{
		if(sft_fp->pmax_compress_block_buf)
		{
			esMEMS_Bfree(sft_fp->pmax_compress_block_buf, sft_fp->sft_c_header.max_compress_block_size);
			sft_fp->pmax_compress_block_buf =NULL;
		}
		if(sft_fp->puncompress_block_buf)
		{
			esMEMS_Bfree(sft_fp->puncompress_block_buf, sft_fp->sft_c_header.block_size);
			sft_fp->puncompress_block_buf =NULL;
		}
		esMEMS_Mfree(0, sft_fp);
		sft_fp = NULL;
	}

	return NULL;	
}

__s32 sft_fclose(ES_FILE* fp)
{
	__s32 ret;
	SFT_FILE* sft_fp;

    ret = EPDK_OK;
	sft_fp = (SFT_FILE*)fp;
	if (!sft_fp)
	{
        ret = EPDK_FAIL;
		goto err;
	}

	if (sft_fp->fp)
	{
		g_fclose(sft_fp->fp);
		sft_fp->fp = NULL;
	}

	if(1 == sft_fp->type)
	{
		if(sft_fp->pmax_compress_block_buf)
		{
			esMEMS_Bfree(sft_fp->pmax_compress_block_buf, sft_fp->sft_c_header.max_compress_block_size);
			sft_fp->pmax_compress_block_buf = NULL;
		}
        if(sft_fp->puncompress_block_buf)
		{
			esMEMS_Bfree(sft_fp->puncompress_block_buf, sft_fp->sft_c_header.block_size);
			sft_fp->puncompress_block_buf =NULL;
		}
	}
	
	esMEMS_Mfree(0, sft_fp);

err:;
	return ret;
}

static __s32 __sft_get_block_part(SFT_FILE* fp, __s32 block_index, __s32 inter_offset, char* pbuf, __s32* len)
{
	SFT_FILE* sft_fp;
	
	sft_fp = (SFT_FILE*)fp;
	if (!sft_fp || !sft_fp->fp)
	{
		goto err;
	}

	if (0 == sft_fp->type)
	{
		__err("sft type err...\n");
		goto err;
	}
	else
	{
		__u32 block_start_addr;
		__u32 block_end_addr;
		__s32 read_len;
		__u32 block_size;
		__s32 compress_flag;

		if(block_index >= sft_fp->sft_c_header.block_cnt)
		{
			__err("block index invalid...\n");
			goto err;
		}

		if(inter_offset > sft_fp->sft_c_header.block_size)
		{
			__err("invalid para...\n");
			goto err;
		}

		if(inter_offset + *len > sft_fp->sft_c_header.block_size)
		{
			__err("invalid para...\n");
			goto err;
		}

		g_fseek(sft_fp->fp, sft_fp->sft_c_header.block_size_start_offset+block_index*4,
			 SEEK_SET);
		read_len = g_fread(&block_start_addr, 1, 4, sft_fp->fp);
		if(4 != read_len)
		{
			__msg("read err...\n");
			goto err;
		}

		read_len = g_fread(&block_end_addr, 1, 4, sft_fp->fp);
		if(4 != read_len)
		{
			__msg("read err...\n");
			goto err;
		}

		block_size = SFT_GET_REAL_SIZE(block_end_addr) - SFT_GET_REAL_SIZE(block_start_addr);
		compress_flag = SFT_GET_COMPRESS_FLAG(block_start_addr);
		if(0 == compress_flag)
		{			
			g_fseek(sft_fp->fp, sft_fp->sft_c_header.data_start_offset+SFT_GET_REAL_SIZE(block_start_addr)+inter_offset, SEEK_SET);
			read_len = g_fread(pbuf, 1, *len, sft_fp->fp);
			if(read_len != *len)
			{
				__msg("read file err...\n");
				goto err;
			}
			*len = read_len;

			return EPDK_OK;
		}
		else
		{
			__s32 ret;
			__s32 uncompress_size;
			char* pcompress_buf;
			char* puncompress_buf;
			
			pcompress_buf = sft_fp->pmax_compress_block_buf;
			puncompress_buf = sft_fp->puncompress_block_buf;
			g_fseek(sft_fp->fp, sft_fp->sft_c_header.data_start_offset+SFT_GET_REAL_SIZE(block_start_addr), SEEK_SET);
			read_len = g_fread(pcompress_buf, 1, block_size, sft_fp->fp);
			if(read_len != (__s32)block_size)
			{
				__msg("read file err...\n");
				goto err;
			}
			uncompress_size = AZ100_GetUncompressSize(pcompress_buf, block_size);						

			ret = AZ100_Uncompress(pcompress_buf, block_size, puncompress_buf, uncompress_size);
			if(EPDK_FAIL == ret)
			{
				__msg("uncompress fail...\n");
				goto err;
			}

			*len = SFT_MIN(*len, uncompress_size-inter_offset);
			g_memcpy(pbuf, puncompress_buf+inter_offset, *len);
			
			return EPDK_OK;
		}		
	}

err:;
	return EPDK_FAIL;
}

static __s32 __sft_get_block(SFT_FILE* fp, __s32 block_index, char* pbuf, __s32* len)
{
	SFT_FILE* sft_fp;
	
	sft_fp = (SFT_FILE*)fp;
	if (!sft_fp || !sft_fp->fp)
	{
		goto err;
	}

	if (0 == sft_fp->type)
	{
		__err("sft type err...\n");
		goto err;
	}
	else
	{
		__u32 block_start_addr;
		__u32 block_end_addr;
		__u32 read_len;
		__u32 block_size;
		__s32 compress_flag;

		if(block_index >= sft_fp->sft_c_header.block_cnt)
		{
			__err("block index invalid...\n");
			goto err;
		}

		g_fseek(sft_fp->fp, sft_fp->sft_c_header.block_size_start_offset+block_index*4,
			 SEEK_SET);
		read_len = g_fread(&block_start_addr, 1, 4, sft_fp->fp);
		if(4 != read_len)
		{
			__msg("read err...\n");
			goto err;
		}

		read_len = g_fread(&block_end_addr, 1, 4, sft_fp->fp);
		if(4 != read_len)
		{
			__msg("read err...\n");
			goto err;
		}

		block_size = SFT_GET_REAL_SIZE(block_end_addr) - SFT_GET_REAL_SIZE(block_start_addr);
		compress_flag = SFT_GET_COMPRESS_FLAG(block_start_addr);
		if(0 == compress_flag)
		{
			if(*len < (__s32)block_size)
			{
				__err("buf too small...\n");
				goto err;
			}
			g_fseek(sft_fp->fp, sft_fp->sft_c_header.data_start_offset+SFT_GET_REAL_SIZE(block_start_addr), SEEK_SET);
			read_len = g_fread(pbuf, 1, block_size, sft_fp->fp);
			if(read_len != block_size)
			{
				__msg("read file err...\n");
				goto err;
			}
			*len = block_size;

			return EPDK_OK;
		}
		else
		{
			__s32 ret;
			__s32 uncompress_size;
			char* pcompress_buf;
			
			pcompress_buf = sft_fp->pmax_compress_block_buf;
			g_fseek(sft_fp->fp, sft_fp->sft_c_header.data_start_offset+SFT_GET_REAL_SIZE(block_start_addr), SEEK_SET);
			read_len = g_fread(pcompress_buf, 1, block_size, sft_fp->fp);
			if(read_len != block_size)
			{
				__msg("read file err...\n");
				goto err;
			}
			uncompress_size = AZ100_GetUncompressSize(pcompress_buf, block_size);			
			if(*len < uncompress_size)
			{
				__msg("buf too small...\n");
				goto err;
			}

			ret = AZ100_Uncompress(pcompress_buf, block_size, pbuf, uncompress_size);
			if(EPDK_FAIL == ret)
			{
				__msg("uncompress fail...\n");
				goto err;
			}
			*len = uncompress_size;
			return EPDK_OK;
		}		
	}

err:;
	return EPDK_FAIL;
}

__size sft_fread(void * pbuf, __size size, __size num, ES_FILE * fp)
{
	SFT_FILE* sft_fp;
	
	sft_fp = (SFT_FILE*)fp;
	if (!sft_fp || !sft_fp->fp)
	{
		goto err;
	}

	if (0 == sft_fp->type)
	{
		return g_fread(pbuf, size, num, sft_fp->fp);
	}
	else
	{		
		__u32 total_size_read;
		__u32 total_size_to_read;		
		char* puncompress_buf;						
		__u32 block_size;
		__s32 size_to_read;
		__s32 read_size;
		__s32 block_index;
		__u32 start_read_pos;
		__s32 inter_offset;
		char* preceive_buf;
		__s32 ret;

		preceive_buf = (char*)pbuf;
		total_size_to_read = size*num;		
		total_size_read = 0;
		puncompress_buf = sft_fp->puncompress_block_buf;
		start_read_pos = sft_fp->offset;

        if(start_read_pos >= sft_fp->sft_c_header.file_size)
        {
            goto err;
        }

		//��ȡ��һ��
		block_size = sft_fp->sft_c_header.block_size;
		block_index = start_read_pos/block_size;
		inter_offset = start_read_pos%block_size;
		if(inter_offset+total_size_to_read > block_size)
		{
			size_to_read = block_size-inter_offset;
		}
		else
		{
			size_to_read = total_size_to_read;
		}

		read_size = size_to_read;
		ret = __sft_get_block_part(sft_fp, block_index, inter_offset, preceive_buf, &read_size);
		if(EPDK_FAIL == ret)
		{
			__msg("__sft_get_block_part fail...\n");
			goto err;
		}				

		preceive_buf += read_size;

		total_size_to_read -= read_size;
		total_size_read += read_size;
		start_read_pos += read_size;
		
		//��ȡ�м������Ŀ�
		while(total_size_to_read >= block_size
            && start_read_pos < sft_fp->sft_c_header.file_size)
		{
			block_index = start_read_pos/block_size;			
			size_to_read = block_size;
			read_size = size_to_read;
			ret = __sft_get_block(sft_fp, block_index, puncompress_buf, (__s32*)&read_size);
			if(EPDK_FAIL == ret)
			{
				__msg("__sft_get_block...\n");
				goto err;
			}
			
			g_memcpy(preceive_buf, puncompress_buf, read_size);
			preceive_buf += read_size;
			
			total_size_to_read -= read_size;
			total_size_read += read_size;
			start_read_pos += read_size;			
			
			if(read_size != size_to_read)
			{
				__msg("__sft_get_block fail...\n");
				break;
			}
		}

		//��ȡ���һ��
		if(total_size_to_read > 0
            && start_read_pos < sft_fp->sft_c_header.file_size)
		{
			block_index = start_read_pos/block_size;
			inter_offset = start_read_pos%block_size;
			if(inter_offset+total_size_to_read > block_size)
			{
				size_to_read = block_size-inter_offset;
			}
			else
			{
				size_to_read = total_size_to_read;
			}

			read_size = size_to_read;
			ret = __sft_get_block_part(sft_fp, block_index, inter_offset, preceive_buf, &read_size);
			if(EPDK_FAIL == ret)
			{
				__msg("__sft_get_block_part fail...\n");
				goto err;
			}				

			preceive_buf += read_size;

			total_size_to_read -= read_size;
			total_size_read += read_size;
			start_read_pos += read_size;
		}

		sft_fp->offset += total_size_read;
		if(sft_fp->offset > sft_fp->sft_c_header.file_size)
		{
			sft_fp->offset = sft_fp->sft_c_header.file_size;
		}

		return total_size_read/size;
		
	}

err:;
	return 0;
}

__s32 sft_fseek(ES_FILE* fp, __s32 offset, __s32 whence)
{
	SFT_FILE* sft_fp;
	
	sft_fp = (SFT_FILE*)fp;
	if (!sft_fp || !sft_fp->fp)
	{
		goto err;
	}
	
	if (0 == sft_fp->type)
	{
		return g_fseek(sft_fp->fp, offset, whence);
	}
	else
	{
		switch (whence)
		{
		case SEEK_SET:
			{
				sft_fp->offset = offset;
				break;
			}
		case SEEK_CUR:
			{
				sft_fp->offset += offset;
				break;
			}
		case SEEK_END:
			{
				sft_fp->offset = sft_fp->sft_c_header.file_size - offset;
				break;
			}
		default:
			{
				__msg("para incorrect...\n");
				goto err;
			}
		}
		if (sft_fp->offset < 0)
		{
			sft_fp->offset = 0;
		}
		if (sft_fp->offset > sft_fp->sft_c_header.file_size)
		{
			sft_fp->offset = sft_fp->sft_c_header.file_size;
		}		
	}

err:;
	return 0;
}

__s32 sft_ftell(ES_FILE* fp)
{
	SFT_FILE* sft_fp;
	
	sft_fp = (SFT_FILE*)fp;
	if (!sft_fp || !sft_fp->fp)
	{
		goto err;
	}
	
	if (0 == sft_fp->type)
	{
		return g_ftell(sft_fp->fp);
	}
	else
	{
		return sft_fp->offset;
	}

err:;
	return 0;
}

///////////////////////////////////////
//test code
__s32 sft_test(void)
{
	char str_uncompress_file[256];
	char str_compress_file[256];
	ES_FILE* fp_uncompress;
	ES_FILE* fp_compress;
	char* puncompress_buf;
	char* pcompress_buf;   
	__s32 uncompress_file_size;
	__s32 compress_file_size;
	__s32 ret;	
	
	g_strcpy(str_uncompress_file, "font16.sft");
	g_strcpy(str_compress_file, "font16_e.sft");

	fp_uncompress = sft_fopen(str_uncompress_file, "rb");
	if(!fp_uncompress)
	{
		return 0;
	}

	fp_compress = sft_fopen(str_compress_file, "rb");
	if(!fp_compress)
	{
		return 0;
	}

	ret = 1;

	{
		__s32 i;		
		__s32 start_pos;
		__s32 read_len;
		__s32 uncompress_read_len;
		__s32 compress_read_len;

		sft_fseek(fp_uncompress, 0, SEEK_END);
		uncompress_file_size = sft_ftell(fp_uncompress);
		puncompress_buf = (char*)esMEMS_Balloc(uncompress_file_size);
		if(!puncompress_buf)
		{
			ret = 0;
			goto out;
		}

		sft_fseek(fp_compress, 0, SEEK_END);
		compress_file_size = sft_ftell(fp_compress);
		pcompress_buf = (char*)esMEMS_Balloc(compress_file_size);
		if(!pcompress_buf)
		{
			ret = 0;
			goto out;
		}

		if(uncompress_file_size != compress_file_size)
		{
			ret = 0;
			goto out;
		}

		for(i = 0 ; i < 100000 ; i++)
		{
			if(33017 == i)
			{
				int a = 0;
			}
			start_pos = eLIBs_ramdom(compress_file_size-100);
			read_len = eLIBs_ramdom(compress_file_size-start_pos);
			if(0 == read_len)
			{
				read_len = 1;
			}

			sft_fseek(fp_uncompress, start_pos, SEEK_SET);
			uncompress_read_len = sft_fread(puncompress_buf, 1, read_len, fp_uncompress);
			if(uncompress_read_len != read_len)
			{
				ret = 0;
				goto out;
			}

			sft_fseek(fp_compress, start_pos, SEEK_SET);
			compress_read_len = sft_fread(pcompress_buf, 1, read_len, fp_compress);
			if(compress_read_len != read_len)
			{
				ret = 0;
				goto out;
			}
			if(0 != g_memcmp(puncompress_buf, pcompress_buf, read_len))
			{
				ret = 0;
				goto out;
			}
		}
	}

out:;

	if(fp_uncompress)
	{
		sft_fclose(fp_uncompress);
		fp_uncompress = NULL;
	}

	if(fp_compress)
	{
		sft_fclose(fp_compress);
		fp_compress = NULL;
	}

	if(puncompress_buf)
	{
		esMEMS_Bfree(puncompress_buf, uncompress_file_size);
		puncompress_buf = NULL;
	}

	if(pcompress_buf)
	{
		esMEMS_Bfree(pcompress_buf, compress_file_size);
		pcompress_buf = NULL;
	}

	return ret;
}

static __s32 sft_check_data_valid(char* pbuf, __s32 len, __s32 offset)
{
	__s32 i;
	unsigned char ch;

	ch = (unsigned char)offset;
	for(i = 0 ; i < len ; i++)
	{
		if((unsigned char)(pbuf[i]) != ch)
		{
			return 0;
		}
		ch++;
	}
	
	return 1;
}

__s32 sft_test2(void)
{	
	char str_compress_file[256];
	ES_FILE* fp_compress;	
	char* pcompress_buf;    
	__s32 compress_file_size;
	__s32 ret;	
		
	g_strcpy(str_compress_file, "d:\\apps\\font16_e.sft");

	fp_compress = sft_fopen(str_compress_file, "rb");
	if(!fp_compress)
	{
		return 0;
	}

	ret = 1;

	{
		__s32 i;		
		__s32 start_pos;
		__s32 read_len;
		__s32 compress_read_len;

		sft_fseek(fp_compress, 0, SEEK_END);
		compress_file_size = sft_ftell(fp_compress);
		pcompress_buf = (char*)esMEMS_Balloc(compress_file_size);
		if(!pcompress_buf)
		{
			ret = 0;
			goto out;
		}

		for(i = 0 ; i < 50000 ; i++)
		{
            __msg("testing, i=%d\n", i);
			if(67 == i)
			{
				int a = 0;
			}
			start_pos =eLIBs_ramdom(compress_file_size-100);			
			read_len = eLIBs_ramdom(compress_file_size-start_pos);
			if(0 == read_len)
			{
				read_len = 1;
			}

			sft_fseek(fp_compress, start_pos, SEEK_SET);
			compress_read_len = sft_fread(pcompress_buf, 1, read_len, fp_compress);
			if(compress_read_len != read_len)
			{
				//ret = 0;
				//goto out;
			}
			if(0 == sft_check_data_valid(pcompress_buf, compress_read_len, start_pos%0x100))
			{
				ret = 0;
				goto out;
			}
		}
	}

out:;

	if(fp_compress)
	{
		sft_fclose(fp_compress);
		fp_compress = NULL;
	}

	if(pcompress_buf)
	{
		esMEMS_Bfree(pcompress_buf, compress_file_size);
		pcompress_buf = NULL;
	}

	return ret;
}

#endif     //  ifndef __sft_decode_c

/* end of sft_decode.c  */
