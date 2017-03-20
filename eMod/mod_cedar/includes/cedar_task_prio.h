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
* File   : cedar_task_prio.h
* Version: V1.0
* By     : Eric_wang
* Date   : 2011-3-29
* Description:
    ר������ָ��cedar�ڲ���ģ����߳����ȼ�
********************************************************************************
*/
#ifndef _CEDAR_TASK_PRIO_H_
#define _CEDAR_TASK_PRIO_H_
//#if(EPDK_ARCH == EPDK_ARCH_SUNI)
#if (defined(EPDK_ARCH_SUNI) && (EPDK_ARCH==EPDK_ARCH_SUNI))

#define APLY_MAINTASK_PRIO                  (0x38)  //level 1
#define VPLY_MAINTASK_PRIO                  (0x39)  //level 1
#define LPLY_MAINTASK_PRIO                  (0x3A)  //level 1
#define ARDR_MAINTASK_PRIO                  (0x58)  //level 2
#define AVS_MAINTASK_PRIO                   (0x59)  //level 2
#define ADRV_MAINTASK_PRIO                  (0x5A)  //level 2
#define ADEC_MAINTASK_PRIO                  (0x5B)  //level 2
#define VDRV_MAINTASK_PRIO                  (0x5C)  //level 3
#define VDEC_MAINTASK_PRIO                  (0x5D)  //level 3
#define CEDAR_MAINTASK_PRIO                 (0x78)  //level 3
#define PARSER_MAINTASK_PRIO                (0x79)  //level 3
#define PARSER_AUDTASK_PRIO                 (0x79)  //level 3, same as parser main task
#define PARSER_LRCTASK_PRIO                 (0x7A)  //level 3
#define LDEC_RECTASK_PRIO                   (0x7B)  //level 3, same as audio file parser
#define LDEC_MAINTASK_PRIO                  (0x7C)  //level 3
//#elif(EPDK_ARCH == EPDK_ARCH_SUNII)
#elif (defined(EPDK_ARCH_SUNII) && (EPDK_ARCH==EPDK_ARCH_SUNII))
#define APLY_MAINTASK_PRIO                  (0x38)  //level 1
#define VPLY_MAINTASK_PRIO                  (0x39)  //level 1
#define LPLY_MAINTASK_PRIO                  (0x3A)  //level 1
#define VDRV_MAINTASK_PRIO                  (0x3B)  //level 1
#define ARDR_MAINTASK_PRIO                  (0x58)  //level 2
#define AVS_MAINTASK_PRIO                   (0x59)  //level 2
#define PARSER_MAINTASK_PRIO                (0x78)  //level 3
//����adrv���ȼ�,�������ʼ����ʱ��������La_Isla_Bonita.avi����Ƶ��ʼ����ʱ����Ƶ���벻��ʱ��ɵ���������.
//��ʱ�ָ���ԭ����
#define ADEC_MAINTASK_PRIO                  (0x79)  //level 3
#define VDEC_MAINTASK_PRIO                  (0x7A)  //level 3
#define CEDAR_MAINTASK_PRIO                 (0x7B)  //level 3   //��ΪKRNL_priolevel1��������ȼ�
#define ADRV_MAINTASK_PRIO                  (0x7C)  //level 3
//#define ADRV_MAINTASK_PRIO                  (0x79)  //level 3
//#define ADEC_MAINTASK_PRIO                  (0x7A)  //level 3
//#define VDEC_MAINTASK_PRIO                  (0x7B)  //level 3
//#define CEDAR_MAINTASK_PRIO                 (0x7C)  //level 3   //��ΪKRNL_priolevel1��������ȼ�
#define PARSER_LRCTASK_PRIO                 (0x7D)  //level 3
#define PARSER_AUDTASK_PRIO                 (0x7E)  //level 3, audio file parser
#define LDEC_RECTASK_PRIO                   (0x7E)  //level 3, same as audio file parser
#define LDEC_MAINTASK_PRIO                  (0x7F)  //level 3, �����Ļ�����߳̽�����������ȼ�, ������Ļ������ܻ����,ֱ����level3.
#elif (defined(EPDK_ARCH_SUNIII) && (EPDK_ARCH==EPDK_ARCH_SUNIII))
#define APLY_MAINTASK_PRIO                  (0x38)  //level 1
#define VPLY_MAINTASK_PRIO                  (0x39)  //level 1
#define LPLY_MAINTASK_PRIO                  (0x3A)  //level 1
#define VDRV_MAINTASK_PRIO                  (0x3B)  //level 1
#define ARDR_MAINTASK_PRIO                  (0x58)  //level 2
#define AVS_MAINTASK_PRIO                   (0x59)  //level 2
#define PARSER_MAINTASK_PRIO                (0x78)  //level 3
#define ADEC_MAINTASK_PRIO                  (0x79)  //level 3
#define VDEC_MAINTASK_PRIO                  (0x7A)  //level 3
#define CEDAR_MAINTASK_PRIO                 (0x7B)  //level 3
#define ADRV_MAINTASK_PRIO                  (0x7C)  //level 3
#define PARSER_LRCTASK_PRIO                 (0x7D)  //level 3
#define PARSER_AUDTASK_PRIO                 (0x7E)  //level 3, audio file parser
#define LDEC_RECTASK_PRIO                   (0x7E)  //level 3, same as audio file parser
#define LDEC_MAINTASK_PRIO                  (0x7F)  //level 3
#elif (defined(EPDK_ARCH_SUNIV) && (EPDK_ARCH==EPDK_ARCH_SUNIV))
#define DEMOD_TSRX_TASK_PRIO                (0x05)  //������ȼ�,cfgs.h, 1619,�����Ӧ�ⲿdemodоƬ�����Ķ���Ϣ�ж�.5ms֮�ڱ�����Ӧ,���ر�����������ȼ�
#define APLY_MAINTASK_PRIO                  (0x38)  //level 1
#define VPLY_MAINTASK_PRIO                  (0x39)  //level 1
#define LPLY_MAINTASK_PRIO                  (0x3A)  //level 1
#define VDRV_MAINTASK_PRIO                  (0x3B)  //level 1
#define ARDR_MAINTASK_PRIO                  (0x58)  //level 2
#define AVS_MAINTASK_PRIO                   (0x59)  //level 2
#define PARSER_MAINTASK_PRIO                (0x78)  //level 3
#define ADEC_MAINTASK_PRIO                  (0x79)  //level 3
#define VDEC_MAINTASK_PRIO                  (0x7A)  //level 3
#define CEDAR_MAINTASK_PRIO                 (0x7B)  //level 3
#define ADRV_MAINTASK_PRIO                  (0x7C)  //level 3
#define PARSER_LRCTASK_PRIO                 (0x7D)  //level 3
#define PARSER_AUDTASK_PRIO                 (0x7E)  //level 3, audio file parser
#define LDEC_RECTASK_PRIO                   (0x7E)  //level 3, same as audio file parser
#define LDEC_MAINTASK_PRIO                  (0x7F)  //level 3
#else
//#define TSC_MAINTASK_PRIO                   (0x05)  //yefl 20150804
#define APLY_MAINTASK_PRIO                  (0x38)  //level 1
#define VPLY_MAINTASK_PRIO                  (0x39)  //level 1
#define LPLY_MAINTASK_PRIO                  (0x3A)  //level 1
#define VDRV_MAINTASK_PRIO                  (0x3B)  //level 1
#define ARDR_MAINTASK_PRIO                  (0x58)  //level 2
#define AVS_MAINTASK_PRIO                   (0x59)  //level 2
#define PARSER_MAINTASK_PRIO                (0x78)  //level 3
#define ADEC_MAINTASK_PRIO                  (0x79)  //level 3
#define VDEC_MAINTASK_PRIO                  (0x7A)  //level 3
#define CEDAR_MAINTASK_PRIO                 (0x7B)  //level 3
#define ADRV_MAINTASK_PRIO                  (0x7C)  //level 3
#define PARSER_LRCTASK_PRIO                 (0x7D)  //level 3
#define PARSER_AUDTASK_PRIO                 (0x7E)  //level 3, audio file parser
#define LDEC_RECTASK_PRIO                   (0x7E)  //level 3, same as audio file parser
#define LDEC_MAINTASK_PRIO                  (0x7F)  //level 3
#endif

#endif  /* _CEDAR_TASK_PRIO_H_ */

