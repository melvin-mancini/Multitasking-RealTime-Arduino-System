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
 * Erika Enterprise - MCU Driver - Atmel ATXMEGA MCUs Implementation File.
 *
 * Author: 2013,  Giuseppe Serano
 */

/* I need AVR8 ISR2 Stub */
#include "cpu/avr8/inc/ee_avr8_irqstub.h"

#define	EE_MCU_CLK_SRCS_ONLY_FLAG_ALL	( 	\
	EE_MCU_CLK_SRCS_FLAG_RC2MHZ	|	\
	EE_MCU_CLK_SRCS_FLAG_RC32MHZ	|	\
	EE_MCU_CLK_SRCS_FLAG_RC32768HZ	|	\
	EE_MCU_CLK_SRCS_FLAG_XOSC	|	\
	EE_MCU_CLK_SRCS_FLAG_PLL	|	\
	EE_MCU_CLK_SRCS_FLAG_RTC_ENABLE		\
)

#define	EE_MCU_CLK_SRCS_FLAG_MASK		0x0000001FU
#define	EE_MCU_CLK_SRCS_FLAG_SH_BITS		0x00000000U

#define	EE_MCU_CLK_SRCS_FLAG_XOSC_MASK		0x0000FF00U
#define	EE_MCU_CLK_SRCS_FLAG_XOSC_SH_BITS	0x00000008U

#define	EE_MCU_CLK_SRCS_FLAG_RTC_MASK		0x000F0000U
#define	EE_MCU_CLK_SRCS_FLAG_RTC_SH_BITS	0x00000010U

#define	EE_MCU_CLK_SRCS_FLAG_DFLL_MASK		0x00700000U
#define	EE_MCU_CLK_SRCS_FLAG_DFLL_SH_BITS	0x00000014U

#define	EE_MCU_CLK_SRCS_FLAG_PLL_MASK		0xFF000000U
#define	EE_MCU_CLK_SRCS_FLAG_PLL_SH_BITS	0x00000018U

#define	EE_MCU_RESET_FLAG_ALL	( 	\
	EE_MCU_RESET_FLAG_POR	|	\
	EE_MCU_RESET_FLAG_EXT	|	\
	EE_MCU_RESET_FLAG_BOR	|	\
	EE_MCU_RESET_FLAG_WDR	|	\
	EE_MCU_RESET_FLAG_PDIR	|	\
	EE_MCU_RESET_FLAG_SR		\
)

#ifdef	__MCU_EXTENDED_STATUS__
#define	EE_MCU_STAT_UNINIT	0x00U
#define	EE_MCU_STAT_INIT	0x01U
#ifdef	DEBUG
static volatile EE_UREG
#else
static EE_UREG
#endif
#if	0
EE_Mcu_Status = EE_MCU_STAT_UNINIT;
#else	/* 0 */
EE_Mcu_Status;
#endif	/* 0 */
#endif	/* __MCU_EXTENDED_STATUS__ */

/*
 * EE_Mcu_Init() Implementation.
 */
#ifdef	__MCU_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_Mcu_Init(
  EE_TYPEMCUFLAGS	Flags
) {

#ifdef	DEBUG
  volatile EE_UREG	flags;
#else
  register EE_UREG	flags;
#endif

#ifdef	__MCU_EXTENDED_STATUS__

  /* Flags Validation. */
  EE_ATXMEGA_VALIDATE_W_RV(
    ( Flags == EE_MCU_FLAG_DEFAULT ),
    E_MCU_PARAM_INIT_FLAGS
  );

#endif	/* __MCU_EXTENDED_STATUS__ */

  /* Suspend IRQs */
  flags = EE_hal_suspendIRQ();

  /* Interrupt Vector Table Recall. */
  EE_AVR8_REQUEST_IRQ_ENTRY(OSC_XOSCF_vect);

#ifdef	__MCU_EXTENDED_STATUS__

  EE_ATXMEGA_VALIDATE_IRQ_W_RV(
    ( EE_Mcu_Status == EE_MCU_STAT_UNINIT ),
    E_NOT_OK,
    flags
  );

  EE_Mcu_Status = EE_MCU_STAT_INIT;

#endif	/* __MCU_EXTENDED_STATUS__ */

  /* PMIC Initialization */
  EE_Pmic_Init();

  /* Power Reduction Initialization */
  EE_Pr_Init();

  /* Resume IRQs */
  EE_hal_resumeIRQ(flags);

  /* Open OSEK/VDX Kernels Enable Interrupts Inside StartOS(). */
#if	( \
  !defined(__OO_BCC1__) && !defined(__OO_BCC2__) && \
  !defined(__OO_ECC1__) && !defined(__OO_ECC2__) \
)
  /* Interrupt Enabling. */
  EE_hal_enableIRQ();
#endif	/*
	 * defined(__OO_BCC1__) || defined(__OO_BCC2__) ||
	 * defined(__OOSECC1__) || defined(__OO_ECC2__)
	 */

#ifdef	__MCU_EXTENDED_STATUS__
  return E_OK;
#endif

}	/* EE_Mcu_Init() */

#ifdef	__MCU_DEINIT_API__
/*
 * EE_Mcu_DeInit() Implementation.
 */
#ifdef	__MCU_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_Mcu_DeInit(
  EE_TYPEMCUFLAGS	Flags
) {

#ifdef	DEBUG
  volatile EE_UREG	flags;
#else
  register EE_UREG	flags;
#endif

#ifdef	__MCU_EXTENDED_STATUS__

  /* Flags Validation. */
  EE_ATXMEGA_VALIDATE_W_RV(
    ( Flags == EE_MCU_FLAG_DEFAULT ),
    E_MCU_PARAM_INIT_FLAGS
  );

#endif	/* __MCU_EXTENDED_STATUS__ */

  /* Suspend IRQs */
  flags = EE_hal_suspendIRQ();

#ifdef	__MCU_EXTENDED_STATUS__

  EE_ATXMEGA_VALIDATE_IRQ_W_RV(
    ( EE_Mcu_Status == EE_MCU_STAT_INIT ),
    E_NOT_OK,
    flags
  );

  EE_Mcu_Status = EE_MCU_STAT_UNINIT;

#endif	/* __MCU_EXTENDED_STATUS__ */

  /* Resume IRQs */
  EE_hal_resumeIRQ(flags);

  /* Open OSEK/VDX Kernels Disable Interrupts Inside ShutdownOS(). */
#if	( \
  !defined(__OO_BCC1__) && !defined(__OO_BCC2__) && \
  !defined(__OO_ECC1__) && !defined(__OO_ECC2__) \
)
  /* Interrupt Disabling */
  EE_hal_disableIRQ();
#endif	/*
	 * defined(__OO_BCC1__) || defined(__OO_BCC2__) ||
	 * defined(__OOSECC1__) || defined(__OO_ECC2__)
	 */

  /* Power Reduction De-Initialization */
  EE_Pr_DeInit();

  /* PMIC De-Initialization */
  EE_Pmic_DeInit();

#ifdef	__MCU_EXTENDED_STATUS__
  return E_OK;
#endif

}	/* EE_Mcu_DeInit() */
#endif	/* __MCU_DEINIT_API__ */

#ifdef __MCU_INITCLOCK_API__
/*
 * EE_Mcu_InitClock() Implementation.
 */
#ifdef	__MCU_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_Mcu_InitClock(
  EE_TYPEMCUCLKSRCSFLAGS	SrcsFlags
) {

#ifdef	DEBUG
  volatile EE_UREG	flags;
#else
  register EE_UREG	flags;
#endif

#ifdef	__MCU_EXTENDED_STATUS__

  /* SrcsFlags Validation. */
  EE_ATXMEGA_VALIDATE_W_RV(
    ( SrcsFlags & EE_MCU_CLK_SRCS_ONLY_FLAG_ALL ),
    E_MCU_PARAM_CLKMUX_FLAGS
  );

#endif	/* __MCU_EXTENDED_STATUS__ */

  /* Suspend IRQs */
  flags = EE_hal_suspendIRQ();

#ifdef	__MCU_EXTENDED_STATUS__

  EE_ATXMEGA_VALIDATE_IRQ_W_RV(
    ( EE_Mcu_Status == EE_MCU_STAT_INIT ),
    E_NOT_OK,
    flags
  );

#endif	/* __MCU_EXTENDED_STATUS__ */

  if ( SrcsFlags & EE_MCU_CLK_SRCS_FLAG_XOSC ) {

    OSC.XOSCCTRL = (EE_UREG)(
      (SrcsFlags & EE_MCU_CLK_SRCS_FLAG_XOSC_MASK) >>
      EE_MCU_CLK_SRCS_FLAG_XOSC_SH_BITS
    );

  }

  if ( SrcsFlags & EE_MCU_CLK_SRCS_FLAG_RTC_ENABLE ) {

    CLK.RTCCTRL = (EE_UREG)(
      (SrcsFlags & EE_MCU_CLK_SRCS_FLAG_RTC_MASK) >>
      EE_MCU_CLK_SRCS_FLAG_RTC_SH_BITS
    );

  }

  if ( SrcsFlags & EE_MCU_CLK_SRCS_FLAG_DFLL_ENABLE ) {

    OSC.DFLLCTRL = (EE_UREG)(
      (SrcsFlags & EE_MCU_CLK_SRCS_FLAG_DFLL_MASK) >>
      EE_MCU_CLK_SRCS_FLAG_DFLL_SH_BITS
    );

    if ( SrcsFlags & EE_MCU_CLK_SRCS_FLAG_RC2MHZ ) {

      DFLLRC2M.CTRL = EE_ON;

    }

    if ( SrcsFlags & EE_MCU_CLK_SRCS_FLAG_RC32MHZ ) {

      DFLLRC32M.CTRL = EE_ON;

    }

  }

  if ( SrcsFlags & EE_MCU_CLK_SRCS_FLAG_PLL ) {

    OSC.PLLCTRL = (EE_UREG)(
      (SrcsFlags & EE_MCU_CLK_SRCS_FLAG_PLL_MASK) >>
      EE_MCU_CLK_SRCS_FLAG_PLL_SH_BITS
    );

  }

  OSC.CTRL |= (EE_UREG)(SrcsFlags & EE_MCU_CLK_SRCS_FLAG_MASK);

  /* Resume IRQs */
  EE_hal_resumeIRQ(flags);

#ifdef	__MCU_EXTENDED_STATUS__
  return E_OK;
#endif

}	/* EE_Mcu_InitClock() */
#endif	/* __MCU_INITCLOCK_API__ */

#ifdef	__MCU_DEINITCLOCK_API__
/*
 * EE_Mcu_DeInitClock() Implementation.
 */
#ifdef	__MCU_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_Mcu_DeInitClock(
  EE_TYPEMCUCLKSRCSFLAGS	SrcsFlags
) {

#ifdef	DEBUG
  volatile EE_UREG	flags;
#else
  register EE_UREG	flags;
#endif

#ifdef	__MCU_EXTENDED_STATUS__

  /* SrcsFlags Validation. */
  EE_ATXMEGA_VALIDATE_W_RV(
    ( SrcsFlags & EE_MCU_CLK_SRCS_ONLY_FLAG_ALL ),
    E_MCU_PARAM_CLKMUX_FLAGS
  );

#endif	/* __MCU_EXTENDED_STATUS__ */

  /* Suspend IRQs */
  flags = EE_hal_suspendIRQ();

#ifdef	__MCU_EXTENDED_STATUS__

  EE_ATXMEGA_VALIDATE_IRQ_W_RV(
    ( EE_Mcu_Status == EE_MCU_STAT_INIT ),
    E_NOT_OK,
    flags
  );

#endif	/* __MCU_EXTENDED_STATUS__ */

  OSC.CTRL &= ~(EE_UREG)(SrcsFlags & EE_MCU_CLK_SRCS_FLAG_MASK);

  if ( SrcsFlags & EE_MCU_CLK_SRCS_FLAG_PLL ) {

    OSC.PLLCTRL = EE_OFF;

  }

  if ( SrcsFlags & EE_MCU_CLK_SRCS_FLAG_RTC_ENABLE ) {

    CLK.RTCCTRL = EE_OFF;

  }

  if ( SrcsFlags & EE_MCU_CLK_SRCS_FLAG_XOSC ) {

    OSC.XOSCCTRL = EE_OFF;

  }

  if ( SrcsFlags & EE_MCU_CLK_SRCS_FLAG_RC2MHZ ) {

    DFLLRC2M.CTRL = EE_OFF;

  }

  if ( SrcsFlags & EE_MCU_CLK_SRCS_FLAG_RC32MHZ ) {

    DFLLRC32M.CTRL = EE_OFF;

  }

  if ( 
    !(
      OSC.CTRL & ( EE_MCU_CLK_SRCS_FLAG_RC2MHZ | EE_MCU_CLK_SRCS_FLAG_RC32MHZ )
    ) 
  ) {

    OSC.DFLLCTRL = EE_OFF;

  }

  /* Resume IRQs */
  EE_hal_resumeIRQ(flags);

#ifdef	__MCU_EXTENDED_STATUS__
  return E_OK;
#endif

}	/* EE_Mcu_DeInitClock() */
#endif	/* __MCU_DEINITCLOCK_API__ */

#ifdef __MCU_GETCLOCKSTATUS_API__
#ifdef	__MCU_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_Mcu_GetClockStatus(
  EE_TYPEMCUCLKSRCSFLAGS	SrcsFlags,
  EE_TYPEMCUCLKSTATUS		*Status
) {

#ifdef	DEBUG
  volatile EE_UREG	flags;
#else
  register EE_UREG	flags;
#endif

#ifdef	__MCU_EXTENDED_STATUS__

  /* SrcsFlags Validation. */
  EE_ATXMEGA_VALIDATE_W_RV(
    ( 
      ( SrcsFlags & EE_MCU_CLK_SRCS_ONLY_FLAG_ALL ) &&
      !( SrcsFlags & ~EE_MCU_CLK_SRCS_ONLY_FLAG_ALL )
    ),
    E_MCU_PARAM_CLKMUX_FLAGS
  );

  /* Status Validation. */
  EE_ATXMEGA_VALIDATE_W_RV( ( Status != EE_NULL_PTR ), E_MCU_PARAM_NULL );

#endif	/* __MCU_EXTENDED_STATUS__ */

  /* Suspend IRQs */
  flags = EE_hal_suspendIRQ();

  (*Status) = EE_MCU_CLK_STATUS_UNDEFINED;

#ifdef	__MCU_EXTENDED_STATUS__

  EE_ATXMEGA_VALIDATE_IRQ_W_RV(
    ( EE_Mcu_Status == EE_MCU_STAT_INIT ),
    E_NOT_OK,
    flags
  );

#endif	/* __MCU_EXTENDED_STATUS__ */

  if ( ( OSC.STATUS & (EE_UREG)(SrcsFlags) ) == (EE_UREG)(SrcsFlags) ) {

    (*Status) = EE_MCU_CLK_STABLE;

  }
  else {

    (*Status) = EE_MCU_CLK_UNSTABLE;

  }

  /* Resume IRQs */
  EE_hal_resumeIRQ(flags);

#ifdef	__MCU_EXTENDED_STATUS__
  return E_OK;
#endif

}	/* EE_Mcu_GetClockStatus() */
#endif	/* __MCU_GETCLOCKSTATUS_API__ */

#ifdef __MCU_DISTRIBUTECLOCK_API__
#ifdef	__MCU_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_Mcu_DistributeClock(
  EE_TYPEMCUCLKMUXFLAGS	MuxFlags,
  EE_TYPEMCUCLKDIVFLAGS	DivFlags
) {

#ifdef	DEBUG
  volatile EE_UREG	flags;
#else
  register EE_UREG	flags;
#endif

#ifdef	__MCU_EXTENDED_STATUS__

  /* MuxFlags Validation. */
  EE_ATXMEGA_VALIDATE_W_RV(
    ( MuxFlags <= EE_MCU_CLK_MUX_FLAG_PLL ),
    E_MCU_PARAM_CLKMUX_FLAGS
  );

  /* DivFlags Validation. */
  EE_ATXMEGA_VALIDATE_W_RV(
    ( 
      ( DivFlags < EE_MCU_CLK_DIV_FLAG_A_2 ) ||
      ( 
	( DivFlags < EE_MCU_CLK_DIV_FLAG_A_512 | EE_MCU_CLK_DIV_FLAG_B_2_C_2 )
        && ( DivFlags & EE_MCU_CLK_DIV_FLAG_A_2 )
      )
    ),
    E_MCU_PARAM_CLKDIV_FLAGS
  );

#endif	/* __MCU_EXTENDED_STATUS__ */

  /* Suspend IRQs */
  flags = EE_hal_suspendIRQ();

#ifdef	__MCU_EXTENDED_STATUS__

  EE_ATXMEGA_VALIDATE_IRQ_W_RV(
    ( EE_Mcu_Status == EE_MCU_STAT_INIT ),
    E_NOT_OK,
    flags
  );

#endif	/* __MCU_EXTENDED_STATUS__ */

  EE_avr8_WriteCCPReg(&CLK.CTRL, MuxFlags);

  if ( CLK.CTRL == MuxFlags ) {

    EE_avr8_WriteCCPReg(&CLK.PSCTRL, DivFlags);

  }

  /* Resume IRQs */
  EE_hal_resumeIRQ(flags);

#ifdef	__MCU_EXTENDED_STATUS__
  return E_OK;
#endif

}	/* EE_Mcu_DistributeClock() */
#endif	/* __MCU_DISTRIBUTECLOCK_API__ */

#ifdef	__MCU_SETMODE_API__
#ifdef	__MCU_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_Mcu_SetMode(
  EE_TYPEMCUMODEFLAGS ModeFlags
) {

#ifndef	__MCU_SETMODE_NTS__
#ifdef	DEBUG
  volatile EE_UREG	flags;
#else
  register EE_UREG	flags;
#endif
#endif	/* __MCU_SETMODE_NTS__ */

#ifdef	__MCU_EXTENDED_STATUS__

  /* ModeFlags Validation. */
  EE_ATXMEGA_VALIDATE_W_RV(
    ( ModeFlags <= EE_MCU_CLK_MUX_FLAG_PLL ),
    E_MCU_PARAM_MODE_FLAGS
  );

#endif	/* __MCU_EXTENDED_STATUS__ */

#ifndef	__MCU_SETMODE_NTS__
  /* Suspend IRQs */
  flags = EE_hal_suspendIRQ();
#endif	/* __MCU_SETMODE_NTS__ */

#ifdef	__MCU_EXTENDED_STATUS__

#ifndef	__MCU_SETMODE_NTS__
  EE_ATXMEGA_VALIDATE_IRQ_W_RV(
    ( EE_Mcu_Status == EE_MCU_STAT_INIT ),
    E_NOT_OK,
    flags
  );
#else	/* __MCU_SETMODE_NTS__ */
  EE_ATXMEGA_VALIDATE_W_RV(
    ( EE_Mcu_Status == EE_MCU_STAT_INIT ),
    E_NOT_OK
  );
#endif	/* __MCU_SETMODE_NTS__ */

#endif	/* __MCU_EXTENDED_STATUS__ */

  SLEEP.CTRL = ModeFlags;

#ifndef	DEBUG
  EE_Pr_DisableJTAG();
#endif

#ifndef	__MCU_SETMODE_NTS__
  /* Resume IRQs */
  EE_hal_resumeIRQ(flags);
#endif	/* __MCU_SETMODE_NTS__ */

  sleep_cpu();

#ifndef	__MCU_SETMODE_NTS__
  /* Suspend IRQs */
  flags = EE_hal_suspendIRQ();
#endif	/* __MCU_SETMODE_NTS__ */

#ifndef	DEBUG
  EE_Pr_EnableJTAG();
#endif

  SLEEP.CTRL = EE_MCU_MODE_FLAG_DEFAULT;

#ifndef	__MCU_SETMODE_NTS__
  /* Resume IRQs */
  EE_hal_resumeIRQ(flags);
#endif	/* __MCU_SETMODE_NTS__ */

#ifdef	__MCU_EXTENDED_STATUS__
  return E_OK;
#endif

}
#endif	/* __MCU_SETMODE_API__ */

#ifdef	__MCU_GETRESETREASON_API__
#ifdef	__MCU_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_Mcu_GetResetReason(
  EE_TYPEMCUMODEFLAGS	*ResetFlags
) {

#ifdef	DEBUG
  volatile EE_UREG	flags;
#else
  register EE_UREG	flags;
#endif

#ifdef	__MCU_EXTENDED_STATUS__

  /* Status Validation. */
  EE_ATXMEGA_VALIDATE_W_RV( ( ResetFlags != EE_NULL_PTR ), E_MCU_PARAM_NULL );

#endif	/* __MCU_EXTENDED_STATUS__ */

  /* Suspend IRQs */
  flags = EE_hal_suspendIRQ();

#ifdef	__MCU_EXTENDED_STATUS__

  EE_ATXMEGA_VALIDATE_IRQ_W_RV(
    ( EE_Mcu_Status == EE_MCU_STAT_INIT ),
    E_NOT_OK,
    flags
  );

#endif	/* __MCU_EXTENDED_STATUS__ */

  (*ResetFlags) = RST.STATUS;
  RST.STATUS = EE_MCU_RESET_FLAG_ALL;

  /* Resume IRQs */
  EE_hal_resumeIRQ(flags);

#ifdef	__MCU_EXTENDED_STATUS__
  return E_OK;
#endif

}
#endif	/* __MCU_GETRESETREASON_API__ */

#ifdef	__MCU_PERFORMRESET_API__
#ifdef	__MCU_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_Mcu_PerformReset(
  void
) {

#ifdef	DEBUG
  volatile EE_UREG	flags;
#else
  register EE_UREG	flags;
#endif

  /* Suspend IRQs */
  flags = EE_hal_suspendIRQ();

#ifdef	__MCU_EXTENDED_STATUS__

  EE_ATXMEGA_VALIDATE_IRQ_W_RV(
    ( EE_Mcu_Status == EE_MCU_STAT_INIT ),
    E_NOT_OK,
    flags
  );

#endif	/* __MCU_EXTENDED_STATUS__ */

  EE_avr8_WriteCCPReg(&RST.CTRL, EE_ON);

  /* Infinite Loop Waiting Reset */
  while ( TRUE );

}
#endif	/* __MCU_PERFORMRESET_API__ */
