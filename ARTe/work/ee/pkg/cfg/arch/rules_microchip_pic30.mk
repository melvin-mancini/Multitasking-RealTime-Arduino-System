# ###*B*###
# ERIKA Enterprise - a tiny RTOS for small microcontrollers
# 
# Copyright (C) 2002-2010  Evidence Srl
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

## Author: 2005- Paolo Gai
## 2010 Bernardo  Dal Seno
## CVS: $Id: rules_microchip_pic30.mk,v 1.24 2008/10/14 22:21:06 nino Exp $

# Enable verbose output from EE_OPT
ifeq ($(call iseeopt, VERBOSE), yes)
VERBOSE = 1
endif

# Initial definition of target all as predefined
all::

include $(EEBASE)/pkg/cfg/dir.mk
include $(PKGBASE)/cfg/verbose.mk
include $(PKGBASE)/cfg/compiler.mk

PIC30_CRT0 := $(EEBASE)/contrib/microchip/pic30/boot/src/crt0.s

PIC30_LIB_DIR := $(PIC30_GCCDIR)/lib

# If PIC30_LIB_DIR has subdirectories... (since 3.10) 
ifneq ($(shell find $(PIC30_LIB_DIR) -mindepth 1 -type d),)
SHORT_MODEL := $(shell echo $(PIC30_MODEL) | awk '{ a = substr($$0,1,3); print a;}')

#search for PIC24F
ifeq ($(findstring 24,$(SHORT_MODEL)), 24)
MCHP_SUPPORT_DIR := $(PIC30_GCCDIR)/support/PIC$(SHORT_MODEL)
PIC30_LIBD_DIR := $(PIC30_LIB_DIR)/PIC$(SHORT_MODEL)
else
MCHP_SUPPORT_DIR := $(PIC30_GCCDIR)/support/dsPIC$(SHORT_MODEL)
PIC30_LIBD_DIR := $(PIC30_LIB_DIR)/dsPIC$(SHORT_MODEL)
endif

else
MCHP_SUPPORT_DIR := $(PIC30_GCCDIR)/support
PIC30_LIBD_DIR :=
endif

PIC30_H_DIR := $(MCHP_SUPPORT_DIR)/h
PIC30_INC_DIR := $(MCHP_SUPPORT_DIR)/inc
PIC30_LINKER_DIR := $(MCHP_SUPPORT_DIR)/gld

# Add linker dependencies
OPT_LINK += --script=loc_gnu.ld -u__reset
LINKDEP += loc_gnu.ld

# Add application file to dependencies
ifneq ($(ONLY_LIBS), TRUE)

## OPT_LIBS is used to link additional libraries (e.g., for C++ support)
ifneq ($(call iseeopt, __BIN_DISTR), yes) 
# the EE library is built in the current directory
OPT_LIBS += -lee -L .
LIBDEP += libee.a
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

# Libraries from MC
OPT_LIBS += -lm -lc -ldsp
ifeq ($(call iseeopt, __USE_ELF__), yes)
OPT_LIBS += -lq-elf 
else
OPT_LIBS += -lq-coff 
endif
OPT_LIBS += -l$(subst .a,,$(subst lib,,$(PIC30_DEV_LIB)))
OPT_LIBS += -lpic30-$(PIC30_OFF)

OPT_LIBS += -L $(call native_path,$(PIC30_LIB_DIR))

# check if PIC30_LIBD_DIR is empty
ifneq ($(PIC30_LIBD_DIR),)
OPT_LIBS += -L $(call native_path,$(PIC30_LIBD_DIR))
endif

## PIC30-related directories
# we should look if these need to be moved inside dir.mk

#if PIC30_GCCDIR is defined
ifneq ($(PIC30_GCCDIR),)
DEFS_CC += -D__PIC30_INCLUDE_REGS__
MPLAB_INCLUDE_REGS=__PIC30_INCLUDE_REGS__
endif

# Add application file to dependencies
ifneq ($(ONLY_LIBS), TRUE)
TARGET:=pic30.objdump
endif

# When building for MPLAB IDE, we do not have to include the source
# code of the application, but we have to include the library
# containing the EE code. 
# note that:
# - libee.a is not included in ALL_LIBS
# - when GENERATE_MPLABIDE_LIBS is defined, ONLY_LIBS is NOT defined
ifeq ($(GENERATE_MPLABIDE_LIBS), TRUE)
TARGET:=libee.a generate_eeopt

# we reset the SRCS variable (no application has to be compiled), and
# we put the eecfg.c inside the library
SRCS:=
LIBEESRCS+= $(OUTBASE)/eecfg.c
endif

include $(PKGBASE)/cfg/cfg.mk

#
# --------------------------------------------------------------------------
#

##
## Source files and paths
##
## Select with an EE-option if compile crt0.s or link libpic30-$(PIC30_OFF).a
##

ifeq ($(call iseeopt, __USE_CRT0_S__), yes)
EE_BOOT_SRCS := frommchp/crt0.s
else
EE_BOOT_SRCS := 
endif

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

SRCS += $(foreach d,$(APP_SRCS),$(call unix_relpath,$d))
OBJS := $(addprefix $(OBJDIR)/, $(patsubst %.c,%.o,$(patsubst %.S,%.o, $(patsubst %.s,%.o, $(SRCS)))))

# Variable used to import dependencies
ALLOBJS = $(LIBEEOBJS) $(LIBOBJS) $(OBJS)

# Variable used to create all needed directories
OBJDIRS=$(sort $(dir $(ALLOBJS))) frommchp

# INCLUDE_PATH is a space-separated list of directories for header file searching
# we consider the ee pkg directory and the application dir
# we also consider the current directory because the app could be compiled
# from the config files generated from eclipse...
INCLUDE_PATH += $(PKGBASE) $(APPBASE) .

vpath %.c $(EE_VPATH) $(APPBASE)
vpath %.S $(EE_VPATH) $(APPBASE)

##
## Compute common variables
##

# Avoid multiple evaluations of OPT_INCLUDE, which may call `cygpath'
COMPUTED_OPT_INCLUDE := $(OPT_INCLUDE)
COMPUTED_OPT_LINK := $(OPT_LINK)
COMPUTED_OPT_ASM_VAR := -Wa,
COMPUTED_OPT_ASM := $(addprefix $(COMPUTED_OPT_ASM_VAR), $(OPT_ASM))
COMPUTED_OPT_CC := $(OPT_CC)
COMPUTED_OPT_AR := $(OPT_AR)

## Select input filename format
SOURCEFILE = $(call native_path,$<)
TARGETFILE = $(call native_path,$@)

##
## Main rules: all clean
##

.PHONY: all clean

all:: make_directories $(ALL_LIBS) $(TARGET) 
	@printf "************************************\n\n"
	@printf "Compilation terminated successfully!\n"

clean::
	@-rm -rf *.a *.ld *.map *.$(PIC30_EXTENSION) *.objdump obj
# to support "make clean all"
ifeq ($(findstring all,$(MAKECMDGOALS)),all)
	@printf "CLEAN (also \"all\" specified, frommchip directory not removed)\n"
else
	@printf "CLEAN\n";
	@-rm -rf frommchp
endif

pic30.objdump: pic30.$(PIC30_EXTENSION)
	@printf "OBJDUMP\n";
	$(QUIET)$(EE_OBJDUMP) -d pic30.$(PIC30_EXTENSION) > pic30.objdump

##
## Object file creation
##


pic30.$(PIC30_EXTENSION): $(OBJS) $(LINKDEP) $(LIBDEP) 
	@printf "LD\n";
	$(QUIET)$(EE_LINK) $(COMPUTED_OPT_LINK) \
                     -o $(TARGETFILE) $(OBJS) \
                     --start-group $(OPT_LIBS) --end-group \
                     -M > pic30.map

# produce the object file from assembly code in a single step
$(OBJDIR)/%.o: %.S ee_pic30regs.inc
	$(VERBOSE_PRINTCPP) $(EE_CC) $(COMPUTED_OPT_INCLUDE) $(COMPUTED_OPT_ASM) $(DEFS_ASM) -mcpu=$(PIC30_MODEL) $(DEPENDENCY_OPT) -c $(SOURCEFILE) -o $(TARGETFILE)
	$(QUIET) $(call make-depend, $<, $@, $(subst .o,.d,$@))

# produce the object file from C code in a single step
$(OBJDIR)/%.o: %.c ee_pic30regs.h
	$(VERBOSE_PRINTCPP) $(EE_CC) $(COMPUTED_OPT_CC) $(COMPUTED_OPT_INCLUDE) $(DEFS_CC) $(DEPENDENCY_OPT) -c $(SOURCEFILE) -o $(TARGETFILE)
	$(QUIET) $(call make-depend, $<, $@, $(subst .o,.d,$@))

$(OBJDIR)/frommchp/crt0.o: frommchp/crt0.s
	$(VERBOSE_PRINTCPP) $(EE_CC) $(COMPUTED_OPT_ASM) -mcpu=$(PIC30_MODEL) -c $(SOURCEFILE) -o $(TARGETFILE)


##
## Microchip C30 files
##

frommchp/crt0.s: $(PIC30_CRT0)
	@printf "\nCP crt0.s\n"; cp $(SOURCEFILE) $(TARGETFILE); chmod +rw $(TARGETFILE)

# Check if the MCU model has been defined
ifneq ($(PIC30_MODEL),)

ee_pic30regs.h: frommchp/$(PIC30_INCLUDE_C)
	@printf "GEN ee_pic30regs.h\n"
	@printf "/* Automatically generated from Makefile */\n" > ee_pic30regs.h
	@printf "#include \"frommchp/$(PIC30_INCLUDE_C)\"\n" >> ee_pic30regs.h

ee_pic30regs.inc: frommchp/$(PIC30_INCLUDE_S)
	@printf "GEN ee_pic30regs.inc\n"
	@printf "; Automatically generated from Makefile\n" > ee_pic30regs.inc
	@printf "	.include \"frommchp/$(PIC30_INCLUDE_S)\" \n" >> ee_pic30regs.inc

#frommchp/$(PIC30_INCLUDE_C): $(PIC30_GCCDIR)/support$(C30SUBDIR)/h/$(PIC30_INCLUDE_C)
frommchp/$(PIC30_INCLUDE_C): $(PIC30_H_DIR)/$(PIC30_INCLUDE_C)
	@printf "CP $(PIC30_INCLUDE_C)\n"; cp $(SOURCEFILE) $(TARGETFILE); chmod +rw $(TARGETFILE)

#frommchp/$(PIC30_INCLUDE_S): $(PIC30_EXTRA_INCLUDE)/support$(C30SUBDIR)/inc/$(PIC30_INCLUDE_S)
frommchp/$(PIC30_INCLUDE_S): $(PIC30_INC_DIR)/$(PIC30_INCLUDE_S)
	@printf "CP $(PIC30_INCLUDE_S)\n"; cp $(SOURCEFILE) $(TARGETFILE); chmod +rw $(TARGETFILE)

else

# no MCU model defined
ee_pic30regs.h: frommchp/$(PIC30_INCLUDE_C)
	@printf "GEN ee_pic30regs.h\n"
	@printf "/* Automatically generated from Makefile */\n" > ee_pic30regs.h
	@printf "/* WARNING! No MCU model selected! */\n" >> ee_pic30regs.h

ee_pic30regs.inc: frommchp/$(PIC30_INCLUDE_S)
	@printf "GEN ee_pic30regs.inc\n"
	@printf "; Automatically generated from Makefile \n" > ee_pic30regs.inc
	@printf "; WARNING! No MCU model selected! \n" >> ee_pic30regs.inc
endif


##
## Locator files
##

#if PIC30_GCCDIR is defined
#loc_gnu.ld: $(PIC30_LINKERDIR)/$(PIC30_LINKERSCRIPT)
#	@printf "LOC\n" ; cp $(PIC30_LINKERDIR)/$(PIC30_LINKERSCRIPT) loc_gnu.ld; chmod +rw loc_gnu.ld
loc_gnu.ld: $(PIC30_LINKER_DIR)/$(PIC30_LINKERSCRIPT)
	@printf "LOC\n" ; cp $(PIC30_LINKER_DIR)/$(PIC30_LINKERSCRIPT) loc_gnu.ld; chmod +rw loc_gnu.ld

##
## EE Library
##

libee.a: $(LIBEEOBJS)
	@printf "AR  libee.a\n" ;
	$(QUIET)$(EE_AR) rs libee.a $(LIBEEOBJS)

#
# --------------------------------------------------------------------------

##
## Directories
##

.PHONY: make_directories

# Objects depend on directories, but they are not remade if directories change
$(ALLOBJS): | make_directories

# Directories are (re)created only when some of them don't exist already
make_directories: 
	@echo "MAKE_DIRECTORIES"
	$(QUIET) mkdir -p $(OBJDIRS)

#
# --------------------------------------------------------------------------
#

# the eeopt file is generated when dealing with MPLAB IDE!
#
# this is a phony because the source code does not depend on this file
# and its content higly depends on the EEOPT variables...
#
.PHONY: generate_eeopt
generate_eeopt:
	@printf "MPLAB - eeopt.h\n"
	@echo // This part of the include file includes all the options > eeopt.h
	@echo // which are typically inserted with >> eeopt.h
	@echo // the -D compiler directive. >> eeopt.h
	@echo // This part is automatically generated by the makefile only for MPLABIDE. >> eeopt.h
	@for x in $(EEOPT) $(MPLAB_INCLUDE_REGS); do \
		echo \#ifndef $${x}      >> eeopt.h; \
		echo \#define $${x}      >> eeopt.h; \
		echo \#endif             >> eeopt.h; \
	done;



#
# --------------------------------------------------------------------------
#

##
## Automatic Generation of dependencies
##
dependencies=$(subst .o,.d,$(ALLOBJS))
ifneq ($(MAKECMDGOALS),clean)
ifneq ($(call iseeopt, NODEPS), yes) 
-include $(dependencies)
endif
endif

