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

## Author: 2011 - Dario Di Stefano

##
## CodeWarrior compiler
##

# Enable verbose output from EE_OPT
ifeq ($(call iseeopt, VERBOSE), yes)
 VERBOSE = 1
endif

# Initial definition of target all as predefined
all::

include $(EEBASE)/pkg/cfg/dir.mk
include $(PKGBASE)/cfg/verbose.mk
include $(PKGBASE)/cfg/compiler.mk

# HCS12_LIB_DIR refers to the location of CodeWarrior libraries
HCS12_LIB_DIR := "`cygpath -ms $(S12_CCDIR)/lib/hc12c/lib`"
HCS12_SRC_DIR := "`cygpath -ms $(S12_CCDIR)/lib/hc12c/src`"
HCS12_INC_DIR := "`cygpath -ms $(S12_CCDIR)/lib/hc12c/include`"

HCS12_LIB_PATH = $(S12_CCDIR)/lib/hc12c/lib
HCS12_SRC_PATH = $(S12_CCDIR)/lib/hc12c/src
HCS12_INC_PATH = $(S12_CCDIR)/lib/hc12c/include

OPT_ENV += -envOBJPATH=$(OBJDIR) -envGENPATH=$(HCS12_SRC_DIR)

# ALLINCPATH is a list of directories for source file searching
# -I = adds directories to the source file search path 
# we consider the ee pkg directory and the application dir
# we also consider the current directory because the app could be compiled
# from the config files generated from eclipse...
# please note the final backslash sequence after the shell command to
# avoid cygpath insering a trailing backslash
# INTERNAL_PKGBASEDIR is used to avoid multiple calls to cygpath
ALLINCPATH += -I$(shell cygpath -ms "$(PKGBASE)") -I$(shell cygpath -ms "$(APPBASE)") -I. -I$(shell cygpath -ms "$(S12_CCDIR)/lib/HC12c/include")
 
# Linker script should be inside each project (so user can modify it)

ifneq "$(S12_LINKERSCRIPT)" ""
  LINKDEP += $(S12_LINKERSCRIPT)
else
  # Set default linker script
  ifeq ($(S12_MODEL), MC9S12XS128)
    LINKDEP += $(EEBASE)/pkg/mcu/hs12xs/MC9S12XS128.prm
  endif
  ifeq ($(S12_MODEL), MC9S12G128)
    LINKDEP += $(EEBASE)/pkg/mcu/hs12xs/MC9S12G128.prm
  endif
  ifeq ($(S12_MODEL), MC9S12GN16)
    LINKDEP += $(EEBASE)/pkg/mcu/hs12xs/MC9S12GN16.prm
  endif
  ifeq ($(S12_MODEL), MC9S12GN32)
    LINKDEP += $(EEBASE)/pkg/mcu/hs12xs/MC9S12GN32.prm
  endif
  ifeq ($(S12_MODEL), MC9S12G48)
    LINKDEP += $(EEBASE)/pkg/mcu/hs12xs/MC9S12G48.prm
  endif
  ifeq ($(S12_MODEL), MC9S12GN48)
    # The same linker script as MC9S12G48
    LINKDEP += $(EEBASE)/pkg/mcu/hs12xs/MC9S12G48.prm
  endif
endif

# Specific option from the libs dependencies
LIBDEP += $(ALL_LIBS)

# Specific option from the application makefile
LIBDEP += $(LDDEPS)

# path for libraries
OPT_LIBS += -L$(HCS12_LIB_DIR)

include $(PKGBASE)/cfg/cfg.mk

#
# --------------------------------------------------------------------------
#

##
## Source files and paths
##

# Add s12 architecture file (e.g. for mc9s12xs128 add mc9s12xs129.c or a user-specified src file)
SRCS += datapage.c

LIBEESRCS += $(EE_SRCS)
LIBEEOBJS := $(addprefix $(OBJDIR)/, $(patsubst %.c,%.o,$(patsubst %.S,%.o,$(LIBEESRCS))))

SRCS += $(APP_SRCS)
#SRCS += $(EE_SRCS)
OBJS := $(addprefix $(OBJDIR)/, $(patsubst %.c,%.o,$(patsubst %.S,%.o, $(SRCS))))

ALLOBJS = $(LIBEEOBJS) $(OBJS)
LIBDEP += libee.a

LINK_OBJS = $(foreach d,$(OBJS),$(addprefix -add,$d))
LINK_LIBS = $(foreach d,$(LIBDEP),$(addprefix -add,$d))


vpath %.cd $(APPBASE)
vpath %.Sd $(APPBASE)
vpath %.c $(EE_VPATH) $(APPBASE) $(S12_CCDIR)/lib/hc12c/src
vpath %.S $(EE_VPATH) $(APPBASE) $(S12_CCDIR)/lib/hc12c/src

##
## Compute common variables
##

COMPUTED_ALLINCPATH := $(ALLINCPATH)
COMPUTED_OPT_LINK := $(OPT_LINK) $(OPT_LIBS)
COMPUTED_OPT_ASM := $(OPT_ASM) $(OPT_ENV)
COMPUTED_OPT_CC := $(OPT_CC) $(OPT_ENV)
COMPUTED_OPT_CC_DEPS := $(OPT_CC_DEPS)
COMPUTED_OPT_AR := $(OPT_AR)
COMPUTED_OPT_OBJDUMP := $(OPT_OBJDUMP) $(OPT_ENV)

## Select input filename format
SOURCEFILE = `cygpath -w $<`
TARGETFILE = `cygpath -w $@`
TARGET := s12.lst
SRCFILE = `cygpath -w $(patsubst %.o,%.src,$@)`
SRCFILE_U = $(patsubst %.o,%.src,$@)
TMPFILE_U = $(patsubst %.o,%.tmp,$@)


##
## Main rules: all clean
##

.PHONY: all clean

all:: make_directories $(ALL_LIBS) $(TARGET) 
	@printf "************************************\n\n"
	@printf "Compilation terminated successfully!\n"

clean::
	@-rm -rf *.a *.ls *.ld *.map *.elf *.lst *.xpr *.log *.dbg EDOUT ee_s12regs.h *.$(HCS12_EXTENSION) *.objdump deps deps.pre obj *.x12
# to support "make clean all"
ifeq ($(findstring all,$(MAKECMDGOALS)),all)
	@printf "CLEAN (also \"all\" specified)\n"
else
	@printf "CLEAN\n";
endif

$(TARGET): s12.$(HCS12_EXTENSION)
	@printf "OBJDUMP\n";
	$(QUIET)$(EE_OBJDUMP) $(COMPUTED_OPT_OBJDUMP) s12.$(HCS12_EXTENSION)

##
## Object file creation
##

s12.$(HCS12_EXTENSION): $(OBJS) $(LINKDEP) $(LIBDEP) 
	@printf "LD\n";
	$(QUIET)$(EE_LINK) $(shell cygpath -ms "$(LINKDEP)") $(LINK_OBJS) $(LINK_LIBS) $(COMPUTED_OPT_LINK) -o$(TARGETFILE) 

# preprocess first the assembly code and then compile the object file
$(OBJDIR)/%.o: %.S ee_s12regs.h
	$(VERBOSE_PRINTPRE) $(EE_PREP) $(COMPUTED_OPT_CC) $(DEPENDENCY_OPT) $(COMPUTED_ALLINCPATH) $(DEFS_CC) -I$(dir $(shell cygpath -ms "$(SOURCEFILE)")) -LpCfg=cms "$(SOURCEFILE)" -Lp=$(SRCFILE)
ifeq ($(call iseeopt, CW_EVAL_VERSION), yes)
	$(QUIET)sed "s/\/\*/;\/\*/g" <$(SRCFILE_U) > $(TMPFILE_U)
	$(QUIET)mv $(TMPFILE_U) $(SRCFILE_U)
endif
	$(VERBOSE_PRINTASM) $(EE_ASM) $(COMPUTED_OPT_ASM) $(COMPUTED_ALLINCPATH) -objn$(TARGETFILE) $(SRCFILE) 
	$(QUIET) $(call make-depend, $<, $@, $(subst .o,.d,$@))
	
# produce the object file from C code in a single step	ATT!!! tolta opzione -c!!! e tolta l'opzione -o $(TARGETFILE) 
$(OBJDIR)/%.o: %.c ee_s12regs.h
	$(VERBOSE_PRINTCPP) $(EE_CC) $(COMPUTED_OPT_CC) $(DEPENDENCY_OPT) $(COMPUTED_ALLINCPATH) $(DEFS_CC) -I$(dir $(shell cygpath -ms "$(SOURCEFILE)")) -objn="$(TARGETFILE)" "$(SOURCEFILE)" 
	$(QUIET) $(call make-depend, $<, $@, $(subst .o,.d,$@))

##
## EE Library
##

libee.a: $(LIBEEOBJS)
	@printf "AR  libee.a\n" ;
	$(QUIET)$(EE_AR) $(COMPUTED_OPT_AR) -Cmd\( $(LIBEEOBJS) = libee.a\)
	@echo

ee_s12regs.h: 
	@printf "GEN ee_s12regs.h\n"
	@printf "/* Automatically generated from Makefile */\n" > ee_s12regs.h
	@printf "#include \"$(S12_INCLUDE_H)\"\n" >> ee_s12regs.h

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

