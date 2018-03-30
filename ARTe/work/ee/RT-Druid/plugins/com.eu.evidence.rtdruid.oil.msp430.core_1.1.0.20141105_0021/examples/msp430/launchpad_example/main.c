/* ###*B*###
 * ERIKA Enterprise - a tiny RTOS for small microcontrollers
 *
 * Copyright (C) 2010  Steve Langstaff
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
 * Author: 2011,  Steve Langstaff
 */

#include "ee.h"

/* flag to indicate that reference temperature should be updated */
static volatile int read_reference = 1;

/* configure timer A to give us periodic interrupts */
static void __inline__ timerA_init (void)
    {
	/* configure timer A to give us interrupts */
    TACCTL0 = CCIE;

    /* source clock from SMCLK, continuous UP */
    TACTL = TASSEL_2 | MC_2;
    }

/* configure ADC to use internal temperature sensor */
static void __inline__ adc_temp_sensor_init (void)
    {
    ADC10CTL1 = INCH_10 + ADC10DIV_3;
    ADC10CTL0 = SREF_1 + ADC10SHT_3 + REFON + ADC10ON + ADC10IE;
    __delay_cycles (1000);
    }

static void __inline__ adc_temp_sensor_start (void)
    {
    /* this will start the conversion and cause an interrupt when done */
    ADC10CTL0 |= ENC + ADC10SC;
    }

static long __inline__ adc_temp_sensor_read (void)
    {
    return ADC10MEM;
    }

ISR2 (PORT1_VECTOR)
    {
    if (EE_buttons_interrupt (BUTTON_0))
        {
        /* clear outstanding interrupt flag */
        EE_buttons_clear_ISRflag (BUTTON_0);

    	/* reset our baseline */
        read_reference = 1;
        }
    }

ISR2 (TIMERA0_VECTOR)
    {
    ActivateTask (Task1);
    }

ISR2 (ADC10_VECTOR)
    {
    ActivateTask (Task2);
    }

TASK (Task1)
    {
	/* start the temperature measurement */
	adc_temp_sensor_start ();
    }

/* read ADC value and update LEDs */
TASK (Task2)
    {
    static long temp_ref = 0;
    long temp_now = adc_temp_sensor_read ();

    if (read_reference)
        {
        read_reference = 0;
        temp_ref = temp_now;
        }

    if (temp_now > temp_ref)
        {
        EE_led_0_off ();
        EE_led_1_toggle ();
        }
    else if (temp_now == temp_ref)
        {
        EE_led_0_on ();
        EE_led_1_on ();
        }
    else
        {
        EE_led_1_off ();
        EE_led_0_toggle ();
        }
    }

int main(void)
    {
    EE_watchdog_disable ();

    EE_sysclk_init_1MHz ();

    EE_leds_init ();

    EE_buttons_init ();

    EE_buttons_enable_interrupts (BUTTON_0);

    adc_temp_sensor_init ();

    timerA_init ();

    EE_hal_enableIRQ ();

    for (;;)
        {
        /* enter low power mode and wait for interrupts */
        EE_msp430_enter_LPM (LPMODE_0);
        }

    return 0;
    }
