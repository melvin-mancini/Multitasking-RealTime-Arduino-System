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
 * Erika Enterprise - GPIO Driver - Atmel ATXMEGA GPIO Driver Implementation.
 *
 * Author: 2013,  Giuseppe Serano
 */

/* ERIKA Enterprise Internals */
#include "ee_internal.h"

/* ERIKA Enterprise IRQs */
#include "ee_irq.h"

#ifdef	__AVR_PGMSPACE__
#include <avr/pgmspace.h>
#endif	/* __AVR8_PGMSPACE__ */

#define	EE_GPIO_MAX_PINS_NUMBER		0x08U

#if	( defined(__AVR_ATxmega16D4__) || defined(__AVR_ATxmega32D4__) )
#define	EE_GPIO_MAX_PORTS_NUMBER	EE_GPIO_PORT_E + 1
#else	/* __AVR_ATxmega16D4__ || __AVR_ATxmega32D4__ */
#define	EE_GPIO_MAX_PORTS_NUMBER	EE_GPIO_PORT_R + 1
#endif	/* __AVR_ATxmega16D4__ || __AVR_ATxmega32D4__ */

#define	EE_GPIO_FLAG_PULL_MASK	( \
	EE_GPIO_FLAG_BUS_KEEPER	| \
	EE_GPIO_FLAG_PULL_DOWN	| \
	EE_GPIO_FLAG_PULL_UP	 \
)
#ifdef	__GPIO_ENABLENOTIF_API__
#define	EE_GPIO_FLAG_EDGE_MASK	EE_GPIO_FLAG_BOTH_EDGE
#define	EE_GPIO_FLAG_ALL_MASK	( \
	EE_GPIO_FLAG_DIR_OUT	| \
	EE_GPIO_FLAG_LVL_HIGH	| \
	EE_GPIO_FLAG_PULL_MASK	| \
	EE_GPIO_FLAG_EDGE_MASK	  \
)
#else	/* __GPIO_ENABLENOTIF_API__ */
#define	EE_GPIO_FLAG_ALL_MASK	( \
	EE_GPIO_FLAG_DIR_OUT	| \
	EE_GPIO_FLAG_LVL_HIGH	| \
	EE_GPIO_FLAG_PULL_MASK	  \
)
#endif	/* __GPIO_ENABLENOTIF_API__ */

#define	EE_GPIO_BASE_PIN_MASK		0x01U
#define	EE_GPIO_ALL_PIN_MASK		0x07U
#define	EE_GPIO_ALL_PORT_MASK		0xF0U
#define	EE_GPIO_PORT_SH_BITS		0x04U

#define	EE_GPIO_ALL_INT_MASK		0x03U

#define	EE_GPIO_FLAGS_EDGES_SH_BITS	0x04U
#define	EE_GPIO_FLAGS_PULL_SH_BITS	0x01U

#define	EE_GPIO_CH_2_PIN(_ch)		( (_ch) & EE_GPIO_ALL_PIN_MASK )

#define	EE_GPIO_PIN_2_PIN_MASK(_pin)	( EE_GPIO_BASE_PIN_MASK << (_pin) )

#define	EE_GPIO_CH_2_PORT(_ch)		( (_ch) >> EE_GPIO_PORT_SH_BITS )

#define	EE_GPIO_PORTA_BASE_ADDR		0x0600U	/* PORT A Base Address */
#define	EE_GPIO_PORTB_BASE_ADDR		0x0620U	/* PORT B Base Address */
#define	EE_GPIO_PORTC_BASE_ADDR		0x0640U	/* PORT C Base Address */
#define	EE_GPIO_PORTD_BASE_ADDR		0x0660U	/* PORT D Base Address */
#define	EE_GPIO_PORTE_BASE_ADDR		0x0680U	/* PORT E Base Address */

#if	( defined(__AVR_ATxmega16D4__) || defined(__AVR_ATxmega32D4__) )
/* 
 * NOTE:	Ports from F to Q are not available on ATxmega16D4 MCU
 * 		Port R is used for XTAL or TOSC
 */
#else	/* __AVR_ATxmega16D4__ || __AVR_ATxmega32D4__ */
#define	EE_GPIO_PORTR_BASE_ADDR		0x07E0U	/* PORT R Base Address */
#endif	/* __AVR_ATxmega16D4__ || __AVR_ATxmega32D4__ */

#define	EE_GPIO_PORT_BASE_ADDR		0x0600U	/* PORT Base Address */

#define	EE_GPIO_PORT_OFS		0x0020U	/* PORT Offset. */
#define	EE_GPIO_PORT_OFS_SH_BITS	0x0005U	/* PORT Offset Shift Bits. */

#define	EE_GPIO_PORT_2_ADDR(_port)	(				\
  EE_GPIO_PORT_BASE_ADDR + ( (_port) << EE_GPIO_PORT_OFS_SH_BITS )	\
)

/*
 * GPIO PORT Register Offsets.
 */
#define	EE_GPIO_PORT_DIR_REG_OFS	0x00U	/* Data Direction	      */
#define	EE_GPIO_PORT_DIRSET_REG_OFS	0x01U	/* Data Direction Set	      */
#define	EE_GPIO_PORT_DIRCLR_REG_OFS	0x02U	/* Data Direction Clear	      */
#define	EE_GPIO_PORT_DIRTGL_REG_OFS	0x03U	/* Data Direction Toggle      */
#define	EE_GPIO_PORT_OUT_REG_OFS	0x04U	/* Data Output Value	      */
#define	EE_GPIO_PORT_OUTSET_REG_OFS	0x05U	/* Data Output Value Set      */
#define	EE_GPIO_PORT_OUTCLR_REG_OFS	0x06U	/* Data Output Value Clear    */
#define	EE_GPIO_PORT_OUTTGL_REG_OFS	0x07U	/* Data Output Value Toggle   */
#define	EE_GPIO_PORT_IN_REG_OFS		0x08U	/* Data Input Value	      */
#define	EE_GPIO_PORT_INTCTRL_REG_OFS	0x09U	/* Interrupt Control	      */
#define	EE_GPIO_PORT_INT0MASK_REG_OFS	0x0AU	/* Interrupt 0 Mask	      */
#define	EE_GPIO_PORT_INT1MASK_REG_OFS	0x0BU	/* Interrupt 1 Mask	      */
#define	EE_GPIO_PORT_INTFLAGS_REG_OFS	0x0CU	/* Interrupt Flags	      */
#define	EE_GPIO_PORT_PIN0CTRL_REG_OFS	0x10U	/* Pin 0 Configuration	      */
#define	EE_GPIO_PORT_PIN1CTRL_REG_OFS	0x11U	/* Pin 1 Configuration	      */
#define	EE_GPIO_PORT_PIN2CTRL_REG_OFS	0x12U	/* Pin 2 Configuration	      */
#define	EE_GPIO_PORT_PIN3CTRL_REG_OFS	0x13U	/* Pin 3 Configuration	      */
#define	EE_GPIO_PORT_PIN4CTRL_REG_OFS	0x14U	/* Pin 4 Configuration	      */
#define	EE_GPIO_PORT_PIN5CTRL_REG_OFS	0x15U	/* Pin 5 Configuration	      */
#define	EE_GPIO_PORT_PIN6CTRL_REG_OFS	0x16U	/* Pin 6 Configuration	      */
#define	EE_GPIO_PORT_PIN7CTRL_REG_OFS	0x17U	/* Pin 7 Configuration	      */

#ifdef	__GPIO_EXTENDED_STATUS__
/*
 * GPIO Ports Pins Status.
 */
#ifdef	DEBUG
static volatile EE_UREG
#else
static EE_UREG
#endif
#if	0
EE_Gpio_Status[EE_GPIO_MAX_PORTS_NUMBER] = { 0 };
#else	/* 0 */
EE_Gpio_Status[EE_GPIO_MAX_PORTS_NUMBER];
#endif	/* 0 */
#endif	/* __GPIO_EXTENDED_STATUS__ */

#ifdef	__GPIO_ENABLENOTIF_API__
#if	( \
  defined( EE_AVR8_PORTA_INT0_ISR ) || defined( EE_AVR8_PORTB_INT0_ISR ) || \
  defined( EE_AVR8_PORTC_INT0_ISR ) || defined( EE_AVR8_PORTD_INT0_ISR ) || \
  defined( EE_AVR8_PORTE_INT0_ISR ) \
)

/*
 * GPIO Port A Pins Callback Array.
 */
#ifdef	EE_AVR8_PORTA_INT0_ISR
#ifdef	DEBUG
static volatile EE_ISR_callback
#else
static EE_ISR_callback
#endif
#if	0
EE_Gpio_PortA_CallBack[EE_GPIO_MAX_PINS_NUMBER] = { 
  EE_NULL_PTR, EE_NULL_PTR, EE_NULL_PTR, EE_NULL_PTR,
  EE_NULL_PTR, EE_NULL_PTR, EE_NULL_PTR, EE_NULL_PTR
};
#else	/* 0 */
EE_Gpio_PortA_CallBack[EE_GPIO_MAX_PINS_NUMBER];
#endif	/* 0 */
#endif	/* EE_AVR8_PORTA_INT0_ISR */

/*
 * GPIO Port B Pins Callback Array.
 */
#ifdef	EE_AVR8_PORTB_INT0_ISR
#ifdef	DEBUG
static volatile EE_ISR_callback
#else
static EE_ISR_callback
#endif
#if	0
EE_Gpio_PortB_CallBack[EE_GPIO_MAX_PINS_NUMBER] = { 
  EE_NULL_PTR, EE_NULL_PTR, EE_NULL_PTR, EE_NULL_PTR,
  EE_NULL_PTR, EE_NULL_PTR, EE_NULL_PTR, EE_NULL_PTR
};
#else	/* 0 */
EE_Gpio_PortB_CallBack[EE_GPIO_MAX_PINS_NUMBER];
#endif	/* 0 */
#endif	/* EE_AVR8_PORTB_INT0_ISR */

/*
 * GPIO Port C Pins Callback Array.
 */
#ifdef	EE_AVR8_PORTC_INT0_ISR
#ifdef	DEBUG
static volatile EE_ISR_callback
#else
static EE_ISR_callback
#endif
#if	0
EE_Gpio_PortC_CallBack[EE_GPIO_MAX_PINS_NUMBER] = { 
  EE_NULL_PTR, EE_NULL_PTR, EE_NULL_PTR, EE_NULL_PTR,
  EE_NULL_PTR, EE_NULL_PTR, EE_NULL_PTR, EE_NULL_PTR
};
#else	/* 0 */
EE_Gpio_PortC_CallBack[EE_GPIO_MAX_PINS_NUMBER];
#endif	/* 0 */
#endif	/* EE_AVR8_PORTC_INT0_ISR */

/*
 * GPIO Port D Pins Callback Array.
 */
#ifdef	EE_AVR8_PORTD_INT0_ISR
#ifdef	DEBUG
static volatile EE_ISR_callback
#else
static EE_ISR_callback
#endif
#if	0
EE_Gpio_PortD_CallBack[EE_GPIO_MAX_PINS_NUMBER] = { 
  EE_NULL_PTR, EE_NULL_PTR, EE_NULL_PTR, EE_NULL_PTR,
  EE_NULL_PTR, EE_NULL_PTR, EE_NULL_PTR, EE_NULL_PTR
};
#else	/* 0 */
EE_Gpio_PortD_CallBack[EE_GPIO_MAX_PINS_NUMBER];
#endif	/* 0 */
#endif	/* EE_AVR8_PORTD_INT0_ISR */

/*
 * GPIO Port E Pins Callback Array.
 */
#ifdef	EE_AVR8_PORTE_INT0_ISR
#ifdef	DEBUG
static volatile EE_ISR_callback
#else
static EE_ISR_callback
#endif
#if	0
EE_Gpio_PortE_CallBack[EE_GPIO_MAX_PINS_NUMBER] = { 
  EE_NULL_PTR, EE_NULL_PTR, EE_NULL_PTR, EE_NULL_PTR,
  EE_NULL_PTR, EE_NULL_PTR, EE_NULL_PTR, EE_NULL_PTR
};
#else	/* 0 */
EE_Gpio_PortE_CallBack[EE_GPIO_MAX_PINS_NUMBER];
#endif	/* 0 */
#endif	/* EE_AVR8_PORTE_INT0_ISR */

/*
 * GPIO Ports Callback Array.
 */
#ifdef	DEBUG
static volatile EE_ISR_callback *
#else
static EE_ISR_callback *
#endif
EE_Gpio_Port_CallBack[EE_GPIO_MAX_PORTS_NUMBER] = {
#ifdef	EE_AVR8_PORTA_INT0_ISR
	&EE_Gpio_PortA_CallBack,
#else	/* EE_AVR8_PORTA_INT0_ISR */
	EE_NULL_PTR,
#endif	/* EE_AVR8_PORTA_INT0_ISR */
#ifdef	EE_AVR8_PORTB_INT0_ISR
	&EE_Gpio_PortB_CallBack,
#else	/* EE_AVR8_PORTB_INT0_ISR */
	EE_NULL_PTR,
#endif	/* EE_AVR8_PORTB_INT0_ISR */
#ifdef	EE_AVR8_PORTC_INT0_ISR
	&EE_Gpio_PortC_CallBack,
#else	/* EE_AVR8_PORTC_INT0_ISR */
	EE_NULL_PTR,
#endif	/* EE_AVR8_PORTC_INT0_ISR */
#ifdef	EE_AVR8_PORTD_INT0_ISR
	&EE_Gpio_PortD_CallBack,
#else	/* EE_AVR8_PORTD_INT0_ISR */
	EE_NULL_PTR,
#endif	/* EE_AVR8_PORTD_INT0_ISR */
#ifdef	EE_AVR8_PORTE_INT0_ISR
	&EE_Gpio_PortE_CallBack,
#else	/* EE_AVR8_PORTE_INT0_ISR */
	EE_NULL_PTR,
#endif	/* EE_AVR8_PORTE_INT0_ISR */
};

/*
 * GPIO Ports IRQ(s) Priority Array.
 */
#ifdef	DEBUG
static volatile const EE_TYPEPRIO
#else
static const EE_TYPEPRIO
#endif
#ifdef	__AVR_PGMSPACE__
EE_Gpio_Port_Pri[EE_GPIO_MAX_PORTS_NUMBER] PROGMEM = {
#else	/* __AVR_PGMSPACE__ */
EE_Gpio_Port_Pri[EE_GPIO_MAX_PORTS_NUMBER] = {
#endif	/* __AVR_PGMSPACE__ */

#ifdef	EE_AVR8_PORTA_INT0_ISR_PRI
EE_AVR8_PORTA_INT0_ISR_PRI,
#else
EE_ISR_PRI_OFF,
#endif
#ifdef	EE_AVR8_PORTB_INT0_ISR_PRI
EE_AVR8_PORTB_INT0_ISR_PRI,
#else
EE_ISR_PRI_OFF,
#endif
#ifdef	EE_AVR8_PORTC_INT0_ISR_PRI
EE_AVR8_PORTC_INT0_ISR_PRI,
#else
EE_ISR_PRI_OFF,
#endif
#ifdef	EE_AVR8_PORTD_INT0_ISR_PRI
EE_AVR8_PORTD_INT0_ISR_PRI,
#else
EE_ISR_PRI_OFF,
#endif
#ifdef	EE_AVR8_PORTE_INT0_ISR_PRI
EE_AVR8_PORTE_INT0_ISR_PRI,
#else
EE_ISR_PRI_OFF,
#endif
};

/* GPIO Generic Port ISR
 *
 * param	Port	PORT Identifier.
 */
static void
EE_Gpio_ISR( EE_UREG Port )
{

#ifdef	DEBUG
  volatile EE_UREG		flags;
  volatile EE_UREG		pin;
  volatile EE_UREG		pin_pad;
  volatile EE_UREG		pin_mask;
  volatile EE_UREG		pin_imask;
  volatile EE_UREG		pin_isc;
  volatile EE_HWREG_PTR		port_addr;
  volatile EE_ISR_callback	pin_cb;
#else
  register EE_UREG		flags;
  register EE_UREG		pin;
  register EE_UREG		pin_pad;
  register EE_UREG		pin_mask;
  register EE_UREG		pin_imask;
  register EE_UREG		pin_isc;
  register EE_HWREG_PTR		port_addr;
  register EE_ISR_callback	pin_cb;
#endif

  /* Suspend Interrupt */
  flags = EE_hal_suspendIRQ();

  port_addr = EE_HWREG_ADDR( EE_GPIO_PORT_2_ADDR( Port ) );
  pin_pad = EE_HWREG(port_addr + EE_GPIO_PORT_IN_REG_OFS);
  pin_imask = EE_HWREG(port_addr + EE_GPIO_PORT_INT0MASK_REG_OFS);

  for (
    pin = 0x00U, pin_mask = EE_GPIO_BASE_PIN_MASK;
    pin < EE_GPIO_MAX_PINS_NUMBER;
    pin++, pin_mask <<= 0x01U
  ) {

    if ( pin_imask & pin_mask ) {

      if ( pin_pad & pin_mask ) {

	pin_isc = PORT_ISC_RISING_gc;

      }
      else {

	pin_isc = PORT_ISC_FALLING_gc;

      }

      switch ( 
	EE_HWREG(port_addr + EE_GPIO_PORT_PIN0CTRL_REG_OFS + pin) &
	PORT_ISC_INPUT_DISABLE_gc
      ) {

	case PORT_ISC_BOTHEDGES_gc:
	  break;

	case PORT_ISC_RISING_gc:
	  if ( pin_isc == PORT_ISC_FALLING_gc) {
	    pin_isc = PORT_ISC_INPUT_DISABLE_gc;
	  }
	  break;

	case PORT_ISC_FALLING_gc:
	  if ( pin_isc == PORT_ISC_RISING_gc) {
	    pin_isc = PORT_ISC_INPUT_DISABLE_gc;
	  }
	  break;

	default:
	  pin_isc = PORT_ISC_INPUT_DISABLE_gc;
	  break;

      }

      pin_cb = EE_Gpio_Port_CallBack[Port][pin];

      if (
	( pin_isc != PORT_ISC_INPUT_DISABLE_gc ) && ( pin_cb != EE_NULL_PTR )
      ) {

	/* Resume Interrupt */
	EE_hal_resumeIRQ(flags);

	/* Call Port Pin CallBack. */
	(*pin_cb)();

	/* Suspend Interrupt */
	flags = EE_hal_suspendIRQ();

      }	/* 
	 * if (
	 *  ( pin_isc != PORT_ISC_INPUT_DISABLE_gc ) &&
	 *  ( pin_cbs[pin] != EE_NULL_PTR )
	 * )
	 */

    }	/* if ( pin_imask & pin_mask ) */

  }	/* 
  	 * for (
  	 *   pin = 0x00U, pin_mask = 0x01U;
  	 *   pin < EE_GPIO_MAX_PINS_NUMBER;
  	 *   pin++, pin_mask <<= 0x01
  	 * )
  	 */

  /* Resume Interrupt */
  EE_hal_resumeIRQ(flags);

}	/* EE_Gpio_ISR( EE_HWREG_PTR port_addr, EE_ISR_callback* pin_cbs ) */

#ifndef	__GPIO_PORTA_ISR_RAW__
#ifdef	EE_AVR8_PORTA_INT0_ISR
#if	( EE_AVR8_PORTA_INT0_ISR_CAT == 1 )
ISR1(EE_AVR8_PORTA_INT0_ISR)
#else
ISR2(EE_AVR8_PORTA_INT0_ISR)
#endif
{
  EE_Gpio_ISR( EE_GPIO_PORT_A );
}
#endif	/* EE_AVR8_PORTA_INT0_ISR */
#endif	/* __GPIO_PORTA_ISR_RAW__ */

#ifndef	__GPIO_PORTB_ISR_RAW__
#ifdef	EE_AVR8_PORTB_INT0_ISR
#if	( EE_AVR8_PORTB_INT0_ISR_CAT == 1 )
ISR1(EE_AVR8_PORTB_INT0_ISR)
#else
ISR2(EE_AVR8_PORTB_INT0_ISR)
#endif
{
  EE_Gpio_ISR( EE_GPIO_PORT_B );
}
#endif	/* EE_AVR8_PORTB_INT0_ISR */
#endif	/* __GPIO_PORTB_ISR_RAW__ */

#ifndef	__GPIO_PORTC_ISR_RAW__
#ifdef	EE_AVR8_PORTC_INT0_ISR
#if	( EE_AVR8_PORTC_INT0_ISR_CAT == 1 )
ISR1(EE_AVR8_PORTC_INT0_ISR)
#else
ISR2(EE_AVR8_PORTC_INT0_ISR)
#endif
{
  EE_Gpio_ISR( EE_GPIO_PORT_C );
}
#endif	/* EE_AVR8_PORTC_INT0_ISR */
#endif	/* __GPIO_PORTC_ISR_RAW__ */

#ifndef	__GPIO_PORTD_ISR_RAW__
#ifdef	EE_AVR8_PORTD_INT0_ISR
#if	( EE_AVR8_PORTD_INT0_ISR_CAT == 1 )
ISR1(EE_AVR8_PORTD_INT0_ISR)
#else
ISR2(EE_AVR8_PORTD_INT0_ISR)
#endif
{
  EE_Gpio_ISR( EE_GPIO_PORT_D );
}
#endif	/* EE_AVR8_PORTD_INT0_ISR */
#endif	/* __GPIO_PORTD_ISR_RAW__ */

#ifndef	__GPIO_PORTE_ISR_RAW__
#ifdef	EE_AVR8_PORTE_INT0_ISR
#if	( EE_AVR8_PORTE_INT0_ISR_CAT == 1 )
ISR1(EE_AVR8_PORTE_INT0_ISR)
#else
ISR2(EE_AVR8_PORTE_INT0_ISR)
#endif
{
  EE_Gpio_ISR( EE_GPIO_PORT_E );
}
#endif	/* EE_AVR8_PORTE_INT0_ISR */
#endif	/* __GPIO_PORTE_ISR_RAW__ */

/* NOTE:	Ports from F to Q are not available on ATxmega16D4 MCU */

#ifndef	__GPIO_PORTR_ISR_RAW__
#ifdef	EE_AVR8_PORTR_INT0_ISR
#if	( EE_AVR8_PORTR_INT0_ISR_CAT == 1 )
ISR1(EE_AVR8_PORTR_INT0_ISR)
#else
ISR2(EE_AVR8_PORTR_INT0_ISR)
#endif
{
  EE_Gpio_ISR( EE_GPIO_PORT_R );
}
#endif	/* EE_AVR8_PORTR_INT0_ISR */
#endif	/* __GPIO_PORTR_ISR_RAW__ */

#endif	/*
	 * EE_AVR8_PORTA_INT0_ISR || EE_AVR8_PORTB_INT0_ISR ||
	 * EE_AVR8_PORTC_INT0_ISR || EE_AVR8_PORTD_INT0_ISR ||
	 * EE_AVR8_PORTE_INT0_ISR ||
	 * NOTE: Ports from F to Q are not available on ATxmega16D4 MCU 
	 * EE_AVR8_PORTR_INT0_ISR
	 */
#endif	/* __GPIO_ENABLENOTIF_API__ */

/*
 * EE_Gpio_Init() Implementation.
 */
#ifdef	__GPIO_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_Gpio_Init(
  EE_TYPEGPIOCHANNEL	Channel,
  EE_TYPEGPIOFLAGS	Flags,
  EE_ISR_callback	NotifCB
) {

#ifdef	DEBUG
  volatile EE_UREG	flags;
  volatile EE_UREG	pin		= EE_GPIO_CH_2_PIN(Channel);
  volatile EE_UREG	pin_mask	= EE_GPIO_PIN_2_PIN_MASK(pin);
  volatile EE_UREG	pin_ctrl	= EE_GPIO_FLAG_DEFAULT;
  volatile EE_UREG	port		= EE_GPIO_CH_2_PORT(Channel);
  volatile EE_HWREG_PTR	port_addr 	= EE_HWREG_ADDR(
  						EE_GPIO_PORT_2_ADDR( port )
  					  );
#else	/* DEBUG */
  register EE_UREG	flags;
  register EE_UREG	pin		= EE_GPIO_CH_2_PIN(Channel);
  register EE_UREG	pin_mask	= EE_GPIO_PIN_2_PIN_MASK(pin);
  register EE_UREG	pin_ctrl	= EE_GPIO_FLAG_DEFAULT;
  register EE_UREG	port		= EE_GPIO_CH_2_PORT(Channel);
  register EE_HWREG_PTR	port_addr 	= EE_HWREG_ADDR(
  						EE_GPIO_PORT_2_ADDR( port )
  					  );
#endif	/* DEBUG */

#ifdef	__GPIO_EXTENDED_STATUS__

  /* Port Pin Validation. */
  EE_ATXMEGA_VALIDATE_W_RV(
    ( (Channel & ~EE_GPIO_ALL_PORT_MASK) <= EE_GPIO_ALL_PIN_MASK ),
    E_GPIO_PARAM_CHANNEL
  );

  /* Port Validation. */
  EE_ATXMEGA_VALIDATE_W_RV(
    ( port < EE_GPIO_MAX_PORTS_NUMBER ),
    E_GPIO_PARAM_CHANNEL
  );

#if	( defined(__AVR_ATxmega16D4__) || defined(__AVR_ATxmega32D4__) )
  /* NOTE: Ports from F to Q are not available on ATxmega16D4 MCU */
  /* ( ( port <= EE_GPIO_PORT_E) || ( port == EE_GPIO_PORT_R) ), */
  EE_ATXMEGA_VALIDATE_W_RV( ( port <= EE_GPIO_PORT_E ), E_GPIO_PARAM_CHANNEL );
#endif	/* __AVR_ATxmega16D4__ || __AVR_ATxmega32D4__ */

#endif	/* __GPIO_EXTENDED_STATUS__ */

  /* Suspend IRQs */
  flags = EE_hal_suspendIRQ();

#ifdef	__GPIO_EXTENDED_STATUS__

  EE_ATXMEGA_VALIDATE_IRQ_W_RV(
    !(EE_Gpio_Status[port] & pin_mask),
    E_NOT_OK,
    flags
  );

  EE_Gpio_Status[port] |= pin_mask;

#endif	/* __GPIO_EXTENDED_STATUS__ */

#ifdef	__GPIO_ENABLENOTIF_API__
#if	( \
  defined( EE_AVR8_PORTA_INT0_ISR ) || defined( EE_AVR8_PORTB_INT0_ISR ) || \
  defined( EE_AVR8_PORTC_INT0_ISR ) || defined( EE_AVR8_PORTD_INT0_ISR ) || \
  defined( EE_AVR8_PORTE_INT0_ISR ) \
)
  EE_Gpio_Port_CallBack[port][pin] = NotifCB;

  if ( 
    !( Flags & EE_GPIO_FLAG_RISE_EDGE ) || !( Flags & EE_GPIO_FLAG_FALL_EDGE )
  ) {

    pin_ctrl = 
	( Flags & EE_GPIO_FLAG_BOTH_EDGE ) >> EE_GPIO_FLAGS_EDGES_SH_BITS;

  }

  EE_HWREG(port_addr + EE_GPIO_PORT_INTFLAGS_REG_OFS) = EE_GPIO_ALL_INT_MASK;
#endif	/*
	 * EE_AVR8_PORTA_INT0_ISR || EE_AVR8_PORTB_INT0_ISR ||
	 * EE_AVR8_PORTC_INT0_ISR || EE_AVR8_PORTD_INT0_ISR ||
	 * EE_AVR8_PORTE_INT0_ISR
	 */
#endif	/* __GPIO_ENABLENOTIF_API__ */

  pin_ctrl |= ( Flags & EE_GPIO_FLAG_PULL_MASK ) << EE_GPIO_FLAGS_PULL_SH_BITS;

  EE_HWREG(port_addr + EE_GPIO_PORT_PIN0CTRL_REG_OFS + pin) = pin_ctrl;

  /* Set direction and initial pin level */
  if (Flags & EE_GPIO_FLAG_DIR_OUT) {

    EE_HWREG(port_addr + EE_GPIO_PORT_DIRSET_REG_OFS) = pin_mask;

#ifdef	__GPIO_EXTENDED_STATUS__
    if (Flags & EE_GPIO_FLAG_RTC) {

      if (Channel == EE_GPIO_CHANNEL_C_6) {

	PORTCFG.CLKEVOUT |= 0x40U;

      }

    }
    else if (Flags & EE_GPIO_FLAG_CLK)  {

      switch (Channel) {

	case EE_GPIO_CHANNEL_C_7:
	  PORTCFG.CLKEVOUT |= 0x01U;
	  break;

	case EE_GPIO_CHANNEL_D_7:
	  PORTCFG.CLKEVOUT |= 0x02U;
	  break;

	default:
	  break;

      }

    }
#endif	/* __GPIO_EXTENDED_STATUS__ */

    if (Flags & EE_GPIO_FLAG_LVL_HIGH) {

      EE_HWREG(port_addr + EE_GPIO_PORT_OUTSET_REG_OFS) = pin_mask;

    }
    else {

      EE_HWREG(port_addr + EE_GPIO_PORT_OUTCLR_REG_OFS) = pin_mask;

    }

  }
  else {

    EE_HWREG(port_addr + EE_GPIO_PORT_DIRCLR_REG_OFS) = pin_mask;

  }

  /* Resume IRQs */
  EE_hal_resumeIRQ(flags);

#ifdef	__GPIO_EXTENDED_STATUS__
  return E_OK;
#endif	/* __GPIO_EXTENDED_STATUS__ */

}

#ifdef	__GPIO_DEINIT_API__
/*
 * EE_Gpio_DeInit() Implementation.
 */
#ifdef	__GPIO_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_Gpio_DeInit(
  EE_TYPEGPIOCHANNEL	Channel,
  EE_TYPEGPIOFLAGS	Flags
) {

#ifdef	DEBUG
  volatile EE_UREG	flags;
  volatile EE_UREG	pin		= EE_GPIO_CH_2_PIN(Channel);
  volatile EE_UREG	pin_mask	= EE_GPIO_PIN_2_PIN_MASK(pin);
  volatile EE_UREG	port		= EE_GPIO_CH_2_PORT(Channel);
  volatile EE_HWREG_PTR	port_addr 	= EE_HWREG_ADDR(
  						EE_GPIO_PORT_2_ADDR( port )
  					  );
#else	/* DEBUG */
  register EE_UREG	flags;
  register EE_UREG	pin		= EE_GPIO_CH_2_PIN(Channel);
  register EE_UREG	pin_mask	= EE_GPIO_PIN_2_PIN_MASK(pin);
  register EE_UREG	port		= EE_GPIO_CH_2_PORT(Channel);
  register EE_HWREG_PTR	port_addr 	= EE_HWREG_ADDR(
  						EE_GPIO_PORT_2_ADDR( port )
  					  );
#endif	/* DEBUG */

#ifdef	__GPIO_EXTENDED_STATUS__

  /* Port Pin Validation. */
  EE_ATXMEGA_VALIDATE_W_RV(
    ( (Channel & ~EE_GPIO_ALL_PORT_MASK) <= EE_GPIO_ALL_PIN_MASK ),
    E_GPIO_PARAM_CHANNEL
  );

  /* Port Validation. */
  EE_ATXMEGA_VALIDATE_W_RV(
    ( port < EE_GPIO_MAX_PORTS_NUMBER ),
    E_GPIO_PARAM_CHANNEL
  );

#if	( defined(__AVR_ATxmega16D4__) || defined(__AVR_ATxmega32D4__) )
  /* NOTE: Ports from F to Q are not available on ATxmega16D4 MCU */
  EE_ATXMEGA_VALIDATE_W_RV(
    ( ( port <= EE_GPIO_PORT_E) || ( port == EE_GPIO_PORT_R) ),
    E_GPIO_PARAM_CHANNEL
  );
#endif	/* __AVR_ATxmega16D4__ || __AVR_ATxmega32D4__ */

  /* Flags Validation. */
  EE_ATXMEGA_VALIDATE_W_RV(
    ( Flags == EE_GPIO_FLAG_DEFAULT ),
    E_GPIO_PARAM_FLAGS
  );

#endif	/* __GPIO_EXTENDED_STATUS__ */

  /* Suspend IRQs */
  flags = EE_hal_suspendIRQ();

#ifdef	__GPIO_EXTENDED_STATUS__

  EE_ATXMEGA_VALIDATE_IRQ_W_RV(
    ( EE_Gpio_Status[port] & pin_mask ),
    E_NOT_OK,
    flags
  );

  EE_Gpio_Status[port] &= ~pin_mask;

#endif	/* __GPIO_EXTENDED_STATUS__ */

  EE_HWREG(port_addr + EE_GPIO_PORT_PIN0CTRL_REG_OFS + pin) = 0x00U;
  EE_HWREG(port_addr + EE_GPIO_PORT_DIRSET_REG_OFS) = pin_mask;
  EE_HWREG(port_addr + EE_GPIO_PORT_OUTCLR_REG_OFS) = pin_mask;

#ifdef	__GPIO_ENABLENOTIF_API__
#if	( \
  defined( EE_AVR8_PORTA_INT0_ISR ) || defined( EE_AVR8_PORTB_INT0_ISR ) || \
  defined( EE_AVR8_PORTC_INT0_ISR ) || defined( EE_AVR8_PORTD_INT0_ISR ) || \
  defined( EE_AVR8_PORTE_INT0_ISR ) || \
  /* NOTE: Ports from F to Q are not available on ATxmega16D4 MCU */ \
  defined( EE_AVR8_PORTR_INT0_ISR ) \
)
  EE_HWREG(port_addr + EE_GPIO_PORT_INTFLAGS_REG_OFS) = EE_GPIO_ALL_INT_MASK;

  EE_HWREG(port_addr + EE_GPIO_PORT_INT0MASK_REG_OFS) &= ~pin_mask;

  if ( !EE_HWREG(port_addr + EE_GPIO_PORT_INT0MASK_REG_OFS) ) {

    EE_HWREG(port_addr + EE_GPIO_PORT_INTCTRL_REG_OFS) = PORT_INT0LVL_OFF_gc;

  }

  EE_Gpio_Port_CallBack[port][pin] = EE_NULL_PTR;

#endif	/*
	 * EE_AVR8_PORTA_INT0_ISR || EE_AVR8_PORTB_INT0_ISR ||
	 * EE_AVR8_PORTC_INT0_ISR || EE_AVR8_PORTD_INT0_ISR ||
	 * EE_AVR8_PORTE_INT0_ISR ||
	 * NOTE: Ports from F to Q are not available on ATxmega16D4 MCU 
	 * EE_AVR8_PORTR_INT0_ISR
	 */
#endif	/* __GPIO_ENABLENOTIF_API__ */

  /* Resume IRQs */
  EE_hal_resumeIRQ(flags);

#ifdef	__GPIO_EXTENDED_STATUS__
  return E_OK;
#endif

}
#endif	/* __GPIO_DEINIT_API__ */

#ifdef	__GPIO_READCHANNEL_API__
/*
 * EE_Gpio_ReadChannel() Implementation.
 */
#ifdef	__GPIO_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_Gpio_ReadChannel(
  EE_TYPEGPIOCHANNEL	Channel,
  EE_TYPEGPIOVALUE	*Value
) {

#ifdef	DEBUG
#ifndef	__GPIO_READCHANNEL_NTS__
  volatile EE_UREG	flags;
#endif	/* __GPIO_WRITECHANNEL_NTS__ */
  volatile EE_UREG	pin		= EE_GPIO_CH_2_PIN(Channel);
  volatile EE_UREG	pin_mask	= EE_GPIO_PIN_2_PIN_MASK(pin);
  volatile EE_UREG	port		= EE_GPIO_CH_2_PORT(Channel);
  volatile EE_HWREG_PTR	port_addr 	= EE_HWREG_ADDR(
  						EE_GPIO_PORT_2_ADDR( port )
  					  );
#else	/* DEBUG */
#ifndef	__GPIO_READCHANNEL_NTS__
  register EE_UREG	flags;
#endif	/* __GPIO_READCHANNEL_NTS__ */
  register EE_UREG	pin		= EE_GPIO_CH_2_PIN(Channel);
  register EE_UREG	pin_mask	= EE_GPIO_PIN_2_PIN_MASK(pin);
  register EE_UREG	port		= EE_GPIO_CH_2_PORT(Channel);
  register EE_HWREG_PTR	port_addr 	= EE_HWREG_ADDR(
  						EE_GPIO_PORT_2_ADDR( port )
  					  );
#endif	/* DEBUG */

#ifdef	__GPIO_EXTENDED_STATUS__

  /* Port Pin Validation. */
  EE_ATXMEGA_VALIDATE_W_RV(
    ( (Channel & ~EE_GPIO_ALL_PORT_MASK) <= EE_GPIO_ALL_PIN_MASK ),
    E_GPIO_PARAM_CHANNEL
  );

  /* Port Validation. */
#if	( defined(__AVR_ATxmega16D4__) || defined(__AVR_ATxmega32D4__) )
  /* 
   * NOTE:	Ports from F to Q are not available on ATxmega16D4 MCU
   * 		Port R is used for XTAL or TOSC
   */
  EE_ATXMEGA_VALIDATE_W_RV(
    ( port <= EE_GPIO_PORT_E ),
    E_GPIO_PARAM_CHANNEL
  );
#else	/* __AVR_ATxmega16D4__ || __AVR_ATxmega32D4__ */
  /* NOTE: Ports from F to Q are not available */
  EE_ATXMEGA_VALIDATE_W_RV(
    ( ( port <= EE_GPIO_PORT_E ) || ( port == EE_GPIO_PORT_R ) ),
    E_GPIO_PARAM_CHANNEL
  );
#endif	/* __AVR_ATxmega16D4__ || __AVR_ATxmega32D4__ */

  /* Value Validation. */
  EE_ATXMEGA_VALIDATE_W_RV( 
    ( Value != EE_NULL_PTR ),
    E_GPIO_PARAM_NULL
  );

#endif	/* __GPIO_EXTENDED_STATUS__ */

#ifndef	__GPIO_READCHANNEL_NTS__
  /* Suspend IRQs */
  flags = EE_hal_suspendIRQ();
#endif	/* __GPIO_READCHANNEL_NTS__ */

#ifdef	__GPIO_EXTENDED_STATUS__

#ifndef	__GPIO_READCHANNEL_NTS__
  EE_ATXMEGA_VALIDATE_IRQ_W_RV(
    ( EE_Gpio_Status[port] & pin_mask ),
    E_NOT_OK,
    flags
  );
#else	/* __GPIO_READCHANNEL_NTS__ */
  EE_ATXMEGA_VALIDATE_W_RV( ( EE_Gpio_Status[port] & pin_mask ), E_NOT_OK );
#endif	/* __GPIO_READCHANNEL_NTS__ */

#endif	/* __GPIO_EXTENDED_STATUS__ */

  if ( EE_HWREG(port_addr + EE_GPIO_PORT_IN_REG_OFS) & pin_mask ) {

    (*Value) = EE_HIGH;

  }
  else {

    (*Value) = EE_LOW;

  }

#ifndef	__GPIO_READCHANNEL_NTS__
  /* Resume IRQs */
  EE_hal_resumeIRQ(flags);
#endif	/* __GPIO_READCHANNEL_NTS__ */

#ifdef	__GPIO_EXTENDED_STATUS__
  return E_OK;
#endif

}
#endif	/* __GPIO_READCHANNEL_API__ */

#ifdef	__GPIO_WRITECHANNEL_API__
/*
 * EE_Gpio_WriteChannel() Implementation.
 */
#ifdef	__GPIO_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_Gpio_WriteChannel(
  EE_TYPEGPIOCHANNEL	Channel,
  EE_TYPEGPIOVALUE	Value
) {

#ifdef	DEBUG
#ifndef	__GPIO_WRITECHANNEL_NTS__
  volatile EE_UREG	flags;
#endif	/* __GPIO_WRITECHANNEL_NTS__ */
  volatile EE_UREG	pin		= EE_GPIO_CH_2_PIN(Channel);
  volatile EE_UREG	pin_mask	= EE_GPIO_PIN_2_PIN_MASK(pin);
  volatile EE_UREG	port		= EE_GPIO_CH_2_PORT(Channel);
  volatile EE_HWREG_PTR	port_addr 	= EE_HWREG_ADDR(
  						EE_GPIO_PORT_2_ADDR( port )
  					  );
#else	/* DEBUG */
#ifndef	__GPIO_WRITECHANNEL_NTS__
  register EE_UREG	flags;
#endif	/* __GPIO_WRITECHANNEL_NTS__ */
  register EE_UREG	pin		= EE_GPIO_CH_2_PIN(Channel);
  register EE_UREG	pin_mask	= EE_GPIO_PIN_2_PIN_MASK(pin);
  register EE_UREG	port		= EE_GPIO_CH_2_PORT(Channel);
  register EE_HWREG_PTR	port_addr 	= EE_HWREG_ADDR(
  						EE_GPIO_PORT_2_ADDR( port )
  					  );
#endif	/* DEBUG */

#ifdef	__GPIO_EXTENDED_STATUS__

  /* Port Pin Validation. */
  EE_ATXMEGA_VALIDATE_W_RV(
    ( ( Channel & ~EE_GPIO_ALL_PORT_MASK ) <= EE_GPIO_ALL_PIN_MASK ),
    E_GPIO_PARAM_CHANNEL
  );

  /* Port Validation. */
#if	( defined(__AVR_ATxmega16D4__) || defined(__AVR_ATxmega32D4__) )
  /* NOTE: Ports from F to Q are not available on ATxmega16D4 MCU */
  /* ( ( port <= EE_GPIO_PORT_E ) || ( port == EE_GPIO_PORT_R ) ) */
  EE_ATXMEGA_VALIDATE_W_RV( ( port <= EE_GPIO_PORT_E ), E_GPIO_PARAM_CHANNEL );
#endif	/* __AVR_ATxmega16D4__ || __AVR_ATxmega32D4__ */

  /* Value Validation. */
  EE_ATXMEGA_VALIDATE_W_RV( 
   ( ( Value == EE_HIGH ) || ( Value == EE_LOW ) ),
    E_GPIO_PARAM_VALUE
  );

#endif	/* __GPIO_EXTENDED_STATUS__ */

#ifndef	__GPIO_WRITECHANNEL_NTS__
  /* Suspend IRQs */
  flags = EE_hal_suspendIRQ();
#endif	/* __GPIO_WRITECHANNEL_NTS__ */

#ifdef	__GPIO_EXTENDED_STATUS__


#ifndef	__GPIO_WRITECHANNEL_NTS__
  EE_ATXMEGA_VALIDATE_IRQ_W_RV(
    ( EE_Gpio_Status[port] & pin_mask ),
    E_NOT_OK,
    flags
  );
#else	/* __GPIO_WRITECHANNEL_NTS__ */
  EE_ATXMEGA_VALIDATE_W_RV( ( EE_Gpio_Status[port] & pin_mask ), E_NOT_OK );
#endif	/* __GPIO_WRITECHANNEL_NTS__ */

#endif	/* __GPIO_EXTENDED_STATUS__ */

  if ( Value == EE_HIGH ) {

    EE_HWREG(port_addr + EE_GPIO_PORT_OUTSET_REG_OFS) = pin_mask;

  }
  else {

    EE_HWREG(port_addr + EE_GPIO_PORT_OUTCLR_REG_OFS) = pin_mask;

  }

#ifndef	__GPIO_WRITECHANNEL_NTS__
  /* Resume IRQs */
  EE_hal_resumeIRQ(flags);
#endif	/* __GPIO_WRITECHANNEL_NTS__ */

#ifdef	__GPIO_EXTENDED_STATUS__
  return E_OK;
#endif

}
#endif	/* __GPIO_WRITECHANNEL_API__ */

#ifdef	__GPIO_FLIPCHANNEL_API__
/*
 * EE_Gpio_FlipChannel() Implementation.
 */
#ifdef	__GPIO_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_Gpio_FlipChannel(
  EE_TYPEGPIOCHANNEL	Channel,
  EE_TYPEGPIOVALUE	*Value
) {

#ifdef	DEBUG
#ifndef	__GPIO_FLIPCHANNEL_NTS__
  volatile EE_UREG	flags;
#endif	/* __GPIO_FLIPCHANNEL_NTS__ */
  volatile EE_UREG	pin		= EE_GPIO_CH_2_PIN(Channel);
  volatile EE_UREG	pin_mask	= EE_GPIO_PIN_2_PIN_MASK(pin);
  volatile EE_UREG	port		= EE_GPIO_CH_2_PORT(Channel);
  volatile EE_HWREG_PTR	port_addr 	= EE_HWREG_ADDR(
  						EE_GPIO_PORT_2_ADDR( port )
  					  );
#else	/* DEBUG */
#ifndef	__GPIO_FLIPCHANNEL_NTS__
  register EE_UREG	flags;
#endif	/* __GPIO_FLIPCHANNEL_NTS__ */
  register EE_UREG	pin		= EE_GPIO_CH_2_PIN(Channel);
  register EE_UREG	pin_mask	= EE_GPIO_PIN_2_PIN_MASK(pin);
  register EE_UREG	port		= EE_GPIO_CH_2_PORT(Channel);
  register EE_HWREG_PTR	port_addr 	= EE_HWREG_ADDR(
  						EE_GPIO_PORT_2_ADDR( port )
  					  );
#endif	/* DEBUG */

#ifdef	__GPIO_EXTENDED_STATUS__

  /* Port Pin Validation. */
  EE_ATXMEGA_VALIDATE_W_RV(
    ( ( Channel & ~EE_GPIO_ALL_PORT_MASK ) <= EE_GPIO_ALL_PIN_MASK ),
    E_GPIO_PARAM_CHANNEL
  );

  /* Port Validation. */
#if	( defined(__AVR_ATxmega16D4__) || defined(__AVR_ATxmega32D4__) )
  /* NOTE: Ports from F to Q are not available on ATxmega16D4 MCU */
  /* ( ( port <= EE_GPIO_PORT_E ) || ( port == EE_GPIO_PORT_R ) ) */
  EE_ATXMEGA_VALIDATE_W_RV( ( port <= EE_GPIO_PORT_E ), E_GPIO_PARAM_CHANNEL );
#endif	/* __AVR_ATxmega16D4__ || __AVR_ATxmega32D4__ */

  /* Value Validation. */
  EE_ATXMEGA_VALIDATE_W_RV( 
    ( Value != EE_NULL_PTR ),
    E_GPIO_PARAM_NULL
  );

#endif	/* __GPIO_EXTENDED_STATUS__ */

#ifndef	__GPIO_FLIPCHANNEL_NTS__
  /* Suspend IRQs */
  flags = EE_hal_suspendIRQ();
#endif	/* __GPIO_FLIPCHANNEL_NTS__ */

#ifdef	__GPIO_EXTENDED_STATUS__

#ifndef	__GPIO_FLIPCHANNEL_NTS__
  EE_ATXMEGA_VALIDATE_IRQ_W_RV(
    ( EE_Gpio_Status[port] & pin_mask ),
    E_NOT_OK,
    flags
  );
#else	/* __GPIO_FLIPCHANNEL_NTS__ */
  EE_ATXMEGA_VALIDATE_W_RV( ( EE_Gpio_Status[port] & pin_mask ), E_NOT_OK );
#endif	/* __GPIO_FLIPCHANNEL_NTS__ */

#endif	/* __GPIO_EXTENDED_STATUS__ */

  if ( EE_HWREG(port_addr + EE_GPIO_PORT_IN_REG_OFS) & pin_mask ) {

    (*Value) = EE_HIGH;

  }
  else {

    (*Value) = EE_LOW;

  }

  if ( EE_HWREG(port_addr + EE_GPIO_PORT_DIR_REG_OFS) & pin_mask ) {

    EE_HWREG(port_addr + EE_GPIO_PORT_OUTTGL_REG_OFS) = pin_mask;

  }

#ifndef	__GPIO_FLIPCHANNEL_NTS__
  /* Resume IRQs */
  EE_hal_resumeIRQ(flags);
#endif	/* __GPIO_FLIPCHANNEL_NTS__ */

#ifdef	__GPIO_EXTENDED_STATUS__
  return E_OK;
#endif

}
#endif	/* __GPIO_FLIPCHANNEL_API__ */

#ifdef	__GPIO_ENABLENOTIF_API__
/*
 * EE_Gpio_EnableNotification() Implementation.
 */
#ifdef	__GPIO_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_Gpio_EnableNotification(
  EE_TYPEGPIOCHANNEL	Channel
) {

#ifdef	DEBUG
  volatile EE_UREG	flags;
  volatile EE_UREG	pin		= EE_GPIO_CH_2_PIN(Channel);
  volatile EE_UREG	pin_mask	= EE_GPIO_PIN_2_PIN_MASK(pin);
  volatile EE_UREG	port		= EE_GPIO_CH_2_PORT(Channel);
  volatile EE_HWREG_PTR	port_addr 	= EE_HWREG_ADDR(
  						EE_GPIO_PORT_2_ADDR( port )
  					  );
#else
  register EE_UREG	flags;
  register EE_UREG	pin		= EE_GPIO_CH_2_PIN(Channel);
  register EE_UREG	pin_mask	= EE_GPIO_PIN_2_PIN_MASK(pin);
  register EE_UREG	port		= EE_GPIO_CH_2_PORT(Channel);
  register EE_HWREG_PTR	port_addr 	= EE_HWREG_ADDR(
  						EE_GPIO_PORT_2_ADDR( port )
  					  );
#endif

#ifdef	__GPIO_EXTENDED_STATUS__

  /* Port Pin Validation. */
  EE_ATXMEGA_VALIDATE_W_RV(
    ( ( Channel & ~EE_GPIO_ALL_PORT_MASK ) <= EE_GPIO_ALL_PIN_MASK ),
    E_GPIO_PARAM_CHANNEL
  );

  /* Port Validation. */
  EE_ATXMEGA_VALIDATE_W_RV(
    ( port < EE_GPIO_MAX_PORTS_NUMBER ),
    E_GPIO_PARAM_CHANNEL
  );

#if	( defined(__AVR_ATxmega16D4__) || defined(__AVR_ATxmega32D4__) )
  /* NOTE: Ports from F to Q are not available on ATxmega16D4 MCU */
  /* ( ( port <= EE_GPIO_PORT_E ) || ( port == EE_GPIO_PORT_R ) ) */
  EE_ATXMEGA_VALIDATE_W_RV( ( port <= EE_GPIO_PORT_E ), E_GPIO_PARAM_CHANNEL );
#endif	/* __AVR_ATxmega16D4__ || __AVR_ATxmega32D4__ */

#endif	/* __GPIO_EXTENDED_STATUS__ */

  /* Suspend IRQs */
  flags = EE_hal_suspendIRQ();

#ifdef	__GPIO_EXTENDED_STATUS__

  EE_ATXMEGA_VALIDATE_IRQ_W_RV(
    ( EE_Gpio_Status[port] & pin_mask ),
    E_NOT_OK,
    flags
  );

#if	( \
  defined( EE_AVR8_PORTA_INT0_ISR ) || defined( EE_AVR8_PORTB_INT0_ISR ) || \
  defined( EE_AVR8_PORTC_INT0_ISR ) || defined( EE_AVR8_PORTD_INT0_ISR ) || \
  defined( EE_AVR8_PORTE_INT0_ISR ) || \
  /* NOTE: Ports from F to Q are not available on ATxmega16D4 MCU */ \
  defined( EE_AVR8_PORTR_INT0_ISR ) \
)

  /* Call-Back Validation. */
  EE_ATXMEGA_VALIDATE_W_RV(
    ( EE_Gpio_Port_CallBack[port][pin] != EE_NULL_PTR ),
    E_GPIO_PARAM_CALLBACK
  );

#else

  /* Call-Back Validation. */
  EE_ATXMEGA_VALIDATE_W_RV( EE_FALSE, E_GPIO_PARAM_CALLBACK );

#endif	/*
	 * EE_AVR8_PORTA_INT0_ISR || EE_AVR8_PORTB_INT0_ISR ||
	 * EE_AVR8_PORTC_INT0_ISR || EE_AVR8_PORTD_INT0_ISR ||
	 * EE_AVR8_PORTE_INT0_ISR ||
	 * NOTE: Ports from F to Q are not available on ATxmega16D4 MCU 
	 * EE_AVR8_PORTR_INT0_ISR
	 */

#endif	/* __GPIO_EXTENDED_STATUS__ */

#if	( \
  defined( EE_AVR8_PORTA_INT0_ISR ) || defined( EE_AVR8_PORTB_INT0_ISR ) || \
  defined( EE_AVR8_PORTC_INT0_ISR ) || defined( EE_AVR8_PORTD_INT0_ISR ) || \
  defined( EE_AVR8_PORTE_INT0_ISR ) || \
  /* NOTE: Ports from F to Q are not available on ATxmega16D4 MCU */ \
  defined( EE_AVR8_PORTR_INT0_ISR ) \
)
  if ( !EE_HWREG(port_addr + EE_GPIO_PORT_INT0MASK_REG_OFS) ) {

    EE_HWREG(port_addr + EE_GPIO_PORT_INTCTRL_REG_OFS) =
#ifdef	__AVR_PGMSPACE__
    pgm_read_byte(&EE_Gpio_Port_Pri[port]);
#else	/* __AVR_PGMSPACE__ */
    EE_Gpio_Port_Pri[port];
#endif	/* __AVR_PGMSPACE__ */

  }

  EE_HWREG(port_addr + EE_GPIO_PORT_INT0MASK_REG_OFS) |= pin_mask;

  EE_HWREG(port_addr + EE_GPIO_PORT_INTFLAGS_REG_OFS) = EE_GPIO_ALL_INT_MASK;
#endif	/*
	 * EE_AVR8_PORTA_INT0_ISR || EE_AVR8_PORTB_INT0_ISR ||
	 * EE_AVR8_PORTC_INT0_ISR || EE_AVR8_PORTD_INT0_ISR ||
	 * EE_AVR8_PORTE_INT0_ISR ||
	 * NOTE: Ports from F to Q are not available on ATxmega16D4 MCU 
	 * EE_AVR8_PORTR_INT0_ISR
	 */

  /* Resume IRQs */
  EE_hal_resumeIRQ(flags);

#ifdef	__GPIO_EXTENDED_STATUS__
  return E_OK;
#endif

}

#ifdef	__GPIO_DISABLENOTIF_API__
/*
 * EE_Gpio_DisableNotification() Implementation.
 */
#ifdef	__GPIO_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_Gpio_DisableNotification(
  EE_TYPEGPIOCHANNEL	Channel
) {

#ifdef	DEBUG
  volatile EE_UREG	flags;
  volatile EE_UREG	pin		= EE_GPIO_CH_2_PIN(Channel);
  volatile EE_UREG	pin_mask	= EE_GPIO_PIN_2_PIN_MASK(pin);
  volatile EE_UREG	port		= EE_GPIO_CH_2_PORT(Channel);
  volatile EE_HWREG_PTR	port_addr 	= EE_HWREG_ADDR(
  						EE_GPIO_PORT_2_ADDR( port )
  					  );
#else
  register EE_UREG	flags;
  register EE_UREG	pin		= EE_GPIO_CH_2_PIN(Channel);
  register EE_UREG	pin_mask	= EE_GPIO_PIN_2_PIN_MASK(pin);
  register EE_UREG	port		= EE_GPIO_CH_2_PORT(Channel);
  register EE_HWREG_PTR	port_addr 	= EE_HWREG_ADDR(
  						EE_GPIO_PORT_2_ADDR( port )
  					  );
#endif

#ifdef	__GPIO_EXTENDED_STATUS__

  /* Port Pin Validation. */
  EE_ATXMEGA_VALIDATE_W_RV(
    ( (Channel & ~EE_GPIO_ALL_PORT_MASK) <= EE_GPIO_ALL_PIN_MASK ),
    E_GPIO_PARAM_CHANNEL
  );

  /* Port Validation. */
  EE_ATXMEGA_VALIDATE_W_RV(
    ( port < EE_GPIO_MAX_PORTS_NUMBER ),
    E_GPIO_PARAM_CHANNEL
  );

#if	( defined(__AVR_ATxmega16D4__) || defined(__AVR_ATxmega32D4__) )
  /* NOTE: Ports from F to Q are not available on ATxmega16D4 MCU */
  /* ( ( port <= EE_GPIO_PORT_E ) || ( port == EE_GPIO_PORT_R ) ) */
  EE_ATXMEGA_VALIDATE_W_RV( ( port <= EE_GPIO_PORT_E ), E_GPIO_PARAM_CHANNEL );
#endif	/* __AVR_ATxmega16D4__ || __AVR_ATxmega32D4__ */

#endif	/* __GPIO_EXTENDED_STATUS__ */

  /* Suspend IRQs */
  flags = EE_hal_suspendIRQ();

#ifdef	__GPIO_EXTENDED_STATUS__

  EE_ATXMEGA_VALIDATE_IRQ_W_RV(
    ( EE_Gpio_Status[port] & pin_mask ),
    E_NOT_OK,
    flags
  );

#if	( \
  defined( EE_AVR8_PORTA_INT0_ISR ) || defined( EE_AVR8_PORTB_INT0_ISR ) || \
  defined( EE_AVR8_PORTC_INT0_ISR ) || defined( EE_AVR8_PORTD_INT0_ISR ) || \
  defined( EE_AVR8_PORTE_INT0_ISR ) || \
  /* NOTE: Ports from F to Q are not available on ATxmega16D4 MCU */ \
  defined( EE_AVR8_PORTR_INT0_ISR ) \
)

  /* Call-Back Validation. */
  EE_ATXMEGA_VALIDATE_W_RV(
    ( EE_Gpio_Port_CallBack[port][pin] != EE_NULL_PTR ),
    E_GPIO_PARAM_CALLBACK
  );

#else

  /* Call-Back Validation. */
  EE_ATXMEGA_VALIDATE_W_RV( EE_FALSE, E_GPIO_PARAM_CALLBACK );

#endif	/*
	 * EE_AVR8_PORTA_INT0_ISR || EE_AVR8_PORTB_INT0_ISR ||
	 * EE_AVR8_PORTC_INT0_ISR || EE_AVR8_PORTD_INT0_ISR ||
	 * EE_AVR8_PORTE_INT0_ISR ||
	 * NOTE: Ports from F to Q are not available on ATxmega16D4 MCU 
	 * EE_AVR8_PORTR_INT0_ISR
	 */

#endif	/* __GPIO_EXTENDED_STATUS__ */

#if	( \
  defined( EE_AVR8_PORTA_INT0_ISR ) || defined( EE_AVR8_PORTB_INT0_ISR ) || \
  defined( EE_AVR8_PORTC_INT0_ISR ) || defined( EE_AVR8_PORTD_INT0_ISR ) || \
  defined( EE_AVR8_PORTE_INT0_ISR ) || \
  /* NOTE: Ports from F to Q are not available on ATxmega16D4 MCU */ \
  defined( EE_AVR8_PORTR_INT0_ISR ) \
)
  EE_HWREG(port_addr + EE_GPIO_PORT_INTFLAGS_REG_OFS) = EE_GPIO_ALL_INT_MASK;

  EE_HWREG(port_addr + EE_GPIO_PORT_INT0MASK_REG_OFS) &= ~pin_mask;

  if ( !EE_HWREG(port_addr + EE_GPIO_PORT_INT0MASK_REG_OFS) ) {

    EE_HWREG(port_addr + EE_GPIO_PORT_INTCTRL_REG_OFS) = PORT_INT0LVL_OFF_gc;

  }
#endif	/*
	 * EE_AVR8_PORTA_INT0_ISR || EE_AVR8_PORTB_INT0_ISR ||
	 * EE_AVR8_PORTC_INT0_ISR || EE_AVR8_PORTD_INT0_ISR ||
	 * EE_AVR8_PORTE_INT0_ISR ||
	 * NOTE: Ports from F to Q are not available on ATxmega16D4 MCU 
	 * EE_AVR8_PORTR_INT0_ISR
	 */

  /* Resume IRQs */
  EE_hal_resumeIRQ(flags);

#ifdef	__GPIO_EXTENDED_STATUS__
  return E_OK;
#endif

}
#endif	/* __GPIO_DISABLENOTIF_API__ */
#endif	/* __GPIO_ENABLENOTIF_API__ */
