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
## Texas Intruments Code Composer Studio TMS 470 compiler
##
## Author: 2011,  Giuseppe Serano
##

# Select object file format
CG_OUT_EXTENSION := out
CG_HEX_EXTENSION := hex
CG_BIN_EXTENSION := bin
CG_OBJDUMP_EXTENSION := xml

BINDIR_CYG := /usr/bin

#CCS_INSTALL_ROOT: Code Composer Studio Installation Root Folder.
#                  Default "C:\Programmi\Texas Instruments\ccsv4".
CG_TOOL_SUFFIX := 470
ifeq ($(call iseeopt, __CCS_4_2_4_00033_OLDER__), yes)
CG_TOOL_ROOT := $(call short_native_path, $(CCS_INSTALL_ROOT)/tools/compiler/tms$(CG_TOOL_SUFFIX))
else
ifeq ($(call iseeopt, __CCS_4_2_5_00005_TMS470_4_9_3__), yes)
CG_TOOL_ROOT := $(call short_native_path, $(CCS_INSTALL_ROOT)/tools/compiler/TMS470 Code Generation Tools 4.9.3)
else
CG_TOOL_ROOT := $(call short_native_path, $(CCS_INSTALL_ROOT)/tools/compiler/TMS470 Code Generation Tools 4.9.7)
endif
endif

CG_BIN_DIR := $(CG_TOOL_ROOT)/bin

CG_BINUTILS_DIR := $(call short_native_path, $(CCS_INSTALL_ROOT)/utils/tiobj2bin)

CG_LIB_DIR := $(CG_TOOL_ROOT)/lib

CG_INCLUDE_DIR := $(CG_TOOL_ROOT)/include

ifdef TMPDIR
CG_TMPDIR := $(call short_native_path, $(TMPDIR))
else
CG_TMPDIR := $(call short_native_path, .)
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

OPT_INCLUDE = $(foreach d,$(INCLUDE_PATH),$(addprefix --include_path=,$(call short_native_path,$d)))

ifndef EE_ASM
EE_ASM:=$(BINDIR_ASM)/cl$(CG_TOOL_SUFFIX).exe
endif

ifndef EE_CC
EE_CC:=$(BINDIR_CC)/cl$(CG_TOOL_SUFFIX).exe
endif

ifndef EE_LINK
EE_LINK:=$(CG_BIN_DIR)/cl$(CG_TOOL_SUFFIX).exe
endif

ifndef EE_DEP
EE_DEP:=$(BINDIR_DEP)/cl$(CG_TOOL_SUFFIX).exe
endif

ifndef EE_AR
EE_AR:=$(CG_BIN_DIR)/ar$(CG_TOOL_SUFFIX).exe
endif

ifndef EE_OBJDUMP
EE_OBJDUMP:=$(CG_BIN_DIR)/ofd$(CG_TOOL_SUFFIX).exe
endif

ifndef EE_OBJDUMP2HEX
EE_OBJDUMP2HEX:=$(BINDIR_BINUTILS)/mkhex4bin.exe
endif

ifndef EE_HEX2BIN
EE_HEX2BIN:=$(CG_BIN_DIR)/hex$(CG_TOOL_SUFFIX).exe
endif

#Add application file to dependencies
ifneq ($(ONLY_LIBS), TRUE)

## OPT_LIBS is used to link additional libraries (e.g., for C++ support)
## Libraries from TI-CCS-TMS470. Put here the link options -i (instead of -L).
OPT_LIBS += -l$(ERIKALIB) -i$(call native_path, $(CG_LIB_DIR))

ifneq ($(call iseeopt, __BIN_DISTR), yes)
OPT_LIBS += -i .
else
OPT_LIBS += -i $(EEBASE)/lib
endif

TARGET = $(TARGET_NAME).$(CG_BIN_EXTENSION)

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

# OPT_HEX2BIN: options for HEX2BIN
OPT_HEX2BIN := -q -b -image

# OPT_OBJDUMP2HEX: options for OBJDUMP2HEX
OPT_OBJDUMP2HEX :=

# OPT_OBJDUMP: options for OBJDUMP
OPT_OBJDUMP := -x --xml_indent=0 --obj_display=none,sections,header,segments

# OPT_AR: options for library generation
OPT_AR = r

## OPT_CC are the options for c compiler invocation
#Note: all warnings are enabled by default
#Note: C is the default language

ifeq ($(call iseeopt, __CORTEX_M4__), yes)
OPT_CC += -mv7M4
ifeq ($(call iseeopt, __CCS_4_2_4_00033_OLDER__), yes)
OPT_CC += --float_support=vfplib
else
OPT_CC += --float_support=FPv4SPD16
endif
endif

ifeq ($(call iseeopt, DEBUG), yes)
OPT_CC += -g
endif

OPT_CC +=  -c --gcc --define=ccs --diag_warning=225 -me \
	   --gen_func_subsections=on --abi=eabi --code_state=16 \
	   --ual --quiet --temp_directory=$(CG_TMPDIR)

ifeq ($(call iseeopt, __LM4F232xxxx__), yes)
OPT_CC += --define=PART_LM4F232H5QD --define=TARGET_IS_BLIZZARD_RA1
endif

# Specific option from the application makefile
OPT_CC += $(CFLAGS)

##OPT_ASM are the options for assembler invocation
ifeq ($(call iseeopt, __CORTEX_M4__), yes)
OPT_ASM += -mv7M4
ifeq ($(call iseeopt, __CCS_4_2_4_00033_OLDER__), yes)
OPT_ASM += --float_support=vfplib
else
OPT_ASM += --float_support=FPv4SPD16
endif
endif

ifeq ($(call iseeopt, DEBUG), yes)
OPT_ASM += -g
endif 

OPT_ASM += -c --gcc --define=ccs --diag_warning=225 -me \
	   --gen_func_subsections=on --abi=eabi --code_state=16 \
	   --ual --quiet --asm_extension=.s --temp_directory=$(CG_TMPDIR)

ifeq ($(call iseeopt, __LM4F232xxxx__), yes)
OPT_ASM += --define=PART_LM4F232H5QD --define=TARGET_IS_BLIZZARD_RA1
endif

# Specific option from the application makefile
OPT_ASM += $(ASFLAGS)

## OPT_LINK represents the options for linker invocation
ifeq ($(call iseeopt, __CORTEX_M4__), yes)
OPT_LINK += -mv7M4
ifeq ($(call iseeopt, __CCS_4_2_4_00033_OLDER__), yes)
OPT_LINK += --float_support=vfplib
else
OPT_LINK += --float_support=FPv4SPD16
endif
endif

ifeq ($(call iseeopt, DEBUG), yes)
OPT_LINK += -g
endif

# FIXME: STACK_SIZE.
OPT_LINK += --diag_warning=225 -me --gen_func_subsections=on --abi=eabi \
	    --code_state=16 -z --define=ccs --reread_libs --rom_model \
	    --warn_sections

ifeq ($(call iseeopt, __LM4F232xxxx__), yes)
OPT_LINK += --define=PART_LM4F232H5QD --define=TARGET_IS_BLIZZARD_RA1
endif

ifeq ($(call iseeopt, __CORTEX_M4_CCS_SYS_STACK_4K__), yes)
OPT_LINK += --stack_size=4096
else
ifeq ($(call iseeopt, __CORTEX_M4_CCS_SYS_STACK_2K__), yes)
OPT_LINK += --stack_size=2048
else
OPT_LINK += --stack_size=1024
endif
endif

ifeq ($(call iseeopt, __CORTEX_M4_CCS_SYS_HEAP_2K__), yes)
OPT_LINK += --heap_size=2048
else
ifeq ($(call iseeopt, __CORTEX_M4_CCS_SYS_HEAP_1K__), yes)
OPT_LINK += --heap_size=1024
else
OPT_LINK += --heap_size=0
endif
endif

# TODO: Generate *.map file (OBJDUMP -> odf470)
#	   -m$(call native_path, $(subst .$(CG_OUT_EXTENSION), .map, $@))

# Specific option from the application makefile
OPT_LINK += $(LDFLAGS)


# Defining EEOPT Macros
# Each identifier that is listed in EEOPT is also inserted as a 
# command-line macro in the compiler...

DEFS_ASM = $(addprefix --define=, $(EEOPT))
DEFS_CC  = $(addprefix --define=, $(EEOPT))


# Automatic dependency generation
ifeq ($(call iseeopt, NODEPS), yes)
DEPENDENCY_OPT = 
make-depend =
else # NODEPS
ifeq ($(call iseeopt, __RTD_CYGWIN__), yes)
# Dependencies on Windows need path translation
# THIS IS THE GNU COMPILER:
#DEPENDENCY_OPT = -MMD -MF $(call native_path,$(subst .o,.d_tmp,$@)) -MP -MT $@
DEPENDENCY_OPT = --preproc_with_compile \
		 --preproc_dependency=$(call native_path,$(subst .o,.d_tmp,$@))
make-depend = sed -e 's_\\\(.\)_/\1_g' \
	      -e 's_\<\([a-zA-Z]\):/_/cygdrive/\l\1/_g' < $3_tmp \
	      > $3 && rm $3_tmp
else # __RTD_CYGWIN__
#DEPENDENCY_OPT = -MMD -MF $(subst .o,.d_tmp,$@) -MP -MT $@
DEPENDENCY_OPT = --preproc_with_compile \
	         --preproc_dependency=$(subst .o,.d_tmp,$@)
make-depend = mv $3_tmp $3
endif # __RTD_CYGWIN__
endif # NODEPS
