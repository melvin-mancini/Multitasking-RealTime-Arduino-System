/** @file	ee_mcu_common_sci.h
 *
 *  @brief	Erika Enterprise - SCI Driver - Atmel ATXMEGA MCUs Header File.
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
 * (ICU driver).
 * 
 *  @author	Riccardo Schiavi
 *  @author	Giuseppe Serano
 *  @version	0.1
 */

#ifndef	__INCLUDE_ATMEL_ATXMEGA_SCI_H__
#define	__INCLUDE_ATMEL_ATXMEGA_SCI_H__

/* 
 * SCI Channels Definition
 */

#define	EE_SCI_CHANNEL_C_0	0x00U	/**< USARTC0 */
#define	EE_SCI_CHANNEL_D_0	0x01U	/**< USARTD0 */

/*
 * SCI Configuration Flags.
 */

/** @brief	Double Transmission Speed
 *
 *  Setting this flag will reduce the divisor of the baud rate divider from 16
 *  to 8, effectively doubling the transfer rate.
 */
#define	EE_SCI_FLAG_CLK2X		0x40U

/** @brief	Parity Disabled. */
#define	EE_SCI_FLAG_PARITY_NONE		0x00U

/** @brief	Parity Even. */
#define	EE_SCI_FLAG_PARITY_EVEN		0x20U

/** @brief	Parity Odd. */
#define	EE_SCI_FLAG_PARITY_ODD		0x30U

/** @brief	1 Stop Bit. */
#define	EE_SCI_FLAG_STOP_BIT_1		0x00U

/** @brief	1 Stop Bit. */
#define	EE_SCI_FLAG_STOP_BIT_2		0x08U

/** @brief	5 Bits. */
#define	EE_SCI_FLAG_5_BITS		0x00U

/** @brief	6 Bits. */
#define	EE_SCI_FLAG_6_BITS		0x01U

/** @brief	7 Bits. */
#define	EE_SCI_FLAG_7_BITS		0x02U

/** @brief	8 Bits. */
#define	EE_SCI_FLAG_8_BITS		0x03U

#ifdef	__SCI_BAUDRATE_TABLE__
/*
 * BaudRate Table Values.
 */
#define	EE_SCI_BAUDRATE_2400		0x00U
#define	EE_SCI_BAUDRATE_4800		0x01U
#define	EE_SCI_BAUDRATE_9600		0x02U
#define	EE_SCI_BAUDRATE_14400		0x03U
#define	EE_SCI_BAUDRATE_19200		0x04U
#define	EE_SCI_BAUDRATE_28800		0x05U
#define	EE_SCI_BAUDRATE_38400		0x06U
#define	EE_SCI_BAUDRATE_57600		0x07U
#define	EE_SCI_BAUDRATE_76800		0x08U
#define	EE_SCI_BAUDRATE_115200		0x09U

#define	EE_SCI_CLOCK_32MHZ		0x00U

#define	EE_TYPESCICLOCK		EE_UREG
#define	EE_TYPESCIBAUDRATE	EE_UREG

#endif	/* __SCI_BAUDRATE_TABLE__ */

#include "mcu/common/inc/ee_mcu_common_sci.h"

#endif	/* __INCLUDE_ATMEL_ATXMEGA_SCI_H__ */
