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


#从所有子目录中得到源代码的列表
SRCCS=$(foreach dir,$(SRCDIRS),$(wildcard $(dir)/*.c))
SRCSS=$(foreach dir,$(SRCDIRS),$(wildcard $(dir)/*.s))

#得到源代码对应的目标文件的列表
OBJS=$(SRCCS:.c=.o) $(SRCSS:.s=.o)


#生成输出目标文件
all:$(LOCALTARGET)
	$(LOAD) $(LDFLAGS) $(TARGET) $(LOCALTARGET)
	@echo -----------------------------------
	@echo demod_MB86A22 make finish
	@echo -----------------------------------

#生成本地目标文件
$(LOCALTARGET):$(OBJS)
	$(LINK) $(LKFLAGS) -o $@ $(filter %.o ,$+) $(LIBS)


# 删除生成的中间文件
clean:
	-rm $(OBJS) $(LOCALTARGET)
