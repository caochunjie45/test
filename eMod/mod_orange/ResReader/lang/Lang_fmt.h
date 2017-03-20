/*
**********************************************************************************************************************
*                                                    ePDK
*                                    the Easy Portable/Player Develop Kits
*                                              eMOD Sub-System
*
*                                   (c) Copyright 2007-2009, SW.China
*                                             All Rights Reserved
*
* Moudle  : Lemon
* File    : Lang_fmt.h
*
* By      : Andy
* Version : v1.0
* Date    : 2008-11-9 9:13:15
**********************************************************************************************************************
*/

#ifndef __LANG_FMT_H__
#define __LANG_FMT_H__


#define	SEC_NAME		".langdat"	//8�ֽڳ��ȣ�����


//������Դ�ļ����ݸ�ʽ����
#if (EPDK_OS == EPDK_OS_WIN2K) || (EPDK_OS == EPDK_OS_WINXP) || (EPDK_OS == EPDK_OS_VISTA)
    #pragma pack(push, 1)
#endif
#ifdef SIM_PC_WIN
#pragma pack(1)
#endif
typedef __PACKED struct tag_LANG_HEAD{
	__s8		SecName[8];		//����
	__u16		version;		//�汾��0x0100
	__u16		size;			//LANG_HEAD���ݽṹ��С
	__u32		LangTabOff;		//LANG����ʼ��ַƫ����----------------->
	__u16		LangSize;		//LANG����ÿ����Ŀ��С
	__u32		LangNum;		//LANG����Ŀ����,�������������
	__u16		StringSize;		//�ַ���size
	__u32		StringNum;		//�ַ���������
	__u32		align;			//���ݱ߽����ģʽ��
	__u32		flags;			//��־:SYSRES,LIBRES,APPRES
}LANG_HEAD; //34 byte
#if (SIM_PC_WIN)||(EPDK_OS == EPDK_OS_WIN2K) || (EPDK_OS == EPDK_OS_WINXP) || (EPDK_OS == EPDK_OS_VISTA)
#pragma pack(pop)
#endif

#if (SIM_PC_WIN) ||(EPDK_OS == EPDK_OS_WIN2K) || (EPDK_OS == EPDK_OS_WINXP) || (EPDK_OS == EPDK_OS_VISTA)
#pragma pack(push, 1)
#endif
typedef __PACKED struct tag_LANG{	
	//__s8		Name[40];			//LANG_CHINESE_TYPE, LANG_ENGLISH_TYPE
	__u16		LangID;				//0X400 0X420
	__u32		LangOffSet;			//LANG ��Ŀ��ʼ��ַƫ����
	__u32		LangSize;			//LANG ��Ŀ��С
	__u32		StringTabOff;		//�ַ�����ʼ��ַ
}LANG; //
#if (SIM_PC_WIN) ||(EPDK_OS == EPDK_OS_WIN2K) || (EPDK_OS == EPDK_OS_WINXP) || (EPDK_OS == EPDK_OS_VISTA)
#pragma pack(pop)
#endif


#if (SIM_PC_WIN) ||(EPDK_OS == EPDK_OS_WIN2K) || (EPDK_OS == EPDK_OS_WINXP) || (EPDK_OS == EPDK_OS_VISTA)
#pragma pack(push, 1)
#endif
typedef __PACKED struct tag_STRING{
	__u16		LangID;			//LangID
	//__s8		Name[32];		//such as:STRING_HELLO	
	__u16		StringID;		//such as:0x0001	
	__u32		offset;			//string dataλ��
	__u32		size;			//data���ݳ���
}LANG_STRING;
#if (SIM_PC_WIN) ||(EPDK_OS == EPDK_OS_WIN2K) || (EPDK_OS == EPDK_OS_WINXP) || (EPDK_OS == EPDK_OS_VISTA)
#pragma pack(pop) 
#endif

#endif /* __LANG_FMT_H__  */




