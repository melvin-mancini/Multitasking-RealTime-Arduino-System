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
 * Author: 2001-2002 Alessandro Colantonio
 *         2005 Antonio Romano
 * CVS: $Id: ee_mcuregs.h,v 1.2 2006/08/02 12:07:32 romano Exp $
 */

#include "cpu/avr5/inc/ee_cpu.h"

#ifndef __INCLUDE_MCU_ATMEGA128_MCUREGS__
#define __INCLUDE_MCU_ATMEGA128_MCUREGS__

/*****************************************************************************
 * I/O Register Definitions
 *****************************************************************************/

/**** Memory Mapped I/O Register Definitions (0xFF-0x60) ****/
#define 	UCSR1C	 0x9D
#define 	UDR1	 0x9C
#define 	UCSR1A	 0x9B
#define 	UCSR1B	 0x9A
#define 	UBRR1L	 0x99
#define 	UBRR1H	 0x98
#define 	UCSR0C	 0x95
#define 	UBRR0H	 0x90
#define 	TCCR3C	 0x8C
#define 	TCCR3A	 0x8B
#define 	TCCR3B	 0x8A
#define 	TCNT3H	 0x89
#define 	TCNT3L	 0x88
#define 	OCR3AH	 0x87
#define 	OCR3AL	 0x86
#define 	OCR3BH	 0x85
#define 	OCR3BL	 0x84
#define 	OCR3CH	 0x83
#define 	OCR3CL	 0x82
#define 	ICR3H	 0x81
#define 	ICR3L	 0x80
#define 	ETIMSK	 0x7D
#define 	ETIFR	 0x7C
#define 	TCCR1C	 0x7A
#define 	OCR1CH	 0x79//
#define 	OCR1CL	 0x78
#define 	TWCR	 0x74
#define 	TWDR	 0x73
#define 	TWAR	 0x72
#define 	TWSR	 0x71
#define 	TWBR	 0x70
#define 	OSCCAL	 0x6F
#define 	XMCRA	 0x6D
#define 	XMCRB	 0x6C
#define 	EICRA	 0x6A
#define 	SPMCSR	 0x68
#define 	SPMCR	 0x68		// old name for SPMCSR
#define 	PORTG	 0x65
#define 	DDRG	 0x64
#define 	PING	 0x63
#define 	PORTF	 0x62
#define 	DDRF	 0x61

//**** I/O Register Definitions (0x3F-0x00) ****
#define 	SREG	 0x5F
#define 	SPH		 0x5E
#define 	SPL		 0x5D
#define 	XDIV	 0x5C
#define 	RAMPZ	 0x5B
#define 	EICRB	 0x5A
#define 	EIMSK	 0x59//
#define 	GIMSK	 0x59//		; old name for EIMSK
#define 	GICR	 0x59//		; old name for EIMSK
#define 	EIFR	 0x58
#define 	GIFR	 0x58		// old name for EIFR
#define 	TIMSK	 0x57
#define 	TIFR	 0x56
#define 	MCUCR	 0x55
#define 	MCUCSR	 0x54
#define 	TCCR0	 0x53
#define 	TCNT0	 0x52
#define 	OCR0	 0x51
#define 	ASSR	 0x50
#define 	TCCR1A	 0x4F
#define 	TCCR1B	 0x4E
#define 	TCNT1H	 0x4D
#define 	TCNT1L	 0x4C
#define 	OCR1AH	 0x4B
#define 	OCR1AL	 0x4A
#define 	OCR1BH	 0x49//
#define 	OCR1BL	 0x48
#define 	ICR1H	 0x47
#define 	ICR1L	 0x46
#define 	TCCR2	 0x45
#define 	TCNT2	 0x44
#define 	OCR2	 0x43
#define 	OCDR	 0x42		// New
#define 	WDTCR	 0x41
#define 	SFIOR	 0x40		// New
#define 	EEARH	 0x3F
#define 	EEARL	 0x3E
#define 	EEDR	 0x3D
#define 	EECR	 0x3C
#define 	PORTA	 0x3B
#define 	DDRA	 0x3A
#define 	PINA	 0x39//
#define 	PORTB	 0x38
#define 	DDRB	 0x37
#define 	PINB	 0x36
#define 	PORTC	 0x35
#define 	DDRC	 0x34		// New
#define 	PINC	 0x33		// New
#define 	PORTD	 0x32
#define 	DDRD	 0x31
#define 	PIND	 0x30
#define 	SPDR	 0x2F
#define 	SPSR	 0x2E
#define 	SPCR	 0x2D
#define 	UDR0	 0x2C
#define 	UCSR0A	 0x2B
#define 	UCSR0B	 0x2A
#define 	UBRR0L	 0x29//
#define 	ACSR	 0x28
#define 	ADMUX	 0x27
#define 	ADCSR	 0x26
#define 	ADCH	 0x25
#define 	ADCL	 0x24
#define 	PORTE	 0x23
#define 	DDRE	 0x22
#define 	PINE	 0x21
#define 	PINF	 0x20


//*****************************************************************************
// Bit Definitions
//*****************************************************************************

//**** MCU Control ****
#define 	SRE		 7		// MCUCR
#define 	SRW10	 6
#define 	SE		 5
#define 	SM1		 4
#define 	SM0		 3
#define 	SM2		 2
#define 	IVSEL	 1
#define 	IVCE	 0

#define 	JTD		 7		// MCUCSR
#define 	JTRF	 4	
#define 	WDRF	 3
#define 	BORF	 2
#define 	EXTRF	 1
#define 	PORF	 0

#define 	SRL2	6		// XMCRA
#define 	SRL1	5
#define 	SRL0	4
#define 	SRW01	3
#define 	SRW00	2
#define 	SRW11	1

#define 	XMBK	 7		// XMCRB
#define 	XMM2	 2
#define 	XMM1	 1
#define 	XMM0	 0

#define 	SPMIE	7		// SPMCSR
#define 	ASB		6		// backwards compatiblity
#define 	ASRE	4		// backwards compatiblity
#define 	RWWSB	6
#define 	RWWSRE	4	
#define 	BLBSET	3
#define 	PGWRT	2
#define 	PGERS	1
#define 	SPMEN	0

#define 	IDRD	 7		// OCDR
#define 	OCDR6	 6
#define 	OCDR5	 5
#define 	OCDR4	 4	
#define 	OCDR3	 3
#define 	OCDR2	 2
#define 	OCDR1	 1
#define 	OCDR0	 0

#define 	XDIVEN	 7		// XDIV
#define 	XDIV6	 6
#define 	XDIV5	 5
#define 	XDIV4	 4
#define 	XDIV3	 3
#define 	XDIV2	 2
#define 	XDIV1	 1
#define 	XDIV0	 0

#define 	TSM		 7		// SFIOR
#define 	ADHSM	 4
#define 	ACME	 3
#define 	PUD		 2
#define 	PSR0	 1
#define 	PSR1	 0
#define 	PSR2	 0
#define 	PSR3	 0
#define 	PSR321	 0	

//**** Analog to Digital Converter ****
#define 	ADEN	 7		// ADCSR
#define 	ADSC	 6
#define 	ADFR	 5
#define 	ADIF	 4
#define 	ADIE	 3
#define 	ADPS2	 2
#define 	ADPS1	 1
#define 	ADPS0	 0

#define     REFS1   7		// ADMUX
#define     REFS0   6
#define     ADLAR   5
#define     MUX4    4
#define     MUX3    3
#define     MUX2    2
#define     MUX1    1
#define     MUX0    0

//**** Analog Comparator ****
#define 	ACD		 7		// ACSR
#define 	ACBG	 6
#define 	ACO		 5
#define 	ACI		 4
#define 	ACIE	 3
#define 	ACIC	 2
#define 	ACIS1	 1
#define 	ACIS0	 0
	

//**** External Interrupts ****
#define 	INT7	 7		// EIMSK
#define 	INT6	 6
#define 	INT5	 5
#define 	INT4	 4
#define 	INT3	 3
#define 	INT2	 2
#define 	INT1	 1
#define 	INT0	 0

#define 	INTF7	 7		// EIFR
#define 	INTF6	 6
#define 	INTF5	 5
#define 	INTF4	 4
#define 	INTF3	 3
#define 	INTF2	 2
#define 	INTF1	 1
#define 	INTF0	 0

#define 	ISC71	 7		// EICRB
#define 	ISC70	 6
#define 	ISC61	 5
#define 	ISC60	 4
#define 	ISC51	 3
#define 	ISC50	 2
#define 	ISC41	 1
#define 	ISC40	 0

#define 	ISC31	 7		// EICRA
#define 	ISC30	 6
#define 	ISC21	 5
#define 	ISC20	 4
#define 	ISC11	 3
#define 	ISC10	 2
#define 	ISC01	 1
#define 	ISC00	 0

//**** Timer Interrupts ****
#define 	OCIE2	 7		// TIMSK
#define 	TOIE2	 6
#define 	TICIE1	 5
#define 	OCIE1A	 4
#define 	OCIE1B	 3
#define 	TOIE1	 2
#define 	OCIE0	 1
#define 	TOIE0	 0

#define 	TICIE3	 5		// ETIMSK
#define 	OCIE3A	 4
#define 	OCIE3B	 3
#define 	TOIE3	 2
#define 	OCIE3C	 1
#define 	OCIE1C	 0

#define 	OCF2	 7		// TIFR
#define 	TOV2	 6
#define 	ICF1	 5
#define 	OCF1A	 4
#define 	OCF1B	 3
#define 	TOV1	 2
#define 	OCF0	 1
#define 	TOV0	 0

#define 	ICF3	 5		// ETIFR
#define 	OCF3A	 4
#define 	OCF3B	 3
#define 	TOV3	 2
#define 	OCF3C	 1
#define 	OCF1C	 0

//**** Asynchronous Timer ****
#define 	AS0		 3		// ASSR
#define 	TCN0UB	 2
#define 	OCR0UB	 1
#define 	TCR0UB	 0

//**** Timer 0 ****
#define     FOC0     7		// TCCR0
#define 	WGM00	 6
#define 	COM01	 5
#define 	COM00	 4
#define 	WGM01	 3
#define 	CS02	 2
#define 	CS01	 1
#define 	CS00	 0

//**** Timer 1 ****
#define 	COM1A1	 7		// TCCR1A
#define 	COM1A0	 6
#define 	COM1B1	 5
#define 	COM1B0	 4
#define 	COM1C1	 3
#define 	COM1C0	 2
#define 	PWM11	 1		// OBSOLETE! Use WGM11
#define 	PWM10	 0		// OBSOLETE! Use WGM10
#define 	WGM11	 1
#define 	WGM10	 0

#define 	ICNC1	 7		// TCCR1B
#define 	ICES1	 6
#define 	CTC11	 4		// OBSOLETE! Use WGM13
#define 	CTC10	 3		// OBSOLETE! Use WGM12
#define 	WGM13	 4
#define 	WGM12	 3
#define 	CS12	 2
#define 	CS11	 1
#define 	CS10	 0

#define 	FOC1A	 7		// TCCR1C
#define 	FOC1B	 6
#define 	FOC1C	 5

//**** Timer 2 ****
#define 	FOC2	 7		// TCCR2
#define 	WGM20	 6
#define 	COM21	 5
#define 	COM20	 4
#define 	WGM21	 3
#define 	CS22	 2
#define 	CS21	 1
#define 	CS20	 0

//**** Timer 3 ****
#define 	COM3A1	 7		// TCCR3A
#define 	COM3A0	 6
#define 	COM3B1	 5
#define 	COM3B0	 4
#define 	COM3C1	 3
#define 	COM3C0	 2
#define 	PWM31	 1		// OBSOLETE! Use WGM31
#define 	PWM30	 0		// OBSOLETE! Use WGM30
#define 	WGM31	 1
#define 	WGM30	 0

#define 	ICNC3	 7		// TCCR3B
#define 	ICES3	 6
#define 	CTC31	 4		// OBSOLETE! Use WGM33
#define 	CTC30	 3		// OBSOLETE! Use WGM32
#define 	WGM33	 4
#define 	WGM32	 3
#define 	CS32	 2
#define 	CS31	 1
#define 	CS30	 0

#define 	FOC3A	 7		// TCCR3C
#define 	FOC3B	 6
#define 	FOC3C	 5

//**** Watchdog Timer ****
#define 	WDCE	 4		// WDTCR
#define 	WDTOE	 4		// For Mega103 compability
#define 	WDE	 3
#define 	WDP2	 2
#define 	WDP1	 1
#define 	WDP0	 0

//**** EEPROM  Control Register ****
#define 	EERIE	 3		// EECR
#define 	EEMWE	 2
#define 	EEWE	 1
#define 	EERE	 0

//**** USART 0 and USART 1 ****
#define 	RXC	 7		// (UCSRA0/1)
#define 	TXC	 6
#define 	UDRE	 5
#define 	FE	 4
#define 	DOR	 3
#define 	PE	 2		// OBSOLETED!
#define 	U2X	 1
#define 	MPCM	 0

#define 	RXC0	 7		// (UCSR0A)
#define 	TXC0	 6
#define 	UDRE0	 5
#define 	FE0	 4
#define 	DOR0	 3
#define 	UPE0	 2
#define 	U2X0	 1
#define 	MPCM0	 0

#define 	RXC1	 7		// (UCSR1A)
#define 	TXC1	 6
#define 	UDRE1	 5
#define 	FE1	 4
#define 	DOR1	 3
#define 	UPE1	 2
#define 	U2X1	 1
#define 	MPCM1	 0

#define 	RXCIE	 7		// (UCSRB0/1)
#define 	TXCIE	 6
#define 	UDRIE	 5
#define 	RXEN	 4
#define 	TXEN	 3
#define 	UCSZ2	 2
#define 	RXB8	 1
#define 	TXB8	 0

#define 	RXCIE0	 7		// (UCSR0B)
#define 	TXCIE0	 6
#define 	UDRIE0	 5
#define 	RXEN0	 4
#define 	TXEN0	 3
#define 	UCSZ02	 2
#define 	RXB80	 1
#define 	TXB80	 0

#define 	RXCIE1	 7		// (UCSR1B)
#define 	TXCIE1	 6
#define 	UDRIE1	 5
#define 	RXEN1	 4
#define 	TXEN1	 3
#define 	UCSZ12	 2
#define 	RXB81	 1
#define 	TXB81	 0

#define 	UMSEL	 6		// (UCSRC0/1)
#define 	UPM1	 5
#define 	UPM0	 4
#define 	USBS	 3
#define 	UCSZ1	 2
#define 	UCSZ0	 1
#define 	UCPOL	 0

#define 	UMSEL0	 6		// (UCSR0C)
#define 	UPM01	 5
#define 	UPM00	 4
#define 	USBS0	 3
#define 	UCSZ01	 2
#define 	UCSZ00	 1
#define 	UCPOL0	 0

#define 	UMSEL1	 6		// (UCSR1C)
#define 	UPM11	 5
#define 	UPM10	 4
#define 	USBS1	 3
#define 	UCSZ11	 2
#define 	UCSZ10	 1
#define 	UCPOL1	 0

	
//**** SPI ****
#define 	SPIE	 7		// SPCR
#define 	SPE	 6
#define 	DORD	 5
#define 	MSTR	 4
#define 	CPOL	 3
#define 	CPHA	 2
#define 	SPR1	 1
#define 	SPR0	 0

#define 	SPIF	 7		// SPSR
#define 	WCOL	 6
#define 	SPI2X	 0

//**** TWI ****	
#define 	TWINT	 7		//TWCR
#define 	TWEA	 6
#define 	TWSTA	 5
#define 	TWSTO	 4
#define 	TWWC	 3
#define     TWEN     2
#define 	TWIE	 0

#define 	TWS7	 7		// TWSR
#define 	TWS6	 6
#define 	TWS5	 5
#define 	TWS4	 4
#define 	TWS3	 3
#define 	TWPS1	 1
#define 	TWPS0	 0

#define 	TWA6	 7
#define 	TWA5	 6
#define 	TWA4	 5
#define 	TWA3	 4
#define 	TWA2	 3
#define 	TWA1	 2
#define 	TWA0	 1
#define 	TWGCE	 0		// TWAR

		
//**** PORT A ****
#define 	PA7	 7		// PORTA
#define 	PA6	 6
#define 	PA5	 5
#define 	PA4	 4
#define 	PA3	 3
#define 	PA2	 2
#define 	PA1	 1
#define 	PA0	 0
#define 	PORTA7	 7
#define 	PORTA6	 6
#define 	PORTA5	 5
#define 	PORTA4	 4
#define 	PORTA3	 3
#define 	PORTA2	 2
#define 	PORTA1	 1
#define 	PORTA0	 0

#define 	DDA7	 7		// DDRA
#define 	DDA6	 6
#define 	DDA5	 5
#define 	DDA4	 4
#define 	DDA3	 3
#define 	DDA2	 2
#define 	DDA1	 1
#define 	DDA0	 0

#define 	PINA7	 7		// PINA
#define 	PINA6	 6
#define 	PINA5	 5
#define 	PINA4	 4
#define 	PINA3	 3
#define 	PINA2	 2
#define 	PINA1	 1
#define 	PINA0	 0

//**** PORT B ****
#define 	PB7	 7		// PORTB
#define 	PB6	 6
#define 	PB5	 5
#define 	PB4	 4
#define 	PB3	 3
#define 	PB2	 2
#define 	PB1	 1
#define 	PB0	 0
#define 	PORTB7	 7
#define 	PORTB6	 6
#define 	PORTB5	 5
#define 	PORTB4	 4
#define 	PORTB3	 3
#define 	PORTB2	 2
#define 	PORTB1	 1
#define 	PORTB0	 0

#define 	DDB7	 7		// DDRB
#define 	DDB6	 6
#define 	DDB5	 5
#define 	DDB4	 4
#define 	DDB3	 3
#define 	DDB2	 2
#define 	DDB1	 1
#define 	DDB0	 0

#define 	PINB7	 7		// PINB
#define 	PINB6	 6
#define 	PINB5	 5
#define 	PINB4	 4
#define 	PINB3	 3
#define 	PINB2	 2
#define 	PINB1	 1
#define 	PINB0	 0

//**** PORT C ****
#define 	PC7	 7		// PORTC
#define 	PC6	 6
#define 	PC5	 5
#define 	PC4	 4
#define 	PC3	 3
#define 	PC2	 2
#define 	PC1	 1
#define 	PC0	 0
#define 	PORTC7	 7
#define 	PORTC6	 6
#define 	PORTC5	 5
#define 	PORTC4	 4
#define 	PORTC3	 3
#define 	PORTC2	 2
#define 	PORTC1	 1
#define 	PORTC0	 0

#define 	DDC7	 7		// DDRC
#define 	DDC6	 6
#define 	DDC5	 5
#define 	DDC4	 4
#define 	DDC3	 3
#define 	DDC2	 2
#define 	DDC1	 1
#define 	DDC0	 0

#define 	PINC7	 7		// PINC
#define 	PINC6	 6
#define 	PINC5	 5
#define 	PINC4	 4
#define 	PINC3	 3
#define 	PINC2	 2
#define 	PINC1	 1
#define 	PINC0	 0

//**** PORT D ****
#define 	PD7	 7		// PORTD
#define 	PD6	 6
#define 	PD5	 5
#define 	PD4	 4
#define 	PD3	 3
#define 	PD2	 2
#define 	PD1	 1
#define 	PD0	 0
#define 	PORTD7	 7
#define 	PORTD6	 6
#define 	PORTD5	 5
#define 	PORTD4	 4
#define 	PORTD3	 3
#define 	PORTD2	 2
#define 	PORTD1	 1
#define 	PORTD0	 0

#define 	DDD7	 7		// DDRD
#define 	DDD6	 6
#define 	DDD5	 5
#define 	DDD4	 4
#define 	DDD3	 3
#define 	DDD2	 2
#define 	DDD1	 1
#define 	DDD0	 0

#define 	PIND7	 7		// PIND
#define 	PIND6	 6
#define 	PIND5	 5
#define 	PIND4	 4
#define 	PIND3	 3
#define 	PIND2	 2
#define 	PIND1	 1
#define 	PIND0	 0

//**** PORT E ****
#define 	PE7	 7		// PORTE
#define 	PE6	 6
#define 	PE5	 5
#define 	PE4	 4
#define 	PE3	 3
#define 	PE2	 2
#define 	PE1	 1
#define 	PE0	 0
#define 	PORTE7	 7		// PORTE
#define 	PORTE6	 6
#define 	PORTE5	 5
#define 	PORTE4	 4
#define 	PORTE3	 3
#define 	PORTE2	 2
#define 	PORTE1	 1
#define 	PORTE0	 0

#define 	DDE7	 7		// DDRE
#define 	DDE6	 6
#define 	DDE5	 5
#define 	DDE4	 4
#define 	DDE3	 3
#define 	DDE2	 2
#define 	DDE1	 1
#define 	DDE0	 0

#define 	PINE7	 7		// PINE
#define 	PINE6	 6
#define 	PINE5	 5
#define 	PINE4	 4
#define 	PINE3	 3
#define 	PINE2	 2
#define 	PINE1	 1
#define 	PINE0	 0

//**** PORT F ****
#define 	PF7	 7		// PORTF
#define 	PF6	 6
#define 	PF5	 5
#define 	PF4	 4
#define 	PF3	 3
#define 	PF2	 2
#define 	PF1	 1
#define 	PF0	 0
#define 	PORTF7	 7
#define 	PORTF6	 6
#define 	PORTF5	 5
#define 	PORTF4	 4
#define 	PORTF3	 3
#define 	PORTF2	 2
#define 	PORTF1	 1
#define 	PORTF0	 0

#define 	DDF7	 7		// DDRF
#define 	DDF6	 6
#define 	DDF5	 5
#define 	DDF4	 4
#define 	DDF3	 3
#define 	DDF2	 2
#define 	DDF1	 1
#define 	DDF0	 0

#define 	PINF7	 7		// PINF
#define 	PINF6	 6
#define 	PINF5	 5
#define 	PINF4	 4
#define 	PINF3	 3
#define 	PINF2	 2
#define 	PINF1	 1
#define 	PINF0	 0

//**** PORT G ****
#define 	PG4	 4		// PORTG
#define 	PG3	 3
#define 	PG2	 2
#define 	PG1	 1
#define 	PG0	 0

#define 	DDG4	 4		// DDRG
#define 	DDG3	 3
#define 	DDG2	 2
#define 	DDG1	 1
#define 	DDG0	 0

#define 	PING4	 4		// PING
#define 	PING3	 3
#define 	PING2	 2
#define 	PING1	 1
#define 	PING0	 0

//**** RAM Page Z Select Register **** 
#define  RAMPZ0  0 



//*****************************************************************************
// CPU Register Declarations
//*****************************************************************************

#define 	XL	 r26		// X pointer low
#define 	XH	 r27		// X pointer high
#define 	YL	 r28		// Y pointer low
#define 	YH	 r29		// Y pointer high
#define 	ZL	 r30		// Z pointer low
#define 	ZH	 r31		// Z pointer high


//*****************************************************************************
// Data Memory Declarations
//*****************************************************************************

#define  	RAMEND	 0x10ff		// Highest internal data memory (SRAM) address.
#define 	EEPROMEND  0x0fff       // Highest EEPROM address.
	
//*****************************************************************************
// Program Memory Declarations
//*****************************************************************************

#define     FLASHEND  0xFFFF	//  Highest program memory (flash) address
	                        // (When addressed as 16 bit words)

//*****************************************************************************
// Registers access function
//*****************************************************************************
#define EE_reg(x) (*(volatile EE_UINT8*)(x))

__INLINE__ void EE_regwrite(EE_UINT8 reg, EE_UINT8 val)
{
        *(volatile EE_UINT8*)reg = val;
}

#endif
