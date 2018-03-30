/* ###*B*###
 * ERIKA Enterprise - a tiny RTOS for small microcontrollers
 *
 * Copyright (C) 2002-2011  Evidence Srl
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
 * Author: 2012  Errico Guidieri
 */

#include <ee.h>
#include <ee_irq.h>
#include <ee_internal.h>

/* This file is needed only if System Timer is defined */
#if	( defined(ENABLE_SYSTEM_TIMER) && defined(EE_SYSTEM_TIMER_DEVICE) )

/* Legit devices as system timer */
#define EE_CORTEX_MX_SYSTICK 1

/* Used for compiler error */
#define PREPROC(s) s

/* System Timer Handler */
ISR(EE_cortex_system_timer_handler)
{
#if defined(__OO_BCC1__) || defined(__OO_BCC2__) || defined(__OO_ECC1__) || \
    defined(__OO_ECC2__) || defined(__AS_SC4__)
  IncrementCounterHardware(EE_SYSTEM_TIMER);
#else
  CounterTick(EE_SYSTEM_TIMER);
#endif
}

/* System Timer Initialization */
void EE_cortex_mx_initialize_system_timer(void) {

#if (EE_SYSTEM_TIMER_DEVICE == EE_CORTEX_MX_SYSTICK)
  /* OSTICKDURATION is defined as nanoseconds macro */
  EE_systick_set_period(MICROSECONDS_TO_TICKS(
    (OSTICKDURATION/1000U),
    EE_CPU_CLOCK)
  );
  EE_systick_enable_int();
  EE_systick_start();
#else
#error Unsupported Device: PREPROC(EE_SYSTEM_TIMER_DEVICE) as\
 System Timer!
#endif
}

#endif /* ENABLE_SYSTEM_TIMER */
