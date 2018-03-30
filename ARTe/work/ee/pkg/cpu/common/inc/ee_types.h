/* ###*B*###
 * ERIKA Enterprise - a tiny RTOS for small microcontrollers
 *
 * Copyright (C) 2002-2009  Evidence Srl
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
 * Erika types, based on standard types defined in <limits.h>
 * Author: 2009 Bernardo Dal Seno
 */

#ifndef __INCLUDE_CPU_COMMON_EE_TYPES__
#define __INCLUDE_CPU_COMMON_EE_TYPES__

/* For Gcc we could use __XXX_MAX__ built-in macros, instead */
#include <limits.h>

/* Primitive data types */
typedef unsigned char EE_BIT;

#if SCHAR_MAX == 127
typedef unsigned char EE_UINT8;
typedef signed char EE_INT8;
#else
#error Unable to define a 8-bit types
#endif

#if INT_MAX == 32767 /* 16-bit architecture */
typedef int EE_INT16;
typedef unsigned int EE_UINT16;
typedef long int EE_INT32;
typedef unsigned long int EE_UINT32;
#ifdef ULLONG_MAX
#if ULLONG_MAX == 0xffffffffffffffffUL
typedef unsigned long long int EE_UINT64;
#endif
#endif
#elif (INT_MAX == 2147483647) && (SHRT_MAX == 32767) /* 32-bit architecture */
typedef short int EE_INT16;
typedef unsigned short int EE_UINT16;
typedef int EE_INT32;
typedef unsigned int EE_UINT32;
#if (ULONG_MAX > 0xffffffffUL)
typedef unsigned long int EE_UINT64;
#elif defined(ULLONG_MAX) && (ULLONG_MAX > 0xffffffffUL)
typedef unsigned long long int EE_UINT64;
#endif
#else /* Unknown architecture */
#error Unknow/unsupported architecture
#endif

/* HAL may need to declare pointers using keywords such as `far'... */
#ifndef __EE_HAL_CUSTOM_POINTERS__

/* Data addresses (that have the same size of a pointer) */
typedef void *EE_ADDR;

/*
 * Introduced to comply with MISRA 16.7.
 * There may be functions requiring pointer to const)
 */
typedef const void *EE_CONST_ADDR;

/* Code addresses (same size of function pointers) */
typedef void (*EE_FADDR)(void);

/* Callbacks with no parameters nor return value */
typedef void (*EE_VOID_CALLBACK)(void);

/* Thread body pointer (same size of function pointers) */
typedef void (*EE_THREAD_PTR)(void);

/* Callback function called by driver handlers */
typedef EE_VOID_CALLBACK EE_ISR_callback;

#endif /* __EE_HAL_CUSTOM_POINTERS__ */

/** Utility Macro that convert an amount of ms in number of ticks of a given
    frequency **/
#define EE_MILLI_TO_TICKS(X_MS, REF_FREQ_HZ)  \
  ((X_MS) * ((REF_FREQ_HZ) / 1000UL))

/** Utility Macro that convert an amount of us in number of ticks of a given
    frequency **/
#define EE_MICRO_TO_TICKS(X_US, REF_FREQ_HZ)              \
  (((X_US) / 1000UL)?                                     \
      EE_MILLI_TO_TICKS(((X_US) / 1000UL), REF_FREQ_HZ):  \
      EE_MILLI_TO_TICKS(X_US, REF_FREQ_HZ) / 1000UL)

/** Utility Macro that convert an amount of us in number of ticks of a given
    frequency **/
#define MICROSECONDS_TO_TICKS(X_MICROSECS, REF_FREQ_HZ)   \
  EE_MICRO_TO_TICKS(X_MICROSECS, REF_FREQ_HZ)

/** Utility Macro that convert an amount of ms in number of ticks of a given
    frequency **/
#define MILLISECONDS_TO_TICKS(X_MILLISECS, REF_FREQ_HZ)   \
  EE_MILLI_TO_TICKS(X_MILLISECS, REF_FREQ_HZ)

#endif /* __INCLUDE_CPU_COMMON_EE_TYPES__ */
