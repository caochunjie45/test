;/*
;*********************************************************************************************************
;*                                                    MELIS
;*                                    the Easy Portable/Player Develop Kits
;*                                             	Standby system
;*
;*                                    (c) Copyright 2012-2016, Sunny China
;*                                             All Rights Reserved
;*
;* File    : standby_delay.s
;* By      : Sunny
;* Version : v1.0
;* Date    : 2012-2-22
;* Descript: delay operations for standby.
;* Update  : date                auther      ver     notes
;*           2012-2-22 14:40:42	Sunny       1.0     Create this file.
;*********************************************************************************************************
;*/
	EXPORT  standby_delay
    
    AREA STANDBY_DELAY, CODE, READONLY
    
standby_delay
	subs    r0, r0, #1
	bhi	    standby_delay
	mov     pc, lr

	END
	