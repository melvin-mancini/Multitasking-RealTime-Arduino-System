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
 * Context switch functions used in HAL implementations
 * Author: 2011 Gianluca Franchino
 *         2011 Giuseppe Serano
 */

#include "ee_internal.h"

#ifdef	__MONO__

void EE_cortex_mx_change_context(EE_TID tid)
{
  do {
    tid = EE_std_run_task_code(tid);
  } while (EE_std_need_context_change(tid));
}

#endif	/* __MONO__ */

#ifdef __MULTI__

int EE_std_need_context_change(EE_TID tid)
{
  /* FIXME: "tid+1" can be used as an index for arrays even when marked if
   * EE_TID is defined as an int.  Otherwise, the mark will cause a memory
   * access violation!  */
  EE_UTID utid;
  int need_context_change = 1;
  if (tid < 0) {
    /* Unmark the tid to access the EE_std_thread_tos, otherwise undefined
       behaviour. (Index out of arrays boundaries)
       FIXME: #1
    */
    utid = (EE_UTID)tid & (~(EE_UTID)TID_IS_STACKED_MARK);
    need_context_change = (EE_hal_active_tos != EE_std_thread_tos[utid + 1U]);
  }
  return need_context_change;
}

#endif /* __MULTI__ */
