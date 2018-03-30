# ###*B*###
# ERIKA Enterprise - a tiny RTOS for small microcontrollers
# 
# Copyright (C) 2002-2010  Evidence Srl
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
## PIC30 GCC compiler version 3.3 dsPIC30, Microchip 1.32
##

ifeq ($(call iseeopt, __PIC30__), yes)

# Select object file format
# IMPORTANT NOTE:
# Due to a bug in MPLAB IDE, debug symbols are only supported in COFF
ifeq ($(call iseeopt, __RTD_CYGWIN__), yes) 

 ifeq ($(call iseeopt, __USE_ELF__), yes) 
  PIC30_OFF := elf
  PIC30_EXTENSION := elf
 else
  # For Windows users using MPLAB X IDE
  PIC30_OFF := coff
  PIC30_EXTENSION := cof
 endif

else
 # Non Windows OS use MPLAB X IDE
 PIC30_OFF := elf
 PIC30_EXTENSION := elf
endif

# Bin install directories of the various compilers
# BINDIR_C30   - Microchip C30 binary directory (includes both ASM30 and C30)
# BINDIR_ASM30 - Microchip ASM30 installed with MPLAB IDE
# BINDIR_CYG   - Cygwin gcc
# BINDIR_EVI   - C30 compiler compiled from Microchip gcc source code

BINDIR_EVI := $(PIC30_GCCDIR)/bin/bin
BINDIR_CYG := /usr/bin

# If Linux, force recompiled gcc usage

ifeq ($(call iseeopt, __RTD_LINUX__), yes) 
PIC30_USE_EEGCC_COMPILE := Y
endif
ifeq ($(call iseeopt, __RTD_CYGWIN__), yes) 
ifeq ($(wildcard $(PIC30_GCCDIR)/bin/bin),)
BINDIR_C30   := $(PIC30_GCCDIR)/bin
else
BINDIR_C30   := $(PIC30_GCCDIR)/bin/bin
endif
endif

# Make sure that the variable TEMP is defined on Cygwin/Windows.
# This is needed on some Cygwin installations, where both TEMP and TMP
# variables are unset inside ".bashrc".  A reasonable default is chosen
# after a check for the existence of the directory.
ifeq ($(call iseeopt, __RTD_CYGWIN__), yes)
ifndef TMP
ifndef TEMP
ifneq (ok,$(shell test -d "/tmp" && echo ok ))
$(error Environment variables TMP and TEMP are unset, and Cygwin /tmp directory does not exist)
endif
export TEMP := $(call native_path,/tmp)
endif
endif
endif

#BINDIR_ASM30 := $(PIC30_ASMDIR)/bin
#BINDIR_CYG   := /usr/bin
#BINDIR_EVI   := 

# Bin directories used for compilation
# BINDIR_ASM      - directory of the Assembler
# BINDIR_CC       - directory of the C compiler
# BINDIR_DEP      - directory of the C compiler used for dependencies
# BINDIR_BINUTILS - directory of the binutils

# If Linux always first branch

ifeq ($(PIC30_USE_EEGCC_COMPILE), Y)
BINDIR_ASM      := $(BINDIR_EVI)
BINDIR_CC       := $(BINDIR_EVI)
BINDIR_BINUTILS := $(BINDIR_EVI)
else
BINDIR_ASM      := $(BINDIR_C30)
BINDIR_CC       := $(BINDIR_C30)
BINDIR_BINUTILS := $(BINDIR_C30)
endif

ifeq ($(wildcard $(PIC30_GCCDIR)/bin/xc16*),)
# Prefix for GCC tools (prefix for deps below)
PIC30_GCCPREFIX := pic30-$(PIC30_OFF)-
PIC30_DEPPREFIX := pic30-$(PIC30_OFF)-
else
# Prefix for GCC tools (prefix for deps below)
PIC30_GCCPREFIX := $(PIC30_OFF)-
PIC30_DEPPREFIX := $(PIC30_OFF)-
endif

ifeq ($(PIC30_USE_EEGCC_DEPS), Y)
BINDIR_DEP := $(BINDIR_EVI)
else
BINDIR_DEP := $(BINDIR_C30)
endif

ifndef EE_LINK
EE_LINK:=$(BINDIR_BINUTILS)/$(PIC30_GCCPREFIX)ld
endif

ifndef EE_ASM
EE_ASM:=$(BINDIR_ASM)/$(PIC30_GCCPREFIX)as
endif

ifndef EE_CC
EE_CC:=$(BINDIR_CC)/$(PIC30_GCCPREFIX)gcc
endif

ifndef EE_DEP
EE_DEP:=$(BINDIR_DEP)/$(PIC30_DEPPREFIX)gcc
endif

ifndef EE_AR
EE_AR:=$(BINDIR_BINUTILS)/$(PIC30_GCCPREFIX)ar
endif

ifndef EE_NM
EE_NM:=$(BINDIR_BINUTILS)/$(PIC30_GCCPREFIX)nm
endif

ifndef EE_OBJCOPY
EE_OBJCOPY:=$(BINDIR_BINUTILS)/$(PIC30_GCCPREFIX)objcopy
endif

ifndef EE_OBJDUMP
EE_OBJDUMP:=$(BINDIR_BINUTILS)/$(PIC30_GCCPREFIX)objdump
endif

OPT_INCLUDE = $(foreach d,$(INCLUDE_PATH),$(addprefix -I,$(call native_path,$d)))

## OPT_CC are the options for arm compiler invocation
OPT_CC = -O2 -Wall -Winline
ifeq ($(call iseeopt, DEBUG), yes)
OPT_CC += -ggdb
endif
# Specific option from the application makefile
OPT_CC += $(CFLAGS)

OPT_CC_DEPS := $(OPT_CC)

# target type, not used for dependencies if the Cygwin GCC is used
ifneq ($(PIC30_MODEL),)
OPT_CC += -mcpu=$(PIC30_MODEL)

ifeq ($(PIC30_USE_EEGCC_DEPS), Y)
OPT_CC_DEPS += -mcpu=$(PIC30_MODEL)
else
OPT_CC_DEPS += -D__dsPIC$(PIC30_MODEL)__
endif
endif

# #OPT_ASM are the options for asm invocation
OPT_ASM = -Ifrommchp
ifeq ($(call iseeopt, DEBUG), yes)
OPT_ASM += -g
#--gstabs
endif
# Specific option from the application makefile
OPT_ASM += $(ASFLAGS)

## OPT_LINK represents the options for armlink invocation
OPT_LINK = 
# Specific option from the application makefile
OPT_LINK += $(LDFLAGS)

# OPT_AR: options for library generation
OPT_AR = rs

# Defining EEOPT Macros
# Each identifier that is listed in EEOPT is also inserted as a 
# command-line macro in the compiler...

DEFS_ASM = $(addprefix -D, $(EEOPT) )
DEFS_CC  = $(addprefix -D, $(EEOPT) )

# Set the right Debug macro for compiler
ifeq ($(call iseeopt, DEBUG), yes)
DEFS_ASM += -D__DEBUG
DEFS_CC += -D__DEBUG
endif

ifeq ($(call iseeopt, __BIN_DISTR), yes) 
# Note: the defines used in EEOPT to compile the library
# are already added in the eecfg.h
DEFS_ASM += -D__CONFIG_$(EELIB)__
DEFS_CC  += -D__CONFIG_$(EELIB)__
endif

# Automatic dependency generation
ifeq ($(call iseeopt, NODEPS), yes)
DEPENDENCY_OPT = 
make-depend =
else # NODEPS
ifeq ($(call iseeopt, __RTD_CYGWIN__), yes)
# Dependencies on Windows need path translation
DEPENDENCY_OPT = -MMD -MF $(call native_path,$(subst .o,.d_tmp,$@)) -MP -MT $@
make-depend = sed -e 's_\\\(.\)_/\1_g' -e 's_\<\([a-zA-Z]\):/_/cygdrive/\l\1/_g' < $3_tmp > $3 && rm $3_tmp
else # __RTD_CYGWIN__
DEPENDENCY_OPT = -MMD -MF $(subst .o,.d_tmp,$@) -MP -MT $@
# Remove spurious quotes generated by the C30 compiler
make-depend = tr -d '"' < $3_tmp > $3
endif # __RTD_CYGWIN__
endif # NODEPS

endif # __PIC30__
