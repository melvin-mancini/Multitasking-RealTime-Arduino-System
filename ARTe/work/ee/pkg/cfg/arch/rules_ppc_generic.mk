# ###*B*###
# ERIKA Enterprise - a tiny RTOS for small microcontrollers

# Copyright (C) 2002-2011  Evidence Srl

# This file is part of ERIKA Enterprise.

# ERIKA Enterprise is free software; you can redistribute it
# and/or modify it under the terms of the GNU General Public License
# version 2 as published by the Free Software Foundation,
# (with a special exception described below).

# Linking this code statically or dynamically with other modules is
# making a combined work based on this code.  Thus, the terms and
# conditions of the GNU General Public License cover the whole
# combination.

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

# ERIKA Enterprise is distributed in the hope that it will be
# useful, but WITHOUT ANY WARRANTY; without even the implied warranty
# of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License version 2 for more details.

# You should have received a copy of the GNU General Public License
# version 2 along with ERIKA Enterprise; if not, write to the
# Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
# Boston, MA 02110-1301 USA.
# ###*E*###

## Author: 2010 Fabio Checconi
## 2010-2011 Bernardo  Dal Seno
## 2013 Eugen Kleinschmidt

# Enable verbose output from EE_OPT
ifeq ($(call iseeopt, VERBOSE), yes)
VERBOSE = 1
endif

# VLE
USE_VLE:=$(if $(call iseeopt, __VLE__),1,0)
#ifeq ($(call iseeopt, __VLE__), yes)
#USE_VLE = 1
#else
#USE_VLE = 0
#endif

# For compatibility with old code
ifeq ($(call iseeopt, __E200Z7_EXECUTE_FROM_RAM__), yes)
EEOPT +=  __E200ZX_EXECUTE_FROM_RAM__
endif

# MCU
ifeq ($(call iseeopt, __MPC5674F__), yes)
PPC_MCU_MODEL = mpc5674f
T32_FLASH_BIN = c90fl5674.bin
endif
ifeq ($(call iseeopt, __MPC5668G__), yes)
# Only `G' version is supported
EEOPT += __MPC5668__
endif
ifeq ($(call iseeopt, __MPC5668__), yes)
PPC_MCU_MODEL = mpc5668
T32_FLASH_BIN = c90fl5668.bin
endif
ifeq ($(call iseeopt, EE_MPC5643L), yes)
PPC_MCU_MODEL = mpc5643l
T32_FLASH_BIN = c90fl564xl.bin
ifeq ($(call iseeopt, LOCK_STEP), yes)
# MPC5643L supports Lock step mode
LS_FLAG = _lock_step
LOCK_STEP = 1
else
LOCK_STEP = 0
LS_FLAG =
endif
endif
ifeq ($(call iseeopt, EE_MPC5644A), yes)
PPC_MCU_MODEL = mpc5644a
T32_FLASH_BIN = c90fl5674.bin
endif
ifndef PPC_MCU_MODEL
$(error No known PPC MCU model found in EE_OPT)
endif

include $(EEBASE)/pkg/cfg/dir.mk

# Read MCU-specific file, if it exists, but don't make it
-include $(PKGBASE)/cfg/arch/rules_ppc_$(PPC_MCU_MODEL).mk
$(PKGBASE)/cfg/arch/rules_ppc_$(PPC_MCU_MODEL).mk: ;

include $(PKGBASE)/cfg/verbose.mk
include $(PKGBASE)/cfg/compiler.mk

ifeq ($(call iseeopt, __MSRP__), yes)
SCRIPT_SUBPATH := multicore/
endif
ifeq ($(call iseeopt, __E200ZX_EXECUTE_FROM_RAM__), yes)
BASE_LD_SCRIPT := $(SCRIPT_SUBPATH)ram$(CPU_NUMID)$(CC_LD_SUFFIX)
T32CMM_SRC := $(SCRIPT_SUBPATH)ram$(CPU_NUMID).cmm
else
BASE_LD_SCRIPT := $(SCRIPT_SUBPATH)rom$(CPU_NUMID)$(CC_LD_SUFFIX)
T32CMM_SRC := $(SCRIPT_SUBPATH)flash$(CPU_NUMID).cmm
endif
ifeq ($(call iseeopt, __EE_MEMORY_PROTECTION__), yes)
BASE_LD_SCRIPT := $(SCRIPT_SUBPATH)memprot$(CPU_NUMID)$(CC_LD_SUFFIX).tmpl
endif

# T32SYS is the environemnt variable recognized by Trace32
T32SYS ?= /opt/case/emulator/lauterbach/t32

# The above part is needed for the base makefile for multicore building
# The part below containes rules for `all' and `clean', and will interfere
# with the rules that handle the per-cpu building process
ifeq ($(and $(call iseeopt, __MSRP__), \
	$(__BASE_MAKEFILE__)), yes)
include $(PKGBASE)/cfg/arch/rules_ppc_multi_base.mk
else

ifneq ($(call iseeopt, __USE_CUSTOM_CRT0__), yes)
EE_CRT0_S := pkg/mcu/freescale_$(PPC_MCU_MODEL)/src/ee_boot.S
ifeq ($(NEED_ASM_TO_C_TRANSLATION), 1)
EE_CRT0_SRCS := $(call asm_to_c_filename,$(EE_CRT0_S))
else
EE_CRT0_SRCS := $(EE_CRT0_S)
endif
endif

# Add application file to dependencies
ifneq ($(or $(call iseeopt, ONLY_LIBS), $(call iseeopt, EE_NO_LIBEE)),yes)
# OPT_LIBS is used to link additional libraries (e.g., for C++ support)
# the EE library is built in the current directory
OPT_LIBS +=  -L. -lee
LIBDEP = libee.a

endif

# Specific option from the libs dependencies
LIBDEP += $(ALL_LIBS)

# Specific option from the application makefile
LIBDEP += $(LDDEPS)

ELFNAME ?= ppc.elf
# if is specified __NO_APP__ the user want Erika as libee.a and noting else
ifneq ($(call iseeopt, __NO_APP__), yes)
TARGET:=$(ELFNAME)
else
TARGET:=libee.a
endif
MAP_FILE = ppc.map

include $(wildcard $(PKGBASE)/cfg/cfg.mk)


#
# --------------------------------------------------------------------------
#

##
## Common Rules
##


# Boot code containing _start should stay outside of the library in
# case of normal compilation
CRT0 := $(addprefix $(OBJDIR)/, $(patsubst %.c,%.o,$(patsubst %.S,%.o, $(EE_CRT0_SRCS))))
OPT_CRT0 := $(CRT0)

LIBEESRCS += $(EE_SRCS)

#if only lib is needed (__NO_APP__) and we want the lib configurated add eecfg.c
#to LIBEESRCS
ifeq ($(and $(call iseeopt, __NO_APP__), $(call iseeopt, EE_CONFIGURATED_LIB)), yes)
LIBEESRCS += eecfg.c
endif

LIBEEOBJS := $(addprefix $(OBJDIR)/, $(patsubst %.c,%.o,$(patsubst %.S,%.o,$(LIBEESRCS))))
LIBOBJS := $(addprefix $(OBJDIR)/, $(patsubst %.c,%.o,$(patsubst %.S,%.o,$(LIBSRCS))))

SRCS += $(APP_SRCS)
OBJS := $(addprefix $(OBJDIR)/, $(patsubst %.c,%.o,$(patsubst %.S,%.o, $(SRCS))))

ifneq ($(call iseeopt, EE_NO_LIBEE), yes)
APPOBJS := $(OBJS)
else
APPOBJS := $(LIBEEOBJS) $(OBJS)
endif

ALLOBJS = $(LIBEEOBJS) $(LIBOBJS) $(OBJS) $(CRT0)

OBJDIRS=$(sort $(dir $(ALLOBJS) $(INTERMEDIATE_FILES)))

# INCLUDE_PATH is a space-separated list of directories for header file searching
# we consider the ee pkg directory and the application dir
# we also consider the current directory because the app could be compiled
# from the config files generated from eclipse...
INCLUDE_PATH += $(APPBASE) $(PKGBASE) .

# Slave CPU: add linkerscript with global shared symbols
ifeq ($(call iseeopt, __MSRP__), yes)
ifneq ($(CPU_NUMID), 0)
ADDITIONAL_LINKSCRIPT = $(GLOBAL_LINKSCRIPT)
endif
endif


vpath %.c $(EE_VPATH) $(APPBASE)
vpath %.S $(EE_VPATH) $(APPBASE)

##
## Compute common variables
##

# Avoid multiple evaluations of OPT_INCLUDE, which may call `cygpath'
COMPUTED_OPT_INCLUDE := $(OPT_INCLUDE)
COMPUTED_OPT_LINK := $(OPT_LINK)
COMPUTED_OPT_ASM := $(OPT_ASM)
COMPUTED_OPT_CC := $(OPT_CC)
COMPUTED_OPT_CC_DEPS := $(OPT_CC_DEPS)
COMPUTED_OPT_TCC := $(OPT_TCC)

## Select input filename format
SOURCEFILE = $(call native_path,$<)
TARGETFILE = $(call native_path,$@)
SRCFILE = $(call native_path,$(patsubst %.o,%.src,$@))


##
## Main rules: all clean
##

.PHONY: all clean t32

.DEFAULT_GOAL := all
all: $(TARGET) t32
# The success message is printed by the $(TARGET) rule, so we get a "Nothing
# do be done" message when everything is up to date

clean:
	@echo "CLEAN\n" ;
	@-rm -rf *.a *.ld *.dld *.map *.elf *.objdump deps deps.pre	\
		debug.bat *.cmm obj *.men *.o *.cd *.src

##
## Lauterbach targets
##
T32GENMENU ?= $(T32SYS)/demo/kernel/orti/genmenu

T32TARGETS := t32.cmm exec_lauterbach.sh
ifneq ($(wildcard system.orti),)
T32TARGETS += orti.cmm orti.men ortiperf.men
T32ORTISTR := do orti.cmm
else
T32ORTISTR :=
endif

t32: $(T32TARGETS)

t32.cmm: $(PKGBASE)/mcu/freescale_$(PPC_MCU_MODEL)/cfg/$(T32CMM_SRC) $(MAKEFILE_LIST)
	$(QUIET)sed -e 's:#ORTICMD#:$(T32ORTISTR):'			\
		-e 's:#USE_VLE#:$(USE_VLE):g'				\
		-e 's:#EXE_NAME#:$(TARGET):g'				\
		-e 's:#USE_LOCK_STEP#:$(LOCK_STEP):g'				\
		$< > $@

orti.cmm ortiperf.men: %: $(PKGBASE)/mcu/freescale_$(PPC_MCU_MODEL)/cfg/%
	$(QUIET) cp $< $@

orti.men: system.orti
	$(QUIET) $(T32GENMENU) $<

# Lauterbach launcher (only for single core: SRAM & FLASH)
ifeq ($(call iseeopt, __MSRP__),yes)
# empty target for multicore, this is defined in rules_ppc_multi_base.sh
exec_lauterbach.sh:
else
exec_lauterbach.sh:  %: $(PKGBASE)/mcu/freescale_$(PPC_MCU_MODEL)/cfg/%
	@echo "Copying exec_lauterbach.sh..."
	@echo
	$(QUIET)cp $< $@
endif

##
## ELF file creation
##
# if is specified __NO_APP__ the user want Erika as libee.a and noting else
ifneq ($(call iseeopt, __NO_APP__), yes)
$(TARGET): $(CRT0) $(APPOBJS) $(LINKDEP) $(LIBDEP)
	@printf "LD\n";
	$(QUIET)$(EE_LINK) $(COMPUTED_OPT_LINK)				\
		-o $(TARGETFILE) $(MAP_OPT) $(OPT_LIBS) $(OPT_CRT0) $(APPOBJS)
	@echo
	@echo "Compilation terminated successfully"
	@echo
endif
##
## Object file creation
##

$(OBJDIR)/%.o: %.S
ifdef PREPROC_ASM_2_PASS
	$(VERBOSE_PRINTPRE)	$(EE_CC)  $(COMPUTED_OPT_INCLUDE) $(DEFS_ASM) $(DEPENDENCY_OPT) -E $(SOURCEFILE) > $(SRCFILE)
	$(VERBOSE_PRINTASM)	$(EE_ASM) $(COMPUTED_OPT_ASM) -o $(TARGETFILE) $(SRCFILE)
else
	$(VERBOSE_PRINTASM)	$(EE_ASM) $(COMPUTED_OPT_INCLUDE) $(DEFS_ASM) $(DEPENDENCY_OPT) $(COMPUTED_OPT_ASM) -o $(TARGETFILE) $(SOURCEFILE)
endif
	$(QUIET) $(call make-depend, $<, $@, $(subst .o,.d,$@))

$(OBJDIR)/%.o: %.c
	$(VERBOSE_PRINTCC)  $(EE_CC) $(COMPUTED_OPT_CC) $(COMPUTED_OPT_INCLUDE) $(DEFS_CC) $(DEPENDENCY_OPT) -c $(SOURCEFILE) -o $(TARGETFILE)
	$(QUIET) $(call make-depend, $<, $@, $(subst .o,.d,$@))


##
## Assembly-to-C translation (used for CodeWarrior VLE)
## (Boot file need translation but it MUST not be belong to libee, so it cannot
##  be added to EE_CASM_SRCS because in cc_ppc_codewarrior.mk these file are
##  added to EE_SRCS).
##
ifeq ($(NEED_ASM_TO_C_TRANSLATION), 1)
EE_CASM_CFILES = $(call asm_to_c_filename,$(EE_CASM_SRCS)) $(EE_CRT0_SRCS)
$(EE_CASM_CFILES): $(call asm_to_c_filename,%.S): %.S
	@echo "ASM2C $(notdir $<)"
	$(QUIET)$(call asm_to_c_command,$<,$@)
$(EE_CASM_CFILES): | make_directories
INTERMEDIATE_FILES += $(EE_CASM_CFILES)
endif


##
## Locator files
##

ifdef EE_LINK_SCRIPT
# The linker script is generated by concatenating the memory map, the
# section part, and an optional file containing global definition (the last
# one is used for shared symbols in the multicore setup)
ifeq ($(call iseeopt, __EE_MEMORY_PROTECTION__), yes)
$(EE_LINK_SCRIPT): apps.conf $(PKGBASE)/mcu/freescale_$(PPC_MCU_MODEL)/cfg/$(SCRIPT_SUBPATH)memory$(CPU_NUMID).ld $(PKGBASE)/cpu/e200zx/cfg/$(BASE_LD_SCRIPT) $(ADDITIONAL_LINKSCRIPT)
	@printf "LOC\n" ;
	$(QUIET) awk -f $(PKGBASE)/cpu/e200zx/cfg/memprot-genld.awk $^ > $@
else # if __EE_MEMORY_PROTECTION__
$(EE_LINK_SCRIPT): $(PKGBASE)/mcu/freescale_$(PPC_MCU_MODEL)/cfg/$(SCRIPT_SUBPATH)memory$(CPU_NUMID)$(LS_FLAG).ld $(PKGBASE)/cpu/e200zx/cfg/$(BASE_LD_SCRIPT) $(ADDITIONAL_LINKSCRIPT)
	@printf "LOC\n" ;
	$(QUIET) cat $^ > $@
endif # else __EE_MEMORY_PROTECTION__
endif # if EE_LINK_SCRIPT


##
## EE Library
##
ifneq ($(call iseeopt, EE_NO_LIBEE), yes)
libee.a: $(LIBEEOBJS)
	@printf "AR  libee.a\n" ;
	$(QUIET)rm -f $@
	$(QUIET)$(EE_AR) $(OPT_AR) $@ $(LIBEEOBJS)
endif

##
## Automatic Generation of dependencies
##

dependencies=$(subst .o,.d,$(ALLOBJS))
ifneq ($(call iseeopt, NODEPS), yes) 
ifneq ($(MAKECMDGOALS),clean)
-include $(dependencies)
endif
endif


##
## Directories
##

.PHONY: make_directories

# Objects depend on directories, but they are not remade if directories change
$(ALLOBJS): | make_directories

make_directories: $(OBJDIRS)

# Directories are (re)created only when some of them don't exist already
$(OBJDIRS):
	@echo "MAKE_DIRECTORIES"
	$(QUIET) mkdir -p $(OBJDIRS)

endif  # __MSRP__ and __BASE_MAKEFILE__
