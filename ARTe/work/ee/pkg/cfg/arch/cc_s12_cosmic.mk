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

##
## Cosmic compiler
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
HCS12_EXTENSION := s12

BINDIR_HCS12   := $(S12_CCDIR)

# Bin directories used for compilation
# BINDIR_ASM      - directory of the Assembler
# BINDIR_CC       - directory of the C compiler
# BINDIR_DEP      - directory of the C compiler used for dependencies
# BINDIR_BINUTILS - directory of the binutils

BINDIR_ASM      := $(BINDIR_HCS12)
BINDIR_CC       := $(BINDIR_HCS12)
BINDIR_BINUTILS := $(BINDIR_HCS12)
BINDIR_DEP      := $(BINDIR_HCS12)

ifndef EE_LINK
EE_LINK:=$(BINDIR_BINUTILS)/clnk.exe
endif

ifndef EE_ASM
EE_ASM:=$(BINDIR_ASM)/cas12x.exe
endif

ifndef EE_CC
EE_CC:=$(BINDIR_CC)/cxs12x.exe
endif

ifndef EE_DEP
EE_DEP:=$(BINDIR_DEP)/cxs12x.exe
endif

ifndef EE_PREP
EE_PREP:= $(BINDIR_CC)/cps12x.exe
endif

ifndef EE_AR
EE_AR:=$(BINDIR_BINUTILS)/clib.exe
endif

ifndef EE_CVDWARF
EE_CVDWARF:=$(BINDIR_BINUTILS)/cvdwarf.exe
endif

#ifndef EE_OBJCOPY
#EE_OBJCOPY:=$(BINDIR_BINUTILS)/objcopy
#endif

ifndef EE_CLABS
EE_CLABS:=$(BINDIR_BINUTILS)/clabs.exe
endif

# ALLINCPATH is a colon separated list of directories for source file searching
# -I = adds directories to the source file search path (for both gcc and gas)
# we consider the ee pkg directory and the application dir
# we also consider the current directory because the app could be compiled
# from the config files generated from eclipse...
# please note the final backslash sequence after the shell command to
# avoid cygpath insering a trailing backslash
# INTERNAL_PKGBASEDIR is used to avoid multiple calls to cygpath

INTERNAL_PKGBASEDIR := -i"$(shell cygpath -w $(PKGBASE))\\." -i"$(shell cygpath -w $(APPBASE))\\." -i.
ALLINCPATH += $(INTERNAL_PKGBASEDIR)

## OPT_CC are the options for Cosmic C-compiler invocation
ifeq ($(call iseeopt, DEBUG), yes)
OPT_CC += +debug
endif
# Specific option from the application makefile
OPT_CC += $(CFLAGS)

## OPT_CC_DEPS are the options for dependencies calculation
OPT_CC_DEPS := $(OPT_CC) -sm

## OPT_ASM are the options for Cosmic assembler invocation
OPT_ASM = 
ifeq ($(call iseeopt, DEBUG), yes)
OPT_ASM += -xx -x
endif
# Specific option from the application makefile
OPT_ASM += $(ASFLAGS)

## OPT_LINK represents the options for Cosmic linker invocation
OPT_LINK = 
# Specific option from the application makefile
OPT_LINK += $(LDFLAGS)

# Defining EEOPT Macros
# Each identifier that is listed in EEOPT is also inserted as a 
# command-line macro in the compiler...

DEFS_ASM = $(addprefix -d, $(EEOPT) )
DEFS_GCCASM = $(addprefix -D, $(EEOPT) )
DEFS_CC  = $(addprefix -d, $(EEOPT) ) 

# Automatic dependency generation
ifeq ($(call iseeopt, NODEPS), yes)
DEPENDENCY_OPT =
DEPENDENCY_OPT_CPS12X =
make-depend =
else # NODEPS
ifeq ($(call iseeopt, __RTD_CYGWIN__), yes)
# Dependencies on Windows need path translation
DEPENDENCY_OPT = -sm
DEPENDENCY_OPT_CPS12X = -md
make-depend = sed -e 's_\\\(.\)_/\1_g' -e 's_\<\([a-zA-Z]\):/_/cygdrive/\l\1/_g' < $3_tmp > $3 && rm $3_tmp
else # __RTD_CYGWIN__
DEPENDENCY_OPT = 
DEPENDENCY_OPT_CPS12X =
make-depend = 
endif # __RTD_CYGWIN__
endif # NODEPS


