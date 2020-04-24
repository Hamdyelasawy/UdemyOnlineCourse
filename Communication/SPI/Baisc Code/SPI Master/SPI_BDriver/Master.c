/*
 * SPI_BDriver.c
 *
 * Created: 4/18/2020 6:28:54 AM
 *  Author: Hamdy
 */ 


#include "uc_Configuration.h"
#include "SPI_ATMEGA32A.h"
#include "ATmega32A_UART.h"
#include <util/delay.h>
#include <avr/io.h>

int main(void)
{

	/* Configure USART */
	UART0.EnableMode = USART_Tx_Rx;
	UART0.BaudRate = 9600;
	UART0.CommunicationMode = USART_Async_DoubleSpeed;
	UART0.ControlFrame = USART_ParityDisable_1Stop;
	UART0.DataFrame = USART_8bitData;
	UART0.CommunicationTerminal= USART_SingleProcessor;
	UART0.InterruptSource = USART_InterruptDisable;
	USART_Init();
	SPI_Init();
    while(1)
    {
		/* Rx Data From UART */
        unsigned char Data = USART_ReadByte_Blocking();
		
		/* Out 0 to SS pin To Select uc */
		//PORTB &= ~(1<<4);
		SPI_SendByte(Data);
		/* Out 1 to SS pin To Terminate Communication */
		//PORTB |= (1<<4);
    }
}