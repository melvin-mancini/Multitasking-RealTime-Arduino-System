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

## Author: Jan C. Kleinsorge, TU Dortmund University, 2010-
## 
##

ifeq ($(call iseeopt, VERBOSE), yes)
VERBOSE = 1
endif

include $(EEBASE)/pkg/cfg/dir.mk
include $(PKGBASE)/cfg/verbose.mk
include $(PKGBASE)/cfg/compiler.mk

# Add application file to dependencies
ifneq ($(ONLY_LIBS), TRUE)

# OPT_LIBS is used to link additional libraries (e.g., for C++ support)
ifneq ($(call iseeopt, __BIN_DISTR), yes) 
# the EE library is built in the current directory
OPT_LIBS += -lee -L .
LIBDEP = libee.a
else
# the EE library is stored in the EE lib directory
OPT_LIBS += -l$(EELIB) -L $(EEBASE)/lib
LIBDEP += $(EEBASE)/lib/lib$(EELIB).a
endif

endif

# Specific option from the libs dependencies
LIBDEP += $(ALL_LIBS)

# Specific option from the application makefile
LIBDEP += $(LDDEPS)

# Add application file to dependencies
ifneq ($(call iseeopt, __BUILD_LIBS__), yes)
TARGET := $(TRICORE1_MODEL).objdump
endif

# TODO: maybe optional '-include'
include $(PKGBASE)/cfg/cfg.mk

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
LIBEEOBJS := $(addprefix $(OBJDIR)/, \
  $(patsubst %.c,%.o,$(patsubst %.S,%.o,$(LIBEESRCS))))

LIBEESRCS += $(LIB_SRCS)
LIBOBJS := $(addprefix $(OBJDIR)/, \
  $(patsubst %.c,%.o,$(patsubst %.S,%.o,$(LIBSRCS))))

SRCS += $(APP_SRCS)
OBJS := $(addprefix $(OBJDIR)/, \
  $(patsubst %.c,%.o,$(patsubst %.S,%.o, $(SRCS))))

vpath %.cd $(APPBASE)
vpath %.Sd $(APPBASE)
vpath %.td $(APPBASE)
vpath %.c $(EE_VPATH) $(APPBASE)
vpath %.S $(EE_VPATH) $(APPBASE)

##
## Compute common variables
##

COMPUTED_ALLINCPATH  := $(ALLINCPATH)
COMPUTED_OPT_LINK    := $(OPT_LINK)
COMPUTED_OPT_ASM     := $(OPT_ASM)
COMPUTED_OPT_CC      := $(OPT_CC)

## Select input filename format
ifeq ($(PLATFORM), LINUX)
SOURCEFILE = $<
TARGETFILE = $@
SRCFILE    = $(patsubst %.o,%.src,$(TARGETFILE))
TOSRCFILE  = $(patsubst %.to,%.src,$(TARGETFILE))
else
  $(error Host-platforms other than 'LINUX' not supported.)
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
	@-rm -rf *.a *.ld *.map *.elf *.objdump deps deps.pre debug.bat \
          t32* obj *.men *.sh *.bin

$(TRICORE1_MODEL).objdump: $(TRICORE1_MODEL).elf
	$(QUIET)$(EE_OBJDUMP) -D $< > $@

#
# --------------------------------------------------------------------------
#

$(TRICORE1_MODEL).elf: $(OBJS) $(LINKDEP) $(LIBDEP)
	@printf "LD\n";
	$(QUIET)$(EE_LINK) $(COMPUTED_OPT_LINK) -o $(TARGETFILE) $(OBJS) \
          -Wl,--start-group $(OPT_LIBS) -Wl--end-group \
          -Wl,-Map=$(TRICORE1_MODEL).map


$(OBJDIR)/%.o: %.S
	$(VERBOSE_PRINTPRE) $(EE_CC)  $(COMPUTED_ALLINCPATH) $(DEFS_ASM) -E $< > $(SRCFILE)
	$(VERBOSE_PRINTASM) $(EE_ASM) $(COMPUTED_OPT_ASM) $(SRCFILE) -o $(TARGETFILE)


$(OBJDIR)/%.o: %.c 
	$(VERBOSE_PRINTCPP) $(EE_CC) $(COMPUTED_OPT_CC) $(COMPUTED_ALLINCPATH) $(DEFS_CC) -c $(SOURCEFILE) -o $(TARGETFILE)

##
## Locator files
##



##
## EE Library
##

libee.a: $(LIBEEOBJS)
	@printf "AR libee.a\n"
	$(QUIET)$(EE_AR) rs libee.a $(LIBEEOBJS) 

#
# --------------------------------------------------------------------------
#

##
## Automatic Generation of dependencies
##

# deps depends on the flag and not on the PHONY rule!
deps: $(OBJDIR)/.make_directories_flag deps.pre
	@printf "GEN deps\n"
	@sed "s/ \<\([A-Za-z]\):/ \/cygdrive\/\l\1/g" < deps.pre > deps

deps.pre: $(addprefix $(OBJDIR)/, $(patsubst %.S,%.Sd,$(patsubst %.c,%.cd, $(SRCS) $(LIBEESRCS) $(LIBSRCS))))
	@printf "GEN deps.pre\n" ; cat $^ > deps.pre

# generate dependencies for .c files and add "file.cd" to the target
$(OBJDIR)/%.cd: %.c 
	$(VERBOSE_PRINTDEP) $(EE_CC) $(COMPUTED_OPT_CC) $(COMPUTED_ALLINCPATH) $(DEFS_CC) -M $(SOURCEFILE) > $(TARGETFILE).tmp
	@echo -n $(TARGETFILE) $(dir $(TARGETFILE)) | cat - $(TARGETFILE).tmp > $(TARGETFILE)
	@rm -rf $(TARGETFILE).tmp
	@test -s $(TARGETFILE) || rm -f $(TARGETFILE)

# generate dependencies for .S files and add "file.Sd" to the target
$(OBJDIR)/%.Sd: %.S 
	$(VERBOSE_PRINTDEP) $(EE_CC) $(COMPUTED_ALLINCPATH) $(DEFS_ASM) -M $(SOURCEFILE) > $(TARGETFILE).tmp
	@echo -n $(TARGETFILE) $(dir $(TARGETFILE)) | cat - $(TARGETFILE).tmp > $(TARGETFILE)
	@rm -rf $(TARGETFILE).tmp
	@test -s $(TARGETFILE) || rm -f $(TARGETFILE)


##
## Automatic Generation of the directory structure
##

.PHONY: make_directories
make_directories:
ifeq ($(findstring clean,$(MAKECMDGOALS)),clean)
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

ifndef NODEPS
ifneq ($(MAKECMDGOALS),clean)
-include deps
endif
endif
