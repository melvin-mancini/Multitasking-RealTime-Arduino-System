#include "mcu/atmel_atmega328/inc/ee_mcuregs.h"
#include "mcu/atmel_atmega328/inc/ee_uart.h"
#include "ee_internal.h"

#ifdef EE_UART_PORT_1_ISR_ENABLE
static void (*Rx1IsrFunction)(EE_UINT8 data) = NULL;
#endif

#define F_CPU 16000000

//TODO: Check if control flow is present in hw or remove mode
EE_INT8 EE_uart_init(EE_UINT8 port, EE_UINT32 baud, EE_UINT16 byte_format, EE_UINT16 mode)
{
	EE_UINT16	baudregs;
	
	if (port != EE_UART_PORT_1)
		return -EE_UART_ERR_BAD_PORT;
		
	/* Initialize UART Port */
	
	UCSR0A = (1<<U2X0); 					//Double speed mode USART0
	UBRR0L = (uint8_t)(F_CPU/(baud*8L)-1);
	UBRR0H = (F_CPU/(baud*8L)-1) >> 8;

	UCSR0C = (1<<UCSZ00) | (1<<UCSZ01); 	// 1 stop bit, no parity
	UCSR0B = (1<<RXEN0) | (1<<TXEN0);   	// enable tx and rx

	return 1;
}

EE_INT8 EE_uart_close(EE_UINT8 port)
{
	if (port != EE_UART_PORT_1)
		return -EE_UART_ERR_BAD_PORT;
	
	//TODO: disable interrupts?
	UCSR0B = ~((1<<RXEN0) | (1<<TXEN0)); // disable tx and rx 
	
	return 1;
}

EE_INT8 EE_uart_set_rx_callback(EE_UINT8 port, void (*RxFunc)(EE_UINT8 data), EE_UINT8 rxmode)
{
	if (port != EE_UART_PORT_1)
		return -EE_UART_ERR_BAD_PORT;
	
#ifdef EE_UART_PORT_1_ISR_ENABLE
	Rx1IsrFunction = RxFunc;
	
	if (RxFunc) 
	{
		// TODO: (Nino) Initialize UART  ISR
		/*U1STA &= 0x5FFF;		
		U1STA |= rxmode & 0xA000;
		IEC0bits.U1RXIE = 1;		
		IFS0bits.U1RXIF = 0;*/
	}
	return 1;
#else
	return -EE_UART_ERR_INT_DISABLED;
#endif
}

EE_INT8 EE_uart_write_byte(EE_UINT8 port, EE_UINT8 data)
{
	if (port != EE_UART_PORT_1)
		return -EE_UART_ERR_BAD_PORT;
		
	/* Polling mode */

	while ( !( UCSR0A & (1<<UDRE0)) ); // Do nothing until UDR is ready for more data to be written to it
	
	UDR0 = data; // Echo back the received byte back to the computer 
	return 1;
}

EE_INT8 EE_uart_read_byte(EE_UINT8 port, EE_UINT8 *data)
{
	if (port != EE_UART_PORT_1)
		return -EE_UART_ERR_BAD_PORT;
		
#ifdef EE_UART_PORT_1_ISR_ENABLE
	if (Rx1IsrFunction == NULL) 
	{
		/* Polling mode */
#endif
		// TODO: (Nino) Check UART Receive
		if(UCSR0A & (1 << RXC0)) 
		{	// if data have been recieved and is ready to be read from UDR
			*data = UDR0; // Fetch the recieved byte value into the variable "ByteReceived"
			return 1;
		}
		return -EE_UART_ERR_NO_DATA;
#ifdef EE_UART_PORT_1_ISR_ENABLE
	}
	return -EE_UART_ERR_INT_MODE;
#endif
}


#ifdef EE_UART_PORT_1_ISR_ENABLE
ISR2(_U1RXInterrupt)
{
	if (Rx1IsrFunction != NULL) 
	{
		/* Execute callback function */
		// TODO: (Nino) Call function with correct data
		//Rx1IsrFunction(U1RXREG & 0x00FF);
	}
	//IFS0bits.U1RXIF = 0;           
}
#endif
