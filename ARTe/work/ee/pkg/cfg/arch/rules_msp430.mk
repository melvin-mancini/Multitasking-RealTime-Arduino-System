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

## Author: 2010  Christian Grioli
## Based on rules_mico32.mk and on MSP430 documentation
##
## Updated: 2011 Steve Langstaff
## Added non-LINUX build platform support

# Enable verbose output from EE_OPT
ifeq ($(call iseeopt, VERBOSE), yes)
VERBOSE = 1
endif

# Initial definition of target `all' as predefined
all:

include $(EEBASE)/pkg/cfg/dir.mk
# This overrides OBJDIR set in dir.mk
OBJDIR := $(OUTPUT_DIR)/obj


include $(PKGBASE)/cfg/verbose.mk


include $(PKGBASE)/cfg/compiler.mk


ERIKALIB = $(OUTPUT_DIR)/libee.a
# Add application file to dependencies
ifneq ($(ONLY_LIBS), TRUE)

## OPT_LIBS is used to link additional libraries (e.g., for C++ support)
ifneq ($(call iseeopt, __BIN_DISTR), yes) 
# the EE library is built in the current directory
OPT_LIBS += -lee -L $(OUTPUT_DIR)
LIBDEP += $(ERIKALIB)
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

include $(PKGBASE)/cfg/cfg.mk


#
# --------------------------------------------------------------------------
#

##
## Source files and paths
##
LIBEESRCS += $(EE_SRCS)
LIBEEOBJS := $(addprefix $(OBJDIR)/, $(patsubst %.c,%.o,$(patsubst %.S,%.o,$(LIBEESRCS))))

LIBEESRCS += $(LIB_SRCS)
LIBOBJS := $(addprefix $(OBJDIR)/, $(patsubst %.c,%.o,$(patsubst %.S,%.o,$(LIBSRCS))))

SRCS += $(APP_SRCS)
OBJS=$(addprefix $(OBJDIR)/, $(sort \
$(patsubst %.cxx, %.o, \
$(patsubst %.cc, %.o, $(patsubst %.cpp, %.o, \
$(patsubst %.C, %.o,$(CXX_SRCS)))))\
$(patsubst %.c, %.o, \
$(patsubst %.S, %.o, $(patsubst %.s, %.o, $(SRCS))))))



# Final executable
APP_OUTPUT_ELF = $(OUTPUT_DIR)/out.elf
# Ultimate target: dissassembly
TARGET= $(OUTPUT_DIR)/dumpMsp430.objdump 

OBJDIRS=$(sort $(dir $(OBJS)) $(dir $(LIBEEOBJS)) $(dir $(LIBOBJS)))

# __RTD_MSP430__ is defined in the EE options by RT-Druid.
# This sets up the compilation environment to build in:
#     <project>/Debug
# with source code and header files found either in:
#     <project>/Debug or
#     <project> 
ifeq ($(call iseeopt, __RTD_MSP430__), yes)
ifeq ($(PLATFORM), WINDOWS)
INCLUDE_PATH += "$(shell cygpath -w $(PKGBASE))" $(APPBASE) .
else
INCLUDE_PATH += $(PKGBASE) $(APPBASE) .
endif
vpath %.c $(EE_VPATH) $(APPBASE)
vpath %.S $(EE_VPATH) $(APPBASE)
else
INCLUDE_PATH += $(PKGBASE) $(APPBASE) $(OUTPUT_DIR)
# APPBASE is the current directory: not needed
vpath %.c $(EE_VPATH) #$(APPBASE)
vpath %.S $(EE_VPATH) #$(APPBASE)
endif


##
## Main rules: all clean
##

	
.PHONY: all

all: $(TARGET)

# The success message is printed by the $(TARGET) rule, so we get a "Nothing
# do be done" message when everything is up to date

# `clean' is performed by the project makefile; nothing to clean here


$(TARGET): $(APP_OUTPUT_ELF)
	$(QUIET) $(EE_OBJDUMP) -d -r -S $< > $@.tmp  && mv $@.tmp $@
# The 'mv' above is needed so $@ is not created if the command fails
	$(QUIET) $(EE_ELF_SIZE) $<
	@echo
	@echo "Compilation terminated successfully"
	@echo


$(APP_OUTPUT_ELF): $(OBJS) $(LD_FILE) $(LIBDEP)
	$(QUIET) $(EE_LINK) $(CPU_CONFIG) -o $@ \
 $(OBJS) $(OPT_LIBS) $(LDFLAGS)

## Select input filename format
ifeq ($(PLATFORM), WINDOWS)
SOURCEFILE = `cygpath -w $<`
TARGETFILE = `cygpath -w $@`
else
SOURCEFILE = $<
TARGETFILE = $@
endif


##
## Object file creation
##

dependencies=$(subst .o,.d,$(LIBEEOBJS) $(OBJS))
ifneq ($(MAKECMDGOALS),clean)
-include $(dependencies)
endif
define make-depend
	$(EE_DEP)	-MM		\
			-MF $3		\
			-MP		\
			-MT $2		\
			$(CPU_CONFIG)	\
			$(CPPFLAGS)	\
			$1
endef

# Build .o files from .c files.
$(OBJDIR)/%.o: %.c
	$(VERBOSE_PRINTCPP) $(EE_CC) -c $(CFLAGS) $(CPPFLAGS) -c -o $(TARGETFILE) $(SOURCEFILE) 
	$(VERBOSE_PRINTDEP) $(call make-depend,$(SOURCEFILE),$(TARGETFILE),$(subst .o,.d,$(TARGETFILE)))

# Build .o files from .S files.
$(OBJDIR)/%.o: %.S
	$(VERBOSE_PRINTASM) $(EE_ASM) -c $(CPU_CONFIG) $(CFLAGS) $(CPPFLAGS) $< -o $@
	$(VERBOSE_PRINTDEP) $(call make-depend,$(SOURCEFILE),$(TARGETFILE),$(subst .o,.d,$(TARGETFILE)))

# Objects depend on directories, but they are not remade if directories change
$(OBJS) $(LIBEEOBJS): | make_directories


##
## EE Library
##

$(ERIKALIB): $(LIBEEOBJS)	
	@echo "AR    $(notdir $@)"
	$(QUIET)$(AR) $(ARFLAGS) $@ $^ $(VERBOSE_DEVNULL)

##
## Directories
##

.PHONY: make_directories
make_directories: $(OBJDIRS)

# Directories are (re)created only when some of them don't exist already
$(OBJDIRS):
	@echo "MAKE_DIRECTORIES"
	$(QUIET) mkdir -p $(OBJDIRS)

