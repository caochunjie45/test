/*
************************************************************************************************************************
*                                                        BITMAP
*
*                             Copyright(C), 2006-2009, SoftWinners Microelectronic Co., Ltd.
*											       All Rights Reserved
*
* File Name : rect_op.c
*
* Author : Gary.Wang
*
* Version : 1.1.0
*
* Date : 2009.07.20
*
* Description : 
*
* Others : None at present.
*
*
* History :
*
*  <Author>        <time>       <version>      <description>
*
* Gary.Wang      2009.07.20       1.1.0        build the file
*
************************************************************************************************************************
*/
#ifndef  __rect_op_c
#define  __rect_op_c


#include "..\\winclass_i.h"
#include "ctrl_redef.h"
#include "rect_op.h"


#define MASK_ALPHA                      0xFF000000



void   rect_copy( const RECT *dst_pos,
			  	  void       *dst_base,
			  	  const RECT *src_pos,
			  	  void       *src_base,
			  	  const SIZE *rect_size,
			  	  __u32       byte_count )
{
	__u32  i;
	
	if( byte_count == 1 )
	{
		__u8   *p;
		__u8   *q;
				
		p = (__u8 *)src_base + ( src_pos->width * src_pos->y + src_pos->x );
		q = (__u8 *)dst_base + ( dst_pos->width * dst_pos->y + dst_pos->x );
		for( i = 0;  i < rect_size->height;  i++ )
		{
			g_memcpy( p, q, rect_size->width * byte_count );
			p += src_pos->width;
			q += dst_pos->width;
		}
	}
	else if( byte_count == 4 )
	{
		__u32 *p;
		__u32 *q;
				
		p = (__u32 *)src_base + ( src_pos->width * src_pos->y + src_pos->x );
		q = (__u32 *)dst_base + ( dst_pos->width * dst_pos->y + dst_pos->x );
		for( i = 0;  i < rect_size->height;  i++ )
		{
			g_memcpy( p, q, rect_size->width * byte_count );
			p += src_pos->width;
			q += dst_pos->width;
		}
	}
}



void   rect_copy_alpha( const RECT *dst_pos,
			  	        void       *dst_base,
			  	        const RECT *src_pos,
			  	        void       *src_base,
			  	        const SIZE *rect_size,
			  	        __u32       byte_count )
{
	__u32  i;
	__u32  j;
	
	if( byte_count == 1 )
	{
		__u8   *p;
		__u8   *q;
				
		p = (__u8 *)src_base + ( src_pos->width * src_pos->y + src_pos->x );
		q = (__u8 *)dst_base + ( dst_pos->width * dst_pos->y + dst_pos->x );
		for( i = 0;  i < rect_size->height;  i++ )
		{
			for( j = 0;  j < rect_size->width;  j++ )
			{
				if( p[j] != 0 )					
					q[j] = p[j];
			}
			p += src_pos->width;
			q += dst_pos->width;
		}
	}
	else if( byte_count == 4 )
	{
		__u32 *p;
		__u32 *q;
				
		p = (__u32 *)src_base + ( src_pos->width * src_pos->y + src_pos->x );
		q = (__u32 *)dst_base + ( dst_pos->width * dst_pos->y + dst_pos->x );
		for( i = 0;  i < rect_size->height;  i++ )
		{
			for( j = 0;  j < rect_size->width;  j++ )
			{
				if( ( p[j] & MASK_ALPHA ) != 0 )					
					q[j] = p[j];
			}
			p += src_pos->width;
			q += dst_pos->width;
		}
	}
}



void   fill_rect_with_column( void *dst_base, const SIZE *dst_size_p, const RECT *valid_rect_p, 
                              const void *column_buf, __u32 byte_count )
{
	__u32  i;
	__u32  j;
	
	if( byte_count == 1 )
	{
		__u8  *p;
		__u8  *q;
		__u8   value;
		
		p = (__u8 *)column_buf;
		q = (__u8 *)dst_base + dst_size_p->width * valid_rect_p->y + valid_rect_p->x;
		for( i = 0;  i < valid_rect_p->height;  i++ )
		{
			value = p[i];
			for( j = 0;  j < valid_rect_p->width;  j++ )
				q[j] = value;
			q += dst_size_p->width;
		}
	}
	else if( byte_count == 4 )
	{
		__u32  *p;
		__u32  *q;
		__u32   value;
		
		p = (__u32 *)column_buf;
		q = (__u32 *)dst_base + dst_size_p->width * valid_rect_p->y + valid_rect_p->x;
		for( i = 0;  i < valid_rect_p->height;  i++ )
		{
			value = p[i];
			for( j = 0;  j < valid_rect_p->width;  j++ )
				q[j] = value;
			q += dst_size_p->width;
		}
	}
}



void   fill_rect_with_row( void *dst_base, const SIZE *dst_size_p, const RECT *valid_rect_p, 
                           const void *row_buf, __u32 byte_count )
{
	__u32  i;
	
	if( byte_count == 1 )
	{
		__u8  *q;
		
		q = (__u8 *)dst_base + dst_size_p->width * valid_rect_p->y + valid_rect_p->x;
		for( i = 0;  i < valid_rect_p->height;  i++ )
		{
			g_memcpy( q, row_buf, valid_rect_p->width * byte_count );
			q += dst_size_p->width;
		}
	}
	else if( byte_count == 4 )
	{
		__u32  *q;
		
		q = (__u32 *)dst_base + dst_size_p->width * valid_rect_p->y + valid_rect_p->x;
		for( i = 0;  i < valid_rect_p->height;  i++ )
		{
			g_memcpy( q, row_buf, valid_rect_p->width * byte_count );
			q += dst_size_p->width;
		}
	}
}
		
		
		
void   rect_hori_mirror( void *buf, __u32 width, __u32 height, __u32 byte_count )
{
	__u32  i;
	__u32  j;
	__u32  half_width;
	__u32  line_end;
	
	half_width = width >> 1;
	line_end   = width - 1;
	if( byte_count == 1 )
	{
		__u8  *p;
		__u8   temp;
		
		p = (__u8 *)buf;
		for( i = 0;  i < height;  i++ )
		{
			for( j = 0;  j < half_width;  j++ )
			{
				temp = p[j];
				p[j] = p[line_end-j];
				p[line_end-j] = temp;
			}
			p += width;
		}
	}
	else if( byte_count == 4 )
	{
		__u32  *p;
		__u32   temp;
		
		p = (__u32 *)buf;
		for( i = 0;  i < height;  i++ )
		{
			for( j = 0;  j < half_width;  j++ )
			{
				temp = p[j];
				p[j] = p[line_end-j];
				p[line_end-j] = temp;
			}
			p += width;
		}
	}
	
}



void   rect_vert_mirror( void *buf, __u32 width, __u32 height, __u32 byte_count )
{
	__u32  i;
	__u32  half_height;
	__u32  row_size;
	void  *temp_buf = NULL;
	
	half_height = height >> 1;
	row_size = width * byte_count;
	temp_buf = g_malloc( row_size );
	if( temp_buf == NULL )
	{
		__err("Error in allocating memory.\n");
		return;
	}
	
	if( byte_count == 1 )
	{
		__u8  *p;
		__u8  *q;
		
		p = (__u8 *)buf;
		q = (__u8 *)buf + width * ( height - 1 );
		for( i = 0;  i < half_height;  i++ )
		{
			g_memcpy( temp_buf, p, row_size );
			g_memcpy( p, q, row_size );
			g_memcpy( q, temp_buf, row_size );
			
			p += width;
			q -= width;
		}
	}
	else if( byte_count == 4 )
	{
		__u32  *p;
		__u32  *q;
		
		p = (__u32 *)buf;
		q = (__u32 *)buf + width * ( height - 1 );
		for( i = 0;  i < half_height;  i++ )
		{
			g_memcpy( temp_buf, p, row_size );
			g_memcpy( p, q, row_size );
			g_memcpy( q, temp_buf, row_size );
			
			p += width;
			q -= width;
		}
	}
	
	g_free( temp_buf );
	temp_buf = NULL;
}



void   rect_get_row( const void *src_base, __u32 width, __u32 height, __u32 y, void *row_buf, __u32 byte_count )
{
	void  *row_base;
	
	if( y >= height )
		return;
		
	row_base = (__u8 *)src_base + width * y * byte_count;
	g_memcpy( row_buf, row_base, width * byte_count );
}
	


void   rect_get_column( const void *src_base, __u32 width, __u32 height, __u32 x, void *column_buf, __u32 byte_count )
{
	__u32  i;
	
	if( x >= width )
		return;
		
	if( byte_count == 1 )
	{
		__u8  *p;
		__u8  *q;
		p = (__u8 *)src_base;
		q = (__u8 *)column_buf;
		for( i = 0;  i < height;  i++ )
		{
			*q = p[x];
			p += width;
			q++;
		}
	}
	else if( byte_count == 4 )
	{
		__u32  *p;
		__u32  *q;
		p = (__u32 *)src_base;
		q = (__u32 *)column_buf;
		for( i = 0;  i < height;  i++ )
		{
			*q = p[x];
			p += width;
			q++;
		}
	}

}
	





#endif     //  ifndef __rect_op_c

/* end of rect_op.c  */
