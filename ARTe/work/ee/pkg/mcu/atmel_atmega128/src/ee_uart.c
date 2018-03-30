#include "mcu/atmel_atmega128/inc/ee_mcuregs.h"
#include "mcu/atmel_atmega128/inc/ee_uart.h"
#include "ee_internal.h"

#ifdef EE_UART_PORT_1_ISR_ENABLE
static void (*Rx1IsrFunction)(EE_UINT8 data) = NULL;
#endif

//TODO: Check if control flow is present in hw or remove mode
EE_INT8 EE_uart_init(EE_UINT8 port, EE_UINT32 baud, EE_UINT16 byte_format, EE_UINT16 mode)
{
	EE_UINT16	baudregs;
	
	if (port != EE_UART_PORT_1)
		return -EE_UART_ERR_BAD_PORT;
		
	/* Initialize UART Port */
	
	EE_reg(UCSR0A) |= 0x02;   // Enable double speed mode (Allow it to be selected?)

	EE_reg(UCSR0B) |= EE_UART_TX_ON | EE_UART_RX_ON;   // Turn on the transmission and reception circuitry
	
	EE_reg(UCSR0C) |= (byte_format & 0x4E);	// Set this? Missing setting for 9bit mode!!!
	
	baudregs = (((EE_UART_INSTRUCTION_CLOCK / (baud * 8UL))) - 1); 
	EE_reg(UBRR0L) = (baudregs & 0x00FF);	// Load lower 8-bits of the baud rate value into the low byte of the UBRR register
	EE_reg(UBRR0H) = (baudregs >> 8);		// Load upper 8-bits of the baud rate value into the high byte of the UBRR register 
	
	return 1;
}

EE_INT8 EE_uart_close(EE_UINT8 port)
{
	if (port != EE_UART_PORT_1)
		return -EE_UART_ERR_BAD_PORT;
	
	/* chris: TODO: Release something */
	return 1;
}

EE_INT8 EE_uart_set_rx_callback(EE_UINT8 port, void (*RxFunc)(EE_UINT8 data), EE_UINT8 rxmode)
{
	if (port != EE_UART_PORT_1)
		return -EE_UART_ERR_BAD_PORT;
	
#ifdef EE_UART_PORT_1_ISR_ENABLE
	Rx1IsrFunction = RxFunc;
	
	if (RxFunc) {
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
	// TODO: (Nino) UART Send
	while ( !( EE_reg(UCSR0A) & (1<<UDRE0)) ); // Do nothing until UDR is ready for more data to be written to it
	EE_reg(UDR0) = data; // Echo back the received byte back to the computer 
	return 1;
}

EE_INT8 EE_uart_read_byte(EE_UINT8 port, EE_UINT8 *data)
{
	if (port != EE_UART_PORT_1)
		return -EE_UART_ERR_BAD_PORT;
		
#ifdef EE_UART_PORT_1_ISR_ENABLE
	if (Rx1IsrFunction == NULL) {
		/* Polling mode */
#endif
		// TODO: (Nino) Check UART Receive
		if(EE_reg(UCSR0A) & (1 << RXC0)) {	// if data have been recieved and is ready to be read from UDR
			*data = EE_reg(UDR0); // Fetch the recieved byte value into the variable "ByteReceived"
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
	if (Rx1IsrFunction != NULL) {
		/* Execute callback function */
		// TODO: (Nino) Call function with correct data
		//Rx1IsrFunction(U1RXREG & 0x00FF);
	}
	//IFS0bits.U1RXIF = 0;           
}
#endif
