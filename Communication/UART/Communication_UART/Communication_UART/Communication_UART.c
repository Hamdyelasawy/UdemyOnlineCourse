/*
 * Communication_UART.c
 *
 * Created: 4/9/2020 8:04:30 AM
 *  Author: Hamdy
 */ 

#define F_CPU (1000000U)
#include <avr/io.h>
#include "ATmega32A_UART.h"

int main(void)
{
	/* Init Code */
	
	UART_Init();
	char Variable = 'a';
    while(1)
    {
		UART_SendByte(Variable);
		Variable = UART_ReceiveByte();
    }
}

