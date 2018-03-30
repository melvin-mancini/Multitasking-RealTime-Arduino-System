/** @file	ee_mcu_common_pwm.h
 *
 *  @brief	Erika Enterprise - PWM Driver - Atmel ATXMEGA MCUs Header File.
 *
 *  <!--
 *    ERIKA Enterprise - a tiny RTOS for small microcontrollers
 *
 *    Copyright (C) 2002-2013  Evidence Srl
 *
 *    This file is part of ERIKA Enterprise.
 *
 *    ERIKA Enterprise is free software; you can redistribute it
 *    and/or modify it under the terms of the GNU General Public License
 *    version 2 as published by the Free Software Foundation, 
 *    (with a special exception described below).
 *
 *    Linking this code statically or dynamically with other modules is
 *    making a combined work based on this code.  Thus, the terms and
 *    conditions of the GNU General Public License cover the whole
 *    combination.
 *
 *    As a special exception, the copyright holders of this library give you
 *    permission to link this code with independent modules to produce an
 *    executable, regardless of the license terms of these independent
 *    modules, and to copy and distribute the resulting executable under
 *    terms of your choice, provided that you also meet, for each linked
 *    independent module, the terms and conditions of the license of that
 *    module.  An independent module is a module which is not derived from
 *    or based on this library.  If you modify this code, you may extend
 *    this exception to your version of the code, but you are not
 *    obligated to do so.  If you do not wish to do so, delete this
 *    exception statement from your version.
 *
 *    ERIKA Enterprise is distributed in the hope that it will be
 *    useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 *    of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License version 2 for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    version 2 along with ERIKA Enterprise; if not, write to the
 *    Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *    Boston, MA 02110-1301 USA.
 *  -->
 *
 *  This specification specifies the functionality, API and the configuration
 *  of the PWM driver.
 *
 *  Each PWM channel is linked to a hardware PWM which belongs to the
 *  microcontroller.
 *
 *  The type of the PWM signal (for example center Align, left Align, Etc...) is
 *  not defined within this specification and is left up to the implementation. 
 *
 *  The driver provides functions for initialization and control of the
 *  microcontroller internal PWM stage (pulse width modulation).
 *
 *  The PWM module generates pulses with variable pulse width. It allows the
 *  selection ofthe duty cycle and the signal period time.
 *
 *  The PWM depends on the system clock. Thus, changes of the system clock (e.g. 
 *  PLL on -> PLL off) also affect the clock settings of the PWM hardware.
 *
 *  All functions from the PWM module except <tt>EE_Pwm_Init()</tt> and
 *  <tt>Pwm_DeInit</tt> shall be re-entrant for different PWM channel numbers.
 *
 *  The PWM module's user shall ensure the integrity if several function calls
 *  are made during run time in different TASKs or ISRs for the same PWM
 *  channel.
 *
 *  To get times out of register values it is necessary to know the oscillator
 *  frequency, prescalers and so on. Since these settings are made in MCU and/or
 *  in other modules it is not possible to calculate such times. Hence the
 *  conversions between time and ticks shall be part of an upper layer.
 *
 *  All time units used within the API services of the PWM module shall be of
 *  the unit ticks.
 * 
 *  @author	Riccardo Schiavi
 *  @author	Giuseppe Serano
 *  @version	0.1
 */

#ifndef	__INCLUDE_ATMEL_ATXMEGA_PWM_H__
#define	__INCLUDE_ATMEL_ATXMEGA_PWM_H__

/** @brief	PWM Period Type
 *
 *  Type for setting the period values (in number of ticks).
 *  The range of this type is uC dependent (width of the pwm register) and has
 *  to be described by the supplier.
 */
#ifndef	EE_TYPEPWMPERIOD
#define	EE_TYPEPWMPERIOD	EE_UINT16
#endif

/** @brief	PWM Duty-Cycle Type
 *
 *  Type for setting the duty-cycle values (in number of ticks).
 *  The range of this type is uC dependent (width of the pwm register) and has
 *  to be described by the supplier.
 */
#ifndef	EE_TYPEPWMDUTYCYCLE
#define	EE_TYPEPWMDUTYCYCLE	EE_UINT16
#endif

#if	( defined(__AVR_ATxmega16D4__) || defined(__AVR_ATxmega32D4__) )

#ifdef	__PWM_CHANNEL_C_0__
/** @brief	Timer/Counter of Type 0 on Port C (TCC0). */
#define	EE_PWM_CHANNEL_C_0	0x00U
#endif

#ifdef	__PWM_CHANNEL_C_1__
/** @brief	Timer/Counter of Type 1 on Port C (TCC1). */
#define	EE_PWM_CHANNEL_C_1	0x04U
#endif

#ifdef	__PWM_CHANNEL_D_0__
/** @brief	Timer/Counter of Type 0 on Port D (TCD0). */
#define	EE_PWM_CHANNEL_D_0	0x01U
#endif

#ifdef	__PWM_CHANNEL_E_0__
/** @brief	Timer/Counter of Type 0 on Port E (TCC0). */
#define	EE_PWM_CHANNEL_E_0	0x02U
#endif

#else	/* __AVR_ATxmega16D4__ || __AVR_ATxmega32D4__ */

#ifdef	__PWM_CHANNEL_C_0__
/** @brief	Timer/Counter of Type 0 on Port C (TCC0). */
#define	EE_PWM_CHANNEL_C_0	0x00U
#endif

#ifdef	__PWM_CHANNEL_C_1__
/** @brief	Timer/Counter of Type 1 on Port C (TCC1). */
#define	EE_PWM_CHANNEL_C_1	0x04U
#endif

#ifdef	__PWM_CHANNEL_D_0__
/** @brief	Timer/Counter of Type 0 on Port D (TCD0). */
#define	EE_PWM_CHANNEL_D_0	0x01U
#endif

#ifdef	__PWM_CHANNEL_D_1__
/** @brief	Timer/Counter of Type 1 on Port D (TCD1). */
#define	EE_PWM_CHANNEL_D_1	0x05U
#endif

#ifdef	__PWM_CHANNEL_E_0__
/** @brief	Timer/Counter of Type 0 on Port E (TCC0). */
#define	EE_PWM_CHANNEL_E_0	0x02U
#endif

#ifdef	__PWM_CHANNEL_E_1__
/** @brief	Timer/Counter of Type 1 on Port E (TCE1). */
#define	EE_PWM_CHANNEL_E_1	0x06U
#endif

#ifdef	__PWM_CHANNEL_F_0__
/** @brief	Timer/Counter of Type 0 on Port F (TCF0). */
#define	EE_PWM_CHANNEL_F_0	0x03U
#endif

#ifdef	__PWM_CHANNEL_F_1__
/** @brief	Timer/Counter of Type 1 on Port F (TCF1). */
#define	EE_PWM_CHANNEL_F_1	0x07U
#endif

#endif	/* __AVR_ATxmega16D4__ || __AVR_ATxmega32D4__ */

#include "mcu/common/inc/ee_mcu_common_pwm.h"

#endif	/* __INCLUDE_ATMEL_ATXMEGA_PWM_H__ */
