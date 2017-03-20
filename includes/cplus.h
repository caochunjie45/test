/*
************************************************************************************************************************
*											        ePDK
*						            the Easy Portable/Player Developent Kits
*									           ePDK interface
*
*                                       (c) Copyright 2009-2010, kevin.z
*                                            All Rights Reserved
*
* File    : cplus.h
* By      : kevin
* Version : V0.1
* Date    : 2010-4-3 19:47
* Descript: ��ͷ�ļ���������c++����ؿ⺯������֧��c++�ı��롣
************************************************************************************************************************
*/
#ifndef _EPDK_CPLUS_H_
#define _EPDK_CPLUS_H_

#include "typedef.h"
#ifndef SIM_PC_WIN

#ifdef __cplusplus
#include <new>


extern "C" {

//initilize the global objects for c++
void __cpp_initialize__aeabi_(void);
//register exit fuction
int __cxa_atexit (void (*func) (void *), void *arg, void *d);
int __aeabi_atexit (void *arg, void (*func) (void *), void *d);
//destroy golbal objects for c++
void __cxa_finalize(void *d);

}
#endif  //__cplusplus

#endif  //SIM_PC_WIN

#endif //_EPDK_CPLUS_H_

