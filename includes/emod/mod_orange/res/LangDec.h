/*
*********************************************************************************************************
*File	:	Decord.h
*version:	v1.0	2008-10-28	
*By		:	Andy.zhang
*Brief	:	���չ涨�ĸ�ʽ��ȡdata�ļ��е����� (vc++ 6.0)
*********************************************************************************************************
*/

#ifndef __LANGDEC__H__
#define __LANGDEC__H__


#define	HLANG	void *
#define OK		0
#define Error	-1

//�����������ļ�
HLANG	Lang_Open(char *szAppFile, __u32 mode);

//����������
int	Lang_Read(HLANG hLang, int address, int length, char *buffer);

//ȡ�������ַ�����ַ
int Lang_GetStringAddress(HLANG hLang, short LangID, short StringID);

//ȡ�������ַ�������
int Lang_GetStringSize(HLANG hLang, short LangID, short StringID);

//ȡ�������ַ�������
int Lang_GetString(HLANG hLang, short LangID, short StringID, char *buffer, int length);

//�ر���������
int Lang_Close(HLANG hLang);

#endif /* __LANGDEC__H__ */
