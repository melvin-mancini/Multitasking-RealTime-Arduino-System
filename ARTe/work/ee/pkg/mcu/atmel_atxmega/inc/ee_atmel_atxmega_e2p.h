/** @file	ee_mcu_common_e2p.h
 *
 *  @brief	Erika Enterprise - E2P Driver - Atmel ATXMEGA MCUs Header File.
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
 *  @author	Giuseppe Serano
 *  @version	0.1
 */

#ifndef	__INCLUDE_ATMEL_ATXMEGA_E2P_H__
#define	__INCLUDE_ATMEL_ATXMEGA_E2P_H__

/** @brief	Base Address.
 *
 * This parameter is the EEPROM device base address.
 *
 * Implementation Type: 	<tt>EE_TYPEE2PADDR</tt>.
 */
#if	( defined(__AVR_ATxmega16D4__) || defined(__AVR_ATxmega32D4__) )
#define	EE_E2P_BASE_ADDRESS	0x1000U
#else	/* __AVR_ATxmega16D4__ || __AVR_ATxmega32D4__ */
#define	EE_E2P_BASE_ADDRESS	0x0000U	/* Unknown Base Address */
#endif	/* __AVR_ATxmega16D4__ || __AVR_ATxmega32D4__ */

/** @brief	Size.
 *
 * This parameter is the used size of EEPROM device in bytes.
 *
 * Implementation Type: 	<tt>EE_TYPEE2PDATASZ</tt>.
 */
#if	( defined(__AVR_ATxmega16D4__) || defined(__AVR_ATxmega32D4__) )
#define	EE_E2P_SIZE		0x0400U	/* 1K Bytes */
#else	/* __AVR_ATxmega16D4__ || __AVR_ATxmega32D4__ */
#define	EE_E2P_SIZE		0x0000U	/* Unknown Size */
#endif	/* __AVR_ATxmega16D4__ || __AVR_ATxmega32D4__ */

/** @brief	Page Size.
 *
 * This parameter is the used page size of EEPROM device in bytes.
 *
 * Implementation Type: 	<tt>EE_TYPEE2PDATASZ</tt>.
 */
#if	( defined(__AVR_ATxmega16D4__) || defined(__AVR_ATxmega32D4__) )
#define	EE_E2P_PAGESIZE		0x0020U	/* 32 Bytes */
#else	/* __AVR_ATxmega16D4__ || __AVR_ATxmega32D4__ */
#define	EE_E2P_PAGESIZE		0x0000U	/* Unknown Size */
#endif	/* __AVR_ATxmega16D4__ || __AVR_ATxmega32D4__ */

#ifndef	__E2P_USE_INTERRUPT__
/** @brief	Job Call Cycle.
 *
 *  Call cycle time of the EEPROM driver's main function.
 *
 *  Unit:	[ms]
 */
#define	EE_E2P_JOB_CALL_CYCLE	0x0001U
#endif	/* __E2P_USE_INTERRUPT__ */

/** @brief	E2P Power Reduction Flag.
 *
 *  Setting this flag enables power saving for the EEPROM.
 *  The EEPROM will then be turned off in a manner equal to entering sleep mode.
 *  If access is required, the bus master will be halted for a time equal to the
 *  start-up time from idle sleep mode.
 */
#define	EE_E2P_FLAG_POWER	0x02U

#include "mcu/common/inc/ee_mcu_common_e2p.h"

#endif	/* __INCLUDE_ATMEL_ATXMEGA_E2P_H__ */
