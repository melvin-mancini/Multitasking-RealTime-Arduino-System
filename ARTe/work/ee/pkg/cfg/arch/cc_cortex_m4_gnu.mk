# ###*B*###
# ERIKA Enterprise - a tiny RTOS for small microcontrollers
# 
# Copyright (C) 2013  Evidence Srl
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
## GCC EABI compiler
## (tested with gcc-eabi 4.7 from Emdebian)
##
## Author: 2013,  Mauro Nino Marinoni
##		   2013,  Alessandro Biondi	
##

#
# Select file formats
#
CG_OUT_EXTENSION := elf
# MM: Check!!!
CG_BIN_EXTENSION := bin

#
# EABI_ROOT: GNU GCC EABI Root Folder.
#          Default "/usr".
CG_TOOL_ROOT := $(GNU_ARM_ROOT)

# BINDIR is the directory of assembler, compiler, linker...
ifeq ($(call iseeopt, __RTD_CYGWIN__), yes) 
CG_BIN_DIR = $(shell cygpath -u "$(CG_TOOL_ROOT)/bin")
else
CG_BIN_DIR = $(CG_TOOL_ROOT)/bin
endif

CG_BINUTILS_DIR := $(CG_TOOL_ROOT)/bin

ifndef EABI_GCC_PREFIX
EABI_GCC_PREFIX := arm-none-eabi
endif

ifndef CG_LIB_DIR
# Automatically detected by gcc through -m options
ifeq ($(call iseeopt, __RTD_CYGWIN__), yes) 
CG_LIB_DIR = $(shell cygpath -u "$(CG_TOOL_ROOT)/$(EABI_GCC_PREFIX)/lib/thumb")
else
CG_LIB_DIR := $(CG_TOOL_ROOT)/$(EABI_GCC_PREFIX)/lib/thumb
endif
endif

ifndef CG_INCLUDE_DIR
ifeq ($(call iseeopt, __RTD_CYGWIN__), yes) 
CG_INCLUDE_DIR = $(shell cygpath -u "$(CG_TOOL_ROOT)/$(EABI_GCC_PREFIX)/include")
else
CG_INCLUDE_DIR := $(CG_TOOL_ROOT)/$(EABI_GCC_PREFIX)/include
endif
endif

ifdef TMPDIR
CG_TMPDIR := $(call native_path, $(TMPDIR))
else
CG_TMPDIR := $(call native_path, .)
endif

# Bin directories used for compilation
# BINDIR_ASM      - directory of the Assembler
# BINDIR_CC       - directory of the C compiler
# BINDIR_DEP      - directory of the C compiler used for dependencies
# BINDIR_BINUTILS - directory of the binutils

BINDIR_ASM      := $(CG_BIN_DIR)
BINDIR_CC       := $(CG_BIN_DIR)
BINDIR_BINUTILS := $(CG_BINUTILS_DIR)
BINDIR_DEP      := $(CG_BIN_DIR)

#
# Define crosscompiler EABI_GCC_PREFIX, EABI_GCC_EXT and EABI_GCC_VERSION
#


ifeq ($(call iseeopt, __RTD_CYGWIN__), yes) 
EABI_GCC_EXT := .exe
endif

#ifndef EABI_GCC_VERSION
EABI_GCC_VERSION := 
#endif

#
# Define crosscompiler tools
#

ifndef EE_ASM
#EE_ASM := $(BINDIR_ASM)/$(EABI_GCC_PREFIX)as$(EABI_GCC_EXT)
EE_ASM := $(BINDIR_CC)/$(EABI_GCC_PREFIX)-gcc$(EABI_GCC_VERSION)$(EABI_GCC_EXT)
endif

ifndef EE_CC
EE_CC := $(BINDIR_CC)/$(EABI_GCC_PREFIX)-gcc$(EABI_GCC_VERSION)$(EABI_GCC_EXT)
endif

ifndef EE_LINK
#EE_LINK := $(BINDIR_ASM)/$(EABI_GCC_PREFIX)ld$(EABI_GCC_EXT)
EE_LINK := $(BINDIR_CC)/$(EABI_GCC_PREFIX)-gcc$(EABI_GCC_VERSION)$(EABI_GCC_EXT)
endif

ifndef EE_DEP
EE_DEP := $(BINDIR_DEP)/$(EABI_GCC_PREFIX)-gcc$(EABI_GCC_VERSION)$(EABI_GCC_EXT)
endif

# Using ar from BinUtils instead of the one from GCC
ifndef EE_AR
EE_AR := $(BINDIR_ASM)/$(EABI_GCC_PREFIX)-ar$(EABI_GCC_EXT)
endif

ifndef EE_NM
EE_NM := $(BINDIR_ASM)/$(EABI_GCC_PREFIX)-nm$(EABI_GCC_EXT)
endif

ifndef EE_OBJCOPY
EE_OBJCOPY := $(BINDIR_ASM)/$(EABI_GCC_PREFIX)-objcopy$(EABI_GCC_EXT)
endif

ifndef EE_OBJDUMP
EE_OBJDUMP := $(BINDIR_ASM)/$(EABI_GCC_PREFIX)-objdump$(EABI_GCC_EXT)
endif



#
#Add application file to dependencies
#
ifneq ($(ONLY_LIBS), TRUE)

## OPT_LIBS is used to link additional libraries (e.g., for C++ support)
# MM: Add others from GCC?
OPT_LIBS += $(ERIKALIB)

TARGET = $(TARGET_NAME).$(CG_BIN_EXTENSION)

else	# ONLY_LIBS

endif	# !ONLY_LIBS



# INCLUDE_PATH is a colon separated list of directories for source file searching
# -I = adds directories to the source file search path (for both gcc and gas)
# we consider the ee pkg directory and the application dir
# we also consider the current directory because the app could be compiled
# from the config files generated from eclipse...
OPT_INCLUDE = $(foreach d,$(INCLUDE_PATH),$(addprefix -I,$(call native_path,$d)))

##
# OPT_AR: options for library generation
##

# MM: Check!!!
OPT_AR = r


##
# OPT_CC are the options for c compiler invocation
##

# Define HW architecture
ifeq ($(call iseeopt, __CORTEX_M4__), yes)
#OPT_CC += -march=armv7e-m -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -mthumb-interwork -mlong-calls
OPT_CC += -mcpu=cortex-m4 -mthumb -mthumb-interwork
endif

# Debug support and optimization level
ifeq ($(call iseeopt, DEBUG), yes)
OPT_CC += -ggdb -O0 -Wno-attributes
else
OPT_CC += -Os
endif

# Compile and assemble, but do not link 
OPT_CC += -c 

# Specific option from the application makefile
OPT_CC += $(CFLAGS)


##
# OPT_ASM are the options for assembler invocation
##

# Define HW architecture
ifeq ($(call iseeopt, __CORTEX_M4__), yes)
# MM: Check!!!
#OPT_ASM += -march=armv7e-m -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mthumb -mthumb-interwork
OPT_ASM += -mcpu=cortex-m4 -mthumb -mthumb-interwork
endif

# Debug support
ifeq ($(call iseeopt, DEBUG), yes)
OPT_ASM += -ggdb
endif



# Enable C preprocessor
#OPT_ASM += --cpreproc
ifeq ($(call iseeopt, DEBUG), yes)
OPT_ASM += -ggdb -O0
else
OPT_ASM += -Os
endif

# Assemble, but do not link 
OPT_ASM += -c 

# Specific option from the application makefile
OPT_ASM += $(ASFLAGS) 


##
# OPT_LINK represents the options for linker invocation
##

OPT_LINK += -static

# Define HW architecture
ifeq ($(call iseeopt, __CORTEX_M4__), yes)
# MM: Check!!!
#OPT_CC += -march=armv7e-m -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -mthumb-interwork -mlong-calls
OPT_LINK += -mcpu=cortex-m4 -mthumb -mthumb-interwork
endif

# FixMe: Not accepted, put in the linker script as temporal solution
#OPT_LINK += -Wl,--start-group -lgcc -lc -lm -lnosys -Wl,--end-group
# Enable support for semihosting 
#OPT_LINK += -lrdimon --specs=rdimon.specs

# Debug support
ifeq ($(call iseeopt, DEBUG), yes)
OPT_LINK += -ggdb
endif

## Put here the link options --userlibpath (instead of -L).
ifneq ($(ONLY_LIBS), TRUE)

ifdef CG_LIB_DIR
OPT_LINK += -L$(CG_LIB_DIR)
OPT_LINK += $(foreach d,$(CG_LIB_DIR),$(addprefix -L,$(call native_path,$d)))
endif

ifneq ($(call iseeopt, __BIN_DISTR), yes)
OPT_LINK += -L.
else
OPT_LINK += -L$(EEBASE)/lib
endif

endif	# !ONLY_LIB

ifdef VERBOSE
OPT_LINK += --verbose
endif

# Specific option from the application makefile
OPT_LINK += $(LDFLAGS)


##
# OPT_AR: options for library generation
##

OPT_OBJCOPY += -Obinary


# Defining EEOPT Macros
# Each identifier that is listed in EEOPT is also inserted as a 
# command-line macro in the compiler...

DEFS_ASM = $(addprefix -D, $(EEOPT))
DEFS_CC  = $(addprefix -D, $(EEOPT))


# Automatic dependency generation
ifeq ($(call iseeopt, NODEPS), yes)
DEPENDENCY_OPT = 
make-depend =
else # NODEPS
ifeq ($(call iseeopt, __RTD_CYGWIN__), yes)
# Dependencies on Windows need path translation
DEPENDENCY_OPT = -MMD -MF $(call native_path,$(subst .o,.d_tmp,$@)) -MP -MT $@
make-depend = sed -e 's_\\\(.\)_/\1_g' \
	      -e 's_\<\([a-zA-Z]\):/_/cygdrive/\l\1/_g' < $3_tmp \
	      > $3 && rm $3_tmp
else # __RTD_CYGWIN__
DEPENDENCY_OPT = -MMD -MF $(subst .o,.d_tmp,$@) -MP -MT $@
make-depend = mv $3_tmp $3
endif # __RTD_CYGWIN__
endif # NODEPS

