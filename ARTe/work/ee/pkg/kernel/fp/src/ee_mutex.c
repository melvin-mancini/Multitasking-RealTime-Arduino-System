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
 * CVS: $Id: ee_mutex.c,v 1.5 2006/12/03 22:04:56 pj Exp $
 */

#include "ee_internal.h"
#include "../inc/ee_kernel.h"

#ifndef __PRIVATE_GETRESOURCE__
void EE_fp_GetResource(ResourceType m)
{
#ifdef __MSRP__
    register EE_TYPERESOURCE tmp;
#endif
    
    register EE_FREG np_flags;
    np_flags = EE_hal_begin_nested_primitive();

#ifdef __MSRP__
    /* mask off the MSB, that indicates whether this is a global or a
       local resource */
    tmp = m & ~EE_GLOBAL_MUTEX;

    EE_resource_oldceiling[tmp] = EE_sys_ceiling;
    EE_sys_ceiling |= EE_resource_ceiling[tmp];

    /* if this is a global resource, lock the others CPUs */
    if (m & EE_GLOBAL_MUTEX) {
      EE_hal_spin_in((EE_TYPESPIN)tmp);
    }

#else
    
    EE_resource_oldceiling[m] = EE_sys_ceiling;
    EE_sys_ceiling |= EE_resource_ceiling[m];
    
#endif

    EE_hal_end_nested_primitive(np_flags);
}
#endif /* __PRIVATE_GETRESOURCE__ */



#ifndef __PRIVATE_RELEASERESOURCE__
void EE_fp_ReleaseResource(ResourceType m)
{
#ifdef __MSRP__
  register EE_TYPERESOURCE tmp;
#endif

  register EE_TID t;

  register EE_FREG np_flags;
  np_flags = EE_hal_begin_nested_primitive();
  
#ifdef __MSRP__
  tmp = m & ~EE_GLOBAL_MUTEX;

  /* if this is a global resource, unlock the others CPUs */
  if (m & EE_GLOBAL_MUTEX) {
    EE_hal_spin_out((EE_TYPESPIN)tmp);
  }
  
  EE_sys_ceiling = EE_resource_oldceiling[tmp];
#else
  EE_sys_ceiling = EE_resource_oldceiling[m];
#endif

  /* The following code is the same as the code contained into schedule.c! */

  t = EE_rq_queryfirst();
  /* check if there is a preemption */
  if (t != EE_NIL) {
    if (EE_sys_ceiling < EE_th_ready_prio[t]) {
#if defined(__MULTI__)
      register int flag;

      flag = EE_th_status[t] & EE_WASSTACKED;
#endif
      
#if defined(__MULTI__) || defined(__WITH_STATUS__)
      EE_th_status[t] = EE_STACKED;
#endif

      EE_sys_ceiling |= EE_th_dispatch_prio[t];

#if defined(__MULTI__)
      if (flag)
	EE_hal_stkchange(EE_rq2stk_exchange());
      else
	EE_hal_ready2stacked(EE_rq2stk_exchange());
#else
      EE_hal_ready2stacked(EE_rq2stk_exchange());
#endif
    }
  }
  
  EE_hal_end_nested_primitive(np_flags);
}		   
#endif /* __PRIVATE_RELEASERESOURCE__ */
