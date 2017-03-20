/*
************************************************************************************************************************
*                                                    Charset
*
*                             Copyright(C), 2006-2009, SoftWinners Microelectronic Co., Ltd.
*											       All Rights Reserved
*
* File Name   : sys_charset.h
*
* Author      : Gary.Wang
*
* Version     : 1.1.0
*
* Date        : 2009.08.22
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
* Gary.Wang      2009.08.22       1.1.0        build the file
*
************************************************************************************************************************
*/
#ifndef  _SYS_CHARSET_H_
#define  _SYS_CHARSET_H_



typedef  enum
{
	EPDK_CHARSET_ENM_GB2312  = 0,       // ��������
	EPDK_CHARSET_ENM_UTF8       ,       // utf8
	EPDK_CHARSET_ENM_UTF16BE    ,       // utf16be
	EPDK_CHARSET_ENM_UTF16LE    ,       // utf16le
    EPDK_CHARSET_ENM_BIG5       ,       // ��������
    EPDK_CHARSET_ENM_GBK        ,       // ����
    EPDK_CHARSET_ENM_SJIS       ,       // ����
    EPDK_CHARSET_ENM_EUC_JP     ,       // ����, �ݲ�֧��
    EPDK_CHARSET_ENM_EUC_KR     ,       // ����
    EPDK_CHARSET_ENM_KIO8_R     ,       // ����
    EPDK_CHARSET_ENM_ISO_8859_1 ,       // cp1252.��ŷ����
    EPDK_CHARSET_ENM_ISO_8859_2 ,       // ��ŷ����
    EPDK_CHARSET_ENM_ISO_8859_3 ,       // ��ŷ����
    EPDK_CHARSET_ENM_ISO_8859_4 ,       // ��ŷ����
    EPDK_CHARSET_ENM_ISO_8859_5 ,       // �������ĸ
    EPDK_CHARSET_ENM_ISO_8859_6 ,       // ��������
    EPDK_CHARSET_ENM_ISO_8859_7 ,       // ϣ����
    EPDK_CHARSET_ENM_ISO_8859_8 ,       // ϣ������, �ݲ�֧��
    EPDK_CHARSET_ENM_ISO_8859_9 ,       // cp1254����������
    EPDK_CHARSET_ENM_ISO_8859_10,       // ��ŷ˹������ά����ϵ
    EPDK_CHARSET_ENM_ISO_8859_11,       // ̩��, �ݲ�֧��
    EPDK_CHARSET_ENM_ISO_8859_12,       // ����, �ݲ�֧��
    EPDK_CHARSET_ENM_ISO_8859_13,       // ���޵ĺ���ϵ
    EPDK_CHARSET_ENM_ISO_8859_14,       // ����������ϵ
    EPDK_CHARSET_ENM_ISO_8859_15,       // ��չ�˷���ͷ��������ŷ��ϵ
    EPDK_CHARSET_ENM_ISO_8859_16,       // ��չ�Ķ���ŷ��ϵ, �ݲ�֧��
    EPDK_CHARSET_ENM_CP874      ,       // ̩��
    EPDK_CHARSET_ENM_CP1250     ,       // ��ŷ
    EPDK_CHARSET_ENM_CP1251     ,       // �������
    EPDK_CHARSET_ENM_CP1253     ,       // ϣ����, �ݲ�֧��
    EPDK_CHARSET_ENM_CP1255     ,       // ϣ������
    EPDK_CHARSET_ENM_CP1256     ,       // ��������, �ݲ�֧��
    EPDK_CHARSET_ENM_CP1257     ,       // ���޵ĺ���, �ݲ�֧��
    EPDK_CHARSET_ENM_CP1258     ,       // Խ��, �ݲ�֧��
    EPDK_CHARSET_ENM_CP1252     ,       // ��ŷ���ԡ�cp1252��iso-8859-1�ĳ���
    EPDK_CHARSET_ENM_CP1254     ,       // �������cp1254��iso-8859-9�ĳ���
	  EPDK_CHARSET_ENM_JIS        ,       // ����JIS�ַ�����
    EPDK_CHARSET_ENM_NUM        ,
    EPDK_CHARSET_ENM_UNKNOWN = -1       // always the last one
}__epdk_charset_enm_e;


#define EPDK_CHARSET_NAME_GB2312        "CHARSET_GB2312"
#define EPDK_CHARSET_NAME_UTF8          "CHARSET_UTF8"
#define EPDK_CHARSET_NAME_UTF16BE       "CHARSET_UTF16BE"
#define EPDK_CHARSET_NAME_UTF16LE       "CHARSET_UTF16LE"
#define EPDK_CHARSET_NAME_BIG5          "CHARSET_BIG5"
#define EPDK_CHARSET_NAME_GBK           "CHARSET_GBK"
#define EPDK_CHARSET_NAME_SJIS          "CHARSET_SJIS"
#define EPDK_CHARSET_NAME_EUC_JP        "CHARSET_EUC_JP"
#define EPDK_CHARSET_NAME_EUC_KR        "CHARSET_EUC_KR"
#define EPDK_CHARSET_NAME_KIO8_R        "CHARSET_KIO8_R"
#define EPDK_CHARSET_NAME_ISO_8859_1    "CHARSET_ISO_8859_1"
#define EPDK_CHARSET_NAME_ISO_8859_2    "CHARSET_ISO_8859_2"
#define EPDK_CHARSET_NAME_ISO_8859_3    "CHARSET_ISO_8859_3"
#define EPDK_CHARSET_NAME_ISO_8859_4    "CHARSET_ISO_8859_4"
#define EPDK_CHARSET_NAME_ISO_8859_5    "CHARSET_ISO_8859_5"
#define EPDK_CHARSET_NAME_ISO_8859_6    "CHARSET_ISO_8859_6"
#define EPDK_CHARSET_NAME_ISO_8859_7    "CHARSET_ISO_8859_7"
#define EPDK_CHARSET_NAME_ISO_8859_8    "CHARSET_ISO_8859_8"
#define EPDK_CHARSET_NAME_ISO_8859_9    "CHARSET_ISO_8859_9"
#define EPDK_CHARSET_NAME_ISO_8859_10   "CHARSET_ISO_8859_10"
#define EPDK_CHARSET_NAME_ISO_8859_11   "CHARSET_ISO_8859_11"
#define EPDK_CHARSET_NAME_ISO_8859_12   "CHARSET_ISO_8859_12"
#define EPDK_CHARSET_NAME_ISO_8859_13   "CHARSET_ISO_8859_13"
#define EPDK_CHARSET_NAME_ISO_8859_14   "CHARSET_ISO_8859_14"
#define EPDK_CHARSET_NAME_ISO_8859_15   "CHARSET_ISO_8859_15"
#define EPDK_CHARSET_NAME_ISO_8859_16   "CHARSET_ISO_8859_16"
#define EPDK_CHARSET_NAME_CP874         "CHARSET_CP874"
#define EPDK_CHARSET_NAME_CP1250        "CHARSET_CP1250"
#define EPDK_CHARSET_NAME_CP1251        "CHARSET_CP1251"
#define EPDK_CHARSET_NAME_CP1253        "CHARSET_CP1253"
#define EPDK_CHARSET_NAME_CP1255        "CHARSET_CP1255"
#define EPDK_CHARSET_NAME_CP1256        "CHARSET_CP1256"
#define EPDK_CHARSET_NAME_CP1257        "CHARSET_CP1257"
#define EPDK_CHARSET_NAME_CP1258        "CHARSET_CP1258"


#define EPDK_CHARSET_NAME_LEN_MAX       31


//************************************************
// ��������
/* system call table */
typedef struct
{
    __pSWI_t esCHS_Uni2Char       ;
    __pSWI_t esCHS_Char2Uni       ;
    __pSWI_t esCHS_GetChUpperTbl  ;
    __pSWI_t esCHS_GetChLowerTbl  ;

} SWIHandler_CHS_t;

#ifndef SIM_PC_WIN
#define SYSCALL_CHS(x,y) __swi(SWINO(SWINO_CHS, SWIHandler_CHS_t, y))x y
#else
#define SYSCALL_CHS(x,y) x y
#endif

/***************************************************************************************************
*Name        : esCHSuni2char
*Prototype   : __s32 esCHSuni2char( __s32 charset_enm, __u16 uni, __u8 *stream, __u32 len )
*Arguments   : charset_enm   input. the type of charset
*              uni           input. unicode value
*              stream        output. charset stream pointer
*              len           input. the length of charset stream
*Return      : The length of the specfic encoding byte stream converted from the corresponding unicode.
*              "<=0" means that converting failed.
*Description : Convert a unicode to the specfic charset.
*Other       :
***************************************************************************************************/
SYSCALL_CHS(__s32,  esCHS_Uni2Char      )(__s32 charset_enm, __u16 uni, __u8 *stream, __u32 len);


/***************************************************************************************************
*Name        : esCHSchar2uni
*Prototype   : __s32 esCHSchar2uni( __s32 charset_enm, const __u8 *stream, __u32 len, __u16 *uni )
*Arguments   : charset_enm   input. the type of charset
*              stream        input. charset stream pointer
*              len           input. the length of charset stream
*              uni           output. the buffer to store the unicode value.
*Return      : The length of the specfic encoding byte stream corresponding to the unicode. "<=0" means
*              that converting failed.
*Description : Convert the specfic charset to a unicode.
*Other       :
***************************************************************************************************/
SYSCALL_CHS(__s32,  esCHS_Char2Uni      )(__s32 charset_enm, const __u8 *stream, __u32 len, __u16 *uni);


/***************************************************************************************************
*Name        : esCHSget_charset2upper_table
*Prototype   : __u32 esCHSget_charset2upper_table( __s32 charset_enm, void *buf, __u32 len );
*Arguments   : charset_enm   input. the type of charset
*              buf           output. the buffer to store the specfic charset2upper table.
*              size          input. the size of the buffer.
*Return      : the valid size of the specfic charset2upper table. 0 means that error occured.
*Description : get the content of the specfic charset2upper table.
*Other       :
***************************************************************************************************/
SYSCALL_CHS(__u32,   esCHS_GetChUpperTbl )(__s32 charset_enm, void *buf, __u32 size);


/***************************************************************************************************
*Name        : esCHSget_charset2lower_table
*Prototype   : __u32 esCHSget_charset2lower_table( __s32 charset_enm, void *buf, __u32 len );
*Arguments   : charset_enm   input. the type of charset
*              buf           output. the buffer to store the specfic charset2lower table.
*              size          input. the size of the buffer.
*Return      : the valid size of the specfic charset2lower table. 0 means that error occured.
*Description : get the content of the specfic charset2lower table.
*Other       :
***************************************************************************************************/
SYSCALL_CHS(__u32,   esCHS_GetChLowerTbl )(__s32 charset_enm, void *buf, __u32 size);


/* iso-8859-1 */
/*��ŷ���ԣ������������������˹��������������̩������������������������������
* ���������������������������������Ƕ�������������¬ɭ���Ų����������
* ��������˹��ո����Ƕ���������Ｐ����� */


/* iso-8859-2 */
/*��ŷ���ԣ����ַ�����Ҫ֧Ԯ�������֣����޵�����ݿ��������������˹�工���˹����������
* �����������������������������Ӣ����������Ҳ���ô��ַ�����ʾ��*/


/* iso-8859-3 */
/*��ŷ���ԣ���ԭ���������ʾ�������Ｐ����������֡������������Ѹ���ISO/IEC 8859-9��ʾ����ʱֻ������
* �Ｐ���������ʹ�ô��ַ��������ַ���ͬʱ��֧Ԯ�������֣�Ӣ���������������Ｐ�������*/


/* iso-8859-4 */
/*��ŷ���ԣ����������ʾ��ɳ����������������ά����������Ｐ���������ԣ�S��mi���������֡�����
* ����ͬʱ��֧Ԯ�������֣������Ӣ��������������Ų���˹���������Ｐ����*/


/* iso-8859-5 */
/*���ּ���¼�������ĸ��������׶���˹��������������������ά����ڿ�����ʹ��*/


/* iso-8859-6 */
/*���ִ���������ʹ�á�����ͬ�ڰ�������׼ASMO-708������ܶ��ڰ�������ʹ�õ���ĸ��δ��֧Ԯ���ʽ�������
* ����Unicodeȡ��*/


/* iso-8859-7 */
/*���ִ�ϣ����ʹ�á���ֻ���ִ�ϣ����ĵ�����ĸ��Monotonic Greek����û����¼��ϣ����ʹ�õĶ�����ĸ
* ��Polytonic Greek����ϣ���Ĺ��ұ�׼ELOT 928��ͬ�ڴ��ּ�*/


/* iso-8859-8 */
/*��ϣ������ʹ�á���ֻ��ϣ�����︨����ĸ����û��Ԫ�����š�*/


/* iso-8859-9 */
/*��ʾ�������Ｐ����������֡�����ַ�����ISO/IEC 8859-1�ݻ����������������Ｐ��������⣬����������
* ���˹��������������̩�����������������Ӣ�����˹���������������������
* ���������Ƕ����ƴ�ַ����������������¬ɭ���Ų������������������˹��ո����Ƕ�
* ������Ҳ���ô��ַ�����ʾ��*/


/* iso-8859-10 */
/*Ϊ���ն�����֧��˹������ά�ǣ������Զ����һ��8λ�ַ��������������ʾ���������������������
*�Ų���Ｐ��������֡����ַ���ͬʱ��֧Ԯ�������֣�Ӣ���ɳ��������������������Ƕ���
* ����ƴ�ַ�����������������˹���������Ｐ���������Բ�������*/


/* iso-8859-11 */
/*̩����ַ�������̩����ҵ��׼ TIS-620��*/


/* iso-8859-13 */
/*Ϊ���޵���������Զ����һ��8λ�ַ�����������ʾ��ɳ��������������ά���Ｐ�����������֡�����
* ����ͬʱ��֧ԮӢ������Ｐ���ֵ�������˹���������Ｐ��������֡�*/


/* iso-8859-14 */
/*���Ա�ʾ��������������֣������������������������������Ƕ����ƴ�ַ�����ƴ�ַ����������
* ����ʿ��ȡ�����֮�⣬�������������˹�����̩������������Ӣ������������������
* �������¬ɭ���Ų������������������˹�������������Ҳ���ô��ַ�����ʾ��*/


/* iso-8859-15 */
/*������ISO/IEC 8859-1�ַ���ȱ�ٵ����ɷ�����ĸ������������ĸ��ͬʱ������� �裨ͨ�û��ҷ��ţ����� �
* ��ŷԪ���ţ������ַ���ͬʱ��֧Ԯ�������֣��������������˹��������������̩�������������
* �������Ӣ���ɳ����������������������˹���������������������������
* �����Ƕ����ƴ�ַ����������������¬ɭ���Ų������������������˹��ո����Ƕ��
* �����������*/


/* iso-8859-16 */
/*����ַ�����������ǰ�������������޵�������������������������������Ｐ˹����������
* �ȶ���ŷ�������ԡ����������������⣬���ַ���ͬʱ��֧Ԯ�������֣�Ӣ��������������������
* �����ƴ�ַ����������*/




//************************************************
// ��������
extern SWIHandler_CHS_t SWIHandler_CHS;
//************************************************
// ��������

/***************************************************************************************************
*Name        : charset_init
*Prototype   : __s32 charset_init( __u32 mode, void *p_arg )
*Arguments   : mode       input. mode, whose value is always 0 at present
*              p_arg      input. a pointer to the parameter, whose value is always NULL at present
*Return      : 0    succeed in initialising.
*              !=0  fail in initialising.
*Description : initialise charset module
*Other       :
***************************************************************************************************/
extern __s32 CHS_Init( __u32 mode, void *p_arg );
/***************************************************************************************************
*Name        : charset_exit
*Prototype   : void charset_exit( void )
*Arguments   : void
*Return      : void
*Description : exit charset module
*Other       :
***************************************************************************************************/
extern void CHS_Exit( void );



#endif     //  ifndef _SYS_CHARSET_H_

