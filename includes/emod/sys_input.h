/*
*********************************************************************************************************
*                                                    eMOD
*                                         the melis Operation System
*                                               input sub-system
*                                        input sub-system internal head file
*
*                                    (c) Copyright 2010-2012, sunny China
*                                              All Rights Reserved
*
* File   : sys_input.h
* Version: V1.0
* By     : Sunny
* Date   : 2010-10-27 8:47:23
* Note   : input system extern service for input driver and event user.
*********************************************************************************************************
*/
#ifndef __SYS_INPUT_H__
#define __SYS_INPUT_H__

#include "typedef.h"
#include "cfgs.h"

#ifdef EPDK_USED_BY_KRNL
#define __swi(x)
#endif

/* The class name of INPUT devices */
#define INPUT_CLASS_NAME           "INPUT"

/* INPUT logical devices name, use for graber logical device */
#define INPUT_LKEYBOARD_DEV_NAME   "LKeyBoardDev"
#define INPUT_LMOUSE_DEV_NAME      "LMouseDev"
#define INPUT_LTS_DEV_NAME         "LTSDev"

/* input system limits */
#define INPUT_LOGICALDEV_MAX        16   /*  max logical device number              */
#define INPUT_CHILDREN_MAX          8    /*  max children number of logical device  */
#define INPUT_EV_BUFFER_MAXLEN      256  /*  max event buffer length                */
#define INPUT_MAX_EVENT_NR          64   /*  max event number                       */

/* input device classes */
#define INPUT_KEYBOARD_CLASS  0    /* keyboard class device   */
#define INPUT_MOUSE_CLASS     1    /* mouse class device      */
#define INPUT_TS_CLASS        2    /* touchpanel class device */

/* basic bit operations */
#define INPUT_BITS_PER_BYTE		    8
#define INPUT_DIV_ROUND_UP(n,d)     (((n) + (d) - 1) / (d))
#define INPUT_BITS_PER_LONG         (sizeof(long) * INPUT_BITS_PER_BYTE)
#define INPUT_BIT(nr)			    (1UL << (nr))
#define INPUT_BIT_MASK(nr)		    (1UL << ((nr) % INPUT_BITS_PER_LONG))
#define INPUT_BIT_WORD(nr)		    ((nr) / INPUT_BITS_PER_LONG)
#define INPUT_BITS_TO_LONGS(nr)	    INPUT_DIV_ROUND_UP(nr, INPUT_BITS_PER_LONG)

/* logical device ioctl command */
#define INPUT_SET_REP_PERIOD        0x100

typedef struct __INPUT_PID
{
	__u16 product;
	__u16 version;
} __input_pid_t;

typedef struct __INPUT_DEV
{
    /* ����4����ԱΪע��ͨ���豸���� */
	const char      *classname;
	const char      *name;
    __dev_devop_t   *ops;
    void            *parg;

    /* ָ��ͨ���豸 */
    __hdle          devnode;
    
    /* ���߼��豸�������豸�ӱ������е�index��� */
    unsigned char   seq;
    
    /* ָ��������豸��Ӧ���߼��豸 */
    __hdle          ldevp;
    
    /* �豸���ͣ��������������豸ע��ʱƥ����Ӧ���߼������豸 */
    __u32           device_class;
    
    /* ���������豸�������Ϣ */
	__input_pid_t     id;

    /* �豸֧�ֵ��¼�bitmap����ӦλΪ1��ʾ֧�ָ����¼� */
	unsigned long evbit[INPUT_BITS_TO_LONGS(EV_CNT)];
    /* �豸֧�ֵİ���bitmap����ӦλΪ1��ʾ֧�ָð��� */
	unsigned long keybit[INPUT_BITS_TO_LONGS(KEY_CNT)];
    /* �豸֧�ֵ����λ�����bitmap����x��y��z����ӦλΪ1��ʾ֧�ָ������λ�� */
	unsigned long relbit[INPUT_BITS_TO_LONGS(REL_CNT)];
    /* �豸֧�ֵľ���λ�����bitmap����x��y��volume����ӦλΪ1��ʾ֧�ָ��־���λ�� */
	unsigned long absbit[INPUT_BITS_TO_LONGS(ABS_CNT)];
    /* �豸֧�ֵ�led������bitmap����capslock�ơ�scroll�ƣ���ӦλΪ1��ʾ֧�ָ��ֵ� */
	unsigned long ledbit[INPUT_BITS_TO_LONGS(LED_CNT)];
    /* �豸֧�ֵ���������bitmap����click��bell����ӦλΪ1��ʾ֧�ָ������� */
	unsigned long sndbit[INPUT_BITS_TO_LONGS(SND_CNT)];
    /* �豸֧�ֵ�����������bitmap�����ֱ��𶯣���ӦλΪ1��ʾ֧�ָ��������� */
	unsigned long ffbit[INPUT_BITS_TO_LONGS(FF_CNT)];

    /* �ظ����� */
	unsigned int    repeat_key;
	
    /* �ظ�����timer */
	__krnl_stmr_t   *timer;

    /* ��ŵ�ǰ�ľ���ֵ����һ�ν�������������ο� */
	int abs[ABS_MAX + 1];
	
    /*  ����ظ������ĸ��׶�ʱ���� */
	int rep[REP_MAX + 1];

    /* key/led/snd״̬bitmap������check��Ӧcode��ֵ�Ƿ�ת */
	unsigned long key[INPUT_BITS_TO_LONGS(KEY_CNT)];
	unsigned long led[INPUT_BITS_TO_LONGS(LED_CNT)];
	unsigned long snd[INPUT_BITS_TO_LONGS(SND_CNT)];

    /* ����У������ֵ����Ϣ���� */
	int absmax[ABS_MAX + 1];
	int absmin[ABS_MAX + 1];
    
    /* ��������Ĳο�ֵ */
	int absfuzz[ABS_MAX + 1];
    
    /* �����¼������� */
	int (*event)(struct __INPUT_DEV *dev, __u32 type, __u32 code, int value);
	
    /* �����¼����������յ�EV_SYN���ת�Ƶ�ѭ������ */
    unsigned int evbuf[INPUT_EV_BUFFER_MAXLEN];
    /*  �����¼���ǰָ�� */
    int ev_pos;
    
} __input_dev_t;


typedef struct __INPUT_EVENT
{
    __u32       type;           /*  �¼����ͣ�key/rel/abs...    */
    __u32       code;           /*  �¼�����                    */
    __s32       value;          /*  �¼�ֵ                      */
} __input_event_t;


typedef struct __INPUT_EVENT_PACKET
{
    void           *pGrabArg;               /*  ע��ص�����ʱ�����˽�о��*/
    __u32           ldev_id;                /*  �߼��豸ID                  */
    __u32           pdev_id;                /*  �����豸ID                  */
    __s32           event_cnt;              /*  �¼��ĸ���                  */
    __input_event_t events[INPUT_MAX_EVENT_NR];/*  �¼�������               */
} __input_event_packet_t;


//************************************************
// ��������
/* GLOBAL VARIABLES */


//************************************************
// ��������
/* system call table */
typedef struct 
{
    __pSWI_t esINPUT_RegDev      ;
    __pSWI_t esINPUT_UnregDev    ;
    __pSWI_t esINPUT_LdevGrab    ;
    __pSWI_t esINPUT_LdevRelease ;
    __pSWI_t esINPUT_SendEvent   ;
    __pSWI_t esINPUT_LdevFeedback;
    __pSWI_t esINPUT_GetLdevID   ;
    __pSWI_t esINPUT_LdevCtl     ;
} SWIHandler_INPUT_t;

#ifndef SIM_PC_WIN
#define SYSCALL_INPUT(x,y) __swi(SWINO(SWINO_INPUT, SWIHandler_INPUT_t, y))x y
#else
#define SYSCALL_INPUT(x,y) x y
#endif

SYSCALL_INPUT(__s32 ,  esINPUT_RegDev      )(__input_dev_t *dev);
SYSCALL_INPUT(__s32 ,  esINPUT_UnregDev    )(__input_dev_t *dev);
SYSCALL_INPUT(__hdle,  esINPUT_LdevGrab    )(char *ldev, __pCBK_t callback, void *pArg, __s32 aux);
SYSCALL_INPUT(__s32 ,  esINPUT_LdevRelease )(__hdle graber);
SYSCALL_INPUT(__s32 ,  esINPUT_SendEvent   )(__input_dev_t *dev, __u32 type, __u32 code, int value);
SYSCALL_INPUT(__s32 ,  esINPUT_LdevFeedback)(__hdle graber, __input_event_packet_t *packet);
SYSCALL_INPUT(__s32 ,  esINPUT_GetLdevID   )(__hdle graber);
SYSCALL_INPUT(__s32 ,  esINPUT_LdevCtl     )(__s32 LdevId, __s32 cmd, __s32 aux, void *pBuffer);

//************************************************
// �������Ͷ���


//************************************************
// ��������
extern SWIHandler_INPUT_t SWIHandler_INPUT;

//************************************************
// ��������
__s32 INPUT_Init(void);
__s32 INPUT_Exit(void);

//**********************************************************************************************************

#endif  /* __SYS_INPUT_H__ */
