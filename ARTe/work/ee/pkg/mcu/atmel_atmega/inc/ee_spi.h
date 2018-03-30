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

#ifndef	__INCLUDE_ATMEL_ATMEGA_SPI_H__
#define	__INCLUDE_ATMEL_ATMEGA_SPI_H__

#include "ee.h"

/*
 * TODO: (Nino) Include registers .h file?
 */

#define EE_SPI_ERR_BAD_PORT	0x01U
#define EE_SPI_ERR_NO_DATA	0x02U
#define EE_SPI_ERR_OVERFLOW	0x03U
#define EE_SPI_ERR_INT_MODE	0x04U
#define EE_SPI_ERR_INT_DISABLED	0x05U

/*
 * TODO: (Nino) HW parameters, insert AVR constants here if needed.
 */

/*
 * Gianluca:	added these constants to be compatible with dsPic which has two
 * 		ports.
 */
#define EE_SPI_PORT_1	0x00U
#define EE_SPI_PORT_2	0x01U
/**
* @brief Initialize the SPI peripheral.
*
* This function initializes the SPI peripheral.
*
* @return 	\todo
*
* @pre		None
*/
EE_INT8 EE_spi_init(void);

EE_INT8 EE_spi_close(void);

/**
* @brief Send a byte through  the SPI peripheral.
*
* This function send a character (8 bits) through the SPI port.
*
* @param[in] data_in	The character to be sent
* @param[out] data_out	\todo
*
* @return 	\todo
*
* @pre		The SPI port must be correctly initialized.
*/
EE_INT8 EE_spi_rw_byte(EE_UINT8 data_in, EE_UINT8 *data_out);

/**
* @brief Send a byte through  the SPI peripheral.
*
* This function send a character (8 bits) through the SPI port.
*
* @param[in] data	The character to be sent
*
* @return 	\todo
*
* @pre		The SPI port must be correctly initialized.
*/
EE_INT8 EE_spi_write_byte(EE_UINT8 data);

/**
* @brief Get a character through the SPI peripheral.
*
* This function send a character (8 bits) through the SPI port.
*
* @return 	\todo
*
* @pre		The SPI port must be correctly initialized.
*/
EE_INT8 EE_spi_read_byte(EE_UINT8 *data);

#endif	/* __INCLUDE_ATMEL_ATMEGA_SPI_H__ */
