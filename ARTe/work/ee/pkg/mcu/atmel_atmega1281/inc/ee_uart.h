#ifndef __INCLUDE_ATMEL_ATMEGA1281_UART_H__
#define __INCLUDE_ATMEL_ATMEGA1281_UART_H__

#include "ee.h"

//TODO: (Nino) Include registers .h file?
//TODO: (Nino) Add support for second port?

// TODO: (Nino) HW parameters, 9bit mode missing!
#define EE_UART_PORT_1	0
#define EE_UART_PORT_2	1

#define EE_UART_TX_ON		0x08
#define EE_UART_RX_ON		0x10

#define EE_UART_PAR_NO		0x00
#define EE_UART_PAR_EVEN	0x20
#define EE_UART_PAR_ODD		0x30

#define EE_UART_BIT_STOP_1	0x00
#define EE_UART_BIT_STOP_2	0x04

#define EE_UART_BIT9		0x06	// SET UCSZ02 in UCSR0B!!!
#define EE_UART_BIT8		0x06
#define EE_UART_BIT7		0x04
#define EE_UART_BIT6		0x02
#define EE_UART_BIT5		0x00



#define EE_UART_ERR_BAD_PORT		1
#define EE_UART_ERR_NO_DATA		2
#define EE_UART_ERR_OVERFLOW		3
#define EE_UART_ERR_INT_MODE		4
#define EE_UART_ERR_INT_DISABLED	5

// TODO: Set the correct value for clock freq
#ifndef EE_UART_INSTRUCTION_CLOCK
#define EE_UART_INSTRUCTION_CLOCK	7370000ul
#endif

// TODO: Add doxygen documentation!
EE_INT8 EE_uart_init(EE_UINT8 port, EE_UINT32 baud, EE_UINT16 byte_format, EE_UINT16 mode);
EE_INT8 EE_uart_close(EE_UINT8 port);
EE_INT8 EE_uart_write_byte(EE_UINT8 port, EE_UINT8 data);
EE_INT8 EE_uart_read_byte(EE_UINT8 port, EE_UINT8 *data);
EE_INT8 EE_uart_set_rx_callback(EE_UINT8 port, void (*RxFunc)(EE_UINT8 data), EE_UINT8 rxmode);

#endif
