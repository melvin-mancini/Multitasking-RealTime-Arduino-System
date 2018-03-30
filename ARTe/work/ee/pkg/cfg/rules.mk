# ###*B*###
# ERIKA Enterprise - a tiny RTOS for small microcontrollers
# 
# Copyright (C) 2002-2009  Evidence Srl
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

## Authors: 2001-2002 Paolo Gai, Enrico Bini, Alessandro Colantonio
## 2002- Paolo Gai
## 2005 Antonio Romano
## 2010 Bernardo  Dal Seno
## CVS: $Id: rules.mk,v 1.33 2008/01/14 10:35:34 pj Exp $
## 2013 Eugen Kleinschmidt

# Include templates and functions to handle paths
include $(EEBASE)/pkg/cfg/path_helper.mk

# Add scicos generated make
ifeq ($(findstring app.mk,$(notdir $(wildcard $(APPBASE)/*.mk))), app.mk)
include $(APPBASE)/app.mk
endif


# Enable libs support
ifeq ($(call iseeopt,__BUILD_ALL_LIBS__), yes)
ENABLE_LIBS:=TRUE
ONLY_LIBS:=TRUE
endif
ifeq ($(call iseeopt,__BUILD_LIBS__), yes)
ENABLE_LIBS:=TRUE
ONLY_LIBS:=TRUE
endif
ifeq ($(call iseeopt,__ADD_LIBS__), yes)
ENABLE_LIBS:=TRUE
endif

# Microchip dsPIC specific
ifeq ($(GENERATE_MPLABIDE_LIBS), TRUE)
ENABLE_LIBS:=TRUE
endif


# EEOPT is used to appropriately configure and compile the particular
# application. Symbols specified in EEOPT are implicitly defined when
# compiling the application using the -D compiler option

# The simbol __ERIKA__ in EE_OPT is used to check the OS in the
# contrib libraries.

EEOPT += __ERIKA__

# EEALLOPT is deprecated (binary distributions have not been around for a while)
# This definition should raise an error if its value is ever used
EEALLOPT=$(error ERROR: use of EEALLOPT)


##
## default automatic inserted dependencies
##########################################################################

ifeq ($(call iseeopt, __EDF__), yes)
ifneq ($(call iseeopt, __TIME_SUPPORT__), yes)
EEOPT += __TIME_SUPPORT__
endif
endif

#ifeq ($(call iseeopt, __MULTI__), yes)
#ifeq ($(call iseeopt, __IRQ_STACK_NEEDED__), yes)
#else
#EEOPT += __IRQ_STACK_NEEDED__
#endif
#endif

ifeq ($(call iseeopt, __IRQ_STACK_NEEDED__), yes)
ifneq ($(call iseeopt, __MULTI__), yes)
EEOPT += __MULTI__
endif
endif

ifeq ($(call iseeopt, __COM_CCC0__), yes)
EEOPT += __ALARMS__
endif

ifeq ($(call iseeopt, __COM_CCC1__), yes)
EEOPT += __ALARMS__
endif

ifeq ($(call iseeopt, __EVALUATOR7T__), yes)
EEOPT += __SAMSUNG_KS32C50100__
endif

ifeq ($(call iseeopt, __MPC5PROTECTED__), yes)
EEOPT += __PROTECTED__
endif

# Bugfix: to be removed!
ifeq ($(call iseeopt, __unibo_mparm__), yes)
EEOPT += __UNIBO_MPARM__
endif

# Bugfix: to be removed!
ifeq ($(call iseeopt, __USE_DEMOBOARD__), yes)
else
ifeq ($(call iseeopt, __USE_MOTIONBOARD__), yes)
else
ifeq ($(call iseeopt, __USE_USB__), yes)
EEOPT += __USE_MOTIONBOARD__
EE_SRCS += pkg/board/ee_flex/src/ee_flex_motionboard.c
endif # __USE_USB__
endif # __USE_MOTIONBOARD__
endif # __USE_DEMOBOARD__

##
## H8/300 - gcc under GNU/Linux
##########################################################################
ifeq ($(call iseeopt, __H8__), yes) 
  include $(EEBASE)/pkg/cfg/arch/rules_lego_rcx.mk
endif 

##
## MPC5XX - gcc under GNU/Linux
##########################################################################
ifeq ($(and $(call iseeopt, __MPC5XX__), $(call iseeopt, __MPC566EVB__)), yes) 
include $(EEBASE)/pkg/cfg/arch/rules_axiom_mpc566evb.mk
endif

ifeq ($(and $(call iseeopt, __MPC5PROTECTED__), \
	$(call iseeopt, __MPC566EVB__)), yes)
include $(EEBASE)/pkg/cfg/arch/rules_axiom_mpc566evb.mk
endif

##
## PPC - diab under GNU/Linux or Cygwin
##########################################################################
ifeq ($(or $(call iseeopt, __PPCE200Z6__), $(call iseeopt, __PPCE200Z7__), \
	$(call iseeopt, __PPCE200Z0__), $(call iseeopt, EE_PPCE200Z4)), yes)
EEOPT +=  __PPCE200ZX__
endif
ifeq ($(call iseeopt, __PPCE200ZX__), yes)
ifeq ($(or $(call iseeopt, __MPC5674F__), $(call iseeopt, __MPC5668__), \
	$(call iseeopt, __MPC5668G__), $(call iseeopt, EE_MPC5643L) , $(call iseeopt, EE_MPC5644A)), yes)
include $(EEBASE)/pkg/cfg/arch/rules_ppc_generic.mk
endif
endif # __PPCE200ZX__

##
## ARM7TDMI - GNU gcc under GNU/Linux or Cygwin
##########################################################################
ifeq ($(call iseeopt, __ARM7GNU__), yes)

ifeq ($(call iseeopt, __EVALUATOR7T__), yes)
include $(EEBASE)/pkg/cfg/arch/rules_arm_evaluator7t.mk
endif

ifeq ($(call iseeopt, __UNIBO_MPARM__), yes)
include $(EEBASE)/pkg/cfg/arch/rules_unibo_mparm.mk
endif

ifeq ($(call iseeopt, __TRISCENDA7S__), yes)
include $(EEBASE)/pkg/cfg/arch/rules_triscend_a7s.mak
endif

endif # __ARM7GNU__


##
## AVR5 
##########################################################################
ifeq ($(call iseeopt, __AVR5__), yes)

##
##  ATmega128 - GNU - stk500
##########################################################################
ifeq ($(call iseeopt, __ATMEL_STK50X__), yes)
include $(EEBASE)/pkg/cfg/arch/rules_atmel_stk500.mk
endif 

##
##  ATmega128 - GNU - mica boarb mib510
##########################################################################

ifeq ($(call iseeopt, __XBOW_MIB5X0__), yes)
include $(EEBASE)/pkg/cfg/arch/rules_xbow_mib5x0.mk
endif 

endif # __AVR5__


##
## AVR8
##########################################################################
ifeq ($(call iseeopt, __AVR8__), yes)
include $(EEBASE)/pkg/cfg/arch/rules_avr8.mk
endif


##
## ST10 - Tasking 6.0 under Windows
##########################################################################
ifeq ($(call iseeopt, __ST10__), yes)
include $(EEBASE)/pkg/cfg/arch/rules_ertec_eva167.mk
endif


##
## Altera NIOS2 - gcc under Windows
##########################################################################
ifeq ($(call iseeopt, __NIOS2__), yes)
include $(EEBASE)/pkg/cpu/nios2/cfg/rules.mk
endif


##
## Microchip DSPIC - gcc under Windows
##########################################################################
ifeq ($(call iseeopt, __PIC30__), yes)
include $(EEBASE)/pkg/cfg/arch/rules_microchip_pic30.mk
endif


##
## Microchip PIC32 - gcc
##########################################################################
ifeq ($(call iseeopt, __PIC32__), yes)
include $(EEBASE)/pkg/cfg/arch/rules_microchip_pic32.mk
endif


##
## Infineon Tricore 1 - Tasking under Windows
##########################################################################
ifeq ($(and $(call iseeopt, __TRICORE1_TASKING___), \
	$(call iseeopt, __TC1775B__)), yes)
include $(EEBASE)/pkg/cfg/arch/rules_infineon_tc1775b.mk
endif # __TRICORE1_TASKING___ AND __TC1775B__

##
## Infineon Tricore - Tasking under Windows
##########################################################################
ifeq ($(call iseeopt, EE_TRICORE__), yes)
include $(EEBASE)/pkg/cfg/arch/rules_infineon_tc_generic.mk
endif

##
## Infineon Tricore - GNU
##########################################################################
ifeq ($(call iseeopt, __TRICORE_GNU__), yes)
ifeq ($(or $(call iseeopt, __TC1796__), $(call iseeopt, __TC1797__)), yes)
  EEOPT += __TC179x__
endif
ifeq ($(call iseeopt, __TC179x__), yes)
include $(EEBASE)/pkg/cfg/arch/rules_infineon_tc179x.mk
endif
endif


##
## Freescale S12X - Cosmic compiler or CodeWarrior compiler
##########################################################################
ifeq ($(call iseeopt, __MC9S12__), yes)
EE_HC12_RULES=YES
endif

ifeq ($(call iseeopt, __HCS12XS__), yes)
EE_HC12_RULES=YES
endif

ifeq ($(EE_HC12_RULES), YES)
 ifeq ($(call iseeopt, __CODEWARRIOR__), yes)
  include $(EEBASE)/pkg/cfg/arch/rules_freescale_s12_codewarrior.mk
 else
  include $(EEBASE)/pkg/cfg/arch/rules_freescale_s12_cosmic.mk
 endif
endif


##
## Lattice Mico32 - gcc under Windows or Linux
##########################################################################
ifeq ($(call iseeopt, __LM32__), yes)
include $(EEBASE)/pkg/cfg/arch/rules_mico32.mk
endif

##
## eSi-RISC - gcc under Windows
##########################################################################
ifeq ($(call iseeopt, __ESI_RISC__), yes)
include $(EEBASE)/pkg/cfg/arch/rules_esi_risc.mk
endif

##
## MSP430 - gcc under Linux
##########################################################################
ifeq ($(call iseeopt, __MSP430__), yes) 
  include $(EEBASE)/pkg/cfg/arch/rules_msp430.mk
endif 

##
## CORTEX MX - Texas Instruments Code Composer Studio v4 TMS 470 or IAR
## compilers toolchains under windows
##########################################################################
ifeq ($(call iseeopt, __CORTEX_MX__), yes)
include $(EEBASE)/pkg/cfg/arch/rules_cortex_generic.mk
endif

##
## RX210 - Renesas High-performance Embedded Workshop with ccrx compiler
## compilers toolchains under windows
##########################################################################
ifeq ($(call iseeopt, __RX200__), yes)
include $(EEBASE)/pkg/cfg/arch/rules_rx200_generic.mk
endif

##
## error checking in EEOPT
##########################################################################
ifeq ($(and $(call iseeopt, __MONO__), $(call iseeopt, __MULTI__)), yes)
$(error __MULTI__ and __MONO__ options are not compatible)
endif

ifeq ($(and $(call iseeopt, __IRQ_STACK_NEEDED__), \
	$(call iseeopt, __MONO__)), yes)
$(error __MONO__ and __IRQ_STACK_NEEDED__ options are not compatible)
endif
