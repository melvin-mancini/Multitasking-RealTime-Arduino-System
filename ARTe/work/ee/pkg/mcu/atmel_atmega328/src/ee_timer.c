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
  * Author: 2012 Pasquale Buonocunto
  * 
  */

#include "mcu/atmel_atmega328/inc/ee_mcuregs.h"

void init_timer0(EE_UINT8 max_tick);
void timer0_start();
void timer0_stop();

void init_timer1(EE_UINT16 max_tick);
void timer1_start();
void timer1_stop();

void init_timer2(EE_UINT8 max_tick);
void timer2_start();
void timer2_stop();



void init_timer0(EE_UINT8 max_tick)
{
   TCCR0B |= (1 << WGM02);  // Configure timer 0 for CTC mode
   TIMSK0 |= (1 << OCIE0A); // Enable CTC interrupt

  // EE_avr5_enableIRQ();     //  Enable global interrupts
   
   TIFR0 |= (1 << OCF0A);
   OCR0A = max_tick;        // Set CTC compare value max_tick
}

void timer0_start()
{
	TCCR0B |= ((1 << CS00) | (1 << CS01)); // Start timer at Fcpu/64 (prescaler)
}

void timer0_stop()
{
	TCCR0B &= ~((1 << CS00) | (1 << CS01)); // Stop timer
}




void init_timer1(EE_UINT16 max_tick) // 16 bit timer
{
   TCCR1B |= (1 << WGM12);  // Configure timer 1 for CTC mode
   TIMSK1 |= (1 << OCIE1A); // Enable CTC interrupt

 //  EE_avr5_enableIRQ();     //  Enable global interrupts
   TIFR1 |= (1 << OCF1A);
   OCR1A = max_tick;      // Set CTC compare value max_tick
}

void timer1_start()
{
	TCCR1B |= ((1 << CS10) | (1 << CS11)); // Start timer at Fcpu/64 (prescaler)
}

void timer1_stop()
{
	TCCR1B &= ~((1 << CS10) | (1 << CS11)); // Stop timer
}




void init_timer2(EE_UINT8 max_tick)
{

}

void timer2_start()
{
}

void timer2_stop()
{
}
