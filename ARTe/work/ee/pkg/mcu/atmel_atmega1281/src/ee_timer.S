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

#include "mcu/atmel_atmega1281/inc/ee_avr5def.S"

.global init_timer0
.global init_timer1
.global init_timer2
.global init_timer3
.global timer0_start
.global timer1_start
.global timer2_start
.global timer3_start
.global timer0_stop
.global timer1_stop
.global timer2_stop
.global timer3_stop
.global timer0_get
.global timer1_get
.global timer2_get
.global timer3_get



.text

// ---------------- Timer 0 ----------------

init_timer0:
	//in  r20, SREG	; Save global interrupt flag
	//cli				; Disable interrupts
	clr r19
	clr r18
	
	out OCR0, r24	; get the limit as parameter (to be tested!!!)
	//ldi r18, 0xff
	//out	OCR0, r18	; set the match register
	
	ldi r19, 0x08	; CTC mode, need to set OCR0
	in  r18, TCCR0
	andi r18, 0xb7
	or  r19, r18
	out TCCR0, r19
	
	in  r19, TIFR
	ori r19, 0x03
	out TIFR, r19	; Clear pending interrupts
	
	in  r18, TIMSK
	andi r18, 0xfc
	//ori  r18, 0x01	; Enable Timer/Counter0 Overflow Interrupt	
	ori  r18, 0x02	; Enable Timer/Counter0 Match Interrupt	
	out TIMSK, r18
	
	sei				; Enable global interrupt
	//out SREG, r20	; Restore global interrupt flag 
	
	ret

timer0_start:
	//in  r20, SREG	; Save global interrupt flag
	//cli				; Disable interrupts
	clr r18
	clr r19
	sts TCNT0, r18

	in r18, TCCR0
	andi r18, 0xf8	; Mask clock source bits
	
	#ifdef __TIMER0_CLK_DIV1__	
 	ori r18, 0x01
	#endif
	#ifdef __TIMER0_CLK_DIV8__	
 	ori r18, 0x02
	#endif
	#ifdef __TIMER0_CLK_DIV32__	
 	ori r18, 0x03
	#endif
	#ifdef __TIMER0_CLK_DIV64__	
 	ori r18, 0x04
	#endif
	#ifdef __TIMER0_CLK_DIV128__	
 	ori r18, 0x05
	#endif
	#ifdef __TIMER0_CLK_DIV256__	
 	ori r18, 0x06
	#endif
	#ifdef __TIMER0_CLK_DIV1024__	
 	ori r18, 0x07
	#endif
	out TCCR0,r18 ; Set timer clock prescaler
	
	//out SREG, r20	; Restore global interrupt flag
	ret

timer0_stop:
	//in  r20, SREG	; Save global interrupt flag
	//cli			; Disable interrupts
	
	in r18, TCCR0
	andi r18, 0xf8
	out TCCR0, r18	; clear cso cs1 cs2 of TCCR0 register. This stop the timer.
	
	//out SREG, r20	; Restore global interrupt flag 
	ret

timer0_get:
	//in  r20, SREG	; Save global interrupt flag
	//cli			; Disable interrupts
	
	in r24, TCNT0	// NINO: Correct register for return?

	//out SREG, r20	; Restore global interrupt flag 
	ret

// ---------------- Timer 1 ----------------

init_timer1:
	//in  r20, SREG	; Save global interrupt flag
	//cli			; Disable interrupts

	clr r19
	clr r18

	out OCR1AH, r25	; get the limit as parameter (to be tested!!!)
	out OCR1AL, r24
	//ldi r18, 0xff
	//ldi r19, 0xff
	//out OCR1AL, r18
	//out OCR1AH, r19	; set the match register to max (0xff)

	clr r18
	out TCCR1A, r18	; Disable all OC1 pins
		
	//clr  r18		; Enable Normal mode
	ldi  r18, 0x08	; Enable CTC mode
	out  TCCR1B, r18

	in  r19, TIFR
	ori r19, 0x3c
	out TIFR, r19	; Clear pending interrupts
	
	in  r18, TIMSK
	andi r18, 0xc3
	//ori  r18, 0x04	; Enable Timer/Counter1 Overflow Interrupt	
	ori  r18, 0x10	; Enable Timer/Counter1 OCA Match Interrupt	
	out TIMSK, r18

	sei				; Enable global interrupt
	//out SREG, r20	; Restore global interrupt flag 

	ret

timer1_start:
	//in  r20, SREG	; Save global interrupt flag
	//cli			; Disable interrupts

	clr r18
	clr r19
	
	in r18, TCCR1B
	andi r18, 0xf8	; Mask clock source bits
	
	#ifdef __TIMER1_CLK_DIV1__	
 	ori  r18, 0x01
	#endif
	#ifdef __TIMER1_CLK_DIV8__	
 	ori  r18, 0x02
	#endif
	#ifdef __TIMER1_CLK_DIV64__	
 	ori  r18, 0x03
	#endif
	#ifdef __TIMER1_CLK_DIV256__	
 	ori  r18, 0x04
	#endif
	#ifdef __TIMER1_CLK_DIV1024__	
 	ori  r18, 0x05
	#endif
	out TCCR1B,r18	; Set timer clock prescaler

	//out SREG, r20	; Restore global interrupt flag 
	ret

timer1_stop:
	//in  r20, SREG	; Save global interrupt flag
	//cli			; Disable interrupts
	
	in r18, TCCR1B
	andi r18, 0xf8
	out TCCR1B, r18	; clear cso cs1 cs2 of TCCR1 register. This stop the timer.
	
	//out SREG, r20	; Restore global interrupt flag 
	ret

timer1_get:
	//in  r20, SREG	; Save global interrupt flag
	//cli			; Disable interrupts

	in r24, TCNT1L
	in r25, TCNT1H

	//out SREG, r20	; Restore global interrupt flag 
	ret

// ---------------- Timer 2 ----------------

init_timer2:
	//in  r20, SREG	; Save global interrupt flag
	//cli			; Disable interrupts
	clr r19
	clr r18
	
	out OCR2, r24	; get the limit as parameter (to be tested!!!)
	//ldi r18, 0xff
	//out	OCR2, r18	; set the match register to max (0xff)
	
	ldi r19, 0x08	; CTC mode, need to set OCR0
	in  r18, TCCR2
	andi r18, 0xb7
	or  r19, r18
	out TCCR2, r19
	
	in  r19, TIFR
	ori r19, 0xC0
	out TIFR, r19	; Clear pending interrupts
	
	in  r18, TIMSK
	andi r18, 0x3f
	//ori  r18, 0x40	; Enable Timer/Counter0 Overflow Interrupt	
	ori  r18, 0x80	; Enable Timer/Counter0 Match Interrupt	
	out TIMSK, r18
	
	sei				; Enable global interrupt
	//out SREG, r20	; Restore global interrupt flag 
	ret

timer2_start:
	//in  r20, SREG	; Save global interrupt flag
	//cli			; Disable interrupts

	clr r18
	clr r19
	
	in r18, TCCR2
	andi r18, 0xf8	; Mask clock source bits
	
	#ifdef __TIMER2_CLK_DIV1__	
 	ori  r18, 0x01
	#endif
	#ifdef __TIMER2_CLK_DIV8__	
 	ori  r18, 0x02
	#endif
	#ifdef __TIMER2_CLK_DIV64__	
 	ori  r18, 0x03
	#endif
	#ifdef __TIMER2_CLK_DIV256__	
 	ori  r18, 0x04
	#endif
	#ifdef __TIMER2_CLK_DIV1024__	
 	ori  r18, 0x05
	#endif
	out TCCR2,r18	; Set timer clock prescaler

	//out SREG, r20	; Restore global interrupt flag 
	ret

timer2_stop:
	//in  r20, SREG	; Save global interrupt flag
	//cli			; Disable interrupts
	
	in r18, TCCR2
	andi r18, 0xf8
	out TCCR2, r18	; clear cso cs1 cs2 of TCCR2 register. This stop the timer.
	
	//out SREG, r20	; Restore global interrupt flag 
	ret

timer2_get:
	//in  r20, SREG	; Save global interrupt flag
	//cli			; Disable interrupts

	in r24, TCNT2	// NINO: Correct register for return?

	//out SREG, r20	; Restore global interrupt flag 
	ret

// ---------------- Timer 3 ----------------
	
init_timer3:
	//in  r20, SREG	; Save global interrupt flag
	//cli			; Disable interrupts

	clr r19
	clr r18

	sts OCR3AH, r25	; get the limit as parameter (to be tested!!!)
	sts OCR3AL, r24
	//ldi r18, 0xff
	//ldi r19, 0xff
	//sts OCR3AL, r18
	//sts OCR3AH, r19	; set the match register to max (0xff)

	clr r18
	sts TCCR3A, r18	; Disable all OC3 pins
		
	//clr  r18		; Enable Normal mode
	ldi  r18, 0x08	; Enable CTC mode
	sts  TCCR3B, r18

	lds  r19, ETIFR
	ori r19, 0x3f
	sts ETIFR, r19	; Clear pending interrupts
	
	lds  r18, ETIMSK
	andi r18, 0xc0
	//ori  r18, 0x04	; Enable Timer/Counter1 Overflow Interrupt	
	ori  r18, 0x10	; Enable Timer/Counter1 OCA Match Interrupt	
	sts ETIMSK, r18

	sei				; Enable global interrupt
	//out SREG, r20	; Restore global interrupt flag 
	ret

timer3_start:
	//in  r20, SREG	; Save global interrupt flag
	//cli			; Disable interrupts

	clr r18
	clr r19
	
	lds r18, TCCR3B
	andi r18, 0xf8	; Mask clock source bits
	
	#ifdef __TIMER3_CLK_DIV1__	
 	ori  r18, 0x01
	#endif
	#ifdef __TIMER3_CLK_DIV8__	
 	ori  r18, 0x02
	#endif
	#ifdef __TIMER3_CLK_DIV64__	
 	ori  r18, 0x03
	#endif
	#ifdef __TIMER3_CLK_DIV256__	
 	ori  r18, 0x04
	#endif
	#ifdef __TIMER3_CLK_DIV1024__	
 	ori  r18, 0x05
	#endif
	sts TCCR3B,r18	; Set timer clock prescaler

	//out SREG, r20	; Restore global interrupt flag 
	ret

timer3_stop:
	//in  r20, SREG	; Save global interrupt flag
	//cli			; Disable interrupts
	
	lds r18, TCCR3B
	andi r18, 0xf8
	sts TCCR3B, r18	; clear cso cs1 cs2 of TCCR1 register. This stop the timer.
	
	//out SREG, r20	; Restore global interrupt flag 
	ret

timer3_get:
	in  r20, SREG	; Save global interrupt flag
	cli			; Disable interrupts

	lds r24, TCNT3L
	lds r25, TCNT3H

	out SREG, r20	; Restore global interrupt flag 
	ret
