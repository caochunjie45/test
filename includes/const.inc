;/*
;*********************************************************************************************************
;*                                                    MELIS
;*                                    the Easy Portable/Player Develop Kits
;*                                               Const Variales Define
;*
;*                                    (c) Copyright 2011-2014, Sunny China
;*                                             All Rights Reserved
;*
;* File    : const.h
;* By      : Sunny
;* Version : v1.0
;* Date    : 2011-11-24
;* Descript: system use const variales define.
;* Update  : date                auther      ver     notes
;*           2011-11-24 9:36:31  Sunny       1.0     Create this file.
;*********************************************************************************************************
;*/
	IF :LNOT::DEF:__CONST_INC__
	GBLA  __CONST_INC__

EPDK_CPU_AC320              equ 0
EPDK_CPU_ARM926EJ_S         equ 0
EPDK_CPU_AC328              equ 1

EPDK_CPU_ARM920T            equ 100
EPDK_CPU_ARM922T            equ 101
EPDK_CPU_ARM940T            equ 102
EPDK_CPU_ARM946E_S          equ 103
EPDK_CPU_ARM1022E           equ 104
EPDK_CPU_ARM1026EJ_S        equ 105
EPDK_CPU_SA_110             equ 106
EPDK_CPU_XSCALE             equ 107
EPDK_CPU_ARM720T            equ 108

EPDK_OS_EPOS                equ 0
EPDK_OS_WIN2K               equ 100
EPDK_OS_WINXP               equ 101
EPDK_OS_VISTA               equ 102
EPDK_OS_LINUX               equ 103
EPDK_OS_MOSH                equ 104

EPDK_ARCH_SUNI              equ 1
EPDK_ARCH_SUNII             equ 2
EPDK_ARCH_SUNIII            equ 3
EPDK_ARCH_SUNIV             equ 4
EPDK_ARCH_SUNV              equ 5


    ENDIF   ;//__CONST_INC__

    end
