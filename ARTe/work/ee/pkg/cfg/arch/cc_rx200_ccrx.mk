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
## Renesas ccrx compiler
##
## Author: 2012,  Gianluca Franchino
##

# Select object file format
ifeq ($(call iseeopt, __USE_MOT__), yes)
RX_OUT_EXTENSION := mot
else
RX_OUT_EXTENSION := abs
endif

RX_BIN_EXTENSION := bin
RX_HEX_EXTENSION := hex

BINDIR_CYG := /usr/bin

#CCRX_ROOT: CCRX Renesas Root Folder.
#          Default "C:\Programmi\Renesas\Hew\Tools\Renesas\RX\1_2_0".
RX_TOOL_ROOT := $(shell cygpath -u $(call short_native_path,$(CCRX_ROOT)))

RX_BIN_DIR := $(RX_TOOL_ROOT)/bin
#RX_LIB_DIR := $(RX_TOOL_ROOT)/lib
#RX_INCLUDE_DIR := $(RX_TOOL_ROOT)/include

ifndef BIN_RX
export BIN_RX := $(shell cygpath -lw '$(CCRX_ROOT)\\Bin')
endif

ifndef INC_RX
export INC_RX := $(shell cygpath -lw '$(CCRX_ROOT)\\Include')
endif

export PATH := $(PATH):$(RX_BIN_DIR)

# Bin directories used for compilation
# BINDIR_ASM      - directory of the Assembler
# BINDIR_CC       - directory of the C compiler
# BINDIR_DEP      - directory of the C compiler used for dependencies
# BINDIR_BINUTILS - directory of the binutils

BINDIR_ASM      := $(RX_BIN_DIR)
BINDIR_CC       := $(RX_BIN_DIR)

ifndef EE_ASM
EE_ASM:=$(BINDIR_ASM)/asrx.exe
endif

ifndef EE_CC
EE_CC:=$(BINDIR_CC)/ccrx.exe
endif

ifndef EE_LINK
EE_LINK:=$(RX_BIN_DIR)/optlnk.exe
endif

ifndef EE_AR
EE_AR:=$(RX_BIN_DIR)/lbgrx.exe
endif

#Add application file to dependencies
ifneq ($(ONLY_LIBS), TRUE)

## OPT_LIBS is used to link additional libraries (e.g., for C++ support)
## Libraries from CCRX compiler.
OPT_LIBS += -library=$(ERIKALIB)

TARGET = $(TARGET_NAME).$(RX_OUT_EXTENSION)

else	# ONLY_LIBS

endif	# !ONLY_LIBS

# INCLUDE_PATH is a colon separated list of directories for source file searching
# -include = adds directories to the source file search path (for ccrx)
# we consider the ee pkg directory and the application dir
# we also consider the current directory because the app could be compiled
# from the config files generated from eclipse...

OPT_INCLUDE = $(foreach d,$(INCLUDE_PATH),$(addprefix -include=,$(call short_native_path,$d)))

# OPT_AR: options for library generation
OPT_AR = -nologo -form=lib

## OPT_CC are the options for c compiler invocation
#Note: all warnings are enabled by default
#Note: C89 is the default language
ifeq ($(call iseeopt, __RX200__), yes)
OPT_CC += -cpu=rx200
endif

ifeq ($(call iseeopt, DEBUG), yes)
OPT_CC += -debug -listfile -optimize=0
else
OPT_CC += -optimize=max
endif

OPT_CC += -lang=c99 -output=obj -nologo -auto_enum -goptimize -show=source


# Specific option from the application makefile
OPT_CC += $(CFLAGS)

##OPT_ASM are the options for assembler invocation
ifeq ($(call iseeopt, __RX200__), yes)
OPT_ASM += -cpu=rx200
endif

ifeq ($(call iseeopt, DEBUG), yes)
OPT_ASM += -debug -listfile
endif

OPT_ASM += -goptimize -nologo

# Specific option from the application makefile
OPT_ASM += $(ASFLAGS)

ifeq ($(call iseeopt, DEBUG), yes)
OPT_LINK += -debug
endif

ifeq ($(call iseeopt, __USE_MOT__), yes)
OPT_LINK += -stype
endif

ifdef VERBOSE
OPT_LINK += -message
else
OPT_LINK += -nomessage
endif

OPT_LINK += -rom=D=R,D_1=R_1,D_2=R_2 -nologo -nooptimize -map -list\
 -show=symbol,reference,xreference,total_size,vector -library=./$(RUNTIMELIB)

ifeq ($(call iseeopt, __R5F5210x__), yes)

ifeq ($(call iseeopt, __ADD_LIB_SECT__), yes)
OPT_LINK += -subcommand=$(call native_path,$(EEBASE)/pkg/mcu/renesas_r5f5210x/src/rx200_lnk_sub_ls.cmd)
else
OPT_LINK += -subcommand=$(call native_path,$(EEBASE)/pkg/mcu/renesas_r5f5210x/src/rx200_lnk_sub.cmd) 
endif
else
OPT_LINK +=
$(error RX200 CPU -> MCU type either not specified or not supported! (Supported MCUs-> R5F52108ADFP))
endif			


# Specific option from the application makefile
OPT_LINK += $(LDFLAGS)

# Specific option for libee.lib creation
OPT_LIB_LINK += -nologo -noprelink -form=library=u

# Defining EEOPT Macros
# Each identifier that is listed in EEOPT is also inserted as a 
# command-line macro in the compiler...

DEFS_ASM = 
DEFS_CC  = $(addprefix -define=, $(EEOPT))


# Automatic dependency generation
#NOTE: CCRX cannot generate a dependencies file. We should do something different
#to handle dependencies.
ifeq ($(call iseeopt, NODEPS), yes)
DEPENDENCY_OPT = 
make-depend =
else	# NODEPS
DEPENDENCY_OPT = 
make-depend =
endif	# NODEPS
