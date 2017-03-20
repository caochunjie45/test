/*
**************************************************************************************************************
*											        ePDK
*						            the Easy Portable/Player Develop Kits
*									           lark sample
*
*						        	(c) Copyright 2006-2007, TERRY, China
*											 All Rights Reserved
*
* File    : format_output.c
* By      : terry
* Version : V1.00
* time    : 2009-12-01
**************************************************************************************************************
*/

//#include "dsk_radio_receive_i.h"
#include "..\\dsk_radio_i.h"
#include <stdarg.h>

/**
 * strnlen - Find the length of a length-limited string
 * @s: The string to be sized
 * @count: The maximum number of bytes to search
 */
/*__u32 strnlen(const char * s, __u32 count)
{
	const char *sc;

	for (sc = s; count-- && *sc != '\0'; ++sc)
		// nothing ;
	return sc - s;
}*/
/*
**********************************************************************************************************************
*                                               my_isdigit
*
* Description: �жϸ��ַ��Ƿ�Ϊ����
*
* Arguments  : s�����жϵ��ַ�
*
* Returns    : ��ȷ����1�����󷵻�0
*
* Notes      : 
*
**********************************************************************************************************************
*/
__s32 my_isdigit(char s)
{
	if(s>'0' && s<'9')
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
/*
**********************************************************************************************************************
*                                               myitoa
*
* Description: ������ת��Ϊ�ַ���
*
* Arguments  : num����ת������ֵ��buf_len��buf���ȣ�bufת��������buf
*
* Returns    : ��ȷ����0�����󷵻�-1
*
* Notes      : 
*
**********************************************************************************************************************
*/
__s32 myitoa(__s32 num,__s32 buf_len,char *buf)
{
    while(buf_len > 0)
    {
        buf[buf_len-1]= (__u8)(num%10 + '0');
        buf_len--;
        num = num/10;
    }
    return 0;
}
/*
**********************************************************************************************************************
*                                               skip_atoi
*
* Description: ���ַ���s��������ַ�ת��Ϊ���֣���ʹ�ַ���ָ�����������ַ�����
*
* Arguments  : s���ַ���ָ��
*
* Returns    : ��ȷ����ת�������ֵ�����󷵻�-1
*
* Notes      : 
*
**********************************************************************************************************************
*/
static __s32 skip_atoi(const char **s)
{
	__s32 i=0;

	while (my_isdigit(**s))
		i = i*10 + *((*s)++) - '0';
	return i;
}

/*
**********************************************************************************************************************
*                                               number
*
* Description: ��ʽ����ֵnumΪ�ַ���
*
* Arguments  : str���ַ���ָ�룻num����ת������ֵ��precision����ת������ֵ����
*
* Returns    : ��ȷ����ת������ַ���ָ�룬���󷵻�-1
*
* Notes      : 
*
**********************************************************************************************************************
*/
static char * number(char * str, __u32 num,__s32 precision )
{
	__u32 num_len=1;
	__u32 temp;
	char s[10];
	__s32 i;
	
	
	temp = num;
	
	while(temp/10 > 0)
	{
		num_len++;
		temp = temp/10;		
	}
	if(precision > num_len)
	{
		myitoa(num,num_len,s);
		for(i = num_len;i<precision;i++)
		{
			*str++ = '0';
		}
		for(i=0;i<num_len;i++)
		{
			*str++ = s[i];
		}
		
	}
	else
	{
		myitoa(num,precision,s);
		for(i=0;i<precision;i++)
		{
			*str++ = s[i];
		}
	}
	return str;
}

/*
**********************************************************************************************************************
*                                               my_output_format
*
* Description: ��ʽ�������buf���ú���������sprintf
*
* Arguments  : buf�����buf��fmt����ʽ��
*
* Returns    : ��ȷ����ʵ��ת���ĸ��������󷵻�-1
*
* Notes      : 
*
**********************************************************************************************************************
*/

__s32 my_output_format(char *buf, const char *fmt,... )
{
	__s32 len;
	__u32 num;
	__s32 i;
	char * str;
	const char *s;
	va_list args;
	__s32 precision;		/* min. # of digits for integers; max
				   number of chars for from string */
	va_start(args,fmt);
	for (str=buf ; *fmt ; ++fmt) {
		if (*fmt != '%') {
			*str++ = *fmt;
			continue;
		}
		++fmt;
		/* get the precision */
		precision = -1;
		if (*fmt == '.') {
			++fmt;
			if (my_isdigit(*fmt))
				precision = skip_atoi(&fmt);
			if (precision < 0)
				precision = 0;
		}


		switch (*fmt) {

		case 's':
			s = va_arg(args, char *);
			if (!s)
				s = "<NULL>";

			len = strnlen(s, precision);
			for (i = 0; i < len; ++i)
				*str++ = *s++;
			while (len < precision--)
				*str++ = ' ';
			continue;

		case 'd':
			num = va_arg(args, unsigned int);
			str = number(str, num,precision);
			continue;

		default:
			continue;
		}
		
	}
	*str = '\0';
	va_end(args);
	return str-buf;
}

