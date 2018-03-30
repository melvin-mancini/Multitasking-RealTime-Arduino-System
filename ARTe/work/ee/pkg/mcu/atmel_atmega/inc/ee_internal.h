/* ###*B*###
 * ERIKA Enterprise - a tiny RTOS for small microcontrollers
 *
 * Copyright (C) 2002-2014  Evidence Srl
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
 * Author: 2005 Antonio Romano
 * 	   2014 Giuseppe Serano
 */

#include "mcu/atmel_atmega/inc/ee_mcu.h"

#ifndef __INCLUDE_MCU_ATMEGA_INTERNAL_H__
#define __INCLUDE_MCU_ATMEGA_INTERNAL_H__

/* I need AVR Interrupt */
#include <avr/interrupt.h>

/* I need AVR Input/Output */
#include <avr/io.h>

/*
 * ISR Priorities.
*/
#define	EE_ISR_PRI_OFF	0x00U
#define	EE_ISR_PRI_LOW	0x01U
#define	EE_ISR_PRI_MED	0x02U
#define	EE_ISR_PRI_HIGH	0x03U

#define	EE_ATMEGA_VALIDATE(_exp)	\
  if( !(_exp) ) {	\
    return;		\
  }

#define	EE_ATMEGA_VALIDATE_IRQ(_exp,_flags)	\
  if( !(_exp) ) {				\
    EE_hal_resumeIRQ(_flags);			\
    return;					\
  }

#define EE_ATMEGA_VALIDATE_W_RV(_exp,_rv)	\
  if( !(_exp) ) {				\
    return (_rv);				\
  }

#define EE_ATMEGA_VALIDATE_IRQ_W_RV(_exp,_rv,_flags)	\
  if( !(_exp) ) {					\
    EE_hal_resumeIRQ(_flags);				\
    return (_rv);					\
  }

#define	EE_LOW_BYTE_SH_BITS	0x08U

__INLINE__ EE_UINT16 __ALWAYS_INLINE__
EE_ATmega_Reg16Read(EE_HWREG_PTR Address)
{
  register EE_UINT16	ret = EE_HWREG(Address++);
  ret |= ( (EE_UINT16)EE_HWREG(Address) << EE_LOW_BYTE_SH_BITS);
  return ret;
}

__INLINE__ void __ALWAYS_INLINE__
EE_ATmega_Reg16Write(EE_HWREG_PTR Address, EE_UINT16 Data)
{
  EE_HWREG(Address++) = (EE_UREG)(Data);
  EE_HWREG(Address) = (EE_UREG)(Data >> EE_LOW_BYTE_SH_BITS);
}

/* True if we are inside an interrupt handler */
#define	EE_is_inside_hw_ISR_call()	EE_FALSE

#endif /* __INCLUDE_MCU_ATMEGA_INTERNAL_H__ */
