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

## Rules for the Lattice Mico32 porting
## Author: 2010  Bernardo  Dal Seno
## Based on rules_microchip_pic30.mk and on Lattice Mico32 documentation

# Enable verbose output from EE_OPT
ifeq ($(call iseeopt, VERBOSE), yes)
VERBOSE = 1
endif

# The __ALLOW_NESTED_IRQ__ option is unsupported:
ifeq ($(call iseeopt, __ALLOW_NESTED_IRQ__), yes)
$(warning Mico32 does not currently support nested interrupts)
$(warning The IRQ nesting option will be ignored by the HAL)
endif

# This EEOPT is added as retro compatibility fix for board old name
ifeq ($(call iseeopt,  __XP2_CAMERA_BOARD__), yes)
EEOPT += __FPG_EYE__
endif # __XP2_CAMERA_BOARD__

EEOPT += __BIG_ENDIAN__

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

ALLOBJS = $(LIBEEOBJS) $(LIBOBJS) $(OBJS)

# Final executable
APP_OUTPUT_ELF = $(OUTPUT_DIR)/out.elf
# Ultimate target: dissassembly
TARGET= $(OUTPUT_DIR)/dump.txt

OBJDIRS=$(sort $(dir $(ALLOBJS)))

INCLUDE_PATH += $(PKGBASE) $(APPBASE) $(OUTPUT_DIR) $(PLATFORM_LIB_PATH)/$(PLATFORM_NAME)
# ALLINCPATH includes the '-I' or '-i' flag, which depends on the compiler

# APPBASE is the current directory: not needed
vpath %.c $(EE_VPATH) $(APPBASE)
vpath %.S $(EE_VPATH) $(APPBASE)


ifndef PLATFORM_BLD_CFG
ifeq ($(call iseeeopt, DEBUG), yes)
PLATFORM_BLD_CFG=Debug
else
PLATFORM_BLD_CFG=Release
endif
endif

# Where platform-dependent makefiles are located.
PLATFORM_MAKEFILES_DIR = $(PLATFORM_LIB_PATH)/$(PLATFORM_NAME)/$(PLATFORM_BLD_CFG)

# Platform library (relative path and name)
PLATFORM_LIBRARY=$(PLATFORM_LIB_PATH)/$(PLATFORM_BLD_CFG)/$(PLATFORM_BLD_CFG)/lib$(PLATFORM_NAME).a

# Platform_rules.mk contains CPU configuration.
include $(PLATFORM_MAKEFILES_DIR)/platform_rules.mk


# Linker scripts (if not provided by the user)
ifneq ($(call iseeopt, __USE_CUSTOM_LINKER_SCRIPT__), yes)
PLATFORM_LINK_SCRIPT=$(PLATFORM_MAKEFILES_DIR)/linker.ld
LINK_SCRIPT=$(OUTPUT_DIR)/ee_linker.ld
# Remove the crt*.o files from the building process
$(LINK_SCRIPT): $(PLATFORM_LINK_SCRIPT)
	@echo "LOC"
	$(QUIET) grep -v -E '^INPUT\(' $< > $@

LINK_SCRIPT_OPT=-T $(LINK_SCRIPT)

else # __USE_CUSTOM_LINKER_SCRIPT__
LINK_SCRIPT=
LINK_SCRIPT_OPT=

endif # __USE_CUSTOM_LINKER_SCRIPT__


# First, remove the crt0 added by old RT-Druid versions
APP_SRCS0 := $(filter-out $(OUTPUT_DIR)/crt0ram.S, $(APP_SRCS))
APP_SRCS = $(APP_SRCS0)
# Add crt0 if not provided by the user
ifneq ($(call iseeopt, __USE_CUSTOM_CRT0__), yes)
APP_SRCS += $(CRT0_SRCS)
endif # __USE_CUSTOM_CRT0__


##
## Main rules: all clean
##

.PHONY: all
all: $(TARGET)
# The success message is printed by the $(TARGET) rule, so we get a "Nothing
# do be done" message when everything is up to date

# `clean' is performed by the project makefile; nothing to clean here
ifeq ($(call iseeopt, EE_IGNORE_MICO32_OBJDUMP_ERRORS), yes)
EE_IGNORE_MICO32_OBJDUMP_ERRORS_TXT :=|| echo "objdump failed "
endif


$(TARGET): $(APP_OUTPUT_ELF)
	@echo OBJDUMP
	$(QUIET) $(EE_OBJDUMP) -d -r -S $< > $@.tmp  && mv $@.tmp $@ $(EE_IGNORE_MICO32_OBJDUMP_ERRORS_TXT)
# The 'mv' above is needed so $@ is not created if the command fails
	$(QUIET) $(EE_ELF_SIZE) $<
	@echo
	@echo "Compilation terminated successfully"
	@echo


$(APP_OUTPUT_ELF): $(OBJS) $(PLATFORM_RULES_MAKEFILE) $(LINK_SCRIPT) \
 $(PLATFORM_LIBRARY) $(LIBDEP)
	@echo LD
	$(QUIET) $(EE_LINK) $(CPU_CONFIG) $(LINK_SCRIPT_OPT) -o $@ \
 $(OBJS) $(OPT_LIBS) $(PLATFORM_LIBRARY) -lm  $(C_LIB) -lgcc \
 $(PLATFORM_LIBRARY) -lnosys $(LDFLAGS)


##
## Object file creation
##

dependencies=$(subst .o,.d,$(ALLOBJS))
ifneq ($(call iseeopt, NODEPS), yes) 
ifneq ($(MAKECMDGOALS),clean)
-include $(dependencies)
endif
endif

# Build .o files from .c files.
$(OBJDIR)/%.o: %.c
	$(VERBOSE_PRINTCPP) $(EE_CC) -c $(CPU_CONFIG) $(CFLAGS) $(CPPFLAGS) $(DEPENDENCY_OPT) $< -o $@
	$(QUIET) $(call make-depend, $<, $@, $(subst .o,.d,$@))

# Build .o files from .S files.
$(OBJDIR)/%.o: %.S
	$(VERBOSE_PRINTASM) $(EE_ASM) -c $(CPU_CONFIG) $(CFLAGS) $(CPPFLAGS) $(DEPENDENCY_OPT) $(ASFLAGS) $< -o $@
	$(QUIET) $(call make-depend, $<, $@, $(subst .o,.d,$@))


##
## EE Library
##

$(ERIKALIB): $(LIBEEOBJS)
	@echo "AR    $(notdir $@)"
	$(QUIET)$(EE_AR) $(ARFLAGS) $@ $^ $(VERBOSE_DEVNULL)


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


##
## Memory deployment files
##

ifndef DEPLOY_DIR
DEPLOY_DIR:=$(OUTPUT_DIR)
endif

ifneq ($(findstring clean,$(MAKECMDGOALS)),clean)
-include $(OUTPUT_DIR)/platform_vars.mk
endif

PLATFORM_LIB_MAKEFILE=$(PLATFORM_LIB_PATH)/$(PLATFORM_BLD_CFG)/makefile

$(OUTPUT_DIR)/platform_vars.mk: $(PLATFORM_LIB_MAKEFILE)
	$(QUIET) grep -E '^(PLATFORM_FILE|PLATFORM_FILE_PATH|PLATFORM_PERL_FILE_PATH)[ ]*=' $< > $@

REAL_PLATFORM_FILE_PATH=$(PLATFORM_LIB_PATH)/$(PLATFORM_FILE_PATH)

PERLSCRIPTDIR=$(realpath $(PLATFORM_PERL_FILE_PATH)/..)
PERLUTILDIR=$(realpath $(PERLSCRIPTDIR)/..)

# Using a phony target, as the actual target files are not known (they
# depend on the platform and library configurations)
.PHONY: meminit
meminit: $(APP_OUTPUT_ELF) $(TARGET)
	$(QUIET) perl $(PERLSCRIPTDIR)/misc/multimem_deploy.pl $(PERLUTILDIR) $(abspath $(REAL_PLATFORM_FILE_PATH)/$(PLATFORM_FILE)) $< $(DEPLOY_DIR)/ meminit
