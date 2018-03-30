/* ###*B*###
 * ERIKA Enterprise - a tiny RTOS for small microcontrollers
 *
 * Copyright (C) 2002-2012  Evidence Srl
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
 * ISR2 Stub declaration
 *
 * Author: 2013,  Giuseppe Serano
 */

#ifndef INCLUDE_EE_AVR8_IRQSTUB_H__
#define INCLUDE_EE_AVR8_IRQSTUB_H__

/* I need Interals */
#include "ee_internal.h"

/* Plus I need IRQ handling defines */
#include "ee_irq.h"

/*
 * ISR Type 1 stub.
 */
void EE_COMPILER_KEEP EE_avr8_isr1_stub(EE_VOID_CALLBACK f);

/*
 * ISR Type 2 stub.
 */
void EE_COMPILER_KEEP EE_avr8_isr2_stub(EE_VOID_CALLBACK f);

/*
 * Provide IRQ Entry global symbol.
 */
#define	EE_AVR8_PROVIDE_IRQ_ENTRY(entry)	\
	EE_AVR8_PROVIDE_SYM(EE_PREPROC_JOIN(tag_,entry))

/*
 * Request IRQ Entry global symbol.
 */
#define	EE_AVR8_REQUEST_IRQ_ENTRY(entry)	\
	EE_AVR8_REQUEST_SYM(EE_PREPROC_JOIN(tag_,entry))

/*
 * ISR pre-stub.
 */
#define	EE_AVR8_ISR_PRESTUB()			\
do {						\
__asm__ __volatile__("push r24\n");		\
__asm__ __volatile__("push r25\n");		\
} while(0)

#define	EE_AVR8_ISR_PREINSTUB()			\
do {						\
__asm__ __volatile__("push r1\n");		\
__asm__ __volatile__("push r0\n");		\
__asm__ __volatile__("in   r0, 0x3f\n");	\
  /* cli(); */ 					\
__asm__ __volatile__("push r0\n");		\
__asm__ __volatile__("eor  r1, r1\n");		\
__asm__ __volatile__("push r18\n");		\
__asm__ __volatile__("push r19\n");		\
__asm__ __volatile__("push r20\n");		\
__asm__ __volatile__("push r21\n");		\
__asm__ __volatile__("push r22\n");		\
__asm__ __volatile__("push r23\n");		\
__asm__ __volatile__("push r26\n");		\
__asm__ __volatile__("push r27\n");		\
__asm__ __volatile__("push r30\n");		\
__asm__ __volatile__("push r31\n");		\
} while(0)

/*
 * ISR post-stub.
 */
#define	EE_AVR8_ISR_POSTSTUB()			\
do {						\
__asm__ __volatile__("pop r25\n");		\
__asm__ __volatile__("pop r24\n");		\
} while(0)

#define	EE_AVR8_ISR_POSTINSTUB()		\
do {						\
__asm__ __volatile__("pop r31\n");		\
__asm__ __volatile__("pop r30\n");		\
__asm__ __volatile__("pop r27\n");		\
__asm__ __volatile__("pop r26\n");		\
__asm__ __volatile__("pop r23\n");		\
__asm__ __volatile__("pop r22\n");		\
__asm__ __volatile__("pop r21\n");		\
__asm__ __volatile__("pop r20\n");		\
__asm__ __volatile__("pop r19\n");		\
__asm__ __volatile__("pop r18\n");		\
__asm__ __volatile__("pop r0\n");		\
__asm__ __volatile__("out 0x3f, r0\n");		\
__asm__ __volatile__("pop r0\n");		\
__asm__ __volatile__("pop r1\n");		\
} while(0)

/* Define an ISR (category 1) */
#define EE_AVR8_ISR1_DEFINITION(v,f)	\
/* Declare The ISR User handler */	\
extern void f ( void );			\
ISR(v, ISR_NAKED)			\
{					\
  EE_AVR8_PROVIDE_IRQ_ENTRY(v);		\
  EE_AVR8_ISR_PRESTUB();		\
  EE_avr8_isr1_stub(f);			\
  EE_AVR8_ISR_POSTSTUB();		\
  reti();				\
}

/* Define an ISR (category 2) */
#define EE_AVR8_ISR2_DEFINITION(v,f)	\
/* Declare The ISR User handler */	\
extern void f ( void );			\
ISR(v, ISR_NAKED)			\
{					\
  EE_AVR8_PROVIDE_IRQ_ENTRY(v);		\
  EE_AVR8_ISR_PRESTUB();		\
  EE_avr8_isr2_stub(f);			\
  EE_AVR8_ISR_POSTSTUB();		\
  __asm__ __volatile__("ret\n");	\
}




/*
ISR2(TIM0_OVF_vect)
{
  //isr code
}

EE_AVR8_ISR2_DEFINITION(TIM0_OVF_vect, real_isr);
*/

#define ISR2(intno)			\
void intno ## _f(void);		\
EE_AVR8_ISR2_DEFINITION(intno, intno ## _f); \
void intno ## _f(void)





/* Un-Defined ISR */
#define	EE_AVR8_ISR_NOT_DEFINED(v)	\
/* Define a NAKED ISR handler */	\
ISR(v, ISR_NAKED)			\
{					\
  EE_AVR8_PROVIDE_IRQ_ENTRY(v);		\
  /* Disables IRQ  */			\
  cli();				\
  /* Hangs in a infinite loop!!! */	\
  for(;;) {				\
    ;					\
  }					\
}

#endif /* INCLUDE_EE_AVR8_IRQSTUB_H__ */
