/* ###*B*###
 * ERIKA Enterprise - a tiny RTOS for small microcontrollers
 *
 * Copyright (C) 2002-2013  Evidence Srl
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
 * Erika Enterprise - PWM Driver - Atmel ATXMEGA MCUs Implementation File.
 *
 * Author: 2013,  Giuseppe Serano
 */

/* ERIKA Enterprise Internals */
#include "ee_internal.h"

/* ERIKA Enterprise IRQs */
#include "ee_irq.h"

#ifdef	__AVR_PGMSPACE__
#include <avr/pgmspace.h>
#endif	/* __AVR_PGMSPACE__ */

#ifdef	__USE_TC_HW__
/* Timer(s)/Counter(s) of Atmel ATXMEGA MCUs. */
#include "mcu/atmel_atxmega/inc/ee_atmel_atxmega_tc_hw.h"
#else	/* __USE_TC_HW__ */
/* Timer(s)/Counter(s) Type 0 (TCX0) of Atmel ATXMEGA MCUs. */
#include "mcu/atmel_atxmega/inc/ee_atmel_atxmega_tcx0.h"
#endif	/* __USE_TC_HW__ */

#define	EE_PWM_FLAG_ALL_MASK	(			\
	EE_PWM_FLAG_POL_HIGH | EE_PWM_FLAG_IDLE_HIGH |	\
	EE_PWM_FLAG_PER_VAR | EE_PWM_FLAG_PER_IMM_UP |	\
	EE_PWM_FLAG_DUTY_IMM_UP				\
)

#define	EE_PWM_FP_MASK		0x40U	/* Fixed Period Mask.		      */
#define	EE_PWM_DEU_MASK		0x20U	/* Duty-Cycle End Period Update Mask. */
#define	EE_PWM_PEU_MASK		0x10U	/* Period End Period Update Mask.     */
#define	EE_PWM_IH_MASK		0x08U	/* Idle State High Mask.	      */
#define	EE_PWM_PH_MASK		0x08U	/* Polarity High Mask.		      */

#define	EE_PWM_ABS_PERIOD_MIN	0x0003U	/* Minimum Absolute PWM Period. */
#define	EE_PWM_ABS_PERIOD_MAX	0xFFFEU	/* Maximum Absolute PWM Period. */

#ifdef	__USE_TC_HW__

/*
 * Timer Counter 0 Compare or Capture Enabling and Counter Divisors.
 */
#ifdef	DEBUG
static volatile const EE_UREG
#else
static const EE_UREG
#endif
#ifdef	__AVR_PGMSPACE__
EE_Pwm_0_Ccx_Div[EE_TC_0_HW_UNITS_NUMBER] PROGMEM = {
#else	/* __AVR_PGMSPACE__ */
EE_Pwm_0_Ccx_Div[EE_TC_0_HW_UNITS_NUMBER] = {
#endif	/* __AVR_PGMSPACE__ */

#if	( defined(__AVR_ATxmega16D4__) || defined(__AVR_ATxmega32D4__) )

/* CHANNEL C 0 */
#ifdef	__PWM_CHANNEL_C_0__
#if	defined(__PWM_C_0_CCA__)
#if	defined(__PWM_CHANNEL_C_0_DIV_2__)
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV2_gc,
#elif	defined(__PWM_CHANNEL_C_0_DIV_4__)
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV4_gc,
#elif	defined(__PWM_CHANNEL_C_0_DIV_8__)
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV8_gc,
#elif	defined(__PWM_CHANNEL_C_0_DIV_64__)
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV64_gc,
#elif	defined(__PWM_CHANNEL_C_0_DIV_256__)
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV256_gc,
#elif	defined(__PWM_CHANNEL_C_0_DIV_1024__)
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV1024_gc,
#else
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV1_gc,
#endif
#elif	defined(__PWM_C_0_CCB__)
#if	defined(__PWM_CHANNEL_C_0_DIV_2__)
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV2_gc,
#elif	defined(__PWM_CHANNEL_C_0_DIV_4__)
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV4_gc,
#elif	defined(__PWM_CHANNEL_C_0_DIV_8__)
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV8_gc,
#elif	defined(__PWM_CHANNEL_C_0_DIV_64__)
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV64_gc,
#elif	defined(__PWM_CHANNEL_C_0_DIV_256__)
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV256_gc,
#elif	defined(__PWM_CHANNEL_C_0_DIV_1024__)
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV1024_gc,
#else
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV1_gc,
#endif
#elif	defined(__PWM_C_0_CCC__)
#if	defined(__PWM_CHANNEL_C_0_DIV_2__)
EE_TC_HW_CCC_ENABLE | TC_CLKSEL_DIV2_gc,
#elif	defined(__PWM_CHANNEL_C_0_DIV_4__)
EE_TC_HW_CCC_ENABLE | TC_CLKSEL_DIV4_gc,
#elif	defined(__PWM_CHANNEL_C_0_DIV_8__)
EE_TC_HW_CCC_ENABLE | TC_CLKSEL_DIV8_gc,
#elif	defined(__PWM_CHANNEL_C_0_DIV_64__)
EE_TC_HW_CCC_ENABLE | TC_CLKSEL_DIV64_gc,
#elif	defined(__PWM_CHANNEL_C_0_DIV_256__)
EE_TC_HW_CCC_ENABLE | TC_CLKSEL_DIV256_gc,
#elif	defined(__PWM_CHANNEL_C_0_DIV_1024__)
EE_TC_HW_CCC_ENABLE | TC_CLKSEL_DIV1024_gc,
#else
EE_TC_HW_CCC_ENABLE | TC_CLKSEL_DIV1_gc,
#endif
#elif	defined(__PWM_C_0_CCD__)
#if	defined(__PWM_CHANNEL_C_0_DIV_2__)
EE_TC_HW_CCD_ENABLE | TC_CLKSEL_DIV2_gc,
#elif	defined(__PWM_CHANNEL_C_0_DIV_4__)
EE_TC_HW_CCD_ENABLE | TC_CLKSEL_DIV4_gc,
#elif	defined(__PWM_CHANNEL_C_0_DIV_8__)
EE_TC_HW_CCD_ENABLE | TC_CLKSEL_DIV8_gc,
#elif	defined(__PWM_CHANNEL_C_0_DIV_64__)
EE_TC_HW_CCD_ENABLE | TC_CLKSEL_DIV64_gc,
#elif	defined(__PWM_CHANNEL_C_0_DIV_256__)
EE_TC_HW_CCD_ENABLE | TC_CLKSEL_DIV256_gc,
#elif	defined(__PWM_CHANNEL_C_0_DIV_1024__)
EE_TC_HW_CCD_ENABLE | TC_CLKSEL_DIV1024_gc,
#else
EE_TC_HW_CCD_ENABLE | TC_CLKSEL_DIV1_gc,
#endif
#else
#if	defined(__PWM_CHANNEL_C_0_DIV_2__)
TC_CLKSEL_DIV2_gc,
#elif	defined(__PWM_CHANNEL_C_0_DIV_4__)
TC_CLKSEL_DIV4_gc,
#elif	defined(__PWM_CHANNEL_C_0_DIV_8__)
TC_CLKSEL_DIV8_gc,
#elif	defined(__PWM_CHANNEL_C_0_DIV_64__)
TC_CLKSEL_DIV64_gc,
#elif	defined(__PWM_CHANNEL_C_0_DIV_256__)
TC_CLKSEL_DIV256_gc,
#elif	defined(__PWM_CHANNEL_C_0_DIV_1024__)
TC_CLKSEL_DIV1024_gc,
#else
TC_CLKSEL_DIV1_gc,
#endif
#endif
#else	/* __PWM_CHANNEL_C_0__ */
TC_CLKSEL_OFF_gc,
#endif	/* __PWM_CHANNEL_C_0__ */

/* CHANNEL D 0 */
#ifdef	__PWM_CHANNEL_D_0__
#if	defined(__PWM_D_0_CCA__)
#if	defined(__PWM_CHANNEL_D_0_DIV_2__)
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV2_gc,
#elif	defined(__PWM_CHANNEL_D_0_DIV_4__)
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV4_gc,
  #elif	defined(__PWM_CHANNEL_D_0_DIV_8__)
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV8_gc,
#elif	defined(__PWM_CHANNEL_D_0_DIV_64__)
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV64_gc,
#elif	defined(__PWM_CHANNEL_D_0_DIV_256__)
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV256_gc,
#elif	defined(__PWM_CHANNEL_D_0_DIV_1024__)
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV1024_gc,
#else
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV1_gc,
#endif
#elif	defined(__PWM_D_0_CCB__)
#if	defined(__PWM_CHANNEL_D_0_DIV_2__)
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV2_gc,
#elif	defined(__PWM_CHANNEL_D_0_DIV_4__)
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV4_gc,
  #elif	defined(__PWM_CHANNEL_D_0_DIV_8__)
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV8_gc,
#elif	defined(__PWM_CHANNEL_D_0_DIV_64__)
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV64_gc,
#elif	defined(__PWM_CHANNEL_D_0_DIV_256__)
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV256_gc,
#elif	defined(__PWM_CHANNEL_D_0_DIV_1024__)
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV1024_gc,
#else
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV1_gc,
#endif
#elif	defined(__PWM_D_0_CCC__)
#if	defined(__PWM_CHANNEL_D_0_DIV_2__)
EE_TC_HW_CCC_ENABLE | TC_CLKSEL_DIV2_gc,
#elif	defined(__PWM_CHANNEL_D_0_DIV_4__)
EE_TC_HW_CCC_ENABLE | TC_CLKSEL_DIV4_gc,
  #elif	defined(__PWM_CHANNEL_D_0_DIV_8__)
EE_TC_HW_CCC_ENABLE | TC_CLKSEL_DIV8_gc,
#elif	defined(__PWM_CHANNEL_D_0_DIV_64__)
EE_TC_HW_CCC_ENABLE | TC_CLKSEL_DIV64_gc,
#elif	defined(__PWM_CHANNEL_D_0_DIV_256__)
EE_TC_HW_CCC_ENABLE | TC_CLKSEL_DIV256_gc,
#elif	defined(__PWM_CHANNEL_D_0_DIV_1024__)
EE_TC_HW_CCC_ENABLE | TC_CLKSEL_DIV1024_gc,
#else
EE_TC_HW_CCC_ENABLE | TC_CLKSEL_DIV1_gc,
#endif
#elif	defined(__PWM_D_0_CCD__)
#if	defined(__PWM_CHANNEL_D_0_DIV_2__)
EE_TC_HW_CCD_ENABLE | TC_CLKSEL_DIV2_gc,
#elif	defined(__PWM_CHANNEL_D_0_DIV_4__)
EE_TC_HW_CCD_ENABLE | TC_CLKSEL_DIV4_gc,
#elif	defined(__PWM_CHANNEL_D_0_DIV_8__)
EE_TC_HW_CCD_ENABLE | TC_CLKSEL_DIV8_gc,
#elif	defined(__PWM_CHANNEL_D_0_DIV_64__)
EE_TC_HW_CCD_ENABLE | TC_CLKSEL_DIV64_gc,
#elif	defined(__PWM_CHANNEL_D_0_DIV_256__)
EE_TC_HW_CCD_ENABLE | TC_CLKSEL_DIV256_gc,
#elif	defined(__PWM_CHANNEL_D_0_DIV_1024__)
EE_TC_HW_CCD_ENABLE | TC_CLKSEL_DIV1024_gc,
#else
EE_TC_HW_CCD_ENABLE | TC_CLKSEL_DIV1_gc,
#endif
#else
#if	defined(__PWM_CHANNEL_D_0_DIV_2__)
TC_CLKSEL_DIV2_gc,
#elif	defined(__PWM_CHANNEL_D_0_DIV_4__)
TC_CLKSEL_DIV4_gc,
#elif	defined(__PWM_CHANNEL_D_0_DIV_8__)
TC_CLKSEL_DIV8_gc,
#elif	defined(__PWM_CHANNEL_D_0_DIV_64__)
TC_CLKSEL_DIV64_gc,
#elif	defined(__PWM_CHANNEL_D_0_DIV_256__)
TC_CLKSEL_DIV256_gc,
#elif	defined(__PWM_CHANNEL_D_0_DIV_1024__)
TC_CLKSEL_DIV1024_gc,
#else
TC_CLKSEL_DIV1_gc,
#endif
#endif
#else	/* __PWM_CHANNEL_D_0__ */
TC_CLKSEL_OFF_gc,
#endif	/* __PWM_CHANNEL_D_0__ */

/* CHANNEL E 0 */
#ifdef	__PWM_CHANNEL_E_0__
#if	defined(__PWM_E_0_CCA__)
#if	defined(__PWM_CHANNEL_E_0_DIV_2__)
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV2_gc,
#elif	defined(__PWM_CHANNEL_E_0_DIV_4__)
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV4_gc,
#elif	defined(__PWM_CHANNEL_E_0_DIV_8__)
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV8_gc,
#elif	defined(__PWM_CHANNEL_E_0_DIV_64__)
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV64_gc,
#elif	defined(__PWM_CHANNEL_E_0_DIV_256__)
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV256_gc,
#elif	defined(__PWM_CHANNEL_E_0_DIV_1024__)
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV1024_gc,
#else
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV1_gc,
#endif
#elif	defined(__PWM_E_0_CCB__)
#if	defined(__PWM_CHANNEL_E_0_DIV_2__)
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV2_gc,
#elif	defined(__PWM_CHANNEL_E_0_DIV_4__)
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV4_gc,
#elif	defined(__PWM_CHANNEL_E_0_DIV_8__)
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV8_gc,
#elif	defined(__PWM_CHANNEL_E_0_DIV_64__)
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV64_gc,
#elif	defined(__PWM_CHANNEL_E_0_DIV_256__)
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV256_gc,
#elif	defined(__PWM_CHANNEL_E_0_DIV_1024__)
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV1024_gc,
#else
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV1_gc,
#endif
#elif	defined(__PWM_E_0_CCC__)
#if	defined(__PWM_CHANNEL_E_0_DIV_2__)
EE_TC_HW_CCC_ENABLE | TC_CLKSEL_DIV2_gc,
#elif	defined(__PWM_CHANNEL_E_0_DIV_4__)
EE_TC_HW_CCC_ENABLE | TC_CLKSEL_DIV4_gc,
#elif	defined(__PWM_CHANNEL_E_0_DIV_8__)
EE_TC_HW_CCC_ENABLE | TC_CLKSEL_DIV8_gc,
#elif	defined(__PWM_CHANNEL_E_0_DIV_64__)
EE_TC_HW_CCC_ENABLE | TC_CLKSEL_DIV64_gc,
#elif	defined(__PWM_CHANNEL_E_0_DIV_256__)
EE_TC_HW_CCC_ENABLE | TC_CLKSEL_DIV256_gc,
#elif	defined(__PWM_CHANNEL_E_0_DIV_1024__)
EE_TC_HW_CCC_ENABLE | TC_CLKSEL_DIV1024_gc,
#else
EE_TC_HW_CCC_ENABLE | TC_CLKSEL_DIV1_gc,
#endif
#elif	defined(__PWM_E_0_CCD__)
#if	defined(__PWM_CHANNEL_E_0_DIV_2__)
EE_TC_HW_CCD_ENABLE | TC_CLKSEL_DIV2_gc,
#elif	defined(__PWM_CHANNEL_E_0_DIV_4__)
EE_TC_HW_CCD_ENABLE | TC_CLKSEL_DIV4_gc,
#elif	defined(__PWM_CHANNEL_E_0_DIV_8__)
EE_TC_HW_CCD_ENABLE | TC_CLKSEL_DIV8_gc,
#elif	defined(__PWM_CHANNEL_E_0_DIV_64__)
EE_TC_HW_CCD_ENABLE | TC_CLKSEL_DIV64_gc,
#elif	defined(__PWM_CHANNEL_E_0_DIV_256__)
EE_TC_HW_CCD_ENABLE | TC_CLKSEL_DIV256_gc,
#elif	defined(__PWM_CHANNEL_E_0_DIV_1024__)
EE_TC_HW_CCD_ENABLE | TC_CLKSEL_DIV1024_gc,
#else
EE_TC_HW_CCD_ENABLE | TC_CLKSEL_DIV1_gc,
#endif
#else
#if	defined(__PWM_CHANNEL_E_0_DIV_2__)
TC_CLKSEL_DIV2_gc,
#elif	defined(__PWM_CHANNEL_E_0_DIV_4__)
TC_CLKSEL_DIV4_gc,
#elif	defined(__PWM_CHANNEL_E_0_DIV_8__)
TC_CLKSEL_DIV8_gc,
#elif	defined(__PWM_CHANNEL_E_0_DIV_64__)
TC_CLKSEL_DIV64_gc,
#elif	defined(__PWM_CHANNEL_E_0_DIV_256__)
TC_CLKSEL_DIV256_gc,
#elif	defined(__PWM_CHANNEL_E_0_DIV_1024__)
TC_CLKSEL_DIV1024_gc,
#else
TC_CLKSEL_DIV1_gc,
#endif
#endif
#else	/* __PWM_CHANNEL_E_0__ */
TC_CLKSEL_OFF_gc,
#endif	/* __PWM_CHANNEL_E_0__ */

#else	/* __AVR_ATxmega16D4__ || __AVR_ATxmega32D4__ */

/* CHANNEL C 0 */
#ifdef	__PWM_CHANNEL_C_0__
#if	defined(__PWM_C_0_CCA__)
#if	defined(__PWM_CHANNEL_C_0_DIV_2__)
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV2_gc,
#elif	defined(__PWM_CHANNEL_C_0_DIV_4__)
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV4_gc,
#elif	defined(__PWM_CHANNEL_C_0_DIV_8__)
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV8_gc,
#elif	defined(__PWM_CHANNEL_C_0_DIV_64__)
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV64_gc,
#elif	defined(__PWM_CHANNEL_C_0_DIV_256__)
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV256_gc,
#elif	defined(__PWM_CHANNEL_C_0_DIV_1024__)
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV1024_gc,
#else
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV1_gc,
#endif
#elif	defined(__PWM_C_0_CCB__)
#if	defined(__PWM_CHANNEL_C_0_DIV_2__)
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV2_gc,
#elif	defined(__PWM_CHANNEL_C_0_DIV_4__)
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV4_gc,
#elif	defined(__PWM_CHANNEL_C_0_DIV_8__)
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV8_gc,
#elif	defined(__PWM_CHANNEL_C_0_DIV_64__)
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV64_gc,
#elif	defined(__PWM_CHANNEL_C_0_DIV_256__)
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV256_gc,
#elif	defined(__PWM_CHANNEL_C_0_DIV_1024__)
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV1024_gc,
#else
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV1_gc,
#endif
#elif	defined(__PWM_C_0_CCC__)
#if	defined(__PWM_CHANNEL_C_0_DIV_2__)
EE_TC_HW_CCC_ENABLE | TC_CLKSEL_DIV2_gc,
#elif	defined(__PWM_CHANNEL_C_0_DIV_4__)
EE_TC_HW_CCC_ENABLE | TC_CLKSEL_DIV4_gc,
#elif	defined(__PWM_CHANNEL_C_0_DIV_8__)
EE_TC_HW_CCC_ENABLE | TC_CLKSEL_DIV8_gc,
#elif	defined(__PWM_CHANNEL_C_0_DIV_64__)
EE_TC_HW_CCC_ENABLE | TC_CLKSEL_DIV64_gc,
#elif	defined(__PWM_CHANNEL_C_0_DIV_256__)
EE_TC_HW_CCC_ENABLE | TC_CLKSEL_DIV256_gc,
#elif	defined(__PWM_CHANNEL_C_0_DIV_1024__)
EE_TC_HW_CCC_ENABLE | TC_CLKSEL_DIV1024_gc,
#else
EE_TC_HW_CCC_ENABLE | TC_CLKSEL_DIV1_gc,
#endif
#elif	defined(__PWM_C_0_CCD__)
#if	defined(__PWM_CHANNEL_C_0_DIV_2__)
EE_TC_HW_CCD_ENABLE | TC_CLKSEL_DIV2_gc,
#elif	defined(__PWM_CHANNEL_C_0_DIV_4__)
EE_TC_HW_CCD_ENABLE | TC_CLKSEL_DIV4_gc,
#elif	defined(__PWM_CHANNEL_C_0_DIV_8__)
EE_TC_HW_CCD_ENABLE | TC_CLKSEL_DIV8_gc,
#elif	defined(__PWM_CHANNEL_C_0_DIV_64__)
EE_TC_HW_CCD_ENABLE | TC_CLKSEL_DIV64_gc,
#elif	defined(__PWM_CHANNEL_C_0_DIV_256__)
EE_TC_HW_CCD_ENABLE | TC_CLKSEL_DIV256_gc,
#elif	defined(__PWM_CHANNEL_C_0_DIV_1024__)
EE_TC_HW_CCD_ENABLE | TC_CLKSEL_DIV1024_gc,
#else
EE_TC_HW_CCD_ENABLE | TC_CLKSEL_DIV1_gc,
#endif
#else
#if	defined(__PWM_CHANNEL_C_0_DIV_2__)
TC_CLKSEL_DIV2_gc,
#elif	defined(__PWM_CHANNEL_C_0_DIV_4__)
TC_CLKSEL_DIV4_gc,
#elif	defined(__PWM_CHANNEL_C_0_DIV_8__)
TC_CLKSEL_DIV8_gc,
#elif	defined(__PWM_CHANNEL_C_0_DIV_64__)
TC_CLKSEL_DIV64_gc,
#elif	defined(__PWM_CHANNEL_C_0_DIV_256__)
TC_CLKSEL_DIV256_gc,
#elif	defined(__PWM_CHANNEL_C_0_DIV_1024__)
TC_CLKSEL_DIV1024_gc,
#else
TC_CLKSEL_DIV1_gc,
#endif
#endif
#else	/* __PWM_CHANNEL_C_0__ */
TC_CLKSEL_OFF_gc,
#endif	/* __PWM_CHANNEL_C_0__ */

/* CHANNEL D 0 */
#ifdef	__PWM_CHANNEL_D_0__
#if	defined(__PWM_D_0_CCA__)
#if	defined(__PWM_CHANNEL_D_0_DIV_2__)
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV2_gc,
#elif	defined(__PWM_CHANNEL_D_0_DIV_4__)
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV4_gc,
  #elif	defined(__PWM_CHANNEL_D_0_DIV_8__)
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV8_gc,
#elif	defined(__PWM_CHANNEL_D_0_DIV_64__)
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV64_gc,
#elif	defined(__PWM_CHANNEL_D_0_DIV_256__)
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV256_gc,
#elif	defined(__PWM_CHANNEL_D_0_DIV_1024__)
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV1024_gc,
#else
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV1_gc,
#endif
#elif	defined(__PWM_D_0_CCB__)
#if	defined(__PWM_CHANNEL_D_0_DIV_2__)
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV2_gc,
#elif	defined(__PWM_CHANNEL_D_0_DIV_4__)
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV4_gc,
  #elif	defined(__PWM_CHANNEL_D_0_DIV_8__)
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV8_gc,
#elif	defined(__PWM_CHANNEL_D_0_DIV_64__)
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV64_gc,
#elif	defined(__PWM_CHANNEL_D_0_DIV_256__)
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV256_gc,
#elif	defined(__PWM_CHANNEL_D_0_DIV_1024__)
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV1024_gc,
#else
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV1_gc,
#endif
#elif	defined(__PWM_D_0_CCC__)
#if	defined(__PWM_CHANNEL_D_0_DIV_2__)
EE_TC_HW_CCC_ENABLE | TC_CLKSEL_DIV2_gc,
#elif	defined(__PWM_CHANNEL_D_0_DIV_4__)
EE_TC_HW_CCC_ENABLE | TC_CLKSEL_DIV4_gc,
  #elif	defined(__PWM_CHANNEL_D_0_DIV_8__)
EE_TC_HW_CCC_ENABLE | TC_CLKSEL_DIV8_gc,
#elif	defined(__PWM_CHANNEL_D_0_DIV_64__)
EE_TC_HW_CCC_ENABLE | TC_CLKSEL_DIV64_gc,
#elif	defined(__PWM_CHANNEL_D_0_DIV_256__)
EE_TC_HW_CCC_ENABLE | TC_CLKSEL_DIV256_gc,
#elif	defined(__PWM_CHANNEL_D_0_DIV_1024__)
EE_TC_HW_CCC_ENABLE | TC_CLKSEL_DIV1024_gc,
#else
EE_TC_HW_CCC_ENABLE | TC_CLKSEL_DIV1_gc,
#endif
#elif	defined(__PWM_D_0_CCD__)
#if	defined(__PWM_CHANNEL_D_0_DIV_2__)
EE_TC_HW_CCD_ENABLE | TC_CLKSEL_DIV2_gc,
#elif	defined(__PWM_CHANNEL_D_0_DIV_4__)
EE_TC_HW_CCD_ENABLE | TC_CLKSEL_DIV4_gc,
#elif	defined(__PWM_CHANNEL_D_0_DIV_8__)
EE_TC_HW_CCD_ENABLE | TC_CLKSEL_DIV8_gc,
#elif	defined(__PWM_CHANNEL_D_0_DIV_64__)
EE_TC_HW_CCD_ENABLE | TC_CLKSEL_DIV64_gc,
#elif	defined(__PWM_CHANNEL_D_0_DIV_256__)
EE_TC_HW_CCD_ENABLE | TC_CLKSEL_DIV256_gc,
#elif	defined(__PWM_CHANNEL_D_0_DIV_1024__)
EE_TC_HW_CCD_ENABLE | TC_CLKSEL_DIV1024_gc,
#else
EE_TC_HW_CCD_ENABLE | TC_CLKSEL_DIV1_gc,
#endif
#else
#if	defined(__PWM_CHANNEL_D_0_DIV_2__)
TC_CLKSEL_DIV2_gc,
#elif	defined(__PWM_CHANNEL_D_0_DIV_4__)
TC_CLKSEL_DIV4_gc,
#elif	defined(__PWM_CHANNEL_D_0_DIV_8__)
TC_CLKSEL_DIV8_gc,
#elif	defined(__PWM_CHANNEL_D_0_DIV_64__)
TC_CLKSEL_DIV64_gc,
#elif	defined(__PWM_CHANNEL_D_0_DIV_256__)
TC_CLKSEL_DIV256_gc,
#elif	defined(__PWM_CHANNEL_D_0_DIV_1024__)
TC_CLKSEL_DIV1024_gc,
#else
TC_CLKSEL_DIV1_gc,
#endif
#endif
#else	/* __PWM_CHANNEL_D_0__ */
TC_CLKSEL_OFF_gc,
#endif	/* __PWM_CHANNEL_D_0__ */

/* CHANNEL E 0 */
#ifdef	__PWM_CHANNEL_E_0__
#if	defined(__PWM_E_0_CCA__)
#if	defined(__PWM_CHANNEL_E_0_DIV_2__)
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV2_gc,
#elif	defined(__PWM_CHANNEL_E_0_DIV_4__)
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV4_gc,
#elif	defined(__PWM_CHANNEL_E_0_DIV_8__)
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV8_gc,
#elif	defined(__PWM_CHANNEL_E_0_DIV_64__)
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV64_gc,
#elif	defined(__PWM_CHANNEL_E_0_DIV_256__)
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV256_gc,
#elif	defined(__PWM_CHANNEL_E_0_DIV_1024__)
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV1024_gc,
#else
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV1_gc,
#endif
#elif	defined(__PWM_E_0_CCB__)
#if	defined(__PWM_CHANNEL_E_0_DIV_2__)
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV2_gc,
#elif	defined(__PWM_CHANNEL_E_0_DIV_4__)
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV4_gc,
#elif	defined(__PWM_CHANNEL_E_0_DIV_8__)
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV8_gc,
#elif	defined(__PWM_CHANNEL_E_0_DIV_64__)
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV64_gc,
#elif	defined(__PWM_CHANNEL_E_0_DIV_256__)
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV256_gc,
#elif	defined(__PWM_CHANNEL_E_0_DIV_1024__)
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV1024_gc,
#else
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV1_gc,
#endif
#elif	defined(__PWM_E_0_CCC__)
#if	defined(__PWM_CHANNEL_E_0_DIV_2__)
EE_TC_HW_CCC_ENABLE | TC_CLKSEL_DIV2_gc,
#elif	defined(__PWM_CHANNEL_E_0_DIV_4__)
EE_TC_HW_CCC_ENABLE | TC_CLKSEL_DIV4_gc,
#elif	defined(__PWM_CHANNEL_E_0_DIV_8__)
EE_TC_HW_CCC_ENABLE | TC_CLKSEL_DIV8_gc,
#elif	defined(__PWM_CHANNEL_E_0_DIV_64__)
EE_TC_HW_CCC_ENABLE | TC_CLKSEL_DIV64_gc,
#elif	defined(__PWM_CHANNEL_E_0_DIV_256__)
EE_TC_HW_CCC_ENABLE | TC_CLKSEL_DIV256_gc,
#elif	defined(__PWM_CHANNEL_E_0_DIV_1024__)
EE_TC_HW_CCC_ENABLE | TC_CLKSEL_DIV1024_gc,
#else
EE_TC_HW_CCC_ENABLE | TC_CLKSEL_DIV1_gc,
#endif
#elif	defined(__PWM_E_0_CCD__)
#if	defined(__PWM_CHANNEL_E_0_DIV_2__)
EE_TC_HW_CCD_ENABLE | TC_CLKSEL_DIV2_gc,
#elif	defined(__PWM_CHANNEL_E_0_DIV_4__)
EE_TC_HW_CCD_ENABLE | TC_CLKSEL_DIV4_gc,
#elif	defined(__PWM_CHANNEL_E_0_DIV_8__)
EE_TC_HW_CCD_ENABLE | TC_CLKSEL_DIV8_gc,
#elif	defined(__PWM_CHANNEL_E_0_DIV_64__)
EE_TC_HW_CCD_ENABLE | TC_CLKSEL_DIV64_gc,
#elif	defined(__PWM_CHANNEL_E_0_DIV_256__)
EE_TC_HW_CCD_ENABLE | TC_CLKSEL_DIV256_gc,
#elif	defined(__PWM_CHANNEL_E_0_DIV_1024__)
EE_TC_HW_CCD_ENABLE | TC_CLKSEL_DIV1024_gc,
#else
EE_TC_HW_CCD_ENABLE | TC_CLKSEL_DIV1_gc,
#endif
#else
#if	defined(__PWM_CHANNEL_E_0_DIV_2__)
TC_CLKSEL_DIV2_gc,
#elif	defined(__PWM_CHANNEL_E_0_DIV_4__)
TC_CLKSEL_DIV4_gc,
#elif	defined(__PWM_CHANNEL_E_0_DIV_8__)
TC_CLKSEL_DIV8_gc,
#elif	defined(__PWM_CHANNEL_E_0_DIV_64__)
TC_CLKSEL_DIV64_gc,
#elif	defined(__PWM_CHANNEL_E_0_DIV_256__)
TC_CLKSEL_DIV256_gc,
#elif	defined(__PWM_CHANNEL_E_0_DIV_1024__)
TC_CLKSEL_DIV1024_gc,
#else
TC_CLKSEL_DIV1_gc,
#endif
#endif
#else	/* __PWM_CHANNEL_E_0__ */
TC_CLKSEL_OFF_gc,
#endif	/* __PWM_CHANNEL_E_0__ */

/* CHANNEL F 0 */
#ifdef	__PWM_CHANNEL_F_0__
#if	defined(__PWM_F_0_CCA__)
#if	defined(__PWM_CHANNEL_F_0_DIV_2__)
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV2_gc,
#elif	defined(__PWM_CHANNEL_F_0_DIV_4__)
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV4_gc,
#elif	defined(__PWM_CHANNEL_F_0_DIV_8__)
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV8_gc,
#elif	defined(__PWM_CHANNEL_F_0_DIV_64__)
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV64_gc,
#elif	defined(__PWM_CHANNEL_F_0_DIV_256__)
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV256_gc,
#elif	defined(__PWM_CHANNEL_F_0_DIV_1024__)
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV1024_gc,
#else
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV1_gc,
#endif
#elif	defined(__PWM_F_0_CCB__)
#if	defined(__PWM_CHANNEL_F_0_DIV_2__)
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV2_gc,
#elif	defined(__PWM_CHANNEL_F_0_DIV_4__)
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV4_gc,
#elif	defined(__PWM_CHANNEL_F_0_DIV_8__)
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV8_gc,
#elif	defined(__PWM_CHANNEL_F_0_DIV_64__)
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV64_gc,
#elif	defined(__PWM_CHANNEL_F_0_DIV_256__)
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV256_gc,
#elif	defined(__PWM_CHANNEL_F_0_DIV_1024__)
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV1024_gc,
#else
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV1_gc,
#endif
#elif	defined(__PWM_F_0_CCC__)
#if	defined(__PWM_CHANNEL_F_0_DIV_2__)
EE_TC_HW_CCC_ENABLE | TC_CLKSEL_DIV2_gc,
#elif	defined(__PWM_CHANNEL_F_0_DIV_4__)
EE_TC_HW_CCC_ENABLE | TC_CLKSEL_DIV4_gc,
#elif	defined(__PWM_CHANNEL_F_0_DIV_8__)
EE_TC_HW_CCC_ENABLE | TC_CLKSEL_DIV8_gc,
#elif	defined(__PWM_CHANNEL_F_0_DIV_64__)
EE_TC_HW_CCC_ENABLE | TC_CLKSEL_DIV64_gc,
#elif	defined(__PWM_CHANNEL_F_0_DIV_256__)
EE_TC_HW_CCC_ENABLE | TC_CLKSEL_DIV256_gc,
#elif	defined(__PWM_CHANNEL_F_0_DIV_1024__)
EE_TC_HW_CCC_ENABLE | TC_CLKSEL_DIV1024_gc,
#else
EE_TC_HW_CCC_ENABLE | TC_CLKSEL_DIV1_gc,
#endif
#elif	defined(__PWM_F_0_CCD__)
#if	defined(__PWM_CHANNEL_F_0_DIV_2__)
EE_TC_HW_CCD_ENABLE | TC_CLKSEL_DIV2_gc,
#elif	defined(__PWM_CHANNEL_F_0_DIV_4__)
EE_TC_HW_CCD_ENABLE | TC_CLKSEL_DIV4_gc,
#elif	defined(__PWM_CHANNEL_F_0_DIV_8__)
EE_TC_HW_CCD_ENABLE | TC_CLKSEL_DIV8_gc,
#elif	defined(__PWM_CHANNEL_F_0_DIV_64__)
EE_TC_HW_CCD_ENABLE | TC_CLKSEL_DIV64_gc,
#elif	defined(__PWM_CHANNEL_F_0_DIV_256__)
EE_TC_HW_CCD_ENABLE | TC_CLKSEL_DIV256_gc,
#elif	defined(__PWM_CHANNEL_F_0_DIV_1024__)
EE_TC_HW_CCD_ENABLE | TC_CLKSEL_DIV1024_gc,
#else
EE_TC_HW_CCD_ENABLE | TC_CLKSEL_DIV1_gc,
#endif
#else
#if	defined(__PWM_CHANNEL_F_0_DIV_2__)
TC_CLKSEL_DIV2_gc,
#elif	defined(__PWM_CHANNEL_F_0_DIV_4__)
TC_CLKSEL_DIV4_gc,
#elif	defined(__PWM_CHANNEL_F_0_DIV_8__)
TC_CLKSEL_DIV8_gc,
#elif	defined(__PWM_CHANNEL_F_0_DIV_64__)
TC_CLKSEL_DIV64_gc,
#elif	defined(__PWM_CHANNEL_F_0_DIV_256__)
TC_CLKSEL_DIV256_gc,
#elif	defined(__PWM_CHANNEL_F_0_DIV_1024__)
TC_CLKSEL_DIV1024_gc,
#else
TC_CLKSEL_DIV1_gc,
#endif
#endif
#else	/* __PWM_CHANNEL_F_0__ */
TC_CLKSEL_OFF_gc,
#endif	/* __PWM_CHANNEL_F_0__ */

#endif	/* __AVR_ATxmega16D4__ || __AVR_ATxmega32D4__ */

};	/* EE_Pwm_0_Ccx_Div[EE_TC_0_HW_UNITS_NUMBER] */

/*
 * Timer Counter 1 Compare or Capture Enabling and Counter Divisors.
 */
#ifdef	DEBUG
static volatile const EE_UREG
#else
static const EE_UREG
#endif
#ifdef	__AVR_PGMSPACE__
EE_Pwm_1_Ccx_Div[EE_TC_1_HW_UNITS_NUMBER] PROGMEM = {
#else	/* __AVR_PGMSPACE__ */
EE_Pwm_1_Ccx_Div[EE_TC_1_HW_UNITS_NUMBER] = {
#endif	/* __AVR_PGMSPACE__ */

#if	( defined(__AVR_ATxmega16D4__) || defined(__AVR_ATxmega32D4__) )

/* CHANNEL C 1 */
#ifdef	__PWM_CHANNEL_C_1__
#if	defined(__PWM_C_1_CCA__)
#if	defined(__PWM_CHANNEL_C_1_DIV_2__)
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV2_gc,
#elif	defined(__PWM_CHANNEL_C_1_DIV_4__)
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV4_gc,
#elif	defined(__PWM_CHANNEL_C_1_DIV_8__)
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV8_gc,
#elif	defined(__PWM_CHANNEL_C_1_DIV_64__)
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV64_gc,
#elif	defined(__PWM_CHANNEL_C_1_DIV_256__)
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV256_gc,
#elif	defined(__PWM_CHANNEL_C_1_DIV_1024__)
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV1024_gc,
#else
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV1_gc,
#endif
#elif	defined(__PWM_C_1_CCB__)
#if	defined(__PWM_CHANNEL_C_1_DIV_2__)
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV2_gc,
#elif	defined(__PWM_CHANNEL_C_1_DIV_4__)
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV4_gc,
#elif	defined(__PWM_CHANNEL_C_1_DIV_8__)
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV8_gc,
#elif	defined(__PWM_CHANNEL_C_1_DIV_64__)
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV64_gc,
#elif	defined(__PWM_CHANNEL_C_1_DIV_256__)
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV256_gc,
#elif	defined(__PWM_CHANNEL_C_1_DIV_1024__)
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV1024_gc,
#else
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV1_gc,
#endif
#else
#if	defined(__PWM_CHANNEL_C_1_DIV_2__)
TC_CLKSEL_DIV2_gc,
#elif	defined(__PWM_CHANNEL_C_1_DIV_4__)
TC_CLKSEL_DIV4_gc,
#elif	defined(__PWM_CHANNEL_C_1_DIV_8__)
TC_CLKSEL_DIV8_gc,
#elif	defined(__PWM_CHANNEL_C_1_DIV_64__)
TC_CLKSEL_DIV64_gc,
#elif	defined(__PWM_CHANNEL_C_1_DIV_256__)
TC_CLKSEL_DIV256_gc,
#elif	defined(__PWM_CHANNEL_C_1_DIV_1024__)
TC_CLKSEL_DIV1024_gc,
#else
TC_CLKSEL_DIV1_gc,
#endif
#endif
#else	/* __PWM_CHANNEL_C_1__ */
TC_CLKSEL_OFF_gc,
#endif	/* __PWM_CHANNEL_C_1__ */

#else	/* __AVR_ATxmega16D4__ || __AVR_ATxmega32D4__ */

/* CHANNEL C 1 */
#ifdef	__PWM_CHANNEL_C_1__
#if	defined(__PWM_C_1_CCA__)
#if	defined(__PWM_CHANNEL_C_1_DIV_2__)
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV2_gc,
#elif	defined(__PWM_CHANNEL_C_1_DIV_4__)
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV4_gc,
#elif	defined(__PWM_CHANNEL_C_1_DIV_8__)
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV8_gc,
#elif	defined(__PWM_CHANNEL_C_1_DIV_64__)
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV64_gc,
#elif	defined(__PWM_CHANNEL_C_1_DIV_256__)
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV256_gc,
#elif	defined(__PWM_CHANNEL_C_1_DIV_1024__)
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV1024_gc,
#else
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV1_gc,
#endif
#elif	defined(__PWM_C_1_CCB__)
#if	defined(__PWM_CHANNEL_C_1_DIV_2__)
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV2_gc,
#elif	defined(__PWM_CHANNEL_C_1_DIV_4__)
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV4_gc,
#elif	defined(__PWM_CHANNEL_C_1_DIV_8__)
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV8_gc,
#elif	defined(__PWM_CHANNEL_C_1_DIV_64__)
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV64_gc,
#elif	defined(__PWM_CHANNEL_C_1_DIV_256__)
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV256_gc,
#elif	defined(__PWM_CHANNEL_C_1_DIV_1024__)
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV1024_gc,
#else
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV1_gc,
#endif
#else
#if	defined(__PWM_CHANNEL_C_1_DIV_2__)
TC_CLKSEL_DIV2_gc,
#elif	defined(__PWM_CHANNEL_C_1_DIV_4__)
TC_CLKSEL_DIV4_gc,
#elif	defined(__PWM_CHANNEL_C_1_DIV_8__)
TC_CLKSEL_DIV8_gc,
#elif	defined(__PWM_CHANNEL_C_1_DIV_64__)
TC_CLKSEL_DIV64_gc,
#elif	defined(__PWM_CHANNEL_C_1_DIV_256__)
TC_CLKSEL_DIV256_gc,
#elif	defined(__PWM_CHANNEL_C_1_DIV_1024__)
TC_CLKSEL_DIV1024_gc,
#else
TC_CLKSEL_DIV1_gc,
#endif
#endif
#else	/* __PWM_CHANNEL_C_1__ */
TC_CLKSEL_OFF_gc,
#endif	/* __PWM_CHANNEL_C_1__ */

/* CHANNEL D 1 */
#ifdef	__PWM_CHANNEL_D_1__
#if	defined(__PWM_D_1_CCA__)
#if	defined(__PWM_CHANNEL_D_1_DIV_2__)
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV2_gc,
#elif	defined(__PWM_CHANNEL_D_1_DIV_4__)
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV4_gc,
  #elif	defined(__PWM_CHANNEL_D_1_DIV_8__)
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV8_gc,
#elif	defined(__PWM_CHANNEL_D_1_DIV_64__)
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV64_gc,
#elif	defined(__PWM_CHANNEL_D_1_DIV_256__)
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV256_gc,
#elif	defined(__PWM_CHANNEL_D_1_DIV_1024__)
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV1024_gc,
#else
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV1_gc,
#endif
#elif	defined(__PWM_D_1_CCB__)
#if	defined(__PWM_CHANNEL_D_1_DIV_2__)
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV2_gc,
#elif	defined(__PWM_CHANNEL_D_1_DIV_4__)
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV4_gc,
  #elif	defined(__PWM_CHANNEL_D_1_DIV_8__)
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV8_gc,
#elif	defined(__PWM_CHANNEL_D_1_DIV_64__)
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV64_gc,
#elif	defined(__PWM_CHANNEL_D_1_DIV_256__)
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV256_gc,
#elif	defined(__PWM_CHANNEL_D_1_DIV_1024__)
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV1024_gc,
#else
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV1_gc,
#endif
#else
#if	defined(__PWM_CHANNEL_D_1_DIV_2__)
TC_CLKSEL_DIV2_gc,
#elif	defined(__PWM_CHANNEL_D_1_DIV_4__)
TC_CLKSEL_DIV4_gc,
#elif	defined(__PWM_CHANNEL_D_1_DIV_8__)
TC_CLKSEL_DIV8_gc,
#elif	defined(__PWM_CHANNEL_D_1_DIV_64__)
TC_CLKSEL_DIV64_gc,
#elif	defined(__PWM_CHANNEL_D_1_DIV_256__)
TC_CLKSEL_DIV256_gc,
#elif	defined(__PWM_CHANNEL_D_1_DIV_1024__)
TC_CLKSEL_DIV1024_gc,
#else
TC_CLKSEL_DIV1_gc,
#endif
#endif
#else	/* __PWM_CHANNEL_D_1__ */
TC_CLKSEL_OFF_gc,
#endif	/* __PWM_CHANNEL_D_1__ */

/* CHANNEL E 1 */
#ifdef	__PWM_CHANNEL_E_1__
#if	defined(__PWM_E_1_CCA__)
#if	defined(__PWM_CHANNEL_E_1_DIV_2__)
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV2_gc,
#elif	defined(__PWM_CHANNEL_E_1_DIV_4__)
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV4_gc,
#elif	defined(__PWM_CHANNEL_E_1_DIV_8__)
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV8_gc,
#elif	defined(__PWM_CHANNEL_E_1_DIV_64__)
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV64_gc,
#elif	defined(__PWM_CHANNEL_E_1_DIV_256__)
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV256_gc,
#elif	defined(__PWM_CHANNEL_E_1_DIV_1024__)
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV1024_gc,
#else
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV1_gc,
#endif
#elif	defined(__PWM_E_1_CCB__)
#if	defined(__PWM_CHANNEL_E_1_DIV_2__)
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV2_gc,
#elif	defined(__PWM_CHANNEL_E_1_DIV_4__)
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV4_gc,
#elif	defined(__PWM_CHANNEL_E_1_DIV_8__)
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV8_gc,
#elif	defined(__PWM_CHANNEL_E_1_DIV_64__)
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV64_gc,
#elif	defined(__PWM_CHANNEL_E_1_DIV_256__)
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV256_gc,
#elif	defined(__PWM_CHANNEL_E_1_DIV_1024__)
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV1024_gc,
#else
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV1_gc,
#endif
#else
#if	defined(__PWM_CHANNEL_E_1_DIV_2__)
TC_CLKSEL_DIV2_gc,
#elif	defined(__PWM_CHANNEL_E_1_DIV_4__)
TC_CLKSEL_DIV4_gc,
#elif	defined(__PWM_CHANNEL_E_1_DIV_8__)
TC_CLKSEL_DIV8_gc,
#elif	defined(__PWM_CHANNEL_E_1_DIV_64__)
TC_CLKSEL_DIV64_gc,
#elif	defined(__PWM_CHANNEL_E_1_DIV_256__)
TC_CLKSEL_DIV256_gc,
#elif	defined(__PWM_CHANNEL_E_1_DIV_1024__)
TC_CLKSEL_DIV1024_gc,
#else
TC_CLKSEL_DIV1_gc,
#endif
#endif
#else	/* __PWM_CHANNEL_E_1__ */
TC_CLKSEL_OFF_gc,
#endif	/* __PWM_CHANNEL_E_1__ */

/* CHANNEL F 1 */
#ifdef	__PWM_CHANNEL_F_1__
#if	defined(__PWM_F_1_CCA__)
#if	defined(__PWM_CHANNEL_F_1_DIV_2__)
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV2_gc,
#elif	defined(__PWM_CHANNEL_F_1_DIV_4__)
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV4_gc,
#elif	defined(__PWM_CHANNEL_F_1_DIV_8__)
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV8_gc,
#elif	defined(__PWM_CHANNEL_F_1_DIV_64__)
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV64_gc,
#elif	defined(__PWM_CHANNEL_F_1_DIV_256__)
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV256_gc,
#elif	defined(__PWM_CHANNEL_F_1_DIV_1024__)
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV1024_gc,
#else
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV1_gc,
#endif
#elif	defined(__PWM_F_1_CCB__)
#if	defined(__PWM_CHANNEL_F_1_DIV_2__)
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV2_gc,
#elif	defined(__PWM_CHANNEL_F_1_DIV_4__)
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV4_gc,
#elif	defined(__PWM_CHANNEL_F_1_DIV_8__)
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV8_gc,
#elif	defined(__PWM_CHANNEL_F_1_DIV_64__)
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV64_gc,
#elif	defined(__PWM_CHANNEL_F_1_DIV_256__)
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV256_gc,
#elif	defined(__PWM_CHANNEL_F_1_DIV_1024__)
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV1024_gc,
#else
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV1_gc,
#endif
#else
#if	defined(__PWM_CHANNEL_F_1_DIV_2__)
TC_CLKSEL_DIV2_gc,
#elif	defined(__PWM_CHANNEL_F_1_DIV_4__)
TC_CLKSEL_DIV4_gc,
#elif	defined(__PWM_CHANNEL_F_1_DIV_8__)
TC_CLKSEL_DIV8_gc,
#elif	defined(__PWM_CHANNEL_F_1_DIV_64__)
TC_CLKSEL_DIV64_gc,
#elif	defined(__PWM_CHANNEL_F_1_DIV_256__)
TC_CLKSEL_DIV256_gc,
#elif	defined(__PWM_CHANNEL_F_1_DIV_1024__)
TC_CLKSEL_DIV1024_gc,
#else
TC_CLKSEL_DIV1_gc,
#endif
#endif
#else	/* __PWM_CHANNEL_F_1__ */
TC_CLKSEL_OFF_gc,
#endif	/* __PWM_CHANNEL_F_1__ */

#endif	/* __AVR_ATxmega16D4__ || __AVR_ATxmega32D4__ */

};	/* EE_Pwm_1_Ccx_Div[EE_TC_1_HW_UNITS_NUMBER] */

/*
 * PWM Prescaler Retrieval.
 */
__INLINE__ EE_UREG __ALWAYS_INLINE__
EE_Pwm_Ccx_GetDiv(
  EE_TYPEPWMCHANNEL	Channel
) {

#if	DEBUG
  volatile EE_UREG	Div;
#else	/* DEBUG */
  register EE_UREG	Div;
#endif	/* DEBUG */

  if ( Channel & EE_TC_HW_ALL_TCX1_MASK ) {

#ifdef	__AVR_PGMSPACE__
    Div = (EE_UREG) pgm_read_byte(
      &EE_Pwm_1_Ccx_Div[(Channel & ~EE_TC_HW_ALL_TCX1_MASK)]
    );
#else	/* __AVR_PGMSPACE__ */
    Div = EE_Pwm_1_Ccx_Div[(Channel & ~EE_TC_HW_ALL_TCX1_MASK)];
#endif	/* __AVR_PGMSPACE__ */

  }
  else {

#ifdef	__AVR_PGMSPACE__
    Div = (EE_UREG) pgm_read_byte(&EE_Pwm_0_Ccx_Div[Channel]);
#else	/* __AVR_PGMSPACE__ */
    Div = EE_Pwm_0_Ccx_Div[Channel];
#endif	/* __AVR_PGMSPACE__ */

  }

  return Div;

}

#else	/* __USE_TC_HW__ */

/*
 * Timer Counter Compare or Capture Enabling and Counter Divisors.
 */
#ifdef	DEBUG
static volatile const EE_UREG
#else
static const EE_UREG
#endif
#ifdef	__AVR_PGMSPACE__
EE_Pwm_Ccx_Div[EE_TC_HW_UNITS_NUMBER] PROGMEM = {
#else	/* __AVR_PGMSPACE__ */
EE_Pwm_Ccx_Div[EE_TC_HW_UNITS_NUMBER] = {
#endif	/* __AVR_PGMSPACE__ */

/* CHANNEL C 0 */
#ifdef	__PWM_CHANNEL_C_0__
#if	defined(__PWM_C_0_CCA__)
#if	defined(__PWM_CHANNEL_C_0_DIV_2__)
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV2_gc,
#elif	defined(__PWM_CHANNEL_C_0_DIV_4__)
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV4_gc,
#elif	defined(__PWM_CHANNEL_C_0_DIV_8__)
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV8_gc,
#elif	defined(__PWM_CHANNEL_C_0_DIV_64__)
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV64_gc,
#elif	defined(__PWM_CHANNEL_C_0_DIV_256__)
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV256_gc,
#elif	defined(__PWM_CHANNEL_C_0_DIV_1024__)
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV1024_gc,
#else
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV1_gc,
#endif
#elif	defined(__PWM_C_0_CCB__)
#if	defined(__PWM_CHANNEL_C_0_DIV_2__)
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV2_gc,
#elif	defined(__PWM_CHANNEL_C_0_DIV_4__)
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV4_gc,
#elif	defined(__PWM_CHANNEL_C_0_DIV_8__)
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV8_gc,
#elif	defined(__PWM_CHANNEL_C_0_DIV_64__)
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV64_gc,
#elif	defined(__PWM_CHANNEL_C_0_DIV_256__)
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV256_gc,
#elif	defined(__PWM_CHANNEL_C_0_DIV_1024__)
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV1024_gc,
#else
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV1_gc,
#endif
#elif	defined(__PWM_C_0_CCC__)
#if	defined(__PWM_CHANNEL_C_0_DIV_2__)
EE_TC_HW_CCC_ENABLE | TC_CLKSEL_DIV2_gc,
#elif	defined(__PWM_CHANNEL_C_0_DIV_4__)
EE_TC_HW_CCC_ENABLE | TC_CLKSEL_DIV4_gc,
#elif	defined(__PWM_CHANNEL_C_0_DIV_8__)
EE_TC_HW_CCC_ENABLE | TC_CLKSEL_DIV8_gc,
#elif	defined(__PWM_CHANNEL_C_0_DIV_64__)
EE_TC_HW_CCC_ENABLE | TC_CLKSEL_DIV64_gc,
#elif	defined(__PWM_CHANNEL_C_0_DIV_256__)
EE_TC_HW_CCC_ENABLE | TC_CLKSEL_DIV256_gc,
#elif	defined(__PWM_CHANNEL_C_0_DIV_1024__)
EE_TC_HW_CCC_ENABLE | TC_CLKSEL_DIV1024_gc,
#else
EE_TC_HW_CCC_ENABLE | TC_CLKSEL_DIV1_gc,
#endif
#elif	defined(__PWM_C_0_CCD__)
#if	defined(__PWM_CHANNEL_C_0_DIV_2__)
EE_TC_HW_CCD_ENABLE | TC_CLKSEL_DIV2_gc,
#elif	defined(__PWM_CHANNEL_C_0_DIV_4__)
EE_TC_HW_CCD_ENABLE | TC_CLKSEL_DIV4_gc,
#elif	defined(__PWM_CHANNEL_C_0_DIV_8__)
EE_TC_HW_CCD_ENABLE | TC_CLKSEL_DIV8_gc,
#elif	defined(__PWM_CHANNEL_C_0_DIV_64__)
EE_TC_HW_CCD_ENABLE | TC_CLKSEL_DIV64_gc,
#elif	defined(__PWM_CHANNEL_C_0_DIV_256__)
EE_TC_HW_CCD_ENABLE | TC_CLKSEL_DIV256_gc,
#elif	defined(__PWM_CHANNEL_C_0_DIV_1024__)
EE_TC_HW_CCD_ENABLE | TC_CLKSEL_DIV1024_gc,
#else
EE_TC_HW_CCD_ENABLE | TC_CLKSEL_DIV1_gc,
#endif
#else
#if	defined(__PWM_CHANNEL_C_0_DIV_2__)
TC_CLKSEL_DIV2_gc,
#elif	defined(__PWM_CHANNEL_C_0_DIV_4__)
TC_CLKSEL_DIV4_gc,
#elif	defined(__PWM_CHANNEL_C_0_DIV_8__)
TC_CLKSEL_DIV8_gc,
#elif	defined(__PWM_CHANNEL_C_0_DIV_64__)
TC_CLKSEL_DIV64_gc,
#elif	defined(__PWM_CHANNEL_C_0_DIV_256__)
TC_CLKSEL_DIV256_gc,
#elif	defined(__PWM_CHANNEL_C_0_DIV_1024__)
TC_CLKSEL_DIV1024_gc,
#else
TC_CLKSEL_DIV1_gc,
#endif
#endif
#else	/* __PWM_CHANNEL_C_0__ */
TC_CLKSEL_OFF_gc,
#endif	/* __PWM_CHANNEL_C_0__ */

/* CHANNEL D 0 */
#ifdef	__PWM_CHANNEL_D_0__
#if	defined(__PWM_D_0_CCA__)
#if	defined(__PWM_CHANNEL_D_0_DIV_2__)
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV2_gc,
#elif	defined(__PWM_CHANNEL_D_0_DIV_4__)
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV4_gc,
  #elif	defined(__PWM_CHANNEL_D_0_DIV_8__)
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV8_gc,
#elif	defined(__PWM_CHANNEL_D_0_DIV_64__)
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV64_gc,
#elif	defined(__PWM_CHANNEL_D_0_DIV_256__)
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV256_gc,
#elif	defined(__PWM_CHANNEL_D_0_DIV_1024__)
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV1024_gc,
#else
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV1_gc,
#endif
#elif	defined(__PWM_D_0_CCB__)
#if	defined(__PWM_CHANNEL_D_0_DIV_2__)
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV2_gc,
#elif	defined(__PWM_CHANNEL_D_0_DIV_4__)
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV4_gc,
  #elif	defined(__PWM_CHANNEL_D_0_DIV_8__)
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV8_gc,
#elif	defined(__PWM_CHANNEL_D_0_DIV_64__)
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV64_gc,
#elif	defined(__PWM_CHANNEL_D_0_DIV_256__)
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV256_gc,
#elif	defined(__PWM_CHANNEL_D_0_DIV_1024__)
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV1024_gc,
#else
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV1_gc,
#endif
#elif	defined(__PWM_D_0_CCC__)
#if	defined(__PWM_CHANNEL_D_0_DIV_2__)
EE_TC_HW_CCC_ENABLE | TC_CLKSEL_DIV2_gc,
#elif	defined(__PWM_CHANNEL_D_0_DIV_4__)
EE_TC_HW_CCC_ENABLE | TC_CLKSEL_DIV4_gc,
  #elif	defined(__PWM_CHANNEL_D_0_DIV_8__)
EE_TC_HW_CCC_ENABLE | TC_CLKSEL_DIV8_gc,
#elif	defined(__PWM_CHANNEL_D_0_DIV_64__)
EE_TC_HW_CCC_ENABLE | TC_CLKSEL_DIV64_gc,
#elif	defined(__PWM_CHANNEL_D_0_DIV_256__)
EE_TC_HW_CCC_ENABLE | TC_CLKSEL_DIV256_gc,
#elif	defined(__PWM_CHANNEL_D_0_DIV_1024__)
EE_TC_HW_CCC_ENABLE | TC_CLKSEL_DIV1024_gc,
#else
EE_TC_HW_CCC_ENABLE | TC_CLKSEL_DIV1_gc,
#endif
#elif	defined(__PWM_D_0_CCD__)
#if	defined(__PWM_CHANNEL_D_0_DIV_2__)
EE_TC_HW_CCD_ENABLE | TC_CLKSEL_DIV2_gc,
#elif	defined(__PWM_CHANNEL_D_0_DIV_4__)
EE_TC_HW_CCD_ENABLE | TC_CLKSEL_DIV4_gc,
#elif	defined(__PWM_CHANNEL_D_0_DIV_8__)
EE_TC_HW_CCD_ENABLE | TC_CLKSEL_DIV8_gc,
#elif	defined(__PWM_CHANNEL_D_0_DIV_64__)
EE_TC_HW_CCD_ENABLE | TC_CLKSEL_DIV64_gc,
#elif	defined(__PWM_CHANNEL_D_0_DIV_256__)
EE_TC_HW_CCD_ENABLE | TC_CLKSEL_DIV256_gc,
#elif	defined(__PWM_CHANNEL_D_0_DIV_1024__)
EE_TC_HW_CCD_ENABLE | TC_CLKSEL_DIV1024_gc,
#else
EE_TC_HW_CCD_ENABLE | TC_CLKSEL_DIV1_gc,
#endif
#else
#if	defined(__PWM_CHANNEL_D_0_DIV_2__)
TC_CLKSEL_DIV2_gc,
#elif	defined(__PWM_CHANNEL_D_0_DIV_4__)
TC_CLKSEL_DIV4_gc,
#elif	defined(__PWM_CHANNEL_D_0_DIV_8__)
TC_CLKSEL_DIV8_gc,
#elif	defined(__PWM_CHANNEL_D_0_DIV_64__)
TC_CLKSEL_DIV64_gc,
#elif	defined(__PWM_CHANNEL_D_0_DIV_256__)
TC_CLKSEL_DIV256_gc,
#elif	defined(__PWM_CHANNEL_D_0_DIV_1024__)
TC_CLKSEL_DIV1024_gc,
#else
TC_CLKSEL_DIV1_gc,
#endif
#endif
#else	/* __PWM_CHANNEL_D_0__ */
TC_CLKSEL_OFF_gc,
#endif	/* __PWM_CHANNEL_D_0__ */

/* CHANNEL E 0 */
#ifdef	__PWM_CHANNEL_E_0__
#if	defined(__PWM_E_0_CCA__)
#if	defined(__PWM_CHANNEL_E_0_DIV_2__)
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV2_gc,
#elif	defined(__PWM_CHANNEL_E_0_DIV_4__)
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV4_gc,
#elif	defined(__PWM_CHANNEL_E_0_DIV_8__)
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV8_gc,
#elif	defined(__PWM_CHANNEL_E_0_DIV_64__)
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV64_gc,
#elif	defined(__PWM_CHANNEL_E_0_DIV_256__)
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV256_gc,
#elif	defined(__PWM_CHANNEL_E_0_DIV_1024__)
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV1024_gc,
#else
EE_TC_HW_CCA_ENABLE | TC_CLKSEL_DIV1_gc,
#endif
#elif	defined(__PWM_E_0_CCB__)
#if	defined(__PWM_CHANNEL_E_0_DIV_2__)
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV2_gc,
#elif	defined(__PWM_CHANNEL_E_0_DIV_4__)
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV4_gc,
#elif	defined(__PWM_CHANNEL_E_0_DIV_8__)
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV8_gc,
#elif	defined(__PWM_CHANNEL_E_0_DIV_64__)
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV64_gc,
#elif	defined(__PWM_CHANNEL_E_0_DIV_256__)
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV256_gc,
#elif	defined(__PWM_CHANNEL_E_0_DIV_1024__)
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV1024_gc,
#else
EE_TC_HW_CCB_ENABLE | TC_CLKSEL_DIV1_gc,
#endif
#elif	defined(__PWM_E_0_CCC__)
#if	defined(__PWM_CHANNEL_E_0_DIV_2__)
EE_TC_HW_CCC_ENABLE | TC_CLKSEL_DIV2_gc,
#elif	defined(__PWM_CHANNEL_E_0_DIV_4__)
EE_TC_HW_CCC_ENABLE | TC_CLKSEL_DIV4_gc,
#elif	defined(__PWM_CHANNEL_E_0_DIV_8__)
EE_TC_HW_CCC_ENABLE | TC_CLKSEL_DIV8_gc,
#elif	defined(__PWM_CHANNEL_E_0_DIV_64__)
EE_TC_HW_CCC_ENABLE | TC_CLKSEL_DIV64_gc,
#elif	defined(__PWM_CHANNEL_E_0_DIV_256__)
EE_TC_HW_CCC_ENABLE | TC_CLKSEL_DIV256_gc,
#elif	defined(__PWM_CHANNEL_E_0_DIV_1024__)
EE_TC_HW_CCC_ENABLE | TC_CLKSEL_DIV1024_gc,
#else
EE_TC_HW_CCC_ENABLE | TC_CLKSEL_DIV1_gc,
#endif
#elif	defined(__PWM_E_0_CCD__)
#if	defined(__PWM_CHANNEL_E_0_DIV_2__)
EE_TC_HW_CCD_ENABLE | TC_CLKSEL_DIV2_gc,
#elif	defined(__PWM_CHANNEL_E_0_DIV_4__)
EE_TC_HW_CCD_ENABLE | TC_CLKSEL_DIV4_gc,
#elif	defined(__PWM_CHANNEL_E_0_DIV_8__)
EE_TC_HW_CCD_ENABLE | TC_CLKSEL_DIV8_gc,
#elif	defined(__PWM_CHANNEL_E_0_DIV_64__)
EE_TC_HW_CCD_ENABLE | TC_CLKSEL_DIV64_gc,
#elif	defined(__PWM_CHANNEL_E_0_DIV_256__)
EE_TC_HW_CCD_ENABLE | TC_CLKSEL_DIV256_gc,
#elif	defined(__PWM_CHANNEL_E_0_DIV_1024__)
EE_TC_HW_CCD_ENABLE | TC_CLKSEL_DIV1024_gc,
#else
EE_TC_HW_CCD_ENABLE | TC_CLKSEL_DIV1_gc,
#endif
#else
#if	defined(__PWM_CHANNEL_E_0_DIV_2__)
TC_CLKSEL_DIV2_gc,
#elif	defined(__PWM_CHANNEL_E_0_DIV_4__)
TC_CLKSEL_DIV4_gc,
#elif	defined(__PWM_CHANNEL_E_0_DIV_8__)
TC_CLKSEL_DIV8_gc,
#elif	defined(__PWM_CHANNEL_E_0_DIV_64__)
TC_CLKSEL_DIV64_gc,
#elif	defined(__PWM_CHANNEL_E_0_DIV_256__)
TC_CLKSEL_DIV256_gc,
#elif	defined(__PWM_CHANNEL_E_0_DIV_1024__)
TC_CLKSEL_DIV1024_gc,
#else
TC_CLKSEL_DIV1_gc,
#endif
#endif
#else	/* __PWM_CHANNEL_E_0__ */
TC_CLKSEL_OFF_gc,
#endif	/* __PWM_CHANNEL_E_0__ */

};	/* EE_Pwm_Ccx_Div[EE_TC_HW_UNITS_NUMBER] */

#endif	/* __USE_TC_HW__ */

#ifdef	__PWM_ENABLENOTIF_API__

#ifdef	__USE_TC_HW__

#if	( \
  ( \
    defined(__PWM_CHANNEL_C_0__) && \
    ( \
      defined(EE_AVR8_TCC0_OVF_ISR) ||	\
      ( defined(__PWM_C_0_CCA__) && defined(EE_AVR8_TCC0_CCA_ISR) ) || \
      ( defined(__PWM_C_0_CCB__) && defined(EE_AVR8_TCC0_CCB_ISR) ) || \
      ( defined(__PWM_C_0_CCC__) && defined(EE_AVR8_TCC0_CCC_ISR) ) || \
      ( defined(__PWM_C_0_CCD__) && defined(EE_AVR8_TCC0_CCD_ISR) ) \
    ) \
  ) || \
  ( \
    defined(__PWM_CHANNEL_D_0__) && \
    ( \
      defined(EE_AVR8_TCD0_OVF_ISR) || \
      ( defined(__PWM_D_0_CCA__) && defined(EE_AVR8_TCD0_CCA_ISR) ) || \
      ( defined(__PWM_D_0_CCB__) && defined(EE_AVR8_TCD0_CCB_ISR) ) || \
      ( defined(__PWM_D_0_CCC__) && defined(EE_AVR8_TCD0_CCC_ISR) ) || \
      ( defined(__PWM_D_0_CCD__) && defined(EE_AVR8_TCD0_CCD_ISR) ) \
    ) \
  ) || \
  ( \
    defined(__PWM_CHANNEL_E_0__) && \
    ( \
      defined(EE_AVR8_TCE0_OVF_ISR) || \
      ( defined(__PWM_E_0_CCA__) && defined(EE_AVR8_TCE0_CCA_ISR) ) || \
      ( defined(__PWM_E_0_CCB__) && defined(EE_AVR8_TCE0_CCB_ISR) ) || \
      ( defined(__PWM_E_0_CCC__) && defined(EE_AVR8_TCE0_CCC_ISR) ) || \
      ( defined(__PWM_E_0_CCD__) && defined(EE_AVR8_TCE0_CCD_ISR) ) \
    ) \
  ) || \
  ( \
    defined(__PWM_CHANNEL_F_0__) && \
    ( \
      defined(EE_AVR8_TCF0_OVF_ISR) || \
      ( defined(__PWM_F_0_CCA__) && defined(EE_AVR8_TCF0_CCA_ISR) ) || \
      ( defined(__PWM_F_0_CCB__) && defined(EE_AVR8_TCF0_CCB_ISR) ) || \
      ( defined(__PWM_F_0_CCC__) && defined(EE_AVR8_TCF0_CCC_ISR) ) || \
      ( defined(__PWM_F_0_CCD__) && defined(EE_AVR8_TCE0_CCD_ISR) ) \
    ) \
  ) || \
  ( \
    defined(__PWM_CHANNEL_C_1__) && \
    ( \
      defined(EE_AVR8_TCC1_OVF_ISR) ||	\
      ( defined(__PWM_C_1_CCA__) && defined(EE_AVR8_TCC1_CCA_ISR) ) || \
      ( defined(__PWM_C_1_CCB__) && defined(EE_AVR8_TCC1_CCB_ISR) ) \
    ) \
  ) || \
  ( \
    defined(__PWM_CHANNEL_D_1__) && \
    ( \
      defined(EE_AVR8_TCD1_OVF_ISR) || \
      ( defined(__PWM_D_1_CCA__) && defined(EE_AVR8_TCD1_CCA_ISR) ) || \
      ( defined(__PWM_D_1_CCB__) && defined(EE_AVR8_TCD1_CCB_ISR) ) \
    ) \
  ) || \
  ( \
    defined(__PWM_CHANNEL_E_1__) && \
    ( \
      defined(EE_AVR8_TCE1_OVF_ISR) || \
      ( defined(__PWM_E_1_CCA__) && defined(EE_AVR8_TCE1_CCA_ISR) ) || \
      ( defined(__PWM_E_1_CCB__) && defined(EE_AVR8_TCE1_CCB_ISR) ) \
    ) \
  ) || \
  ( \
    defined(__PWM_CHANNEL_F_1__) && \
    ( \
      defined(EE_AVR8_TCF1_OVF_ISR) || \
      ( defined(__PWM_F_1_CCA__) && defined(EE_AVR8_TCF1_CCA_ISR) ) || \
      ( defined(__PWM_F_1_CCB__) && defined(EE_AVR8_TCF1_CCB_ISR) ) \
    ) \
  ) \
)

/*
 * PWM 0 IRQ(s) Priority Array.
 */
#ifdef	DEBUG
volatile const EE_UREG
#else
const EE_UREG
#endif
#ifdef	__AVR_PGMSPACE__
EE_Pwm_0_Ccx_Pri[EE_TC_0_HW_UNITS_NUMBER] PROGMEM = {
#else	/* __AVR_PGMSPACE__ */
EE_Pwm_0_Ccx_Pri[EE_TC_0_HW_UNITS_NUMBER] = {
#endif	/* __AVR_PGMSPACE__ */

#if	( defined(__AVR_ATxmega16D4__) || defined(__AVR_ATxmega32D4__) )

/* CHANNEL C 0 */
#ifdef	__PWM_CHANNEL_C_0__
#if	defined(__PWM_C_0_CCA__)
#ifdef	EE_AVR8_TCC0_CCA_ISR_PRI
EE_TC_HW_IRQ_CCA_PRI(EE_AVR8_TCC0_CCA_ISR_PRI),
#else
EE_ISR_PRI_OFF,
#endif
#elif	defined(__PWM_C_0_CCB__)
#ifdef	EE_AVR8_TCC0_CCB_ISR_PRI
EE_TC_HW_IRQ_CCB_PRI(EE_AVR8_TCC0_CCB_ISR_PRI),
#else
EE_ISR_PRI_OFF,
#endif
#elif	defined(__PWM_C_0_CCC__)
#ifdef	EE_AVR8_TCC0_CCC_ISR_PRI
EE_TC_HW_IRQ_CCC_PRI(EE_AVR8_TCC0_CCC_ISR_PRI),
#else
EE_ISR_PRI_OFF,
#endif
#elif	defined(__PWM_C_0_CCD__)
#ifdef	EE_AVR8_TCC0_CCD_ISR_PRI
EE_TC_HW_IRQ_CCD_PRI(EE_AVR8_TCC0_CCD_ISR_PRI),
#else
EE_ISR_PRI_OFF,
#endif
#else
EE_ISR_PRI_OFF,
#endif
#else	/* __PWM_CHANNEL_C_0__ */
EE_ISR_PRI_OFF,
#endif	/* __PWM_CHANNEL_C_0__ */

/* CHANNEL D 0 */
#ifdef __PWM_CHANNEL_D_0__
#if	defined(__PWM_D_0_CCA__)
#ifdef	EE_AVR8_TCD0_CCA_ISR_PRI
EE_TC_HW_IRQ_CCA_PRI(EE_AVR8_TCD0_CCA_ISR_PRI),
#else
EE_ISR_PRI_OFF,
#endif
#elif	defined(__PWM_D_0_CCB__)
#ifdef	EE_AVR8_TCD0_CCB_ISR_PRI
EE_TC_HW_IRQ_CCB_PRI(EE_AVR8_TCD0_CCB_ISR_PRI),
#else
EE_ISR_PRI_OFF,
#endif
#elif	defined(__PWM_D_0_CCC__)
#ifdef	EE_AVR8_TCD0_CCC_ISR_PRI
EE_TC_HW_IRQ_CCC_PRI(EE_AVR8_TCD0_CCC_ISR_PRI),
#else
EE_ISR_PRI_OFF,
#endif
#elif	defined(__PWM_D_0_CCD__)
#ifdef	EE_AVR8_TCD0_CCD_ISR_PRI
EE_TC_HW_IRQ_CCD_PRI(EE_AVR8_TCD0_CCD_ISR_PRI),
#else
EE_ISR_PRI_OFF,
#endif
#else
EE_ISR_PRI_OFF,
#endif
#else	/* __PWM_CHANNEL_D_0__ */
EE_ISR_PRI_OFF,
#endif	/* __PWM_CHANNEL_D_0__ */

/* CHANNEL E 0 */
#ifdef __PWM_CHANNEL_E_0__
#if	defined(__PWM_E_0_CCA__)
#ifdef	EE_AVR8_TCE0_CCA_ISR_PRI
EE_TC_HW_IRQ_CCA_PRI(EE_AVR8_TCE0_CCA_ISR_PRI),
#else
EE_ISR_PRI_OFF,
#endif
#elif	defined(__PWM_E_0_CCB__)
#ifdef	EE_AVR8_TCE0_CCB_ISR_PRI
EE_TC_HW_IRQ_CCB_PRI(EE_AVR8_TCE0_CCB_ISR_PRI),
#else
EE_ISR_PRI_OFF,
#endif
#elif	defined(__PWM_E_0_CCC__)
#ifdef	EE_AVR8_TCE0_CCC_ISR_PRI
EE_TC_HW_IRQ_CCC_PRI(EE_AVR8_TCE0_CCC_ISR_PRI),
#else
EE_ISR_PRI_OFF,
#endif
#elif	defined(__PWM_E_0_CCD__)
#ifdef	EE_AVR8_TCE0_CCD_ISR_PRI
EE_TC_HW_IRQ_CCD_PRI(EE_AVR8_TCE0_CCD_ISR_PRI),
#else
EE_ISR_PRI_OFF,
#endif
#else
EE_ISR_PRI_OFF,
#endif
#else	/* __PWM_CHANNEL_E_0__ */
EE_ISR_PRI_OFF,
#endif	/* __PWM_CHANNEL_E_0__ */

#else	/* __AVR_ATxmega16D4__ || __AVR_ATxmega32D4__ */

/* CHANNEL C 0 */
#ifdef	__PWM_CHANNEL_C_0__
#if	defined(__PWM_C_0_CCA__)
#ifdef	EE_AVR8_TCC0_CCA_ISR_PRI
EE_TC_HW_IRQ_CCA_PRI(EE_AVR8_TCC0_CCA_ISR_PRI),
#else
EE_ISR_PRI_OFF,
#endif
#elif	defined(__PWM_C_0_CCB__)
#ifdef	EE_AVR8_TCC0_CCB_ISR_PRI
EE_TC_HW_IRQ_CCB_PRI(EE_AVR8_TCC0_CCB_ISR_PRI),
#else
EE_ISR_PRI_OFF,
#endif
#elif	defined(__PWM_C_0_CCC__)
#ifdef	EE_AVR8_TCC0_CCC_ISR_PRI
EE_TC_HW_IRQ_CCC_PRI(EE_AVR8_TCC0_CCC_ISR_PRI),
#else
EE_ISR_PRI_OFF,
#endif
#elif	defined(__PWM_C_0_CCD__)
#ifdef	EE_AVR8_TCC0_CCD_ISR_PRI
EE_TC_HW_IRQ_CCD_PRI(EE_AVR8_TCC0_CCD_ISR_PRI),
#else
EE_ISR_PRI_OFF,
#endif
#else
EE_ISR_PRI_OFF,
#endif
#else	/* __PWM_CHANNEL_C_0__ */
EE_ISR_PRI_OFF,
#endif	/* __PWM_CHANNEL_C_0__ */

/* CHANNEL D 0 */
#ifdef __PWM_CHANNEL_D_0__
#if	defined(__PWM_D_0_CCA__)
#ifdef	EE_AVR8_TCD0_CCA_ISR_PRI
EE_TC_HW_IRQ_CCA_PRI(EE_AVR8_TCD0_CCA_ISR_PRI),
#else
EE_ISR_PRI_OFF,
#endif
#elif	defined(__PWM_D_0_CCB__)
#ifdef	EE_AVR8_TCD0_CCB_ISR_PRI
EE_TC_HW_IRQ_CCB_PRI(EE_AVR8_TCD0_CCB_ISR_PRI),
#else
EE_ISR_PRI_OFF,
#endif
#elif	defined(__PWM_D_0_CCC__)
#ifdef	EE_AVR8_TCD0_CCC_ISR_PRI
EE_TC_HW_IRQ_CCC_PRI(EE_AVR8_TCD0_CCC_ISR_PRI),
#else
EE_ISR_PRI_OFF,
#endif
#elif	defined(__PWM_D_0_CCD__)
#ifdef	EE_AVR8_TCD0_CCD_ISR_PRI
EE_TC_HW_IRQ_CCD_PRI(EE_AVR8_TCD0_CCD_ISR_PRI),
#else
EE_ISR_PRI_OFF,
#endif
#else
EE_ISR_PRI_OFF,
#endif
#else	/* __PWM_CHANNEL_D_0__ */
EE_ISR_PRI_OFF,
#endif	/* __PWM_CHANNEL_D_0__ */

/* CHANNEL E 0 */
#ifdef __PWM_CHANNEL_E_0__
#if	defined(__PWM_E_0_CCA__)
#ifdef	EE_AVR8_TCE0_CCA_ISR_PRI
EE_TC_HW_IRQ_CCA_PRI(EE_AVR8_TCE0_CCA_ISR_PRI),
#else
EE_ISR_PRI_OFF,
#endif
#elif	defined(__PWM_E_0_CCB__)
#ifdef	EE_AVR8_TCE0_CCB_ISR_PRI
EE_TC_HW_IRQ_CCB_PRI(EE_AVR8_TCE0_CCB_ISR_PRI),
#else
EE_ISR_PRI_OFF,
#endif
#elif	defined(__PWM_E_0_CCC__)
#ifdef	EE_AVR8_TCE0_CCC_ISR_PRI
EE_TC_HW_IRQ_CCC_PRI(EE_AVR8_TCE0_CCC_ISR_PRI),
#else
EE_ISR_PRI_OFF,
#endif
#elif	defined(__PWM_E_0_CCD__)
#ifdef	EE_AVR8_TCE0_CCD_ISR_PRI
EE_TC_HW_IRQ_CCD_PRI(EE_AVR8_TCE0_CCD_ISR_PRI),
#else
EE_ISR_PRI_OFF,
#endif
#else
EE_ISR_PRI_OFF,
#endif
#else	/* __PWM_CHANNEL_E_0__ */
EE_ISR_PRI_OFF,
#endif	/* __PWM_CHANNEL_E_0__ */

/* CHANNEL F 0 */
#ifdef __PWM_CHANNEL_F_0__
#if	defined(__PWM_F_0_CCA__)
#ifdef	EE_AVR8_TCF0_CCA_ISR_PRI
EE_TC_HW_IRQ_CCA_PRI(EE_AVR8_TCF0_CCA_ISR_PRI),
#else
EE_ISR_PRI_OFF,
#endif
#elif	defined(__PWM_F_0_CCB__)
#ifdef	EE_AVR8_TCF0_CCB_ISR_PRI
EE_TC_HW_IRQ_CCB_PRI(EE_AVR8_TCF0_CCB_ISR_PRI),
#else
EE_ISR_PRI_OFF,
#endif
#elif	defined(__PWM_F_0_CCC__)
#ifdef	EE_AVR8_TCF0_CCC_ISR_PRI
EE_TC_HW_IRQ_CCC_PRI(EE_AVR8_TCF0_CCC_ISR_PRI),
#else
EE_ISR_PRI_OFF,
#endif
#elif	defined(__PWM_F_0_CCD__)
#ifdef	EE_AVR8_TCF0_CCD_ISR_PRI
EE_TC_HW_IRQ_CCD_PRI(EE_AVR8_TCF0_CCD_ISR_PRI),
#else
EE_ISR_PRI_OFF,
#endif
#else
EE_ISR_PRI_OFF,
#endif
#else	/* __PWM_CHANNEL_F_0__ */
EE_ISR_PRI_OFF,
#endif	/* __PWM_CHANNEL_F_0__ */

#endif	/* __AVR_ATxmega16D4__ || __AVR_ATxmega32D4__ */

};	/* EE_Pwm_0_Ccx_Pri[EE_TC_0_HW_UNITS_NUMBER] */

/*
 * PWM 1 IRQ(s) Priority Array.
 */
#ifdef	DEBUG
volatile const EE_UREG
#else
const EE_UREG
#endif
#ifdef	__AVR_PGMSPACE__
EE_Pwm_1_Ccx_Pri[EE_TC_1_HW_UNITS_NUMBER] PROGMEM = {
#else	/* __AVR_PGMSPACE__ */
EE_Pwm_1_Ccx_Pri[EE_TC_1_HW_UNITS_NUMBER] = {
#endif	/* __AVR_PGMSPACE__ */

#if	( defined(__AVR_ATxmega16D4__) || defined(__AVR_ATxmega32D4__) )

/* CHANNEL C 1 */
#ifdef	__PWM_CHANNEL_C_1__
#if	defined(__PWM_C_1_CCA__)
#ifdef	EE_AVR8_TCC1_CCA_ISR_PRI
EE_TC_HW_IRQ_CCA_PRI(EE_AVR8_TCC1_CCA_ISR_PRI),
#else
EE_ISR_PRI_OFF,
#endif
#elif	defined(__PWM_C_1_CCB__)
#ifdef	EE_AVR8_TCC1_CCB_ISR_PRI
EE_TC_HW_IRQ_CCB_PRI(EE_AVR8_TCC1_CCB_ISR_PRI),
#else
EE_ISR_PRI_OFF,
#endif
#else
EE_ISR_PRI_OFF,
#endif
#else	/* __PWM_CHANNEL_C_1__ */
EE_ISR_PRI_OFF,
#endif	/* __PWM_CHANNEL_C_1__ */

#else	/* __AVR_ATxmega16D4__ || __AVR_ATxmega32D4__ */

/* CHANNEL C 1 */
#ifdef	__PWM_CHANNEL_C_1__
#if	defined(__PWM_C_1_CCA__)
#ifdef	EE_AVR8_TCC1_CCA_ISR_PRI
EE_TC_HW_IRQ_CCA_PRI(EE_AVR8_TCC1_CCA_ISR_PRI),
#else
EE_ISR_PRI_OFF,
#endif
#elif	defined(__PWM_C_1_CCB__)
#ifdef	EE_AVR8_TCC1_CCB_ISR_PRI
EE_TC_HW_IRQ_CCB_PRI(EE_AVR8_TCC1_CCB_ISR_PRI),
#else
EE_ISR_PRI_OFF,
#endif
#else
EE_ISR_PRI_OFF,
#endif
#else	/* __PWM_CHANNEL_C_1__ */
EE_ISR_PRI_OFF,
#endif	/* __PWM_CHANNEL_C_1__ */

/* CHANNEL D 1 */
#ifdef __PWM_CHANNEL_D_1__
#if	defined(__PWM_D_1_CCA__)
#ifdef	EE_AVR8_TCD1_CCA_ISR_PRI
EE_TC_HW_IRQ_CCA_PRI(EE_AVR8_TCD1_CCA_ISR_PRI),
#else
EE_ISR_PRI_OFF,
#endif
#elif	defined(__PWM_D_1_CCB__)
#ifdef	EE_AVR8_TCD1_CCB_ISR_PRI
EE_TC_HW_IRQ_CCB_PRI(EE_AVR8_TCD1_CCB_ISR_PRI),
#else
EE_ISR_PRI_OFF,
#endif
#else
EE_ISR_PRI_OFF,
#endif
#else	/* __PWM_CHANNEL_D_1__ */
EE_ISR_PRI_OFF,
#endif	/* __PWM_CHANNEL_D_1__ */

/* CHANNEL E 1 */
#ifdef __PWM_CHANNEL_E_1__
#if	defined(__PWM_E_1_CCA__)
#ifdef	EE_AVR8_TCE1_CCA_ISR_PRI
EE_TC_HW_IRQ_CCA_PRI(EE_AVR8_TCE1_CCA_ISR_PRI),
#else
EE_ISR_PRI_OFF,
#endif
#elif	defined(__PWM_E_1_CCB__)
#ifdef	EE_AVR8_TCE1_CCB_ISR_PRI
EE_TC_HW_IRQ_CCB_PRI(EE_AVR8_TCE1_CCB_ISR_PRI),
#else
EE_ISR_PRI_OFF,
#endif
#else
EE_ISR_PRI_OFF,
#endif
#else	/* __PWM_CHANNEL_E_1__ */
EE_ISR_PRI_OFF,
#endif	/* __PWM_CHANNEL_E_1__ */

/* CHANNEL F 1 */
#ifdef __PWM_CHANNEL_F_1__
#if	defined(__PWM_F_1_CCA__)
#ifdef	EE_AVR8_TCF1_CCA_ISR_PRI
EE_TC_HW_IRQ_CCA_PRI(EE_AVR8_TCF1_CCA_ISR_PRI),
#else
EE_ISR_PRI_OFF,
#endif
#elif	defined(__PWM_F_1_CCB__)
#ifdef	EE_AVR8_TCF1_CCB_ISR_PRI
EE_TC_HW_IRQ_CCB_PRI(EE_AVR8_TCF1_CCB_ISR_PRI),
#else
EE_ISR_PRI_OFF,
#endif
#else
EE_ISR_PRI_OFF,
#endif
#else	/* __PWM_CHANNEL_F_1__ */
EE_ISR_PRI_OFF,
#endif	/* __PWM_CHANNEL_F_1__ */

#endif	/* __AVR_ATxmega16D4__ || __AVR_ATxmega32D4__ */

};	/* EE_Pwm_1_Ccx_Pri[EE_TC_1_HW_UNITS_NUMBER] */

/*
 * PWM IRQ(s) Priority Retrieval.
 */
__INLINE__ EE_UREG __ALWAYS_INLINE__
EE_Pwm_Ccx_GetPri(
  EE_TYPEPWMCHANNEL	Channel
) {

#if	DEBUG
  volatile EE_UREG	Pri;
#else	/* DEBUG */
  register EE_UREG	Pri;
#endif	/* DEBUG */

  if ( Channel & EE_TC_HW_ALL_TCX1_MASK ) {

#ifdef	__AVR_PGMSPACE__
    Pri = (EE_UREG) pgm_read_byte(
      &EE_Pwm_1_Ccx_Pri[(Channel & ~EE_TC_HW_ALL_TCX1_MASK)]
    );
#else	/* __AVR_PGMSPACE__ */
    Pri = EE_Pwm_1_Ccx_Pri[(Channel & ~EE_TC_HW_ALL_TCX1_MASK)];
#endif	/* __AVR_PGMSPACE__ */

  }
  else {

#ifdef	__AVR_PGMSPACE__
    Pri = (EE_URGE) pgm_read_byte(&EE_Pwm_0_Ccx_Pri[Channel]);
#else	/* __AVR_PGMSPACE__ */
    Pri = EE_Pwm_0_Ccx_Pri[Channel];
#endif	/* __AVR_PGMSPACE__ */

  }

  return Pri;

}

#if	0
/* PWM ISR
 *
 * param[in]	Channel		PWM Channel Identifier.
 */
static void
EE_Pwm_ISR( EE_TYPEPWMCHANNEL Channel )
{

#ifdef	DEBUG
  volatile EE_UREG		flags;
  volatile EE_ISR_callback	cb;
#else	/* DEBUG */
  register EE_UREG		flags;
  register EE_ISR_callback	cb;
#endif	/* DEBUG */

  /* Suspend Interrupt */
  flags = EE_hal_suspendIRQ();

  cb = EE_Tc_Hw_GetCallback(Channel);

  /* Resume Interrupt */
  EE_hal_resumeIRQ(flags);

  /* Notification Enabled Check!!! */
  if ( cb != EE_NULL_PTR ) {

    /* Call Call-Back. */
    (*cb)();

  }

}	/* EE_Pwm_ISR( EE_TYPEPWMCHANNEL Channel ) */
#endif	/* 0 */

#if	( defined(__AVR_ATxmega16D4__) || defined(__AVR_ATxmega32D4__) )

#if	( defined(__PWM_CHANNEL_C_0__) && defined(EE_AVR8_TCC0_OVF_ISR) )
#if	( EE_AVR8_TCC0_OVF_ISR_CAT == 1 )
ISR1(EE_AVR8_TCC0_OVF_ISR)
#else
ISR2(EE_AVR8_TCC0_OVF_ISR)
#endif
{
#if	0
  EE_Pwm_ISR( EE_PWM_CHANNEL_C_0 );
#endif
  EE_Tc_Hw_GetCallback(EE_PWM_CHANNEL_C_0)();
}
#endif	/* __PWM_CHANNEL_C_0__ && EE_AVR8_TCC0_OVF_ISR */

#if	( \
	defined(__PWM_CHANNEL_C_0__) &&	\
	defined(__PWM_C_0_CCA__) &&	\
	defined(EE_AVR8_TCC0_CCA_ISR)	\
)
#if	( EE_AVR8_TCC0_CCA_ISR_CAT == 1 )
ISR1(EE_AVR8_TCC0_CCA_ISR)
#else
ISR2(EE_AVR8_TCC0_CCA_ISR)
#endif
{
#if	0
  EE_Pwm_ISR( EE_PWM_CHANNEL_C_0 );
#endif
  EE_Tc_Hw_GetCallback(EE_PWM_CHANNEL_C_0)();
}
#endif	/* __PWM_CHANNEL_C_0__ && __PWM_C_0_CCA__ && EE_AVR8_TCC0_CCA_ISR */

#if	( \
	defined(__PWM_CHANNEL_C_0__) &&	\
	defined(__PWM_C_0_CCB__) &&	\
	defined(EE_AVR8_TCC0_CCB_ISR)	\
)
#if	( EE_AVR8_TCC0_CCB_ISR_CAT == 1 )
ISR1(EE_AVR8_TCC0_CCB_ISR)
#else
ISR2(EE_AVR8_TCC0_CCB_ISR)
#endif
{
#if	0
  EE_Pwm_ISR( EE_PWM_CHANNEL_C_0 );
#endif
  EE_Tc_Hw_GetCallback(EE_PWM_CHANNEL_C_0)();
}
#endif	/* __PWM_CHANNEL_C_0__ && __PWM_C_0_CCB__ && EE_AVR8_TCC0_CCB_ISR */

#if	( \
	defined(__PWM_CHANNEL_C_0__) &&	\
	defined(__PWM_C_0_CCC__) &&	\
	defined(EE_AVR8_TCC0_CCC_ISR)	\
)
#if	( EE_AVR8_TCC0_CCC_ISR_CAT == 1 )
ISR1(EE_AVR8_TCC0_CCC_ISR)
#else
ISR2(EE_AVR8_TCC0_CCC_ISR)ù
#endif
{
#if	0
  EE_Pwm_ISR( EE_PWM_CHANNEL_C_0 );
#endif
  EE_Tc_Hw_GetCallback(EE_PWM_CHANNEL_C_0)();
}
#endif	/* __PWM_CHANNEL_C_0__ && __PWM_C_0_CCC__ && EE_AVR8_TCC0_CCC_ISR */

#if	( \
	defined(__PWM_CHANNEL_C_0__) &&	\
	defined(__PWM_C_0_CCD__) &&	\
	defined(EE_AVR8_TCC0_CCD_ISR)	\
)
#if	( EE_AVR8_TCC0_CCD_ISR_CAT == 1 )
ISR1(EE_AVR8_TCC0_CCD_ISR)
#else
ISR2(EE_AVR8_TCC0_CCD_ISR)
#endif
{
#if	0
  EE_Pwm_ISR( EE_PWM_CHANNEL_C_0 );
#endif
  EE_Tc_Hw_GetCallback(EE_PWM_CHANNEL_C_0)();
}
#endif	/* __PWM_CHANNEL_C_0__ && __PWM_C_0_CCD__ && EE_AVR8_TCC0_CCD_ISR */

#if	( defined(__PWM_CHANNEL_D_0__) && defined(EE_AVR8_TCD0_OVF_ISR) )
#if	( EE_AVR8_TCD0_OVF_ISR_CAT == 1 )
ISR1(EE_AVR8_TCD0_OVF_ISR)
#else
ISR2(EE_AVR8_TCD0_OVF_ISR)
#endif
{
#if	0
  EE_Pwm_ISR( EE_PWM_CHANNEL_D_0 );
#endif
  EE_Tc_Hw_GetCallback(EE_PWM_CHANNEL_D_0)();
}
#endif	/* __PWM_CHANNEL_D_0__ && EE_AVR8_TCD0_OVF_ISR */

#if	( \
	defined(__PWM_CHANNEL_D_0__) &&	\
	defined(__PWM_D_0_CCA__) &&	\
	defined(EE_AVR8_TCD0_CCA_ISR)	\
)
#if	( EE_AVR8_TCD0_CCA_ISR_CAT == 1 )
ISR1(EE_AVR8_TCD0_CCA_ISR)
#else
ISR2(EE_AVR8_TCD0_CCA_ISR)
#endif
{
#if	0
  EE_Pwm_ISR( EE_PWM_CHANNEL_D_0 );
#endif
  EE_Tc_Hw_GetCallback(EE_PWM_CHANNEL_D_0)();
}
#endif	/* __PWM_CHANNEL_D_0__ && __PWM_D_0_CCA__ && EE_AVR8_TCD0_CCA_ISR */

#if	( \
	defined(__PWM_CHANNEL_D_0__) &&	\
	defined(__PWM_D_0_CCB__) &&	\
	defined(EE_AVR8_TCD0_CCB_ISR)	\
)
#if	( EE_AVR8_TCD0_CCB_ISR_CAT == 1 )
ISR1(EE_AVR8_TCD0_CCB_ISR)
#else
ISR2(EE_AVR8_TCD0_CCB_ISR)
#endif
{
#if	0
  EE_Pwm_ISR( EE_PWM_CHANNEL_D_0 );
#endif
  EE_Tc_Hw_GetCallback(EE_PWM_CHANNEL_D_0)();
}
#endif	/* __PWM_CHANNEL_D_0__ && __PWM_D_0_CCB__ && EE_AVR8_TCD0_CCB_ISR */

#if	( \
	defined(__PWM_CHANNEL_D_0__) &&	\
	defined(__PWM_D_0_CCC__) &&	\
	defined(EE_AVR8_TCD0_CCC_ISR)	\
)
#if	( EE_AVR8_TCD0_CCC_ISR_CAT == 1 )
ISR1(EE_AVR8_TCD0_CCC_ISR)
#else
ISR2(EE_AVR8_TCD0_CCC_ISR)
#endif
{
#if	0
  EE_Pwm_ISR( EE_PWM_CHANNEL_D_0 );
#endif
  EE_Tc_Hw_GetCallback(EE_PWM_CHANNEL_D_0)();
}
#endif	/* __PWM_CHANNEL_D_0__ && __PWM_D_0_CCC__ && EE_AVR8_TCD0_CCC_ISR */

#if	( \
	defined(__PWM_CHANNEL_D_0__) &&	\
	defined(__PWM_D_0_CCD__) &&	\
	defined(EE_AVR8_TCD0_CCD_ISR)	\
)
#if	( EE_AVR8_TCD0_CCD_ISR_CAT == 1 )
ISR1(EE_AVR8_TCD0_CCD_ISR)
#else
ISR2(EE_AVR8_TCD0_CCD_ISR)
#endif
{
#if	0
  EE_Pwm_ISR( EE_PWM_CHANNEL_D_0 );
#endif
  EE_Tc_Hw_GetCallback(EE_PWM_CHANNEL_D_0)();
}
#endif	/* __PWM_CHANNEL_D_0__ && __PWM_D_0_CCD__ && EE_AVR8_TCD0_CCD_ISR */

#if	( defined(__PWM_CHANNEL_E_0__) && defined(EE_AVR8_TCE0_OVF_ISR) )
#if	( EE_AVR8_TCE0_OVF_ISR_CAT == 1 )
ISR1(EE_AVR8_TCE0_OVF_ISR)
#else
ISR2(EE_AVR8_TCE0_OVF_ISR)
#endif
{
#if	0
  EE_Pwm_ISR( EE_PWM_CHANNEL_E_0 );
#endif
  EE_Tc_Hw_GetCallback(EE_PWM_CHANNEL_E_0)();
}
#endif	/* __PWM_CHANNEL_E_0__ && EE_AVR8_TCE0_OVF_ISR */

#if	( \
	defined(__PWM_CHANNEL_E_0__) &&	\
	defined(__PWM_E_0_CCA__) &&	\
	defined(EE_AVR8_TCE0_CCA_ISR)	\
)
#if	( EE_AVR8_TCE0_CCA_ISR_CAT == 1 )
ISR1(EE_AVR8_TCE0_CCA_ISR)
#else
ISR2(EE_AVR8_TCE0_CCA_ISR)
#endif
{
#if	0
  EE_Pwm_ISR( EE_PWM_CHANNEL_E_0 );
#endif
  EE_Tc_Hw_GetCallback(EE_PWM_CHANNEL_E_0)();
}
#endif	/* __PWM_CHANNEL_E_0__ && __PWM_E_0_CCA__ && EE_AVR8_TCE0_CCA_ISR */

#if	( \
	defined(__PWM_CHANNEL_E_0__) &&	\
	defined(__PWM_E_0_CCB__) &&	\
	defined(EE_AVR8_TCE0_CCB_ISR)	\
)
#if	( EE_AVR8_TCE0_CCB_ISR_CAT == 1 )
ISR1(EE_AVR8_TCE0_CCB_ISR)
#else
ISR2(EE_AVR8_TCE0_CCB_ISR)
#endif
{
#if	0
  EE_Pwm_ISR( EE_PWM_CHANNEL_E_0 );
#endif
  EE_Tc_Hw_GetCallback(EE_PWM_CHANNEL_E_0)();
}
#endif	/* __PWM_CHANNEL_E_0__ && __PWM_E_0_CCB__ && EE_AVR8_TCE0_CCB_ISR */

#if	( \
	defined(__PWM_CHANNEL_E_0__) &&	\
	defined(__PWM_E_0_CCC__) &&	\
	defined(EE_AVR8_TCE0_CCC_ISR)	\
)
#if	( EE_AVR8_TCE0_CCC_ISR_CAT == 1 )
ISR1(EE_AVR8_TCE0_CCC_ISR)
#else
ISR2(EE_AVR8_TCE0_CCC_ISR)
#endif

{
#if	0
  EE_Pwm_ISR( EE_PWM_CHANNEL_E_0 );
#endif
  EE_Tc_Hw_GetCallback(EE_PWM_CHANNEL_E_0)();
}
#endif	/* __PWM_CHANNEL_E_0__ && __PWM_E_0_CCC__ && EE_AVR8_TCE0_CCC_ISR */

#if	( \
	defined(__PWM_CHANNEL_E_0__) &&	\
	defined(__PWM_E_0_CCD__) &&	\
	defined(EE_AVR8_TCE0_CCD_ISR)	\
)
#if	( EE_AVR8_TCE0_CCD_ISR_CAT == 1 )
ISR1(EE_AVR8_TCE0_CCD_ISR)
#else
ISR2(EE_AVR8_TCE0_CCD_ISR)
#endif
{
#if	0
  EE_Pwm_ISR( EE_PWM_CHANNEL_E_0 );
#endif
  EE_Tc_Hw_GetCallback(EE_PWM_CHANNEL_E_0)();
}
#endif	/* __PWM_CHANNEL_E_0__ && __PWM_E_0_CCD__ && EE_AVR8_TCE0_CCD_ISR */

#if	( defined(__PWM_CHANNEL_C_1__) && defined(EE_AVR8_TCC1_OVF_ISR) )
#if	( EE_AVR8_TCC1_OVF_ISR_CAT == 1 )
ISR1(EE_AVR8_TCC1_OVF_ISR)
#else
ISR2(EE_AVR8_TCC1_OVF_ISR)
#endif
{
#if	0
  EE_Pwm_ISR( EE_PWM_CHANNEL_C_1 );
#endif
  EE_Tc_Hw_GetCallback(EE_PWM_CHANNEL_C_1)();
}
#endif	/* __PWM_CHANNEL_C_1__ && EE_AVR8_TCC1_OVF_ISR */

#if	( \
	defined(__PWM_CHANNEL_C_1__) &&	\
	defined(__PWM_C_1_CCA__) &&	\
	defined(EE_AVR8_TCC1_CCA_ISR)	\
)
#if	( EE_AVR8_TCC1_CCA_ISR_CAT == 1 )
ISR1(EE_AVR8_TCC1_CCA_ISR)
#else
ISR2(EE_AVR8_TCC1_CCA_ISR)
#endif
{
#if	0
  EE_Pwm_ISR( EE_PWM_CHANNEL_C_1 );
#endif
  EE_Tc_Hw_GetCallback(EE_PWM_CHANNEL_C_1)();
}
#endif	/* __PWM_CHANNEL_C_1__ && __PWM_C_1_CCA__ && EE_AVR8_TCC1_CCA_ISR */

#if	( \
	defined(__PWM_CHANNEL_C_1__) &&	\
	defined(__PWM_C_1_CCB__) &&	\
	defined(EE_AVR8_TCC1_CCB_ISR)	\
)
#if	( EE_AVR8_TCC1_CCB_ISR_CAT == 1 )
ISR1(EE_AVR8_TCC1_CCB_ISR)
#else
ISR2(EE_AVR8_TCC1_CCB_ISR)
#endif
{
#if	0
  EE_Pwm_ISR( EE_PWM_CHANNEL_C_1 );
#endif
  EE_Tc_Hw_GetCallback(EE_PWM_CHANNEL_C_1)();
}
#endif	/* __PWM_CHANNEL_C_1__ && __PWM_C_1_CCB__ && EE_AVR8_TCC1_CCB_ISR */

#else	/* __AVR_ATxmega16D4__ || __AVR_ATxmega32D4__ */

#if	( defined(__PWM_CHANNEL_C_0__) && defined(EE_AVR8_TCC0_OVF_ISR) )
#if	( EE_AVR8_TCC0_OVF_ISR_CAT == 1 )
ISR1(EE_AVR8_TCC0_OVF_ISR)
#else
ISR2(EE_AVR8_TCC0_OVF_ISR)
#endif
{
#if	0
  EE_Pwm_ISR( EE_PWM_CHANNEL_C_0 );
#endif
  EE_Tc_Hw_GetCallback(EE_PWM_CHANNEL_C_0)();
}
#endif	/* __PWM_CHANNEL_C_0__ && EE_AVR8_TCC0_OVF_ISR */

#if	( \
	defined(__PWM_CHANNEL_C_0__) &&	\
	defined(__PWM_C_0_CCA__) &&	\
	defined(EE_AVR8_TCC0_CCA_ISR)	\
)
#if	( EE_AVR8_TCC0_CCA_ISR_CAT == 1 )
ISR1(EE_AVR8_TCC0_CCA_ISR)
#else
ISR2(EE_AVR8_TCC0_CCA_ISR)
#endif
{
#if	0
  EE_Pwm_ISR( EE_PWM_CHANNEL_C_0 );
#endif
  EE_Tc_Hw_GetCallback(EE_PWM_CHANNEL_C_0)();
}
#endif	/* __PWM_CHANNEL_C_0__ && __PWM_C_0_CCA__ && EE_AVR8_TCC0_CCA_ISR */

#if	( \
	defined(__PWM_CHANNEL_C_0__) &&	\
	defined(__PWM_C_0_CCB__) &&	\
	defined(EE_AVR8_TCC0_CCB_ISR)	\
)
#if	( EE_AVR8_TCC0_CCB_ISR_CAT == 1 )
ISR1(EE_AVR8_TCC0_CCB_ISR)
#else
ISR2(EE_AVR8_TCC0_CCB_ISR)
#endif
{
#if	0
  EE_Pwm_ISR( EE_PWM_CHANNEL_C_0 );
#endif
  EE_Tc_Hw_GetCallback(EE_PWM_CHANNEL_C_0)();
}
#endif	/* __PWM_CHANNEL_C_0__ && __PWM_C_0_CCB__ && EE_AVR8_TCC0_CCB_ISR */

#if	( \
	defined(__PWM_CHANNEL_C_0__) &&	\
	defined(__PWM_C_0_CCC__) &&	\
	defined(EE_AVR8_TCC0_CCC_ISR)	\
)
#if	( EE_AVR8_TCC0_CCC_ISR_CAT == 1 )
ISR1(EE_AVR8_TCC0_CCC_ISR)
#else
ISR2(EE_AVR8_TCC0_CCC_ISR)ù
#endif
{
#if	0
  EE_Pwm_ISR( EE_PWM_CHANNEL_C_0 );
#endif
  EE_Tc_Hw_GetCallback(EE_PWM_CHANNEL_C_0)();
}
#endif	/* __PWM_CHANNEL_C_0__ && __PWM_C_0_CCC__ && EE_AVR8_TCC0_CCC_ISR */

#if	( \
	defined(__PWM_CHANNEL_C_0__) &&	\
	defined(__PWM_C_0_CCD__) &&	\
	defined(EE_AVR8_TCC0_CCD_ISR)	\
)
#if	( EE_AVR8_TCC0_CCD_ISR_CAT == 1 )
ISR1(EE_AVR8_TCC0_CCD_ISR)
#else
ISR2(EE_AVR8_TCC0_CCD_ISR)
#endif
{
#if	0
  EE_Pwm_ISR( EE_PWM_CHANNEL_C_0 );
#endif
  EE_Tc_Hw_GetCallback(EE_PWM_CHANNEL_C_0)();
}
#endif	/* __PWM_CHANNEL_C_0__ && __PWM_C_0_CCD__ && EE_AVR8_TCC0_CCD_ISR */

#if	( defined(__PWM_CHANNEL_D_0__) && defined(EE_AVR8_TCD0_OVF_ISR) )
#if	( EE_AVR8_TCD0_OVF_ISR_CAT == 1 )
ISR1(EE_AVR8_TCD0_OVF_ISR)
#else
ISR2(EE_AVR8_TCD0_OVF_ISR)
#endif
{
#if	0
  EE_Pwm_ISR( EE_PWM_CHANNEL_D_0 );
#endif
  EE_Tc_Hw_GetCallback(EE_PWM_CHANNEL_D_0)();
}
#endif	/* __PWM_CHANNEL_D_0__ && EE_AVR8_TCD0_OVF_ISR */

#if	( \
	defined(__PWM_CHANNEL_D_0__) &&	\
	defined(__PWM_D_0_CCA__) &&	\
	defined(EE_AVR8_TCD0_CCA_ISR)	\
)
#if	( EE_AVR8_TCD0_CCA_ISR_CAT == 1 )
ISR1(EE_AVR8_TCD0_CCA_ISR)
#else
ISR2(EE_AVR8_TCD0_CCA_ISR)
#endif
{
#if	0
  EE_Pwm_ISR( EE_PWM_CHANNEL_D_0 );
#endif
  EE_Tc_Hw_GetCallback(EE_PWM_CHANNEL_D_0)();
}
#endif	/* __PWM_CHANNEL_D_0__ && __PWM_D_0_CCA__ && EE_AVR8_TCD0_CCA_ISR */

#if	( \
	defined(__PWM_CHANNEL_D_0__) &&	\
	defined(__PWM_D_0_CCB__) &&	\
	defined(EE_AVR8_TCD0_CCB_ISR)	\
)
#if	( EE_AVR8_TCD0_CCB_ISR_CAT == 1 )
ISR1(EE_AVR8_TCD0_CCB_ISR)
#else
ISR2(EE_AVR8_TCD0_CCB_ISR)
#endif
{
#if	0
  EE_Pwm_ISR( EE_PWM_CHANNEL_D_0 );
#endif
  EE_Tc_Hw_GetCallback(EE_PWM_CHANNEL_D_0)();
}
#endif	/* __PWM_CHANNEL_D_0__ && __PWM_D_0_CCB__ && EE_AVR8_TCD0_CCB_ISR */

#if	( \
	defined(__PWM_CHANNEL_D_0__) &&	\
	defined(__PWM_D_0_CCC__) &&	\
	defined(EE_AVR8_TCD0_CCC_ISR)	\
)
#if	( EE_AVR8_TCD0_CCC_ISR_CAT == 1 )
ISR1(EE_AVR8_TCD0_CCC_ISR)
#else
ISR2(EE_AVR8_TCD0_CCC_ISR)
#endif
{
#if	0
  EE_Pwm_ISR( EE_PWM_CHANNEL_D_0 );
#endif
  EE_Tc_Hw_GetCallback(EE_PWM_CHANNEL_D_0)();
}
#endif	/* __PWM_CHANNEL_D_0__ && __PWM_D_0_CCC__ && EE_AVR8_TCD0_CCC_ISR */

#if	( \
	defined(__PWM_CHANNEL_D_0__) &&	\
	defined(__PWM_D_0_CCD__) &&	\
	defined(EE_AVR8_TCD0_CCD_ISR)	\
)
#if	( EE_AVR8_TCD0_CCD_ISR_CAT == 1 )
ISR1(EE_AVR8_TCD0_CCD_ISR)
#else
ISR2(EE_AVR8_TCD0_CCD_ISR)
#endif
{
#if	0
  EE_Pwm_ISR( EE_PWM_CHANNEL_D_0 );
#endif
  EE_Tc_Hw_GetCallback(EE_PWM_CHANNEL_D_0)();
}
#endif	/* __PWM_CHANNEL_D_0__ && __PWM_D_0_CCD__ && EE_AVR8_TCD0_CCD_ISR */

#if	( defined(__PWM_CHANNEL_E_0__) && defined(EE_AVR8_TCE0_OVF_ISR) )
#if	( EE_AVR8_TCE0_OVF_ISR_CAT == 1 )
ISR1(EE_AVR8_TCE0_OVF_ISR)
#else
ISR2(EE_AVR8_TCE0_OVF_ISR)
#endif
{
#if	0
  EE_Pwm_ISR( EE_PWM_CHANNEL_E_0 );
#endif
  EE_Tc_Hw_GetCallback(EE_PWM_CHANNEL_E_0)();
}
#endif	/* __PWM_CHANNEL_E_0__ && EE_AVR8_TCE0_OVF_ISR */

#if	( \
	defined(__PWM_CHANNEL_E_0__) &&	\
	defined(__PWM_E_0_CCA__) &&	\
	defined(EE_AVR8_TCE0_CCA_ISR)	\
)
#if	( EE_AVR8_TCE0_CCA_ISR_CAT == 1 )
ISR1(EE_AVR8_TCE0_CCA_ISR)
#else
ISR2(EE_AVR8_TCE0_CCA_ISR)
#endif
{
#if	0
  EE_Pwm_ISR( EE_PWM_CHANNEL_E_0 );
#endif
  EE_Tc_Hw_GetCallback(EE_PWM_CHANNEL_E_0)();
}
#endif	/* __PWM_CHANNEL_E_0__ && __PWM_E_0_CCA__ && EE_AVR8_TCE0_CCA_ISR */

#if	( \
	defined(__PWM_CHANNEL_E_0__) &&	\
	defined(__PWM_E_0_CCB__) &&	\
	defined(EE_AVR8_TCE0_CCB_ISR)	\
)
#if	( EE_AVR8_TCE0_CCB_ISR_CAT == 1 )
ISR1(EE_AVR8_TCE0_CCB_ISR)
#else
ISR2(EE_AVR8_TCE0_CCB_ISR)
#endif
{
#if	0
  EE_Pwm_ISR( EE_PWM_CHANNEL_E_0 );
#endif
  EE_Tc_Hw_GetCallback(EE_PWM_CHANNEL_E_0)();
}
#endif	/* __PWM_CHANNEL_E_0__ && __PWM_E_0_CCB__ && EE_AVR8_TCE0_CCB_ISR */

#if	( \
	defined(__PWM_CHANNEL_E_0__) &&	\
	defined(__PWM_E_0_CCC__) &&	\
	defined(EE_AVR8_TCE0_CCC_ISR)	\
)
#if	( EE_AVR8_TCE0_CCC_ISR_CAT == 1 )
ISR1(EE_AVR8_TCE0_CCC_ISR)
#else
ISR2(EE_AVR8_TCE0_CCC_ISR)
#endif
{
#if	0
  EE_Pwm_ISR( EE_PWM_CHANNEL_E_0 );
#endif
  EE_Tc_Hw_GetCallback(EE_PWM_CHANNEL_E_0)();
}
#endif	/* __PWM_CHANNEL_E_0__ && __PWM_E_0_CCC__ && EE_AVR8_TCE0_CCC_ISR */

#if	( \
	defined(__PWM_CHANNEL_E_0__) &&	\
	defined(__PWM_E_0_CCD__) &&	\
	defined(EE_AVR8_TCE0_CCD_ISR)	\
)
#if	( EE_AVR8_TCE0_CCD_ISR_CAT == 1 )
ISR1(EE_AVR8_TCE0_CCD_ISR)
#else
ISR2(EE_AVR8_TCE0_CCD_ISR)
#endif
{
#if	0
  EE_Pwm_ISR( EE_PWM_CHANNEL_E_0 );
#endif
  EE_Tc_Hw_GetCallback(EE_PWM_CHANNEL_E_0)();
}
#endif	/* __PWM_CHANNEL_E_0__ && __PWM_E_0_CCD__ && EE_AVR8_TCE0_CCD_ISR */

#if	( defined(__PWM_CHANNEL_F_0__) && defined(EE_AVR8_TCF0_OVF_ISR) )
#if	( EE_AVR8_TCF0_OVF_ISR_CAT == 1 )
ISR1(EE_AVR8_TCF0_OVF_ISR)
#else
ISR2(EE_AVR8_TCF0_OVF_ISR)
#endif
{
#if	0
  EE_Pwm_ISR( EE_PWM_CHANNEL_F_0 );
#endif
  EE_Tc_Hw_GetCallback(EE_PWM_CHANNEL_F_0)();
}
#endif	/* __PWM_CHANNEL_F_0__ && EE_AVR8_TCF0_OVF_ISR */

#if	( \
	defined(__PWM_CHANNEL_F_0__) &&	\
	defined(__PWM_F_0_CCA__) &&	\
	defined(EE_AVR8_TCF0_CCA_ISR)	\
)
#if	( EE_AVR8_TCF0_CCA_ISR_CAT == 1 )
ISR1(EE_AVR8_TCF0_CCA_ISR)
#else
ISR2(EE_AVR8_TCF0_CCA_ISR)
#endif
{
#if	0
  EE_Pwm_ISR( EE_PWM_CHANNEL_F_0 );
#endif
  EE_Tc_Hw_GetCallback(EE_PWM_CHANNEL_F_0)();
}
#endif	/* __PWM_CHANNEL_F_0__ && __PWM_F_0_CCA__ && EE_AVR8_TCF0_CCA_ISR */

#if	( \
	defined(__PWM_CHANNEL_F_0__) &&	\
	defined(__PWM_F_0_CCB__) &&	\
	defined(EE_AVR8_TCF0_CCB_ISR)	\
)
#if	( EE_AVR8_TCF0_CCB_ISR_CAT == 1 )
ISR1(EE_AVR8_TCF0_CCB_ISR)
#else
ISR2(EE_AVR8_TCF0_CCB_ISR)
#endif
{
#if	0
  EE_Pwm_ISR( EE_PWM_CHANNEL_F_0 );
#endif
  EE_Tc_Hw_GetCallback(EE_PWM_CHANNEL_F_0)();
}
#endif	/* __PWM_CHANNEL_F_0__ && __PWM_F_0_CCB__ && EE_AVR8_TCF0_CCB_ISR */

#if	( \
	defined(__PWM_CHANNEL_F_0__) &&	\
	defined(__PWM_F_0_CCC__) &&	\
	defined(EE_AVR8_TCF0_CCC_ISR)	\
)
#if	( EE_AVR8_TCF0_CCC_ISR_CAT == 1 )
ISR1(EE_AVR8_TCF0_CCC_ISR)
#else
ISR2(EE_AVR8_TCF0_CCC_ISR)
#endif

{
#if	0
  EE_Pwm_ISR( EE_PWM_CHANNEL_F_0 );
#endif
  EE_Tc_Hw_GetCallback(EE_PWM_CHANNEL_F_0)();
}
#endif	/* __PWM_CHANNEL_F_0__ && __PWM_F_0_CCC__ && EE_AVR8_TCF0_CCC_ISR */

#if	( \
	defined(__PWM_CHANNEL_F_0__) &&	\
	defined(__PWM_F_0_CCD__) &&	\
	defined(EE_AVR8_TCF0_CCD_ISR)	\
)
#if	( EE_AVR8_TCF0_CCD_ISR_CAT == 1 )
ISR1(EE_AVR8_TCF0_CCD_ISR)
#else
ISR2(EE_AVR8_TCF0_CCD_ISR)
#endif
{
#if	0
  EE_Pwm_ISR( EE_PWM_CHANNEL_F_0 );
#endif
  EE_Tc_Hw_GetCallback(EE_PWM_CHANNEL_F_0)();
}
#endif	/* __PWM_CHANNEL_F_0__ && __PWM_F_0_CCD__ && EE_AVR8_TCF0_CCD_ISR */

#if	( defined(__PWM_CHANNEL_C_1__) && defined(EE_AVR8_TCC1_OVF_ISR) )
#if	( EE_AVR8_TCC1_OVF_ISR_CAT == 1 )
ISR1(EE_AVR8_TCC1_OVF_ISR)
#else
ISR2(EE_AVR8_TCC1_OVF_ISR)
#endif
{
#if	0
  EE_Pwm_ISR( EE_PWM_CHANNEL_C_1 );
#endif
  EE_Tc_Hw_GetCallback(EE_PWM_CHANNEL_C_1)();
}
#endif	/* __PWM_CHANNEL_C_1__ && EE_AVR8_TCC1_OVF_ISR */

#if	( \
	defined(__PWM_CHANNEL_C_1__) &&	\
	defined(__PWM_C_1_CCA__) &&	\
	defined(EE_AVR8_TCC1_CCA_ISR)	\
)
#if	( EE_AVR8_TCC1_CCA_ISR_CAT == 1 )
ISR1(EE_AVR8_TCC1_CCA_ISR)
#else
ISR2(EE_AVR8_TCC1_CCA_ISR)
#endif
{
#if	0
  EE_Pwm_ISR( EE_PWM_CHANNEL_C_1 );
#endif
  EE_Tc_Hw_GetCallback(EE_PWM_CHANNEL_C_1)();
}
#endif	/* __PWM_CHANNEL_C_1__ && __PWM_C_1_CCA__ && EE_AVR8_TCC1_CCA_ISR */

#if	( \
	defined(__PWM_CHANNEL_C_1__) &&	\
	defined(__PWM_C_1_CCB__) &&	\
	defined(EE_AVR8_TCC1_CCB_ISR)	\
)
#if	( EE_AVR8_TCC1_CCB_ISR_CAT == 1 )
ISR1(EE_AVR8_TCC1_CCB_ISR)
#else
ISR2(EE_AVR8_TCC1_CCB_ISR)
#endif
{
#if	0
  EE_Pwm_ISR( EE_PWM_CHANNEL_C_1 );
#endif
  EE_Tc_Hw_GetCallback(EE_PWM_CHANNEL_C_1)();
}
#endif	/* __PWM_CHANNEL_C_1__ && __PWM_C_1_CCB__ && EE_AVR8_TCC1_CCB_ISR */

#if	( defined(__PWM_CHANNEL_D_1__) && defined(EE_AVR8_TCD1_OVF_ISR) )
#if	( EE_AVR8_TCD0_OVF_ISR_CAT == 1 )
ISR1(EE_AVR8_TCD1_OVF_ISR)
#else
ISR2(EE_AVR8_TCD1_OVF_ISR)
#endif
{
#if	0
  EE_Pwm_ISR( EE_PWM_CHANNEL_D_1 );
#endif
  EE_Tc_Hw_GetCallback(EE_PWM_CHANNEL_D_1)();
}
#endif	/* __PWM_CHANNEL_D_1__ && EE_AVR8_TCD1_OVF_ISR */

#if	( \
	defined(__PWM_CHANNEL_D_1__) &&	\
	defined(__PWM_D_1_CCA__) &&	\
	defined(EE_AVR8_TCD1_CCA_ISR)	\
)
#if	( EE_AVR8_TCD1_CCA_ISR_CAT == 1 )
ISR1(EE_AVR8_TCD1_CCA_ISR)
#else
ISR2(EE_AVR8_TCD1_CCA_ISR)
#endif
{
#if	0
  EE_Pwm_ISR( EE_PWM_CHANNEL_D_1 );
#endif
  EE_Tc_Hw_GetCallback(EE_PWM_CHANNEL_D_1)();
}
#endif	/* __PWM_CHANNEL_D_1__ && __PWM_D_1_CCA__ && EE_AVR8_TCD1_CCA_ISR */

#if	( \
	defined(__PWM_CHANNEL_D_1__) &&	\
	defined(__PWM_D_1_CCB__) &&	\
	defined(EE_AVR8_TCD1_CCB_ISR)	\
)
#if	( EE_AVR8_TCD1_CCB_ISR_CAT == 1 )
ISR1(EE_AVR8_TCD1_CCB_ISR)
#else
ISR2(EE_AVR8_TCD1_CCB_ISR)
#endif
{
#if	0
  EE_Pwm_ISR( EE_PWM_CHANNEL_D_1 );
#endif
  EE_Tc_Hw_GetCallback(EE_PWM_CHANNEL_D_1)();
}
#endif	/* __PWM_CHANNEL_D_1__ && __PWM_D_1_CCB__ && EE_AVR8_TCD1_CCB_ISR */

#if	( defined(__PWM_CHANNEL_E_1__) && defined(EE_AVR8_TCE1_OVF_ISR) )
#if	( EE_AVR8_TCE1_OVF_ISR_CAT == 1 )
ISR1(EE_AVR8_TCE1_OVF_ISR)
#else
ISR2(EE_AVR8_TCE1_OVF_ISR)
#endif
{
#if	0
  EE_Pwm_ISR( EE_PWM_CHANNEL_E_1 );
#endif
  EE_Tc_Hw_GetCallback(EE_PWM_CHANNEL_E_1)();
}
#endif	/* __PWM_CHANNEL_E_1__ && EE_AVR8_TCE1_OVF_ISR */

#if	( \
	defined(__PWM_CHANNEL_E_1__) &&	\
	defined(__PWM_E_1_CCA__) &&	\
	defined(EE_AVR8_TCE1_CCA_ISR)	\
)
#if	( EE_AVR8_TCE1_CCA_ISR_CAT == 1 )
ISR1(EE_AVR8_TCE1_CCA_ISR)
#else
ISR2(EE_AVR8_TCE1_CCA_ISR)
#endif
{
#if	0
  EE_Pwm_ISR( EE_PWM_CHANNEL_E_1 );
#endif
  EE_Tc_Hw_GetCallback(EE_PWM_CHANNEL_E_1)();
}
#endif	/* __PWM_CHANNEL_E_1__ && __PWM_E_1_CCA__ && EE_AVR8_TCE1_CCA_ISR */

#if	( \
	defined(__PWM_CHANNEL_E_1__) &&	\
	defined(__PWM_E_1_CCB__) &&	\
	defined(EE_AVR8_TCE1_CCB_ISR)	\
)
#if	( EE_AVR8_TCE1_CCB_ISR_CAT == 1 )
ISR1(EE_AVR8_TCE1_CCB_ISR)
#else
ISR2(EE_AVR8_TCE1_CCB_ISR)
#endif
{
#if	0
  EE_Pwm_ISR( EE_PWM_CHANNEL_E_1 );
#endif
  EE_Tc_Hw_GetCallback(EE_PWM_CHANNEL_E_1)();
}
#endif	/* __PWM_CHANNEL_E_1__ && __PWM_E_1_CCB__ && EE_AVR8_TCE1_CCB_ISR */

#if	( defined(__PWM_CHANNEL_F_1__) && defined(EE_AVR8_TCF1_OVF_ISR) )
#if	( EE_AVR8_TCF1_OVF_ISR_CAT == 1 )
ISR1(EE_AVR8_TCF1_OVF_ISR)
#else
ISR2(EE_AVR8_TCF1_OVF_ISR)
#endif
{
#if	0
  EE_Pwm_ISR( EE_PWM_CHANNEL_F_1 );
#endif
  EE_Tc_Hw_GetCallback(EE_PWM_CHANNEL_F_1)();
}
#endif	/* __PWM_CHANNEL_F_1__ && EE_AVR8_TCF1_OVF_ISR */

#if	( \
	defined(__PWM_CHANNEL_F_1__) &&	\
	defined(__PWM_F_1_CCA__) &&	\
	defined(EE_AVR8_TCF1_CCA_ISR)	\
)
#if	( EE_AVR8_TCF1_CCA_ISR_CAT == 1 )
ISR1(EE_AVR8_TCF1_CCA_ISR)
#else
ISR2(EE_AVR8_TCF1_CCA_ISR)
#endif
{
#if	0
  EE_Pwm_ISR( EE_PWM_CHANNEL_F_1 );
#endif
  EE_Tc_Hw_GetCallback(EE_PWM_CHANNEL_F_1)();
}
#endif	/* __PWM_CHANNEL_F_1__ && __PWM_F_1_CCA__ && EE_AVR8_TCF1_CCA_ISR */

#if	( \
	defined(__PWM_CHANNEL_F_1__) &&	\
	defined(__PWM_F_1_CCB__) &&	\
	defined(EE_AVR8_TCF1_CCB_ISR)	\
)
#if	( EE_AVR8_TCF1_CCB_ISR_CAT == 1 )
ISR1(EE_AVR8_TCF1_CCB_ISR)
#else
ISR2(EE_AVR8_TCF1_CCB_ISR)
#endif
{
#if	0
  EE_Pwm_ISR( EE_PWM_CHANNEL_F_1 );
#endif
  EE_Tc_Hw_GetCallback(EE_PWM_CHANNEL_F_1)();
}
#endif	/* __PWM_CHANNEL_F_1__ && __PWM_F_1_CCB__ && EE_AVR8_TCF1_CCB_ISR */

#endif	/* __AVR_ATxmega16D4__ || __AVR_ATxmega32D4__ */

#endif	/*
	 *  (
	 *    defined(__PWM_CHANNEL_C_0__) &&
	 *    (
	 *      defined(EE_AVR8_TCC0_OVF_ISR) ||
	 *      ( defined(__PWM_C_0_CCA__) && defined(EE_AVR8_TCC0_CCA_ISR) ) ||
	 *      ( defined(__PWM_C_0_CCB__) && defined(EE_AVR8_TCC0_CCB_ISR) ) ||
	 *      ( defined(__PWM_C_0_CCC__) && defined(EE_AVR8_TCC0_CCC_ISR) ) ||
	 *      ( defined(__PWM_C_0_CCD__) && defined(EE_AVR8_TCC0_CCD_ISR) )
	 *    )
	 *  ) || (
	 *    defined(__PWM_CHANNEL_D_0__) &&
	 *    (
	 *      defined(EE_AVR8_TCD0_OVF_ISR) ||
	 *      ( defined(__PWM_D_0_CCA__) && defined(EE_AVR8_TCD0_CCA_ISR) ) ||
	 *      ( defined(__PWM_D_0_CCB__) && defined(EE_AVR8_TCD0_CCB_ISR) ) ||
	 *      ( defined(__PWM_D_0_CCC__) && defined(EE_AVR8_TCD0_CCC_ISR) ) ||
	 *      ( defined(__PWM_D_0_CCD__) && defined(EE_AVR8_TCD0_CCD_ISR) )
	 *    )
	 *  ) || (
	 *    defined(__PWM_CHANNEL_E_0__) &&
	 *    (
	 *      defined(EE_AVR8_TCE0_OVF_ISR) ||
	 *      ( defined(__PWM_E_0_CCA__) && defined(EE_AVR8_TCE0_CCA_ISR) ) ||
	 *      ( defined(__PWM_E_0_CCB__) && defined(EE_AVR8_TCE0_CCB_ISR) ) ||
	 *      ( defined(__PWM_E_0_CCC__) && defined(EE_AVR8_TCE0_CCC_ISR) ) ||
	 *      ( defined(__PWM_E_0_CCD__) && defined(EE_AVR8_TCE0_CCD_ISR) )
	 *     )
	 *  ) || (
	 *    defined(__PWM_CHANNEL_F_0__) &&
	 *    (
	 *      defined(EE_AVR8_TCF0_OVF_ISR) ||
	 *      ( defined(__PWM_F_0_CCA__) && defined(EE_AVR8_TCF0_CCA_ISR) ) ||
	 *      ( defined(__PWM_F_0_CCB__) && defined(EE_AVR8_TCF0_CCB_ISR) ) ||
	 *      ( defined(__PWM_F_0_CCC__) && defined(EE_AVR8_TCF0_CCC_ISR) ) ||
	 *      ( defined(__PWM_F_0_CCD__) && defined(EE_AVR8_TCF0_CCD_ISR) )
	 *     )
	 *  ) || (
	 *    defined(__PWM_CHANNEL_C_1__) &&
	 *    (
	 *      defined(EE_AVR8_TCC1_OVF_ISR) ||
	 *      ( defined(__PWM_C_1_CCA__) && defined(EE_AVR8_TCC1_CCA_ISR) ) ||
	 *      ( defined(__PWM_C_1_CCB__) && defined(EE_AVR8_TCC1_CCB_ISR) )
	 *    )
	 *  ) || (
	 *    defined(__PWM_CHANNEL_D_1__) &&
	 *    (
	 *      defined(EE_AVR8_TCD1_OVF_ISR) ||
	 *      ( defined(__PWM_D_1_CCA__) && defined(EE_AVR8_TCD1_CCA_ISR) ) ||
	 *      ( defined(__PWM_D_1_CCB__) && defined(EE_AVR8_TCD1_CCB_ISR) )
	 *    )
	 *  ) || (
	 *    defined(__PWM_CHANNEL_E_1__) &&
	 *    (
	 *      defined(EE_AVR8_TCE1_OVF_ISR) ||
	 *      ( defined(__PWM_E_1_CCA__) && defined(EE_AVR8_TCE1_CCA_ISR) ) ||
	 *      ( defined(__PWM_E_1_CCB__) && defined(EE_AVR8_TCE1_CCB_ISR) )
	 *     )
	 *  ) || (
	 *    defined(__PWM_CHANNEL_F_1__) &&
	 *    (
	 *      defined(EE_AVR8_TCF1_OVF_ISR) ||
	 *      ( defined(__PWM_F_1_CCA__) && defined(EE_AVR8_TCF1_CCA_ISR) ) ||
	 *      ( defined(__PWM_F_1_CCB__) && defined(EE_AVR8_TCF1_CCB_ISR) )
	 *     )
	 *  )
	 */

#else	/* __USE_TC_HW__ */

#if	( \
  ( \
    defined(__PWM_CHANNEL_C_0__) && \
    ( \
      defined(EE_AVR8_TCC0_OVF_ISR) ||	\
      ( defined(__PWM_C_0_CCA__) && defined(EE_AVR8_TCC0_CCA_ISR) ) || \
      ( defined(__PWM_C_0_CCB__) && defined(EE_AVR8_TCC0_CCB_ISR) ) || \
      ( defined(__PWM_C_0_CCC__) && defined(EE_AVR8_TCC0_CCC_ISR) ) || \
      ( defined(__PWM_C_0_CCD__) && defined(EE_AVR8_TCC0_CCD_ISR) ) \
    ) \
  ) || \
  ( \
    defined(__PWM_CHANNEL_D_0__) && \
    ( \
      defined(EE_AVR8_TCD0_OVF_ISR) || \
      ( defined(__PWM_D_0_CCA__) && defined(EE_AVR8_TCD0_CCA_ISR) ) || \
      ( defined(__PWM_D_0_CCB__) && defined(EE_AVR8_TCD0_CCB_ISR) ) || \
      ( defined(__PWM_D_0_CCC__) && defined(EE_AVR8_TCD0_CCC_ISR) ) || \
      ( defined(__PWM_D_0_CCD__) && defined(EE_AVR8_TCD0_CCD_ISR) ) \
    ) \
  ) || \
  ( \
    defined(__PWM_CHANNEL_E_0__) && \
    ( \
      defined(EE_AVR8_TCE0_OVF_ISR) || \
      ( defined(__PWM_E_0_CCA__) && defined(EE_AVR8_TCE0_CCA_ISR) ) || \
      ( defined(__PWM_E_0_CCB__) && defined(EE_AVR8_TCE0_CCB_ISR) ) || \
      ( defined(__PWM_E_0_CCC__) && defined(EE_AVR8_TCE0_CCC_ISR) ) || \
      ( defined(__PWM_E_0_CCD__) && defined(EE_AVR8_TCE0_CCD_ISR) ) \
    ) \
  ) \
)

/*
 * PWM IRQ(s) Priority Array.
 */
#ifdef	DEBUG
volatile const EE_UREG
#else
const EE_UREG
#endif
#ifdef	__AVR_PGMSPACE__
EE_Pwm_Ccx_Pri[EE_TC_HW_UNITS_NUMBER] PROGMEM = {
#else	/* __AVR_PGMSPACE__ */
EE_Pwm_Ccx_Pri[EE_TC_HW_UNITS_NUMBER] = {
#endif	/* __AVR_PGMSPACE__ */

/* CHANNEL C 0 */
#ifdef	__PWM_CHANNEL_C_0__
#if	defined(__PWM_C_0_CCA__)
#ifdef	EE_AVR8_TCC0_CCA_ISR_PRI
EE_TC_HW_IRQ_CCA_PRI(EE_AVR8_TCC0_CCA_ISR_PRI),
#else
EE_ISR_PRI_OFF,
#endif
#elif	defined(__PWM_C_0_CCB__)
#ifdef	EE_AVR8_TCC0_CCB_ISR_PRI
EE_TC_HW_IRQ_CCB_PRI(EE_AVR8_TCC0_CCB_ISR_PRI),
#else
EE_ISR_PRI_OFF,
#endif
#elif	defined(__PWM_C_0_CCC__)
#ifdef	EE_AVR8_TCC0_CCC_ISR_PRI
EE_TC_HW_IRQ_CCC_PRI(EE_AVR8_TCC0_CCC_ISR_PRI),
#else
EE_ISR_PRI_OFF,
#endif
#elif	defined(__PWM_C_0_CCD__)
#ifdef	EE_AVR8_TCC0_CCD_ISR_PRI
EE_TC_HW_IRQ_CCD_PRI(EE_AVR8_TCC0_CCD_ISR_PRI),
#else
EE_ISR_PRI_OFF,
#endif
#else
EE_ISR_PRI_OFF,
#endif
#else	/* __PWM_CHANNEL_C_0__ */
EE_ISR_PRI_OFF,
#endif	/* __PWM_CHANNEL_C_0__ */

/* CHANNEL D 0 */
#ifdef __PWM_CHANNEL_D_0__
#if	defined(__PWM_D_0_CCA__)
#ifdef	EE_AVR8_TCD0_CCA_ISR_PRI
EE_TC_HW_IRQ_CCA_PRI(EE_AVR8_TCD0_CCA_ISR_PRI),
#else
EE_ISR_PRI_OFF,
#endif
#elif	defined(__PWM_D_0_CCB__)
#ifdef	EE_AVR8_TCD0_CCB_ISR_PRI
EE_TC_HW_IRQ_CCB_PRI(EE_AVR8_TCD0_CCB_ISR_PRI),
#else
EE_ISR_PRI_OFF,
#endif
#elif	defined(__PWM_D_0_CCC__)
#ifdef	EE_AVR8_TCD0_CCC_ISR_PRI
EE_TC_HW_IRQ_CCC_PRI(EE_AVR8_TCD0_CCC_ISR_PRI),
#else
EE_ISR_PRI_OFF,
#endif
#elif	defined(__PWM_D_0_CCD__)
#ifdef	EE_AVR8_TCD0_CCD_ISR_PRI
EE_TC_HW_IRQ_CCD_PRI(EE_AVR8_TCD0_CCD_ISR_PRI),
#else
EE_ISR_PRI_OFF,
#endif
#else
EE_ISR_PRI_OFF,
#endif
#else	/* __PWM_CHANNEL_D_0__ */
EE_ISR_PRI_OFF,
#endif	/* __PWM_CHANNEL_D_0__ */

/* CHANNEL E 0 */
#ifdef __PWM_CHANNEL_E_0__
#if	defined(__PWM_E_0_CCA__)
#ifdef	EE_AVR8_TCE0_CCA_ISR_PRI
EE_TC_HW_IRQ_CCA_PRI(EE_AVR8_TCE0_CCA_ISR_PRI),
#else
EE_ISR_PRI_OFF,
#endif
#elif	defined(__PWM_E_0_CCB__)
#ifdef	EE_AVR8_TCE0_CCB_ISR_PRI
EE_TC_HW_IRQ_CCB_PRI(EE_AVR8_TCE0_CCB_ISR_PRI),
#else
EE_ISR_PRI_OFF,
#endif
#elif	defined(__PWM_E_0_CCC__)
#ifdef	EE_AVR8_TCE0_CCC_ISR_PRI
EE_TC_HW_IRQ_CCC_PRI(EE_AVR8_TCE0_CCC_ISR_PRI),
#else
EE_ISR_PRI_OFF,
#endif
#elif	defined(__PWM_E_0_CCD__)
#ifdef	EE_AVR8_TCE0_CCD_ISR_PRI
EE_TC_HW_IRQ_CCD_PRI(EE_AVR8_TCE0_CCD_ISR_PRI),
#else
EE_ISR_PRI_OFF,
#endif
#else
EE_ISR_PRI_OFF,
#endif
#else	/* __PWM_CHANNEL_E_0__ */
EE_ISR_PRI_OFF,
#endif	/* __PWM_CHANNEL_E_0__ */

};

#if	0
/* PWM ISR
 *
 * param[in]	Channel		PWM Channel Identifier.
 */
static void
EE_Pwm_ISR( EE_TYPEPWMCHANNEL Channel )
{

#ifdef	DEBUG
  volatile EE_UREG		flags;
  volatile EE_ISR_callback	cb;
#else	/* DEBUG */
  register EE_UREG		flags;
  register EE_ISR_callback	cb;
#endif	/* DEBUG */

  /* Suspend Interrupt */
  flags = EE_hal_suspendIRQ();

  cb = EE_Tc_Hw_CallBack[Channel];

  /* Resume Interrupt */
  EE_hal_resumeIRQ(flags);

  /* Notification Enabled Check!!! */
  if ( cb != EE_NULL_PTR ) {

    /* Call Call-Back. */
    (*cb)();

  }

}	/* EE_Pwm_ISR( EE_TYPEPWMCHANNEL Channel ) */
#endif	/* 0 */

#if	( defined(__PWM_CHANNEL_C_0__) && defined(EE_AVR8_TCC0_OVF_ISR) )
#if	( EE_AVR8_TCC0_OVF_ISR_CAT == 1 )
ISR1(EE_AVR8_TCC0_OVF_ISR)
#else
ISR2(EE_AVR8_TCC0_OVF_ISR)
#endif
{
#if	0
  EE_Pwm_ISR( EE_PWM_CHANNEL_C_0 );
#endif
  (EE_Tc_Hw_CallBack[EE_PWM_CHANNEL_C_0])();
}
#endif	/* __PWM_CHANNEL_C_0__ && EE_AVR8_TCC0_OVF_ISR */

#if	( \
	defined(__PWM_CHANNEL_C_0__) &&	\
	defined(__PWM_C_0_CCA__) &&	\
	defined(EE_AVR8_TCC0_CCA_ISR)	\
)
#if	( EE_AVR8_TCC0_CCA_ISR_CAT == 1 )
ISR1(EE_AVR8_TCC0_CCA_ISR)
#else
ISR2(EE_AVR8_TCC0_CCA_ISR)
#endif
{
#if	0
  EE_Pwm_ISR( EE_PWM_CHANNEL_C_0 );
#endif
  (EE_Tc_Hw_CallBack[EE_PWM_CHANNEL_C_0])();
}
#endif	/* __PWM_CHANNEL_C_0__ && __PWM_C_0_CCA__ && EE_AVR8_TCC0_CCA_ISR */

#if	( \
	defined(__PWM_CHANNEL_C_0__) &&	\
	defined(__PWM_C_0_CCB__) &&	\
	defined(EE_AVR8_TCC0_CCB_ISR)	\
)
#if	( EE_AVR8_TCC0_CCB_ISR_CAT == 1 )
ISR1(EE_AVR8_TCC0_CCB_ISR)
#else
ISR2(EE_AVR8_TCC0_CCB_ISR)
#endif
{
#if	0
  EE_Pwm_ISR( EE_PWM_CHANNEL_C_0 );
#endif
  (EE_Tc_Hw_CallBack[EE_PWM_CHANNEL_C_0])();
}
#endif	/* __PWM_CHANNEL_C_0__ && __PWM_C_0_CCB__ && EE_AVR8_TCC0_CCB_ISR */

#if	( \
	defined(__PWM_CHANNEL_C_0__) &&	\
	defined(__PWM_C_0_CCC__) &&	\
	defined(EE_AVR8_TCC0_CCC_ISR)	\
)
#if	( EE_AVR8_TCC0_CCC_ISR_CAT == 1 )
ISR1(EE_AVR8_TCC0_CCC_ISR)
#else
ISR2(EE_AVR8_TCC0_CCC_ISR)ù
#endif
{
#if	0
  EE_Pwm_ISR( EE_PWM_CHANNEL_C_0 );
#endif
  (EE_Tc_Hw_CallBack[EE_PWM_CHANNEL_C_0])();
}
#endif	/* __PWM_CHANNEL_C_0__ && __PWM_C_0_CCC__ && EE_AVR8_TCC0_CCC_ISR */

#if	( \
	defined(__PWM_CHANNEL_C_0__) &&	\
	defined(__PWM_C_0_CCD__) &&	\
	defined(EE_AVR8_TCC0_CCD_ISR)	\
)
#if	( EE_AVR8_TCC0_CCD_ISR_CAT == 1 )
ISR1(EE_AVR8_TCC0_CCD_ISR)
#else
ISR2(EE_AVR8_TCC0_CCD_ISR)
#endif
{
#if	0
  EE_Pwm_ISR( EE_PWM_CHANNEL_C_0 );
#endif
  (EE_Tc_Hw_CallBack[EE_PWM_CHANNEL_C_0])();
}
#endif	/* __PWM_CHANNEL_C_0__ && __PWM_C_0_CCD__ && EE_AVR8_TCC0_CCD_ISR */

#if	( defined(__PWM_CHANNEL_D_0__) && defined(EE_AVR8_TCD0_OVF_ISR) )
#if	( EE_AVR8_TCD0_OVF_ISR_CAT == 1 )
ISR1(EE_AVR8_TCD0_OVF_ISR)
#else
ISR2(EE_AVR8_TCD0_OVF_ISR)
#endif
{
#if	0
  EE_Pwm_ISR( EE_PWM_CHANNEL_D_0 );
#endif
  (EE_Tc_Hw_CallBack[EE_PWM_CHANNEL_D_0])();
}
#endif	/* __PWM_CHANNEL_D_0__ && EE_AVR8_TCD0_OVF_ISR */

#if	( \
	defined(__PWM_CHANNEL_D_0__) &&	\
	defined(__PWM_D_0_CCA__) &&	\
	defined(EE_AVR8_TCD0_CCA_ISR)	\
)
#if	( EE_AVR8_TCD0_CCA_ISR_CAT == 1 )
ISR1(EE_AVR8_TCD0_CCA_ISR)
#else
ISR2(EE_AVR8_TCD0_CCA_ISR)
#endif
{
#if	0
  EE_Pwm_ISR( EE_PWM_CHANNEL_D_0 );
#endif
  (EE_Tc_Hw_CallBack[EE_PWM_CHANNEL_D_0])();
}
#endif	/* __PWM_CHANNEL_D_0__ && __PWM_D_0_CCA__ && EE_AVR8_TCD0_CCA_ISR */

#if	( \
	defined(__PWM_CHANNEL_D_0__) &&	\
	defined(__PWM_D_0_CCB__) &&	\
	defined(EE_AVR8_TCD0_CCB_ISR)	\
)
#if	( EE_AVR8_TCD0_CCB_ISR_CAT == 1 )
ISR1(EE_AVR8_TCD0_CCB_ISR)
#else
ISR2(EE_AVR8_TCD0_CCB_ISR)
#endif
{
#if	0
  EE_Pwm_ISR( EE_PWM_CHANNEL_D_0 );
#endif
  (EE_Tc_Hw_CallBack[EE_PWM_CHANNEL_D_0])();
}
#endif	/* __PWM_CHANNEL_D_0__ && __PWM_D_0_CCB__ && EE_AVR8_TCD0_CCB_ISR */

#if	( \
	defined(__PWM_CHANNEL_D_0__) &&	\
	defined(__PWM_D_0_CCC__) &&	\
	defined(EE_AVR8_TCD0_CCC_ISR)	\
)
#if	( EE_AVR8_TCD0_CCC_ISR_CAT == 1 )
ISR1(EE_AVR8_TCD0_CCC_ISR)
#else
ISR2(EE_AVR8_TCD0_CCC_ISR)
#endif
{
#if	0
  EE_Pwm_ISR( EE_PWM_CHANNEL_D_0 );
#endif
  (EE_Tc_Hw_CallBack[EE_PWM_CHANNEL_D_0])();
}
#endif	/* __PWM_CHANNEL_D_0__ && __PWM_D_0_CCC__ && EE_AVR8_TCD0_CCC_ISR */

#if	( \
	defined(__PWM_CHANNEL_D_0__) &&	\
	defined(__PWM_D_0_CCD__) &&	\
	defined(EE_AVR8_TCD0_CCD_ISR)	\
)
#if	( EE_AVR8_TCD0_CCD_ISR_CAT == 1 )
ISR1(EE_AVR8_TCD0_CCD_ISR)
#else
ISR2(EE_AVR8_TCD0_CCD_ISR)
#endif
{
#if	0
  EE_Pwm_ISR( EE_PWM_CHANNEL_D_0 );
#endif
  (EE_Tc_Hw_CallBack[EE_PWM_CHANNEL_D_0])();
}
#endif	/* __PWM_CHANNEL_D_0__ && __PWM_D_0_CCD__ && EE_AVR8_TCD0_CCD_ISR */

#if	( defined(__PWM_CHANNEL_E_0__) && defined(EE_AVR8_TCE0_OVF_ISR) )
#if	( EE_AVR8_TCE0_OVF_ISR_CAT == 1 )
ISR1(EE_AVR8_TCE0_OVF_ISR)
#else
ISR2(EE_AVR8_TCE0_OVF_ISR)
#endif
{
#if	0
  EE_Pwm_ISR( EE_PWM_CHANNEL_E_0 );
#endif
  (EE_Tc_Hw_CallBack[EE_PWM_CHANNEL_E_0])();
}
#endif	/* __PWM_CHANNEL_E_0__ && EE_AVR8_TCE0_OVF_ISR */

#if	( \
	defined(__PWM_CHANNEL_E_0__) &&	\
	defined(__PWM_E_0_CCA__) &&	\
	defined(EE_AVR8_TCE0_CCA_ISR)	\
)
#if	( EE_AVR8_TCE0_CCA_ISR_CAT == 1 )
ISR1(EE_AVR8_TCE0_CCA_ISR)
#else
ISR2(EE_AVR8_TCE0_CCA_ISR)
#endif
{
#if	0
  EE_Pwm_ISR( EE_PWM_CHANNEL_E_0 );
#endif
  (EE_Tc_Hw_CallBack[EE_PWM_CHANNEL_E_0])();
}
#endif	/* __PWM_CHANNEL_E_0__ && __PWM_E_0_CCA__ && EE_AVR8_TCE0_CCA_ISR */

#if	( \
	defined(__PWM_CHANNEL_E_0__) &&	\
	defined(__PWM_E_0_CCB__) &&	\
	defined(EE_AVR8_TCE0_CCB_ISR)	\
)
#if	( EE_AVR8_TCE0_CCB_ISR_CAT == 1 )
ISR1(EE_AVR8_TCE0_CCB_ISR)
#else
ISR2(EE_AVR8_TCE0_CCB_ISR)
#endif
{
#if	0
  EE_Pwm_ISR( EE_PWM_CHANNEL_E_0 );
#endif
  (EE_Tc_Hw_CallBack[EE_PWM_CHANNEL_E_0])();
}
#endif	/* __PWM_CHANNEL_E_0__ && __PWM_E_0_CCB__ && EE_AVR8_TCE0_CCB_ISR */

#if	( \
	defined(__PWM_CHANNEL_E_0__) &&	\
	defined(__PWM_E_0_CCC__) &&	\
	defined(EE_AVR8_TCE0_CCC_ISR)	\
)
#if	( EE_AVR8_TCE0_CCC_ISR_CAT == 1 )
ISR1(EE_AVR8_TCE0_CCC_ISR)
#else
ISR2(EE_AVR8_TCE0_CCC_ISR)
#endif

{
#if	0
  EE_Pwm_ISR( EE_PWM_CHANNEL_E_0 );
#endif
  (EE_Tc_Hw_CallBack[EE_PWM_CHANNEL_E_0])();
}
#endif	/* __PWM_CHANNEL_E_0__ && __PWM_E_0_CCC__ && EE_AVR8_TCE0_CCC_ISR */

#if	( \
	defined(__PWM_CHANNEL_E_0__) &&	\
	defined(__PWM_E_0_CCD__) &&	\
	defined(EE_AVR8_TCE0_CCD_ISR)	\
)
#if	( EE_AVR8_TCE0_CCD_ISR_CAT == 1 )
ISR1(EE_AVR8_TCE0_CCD_ISR)
#else
ISR2(EE_AVR8_TCE0_CCD_ISR)
#endif
{
#if	0
  EE_Pwm_ISR( EE_PWM_CHANNEL_E_0 );
#endif
  (EE_Tc_Hw_CallBack[EE_PWM_CHANNEL_E_0])();
}
#endif	/* __PWM_CHANNEL_E_0__ && __PWM_E_0_CCD__ && EE_AVR8_TCE0_CCD_ISR */

#endif	/*
	 *  (
	 *    defined(__PWM_CHANNEL_C_0__) &&
	 *    (
	 *      defined(EE_AVR8_TCC0_OVF_ISR) ||
	 *      ( defined(__PWM_C_0_CCA__) && defined(EE_AVR8_TCC0_CCA_ISR) ) ||
	 *      ( defined(__PWM_C_0_CCB__) && defined(EE_AVR8_TCC0_CCB_ISR) ) ||
	 *      ( defined(__PWM_C_0_CCC__) && defined(EE_AVR8_TCC0_CCC_ISR) ) ||
	 *      ( defined(__PWM_C_0_CCD__) && defined(EE_AVR8_TCC0_CCD_ISR) )
	 *    )
	 *  ) || (
	 *    defined(__PWM_CHANNEL_D_0__) &&
	 *    (
	 *      defined(EE_AVR8_TCD0_OVF_ISR) ||
	 *      ( defined(__PWM_D_0_CCA__) && defined(EE_AVR8_TCD0_CCA_ISR) ) ||
	 *      ( defined(__PWM_D_0_CCB__) && defined(EE_AVR8_TCD0_CCB_ISR) ) ||
	 *      ( defined(__PWM_D_0_CCC__) && defined(EE_AVR8_TCD0_CCC_ISR) ) ||
	 *      ( defined(__PWM_D_0_CCD__) && defined(EE_AVR8_TCD0_CCD_ISR) )
	 *    )
	 *  ) || (
	 *    defined(__PWM_CHANNEL_E_0__) &&
	 *    (
	 *      defined(EE_AVR8_TCE0_OVF_ISR) ||
	 *      ( defined(__PWM_E_0_CCA__) && defined(EE_AVR8_TCE0_CCA_ISR) ) ||
	 *      ( defined(__PWM_E_0_CCB__) && defined(EE_AVR8_TCE0_CCB_ISR) ) ||
	 *      ( defined(__PWM_E_0_CCC__) && defined(EE_AVR8_TCE0_CCC_ISR) ) ||
	 *      ( defined(__PWM_E_0_CCD__) && defined(EE_AVR8_TCE0_CCD_ISR) )
	 *     )
	 *  )
	 */

#endif	/* __USE_TC_HW__ */

#endif	/* __PWM_ENABLENOTIF_API__ */

#ifdef	__PWM_GETSTATUS_API__
/*
 * EE_Pwm_GetStatus() Implementation.
 */
EE_TYPEPWMCHSTAT
EE_Pwm_GetStatus(
  EE_TYPEPWMCHANNEL	Channel
) {

#ifdef	DEBUG
  volatile EE_UREG	flags;
  volatile EE_UINT16	period;
  volatile EE_HWREG_PTR	addr;
#else
  register EE_UREG	flags;
  register EE_UINT16	period;
  register EE_HWREG_PTR	addr;
#endif

#ifdef	__PWM_EXTENDED_STATUS__

  /* Channel Validation */
  EE_ATXMEGA_VALIDATE_W_RV( 
#ifdef	__USE_TC_HW__
    ( Channel < EE_TC_HW_ALL_UNITS_MASK ),
#else	/* __USE_TC_HW__ */
    ( Channel < EE_TC_HW_ALL_TCX0_MASK ),
#endif	/* __USE_TC_HW__ */
    EE_PWM_CH_UNINIT
  );

#endif	/* __PWM_EXTENDED_STATUS__ */

  /* Suspend Interrupt */
  flags = EE_hal_suspendIRQ();

  /* Channel Initialization Validation */
  EE_ATXMEGA_VALIDATE_IRQ_W_RV(
#ifdef	__USE_TC_HW__
    ( EE_Tc_Hw_GetStatus(Channel) & EE_TC_HW_INIT_MASK ),
#else	/* __USE_TC_HW__ */
    ( EE_Tc_Hw_Status[Channel] & EE_TC_HW_INIT_MASK ),
#endif	/* __USE_TC_HW__ */
    EE_PWM_CH_UNINIT,
    flags
  );

#ifdef	__USE_TC_HW__
  addr = EE_TC_HW_2_ADDR(Channel);
#else	/* __USE_TC_HW__ */
  addr = EE_TC_HW_TCX0_2_ADDR(Channel);
#endif	/* __USE_TC_HW__ */

  /* Channel Running Validation */
  EE_ATXMEGA_VALIDATE_IRQ_W_RV(
#ifdef	__USE_TC_HW__
    ( EE_HWREG(addr + EE_TC_HW_CTRLB_REG_OFS) == TC_CLKSEL_OFF_gc ),
#else	/* __USE_TC_HW__ */
    ( EE_HWREG(addr + EE_TC_HW_TCX0_CTRLB_REG_OFS) == TC_CLKSEL_OFF_gc ),
#endif	/* __USE_TC_HW__ */
    EE_PWM_CH_RUNNING,
    flags
  );

#ifdef	__USE_TC_HW__
  period = EE_ATxmega_Reg16Read(addr + EE_TC_HW_PER_REG_OFS);
#else	/* __USE_TC_HW__ */
  period = EE_ATxmega_Reg16Read(addr + EE_TC_HW_TCX0_PER_REG_OFS);
#endif	/* __USE_TC_HW__ */

  /* Channel Stopped Validation */
  EE_ATXMEGA_VALIDATE_IRQ_W_RV(
    ( 
      ( Period < EE_PWM_ABS_PERIOD_MIN ) ||
      ( Period > EE_PWM_ABS_PERIOD_MAX )
    ),
    EE_PWM_CH_STOPPED,
    flags
  );

  /* Resume Interrupt */
  EE_hal_resumeIRQ(flags);

  return EE_PWM_CH_OPERATIONAL;

};
#endif	/* __PWM_GETSTATUS_API__ */

/*
 * EE_Pwm_Init() Implementation.
 */
#ifdef	__PWM_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_Pwm_Init(
  EE_TYPEPWMCHANNEL	Channel,
  EE_TYPEPWMFLAGS	Flags,
  EE_ISR_callback	NotifCB
) {

#ifdef	__USE_TC_HW__

#ifdef	DEBUG
  volatile EE_UREG	flags;
  volatile EE_UREG	stat;
  volatile EE_HWREG_PTR	addr;
#else
  register EE_UREG	flags;
  register EE_UREG	stat;
  register EE_HWREG_PTR	addr;
#endif

#ifdef	__PWM_EXTENDED_STATUS__

  /* Channel Validation */
  EE_ATXMEGA_VALIDATE_W_RV( 
    ( Channel < EE_TC_HW_ALL_UNITS_MASK ),
    E_PWM_PARAM_CHANNEL
  );

  /* Flags Validation. */
  EE_ATXMEGA_VALIDATE_W_RV(
    ( !( Flags & ~EE_PWM_FLAG_ALL_MASK ) ),
    E_PWM_PARAM_FLAGS
  );

  /* Flags Validation. */
  EE_ATXMEGA_VALIDATE_W_RV(
    ( Flags & EE_PWM_FLAG_POL_HIGH ),
    E_PWM_PARAM_FLAGS
  );

#endif	/* __PWM_EXTENDED_STATUS__ */

  /* Suspend Interrupt */
  flags = EE_hal_suspendIRQ();

  stat = EE_Tc_Hw_GetStatus(Channel);

#ifdef	__PWM_EXTENDED_STATUS__

  EE_ATXMEGA_VALIDATE_IRQ_W_RV(!( stat & EE_TC_HW_INIT_MASK ), E_NOT_OK, flags);

#endif	/* __PWM_EXTENDED_STATUS__ */

  stat |= EE_TC_HW_INIT_MASK;

  if ( Flags & EE_PWM_FLAG_PER_FIX ) {

    stat |= EE_PWM_FP_MASK;

  }

  if ( Flags & EE_PWM_FLAG_PER_END_UP ) {

    stat |= EE_PWM_PEU_MASK;

  }

  if ( Flags & EE_PWM_FLAG_DUTY_END_UP ) {

    stat |= EE_PWM_DEU_MASK;

  }

  if ( Flags & EE_PWM_FLAG_IDLE_HIGH ) {

    stat |= EE_PWM_IH_MASK;

  }

  if ( Flags & EE_PWM_FLAG_POL_HIGH ) {

    stat |= EE_PWM_PH_MASK;

  }

  addr = EE_TC_HW_2_ADDR(Channel);

#if	0
  EE_HWREG(addr + EE_TC_HW_CTRLB_REG_OFS) = TC_WGMODE_SS_gc;
#endif

#ifdef	__PWM_ENABLENOTIF_API__
#if	( \
  ( \
    defined(__PWM_CHANNEL_C_0__) && \
    ( \
      defined(EE_AVR8_TCC0_OVF_ISR) ||	\
      ( defined(__PWM_C_0_CCA__) && defined(EE_AVR8_TCC0_CCA_ISR) ) || \
      ( defined(__PWM_C_0_CCB__) && defined(EE_AVR8_TCC0_CCB_ISR) ) || \
      ( defined(__PWM_C_0_CCC__) && defined(EE_AVR8_TCC0_CCC_ISR) ) || \
      ( defined(__PWM_C_0_CCD__) && defined(EE_AVR8_TCC0_CCD_ISR) ) \
    ) \
  ) || \
  ( \
    defined(__PWM_CHANNEL_D_0__) && \
    ( \
      defined(EE_AVR8_TCD0_OVF_ISR) || \
      ( defined(__PWM_D_0_CCA__) && defined(EE_AVR8_TCD0_CCA_ISR) ) || \
      ( defined(__PWM_D_0_CCB__) && defined(EE_AVR8_TCD0_CCB_ISR) ) || \
      ( defined(__PWM_D_0_CCC__) && defined(EE_AVR8_TCD0_CCC_ISR) ) || \
      ( defined(__PWM_D_0_CCD__) && defined(EE_AVR8_TCD0_CCD_ISR) ) \
    ) \
  ) || \
  ( \
    defined(__PWM_CHANNEL_E_0__) && \
    ( \
      defined(EE_AVR8_TCE0_OVF_ISR) || \
      ( defined(__PWM_E_0_CCA__) && defined(EE_AVR8_TCE0_CCA_ISR) ) || \
      ( defined(__PWM_E_0_CCB__) && defined(EE_AVR8_TCE0_CCB_ISR) ) || \
      ( defined(__PWM_E_0_CCC__) && defined(EE_AVR8_TCE0_CCC_ISR) ) || \
      ( defined(__PWM_E_0_CCD__) && defined(EE_AVR8_TCE0_CCD_ISR) ) \
    ) \
  ) || \
  ( \
    defined(__PWM_CHANNEL_F_0__) && \
    ( \
      defined(EE_AVR8_TCF0_OVF_ISR) || \
      ( defined(__PWM_F_0_CCA__) && defined(EE_AVR8_TCF0_CCA_ISR) ) || \
      ( defined(__PWM_F_0_CCB__) && defined(EE_AVR8_TCF0_CCB_ISR) ) || \
      ( defined(__PWM_F_0_CCC__) && defined(EE_AVR8_TCF0_CCC_ISR) ) || \
      ( defined(__PWM_F_0_CCD__) && defined(EE_AVR8_TCE0_CCD_ISR) ) \
    ) \
  ) || \
  ( \
    defined(__PWM_CHANNEL_C_1__) && \
    ( \
      defined(EE_AVR8_TCC1_OVF_ISR) ||	\
      ( defined(__PWM_C_1_CCA__) && defined(EE_AVR8_TCC1_CCA_ISR) ) || \
      ( defined(__PWM_C_1_CCB__) && defined(EE_AVR8_TCC1_CCB_ISR) ) \
    ) \
  ) || \
  ( \
    defined(__PWM_CHANNEL_D_1__) && \
    ( \
      defined(EE_AVR8_TCD1_OVF_ISR) || \
      ( defined(__PWM_D_1_CCA__) && defined(EE_AVR8_TCD1_CCA_ISR) ) || \
      ( defined(__PWM_D_1_CCB__) && defined(EE_AVR8_TCD1_CCB_ISR) ) \
    ) \
  ) || \
  ( \
    defined(__PWM_CHANNEL_E_1__) && \
    ( \
      defined(EE_AVR8_TCE1_OVF_ISR) || \
      ( defined(__PWM_E_1_CCA__) && defined(EE_AVR8_TCE1_CCA_ISR) ) || \
      ( defined(__PWM_E_1_CCB__) && defined(EE_AVR8_TCE1_CCB_ISR) ) \
    ) \
  ) || \
  ( \
    defined(__PWM_CHANNEL_F_1__) && \
    ( \
      defined(EE_AVR8_TCF1_OVF_ISR) || \
      ( defined(__PWM_F_1_CCA__) && defined(EE_AVR8_TCF1_CCA_ISR) ) || \
      ( defined(__PWM_F_1_CCB__) && defined(EE_AVR8_TCF1_CCB_ISR) ) \
    ) \
  ) \
)
  EE_Tc_Hw_SetCallback(Channel, NotifCB);
#endif	/*
	 *  (
	 *    defined(__PWM_CHANNEL_C_0__) &&
	 *    (
	 *      defined(EE_AVR8_TCC0_OVF_ISR) ||
	 *      ( defined(__PWM_C_0_CCA__) && defined(EE_AVR8_TCC0_CCA_ISR) ) ||
	 *      ( defined(__PWM_C_0_CCB__) && defined(EE_AVR8_TCC0_CCB_ISR) ) ||
	 *      ( defined(__PWM_C_0_CCC__) && defined(EE_AVR8_TCC0_CCC_ISR) ) ||
	 *      ( defined(__PWM_C_0_CCD__) && defined(EE_AVR8_TCC0_CCD_ISR) )
	 *    )
	 *  ) || (
	 *    defined(__PWM_CHANNEL_D_0__) &&
	 *    (
	 *      defined(EE_AVR8_TCD0_OVF_ISR) ||
	 *      ( defined(__PWM_D_0_CCA__) && defined(EE_AVR8_TCD0_CCA_ISR) ) ||
	 *      ( defined(__PWM_D_0_CCB__) && defined(EE_AVR8_TCD0_CCB_ISR) ) ||
	 *      ( defined(__PWM_D_0_CCC__) && defined(EE_AVR8_TCD0_CCC_ISR) ) ||
	 *      ( defined(__PWM_D_0_CCD__) && defined(EE_AVR8_TCD0_CCD_ISR) )
	 *    )
	 *  ) || (
	 *    defined(__PWM_CHANNEL_E_0__) &&
	 *    (
	 *      defined(EE_AVR8_TCE0_OVF_ISR) ||
	 *      ( defined(__PWM_E_0_CCA__) && defined(EE_AVR8_TCE0_CCA_ISR) ) ||
	 *      ( defined(__PWM_E_0_CCB__) && defined(EE_AVR8_TCE0_CCB_ISR) ) ||
	 *      ( defined(__PWM_E_0_CCC__) && defined(EE_AVR8_TCE0_CCC_ISR) ) ||
	 *      ( defined(__PWM_E_0_CCD__) && defined(EE_AVR8_TCE0_CCD_ISR) )
	 *     )
	 *  ) || (
	 *    defined(__PWM_CHANNEL_F_0__) &&
	 *    (
	 *      defined(EE_AVR8_TCF0_OVF_ISR) ||
	 *      ( defined(__PWM_F_0_CCA__) && defined(EE_AVR8_TCF0_CCA_ISR) ) ||
	 *      ( defined(__PWM_F_0_CCB__) && defined(EE_AVR8_TCF0_CCB_ISR) ) ||
	 *      ( defined(__PWM_F_0_CCC__) && defined(EE_AVR8_TCF0_CCC_ISR) ) ||
	 *      ( defined(__PWM_F_0_CCD__) && defined(EE_AVR8_TCF0_CCD_ISR) )
	 *     )
	 *  ) || (
	 *    defined(__PWM_CHANNEL_C_1__) &&
	 *    (
	 *      defined(EE_AVR8_TCC1_OVF_ISR) ||
	 *      ( defined(__PWM_C_1_CCA__) && defined(EE_AVR8_TCC1_CCA_ISR) ) ||
	 *      ( defined(__PWM_C_1_CCB__) && defined(EE_AVR8_TCC1_CCB_ISR) )
	 *    )
	 *  ) || (
	 *    defined(__PWM_CHANNEL_D_1__) &&
	 *    (
	 *      defined(EE_AVR8_TCD1_OVF_ISR) ||
	 *      ( defined(__PWM_D_1_CCA__) && defined(EE_AVR8_TCD1_CCA_ISR) ) ||
	 *      ( defined(__PWM_D_1_CCB__) && defined(EE_AVR8_TCD1_CCB_ISR) )
	 *    )
	 *  ) || (
	 *    defined(__PWM_CHANNEL_E_1__) &&
	 *    (
	 *      defined(EE_AVR8_TCE1_OVF_ISR) ||
	 *      ( defined(__PWM_E_1_CCA__) && defined(EE_AVR8_TCE1_CCA_ISR) ) ||
	 *      ( defined(__PWM_E_1_CCB__) && defined(EE_AVR8_TCE1_CCB_ISR) )
	 *     )
	 *  ) || (
	 *    defined(__PWM_CHANNEL_F_1__) &&
	 *    (
	 *      defined(EE_AVR8_TCF1_OVF_ISR) ||
	 *      ( defined(__PWM_F_1_CCA__) && defined(EE_AVR8_TCF1_CCA_ISR) ) ||
	 *      ( defined(__PWM_F_1_CCB__) && defined(EE_AVR8_TCF1_CCB_ISR) )
	 *     )
	 *  )
	 */
#endif	/* __PWM_ENABLENOTIF_API__ */

  EE_Tc_Hw_SetStatus(Channel, stat);

  /* Resume IRQs */
  EE_hal_resumeIRQ(flags);

#ifdef	__PWM_EXTENDED_STATUS__
  return E_OK;
#endif

#else	/* __USE_TC_HW__ */

#ifdef	DEBUG
  volatile EE_UREG	flags;
  volatile EE_HWREG_PTR	addr;
#else
  register EE_UREG	flags;
  register EE_HWREG_PTR	addr;
#endif

#ifdef	__PWM_EXTENDED_STATUS__

  /* Channel Validation */
  EE_ATXMEGA_VALIDATE_W_RV( 
    ( Channel < EE_TC_HW_ALL_TCX0_MASK ),
    E_PWM_PARAM_CHANNEL
  );

  /* Flags Validation. */
  EE_ATXMEGA_VALIDATE_W_RV(
    ( !( Flags & ~EE_PWM_FLAG_ALL_MASK ) ),
    E_PWM_PARAM_FLAGS
  );

  /* Flags Validation. */
  EE_ATXMEGA_VALIDATE_W_RV(
    ( Flags & EE_PWM_FLAG_POL_HIGH ),
    E_PWM_PARAM_FLAGS
  );

#endif	/* __PWM_EXTENDED_STATUS__ */

  /* Suspend Interrupt */
  flags = EE_hal_suspendIRQ();

#ifdef	__PWM_EXTENDED_STATUS__

  EE_ATXMEGA_VALIDATE_IRQ_W_RV(
    !( EE_Tc_Hw_Status[Channel] & EE_TC_HW_INIT_MASK ),
    E_NOT_OK,
    flags
  );

#endif	/* __PWM_EXTENDED_STATUS__ */

  EE_Tc_Hw_Status[Channel] |= EE_TC_HW_INIT_MASK;

  if ( Flags & EE_PWM_FLAG_PER_FIX ) {

    EE_Tc_Hw_Status[Channel] |= EE_PWM_FP_MASK;

  }

  if ( Flags & EE_PWM_FLAG_PER_END_UP ) {

    EE_Tc_Hw_Status[Channel] |= EE_PWM_PEU_MASK;

  }

  if ( Flags & EE_PWM_FLAG_DUTY_END_UP ) {

    EE_Tc_Hw_Status[Channel] |= EE_PWM_DEU_MASK;

  }

  if ( Flags & EE_PWM_FLAG_IDLE_HIGH ) {

    EE_Tc_Hw_Status[Channel] |= EE_PWM_IH_MASK;

  }

  if ( Flags & EE_PWM_FLAG_POL_HIGH ) {

    EE_Tc_Hw_Status[Channel] |= EE_PWM_PH_MASK;

  }

  addr = EE_TC_HW_TCX0_2_ADDR(Channel);

#if	0
  EE_HWREG(addr + EE_TC_HW_TCX0_CTRLB_REG_OFS) = TC_WGMODE_SS_gc;
#endif

#ifdef	__PWM_ENABLENOTIF_API__
#if	( \
  ( \
    defined(__PWM_CHANNEL_C_0__) && \
    ( \
      defined(EE_AVR8_TCC0_OVF_ISR) ||	\
      ( defined(__PWM_C_0_CCA__) && defined(EE_AVR8_TCC0_CCA_ISR) ) || \
      ( defined(__PWM_C_0_CCB__) && defined(EE_AVR8_TCC0_CCB_ISR) ) || \
      ( defined(__PWM_C_0_CCC__) && defined(EE_AVR8_TCC0_CCC_ISR) ) || \
      ( defined(__PWM_C_0_CCD__) && defined(EE_AVR8_TCC0_CCD_ISR) ) \
    ) \
  ) || \
  ( \
    defined(__PWM_CHANNEL_D_0__) && \
    ( \
      defined(EE_AVR8_TCD0_OVF_ISR) || \
      ( defined(__PWM_D_0_CCA__) && defined(EE_AVR8_TCD0_CCA_ISR) ) || \
      ( defined(__PWM_D_0_CCB__) && defined(EE_AVR8_TCD0_CCB_ISR) ) || \
      ( defined(__PWM_D_0_CCC__) && defined(EE_AVR8_TCD0_CCC_ISR) ) || \
      ( defined(__PWM_D_0_CCD__) && defined(EE_AVR8_TCD0_CCD_ISR) ) \
    ) \
  ) || \
  ( \
    defined(__PWM_CHANNEL_E_0__) && \
    ( \
      defined(EE_AVR8_TCE0_OVF_ISR) || \
      ( defined(__PWM_E_0_CCA__) && defined(EE_AVR8_TCE0_CCA_ISR) ) || \
      ( defined(__PWM_E_0_CCB__) && defined(EE_AVR8_TCE0_CCB_ISR) ) || \
      ( defined(__PWM_E_0_CCC__) && defined(EE_AVR8_TCE0_CCC_ISR) ) || \
      ( defined(__PWM_E_0_CCD__) && defined(EE_AVR8_TCE0_CCD_ISR) ) \
    ) \
  ) \
)
  EE_Tc_Hw_CallBack[Channel] = NotifCB;
#endif	/*
	 *  (
	 *    defined(__PWM_CHANNEL_C_0__) &&
	 *    (
	 *      defined(EE_AVR8_TCC0_OVF_ISR) ||
	 *      ( defined(__PWM_C_0_CCA__) && defined(EE_AVR8_TCC0_CCA_ISR) ) ||
	 *      ( defined(__PWM_C_0_CCB__) && defined(EE_AVR8_TCC0_CCB_ISR) ) ||
	 *      ( defined(__PWM_C_0_CCC__) && defined(EE_AVR8_TCC0_CCC_ISR) ) ||
	 *      ( defined(__PWM_C_0_CCD__) && defined(EE_AVR8_TCC0_CCD_ISR) )
	 *    )
	 *  ) || (
	 *    defined(__PWM_CHANNEL_D_0__) &&
	 *    (
	 *      defined(EE_AVR8_TCD0_OVF_ISR) ||
	 *      ( defined(__PWM_D_0_CCA__) && defined(EE_AVR8_TCD0_CCA_ISR) ) ||
	 *      ( defined(__PWM_D_0_CCB__) && defined(EE_AVR8_TCD0_CCB_ISR) ) ||
	 *      ( defined(__PWM_D_0_CCC__) && defined(EE_AVR8_TCD0_CCC_ISR) ) ||
	 *      ( defined(__PWM_D_0_CCD__) && defined(EE_AVR8_TCD0_CCD_ISR) )
	 *    )
	 *  ) || (
	 *    defined(__PWM_CHANNEL_E_0__) &&
	 *    (
	 *      defined(EE_AVR8_TCE0_OVF_ISR) ||
	 *      ( defined(__PWM_E_0_CCA__) && defined(EE_AVR8_TCE0_CCA_ISR) ) ||
	 *      ( defined(__PWM_E_0_CCB__) && defined(EE_AVR8_TCE0_CCB_ISR) ) ||
	 *      ( defined(__PWM_E_0_CCC__) && defined(EE_AVR8_TCE0_CCC_ISR) ) ||
	 *      ( defined(__PWM_E_0_CCD__) && defined(EE_AVR8_TCE0_CCD_ISR) )
	 *     )
	 *  )
	 */
#endif	/* __PWM_ENABLENOTIF_API__ */

  /* Resume IRQs */
  EE_hal_resumeIRQ(flags);

#ifdef	__PWM_EXTENDED_STATUS__
  return E_OK;
#endif

#endif	/* __USE_TC_HW__ */

}	/* EE_Pwm_Init() */

#ifdef	__PWM_DEINIT_API__
/*
 * EE_Pwm_DeInit() Implementation.
 */
#ifdef	__PWM_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_Pwm_DeInit(
  EE_TYPEPWMCHANNEL	Channel,
  EE_TYPEPWMFLAGS	Flags
) {

#ifdef	__USE_TC_HW__

#ifdef	DEBUG
  volatile EE_UREG	flags;
  volatile EE_UREG	stat;
  volatile EE_HWREG_PTR	addr;
#else
  register EE_UREG	flags;
  register EE_UREG	stat;
  register EE_HWREG_PTR	addr;
#endif

#ifdef	__PWM_EXTENDED_STATUS__

  /* Channel Validation */
  EE_ATXMEGA_VALIDATE_W_RV( 
    ( Channel < EE_TC_HW_ALL_UNITS_MASK ),
    E_PWM_PARAM_CHANNEL
  );

  /* Flags Validation. */
  EE_ATXMEGA_VALIDATE_W_RV(
    ( Flags == EE_PWM_FLAG_DEFAULT ),
    E_PWM_PARAM_FLAGS
  );

#endif	/* __PWM_EXTENDED_STATUS__ */

  /* Suspend Interrupt */
  flags = EE_hal_suspendIRQ();

  stat = EE_Tc_Hw_GetStatus(Channel);

#ifdef	__PWM_EXTENDED_STATUS__

  EE_ATXMEGA_VALIDATE_IRQ_W_RV(( stat & EE_TC_HW_INIT_MASK ), E_NOT_OK, flags);

#endif	/* __PWM_EXTENDED_STATUS__ */

  addr = EE_TC_HW_2_ADDR(Channel);

#ifdef	__PWM_EXTENDED_STATUS__

  EE_ATXMEGA_VALIDATE_IRQ_W_RV(
    ( EE_HWREG(addr + EE_TC_HW_CTRLA_REG_OFS) == TC_CLKSEL_OFF_gc ),
    E_PWM_CHANNEL_BUSY,
    flags
  );

#endif	/* __PWM_EXTENDED_STATUS__ */

  stat &= ~( 
    EE_TC_HW_INIT_MASK	|
    EE_PWM_FP_MASK	|
    EE_PWM_DEU_MASK	|
    EE_PWM_PEU_MASK	|
    EE_PWM_IH_MASK	|
    EE_PWM_PH_MASK
  );

  EE_HWREG(addr + EE_TC_HW_CTRLB_REG_OFS) = TC_WGMODE_NORMAL_gc;

#if	0
  EE_ATxmega_Reg16Write( addr + EE_TC_HW_CNT_REG_OFS, 0x0000U );
  EE_ATxmega_Reg16Write( addr + EE_TC_HW_PER_REG_OFS, 0xFFFFU );
  EE_ATxmega_Reg16Write( addr + EE_TC_HW_PERBUF_REG_OFS, 0xFFFFU );
  EE_ATxmega_Reg16Write( addr + EE_TC_HW_CCA_REG_OFS, 0x0000U );
  EE_ATxmega_Reg16Write( addr + EE_TC_HW_CCABUF_REG_OFS, 0x0000U );
  EE_ATxmega_Reg16Write( addr + EE_TC_HW_CCB_REG_OFS, 0x0000U );
  EE_ATxmega_Reg16Write( addr + EE_TC_HW_CCBBUF_REG_OFS, 0x0000U );
  EE_ATxmega_Reg16Write( addr + EE_TC_HW_CCC_REG_OFS, 0x0000U );
  EE_ATxmega_Reg16Write( addr + EE_TC_HW_CCCBUF_REG_OFS, 0x0000U );
  EE_ATxmega_Reg16Write( addr + EE_TC_HW_CCD_REG_OFS, 0x0000U );
  EE_ATxmega_Reg16Write( addr + EE_TC_HW_CCDBUF_REG_OFS, 0x0000U );
#endif

#ifdef	__PWM_ENABLENOTIF_API__
#if	( \
  ( \
    defined(__PWM_CHANNEL_C_0__) && \
    ( \
      defined(EE_AVR8_TCC0_OVF_ISR) ||	\
      ( defined(__PWM_C_0_CCA__) && defined(EE_AVR8_TCC0_CCA_ISR) ) || \
      ( defined(__PWM_C_0_CCB__) && defined(EE_AVR8_TCC0_CCB_ISR) ) || \
      ( defined(__PWM_C_0_CCC__) && defined(EE_AVR8_TCC0_CCC_ISR) ) || \
      ( defined(__PWM_C_0_CCD__) && defined(EE_AVR8_TCC0_CCD_ISR) ) \
    ) \
  ) || \
  ( \
    defined(__PWM_CHANNEL_D_0__) && \
    ( \
      defined(EE_AVR8_TCD0_OVF_ISR) || \
      ( defined(__PWM_D_0_CCA__) && defined(EE_AVR8_TCD0_CCA_ISR) ) || \
      ( defined(__PWM_D_0_CCB__) && defined(EE_AVR8_TCD0_CCB_ISR) ) || \
      ( defined(__PWM_D_0_CCC__) && defined(EE_AVR8_TCD0_CCC_ISR) ) || \
      ( defined(__PWM_D_0_CCD__) && defined(EE_AVR8_TCD0_CCD_ISR) ) \
    ) \
  ) || \
  ( \
    defined(__PWM_CHANNEL_E_0__) && \
    ( \
      defined(EE_AVR8_TCE0_OVF_ISR) || \
      ( defined(__PWM_E_0_CCA__) && defined(EE_AVR8_TCE0_CCA_ISR) ) || \
      ( defined(__PWM_E_0_CCB__) && defined(EE_AVR8_TCE0_CCB_ISR) ) || \
      ( defined(__PWM_E_0_CCC__) && defined(EE_AVR8_TCE0_CCC_ISR) ) || \
      ( defined(__PWM_E_0_CCD__) && defined(EE_AVR8_TCE0_CCD_ISR) ) \
    ) \
  ) || \
  ( \
    defined(__PWM_CHANNEL_F_0__) && \
    ( \
      defined(EE_AVR8_TCF0_OVF_ISR) || \
      ( defined(__PWM_F_0_CCA__) && defined(EE_AVR8_TCF0_CCA_ISR) ) || \
      ( defined(__PWM_F_0_CCB__) && defined(EE_AVR8_TCF0_CCB_ISR) ) || \
      ( defined(__PWM_F_0_CCC__) && defined(EE_AVR8_TCF0_CCC_ISR) ) || \
      ( defined(__PWM_F_0_CCD__) && defined(EE_AVR8_TCE0_CCD_ISR) ) \
    ) \
  ) || \
  ( \
    defined(__PWM_CHANNEL_C_1__) && \
    ( \
      defined(EE_AVR8_TCC1_OVF_ISR) ||	\
      ( defined(__PWM_C_1_CCA__) && defined(EE_AVR8_TCC1_CCA_ISR) ) || \
      ( defined(__PWM_C_1_CCB__) && defined(EE_AVR8_TCC1_CCB_ISR) ) \
    ) \
  ) || \
  ( \
    defined(__PWM_CHANNEL_D_1__) && \
    ( \
      defined(EE_AVR8_TCD1_OVF_ISR) || \
      ( defined(__PWM_D_1_CCA__) && defined(EE_AVR8_TCD1_CCA_ISR) ) || \
      ( defined(__PWM_D_1_CCB__) && defined(EE_AVR8_TCD1_CCB_ISR) ) \
    ) \
  ) || \
  ( \
    defined(__PWM_CHANNEL_E_1__) && \
    ( \
      defined(EE_AVR8_TCE1_OVF_ISR) || \
      ( defined(__PWM_E_1_CCA__) && defined(EE_AVR8_TCE1_CCA_ISR) ) || \
      ( defined(__PWM_E_1_CCB__) && defined(EE_AVR8_TCE1_CCB_ISR) ) \
    ) \
  ) || \
  ( \
    defined(__PWM_CHANNEL_F_1__) && \
    ( \
      defined(EE_AVR8_TCF1_OVF_ISR) || \
      ( defined(__PWM_F_1_CCA__) && defined(EE_AVR8_TCF1_CCA_ISR) ) || \
      ( defined(__PWM_F_1_CCB__) && defined(EE_AVR8_TCF1_CCB_ISR) ) \
    ) \
  ) \
)
  EE_Tc_Hw_SetCallback(Channel, EE_NULL_PTR);

  EE_HWREG(addr + EE_TC_HW_INTCTRLA_REG_OFS) = EE_OFF;

  EE_HWREG(addr + EE_TC_HW_INTCTRLB_REG_OFS) = EE_OFF;

  EE_HWREG(addr + EE_TC_HW_INTFLAGS_REG_OFS) = EE_TC_HW_ALL_INT_MASK;

#endif	/*
	 *    defined(__PWM_CHANNEL_C_0__) &&
	 *    (
	 *      defined(EE_AVR8_TCC0_OVF_ISR) ||
	 *      ( defined(__PWM_C_0_CCA__) && defined(EE_AVR8_TCC0_CCA_ISR) ) ||
	 *      ( defined(__PWM_C_0_CCB__) && defined(EE_AVR8_TCC0_CCB_ISR) ) ||
	 *      ( defined(__PWM_C_0_CCC__) && defined(EE_AVR8_TCC0_CCC_ISR) ) ||
	 *      ( defined(__PWM_C_0_CCD__) && defined(EE_AVR8_TCC0_CCD_ISR) )
	 *    )
	 *  ) || (
	 *    defined(__PWM_CHANNEL_D_0__) &&
	 *    (
	 *      defined(EE_AVR8_TCD0_OVF_ISR) ||
	 *      ( defined(__PWM_D_0_CCA__) && defined(EE_AVR8_TCD0_CCA_ISR) ) ||
	 *      ( defined(__PWM_D_0_CCB__) && defined(EE_AVR8_TCD0_CCB_ISR) ) ||
	 *      ( defined(__PWM_D_0_CCC__) && defined(EE_AVR8_TCD0_CCC_ISR) ) ||
	 *      ( defined(__PWM_D_0_CCD__) && defined(EE_AVR8_TCD0_CCD_ISR) )
	 *    )
	 *  ) || (
	 *    defined(__PWM_CHANNEL_E_0__) &&
	 *    (
	 *      defined(EE_AVR8_TCE0_OVF_ISR) ||
	 *      ( defined(__PWM_E_0_CCA__) && defined(EE_AVR8_TCE0_CCA_ISR) ) ||
	 *      ( defined(__PWM_E_0_CCB__) && defined(EE_AVR8_TCE0_CCB_ISR) ) ||
	 *      ( defined(__PWM_E_0_CCC__) && defined(EE_AVR8_TCE0_CCC_ISR) ) ||
	 *      ( defined(__PWM_E_0_CCD__) && defined(EE_AVR8_TCE0_CCD_ISR) )
	 *     )
	 *  ) || (
	 *    defined(__PWM_CHANNEL_F_0__) &&
	 *    (
	 *      defined(EE_AVR8_TCF0_OVF_ISR) ||
	 *      ( defined(__PWM_F_0_CCA__) && defined(EE_AVR8_TCF0_CCA_ISR) ) ||
	 *      ( defined(__PWM_F_0_CCB__) && defined(EE_AVR8_TCF0_CCB_ISR) ) ||
	 *      ( defined(__PWM_F_0_CCC__) && defined(EE_AVR8_TCF0_CCC_ISR) ) ||
	 *      ( defined(__PWM_F_0_CCD__) && defined(EE_AVR8_TCF0_CCD_ISR) )
	 *     )
	 *  ) || (
	 *    defined(__PWM_CHANNEL_C_1__) &&
	 *    (
	 *      defined(EE_AVR8_TCC1_OVF_ISR) ||
	 *      ( defined(__PWM_C_1_CCA__) && defined(EE_AVR8_TCC1_CCA_ISR) ) ||
	 *      ( defined(__PWM_C_1_CCB__) && defined(EE_AVR8_TCC1_CCB_ISR) )
	 *    )
	 *  ) || (
	 *    defined(__PWM_CHANNEL_D_1__) &&
	 *    (
	 *      defined(EE_AVR8_TCD1_OVF_ISR) ||
	 *      ( defined(__PWM_D_1_CCA__) && defined(EE_AVR8_TCD1_CCA_ISR) ) ||
	 *      ( defined(__PWM_D_1_CCB__) && defined(EE_AVR8_TCD1_CCB_ISR) )
	 *    )
	 *  ) || (
	 *    defined(__PWM_CHANNEL_E_1__) &&
	 *    (
	 *      defined(EE_AVR8_TCE1_OVF_ISR) ||
	 *      ( defined(__PWM_E_1_CCA__) && defined(EE_AVR8_TCE1_CCA_ISR) ) ||
	 *      ( defined(__PWM_E_1_CCB__) && defined(EE_AVR8_TCE1_CCB_ISR) )
	 *     )
	 *  ) || (
	 *    defined(__PWM_CHANNEL_F_1__) &&
	 *    (
	 *      defined(EE_AVR8_TCF1_OVF_ISR) ||
	 *      ( defined(__PWM_F_1_CCA__) && defined(EE_AVR8_TCF1_CCA_ISR) ) ||
	 *      ( defined(__PWM_F_1_CCB__) && defined(EE_AVR8_TCF1_CCB_ISR) )
	 *     )
	 *  )
	 */
#endif	/* __PWM_ENABLENOTIF_API__ */

  EE_Tc_Hw_SetStatus(Channel, stat);

  /* Resume IRQs */
  EE_hal_resumeIRQ(flags);

#ifdef	__PWM_EXTENDED_STATUS__
  return E_OK;
#endif

#else	/* __USE_TC_HW__ */

#ifdef	DEBUG
  volatile EE_UREG	flags;
  volatile EE_HWREG_PTR	addr;
#else
  register EE_UREG	flags;
  register EE_HWREG_PTR	addr;
#endif

#ifdef	__PWM_EXTENDED_STATUS__

  /* Channel Validation */
  EE_ATXMEGA_VALIDATE_W_RV( 
    ( Channel < EE_TC_HW_ALL_TCX0_MASK ),
    E_PWM_PARAM_CHANNEL
  );

  /* Flags Validation. */
  EE_ATXMEGA_VALIDATE_W_RV(
    ( Flags == EE_PWM_FLAG_DEFAULT ),
    E_PWM_PARAM_FLAGS
  );

#endif	/* __PWM_EXTENDED_STATUS__ */

  /* Suspend Interrupt */
  flags = EE_hal_suspendIRQ();

#ifdef	__PWM_EXTENDED_STATUS__

  EE_ATXMEGA_VALIDATE_IRQ_W_RV(
    ( EE_Tc_Hw_Status[Channel] & EE_TC_HW_INIT_MASK ),
    E_NOT_OK,
    flags
  );

#endif	/* __PWM_EXTENDED_STATUS__ */

  addr = EE_TC_HW_TCX0_2_ADDR(Channel);

#ifdef	__PWM_EXTENDED_STATUS__

  EE_ATXMEGA_VALIDATE_IRQ_W_RV(
    ( EE_HWREG(addr + EE_TC_HW_TCX0_CTRLA_REG_OFS) == TC_CLKSEL_OFF_gc ),
    E_PWM_CHANNEL_BUSY,
    flags
  );

#endif	/* __PWM_EXTENDED_STATUS__ */

  EE_Tc_Hw_Status[Channel] &= ~( 
    EE_TC_HW_INIT_MASK	|
    EE_PWM_FP_MASK	|
    EE_PWM_DEU_MASK	|
    EE_PWM_PEU_MASK	|
    EE_PWM_IH_MASK	|
    EE_PWM_PH_MASK
  );

  EE_HWREG(addr + EE_TC_HW_TCX0_CTRLB_REG_OFS) = TC_WGMODE_NORMAL_gc;

#if	0
  EE_ATxmega_Reg16Write( addr + EE_TC_HW_TCX0_CNT_REG_OFS, 0x0000U );
  EE_ATxmega_Reg16Write( addr + EE_TC_HW_TCX0_PER_REG_OFS, 0xFFFFU );
  EE_ATxmega_Reg16Write( addr + EE_TC_HW_TCX0_PERBUF_REG_OFS, 0xFFFFU );
  EE_ATxmega_Reg16Write( addr + EE_TC_HW_TCX0_CCA_REG_OFS, 0x0000U );
  EE_ATxmega_Reg16Write( addr + EE_TC_HW_TCX0_CCABUF_REG_OFS, 0x0000U );
  EE_ATxmega_Reg16Write( addr + EE_TC_HW_TCX0_CCB_REG_OFS, 0x0000U );
  EE_ATxmega_Reg16Write( addr + EE_TC_HW_TCX0_CCBBUF_REG_OFS, 0x0000U );
  EE_ATxmega_Reg16Write( addr + EE_TC_HW_TCX0_CCC_REG_OFS, 0x0000U );
  EE_ATxmega_Reg16Write( addr + EE_TC_HW_TCX0_CCCBUF_REG_OFS, 0x0000U );
  EE_ATxmega_Reg16Write( addr + EE_TC_HW_TCX0_CCD_REG_OFS, 0x0000U );
  EE_ATxmega_Reg16Write( addr + EE_TC_HW_TCX0_CCDBUF_REG_OFS, 0x0000U );
#endif

#ifdef	__PWM_ENABLENOTIF_API__
#if	( \
  ( \
    defined(__PWM_CHANNEL_C_0__) && \
    ( \
      defined(EE_AVR8_TCC0_OVF_ISR) ||	\
      ( defined(__PWM_C_0_CCA__) && defined(EE_AVR8_TCC0_CCA_ISR) ) || \
      ( defined(__PWM_C_0_CCB__) && defined(EE_AVR8_TCC0_CCB_ISR) ) || \
      ( defined(__PWM_C_0_CCC__) && defined(EE_AVR8_TCC0_CCC_ISR) ) || \
      ( defined(__PWM_C_0_CCD__) && defined(EE_AVR8_TCC0_CCD_ISR) ) \
    ) \
  ) || \
  ( \
    defined(__PWM_CHANNEL_D_0__) && \
    ( \
      defined(EE_AVR8_TCD0_OVF_ISR) || \
      ( defined(__PWM_D_0_CCA__) && defined(EE_AVR8_TCD0_CCA_ISR) ) || \
      ( defined(__PWM_D_0_CCB__) && defined(EE_AVR8_TCD0_CCB_ISR) ) || \
      ( defined(__PWM_D_0_CCC__) && defined(EE_AVR8_TCD0_CCC_ISR) ) || \
      ( defined(__PWM_D_0_CCD__) && defined(EE_AVR8_TCD0_CCD_ISR) ) \
    ) \
  ) || \
  ( \
    defined(__PWM_CHANNEL_E_0__) && \
    ( \
      defined(EE_AVR8_TCE0_OVF_ISR) || \
      ( defined(__PWM_E_0_CCA__) && defined(EE_AVR8_TCE0_CCA_ISR) ) || \
      ( defined(__PWM_E_0_CCB__) && defined(EE_AVR8_TCE0_CCB_ISR) ) || \
      ( defined(__PWM_E_0_CCC__) && defined(EE_AVR8_TCE0_CCC_ISR) ) || \
      ( defined(__PWM_E_0_CCD__) && defined(EE_AVR8_TCE0_CCD_ISR) ) \
    ) \
  ) \
)
  EE_Tc_Hw_CallBack[Channel] = EE_NULL_PTR;

  EE_HWREG(addr + EE_TC_HW_TCX0_INTCTRLA_REG_OFS) = EE_OFF;

  EE_HWREG(addr + EE_TC_HW_TCX0_INTCTRLB_REG_OFS) = EE_OFF;

  EE_HWREG(addr + EE_TC_HW_TCX0_INTFLAGS_REG_OFS) = EE_TC_HW_ALL_INT_MASK;

#endif	/*
	 *  (
	 *    defined(__PWM_CHANNEL_C_0__) &&
	 *    (
	 *      defined(EE_AVR8_TCC0_OVF_ISR) ||
	 *      ( defined(__PWM_C_0_CCA__) && defined(EE_AVR8_TCC0_CCA_ISR) ) ||
	 *      ( defined(__PWM_C_0_CCB__) && defined(EE_AVR8_TCC0_CCB_ISR) ) ||
	 *      ( defined(__PWM_C_0_CCC__) && defined(EE_AVR8_TCC0_CCC_ISR) ) ||
	 *      ( defined(__PWM_C_0_CCD__) && defined(EE_AVR8_TCC0_CCD_ISR) )
	 *    )
	 *  ) || (
	 *    defined(__PWM_CHANNEL_D_0__) &&
	 *    (
	 *      defined(EE_AVR8_TCD0_OVF_ISR) ||
	 *      ( defined(__PWM_D_0_CCA__) && defined(EE_AVR8_TCD0_CCA_ISR) ) ||
	 *      ( defined(__PWM_D_0_CCB__) && defined(EE_AVR8_TCD0_CCB_ISR) ) ||
	 *      ( defined(__PWM_D_0_CCC__) && defined(EE_AVR8_TCD0_CCC_ISR) ) ||
	 *      ( defined(__PWM_D_0_CCD__) && defined(EE_AVR8_TCD0_CCD_ISR) )
	 *    )
	 *  ) || (
	 *    defined(__PWM_CHANNEL_E_0__) &&
	 *    (
	 *      defined(EE_AVR8_TCE0_OVF_ISR) ||
	 *      ( defined(__PWM_E_0_CCA__) && defined(EE_AVR8_TCE0_CCA_ISR) ) ||
	 *      ( defined(__PWM_E_0_CCB__) && defined(EE_AVR8_TCE0_CCB_ISR) ) ||
	 *      ( defined(__PWM_E_0_CCC__) && defined(EE_AVR8_TCE0_CCC_ISR) ) ||
	 *      ( defined(__PWM_E_0_CCD__) && defined(EE_AVR8_TCE0_CCD_ISR) )
	 *     )
	 *  )
	 */
#endif	/* __PWM_ENABLENOTIF_API__ */

  /* Resume IRQs */
  EE_hal_resumeIRQ(flags);

#ifdef	__PWM_EXTENDED_STATUS__
  return E_OK;
#endif

#endif	/* __USE_TC_HW__ */

}
#endif	/* __PWM_DEINIT_API__ */


#ifdef	__PWM_ENABLENOTIF_API__
/*
 * EE_Pwm_EnableNotification() Implementation.
 */
#ifdef	__PWM_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_Pwm_EnableNotification(
  EE_TYPEPWMCHANNEL	Channel,
  EE_TYPEPWMEDGENOTIF	Notification
) {

#ifdef	__USE_TC_HW__

#ifdef	DEBUG
  volatile EE_UREG	flags;
  volatile EE_UREG	stat;
  volatile EE_HWREG_PTR	addr;
#else
  register EE_UREG	flags;
  register EE_UREG	stat;
  register EE_HWREG_PTR	addr;
#endif

#ifdef	__PWM_EXTENDED_STATUS__

  /* Channel Validation */
  EE_ATXMEGA_VALIDATE_W_RV( 
    ( Channel < EE_TC_HW_ALL_INITS_MASK ),
    E_PWM_PARAM_CHANNEL
  );

  /* Notification Validation */
  EE_ATXMEGA_VALIDATE_W_RV( 
    (
      ( Notification == EE_PWM_RISING_EDGE )	||
      ( Notification == EE_PWM_FALLING_EDGE )	||
      ( Notification == EE_PWM_BOTH_EDGES )
    ),
    E_PWM_PARAM_EDGE
  );

#endif	/* __PWM_EXTENDED_STATUS__ */

  /* Suspend Interrupt */
  flags = EE_hal_suspendIRQ();

  stat = EE_Tc_Hw_GetStatus(Channel);

#ifdef	__PWM_EXTENDED_STATUS__

  EE_ATXMEGA_VALIDATE_IRQ_W_RV(( stat & EE_TC_HW_INIT_MASK ), E_NOT_OK, flags);

#if	( \
  ( \
    defined(__PWM_CHANNEL_C_0__) && \
    ( \
      defined(EE_AVR8_TCC0_OVF_ISR) ||	\
      ( defined(__PWM_C_0_CCA__) && defined(EE_AVR8_TCC0_CCA_ISR) ) || \
      ( defined(__PWM_C_0_CCB__) && defined(EE_AVR8_TCC0_CCB_ISR) ) || \
      ( defined(__PWM_C_0_CCC__) && defined(EE_AVR8_TCC0_CCC_ISR) ) || \
      ( defined(__PWM_C_0_CCD__) && defined(EE_AVR8_TCC0_CCD_ISR) ) \
    ) \
  ) || \
  ( \
    defined(__PWM_CHANNEL_D_0__) && \
    ( \
      defined(EE_AVR8_TCD0_OVF_ISR) || \
      ( defined(__PWM_D_0_CCA__) && defined(EE_AVR8_TCD0_CCA_ISR) ) || \
      ( defined(__PWM_D_0_CCB__) && defined(EE_AVR8_TCD0_CCB_ISR) ) || \
      ( defined(__PWM_D_0_CCC__) && defined(EE_AVR8_TCD0_CCC_ISR) ) || \
      ( defined(__PWM_D_0_CCD__) && defined(EE_AVR8_TCD0_CCD_ISR) ) \
    ) \
  ) || \
  ( \
    defined(__PWM_CHANNEL_E_0__) && \
    ( \
      defined(EE_AVR8_TCE0_OVF_ISR) || \
      ( defined(__PWM_E_0_CCA__) && defined(EE_AVR8_TCE0_CCA_ISR) ) || \
      ( defined(__PWM_E_0_CCB__) && defined(EE_AVR8_TCE0_CCB_ISR) ) || \
      ( defined(__PWM_E_0_CCC__) && defined(EE_AVR8_TCE0_CCC_ISR) ) || \
      ( defined(__PWM_E_0_CCD__) && defined(EE_AVR8_TCE0_CCD_ISR) ) \
    ) \
  ) || \
  ( \
    defined(__PWM_CHANNEL_F_0__) && \
    ( \
      defined(EE_AVR8_TCF0_OVF_ISR) || \
      ( defined(__PWM_F_0_CCA__) && defined(EE_AVR8_TCF0_CCA_ISR) ) || \
      ( defined(__PWM_F_0_CCB__) && defined(EE_AVR8_TCF0_CCB_ISR) ) || \
      ( defined(__PWM_F_0_CCC__) && defined(EE_AVR8_TCF0_CCC_ISR) ) || \
      ( defined(__PWM_F_0_CCD__) && defined(EE_AVR8_TCE0_CCD_ISR) ) \
    ) \
  ) || \
  ( \
    defined(__PWM_CHANNEL_C_1__) && \
    ( \
      defined(EE_AVR8_TCC1_OVF_ISR) ||	\
      ( defined(__PWM_C_1_CCA__) && defined(EE_AVR8_TCC1_CCA_ISR) ) || \
      ( defined(__PWM_C_1_CCB__) && defined(EE_AVR8_TCC1_CCB_ISR) ) \
    ) \
  ) || \
  ( \
    defined(__PWM_CHANNEL_D_1__) && \
    ( \
      defined(EE_AVR8_TCD1_OVF_ISR) || \
      ( defined(__PWM_D_1_CCA__) && defined(EE_AVR8_TCD1_CCA_ISR) ) || \
      ( defined(__PWM_D_1_CCB__) && defined(EE_AVR8_TCD1_CCB_ISR) ) \
    ) \
  ) || \
  ( \
    defined(__PWM_CHANNEL_E_1__) && \
    ( \
      defined(EE_AVR8_TCE1_OVF_ISR) || \
      ( defined(__PWM_E_1_CCA__) && defined(EE_AVR8_TCE1_CCA_ISR) ) || \
      ( defined(__PWM_E_1_CCB__) && defined(EE_AVR8_TCE1_CCB_ISR) ) \
    ) \
  ) || \
  ( \
    defined(__PWM_CHANNEL_F_1__) && \
    ( \
      defined(EE_AVR8_TCF1_OVF_ISR) || \
      ( defined(__PWM_F_1_CCA__) && defined(EE_AVR8_TCF1_CCA_ISR) ) || \
      ( defined(__PWM_F_1_CCB__) && defined(EE_AVR8_TCF1_CCB_ISR) ) \
    ) \
  ) \
)

  /* Call-Back Validation. */
  EE_ATXMEGA_VALIDATE_W_RV(
    ( EE_Tc_Hw_GetCallback(Channel) != EE_NULL_PTR ),
    E_PWM_PARAM_CALLBACK
  );

#else

  /* Call-Back Validation. */
  EE_ATXMEGA_VALIDATE_W_RV( EE_FALSE, E_PWM_PARAM_CALLBACK );

#endif	/*
	 *    defined(__PWM_CHANNEL_C_0__) &&
	 *    (
	 *      defined(EE_AVR8_TCC0_OVF_ISR) ||
	 *      ( defined(__PWM_C_0_CCA__) && defined(EE_AVR8_TCC0_CCA_ISR) ) ||
	 *      ( defined(__PWM_C_0_CCB__) && defined(EE_AVR8_TCC0_CCB_ISR) ) ||
	 *      ( defined(__PWM_C_0_CCC__) && defined(EE_AVR8_TCC0_CCC_ISR) ) ||
	 *      ( defined(__PWM_C_0_CCD__) && defined(EE_AVR8_TCC0_CCD_ISR) )
	 *    )
	 *  ) || (
	 *    defined(__PWM_CHANNEL_D_0__) &&
	 *    (
	 *      defined(EE_AVR8_TCD0_OVF_ISR) ||
	 *      ( defined(__PWM_D_0_CCA__) && defined(EE_AVR8_TCD0_CCA_ISR) ) ||
	 *      ( defined(__PWM_D_0_CCB__) && defined(EE_AVR8_TCD0_CCB_ISR) ) ||
	 *      ( defined(__PWM_D_0_CCC__) && defined(EE_AVR8_TCD0_CCC_ISR) ) ||
	 *      ( defined(__PWM_D_0_CCD__) && defined(EE_AVR8_TCD0_CCD_ISR) )
	 *    )
	 *  ) || (
	 *    defined(__PWM_CHANNEL_E_0__) &&
	 *    (
	 *      defined(EE_AVR8_TCE0_OVF_ISR) ||
	 *      ( defined(__PWM_E_0_CCA__) && defined(EE_AVR8_TCE0_CCA_ISR) ) ||
	 *      ( defined(__PWM_E_0_CCB__) && defined(EE_AVR8_TCE0_CCB_ISR) ) ||
	 *      ( defined(__PWM_E_0_CCC__) && defined(EE_AVR8_TCE0_CCC_ISR) ) ||
	 *      ( defined(__PWM_E_0_CCD__) && defined(EE_AVR8_TCE0_CCD_ISR) )
	 *     )
	 *  ) || (
	 *    defined(__PWM_CHANNEL_F_0__) &&
	 *    (
	 *      defined(EE_AVR8_TCF0_OVF_ISR) ||
	 *      ( defined(__PWM_F_0_CCA__) && defined(EE_AVR8_TCF0_CCA_ISR) ) ||
	 *      ( defined(__PWM_F_0_CCB__) && defined(EE_AVR8_TCF0_CCB_ISR) ) ||
	 *      ( defined(__PWM_F_0_CCC__) && defined(EE_AVR8_TCF0_CCC_ISR) ) ||
	 *      ( defined(__PWM_F_0_CCD__) && defined(EE_AVR8_TCF0_CCD_ISR) )
	 *     )
	 *  ) || (
	 *    defined(__PWM_CHANNEL_C_1__) &&
	 *    (
	 *      defined(EE_AVR8_TCC1_OVF_ISR) ||
	 *      ( defined(__PWM_C_1_CCA__) && defined(EE_AVR8_TCC1_CCA_ISR) ) ||
	 *      ( defined(__PWM_C_1_CCB__) && defined(EE_AVR8_TCC1_CCB_ISR) )
	 *    )
	 *  ) || (
	 *    defined(__PWM_CHANNEL_D_1__) &&
	 *    (
	 *      defined(EE_AVR8_TCD1_OVF_ISR) ||
	 *      ( defined(__PWM_D_1_CCA__) && defined(EE_AVR8_TCD1_CCA_ISR) ) ||
	 *      ( defined(__PWM_D_1_CCB__) && defined(EE_AVR8_TCD1_CCB_ISR) )
	 *    )
	 *  ) || (
	 *    defined(__PWM_CHANNEL_E_1__) &&
	 *    (
	 *      defined(EE_AVR8_TCE1_OVF_ISR) ||
	 *      ( defined(__PWM_E_1_CCA__) && defined(EE_AVR8_TCE1_CCA_ISR) ) ||
	 *      ( defined(__PWM_E_1_CCB__) && defined(EE_AVR8_TCE1_CCB_ISR) )
	 *     )
	 *  ) || (
	 *    defined(__PWM_CHANNEL_F_1__) &&
	 *    (
	 *      defined(EE_AVR8_TCF1_OVF_ISR) ||
	 *      ( defined(__PWM_F_1_CCA__) && defined(EE_AVR8_TCF1_CCA_ISR) ) ||
	 *      ( defined(__PWM_F_1_CCB__) && defined(EE_AVR8_TCF1_CCB_ISR) )
	 *     )
	 *  )
	 */
#endif	/* __PWM_EXTENDED_STATUS__ */

#if	( \
  ( \
    defined(__PWM_CHANNEL_C_0__) && \
    ( \
      defined(EE_AVR8_TCC0_OVF_ISR) ||	\
      ( defined(__PWM_C_0_CCA__) && defined(EE_AVR8_TCC0_CCA_ISR) ) || \
      ( defined(__PWM_C_0_CCB__) && defined(EE_AVR8_TCC0_CCB_ISR) ) || \
      ( defined(__PWM_C_0_CCC__) && defined(EE_AVR8_TCC0_CCC_ISR) ) || \
      ( defined(__PWM_C_0_CCD__) && defined(EE_AVR8_TCC0_CCD_ISR) ) \
    ) \
  ) || \
  ( \
    defined(__PWM_CHANNEL_D_0__) && \
    ( \
      defined(EE_AVR8_TCD0_OVF_ISR) || \
      ( defined(__PWM_D_0_CCA__) && defined(EE_AVR8_TCD0_CCA_ISR) ) || \
      ( defined(__PWM_D_0_CCB__) && defined(EE_AVR8_TCD0_CCB_ISR) ) || \
      ( defined(__PWM_D_0_CCC__) && defined(EE_AVR8_TCD0_CCC_ISR) ) || \
      ( defined(__PWM_D_0_CCD__) && defined(EE_AVR8_TCD0_CCD_ISR) ) \
    ) \
  ) || \
  ( \
    defined(__PWM_CHANNEL_E_0__) && \
    ( \
      defined(EE_AVR8_TCE0_OVF_ISR) || \
      ( defined(__PWM_E_0_CCA__) && defined(EE_AVR8_TCE0_CCA_ISR) ) || \
      ( defined(__PWM_E_0_CCB__) && defined(EE_AVR8_TCE0_CCB_ISR) ) || \
      ( defined(__PWM_E_0_CCC__) && defined(EE_AVR8_TCE0_CCC_ISR) ) || \
      ( defined(__PWM_E_0_CCD__) && defined(EE_AVR8_TCE0_CCD_ISR) ) \
    ) \
  ) || \
  ( \
    defined(__PWM_CHANNEL_F_0__) && \
    ( \
      defined(EE_AVR8_TCF0_OVF_ISR) || \
      ( defined(__PWM_F_0_CCA__) && defined(EE_AVR8_TCF0_CCA_ISR) ) || \
      ( defined(__PWM_F_0_CCB__) && defined(EE_AVR8_TCF0_CCB_ISR) ) || \
      ( defined(__PWM_F_0_CCC__) && defined(EE_AVR8_TCF0_CCC_ISR) ) || \
      ( defined(__PWM_F_0_CCD__) && defined(EE_AVR8_TCE0_CCD_ISR) ) \
    ) \
  ) || \
  ( \
    defined(__PWM_CHANNEL_C_1__) && \
    ( \
      defined(EE_AVR8_TCC1_OVF_ISR) ||	\
      ( defined(__PWM_C_1_CCA__) && defined(EE_AVR8_TCC1_CCA_ISR) ) || \
      ( defined(__PWM_C_1_CCB__) && defined(EE_AVR8_TCC1_CCB_ISR) ) \
    ) \
  ) || \
  ( \
    defined(__PWM_CHANNEL_D_1__) && \
    ( \
      defined(EE_AVR8_TCD1_OVF_ISR) || \
      ( defined(__PWM_D_1_CCA__) && defined(EE_AVR8_TCD1_CCA_ISR) ) || \
      ( defined(__PWM_D_1_CCB__) && defined(EE_AVR8_TCD1_CCB_ISR) ) \
    ) \
  ) || \
  ( \
    defined(__PWM_CHANNEL_E_1__) && \
    ( \
      defined(EE_AVR8_TCE1_OVF_ISR) || \
      ( defined(__PWM_E_1_CCA__) && defined(EE_AVR8_TCE1_CCA_ISR) ) || \
      ( defined(__PWM_E_1_CCB__) && defined(EE_AVR8_TCE1_CCB_ISR) ) \
    ) \
  ) || \
  ( \
    defined(__PWM_CHANNEL_F_1__) && \
    ( \
      defined(EE_AVR8_TCF1_OVF_ISR) || \
      ( defined(__PWM_F_1_CCA__) && defined(EE_AVR8_TCF1_CCA_ISR) ) || \
      ( defined(__PWM_F_1_CCB__) && defined(EE_AVR8_TCF1_CCB_ISR) ) \
    ) \
  ) \
)
  addr = EE_TC_HW_2_ADDR(Channel);

  if ( 
    ( Notification == EE_PWM_RISING_EDGE )	||
    ( Notification == EE_PWM_BOTH_EDGES )
  ) {

    EE_HWREG(addr + EE_TC_HW_INTCTRLA_REG_OFS) =
    ( stat & EE_TC_HW_IRQ_OVF_PRI_MASK );

  }

  if ( 
    ( Notification == EE_PWM_FALLING_EDGE )	||
    ( Notification == EE_PWM_BOTH_EDGES )
  ) {

#if	0
    EE_HWREG(addr + EE_TC_HW_INTCTRLB_REG_OFS) =
    ( EE_Pwm_Ccx_GetDiv(Channel) & EE_TC_HW_CCX_EN_MASK );
#else	/* 0 */
    EE_HWREG(addr + EE_TC_HW_INTCTRLB_REG_OFS) = EE_Pwm_Ccx_GetPri(Channel);
#endif	/* 0 */

  }

  EE_HWREG(addr + EE_TC_HW_INTFLAGS_REG_OFS) = EE_TC_HW_ALL_INT_MASK;
#endif	/*
	 *    defined(__PWM_CHANNEL_C_0__) &&
	 *    (
	 *      defined(EE_AVR8_TCC0_OVF_ISR) ||
	 *      ( defined(__PWM_C_0_CCA__) && defined(EE_AVR8_TCC0_CCA_ISR) ) ||
	 *      ( defined(__PWM_C_0_CCB__) && defined(EE_AVR8_TCC0_CCB_ISR) ) ||
	 *      ( defined(__PWM_C_0_CCC__) && defined(EE_AVR8_TCC0_CCC_ISR) ) ||
	 *      ( defined(__PWM_C_0_CCD__) && defined(EE_AVR8_TCC0_CCD_ISR) )
	 *    )
	 *  ) || (
	 *    defined(__PWM_CHANNEL_D_0__) &&
	 *    (
	 *      defined(EE_AVR8_TCD0_OVF_ISR) ||
	 *      ( defined(__PWM_D_0_CCA__) && defined(EE_AVR8_TCD0_CCA_ISR) ) ||
	 *      ( defined(__PWM_D_0_CCB__) && defined(EE_AVR8_TCD0_CCB_ISR) ) ||
	 *      ( defined(__PWM_D_0_CCC__) && defined(EE_AVR8_TCD0_CCC_ISR) ) ||
	 *      ( defined(__PWM_D_0_CCD__) && defined(EE_AVR8_TCD0_CCD_ISR) )
	 *    )
	 *  ) || (
	 *    defined(__PWM_CHANNEL_E_0__) &&
	 *    (
	 *      defined(EE_AVR8_TCE0_OVF_ISR) ||
	 *      ( defined(__PWM_E_0_CCA__) && defined(EE_AVR8_TCE0_CCA_ISR) ) ||
	 *      ( defined(__PWM_E_0_CCB__) && defined(EE_AVR8_TCE0_CCB_ISR) ) ||
	 *      ( defined(__PWM_E_0_CCC__) && defined(EE_AVR8_TCE0_CCC_ISR) ) ||
	 *      ( defined(__PWM_E_0_CCD__) && defined(EE_AVR8_TCE0_CCD_ISR) )
	 *     )
	 *  ) || (
	 *    defined(__PWM_CHANNEL_F_0__) &&
	 *    (
	 *      defined(EE_AVR8_TCF0_OVF_ISR) ||
	 *      ( defined(__PWM_F_0_CCA__) && defined(EE_AVR8_TCF0_CCA_ISR) ) ||
	 *      ( defined(__PWM_F_0_CCB__) && defined(EE_AVR8_TCF0_CCB_ISR) ) ||
	 *      ( defined(__PWM_F_0_CCC__) && defined(EE_AVR8_TCF0_CCC_ISR) ) ||
	 *      ( defined(__PWM_F_0_CCD__) && defined(EE_AVR8_TCF0_CCD_ISR) )
	 *     )
	 *  ) || (
	 *    defined(__PWM_CHANNEL_C_1__) &&
	 *    (
	 *      defined(EE_AVR8_TCC1_OVF_ISR) ||
	 *      ( defined(__PWM_C_1_CCA__) && defined(EE_AVR8_TCC1_CCA_ISR) ) ||
	 *      ( defined(__PWM_C_1_CCB__) && defined(EE_AVR8_TCC1_CCB_ISR) )
	 *    )
	 *  ) || (
	 *    defined(__PWM_CHANNEL_D_1__) &&
	 *    (
	 *      defined(EE_AVR8_TCD1_OVF_ISR) ||
	 *      ( defined(__PWM_D_1_CCA__) && defined(EE_AVR8_TCD1_CCA_ISR) ) ||
	 *      ( defined(__PWM_D_1_CCB__) && defined(EE_AVR8_TCD1_CCB_ISR) )
	 *    )
	 *  ) || (
	 *    defined(__PWM_CHANNEL_E_1__) &&
	 *    (
	 *      defined(EE_AVR8_TCE1_OVF_ISR) ||
	 *      ( defined(__PWM_E_1_CCA__) && defined(EE_AVR8_TCE1_CCA_ISR) ) ||
	 *      ( defined(__PWM_E_1_CCB__) && defined(EE_AVR8_TCE1_CCB_ISR) )
	 *     )
	 *  ) || (
	 *    defined(__PWM_CHANNEL_F_1__) &&
	 *    (
	 *      defined(EE_AVR8_TCF1_OVF_ISR) ||
	 *      ( defined(__PWM_F_1_CCA__) && defined(EE_AVR8_TCF1_CCA_ISR) ) ||
	 *      ( defined(__PWM_F_1_CCB__) && defined(EE_AVR8_TCF1_CCB_ISR) )
	 *     )
	 *  )
	 */

  EE_Tc_Hw_SetStatus(Channel, stat);

  /* Resume IRQs */
  EE_hal_resumeIRQ(flags);

#ifdef	__PWM_EXTENDED_STATUS__
  return E_OK;
#endif

#else	/* __USE_TC_HW__ */

#ifdef	DEBUG
  volatile EE_UREG	flags;
  volatile EE_HWREG_PTR	addr;
#else
  register EE_UREG	flags;
  register EE_HWREG_PTR	addr;
#endif

#ifdef	__PWM_EXTENDED_STATUS__

  /* Channel Validation */
  EE_ATXMEGA_VALIDATE_W_RV( 
    ( Channel < EE_TC_HW_ALL_TCX0_MASK ),
    E_PWM_PARAM_CHANNEL
  );

  /* Notification Validation */
  EE_ATXMEGA_VALIDATE_W_RV( 
    (
      ( Notification == EE_PWM_RISING_EDGE )	||
      ( Notification == EE_PWM_FALLING_EDGE )	||
      ( Notification == EE_PWM_BOTH_EDGES )
    ),
    E_PWM_PARAM_EDGE
  );

#endif	/* __PWM_EXTENDED_STATUS__ */

  /* Suspend Interrupt */
  flags = EE_hal_suspendIRQ();

#ifdef	__PWM_EXTENDED_STATUS__

  EE_ATXMEGA_VALIDATE_IRQ_W_RV(
    ( EE_Tc_Hw_Status[Channel] & EE_TC_HW_INIT_MASK ),
    E_NOT_OK,
    flags
  );

#if	( \
  ( \
    defined(__PWM_CHANNEL_C_0__) && \
    ( \
      defined(EE_AVR8_TCC0_OVF_ISR) ||	\
      ( defined(__PWM_C_0_CCA__) && defined(EE_AVR8_TCC0_CCA_ISR) ) || \
      ( defined(__PWM_C_0_CCB__) && defined(EE_AVR8_TCC0_CCB_ISR) ) || \
      ( defined(__PWM_C_0_CCC__) && defined(EE_AVR8_TCC0_CCC_ISR) ) || \
      ( defined(__PWM_C_0_CCD__) && defined(EE_AVR8_TCC0_CCD_ISR) ) \
    ) \
  ) || \
  ( \
    defined(__PWM_CHANNEL_D_0__) && \
    ( \
      defined(EE_AVR8_TCD0_OVF_ISR) || \
      ( defined(__PWM_D_0_CCA__) && defined(EE_AVR8_TCD0_CCA_ISR) ) || \
      ( defined(__PWM_D_0_CCB__) && defined(EE_AVR8_TCD0_CCB_ISR) ) || \
      ( defined(__PWM_D_0_CCC__) && defined(EE_AVR8_TCD0_CCC_ISR) ) || \
      ( defined(__PWM_D_0_CCD__) && defined(EE_AVR8_TCD0_CCD_ISR) ) \
    ) \
  ) || \
  ( \
    defined(__PWM_CHANNEL_E_0__) && \
    ( \
      defined(EE_AVR8_TCE0_OVF_ISR) || \
      ( defined(__PWM_E_0_CCA__) && defined(EE_AVR8_TCE0_CCA_ISR) ) || \
      ( defined(__PWM_E_0_CCB__) && defined(EE_AVR8_TCE0_CCB_ISR) ) || \
      ( defined(__PWM_E_0_CCC__) && defined(EE_AVR8_TCE0_CCC_ISR) ) || \
      ( defined(__PWM_E_0_CCD__) && defined(EE_AVR8_TCE0_CCD_ISR) ) \
    ) \
  ) \
)

  /* Call-Back Validation. */
  EE_ATXMEGA_VALIDATE_W_RV(
    ( EE_Tc_Hw_CallBack[Channel] != EE_NULL_PTR ),
    E_PWM_PARAM_CALLBACK
  );

#else

  /* Call-Back Validation. */
  EE_ATXMEGA_VALIDATE_W_RV( EE_FALSE, E_PWM_PARAM_CALLBACK );

#endif	/*
	 *  (
	 *    defined(__PWM_CHANNEL_C_0__) &&
	 *    (
	 *      defined(EE_AVR8_TCC0_OVF_ISR) ||
	 *      ( defined(__PWM_C_0_CCA__) && defined(EE_AVR8_TCC0_CCA_ISR) ) ||
	 *      ( defined(__PWM_C_0_CCB__) && defined(EE_AVR8_TCC0_CCB_ISR) ) ||
	 *      ( defined(__PWM_C_0_CCC__) && defined(EE_AVR8_TCC0_CCC_ISR) ) ||
	 *      ( defined(__PWM_C_0_CCD__) && defined(EE_AVR8_TCC0_CCD_ISR) )
	 *    )
	 *  ) || (
	 *    defined(__PWM_CHANNEL_D_0__) &&
	 *    (
	 *      defined(EE_AVR8_TCD0_OVF_ISR) ||
	 *      ( defined(__PWM_D_0_CCA__) && defined(EE_AVR8_TCD0_CCA_ISR) ) ||
	 *      ( defined(__PWM_D_0_CCB__) && defined(EE_AVR8_TCD0_CCB_ISR) ) ||
	 *      ( defined(__PWM_D_0_CCC__) && defined(EE_AVR8_TCD0_CCC_ISR) ) ||
	 *      ( defined(__PWM_D_0_CCD__) && defined(EE_AVR8_TCD0_CCD_ISR) )
	 *    )
	 *  ) || (
	 *    defined(__PWM_CHANNEL_E_0__) &&
	 *    (
	 *      defined(EE_AVR8_TCE0_OVF_ISR) ||
	 *      ( defined(__PWM_E_0_CCA__) && defined(EE_AVR8_TCE0_CCA_ISR) ) ||
	 *      ( defined(__PWM_E_0_CCB__) && defined(EE_AVR8_TCE0_CCB_ISR) ) ||
	 *      ( defined(__PWM_E_0_CCC__) && defined(EE_AVR8_TCE0_CCC_ISR) ) ||
	 *      ( defined(__PWM_E_0_CCD__) && defined(EE_AVR8_TCE0_CCD_ISR) )
	 *     )
	 *  )
	 */
#endif	/* __PWM_EXTENDED_STATUS__ */

#if	( \
  ( \
    defined(__PWM_CHANNEL_C_0__) && \
    ( \
      defined(EE_AVR8_TCC0_OVF_ISR) ||	\
      ( defined(__PWM_C_0_CCA__) && defined(EE_AVR8_TCC0_CCA_ISR) ) || \
      ( defined(__PWM_C_0_CCB__) && defined(EE_AVR8_TCC0_CCB_ISR) ) || \
      ( defined(__PWM_C_0_CCC__) && defined(EE_AVR8_TCC0_CCC_ISR) ) || \
      ( defined(__PWM_C_0_CCD__) && defined(EE_AVR8_TCC0_CCD_ISR) ) \
    ) \
  ) || \
  ( \
    defined(__PWM_CHANNEL_D_0__) && \
    ( \
      defined(EE_AVR8_TCD0_OVF_ISR) || \
      ( defined(__PWM_D_0_CCA__) && defined(EE_AVR8_TCD0_CCA_ISR) ) || \
      ( defined(__PWM_D_0_CCB__) && defined(EE_AVR8_TCD0_CCB_ISR) ) || \
      ( defined(__PWM_D_0_CCC__) && defined(EE_AVR8_TCD0_CCC_ISR) ) || \
      ( defined(__PWM_D_0_CCD__) && defined(EE_AVR8_TCD0_CCD_ISR) ) \
    ) \
  ) || \
  ( \
    defined(__PWM_CHANNEL_E_0__) && \
    ( \
      defined(EE_AVR8_TCE0_OVF_ISR) || \
      ( defined(__PWM_E_0_CCA__) && defined(EE_AVR8_TCE0_CCA_ISR) ) || \
      ( defined(__PWM_E_0_CCB__) && defined(EE_AVR8_TCE0_CCB_ISR) ) || \
      ( defined(__PWM_E_0_CCC__) && defined(EE_AVR8_TCE0_CCC_ISR) ) || \
      ( defined(__PWM_E_0_CCD__) && defined(EE_AVR8_TCE0_CCD_ISR) ) \
    ) \
  ) \
)
  addr = EE_TC_HW_TCX0_2_ADDR(Channel);

  if ( 
    ( Notification == EE_PWM_RISING_EDGE )	||
    ( Notification == EE_PWM_BOTH_EDGES )
  ) {

    EE_HWREG(addr + EE_TC_HW_TCX0_INTCTRLA_REG_OFS) =
    ( EE_Tc_Hw_Status[Channel] & EE_TC_HW_IRQ_OVF_PRI_MASK );

  }

  if ( 
    ( Notification == EE_PWM_FALLING_EDGE )	||
    ( Notification == EE_PWM_BOTH_EDGES )
  ) {

#if	0
    EE_HWREG(addr + EE_TC_HW_TCX0_INTCTRLB_REG_OFS) =
    ( EE_Pwm_Ccx_Div[Channel] & EE_TC_HW_CCX_EN_MASK );
#else	/* 0 */
    EE_HWREG(addr + EE_TC_HW_TCX0_INTCTRLB_REG_OFS) =
#ifdef	__AVR_PGMSPACE__
    (EE_UREG) pgm_read_byte(&EE_Pwm_Ccx_Pri[Channel]);
#else	/* __AVR_PGMSPACE__ */
    EE_Pwm_Ccx_Pri[Channel];
#endif	/* __AVR_PGMSPACE__ */
#endif	/* 0 */

  }

  EE_HWREG(addr + EE_TC_HW_TCX0_INTFLAGS_REG_OFS) = EE_TC_HW_ALL_INT_MASK;
#endif	/*
	 *  (
	 *    defined(__PWM_CHANNEL_C_0__) &&
	 *    (
	 *      defined(EE_AVR8_TCC0_OVF_ISR) ||
	 *      ( defined(__PWM_C_0_CCA__) && defined(EE_AVR8_TCC0_CCA_ISR) ) ||
	 *      ( defined(__PWM_C_0_CCB__) && defined(EE_AVR8_TCC0_CCB_ISR) ) ||
	 *      ( defined(__PWM_C_0_CCC__) && defined(EE_AVR8_TCC0_CCC_ISR) ) ||
	 *      ( defined(__PWM_C_0_CCD__) && defined(EE_AVR8_TCC0_CCD_ISR) )
	 *    )
	 *  ) || (
	 *    defined(__PWM_CHANNEL_D_0__) &&
	 *    (
	 *      defined(EE_AVR8_TCD0_OVF_ISR) ||
	 *      ( defined(__PWM_D_0_CCA__) && defined(EE_AVR8_TCD0_CCA_ISR) ) ||
	 *      ( defined(__PWM_D_0_CCB__) && defined(EE_AVR8_TCD0_CCB_ISR) ) ||
	 *      ( defined(__PWM_D_0_CCC__) && defined(EE_AVR8_TCD0_CCC_ISR) ) ||
	 *      ( defined(__PWM_D_0_CCD__) && defined(EE_AVR8_TCD0_CCD_ISR) )
	 *    )
	 *  ) || (
	 *    defined(__PWM_CHANNEL_E_0__) &&
	 *    (
	 *      defined(EE_AVR8_TCE0_OVF_ISR) ||
	 *      ( defined(__PWM_E_0_CCA__) && defined(EE_AVR8_TCE0_CCA_ISR) ) ||
	 *      ( defined(__PWM_E_0_CCB__) && defined(EE_AVR8_TCE0_CCB_ISR) ) ||
	 *      ( defined(__PWM_E_0_CCC__) && defined(EE_AVR8_TCE0_CCC_ISR) ) ||
	 *      ( defined(__PWM_E_0_CCD__) && defined(EE_AVR8_TCE0_CCD_ISR) )
	 *     )
	 *  )
	 */


  /* Resume IRQs */
  EE_hal_resumeIRQ(flags);

#ifdef	__PWM_EXTENDED_STATUS__
  return E_OK;
#endif

#endif	/* __USE_TC_HW__ */

}	/* EE_Pwm_EnableNotification() */

#ifdef	__PWM_DISABLENOTIF_API__
/*
 * EE_Pwm_DisableNotification() Implementation.
 */
#ifdef	__PWM_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_Pwm_DisableNotification(
  EE_TYPEPWMCHANNEL	Channel
) {

#ifdef	__USE_TC_HW__

#ifdef	DEBUG
  volatile EE_UREG	flags;
  volatile EE_HWREG_PTR	addr;
#else
  register EE_UREG	flags;
  register EE_HWREG_PTR	addr;
#endif

#ifdef	__PWM_EXTENDED_STATUS__

  /* Channel Validation */
  EE_ATXMEGA_VALIDATE_W_RV( 
    ( Channel < EE_TC_HW_ALL_UNITS_MASK ),
    E_PWM_PARAM_CHANNEL
  );

#endif	/* __PWM_EXTENDED_STATUS__ */

  /* Suspend Interrupt */
  flags = EE_hal_suspendIRQ();

#ifdef	__PWM_EXTENDED_STATUS__

  EE_ATXMEGA_VALIDATE_IRQ_W_RV(
    ( EE_Tc_Hw_GetStatus[Channel] & EE_TC_HW_INIT_MASK ),
    E_NOT_OK,
    flags
  );

#if	( \
  ( \
    defined(__PWM_CHANNEL_C_0__) && \
    ( \
      defined(EE_AVR8_TCC0_OVF_ISR) ||	\
      ( defined(__PWM_C_0_CCA__) && defined(EE_AVR8_TCC0_CCA_ISR) ) || \
      ( defined(__PWM_C_0_CCB__) && defined(EE_AVR8_TCC0_CCB_ISR) ) || \
      ( defined(__PWM_C_0_CCC__) && defined(EE_AVR8_TCC0_CCC_ISR) ) || \
      ( defined(__PWM_C_0_CCD__) && defined(EE_AVR8_TCC0_CCD_ISR) ) \
    ) \
  ) || \
  ( \
    defined(__PWM_CHANNEL_D_0__) && \
    ( \
      defined(EE_AVR8_TCD0_OVF_ISR) || \
      ( defined(__PWM_D_0_CCA__) && defined(EE_AVR8_TCD0_CCA_ISR) ) || \
      ( defined(__PWM_D_0_CCB__) && defined(EE_AVR8_TCD0_CCB_ISR) ) || \
      ( defined(__PWM_D_0_CCC__) && defined(EE_AVR8_TCD0_CCC_ISR) ) || \
      ( defined(__PWM_D_0_CCD__) && defined(EE_AVR8_TCD0_CCD_ISR) ) \
    ) \
  ) || \
  ( \
    defined(__PWM_CHANNEL_E_0__) && \
    ( \
      defined(EE_AVR8_TCE0_OVF_ISR) || \
      ( defined(__PWM_E_0_CCA__) && defined(EE_AVR8_TCE0_CCA_ISR) ) || \
      ( defined(__PWM_E_0_CCB__) && defined(EE_AVR8_TCE0_CCB_ISR) ) || \
      ( defined(__PWM_E_0_CCC__) && defined(EE_AVR8_TCE0_CCC_ISR) ) || \
      ( defined(__PWM_E_0_CCD__) && defined(EE_AVR8_TCE0_CCD_ISR) ) \
    ) \
  ) || \
  ( \
    defined(__PWM_CHANNEL_F_0__) && \
    ( \
      defined(EE_AVR8_TCF0_OVF_ISR) || \
      ( defined(__PWM_F_0_CCA__) && defined(EE_AVR8_TCF0_CCA_ISR) ) || \
      ( defined(__PWM_F_0_CCB__) && defined(EE_AVR8_TCF0_CCB_ISR) ) || \
      ( defined(__PWM_F_0_CCC__) && defined(EE_AVR8_TCF0_CCC_ISR) ) || \
      ( defined(__PWM_F_0_CCD__) && defined(EE_AVR8_TCE0_CCD_ISR) ) \
    ) \
  ) || \
  ( \
    defined(__PWM_CHANNEL_C_1__) && \
    ( \
      defined(EE_AVR8_TCC1_OVF_ISR) ||	\
      ( defined(__PWM_C_1_CCA__) && defined(EE_AVR8_TCC1_CCA_ISR) ) || \
      ( defined(__PWM_C_1_CCB__) && defined(EE_AVR8_TCC1_CCB_ISR) ) \
    ) \
  ) || \
  ( \
    defined(__PWM_CHANNEL_D_1__) && \
    ( \
      defined(EE_AVR8_TCD1_OVF_ISR) || \
      ( defined(__PWM_D_1_CCA__) && defined(EE_AVR8_TCD1_CCA_ISR) ) || \
      ( defined(__PWM_D_1_CCB__) && defined(EE_AVR8_TCD1_CCB_ISR) ) \
    ) \
  ) || \
  ( \
    defined(__PWM_CHANNEL_E_1__) && \
    ( \
      defined(EE_AVR8_TCE1_OVF_ISR) || \
      ( defined(__PWM_E_1_CCA__) && defined(EE_AVR8_TCE1_CCA_ISR) ) || \
      ( defined(__PWM_E_1_CCB__) && defined(EE_AVR8_TCE1_CCB_ISR) ) \
    ) \
  ) || \
  ( \
    defined(__PWM_CHANNEL_F_1__) && \
    ( \
      defined(EE_AVR8_TCF1_OVF_ISR) || \
      ( defined(__PWM_F_1_CCA__) && defined(EE_AVR8_TCF1_CCA_ISR) ) || \
      ( defined(__PWM_F_1_CCB__) && defined(EE_AVR8_TCF1_CCB_ISR) ) \
    ) \
  ) \
)
  /* Call-Back Validation. */
  EE_ATXMEGA_VALIDATE_W_RV(
    ( EE_Tc_Hw_GetCallback(Channel) != EE_NULL_PTR ),
    E_PWM_PARAM_CALLBACK
  );

#else

  /* Call-Back Validation. */
  EE_ATXMEGA_VALIDATE_W_RV( EE_FALSE, E_PWM_PARAM_CALLBACK );

#endif	/*
	 *    defined(__PWM_CHANNEL_C_0__) &&
	 *    (
	 *      defined(EE_AVR8_TCC0_OVF_ISR) ||
	 *      ( defined(__PWM_C_0_CCA__) && defined(EE_AVR8_TCC0_CCA_ISR) ) ||
	 *      ( defined(__PWM_C_0_CCB__) && defined(EE_AVR8_TCC0_CCB_ISR) ) ||
	 *      ( defined(__PWM_C_0_CCC__) && defined(EE_AVR8_TCC0_CCC_ISR) ) ||
	 *      ( defined(__PWM_C_0_CCD__) && defined(EE_AVR8_TCC0_CCD_ISR) )
	 *    )
	 *  ) || (
	 *    defined(__PWM_CHANNEL_D_0__) &&
	 *    (
	 *      defined(EE_AVR8_TCD0_OVF_ISR) ||
	 *      ( defined(__PWM_D_0_CCA__) && defined(EE_AVR8_TCD0_CCA_ISR) ) ||
	 *      ( defined(__PWM_D_0_CCB__) && defined(EE_AVR8_TCD0_CCB_ISR) ) ||
	 *      ( defined(__PWM_D_0_CCC__) && defined(EE_AVR8_TCD0_CCC_ISR) ) ||
	 *      ( defined(__PWM_D_0_CCD__) && defined(EE_AVR8_TCD0_CCD_ISR) )
	 *    )
	 *  ) || (
	 *    defined(__PWM_CHANNEL_E_0__) &&
	 *    (
	 *      defined(EE_AVR8_TCE0_OVF_ISR) ||
	 *      ( defined(__PWM_E_0_CCA__) && defined(EE_AVR8_TCE0_CCA_ISR) ) ||
	 *      ( defined(__PWM_E_0_CCB__) && defined(EE_AVR8_TCE0_CCB_ISR) ) ||
	 *      ( defined(__PWM_E_0_CCC__) && defined(EE_AVR8_TCE0_CCC_ISR) ) ||
	 *      ( defined(__PWM_E_0_CCD__) && defined(EE_AVR8_TCE0_CCD_ISR) )
	 *     )
	 *  ) || (
	 *    defined(__PWM_CHANNEL_F_0__) &&
	 *    (
	 *      defined(EE_AVR8_TCF0_OVF_ISR) ||
	 *      ( defined(__PWM_F_0_CCA__) && defined(EE_AVR8_TCF0_CCA_ISR) ) ||
	 *      ( defined(__PWM_F_0_CCB__) && defined(EE_AVR8_TCF0_CCB_ISR) ) ||
	 *      ( defined(__PWM_F_0_CCC__) && defined(EE_AVR8_TCF0_CCC_ISR) ) ||
	 *      ( defined(__PWM_F_0_CCD__) && defined(EE_AVR8_TCF0_CCD_ISR) )
	 *     )
	 *  ) || (
	 *    defined(__PWM_CHANNEL_C_1__) &&
	 *    (
	 *      defined(EE_AVR8_TCC1_OVF_ISR) ||
	 *      ( defined(__PWM_C_1_CCA__) && defined(EE_AVR8_TCC1_CCA_ISR) ) ||
	 *      ( defined(__PWM_C_1_CCB__) && defined(EE_AVR8_TCC1_CCB_ISR) )
	 *    )
	 *  ) || (
	 *    defined(__PWM_CHANNEL_D_1__) &&
	 *    (
	 *      defined(EE_AVR8_TCD1_OVF_ISR) ||
	 *      ( defined(__PWM_D_1_CCA__) && defined(EE_AVR8_TCD1_CCA_ISR) ) ||
	 *      ( defined(__PWM_D_1_CCB__) && defined(EE_AVR8_TCD1_CCB_ISR) )
	 *    )
	 *  ) || (
	 *    defined(__PWM_CHANNEL_E_1__) &&
	 *    (
	 *      defined(EE_AVR8_TCE1_OVF_ISR) ||
	 *      ( defined(__PWM_E_1_CCA__) && defined(EE_AVR8_TCE1_CCA_ISR) ) ||
	 *      ( defined(__PWM_E_1_CCB__) && defined(EE_AVR8_TCE1_CCB_ISR) )
	 *     )
	 *  ) || (
	 *    defined(__PWM_CHANNEL_F_1__) &&
	 *    (
	 *      defined(EE_AVR8_TCF1_OVF_ISR) ||
	 *      ( defined(__PWM_F_1_CCA__) && defined(EE_AVR8_TCF1_CCA_ISR) ) ||
	 *      ( defined(__PWM_F_1_CCB__) && defined(EE_AVR8_TCF1_CCB_ISR) )
	 *     )
	 *  )
	 */

#endif	/* __PWM_EXTENDED_STATUS__ */

#if	( \
  ( \
    defined(__PWM_CHANNEL_C_0__) && \
    ( \
      defined(EE_AVR8_TCC0_OVF_ISR) ||	\
      ( defined(__PWM_C_0_CCA__) && defined(EE_AVR8_TCC0_CCA_ISR) ) || \
      ( defined(__PWM_C_0_CCB__) && defined(EE_AVR8_TCC0_CCB_ISR) ) || \
      ( defined(__PWM_C_0_CCC__) && defined(EE_AVR8_TCC0_CCC_ISR) ) || \
      ( defined(__PWM_C_0_CCD__) && defined(EE_AVR8_TCC0_CCD_ISR) ) \
    ) \
  ) || \
  ( \
    defined(__PWM_CHANNEL_D_0__) && \
    ( \
      defined(EE_AVR8_TCD0_OVF_ISR) || \
      ( defined(__PWM_D_0_CCA__) && defined(EE_AVR8_TCD0_CCA_ISR) ) || \
      ( defined(__PWM_D_0_CCB__) && defined(EE_AVR8_TCD0_CCB_ISR) ) || \
      ( defined(__PWM_D_0_CCC__) && defined(EE_AVR8_TCD0_CCC_ISR) ) || \
      ( defined(__PWM_D_0_CCD__) && defined(EE_AVR8_TCD0_CCD_ISR) ) \
    ) \
  ) || \
  ( \
    defined(__PWM_CHANNEL_E_0__) && \
    ( \
      defined(EE_AVR8_TCE0_OVF_ISR) || \
      ( defined(__PWM_E_0_CCA__) && defined(EE_AVR8_TCE0_CCA_ISR) ) || \
      ( defined(__PWM_E_0_CCB__) && defined(EE_AVR8_TCE0_CCB_ISR) ) || \
      ( defined(__PWM_E_0_CCC__) && defined(EE_AVR8_TCE0_CCC_ISR) ) || \
      ( defined(__PWM_E_0_CCD__) && defined(EE_AVR8_TCE0_CCD_ISR) ) \
    ) \
  ) || \
  ( \
    defined(__PWM_CHANNEL_F_0__) && \
    ( \
      defined(EE_AVR8_TCF0_OVF_ISR) || \
      ( defined(__PWM_F_0_CCA__) && defined(EE_AVR8_TCF0_CCA_ISR) ) || \
      ( defined(__PWM_F_0_CCB__) && defined(EE_AVR8_TCF0_CCB_ISR) ) || \
      ( defined(__PWM_F_0_CCC__) && defined(EE_AVR8_TCF0_CCC_ISR) ) || \
      ( defined(__PWM_F_0_CCD__) && defined(EE_AVR8_TCE0_CCD_ISR) ) \
    ) \
  ) || \
  ( \
    defined(__PWM_CHANNEL_C_1__) && \
    ( \
      defined(EE_AVR8_TCC1_OVF_ISR) ||	\
      ( defined(__PWM_C_1_CCA__) && defined(EE_AVR8_TCC1_CCA_ISR) ) || \
      ( defined(__PWM_C_1_CCB__) && defined(EE_AVR8_TCC1_CCB_ISR) ) \
    ) \
  ) || \
  ( \
    defined(__PWM_CHANNEL_D_1__) && \
    ( \
      defined(EE_AVR8_TCD1_OVF_ISR) || \
      ( defined(__PWM_D_1_CCA__) && defined(EE_AVR8_TCD1_CCA_ISR) ) || \
      ( defined(__PWM_D_1_CCB__) && defined(EE_AVR8_TCD1_CCB_ISR) ) \
    ) \
  ) || \
  ( \
    defined(__PWM_CHANNEL_E_1__) && \
    ( \
      defined(EE_AVR8_TCE1_OVF_ISR) || \
      ( defined(__PWM_E_1_CCA__) && defined(EE_AVR8_TCE1_CCA_ISR) ) || \
      ( defined(__PWM_E_1_CCB__) && defined(EE_AVR8_TCE1_CCB_ISR) ) \
    ) \
  ) || \
  ( \
    defined(__PWM_CHANNEL_F_1__) && \
    ( \
      defined(EE_AVR8_TCF1_OVF_ISR) || \
      ( defined(__PWM_F_1_CCA__) && defined(EE_AVR8_TCF1_CCA_ISR) ) || \
      ( defined(__PWM_F_1_CCB__) && defined(EE_AVR8_TCF1_CCB_ISR) ) \
    ) \
  ) \
)
  addr = EE_TC_HW_2_ADDR(Channel);

  EE_HWREG(addr + EE_TC_HW_INTCTRLA_REG_OFS) = EE_OFF;

  EE_HWREG(addr + EE_TC_HW_INTCTRLB_REG_OFS) = EE_OFF;

  EE_HWREG(addr + EE_TC_HW_INTFLAGS_REG_OFS) = EE_TC_HW_ALL_INT_MASK;
#endif	/*
	 *    defined(__PWM_CHANNEL_C_0__) &&
	 *    (
	 *      defined(EE_AVR8_TCC0_OVF_ISR) ||
	 *      ( defined(__PWM_C_0_CCA__) && defined(EE_AVR8_TCC0_CCA_ISR) ) ||
	 *      ( defined(__PWM_C_0_CCB__) && defined(EE_AVR8_TCC0_CCB_ISR) ) ||
	 *      ( defined(__PWM_C_0_CCC__) && defined(EE_AVR8_TCC0_CCC_ISR) ) ||
	 *      ( defined(__PWM_C_0_CCD__) && defined(EE_AVR8_TCC0_CCD_ISR) )
	 *    )
	 *  ) || (
	 *    defined(__PWM_CHANNEL_D_0__) &&
	 *    (
	 *      defined(EE_AVR8_TCD0_OVF_ISR) ||
	 *      ( defined(__PWM_D_0_CCA__) && defined(EE_AVR8_TCD0_CCA_ISR) ) ||
	 *      ( defined(__PWM_D_0_CCB__) && defined(EE_AVR8_TCD0_CCB_ISR) ) ||
	 *      ( defined(__PWM_D_0_CCC__) && defined(EE_AVR8_TCD0_CCC_ISR) ) ||
	 *      ( defined(__PWM_D_0_CCD__) && defined(EE_AVR8_TCD0_CCD_ISR) )
	 *    )
	 *  ) || (
	 *    defined(__PWM_CHANNEL_E_0__) &&
	 *    (
	 *      defined(EE_AVR8_TCE0_OVF_ISR) ||
	 *      ( defined(__PWM_E_0_CCA__) && defined(EE_AVR8_TCE0_CCA_ISR) ) ||
	 *      ( defined(__PWM_E_0_CCB__) && defined(EE_AVR8_TCE0_CCB_ISR) ) ||
	 *      ( defined(__PWM_E_0_CCC__) && defined(EE_AVR8_TCE0_CCC_ISR) ) ||
	 *      ( defined(__PWM_E_0_CCD__) && defined(EE_AVR8_TCE0_CCD_ISR) )
	 *     )
	 *  ) || (
	 *    defined(__PWM_CHANNEL_F_0__) &&
	 *    (
	 *      defined(EE_AVR8_TCF0_OVF_ISR) ||
	 *      ( defined(__PWM_F_0_CCA__) && defined(EE_AVR8_TCF0_CCA_ISR) ) ||
	 *      ( defined(__PWM_F_0_CCB__) && defined(EE_AVR8_TCF0_CCB_ISR) ) ||
	 *      ( defined(__PWM_F_0_CCC__) && defined(EE_AVR8_TCF0_CCC_ISR) ) ||
	 *      ( defined(__PWM_F_0_CCD__) && defined(EE_AVR8_TCF0_CCD_ISR) )
	 *     )
	 *  ) || (
	 *    defined(__PWM_CHANNEL_C_1__) &&
	 *    (
	 *      defined(EE_AVR8_TCC1_OVF_ISR) ||
	 *      ( defined(__PWM_C_1_CCA__) && defined(EE_AVR8_TCC1_CCA_ISR) ) ||
	 *      ( defined(__PWM_C_1_CCB__) && defined(EE_AVR8_TCC1_CCB_ISR) )
	 *    )
	 *  ) || (
	 *    defined(__PWM_CHANNEL_D_1__) &&
	 *    (
	 *      defined(EE_AVR8_TCD1_OVF_ISR) ||
	 *      ( defined(__PWM_D_1_CCA__) && defined(EE_AVR8_TCD1_CCA_ISR) ) ||
	 *      ( defined(__PWM_D_1_CCB__) && defined(EE_AVR8_TCD1_CCB_ISR) )
	 *    )
	 *  ) || (
	 *    defined(__PWM_CHANNEL_E_1__) &&
	 *    (
	 *      defined(EE_AVR8_TCE1_OVF_ISR) ||
	 *      ( defined(__PWM_E_1_CCA__) && defined(EE_AVR8_TCE1_CCA_ISR) ) ||
	 *      ( defined(__PWM_E_1_CCB__) && defined(EE_AVR8_TCE1_CCB_ISR) )
	 *     )
	 *  ) || (
	 *    defined(__PWM_CHANNEL_F_1__) &&
	 *    (
	 *      defined(EE_AVR8_TCF1_OVF_ISR) ||
	 *      ( defined(__PWM_F_1_CCA__) && defined(EE_AVR8_TCF1_CCA_ISR) ) ||
	 *      ( defined(__PWM_F_1_CCB__) && defined(EE_AVR8_TCF1_CCB_ISR) )
	 *     )
	 *  )
	 */

  /* Resume IRQs */
  EE_hal_resumeIRQ(flags);

#ifdef	__PWM_EXTENDED_STATUS__
  return E_OK;
#endif

#else	/* __USE_TC_HW__ */

#ifdef	DEBUG
  volatile EE_UREG	flags;
  volatile EE_HWREG_PTR	addr;
#else
  register EE_UREG	flags;
  register EE_HWREG_PTR	addr;
#endif

#ifdef	__PWM_EXTENDED_STATUS__

  /* Channel Validation */
  EE_ATXMEGA_VALIDATE_W_RV( 
    ( Channel < EE_TC_HW_ALL_TCX0_MASK ),
    E_PWM_PARAM_CHANNEL
  );

#endif	/* __PWM_EXTENDED_STATUS__ */

  /* Suspend Interrupt */
  flags = EE_hal_suspendIRQ();

#ifdef	__PWM_EXTENDED_STATUS__

  EE_ATXMEGA_VALIDATE_IRQ_W_RV(
    ( EE_Tc_Hw_Status[Channel] & EE_TC_HW_INIT_MASK ),
    E_NOT_OK,
    flags
  );

#if	( \
  ( \
    defined(__PWM_CHANNEL_C_0__) && \
    ( \
      defined(EE_AVR8_TCC0_OVF_ISR) ||	\
      ( defined(__PWM_C_0_CCA__) && defined(EE_AVR8_TCC0_CCA_ISR) ) || \
      ( defined(__PWM_C_0_CCB__) && defined(EE_AVR8_TCC0_CCB_ISR) ) || \
      ( defined(__PWM_C_0_CCC__) && defined(EE_AVR8_TCC0_CCC_ISR) ) || \
      ( defined(__PWM_C_0_CCD__) && defined(EE_AVR8_TCC0_CCD_ISR) ) \
    ) \
  ) || \
  ( \
    defined(__PWM_CHANNEL_D_0__) && \
    ( \
      defined(EE_AVR8_TCD0_OVF_ISR) || \
      ( defined(__PWM_D_0_CCA__) && defined(EE_AVR8_TCD0_CCA_ISR) ) || \
      ( defined(__PWM_D_0_CCB__) && defined(EE_AVR8_TCD0_CCB_ISR) ) || \
      ( defined(__PWM_D_0_CCC__) && defined(EE_AVR8_TCD0_CCC_ISR) ) || \
      ( defined(__PWM_D_0_CCD__) && defined(EE_AVR8_TCD0_CCD_ISR) ) \
    ) \
  ) || \
  ( \
    defined(__PWM_CHANNEL_E_0__) && \
    ( \
      defined(EE_AVR8_TCE0_OVF_ISR) || \
      ( defined(__PWM_E_0_CCA__) && defined(EE_AVR8_TCE0_CCA_ISR) ) || \
      ( defined(__PWM_E_0_CCB__) && defined(EE_AVR8_TCE0_CCB_ISR) ) || \
      ( defined(__PWM_E_0_CCC__) && defined(EE_AVR8_TCE0_CCC_ISR) ) || \
      ( defined(__PWM_E_0_CCD__) && defined(EE_AVR8_TCE0_CCD_ISR) ) \
    ) \
  ) \
)
  /* Call-Back Validation. */
  EE_ATXMEGA_VALIDATE_W_RV(
    ( EE_Tc_Hw_CallBack[Channel] != EE_NULL_PTR ),
    E_PWM_PARAM_CALLBACK
  );

#else

  /* Call-Back Validation. */
  EE_ATXMEGA_VALIDATE_W_RV( EE_FALSE, E_PWM_PARAM_CALLBACK );

#endif	/*
	 *  (
	 *    defined(__PWM_CHANNEL_C_0__) &&
	 *    (
	 *      defined(EE_AVR8_TCC0_OVF_ISR) ||
	 *      ( defined(__PWM_C_0_CCA__) && defined(EE_AVR8_TCC0_CCA_ISR) ) ||
	 *      ( defined(__PWM_C_0_CCB__) && defined(EE_AVR8_TCC0_CCB_ISR) ) ||
	 *      ( defined(__PWM_C_0_CCC__) && defined(EE_AVR8_TCC0_CCC_ISR) ) ||
	 *      ( defined(__PWM_C_0_CCD__) && defined(EE_AVR8_TCC0_CCD_ISR) )
	 *    )
	 *  ) || (
	 *    defined(__PWM_CHANNEL_D_0__) &&
	 *    (
	 *      defined(EE_AVR8_TCD0_OVF_ISR) ||
	 *      ( defined(__PWM_D_0_CCA__) && defined(EE_AVR8_TCD0_CCA_ISR) ) ||
	 *      ( defined(__PWM_D_0_CCB__) && defined(EE_AVR8_TCD0_CCB_ISR) ) ||
	 *      ( defined(__PWM_D_0_CCC__) && defined(EE_AVR8_TCD0_CCC_ISR) ) ||
	 *      ( defined(__PWM_D_0_CCD__) && defined(EE_AVR8_TCD0_CCD_ISR) )
	 *    )
	 *  ) || (
	 *    defined(__PWM_CHANNEL_E_0__) &&
	 *    (
	 *      defined(EE_AVR8_TCE0_OVF_ISR) ||
	 *      ( defined(__PWM_E_0_CCA__) && defined(EE_AVR8_TCE0_CCA_ISR) ) ||
	 *      ( defined(__PWM_E_0_CCB__) && defined(EE_AVR8_TCE0_CCB_ISR) ) ||
	 *      ( defined(__PWM_E_0_CCC__) && defined(EE_AVR8_TCE0_CCC_ISR) ) ||
	 *      ( defined(__PWM_E_0_CCD__) && defined(EE_AVR8_TCE0_CCD_ISR) )
	 *     )
	 *  )
	 */

#endif	/* __PWM_EXTENDED_STATUS__ */

#if	( \
  ( \
    defined(__PWM_CHANNEL_C_0__) && \
    ( \
      defined(EE_AVR8_TCC0_OVF_ISR) ||	\
      ( defined(__PWM_C_0_CCA__) && defined(EE_AVR8_TCC0_CCA_ISR) ) || \
      ( defined(__PWM_C_0_CCB__) && defined(EE_AVR8_TCC0_CCB_ISR) ) || \
      ( defined(__PWM_C_0_CCC__) && defined(EE_AVR8_TCC0_CCC_ISR) ) || \
      ( defined(__PWM_C_0_CCD__) && defined(EE_AVR8_TCC0_CCD_ISR) ) \
    ) \
  ) || \
  ( \
    defined(__PWM_CHANNEL_D_0__) && \
    ( \
      defined(EE_AVR8_TCD0_OVF_ISR) || \
      ( defined(__PWM_D_0_CCA__) && defined(EE_AVR8_TCD0_CCA_ISR) ) || \
      ( defined(__PWM_D_0_CCB__) && defined(EE_AVR8_TCD0_CCB_ISR) ) || \
      ( defined(__PWM_D_0_CCC__) && defined(EE_AVR8_TCD0_CCC_ISR) ) || \
      ( defined(__PWM_D_0_CCD__) && defined(EE_AVR8_TCD0_CCD_ISR) ) \
    ) \
  ) || \
  ( \
    defined(__PWM_CHANNEL_E_0__) && \
    ( \
      defined(EE_AVR8_TCE0_OVF_ISR) || \
      ( defined(__PWM_E_0_CCA__) && defined(EE_AVR8_TCE0_CCA_ISR) ) || \
      ( defined(__PWM_E_0_CCB__) && defined(EE_AVR8_TCE0_CCB_ISR) ) || \
      ( defined(__PWM_E_0_CCC__) && defined(EE_AVR8_TCE0_CCC_ISR) ) || \
      ( defined(__PWM_E_0_CCD__) && defined(EE_AVR8_TCE0_CCD_ISR) ) \
    ) \
  ) \
)
  addr = EE_TC_HW_TCX0_2_ADDR(Channel);

  EE_HWREG(addr + EE_TC_HW_TCX0_INTCTRLA_REG_OFS) = EE_OFF;

  EE_HWREG(addr + EE_TC_HW_TCX0_INTCTRLB_REG_OFS) = EE_OFF;

  EE_HWREG(addr + EE_TC_HW_TCX0_INTFLAGS_REG_OFS) = EE_TC_HW_ALL_INT_MASK;
#endif	/*
	 *  (
	 *    defined(__PWM_CHANNEL_C_0__) &&
	 *    (
	 *      defined(EE_AVR8_TCC0_OVF_ISR) ||
	 *      ( defined(__PWM_C_0_CCA__) && defined(EE_AVR8_TCC0_CCA_ISR) ) ||
	 *      ( defined(__PWM_C_0_CCB__) && defined(EE_AVR8_TCC0_CCB_ISR) ) ||
	 *      ( defined(__PWM_C_0_CCC__) && defined(EE_AVR8_TCC0_CCC_ISR) ) ||
	 *      ( defined(__PWM_C_0_CCD__) && defined(EE_AVR8_TCC0_CCD_ISR) )
	 *    )
	 *  ) || (
	 *    defined(__PWM_CHANNEL_D_0__) &&
	 *    (
	 *      defined(EE_AVR8_TCD0_OVF_ISR) ||
	 *      ( defined(__PWM_D_0_CCA__) && defined(EE_AVR8_TCD0_CCA_ISR) ) ||
	 *      ( defined(__PWM_D_0_CCB__) && defined(EE_AVR8_TCD0_CCB_ISR) ) ||
	 *      ( defined(__PWM_D_0_CCC__) && defined(EE_AVR8_TCD0_CCC_ISR) ) ||
	 *      ( defined(__PWM_D_0_CCD__) && defined(EE_AVR8_TCD0_CCD_ISR) )
	 *    )
	 *  ) || (
	 *    defined(__PWM_CHANNEL_E_0__) &&
	 *    (
	 *      defined(EE_AVR8_TCE0_OVF_ISR) ||
	 *      ( defined(__PWM_E_0_CCA__) && defined(EE_AVR8_TCE0_CCA_ISR) ) ||
	 *      ( defined(__PWM_E_0_CCB__) && defined(EE_AVR8_TCE0_CCB_ISR) ) ||
	 *      ( defined(__PWM_E_0_CCC__) && defined(EE_AVR8_TCE0_CCC_ISR) ) ||
	 *      ( defined(__PWM_E_0_CCD__) && defined(EE_AVR8_TCE0_CCD_ISR) )
	 *     )
	 *  )
	 */

  /* Resume IRQs */
  EE_hal_resumeIRQ(flags);

#ifdef	__PWM_EXTENDED_STATUS__
  return E_OK;
#endif

#endif	/* __USE_TC_HW__ */

}
#endif	/* __PWM_DISABLENOTIF_API__ */
#endif	/* __PWM_ENABLENOTIF_API__ */

/* Absolute Duty-Cycle in ticks.
 *
 * param	Period		Absolute Period in ticks.
 * param	DutyCycle	Relative DutyCycle (%).
 * param	DutyCycle	Relative DutyCycle (0x0000 - 0x8000).
 * return	Absolute Duty-Cycle in ticks.
 */
static EE_UINT16
EE_Pwm_AbsDutyCycle_Internal(
  EE_TYPEPWMPERIOD	Period,
  EE_TYPEPWMDUTYCYCLE	DutyCycle
){

#ifdef	DEBUG
  volatile EE_UINT16	abs_dc;
#else
  register EE_UINT16	abs_dc;
#endif

#if	0
  abs_dc = (EE_UINT16)(((EE_UINT32)DutyCycle * Period) / (EE_UINT16)0x64U );
#else
  abs_dc = ((EE_UINT32)Period * DutyCycle) >> 0x0FU;
#endif

  if ( abs_dc == Period ) {

    abs_dc++;

  }

  return abs_dc;

}	/* EE_Pwm_AbsDutyCycle_Internal */

/* Duty-Cycle Setting-Up.
 *
 * param	addr	PWM Module Base Address.
 * param	abs_dc	Absolute Duty-Cycle in ticks to Set-Up.
 * param	stat	PWM Module Status.
 * param	ccx_div	Compare and Capture plus Divisor.
 * return	Nothing.
 *
 */
static void
EE_Pwm_SetDutyCycle_Internal(
  EE_HWREG_PTR	addr,
  EE_UINT16	abs_dc,
  EE_UREG	stat,
  EE_UREG	ccx_div
) {

#ifdef	__USE_TC_HW__

#if	( defined(__AVR_ATxmega16D4__) || defined(__AVR_ATxmega32D4__) )

#if	( \
  defined(__PWM_C_0_CCD__) || defined(__PWM_D_0_CCA__) || \
  defined(__PWM_E_0_CCD__) || defined(__PWM_C_1_CCD__) \
)
  if ( ccx_div & EE_TC_HW_CCA_ENABLE ) {

    /*
     * EE_ATxmega_Reg16Write(
     *   addr + EE_TC_HW_CCA_REG_OFS + EE_TC_HW_BUF_REG_OFS,
     *   abs_dc
     * );
     */
    EE_ATxmega_Reg16Write( addr + EE_TC_HW_CCABUF_REG_OFS, abs_dc );

    if ( ! ( stat & EE_PWM_DEU_MASK ) ) {

      /*
       * EE_ATxmega_Reg16Write(addr + EE_TC_HW_CCA_REG_OFS, abs_dc );
       */
      EE_ATxmega_Reg16Write( addr + EE_TC_HW_CCA_REG_OFS, abs_dc );

    }

  }
#endif	/*
	 * __PWM_C_0_CCA__ || __PWM_D_0_CCA__ ||
	 * __PWM_E_0_CCA__ || __PWM_C_1_CCA__
	 */

#if	( \
  defined(__PWM_C_0_CCB__) || defined(__PWM_D_0_CCB__) || \
  defined(__PWM_E_0_CCB__) || defined(__PWM_C_1_CCB__) \
)
  if ( ccx_div & EE_TC_HW_CCB_ENABLE ) {

    EE_ATxmega_Reg16Write( addr + EE_TC_HW_CCBBUF_REG_OFS, abs_dc );

    if ( ! ( stat & EE_PWM_DEU_MASK ) ) {

      EE_ATxmega_Reg16Write( addr + EE_TC_HW_CCB_REG_OFS, abs_dc );

    }

  }
#endif	/*
	 * __PWM_C_0_CCB__ || __PWM_D_0_CCB__ ||
	 * __PWM_E_0_CCB__ || __PWM_C_1_CCB__
	 */

#if	( \
  defined(__PWM_C_0_CCC__) || defined(__PWM_D_0_CCC__) || \
  defined(__PWM_E_0_CCC__) \
)
  if ( ccx_div & EE_TC_HW_CCC_ENABLE ) {

    EE_ATxmega_Reg16Write( addr + EE_TC_HW_CCCBUF_REG_OFS, abs_dc );

    if ( ! ( stat & EE_PWM_DEU_MASK ) ) {

      EE_ATxmega_Reg16Write( addr + EE_TC_HW_CCC_REG_OFS, abs_dc );

    }

  }
#endif	/*
	 * __PWM_C_0_CCC__ || __PWM_D_0_CCC__ ||
	 * __PWM_E_0_CCC__
	 */

#if	( \
  defined(__PWM_C_0_CCD__) || defined(__PWM_D_0_CCD__) || \
  defined(__PWM_E_0_CCD__) \
)
  if ( ccx_div & EE_TC_HW_CCD_ENABLE ) {

    EE_ATxmega_Reg16Write( addr + EE_TC_HW_CCDBUF_REG_OFS, abs_dc );

    if ( ! ( stat & EE_PWM_DEU_MASK ) ) {

      EE_ATxmega_Reg16Write( addr + EE_TC_HW_CCD_REG_OFS, abs_dc );

    }

  }
#endif	/*
	 * __PWM_C_0_CCD__ || __PWM_D_0_CCD__ ||
	 * __PWM_E_0_CCD__
	 */

#else	/* __AVR_ATxmega16D4__ || __AVR_ATxmega32D4__ */

#if	( \
  defined(__PWM_C_0_CCD__) || defined(__PWM_D_0_CCA__) || \
  defined(__PWM_E_0_CCD__) || defined(__PWM_F_0_CCA__) || \
  defined(__PWM_C_1_CCD__) || defined(__PWM_D_1_CCA__) || \
  defined(__PWM_E_1_CCD__) || defined(__PWM_F_1_CCA__) \
)
  if ( ccx_div & EE_TC_HW_CCA_ENABLE ) {

    /*
     * EE_ATxmega_Reg16Write(
     *   addr + EE_TC_HW_CCA_REG_OFS + EE_TC_HW_BUF_REG_OFS,
     *   abs_dc
     * );
     */
    EE_ATxmega_Reg16Write( addr + EE_TC_HW_CCABUF_REG_OFS, abs_dc );

    if ( ! ( stat & EE_PWM_DEU_MASK ) ) {

      /*
       * EE_ATxmega_Reg16Write(addr + EE_TC_HW_CCA_REG_OFS, abs_dc );
       */
      EE_ATxmega_Reg16Write( addr + EE_TC_HW_CCA_REG_OFS, abs_dc );

    }

  }
#endif	/*
	 * __PWM_C_0_CCA__ || __PWM_D_0_CCA__ ||
	 * __PWM_E_0_CCA__ || __PWM_F_0_CCA__ ||
	 * __PWM_C_1_CCA__ || __PWM_D_1_CCA__ ||
	 * __PWM_E_1_CCA__ || __PWM_F_1_CCA__
	 */

#if	( \
  defined(__PWM_C_0_CCB__) || defined(__PWM_D_0_CCB__) || \
  defined(__PWM_E_0_CCB__) || defined(__PWM_F_0_CCB__) || \
  defined(__PWM_C_1_CCB__) || defined(__PWM_D_1_CCB__) || \
  defined(__PWM_E_1_CCB__) || defined(__PWM_F_1_CCB__) \
)
  if ( ccx_div & EE_TC_HW_CCB_ENABLE ) {

    EE_ATxmega_Reg16Write( addr + EE_TC_HW_CCBBUF_REG_OFS, abs_dc );

    if ( ! ( stat & EE_PWM_DEU_MASK ) ) {

      EE_ATxmega_Reg16Write( addr + EE_TC_HW_CCB_REG_OFS, abs_dc );

    }

  }
#endif	/*
	 * __PWM_C_0_CCB__ || __PWM_D_0_CCB__ ||
	 * __PWM_E_0_CCB__ || __PWM_F_0_CCB__ ||
	 * __PWM_C_1_CCB__ || __PWM_D_1_CCB__ ||
	 * __PWM_E_1_CCB__ || __PWM_F_1_CCB__
	 */

#if	( \
  defined(__PWM_C_0_CCC__) || defined(__PWM_D_0_CCC__) || \
  defined(__PWM_E_0_CCC__) || defined(__PWM_F_0_CCC__) \
)
  if ( ccx_div & EE_TC_HW_CCC_ENABLE ) {

    EE_ATxmega_Reg16Write( addr + EE_TC_HW_CCCBUF_REG_OFS, abs_dc );

    if ( ! ( stat & EE_PWM_DEU_MASK ) ) {

      EE_ATxmega_Reg16Write( addr + EE_TC_HW_CCC_REG_OFS, abs_dc );

    }

  }
#endif	/*
	 * __PWM_C_0_CCC__ || __PWM_D_0_CCC__ ||
	 * __PWM_E_0_CCC__ || __PWM_F_0_CCC__
	 */

#if	( \
  defined(__PWM_C_0_CCD__) || defined(__PWM_D_0_CCD__) || \
  defined(__PWM_E_0_CCD__) || defined(__PWM_F_0_CCD__) \
)
  if ( ccx_div & EE_TC_HW_CCD_ENABLE ) {

    EE_ATxmega_Reg16Write( addr + EE_TC_HW_CCDBUF_REG_OFS, abs_dc );

    if ( ! ( stat & EE_PWM_DEU_MASK ) ) {

      EE_ATxmega_Reg16Write( addr + EE_TC_HW_CCD_REG_OFS, abs_dc );

    }

  }
#endif	/*
	 * __PWM_C_0_CCD__ || __PWM_D_0_CCD__ ||
	 * __PWM_E_0_CCD__ || __PWM_F_0_CCD__
	 */

#endif	/* __AVR_ATxmega16D4__ || __AVR_ATxmega32D4__ */

  if ( EE_HWREG(addr + EE_TC_HW_CTRLA_REG_OFS) == TC_CLKSEL_OFF_gc ) {

    EE_ATxmega_Reg16Write( addr + EE_TC_HW_CNT_REG_OFS, EE_OFF );

    EE_HWREG(addr + EE_TC_HW_CTRLB_REG_OFS) = (
      ( ccx_div & EE_TC_HW_CCX_EN_MASK ) |
      TC_WGMODE_SS_gc
    );

    EE_HWREG(addr + EE_TC_HW_CTRLA_REG_OFS) = ( 
      ccx_div & ~EE_TC_HW_CCX_EN_MASK 
    );

  }

#else	/* __USE_TC_HW__ */

#if	( \
  defined(__PWM_C_0_CCA__) || \
  defined(__PWM_D_0_CCA__) || \
  defined(__PWM_E_0_CCA__) \
)
  if ( ccx_div & EE_TC_HW_CCA_ENABLE ) {

    /*
     * EE_ATxmega_Reg16Write(
     *   addr + EE_TC_HW_TCX0_CCA_REG_OFS + EE_TC_HW_TCX0_BUF_REG_OFS,
     *   abs_dc
     * );
     */
    EE_ATxmega_Reg16Write( addr + EE_TC_HW_TCX0_CCABUF_REG_OFS, abs_dc );

    if ( ! ( stat & EE_PWM_DEU_MASK ) ) {

      /*
       * EE_ATxmega_Reg16Write(addr + EE_TC_HW_TCX0_CCA_REG_OFS, abs_dc );
       */
      EE_ATxmega_Reg16Write( addr + EE_TC_HW_TCX0_CCA_REG_OFS, abs_dc );

    }

  }
#endif /* __PWM_C_0_CCA__ || __PWM_D_0_CCA__ || __PWM_E_0_CCA__ */

#if	( \
  defined(__PWM_C_0_CCB__) || \
  defined(__PWM_D_0_CCB__) || \
  defined(__PWM_E_0_CCB__) \
)
  if ( ccx_div & EE_TC_HW_CCB_ENABLE ) {

    EE_ATxmega_Reg16Write( addr + EE_TC_HW_TCX0_CCBBUF_REG_OFS, abs_dc );

    if ( ! ( stat & EE_PWM_DEU_MASK ) ) {

      EE_ATxmega_Reg16Write( addr + EE_TC_HW_TCX0_CCB_REG_OFS, abs_dc );

    }

  }
#endif /* __PWM_C_0_CCB__ || __PWM_D_0_CCB__ || __PWM_E_0_CCB__ */

#if	( \
  defined(__PWM_C_0_CCC__) || \
  defined(__PWM_D_0_CCC__) || \
  defined(__PWM_E_0_CCC__) \
)
  if ( ccx_div & EE_TC_HW_CCC_ENABLE ) {

    EE_ATxmega_Reg16Write( addr + EE_TC_HW_TCX0_CCCBUF_REG_OFS, abs_dc );

    if ( ! ( stat & EE_PWM_DEU_MASK ) ) {

      EE_ATxmega_Reg16Write( addr + EE_TC_HW_TCX0_CCC_REG_OFS, abs_dc );

    }

  }
#endif /* __PWM_C_0_CCC__ || __PWM_D_0_CCC__ || __PWM_E_0_CCC__ */

#if	( \
  defined(__PWM_C_0_CCD__) || \
  defined(__PWM_D_0_CCD__) || \
  defined(__PWM_E_0_CCD__) \
)
  if ( ccx_div & EE_TC_HW_CCD_ENABLE ) {

    EE_ATxmega_Reg16Write( addr + EE_TC_HW_TCX0_CCDBUF_REG_OFS, abs_dc );

    if ( ! ( stat & EE_PWM_DEU_MASK ) ) {

      EE_ATxmega_Reg16Write( addr + EE_TC_HW_TCX0_CCD_REG_OFS, abs_dc );

    }

  }
#endif /* __PWM_C_0_CCD__ || __PWM_D_0_CCD__ || __PWM_E_0_CCD__ */

  if ( EE_HWREG(addr + EE_TC_HW_TCX0_CTRLA_REG_OFS) == TC_CLKSEL_OFF_gc ) {

    EE_ATxmega_Reg16Write( addr + EE_TC_HW_TCX0_CNT_REG_OFS, EE_OFF );

    EE_HWREG(addr + EE_TC_HW_TCX0_CTRLB_REG_OFS) = (
      ( ccx_div & EE_TC_HW_CCX_EN_MASK ) |
      TC_WGMODE_SS_gc
    );

    EE_HWREG(addr + EE_TC_HW_TCX0_CTRLA_REG_OFS) = ( 
      ccx_div & ~EE_TC_HW_CCX_EN_MASK 
    );

  }

#endif	/* __USE_TC_HW__ */

}	/* EE_Pwm_SetDutyCycle_Internal() */

#ifdef	__PWM_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_Pwm_SetPeriodAndDuty(
  EE_TYPEPWMCHANNEL	Channel,
  EE_TYPEPWMPERIOD	Period,
  EE_TYPEPWMDUTYCYCLE	DutyCycle
) {

#ifdef	DEBUG
  volatile EE_UREG	flags;
  volatile EE_UREG	stat;
  volatile EE_UINT16	abs_dc;
  volatile EE_HWREG_PTR	addr;
#else
  register EE_UREG	flags;
  register EE_UREG	stat;
  register EE_UINT16	abs_dc;
  register EE_HWREG_PTR	addr;
#endif

#ifdef	__PWM_EXTENDED_STATUS__

  /* Channel Validation */
  EE_ATXMEGA_VALIDATE_W_RV(
#ifdef	__USE_TC_HW__
    ( Channel < EE_TC_HW_ALL_UNITS_MASK ),
#else	/* __USE_TC_HW__ */
    ( Channel < EE_TC_HW_ALL_TCX0_MASK ),
#endif	/* __USE_TC_HW__ */
    E_PWM_PARAM_CHANNEL
  );

  /* Period Validation */
  EE_ATXMEGA_VALIDATE_W_RV( 
    ( 
      ( Period >= EE_PWM_ABS_PERIOD_MIN ) &&
      ( Period <= EE_PWM_ABS_PERIOD_MAX )
    ),
    E_PWM_PARAM_PERIOD
  );

  /* Duty-Cycle Validation */
  EE_ATXMEGA_VALIDATE_W_RV(
    ( DutyCycle <= EE_PWM_DUTY_CYCLE_MAX ),
    E_PWM_PARAM_PERIOD
  );

#endif	/* __PWM_EXTENDED_STATUS__ */

  /* Suspend Interrupt */
  flags = EE_hal_suspendIRQ();

#ifdef	__USE_TC_HW__
  stat = EE_Tc_Hw_GetStatus(Channel);
#else	/* __USE_TC_HW__ */
  stat = EE_Tc_Hw_Status[Channel];
#endif	/* __USE_TC_HW__ */

#ifdef	__PWM_EXTENDED_STATUS__

  EE_ATXMEGA_VALIDATE_IRQ_W_RV(
    ( stat & EE_TC_HW_INIT_MASK ),
    E_NOT_OK,
    flags
  );

#endif	/* __PWM_EXTENDED_STATUS__ */

#ifdef	__USE_TC_HW__
  addr = EE_TC_HW_2_ADDR(Channel);
#else	/* __USE_TC_HW__ */
  addr = EE_TC_HW_TCX0_2_ADDR(Channel);
#endif	/* __USE_TC_HW__ */

#ifdef	__PWM_EXTENDED_STATUS__

  EE_ATXMEGA_VALIDATE_IRQ_W_RV(
    ( 
      !( stat & EE_PWM_FP_MASK ) ||
#ifdef	__USE_TC_HW__
      ( EE_HWREG(addr + EE_TC_HW_CTRLA_REG_OFS) == TC_CLKSEL_OFF_gc )
#else	/* __USE_TC_HW__ */
      ( EE_HWREG(addr + EE_TC_HW_TCX0_CTRLA_REG_OFS) == TC_CLKSEL_OFF_gc )
#endif	/* __USE_TC_HW__ */
    ),
    E_PWM_FIXED_PERIOD,
    flags
  );

#endif	/* __PWM_EXTENDED_STATUS__ */

#ifdef	__USE_TC_HW__
  EE_ATxmega_Reg16Write( addr + EE_TC_HW_PERBUF_REG_OFS, Period );
#else	/* __USE_TC_HW__ */
  EE_ATxmega_Reg16Write( addr + EE_TC_HW_TCX0_PERBUF_REG_OFS, Period );
#endif	/* __USE_TC_HW__ */

  if ( !( stat & EE_PWM_PEU_MASK ) ) {

#ifdef	__USE_TC_HW__
    EE_ATxmega_Reg16Write( addr + EE_TC_HW_PER_REG_OFS, Period );
#else	/* __USE_TC_HW__ */
    EE_ATxmega_Reg16Write( addr + EE_TC_HW_TCX0_PER_REG_OFS, Period );
#endif	/* __USE_TC_HW__ */

  }

  abs_dc = EE_Pwm_AbsDutyCycle_Internal( Period, DutyCycle );

#ifdef	__USE_TC_HW__
  EE_Pwm_SetDutyCycle_Internal(
    addr, abs_dc, stat, EE_Pwm_Ccx_GetDiv(Channel)
  );
#else	/* __USE_TC_HW__ */
  EE_Pwm_SetDutyCycle_Internal(
    addr,
    abs_dc,
    stat,
#ifdef	__AVR_PGMSPACE__
    (EE_UREG) pgm_read_bye(&EE_Pwm_Ccx_Div[Channel])
#else	/* __AVR_PGMSPACE__ */
    EE_Pwm_Ccx_Div[Channel]
#endif	/* __AVR_PGMSPACE__ */
  );
#endif	/* __USE_TC_HW__ */

  /* Resume IRQs */
  EE_hal_resumeIRQ(flags);

#ifdef	__PWM_EXTENDED_STATUS__
  return E_OK;
#endif

}

#ifdef	__PWM_SETDUTYCYCLE_API__
#ifdef	__PWM_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_Pwm_SetDutyCycle(
  EE_TYPEPWMCHANNEL	Channel,
  EE_TYPEPWMDUTYCYCLE	DutyCycle
) {

#ifdef	DEBUG
  volatile EE_UREG	flags;
  volatile EE_UREG	stat;
  volatile EE_UINT16	period;
  volatile EE_UINT16	abs_dc;
  volatile EE_HWREG_PTR	addr;
#else
  register EE_UREG	flags;
  register EE_UREG	stat;
  register EE_UINT16	period;
  register EE_UINT16	abs_dc;
  register EE_HWREG_PTR	addr;
#endif

#ifdef	__PWM_EXTENDED_STATUS__

  /* Channel Validation */
  EE_ATXMEGA_VALIDATE_W_RV( 
#ifdef	__USE_TC_HW__
    ( Channel < EE_TC_HW_ALL_UNITS_MASK ),
#else	/* __USE_TC_HW__ */
    ( Channel < EE_TC_HW_ALL_TCX0_MASK ),
#endif	/* __USE_TC_HW__ */
    E_PWM_PARAM_CHANNEL
  );

  /* Duty-Cycle Validation */
  EE_ATXMEGA_VALIDATE_W_RV( 
    ( DutyCycle <= EE_PWM_DUTY_CYCLE_MAX ),
    E_PWM_PARAM_PERIOD
  );

#endif	/* __PWM_EXTENDED_STATUS__ */

  /* Suspend Interrupt */
  flags = EE_hal_suspendIRQ();

#ifdef	__USE_TC_HW__
  stat = EE_Tc_Hw_GetStatus(Channel);
#else	/* __USE_TC_HW__ */
  stat = EE_Tc_Hw_Status[Channel];
#endif	/* __USE_TC_HW__ */

#ifdef	__PWM_EXTENDED_STATUS__

  EE_ATXMEGA_VALIDATE_IRQ_W_RV(
    ( stat & EE_TC_HW_INIT_MASK ),
    E_NOT_OK,
    flags
  );

#endif	/* __PWM_EXTENDED_STATUS__ */

#ifdef	__USE_TC_HW__
  addr = EE_TC_HW_2_ADDR(Channel);

  period = EE_ATxmega_Reg16Read( addr + EE_TC_HW_PER_REG_OFS );
#else	/* __USE_TC_HW__ */
  addr = EE_TC_HW_TCX0_2_ADDR(Channel);

  period = EE_ATxmega_Reg16Read( addr + EE_TC_HW_TCX0_PER_REG_OFS );
#endif	/* __USE_TC_HW__ */

#ifdef	__PWM_EXTENDED_STATUS__

  EE_ATXMEGA_VALIDATE_IRQ_W_RV(
    ( 
      ( period >= EE_PWM_ABS_PERIOD_MIN ) &&
      ( period <= EE_PWM_ABS_PERIOD_MAX )
    ),
    E_PWM_PARAM_PERIOD,
    flags
  );

#endif	/* __PWM_EXTENDED_STATUS__ */

  abs_dc = EE_Pwm_AbsDutyCycle_Internal( period, DutyCycle );

#ifdef	__USE_TC_HW__
  EE_Pwm_SetDutyCycle_Internal(
    addr, abs_dc, stat, EE_Pwm_Ccx_GetDiv(Channel)
  );
#else	/* __USE_TC_HW__ */
  EE_Pwm_SetDutyCycle_Internal(
    addr,
    abs_dc,
    stat,
#ifdef	__AVR_PGMSPACE__
    (EE_UREG) pgm_read_bye(&EE_Pwm_Ccx_Div[Channel])
#else	/* __AVR_PGMSPACE__ */
    EE_Pwm_Ccx_Div[Channel]
#endif	/* __AVR_PGMSPACE__ */
  );
#endif	/* __USE_TC_HW__ */

  /* Resume IRQs */
  EE_hal_resumeIRQ(flags);

#ifdef	__PWM_EXTENDED_STATUS__
  return E_OK;
#endif

}
#endif	/* __PWM_SETDUTYCYCLE_API__ */

#ifdef	__PWM_SETOUTPUTTOIDLE_API__
#ifdef	__PWM_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_Pwm_SetOutputToIdle(
  EE_TYPEPWMCHANNEL	Channel
) {

#ifdef	DEBUG
  volatile EE_UREG	flags;
  volatile EE_UREG	stat;
  volatile EE_UINT16	period;
  volatile EE_HWREG_PTR	addr;
#else
  register EE_UREG	flags;
  register EE_UREG	stat;
  register EE_UINT16	period;
  register EE_HWREG_PTR	addr;
#endif

#ifdef	__PWM_EXTENDED_STATUS__

  /* Channel Validation */
  EE_ATXMEGA_VALIDATE_W_RV( 
#ifdef	__USE_TC_HW__
    ( Channel < EE_TC_HW_ALL_UNITS_MASK ),
#else	/* __USE_TC_HW__ */
    ( Channel < EE_TC_HW_ALL_TCX0_MASK ),
#endif	/* __USE_TC_HW__ */
    E_PWM_PARAM_CHANNEL
  );

#endif	/* __PWM_EXTENDED_STATUS__ */

  /* Suspend Interrupt */
  flags = EE_hal_suspendIRQ();

#ifdef	__USE_TC_HW__
  stat = EE_Tc_Hw_GetStatus(Channel);
#else	/* __USE_TC_HW__ */
  stat = EE_Tc_Hw_Status[Channel];
#endif	/* __USE_TC_HW__ */

#ifdef	__PWM_EXTENDED_STATUS__

  EE_ATXMEGA_VALIDATE_IRQ_W_RV(
    ( stat & EE_TC_HW_INIT_MASK ),
    E_NOT_OK,
    flags
  );

#endif	/* __PWM_EXTENDED_STATUS__ */

#ifdef	__USE_TC_HW__
  addr = EE_TC_HW_2_ADDR(Channel);

  if ( stat & EE_PWM_IH_MASK ) {

    period = EE_OFF;

  }
  else {

    period = EE_ATxmega_Reg16Read( addr + EE_TC_HW_PER_REG_OFS );

  }

  EE_Pwm_SetDutyCycle_Internal(
    addr, period, stat, EE_Pwm_Ccx_GetDiv(Channel)
  );

  EE_HWREG(addr + EE_TC_HW_INTFLAGS_REG_OFS) = EE_TC_HW_ALL_INT_MASK;

  /* Wait Next PWM Period. */
  while ( 
    !( 
	EE_HWREG(addr + EE_TC_HW_INTFLAGS_REG_OFS) &
	EE_TC_HW_BASE_MASK
    ) 
  );

  EE_HWREG(addr + EE_TC_HW_CTRLA_REG_OFS) = TC_CLKSEL_OFF_gc;

  EE_HWREG(addr + EE_TC_HW_INTFLAGS_REG_OFS) = EE_TC_HW_ALL_INT_MASK;
#else	/* __USE_TC_HW__ */
  addr = EE_TC_HW_TCX0_2_ADDR(Channel);

  if ( stat & EE_PWM_IH_MASK ) {

    period = EE_OFF;

  }
  else {

    period = EE_ATxmega_Reg16Read( addr + EE_TC_HW_TCX0_PER_REG_OFS );

  }

  EE_Pwm_SetDutyCycle_Internal(
    addr,
    period,
    stat,
#ifdef	__AVR_PGMSPACE__
    (EE_UREG) pgm_read_byte(&EE_Pwm_Ccx_Div[Channel])
#else	/* __AVR_PGMSPACE__ */
    EE_Pwm_Ccx_Div[Channel]
#endif	/* __AVR_PGMSPACE__ */
  );

  EE_HWREG(addr + EE_TC_HW_TCX0_INTFLAGS_REG_OFS) = EE_TC_HW_ALL_INT_MASK;

  /* Wait Next PWM Period. */
  while ( 
    !( 
	EE_HWREG(addr + EE_TC_HW_TCX0_INTFLAGS_REG_OFS) &
	EE_TC_HW_TCX0_BASE_MASK
    ) 
  );

  EE_HWREG(addr + EE_TC_HW_TCX0_CTRLA_REG_OFS) = TC_CLKSEL_OFF_gc;

  EE_HWREG(addr + EE_TC_HW_TCX0_INTFLAGS_REG_OFS) = EE_TC_HW_ALL_INT_MASK;
#endif	/* __USE_TC_HW__ */

  /* Resume IRQs */
  EE_hal_resumeIRQ(flags);

#ifdef	__PWM_EXTENDED_STATUS__
  return E_OK;
#endif

}
#endif	/* __PWM_SETOUTPUTTOIDLE_API__ */

#ifdef	__PWM_GETOUTPUTSTATE_API__
/*
 * EE_Pwm_GetOutputStates() Implementation.
 */
EE_Pwm_GetOutputState(
  EE_TYPEPWMCHANNEL	Channel,
  EE_UINT8		*State
){

#ifdef	__USE_TC_HW__

#ifdef	DEBUG
  volatile EE_UREG	flags;
  volatile EE_UREG	stat;
  volatile EE_UREG	ccx_div;
  volatile EE_UINT16	abs_dc;
  volatile EE_HWREG_PTR	addr;
#else
  register EE_UREG	flags;
  register EE_UREG	stat;
  register EE_UREG	ccx_div;
  register EE_UINT16	abs_dc;
  register EE_HWREG_PTR	addr;
#endif

#ifdef	__PWM_EXTENDED_STATUS__

  /* State Validation */
  EE_ATXMEGA_VALIDATE_W_RV( 
    ( State != EE_NULL_PTR ),
    EE_PWM_PARAM_STATE
  );

#endif	/* __PWM_EXTENDED_STATUS__ */

  (*Status) = STD_LOW;

#ifdef	__PWM_EXTENDED_STATUS__

  /* Channel Validation */
  EE_ATXMEGA_VALIDATE_W_RV( 
    ( Channel < EE_TC_HW_ALL_UNITS_MASK ),
    EE_PWM_CH_UNINIT
  );

#endif	/* __PWM_EXTENDED_STATUS__ */

  /* Suspend Interrupt */
  flags = EE_hal_suspendIRQ();

  stat = EE_Tc_Hw_GetStatus(Channel);

#ifdef	__PWM_EXTENDED_STATUS__

  /* Channel Initialization Validation */
  EE_ATXMEGA_VALIDATE_IRQ_W_RV(
    ( stat & EE_TC_HW_INIT_MASK ),
    E_NOT_OK,
    flags
  );

#endif	/* __PWM_EXTENDED_STATUS__ */

  addr = EE_EE_TC_HW_2_ADDR(Channel);

  if (
    ( EE_HWREG(addr + EE_TC_HW_CTRLB_REG_OFS) == TC_CLKSEL_OFF_gc ) &&
    ( stat & EE_PWM_IH_MASK )
  ) {
  
    (*State) = STD_HIGH;
  }
  else {

    abs_dc = EE_OFF;

    ccx_div = EE_Pwm_Ccx_GetDiv(Channel);

#if	( defined(__AVR_ATxmega16D4__) || defined(__AVR_ATxmega32D4__) )

#else	/* __AVR_ATxmega16D4__ || __AVR_ATxmega32D4__ */

#if	( \
  defined(__PWM_C_0_CCA__) || defined(__PWM_D_0_CCA__) || \
  defined(__PWM_E_0_CCA__) || defined(__PWM_F_0_CCA__) || \
  defined(__PWM_C_1_CCA__) || defined(__PWM_D_1_CCA__) || \
  defined(__PWM_E_1_CCA__) || defined(__PWM_F_1_CCA__) \
)
    if ( ccx_div & EE_TC_HW_CCA_ENABLE ) {

      abs_dc = EE_ATxmega_Reg16Read( addr + EE_TC_HW_CCA_REG_OFS );

    }
#endif	/*
	 * __PWM_C_0_CCA__ || __PWM_D_0_CCA__ ||
	 * __PWM_E_0_CCA__ || __PWM_F_0_CCA__ ||
	 * __PWM_C_1_CCA__ || __PWM_D_1_CCA__ ||
	 * __PWM_E_1_CCA__ || __PWM_F_1_CCA__
	 */

#if	( \
  defined(__PWM_C_0_CCB__) || defined(__PWM_D_0_CCB__) || \
  defined(__PWM_E_0_CCB__) || defined(__PWM_F_0_CCB__) || \
  defined(__PWM_C_1_CCB__) || defined(__PWM_D_1_CCB__) || \
  defined(__PWM_E_1_CCB__) || defined(__PWM_F_1_CCB__) \
)
    if ( ccx_div & EE_TC_HW_CCB_ENABLE ) {

      abs_dc = EE_ATxmega_Reg16Read( addr + EE_TC_HW_CCB_REG_OFS );

    }
#endif	/*
	 * __PWM_C_0_CCB__ || __PWM_D_0_CCB__ ||
	 * __PWM_E_0_CCB__ || __PWM_F_0_CCB__ ||
	 * __PWM_C_1_CCB__ || __PWM_D_1_CCB__ ||
	 * __PWM_E_1_CCB__ || __PWM_F_1_CCB__
	 */

#if	( \
  defined(__PWM_C_0_CCC__) || defined(__PWM_D_0_CCC__) || \
  defined(__PWM_E_0_CCC__) || defined(__PWM_F_0_CCC__) \
)
    if ( ccx_div & EE_TC_HW_CCC_ENABLE ) {

      abs_dc = EE_ATxmega_Reg16Read( addr + EE_TC_HW_CCC_REG_OFS );

    }
#endif	/*
	 * __PWM_C_0_CCC__ || __PWM_D_0_CCC__ ||
	 * __PWM_E_0_CCC__ || __PWM_F_0_CCC__
	 */

#if	( \
  defined(__PWM_C_0_CCD__) || defined(__PWM_D_0_CCD__) || \
  defined(__PWM_E_0_CCD__) || defined(__PWM_F_0_CCD__) \
)
    if ( ccx_div & EE_TC_HW_CCD_ENABLE ) {

      abs_dc = EE_ATxmega_Reg16Read( addr + EE_TC_HW_TCX0_CCD_REG_OFS );

    }
#endif	/*
	 * __PWM_C_0_CCD__ || __PWM_D_0_CCD__ ||
	 * __PWM_E_0_CCD__ || __PWM_F_0_CCD__
	 */

#endif	/* __AVR_ATxmega16D4__ || __AVR_ATxmega32D4__ */

    if ( EE_ATxmega_Reg16Read( addr + EE_TC_HW_CNT_REG_OFS ) > abs_dc ) {

      (*State) = STD_HIGH;

    }

  }

  /* Resume Interrupt */
  EE_hal_resumeIRQ(flags);

  return EE_PWM_CH_OPERATIONAL;

#else	/* __USE_TC_HW__ */

#ifdef	DEBUG
  volatile EE_UREG	flags;
  volatile EE_UREG	stat;
  volatile EE_UREG	ccx_div;
  volatile EE_UINT16	abs_dc;
  volatile EE_HWREG_PTR	addr;
#else
  register EE_UREG	flags;
  register EE_UREG	stat;
  register EE_UREG	ccx_div;
  register EE_UINT16	abs_dc;
  register EE_HWREG_PTR	addr;
#endif

#ifdef	__PWM_EXTENDED_STATUS__

  /* State Validation */
  EE_ATXMEGA_VALIDATE_W_RV( 
    ( State != EE_NULL_PTR ),
    EE_PWM_PARAM_STATE
  );

#endif	/* __PWM_EXTENDED_STATUS__ */

  (*Status) = STD_LOW;

#ifdef	__PWM_EXTENDED_STATUS__

  /* Channel Validation */
  EE_ATXMEGA_VALIDATE_W_RV( 
    ( Channel < EE_TC_HW_ALL_TCX0_MASK ),
    EE_PWM_CH_UNINIT
  );

#endif	/* __PWM_EXTENDED_STATUS__ */

  /* Suspend Interrupt */
  flags = EE_hal_suspendIRQ();

  stat = EE_Tc_Hw_Status[Channel];

#ifdef	__PWM_EXTENDED_STATUS__

  /* Channel Initialization Validation */
  EE_ATXMEGA_VALIDATE_IRQ_W_RV(
    ( stat & EE_TC_HW_INIT_MASK ),
    E_NOT_OK,
    flags
  );

#endif	/* __PWM_EXTENDED_STATUS__ */

  addr = EE_EE_TC_HW_TCX0_2_ADDR(Channel);

  if (
    ( EE_HWREG(addr + EE_TC_HW_TCX0_CTRLB_REG_OFS) == TC_CLKSEL_OFF_gc ) &&
    ( stat & EE_PWM_IH_MASK )
  ) {
  
    (*State) = STD_HIGH;
  }
  else {

    abs_dc = EE_OFF;

#ifdef	__AVR_PGMSPACE__
    ccx_div = (EE_UREG) pgm_read_byte(&EE_Pwm_Ccx_Div[Channel]);
#else	/* __AVR_PGMSPACE__ */
    ccx_div = EE_Pwm_Ccx_Div[Channel];
#endif	/* __AVR_PGMSPACE__ */

#if	( \
  defined(__PWM_C_0_CCA__) || \
  defined(__PWM_D_0_CCA__) || \
  defined(__PWM_E_0_CCA__) \
)
    if ( ccx_div & EE_TC_HW_CCA_ENABLE ) {

      abs_dc = EE_ATxmega_Reg16Read( addr + EE_TC_HW_TCX0_CCA_REG_OFS );

    }
#endif /* __PWM_C_0_CCA__ || __PWM_D_0_CCA__ || __PWM_E_0_CCA__ */

#if	( \
  defined(__PWM_C_0_CCB__) || \
  defined(__PWM_D_0_CCB__) || \
  defined(__PWM_E_0_CCB__) \
)
    if ( ccx_div & EE_TC_HW_CCB_ENABLE ) {

      abs_dc = EE_ATxmega_Reg16Read( addr + EE_TC_HW_TCX0_CCB_REG_OFS );

    }
#endif /* __PWM_C_0_CCB__ || __PWM_D_0_CCB__ || __PWM_E_0_CCB__ */

#if	( \
  defined(__PWM_C_0_CCC__) || \
  defined(__PWM_D_0_CCC__) || \
  defined(__PWM_E_0_CCC__) \
)
    if ( ccx_div & EE_TC_HW_CCC_ENABLE ) {

      abs_dc = EE_ATxmega_Reg16Read( addr + EE_TC_HW_TCX0_CCC_REG_OFS );

    }
#endif /* __PWM_C_0_CCC__ || __PWM_D_0_CCC__ || __PWM_E_0_CCC__ */

#if	( \
  defined(__PWM_C_0_CCD__) || \
  defined(__PWM_D_0_CCD__) || \
  defined(__PWM_E_0_CCD__) \
)
    if ( ccx_div & EE_TC_HW_CCD_ENABLE ) {

      abs_dc = EE_ATxmega_Reg16Read( addr + EE_TC_HW_TCX0_CCD_REG_OFS );

    }
#endif /* __PWM_C_0_CCD__ || __PWM_D_0_CCD__ || __PWM_E_0_CCD__ */

    if ( EE_ATxmega_Reg16Read( addr + EE_TC_HW_TCX0_CNT_REG_OFS ) > abs_dc ) {

      (*State) = STD_HIGH;

    }

  }

  /* Resume Interrupt */
  EE_hal_resumeIRQ(flags);

  return EE_PWM_CH_OPERATIONAL;

#endif	/* __USE_TC_HW__ */

};
#endif	/* __PWM_GETOUTPUTSTATE_API__ */