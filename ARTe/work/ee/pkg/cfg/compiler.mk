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

## Author: 2005 Paolo Gai
## CVS: $Id: compiler.mk,v 1.18 2006/12/06 17:39:08 pj Exp $

##
## Compiler related options
##

# -------------------------------------------------------------

ifeq ($(call iseeopt, __AVR5__), yes)
include $(PKGBASE)/cfg/arch/cc_avr5gnu.mk
endif

ifeq ($(call iseeopt, __AVR8__), yes)
include $(PKGBASE)/cfg/arch/cc_avr8gnu.mk
endif

# -------------------------------------------------------------

ifeq ($(call iseeopt, __ARM7GNU__), yes)
include $(PKGBASE)/cfg/arch/cc_arm7gnu.mk
endif

ifeq ($(call iseeopt, __ARM7ADS__), yes)
include $(PKGBASE)/cfg/arch/cc_arm7ads.mk
endif

# -------------------------------------------------------------

ifeq ($(call iseeopt, __MPC5XX__), yes)
include $(PKGBASE)/cfg/arch/cc_ppcgnu.mk
endif

ifeq ($(call iseeopt, __MPC5PROTECTED__), yes)
include $(PKGBASE)/cfg/arch/cc_ppcgnu.mk
endif

ifeq ($(or $(call iseeopt, __PPCE200ZX__), $(call iseeopt, __PPCE200Z7__)), yes)
ifeq ($(call iseeopt, __CODEWARRIOR__), yes)
include $(PKGBASE)/cfg/arch/cc_ppc_codewarrior.mk
else # CODEWARRIOR
ifeq ($(call iseeopt, __DIAB__), yes)
include $(PKGBASE)/cfg/arch/cc_ppc_diab.mk
endif # DIAB
endif # CODEWARRIOR
endif # PPCE200ZX

# -------------------------------------------------------------

ifeq ($(call iseeopt, __PIC30__), yes)
include $(PKGBASE)/cfg/arch/cc_pic30.mk
endif

ifeq ($(call iseeopt, __PIC32__), yes)
include $(PKGBASE)/cfg/arch/cc_pic32.mk
endif

# -------------------------------------------------------------

ifeq ($(call iseeopt, __TRICORE1_TASKING__), yes)
include $(PKGBASE)/cfg/arch/cc_tricore_tasking.mk
endif

ifeq ($(call iseeopt, __TRICORE_GNU__), yes)
include $(PKGBASE)/cfg/arch/cc_tricore_gnu.mk
endif

ifeq ($(call iseeopt, EE_TRICORE__), yes)
ifeq ($(call iseeopt, EE_TASKING__), yes)
include $(PKGBASE)/cfg/arch/cc_tricore_tasking_new.mk
endif # EE_TASKING__
ifeq ($(call iseeopt, EE_GNU__), yes)
include $(PKGBASE)/cfg/arch/cc_tricore_gnu_new.mk
endif # EE_GNU__
ifeq ($(call iseeopt, EE_DIAB__), yes)
include $(PKGBASE)/cfg/arch/cc_tricore_diab.mk
endif # EE_DIAB__
endif # EE_TRICORE__

# -------------------------------------------------------------

ifeq ($(call iseeopt, __MC9S12__), yes)
EE_HC12_CC=YES
endif

ifeq ($(call iseeopt, __HCS12XS__), yes)
EE_HC12_CC=YES
endif

ifeq ($(EE_HC12_CC), YES)
 ifeq ($(call iseeopt, __CODEWARRIOR__), yes)
  include $(PKGBASE)/cfg/arch/cc_s12_codewarrior.mk
 else
  include $(PKGBASE)/cfg/arch/cc_s12_cosmic.mk
 endif
endif

# -------------------------------------------------------------

ifeq ($(call iseeopt, __LM32__), yes)
include $(EEBASE)/pkg/cfg/arch/cc_mico32.mk
endif

# -------------------------------------------------------------

ifeq ($(call iseeopt, __ESI_RISC__), yes)
include $(EEBASE)/pkg/cfg/arch/cc_esi_risc.mk
endif

# -------------------------------------------------------------

ifeq ($(call iseeopt, __MSP430__), yes)
include $(EEBASE)/pkg/cfg/arch/cc_msp430.mk
endif

# -------------------------------------------------------------

#ifeq ($(and $(call iseeopt, __CORTEX_M0__), $(call iseeopt, __IAR__)), yes)
#include $(EEBASE)/pkg/cfg/arch/cc_cortex_m0_iar.mk
#endif

ifeq ($(call iseeopt, __CORTEX_M0__), yes)
ifeq ($(call iseeopt, __IAR__), yes)
include $(EEBASE)/pkg/cfg/arch/cc_cortex_m0_iar.mk
else	# __IAR__
ifeq ($(call iseeopt, __KEIL__), yes)
include $(EEBASE)/pkg/cfg/arch/cc_cortex_m0_keil.mk
else	# __KEIL__
ifeq ($(call iseeopt, __GNU__), yes)
include $(EEBASE)/pkg/cfg/arch/cc_cortex_m0_gnu.mk
endif	# __GNU__
endif	# !__KEIL__
endif	# !__IAR__
endif	# __CORTEX_M0__

ifeq ($(call iseeopt, __CORTEX_M3__), yes)
# MM: ToDo
#ifeq ($(call iseeopt, __KEIL__), yes)
#include $(EEBASE)/pkg/cfg/arch/cc_cortex_m3_keil.mk
#else	# __KEIL__
ifeq ($(call iseeopt, __GNU__), yes)
include $(EEBASE)/pkg/cfg/arch/cc_cortex_m3_gnu.mk
endif	# __GNU__
#endif	# !__KEIL__
endif	# __CORTEX_M3__

ifeq ($(call iseeopt, __CORTEX_M4__), yes)
ifeq ($(call iseeopt, __CCS__), yes)
include $(EEBASE)/pkg/cfg/arch/cc_cortex_m4_ccs.mk
else	# __CCS__
ifeq ($(call iseeopt, __KEIL__), yes)
include $(EEBASE)/pkg/cfg/arch/cc_cortex_m4_keil.mk
else	# __KEIL__
ifeq ($(call iseeopt, __GNU__), yes)
include $(EEBASE)/pkg/cfg/arch/cc_cortex_m4_gnu.mk
endif	# __GNU__
endif	# !__KEIL__
endif	# !__CCS__
endif	# __CORTEX_M4__

ifeq ($(call iseeopt, __RX200__), yes)
include $(EEBASE)/pkg/cfg/arch/cc_rx200_ccrx.mk
endif


