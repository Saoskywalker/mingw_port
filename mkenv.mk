.PHONY:all clean test_makefile
.PHONY:write mktool dump run

BUILD_VERBOSE = 0
ifeq ($(BUILD_VERBOSE),0)
Q = @
else
Q =
endif

#
# System environment variable.
# system app extension
#
ifeq ($(OS), Windows_NT)
	HOSTOS	:= windows
	EXTENSION ?= .exe
else
	ifneq (,$(findstring Linux, $(shell uname -a)))
		HOSTOS	:= linux
		EXTENSION ?= .elf
	endif
endif

# define main target
PROG ?= APP

# define output dir
BUILD ?= build

# define command
RM = rm
ECHO = @echo
CP = cp
MKDIR = mkdir
SED = sed
PYTHON = python

#gcc compiler select
CROSS_COMPILE ?= #arm-eabi-
CC	          = $(CROSS_COMPILE)gcc
AS	          = $(CROSS_COMPILE)gcc -x assembler-with-cpp
CXX 		  = $(CROSS_COMPILE)g++
GDB 		  = $(CROSS_COMPILE)gdb
LD	          = $(CROSS_COMPILE)ld
OBJCOPY	      = $(CROSS_COMPILE)objcopy
OBJDUMP       = $(CROSS_COMPILE)objdump
SIZE 		  = $(CROSS_COMPILE)size
STRIP 		  = $(CROSS_COMPILE)strip
AR 			  = $(CROSS_COMPILE)ar

#define
DEFINES		+= 

# compiler set
# Tune for Debugging or Optimization
DEBUG ?= 0
ifeq ($(DEBUG), 1)
PROG = main_debug
ASFLAGS		= -g -ggdb -Wall -O0 -ffreestanding -std=gnu99 $(DEFINES)
CFLAGS		= -g -ggdb -Wall -O0 -ffreestanding -std=gnu99 $(DEFINES)
CXXFLAGS	= -g -ggdb -Wall -O0 -ffreestanding -std=c++11 $(DEFINES)
LDFLAGS		+= #-T f1c100s.ld -nostdlib
MCFLAGS		+= #-march=armv5te -mtune=arm926ej-s -mfloat-abi=soft -marm -mno-thumb-interwork
else
PROG = main_release
ASFLAGS		= -Wall -Os -ffreestanding -std=gnu99 $(DEFINES)
CFLAGS		= -Wall -Os -ffreestanding -std=gnu99 $(DEFINES)
CXXFLAGS	= -Wall -Os -ffreestanding -std=c++11 $(DEFINES)
LDFLAGS		+= -mwindows
MCFLAGS		+= #-march=armv5te -mtune=arm926ej-s -mfloat-abi=soft -marm -mno-thumb-interwork
endif

#include path
INCDIRS	+= \
	-Iarch/x64/public/include \

#library path
LIBDIRS	+=

#library
LIBS += ico/project_ico.res -lgcc -lmingw32 -lSDL2main -lSDL2 -lwinmm

#c source path
SRCDIRS_C += 

#base drivers src
SRC_C += \

all: $(BUILD)/$(addprefix $(PROG), $(EXTENSION))
	$(ECHO) "<><><><><><>><><>><><><><><><><><><>><><><><><><>"
	$(ECHO) make compile ok: $< 
	$(ECHO) $(shell date +%F%n%T)
	$(ECHO) "<><><><><><>><><>><><><><><><><><><><><><><<><><>"

run:
	$(ECHO) run ./$(BUILD)/$(addprefix $(PROG), $(EXTENSION))
	@ ./$(BUILD)/$(addprefix $(PROG), $(EXTENSION))

clean:
	find ./$(BUILD)/ -name "*.o"  | xargs rm -f  
	find ./$(BUILD)/ -name "*.bin"  | xargs rm -f
	find ./$(BUILD)/ -name "*.elf"  | xargs rm -f
	find ./$(BUILD)/ -name "*.P"  | xargs rm -f
	find ./$(BUILD)/ -name "*.d"  | xargs rm -f
	find ./$(BUILD)/ -name "*.map"  | xargs rm -f

dump:
	$(OBJDUMP) -S myboot.o | less

write:
	sudo sunxi-fel -p spiflash-write 0 $(BUILD)/$(addprefix $(PROG), .bin)

mktool:
	cd tools/mksunxiboot && make
	cd tools/mksunxi && make

MKSUNXI		:= tools/mksunxi/mksunxi

mkboot:
	$(ECHO) Make header information for brom booting
	@$(MKSUNXI) $(BUILD)/$(addprefix $(PROG), .bin)

test_makefile:
	@echo $(SRC_C)
