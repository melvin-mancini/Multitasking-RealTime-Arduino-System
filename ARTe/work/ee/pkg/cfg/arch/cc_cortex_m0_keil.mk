# ###*B*###
# ERIKA Enterprise - a tiny RTOS for small microcontrollers
# 
# Copyright (C) 20011  Evidence Srl
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
## Keil uVision MDK-Lite compiler
##
## Author: 2011,  Giuseppe Serano
## Author: 2012, Gianluca Franchino
##

# Select object file format
ifeq ($(call iseeopt, __KEIL_USE_AXF_EXT__), yes)
CG_OUT_EXTENSION := axf
else
CG_OUT_EXTENSION := out
endif

ifeq ($(call iseeopt, __NRF51X22__), yes)
# Hex file (Intel32) required by nRFgo Studio
CG_HEX_EXTENSION := hex
endif

BINDIR_CYG := /usr/bin

#ARM_ROOT: Keil uVision ARM Installation Root Folder.
#          Default "C:\Keil\ARM".
CG_TOOL_ROOT := $(ARM_ROOT)

ifeq ($(call iseeopt, __KEIL_4_54_OLDER__), yes)
CG_BIN_DIR := $(CG_TOOL_ROOT)/BIN40
else
CG_BIN_DIR := $(CG_TOOL_ROOT)/ARMCC/bin
endif

CG_BINUTILS_DIR := $(CG_TOOL_ROOT)/BIN

ifeq ($(call iseeopt, __KEIL_4_54_OLDER__), yes)
CG_LIB_DIR := $(CG_TOOL_ROOT)/RV31/LIB
else
CG_LIB_DIR := $(CG_TOOL_ROOT)/ARMCC/lib $(CG_TOOL_ROOT)/RV31/LIB
endif

ifeq ($(call iseeopt, __KEIL_4_54_OLDER__), yes)
CG_INCLUDE_DIR := $(CG_TOOL_ROOT)/RV31/INC/
else
CG_INCLUDE_DIR := $(CG_TOOL_ROOT)/ARMCC/include $(CG_TOOL_ROOT)/RV31/INC/
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

ifndef EE_ASM
EE_ASM:=$(BINDIR_ASM)/armasm.exe
endif

ifndef EE_CC
EE_CC:=$(BINDIR_CC)/armcc.exe
endif

ifndef EE_LINK
EE_LINK:=$(CG_BIN_DIR)/armlink.exe
endif

ifndef EE_DEP
EE_DEP:=$(BINDIR_DEP)/armcc.exe
endif

ifndef EE_AR
EE_AR:=$(CG_BIN_DIR)/armar.exe
endif

ifndef EE_FROMELF
EE_FROMELF:=$(CG_BIN_DIR)/fromelf.exe
endif

#Add application file to dependencies
ifneq ($(ONLY_LIBS), TRUE)

## OPT_LIBS is used to link additional libraries (e.g., for C++ support)
## Libraries from Keil-uVision-MKD-Lite.
OPT_LIBS += $(ERIKALIB)

ifeq ($(call iseeopt, __NRF51X22__), yes)
TARGET = $(TARGET_NAME).$(CG_HEX_EXTENSION)
else
TARGET = $(TARGET_NAME).$(CG_OUT_EXTENSION)
endif 	#__NRF51X22__

else	# ONLY_LIBS

endif	# !ONLY_LIBS

# INCLUDE_PATH is a colon separated list of directories for source file searching
# -I = adds directories to the source file search path (for both gcc and gas)
# we consider the ee pkg directory and the application dir
# we also consider the current directory because the app could be compiled
# from the config files generated from eclipse...

OPT_INCLUDE = $(foreach d,$(INCLUDE_PATH),$(addprefix -I,$(call native_path,$d)))

# OPT_AR: options for library generation
OPT_AR += -r --create

ifeq ($(call iseeopt, DEBUG), yes)
OPT_AR += --debug_symbols
endif

## OPT_CC are the options for c compiler invocation
#Note: all warnings are enabled by default
#Note: C is the default language
ifeq ($(call iseeopt, __CORTEX_M0__), yes)
OPT_CC += --cpu Cortex-M0
endif

ifeq ($(call iseeopt, DEBUG), yes)
OPT_CC += -g -O0
endif

#AB: FIXME why __EVAL?
OPT_CC += -c -D__EVAL

# Specific option from the application makefile
OPT_CC += $(CFLAGS)

ifeq ($(call iseeopt, __NRF51X22__), yes)
OPT_CC += -DNRF51 -DSETUPA -D__MICROLIB --apcs=interwork --split_sections --li --c99
INCLUDE_PATH += $(ARM_ROOT)/CMSIS/Include
#Option to generate hex file (Intel 32 format)
OPT_FROMELF += --i32
endif

OPT_CC += --omf_browse $(TARGET_NAME).crf --depend $(TARGET_NAME).d

##OPT_ASM are the options for assembler invocation
ifeq ($(call iseeopt, __CORTEX_M0__), yes)
OPT_ASM += --cpu Cortex-M0
endif

ifeq ($(call iseeopt, DEBUG), yes)
OPT_ASM += -g
endif

OPT_ASM += --xref --cpreproc

# Specific option from the application makefile
OPT_ASM += $(ASFLAGS)

## OPT_LINK represents the options for linker invocation
ifeq ($(call iseeopt, __CORTEX_M0__), yes)
OPT_LINK += --cpu Cortex-M0
endif

ifeq ($(call iseeopt, DEBUG), yes)
OPT_LINK += --debug
endif

## Put here the link options --userlibpath (instead of -L).
ifneq ($(ONLY_LIBS), TRUE)

ifdef CG_LIB_DIR
ifeq ($(call iseeopt, __KEIL_4_54_OLDER__), yes)
OPT_LINK += --libpath $(call native_path, $(CG_LIB_DIR))
else
OPT_LINK += $(foreach d,$(CG_LIB_DIR),$(addprefix --libpath ,$(call native_path,$d)))
endif
endif

ifneq ($(call iseeopt, __BIN_DISTR), yes)
OPT_LINK += --userlibpath .
else
OPT_LINK += --userlibpath $(EEBASE)/lib
endif

endif	# !ONLY_LIB

ifdef VERBOSE
OPT_LINK += --verbose
endif

## FIX ME.
OPT_LINK += --strict --map --xref --callgraph  --symbols \
	    --summary_stderr --info summarysizes --info sizes --info totals \
	    --info unused --info veneers --info libraries

ifndef LPCXPRESSO_LINKERSCRIPT

#OPT_LINK += --ro-base 0x00000000 --rw-base 0x10000000 \
#	     --first EE_cortex_mx_vtable

#ifdef EE_CORTEX_MX_RESET_ISR
# OPT_LINK += --entry $(EE_CORTEX_MX_RESET_ISR)
#else
# OPT_LINK += --entry EE_cortex_mx_default_reset_ISR
#endif

endif	# LPCXPRESSO_LINKERSCRIPT

# ifndef STELLARIS_LINKERSCRIPT

# OPT_LINK += --ro-base 0x00000000 --rw-base 0x10000000 \
	    # --first EE_cortex_mx_vtable

# ifdef EE_CORTEX_MX_RESET_ISR
# OPT_LINK += --entry $(EE_CORTEX_MX_RESET_ISR)
# else
# OPT_LINK += --entry EE_cortex_mx_default_reset_ISR
# endif

# endif	# STELLARIS_LINKERSCRIPT

# Specific option from the application makefile
OPT_LINK += $(LDFLAGS)


# Defining EEOPT Macros
# Each identifier that is listed in EEOPT is also inserted as a 
# command-line macro in the compiler...

DEFS_ASM = $(foreach d,$(EEOPT),--pd "$d SETA 1")
DEFS_CC  = $(addprefix -D, $(EEOPT))


# Automatic dependency generation
ifeq ($(call iseeopt, NODEPS), yes)
DEPENDENCY_OPT = 
make-depend =
else	# NODEPS
DEPENDENCY_OPT = --depend $(call native_path,$(subst .o,.d_tmp,$@))
ifeq ($(call iseeopt, __RTD_CYGWIN__), yes)
# Dependencies on Windows need path translation
make-depend = sed -e 's_\\\(.\)_/\1_g' \
	      -e 's_\<\([a-zA-Z]\):/_/cygdrive/\l\1/_g' < $3_tmp \
	      > $3 && rm $3_tmp
else # __RTD_CYGWIN__
make-depend = mv $3_tmp $3
endif	# __RTD_CYGWIN__
endif	# NODEPS
