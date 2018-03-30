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
## ARM7GNU GCC compiler version 3
##

ifeq ($(call iseeopt, __ARM7GNU__), yes)

# ARM7_GCCDIR is the base directory of development environment
# it should be defined in your .bashrc
# ARM7_GCCDIR = 

# BINDIR is the directory of assembler, compiler, linker...
ifeq ($(PLATFORM), LINUX)
BINDIR = $(ARM7_GCCDIR)/bin
else
BINDIR = $(shell cygpath -u "$(ARM7_GCCDIR)/bin")
endif

# ALLINCPATH is a colon separated list of directories for source file searching
# -I = adds directories to the source file search path (for both gcc and gas)
ALLINCPATH += -I$(EEBASE)/pkg -I$(APPBASE) -I.

ifndef ARM7_GCCPREFIX
ARM7_GCCPREFIX = arm-unknown-linux-gnu
endif

# GNUPro compilers
ifndef EE_LINK
EE_LINK=$(BINDIR)/$(ARM7_GCCPREFIX)-ld
endif
ifndef EE_ASM
EE_ASM=$(BINDIR)/$(ARM7_GCCPREFIX)-as
endif
ifndef EE_CC
EE_CC=$(DISTCC) $(BINDIR)/$(ARM7_GCCPREFIX)-gcc
endif
ifndef EE_TCC
ifeq ($(call iseeopt, __UNIBO_MPARM__), yes)
EE_TCC=$(EE_CC) -mthumb # 16-bit thumb C compiler
else
EE_TCC=$(EE_CC) # 16-bit thumb mode not supported, so use same compiler as for arm-32 mode
endif
endif
ifndef EE_AR
EE_AR=$(BINDIR)/$(ARM7_GCCPREFIX)-ar
endif
ifndef EE_NM
EE_NM=$(BINDIR)/$(ARM7_GCCPREFIX)-nm
endif
ifndef EE_OBJCOPY
EE_OBJCOPY=$(BINDIR)/$(ARM7_GCCPREFIX)-objcopy
endif
ifndef EE_OBJDUMP
EE_OBJDUMP=$(BINDIR)/$(ARM7_GCCPREFIX)-objdump
endif


## OPT_CC are the options for arm compiler invocation
# -mcpu=arm7tdmi    = generate optimized code for ARM7TDMI processor
# -mno-apcs-frame   = don't generate a stack frame APCS compliant
#                     (should be the default, anyway when specified 
#                     the compiler puts a warning in conjunction with 
#                     the use of -g)
# -O                = select the optimization level
#                     NOTE that -O0 can NOT be used because it not expands
#                     inline functions!!!!
# -ggdb             = add debug tables for GDB
# -c                = compile only (do not link)
# -mthumb-interwork = support arm/thumb interwork
# -Wall		    = all compiling warning
# -Winline	    = warn if a function does not expand inline
# -finline-functions = make all simple functions inline
ifeq ($(call iseeopt, __UNIBO_MPARM__), yes)
OPT_CC = -march=armv4 -mno-thumb-interwork -O3 -Wall -Winline -c
else
OPT_CC = -mcpu=arm7tdmi -mthumb-interwork \
	-O3 -Wall -Winline -c
endif
ifneq ($(call iseeopt, __BIN_DISTR), yes)
ifeq ($(call iseeopt, DEBUG), yes)
OPT_CC += -ggdb
endif
endif
 
## Specific compiler option from the application makefile
OPT_CC += $(CFLAGS)


## OPT_TCC are the options for thumb compiler invocation
# -mcallee-super-interworking = switch to thumb mode after an arm header
#OPT_TCC = $(OPT_CC) -mthumb-interwork -mcallee-super-interworking
ifeq ($(call iseeopt, __UNIBO_MPARM__), yes)
OPT_TCC = $(OPT_CC)
else
OPT_TCC = $(OPT_CC) -mcallee-super-interworking
endif

## OPT_ASM are the options for asm invocation
# -marm7tdmi        = generate optimized code for ARM7TDMI processor
# -mapcs-32	    = support APCS function calling standard
# -mthumb-interwork = support arm/thumb interwork
ifeq ($(call iseeopt, __UNIBO_MPARM__), yes)
OPT_ASM = -march=armv4 -mapcs-32
else
#OPT_ASM = -marm7tdmi -mthumb-interwork -mapcs-32
OPT_ASM = -mcpu=arm7tdmi -mthumb-interwork -mapcs-32
endif

ifneq ($(call iseeopt, __BIN_DISTR), yes)
ifeq ($(call iseeopt, DEBUG), yes)
OPT_ASM += --gstabs
endif
endif

## Specific assembler option from the application makefile
OPT_ASM += $(ASFLAGS)

## Specific linker option from the application makefile
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

endif
