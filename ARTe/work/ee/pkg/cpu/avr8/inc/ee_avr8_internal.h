/* ###*B*###
 * ERIKA Enterprise - a tiny RTOS for small microcontrollers
 *
 * Copyright (C) 2002-2013  Evidence Srl
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

/*
 * Author: 2013,  Giuseppe Serano
 */

#ifndef __INCLUDE_EE_AVR8_INTERNAL_H__
#define __INCLUDE_EE_AVR8_INTERNAL_H__

#include "cpu/avr8/inc/ee_avr8_cpu.h"

/*************************************************************************
 Macros
 *************************************************************************/

/*
 * Provide global symbol and declare an assembly label.
 */
#define	EE_AVR8_PROVIDE_SYM(symbol)				\
	__asm__ __volatile__(					\
		".global " EE_PREPROC_STRING(symbol) "\n"	\
		EE_PREPROC_STRING(symbol) ":\n"			\
		::						\
	)

/*
 * Access a global symbol.
 */
#define	EE_AVR8_REQUEST_SYM(symbol)				\
	__asm__ __volatile__(					\
		".global " EE_PREPROC_STRING(symbol)		\
		"\n ldi r30, lo8(" EE_PREPROC_STRING(symbol)	\
		")\n" ::					\
	)

/*************************************************************************
 Functions
 *************************************************************************/

/*
 * Generic Primitives
 */
#include "cpu/common/inc/ee_primitives.h"

/* called as _first_ function of a primitive that can be called into
   an IRQ and into a task */
__INLINE__ EE_FREG EE_hal_begin_nested_primitive(void)
{
  return EE_hal_suspendIRQ();
}

/* called as _last_ function of a primitive that can be called into
   an IRQ and into a task */



__INLINE__ void EE_hal_end_nested_primitive(EE_FREG f)
{
  EE_hal_resumeIRQ(f);
}

/* 
 * Context Handling  
 */

#include "cpu/common/inc/ee_context.h"

/* typically called at the end of an interrupt */
#define EE_hal_IRQ_stacked	EE_hal_endcycle_stacked
#define EE_hal_IRQ_ready	EE_hal_endcycle_ready

/* 
 * OO TerminateTask related stuffs
 */

#if	( \
  defined(__OO_BCC1__) || defined(__OO_BCC2__) || \
  defined(__OO_ECC1__) || defined(__OO_ECC2__) \
)

/* Implemented in Assembly in ee_avr8_oo.S */
extern void EE_avr8_terminate_savestk(EE_ADDR sp, EE_ADDR realbody);
extern void EE_avr8_terminate_task(EE_ADDR sp);

__INLINE__ void __ALWAYS_INLINE__ EE_hal_terminate_savestk(EE_TID t)
{
  EE_avr8_terminate_savestk(
    &EE_terminate_data[t],
    (EE_ADDR)EE_terminate_real_th_body[t]
  );
}

__INLINE__ void __ALWAYS_INLINE__ EE_hal_terminate_task(EE_TID t)
{
  EE_avr8_terminate_task(&EE_terminate_data[t]);
}

#endif	/*
	 * defined(__OO_BCC1__) || defined(__OO_BCC2__) ||
	 * defined(__OOSECC1__) || defined(__OO_ECC2__)
	 */ 

#endif	/* __INCLUDE_EE_AVR8_INTERNAL_H__ */

