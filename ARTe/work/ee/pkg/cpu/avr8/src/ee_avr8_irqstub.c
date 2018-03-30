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
 * ISR2 Stub implementation
 *
 * Author: 2013,  Giuseppe Serano
 */

/* ISR2 Stub declaration */
#include "cpu/avr8/inc/ee_avr8_irqstub.h"

/* 
 * I need Kernel inclusion for callbacks and Common Context Implementation,
 * so I include the whole internal.
 */
#include "ee_internal.h"

/* Plus I need Common End-ISR Scheduler Implementation */
#include "cpu/common/inc/ee_irqstub.h"

#ifdef	__MULTI__
#ifdef	__IRQ_STACK_NEEDED__
struct EE_TOS EE_avr8_old_tos = {(EE_ADDR)0U};
extern	void EE_avr8_IRQ_change_stack( void );
extern	void EE_avr8_IRQ_stack_back( void );
#define	EE_std_IRQ_change_stack()	EE_avr8_IRQ_change_stack()
#define	EE_std_IRQ_stack_back()		EE_avr8_IRQ_stack_back()
#else
#define	EE_std_IRQ_change_stack()	((void)0)
#define	EE_std_IRQ_stack_back()		((void)0)
#endif
#else
#define	EE_std_IRQ_change_stack()	((void)0)
#define	EE_std_IRQ_stack_back()		((void)0)
#endif	/* __MULTI__ */

/* Needed by common irq stub */
#ifdef __ALLOW_NESTED_IRQ__

#define	EE_std_enableIRQ_nested()	EE_avr8_enableIRQ()
#define	EE_std_disableIRQ_nested()	EE_avr8_disableIRQ()

#endif  /* __ALLOW_NESTED_IRQ__ */

/*
 * ISR Type 1 stub.
 */
void EE_COMPILER_KEEP EE_NAKED EE_avr8_isr1_stub(EE_VOID_CALLBACK f)
{

  EE_AVR8_ISR_PREINSTUB();

  /* Disable IRQ */
  EE_hal_disableIRQ();

#ifdef	__IRQ_STACK_NEEDED__
  if ( !EE_is_inside_ISR_call() ) {

    EE_std_IRQ_change_stack();

  }
#endif	/* __IRQ_STACK_NEEDED__ */

  /* Increment nesting level here, with IRQ disabled */
  EE_increment_IRQ_nesting_level();

  /* Enable IRQ if nesting is allowed */
  EE_std_enableIRQ_nested();

  /* Call The ISR User Handler */
  f();

  /* 
   * Disable IRQ if nesting is allowed.
   * Note:	if nesting is not allowed, the IRQs are already disabled
   */
  EE_std_disableIRQ_nested();

  /* Decrement nesting level here, with IRQ disabled */
  EE_decrement_IRQ_nesting_level();

  /* Check for scheduling point */
  if ( !EE_is_inside_ISR_call() ) {

    /* Restore task stack pointer if needed */
    EE_std_IRQ_stack_back();

  }

  EE_AVR8_ISR_POSTINSTUB();

  /* Enable IRQ */
  EE_hal_enableIRQ();

  EE_return();

}	/* EE_avr8_isr1_stub() */

/*
 * ISR Type 2 stub.
 */
void EE_COMPILER_KEEP EE_NAKED EE_avr8_isr2_stub(EE_VOID_CALLBACK f)
{

  EE_AVR8_ISR_PREINSTUB();

  /* Disable IRQ */
  EE_hal_disableIRQ();

#ifdef	__IRQ_STACK_NEEDED__
  if ( !EE_is_inside_ISR_call() ) {

    EE_std_IRQ_change_stack();

  }
#endif	/* __IRQ_STACK_NEEDED__ */

  /* Increment nesting level here, with IRQ disabled */
  EE_increment_IRQ_nesting_level();

  /* Enable IRQ if nesting is allowed */
  EE_std_enableIRQ_nested();

  /* Call The ISR User Handler */
  f();

  /* 
   * Disable IRQ if nesting is allowed.
   * Note:	if nesting is not allowed, the IRQs are already disabled
   */
  EE_std_disableIRQ_nested();

  /* 
   * ISR2 instance clean-up as requested by AR, must be called before
   * EE_decrement_IRQ_nesting_level()
   */
  EE_std_end_IRQ_post_stub();

  /* Decrement nesting level here, with IRQ disabled */
  EE_decrement_IRQ_nesting_level();

#ifdef	__ATXMEGA__
#ifdef	__IRQ_STACK_NEEDED__
  if ( !EE_is_inside_ISR_call() ) {

    EE_std_IRQ_stack_back();

  }
#endif	/* __IRQ_STACK_NEEDED__ */

  /* Call the scheduler */
  EE_avr8_after_IRQ_schedule();
#endif	/* __ATXMEGA__ */

#ifdef	__ATMEGA__
  if ( !EE_is_inside_ISR_call() ) {

#ifdef	__IRQ_STACK_NEEDED__
    EE_std_IRQ_stack_back();
#endif	/* __IRQ_STACK_NEEDED__ */

    EE_std_after_IRQ_schedule();

  }

#endif	/* __ATMEGA__ */

  EE_AVR8_ISR_POSTINSTUB();

  /* Enable IRQ */
  EE_hal_enableIRQ();

  EE_return();

}	/* EE_avr8_isr2_stub() */


#ifdef	__ATXMEGA__
extern void EE_avr8_after_IRQ_schedule( void );

volatile void EE_avr8_std_after_IRQ_schedule( void ) {

  /* 
   * Disable IRQ: RETI ENABLE INTERRUPT!!!
   */
  EE_avr8_disableIRQ();

  /* Check for scheduling point */
  if ( !EE_is_inside_ISR_call() && !EE_is_inside_hw_ISR_call() ) {

    /* Call the scheduler */
    EE_std_after_IRQ_schedule();

  }

}
#endif	/* __ATXMEGA__ */