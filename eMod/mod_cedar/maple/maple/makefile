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

# configuration for compilling.
include make.cfg

# get list of source files.
SRCCS   = $(wildcard *.c) $(CEDARROOT)/ToolKit/semaphore_obj.c

ifeq ($(SCANLIB), DVBT)
SRCCS  := $(SRCCS) libDvb/dvb_support.c
SCANLIB_PATH = libDvb
endif
ifeq ($(SCANLIB), ISDBT)
SRCCS  := $(SRCCS) libIsdb/isdb_support.c
SCANLIB_PATH = libIsdb
endif
ifeq ($(SCANLIB), DTMB)
SRCCS  := $(SRCCS) libDtmb/dtmb_support.c
SCANLIB_PATH = libDtmb
endif
ifeq ($(SCANLIB), ATSC)
SRCCS  := $(SRCCS) libAtsc/atsc_support.c
SCANLIB_PATH = libAtsc
endif
ifeq ($(SCANLIB), CMMB)
SRCCS  := $(SRCCS) libCmmb/cmmb_support.c
SCANLIB_PATH = libCmmb
endif
ifeq ($(SCANLIB), DVBT2)
SRCCS  := $(SRCCS) libDvbt2/dvbt2_support.c
SCANLIB_PATH = libDvbt2
endif

# get list of object files.
OBJS=$(SRCCS:.c=.o) $(SRCSS:.s=.o)


# generate object files.
all:$(LOCALTARGET)
	$(LOAD) $(LDFLAGS) $(TARGET) $(LOCALTARGET)
	@echo ----------------------------
	@echo maple make finish
	@echo ----------------------------
	

# generate local target.
$(LOCALTARGET):$(OBJS)
	make -C $(SCANLIB_PATH)
	$(LINK) $(LKFLAGS) -o $@ $(filter %.o ,$+) $(LIBS)
	
# generate scan library
#lib:
#	make -C $(SCANLIB_PATH)

# delete object files.
clean:
	-rm $(OBJS) $(LOCALTARGET)
	make -C $(SCANLIB_PATH) clean
	
#clean_lib:
#	make -C $(SCANLIB_PATH) clean
	
#clean_all:
#	make -C libDvb  clean
#	make -C libIsdb clean
#	make -C libAtsc clean
#	make -C libDtmb clean
#	make -C libCmmb clean
#	make -C libDvbt2 clean
#	rm $(OBJS) $(LOCALTARGET)
