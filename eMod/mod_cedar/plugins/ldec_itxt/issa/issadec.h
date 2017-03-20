/*
*********************************************************************************************************
*                                                    LYRIC DECODE LIB
*                                                      *.SSA Format
*
*                                       (c) Copyright 2008-2009, Kevin.z China
*                                                   All Rights Reserved
*
* File   : srtdec.h
* Version: V1.0
* By     : kevin
* Date   : 2008-7-17 11:23
*********************************************************************************************************
*/
#ifndef _ISSADEC_H_
#define _ISSADEC_H_

//#include "ldec_issa_i.h"

#define _IsNumeric(x)       (((x < '0') || (x > '9'))? 0 : 1)
#define isalpha(c) ((('a' <= (c)) && ((c) <= 'z')) || (('A' <= (c)) && ((c) <= 'Z')))
#define isupper(c) (('A' <= (c)) && ((c) <= 'Z'))
#define tolower(c) (isupper(c) ? (((c) - 'A') + 'a') : (c))

#define SSA_MAX_NAME_LENGTH 30
#define SSA_MAX_STYLE_NUM 256

/*Marked, Start, End, Style, Name, MarginL, MarginR, MarginV, Effect, Text */
typedef struct __SSA_DIALOGUE_ITEM
{
    __s32 nStartTime;     //������Ļ����ʼʱ��
    __s32 nEndTime;       //������Ļ�Ľ���ʱ��
    __u8 *pText;         //������Ļ���ı�����
    __u32 nTextLen;       //������Ļ�����ݳ���
    __u16 style;  //style index
} __ssa_dialogue_item_t;

typedef struct __SSA_STYTLE
{
    __u8 stylename[SSA_MAX_NAME_LENGTH];  //style
    __u32 encoding;
} __ssa_stytle_item_t;


typedef struct __SSA_DIALOGUE_NODE
{
    __ssa_dialogue_item_t      dialogueItem;
    struct __SSA_DIALOGUE_NODE      *pRev;
    struct __SSA_DIALOGUE_NODE      *pNext;

} __ssa_dialogue_node_t;



typedef enum __SSA_LINE_DESCRIPTOR 
{
    SSA_LINE_DESCRIPTOR_ERR = 0,
    SSA_LINE_DESCRIPTOR_IGNORED,
    SSA_LINE_DESCRIPTOR_SECTION,
    SSA_LINE_DESCRIPTOR_DIALOGUE,
    SSA_LINE_DESCRIPTOR_STYLE,
    SSA_LINE_DESCRIPTOR_FORMAT,
}__ssa_line_descriptor_t;


typedef struct __SSA_DEC_INFO
{
    __u32 txtEncType;       //CEDAR_SUB_ENCODE_ANSI��
    __ssa_line_descriptor_t curDescriptor;  //ssa��������
    __u32 nSpareBufSize;    //Ӧ��ָ����framebuf��ʣ�೤��
    __u8 *pCurBufPos;       //frambuf����ʼ��ַ,��Ž�������Ļ��
    __u8 *pBufStart;    //�������lbsbuf��ʼ��ַ.
    __ssa_dialogue_node_t *pCurDialogueNode;   //������ssa��Ļ��������,һ����NULL.
    __u16 style_num;    //style�ĸ���
    __u16 default_style;
    __ssa_stytle_item_t style_item[SSA_MAX_STYLE_NUM];
} __ssa_dec_info_t;


#define ISSADEC_MAX_LBSBUF_SIZE    (1024 * 10)  //lbsbufsize, ��ͬ����Ļ������,ֵ��һ��,issa���ǵ�issaͷ��Ϣ�ĳ���,���ô�һ��
#define ISSADEC_MAX_FRM_SIZE       (1024 * 1)  //һ֡�ĳ���,����issa��˵,1024�ֽ��㹻��
#define ISSADEC_MAX_ANCI_SIZE      (1024 * 1)  //����lbsbuffer��ͷ��buffer,1k�ֽ��㹻��

typedef struct tag_IssaDec
{
    ItxtDec base;
}IssaDec;

extern __s32 IssaDec_Init(IssaDec *thiz);
extern __s32 IssaDec_Exit(IssaDec *thiz);
extern IssaDec *newIssaDec(void);
extern void deleteIssaDec(IssaDec *thiz);

#endif  //_SSADEC_H_

