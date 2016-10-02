;/*
;*********************************************************************************************************
;*                                                    MELIS
;*                                    the Easy Portable/Player Develop Kits
;*                                         Melis Configuration Module
;*
;*                                    (c) Copyright 2010-2014, sunny China
;*                                             All Rights Reserved
;*
;* File    : epdk_arch_cfgs.h
;* By      : sunny
;* Version : v2.0
;* Date    : 2011-11-24
;* Descript: config arch parameters for melis.
;* Update  : date                auther      ver     notes
;*           2011-11-24 9:31:15  sunny       2.0     build the file;
;*********************************************************************************************************
;*/
    IF :LNOT::DEF:__EPDK_ARCH_CFGS_INC__
    GBLA  __EPDK_ARCH_CFGS_INC__

    include  const.inc

EPDK_CPU        equ EPDK_CPU_AC320
EPDK_ARCH       equ EPDK_ARCH_SUNV

    ENDIF
    END       ;// end of __EPDK_ARCH_CFGS_INC__
