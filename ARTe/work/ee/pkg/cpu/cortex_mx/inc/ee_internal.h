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
	@file	ee_internal.h
	@brief	Derived from cpu/pic30/inc/ee_internal.h
	@author	Gianluca Franchino
	@author	Giuseppe Serano
	@date	2012
*/  

#ifndef __INCLUDE_CORTEX_MX_INTERNAL_H__
#define __INCLUDE_CORTEX_MX_INTERNAL_H__


#include "cpu/cortex_mx/inc/ee_cpu.h"


/*************************************************************************
 Functions
 *************************************************************************/

/*
 * Generic Primitives
 */

#include "cpu/common/inc/ee_primitives.h"

/*************************************************************************
                            System startup
 *************************************************************************/

#define __OO_CPU_HAS_STARTOS_ROUTINE__

/* If system is defined I have to initialize it*/
#if	( defined(ENABLE_SYSTEM_TIMER) && defined(EE_SYSTEM_TIMER_DEVICE) )
void EE_cortex_mx_initialize_system_timer(void);
#else /* ENABLE_SYSTEM_TIMER */
#define EE_cortex_mx_initialize_system_timer() ((void) 0)
#endif /* ENABLE_SYSTEM_TIMER */

static int __ALWAYS_INLINE__ EE_cpu_startos(void);
__INLINE__ int __ALWAYS_INLINE__ EE_cpu_startos(void)
{
  EE_system_init();
  EE_cortex_mx_initialize_system_timer();
  return 0;
}


/** Called as _first_ function of a primitive that can be called in
   an IRQ and in a task */
__INLINE__ EE_FREG __ALWAYS_INLINE__ EE_hal_begin_nested_primitive(void)
{
    return EE_cortex_mx_suspendIRQ();
}


/** Called as _last_ function of a primitive that can be called in
   an IRQ and in a task.  Enable IRQs if they were enabled before entering. */
__INLINE__ void __ALWAYS_INLINE__ EE_hal_end_nested_primitive(EE_FREG f)
{
    EE_cortex_mx_resumeIRQ(f);
}

/* Used to get internal CPU priority. */
__INLINE__ EE_TYPEISR2PRIO __ALWAYS_INLINE__ EE_hal_get_int_prio(void)
{
  return EE_cortex_mx_get_int_prio();
}

/* Used to set internal CPU priority. */
__INLINE__ void __ALWAYS_INLINE__ EE_hal_set_int_prio(EE_TYPEISR2PRIO prio)
{
  EE_cortex_mx_set_int_prio(prio);
}

/*
 * Used to change internal CPU priority and return a status flag mask.
 *
 * Note:	EE_FREG param flag and return value needed only for according to
 * 		HAL interface.
 */
__INLINE__ EE_FREG __ALWAYS_INLINE__ EE_hal_change_int_prio(
  EE_TYPEISR2PRIO prio, EE_FREG flag
)
{
  EE_hal_set_int_prio(prio);
  return flag;
}

/*
 * Used to raise internal CPU interrupt priority if param new_prio is greater
 * than actual priority.
 *
 * Note:	EE_FREG param flag and return value needed only for according to
 * 		HAL interface.
 */
__INLINE__ EE_FREG __ALWAYS_INLINE__ EE_hal_raise_int_prio_if_less(
  EE_TYPEISR2PRIO new_prio, EE_FREG flag
)
{
  register EE_TYPEISR2PRIO prev_prio = EE_cortex_mx_get_int_prio();
  if(((new_prio != 0U) && (prev_prio > new_prio)) || (prev_prio == 0)) {
    EE_cortex_mx_set_int_prio(new_prio);
  }
  return flag;
}

/*
 * Used to check internal CPU interrupt priority if param new_prio is greater
 * than actual priority.
 */
__INLINE__ EE_BIT __ALWAYS_INLINE__ EE_hal_check_int_prio_if_higher(
  EE_TYPEISR2PRIO new_prio
)
{
  register EE_TYPEISR2PRIO prev_prio = EE_cortex_mx_get_int_prio();
  return ((prev_prio != 0U) && ((prev_prio < new_prio) || (new_prio == 0U)));
}

/* 
 * Context Handling  
 */

#include "cpu/cortex_mx/inc/ee_context.h"

/* Launch a new task on the current stack, clean up the task after it ends, and
 * call the scheduler.  Return the next task to launch, which is "marked as
 * stacked" if there is no new task to launch. */
EE_TID EE_std_run_task_code(EE_TID tid);

/* typically called at the end of an interrupt */
#define EE_hal_IRQ_stacked	EE_hal_endcycle_stacked
#define EE_hal_IRQ_ready	EE_hal_endcycle_ready

/* 
 * OO TerminateTask related stuffs
 */

#if defined(__OO_BCC1__) || defined(__OO_BCC2__) || defined(__OO_ECC1__) || defined(__OO_ECC2__)

/** Save the context and call the body of the task `tid'.  Implemented in
 * assembly */
void EE_hal_terminate_savestk(EE_TID tid);

/** Restore the context saved by EE_hal_terminate_savestk() for the task `tid' and
 * return from EE_hal_terminate_savestk().  Implemented in assembly */
NORETURN void EE_hal_terminate_task(EE_TID tid);

#endif /* __OO_BCCx */


#endif /* __INCLUDE_CORTEX_MX_INTERNAL_H__ */
