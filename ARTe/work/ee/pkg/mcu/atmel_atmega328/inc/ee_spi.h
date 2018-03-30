#ifndef __INCLUDE_ATMEL_ATMEGA128_SPI_H__
#define __INCLUDE_ATMEL_ATMEGA128_SPI_H__

#include "ee.h"

//TODO: (Nino) Include registers .h file?

#define EE_SPI_ERR_BAD_PORT		1
#define EE_SPI_ERR_NO_DATA		2
#define EE_SPI_ERR_OVERFLOW		3
#define EE_SPI_ERR_INT_MODE		4
#define EE_SPI_ERR_INT_DISABLED	5

// TODO: (Nino) HW parameters, insert AVR constants here if needed.

/* Gianluca: added these constants to be compatible with dsPic which has two ports.*/
#define EE_SPI_PORT_1 0
#define EE_SPI_PORT_2 1
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

#endif
