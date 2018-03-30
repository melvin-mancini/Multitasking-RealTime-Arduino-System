/* ###*B*###
 * ERIKA Enterprise - a tiny RTOS for small microcontrollers
 *
 * Copyright (C) 2002-2008  Evidence Srl
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
 * Author:  Antonio Romano, 2005-2006 	
 * CVS: $Id: ee_irqstub.S,v 1.1 2007/10/04 14:39:08 romano Exp $
 */

#ifndef __INCLUDE_ATMEGA1281_IRQSTUB_S__
#define __INCLUDE_ATMEGA1281_IRQSTUB_S__
 

#include "cpu/avr5/inc/ee_compiler.h"
#include "cpu/avr5/src/stackManage.S"

/*
 * Imported symbols
 */ 
	; kernel scheduler addresses 
#ifdef __FP__
	.extern	EE_IRQ_end_instance
	#define EE_ENDINSTANCE EE_IRQ_end_instance
#endif

#ifdef __EDF__
	.extern	EE_edf_IRQ_end_instance
	#define EE_ENDINSTANCE EE_edf_IRQ_end_instance
#endif

#if defined(__OO_BCC1__) || defined(__OO_BCC2__) || defined(__OO_ECC1__) || defined(__OO_ECC2__)
	.extern	EE_oo_IRQ_end_instance
	#define EE_ENDINSTANCE EE_oo_IRQ_end_instance
#endif




	.extern EE_avr5_hal_ready2stacked
	
	; stack used by IRQ handlers
#ifdef __MULTI__
	.extern EE_avr5_hal_stkchange


#ifdef __IRQ_STACK_NEEDED__
	.extern EE_avr5_IRQ_tos
	
#endif
#endif

        ; interrupt handlers nesting
	.extern EE_IRQ_nesting_level
	.extern EE_avr5_temp_tos


/* ------------------------------------------------------------------------
 * ISR Level 1 Macro
 * ------------------------------------------------------------------------
 *
 * ISR Level 1 contains all the code for a typical ISR1 routine.
 * Features of ISR1 include:
 * - Support for Nested interrupt
 * - The ISR C routine runs in System mode
 * - In multi stack systems if needed the stack can be switched 
 *   to the separate Interrupt stack
 * - No EE Functions can be called from this handler
 * - It is supposed that ISR 1 have priority higher than ISR 2
 * - It is supposed that ISR 1 have priority lower than ISR 1 Minimal
 */


.macro EE_ISR1_HANDLER_STUB C_FUNCTION_NAME, STUB_FUNCTION_NAME

	.extern \C_FUNCTION_NAME
	.global \STUB_FUNCTION_NAME
	

\STUB_FUNCTION_NAME:

	; NOTE: interrupts are disabled

	; save LR_irq, PSR of the preempted thread and work registers
	; that are not preserved across function calls -- that can be
	; overwritten if another interrupt occurs while the current
	; one is being processed

	;salvare i registri  r18 - r27, r30 + r31, SREG


	PUSH R0
	PUSH R1
	PUSH R18
	PUSH R19
	PUSH R20
	PUSH R21
	PUSH R22
	PUSH R23
	PUSH R24
	PUSH R25
	PUSH R26
	PUSH R27
	PUSH R30
	PUSH R31
	
	;save SREG	

	IN R1, 0x3F  
	PUSH R1
	CLR R1

#ifdef __ALLOW_NESTED_IRQ__

	; increase the nesting level

	LDI R30, lo8(EE_IRQ_nesting_level)
	LDI R31, hi8(EE_IRQ_nesting_level)
	LD R24, Z+
	LD R25, Z
	ADIW R24,1
	ST Z,  R25
	ST -Z, R24
	
	
#ifdef __MULTI__
#ifdef __IRQ_STACK_NEEDED__
	; if this is the first handler instance then we have to
	; change stack
	
	
	LDI R22, 1
	LDI R21, 0
	CP  R24, R22
	CPC R25, R21
	BRNE 1f 
	EE_avr5_change_to_irq_tos
1:
	CLR R0
	
	
#endif
#endif
	
	; now we can re-enable interrupts and switch to SYS mode

	SEI

#else 	// not __ALLOW_NESTED_IRQ__
	
	// nesting level = 1

	LDI R30, lo8(EE_IRQ_nesting_level) 
	LDI R31, hi8(EE_IRQ_nesting_level)
	LDI R20, 1
	CLR R1
	ST Z+, R20 //parte meno  significativa del dato (little endian)
	ST Z, R1  //parte piu' significativa
	CLR R0

	
#endif  // __ALLOW_NESTED_IRQ__


	// 
	// jump to the correct handler
	// 
	

	CALL \C_FUNCTION_NAME

		
#ifdef __ALLOW_NESTED_IRQ__
#ifdef __MULTI__
	// we can disable interrupts 
		
 	
	CLI

 	
#endif     // __MULTI__ 
#endif     //__ALLOW_NESTED_IRQ__


#ifdef __ALLOW_NESTED_IRQ__

	;decrease the nesting level

	LDI R30, lo8(EE_IRQ_nesting_level) 
	LDI R31, hi8(EE_IRQ_nesting_level)
	LD r24, Z+
	LD r25, Z
	sbiw r24,1
	ST Z,R25
	ST -Z,R24	


	
#ifdef __MULTI__
#ifdef __IRQ_STACK_NEEDED__
	// is this the first handler instance?
	
	CLR R0
	CLR R1	
	CP  R0, R24
	CPC R1, R25
	BRNE 2f 
	EE_avr5_change_from_irq_tos
2:
	// if this is the first handler instance then we have to
	// change stack

#endif //__IRQ_STACK_NEEDED__
	   // (with interrupts disabled)
	
	
#endif //__MULTI__

#else // not __ALLOW_NESTED_IRQ__
      // nesting level = 0
	
	
	LDI R30, lo8(EE_IRQ_nesting_level) 
	LDI R31, hi8(EE_IRQ_nesting_level)
	CLR R0
	CLR R1
	ST Z+,R1
	ST Z,R0	

#endif //__ALLOW_NESTED_IRQ__

	// restore IRQ-handler working registers and return from IRQ

	POP r1
	OUT 0x3F, r1  
	
	;//restore SREG
	
	//restore working registers
	
	POP r31
	POP r30
	POP r27
	POP r26
	POP r25
	POP r24
	POP r23
	POP r22
	POP r21
	POP r20
	POP r19
	POP r18
	POP r1
	POP r0
	.endm
	
	
	       
/* ------------------------------------------------------------------------
 * ISR Level 2 Macro
 * ------------------------------------------------------------------------
	
 * ISR Level 1 contains all the code for a typical ISR2 routine.
 * Features of ISR2 include:
 * - Support for Nested interrupt
 * - The ISR C routine runs in System mode
 * - In multi stack systems if needed the stack can be switched 
 *   to the separate Interrupt stack
 * - EE Functions can be called from this handler
 * - It is supposed that ISR 2 have priority lower than ISR 1
 * - It is supposed that ISR 1 have priority lower than ISR 1 Minimal
 */



.macro EE_ISR2_HANDLER_STUB C_FUNCTION_NAME, STUB_FUNCTION_NAME

	.extern \C_FUNCTION_NAME
	


.global \STUB_FUNCTION_NAME

\STUB_FUNCTION_NAME:
	
	// NOTE: interrupts are disabled

	// save LR_irq, PSR of the preempted thread and work registers
	// that are not preserved across function calls -- that can be
	// overwritten if another interrupt occurs while the current
	// one is being processed
	
	PUSH R0
	PUSH R1
	PUSH R18
	PUSH R19
	PUSH R20
	PUSH R21
	PUSH R22
	PUSH R23
	PUSH R24
	PUSH R25
	PUSH R26
	PUSH R27
	PUSH R30
	PUSH R31
	
	//save SREG	
	
	IN R1, 0x3F  
	PUSH R1
	CLR R1
		
#ifdef __ALLOW_NESTED_IRQ__

	// increase the nesting level
	
	LDI R26, lo8(EE_IRQ_nesting_level)
	LDI R27, hi8(EE_IRQ_nesting_level)
	LD R24, X+
	LD R25, X
	ADIW R24,1
	ST  X, R25
	ST -X, R24
	
	
#ifdef __MULTI__
#ifdef __IRQ_STACK_NEEDED__
	// if this is the first handler instance then we have to
	// change stack
	
	LDI R22, 1
	LDI R21, 0
	CP  R24, R22
	CPC R25, R21
	BRNE 1f
	EE_avr5_change_to_irq_tos
1:
	CLR R0


	
#endif
#endif
	
	// now we can re-enable interrupts
 	
	SEI
	
#else // __ALLOW_NESTED_IRQ__
	
	// nesting level = 1
	
	LDI R26, lo8(EE_IRQ_nesting_level) 
	LDI R27, hi8(EE_IRQ_nesting_level)
	LDI R18, 1
	CLR R1
	ST X+, R18 //parte meno  significativa del dato (little endian)
	ST X, R1  //parte piu' significativa
	

		
#endif // __ALLOW_NESTED_IRQ__


	// 
	// jump to the correct handler
	// 


	CALL \C_FUNCTION_NAME

		
#ifdef __ALLOW_NESTED_IRQ__
	// we can disable interrupts (_only_ IRQ, _not_ FIQ)
 
 	CLI 

#endif

	


#ifdef __ALLOW_NESTED_IRQ__
// decrease the nesting level

	LDI R26, lo8(EE_IRQ_nesting_level) 
	LDI R27, hi8(EE_IRQ_nesting_level)
	LD R24, X+
	LD R25, X
	SBIW R24,1
	ST  X, R25
	ST -X, R24	

	// is this the first handler instance?
	CLR R0
	CLR R1	
	CP  R0, R24
	CPC R1, R25

	
#ifdef __MULTI__
#ifdef __IRQ_STACK_NEEDED__
	// if this is the first handler instance then we have to
	// change stack
	
	BRNE	1f	
	EE_avr5_change_from_irq_tos
	jmp 3f
1:

#endif
#endif

	// call the scheduler only if this is the first instance
	BREQ	1f

#else // not __ALLOW_NESTED_IRQ__
	// nesting level = 0
	
	LDI R26, lo8(EE_IRQ_nesting_level) 
	LDI R27, hi8(EE_IRQ_nesting_level)
	CLR R0
	CLR R1
	ST X+, R1
	ST X, R0	

#endif // __ALLOW_NESTED_IRQ__


3:
	// call the scheduler only if this is the first instance
	
	CALL EE_ENDINSTANCE


	// check if the scheduler has called IRQ_ready or IRQ_stacked
#ifdef __MULTI__

	LDI r30, lo8(EE_hal_endcycle_next_tos)
	LDI r31, hi8(EE_hal_endcycle_next_tos) 
	LD r22, Z		

	

#endif

	LDI r30, lo8(EE_hal_endcycle_next_thread)
	LDI r31, hi8(EE_hal_endcycle_next_thread) 
	LD r24, Z+
	LD r25, Z
	CLR r0
	CLR r1
	CP  r24, r0
	CPC r25, r1
	
	// now we have to call two different functions

#ifdef __MULTI__


	//that is, theend
	
	BREQ 2f 
	call EE_avr5_hal_ready2stacked
	jmp 1f
2:
	call EE_avr5_hal_stkchange	

#else

	BRNE 2f 
2:
	call EE_avr5_hal_ready2stacked 
	
	
#endif
	
1:	
	
	// restore IRQ-handler working registers and return from IRQ

	POP r1
	OUT 0x3F, r1  //restore SREG
	
	//restore working registers
	POP r31
	POP r30
	POP r27
	POP r26
	POP r25
	POP r24
	POP r23
	POP r22
	POP r21
	POP r20
	POP r19
	POP r18
	POP r1
	POP r0
	.endm

#endif

