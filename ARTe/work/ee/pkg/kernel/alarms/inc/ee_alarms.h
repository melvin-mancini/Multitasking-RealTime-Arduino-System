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
 * Author 2002 Paolo Gai
 * CVS: $Id: ee_alarms.h,v 1.4 2006/12/03 22:03:53 pj Exp $
 */


#ifndef __INCLUDE_KERNEL_ALARMS_ALARMS_H__
#define __INCLUDE_KERNEL_ALARMS_ALARMS_H__


/*************************************************************************
 Alarm Types
 *************************************************************************/


/* alarm id type (signed!) */
#ifndef EE_TYPEALARM
#define EE_TYPEALARM EE_SREG
#endif

/* counter id type (signed!) */
#ifndef EE_TYPECOUNTER
#define EE_TYPECOUNTER EE_SREG
#endif

/* counter tick type (signed!) */
#ifndef EE_TYPETICK
#define EE_TYPETICK EE_SREG
#endif

/* notification type (signed!) */
#ifndef EE_TYPENOTIFY
#define EE_TYPENOTIFY EE_UINT8
#endif


/*************************************************************************
 Variables defined by the application
 *************************************************************************/

/* initvalue: {0, -1} */
typedef struct {
  EE_TYPETICK value;              /* current value of the counter */
  EE_TYPEALARM first;         /* first alarm queued on the counter */
} EE_counter_RAM_type;

/* these are the different types of alarm notifications... */
#define EE_ALARM_ACTION_TASK 0U
#define EE_ALARM_ACTION_CALLBACK 1U

/* initvalue: {a_valid_counter, a_valid_action, then you must put the correct
   parameters depending on the action } */
typedef struct {
  EE_TYPECOUNTER c;           /* the counter linked to the alarm */

  EE_TYPENOTIFY action;
  
  EE_TID TaskID;

  EE_VOID_CALLBACK f;
} EE_alarm_ROM_type;

/* initvalue: all zeroes --> no initialization! */
typedef struct {
  EE_TYPETICK cycle;          /* cycle for periodic alarms */
  EE_TYPETICK delta;          /* delta expiration time (into a queue!) */
  EE_TYPEALARM next;          /* next alarm in the delta queue */
} EE_alarm_RAM_type;

/* this is the RAM part of a counter. 
   Initvalue = an array of {0,-1} elements */
extern EE_counter_RAM_type       EE_counter_RAM[];

/* this is the fixed part of the configuration of an alarm
   Initvalue= depends on how the alarm notification have to be configured */
extern const EE_alarm_ROM_type   EE_alarm_ROM[];

/* this part is the variable part of an alarm.
   Initvalue: an array of {0,0,0,-1} */
extern EE_alarm_RAM_type         EE_alarm_RAM[];


/***************************************************************************
 * Public Types
 ***************************************************************************/

typedef EE_TYPEALARM   AlarmType;
typedef EE_TYPECOUNTER CounterType;
typedef EE_TYPETICK    TickType;
typedef EE_TYPETICK    *TickRefType;


/***************************************************************************
 * Implementation defined functions
 ***************************************************************************/

/* This function notifies a tick to a counter. That is, the counter is
   incremented by 1.  It must be called into an ISR or into a
   thread notify that the event linked to the counter occurred. 

   The function will also implement the notification of expired alarms
   (calling an alarm callback or activating a thread). 

   The function is NOT atomic, and NO RESCHEDULING will take place
   after the execution of this function. To implement the
   rescheduling at task level, you can use the Schedule() function
   just after this notification.

   These functions can be called both into a thread and into an ISR.
*/
#ifndef __PRIVATE_COUNTER_TICK__
void EE_alarm_CounterTick(CounterType c);
#endif

#ifndef __PRIVATE_ALARM_GET__
void EE_alarm_GetAlarm(AlarmType AlarmID, TickType *Tick);
#endif

#ifndef __PRIVATE_ALARM_SETREL__
void EE_alarm_SetRelAlarm(AlarmType AlarmID, 
			  TickType increment, TickType cycle);
#endif

#ifndef __PRIVATE_ALARM_SETABS__
void EE_alarm_SetAbsAlarm(AlarmType AlarmID, 
			  TickType start, TickType cycle);
#endif

#ifndef __PRIVATE_ALARM_CANCEL__
void EE_alarm_CancelAlarm(AlarmType AlarmID);
#endif

/* Internal function */
void EE_alarm_insert(AlarmType AlarmID, TickType increment);

#endif
