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

## Author: 2009 - Paolo Gai
## CVS: $Id: rules_freescale_s12x_cosmic.mk,v 1.00 2009/11/11 22:21:06 Paolo Exp $

# Enable verbose output from EE_OPT
ifeq ($(call iseeopt, VERBOSE), yes)
VERBOSE = 1
endif

# Initial definition of target all as predefined
all::

include $(EEBASE)/pkg/cfg/dir.mk
include $(PKGBASE)/cfg/verbose.mk
include $(PKGBASE)/cfg/compiler.mk

# Path used for HCS12 libraries and headers search:
HCS12_LIB_DIR := $(S12_CCDIR)/Lib
HCS12_INCLUDE_DIR := $(S12_CCDIR)/HS12x

# options related to the libraries paths 
OPT_LIBS += -l "`cygpath -w $(HCS12_LIB_DIR)`"
OPT_LIBS += -l "`cygpath -w $(shell pwd)`"

# INTERNAL_CCINCLUDEDIR is used to avoid multiple calls to cygpath
INTERNAL_CCINCLUDEDIR := -i"`cygpath -w $(HCS12_INCLUDE_DIR)`"
ALLINCPATH += $(INTERNAL_CCINCLUDEDIR)

# Add crtsx.S if needed
ifeq "$(S12_INCLUDE_S)" ""
# A default startup script will be used
STARTUP_SRC := pkg/mcu/hs12xs/src/crtsx.S
STARTUP_OBJ := obj\\pkg\\mcu\\hs12xs\\src\\crtsx.o
APP_SRCS += $(STARTUP_SRC)
else
STARTUP_OBJ := $(addprefix $(OBJDIR)\\, $(patsubst %.s,%.o,$(patsubst %.S,%.o, $(S12_INCLUDE_S))))
endif

# Add linker dependencies
OPT_LINK += 
ifeq "$(S12_LINKERSCRIPT)" ""
# A linker-script will be created in the project folder 
LINKDEP += $(APPBASE)/$(S12_MODEL).lkf
else
# User linker-script will be used
LINKDEP += $(S12_LINKERSCRIPT)
endif

# Specific option from the libs dependencies
LIBDEP += $(ALL_LIBS)

# Specific option from the application makefile
LIBDEP += $(LDDEPS)

include $(PKGBASE)/cfg/cfg.mk


#
# --------------------------------------------------------------------------
#

##
## Source files and paths
##
SRCS += 

LIBEESRCS += $(EE_SRCS)
LIBEEOBJS := $(addprefix $(OBJDIR)/, $(patsubst %.c,%.o,$(patsubst %.S,%.o,$(LIBEESRCS))))
SRCS += $(APP_SRCS)
OBJS := $(addprefix $(OBJDIR)/, $(patsubst %.c,%.o,$(patsubst %.S,%.o, $(SRCS))))
ALLOBJS = $(LIBEEOBJS) $(OBJS)

# Cosmic compiler libraries list (to be added in the linker script)
COSMIC_LIBDEP += $(shell cygpath -m $(S12_CCDIR))/Lib/libe.x12 \
                 $(shell cygpath -m $(S12_CCDIR))/Lib/libf.x12 \
                 $(shell cygpath -m $(S12_CCDIR))/Lib/libi.x12 \
                 $(shell cygpath -m $(S12_CCDIR))/Lib/libm.x12

# Check if clib.exe exists in the compiler directory
RES_CLIB_CHECK := $(if $(wildcard $(S12_CCDIR)/clib.exe),PASSED,FAILED)
ifeq ($(RES_CLIB_CHECK),FAILED)
 # CLIB.EXE does not exists...
 LIBDEP += $(LIBEEOBJS)
else
 # CLIB.EXE exists (an archive will be created)...
 LIBDEP += libee.x12
endif

vpath %.cd $(APPBASE)
vpath %.Sd $(APPBASE)
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

## Select input filename format
SOURCEFILE = `cygpath -w $<`
TARGETFILE = `cygpath -w $@`
TARGET := $(S12_MODEL).objdump
SRCFILE = `cygpath -w $(patsubst %.o,%.src,$@)`

##
## Main rules: all clean
##

.PHONY: all clean

all:: make_directories $(ALL_LIBS) $(TARGET) 
	@printf "************************************\n\n"
	@printf "Compilation terminated successfully!\n"

clean::
	@-rm -rf *.list *.a *.ls *.ld *.map *.elf *.$(HCS12_EXTENSION) *.objdump deps deps.pre obj *.x12 ee_s12regs.h

# to support "make clean all"
ifeq ($(findstring all,$(MAKECMDGOALS)),all)
	@printf "CLEAN (also \"all\" specified)\n"
else
	@printf "CLEAN\n";
endif

$(TARGET): $(S12_MODEL).$(HCS12_EXTENSION)
	@printf "OBJDUMP\n";
	$(QUIET)$(EE_CLABS) $(S12_MODEL).$(HCS12_EXTENSION) 
	@cat `find . -type f -name "*.la"` > $(TARGET)
	@printf "ELF\n";
	$(QUIET)$(EE_CVDWARF) $(S12_MODEL).$(HCS12_EXTENSION)

##
## Object file creation
##

# ATT!!! tolta l'opzione -m > $(S12_MODEL).map
$(S12_MODEL).$(HCS12_EXTENSION): $(OBJS) $(LIBDEP)
	@printf "LD\n";
ifeq "$(S12_LINKERSCRIPT)" ""
# create a file with the EE-objs list
	$(QUIET)rm -rf eeobjs.list $(APPBASE)/$(S12_MODEL).lkf
	$(QUIET)echo "# Place obj files for the unbanked section here" > eeobjs.list;
	$(QUIET)echo \"$(STARTUP_OBJ)\"    >> eeobjs.list;
	$(QUIET)echo "# Place libraries and Erika RTOS obj-files list here" >> eeobjs.list;
	$(QUIET)echo \"obj\\eecfg.o\"      >> eeobjs.list;
	$(QUIET)for x in $(LIBDEP); do \
	  echo \"$${x}\"      >> eeobjs.list; \
	done;
	$(QUIET)for x in $(COSMIC_LIBDEP); do \
	  echo \"$${x}\"      >> eeobjs.list; \
	done;
# create the linker script
	$(QUIET)cat $(EEBASE)/pkg/mcu/hs12xs/$(S12_MODEL).lkf eeobjs.list > $(APPBASE)/$(S12_MODEL).lkf
endif
	$(QUIET)$(EE_LINK) $(COMPUTED_OPT_LINK) -o $(TARGETFILE) $(OPT_LIBS) $(LINKDEP) $(OBJS) $(LIBDEP)

# preprocess first the assembly code and then compile the resulting file to obtain the object file
$(OBJDIR)/%.o: %.S
ifneq ($(call iseeopt, NODEPS), yes)
	@echo "$(TARGETFILE): \\" > $(call native_path,$(subst .o,.d_tmp,$@))
	$(QUIET) $(EE_PREP) $(COMPUTED_ALLINCPATH) $(DEFS_CC) $(DEPENDENCY_OPT_CPS12X) "$(SOURCEFILE)"  >> $(call native_path,$(subst .o,.d_tmp,$@))
	$(QUIET) $(call make-depend, $<, $@, $(subst .o,.d,$@))
endif
	$(VERBOSE_PRINTPRE) $(EE_PREP) $(COMPUTED_ALLINCPATH) $(DEFS_CC) -e "$(SOURCEFILE)" > $(SRCFILE)
	$(VERBOSE_PRINTASM) $(EE_ASM) $(COMPUTED_OPT_ASM) -o $(TARGETFILE) $(SRCFILE) 

# produce the object file from C code
$(OBJDIR)/%.o: %.c ee_s12regs.h  
ifneq ($(call iseeopt, NODEPS), yes)
	@echo "$(TARGETFILE): \\" > $(call native_path,$(subst .o,.d_tmp,$@))
	$(QUIET) $(EE_PREP) $(COMPUTED_ALLINCPATH) $(DEFS_CC) $(DEPENDENCY_OPT_CPS12X) "$(SOURCEFILE)"  >> $(call native_path,$(subst .o,.d_tmp,$@))
	$(QUIET) $(call make-depend, $<, $@, $(subst .o,.d,$@))
endif
	$(VERBOSE_PRINTCPP) $(EE_CC) $(COMPUTED_OPT_CC) $(COMPUTED_ALLINCPATH) $(DEFS_CC) -a"-o $(TARGETFILE)" "$(SOURCEFILE)" 

##
## EE Library
##
libee.x12: $(LIBEEOBJS)
	@printf "AR  libee.x12\n" ;
	$(QUIET)$(EE_AR) -c libee.x12 $(LIBEEOBJS)

ee_s12regs.h: 
	@printf "GEN ee_s12regs.h\n"
	@printf "/* Automatically generated from Makefile */\n" > ee_s12regs.h
	@printf "#ifndef __EE_S12REGS_H__\n" >> ee_s12regs.h
	@printf "#define __EE_S12REGS_H__\n" >> ee_s12regs.h
	@printf "#include \"$(S12_INCLUDE_H)\"\n" >> ee_s12regs.h
	@printf "#endif\n" >> ee_s12regs.h

##
## Automatic Generation of dependencies
##

dependencies=$(subst .o,.d,$(ALLOBJS))
ifneq ($(call iseeopt, NODEPS), yes) 
ifneq ($(MAKECMDGOALS),clean)
-include $(dependencies)
endif
endif

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


