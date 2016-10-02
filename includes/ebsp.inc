;/*
;*********************************************************************************************************
;*                                                    MELIS
;*                                    the Easy Portable/Player Develop Kits
;*                                             BSP Modules Header
;*
;*                                    (c) Copyright 2011-2014, Sunny China
;*                                             All Rights Reserved
;*
;* File    : ebsp.inc
;* By      : Sunny
;* Version : v1.0
;* Date    : 2011-11-18
;* Descript: BSP modules header file.
;* Update  : date                auther      ver     notes
;*           2011-11-18 9:25:33  Sunny       1.0     Create this file.
;*********************************************************************************************************
;*/


    if :LNOT::DEF:__EBSP_INC__
	GBLA  __EBSP_INC__

    include  cfgs.inc
    
    IF EPDK_ARCH = EPDK_ARCH_SUNIII
	    include arch\suniii\suniii.inc
    ENDIF

    IF EPDK_ARCH = EPDK_ARCH_SUNIV
	    include arch\suniii\suniv.inc
    ENDIF
    
    IF EPDK_ARCH = EPDK_ARCH_SUNV
	    include arch\sunv\sunv.inc
    ENDIF
    
    ENDIF

    end         ;//__EBSP_INC__

