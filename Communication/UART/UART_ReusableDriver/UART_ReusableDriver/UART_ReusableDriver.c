/*
 * UART_ReusableDriver.c
 *
 * Created: 4/10/2020 5:29:04 AM
 *  Author: Hamdy
 */ 


#include <avr/io.h>
#include "ATmega32A_UART.h"

int main(void)
{
	UART0.BaudRate=9600;
	UART0.EnableMode = USART_Tx_Rx;
	UART0.CommunicationMode = USART_Async_DoubleSpeed;
	UART0.CommunicationTerminal = USART_SingleProcessor;
	UART0.InterruptSource = USART_InterruptDisable;
	UART0.DataFrame = USART_7bitData;
	UART0.ControlFrame = USART_ParityDisable_2Stop;
	UART0.BaudRate = 9600;
	
	USART_Init();
	char x = 'C';
    while(1)
    {
        USART_SendByte_Blocking(x);
		x = USART_ReadByte_Blocking();
    }
}