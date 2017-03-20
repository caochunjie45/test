/*
*********************************************************************************************************
*                                                    eMOD
*                                   the Easy Portable/Player Operation System
*                                              mod_mmp sub-system
*
*                                    (c) Copyright 2008-2009, Kevin.Z China
*                                              All Rights Reserved
*
* File   : cedar_fs.h
* Version: V1.0
* By     : kevin.z
* Date   : 2009-2-2 8:37
    cedar��ϵ���ļ�������API.
    cedar�ڲ�����ģ�鶼�������API�����ļ�������ֱ�ӵ���ϵͳ��fopen�Ⱥ�����
    
    ǿ�ƹ涨:
    �����ʹ��cedar_fs.h�ṩ�ĺ�����������ģ���ʱ�ȵ���
    CEDAR_InitHeap();
    CEDAR_InitPhyHeap();
    ��ʼ��cedar_common���ڴ������.��Ϊcedar_fs�ĺ���Ĭ��ʹ��cedar_common�ṩ��
    �ڴ��������.
*********************************************************************************************************
*/
#ifndef _CEDAR_FS_H_
#define _CEDAR_FS_H_

//#include "cedar_malloc.h"
//#include "mod_cedar_i.h"

/* cedar fs read mode */
//#define CEDAR_FS_WORK_MODE_BYTEALIGN            0   // read align by byte
//#define CEDAR_FS_WORK_MODE_SECTALIGN            1   // read align by sector
//#define CEDAR_FS_WORK_MODE_USER_DEFINED         2   // user defined read,ĿǰĬ��no cache
//#define CEDAR_FS_WORK_MODE_NO_CACHE             3   // use fread of fs��no cache
typedef enum tag_CEDAR_FS_WORK_MODE
{
    CEDAR_FS_WORK_MODE_BYTEALIGN    = 0,   // read align by byte
    CEDAR_FS_WORK_MODE_SECTALIGN    = 1,   // read align by sector
    CEDAR_FS_WORK_MODE_USER_DEFINED = 2,   // user defined read,�û��Լ����������ȡ�ļ�.ĿǰĬ��no cache,�����������������Ӳ���ϵ��ļ������ͣ�������ڴ��ȡ�ȣ����������ļ��������ļ������������ļ�,��Ҫ��fopen()�Ⱥ���
    CEDAR_FS_WORK_MODE_NO_CACHE     = 3,   // use fread of fs��no cache
    CEDAR_FS_WORK_MODE_HERBCACHE  = 4,    //�ڲ����߳�,��Ҫ����¼��д.
}CedarFSWorkMode;

//extern __s32    cedar_fs_work_mode;    // cedar fs work mode
//extern __s32    CEDAR_FS_CACHE_SIZE;
//extern __s32    CEDAR_FS_CACHE_BITS;

extern FILE     *CEDAR_fopen(const char *path, const char *mode);
extern int      CEDAR_fclose(FILE *fp);
extern int      CEDAR_fread(void *buf, int size, int count, FILE *fp);
extern int      CEDAR_fwrite(void *buf, int size, int count, FILE *fp);
extern int      CEDAR_fseek(FILE *fp, __s64 offset, int origin);
extern __s64    CEDAR_ftell(FILE *fp);
extern int      CEDAR_FIOCTRL(FILE *fp, __s32 Cmd, __s32 Aux, void *pBuffer);
extern __u32    CEDAR_fsize(FILE *fp);      //����filesize�ĺ���

//���º�������cedarfs�����ԣ�������newCedarFs֮ǰ(������CEDAR_fopen֮ǰ)ȫ�����úá�����CedarFS֮���ٵ�����3��������������
extern __s32    CEDAR_fs_set_workmod(__s32 mode);
extern __s32    CEDAR_fs_get_workmod(void);
extern int      CEDAR_fs_SetCacheSize(int cache_size,int mode);  //������mode = CEDAR_FS_WORK_MODE_SECTALIGN����cache��ģʽ
extern void     CEDAR_fs_set_mid(__u32 mid); //��Ϊfs buffer modeҪ���̣߳� epdkϵͳ�£����̵߳ø�һ��module id.
//�����USRDEFģʽ��������__cedar_usr_file_op_t
extern void     CEDAR_ufinit(void *fop);    //������CEDAR_FS_WORK_MODE_USER_DEFINED����ģʽ�����úñ�Ҫ�����ݽṹfop = __cedar_usr_file_op_t g_cedar_usr_fop;�ڴ��ļ�ʱʹ��

#endif //_CEDAR_FS_H_

