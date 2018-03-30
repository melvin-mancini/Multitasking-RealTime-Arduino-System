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

## Author: 2003- Paolo Gai
## CVS: $Id: rules_infineon_tc1775b.mk,v 1.2 2006/12/21 15:38:52 pj Exp $


include $(EEBASE)/pkg/cfg/dir.mk
include $(PKGBASE)/cfg/verbose.mk
include $(PKGBASE)/cfg/compiler.mk

# OPT_LIBS is used to link additional libraries (e.g., for C++ support)
#ifneq ($(call iseeopt, __BIN_DISTR), yes) 
#OPT_LIBS += -lee -L $(APPBASE)/out
#else
#OPT_LIBS += -lee_$(EELIB) -L $(EEBASE)/lib
#endif

include $(wildcard $(PKGBASE)/cfg/cfg.mk)




#
# --------------------------------------------------------------------------
#

##
## Common Rules
##


# Boot code containing _start should stay outside of the library in
# case of normal compilation
ifeq ($(call iseeopt, __BIN_DISTR), yes)
LIBSRCS += $(EE_BOOT_SRCS)
else
SRCS += $(EE_BOOT_SRCS)
endif

LIBSRCS += $(EE_SRCS)
LIBOBJS = $(addprefix out/, $(patsubst %.c,%.o,$(patsubst %.S,%.o,$(LIBSRCS))))

SRCS += $(APP_SRCS)
OBJS= $(addprefix out/, $(patsubst %.c,%.o,$(patsubst %.S,%.o,$(SRCS))))
THUMB_OBJS= $(addprefix out/, $(patsubst %.c,%.to,$(THUMB_SRCS)))



vpath %.elf out $(shell mkdir -p out)
vpath %.o out
vpath %.to out
vpath %.cd out
vpath %.Sd out
vpath %.td out
vpath %.c $(EE_VPATH) $(APPBASE)
vpath %.S $(EE_VPATH) $(APPBASE)
vpath %.src out

out/%.o: %.asm
	$(VERBOSE_PRINTASM)
	$(EE_ASM) -o $@ -co "$<" $(OPT_CC) $(DEFS_CC)

out/%.o: %.c
	$(VERBOSE_PRINTCC)
	$(EE_CC) -t -o $@ -co "$<" $(OPT_CC) $(DEFS_CC)

##
## Main rules: all clean
##

.PHONY: all clean

all:: out/tricore1.elf

clean::
	@echo CLEAN
	@-rm -rf out

##
## ELF file creation
##


out/tricore1.elf: $(OBJS) $(THUMB_OBJS) $(LINKDEP) \
			$(LIB_OBJS) 
	@echo LD
	$(EE_LINK) -o"out/$@" -d"_linker.lsl" $(OPT_LINK)
