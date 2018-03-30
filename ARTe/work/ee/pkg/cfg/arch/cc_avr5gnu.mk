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

## Author: 2001-2002 Paolo Gai, Alessandro Colantonio, 2003 Paolo Gai
## CVS: $Id: cc_avr5gnu.mk,v 1.3 2007/10/04 14:35:08 romano Exp $

##
## Compiler related options
##


## AVRGNU GCC compiler version 4.1.1
## Binutils Version AVR-as AVR-ld 2.17+

ifeq ($(call iseeopt, __AVR5__), yes)


# BINDIR is the directory of assembler, linker, ... distributed with MPLAB IDE
BINDIR := $(AVR5_DIR)/bin
#if AVR5_GCCDIR is defined
ifneq ($(AVR5_GCCDIR),)
# Select between X GCC and EE GCC
ifeq ($(AVR5_USE_EEGCC), Y)
BINDIRCC := $(ECLIPSEBASE)/../avr5/AVRGCC/avrgcc/bin
else
BINDIRCC := $(AVR5_GCCDIR)/bin
endif
else
# We _MUST_ use EE version of GCC
BINDIRCC := $(ECLIPSEBASE)/../avr5/AVRGCC/avrgcc/bin
endif

ifeq ($(PLATFORM), LINUX)

# ALLINCPATH is a colon separated list of directories for source file searching
# -I = adds directories to the source file search path (for both gcc and gas)
# we consider the ee pkg directory and the application dir
# we also consider the current directory because the app could be compiled
# from the config files generated from eclipse...
# please note the final backslash sequence after the shell command to
# avoid cygpath insering a trailing backslash
# INTERNAL_PKGBASEDIR is used to avoid multiple calls to cygpath
INTERNAL_PKGBASEDIR := -I$(PKGBASE)) -I$(APPBASE) -I.
ALLINCPATH += $(INTERNAL_PKGBASEDIR)

else

# ALLINCPATH is a colon separated list of directories for source file searching
# -I = adds directories to the source file search path (for both gcc and gas)
# we consider the ee pkg directory and the application dir
# we also consider the current directory because the app could be compiled
# from the config files generated from eclipse...
# please note the final backslash sequence after the shell command to
# avoid cygpath insering a trailing backslash
# INTERNAL_PKGBASEDIR is used to avoid multiple calls to cygpath
INTERNAL_PKGBASEDIR := -I"$(shell cygpath -w $(PKGBASE))\\." -I"$(shell cygpath -w $(APPBASE))\\." -I.
ALLINCPATH += $(INTERNAL_PKGBASEDIR)

endif

ifndef AVR_GCCPREFIX
AVR_GCCPREFIX = avr
endif

ifndef EE_OBJDUMP
EE_OBJDUMP=$(BINDIR)/$(AVR_GCCPREFIX)-objdump
endif

# GNUPro compilers
ifndef EE_LINK
EE_LINK=$(BINDIR)/$(AVR_GCCPREFIX)-gcc
endif

ifndef EE_ASM
EE_ASM=$(BINDIR)/$(AVR_GCCPREFIX)-gcc
endif

ifndef EE_CC
EE_CC=$(BINDIR)/$(AVR_GCCPREFIX)-gcc
endif

ifndef EE_AR
EE_AR = $(BINDIR)/$(AVR_GCCPREFIX)-ar
endif

ifndef EE_NM
EE_NM=$(BINDIR)/$(AVR_GCCPREFIX)-nm
endif

ifndef EE_OBJCOPY
EE_OBJCOPY=$(BINDIR)/$(AVR_GCCPREFIX)-objcopy
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


## OPT_CC are the options for arm compiler invocation 

OPT_CC = -Os -Wall -std=gnu99 -Winline -w -c
OPT_CC += -DRF_BAND=BAND_2400 -DRF_CHANNEL=18 
OPT_CC += -DBOARD_TYPE=RDK230 -DAPP_TYPE=APP_L2  -DSPECIAL_PEER=0 -DDEVICE_TYPE=FD1_NOGTS -DCTRL_IF=UART1


ifeq ($(call iseeopt, DEBUG), yes)
OPT_CC += -gdwarf-2
endif
ifneq ($(AVR5_MODEL),)
OPT_CC += -mmcu=$(AVR5_MODEL)
endif
# Specific option from the application makefile
OPT_CC += $(CFLAGS)


## OPT_ASM are the options for asm invocation
# --gdwarf2     = generate dwarf2 debbugging information for each assembler line
## -x assembler-with-cpp -Wa,-gdwarf2

OPT_ASM = -c -mmcu=$(AVR5_MODEL) -std=gnu99 -x assembler-with-cpp -Wa, 
ifeq ($(call iseeopt, DEBUG), yes)
OPT_ASM += -gdwarf2 
endif


LDFLAGS = -mmcu=$(AVR5_MODEL)
# OPT_LINK represents the options for avrlink invocation
OPT_LINK += $(LDFLAGS)

# Defining EEOPT Macros
# Each identifier that is listed in EEOPT is also inserted as a 
# command-line macro in the compiler...

DEFS_ASM = $(addprefix -D, $(EEOPT) )
DEFS_CC  = $(addprefix -D, $(EEOPT) )

ifeq ($(call iseeopt, __BIN_DISTR), yes) 
# Note: the defines used in EEOPT to compile the library
# are already added in the eecfg.h
DEFS_ASM += -D__CONFIG_$(EELIB)__
DEFS_CC  += -D__CONFIG_$(EELIB)__
endif

endif
