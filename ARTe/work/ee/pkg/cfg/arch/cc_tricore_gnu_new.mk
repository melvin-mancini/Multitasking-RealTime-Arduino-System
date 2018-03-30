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
## Author: 2012,  Errico Guidieri
## Modified: 2012 Francesco Esposito (added Lin support)
##


##
## TRICORE_GNU GCC compiler version 4
## This file has been generated for HIGHTEC GCC v4.6.1.2
##

ifeq ($(call iseeopt, EE_FIND_COMPILER_IN_PATH), yes) 
## If this EE_OPT is used, the toolchain expects the compiler present in path
## This solution has been introduced because Lin does not like compiler paths
EE_LINK ?= tricore-gcc
EE_ASM  ?= tricore-gcc
EE_CC   ?= tricore-gcc
EE_AR   ?= tricore-ar
EE_OBJDUMP ?= tricore-objdump
else # EE_FIND_COMPILER_IN_PATH
## We will use "control program" gcc instead specific single tool,
## because it understand better implicit options (empirical knowledge)
## and because we can easily use HIGHTEC IDE to get the right options.
EE_LINK ?= $(TRICORE_GCCDIR)/bin/tricore-gcc
EE_ASM  ?= $(TRICORE_GCCDIR)/bin/tricore-gcc
EE_CC   ?= $(TRICORE_GCCDIR)/bin/tricore-gcc
EE_AR   ?= $(TRICORE_GCCDIR)/bin/tricore-ar
EE_OBJDUMP ?= $(TRICORE_GCCDIR)/bin/tricore-objdump
endif # EE_FIND_COMPILER_IN_PATH

# WORK AROUND TO PERSUADE LINKER TO PRESERVE SECTIONS IN SLAVE ELFs
ifeq ($(call iseeopt, __MSRP__), yes)
ifneq  ($(call iseeopt, EE_BUILD_SINGLE_ELF), yes)
ifneq ($(call iseeopt, EE_USE_CUSTOM_STARTUP_CODE), yes)
ifneq ($(CPU_NUMID), 0)
OBJS += ../$(CPU_MASTER_DIR)/$(OBJDIR)/pkg/mcu/infineon_common_tc2Yx/src/ee_tc2Yx_cstart.o
endif # CPU_NUMID not eq 0
endif # !EE_USE_CUSTOM_STARTUP_CODE
endif # !EE_BUILD_SINGLE_ELF
endif # __MSRP__

# Suffix used for Linker Scripts Files for TASKING
CC_LD_SUFFIX := ld

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

# Tricore model remapping (TASKING and TRICORE have a different naming convention for model names :( )
ifeq ($(TRICORE_MODEL),tc27x)

CFLAGS  += -D__CPU__=$(TRICORE_MODEL)
ASFLAGS += -D__CPU__=$(TRICORE_MODEL)
GNUC_TRICORE_MODEL := tc27xx
endif
ifeq ($(TRICORE_MODEL),tc26x)
CFLAGS  += -D__CPU__=$(TRICORE_MODEL)
ASFLAGS += -D__CPU__=$(TRICORE_MODEL)
GNUC_TRICORE_MODEL := tc26xx
endif

## OPT_CC are the options for compiler invocation
ifeq ($(call iseeopt, EE_MM_OPT), yes)
# -fomit-frame-pointer (neeeded to avoid that additional instructions are inserted before _START symbol),
# -ffunction-sections (needed to easily pick function in linker scripts and place them wherever you want),
# -fzero-initialized-in-bss, -finline-functions are all set in CFLAGS_tricore_gcc.txt file
OPT_CC += -c @$(call native_path,$(EEBASE))/pkg/cfg/arch/CFLAGS_tricore_gcc.txt -mcpu=$(GNUC_TRICORE_MODEL)\
 -Wdisabled-optimization -mcpu114

else # EE_MM_OPT
# -fomit-frame-pointer  is needed to avoid that additional instructions are
#                       inserted before _START symbol
# -fno-tree-loop-optimize needed to work around to a compiler bug
# TODO: TO BE ADDED -Wextra
# TODO: use -msmall-pid to introduce support for small data addressing
OPT_CC += -c -mcpu=$(GNUC_TRICORE_MODEL) -Wall -fno-common -fomit-frame-pointer\
 -fstrict-volatile-bitfields -fshort-double -Winline -finline-functions\
 -fzero-initialized-in-bss -std=gnu99 -fno-tree-loop-optimize\
 -ffunction-sections -fdata-sections -Wdiv-by-zero -Wdouble-promotion\
 -Wcast-align -Wformat-security -Wignored-qualifiers -fselective-scheduling\
 -mversion-info

ifeq ($(call iseeopt, EE_DEBUG), yes)
OPT_CC += -g3 -ggdb -O1
else # EE_DEBUG
OPT_CC += -g -O3
endif # EE_DEBUG

endif # EE_MM_OPT

ifeq ($(call iseeopt, EE_SAVE_TEMP_FILES), yes)
OPT_CC += -save-temps=obj
endif

# Specific option from the application makefile
# Add those flags at the end should be enough to give to user the option to
# completly customize build process (because last options will ovveride previous ones).
OPT_CC += $(CFLAGS)

#Functions to be used to select wrap with right options source and target for Compiler:
#Nothing special to do with source file
source_c_file=$(strip $1)
##--create stop  "control program" after object file is created
target_c_file=$(addprefix -o ,$1)


##
## OPT_ASM are the options for asm invocation
##
ifeq ($(call iseeopt, EE_MM_OPT), yes)
OPT_ASM = -c -mcpu=$(GNUC_TRICORE_MODEL) -mtc161 \
 -mcpu009 -mcpu034 -mcpu048 -mcpu050 -mcpu060 -mcpu069 -mcpu070 -mcpu072 \
 -mcpu081 -mcpu082 -mcpu083 -mcpu094 -mcpu095

else # EE_MM_OPT
OPT_ASM += -c -mcpu=$(GNUC_TRICORE_MODEL) -mtc161 -Wall -fshort-double

endif # EE_MM_OPT

ifeq ($(call iseeopt, EE_DEBUG), yes)
OPT_ASM += -Wa,--gdwarf2 
endif # EE_DEBUG


# Specific option from the application makefile
# Add those flags at the end should be enough to give to user the option to
# completly customize build process (because last options will ovveride previous ones).
OPT_ASM += $(ASFLAGS)

#Functions to be used to wrap with right options source and target for Assembler:
#Nothing special to do with source file
source_asm_file=$(strip $1)
##--create stop  "control program" after object file is created
target_asm_file=$(addprefix -o ,$1)

ifneq ($(call iseeopt, EE_AS_OSAPPLICATIONS__), yes)
# apps.conf is not generated if there is no Memory Protection
apps.conf:
	@touch $@
endif # EE_AS_OSAPPLICATIONS__ 

# Generate configuration files for Os-Applications
ifeq ($(call iseeopt, EE_AS_OSAPPLICATIONS__), yes)
# Rule to generate MemMap.h File
MemMap.h: apps.conf $(PKGBASE)/mcu/infineon_common_tc2Yx/cfg/MemMap.h.gnu.tmpl
# Preprocess MemMap.h.tmpl
	@echo GEN $@ from TEMPLATE $(word 2,$^)
	$(QUIET) awk -f $(PKGBASE)/cpu/common/cfg/memprot_generator.awk $^ > $@

# Add MemMap.h as dependency for OBJS
OBJDEP += MemMap.h
endif # EE_AS_OSAPPLICATIONS__

##
## Linker scripts templates handling rules
##
ifeq ($(and $(call iseeopt, __MSRP__), $(call iseeopt, EE_BUILD_SINGLE_ELF)), yes)
EE_LINKERSCRIPT_MODEL_FOLDER = $(PKGBASE)/mcu/infineon_$(TRICORE_MODEL)/cfg

ifeq ($(call iseeopt, EE_EXECUTE_FROM_RAM), yes)
EE_LINKERSCRIPT_PREFIX := ram
else # EE_EXECUTE_FROM_RAM
EE_LINKERSCRIPT_PREFIX := flash
endif # EE_EXECUTE_FROM_RAM

## Handle Global Linking in multicore environment with single ELF
ifeq ($(__BASE_MAKEFILE__), yes)

EE_LINKERSCRIPT := ee_$(TRICORE_MODEL)_gnu_$(EE_LINKERSCRIPT_PREFIX)_recollect.ld
EE_LINKERSCRIPT_TEMPLATE := $(PKGBASE)/mcu/infineon_common_tc2Yx/cfg/multicore/ee_tc2Yx_gnu_$(EE_LINKERSCRIPT_PREFIX)_recollect.ld.tmpl
EE_LINKERMEMORY += $(EE_LINKERSCRIPT_MODEL_FOLDER)/ee_$(TRICORE_MODEL)_gnu_memory_map.x

EE_RECOLLECT_LINKERSCRIPTS += $(PKGBASE)/mcu/infineon_common_tc2Yx/cfg/multicore/ee_tc2Yx_gnu_$(EE_LINKERSCRIPT_PREFIX)_startup_recollect.ld
EE_RECOLLECT_LINKERSCRIPTS += $(PKGBASE)/mcu/infineon_common_tc2Yx/cfg/multicore/ee_tc2Yx_gnu_$(EE_LINKERSCRIPT_PREFIX)_recollect_prefix.ld
EE_RECOLLECT_LINKERSCRIPTS += $(addsuffix ee_$(TRICORE_MODEL)_gnu_flash_recollect.ld.frag, $(EE_CORE_DIRS))
EE_RECOLLECT_LINKERSCRIPTS += $(PKGBASE)/mcu/infineon_common_tc2Yx/cfg/multicore/ee_tc2Yx_gnu_$(EE_LINKERSCRIPT_PREFIX)_recollect_suffix.ld

$(EE_LINKERSCRIPT): $(EE_LINKERMEMORY) $(EE_RECOLLECT_LINKERSCRIPTS)
	@echo GEN $@ from $(EE_LINKERSCRIPT_TEMPLATE)
	$(QUIET) cat $^ > $@

# Add Linker Script to Link Dependencies
LINKDEP += $(EE_LINKERSCRIPT)
else # __BASE_MAKEFILE__
EE_LINKERSCRIPT       := ee_$(TRICORE_MODEL)_gnu_relocable.ld
EE_LINKERSCRIPT_TEMPLATE := $(PKGBASE)/mcu/infineon_common_tc2Yx/cfg/multicore/ee_tc2Yx_gnu_relocable.ld.tmpl

EE_PARTIAL_RECOLLECT_LINKERSCRIPT := ee_$(TRICORE_MODEL)_gnu_flash_recollect.ld.frag
EE_PARTIAL_RECOLLECT_LINKERSCRIPT_TEMPLATE += $(PKGBASE)/mcu/infineon_common_tc2Yx/cfg/multicore/ee_tc2Yx_gnu_$(EE_LINKERSCRIPT_PREFIX)_recollect.ld.tmpl

$(EE_LINKERSCRIPT): apps.conf $(EE_LINKERSCRIPT_TEMPLATE)
	@echo GEN $@ from $(EE_LINKERSCRIPT_TEMPLATE)
	$(QUIET) awk -f $(PKGBASE)/cpu/common/cfg/memprot_generator.awk $^ > $@

$(EE_PARTIAL_RECOLLECT_LINKERSCRIPT): apps.conf $(EE_PARTIAL_RECOLLECT_LINKERSCRIPT_TEMPLATE)
	@echo GEN $@ from $(EE_PARTIAL_RECOLLECT_LINKERSCRIPT_TEMPLATE)
	$(QUIET) echo "" > $@;
	$(QUIET) echo "CORE_ID = CPU$(CPU_NUMID);" >> $@ ;
	$(QUIET) awk -f $(PKGBASE)/cpu/common/cfg/memprot_generator.awk $^ >> $@

#$(EE_PARTIAL_RECOLLECT_LINKERSCRIPT): $(EE_PARTIAL_RECOLLECT_LINKERSCRIPT).tbs
#	@echo GEN $@ from $<
#	$(QUIET) sed -e 's-#c#-$(CPU_NUMID)-g' $< > $@

# Add Linker Script to Link Dependencies
LINKDEP += $(EE_LINKERSCRIPT) $(EE_PARTIAL_RECOLLECT_LINKERSCRIPT)
endif # __BASE_MAKEFILE__
else # __MSRP__ && EE_BUILD_SINGLE_ELF
##
## Specific linker option from the application makefile
##
# Custom Linker Script Should be Provided with EE_LINK_SCRIPT variable
ifndef EE_LINKERSCRIPT
# Linker script start folder
EE_LINKERSCRIPT_MODEL_FOLDER = $(PKGBASE)/mcu/infineon_$(TRICORE_MODEL)/cfg

# Use The Erika Default One
ifeq ($(call iseeopt, EE_EXECUTE_FROM_RAM), yes)

EE_LINKERSCRIPT       := ee_$(TRICORE_MODEL)_gnu_ram.ld
EE_LINKERSCRIPT_TEMPLATE := $(PKGBASE)/mcu/infineon_common_tc2Yx/cfg/ee_tc2Yx_gnu_ram.ld.tmpl
EE_LINKERMEMORY += $(EE_LINKERSCRIPT_MODEL_FOLDER)/multicore/ee_$(TRICORE_MODEL)_gnu_ram_slaves_entry_points.ld
else # EE_EXECUTE_FROM_RAM

EE_LINKERSCRIPT       := ee_$(TRICORE_MODEL)_gnu_flash.ld

ifneq ($(call iseeopt, __MSRP__), yes)
EE_LINKERSCRIPT_TEMPLATE := $(addprefix $(PKGBASE)/mcu/infineon_common_tc2Yx/cfg/, ee_tc2Yx_gnu_flash_startup.ld ee_tc2Yx_gnu_flash.ld.tmpl)
else #!__MSRP__
ifeq ($(CPU_NUMID),0)
EE_LINKERSCRIPT_TEMPLATE := $(addprefix $(PKGBASE)/mcu/infineon_common_tc2Yx/cfg/, ee_tc2Yx_gnu_flash_startup.ld ee_tc2Yx_gnu_flash.ld.tmpl)
else # CPU_NUMID eq 0
EE_LINKERSCRIPT_TEMPLATE = $(EE_LINKERSCRIPT_MODEL_FOLDER)/multicore/ee_$(TRICORE_MODEL)_gnu_flash_startup_core$(CPU_NUMID).ld
EE_LINKERSCRIPT_TEMPLATE += $(PKGBASE)/mcu/infineon_common_tc2Yx/cfg/ee_tc2Yx_gnu_flash.ld.tmpl
endif # CPU_NUMID eq 0
endif # !__MSRP__
EE_LINKERMEMORY += $(EE_LINKERSCRIPT_MODEL_FOLDER)/multicore/ee_$(TRICORE_MODEL)_gnu_flash_slaves_entry_points.ld
endif # EE_EXECUTE_FROM_RAM

# The default linker script is made by a memory description part and a rules part
# (Select the right memory part in case of multicore build)
ifeq ($(call iseeopt, __MSRP__), yes)
EE_LINKERMEMORY += $(EE_LINKERSCRIPT_MODEL_FOLDER)/multicore/ee_$(TRICORE_MODEL)_gnu_memory_core$(CPU_NUMID).x
# Slave CPUs: add linkerscript with global shared symbols (No more needed)
ifneq ($(CPU_NUMID), 0)
ADDITIONAL_LINKSCRIPT = $(GLOBAL_LINKSCRIPT)
endif # CPU_NUMID not eq 0
else # __MSRP__

EE_LINKERMEMORY += $(EE_LINKERSCRIPT_MODEL_FOLDER)/ee_$(TRICORE_MODEL)_gnu_memory_singlecore.x
endif # __MSRP__

# If shared symbol container in slaves multicore build is defined, it has to be
# concatenated with linker memory descriptions and linker rules to obtain the
# complete linker script
$(EE_LINKERSCRIPT): apps.conf $(EE_LINKERMEMORY) $(EE_LINKERSCRIPT_TEMPLATE) $(ADDITIONAL_LINKSCRIPT)
	@echo GEN $@ from $(EE_LINKERSCRIPT_TEMPLATE)
	$(QUIET) awk -f $(PKGBASE)/cpu/common/cfg/memprot_generator.awk $^ > $@
else # !EE_LINKERSCRIPT

# Slave CPUs: queue linkerscript with global shared symbols (No more needed)
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
endif # !EE_LINKERSCRIPT
endif # __MSRP__ && EE_BUILD_SINGLE_ELF

# If default compiler behaviour is chosen do not add linkerscript to dependencies and
# do not add it to linker options. Moreover do not disable startup code linking.
ifneq ($(call iseeopt, EE_COMPILER_DEFAULT),yes)

# Add Linker Script to Link Dependencies
LINKDEP += $(EE_LINKERSCRIPT)

OPT_LINK += -T $(EE_LINKERSCRIPT) -nostartfiles
endif # EE_COMPILER_DEFAULT

## Handle relocating code if needed
ifeq ($(and $(call iseeopt, __MSRP__), $(call iseeopt, EE_BUILD_SINGLE_ELF)), yes)
ifneq ($(__BASE_MAKEFILE__), yes)
##Use always the export file to MASK replicated symbols
EXPORT_FILE := export_ee_global
ifeq ($(CPU_NUMID),0)
$(EXPORT_FILE): $(PKGBASE)/mcu/infineon_common_tc2Yx/cfg/multicore/$(EXPORT_FILE)
	@echo CP symbols export file $@ $<
	$(QUIET) cp -f $< $@
else # CPU_NUMID eq 0
$(EXPORT_FILE):
	@echo "EXPORT FUNCTION EE_tc2Yx_cpu$(CPU_NUMID)_start ;" > $@
endif  # CPU_NUMID eq 0

##Put the EXPORT_FILE into linking dependencies
LINKDEP += $(EXPORT_FILE)

##Relocatable linking in case of multicore
OPT_LINK += -Wl,-r -mcpu=$(GNUC_TRICORE_MODEL) -Wl,--core=CPU$(CPU_NUMID) -Wl,--export=$(EXPORT_FILE)\
 -Wl,-Map="$(basename $(notdir $@)).map" -Wl,--extmap="a"
else #  !__BASE_MAKEFILE__
#Recollection in Single elf build
OPT_LINK += -mcpu=$(GNUC_TRICORE_MODEL) -Wl,--gc-sections\
  -Wl,--mem-holes -Wl,-warn-flags -Wl,-Map="$(basename $(notdir $@)).map" -Wl,--cref -fshort-double -Wl,--extmap="a"

ifeq ($(call iseeopt, EE_DEBUG), yes)
OPT_LINK +=
endif

# Specific option from the application makefile
# Add those flags at the end should be enough to give to user the option to
# completly customize build process (because last options will ovveride previous ones).
OPT_LINK += $(LDFLAGS)
endif # !__BASE_MAKEFILE__

else # __MSRP__ && EE_BUILD_SINGLE_ELF
#Normal build
OPT_LINK += -Wl,--core=GLOBAL -mcpu=$(GNUC_TRICORE_MODEL) -Wl,--gc-sections\
  -Wl,--mem-holes -Wl,-warn-flags -Wl,-Map="$(basename $(notdir $@)).map" -Wl,--cref -fshort-double -Wl,--extmap="a"

ifeq ($(call iseeopt, EE_DEBUG), yes)
OPT_LINK +=
endif

# Specific option from the application makefile
# Add those flags at the end should be enough to give to user the option to
# completly customize build process (because last options will ovveride previous ones).
OPT_LINK += $(LDFLAGS)
endif # __MSRP__ && EE_BUILD_SINGLE_ELF

#Functions to be used to wrap with right options target for Linker:
target_ld_file=$(addprefix -o ,$1)

# Options for Archiver
OPT_AR += -r -c

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
DEPENDENCY_OPT = -MD -MF $(call native_path,$(subst .o,.d_tmp,$(@)))
# Create dependency for all headers, and add a target for each header
DEPENDENCY_OPT_ASM = -MD -MF $(call native_path,$(subst .o,.d_tmp,$(@)))
# Dependencies on Windows need path translation and quote remotion
make-depend = sed -e 's_\\\(.\)_/\1_g' -e 's_\<\([a-zA-Z]\):/_/cygdrive/\l\1/_g' < $1_tmp > $1 && rm $1_tmp
endif # NODEPS

