#/*
#*********************************************************************************************************
#*                                                    MELIS
#*                                    the Easy Portable/Player Develop Kits
#*                                               makefile Module
#*
#*                                  (c) Copyright 2010-2014, Calder.Chen China
#*                                             All Rights Reserved
#*
#* File    : makefile
#* By      : CC
#* Version : v1.0
#* Date    : 2011-01-01 08:30
#* Descript:
#* Update  :    date            auther          ver         notes
#*           2011-01-01           CC            1.0     create this file.
#*********************************************************************************************************
#*/
ROOT    = .
SDKROOT = $(ROOT)/../../..
CEDARROOT = $(SDKROOT)/eMod/mod_cedar

#µº»Î±‡“Î∆˜≈‰÷√
include $(CEDARROOT)/MAPLE.CFG


ifeq ($(DEMODULATOR), ATBM781X)

all:
	make -C ldec_itxt

clean:
	make -C ldec_itxt clean

all:
	make -C subtitle_plybk

clean:
	make -C subtitle_plybk clean

endif

ifeq ($(DEMODULATOR), CDTRF00)

all:
	make -C ldec_itxt

clean:
	make -C ldec_itxt clean

all:
	make -C subtitle_plybk

clean:
	make -C subtitle_plybk clean

endif 

ifeq ($(DEMODULATOR), FC8300B)

all:
	make -C ldec_itxt

clean:
	make -C ldec_itxt clean

all:
	make -C subtitle_plybk

clean:
	make -C subtitle_plybk clean

endif 
 
