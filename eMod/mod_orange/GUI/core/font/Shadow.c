/*
**********************************************************************************************************************
*                                                    ePDK
*                                    the Easy Portable/Player Develop Kits
*                                              eMOD Sub-System
*
*                                   (c) Copyright 2007-2009, Steven.ZGJ.China
*                                             All Rights Reserved
*
* Moudle  : lemon
* File    : Shadow.c
* Purpose : shadow for SFT
*
* By      : Zhao Jingman 
* Version : v1.0
* Date    : 2009-1-6 11:55:42
**********************************************************************************************************************
*/


/***************************************************************
	������Ӱʵ�ֺ���shadow������Ϊ�洢���ֵ�buffer������Ľӿ�
	Ϊ��char * in,����ĳ�width������Ŀ�height,�Լ�����ÿһ��
	���ֽ���linebytes��

	�������ͣ�
	U32  :  unsigned int
	U8   :  unsigned char
***************************************************************/

#include "gui_font_private.h"
#define SHADOW_SIZE   2

static void shadow_move(U32 * in, U32 * out, U32 xsize, U32 ysize);
static void shadow_smooth(U32 * in, U32 * out, U32 xsize, U32 ysize);

void shadow(U32 *pBuffer, U32 xsize, U32 ysize)
{	
	U32 	x;
	U32 	y;
	void 	*lptemp;
	void 	*lptemp1;
	U8  	*lpPtr;
	U8  	*lpTempPtr;

	if((lptemp = (void *)esMEMS_Malloc(0, (xsize+SHADOW_SIZE)*4*(ysize+SHADOW_SIZE)))==NULL)
	{
    	__inf("malloc lptemp fail\n");
	}
	if((lptemp1 = (void *)esMEMS_Malloc(0, (xsize+SHADOW_SIZE)*4*(ysize+SHADOW_SIZE)))==NULL)
	{
    	__inf("malloc lptemp1 fail\n");
    	
	}
	for(y=0; y<ysize+SHADOW_SIZE; y++)
	{
		for(x=0; x<xsize+SHADOW_SIZE; x++)
		{
			if((y>=ysize)||(x>=xsize))
			{
				lpTempPtr = (U8 *)lptemp + y*(xsize+SHADOW_SIZE)*4 + 4*x;
				*(U32 *)lpTempPtr = 0x00000000;
			}
			else
			{
				lpTempPtr = (U8 *)lptemp + y*(xsize+SHADOW_SIZE)*4 + 4*x;
				lpPtr = (U8 *)pBuffer + y*xsize*4 + 4*x;
				*(U32 *)lpTempPtr = *(U32 *)lpPtr;
			}
		}
	}

	
	xsize += SHADOW_SIZE;
	ysize += SHADOW_SIZE;
//	memcpy((U8 *)pBuffer,(U8 *)lptemp,xsize*ysize*4);
	orange_memcpy((U8 *)lptemp1,(U8 *)lptemp,xsize*ysize*4);
	shadow_move((U32 *)lptemp, pBuffer, xsize, ysize);
	orange_memcpy((U8 *)lptemp, (U8 *)pBuffer, xsize*ysize*4);
	orange_memset((U8 *)pBuffer,0,xsize*ysize*4);
	shadow_smooth((U32 *)lptemp, pBuffer, xsize, ysize);	
	orange_memcpy((U8 *)lptemp, (U8 *)pBuffer, xsize*ysize*4);
	orange_memset((U8 *)pBuffer,0,xsize*ysize*4);
	shadow_smooth((U32 *)lptemp, pBuffer, xsize, ysize);
	
//ԭʼͼ��ɫ����	
	for(y=0; y<ysize; y++)
	{
	  for(x=0; x<xsize; x++)
	  {
	  	lpPtr = (U8 *)lptemp1 + y*xsize*4 + 4*x;		
		if(*(lpPtr+3) != 0)
		{
			lpTempPtr = (U8 *)pBuffer + y*xsize*4 + 4*x;
			*(int *)(lpTempPtr) = *(int *)lpPtr;
/*
				*(lpTempPtr) =(alpha0*(*(lpPtr))+(255-alpha0)*(*(lpTempPtr)))>>8;
				lpTempPtr++;
		        lpPtr++;
		        *(lpTempPtr) =(alpha0*(*(lpPtr))+(255-alpha0)*(*(lpTempPtr)))>>8;
				lpTempPtr++;
		        lpPtr++;
		        *(lpTempPtr) =(alpha0*(*(lpPtr))+(255-alpha0)*(*(lpTempPtr)))>>8;
				lpTempPtr++;
		        lpPtr++;
		        *(lpTempPtr) =(alpha0*(*(lpPtr))+(255-alpha0)*(*(lpTempPtr)))>>8;
*/
		}
	  }
	}
	
	esMEMS_Mfree(0, lptemp);
	esMEMS_Mfree(0, lptemp1);

}

static void shadow_move(U32 * in, U32 * out, U32 xsize, U32 ysize)
{
	U8 		*src;
	U8 		*dest;
	U32 	num;
	U32 	i;
	U32		j;
	U32		step;
	
	step = 2;
	for( i=0; i<ysize; i++)
	{
		for( j=0; j<xsize; j++)
		{
		
			dest =(U8 *) out+ xsize*4*i+4*j;
			*(U32 *)dest =0x00000000;
			}				
		}
	
	for( i = 0; i < ysize; i++)	// ��(��ȥ��Ե����)
	{		
		for( j = 0; j < xsize ; j++)// ��(��ȥ��Ե����)
		{			
			// ָ��DIB��i�У���j �����ص�ָ��
			src = (U8 *)in + xsize*4*(i) + 4*j ;
			num = *(src + 3);
			if(num!=0)
			{
			// ָ����DIB��i�У���j�����ص�ָ��
				dest =(U8 *) out+ xsize*4*((i+(step+1)))+4*(j+step);			
				*(unsigned char *)(dest+3) =(unsigned char) num;

			}
		}
	}
}

static void shadow_smooth(U32 * in, U32 * out, U32 xsize, U32 ysize)
{
	U8 		*src;
	U8 		*dest;
	U32 	Result;
	U32 	Tmp;
	U32 	i;
	U32		j;
	U32		k;
	U32		l;
		
	
/*	for(i = 1; i < ysize - 4 + 2 + 1; i++)	// ��(��ȥ��Ե����)
	{		
//��һ�����ݵ�Ĵ���
		Result=0;
		j = 1;
		// ָ����DIB��i�У���j�����ص�ָ��
		dest = (U8 *)out + xsize*4*i + 4 * j;
		for (k = 0; k < 4 ; k++)
		{
			for (l = 0; l < 4 ; l++)
			{
				// ָ��DIB��i - iTempMY + k�У���j - iTempMX + l�����ص�ָ��
				src = (U8 *)in + xsize*4*(  i - 2 + k)+4*(j-2+l);	
				// ��������ֵ
				Result += (* (src+3)) ;				
			}
		}
		
		Tmp = Result/20;					// ����ϵ��	
		if(Tmp > 255)
		{
			* (dest+3) = (U8 )255;
		}
		
		else
		{
			* (dest+3) = (U8) (Tmp);
		}			
		
//֮������ݵ�Ĵ�������ģ��ƽ�ƣ�������		
		for(j=j+1; j < xsize - 4 + 2 + 1; j++)// ��(��ȥ��Ե����)
		{
			// ָ����DIB��i�У���j�����ص�ָ��
			dest = (U8 *)out  + xsize*4*i +4 * j;

			// ָ��ԴDIB�� i - 2 �У���j-3 �����ص�ָ��
			src = (U8 *)in + xsize*4*( i - 2 )+4*(j-3);					
			Result -= *(src+3) ;
			// ָ��ԴDIB�� i -1 �У���j-3 �����ص�ָ��
			src += xsize*4;				
			Result -= *(src+3) ;
			// ָ��ԴDIB�� i   �У���j-3 �����ص�ָ��
			src += xsize*4;				
			Result -= *(src+3) ;
			// ָ��ԴDIB�� i + 1   �У���j-3 �����ص�ָ��
			src += xsize*4;				
			Result -= *(src+3) ;
			// ָ��ԴDIB�� i + 1  �У���j + 1 �����ص�ָ��
			src += 4*4;				
			Result += *(src+3) ;
			// ָ��ԴDIB�� i  �У���j + 1 �����ص�ָ��
			src -= xsize*4;				
			Result += *(src+3) ;
			// ָ��ԴDIB�� i - 1 �У���j + 1 �����ص�ָ��
			src -= xsize*4;				
			Result += *(src+3) ;
			// ָ��ԴDIB�� i - 2 �У���j + 1 �����ص�ָ��
			src -= xsize*4;				
			Result += *(src+3) ;			

			Tmp = Result/20;					// ����ϵ��	
			if(Tmp > 255)
			{
				* (dest+3)= (U8 )255;
			}
			else
			{
				* (dest+3)= (U8) Tmp;
			}
			
		}
	}*/
	
	
	for( i = 2; i < (ysize - 4 + 2 + 1); i++)	// ��(��ȥ��Ե����)
	{			
		for( j=2; j<(xsize - 4 + 2 + 1); j++)
		{
			Result=0;
			// ָ����DIB��i�У���j�����ص�ָ��
			dest = (unsigned char *)out +  xsize*4* (i) + 4 * j;
			for ( k = 0; k < 4; k++)
			{
				for ( l = 0; l < 4; l++)
				{
					// ָ��DIB��i - iTempMY + k�У���j - iTempMX + l�����ص�ָ��
					src = (unsigned  char*)in + xsize*4 * ( 
						i - 2 + k) + 4 * ( j - 2 + l );
					
					// ��������ֵ
					Result += (* (unsigned char *)(src+3));				
				}
			}			
			Tmp = Result / 20;					// ����ϵ��	
			if(Tmp > 255)
			{
				* (unsigned char * )(dest+3) = (unsigned char)255;
			}			
			else
			{
				*(unsigned char *) (dest+3) = (unsigned char) (Tmp);
			}

		}

	}	
	

}
