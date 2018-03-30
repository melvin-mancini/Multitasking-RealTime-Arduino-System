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
 * Erika Enterprise - RTC Driver - Atmel ATXMEGA MCUs Implementation File.
 *
 * Author: 2013,  Giuseppe Serano
 */

/* ERIKA Enterprise Internals */
#include "ee_internal.h"

/* ERIKA Enterprise IRQs */
#include "ee_irq.h"

#ifdef	__RTC_CHANNEL_0__
#define	EE_RTC_MAX_CHANNELS_NUMBER	EE_RTC_CHANNEL_0 + 1
#else
#define	EE_RTC_MAX_CHANNELS_NUMBER	0x00U
#endif

#ifdef	__RTC_EXTENDED_STATUS__

#define	EE_RTC_INIT_MASK	0x80U

/*
 * RTC Status.
 */
#ifdef	DEBUG
static volatile EE_UREG
#else
static EE_UREG
#endif
#if	0
EE_Rtc_Status = EE_OFF
#else	/* 0 */
EE_Rtc_Status;
#endif	/* 0 */

#endif	/* __RTC_EXTENDED_STATUS__ */

#if	( \
  defined(__RTC_ENABLENOTIF_API__)	&& \
  defined(__RTC_CHANNEL_0__)		&& \
  defined(EE_AVR8_RTC_OVF_ISR)		   \
)

/*
 * RTC Hardware Units Callback.
 */
#ifdef	DEBUG
static volatile EE_ISR_callback
#else
static EE_ISR_callback
#endif
#if	0
EE_Rtc_CallBack = EE_NULL_PTR;
#else	/* 0 */
EE_Rtc_CallBack;
#endif	/* 0 */

#endif	/* 
	 * __RTC_ENABLENOTIF_API__	&&
	 * __RTC_CHANNEL_0__		&&
	 * EE_AVR8_RTC_OVF_ISR
	 */

#if	( \
  defined(__RTC_ENABLENOTIF_API__)	&& \
  defined(__RTC_CHANNEL_0__)		&& \
  defined(EE_AVR8_RTC_OVF_ISR)		   \
)

/* RTC ISR
 *
 * param[in]	Channel		RTC Channel Identifier.
 */
#if	( EE_AVR8_RTC_OVF_ISR_CAT == 1 )
ISR1(EE_AVR8_RTC_OVF_ISR)
#else
ISR2(EE_AVR8_RTC_OVF_ISR)
#endif
{

  /* Call RTC CallBack. */
  EE_Rtc_CallBack();

}	/* ISR2(EE_AVR8_RTC_OVF_ISR) */

#endif	/* 
	 * __RTC_ENABLENOTIF_API__	&&
	 * __RTC_CHANNEL_0__		&&
	 * EE_AVR8_RTC_OVF_ISR
	 */

#ifdef	__RTC_GETSTATUS_API__
/*
 * EE_Rtc_GetStatus() Implementation.
 */
EE_TYPERTCCHSTAT
EE_Rtc_GetStatus(
  EE_TYPERTCCHANNEL	Channel
) {

#ifdef	DEBUG
  volatile EE_UREG	flags;
#else
  register EE_UREG	flags;
#endif

#ifdef	__RTC_EXTENDED_STATUS__

  /* Channel Validation */
  EE_ATXMEGA_VALIDATE_W_RV( 
    ( Channel < EE_RTC_MAX_CHANNELS_NUMBER ),
    EE_RTC_CH_UNINIT
  );

#endif	/* __RTC_EXTENDED_STATUS__ */

  /* Suspend Interrupt */
  flags = EE_hal_suspendIRQ();

#ifdef	__RTC_EXTENDED_STATUS__

  /* Channel Initialization Validation */
  EE_ATXMEGA_VALIDATE_IRQ_W_RV(
    ( EE_Rtc_Status & EE_RTC_INIT_MASK ),
    EE_RTC_CH_UNINIT,
    flags
  );

#endif	/* __RTC_EXTENDED_STATUS__ */

  addr = EE_RTC_HW_TCX0_2_ADDR(Channel);

  /* Channel Running Validation */
  EE_ATXMEGA_VALIDATE_IRQ_W_RV(
    ( RTC.CTRL == RTC_PRESCALER_OFF_gc ),
    EE_RTC_CH_RUNNING,
    flags
  );

  /* Channel Stopped Validation */
  EE_ATXMEGA_VALIDATE_IRQ_W_RV(
    ( EE_ATxmega_Reg16Read(&RTC.PER) == EE_OFF ),
    EE_RTC_CH_STOPPED,
    flags
  );

  /* Resume Interrupt */
  EE_hal_resumeIRQ(flags);

  return EE_RTC_CH_OPERATIONAL;

};
#endif	/* __RTC_GETSTATUS_API__ */

/** @brief	This macro checks the RTC busy flag.
 *
 *  @return	Non-Zero if RTC is busy, zero otherwise.
 */
#define	RTC_Busy()	( RTC.STATUS & RTC_SYNCBUSY_bm )

/*
 * EE_Rtc_Init() Implementation.
 */
#ifdef	__RTC_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_Rtc_Init(
  EE_TYPERTCCHANNEL	Channel,
  EE_TYPERTCFLAGS	Flags,
  EE_ISR_callback	NotifCB
) {

#ifdef	DEBUG
  volatile EE_UREG	flags;
#else
  register EE_UREG	flags;
#endif

#ifdef	__RTC_EXTENDED_STATUS__

  /* Channel Validation */
  EE_ATXMEGA_VALIDATE_W_RV( 
    ( Channel < EE_RTC_MAX_CHANNELS_NUMBER ),
    E_RTC_PARAM_CHANNEL
  );

  /* Flags Validation. */
  EE_ATXMEGA_VALIDATE_W_RV(
    ( Flags == EE_RTC_FLAG_DEFAULT ),
    E_RTC_PARAM_FLAGS
  );

#endif	/* __RTC_EXTENDED_STATUS__ */

  /* Suspend Interrupt */
  flags = EE_hal_suspendIRQ();

#ifdef	__RTC_EXTENDED_STATUS__

  EE_ATXMEGA_VALIDATE_IRQ_W_RV(
    !( EE_Rtc_Status & EE_RTC_INIT_MASK ),
    E_NOT_OK,
    flags
  );

#endif	/* __RTC_EXTENDED_STATUS__ */

	do {
		/* Wait until RTC is not busy. */
	} while ( RTC_Busy() );

  EE_ATxmega_Reg16Write( &RTC.PER, EE_OFF );

#if	( \
  defined(__RTC_ENABLENOTIF_API__)	&& \
  defined(__RTC_CHANNEL_0__)		&& \
  defined(EE_AVR8_RTC_OVF_ISR)		   \
)

  EE_Rtc_CallBack = NotifCB;

	do {
		/* Wait until RTC is not busy. */
	} while ( RTC_Busy() );

  RTC.INTCTRL = EE_ISR_PRI_OFF;

	do {
		/* Wait until RTC is not busy. */
	} while ( RTC_Busy() );

  RTC.INTFLAGS = ( RTC_COMPIF_bm | RTC_OVFIF_bm );

#endif	/* 
	 * __RTC_ENABLENOTIF_API__	&&
	 * __RTC_CHANNEL_0__		&&
	 * EE_AVR8_RTC_OVF_ISR
	 */

#ifdef	__RTC_EXTENDED_STATUS__

  EE_Rtc_Status |= EE_RTC_INIT_MASK;

#endif	/* __RTC_EXTENDED_STATUS__ */

  /* Resume IRQs */
  EE_hal_resumeIRQ(flags);

#ifdef	__RTC_EXTENDED_STATUS__
  return E_OK;
#endif	/* __RTC_EXTENDED_STATUS__ */

}

#ifdef	__RTC_DEINIT_API__
/*
 * EE_Rtc_DeInit() Implementation.
 */
#ifdef	__RTC_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_Rtc_DeInit(
  EE_TYPERTCCHANNEL	Channel,
  EE_TYPERTCFLAGS	Flags
) {

#ifdef	DEBUG
  volatile EE_UREG	flags;
#else
  register EE_UREG	flags;
#endif

#ifdef	__RTC_EXTENDED_STATUS__

  /* Channel Validation */
  EE_ATXMEGA_VALIDATE_W_RV( 
    ( Channel < EE_RTC_MAX_CHANNELS_NUMBER ),
    E_RTC_PARAM_CHANNEL
  );

  /* Flags Validation. */
  EE_ATXMEGA_VALIDATE_W_RV(
    ( Flags == EE_RTC_FLAG_DEFAULT ),
    E_RTC_PARAM_FLAGS
  );

#endif	/* __RTC_EXTENDED_STATUS__ */

  /* Suspend Interrupt */
  flags = EE_hal_suspendIRQ();

#ifdef	__RTC_EXTENDED_STATUS__

  EE_ATXMEGA_VALIDATE_IRQ_W_RV(
    ( EE_Rtc_Status & EE_RTC_INIT_MASK ),
    E_NOT_OK,
    flags
  );

  EE_ATXMEGA_VALIDATE_IRQ_W_RV(
    ( RTC.CTRL == RTC_PRESCALER_OFF_gc ),
    E_RTC_CHANNEL_BUSY,
    flags
  );

#endif	/* __RTC_EXTENDED_STATUS__ */

	do {
		/* Wait until RTC is not busy. */
	} while ( RTC_Busy() );

  EE_ATxmega_Reg16Write( &RTC.PER, 0xFFFFU );

#if	( \
  defined(__RTC_ENABLENOTIF_API__)	&& \
  defined(__RTC_CHANNEL_0__)		&& \
  defined(EE_AVR8_RTC_OVF_ISR)		   \
)

  EE_Rtc_CallBack = EE_NULL_PTR;

	do {
		/* Wait until RTC is not busy. */
	} while ( RTC_Busy() );

  RTC.INTFLAGS = ( RTC_COMPIF_bm | RTC_OVFIF_bm );

	do {
		/* Wait until RTC is not busy. */
	} while ( RTC_Busy() );

  RTC.INTCTRL = EE_ISR_PRI_OFF;

#endif	/* 
	 * __RTC_ENABLENOTIF_API__	&&
	 * __RTC_CHANNEL_0__		&&
	 * EE_AVR8_RTC_OVF_ISR
	 */

#ifdef	__RTC_EXTENDED_STATUS__

  EE_Rtc_Status &= ~EE_RTC_INIT_MASK;

#endif	/* __RTC_EXTENDED_STATUS__ */

  /* Resume IRQs */
  EE_hal_resumeIRQ(flags);

#ifdef	__RTC_EXTENDED_STATUS__
  return E_OK;
#endif	/* __RTC_EXTENDED_STATUS__ */

}
#endif	/* __RTC_DEINIT_API__ */

#ifdef	__RTC_ENABLENOTIF_API__
/*
 * EE_Rtc_EnableNotification() Implementation.
 */
#ifdef	__RTC_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_Rtc_EnableNotification(
  EE_TYPERTCCHANNEL	Channel
) {

#ifdef	DEBUG
  volatile EE_UREG	flags;
#else
  register EE_UREG	flags;
#endif

#ifdef	__RTC_EXTENDED_STATUS__

  /* Channel Validation */
  EE_ATXMEGA_VALIDATE_W_RV( 
    ( Channel < EE_RTC_MAX_CHANNELS_NUMBER ),
    E_RTC_PARAM_CHANNEL
  );

#endif	/* __RTC_EXTENDED_STATUS__ */

  /* Suspend Interrupt */
  flags = EE_hal_suspendIRQ();

#ifdef	__RTC_EXTENDED_STATUS__

  EE_ATXMEGA_VALIDATE_IRQ_W_RV(
    ( EE_Rtc_Status & EE_RTC_INIT_MASK ),
    E_NOT_OK,
    flags
  );

#if	( \
  defined(__RTC_ENABLENOTIF_API__)	&& \
  defined(__RTC_CHANNEL_0__)		&& \
  defined(EE_AVR8_RTC_OVF_ISR)		   \
)

  /* Call-Back Validation. */
  EE_ATXMEGA_VALIDATE_W_RV(
    ( EE_Rtc_CallBack != EE_NULL_PTR ),
    E_RTC_PARAM_CALLBACK
  );

#endif	/* 
	 * __RTC_ENABLENOTIF_API__	&&
	 * __RTC_CHANNEL_0__		&&
	 * EE_AVR8_RTC_OVF_ISR
	 */

#endif	/* __RTC_EXTENDED_STATUS__ */

#if	( \
  defined(__RTC_ENABLENOTIF_API__)	&& \
  defined(__RTC_CHANNEL_0__)		&& \
  defined(EE_AVR8_RTC_OVF_ISR)		   \
)

	do {
		/* Wait until RTC is not busy. */
	} while ( RTC_Busy() );

  RTC.INTFLAGS = ( RTC_COMPIF_bm | RTC_OVFIF_bm );

	do {
		/* Wait until RTC is not busy. */
	} while ( RTC_Busy() );

#ifdef	EE_AVR8_RTC_OVF_ISR_PRI
  RTC.INTCTRL = EE_AVR8_RTC_OVF_ISR_PRI;
#else
  RTC.INTCTRL = EE_ISR_PRI_OFF;
#endif

#endif	/* 
	 * __RTC_ENABLENOTIF_API__	&&
	 * __RTC_CHANNEL_0__		&&
	 * EE_AVR8_RTC_OVF_ISR
	 */

  /* Resume IRQs */
  EE_hal_resumeIRQ(flags);

#ifdef	__RTC_EXTENDED_STATUS__
  return E_OK;
#endif	/* __RTC_EXTENDED_STATUS__ */

}

#ifdef	__RTC_DISABLENOTIF_API__
/*
 * EE_Rtc_DisableNotification() Implementation.
 */
#ifdef	__RTC_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_Rtc_DisableNotification(
  EE_TYPERTCCHANNEL	Channel
) {

#ifdef	DEBUG
  volatile EE_UREG	flags;
#else
  register EE_UREG	flags;
#endif

#ifdef	__RTC_EXTENDED_STATUS__

  /* Channel Validation */
  EE_ATXMEGA_VALIDATE_W_RV( 
    ( Channel < EE_RTC_MAX_CHANNELS_NUMBER ),
    E_RTC_PARAM_CHANNEL
  );

#endif	/* __RTC_EXTENDED_STATUS__ */

  /* Suspend Interrupt */
  flags = EE_hal_suspendIRQ();

#ifdef	__RTC_EXTENDED_STATUS__

  EE_ATXMEGA_VALIDATE_IRQ_W_RV(
    ( EE_Rtc_Status & EE_RTC_INIT_MASK ),
    E_NOT_OK,
    flags
  );

#if	( \
  defined(__RTC_ENABLENOTIF_API__)	&& \
  defined(__RTC_CHANNEL_0__)		&& \
  defined(EE_AVR8_RTC_OVF_ISR)		   \
)

  /* Call-Back Validation. */
  EE_ATXMEGA_VALIDATE_W_RV(
    ( EE_Rtc_CallBack != EE_NULL_PTR ),
    E_RTC_PARAM_CALLBACK
  );

#endif	/* 
	 * __RTC_ENABLENOTIF_API__	&&
	 * __RTC_CHANNEL_0__		&&
	 * EE_AVR8_RTC_OVF_ISR
	 */

#endif	/* __RTC_EXTENDED_STATUS__ */

#if	( \
  defined(__RTC_ENABLENOTIF_API__)	&& \
  defined(__RTC_CHANNEL_0__)		&& \
  defined(EE_AVR8_RTC_OVF_ISR)		   \
)

	do {
		/* Wait until RTC is not busy. */
	} while ( RTC_Busy() );

  RTC.INTFLAGS = ( RTC_COMPIF_bm | RTC_OVFIF_bm );

	do {
		/* Wait until RTC is not busy. */
	} while ( RTC_Busy() );

  RTC.INTCTRL = EE_ISR_PRI_OFF;

#endif	/* 
	 * __RTC_ENABLENOTIF_API__	&&
	 * __RTC_CHANNEL_0__		&&
	 * EE_AVR8_RTC_OVF_ISR
	 */

  /* Resume IRQs */
  EE_hal_resumeIRQ(flags);

#ifdef	__RTC_EXTENDED_STATUS__
  return E_OK;
#endif	/* __RTC_EXTENDED_STATUS__ */

}
#endif	/* __RTC_DISABLENOTIF_API__ */
#endif	/* __RTC_ENABLENOTIF_API__ */

#ifdef	__RTC_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_Rtc_Start(
  EE_TYPERTCCHANNEL	Channel,
  EE_TYPERTCCOUNTER	Period
) {

#ifdef	DEBUG
  volatile EE_UREG	flags;
#else
  register EE_UREG	flags;
#endif

#ifdef	__RTC_EXTENDED_STATUS__

  /* Channel Validation */
  EE_ATXMEGA_VALIDATE_W_RV( 
    ( Channel < EE_RTC_MAX_CHANNELS_NUMBER ),
    E_RTC_PARAM_CHANNEL
  );

#if	0
  /* Period Validation */
  EE_ATXMEGA_VALIDATE_W_RV( 
    ( Period != EE_OFF ),
    E_RTC_PARAM_COUNTER
  );
#endif

#endif	/* __RTC_EXTENDED_STATUS__ */

  /* Suspend Interrupt */
  flags = EE_hal_suspendIRQ();

#ifdef	__RTC_EXTENDED_STATUS__

  EE_ATXMEGA_VALIDATE_IRQ_W_RV(
    ( EE_Rtc_Status & EE_RTC_INIT_MASK ),
    E_NOT_OK,
    flags
  );

  EE_ATXMEGA_VALIDATE_IRQ_W_RV(
    ( RTC.CTRL == RTC_PRESCALER_OFF_gc ),
    E_RTC_CHANNEL_BUSY,
    flags
  );

#endif	/* __RTC_EXTENDED_STATUS__ */

	do {
		/* Wait until RTC is not busy. */
	} while ( RTC_Busy() );

  EE_ATxmega_Reg16Write( &RTC.CNT, EE_OFF );

	do {
		/* Wait until RTC is not busy. */
	} while ( RTC_Busy() );

  EE_ATxmega_Reg16Write( &RTC.PER, Period );

	do {
		/* Wait until RTC is not busy. */
	} while ( RTC_Busy() );

#ifdef	__RTC_CHANNEL_0__
#if	defined(__RTC_CHANNEL_0_DIV_2__)
  RTC.CTRL = RTC_PRESCALER_DIV2_gc;
#elif	defined(__RTC_CHANNEL_0_DIV_8__)
  RTC.CTRL = RTC_PRESCALER_DIV8_gc;
#elif	defined(__RTC_CHANNEL_0_DIV_16__)
  RTC.CTRL = RTC_PRESCALER_DIV16_gc;
#elif	defined(__RTC_CHANNEL_0_DIV_64__)
  RTC.CTRL = RTC_PRESCALER_DIV64_gc;
#elif	defined(__RTC_CHANNEL_0_DIV_256__)
  RTC.CTRL = RTC_PRESCALER_DIV256_gc;
#elif	defined(__RTC_CHANNEL_0_DIV_1024__)
  RTC.CTRL = RTC_PRESCALER_DIV1024_gc;
#else
  RTC.CTRL = RTC_PRESCALER_DIV1_gc;
#endif
#else	/* __RTC_CHANNEL_0__ */
  RTC.CTRL = RTC_PRESCALER_OFF_gc;
#endif	/* __RTC_CHANNEL_0__ */

  /* Resume IRQs */
  EE_hal_resumeIRQ(flags);

#ifdef	__RTC_EXTENDED_STATUS__
  return E_OK;
#endif	/* __RTC_EXTENDED_STATUS__ */

}

#ifdef	__RTC_STOP_API__
#ifdef	__RTC_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_Rtc_Stop(
  EE_TYPERTCCHANNEL	Channel
) {

  #ifdef	DEBUG
  volatile EE_UREG	flags;
#else
  register EE_UREG	flags;
#endif

#ifdef	__RTC_EXTENDED_STATUS__

  /* Channel Validation */
  EE_ATXMEGA_VALIDATE_W_RV( 
    ( Channel < EE_RTC_MAX_CHANNELS_NUMBER ),
    E_RTC_PARAM_CHANNEL
  );

#endif	/* __RTC_EXTENDED_STATUS__ */

  /* Suspend Interrupt */
  flags = EE_hal_suspendIRQ();

#ifdef	__RTC_EXTENDED_STATUS__

  EE_ATXMEGA_VALIDATE_IRQ_W_RV(
    ( EE_Rtc_Status & EE_RTC_INIT_MASK ),
    E_NOT_OK,
    flags
  );

#endif	/* __RTC_EXTENDED_STATUS__ */

	do {
		/* Wait until RTC is not busy. */
	} while ( RTC_Busy() );

  RTC.CTRL = TC_CLKSEL_OFF_gc;

  /* Resume IRQs */
  EE_hal_resumeIRQ(flags);

#ifdef	__RTC_EXTENDED_STATUS__
  return E_OK;
#endif	/* __RTC_EXTENDED_STATUS__ */

}
#endif	/* __RTC_STOP_API__ */

#ifdef	__RTC_GETCOUNTER_API__
#ifdef	__RTC_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_Rtc_GetCounter(
  EE_TYPERTCCHANNEL	Channel,
  EE_TYPERTCCOUNTER	*Counter
) {

#ifdef	DEBUG
  volatile EE_UREG	flags;
#else
  register EE_UREG	flags;
#endif

#ifdef	__RTC_EXTENDED_STATUS__

  /* Counter Validation */
  EE_ATXMEGA_VALIDATE_W_RV( 
    ( Counter != EE_NULL_PTR ),
    E_RTC_PARAM_COUNTER
  );

  (*Counter) = EE_OFF;

  /* Channel Validation */
  EE_ATXMEGA_VALIDATE_W_RV( 
    ( Channel < EE_RTC_MAX_CHANNELS_NUMBER ),
    E_RTC_PARAM_CHANNEL
  );

#endif	/* __RTC_EXTENDED_STATUS__ */

  /* Suspend Interrupt */
  flags = EE_hal_suspendIRQ();

#ifdef	__RTC_EXTENDED_STATUS__

  EE_ATXMEGA_VALIDATE_IRQ_W_RV(
    ( EE_Rtc_Status & EE_RTC_INIT_MASK ),
    E_NOT_OK,
    flags
  );

#endif	/* __RTC_EXTENDED_STATUS__ */

  (*Counter) = EE_ATxmega_Reg16Read( &RTC.CNT );

  /* Resume IRQs */
  EE_hal_resumeIRQ(flags);

#ifdef	__RTC_EXTENDED_STATUS__
  return E_OK;
#endif	/* __RTC_EXTENDED_STATUS__ */

}
#endif	/* __RTC_GETCOUNTER_API__ */

#ifdef	__RTC_SETCOUNTER_API__
#ifdef	__RTC_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_Rtc_SetCounter(
  EE_TYPERTCCHANNEL	Channel,
  EE_TYPERTCCOUNTER	Counter
) {

#ifdef	DEBUG
  volatile EE_UREG	flags;
#else
  register EE_UREG	flags;
#endif

#ifdef	__RTC_EXTENDED_STATUS__

  /* Channel Validation */
  EE_ATXMEGA_VALIDATE_W_RV( 
    ( Channel < EE_RTC_MAX_CHANNELS_NUMBER ),
    E_RTC_PARAM_CHANNEL
  );

#endif	/* __RTC_EXTENDED_STATUS__ */

  /* Suspend Interrupt */
  flags = EE_hal_suspendIRQ();

#ifdef	__RTC_EXTENDED_STATUS__

  EE_ATXMEGA_VALIDATE_IRQ_W_RV(
    ( EE_Rtc_Status & EE_TC_HW_INIT_MASK ),
    E_NOT_OK,
    flags
  );

#endif	/* __RTC_EXTENDED_STATUS__ */

	do {
		/* Wait until RTC is not busy. */
	} while ( RTC_Busy() );

  EE_ATxmega_Reg16Write( &RTC.CNT, Counter );

  /* Resume IRQs */
  EE_hal_resumeIRQ(flags);

#ifdef	__RTC_EXTENDED_STATUS__
  return E_OK;
#endif	/* __RTC_EXTENDED_STATUS__ */

}
#endif	/* __RTC_SETCOUNTER_API__ */
