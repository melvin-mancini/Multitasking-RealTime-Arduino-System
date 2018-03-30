/**************************************************************************//**
*
* @file    main.c
* @brief   Running LED demo for EnSilica ERIKA Enterprise port
* @version $Id: $
* @author  Steve Langstaff
*
* @note
* COPYRIGHT (C) 2010-2011 Pebble Bay Consulting Ltd.  ALL RIGHTS RESERVED.
*
* @par
* This software is the property of Pebble Bay Consulting Ltd and/or its
* licencees. No right, title, ownership or other interest is hereby granted
* or transferred. The information contained is valuable confidential
* information that should not be disclosed to any third party.
*/

/**
@details
This file provides a running light demo for EnSilica ERIKA Enterprise port.
@file
*/

#include "ee.h"
#include "cpu/esi_risc/inc/ee_irq.h"

#include <esirisc/esirisc.h>

/* Information about the timer device, such as which interrupt it is using. */
static void * timer0Device;

static const unsigned char kitt[] = {LED_0, LED_1, LED_2, LED_3, LED_2, LED_1 };
#define KITT_LIMIT    (sizeof(kitt)/sizeof(kitt[0]))

/**************************************************************************//**
*
* Task1 - Illuminate the next LED in the pattern
*
* This task displays the next LED in the \ref kitt pattern.
*/
TASK(ledUpdate)
{
    static int kittIndex = 0;

    EE_leds_off(kitt[kittIndex]);
    if (++kittIndex >= KITT_LIMIT)
    {
        kittIndex = 0;
    }
    EE_leds_on(kitt[kittIndex]);
}

/**************************************************************************//**
*
* timer 0 IRQ handler
*
* This IRQ hander acknowledges the timer IRQ and activates Task1.
*/
ISR(timer0IrqHandler)
{
	timer_irq_ack(timer0Device);
	ActivateTask(ledUpdate);
}

/**************************************************************************//**
*
* Main entry point
*
* This function displays the first of the \ref kitt LED patterns and then
* starts a timer. The IRQ hander for the timer will step through the rest of
* the pattern.
*/
int main(void)
{
	EE_leds_init();

    EE_leds_on(kitt[0]);

	timer_init(&timer0Device, 0, esi_get_frequency()/10, timer0IrqHandler);

	/* Enable interrupts to the CPU. */
	esi_interrupt_enable();
	
	/* sleep, waiting for interrupts */
	for (;;)
	{
		asm("wait");
	}

	return 0;
}
