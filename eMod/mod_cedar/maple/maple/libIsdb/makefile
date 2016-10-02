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
SRCCS=isdb.c isdb_context.c isdb_eit_parser.c isdb_eit_pool.c $(CEDARROOT)/ToolKit/dlist.c

# get list of object files.
OBJS=$(SRCCS:.c=.o) $(SRCSS:.s=.o)


# generate object files.
all:$(OBJS)
	$(AR) $(ARFLAGS) $(TARGET) $(filter %.o ,$+)
	@echo ----------------------------
	@echo libIsdb.a make finish
	@echo ----------------------------

# delete object files.
clean:
	-rm $(OBJS) $(LOCALTARGET)
#	-rm $(wildcard *.o)
