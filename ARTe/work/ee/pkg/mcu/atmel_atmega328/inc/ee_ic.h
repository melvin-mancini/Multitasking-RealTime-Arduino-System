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
 * CVS: $Id: ee_ic.h,v 1.2 2006/08/02 12:07:32 romano Exp $
 */

#include "mcu/atmel_atmega328/inc/ee_mcuregs.h"

#ifndef __INCLUDE_ATMEGA328_IC_H__
#define __INCLUDE_ATMEGA328_IC_H__

/*************************************************************************
 Interrupts handling
 *************************************************************************/

/* this is the source code for the support of the KS32C50100 Interrupt
   controller. This file declares a set of functions for handling the
   various registers of the peripheral, and a set of function that
   helps the initializations of some table used during the interrupt
   handling routines. 

   The following defines work only when __ALLOW_NESTED_IRQ__ is present:
   __IRQ_PRIORITY_MASKS__ is used to specify if some additional
   software should be used to appropriately set up the IRQ masks.

   __IRQ_PRIORITY_MASKS_NOCONST__ is used to specify that the mask
   vector used is not const. That can be useful if the masks changes
   during the application life. masks can be recalculated using the
   provided function EE_IC_compute_masks.

   __IRQ_VECTOR_NOCONST__ is used to specify that the IRQ vector can
   be changed at runtime. That feauture can be useful if you want to
   change the irq handlers during the life of the application
*/


/*
 * Constants 
 */

/* Masks */

/* Priorities */



/* 
 * Types
 */

/* a number between 0 and 21 */
typedef EE_UINT32 EE_TYPEIRQ;

/* a bitmask from 0x0 to 0xFF */
typedef EE_UINT8 EE_TYPEIRQMASK;


typedef void (* EE_FUN)(void);


		     


/* 
 * Variables
 */


#ifdef __IRQ_VECTOR_NOCONST__
#define EE_IRQVECTOR_CONST
#else
#define EE_IRQVECTOR_CONST const
#endif

// This is the external interrupt vector, that contains a function
//   pointer for each external interrupt in the system

extern EE_ADDR EE_IC_EXTIRQ_vector[22];

#if defined(__IRQ_PRIORITY_MASKS__) && defined(__ALLOW_NESTED_IRQ__)
/* the set of masks that can be used to mask the INTMSK register to
   implement a priority nesting */

#ifdef __IRQ_PRIORITY_MASKS_NOCONST__
#define EE_EXTIRQMASK_CONST
#else
#define EE_EXTIRQMASK_CONST const
#endif

extern EE_EXTIRQMASK_CONST EE_TYPEIRQMASK EE_IC_EXTIRQ_masks[21];
#endif




/* 
 * Functions
 */

/* parameters are a bit mask obtained using IRQ_*_MASK */

/* This function enables the external IRQ mask given as parameter.
   Inthis case, the global mask is enabled by default
 */
 
void initIntExt0(void);
void initIntExt1(void);

void disableExtInt0(void);
void disableExtInt1(void);


void EE_IC_enable_external_IRQ(EE_TYPEIRQMASK i);


/* This function disables the external IRQs passed as parameters.
   The global mask in this case is not modified if not explicitly stated */
void EE_IC_disable_external_IRQ(EE_TYPEIRQMASK i);




/* signal to the interrupt controller that the interrupts i have been
   serviced. it is typically not used, because the IRQ/FIQ handlers
   take care of it */
__INLINE__ void EE_IC_accept_external_IRQ(EE_TYPEIRQMASK i)
{

}

/* clears ALL the pending interrupts */
__INLINE__ void EE_IC_clear_pending_IRQ(void)
{
clearPendingIRQ();
}


/* This function computes the masks to be used as values for
   EE_IC_EXTIRQ_masks. Usage: set the INTPRI* registers as wanted,
   then call this function. It will fill up a vector of 20 words
   containing the values for the INTMSK vector. 

   result must be a vector of at least 21 words.

   masked is the set of interrupts that are always masked (e.g.,
   because their interrupt must never be raised.
*/
void EE_IC_compute_masks(void);


#endif

