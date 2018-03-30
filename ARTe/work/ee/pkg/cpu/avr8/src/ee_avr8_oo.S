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
 * Author: 2013  Giuseppe Serano
 */

	.text

/*
 * exterb void EE_thread_not_terminated( void );
 */
	.extern	EE_thread_not_terminated

/*
 * void EE_avr8_terminate_savestk(EE_ADDR sp, EE_ADDR realbody);
 *
 * r25:r24 = &terminate_data[t]
 * r23:r22 = terminate_real_th_body[t]()
 */
	.global	EE_avr8_terminate_savestk
EE_avr8_terminate_savestk:
	/* Please note that the interrupt are ENABLED */

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

	/* save sp in terminate_data[t] */
	movw	r30,	r24	/* r25:r24 -> r31:r30 */
	in	r24,	0x3D	/* spl -> r24 */
	in	r25,	0x3E	/* sph -> r25 */
	st	z+,	r24
	st	z,	r25

	/* terminate_real_th_body[t]() */
	movw	r30,	r22
	icall

	/* EE_thread_not_terminated() */
	call	EE_thread_not_terminated

/*
 * void EE_avr8_terminate_task(EE_ADDR sp;
 *
 * r25:r24 = &terminate_data[t]
 */
	.global	EE_avr8_terminate_task
EE_avr8_terminate_task:
	/* reload sp from terminate_data[t] */
	movw	r30,	r24	/* r25:r24 -> r31:r30 */
	ld	r24,	z+
	ld	r25,	z
	out	0x3D,	r24	/* r24 -> spl */
	out	0x3E,	r25	/* r25 -> sph */

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

	/* return back to the caller */
	ret
