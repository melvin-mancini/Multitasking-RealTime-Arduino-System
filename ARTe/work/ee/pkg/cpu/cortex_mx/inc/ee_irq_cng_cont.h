/* ###*B*###
 * ERIKA Enterprise - a tiny RTOS for small microcontrollers
 *
 * Copyright (C) 2002-2011  Evidence Srl
 *
 * This file is part of ERIKA Enterprise.
 *
 * ERIKA Enterprise is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation, 
 * (with a special exception described below).
 *
 * Linking this code statically or dynamically with other modules is
 * making a combined work based on this code.  Thus, the terms and
 * conditions of the GNU General Public License cover the whole
 * combination.
 *
 * As a special exception, the copyright holders of this library give you
 * permission to link this code with independent modules to produce an
 * executable, regardless of the license terms of these independent
 * modules, and to copy and distribute the resulting executable under
 * terms of your choice, provided that you also meet, for each linked
 * independent module, the terms and conditions of the license of that
 * module.  An independent module is a module which is not derived from
 * or based on this library.  If you modify this code, you may extend
 * this exception to your version of the code, but you are not
 * obligated to do so.  If you do not wish to do so, delete this
 * exception statement from your version.
 *
 * ERIKA Enterprise is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License version 2 for more details.
 *
 * You should have received a copy of the GNU General Public License
 * version 2 along with ERIKA Enterprise; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301 USA.
 * ###*E*### */

 /** 
	
	@file	ee_irq_cng_cont.h
	@brief	Function active the context change interrupt
	@author	Gianluca Franchino
	@author	Giuseppe Serano
	@date	2012
*/ 

#ifndef __INCLUDE_CORTEX_MX_IRQ_CNG_CONTEXT_H__
#define __INCLUDE_CORTEX_MX_IRQ_CNG_CONTEXT_H__

#ifdef __IAR__
#include "cpu/common/inc/ee_compiler_iar.h"
#else	/* __IAR__ */
#ifdef __CCS__
#include "cpu/common/inc/ee_compiler_ccs.h"
#else	/* __CCS__ */
#ifdef __KEIL__
#include "cpu/common/inc/ee_compiler_keil.h"
#else	/* __KEIL__ */
#ifdef __GNU__
#include "cpu/common/inc/ee_compiler_gcc.h"
#else	/* __GNU__ */
#error Unsupported compiler
#endif	/* !__GNU__ */
#endif	/* !__KEIL__ */
#endif	/* !__CCS__ */
#endif	/* !__IAR__ */

/* Implemented in ee_<compiler>_change_context_isr.s */
extern void EE_switch_context(void);

__INLINE__ void __ALWAYS_INLINE__ EE_cortex_mx_IRQ_active_change_context(void)
{
	EE_switch_context();
}

#endif /* __INCLUDE_CORTEX_MX_IRQ_CNG_CONTEXT_H__ */

