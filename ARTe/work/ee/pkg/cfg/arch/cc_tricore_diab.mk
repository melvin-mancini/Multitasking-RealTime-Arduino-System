# ###*B*###
# ERIKA Enterprise - a tiny RTOS for small microcontrollers
# 
# Copyright (C) 2002-2013  Evidence Srl
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
## Author: 2013,  Errico Guidieri
##


##
## TRICORE_DIAB compiler version 5.9.2.0
## This file has been generated for WindRiver Diad v5.9.2.0
##

ifeq ($(call iseeopt, EE_FIND_COMPILER_IN_PATH), yes) 
## If this EE_OPT is used, the toolchain expects the compiler present in path
## This solution has been introduced because Lin does not like compiler paths
EE_CC   ?= dcc
EE_ASM  ?= das
EE_LINK ?= dld
EE_AR   ?= dar
EE_OBJDUMP ?= ddump
else # EE_FIND_COMPILER_IN_PATH
## We will use "control program" gcc instead specific single tool,
## because it understand better implicit options (empirical knowledge)
## and because we can easily use HIGHTEC IDE to get the right options.
EE_CC   ?= $(TRICORE_DIABDIR)bin/dcc
EE_ASM  ?= $(TRICORE_DIABDIR)bin/das
EE_LINK ?= $(TRICORE_DIABDIR)bin/dld
EE_AR   ?= $(TRICORE_DIABDIR)bin/dar
EE_OBJDUMP ?= $(TRICORE_DIABDIR)bin/ddump
endif # EE_FIND_COMPILER_IN_PATH

# Suffix used for Linker Scripts Files for TASKING
CC_LD_SUFFIX := dld

ifneq ($(ONLY_LIBS), TRUE)
# OPT_LIBS is used to link additional libraries (e.g., for C++ support)
ifneq ($(call iseeopt, __BIN_DISTR), yes) 
OPT_LIBS +=
else  # EEOPT __BIN_DISTR
OPT_LIBS +=
endif # EEOPT __BIN_DISTR
endif # ONLY_LIBS

# INCLUDE_PATH is a colon separated list of directories for source file searching
# -I = adds directories to the source file search path (for both dcc and das)
# we consider the ee pkg directory and the application dir
# we also consider the current directory because the app could be compiled
# from the config files generated from eclipse...
OPT_INCLUDE = $(foreach d,$(INCLUDE_PATH),$(addprefix -I,$(call native_path,$d)))

# Tricore model remapping (TASKING and TRICORE have a different naming convention for model names :( )
ifeq ($(TRICORE_MODEL),tc27x)

CFLAGS  += -D__CPU__=$(TRICORE_MODEL)
ASFLAGS += -D__CPU__=$(TRICORE_MODEL)
DIAB_TRICORE_TARGET := -tTC27xAstep16PNG:simple
#DIAB_TRICORE_TARGET := -tTC16NF:simple
endif

## OPT_CC are the options for compiler invocation
# possible warnings that could be ignored -ei5388,2273,5387 -ei4177,4550,4549,4068,5388 -ew4084
# -Xstderr-fully-buffered
# Suppresed Warnings:
# i5388 - cast from Integer to pointer of same dimension
# (etoa:1573): #pragma weak is incompatible with -Xlocal-data-area: turning off LDA optimization

OPT_CC += -c -Xkeep-assembly-file=2 $(DIAB_TRICORE_TARGET) -D__TC161__ -ei5388,2273,5387,1573 -Xintrinsic-mask=0x2\
 -Xc-new -Xdialect-c99 -Xsection-split=3 -Xdebug-inline-on  -Xinline=64 -Xbss-common-off\
 -Xunroll-size=200 -Xname-const=.rodata -Xname-uconst=.bss -Xmacro-in-pragma

ifeq ($(call iseeopt, __EE_MEMORY_PROTECTION__), yes)
# handling small data with memory protection is a little pain and nobody paid us
# to do that
OPT_CC += -Xsmall-data=0
endif #__EE_MEMORY_PROTECTION__

ifeq ($(call iseeopt, EE_DEBUG), yes)
OPT_CC += -Xdiagnose-inline-verbose -g -O
else # EE_DEBUG
OPT_CC += -g3 -XO
endif # EE_DEBUG

# If we are trying to put the application in RAM we have to optimize for 
ifeq ($(call iseeopt, EE_EXECUTE_FROM_RAM), yes)
OPT_CC += -Xsize-opt
endif # EE_EXECUTE_FROM_RAM

# Specific option from the application makefile
# Add those flags at the end should be enough to give to user the option to
# completely customize build process (because last options will override previous ones).
OPT_CC += $(CFLAGS)

#Functions to be used to select wrap with right options source and target for Compiler:
#Nothing special to do with source file
source_c_file=$(strip $1)
##--create stop  "control program" after object file is created
target_c_file=$(addprefix -o ,$1)

# Specific option from the application makefile
# Add those flags at the end should be enough to give to user the option to
# completely customize build process (because last options will override previous ones).
OPT_ASM += $(ASFLAGS)

#Functions to be used to wrap with right options source and target for Assembler:
#Nothing special to do with source file
source_asm_file=$(strip $1)
##--create stop  "control program" after object file is created
target_asm_file=$(addprefix -o ,$1)

##
## Templates handling target and rules
##

ifneq ($(call iseeopt, EE_AS_OSAPPLICATIONS__), yes)
# apps.conf is not generated if there is no Memory Protection
apps.conf:
	@touch $@
endif # EE_AS_OSAPPLICATIONS__

# Generate configuration files for Os-Applications
ifeq ($(call iseeopt, EE_AS_OSAPPLICATIONS__), yes)
# Rule to generate MemMap.h File
MemMap.h: apps.conf $(PKGBASE)/mcu/infineon_$(TRICORE_MODEL)/cfg/MemMap.h.diab.tmpl
# Preprocess MemMap.h.tmpl
	@echo GEN $@ from TEMPLATE $(word 2,$^)
	$(QUIET) awk -f $(PKGBASE)/cpu/common/cfg/memprot_generator.awk $^ > $@

# Add MemMap.h as dependency for OBJS
OBJDEP += MemMap.h
endif # EE_AS_OSAPPLICATIONS__

##
## Specific linker option from the application makefile
##

# Custom Linker Script Should be Provided with EE_LINK_SCRIPT variable
ifndef EE_LINKERSCRIPT
# Use The Erika Default One
ifeq ($(call iseeopt, EE_EXECUTE_FROM_RAM), yes)
EE_LINKERSCRIPT := ee_tc27x_diab_ram.dld
else # EE_EXECUTE_FROM_RAM
EE_LINKERSCRIPT := ee_tc27x_diab_flash.dld
endif # EE_EXECUTE_FROM_RAM

# Linker script start folder
EE_LINKERSCRIPT_FOLDER = $(PKGBASE)/mcu/infineon_$(TRICORE_MODEL)/cfg
# Linker Script Template
EE_LINKERSCRIPT_TEMPLATE = $(EE_LINKERSCRIPT_FOLDER)/$(EE_LINKERSCRIPT).tmpl

# The default linker script is made by a memory description part and a rules part
# (Select the right memory part in case of multicore build)
ifeq ($(call iseeopt, __MSRP__), yes)
EE_LINKERMEMORY = $(EE_LINKERSCRIPT_FOLDER)/multicore/ee_tc27x_diab_memory_core$(CPU_NUMID).x
# Slave CPUs: add linkerscript with global shared symbols
ifneq ($(CPU_NUMID), 0)
ADDITIONAL_LINKSCRIPT = $(GLOBAL_LINKSCRIPT)
endif # CPU_NUMID not eq 0
else # __MSRP__
EE_LINKERMEMORY = $(EE_LINKERSCRIPT_FOLDER)/ee_tc27x_diab_memory_singlecore.x
endif # __MSRP__

# Handle Copytable command generation
ifeq ($(call iseeopt, EE_EXECUTE_FROM_RAM), yes)
HANDLE_COPY_TABLE = $(QUIET) echo "/* -Xgenerate-copytables  When you work directly on RAM you don't need copy tables */" > $@
else # EE_EXECUTE_FROM_RAM
HANDLE_COPY_TABLE = $(QUIET) echo -Xgenerate-copytables > $@
endif # EE_EXECUTE_FROM_RAM

# If shared symbol container in slaves multicore build is defined, it has to be
# concatenated with linker memory descriptions and linker rules to obtain the
# complete linker script
$(EE_LINKERSCRIPT): apps.conf $(EE_LINKERMEMORY) $(EE_LINKERSCRIPT_TEMPLATE) $(ADDITIONAL_LINKSCRIPT)
	@echo GEN $@ from TEMPLATE $(notdir $(EE_LINKERSCRIPT_TEMPLATE))
	$(HANDLE_COPY_TABLE)
	$(QUIET) awk -f $(PKGBASE)/cpu/common/cfg/memprot_generator.awk $^ >> $@
else # EE_LINKERSCRIPT

# Slave CPUs: queue linkerscript with global shared symbols
ifeq ($(call iseeopt, __MSRP__), yes)
ifneq ($(CPU_NUMID), 0)
ADDITIONAL_LINKSCRIPT = $(GLOBAL_LINKSCRIPT)
endif # CPU_NUMID not eq 0
endif # __MSRP__

# Copy EE_LINKERSCRIPT in building folder eventually enqueuing shared symbols
# informations
$(EE_LINKERSCRIPT) : $(APPBASE)/$(EE_LINKERSCRIPT) $(ADDITIONAL_LINKSCRIPT)
	@echo GEN $@
	$(QUIET) rm -f $@
	$(QUIET) cat $^ >> $@

endif # EE_LINKERSCRIPT

# Add the Linker Script as dependency for linking process
LINKDEP += $(EE_LINKERSCRIPT)

# I use -m6 format because is not possible to save a file from dld
# -Xcompress-debug-info: This option make dld to crash
# -Xremove-unused-sections: This make dld to return with a return value
#   equal to 127 that is different to zero
OPT_LINK += $(EE_LINKERSCRIPT) $(DIAB_TRICORE_TARGET) -m30 -lc -limpl\
 -Xcombine-readonly-sections  -Xcheck-overlapping -Xremove-unused-sections

# Redirect Linker Output to create map file
LINK_REDIRECT = > "$(basename $(notdir $@)).map"

ifeq ($(call iseeopt, EE_DEBUG), yes)
OPT_LINK +=
endif

# Specific option from the application makefile
# Add those flags at the end should be enough to give to user the option to
# completely customize build process (because last options will override previous ones).
OPT_LINK += $(LDFLAGS)

#Functions to be used to wrap with right options target for Linker:
target_ld_file=$(addprefix -o ,$1)

# Options for Archiver
OPT_AR += rs

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
# Create dependency for all headers, and add a target for each header
DEPENDENCY_OPT = -Xmake-dependency=d -Xmake-dependency-target=$@ -Xmake-dependency-savefile=$(call native_path,$(subst .o,.d_tmp,$@))
# Dependencies on Windows need path translation and quote remotion
make-depend = sed -e 's_\\\(.\)_/\1_g' -e 's_\<\([a-zA-Z]\):/_/cygdrive/\l\1/_g' < $1_tmp > $1 && rm $1_tmp
endif # NODEPS

