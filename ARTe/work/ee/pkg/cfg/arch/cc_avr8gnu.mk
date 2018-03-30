# ###*B*###
# ERIKA Enterprise - a tiny RTOS for small microcontrollers
# 
# Copyright (C) 2002-2013  Evidence Srl
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

## Author: 2013,  Giuseppe Serano

##
## Compiler related options
##


## AVR Tools GNU GCC compiler

# BINDIR is the directory of assembler, linker, ... distributed with AVR Tools
BINDIR := $(call native_path,$(AVR_TOOLS))/bin

# ALLINCPATH is a colon separated list of directories for source file searching
# -I = adds directories to the source file search path (for both gcc and gas)
# we consider the ee pkg directory and the application dir
# we also consider the current directory because the app could be compiled
# from the config files generated from eclipse...
ALLINCPATH += \
$(foreach d,$(INCLUDE_PATH),$(addprefix -I,$(call native_path,$d)))

ifndef	AVR_GCCPREFIX
AVR_GCCPREFIX = avr
endif

ifndef	EE_OBJDUMP
EE_OBJDUMP = $(BINDIR)/$(AVR_GCCPREFIX)-objdump
endif

# GNUPro compilers
ifndef	EE_LINK
EE_LINK = $(BINDIR)/$(AVR_GCCPREFIX)-gcc
endif

ifndef	EE_ASM
EE_ASM = $(BINDIR)/$(AVR_GCCPREFIX)-gcc
endif

ifndef	EE_CC
EE_CC = $(BINDIR)/$(AVR_GCCPREFIX)-gcc
endif

ifndef	EE_CXX
EE_CXX = $(BINDIR)/$(AVR_GCCPREFIX)-g++
endif

ifndef	EE_AR
EE_AR = $(BINDIR)/$(AVR_GCCPREFIX)-ar
endif

ifndef	EE_NM
EE_NM = $(BINDIR)/$(AVR_GCCPREFIX)-nm
endif

ifndef	EE_OBJCOPY
EE_OBJCOPY = $(BINDIR)/$(AVR_GCCPREFIX)-objcopy
endif

ifndef	EE_SIZE
EE_SIZE = $(BINDIR)/$(AVR_GCCPREFIX)-size
endif

################################################################################
## OPT_CC are the options for arm compiler invocation			      ##
# -mmcu=atmega128    = generate optimized code for AVR atmega128 processor     #
# -Os                 = select the optimization level			       #
#                     NOTE that -O0 can NOT be used because it not expands     #
#                     inline functions!!!!				       #
# -c                 = compile only (do not link)			       #
# -Wall		   = all compiling warning				       #
# -Winline	         = warn if a function does not expand inline           #
# -finline-functions = make all simple functions inline			       #
##-Wl                =Pass option to the linker avr-ld   		      ##
################################################################################

## OPT_CC are the options for avr compiler invocation

OPT_CC += -c -g -Os -w -fdata-sections -ffunction-sections -std=gnu99
ifneq ($(AVR8_MCU),)
OPT_CC += -mmcu=$(AVR8_MCU)
endif	# AVR8_MCU


# Specific option from the application makefile
OPT_CC += $(CFLAGS)

## OPT_CXX are the options for avr compiler invocation

OPT_CXX += -c -g -Os -w -fdata-sections -ffunction-sections -fno-exceptions
ifneq ($(AVR8_MCU),)
OPT_CXX += -mmcu=$(AVR8_MCU)
endif	# AVR8_MCU
ifeq ($(call iseeopt, DEBUG), yes)
OPT_CXX += -gdwarf-2
endif
# Specific option from the application makefile
OPT_CXX += $(CFLAGS) $(CXXFLAGS)

## OPT_ASM are the options for avr assembler invocation
# --gdwarf2     = generate dwarf2 debbugging information for each assembler line
## -x assembler-with-cpp -gdwarf2
OPT_ASM = -c -x assembler-with-cpp
ifeq ($(call iseeopt, __AVR8_GCC_C99__), yes)
OPT_ASM += -std=gnu99
else
OPT_ASM += -std=c89
endif
ifneq ($(AVR8_MCU),)
OPT_ASM += -mrelax
ifeq ($(findstring atxmega, $(AVR8_MCU)), atxmega)
OPT_ASM += -Wa,-gdwarf2
else	# ATXMEGA
ifeq ($(call iseeopt, DEBUG), yes)
OPT_ASM += -Wa,-gdwarf2
endif	# DEBUG
endif	# ATXMEGA
OPT_ASM += -mmcu=$(AVR8_MCU)
endif	# AVR8_MCU

# OPT_AR represents the options for avr linker invocation
OPT_AR = rcs

# OPT_LINK represents the options for avr linker invocation

OPT_LINK += -Os -Wl,--gc-sections
ifneq ($(AVR8_MCU),)
OPT_LINK += -mmcu=$(AVR8_MCU)
endif	# AVR8_MCU



# Defining EEOPT Macros
# Each identifier that is listed in EEOPT is also inserted as a 
# command-line macro in the compiler...

DEFS_ASM = $(addprefix -D,$(EEOPT))
DEFS_CC  = $(addprefix -D,$(EEOPT))
DEFS_CXX  = $(addprefix -D,$(EEOPT))

ifeq ($(call iseeopt, __BIN_DISTR), yes) 
# Note: the defines used in EEOPT to compile the library
# are already added in the eecfg.h
DEFS_ASM += -D__CONFIG_$(EELIB)__
DEFS_CC  += -D__CONFIG_$(EELIB)__
DEFS_CXX  += -D__CONFIG_$(EELIB)__
endif

# Automatic dependency generation
ifeq ($(call iseeopt, NODEPS), yes)
DEPENDENCY_OPT =
make-depend =
else	# EEOPT NODEPS
# Create dependency for all headers, and add a target for each header
DEPENDENCY_OPT = -MP -MMD -MF $(call native_path,$(subst .o,.d_tmp,$(@)))
ifeq ($(call iseeopt, __RTD_CYGWIN__), yes)
# Dependencies on Windows need path translation and quote remotion
make-depend = sed -e 's_\\\(.\)_/\1_g' \
	-e 's_\<\([a-zA-Z]\):/_/cygdrive/\l\1/_g' < $1_tmp > $1 && rm $1_tmp
else	# EEOPT __RTD_CYGWIN__
make-depend = mv $1_tmp $1
endif	# EEOPT __RTD_CYGWIN__
endif	# EEOPT NODEPS
