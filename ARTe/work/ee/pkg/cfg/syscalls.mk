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

## Author: 2004 Paolo Gai
## CVS: $Id: syscalls.mk,v 1.6 2007/01/29 09:15:49 pj Exp $

# This file contains a set of scripts that are included by all the
# architectures that implement memory protection. In particular, tehse
# scripts are used to automatically generate the data structures
# needed to call the API functions.

# Usage of the following scripts:

# inside each CFG.MK there must be a line like:
# EE_SYSCALLS = list
# where list is a list of the exported function names.

# Then, the makefile of the protected configurations have to somehow
# call the following target (typically it should be a prerequisite of
# the dependencies


ifeq ($(call iseeopt, __PROTECTED__), yes)
# note: I cannot generate ee_libapi.c inside out/syscall because it is
# not created yet, so the makefile does not put it into VPATH!!!
EE_SYSCALLS_PREREQUISITES = out/syscalls/ee_libapi.h
EE_SRCS += ee_libapi.c
EE_VPATH += out/syscalls $(shell mkdir -p out/syscalls)
ALLINCPATH += -Iout/syscalls
endif

out/syscalls/ee_libapi.h:
	@echo LIBAPI ee_libapi.h
!	@echo $(EE_SYSCALLS) | awk '{ printf("/*  ee_libapi.h - Automatically generated file - DO NOT CHANGE IT!!! */\n\n#ifndef __INCLUDE_EE_LIBAPI_H__\n#define __INCLUDE_EE_LIBAPI_H__\n\n"); for (i=1; i<=NF; i++) { printf("#define EE_API_ID_%s %d\n", toupper($$i), i-1) } printf("\n#endif\n") }' > $@

out/syscalls/ee_libapi.c:
	@echo LIBAPI ee_libapi.c
	@echo $(EE_SYSCALLS) | awk '{printf("/*  ee_libapi.c - Automatically generated file - DO NOT CHANGE IT!!! */\n\n#include \"ee.h\"\n#include \"ee_internal.h\"\nconst EE_ADDR EE_hal_system_call[%d] = {\n",NF); for (i=1; i<=NF; i++) { printf("\t(EE_ADDR)%s", $$i); if (i==NF) { printf("\n") } else { printf(",\n") } } printf("};\n") }' > $@
