/*
********************************************************************************
*                                    eMOD
*                   the Easy Portable/Player Develop Kits
*                               mod_cedar sub-system
*                          (module name, e.g.mpeg4 decoder plug-in) module
*
*          (c) Copyright 2010-2012, Allwinner Microelectronic Co., Ltd.
*                              All Rights Reserved
*
* File   : isdbdec.c
* Version: V1.0
* By     : Michael_LEE
* Date   : 2012-3-10
* Description:
********************************************************************************
*/
#if PLAY_ISDBT_CLOSECAPTION

#include "isdbdec_s.h"
//#include "isdbdec.h"
//#include "string_jis2eucjp.h"

#define AV_RB16(p)  ((*(p))<<8 | (*((p)+1)))
#define AV_RB24(p)  (*(p))<<16 | (*((p)+1))<<8 | (*((p)+2))
#define AV_RB32(p)  ((*(p))<<24 | (*((p)+1))<<16 | (*((p)+2))<<8 | (*((p)+3)))
static __s32 data_group(CCedarSubtitleFrame *tmpItemInf, const __u8 *section, __s32 section_len,
    __u8 * dst, __s32 * dstLen, __s32 *groupLen);
static __s32 caption_management_data(CCedarSubtitleFrame *tmpItemInf, const __u8 *section, __s32 section_len,
    __u8 *dst, __s32 *dstLen, __s32 *groupLen);
static __s32 groupa_caption_data(CCedarSubtitleFrame *tmpItemInf, const __u8 *section, __s32 section_len,
    __u8 *dst, __s32 *dstLen, __s32 *groupLen);
static __s32 groupb_caption_data(CCedarSubtitleFrame *tmpItemInf, const __u8 *section, __s32 section_len,
    __u8 *dst, __s32 *dstLen, __s32 *groupLen);
static __s32 groupc_caption_data(CCedarSubtitleFrame *tmpItemInf, const __u8 *section, __s32 section_len,
    __u8 *dst, __s32 *dstLen, __s32 *groupLen);
static __s32 groupb_data_unit(CCedarSubtitleFrame *tmpItemInf, const __u8 *section, __s32 section_len, 
    __u8 *dst, __s32 *dstLen, __s32 *groupLen);
static __s32 groupc_data_unit(CCedarSubtitleFrame *tmpItemInf, const __u8 *section, __s32 section_len, 
    __u8 *dst, __s32 *dstLen, __s32 *groupLen);
static __s32 groupa_data_unit(CCedarSubtitleFrame *tmpItemInf, const __u8 *section, __s32 section_len, 
    __u8 *dst, __s32 *dstLen, __s32 *groupLen);
static __s32 b_data_unit(CCedarSubtitleFrame *tmpItemInf, const __u8 *section, __s32 section_len, 
    __u8 *dst, __s32 *dstLen, __s32 *groupLen);
static __s32 c_data_unit(CCedarSubtitleFrame *tmpItemInf, const __u8 *section, __s32 section_len, 
    __u8 *dst, __s32 *dstLen, __s32 *groupLen);
static __s32 a_data_unit(CCedarSubtitleFrame *tmpItemInf, const __u8 *section, __s32 section_len, 
    __u8 *dst, __s32 *dstLen, __s32 *groupLen);

static __s32 get8( const __u8 **pp, const __u8 *p_end)
{
    const __u8 *p;
    __s32 c;

    p = *pp;
    if (p >= p_end)
        return -1;
    c = *p++;
    *pp = p;
    return c;
}

static __u32 get16( const __u8 **pp, const __u8 *p_end)
{
    const __u8 *p;
    __u32 c;

    p = *pp;
    if ((p + 1) >= p_end)
        return (__u32)-1;
    c = AV_RB16(p);
    p += 2;
    *pp = p;
    return c;
}

static __u32 get24( const __u8 **pp, const __u8 *p_end)
{
    const __u8 *p;
    __u32 c;

    p = *pp;
    if ((p + 2) >= p_end)
        return (__u32)-1;
    c = AV_RB24(p);
    p += 3;
    *pp = p;
    return c;
}

static __u32 get32( const __u8 **pp, const __u8 *p_end)
{
    const __u8 *p;
    __u32 c;

    p = *pp;
    if ((p + 3) >= p_end)
        return (__u32)-1;
    c = AV_RB32(p);
    p += 4;
    *pp = p;
    return c;
}

__epdk_charset_enm_e convertCedarlibLyricEncodeType2SysCharset(enum __CEDARLIB_LYRIC_ENCODE_TYPE nCedarlibLyricEncodeType)
{
    __epdk_charset_enm_e charCodeType;
    switch(nCedarlibLyricEncodeType)
    {
        case LYRIC_SUB_ENCODE_UTF8:
        {
            charCodeType = EPDK_CHARSET_ENM_UTF8;
            break;
        }
        case LYRIC_SUB_ENCODE_GB2312:
        {
            charCodeType = EPDK_CHARSET_ENM_GB2312;
            break;
        }
        case LYRIC_SUB_ENCODE_ISO_8859_1:
        {
            charCodeType = EPDK_CHARSET_ENM_ISO_8859_1;
            break;
        }
        case LYRIC_SUB_ENCODE_ISO_8859_15:
        {
            charCodeType = EPDK_CHARSET_ENM_ISO_8859_15;
            break;
        }
        case LYRIC_SUB_ENCODE_ARIBJIS:
        {
            charCodeType = EPDK_CHARSET_ENM_JIS;
            break;
        }
        default:
        {
            WARNING("Unknown nCedarlibLyricEncodeType[0x%x],convert to EPDK_CHARSET_ENM_UTF8!\n", nCedarlibLyricEncodeType);
            charCodeType = EPDK_CHARSET_ENM_UTF8;
            break;
        }
    }
    return charCodeType;
}

/*******************************************************************************
Function name: isdbsub_convert_str_to_utf8
Description: 
    用于isdb字幕解码模块的字符串转换函数,转为utf8.
    转换完成后会加2个字节的\0.
Parameters: 
    src: 待转换的字符串的起始地址.调用完后,值会被改掉,改成指向下一个待转换的字节的位置.
    srcLen: input的有效数据长度,不包括\0.
    dst:存储转换后的utf8的字符串的buffer.调用完后,值会被改掉,改成outputbuffer的下一个可以写入的位置.
    dstLen: output buffer的总长度.字节为单位.
    mode: 如果是aribjis字符编码,需要指示模式.
    nCharEncodeType: src字符串的字符编码格式.
Return: 
    转换后的存储在output字符串buffer内的字节长度.不包括\0.
    如果outbuf刚好能全部容纳转换后的字符,那么就没有存放\0的空间了,就不存放\0. 因为转换完了,所以
    也不会给出警告.如果还有空间的话,是会在字符串的最后加上\0的.对于这一点要特别注意.
Time: 2012/7/24
*******************************************************************************/
__s32 isdbsub_convert_str_to_utf8(__u8* src, __u32 srcLen, __u8* dst, __u32 dstLen, GraphicSetsDesignationMode mode, enum __CEDARLIB_LYRIC_ENCODE_TYPE nCharEncodeType)
{
    __u32   utf8StringLen;

    if(NULL == src || 0 == srcLen || NULL == dst || dstLen <= 2)
    {
        return 0;
    }
    //WARNING("**isdbsub_convert_str_to_utf8 %x,%x,%x,%x,%x,%x,%x,%x,%x,%x **\n",src[0],src[1],src[2],src[3],src[4],src[5],src[6],src[7],src[8],src[9]);
    if(nCharEncodeType != LYRIC_SUB_ENCODE_ARIBJIS)
    {
        //skip first non-spacing, non-displayed char.
        if(0)////if(src[0] < 0x20)
        {
            WARNING("isdb skip first char[0x%x]\n", src[0]);
            src++;
            srcLen--;
        }
      //  if(src[0] == 0x1d)
        {
      //      WARNING("isdb music char[0x%x]\n", src[0]);
      //      src++;
      //      srcLen--;
		//				*src =0xfe;
        }

        utf8StringLen = eLIBs_charset_convert(convertCedarlibLyricEncodeType2SysCharset(nCharEncodeType), EPDK_CHARSET_ENM_UTF8, src, srcLen, dst, dstLen);
        dst[utf8StringLen] = 0;
        dst[utf8StringLen+1] = 0;
   // WARNING("*conv_arib_to_utf8 %x,%x,%x,%x,%x,%x,%x,%x,%x,%x *\n",dst[0],dst[1],dst[2],dst[3],dst[4],dst[5],dst[6],dst[7],dst[8],dst[9]);	//test130703 
        return utf8StringLen;
    }
    //系统本身不支持JIS编码,所以由中间件自己处理.ARIBJIS ->UTF8
    //直接调用aribjis->utf8的字符转换库
    utf8StringLen = conv_arib_to_utf8((char*)src, (int)srcLen, (char*)dst, (int)dstLen - 2, mode);
    dst[utf8StringLen] = 0;
    dst[utf8StringLen+1] = 0;
   // WARNING("*conv_arib_to_utf8 %x,%x,%x,%x,%x,%x,%x,%x,%x,%x *\n",dst[0],dst[1],dst[2],dst[3],dst[4],dst[5],dst[6],dst[7],dst[8],dst[9]);	//test130703 
    
    return utf8StringLen;
}

/*******************************************************************************
Function name: isdb_read
Description: 
    
Parameters: 
    
Return: 
    0:data group是caption statement.并解析完毕.
    1:data group是caption management.并解析完毕
    -1: 解析出错
Time: 2012/7/26
*******************************************************************************/
static __s32 isdb_read(CCedarSubtitleFrame *tmpItemInf, const __u8 *section, __s32 section_len,
    __u8 *dst, __s32 *dstLen, __s32 *groupLen)
{
    const __u8 *p;
    const __u8 *p_end;
    __u32 type;
    __s32   ret = 0;
    p_end = section + section_len;
    p     = section;
    //*dstLen = 0;
    if(0 == section_len)
        return 0;
    
    type = get8(&p, p_end); /* Data_identifier */
    if (type != 0x80)
    	{
    		WARNING("isdb read error! type =%x\n",type);
        return -1;  
    	}
    get8(&p, p_end);       /* private_stream_id */
    get8(&p, p_end);       /* reserved */

    ret = data_group(tmpItemInf, p, section_len-3, dst, dstLen, groupLen);

    return ret;
}



/*******************************************************************************
Function name: data_group
Description: 
    1.解析data group
Parameters: 
    
Return: 
    0:data group是caption statement.并解析完毕.
    1:data group是caption management.并解析完毕
    -1: 解析出错
Time: 2012/7/26
*******************************************************************************/
static __s32 data_group(CCedarSubtitleFrame *tmpItemInf, const __u8 *section, __s32 section_len,
    __u8 * dst, __s32 * dstLen, __s32 *groupLen)
{
    const __u8  *p;
    const __u8  *p_end;
    const __u8  *desc_end;
    __u32 data_group_id;
    __u32 data_group_version;
    __u32 data_group_data_byte;
    __u32 type;
    __s32   nManagementFlag = 0;
    p_end = section + section_len;
    p     = section;
    //*dstLen = 0;
    if(0 == section_len)
        return 0;
    
    type = get8(&p, p_end);                       /* 6: data_group_id  2: data_group_version */
    data_group_id = (type >> 2) & 0x3f;
    data_group_version = type & 0x03;

     get8(&p, p_end);                             /* data_group_link_number */
     get8(&p, p_end);                             /* last_data_group_link_number */
     data_group_data_byte = get16(&p, p_end);     /* data_group_data_byte   data_group_size*/
		 WARNING("data_group_id=0x%x,data_group_size = 0x%x\n",data_group_id,data_group_data_byte);	//data_group_data_byte =	//test130705 
     desc_end = p + data_group_data_byte;
     
     if (p_end < desc_end)
     	{
     		WARNING("cky error!*p_end=%x,*desc_end=%x,diff=%d,diff =%d, data_group_byte=%x\n",*p_end,*desc_end,desc_end - p_end,*desc_end - *p_end,data_group_data_byte);
        return -1;
     	}
    tmpItemInf->uDuration = 0;
    switch(data_group_id) 
        {
        case 0x00:
            caption_management_data(tmpItemInf, p, data_group_data_byte, dst, dstLen, groupLen);
            nManagementFlag = 1;
            break;            
        case 0x20:
            caption_management_data(tmpItemInf, p, data_group_data_byte, dst, dstLen, groupLen);
            nManagementFlag = 1;
            break;
        case 0x01:
        case 0x02:
        case 0x03:
        case 0x04:
        case 0x05:
        case 0x06:
        case 0x07:
        case 0x08: 
            if(tmpItemInf->displayMode == MAPLE_SUB_DISPLAY_MODE_MOBILE)
            {
                tmpItemInf->position_flags = 0;
                tmpItemInf->subNum = 0;
                groupa_caption_data(tmpItemInf, p, data_group_data_byte, dst, dstLen, groupLen);
            }
            else if(tmpItemInf->displayMode == MAPLE_SUB_DISPLAY_MODE_HD_SIDE)
            {
                tmpItemInf->position_flags = 0;
                tmpItemInf->subNum = 0;
                tmpItemInf->videoWidth = 960;
                tmpItemInf->videoHeight = 540;
                groupb_caption_data(tmpItemInf, p, data_group_data_byte, dst, dstLen, groupLen);
            }
           else if(tmpItemInf->displayMode == MAPLE_SUB_DISPLAY_MODE_SD_SIDE || tmpItemInf->displayMode == MAPLE_SUB_DISPLAY_MODE_SD_WIDE_SIDE)
           {
                tmpItemInf->position_flags = 0;
                tmpItemInf->subNum = 0;
                tmpItemInf->videoWidth = 720;
                tmpItemInf->videoHeight = 480;
                groupb_caption_data(tmpItemInf, p, data_group_data_byte, dst, dstLen, groupLen);
           }
           else if(tmpItemInf->displayMode == MAPLE_SUB_DISPLAY_MODE_UNKNOW)
           {
               tmpItemInf->position_flags = 0;
               tmpItemInf->subNum = 0;
               tmpItemInf->videoWidth = 960;
               tmpItemInf->videoHeight = 540;
               groupc_caption_data(tmpItemInf, p, data_group_data_byte, dst, dstLen, groupLen);
           }
           break;
        case 0x21:
        case 0x22:
        case 0x23:
        case 0x24:
        case 0x25:
        case 0x26:
        case 0x27:
        case 0x28:  
            if(tmpItemInf->displayMode == MAPLE_SUB_DISPLAY_MODE_MOBILE)
            {
                tmpItemInf->position_flags = 0;
                tmpItemInf->subNum = 0;
                groupa_caption_data(tmpItemInf, p, data_group_data_byte, dst, dstLen, groupLen);
            }
            else if(tmpItemInf->displayMode == MAPLE_SUB_DISPLAY_MODE_HD_SIDE)
            {
                tmpItemInf->position_flags = 0;
                tmpItemInf->subNum = 0;
                tmpItemInf->videoWidth = 960;
                tmpItemInf->videoHeight = 540;
                groupb_caption_data(tmpItemInf, p, data_group_data_byte, dst, dstLen, groupLen);
            }
           else if(tmpItemInf->displayMode == MAPLE_SUB_DISPLAY_MODE_SD_SIDE || tmpItemInf->displayMode == MAPLE_SUB_DISPLAY_MODE_SD_WIDE_SIDE)
           {
                tmpItemInf->position_flags = 0;
                tmpItemInf->subNum = 0;
                tmpItemInf->videoWidth = 720;
                tmpItemInf->videoHeight = 480;
                groupb_caption_data(tmpItemInf, p, data_group_data_byte, dst, dstLen, groupLen);
           }
           else if(tmpItemInf->displayMode == MAPLE_SUB_DISPLAY_MODE_UNKNOW)
           {
               tmpItemInf->position_flags = 0;
               tmpItemInf->subNum = 0;
               tmpItemInf->videoWidth = 960;
               tmpItemInf->videoHeight = 540;
               groupc_caption_data(tmpItemInf, p, data_group_data_byte, dst, dstLen, groupLen);
           }
           break;
        default:
            nManagementFlag = 1;
            break;
    }
    get16(&p, p_end);       /* CRC_16 */
    if(0 == tmpItemInf->uDuration)
    {
        tmpItemInf->uDuration = ISDB_FRAME_MAX_DURATION;
    }
    if(nManagementFlag)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

static __s32 caption_management_data(CCedarSubtitleFrame *tmpItemInf, const __u8 *section, __s32 section_len,
    __u8 *dst, __s32 *dstLen, __s32 *groupLen)

{
    const __u8 *p;
    const __u8 *p_end;
    const __u8 *desc_end;
    __u32 type;
    __u32 tmd;
    __s32 num_languages;
    __u32 language_tag;
    __u32 DMF;
    __u32 ISO_639_language_code;
    __u32 data_unit_loop_length;
    __s32 i;
    
    p_end = section + section_len;
    p     = section;
    *dstLen = 0;
    if(0 == section_len)
        return 0;
    
    type = get8(&p, p_end);           /* TMD */
    tmd  = (type >>6) & 0x03;
    if (0x02 ==  tmd)
    {
        get8(&p, p_end);              /* OTM */
        get8(&p, p_end); 
        get8(&p, p_end); 
        get8(&p, p_end); 
        get8(&p, p_end); 
    }
    num_languages = get8(&p, p_end);         /* num_languages */
		//WARNING("tmd = %d,num_languages = %d\n",tmd,num_languages);
    for(i=0; i<num_languages; i++)
    {
        type = get8(&p, p_end); 
        language_tag  = (type >>5) & 0x07;   /* language_tag */
        DMF  = (type >>0) & 0x0f;   /* DMF Display mode*/
				//WARNING("type = %x,language_tag = %x,DMF = %x\n",type,language_tag,DMF);

        if ((0x0c == DMF) || (0x0d == DMF) || (0x0e == DMF))
        {
            get8(&p, p_end);                 /* DC Display condition designation*/
        }
        ISO_639_language_code = get24(&p, p_end);		/*24:Language code e.p:jpn*/
				//WARNING("ISO_639_language_code:%x,%s\n",ISO_639_language_code,&ISO_639_language_code);

			if(i == 0)//取第一种语言字幕
			{
				switch(ISO_639_language_code)
				{
						case JAPANESE://日语
							{
								tmpItemInf->nCharEncodeType = LYRIC_SUB_ENCODE_ARIBJIS;	
								//WARNING("tmpItemInf->nCharEncodeType = %d\n",tmpItemInf->nCharEncodeType);

								break;
							}
							
						case PORTUGUESE://葡萄牙语
							{
								tmpItemInf->nCharEncodeType = LYRIC_SUB_ENCODE_ISO_8859_15;	
								//WARNING("tmpItemInf->nCharEncodeType = %d\n",tmpItemInf->nCharEncodeType);

								break;
							}
							
						case SPANISH://西班牙语
							{
								tmpItemInf->nCharEncodeType = LYRIC_SUB_ENCODE_ISO_8859_15;	
								//WARNING("tmpItemInf->nCharEncodeType = %d\n",tmpItemInf->nCharEncodeType);

								break;
							}
							
						default://本地多媒体会默认日语，TV由用户自己设置
							{
								WARNING("fatal error!Isdb_Dec->nCharEncodeType = %d,ISO_639_language_code:%x,%s\n",tmpItemInf->nCharEncodeType,ISO_639_language_code,&ISO_639_language_code);
								//Isdb_Dec->nCharEncodeType = LYRIC_SUB_ENCODE_ARIBJIS;			

								break;
							}
						
				}
			}
				get8(&p, p_end);                         /* 4: format(display format) 2: TCS(Character coding) 2: rollup_mode*/
    }
    data_unit_loop_length = get24(&p, p_end);    /* data_unit_loop_length */ 
    desc_end = p + data_unit_loop_length;
    if (p_end < desc_end)
        return -1;
    //data_unit(tmpItemInf, p, data_unit_loop_length, dst, dstLen);
    
    p = p_end;       
    return 0;
}

static __s32 groupb_caption_data(CCedarSubtitleFrame *tmpItemInf, const __u8 *section, __s32 section_len,
    __u8 *dst, __s32 *dstLen, __s32 *groupLen)
{
    const __u8 *p;
    const __u8 *p_end;
    const __u8 *desc_end;
    __u32 type;
    __u32 tmd;
    __u32 data_unit_loop_length;
    
    p_end = section + section_len;
    p     = section;
    //*dstLen = 0;
     if(0 == section_len)
        return 0;
    
    type = get8(&p, p_end); /* 2: TMD  6: reserved */
    tmd  = (type >>6) & 0x03;
    if (0x02 ==  tmd || 0x01 == tmd)
    {
        get8(&p, p_end);     /* STM */
        get8(&p, p_end); 
        get8(&p, p_end); 
        get8(&p, p_end); 
        get8(&p, p_end); 
    }
    data_unit_loop_length = get24(&p, p_end); /* data_unit_loop_length */
    desc_end = p + data_unit_loop_length;
    if (p_end < desc_end)
        return -1;
    groupb_data_unit(tmpItemInf, p, data_unit_loop_length, dst, dstLen, groupLen);
    
    p = p_end;    
    return 0;
}

static __s32 groupc_caption_data(CCedarSubtitleFrame *tmpItemInf, const __u8 *section, __s32 section_len,
    __u8 *dst, __s32 *dstLen, __s32 *groupLen)
{
    const __u8 *p;
    const __u8 *p_end;
    const __u8 *desc_end;
    __u32 type;
    __u32 tmd;
    __u32 data_unit_loop_length;
    
    p_end = section + section_len;
    p     = section;
    //*dstLen = 0;
     if(0 == section_len)
        return 0;
    
    type = get8(&p, p_end); /* 2: TMD  6: reserved */
    tmd  = (type >>6) & 0x03;
    if (0x02 ==  tmd || 0x01 == tmd)
    {
        get8(&p, p_end);     /* STM */
        get8(&p, p_end); 
        get8(&p, p_end); 
        get8(&p, p_end); 
        get8(&p, p_end); 
    }
    data_unit_loop_length = get24(&p, p_end); /* data_unit_loop_length */
    desc_end = p + data_unit_loop_length;
    if (p_end < desc_end)
        return -1;
    groupc_data_unit(tmpItemInf, p, data_unit_loop_length, dst, dstLen, groupLen);
    
    p = p_end;    
    return 0;
}


static __s32 groupa_caption_data(CCedarSubtitleFrame *tmpItemInf, const __u8 *section, __s32 section_len,
    __u8 *dst, __s32 *dstLen, __s32 *groupLen)
{
    const __u8 *p;
    const __u8 *p_end;
    const __u8 *desc_end;
    __u32 type;
    __u32 tmd;
    __u32 data_unit_loop_length;
    
    p_end = section + section_len;
    p     = section;
    //*dstLen = 0;
     if(0 == section_len)
        return 0;
    
    type = get8(&p, p_end); /* 2: TMD(Time control mode)  6: reserved */
    tmd  = (type >>6) & 0x03;
    if (0x02 ==  tmd || 0x01 == tmd)
    {
        get8(&p, p_end);     /* STM  Presentation start-time*/
        get8(&p, p_end); 
        get8(&p, p_end); 
        get8(&p, p_end); 
        get8(&p, p_end); 
    }
    data_unit_loop_length = get24(&p, p_end); /* data_unit_loop_length */
    desc_end = p + data_unit_loop_length;
    if (p_end < desc_end)
        return -1;
    groupa_data_unit(tmpItemInf, p, data_unit_loop_length, dst, dstLen, groupLen);
    
    p = p_end;    
    return 0;
}


static __s32 groupb_data_unit(CCedarSubtitleFrame *tmpItemInf, const __u8 *section, __s32 section_len, 
    __u8 *dst, __s32 *dstLen, __s32 *groupLen)
{
    const __u8 *p;
    const __u8 *p_end;
    //const __u8 *desc_end;
//    __u32 type;
//    __u32 tmd;
    //__u32 data_unit_parameter;
    //__s32 data_unit_size;
    __s32 len;
    __s32 i;
    
    
    p_end = section + section_len;
    p     = section;
    *dstLen = 0;
    WARNING("**groupb_data_unit p %x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x **\n",p[0],p[1],p[2],p[3],p[4],p[5],p[6],p[7],p[8],p[9],p[10],p[11],p[12],p[13],p[14],p[15],p[16],p[17],p[18],p[19]);

    if(0 == section_len)
        return 0;

    //len = data_unit(tmpItemInf, p, section_len, dst, dstLen, subNum);

    while(section_len > 0)
    {
        if(tmpItemInf->subNum >0)
        {
            if(tmpItemInf->subNum > ISDBDEC_MAX_LINE_NUM)
                break;
            for(i=0; i<tmpItemInf->subNum; i++)
            {
                groupLen++;
            }
        }
        len = b_data_unit(tmpItemInf, p, section_len, (dst+(*dstLen)), dstLen, groupLen);
        if (len < 0)
        {
            break;
        }
        else
        {
            section_len  = section_len - len - 5;  
            p += (len+5);
        }
    }

    p = p_end;   
    return 0;  
}

static __s32 groupc_data_unit(CCedarSubtitleFrame *tmpItemInf, const __u8 *section, __s32 section_len, 
    __u8 *dst, __s32 *dstLen, __s32 *groupLen)
{
    const __u8 *p;
    const __u8 *p_end;
    //const __u8 *desc_end;
//    __u32 type;
//    __u32 tmd;
    //__u32 data_unit_parameter;
    //__s32 data_unit_size;
    __s32 len;
    __s32 i;
    
    
    p_end = section + section_len;
    p     = section;
    *dstLen = 0;

    WARNING("**groupc_data_unit p %x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x **\n",p[0],p[1],p[2],p[3],p[4],p[5],p[6],p[7],p[8],p[9],p[10],p[11],p[12],p[13],p[14],p[15],p[16],p[17],p[18],p[19]);
    if(0 == section_len)
        return 0;
   
    //len = data_unit(tmpItemInf, p, section_len, dst, dstLen, subNum);

    while(section_len > 0)
    {
        if(tmpItemInf->subNum >0)
        {
            if(tmpItemInf->subNum > ISDBDEC_MAX_LINE_NUM)
                break;
            for(i=0; i<tmpItemInf->subNum; i++)
            {
                groupLen++;
            }
        }
        len = c_data_unit(tmpItemInf, p, section_len, (dst+(*dstLen)), dstLen, groupLen);
        if (len < 0)
        {
            break;
        }
        else
        {
            section_len  = section_len - len - 5;  
            p += (len+5);
        }
    }

    p = p_end;   
    return 0;  
}


static __s32 groupa_data_unit(CCedarSubtitleFrame *tmpItemInf, const __u8 *section, __s32 section_len, 
    __u8 *dst, __s32 *dstLen, __s32 *groupLen)
{
    const __u8 *p;
    const __u8 *p_end;
    //const __u8 *desc_end;
    //__u32 type;
    //__u32 tmd;
    //__u32 data_unit_parameter;
    //__s32 data_unit_size;
    __s32 len;
    __s32 i;
    
    
    p_end = section + section_len;
    p     = section;
    *dstLen = 0;
    WARNING("**groupa_data_unit p %x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x **\n",p[0],p[1],p[2],p[3],p[4],p[5],p[6],p[7],p[8],p[9],p[10],p[11],p[12],p[13],p[14],p[15],p[16],p[17],p[18],p[19]);
    if(0 == section_len)
        return 0;
   
    //len = data_unit(tmpItemInf, p, section_len, dst, dstLen, subNum);
    
    //初始化一些变量
    tmpItemInf->subStyleSize[0] = MAPLE_SUB_STYLE_NORMAL_SIZE; //Mobile closed caption类型默认字体类型；
    tmpItemInf->subColor[0] = MAPLE_SUB_COLOR_WHITE;    //Mobile的字体颜色默认为白色

    while(section_len > 0)
    {
        if(tmpItemInf->subNum >0)
        {
            if(tmpItemInf->subNum > ISDBDEC_MAX_LINE_NUM)
                break;
            for(i=0; i<tmpItemInf->subNum; i++)
            {
                groupLen++;
            }
        }
        len = a_data_unit(tmpItemInf, p, section_len, (dst+(*dstLen)), dstLen, groupLen);
        if (len < 0)
        {
            break;
        }
        else
        {
            section_len  = section_len - len - 5;  
            p += (len+5);
        }
    }
    
    p = p_end;   
    return 0;  
}

maple_subtitle_color_mode_t convertISDBCOL2MapleSubtitleColor(__s32 IsdbCol)  //把isdb字幕spec里规定的颜色值转为maple自己规定的颜色值
{
    maple_subtitle_color_mode_t nMapleSubColor = MAPLE_SUB_COLOR_WHITE;
    switch(IsdbCol)
    {
        case BKF:
            nMapleSubColor = MAPLE_SUB_COLOR_BLACK;
            break;
        case RDF:
            nMapleSubColor = MAPLE_SUB_COLOR_RED;
            break;
        case GRF:
            nMapleSubColor = MAPLE_SUB_COLOR_GREEN;
            break;
        case YLF:
            nMapleSubColor = MAPLE_SUB_COLOR_YELLOW;
            break;
        case BLF:
            nMapleSubColor = MAPLE_SUB_COLOR_BLUE;
            break;
        case MGF:
            nMapleSubColor = MAPLE_SUB_COLOR_MAGENTA;
            break;
        case CNF:
            nMapleSubColor = MAPLE_SUB_COLOR_CYAN;
            break;
        case WHF:
            nMapleSubColor = MAPLE_SUB_COLOR_WHITE;
            break;
        default:
            WARNING("fatal error! why isdb color = 0x%x?\n", IsdbCol);
            nMapleSubColor = MAPLE_SUB_COLOR_WHITE;
            break;
    }
    return nMapleSubColor;
}

static __s32 a_data_unit(CCedarSubtitleFrame *tmpItemInf, const __u8 *section, __s32 section_len, 
    __u8 *dst, __s32 *dstLen, __s32 *groupLen)
{
    const __u8 *p;
    const __u8 *p_end;
    const __u8 *desc_end;
    __u32 type = 0;
    //__u32 last_type =0;
    //__u32 tmd;
    __u32 data_unit_parameter;
    __s32 data_unit_size;
    __s32 len;
    __u32 tmpBuf[16];
    __s32 tmpPoint = 0;
    __s8  StyleSize_flags = 1;

    p_end = section + section_len;
    p     = section;
    //*dstLen = 0;
    if(0 == section_len)
        return 0;
    
    type = get8(&p, p_end);                       /* unit_separator */
    if (0x1f != type)
        return -1;
    
    data_unit_parameter = get8(&p, p_end);       /* data_unit_parameter */
            WARNING("a_data_unit data_unit_parameter = 0x%x\n", data_unit_parameter);	//test130702 

/*
Data unit 		Data unit 	Function
				parameter
Statement body	0x20 		Character data of caption statement is transmitted. Setting data
							of display area in caption management is transmitted.
Geometric 		0x28 		Geometric graphics data is transmitted

Synthesized sound	0x2c 		Synthesized sound information data is transmitted.
1-byte DRCS 		0x30 		1-bite DRCS pattern data is transmitted.
2-byte DRCS 		0x31 		2-bite DRCS pattern data is transmitted.
Color map 		0x34 		Color map data is transmitted.
Bit map 			0x35 		Bitmap data is transmitted.

*/

    if (0x28 == data_unit_parameter)
    {
        data_unit_size = get24(&p, p_end);           /* data_unit_size */
        desc_end = p + data_unit_size; 
        if (p_end < desc_end)
            return -1;
        p = desc_end; 
        return data_unit_size;
    }
    else if (0x2c == data_unit_parameter)
    {
        data_unit_size = get24(&p, p_end);           /* data_unit_size */
        desc_end = p + data_unit_size; 
        if (p_end < desc_end)
            return -1;
        p = desc_end; 
        return data_unit_size; 
    }
    else if (0x30 == data_unit_parameter)
    {
        data_unit_size = get24(&p, p_end);           /* data_unit_size */
        desc_end = p + data_unit_size; 
        if (p_end < desc_end)
            return -1;
        p = desc_end; 
        return data_unit_size;
    }
    else if (0x31 == data_unit_parameter)
    {
        data_unit_size = get24(&p, p_end);           /* data_unit_size */
        desc_end = p + data_unit_size; 
        if (p_end < desc_end)
            return -1;
        p = desc_end; 
        return data_unit_size;
    }
    else if (0x34 == data_unit_parameter)
    {
        data_unit_size = get24(&p, p_end);           /* data_unit_size */
        desc_end = p + data_unit_size; 
        if (p_end < desc_end)
            return -1;
        p = desc_end; 
        return data_unit_size;
    }
    else if (0x35 == data_unit_parameter)
    {
        data_unit_size = get24(&p, p_end);           /* data_unit_size */
        desc_end = p + data_unit_size; 
        if (p_end < desc_end)
            return -1;
        p = desc_end; 
        return data_unit_size;
    }
    else if (0x20 != data_unit_parameter)
        return -1;
    
    data_unit_size = get24(&p, p_end);           /* data_unit_size */
    desc_end = p + data_unit_size;
    if (p_end < desc_end)
        return -1;

    switch(data_unit_parameter) 
        {
        case 0x20:
            break;
        case 0x28:
        case 0x2c:
        case 0x30:
        case 0x31:
        case 0x34:
        case 0x35:
            WARNING("NO Character data of caption statement!\n");
            break;
        default:
            WARNING("NO Character data of caption statement!\n");
            break;
    }
            WARNING("a_data_unit data_unit_size = 0x%x\n", data_unit_size);	//test130702 

    len = data_unit_size;
//    if(data_unit_size == 0x0f
//        && p[0] == 0x0c
//        && p[1] == 0x83
//        && p[2] == 0x8a
//        && p[3] == 0xa4
//        && p[4] == 0xa8
//        && p[5] == 0xa1
//        && p[6] == 0xc1
//        && p[7] == 0xa4
//        && p[8] == 0xc3
//        && p[9] == 0xfd
//        && p[10] == 0xef
//        && p[11] == 0x9d
//        )
//    {
//        WARNING("find this spectial subtitle!\n");
//    }
    while(len > 0)
    {
        //last_type = type;
        type = get8(&p, desc_end);
        type &=0xff;
            //WARNING("a_data_unit type = (0x%x,%d)\n", type,len);	//test130702 

        len--;
        switch(type) 
        {
        //* C0 control set
        case NUL:
        case BEL:
            break;
        case SP:
            //*dst++ = SP;
            //*dst++ = SP;
            //*dstLen+= 2;
            *dst++ = SP;	//test130703 
            *dstLen+=1;
            break;
        case DEL:
            break;
        case PAPF:
            type = get8(&p, desc_end);
            type &=0xff;
            len--;
            break;
#if 0	//test130703 
        case APS:    // 默认换行;   
            type = get8(&p, desc_end);
            type &=0xff;
            len--;
            type = get8(&p, desc_end);
            type &=0xff;
            len--;
            break;  
#endif
        case CS:
            break;

        case CAN:
            break;
        case ESC:
            *dst++ = (__u8)type;
            *dstLen+= 1;
            break;
        case LS1:
        case LS0:
            *dst++ = (__u8)type;
            *dstLen+= 1;
            break;
        case SS2:
            *dst++ = (__u8)type;
            *dstLen+= 1;
            type = get8(&p, desc_end);
            type &=0xff;
            len--;
            *dst++ = (__u8)type;
            *dstLen+= 1;        
            break;
        case SS3:
            *dst++ = (__u8)type;
            *dstLen+= 1;
            type = get8(&p, desc_end);
            type &=0xff;
            len--;
            *dst++ = (__u8)type;
            *dstLen+= 1;        
            break;
        case RS:
            //*dst++ = (__u8)type;
            //*dstLen+= 1;
            break;
        case US:
            WARNING(" Be careful: Data unit!\n");
            return -1;
             
        //* C1 control set  
        case BKF:
        case RDF:
        case GRF:
        case YLF:
        case BLF:
        case MGF:
        case CNF:
        case WHF:   //对于oneseg的字幕,我们假定:如果字幕项里有颜色信息, 一定每一行都会有颜色信息. 不换颜色我们就不换行.
            if(tmpItemInf->subNum >= ISDBDEC_MAX_LINE_NUM)    //首先判断是否超出最大行数
            {
                WARNING("fatal error: tmpItemInf->subNum[%d] >= ISDBDEC_MAX_LINE_NUM?\n", tmpItemInf->subNum);
                break;
            }
            tmpItemInf->subColor[tmpItemInf->subNum] = convertISDBCOL2MapleSubtitleColor(type);
            if(tmpItemInf->subNum > 0)  //换行从碰到第二种颜色开始.
            {
               *groupLen = *dstLen;     //把到换新行之前存储的字节总数赋给数组groupLen.
                groupLen++;
            }
            tmpItemInf->subNum++;
            break;
        case COL:
            type = get8(&p, desc_end);
            type &=0xff;
            len--;
            if (type == 0x20)
            {
                type = get8(&p, desc_end);
                type &=0xff;
                len--;
            }
            else if((type > 0x7f) && (type < 0x48))
            {
                WARNING("## fatal error: COL error, check code!\n");
            }
            break;
        case POL:
            get8(&p, desc_end);
            len--;
            break;               
//            case SSZ:
//            case MSZ:
//            case NSZ:
//                break;
        case SZX:
            get8(&p, desc_end);
            len--;
            break;
        case FLC:
            get8(&p, desc_end);
            len--;
            break;
        case CDC:
            type = get8(&p, desc_end);
            type &=0xff;
            len--;
            if (type == 0x20)
            {
                type = get8(&p, desc_end);
                type &=0xff;
                len--;
            }
            else if((type != 0x40) && (type != 0x4f))
            {
                WARNING("## fatal error: CDC error, check code!\n");
            }
            break;                
        case WMM:
            get8(&p, desc_end);
            len--;
            break;
        case TIME:
            type = get8(&p, desc_end);
            type &=0xff;
            len--;
                WARNING("## data_unit TIME 0x%x\n",type);	//test130704 
            if(type == 0x20)
            {
                type = get8(&p, desc_end);
                type &=0xff;
                len--;
                if( (type >= 0x40) && (type <= 0x7f))
                    tmpItemInf->uDuration += (type - 0x40)*100;          //subtitle duration times: ms;
            }
            else if(type == 0x28)
            {
                type = get8(&p, desc_end);
                type &=0xff;
                len--;
            }
            else
            {
                WARNING("Be careful!!TIME : type[0x%x] is not processed!\n", type);
            }
            break;
        case MACRO:
            get8(&p, desc_end);
            len--;
            break;
        case RPC:
            get8(&p, desc_end);
            len--;
            break;
        case STL:
            break;
        case HLC:
            get8(&p, desc_end);
            len--;
            break;
        case CSI:
            tmpPoint = 0;
            do{
                type = get8(&p, desc_end);
                type &=0xff;
                *(tmpBuf+tmpPoint) = type;
                tmpPoint++;
                len--;
                if(type == 0x42)
                {
                    //WARNING(" CSI: GSM, Character deformation!\n");
                    break;
                }    
                else if ((type >= 0x53) && (type <= 0x6f))
                {
                    //WARNING(" CSI: Extension control code!\n");
                    break;
                } 
                else if(tmpPoint > 15)
                {
                    type = 0x00;
                    break;
                }                
            }while(1);
            
            break;
        case SSZ:
            if (StyleSize_flags == 0)
            {
                if(tmpItemInf->subNum > 0)
                {
                    tmpItemInf->subStyleSize[tmpItemInf->subNum-1] = MAPLE_SUB_STYLE_SMALL_SIZE;
                }
                else
                {
                    tmpItemInf->subStyleSize[0] = MAPLE_SUB_STYLE_SMALL_SIZE;
                }
                
                StyleSize_flags = 1;
            }  
            //*dst++ = (__u8)type;
            //*dstLen+= 1;
            break;
        case MSZ:
           if (StyleSize_flags == 0)
           {
               if(tmpItemInf->subNum > 0)
               {
                   tmpItemInf->subStyleSize[tmpItemInf->subNum-1] = MAPLE_SUB_STYLE_MIDDLE_SIZE;
               }
               else
               {
                   tmpItemInf->subStyleSize[0] = MAPLE_SUB_STYLE_MIDDLE_SIZE;
               }
               
               StyleSize_flags = 1;
           }  
           //*dst++ = (__u8)type;
           //*dstLen+= 1;
           break;
        case NSZ:
           if (StyleSize_flags == 0)
           {
               if(tmpItemInf->subNum > 0)
               {
                   tmpItemInf->subStyleSize[tmpItemInf->subNum-1] = MAPLE_SUB_STYLE_NORMAL_SIZE;
               }
               else
               {
                   tmpItemInf->subStyleSize[0] = MAPLE_SUB_STYLE_NORMAL_SIZE;
               }
               
               StyleSize_flags = 1;
           }  
           //*dst++ = (__u8)type;
           //*dstLen+= 1;
           break;            
        case APB:
        case APF:
        case APD:
        case APU:
           //*dst++ = (__u8)type;
           //*dstLen+= 1;
            break;

#if 1	//test130703 
        case APS:    // 默认换行;   
            type = get8(&p, desc_end);
            type &=0xff;
            len--;
            type = get8(&p, desc_end);
            type &=0xff;
            len--;
            //break;  
#endif
        case APR:
            if(tmpItemInf->subNum >= ISDBDEC_MAX_LINE_NUM)    //首先判断是否超出最大行数
            {
                WARNING("fatal error: tmpItemInf->subNum[%d] >= ISDBDEC_MAX_LINE_NUM?\n", tmpItemInf->subNum);
                break;
            }
            if(tmpItemInf->subNum > 0)  //换行从碰到第二种颜色开始.
            {
               *groupLen = *dstLen;     //把到换新行之前存储的字节总数赋给数组groupLen.
                groupLen++;
            }
            tmpItemInf->subNum++;
            break;

        default:
            *dst++ = (__u8)type;
            *dstLen+= 1;
            break;
        }
    }    
    p = desc_end; 
    return data_unit_size;
}



static __s32 b_data_unit(CCedarSubtitleFrame *tmpItemInf, const __u8 *section, __s32 section_len, 
    __u8 *dst, __s32 *dstLen, __s32 *groupLen)
{
    const __u8 *p;
    const __u8 *p_end;
    const __u8 *desc_end;
    __u32 type = 0;
    __u32 last_type =0;
    //__u32 tmd;
    __u32 data_unit_parameter;
    __s32 data_unit_size;
    __s32 len;
    __s32 i;
    __u32 tmp = 0;
    __u32 tmpBuf[16];
    __s32 tmpPoint = 0;
    //__s8  APS_flags = 0;
    __s8  SDF_flags = 0;
    __s8  SDP_flags = 0;
    __s8  SWF_flags = 0;
    //__s8  ORN_flags = 0;
    __s8  StyleSize_flags = 0;

    maple_subtitle_style_size_t  StyleSize  = MAPLE_SUB_STYLE_NORMAL_SIZE, LastStyleSize = MAPLE_SUB_STYLE_NORMAL_SIZE;
    maple_subtitle_invoke_code_t InvodeCode = MAPLE_SUB_INVOKE_NONE;

    __s8 single_shift_flags = 0;
    __u8 single_shift_data[2];
    __s16 AcpsDataX = 0, AcpsDataY = 0;
    
    p_end = section + section_len;
    p     = section;
    //*dstLen = 0;
    if(0 == section_len)
        return 0;
    
    type = get8(&p, p_end);                       /* unit_separator */
    if (0x1f != type)
        return -1;
      
    data_unit_parameter = get8(&p, p_end);       /* data_unit_parameter */
    if (0x28 == data_unit_parameter)
    {
        data_unit_size = get24(&p, p_end);           /* data_unit_size */
        desc_end = p + data_unit_size; 
        if (p_end < desc_end)
            return -1;
        p = desc_end; 
        return data_unit_size;
    }
    else if (0x2c == data_unit_parameter)
    {
        data_unit_size = get24(&p, p_end);           /* data_unit_size */
        desc_end = p + data_unit_size; 
        if (p_end < desc_end)
            return -1;
        p = desc_end; 
        return data_unit_size; 
    }
    else if (0x30 == data_unit_parameter)
    {
        data_unit_size = get24(&p, p_end);           /* data_unit_size */
        desc_end = p + data_unit_size; 
        if (p_end < desc_end)
            return -1;
        p = desc_end; 
        return data_unit_size;
    }
    else if (0x31 == data_unit_parameter)
    {
        data_unit_size = get24(&p, p_end);           /* data_unit_size */
        desc_end = p + data_unit_size; 
        if (p_end < desc_end)
            return -1;
        p = desc_end; 
        return data_unit_size;
    }
    else if (0x34 == data_unit_parameter)
    {
        data_unit_size = get24(&p, p_end);           /* data_unit_size */
        desc_end = p + data_unit_size; 
        if (p_end < desc_end)
            return -1;
        p = desc_end; 
        return data_unit_size;
    }
    else if (0x35 == data_unit_parameter)
    {
        data_unit_size = get24(&p, p_end);           /* data_unit_size */
        desc_end = p + data_unit_size; 
        if (p_end < desc_end)
            return -1;
        p = desc_end; 
        return data_unit_size;
    }
    else if (0x20 != data_unit_parameter)
        return -1;
    
    data_unit_size = get24(&p, p_end);           /* data_unit_size */
    desc_end = p + data_unit_size;
    if (p_end < desc_end)
        return -1;

    switch(data_unit_parameter) 
        {
        case 0x20:
            break;
        case 0x28:
        case 0x2c:
        case 0x30:
        case 0x31:
        case 0x34:
        case 0x35:
            WARNING("NO Character data of caption statement!\n");
            break;
        default:
            WARNING("NO Character data of caption statement!\n");
            break;
    }

    len = data_unit_size;
//    if(data_unit_size == 0x53
//            && p[0x3f] == 0x24
//            && p[0x3f+1] == 0x47
//            && p[0x3f+2] == 0x24
//            && p[0x3f+3] == 0x62
//            && p[0x3f+4] == 0x24
//            && p[0x3f+5] == 0x35
//            && p[0x3f+6] == 0x89
//            && p[0x3f+7] == 0x20
//            && p[0x3f+8] == 0x8a
//            && p[0x3f+9] == 0x4b
//            && p[0x3f+10] == 0x4d
//            && p[0x3f+11] == 0x24
//            )
//        {
//            WARNING("find this empty subtitle!\n");
//        }
    while(len > 0)
    {
        last_type = type;
        type = get8(&p, desc_end);
        type &=0xff;
        len--;
        switch(type) 
        {
        //* C0 control set
        case NUL:
        case BEL:
            break;
        case SP:
            *dst++ = SP;
            *dst++ = SP;
            *dstLen+= 2;
            break;
        case DEL:
            break;
        case PAPF:
            type = get8(&p, desc_end);        //暂不解析；
            type &=0xff;
            len--;
            break;
        case APR:    // 默认换行;   
            tmpItemInf->position_flags = 1;
            if ((last_type == SSZ) || (last_type == MSZ) || (last_type == NSZ))
            {
                if(tmpItemInf->subNum > 0)
                {
                    tmpItemInf->subStyleSize[tmpItemInf->subNum-1] = LastStyleSize;
                }
                else
                {
                    tmpItemInf->subStyleSize[0] = LastStyleSize;
                }
            }
            
            if(tmpItemInf->subNum >0)
            {
               *groupLen = *dstLen; 
                groupLen++;
            }
            tmpItemInf->subNum++;  
            
            StyleSize_flags = 0;
            if(tmpItemInf->subNum > 0)
            {
                tmpItemInf->subStyleSize[tmpItemInf->subNum-1] = StyleSize;
            }
            else
            {
                tmpItemInf->subStyleSize[0] = StyleSize;
            }
            if (tmpItemInf->subNum > ISDBDEC_MAX_LINE_NUM)
            {
                tmpItemInf->subNum = ISDBDEC_MAX_LINE_NUM;
                groupLen--;
                WARNING("Be careful: tmpItemInf->subNum > ISDBDEC_MAX_LINE_NUM?\n");
            }  
            if (single_shift_flags == 1)
            {
                *dst++ = single_shift_data[0];
                *dstLen+= 1;
                *dst++ = single_shift_data[1];
                *dstLen+= 1;
            }
            if (InvodeCode == MAPLE_SUB_INVOKE_NONE)
            {
                ;
            }
            else if(InvodeCode == MAPLE_SUB_INVOKE_LS0)
            {
                *dst++ = 0x0f;
                *dstLen+= 1;
            }
            else if(InvodeCode == MAPLE_SUB_INVOKE_LS1)
            {
                *dst++ = 0x0e;
                *dstLen+= 1;
            }
            else if(InvodeCode == MAPLE_SUB_INVOKE_LS2)
            {
                *dst++ = 0x1b;
                *dst++ = 0x6e;
                *dstLen+= 2;
            }
            else if(InvodeCode == MAPLE_SUB_INVOKE_LS3)
            {
                *dst++ = 0x1b;
                *dst++ = 0x6f;
                *dstLen+= 2;
            }
            else if(InvodeCode == MAPLE_SUB_INVOKE_LS1R)
            {
                *dst++ = 0x1b;
                *dst++ = 0x7e;
                *dstLen+= 2;
            }
            else if(InvodeCode == MAPLE_SUB_INVOKE_LS2R)
            {
                *dst++ = 0x1b;
                *dst++ = 0x7d;
                *dstLen+= 2;
            }
            else if(InvodeCode == MAPLE_SUB_INVOKE_LS3R)
            {
                *dst++ = 0x1b;
                *dst++ = 0x7c;
                *dstLen+= 2;
            }
            type = get8(&p, desc_end);
            type &=0xff;
            len--;
            //tmpItemInf->line[tmpItemInf->subNum-1] = (__s16)(type - 0x40)*2;
            if (StyleSize == MAPLE_SUB_STYLE_SMALL_SIZE)
            {
                tmpItemInf->line[tmpItemInf->subNum-1] = (__s16)(type - 0x40);
            }
            else
            {
                tmpItemInf->line[tmpItemInf->subNum-1]= (__s16)(type - 0x40)*2;
            }       
            type = get8(&p, desc_end);
            type &=0xff;
            len--;  
//            if (last_type == SSZ || (last_type == MSZ))
//            {
//                tmpItemInf->forward[tmpItemInf->subNum-1]= (__s16)(type - 0x40);
//            }
//            else
//            {
//                tmpItemInf->forward[tmpItemInf->subNum-1]= (__s16)(type - 0x40)*2;
//            }
            if (StyleSize == MAPLE_SUB_STYLE_SMALL_SIZE || (StyleSize == MAPLE_SUB_STYLE_MIDDLE_SIZE))
            {
                tmpItemInf->forward[tmpItemInf->subNum-1]= (__s16)(type - 0x40);
            }
            else
            {
                tmpItemInf->forward[tmpItemInf->subNum-1]= (__s16)(type - 0x40)*2;
            }       
            break;   
        case CS:
        case CAN:
            break;
        case ESC:
            *dst++ = (__u8)type;
            *dstLen+= 1;
            type = get8(&p, desc_end);
            type &=0xff;
            len--;
            if(type == 0x6e)
                InvodeCode = MAPLE_SUB_INVOKE_LS2;
            else if(type == 0x6f)
                InvodeCode = MAPLE_SUB_INVOKE_LS3;
            else if(type == 0x7e)
                InvodeCode = MAPLE_SUB_INVOKE_LS1R;
            else if(type == 0x7d)
                InvodeCode = MAPLE_SUB_INVOKE_LS2R;
            else if(type == 0x7c)
                InvodeCode = MAPLE_SUB_INVOKE_LS3R;
            else    //不是invocation,就是designation of graphic sets
            {
                InvodeCode = MAPLE_SUB_INVOKE_NONE;
                *dst++ = (__u8)type;
                *dstLen+= 1;
                if(((type>>4)&0x0f)==0x02)
                {
                    for(i=0;i<3;i++)    //根据spec,designation去掉ESC,最多4字节,已经读了一个字节,再读3字节,正常情况下应该整个字段读完
                    {
                        type = get8(&p, desc_end);
                        if(-1 == (__s32)type)
                        {
                            WARNING("fatal error! read buffer end! i=[%d], p[0x%x]\n", i, p);
                            break;
                        }
                        type &=0xff;
                        len--;
                        *dst++ = (__u8)type;
                        *dstLen+= 1;
                        if(((type>>4)&0x0f)!=0x02)
                        {
                            break;
                        }
                    }
                    if(i >= 3)
                    {
                        WARNING("fatal error! ESC designation exceed 5 bytes?\n");
                    }
                }
                else
                {
                    WARNING("fatal error! ESC designation wrong!p[0x%x]\n",p);
                }
                break;
            }
            
            *dst++ = (__u8)type;
            *dstLen+= 1;
            break;
        case LS1:
            InvodeCode = MAPLE_SUB_INVOKE_LS1;
            *dst++ = (__u8)type;
            *dstLen+= 1;
            break;
        case LS0:
            InvodeCode = MAPLE_SUB_INVOKE_LS0;
            *dst++ = (__u8)type;
            *dstLen+= 1;
            break;
        case SS2:
            single_shift_flags = 1;
            *dst++ = (__u8)type;
            single_shift_data[0] = (__u8)type;
            *dstLen+= 1;
            type = get8(&p, desc_end);
            type &=0xff;
            len--;
            *dst++ = (__u8)type;
            single_shift_data[1] = (__u8)type;
            *dstLen+= 1;           
            break;
        case SS3:
            single_shift_flags = 1;
            *dst++ = (__u8)type;
            single_shift_data[0] = (__u8)type;
            *dstLen+= 1;
            type = get8(&p, desc_end);
            type &=0xff;
            len--;
            *dst++ = (__u8)type;
            single_shift_data[1] = (__u8)type;
            *dstLen+= 1;  
            break;
        case RS:
            //*dst++ = (__u8)type;
            //*dstLen+= 1;
            break;
        case US:
            WARNING(" Be careful: Data unit!\n");
            return -1;
             
        //* C1 control set  
        case BKF:
            if(tmpItemInf->subNum > 0)
            {
                tmpItemInf->subColor[tmpItemInf->subNum-1] = MAPLE_SUB_COLOR_BLACK;
            }
            else
            {
                tmpItemInf->subColor[0] = MAPLE_SUB_COLOR_BLACK;
            }
            break;
        case RDF:
            if(tmpItemInf->subNum > 0)
            {
                tmpItemInf->subColor[tmpItemInf->subNum-1] = MAPLE_SUB_COLOR_RED;
            }
            else
            {
                tmpItemInf->subColor[0] = MAPLE_SUB_COLOR_RED;
            }
            break;
        case GRF:
            if(tmpItemInf->subNum > 0)
            {
                tmpItemInf->subColor[tmpItemInf->subNum-1] = MAPLE_SUB_COLOR_GREEN;
            }
            else
            {
                tmpItemInf->subColor[0] = MAPLE_SUB_COLOR_GREEN;
            }
            break;
        case YLF:
            if(tmpItemInf->subNum > 0)
            {
                tmpItemInf->subColor[tmpItemInf->subNum-1] = MAPLE_SUB_COLOR_YELLOW;
            }
            else
            {
                tmpItemInf->subColor[0] = MAPLE_SUB_COLOR_YELLOW;
            }
            break;
        case BLF:
            if(tmpItemInf->subNum > 0)
            {
                tmpItemInf->subColor[tmpItemInf->subNum-1] = MAPLE_SUB_COLOR_BLUE;
            }
            else
            {
                tmpItemInf->subColor[0] = MAPLE_SUB_COLOR_BLUE;
            }
            break;
        case MGF:
            if(tmpItemInf->subNum > 0)
            {
                tmpItemInf->subColor[tmpItemInf->subNum-1] = MAPLE_SUB_COLOR_MAGENTA;
            }
            else
            {
                tmpItemInf->subColor[0] = MAPLE_SUB_COLOR_MAGENTA;
            }
            break;
        case CNF:
            if(tmpItemInf->subNum > 0)
            {
                tmpItemInf->subColor[tmpItemInf->subNum-1] = MAPLE_SUB_COLOR_CYAN;
            }
            else
            {
                tmpItemInf->subColor[0] = MAPLE_SUB_COLOR_CYAN;
            }
            break;
        case WHF:
            if(tmpItemInf->subNum > 0)
            {
                tmpItemInf->subColor[tmpItemInf->subNum-1] = MAPLE_SUB_COLOR_WHITE;
            }
            else
            {
                tmpItemInf->subColor[0] = MAPLE_SUB_COLOR_WHITE;
            }
            break;
        case COL:
            type = get8(&p, desc_end);
            type &=0xff;
            len--;
            if (type == 0x20)
            {
                type = get8(&p, desc_end);
                type &=0xff;
                len--;
            }
            else if((type > 0x7f) && (type < 0x48))
            {
                WARNING("Be careful: COL error, check code!\n");
            }
            break;
        case POL:
            get8(&p, desc_end);
            len--;
            break;               
//            case SSZ:
//            case MSZ:
//            case NSZ:
//                break;
        case SZX:
            get8(&p, desc_end);
            len--;
            break;
        case FLC:
            get8(&p, desc_end);
            len--;
            break;
        case CDC:
            type = get8(&p, desc_end);
            type &=0xff;
            len--;
            if (type == 0x20)
            {
                type = get8(&p, desc_end);
                type &=0xff;
                len--;
            }
            else if((type != 0x40) && (type != 0x4f))
            {
                WARNING("Be careful: CDC error, check code!\n");
            }
            break;                
        case WMM:
            get8(&p, desc_end);
            len--;
            break;
        case TIME:
            type = get8(&p, desc_end);
            type &=0xff;
            len--;
            if(type == 0x20)
            {
                type = get8(&p, desc_end);
                type &=0xff;
                len--;
                if( (type >= 0x40) && (type <= 0x7f))
                    tmpItemInf->uDuration += (type - 0x40)*100;          //subtitle duration times: ms;
            }
            else if(type == 0x28)
            {
                type = get8(&p, desc_end);
                type &=0xff;
                len--;
            }
            else
            {
                WARNING("Be careful! TIME : type[0x%x] is not processed!\n", type);
            }
            break;
        case MACRO:
            get8(&p, desc_end);
            len--;
            break;
        case RPC:
            get8(&p, desc_end);
            len--;
            break;
        case STL:
            break;
        case HLC:
            get8(&p, desc_end);
            len--;
            break;
        case CSI:
            tmpPoint = 0;
            do{
                type = get8(&p, desc_end);
                type &=0xff;
                *(tmpBuf+tmpPoint) = type;
                tmpPoint++;
                len--;
                if(type == 0x42)
                {
                    //WARNING(" CSI: GSM, Character deformation!\n");
                    break;
                }    
                else if ((type >= 0x53) && (type <= 0x6f))
                {
                    //WARNING(" CSI: Extension control code!\n");
                    break;
                } 
                else if(tmpPoint > 15)
                {
                    type = 0x00;
                    break;
                }                
            }while(1);
            if(type == 0x53)  //* SWF
            { 
                if (SWF_flags != 0)
                {
                    break;
                }
                else
                {
                    SWF_flags = 1;
                }  
                i = 0;
                tmp = 0;
                while((tmpBuf[i] != 0x20) && (i < tmpPoint))
                {
                    tmp*=10; 
                    tmp = tmp + tmpBuf[i] - 0x30;
                    i++;
                }    
                if ( tmp == 0)
                {
                    ;   
                }
                else if(tmp == 1)
                {
                   ;  
                }
                else if(tmp == 2)
                {
                   ;
                }
                else if(tmp == 3)
                {
                    ;  
                }
                else if(tmp == 4)
                {
                    ;      
                }
                else if(tmp == 5)
                {
                    tmpItemInf->videoWidth  = 1920;
                    tmpItemInf->videoHeight = 1080;      
                }
                else if(tmp == 6)
                {
                    tmpItemInf->videoWidth  = 1920;
                    tmpItemInf->videoHeight = 1080;      
                }
                else if(tmp == 7)
                {
                    tmpItemInf->videoWidth  = 960;
                    tmpItemInf->videoHeight = 540;      
                }
                else if(tmp == 8)
                {
                    tmpItemInf->videoWidth  = 960;
                    tmpItemInf->videoHeight = 540;      
                }
                else if(tmp == 9)
                {
                    tmpItemInf->videoWidth  = 720;
                    tmpItemInf->videoHeight = 480;      
                }
                else if(tmp == 10)
                {
                    tmpItemInf->videoWidth  = 720;
                    tmpItemInf->videoHeight = 480;      
                }
                else if(tmp == 11)
                {
                    tmpItemInf->videoWidth  = 1280;
                    tmpItemInf->videoHeight = 720;      
                }
                else if(tmp == 12)
                {
                    tmpItemInf->videoWidth  = 1280;
                    tmpItemInf->videoHeight = 720;      
                }
                break;
            } 
            else if (type == 0x56)          //* SDF
            {
            
                if(SDF_flags != 0)
                {
                    break;
                }
                else
                {
                    SDF_flags = 1;
                }
                
                tmpItemInf->subWidth = 0;
                tmpItemInf->subHeight = 0;
                i = 0;
                do{
                    if(tmpBuf[i] != 0x3b)
                    {
                        tmpItemInf->subWidth *=10;
                        tmpItemInf->subWidth = tmpItemInf->subWidth+tmpBuf[i] - 0x30;
                        i++;
                    }
                    else
                    {
                        i++;
                        break;
                    }
                    
                }while(i < tmpPoint);

                while((tmpBuf[i] != 0x20) && (i < tmpPoint))
                {
                    tmpItemInf->subHeight*=10; 
                    tmpItemInf->subHeight= tmpItemInf->subHeight+tmpBuf[i] - 0x30;
                    i++;
                }
                break;
            }
            else if(type == 0x61)  //* ACPS
            {
                tmpItemInf->position_flags = 1;  
                if(tmpItemInf->subNum >0) 
                {
                   *groupLen = *dstLen; 
                    groupLen++;
                }
                tmpItemInf->subNum++;   
                if (tmpItemInf->subNum > ISDBDEC_MAX_LINE_NUM)
                {
                    tmpItemInf->subNum = ISDBDEC_MAX_LINE_NUM;
                    groupLen--;
                    WARNING("fatal error: tmpItemInf->subNum > ISDBDEC_MAX_LINE_NUM?\n");
                } 
                tmpItemInf->subStyleSize[tmpItemInf->subNum-1] = StyleSize;               
                AcpsDataX = 0;
                AcpsDataY = 0;  
                if (single_shift_flags == 1)
                {
                    *dst++ = single_shift_data[0];
                    *dstLen+= 1;
                    *dst++ = single_shift_data[1];
                    *dstLen+= 1;
                }
                if (InvodeCode == MAPLE_SUB_INVOKE_NONE)
                {
                    ;
                }
                else if(InvodeCode == MAPLE_SUB_INVOKE_LS0)
                {
                    *dst++ = 0x0f;
                    *dstLen+= 1;
                }
                else if(InvodeCode == MAPLE_SUB_INVOKE_LS1)
                {
                    *dst++ = 0x0e;
                    *dstLen+= 1;
                }
                else if(InvodeCode == MAPLE_SUB_INVOKE_LS2)
                {
                    *dst++ = 0x1b;
                    *dst++ = 0x6e;
                    *dstLen+= 2;
                }
                else if(InvodeCode == MAPLE_SUB_INVOKE_LS3)
                {
                    *dst++ = 0x1b;
                    *dst++ = 0x6f;
                    *dstLen+= 2;
                }
                else if(InvodeCode == MAPLE_SUB_INVOKE_LS1R)
                {
                    *dst++ = 0x1b;
                    *dst++ = 0x7e;
                    *dstLen+= 2;
                }
                else if(InvodeCode == MAPLE_SUB_INVOKE_LS2R)
                {
                    *dst++ = 0x1b;
                    *dst++ = 0x7d;
                    *dstLen+= 2;
                }
                else if(InvodeCode == MAPLE_SUB_INVOKE_LS3R)
                {
                    *dst++ = 0x1b;
                    *dst++ = 0x7c;
                    *dstLen+= 2;
                }
                i = 0;
                do{
                    if(tmpBuf[i] != 0x3b)
                    {
                        AcpsDataX *=10; 
                        AcpsDataX = AcpsDataX+tmpBuf[i] - 0x30;
                        i++;
                    }
                    else
                    {
                        i++;
                        break;
                    }
                    
                }while(i < tmpPoint);
                
                while((tmpBuf[i] != 0x20) && (i < tmpPoint))
                {
                    AcpsDataY *=10; 
                    AcpsDataY = AcpsDataY + tmpBuf[i] - 0x30;
                    i++;
                }
                tmpItemInf->line[tmpItemInf->subNum-1]=(AcpsDataY - tmpItemInf->startY)/30;
                tmpItemInf->forward[tmpItemInf->subNum-1]=(AcpsDataX - tmpItemInf->startX)/20;
                break;
            }
            else if(type == 0x5f)     //* SDP
            {
                if (SDP_flags != 0)
                {
                    break;
                }
                else
                {
                    SDP_flags = 1;
                }  
                tmpItemInf->position_flags = 1;
                tmpItemInf->startX = 0;
                tmpItemInf->startY = 0;

                i = 0;
                do{
                    if(tmpBuf[i] != 0x3b)
                    {
                        tmpItemInf->startX *=10; 
                        tmpItemInf->startX = tmpItemInf->startX+tmpBuf[i] - 0x30;
                        i++;
                    }
                    else
                    {
                        i++;
                        break;
                    }
                    
                }while(i < tmpPoint);
                

                while((tmpBuf[i] != 0x20) && (i < tmpPoint))
                {
                    tmpItemInf->startY*=10; 
                    tmpItemInf->startY= tmpItemInf->startY+tmpBuf[i] - 0x30;
                    i++;
                }
                break;
            }
            break;
        case SSZ:
            if (StyleSize_flags == 0)
            {
                if(tmpItemInf->subNum > 0)
                {
                    tmpItemInf->subStyleSize[tmpItemInf->subNum-1] = MAPLE_SUB_STYLE_SMALL_SIZE;
                }
                else
                {
                    tmpItemInf->subStyleSize[0] = MAPLE_SUB_STYLE_SMALL_SIZE;
                }
                
                StyleSize_flags = 1;
            }  
            LastStyleSize = StyleSize;
            StyleSize = MAPLE_SUB_STYLE_SMALL_SIZE;
            //*dst++ = (__u8)type;
            //*dstLen+= 1;
            break;
        case MSZ:
           if (StyleSize_flags == 0)
           {
               if(tmpItemInf->subNum > 0)
               {
                   tmpItemInf->subStyleSize[tmpItemInf->subNum-1] = MAPLE_SUB_STYLE_MIDDLE_SIZE;
               }
               else
               {
                   tmpItemInf->subStyleSize[0] =  MAPLE_SUB_STYLE_MIDDLE_SIZE;
               }
               StyleSize_flags = 1;
           }  
           LastStyleSize = StyleSize;
           StyleSize = MAPLE_SUB_STYLE_MIDDLE_SIZE;
           
           if(tmpItemInf->subNum > 0)
           {
               if (tmpItemInf->subStyleSize[tmpItemInf->subNum-1] ==  MAPLE_SUB_STYLE_SMALL_SIZE)
                   tmpItemInf->subStyleSize[tmpItemInf->subNum-1] = MAPLE_SUB_STYLE_MIDDLE_SIZE;
           }
           else
           {
               if (tmpItemInf->subStyleSize[0] ==  MAPLE_SUB_STYLE_SMALL_SIZE)
                   tmpItemInf->subStyleSize[0] = MAPLE_SUB_STYLE_MIDDLE_SIZE;
           }
           //*dst++ = (__u8)type;
           //*dstLen+= 1;
           break;
        case NSZ:
           if (StyleSize_flags == 0)
           {
               if(tmpItemInf->subNum > 0)
               {
                   tmpItemInf->subStyleSize[tmpItemInf->subNum-1] =  MAPLE_SUB_STYLE_NORMAL_SIZE;
               }
               else
               {
                   tmpItemInf->subStyleSize[0] =  MAPLE_SUB_STYLE_NORMAL_SIZE;
               }
               StyleSize_flags = 1;
           } 
           LastStyleSize = StyleSize;
           StyleSize = MAPLE_SUB_STYLE_NORMAL_SIZE;
           
           if(tmpItemInf->subNum > 0)
           {
               if (tmpItemInf->subStyleSize[tmpItemInf->subNum-1] ==  MAPLE_SUB_STYLE_SMALL_SIZE)
                   tmpItemInf->subStyleSize[tmpItemInf->subNum-1] = MAPLE_SUB_STYLE_NORMAL_SIZE;
           }
           else
           {
               if (tmpItemInf->subStyleSize[0] ==  MAPLE_SUB_STYLE_SMALL_SIZE)
                   tmpItemInf->subStyleSize[0] = MAPLE_SUB_STYLE_NORMAL_SIZE;
           }
           //*dst++ = (__u8)type;
           //*dstLen+= 1;
           break;            
        case APB:
        case APF:
        case APD:
        case APU:
           //*dst++ = (__u8)type;
           //*dstLen+= 1;
            break;
        case APS:
            //if(tmpItemInf->subNum > 0)
            //{
            //    tmpItemInf->forward[tmpItemInf->subNum-1] = 0;
            //}
            //else
            //{
            //    tmpItemInf->forward[0] = 0;
            //}
            //*dst++ = (__u8)type;
            //*dstLen+= 1;
            break;
        default:
            *dst++ = (__u8)type;
            *dstLen+= 1;
            break;
        }
    }    
    p = desc_end; 
    return data_unit_size;
}


static __s32 c_data_unit(CCedarSubtitleFrame *tmpItemInf, const __u8 *section, __s32 section_len, 
    __u8 *dst, __s32 *dstLen, __s32 *groupLen)
{
    const __u8 *p;
    const __u8 *p_end;
    const __u8 *desc_end;
    __u32 type = 0;  
    __u32 last_type =0;
    __u32 data_unit_parameter;
    __s32 data_unit_size;
    __s32 len;
    __s32 i;
    __u32 tmp  = 0;
    __u32 tmpBuf[16];
    __s32 tmpPoint = 0;
    //__s8  APS_flags = 0;
    __s8  SDF_flags = 0;
    __s8  SDP_flags = 0;
    __s8  SWF_flags = 0; 
    
    __s8  SSM_flags = 0;
    __s8  SHS_flags = 0;
    __s8  SVS_flags = 0;
    //__s8  ORN_flags = 0;
    maple_subtitle_style_size_t  StyleSize  = MAPLE_SUB_STYLE_NORMAL_SIZE, LastStyleSize = MAPLE_SUB_STYLE_NORMAL_SIZE;
    maple_subtitle_invoke_code_t InvodeCode = MAPLE_SUB_INVOKE_NONE;

    __s8 single_shift_flags = 0;
    __u8 single_shift_data[2];
    
    __s8  StyleSize_flags = 0;
    //__s8  ACPS_flags = 0;
    __s16 ssmData = 0;
    __s16 shsData = 0, svsData = 0;
    __s16 AcpsDataX = 0, AcpsDataY = 0;
    //__s16 line = 0, forward = 0;

    p_end = section + section_len;
    p     = section;
    //*dstLen = 0;
    if(0 == section_len)
        return 0;
    
    type = get8(&p, p_end);                       /* unit_separator */
    if (0x1f != type)
        return -1;
    
    data_unit_parameter = get8(&p, p_end);       /* data_unit_parameter */
    if (0x28 == data_unit_parameter)
    {
        data_unit_size = get24(&p, p_end);           /* data_unit_size */
        desc_end = p + data_unit_size; 
        if (p_end < desc_end)
            return -1;
        p = desc_end; 
        return data_unit_size;
    }
    else if (0x2c == data_unit_parameter)
    {
        data_unit_size = get24(&p, p_end);           /* data_unit_size */
        desc_end = p + data_unit_size; 
        if (p_end < desc_end)
            return -1;
        p = desc_end; 
        return data_unit_size; 
    }
    else if (0x30 == data_unit_parameter)
    {
        data_unit_size = get24(&p, p_end);           /* data_unit_size */
        desc_end = p + data_unit_size; 
        if (p_end < desc_end)
            return -1;
        p = desc_end; 
        return data_unit_size;
    }
    else if (0x31 == data_unit_parameter)
    {
        data_unit_size = get24(&p, p_end);           /* data_unit_size */
        desc_end = p + data_unit_size; 
        if (p_end < desc_end)
            return -1;
        p = desc_end; 
        return data_unit_size;
    }
    else if (0x34 == data_unit_parameter)
    {
        data_unit_size = get24(&p, p_end);           /* data_unit_size */
        desc_end = p + data_unit_size; 
        if (p_end < desc_end)
            return -1;
        p = desc_end; 
        return data_unit_size;
    }
    else if (0x35 == data_unit_parameter)
    {
        data_unit_size = get24(&p, p_end);           /* data_unit_size */
        desc_end = p + data_unit_size; 
        if (p_end < desc_end)
            return -1;
        p = desc_end; 
        return data_unit_size;
    }
    else if (0x20 != data_unit_parameter)
        return -1;
    
    data_unit_size = get24(&p, p_end);           /* data_unit_size */
    desc_end = p + data_unit_size;
    if (p_end < desc_end)
        return -1;

    switch(data_unit_parameter) 
        {
        case 0x20:
            break;
        case 0x28:
        case 0x2c:
        case 0x30:
        case 0x31:
        case 0x34:
        case 0x35:
            WARNING("NO Character data of caption statement!\n");
            break;
        default:
            WARNING("NO Character data of caption statement!\n");
            break;
    }

    len = data_unit_size;
//    if(data_unit_size == 0x53
//            && p[0x3f] == 0x24
//            && p[0x3f+1] == 0x47
//            && p[0x3f+2] == 0x24
//            && p[0x3f+3] == 0x62
//            && p[0x3f+4] == 0x24
//            && p[0x3f+5] == 0x35
//            && p[0x3f+6] == 0x89
//            && p[0x3f+7] == 0x20
//            && p[0x3f+8] == 0x8a
//            && p[0x3f+9] == 0x4b
//            && p[0x3f+10] == 0x4d
//            && p[0x3f+11] == 0x24
//            )
//        {
//            WARNING("find this empty subtitle!\n");
//        }
    while(len > 0)
    {
        last_type = type;
        type = get8(&p, desc_end);
        type &=0xff;
        len--;
        switch(type) 
        {
        //* C0 control set
        case NUL:
        case BEL:
            break;
        case SP:
            *dst++ = SP;
            *dst++ = SP;
            *dstLen+= 2;
            break;
        case DEL:
            //*dst++ = (__u8)type;
            //*dstLen+= 1;
            break;
        case PAPF:                     //暂不解析；
            type = get8(&p, desc_end);
            type &=0xff;
            len--;
            break;
        case APR:    // 默认换行;   
            tmpItemInf->position_flags = 1;  
            if ((last_type == SSZ) || (last_type == MSZ) || (last_type == NSZ))
            {
                if(tmpItemInf->subNum > 0)
                {
                    tmpItemInf->subStyleSize[tmpItemInf->subNum-1] = LastStyleSize;
                }
                else
                {
                    tmpItemInf->subStyleSize[0] = LastStyleSize;
                }
            }
            if(tmpItemInf->subNum >0)
            {
               *groupLen = *dstLen; 
                groupLen++;
            }
            tmpItemInf->subNum++;   
            StyleSize_flags = 0;
            if(tmpItemInf->subNum > 0)
            {
                tmpItemInf->subStyleSize[tmpItemInf->subNum-1] = StyleSize;
            }
            else
            {
                tmpItemInf->subStyleSize[0] = StyleSize;
            }
            if (tmpItemInf->subNum > ISDBDEC_MAX_LINE_NUM)
            {
                tmpItemInf->subNum = ISDBDEC_MAX_LINE_NUM;
                groupLen--;
                WARNING("fatal error: tmpItemInf->subNum > ISDBDEC_MAX_LINE_NUM?\n");
            }
            if (single_shift_flags == 1)
            {
                *dst++ = single_shift_data[0];
                *dstLen+= 1;
                *dst++ = single_shift_data[1];
                *dstLen+= 1;
            }
            if (InvodeCode == MAPLE_SUB_INVOKE_NONE)
            {
                ;
            }
            else if(InvodeCode == MAPLE_SUB_INVOKE_LS0)
            {
                *dst++ = 0x0f;
                *dstLen+= 1;
            }
            else if(InvodeCode == MAPLE_SUB_INVOKE_LS1)
            {
                *dst++ = 0x0e;
                *dstLen+= 1;
            }
            else if(InvodeCode == MAPLE_SUB_INVOKE_LS2)
            {
                *dst++ = 0x1b;
                *dst++ = 0x6e;
                *dstLen+= 2;
            }
            else if(InvodeCode == MAPLE_SUB_INVOKE_LS3)
            {
                *dst++ = 0x1b;
                *dst++ = 0x6f;
                *dstLen+= 2;
            }
            else if(InvodeCode == MAPLE_SUB_INVOKE_LS1R)
            {
                *dst++ = 0x1b;
                *dst++ = 0x7e;
                *dstLen+= 2;
            }
            else if(InvodeCode == MAPLE_SUB_INVOKE_LS2R)
            {
                *dst++ = 0x1b;
                *dst++ = 0x7d;
                *dstLen+= 2;
            }
            else if(InvodeCode == MAPLE_SUB_INVOKE_LS3R)
            {
                *dst++ = 0x1b;
                *dst++ = 0x7c;
                *dstLen+= 2;
            }
            type = get8(&p, desc_end);
            type &=0xff;
            len--;
            //tmpItemInf->line[tmpItemInf->subNum-1] = (__s16)(type - 0x40)*2;
            if (StyleSize == MAPLE_SUB_STYLE_SMALL_SIZE)
            {
                tmpItemInf->line[tmpItemInf->subNum-1] = (__s16)(type - 0x40);
            }
            else
            {
                tmpItemInf->line[tmpItemInf->subNum-1]= (__s16)(type - 0x40)*2;
            }       
            type = get8(&p, desc_end);
            type &=0xff;
            len--;
//            if (last_type == SSZ || (last_type == MSZ))
//            {
//                tmpItemInf->forward[tmpItemInf->subNum-1]= (__s16)(type - 0x40);
//            }
//            else
//            {
//                tmpItemInf->forward[tmpItemInf->subNum-1]= (__s16)(type - 0x40)*2;
//            }
            if (StyleSize == MAPLE_SUB_STYLE_SMALL_SIZE || (StyleSize == MAPLE_SUB_STYLE_MIDDLE_SIZE))
            {
                tmpItemInf->forward[tmpItemInf->subNum-1]= (__s16)(type - 0x40);
            }
            else
            {
                tmpItemInf->forward[tmpItemInf->subNum-1]= (__s16)(type - 0x40)*2;
            }       
            break;    
        case CS:
        case CAN:
            break;
        case ESC:
            *dst++ = (__u8)type;
            *dstLen+= 1;
            type = get8(&p, desc_end);
            type &=0xff;
            len--;
            if(type == 0x6e)
                InvodeCode = MAPLE_SUB_INVOKE_LS2;
            else if(type == 0x6f)
                InvodeCode = MAPLE_SUB_INVOKE_LS3;
            else if(type == 0x7e)
                InvodeCode = MAPLE_SUB_INVOKE_LS1R;
            else if(type == 0x7d)
                InvodeCode = MAPLE_SUB_INVOKE_LS2R;
            else if(type == 0x7c)
                InvodeCode = MAPLE_SUB_INVOKE_LS3R;
            else    //不是invocation,就是designation of graphic sets
            {
                InvodeCode = MAPLE_SUB_INVOKE_NONE;
                *dst++ = (__u8)type;
                *dstLen+= 1;
                if(((type>>4)&0x0f)==0x02)
                {
                    for(i=0;i<3;i++)    //根据spec,designation去掉ESC,最多4字节,已经读了一个字节,再读3字节,正常情况下应该整个字段读完
                    {
                        type = get8(&p, desc_end);
                        if(-1 == (__s32)type)
                        {
                            WARNING("fatal error! read buffer end! i=[%d], p[0x%x]\n", i, p);
                            break;
                        }
                        type &=0xff;
                        len--;
                        *dst++ = (__u8)type;
                        *dstLen+= 1;
                        if(((type>>4)&0x0f)!=0x02)
                        {
                            break;
                        }
                    }
                    if(i >= 3)
                    {
                        WARNING("fatal error! ESC designation exceed 5 bytes?\n");
                    }
                }
                else
                {
                    WARNING("fatal error! ESC designation wrong!p[0x%x]\n",p);
                }
                break;
            }
            
            *dst++ = (__u8)type;
            *dstLen+= 1;
            break;
        case LS1:
            InvodeCode = MAPLE_SUB_INVOKE_LS1;
            *dst++ = (__u8)type;
            *dstLen+= 1;
            break;
        case LS0:
            InvodeCode = MAPLE_SUB_INVOKE_LS0;
            *dst++ = (__u8)type;
            *dstLen+= 1;
            break;
        case SS2:
            single_shift_flags = 1;
            *dst++ = (__u8)type;
            single_shift_data[0] = (__u8)type;
            *dstLen+= 1;
            type = get8(&p, desc_end);
            type &=0xff;
            len--;
            *dst++ = (__u8)type;
            single_shift_data[1] = (__u8)type;
            *dstLen+= 1;           
            break;
        case SS3:
            single_shift_flags = 1;
            *dst++ = (__u8)type;
            single_shift_data[0] = (__u8)type;
            *dstLen+= 1;
            type = get8(&p, desc_end);
            type &=0xff;
            len--;
            *dst++ = (__u8)type;
            single_shift_data[1] = (__u8)type;
            *dstLen+= 1;  
            break;
        case RS:
            //*dst++ = (__u8)type;
            //*dstLen+= 1;
            break;

        case US:
            WARNING(" Be careful: Data unit!\n");
            return -1;
             
        //* C1 control set  
        case BKF:
            if(tmpItemInf->subNum > 0)
            {
                tmpItemInf->subColor[tmpItemInf->subNum-1] = MAPLE_SUB_COLOR_BLACK;
            }
            else
            {
                tmpItemInf->subColor[0] = MAPLE_SUB_COLOR_BLACK;
            }
            break;
        case RDF:
            if(tmpItemInf->subNum > 0)
            {
                tmpItemInf->subColor[tmpItemInf->subNum-1] = MAPLE_SUB_COLOR_RED;
            }
            else
            {
                tmpItemInf->subColor[0] = MAPLE_SUB_COLOR_RED;
            }
            break;
        case GRF:
            if(tmpItemInf->subNum > 0)
            {
                tmpItemInf->subColor[tmpItemInf->subNum-1] = MAPLE_SUB_COLOR_GREEN;
            }
            else
            {
                tmpItemInf->subColor[0] = MAPLE_SUB_COLOR_GREEN;
            }
            break;
        case YLF:
            if(tmpItemInf->subNum > 0)
            {
                tmpItemInf->subColor[tmpItemInf->subNum-1] = MAPLE_SUB_COLOR_YELLOW;
            }
            else
            {
                tmpItemInf->subColor[0] = MAPLE_SUB_COLOR_YELLOW;
            }
            break;
        case BLF:
            if(tmpItemInf->subNum > 0)
            {
                tmpItemInf->subColor[tmpItemInf->subNum-1] = MAPLE_SUB_COLOR_BLUE;
            }
            else
            {
                tmpItemInf->subColor[0] = MAPLE_SUB_COLOR_BLUE;
            }
            break;
        case MGF:
            if(tmpItemInf->subNum > 0)
            {
                tmpItemInf->subColor[tmpItemInf->subNum-1] = MAPLE_SUB_COLOR_MAGENTA;
            }
            else
            {
                tmpItemInf->subColor[0] = MAPLE_SUB_COLOR_MAGENTA;
            }
            break;
        case CNF:
            if(tmpItemInf->subNum > 0)
            {
                tmpItemInf->subColor[tmpItemInf->subNum-1] = MAPLE_SUB_COLOR_CYAN;
            }
            else
            {
                tmpItemInf->subColor[0] = MAPLE_SUB_COLOR_CYAN;
            }
            break;
        case WHF:
            if(tmpItemInf->subNum > 0)
            {
                tmpItemInf->subColor[tmpItemInf->subNum-1] = MAPLE_SUB_COLOR_WHITE;
            }
            else
            {
                tmpItemInf->subColor[0] = MAPLE_SUB_COLOR_WHITE;
            }
            break;
        case COL:
            type = get8(&p, desc_end);
            type &=0xff;
            len--;
            if (type == 0x20)
            {
                type = get8(&p, desc_end);
                type &=0xff;
                len--;
            }
            else if((type > 0x7f) && (type < 0x48))
            {
                WARNING("## fatal error: COL error, check code!\n");
            }
            break;
        case POL:
            get8(&p, desc_end);
            len--;
            break;               
//            case SSZ:
//            case MSZ:
//            case NSZ:
//                break;
        case SZX:
            get8(&p, desc_end);
            len--;
            break;
        case FLC:
            get8(&p, desc_end);
            len--;
            break;
        case CDC:
            type = get8(&p, desc_end);
            type &=0xff;
            len--;
            if (type == 0x20)
            {
                type = get8(&p, desc_end);
                type &=0xff;
                len--;
            }
            else if((type != 0x40) && (type != 0x4f))
            {
                WARNING("## fatal error: CDC error, check code!\n");
            }
            break;                
        case WMM:
            get8(&p, desc_end);
            len--;
            break;
        case TIME:
            type = get8(&p, desc_end);
            type &=0xff;
            len--;
            if(type == 0x20)
            {
                type = get8(&p, desc_end);
                type &=0xff;
                len--;
                if( (type >= 0x40) && (type <= 0x7f))
                    tmpItemInf->uDuration += (type - 0x40)*100;          //subtitle duration times: ms;
            }
            else if(type == 0x28)
            {
                type = get8(&p, desc_end);
                type &=0xff;
                len--;
            }
            else
            {
                WARNING("Be careful!!TIME : type[0x%x] is not processed!\n", type);
            }
            break;
        case MACRO:
            type = get8(&p, desc_end);
            type &=0xff;
            len--;
            WARNING("## MACRO: type = [%x]\n", type);
            break;
        case RPC:
            get8(&p, desc_end);
            len--;
            break;
        case STL:
            break;
        case HLC:
            get8(&p, desc_end);
            len--;
            break;
        case CSI:
            tmpPoint = 0;
            do{
                type = get8(&p, desc_end);
                type &=0xff;
                *(tmpBuf+tmpPoint) = type;
                tmpPoint++;
                len--;
                if(type == 0x42)
                {
                    //WARNING(" CSI: GSM, Character deformation!\n");
                    break;
                }    
                else if ((type >= 0x53) && (type <= 0x6f))
                {
                    //WARNING(" CSI: Extension control code!\n");
                    break;
                } 
                else if(tmpPoint > 15)
                {
                    type = 0x00;
                    break;
                }                
            }while(1);
            //* Extension control code (CSI)
            if(type == 0x53)  //* SWF
            { 
                if (SWF_flags != 0)
                {
                    break;
                }
                else
                {
                    SWF_flags = 1;
                }  
                i = 0;
                tmp = 0;
                while((tmpBuf[i] != 0x20) && (i < tmpPoint))
                {
                    tmp*=10; 
                    tmp = tmp + tmpBuf[i] - 0x30;
                    i++;
                }    
                if ( tmp == 0)
                {
                    ;   
                }
                else if(tmp == 1)
                {
                   ;  
                }
                else if(tmp == 2)
                {
                   ;
                }
                else if(tmp == 3)
                {
                    ;  
                }
                else if(tmp == 4)
                {
                    ;      
                }
                else if(tmp == 5)
                {
                    tmpItemInf->videoWidth  = 1920;
                    tmpItemInf->videoHeight = 1080;      
                }
                else if(tmp == 6)
                {
                    tmpItemInf->videoWidth  = 1920;
                    tmpItemInf->videoHeight = 1080;      
                }
                else if(tmp == 7)
                {
                    tmpItemInf->videoWidth  = 960;
                    tmpItemInf->videoHeight = 540;      
                }
                else if(tmp == 8)
                {
                    tmpItemInf->videoWidth  = 960;
                    tmpItemInf->videoHeight = 540;      
                }
                else if(tmp == 9)
                {
                    tmpItemInf->videoWidth  = 720;
                    tmpItemInf->videoHeight = 480;      
                }
                else if(tmp == 10)
                {
                    tmpItemInf->videoWidth  = 720;
                    tmpItemInf->videoHeight = 480;      
                }
                else if(tmp == 11)
                {
                    tmpItemInf->videoWidth  = 1280;
                    tmpItemInf->videoHeight = 720;      
                }
                else if(tmp == 12)
                {
                    tmpItemInf->videoWidth  = 1280;
                    tmpItemInf->videoHeight = 720;      
                }
                break;
            }
            else if (type == 0x56)   //* SDF
            {
                if(SDF_flags != 0)
                {
                    break;
                }
                else
                {
                    SDF_flags = 1;
                }
                
                tmpItemInf->subWidth = 0;
                tmpItemInf->subHeight = 0;
                i = 0;
                do{
                    if(tmpBuf[i] != 0x3b)
                    {
                        tmpItemInf->subWidth *=10;
                        tmpItemInf->subWidth = tmpItemInf->subWidth+tmpBuf[i] - 0x30;
                        i++;
                    }
                    else
                    {
                        i++;
                        break;
                    }
                    
                }while(i < tmpPoint);

                while((tmpBuf[i] != 0x20) && (i < tmpPoint))
                {
                    tmpItemInf->subHeight*=10; 
                    tmpItemInf->subHeight= tmpItemInf->subHeight+tmpBuf[i] - 0x30;
                    i++;
                }
                break;
            }
            else if(type == 0x5f)  //* SDP
            {
                if (SDP_flags != 0)
                {
                    break;
                }
                else
                {
                    SDP_flags = 1;
                }  
                tmpItemInf->position_flags = 1;
                tmpItemInf->startX = 0;
                tmpItemInf->startY = 0;

                i = 0;
                do{
                    if(tmpBuf[i] != 0x3b)
                    {
                        tmpItemInf->startX *=10; 
                        tmpItemInf->startX = tmpItemInf->startX+tmpBuf[i] - 0x30;
                        i++;
                    }
                    else
                    {
                        i++;
                        break;
                    }
                    
                }while(i < tmpPoint);
                

                while((tmpBuf[i] != 0x20) && (i < tmpPoint))
                {
                    tmpItemInf->startY*=10; 
                    tmpItemInf->startY= tmpItemInf->startY+tmpBuf[i] - 0x30;
                    i++;
                }
                break;
            }
            else if(type == 0x61)  //* ACPS
            {
                tmpItemInf->position_flags = 1;  
                if(tmpItemInf->subNum >0) 
                {
                   *groupLen = *dstLen; 
                    groupLen++;
                }
                tmpItemInf->subNum++;   
                if (tmpItemInf->subNum > ISDBDEC_MAX_LINE_NUM)
                {
                    tmpItemInf->subNum = ISDBDEC_MAX_LINE_NUM;
                    groupLen--;
                    WARNING("fatal error: tmpItemInf->subNum > ISDBDEC_MAX_LINE_NUM?\n");
                } 
                tmpItemInf->subStyleSize[tmpItemInf->subNum-1] = StyleSize;               
                AcpsDataX = 0;
                AcpsDataY = 0;  
                if (single_shift_flags == 1)
                {
                    *dst++ = single_shift_data[0];
                    *dstLen+= 1;
                    *dst++ = single_shift_data[1];
                    *dstLen+= 1;
                }
                if (InvodeCode == MAPLE_SUB_INVOKE_NONE)
                {
                    ;
                }
                else if(InvodeCode == MAPLE_SUB_INVOKE_LS0)
                {
                    *dst++ = 0x0f;
                    *dstLen+= 1;
                }
                else if(InvodeCode == MAPLE_SUB_INVOKE_LS1)
                {
                    *dst++ = 0x0e;
                    *dstLen+= 1;
                }
                else if(InvodeCode == MAPLE_SUB_INVOKE_LS2)
                {
                    *dst++ = 0x1b;
                    *dst++ = 0x6e;
                    *dstLen+= 2;
                }
                else if(InvodeCode == MAPLE_SUB_INVOKE_LS3)
                {
                    *dst++ = 0x1b;
                    *dst++ = 0x6f;
                    *dstLen+= 2;
                }
                else if(InvodeCode == MAPLE_SUB_INVOKE_LS1R)
                {
                    *dst++ = 0x1b;
                    *dst++ = 0x7e;
                    *dstLen+= 2;
                }
                else if(InvodeCode == MAPLE_SUB_INVOKE_LS2R)
                {
                    *dst++ = 0x1b;
                    *dst++ = 0x7d;
                    *dstLen+= 2;
                }
                else if(InvodeCode == MAPLE_SUB_INVOKE_LS3R)
                {
                    *dst++ = 0x1b;
                    *dst++ = 0x7c;
                    *dstLen+= 2;
                }
                i = 0;
                do{
                    if(tmpBuf[i] != 0x3b)
                    {
                        AcpsDataX *=10; 
                        AcpsDataX = AcpsDataX+tmpBuf[i] - 0x30;
                        i++;
                    }
                    else
                    {
                        i++;
                        break;
                    }
                    
                }while(i < tmpPoint);
                
                while((tmpBuf[i] != 0x20) && (i < tmpPoint))
                {
                    AcpsDataY *=10; 
                    AcpsDataY = AcpsDataY + tmpBuf[i] - 0x30;
                    i++;
                }
                if ((ssmData>0) && (svsData>0))
                   tmpItemInf->line[tmpItemInf->subNum-1]=(AcpsDataY - tmpItemInf->startY)*2/(ssmData + svsData);
                else
                    tmpItemInf->line[tmpItemInf->subNum-1]=(AcpsDataY - tmpItemInf->startY)/30;
                
                if ((ssmData>0) && (shsData>0))
                   tmpItemInf->forward[tmpItemInf->subNum-1]=(AcpsDataX- tmpItemInf->startX)*2/(ssmData + shsData);
                else
                    tmpItemInf->forward[tmpItemInf->subNum-1]=(AcpsDataX - tmpItemInf->startX)/20;
                break;
            }
            else if(type == 0x58)  //* SHS
            { 
                if (SHS_flags != 0)
                {
                    break;
                }
                else
                {
                    SHS_flags = 1;
                }  
                i = 0;
                do{
                    if(tmpBuf[i] != 0x20)
                    {
                        shsData *=10; 
                        shsData = shsData+tmpBuf[i] - 0x30;
                        i++;
                    }
                    else
                    {
                        i++;
                        break;
                    }
                    
                }while(i < tmpPoint);
                break;
            }
            else if(type == 0x59)  //* SVS
            { 
                if (SVS_flags != 0)
                {
                    break;
                }
                else
                {
                    SVS_flags = 1;
                }  
                i = 0;
                do{
                    if(tmpBuf[i] != 0x20)
                    {
                        svsData *=10; 
                        svsData  = svsData + tmpBuf[i] - 0x30;
                        i++;
                    }
                    else
                    {
                        i++;
                        break;
                    }
                    
                }while(i < tmpPoint);
                break;
            }
            else if(type == 0x57)  //* SSM
            {
                if (SSM_flags != 0)
                {
                    break;
                }
                else
                {
                    SSM_flags = 1;
                }  
               i = 0;
               do{
                   if(tmpBuf[i] != 0x3b)
                   {
                       ssmData *=10; 
                       ssmData  = ssmData+tmpBuf[i] - 0x30;
                       i++;
                   }
                   else
                   {
                       i++;
                       break;
                   }
                   
               }while(i < tmpPoint);
               while((tmpBuf[i] != 0x20) && (i < tmpPoint))
               {
                   i++;
               }
               if(ssmData > 0)
                   tmpItemInf->subSize = ssmData;
               break;
            }
            break; 
        case SSZ:
            if (StyleSize_flags == 0)
            {
                if(tmpItemInf->subNum > 0)
                {
                    tmpItemInf->subStyleSize[tmpItemInf->subNum-1] =  MAPLE_SUB_STYLE_SMALL_SIZE;
                }
                else
                {
                    tmpItemInf->subStyleSize[0] =  MAPLE_SUB_STYLE_SMALL_SIZE;
                }
                
                StyleSize_flags = 1;
            } 
            LastStyleSize = StyleSize;
            StyleSize = MAPLE_SUB_STYLE_SMALL_SIZE;
            //*dst++ = (__u8)type;
            //*dstLen+= 1;
            break;
        case MSZ:
            if (StyleSize_flags == 0)
            {
                if(tmpItemInf->subNum > 0)
                {
                    tmpItemInf->subStyleSize[tmpItemInf->subNum-1] =  MAPLE_SUB_STYLE_MIDDLE_SIZE;
                }
                else
                {
                    tmpItemInf->subStyleSize[0] =  MAPLE_SUB_STYLE_MIDDLE_SIZE;
                }
                
                StyleSize_flags = 1;
            }  
            LastStyleSize = StyleSize;
            StyleSize = MAPLE_SUB_STYLE_MIDDLE_SIZE;
            //*dst++ = (__u8)type;
            //*dstLen+= 1;
            break;
        case NSZ:
            if (StyleSize_flags == 0)
            {
                if(tmpItemInf->subNum > 0)
                {
                    tmpItemInf->subStyleSize[tmpItemInf->subNum-1] =  MAPLE_SUB_STYLE_NORMAL_SIZE;
                }
                else
                {
                    tmpItemInf->subStyleSize[0] = MAPLE_SUB_STYLE_NORMAL_SIZE;
                }
                
                StyleSize_flags = 1;
            } 
            LastStyleSize = StyleSize;
            StyleSize = MAPLE_SUB_STYLE_NORMAL_SIZE;
            //*dst++ = (__u8)type;
            //*dstLen+= 1;
            break;
        case APB:
        case APF:
        case APD:
        case APU:
           //*dst++ = (__u8)type;
           //*dstLen+= 1;
            break;
        case APS:
            if (last_type == SSZ)
            {
                break;
            }
            else if (*dstLen > 0)
            {
//                if (tmpItemInf->position_flags == 0)
//                {
//                    tmpItemInf->position_flags = 1; 
//                }
                if (tmpItemInf->subNum == 0)
                {
                    tmpItemInf->subNum = 1;
                }
                if(tmpItemInf->subNum >0)
                {
                   *groupLen = *dstLen; 
                    groupLen++;
                }
                tmpItemInf->subNum++;   
                StyleSize_flags = 0;
                if(tmpItemInf->subNum > 0)
                {
                    tmpItemInf->subStyleSize[tmpItemInf->subNum-1] = StyleSize;
                }
                else
                {
                    tmpItemInf->subStyleSize[0] = StyleSize;
                }
                if (tmpItemInf->subNum > ISDBDEC_MAX_LINE_NUM)
                {
                    tmpItemInf->subNum = ISDBDEC_MAX_LINE_NUM;
                    groupLen--;
                    WARNING("fatal error: tmpItemInf->subNum > ISDBDEC_MAX_LINE_NUM?\n");
                }
                if (single_shift_flags == 1)
                {
                    *dst++ = single_shift_data[0];
                    *dstLen+= 1;
                    *dst++ = single_shift_data[1];
                    *dstLen+= 1;
                }
                if (InvodeCode == MAPLE_SUB_INVOKE_NONE)
                {
                    ;
                }
                else if(InvodeCode == MAPLE_SUB_INVOKE_LS0)
                {
                    *dst++ = 0x0f;
                    *dstLen+= 1;
                }
                else if(InvodeCode == MAPLE_SUB_INVOKE_LS1)
                {
                    *dst++ = 0x0e;
                    *dstLen+= 1;
                }
                else if(InvodeCode == MAPLE_SUB_INVOKE_LS2)
                {
                    *dst++ = 0x1b;
                    *dst++ = 0x6e;
                    *dstLen+= 2;
                }
                else if(InvodeCode == MAPLE_SUB_INVOKE_LS3)
                {
                    *dst++ = 0x1b;
                    *dst++ = 0x6f;
                    *dstLen+= 2;
                }
                else if(InvodeCode == MAPLE_SUB_INVOKE_LS1R)
                {
                    *dst++ = 0x1b;
                    *dst++ = 0x7e;
                    *dstLen+= 2;
                }
                else if(InvodeCode == MAPLE_SUB_INVOKE_LS2R)
                {
                    *dst++ = 0x1b;
                    *dst++ = 0x7d;
                    *dstLen+= 2;
                }
                else if(InvodeCode == MAPLE_SUB_INVOKE_LS3R)
                {
                    *dst++ = 0x1b;
                    *dst++ = 0x7c;
                    *dstLen+= 2;
                }
                if (tmpItemInf->subNum >= 2)
                    tmpItemInf->line[tmpItemInf->subNum-1] = tmpItemInf->line[tmpItemInf->subNum-2]+1;
                break;
            }
            if(tmpItemInf->subNum > 0)
            {
                tmpItemInf->forward[tmpItemInf->subNum-1] = 0;
            }
            else
            {
                tmpItemInf->forward[0] = 0;
            }
            //*dst++ = (__u8)type;
            //*dstLen+= 1;
            break;
        default:
            *dst++ = (__u8)type;
            *dstLen+= 1;
            break;
        }
    }    
    p = desc_end; 
    return data_unit_size;
}

/*******************************************************************************
Function name: checkEmptySubtitle
Description: 
    1.规定这时送入的字符串都必须是UTF8编码!
    2. 检查该字幕是否有内容，是否是空字幕.
Parameters: 
    
Return: 
    EPDK_TRUE: Full subtitle;
    EPDK_FALSE: Empty subtitle.
Time: 2012/7/27
*******************************************************************************/
__s32 checkFullSubtitle(CCedarSubtitleFrame *tmpItemInf)
{
    __s32   nLen;
    __u8    *pString;
    if(tmpItemInf->subNum > 1)
    {
        return EPDK_TRUE;
    }
    else if(tmpItemInf->subNum <= 0)
    {
        return EPDK_FALSE;
    }

    //再查内容，如果只有空格符,(unicode:0x20或者是0x3000),则虽然有内容,也认为是空字幕.
    nLen = tmpItemInf->subDataLen[0];
    if(nLen <= 0)
    {
        return EPDK_FALSE;
    }
    pString = tmpItemInf->subData[0];
    WARNING("**checkFullSubtitle %x,%x,%x,%x,%x,%x,%x,%x,%x,%x **\n",pString[0],pString[1],pString[2],pString[3],pString[4],pString[5],pString[6],pString[7],pString[8],pString[9]);
    if(1 == nLen)
    {
        if(0x20 == pString[0])
        {
            WARNING("empty subtitle frame has only 0x20!\n");
            return EPDK_FALSE;
        }
        else
        {
            return EPDK_TRUE;
        }
    }
    else if(3 == nLen)
    {
        if(0xe3 == pString[0] && 0x80 == pString[1] && 0x80 == pString[2])
        {
            WARNING("empty subtitle frame has 0xe3, 0x80, 0x80!\n");
            return EPDK_FALSE;
        }
        else
        {
            return EPDK_TRUE;
        }
    }
    else
    {
        return EPDK_TRUE;
    }
}

/*******************************************************************************
Function name: ldec_isdb_decode
Description: 
    
Parameters: 
    
Return: 
    LDEC_ERR_IGNORE: 字幕项不是caption statement.解析完成.
    LDEC_ERR_DEC_SUBTITLE:解析出错.
    LDEC_ERR_NONE: 解析完成,解析出一个有内容的字幕项.
    LDEC_LBS_EMPTY: 解析完成.解析出一个空字幕项.
Time: 2012/7/26
*******************************************************************************/
__s32 ldec_isdb_decode(IsdbDec *thiz, CCedarSubtitleFrame *tmpItemInf, __u8 *curDataPtr, __u32 *usedBytes, __u8 *privateData)
{
    __s32   i = 0, j = 0;
    //__u32   dstlen = 0;
    __u8    *pstart = NULL;
    //__u8    *dststring = NULL;
    __u8    dataBuf[ISDBDEC_LBSBUF_SIZE];
    __u8    dstData[3*ISDBDEC_LBSBUF_SIZE+100];
    //__s32   dataBufLen = 0;
    __s32   dst_length = 0;
    __s32   src_length = 0;
    __u32   ret = 0; 
    __s32   subNum =0;
    __u8    type;
    __u8    srcData[ISDBDEC_LBSBUF_SIZE];   //一个字幕项去除颜色等特殊字符后的ARIBJIS字符串.多行在一起.
    __s32   srcLen = 0; //srcData的长度.不超过188字节.不包括\0.
    __s32   groupLen[ISDBDEC_MAX_LINE_NUM]; //如果一个字幕项分多行,group[0]:第一行的长度;group[1]:前两行的长度;group[2]:前3行的长度.在解析时,最后一行的数值不存.即如果有3行字符,那么groupLen[]只存前两行的值.
    int     mode = thiz->nOneSegFlag;
		__u16   data_length;
		//__u32   tmpCharEncodeType;
    WARNING("## thiz->nOneSegFlag = [%d]\n", thiz->nOneSegFlag);

    memset((__u8*)tmpItemInf->subColor, 0, ISDBDEC_MAX_LINE_NUM*sizeof(__u32)); 
    memset((__u8*)tmpItemInf->subStyleSize,  0, ISDBDEC_MAX_LINE_NUM*sizeof(__u32));  
    memset((__u8*)tmpItemInf->line, 0, ISDBDEC_MAX_LINE_NUM*sizeof(__u16)); 
    memset((__u8*)tmpItemInf->forward,  0, ISDBDEC_MAX_LINE_NUM*sizeof(__u16)); 
    memset((__u8*)groupLen, 0, ISDBDEC_MAX_LINE_NUM*sizeof(__s32));	
    memset((__u8*)dataBuf, 0, ISDBDEC_LBSBUF_SIZE);	
    memset((__u8*)srcData, 0, ISDBDEC_LBSBUF_SIZE);	
    memset((__u8*)dstData, 0, (3*ISDBDEC_LBSBUF_SIZE+100));	
   
   *usedBytes = tmpItemInf->uSubSize;
    pstart = curDataPtr;
		data_length = ((pstart[6]<<8)|pstart[7]+10);
    //WARNING("**ldec_isdb_decode pstart %x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x **\n",pstart[0],pstart[1],pstart[2],pstart[3],pstart[4],pstart[5],pstart[6],pstart[7],pstart[8],pstart[9],pstart[10],pstart[11],pstart[12],pstart[13],pstart[14],pstart[15],pstart[16],pstart[17],pstart[18],pstart[19]);
		//WARNING("tmpItemInf->uSubSize = %d\n",tmpItemInf->uSubSize);
		//遇到PES包由两个TS包组成时会有两段数据从DEMUX传递过来，这时要重组为一个数据段
		if((pstart[0]==0x80)&&(pstart[1]==0xff)&&(pstart[2]==0xf0))
		{
				data_length = ((pstart[6]<<8)|pstart[7])+10;
				//WARNING("2 data_length = %d\n",data_length);
				if(*usedBytes < data_length)
					{
						memcpy(tmpItemInf->pSubtitle, curDataPtr, tmpItemInf->uSubSize);
						tmpItemInf->tmpSubSize = tmpItemInf->uSubSize;
						//WARNING("tmpItemInf->tmpSubSize =%d\n",tmpItemInf->tmpSubSize);
						return LDEC_ERR_LBS_UNDERFLOW;
					}
				else
					memcpy(tmpItemInf->pSubtitle, curDataPtr, tmpItemInf->uSubSize);
		}	
		else
		{
    			const __u8 *p;
				memcpy(tmpItemInf->pSubtitle+tmpItemInf->tmpSubSize, curDataPtr, tmpItemInf->uSubSize);
				*usedBytes =tmpItemInf->tmpSubSize + tmpItemInf->uSubSize;
				//WARNING("*usedBytes =%d,tmpSubSize=%d,uSubSize=%d\n",*usedBytes,tmpItemInf->tmpSubSize,tmpItemInf->uSubSize);
				p = tmpItemInf->pSubtitle+tmpItemInf->tmpSubSize;
    		//WARNING("**ldec_isdb_decode2 p %x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x **\n",p[0],p[1],p[2],p[3],p[4],p[5],p[6],p[7],p[8],p[9],p[10],p[11],p[12],p[13],p[14],p[15],p[16],p[17],p[18],p[19]);
		}
		//memcpy(tmpItemInf->pSubtitle, curDataPtr, tmpItemInf->uSubSize);
    pstart = tmpItemInf->pSubtitle;	//test130703 
    for (i =0; i<4; i++)
    {
       type = *privateData++;     //CCIS
    }
    type = *privateData; 
    //WARNING("ldec_isdb_decode  CCIS type=%d\n",type);
    if(1 == thiz->nOneSegFlag)
    {
        tmpItemInf->displayMode = MAPLE_SUB_DISPLAY_MODE_MOBILE;
    }
    else if(0x01 == type)
    {
        tmpItemInf->displayMode = MAPLE_SUB_DISPLAY_MODE_HD_SIDE;
    }
    else if(0x02 == type)
    {
         tmpItemInf->displayMode = MAPLE_SUB_DISPLAY_MODE_SD_SIDE;
    }
    else if(0x03 == type)
    {
        tmpItemInf->displayMode = MAPLE_SUB_DISPLAY_MODE_SD_WIDE_SIDE;
    }
    else if(0x04 == type)
    {
        tmpItemInf->displayMode = MAPLE_SUB_DISPLAY_MODE_MOBILE;
    }
    else
    {
        WARNING("Be careful: CCIS subtitle? \n");
        tmpItemInf->displayMode = MAPLE_SUB_DISPLAY_MODE_MOBILE;//MAPLE_SUB_DISPLAY_MODE_UNKNOW;
        //return LDEC_LBS_EMPTY;
    }
    
    ret = isdb_read(tmpItemInf, pstart, *usedBytes, srcData, &srcLen, groupLen);
		if(ret!=0)
    {
        tmpItemInf->subNum = 0;
        tmpItemInf->isFull = 0;
        if(1 == ret)
        {
            return LDEC_ERR_IGNORE;
        }
        else
        {
        		WARNING("check decode subtitle error!\n");
            return LDEC_ERR_DEC_SUBTITLE;
        }
    }
    if(tmpItemInf->subNum > ISDBDEC_MAX_LINE_NUM)
    {
        tmpItemInf->subNum = ISDBDEC_MAX_LINE_NUM;
    }
    else if(tmpItemInf->subNum <0)
    {
        WARNING("fatal error: tmpItemInf->subNum < 0!\n");
    }
    else if(tmpItemInf->subNum == 0)
    {
        if (srcLen > 0)
        {
            tmpItemInf->subNum = 1;
        }
    }
		//WARNING("thiz->nCharEncodeType = %d,tmpItemInf->nCharEncodeType = %d\n",thiz->nCharEncodeType,tmpItemInf->nCharEncodeType);

		if ((tmpItemInf->nCharEncodeType != 0)&&(thiz->nCharEncodeType != tmpItemInf->nCharEncodeType))//cky test  add 
		{
				thiz->tmpCharEncodeType = tmpItemInf->nCharEncodeType;
		}
		else
		{
				thiz->tmpCharEncodeType = thiz->nCharEncodeType;
		}
		//WARNING("thiz->tmpCharEncodeType = %d\n",thiz->tmpCharEncodeType);
    if(tmpItemInf->subColor[0] == 0)         //color default value;
        tmpItemInf->subColor[0] = MAPLE_SUB_COLOR_WHITE;
    
    if(tmpItemInf->subStyleSize[0] == 0)     //subtitle default style size;
        tmpItemInf->subStyleSize[0] = MAPLE_SUB_STYLE_NORMAL_SIZE; 
    for(i=1; i<tmpItemInf->subNum; i++) 
    {
        if(tmpItemInf->subColor[i] == 0)
            tmpItemInf->subColor[i] = tmpItemInf->subColor[i-1];
        
        if(tmpItemInf->subStyleSize[i] == 0)
            tmpItemInf->subStyleSize[i] = tmpItemInf->subStyleSize[i-1];
    }
    //* 计算字幕条数:tmpItemInf->subNum;
    subNum = tmpItemInf->subNum;
    tmpItemInf->subNum = 0;
    if ((EPDK_OK == ret) && (srcLen > 0))
    {
        i = 0;
        src_length = 0;
        while((srcLen>src_length) && (i < subNum))
        {
            if (groupLen[i]>0)
            {
                if(src_length < groupLen[i])
                {
                     memset((__u8*)dataBuf, 0, ISDBDEC_LBSBUF_SIZE);
                     //memcpy(dataBuf+dataBufLen, srcData+src_length, (groupLen[i]-src_length));
                     memcpy(dataBuf, srcData+src_length, (groupLen[i]-src_length));
                     //dst_length = isdb_convert_str_to_utf8(dataBuf, (groupLen[i]-src_length+dataBufLen), dstData, 3*ISDBDEC_LBSBUF_SIZE+100, NULL);
                     //dst_length = conv_arib_to_utf8((char*)dataBuf, (groupLen[i]-src_length), (char*)dstData, 3*ISDBDEC_LBSBUF_SIZE+100, mode);
                    // WARNING("thiz->tmpCharEncodeType = %d\n",thiz->tmpCharEncodeType);
                     //dst_length = isdbsub_convert_str_to_utf8(dataBuf, (groupLen[i]-src_length), dstData, 3*ISDBDEC_LBSBUF_SIZE+100, (GraphicSetsDesignationMode)mode, thiz->nCharEncodeType);
										 dst_length = isdbsub_convert_str_to_utf8(dataBuf, (groupLen[i]-src_length), dstData, 3*ISDBDEC_LBSBUF_SIZE+100, (GraphicSetsDesignationMode)mode, thiz->tmpCharEncodeType);

										 if(dst_length>0)
                     {
                         memcpy(tmpItemInf->subData[tmpItemInf->subNum], dstData, dst_length);
                         tmpItemInf->subDataLen[tmpItemInf->subNum]= dst_length;
                         *(tmpItemInf->subData[tmpItemInf->subNum]+dst_length) = 0;
                         tmpItemInf->subNum++;
                     }                      
                    src_length = groupLen[i];
                    i++;
                }
                else
                {
                    for(j = i; j < subNum; j++)
                    {
                        tmpItemInf->subColor[j]     = tmpItemInf->subColor[j+1];
                        tmpItemInf->subStyleSize[j] = tmpItemInf->subStyleSize[j+1];
                        tmpItemInf->line[j]         = tmpItemInf->line[j+1];
                        tmpItemInf->forward[j]      = tmpItemInf->forward[j+1];
                    } 
                    src_length = groupLen[i];
                    i++;
                    continue;
                }
                   
            }
            else if (i >= subNum-1 )
            {
            		
                memset((__u8*)dataBuf, 0, ISDBDEC_LBSBUF_SIZE);					
                memcpy(dataBuf, srcData+src_length, (srcLen-src_length));
                //dst_length = isdb_convert_str_to_utf8(dataBuf, (srcLen-src_length+dataBufLen), dstData, 3*ISDBDEC_LBSBUF_SIZE+100, NULL);
                //dst_length = conv_arib_to_utf8((char*)dataBuf, (srcLen-src_length), (char*)dstData, 3*ISDBDEC_LBSBUF_SIZE+100, mode);
               // WARNING("thiz->tmpCharEncodeType = %d\n",thiz->tmpCharEncodeType);
								// dst_length = isdbsub_convert_str_to_utf8(dataBuf, (srcLen-src_length), dstData, 3*ISDBDEC_LBSBUF_SIZE+100, (GraphicSetsDesignationMode)mode, thiz->nCharEncodeType);

                dst_length = isdbsub_convert_str_to_utf8(dataBuf, (srcLen-src_length), dstData, 3*ISDBDEC_LBSBUF_SIZE+100, (GraphicSetsDesignationMode)mode, thiz->tmpCharEncodeType);
                src_length = srcLen;
                i++;
                if(dst_length>0)
                {
                     memcpy(tmpItemInf->subData[tmpItemInf->subNum], dstData, dst_length);
                     tmpItemInf->subDataLen[tmpItemInf->subNum]= dst_length;
                     *(tmpItemInf->subData[tmpItemInf->subNum]+dst_length) = 0;
                     tmpItemInf->subNum++;
                }
            }
            else
            {
                for(j = i; j < subNum; j++)
                {
                    tmpItemInf->subColor[j]     = tmpItemInf->subColor[j+1];
                    tmpItemInf->subStyleSize[j] = tmpItemInf->subStyleSize[j+1];
                    tmpItemInf->line[j]         = tmpItemInf->line[j+1];
                    tmpItemInf->forward[j]      = tmpItemInf->forward[j+1];
                } 
                i++;
                continue;
            }
        }
           
    }
    else
    {
        tmpItemInf->subNum = 0;
    }
    for(i=0; i<tmpItemInf->subNum; i++)
    {
        WARNING("## tmpItemInf->subDataLen[%d] = [%d], tmpItemInf->subColor[%d] = [%d], tmpItemInf->subStyleSize[%d] = [%d]!\n", 
            i, tmpItemInf->subDataLen[i], i,tmpItemInf->subColor[i], i, tmpItemInf->subStyleSize[i]);
    }    
    tmpItemInf->uDataType = CEDAR_SUB_ENCODE_UTF8;

    //if(tmpItemInf->subNum > 0)
    if(EPDK_TRUE == checkFullSubtitle(tmpItemInf))
    {
        tmpItemInf->isFull = 1;
        return LDEC_ERR_NONE;
    }
    else
    {
        tmpItemInf->isFull = 0;
        WARNING("ISDB decode: meet empty subtitle frame!\n");
        return LDEC_LBS_EMPTY;
    }
}

#endif
