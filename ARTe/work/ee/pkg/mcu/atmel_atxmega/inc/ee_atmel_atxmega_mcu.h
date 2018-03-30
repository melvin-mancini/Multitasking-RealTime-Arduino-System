/** @file	ee_atmel_atxmega_mcu.h
 *
 *  @brief	Erika Enterprise - MCU Driver - Atmel ATXMEGA MCUs Header File.
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
 *  The MCU driver provides MCU services for Clock and RAM initialization.
 *  In the MCU configuration set, the MCU specific settings for the Clock
 *  (i.e. PLL setting) and RAM (i.e. section base address and size) shall be
 *  configured.
 *
 *  The MCU module shall provide a service to provide software triggering of a
 *  hardware reset.
 *
 *  @note	Only an authorized user shall be able to call this reset service
 *  		function.
 *
 *  The MCU module shall provide services to get the reset reason of the last
 *  reset if the hardware supports such a feature.
 *
 *  @note	In an ECU, there are several sources which can cause a reset.
 *  		Depending on the reset reason, several application scenarios
 *  		might be necessary after reinitialization of the MCU.
 * 
 *  @author	Giuseppe Serano
 *  @version	0.1
 *  @date	2013
 */

#ifndef	__INCLUDE_ATMEL_ATXMEGA_MCU_H__
#define	__INCLUDE_ATMEL_ATXMEGA_MCU_H__

/*****************************************************************************
 Configuration Flags.
 ******************************************************************************/

/*****************************************************************************
 Clock Sources.
 ******************************************************************************/

/** @brief	MCU Clock Sources Configuration Flags.
 *
 *  Type for setting-up a MCU Clock Sources.
 *
 *  Implementation specific. But not all values may be valid within this type.
 *
 *  This type shall be chosen in order to have the most efficient implementation
 *  on a specific micro controller platform.
 *
 *  For parameter values of type <tt>EE_TYPEMCUCLKSRCSFLAGS</tt>, the Mcu's user
 *  shall use the symbolic names provided by the driver implementation.
 *
 *  The type shall be EE_UREG depending on uC platform.
 * 
 */
#define	EE_TYPEMCUCLKSRCSFLAGS	EE_UINT32

/*
 * Internal Oscillators.
 */

/** @brief	2MHz Run-time Calibrated Oscillator.
 *
 *  The 2MHz run-time calibrated internal oscillator is the default system clock
 *  source after reset.
 *
 *  It is calibrated during production to provide a default frequency close to
 *  its nominal frequency.
 *
 *  A digital fre-quency looked loop (DFLL) can be enabled for automatic
 *  run-time calibration of the oscillator to compensate for temperature and
 *  voltage drift and optimize the oscillator accuracy.
 */
#define	EE_MCU_CLK_SRCS_FLAG_RC2MHZ	0x00000001U

/** @brief	32MHz Run-timeCalibrated Oscillator.
 *
 *  The 32MHz run-time calibrated internal oscillator is a high-requency
 *  oscillator.
 *
 *  It is calibrated during production to provide a default frequency close to
 *  its nominal frequency.
 *
 *  A digital fre-quency looked loop (DFLL) can be enabled for automatic
 *  run-time calibration of the oscillator to compensate for temperature and
 *  voltage drift and optimize the oscillator accuracy.
 *
 *  This oscillator can also be adjusted and calibrated to any frequency between
 *  30MHz and 55MHz.
 */
#define	EE_MCU_CLK_SRCS_FLAG_RC32MHZ	0x00000002U

/** @brief	32.768kHz Calibrated Oscillator.
 *
 *  This oscillator provides an approximate 32.768kHz clock.
 *
 *  It is calibrated during production to provide a default frequency close to
 *  its nominal frequency.
 *
 *  The calibration register can also be written from software for
 *  run-time calibration of the oscillator frequency.
 *
 *  The oscillator employs a built-in prescaler, which provides both a 32.768kHz
 *  output and a 1.024kHz output.
 */
#define	EE_MCU_CLK_SRCS_FLAG_RC32768HZ	0x00000004U

/** @brief	External Oscillator.
 *
 *  @see	Below.
 */
#define	EE_MCU_CLK_SRCS_FLAG_XOSC	0x00000008U

/** @brief	PLL
 *
 *  The built-in phase locked loop (PLL) can be used to generate a
 *  high-frequency system clock.
 *
 *  The PLL has a user-selectable multiplication factor of from 1 to 31.
 *
 *  The output frequency, fOUT, is given by the input frequency, fIN, multiplied
 *  by the multiplication factor, PLL_FAC.
 *
 *  Four different clock sources can be chosen as input to the PLL:
 *  - 2MHz internal oscillator
 *  - 32MHz internal oscillator divided by 4
 *  - 0.4MHz - 16MHz crystal oscillator
 *  - External clock
 *
 *  To enable the PLL, the following procedure must be followed:
 *  1. Enable reference clock source.
 *  2. Set the multiplication factor and select the clock reference for the PLL.
 *  3. Wait until the clock reference source is stable.
 *  4. Enable the PLL.
 *
 *  Hardware ensures that the PLL configuration cannot be changed when the PLL
 *  is in use. The PLL must be disabled before a new configuration can be
 *  written.
 *
 *  It is not possible to use the PLL before the selected clock source is stable
 *  and the PLL has locked.
 *
 *  The reference clock source cannot be disabled while the PLL is running.
 */
#define	EE_MCU_CLK_SRCS_FLAG_PLL	0x00000010U

/*
 * External Oscillators.
 */

/** @brief	External Clock
 *
 *  To drive the device from an external clock source, XTAL1 must be driven as
 *  External Clock Signal Input. In this mode, XTAL2 can be used as a general
 *  I/O pin.
 *
 *  @note	Start-Up Time: 6 CLK
 *
 *  @note	When the external oscillator is used as the reference for a
 *  		DFLL, only EXTCLK and 32KHZ can be selected.
 */
#define	EE_MCU_CLK_SRCS_FLAG_XOSC_EXTCLK	0x00000000U

/** @brief	32.768kHz Crystal Oscillator
 *
 *  A 32.768kHz crystal oscillator can be connected between the TOSC1 and TOSC2
 *  pins and enables a dedicated low frequency oscillator inputcircuit.
 *
 *  A low power mode with reduced voltage swing on TOSC2 is available.
 *
 *  This oscillator can be used as a clock source for the system clock and RTC,
 *  and as the DFLL refer-ence clock.
 *
 *  @note	Start-Up Time: 16K CLK
 *
 *  @note	When the external oscillator is used as the reference for a
 *  		DFLL, only EXTCLK and 32KHZ can be selected.
 */
#define	EE_MCU_CLK_SRCS_FLAG_XOSC_32KHZ		0x00000200U

/** @brief	0.4MHz - 16MHz Crystal Oscillator - Start-Up Time 256 CLK
 *
 *  This oscillator can operate in four different modes optimized for different
 *  frequency ranges, all within 0.4MHz - 16MHz.
 *
 *  @note	Start-Up Time: 256 CLK
 *
 *  @note	This flag should be used only when frequency stability at
 *  		startup is not important for the application.
 *  		The option is not suitable for crystals.
 */
#define	EE_MCU_CLK_SRCS_FLAG_XOSC_XTAL_256	0x00000300U

/** @brief	0.4MHz - 16MHz Crystal Oscillator - Start-Up Time 1K CLK
 *
 *  This oscillator can operate in four different modes optimized for different
 *  frequency ranges, all within 0.4MHz - 16MHz.
 *
 *  @note	Start-Up Time: 1K CLK
 *
 *  @note	This flag is intended for use with ceramic resonators. It can
 *  		also be used when the frequency stability at startup is not
 *  		important for the application.
 */
#define	EE_MCU_CLK_SRCS_FLAG_XOSC_XTAL_1K	0x00000700U

/** @brief	0.4MHz - 16MHz Crystal Oscillator - Start-Up Time 16K CLK
 *
 *  This oscillator can operate in four different modes optimized for different
 *  frequency ranges, all within 0.4MHz - 16MHz.
 *
 *  @note	Start-Up Time: 16K CLK
 */
#define	EE_MCU_CLK_SRCS_FLAG_XOSC_XTAL_16K	0x00000B00U

/** @brief	 Crystal Oscillator Drive
 *
 *  Setting this flag will increase the current in the 0.4MHz - 16MHz crystal
 *  oscillator and increase the swing on the XTAL2 pin.
 *
 *  This allows for driving crystals with higher load or higher frequency than
 *  specfiied by the frequency range flags.
 *
 */
#define	EE_MCU_CLK_SRCS_FLAG_XOSC_PWR		0x00001000U

/** @brief	Crystal Oscillator 32.768kHz Low Power Mode
 *
 *  Setting this flag enables the low power mode for the 32.768kHz crystal
 *  oscillator. This will reduce the swing on the TOSC2 pin.
 */
#define	EE_MCU_CLK_SRCS_FLAG_XOSC_32K_LPM	0x00002000U

/** @brief	0.4MHz - 2MHz Crystal Oscillator.
 *
 *  @note	Recommended Range for Capacitors C1 and C2 (pF): 100-300
 */
#define	EE_MCU_CLK_SRCS_FLAG_XOSC_XTAL_04TO2	0x00000000U

/** @brief	2MHz - 9MHz Crystal Oscillator.
 *
 *  @note	Recommended Range for Capacitors C1 and C2 (pF): 10-40
 */
#define	EE_MCU_CLK_SRCS_FLAG_XOSC_XTAL_2TO9	0x00004000U

/** @brief	9MHz - 12MHz Crystal Oscillator.
 *
 *  @note	Recommended Range for Capacitors C1 and C2 (pF): 10-40
 */
#define	EE_MCU_CLK_SRCS_FLAG_XOSC_XTAL_9TO12	0x00008000U

/** @brief	12MHz - 16MHz Crystal Oscillator.
 *
 *  @note	Recommended Range for Capacitors C1 and C2 (pF): 10-40
 */
#define	EE_MCU_CLK_SRCS_FLAG_XOSC_XTAL_12TO16	0x0000C000U

/*
 * RTC.
 */

/** @brief	RTC Clock Source Enable.
 *
 *  Setting this flag enables the selected RTC clock source for the real-time
 *  counter. 
 */
#define	EE_MCU_CLK_SRCS_FLAG_RTC_ENABLE		0x00010000U

/** @brief	1kHz from 32kHz internal ULP oscillator. */
#define	EE_MCU_CLK_SRCS_FLAG_RTC_ULP		0x00000000U

/** @brief	1.024kHz from 32.768kHz crystal oscillator on TOSC. */
#define	EE_MCU_CLK_SRCS_FLAG_RTC_TOSC		0x00020000U

/** @brief	1.024kHz from 32.768kHz internal oscillator. */
#define	EE_MCU_CLK_SRCS_FLAG_RTC_RCOSC		0x00040000U

/** @brief	32.768kHz from 32.768kHz crystal oscillator on TOSC. */
#define	EE_MCU_CLK_SRCS_FLAG_RTC_TOSC32		0x000A0000U

/** @brief	32.768kHz from 32.768kHz internal oscillator. */
#define	EE_MCU_CLK_SRCS_FLAG_RTC_RCOSC32	0x000C0000U

/** @brief	External clock from TOSC1. */
#define	EE_MCU_CLK_SRCS_FLAG_RTC_EXTCLK		0x000E0000U

/*
 * DFLL
 */

/** @brief	DFLL 2MHz Oscillator Calibration Reference.
 *
 *  This flag is used to select the calibration source for the 2MHz DFLL.
 *  By default, this flag is of and the 32.768kHz internal oscillator is
 *  selected.
 *
 *  If this flag is set to on, the 32.768kHz crystal oscillator on TOSC is
 *  selected as the reference.
 *
 *  This flag will select only which calibration source to use for the DFLL.
 *  In addition, the actual clock source that is selected must enabled and
 *  configured for the calibration to function.
 */
#define	EE_MCU_CLK_SRCS_FLAG_DFLL_RC2MCREF	0x00100000U

/** @brief	DFLL 32MHz Oscillator Calibration Reference.
 *
 *  This flag is used to select the calibration source for the 32MHz DFLL.
 *  By default, this flag is of and the 32.768kHz internal oscillator is
 *  selected.
 *
 *  If this flag is set to on, the 32.768kHz crystal oscillator on TOSC is
 *  selected as the reference.
 *
 *  This flag will select only which calibration source to use for the DFLL.
 *  In addition, the actual clock source that is selected must enabled and
 *  configured for the calibration to function.
 */
#define	EE_MCU_CLK_SRCS_FLAG_DFLL_RC32MCREF	0x00200000U

/** @brief	DFLL Enable.
 *
 *  Setting this flag enables the DFLL and auto-calibration of the internal
 *  oscillator.
 */
#define	EE_MCU_CLK_SRCS_FLAG_DFLL_ENABLE	0x00800000U

/*
 * PLL.
 */

/** @brief	PLL Multiplication Factor 0X.
 *
 *  These bits select the multiplication factor for the PLL.
 *  The multiplication factor can be in the range of from 1x to 31x. 
 *
 *  @note	NOT ALLOWED!!!
 */
#define	EE_MCU_CLK_SRCS_FLAG_PLL_0X		0x00000000U

/** @brief	PLL Multiplication Factor 1X. */
#define	EE_MCU_CLK_SRCS_FLAG_PLL_1X		0x01000000U

/** @brief	PLL Multiplication Factor 2X. */
#define	EE_MCU_CLK_SRCS_FLAG_PLL_2X		0x02000000U

/** @brief	PLL Multiplication Factor 3X. */
#define	EE_MCU_CLK_SRCS_FLAG_PLL_3X		0x03000000U

/** @brief	PLL Multiplication Factor 4X. */
#define	EE_MCU_CLK_SRCS_FLAG_PLL_4X		0x04000000U

/** @brief	PLL Multiplication Factor 5X. */
#define	EE_MCU_CLK_SRCS_FLAG_PLL_5X		0x05000000U

/** @brief	PLL Multiplication Factor 6X. */
#define	EE_MCU_CLK_SRCS_FLAG_PLL_6X		0x06000000U

/** @brief	PLL Multiplication Factor 7X. */
#define	EE_MCU_CLK_SRCS_FLAG_PLL_7X		0x07000000U

/** @brief	PLL Multiplication Factor 8X. */
#define	EE_MCU_CLK_SRCS_FLAG_PLL_8X		0x08000000U

/** @brief	PLL Multiplication Factor 9X. */
#define	EE_MCU_CLK_SRCS_FLAG_PLL_9X		0x09000000U

/** @brief	PLL Multiplication Factor 10X. */
#define	EE_MCU_CLK_SRCS_FLAG_PLL_10X		0x0A000000U

/** @brief	PLL Multiplication Factor 11X. */
#define	EE_MCU_CLK_SRCS_FLAG_PLL_11X		0x0B000000U

/** @brief	PLL Multiplication Factor 12X. */
#define	EE_MCU_CLK_SRCS_FLAG_PLL_12X		0x0C000000U

/** @brief	PLL Multiplication Factor 13X. */
#define	EE_MCU_CLK_SRCS_FLAG_PLL_13X		0x0D000000U

/** @brief	PLL Multiplication Factor 14X. */
#define	EE_MCU_CLK_SRCS_FLAG_PLL_14X		0x0E000000U

/** @brief	PLL Multiplication Factor 15X. */
#define	EE_MCU_CLK_SRCS_FLAG_PLL_15X		0x0F000000U

/** @brief	PLL Multiplication Factor 16X. */
#define	EE_MCU_CLK_SRCS_FLAG_PLL_16X		0x10000000U

/** @brief	PLL Multiplication Factor 17X. */
#define	EE_MCU_CLK_SRCS_FLAG_PLL_17X		0x11000000U

/** @brief	PLL Multiplication Factor 18X. */
#define	EE_MCU_CLK_SRCS_FLAG_PLL_18X		0x12000000U

/** @brief	PLL Multiplication Factor 19X. */
#define	EE_MCU_CLK_SRCS_FLAG_PLL_19X		0x13000000U

/** @brief	PLL Multiplication Factor 20X. */
#define	EE_MCU_CLK_SRCS_FLAG_PLL_20X		0x14000000U

/** @brief	PLL Multiplication Factor 21X. */
#define	EE_MCU_CLK_SRCS_FLAG_PLL_21X		0x15000000U

/** @brief	PLL Multiplication Factor 22X. */
#define	EE_MCU_CLK_SRCS_FLAG_PLL_22X		0x16000000U

/** @brief	PLL Multiplication Factor 23X. */
#define	EE_MCU_CLK_SRCS_FLAG_PLL_23X		0x17000000U

/** @brief	PLL Multiplication Factor 24X. */
#define	EE_MCU_CLK_SRCS_FLAG_PLL_24X		0x18000000U

/** @brief	PLL Multiplication Factor 25X. */
#define	EE_MCU_CLK_SRCS_FLAG_PLL_25X		0x19000000U

/** @brief	PLL Multiplication Factor 26X. */
#define	EE_MCU_CLK_SRCS_FLAG_PLL_26X		0x1A000000U

/** @brief	PLL Multiplication Factor 27X. */
#define	EE_MCU_CLK_SRCS_FLAG_PLL_27X		0x1B000000U

/** @brief	PLL Multiplication Factor 28X. */
#define	EE_MCU_CLK_SRCS_FLAG_PLL_28X		0x1C000000U

/** @brief	PLL Multiplication Factor 29X. */
#define	EE_MCU_CLK_SRCS_FLAG_PLL_29X		0x1D000000U

/** @brief	PLL Multiplication Factor 30X. */
#define	EE_MCU_CLK_SRCS_FLAG_PLL_30X		0x1E000000U

/** @brief	PLL Multiplication Factor 31X. */
#define	EE_MCU_CLK_SRCS_FLAG_PLL_31X		0x1F000000U

/** @brief	PLL Divided Output Enable
 *
 *  Setting this flag will divide the output from the PLL by 2.
 */
#define	EE_MCU_CLK_SRCS_FLAG_PLL_DOE		0x20000000U

/** @brief	PLL Clock Source 2MHz internal oscillator. */
#define	EE_MCU_CLK_SRCS_FLAG_PLL_RC2M		0x00000000U

/** @brief	PLL Clock Source 32MHz internal oscillator. */
#define	EE_MCU_CLK_SRCS_FLAG_PLL_RC32M		0x80000000U

/** @brief	PLL Clock Source external oscillator.
 *
 *  @note	The 32.768kHz TOSC cannot be selected as the source for the PLL.
 *  		An external clock must be a minimum 0.4MHz to be used as the
 *  		source clock.
 */
#define	EE_MCU_CLK_SRCS_FLAG_PLL_XOSC		0xC0000000U

/*****************************************************************************
 Clock Multiplexer.
 ******************************************************************************/

/** @brief	2MHz internal oscillator.	 */
#define EE_MCU_CLK_MUX_FLAG_RC2MHZ	EE_MCU_CLK_MUX_FLAG_DEFAULT

/** @brief	32MHz internal oscillator.	 */
#define EE_MCU_CLK_MUX_FLAG_RC32MHZ	0x01U

/** @brief	32.768kHz internal oscillator.	 */
#define EE_MCU_CLK_MUX_FLAG_RC32KHZ	0x02U

/** @brief	External oscillator or clock.	 */
#define EE_MCU_CLK_MUX_FLAG_XOSC	0x03U

/** @brief	Phase locked loop.		 */
#define EE_MCU_CLK_MUX_FLAG_PLL		0x04U

/*****************************************************************************
 Clock Divisor.
 ******************************************************************************/

/** @brief	Prescaler B No division and Prescaler C No division. */
#define	EE_MCU_CLK_DIV_FLAG_B_1_C_1	EE_MCU_CLK_MUX_FLAG_DEFAULT

/** @brief	Prescaler B No division and Prescaler C Divide by 2. */
#define	EE_MCU_CLK_DIV_FLAG_B_1_C_2	0x01U

/** @brief	Prescaler B Divide by 4 and Prescaler C No division. */
#define	EE_MCU_CLK_DIV_FLAG_B_4_C_1	0x02U

/** @brief	Prescaler B Divide by 2 and Prescaler C Divide by 2. */
#define	EE_MCU_CLK_DIV_FLAG_B_2_C_2	0x03U

/** @brief	Prescaler A No division. */
#define	EE_MCU_CLK_DIV_FLAG_A_1		EE_MCU_CLK_MUX_FLAG_DEFAULT

/** @brief	Prescaler A Divide by 2. */
#define	EE_MCU_CLK_DIV_FLAG_A_2		0x04U

/** @brief	Prescaler A Divide by 4. */
#define	EE_MCU_CLK_DIV_FLAG_A_4		0x0CU

/** @brief	Prescaler A Divide by 8. */
#define	EE_MCU_CLK_DIV_FLAG_A_8		0x14U

/** @brief	Prescaler A Divide by 16. */
#define	EE_MCU_CLK_DIV_FLAG_A_16	0x1CU

/** @brief	Prescaler A Divide by 32. */
#define	EE_MCU_CLK_DIV_FLAG_A_32	0x24U

/** @brief	Prescaler A Divide by 64. */
#define	EE_MCU_CLK_DIV_FLAG_A_64	0x2CU

/** @brief	Prescaler A Divide by 128. */
#define	EE_MCU_CLK_DIV_FLAG_A_128	0x34U

/** @brief	Prescaler A Divide by 256. */
#define	EE_MCU_CLK_DIV_FLAG_A_256	0x3CU

/** @brief	Prescaler A Divide by 512. */
#define	EE_MCU_CLK_DIV_FLAG_A_512	0x44U

/*****************************************************************************
 Operational Mode.
 ******************************************************************************/

/** @brief	IDLE Mode.
 *
 *  In idle mode the CPU and non-volatile memory are stopped (note that any
 *  ongoing programming will be completed), but all peripherals, including the
 *  interrupt controller and event system are kept running.
 *
 *  Any enabled interrupt will wake the device.
 */
#define	EE_MCU_MODE_FLAG_IDLE	0x01U

/** @brief	Power-Down Mode.
 *
 *  In power-down mode, all clocks, including the real-time counter clock
 *  source, are stopped. This allows operation only of asynchronous modules that
 *  do not require a running clock.
 *
 *  The only interrupts that can wake up the MCU are the two-wire interface
 *  address match interrupt and asynchronous port interrupts.
 */
#define	EE_MCU_MODE_FLAG_PDOWN	0x05U

/** @brief	Power-Save Mode.
 *
 *  Power-save mode is identical to power down, with one exception. If the
 *  real-time counter (RTC) is enabled, it will keep running during sleep, and
 *  the device can also wake up from either an RTC overflow or compare match
 *  interrupt.
 */
#define	EE_MCU_MODE_FLAG_PSAVE	0x07U

/** @brief	Standby Mode.
 *
 *  Standby mode is identical to power down, with the exception that the enabled
 *  system clock sources are kept running while the CPU, peripheral, and RTC
 *  clocks are stopped. This reduces the wake-up time.
 */
#define	EE_MCU_MODE_FLAG_STDBY	0x0DU

/** @brief	Extended Standby Mode.
 *
 *  Extended standby mode is identical to power-save mode, with the exception
 *  that the enabled system clock sources are kept running while the CPU and
 *  peripheral clocks are stopped. This reduces the wake-up time.
 */
#define	EE_MCU_MODE_FLAG_ESTDBY	0x0FU

/*****************************************************************************
 Reset Flags.
 ******************************************************************************/

/** @brief	Power-On Reset.
 *
 *  This flag is set if a power-on reset occurs.
 */
#define	EE_MCU_RESET_FLAG_POR	0x01U

/** @brief	External Reset.
 *
 *  This flag is set if an external reset occurs.
 *
 *  @note	The flag will be cleared by Power-On Reset.
 */
#define	EE_MCU_RESET_FLAG_EXT	0x02U

/** @brief	Brownout Reset.
 *
 *  This flag is set if a brownout reset occurs.
 *
 *  @note	The flag will be cleared by Power-On Reset.
 */
#define	EE_MCU_RESET_FLAG_BOR	0x04U

/** @brief	Watchdog Reset.
 *
 *  This flag is set if a watchdog reset occurs.
 *
 *  @note	The flag will be cleared by Power-On Reset.
 */
#define	EE_MCU_RESET_FLAG_WDR	0x08U

/** @brief	Program and Debug Interface Reset.
 *
 *  This flag is set if a programming interface reset occurs.
 *
 *  @note	The flag will be cleared by Power-On Reset.
 */
#define	EE_MCU_RESET_FLAG_PDIR	0x10U

/** @brief	Software Reset Reset.
 *
 *  This flag is set if a software reset occurs.
 *
 *  @note	The flag will be cleared by Power-On Reset.
 */
#define	EE_MCU_RESET_FLAG_SR	0x20U

/*****************************************************************************
 Common Types.
 ******************************************************************************/
 
#include "mcu/common/inc/ee_mcu_common_types.h"

/*****************************************************************************
 Common APIs.
 ******************************************************************************/

#include "mcu/common/inc/ee_mcu_common_mcu.h"

/*****************************************************************************
 Peripheral Drivers.
 ******************************************************************************/

#ifdef	__E2P_DRIVER__
#include "mcu/atmel_atxmega/inc/ee_atmel_atxmega_e2p.h"
#endif

#ifdef	__GPIO_DRIVER__
#include "mcu/atmel_atxmega/inc/ee_atmel_atxmega_gpio.h"
#endif

#ifdef	__GPT_DRIVER__
#include "mcu/atmel_atxmega/inc/ee_atmel_atxmega_gpt.h"
#endif

/*
#ifdef	__I2C_DRIVER__
#include "mcu/atmel_atxmega/inc/ee_atmel_atxmega_i2c.h"
#endif
*/

#ifdef	__PWM_DRIVER__
#include "mcu/atmel_atxmega/inc/ee_atmel_atxmega_pwm.h"
#endif

#ifdef	__RTC_DRIVER__
#include "mcu/atmel_atxmega/inc/ee_atmel_atxmega_rtc.h"
#endif

#ifdef	__SCI_DRIVER__
#include "mcu/atmel_atxmega/inc/ee_atmel_atxmega_sci.h"
#endif

#endif	/* __INCLUDE_ATMEL_ATXMEGA_MCU_H__ */
