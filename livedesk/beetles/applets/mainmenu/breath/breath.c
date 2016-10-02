/*
*********************************************************************************************************
*											        ePDK
*						            the Easy Portable/Player Develop Kits
*									           willow app sample
*
*						        (c) Copyright 2006-2007, RICHARD, China
*											All	Rights Reserved
*
* File    : breath.c
* By      : worldchip by James
* Version : V1.00
*********************************************************************************************************
*/

#include "breath.h"


/***********************************************************
该函数用于生成具有高斯分布的alpha值，传入的数据包括原始数据的指针
原始图像的SIZE信息，以及内部图像的SIZE信息，
和内部图像在边框图像中的位置OFFSET
通过该函数输入buffer中的alpha位被重新写入。
**********************************************************/
static __sw_breath_info_t		sw_breath;


void _embed_sub_img(void *dst_data,SIZE *dst_size,void *src_data,SIZE *src_size,__pos_t *site)
{
	__u32 height;
	__u32 offset;

	offset = dst_size->width * site->y + site->x;
	
	for (height = 0; height < src_size->height; height++)
	{
		eLIBs_memcpy((__u32 *)dst_data + offset + height * dst_size->width,
						(__u32 *)src_data + height * src_size->width,src_size->width * 4);
	}
}

void memsetw(void *buf,__u32 value,__u32 len)
{
	__u32 i;
	__u32 *tmp = buf;
	
	for(i = 0; i < len; i++)
	{
		tmp[i] = value;
	}
}

__bool _CreatAlpha(__u8 * in,SIZE * b)
{
	__u8 * 	lpTempPtr;
	__u8 * 	lpPtr;
	void *	lptemp;
	void *  lptemp1;
	__u32	Result;
	__s32	i;
	__s32 	j;
	__s32 	k;
	__s32 	N=7;
	__s32   iTempH	= 2*N+1	;       //- 模板的高度
	__s32   iTempW	= 2*N+1 ;	      //- 模板的宽度
	__s32   iTempMX	= N ;       //- 模板的中心元素X坐标 
	__s32   iTempMY	= N	;       //- 模板的中心元素Y坐标 
	__s32 	bh=b->height;
	__s32	bw=b->width;

	if((lptemp=esMEMS_Malloc(0,bw*bh*4))==NULL)
	{
    		//__wrn("%s\n","无法获取lptemp动态内存！");
		return EPDK_FALSE;
	}	
	
	if((lptemp1=esMEMS_Malloc(0,b->width*b->height*4))==NULL)
	{
    		//__wrn("%s\n","无法获取lptemp动态内存！");
		return EPDK_FALSE;
	}	
	lpTempPtr=(__u8 *)lptemp;
	lpPtr=(__u8 * )in;
	eLIBs_memcpy(lpTempPtr,lpPtr,bw*bh*4);	
	
	for( i = iTempMY; i < bh - iTempH + iTempMY + 1; i++)	// 行(除去边缘几行)
	{				
		for( j = iTempMX; j < bw - iTempW + iTempMX + 1; j++)// 列(除去边缘几列)
		{
						
			// 指向新DIB第i行，第j个象素的指针
			lpTempPtr = in + 4*bw*i + 4*j;			
			Result = 0;	
			for (k = 0; k < iTempH; k++)
			{
				lpPtr=( __u8*)lptemp+ 4*bw*( i - iTempMY + k )+4*(j - iTempMX )	;
				Result += (* (__u8 *)(lpPtr+3));
				lpPtr += 4;	
				Result += (* (__u8 *)(lpPtr+3));
				lpPtr += 4;	
				Result += (* (__u8 *)(lpPtr+3));
				lpPtr += 4;	
				Result += (* (__u8 *)(lpPtr+3));
				lpPtr += 4;	
				Result += (* (__u8 *)(lpPtr+3));
				lpPtr += 4;	
				Result += (* (__u8 *)(lpPtr+3));
				lpPtr += 4;	
				Result += (* (__u8 *)(lpPtr+3));
				lpPtr += 4;	
				Result += (* (__u8 *)(lpPtr+3));
				lpPtr += 4;	
				Result += (* (__u8 *)(lpPtr+3));
				lpPtr += 4;	
				Result += (* (__u8 *)(lpPtr+3));
				lpPtr += 4;	
				Result += (* (__u8 *)(lpPtr+3));
				lpPtr += 4;	
				Result += (* (__u8 *)(lpPtr+3));
				lpPtr += 4;	
				Result += (* (__u8 *)(lpPtr+3));
				lpPtr += 4;	
				Result += (* (__u8 *)(lpPtr+3));
				lpPtr += 4;	
				Result += (* (__u8 *)(lpPtr+3));
			}							
			Result =Result>>8;					// 乘上系数	
			if(* (lpTempPtr+3) == 0)
				*(__u32 *)lpTempPtr = 0xffffff;
				
			* (lpTempPtr+3) = (Result>255?255:Result);			
		}
	}		

	eLIBs_memcpy((__u8 *)lptemp1,in,b->width*b->height*4);	

	for( i = iTempMY; i < bh - iTempH + iTempMY + 1; i++)	// 行(除去边缘几行)
	{				
		for( j = iTempMX; j < bw - iTempW + iTempMX + 1; j++)// 列(除去边缘几列)
		{
						
			// 指向新DIB第i行，第j个象素的指针
			lpTempPtr = in + 4*bw*i + 4*j;			
			Result = 0;	
			for (k = 0; k < iTempH; k++)
			{
				lpPtr=( __u8*)lptemp1+ 4*bw*( i - iTempMY + k )+4*(j - iTempMX )	;
				Result += (* (__u8 *)(lpPtr+3));
				lpPtr += 4;	
				Result += (* (__u8 *)(lpPtr+3));
				lpPtr += 4;	
				Result += (* (__u8 *)(lpPtr+3));
				lpPtr += 4;	
				Result += (* (__u8 *)(lpPtr+3));
				lpPtr += 4;	
				Result += (* (__u8 *)(lpPtr+3));
				lpPtr += 4;	
				Result += (* (__u8 *)(lpPtr+3));
				lpPtr += 4;	
				Result += (* (__u8 *)(lpPtr+3));
				lpPtr += 4;	
				Result += (* (__u8 *)(lpPtr+3));
				lpPtr += 4;	
				Result += (* (__u8 *)(lpPtr+3));
				lpPtr += 4;	
				Result += (* (__u8 *)(lpPtr+3));
				lpPtr += 4;	
				Result += (* (__u8 *)(lpPtr+3));
				lpPtr += 4;	
				Result += (* (__u8 *)(lpPtr+3));
				lpPtr += 4;	
				Result += (* (__u8 *)(lpPtr+3));
				lpPtr += 4;	
				Result += (* (__u8 *)(lpPtr+3));
				lpPtr += 4;	
				Result += (* (__u8 *)(lpPtr+3));
			}							
			Result =Result>>8;					// 乘上系数	

			if(* (lpTempPtr+3) == 0)
				*(__u32 *)lpTempPtr = 0xffffff;
			
			* (lpTempPtr+3) = (Result>255?255:Result);			
		}
	}
	
	for( i=0; i<bh; i++)
	{
		for( j=0; j<bw; j++)
		{
			__u32 alpha0;
			lpPtr = ( __u8*)lptemp+ b->width*4*i+4*j;
			alpha0 = *(__u8 *) (lpPtr+3);
						
			if(alpha0 != 0)
			{
				__u32 ntmp0;
				__u32 ntmp1;
				__u32 alpha1;
				__u32 ntmp2;
				__u32 ntmp3;
				lpTempPtr = in + 4*b->width*i + 4*j;
				ntmp0 = *(__u32 *)(lpPtr);
				ntmp1 = *(__u32 *)(lpTempPtr);
				alpha1 = 255-alpha0;
				ntmp2 = ((ntmp0&0x00ff00ff)*alpha0+(ntmp1&0x00ff00ff)*alpha1)&0xff00ff00;
				ntmp0 = ntmp0>>8;
				ntmp1 = ntmp1>>8;
				ntmp3 = ((ntmp0&0x00ff00ff)*alpha0+(ntmp1&0x00ff00ff)*alpha1)&0xff00ff00;
				*(__u32 *)(lpTempPtr) = (ntmp2>>8)|ntmp3;			
			}											
		}	
	}

	esMEMS_Mfree(0,lptemp);
	esMEMS_Mfree(0,lptemp1);
	
	return EPDK_TRUE; 	
}

 __u8 _alphastep(__s32 step ,  __u8 alpha)
{
	__u32 a;

	if(alpha == 0)
		return 0;
	
	a = 255-(alpha);
	switch(step)
    {
      case 0 :
        a = a;	         
        break;
      case 1 :
        a = (a*a) >> 8;        	        
        break;
      case 2:
        a=(a*a*a)>>16;        	        
        break;
      case 3:
         a=(a*a*a*a)>>24;        	        
        break;
      case 4:
        a=(((a*a*a*a)>>24) * a) >> 8;
		break;
	  case 5:
        a=(((a*a*a*a)>>24) * a * a) >> 16;
		break;
	  case 6:
        a=(((a*a*a*a)>>24) * a * a * a) >> 24;
		break;
	}
    return ( __u8) (255 - a) ;
}

void _run_breath(__u32 step)
{
	__u32 i;
	__u8 alpha;	
	
	for (i = 0; i < sw_breath.breath_size.width*sw_breath.breath_size.height; i++)
	{
		alpha = *((__u8 *)sw_breath.buf+ i * 4 + 3);				
		*((__u8 *)sw_breath.fb.addr[0] + i * 4 + 3) = _alphastep(step,alpha);
		
	}
}

void _main_task(void *arg)
{	
	__u8 err;
	__s8 cnt = 0;
	__bool revert = 0;
	
	while(1)
    {
       	if(esKRNL_TDelReq(EXEC_prioself) == OS_TASK_DEL_REQ)
   			break;
		esKRNL_TimeDly(16);	
	
		esKRNL_SemPend(sw_breath.sem,0xffff,&err);
	
		if (!sw_breath.exit && (GUI_LockAccept() == EPDK_OK) )
		{	
			if (!revert){	
				cnt++;				
				if (cnt == 7){	
					revert = 1;
					cnt = 5;
				}				
			}
			else{
				cnt--;
				if (cnt == -1){	
					revert = 0;
					cnt = 1;
				}				
			}
			_run_breath(cnt);
			GUI_LyrWinSel(sw_breath.layer);	
			GUI_BitString_DrawEx(&sw_breath.fb, sw_breath.site.x, sw_breath.site.y);

			GUI_Unlock();
		}
		
		esKRNL_SemPost(sw_breath.sem);
	
	}	
	esKRNL_TDel(EXEC_prioself);
	return;
}

__s32 BREATH_Init(void)
{	
#ifdef ENABLE_BREATH_EFFECT
	__u8 err;
	
	/*init anole breath varible*/
	sw_breath.exit= 1;
	
	sw_breath.fb.fmt.type = FB_TYPE_RGB;
	sw_breath.fb.fmt.fmt.rgb.pixelfmt = PIXEL_COLOR_ARGB8888;
	sw_breath.fb.fmt.fmt.rgb.br_swap = 0;
	sw_breath.fb.fmt.fmt.rgb.pixseq = 0;
	sw_breath.fb.fmt.fmt.rgb.palette.addr = NULL;
	sw_breath.fb.fmt.fmt.rgb.palette.size = 0;
#ifdef MAINMENU_SHINE_NEW
	sw_breath.fb.size.height = 200;
	sw_breath.fb.size.width = 200;
#else
	sw_breath.fb.size.height = 128;
	sw_breath.fb.size.width = 128;
#endif
	sw_breath.fb.addr[0] = esMEMS_Malloc(0,sw_breath.fb.size.width*sw_breath.fb.size.height*4);
	if (sw_breath.fb.addr[0] == NULL)
	{
		__wrn("malloc memory fail\n");
		goto _exit_BREATH_Init0;
	}	
	sw_breath.buf = esMEMS_Malloc(0,sw_breath.fb.size.width*sw_breath.fb.size.height*4);
	if (sw_breath.buf == NULL)
	{
		__wrn("malloc memory fail\n");
		goto _exit_BREATH_Init1;
	}
	
	/*create breath sem*/
	sw_breath.sem = esKRNL_SemCreate(1);

	/*create thread for show*/
	sw_breath.mtsk = esKRNL_TCreate(_main_task, 0, 0x500, KRNL_priolevel6);
	if(sw_breath.mtsk ==NULL)
	{
		__wrn("create breath thread fail\n");
		goto _exit_BREATH_Init2;
	}

	return EPDK_OK;
	
_exit_BREATH_Init2:
	esKRNL_SemDel(sw_breath.sem,OS_DEL_ALWAYS,&err);
	esMEMS_Mfree(0,sw_breath.buf);
_exit_BREATH_Init1:
	esMEMS_Mfree(0,sw_breath.fb.addr[0]);
_exit_BREATH_Init0:
	return EPDK_FAIL;
#else
	return EPDK_OK;
#endif
}

void BREATH_Exit(void)
{
#ifdef ENABLE_BREATH_EFFECT
	__u8 err;

	/*kill breath thread*/
	while(1)
	{	
		if(esKRNL_TDelReq(sw_breath.mtsk) == OS_TASK_NOT_EXIST)
			break;
		esKRNL_TimeDly(3);
	}
	
	/*delete breath sem*/
	esKRNL_SemDel(sw_breath.sem,OS_DEL_ALWAYS,&err);
	
	esMEMS_Mfree(0,sw_breath.buf);
	sw_breath.buf = NULL;
	esMEMS_Mfree(0,sw_breath.fb.addr[0]);
	sw_breath.fb.addr[0] = NULL;
#endif

	return;
}


void BREATH_ReStart(void)
{
#ifdef ENABLE_BREATH_EFFECT
	__u8    err;

	esKRNL_SemPend(sw_breath.sem,0xffff,&err);
	sw_breath.exit = 0;
	esKRNL_SemPost(sw_breath.sem);
#endif

	return;
}

void BREATH_Start(H_LYR layer,__pos_t *site,SIZE *core_size, void *core_buf,SIZE *breath_size)
{
#ifdef ENABLE_BREATH_EFFECT
	__u8    err;
	__pos_t site1;
	
	sw_breath.core_size = *core_size;	
	sw_breath.site = *site;
	sw_breath.breath_size = *breath_size;
	
	site1.x = sw_breath.breath_size.width/2 - sw_breath.core_size.width/2;
	site1.y = sw_breath.breath_size.height/2 - sw_breath.core_size.height/2;

	memsetw(sw_breath.buf,0x00ffffff,sw_breath.breath_size.height * sw_breath.breath_size.width);
	_embed_sub_img(sw_breath.buf, &sw_breath.breath_size, core_buf, &sw_breath.core_size, &site1);

	_CreatAlpha(sw_breath.buf, &sw_breath.breath_size);
	
	eLIBs_memcpy(sw_breath.fb.addr[0],sw_breath.buf,
		sw_breath.breath_size.height * sw_breath.breath_size.width * 4);

	sw_breath.fb.size = sw_breath.breath_size;
	sw_breath.layer = layer;
	esKRNL_SemPend(sw_breath.sem,0xffff,&err);
	sw_breath.exit = 0;
	esKRNL_SemPost(sw_breath.sem);
#endif

	return;
}

void BREATH_Stop(__u8 *core_data,SIZE *core_size)
{
#ifdef ENABLE_BREATH_EFFECT
	__u8 	err;

	esKRNL_SemPend(sw_breath.sem,0xffff,&err);
	sw_breath.exit =  1;		
	esKRNL_SemPost(sw_breath.sem);

	if(core_data)
	{	
		__pos_t site1;
		site1.x = sw_breath.breath_size.width/2 - core_size->width/2;
		site1.y = sw_breath.breath_size.height/2 - core_size->height/2;
		memsetw(sw_breath.fb.addr[0],0,sw_breath.breath_size.height * sw_breath.breath_size.width);
		_embed_sub_img(sw_breath.fb.addr[0], &sw_breath.breath_size, core_data, core_size, &site1);
	
		GUI_LyrWinSel(sw_breath.layer);			
		GUI_BitString_DrawEx(&sw_breath.fb, sw_breath.site.x, sw_breath.site.y);			
	}
#endif

	return;
}

