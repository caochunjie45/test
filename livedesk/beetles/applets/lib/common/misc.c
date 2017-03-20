/*
************************************************************************************************************************
*                                                        CUCKOO
*                                                   the Audio Player
*
*                             Copyright(C), 2006-2009, SoftWinners Microelectronic Co., Ltd.
*											       All Rights Reserved
*
* File Name   : misc.c
*
* Author      : Gary.Wang
*
* Version     : 1.1.0
*
* Date        : 2008.11.08
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
* Gary.Wang      2008.11.08       1.1.0        build the file
*
************************************************************************************************************************
*/
#ifndef  __misc_c
#define  __misc_c

#include "Misc.h"

#define LANG_COUNT 3

#define AUDIO_DRV_PATH "b:\\AUDIO\\PLAY"

#define COM_GET_R_VAL(color) ( ((__u32)color)&0xff )
#define COM_GET_G_VAL(color) ( (((__u32)color)>>8)&0xff )
#define COM_GET_B_VAL(color) ( (((__u32)color)>>16)&0xff )
#define COM_GET_ALPHA_VAL(color) ( (((__u32)color)>>24)&0xff )

#define PIOC_MEM_BASE   (0xf1c20800)

#define PIO_REG_CFG(n, i)               ((volatile unsigned int *)( PIOC_MEM_BASE + ((n)-1)*0x24 + ((i)<<2) + 0x00))
#define PIO_REG_DLEVEL(n, i)            ((volatile unsigned int *)( PIOC_MEM_BASE + ((n)-1)*0x24 + ((i)<<2) + 0x14))
#define PIO_REG_PULL(n, i)              ((volatile unsigned int *)( PIOC_MEM_BASE + ((n)-1)*0x24 + ((i)<<2) + 0x1C))
#define PIO_REG_DATA(n) 	            ((volatile unsigned int *)( PIOC_MEM_BASE + ((n)-1)*0x24 + 0x10))

static __hdle misc_mute_hdl = 0;
static __u8 audio_mute = 0;
static __hdle audio_mode_hdl = 0;
static __u8 mic_ctrl_status;


//libaiao, 2011.5.15
//����ָ�����ȴ�src�н�ȡ�ַ���
//����ֵΪ��ȡ�ַ����ĳ���(byteΪ��λ)
__s32 GUI_GetStringByWidth(char *dst, const char GUI_UNI_PTR *src, __u32 width) 
{
    const char GUI_UNI_PTR * s = src;
    U16 r = 0,w = 0, len = 0;
	
	
	r   = GUI_UC_GetCharCode(s);
	if(r == 0)
	{
		return 0;
	}
	while(r != 0)
	{
		if(w >= width)
		{
			break;
		}	
    	r   = GUI_UC_GetCharCode(s+len);
	    len  += GUI_UC_GetCharSize(s+len);		//ָ����һ���ַ�
		w += GUI_GetCharDistX(r);
	}
	eLIBs_strncpy(dst, src, len);
    return len;
}

//libaiao, 2011.5.15
//extern __s32 GUI_GetStringByWidth(char *dst, const char GUI_UNI_PTR *src, __u32 width);
//���ݿ���ʾ�����ȡ�ִ�
//����ִ���ȴ�����ʾ����,���ڿ���ʾ�ִ��������ʡ�Ժ�
//ע��ú���ֻ�жϿ�ȣ����жϸ߶�
//appendΪ׷���ں����ʡ�Ժ�
__s32 GetClippedString(GUI_RECT *region, char *src_str, char *dst_str, const char *append)
{
	__u32 src_width = 0, dst_width = 0, append_width = 0;	
	__u32 len = 0, char_width = 0;
	__u32 gap = 0;
	
	dst_width = region->x1 - region->x0;	
	src_width = GUI_GetStringDistX(src_str);
	if(dst_width >= src_width)
	{
		eLIBs_strcpy(dst_str, src_str);
		return EPDK_OK;
	}
	append_width = GUI_GetStringDistX(append);
	if(append_width >= dst_width)
	{
		eLIBs_strcpy(dst_str, append);
		return EPDK_OK;
	}
	gap = dst_width - append_width;
	GUI_GetStringByWidth(dst_str, src_str, gap);
	//GUI_DispStringLen();
	eLIBs_strcat(dst_str, append);
	return EPDK_OK;
}

void time2time( __u32 second_time, cuckoo_time_t *format_time )
{
	__u32  total_minute;
	
	total_minute = second_time/60;
	format_time->second = second_time%60;
	format_time->minute = total_minute%60;
	format_time->hour   = total_minute/60;	
}

void   uint2str( __u32 input, char * str )
{
	char stack[11] ;
	int i ;
	int j ;
	int count;
	
	
	if( input == 0 )
	{
		str[0] = '0';
		str[1] = '0';
		str[2] = '\0';                   // 'str' must end with '\0'
		return ;
	}
	
	for( i = 0; input > 0; ++i )
	{
		stack[i] = input%10 + '0';       // characters in reverse order are put in 'stack' .
		input /= 10;
	}                                    // at the end of 'for' loop, 'i' is the number of characters.
	count = i;
	
	for( j = 0, i = 0; i < ( 2-count); i++ )    // pad '0'
		str[j++] = '0';
	for( i = count, --i; i >= 0; --i, ++j )
		str[j] = stack[i];		
	str[j] = '\0';		                 // 'str' must end with '\0'
}

static void __time2str( const cuckoo_time_t *time, char *str )
{
	uint2str( time->hour,   str );
	str[2] = ':';
	uint2str( time->minute, str+3 );
	str[5] = ':';
	uint2str( time->second, str+6 );
}

void  time2str( __u32 ms, char *str )
{
	__u32  second;
	cuckoo_time_t  time;
	
	second = ms /1000;
	time2time( second, &time );
	__time2str( &time, str );
}


//����Ҫ��ʾ�ĸ�ʽת��
void time2str_by_format(__u32 ms, char *str, time_format_e format)
{
	__u32  second;
	cuckoo_time_t  time;
	
	second = ms /1000;
	time2time( second, &time );
	
	switch(format)
	{
		case TIME_S:
			uint2str( time.second, str);		
			break;
		case TIME_MS:
			uint2str( time.minute, str);
			str[2] = ':';
			uint2str( time.second, str+3);		
			break;
		case TIME_HMS:
			time2str(ms, str);
			break;
		case TIME_AUTO_HMS:
			uint2str( time.minute, str);
			str[2] = ':';
			uint2str( time.second, str+3);	
			if(time.hour != 0)
			{
				time2str(ms, str);
			}
			break;
		default:
			break;
	}
	
}
	

//ת���ļ�size ���ַ���
//add by libaiao, 2011.5.8
void filesize2str(__u32 size, char *str)
{
	__msg("size = %d\n" , size );
	if (size < 1024)	// С��1 k
	{
		eLIBs_sprintf(str, "%d B",size);
	}
	else if ( size < (1024*1024))	// С�� 1 M
	{
		eLIBs_sprintf(str, "%d K",size/1024);
	}
	else if (size < (1024*1024*1024))	// С�� 1 G
	{
		//eLIBs_sprintf(str, "%d.%d M",size/(1024*1024), (size%(1024*1024))*100/(1024*1024)/10);
		eLIBs_sprintf(str, "%d.%d%d M",size/(1024*1024), ((size%(1024*1024))/1024)*1000/1024/100,((size%(1024*1024))/1024)*1000/1024%100/10);	//������λС��

	}
	else
	{
		//eLIBs_sprintf(str, "%d.%d G",size/(1024*1024*1024),(size%(1024*1024*1024))*100/(1024*1024*1024)/10);
		eLIBs_sprintf(str, "%d.%d%d G",size/(1024*1024*1024),(size%(1024*1024*1024))/(1024*1024)*1000/1024/100,(size%(1024*1024*1024))/(1024*1024)*1000/1024%100/10);

	}
}





//����24ʱ��Сʱֵ������12ʱ��Сʱֵ������0Ϊam����0Ϊpm
__s32 hour24_to_hour12(__s32* hour)
{
	if (*hour == 0)
	{//�賿12��
		*hour = 12;
	} 
	else if (*hour == 12)
	{//����12��
		return 1;
	}
	else if (12 < *hour)
	{
		*hour -= 12;
		return 1;
	}
	return 0;
}

//��ѯapp_name���Ƶ�Ӧ���Ƿ����
//��ѯ���������Ƿ���ڿ�����is_app_exist(APP_MUSIC)
__s32 is_app_exist(char* app_name)
{
    H_WIN root, child;
	char  winname[256];  
    __bool exist;
   
    root  = GUI_WinGetHandFromName(APP_ROOT);	

    if(!root)
    {
        return 0;
    }
    
    child = GUI_WinGetFirstChild(root);

    exist = 0;
    
	while( child != NULL )
	{
		eLIBs_memset(winname, 0, 256);
		GUI_WinGetName(child, winname);
				
		__msg("winname = %s \n", winname);
		
		if( 0 == eLIBs_strcmp( app_name, winname))		
		{            
			exist = 1;
            break;
		}
		
		child = GUI_WinGetNextBro(child);
	}

    return exist;
}

__u32 get_memory_info(char* title, void* pExtPara)
{
	static __u32 last_free_size = 0;
	__u32 free_size = 0, total_size = 0;
	__u32 size;

	//__msg("\n");
	//__msg("=====================title: %s====================\n", title);	
	free_size = esMEMS_FreeMemSize();
	if(free_size > last_free_size)
	{
		size = free_size - last_free_size;
		if(last_free_size == 0)
		{
			__msg("=====================malloc memory size is %d=======\n", last_free_size);		
		}
		else
		{
			__msg("=====================release memory size is %d=======\n", size);		
		}
	}
	else if(free_size <= last_free_size)
	{
		size = last_free_size - free_size;
		__msg("=====================malloc memory size is %d=======\n", size);		
	}
	
	total_size = esMEMS_TotalMemSize();
	__msg("=====================free memory size is %d=======\n", free_size);
	__msg("=====================used memory size is %d=======\n", total_size-free_size);
	__msg("=====================total memory size is %d=======\n", total_size);
	last_free_size = free_size;
}


void * My_Palloc(__u32 npage, __u32 mode)
{
    void* p;

    //if(npage)
    
    p = esMEMS_Palloc( npage, mode);
    __wrn("palloc p=%x, npage=%d\n", p, npage);
    return p;
}

void My_Pfree(void *mblk, __u32 npage)
{    
    __wrn("Pfree p=%x\n", mblk);
    esMEMS_Pfree(mblk, npage);
}

void* My_Balloc(__u32 size)
{
	static __u32 index = 0;    
	void* p = esMEMS_Balloc(size);
    if(52 == size)
    {
       // __wrn("********My_Balloc******\n");
    }
	__wrn("Balloc index=%d p=%x, size=%d\n", index, (__u32)p, size);
	index++;
	return p;
}

void  My_Bfree(void *mblk, __u32 size)
{
	__wrn("Bfree p=%x, size=%d\n",(__u32)mblk, size);
	esMEMS_Bfree(mblk, size);
}

void* My_Malloc(__mems_heap_t *heap, __u32 size)
{
	static __u32 index = 0;
	void* p = esMEMS_Malloc(heap,  size);
	__wrn("Malloc index=%d p=%x\n", index, (__u32)p);
	index++;
	return p;
}


void  My_Mfree(__mems_heap_t *heap, void *f)
{
	__wrn("Mfree p=%x\n",(__u32)f);
	esMEMS_Mfree(heap, f);
}

__u8 app_misc_get_mute_status(void)
{
	return audio_mute;
}
__u8 get_mic_ctrl_status()
{
	return mic_ctrl_status;
}

__s32 set_misc_mic_ctrl_status(__s32 mode)
{
	mic_ctrl_status = EPDK_FALSE;
	return EPDK_OK;
}
__u32 mic_ctrl_on_off(__u8 mode)
{
	mic_ctrl_status = mode;
}

__s32 app_misc_audio_release()
{
	if(misc_mute_hdl != 0)
	{
		esPINS_PinGrpRel(misc_mute_hdl, 2);
		misc_mute_hdl = 0;
	}
	if(audio_mode_hdl != 0)
	{
		esPINS_PinGrpRel(audio_mode_hdl, 2);
		audio_mode_hdl = 0;
	}

}

__s32 app_misc_audio_power_mute(__u8 mute)
{  
	__s32			 ret;
	user_gpio_set_t  gpio_set[1];
	audio_mute = mute;
	eLIBs_memset(gpio_set, 0, sizeof(user_gpio_set_t));
	
	ret = esCFG_GetKeyValue("mute_para", "mute_en", (int *)gpio_set, sizeof(user_gpio_set_t)/4);
	if (!ret)
	{
		if(misc_mute_hdl == 0)
		{
			misc_mute_hdl = esPINS_PinGrpReq((__user_pin_set_t*)gpio_set, 1);
			if (!misc_mute_hdl)
			{
				__msg("request output_col pin failed\n");
				return EPDK_FAIL;
			}
		}
	}
	else
	{
		__msg("fetch para from script failed\n");
		return EPDK_FAIL;
	}

	if(mute == 1)
	{
		mute = 0;
	}
	else
	{
		mute = 1;
	}

	ret = esPINS_WritePinData(misc_mute_hdl, mute, NULL);
	if (ret)
	{
		__msg("pull output_col failed\n");
		return EPDK_FAIL;
	}
	else
	{
		__msg("esPINS_SetPinPull success\n");
	}
	//esPINS_PinGrpRel(root_mute_hdl, 0);//��Ҫ�ͷţ������������
	
}


__s32 app_misc_audio_mute(__u8 mute)
{  
    __s32            ret;
    user_gpio_set_t  gpio_set[1];	
	reg_system_para_t* para;
	para = (reg_system_para_t*)dsk_reg_get_para_by_app(REG_APP_SYSTEM);
	audio_mute = mute;

	if(mute)//������
	{
		{
			ES_FILE* pAuCtrl = eLIBs_fopen("b:\\AUDIO\\CTRL","r+");
			if(!pAuCtrl){
				__wrn("Fail to open Audio control dev.\n");
				return EPDK_FAIL;
			}
			

			eLIBs_fioctrl(pAuCtrl, AUDIO_DEV_CMD_MUTE, 0, NULL);

			eLIBs_fclose(pAuCtrl);
			
			dsk_volume_set(para->volume);
		}
	}
	else//����
	{
		{
			ES_FILE 	*p_dac; 
			__s32		 ret;
			
			ES_FILE* pAuCtrl = eLIBs_fopen("b:\\AUDIO\\CTRL","r+"); 		
			if(!pAuCtrl){
				__wrn("Fail to open Audio control dev.\n");
				return EPDK_FAIL;
			}
			eLIBs_fioctrl(pAuCtrl, AUDIO_DEV_CMD_MUTE, 1, NULL);
			eLIBs_fclose(pAuCtrl);

			p_dac = eLIBs_fopen(AUDIO_DRV_PATH, "r+");
			if(!p_dac)
			{
				__err("open dac device fail!\n");
				return EPDK_FAIL;
			}
			ret = eLIBs_fioctrl(p_dac, AUDIO_DEV_CMD_SET_VOLUME, 0, 0);
			eLIBs_fclose(p_dac);  
		}
	}

	/*
    eLIBs_memset(gpio_set, 0, sizeof(user_gpio_set_t));
    ret = esCFG_GetKeyValue("mute_para", "mute_en", (int *)gpio_set, sizeof(user_gpio_set_t)/4);
    if (!ret)
    {
    	if(misc_mute_hdl == 0)
    	{
            misc_mute_hdl = esPINS_PinGrpReq((__user_pin_set_t*)gpio_set, 1);
            if (!misc_mute_hdl)
            {
                __msg("request output_col pin failed\n");
                return EPDK_FAIL;
            }
    	}
    }
    else
    {
        __msg("fetch para from script failed\n");
        return EPDK_FAIL;
    }

	if(mute == 1)
	{
		mute = 0;
	}
	else
	{
		mute = 1;
	}

    ret = esPINS_WritePinData(misc_mute_hdl, mute, NULL);
	if (ret)
	{
		__msg("pull output_col failed\n");
		return EPDK_FAIL;
	}
    else
    {
        __msg("esPINS_SetPinPull success\n");
    }
    //esPINS_PinGrpRel(root_mute_hdl, 0);//��Ҫ�ͷţ������������
    */
}
__s32 audio_mode_switch(__u8 mode)
{  
    __s32            ret;
    user_gpio_set_t  gpio_set[1];
        eLIBs_memset(gpio_set, 0, sizeof(user_gpio_set_t));
        ret = esCFG_GetKeyValue("audio_type", "audio_mode", (int *)gpio_set, sizeof(user_gpio_set_t)/4);
        if (!ret)
        {
        	if(audio_mode_hdl == 0)
        	{
	            audio_mode_hdl = esPINS_PinGrpReq((__user_pin_set_t*)gpio_set, 1);
	            if (!audio_mode_hdl)
	            {
	                __msg("request output_col pin failed\n");
	                return EPDK_FAIL;
	            }
        	}
        }
        else
        {
            __msg("fetch para from script failed\n");
            return EPDK_FAIL;
        }

		

    ret = esPINS_WritePinData(audio_mode_hdl, mode, NULL);
	if (ret)
	{
		__msg("pull output_col failed\n");
		return EPDK_FAIL;
	}
    else
    {
        __msg("esPINS_SetPinPull success\n");
		return EPDK_OK;
    }
    //esPINS_PinGrpRel(root_mute_hdl, 0);//��Ҫ�ͷţ������������
    
}



static __s32 com_copy_rect_32bpp(__u32* pDst, __s32 dst_x, __s32 dst_y, __s32 dst_line_w, __u32* pSrc,
                __s32 src_x, __s32 src_y, __s32 src_line_w, __s32 w, __s32 h)
{
    __s32 i, j;

    for(i = 0 ; i < w ; i++)
    {
        for(j = 0 ; j < h ; j++)
        {
            DATA_OF_ADRR(__u32, pDst, dst_x+i, dst_y+j, dst_line_w) = DATA_OF_ADRR(__u32, pSrc, src_x+i, src_y+j, src_line_w);
        }
    }
    
    return EPDK_OK;
}

static __s32 com_copy_rect_8bpp(char* pDst, __s32 dst_x, __s32 dst_y, __s32 dst_line_w, char* pSrc,
                __s32 src_x, __s32 src_y, __s32 src_line_w, __s32 w, __s32 h)
{
    __s32 i, j;

    src_line_w = (src_line_w+3)/4*4;
   // dst_line_w = (dst_line_w+3)/4*4;
    
    for(i = 0 ; i < w ; i++)
    {
        for(j = 0 ; j < h ; j++)
        {
            DATA_OF_ADRR(__u8, pDst, dst_x+i, dst_y+j, dst_line_w) = 0xf0;//DATA_OF_ADRR(__u8, pSrc, src_x+i, src_y+j, src_line_w);
        }
    }
    
    return EPDK_OK;
}

//////////////////////////////////////////////////////////////////////////

/************************************************************************/
// pFileData --- ��Դ������õ�����(Ҳ��32λbmp�ļ�����)
// x��y      --- Ŀ������
// x0, y0, x1, y1 --- Դ����(��bmpͼƬĳһ����)
/************************************************************************/
__s32 APP_BMP_DrawEx(H_LYR hlyr, const void * pFileData, int x, int y, int x0, int y0, int x1, int y1)
{
	__s32 Ret = EPDK_OK;
	__u32 *pSrc32;
    char *pSrc8;
	int bmp_w, bmp_h;
	int w, h;
    GUI_RECT rt;    
    FB fb;
    __u32* pfb_addr;
    __s32 i, j;
    __s32 nBitCount;
    __s32 offbits;

    if(!hlyr)
    {
        __err("invalid para...\n");
        return EPDK_FAIL;
    }
    
    GUI_LyrWinGetFB(hlyr, &fb);
    
	//�������Χ
	bmp_w = GUI_BMP_GetXSize(pFileData);
	if ( ! ValueBetweenRangeEx(0, x0, x1, bmp_w))
	{
		return EPDK_FAIL;
	}

	bmp_h = GUI_BMP_GetYSize(pFileData);
	if ( ! ValueBetweenRangeEx(0, y0, y1, bmp_h))
	{
		return EPDK_FAIL;
	}	

    if(x1-x0+x >= fb.size.width
        || y1-y0+y >= fb.size.height)
    {
        return EPDK_FAIL;
    }

    nBitCount = *((char*)pFileData + 28);
    
    offbits = *(__u32*)((char*)pFileData+10);

    pSrc8 = (char*)pFileData + offbits;
    
    pSrc32 = (__u32*)pSrc8;

    pfb_addr = (__u32*)(fb.addr[0]);       
    
    if(32 == nBitCount)
    {
        com_copy_rect_32bpp(pfb_addr, x, y, fb.size.width, pSrc32, x0, y0, bmp_w, x1-x0+1, y1-y0+1);       
    }
    else if(8 == nBitCount)
    {
        com_copy_rect_8bpp((char*)pfb_addr, x, y, fb.size.width, pSrc8, x0, y0, bmp_w, x1-x0+1, y1-y0+1);       
    }
    else
    {
        __err("not 32bpp or 8bpp...\n");
    }
    

	return Ret;
}

__s32 app_set_title(__s32 res_id)
{
	char title[32] = {0};

	dsk_langres_get_menu_text(res_id, title, sizeof(title));

	return gscene_hbar_set_title(title, sizeof(title));
}


__s32 com_set_backcolor(__u32 color)
{		
    dsk_display_set_backcolor(
    	(COM_GET_B_VAL(color)<<24) 
    	| (COM_GET_G_VAL(color)<<16)
    	| (COM_GET_R_VAL(color)<<8)
    	| (COM_GET_ALPHA_VAL(color)<<0) );

    return EPDK_OK;
}

void pull_down_gpio(__u32 port, __u32 port_num)
{
    volatile  __u32    *tmp_addr;
    __u32               tmp_data;
    __u32               tmp1;
    __u32               tmp2;
    

    //����Ϊ���
    tmp1 = (port_num - ((port_num>>3)<<3))<<2;
    tmp_addr = PIO_REG_CFG(port, (port_num>>3));
    tmp_data = *tmp_addr;
    tmp_data &= ~(0x07 << tmp1);
    tmp_data |=  (0x01 << tmp1);
    *tmp_addr = tmp_data;

    //����Ϊ����
    tmp2 = (port_num - ((port_num>>4)<<4))<<1;
    tmp_addr = PIO_REG_PULL(port, (port_num>>4));
    tmp_data = *tmp_addr;
    tmp_data &= ~(0x03 << tmp2);
    tmp_data |=  (0x02 << tmp2);
    *tmp_addr = tmp_data;      
    
    //����͵�ƽ
    tmp_addr = PIO_REG_DATA(port);
    tmp_data = *tmp_addr;
    tmp_data &= ~(1 << port_num);
    //tmp_data |=  (1 << port_num);
    *tmp_addr = tmp_data;    
    
}

void pull_up_gpio(__u32 port, __u32 port_num)
{
    volatile  __u32    *tmp_addr;
    __u32               tmp_data;
    __u32               tmp1;
    __u32               tmp2;
    

    //����Ϊ���
    tmp1 = (port_num - ((port_num>>3)<<3))<<2;
    tmp_addr = PIO_REG_CFG(port, (port_num>>3));
    tmp_data = *tmp_addr;
    tmp_data &= ~(0x07 << tmp1);
    tmp_data |=  (0x01 << tmp1);
    *tmp_addr = tmp_data;

    //����Ϊ����
    tmp2 = (port_num - ((port_num>>4)<<4))<<1;
    tmp_addr = PIO_REG_PULL(port, (port_num>>4));
    tmp_data = *tmp_addr;
    tmp_data &= ~(0x03 << tmp2);
    tmp_data |=  (0x01 << tmp2);
    *tmp_addr = tmp_data;      
    
    //����ߵ�ƽ
    tmp_addr = PIO_REG_DATA(port);
    tmp_data = *tmp_addr;
    tmp_data &= ~(1 << port_num);
    tmp_data |=  (1 << port_num);
    *tmp_addr = tmp_data;    
    
}

//����gpioΪ����
//pull: 0:���� 1:����  2:����
void set_input_gpio(__u32 port, __u32 port_num, __s32 pull)
{
    volatile  __u32    *tmp_addr;
    __u32               tmp_data;
    __u32               tmp1;
    __u32               tmp2;
    

    //����Ϊ����
    tmp1 = (port_num - ((port_num>>3)<<3))<<2;
    tmp_addr = PIO_REG_CFG(port, (port_num>>3));
    tmp_data = *tmp_addr;
    tmp_data &= ~(0x07 << tmp1);
    //tmp_data |=  (0x01 << tmp1);
    *tmp_addr = tmp_data;

    //����pull����
    tmp2 = (port_num - ((port_num>>4)<<4))<<1;
    tmp_addr = PIO_REG_PULL(port, (port_num>>4));
    tmp_data = *tmp_addr;
    tmp_data &= ~(0x03 << tmp2);
    tmp_data |=  (pull << tmp2);
    *tmp_addr = tmp_data;          
    
}


__s32 get_gpio_status(__u32 port, __u32 port_num)
{
    volatile  __u32    *tmp_addr;
    __u32               tmp_data;
    __s32               ret;
    __u32               tmp1;
    __u32               tmp2;

    //����Ϊ����
    tmp1 = (port_num - ((port_num>>3)<<3))<<2;
    tmp_addr = PIO_REG_CFG(port, (port_num>>3));
    tmp_data = *tmp_addr;
    tmp_data &= ~(0x07 << tmp1);
    tmp_data |=  (0x00 << tmp1);
    *tmp_addr = tmp_data;
    
    //����Ϊ����
    tmp2 = (port_num - ((port_num>>4)<<4))<<1;
    tmp_addr = PIO_REG_PULL(port, (port_num>>4));
    tmp_data = *tmp_addr;
    tmp_data &= ~(0x03 << tmp2);
    tmp_data |=  (0x02 << tmp2);
    *tmp_addr = tmp_data;
    
    
    tmp_addr = PIO_REG_DATA(port);
    tmp_data = *tmp_addr;

    __wrn("tmp_data=0x%x\n", tmp_data);
    ret = (tmp_data & (1 << port_num))>>port_num;
    
    return ret;
}

int print_layer_info(void)
{
	ES_FILE * dispfile;
	__s32 ret = 0;
	__u32 pbuffer[3] = {0};
    dispfile = eLIBs_fopen("b:\\DISP\\DISPLAY", "r+");
    if(!dispfile)
    {
        eLIBs_printf("open display device fail!\n");
        return EPDK_FAIL;
    }

	if(1)
	{
		__disp_layer_info_t     layer_para={0};
		__s32 i = 0;
		char * wm[6] = {"NORMAL","PALETTE","INTER_BUF","GAMMA","SCALER","reserved"};
		char * format[17] = {"1-bpp","2-bpp","4-bpp","8-bpp","rgb655","rgb565","rgb556",
							"argb1555","rgba5551","rgb888","argb8888",
							"YUV444","YUV422","YUV420","YUV411","CSI RGB","reserved"};
		char * yuv_mode[6] = {"non-macro block planar","interleaved","non-macro block UV combined",
							"macro block planar","macro block UV combined","reserved"};

		eLIBs_printf(".Layers Information Begin.................................................................................................\n");
		for(i=0; i<4; i++)
		{
			__s32 pixel_fmt = 0;
			__s32 pixel_mode = 0;
			__s32 pixel_seq = 0;

			pbuffer[0] = i+100;
			pbuffer[1] = (__u32)&layer_para;
	    	ret = eLIBs_fioctrl(dispfile, DISP_CMD_LAYER_GET_PARA, 0, (void*)pbuffer);
	    	if(ret != EPDK_OK)
	    	{
		        eLIBs_printf(".\tLayer %d:close\n",i);
		        continue;
		    }
	    	eLIBs_printf(".\tLayer %d:open\n",i);
	    	
	    	eLIBs_printf(".\t\twork mode:%s\n",((__s32)layer_para.mode>=0 && layer_para.mode<5)?wm[layer_para.mode]:wm[5]);
	    	eLIBs_printf(".\t\tpriority:%d\n",layer_para.prio);

		    pixel_fmt = layer_para.fb.format;
		    pixel_mode = layer_para.fb.mode;
		    pixel_seq = layer_para.fb.seq;
	    	eLIBs_printf(".\t\tformat:%s,",(pixel_fmt>=0 && pixel_fmt<16)?format[pixel_fmt]:format[16]);
	    	eLIBs_printf("\tmode:%s,",(pixel_mode>=0 && pixel_mode<5)?yuv_mode[pixel_mode]:yuv_mode[5]);
	    	

		    if(pixel_fmt == DISP_FORMAT_ARGB8888 && pixel_mode == DISP_MOD_INTERLEAVED)
		    {
		    	eLIBs_printf("\tpixels sequence:%s,",(pixel_seq==DISP_SEQ_ARGB)?"ARGB":"BGRA");
		    }
		    else if(pixel_fmt == DISP_FORMAT_YUV422 && pixel_mode == DISP_MOD_INTERLEAVED)
		    {
		    	     if(pixel_seq==DISP_SEQ_UYVY) eLIBs_printf("\tpixels sequence:UYVY,");
		    	else if(pixel_seq==DISP_SEQ_YUYV) eLIBs_printf("\tpixels sequence:YUYV,");
		    	else if(pixel_seq==DISP_SEQ_VYUY) eLIBs_printf("\tpixels sequence:VYUY,");
		    	else if(pixel_seq==DISP_SEQ_YVYU) eLIBs_printf("\tpixels sequence:YVYU,");
		    }
		    else if(pixel_fmt == DISP_FORMAT_YUV444 && pixel_mode == DISP_MOD_INTERLEAVED)
		    {
		    	     if(pixel_seq==DISP_SEQ_AYUV) eLIBs_printf("\tpixels sequence:AYUV,");
		    	else if(pixel_seq==DISP_SEQ_VUYA) eLIBs_printf("\tpixels sequence:VUYA,");
		    }
		    else if(pixel_fmt == DISP_FORMAT_YUV420 && (pixel_mode == DISP_MOD_NON_MB_UV_COMBINED || pixel_mode == DISP_MOD_MB_UV_COMBINED))
		    {
		    	     if(pixel_seq==DISP_SEQ_UVUV) eLIBs_printf("\tpixels sequence:UVUV,");
		    	else if(pixel_seq==DISP_SEQ_VUVU) eLIBs_printf("\tpixels sequence:VUVU,");
		    }
		    else
		    {
		    	eLIBs_printf("\tpixels sequence:%d,",pixel_seq);
		    }
		    eLIBs_printf("\tB R channel swap:%s\n",layer_para.fb.br_swap==0?"RGB":"BGR");

			if(pixel_mode == DISP_MOD_INTERLEAVED)
			{
				eLIBs_printf(".\t\tframe buffer address 0:%x\n",layer_para.fb.addr[0]);
			}
		    else if(pixel_mode == DISP_MOD_NON_MB_UV_COMBINED || pixel_mode == DISP_MOD_MB_UV_COMBINED)
		    {
		    	eLIBs_printf(".\t\tframe buffer address 0:%x,\taddress 1:%x\n",layer_para.fb.addr[0],layer_para.fb.addr[1]);
		    }
		    else
		    {
		    	eLIBs_printf(".\t\tframe buffer address 0:%x,\taddress 1:%x,\tddress 2:%x\n",layer_para.fb.addr[0],layer_para.fb.addr[1],layer_para.fb.addr[2]);
		    }
		    
	    	eLIBs_printf(".\t\tsource window(x:%d,\ty:%d,\twidth:%d,\theight:%d)\n",layer_para.src_win.x,layer_para.src_win.y,layer_para.src_win.width,layer_para.src_win.height);
	    	eLIBs_printf(".\t\tscreen window(x:%d,\ty:%d,\twidth:%d,\theight:%d)\n",layer_para.scn_win.x,layer_para.scn_win.y,layer_para.scn_win.width,layer_para.scn_win.height);

	    	eLIBs_printf(".\t\tcolor key:%s\n",layer_para.ck_enable==0?"disable":"enable");
	    	eLIBs_printf(".\t\talpha:%s,\talpha value:%x\n",layer_para.alpha_en==0?"disable":"enable",layer_para.alpha_val);
	    	eLIBs_printf(".\t\tpipe:%d\n",layer_para.pipe);
	    }
		eLIBs_printf(".Layers Information End.................................................................................................\n");
	}
    
    eLIBs_fclose(dispfile);

    return EPDK_OK;
}

__s32 app_root_play_app_sounds(short_key_sound_t type)
{
   
    return EPDK_OK;
}



#endif     //  ifndef __misc_c

/* end of misc.c */
