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
## TASKING TRICORE Compiler
##

ifeq ($(call iseeopt, __TRICORE1__), yes)

# TRICOREBASE is the base directory of development environment
# it should be defined in your .bashrc
# ARM7BASE = 

ifeq ($(PLATFORM), LINUX)

# ALLINCPATH is a colon separated list of directories for source file searching
# -I = adds directories to the source file search path (for both gcc and gas)
ALLINCPATH += -I$(subst \,\\,$(EEBASE)/pkg)
ALLINCPATH += -I$(subst \,\\,$(APPBASE))

else

# ALLINCPATH is a colon separated list of directories for source file searching
# -I = adds directories to the source file search path (for both gcc and gas)
ALLINCPATH += -I$(subst \,\\,$(shell cygpath -wa $(EEBASE)/pkg))
ALLINCPATH += -I$(subst \,\\,$(shell cygpath -wa $(APPBASE)))

endif

# BINDIR is the directory of assembler, compiler, linker...
BINDIR = $(TRICOREBASE)/bin

# TASKING compiler
ifndef EE_LINK
EE_LINK=$(QUIET)$(BINDIR)/cctc.exe
endif
ifndef EE_ASM
EE_ASM=$(QUIET)$(BINDIR)/cctc.exe
endif
ifndef EE_CC
EE_CC=$(QUIET)$(BINDIR)/cctc.exe
endif
ifndef EE_AR
#EE_AR=$(QUIET)$(BINDIR)/ not in the demo version
endif

## OPT_CC are the options for arm compiler invocation
OPT_CC = --embedded -Ctc1775b -Wc-c99 -Wc-Ax -Wc-g --switch=auto -Wc--align=1 -Wc-O2 -Wc-t2 --inline-max-incr=25 --inline-max-size=10 -Wc-N8  -Wc-s  -Wc--silicon-bug=cor1 -Wa-mt -Wa-gs -Wa-Ogs -Wa-il -Wa--silicon-bug=cor1 $(ALLINCPATH)
#Cosa sono queste opzioni?
#-Wc-I"$(PRODDIR)\include"
#-Wa-I"$(PRODDIR)\include"
#-Wa-H"c:\program files\tasking\ctc v2.1\myprograms\benchmark\dhrystone\_dhrystone.inc"
ifneq ($(call iseeopt, __BIN_DISTR), yes)
ifeq ($(call iseeopt, DEBUG), yes)
#rimuovere le opzioni di debug!!!OPT_CC += -ggdb
endif
endif

#???
#OPT_ASPCP = -Ctc1775b -Wpcp-P_PCP_ -Wpcp-I"$(PRODDIR)\include" -Wpcp-gs -Wpcp-il
## OPT_ASM are the options for asm invocation
#OPT_ASM = 

ifneq ($(call iseeopt, __BIN_DISTR), yes)
ifeq ($(call iseeopt, DEBUG), yes)
#OPT_ASM += --gstabs
endif
endif


# OPT_LINK represents the options for armlink invocation
OPT_LINK += --format=elf  -Ctc1775b -Wl-M -Wl-mcfklmoQrSU -Wl-OCDLSTXY

#-Wl-L"$(PRODDIR)\lib\$(PROTECTED)"

# ???
#OPT_XVW = -tcfg infineon_triboard_tc1775b.cfg -C tc1775b -i --load_application_goto_main=true --sync_on_halt=on -a 100 -b 500 -s 26




# Defining EEOPT Macros
# Each identifier that is listed in EEOPT is also inserted as a 
# command-line macro in the compiler...

DEFS_CC  = $(addprefix -D, $(EEOPT) )

#ifeq ($(call iseeopt, __BIN_DISTR), yes) 
# Note: the defines used in EEOPT to compile the library
# are already added in the eecfg.h
#DEFS_ASM += -D__CONFIG_$(EELIB)__
#DEFS_CC  += -D__CONFIG_$(EELIB)__
#endif

endif
