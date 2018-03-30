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
 * (ICU driver).
 * 
 *  @author	Riccardo Schiavi
 *  @author	Giuseppe Serano
 *  @version	0.1
 */

#ifndef	__INCLUDE_ATMEL_ATXMEGA_GPIO_H__
#define	__INCLUDE_ATMEL_ATXMEGA_GPIO_H__

/* 
 * GPIO Channels Definition
 */

/* PORT A */
#define	EE_GPIO_PORT_A		0x00U	/**< PORT A	    */
#define	EE_GPIO_CHANNEL_A_0	0x00U	/**< PORT A - PIN 0 */
#define	EE_GPIO_CHANNEL_A_1	0x01U	/**< PORT A - PIN 1 */
#define	EE_GPIO_CHANNEL_A_2	0x02U	/**< PORT A - PIN 2 */
#define	EE_GPIO_CHANNEL_A_3	0x03U	/**< PORT A - PIN 3 */
#define	EE_GPIO_CHANNEL_A_4	0x04U	/**< PORT A - PIN 4 */
#define	EE_GPIO_CHANNEL_A_5	0x05U	/**< PORT A - PIN 5 */
#define	EE_GPIO_CHANNEL_A_6	0x06U	/**< PORT A - PIN 6 */
#define	EE_GPIO_CHANNEL_A_7	0x07U	/**< PORT A - PIN 7 */

/* PORT B */
#define	EE_GPIO_PORT_B		0x01U	/**< PORT B	    */
#define	EE_GPIO_CHANNEL_B_0	0x10U	/**< PORT B - PIN 0 */
#define	EE_GPIO_CHANNEL_B_1	0x11U	/**< PORT B - PIN 1 */
#define	EE_GPIO_CHANNEL_B_2	0x12U	/**< PORT B - PIN 2 */
#define	EE_GPIO_CHANNEL_B_3	0x13U	/**< PORT B - PIN 3 */
#define	EE_GPIO_CHANNEL_B_4	0x14U	/**< PORT B - PIN 4 */
#define	EE_GPIO_CHANNEL_B_5	0x15U	/**< PORT B - PIN 5 */
#define	EE_GPIO_CHANNEL_B_6	0x16U	/**< PORT B - PIN 6 */
#define	EE_GPIO_CHANNEL_B_7	0x17U	/**< PORT B - PIN 7 */

/* PORT C */
#define	EE_GPIO_PORT_C		0x02U	/**< PORT C	    */
#define	EE_GPIO_CHANNEL_C_0	0x20U	/**< PORT C - PIN 0 */
#define	EE_GPIO_CHANNEL_C_1	0x21U	/**< PORT C - PIN 1 */
#define	EE_GPIO_CHANNEL_C_2	0x22U	/**< PORT C - PIN 2 */
#define	EE_GPIO_CHANNEL_C_3	0x23U	/**< PORT C - PIN 3 */
#define	EE_GPIO_CHANNEL_C_4	0x24U	/**< PORT C - PIN 4 */
#define	EE_GPIO_CHANNEL_C_5	0x25U	/**< PORT C - PIN 5 */
#define	EE_GPIO_CHANNEL_C_6	0x26U	/**< PORT C - PIN 6 */
#define	EE_GPIO_CHANNEL_C_7	0x27U	/**< PORT C - PIN 7 */

/* PORT D */
#define	EE_GPIO_PORT_D		0x03U	/**< PORT D	    */
#define	EE_GPIO_CHANNEL_D_0	0x30U	/**< PORT D - PIN 0 */
#define	EE_GPIO_CHANNEL_D_1	0x31U	/**< PORT D - PIN 1 */
#define	EE_GPIO_CHANNEL_D_2	0x32U	/**< PORT D - PIN 2 */
#define	EE_GPIO_CHANNEL_D_3	0x33U	/**< PORT D - PIN 3 */
#define	EE_GPIO_CHANNEL_D_4	0x34U	/**< PORT D - PIN 4 */
#define	EE_GPIO_CHANNEL_D_5	0x35U	/**< PORT D - PIN 5 */
#define	EE_GPIO_CHANNEL_D_6	0x36U	/**< PORT D - PIN 6 */
#define	EE_GPIO_CHANNEL_D_7	0x37U	/**< PORT D - PIN 7 */

/* PORT E */
#define	EE_GPIO_PORT_E		0x04U	/**< PORT E	    */
#define	EE_GPIO_CHANNEL_E_0	0x40U	/**< PORT E - PIN 0 */
#define	EE_GPIO_CHANNEL_E_1	0x41U	/**< PORT E - PIN 1 */
#define	EE_GPIO_CHANNEL_E_2	0x42U	/**< PORT E - PIN 2 */
#define	EE_GPIO_CHANNEL_E_3	0x43U	/**< PORT E - PIN 3 */
#define	EE_GPIO_CHANNEL_E_4	0x44U	/**< PORT E - PIN 4 */
#define	EE_GPIO_CHANNEL_E_5	0x45U	/**< PORT E - PIN 5 */
#define	EE_GPIO_CHANNEL_E_6	0x46U	/**< PORT E - PIN 6 */
#define	EE_GPIO_CHANNEL_E_7	0x47U	/**< PORT E - PIN 7 */

#if	( defined(__AVR_ATxmega16D4__) || defined(__AVR_ATxmega32D4__) )

/* 
 * NOTE:	Ports from F to Q are not available on ATxmega16D4 MCU
 * 		Port R is used for XTAL or TOSC
 */

#else	/* __AVR_ATxmega16D4__ || __AVR_ATxmega32D4__ */

/* PORT R */
#define	EE_GPIO_PORT_R		0x0FU	/**< PORT R	    */
#define	EE_GPIO_CHANNEL_R_0	0xF0U	/**< PORT R - PIN 0 */
#define	EE_GPIO_CHANNEL_R_1	0xF1U	/**< PORT R - PIN 1 */
#define	EE_GPIO_CHANNEL_R_2	0xF2U	/**< PORT R - PIN 2 */
#define	EE_GPIO_CHANNEL_R_3	0xF3U	/**< PORT R - PIN 3 */
#define	EE_GPIO_CHANNEL_R_4	0xF4U	/**< PORT R - PIN 4 */
#define	EE_GPIO_CHANNEL_R_5	0xF5U	/**< PORT R - PIN 5 */
#define	EE_GPIO_CHANNEL_R_6	0xF6U	/**< PORT R - PIN 6 */
#define	EE_GPIO_CHANNEL_R_7	0xF7U	/**< PORT R - PIN 7 */

#endif	/* __AVR_ATxmega16D4__ || __AVR_ATxmega32D4__ */

#include "mcu/common/inc/ee_mcu_common_gpio.h"

#endif	/* __INCLUDE_ATMEL_ATXMEGA_GPIO_H__ */
