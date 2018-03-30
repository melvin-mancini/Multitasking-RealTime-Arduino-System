/* ###*B*###
 * ERIKA Enterprise - a tiny RTOS for small microcontrollers
 *
 * Copyright (C) 2002-2014  Evidence Srl
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
 * Author: 2014  Giuseppe serano
 */


/* the define is needed for binary distributions */

#define __EECFG_THIS_IS_ASSEMBLER__

#ifdef	__MULTI__

	.text

/*
 * extern const EE_UREG EE_avr8_thread_tos[];
 */
	.extern	EE_std_thread_tos

/*
 * extern EE_UREG EE_avr8_active_tos;
 */
	.extern	EE_avr8_active_tos

/*
 * extern struct EE_TOS EE_avr8_system_tos;
 */
	.extern	EE_avr8_system_tos

/*
 * extern EE_UREG EE_std_run_task_code( EE_TID tid );
 */
	.extern	EE_std_run_task_code
/*
 * void EE_std_change_context( EE_TID tid )
 *
 * r24 = tid
 *
 *  Pseudo code for EE_std_change_context in multistack environment:
 *
 *  begin:
 *    tos_index = EE_std_thread_tos[tid+1];
 *    if is_not_the_current_stack(tos_index) {
 *       save_caller_saved_registers();
 *       switch_stacks(tos_index);
 *       restore_caller_saved_registers();
 *    }
 *    if (is_not_marked_stacked(tid)) {
 *       tid = EE_std_run_task_code(tid);
 *        goto begin;
 *    }
 *
 *  Please notice that the "goto begin" is actually a recursive call to
 *  EE_std_change_context_multi(), but in this way there is no stack growing.
 *
 *  For processors where the return address is saved in a register, that
 *  register must be saved in the stack too.
 *
 *  switch_stacks() should also update EE_hal_active_tos.
 */
	.global	EE_std_change_context
EE_std_change_context:

/* r25 = utid = (tid + 1) & ~TID_IS_STACKED_MARK */
	mov	r25,	r24
	inc	r25
	andi	r25,	0x7F

/* z = r31:r30 = &EE_avr8_thread_tos */
	ldi	r30,	lo8(EE_avr8_thread_tos)
	ldi	r31,	hi8(EE_avr8_thread_tos)

/* z = r31:r30 = &EE_avr8_thread_tos[utid] */
	add	r30,	r25
	adc	r31,	r1

/* r22 = tos_new = EE_avr8_thread_tos[utid] */
	ld	r22,	z

/* z = r31:r30 = &EE_avr8_active_tos */
	ldi	r30,	lo8(EE_avr8_active_tos)
	ldi	r31,	hi8(EE_avr8_active_tos)

/* r23 = tos_from = EE_avr8_active_tos */
	ld	r23,	z

/*	if ( tos_from != tos_new ) */
	cp	r23,	r22
	breq	EE_std_change_context_check_stacked_mark

/* Save working registers of preempted thread */
	push	r2
	push	r3
	push	r4
	push	r5
	push	r6
	push	r7
	push	r8
	push	r9
	push	r10
	push	r11
	push	r12
	push	r13
	push	r14
	push	r15
	push	r16
	push	r17
	push	r28
	push	r29

/* Now r2-r17 are scratch!!! */

/* r21:r20 = sp */
	in	r20, 0x3D	/* spl -> r20 */
	in	r21, 0x3E	/* sph -> r21 */

/* z = r31:r30 = &EE_avr8_system_tos */
	ldi	r30, lo8(EE_avr8_system_tos)
	ldi	r31, hi8(EE_avr8_system_tos)

/* r17:r16 = tos_from */
	mov	r16, r23
	clr	r17

/* Multiply r17:r16 by 2 */
	lsl	r16
	rol	r17

/* z = r31:r30 = &EE_avr8_system_tos[tos_from] */
	add	r30, r16
	adc	r31, r17

/* EE_avr8_system_tos[tos_from] = sp */
	st	z+, r20
	st	z,  r21

/*
 * New current TOS
 */

/* z = r31:r30 = &EE_avr8_system_tos */
	ldi	r30, lo8(EE_avr8_system_tos)
	ldi	r31, hi8(EE_avr8_system_tos)

/* r17:r16 = tos_new */
	mov	r16, r22
	clr	r17

/* Multiply r17:r16 by 2 */
	lsl	r16
	rol	r17

/* z = r31:r30 = &EE_avr8_system_tos[tos_new] */
	add	r30, r16
	adc	r31, r17

/* r17:r16 = EE_avr8_system_tos[tos_new] */
	ld	r16, z+
	ld	r17, z

/* sp = EE_avr8_system_tos[tos_index] */
	out	0x3D, r16
	out	0x3E, r17

/* Now SP point to the stack of preempter thread */

/* z = r31:r30 = &EE_avr8_active_tos */
	ldi	r30, lo8(EE_avr8_active_tos)
	ldi	r31, hi8(EE_avr8_active_tos)

/* EE_avr8_active_tos = tos_index */
	st	z, r22

/*
 * Here we have to restore the registers saved before the stack previous change.
 * If this is the first time we use the current stack, restoring is useless...
 */

/* restore working registers */
	pop	r29
	pop	r28
	pop	r17
	pop	r16
	pop	r15
	pop	r14
	pop	r13
	pop	r12
	pop	r11
	pop	r10
	pop	r9
	pop	r8
	pop	r7
	pop	r6
	pop	r5
	pop	r4
	pop	r3
	pop	r2

EE_std_change_context_check_stacked_mark:
/* r25 = tid & TID_IS_STACKED_MARK */
	mov	r25,	r24
	andi	r25,	0x80
	brne	EE_std_change_context_end	/* Z=0 */

/* z = r31:r30 = &EE_std_run_task_code */
	ldi	r30,	lo8(EE_std_run_task_code)
	ldi	r31,	hi8(EE_std_run_task_code)

/* r24 = tid = EE_std_run_task_code(tid) */
	clr	r25
	lsr	r31
	ror	r30
	icall

/* goto begin; */
	rjmp	EE_std_change_context

EE_std_change_context_end:
	ret

#ifdef	__IRQ_STACK_NEEDED__

/*
 * extern struct EE_TOS EE_avr8_IRQ_tos;
 */
	.extern	EE_avr8_IRQ_tos


/*
 * extern struct EE_TOS EE_avr8_old_tos;
 */
	.extern	EE_avr8_old_tos

/*
 * void EE_avr8_IRQ_change_stack( void );
 */
	.global	EE_avr8_IRQ_change_stack
EE_avr8_IRQ_change_stack:

/* r23:r22 = Return Address */
	pop	r23
	pop	r22

/* z = r31:r30 = &EE_avr8_old_tos */
	ldi r30, lo8(EE_avr8_old_tos)
	ldi r31, hi8(EE_avr8_old_tos) 

/* r25:r24 = sp */
	in	r24, 0x3D	/* spl -> r24 */
	in	r25, 0x3E	/* sph -> r25 */

/* store within data memory the tos of interrupted thread */
	st	z+, r24
	st	z, r25

/* z = r31:r30 = &EE_avr8_IRQ_tos */
	ldi	r30, lo8(EE_avr8_IRQ_tos)
	ldi	r31, hi8(EE_avr8_IRQ_tos) 

/* r25:r24 = EE_avr8_IRQ_tos */
	ld	r24, z+
	ld	r25, z

/* sp = EE_avr8_IRQ_tos */
	out	0x3D, r24	/* r24 -> spl */
	out	0x3E, r25	/* r25 -> sph */

/* now SP point to the stack of IRQ */

/* Push Return Address in the stack */
	push	r22
	push	r23

/* end of EE_avr8_IRQ_change_stack */
	ret

/*
 * void EE_avr8_IRQ_stack_back( void );
 */
	.global	EE_avr8_IRQ_stack_back
EE_avr8_IRQ_stack_back:

/* r23:r22 = Return Address */
	pop	r23
	pop	r22

/* z = r31:r30 = &EE_avr8_old_tos */
	ldi r30, lo8(EE_avr8_old_tos)
	ldi r31, hi8(EE_avr8_old_tos) 

/* r25:r24 = EE_avr8_old_tos */
	ld	r24, z+
	ld	r25, z

/* sp = EE_avr8_old_tos */
	out	0x3D, r24	/* r24 -> spl */
	out	0x3E, r25	/* r25 -> sph */

/* now SP point to the stack of interrupted thread */

/* Push Return Address in the stack */
	push	r22
	push	r23

/* end of EE_avr8_IRQ_stack_back */
	ret

#endif	/* __IRQ_STACK_NEEDED__ */

#endif	/* __MULTI__ */
