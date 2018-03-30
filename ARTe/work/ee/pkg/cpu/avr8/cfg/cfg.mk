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

ifeq ($(call iseeopt, __AVR8__), yes)

ifeq ($(call iseeopt, __ATXMEGA__), yes)
EE_SRCS += pkg/cpu/avr8/src/ee_avr8_irqsched.S
endif
EE_SRCS += pkg/cpu/avr8/src/ee_avr8_irqstub.c
EE_SRCS += pkg/cpu/avr8/src/ee_avr8_utils.c

ifeq ($(call iseeopt, __MULTI__), yes)
EE_SRCS += pkg/cpu/avr8/src/ee_avr8_context.S
endif

ifeq ($(call iseeopt, __OO_BCC1__), yes)
CPU_OO=YES
endif
ifeq ($(call iseeopt, __OO_BCC2__), yes)
CPU_OO=YES
endif
ifeq ($(call iseeopt, __OO_ECC1__), yes)
CPU_OO=YES
endif
ifeq ($(call iseeopt, __OO_ECC2__), yes)
CPU_OO=YES
endif

ifeq ($(CPU_OO), YES)
EE_SRCS += pkg/cpu/avr8/src/ee_avr8_oo.S
endif

endif # __AVR8__

