#include "string.h"



int String_Row(char *str[STRING_MAX])
{
	
	return sizeof(str)/sizeof(str[0]);
}


__s32 __Atoi(char *s)
{
	__s32 num = 0;

	while (NULL != eLIBs_strchr("0123456789", *s) && *s)
	{
		num *= 10;
		num += *s - '0';

		s++;
	}

	return num;
}

__s32 Atoi(char *s)
{
	char *o;

	o = s;
	while (*s)
	{
		if (NULL == eLIBs_strchr("+-0123456789", *s))	
		{
			eLIBs_printf("not a number !\n");	
			return -1;
		}
		s++;
	}

	s = o;
	if (*s == '-')
		return -__Atoi(s + 1);
	else if (*s == '+')
		return __Atoi(s + 1);
	else
		return __Atoi(s);
}

/*
 *__s32 my_atoi(char *s)
 *{
 *    __s32 num = 0;
 *    while (*s)
 *    {
 *        num *= 10;	
 *        num += *s - '0';
 *        s++;
 *    }
 *}
 */

void Reverse(char *s)
{
	__s32 i, len = eLIBs_strlen(s);

	for (i = 0; i < len / 2; i++)
	{
		*(s + i) ^= *(s + len - 1 - i);	
		*(s + len - 1 - i) ^= *(s + i);
		*(s + i) ^= *(s + len - 1 - i);	
	}
}

void Itostr(__u32 num, char *s)
{
	char *o;

	
	o = s;
	if (num < 0)
	{
		*s++ = '-';	
		num = -num;
	}

	while (num > 0)
	{
		*s++ = num % 10 + '0';	
		num = num / 10;
		
	}
	*s = '\0';

	if (*o == '-')
		Reverse(o + 1);
	else
		Reverse(o);

	s = o;
}


