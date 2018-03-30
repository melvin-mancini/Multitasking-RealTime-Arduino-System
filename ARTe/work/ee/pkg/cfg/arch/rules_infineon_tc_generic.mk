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
## TriCore compilation rule, based on new best practices
##
## Author: 2012, Errico Guidieri
##

ifeq ($(call iseeopt, VERBOSE), yes)
VERBOSE = 1
endif

include $(EEBASE)/pkg/cfg/dir.mk
include $(PKGBASE)/cfg/verbose.mk
include $(PKGBASE)/cfg/compiler.mk
# Read MCU-specific file, if it exists, but don't make it.
-include $(PKGBASE)/cfg/arch/rules_infineon_$(TRICORE_MODEL).mk

# T32SYS is the environemnt variable recognized by Trace32
T32SYS ?= C:/T32
T32ARCH ?= windows64

# The above part is needed for the base makefile for multicore building
# The part below containes rules for `all' and `clean', and will interfere
# with the rules that handle the per-cpu building process
ifeq ($(and $(call iseeopt, __MSRP__), $(__BASE_MAKEFILE__)), yes)
include $(PKGBASE)/cfg/arch/rules_infineon_multi_base.mk
else  # __MSRP__ and __BASE_MAKEFILE__

# Explicit the default target
.DEFAULT_GOAL := all

# Erika LIB name
EELIB    ?= ee
ERIKALIB  = lib$(EELIB).a

# Add application file to dependencies
ifneq ($(ONLY_LIBS), TRUE)
# OPT_LIBS is used to link additional libraries (e.g., for C++ support)
ifneq ($(call iseeopt, __BIN_DISTR), yes) 
# the EE library is built in the current directory
LIBDEP = $(ERIKALIB)
else # EEOPT __BIN_DISTR
# the EE library is stored in the EE lib directory
LIBDEP += $(EEBASE)/lib/$(ERIKALIB)
endif # EEOPT __BIN_DISTR
endif # ONLY_LIBS

# Specific option from the libs dependencies
LIBDEP += $(ALL_LIBS)

# Define the TARGET
TARGET_NAME ?= $(TRICORE_MODEL)

# Add application file to dependencies
ifneq ($(call iseeopt, __BUILD_LIBS__), yes)
TARGET := $(TARGET_NAME).elf
endif

# Continue build system configuration
# If the environment tag ERIKA as FLAT, use the right configuration file
ifdef EE_FLAT_LAYOUT
# Remember: all the sources are found starting from vpath=$(EE_VPATH)=$(EEBASE)
include $(EEBASE)/ee_cfg.mk
# Add repository root to INCLUDE_PATH (by default only PKGBASE is included)
INCLUDE_PATH += $(EEBASE)
# Make without dir even EE_BOOT_SRCS
EE_BOOT_SRCS := $(notdir $(EE_BOOT_SRCS))
else
include $(PKGBASE)/cfg/cfg.mk
endif # EE_FLAT_LAYOUT

#
# --------------------------------------------------------------------------
#

##
## Common Rules
##

# Boot code containing _start should stay outside of the library in
# case of normal compilation
ifeq ($(call iseeopt, __BIN_DISTR), yes)
LIBSRCS += $(EE_BOOT_SRCS)
else
SRCS += $(EE_BOOT_SRCS)
endif

LIBEESRCS += $(EE_SRCS)
LIBEEOBJS += $(addprefix $(OBJDIR)/, $(patsubst %.c,%.o,$(patsubst %.S,%.o,$(LIBEESRCS))))

LIBEESRCS += $(LIB_SRCS)
LIBOBJS += $(addprefix $(OBJDIR)/, $(patsubst %.c,%.o,$(patsubst %.S,%.o,$(LIBSRCS))))

SRCS += $(APP_SRCS)
OBJS += $(addprefix $(OBJDIR)/, $(patsubst %.c,%.o,$(patsubst %.S,%.o, $(SRCS))))

# Variable used to import dependencies
ALLOBJS = $(LIBEEOBJS) $(LIBOBJS) $(OBJS) 

OBJDIRS=$(sort $(dir $(ALLOBJS)))

# Add basic include paths
INCLUDE_PATH += $(PKGBASE) $(APPBASE) .

vpath %.c $(EE_VPATH) $(APPBASE)
vpath %.S $(EE_VPATH) $(APPBASE)

## Select input filename format
SOURCEFILE = $(call native_path,$<)
TARGETFILE = $(call native_path,$@)
## source_c_file and target_c_file, must be defined inside compiler
## make
SOURCE_C_FILE = $(call source_c_file,$(SOURCEFILE))
TARGET_C_FILE = $(call target_c_file,$(TARGETFILE))
## source_c_file and target_c_file, must be defined inside compiler
## make
SOURCE_ASM_FILE = $(call source_asm_file,$(SOURCEFILE))
TARGET_ASM_FILE = $(call target_asm_file,$(TARGETFILE))
## target_ld_file, must be defined inside compiler
## make
TARGET_LD_FILE = $(call target_ld_file,$(TARGETFILE))

##
## Main rules: all clean
##

.PHONY: all clean t32

all: make_directories $(ALL_LIBS) $(TARGET) t32
# The success message is printed by the $(TARGET) rule, so we get a "Nothing
# do be done" message when everything is up to date

clean:
	@printf "CLEAN\n" ;
	@-rm -rf *.a *.ld *.map *.elf *.objdump t32* obj *.men *.sh *.bin
##
## Lauterbach targets
##

#T32GENMENU ?= $(T32SYS)/demo/kernel/orti/genmenu

T32TARGETS := t32.cmm
ifneq ($(wildcard system.orti),)
#T32TARGETS += orti.cmm markers.cmm orti.men ortiperf.men
T32TARGETS += orti.cmm markers.cmm
T32ORTISTR := do orti.cmm
else
T32ORTISTR :=
endif

ifeq ($(call iseeopt, EE_EXECUTE_FROM_RAM), yes)
T32SOURCE := t32_$(TRICORE_MODEL)_ram.cmm
else
T32SOURCE := t32_$(TRICORE_MODEL).cmm
endif

t32: $(T32TARGETS)

t32.cmm: $(PKGBASE)/mcu/infineon_$(TRICORE_MODEL)/cfg/$(T32SOURCE)
	@echo "GEN $@ from $<"
	$(QUIET)sed -e 's:#ORTICMD#:$(T32ORTISTR):'			\
				-e 's:#EXE_NAME#:$(TARGET_NAME).elf:g'	\
			$< > $@

ifeq ($(call iseeopt,__MSRP__), yes)
ifeq ($(CPU_NUMID),0)
orti.cmm markers.cmm: %:  $(PKGBASE)/mcu/infineon_common_tc2Yx/cfg/%
	@echo "GEN $@ from $<"
	$(QUIET) cp $< $@
else #CPU_NUMID == 0
orti.cmm: %:  $(PKGBASE)/mcu/infineon_common_tc2Yx/cfg/multicore/orti_slave.cmm
	@echo "GEN $@ from $<"
	$(QUIET) cp $< $@
markers.cmm: %:  $(PKGBASE)/mcu/infineon_common_tc2Yx/cfg/%
	@echo "GEN $@ from $<"
	$(QUIET) cp $< $@
endif #CPU_NUMID == 0
else #__MSRP__
orti.cmm markers.cmm: %:  $(PKGBASE)/mcu/infineon_common_tc2Yx/cfg/%
	@echo "GEN $@ from $<"
	$(QUIET) cp $< $@
endif #__MSRP__

#orti.men: system.orti
#	$(QUIET) $(T32GENMENU) $<

#
# --------------------------------------------------------------------------
#

### Target file creation ###
$(TARGET_NAME).elf: $(OBJS) $(LIBDEP) $(LINKDEP)
	@echo "LD $@";
	$(QUIET)$(EE_LINK) $(OPT_LINK) $(TARGET_LD_FILE) $(OBJS) $(LIBDEP) $(LINK_REDIRECT)
	@echo "************************************"
	@echo "Compilation terminated successfully!"

# produce the object file from assembly code in a single step
$(OBJDIR)/%.o: %.S $(OBJDEP)
	$(VERBOSE_PRINTASM) $(EE_ASM) $(DEFS_ASM) $(OPT_INCLUDE) $(OPT_ASM) $(DEPENDENCY_OPT_ASM) \
	$(TARGET_ASM_FILE) $(SOURCE_ASM_FILE)
	$(QUIET)$(call make-depend, $(subst .o,.d,$(@)))

# produce the object file from C code in a single step
$(OBJDIR)/%.o: %.c $(OBJDEP)
	$(VERBOSE_PRINTCC) $(EE_CC) $(DEFS_CC) $(OPT_INCLUDE) $(OPT_CC) $(DEPENDENCY_OPT) \
	$(TARGET_C_FILE) $(SOURCE_C_FILE)
	$(QUIET)$(call make-depend, $(subst .o,.d,$(@)))

##
## EE Library
##
$(ERIKALIB): $(LIBEEOBJS)
	@echo "AR $@"
	$(QUIET)rm -f $@
	$(QUIET)$(EE_AR) $(OPT_AR) $(ERIKALIB) $(LIBEEOBJS)

##
## Directories
##

.PHONY: make_directories

## Objects depend on directories, but they are not remade if directories change
$(ALLOBJS): | make_directories

make_directories: $(OBJDIRS)

## Directories are (re)created only when some of them don't exist already
$(OBJDIRS): 
	@echo "MAKE_DIRECTORIES"
	$(QUIET)mkdir -p $(OBJDIRS)

# the eeopt file is generated when dealing with ONLY_LIBS!
#
# this is a phony because the source code does not depend on this file
# and its content higly depends on the EEOPT variables...
#
.PHONY: generate_eeopt
generate_eeopt:
	@echo "GEN   eeopt.h"
	$(QUIET)for x in $(EEOPT); do \
		echo \#ifndef $${x}      >> eeopt.h; \
		echo \#define $${x}      >> eeopt.h; \
		echo \#endif             >> eeopt.h; \
	done;

##
## Automatic Generation of dependencies
##
dependencies=$(subst .o,.d,$(ALLOBJS))
ifneq ($(call iseeopt, NODEPS), yes) 
ifneq ($(MAKECMDGOALS),clean)
-include $(dependencies)
endif
endif

endif  # __MSRP__ and __BASE_MAKEFILE__

