/*
************************************************************************************************************************
*                                                  language definition
*
*                                  Copyright(C), 2006-2010, AllWinner Technology Co., Ltd.
*											       All Rights Reserved
*
* File Name   : elibs_language.h
*
* Author      : Gary.Wang
*
* Version     : 1.0
*
* Date        : 2010.08.25
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
* Gary.Wang      2010.08.25        1.0         build the file
*
************************************************************************************************************************
*/
#ifndef  __ELIBS_LANGUAGE_H__
#define  __ELIBS_LANGUAGE_H__



typedef  enum
{
	EPDK_LANGUAGE_ENM_ENGLISH    = 0x400,      // Ӣ��
	EPDK_LANGUAGE_ENM_CHINESES   = 0x410,      // ��������
	EPDK_LANGUAGE_ENM_CHINESET   = 0x420,      // ��������
    EPDK_LANGUAGE_ENM_JAPANESE   = 0x430,      // ����
    EPDK_LANGUAGE_ENM_KOREAN     = 0x440,      // ����
    EPDK_LANGUAGE_ENM_GERMAN     = 0x450,      // ����
	EPDK_LANGUAGE_ENM_SPANISH    = 0x460,      // ��������
    EPDK_LANGUAGE_ENM_FRENCH     = 0x470,      // ����
    EPDK_LANGUAGE_ENM_ITALIAN    = 0x480,      // �������
    EPDK_LANGUAGE_ENM_PORTUGUESE = 0x490,      // ��������
    EPDK_LANGUAGE_ENM_DUTCH      = 0x4a0,      // ������
    EPDK_LANGUAGE_ENM_RUSSIAN    = 0x4b0,      // ����
    EPDK_LANGUAGE_ENM_POLISH     = 0x4c0,      // ������
    EPDK_LANGUAGE_ENM_TURKISH    = 0x4d0,      // ��������
    EPDK_LANGUAGE_ENM_CZECH      = 0x4e0,      // �ݿ���
    EPDK_LANGUAGE_ENM_DANISH     = 0x4f0,      // ������
    EPDK_LANGUAGE_ENM_HUNGARIAN  = 0x500,      // ��������
    EPDK_LANGUAGE_ENM_SWEDISH    = 0x510,      // �����
    EPDK_LANGUAGE_ENM_THAI       = 0x520,      // ̩��
    EPDK_LANGUAGE_ENM_HEBREW     = 0x530,      // ϣ������
    EPDK_LANGUAGE_ENM_ARABIC     = 0x540,      // ��������

    EPDK_LANGUAGE_ENM_UNKNOWN    = -1          // always the last one
}__epdk_language_enm_e;


#define EPDK_LANGUAGE_NAME_ENGLISH       "ENGLISH"      // Ӣ��    
#define EPDK_LANGUAGE_NAME_CHINESES      "CHINESES"     // ��������
#define EPDK_LANGUAGE_NAME_CHINESET      "CHINESET"     // ��������
#define EPDK_LANGUAGE_NAME_JAPANESE      "JAPANESE"     // ����    
#define EPDK_LANGUAGE_NAME_KOREAN        "KOREAN"       // ����    
#define EPDK_LANGUAGE_NAME_GERMAN        "GERMAN"       // ����    
#define EPDK_LANGUAGE_NAME_SPANISH 	     "SPANISH"      // ��������
#define EPDK_LANGUAGE_NAME_FRENCH        "FRENCH"       // ����    
#define EPDK_LANGUAGE_NAME_ITALIAN       "ITALIAN"      // �������
#define EPDK_LANGUAGE_NAME_PORTUGUESE    "PORTUGUESE"   // ��������
#define EPDK_LANGUAGE_NAME_DUTCH         "DUTCH"        // ������  
#define EPDK_LANGUAGE_NAME_RUSSIAN       "RUSSIAN"      // ����    
#define EPDK_LANGUAGE_NAME_POLISH        "POLISH"       // ������  
#define EPDK_LANGUAGE_NAME_TURKISH       "TURKISH"      // ��������
#define EPDK_LANGUAGE_NAME_CZECH         "CZECH"        // �ݿ���  
#define EPDK_LANGUAGE_NAME_DANISH        "DANISH"       // ������  
#define EPDK_LANGUAGE_NAME_HUNGARIAN     "HUNGARIAN"    // ��������
#define EPDK_LANGUAGE_NAME_SWEDISH       "SWEDISH"      // �����  
#define EPDK_LANGUAGE_NAME_THAI          "THAI"         // ̩��    
#define EPDK_LANGUAGE_NAME_HEBREW        "HEBREW"       // ϣ������
#define EPDK_LANGUAGE_NAME_ARABIC        "ARABIC"       // ��������


#define EPDK_LANGUAGE_NAME_LEN_MAX       31



/*
****************************************************************************************************
*Name        : eLIBs_language_name2enm
*Prototype   : __epdk_language_enm_e  eLIBs_language_name2enm( const char *language_name )
*Arguments   : language_name   input. the name of a language
*Return      : the enum value of the language. return EPDK_LANGUAGE_ENM_UNKNOWN, if fail.
*Description : get the enum value of the language through its name
*Other       :
****************************************************************************************************
*/
extern __epdk_language_enm_e  eLIBs_language_name2enm( const char *language_name );

/*
****************************************************************************************************
*Name        : eLIBs_language_enm2name
*Prototype   : __s32  eLIBs_language_enm2name( __epdk_language_enm_e language_enm, 
*                                              char *language_name, __u32 size )
*Arguments   : language_enm    input. the enum value of a language
*              language_name   output. a buffert to store the name of a language
*              size            input. the buffer's size.
*Return      : ==0      succeed
*              !=0      fail
*Description : get the name of the language through its enum value
*Other       :
****************************************************************************************************
*/
extern __s32  eLIBs_language_enm2name( __epdk_language_enm_e language_enm, char *language_name, __u32 size );



#endif     //  ifndef __ELIBS_LANGUAGE_H__

/* end of elibs_language.h */
