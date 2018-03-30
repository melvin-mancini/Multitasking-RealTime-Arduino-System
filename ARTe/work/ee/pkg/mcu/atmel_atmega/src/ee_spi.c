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

/* TODO: (Nino) Convert from PIC30 to AVR5! */

EE_INT8 EE_spi_init(void)
{
	/* 
	 * Note:	the chip select pin (SS1) work manually in master mode,
	 * 		we drive this pin directly setting it as a normal I/O
	 * 		pin.
	 */

	/* Setup SPI I/O pins */
	/* set MISO as input */
	DDRB &= ~(1<<PB3);
	/* set SS, SCK and MOSI as output */
	DDRB |= (1<<PB0)|(1<<PB1)|(1<<PB2);
	/* set SCK and SS as high */
	PORTB |= (1<<PB0)|(1<<PB1);

#ifdef	__AVR_ATmega128__ 
	/* Set clock rate fck/2 */
	/* Note: with these prescale values the port works at 4 MHz. */
	SPCR &= ~(1 << SPR0);
	SPCR &= ~(1 << SPR1);
	SPSR |= (1 << SPI2X);
#endif	/* __AVR_ATmega128__  */
#ifdef	__AVR_ATmega1281__
	/* Set clock rate fck/4 */
	/* Note: with these prescale values the port works at 2 MHz. */
	SPCR |= (1 << SPR0);
	SPCR &= ~(1 << SPR1);
	SPSR &= ~(1 << SPI2X);
#endif	/* __AVR_ATmega1281__* */
	
	/* TODO: (Nino): Clock polarity and Phase are ok? */
	SPCR |= (1<<CPOL)|(1<<CPHA);
	
	/* Enable SPI, Master */
	SPCR |= (1<<SPE)|(1<<MSTR);
	return 1;
}

EE_INT8 EE_spi_close(void)
{
	return 1;
}

EE_INT8 EE_spi_rw_byte(EE_UINT8 data_in, EE_UINT8 *data_out)
{
	/* Start transmission */
	SPDR = data_in;

	/* Wait for transmission complete */
	while( !(SPSR & (1<<SPIF)) );

	/* Start transmission */
	*data_out = SPDR;

	return 1;
}

EE_INT8 EE_spi_write_byte(EE_UINT8 data)
{
	/* Start transmission */
	SPDR = data;

	/* Wait for transmission complete */
	while( !(SPSR & (1<<SPIF)) );

	return 1;
}

EE_INT8 EE_spi_read_byte(EE_UINT8 *data)
{
	/* Start transmission */
	SPDR = data;

	/* Wait for transmission complete */
	while( !(SPSR & (1<<SPIF)) );

	return 1;
}
