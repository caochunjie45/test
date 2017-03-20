/*
*********************************************************************************************************
*File	:	Decord.cpp
*version:	v1.0	2008-10-28
*By		:	Andy.zhang
*Brief	:	���չ涨�ĸ�ʽ��ȡdata�ļ��е�����
*********************************************************************************************************
*/

#include "LangDec_private.h"
#ifdef SIM_PC_WIN
extern __declspec(dllimport) FILE* OpenLangue(const char * szAppFile);
#pragma pack(1)
#endif
/**
*@brief:  ��ָ��·���ļ��������ļ������
*@param:  szAppFile	[in]	�ļ�·����
*@param:  mode		[in]	��ģʽ��Ŀǰ�汾�˲���û������
*@return: HLANG		HHEAD*	���ݽṹ
**/
HLANG	Lang_Open(char *szAppFile, __u32 mode)
{
	FILE		*fp;
	HHEAD		*pHHead;

	LANG_HEAD	m_LangHead;
	LANG		m_Lang;

	char		*m_langbuffer;
	unsigned int i;

/*
#if LANGDEC_CHECK_ARG_EN > 0
	if(szAppFile == NULL)
	{
	    __wrn("szAppFile is empty!\n");
	    return NULL;
	}
#endif
*/

	mode = mode;

	//�����ڴ�HHEAD *
	pHHead = (HHEAD *)malloc(sizeof(HHEAD));
	if(pHHead == NULL)
	{
	#if LANGDEC_CHECK_ARG_EN > 0
	    __wrn("pHHead is empty!\n");
	#endif
	    return NULL;
	}
#ifndef SIM_PC_WIN
#if EPDK_OS == EPDK_OS_EPOS
    fp = fopen1(szAppFile, "r_lang");//(FILE*)esRESM_ROpen(szAppFile, "r_lang");//
#else
	fp = fopen1(szAppFile, "rb");
#endif
#else
 fp = fopen1(szAppFile, "r_lang");//(FILE*)esRESM_ROpen(szAppFile, "r_lang");//
#endif
	if(fp == NULL)
	{
	#if LANGDEC_CHECK_ARG_EN > 0
	    __wrn("open file fail!\n");
	#endif
	    free((void *)pHHead);
	    return NULL;
	}

	pHHead->fp = fp;

	//��ȡlang_head
	fseek1(fp, 0, SEEK_SET);
	fread1(&m_LangHead, sizeof(m_LangHead), 1,fp);

	//�ж��ļ�ͷ���Ƿ�ָ������
	if(strcmp((char *)m_LangHead.SecName, SEC_NAME))
	{
	#if LANGDEC_CHECK_ARG_EN > 0
		__inf("the file is not lang bin file");
	#endif
		fclose1(fp);
		free((void *)pHHead);
		return NULL;
	}

	//���ݶ�ȡ�ļ�ͷ����ʼ��HHEAD *���ݽṹ
	pHHead->LangTabOff	= m_LangHead.LangTabOff;
	pHHead->LangSize	= m_LangHead.LangSize;
	pHHead->LangNum		= m_LangHead.LangNum;
	pHHead->StringSize	= m_LangHead.StringSize;
	pHHead->StringNum	= m_LangHead.StringNum;
	pHHead->align		= m_LangHead.align;
	if(m_LangHead.version < 0x0200)
		pHHead->pIdTab = NULL;
	else
	{
		pHHead->pIdTab = (__u16*)malloc(m_LangHead.StringNum * sizeof(__u16));
		fseek1(fp, 44, SEEK_SET);
		fread1(pHHead->pIdTab, 1, m_LangHead.StringNum * sizeof(__u16), fp);
	}

	//�����ڴ棬����lang_list
	m_langbuffer = (char *)malloc(m_LangHead.LangNum*m_LangHead.LangSize*sizeof(char));
	//Ѱ�ҵ�lang_list��ʼ��ַ
	fseek1(fp, m_LangHead.LangTabOff, SEEK_SET);
	//��ȡlang_list
	fread1(m_langbuffer, sizeof(char), m_LangHead.LangNum*m_LangHead.LangSize,fp);

	pHHead->pHLangList	= (HLANGLIST *)malloc( sizeof(HLANGLIST)*pHHead->LangNum );

	for(i = 0; i < pHHead->LangNum; i++)
	{
		//4�ֽڶ���,ע�⿽������
		memcpy(&m_Lang, m_langbuffer+i*m_LangHead.LangSize, sizeof(m_Lang));//4�ֽڶ��룻

		pHHead->pHLangList[i].LangID		= m_Lang.LangID;
		pHHead->pHLangList[i].StringTabOff	= m_Lang.StringTabOff;
	}

	//�ͷ��ڴ�
	free(m_langbuffer);
	return (void *)pHHead;
}

/**
*@brief:  ��ȡ�ļ�ָ����ַ��������
*@param:  hLang		[in]	�ļ������
*@param:  address	[in]	����ļ���㴦��ƫ�Ƶ�ַ��
*@param:  length	[in]	��ȡ�����ݳ���
*@param:  length	[out]	�����ȡ������
*@return: ʵ�ʶ�ȡ���ֽ���
**/
int	Lang_Read(HLANG hLang, int address, int length, char *buffer)
{
	FILE	*fp;
	int		nr_read;
	HHEAD	*pHHead;

#if LANGDEC_CHECK_ARG_EN > 0
    if(hLang == NULL)
    {
        __wrn("hlang is null!\n");
        return 0;
    }
#endif

	pHHead	= (HHEAD *)hLang;
	fp		= pHHead->fp;

	//Ѱ�ҵ�ָ���ڴ�
	fseek1(fp, address, SEEK_SET);
	//��ȡ����
	nr_read = fread1(buffer, sizeof(char), length, fp);
	return nr_read;
}

/**
*@brief:  ��ѯ���ݵ�ַ
*@param:  hLang		[in]	�ļ������
*@param:  LangID	[in]	����ID
*@param:  StringID	[in]	SringID, ��Lang.bat���ɵ�Lang.h�ļ��ж���
*@return: ��ַ(����ļ���ʼƫ����)
**/
int Lang_GetStringAddress(HLANG hLang, short LangID, short StringID)
{
	unsigned int i;

	FILE	*fp;
	LANG_STRING	m_String;

	HHEAD		*pHhead;
	HLANGLIST	 m_HLangList;

	char *m_strbuffer;
	int	 addr = 0;

#if LANGDEC_CHECK_ARG_EN > 0
    if(hLang == NULL)
    {
        __wrn("hlang is null!\n");
        return 0;
    }
#endif

	pHhead = (HHEAD *)hLang;
	fp	   = pHhead->fp;
	
	//��ѯHHEAD���ݽṹ��Ѱ��LangID��
	for(i = 0; i < pHhead->LangNum; i++)
	{
		m_HLangList = pHhead->pHLangList[i];

		//�ҵ�LangID��
		//ע��StringID��1��ʼ�����ε����Ĵ�ȡ��bin�ļ��У�����ֱ��Ѱ�ҵ���ʼ��ַ
		if(m_HLangList.LangID == LangID)
		{
			//�ƶ��ļ�ָ�뵽StringID����ʼ��ַ

			//////////////////////////////////////////////////////////////////////////
			// ��������
			int nIndex = StringID-1;
			int n;
 			if(pHhead->pIdTab)
			{
				nIndex = -1;
				for(n = 0; n < pHhead->StringNum; n++)
				{
					if(pHhead->pIdTab[n] == StringID)
					{
						nIndex = n;
						break;
					}
				}				
				//���仺�棬����stringlist����
			}
			if(nIndex == -1)
			{
				 __wrn("strID error!\n");
				 return 0;
			}
			fseek1(fp, m_HLangList.StringTabOff+(nIndex)*(pHhead->StringSize), SEEK_SET);
			m_strbuffer = (char *)malloc(pHhead->StringSize);
			fread1(m_strbuffer, sizeof(char), pHhead->StringSize, fp);
			//4�ֽڶ��룬
			memcpy(&m_String, m_strbuffer, sizeof(m_String));

			if(m_String.StringID == StringID)
			{
				addr = m_String.offset;
			}
			else
			{
				addr = 0;
			}
        	free(m_strbuffer);
		}
	}
	return addr;
}

/**
*@brief:  ��ѯ���ݳ���
*@param:  hLang		[in]	�ļ������
*@param:  LangID	[in]	����ID
*@param:  StringID	[in]	SringID, ��Lang.bat���ɵ�Lang.h�ļ��ж���
*@return: ��ѯ���ݳ���(byte)
**/
int Lang_GetStringSize(HLANG hLang, short LangID, short StringID)
{
	unsigned int i;

	FILE	*fp;
	LANG_STRING	m_String;

	HHEAD		*pHhead;
	HLANGLIST	m_HLangList;

	char *m_strbuffer;
	int	 size = 0;

#if LANGDEC_CHECK_ARG_EN > 0
    if(hLang == NULL)
    {
       __wrn("hlang is null!\n");
       return 0;
    }
#endif
    pHhead = (HHEAD *)hLang;
	fp	   = pHhead->fp;

	for(i = 0; i < pHhead->LangNum; i++)
	{
		m_HLangList = pHhead->pHLangList[i];

		if(m_HLangList.LangID == LangID)
		{
			//////////////////////////////////////////////////////////////////////////
			// ��������
			int nIndex = StringID-1;
			int n;
 			if(pHhead->pIdTab)
			{
				nIndex = -1;
				for(n = 0; n < pHhead->StringNum; n++)
				{
					if(pHhead->pIdTab[n] == StringID)
					{
						nIndex = n;
						break;
					}
				}				
				//���仺�棬����stringlist����
			}
			if(nIndex == -1)
			{
				 __wrn("strID error!\n");
				 return 0;
			}
			fseek1(fp, m_HLangList.StringTabOff+(nIndex)*(pHhead->StringSize), SEEK_SET);

			m_strbuffer = (char *)malloc(pHhead->StringSize);

			fread1(m_strbuffer, sizeof(char), pHhead->StringSize, fp);
			memcpy(&m_String, m_strbuffer, sizeof(m_String));

			if(m_String.StringID == StringID)
			{
				//�������ݳ���
				size = m_String.size;
			}
			else
			{
				size = 0;
			}
        	free(m_strbuffer);
		}
	}
	return size;

}

/**
*@brief:  ��ѯ����
*@param:  hLang		[in]	�ļ������
*@param:  LangID	[in]	����ID
*@param:  StringID	[in]	SringID, ��Lang.bat���ɵ�Lang.h�ļ��ж���
*@param:  buffer	[out]	�������������
*@param:  buffer	[in]	�����������������
*@return: ����ʵ�ʳ���(byte)
**/
int Lang_GetString(HLANG hLang, short LangID, short StringID, char *buffer, int length)
{

	unsigned int i;

	FILE	*fp;
	LANG_STRING	m_String;

	HHEAD		*pHhead;
	HLANGLIST	m_HLangList;

	char *m_strbuffer;

#if LANGDEC_CHECK_ARG_EN > 0
    if(hLang == NULL)
    {
       __wrn("hlang is null!\n");
        return 0;
    }
#endif

	pHhead = (HHEAD *)hLang;
	fp	   = pHhead->fp;

	for(i = 0; i < pHhead->LangNum; i++)
	{
		m_HLangList = pHhead->pHLangList[i];

		if(m_HLangList.LangID == LangID)
		{
			//////////////////////////////////////////////////////////////////////////
			// ��������
			int nIndex = StringID-1;
			int n;
 			if(pHhead->pIdTab)
			{
				nIndex = -1;
				for(n = 0; n < pHhead->StringNum; n++)
				{
					if(pHhead->pIdTab[n] == StringID)
					{
						nIndex = n;
						break;
					}
				}				
				//���仺�棬����stringlist����
			}
			if(nIndex == -1)
			{
				 __wrn("strID error!\n");
				 return 0;
			}
			
			fseek1(fp, m_HLangList.StringTabOff+(nIndex)*(pHhead->StringSize), SEEK_SET);

		//	fseek1(fp, m_HLangList.StringTabOff+(StringID-1)*(pHhead->StringSize), SEEK_SET);
			m_strbuffer = (char *)malloc(pHhead->StringSize);
			fread1(m_strbuffer, sizeof(char), pHhead->StringSize, fp);
			memcpy(&m_String, m_strbuffer, sizeof(m_String));
            
			if(m_String.StringID == StringID)
			{
				//�ƶ��ļ�ָ�뵽���ݵ���ʼ��ַ
				fseek1(fp, m_String.offset, SEEK_SET);
				if( m_String.size<= (unsigned)length)
				{
					//�����ݵ�buffer
					fread1(buffer, sizeof(char), m_String.size,fp);
					free(m_strbuffer);
					return m_String.size;
				}
				else
				{
					fread1(buffer, sizeof(char), length, fp);
					free(m_strbuffer);
					return length;
				}
			}

			free(m_strbuffer);
			return 0;
		}

	}
	return 0;

}
/**
*@brief:  �رվ�����ͷ��ڴ�
*@param:  hLang		[in]	�ļ������
**/

int Lang_Close(HLANG hLang)
{
	FILE		*fp;
	HHEAD		*pHhead;

#if LANGDEC_CHECK_ARG_EN > 0
    if(hLang == NULL)
    {
        __wrn("hlang is null!\n");
        return 0;
    }
#endif

	pHhead = (HHEAD *)hLang;
	fp	   = pHhead->fp;
	//�ر��ļ�
	fclose1(fp);

	//�ͷ��ڴ棻
	free(pHhead->pHLangList);
	free(pHhead);

	return OK;
}
