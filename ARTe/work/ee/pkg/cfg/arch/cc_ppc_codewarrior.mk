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

##
## Freescale (CodeWarrior) compiler for PPC
## Author: 2011 Bernardo  Dal Seno
##         2012 Francesco Esposito
##

# BINDIR is the directory of assembler, compiler, linker...
MCUToolsBaseDirEnv ?= $(PPC_CW_BASEDIR)
CW_SUPPORTDIR = $(MCUToolsBaseDirEnv)/PA_Support
#ifeq ($(call iseeopt, __RTD_CYGWIN__), yes)
#CW_TOOLSDIR = $(MCUToolsBaseDirEnv)/PowerPC_EABI_Tools
#else # RTD_CYGWIN
CW_TOOLSDIR = $(MCUToolsBaseDirEnv)/PA_Tools
#endif # else RTD_CYGWIN
BINDIR = $(CW_TOOLSDIR)/Command_Line_Tools/
MWCIncludes ?= $(CW_SUPPORTDIR)/ewl/EWL_C/include
MWLibraries ?= $(CW_SUPPORTDIR)/ewl/lib
ifeq ($(call iseeopt, __PPCE200Z0__), yes)
MW_LIB_FLAVOR = _E200z0_VLE_Soft
else
ifeq ($(or $(call iseeopt, __PPCE200Z6__), $(call iseeopt, __PPCE200Z7__), \
$(call iseeopt, EE_PPCE200Z4)), yes)
MW_LIB_FLAVOR = _E200z650
endif
ifneq ($(__BASE_MAKEFILE__), yes)
ifndef MW_LIB_FLAVOR
$(error CPU unsupported by know CodeWarrior libraries)
endif
endif # __BASE_MAKEFILE__
ifeq ($(call iseeopt, __VLE__), yes)
MW_LIB_FLAVOR := $(MW_LIB_FLAVOR)_VLE_SPFP_Only
else
MW_LIB_FLAVOR := $(MW_LIB_FLAVOR)_SPFP_Only
endif
endif # PPCE200Z0
# Apparently, libc_XXX works on Windows, librt_XXX on Linux
ifeq ($(call iseeopt, __RTD_CYGWIN__), yes)
MW_LIBS = $(addsuffix $(MW_LIB_FLAVOR), c rt)
else
MW_LIBS = $(addsuffix $(MW_LIB_FLAVOR), rt)
endif


# Compilers
EE_LINK = $(BINDIR)mwldeppc
EE_ASM = $(BINDIR)mwasmeppc
EE_CC = $(BINDIR)mwcceppc
EE_AR = $(EE_LINK)

INCLUDE_PATH += $(MWCIncludes)
OPT_INCLUDE = $(foreach d,$(INCLUDE_PATH),$(addprefix -I,$(call native_path,$d)))

OPT_TARGET := -proc Zen
## Candidate OPT_CC
# Defaults:
# -abi eabi -model absolute -fp soft -big
OPT_CC = $(OPT_TARGET) $(VLE_OPT) -RTTI off -Cpp_exceptions off \
 -gccinc -char unsigned -nostdinc -pragma "section RW \".stack\" \".ustack\""
ifneq ($(call iseeopt, __MINIMAL_CC_OPTIONS__), yes)
OPT_CC += -flag require_prototypes -msgstyle gcc -rostr \
 -O4 -ipa file -inline on,auto -schedule on
endif
ifeq ($(call iseeopt, __MSRP__), yes)
OPT_CC += -pragma "section RW \"ee_mcglobald\" \"ee_mcglobalu\"" \
 -pragma "section R \"ee_mcglobalc\" \"ee_mcglobalc\""
endif

# VLE support
ifeq ($(call iseeopt, __VLE__), yes)
VLE_OPT = -vle
OPT_CC += -ppc_asm_to_vle
else
VLE_OPT =
endif

# Concatenate CFLAGS to OPT_CC to overwrite OPT_CC values with CFLAGS
# chosen by the user at .oil level
OPT_CC += $(CFLAGS)

## OPT_ASM are the options for asm invocation
OPT_ASM += $(OPT_TARGET) $(VLE_OPT) -msgstyle gcc -gccinc -gnu_mode

# OPT_LINK represents the options for ld invocation
OPT_LINK += $(LDFLAGS) $(OPT_TARGET) -L$(call native_path,$(MWLibraries))
OPT_LINK += $(addprefix -l, $(MW_LIBS))
OPT_LINK += -msgstyle gcc -nostdlib -char unsigned -m ee_start
ifneq ($(call iseeopt, __E200ZX_EXECUTE_FROM_RAM__), yes)
# Here we assume that ROM is always at address 0.  Maybe it's too strong
# an assumption, and a variable should be set in an MCU-specific makefile
OPT_LINK += -romaddr 0x0 -rambuffer 0x0
endif
MAP_OPT = -map $(call native_path,$(MAP_FILE)) -listclosure
# Linker script
ifneq ($(call iseeopt, __USE_CUSTOM_LINKER_SCRIPT__), yes)
EE_LINK_SCRIPT = loc_codewarrior.lcf
LINKDEP = $(EE_LINK_SCRIPT)
OPT_LINK +=  -lcf $(EE_LINK_SCRIPT)
else
EE_LINK_SCRIPT =
LINKDEP =
endif

# S-record file generation
ifeq ($(call iseeopt, GENERATE_S_RECORD), yes)
OPT_LINK += -srec ppc.mot -sreclength 22
endif

CC_LD_SUFFIX = _codewarrior.ld

ifeq ($(call iseeopt, __DEFAULT_LD_SCRIPT__), yes)
$(error "EEOPT __DEFAULT_LD_SCRIPT__ not supported")
endif

ifneq ($(call iseeopt, __BIN_DISTR), yes)
ifeq ($(call iseeopt, DEBUG), yes)
OPT_CC += -gdwarf-2 -use_lmw_stmw off
OPT_ASM += -gdwarf-2
OPT_LINK += -gdwarf-2
else
ifneq ($(call iseeopt, __MINIMAL_CC_OPTIONS__), yes)
OPT_CC += -use_lmw_stmw on
endif
endif
else
ifneq ($(call iseeopt, __MINIMAL_CC_OPTIONS__), yes)
OPT_CC += -use_lmw_stmw on
endif
endif


# OPT_AR: options for library generation
OPT_AR = -library $(OPT_TARGET) -o

DEFS_ASM = $(addprefix -D, $(EEOPT) )
DEFS_CC  = $(addprefix -D, $(EEOPT) )

# Automatic dependency generation
ifeq ($(call iseeopt, NODEPS), yes)
DEPENDENCY_OPT = 
make-depend =
else # NODEPS
ifeq ($(call iseeopt, __RTD_CYGWIN__), yes)
# Create dependency for all headers, and add a target for each header
DEPENDENCY_OPT = -MDfile $(call native_path,$(subst .o,.d_tmp,$@))
# Dependencies on Windows need path translation
make-depend = sed -e 's_\\\(.\)_/\1_g' -e 's_\<\([a-zA-Z]\):/_/cygdrive/\l\1/_g' < $3_tmp > $3 && rm $3_tmp
else # __RTD_CYGWIN__
# Create dependency for all headers, and add a target for each header
DEPENDENCY_OPT = -MDfile $(subst .o,.d_tmp,$@)
#-gccdep ??
make-depend = mv $3_tmp $3
endif # __RTD_CYGWIN__
endif # NODEPS

NEED_ASM_TO_C_TRANSLATION = 1
asm_to_c_filename = $(addprefix $(OBJDIR)/,$(subst .S,_asm.c,$1))
asm_to_c_command = gawk -f $(EEBASE)/pkg/cfg/arch/ppc_codewarrior_vle.awk < $1 > $2 || rm $2
EE_SRCS += $(call asm_to_c_filename,$(EE_CASM_SRCS))
