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
 * Author: 2001-2002 Paolo Gai
 * CVS: $Id: ee_irqsc.c,v 1.3 2006/05/14 17:49:02 pj Exp $
 */

#include "ee_internal.h"


#ifndef __PRIVATE_IRQ_END_INSTANCE__
/* This primitive shall be atomic.
   This primitive shall be inserted as the last function in an IRQ handler.
   If the HAL allow IRQ nesting the C_end_instance should work as follows:
   - it must implement the preemption test only if it is the last IRQ on the stack
   - if there are other interrupts on the stack the IRQ end_instance should do nothing
*/
void EE_IRQ_end_instance(void)
{
    register EE_TID t;

    t = EE_rq_queryfirst();
    if ((t != EE_NIL) && (EE_sys_ceiling < EE_th_ready_prio[t])) {
#if defined(__MULTI__)
      register int flag;

      flag = EE_th_status[t] & EE_WASSTACKED;
#endif

      EE_sys_ceiling |= EE_th_dispatch_prio[t];

#if defined(__MULTI__) || defined(__WITH_STATUS__)
      EE_th_status[t] = EE_STACKED;
#endif

#if defined(__MULTI__)
      if (flag)
	EE_hal_IRQ_stacked(EE_rq2stk_exchange());
      else
	EE_hal_IRQ_ready(EE_rq2stk_exchange());
#else
      EE_hal_IRQ_ready(EE_rq2stk_exchange());
#endif
    } else {
      EE_hal_IRQ_stacked(EE_stk_queryfirst());
    }
}

#endif /* __PRIVATE_IRQ_END_INSTANCE__ */
