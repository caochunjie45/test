/*
********************************************************************************
*                                    eMOD
*                   the Easy Portable/Player Develop Kits
*                               mod_cedar sub-system
*                          (module name, e.g.mpeg4 decoder plug-in) module
*
*          (c) Copyright 2010-2012, Allwinner Microelectronic Co., Ltd.
*                              All Rights Reserved
*
* File   : ldec_lbs.h
* Version: V1.0
* By     : Eric_wang
* Date   : 2011-10-8
* Description:
    lbs buffer�Ĺ�����.�����г�.
********************************************************************************
*/
#ifndef _LDEC_LBS_H_
#define _LDEC_LBS_H_

#define LDEC_MAX_TXT_DATA_NUM      (64) //lbs�ֿ�ĸ���

#define LDEC_BUF_PRIVATE_DATA_LEN (16)

typedef struct LDEC_BUFFER_CTRL
{
    __u8   *startAddr;  //���ݿ����ʼ��ַ
    __u8    coverBufEnd;    //��ʾ�Ƿ�ռ����buf�����һ���ֽ�. 1:ռ��;0:û��ռ��.��������ݿ�����ռ�����һ���ֽ�,��ôҲ����1��.
    __u8    hasPtsFlag; //�Ƿ����pts.
    __u16   dataType;   //LYRIC_SUB_ENCODE_UTF8��,��ʾ������Ļ�����ݵı����ʽ.�����Ļ�����ı�,һ������ı����ַ���������;�����ͼ��,һ�����LYRIC_SUB_ENCODE_BITMAP
    __u32   ptsVal;     //����Ļ��Ŀ��pts
    __u32   duration;   //����Ļ��Ŀ�ĳ���ʱ��
    __u32   dataLen;    //��chunk���ֽڳ���
    __s8    lbsChgFlg;  //��ʾ�Ƿ���Ļ��,��Ȼ��Ҫ,����issa��˵,���ʾ�����ݿ��ǲ���ssa��ͷ��Ϣ. ������Ļ���Ͳ���Ҫ. ������Ļ��������ģ�����ldec_itxt����.ldec_itxt�Լ���������������.
    __u8    privateData[LDEC_BUF_PRIVATE_DATA_LEN];
}__ldec_buffer_ctrl_t;


typedef struct LDEC_DATA_BUF
{
    __u8    *ldecBuf;   //lbsbuf����ʼ��ַ
    __u8    *ldecBufEnd;    //ָ��lbsbuf�����һ���ֽ�
    __u8    *readAddr;  //decode��Ҫ�����ֽ�
    __u8    *writeAddr; //ָ��Ҫд���ֽ�
    __ldec_buffer_ctrl_t ldecBufCrl[LDEC_MAX_TXT_DATA_NUM]; //lbs�Ŀ����
    __u16   ldecReadIdx;    //decoder��Ҫ���ĸ�lbs��
    __u16   ldecWriteIdx;   //��Ҫд��lbs������
    __u16   ldecIdxSize;    //��Ч��lbs�������
    __u32   validSize;  //��Ч���ֽ���

}__ldec_lbs_buf_t;


extern void  ldec_update_read_pointer(__ldec_lbs_buf_t *ldecLbsBuf, __u32 uread_byte);
extern __u8 *ldec_process_chunk_cover_bufEnd(__ldec_lbs_buf_t *pLbsBufManager, __u8 **ppanciDataBuf, __u32 *pnanciDataBufSize);

#endif  /* _LDEC_LBS_H_ */

