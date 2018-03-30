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
 * Erika Enterprise - E2P Driver - Atmel ATXMEGA E2P Driver Implementation.
 *
 * Author: 2013,  Giuseppe Serano
 */

/* ERIKA Enterprise Internals */
#include "ee_internal.h"

/* ERIKA Enterprise IRQs */
#include "ee_irq.h"

#define	EE_E2P_INIT_MASK	0x80U
#define	EE_E2P_WCR_MASK		0x40U
#define	EE_E2P_CONT_MASK	0x20U
#define	EE_E2P_RES_MASK		0x10U
#if	( defined(__E2P_CANCEL_API__) && !defined(__E2P_SYNCHRONOUS__) )
#define	EE_E2P_CANC_MASK	0x08U
#endif	/* __E2P_CANCEL_API__) && !__E2P_SYNCHRONOUS__ */
#define	EE_E2P_BUSY_MASK	0x04U
#define	EE_E2P_OP_MASK		0x03U

#define	EE_E2P_OP_READ		0x00U
#define	EE_E2P_OP_WRITE		0x01U
#define	EE_E2P_OP_CMP		0x02U
#define	EE_E2P_OP_ERASE		0x03U

#define	EE_E2P_FLAGS_MASK	( EE_E2P_FLAG_WCR | EE_E2P_FLAG_POWER )

/*
 * E2P Internal Status Type
 */
#pragma pack(1)
typedef struct {

  EE_TYPEE2PSIZE	Counter;	/* Transfer Data Counter	 */

  EE_TYPEE2PSIZE	Size;		/* Transfer Data Buffer Size	 */

  EE_TYPEE2PDATA *	Data;		/* Transfer Data Buffer Pointer	 */

  EE_ISR_callback	CallBack;	/* Notification Callback.	 */

  EE_UREG		Flags;		/* Status Flags			 */

} EE_TYPEE2PINTSTAT;
#pragma pack()

#ifdef __E2P_WRITE_API__
/*
 * E2P Flush Buffer.
 */
static void EE_E2p_FlushBuffer(void)
{
  /* Flush EEPROM page buffer if necessary. */
  if ( NVM.STATUS & NVM_EELOAD_bm ) {
    NVM.CMD = NVM_CMD_ERASE_EEPROM_BUFFER_gc;
    EE_avr8_WriteCCPReg(&NVM.CTRLA, NVM_CMDEX_bm);
  }
}
#endif	/* __E2P_WRITE_API__ */

/*
 * E2P Status.
 */
#if	(					\
	defined(DEBUG)			||	\
	defined(__E2P_CANCEL_API__)	||	\
	defined(__E2P_SYNCHRONOUS__)		\
)
static volatile EE_TYPEE2PINTSTAT
#else
static EE_TYPEE2PINTSTAT
#endif
#if	0
EE_E2p_Status = { 0 };
#else	/* 0 */
EE_E2p_Status;
#endif	/* 0 */

/* 
 * E2P Set-Up Address.
 */
#if	0
static void EE_E2p_SetAddress( EE_TYPEE2PADDR Address ) {
#else	/* 0 */
__INLINE__ void __ALWAYS_INLINE__ EE_E2p_SetAddress( EE_TYPEE2PADDR Address ) {
#endif	/* 0 */
  NVM.ADDR0 = (Address & 0xFFU);
  NVM.ADDR1 = (Address >> 8);
  NVM.ADDR2 = 0x00;
}

/* 
 * E2P Increment Address.
 */
#if	1
static void EE_E2p_IncAddress( void ) {
#else	/* 0 */
__INLINE__ void __ALWAYS_INLINE__ EE_E2p_IncAddress( void ) {
#endif	/* 0 */
#if	0
  NVM.ADDR0++;
  if ( !NVM.ADDR0 ) {
    NVM.ADDR1++;
  }
#else	/* 0 */
  if ( !(++NVM.ADDR0) ) {
    ++NVM.ADDR1;
  }
#endif	/* 0 */
}

/* 
 * E2P Internal Handler Implementation.
 */
static void EE_E2p_Internal_Handler( void )
{

#ifdef	DEBUG
  volatile EE_UREG	flags;
#else
  register EE_UREG	flags;
#endif

  /* Suspend Interrupt */
  flags = EE_hal_suspendIRQ();

#if	( defined(__E2P_INTERRUPT__) && defined(EE_AVR8_NVM_EE_ISR) )
  NVM.INTCTRL = EE_ISR_PRI_OFF;
#endif	/* __E2P_INTERRUPT__ && EE_AVR8_NVM_EE_ISR */

#if	( defined(__E2P_CANCEL_API__) && !defined(__E2P_SYNCHRONOUS__) )
  if ( EE_E2p_Status.Flags & EE_E2P_CANC_MASK ) {

    EE_E2p_Status.Flags &= ~(
      EE_E2P_CONT_MASK | EE_E2P_RES_MASK | EE_E2P_BUSY_MASK
    );

  }
  else
#endif	/* __E2P_CANCEL_API__ && !__E2P_SYNCHRONOUS__ */

  if ( EE_E2p_Status.Counter < EE_E2p_Status.Size ) {

    switch ( EE_E2p_Status.Flags & EE_E2P_OP_MASK ) {

#ifdef __E2P_READ_API__
      case EE_E2P_OP_READ:
	EE_E2p_Status.Data[EE_E2p_Status.Counter] = NVM.DATA0;
	EE_E2p_IncAddress();
#if	0
	EE_E2p_Status.Counter++;
#else	/* 0 */
	++EE_E2p_Status.Counter;
#endif	/* 0 */
	break;
#endif /* __E2P_READ_API__ */

#ifdef __E2P_WRITE_API__
      case EE_E2P_OP_WRITE:
#if	0
	if ( EE_E2p_Status.Flags & EE_E2P_WCR_MASK ) {
	  if ( EE_E2p_Status.Flags & EE_E2P_CONT_MASK ) {
	    /* Issue EEPROM Read command. */
	    NVM.CMD = NVM_CMD_READ_EEPROM_gc;
	    EE_E2p_Status.Flags &= ~EE_E2P_CONT_MASK;
	  }
	  else {
	    if ( NVM.DATA0 != EE_E2p_Status.Data[EE_E2p_Status.Counter] ) {
	      /*  Flush buffer to make sure no unintetional data is written and
	       *  load the "Page Load" command into the command register.
	       */
	      EE_E2p_FlushBuffer();
	      NVM.CMD = NVM_CMD_LOAD_EEPROM_BUFFER_gc;
	      /* 
	       * Load data to write, which triggers the loading of EEPROM page
	       * buffer.
	       */
	      NVM.DATA0 = EE_E2p_Status.Data[EE_E2p_Status.Counter];
	      /*
	       * Issue EEPROM Atomic Write (Erase&Write) command.
	       * Load command, write the protection signature and execute
	       * command.
	       */
	      NVM.CMD = NVM_CMD_ERASE_WRITE_EEPROM_PAGE_gc;
	      EE_E2p_Status.Flags |= EE_E2P_CONT_MASK;
	    }
	    EE_E2p_IncAddress();
#if	0
	    EE_E2p_Status.Counter++;
#else	/* 0 */
	    ++EE_E2p_Status.Counter;
#endif	/* 0 */
	  }
	}
	else {
#endif	/* 0 */
	  /*  Flush buffer to make sure no unintetional data is written and
	   *  load the "Page Load" command into the command register.
	   */
	  EE_E2p_FlushBuffer();
	  EE_E2p_IncAddress();
	  NVM.CMD = NVM_CMD_LOAD_EEPROM_BUFFER_gc;
	  /* 
	   * Load data to write, which triggers the loading of EEPROM page
	   * buffer.
	   */
	  NVM.DATA0 = EE_E2p_Status.Data[EE_E2p_Status.Counter];
	  /*
	   * Issue EEPROM Atomic Write (Erase&Write) command.
	   * Load command, write the protection signature and execute command.
	   */
	  NVM.CMD = NVM_CMD_ERASE_WRITE_EEPROM_PAGE_gc;
#if	0
	  EE_E2p_Status.Counter++;
#else	/* 0 */
	  ++EE_E2p_Status.Counter;
#endif	/* 0 */
#if	0
	}
#endif	/* 0 */
	break;
#endif	/* __E2P_WRITE_API__ */

#ifdef __E2P_COMPARE_API__
      case EE_E2P_OP_CMP:
	if ( NVM.DATA0 != EE_E2p_Status.Data[EE_E2p_Status.Counter] ) {
	  EE_E2p_Status.Flags &= ~( 
	    EE_E2P_CONT_MASK	|
#if	( defined(__E2P_CANCEL_API__) && !defined(__E2P_SYNCHRONOUS__) )
	    EE_E2P_CANC_MASK	|
#endif	/* __E2P_CANCEL_API__ && !__E2P_SYNCHRONOUS__ */
	    EE_E2P_BUSY_MASK
	  );
	  EE_E2p_Status.Flags |= EE_E2P_RES_MASK;
	}
	else {
	  EE_E2p_IncAddress();
#if	0
	  EE_E2p_Status.Counter++;
#else	/* 0 */
	  ++EE_E2p_Status.Counter;
#endif	/* 0 */
	}
	break;
#endif	/* __E2P_COMPARE_API__ */

#ifdef __E2P_ERASE_API__
      case EE_E2P_OP_ERASE:
	EE_E2p_IncAddress();
	EE_E2p_Status.Counter++;
	break;
#endif	/* __E2P_ERASE_API__ */

    };

  }
  else {

    EE_E2p_Status.Flags &= ~( 
      EE_E2P_CONT_MASK	|
      EE_E2P_RES_MASK	|
#if	( defined(__E2P_CANCEL_API__) && !defined(__E2P_SYNCHRONOUS__) )
      EE_E2P_CANC_MASK	|
#endif	/* __E2P_CANCEL_API__ && !__E2P_SYNCHRONOUS__ */
      EE_E2P_BUSY_MASK
    );

  }

  if ( EE_E2p_Status.Flags & EE_E2P_BUSY_MASK ) {

    EE_avr8_WriteCCPReg(&NVM.CTRLA, NVM_CMDEX_bm);

#if	( defined(__E2P_INTERRUPT__) && defined(EE_AVR8_NVM_EE_ISR) )
#ifdef	EE_AVR8_NVM_EE_ISR_PRI
    NVM.INTCTRL = EE_AVR8_NVM_EE_ISR_PRI;
#else
    NVM.INTCTRL = EE_ISR_PRI_OFF;
#endif
#endif	/* __E2P_INTERRUPT__ && EE_AVR8_NVM_EE_ISR */

  }
  else if ( EE_E2p_Status.CallBack != EE_NULL_PTR ) {

    /* Resume Interrupt */
    EE_hal_resumeIRQ(flags);

    /* Call Port Pin CallBack. */
    (EE_E2p_Status.CallBack)();

    /* Suspend Interrupt */
    flags = EE_hal_suspendIRQ();

  }

  /* Resume Interrupt */
  EE_hal_resumeIRQ(flags);

}	/* EE_E2p_Internal_Handler() */

#if	( defined(__E2P_CANCEL_API__) || defined(__E2P_SYNCHRONOUS__) )
/*
 * E2P Busy Wait.
 */
static void EE_E2p_BusyWait( void )
{

#ifdef	DEBUG
  volatile EE_UREG	flags;
  volatile EE_UREG	stat;
#else	/* DEBUG */
  register EE_UREG	flags;
  register EE_UREG	stat;
#endif	/* DEBUG */

  do {

    /* Suspend IRQs */
    flags = EE_hal_suspendIRQ();

#ifndef	__E2P_INTERRUPT__
    if ( !(NVM.STATUS & NVM_NVMBUSY_bm) ) {

      /* Resume IRQs */
      EE_hal_resumeIRQ(flags);

      EE_E2p_Internal_Handler();

      /* Suspend IRQs */
      flags = EE_hal_suspendIRQ();

    }
#endif	/* __E2P_INTERRUPT__ */

    stat = EE_E2p_Status.Flags;

    /* Resume IRQs */
    EE_hal_resumeIRQ(flags);

  } while ( stat & EE_E2P_BUSY_MASK );

}
#endif	/* __E2P_CANCEL_API__ || __E2P_SYNCHRONOUS__ */

#if	( defined(__E2P_INTERRUPT__) && defined(EE_AVR8_NVM_EE_ISR) )
/* 
 * E2P ISR
 */
#if	( EE_AVR8_NVM_EE_ISR_CAT == 1 )
ISR1(EE_AVR8_NVM_EE_ISR)
#else
ISR2(EE_AVR8_NVM_EE_ISR)
#endif
{ EE_E2p_Internal_Handler(); }
#endif	/* __E2P_INTERRUPT__ && EE_AVR8_NVM_EE_ISR */

#ifdef	__E2P_GETSTATUS_API__
EE_TYPEE2PSTAT EE_E2p_GetStatus( void )
{
  #ifdef	DEBUG
  volatile EE_UREG		flags;
  volatile EE_UREG		stat;
  volatile EE_TYPESCICHSTAT	ret;
#else
  register EE_UREG		flags;
  register EE_UREG		stat;
  register EE_TYPESCICHSTAT	ret;
#endif

  /* Suspend Interrupt */
  flags = EE_hal_suspendIRQ();

  stat = EE_E2p_Status.Flags;

  /* Resume Interrupt */
  EE_hal_resumeIRQ(flags);

  ret = EE_E2P_OPERATIONAL;

  /* Initialization Validation */
  if ( stat & EE_E2P_INIT_MASK ) {

    ret = EE_E2P_UNINIT;

  }
  else if ( stat & EE_E2P_BUSY_MASK ) {

    ret = EE_E2P_BUSY;

  }
#if	( defined(__E2P_CANCEL_API__) && !defined(__E2P_SYNCHRONOUS__) )
  else if ( stat & EE_E2P_CANC_MASK ) {

    ret = EE_E2P_CANCELLED;

  }
#endif	/* __E2P_CANCEL_API__ && !__E2P_SYNCHRONOUS__ */
  else if ( stat & EE_E2P_RES_MASK ) {

    ret = EE_E2P_ERROR;

  }

  return ret;
}
#endif	/* __E2P_GETSTATUS_API__ */

/*
 * EE_E2p_Init() Implementation.
 */
#ifdef	__E2P_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_E2p_Init(
  EE_TYPEE2PFLAGS	Flags,
  EE_ISR_callback	NotifCB
) {

#ifdef	DEBUG
  volatile EE_UREG	flags;
#else	/* DEBUG */
  register EE_UREG	flags;
#endif	/* DEBUG */

#ifdef	__E2P_EXTENDED_STATUS__

  /* Flags Validation. */
  EE_ATXMEGA_VALIDATE_W_RV( ( Flags & ~EE_E2P_FLAGS_MASK ), E_E2P_PARAM_FLAGS );

#endif	/* __E2P_EXTENDED_STATUS__ */

  /* Suspend IRQs */
  flags = EE_hal_suspendIRQ();

#ifdef	__E2P_EXTENDED_STATUS__

  EE_ATXMEGA_VALIDATE_IRQ_W_RV(
    !( EE_E2p_Status.Flags & EE_E2P_INIT_MASK ),
    E_NOT_OK,
    flags
  );

#endif	/* __E2P_EXTENDED_STATUS__ */

  EE_E2p_Status.Flags = EE_E2P_INIT_MASK;

#if	0
  if ( Flags & EE_E2P_FLAG_WCR ) {

    /* Set Write Cycle Reduction Flag */
    EE_E2p_Status.Flags |= EE_E2P_WCR_MASK;

  }
#endif	/* 0 */

  if ( Flags & EE_E2P_FLAG_POWER ) {

    /* Set Power Reduction Mode */
    NVM.CTRLB |= NVM_EPRM_bm;

  }

#if	( defined(__E2P_INTERRUPT__) && defined(EE_AVR8_NVM_EE_ISR) )
  NVM.INTCTRL = EE_ISR_PRI_OFF;
#endif	/* __E2P_INTERRUPT__ && EE_AVR8_NVM_EE_ISR */

  EE_E2p_Status.CallBack = NotifCB;

  /* Resume IRQs */
  EE_hal_resumeIRQ(flags);

#ifdef	__E2P_EXTENDED_STATUS__
  return E_OK;
#endif	/* __E2P_EXTENDED_STATUS__ */

}

#ifdef	__E2P_DEINIT_API__
/*
 * EE_E2p_DeInit() Implementation.
 */
#ifdef	__E2P_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_E2p_DeInit(
  EE_TYPEE2PFLAGS	Flags
) {

#ifdef	DEBUG
  volatile EE_UREG	flags;
#else	/* DEBUG */
  register EE_UREG	flags;
#endif	/* DEBUG */

#ifdef	__E2P_EXTENDED_STATUS__

  /* Flags Validation. */
  EE_ATXMEGA_VALIDATE_W_RV(
    ( Flags == EE_E2P_FLAG_DEFAULT ),
    E_E2P_PARAM_FLAGS
  );

#endif	/* __E2P_EXTENDED_STATUS__ */

  /* Suspend IRQs */
  flags = EE_hal_suspendIRQ();

#ifdef	__E2P_EXTENDED_STATUS__

  EE_ATXMEGA_VALIDATE_IRQ_W_RV(
    ( EE_E2p_Status.Flags & EE_E2P_INIT_MASK ),
    E_NOT_OK,
    flags
  );

#endif	/* __E2P_EXTENDED_STATUS__ */

  EE_E2p_Status.Flags = EE_OFF;

  /* Clear Power Reduction Mode */
  NVM.CTRLB &= ~NVM_EPRM_bm;

#if	( defined(__E2P_INTERRUPT__) && defined(EE_AVR8_NVM_EE_ISR) )
  NVM.INTCTRL = EE_ISR_PRI_OFF;
#endif	/* __E2P_INTERRUPT__ && EE_AVR8_NVM_EE_ISR */

  EE_E2p_Status.CallBack = EE_NULL_PTR;

  /* Resume IRQs */
  EE_hal_resumeIRQ(flags);

#ifdef	__E2P_EXTENDED_STATUS__
  return E_OK;
#endif

}
#endif	/* __E2P_DEINIT_API__ */

#ifdef	__E2P_READ_API__
/*
 * EE_E2p_Read() Implementation.
 */
#ifdef	__E2P_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_E2p_Read(
  EE_TYPEE2PADDR	Address,
  EE_TYPEE2PDATA	*BufferPtr,
  EE_TYPEE2PSIZE	Size
) {

#ifdef	DEBUG
  volatile EE_UREG	flags;
#else	/* DEBUG */
  register EE_UREG	flags;
#endif	/* DEBUG */

#ifdef	__E2P_EXTENDED_STATUS__

  /* Address Validation. */
  EE_ATXMEGA_VALIDATE_W_RV( ( Address < EE_E2P_SIZE ), E_E2P_PARAM_ADDRESS );

  /* BufferPtr Validation. */
  EE_ATXMEGA_VALIDATE_W_RV( ( BufferPtr != EE_NULL_PTR ), E_E2P_PARAM_NULL );

  /* Size Validation. */
  EE_ATXMEGA_VALIDATE_W_RV( 
    ( ( Size != 0x0000U ) && ( Size <= ( EE_E2P_SIZE - Address ) ) ),
    E_E2P_PARAM_SIZE
  );

#endif	/* __E2P_EXTENDED_STATUS__ */

  /* Suspend IRQs */
  flags = EE_hal_suspendIRQ();

#ifdef	__E2P_EXTENDED_STATUS__

  EE_ATXMEGA_VALIDATE_IRQ_W_RV(
    ( EE_E2p_Status.Flags & EE_E2P_INIT_MASK ),
    E_NOT_OK,
    flags
  );

  EE_ATXMEGA_VALIDATE_IRQ_W_RV(
    !( EE_E2p_Status.Flags & EE_E2P_BUSY_MASK ),
    E_E2P_BUSY,
    flags
  );

#endif	/* __E2P_EXTENDED_STATUS__ */

  EE_E2p_Status.Flags &= ~(
    EE_E2P_CONT_MASK	|
    EE_E2P_RES_MASK	|
#if	( defined(__E2P_CANCEL_API__) && !defined(__E2P_SYNCHRONOUS__) )
    EE_E2P_CANC_MASK	|
#endif	/* __E2P_CANCEL_API__ && !__E2P_SYNCHRONOUS__ */
    EE_E2P_OP_MASK
  );
  EE_E2p_Status.Flags |= ( EE_E2P_BUSY_MASK | EE_E2P_OP_READ );

  EE_E2p_Status.Data = BufferPtr;
  EE_E2p_Status.Size = Size;
  EE_E2p_Status.Counter = 0x0000U;

  /* Set address. */
  EE_E2p_SetAddress( Address );

  /* Issue EEPROM Read command. */
  NVM.CMD = NVM_CMD_READ_EEPROM_gc;

  EE_avr8_WriteCCPReg(&NVM.CTRLA, NVM_CMDEX_bm);

#if	( defined(__E2P_INTERRUPT__) && defined(EE_AVR8_NVM_EE_ISR) )
#ifdef	EE_AVR8_NVM_EE_ISR_PRI
  NVM.INTCTRL = EE_AVR8_NVM_EE_ISR_PRI;
#else
  NVM.INTCTRL = EE_ISR_PRI_OFF;
#endif
#endif	/* __E2P_INTERRUPT__ && EE_AVR8_NVM_EE_ISR */

  /* Resume IRQs */
  EE_hal_resumeIRQ(flags);

#ifdef	__E2P_SYNCHRONOUS__
  EE_E2p_BusyWait();
#endif	/* __E2P_SYNCHRONOUS__ */

#ifdef	__E2P_EXTENDED_STATUS__
  return E_OK;
#endif	/* __E2P_EXTENDED_STATUS__ */

}
#endif	/* __E2P_READ_API__ */

#ifdef	__E2P_WRITE_API__
/*
 * EE_E2p_Write() Implementation.
 */
#ifdef	__E2P_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_E2p_Write(
  EE_TYPEE2PADDR	Address,
  EE_TYPEE2PDATA	*BufferPtr,
  EE_TYPEE2PSIZE	Size
) {

#ifdef	DEBUG
  volatile EE_UREG	flags;
#else	/* DEBUG */
  register EE_UREG	flags;
#endif	/* DEBUG */

#ifdef	__E2P_EXTENDED_STATUS__

  /* Address Validation. */
  EE_ATXMEGA_VALIDATE_W_RV( ( Address < EE_E2P_SIZE ), E_E2P_PARAM_ADDRESS );

  /* BufferPtr Validation. */
  EE_ATXMEGA_VALIDATE_W_RV( ( BufferPtr != EE_NULL_PTR ), E_E2P_PARAM_NULL );

  /* Size Validation. */
  EE_ATXMEGA_VALIDATE_W_RV( 
    ( ( Size != 0x0000U ) && ( Size <= ( EE_E2P_SIZE - Address ) ) ),
    E_E2P_PARAM_SIZE
  );

#endif	/* __E2P_EXTENDED_STATUS__ */

  /* Suspend IRQs */
  flags = EE_hal_suspendIRQ();

#ifdef	__E2P_EXTENDED_STATUS__

  EE_ATXMEGA_VALIDATE_IRQ_W_RV(
    ( EE_E2p_Status.Flags & EE_E2P_INIT_MASK ),
    E_NOT_OK,
    flags
  );

  EE_ATXMEGA_VALIDATE_IRQ_W_RV(
    !( EE_E2p_Status.Flags & EE_E2P_BUSY_MASK ),
    E_E2P_BUSY,
    flags
  );

#endif	/* __E2P_EXTENDED_STATUS__ */

  EE_E2p_Status.Flags &= ~(
    EE_E2P_CONT_MASK	|
    EE_E2P_RES_MASK	|
#if	( defined(__E2P_CANCEL_API__) && !defined(__E2P_SYNCHRONOUS__) )
    EE_E2P_CANC_MASK	|
#endif	/* __E2P_CANCEL_API__ && !__E2P_SYNCHRONOUS__ */
    EE_E2P_OP_MASK
  );
  EE_E2p_Status.Flags |= ( EE_E2P_BUSY_MASK | EE_E2P_OP_WRITE );

  EE_E2p_Status.Data = BufferPtr;
  EE_E2p_Status.Size = Size;
  EE_E2p_Status.Counter = 0x0000U;

#if	0
  if ( EE_E2p_Status.Flags & EE_E2P_WCR_MASK ) {
    /* Set address. */
    EE_E2p_SetAddress( Address );
    /* Issue EEPROM Read command. */
    NVM.CMD = NVM_CMD_READ_EEPROM_gc;
  }
  else {
#endif	/* 0 */
    /*  Flush buffer to make sure no unintetional data is written and
     *  load the "Page Load" command into the command register.
     */
    EE_E2p_FlushBuffer();
    /* Set address. */
    EE_E2p_SetAddress( Address );
    NVM.CMD = NVM_CMD_LOAD_EEPROM_BUFFER_gc;
    /* 
     * Load data to write, which triggers the loading of EEPROM page
     * buffer.
     */
    NVM.DATA0 = EE_E2p_Status.Data[EE_E2p_Status.Counter];
    /*
     * Issue EEPROM Atomic Write (Erase&Write) command.
     * Load command, write the protection signature and execute command.
     */
    NVM.CMD = NVM_CMD_ERASE_WRITE_EEPROM_PAGE_gc;
#if	0
    EE_E2p_Status.Counter++;
#else	/* 0 */
    ++EE_E2p_Status.Counter;
#endif	/* 0 */
#if	0
  }
#endif	/* 0 */

  EE_avr8_WriteCCPReg(&NVM.CTRLA, NVM_CMDEX_bm);

#if	( defined(__E2P_INTERRUPT__) && defined(EE_AVR8_NVM_EE_ISR) )
#ifdef	EE_AVR8_NVM_EE_ISR_PRI
  NVM.INTCTRL = EE_AVR8_NVM_EE_ISR_PRI;
#else
  NVM.INTCTRL = EE_ISR_PRI_OFF;
#endif
#endif	/* __E2P_INTERRUPT__ && EE_AVR8_NVM_EE_ISR */

  /* Resume IRQs */
  EE_hal_resumeIRQ(flags);

#ifdef	__E2P_SYNCHRONOUS__
  EE_E2p_BusyWait();
#endif	/* __E2P_SYNCHRONOUS__ */

#ifdef	__E2P_EXTENDED_STATUS__
  return E_OK;
#endif	/* __E2P_EXTENDED_STATUS__ */

}
#endif	/* __E2P_WRITE_API__ */

#ifdef	__E2P_ERASE_API__
/*
 * EE_E2p_Erase() Implementation.
 */
#ifdef	__E2P_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_E2p_Erase(
  EE_TYPEE2PADDR	Address,
  EE_TYPEE2PSIZE	Size
) {

#ifdef	DEBUG
  volatile EE_UREG	flags;
#else	/* DEBUG */
  register EE_UREG	flags;
#endif	/* DEBUG */

#ifdef	__E2P_EXTENDED_STATUS__

  /* Address Validation. */
  EE_ATXMEGA_VALIDATE_W_RV( ( Address < EE_E2P_SIZE ), E_E2P_PARAM_ADDRESS );

  /* Size Validation. */
  EE_ATXMEGA_VALIDATE_W_RV( 
    ( ( Size != 0x0000U ) && ( Size <= ( EE_E2P_SIZE - Address ) ) ),
    E_E2P_PARAM_SIZE
  );

#endif	/* __E2P_EXTENDED_STATUS__ */

  /* Suspend IRQs */
  flags = EE_hal_suspendIRQ();

#ifdef	__E2P_EXTENDED_STATUS__

  EE_ATXMEGA_VALIDATE_IRQ_W_RV(
    ( EE_E2p_Status.Flags & EE_E2P_INIT_MASK ),
    E_NOT_OK,
    flags
  );

  EE_ATXMEGA_VALIDATE_IRQ_W_RV(
    !( EE_E2p_Status.Flags & EE_E2P_BUSY_MASK ),
    E_E2P_BUSY,
    flags
  );

#endif	/* __E2P_EXTENDED_STATUS__ */

  EE_E2p_Status.Flags &= ~(
    EE_E2P_CONT_MASK	|
    EE_E2P_RES_MASK	|
#if	( defined(__E2P_CANCEL_API__) && !defined(__E2P_SYNCHRONOUS__) )
    EE_E2P_CANC_MASK	|
#endif	/* __E2P_CANCEL_API__ && !__E2P_SYNCHRONOUS__ */
    EE_E2P_OP_MASK
  );
  EE_E2p_Status.Flags |= ( EE_E2P_BUSY_MASK | EE_E2P_OP_ERASE );

  EE_E2p_Status.Size = Size;

  /* Set address. */
  EE_E2p_SetAddress( Address );

  if ( !Address && (Size == EE_E2P_SIZE) ) {
    EE_E2p_Status.Counter = Size;
    /* Issue EEPROM Erase All command. */
    NVM.CMD = NVM_CMD_ERASE_EEPROM_gc;
  }
  else {
    EE_E2p_Status.Counter = 0x0000U;
    /* Issue EEPROM Erase command. */
    NVM.CMD = NVM_CMD_ERASE_EEPROM_PAGE_gc;
  }

  EE_avr8_WriteCCPReg(&NVM.CTRLA, NVM_CMDEX_bm);

#if	( defined(__E2P_INTERRUPT__) && defined(EE_AVR8_NVM_EE_ISR) )
#ifdef	EE_AVR8_NVM_EE_ISR_PRI
  NVM.INTCTRL = EE_AVR8_NVM_EE_ISR_PRI;
#else
  NVM.INTCTRL = EE_ISR_PRI_OFF;
#endif
#endif	/* __E2P_INTERRUPT__ && EE_AVR8_NVM_EE_ISR */

  /* Resume IRQs */
  EE_hal_resumeIRQ(flags);

#ifdef	__E2P_SYNCHRONOUS__
  EE_E2p_BusyWait();
#endif	/* __E2P_SYNCHRONOUS__ */

#ifdef	__E2P_EXTENDED_STATUS__
  return E_OK;
#endif	/* __E2P_EXTENDED_STATUS__ */

}
#endif	/* __E2P_ERASE_API__ */

#ifdef	__E2P_COMPARE_API__
/*
 * EE_E2p_Compare() Implementation.
 */
#ifdef	__E2P_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_E2p_Compare(
  EE_TYPEE2PADDR	Address,
  EE_TYPEE2PDATA	*BufferPtr,
  EE_TYPEE2PSIZE	Size
) {

#ifdef	DEBUG
  volatile EE_UREG	flags;
#else	/* DEBUG */
  register EE_UREG	flags;
#endif	/* DEBUG */

#ifdef	__E2P_EXTENDED_STATUS__

  /* Address Validation. */
  EE_ATXMEGA_VALIDATE_W_RV( ( Address < EE_E2P_SIZE ), E_E2P_PARAM_ADDRESS );

  /* BufferPtr Validation. */
  EE_ATXMEGA_VALIDATE_W_RV( ( BufferPtr != EE_NULL_PTR ), E_E2P_PARAM_NULL );

  /* Size Validation. */
  EE_ATXMEGA_VALIDATE_W_RV( 
    ( ( Size != 0x0000U ) && ( Size <= ( EE_E2P_SIZE - Address ) ) ),
    E_E2P_PARAM_SIZE
  );

#endif	/* __E2P_EXTENDED_STATUS__ */

  /* Suspend IRQs */
  flags = EE_hal_suspendIRQ();

#ifdef	__E2P_EXTENDED_STATUS__

  EE_ATXMEGA_VALIDATE_IRQ_W_RV(
    ( EE_E2p_Status.Flags & EE_E2P_INIT_MASK ),
    E_NOT_OK,
    flags
  );

  EE_ATXMEGA_VALIDATE_IRQ_W_RV(
    !( EE_E2p_Status.Flags & EE_E2P_BUSY_MASK ),
    E_E2P_BUSY,
    flags
  );

#endif	/* __E2P_EXTENDED_STATUS__ */

  EE_E2p_Status.Flags &= ~(
    EE_E2P_CONT_MASK	|
    EE_E2P_RES_MASK	|
#if	( defined(__E2P_CANCEL_API__) && !defined(__E2P_SYNCHRONOUS__) )
    EE_E2P_CANC_MASK	|
#endif	/* __E2P_CANCEL_API__ && !__E2P_SYNCHRONOUS__ */
    EE_E2P_OP_MASK
  );
  EE_E2p_Status.Flags |= ( EE_E2P_BUSY_MASK | EE_E2P_OP_CMP );

  EE_E2p_Status.Data = BufferPtr;
  EE_E2p_Status.Size = Size;
  EE_E2p_Status.Counter = 0x0000U;

  /* Set address. */
  EE_E2p_SetAddress( Address );

  /* Issue EEPROM Read command. */
  NVM.CMD = NVM_CMD_READ_EEPROM_gc;

  EE_avr8_WriteCCPReg(&NVM.CTRLA, NVM_CMDEX_bm);

#if	( defined(__E2P_INTERRUPT__) && defined(EE_AVR8_NVM_EE_ISR) )
#ifdef	EE_AVR8_NVM_EE_ISR_PRI
  NVM.INTCTRL = EE_AVR8_NVM_EE_ISR_PRI;
#else
  NVM.INTCTRL = EE_ISR_PRI_OFF;
#endif
#endif	/* __E2P_INTERRUPT__ && EE_AVR8_NVM_EE_ISR */

  /* Resume IRQs */
  EE_hal_resumeIRQ(flags);

#ifdef	__E2P_SYNCHRONOUS__
  EE_E2p_BusyWait();
#endif	/* __E2P_SYNCHRONOUS__ */

#ifdef	__E2P_EXTENDED_STATUS__
  return E_OK;
#endif	/* __E2P_EXTENDED_STATUS__ */

}
#endif	/* __E2P_COMPARE_API__ */

#if	( defined(__E2P_CANCEL_API__) && !defined(__E2P_SYNCHRONOUS__) )
/*
 * EE_E2p_Cancel() Implementation.
 */
#ifdef	__E2P_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_E2p_Cancel(
  void
) {

#ifdef	DEBUG
  volatile EE_UREG	flags;
#else	/* DEBUG */
  register EE_UREG	flags;
#endif	/* DEBUG */

  /* Suspend IRQs */
  flags = EE_hal_suspendIRQ();

#ifdef	__E2P_EXTENDED_STATUS__

  EE_ATXMEGA_VALIDATE_IRQ_W_RV(
    ( EE_E2p_Status.Flags & EE_E2P_INIT_MASK ),
    E_NOT_OK,
    flags
  );

#endif	/* __E2P_EXTENDED_STATUS__ */

  if ( EE_E2p_Status.Flags & EE_E2P_BUSY_MASK ) {

    EE_E2p_Status.Flags |= EE_E2P_CANC_MASK;

  }

  /* Resume IRQs */
  EE_hal_resumeIRQ(flags);

  EE_E2p_BusyWait();

#ifdef	__E2P_EXTENDED_STATUS__
  return E_OK;
#endif	/* __E2P_EXTENDED_STATUS__ */

}
#endif	/* __E2P_CANCEL_API__ && !__E2P_SYNCHRONOUS__ */

#if	( !defined(__E2P_INTERRUPT__) && !defined(__E2P_SYNCHRONOUS__) )
void EE_E2p_MainFunction( void )
{
#ifdef	DEBUG
  volatile EE_UREG	flags;
  volatile EE_UREG	sw_stat;
  volatile EE_UREG	hw_stat;
#else	/* DEBUG */
  register EE_UREG	flags;
  register EE_UREG	sw_stat;
  register EE_UREG	hw_stat;
#endif	/* DEBUG */

  /* Suspend IRQs */
  flags = EE_hal_suspendIRQ();

  sw_stat = EE_E2p_Status.Flags;

  hw_stat = NVM.STATUS;

  /* Resume IRQs */
  EE_hal_resumeIRQ(flags);

  if (
    ( sw_stat & EE_E2P_INIT_MASK ) &&
    ( sw_stat & EE_E2P_BUSY_MASK ) &&
    !(hw_stat & NVM_NVMBUSY_bm)
  ) {

    EE_E2p_Internal_Handler();

  }

}
#endif	/* !__E2P_INTERRUPT__ && !__E2P_SYNCHRONOUS__ */
