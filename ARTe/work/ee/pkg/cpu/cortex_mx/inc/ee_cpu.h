/* ###*B*###
 * ERIKA Enterprise - a tiny RTOS for small microcontrollers
 *
 * Copyright (C) 2002-2011  Evidence Srl
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

/** 
	@file ee_cpu.h
	@brief CPU-dependent part of HAL. Derived from pkg/cpu/pic30/inc/ee_cpu.h
	@author Gianluca Franchino
	@author Giuseppe Serano
	@date 2011
*/ 

#ifndef __INCLUDE_CORTEX_MX_EE_CPU_H__
#define __INCLUDE_CORTEX_MX_EE_CPU_H__

#include "eecfg.h"

#ifdef __IAR__
#include "cpu/common/inc/ee_compiler_iar.h"
#else	/* __IAR__ */
#ifdef __CCS__
#include "cpu/common/inc/ee_compiler_ccs.h"
#else	/* __CCS__ */
#ifdef __KEIL__
#include "cpu/common/inc/ee_compiler_keil.h"
#else	/* __KEIL__ */
#ifdef __GNU__
#include "cpu/common/inc/ee_compiler_gcc.h"
#else	/* __GNU__ */
#error Unsupported compiler
#endif	/* !__GNU__ */
#endif	/* !__KEIL__ */
#endif	/* !__CCS__ */
#endif	/* !__IAR__ */

/* Initial stack offset (in words): Used in multistack. */
#ifndef CORTEX_MX_INIT_TOS_OFFSET
#define CORTEX_MX_INIT_TOS_OFFSET 10
#endif

/*************************************************************************
 HAL Types and structures
 *************************************************************************/

/* Primitive data types */
#include "cpu/common/inc/ee_types.h"

typedef EE_UINT32 EE_UREG;
typedef EE_INT32  EE_SREG;
typedef EE_UINT32 EE_FREG;
#define EE_UREG_SIZE 4

#define	EE_HWREG_PTR		volatile EE_UREG *
#define	EE_HWREG_ADDR(x)	((EE_HWREG_PTR)(x))
#define	EE_HWREG(x)		(*EE_HWREG_ADDR(x))

#ifdef __GNU__

/* Get current SP */
__INLINE__ EE_UINT32 __ALWAYS_INLINE__ __current_sp(void)
{
EE_UINT32 temp;
__ASM ("mov %0, sp" : "=r" (temp) : );
return temp;
}

/* Set a breakpoint */
#define __breakpoint(value)	__ASM ("bkpt "#value)

#endif

/* ISR Priority representation type */
typedef EE_UREG EE_TYPEISR2PRIO;

/* Thread IDs */
typedef EE_INT32 EE_TID;

/* Thread IDs - unsigned version*/
typedef EE_UINT32 EE_UTID;

/* Used by the common layer to decide whether to start a new thread */
#define TID_IS_STACKED_MARK 0x80000000

/* EE_TYPEIRQ is currently unused */

/* XXX: define EE_TIME? */

/* Use the "standard" implementation */
#include "cpu/common/inc/ee_hal_structs.h"

/******************************************************************************
 Application dependent data types
 ******************************************************************************/

#ifdef	__HAS_TYPES_H__
#include	"types.h"
#endif

/******************************************************************************
 CPU Peripherals
 ******************************************************************************/

#ifdef __CORTEX_MX__
#include "cpu/cortex_mx/inc/ee_nvic.h"
#include "cpu/cortex_mx/inc/ee_systick.h"
#endif

/******************************************************************************
 HAL Variables
 ******************************************************************************/

#ifdef __MULTI__

/* Top-of-stack of each private stack */
extern struct EE_TOS EE_cortex_mx_system_tos[];
//#define	EE_std_system_tos EE_cortex_mx_system_tos

/* Index of the current stack */
extern EE_UREG EE_cortex_mx_active_tos;
#define	EE_hal_active_tos EE_cortex_mx_active_tos

/*extern	EE_UREG EE_pic30_thread_tos[];
#define	EE_std_thread_tos EE_pic30_thread_tos */
#endif /* __MULTI__ */

/*********************************************************************
 Cortex M0 interrupt disabling/enabling
 *********************************************************************/

/* Used to check the value returned by EE_cortex_mx_disableIRQ */
#define EE_cortex_mx_are_IRQs_enabled(ie) ((ie) ^ 1)

/**
 * Enable interrupts
 */
__INLINE__ void __ALWAYS_INLINE__ EE_cortex_mx_enableIRQ(void)
{
#ifdef __IAR__
	__ASM ("cpsie i");
#else	/* __IAR__ */
#ifdef __CCS__
	__ASM ("    cpsie    i\n");
#else	/* __CCS__ */
#ifdef __KEIL__
	__enable_irq();
#else	/* __KEIL__ */
	__ASM volatile("cpsie i");
#endif	/* !__KEIL__ */
#endif	/* !__CCS__ */
#endif	/* !__IAR__ */
}

/**
 * Disable interrupts
 */
__INLINE__ void __ALWAYS_INLINE__ EE_cortex_mx_disableIRQ(void)
{
#ifdef __IAR__
	__ASM ("cpsid i");
#else	/* __IAR__ */
#ifdef __CCS__
	__ASM ("    cpsid    i\n");
#else	/* __CCS__ */
#ifdef __KEIL__
	__disable_irq();
#else	/* __KEIL__ */
	__ASM volatile("cpsid i");
#endif	/* !__KEIL__ */
#endif	/* !__CCS__ */
#endif	/* !__IAR__ */
}

/**
 * Resume interrupts
 */
__INLINE__ void __ALWAYS_INLINE__ EE_cortex_mx_resumeIRQ(EE_FREG f)
{
#ifdef __IAR__
	__set_PRIMASK(f);
#else	/* __IAR__ */
#ifdef __CCS__
	__ASM ("    msr    primask, r0\n");
#else	/* __CCS__ */
#ifdef __KEIL__
	register EE_FREG PRIMASK __ASM("primask");
	PRIMASK = f;
#else	/* __KEIL__ */
	__ASM volatile("msr primask, %0" :: "r" (f));
#endif	/* !__KEIL__ */
#endif	/* !__CCS__ */
#endif	/* !__IAR__ */
}

/**
 * Suspend interrupts
 */
__INLINE__ EE_FREG __ALWAYS_INLINE__ EE_cortex_mx_suspendIRQ(void)
{
#ifndef __CCS__
	EE_FREG istat;
#endif
#ifdef __IAR__
	 istat = __get_PRIMASK();
	__ASM ("cpsid i");
#else	/* __IAR__ */
#ifdef __CCS__
	__ASM ("    mrs     r0, primask\n"
	       "    cpsid   i\n"
	       "    bx      lr\n");
#else	/* __CCS__ */
#ifdef __KEIL__
	register EE_FREG PRIMASK __ASM("primask");
	istat = PRIMASK;
	__disable_irq();
#else	/* __KEIL__ */
	__ASM volatile ("mrs %0, primask" : "=r" (istat) );
	__ASM volatile ("cpsid i");
#endif	/* !__KEIL__ */
#endif	/* !__CCS__ */
#endif	/* !__IAR__ */
#ifdef __CCS__
	return 0;
#else
	return istat;
#endif
}

/**
 * Return true (not 0) if IRQs are enabled, 0 (false) if IRQ are disabled.
 */
__INLINE__ EE_UINT32 __ALWAYS_INLINE__ EE_cortex_mx_get_IRQ_enabled(void)
{
#ifndef __CCS__
	EE_UREG ie;
#endif
#ifdef __IAR__
	ie = __get_PRIMASK();
#else	/* __IAR__ */
#ifdef __CCS__
	__ASM ("    MRS     r0, PRIMASK\n"
	       "    EOR     r0, #1\n"
	       "    bx      lr\n");
#else	/* __CCS__ */
#ifdef __KEIL__
	register EE_FREG PRIMASK __ASM("primask");
	ie = PRIMASK;
#else	/* __KEIL__ */
	__ASM volatile ("MRS %0, primask" : "=r" (ie) );
#endif	/* !__KEIL__ */
#endif	/* !__CCS__ */
#endif	/* !__IAR__ */
#ifdef __CCS__
	return 0;
#else
	return EE_cortex_mx_are_IRQs_enabled(ie);
#endif
}

__INLINE__ EE_TYPEISR2PRIO __ALWAYS_INLINE__ EE_cortex_mx_get_int_prio(void)
{
#ifdef __CORTEX_M4__

#ifndef __CCS__
	register EE_UREG prio;
#ifdef	__KEIL__
	register EE_FREG BASEPRI __ASM("basepri");
#endif

#endif

#ifdef __CCS__
	/* NVIC_INT_PRI_S = 5 */
	__ASM ("    MRS     R0, BASEPRI\n"
	       "    ASR     R0, #5\n"
	       "    BLX     LR\n");
#else	/* __CCS__ */
#ifdef __KEIL__
	prio = BASEPRI;
#else	/* __KEIL__ */
#ifdef __GNU__
	__ASM volatile("MRS %0, BASEPRI" : "=r" (prio) :: );
#else
	__ASM volatile("MRS %0, BASEPRI" :: "=r" (prio));
#endif	/* !__GNU__ */
#endif	/* !__KEIL__ */
#endif	/* !__CCS__ */

#ifdef __CCS__
	return 0;
#else
	return prio >> NVIC_INT_PRI_S;
#endif

#else	/* __CORTEX_M4__ */
	return 0;
#endif	/* !__CORTEX_M4__ */
}

__INLINE__ void __ALWAYS_INLINE__ EE_cortex_mx_set_int_prio(
  EE_TYPEISR2PRIO prio
)
{
#ifdef __CORTEX_M4__
#ifdef __CCS__
	/* NVIC_INT_PRI_S = 5 */
	__ASM ("    LSL    R0, #5\n"
	       "    MSR    BASEPRI, R0\n");
#else	/* __CCS__ */
#ifdef __KEIL__
	register EE_FREG BASEPRI __ASM("basepri");
	BASEPRI = (prio << NVIC_INT_PRI_S);
#else	/* __KEIL__ */
#ifdef __GNU__
	__ASM volatile("MSR BASEPRI, %0" :: "r" (prio << NVIC_INT_PRI_S) : );
#else
	__ASM volatile("MSR BASEPRI, %0" :: "r" (prio << NVIC_INT_PRI_S));
#endif	/* !__GNU__ */
#endif	/* !__KEIL__ */
#endif	/* !__CCS__ */
#endif	/* __CORTEX_M4__ */
}

__INLINE__ EE_TYPEISR2PRIO __ALWAYS_INLINE__ EE_cortex_mx_get_isr_prio(
  void
)
{
#ifdef __CORTEX_M4__
  register EE_UREG vectact = NVIC_INT_CTRL_R & NVIC_INT_CTRL_VEC_ACT_M;
  register EE_TYPEISR2PRIO prio = 0;

  if (vectact > EE_CORTEX_MX_SYSTICK_EXC_NUM) {
    vectact -= EE_CORTEX_MX_SYSTICK_EXC_NUM + 1;
    prio = NVIC_GET_PRI(vectact);
  }
#ifdef	__USE_SYSTICK__
  else if (vectact == EE_CORTEX_MX_SYSTICK_EXC_NUM) {
    prio = NVIC_SYS_PRI3_R >> NVIC_SYS_PRI3_TICK_S;
  }
#endif
#ifdef	__USE_SVC__
  else if (vectact == EE_CORTEX_MX_SVCALL_EXC_NUM) {
    prio = NVIC_SYS_PRI2_R >> NVIC_SYS_PRI2_SVC_S;
  }
#endif

  return prio;

#else	/* __CORTEX_M4__ */
  return 0;
#endif	/* !__CORTEX_M4__ */
}

/*************************************************************************
 Functions exported by the HAL to the kernel
 *************************************************************************/

/* 
 * Interrupt Handling
 */

/** Hal Enable Interrupts */
__INLINE__ void __ALWAYS_INLINE__ EE_hal_enableIRQ(void)
{
    EE_cortex_mx_enableIRQ();
}

/** Hal Disable Interrupts */
__INLINE__ void __ALWAYS_INLINE__ EE_hal_disableIRQ(void)
{
    EE_cortex_mx_disableIRQ();
}

/** Hal Resume Interrupts */
__INLINE__ void __ALWAYS_INLINE__ EE_hal_resumeIRQ(EE_FREG f)
{
    EE_cortex_mx_resumeIRQ(f);
}

/** Hal Suspend Interrupts */
__INLINE__ EE_FREG __ALWAYS_INLINE__ EE_hal_suspendIRQ(void)
{
    return EE_cortex_mx_suspendIRQ();
}

/**************************************************************************
 System Initialization
***************************************************************************/

void EE_system_init(void);

/*************************************************************************
 CPU-dependent ORT support (mainly OTM)
 *************************************************************************/

/* Probably, some parts of the OTM code below does not depend on the
 * architecture.  They should be moved somewhere into pkg/cpu/common if this
 * turns out to be the case. */

#define EE_ORTI_OTM_ID_RUNNINGISR2 1
#define EE_ORTI_OTM_ID_SERVICETRACE 2

#ifdef __OO_ORTI_USE_OTM__
void EE_cortex_mx_send_otm8(EE_UINT8 id, EE_UINT8 data);
void EE_cortex_mx_send_otm32(EE_UINT8 id, EE_UINT32 data);

#else /* if __OO_ORTI_USE_OTM__ */
__INLINE__ void EE_cortex_mx_send_otm8(EE_UINT8 id, EE_UINT8 data)
{
	/* OTM disabled */
}

__INLINE__ void EE_cortex_mx_send_otm32(EE_UINT8 id, EE_UINT32 data)
{
	/* OTM disabled */
}
#endif /* else __OO_ORTI_USE_OTM__ */

#ifdef __OO_ORTI_RUNNINGISR2__
__INLINE__ void EE_ORTI_send_otm_runningisr2(EE_ORTI_runningisr2_type isr2)
{
	EE_cortex_mx_send_otm32(EE_ORTI_OTM_ID_RUNNINGISR2, (EE_UINT32)isr2);
}
#endif /* __OO_ORTI_RUNNINGISR2__ */

#ifdef __OO_ORTI_SERVICETRACE__
__INLINE__ void EE_ORTI_send_otm_servicetrace(EE_UINT8 srv)
{
	EE_cortex_mx_send_otm8(EE_ORTI_OTM_ID_SERVICETRACE, srv);
}

#endif /* __OO_ORTI_SERVICETRACE__ */

#endif /* __INCLUDE_CORTEX_MX_EE_CPU_H__ */
