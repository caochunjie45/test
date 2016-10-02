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

ifeq ($(DEMUX), TS)

all:
	make -C tsDemux

clean:
	make -C tsDemux clean

else

all:
	make -C cmmbDemux

clean:
	make -C cmmbDemux clean

endif
