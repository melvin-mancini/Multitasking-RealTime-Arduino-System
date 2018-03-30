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
 *
 * CVS: $Id: ee_thact.c,v 1.8 2006/12/03 22:04:56 pj Exp $
 */

#include "ee_internal.h"
#include "../inc/ee_kernel.h"

#ifndef __PRIVATE_ACTIVATETASK__

void EE_fp_ActivateTask(TaskType t)
{
  register EE_TID tmp;
  register EE_FREG flag;
  
#ifdef __RN_TASK__
  int rn_return_val;

  if (EE_IS_TID_REMOTE(t)) {
    register EE_TYPERN_PARAM par;
    par.pending = 1U;
    /* forward the request to another CPU whether the task do
       not become to the current CPU */
    rn_return_val = EE_rn_send((EE_SREG)EE_MARK_REMOTE_TID(t),
EE_RN_TASK, par );
  } else {
#endif
    
    flag = EE_hal_begin_nested_primitive();
    
    /* check for first activation */
    if (EE_th_nact[t] == (EE_UREG)0U) {
#if defined(__MULTI__) || defined(__WITH_STATUS__)
      EE_th_status[t] = EE_READY;
#endif
      EE_rq_insert(t);
    }
    
    /* activate the task avoiding the counter wraparound */
    if (EE_th_nact[t] != EE_MAX_NACT) {
      EE_th_nact[t] ++;
    }
    
    /* check for preemption */
    if (EE_hal_get_IRQ_nesting_level() == 0U) {
      tmp = EE_rq_queryfirst();
      if (tmp != EE_NIL) {
	if (EE_sys_ceiling < EE_th_ready_prio[tmp]) {
#if defined(__MULTI__)
	  register int wasstacked;
	  
	  wasstacked = EE_th_status[tmp] & EE_WASSTACKED;
#endif
	  
#if defined(__MULTI__) || defined(__WITH_STATUS__)
	  EE_th_status[tmp] = EE_STACKED;
#endif
	  
	  EE_sys_ceiling |= EE_th_dispatch_prio[tmp];
	  
#if defined(__MULTI__)
	  if (wasstacked)
	    EE_hal_stkchange(EE_rq2stk_exchange());
	  else
	    EE_hal_ready2stacked(EE_rq2stk_exchange());
#else
	  EE_hal_ready2stacked(EE_rq2stk_exchange());
#endif
	}
      }
    }
    
    EE_hal_end_nested_primitive(flag);
    
#ifdef __RN_TASK__
  }
#endif
}

#endif
