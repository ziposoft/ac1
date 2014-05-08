#
#		 ZIPO 
#
#		  COMMON MAKFILE

# $ sed 's/:\([0-9][0-9]*\):/(\1) :/g' make.log > make2.log






ifeq ($(TGT_NAME),)
$(error TGT_NAME not set.)
endif

ifeq ($(CODEROOT),)
$(error CODEROOT not set.)
endif


ZROOT= $(CODEROOT)/ziposoft

ifneq ($(WIND_BASE),)
	PLATFORM_BUILD=BUILD_VX
	PLATFORM_TARGET=vxworks
	
else
MACHINE= $(shell gcc -dumpmachine)
ifneq (,$(findstring mingw32,$(MACHINE)))
	PLATFORM_BUILD=BUILD_GCC
	LINK_OPTS+= -static  
	OS=WINDOWS
else
ifneq (,$(findstring linux,$(MACHINE)))
	PLATFORM_BUILD=BUILD_GCC
	LINK_OPTS+= -static  -lrt
	OS=LINUX
else
ifneq (,$(findstring lynxos,$(MACHINE)))
	PLATFORM_BUILD=BUILD_GCC
	LINK_OPTS+= -static
	OS=LYNX
else
ifneq (,$(findstring darwin,$(MACHINE)))
	PLATFORM_BUILD=BUILD_GCC
	PLATFORM_TARGET=OSX
	OS=OSX

endif
endif
endif
endif
endif


ifeq ($(PLATFORM_BUILD),)
$(error PLATFORM_BUILD not set.)
endif
CC=gcc
CPP=g++

ifeq ($(PLATFORM_BUILD),BUILD_VX)
WIND_BASE := $(subst \,/,$(WIND_BASE))
TGT_DIR= $(WIND_BASE)/target
TOOL=gnu
CPU=Pentium4
ifeq "$(PROC)" "ppc"
	CPU=PPC32
	CPU_VARIANT= _ppc85XX_e500mc
	VX_CPU_FAMILY= 
	MACHINE=vx_ppc32
endif
ifeq "$(PROC)" "x64"
	CPU = NEHALEM
	CPU_VARIANT = none
	VXBUILD=LP64
	_WRS_CONFIG_LP64=1
	export _WRS_CONFIG_LP64 
	MACHINE=vx_x64
endif

#include $(TGT_DIR)/h/make/defs.option
#include $(TGT_DIR)/h/make/defs.default
#include $(TGT_DIR)/h/tool/$(TOOL_FAMILY)/make.$(CPU)$(TOOL)
include $(TGT_DIR)/h/make/defs.library
OPTION_ANSI=
CFLAGS+= -std=gnu99 -pedantic  -D_WRS_KERNEL
include $(TGT_DIR)/h/make/rules.library
OPTION_ANSI=

endif

CFLAGS+= -I$(CODEROOT)  -I$(ZROOT) $(INC_DIRS)  -D$(PLATFORM_BUILD)

ifeq ($(DEBUG),1)
CFLAGS+= -g  -ggdb -DDEBUG=1
BUILD_TYPE= debug

else
CFLAGS+= -O2 
BUILD_TYPE= release
endif

LIB_PATH= $(ZROOT)/lib/$(BUILD_TYPE)/$(MACHINE)/
DIR_BUILD = $(ZROOT)/build/$(BUILD_TYPE)/$(MACHINE)/$(TGT_NAME)/
BIN_PATH=  $(ZROOT)/$(BUILD_TYPE)/$(MACHINE)/

SRC_AUTO_C =	$(wildcard	$(SRC_PATH)*.c)
SRC_AUTO_CPP =	$(wildcard	$(SRC_PATH)*.cpp) 


DEP_AUTO_C=$(SRC_AUTO_C:$(SRC_PATH)%.c=$(DIR_BUILD)%.d)
DEP_AUTO_CPP=$(SRC_AUTO_CPP:$(SRC_PATH)%.cpp=$(DIR_BUILD)%.dpp)
DEPS= $(DEP_AUTO_C) $(DEP_AUTO_CPP)
OBJS_AUTO_C=$(SRC_AUTO_C:$(SRC_PATH)%.c=$(DIR_BUILD)%.o)
OBJS_AUTO_CPP=$(SRC_AUTO_CPP:$(SRC_PATH)%.cpp=$(DIR_BUILD)%.opp)
OBJS_C=$(SRC_C:%.c=$(DIR_BUILD)%.o)

OBJS_LOCAL= $(OBJS_AUTO_C) $(OBJS_AUTO_CPP) $(OBJS_C)
#OBJS= $(patsubst %.o,$(DIR_BUILD)%.o, $(OBJS_LOCAL))
OBJS= $(OBJS_LOCAL)

$(DIR_BUILD)%.d: $(SRC_PATH)%.c
	@set -e;
	@echo "Making $@  $<"
	@rm -f $@;
	@$(CC) -M $(CFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

$(DIR_BUILD)%.dpp: $(SRC_PATH)%.cpp
	@echo "Making $@  $<"
	@set -e;
	@rm -f $@;
	@$(CPP) -M $(CPPFLAGS) $(CFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$


$(DIR_BUILD)%.o: $(SRC_PATH)%.c $(DIR_BUILD)%.d
	$(CC) $(CFLAGS)  -c $< -o $@

$(DIR_BUILD)%.opp: $(SRC_PATH)%.cpp $(DIR_BUILD)%.dpp
	$(CPP)	$(CPPFLAGS) $(CFLAGS)  -c $< -o $@


all: prepare $(TGT)

depend: $(DEP_AUTO_C)
	@echo DEPS: $(DEP_AUTO_C)

deptouch:
	@touch $(DEP_AUTO_C)


prepare:
	@echo PLATFORM_BUILD=$(PLATFORM_BUILD)
	@echo UNAME=$(UNAME)
	@-mkdir -p $(DIR_BUILD) 2>/dev/null
	@-mkdir -p $(LIB_PATH) 2>/dev/null
	@-mkdir -p $(BIN_PATH) 2>/dev/null

clean:	
	@-rm  $(TGT) $(DEPS) $(OBJS)  2>/dev/null
	
show:
	@echo PLATFORM_TARGET=$(PLATFORM_TARGET)
	@echo PLATFORM_BUILD=$(PLATFORM_BUILD)
	@echo DIR_BUILD=$(DIR_BUILD)
	@echo LIB_PATH=$(LIB_PATH)
	@echo BIN_PATH=$(BIN_PATH)
	@echo MACHINE=$(MACHINE)
	@echo SRC_PATH: $(SRC_PATH)
	@echo CC: $(CC)
	@echo TGT: $(TGT)
	@echo 	

show_files:
	@echo PLATFORM_TARGET=$(PLATFORM_TARGET)
	@echo PLATFORM_BUILD=$(PLATFORM_BUILD)
	@echo DIR_BUILD=$(DIR_BUILD)
	@echo MACHINE=$(MACHINE)
	@echo SRC_PATH: $(SRC_PATH)
	@echo 	
	@echo DEPS: $(DEPS)
	@echo 	
	@echo CC: $(CC)
	@echo 	
	@echo TGT: $(TGT)
	@echo 	
	@echo SRC_AUTO_CPP: $(SRC_AUTO_CPP)
	@echo 	
	@echo SRC_AUTO_C: $(SRC_AUTO_C)
	@echo 	
	@echo OBJS: $(OBJS)
	@echo 	
	@echo OBJS_LOCAL: $(OBJS_LOCAL)
	@echo 	
	@echo OBJS_AUTO_CPP: $(OBJS_AUTO_CPP)
	@echo 	


		
.PHONY : clean touchall show_deps show	prepare depend Makefile

-include $(DEPS)

