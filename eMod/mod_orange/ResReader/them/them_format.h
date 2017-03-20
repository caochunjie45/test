//------------------------------------------------------------------------------------------------------------
//2008-05-12 19:08:55
//scott
//res.h
//------------------------------------------------------------------------------------------------------------

#ifndef __THEM_FORMAT_H__
#define __THEM_FORMAT_H__     1


#include "themdec_cfg.h"
#include "os_related.h"

//------------------------------------------------------------------------------------------------------------
//��Դ���ݽ���
//------------------------------------------------------------------------------------------------------------
#define RES_SEC_NAME_SIZE	8			//
#define RES_SEC_NAME		".lionres"	//8�ֽڳ���
#define RES_VERSION			0x0100		//��Դ���ݽڸ�ʽ�汾
#define RES_ALIGN			32


//------------------------------------------------------------------------------------------------------------
//��Դ�ļ�����
//------------------------------------------------------------------------------------------------------------
#define SYSRES 				0
#define LIBRES  			1
#define APPRES				2			



//------------------------------------------------------------------------------------------------------------
//��Դ����ͷ����
//------------------------------------------------------------------------------------------------------------
#ifdef WIN32_DATA_PACK
#pragma pack(push, 1)
#endif
typedef __PACKED struct tag_RESHEAD{
	int8		SecName[8];		    //����
	uint16		version;		    //�汾0x0100
	uint16		size;			    //ͷ���ݽṹ��С	
	uint32		StyleOffset;	    //������ݽṹƫ����
	uint32		StyleNum;		    //������
    uint32		ResNum;		        //��Դ����
	uint32		align;			    //���ݱ߽����ģʽ
	uint32		flags;			    //��־��SYSRES LIBRES  APPRES
}RESHEAD;
#ifdef WIN32_DATA_PACK
#pragma pack(pop)
#endif

//���ṹ�Ĵ�С
#define RESHEAD_SIZE	sizeof(RESHEAD)	//32 byte




//------------------------------------------------------------------------------------------------------------
//������������
//------------------------------------------------------------------------------------------------------------
#define STYLE_NAME_SIZE 18

#ifdef WIN32_DATA_PACK
#pragma pack(push, 1)
#endif
typedef __PACKED struct tag_STYLE{
	uint16		ID;					//���ID�� Ψһ
	int8		Name[STYLE_NAME_SIZE];//������ƣ�ASCII��	
	uint32		ResTableOffset;	    //��Դ����ʼ��ַƫ����
	uint32		ResOffset;		    //��Դ������ʼ��ַƫ����
	uint32		ResSize;			//��Դ���ݳ���
}STYLE;
#ifdef WIN32_DATA_PACK
#pragma pack(pop)
#endif

//���ṹ�Ĵ�С
#define STYLE_SIZE	sizeof(STYLE)	//32 byte




//------------------------------------------------------------------------------------------------------------
//��Դ��������
//------------------------------------------------------------------------------------------------------------
#define RES_NAME_SIZE    52

#ifdef WIN32_DATA_PACK
#pragma pack(push, 1)
#endif
typedef __PACKED struct tag_RESITEM{	
    uint16		type;			    //��Դ����
	uint16		ID;				    //��ԴID,��ͬ������ͬ��ԴID��ͬ
	int8		Name[RES_NAME_SIZE];//��Դ���ƣ����ƿ����ظ�
	uint32		offset;		        //��Դ��ʼ��ַ��res�ڵ�ƫ��
	uint32		size;			    //��Դ��size
}RESITEM;
#ifdef WIN32_DATA_PACK
#pragma pack(pop)
#endif

//���ṹ�Ĵ�С
#define RESITEM_SIZE	sizeof(RESITEM) //32



#endif //__THEM_FORMAT_H__

