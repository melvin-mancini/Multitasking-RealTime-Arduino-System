# ###*B*###
# ERIKA Enterprise - a tiny RTOS for small microcontrollers
# 
# Copyright (C) 2009  Evidence Srl
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
## IAR compiler
##
## Author: 2011,  Gianluca Franchino
##                Giuseppe Serano
##

# Select object file format
IAR_EXTENSION := out
CG_OUT_EXTENSION := $(IAR_EXTENSION)
CG_HEX_EXTENSION := hex
CG_OBJBUMP_EXTENSION := objdump

#IAR_CCDIR refers to the location of IAR libraries
BINDIR_CYG := /usr/bin
BINDIR_IAR := $(call short_native_path, $(IAR_CCDIR)/bin)

CG_LIB_DIR := $(call short_native_path, $(IAR_CCDIR)/lib)

CG_INCLUDE_DIR := $(call short_native_path, $(IAR_CCDIR)/inc/NXP)

# Bin directories used for compilation
# BINDIR_ASM      - directory of the Assembler
# BINDIR_CC       - directory of the C compiler
# BINDIR_DEP      - directory of the C compiler used for dependencies
# BINDIR_BINUTILS - directory of the binutils

BINDIR_ASM      := $(BINDIR_IAR)
BINDIR_CC       := $(BINDIR_IAR)
BINDIR_BINUTILS := $(BINDIR_IAR)
BINDIR_DEP      := $(BINDIR_IAR)


OPT_INCLUDE = $(foreach d,$(INCLUDE_PATH),$(addprefix -I,$(call short_native_path,$d)))


ifndef EE_ASM
EE_ASM:=$(BINDIR_ASM)/iasmarm.exe
endif

ifndef EE_CC
EE_CC:=$(BINDIR_CC)/iccarm.exe
endif

ifndef EE_LINK
EE_LINK:=$(BINDIR_ASM)/ilinkarm.exe
endif

ifndef EE_DEP
EE_DEP:=$(BINDIR_DEP)/iccarm.exe
endif

ifndef EE_AR
EE_AR:=$(BINDIR_BINUTILS)/iarchive.exe
endif

ifndef EE_BIN2HEX
EE_BIN2HEX:=$(BINDIR_BINUTILS)/ielftool.exe
endif

ifndef EE_OBJDUMP
EE_OBJDUMP:=$(BINDIR_BINUTILS)/ielfdumpar.exe
endif

#The IAR ELF Object Tool—iobjmanip—is used for performing low-level
#manipulation of ELF object files

ifndef EE_OBJMAN
EE_OBJMAN:=$(BINDIR_BINUTILS)/iobjmanip.exe
endif

#The IAR Absolute Symbol Exporter—isymexport—exports absolute symbols
#from a ROM image file, so that they can be used when linking an add-on
#application.

ifndef EE_ROM_EXPORT_SYM
EE_ROM_EXPORT_SYM:=$(BINDIR_BINUTILS)/isymexport.exe
endif

#Add application file to dependencies
ifneq ($(ONLY_LIBS), TRUE)

## OPT_LIBS is used to link additional libraries (e.g., for C++ support)
# the EE library is built in the current directory
## Libraries from IAR. Put here the link options --search (instead of -L)
## Option -l dooes not exist with IAR linker, just put the file name of the library
##if we need to link any standard library
OPT_LIBS += $(ERIKALIB)

ifneq ($(call iseeopt, __BIN_DISTR), yes)
OPT_LIBS += --search .
else
OPT_LIBS += --search $(EEBASE)/lib
endif

TARGET = $(TARGET_NAME).$(CG_HEX_EXTENSION) $(TARGET_NAME).$(CG_OUT_EXTENSION)

endif	# ONLY_LIBS

# INCLUDE_PATH is a colon separated list of directories for source file searching
# -I = adds directories to the source file search path (for both gcc and gas)
# we consider the ee pkg directory and the application dir
# we also consider the current directory because the app could be compiled
# from the config files generated from eclipse...
# please note the final backslash sequence after the shell command to
# avoid cygpath insering a trailing backslash
# INTERNAL_PKGBASEDIR is used to avoid multiple calls to cygpath

#INTERNAL_PKGBASEDIR := "$(shell cygpath -w $(PKGBASE))\\." "$(shell cygpath -w $(APPBASE))\\." .
#INCLUDE_PATH += $(INTERNAL_PKGBASEDIR)

ifeq ($(call iseeopt, __USE_CMSI_IAR__), yes)
IAR_CMSIS_INCPATH := $(IAR_CCDIR)/CMSIS/Include
INCLUDE_PATH += $(call native_path,$(IAR_CMSIS_INCPATH))
endif

# OPT_BIN2HEX: options for BIN2HEX
OPT_BIN2HEX = --ihex

# OPT_OBJDUMP: options for OBJDUMP
OPT_OBJDUMP = --code

# OPT_AR: options for library generation
OPT_AR = --create

## OPT_CC are the options for iar compiler invocation
#Note: all warnings are enabled by default
#Note: C is the default language
#Note: -e is to use language extentions as anonymous struct and union
#which are used, for instance, in LPC12xx.h

OPT_CC = -O -e --silent --diag_suppress Pa050 --diag_suppress Pe111
ifeq ($(call iseeopt, DEBUG), yes)
OPT_CC += --debug
endif
# Specific option from the application makefile
OPT_CC += $(CFLAGS)
OPT_CC += --cpu=Cortex-M0

##OPT_ASM are the options for asm invocation
OPT_ASM =
OPT_ASM += -S --cpu Cortex-M0 

ifeq ($(call iseeopt, DEBUG), yes)
OPT_ASM += -r
endif 

# Specific option from the application makefile
OPT_ASM += $(ASFLAGS)

## OPT_LINK represents the options for iarlinker invocation
OPT_LINK =
ifeq ($(call iseeopt, DEBUG), yes)
OPT_LINK += --semihosting
endif

OPT_LINK += --silent --cpu=CORTEX-M0

# Specific option from the application makefile
OPT_LINK += $(LDFLAGS)


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
# THIS IS THE GNU COMPILER:
#DEPENDENCY_OPT = -MMD -MF $(call native_path,$(subst .o,.d_tmp,$@)) -MP -MT $@
DEPENDENCY_OPT = --dependencies=m $(call native_path,$(subst .o,.d_tmp,$@))  
make-depend = sed -e 's_\\\(.\)_/\1_g' -e 's_\<\([a-zA-Z]\):/_/cygdrive/\l\1/_g' < $3_tmp > $3 && rm $3_tmp
else #_RTD_CYGWIN__
#DEPENDENCY_OPT = -MMD -MF $(subst .o,.d_tmp,$@) -MP -MT $@
DEPENDENCY_OPT = --dependencies=m $(subst .o,.d_tmp,$@)
make-depend = mv $3_tmp $3
endif # __RTD_CYGWIN__
endif # NODEPS
