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
 * Author: 2006 Antonio Romano
 * 	   2014 Giuseppe Serano
 */

#include <avr/io.h>

#define	TEMP	r18
.global	EE_atmega_initIntExt0
.global	EE_atmega_initIntExt1
#if	( defined(__AVR_ATmega128__) || defined(__AVR_ATmega1281__) )
.global	EE_atmega_initIntExt2
.global	EE_atmega_initIntExt3
.global	EE_atmega_initIntExt4
.global	EE_atmega_initIntExt5
.global	EE_atmega_initIntExt6
.global	EE_atmega_initIntExt7
#endif	/* __AVR_ATmega128__ || __AVR_ATmega1281__ */
.global	EE_atmega_disableExtInt0
.global	EE_atmega_disableExtInt1
#if	( defined(__AVR_ATmega128__) || defined(__AVR_ATmega1281__) )
.global	EE_atmega_disableExtInt2
.global	EE_atmega_disableExtInt3
.global	EE_atmega_disableExtInt4
.global	EE_atmega_disableExtInt5
.global	EE_atmega_disableExtInt6
.global	EE_atmega_disableExtInt7
#endif	/* __AVR_ATmega128__ || __AVR_ATmega1281__ */

.global	EE_atmega_clearPendingIRQ

.text

EE_atmega_initIntExt0:
	IN	TEMP,	DDRD
	ANDI	TEMP,	0xFC
	OUT	DDRD,	TEMP
	IN	TEMP,	PORTD
	ORI	TEMP,	0x01
	OUT	PORTD,	TEMP	; pullup activated
	LDS	TEMP,	EICRA
	ORI	TEMP,	0x02
	STS	EICRA,	TEMP
	LDS	TEMP,	EIFR
	ORI	TEMP,	0x01
	STS	EIFR,	TEMP
	LDS	TEMP,	EIMSK
	ORI	TEMP,	0x01	; int0 bit-mask
	STS	EIMSK,	TEMP
	LDI	TEMP,	0x0F	; interrupt int0
	STS	MCUCR,	TEMP
	; SEI
	RET

EE_atmega_initIntExt1:
	IN	TEMP,	DDRD
	ANDI	TEMP,	0xFD
	OUT	DDRD,	TEMP
	IN	TEMP,	PORTD
	ORI	TEMP,	0x02
	OUT	PORTD,	TEMP	; pullup activated
	LDS	TEMP,	EICRA
	ORI	TEMP,	0x0C
	STS	EICRA,	TEMP
	LDS	TEMP,	EIFR
	ORI	TEMP,	0x02
	STS	EIFR,	TEMP
	LDS	TEMP,	EIMSK
	ORI	TEMP,	0x02	; int1 bit-mask
	STS	EIMSK,	TEMP
	LDI	TEMP,	0x0F	; interrupt int1
	STS	MCUCR,	TEMP
	; SEI
	RET

#if	( defined(__AVR_ATmega128__) || defined(__AVR_ATmega1281__) )
EE_atmega_initIntExt2:
	IN	TEMP,	DDRD
	ANDI	TEMP,	0xFB
	OUT	DDRD,	TEMP
	IN	TEMP,	PORTD
	ORI	TEMP,	0x04
	OUT	PORTD,	TEMP	; pullup activated
	LDS	TEMP,	EICRA
	ORI	TEMP,	0x30
	STS	EICRA,	TEMP
	LDS	TEMP,	EIFR
	ORI	TEMP,	0x04
	STS	EIFR,	TEMP
	LDS	TEMP,	EIMSK
	ORI	TEMP,	0x04	; int2 bit-mask
	STS	EIMSK,	TEMP
	LDI	TEMP,	0x0F	; interrupt int2
	STS	MCUCR,	TEMP
	; SEI
	RET

EE_atmega_initIntExt3:
	IN	TEMP,	DDRD
	ANDI	TEMP,	0xF7
	OUT	DDRD,	TEMP
	IN	TEMP,	PORTD
	ORI	TEMP,	0x08
	OUT	PORTD,	TEMP	; pullup activated
	LDS	TEMP,	EICRA
	ORI	TEMP,	0xC0
	STS	EICRA,	TEMP
	LDS	TEMP,	EIFR
	ORI	TEMP,	0x08
	STS	EIFR,	TEMP
	LDS	TEMP,	EIMSK
	ORI	TEMP,	0x08	; int3 bit-mask
	STS	EIMSK,	TEMP
	LDI	TEMP,	0x0F	; interrupt int3
	STS	MCUCR,	TEMP
	; SEI
	RET

EE_atmega_initIntExt4:
	IN	TEMP,	DDRE
	ANDI	TEMP,	0xEF
	OUT	DDRE,	TEMP
	IN	TEMP,	PORTE
	ORI	TEMP,	0x10
	OUT	PORTE,	TEMP	; pullup activated
	LDS	TEMP,	EICRB
	ORI	TEMP,	0x03
	STS	EICRB,	TEMP
	LDS	TEMP,	EIFR
	ORI	TEMP,	0x10
	STS	EIFR,	TEMP
	LDS	TEMP,	EIMSK
	ORI	TEMP,	0x10	; int4 bit-mask
	STS	EIMSK,	TEMP
	LDI	TEMP,	0x0F	; interrupt int4
	STS	MCUCR,	TEMP
	; SEI
	RET

EE_atmega_initIntExt5:
	IN	TEMP,	DDRE
	ANDI	TEMP,	0xDF
	OUT	DDRE,	TEMP
	IN	TEMP,	PORTE
	ORI	TEMP,	0x20
	OUT	PORTE,	TEMP	; pullup activated
	LDS	TEMP,	EICRB
	ORI	TEMP,	0x0C
	STS	EICRB,	TEMP
	LDS	TEMP,	EIFR
	ORI	TEMP,	0x20
	STS	EIFR,	TEMP
	LDS	TEMP,	EIMSK
	ORI	TEMP,	0x20	; int5 bit-mask
	STS	EIMSK,	TEMP
	LDI	TEMP,	0x0F	; interrupt int5
	STS	MCUCR,	TEMP
	; SEI
	RET

EE_atmega_initIntExt6:
	IN	TEMP,	DDRE
	ANDI	TEMP,	0xBF
	OUT	DDRE,	TEMP
	IN	TEMP,	PORTE
	ORI	TEMP,	0x40
	OUT	PORTE,	TEMP	; pullup activated
	LDS	TEMP,	EICRB
	ORI	TEMP,	0x30
	STS	EICRB,	TEMP
	LDS	TEMP,	EIFR
	ORI	TEMP,	0x40
	STS	EIFR,	TEMP
	LDS	TEMP,	EIMSK
	ORI	TEMP,	0x40	; int6 bit-mask
	STS	EIMSK,	TEMP
	LDI	TEMP,	0x0F	; interrupt int6
	STS	MCUCR,	TEMP
	; SEI
	RET

EE_atmega_initIntExt7:
	IN	TEMP,	DDRE
	ANDI	TEMP,	0x8F
	OUT	DDRE,	TEMP
	IN	TEMP,	PORTE
	ORI	TEMP,	0x80
	OUT	PORTE,	TEMP	; pullup activated
	LDS	TEMP,	EICRB
	ORI	TEMP,	0xC0
	STS	EICRB,	TEMP
	LDS	TEMP,	EIFR
	ORI	TEMP,	0x80
	STS	EIFR,	TEMP
	LDS	TEMP,	EIMSK
	ORI	TEMP,	0x80	; int7 bit-mask
	STS	EIMSK,	TEMP
	LDI	TEMP,	0x0F	; interrupt int7
	STS	MCUCR,	TEMP
	; SEI
	RET
#endif	/* __AVR_ATmega128__ || __AVR_ATmega1281__ */

EE_atmega_disableExtInt0:
	; CLI
	LDS	TEMP,	EIMSK
	ANDI	TEMP,	0xFE
	STS	EIMSK,	TEMP
	; SEI
	RET

EE_atmega_disableExtInt1:
	; CLI
	LDS	TEMP,	EIMSK
	ANDI	TEMP,	0xFD
	STS	EIMSK,	TEMP
	; SEI
	RET

#if	( defined(__AVR_ATmega128__) || defined(__AVR_ATmega1281__) )
EE_atmega_disableExtInt2:
	; CLI
	LDS	TEMP,	EIMSK
	ANDI	TEMP,	0xFB
	STS	EIMSK,	TEMP
	; SEI
	RET

EE_atmega_disableExtInt3:
	 CLI
	LDS	TEMP,	EIMSK
	ANDI	TEMP,	0xF7
	STS	EIMSK,	TEMP
	; SEI
	RET

EE_atmega_disableExtInt4:
	; CLI
	LDS	TEMP,	EIMSK
	ANDI	TEMP,	0xEF
	STS	EIMSK,	TEMP
	; SEI
	RET

EE_atmega_disableExtInt5:
	; CLI
	LDS	TEMP,	EIMSK
	ANDI	TEMP,	0xDF
	STS	EIMSK,	TEMP
	; SEI
	RET

EE_atmega_disableExtInt6:
	; CLI
	LDS	TEMP,	EIMSK
	ANDI	TEMP,	0xBF
	STS	EIMSK,	TEMP
	; SEI
	RET

EE_atmega_disableExtInt7:
	; CLI
	LDS	TEMP,	EIMSK
	ANDI	TEMP,	0xF7
	STS	EIMSK,	TEMP
	; SEI
	RET
#endif	/* __AVR_ATmega128__ || __AVR_ATmega1281__ */

EE_atmega_clearPendingIRQ:
	; CLI
	SER	TEMP
	STS	EIFR,	TEMP
	; SEI
	RET

