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
 * Author: 2005 Antonio Romano
 * 	   2014 Giuseppe Serano
 */

#include "ee_internal.h"

#ifdef	EE_AVR8_USART1_RX_ISR
static void (*Rx1IsrFunction)(EE_UINT8 data) = NULL;
#endif

#if	( defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__) )
#ifndef	F_CPU
#define F_CPU 16000000
#endif
#endif	/* __AVR_ATmega328__ || __AVR_ATmega328P__ */

/*
 * TODO: Check if control flow is present in hw or remove mode
 */
EE_INT8
EE_uart_init(
  EE_UINT8	port,
  EE_UINT32	baud,
  EE_UINT16	byte_format,
  EE_UINT16	mode
) {
	EE_UINT16	baudregs;
	
	if (port != EE_UART_PORT_1) 
		return -EE_UART_ERR_BAD_PORT;
		
	/* Initialize UART Port */
#if	( defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__) )
	UCSR0A = (1<<U2X0); 			/* Double speed mode USART0 */
	UBRR0L = (uint8_t)(F_CPU/(baud*8L)-1);
	UBRR0H = (F_CPU/(baud*8L)-1) >> 8;

	UCSR0C = (1<<UCSZ00) | (1<<UCSZ01);	/* 1 stop bit, no parity */
	UCSR0B = (1<<RXEN0) | (1<<TXEN0);	/* enable tx and rx */
#else	/* __AVR_ATmega328__ || __AVR_ATmega328P__ */
	/* Enable double speed mode (Allow it to be selected?) */
	UCSR0A |= 0x02;
	
	/* Turn on the transmission and reception circuitry */
	UCSR0B |= EE_UART_TX_ON | EE_UART_RX_ON;
	
	/* Set this? Missing setting for 9bit mode!!! */
	UCSR0C |= (byte_format & 0x4E);
	
	baudregs = (((EE_UART_INSTRUCTION_CLOCK / (baud * 8UL))) - 1); 
	
	/*
	 * Load lower 8-bits of the baud rate value into the low byte of the
	 * UBRR register
	 */
	UBRR0L = (baudregs & 0x00FF);
	
	/*
	 * Load upper 8-bits of the baud rate value into the high byte of the
	 * UBRR register
	 */
	UBRR0H = (baudregs >> 8);
#endif	/* __AVR_ATmega328__ || __AVR_ATmega328P__ */
	return 1;
}

EE_INT8
EE_uart_close(
  EE_UINT8	port
) {
	if (port != EE_UART_PORT_1)
		return -EE_UART_ERR_BAD_PORT;
	
	/* chris: TODO: Release something */
#if	( defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__) )
	/* TODO: disable interrupts? */
	UCSR0B = ~((1<<RXEN0) | (1<<TXEN0));	/* disable tx and rx */
#endif	/* __AVR_ATmega328__ || __AVR_ATmega328P__ */
	return 1;
}

EE_INT8
EE_uart_set_rx_callback(
  EE_UINT8	port,
  void (*RxFunc)(EE_UINT8 data),
  EE_UINT8	rxmode
) {
	if (port != EE_UART_PORT_1)
		return -EE_UART_ERR_BAD_PORT;
	
#ifdef	EE_AVR8_USART1_RX_ISR
	Rx1IsrFunction = RxFunc;
	
	if (RxFunc) {
		/*
		 * TODO: (Nino) Initialize UART  ISR
		 */
		/*
		U1STA &= 0x5FFF;
		U1STA |= rxmode & 0xA000;
		IEC0bits.U1RXIE = 1;
		IFS0bits.U1RXIF = 0;
		*/
	}
	return 1;
#else
	return -EE_UART_ERR_INT_DISABLED;
#endif
}

EE_INT8
EE_uart_write_byte(
  EE_UINT8	port,
  EE_UINT8	data
) {
	if (port != EE_UART_PORT_1)
		return -EE_UART_ERR_BAD_PORT;
		
	/* Polling mode */
	/* TODO: (Nino) UART Send */
	/* Do nothing until UDR is ready for more data to be written to it */
	while ( !( UCSR0A & (1<<UDRE0)) );
	/* Echo back the received byte back to the computer */
	UDR0 = data;
	return 1;
}

EE_INT8
EE_uart_read_byte(
  EE_UINT8	port,
  EE_UINT8	*data
) {
	if (port != EE_UART_PORT_1)
		return -EE_UART_ERR_BAD_PORT;
		
#ifdef	EE_AVR8_USART1_RX_ISR
	if (Rx1IsrFunction == NULL) {
		/* Polling mode */
#endif
		/* TODO: (Nino) Check UART Receive */
		/*
		 * if data have been recieved and is ready to be read from UDR
		 */
		if(UCSR0A & (1 << RXC0)) {
			/*
			 * Fetch the recieved byte value into the variable
			 * "ByteReceived"
			 */
			*data = UDR0;
			return 1;
		}
		return -EE_UART_ERR_NO_DATA;
#ifdef	EE_AVR8_USART1_RX_ISR
	}
	return -EE_UART_ERR_INT_MODE;
#endif
}


#ifdef	EE_AVR8_USART1_RX_ISR
#if	(EE_AVR8_USART1_RX_ISR_CAT == 2)
ISR2(EE_AVR8_USART1_RX_ISR)
#else
ISR1(EE_AVR8_USART1_RX_ISR)
#endif
{
	if (Rx1IsrFunction != NULL) {
		/* Execute callback function */
		/* TODO: (Nino) Call function with correct data */
		/* Rx1IsrFunction(U1RXREG & 0x00FF); */
	}
	/* IFS0bits.U1RXIF = 0; */
}
#endif
