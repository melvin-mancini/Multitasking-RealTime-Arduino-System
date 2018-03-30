/** @file	ee_atmel_atxmega_pmic.h
 *
 *  @brief	Erika Enterprise - PR Driver - Atmel ATXMEGA MCUs Header File.
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
 *  POwer Reduction Driver Header file.
 *
 *  @author	Giuseppe Serano
 *  @version	0.1
 *  @date	2013
 */

#ifndef	__INCLUDE_ATMEL_ATXMEGA_PR_H__
#define	__INCLUDE_ATMEL_ATXMEGA_PR_H__

#include <avr/io.h>

/** @brief	Power Reduction Initialization.
 *
 *  Disables all not used pheripherals.
 */
#ifdef	__MCU_POWER_REDUCTION__
__INLINE__ void __ALWAYS_INLINE__ EE_Pr_Init(void)
{

#ifdef	DEBUG
  volatile EE_UREG	pr;
#else
  register EE_UREG	pr;
#endif

  /* Event System Disabling */
  pr = PR_EVSYS_bm;

#if	( !( defined(__RTC_DRIVER__) && defined(__RTC_CHANNEL_0__) ) )

  /* RTC Disabling */
  pr |= PR_RTC_bm;

#endif	/* !( __RTC_DRIVER__ && __RTC_CHANNEL_0__ ) */

  /* General Power Reduction */
  PR.PRGEN = pr;

  pr = EE_OFF;

#if	( !( defined(__ADC_DRIVER__) ) )

  /* ADC Disabling */
  pr |= PR_ADC_bm;

#endif	/* !( __ADC_DRIVER__ ) */

#if	( !( defined(__AC_DRIVER__) ) )

  /* AC Disabling */
  pr |= PR_AC_bm;

#endif	/* !( __AC_DRIVER__ ) */

  /* Port A Power Reduction */
  PR.PRPA = pr;

  /* TODO: TWI */
  pr = PR_SPI_bm | PR_HIRES_bm;

#if	( !( defined(__SCI_DRIVER__) && defined(__SCI_CHANNEL_C_0__) ) )

  /* USART0 Disabling */
  pr |= PR_USART0_bm;

#endif	/* !( __SCI_DRIVER__ && __SCI_CHANNEL_C_0__ ) */

#if	( \
  !( defined(__GPT_DRIVER__) && defined(__GPT_CHANNEL_C_1__) ) && \
  !( defined(__PWM_DRIVER__) && defined(__PWM_CHANNEL_C_1__) ) \
)

  /* TC1 Disabling */
  pr |= PR_TC1_bm;

#endif	/*
	 * !( __GPT_DRIVER__ && __GPT_CHANNEL_C_1__ ) &&
	 * !( __PWM_DRIVER__ && __PWM_CHANNEL_C_1__ )
	 */

#if	( \
  !( defined(__GPT_DRIVER__) && defined(__GPT_CHANNEL_C_0__) ) && \
  !( defined(__PWM_DRIVER__) && defined(__PWM_CHANNEL_C_0__) ) \
)

  /* TC0 Disabling */
  pr |= PR_TC0_bm;

#endif	/*
	 * !( __GPT_DRIVER__ && __GPT_CHANNEL_C_0__ ) &&
	 * !( __PWM_DRIVER__ && __PWM_CHANNEL_C_0__ )
	 */

  /* Port C Power Reduction */
  PR.PRPC = pr;

  /* TODO: USART0 */
  pr = PR_SPI_bm;

#if	( !( defined(__SCI_DRIVER__) && defined(__SCI_CHANNEL_D_0__) ) )

  /* USART0 Disabling */
  pr |= PR_USART0_bm;

#endif	/* !( __SCI_DRIVER__ && __SCI_CHANNEL_D_0__ ) */

#if	( \
  !( defined(__GPT_DRIVER__) && defined(__GPT_CHANNEL_D_0__) ) && \
  !( defined(__PWM_DRIVER__) && defined(__PWM_CHANNEL_D_0__) ) \
)

  /* TC0 Disabling */
  pr |= PR_TC0_bm;

#endif	/*
	 * !( __GPT_DRIVER__ && __GPT_CHANNEL_D_0__ ) &&
	 * !( __PWM_DRIVER__ && __PWM_CHANNEL_D_0__ )
	 */

  /* Port D Power Reduction */
  PR.PRPD = pr;

  /* TODO: TWI AND USART0 */
  pr = EE_OFF;

#if	( \
  !( defined(__GPT_DRIVER__) && defined(__GPT_CHANNEL_E_0__) ) && \
  !( defined(__PWM_DRIVER__) && defined(__PWM_CHANNEL_E_0__) ) \
)

  /* TC0 Disabling */
  pr |= PR_TC0_bm;

#endif	/*
	 * !( __GPT_DRIVER__ && __GPT_CHANNEL_E_0__ ) &&
	 * !( __PWM_DRIVER__ && __PWM_CHANNEL_E_0__ )
	 */

  /* Port E Power Reduction */
  PR.PRPE = pr;

  /* ALL PORT PULL-UP/PULL-DOWN */
  PORTCFG.MPCMASK = 0xFFU;
#ifdef	__MCU_PR_PULL_UP__
  PORTA.PIN0CTRL = PORT_OPC_PULLUP_gc;
#else
  PORTA.PIN0CTRL = PORT_OPC_PULLDOWN_gc;
#endif
  PORTCFG.MPCMASK = 0xFFU;
#ifdef	__MCU_PR_PULL_UP__
  PORTB.PIN0CTRL = PORT_OPC_PULLUP_gc;
#else
  PORTB.PIN0CTRL = PORT_OPC_PULLDOWN_gc;
#endif
  PORTCFG.MPCMASK = 0xFFU;
#ifdef	__MCU_PR_PULL_UP__
  PORTC.PIN0CTRL = PORT_OPC_PULLUP_gc;
#else
  PORTC.PIN0CTRL = PORT_OPC_PULLDOWN_gc;
#endif
  PORTCFG.MPCMASK = 0xFFU;
#ifdef	__MCU_PR_PULL_UP__
  PORTD.PIN0CTRL = PORT_OPC_PULLUP_gc;
#else
  PORTD.PIN0CTRL = PORT_OPC_PULLDOWN_gc;
#endif
  PORTCFG.MPCMASK = 0xFFU;
#ifdef	__MCU_PR_PULL_UP__
  PORTE.PIN0CTRL = PORT_OPC_PULLUP_gc;
#else
  PORTE.PIN0CTRL = PORT_OPC_PULLDOWN_gc;
#endif

#ifdef	__MCU_PR_PORT_OUT__
  PORTA.DIRSET = 0xFFU;
  PORTA.OUTCLR = 0xFFU;
  PORTB.DIRSET = 0xFFU;
  PORTB.OUTCLR = 0xFFU;
  PORTC.DIRSET = 0xFFU;
  PORTC.OUTCLR = 0xFFU;
  PORTD.DIRSET = 0xFFU;
  PORTD.OUTCLR = 0xFFU;
  PORTE.DIRSET = 0xFFU;
  PORTE.OUTCLR = 0xFFU;
#endif	/* __MCU_PR_PORT_OUT__ */

}
#else	/* __MCU_POWER_REDUCTION__ */
#define	EE_Pr_Init()
#endif	/* __MCU_POWER_REDUCTION__ */

/** @brief	Power Reduction De-Initialization.
 *
 *  Re-Enables all not used pheripherals.
 */
#ifdef	__MCU_POWER_REDUCTION__
__INLINE__ void __ALWAYS_INLINE__ EE_Pr_DeInit(void)
{

  PR.PRGEN = EE_OFF;
  PR.PRPA = EE_OFF;
  PR.PRPC = EE_OFF;
  PR.PRPD = EE_OFF;
  PR.PRPE = EE_OFF;

  /* ALL PORT PUSH-PULL */
  PORTCFG.MPCMASK = 0xFFU ;
  PORTA.PIN0CTRL = PORT_OPC_TOTEM_gc;
  PORTCFG.MPCMASK = 0xFFU ;
  PORTB.PIN0CTRL = PORT_OPC_TOTEM_gc;
  PORTCFG.MPCMASK = 0xFFU ;
  PORTC.PIN0CTRL = PORT_OPC_TOTEM_gc;
  PORTCFG.MPCMASK = 0xFFU ;
  PORTD.PIN0CTRL = PORT_OPC_TOTEM_gc;
  PORTCFG.MPCMASK = 0xFFU ;
  PORTE.PIN0CTRL = PORT_OPC_TOTEM_gc;

}
#else	/* __MCU_POWER_REDUCTION__ */
#define	EE_Pr_DeInit()
#endif	/* __MCU_POWER_REDUCTION__ */

/** @brief	JTAG Disable.
 *
 *  Disables JTAG interface.
 */
#if	( defined(__MCU_POWER_REDUCTION__) && defined(__MCU_PR_JTAG__) )
__INLINE__ void __ALWAYS_INLINE__ EE_Pr_DisableJTAG(void)
{
  EE_avr8_WriteCCPReg(&MCU.MCUCR, MCU_JTAGD_bm);
}
#else	/* __MCU_POWER_REDUCTION__ && __MCU_PR_JTAG__ */
#define	EE_Pr_DisableJTAG()
#endif	/* __MCU_POWER_REDUCTION__ && __MCU_PR_JTAG__ */

/** @brief	JTAG Enable.
 *
 *  Enables JTAG interface.
 */
#if	( defined(__MCU_POWER_REDUCTION__) && defined(__MCU_PR_JTAG__) )
__INLINE__ void __ALWAYS_INLINE__ EE_Pr_EnableJTAG(void)
{
  EE_avr8_WriteCCPReg(&MCU.MCUCR, EE_OFF);
}
#else	/* __MCU_POWER_REDUCTION__ && __MCU_PR_JTAG__ */
#define	EE_Pr_EnableJTAG()
#endif	/* __MCU_POWER_REDUCTION__ && __MCU_PR_JTAG__ */

#endif	/* __INCLUDE_ATMEL_ATXMEGA_PR_H__ */
