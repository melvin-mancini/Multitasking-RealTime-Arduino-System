/* ###*B*###
 * ERIKA Enterprise - a tiny RTOS for small microcontrollers
 *
 * Copyright (C) 2002-2014  Evidence Srl
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
 * Author: 2005 Antonio Romano
 * 	   2014 Giuseppe Serano
 */

#ifndef	__INCLUDE_ATMEL_ATMEGA_UART_H__
#define	__INCLUDE_ATMEL_ATMEGA_UART_H__

#include "ee.h"

/*
 * TODO: (Nino) Include registers .h file?
 * TODO: (Nino) Add support for second port?
 *
 * TODO: (Nino) HW parameters, 9bit mode missing!
 */

#define EE_UART_PORT_1		0x00U
#define EE_UART_PORT_2		0x01U

#define EE_UART_TX_ON		0x08U
#define EE_UART_RX_ON		0x10U

#define EE_UART_PAR_NO		0x00U
#define EE_UART_PAR_EVEN	0x20U
#define EE_UART_PAR_ODD		0x30U

#define EE_UART_BIT_STOP_1	0x00U
#define EE_UART_BIT_STOP_2	0x04U

#define EE_UART_BIT9		0x06U	/* SET UCSZ02 in UCSR0B!!! */
#define EE_UART_BIT8		0x06U
#define EE_UART_BIT7		0x04U
#define EE_UART_BIT6		0x02U
#define EE_UART_BIT5		0x00U

#define EE_UART_BIT8_NO		(EE_UART_BIT8 | EE_UART_PAR_NO)

#define EE_UART_ERR_BAD_PORT		0x01U
#define EE_UART_ERR_NO_DATA		0x02U
#define EE_UART_ERR_OVERFLOW		0x03U
#define EE_UART_ERR_INT_MODE		0x04U
#define EE_UART_ERR_INT_DISABLED	0x05U

/* TODO: Set the correct value for clock freq */
#ifndef EE_UART_INSTRUCTION_CLOCK
#define EE_UART_INSTRUCTION_CLOCK	7370000UL
#endif

/* TODO: Add doxygen documentation! */
EE_INT8
EE_uart_init(
  EE_UINT8	port,
  EE_UINT32	baud,
  EE_UINT16	byte_format,
  EE_UINT16	mode
);

EE_INT8
EE_uart_close(
  EE_UINT8 port
);

EE_INT8
EE_uart_write_byte(
  EE_UINT8	port,
  EE_UINT8	data
);

EE_INT8
EE_uart_read_byte(
  EE_UINT8	port,
  EE_UINT8 *	data
);

EE_INT8
EE_uart_set_rx_callback(
  EE_UINT8	port,
  void (*RxFunc)(EE_UINT8 data),
  EE_UINT8	rxmode
);

#endif	/* __INCLUDE_ATMEL_ATMEGA_UART_H__ */
