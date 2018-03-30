/** @file	ee_atmel_atxmega_pmic.h
 *
 *  @brief	Erika Enterprise - PMIC Driver - Atmel ATXMEGA MCUs Header File.
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
 *  Programmable Multilevel Interrupt Controller Driver Header file.
 *
 *  @author	Giuseppe Serano
 *  @version	0.1
 *  @date	2013
 */

#ifndef	__INCLUDE_ATMEL_ATXMEGA_PMIC_H__
#define	__INCLUDE_ATMEL_ATXMEGA_PMIC_H__

#include <avr/io.h>

/** @brief	PMIC Initialization.
 *
 *  Enables all interrupt levels and fixed priority scheduling.
 */
__INLINE__ void __ALWAYS_INLINE__ EE_Pmic_Init(void)
{
  PMIC.CTRL |= ( PMIC_LOLVLEN_bm | PMIC_MEDLVLEN_bm | PMIC_HILVLEN_bm );
}

/** @brief	PMIC De-Initialization.
 *
 *  Disables all interrupt levels and fixed priority scheduling.
 */
__INLINE__ void __ALWAYS_INLINE__ EE_Pmic_DeInit(void)
{
  PMIC.CTRL &= ~( PMIC_LOLVLEN_bm | PMIC_MEDLVLEN_bm | PMIC_HILVLEN_bm );
}

/** @brief	PMIC Status Retrieval. */
__INLINE__ EE_UREG __ALWAYS_INLINE__ EE_Pmic_GetStatus(void)
{
  return PMIC.STATUS;
}

#endif	/* __INCLUDE_ATMEL_ATXMEGA_PMIC_H__ */
