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
#导入编译器配置
include make.cfg

#specify configuration for compiling maple.
include MAPLE.CFG

DEMOD_CONFIG_FILE_DIR = ./maple/inc/maple_config_files

ifeq ($(DEMODULATOR), DIBCOM7070)
DEMOD_CONFIG_FILE = $(DEMOD_CONFIG_FILE_DIR)/dibcom7070_dvbt.h
endif

ifeq ($(DEMODULATOR), SIANO1140)
ifeq ($(DEMODULATOR_FUNCTION), DVBT)
DEMOD_CONFIG_FILE = $(DEMOD_CONFIG_FILE_DIR)/siano1140_dvbt.h
else
ifeq ($(DEMODULATOR_FUNCTION), ISDBT)
DEMOD_CONFIG_FILE = $(DEMOD_CONFIG_FILE_DIR)/siano1140_isdbt.h
else
	@echo SIAMO1140 DEMODULATOR_FUNCTION wrong!
endif
endif
endif


ifeq ($(DEMODULATOR), SIANO2270)
DEMOD_CONFIG_FILE = $(DEMOD_CONFIG_FILE_DIR)/siano2270_isdbt.h
endif


ifeq ($(DEMODULATOR), FC8300B)
DEMOD_CONFIG_FILE = $(DEMOD_CONFIG_FILE_DIR)/fc8300b_isdbt.h
endif

ifeq ($(DEMODULATOR), DIBCOM9090)
DEMOD_CONFIG_FILE = $(DEMOD_CONFIG_FILE_DIR)/dibcom9090_dvbt.h
endif

ifeq ($(DEMODULATOR), CDTRF00)
DEMOD_CONFIG_FILE = $(DEMOD_CONFIG_FILE_DIR)/cdtrf00_isdbt.h
endif

ifeq ($(DEMODULATOR), CDTRS00)
DEMOD_CONFIG_FILE = $(DEMOD_CONFIG_FILE_DIR)/cdtrs00_dtmb.h
endif

ifeq ($(DEMODULATOR), ATBM888X)
DEMOD_CONFIG_FILE = $(DEMOD_CONFIG_FILE_DIR)/atbm888x_dtmb.h
endif

ifeq ($(DEMODULATOR), ATBM781X)
DEMOD_CONFIG_FILE = $(DEMOD_CONFIG_FILE_DIR)/atbm781x_dvbt2.h
endif

ifeq ($(DEMODULATOR), DIBCOM10096)
ifeq ($(DEMODULATOR_FUNCTION), DVBT)
DEMOD_CONFIG_FILE = $(DEMOD_CONFIG_FILE_DIR)/dibcom10096_dvbt.h
else
ifeq ($(DEMODULATOR_FUNCTION), ISDBT)
DEMOD_CONFIG_FILE = $(DEMOD_CONFIG_FILE_DIR)/dibcom10096_isdbt.h
endif
endif
endif  

ifeq ($(DEMODULATOR), MTV818)
DEMOD_CONFIG_FILE = $(DEMOD_CONFIG_FILE_DIR)/mtv818_isdbt.h
endif

ifeq ($(DEMODULATOR), SIANO3130)
DEMOD_CONFIG_FILE = $(DEMOD_CONFIG_FILE_DIR)/siano3130_isdbt.h
endif

# CMMB方案不需要tsc
ifeq ($(DEMODULATOR_FUNCTION), CMMB)
ifeq ($(DEMODULATOR), SIANO1184)
DEMOD_CONFIG_FILE = $(DEMOD_CONFIG_FILE_DIR)/siano1184_cmmb.h
endif
ifeq ($(DEMODULATOR), SIANO2180)
DEMOD_CONFIG_FILE = $(DEMOD_CONFIG_FILE_DIR)/siano2180_cmmb.h
endif
ifeq ($(DEMODULATOR), IF206)
DEMOD_CONFIG_FILE = $(DEMOD_CONFIG_FILE_DIR)/if206_cmmb.h
endif
ifeq ($(DEMODULATOR), IF238)
DEMOD_CONFIG_FILE = $(DEMOD_CONFIG_FILE_DIR)/if238_cmmb.h
endif
ifeq ($(DEMODULATOR), TP3213)
DEMOD_CONFIG_FILE = $(DEMOD_CONFIG_FILE_DIR)/tp3213_cmmb.h
endif
endif
# compile all modules of maple.
all:
	-cp $(DEMOD_CONFIG_FILE) ./maple/inc/maple_config.h

	make -C maple
	make -C frontend
	make -C demux
	make -C plugins
	

# delete object files.
clean:
	make -C maple       clean
	make -C frontend    clean
	make -C demux       clean
	make -C plugins     clean
	
	
cleanall:
	-rm $(WORKSPACEPATH)/rootfs/mod/maple.mod
	-rm $(WORKSPACEPATH)/rootfs/mod/cedar/*.*
	
	make -C maple       clean
	make -C frontend    clean
	make -C demux       clean
	make -C plugins     clean	

