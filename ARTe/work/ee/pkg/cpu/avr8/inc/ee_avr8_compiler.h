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
 * Author: 2013,  Giuseppe Serano
 */

#ifndef	__INCLUDE_EE_AVR8_COMPILER_H__
#define	__INCLUDE_EE_AVR8_COMPILER_H__

/*
 * Compiler dependent interface
 */

#include	"cpu/common/inc/ee_compiler_gcc.h"
#include	"cpu/common/inc/ee_stdint.h"

#define EE_PREPROC_VALUE(x)		EE_PREPROC_STRING(x)
#define EE_PREPROC_VAR_VALUE(v)		#v "=" EE_PREPROC_VALUE(v)

/** @def	EE_return()
 *
 *  Returns from a function. This should be the last command executed before
 *  leaving a function defined with the EE_NAKED attribute.
 *
 *  This macro actually compiles into a single line of assembly, so there is
 *   no function call overhead.
 */

#define	EE_return()	__asm__ __volatile__ ("ret" ::)

/** @def	EE_NAKED
 *
 *  The function is created with no prologue or epilogue code.
 *
 *  The user code is responsible for preservation of the machine state including
 *  the SREG register, as well as placing a EE_return() at the end of the
 *  function.

 *   Use this attribute in the attributes parameter of the function.
 */
#define	EE_NAKED	__attribute__((naked))

#endif	/* __INCLUDE_EE_AVR8_COMPILER_H__ */

