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
 * Erika Enterprise - SCI Driver - Atmel ATXMEGA SCI Driver Implementation.
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

#define	EE_SCI_MAX_CHANNELS_NUMBER	0x02U

#define	EE_SCI_FLAG_UNUSED		0x80U	/* Unused Flag */
#define	EE_SCI_FLAG_PARITY_WRONG	0x10U	/* Parity Wrong Flag */
#define	EE_SCI_FLAG_BIT_WRONG		0x04U	/* Bit Wrong Flag */

#define	EE_SCI_USART_C_0_BASE_ADDR	0x08A0U	/* USARTC0 Base Address */
#define	EE_SCI_USART_D_0_BASE_ADDR	0x09A0U	/* USARTD0 Base Address */

#define	EE_SCI_USART_BASE_ADDR		0x08A0U	/* USART Base Address */

#define	EE_SCI_USART_0_OFS		0x0100U	/* USART 0 Offset. */
#define	EE_SCI_USART_0_OFS_SH_BITS	0x0008U	/* USART 0 Offset Shift Bits. */

#define	EE_SCI_CH_2_ADDR(_ch)	(					\
  EE_SCI_USART_BASE_ADDR + ( (_ch) << EE_SCI_USART_0_OFS_SH_BITS )	\
)

/*
 * USART Register Offsets.
 */
#define	EE_SCI_USART_DATA_REG_OFS	0x00U	/* Data			      */
#define	EE_SCI_USART_STATUS_REG_OFS	0x01U	/* Status		      */

#define	EE_SCI_USART_CTRLA_REG_OFS	0x03U	/* Control A		      */
#define	EE_SCI_USART_CTRLB_REG_OFS	0x04U	/* Control B		      */
#define	EE_SCI_USART_CTRLC_REG_OFS	0x05U	/* Control C		      */

#define	EE_SCI_USART_BAUD_CTRLA_REG_OFS	0x06U	/* Baud Rate Control A	      */
#define	EE_SCI_USART_BAUD_CTRLB_REG_OFS	0x07U	/* Baud Rate Control B	      */

#define	EE_SCI_INIT_MASK	0x80U	/* Channel Initialization Mask.	      */

#define	EE_SCI_USART_0_BSEL_SH_BITS	0x08U	/* CTRB BSEL Shift Bits	      */

#if	( \
  defined(__SCI_EXTENDED_STATUS__) || \
  ( \
    defined(__SCI_ENABLENOTIF_API__) && \
    ( \
      defined( EE_AVR8_USARTC0_RXC_ISR ) || \
      defined( EE_AVR8_USARTC0_TXC_ISR ) || \
      defined( EE_AVR8_USARTD0_RXC_ISR ) || \
      defined( EE_AVR8_USARTD0_TXC_ISR ) \
    ) \
  ) \
)
#pragma pack(1)
/*
 * SCI Channel Status Type
 */
typedef struct {

#ifdef	__SCI_EXTENDED_STATUS__
  EE_UREG		Flags;		/* Status Flags			 */
#endif	/* __SCI_EXTENDED_STATUS__ */

#ifdef	__SCI_ENABLENOTIF_API__
#if	( \
  defined( EE_AVR8_USARTC0_RXC_ISR ) || defined( EE_AVR8_USARTC0_TXC_ISR ) || \
  defined( EE_AVR8_USARTD0_RXC_ISR ) || defined( EE_AVR8_USARTD0_TXC_ISR ) \
)

#if	( \
  defined( EE_AVR8_USARTC0_RXC_ISR ) || defined( EE_AVR8_USARTD0_RXC_ISR ) \
)
  EE_ISR_callback	RxCallBack;	/* Rx Notification Callback.	 */
#endif	/* EE_AVR8_USARTC0_RXC_ISR || EE_AVR8_USARTD0_RXC_ISR */

#if	( \
  defined( EE_AVR8_USARTC0_TXC_ISR ) || defined( EE_AVR8_USARTD0_TXC_ISR ) \
)
  EE_ISR_callback	TxCallBack;	/* Tx Notification Callback.	 */
#endif	/* EE_AVR8_USARTC0_TXC_ISR || EE_AVR8_USARTD0_TXC_ISR */

#endif	/*
	 * EE_AVR8_USARTC0_RXC_ISR || EE_AVR8_USARTC0_TXC_ISR ||
	 * EE_AVR8_USARTD0_RXC_ISR || EE_AVR8_USARTD0_TXC_ISR
	 */
#endif	/* __SCI_ENABLENOTIF_API__ */

} EE_TYPESCISTATUS;
#pragma pack()

/*
 * SCI Channel Status Array.
 */
#ifdef	DEBUG
static volatile EE_TYPESCISTATUS
#else
static EE_TYPESCISTATUS
#endif
#if	0
EE_Sci_Status[EE_SCI_MAX_CHANNELS_NUMBER] = { 0 };
#else	/* 0 */
EE_Sci_Status[EE_SCI_MAX_CHANNELS_NUMBER];
#endif	/* 0 */
#endif	/*
	 * __SCI_EXTENDED_STATUS__ ||
	 *   __SCI_ENABLENOTIF_API__ &&
	 *     EE_AVR8_USARTC0_RXC_ISR || EE_AVR8_USARTC0_TXC_ISR ||
	 *     EE_AVR8_USARTD0_RXC_ISR || EE_AVR8_USARTD0_TXC_ISR
	 */

#ifdef	__SCI_ENABLENOTIF_API__
#if	( \
  defined( EE_AVR8_USARTC0_RXC_ISR ) || defined( EE_AVR8_USARTC0_TXC_ISR ) || \
  defined( EE_AVR8_USARTD0_RXC_ISR ) || defined( EE_AVR8_USARTD0_TXC_ISR ) \
)

#ifndef	__AVR_PGMSPACE__
#pragma pack(1)
/*
 * SCI IRQ(s) Priority Type.
 */
typedef struct {

#if	( \
  defined( EE_AVR8_USARTC0_RXC_ISR ) || defined( EE_AVR8_USARTD0_RXC_ISR ) \
)
  EE_TYPEPRIO	Rx;
#endif	/* EE_AVR8_USARTC0_RXC_ISR || EE_AVR8_USARTD0_RXC_ISR */

#if	( \
  defined( EE_AVR8_USARTC0_TXC_ISR ) || defined( EE_AVR8_USARTD0_TXC_ISR ) \
)
  EE_TYPEPRIO	Tx;
#endif	/* EE_AVR8_USARTC0_TXC_ISR || EE_AVR8_USARTD0_TXC_ISR */

} EE_TYPESCIPRIO;
#pragma pack()
#endif	/* __AVR_PGMSPACE__ */

#ifdef	__AVR_PGMSPACE__
#ifdef	DEBUG
/*
 * SCI RX IRQ(s) Priority Array.
 */
static volatile const EE_TYPEPRIO
#else
static const EE_TYPEPRIO
#endif
EE_Sci_Rx_Irq_Priority[EE_SCI_MAX_CHANNELS_NUMBER] PROGMEM = {

#if	( \
  defined( EE_AVR8_USARTC0_RXC_ISR ) || defined( EE_AVR8_USARTD0_RXC_ISR ) \
)
#ifdef	EE_AVR8_USARTC0_RXC_ISR_PRI
	( EE_AVR8_USARTC0_RXC_ISR_PRI << USART_RXCINTLVL_gp ),
#else
	EE_ISR_PRI_OFF,
#endif
#endif	/* EE_AVR8_USARTC0_RXC_ISR || EE_AVR8_USARTD0_RXC_ISR */

#if	( \
  defined( EE_AVR8_USARTC0_RXC_ISR ) || defined( EE_AVR8_USARTD0_RXC_ISR ) \
)
#ifdef	EE_AVR8_USARTD0_RXC_ISR_PRI
	( EE_AVR8_USARTD0_RXC_ISR_PRI << USART_RXCINTLVL_gp ),
#else
	EE_ISR_PRI_OFF,
#endif
#endif	/* EE_AVR8_USARTC0_RXC_ISR || EE_AVR8_USARTD0_RXC_ISR */

};	/* EE_Sci_Rx_Irq_Priority[] */

#ifdef	DEBUG
/*
 * SCI TX IRQ(s) Priority Array.
 */
static volatile const EE_TYPEPRIO
#else
static const EE_TYPEPRIO
#endif
EE_Sci_Tx_Irq_Priority[EE_SCI_MAX_CHANNELS_NUMBER] PROGMEM = {

#if	( \
  defined( EE_AVR8_USARTC0_TXC_ISR ) || defined( EE_AVR8_USARTD0_TXC_ISR ) \
)
#ifdef	EE_AVR8_USARTC0_TXC_ISR_PRI
	( EE_AVR8_USARTC0_TXC_ISR_PRI << USART_TXCINTLVL_gp ),
#else
	EE_ISR_PRI_OFF,
#endif
#endif	/* EE_AVR8_USARTC0_TXC_ISR || EE_AVR8_USARTD0_TXC_ISR */

#if	( \
  defined( EE_AVR8_USARTC0_TXC_ISR ) || defined( EE_AVR8_USARTD0_TXC_ISR ) \
)
#ifdef	EE_AVR8_USARTD0_TXC_ISR_PRI
	( EE_AVR8_USARTD0_TXC_ISR_PRI << USART_TXCINTLVL_gp ),
#else
	EE_ISR_PRI_OFF,
#endif
#endif	/* EE_AVR8_USARTC0_TXC_ISR || EE_AVR8_USARTD0_TXC_ISR */

};	/* EE_Sci_Tx_Irq_Priority[] */

#else	/* __AVR_PGMSPACE__ */
#ifdef	DEBUG
/*
 * SCI IRQ(s) Priority Array.
 */
static volatile const EE_TYPESCIPRIO
#else
static const EE_TYPESCIPRIO
#endif
EE_Sci_Irq_Priority[EE_SCI_MAX_CHANNELS_NUMBER] = {

	{
#if	( \
  defined( EE_AVR8_USARTC0_RXC_ISR ) || defined( EE_AVR8_USARTD0_RXC_ISR ) \
)
#ifdef	EE_AVR8_USARTC0_RXC_ISR_PRI
		( EE_AVR8_USARTC0_RXC_ISR_PRI << USART_RXCINTLVL_gp ),
#else
		EE_ISR_PRI_OFF,
#endif
#endif	/* EE_AVR8_USARTC0_RXC_ISR || EE_AVR8_USARTD0_RXC_ISR */
#if	( \
  defined( EE_AVR8_USARTC0_TXC_ISR ) || defined( EE_AVR8_USARTD0_TXC_ISR ) \
)
#ifdef	EE_AVR8_USARTC0_TXC_ISR_PRI
		( EE_AVR8_USARTC0_TXC_ISR_PRI << USART_TXCINTLVL_gp ),
#else
		EE_ISR_PRI_OFF,
#endif
#endif	/* EE_AVR8_USARTC0_TXC_ISR || EE_AVR8_USARTD0_TXC_ISR */
	},
	{
#if	( \
  defined( EE_AVR8_USARTC0_RXC_ISR ) || defined( EE_AVR8_USARTD0_RXC_ISR ) \
)
#ifdef	EE_AVR8_USARTD0_RXC_ISR_PRI
		( EE_AVR8_USARTD0_RXC_ISR_PRI << USART_RXCINTLVL_gp ),
#else
		EE_ISR_PRI_OFF,
#endif
#endif	/* EE_AVR8_USARTC0_RXC_ISR || EE_AVR8_USARTD0_RXC_ISR */
#if	( \
  defined( EE_AVR8_USARTC0_TXC_ISR ) || defined( EE_AVR8_USARTD0_TXC_ISR ) \
)
#ifdef	EE_AVR8_USARTD0_TXC_ISR_PRI
		( EE_AVR8_USARTD0_TXC_ISR_PRI << USART_TXCINTLVL_gp ),
#else
		EE_ISR_PRI_OFF,
#endif
#endif	/* EE_AVR8_USARTC0_TXC_ISR || EE_AVR8_USARTD0_TXC_ISR */
	}

};	/* EE_Sci_Irq_Priority[] */
#endif	/* __AVR_PGMSPACE__ */

#if	0

#if	( \
  defined( EE_AVR8_USARTC0_RXC_ISR ) || defined( EE_AVR8_USARTD0_RXC_ISR ) \
)

/* SCI Generic Rx ISR
 *
 * param	Channel	Channel Identifier.
 */
static void
EE_Sci_Rx_ISR( EE_TYPESCICHANNEL Channel )
{

#ifdef	DEBUG
  volatile EE_UREG		flags;
  //~ volatile EE_HWREG_PTR		addr;
  volatile EE_ISR_callback	cb;
#else
  register EE_UREG		flags;
  //~ register EE_HWREG_PTR		addr;
  register EE_ISR_callback	cb;
#endif

  /* Suspend Interrupt */
  flags = EE_hal_suspendIRQ();

  cb = EE_Sci_Status[Channel].RxCallBack;

  if ( cb != EE_NULL_PTR ) {

    /* Resume Interrupt */
    EE_hal_resumeIRQ(flags);

    /* Call SCI Rx CallBack. */
    (*cb)();

    /* Suspend Interrupt */
    flags = EE_hal_suspendIRQ();

  }

  //~ addr	= EE_HWREG_ADDR(
  	    //~ EE_SCI_CH_2_ADDR( Channel ) + EE_SCI_USART_STATUS_REG_OFS
  	  //~ );

  //~ EE_HWREG(addr) = (
    //~ USART_RXCIF_bm | USART_FERR_bm | USART_BUFOVF_bm | USART_PERR_bm
  //~ );

  /* Resume Interrupt */
  EE_hal_resumeIRQ(flags);

}	/* EE_Sci_Rx_ISR( EE_TYPESCICHANNEL Channel ) */

#endif	/* EE_AVR8_USARTC0_RXC_ISR || EE_AVR8_USARTD0_RXC_ISR */

#if	( \
  defined( EE_AVR8_USARTC0_TXC_ISR ) || defined( EE_AVR8_USARTD0_TXC_ISR ) \
)

/* SCI Generic Tx ISR
 *
 * param	Channel	Channel Identifier.
 */
static void
EE_Sci_Tx_ISR( EE_TYPESCICHANNEL Channel )
{

#ifdef	DEBUG
  volatile EE_UREG		flags;
  volatile EE_ISR_callback	cb;
#else
  register EE_UREG		flags;
  register EE_ISR_callback	cb;
#endif

  /* Suspend Interrupt */
  flags = EE_hal_suspendIRQ();

  cb = EE_Sci_Status[Channel].TxCallBack;

  if ( cb != EE_NULL_PTR ) {

    /* Resume Interrupt */
    EE_hal_resumeIRQ(flags);

    /* Call SCI Tx CallBack. */
    (*cb)();

    /* Suspend Interrupt */
    flags = EE_hal_suspendIRQ();

  }

  /* Resume Interrupt */
  EE_hal_resumeIRQ(flags);

}	/* EE_Sci_Tx_ISR( EE_TYPESCICHANNEL Channel ) */

#endif	/* EE_AVR8_USARTC0_TXC_ISR || EE_AVR8_USARTD0_TXC_ISR */

#ifdef	EE_AVR8_USARTC0_RXC_ISR
#if	( EE_AVR8_USARTC0_RXC_ISR_CAT == 1 )
ISR1(EE_AVR8_USARTC0_RXC_ISR)
{
  EE_Sci_Rx_ISR( EE_SCI_CHANNEL_C_0 );
}
#else
ISR2(EE_AVR8_USARTC0_RXC_ISR)
{
  EE_Sci_Rx_ISR( EE_SCI_CHANNEL_C_0 );
}
#endif
#endif	/* EE_AVR8_USARTC0_RXC_ISR */

#ifdef	EE_AVR8_USARTC0_TXC_ISR
#if	( EE_AVR8_USARTC0_TXC_ISR_CAT == 1 )
ISR1(EE_AVR8_USARTC0_TXC_ISR)
{
  EE_Sci_Tx_ISR( EE_SCI_CHANNEL_C_0 );
}
#else
ISR2(EE_AVR8_USARTC0_TXC_ISR)
{
  EE_Sci_Tx_ISR( EE_SCI_CHANNEL_C_0 );
}
#endif
#endif	/* EE_AVR8_USARTC0_TXC_ISR */

#ifdef	EE_AVR8_USARTD0_RXC_ISR
#if	( EE_AVR8_USARTD0_RXC_ISR_CAT == 1 )
ISR1(EE_AVR8_USARTD0_RXC_ISR)
{
  EE_Sci_Rx_ISR( EE_SCI_CHANNEL_D_0 );
}
#else
ISR2(EE_AVR8_USARTD0_RXC_ISR)
{
  EE_Sci_Rx_ISR( EE_SCI_CHANNEL_D_0 );
}
#endif
#endif	/* EE_AVR8_USARTD0_RXC_ISR */

#ifdef	EE_AVR8_USARTD0_TXC_ISR
#if	( EE_AVR8_USARTD0_TXC_ISR_CAT == 1 )
ISR1(EE_AVR8_USARTD0_TXC_ISR)
{
  EE_Sci_Tx_ISR( EE_SCI_CHANNEL_D_0 );
}
#else
ISR2(EE_AVR8_USARTD0_TXC_ISR)
{
  EE_Sci_Tx_ISR( EE_SCI_CHANNEL_D_0 );
}
#endif
#endif	/* EE_AVR8_USARTD0_TXC_ISR */

#else	/* 0 */

#ifdef	EE_AVR8_USARTC0_RXC_ISR
#if	( EE_AVR8_USARTC0_RXC_ISR_CAT == 1 )
ISR1(EE_AVR8_USARTC0_RXC_ISR)
#else
ISR2(EE_AVR8_USARTC0_RXC_ISR)
#endif
{
  (EE_Sci_Status[EE_SCI_CHANNEL_C_0].RxCallBack)();
}
#endif	/* EE_AVR8_USARTC0_RXC_ISR */

#ifdef	EE_AVR8_USARTC0_TXC_ISR
#if	( EE_AVR8_USARTC0_TXC_ISR_CAT == 1 )
ISR1(EE_AVR8_USARTC0_TXC_ISR)
#else
ISR2(EE_AVR8_USARTC0_TXC_ISR)
#endif
{
  (EE_Sci_Status[EE_SCI_CHANNEL_C_0].TxCallBack)();
}
#endif	/* EE_AVR8_USARTC0_TXC_ISR */

#ifdef	EE_AVR8_USARTD0_RXC_ISR
#if	( EE_AVR8_USARTD0_RXC_ISR_CAT == 1 )
ISR1(EE_AVR8_USARTD0_RXC_ISR)
#else
ISR2(EE_AVR8_USARTD0_RXC_ISR)
#endif
{
  (EE_Sci_Status[EE_SCI_CHANNEL_D_0].RxCallBack)();
}
#endif	/* EE_AVR8_USARTD0_RXC_ISR */

#ifdef	EE_AVR8_USARTD0_TXC_ISR
#if	( EE_AVR8_USARTD0_TXC_ISR_CAT == 1 )
ISR1(EE_AVR8_USARTD0_TXC_ISR)
#else
ISR2(EE_AVR8_USARTD0_TXC_ISR)
#endif
{
  (EE_Sci_Status[EE_SCI_CHANNEL_D_0].TxCallBack)();
}
#endif	/* EE_AVR8_USARTD0_TXC_ISR */

#endif	/* 0 */

#endif	/*
	 * EE_AVR8_USARTC0_RXC_ISR || EE_AVR8_USARTC0_TXC_ISR ||
	 * EE_AVR8_USARTD0_RXC_ISR || EE_AVR8_USARTD0_TXC_ISR
	 */
#endif	/* __SCI_ENABLENOTIF_API__ */

#ifndef	__SCI_BAUDRATE_TABLE__

/*
 * BaudRate Calculation Formulae
 *
 * - CLK2X = 0 and BSCALE >= 0:
 *
 *   	BAUDRATE = CLK / ( 2^BSCALE * 16 * (BSEL + 1) )
 *
 * - CLK2X = 0 and BSCALE < 0:
 *
 *   	BAUDRATE = CLK / ( 16 * ( ( 2^BSCALE * BSEL ) + 1 ) )
 *
 * - CLK2X = 1 and BSCALE >= 0:
 *
 *   	BAUDRATE = CLK / ( 2^BSCALE * 8 * (BSEL + 1) )
 *
 * - CLK2X = 1 and BSCALE < 0:
 *
 *   	BAUDRATE = CLK / ( 8 * ( ( 2^BSCALE * BSEL ) + 1 ) )
 */

#define	EE_SCI_BSEL_MIN		0x0000U		/*    0	*/
#define	EE_SCI_BSEL_MAX		0x1000U		/* 4096	*/
#define	EE_SCI_BSCALE_MIN	0xF9		/*   -7	*/
#define	EE_SCI_BSCALE_MAX	0x08		/*    8	*/

/*
 * BaudRate Parameters Calculation.
 *
 * param[in]	BaudRate	BAUDRATE
 * param[in]	Clock		CLK
 * param[in]	Clk2x		CLK2X
 * param[out]	Scale		BSCALE
 * param[out]	Period		BSEL
 *
 * return	Nothing.
 */
#ifdef	DEBUG
static volatile
#else
static
#endif
void EE_Sci_BaudRate2PeriodScale(
  EE_TYPESCIBAUDRATE	BaudRate,
  EE_TYPESCICLOCK	Clock,
  EE_UINT8		Clk2x,
  EE_INT8		*ScalePtr,
  EE_UINT16		*PeriodPtr
) {

#ifdef	DEBUG
  volatile EE_INT8		bscale;
  volatile EE_UINT16		bsel;
  volatile EE_TYPESCICLOCK	clk, err;
#else
  register EE_INT8		bscale;
  register EE_UINT16		bsel;
  register EE_TYPESCICLOCK	clk, err;
#endif

  err = Clock;

  (*ScalePtr) = EE_SCI_BSCALE_MIN;
  (*PeriodPtr) = EE_SCI_BSEL_MIN;

  for ( bscale = EE_SCI_BSCALE_MIN; bscale < EE_SCI_BSCALE_MAX; bscale++ ) {

    for ( bsel = EE_SCI_BSEL_MIN; bsel < EE_SCI_BSEL_MAX; bsel++ ) {

      if ( bscale < 0x00 ) {

	clk = ( 
	  ( 
	    ( ( (EE_TYPESCICLOCK)0x01U << (EE_UINT8)(-bscale) ) * bsel ) + 1
	  ) << Clk2x 
	);

      }
      else {

	clk = ( 
	  ( (EE_TYPESCICLOCK)0x01U << (EE_UINT8)bscale ) *
	  ( ( bsel + 1 ) << Clk2x ) 
	);

      }

      clk *= BaudRate;

      if ( clk > Clock ) {

	clk -= Clock;

      }
      else {

	clk = Clock - clk;

      }

      if ( clk < err ) {

	err = clk;
	(*ScalePtr) = bscale;
	(*PeriodPtr) = bsel;

      }

    }	/* for ( bsel ) */

  }	/* for ( bscale ) */

}	/* EE_Sci_BaudRate2PeriodScale() */

#else	/* __SCI_BAUDRATE_TABLE__ */

#define	EE_SCI_BAUDRATE_TABLE_ENTRIES	\
	( ( EE_SCI_BAUDRATE_115200 + 0x01U ) << 0x01U )

static const EE_UINT16
#ifdef	__AVR_PGMSPACE__
EE_Sci_BSEL[EE_SCI_BAUDRATE_TABLE_ENTRIES] PROGMEM = {
#else	/* __AVR_PGMSPACE__ */
EE_Sci_BSEL[EE_SCI_BAUDRATE_TABLE_ENTRIES] = {
#endif	/* __AVR_PGMSPACE__ */
	12, 12, 12, 138, 12, 137, 12, 135, 12, 131,
	12, 12, 12, 138, 12, 138, 12, 137, 12, 135
};

static const EE_INT8
#ifdef	__AVR_PGMSPACE__
EE_Sci_BSCALE[EE_SCI_BAUDRATE_TABLE_ENTRIES] PROGMEM = {
#else	/* __AVR_PGMSPACE__ */
EE_Sci_BSCALE[EE_SCI_BAUDRATE_TABLE_ENTRIES] = {
#endif	/* __AVR_PGMSPACE__ */
	6, 5, 4, 0, 3, -1, 2, -2, 1, -3,
	7, 6, 5, 1, 4, 0, 3, -1, 2, -2
};

#if	0	/* Baudrate Error % Multiplied By 10. */
static const EE_INT8
#ifdef	__AVR_PGMSPACE__
EE_Sci_BERR[EE_SCI_BAUDRATE_TABLE_ENTRIES] PROGMEM = {
#else	/* __AVR_PGMSPACE__ */
EE_Sci_BERR[EE_SCI_BAUDRATE_TABLE_ENTRIES] = {
#endif	/* __AVR_PGMSPACE__ */
	2, 2, 2, -1, 2, -1, 2, -1, 2, -1,
	2, 2, 2, -1, 2, -1, 2, -1, 2, -1
};
#endif	/* 0 - Baudrate Error % Multiplied By 10. */

#ifdef	DEBUG
static volatile
#else
static
#endif
void EE_Sci_BaudRate2PeriodScale(
  EE_TYPESCIBAUDRATE	BaudRate,
  EE_TYPESCICLOCK	Clock,
  EE_UINT8		Clk2x,
  EE_INT8		*ScalePtr,
  EE_UINT16		*PeriodPtr
) {

  if ( Clk2x != 0x00U ) {	/* CLK2X = 1 */

    BaudRate += EE_SCI_BAUDRATE_115200 + 0x01U;

  }

#ifdef	__AVR_PGMSPACE__
  (*ScalePtr) = (EE_INT8) pgm_read_byte(&EE_Sci_BSCALE[BaudRate]);
  (*PeriodPtr) = (EE_UINT16) pgm_read_word(&EE_Sci_BSEL[BaudRate]);
#else	/* __AVR_PGMSPACE__ */
  (*ScalePtr) = EE_Sci_BSCALE[BaudRate];
  (*PeriodPtr) = EE_Sci_BSEL[BaudRate];
#endif	/* __AVR_PGMSPACE__ */

}	/* EE_Sci_BaudRate2PeriodScale() */

#endif	/* __SCI_BAUDRATE_TABLE__ */


#ifdef	__SCI_GETSTATUS_API__
/*
 * EE_Sci_GetStatus() Implementation.
 */
EE_TYPESCICHSTAT
EE_Sci_GetStatus(
  EE_TYPESCICHANNEL	Channel
) {

#ifdef	DEBUG
  volatile EE_UREG		flags;
  volatile EE_UREG		hw_status;
  volatile EE_HWREG_PTR		addr;
  volatile EE_TYPESCICHSTAT	ret;
#else
  register EE_UREG		flags;
  register EE_UREG		hw_status;
  register EE_HWREG_PTR		addr;
  register EE_TYPESCICHSTAT	ret;
#endif

#ifdef	__SCI_EXTENDED_STATUS__

  /* Channel Validation */
  EE_ATXMEGA_VALIDATE_W_RV( 
    ( Channel < EE_SCI_MAX_CHANNELS_NUMBER ),
    EE_SCI_CH_UNINIT
  );

#endif	/* __SCI_EXTENDED_STATUS__ */

  /* Suspend Interrupt */
  flags = EE_hal_suspendIRQ();

  ret = EE_SCI_CH_OPERATIONAL;

  addr	= EE_HWREG_ADDR( 
    EE_SCI_CH_2_ADDR( Channel ) + EE_SCI_USART_STATUS_REG_OFS
  );

  hw_status = EE_HWREG(addr);

#ifdef	__SCI_EXTENDED_STATUS__
  /* Channel Initialization Validation */
  if ( !(EE_Sci_Status[Channel].Flags & EE_SCI_INIT_MASK ) ) {

    ret = EE_SCI_CH_UNINIT;

  }
  else
#endif	/* __SCI_EXTENDED_STATUS__ */

  if ( hw_status & ( USART_FERR_bm | USART_BUFOVF_bm | USART_PERR_bm ) ) {

    EE_HWREG(addr) = USART_RXCIF_bm;

    ret = EE_SCI_CH_RX_ERROR;

  }
  else if ( hw_status & USART_RXCIF_bm ) {

    EE_HWREG(addr) = USART_RXCIF_bm;
    ret = EE_SCI_CH_RX_OK;

  }
  else if ( !( hw_status & USART_DREIF_bm ) ) {

    ret = EE_SCI_CH_TX_BUSY;
  
  }
  else if ( hw_status & USART_TXCIF_bm ) {

    EE_HWREG(addr) = USART_TXCIF_bm;
    ret = EE_SCI_CH_TX_OK;

  }

  /* Resume Interrupt */
  EE_hal_resumeIRQ(flags);

  return ret;

};
#endif	/* __SCI_GETSTATUS_API__ */

/*
 * EE_Sci_Init() Implementation.
 */
#ifdef	__SCI_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_Sci_Init(
  EE_TYPESCICHANNEL	Channel,
  EE_TYPESCIFLAGS	Flags,
  EE_TYPESCICLOCK	Clock,
  EE_TYPESCIBAUDRATE	BaudRate,
  EE_ISR_callback	RxNotifCB,
  EE_ISR_callback	TxNotifCB
) {

#ifdef	DEBUG
  volatile EE_UREG		flags;
  volatile EE_HWREG_PTR		addr;
  volatile EE_UINT8		clk2x;
  volatile EE_INT8		bscale;
  volatile EE_UINT16		bsel;
#if	( \
  defined(__SCI_EXTENDED_STATUS__) || \
  ( \
    defined(__SCI_ENABLENOTIF_API__) && \
    ( \
      defined( EE_AVR8_USARTC0_RXC_ISR ) || \
      defined( EE_AVR8_USARTC0_TXC_ISR ) || \
      defined( EE_AVR8_USARTD0_RXC_ISR ) || \
      defined( EE_AVR8_USARTD0_TXC_ISR ) \
    ) \
  ) \
)
  volatile EE_TYPESCISTATUS	*sw_status;
#endif	/*
	 * __SCI_EXTENDED_STATUS__ ||
	 *   __SCI_ENABLENOTIF_API__ &&
	 *     EE_AVR8_USARTC0_RXC_ISR || EE_AVR8_USARTC0_TXC_ISR ||
	 *     EE_AVR8_USARTD0_RXC_ISR || EE_AVR8_USARTD0_TXC_ISR
	 */
#else	/* DEBUG */
  register EE_UREG		flags;
  register EE_HWREG_PTR		addr;
  register EE_UINT8		clk2x;
           EE_INT8		bscale;
           EE_UINT16		bsel;
#if	( \
  defined(__SCI_EXTENDED_STATUS__) || \
  ( \
    defined(__SCI_ENABLENOTIF_API__) && \
    ( \
      defined( EE_AVR8_USARTC0_RXC_ISR ) || \
      defined( EE_AVR8_USARTC0_TXC_ISR ) || \
      defined( EE_AVR8_USARTD0_RXC_ISR ) || \
      defined( EE_AVR8_USARTD0_TXC_ISR ) \
    ) \
  ) \
)
  register EE_TYPESCISTATUS	*sw_status;
#endif	/*
	 * __SCI_EXTENDED_STATUS__ ||
	 *   __SCI_ENABLENOTIF_API__ &&
	 *     EE_AVR8_USARTC0_RXC_ISR || EE_AVR8_USARTC0_TXC_ISR ||
	 *     EE_AVR8_USARTD0_RXC_ISR || EE_AVR8_USARTD0_TXC_ISR
	 */
#endif	/* DEBUG */

#ifdef	__SCI_EXTENDED_STATUS__

  /* Channel Validation. */
  EE_ATXMEGA_VALIDATE_W_RV(
    ( Channel < EE_SCI_MAX_CHANNELS_NUMBER ),
    E_SCI_PARAM_CHANNEL
  );

  /* Flags Validation. */
  EE_ATXMEGA_VALIDATE_W_RV(
    !( 
	Flags & ( 
	  EE_SCI_FLAG_UNUSED | EE_SCI_FLAG_PARITY_WRONG | EE_SCI_FLAG_BIT_WRONG
	) 
    ),
    E_SCI_PARAM_FLAGS
  );

#endif	/* __SCI_EXTENDED_STATUS__ */

  if ( Flags & EE_SCI_FLAG_CLK2X ) {

#ifndef	__SCI_BAUDRATE_TABLE__
    clk2x = 0x03U;
#else
    clk2x = USART_CLK2X_bm;
#endif

  }
  else {

#ifndef	__SCI_BAUDRATE_TABLE__
    clk2x = 0x04U;
#else
    clk2x = 0x00U;
#endif

  }

#ifdef	__SCI_EXTENDED_STATUS__

  /* BaudRate and Clock Validation. */
  EE_ATXMEGA_VALIDATE_W_RV(
#ifndef	__SCI_BAUDRATE_TABLE__
    ( ( BaudRate << Clk2x ) <= Clock ),
#else
    ( BaudRate <= EE_SCI_BAUDRATE_115200 ) && ( Clock <= EE_SCI_CLOCK_32MHZ ),
#endif
    E_SCI_PARAM_BAUDRATE
  );

#endif	/* __SCI_EXTENDED_STATUS__ */

  /* Suspend IRQs */
  flags = EE_hal_suspendIRQ();

#if	( \
  defined(__SCI_EXTENDED_STATUS__) || \
  ( \
    defined(__SCI_ENABLENOTIF_API__) && \
    ( \
      defined( EE_AVR8_USARTC0_RXC_ISR ) || \
      defined( EE_AVR8_USARTC0_TXC_ISR ) || \
      defined( EE_AVR8_USARTD0_RXC_ISR ) || \
      defined( EE_AVR8_USARTD0_TXC_ISR ) \
    ) \
  ) \
)
  sw_status = &EE_Sci_Status[Channel];
#endif	/*
	 * __SCI_EXTENDED_STATUS__ ||
	 *   __SCI_ENABLENOTIF_API__ &&
	 *     EE_AVR8_USARTC0_RXC_ISR || EE_AVR8_USARTC0_TXC_ISR ||
	 *     EE_AVR8_USARTD0_RXC_ISR || EE_AVR8_USARTD0_TXC_ISR
	 */

#ifdef	__SCI_EXTENDED_STATUS__

  EE_ATXMEGA_VALIDATE_IRQ_W_RV(
    !( sw_status->Flags & EE_SCI_INIT_MASK ),
    E_NOT_OK,
    flags
  );

#endif	/* __SCI_EXTENDED_STATUS__ */

#ifdef	__SCI_ENABLENOTIF_API__
#if	( \
  defined( EE_AVR8_USARTC0_RXC_ISR ) || defined( EE_AVR8_USARTC0_TXC_ISR ) || \
  defined( EE_AVR8_USARTD0_RXC_ISR ) || defined( EE_AVR8_USARTD0_TXC_ISR ) \
)
  sw_status->RxCallBack = RxNotifCB;
  sw_status->TxCallBack = TxNotifCB;
#endif	/*
	 * EE_AVR8_USARTC0_RXC_ISR || EE_AVR8_USARTC0_TXC_ISR ||
	 * EE_AVR8_USARTD0_RXC_ISR || EE_AVR8_USARTD0_TXC_ISR
	 */
#endif	/* __SCI_ENABLENOTIF_API__ */

  EE_Sci_BaudRate2PeriodScale(BaudRate, Clock, clk2x, &bscale, &bsel);

  addr = EE_HWREG_ADDR( EE_SCI_CH_2_ADDR( Channel ) );

  EE_HWREG(addr + EE_SCI_USART_BAUD_CTRLA_REG_OFS) = (EE_UREG)bsel;
  EE_HWREG(addr + EE_SCI_USART_BAUD_CTRLB_REG_OFS) =
  ( (bscale << USART_BSCALE0_bp) | (bsel >> EE_SCI_USART_0_BSEL_SH_BITS) );

  EE_HWREG(addr + EE_SCI_USART_CTRLC_REG_OFS) = ( Flags & ~EE_SCI_FLAG_CLK2X );

#ifndef	__SCI_BAUDRATE_TABLE__
  if ( Flags & EE_SCI_FLAG_CLK2X ) {

    clk2x = USART_CLK2X_bm;

  }
  else {

    clk2x = 0x00U;

  }
#endif	/* __SCI_BAUDRATE_TABLE__ */

#ifndef	__SCI_ENABLETX_API__
  clk2x |= USART_TXEN_bm;
#endif	/* !__SCI_ENABLETX_API__ */

#ifndef	__SCI_ENABLERX_API__
  clk2x |= USART_RXEN_bm;
#endif	/* !__SCI_ENABLERX_API__ */

  EE_HWREG(addr + EE_SCI_USART_CTRLB_REG_OFS) = clk2x;

#ifdef	__SCI_EXTENDED_STATUS__
  sw_status->Flags |= EE_SCI_INIT_MASK;
#endif	/* __SCI_EXTENDED_STATUS__ */

  /* Resume IRQs */
  EE_hal_resumeIRQ(flags);

#ifdef	__SCI_EXTENDED_STATUS__
  return E_OK;
#endif	/* __SCI_EXTENDED_STATUS__ */

}

#ifdef	__SCI_DEINIT_API__
/*
 * EE_Sci_DeInit() Implementation.
 */
#ifdef	__SCI_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_Sci_DeInit(
  EE_TYPESCICHANNEL	Channel,
  EE_TYPESCIFLAGS	Flags
) {

#ifdef	DEBUG
  volatile EE_UREG		flags;
  volatile EE_HWREG_PTR		addr;
#if	( \
  defined(__SCI_EXTENDED_STATUS__) || \
  ( \
    defined(__SCI_ENABLENOTIF_API__) && \
    ( \
      defined( EE_AVR8_USARTC0_RXC_ISR ) || \
      defined( EE_AVR8_USARTC0_TXC_ISR ) || \
      defined( EE_AVR8_USARTD0_RXC_ISR ) || \
      defined( EE_AVR8_USARTD0_TXC_ISR ) \
    ) \
  ) \
)
  volatile EE_TYPESCISTATUS	*sw_status;
#endif	/*
	 * __SCI_EXTENDED_STATUS__ ||
	 *   __SCI_ENABLENOTIF_API__ &&
	 *     EE_AVR8_USARTC0_RXC_ISR || EE_AVR8_USARTC0_TXC_ISR ||
	 *     EE_AVR8_USARTD0_RXC_ISR || EE_AVR8_USARTD0_TXC_ISR
	 */
#else	/* DEBUG */
  register EE_UREG		flags;
  register EE_HWREG_PTR		addr;
#if	( \
  defined(__SCI_EXTENDED_STATUS__) || \
  ( \
    defined(__SCI_ENABLENOTIF_API__) && \
    ( \
      defined( EE_AVR8_USARTC0_RXC_ISR ) || \
      defined( EE_AVR8_USARTC0_TXC_ISR ) || \
      defined( EE_AVR8_USARTD0_RXC_ISR ) || \
      defined( EE_AVR8_USARTD0_TXC_ISR ) \
    ) \
  ) \
)
  register EE_TYPESCISTATUS	*sw_status;
#endif	/*
	 * __SCI_EXTENDED_STATUS__ ||
	 *   __SCI_ENABLENOTIF_API__ &&
	 *     EE_AVR8_USARTC0_RXC_ISR || EE_AVR8_USARTC0_TXC_ISR ||
	 *     EE_AVR8_USARTD0_RXC_ISR || EE_AVR8_USARTD0_TXC_ISR
	 */
#endif	/* DEBUG */

#ifdef	__SCI_EXTENDED_STATUS__

  /* Channel Validation. */
  EE_ATXMEGA_VALIDATE_W_RV(
    ( Channel < EE_SCI_MAX_CHANNELS_NUMBER ),
    E_SCI_PARAM_CHANNEL
  );

  /* Flags Validation. */
  EE_ATXMEGA_VALIDATE_W_RV(
    ( Flags == EE_SCI_FLAG_DEFAULT ),
    E_SCI_PARAM_FLAGS
  );

#endif	/* __SCI_EXTENDED_STATUS__ */

  /* Suspend IRQs */
  flags = EE_hal_suspendIRQ();

#if	( \
  defined(__SCI_EXTENDED_STATUS__) || \
  ( \
    defined(__SCI_ENABLENOTIF_API__) && \
    ( \
      defined( EE_AVR8_USARTC0_RXC_ISR ) || \
      defined( EE_AVR8_USARTC0_TXC_ISR ) || \
      defined( EE_AVR8_USARTD0_RXC_ISR ) || \
      defined( EE_AVR8_USARTD0_TXC_ISR ) \
    ) \
  ) \
)
  sw_status = &EE_Sci_Status[Channel];
#endif	/*
	 * __SCI_EXTENDED_STATUS__ ||
	 *   __SCI_ENABLENOTIF_API__ &&
	 *     EE_AVR8_USARTC0_RXC_ISR || EE_AVR8_USARTC0_TXC_ISR ||
	 *     EE_AVR8_USARTD0_RXC_ISR || EE_AVR8_USARTD0_TXC_ISR
	 */

#ifdef	__SCI_EXTENDED_STATUS__

  EE_ATXMEGA_VALIDATE_IRQ_W_RV(
    ( sw_status->Flags & EE_SCI_INIT_MASK ),
    E_NOT_OK,
    flags
  );

#endif	/* __SCI_EXTENDED_STATUS__ */

  addr	= EE_HWREG_ADDR( EE_SCI_CH_2_ADDR( Channel ) );

  EE_HWREG(addr + EE_SCI_USART_CTRLB_REG_OFS) &= ~(
    USART_RXEN_bm | USART_TXEN_bm | USART_CLK2X_bm
  );

#ifdef	__SCI_ENABLENOTIF_API__
#if	( \
  defined( EE_AVR8_USARTC0_RXC_ISR ) || defined( EE_AVR8_USARTC0_TXC_ISR ) || \
  defined( EE_AVR8_USARTD0_RXC_ISR ) || defined( EE_AVR8_USARTD0_TXC_ISR ) \
)
  EE_HWREG( addr + EE_SCI_USART_CTRLA_REG_OFS ) &=
  ~( USART_RXCINTLVL_gm | USART_TXCINTLVL_gm | USART_DREINTLVL_gm );

  sw_status->RxCallBack = EE_NULL_PTR;
  sw_status->TxCallBack = EE_NULL_PTR;
#endif	/*
	 * EE_AVR8_USARTC0_RXC_ISR || EE_AVR8_USARTC0_TXC_ISR ||
	 * EE_AVR8_USARTD0_RXC_ISR || EE_AVR8_USARTD0_TXC_ISR
	 */
#endif	/* __SCI_ENABLENOTIF_API__ */

  EE_HWREG( addr + EE_SCI_USART_STATUS_REG_OFS ) |= (
    USART_RXCIF_bm | USART_TXCIF_bm |
    USART_FERR_bm | USART_BUFOVF_bm | USART_PERR_bm
  );

#ifdef	__SCI_EXTENDED_STATUS__
  sw_status->Flags &= ~EE_SCI_INIT_MASK;
#endif	/* __SCI_EXTENDED_STATUS__ */

  /* Resume IRQs */
  EE_hal_resumeIRQ(flags);

#ifdef	__SCI_EXTENDED_STATUS__
  return E_OK;
#endif

}
#endif	/* __SCI_DEINIT_API__ */

#ifdef	__SCI_WRITETXDATA_API__
/*
 * EE_Sci_WriteTxData() Implementation.
 */
#ifdef	__SCI_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_Sci_WriteTxData(
  EE_TYPESCICHANNEL	Channel,
  EE_TYPESCIDATA	Data
) {

#ifdef	DEBUG
#ifndef	__SCI_WRITETXDATA_NTS__
  volatile EE_UREG		flags;
#endif
  volatile EE_HWREG_PTR		addr;
#else	/* DEBUG */
#ifndef	__SCI_WRITETXDATA_NTS__
  register EE_UREG		flags;
#endif
  register EE_HWREG_PTR		addr;
#endif	/* DEBUG */

#ifdef	__SCI_EXTENDED_STATUS__

  /* Channel Validation. */
  EE_ATXMEGA_VALIDATE_W_RV(
    ( Channel < EE_SCI_MAX_CHANNELS_NUMBER ),
    E_SCI_PARAM_CHANNEL
  );

#endif	/* __SCI_EXTENDED_STATUS__ */

#ifndef	__SCI_WRITETXDATA_NTS__
  /* Suspend IRQs */
  flags = EE_hal_suspendIRQ();
#endif

  addr = EE_HWREG_ADDR( EE_SCI_CH_2_ADDR( Channel ) );

#ifdef	__SCI_EXTENDED_STATUS__

#ifndef	__SCI_WRITETXDATA_NTS__
  EE_ATXMEGA_VALIDATE_IRQ_W_RV(
    ( EE_Sci_Status[Channel].Flags & EE_SCI_INIT_MASK ),
    E_NOT_OK,
    flags
  );

  EE_ATXMEGA_VALIDATE_IRQ_W_RV(
    ( EE_HWREG( addr + EE_SCI_USART_STATUS_REG_OFS ) & USART_DREIF_bm ),
    E_SCI_PARAM_BUSY,
    flags
  );
#else
  EE_ATXMEGA_VALIDATE_W_RV(
    ( EE_Sci_Status[Channel].Flags & EE_SCI_INIT_MASK ), E_NOT_OK
  );

  EE_ATXMEGA_VALIDATE_W_RV(
    ( EE_HWREG( addr + EE_SCI_USART_STATUS_REG_OFS ) & USART_DREIF_bm ),
    E_SCI_PARAM_BUSY
  );
#endif

#endif	/* __SCI_EXTENDED_STATUS__ */

  EE_HWREG(addr + EE_SCI_USART_DATA_REG_OFS) = Data;

#ifndef	__SCI_WRITETXDATA_NTS__
  /* Resume IRQs */
  EE_hal_resumeIRQ(flags);
#endif

#ifdef	__SCI_EXTENDED_STATUS__
  return E_OK;
#endif

}
#endif	/* __SCI_WRITETXDATA_API__ */

#ifdef	__SCI_DISABLETX_API__
/*
 * EE_Sci_DisableTx() Implementation.
 */
#ifdef	__SCI_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_Sci_DisableTx(
  EE_TYPESCICHANNEL	Channel
) {

#ifdef	DEBUG
  volatile EE_UREG		flags;
  volatile EE_HWREG_PTR		addr;
#else	/* DEBUG */
  register EE_UREG		flags;
  register EE_HWREG_PTR		addr;
#endif	/* DEBUG */

#ifdef	__SCI_EXTENDED_STATUS__

  /* Channel Validation. */
  EE_ATXMEGA_VALIDATE_W_RV(
    ( Channel < EE_SCI_MAX_CHANNELS_NUMBER ),
    E_SCI_PARAM_CHANNEL
  );

#endif	/* __SCI_EXTENDED_STATUS__ */

  /* Suspend IRQs */
  flags = EE_hal_suspendIRQ();

  addr = EE_HWREG_ADDR( EE_SCI_CH_2_ADDR( Channel ) );

#ifdef	__SCI_EXTENDED_STATUS__

  EE_ATXMEGA_VALIDATE_IRQ_W_RV(
    ( EE_Sci_Status[Channel].Flags & EE_SCI_INIT_MASK ),
    E_NOT_OK,
    flags
  );

  EE_ATXMEGA_VALIDATE_IRQ_W_RV(
    ( EE_HWREG( addr + EE_SCI_USART_STATUS_REG_OFS ) & USART_DREIF_bm ),
    E_SCI_PARAM_BUSY,
    flags
  );

#endif	/* __SCI_EXTENDED_STATUS__ */

  EE_HWREG(addr + EE_SCI_USART_CTRLB_REG_OFS) &= ~USART_TXEN_bm;

  /* Resume IRQs */
  EE_hal_resumeIRQ(flags);

#ifdef	__SCI_EXTENDED_STATUS__
  return E_OK;
#endif

}

#ifdef	__SCI_ENABLETX_API__
/*
 * EE_Sci_EnableTx() Implementation.
 */
#ifdef	__SCI_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_Sci_EnableTx(
  EE_TYPESCICHANNEL	Channel
) {

#ifdef	DEBUG
  volatile EE_UREG		flags;
#else	/* DEBUG */
  register EE_UREG		flags;
#endif	/* DEBUG */

#ifdef	__SCI_EXTENDED_STATUS__

  /* Channel Validation. */
  EE_ATXMEGA_VALIDATE_W_RV(
    ( Channel < EE_SCI_MAX_CHANNELS_NUMBER ),
    E_SCI_PARAM_CHANNEL
  );

#endif	/* __SCI_EXTENDED_STATUS__ */

  /* Suspend IRQs */
  flags = EE_hal_suspendIRQ();

#ifdef	__SCI_EXTENDED_STATUS__

  EE_ATXMEGA_VALIDATE_IRQ_W_RV(
    ( EE_Sci_Status[Channel].Flags & EE_SCI_INIT_MASK ),
    E_NOT_OK,
    flags
  );

#endif	/* __SCI_EXTENDED_STATUS__ */

  EE_HWREG( EE_SCI_CH_2_ADDR( Channel ) + EE_SCI_USART_CTRLB_REG_OFS ) |=
  USART_TXEN_bm;

  /* Resume IRQs */
  EE_hal_resumeIRQ(flags);

#ifdef	__SCI_EXTENDED_STATUS__
  return E_OK;
#endif

}
#endif	/* __SCI_ENABLETX_API__ */
#endif	/* __SCI_DISABLETX_API__ */

#ifdef	__SCI_READRXDATA_API__
/*
 * EE_Sci_ReadRxData() Implementation.
 */
#ifdef	__SCI_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_Sci_ReadRxData(
  EE_TYPESCICHANNEL	Channel,
  EE_TYPESCIDATA	*DataPtr
) {

#ifdef	DEBUG
#ifndef	__SCI_READRXDATA_NTS__
  volatile EE_UREG	flags;
#endif
  volatile EE_HWREG_PTR	addr;
#else	/* DEBUG */
#ifndef	__SCI_READRXDATA_NTS__
  register EE_UREG	flags;
#endif
  register EE_HWREG_PTR	addr;
#endif	/* DEBUG */

#ifdef	__SCI_EXTENDED_STATUS__

  /* Channel Validation. */
  EE_ATXMEGA_VALIDATE_W_RV(
    ( Channel < EE_SCI_MAX_CHANNELS_NUMBER ),
    E_SCI_PARAM_CHANNEL
  );

  /* Data Pointer Validation. */
  EE_ATXMEGA_VALIDATE_W_RV( ( DataPtr != EE_NULL_PTR ), E_SCI_PARAM_NULL );

#endif	/* __SCI_EXTENDED_STATUS__ */

#ifndef	__SCI_READRXDATA_NTS__
  /* Suspend IRQs */
  flags = EE_hal_suspendIRQ();
#endif

  addr = EE_HWREG_ADDR( EE_SCI_CH_2_ADDR( Channel ) );

#ifdef	__SCI_EXTENDED_STATUS__

#ifndef	__SCI_READRXDATA_NTS__
  EE_ATXMEGA_VALIDATE_IRQ_W_RV(
    ( EE_Sci_Status[Channel].Flags & EE_SCI_INIT_MASK ),
    E_NOT_OK,
    flags
  );
#else
  EE_ATXMEGA_VALIDATE_W_RV(
    ( EE_Sci_Status[Channel].Flags & EE_SCI_INIT_MASK ),
    E_NOT_OK
  );
#endif

#endif	/* __SCI_EXTENDED_STATUS__ */

  (*DataPtr) = EE_HWREG(addr + EE_SCI_USART_DATA_REG_OFS);

#ifndef	__SCI_READRXDATA_NTS__
  /* Resume IRQs */
  EE_hal_resumeIRQ(flags);
#endif

#ifdef	__SCI_EXTENDED_STATUS__
  return E_OK;
#endif

}
#endif	/* __SCI_READRXDATA_API__ */

#ifdef	__SCI_DISABLERX_API__
/*
 * EE_Sci_DisableRx() Implementation.
 */
#ifdef	__SCI_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_Sci_DisableRx(
  EE_TYPESCICHANNEL	Channel
) {

#ifdef	DEBUG
  volatile EE_UREG	flags;
  volatile EE_HWREG_PTR	addr;
#else	/* DEBUG */
  register EE_UREG	flags;
  register EE_HWREG_PTR	addr;
#endif	/* DEBUG */

#ifdef	__SCI_EXTENDED_STATUS__

  /* Channel Validation. */
  EE_ATXMEGA_VALIDATE_W_RV(
    ( Channel < EE_SCI_MAX_CHANNELS_NUMBER ),
    E_SCI_PARAM_CHANNEL
  );

#endif	/* __SCI_EXTENDED_STATUS__ */

  /* Suspend IRQs */
  flags = EE_hal_suspendIRQ();

  addr = EE_HWREG_ADDR( EE_SCI_CH_2_ADDR( Channel ) );

#ifdef	__SCI_EXTENDED_STATUS__

  EE_ATXMEGA_VALIDATE_IRQ_W_RV(
    ( EE_Sci_Status[Channel].Flags & EE_SCI_INIT_MASK ),
    E_NOT_OK,
    flags
  );

#endif	/* __SCI_EXTENDED_STATUS__ */

  EE_HWREG(addr + EE_SCI_USART_CTRLB_REG_OFS) &= ~USART_RXEN_bm;

  /* Resume IRQs */
  EE_hal_resumeIRQ(flags);

#ifdef	__SCI_EXTENDED_STATUS__
  return E_OK;
#endif

}

#ifdef	__SCI_ENABLERX_API__
/*
 * EE_Sci_EnableRx() Implementation.
 */
#ifdef	__SCI_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_Sci_EnableRx(
  EE_TYPESCICHANNEL	Channel
) {

#ifdef	DEBUG
  volatile EE_UREG	flags;
#else	/* DEBUG */
  register EE_UREG	flags;
#endif	/* DEBUG */

#ifdef	__SCI_EXTENDED_STATUS__

  /* Channel Validation. */
  EE_ATXMEGA_VALIDATE_W_RV(
    ( Channel < EE_SCI_MAX_CHANNELS_NUMBER ),
    E_SCI_PARAM_CHANNEL
  );

#endif	/* __SCI_EXTENDED_STATUS__ */

  /* Suspend IRQs */
  flags = EE_hal_suspendIRQ();

#ifdef	__SCI_EXTENDED_STATUS__

  EE_ATXMEGA_VALIDATE_IRQ_W_RV(
    ( EE_Sci_Status[Channel].Flags & EE_SCI_INIT_MASK ),
    E_NOT_OK,
    flags
  );

#endif	/* __SCI_EXTENDED_STATUS__ */

  EE_HWREG(EE_SCI_CH_2_ADDR( Channel ) + EE_SCI_USART_CTRLB_REG_OFS) |= 
  USART_RXEN_bm;

  /* Resume IRQs */
  EE_hal_resumeIRQ(flags);

#ifdef	__SCI_EXTENDED_STATUS__
  return E_OK;
#endif

}
#endif	/* __SCI_ENABLERX_API__ */
#endif	/* __SCI_DISABLERX_API__ */

#ifdef	__SCI_ENABLENOTIF_API__
/*
 * EE_Sci_EnableNotification() Implementation.
 */
#ifdef	__SCI_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_Sci_EnableNotifications(
  EE_TYPESCICHANNEL	Channel
) {

#ifdef	DEBUG
  volatile EE_UREG		flags;
  volatile EE_UREG		tmp;
  volatile EE_HWREG_PTR		addr;
#if	( \
  defined(__SCI_EXTENDED_STATUS__) || \
  defined( EE_AVR8_USARTC0_RXC_ISR ) || defined( EE_AVR8_USARTC0_TXC_ISR ) || \
  defined( EE_AVR8_USARTD0_RXC_ISR ) || defined( EE_AVR8_USARTD0_TXC_ISR ) \
)
  volatile EE_TYPESCISTATUS	*sw_status;
#endif	/*
	 * __SCI_EXTENDED_STATUS__ ||
	 * EE_AVR8_USARTC0_RXC_ISR || EE_AVR8_USARTC0_TXC_ISR ||
	 * EE_AVR8_USARTD0_RXC_ISR || EE_AVR8_USARTD0_TXC_ISR
	 */
#if	( \
  defined( EE_AVR8_USARTC0_RXC_ISR ) || defined( EE_AVR8_USARTC0_TXC_ISR ) || \
  defined( EE_AVR8_USARTD0_RXC_ISR ) || defined( EE_AVR8_USARTD0_TXC_ISR ) \
)
#ifndef	__AVR_PGMSPACE__
  volatile const EE_TYPESCIPRIO	*prio;
#endif	/* __AVR_PGMSPACE__ */
#endif	/*
	 * EE_AVR8_USARTC0_RXC_ISR || EE_AVR8_USARTC0_TXC_ISR ||
	 * EE_AVR8_USARTD0_RXC_ISR || EE_AVR8_USARTD0_TXC_ISR
	 */
#else	/* DEBUG */
  register EE_UREG		flags;
  register EE_UREG		tmp;
  register EE_HWREG_PTR		addr;
#if	( \
  defined(__SCI_EXTENDED_STATUS__) || \
  defined( EE_AVR8_USARTC0_RXC_ISR ) || defined( EE_AVR8_USARTC0_TXC_ISR ) || \
  defined( EE_AVR8_USARTD0_RXC_ISR ) || defined( EE_AVR8_USARTD0_TXC_ISR ) \
)
  register EE_TYPESCISTATUS	*sw_status;
#endif	/*
	 * __SCI_EXTENDED_STATUS__ ||
	 * EE_AVR8_USARTC0_RXC_ISR || EE_AVR8_USARTC0_TXC_ISR ||
	 * EE_AVR8_USARTD0_RXC_ISR || EE_AVR8_USARTD0_TXC_ISR
	 */
#if	( \
  defined( EE_AVR8_USARTC0_RXC_ISR ) || defined( EE_AVR8_USARTC0_TXC_ISR ) || \
  defined( EE_AVR8_USARTD0_RXC_ISR ) || defined( EE_AVR8_USARTD0_TXC_ISR ) \
)
#ifndef	__AVR_PGMSPACE__
  register const EE_TYPESCIPRIO	*prio;
#endif	/* __AVR_PGMSPACE__ */
#endif	/*
	 * EE_AVR8_USARTC0_RXC_ISR || EE_AVR8_USARTC0_TXC_ISR ||
	 * EE_AVR8_USARTD0_RXC_ISR || EE_AVR8_USARTD0_TXC_ISR
	 */
#endif	/* DEBUG */

#ifdef	__SCI_EXTENDED_STATUS__

  /* Channel Validation. */
  EE_ATXMEGA_VALIDATE_W_RV(
    ( Channel < EE_SCI_MAX_CHANNELS_NUMBER ),
    E_SCI_PARAM_CHANNEL
  );

#endif	/* __SCI_EXTENDED_STATUS__ */

  /* Suspend IRQs */
  flags = EE_hal_suspendIRQ();

#if	( \
  defined(__SCI_EXTENDED_STATUS__) || \
  defined( EE_AVR8_USARTC0_RXC_ISR ) || defined( EE_AVR8_USARTC0_TXC_ISR ) || \
  defined( EE_AVR8_USARTD0_RXC_ISR ) || defined( EE_AVR8_USARTD0_TXC_ISR ) \
)
  sw_status = &EE_Sci_Status[Channel];
#endif	/*
	 * __SCI_EXTENDED_STATUS__ ||
	 * EE_AVR8_USARTC0_RXC_ISR || EE_AVR8_USARTC0_TXC_ISR ||
	 * EE_AVR8_USARTD0_RXC_ISR || EE_AVR8_USARTD0_TXC_ISR
	 */

#if	( \
  defined( EE_AVR8_USARTC0_RXC_ISR ) || defined( EE_AVR8_USARTC0_TXC_ISR ) || \
  defined( EE_AVR8_USARTD0_RXC_ISR ) || defined( EE_AVR8_USARTD0_TXC_ISR ) \
)

#ifndef	__AVR_PGMSPACE__
  prio = &EE_Sci_Irq_Priority[Channel];
#endif	/* __AVR_PGMSPACE__ */

#endif	/*
	 * EE_AVR8_USARTC0_RXC_ISR || EE_AVR8_USARTC0_TXC_ISR ||
	 * EE_AVR8_USARTD0_RXC_ISR || EE_AVR8_USARTD0_TXC_ISR
	 */

#ifdef	__SCI_EXTENDED_STATUS__

  EE_ATXMEGA_VALIDATE_IRQ_W_RV(
    ( sw_status->Flags & EE_SCI_INIT_MASK ),
    E_NOT_OK,
    flags
  );

#if	( \
  defined( EE_AVR8_USARTC0_RXC_ISR ) || defined( EE_AVR8_USARTC0_TXC_ISR ) || \
  defined( EE_AVR8_USARTD0_RXC_ISR ) || defined( EE_AVR8_USARTD0_TXC_ISR ) \
)

  /* Call-Back Validation. */
  EE_ATXMEGA_VALIDATE_W_RV(
    ( 
      ( sw_status->RxCallBack != EE_NULL_PTR ) ||
      ( sw_status->TxCallBack != EE_NULL_PTR )
    ),
    E_SCI_PARAM_CALLBACK
  );

#else

  /* Call-Back Validation. */
  EE_ATXMEGA_VALIDATE_W_RV( EE_FALSE, E_GPIO_PARAM_CALLBACK );

#endif	/*
	 * EE_AVR8_USARTC0_RXC_ISR || EE_AVR8_USARTC0_TXC_ISR ||
	 * EE_AVR8_USARTD0_RXC_ISR || EE_AVR8_USARTD0_TXC_ISR
	 */

#endif	/* __SCI_EXTENDED_STATUS__ */

  addr	= EE_HWREG_ADDR( 
	    EE_SCI_CH_2_ADDR( Channel ) + EE_SCI_USART_CTRLA_REG_OFS
	  );

  tmp = EE_HWREG(addr);

#if	( \
  defined( EE_AVR8_USARTC0_RXC_ISR ) || defined( EE_AVR8_USARTD0_RXC_ISR ) \
)
  if ( sw_status->RxCallBack != EE_NULL_PTR ) {

    tmp &= ~USART_RXCINTLVL_gm;
#ifdef	__AVR_PGMSPACE__
    tmp |= (EE_TYPEPRIO) pgm_read_byte(&EE_Sci_Rx_Irq_Priority[Channel]);
#else	/* __AVR_PGMSPACE__ */
    tmp |= prio->Rx;
#endif	/* __AVR_PGMSPACE__ */

  }
#endif	/* EE_AVR8_USARTC0_RXC_ISR || EE_AVR8_USARTD0_RXC_ISR */

#if	( \
  defined( EE_AVR8_USARTC0_TXC_ISR ) || defined( EE_AVR8_USARTD0_TXC_ISR ) \
)
  if ( sw_status->TxCallBack != EE_NULL_PTR ) {

    tmp &= ~USART_TXCINTLVL_gm;
#ifdef	__AVR_PGMSPACE__
    tmp |= (EE_TYPEPRIO) pgm_read_byte(&EE_Sci_Tx_Irq_Priority[Channel]);
#else	/* __AVR_PGMSPACE__ */
    tmp |= prio->Tx;
#endif	/* __AVR_PGMSPACE__ */

  }
#endif	/* EE_AVR8_USARTC0_TXC_ISR || EE_AVR8_USARTD0_TXC_ISR */

  EE_HWREG(addr) = tmp;

  /* Resume IRQs */
  EE_hal_resumeIRQ(flags);

#ifdef	__SCI_EXTENDED_STATUS__
  return E_OK;
#endif

}

#ifdef	__SCI_DISABLENOTIF_API__
/*
 * EE_Sci_DisableNotification() Implementation.
 */
#ifdef	__SCI_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_Sci_DisableNotifications(
  EE_TYPESCICHANNEL	Channel
) {

#ifdef	DEBUG
  volatile EE_UREG		flags;
#ifdef	__SCI_EXTENDED_STATUS__
  volatile EE_TYPESCISTATUS	*sw_status;
#endif	/* __SCI_EXTENDED_STATUS__ */
#else	/* DEBUG */
  register EE_UREG		flags;
#ifdef	__SCI_EXTENDED_STATUS__
  register EE_TYPESCISTATUS	*sw_status;
#endif	/* __SCI_EXTENDED_STATUS__ */
#endif	/* DEBUG */

#ifdef	__SCI_EXTENDED_STATUS__

  /* Channel Validation. */
  EE_ATXMEGA_VALIDATE_W_RV(
    ( Channel < EE_SCI_MAX_CHANNELS_NUMBER ),
    E_SCI_PARAM_CHANNEL
  );

#endif	/* __SCI_EXTENDED_STATUS__ */

  /* Suspend IRQs */
  flags = EE_hal_suspendIRQ();

#ifdef	__SCI_EXTENDED_STATUS__

  sw_status = &EE_Sci_Status[Channel];

  EE_ATXMEGA_VALIDATE_IRQ_W_RV(
    ( sw_status->Flags & EE_SCI_INIT_MASK ),
    E_NOT_OK,
    flags
  );

#if	( \
  defined( EE_AVR8_USARTC0_RXC_ISR ) || defined( EE_AVR8_USARTC0_TXC_ISR ) || \
  defined( EE_AVR8_USARTD0_RXC_ISR ) || defined( EE_AVR8_USARTD0_TXC_ISR ) \
)
  /* Call-Back Validation. */
  EE_ATXMEGA_VALIDATE_W_RV(
    ( 
      ( sw_status->RxCallBack != EE_NULL_PTR ) ||
      ( sw_status->TxCallBack != EE_NULL_PTR )
    ),
    E_SCI_PARAM_CALLBACK
  );
#else
  /* Call-Back Validation. */
  EE_ATXMEGA_VALIDATE_W_RV( EE_FALSE, E_GPIO_PARAM_CALLBACK );
#endif	/*
	 * EE_AVR8_USARTC0_RXC_ISR || EE_AVR8_USARTC0_TXC_ISR ||
	 * EE_AVR8_USARTD0_RXC_ISR || EE_AVR8_USARTD0_TXC_ISR
	 */

#endif	/* __SCI_EXTENDED_STATUS__ */

  EE_HWREG( EE_SCI_CH_2_ADDR( Channel ) + EE_SCI_USART_CTRLA_REG_OFS ) &=
  ~( USART_RXCINTLVL_gm | USART_TXCINTLVL_gm | USART_DREINTLVL_gm );

  /* Resume IRQs */
  EE_hal_resumeIRQ(flags);

#ifdef	__SCI_EXTENDED_STATUS__
  return E_OK;
#endif

}
#endif	/* __SCI_DISABLENOTIF_API__ */
#endif	/* __SCI_ENABLENOTIF_API__ */
