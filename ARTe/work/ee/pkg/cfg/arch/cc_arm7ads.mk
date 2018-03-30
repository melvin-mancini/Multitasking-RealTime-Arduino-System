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

##
## ARM7ADS ARM ADS compiler
##

ifeq ($(call iseeopt, __ARM7ADS__), yes)

ifeq ($(PLATFORM), LINUX)

# BINDIR is the directory of assembler, compiler, linker...
BINDIR = $(ARMCONF)

# ALLINCPATH is a colon separated list of directories for source file searching
ALLINCPATH += $(EEBASE)/pkg,$(APPBASE),.

else

# BINDIR is the directory of assembler, compiler, linker...
BINDIR = $(shell cygpath -u $(ARMCONF))

# ALLINCPATH is a colon separated list of directories for source file searching
ALLINCPATH += $(shell cygpath -wsa "$(EEBASE)/pkg"),$(shell cygpath -wsa "$(APPBASE)"),.

endif

ifndef EE_LINK
EE_LINK=$(BINDIR)/armlink
endif
ifndef EE_ASM
EE_ASM=$(BINDIR)/armasm
endif
ifndef EE_CC
EE_CC=$(BINDIR)/armcc
endif
ifndef EE_TCC
EE_TCC=$(BINDIR)/tcc
endif
ifndef EE_AR
EE_AR=$(BINDIR)/armar
endif

# These seems not to be available for ADS
#ifndef EE_NM
#EE_NM=
#endif
#ifndef EE_OBJCOPY
#EE_OBJCOPY=
#endif
#ifndef EE_OBJDUMP
#EE_OBJDUMP=
#endif

## OPT_CC are the options for arm compiler invocation
# -g  = add debug tables
# -O1 = select minimum optimization
# -c  = compile only (do not link)
# -j  = adds directories to the source file search path
OPT_CC = -O2 -c -j"$(ALLINCPATH)" -j- -apcs /interwork
ifneq ($(call iseeopt, __BIN_DISTR), yes)
ifeq ($(call iseeopt, DEBUG), yes)
OPT_CC += -g
endif
endif

## OPT_TCC are the options for thumb compiler invocation
OPT_TCC = $(OPT_CC)

## OPT_ASM are the options for asm invocation
# -g    = generate debug tables
# -i    = adds directories to the source file search path
OPT_ASM = -i "$(ALLINCPATH)" -apcs /interwork

ifneq ($(call iseeopt, __BIN_DISTR), yes)
ifeq ($(call iseeopt, DEBUG), yes)
OPT_ASM += -g
endif
endif

# OPT_LINK represents the options for armlink invocation
OPT_LINK += 

# Defining EEOPT Macros
# Each identifier that is listed in EEOPT is also inserted as a 
# command-line macro in the compiler...

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

# we use gcc as preprocessor for the dependencies
ifndef GCC
GCC = $(QUIET)gcc
endif
OPT_GCC += -I$(EEBASE)/pkg -I$(APPBASE) -I. $(addprefix -D, $(EEOPT) )



endif
