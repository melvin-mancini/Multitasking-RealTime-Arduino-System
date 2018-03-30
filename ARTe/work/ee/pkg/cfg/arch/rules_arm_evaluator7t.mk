# ###*B*###
# ERIKA Enterprise - a tiny RTOS for small microcontrollers
# 
# Copyright (C) 2002-2008  Evidence Srl
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

## Author: 2001-2002 Paolo Gai, Alessandro Colantonio, 2003- Paolo Gai
## CVS: $Id: rules_arm_evaluator7t.mk,v 1.3 2008/10/10 11:37:40 nino Exp $

# Enable verbose output from EE_OPT
ifeq ($(call iseeopt, VERBOSE), yes)
VERBOSE = 1
endif

include $(EEBASE)/pkg/cfg/dir.mk
include $(PKGBASE)/cfg/verbose.mk
include $(PKGBASE)/cfg/compiler.mk

#
# Trace32 Locations
#
ifndef T32_DRIVE
T32_DRIVE=c:
endif
ifndef T32_DIR
T32_DIR=/cygdrive/c/T32
endif

# ARM7-related directories
# BINDIRCC = $(ECLIPSEBASE)/../arm/gcc-4.1.0-glibc-2.3.2\arm-unknown-linux-gnu/bin

# OPT_LINK represents the options for armlink invocation
ifneq ($(call iseeopt, __DEFAULT_LD_SCRIPT__), yes)
OPT_LINK += --script=loc_gnu.ld -u__start
LINKDEP = loc_gnu.ld loc_evaluator7t.ld
endif

# Add application file to dependencies
ifneq ($(ONLY_LIBS), TRUE)

# OPT_LIBS is used to link additional libraries (e.g., for C++ support)
ifneq ($(call iseeopt, __BIN_DISTR), yes) 
# the EE library is built in the current directory
OPT_LIBS += -lee -L .
LIBDEP = libee.a
else
OPT_LIBS += -lee_$(EELIB) -L $(EEBASE)/lib
endif

endif

# Specific option from the libs dependencies
LIBDEP += $(ALL_LIBS)

# Specific option from the application makefile
LIBDEP += $(LDDEPS)

# Add application file to dependencies
ifneq ($(call iseeopt, __BUILD_LIBS__), yes)
TARGET:=evaluator7t.objdump t32
endif

include $(wildcard $(PKGBASE)/cfg/cfg.mk)


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
LIBEEOBJS := $(addprefix $(OBJDIR)/, $(patsubst %.c,%.o,$(patsubst %.S,%.o,$(LIBEESRCS))))

LIBEESRCS += $(LIB_SRCS)
LIBOBJS := $(addprefix $(OBJDIR)/, $(patsubst %.c,%.o,$(patsubst %.S,%.o,$(LIBSRCS))))

SRCS += $(APP_SRCS)
OBJS := $(addprefix $(OBJDIR)/, $(patsubst %.c,%.o,$(patsubst %.S,%.o, $(SRCS))))
THUMB_OBJS= $(addprefix $(OBJDIR)/, $(patsubst %.c,%.to, $(THUMB_SRCS)))


vpath %.cd $(APPBASE)
vpath %.Sd $(APPBASE)
vpath %.td $(APPBASE)
vpath %.c $(EE_VPATH) $(APPBASE)
vpath %.S $(EE_VPATH) $(APPBASE)

##
## Compute common variables
##

COMPUTED_ALLINCPATH := $(ALLINCPATH)
COMPUTED_OPT_LINK := $(OPT_LINK)
COMPUTED_OPT_ASM := $(OPT_ASM)
COMPUTED_OPT_CC := $(OPT_CC)
COMPUTED_OPT_CC_DEPS := $(OPT_CC_DEPS)
COMPUTED_OPT_TCC := $(OPT_TCC)

## Select input filename format
ifeq ($(PLATFORM), LINUX)
SOURCEFILE = $<
TARGETFILE = $@
SRCFILE = $(patsubst %.o,%.src,$(TARGETFILE))
TOSRCFILE = $(patsubst %.to,%.src,$(TARGETFILE))
else
SOURCEFILE = `cygpath -w $<`
TARGETFILE = `cygpath -w $@`
SRCFILE = `cygpath -w $(patsubst %.o,%.src,$@)`
TOSRCFILE = `cygpath -w $(patsubst %.to,%.src,$@)`
endif

##
## Main rules: all clean
##

.PHONY: all clean

all:: make_directories $(ALL_LIBS) $(TARGET) 
	@printf "************************************\n\n"
	@printf "Compilation terminated successfully!\n"

clean::
	@printf "CLEAN\n" ;
	@-rm -rf *.a *.ld *.map *.elf *.objdump deps deps.pre debug.bat t32* obj *.men

evaluator7t.objdump: evaluator7t.elf
	$(QUIET)$(EE_OBJDUMP) -D evaluator7t.elf > evaluator7t.objdump


##
## ELF file creation
##


evaluator7t.elf: $(OBJS) $(THUMB_OBJS) $(LINKDEP) $(LIBDEP)
	@printf "LD\n";
	$(QUIET)$(EE_LINK) $(COMPUTED_OPT_LINK) \
                     -o $(TARGETFILE) $(OBJS) $(THUMB_OBJS) \
                     --start-group $(OPT_LIBS) --end-group \
                     -M > evaluator7t.map


$(OBJDIR)/%.o: %.S
	$(VERBOSE_PRINTPRE)	$(EE_CC)  $(COMPUTED_ALLINCPATH) $(DEFS_ASM) -E $< > $(SRCFILE)
	$(VERBOSE_PRINTASM)	$(EE_ASM) $(COMPUTED_OPT_ASM) $(SRCFILE) -o $(TARGETFILE)

$(OBJDIR)/%.o: %.c
	$(VERBOSE_PRINTCC)  $(EE_CC) $(COMPUTED_OPT_CC) $(COMPUTED_ALLINCPATH) $(DEFS_CC) $< -o $(TARGETFILE)
	$(VERBOSE_PRINTASM)	$(EE_CC) $(COMPUTED_OPT_CC) $(COMPUTED_ALLINCPATH) $(DEFS_CC) $< -S -o $(SRCFILE)

$(OBJDIR)/%.to: %.c
	$(VERBOSE_PRINTTCC)	$(EE_TCC) $(COMPUTED_OPT_TCC) $(COMPUTED_ALLINCPATH) $(DEFS_TCC) $< -o $(TARGETFILE)
	$(VERBOSE_PRINTASM)	$(EE_TCC) $(COMPUTED_OPT_TCC) $(COMPUTED_ALLINCPATH) $(DEFS_TCC) $< -S -o $(TOSRCFILE)


##
## Locator files
##

loc_gnu.ld: $(PKGBASE)/board/arm_evaluator7t/cfg/loc_gnu.ld
	@printf "LOC\n" ;
	@cp $(PKGBASE)/board/arm_evaluator7t/cfg/loc_gnu.ld .

loc_evaluator7t.ld: cfg_e7t.h
	@printf "AWK\n" ;
	@awk '/#define/{ print $$2 " = " $$3 ";" }' < cfg_e7t.h >loc_evaluator7t.ld



##
## EE Library
##

libee.a: $(LIBEEOBJS)
	@printf "AR  libee.a\n" ;
	$(QUIET)$(EE_AR) rs libee.a $(LIBEEOBJS) 


##
## Automatic Generation of dependencies
##

# deps depends on the flag and not on the PHONY rule!
deps: $(OBJDIR)/.make_directories_flag deps.pre
	@printf "GEN deps\n"
	@sed "s/ \<\([A-Za-z]\):/ \/cygdrive\/\l\1/g" < deps.pre > deps

deps.pre: $(addprefix $(OBJDIR)/, $(patsubst %.S,%.Sd,$(patsubst %.c,%.cd, $(SRCS) $(LIBEESRCS) $(LIBSRCS))) $(patsubst %.c,%.td, $(THUMB_SRCS)))
	@printf "GEN deps.pre\n" ; cat $^ > deps.pre

# generate dependencies for ARM .c files and add "file.cd" to the target
$(OBJDIR)/%.cd: %.c
	$(VERBOSE_PRINTDEP) $(EE_CC) $(COMPUTED_OPT_CC) $(COMPUTED_ALLINCPATH) $(DEFS_CC) -M $<  > $(TARGETFILE).tmp
	@echo -n $(TARGETFILE) $(dir $(TARGETFILE)) | cat - $(TARGETFILE).tmp > $(TARGETFILE)
	@rm -rf $(TARGETFILE).tmp
	@test -s $(TARGETFILE) || rm -f $(TARGETFILE)

# generate dependencies for THUMB .c files and add "file.td" to the target
$(OBJDIR)/%.td: %.c
	$(VERBOSE_PRINTDEP) $(EE_TCC) $(COMPUTED_OPT_TCC) $(COMPUTED_ALLINCPATH) $(DEFS_TCC) -M $< > $(TARGETFILE).tmp
	@echo -n $(TARGETFILE) $(dir $(TARGETFILE)) | cat - $(TARGETFILE).tmp > $(TARGETFILE)
	@rm -rf $(TARGETFILE).tmp
	@test -s $(TARGETFILE) || rm -f $(TARGETFILE)

# generate dependencies for .S files and add "file.Sd" to the target
$(OBJDIR)/%.Sd: %.S
	$(VERBOSE_PRINTDEP)	$(EE_CC) $(COMPUTED_ALLINCPATH) $(DEFS_ASM) -M $< > $(TARGETFILE).tmp
	@echo -n $(TARGETFILE) $(dir $(TARGETFILE)) | cat - $(TARGETFILE).tmp > $(TARGETFILE)
	@rm -rf $(TARGETFILE).tmp
	@test -s $(TARGETFILE) || rm -f $(TARGETFILE)


#
# --------------------------------------------------------------------------
#

# interesting read: http://www.cmcrossroads.com/content/view/6936/120/

# this forces the directory creation when issuing the "make all"
# rule. there is need for this rule because it may be that the user
# asks for a "make clean all". "clean" removes the directories which
# are then needed for "all", so that when "all" arrives the
# directories must be recreated. We cannot use a flag file like in the
# rule just after because when there is a "clean all" the check for
# prerequisites is not done again when doing "all".
.PHONY: make_directories
make_directories:
ifneq ($(findstring clean,$(MAKECMDGOALS)),clean)
	@printf "MAKE_DIRECTORIES (after a clean)\n"
	$(QUIET)mkdir -p $(dir $(basename $(addprefix $(OBJDIR)/, $(SRCS) $(LIBEESRCS) $(LIBSRCS))))
endif

# this checks but not forces the directory creation when creating dependencies
$(OBJDIR)/.make_directories_flag:
	@printf "MAKE_DIRECTORIES\n"
	$(QUIET)mkdir -p $(dir $(basename $(addprefix $(OBJDIR)/, $(SRCS) $(LIBEESRCS) $(LIBSRCS))))
	$(QUIET)touch $(TARGETFILE)

	
#
# --------------------------------------------------------------------------
#

# Lauterbach t32 scripts

T32_SCRIPTS = $(shell cd $(PKGBASE)/board/arm_evaluator7t/debug/lauterbach; find . ! -type d | grep -v CVS)

T32_DIRS = $(shell cd $(PKGBASE)/board/arm_evaluator7t/debug/lauterbach; find .  -type d | grep -v CVS)


# "touch" solves some dependency issues with make
define T32_template
$(1): $(PKGBASE)/board/arm_evaluator7t/debug/lauterbach/$(1)
	@echo CP $$(TARGETFILE)
	@mkdir -p $$(T32_DIRS)
	@cp -a $$< $$(TARGETFILE)
	@touch $$(TARGETFILE)
endef

$(foreach file,$(T32_SCRIPTS),$(eval $(call T32_template,$(file))))

t32: $(T32_SCRIPTS)
	@echo \&ADS=0 >t32_settings.cmm
	@if test -e system.orti; then \
		echo \&ORTI=1 >>t32_settings.cmm; \
		$(T32_DIR)/demo/kernel/orti/genmenu.exe system.orti; \
		cp $(T32_DIR)/demo/kernel/orti/ortiperf.men .; \
	else echo \&ORTI=0 >>t32_settings.cmm; fi
	@echo "rem ERIKA Enterprise Debug script!"                         > debug.bat
	@echo "rem Do not modify!"                                        >> debug.bat
	@echo "cd `cygpath -aw .`" 					  >> debug.bat
	@echo `cygpath -aw $(T32_DIR)`\\t32marm -c `cygpath -aw $(T32_DIR)/config.t32` >> debug.bat
	@chmod +x debug.bat


ifndef NODEPS
ifneq ($(MAKECMDGOALS),clean)
ifneq ($(call iseeopt, NODEPS), yes) 
-include deps
endif
endif
endif
