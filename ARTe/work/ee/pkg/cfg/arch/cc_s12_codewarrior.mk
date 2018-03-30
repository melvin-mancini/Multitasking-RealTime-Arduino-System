# ###*B*###
# ERIKA Enterprise - a tiny RTOS for small microcontrollers
# 
# Copyright (C) 2011  Evidence Srl
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

## Author: 2011 - Dario Di Stefano

##
## CodeWarrior compiler
##

# Definitions to keep compatibility with previous versions
# RTDRUID_CONFIGURATOR_NUMBER < 1276
ifndef S12_ASMDIR
S12_ASMDIR := $(COSMIC_ASMDIR)
endif
ifndef S12_CCDIR
S12_CCDIR := $(COSMIC_CCDIR)
endif
ifndef S12_MODEL
S12_MODEL := $(COSMIC_MODEL)
endif
ifndef S12_LINKERSCRIPT
S12_LINKERSCRIPT := $(COSMIC_LINKERSCRIPT)
endif
ifndef S12_INCLUDE_C
S12_INCLUDE_C := $(COSMIC_INCLUDE_C)
endif
ifndef S12_INCLUDE_S
S12_INCLUDE_S := $(COSMIC_INCLUDE_S)
endif
ifndef S12_INCLUDE_H
S12_INCLUDE_H := $(COSMIC_INCLUDE_H)
endif
 
# Select object file format
HCS12_EXTENSION := elf

BINDIR_HCS12   := $(S12_CCDIR)/Prog

# Bin directories used for compilation
# BINDIR_ASM      - directory of the Assembler
# BINDIR_CC       - directory of the C compiler
# BINDIR_BINUTILS - directory of the binutils

BINDIR_ASM      := $(BINDIR_HCS12)
BINDIR_CC       := $(BINDIR_HCS12)
BINDIR_BINUTILS := $(BINDIR_HCS12)

ifndef EE_LINK
 EE_LINK:=$(BINDIR_BINUTILS)/piper.exe linker.exe
endif

ifndef EE_ASM
 EE_ASM:=$(BINDIR_ASM)/piper.exe ahc12.exe
endif

ifndef EE_CC
 EE_CC:=$(BINDIR_CC)/piper.exe chc12.exe
endif

ifndef EE_PREP
 EE_PREP:= $(BINDIR_CC)/piper.exe chc12.exe -LpX
endif

ifndef EE_AR
 EE_AR:=$(BINDIR_BINUTILS)/piper.exe libmaker.exe
endif

ifndef EE_OBJDUMP
 EE_OBJDUMP:=$(BINDIR_BINUTILS)/piper.exe decoder.exe -C
endif

#ifndef EE_CVDWARF
# EE_CVDWARF:=$(BINDIR_BINUTILS)/... todo
#endif

#ifndef EE_CLABS
# EE_CLABS:=$(BINDIR_BINUTILS)/... todo
#endif

# Set CPU model, only models currently supported
S12XS_CPU_MODEL = MC9S12XS64 MC9S12XS128 MC9S12XS256
S12_CPU_MODEL = MC9S12G96 MC9S12G128 MC9S12GN16 MC9S12GN32

ifeq ($(if $(filter $(S12_MODEL),$(S12XS_CPU_MODEL)),yes,) ,yes) 
CW_CPU_MODEL = -CpuHCS12X
endif
ifeq ($(if $(filter $(S12_MODEL),$(S12_CPU_MODEL)),yes,) ,yes) 
CW_CPU_MODEL = -CpuHCS12
endif

## OPT_CC are the options for compiler invocation
OPT_CC = $(CW_CPU_MODEL) -D__NO_FLOAT__ -Mb -F2 -Ccx -WmsgNu=abcde -W1
ifeq ($(call iseeopt, __EMBEDDED_CPP_SUPPORT__ ), yes)
OPT_CC += -C++e -Cppc
endif
ifeq ($(call iseeopt, DEBUG), yes)
 OPT_CC += 
else
 OPT_CC += -NoDebugInfo
endif
# User specific options from the application makefile
OPT_CC += $(CFLAGS)
#-Lm=mymake.txt

#OPT_ASM are the options for asm invocation
OPT_ASM = $(CW_CPU_MODEL) -D__NO_FLOAT__ -Mb -F2 -WmsgNu=abcde -W1
ifeq ($(call iseeopt, DEBUG), yes)
 OPT_ASM += 
else
 OPT_ASM += -NoDebugInfo
endif
# User specific options from the application makefile
OPT_ASM += $(ASFLAGS)

## OPT_LINK represents the options for armlink invocation
OPT_LINK = -WmsgNu=abcde
# User specific options from the application makefile
OPT_LINK += $(LDFLAGS)

OPT_AR = -WmsgNu=abcde
OPT_OBJDUMP = -WmsgNu=abcde

# Defining EEOPT Macros
# Each identifier that is listed in EEOPT is also inserted as a 
# command-line macro in the compiler...
DEFS_ASM = $(addprefix -D, $(EEOPT) )
DEFS_CC  = $(addprefix -D, $(EEOPT) )

# Automatic dependency generation
ifeq ($(call iseeopt, NODEPS), yes)
DEPENDENCY_OPT = 
make-depend =
else # NODEPS
ifeq ($(call iseeopt, __RTD_CYGWIN__), yes)
# Dependencies on Windows need path translation
DEPENDENCY_OPT = -LmCfg=ilmo -Lm=$(call native_path,$(subst .o,.d_tmp,$@))
#-MMD -MF $(call native_path,$(subst .o,.d_tmp,$@)) -MP -MT $@
make-depend = sed -e 's_\\\(.\)_/\1_g' -e 's_\<\([a-zA-Z]\):/_/cygdrive/\l\1/_g' < $3_tmp > $3 && rm $3_tmp
else # __RTD_CYGWIN__
DEPENDENCY_OPT = 
make-depend = 
endif # __RTD_CYGWIN__
endif # NODEPS

