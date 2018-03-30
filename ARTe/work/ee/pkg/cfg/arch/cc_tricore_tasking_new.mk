# ###*B*###
# ERIKA Enterprise - a tiny RTOS for small microcontrollers
# 
# Copyright (C) 2002-2012  Evidence Srl
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
## TASKING TriCore Compiler support
##
## This file has been generated for TASKING 4.0r1. 
## You can find TASKING documentation pdf file for compiler at:
## http://www.tasking.com/support/tricore/tc_user_guide_v4.0.pdf
## In following comments, sometime, paragraph reference of this document
## are added.
##
## Author: 2012,  Errico Guidieri
##

## We will use "control program" cctc instead specific single tool,
## because it understand better implicit options (empirical knowledge)
## and because we can easily use TASKING IDE to get the right options.
EE_LINK ?= $(TRICORE_TASKINGDIR)/ctc/bin/cctc.exe
EE_ASM  ?= $(TRICORE_TASKINGDIR)/ctc/bin/cctc.exe
EE_CC   ?= $(TRICORE_TASKINGDIR)/ctc/bin/cctc.exe
EE_AR   ?= $(TRICORE_TASKINGDIR)/ctc/bin/artc.exe
EE_OBJDUMP ?= $(TRICORE_TASKINGDIR)/ctc/bin/hldumptc.exe

# HORRIBLE WORK AROUND TO PERSUADE TASKING LINKER TO PRESERVE SECTIONS IN SLAVE ELFs
ifeq ($(call iseeopt, __MSRP__), yes)
ifneq ($(CPU_NUMID), 0)
OBJS += ../$(CPU_MASTER_DIR)/$(OBJDIR)/pkg/mcu/infineon_$(TRICORE_MODEL)/src/ee_tc27x_cstart.o
endif # CPU_NUMID not eq 0
endif # __MSRP__

# Suffix used for Linker Scripts Files for TASKING
CC_LD_SUFFIX := lsl

ifneq ($(ONLY_LIBS), TRUE)
# OPT_LIBS is used to link additional libraries (e.g., for C++ support)
ifneq ($(call iseeopt, __BIN_DISTR), yes) 
OPT_LIBS +=
else  # EEOPT __BIN_DISTR
OPT_LIBS +=
endif # EEOPT __BIN_DISTR
endif # ONLY_LIBS

# INCLUDE_PATH is a colon separated list of directories for source file searching
# -I = adds directories to the source file search path (for both gcc and gas)
# we consider the ee pkg directory and the application dir
# we also consider the current directory because the app could be compiled
# from the config files generated from eclipse...
OPT_INCLUDE = $(foreach d,$(INCLUDE_PATH),$(addprefix -I,$(call native_path,$d)))

## OPT_CC are the options for compiler invocation
## for the meaning of the options look at documentation paragraph 
## 11.6. Control Program Options
OPT_CC += -C$(TRICORE_MODEL) -t -Wa-gAHLs -Wa-Ogs -Wa--error-limit=42 --no-preprocessing-only -E+comments,+noline \
--iso=99 --language=+gcc,-volatile,+strings --switch=auto --align=0 \
--default-near-size=8 --default-a0-size=0 --default-a1-size=0 --tradeoff=4 --compact-max-size=200 --source

ifeq ($(call iseeopt, EE_DEBUG), yes)
OPT_CC += --emit-locals=+equ,+symbols -g -O1
else
OPT_CC += --emit-locals=-equ,-symbols -O3
endif

## MISRA-C CHECK Options. This configuration enable required rules only as warning.
ifeq ($(call iseeopt, EE_MISRA_C_CHECK_ON),yes)
OPT_CC += --misrac="1.1,1.2,2.1,2.2,2.3,3.4,3.5,4.1,4.2,5.1,5.2,5.3,5.4,6.1,6.2,6.4,6.5,\
7.1,8.1,8.2,8.3,8.4,8.5,8.6,8.7,8.8,8.9,8.11,8.12,9.1,9.2,9.3,10.1,10.2,10.3,10.4,10.5,10.6,\
11.1,11.2,11.5,12.2,12.3,12.4,12.5,12.7,12.8,12.9,12.10,12.12,13.1,13.3,13.4,13.5,13.6,13.7,\
14.1,14.2,14.3,14.4,14.5,14.6,14.7,14.8,14.9,14.10,15.1,15.2,15.3,15.4,15.5,\
16.1,16.2,16.3,16.4,16.5,16.6,16.8,16.9,16.10,17.3,17.4,17.6,18.1,18.2,18.4,\
19.3,19.4,19.5,19.6,19.8,19.9,19.10,19.11,19.12,19.14,19.15,19.16,19.17,\
20.1,20.2,20.4,20.5,20.6,20.7,20.8,20.9,20.10,20.11,20.12"
OPT_CC += --misrac-version=2004 --misrac-required-warnings --misrac-advisory-warnings 
endif

# Specific option from the application makefile
# Add those flags at the end should be enough to give to user the option to
# completly customize build process (because last options will ovveride previous ones).
OPT_CC += $(CFLAGS)

#Functions to be used to select wrap with right options source and target for Compiler:
#Nothing special to do with source file
source_c_file=$(strip $1)
##--create stop  "control program" after object file is created
target_c_file=$(addprefix --create --output=,$1)

#For ASM I leave --tasking-sfr option active
OPT_ASM += -C$(TRICORE_MODEL) -t -Wa--tasking-sfr -Wa-gAHLs -Wa-Ogs -Wa--error-limit=42

ifeq ($(call iseeopt, EE_DEBUG), yes)
OPT_ASM += --emit-locals=+equ,+symbols
else
OPT_ASM += --emit-locals=-equ,-symbols
endif

# Specific option from the application makefile
# Add those flags at the end should be enough to give to user the option to
# completly customize build process (because last options will ovveride previous ones).
OPT_ASM += $(ASFLAGS)

#Functions to be used to wrap with right options source and target for Assembler:
#Nothing special to do with source file
source_asm_file=$(strip $1)
##--create stop  "control program" after object file is created
target_asm_file=$(addprefix --create --output=,$1)

# Custom Linker Script Should be Provided with EE_LINKERSCRIPT variable
ifndef EE_LINKERSCRIPT
# Use The Erika Default One
ifeq ($(call iseeopt, EE_EXECUTE_FROM_RAM), yes)
EE_LINKERSCRIPT := ee_tc27x_tasking_ram.lsl
else
EE_LINKERSCRIPT := ee_tc27x_tasking_flash.lsl
endif
$(EE_LINKERSCRIPT) : % : $(PKGBASE)/mcu/infineon_$(TRICORE_MODEL)/cfg/%
	@echo CP $@
	$(QUIET) cp $< $@
endif
# Add Linker Script to Link Dependencies
LINKDEP += $(EE_LINKERSCRIPT)

OPT_LINK += -C$(TRICORE_MODEL) -t $(EE_LINKERSCRIPT) -Wl-O1 -Wl--map-file=$(TARGET_NAME).mapxml:XML -Wl-m2 -Wl--error-limit=42 -Wl--no-warnings=159 -g
#no warning is needed for following warning message in TC0:
#ltc W159: LSL: section ".text.libc.reset" (function _START) was not selected because it already has an absolute restriction

ifeq ($(call iseeopt, EE_DEBUG), yes)
OPT_LINK +=
endif

# Option to execute code and locate data structures in RAM 
ifeq ($(call iseeopt, EE_EXECUTE_FROM_RAM), yes)
#-Wl--non-romable SHOULD BE ADDED to the options but it doens't work
#(conflict with INTTAB & TRAPTAB) -> TASKING BUG
OPT_LINK += --no-rom-copy -Wl-DEE_EXECUTE_FROM_RAM
endif

#Defined when multicore support is needed, used to preprocess Multicore Linker Script
ifdef CPU_NUMID
OPT_LINK += -Wl-DCPU_NUMID=$(CPU_NUMID)
endif

# Specific option from the application makefile
# Add those flags at the end should be enough to give to user the option to
# completly customize build process (because last options will ovveride previous ones).
OPT_LINK += $(LDFLAGS)

#Functions to be used to wrap with right options target for Linker:
target_ld_file=$(addprefix -o ,$1)

# Options for Archiver
OPT_AR += -r -c

# Option for OBJDUMP Command (the following is the default and it's ok, added just to remember that)
OPT_OBJDUMP += --dump-format=dhmsy

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
# Create dependency for all headers, and add a target for each header
DEPENDENCY_OPT = --dep-file=$(call native_path,$(subst .o,.d_tmp,$(@))) -Wc--make-target=$(@)
# Create dependency for all headers, and add a target for each header
DEPENDENCY_OPT_ASM = -Wa--dep-file=$(call native_path,$(subst .o,.d_tmp,$(@))) -Wa--make-target=$(@)
# Dependencies on Windows need path translation and quote remotion
make-depend = sed -e 's_"__g' -e 's_\\\(.\)_/\1_g' -e 's_\<\([a-zA-Z]\):/_/cygdrive/\l\1/_g' < $1_tmp > $1 && rm $1_tmp
endif # NODEPS

