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

#include "mcu/atmel_atmega128/inc/ee_avr5def.S"


.global uart_Init
.global uart_transmit
.global __vector_31

.text
uart_Init: 
			
			ldi r21, 0x08
			sts UBRR1L, r21
			ret


uart_transmit:
			// direzione dati = in uscita
			cli
			lds r21, UCSR1B
			ori r21, 0x68
			sts UCSR1B, r21
			in r21, DDRD
			ori r21, 0x08
			out DDRD, r21 
			sei
			ret
			
			

// vector table
// 1 external interrupt request 0
// 2 external interrupt request 1
// 3 external interrupt request 2
// 4 external interrupt request 3
// 5 external interrupt request 4
// 6 external interrupt request 5
// 7 external interrupt request 6
// 8 external interrupt request 7
// 9  Timer/Counter 2 Compare Match
// 10 Timer/Counter 2 Overflow
// 11 Timer/Counter 1 Capture Event
// 12 Timer/Counter 1 Compare Match A
// 13 Timer/Counter 1 Compare Match B
// 14 Timer/Counter 1 Overflow
// 15 Timer/Counter 0 Compare Match
// 16 timer/Counter 0 Overflow
// 17 SPI Serial Transfer Complete
// 18 USART0 Rx complete
// 19 USART0 Data Register Empty
// 20 Usart0 Tx complete
// 21 ADC Conversion Complete
// 22 EEPROM Ready
// 23 Analog Comparator
// 24 Timer/Counter 1 Compare Match C
// 25 Timer/Counter 3 Capture Event
// 26 Timer/Counter 3 Compare Match A
// 27 Timer/Counter 3 Compare Match B
// 28 Timer/Counter 3 Compare Match C
// 29 Timer/Counter 3 Overflow
// 30 USART1 Rx Complete
// 31 USART1 Data Register Empty
// 32 USART1 Tx Complete
// 33 Two-wire serial Interface
// 34 Store program Memory Ready

.extern msdu_uart
.extern msdu_uartLength


.global next_byte_to_send

.section .bss

next_byte_to_send:
	.byte 0x00


.text
.global __vector_31
__vector_31 :
			push r31
			push r30
			push r29
			push r28
			push r27
			push r26
			push r21
			push r20
			push r22

			ldi r30, lo8(msdu_uart)
			ldi r31, hi8(msdu_uart)
			ldi r28, lo8(msdu_uartLength)
			ldi r29, hi8(msdu_uartLength) 
			ldi r26, lo8(next_byte_to_send)
			ldi r27, hi8(next_byte_to_send)
			clr r21
			ld r20, x
			//  r31:r30 ( base address ) + r27:r26 ( offset)
			add r30, r20
			adc r31, r21
			ld r22, z
			sts UDR1, r22
			inc r20
			st x, r20
					
			// check if must be send other byte
			ld r20, x
			ld r21, y
			cp r20, r21
			brne again	
			//disable output portD
			in r21, DDRD
			andi r21, 0xF7
			out DDRD, r21
			
			lds r21, UCSR1B
			andi r21, 0xDF
			sts UCSR1B, r21
			clr r22
			st x, r22	
	again:		
			
			pop r22
			pop r20
			pop r21
			pop r26
			pop r27
			pop r28
			pop r29
			pop r30
			pop r31
			reti




.global __vector_32
__vector_32 :
			//ldi r21, 0xff
			//out DDRE, r21
			//clr r21
			//out DDRE, r21
			reti








