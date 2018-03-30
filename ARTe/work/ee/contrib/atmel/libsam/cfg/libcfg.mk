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

## Author: 2013 Mauro Marinoni

##Check if is a compatible microcontroller
ifeq ($(or $(call iseeopt, __SAM3__), $(call iseeopt, __SAM4__)), yes)

## Check the STM32F4XX_SPD library selection
ifeq ($(call islibopt,__LIB_ATMEL_SAM_DEV__), yes)
INCLUDE_ATMEL_SAM_DEV = YES
endif

## Check if it's selected all libs inclusion
ifeq ($(call iseeopt, __BUILD_ALL_LIBS__), yes)
INCLUDE_ATMEL_SAM_DEV = YES
endif

endif

##
## If the library is required
##
ifeq ($(INCLUDE_ATMEL_SAM_DEV), YES)

#EEOPT += USE_STDPERIPH_DRIVER

##
## Library code
##
## Add the inc path to the include pathlist
##New include mechanism

INCLUDE_PATH += $(EEBASE)/contrib/atmel/CMSIS/CMSIS/Include
INCLUDE_PATH += $(EEBASE)/contrib/atmel/CMSIS/Device/ATMEL
INCLUDE_PATH += $(EEBASE)/contrib/atmel/libsam
INCLUDE_PATH += $(EEBASE)/contrib/atmel/libsam/include


## Add each file individually
ifeq ($(or $(call iseeopt, __USE_ATMEL_SAM_DEV_ALL__), $(call iseeopt, __USE_ATMEL_SAM_DEV_ADC__)), yes) 
EE_SRCS_ATMEL_SAM_DEV += contrib/atmel/libsam/source/adc.c
EE_SRCS_ATMEL_SAM_DEV += contrib/atmel/libsam/source/adc12_sam3u.c
endif

ifeq ($(or $(call iseeopt, __USE_ATMEL_SAM_DEV_ALL__), $(call iseeopt, __USE_ATMEL_SAM_DEV_DAC__)), yes) 
EE_SRCS_ATMEL_SAM_DEV += contrib/atmel/libsam/source/dacc.c
endif

ifeq ($(or $(call iseeopt, __USE_ATMEL_SAM_DEV_ALL__), $(call iseeopt, __USE_ATMEL_SAM_DEV_NVIC__)), yes) 
EE_SRCS_ATMEL_SAM_DEV += contrib/atmel/libsam/source/interrupt_sam_nvic.c
endif

ifeq ($(or $(call iseeopt, __USE_ATMEL_SAM_DEV_ALL__), $(call iseeopt, __USE_ATMEL_SAM_DEV_PIO__)), yes) 
EE_SRCS_ATMEL_SAM_DEV += contrib/atmel/libsam/source/pio.c
endif

ifeq ($(or $(call iseeopt, __USE_ATMEL_SAM_DEV_ALL__), $(call iseeopt, __USE_ATMEL_SAM_DEV_PMC__)), yes) 
EE_SRCS_ATMEL_SAM_DEV += contrib/atmel/libsam/source/pmc.c
endif

ifeq ($(or $(call iseeopt, __USE_ATMEL_SAM_DEV_ALL__), $(call iseeopt, __USE_ATMEL_SAM_DEV_PWMC__)), yes) 
EE_SRCS_ATMEL_SAM_DEV += contrib/atmel/libsam/source/pwmc.c
endif

ifeq ($(or $(call iseeopt, __USE_ATMEL_SAM_DEV_ALL__), $(call iseeopt, __USE_ATMEL_SAM_DEV_RTC__)), yes) 
EE_SRCS_ATMEL_SAM_DEV += contrib/atmel/libsam/source/rtc.c
endif

ifeq ($(or $(call iseeopt, __USE_ATMEL_SAM_DEV_ALL__), $(call iseeopt, __USE_ATMEL_SAM_DEV_RTTC__)), yes) 
EE_SRCS_ATMEL_SAM_DEV += contrib/atmel/libsam/source/rtt.c
endif

ifeq ($(or $(call iseeopt, __USE_ATMEL_SAM_DEV_ALL__), $(call iseeopt, __USE_ATMEL_SAM_DEV_SPI__)), yes) 
EE_SRCS_ATMEL_SAM_DEV += contrib/atmel/libsam/source/spi.c
endif

ifeq ($(or $(call iseeopt, __USE_ATMEL_SAM_DEV_ALL__), $(call iseeopt, __USE_ATMEL_SAM_DEV_TC__)), yes) 
EE_SRCS_ATMEL_SAM_DEV += contrib/atmel/libsam/source/tc.c
endif

ifeq ($(or $(call iseeopt, __USE_ATMEL_SAM_DEV_ALL__), $(call iseeopt, __USE_ATMEL_SAM_DEV_TIMETICK__)), yes) 
EE_SRCS_ATMEL_SAM_DEV += contrib/atmel/libsam/source/timetick.c
endif

ifeq ($(or $(call iseeopt, __USE_ATMEL_SAM_DEV_ALL__), $(call iseeopt, __USE_ATMEL_SAM_DEV_TWI__)), yes) 
EE_SRCS_ATMEL_SAM_DEV += contrib/atmel/libsam/source/twi.c
endif

ifeq ($(or $(call iseeopt, __USE_ATMEL_SAM_DEV_ALL__), $(call iseeopt, __USE_ATMEL_SAM_DEV_UDP__)), yes) 
EE_SRCS_ATMEL_SAM_DEV += contrib/atmel/libsam/source/udp.c
EE_SRCS_ATMEL_SAM_DEV += contrib/atmel/libsam/source/udphs.c
endif

ifeq ($(or $(call iseeopt, __USE_ATMEL_SAM_DEV_ALL__), $(call iseeopt, __USE_ATMEL_SAM_DEV_USB_OTG__)), yes) 
EE_SRCS_ATMEL_SAM_DEV += contrib/atmel/libsam/source/uotghs.c
EE_SRCS_ATMEL_SAM_DEV += contrib/atmel/libsam/source/uotghs_device.c
EE_SRCS_ATMEL_SAM_DEV += contrib/atmel/libsam/source/uotghs_host.c
endif

ifeq ($(or $(call iseeopt, __USE_ATMEL_SAM_DEV_ALL__), $(call iseeopt, __USE_ATMEL_SAM_DEV_USART__)), yes) 
EE_SRCS_ATMEL_SAM_DEV += contrib/atmel/libsam/source/usart.c
endif

ifeq ($(or $(call iseeopt, __USE_ATMEL_SAM_DEV_ALL__), $(call iseeopt, __USE_ATMEL_SAM_DEV_WDT__)), yes) 
EE_SRCS_ATMEL_SAM_DEV += contrib/atmel/libsam/source/wdt.c
endif



## If the file list is not empty, create the libatmelsamdev lib
ifneq ($(EE_SRCS_ATMEL_SAM_DEV),)
EE_OBJS_ATMEL_SAM_DEV := $(addprefix $(OBJDIR)/, $(patsubst %.c,%.o,$(patsubst %.s,%.o, $(EE_SRCS_ATMEL_SAM_DEV))))
LIBSRCS += $(EE_SRCS_ATMEL_SAM_DEV)

ATMELSAMDEVLIB := libatmelsamdev.a

$(ATMELSAMDEVLIB): $(EE_OBJS_ATMEL_SAM_DEV)
	@echo "AR    $(ATMELSAMDEVLIB)";
	$(QUIET)$(EE_AR) $(COMPUTED_OPT_AR) $@ $^
OPT_LIBS += $(ATMELSAMDEVLIB)
ALL_LIBS += $(ATMELSAMDEVLIB)

endif

endif

