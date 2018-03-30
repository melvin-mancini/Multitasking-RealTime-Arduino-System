/** @file	ee_mcu_common_gpt.h
 *
 *  @brief	Erika Enterprise - RTC Driver - Atmel ATXMEGA MCUs Header File.
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
 *  The RTC driver provides Real Time Counter(s) services.
 *
 *  The RTC driver is part of the microcontroller abstraction layer (MCAL). It
 *  initializes and controls the internal Real Time Counter(s) (RTC) of the
 *  microcontroller.
 *
 *  The RTC driver provides services and configuration parameters for
 *  - Starting and stopping hardware counters
 *  - Getting counter values
 *  - Controlling time triggered interrupt notifications
 *
 *  The tick duration of a RTC channel depends on channel specific settings
 *  (part of RTC driver) as well as on system clock and settings of the clock
 *  tree controlled by the MCU module. The tick duration is not limited by this
 *  specification.
 *
 *  Not all hardware counters must be controlled by the RTC module.
 *  Some counters  may be controlled by Operating System (OS) or Other Drivers
 *  directly.
 *
 *  The number of RTC channels controlled by the RTC driver depends on hardware,
 *  implementation and system configuration.
 * 
 *  @author	Riccardo Schiavi
 *  @author	Giuseppe Serano
 *  @version	0.1
 */

#ifndef	__INCLUDE_ATMEL_ATXMEGA_RTC_H__
#define	__INCLUDE_ATMEL_ATXMEGA_RTC_H__

/** @brief	RTC Counter Type
 *
 *  Type for reading and setting the counter values (in number of ticks).
 *  The range of this type is uC dependent (width of the counter register) and
 *  has to be described by the supplier.
 */
#ifndef	EE_TYPERTCCOUNTER
#define	EE_TYPERTCCOUNTER	EE_UINT16
#endif

#ifdef	__RTC_CHANNEL_0__
/** @brief	RTC Channel 0. */
#define	EE_RTC_CHANNEL_0	0x00U
#endif

#include "mcu/common/inc/ee_mcu_common_rtc.h"

#endif	/* __INCLUDE_ATMEL_ATXMEGA_RTC_H__ */
