/*
**********************************************************************************************************************
*                                                    ePDK
*                                    the Easy Portable/Player Develop Kits
*                                              eMOD Sub-System
*
*                                   (c) Copyright 2007-2009, Steven.ZGJ.China
*                                             All Rights Reserved
*
* Moudle  : lion
* File    : layer.h
*
* By      : Steven
* Version : v1.0
* Date    : 2008-10-4 11:28:56
**********************************************************************************************************************
*/

#ifndef __APP_LOAD_PARA_H__
#define __APP_LOAD_PARA_H__


#define SEARCH_MAX_KEYWORD_LEN   1024


/**********************************************************************************************************************/
/* explorer ���ز��� */
/**********************************************************************************************************************/

/*!
  * \brief
  *		��������
*/
typedef enum
{
	EXPLR_APP_MAIN,
	EXPLR_APP_MUSIC,
	EXPLR_APP_PHOTO,
	EXPLR_APP_MOVIE,
	EXPLR_APP_EBOOK,
	EXPLR_APP_GAME,
	EXPLR_APP_SEARCH,
	EXPLR_APP_FLASH,
	EXPLR_APP_ALL
}explr_type_t;
/*!
  * \brief
  *		explorer����ģʽö��
*/
typedef enum
{
	/*! �Ӹ����*/
	EXPLR_APP_ROOT,
	/*! �ָ����*/
	EXPLR_APP_RESTORE
}explr_mode_t;
/*!
  * \brief
  *		explorer�ļ��ز���
*/
typedef struct
{
	/*! Ӧ������*/
	explr_type_t type;
	/*! ����ģʽ*/
	explr_mode_t mode;
	/*! �Ƿ���Ҫ�ļ�������0��Ҫ��1��Ҫ*/
	__u32 		 file_op_flag;
	/*!	�Ƿ���Ҫ��ʾ����ͼ 0��Ҫ��1Ҫ*/
	__u32 miniature_flag;
	
	__u32 search_type;
	char  search_word[SEARCH_MAX_KEYWORD_LEN];
	void 		*attr;
}explr_arg_t;

#endif /* __APP_LOAD_PARA_H__ */
