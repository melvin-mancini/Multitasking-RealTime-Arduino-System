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

ifeq ($(call iseeopt, __ATXMEGA__), yes)

EE_SRCS += pkg/mcu/atmel_atxmega/src/ee_atmel_atxmega_mcu.c
EE_SRCS += pkg/mcu/atmel_atxmega/src/ee_atmel_atxmega_intvect.c

ifeq ($(call iseeopt, __E2P_DRIVER__), yes)
EE_SRCS += pkg/mcu/atmel_atxmega/src/ee_atmel_atxmega_e2p.c
endif

ifeq ($(call iseeopt, __GPIO_DRIVER__), yes)
EE_SRCS += pkg/mcu/atmel_atxmega/src/ee_atmel_atxmega_gpio.c
endif

ifeq ($(call iseeopt, __GPT_DRIVER__), yes)
EE_SRCS += pkg/mcu/atmel_atxmega/src/ee_atmel_atxmega_gpt.c
endif

#~ ifeq ($(call iseeopt, __I2C_DRIVER__), yes)
#~ EE_SRCS += pkg/mcu/atmel_atxmega/src/ee_atmel_atxmega_i2c.c
#~ endif


ifeq ($(call iseeopt, __PWM_DRIVER__), yes)
EE_SRCS += pkg/mcu/atmel_atxmega/src/ee_atmel_atxmega_pwm.c
endif

ifeq ($(call iseeopt, __RTC_DRIVER__), yes)
EE_SRCS += pkg/mcu/atmel_atxmega/src/ee_atmel_atxmega_rtc.c
endif

ifeq ($(call iseeopt, __SCI_DRIVER__), yes)
EE_SRCS += pkg/mcu/atmel_atxmega/src/ee_atmel_atxmega_sci.c
endif

ifeq ($(or $(call iseeopt, __GPT_DRIVER__), $(call iseeopt, __PWM_DRIVER__), $(call iseeopt, __ICU_DRIVER__)), yes)
ifeq ($(call iseeopt, __USE_TC_HW__), yes)
EE_SRCS += pkg/mcu/atmel_atxmega/src/ee_atmel_atxmega_tc_hw.c
else
EE_SRCS += pkg/mcu/atmel_atxmega/src/ee_atmel_atxmega_tcx0.c
endif
endif

endif	# EEOPT __ATXMEGA__
