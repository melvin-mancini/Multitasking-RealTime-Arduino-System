# ###*B*###
# ERIKA Enterprise - a tiny RTOS for small microcontrollers
# 
# Copyright (C) 2002-2011  Evidence Srl
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

## Variables for the GCC suite for Lattice Mico32
## Author: 2010-2011,  Bernardo  Dal Seno
## Based on cc_pic30.mk and on Lattice Mico32 documentation

# Override native_path on Windows, as the compiler is a Cygwin application
native_path = $1

EE_CC = lm32-elf-gcc
EE_DEP = lm32-elf-gcc
EE_ASM = lm32-elf-gcc
EE_LINK = lm32-elf-gcc
EE_OBJDUMP = lm32-elf-objdump
EE_ELF_SIZE = lm32-elf-size
ifeq ($(call iseeopt, __SPACE__), yes)
CFLAGS += -Os
else
CFLAGS += -O2
endif
CFLAGS += -ffunction-sections -Wall
CPPFLAGS += $(DEFS_CC) $(addprefix -I, $(INCLUDE_PATH))
LDFLAGS +=-Wl,--gc-sections
DEFS_GCCASM := $(addprefix -D,$(EEOPT))
DEFS_CC := $(addprefix -D,$(EEOPT))
EE_AR = lm32-elf-ar

# Dependenciese are always generated, as they cost negligible time
DEPENDENCY_OPT = -MMD -MF $(subst .o,.d_tmp,$@) -MP -MT $@
make-depend = mv $3_tmp $3
