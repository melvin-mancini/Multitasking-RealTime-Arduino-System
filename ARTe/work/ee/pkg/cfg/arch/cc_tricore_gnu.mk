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


##
## TRICORE_GNU GCC compiler version 3
##

ifeq ($(call iseeopt, __TRICORE_GNU__), yes)

ifeq ($(PLATFORM), LINUX)
BINDIR = $(TRICORE1_GCCDIR)/bin
else
$(error Host-platforms other than 'LINUX' not supported.)
endif

# ALLINCPATH is a colon separated list of directories for source file searching
# -I = adds directories to the source file search path (for both gcc and gas)
ALLINCPATH += -I$(EEBASE)/pkg -I$(APPBASE) -I.

ifndef TRICORE1_GCCPREFIX
TRICORE1_GCCPREFIX = tricore
endif

ifndef EE_OBJDUMP
EE_OBJDUMP=$(BINDIR)/$(TRICORE1_GCCPREFIX)-objdump
endif

# GNUPro compilers
ifndef EE_LINK
EE_LINK=$(BINDIR)/$(TRICORE1_GCCPREFIX)-gcc
endif

ifndef EE_ASM
EE_ASM=$(BINDIR)/$(TRICORE1_GCCPREFIX)-gcc
endif

ifndef EE_CC
EE_CC=$(BINDIR)/$(TRICORE1_GCCPREFIX)-gcc
endif

ifndef EE_AR
EE_AR = $(BINDIR)/$(TRICORE1_GCCPREFIX)-ar
endif

ifndef EE_NM
EE_NM=$(BINDIR)/$(TRICORE1_GCCPREFIX)-nm
endif

ifndef EE_OBJCOPY
EE_OBJCOPY=$(BINDIR)/$(TRICORE1_GCCPREFIX)-objcopy
endif

##
## OPT_CC are the options for arm compiler invocation
##

OPT_CC = -Wall -Winline -finline-functions -std=gnu99 -ggdb -O2

ifeq ($(call iseeopt, __TC179x__), yes)
OPT_CC += -mcpu=$(TRICORE1_MODEL)
endif

# Specific option from the application makefile
OPT_CC += $(CFLAGS)

##
## OPT_ASM are the options for asm invocation
##

OPT_ASM = -std=gnu99 -x assembler-with-cpp -c 

ifeq ($(call iseeopt, DEBUG), yes)
OPT_ASM += -Wa,--gdwarf2 
endif

ifeq ($(call iseeopt, __TC13__), yes)
OPT_ASM += -mtc13 
endif
ifeq ($(call iseeopt, __TC131__), yes)
OPT_ASM += -mtc131 
endif

OPT_ASM += $(ASFLAGS) 

##
## Specific linker option from the application makefile
##

OPT_LINK = -Wall 


ifneq ($(call iseeopt, __TSIM__), yes)

ifeq ($(call iseeopt, __TC179x__), yes)
OPT_LINK += -mcpu=$(TRICORE1_MODEL)
endif

endif


# Enforce linking of this file to provide '.ctors' entries for 
# auto-initialization.
OPT_LINK += -u EE_link_ctors

ifeq ($(call iseeopt, __INT__), yes)
OPT_LINK += -u EE_int_vector
endif

ifeq ($(call iseeopt, __TRAP__), yes)
OPT_LINK += -u EE_trap_vector
endif

OPT_LINK += $(LDFLAGS)


# Defining EEOPT Macros
# Each identifier that is listed in EEOPT is also inserted as a 
# command-line macro in the compiler...

# this was on ST10: DEFS_ASM += $(foreach SYM, $(EEOPT), --defsym $(SYM)=1)
DEFS_ASM = $(addprefix -D, $(EEOPT) )
DEFS_CC  = $(addprefix -D, $(EEOPT) )
DEFS_TCC = $(addprefix -D, $(EEOPT) )

ifeq ($(call iseeopt, __BIN_DISTR), yes) 
# Note: the defines used in EEOPT to compile the library
# are already added in the eecfg.h
DEFS_ASM += -D__CONFIG_$(EELIB)__
DEFS_CC  += -D__CONFIG_$(EELIB)__
DEFS_TCC += -D__CONFIG_$(EELIB)__
endif

endif # __TRICORE_GNU__
