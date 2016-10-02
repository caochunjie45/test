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

ifeq ($(DEMODULATOR), DIBCOM7070)

all:
	make -C demod_dibcom_7070
	make -C hw_tsc

clean:
	make -C demod_dibcom_7070 clean
	make -C hw_tsc clean

endif

ifeq ($(DEMODULATOR), DIBCOM9090)

all:
	make -C demod_dibcom_9090
	make -C hw_tsc

clean:
	make -C demod_dibcom_9090 clean
	make -C hw_tsc clean

endif 

ifeq ($(DEMODULATOR), CDTRF00)

all:
	make -C demod_cdtrf00
	make -C hw_tsc

clean:
	make -C demod_cdtrf00 clean
	make -C hw_tsc clean

endif 
ifeq ($(DEMODULATOR), CDTRS00)

all:
	make -C demod_cdtrs00
	make -C hw_tsc

clean:
	make -C demod_cdtrs00 clean
	make -C hw_tsc clean

endif 
ifeq ($(DEMODULATOR), ATBM888X)

all:
	make -C demod_atbm888x
	make -C hw_tsc

clean:
	make -C demod_atbm888x clean
	make -C hw_tsc clean

endif

ifeq ($(DEMODULATOR), ATBM781X)

all:
	make -C demod_atbm781x
	make -C hw_tsc

clean:
	make -C demod_atbm781x clean
	make -C hw_tsc clean

endif  

ifeq ($(DEMODULATOR), SIANO1140)

all:
	make -C demod_siano_1140
	make -C hw_tsc

clean:
	make -C demod_siano_1140 clean
	make -C hw_tsc clean

endif

ifeq ($(DEMODULATOR), SIANO2270)

all:
	make -C demod_siano_2270
	make -C hw_tsc

clean:
	make -C demod_siano_2270 clean
	make -C hw_tsc clean

endif

ifeq ($(DEMODULATOR), SIANO1184)

all:
	make -C demod_siano_1184
	make -C drv_siano_1184

clean:
	make -C demod_siano_1184 clean
	make -C drv_siano_1184 clean

endif

ifeq ($(DEMODULATOR), DIBCOM10096)

all:
	make -C demod_dibcom_10096
	make -C hw_tsc

clean:
	make -C demod_dibcom_10096 clean
	make -C hw_tsc clean

endif

ifeq ($(DEMODULATOR), MTV818)

all:
	make -C mtv818 
	make -C sw_tsc

clean:
	make -C mtv818 clean
	make -C sw_tsc clean

endif

ifeq ($(DEMODULATOR), IF206)

all:
	make -C demod_if_206
	make -C drv_if_206

clean:
	make -C demod_if_206 clean
	make -C drv_if_206 clean

endif

ifeq ($(DEMODULATOR), IF238)

all:
	make -C demod_if_238
	make -C drv_if_238

clean:
	make -C demod_if_238 clean
	make -C drv_if_238 clean

endif

ifeq ($(DEMODULATOR), SIANO2180)

all:
	make -C demod_siano_2180
	make -C drv_siano_2180

clean:
	make -C demod_siano_2180 clean
	make -C drv_siano_2180 clean

endif


ifeq ($(DEMODULATOR), TP3213)

all:
	make -C demod_tp3213
	make -C drv_tp3213

clean:
	make -C demod_tp3213 clean
	make -C drv_tp3213 clean

endif

ifeq ($(DEMODULATOR), SIANO3130)

all:
	make -C demod_siano_3130 
	make -C sw_tsc

clean:
	make -C demod_siano_3130 clean
	make -C sw_tsc clean

endif


ifeq ($(DEMODULATOR), FC8300B)

all:
	make -C demod_isdb8300b 
	make -C hw_tsc

clean:
	make -C demod_isdb8300b clean
	make -C hw_tsc clean

endif