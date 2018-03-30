/** @file	ee_mcu_common_types.h
 *
 *  @brief	Erika Enterprise - TYPES Definitions - MCU Common Header File.
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
 *  This files contains all types and symbols for ERIKA Enterprise MCUs.
 *  Those types must be abstracted in order to become platform and compiler
 *  independend.
 * 
 *  @author	Riccardo Schiavi
 *  @author	Giuseppe Serano
 *  @version	0.1
 *  @date	2013
 */

#ifndef	__EE_MCU_COMMON_TYPES_H__
#define	__EE_MCU_COMMON_TYPES_H__

/******************************************************************************
 MCU dependent data types
 ******************************************************************************/
 
#ifdef __HAS_MCU_TYPES_H__
#include "ee_mcu_types.h"
#endif

/******************************************************************************
 MCU independent data types
 ******************************************************************************/

/*
 * The symbols EE_TRUE and EE_FALSE shall be defined as follows:
 */
#ifndef	EE_NULL_PTR
#define	EE_NULL_PTR	0x00U	/**< Pointer value NULL. */
#endif

/** @brief	Standard Return Type
 *
 *  This type can be used as standard API return type which is shared between
 *  the MCU modules.
 *
 *  The <tt>EE_TYPERET</tt> shall normally be used with value <tt>E_OK</tt> or
 *  <tt>E_NOT_OK</tt>. If those return values are not sufficient, MCU modules
 *  specific values can be defined.
 */
#ifndef	EE_TYPERET
#define	EE_TYPERET	EE_SREG
#endif

#ifndef	STATUSTYPEDEFINED
#define	STATUSTYPEDEFINED	/**< OSEK compliance */
#define	E_OK		0x00U	/**< OK		     */

/** @brief	OSEK compliance
 *
 *  Because <tt>E_OK</tt> is already defined within OSEK, the symbol
 *  <tt>E_OK</tt> has to be shared. To avoid name clashes and redefinition
 *  problems, the symbols have to be defined in the following way
 *  (approved within implementation):<br>
 *  <code>
 *    \#ifndef STATUSTYPEDEFINED<br>
 *    \#define STATUSTYPEDEFINED<br>
 *    \#define E_OK	0x00U<br>
 *    typedef unsigned char StatusType;<br>
 *    \#endif<br>
 *    \#define E_NOT_OK	0x01U<br>
 *  </code>
 */
typedef	unsigned char	StatusType;	/* OSEK compliance */
#endif
#define	E_NOT_OK	0xFFU	/**< NOT OK */

/*
 * The symbols EE_TRUE and EE_FALSE shall be defined as follows:
 */
#ifndef	EE_FALSE
#define	EE_FALSE	0x00U	/**< Boolean value FALSE. */
#endif
#ifndef	EE_TRUE
#define	EE_TRUE		0x01U	/**< Boolean value TRUE. */
#endif

/*
 * The symbols EE_HIGH and EE_LOW shall be defined as follows:
 */
#ifndef	EE_LOW
#define	EE_LOW		0x00U	/**< Physical state 0V */
#endif
#ifndef	EE_HIGH
#define	EE_HIGH		0x01U	/**< Physical state 5V or 3.3V */
#endif

/*
 * The symbols EE_ON and EE_OFF shall be defined as follows:
 */
#ifndef	EE_OFF
#define	EE_OFF		0x00U	/**< OFF */
#endif
#ifndef	EE_ON
#define	EE_ON		0x01U	/**< ON */
#endif

/*
 * The symbols EE_ACTIVE and EE_IDLE shall be defined as follows:
 */
#ifndef	EE_IDLE
#define	EE_IDLE		0x00U	/**< Logical state idle   */
#endif
#ifndef	EE_ACTIVE
#define	EE_ACTIVE	0x01U	/**< Logical state active */
#endif

/** @brief	Harware Register Pointer Type */
#define	EE_HWREG_PTR		volatile EE_UREG *

/** @brief	Hardware Register L-Value */
#define	EE_HWREG_ADDR(x)	((EE_HWREG_PTR)(x))

/** @brief	Hardware Register R-Value */
#define	EE_HWREG(x)		(*EE_HWREG_ADDR(x))

#endif	/* __EE_MCU_COMMON_TYPES_H__ */
