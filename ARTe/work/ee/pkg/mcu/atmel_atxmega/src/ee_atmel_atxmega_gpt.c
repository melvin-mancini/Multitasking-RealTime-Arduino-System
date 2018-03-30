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
 * Erika Enterprise - GPT Driver - Atmel ATXMEGA MCUs Implementation File.
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

#ifdef	__GPT_ONESHOT__
#define	EE_GPT_FLAG_ALL_MASK	EE_GPT_FLAG_ONESHOT

#define	EE_GPT_ONESHOT_MASK	0x40U
#define	EE_GPT_EXPIRED_MASK	0x20U
#endif	/* __GPT_ONESHOT__ */
#ifdef	__GPT_ENABLENOTIF_API__
#define	EE_GPT_NOTIF_MASK	0x10U
#endif	/* __GPT_ENABLENOTIF_API__ */

#ifdef	__USE_TC_HW__

/*
 * Timer Counter 0 Prescalers
 */
#ifdef	DEBUG
static volatile const EE_UREG
#else
static const EE_UREG
#endif
#ifdef	__AVR_PGMSPACE__
EE_Gpt_0_Div[EE_TC_0_HW_UNITS_NUMBER] PROGMEM = {
#else	/* __AVR_PGMSPACE__ */
EE_Gpt_0_Div[EE_TC_0_HW_UNITS_NUMBER] = {
#endif	/* __AVR_PGMSPACE__ */

#if	( defined(__AVR_ATxmega16D4__) || defined(__AVR_ATxmega32D4__) )

/* CHANNEL C 0 */
#ifdef	__GPT_CHANNEL_C_0__
#if	defined(__GPT_CHANNEL_C_0_DIV_2__)
TC_CLKSEL_DIV2_gc,
#elif	defined(__GPT_CHANNEL_C_0_DIV_4__)
TC_CLKSEL_DIV4_gc,
  #elif	defined(__GPT_CHANNEL_C_0_DIV_8__)
TC_CLKSEL_DIV8_gc,
#elif	defined(__GPT_CHANNEL_C_0_DIV_64__)
TC_CLKSEL_DIV64_gc,
#elif	defined(__GPT_CHANNEL_C_0_DIV_256__)
TC_CLKSEL_DIV256_gc,
#elif	defined(__GPT_CHANNEL_C_0_DIV_1024__)
TC_CLKSEL_DIV1024_gc,
#else
TC_CLKSEL_DIV1_gc,
#endif
#else	/* __GPT_CHANNEL_C_0__ */
TC_CLKSEL_OFF_gc,
#endif	/* __GPT_CHANNEL_C_0__ */

/* CHANNEL D 0 */
#ifdef	__GPT_CHANNEL_D_0__
#if	defined(__GPT_CHANNEL_D_0_DIV_2__)
TC_CLKSEL_DIV2_gc,
#elif	defined(__GPT_CHANNEL_D_0_DIV_4__)
TC_CLKSEL_DIV4_gc,
  #elif	defined(__GPT_CHANNEL_D_0_DIV_8__)
TC_CLKSEL_DIV8_gc,
#elif	defined(__GPT_CHANNEL_D_0_DIV_64__)
TC_CLKSEL_DIV64_gc,
#elif	defined(__GPT_CHANNEL_D_0_DIV_256__)
TC_CLKSEL_DIV256_gc,
#elif	defined(__GPT_CHANNEL_D_0_DIV_1024__)
TC_CLKSEL_DIV1024_gc,
#else
TC_CLKSEL_DIV1_gc,
#endif
#else	/* __GPT_CHANNEL_D_0__ */
TC_CLKSEL_OFF_gc,
#endif	/* __GPT_CHANNEL_D_0__ */

/* CHANNEL E 0 */
#ifdef	__GPT_CHANNEL_E_0__
#if	defined(__GPT_CHANNEL_E_0_DIV_2__)
TC_CLKSEL_DIV2_gc,
#elif	defined(__GPT_CHANNEL_E_0_DIV_4__)
TC_CLKSEL_DIV4_gc,
  #elif	defined(__GPT_CHANNEL_E_0_DIV_8__)
TC_CLKSEL_DIV8_gc,
#elif	defined(__GPT_CHANNEL_E_0_DIV_64__)
TC_CLKSEL_DIV64_gc,
#elif	defined(__GPT_CHANNEL_E_0_DIV_256__)
TC_CLKSEL_DIV256_gc,
#elif	defined(__GPT_CHANNEL_E_0_DIV_1024__)
TC_CLKSEL_DIV1024_gc,
#else
TC_CLKSEL_DIV1_gc,
#endif
#else	/* __GPT_CHANNEL_E_0__ */
TC_CLKSEL_OFF_gc,
#endif	/* __GPT_CHANNEL_E_0__ */

#else	/* __AVR_ATxmega16D4__ || __AVR_ATxmega32D4__ */

/* CHANNEL C 0 */
#ifdef	__GPT_CHANNEL_C_0__
#if	defined(__GPT_CHANNEL_C_0_DIV_2__)
TC_CLKSEL_DIV2_gc,
#elif	defined(__GPT_CHANNEL_C_0_DIV_4__)
TC_CLKSEL_DIV4_gc,
  #elif	defined(__GPT_CHANNEL_C_0_DIV_8__)
TC_CLKSEL_DIV8_gc,
#elif	defined(__GPT_CHANNEL_C_0_DIV_64__)
TC_CLKSEL_DIV64_gc,
#elif	defined(__GPT_CHANNEL_C_0_DIV_256__)
TC_CLKSEL_DIV256_gc,
#elif	defined(__GPT_CHANNEL_C_0_DIV_1024__)
TC_CLKSEL_DIV1024_gc,
#else
TC_CLKSEL_DIV1_gc,
#endif
#else	/* __GPT_CHANNEL_C_0__ */
TC_CLKSEL_OFF_gc,
#endif	/* __GPT_CHANNEL_C_0__ */

/* CHANNEL D 0 */
#ifdef	__GPT_CHANNEL_D_0__
#if	defined(__GPT_CHANNEL_D_0_DIV_2__)
TC_CLKSEL_DIV2_gc,
#elif	defined(__GPT_CHANNEL_D_0_DIV_4__)
TC_CLKSEL_DIV4_gc,
  #elif	defined(__GPT_CHANNEL_D_0_DIV_8__)
TC_CLKSEL_DIV8_gc,
#elif	defined(__GPT_CHANNEL_D_0_DIV_64__)
TC_CLKSEL_DIV64_gc,
#elif	defined(__GPT_CHANNEL_D_0_DIV_256__)
TC_CLKSEL_DIV256_gc,
#elif	defined(__GPT_CHANNEL_D_0_DIV_1024__)
TC_CLKSEL_DIV1024_gc,
#else
TC_CLKSEL_DIV1_gc,
#endif
#else	/* __GPT_CHANNEL_D_0__ */
TC_CLKSEL_OFF_gc,
#endif	/* __GPT_CHANNEL_D_0__ */

/* CHANNEL E 0 */
#ifdef	__GPT_CHANNEL_E_0__
#if	defined(__GPT_CHANNEL_E_0_DIV_2__)
TC_CLKSEL_DIV2_gc,
#elif	defined(__GPT_CHANNEL_E_0_DIV_4__)
TC_CLKSEL_DIV4_gc,
  #elif	defined(__GPT_CHANNEL_E_0_DIV_8__)
TC_CLKSEL_DIV8_gc,
#elif	defined(__GPT_CHANNEL_E_0_DIV_64__)
TC_CLKSEL_DIV64_gc,
#elif	defined(__GPT_CHANNEL_E_0_DIV_256__)
TC_CLKSEL_DIV256_gc,
#elif	defined(__GPT_CHANNEL_E_0_DIV_1024__)
TC_CLKSEL_DIV1024_gc,
#else
TC_CLKSEL_DIV1_gc,
#endif
#else	/* __GPT_CHANNEL_E_0__ */
TC_CLKSEL_OFF_gc,
#endif	/* __GPT_CHANNEL_E_0__ */

/* CHANNEL F 0 */
#ifdef	__GPT_CHANNEL_F_0__
#if	defined(__GPT_CHANNEL_F_0_DIV_2__)
TC_CLKSEL_DIV2_gc,
#elif	defined(__GPT_CHANNEL_F_0_DIV_4__)
TC_CLKSEL_DIV4_gc,
  #elif	defined(__GPT_CHANNEL_F_0_DIV_8__)
TC_CLKSEL_DIV8_gc,
#elif	defined(__GPT_CHANNEL_F_0_DIV_64__)
TC_CLKSEL_DIV64_gc,
#elif	defined(__GPT_CHANNEL_F_0_DIV_256__)
TC_CLKSEL_DIV256_gc,
#elif	defined(__GPT_CHANNEL_F_0_DIV_1024__)
TC_CLKSEL_DIV1024_gc,
#else
TC_CLKSEL_DIV1_gc,
#endif
#else	/* __GPT_CHANNEL_F_0__ */
TC_CLKSEL_OFF_gc,
#endif	/* __GPT_CHANNEL_F_0__ */

#endif	/* __AVR_ATxmega16D4__ || __AVR_ATxmega32D4__ */

};	/* EE_Gpt_0_Div[EE_TC_0_HW_UNITS_NUMBER] */

/*
 * Timer Counter 1 Prescalers
 */
#ifdef	DEBUG
static volatile const EE_UREG
#else
static const EE_UREG
#endif
#ifdef	__AVR_PGMSPACE__
EE_Gpt_1_Div[EE_TC_1_HW_UNITS_NUMBER] PROGMEM = {
#else	/* __AVR_PGMSPACE__ */
EE_Gpt_1_Div[EE_TC_1_HW_UNITS_NUMBER] = {
#endif	/* __AVR_PGMSPACE__ */

#if	( defined(__AVR_ATxmega16D4__) || defined(__AVR_ATxmega32D4__) )

/* CHANNEL C 1 */
#ifdef	__GPT_CHANNEL_C_1__
#if	defined(__GPT_CHANNEL_C_1_DIV_2__)
TC_CLKSEL_DIV2_gc,
#elif	defined(__GPT_CHANNEL_C_1_DIV_4__)
TC_CLKSEL_DIV4_gc,
  #elif	defined(__GPT_CHANNEL_C_1_DIV_8__)
TC_CLKSEL_DIV8_gc,
#elif	defined(__GPT_CHANNEL_C_1_DIV_64__)
TC_CLKSEL_DIV64_gc,
#elif	defined(__GPT_CHANNEL_C_1_DIV_256__)
TC_CLKSEL_DIV256_gc,
#elif	defined(__GPT_CHANNEL_C_1_DIV_1024__)
TC_CLKSEL_DIV1024_gc,
#else
TC_CLKSEL_DIV1_gc,
#endif
#else	/* __GPT_CHANNEL_C_1__ */
TC_CLKSEL_OFF_gc,
#endif	/* __GPT_CHANNEL_C_1__ */

#else	/* __AVR_ATxmega16D4__ || __AVR_ATxmega32D4__ */

/* CHANNEL C 1 */
#ifdef	__GPT_CHANNEL_C_1__
#if	defined(__GPT_CHANNEL_C_1_DIV_2__)
TC_CLKSEL_DIV2_gc,
#elif	defined(__GPT_CHANNEL_C_1_DIV_4__)
TC_CLKSEL_DIV4_gc,
  #elif	defined(__GPT_CHANNEL_C_1_DIV_8__)
TC_CLKSEL_DIV8_gc,
#elif	defined(__GPT_CHANNEL_C_1_DIV_64__)
TC_CLKSEL_DIV64_gc,
#elif	defined(__GPT_CHANNEL_C_1_DIV_256__)
TC_CLKSEL_DIV256_gc,
#elif	defined(__GPT_CHANNEL_C_1_DIV_1024__)
TC_CLKSEL_DIV1024_gc,
#else
TC_CLKSEL_DIV1_gc,
#endif
#else	/* __GPT_CHANNEL_C_1__ */
TC_CLKSEL_OFF_gc,
#endif	/* __GPT_CHANNEL_C_1__ */

/* CHANNEL D 1 */
#ifdef	__GPT_CHANNEL_D_1__
#if	defined(__GPT_CHANNEL_D_1_DIV_2__)
TC_CLKSEL_DIV2_gc,
#elif	defined(__GPT_CHANNEL_D_1_DIV_4__)
TC_CLKSEL_DIV4_gc,
  #elif	defined(__GPT_CHANNEL_D_1_DIV_8__)
TC_CLKSEL_DIV8_gc,
#elif	defined(__GPT_CHANNEL_D_1_DIV_64__)
TC_CLKSEL_DIV64_gc,
#elif	defined(__GPT_CHANNEL_D_1_DIV_256__)
TC_CLKSEL_DIV256_gc,
#elif	defined(__GPT_CHANNEL_D_1_DIV_1024__)
TC_CLKSEL_DIV1024_gc,
#else
TC_CLKSEL_DIV1_gc,
#endif
#else	/* __GPT_CHANNEL_D_1__ */
TC_CLKSEL_OFF_gc,
#endif	/* __GPT_CHANNEL_D_1__ */

/* CHANNEL E 1 */
#ifdef	__GPT_CHANNEL_E_1__
#if	defined(__GPT_CHANNEL_E_1_DIV_2__)
TC_CLKSEL_DIV2_gc,
#elif	defined(__GPT_CHANNEL_E_1_DIV_4__)
TC_CLKSEL_DIV4_gc,
  #elif	defined(__GPT_CHANNEL_E_1_DIV_8__)
TC_CLKSEL_DIV8_gc,
#elif	defined(__GPT_CHANNEL_E_1_DIV_64__)
TC_CLKSEL_DIV64_gc,
#elif	defined(__GPT_CHANNEL_E_1_DIV_256__)
TC_CLKSEL_DIV256_gc,
#elif	defined(__GPT_CHANNEL_E_1_DIV_1024__)
TC_CLKSEL_DIV1024_gc,
#else
TC_CLKSEL_DIV1_gc,
#endif
#else	/* __GPT_CHANNEL_E_1__ */
TC_CLKSEL_OFF_gc,
#endif	/* __GPT_CHANNEL_E_1__ */

/* CHANNEL F 1 */
#ifdef	__GPT_CHANNEL_F_1__
#if	defined(__GPT_CHANNEL_F_1_DIV_2__)
TC_CLKSEL_DIV2_gc,
#elif	defined(__GPT_CHANNEL_F_1_DIV_4__)
TC_CLKSEL_DIV4_gc,
  #elif	defined(__GPT_CHANNEL_F_1_DIV_8__)
TC_CLKSEL_DIV8_gc,
#elif	defined(__GPT_CHANNEL_F_1_DIV_64__)
TC_CLKSEL_DIV64_gc,
#elif	defined(__GPT_CHANNEL_F_1_DIV_256__)
TC_CLKSEL_DIV256_gc,
#elif	defined(__GPT_CHANNEL_F_1_DIV_1024__)
TC_CLKSEL_DIV1024_gc,
#else
TC_CLKSEL_DIV1_gc,
#endif
#else	/* __GPT_CHANNEL_F_1__ */
TC_CLKSEL_OFF_gc,
#endif	/* __GPT_CHANNEL_F_1__ */

#endif	/* __AVR_ATxmega16D4__ || __AVR_ATxmega32D4__ */

};	/* EE_Gpt_1_Div[EE_TC_1_HW_UNITS_NUMBER] */

/*
 * GPT Prescaler Retrieval.
 */
__INLINE__ EE_UREG __ALWAYS_INLINE__
EE_Gpt_GetDiv(
  EE_TYPEGPTCHANNEL	Channel
) {

#if	DEBUG
  volatile EE_UREG	Div;
#else	/* DEBUG */
  register EE_UREG	Div;
#endif	/* DEBUG */

  if ( Channel & EE_TC_HW_ALL_TCX1_MASK ) {

#ifdef	__AVR_PGMSPACE__
    Div = (EE_UREG) pgm_read_byte(
	&EE_Gpt_1_Div[(Channel & ~EE_TC_HW_ALL_TCX1_MASK)]
    );
#else	/* __AVR_PGMSPACE__ */
    Div = EE_Gpt_1_Div[(Channel & ~EE_TC_HW_ALL_TCX1_MASK)];
#endif	/* __AVR_PGMSPACE__ */

  }
  else {

#ifdef	__AVR_PGMSPACE__
    Div = (EE_UREG) pgm_read_byte(&EE_Gpt_0_Div[Channel]);
#else	/* __AVR_PGMSPACE__ */
    Div = EE_Gpt_0_Div[Channel];
#endif	/* __AVR_PGMSPACE__ */

  }

  return Div;

}

#else	/* __USE_TC_HW__ */

/*
 * Timer Counter Prescalers
 */
#ifdef	DEBUG
static volatile const EE_UREG
#else
static const EE_UREG
#endif
#ifdef	__AVR_PGMSPACE__
EE_Gpt_Div[EE_TC_HW_UNITS_NUMBER] = PROGMEM {
#else	__AVR_PGMSPACE__
EE_Gpt_Div[EE_TC_HW_UNITS_NUMBER] = {
#endif	/* __AVR_PGMSPACE__ */

/* CHANNEL C 0 */
#ifdef	__GPT_CHANNEL_C_0__
#if	defined(__GPT_CHANNEL_C_0_DIV_2__)
TC_CLKSEL_DIV2_gc,
#elif	defined(__GPT_CHANNEL_C_0_DIV_4__)
TC_CLKSEL_DIV4_gc,
  #elif	defined(__GPT_CHANNEL_C_0_DIV_8__)
TC_CLKSEL_DIV8_gc,
#elif	defined(__GPT_CHANNEL_C_0_DIV_64__)
TC_CLKSEL_DIV64_gc,
#elif	defined(__GPT_CHANNEL_C_0_DIV_256__)
TC_CLKSEL_DIV256_gc,
#elif	defined(__GPT_CHANNEL_C_0_DIV_1024__)
TC_CLKSEL_DIV1024_gc,
#else
TC_CLKSEL_DIV1_gc,
#endif
#else	/* __GPT_CHANNEL_C_0__ */
TC_CLKSEL_OFF_gc,
#endif	/* __GPT_CHANNEL_C_0__ */

/* CHANNEL D 0 */
#ifdef	__GPT_CHANNEL_D_0__
#if	defined(__GPT_CHANNEL_D_0_DIV_2__)
TC_CLKSEL_DIV2_gc,
#elif	defined(__GPT_CHANNEL_D_0_DIV_4__)
TC_CLKSEL_DIV4_gc,
  #elif	defined(__GPT_CHANNEL_D_0_DIV_8__)
TC_CLKSEL_DIV8_gc,
#elif	defined(__GPT_CHANNEL_D_0_DIV_64__)
TC_CLKSEL_DIV64_gc,
#elif	defined(__GPT_CHANNEL_D_0_DIV_256__)
TC_CLKSEL_DIV256_gc,
#elif	defined(__GPT_CHANNEL_D_0_DIV_1024__)
TC_CLKSEL_DIV1024_gc,
#else
TC_CLKSEL_DIV1_gc,
#endif
#else	/* __GPT_CHANNEL_D_0__ */
TC_CLKSEL_OFF_gc,
#endif	/* __GPT_CHANNEL_D_0__ */

/* CHANNEL E 0 */
#ifdef	__GPT_CHANNEL_E_0__
#if	defined(__GPT_CHANNEL_E_0_DIV_2__)
TC_CLKSEL_DIV2_gc,
#elif	defined(__GPT_CHANNEL_E_0_DIV_4__)
TC_CLKSEL_DIV4_gc,
  #elif	defined(__GPT_CHANNEL_E_0_DIV_8__)
TC_CLKSEL_DIV8_gc,
#elif	defined(__GPT_CHANNEL_E_0_DIV_64__)
TC_CLKSEL_DIV64_gc,
#elif	defined(__GPT_CHANNEL_E_0_DIV_256__)
TC_CLKSEL_DIV256_gc,
#elif	defined(__GPT_CHANNEL_E_0_DIV_1024__)
TC_CLKSEL_DIV1024_gc,
#else
TC_CLKSEL_DIV1_gc,
#endif
#else	/* __GPT_CHANNEL_E_0__ */
TC_CLKSEL_OFF_gc,
#endif	/* __GPT_CHANNEL_E_0__ */

};	/* EE_Gpt_Div[EE_TC_HW_UNITS_NUMBER] */

#endif	/* __USE_TC_HW__ */

#if	( defined(__GPT_ONESHOT__) || defined(__GPT_ENABLENOTIF_API__) )

#ifdef	__USE_TC_HW__

#if	( \
	( defined(__GPT_CHANNEL_C_0__) && defined(EE_AVR8_TCC0_OVF_ISR) ) || \
	( defined(__GPT_CHANNEL_D_0__) && defined(EE_AVR8_TCD0_OVF_ISR) ) || \
	( defined(__GPT_CHANNEL_E_0__) && defined(EE_AVR8_TCE0_OVF_ISR) ) || \
	( defined(__GPT_CHANNEL_F_0__) && defined(EE_AVR8_TCF0_OVF_ISR) ) || \
	( defined(__GPT_CHANNEL_C_1__) && defined(EE_AVR8_TCC1_OVF_ISR) ) || \
	( defined(__GPT_CHANNEL_D_1__) && defined(EE_AVR8_TCD1_OVF_ISR) ) || \
	( defined(__GPT_CHANNEL_E_1__) && defined(EE_AVR8_TCE1_OVF_ISR) ) || \
	( defined(__GPT_CHANNEL_F_1__) && defined(EE_AVR8_TCF1_OVF_ISR) ) \
)

/* GPT ISR
 *
 * param[in]	Channel		GPT Channel Identifier.
 */
static void
EE_Gpt_ISR( EE_TYPEGPTCHANNEL Channel )
{

#ifdef	DEBUG
  volatile EE_UREG		flags;
  volatile EE_UREG		stat;
#ifdef	__GPT_ONESHOT__
  volatile EE_HWREG_PTR		addr;
#endif	/* __GPT_ONESHOT__ */
#ifdef	__GPT_ENABLENOTIF_API__
  volatile EE_ISR_callback	cb;
#endif	/* __GPT_ENABLENOTIF_API__ */
#else	/* DEBUG */
  register EE_UREG		flags;
  register EE_UREG		stat;
#ifdef	__GPT_ONESHOT__
  register EE_HWREG_PTR		addr;
#endif	/* __GPT_ONESHOT__ */
#ifdef	__GPT_ENABLENOTIF_API__
  register EE_ISR_callback	cb;
#endif	/* __GPT_ENABLENOTIF_API__ */
#endif	/* DEBUG */

  /* Suspend Interrupt */
  flags = EE_hal_suspendIRQ();

  stat = EE_Tc_Hw_GetStatus(Channel);

#ifdef	__GPT_ONESHOT__
  addr = EE_TC_HW_2_ADDR(Channel);

  /* One-Shot Check! */
  if ( stat & EE_GPT_ONESHOT_MASK ) {

    /* Stop the Timer/Counter. */
    EE_HWREG(addr + EE_TC_HW_CTRLA_REG_OFS) = TC_CLKSEL_OFF_gc;

    /* Set Channel as Expired! */
    EE_Tc_Hw SetStatus(Channel, (stat | EE_GPT_EXPIRED_MASK));

  }
#endif	/* __GPT_ONESHOT__ */

#ifdef	__GPT_ENABLENOTIF_API__
  cb = EE_Tc_Hw_GetCallback(Channel);
#endif	/* __GPT_ENABLENOTIF_API__ */

  /* Resume Interrupt */
  EE_hal_resumeIRQ(flags);

#ifdef	__GPT_ENABLENOTIF_API__
  /* Notification Enabled Check!!! */
  if ( ( stat & EE_GPT_NOTIF_MASK ) && ( cb != EE_NULL_PTR ) ) {

    /* Call Port Pin CallBack. */
    cb();

  }
#endif	/* __GPT_ENABLENOTIF_API__ */

}	/* EE_Gpt_ISR( EE_TYPEGPTCHANNEL ch, EE_ISR_callback cb ) */

#if	( defined(__AVR_ATxmega16D4__) || defined(__AVR_ATxmega32D4__) )

#if	( defined(__GPT_CHANNEL_C_0__) && defined(EE_AVR8_TCC0_OVF_ISR) )
#if	( EE_AVR8_TCC0_OVF_ISR_CAT == 1 )
ISR1(EE_AVR8_TCC0_OVF_ISR)
#else
ISR2(EE_AVR8_TCC0_OVF_ISR)
#endif
{
  EE_Gpt_ISR( EE_GPT_CHANNEL_C_0 );
}
#endif	/* __GPT_CHANNEL_C_0__ && EE_AVR8_TCC0_OVF_ISR */

#if	( defined(__GPT_CHANNEL_D_0__) && defined(EE_AVR8_TCD0_OVF_ISR) )
#if	( EE_AVR8_TCD0_OVF_ISR_CAT == 1 )
ISR1(EE_AVR8_TCD0_OVF_ISR)
#else
ISR2(EE_AVR8_TCD0_OVF_ISR)
#endif
{
  EE_Gpt_ISR( EE_GPT_CHANNEL_D_0 );
}
#endif	/* __GPT_CHANNEL_D_0__ && EE_AVR8_TCD0_OVF_ISR */

#if	( defined(__GPT_CHANNEL_E_0__) && defined(EE_AVR8_TCE0_OVF_ISR) )
#if	( EE_AVR8_TCE0_OVF_ISR_CAT == 1 )
ISR1(EE_AVR8_TCE0_OVF_ISR)
#else
ISR2(EE_AVR8_TCE0_OVF_ISR)
#endif
{
  EE_Gpt_ISR( EE_GPT_CHANNEL_E_0 );
}
#endif	/* __GPT_CHANNEL_E_0__ && EE_AVR8_TCE0_OVF_ISR */

#if	( defined(__GPT_CHANNEL_C_1__) && defined(EE_AVR8_TCC1_OVF_ISR) )
#if	( EE_AVR8_TCC1_OVF_ISR_CAT == 1 )
ISR1(EE_AVR8_TCC1_OVF_ISR)
#else
ISR2(EE_AVR8_TCC1_OVF_ISR)
#endif
{
  EE_Gpt_ISR( EE_GPT_CHANNEL_C_1 );
}
#endif	/* __GPT_CHANNEL_C_1__ && EE_AVR8_TCC1_OVF_ISR */

#else	/* __AVR_ATxmega16D4__ || __AVR_ATxmega32D4__ */

#if	( defined(__GPT_CHANNEL_C_0__) && defined(EE_AVR8_TCC0_OVF_ISR) )
#if	( EE_AVR8_TCC0_OVF_ISR_CAT == 1 )
ISR1(EE_AVR8_TCC0_OVF_ISR)
#else
ISR2(EE_AVR8_TCC0_OVF_ISR)
#endif
{
  EE_Gpt_ISR( EE_GPT_CHANNEL_C_0 );
}
#endif	/* __GPT_CHANNEL_C_0__ && EE_AVR8_TCC0_OVF_ISR */

#if	( defined(__GPT_CHANNEL_D_0__) && defined(EE_AVR8_TCD0_OVF_ISR) )
#if	( EE_AVR8_TCD0_OVF_ISR_CAT == 1 )
ISR1(EE_AVR8_TCD0_OVF_ISR)
#else
ISR2(EE_AVR8_TCD0_OVF_ISR)
#endif
{
  EE_Gpt_ISR( EE_GPT_CHANNEL_D_0 );
}
#endif	/* __GPT_CHANNEL_D_0__ && EE_AVR8_TCD0_OVF_ISR */

#if	( defined(__GPT_CHANNEL_E_0__) && defined(EE_AVR8_TCE0_OVF_ISR) )
#if	( EE_AVR8_TCE0_OVF_ISR_CAT == 1 )
ISR1(EE_AVR8_TCE0_OVF_ISR)
#else
ISR2(EE_AVR8_TCE0_OVF_ISR)
#endif
{
  EE_Gpt_ISR( EE_GPT_CHANNEL_E_0 );
}
#endif	/* __GPT_CHANNEL_E_0__ && EE_AVR8_TCE0_OVF_ISR */

#if	( defined(__GPT_CHANNEL_F_0__) && defined(EE_AVR8_TCF0_OVF_ISR) )
#if	( EE_AVR8_TCF0_OVF_ISR_CAT == 1 )
ISR1(EE_AVR8_TCF0_OVF_ISR)
#else
ISR2(EE_AVR8_TCF0_OVF_ISR)
#endif
{
  EE_Gpt_ISR( EE_GPT_CHANNEL_F_0 );
}
#endif	/* __GPT_CHANNEL_F_0__ && EE_AVR8_TCF0_OVF_ISR */

#if	( defined(__GPT_CHANNEL_C_1__) && defined(EE_AVR8_TCC1_OVF_ISR) )
#if	( EE_AVR8_TCC1_OVF_ISR_CAT == 1 )
ISR1(EE_AVR8_TCC1_OVF_ISR)
#else
ISR2(EE_AVR8_TCC1_OVF_ISR)
#endif
{
  EE_Gpt_ISR( EE_GPT_CHANNEL_C_1 );
}
#endif	/* __GPT_CHANNEL_C_1__ && EE_AVR8_TCC1_OVF_ISR */

#if	( defined(__GPT_CHANNEL_D_1__) && defined(EE_AVR8_TCD1_OVF_ISR) )
#if	( EE_AVR8_TCD1_OVF_ISR_CAT == 1 )
ISR1(EE_AVR8_TCD1_OVF_ISR)
#else
ISR2(EE_AVR8_TCD1_OVF_ISR)
#endif
{
  EE_Gpt_ISR( EE_GPT_CHANNEL_D_1 );
}
#endif	/* __GPT_CHANNEL_D_1__ && EE_AVR8_TCD0_OVF_ISR */

#if	( defined(__GPT_CHANNEL_E_1__) && defined(EE_AVR8_TCE1_OVF_ISR) )
#if	( EE_AVR8_TCE1_OVF_ISR_CAT == 1 )
ISR1(EE_AVR8_TCE1_OVF_ISR)
#else
ISR2(EE_AVR8_TCE1_OVF_ISR)
#endif
{
  EE_Gpt_ISR( EE_GPT_CHANNEL_E_1 );
}
#endif	/* __GPT_CHANNEL_E_1__ && EE_AVR8_TCE1_OVF_ISR */

#if	( defined(__GPT_CHANNEL_F_1__) && defined(EE_AVR8_TCF1_OVF_ISR) )
#if	( EE_AVR8_TCF1_OVF_ISR_CAT == 1 )
ISR1(EE_AVR8_TCF1_OVF_ISR)
#else
ISR2(EE_AVR8_TCF1_OVF_ISR)
#endif
{
  EE_Gpt_ISR( EE_GPT_CHANNEL_F_1 );
}
#endif	/* __GPT_CHANNEL_F_1__ && EE_AVR8_TCF1_OVF_ISR */

#endif	/* __AVR_ATxmega16D4__ || __AVR_ATxmega32D4__ */

#endif	/*
	 * ( __GPT_CHANNEL_C_0__ && EE_AVR8_TCC0_OVF_ISR ) ||
	 * ( __GPT_CHANNEL_D_0__ && EE_AVR8_TCD0_OVF_ISR ) ||
	 * ( __GPT_CHANNEL_E_0__ && EE_AVR8_TCE0_OVF_ISR ) ||
	 * ( __GPT_CHANNEL_F_0__ && EE_AVR8_TCF0_OVF_ISR ) ||
	 * ( __GPT_CHANNEL_C_1__ && EE_AVR8_TCC1_OVF_ISR ) ||
	 * ( __GPT_CHANNEL_D_1__ && EE_AVR8_TCD1_OVF_ISR ) ||
	 * ( __GPT_CHANNEL_E_1__ && EE_AVR8_TCE1_OVF_ISR ) ||
	 * ( __GPT_CHANNEL_F_1__ && EE_AVR8_TCF1_OVF_ISR )
	 */

#else	/* __USE_TC_HW__ */

#if	( \
	( defined(__GPT_CHANNEL_C_0__) && defined(EE_AVR8_TCC0_OVF_ISR) ) || \
	( defined(__GPT_CHANNEL_D_0__) && defined(EE_AVR8_TCD0_OVF_ISR) ) || \
	( defined(__GPT_CHANNEL_E_0__) && defined(EE_AVR8_TCE0_OVF_ISR) ) \
)

/* GPT ISR
 *
 * param[in]	Channel		GPT Channel Identifier.
 */
static void
EE_Gpt_ISR( EE_TYPEGPTCHANNEL Channel )
{

#ifdef	DEBUG
  volatile EE_UREG		flags;
  volatile EE_UREG		stat;
#ifdef	__GPT_ONESHOT__
  volatile EE_HWREG_PTR		addr;
#endif	/* __GPT_ONESHOT__ */
#ifdef	__GPT_ENABLENOTIF_API__
  volatile EE_ISR_callback	cb;
#endif	/* __GPT_ENABLENOTIF_API__ */
#else	/* DEBUG */
  register EE_UREG		flags;
  register EE_UREG		stat;
#ifdef	__GPT_ONESHOT__
  register EE_HWREG_PTR		addr;
#endif	/* __GPT_ONESHOT__ */
#ifdef	__GPT_ENABLENOTIF_API__
  register EE_ISR_callback	cb;
#endif	/* __GPT_ENABLENOTIF_API__ */
#endif	/* DEBUG */

  /* Suspend Interrupt */
  flags = EE_hal_suspendIRQ();

  stat = EE_Tc_Hw_Status[Channel];

#ifdef	__GPT_ONESHOT__
  addr = EE_TC_HW_TCX0_2_ADDR(Channel);

  /* One-Shot Check! */
  if ( stat & EE_GPT_ONESHOT_MASK ) {

    /* Stop the Timer/Counter. */
    EE_HWREG(addr + EE_TC_HW_TCX0_CTRLA_REG_OFS) = TC_CLKSEL_OFF_gc;

    /* Set Channel as Expired! */
    EE_Tc_Hw_Status[Channel] |= EE_GPT_EXPIRED_MASK;

  }
#endif	/* __GPT_ONESHOT__ */

#ifdef	__GPT_ENABLENOTIF_API__
  cb = EE_Tc_Hw_CallBack[Channel];
#endif	/* __GPT_ENABLENOTIF_API__ */

  /* Resume Interrupt */
  EE_hal_resumeIRQ(flags);

#ifdef	__GPT_ENABLENOTIF_API__
  /* Notification Enabled Check!!! */
  if ( ( stat & EE_GPT_NOTIF_MASK ) && ( cb != EE_NULL_PTR ) ) {

    /* Call Port Pin CallBack. */
    (*cb)();

  }
#endif	/* __GPT_ENABLENOTIF_API__ */

}	/* EE_Gpt_ISR( EE_TYPEGPTCHANNEL ch, EE_ISR_callback cb ) */

#if	( defined(__GPT_CHANNEL_C_0__) && defined(EE_AVR8_TCC0_OVF_ISR) )
#if	( EE_AVR8_TCC0_OVF_ISR_CAT == 1 )
ISR1(EE_AVR8_TCC0_OVF_ISR)
#else
ISR2(EE_AVR8_TCC0_OVF_ISR)
#endif
{
  EE_Gpt_ISR( EE_GPT_CHANNEL_C_0 );
}
#endif	/* __GPT_CHANNEL_C_0__ && EE_AVR8_TCC0_OVF_ISR */

#if	( defined(__GPT_CHANNEL_D_0__) && defined(EE_AVR8_TCD0_OVF_ISR) )
#if	( EE_AVR8_TCD0_OVF_ISR_CAT == 1 )
ISR1(EE_AVR8_TCD0_OVF_ISR)
#else
ISR2(EE_AVR8_TCD0_OVF_ISR)
#endif
{
  EE_Gpt_ISR( EE_GPT_CHANNEL_D_0 );
}
#endif	/* __GPT_CHANNEL_D_0__ && EE_AVR8_TCD0_OVF_ISR */

#if	( defined(__GPT_CHANNEL_E_0__) && defined(EE_AVR8_TCE0_OVF_ISR) )
#if	( EE_AVR8_TCE0_OVF_ISR_CAT == 1 )
ISR1(EE_AVR8_TCE0_OVF_ISR)
#else
ISR2(EE_AVR8_TCE0_OVF_ISR)
#endif
{
  EE_Gpt_ISR( EE_GPT_CHANNEL_E_0 );
}
#endif	/* __GPT_CHANNEL_E_0__ && EE_AVR8_TCE0_OVF_ISR */

#endif	/*
	 * ( __GPT_CHANNEL_C_0__ && EE_AVR8_TCC0_OVF_ISR ) ||
	 * ( __GPT_CHANNEL_D_0__ && EE_AVR8_TCD0_OVF_ISR ) ||
	 * ( __GPT_CHANNEL_E_0__ && EE_AVR8_TCE0_OVF_ISR )
	 */

#endif	/* __USE_TC_HW__ */

#endif	/* __GPT_ONESHOT__ || __GPT_ENABLENOTIF_API__ */

#ifdef	__GPT_GETSTATUS_API__
/*
 * EE_Gpt_GetStatus() Implementation.
 */
EE_TYPEGPTCHSTAT
EE_Gpt_GetStatus(
  EE_TYPEGPTCHANNEL	Channel
) {

#ifdef	DEBUG
  volatile EE_UREG	flags;
  volatile EE_UREG	stat;
  volatile EE_HWREG_PTR	addr;
#else
  register EE_UREG	flags;
  register EE_UREG	stat;
  register EE_HWREG_PTR	addr;
#endif

#ifdef	__GPT_EXTENDED_STATUS__

  /* Channel Validation */
  EE_ATXMEGA_VALIDATE_W_RV(
#ifdef	__USE_TC_HW__
    ( Channel < EE_TC_HW_ALL_UNITS_MASK ),
#else	/* __USE_TC_HW__ */
    ( Channel < EE_TC_HW_ALL_TCX0_MASK ),
#endif	/* __USE_TC_HW__ */
    EE_GPT_CH_UNINIT
  );

#endif	/* __GPT_EXTENDED_STATUS__ */

  /* Suspend Interrupt */
  flags = EE_hal_suspendIRQ();

#ifdef	__USE_TC_HW__
  stat = EE_Tc_Hw_GetStatus(Channel);
#else	/* __USE_TC_HW__ */
  stat = EE_Tc_Hw_Status[Channel];
#endif	/* __USE_TC_HW__ */

  /* Channel Initialization Validation */
  EE_ATXMEGA_VALIDATE_IRQ_W_RV(
    ( stat & EE_TC_HW_INIT_MASK ),
    EE_GPT_CH_UNINIT,
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
    EE_GPT_CH_RUNNING,
    flags
  );

#ifdef	__GPT_ONESHOT__
  /* Channel Expiration Validation */
  EE_ATXMEGA_VALIDATE_IRQ_W_RV(
    !( stat & EE_GPT_EXPIRED_MASK ),
    EE_GPT_CH_EXPIRED,
    flags
  );
#endif	/* __GPT_ONESHOT__ */

  /* Channel Stopped Validation */
  EE_ATXMEGA_VALIDATE_IRQ_W_RV(
#ifdef	__USE_TC_HW__
    ( EE_ATxmega_Reg16Read(addr + EE_TC_HW_PER_REG_OFS) == EE_OFF ),
#else	/* __USE_TC_HW__ */
    ( EE_ATxmega_Reg16Read(addr + EE_TC_HW_TCX0_PER_REG_OFS) == 0x0000U ),
#endif	/* __USE_TC_HW__ */
    EE_GPT_CH_STOPPED,
    flags
  );

  /* Resume Interrupt */
  EE_hal_resumeIRQ(flags);

  return EE_GPT_CH_OPERATIONAL;

};
#endif	/* __GPT_GETSTATUS_API__ */

/*
 * EE_Gpt_Init() Implementation.
 */
#ifdef	__GPT_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_Gpt_Init(
  EE_TYPEGPTCHANNEL	Channel,
  EE_TYPEGPTFLAGS	Flags,
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

#ifdef	__GPT_EXTENDED_STATUS__

  /* Channel Validation */
  EE_ATXMEGA_VALIDATE_W_RV( 
    ( Channel < EE_TC_HW_ALL_UNITS_MASK ),
    E_GPT_PARAM_CHANNEL
  );

#ifdef	__GPT_ONESHOT__
  /* Flags Validation. */
  EE_ATXMEGA_VALIDATE_W_RV(
    ( !( Flags & ~EE_GPT_FLAG_ALL_MASK ) ),
    E_GPT_PARAM_FLAGS
  );
#else	/* __GPT_ONESHOT__ */
  /* Flags Validation. */
  EE_ATXMEGA_VALIDATE_W_RV(
    ( Flags == EE_GPT_FLAG_DEFAULT ),
    E_GPT_PARAM_FLAGS
  );
#endif	/* __GPT_ONESHOT__ */

#endif	/* __GPT_EXTENDED_STATUS__ */

  /* Suspend Interrupt */
  flags = EE_hal_suspendIRQ();

  stat = EE_Tc_Hw_GetStatus(Channel);

#ifdef	__GPT_EXTENDED_STATUS__

  EE_ATXMEGA_VALIDATE_IRQ_W_RV(!( stat & EE_TC_HW_INIT_MASK ), E_NOT_OK, flags);

#endif	/* __GPT_EXTENDED_STATUS__ */

  stat |= EE_TC_HW_INIT_MASK;

  addr = EE_TC_HW_2_ADDR(Channel);

  EE_HWREG(addr + EE_TC_HW_CTRLB_REG_OFS) = TC_WGMODE_NORMAL_gc;

  EE_ATxmega_Reg16Write( addr + EE_TC_HW_PER_REG_OFS, EE_OFF );

#if	( defined(__GPT_ONESHOT__) || defined(__GPT_ENABLENOTIF_API__) )
#if	( \
	( defined(__GPT_CHANNEL_C_0__) && defined(EE_AVR8_TCC0_OVF_ISR) ) || \
	( defined(__GPT_CHANNEL_D_0__) && defined(EE_AVR8_TCD0_OVF_ISR) ) || \
	( defined(__GPT_CHANNEL_E_0__) && defined(EE_AVR8_TCE0_OVF_ISR) ) || \
	( defined(__GPT_CHANNEL_F_0__) && defined(EE_AVR8_TCF0_OVF_ISR) ) || \
	( defined(__GPT_CHANNEL_C_1__) && defined(EE_AVR8_TCC1_OVF_ISR) ) || \
	( defined(__GPT_CHANNEL_D_1__) && defined(EE_AVR8_TCD1_OVF_ISR) ) || \
	( defined(__GPT_CHANNEL_E_1__) && defined(EE_AVR8_TCE1_OVF_ISR) ) || \
	( defined(__GPT_CHANNEL_F_1__) && defined(EE_AVR8_TCF1_OVF_ISR) ) \
)

#ifdef	__GPT_ONESHOT__
  if ( Flags & EE_GPT_FLAG_ONESHOT ) {

    stat |= EE_GPT_ONESHOT_MASK;

  }
#endif	/* __GPT_ONESHOT__ */

#if ( defined(__GPT_ONESHOT__) && defined(__GPT_ENABLENOTIF_API__) )
  stat &= ~( EE_GPT_NOTIF_MASK | EE_GPT_EXPIRED_MASK );
#elif defined(__GPT_ENABLENOTIF_API__)
  stat &= ~EE_GPT_NOTIF_MASK;
#else
  stat &= ~EE_GPT_EXPIRED_MASK;
#endif

#ifdef	__GPT_ENABLENOTIF_API__
  EE_Tc_Hw_SetCallback(Channel, NotifCB);

  EE_HWREG(addr + EE_TC_HW_INTCTRLA_REG_OFS) =
  ( stat & EE_TC_HW_IRQ_OVF_PRI_MASK );

  EE_HWREG(addr + EE_TC_HW_INTFLAGS_REG_OFS) = EE_TC_HW_ALL_INT_MASK;
#endif	/* __GPT_ENABLENOTIF_API__ */

#endif	/*
	 * ( __GPT_CHANNEL_C_0__ && EE_AVR8_TCC0_OVF_ISR ) ||
	 * ( __GPT_CHANNEL_D_0__ && EE_AVR8_TCD0_OVF_ISR ) ||
	 * ( __GPT_CHANNEL_E_0__ && EE_AVR8_TCE0_OVF_ISR ) ||
	 * ( __GPT_CHANNEL_F_0__ && EE_AVR8_TCF0_OVF_ISR ) ||
	 * ( __GPT_CHANNEL_C_1__ && EE_AVR8_TCC1_OVF_ISR ) ||
	 * ( __GPT_CHANNEL_D_1__ && EE_AVR8_TCD1_OVF_ISR ) ||
	 * ( __GPT_CHANNEL_E_1__ && EE_AVR8_TCE1_OVF_ISR ) ||
	 * ( __GPT_CHANNEL_F_1__ && EE_AVR8_TCF1_OVF_ISR )
	 */
#endif	/* __GPT_ONESHOT__ || __GPT_ENABLENOTIF_API__ */

  EE_Tc_Hw_SetStatus(Channel, stat);

  /* Resume IRQs */
  EE_hal_resumeIRQ(flags);

#ifdef	__GPT_EXTENDED_STATUS__
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

#ifdef	__GPT_EXTENDED_STATUS__

  /* Channel Validation */
  EE_ATXMEGA_VALIDATE_W_RV( 
    ( Channel < EE_TC_HW_ALL_TCX0_MASK ),
    E_GPT_PARAM_CHANNEL
  );

#ifdef	__GPT_ONESHOT__
  /* Flags Validation. */
  EE_ATXMEGA_VALIDATE_W_RV(
    ( !( Flags & ~EE_GPT_FLAG_ALL_MASK ) ),
    E_GPT_PARAM_FLAGS
  );
#else	/* __GPT_ONESHOT__ */
  /* Flags Validation. */
  EE_ATXMEGA_VALIDATE_W_RV(
    ( Flags == EE_GPT_FLAG_DEFAULT ),
    E_GPT_PARAM_FLAGS
  );
#endif	/* __GPT_ONESHOT__ */

#endif	/* __GPT_EXTENDED_STATUS__ */

  /* Suspend Interrupt */
  flags = EE_hal_suspendIRQ();

#ifdef	__GPT_EXTENDED_STATUS__

  EE_ATXMEGA_VALIDATE_IRQ_W_RV(
    !( EE_Tc_Hw_Status[Channel] & EE_TC_HW_INIT_MASK ),
    E_NOT_OK,
    flags
  );

#endif	/* __GPT_EXTENDED_STATUS__ */

  EE_Tc_Hw_Status[Channel] |= EE_TC_HW_INIT_MASK;

  addr = EE_TC_HW_TCX0_2_ADDR(Channel);

  EE_HWREG(addr + EE_TC_HW_TCX0_CTRLB_REG_OFS) = TC_WGMODE_NORMAL_gc;

  EE_ATxmega_Reg16Write( addr + EE_TC_HW_TCX0_PER_REG_OFS, 0x0000U );

#if	( defined(__GPT_ONESHOT__) || defined(__GPT_ENABLENOTIF_API__) )
#if	( \
	( defined(__GPT_CHANNEL_C_0__) && defined(EE_AVR8_TCC0_OVF_ISR) ) || \
	( defined(__GPT_CHANNEL_D_0__) && defined(EE_AVR8_TCD0_OVF_ISR) ) || \
	( defined(__GPT_CHANNEL_E_0__) && defined(EE_AVR8_TCE0_OVF_ISR) ) \
)

#ifdef	__GPT_ONESHOT__
  if ( Flags & EE_GPT_FLAG_ONESHOT ) {

    EE_Tc_Hw_Status[Channel] |= EE_GPT_ONESHOT_MASK;

  }
#endif	/* __GPT_ONESHOT__ */

#if ( defined(__GPT_ONESHOT__) && defined(__GPT_ENABLENOTIF_API__) )
  EE_Tc_Hw_Status[Channel] &= ~( EE_GPT_NOTIF_MASK | EE_GPT_EXPIRED_MASK );
#elif defined(__GPT_ENABLENOTIF_API__)
  EE_Tc_Hw_Status[Channel] &= ~EE_GPT_NOTIF_MASK;
#else
  EE_Tc_Hw_Status[Channel] &= ~EE_GPT_EXPIRED_MASK;
#endif

#ifdef	__GPT_ENABLENOTIF_API__
  EE_Tc_Hw_CallBack[Channel] = NotifCB;

  EE_HWREG(addr + EE_TC_HW_TCX0_INTCTRLA_REG_OFS) =
  ( EE_Tc_Hw_Status[Channel] & EE_TC_HW_IRQ_OVF_PRI_MASK );

  EE_HWREG(addr + EE_TC_HW_TCX0_INTFLAGS_REG_OFS) = EE_TC_HW_ALL_INT_MASK;
#endif	/* __GPT_ENABLENOTIF_API__ */

#endif	/*
	 * ( __GPT_CHANNEL_C_0__ && EE_AVR8_TCC0_OVF_ISR ) ||
	 * ( __GPT_CHANNEL_D_0__ && EE_AVR8_TCD0_OVF_ISR ) ||
	 * ( __GPT_CHANNEL_E_0__ && EE_AVR8_TCE0_OVF_ISR )
	 */
#endif	/* __GPT_ONESHOT__ || __GPT_ENABLENOTIF_API__ */

  /* Resume IRQs */
  EE_hal_resumeIRQ(flags);

#ifdef	__GPT_EXTENDED_STATUS__
  return E_OK;
#endif

#endif	/* __USE_TC_HW__ */

}	/* EE_Gpt_Init() */

#ifdef	__GPT_DEINIT_API__
/*
 * EE_Gpt_DeInit() Implementation.
 */
#ifdef	__GPT_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_Gpt_DeInit(
  EE_TYPEGPTCHANNEL	Channel,
  EE_TYPEGPTFLAGS	Flags
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

#ifdef	__GPT_EXTENDED_STATUS__

  /* Channel Validation */
  EE_ATXMEGA_VALIDATE_W_RV( 
    ( Channel < EE_TC_HW_ALL_UNITS_MASK ),
    E_GPT_PARAM_CHANNEL
  );

  /* Flags Validation. */
  EE_ATXMEGA_VALIDATE_W_RV(
    ( Flags == EE_GPT_FLAG_DEFAULT ),
    E_GPT_PARAM_FLAGS
  );

#endif	/* __GPT_EXTENDED_STATUS__ */

  /* Suspend Interrupt */
  flags = EE_hal_suspendIRQ();

  stat = EE_Tc_Hw_GetStatus(Channel);

#ifdef	__GPT_EXTENDED_STATUS__

  EE_ATXMEGA_VALIDATE_IRQ_W_RV(
    ( stat & EE_TC_HW_INIT_MASK ),
    E_NOT_OK,
    flags
  );

#endif	/* __GPT_EXTENDED_STATUS__ */

  addr = EE_EE_TC_HW_2_ADDR(Channel);

#ifdef	__GPT_EXTENDED_STATUS__

  EE_ATXMEGA_VALIDATE_IRQ_W_RV(
    ( EE_HWREG(addr + EE_TC_HW_CTRLA_REG_OFS) == TC_CLKSEL_OFF_gc ),
    E_GPT_CHANNEL_BUSY,
    flags
  );

#endif	/* __GPT_EXTENDED_STATUS__ */

  stat &= ~EE_TC_HW_INIT_MASK;

  EE_ATxmega_Reg16Write( addr + EE_TC_HW_PER_REG_OFS, 0xFFFFU );

#if	( defined(__GPT_ONESHOT__) || defined(__GPT_ENABLENOTIF_API__) )
#if	( \
	( defined(__GPT_CHANNEL_C_0__) && defined(EE_AVR8_TCC0_OVF_ISR) ) || \
	( defined(__GPT_CHANNEL_D_0__) && defined(EE_AVR8_TCD0_OVF_ISR) ) || \
	( defined(__GPT_CHANNEL_E_0__) && defined(EE_AVR8_TCE0_OVF_ISR) ) || \
	( defined(__GPT_CHANNEL_F_0__) && defined(EE_AVR8_TCF0_OVF_ISR) ) || \
	( defined(__GPT_CHANNEL_C_1__) && defined(EE_AVR8_TCC1_OVF_ISR) ) || \
	( defined(__GPT_CHANNEL_D_1__) && defined(EE_AVR8_TCD1_OVF_ISR) ) || \
	( defined(__GPT_CHANNEL_E_1__) && defined(EE_AVR8_TCE1_OVF_ISR) ) || \
	( defined(__GPT_CHANNEL_F_1__) && defined(EE_AVR8_TCF1_OVF_ISR) ) \
)

#ifdef	__GPT_ENABLENOTIF_API__
  EE_HWREG(addr + EE_TC_HW_INTFLAGS_REG_OFS) = EE_TC_HW_ALL_INT_MASK;

  EE_HWREG(addr + EE_TC_HW_INTCTRLA_REG_OFS) = EE_ISR_PRI_OFF;

  EE_Tc_Hw_SetCallback(Channel, EE_NULL_PTR);
#endif	/* __GPT_ENABLENOTIF_API__ */

#if	( defined(__GPT_ONESHOT__) && defined(__GPT_ENABLENOTIF_API__) )
  stat &= ~( EE_GPT_NOTIF_MASK | EE_GPT_EXPIRED_MASK | EE_GPT_ONESHOT_MASK );
#elif	defined(__GPT_ENABLENOTIF_API__)
  stat &= ~EE_GPT_NOTIF_MASK;
#else
  stat &= ~( EE_GPT_EXPIRED_MASK | EE_GPT_ONESHOT_MASK );
#endif	/* __GPT_ONESHOT__ */

#endif	/*
	 * ( __GPT_CHANNEL_C_0__ && EE_AVR8_TCC0_OVF_ISR ) ||
	 * ( __GPT_CHANNEL_D_0__ && EE_AVR8_TCD0_OVF_ISR ) ||
	 * ( __GPT_CHANNEL_E_0__ && EE_AVR8_TCE0_OVF_ISR ) ||
	 * ( __GPT_CHANNEL_F_0__ && EE_AVR8_TCF0_OVF_ISR ) ||
	 * ( __GPT_CHANNEL_C_1__ && EE_AVR8_TCC1_OVF_ISR ) ||
	 * ( __GPT_CHANNEL_D_1__ && EE_AVR8_TCD1_OVF_ISR ) ||
	 * ( __GPT_CHANNEL_E_1__ && EE_AVR8_TCE1_OVF_ISR ) ||
	 * ( __GPT_CHANNEL_F_1__ && EE_AVR8_TCF1_OVF_ISR )
	 */
#endif	/* __GPT_ONESHOT__ || __GPT_ENABLENOTIF_API__ */

  EE_Tc_Hw_SetStatus(Channel, stat);

  /* Resume IRQs */
  EE_hal_resumeIRQ(flags);

#ifdef	__GPT_EXTENDED_STATUS__
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

#ifdef	__GPT_EXTENDED_STATUS__

  /* Channel Validation */
  EE_ATXMEGA_VALIDATE_W_RV( 
    ( Channel < EE_TC_HW_ALL_TCX0_MASK ),
    E_GPT_PARAM_CHANNEL
  );

  /* Flags Validation. */
  EE_ATXMEGA_VALIDATE_W_RV(
    ( Flags == EE_GPT_FLAG_DEFAULT ),
    E_GPT_PARAM_FLAGS
  );

#endif	/* __GPT_EXTENDED_STATUS__ */

  /* Suspend Interrupt */
  flags = EE_hal_suspendIRQ();

#ifdef	__GPT_EXTENDED_STATUS__

  EE_ATXMEGA_VALIDATE_IRQ_W_RV(
    ( EE_Tc_Hw_Status[Channel] & EE_TC_HW_INIT_MASK ),
    E_NOT_OK,
    flags
  );

#endif	/* __GPT_EXTENDED_STATUS__ */

  addr = EE_EE_TC_HW_TCX0_2_ADDR(Channel);

#ifdef	__GPT_EXTENDED_STATUS__

  EE_ATXMEGA_VALIDATE_IRQ_W_RV(
    ( EE_HWREG(addr + EE_TC_HW_TCX0_CTRLA_REG_OFS) == TC_CLKSEL_OFF_gc ),
    E_GPT_CHANNEL_BUSY,
    flags
  );

#endif	/* __GPT_EXTENDED_STATUS__ */

  EE_Tc_Hw_Status[Channel] &= ~EE_TC_HW_INIT_MASK;

  EE_ATxmega_Reg16Write( addr + EE_TC_HW_TCX0_PER_REG_OFS, 0xFFFFU );

#if	( defined(__GPT_ONESHOT__) || defined(__GPT_ENABLENOTIF_API__) )
#if	( \
	( defined(__GPT_CHANNEL_C_0__) && defined(EE_AVR8_TCC0_OVF_ISR) ) || \
	( defined(__GPT_CHANNEL_D_0__) && defined(EE_AVR8_TCD0_OVF_ISR) ) || \
	( defined(__GPT_CHANNEL_E_0__) && defined(EE_AVR8_TCE0_OVF_ISR) ) \
)

#ifdef	__GPT_ENABLENOTIF_API__
  EE_HWREG(addr + EE_TC_HW_TCX0_INTFLAGS_REG_OFS) = EE_TC_HW_ALL_INT_MASK;

  EE_HWREG(addr + EE_TC_HW_TCX0_INTCTRLA_REG_OFS) = EE_ISR_PRI_OFF;

  EE_Tc_Hw_CallBack[Channel] = EE_NULL_PTR;
#endif	/* __GPT_ENABLENOTIF_API__ */

#if	( defined(__GPT_ONESHOT__) && defined(__GPT_ENABLENOTIF_API__) )
  EE_Tc_Hw_Status[Channel] &= ~( 
    EE_GPT_NOTIF_MASK | EE_GPT_EXPIRED_MASK | EE_GPT_ONESHOT_MASK
  );
#elif	defined(__GPT_ENABLENOTIF_API__)
  EE_Tc_Hw_Status[Channel] &= ~EE_GPT_NOTIF_MASK;
#else
  EE_Tc_Hw_Status[Channel] &= ~( EE_GPT_EXPIRED_MASK | EE_GPT_ONESHOT_MASK );
#endif	/* __GPT_ONESHOT__ */

#endif	/*
	 * ( __GPT_CHANNEL_C_0__ && EE_AVR8_TCC0_OVF_ISR ) ||
	 * ( __GPT_CHANNEL_D_0__ && EE_AVR8_TCD0_OVF_ISR ) ||
	 * ( __GPT_CHANNEL_E_0__ && EE_AVR8_TCE0_OVF_ISR )
	 */
#endif	/* __GPT_ONESHOT__ || __GPT_ENABLENOTIF_API__ */

  /* Resume IRQs */
  EE_hal_resumeIRQ(flags);

#ifdef	__GPT_EXTENDED_STATUS__
  return E_OK;
#endif

#endif	/* __USE_TC_HW__ */

}
#endif	/* __GPT_DEINIT_API__ */

#ifdef	__GPT_ENABLENOTIF_API__
/*
 * EE_Gpt_EnableNotification() Implementation.
 */
#ifdef	__GPT_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_Gpt_EnableNotification(
  EE_TYPEGPTCHANNEL	Channel
) {

#ifdef	__USE_TC_HW__

#ifdef	DEBUG
  volatile EE_UREG	flags;
  volatile EE_UREG	stat;
#else
  register EE_UREG	flags;
  register EE_UREG	stat;
#endif

#ifdef	__GPT_EXTENDED_STATUS__

  /* Channel Validation */
  EE_ATXMEGA_VALIDATE_W_RV( 
    ( Channel < EE_TC_HW_ALL_UNITS_MASK ),
    E_GPT_PARAM_CHANNEL
  );

#endif	/* __GPT_EXTENDED_STATUS__ */

  /* Suspend Interrupt */
  flags = EE_hal_suspendIRQ();

  stat = EE_Tc_Hw_GetStatus(Channel);

#ifdef	__GPT_EXTENDED_STATUS__

  EE_ATXMEGA_VALIDATE_IRQ_W_RV(( stat & EE_TC_HW_INIT_MASK ), E_NOT_OK, flags);

#if	( \
	( defined(__GPT_CHANNEL_C_0__) && defined(EE_AVR8_TCC0_OVF_ISR) ) || \
	( defined(__GPT_CHANNEL_D_0__) && defined(EE_AVR8_TCD0_OVF_ISR) ) || \
	( defined(__GPT_CHANNEL_E_0__) && defined(EE_AVR8_TCE0_OVF_ISR) ) || \
	( defined(__GPT_CHANNEL_F_0__) && defined(EE_AVR8_TCF0_OVF_ISR) ) || \
	( defined(__GPT_CHANNEL_C_1__) && defined(EE_AVR8_TCC1_OVF_ISR) ) || \
	( defined(__GPT_CHANNEL_D_1__) && defined(EE_AVR8_TCD1_OVF_ISR) ) || \
	( defined(__GPT_CHANNEL_E_1__) && defined(EE_AVR8_TCE1_OVF_ISR) ) || \
	( defined(__GPT_CHANNEL_F_1__) && defined(EE_AVR8_TCF1_OVF_ISR) ) \
)

  /* Call-Back Validation. */
  EE_ATXMEGA_VALIDATE_W_RV(
    ( EE_Tc_Hw_GetCallback(Channel) != EE_NULL_PTR ),
    E_GPT_PARAM_CALLBACK
  );

#else

  /* Call-Back Validation. */
  EE_ATXMEGA_VALIDATE_W_RV( EE_FALSE, E_GPT_PARAM_CALLBACK );

#endif	/*
	 * ( __GPT_CHANNEL_C_0__ && EE_AVR8_TCC0_OVF_ISR ) ||
	 * ( __GPT_CHANNEL_D_0__ && EE_AVR8_TCD0_OVF_ISR ) ||
	 * ( __GPT_CHANNEL_E_0__ && EE_AVR8_TCE0_OVF_ISR ) ||
	 * ( __GPT_CHANNEL_F_0__ && EE_AVR8_TCF0_OVF_ISR ) ||
	 * ( __GPT_CHANNEL_C_1__ && EE_AVR8_TCC1_OVF_ISR ) ||
	 * ( __GPT_CHANNEL_D_1__ && EE_AVR8_TCD1_OVF_ISR ) ||
	 * ( __GPT_CHANNEL_E_1__ && EE_AVR8_TCE1_OVF_ISR ) ||
	 * ( __GPT_CHANNEL_F_1__ && EE_AVR8_TCF1_OVF_ISR )
	 */

#endif	/* __GPT_EXTENDED_STATUS__ */

#if	( \
	( defined(__GPT_CHANNEL_C_0__) && defined(EE_AVR8_TCC0_OVF_ISR) ) || \
	( defined(__GPT_CHANNEL_D_0__) && defined(EE_AVR8_TCD0_OVF_ISR) ) || \
	( defined(__GPT_CHANNEL_E_0__) && defined(EE_AVR8_TCE0_OVF_ISR) ) || \
	( defined(__GPT_CHANNEL_F_0__) && defined(EE_AVR8_TCF0_OVF_ISR) ) || \
	( defined(__GPT_CHANNEL_C_1__) && defined(EE_AVR8_TCC1_OVF_ISR) ) || \
	( defined(__GPT_CHANNEL_D_1__) && defined(EE_AVR8_TCD1_OVF_ISR) ) || \
	( defined(__GPT_CHANNEL_E_1__) && defined(EE_AVR8_TCE1_OVF_ISR) ) || \
	( defined(__GPT_CHANNEL_F_1__) && defined(EE_AVR8_TCF1_OVF_ISR) ) \
)

  EE_Tc_Hw_SetStatus(Channel, (stat | EE_GPT_NOTIF_MASK));

#endif	/*
	 * ( __GPT_CHANNEL_C_0__ && EE_AVR8_TCC0_OVF_ISR ) ||
	 * ( __GPT_CHANNEL_D_0__ && EE_AVR8_TCD0_OVF_ISR ) ||
	 * ( __GPT_CHANNEL_E_0__ && EE_AVR8_TCE0_OVF_ISR ) ||
	 * ( __GPT_CHANNEL_F_0__ && EE_AVR8_TCF0_OVF_ISR ) ||
	 * ( __GPT_CHANNEL_C_1__ && EE_AVR8_TCC1_OVF_ISR ) ||
	 * ( __GPT_CHANNEL_D_1__ && EE_AVR8_TCD1_OVF_ISR ) ||
	 * ( __GPT_CHANNEL_E_1__ && EE_AVR8_TCE1_OVF_ISR ) ||
	 * ( __GPT_CHANNEL_F_1__ && EE_AVR8_TCF1_OVF_ISR )
	 */

  /* Resume IRQs */
  EE_hal_resumeIRQ(flags);

#ifdef	__GPT_EXTENDED_STATUS__
  return E_OK;
#endif

#else	/* __USE_TC_HW__ */

#ifdef	DEBUG
  volatile EE_UREG	flags;
#else
  register EE_UREG	flags;
#endif

#ifdef	__GPT_EXTENDED_STATUS__

  /* Channel Validation */
  EE_ATXMEGA_VALIDATE_W_RV( 
    ( Channel < EE_TC_HW_ALL_TCX0_MASK ),
    E_GPT_PARAM_CHANNEL
  );

#endif	/* __GPT_EXTENDED_STATUS__ */

  /* Suspend Interrupt */
  flags = EE_hal_suspendIRQ();

#ifdef	__GPT_EXTENDED_STATUS__

  EE_ATXMEGA_VALIDATE_IRQ_W_RV(
    ( EE_Tc_Hw_Status[Channel] & EE_TC_HW_INIT_MASK ),
    E_NOT_OK,
    flags
  );

#if	( \
	( defined(__GPT_CHANNEL_C_0__) && defined(EE_AVR8_TCC0_OVF_ISR) ) || \
	( defined(__GPT_CHANNEL_D_0__) && defined(EE_AVR8_TCD0_OVF_ISR) ) || \
	( defined(__GPT_CHANNEL_E_0__) && defined(EE_AVR8_TCE0_OVF_ISR) ) \
)

  /* Call-Back Validation. */
  EE_ATXMEGA_VALIDATE_W_RV(
    ( EE_Tc_Hw_CallBack[Channel] != EE_NULL_PTR ),
    E_GPT_PARAM_CALLBACK
  );

#else

  /* Call-Back Validation. */
  EE_ATXMEGA_VALIDATE_W_RV( EE_FALSE, E_GPT_PARAM_CALLBACK );

#endif	/*
	 * ( __GPT_CHANNEL_C_0__ && EE_AVR8_TCC0_OVF_ISR ) ||
	 * ( __GPT_CHANNEL_D_0__ && EE_AVR8_TCD0_OVF_ISR ) ||
	 * ( __GPT_CHANNEL_E_0__ && EE_AVR8_TCE0_OVF_ISR )
	 */

#endif	/* __GPT_EXTENDED_STATUS__ */

#if	( \
	( defined(__GPT_CHANNEL_C_0__) && defined(EE_AVR8_TCC0_OVF_ISR) ) || \
	( defined(__GPT_CHANNEL_D_0__) && defined(EE_AVR8_TCD0_OVF_ISR) ) || \
	( defined(__GPT_CHANNEL_E_0__) && defined(EE_AVR8_TCE0_OVF_ISR) ) \
)

  EE_Tc_Hw_Status[Channel] |= EE_GPT_NOTIF_MASK;

#endif	/*
	 * ( __GPT_CHANNEL_C_0__ && EE_AVR8_TCC0_OVF_ISR ) ||
	 * ( __GPT_CHANNEL_D_0__ && EE_AVR8_TCD0_OVF_ISR ) ||
	 * ( __GPT_CHANNEL_E_0__ && EE_AVR8_TCE0_OVF_ISR )
	 */

  /* Resume IRQs */
  EE_hal_resumeIRQ(flags);

#ifdef	__GPT_EXTENDED_STATUS__
  return E_OK;
#endif

#endif	/* __USE_TC_HW__ */

}	/* EE_Gpt_EnableNotification() */

#ifdef	__GPT_DISABLENOTIF_API__
/*
 * EE_Gpt_DisableNotification() Implementation.
 */
#ifdef	__GPT_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_Gpt_DisableNotification(
  EE_TYPEGPTCHANNEL	Channel
) {

#ifdef	__USE_TC_HW__

#ifdef	DEBUG
  volatile EE_UREG	flags;
  volatile EE_UREG	stat;
#else
  register EE_UREG	flags;
  register EE_UREG	stat;
#endif

#ifdef	__GPT_EXTENDED_STATUS__

  /* Channel Validation */
  EE_ATXMEGA_VALIDATE_W_RV( 
    ( Channel < EE_TC_HW_ALL_UNITS_MASK ),
    E_GPT_PARAM_CHANNEL
  );

#endif	/* __GPT_EXTENDED_STATUS__ */

  /* Suspend Interrupt */
  flags = EE_hal_suspendIRQ();

  stat = EE_Tc_Hw_GetStatus(Channel);

#ifdef	__GPT_EXTENDED_STATUS__

  EE_ATXMEGA_VALIDATE_IRQ_W_RV(( stat & EE_TC_HW_INIT_MASK ), E_NOT_OK, flags);

#if	( \
	( defined(__GPT_CHANNEL_C_0__) && defined(EE_AVR8_TCC0_OVF_ISR) ) || \
	( defined(__GPT_CHANNEL_D_0__) && defined(EE_AVR8_TCD0_OVF_ISR) ) || \
	( defined(__GPT_CHANNEL_E_0__) && defined(EE_AVR8_TCE0_OVF_ISR) ) || \
	( defined(__GPT_CHANNEL_F_0__) && defined(EE_AVR8_TCF0_OVF_ISR) ) || \
	( defined(__GPT_CHANNEL_C_1__) && defined(EE_AVR8_TCC1_OVF_ISR) ) || \
	( defined(__GPT_CHANNEL_D_1__) && defined(EE_AVR8_TCD1_OVF_ISR) ) || \
	( defined(__GPT_CHANNEL_E_1__) && defined(EE_AVR8_TCE1_OVF_ISR) ) || \
	( defined(__GPT_CHANNEL_F_1__) && defined(EE_AVR8_TCF1_OVF_ISR) ) \
)

  /* Call-Back Validation. */
  EE_ATXMEGA_VALIDATE_W_RV(
    ( EE_Tc_Hw_GetCallback(Channel) != EE_NULL_PTR ),
    E_GPT_PARAM_CALLBACK
  );

#else

  /* Call-Back Validation. */
  EE_ATXMEGA_VALIDATE_W_RV( EE_FALSE, E_GPT_PARAM_CALLBACK );

#endif	/*
	 * ( __GPT_CHANNEL_C_0__ && EE_AVR8_TCC0_OVF_ISR ) ||
	 * ( __GPT_CHANNEL_D_0__ && EE_AVR8_TCD0_OVF_ISR ) ||
	 * ( __GPT_CHANNEL_E_0__ && EE_AVR8_TCE0_OVF_ISR ) ||
	 * ( __GPT_CHANNEL_F_0__ && EE_AVR8_TCF0_OVF_ISR ) ||
	 * ( __GPT_CHANNEL_C_1__ && EE_AVR8_TCC1_OVF_ISR ) ||
	 * ( __GPT_CHANNEL_D_1__ && EE_AVR8_TCD1_OVF_ISR ) ||
	 * ( __GPT_CHANNEL_E_1__ && EE_AVR8_TCE1_OVF_ISR ) ||
	 * ( __GPT_CHANNEL_F_1__ && EE_AVR8_TCF1_OVF_ISR )
	 */

#endif	/* __GPT_EXTENDED_STATUS__ */

#if	( \
	( defined(__GPT_CHANNEL_C_0__) && defined(EE_AVR8_TCC0_OVF_ISR) ) || \
	( defined(__GPT_CHANNEL_D_0__) && defined(EE_AVR8_TCD0_OVF_ISR) ) || \
	( defined(__GPT_CHANNEL_E_0__) && defined(EE_AVR8_TCE0_OVF_ISR) ) || \
	( defined(__GPT_CHANNEL_F_0__) && defined(EE_AVR8_TCF0_OVF_ISR) ) || \
	( defined(__GPT_CHANNEL_C_1__) && defined(EE_AVR8_TCC1_OVF_ISR) ) || \
	( defined(__GPT_CHANNEL_D_1__) && defined(EE_AVR8_TCD1_OVF_ISR) ) || \
	( defined(__GPT_CHANNEL_E_1__) && defined(EE_AVR8_TCE1_OVF_ISR) ) || \
	( defined(__GPT_CHANNEL_F_1__) && defined(EE_AVR8_TCF1_OVF_ISR) ) \
)

  EE_Tc_Hw_SetStatus(Channel, (stat & ~EE_GPT_NOTIF_MASK));

#endif	/*
	 * ( __GPT_CHANNEL_C_0__ && EE_AVR8_TCC0_OVF_ISR ) ||
	 * ( __GPT_CHANNEL_D_0__ && EE_AVR8_TCD0_OVF_ISR ) ||
	 * ( __GPT_CHANNEL_E_0__ && EE_AVR8_TCE0_OVF_ISR ) ||
	 * ( __GPT_CHANNEL_F_0__ && EE_AVR8_TCF0_OVF_ISR ) ||
	 * ( __GPT_CHANNEL_C_1__ && EE_AVR8_TCC1_OVF_ISR ) ||
	 * ( __GPT_CHANNEL_D_1__ && EE_AVR8_TCD1_OVF_ISR ) ||
	 * ( __GPT_CHANNEL_E_1__ && EE_AVR8_TCE1_OVF_ISR ) ||
	 * ( __GPT_CHANNEL_F_1__ && EE_AVR8_TCF1_OVF_ISR )
	 */

  /* Resume IRQs */
  EE_hal_resumeIRQ(flags);

#ifdef	__GPT_EXTENDED_STATUS__
  return E_OK;
#endif

#else	/* __USE_TC_HW__ */

#ifdef	DEBUG
  volatile EE_UREG	flags;
#else
  register EE_UREG	flags;
#endif

#ifdef	__GPT_EXTENDED_STATUS__

  /* Channel Validation */
  EE_ATXMEGA_VALIDATE_W_RV( 
    ( Channel < EE_TC_HW_ALL_TCX0_MASK ),
    E_GPT_PARAM_CHANNEL
  );

#endif	/* __GPT_EXTENDED_STATUS__ */

  /* Suspend Interrupt */
  flags = EE_hal_suspendIRQ();

#ifdef	__GPT_EXTENDED_STATUS__

  EE_ATXMEGA_VALIDATE_IRQ_W_RV(
    ( EE_Tc_Hw_Status[Channel] & EE_TC_HW_INIT_MASK ),
    E_NOT_OK,
    flags
  );

#if	( \
	( defined(__GPT_CHANNEL_C_0__) && defined(EE_AVR8_TCC0_OVF_ISR) ) || \
	( defined(__GPT_CHANNEL_D_0__) && defined(EE_AVR8_TCD0_OVF_ISR) ) || \
	( defined(__GPT_CHANNEL_E_0__) && defined(EE_AVR8_TCE0_OVF_ISR) ) \
)

  /* Call-Back Validation. */
  EE_ATXMEGA_VALIDATE_W_RV(
    ( EE_Tc_Hw_CallBack[Channel] != EE_NULL_PTR ),
    E_GPT_PARAM_CALLBACK
  );

#else

  /* Call-Back Validation. */
  EE_ATXMEGA_VALIDATE_W_RV( EE_FALSE, E_GPT_PARAM_CALLBACK );

#endif	/*
	 * ( __GPT_CHANNEL_C_0__ && EE_AVR8_TCC0_OVF_ISR ) ||
	 * ( __GPT_CHANNEL_D_0__ && EE_AVR8_TCD0_OVF_ISR ) ||
	 * ( __GPT_CHANNEL_E_0__ && EE_AVR8_TCE0_OVF_ISR )
	 */

#endif	/* __GPT_EXTENDED_STATUS__ */

#if	( \
	( defined(__GPT_CHANNEL_C_0__) && defined(EE_AVR8_TCC0_OVF_ISR) ) || \
	( defined(__GPT_CHANNEL_D_0__) && defined(EE_AVR8_TCD0_OVF_ISR) ) || \
	( defined(__GPT_CHANNEL_E_0__) && defined(EE_AVR8_TCE0_OVF_ISR) ) \
)

  EE_Tc_Hw_Status[Channel] &= ~EE_GPT_NOTIF_MASK;

#endif	/*
	 * ( __GPT_CHANNEL_C_0__ && EE_AVR8_TCC0_OVF_ISR ) ||
	 * ( __GPT_CHANNEL_D_0__ && EE_AVR8_TCD0_OVF_ISR ) ||
	 * ( __GPT_CHANNEL_E_0__ && EE_AVR8_TCE0_OVF_ISR )
	 */

  /* Resume IRQs */
  EE_hal_resumeIRQ(flags);

#ifdef	__GPT_EXTENDED_STATUS__
  return E_OK;
#endif

#endif	/* __USE_TC_HW__ */

}
#endif	/* __GPT_DISABLENOTIF_API__ */
#endif	/* __GPT_ENABLENOTIF_API__ */

#ifdef	__GPT_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_Gpt_StartTimer(
  EE_TYPEGPTCHANNEL	Channel,
  EE_TYPEGPTCOUNTER	Counter
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

#ifdef	__GPT_EXTENDED_STATUS__

  /* Channel Validation */
  EE_ATXMEGA_VALIDATE_W_RV( 
    ( Channel < EE_TC_HW_ALL_UNITS_MASK ),
    E_GPT_PARAM_CHANNEL
  );

    /* Channel Validation */
  EE_ATXMEGA_VALIDATE_W_RV(( Counter != EE_OFF ), E_GPT_PARAM_COUNTER);

#endif	/* __GPT_EXTENDED_STATUS__ */

  /* Suspend Interrupt */
  flags = EE_hal_suspendIRQ();

  stat = EE_Tc_Hw_GetStatus(Channel);

#ifdef	__GPT_EXTENDED_STATUS__

  EE_ATXMEGA_VALIDATE_IRQ_W_RV(( stat & EE_TC_HW_INIT_MASK ), E_NOT_OK, flags);

#endif	/* __GPT_EXTENDED_STATUS__ */

  addr = EE_TC_HW_2_ADDR(Channel);

#ifdef	__GPT_EXTENDED_STATUS__

  EE_ATXMEGA_VALIDATE_IRQ_W_RV(
    ( EE_HWREG(addr + EE_TC_HW_CTRLA_REG_OFS) == TC_CLKSEL_OFF_gc ),
    E_GPT_CHANNEL_BUSY,
    flags
  );

#endif	/* __GPT_EXTENDED_STATUS__ */

#ifdef	__GPT_ONESHOT__
  EE_Tc_Hw_SetStatus(Channel, ( stat & ~EE_TC_HW_EXPIRED_MASK ));
#endif

  EE_ATxmega_Reg16Write( addr + EE_TC_HW_CNT_REG_OFS, EE_OFF );

  EE_ATxmega_Reg16Write( addr + EE_TC_HW_PER_REG_OFS, Counter );

  EE_HWREG(addr + EE_TC_HW_CTRLA_REG_OFS) = EE_Gpt_GetDiv(Channel);

  /* Resume IRQs */
  EE_hal_resumeIRQ(flags);

#ifdef	__GPT_EXTENDED_STATUS__
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

#ifdef	__GPT_EXTENDED_STATUS__

  /* Channel Validation */
  EE_ATXMEGA_VALIDATE_W_RV( 
    ( Channel < EE_TC_HW_ALL_TCX0_MASK ),
    E_GPT_PARAM_CHANNEL
  );

    /* Channel Validation */
  EE_ATXMEGA_VALIDATE_W_RV( 
    ( Counter != 0x0000U ),
    E_GPT_PARAM_COUNTER
  );

#endif	/* __GPT_EXTENDED_STATUS__ */

  /* Suspend Interrupt */
  flags = EE_hal_suspendIRQ();

#ifdef	__GPT_EXTENDED_STATUS__

  EE_ATXMEGA_VALIDATE_IRQ_W_RV(
    ( EE_Tc_Hw_Status[Channel] & EE_TC_HW_INIT_MASK ),
    E_NOT_OK,
    flags
  );

#endif	/* __GPT_EXTENDED_STATUS__ */

  addr = EE_TC_HW_TCX0_2_ADDR(Channel);

#ifdef	__GPT_EXTENDED_STATUS__

  EE_ATXMEGA_VALIDATE_IRQ_W_RV(
    ( EE_HWREG(addr + EE_TC_HW_TCX0_CTRLA_REG_OFS) == TC_CLKSEL_OFF_gc ),
    E_GPT_CHANNEL_BUSY,
    flags
  );

#endif	/* __GPT_EXTENDED_STATUS__ */

#ifdef	__GPT_ONESHOT__
  EE_Tc_Hw_Status[Channel] &= ~EE_TC_HW_EXPIRED_MASK;
#endif

  EE_ATxmega_Reg16Write( addr + EE_TC_HW_TCX0_CNT_REG_OFS, 0x0000U );

  EE_ATxmega_Reg16Write( addr + EE_TC_HW_TCX0_PER_REG_OFS, Counter );

  EE_HWREG(addr + EE_TC_HW_TCX0_CTRLA_REG_OFS) =
#ifdef	__AVR_PGMSPACE__
  (EE_URGE) pgm_read_byte(&EE_Gpt_Div[Channel]);
#else	/* __AVR_PGMSPACE__ */
  EE_Gpt_Div[Channel];
#endif	/* __AVR_PGMSPACE__ */

  /* Resume IRQs */
  EE_hal_resumeIRQ(flags);

#ifdef	__GPT_EXTENDED_STATUS__
  return E_OK;
#endif

#endif	/* __USE_TC_HW__ */

}

#ifdef	__GPT_STOPTIMER_API__
#ifdef	__GPT_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_Gpt_StopTimer(
  EE_TYPEGPTCHANNEL	Channel
) {

#ifdef	__USE_TC_HW__

#ifdef	DEBUG
  volatile EE_UREG	flags;
  volatile EE_HWREG_PTR	addr;
#else
  register EE_UREG	flags;
  register EE_HWREG_PTR	addr;
#endif

#ifdef	__GPT_EXTENDED_STATUS__

  /* Channel Validation */
  EE_ATXMEGA_VALIDATE_W_RV( 
    ( Channel < EE_TC_HW_ALL_UNITS_MASK ),
    E_GPT_PARAM_CHANNEL
  );

#endif	/* __GPT_EXTENDED_STATUS__ */

  /* Suspend Interrupt */
  flags = EE_hal_suspendIRQ();

#ifdef	__GPT_EXTENDED_STATUS__

  EE_ATXMEGA_VALIDATE_IRQ_W_RV(
    ( EE_Tc_Hw_GetStatus(Channel) & EE_TC_HW_INIT_MASK ), E_NOT_OK, flags
  );

#endif	/* __GPT_EXTENDED_STATUS__ */

  addr = EE_TC_HW_2_ADDR(Channel);

  EE_HWREG(addr + EE_TC_HW_CTRLA_REG_OFS) = TC_CLKSEL_OFF_gc;

  /* Resume IRQs */
  EE_hal_resumeIRQ(flags);

#ifdef	__GPT_EXTENDED_STATUS__
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

#ifdef	__GPT_EXTENDED_STATUS__

  /* Channel Validation */
  EE_ATXMEGA_VALIDATE_W_RV( 
    ( Channel < EE_TC_HW_ALL_TCX0_MASK ),
    E_GPT_PARAM_CHANNEL
  );

#endif	/* __GPT_EXTENDED_STATUS__ */

  /* Suspend Interrupt */
  flags = EE_hal_suspendIRQ();

#ifdef	__GPT_EXTENDED_STATUS__

  EE_ATXMEGA_VALIDATE_IRQ_W_RV(
    ( EE_Tc_Hw_Status[Channel] & EE_TC_HW_INIT_MASK ),
    E_NOT_OK,
    flags
  );

#endif	/* __GPT_EXTENDED_STATUS__ */

  addr = EE_TC_HW_TCX0_2_ADDR(Channel);

  EE_HWREG(addr + EE_TC_HW_TCX0_CTRLA_REG_OFS) = TC_CLKSEL_OFF_gc;

  /* Resume IRQs */
  EE_hal_resumeIRQ(flags);

#ifdef	__GPT_EXTENDED_STATUS__
  return E_OK;
#endif

#endif	/* __USE_TC_HW__ */

}
#endif	/* __GPT_STOPTIMER_API__ */

#ifdef	__GPT_GETTIMEELAPSED_API__
#ifdef	__GPT_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_Gpt_GetTimeElapsed(
  EE_TYPEGPTCHANNEL	Channel,
  EE_TYPEGPTCOUNTER	*Counter
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

#ifdef	__GPT_EXTENDED_STATUS__

  /* Counter Validation */
  EE_ATXMEGA_VALIDATE_W_RV( 
    ( Counter != EE_NULL_PTR ),
    E_GPT_PARAM_COUNTER
  );

  (*Counter) = EE_OFF;

  /* Channel Validation */
  EE_ATXMEGA_VALIDATE_W_RV( 
    ( Channel < EE_TC_HW_ALL_UNITS_MASK ),
    E_GPT_PARAM_CHANNEL
  );

#endif	/* __GPT_EXTENDED_STATUS__ */

  /* Suspend Interrupt */
  flags = EE_hal_suspendIRQ();

  stat = EE_Tc_Hw_GetStatus(Channel);

#ifdef	__GPT_EXTENDED_STATUS__

  EE_ATXMEGA_VALIDATE_IRQ_W_RV(( stat & EE_TC_HW_INIT_MASK ), E_NOT_OK, flags);

#endif	/* __GPT_EXTENDED_STATUS__ */

  addr = EE_TC_HW_2_ADDR(Channel);

#ifdef	__GPT_ONESHOT__
  if ( ( stat & EE_GPT_ONESHOT_MASK ) && ( stat & EE_GPT_EXPIRED_MASK ) ) {
    (*Counter) = EE_ATxmega_Reg16Read( addr + EE_TC_HW_PER_REG_OFS );
  }
  else {
#endif	/* __GPT_ONESHOT__ */
    (*Counter) = EE_ATxmega_Reg16Read( addr + EE_TC_HW_CNT_REG_OFS );
#ifdef	__GPT_ONESHOT__
  }
#endif	/* __GPT_ONESHOT__ */

  /* Resume IRQs */
  EE_hal_resumeIRQ(flags);

#ifdef	__GPT_EXTENDED_STATUS__
  return E_OK;
#endif

#else	/* __USE_TC_HW__ */

#ifdef	DEBUG
  volatile EE_UREG	flags;
  volatile EE_UREG	stat;
  volatile EE_HWREG_PTR	addr;
#else
  register EE_UREG	flags;
  register EE_UREG	stat;
  register EE_HWREG_PTR	addr;
#endif

#ifdef	__GPT_EXTENDED_STATUS__

  /* Counter Validation */
  EE_ATXMEGA_VALIDATE_W_RV( 
    ( Counter != EE_NULL_PTR ),
    E_GPT_PARAM_COUNTER
  );

  (*Counter) = 0x0000U;

  /* Channel Validation */
  EE_ATXMEGA_VALIDATE_W_RV( 
    ( Channel < EE_TC_HW_ALL_TCX0_MASK ),
    E_GPT_PARAM_CHANNEL
  );

#endif	/* __GPT_EXTENDED_STATUS__ */

  /* Suspend Interrupt */
  flags = EE_hal_suspendIRQ();

  stat = EE_Tc_Hw_Status[Channel];

#ifdef	__GPT_EXTENDED_STATUS__

  EE_ATXMEGA_VALIDATE_IRQ_W_RV(
    ( stat & EE_TC_HW_INIT_MASK ),
    E_NOT_OK,
    flags
  );

#endif	/* __GPT_EXTENDED_STATUS__ */

  addr = EE_TC_HW_TCX0_2_ADDR(Channel);

#ifdef	__GPT_ONESHOT__
  if ( ( stat & EE_GPT_ONESHOT_MASK ) && ( stat & EE_GPT_EXPIRED_MASK ) ) {
    (*Counter) = EE_ATxmega_Reg16Read( addr + EE_TC_HW_TCX0_PER_REG_OFS );
  }
  else {
#endif	/* __GPT_ONESHOT__ */
    (*Counter) = EE_ATxmega_Reg16Read( addr + EE_TC_HW_TCX0_CNT_REG_OFS );
#ifdef	__GPT_ONESHOT__
  }
#endif	/* __GPT_ONESHOT__ */

  /* Resume IRQs */
  EE_hal_resumeIRQ(flags);

#ifdef	__GPT_EXTENDED_STATUS__
  return E_OK;
#endif

#endif	/* __USE_TC_HW__ */

}
#endif	/* __GPT_GETTIMEELAPSED_API__ */

#ifdef	__GPT_GETTIMEREMAINING_API__
#ifdef	__GPT_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_Gpt_GetTimeRemaining(
  EE_TYPEGPTCHANNEL	Channel,
  EE_TYPEGPTCOUNTER	*Counter
) {

#ifdef	__USE_TC_HW__

#ifdef	DEBUG
  volatile EE_UREG	flags;
  volatile EE_UREG	stat;
  volatile EE_UINT16	tmp;
  volatile EE_HWREG_PTR	addr;
#else
  register EE_UREG	flags;
  register EE_UREG	stat;
  register EE_UINT16	tmp;
  register EE_HWREG_PTR	addr;
#endif

#ifdef	__GPT_EXTENDED_STATUS__

  /* Channel Validation */
  EE_ATXMEGA_VALIDATE_W_RV( 
    ( Counter != EE_NULL_PTR ),
    E_GPT_PARAM_COUNTER
  );

  (*Counter) = EE_OFF;

  /* Channel Validation */
  EE_ATXMEGA_VALIDATE_W_RV( 
    ( Channel < EE_TC_HW_ALL_UNITS_MASK ),
    E_GPT_PARAM_CHANNEL
  );

#endif	/* __GPT_EXTENDED_STATUS__ */

  /* Suspend Interrupt */
  flags = EE_hal_suspendIRQ();

  stat = EE_Tc_Hw_GetStatus(Channel);

#ifdef	__GPT_EXTENDED_STATUS__

  EE_ATXMEGA_VALIDATE_IRQ_W_RV(
    ( stat & EE_TC_HW_INIT_MASK ),
    E_NOT_OK,
    flags
  );

#endif	/* __GPT_EXTENDED_STATUS__ */

  addr = EE_TC_HW_2_ADDR(Channel);

#ifdef	__GPT_ONESHOT__
  if ( ( stat & EE_GPT_ONESHOT_MASK ) && ( stat & EE_GPT_EXPIRED_MASK ) ) {
    tmp = EE_ATxmega_Reg16Read( addr + EE_TC_HW_PER_REG_OFS );
  }
  else {
#endif	/* __GPT_ONESHOT__ */
    tmp = EE_ATxmega_Reg16Read( addr + EE_TC_HW_CNT_REG_OFS );
#ifdef	__GPT_ONESHOT__
  }
#endif	/* __GPT_ONESHOT__ */

  (*Counter) = EE_ATxmega_Reg16Read( addr + EE_TC_HW_PER_REG_OFS ) - tmp;

  /* Resume IRQs */
  EE_hal_resumeIRQ(flags);

#ifdef	__GPT_EXTENDED_STATUS__
  return E_OK;
#endif

#else	/* __USE_TC_HW__ */

#ifdef	DEBUG
  volatile EE_UREG	flags;
  volatile EE_UREG	stat;
  volatile EE_UINT16	tmp;
  volatile EE_HWREG_PTR	addr;
#else
  register EE_UREG	flags;
  register EE_UREG	stat;
  register EE_UINT16	tmp;
  register EE_HWREG_PTR	addr;
#endif

#ifdef	__GPT_EXTENDED_STATUS__

  /* Channel Validation */
  EE_ATXMEGA_VALIDATE_W_RV( 
    ( Counter != EE_NULL_PTR ),
    E_GPT_PARAM_COUNTER
  );

  (*Counter) = 0x0000U;

  /* Channel Validation */
  EE_ATXMEGA_VALIDATE_W_RV( 
    ( Channel < EE_TC_HW_ALL_TCX0_MASK ),
    E_GPT_PARAM_CHANNEL
  );

#endif	/* __GPT_EXTENDED_STATUS__ */

  /* Suspend Interrupt */
  flags = EE_hal_suspendIRQ();

  stat = EE_Tc_Hw_Status[Channel];

#ifdef	__GPT_EXTENDED_STATUS__

  EE_ATXMEGA_VALIDATE_IRQ_W_RV(
    ( stat & EE_TC_HW_INIT_MASK ),
    E_NOT_OK,
    flags
  );

#endif	/* __GPT_EXTENDED_STATUS__ */

  addr = EE_TC_HW_TCX0_2_ADDR(Channel);

#ifdef	__GPT_ONESHOT__
  if ( ( stat & EE_GPT_ONESHOT_MASK ) && ( stat & EE_GPT_EXPIRED_MASK ) ) {
    tmp = EE_ATxmega_Reg16Read( addr + EE_TC_HW_TCX0_PER_REG_OFS );
  }
  else {
#endif	/* __GPT_ONESHOT__ */
    tmp = EE_ATxmega_Reg16Read( addr + EE_TC_HW_TCX0_CNT_REG_OFS );
#ifdef	__GPT_ONESHOT__
  }
#endif	/* __GPT_ONESHOT__ */

  (*Counter) = EE_ATxmega_Reg16Read( addr + EE_TC_HW_TCX0_PER_REG_OFS ) - tmp;

  /* Resume IRQs */
  EE_hal_resumeIRQ(flags);

#ifdef	__GPT_EXTENDED_STATUS__
  return E_OK;
#endif

#endif	/* __USE_TC_HW__ */

}
#endif	/* __GPT_GETTIMEREMAINING_API__ */
