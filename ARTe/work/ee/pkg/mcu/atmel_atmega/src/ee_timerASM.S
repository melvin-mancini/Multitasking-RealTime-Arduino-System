/* ###*B*###
 * ERIKA Enterprise - a tiny RTOS for small microcontrollers
 *
 * Copyright (C) 2002-2014  Evidence Srl
 *
 * This file is part of ERIKA Enterprise.
 *
 * ERIKA Enterprise is free software; you can redistribute it
 * and/OR modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation,
 * (with a special exception described below).
 *
 * Linking this code statically OR dynamically with other modules is
 * making a combined work based on this code.  Thus, the terms and
 * conditions of the GNU General Public License cover the whole
 * combination.
 *
 * As a special exception, the copyright holders of this library give you
 * permission to link this code with independent modules to produce an
 * executable, regardless of the license terms of these independent
 * modules, and to copy and distribute theresulting executable under
 * terms of your choice, provided that you also meet, for each linked
 * independent module, the terms and conditions of the license of that
 * module.  An independent module is a module which is not derived from
 * OR based on this library.  If you modify this code, you may extend
 * this exception to your version of the code, but you are not
 * obligated to do so.  If you do not wish to do so, delete this
 * exception statement from your version.
 *
 * ERIKA Enterprise is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License version 2 for more details.
 *
 * You should have received a copy of the GNU General Public License
 * version 2 along with ERIKA Enterprise; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301 USA.
 * ###*E*### */

/*
 * Author: 2006 Antonio Romano
 * 	   2014 Giuseppe Serano
 */

#include <avr/io.h>

.global	EE_atmega_init_timer0
.global	EE_atmega_init_timer1
.global	EE_atmega_init_timer2
.global	EE_atmega_init_timer3
.global	EE_atmega_timer0_start
.global	EE_atmega_timer1_start
.global	EE_atmega_timer2_start
.global	EE_atmega_timer3_start
.global	EE_atmega_timer0_stop
.global	EE_atmega_timer1_stop
.global	EE_atmega_timer2_stop
.global	EE_atmega_timer3_stop
.global	EE_atmega_timer0_get
.global	EE_atmega_timer1_get
.global	EE_atmega_timer2_get
.global	EE_atmega_timer3_get



.text

/* ---------------- Timer 0 ----------------  */

#ifdef	__TIMER_0_USED__

EE_atmega_init_timer0:
	/* IN	R20,	SREG	; Save global interrupt flag */
	/* CLI			; Disable interrupts */
	CLR	R19
	CLR	R18

	; get the limit as parameter (to be tested!!!)
	OUT	_SFR_IO_ADDR(OCR0),	R24
	/* LDI	R18,	0xFF */
	/* OUT	_SFR_IO_ADDR(OCR0),	R18	; set the match	Register */
	
	LDI	R19,	0x08	; CTC mode, need to set OCR0
	IN	R18,	_SFR_IO_ADDR(TCCR0)
	ANDI	R18,	0xB7
	OR	R19,	R18
	OUT	_SFR_IO_ADDR(TCCR0),	R19
	
	IN	R19,	_SFR_IO_ADDR(TIFR)
	ORI	R19,	0x03
	OUT	_SFR_IO_ADDR(TIFR),	R19	; Clear pending interrupts
	
	IN	R18,	_SFR_IO_ADDR(TIMSK)
	ANDI	R18,	0xFC
	/* ORI	R18,	0x01	; Enable Timer/Counter0 Overflow Interrupt */
	ORI	R18,	0x02	; Enable Timer/Counter0 Match Interrupt
	OUT	_SFR_IO_ADDR(TIMSK),	R18
	
	/* SEI			; Enable global interrupt */
	/* OUT	SREG,	R20	; Restore global interrupt flag */
	
	RET

EE_atmega_timer0_start:
	/* IN	R20,	SREG	; Save global interrupt flag */
	/* CLI			; Disable interrupts */
	CLR	R18
	CLR	R19
	STS	_SFR_IO_ADDR(TCNT0),	R18
	
	IN	R18,	_SFR_IO_ADDR(TCCR0)
	ANDI	R18,	0xF8	; Mask clock source bits
	
#ifdef	__TIMER0_CLK_DIV1__
	ORI	R18,	0x01
#endif
#ifdef	__TIMER0_CLK_DIV8__
	ORI	R18,	0x02
#endif
#ifdef	__TIMER0_CLK_DIV32__
	ORI	R18,	0x03
#endif
#ifdef	__TIMER0_CLK_DIV64__
	ORI	R18,	0x04
#endif
#ifdef	__TIMER0_CLK_DIV128__
	ORI	R18,	0x05
#endif
#ifdef	__TIMER0_CLK_DIV256__
	ORI	R18,	0x06
#endif
#ifdef	__TIMER0_CLK_DIV1024__
	ORI	R18,	0x07
#endif
	OUT	_SFR_IO_ADDR(TCCR0),	R18	; Set timer clock prescaler
	
	/* OUT SREG,	R20	; Restore global interrupt flag */
	RET

EE_atmega_timer0_stop:
	/* IN	R20,	SREG	; Save global interrupt flag */
	/* CLI			; Disable interrupts */
	
	IN	R18,	_SFR_IO_ADDR(TCCR0)
	ANDI	R18,	0xF8
	; clear cso cs1 cs2 of TCCR0 Register. This stop the timer.
	OUT	_SFR_IO_ADDR(TCCR0),	R18
	
	/* OUT	SREG,	r20	; Restore global interrupt flag */
	RET

EE_atmega_timer0_get:
	/* IN	R20,	SREG	; Save global interrupt flag */
	/* CLI			; Disable interrupts */
	
	/*  NINO: Correct Register for Return? */
	IN	R24,	_SFR_IO_ADDR(TCNT0)
	
	/* OUT	SREG,	R20	; Restore global interrupt flag */
	RET

#endif	/* __TIMER_0_USED__ */

/*  ---------------- Timer 1 ----------------  */

#ifdef	__TIMER_1_USED__

EE_atmega_init_timer1:
	/* IN	R20,	SREG	; Save global interrupt flag */
	/* CLI			; Disable interrupts */
	
	CLR	R19
	CLR	R18
	
	; get the limit as parameter (to be tested!!!)
	OUT	_SFR_IO_ADDR(OCR1AH),	R25
	OUT	_SFR_IO_ADDR(OCR1AL),	R24
	; set the match Register to max (0xFF)
	/* LDI	R18,	0xFF */
	/* LDI	R19,	0xFF */
	/* OUT	_SFR_IO_ADDR(OCR1AL),	R18 */
	/* OUT	_SFR_IO_ADDR(OCR1AH),	R19 */
	
	CLR	R18
	OUT	_SFR_IO_ADDR(TCCR1A),	R18	; Disable all OC1 pins
	
	/* CLR	R18		; Enable Normal mode */
	LDI	R18,	0x08	; Enable CTC mode
	OUT	_SFR_IO_ADDR(TCCR1B),	R18
	
	IN	R19,	_SFR_IO_ADDR(TIFR)
	ORI	R19,	0x3C
	OUT	_SFR_IO_ADDR(TIFR),	R19	; Clear pending interrupts
	
	IN	R18,	_SFR_IO_ADDR(TIMSK)
	ANDI	R18,	0xC3
	/* ORI	R18,	0x04	; Enable Timer/Counter1 Overflow Interrupt */
	ORI	R18,	0x10	; Enable Timer/Counter1 OCA Match Interrupt
	OUT	_SFR_IO_ADDR(TIMSK),	r18
	
	/* SEI			; Enable global interrupt */
	/* OUT	SREG,	R20	; Restore global interrupt flag */
	
	RET

EE_atmega_timer1_start:
	/* IN	R20,	SREG	; Save global interrupt flag */
	/* CLI			; Disable interrupts */
	
	CLR	R18
	CLR	R19
	
	IN	R18,	_SFR_IO_ADDR(TCCR1B)
	ANDI	R18,	0xf8	; Mask clock source bits
	
#ifdef	__TIMER1_CLK_DIV1__
	ORI	R18,	0x01
#endif
#ifdef	__TIMER1_CLK_DIV8__
	ORI	R18,	0x02
#endif
#ifdef	__TIMER1_CLK_DIV64__
	ORI	R18,	0x03
#endif
#ifdef	__TIMER1_CLK_DIV256__
	ORI	R18,	0x04
#endif
#ifdef	__TIMER1_CLK_DIV1024__
	ORI	R18,	0x05
#endif
	OUT	_SFR_IO_ADDR(TCCR1B),	R18	; Set timer clock prescaler
	
	/* OUT	SREG,	R20	; Restore global interrupt flag */
	RET

EE_atmega_timer1_stop:
	/* IN	R20,	SREG	; Save global interrupt flag */
	/* CLI			; Disable interrupts */
	
	; clear cso cs1 cs2 of TCCR1 Register.
	IN	R18,	_SFR_IO_ADDR(TCCR1B)
	ANDI	R18,	0xF8
	OUT	_SFR_IO_ADDR(TCCR1B),	R18
				; This stop the timer.
	
	/* OUT	SREG,	R20	; Restore global interrupt flag */
	RET

EE_atmega_timer1_get:
	/* IN	R20,	SREG	; Save global interrupt flag */
	/* CLI			; Disable interrupts */
	
	IN	R24,	_SFR_IO_ADDR(TCNT1L)
	IN	R25,	_SFR_IO_ADDR(TCNT1H)
	
	/* OUT	SREG,	R20	; Restore global interrupt flag */
	RET

#endif	/* __TIMER_1_USED__ */

/*  ---------------- Timer 2 ----------------  */

#ifdef	__TIMER_2_USED__

EE_atmega_init_timer2:
	/* IN	R20,	SREG	; Save global interrupt flag */
	/* CLI			; Disable interrupts */
	CLR	R19
	CLR	R18
	
	; get the limit as parameter (to be tested!!!)
	OUT	_SFR_IO_ADDR(OCR2),	R24
	; set the match	Register to max (0xFF) */
	/* LDI	R18,	0xFF */
	/* OUT	_SFR_IO_ADDR(OCR2),	R18 */
	
	LDI	R19,	0x08	; CTC mode,	need to set OCR0
	IN	R18,	_SFR_IO_ADDR(TCCR2)
	ANDI	R18,	0xB7
	OR	R19,	R18
	OUT	_SFR_IO_ADDR(TCCR2),	R19
	
	IN	R19,	_SFR_IO_ADDR(TIFR)
	ORI	R19,	0xC0
	OUT	_SFR_IO_ADDR(TIFR),	r19	; Clear pending interrupts
	
	IN	R18,	_SFR_IO_ADDR(TIMSK)
	ANDI	R18,	0x3F
	/* ORI	R18,	0x40	; Enable Timer/Counter0 Overflow Interrupt */
	ORI	R18,	0x80	; Enable Timer/Counter0 Match Interrupt
	OUT	_SFR_IO_ADDR(TIMSK),	R18
	
	/* SEI			; Enable global interrupt */
	/* OUT	SREG,	r20	; Restore global interrupt flag */
	RET

EE_atmega_timer2_start:
	/* IN	R20,	SREG	; Save global interrupt flag */
	/* CLI			; Disable interrupts */
	
	CLR	R18
	CLR	R19
	
	IN	R18,	_SFR_IO_ADDR(TCCR2)
	ANDI	R18,	0xF8	; Mask clock source bits
	
#ifdef	__TIMER2_CLK_DIV1__
	ORI	R18,	0x01
#endif
#ifdef	__TIMER2_CLK_DIV8__
	ORI	R18,	0x02
#endif
#ifdef	__TIMER2_CLK_DIV64__
	ORI	R18,	0x03
#endif
#ifdef	__TIMER2_CLK_DIV256__
	ORI	R18,	0x04
#endif
#ifdef	__TIMER2_CLK_DIV1024__
	ORI	R18,	0x05
#endif
	OUT	_SFR_IO_ADDR(TCCR2),	R18	; Set timer clock prescaler
	
	/* OUT	SREG,	r20	; Restore global interrupt flag */
	RET

EE_atmega_timer2_stop:
	/* IN	R20,	SREG	; Save global interrupt flag */
	/* CLI			; Disable interrupts */
	
	; clear cso cs1 cs2 of TCCR2 Register. ; This stop the timer.
	IN	R18,	_SFR_IO_ADDR(TCCR2)
	ANDI	R18,	0xF8
	OUT	_SFR_IO_ADDR(TCCR2),	R18
	
	/* OUT	SREG,	R20	; Restore global interrupt flag */
	RET

EE_atmega_timer2_get:
	/* IN	R20,	SREG	; Save global interrupt flag */
	/* CLI			; Disable interrupts */
	
	/*  NINO: Correct Register for Return? */
	IN	R24,	_SFR_IO_ADDR(TCNT2)
	
	/* OUT	SREG,	r20	; Restore global interrupt flag */
	RET

#endif	/* __TIMER_2_USED__ */

/*  ---------------- Timer 3 ----------------  */

#ifdef	__TIMER_3_USED__

EE_atmega_init_timer3:
	/* IN	R20,	SREG	; Save global interrupt flag */
	/* CLI			; Disable interrupts */
	
	CLR	R19
	CLR	R18
	
	STS	OCR3AH,	R25	; get the limit as parameter (to be tested!!!)
	STS	OCR3AL,	R24
	/* LDI	R18,	0xFF */
	/* LDI	R19,	0xFF */
	/* STS	OCR3AL,	R18 */
	/* STS	OCR3AH,	R19	; set the match	Register to max (0xFF) */
	
	CLR	R18
	STS	TCCR3A,	R18	; Disable all OC3 pins
	
	/* CLR	R18		; Enable Normal mode */
	LDI	R18,	0x08	; Enable CTC mode
	STS	TCCR3B,	R18
	
	LDS	R19,	ETIFR
	ORI	R19,	0x3F
	STS	ETIFR,	R19	; Clear pending interrupts
	
	LDS	R18,	ETIMSK
	ANDI	R18,	0xC0
	/* ORI	R18,	0x04	; Enable Timer/Counter1 Overflow Interrupt */
	ORI	R18,	0x10	; Enable Timer/Counter1 OCA Match Interrupt
	STS	ETIMSK,	R18
	
	/* SEI			; Enable global interrupt */
	/* OUT	SREG,	R20	; Restore global interrupt flag */
	RET

EE_atmega_timer3_start:
	/* IN	R20,	SREG	; Save global interrupt flag */
	/* CLI			; Disable interrupts */
	
	CLR	R18
	CLR	R19
	
	LDS	R18,	TCCR3B
	ANDI	R18,	0xF8	; Mask clock source bits
	
#ifdef	__TIMER3_CLK_DIV1__
	ORI	R18,	0x01
#endif
#ifdef	__TIMER3_CLK_DIV8__
	ORI	R18,	0x02
#endif
#ifdef	__TIMER3_CLK_DIV64__
	ORI	R18,	0x03
#endif
#ifdef	__TIMER3_CLK_DIV256__
	ORI	R18,	0x04
#endif
#ifdef	__TIMER3_CLK_DIV1024__
	ORI	R18,	0x05
#endif
	STS	TCCR3B,	R18	; Set timer clock prescaler
	
	/* OUT	SREG,	r20	; Restore global interrupt flag */
	RET

EE_atmega_timer3_stop:
	/* IN	R20,	SREG	; Save global interrupt flag */
	/* CLI			; Disable interrupts */
	
	LDS	R18,	TCCR3B
	ANDI	R18,	0xF8
	STS	TCCR3B,	R18	; clear cso cs1 cs2 of TCCR1 Register.
				; This stop the timer.
	
	/* OUT	SREG,	r20	; Restore global interrupt flag */
	RET

EE_atmega_timer3_get:
	/* IN	R20,	SREG	; Save global interrupt flag */
	/* CLI			; Disable interrupts */
	
	LDS	R24,	TCNT3L
	LDS	R25,	TCNT3H
	
	/* OUT	SREG,	r20	; Restore global interrupt flag */
	RET

#endif	/* __TIMER_3_USED__ */
