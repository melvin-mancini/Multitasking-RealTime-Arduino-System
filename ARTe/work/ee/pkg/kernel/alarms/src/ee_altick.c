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
 * Author: 2002 Paolo Gai
 * CVS: $Id: ee_altick.c,v 1.9 2008/07/14 10:41:58 pj Exp $
 */

#include "ee_internal.h"

void EE_alarm_insert(AlarmType AlarmID, TickType increment)
{
  register AlarmType current, previous;

  current = EE_counter_RAM[EE_alarm_ROM[AlarmID].c].first;

  if (current == (AlarmType)-1) {
    /* the alarm becomes the first into the delta queue */
    EE_counter_RAM[EE_alarm_ROM[AlarmID].c].first = AlarmID;
  } else if (EE_alarm_RAM[current].delta > increment) {
    EE_counter_RAM[EE_alarm_ROM[AlarmID].c].first = AlarmID;
    EE_alarm_RAM[current].delta -= increment;
  }
  else {
    /* the alarm is not the first into the delta queue */

    /* follow the delta chain until I reach the right place */
    do {
      increment -= EE_alarm_RAM[current].delta;
      previous = current;
      current = EE_alarm_RAM[current].next;
    } while((current != (AlarmType)-1) && (EE_alarm_RAM[current].delta <= increment));

    /* insert the alarm between previous and current */
    if (current != (AlarmType)-1) {
      EE_alarm_RAM[current].delta -= increment;
    }
    EE_alarm_RAM[previous].next = AlarmID;
  }

  EE_alarm_RAM[AlarmID].delta = increment;
  EE_alarm_RAM[AlarmID].next = current;
}

#ifndef __PRIVATE_COUNTER_TICK__
void EE_alarm_CounterTick(CounterType c)
{
  register EE_TYPEALARM current;
  register EE_TID t;
  register EE_FREG flag;
#ifdef __RN_TASK__
  int rn_return_val;
#endif
  
  flag = EE_hal_begin_nested_primitive();

  /* increment the counter value */
  EE_counter_RAM[c].value++;

  /* if there are queued alarms */
  if (EE_counter_RAM[c].first != (EE_TYPEALARM)-1) {
    /* decrement first queued alarm delta */
    EE_alarm_RAM[EE_counter_RAM[c].first].delta--;

    /* execute all the alarms with counter 0 */
    current = EE_counter_RAM[c].first;
    while (EE_alarm_RAM[current].delta == 0) {

      /* remove the current entry */
      EE_counter_RAM[c].first = EE_alarm_RAM[current].next;

      /* the alarm is cyclic? */
      if (EE_alarm_RAM[current].cycle) {
	/* enqueue it again 
	   note: this can modify EE_counter_RAM[c].first!!! see (*)
	*/
	EE_alarm_insert(current,EE_alarm_RAM[current].cycle);
      }

      /* execute it */
      switch (EE_alarm_ROM[current].action) {

      case  EE_ALARM_ACTION_TASK:
	/* activate the task; NOTE: no preemption at all... 
	   This code was directly copied from ActivateTask */

	t = EE_alarm_ROM[current].TaskID;

	/* this code is similar to the first part of thread_activate */
#ifdef __RN_TASK__
	  if ((EE_UTID)t & (EE_UTID)EE_REMOTE_TID) {
	    register EE_TYPERN_PARAM par;
	    par.pending = 1U;
	    /* forward the request to another CPU whether the thread do
	       not become to the current CPU */
	    rn_return_val = EE_rn_send((EE_SREG)EE_MARK_REMOTE_TID(t),
EE_RN_TASK, par);
	  } else {
#endif



#if defined (__FRSH__) || defined(__HR__)

	    {
	      register EE_TIME tmp_time;
	      tmp_time = EE_hal_gettime();
	  
		  
	      if (EE_th[t].nact == 0) {
#ifdef __HR__			  
		  if(EE_ct[EE_th[t].vres].sched_algo==EE_SCHED_EDF)
			EE_th[t].absdline = tmp_time+EE_th[t].reldline;
			
		  if (EE_hr_updatecapacity(t, tmp_time) == EE_UC_InsertRDQueue){
#else
		if (EE_frsh_updatecapacity(t, tmp_time) == EE_UC_InsertRDQueue){
#endif
		  EE_rq_insert(t);
		}
#ifdef __HR__			  
		else { // EE_UC_InsertedRCGQueue
			EE_insert_in_vres(t);
        }
		EE_vres[EE_th[t].vres].act_tasks++;
#endif		
		EE_th[t].status = EE_TASK_READY;
	      } 
	      EE_th[t].nact++;
	    }
#else
	   if (EE_th_nact[t] == (EE_TYPENACT)0U) {
#ifdef __EDF__
	      // compute the deadline 
	      EE_th_absdline[t] = EE_hal_gettime()+EE_th_reldline[t];
#endif
#if defined(__MULTI__) || defined(__WITH_STATUS__)
	      EE_th_status[t] = EE_READY;
#endif
	      EE_rq_insert(t);
	    }
	    EE_th_nact[t]++;
#endif




#ifdef __RN_TASK__
	  }
#endif
	  
  	break;

      case EE_ALARM_ACTION_CALLBACK:
	(EE_alarm_ROM[current].f)();
	break;
      default:
        /* Invalid action: this should never happen, as `action' is
           initialized by RT-Druid */
        break;
      }

      /* (*) here we need EE_counter_RAM[c].first again... */
      if ((current = EE_counter_RAM[c].first) == (EE_TYPEALARM)-1) {
          break;
      }
    }
  }    

  EE_hal_end_nested_primitive(flag);
}
#endif
