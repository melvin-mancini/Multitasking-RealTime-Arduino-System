/** @file	ee_mcu_common_gpt.h
 *
 *  @brief	Erika Enterprise - GPT Driver - Atmel ATXMEGA MCUs Header File.
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
 *  The GPT driver provides General Porpose Times(s) services.
 *
 *  The GPT driver is part of the microcontroller abstraction layer (MCAL). It
 *  initializes and controls the internal General Purpose Timer(s) (GPT) of the
 *  microcontroller.
 *
 *  The GPT driver provides services and configuration parameters for
 *  - Starting and stopping hardware timers
 *  - Getting timer values
 *  - Controlling time triggered interrupt notifications
 *
 *  The tick duration of a timer channel depends on channel specific settings
 *  (part of GPT driver) as well as on system clock and settings of the clock
 *  tree controlled by the MCU module. The tick duration is not limited by this
 *  specification.
 *
 *  Not all hardware timers must be controlled by the GPT module. Some timers
 *  may be controlled by Operating System (OS) or Other Drivers directly.
 *
 *  The number of timer channels controlled by the GPT driver depends on
 *  hardware, implementation and system configuration.
 *
 *  The GPT driver only generates time bases, and does not serve as an event
 *  counter. This functionality is provided by another driver module
 *  (ICU driver).
 * 
 *  @author	Riccardo Schiavi
 *  @author	Giuseppe Serano
 *  @version	0.1
 */

#ifndef	__INCLUDE_ATMEL_ATXMEGA_GPT_H__
#define	__INCLUDE_ATMEL_ATXMEGA_GPT_H__

/** @brief	GPT Counter Type
 *
 *  Type for reading and setting the timer values (in number of ticks).
 *  The range of this type is uC dependent (width of the timer register) and has
 *  to be described by the supplier.
 */
#ifndef	EE_TYPEGPTCOUNTER
#define	EE_TYPEGPTCOUNTER	EE_UINT16
#endif

#ifdef	__USE_TC_HW__

#if	( defined(__AVR_ATxmega16D4__) || defined(__AVR_ATxmega32D4__) )

#ifdef	__GPT_CHANNEL_C_0__
/** @brief	Timer/Counter of Type 0 on Port C (TCC0). */
#define	EE_GPT_CHANNEL_C_0	0x00U
#endif

#ifdef	__GPT_CHANNEL_C_1__
/** @brief	Timer/Counter of Type 1 on Port C (TCC1). */
#define	EE_GPT_CHANNEL_C_1	0x04U
#endif

#ifdef	__GPT_CHANNEL_D_0__
/** @brief	Timer/Counter of Type 0 on Port D (TCD0). */
#define	EE_GPT_CHANNEL_D_0	0x01U
#endif

#ifdef	__GPT_CHANNEL_E_0__
/** @brief	Timer/Counter of Type 0 on Port E (TCE0). */
#define	EE_GPT_CHANNEL_E_0	0x02U
#endif

#else	/* __AVR_ATxmega16D4__ || __AVR_ATxmega32D4__ */

#ifdef	__GPT_CHANNEL_C_0__
/** @brief	Timer/Counter of Type 0 on Port C (TCC0). */
#define	EE_GPT_CHANNEL_C_0	0x00U
#endif

#ifdef	__GPT_CHANNEL_C_1__
/** @brief	Timer/Counter of Type 1 on Port C (TCC1). */
#define	EE_GPT_CHANNEL_C_1	0x04U
#endif

#ifdef	__GPT_CHANNEL_D_0__
/** @brief	Timer/Counter of Type 0 on Port D (TCD0). */
#define	EE_GPT_CHANNEL_D_0	0x01U
#endif

#ifdef	__GPT_CHANNEL_D_1__
/** @brief	Timer/Counter of Type 1 on Port C (TCD1). */
#define	EE_GPT_CHANNEL_D_1	0x05U
#endif

#ifdef	__GPT_CHANNEL_E_0__
/** @brief	Timer/Counter of Type 0 on Port E (TCE0). */
#define	EE_GPT_CHANNEL_E_0	0x02U
#endif

#ifdef	__GPT_CHANNEL_E_1__
/** @brief	Timer/Counter of Type 1 on Port E (TCE1). */
#define	EE_GPT_CHANNEL_E_1	0x06U
#endif

#ifdef	__GPT_CHANNEL_F_0__
/** @brief	Timer/Counter of Type 0 on Port F (TCF0). */
#define	EE_GPT_CHANNEL_F_0	0x03U
#endif

#ifdef	__GPT_CHANNEL_F_1__
/** @brief	Timer/Counter of Type 1 on Port F (TCF1). */
#define	EE_GPT_CHANNEL_F_1	0x07U
#endif

#endif	/* __AVR_ATxmega16D4__ || __AVR_ATxmega32D4__ */

#else	/* __USE_TC_HW__ */

#if	( defined(__AVR_ATxmega16D4__) || defined(__AVR_ATxmega32D4__) )

#ifdef	__GPT_CHANNEL_C_0__
/** @brief	Timer/Counter of Type 0 on Port C (TCC0). */
#define	EE_GPT_CHANNEL_C_0	0x00U
#endif

#ifdef	__GPT_CHANNEL_D_0__
/** @brief	Timer/Counter of Type 0 on Port D (TCD0). */
#define	EE_GPT_CHANNEL_D_0	0x01U
#endif

#ifdef	__GPT_CHANNEL_E_0__
/** @brief	Timer/Counter of Type 0 on Port E (TCE0). */
#define	EE_GPT_CHANNEL_E_0	0x02U
#endif

#else	/* __AVR_ATxmega16D4__ || __AVR_ATxmega32D4__ */

#ifdef	__GPT_CHANNEL_C_0__
/** @brief	Timer/Counter of Type 0 on Port C (TCC0). */
#define	EE_GPT_CHANNEL_C_0	0x00U
#endif

#ifdef	__GPT_CHANNEL_D_0__
/** @brief	Timer/Counter of Type 0 on Port D (TCD0). */
#define	EE_GPT_CHANNEL_D_0	0x01U
#endif

#ifdef	__GPT_CHANNEL_E_0__
/** @brief	Timer/Counter of Type 0 on Port E (TCE0). */
#define	EE_GPT_CHANNEL_E_0	0x02U
#endif

#ifdef	__GPT_CHANNEL_F_0__
/** @brief	Timer/Counter of Type 0 on Port F (TCF0). */
#define	EE_GPT_CHANNEL_F_0	0x03U
#endif

#endif	/* __AVR_ATxmega16D4__ || __AVR_ATxmega32D4__ */

#endif	/* __USE_TC_HW__ */

#include "mcu/common/inc/ee_mcu_common_gpt.h"

#endif	/* __INCLUDE_ATMEL_ATXMEGA_GPT_H__ */
