# ###*B*###
# ERIKA Enterprise - a tiny RTOS for small microcontrollers
# 
# Copyright (C) 2012  Evidence Srl
# 
# This file is part of ERIKA Enterprise.
# 
# ERIKA Enterprise is free software; you can redistribute it
# and/or modify it under the terms of the GNU General Public License
# version 2 as published by the Free Software Foundation, 
# (with a special exception described below).
# 
# Linking this code statically or dynamically with other modules is
# making a combined work based on this code.  Thus, the terms and
# conditions of the GNU General Public License cover the whole
# combination.
# 
# As a special exception, the copyright holders of this library give you
# permission to link this code with independent modules to produce an
# executable, regardless of the license terms of these independent
# modules, and to copy and distribute the resulting executable under
# terms of your choice, provided that you also meet, for each linked
# independent module, the terms and conditions of the license of that
# module.  An independent module is a module which is not derived from
# or based on this library.  If you modify this code, you may extend
# this exception to your version of the code, but you are not
# obligated to do so.  If you do not wish to do so, delete this
# exception statement from your version.
# 
# ERIKA Enterprise is distributed in the hope that it will be
# useful, but WITHOUT ANY WARRANTY; without even the implied warranty
# of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License version 2 for more details.
# 
# You should have received a copy of the GNU General Public License
# version 2 along with ERIKA Enterprise; if not, write to the
# Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
# Boston, MA 02110-1301 USA.
# ###*E*###

##
## Renesas RX200 compilation rules
##
## Author: 2012,  Gianluca Franchino
##
## Based on rules_cortex_mx.mk and RX200 Hardware Manual.
##

# Enable verbose output from EE_OPT
ifeq ($(call iseeopt, VERBOSE), yes)
VERBOSE = 1
endif

ERIKALIB = libee.lib
RUNTIMELIB = runtime.lib

# Initial definition of target all as predefined
all:

include $(EEBASE)/pkg/cfg/dir.mk
include $(PKGBASE)/cfg/verbose.mk
include $(PKGBASE)/cfg/compiler.mk

RXGCC_LIB_DIR := $(RXGCCDIR)/lib

ifneq ($(ONLY_LIBS), TRUE)

ifneq ($(call iseeopt, __BIN_DISTR), yes)
LIBDEP += $(ERIKALIB)
else
LIBDEP += $(EEBASE)/lib/$(ERIKALIB)
endif

endif	# ONLY_LIBS

#Add runtime library to dependencies
LIBDEP += $(RUNTIMELIB)

# check if RX_LIB_DIR is empty
ifneq ($(RX_LIB_DIR),)
OPT_LIBS += -L $(call native_path,$(RX_LIB_DIR))
endif

#Includes from Compiler
INCLUDE_PATH += $(RX_INCLUDE_DIR)

# Specific option from the libs dependencies
LIBDEP += $(ALL_LIBS)

# Specific option from the application makefile
LIBDEP += $(LDDEPS)


include $(PKGBASE)/cfg/cfg.mk

# MCU

ifeq ($(call iseeopt, __RX200__), yes)
#~ RSKRX200_MODEL
#~ RSKRX200_LINKERSCRIPT
#~ RSKRX200_INCLUDE_C
#~ RSKRX200_INCLUDE_S
#~ RSKRX200_STARTUP
ifneq ($(call iseeopt, __R5F5210x__), yes)
RX200_MCU_MODEL = $(RX200_MODEL)
RX200_MCU_STARTUP = $(RX200_STARTUP)
RX200_MCU_LINKERSCRIPT = $(RX200_LINKERSCRIPT)
else	# !__R5F5210x__
RX200_MCU_MODEL = $(RX200_MODEL)
RX200_MCU_STARTUP = $(CRT0_SRCS)
endif	# __R5F5210x__
TARGET_NAME = rx200
endif	# __RX200__

ifndef RX200_MCU_MODEL
$(error No known MCU model found in EE_OPT)
endif	# RX200_MCU_MODEL

EE_BOOT_SRCS := $(RX200_MCU_STARTUP)

# Boot code containing _start should stay outside of the library in
# case of normal compilation
ifeq ($(call iseeopt, __BIN_DISTR), yes)
LIBSRCS += $(EE_BOOT_SRCS)
else
SRCS += $(EE_BOOT_SRCS)
endif

# Add a custom linker script if provided by the user
LINKDEP += $(RX200_MCU_LINKERSCRIPT)

ifeq ($(call iseeopt, __NO_APP__), yes)
TARGET := $(ERIKALIB)

# we put the eecfg.c inside the library
LIBEESRCS += eecfg.c
endif

#
# --------------------------------------------------------------------------
#

##
## Source files and paths
##
LIBEESRCS += $(EE_SRCS)
LIBEEOBJS := $(addprefix $(OBJDIR)/, $(patsubst %.c, %.obj, $(patsubst %.s, %.obj, \
$(patsubst %.asm, %.obj, $(patsubst %.src, %.obj, $(LIBEESRCS))))))

#LIBEESRCS += $(LIB_SRCS)

LIBOBJS := $(addprefix $(OBJDIR)/, $(patsubst %.c, %.obj, $(patsubst %.s, %.obj,\
 $(patsubst %.asm, %.obj, $(patsubst %.src, %.obj, $(LIBSRCS))))))

SRCS += $(APP_SRCS)

OBJS := $(addprefix $(OBJDIR)/, $(sort $(patsubst %.cpp, %.obj, \
$(patsubst %.cxx, %.obj, $(patsubst %.cc, %.obj, $(patsubst %.C, %.obj, \
$(CXX_SRCS))))) $(patsubst %.c, %.obj, $(patsubst %.s, %.obj, \
$(patsubst %.asm, %.obj, $(patsubst %.src, %.obj, $(SRCS)))))))


# Variable used to import dependencies
ALLOBJS = $(LIBEEOBJS) $(LIBOBJS) $(OBJS) 

OBJDIRS=$(sort $(dir $(ALLOBJS)))

INCLUDE_PATH += $(PKGBASE) $(APPBASE) . $(RX_INLCUDE_DIR)

vpath %.c $(EE_VPATH) $(APPBASE)
vpath %.s $(EE_VPATH) $(APPBASE)
vpath %.src $(EE_VPATH) $(APPBASE)

## Compute common variables ##
COMPUTED_INCLUDE_PATH := $(OPT_INCLUDE)
COMPUTED_OPT_LINK := $(OPT_LINK)
COMPUTED_OPT_LIB_LINK := $(OPT_LIB_LINK)
COMPUTED_OPT_ASM := $(OPT_ASM)
COMPUTED_OPT_CC := $(OPT_CC)
COMPUTED_OPT_AR := $(OPT_AR)
#COMPUTED_OPT_OBJDUMP := $(OPT_OBJDUMP)

ifdef RX200_MCU_LINKERSCRIPT
ifeq ($(call iseeopt, __CCRX__), yes)
COMPUTED_LINKERSCRIPT := -lnkcmd $(call native_path, $(RX_MCU_LINKERSCRIPT))
else
COMPUTED_LINKERSCRIPT := -T
endif 	# __CCRX__
endif	# RX200_MCU_LINKERSCRIPT

## Select input filename format ##
SOURCEFILE = $(call native_path,$<)
TARGETFILE = $(call native_path,$@)
## 

## Main rules: all clean ##

.PHONY: all clean

all: make_directories $(ALL_LIBS) $(TARGET)
	@echo "************************************"
	@echo "Compilation terminated successfully!"

clean:
	@-rm -rf obj *.obj *.lib *.bls *.map *.sim *.htm *.$(RX_BIN_EXTENSION) \
	*.$(RX_OUT_EXTENSION) *.$(RX_HEX_EXTENSION)
	@echo "CLEAN";

### Target file creation ###
$(TARGET_NAME).$(RX_OUT_EXTENSION): $(OBJS) $(LINKDEP) $(LIBDEP) 
	@echo "LD $(TARGET_NAME).$(RX_OUT_EXTENSION)";
ifeq ($(call iseeopt, __CCRX__), yes)	
	$(QUIET)$(EE_LINK) $(COMPUTED_OPT_LINK) -output=$@ $(OBJS) $(OPT_LIBS)
else
	$(QUIET)$(EE_LINK) $(COMPUTED_OPT_LINK) -o $(TARGETFILE) $(OBJS) \
                     --start-group $(OPT_LIBS) --end-group -M > rx200.map
endif	
	
### Object file creation ###

# produce the object file from assembly code in a single step
$(OBJDIR)/%.obj: %.s
	$(VERBOSE_PRINTASM) $(EE_ASM) $(COMPUTED_OPT_ASM) \
	$(COMPUTED_INCLUDE_PATH) $(DEFS_ASM) $(SOURCEFILE) \
	-output=$(TARGETFILE)

$(OBJDIR)/%.obj: %.src
	$(VERBOSE_PRINTASM)	$(EE_ASM) $(COMPUTED_OPT_ASM) \
	$(COMPUTED_INCLUDE_PATH) $(DEFS_ASM) $(SOURCEFILE) \
	-output=$(TARGETFILE)

# produce the object file from C code in a single step
$(OBJDIR)/%.obj: %.c
	$(VERBOSE_PRINTCC) $(EE_CC) $(COMPUTED_OPT_CC) \
	$(COMPUTED_INCLUDE_PATH) $(DEFS_CC) $(DEFS_ISR) $(DEPENDENCY_OPT) \
	$(SOURCEFILE) -output=obj=$(TARGETFILE)
	$(QUIET)$(call make-depend, $<, $@, $(subst .obj,.d,$@))

#produce the runtime library
$(RUNTIMELIB):
	@echo "AR $(RUNTIMELIB)";
	$(QUIET)$(EE_AR) -nologo -cpu=rx200 -output=$@ -head=runtime

	
##
## EE Library
##s
$(ERIKALIB): $(LIBEEOBJS)
	@echo "AR $(ERIKALIB)";
	$(QUIET) $(EE_LINK) $(COMPUTED_OPT_LIB_LINK) -output=$@ $(LIBEEOBJS)

	
##
## Directories
##

.PHONY: make_directories

## Objects depend on directories, but they are not remade if directories change
$(ALLOBJS): | make_directories

make_directories: $(OBJDIRS)

## Directories are (re)created only when some of them don't exist already
$(OBJDIRS): 
	@echo "MAKE_DIRECTORIES"
	$(QUIET)mkdir -p $(OBJDIRS)

# the eeopt file is generated when dealing with ONLY_LIBS!
#
# this is a phony because the source code does not depend on this file
# and its content higly depends on the EEOPT variables...
#
.PHONY: generate_eeopt
generate_eeopt:
	@echo "GEN   eeopt.h"
	$(QUIET)for x in $(EEOPT); do \
		echo \#ifndef $${x}      >> eeopt.h; \
		echo \#define $${x}      >> eeopt.h; \
		echo \#endif             >> eeopt.h; \
	done;

##
## Automatic Generation of dependencies
##

dependencies=$(subst .obj,.d,$(ALLOBJS))
ifneq ($(call iseeopt, NODEPS), yes) 
ifneq ($(MAKECMDGOALS),clean)
-include $(dependencies)
endif
endif
