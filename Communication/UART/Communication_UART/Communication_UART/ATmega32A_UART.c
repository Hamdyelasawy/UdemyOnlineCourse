/*
 * ATmega32A_UART.c
 *
 * Created: 4/9/2020 8:05:42 AM
 *  Author: Hamdy
 */ 

/*
 * BAUD = 9600
 * Stop = 1 Stop
 * parity = Disabled
 * Data = 7bit
 * Async
 */

#include "ATmega32A_UART.h"
#include <avr/io.h>

/* This Func Starts UART */
void UART_Init(void)
{
	/* According to The Equation in Data Sheet UBRR must be Set to 12 to have a 9600 BaudRate*/
	UBRRH = 0;
	UBRRL = 12;
	/* This Enabled Double Speed Communication */
	UCSRA = (1<<U2X);
	/* This Line Selects The Data Frame Width to be 7 bit Data */
	UCSRC = (1<<URSEL) | (1<<UCSZ1);
	/* This Line Enables UART Send And Rec pins */
	UCSRB = (1<<RXEN) | (1<<TXEN) ; 
}

/* This Function Sends Byte Through UART */
void UART_SendByte(unsigned char Byte)
{
	/* This Line Polls UDRE Flag to Check if Tx Register "UDR" Can Be Written*/
	while((UCSRA & (1<<UDRE)) == 0){ /* Do Nothing */}
	
	/* This Line Assigns Data to UDR Register to be Sent Using UART */
	UDR = Byte;
}

/* This Function Receive Byte Through UART*/
unsigned char UART_ReceiveByte(void)
{
	/* This Line Polls RXC Flag to Check if Rx Register "UDR" Can Be Read for New Incoming Data*/
	while((UCSRA & (1<<RXC)) == 0){ /* Do Nothing */}
		
	return UDR;
}
