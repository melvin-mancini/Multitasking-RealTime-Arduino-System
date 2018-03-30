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
 * Erika Enterprise - TCX0 Driver - Atmel ATXMEGA MCUs Implementation File.
 *
 * Author: 2013,  Giuseppe Serano
 */

/* ERIKA Enterprise Internals */
#include "ee_internal.h"

/* Erika Enterprise - TCX0 - Atmel ATXMEGA MCUs Header File. */
#include "mcu/atmel_atxmega/inc/ee_atmel_atxmega_tcx0.h"

/*
 * Timer(s)/Counter(s) Hardware Units Status Array.
 */
#ifdef	DEBUG
volatile EE_UINT8
#else
EE_UINT8
#endif
EE_Tc_Hw_Status[EE_TC_HW_UNITS_NUMBER] = {

/* TCC0 */
#ifdef	EE_AVR8_TCC0_OVF_ISR_PRI
EE_AVR8_TCC0_OVF_ISR_PRI,
#else
EE_ISR_PRI_OFF,
#endif

/* TCD0 */
#ifdef	EE_AVR8_TCD0_OVF_ISR_PRI
EE_AVR8_TCD0_OVF_ISR_PRI,
#else
EE_ISR_PRI_OFF,
#endif

/* TCE0 */
#ifdef	EE_AVR8_TCE0_OVF_ISR_PRI
EE_AVR8_TCE0_OVF_ISR_PRI,
#else
EE_ISR_PRI_OFF,
#endif

};

#if	( \
  defined( EE_AVR8_TCC0_OVF_ISR ) || \
  defined( EE_AVR8_TCC0_CCA_ISR ) || defined( EE_AVR8_TCC0_CCB_ISR ) || \
  defined( EE_AVR8_TCC0_CCC_ISR ) || defined( EE_AVR8_TCC0_CCD_ISR ) || \
  defined( EE_AVR8_TCD0_OVF_ISR ) || \
  defined( EE_AVR8_TCD0_CCA_ISR ) || defined( EE_AVR8_TCD0_CCB_ISR ) || \
  defined( EE_AVR8_TCD0_CCC_ISR ) || defined( EE_AVR8_TCD0_CCD_ISR ) || \
  defined( EE_AVR8_TCE0_OVF_ISR ) || \
  defined( EE_AVR8_TCE0_CCA_ISR ) || defined( EE_AVR8_TCE0_CCB_ISR ) || \
  defined( EE_AVR8_TCE0_CCC_ISR ) || defined( EE_AVR8_TCE0_CCD_ISR ) \
)

/*
 * Timers(s)/Counters(s) Hardware Units Callback Array.
 */
#ifdef	DEBUG
volatile EE_ISR_callback
#else
EE_ISR_callback
#endif
#if	0
EE_Tc_Hw_CallBack[EE_TC_HW_UNITS_NUMBER] = { EE_NULL_PTR };
#else	/* 0 */
EE_Tc_Hw_CallBack[EE_TC_HW_UNITS_NUMBER];
#endif	/* 0 */

#endif	/*
	 * EE_AVR8_TCC0_OVF_ISR ||
	 * EE_AVR8_TCC0_CCA_ISR || EE_AVR8_TCC0_CCB_ISR ||
	 * EE_AVR8_TCC0_CCC_ISR || EE_AVR8_TCC0_CCD_ISR ||
	 * EE_AVR8_TCD0_OVF_ISR ||
	 * EE_AVR8_TCD0_CCA_ISR || EE_AVR8_TCD0_CCB_ISR ||
	 * EE_AVR8_TCD0_CCC_ISR || EE_AVR8_TCD0_CCD_ISR ||
	 * EE_AVR8_TCE0_OVF_ISR ||
	 * EE_AVR8_TCC0_CCA_ISR || EE_AVR8_TCC0_CCB_ISR ||
	 * EE_AVR8_TCC0_CCC_ISR || EE_AVR8_TCC0_CCD_ISR
	 */
