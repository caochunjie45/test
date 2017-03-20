/*
*********************************************************************************************************
*											        ePOS
*						           the Easy Portable/Player Operation System
*									           PLUGIN sub-system
*
*						        (c) Copyright 2006-2007, Steven.ZGJ China
*											All	Rights Reserved
*
* File   : plugin.h
* Version: V1.0
* By     : steven.zgj
*********************************************************************************************************
*/

#ifndef _PLUGIN_H_
#define _PLUGIN_H_
#include "typedef.h"
#include "cfgs.h"


#ifdef EPDK_USED_BY_KRNL
#define __swi(x)
#endif
//**********************************************************************************************************
//* define level 0( system level)
//************************************************
// ��������

#define     PLUG_MAGIC0     'e'
#define     PLUG_MAGIC1     'P'
#define     PLUG_MAGIC2     'D'
#define     PLUG_MAGIC3     'K'
#define     PLUG_MAGIC4     '.'
#define     PLUG_MAGIC5     'p'
#define     PLUG_MAGIC6     'l'
#define     PLUG_MAGIC7     'g'


//************************************************
// �������Ͷ���
typedef struct __MP         //mp�ľ���ṹ��ģ���Լ����ж��壬��С��������ǿ�Ƶ�һ��byte���mid
{                           //mid�����ڵ��þ����module����ʱfsfc��
    __u8            mid;
    __u8            userdef;
}__mp;

typedef struct __MIF
{
    __s32          (*MInit)(void);              //��install/resetģ��ʱʹ��,��ģ����Ҫ��̨����,�����ڴ�����
    __s32          (*MExit)(void);              //��uninstallģ��ʱʹ��,��ģ���к�̨����,�����ڴ˴��ر�
    __mp           *(*MOpen)(__u8 id, __u8 mode);         //��һ��ģ����
    __s32          (*MClose)(__mp *mp);         //�ر�һ��ģ����
    __u32          (*MRead)(void *pdata, __u32 size, __u32 n, __mp *mp);                  //ģ���
    __u32          (*MWrite)(const void *pdata, __u32 size, __u32 n, __mp *mp);           //ģ��д
    __s32          (*MIoctrl)(__mp *mp, __s32 cmd, __s32 aux, void *pbuffer);             //ģ�����
} __mif_t;
typedef struct __MODS_MGSEC
{
    const char      magic[8];           // "ePDK.mod"
    __u32           version;
    __u32           type;               // 0xff: user module(midware)

  	__u32           heapaddr;           //heap��32M�ռ��ڵ�λ�ã�ͨ�������1M(31*1024*1024)
	                                    //����ַ����0����ô��ʾ���̲���Ҫ�ֲ��ѣ�������ϵͳ��
    __u32           heapsize;	        //heap�ĳ�ʼ��С
    __mif_t         mif;                //pcb���һ��������׵�ַ
} __mods_mgsec_t;


typedef struct __MODS_MCB        		//���̿��ƿ�
{
    __krnl_xcb_t    xcb;
    __u8            type;               //ϵͳģ����û�ģ��
    __mif_t         mif;                //ģ����ڣ�ģ��ı�׼������ڵ�ַ����ڴ˴�
}__mods_plg_t;

//************************************************
// ��������
/* GLOBAL VARIABLES */

//************************************************
// ��������
/* system call table */
typedef struct 
{
    __esswi esMODS_PlugIn              ;
    __esswi esMODS_PlugOut             ;
    __esswi esMODS_PlugOpen            ;
    __esswi esMODS_PlugClose           ;
} SWIHandler_PLUG_t;

#define SYSCALL_MODS(x,y) __swi(SWINO(SWINO_MODS, SWIHandler_MODS_t, y))x y

SYSCALL_MODS(__u32, esMODS_PInstall  )(const char *mfile, __u8 mode);
SYSCALL_MODS(__s32, esMODS_PUninstall)(__u8 mid);
SYSCALL_MODS(__mp*, esMODS_POpen     )(__u8 mid, __u8 mode);
SYSCALL_MODS(__s32, esMODS_PClose    )(__mp *mp);
SYSCALL_MODS(__u32, esMODS_PRead     )(void *pdata, __u32 size, __u32 n, __mp *mp);
SYSCALL_MODS(__u32, esMODS_PWrite    )(const void *pdata, __u32 size, __u32 n, __mp *mp);
SYSCALL_MODS(__s32, esMODS_PIoctrl   )(__mp *mp, __s32 cmd, __s32 aux, void *pbuffer);
//**********************************************************************************************************

//**********************************************************************************************************
//* define level 1( system level)
//************************************************
// ��������

//************************************************
// �������Ͷ���

//************************************************
// ��������

extern SWIHandler_MODS_t SWIHandler_MODS;
//************************************************
// ��������
__s32 esMODS_init	(void);
__s32 esMODS_exit  (void);


//**********************************************************************************************************

#endif  /* _PLUGIN_H_ */


