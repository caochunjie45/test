/*
**************************************************************************************************************
*											         ePDK
*						            the Easy Portable/Player Develop Kits
*									           desktop system 
*
*						        	 (c) Copyright 2007-2010, ANDY, China
*											 All Rights Reserved
*
* File    	: common.h
* By      	: Bayden.chen
* Func		: 
* Version	: v1.0
* ============================================================================================================
* 2011-05-06  create this file
**************************************************************************************************************
*/

#ifndef __MOVIE_COMMON_H__
#define  __MOVIE_COMMON_H__

#define NAME_MOVIE_BKPOINT				   "MOVIEINF"	

#define MOVIE_ENABLE_BREAKPLAY    0//�������movieʱ�����Ի�����ʾ�Ƿ�ϵ㲥��
#define MOVIE_ROBIN_PEND __movie_robin_pend();

#define MOVIE_ROBIN_ACCEPT __movie_robin_accept();

#define MOVIE_ROBIN_POST __movie_robin_post();

__s32 __movie_robin_sem_init(void);

__s32 __movie_robin_sem_deinit(void);

__s32 __movie_robin_pend(void);

__s32 __movie_robin_accept(void);

__s32 __movie_robin_post(void);
__s32  movie_cmd2parent(H_WIN hwin, __s32 id, __s32 data2, __s32 reserved);

__s32  movie_cmd2parent_syn(H_WIN hwin, __s32 id, __s32 data2, __s32 reserved);

//�ȴ�cedarΪplay��״̬������ڼ䷢���˴��󣬻��߳�ʱ��������󲢷���EPDK_FAIL
//����ɹ��򷵻�EDPK_OK��
__s32 movie_wait_for_in_cedar_play_sta(void);

#endif

